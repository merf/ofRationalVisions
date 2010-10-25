/*
 *  BasicScope.cpp
 *  RationalVisions
 *
 *  Created by Neil Wallace on 19/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BasicScope.h"

#include "Halftone.h"
#include "RationalVisionsApp.h"
#include "SoundEngine.h"

using namespace NScene;

//*******************************************************************************************************
//*******************************************************************************************************
CBasicScope::CBasicScope() 
: CBaseScene() 
{
}

//*******************************************************************************************************
void CBasicScope::Init()
{
}

//*******************************************************************************************************
void CBasicScope::Update(float time_step)
{
}

//*******************************************************************************************************
void CBasicScope::Draw()
{
	float w = CRationalVisionsApp::Get()->GetWidth();
	float h = CRationalVisionsApp::Get()->GetHeight();
	
	float prev_x = 0.0f;
	float prev_y = h/2.0f;
	
	ofSetColor(0, 0, 0);
	//ofSetColor(255, 255, 255);
	
	int buffer_size = 0;
	float* raw_audio = CRationalVisionsApp::Get()->GetSoundEngine().GetRawAudio(buffer_size);
	for(int i=0; i<buffer_size; i++)
	{
		float x = w*i/(float)buffer_size;
		float t_h = raw_audio[i] * h * 0.25f;
		float y = h/2.0f + t_h;
		
		ofRect(x, h/2.0f, 1, t_h);
		ofLine(prev_x, prev_y, x, y);
		
		prev_x = x;
		prev_y = y;
	}

	static int alpha = 128;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	
	float bar_width = w/(float)(NUM_BARK_SCALE_BANDS-1);
	for(int i=0; i<NUM_BARK_SCALE_BANDS; ++i)
	{
		float f = i/(float)(NUM_BARK_SCALE_BANDS-1);
		float amp = CRationalVisionsApp::Get()->GetSoundEngine().GetShortAverage(f);
		float avg = CRationalVisionsApp::Get()->GetSoundEngine().GetAverage(f);
		float movement = CRationalVisionsApp::Get()->GetSoundEngine().GetMovement(f);
		
		//if(fabs(val) > 0.5f)
		{
			static float bar_height = 0.2f;

			//background...
			ofSetColor(64, 64, 64, alpha);
			ofRect(f*w, h, bar_width, -h*bar_height);

			//grid...
			ofSetColor(96, 96, 96, alpha);
			for(float grid_pos = 0.0f; grid_pos < 1.0f; grid_pos += 0.1f)
			{
				float v = h - grid_pos*bar_height*h;
				ofLine(f*w+bar_width*0.25f, v, f*w+bar_width*0.75f, v);
			}

			//bars...
			float bar_sub_width = bar_width / 8.0f;
			float curr_barr_x = f*w;
			curr_barr_x += bar_sub_width;
			ofSetColor(255, 255, 0, alpha);
			ofRect(curr_barr_x, h, bar_sub_width, -amp*h*bar_height);
			curr_barr_x += bar_sub_width;
			curr_barr_x += bar_sub_width;
			ofSetColor(255, 192, 0, alpha);
			ofRect(curr_barr_x, h, bar_sub_width, -avg*h*bar_height);
			curr_barr_x += bar_sub_width;
			curr_barr_x += bar_sub_width;
			ofSetColor(255, 128, 0, alpha);
			ofRect(curr_barr_x, h, bar_sub_width, -movement*h*bar_height);
		}
	}
}