/*
 *  SceneManager.cpp
 *  RationalVisions
 *
 *  Created by Neil Wallace on 18/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SceneManager.h"
#include "BaseScene.h"

#include "FireFloater.h"
#include "DaisyScene.h"
#include "Halftone.h"
#include "TriBandsScene.h"
#include "BasicScope.h"
#include "BeatDetectiveScene.h"

//*******************************************************************************************************
//*******************************************************************************************************
CSceneManager::CSceneManager()
: 
m_ShowScope(false) 
{ 
	Init(); 
}

//*******************************************************************************************************
void CSceneManager::Init()
{
	m_Scenes.push_back(new NScene::CBeatDetectiveScene());
	m_Scenes.push_back(new NScene::CTriBandsScene());
	m_Scenes.push_back(new NScene::CDaisyScene());
	m_Scenes.push_back(new NScene::CFireFloaterScene());
	m_Scenes.push_back(new NScene::CHalftone());

	for(TSceneList::iterator it = m_Scenes.begin(); it != m_Scenes.end(); ++it)
	{
		(*it)->Init();
	}
	
	mp_ScopeScene = new NScene::CBasicScope();
	mp_ScopeScene->Init();
	
	m_CurrScene = 0;
	
	//m_ShowScope = true;
}

//*******************************************************************************************************
void CSceneManager::Update(float time_step)
{
	m_Scenes[m_CurrScene]->Update(time_step);
}

//*******************************************************************************************************
void CSceneManager::Draw()
{
	m_Scenes[m_CurrScene]->Draw();
	
	if(m_ShowScope)
	{
		mp_ScopeScene->Draw();
	}
}

//*******************************************************************************************************
void CSceneManager::KeyPressed(int key) 
{
	switch (key) 
	{
		case 'n':
			NextScene();
			break;
		case 's':
			m_ShowScope = !m_ShowScope;
			break;
		case 'h':
			NScene::CFireFloaterScene::m_Hold = !NScene::CFireFloaterScene::m_Hold;
			break;
		default:
			break;
	}
}

//*******************************************************************************************************
void CSceneManager::NextScene()
{
	m_CurrScene++;
	m_CurrScene = m_CurrScene % m_Scenes.size();
}