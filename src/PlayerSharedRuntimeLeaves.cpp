struct PlayerSharedRuntimeView
{
    int IsBlocked();
    unsigned char unknown0000[0x1095C];
    int updateBlock1095C;
    int drawCounter10960;
};

int PlayerSharedRuntimeView::IsBlocked()
{
    int value = *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(this) + 0xE94C);
    if (value >= 0 || value == -2)
        return 1;
    return 0;
}
