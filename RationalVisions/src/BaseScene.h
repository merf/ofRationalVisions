/*
 *  BaseScene.h
 *  RationalVisions
 *
 *  Created by Neil Wallace on 18/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once;

#include "ofMain.h"

class CRationalVisionsApp;
class CSoundEngine;

namespace NScene
{
	//-------------------------------------------------------------------------------------------------------
	class CBaseScene
	{
	public:
		CBaseScene();
		
		//virtual methods
		virtual void Init();
		virtual void Draw();
		virtual void Update(float time_step);
		virtual void Destroy();
		
		//general handy stuff thats handy to have in most scenes.
		CRationalVisionsApp&	GetApp();
		CSoundEngine&			GetSoundEngine();
		float					GetWorldTime();
		float					GetAppWidth();
		float					GetAppHeight();
	};
};