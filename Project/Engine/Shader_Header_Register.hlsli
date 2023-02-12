#ifndef SHADER_HEADER_REGISTER
#define SHADER_HEADER_REGISTER

#include "Shader_Header_Struct.hlsli"

#ifdef __cplusplus
#define REGISTER_IDX(RegisterType, Idx) Idx
#else
#define REGISTER_IDX(RegisterType, Idx) RegisterType##Idx
#endif

//==========================================
//	   CONSTANT BUFFERS ( b Register )
//==========================================

//Register Number
#define e_b_CBUFFER_TRANSFORM				REGISTER_IDX(b, 0)
#define e_b_CBUFFER_MATERIAL				REGISTER_IDX(b, 1)
#define e_b_CBUFFER_GLOBAL					REGISTER_IDX(b, 2)
#define e_b_CBUFFER_SBUFFER_SHAREDATA		REGISTER_IDX(b, 3)
#define e_b_CBUFFER_PARTICLE_MODULEDATA		REGISTER_IDX(b, 4)
#define e_b_END								5

	
//eCBUFFER_IDX_SBUFFER_SHAREDATA Inner Index
//��� ���� 'SBUFFERINFO' ������ �ε��� ��ȣ�� �����ϴ� ����ü
ENUM_START(eCBUFFER_SBUFFER_SHAREDATA_IDX, int)
	ENUM_MEMBER(NONE, int, -1)
	ENUM_MEMBER(LIGHT2D, int, 0)
	ENUM_MEMBER(TILE, int, 1)
	ENUM_MEMBER(SETCOLOR, int, 2)
	ENUM_MEMBER(PARTICLE, int, 3)
	ENUM_MEMBER(END, int, 4)
ENUM_END


#ifdef __cplusplus
	

//�Ʒ��� CBuffer_SBUFFER_SHARED_DATA�� ���޵Ǵ� �������
extern tSBufferInfo g_arrSBufferShareData[(int)eCBUFFER_SBUFFER_SHAREDATA_IDX::END];

#else


cbuffer CBuffer_Transform : register(e_b_CBUFFER_TRANSFORM)
{
	tTransform g_CBuffer_Transform;
};

cbuffer CBuffer_Material : register(e_b_CBUFFER_MATERIAL)
{
	tMtrlData g_CBuffer_MtrlData;
};


	//������ ���� ������ �Ѱ��ֱ� ���� �������
cbuffer CBuffer_Global : register(e_b_CBUFFER_GLOBAL)
{
	tGlobalValue g_CBuffer_GlobalData;
};
	 
cbuffer CBuffer_SBUFFER_SHARED_DATA : register(e_b_CBUFFER_SBUFFER_SHAREDATA)
{
	tSBufferInfo g_SBufferInfo[eCBUFFER_SBUFFER_SHAREDATA_IDX::END];
}


cbuffer CBuffer_ParticleModule : register(e_b_CBUFFER_PARTICLE_MODULEDATA)
{
	tParticleModule g_CBuffer_ParticleModule;
}


	#endif




//==================================================
// SRV : Texture / Structured Buffers ( t Register )
//==================================================

#define e_t_TEXTURE_0 REGISTER_IDX(t, 0)
#define e_t_TEXTURE_1 REGISTER_IDX(t, 1)
#define e_t_TEXTURE_2 REGISTER_IDX(t, 2)
#define e_t_TEXTURE_3 REGISTER_IDX(t, 3)
#define e_t_TEXTURE_4 REGISTER_IDX(t, 4)
#define e_t_TEXTURE_5 REGISTER_IDX(t, 5)
#define e_t_TEXTURE_6 REGISTER_IDX(t, 6)
#define e_t_TEXTURE_7 REGISTER_IDX(t, 7)

#ifdef __cplusplus
#define eMTRLDATA_TEXPARAM_END 8
#endif

#define e_t_TEXTURE_CUBE_0 REGISTER_IDX(t, 8)
#define e_t_TEXTURE_CUBE_1 REGISTER_IDX(t, 9)

#define e_t_TEXTURE_ARRAY_0 REGISTER_IDX(t, 10)
#define e_t_TEXTURE_ARRAY_1 REGISTER_IDX(t, 11)

#define e_t_SBUFFER_LIGHT2D				REGISTER_IDX(t, 12)
#define e_t_SBUFFER_TILE				REGISTER_IDX(t, 13)
#define e_t_SBUFFER_SETCOLOR			REGISTER_IDX(t, 14)
#define e_t_SBUFFER_PARTICLE_TRANSFORM	REGISTER_IDX(t, 15)

#define e_t_TEXUTRE_NOISE				REGISTER_IDX(t, 16)


	#ifdef __cplusplus

#define e_t_SRV_NONE -1


	#else

Texture2D g_tex_0 : register(e_t_TEXTURE_0);
Texture2D g_tex_1 : register(e_t_TEXTURE_1);
Texture2D g_tex_2 : register(e_t_TEXTURE_2);
Texture2D g_tex_3 : register(e_t_TEXTURE_3);
Texture2D g_tex_4 : register(e_t_TEXTURE_4);
Texture2D g_tex_5 : register(e_t_TEXTURE_5);
Texture2D g_tex_6 : register(e_t_TEXTURE_6);
Texture2D g_tex_7 : register(e_t_TEXTURE_7);

TextureCube g_cube_0 : register(e_t_TEXTURE_CUBE_0);
TextureCube g_cube_1 : register(e_t_TEXTURE_CUBE_1);

Texture2DArray g_arrtex_0 : register(e_t_TEXTURE_ARRAY_0);
Texture2DArray g_arrtex_1 : register(e_t_TEXTURE_ARRAY_1);


StructuredBuffer<tLightInfo> g_SBuffer_Light2D : register(e_t_SBUFFER_LIGHT2D);
StructuredBuffer<tTile> g_SBuffer_Tile : register(e_t_SBUFFER_TILE);
StructuredBuffer<float4> g_SBuffer_SetColor : register(e_t_SBUFFER_SETCOLOR);
StructuredBuffer<tParticleTransform> g_SBuffer_ParticleTransform : register(e_t_SBUFFER_PARTICLE_TRANSFORM);
Texture2D g_Tex_Noise : register(e_t_TEXUTRE_NOISE);


	#endif


//=========================================================
// UAV : RW Structured Buffer / RW Texture ( u Register)
//=========================================================
//UAV -> u Register


//u �������ʹ� DX11 ���� 0 ~ 7�������� �����ϹǷ� ����
#define e_u_SETCOLOR_TEXTURERW			REGISTER_IDX(u, 0)
#define e_u_SETCOLOR_SBUFFERRW			REGISTER_IDX(u, 1)
#define e_u_PARTICLE_SBUFFERRW			REGISTER_IDX(u, 0)
#define e_u_PARTICLE_SBUFFERRW_SHAREDATA	REGISTER_IDX(u, 1)

	#ifdef __cplusplus

#define e_u_UAV_NONE -1

	#else

//���� ����ϴ� ���̴����� ������ �����Ͽ���.
//u0
////g_TexRW_Output
////g_SBufferRW_Particle

//u1
////g_SBufferRW_Test
////g_SBufferRW_Particle_Shared

	#endif


//==================================
//			SAMPLER
//==================================

#define e_s_SAMPLER_ANISOTROPIC REGISTER_IDX(s, 0)
#define e_s_SAMPLER_POINT		REGISTER_IDX(s, 1)

#ifdef __cplusplus

#define e_s_SAMPLER_END 2

#else

SamplerState g_Sampler_0 : register(e_s_SAMPLER_ANISOTROPIC);
SamplerState g_Sampler_1 : register(e_s_SAMPLER_POINT);

#endif






//==================================
//				ETC
//==================================
#ifdef __cplusplus



#else


#endif


#endif