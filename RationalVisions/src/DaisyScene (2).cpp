/*
 *  DaisyScene.cpp
 *  RationalVisions
 *
 *  Created by Neil Wallace on 13/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DaisyScene.h"
#include "BasicParticle.h"
#include "SoundEngine.h"
#include "RationalVisionsApp.h"

namespace NScene
{
	const int num_particles = 50.0f;
	const float one_on_num = 1.0f / (float)num_particles;
	
	//*******************************************************************************************************
	//*******************************************************************************************************
	CDaisyScene::CDaisyScene()
	: CBaseScene()
	{
	}
	
	//*******************************************************************************************************
	void CDaisyScene::Init()
	{
		for(int i=0; i<num_particles; i++)
		{
			CBasicParticle* p = new CBasicParticle();
			m_Particles.push_back(new CBasicParticle());
		}
	}

	//*******************************************************************************************************
	void CDaisyScene::Draw()
	{
		float h = GetAppHeight();
		float w = GetAppWidth();
		
		// clear depth buffer (but not color buffer)
		glClear(GL_DEPTH_BUFFER_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		
		glPushMatrix();

		ofxVec2f centre = ofxVec2f(w/2.0f, h/2.0f);
		
		float time = GetWorldTime();
		//centre += ofxVec2f(GetSoundEngine().GetMovement(0.2f)*w*0.05, GetSoundEngine().GetMovement(0.6f)*h*0.05f);
		
		float f = 0.0f;
		
		for(TParticleList::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it)
		{
			CBasicParticle* p_particle = *it;
			
			ofxVec2f pos = centre + p_particle->GetPos() * w * 0.5f;
			
			
			float size = GetSoundEngine().GetAverage(f);
			RMath::LimitUpper(size, 0.5f);
			size *= (1.0f + GetSoundEngine().GetMovement(f));
			//size *= 0.1f;

			float dist = p_particle->GetPos().length() * 100.0f;
			RMath::Limit(1.0f, dist, 100.0f);
			
			float size_mul = 1.0f;
			glPushMatrix();

			float alpha = dist * 2.0f;
			DrawCircleRecursive(dist, size, f, pos, alpha);

			glPopMatrix();
			f += one_on_num;
		}
		
		glDisable(GL_BLEND);
		
		glPopMatrix();
	}

	//*******************************************************************************************************
	void CDaisyScene::DrawCircleRecursive(float dist, float size, float f, ofxVec2f pos, float alpha)
	{
		glTranslatef(pos.x, pos.y, 0);

		int num = (int)dist;

		static int num_per_recurse = 25;

		if(num > num_per_recurse)
		{
			dist *= 0.5f;

			float angle = GetWorldTime() * 2.0f;
			float inc_num = num_per_recurse / 4;
			float angle_inc = TWO_PI / (float)inc_num;

			for(int i=0; i<inc_num; i++)
			{
				angle += angle_inc;

				ofxVec2f final_pos = ofxVec2f(sin(angle), -cos(angle));
				final_pos *= 3.0f * dist;
				glPushMatrix();
				DrawCircleRecursive(dist, size*0.7f, f, final_pos, alpha);
				glPopMatrix();
			}
		}
		else
		{
			//dist -= 1.0f;
			ofSetColor(255, 255*f, 0.0f, alpha);
			//ofCircle(pos.x, pos.y, GetApp().GetScreenRelativeSize(0.05f));
			ofCircle(0, 0, GetApp().GetScreenRelativeSize(0.05f*size));


			//float angle = GetWorldTime() * 2.0f;
			//float angle_inc = TWO_PI / dist;
			//size /= (dist/10.0f);
			//RMath::Limit(0.0f, size, 0.1f);

			//dist -= 1.0f;
			//ofSetColor(255, 255*f, 0.0f, dist*dist);

			//for(int i=0; i<num; ++i)
			//{
			//	angle += angle_inc;
			//	ofxVec2f final_pos = pos + ofxVec2f(sin(angle), -cos(angle)) * dist * 2.0f;
			//	ofCircle(final_pos.x, final_pos.y, GetApp().GetScreenRelativeSize(0.1f*size*size_mul));
			//}
		}
	}
	
	//*******************************************************************************************************
	void CDaisyScene::Update()
	{
		float f = 0.0f;
		for(TParticleList::iterator it = m_Particles.begin(); it != m_Particles.end(); ++it)
		{
			float movement = GetSoundEngine().GetMovement(f);
			
			float angle = f * TWO_PI - GetWorldTime();
			
			CBasicParticle* p_particle = *it;

			//ofxVec2f new_pos = ofxVec2f(cos(angle), -sin(angle));
			//new_pos *= cos(GetWorldTime());
			//p_particle->SetPos(new_pos);
			
			static float scaler = 0.005f;
			if(movement > 0.0f)
			{
				float dist = p_particle->GetPos().length() + 0.01f;
				ofxVec2f vel_inc = ofxVec2f(cos(angle), -sin(angle)) * movement * scaler * 1.0f/dist;			
				p_particle->IncVel(vel_inc);
			}
			else 
			{
				ofxVec2f to_centre = -p_particle->GetPos();
				to_centre = to_centre * 0.01f;
				p_particle->IncVel(to_centre);
			}

			p_particle->DampVel(0.5f);
			
			p_particle->Update();
			
			f += one_on_num;
		}
	}
}
