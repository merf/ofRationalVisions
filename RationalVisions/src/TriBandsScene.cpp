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

		float num = 100.0f;

		float w_inc = w / num;
		for(float f=0; f<=1.0f; f+=1.0f/num)
		{
			ofTranslate(w_inc, 0.0f);
			float val = -GetSoundEngine().GetShortAverage(f) * h * 0.2f;
			ofTriangle(curr_w, base_h, curr_w + w_inc * 0.5f, base_h + val, curr_w + w_inc, base_h);
		}
	}


}