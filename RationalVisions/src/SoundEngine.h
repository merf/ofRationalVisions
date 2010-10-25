/*
 *  SoundEngine.h
 *  RationalVisions
 *
 *  Created by Neil Wallace on 18/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "ofxDirList/src/ofxDirList.h"

class ofxFft;
class ofBaseApp;
class ofSoundPlayer;
class CRationalVisionsApp;

#define NUM_BARK_SCALE_BANDS 25
#define NUM_FFT_BANDS 512

enum ENormalizationStrategy
{
	NORMALIZE_CLAMP_BARK_BANDS,
	SMOOTH_LIMIT_BARK_BANDS,
};

//-------------------------------------------------------------------------------------------------------
class CSoundEngine
{
public:
	CSoundEngine(ofBaseApp* p_parent_app, int buffer_size);
	
	void		Init(ofBaseApp* p_parent_app);
	void		Update(float time_step);
	void		Draw();
	

	void		ReceiveAudio(float* input, int buffer_size, int num_channels);
	float*		GetRawAudio(int& buffer_size);



	void		NextAudioFile();
	string*		GetCurrentFile() { return &m_CurrentFile; }

	void		FFD();
	void		RWD();
	


	float		GetAmplitude(float f);
	float		GetAverage(float f);
	float		GetShortAverage(float f);
	float		GetMovement(float f);

	float		GetSmoothVolume() { return m_SmoothVolume; }

	ofSoundPlayer*	mp_SoundPlayer;

private:
	void		ProcessAudio(float time_step);
	void		NormalizeAudio();

	float		GetVariance(float f);
	float		GetValue(float* vals, float f);

	ofxDirList		m_DirList;
	string			m_CurrentFile;

	ofxFft* mp_FFT;
	
	float* mp_RawAudio;
	float* mp_FFTOutput;
	
	float m_BarkScaleValues[NUM_BARK_SCALE_BANDS];
	float m_BarkShortAverages[NUM_BARK_SCALE_BANDS];
	float m_BarkLongAverages[NUM_BARK_SCALE_BANDS];
	float m_BarkVariance[NUM_BARK_SCALE_BANDS];
	float m_BarkBandFrequencies[NUM_BARK_SCALE_BANDS];
	float m_BarkBandMultipliers[NUM_BARK_SCALE_BANDS];

	std::list<float>		m_History[NUM_BARK_SCALE_BANDS];
	
	int* binToBark;
	
	int m_BufferSize;
	int m_NumFFTBands;
	
	float m_SmoothVolume;
	float m_SmoothBarkBandMax;

	friend class CRationalVisionsApp;
};