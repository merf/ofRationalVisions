#include "TriBandsScene.h"
#include "RationalVisionsApp.h"
#include "SoundEngine.h"

namespace NScene
{
	//*******************************************************************************************************
	//*******************************************************************************************************
	void CTriBandsScene::Init()
	{

	}

	//*******************************************************************************************************
	void CTriBandsScene::Update()
	{

	}

	//*******************************************************************************************************
	void CTriBandsScene::Draw()
	{
		float w = GetApp().GetWidth();
		float h  = GetApp().GetHeight();

		float base_h = h / 2.0f;
		float curr_w = 0.0f;

		float num = 25.0f;
		
		const float tan_60 = tan(60.0f * DEG_TO_RAD);
		
		ofSetColor(255, 255, 255);

		float w_inc = w / num;
		
		float volume_zoom_factor = 100.0f;
		
		ofTranslate(-w_inc*0.5f, 0.0f);
		
		

		for(float f=0; f<=1.0f; f+=1.0f/num)
		{
			ofTranslate(w_inc, 0.0f);
			float height = -GetSoundEngine().GetShortAverage(f) * w_inc;
			float half_width = height / tan_60;
			ofTriangle(-half_width, base_h, 0, base_h + height, half_width, base_h);
		}
	}


}