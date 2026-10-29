#include "ExAttackRecordRuntimeView.hpp"
#include "GameManagerRuntimeView.hpp"

ExAttackGameManagerSideView *ExAttackRecordSideView::GetSideState()
{
    return reinterpret_cast<ExAttackGameManagerSideView *>(
        &g_GameManager.sides[side08]);
}
