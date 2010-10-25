/*
 *  RationalVisionsApp.cpp
 *  RationalVisions
 *
 *  Created by Neil Wallace on 18/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "RationalVisionsApp.h"

#include "SceneManager.h"
#include "SoundEngine.h"

CRationalVisionsApp* CRationalVisionsApp::mp_App = NULL;

//*******************************************************************************************************
CRationalVisionsApp::CRationalVisionsApp()
:
m_Fullscreen(false),
m_WorldTime(0.0f)
{ 
	mp_App = this; 
}

//*******************************************************************************************************
void CRationalVisionsApp::setup()
{
	mp_SoundEngine = new CSoundEngine(this, 512);
	
	mp_SceneManager = new CSceneManager();
	
	ofBackground(255,255,255);
	ofBackground(50,50,50);
	//ofSetFrameRate(60);
	//ofSetVerticalSync(true);
	
	//ofSetBackgroundAuto(false);
	
	// enable depth testing
	//glEnable(GL_DEPTH_TEST);
	
	// select normal blend mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//ofSetFullscreen(true);
}

//*******************************************************************************************************
void CRationalVisionsApp::update()
{
	m_WorldTime += 1.0f / ofGetFrameRate();
	mp_SoundEngine->Update();
	mp_SceneManager->Update();
}

//*******************************************************************************************************
void CRationalVisionsApp::draw()
{
	ofSetColor(0,0,0);
	stringstream fps_str;
	fps_str << "FPS: " << ofGetFrameRate();
	static int font_size = 12;
	static int gap_size = 2;
	float curr_y = 0;
	ofDrawBitmapString(fps_str.str(), gap_size, curr_y + font_size);
	curr_y += font_size + gap_size;
	stringstream file_str;
	file_str << "File=" << *GetSoundEngine().GetCurrentFile();
	ofDrawBitmapString(file_str.str(), gap_size, curr_y + font_size);
	mp_SceneManager->Draw();
}

//*******************************************************************************************************
void CRationalVisionsApp::audioReceived(float* input, int bufferSize, int nChannels) 
{
	mp_SoundEngine->ReceiveAudio(input, bufferSize, nChannels);
}

//*******************************************************************************************************
void CRationalVisionsApp::keyPressed(int key) 
{
	switch (key) {
		case 'f':
			m_Fullscreen = !m_Fullscreen;
			ofSetFullscreen(m_Fullscreen);
			break;
		case '=':
			mp_SoundEngine->NextAudioFile();
			break;
		case ']':
			mp_SoundEngine->FFD();
			break;
		case '[':
			mp_SoundEngine->RWD();
			break;
		default:
			mp_SceneManager->KeyPressed(key);
			break;
	}
}