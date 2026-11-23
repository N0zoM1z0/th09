"""Read named members of Microsoft COFF archives."""

from pathlib import Path


def checked_slice(data: bytes, offset: int, size: int, what: str) -> bytes:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise ValueError(f"truncated {what}")
    return data[offset : offset + size]


def archive_members(path: Path) -> list[tuple[str, bytes]]:
    data = path.read_bytes()
    if not data.startswith(b"!<arch>\n"):
        raise ValueError(f"not a COFF archive: {path}")
    records: list[tuple[str, bytes]] = []
    long_names = b""
    offset = 8
    while offset < len(data):
        header = checked_slice(data, offset, 60, f"archive header in {path.name}")
        if header[58:60] != b"`\n":
            raise ValueError(f"invalid archive header in {path.name}")
        raw_name = header[:16].decode("ascii", errors="strict").rstrip()
        try:
            size = int(header[48:58].decode("ascii").strip())
        except ValueError as exc:
            raise ValueError(f"invalid archive member size in {path.name}") from exc
        payload = checked_slice(data, offset + 60, size, f"archive member in {path.name}")
        records.append((raw_name, payload))
        if raw_name == "//":
            long_names = payload
        offset += 60 + size
        if offset & 1:
            offset += 1
    if offset != len(data):
        raise ValueError(f"misaligned archive tail in {path.name}")

    result: list[tuple[str, bytes]] = []
    for raw_name, payload in records:
        if raw_name in {"/", "//"}:
            continue
        if raw_name.startswith("#1/"):
            name_size = int(raw_name[3:])
            name = checked_slice(payload, 0, name_size, "BSD archive name").decode(
                "utf-8", errors="replace"
            )
            payload = payload[name_size:]
        elif raw_name.startswith("/") and raw_name[1:].isdigit():
            name_offset = int(raw_name[1:])
            if name_offset >= len(long_names):
                raise ValueError(f"invalid long-name offset in {path.name}")
            tail = long_names[name_offset:]
            terminators = [value for value in (tail.find(b"/\n"), tail.find(b"\x00")) if value >= 0]
            end = min(terminators) if terminators else len(tail)
            name = tail[:end].decode("utf-8", errors="replace")
        else:
            name = raw_name.rstrip("/")
        result.append((name, payload))
    return result
