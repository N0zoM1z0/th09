// AnmManager::SpriteHasTexture is an inline body shared by its callers.
// The /Ob0 ANM runtime unit sees it without expanding it, and the /O1
// Title draw unit emits the out-of-line copy kept at 0x00423469.
// Its 0x120-byte ANM file slots begin at manager offset +0x38.
struct AnmFileTextureSlot
{
    void *textures;
    unsigned char unknown004[0x11C];
};
typedef char AnmFileTextureSlotSizeIs120[(sizeof(AnmFileTextureSlot) == 0x120) ? 1 : -1];

inline int AnmManager::SpriteHasTexture(AnmVm *vm)
{
    if (vm->loadedSprite == NULL)
    {
        return 0;
    }
    if (vm->loadedSprite->anmIdx < 0)
    {
        return 0;
    }
    return reinterpret_cast<AnmFileTextureSlot *>(
        reinterpret_cast<unsigned int>(this) + 0x38 +
        static_cast<unsigned int>(vm->loadedSprite->anmIdx) * sizeof(AnmFileTextureSlot))->textures != NULL;
}
