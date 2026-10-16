#include "ExAttackRecordRuntimeView.hpp"

extern ExAttackGameManagerView g_GameManager;

ExAttackGameManagerSideView *ExAttackRecordSideView::GetSideState()
{
    return &g_GameManager.sides[side08];
}
