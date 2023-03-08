#include "S_H_Register.hlsli"

RWStructuredBuffer<tInitSetting> g_SBufferRW_InitSetting : register(e_u_INIT_SETTING);

[numthreads(1, 1, 1)]
void CS_HLSL_Init(uint3 DTID : SV_DispatchThreadID)
{
	if (all(uint3(0u, 0u, 0u) != DTID))
		return;

	
	//�ʱ�ȭ�ؾ� �ϴ� ���� ������ ���⼭ �ʱ�ȭ
	if (1u == g_SBufferRW_InitSetting[0].bIsLittleEndian)
	{
		for (uint i = 0u; i < 4u; ++i)
		{
			uint shift = 8u * i;
			g_SBufferRW_InitSetting[0].u8BitPartInU32Pack[i] = 0x000000FFu << shift;
			g_SBufferRW_InitSetting[0].u8BitShiftInU32Pack[i] = shift;
		}

		for (uint j = 0u; j < 2u; ++j)
		{
			uint shift = 16u * j;
			g_SBufferRW_InitSetting[0].u16BitPartInU32Pack[j] = 0x0000FFFFu << shift;
			g_SBufferRW_InitSetting[0].u16BitShiftInU32Pack[j] = shift;
		}
	}
	else
	{
		for (uint i = 0u; i < 4u; ++i)
		{
			uint shift = 8u * (3u - i);
			g_SBufferRW_InitSetting[0].u8BitPartInU32Pack[i] = 0x000000FFu << shift;
			g_SBufferRW_InitSetting[0].u8BitShiftInU32Pack[i] = shift;
		}

		for (uint j = 0u; j < 2u; ++j)
		{
			uint shift = 16u * (1u - j);
			g_SBufferRW_InitSetting[0].u16BitPartInU32Pack[j] = 0x0000FFFFu << shift;
			g_SBufferRW_InitSetting[0].u16BitShiftInU32Pack[j] = shift;
		}
	}
	
	
}