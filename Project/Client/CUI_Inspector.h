#pragma once
#include "CUI_BasicWindow.h"

class CUI_Component;

//Ư�� ���ӿ�����Ʈ�� ���� ���¸� Ȯ���ϴ� Ŭ����.

class CUI_Inspector :
    public CUI_BasicWindow
{
public:
    CUI_Inspector();
    virtual ~CUI_Inspector();

private:
    CGameObject* m_pTarget;
    CUI_Component* m_arrComUI[eCOMPONENT_END];

public:
    void init() override;
    void tick() override;


private:


};
