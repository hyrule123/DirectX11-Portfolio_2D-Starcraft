#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CMeshRender.h"
#include "CScriptHolder.h"
#include "CScript.h"

//레이어 옮길 때 사용
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

//트랜스폼 상속
#include "CTransform.h"

//생명주기 계산용
#include "CTimeMgr.h"


CGameObject::CGameObject()
	: m_arrCom{}
	, m_bInitialized()
	, m_RenderCom()
	, m_Parent()
	, m_iLayerIdx(-1)
	, m_bFixLayer()
	, m_bDestroy()
	, m_fLifeSpan(FLT_MAX_NEG)
{
}

CGameObject::CGameObject(const CGameObject& _other)
	: CEntity(_other)
	, m_iLayerIdx(_other.m_iLayerIdx)
	, m_bFixLayer(_other.m_bFixLayer)
	, m_bInitialized(_other.m_bInitialized)
	, m_bDestroy()
	, m_fLifeSpan(FLT_MAX_NEG)
{
	//1. 컴포넌트 목록 복사
	for (int i = 0; i < eCOMPONENT_END; ++i)
	{
		if (nullptr != _other.m_arrCom[i])
		{
			m_arrCom[i] = _other.m_arrCom[i]->Clone();
			m_arrCom[i]->SetOwner(this);

			//1-1. 렌더링 컴포넌트 일 경우 m_RenderCom에 복사
			if (g_RenderComIdxStart <= i && i < g_RenderComIdxEnd)
			{
				assert(nullptr == m_RenderCom);
				m_RenderCom = static_cast<CRenderComponent*>(m_arrCom[i]);
			}
		}
	}

	//2. 자녀 오브젝트 복사
	size_t size = _other.m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddChild(_other.m_vecChild[i]->Clone());
	}

}

CGameObject::~CGameObject()
{
	/*for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			delete m_arrCom[i];
	}*/

	Safe_Del_Array(m_arrCom);
}

void CGameObject::init()
{
	m_bInitialized = true;

	for (UINT i = 0; i < eCOMPONENT_END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->init();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->init();
	}

}

void CGameObject::tick()
{
	//자신이 파괴 대기 상태일 경우 자신과 모든 자식들에 대해 tick을 처리하지 않음
	if (true == m_bDestroy)
		return;


	for (UINT i = 0; i < eCOMPONENT_END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	//자신이 파괴 대기 상태일 경우 자신과 모든 자식들에 대해 tick을 처리하지 않음
	if (true == m_bDestroy)
	{
		//스크립트를 제외한 컴포넌트들에 대해 finaltick()을 호출한다.
		cleanup();
		return;
	}
	else if (FLT_MAX_NEG != m_fLifeSpan)
	{
		m_fLifeSpan -= DELTA_TIME;
		if (m_fLifeSpan < 0.f)
		{
			DestroyObject(this);
			return;
		}
	}
		

	//스크립트를 제외한 컴포넌트들에 대해 finaltick()을 호출한다.
	for (UINT i = 0; i < eCOMPONENT_SCRIPT_HOLDER; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->finaltick();
	}
}

void CGameObject::render()
{
	//삭제 대기 상태일 경우 렌더링을 하지 않음.
	if (nullptr == m_RenderCom || true == m_bDestroy)
		return;

	m_RenderCom->render();
}

void CGameObject::cleanup()
{
	//본인의 컴포넌트 정리
	for (UINT i = 0; i < eCOMPONENT_END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->cleanup();
	}

	//자식 컴포넌트들도 모두 cleanup
	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->cleanup();
	}
	
	//만약 자신의 부모 오브젝트가 존재하고, 해당 오브젝트는 삭제 대기 상태가 아닐 경우
	//※부모 오브젝트도 삭제 대기 상태일 경우에는 부모 오브젝트도 어차피 이번 프레임 마지막에 삭제가 될 것이므로
	//굳이 자신의 주소를 정리해달라고 요청하지 않아도 된다.
	if (nullptr != m_Parent && false == m_Parent->GetDestroyed())
	{
		m_Parent->RemoveChild(this);
		m_Parent = nullptr;
	}
}

void CGameObject::AddComponent(CComponent* _Component)
{
	UINT ComType = (UINT)_Component->GetType();
	assert(nullptr == m_arrCom[ComType]);

	switch ((eCOMPONENT_TYPE)ComType)
	{
	case eCOMPONENT_TYPE::eCOMPONENT_TRANSFORM:
	case eCOMPONENT_TYPE::eCOMPONENT_COLLIDER2D:
	case eCOMPONENT_TYPE::eCOMPONENT_COLLIDER3D:
	case eCOMPONENT_TYPE::eCOMPONENT_ANIMATOR2D:
	case eCOMPONENT_TYPE::eCOMPONENT_ANIMATOR3D:
	case eCOMPONENT_TYPE::eCOMPONENT_LIGHT2D:
	case eCOMPONENT_TYPE::eCOMPONENT_LIGHT3D:
	case eCOMPONENT_TYPE::eCOMPONENT_CAMERA:
		break;

	//Render Components
	case eCOMPONENT_TYPE::eCOMPONENT_MESH_RENDER:
	case eCOMPONENT_TYPE::eCOMPONENT_PARTICLE_SYSTEM:
	case eCOMPONENT_TYPE::eCOMPONENT_TILEMAP:
	case eCOMPONENT_TYPE::eCOMPONENT_LANDSCAPE:
	case eCOMPONENT_TYPE::eCOMPONENT_DECAL:
		//m_RenderCom에 하나 이상의 Render 컴포넌트가 들어가 있을 경우 에러 발생시킴.
		assert(nullptr == m_RenderCom);
		m_RenderCom = static_cast<CRenderComponent*>(_Component);
		break;

	case eCOMPONENT_TYPE::eCOMPONENT_SCRIPT_HOLDER:
		break;
	default:
		break;
	}

	//소유자 주소를 등록.
	_Component->m_pOwner = this;
	m_arrCom[ComType] = _Component;

	//이미 작동중일 경우 바로 init() 호출
	if(m_bInitialized)
		_Component->init();
}

void CGameObject::AddScript(CScript* _Script)
{
	if (nullptr == _Script)
		return;

	CScriptHolder* pScriptHolder = static_cast<CScriptHolder*>(m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_SCRIPT_HOLDER]);
	if (nullptr == pScriptHolder)
	{
		pScriptHolder = new CScriptHolder;
		AddComponent(pScriptHolder);
	}
	pScriptHolder->AddScript(_Script);
}

void CGameObject::AddChild(CGameObject* _Object)
{
	if (nullptr != (_Object->GetParent()))
		_Object->GetParent()->RemoveChild(_Object);

	_Object->SetParent(this);
	m_vecChild.push_back(_Object);
}

void CGameObject::RemoveChild(CGameObject* _Object)
{
	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();
	while (iter != iterEnd)
	{
		if (_Object == (*iter))
		{
			m_vecChild.erase(iter);
			return;
		}
		++iter;
	}
}


void CGameObject::AddAllHierarchyObjects(int _iLayerIdx, vector<CGameObject*>& _vecObj)
{
	//인덱스가 아직 지정되지 않았을 경우 -> 무조건 인덱스 지정을 한번 해줘야함
	//레이어가 고정되지 않았을 경우 -> 부모 오브젝트의 레이어를 따라감
	//둘중 하나라도 걸리면 일단 옮겨야함
	if (-1 == m_iLayerIdx || false == m_bFixLayer)
	{
		//만약 어딘가의 레이어에 등록되어 있었다면 기존 레이어에서 자신을 삭제해야 한다.
		if (m_iLayerIdx > 0)
		{
			CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx)->RemoveGameObject(this);
		}

		//삭제하고 새로운 레이어에 등록
		m_iLayerIdx = _iLayerIdx;
		_vecObj.push_back(this);
	}
	
	//위 조건의 반대는
	//m_iLayerIdx가 1이 아니고(&&) m_bFixLayer가 true 일때 이므로 이 때는 레이어 이동을 무조건 안하는 경우이다.
	
	//자식들을 대상으로도 재귀적으로 호출
	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddAllHierarchyObjects(_iLayerIdx, _vecObj);
	}
}

void CGameObject::SetParentTransformUpdated()
{
	if (nullptr != Transform())
		Transform()->SetParentUpdate();

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetParentTransformUpdated();
	}
}

bool CGameObject::GetParentWorldMatrix(Matrix& _mat)
{
	//부모 오브젝트가 없거나 트랜스폼이 없을 경우 false 반환
	if (nullptr == m_Parent || nullptr == m_Parent->Transform())
		return false;

	//있을 경우 인자에 그대로 대입, true 반환.
	_mat = m_Parent->Transform()->GetWorldMat();
	return true;
}


void CGameObject::SetChildTransformToUpdate()
{
	if (nullptr != Transform())
		Transform()->SetParentUpdate();

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetParentTransformUpdated();
	}
}