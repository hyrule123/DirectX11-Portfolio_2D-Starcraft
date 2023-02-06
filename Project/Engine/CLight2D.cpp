#include "pch.h"
#include "CLight2D.h"

#include "CTransform.h"

//렌더링매니저에 매 프레임마다 자신의 정보를 업데이트 해야함.
#include "CRenderMgr.h"

CLight2D::CLight2D()
	: CLight(eCOMPONENT_TYPE::LIGHT2D)
	, m_LightInfo{}
{
	m_LightInfo.LightColor.vDiffuse = Vec4(1.f, 1.f, 1.f, 1.f);
	m_LightInfo.LightColor.vAmbient = Vec4(1.f, 1.f, 1.f, 1.f);
	//m_LightInfo.vLightDir = Vec4::Unit[eAXIS3D::X];
	m_LightInfo.fAngle = XM_PI / 2.5f;	//30 degree
	m_LightInfo.fRadius = 400.f;
}

CLight2D::~CLight2D()
{
}

void CLight2D::finaltick()
{
	m_LightInfo.vLightWorldPos = Transform()->GetWorldPos();

	//만약 스포트라이트 또는 직사광선일 경우 트랜스폼의 방향을 따라가도록 설정
	if((UINT)eLIGHT_TYPE::eLIGHT_DIRECTIONAL == m_LightInfo.LightType
		||
		(UINT)eLIGHT_TYPE::eLIGHT_SPOTLIGHT == m_LightInfo.LightType)
		m_LightInfo.vLightDir = Transform()->GetRelativeDir(eDIR_TYPE::RIGHT);

	CRenderMgr::GetInst()->AddLight2DData(m_LightInfo);
}
