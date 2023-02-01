#include "pch.h"
#include "CUI_PopupWIndow.h"

CUI_PopupWindow::CUI_PopupWindow()
	: CUI_BasicWindow("PopupWindow")
	, m_PopupFlags()
	, m_vPopupPos()
	, m_bModal()
{
}

CUI_PopupWindow::~CUI_PopupWindow()
{
}

bool CUI_PopupWindow::beginUI()
{


	////�θ� UI �ּҰ� ���� ���, �� �ڽ��� �ֻ�� ������ UI�� ���
//if (nullptr == m_ParentUI)
//{
//	//��޸��� â�� ���(�� UI�� ���� �־ �ٸ� UI �� ����� �� ���� ���)
//	if (false == m_bModal)
//	{
//		//���� �Ϲ����� Begin �Լ��� �̿��ؼ� â�� �����Ѵ�.
//		ImGui::Begin(strFullName.c_str(), &m_Active);

//		

//		//�ڽ��� ���� �����鿡 ���� ������ �۾��� �����ϰ�
//		render_update();

//		//�ڽ� ���� ��ҵ鿡 ���� ������Ʈ�� �����Ѵ�.
//		LoopChildFinaltick();

//		//�ڽ��� Parent UI �̹Ƿ� End()
//		ImGui::End();
//	}

//	//���
//	else
//	{

//	}
//}

////�ڽ��� �ڽ� UI�� ���
//else
//{
//	ImGui::BeginChild(strFullName.c_str(), m_vSize);

//	render_update();

//	LoopChildFinaltick();

//	ImGui::EndChild();
//}

//endUI();

	ImGui::SetNextWindowPos(m_vPopupPos);
	ImGui::SetNextWindowSize(GetSize());

	ImGui::OpenPopup(GetstrID().data(), m_PopupFlags);

if (true == m_bModal)
{
	return ImGui::BeginPopupModal(GetstrID().data(), GetActivePtr(), GetWindowFlag());
}

	return true;
}

void CUI_PopupWindow::endUI()
{
	ImGui::EndPopup();
}