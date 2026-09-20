#include "EclRunState.inl"

int Th09EclRunState::ManagerStateView::InitializeSubroutine(
    Th09EclContextView *context, short subroutineId)
{
    if (subroutineId >= 0)
    {
        Th09EclRawInstructionHeaderView **subroutineInstructions =
            *reinterpret_cast<Th09EclRawInstructionHeaderView ***>(
                reinterpret_cast<unsigned char *>(this) + 0x04);
        context->currentInstruction004 =
            subroutineInstructions[subroutineId];
        context->time008 = 0;
        context->secondaryTime094 = 0;
        context->subroutineId228 = subroutineId;
        *reinterpret_cast<Th09EclRunState::ManagerStateView **>(context) = this;
    }
    return 0;
}
