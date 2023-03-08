#include "S_0_H_TilemapAtlas.hlsli"


// pixel shader
float4 PS_Tilemap_Atlas(VTX_TILEMAP_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // �Ҽ���Ʈ, frac(_in.vUV) : Ÿ�� ��ĭ ������ �ȼ��� ������� ��ġ (0 ~ 1)
    // ������Ʈ, floor(_in.vUV): ��ü Ÿ�� �߿��� �ε���(��, ��)    
    int2 TileIdx = floor(_in.vUV);
    int BufferIdx = g_CBuffer_Mtrl_Scalar.INT_0 * TileIdx.y + TileIdx.x;
    float2 vUV = g_SBuffer_Tile[BufferIdx].vLeftTop + (g_SBuffer_Tile[BufferIdx].vSlice * frac(_in.vUV));
    
	if (g_CBuffer_Mtrl_Tex.bTEX_0)
    {
        vOutColor = g_tex_0.Sample(g_Sampler_1, vUV);
    }
    
    return vOutColor;
}