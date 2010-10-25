/*
 *  BasicScope.h
 *  RationalVisions
 *
 *  Created by Neil Wallace on 19/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once;

#include "BaseScene.h"

namespace NScene 
{
	//-------------------------------------------------------------------------------------------------------
	class CBasicScope : public CBaseScene
	{
	public:
		CBasicScope();
		
		virtual void Init();
		virtual void Update(float time_step);
		virtual void Draw();
		
	};
};