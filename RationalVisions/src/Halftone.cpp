/*
 *  Halftone.cpp
 *  RationalVisions
 *
 *  Created by Neil Wallace on 19/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Halftone.h"
#include "RationalVisionsApp.h"
#include "SoundEngine.h"

using namespace NScene;

//*******************************************************************************************************
//*******************************************************************************************************
CHalftone::CHalftone() 
: CBaseScene() 
{
};

//*******************************************************************************************************
void CHalftone::Init()
{
}

//*******************************************************************************************************
void CHalftone::Update(float time_step)
{
}

//*******************************************************************************************************
void CHalftone::Draw()
{
	glPushMatrix();
	
	float w = CRationalVisionsApp::Get()->GetWidth();
	float h = CRationalVisionsApp::Get()->GetHeight();
	
	glTranslatef(w/2.0f, 0, 0);
	
	float width_to_use = 0.8f;
	glTranslatef(-w/2.0f * width_to_use, 0, 0);
	
	float num = 25.0f;
	float one_on_num = 1.0f / num;
	
	bool flipper = true;
	
	for(float j = 0.0f; j<=1.19f; j += one_on_num)
	{
		float x_off = 0.0f;
		if(flipper)
		{
			x_off = one_on_num * w * width_to_use * 0.5f;
		}
		flipper = !flipper;
		
		float curr_h = h*width_to_use*j;
		glTranslatef(0, one_on_num*h*width_to_use, 0);
		
		glPushMatrix();
		glTranslatef(x_off, 0, 0);
		
		for(float f = 0.0f; f<=1.0f; f += one_on_num)
		{
			glTranslatef(one_on_num * w * width_to_use, 0, 0);
			ofSetColor(255,f*255, 0);
			float raw_size = CRationalVisionsApp::Get()->GetSoundEngine().GetAmplitude(f);
			float dist_size = 1.0f-fabs((curr_h - h/2.0f) / h);
			raw_size *= dist_size;
			
			ofCircle(0, 0, raw_size*CRationalVisionsApp::Get()->GetScreenRelativeSize(0.01f));
		}
		glPopMatrix();
	}
	glPopMatrix();
}