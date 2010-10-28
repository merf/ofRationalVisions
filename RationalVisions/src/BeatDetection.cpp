#include "BeatDetection.h"
#include "SoundEngine.h"
#include "RationalVisionsApp.h"
#include "RMath.h"

const int CBeatDetective::HISTORY_SIZE = 180; //3 seconds @ 60fps

//*******************************************************************************************************
//*******************************************************************************************************
void CBeatDetective::Init()
{
	m_History.reserve(NUM_BARK_SCALE_BANDS);

	for(int band=0; band<NUM_BARK_SCALE_BANDS; ++band)
	{
		m_History.push_back(std::vector<float>());

		m_History[band].reserve(HISTORY_SIZE);
		for(int i=0; i<HISTORY_SIZE; ++i)
		{
			m_History[band].push_back(0.0f);
		}
	}

	m_CurrHistoryIndex = 0;
}

//*******************************************************************************************************
void CBeatDetective::Update()
{
	int band = 0;
	for(float f=0.0f; f<=1.0f; f+=1.0f/NUM_BARK_SCALE_BANDS)
	{
		float t_val = CRationalVisionsApp::Get()->GetSoundEngine().GetMovement(f);

		RMath::Limit(0.0f, t_val, 1.0f);

		//if(t_val < 0.8f)
		//{
		//	t_val = 0.0f;
		//}

		m_History[band][m_CurrHistoryIndex] = t_val;
		++band;
	}
	//m_History[m_CurrHistoryIndex] = val;

	++m_CurrHistoryIndex;
	if(m_CurrHistoryIndex >= HISTORY_SIZE)
	{
		m_CurrHistoryIndex = 0;
	}
}

//*******************************************************************************************************
float CBeatDetective::GetHistoryItem(int band, int i) 
{ 
	int index = m_CurrHistoryIndex - i;
	if(index < 0)
	{
		index += HISTORY_SIZE;
	}

	return m_History[band][index];
}

//*******************************************************************************************************
float CBeatDetective::GetEntropy(int band)
{
	static int method = 1;

	switch(method)
	{
	case 0:
		{
			float total_contribution = 0.0f;
			int num_contributions = 0;
			for(int i=0; i<HISTORY_SIZE; ++i)
			{
				float t_val = m_History[band][i];
				if(t_val > 0.0f)
				{
					total_contribution += t_val;
					num_contributions++;
				}
			}

			if(num_contributions > 0)
			{
				return total_contribution / (float)num_contributions;
			}
			else
			{
				return 0.0f;
			}
		}
		break;
	case 1:
		{
			int num_fluffy = 0;
			for(int i=0; i<HISTORY_SIZE; ++i)
			{
				float t_val = m_History[band][i];
				if(t_val > 0.0f && t_val < 0.8f)
				{
					num_fluffy++;
				}
			}

			return num_fluffy/(float)HISTORY_SIZE;
		}
		break;
	}
}