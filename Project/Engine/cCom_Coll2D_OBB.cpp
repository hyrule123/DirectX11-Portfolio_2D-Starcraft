#include "pch.h"
#include "cCom_Coll2D_OBB.h"

#include "cTransform.h"

//디버그 출력용
#include "cRenderMgr.h"

#include "S_H_Debug.hlsli"

#include "cGameObject.h"

cCom_Coll2D_OBB::cCom_Coll2D_OBB()
	: ICollider2D(eCOLLIDER_TYPE_2D::OBB)
	, m_tOBBInfo{}
{
}

cCom_Coll2D_OBB::cCom_Coll2D_OBB(const cCom_Coll2D_OBB& _other)
	: ICollider2D(_other)
{
}

cCom_Coll2D_OBB::~cCom_Coll2D_OBB()
{
}


//이 함수는 cTransform에서 값이 변했을 경우에만 호출된다.
void cCom_Coll2D_OBB::UpdateCollider()
{
	//충돌체 주소를 가져온다.
	const cTransform& pTransform = GetOwner()->Transform();

	//자신의 OBB 정보를 계산한다.
	const Matrix& WorldMat = pTransform.GetWorldMatWithoutSize();
	const Vec3& Size = GetCollSize();

	for (int i = 0; i < (int)eAXIS2D::END; ++i)
	{
		//단위벡터로 만든 뒤 size를 곱해준다.
		m_tOBBInfo.m_v2Axis[i] = Vec3::TransformNormal(Vec3::Unit[i], WorldMat) * Size[i];
	}

	//부모의 동일함수를 호출(Simple ICollider 계산)
	ICollider2D::UpdateCollider();
}

void cCom_Coll2D_OBB::UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos)
{
	const Vec2& CenterPos = GetCenterPos().XY();
	_vSimpleCollLBRTPos = Vec4(CenterPos, CenterPos);

	Vec2 size = GetCollSize().XY();
	//제곱
	size *= size;
	//대각선 길이를 구한다.
	float diagLen = sqrtf(size.x + size.y) * 0.5f;

	_vSimpleCollLBRTPos += Vec4(-diagLen, -diagLen, diagLen, diagLen);
}


void cCom_Coll2D_OBB::DebugRender()
{
	//충돌체(자신)의 정보)
	const Matrix& matSize = Matrix::CreateScale(GetCollSize());

	float angle = atan2f(m_tOBBInfo.m_v2Axis[0].y, m_tOBBInfo.m_v2Axis[0].x);

	const Matrix& matRot = Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(0.f, 0.f, angle));

	const Vec3& offsetPos = GetCenterPos();

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eDEBUGSHAPE_TYPE::RECT;
	Info.matWorld = matSize * matRot;

	//offsetPos만큼 이동
	Info.matWorld._41 += offsetPos.x;
	Info.matWorld._42 += offsetPos.y;
	Info.matWorld._43 += offsetPos.z;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	cRenderMgr::GetInst()->AddDebugShapeRender(Info);
}
