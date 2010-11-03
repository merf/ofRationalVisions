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
			glPushMatrix();
			{
				for(int i=0; i<CBeatDetective::HISTORY_SIZE; ++i)
				{
					float f = GetSoundEngine().GetPBeatDetective()->GetBandHistoryItem(band, i);
					ofSetColor(f*255, 0, 0);
					ofRect(0, 0, w_inc, h_inc);
					ofTranslate(w_inc, 0.0f);
				}
			}
			glPopMatrix();
			ofTranslate(0.0f, h_inc);
		}
		glPopMatrix();

		//Channels...
		glPushMatrix();
		ofTranslate(0.0f, (1.0f-2*h_percent_for_ticker) * h);
		for(int i=0; i<CBeatDetective::HISTORY_SIZE; ++i)
		{
			glPushMatrix();
			{
				for(int channel=0; channel<CBeatDetective::NUM_CHANNELS; ++channel)
				{
					float channel_height = GetSoundEngine().GetPBeatDetective()->GetChannelMaxBand(channel);

					if(channel > 0)
					{
						channel_height -= GetSoundEngine().GetPBeatDetective()->GetChannelMaxBand(channel-1);
					}

					float colour_val = channel / (float)(CBeatDetective::NUM_CHANNELS-1);
					if(GetSoundEngine().GetPBeatDetective()->IsBeat(i, channel))
					{
						ofSetColor(255*colour_val, 0, 255);
					}
					else
					{
						ofSetColor(255, 255, 255);
					}

					float val = GetSoundEngine().GetPBeatDetective()->GetChannelHistoryItem(channel, i);
					ofLine(0.0f, h_percent_for_ticker*h*channel_height*(1.0f-val), 0, h_percent_for_ticker*h*channel_height);

					ofTranslate(0.0f, h_percent_for_ticker*h*channel_height);
				}
			}
			glPopMatrix();
			ofTranslate(w_inc, 0.0f);
		}
		glPopMatrix();

		glPushMatrix();
		ofTranslate(0.0f, (1.0f-2*h_percent_for_ticker) * h);
		for(int channel=0; channel<CBeatDetective::NUM_CHANNELS; ++channel)
		{
			float channel_height = GetSoundEngine().GetPBeatDetective()->GetChannelMaxBand(channel);

			stringstream str;
			float bpm = GetSoundEngine().GetPBeatDetective()->GetBPM(channel);
			str << "BPM = " << bpm;
			static int font_size = 12;
			float curr_y = 0;
			ofSetColor(255, 255, 255);
			ofDrawBitmapString(str.str(), 2, h_percent_for_ticker*h*channel_height-20);

			//ofTranslate(0.0f, h_percent_for_ticker*h*channel_height);
		}
		glPopMatrix();

		//Beat graph...
		//glPushMatrix();
		//ofTranslate(0.0f, (1.0f-3*h_percent_for_ticker) * h);
		//glPushMatrix();
		//for(int i=0; i<CBeatDetective::HISTORY_SIZE; ++i)
		//{
		//	float f = GetSoundEngine().GetPBeatDetective()->GetHistoryItem(i);

		//	if(GetSoundEngine().GetPBeatDetective()->IsBeat(i))
		//	{
		//		ofSetColor(0, 255, 0);
		//	}
		//	else
		//	{
		//		ofSetColor(0, 0, 255);
		//	}

		//	ofRect(0, h_percent_for_ticker*h, w_inc, -h_percent_for_ticker*h*f);

		//	ofTranslate(w_inc, 0.0f);
		//}
		//glPopMatrix();
		//ofSetColor(255, 255, 255);
		//float avg = GetSoundEngine().GetPBeatDetective()->GetAverage();
		//float line_height = h_percent_for_ticker*h-h_percent_for_ticker*h*avg;
		//ofLine(0, line_height, w, line_height);
		//ofSetColor(255, 0, 0);
		//float var = GetSoundEngine().GetPBeatDetective()->GetVariance();
		//line_height = h_percent_for_ticker*h-h_percent_for_ticker*h*(avg+var);
		//ofLine(0, line_height, w, line_height);
		//line_height = h_percent_for_ticker*h-h_percent_for_ticker*h*(avg-var);
		//ofLine(0, line_height, w, line_height);
		//glPopMatrix();


		//stringstream fps_str;
		//float phase = GetSoundEngine().GetPBeatDetective()->GetPhase();
		//fps_str << "Phase = " << phase;
		//static int font_size = 12;
		//float curr_y = 0;
		//ofSetColor(255, 255, 255);
		//ofDrawBitmapString(fps_str.str(), 2, 50);

		//glPushMatrix();
		//ofTranslate(0.0f, (1.0f-3*h_percent_for_ticker) * h);
		//int counter = 0;
		//for(int i=0; i<CBeatDetective::HISTORY_SIZE; ++i)
		//{
		//	counter++;
		//	if(counter > phase)
		//	{
		//		ofLine(0, 0, 0, line_height);
		//		counter = 0;
		//	}

		//	ofTranslate(w_inc, 0.0f);
		//}
		//glPopMatrix();
	}
}