#include "pch.h"

#include "CCollisionMgr.h"

#include "CCollider2D.h"

#include "CCollider2D_Rect.h"
#include "CCollider2D_Circle.h"
#include "CCollider2D_OBB.h"

CCollider2D::CCollider2D(eCOLLIDER_TYPE _eColType)
	: CCollider(eCOMPONENT_TYPE::COLLIDER2D, _eColType)
	, m_SpatialPartitionInfo{}
{
}

CCollider2D::CCollider2D(const CCollider2D& _other)
	:CCollider(_other)
	, m_SpatialPartitionInfo(_other.m_SpatialPartitionInfo)
{
}

CCollider2D::~CCollider2D()
{
}

bool CCollider2D::CheckCollision(CCollider* _other)
{
	switch (GetColliderType())
	{
	case eCOLLIDER_TYPE::_2D_RECT:
		return CheckCollisionRect(static_cast<CCollider2D_Rect*>(_other));
	case eCOLLIDER_TYPE::_2D_CIRCLE:
		return CheckCollisionCircle(static_cast<CCollider2D_Circle*>(_other));
	case eCOLLIDER_TYPE::_2D_OBB:
		return CheckCollisionOBB2D(static_cast<CCollider2D_OBB*>(_other));
	}


	return false;
}

void CCollider2D::finaltick()
{
	//아래의 두 메소드는 transform에서 하는 걸로 변경
	//UpdateColliderInfo();
	//UpdateSpatialPartitionInfo();
	DebugRender();

	CCollisionMgr::GetInst()->AddCollider2D(this, GetSpatialPartitionInfo());
}



