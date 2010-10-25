/*
 *  RationalVisionsApp.h
 *  RationalVisions
 *
 *  Created by Neil Wallace on 18/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"

class CSoundEngine;
class CSceneManager;

//#define SAVE_FRAMES true
#define VIDEO_FPS 30

//-------------------------------------------------------------------------------------------------------
class CRationalVisionsApp : public ofBaseApp 
{
public:
	CRationalVisionsApp();
	
	void			setup();
	void			plot(float* array, int length, float scale, float offset);
	void			audioReceived(float* input, int bufferSize, int nChannels);
	void			update();
	void			draw();
	void			keyPressed(int key);
	
	static CRationalVisionsApp* Get() { return mp_App; }
	
	float			GetWidth() { return ofGetWidth(); }
	float			GetHeight() { return ofGetHeight(); }
	
	float			GetScreenRelativeSize(float size) { return GetWidth() * size; }
	
	float			GetWorldTime() { return m_WorldTime; }
	
	CSoundEngine&	GetSoundEngine() { return *mp_SoundEngine; }
	
private:
	CSoundEngine*	mp_SoundEngine;
	CSceneManager*	mp_SceneManager;
	
	bool			m_Fullscreen;
	static CRationalVisionsApp* mp_App;
	
	float			m_WorldTime;

	ofSoundPlayer music;
};