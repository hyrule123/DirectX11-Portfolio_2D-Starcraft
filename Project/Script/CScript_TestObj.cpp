#include "pch.h"
#include "CScript_TestObj.h"

#include <Engine/CMeshRender.h>
#include <Engine/CMaterial.h>
#include <Engine/CTransform.h>

#include <Engine/CTimeMgr.h>
#include <Engine/CKeyMgr.h>

CScript_TestObj::CScript_TestObj()
	: CScript(TYPE_INDEX(CScript_TestObj))
	, m_ColorKey(1.f, 1.f, 1.f, 1.f)
	, m_MoveSpeed(400.f)
	, m_TurningForceRad(XM_PI / 2.f)
{
}

CScript_TestObj::~CScript_TestObj()
{
}

void CScript_TestObj::BeginCollision(CCollider* _pCol)
{
	GetOwner()->SetLifeSpan(2.f);
}

void CScript_TestObj::init()
{
	//������ۿ� �÷�Ű�� ����, �ȼ� ���̴��� ������۰� ���޵ǵ��� ����
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &m_ColorKey);
}

void CScript_TestObj::tick()
{
	
	Vec3 Scale = Transform()->GetRelativeScale();
	Vec3 Rot = Transform()->GetRelativeRot();

	if (KEY_PRESSED(KEY::E))
	{
		Scale += Vec3(1.f, 1.f, 0.f) * DELTA_TIME;
		Rot += Vec3(0.f, 0.f, 1.f) * DELTA_TIME;
	}

	Transform()->SetRelativeRot(Rot);
	Transform()->SetRelativeScale(Scale);

}