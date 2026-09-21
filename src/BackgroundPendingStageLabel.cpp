#include "Background.hpp"

void Background::ApplyPendingStageLabel()
{
    int pending = this->pendingStageLabel18;
    if (pending != 0)
    {
        RawStageInstr *instruction = this->stageScript;
        int index = 0;
        this->stageScriptInstructionIndex = 0;

        while (instruction->opcode != 31 || pending != instruction->args[0])
        {
            if (instruction->frame == -1)
                return;
            ++instruction;
            ++index;
        }

        if (instruction->frame != -1)
        {
            this->stageScriptInstructionIndex = index + 1;
            this->stageScriptTimer = instruction->frame;
            this->pendingStageLabel18 = 0;
        }
    }
}
