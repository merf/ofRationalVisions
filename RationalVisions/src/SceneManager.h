/*
 *  SceneManager.h
 *  RationalVisions
 *
 *  Created by Neil Wallace on 18/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once;

#include "BaseScene.h"
#include <vector>

//-------------------------------------------------------------------------------------------------------
class CSceneManager
{
public:
	
	CSceneManager();
	
	void Init();
	void Update(float time_step);
	void Draw();
	void KeyPressed(int key);
	
	void NextScene();
	
private:
	typedef std::vector<NScene::CBaseScene*> TSceneList;
	TSceneList						m_Scenes;
	
	NScene::CBaseScene*				mp_ScopeScene;
	
	int								m_CurrScene;
	bool							m_ShowScope;
};