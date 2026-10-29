// Direct headless Ghidra target verification and bounded decompilation.
// @category TH09

import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.Arrays;
import java.util.HexFormat;

import com.google.gson.GsonBuilder;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.util.headless.HeadlessScript;
import ghidra.framework.Application;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.address.AddressRangeIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Program;

public class Th09Export extends HeadlessScript {
    private static final String TARGET_SHA256 =
        "10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822";

    private String sha256(byte[] bytes) throws Exception {
        return HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(bytes));
    }

    private void require(boolean condition, String message) {
        if (!condition) {
            throw new IllegalStateException(message);
        }
    }

    private void writeJson(Path path, JsonObject value) throws Exception {
        Files.writeString(path, new GsonBuilder().setPrettyPrinting().create().toJson(value) + "\n");
    }

    private JsonObject verifyTarget(JsonObject request) throws Exception {
        byte[] input = Files.readAllBytes(Path.of(request.get("input").getAsString()));
        require(sha256(input).equals(TARGET_SHA256), "disk target SHA-256 mismatch");
        require(TARGET_SHA256.equalsIgnoreCase(currentProgram.getExecutableSHA256()),
            "Ghidra Program target SHA-256 mismatch");
        require(currentProgram.getImageBase().getOffset() == 0x400000L,
            "Ghidra image base mismatch");
        require(currentProgram.getDefaultPointerSize() == 4 &&
            currentProgram.getLanguageID().toString().startsWith("x86:LE:32:"),
            "Ghidra target is not little-endian i386");
        require(currentProgram.getSymbolTable().isExternalEntryPoint(toAddr(0x47d45fL)),
            "Ghidra entry point mismatch");

        JsonArray checked = new JsonArray();
        for (JsonElement element : request.getAsJsonArray("file_ranges")) {
            JsonObject range = element.getAsJsonObject();
            int offset = range.get("file_offset").getAsInt();
            int length = range.get("length").getAsInt();
            require(offset >= 0 && length > 0 && (long)offset + length <= input.length,
                "invalid source file range");
            byte[] expected = Arrays.copyOfRange(input, offset, offset + length);
            byte[] mapped = new byte[length];
            Address address = toAddr(range.get("address").getAsString());
            require(currentProgram.getMemory().getBytes(address, mapped) == length,
                "incomplete Ghidra memory range: " + address);
            require(Arrays.equals(expected, mapped),
                "Ghidra mapped bytes differ from original file: " + address);
            JsonObject observation = range.deepCopy();
            observation.addProperty("sha256", sha256(mapped));
            checked.add(observation);
        }
        JsonObject result = new JsonObject();
        result.addProperty("target_sha256", TARGET_SHA256);
        result.addProperty("program_sha256", currentProgram.getExecutableSHA256());
        result.addProperty("program_input", currentProgram.getExecutablePath());
        result.addProperty("image_base", currentProgram.getImageBase().toString());
        result.addProperty("entry_point", "0047d45f");
        result.addProperty("language", currentProgram.getLanguageID().toString());
        result.addProperty("compiler_spec", currentProgram.getCompilerSpec().getCompilerSpecID().toString());
        result.addProperty("ghidra_version", Application.getApplicationVersion());
        result.add("file_ranges", checked);
        return result;
    }

    @Override
    public void run() throws Exception {
        String[] arguments = getScriptArgs();
        require(arguments.length == 2, "expected request.json and verify/export mode");
        JsonObject request = JsonParser.parseString(Files.readString(Path.of(arguments[0]))).getAsJsonObject();
        Path output = Path.of(request.get("output").getAsString());
        try {
            JsonObject binding = verifyTarget(request);
            if (arguments[1].equals("verify")) {
                writeJson(output.resolve("precheck.json"), binding);
                return;
            }
            require(arguments[1].equals("export"), "unknown export mode");
            require(!analysisTimeoutOccurred(), "Ghidra automatic analysis timed out");
            require(currentProgram.getOptions(Program.PROGRAM_INFO)
                .getBoolean(Program.ANALYZED_OPTION_NAME, false), "Ghidra analysis is incomplete");

            JsonObject report = new JsonObject();
            report.add("binding", binding);
            report.addProperty("analysis_complete", true);
            report.addProperty("exactness_credit", "none");
            JsonArray functions = new JsonArray();
            boolean complete = true;
            DecompInterface decompiler = new DecompInterface();
            try {
                require(decompiler.openProgram(currentProgram), "could not open Ghidra decompiler");
                for (JsonElement element : request.getAsJsonArray("functions")) {
                    Address address = toAddr(element.getAsString());
                    JsonObject item = new JsonObject();
                    item.addProperty("address", address.toString());
                    Function function = currentProgram.getFunctionManager().getFunctionAt(address);
                    if (function == null) {
                        item.addProperty("error", "no Ghidra function at requested entry");
                        complete = false;
                    }
                    else {
                        item.addProperty("name", function.getName());
                        item.addProperty("signature", function.getSignature().toString());
                        item.addProperty("body_bytes", function.getBody().getNumAddresses());
                        JsonArray ranges = new JsonArray();
                        AddressRangeIterator iterator = function.getBody().getAddressRanges();
                        while (iterator.hasNext()) {
                            AddressRange range = iterator.next();
                            JsonObject part = new JsonObject();
                            part.addProperty("start", range.getMinAddress().toString());
                            part.addProperty("end_inclusive", range.getMaxAddress().toString());
                            ranges.add(part);
                        }
                        item.add("body_ranges", ranges);
                        DecompileResults result = decompiler.decompileFunction(function, 90, monitor);
                        item.addProperty("decompile_complete", result.decompileCompleted());
                        item.addProperty("decompiler_message", result.getErrorMessage());
                        if (result.decompileCompleted() && result.getDecompiledFunction() != null) {
                            String filename = "function-" + address.toString() + ".c";
                            String code = result.getDecompiledFunction().getC();
                            Files.writeString(output.resolve(filename), code);
                            item.addProperty("pseudocode_file", filename);
                            item.addProperty("pseudocode_sha256", sha256(code.getBytes(java.nio.charset.StandardCharsets.UTF_8)));
                        }
                        else {
                            complete = false;
                        }
                    }
                    functions.add(item);
                }
            }
            finally {
                decompiler.dispose();
            }
            report.add("functions", functions);
            report.addProperty("requested_functions_complete", complete);
            writeJson(output.resolve("report.json"), report);
            require(complete, "one or more requested functions could not be decompiled");
        }
        catch (Exception error) {
            JsonObject failure = new JsonObject();
            failure.addProperty("error", error.toString());
            writeJson(output.resolve("failure.json"), failure);
            setHeadlessContinuationOption(isImporting() ?
                HeadlessContinuationOption.ABORT_AND_DELETE : HeadlessContinuationOption.ABORT);
            throw error;
        }
    }
}
