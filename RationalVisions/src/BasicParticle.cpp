/*
 *  BasicParticle.cpp
 *  RationalVisions
 *
 *  Created by Neil Wallace on 13/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BasicParticle.h"

namespace NScene
{
	//*******************************************************************************************************
	//*******************************************************************************************************
	void CBasicParticle::Init(ofxVec2f& pos, float size)
	{
		m_Pos = pos; 
		m_Size = size;
		m_Life = 1.0f; 
	}

	//*******************************************************************************************************
	void CBasicParticle::Update(float time_step)
	{
		if(m_Life > 0.0f)
		{
			float mult = time_step * 60.0f;
			float damp_factor = 0.5 / mult;
			m_Vel *= damp_factor;
			m_Pos += (m_Vel * mult);
		}
	}
}