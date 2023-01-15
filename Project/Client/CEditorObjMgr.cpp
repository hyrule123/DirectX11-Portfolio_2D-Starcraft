#include "pch.h"

#include "CEditorObjMgr.h"

#include <Engine/CGameObject.h>

#include <Engine/CTimeMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCamera.h>

#define DEBUG_MAT_WVP MAT_0
#define DEBUG_VEC4_COLOR VEC4_0

CEditorObjMgr::CEditorObjMgr()
	: m_arrDebugShape{}
{
}
CEditorObjMgr::~CEditorObjMgr()
{
	for (int i = 0; i < eSHAPE_END; ++i)
	{
		SAFE_DELETE(m_arrDebugShape[i])
	}
}


void CEditorObjMgr::init()
{
	Ptr<CMaterial> pDebugMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DebugMtrl");


	for (int i = 0; i < eSHAPE_END; ++i)
	{
		m_arrDebugShape[i] = new CGameObject;

		switch ((eSHAPE_TYPE)i)
		{
		case eSHAPE_RECT:
		{
			CMeshRender* pMesh = new CMeshRender;
			//월드행렬을 직접 받아서 쉐이더에 보낼 것이기 떄문에 Transform은 필요하지 않음.
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug");
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);

			 break;
		}

		case eSHAPE_CIRCLE:
		{
			CMeshRender* pMesh = new CMeshRender;
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug");
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);
			break;
		}
		case eSHAPE_CUBE:
		{
			break;
		}
		case eSHAPE_SPHERE:
		{
			break;
		}
		default:
			break;
		}
	}
}

void CEditorObjMgr::progress()
{
	tick();
	render();
}

//Engine으로부터 디버그에 사용할 오브젝트들 정보를 받아오기
void CEditorObjMgr::tick()
{
	//이 메소드의 호출 시점은 모든 게임의 메인 렌더링과정이 종료된 이후이다. 

	//CRenderMgr로부터 이번 프레임에 그릴 디버그 쉐이프 목록을 받아온다.
	CRenderMgr::GetInst()->UpdateDebugShapeRender(m_listDebugShapeInfo);

	float DT = DELTA_TIME;

	//여기선 단순히 시간만 뺴 준다. render()에서 그리고 나서 음수이면 제거함.
	for (auto& iter : m_listDebugShapeInfo)
	{
		iter.fLifeSpan -= DT;
	}
}

//받아온 오브젝트를 통해서 디버그 정보를 사각형으로 그리기
void CEditorObjMgr::render()
{
	auto iter = m_listDebugShapeInfo.begin();
	const auto& iterEnd = m_listDebugShapeInfo.end();

	//메인 카메라의 View Projection  행렬을 가져온다.
	const Matrix& matVP = CRenderMgr::GetInst()->GetCamera(eCAMIDX_MAIN)->GetViewProjMatrix();

	while (iter != iterEnd)
	{
		switch (iter->eShape)
		{
		case eSHAPE_RECT:
		{
			Ptr<CMaterial> pMtrl = m_arrDebugShape[eSHAPE_RECT]->MeshRender()->GetMaterial();
			//월드행렬 전달.

			Matrix matWVP = iter->matWorld * matVP;
			pMtrl->SetScalarParam(DEBUG_MAT_WVP, matWVP.m);
			pMtrl->SetScalarParam(DEBUG_VEC4_COLOR, iter->vColor);

			//레이어에 속해서 게임 내에서 돌아가는 게임오브젝트가 아니므로 강제로 render()를 호출해야 한다.
			m_arrDebugShape[eSHAPE_RECT]->render();

			break;
		}
		case eSHAPE_CIRCLE:
		{
			Ptr<CMaterial> pMtrl = m_arrDebugShape[eSHAPE_CIRCLE]->MeshRender()->GetMaterial();
			//월드행렬 전달.
			Matrix matWVP = iter->matWorld * matVP;
			pMtrl->SetScalarParam(DEBUG_MAT_WVP, matWVP.m);
			pMtrl->SetScalarParam(DEBUG_VEC4_COLOR, iter->vColor);

			m_arrDebugShape[eSHAPE_CIRCLE]->render();
			break;
		}
		case eSHAPE_CUBE:
			break;
		case eSHAPE_SPHERE:
			break;
		case eSHAPE_END:
			break;
		default:
			break;
		}
			
			//시간이 다 됐을 경우 해당 iterator은 삭제
		if (iter->fLifeSpan < 0.f)
		{
			iter = m_listDebugShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}

}
