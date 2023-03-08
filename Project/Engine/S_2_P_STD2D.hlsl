
#include "S_0_H_STD2D.hlsli"

#include "S_H_Func.hlsli"


float4 PS_std2D(VS_OUT _in) : SV_TARGET
{
    float4 vOutColor = (float4) 0.f;
    
	tMtrlScalarData Data = (tMtrlScalarData) 0.f;
    
	if (0 == g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR].uSBufferCount)
	{
		Data = g_CBuffer_Mtrl_Scalar;
	}
	else
	{
		Data = g_SBuffer_Mtrl_Scalar[_in.uInstID];
	}
    
    
    //�����ؽ�ó�� �������� �������� ������ ����Ÿ ������ return;
	if (FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
	{
		vOutColor = float4(1.f, 0.f, 1.f, 1.f);
	}
    
    //�ִϸ��̼� ������ϰ��
    else if (eMTRL_SCALAR_STD2D_FLAG::USE_ANIM & Data.MTRL_SCALAR_STD2D_FLAG)
    {
		float2 vUV = _in.vUV;
        if(eMTRL_SCALAR_STD2D_FLAG::NEED_FLIP_X & Data.MTRL_SCALAR_STD2D_FLAG)
			vUV.x = 1.f - vUV.x;
        
        //�ִϸ��̼��� Left Top���� Slice�� �ڽ��� UV���� ���ؼ� ���� UV���� �����ش�.
		float2 RealUV = Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP + Data.MTRL_SCALAR_STD2D_ANIM_UV_SLICE * vUV;

		int idx = Data.MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX;
		vOutColor = SampleMtrlTex(idx, g_Sampler_0, RealUV);
	}
    else
    {
        vOutColor = g_tex_0.Sample(g_Sampler_0, _in.vUV);
    }
	
	//Alpha Check 
	if (0.f == vOutColor.a)
	{
		discard;
	}
    
	//ColorKey Check
	if(eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		if(all(vOutColor.rgb == Data.MTRL_SCALAR_STD2D_COLORKEY.rgb))
			discard;
	}
	

    
    return vOutColor;
}