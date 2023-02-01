#include "pch.h"
#include "CUI.h"


#include "CImGuiMgr.h"

CUI::CUI(const string& _Name)
	: m_Active(true)
	, m_ParentUI()
{
	SetstrID(_Name);
}

CUI::~CUI()
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (nullptr != m_vecChildUI[i])
			delete(m_vecChildUI[i]);
	}
}




void CUI::tickRecursive()
{
	tick();

	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; i++)
	{
		m_vecChildUI[i]->tickRecursive();
	}
}


void CUI::finaltick()
{
	if (false == m_Active)
		return;

	if (true == beginUI())
	{
		render_update();

		size_t size = m_vecChildUI.size();
		for (size_t i = 0; i < size; ++i)
		{
			//�ڽ� UI�� �ڽ��� UI ���ʿ� �������Ѵ�.
			m_vecChildUI[i]->finaltick();
		}

		ImGui::Separator();

		endUI();
	}


}



void CUI::AddChildUI(CUI* _UI)
{
	_UI->m_ParentUI = this;
	m_vecChildUI.push_back(_UI);

	CImGuiMgr::GetInst()->CreateUI(_UI);
}



CUI* CUI::FindChildUIByName(const string& _Name)
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		size_t pos = m_vecChildUI[i]->GetName().find(_Name);
		if (pos != string::npos)
			return m_vecChildUI[i];
	}

	return nullptr;
}



