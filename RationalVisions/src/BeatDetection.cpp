#include "BeatDetection.h"
#include "SoundEngine.h"
#include "RationalVisionsApp.h"
#include "RMath.h"

const int	CBeatDetective::HISTORY_SIZE = 180; //3 seconds @ 60fps
const int	CBeatDetective::NUM_CHANNELS = 4;
int			CBeatDetective::m_CurrHistoryIndex = 0;

const float CHANNEL_BOUNDARIES[CBeatDetective::NUM_CHANNELS] = { 0.25f, 0.5f, 0.75f, 1.0f };

//*******************************************************************************************************
//*******************************************************************************************************
CBeatChannel::CBeatChannel(int history_size, float max_band) 
: m_PreviousWasBeat(false), m_HistorySize(history_size), m_MaxBandToInclude(max_band), m_Average(0.0f), m_Variance(0.0f), m_PreviousBPM(120.0f)
{
	m_History.reserve(history_size);
	for(int i=0; i<history_size; ++i)
	{
		m_History.push_back(CBeatChannelHistoryItem(0.0f, false));
	}
}

//*******************************************************************************************************
bool CBeatChannel::IsBeat(int age)
{
	int index = CBeatDetective::GetIndexForAge(age);
	return m_History[index].IsBeat();
}

//*******************************************************************************************************
void CBeatChannel::FindBeats()
{
	int oldest_item = CBeatDetective::m_CurrHistoryIndex + 1;
	if(oldest_item >= m_HistorySize)
	{
		oldest_item = 0;
	}

	static bool use_variance = true;
	if(use_variance)
	{
		float one_on_history_size = 1.0f / m_HistorySize;
		m_Average -= m_History[oldest_item].GetValue() * one_on_history_size;
		m_Average += m_History[CBeatDetective::m_CurrHistoryIndex].GetValue() * one_on_history_size;

		m_Variance = 0.0f;
		for(int i=0; i<m_HistorySize; ++i)
		{
			m_Variance += pow((m_History[i].GetValue() - m_Average), 2);
		}
		m_Variance /= m_HistorySize;
		m_Variance = pow(m_Variance, 0.5f);

		//TODO - this should be dynamic based on how prominent the beat is...
		static float variance_mul = 1.5f;

		if(m_History[CBeatDetective::m_CurrHistoryIndex].GetValue() > m_Average + m_Variance * variance_mul)
		{
			if(!m_PreviousWasBeat)
			{
				m_History[CBeatDetective::m_CurrHistoryIndex].SetIsBeat(true);
				m_PreviousWasBeat = true;
			}
			else
			{
				m_History[CBeatDetective::m_CurrHistoryIndex].SetIsBeat(false);
			}
		}
		else
		{
			m_History[CBeatDetective::m_CurrHistoryIndex].SetIsBeat(false);
			m_PreviousWasBeat = false;
		}
	}
	else
	{
		float max = 0.0f;
		for(int i=0; i<m_HistorySize; ++i)
		{
			float t_val = m_History[i].GetValue();
			if(t_val > max)
			{
				max = t_val;
			}
		}

		if(m_History[CBeatDetective::m_CurrHistoryIndex].GetValue() > max * 0.5f)
		{
			m_History[CBeatDetective::m_CurrHistoryIndex].SetIsBeat(true);
		}
		else
		{
			m_History[CBeatDetective::m_CurrHistoryIndex].SetIsBeat(false);
		}
	}

	CalcBPM();
}

//*******************************************************************************************************
void CBeatChannel::CalcBPM()
{
	int prev_beat_pos = -1;

	std::map<int, int> beat_gap_counts;

	//ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***
	//ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***
	//ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***ERROR***
	//This does not deal correctly with the fact the beats are in a circular buffer
	//Need to start from first item...
	for(int i=0; i<m_HistorySize; ++i)
	{
		int index = CBeatDetective::m_CurrHistoryIndex + i;
		if(index >= m_HistorySize)
		{
			index -= m_HistorySize;
		}

		if(m_History[index].IsBeat())
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
				}
			}
			prev_beat_pos = i;
		}
	}

	if(beat_gap_counts.empty())
	{
		m_PreviousBPM = 120.0f;
	}
	else
	{
		float previous_phase = 60.0f * 60.0f / m_PreviousBPM;

		int closest_to_average = 0;
		float best_dist_to_average = 100000.0f;
		for(std::map<int, int>::iterator it = beat_gap_counts.begin(); it != beat_gap_counts.end(); ++it)
		{
			float dist = fabsf(previous_phase - it->first);
			if(dist < best_dist_to_average)
			{
				best_dist_to_average = dist;
				closest_to_average = it->first;
			}
		}

		std::vector<float> beat_gaps;

		for(std::map<int, int>::iterator it = beat_gap_counts.begin(); it != beat_gap_counts.end(); ++it)
		{
			float new_val = it->first;
			while(new_val < closest_to_average * 0.75f)
			{
				new_val *= 2.0f;
			}
			while(new_val > closest_to_average * 1.5f)
			{
				new_val /= 2.0f;
			}

			for(int i=0; i<it->second; i++)
			{
				beat_gaps.push_back(new_val);
			}
		}


		float total = 0.0f;

		for(std::vector<float>::iterator it = beat_gaps.begin(); it != beat_gaps.end(); ++it)
		{
			total += *it;
		}

		float phase = total / (float)beat_gaps.size();
		float bpm = 60 * 60 / phase;

		if(bpm < 80.0f)
		{
			bpm *= 2.0f;
		}
		else if(bpm > 160.0f)
		{
			bpm /= 2.0f;
		}

		m_PreviousBPM = RMath::Lerp(m_PreviousBPM, bpm, 0.01f);
	}
}

float CBeatChannel::GetBPM()
{
	return m_PreviousBPM;
}

//*******************************************************************************************************
//*******************************************************************************************************
void CBeatDetective::Init()
{
	m_Channels.reserve(NUM_CHANNELS);
	for(int i=0; i<NUM_CHANNELS; ++i)
	{
		m_Channels.push_back(CBeatChannel(HISTORY_SIZE, CHANNEL_BOUNDARIES[i]));
	}

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

	m_CurrHistoryIndex = 0;
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
	TBeatChannelList::iterator channel_it = m_Channels.begin();

	int band = 0;
	float total = 0.0f;

	float channel_total = 0.0f;
	int channel_count = 0;

	for(float f=0.0f; f<=1.0f; f+=1.0f/NUM_BARK_SCALE_BANDS)
	{
		float t_val = CRationalVisionsApp::Get()->GetSoundEngine().GetMovement(f);

		RMath::Limit(0.0f, t_val, 1.0f);

		m_BandHistory[band][m_CurrHistoryIndex] = t_val;
		total += t_val;
		++band;

		channel_total += t_val;
		channel_count++;

		if(f > channel_it->GetMaxBand())
		{
			if(channel_it != m_Channels.end())
			{
				channel_it->SetHistoryItem(m_CurrHistoryIndex, channel_total/(float)channel_count);
				++channel_it;
			}
		}
	}

	if(channel_it != m_Channels.end())
	{
		channel_it->SetHistoryItem(m_CurrHistoryIndex, channel_total/(float)channel_count);
		++channel_it;
	}
}

//*******************************************************************************************************
void CBeatDetective::FindBeats()
{
	for(TBeatChannelList::iterator it = m_Channels.begin(); it != m_Channels.end(); ++it)
	{
		it->FindBeats();
	}

	//int oldest_item = m_CurrHistoryIndex + 1;
	//if(oldest_item >= HISTORY_SIZE)
	//{
	//	oldest_item = 0;
	//}

	//const float one_on_history_size = 1.0f / HISTORY_SIZE;
	//m_Average -= m_History[oldest_item] * one_on_history_size;
	//m_Average += m_History[m_CurrHistoryIndex] * one_on_history_size;

	//m_Variance = 0.0f;
	//for(int i=0; i<HISTORY_SIZE; ++i)
	//{
	//	m_Variance += pow((m_History[i] - m_Average), 2);
	//}
	//m_Variance /= HISTORY_SIZE;
	//m_Variance = pow(m_Variance, 0.5f);

	//if(m_History[m_CurrHistoryIndex] > m_Average + m_Variance)
	//{
	//	m_Beats[m_CurrHistoryIndex] = true;
	//}
	//else
	//{
	//	m_Beats[m_CurrHistoryIndex] = false;
	//}
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
float CBeatDetective::GetBandHistoryItem(int band, int age) 
{ 
	return m_BandHistory[band][GetIndexForAge(age)];
}

//*******************************************************************************************************
float CBeatDetective::GetChannelHistoryItem(int channel, int age) 
{ 
	return m_Channels[channel].GetHistoryItem(GetIndexForAge(age));
}

//*******************************************************************************************************
bool CBeatDetective::IsBeat(int age, int channel) 
{
	return m_Channels[channel].IsBeat(age);
}
