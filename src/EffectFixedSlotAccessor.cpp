struct EffectFixedSlotRecordView
{
    unsigned char bytes[0xD8];
};

struct EffectFixedSlotManagerView
{
    unsigned char unknown00[0x30];
    EffectFixedSlotRecordView *effects30;

    EffectFixedSlotRecordView *GetFixedSlotEffect(int slot);
};

EffectFixedSlotRecordView *
EffectFixedSlotManagerView::GetFixedSlotEffect(int slot)
{
    return &effects30[256 + slot];
}
