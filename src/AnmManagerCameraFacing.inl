// A full three-word vector copy of the actual subtraction result. The
// source words are all loaded before the destination is touched. Keeping the
// stores as ordinary unsigned words lets VC7.1 preserve the target's initial
// X store and the immediately following delta-address calculation. Its two
// trailing stores still differ in order from the original Y-then-Z schedule.
// This is a compiler-observed source model, not a retail C++ spelling claim.
static __forceinline void CopyCameraFacingProjectionDelta(
    Float3 &dst, const Float3 &src)
{
    const unsigned int *sourceWords =
        reinterpret_cast<const unsigned int *>(&src);
    unsigned int *targetWords = reinterpret_cast<unsigned int *>(&dst);
    unsigned int x = sourceWords[0];
    unsigned int y = sourceWords[1];
    unsigned int z = sourceWords[2];
    targetWords[2] = z;
    targetWords[0] = x;
    targetWords[1] = y;
}

// TH09 camera-facing projection owner; included after the exact TranslateRotation
// implementation in the native AnmManager render translation unit.
// Three simultaneously live 12-byte projection vectors form a typed scratch
// area. The verified TH09 original uses contiguous origin/delta/reference
// stack slots; this workspace recovers those physical addresses in VC7.1.
// Its layout is a compiler-observed source hypothesis, not proof of the
// original C++ declaration or translation-unit boundaries.
struct CameraProjectionScratch
{
    Float3 origin;
    Float3 delta;
    D3DXVECTOR3 reference;
    CameraProjectionScratch() : origin(0.0f, 0.0f, 0.0f) {}
};
typedef char CameraProjectionScratchSizeIs24[
    (sizeof(CameraProjectionScratch) == 0x24) ? 1 : -1];
typedef char CameraProjectionScratchDeltaAt0C[
    (offsetof(CameraProjectionScratch, delta) == 0x0C) ? 1 : -1];
typedef char CameraProjectionScratchReferenceAt18[
    (offsetof(CameraProjectionScratch, reference) == 0x18) ? 1 : -1];
int AnmManager::ProjectCameraFacingQuad(AnmVm *vm)
{
    float rotation,sine,cosine,xOffset,yOffset,halfHeight,halfWidth;
    D3DXMATRIX worldMatrix;
    D3DXVECTOR3 projectedPosition;
    float *position;
    AnmVmProjectionView *projectionVm=reinterpret_cast<AnmVmProjectionView *>(vm);
    rotation=projectionVm->rotation.z;
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
#error TH09 ProjectCameraFacingQuad requires target-observed x87 FSINCOS.
#endif
    CameraProjectionScratch scratch;
    D3DXMatrixIdentity(&worldMatrix);
    position=projectionVm->pos.operator float *();
    worldMatrix._41=position[0]; worldMatrix._42=position[1]; worldMatrix._43=position[2];
    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&projectedPosition),reinterpret_cast<D3DXVECTOR3 *>(&scratch.origin),&g_AnmProjectionContext->viewport,&g_AnmProjectionContext->projectionMatrix,&g_AnmProjectionContext->viewMatrix,&worldMatrix);
    if (projectedPosition.z<0.0f || projectedPosition.z>1.0f) return -1;
    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&scratch.reference),reinterpret_cast<D3DXVECTOR3 *>(&g_AnmProjectionContext->cameraRight),&g_AnmProjectionContext->viewport,&g_AnmProjectionContext->projectionMatrix,&g_AnmProjectionContext->viewMatrix,&worldMatrix);
    CopyCameraFacingProjectionDelta(scratch.delta,*reinterpret_cast<Float3 *>(&scratch.reference) -
                   *reinterpret_cast<Float3 *>(&projectedPosition));
    xOffset=D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&scratch.delta))*0.5f;
    halfWidth=xOffset*vm->spriteSize.x*vm->scale.x;
    halfHeight=xOffset*vm->spriteSize.y*vm->scale.y;
    xOffset=projectedPosition.x; yOffset=projectedPosition.y;
    this->TranslateRotation(&g_AnmRenderQuad[0],-halfWidth,-halfHeight,sine,cosine,xOffset,yOffset);
    this->TranslateRotation(&g_AnmRenderQuad[1],halfWidth,-halfHeight,sine,cosine,xOffset,yOffset);
    this->TranslateRotation(&g_AnmRenderQuad[2],-halfWidth,halfHeight,sine,cosine,xOffset,yOffset);
    this->TranslateRotation(&g_AnmRenderQuad[3],halfWidth,halfHeight,sine,cosine,xOffset,yOffset);
    g_AnmRenderQuad[0].z=g_AnmRenderQuad[1].z=g_AnmRenderQuad[2].z=g_AnmRenderQuad[3].z=projectedPosition.z;
    if ((vm->anchor & 1) != 0) {
        g_AnmRenderQuad[0].x += halfWidth;
        g_AnmRenderQuad[1].x += halfWidth;
        g_AnmRenderQuad[2].x += halfWidth;
        g_AnmRenderQuad[3].x += halfWidth;
    }
    if ((vm->anchor & 2) != 0) {
        g_AnmRenderQuad[0].y += halfHeight;
        g_AnmRenderQuad[1].y += halfHeight;
        g_AnmRenderQuad[2].y += halfHeight;
        g_AnmRenderQuad[3].y += halfHeight;
    }
    return 0;
}
