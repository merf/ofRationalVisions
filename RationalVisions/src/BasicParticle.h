/*
 *  BasicParticle.h
 *  RationalVisions
 *
 *  Created by Neil Wallace on 13/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "RMath.h"
#include "ofxVec2f.h"

namespace NScene 
{
	//------------------------------------------------------------------------------------------------------
	class CBasicParticle
	{
	public:
		CBasicParticle() : m_Pos(0,0), m_Life(1.0f), m_Size(0.0f) {};
		
		void		Init(ofxVec2f& pos, float size);
		void		Update(float time_step);
		
		bool		IsActive()	{ return m_Life > 0.0f; }
		float		GetSize()	{ return m_Size; }
		float		GetLife()	{ return m_Life; }
		
		void		IncVel(ofxVec2f& vel_inc) { m_Vel += vel_inc; }
		void		SetVel(ofxVec2f& new_vel) { m_Vel = new_vel; }
		
		void		DecLife(float dec) { m_Life -= dec; RMath::LimitLower(m_Life, 0.0f); }
		
		ofxVec2f	GetPos()	{ return m_Pos; }
		void		SetPos(ofxVec2f& pos)	{ m_Pos = pos; }
		
	private:
		ofxVec2f	m_Pos;
		ofxVec2f	m_Vel;
		
		float		m_Life;
		float		m_Size;
	};
}