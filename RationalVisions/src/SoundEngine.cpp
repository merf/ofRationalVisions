/*
 *  SoundEngine.cpp
 *  RationalVisions
 *
 *  Created by Neil Wallace on 18/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "RationalVisionsApp.h"
#include "SoundEngine.h"
#include "ofxFft.h"

#include "RMath.h"

float sampling_freq = 44100.0f;
ENormalizationStrategy g_NormalizationStrategy = SMOOTH_LIMIT_BARK_BANDS;
static bool from_file = true;
static bool stream_files = true;

const string MUSIC_DIR = "music/";

static int static_short_history_size = 3;
static int static_long_history_size = 50;

static int short_history_size = 3;
static int long_history_size = 50;

//*******************************************************************************************************
CSoundEngine::CSoundEngine(ofBaseApp* p_parent_app, int buffer_size)
:
m_BufferSize(buffer_size),
m_SmoothVolume(1.0f),
m_SmoothBarkBandMax(0.0f),
m_CurrentFile("Live Audio")
{
	Init(p_parent_app);
}

//*******************************************************************************************************
void CSoundEngine::Init(ofBaseApp* p_parent_app)
{
	if(from_file)
	{
		m_NumFFTBands = NUM_FFT_BANDS;
		mp_SoundPlayer = new ofSoundPlayer();

		m_DirList.allowExt("mp3");
		NextAudioFile();

	}
	else
	{
		//mp_FFT = ofxFft::create(m_BufferSize, OF_FFT_WINDOW_BARTLETT, OF_FFT_FFTW);
		mp_FFT = ofxFft::create(m_BufferSize, OF_FFT_WINDOW_BARTLETT);
		m_NumFFTBands = mp_FFT->getBinSize();
	}
	// To use with FFTW, try:
	// fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_BARTLETT, OF_FFT_FFTW);
	
	mp_RawAudio = new float[m_BufferSize];
	mp_FFTOutput = new float[m_NumFFTBands];
	
	//bins are 0-nyquist - ie 0Hz - 22,500Hz
	//we want to clump according bark scale - freq bands are:
	//20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500.
	//http://en.wikipedia.org/wiki/Bark_scale
	
	m_BarkBandFrequencies[0] = 20;
	m_BarkBandFrequencies[1] = 100;
	m_BarkBandFrequencies[2] = 200;
	m_BarkBandFrequencies[3] = 300;
	m_BarkBandFrequencies[4] = 400;
	m_BarkBandFrequencies[5] = 510;
	m_BarkBandFrequencies[6] = 630;
	m_BarkBandFrequencies[7] = 770;
	m_BarkBandFrequencies[8] = 920;
	m_BarkBandFrequencies[9] = 1080;
	m_BarkBandFrequencies[10] = 1270;
	m_BarkBandFrequencies[11] = 1480;
	m_BarkBandFrequencies[12] = 1720;
	m_BarkBandFrequencies[13] = 2000;
	m_BarkBandFrequencies[14] = 2320;
	m_BarkBandFrequencies[15] = 2700;
	m_BarkBandFrequencies[16] = 3150;
	m_BarkBandFrequencies[17] = 3700;
	m_BarkBandFrequencies[18] = 4400;
	m_BarkBandFrequencies[19] = 5300;
	m_BarkBandFrequencies[20] = 6400;
	m_BarkBandFrequencies[21] = 7700;
	m_BarkBandFrequencies[22] = 9500;
	m_BarkBandFrequencies[23] = 12000;
	m_BarkBandFrequencies[24] = 15500;
	
	binToBark = new int[m_NumFFTBands];
	int bark = 0;
	int count = 0;
	for(int i=0; i<m_NumFFTBands; ++i)
	{
		float f = (float)i / (float)m_NumFFTBands;
		float freq = sampling_freq * 0.5f * f;
		if(freq > m_BarkBandFrequencies[bark] && bark <= NUM_BARK_SCALE_BANDS-1)
		{
			m_BarkBandMultipliers[bark] = 1.0f / count;
			count = 0;

			++bark;
		}
		++count;
		binToBark[i] = bark;
	}	

	float div = 60.0f / ofGetFrameRate();

	long_history_size = ceil(long_history_size/div);
	short_history_size = ceil(short_history_size/div);
	
	for(int i=0; i<NUM_BARK_SCALE_BANDS; ++i)
	{
		m_BarkLongAverages[i] = 0.1f;

		for(int j=0; j<long_history_size; ++j)
		{
			m_History[i].push_back(0.0f);
		}
	}

	//memset(m_BarkLongAverages, 1, sizeof(float) * NUM_BARK_SCALE_BANDS);
	memset(m_BarkShortAverages, 0, sizeof(float) * NUM_BARK_SCALE_BANDS);
	memset(mp_RawAudio, 0, sizeof(float) * m_BufferSize);
	
	if(!from_file)
	{
		ofSoundStreamSetup(0, 1, p_parent_app, sampling_freq, m_BufferSize, 4);
	}
}

//*******************************************************************************************************
void CSoundEngine::NextAudioFile()
{
	if(from_file)
	{
		mp_SoundPlayer->stop();
		int num_files = m_DirList.listDir(MUSIC_DIR);
		int rand_file = ofRandom(0, num_files);
		mp_SoundPlayer->loadSound(m_DirList.getPath(rand_file), stream_files);

#ifdef SAVE_FRAMES
		mp_SoundPlayer->loadSound("music/Mount Kimbie_10_Mayor.mp3");
#endif
		
		//mp_SoundPlayer->loadSound("test.mp3");
		mp_SoundPlayer->play();

		m_CurrentFile = m_DirList.getName(rand_file);
	}
}

//*******************************************************************************************************
void CSoundEngine::FFD()
{
	if(from_file)
	{
		static float percent_to_skip = 0.1f;
		float new_pos = mp_SoundPlayer->getPosition() + percent_to_skip;
		if(new_pos > 1.0f)
		{
			NextAudioFile();
		}
		else
		{
			mp_SoundPlayer->setPosition(new_pos);
		}
	}
}

//*******************************************************************************************************
void CSoundEngine::RWD()
{
	if(from_file)
	{
		static float percent_to_skip = 0.1f;
		float new_pos = mp_SoundPlayer->getPosition() - percent_to_skip;
		if(new_pos < 0.0f)
		{
			new_pos = 0.0f;
		}
		mp_SoundPlayer->setPosition(new_pos);
	}
}

//*******************************************************************************************************
float CSoundEngine::GetAmplitude(float f)
{
	return GetValue(m_BarkScaleValues, f);

	float f_bin = f * NUM_BARK_SCALE_BANDS;
	int bark_bin = floor(f_bin);
	if(bark_bin == NUM_BARK_SCALE_BANDS - 1)
	{
		return m_BarkScaleValues[bark_bin];
	}
	else
	{
		f_bin -= bark_bin;
		return RMath::Lerp(m_BarkScaleValues[bark_bin], m_BarkScaleValues[bark_bin+1], f_bin);
	}
}

//*******************************************************************************************************
float CSoundEngine::GetAverage(float f)
{
	return GetValue(m_BarkLongAverages, f);

	float f_bin = f * NUM_BARK_SCALE_BANDS;
	int bark_bin = floor(f_bin);
	if(bark_bin == NUM_BARK_SCALE_BANDS - 1)
	{
		return m_BarkLongAverages[bark_bin];
	}
	else
	{
		f_bin -= bark_bin;
		return RMath::Lerp(m_BarkLongAverages[bark_bin], m_BarkLongAverages[bark_bin+1], f_bin);
	}
}

//*******************************************************************************************************
float CSoundEngine::GetShortAverage(float f)
{
	return GetValue(m_BarkShortAverages, f);

	float f_bin = f * NUM_BARK_SCALE_BANDS;
	int bark_bin = floor(f_bin);
	if(bark_bin == NUM_BARK_SCALE_BANDS - 1)
	{
		return m_BarkShortAverages[bark_bin];
	}
	else
	{
		f_bin -= bark_bin;
		return RMath::Lerp(m_BarkShortAverages[bark_bin], m_BarkShortAverages[bark_bin+1], f_bin);
	}
}

//*******************************************************************************************************
float CSoundEngine::GetVariance(float f)
{
	return GetValue(m_BarkVariance, f);
}
//*******************************************************************************************************
float CSoundEngine::GetValue(float* vals, float f)
{
	float f_bin = f * NUM_BARK_SCALE_BANDS;
	int bark_bin = floor(f_bin);
	if(bark_bin == NUM_BARK_SCALE_BANDS - 1)
	{
		return vals[bark_bin];
	}
	else
	{
		f_bin -= bark_bin;
		return RMath::Lerp(vals[bark_bin], vals[bark_bin+1], f_bin);
	}
}

//*******************************************************************************************************
float CSoundEngine::GetMovement(float f)
{
	//TODO perhaps movement etc. should be calculated once per frame and cached...
	static bool use_variance = true;
	if(use_variance)
	{
		float long_average = GetAverage(f);
		float short_average = GetShortAverage(f);

		float diff = short_average - long_average;
		
		float diff_sq = pow(diff, 2);
		float variance = GetVariance(f);
		float movement_mag = diff_sq / variance;

		static float one_on_num_standard_dev_that_counts_as_max = 1.0f / 3.0f;
		movement_mag *= one_on_num_standard_dev_that_counts_as_max;

		if(diff < 0)
		{
			movement_mag *= -1.0f;
		}

		RMath::Limit(-1.0f, movement_mag, 1.0f);
		return movement_mag;
	}
	else
	{
		float long_average = GetAverage(f);
		float short_average = GetShortAverage(f);

		float diff = short_average - long_average;

		static float amount_for_max = 0.5f;

		float val = diff/(amount_for_max * long_average);

		RMath::Limit(-1.0f, val, 1.0f);

		return val;
	}
}

//*******************************************************************************************************
float* CSoundEngine::GetRawAudio(int& buffer_size)
{
	buffer_size = m_BufferSize;
	return mp_RawAudio;
}

//*******************************************************************************************************
void CSoundEngine::Update(float time_step)
{
	if(from_file)
	{
#ifdef SAVE_FRAMES
		float totalFrames = (mp_SoundPlayer->length / mp_SoundPlayer->internalFreq) * 60.0f;
		float musicPos = (float)ofGetFrameNum() / totalFrames;
		mp_SoundPlayer->setPosition(musicPos);
#endif

		mp_FFTOutput = ofSoundGetSpectrum(m_NumFFTBands);
		mp_RawAudio = ofSoundGetWave(m_BufferSize);

		//ofSoundUpdate();
		ProcessAudio(time_step);
	}
}

//*******************************************************************************************************
void CSoundEngine::ReceiveAudio(float* input, int buffer_size, int num_channels)
{
	if(!from_file)
	{
		// store input buffer
		memcpy(mp_RawAudio, input, sizeof(float) * buffer_size);

		// get FFT
		mp_FFT->setSignal(mp_RawAudio);
		memcpy(mp_FFTOutput, mp_FFT->getAmplitude(), sizeof(float) * m_NumFFTBands);

		ProcessAudio(1.0f / 60.0f);
	}
}

//*******************************************************************************************************
void CSoundEngine::ProcessAudio(float time_step)
{
	float max_vol = 0.0f;
	float avg_vol = 0.0f;
	for(int i = 0; i < m_NumFFTBands; i++)
	{
		if(mp_FFTOutput[i] > max_vol)
		{
			max_vol = mp_FFTOutput[i];
		}
		avg_vol += mp_FFTOutput[i];
	}
	avg_vol /= m_NumFFTBands;

	static float min_allowed = 0.01f;
	RMath::LimitLower(min_allowed, avg_vol);

	m_SmoothVolume = RMath::Lerp(m_SmoothVolume, avg_vol, 0.1f);
	m_SmoothVolume = RMath::Lerp(m_SmoothVolume, max_vol, 0.1f);

	memset(m_BarkScaleValues, 0, sizeof(float) * NUM_BARK_SCALE_BANDS);
	
	//chunk up the FFT into the bark scale bins...
	for(int i = 0; i < m_NumFFTBands; i++)
	{
		int bark = binToBark[i];
		m_BarkScaleValues[bark] += mp_FFTOutput[i];
	}
	
	NormalizeAudio();

	//TODO - try using proper averages as is seems this can sometimes get into a situation where the short
	//average is always greater than the long average :(
	for(int i=0; i<NUM_BARK_SCALE_BANDS; ++i)
	{
		float popped_val = m_History[i].back();
		m_History[i].pop_back();
		m_History[i].push_front(m_BarkScaleValues[i]);

		float long_avg = 0.0f;
		float short_avg = 0.0f;
		float variance = 0.0f;

		int curr_el = 0;
		for(std::list<float>::iterator it = m_History[i].begin(); it != m_History[i].end(); ++it)
		{
			long_avg += *it;

			if(curr_el < short_history_size)
			{
				short_avg += *it;
			}

			variance += pow((m_BarkLongAverages[i] - *it), 2);

			curr_el++;
		}

		long_avg /= (float)long_history_size;
		variance /= (float)long_history_size;
		short_avg /= (float)short_history_size;

		m_BarkLongAverages[i] = long_avg;
		m_BarkShortAverages[i] = short_avg;
		m_BarkVariance[i] = variance;

		//static float long_num = 50;
		//m_BarkLongAverages[i] -= m_BarkLongAverages[i] / long_num;
		//m_BarkLongAverages[i] += m_BarkScaleValues[i] / long_num;
		//RMath::LimitUpper(m_BarkLongAverages[i], 1.0f);
		//
		//static float short_num = 5;
		//m_BarkShortAverages[i] -= m_BarkShortAverages[i] / short_num;
		//m_BarkShortAverages[i] += m_BarkScaleValues[i] / short_num;
		//RMath::LimitUpper(m_BarkShortAverages[i], 1.0f);
	}	
}

//*******************************************************************************************************
void CSoundEngine::NormalizeAudio()
{
	switch(g_NormalizationStrategy)
	{
	case NORMALIZE_CLAMP_BARK_BANDS:
		{
			//find the max
			float max_bark = 0.0f;
			for(int i=0; i<NUM_BARK_SCALE_BANDS; ++i)
			{
				if(m_BarkScaleValues[i] > max_bark)
				{
					max_bark = m_BarkScaleValues[i];
				}
			}

			//multiply through by 1/max;
			float normalization_mul = 1.0f / max_bark;
			if(m_SmoothVolume > 0.0f)
			{
				for(int i=0; i<NUM_BARK_SCALE_BANDS; ++i)
				{
					m_BarkScaleValues[i] *= normalization_mul;
				}
			}
		}

		break;
	case SMOOTH_LIMIT_BARK_BANDS:
		{
			//find the max
			float max_bark = 0.0f;
			for(int i=0; i<NUM_BARK_SCALE_BANDS; ++i)
			{
				if(m_BarkScaleValues[i] > max_bark)
				{
					max_bark = m_BarkScaleValues[i];
				}
			}

			max_bark += 0.1f;

			static float lerp_speed_inc = 0.1f;
			static float lerp_speed_dec = 0.01f;

			if(max_bark > m_SmoothBarkBandMax)
			{
				m_SmoothBarkBandMax = RMath::Lerp(m_SmoothBarkBandMax, max_bark, lerp_speed_inc);
			}
			else
			{
				m_SmoothBarkBandMax = RMath::Lerp(m_SmoothBarkBandMax, max_bark, lerp_speed_dec);
			}

			//multiply through by 1/max;
			float normalization_mul = 1.0f / m_SmoothBarkBandMax;
			if(m_SmoothVolume > 0.0f)
			{
				for(int i=0; i<NUM_BARK_SCALE_BANDS; ++i)
				{
					m_BarkScaleValues[i] *= normalization_mul;
					RMath::LimitUpper(m_BarkScaleValues[i], 1.0f);
				}
			}
		}

		break;

	}
}