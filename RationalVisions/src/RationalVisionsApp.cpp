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
const int BUFFER_SIZE = 512;

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
	ofBackground(255,255,255);
	ofBackground(50,50,50);

#if defined TARGET_OSX
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
#else
	//ofSetFrameRate(60);
	ofSetVerticalSync(true);
#endif

	ofEnableSmoothing();
	
	//ofSetBackgroundAuto(false);
	
	// enable depth testing
	//glEnable(GL_DEPTH_TEST);
	
	// select normal blend mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//ofSetFullscreen(true);

	//music.loadSound("music/Mount Kimbie_10_Mayor.mp3");
	//music.play();

	mp_SoundEngine = new CSoundEngine(this, BUFFER_SIZE);

	mp_SceneManager = new CSceneManager();
}

//*******************************************************************************************************
void CRationalVisionsApp::update()
{
	float time_step = 1.0f / (float)ofGetFrameRate();
	//float time_step = 1.0f / 60.0f;
	
#ifdef SAVE_FRAMES
	//time_step = 1.0f / (float)VIDEO_FPS;
#endif

	m_WorldTime += time_step;

	//if(music.getIsPlaying())
	{
		mp_SoundEngine->Update(time_step);
		mp_SceneManager->Update(time_step);
	}
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


#ifdef SAVE_FRAMES
	if(music.getPosition() < 1.0)
	{
		int frame_num = ofGetFrameNum();
		if(frame_num % 2 == 0)
		{
			ofImage frame;
			frame.grabScreen(0,0,ofGetWidth(), ofGetHeight());
			string fileName = "../../../../../../../frames/frameNum" + ofToString(frame_num/2) + ".png";
			frame.saveImage(fileName);
		}
		else 
		{
			exit();
		}

	} 
#endif
}

//*******************************************************************************************************
void CRationalVisionsApp::audioReceived(float* input, int bufferSize, int nChannels) 
{
	//mp_SoundEngine->ReceiveAudio(input, bufferSize, nChannels);
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
		case 'p':
			mp_SoundEngine->Pause();
			break;
		default:
			mp_SceneManager->KeyPressed(key);
			break;
	}
}