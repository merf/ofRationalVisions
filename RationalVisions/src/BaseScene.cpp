/*
 *  BaseScene.cpp
 *  RationalVisions
 *
 *  Created by Neil Wallace on 18/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BaseScene.h"
#include "RationalVisionsApp.h"
#include "SoundEngine.h"

using namespace NScene;

CBaseScene::CBaseScene()
{
	Init();
}

void CBaseScene::Init()
{
}

void CBaseScene::Update(float time_step)
{
}

void CBaseScene::Draw()
{
}

void CBaseScene::Destroy()
{
}

CRationalVisionsApp& CBaseScene::GetApp()
{
	return *CRationalVisionsApp::Get();
}

CSoundEngine& CBaseScene::GetSoundEngine()
{
	return GetApp().GetSoundEngine();
}

float CBaseScene::GetWorldTime()
{
	return GetApp().GetWorldTime();
}

float CBaseScene::GetAppWidth()
{
	return GetApp().GetWidth();
}

float CBaseScene::GetAppHeight()
{
	return GetApp().GetHeight();
}