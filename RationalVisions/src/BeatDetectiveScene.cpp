#include "BeatDetectiveScene.h"
#include "RationalVisionsApp.h"
#include "SoundEngine.h"
#include "BeatDetection.h"

namespace NScene
{
	//*******************************************************************************************************
	//*******************************************************************************************************
	void CBeatDetectiveScene::Init()
	{

	}

	//*******************************************************************************************************
	void CBeatDetectiveScene::Draw()
	{
		float w = GetApp().GetWidth();
		float h  = GetApp().GetHeight();

		float base_h = h / 2.0f;
		float curr_w = 0.0f;

		float num = CBeatDetective::HISTORY_SIZE;

		float w_inc = w / num;
		float h_inc = h / (float)NUM_BARK_SCALE_BANDS;

		float h_percent_for_ticker = 0.2f;
		h_inc *= h_percent_for_ticker;

		glPushMatrix();
		ofTranslate(0.0f, (1.0f-h_percent_for_ticker) * h);

		for(int band=0; band<NUM_BARK_SCALE_BANDS; ++band)
		{
			float entropy = GetSoundEngine().GetPBeatDetective()->GetEntropy(band);

			glPushMatrix();
			{
				for(int i=0; i<CBeatDetective::HISTORY_SIZE; ++i)
				{
					float f = GetSoundEngine().GetPBeatDetective()->GetHistoryItem(band, i);
					ofSetColor(f*255, entropy*255, 0);
					ofRect(0, 0, w_inc, h_inc);
					ofTranslate(w_inc, 0.0f);
				}
			}
			glPopMatrix();
			ofTranslate(0.0f, h_inc);
		}
		glPopMatrix();

		//HI-LO bands...
		glPushMatrix();
		ofTranslate(0.0f, (1.0f-2*h_percent_for_ticker) * h);
		for(int i=0; i<CBeatDetective::HISTORY_SIZE; ++i)
		{
			float lo_total = 0.0f;
			float hi_total = 0.0f;

			float hi_band_size = 0.75f;
			float lo_band_size = 1.0f - hi_band_size;

			glPushMatrix();
			{
				int band=0;
				for(; band<NUM_BARK_SCALE_BANDS*lo_band_size; ++band)
				{
					float f = GetSoundEngine().GetPBeatDetective()->GetHistoryItem(band, i);
					lo_total += f;
				}
				for(; band<NUM_BARK_SCALE_BANDS; ++band)
				{
					float f = GetSoundEngine().GetPBeatDetective()->GetHistoryItem(band, i);
					hi_total += f;
				}
			}

			hi_total /= (NUM_BARK_SCALE_BANDS*hi_band_size);
			lo_total /= (NUM_BARK_SCALE_BANDS*lo_band_size);

			ofSetColor(lo_total*255, 0, lo_total*255);
			ofRect(0, 0, w_inc, h_percent_for_ticker*h*lo_band_size);

			ofTranslate(0.0f, h_percent_for_ticker*h*lo_band_size);

			ofSetColor(hi_total*255, hi_total*255, 0);
			ofRect(0, 0, w_inc, h_percent_for_ticker*h*hi_band_size);

			glPopMatrix();
			ofTranslate(w_inc, 0.0f);
		}
		glPopMatrix();

		//Beat graph...
		glPushMatrix();
		ofTranslate(0.0f, (1.0f-3*h_percent_for_ticker) * h);
		glPushMatrix();
		for(int i=0; i<CBeatDetective::HISTORY_SIZE; ++i)
		{
			//ofSetColor(255, 255, 255);
			//ofRect(0, h_percent_for_ticker*h, w_inc, -h_percent_for_ticker*h);
			//ofSetColor(128, 128, 128);
			//ofRect(0, h_percent_for_ticker*h, w_inc, -h_percent_for_ticker*h*GetSoundEngine().GetPBeatDetective()->GetAverage());

			float total = 0.0f;

			float f = GetSoundEngine().GetPBeatDetective()->GetHistoryItem(i);

			if(GetSoundEngine().GetPBeatDetective()->IsBeat(i))
			{
				ofSetColor(255, 0, 255);
			}
			else
			{
				ofSetColor(0, 0, 255);
			}

			ofRect(0, h_percent_for_ticker*h, w_inc, -h_percent_for_ticker*h*f);

			ofTranslate(w_inc, 0.0f);
		}
		glPopMatrix();
		ofSetColor(255, 255, 255);
		float avg = GetSoundEngine().GetPBeatDetective()->GetAverage();
		float line_height = h_percent_for_ticker*h-h_percent_for_ticker*h*avg;
		ofLine(0, line_height, w, line_height);
		ofSetColor(255, 0, 0);
		float var = GetSoundEngine().GetPBeatDetective()->GetVariance();
		line_height = h_percent_for_ticker*h-h_percent_for_ticker*h*(avg+var);
		ofLine(0, line_height, w, line_height);
		line_height = h_percent_for_ticker*h-h_percent_for_ticker*h*(avg-var);
		ofLine(0, line_height, w, line_height);
		glPopMatrix();
	}
}