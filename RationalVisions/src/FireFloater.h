/*
 *  FireFloater.h
 *  RationalVisions
 *
 *  Created by Neil Wallace on 18/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once;

#include "BaseScene.h"
#include "ofxVec2f.h"
#include "MSAPerlin.h"

namespace NScene 
{
	//-------------------------------------------------------------------------------------------------------
	class CParticle
	{
	public:
		CParticle() : m_Pos(0,0), m_Life(0.0f), m_Size(0.0f) {};
		
		void		Init(ofxVec2f& pos, float size);
		void		Update();
		
		bool		IsActive()	{ return m_Life > 0.0f; }
		float		GetSize();
		float		GetLife()	{ return m_Life; }
		ofxVec2f	GetPos()	{ return m_Pos; }
		
	private:
		ofxVec2f	m_Pos;
		ofxVec2f	m_Vel;
		
		float		m_Life;
		float		m_Size;
	};
	
	//-------------------------------------------------------------------------------------------------------
	class CFireFloaterScene : public CBaseScene
	{
	public:
		CFireFloaterScene();
		
		virtual void	Init();
		virtual void	Update(float time_step);
		virtual void	Draw();
		
		static MSA::Perlin	m_Perlin;
		static bool		m_Hold;
		
	private:
		void			GetPosAndSize(float f, ofxVec2f& pos, float& size);
		void			AddParticle(ofxVec2f& pos, float size);
		CParticle**		m_Particles;
		int				m_CurrentParticle;
	};
};