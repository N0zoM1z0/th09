#include "ExAttackRecordRuntimeView.hpp"
#include "GameManagerRuntimeView.hpp"

ExAttackGameManagerSideView *ExAttackRecordSideView::GetSideState()
{
    // Retail scales the signed side DWORD by the real GameManager
    // 0x38-byte side record stride, without a C++ array-bounds check.
    return reinterpret_cast<ExAttackGameManagerSideView *>(
        reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) +
        static_cast<unsigned int>(side08) * sizeof(g_GameManager.sides[0]));
}
