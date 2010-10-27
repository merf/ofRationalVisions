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
		for(int i=0; i<CBeatDetective::HISTORY_SIZE; ++i)
		{
			glPushMatrix();
			{
				for(int band=0; band<NUM_BARK_SCALE_BANDS; ++band)
				{
					float f = GetSoundEngine().GetPBeatDetective()->GetHistoryItem(band, i);
					ofSetColor(f*255, 0, 0);
					ofRect(0, 0, w_inc, h_inc);
					ofTranslate(0.0f, h_inc);
				}
			}
			glPopMatrix();
			ofTranslate(w_inc, 0.0f);
		}
		glPopMatrix();
	}
}