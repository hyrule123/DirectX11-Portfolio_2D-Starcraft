#include "S_0_H_STD2DLight.hlsli"

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light);
void CalcLight2DNormal(float3 _vWorldPos, float3 _vNormalDir, inout tLightColor _Light);

float4 PS_std2D_Light(VS_OUT _in) : SV_TARGET
{
    float4 vOutColor = (float4)0.f;
    
	tMtrlScalarData Data = GetMtrlScalarData(_in.uInstID);
    
    //�����ؽ�ó�� �������� �������� ������ ����Ÿ ������ return;
    if(FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
    {
        vOutColor = float4(1.f, 1.f, 0.f, 1.f);
    }
    
    //�ִϸ��̼� ������ϰ��
	else if (TRUE == Data.SHADER_STD2DMTRL_bAnimUse)
	{

		if (eMTRL_SCALAR_STD2D_FLAG::USE_ANIM & Data.MTRL_SCALAR_STD2D_FLAG)
		{
			float2 vUV = _in.vUV;
			if (eMTRL_SCALAR_STD2D_FLAG::NEED_FLIP_X & Data.MTRL_SCALAR_STD2D_FLAG)
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
	if (eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		if (all(vOutColor.rgb == Data.MTRL_SCALAR_STD2D_COLORKEY.rgb))
			discard;
	}
        
        
    //�븻���� ���� ��� �� ó���� ���ش�. �븻���� Tex 1���� ����Ǿ� �ִ�.
    //���� ���忡 ��ġ�Ǿ��ִ� ������ ������ tGlobal ������ۿ�,
    //���� ���� �����ʹ� ����ȭ ���ۿ� ��ġ�Ǿ� �ִ�.
    float3 vNormal = (float3) 0.f;
    BOOL bNormalMapProcessed = FALSE;
    if (TRUE == g_CBuffer_Mtrl_Tex.bTEX_1)
    {
		bNormalMapProcessed = TRUE;
        
        vNormal = g_tex_1.Sample(g_Sampler_0, _in.vUV).xyz;
            
        //��ָ��� ������ ������ '����'(-1 ~ 1), ���� ���̴��� ���� ������ R8G8B8A8 UNORM(0 ~ 1)
        vNormal = (vNormal * 2.f) - 1.f;
            
        //���� ��ָ��� DX ��ǥ��� y��� z���� �ٸ��� �Ǿ� ����.
        vNormal.y = -vNormal.y;

        //��ָ��� ���� ���⿡ �ȼ��� ���� ����� ���� �� normalize ���ش�.
        //�� �� ������ǥ�� 0�̹Ƿ� ������� ��ǥ���� �ƴ� ���Ͱ��� �ȴ�.
        //������� �ش� �ȼ��� ȸ�� ����� �ݿ��Ǿ� ������ �ٶ󺸰� �ִ� ������ �ȴ�.
		vNormal = normalize(mul(float4(vNormal, 0.f), Data.MTRL_SCALAR_MAT_WORLD).xyz);
    }
        

    tLightColor LightColor = (tLightColor) 0.f;
    //�븻���� �����Ͷ��, �� �븻�� �ؽ�ó�� ��� 0.f�� �ʱ�ȭ�� ���� �״�ζ��
    if (FALSE == bNormalMapProcessed)
    {
        //�ڽ��� ��ְ��� �������� ���� ���� ����� ����
        CalcLight2D(_in.vWorldPos, LightColor);
    }
    else
    {     
        //��ְ����� �ջ�� ���� ����� ����.
        CalcLight2DNormal(_in.vWorldPos, vNormal, LightColor);
    }
            
    //���������� ���� ���� ������ �ȼ� ���� �����ش�.
    vOutColor.rgb *= (LightColor.vDiffuse.rgb + LightColor.vAmbient.rgb);

	
    return vOutColor;
}

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light)
{
	for (uint i = 0; i < g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::LIGHT2D].uSBufferCount; ++i)
    {  
        if (eLIGHT_TYPE::DIRECTIONAL == g_SBuffer_Light2D[i].LightType)
        {
            _Light.vAmbient.rgb += g_SBuffer_Light2D[i].LightColor.vAmbient.rgb;
            _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb;
        }
        else if (eLIGHT_TYPE::POINT == g_SBuffer_Light2D[i].LightType)
        {
            //�ȼ��� ���� ������ ���̸� ����ϰ�, �װ� �������� ����(fRadius)�� �����ش�.
            float DistancePow = distance(_vWorldPos.xy, g_SBuffer_Light2D[i].vLightWorldPos.xy) / g_SBuffer_Light2D[i].fRadius;
            
            //�� ���� 1���� �� �ְ�(�־������� ���� �۾������� - ������ �������� ���� �۾�����.)
            //saturate �Լ��� ���ؼ� ������ 0~1 ���̷� �����Ѵ�.(���� ���ϴٰ� ���� �������� ���� �ƴϹǷ�)
            DistancePow = saturate(1.f - DistancePow);
            
            //�Ÿ��� ���� ���� ������ ������� �� �� ���� Diffuse ���� �־��ش�.
            //�������� ��� ȯ�汤(Ambient)�� ���ٰ� �����Ѵ�.
            _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb * DistancePow;
        }
        else if (eLIGHT_TYPE::SPOTLIGHT == g_SBuffer_Light2D[i].LightType)
        {
            //����Ʈ����Ʈ�� ��쿡�� �߰������� ���� ����� ��ä���� ������ �����Ѵ�. �� ���� ������ acos�� ���ؼ� ���� �� �ִ�.
            //�ȼ��� ��ġ�� ������ ��ġ�� �� �� ����ȭ �Ͽ� �������κ��� �ȼ������� ���� ���͸� ���Ѵ�.
            float2 LightToPixelDir = normalize(_vWorldPos.xy - g_SBuffer_Light2D[i].vLightWorldPos.xy);
            
            //������ ���� ���� ���Ϳ� ���� ���� ���͸� �����ϸ� ���հ��� �ڻ��ΰ��� ���� �� �ִ�.
            float Angle = acos(dot(LightToPixelDir, g_SBuffer_Light2D[i].vLightDir.xy));
            

            //���� ������ ����Ʈ����Ʈ �������� ������ ��ä���� �������� ���� ��쿡�� ó�����ش�.
            if(Angle < g_SBuffer_Light2D[i].fAngle)
            {
                //������ ���� ���� ���⸦ ����Ѵ�.
                float AnglePow = saturate(1.f - (Angle / g_SBuffer_Light2D[i].fAngle));

                float DistancePow = distance(_vWorldPos.xy, g_SBuffer_Light2D[i].vLightWorldPos.xy) / g_SBuffer_Light2D[i].fRadius;
                DistancePow = saturate(1.f - DistancePow);
            
                _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb * DistancePow * AnglePow;
            }

        }
        
    }

}

void CalcLight2DNormal(float3 _vWorldPos, float3 _vNormalDir, inout tLightColor _Light)
{
    //���⼱ �븻������ ��������� ��.    
    for (uint i = 0; i < g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::LIGHT2D].uSBufferCount; ++i)
    {
        if (eLIGHT_TYPE::DIRECTIONAL == g_SBuffer_Light2D[i].LightType)
        {
        //����Ʈ �ڻ��� ��Ģ�� ���� ���� ����(=�ڻ��ΰ�)�� ���Ѵ�.
            float DiffusePow = saturate(dot(-g_SBuffer_Light2D[i].vLightDir.xyz, _vNormalDir));
            
        //���籤���� ��� ���⸸ ����
            _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb * DiffusePow;
            _Light.vAmbient.rgb += g_SBuffer_Light2D[i].LightColor.vAmbient.rgb;
        }
        else if (eLIGHT_TYPE::POINT == g_SBuffer_Light2D[i].LightType)
        {
        //�������� ���� ���� ��ġ�κ��� ���� �������� ���������ٰ� �����Ѵ�. �¾��� ���̶�� �����ϸ� �ɵ�.
            
        //�������� ���� �������� ���� �����Ƿ� ������ �ȼ� ������ ���⺤�Ͱ� �� ���� ���� �����̴�.
            float3 LightToPixelVector = _vWorldPos - g_SBuffer_Light2D[i].vLightWorldPos.xyz;
            
        //�켱 �Ÿ��� ���� ���� ������ ����Ѵ�.
            float DistancePow = saturate(
        1.f - (length(LightToPixelVector.xy) / g_SBuffer_Light2D[i].fRadius));
            
        //���� ���� �ʴ� �ȼ� ��� continue
            if (DistancePow <= 0.f)
                continue;
            
        //�븻 ���Ϳ� ���� ���� ���� ������ �ڻ��ΰ��� ���Ѵ�.
            float DiffusePow = saturate(dot(-LightToPixelVector, _vNormalDir));
            
            _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb * DiffusePow * DistancePow;

        }
        else if (eLIGHT_TYPE::SPOTLIGHT == g_SBuffer_Light2D[i].LightType)
        {
            
            float DiffusePow = saturate(dot(-g_SBuffer_Light2D[i].vLightDir.xyz, _vNormalDir));
            
        //�������� �ȼ������� ���� ����
            float2 LightToPixelDir = _vWorldPos.xy - g_SBuffer_Light2D[i].vLightWorldPos.xy;
            
            float2 LightToPixelDirNorm = normalize(LightToPixelDir);
           
            
        //������ ����� �������� �ȼ������� ���⺤�͸� �����ϰ�, ��ũ�ڻ����� ���ؼ� ���� ����
        //�� �� ������ ��ũ�ڻ��� ���� ������ ����̴�.(0 ~ PI) ��ȣ�� ���� ������ ��ȯ��.
            float Angle = acos(dot(LightToPixelDirNorm, g_SBuffer_Light2D[i].vLightDir.xy));
            
        //�ݰ� 90�� ���� ��쿡�� ó��
            if (Angle < g_SBuffer_Light2D[i].fAngle)
            {
            //�Ʊ� ���� �������� �ȼ������� ���͸� ���� �Ÿ� ���
                float DistancePow = saturate(1.f - (length(LightToPixelDir) / g_SBuffer_Light2D[i].fRadius));
                
            //saturate �� �ʿ� ����. Angle�� �� �� ������ �����. ���� ���� ���ǹ� ������ Angle / Light.fAngle ���� 1�� �Ѿ �� ����.
                float AnglePow = 1.f - (Angle / g_SBuffer_Light2D[i].fAngle);

                _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb * DiffusePow * DistancePow * AnglePow;
            }
            
        }

    }

}