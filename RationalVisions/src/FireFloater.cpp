/*
 *  FireFloater.cpp
 *  RationalVisions
 *
 *  Created by Neil Wallace on 18/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FireFloater.h"
#include "RationalVisionsApp.h"
#include "SoundEngine.h"

#include "RMath.h"

using namespace NScene;

MSA::Perlin CFireFloaterScene::m_Perlin;
bool CFireFloaterScene::m_Hold = true;

const int num_particles = 5000;
float num_per_row = 25.0f;
float one_on_num = 1.0f / num_per_row;




//*******************************************************************************************************
//*******************************************************************************************************
void CParticle::Init(ofxVec2f& pos, float size)
{
	m_Pos = pos; 
	m_Size = size;
	//RMath::LimitUpper(m_Size, 10.0f);
	m_Life = 1.0f; 
	static float scale_to_vel = 0.5f;
	m_Vel.set(0.0f, -size*scale_to_vel);
}

//*******************************************************************************************************
void CParticle::Update()
{
	if(m_Life > 0.0f)
	{
		static float vel = 0.1f;
		static float death_rate = 0.01f;
		
		m_Pos += m_Vel;
		
		float f = m_Pos.x / CRationalVisionsApp::Get()->GetWidth();
		RMath::Limit(0.0f, f, 1.0f);
		
		float vel_dir = -CRationalVisionsApp::Get()->GetSoundEngine().GetMovement(f);
		//vel_dir -= 0.5f;
		vel_dir *= 1.0f;
		float hold_pos = (0.75f + 0.1f * cos(m_Pos.x + CRationalVisionsApp::Get()->GetWorldTime())) * CRationalVisionsApp::Get()->GetHeight();
		
		static bool m2 = true;
		if(CFireFloaterScene::m_Hold && m2)
		{
			m_Vel.y += vel * vel_dir;
		}
		else
		{
			if(CFireFloaterScene::m_Hold && m_Pos.y < hold_pos)
			{
				m_Vel.y += vel * 3.0f; 
			}
			else
			{
				m_Vel.y -= vel; 
			}
		}
		m_Life -= death_rate;
		
		m_Vel.x += CFireFloaterScene::m_Perlin.get(m_Pos.x + CRationalVisionsApp::Get()->GetWorldTime(), m_Pos.y) * (1.1f*m_Life);
		m_Vel.y -= CFireFloaterScene::m_Perlin.get(m_Pos.y + CRationalVisionsApp::Get()->GetWorldTime(), m_Pos.x) * 1.1f;
		
		if(m_Vel.y > 0.0f)
		{
			//m_Vel.y = 0.0f;
		}
		
		//m_Vel *= (m_Life+0.2f);
		//m_Pos.y += CFireFloaterScene::m_Perlin.get(m_Pos.y, CRationalVisionsApp::Get()->GetWorldTime() * 0.1f) * 1.0f;
	}
}

//*******************************************************************************************************
float CParticle::GetSize()
{ 
	//float f = m_Pos.x / CRationalVisionsApp::Get()->GetWidth();
	//RMath::Limit(0.0f, f, 1.0f);	
	//float avg = 5.0f * CRationalVisionsApp::Get()->GetSoundEngine().GetAverage(f);
	
	return m_Size * (0.1f+m_Life); 
}

//*******************************************************************************************************
//*******************************************************************************************************
CFireFloaterScene::CFireFloaterScene() 
: CBaseScene(),
m_CurrentParticle(0)
{
};

//*******************************************************************************************************
void CFireFloaterScene::Init()
{
	m_Particles = new CParticle*[num_particles];
	
	for(int i=0; i<num_particles; ++i)
	{
		m_Particles[i] = new CParticle();
	}
}


float ticker = 0.0f;

//*******************************************************************************************************
void CFireFloaterScene::Update(float time_step)
{
	ticker -= 0.1f;
	
	bool spawn_all = false;
	
	if(ticker < 0.0f)
	{
		//spawn_all = true;
		ticker = 30.0f;
	}
	
	static float spawn_size = 10.0f;
	
	ofxVec2f pos;
	float size;
	for(float f = 0.0f; f<=1.0f; f += one_on_num)
	{
		GetPosAndSize(f, pos, size);
		if(spawn_all || GetSoundEngine().GetMovement(f) > 0.5f)
		//if(spawn_all || size > spawn_size)
		{
			pos.x += CFireFloaterScene::m_Perlin.get(GetWorldTime()*0.1f, size) * GetAppWidth() * 0.2f;
			AddParticle(pos, size);
		}
	}
		
	
	for(int i=0; i<num_particles; ++i)
	{
		m_Particles[i]->Update();
	}
}

//*******************************************************************************************************
void CFireFloaterScene::GetPosAndSize(float f, ofxVec2f& pos, float& size)
{
	float w = GetAppWidth();
	float h = GetAppHeight();
	
	float width_to_use = 0.8f;
	
	pos = ofxVec2f(f * w * width_to_use + (0.5f * (1.0f - width_to_use) * w), h);
	//size = (1.0f + GetSoundEngine().GetMovement(f)) * 0.5f * GetSoundEngine().GetAmplitude(f);
	//size = GetSoundEngine().GetMovement(f) * GetSoundEngine().GetAverage(f);
	size = GetSoundEngine().GetMovement(f) * GetSoundEngine().GetAmplitude(f);

	//size = GetSoundEngine().GetAmplitude(f);
	
	RMath::Limit(0.03f, size, 1.0f);
	
	size *= GetApp().GetScreenRelativeSize(0.01f);
}

//*******************************************************************************************************
void CFireFloaterScene::AddParticle(ofxVec2f& pos, float size)
{
	m_Particles[m_CurrentParticle]->Init(pos, size);
	++m_CurrentParticle;
	m_CurrentParticle = m_CurrentParticle % num_particles;
}

//*******************************************************************************************************
void CFireFloaterScene::Draw()
{
	glPushMatrix();
		
	// clear depth buffer (but not color buffer)
	glClear(GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	float w = GetAppWidth();
	float h = GetAppHeight();

	ofSetColor(50, 50, 50, 200);
	ofRect(0, 0, w, h);
	
	glPushMatrix();
	
	//glTranslatef(w/2, 0, 0);
	//float angle = CRationalVisionsApp::Get()->GetWorldTime()*100.0f;
	//glRotatef(angle, 0, 1, 0);
	//glTranslatef(-w/2, 0, 0);

	
	ofSetColor(255, 0, 0);

	//draw current line...
	ofxVec2f pos;
	float size;
	for(float f = 0.0f; f<=1.0f; f += one_on_num)
	{
		GetPosAndSize(f, pos, size);
		//ofCircle(pos.x, pos.y-size*2.0f, size);
	}
	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//draw particles
	for(int i=0; i<num_particles; ++i)
	{
		if(m_Particles[i]->IsActive() && m_Particles[i]->GetSize() > 0.0f)
		{
			float life = m_Particles[i]->GetLife();
			ofSetColor(255, 255*(life), 0, 255*(life));

			glPushMatrix();
			pos = m_Particles[i]->GetPos();
			size = m_Particles[i]->GetSize();
			glTranslatef(pos.x, pos.y, 0);
			//glRotatef(-angle, 0, 1, 0);
			ofCircle(0, 0, size);
			glPopMatrix();
		}
	}
	
	// disable blending
	glDisable(GL_BLEND);
	
	glPopMatrix();
	
	glPopMatrix();
}