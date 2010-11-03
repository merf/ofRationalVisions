#include "BeatDetection.h"
#include "SoundEngine.h"
#include "RationalVisionsApp.h"
#include "RMath.h"

const int CBeatDetective::HISTORY_SIZE = 180; //3 seconds @ 60fps

//*******************************************************************************************************
//*******************************************************************************************************
void CBeatDetective::Init()
{
	m_BandHistory.reserve(NUM_BARK_SCALE_BANDS);

	for(int band=0; band<NUM_BARK_SCALE_BANDS; ++band)
	{
		m_BandHistory.push_back(std::vector<float>());

		m_BandHistory[band].reserve(HISTORY_SIZE);
		for(int i=0; i<HISTORY_SIZE; ++i)
		{
			m_BandHistory[band].push_back(0.0f);
		}
	}

	m_History.reserve(HISTORY_SIZE);
	m_Beats.reserve(HISTORY_SIZE);

	for(int i=0; i<HISTORY_SIZE; ++i)
	{
		m_History.push_back(0.0f);
		m_Beats.push_back(false);
	}

	m_CurrHistoryIndex = 0;

	m_Average = 0.0f;
}

//*******************************************************************************************************
void CBeatDetective::Update()
{
	StoreIncomingAudio();

	FindBeats();

	++m_CurrHistoryIndex;
	if(m_CurrHistoryIndex >= HISTORY_SIZE)
	{
		m_CurrHistoryIndex = 0;
	}
}

//*******************************************************************************************************
void CBeatDetective::StoreIncomingAudio()
{
	int band = 0;
	float total = 0.0f;
	for(float f=0.0f; f<=1.0f; f+=1.0f/NUM_BARK_SCALE_BANDS)
	{
		float t_val = CRationalVisionsApp::Get()->GetSoundEngine().GetMovement(f);

		RMath::Limit(0.0f, t_val, 1.0f);

		m_BandHistory[band][m_CurrHistoryIndex] = t_val;
		total += t_val;
		++band;
	}

	m_History[m_CurrHistoryIndex] = total / NUM_BARK_SCALE_BANDS;
}

//*******************************************************************************************************
void CBeatDetective::FindBeats()
{
	int oldest_item = m_CurrHistoryIndex + 1;
	if(oldest_item >= HISTORY_SIZE)
	{
		oldest_item = 0;
	}

	const float one_on_history_size = 1.0f / HISTORY_SIZE;
	m_Average -= m_History[oldest_item] * one_on_history_size;
	m_Average += m_History[m_CurrHistoryIndex] * one_on_history_size;

	m_Variance = 0.0f;
	for(int i=0; i<HISTORY_SIZE; ++i)
	{
		m_Variance += pow((m_History[i] - m_Average), 2);
	}
	m_Variance /= HISTORY_SIZE;
	m_Variance = pow(m_Variance, 0.5f);

	if(m_History[m_CurrHistoryIndex] > m_Average + m_Variance)
	{
		m_Beats[m_CurrHistoryIndex] = true;
	}
	else
	{
		m_Beats[m_CurrHistoryIndex] = false;
	}
}

//*******************************************************************************************************
float CBeatDetective::GetVariance()
{
	return m_Variance;
}

//*******************************************************************************************************
float CBeatDetective::GetPhase()
{
	int prev_beat_pos = -1;
	std::vector<int> beat_gaps;

	std::map<int, int> beat_gap_counts;

	//ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***
	//ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***
	//ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***
	//This does not deal correctly with the fact the beats are in a circular buffer
	//Need to start from first item...
	for(int i=0; i<HISTORY_SIZE; ++i)
	{
		if(m_Beats[i])
		{
			if(prev_beat_pos != -1)
			{
				int gap = i-prev_beat_pos;
				if(gap > 1)
				{
					if(beat_gap_counts.find(gap) == beat_gap_counts.end())
					{
						beat_gap_counts[gap] = 0;
					}
					beat_gap_counts[gap]++;
					beat_gaps.push_back(i-prev_beat_pos);
				}
			}
			prev_beat_pos = i;
		}
	}

	if(beat_gaps.empty())
	{
		return 0.0f;
	}
	else
	{
		float avg = 0.0f;
		for(std::vector<int>::iterator it = beat_gaps.begin(); it != beat_gaps.end(); ++it)
		{
			avg += *it;
		}

		return avg / (float)beat_gaps.size();
	}
}

//*******************************************************************************************************
int CBeatDetective::GetIndexForAge(int age)
{
	int index = m_CurrHistoryIndex - age;
	if(index < 0)
	{
		index += HISTORY_SIZE;
	}
	return index;
}

//*******************************************************************************************************
float CBeatDetective::GetHistoryItem(int band, int age) 
{ 
	return m_BandHistory[band][GetIndexForAge(age)];
}

//*******************************************************************************************************
float CBeatDetective::GetHistoryItem(int age) 
{ 
	return m_History[GetIndexForAge(age)];
}

//*******************************************************************************************************
bool CBeatDetective::IsBeat(int age) 
{
	return m_Beats[GetIndexForAge(age)];
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
				float t_val = m_BandHistory[band][i];
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
				float t_val = m_BandHistory[band][i];
				if(t_val > 0.0f && t_val < 0.8f)
				{
					num_fluffy++;
				}
			}

			return num_fluffy/(float)HISTORY_SIZE;
		}
		break;
	}

	return 0.0f;
}