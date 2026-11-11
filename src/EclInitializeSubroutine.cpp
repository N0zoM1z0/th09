#include "EclRunState.inl"

// The relocated runtime subroutine table uses native 32-bit pointer
// addresses even when a script supplies an extreme signed short index.
int EnemyEclManagerView::InitializeSubroutine(
    Th09EclContextView *context, short subroutineId)
{
    if (subroutineId >= 0)
    {
        context->currentInstruction004 =
            *reinterpret_cast<Th09EclRawInstructionHeaderView *const *>(
                reinterpret_cast<unsigned int>(subroutineTable04) +
                static_cast<unsigned int>(static_cast<int>(subroutineId)) *
                sizeof(subroutineTable04[0]));
        context->time008 = 0;
        context->secondaryTime094 = 0;
        context->subroutineId228 = subroutineId;
        context->selectedManager000 = this;
    }
    return 0;
}
