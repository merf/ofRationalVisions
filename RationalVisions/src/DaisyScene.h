/*
 *  DaisyScene.h
 *  RationalVisions
 *
 *  Created by Neil Wallace on 13/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once;

#include "BaseScene.h"
#include "ofxVec2f.h"
#include "MSAPerlin.h"


namespace NScene 
{
	class CBasicParticle;

	//-------------------------------------------------------------------------------------------------------
	class CDaisyScene : public CBaseScene
	{
	public:
		CDaisyScene();
		
		virtual void	Init();
		virtual void	Update(float time_step);
		virtual void	Draw();

		void			DrawCircleRecursive(float depth, float size, float f, ofxVec2f pos, float alpha);

		static MSA::Perlin	m_Perlin;
		
	private:
		typedef std::vector<CBasicParticle*> TParticleList;
		TParticleList	m_Particles;
		float			m_Spin;
	};
}