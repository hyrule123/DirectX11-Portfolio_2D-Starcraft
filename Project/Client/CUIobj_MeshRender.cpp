#include "pch.h"
#include "CUIobj_MeshRender.h"

#include <Engine/CGameObject.h>
#include <Engine/CMeshRender.h>
#include <Engine/CResMgr.h>

#include "CUI_ComboBox.h"

CUIobj_MeshRender::CUIobj_MeshRender()
	: CUIobj_Component("MeshRender", eCOMPONENT_TYPE::COLLIDER2D)
{
	m_pComboBoxMesh = new CUI_ComboBox("MeshComboBox");
	AddChildUI(m_pComboBoxMesh);

	m_pComboBoxMtrl = new CUI_ComboBox("MtrlComboBox");
	AddChildUI(m_pComboBoxMtrl);
}

CUIobj_MeshRender::~CUIobj_MeshRender()
{
}

void CUIobj_MeshRender::UpdateMeshListCallback()
{
	//원래는 메쉬의 갯수 자체만으로 새로 목록을 갱신해야할지 여부를 판단하는것은 좋지 않음.
	const auto& map = CResMgr::GetInst()->GetResMap(eRES_TYPE::MESH);
	if (m_pComboBoxMesh->GetItemNum() != map.size())
	{
		vector<tComboItem> tempVec;
		for (const auto& iter : map)
		{
			tempVec.push_back(tComboItem{ iter.second.Get()->GetKey(), iter.second.Get(), });
		}

		m_pComboBoxMesh->SetItem(tempVec);
	}

}

//메쉬 콤보박스에서 특정 메쉬가 클릭되었을 때 호출될 함수
void CUIobj_MeshRender::ChangeMeshCallback()
{
	const tComboItem& sel = m_pComboBoxMesh->GetCurrentSelected();
	if (true == sel.strName.empty())
		return;

	if (nullptr == sel.pData.ptr)
		return;

	GetTarget()->MeshRender()->SetMesh(static_cast<CMesh*>(sel.pData.ptr));
}

void CUIobj_MeshRender::UpdateMtrlListCallback()
{
	//원래는 메쉬의 갯수 자체만으로 새로 목록을 갱신해야할지 여부를 판단하는것은 좋지 않음.
	const auto& map = CResMgr::GetInst()->GetResMap(eRES_TYPE::MATERIAL);
	if (m_pComboBoxMtrl->GetItemNum() != map.size())
	{
		vector<tComboItem> tempVec;
		for (const auto& iter : map)
		{
			tempVec.push_back(tComboItem{ iter.second.Get()->GetKey(), iter.second.Get(), });
		}

		m_pComboBoxMtrl->SetItem(tempVec);
	}
}

void CUIobj_MeshRender::ChangeMtrlCallback()
{
	const tComboItem& sel = m_pComboBoxMtrl->GetCurrentSelected();
	if (true == sel.strName.empty())
		return;

	Ptr<CMaterial> mtrl = static_cast<CMaterial*>(sel.pData.ptr);

	if (nullptr == mtrl)
		return;

	GetTarget()->MeshRender()->SetMaterial(mtrl);
}

void CUIobj_MeshRender::init()
{
	m_pComboBoxMesh->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CUIobj_MeshRender::UpdateMeshListCallback), eCALLBACK_TYPE::ONCLICK);
	m_pComboBoxMesh->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CUIobj_MeshRender::ChangeMeshCallback)
	, eCALLBACK_TYPE::ONSELECT);

	m_pComboBoxMtrl->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CUIobj_MeshRender::UpdateMtrlListCallback)
		, eCALLBACK_TYPE::ONCLICK);
	m_pComboBoxMtrl->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CUIobj_MeshRender::ChangeMtrlCallback)
		, eCALLBACK_TYPE::ONSELECT);

	//일단 기본적으로 메쉬와 리스트의 이름을 한번 받아온다.
	UpdateMeshListCallback();
	UpdateMtrlListCallback();

	
	//타겟이 지정되어 있을경우 타겟의 메쉬와 재질의 이름을 기본 세팅해준다.
	if (nullptr != GetTarget() && nullptr != GetTarget()->MeshRender())
	{
		const string& meshname = GetTarget()->MeshRender()->GetMesh()->GetKey();
		m_pComboBoxMesh->SetCurrentSelected(meshname);


		const string& mtrlname = GetTarget()->MeshRender()->GetCurMaterial()->GetKey();
		m_pComboBoxMtrl->SetCurrentSelected(meshname);
	}
}

void CUIobj_MeshRender::tick()
{
	//나중에 게임오브젝트를 선택해서 가져오는 기능 구현하면 아래코드는 지울것,.
	if (nullptr != GetTarget() && nullptr != GetTarget()->MeshRender())
	{
		if (-1 == m_pComboBoxMesh->GetCurrentIndex())
		{
			const string& meshname = GetTarget()->MeshRender()->GetMesh()->GetKey();
			m_pComboBoxMesh->SetCurrentSelected(meshname);
		}

		if (-1 == m_pComboBoxMtrl->GetCurrentIndex())
		{
			const string& mtrlname = GetTarget()->MeshRender()->GetCurMaterial()->GetKey();
			m_pComboBoxMtrl->SetCurrentSelected(mtrlname);
		}
	}

}