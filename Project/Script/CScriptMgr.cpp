
#pragma once

#include "pch.h"
#include "CScriptMgr.h"


//Created Scripts
#include "CScript_Bullet.h"
#include "CScript_CameraMove.h"
#include "CScript_TestObj.h"
#include "CScript_Debug.h"
#include "CScript_Player.h"
#include "CScript_MouseCursor.h"



CScriptMgr::CScriptMgr() {}
CScriptMgr::~CScriptMgr()
{
	for(const auto& iter : m_umapScript)
	{
		DESTRUCTOR_DELETE(iter.second);
	}
}


void CScriptMgr::init()
{
	m_umapScript[SCRIPTS::BULLET] = new CScript_Bullet;
	m_umapScript[SCRIPTS::CAMERAMOVE] = new CScript_CameraMove;
	m_umapScript[SCRIPTS::TESTOBJ] = new CScript_TestObj;
	m_umapScript[SCRIPTS::DEBUG] = new CScript_Debug;
	m_umapScript[SCRIPTS::PLAYER] = new CScript_Player;
	m_umapScript[SCRIPTS::MOUSECURSOR] = new CScript_MouseCursor;
}

CScript* CScriptMgr::GetNewScript(const string& _strKey)
{
	const auto& iter = m_umapScript.find(_strKey);

	if (iter == m_umapScript.end())
		return nullptr;

	return iter->second->Clone();
}
