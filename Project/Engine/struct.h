#pragma once

struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

typedef tVertex Vtx;


struct tMtrlConst
{
	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];
	int arrbTex[eTEX_PARAM::eTEX_END];
};

struct tTransform
{
	Matrix matViewProj;
};

extern tTransform g_transform;

struct tEvent
{
	eEVENT_TYPE Type;
	DWORD_PTR	lParam;
	DWORD_PTR	rParam;
};

struct tDebugShapeInfo
{
	eSHAPE_TYPE	eShape;
	float		fLifeSpan;
	Vec2		bytepadding;
	Matrix		matWorld;
	Vec4		vColor;
};

struct tSquareInfo
{
	float LB_X;
	float LB_Y;
	float Size;
};

struct tRectInfo
{
	Vec2 LB;
	Vec2 RT;
};

typedef Vector4 tRectLBRT;

struct tLightColor
{
	Vec4 vDiffuse;
	Vec4 vAmbient;
};

struct tLightInfo
{
	tLightColor LightColor;

	Vec4 vLightWorldPos;

	Vec4 vLightDir;	//직사광선 또는 스포트라이트의 방향

	float fRadius;	//점광원 또는 스포트라이트의 거리
	float fAngle;
	UINT LightType;
	int padding;
};

enum class eLIGHT_TYPE : UINT
{
	eLIGHT_DIRECTIONAL,	//직사광선
	eLIGHT_POINT,		//점광원
	eLIGHT_SPOTLIGHT	//스포트라이트
};