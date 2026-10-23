#include "EclRunState.inl"

int EnemyEclManagerView::InitializeSubroutine(
    Th09EclContextView *context, short subroutineId)
{
    if (subroutineId >= 0)
    {
        Th09EclRawInstructionHeaderView **subroutineInstructions =
            reinterpret_cast<Th09EclRawInstructionHeaderView **>(
                subroutineTable04);
        context->currentInstruction004 =
            subroutineInstructions[subroutineId];
        context->time008 = 0;
        context->secondaryTime094 = 0;
        context->subroutineId228 = subroutineId;
        context->selectedManager000 = this;
    }
    return 0;
}
