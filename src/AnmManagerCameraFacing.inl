// TH09 camera-facing projection owner; included after the exact TranslateRotation
// implementation in the native AnmManager render translation unit.
int AnmManager::ProjectCameraFacingQuad(AnmVm *vm)
{
    float rotation,sine,cosine,xOffset,yOffset,halfHeight,halfWidth;
    D3DXMATRIX worldMatrix;
    D3DXVECTOR3 projectedPosition;
    D3DXVECTOR3 projectedReference;
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
    Float3 origin(0.0f, 0.0f, 0.0f);
    D3DXMatrixIdentity(&worldMatrix);
    position=projectionVm->pos.operator float *();
    worldMatrix._41=position[0]; worldMatrix._42=position[1]; worldMatrix._43=position[2];
    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&projectedPosition),reinterpret_cast<D3DXVECTOR3 *>(&origin),&g_AnmProjectionContext->viewport,&g_AnmProjectionContext->projectionMatrix,&g_AnmProjectionContext->viewMatrix,&worldMatrix);
    if (projectedPosition.z<0.0f || projectedPosition.z>1.0f) return -1;
    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&projectedReference),reinterpret_cast<D3DXVECTOR3 *>(&g_AnmProjectionContext->cameraRight),&g_AnmProjectionContext->viewport,&g_AnmProjectionContext->projectionMatrix,&g_AnmProjectionContext->viewMatrix,&worldMatrix);
    Float3 delta;
    delta = *reinterpret_cast<Float3 *>(&projectedReference) -
                   *reinterpret_cast<Float3 *>(&projectedPosition);
    xOffset=D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&delta))*0.5f;
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
