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
