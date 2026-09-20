struct ExAttackGameManagerSideView
{
    unsigned char bytes[0x38];
};

struct ExAttackGameManagerView
{
    ExAttackGameManagerSideView sides[2];
};

extern ExAttackGameManagerView g_GameManager;

struct ExAttackRecordSideView
{
    int unknown00;
    int opponentSide04;
    int side08;

    ExAttackGameManagerSideView *GetSideState();
};

ExAttackGameManagerSideView *ExAttackRecordSideView::GetSideState()
{
    return &g_GameManager.sides[side08];
}
