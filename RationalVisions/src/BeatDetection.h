#pragma once
#include <vector>


//-------------------------------------------------------------------------------------------------------
class CBeatChannelHistoryItem
{
public:
	CBeatChannelHistoryItem(float val, bool is_beat) : m_Value(val), m_IsBeat(is_beat) {}

	float	GetValue() { return m_Value; }
	void	SetValue(float val) { m_Value = val; }

	bool	IsBeat() { return m_IsBeat; }
	void	SetIsBeat(bool is_beat) { m_IsBeat = is_beat; }

private:
	float	m_Value;
	bool	m_IsBeat;
};

//-------------------------------------------------------------------------------------------------------
class CBeatChannel
{
public:
	CBeatChannel(int history_size, float max_band);

	float									GetMaxBand() const { return m_MaxBandToInclude; }

	float									GetHistoryItem(int index) { return m_History[index].GetValue(); }
	void									SetHistoryItem(int index, float val) { m_History[index].SetValue(val); }

	void									FindBeats();
	void									CalcBPM();

	bool									IsBeat(int age);

	float									GetAverage() const { return m_Average; }
	float									GetVariance() const { return m_Variance; }

	float									GetBPM();
	float									GetBPMConfidence() const { return m_Confidence; }
private:
	int										m_StepsSinceLastBeat;

	int										m_HistorySize;

	float									m_MaxBandToInclude;

	float									m_Average;
	float									m_Variance;

	float									m_PreviousBPM;
	float									m_Confidence;

	std::vector<CBeatChannelHistoryItem>	m_History;
};

//-------------------------------------------------------------------------------------------------------
class CBeatDetective
{
public:
	CBeatDetective() { Init(); }

	void	Init();
	void	Update();

	void	StoreIncomingAudio();
	void	FindBeats();

	bool	IsBeat(int age);
	bool	IsBeat(int age, int channel);
	float	GetAverage(int channel) { return m_Channels[channel].GetAverage(); }
	float	GetVariance(int channel) { return m_Channels[channel].GetVariance(); }

	float	GetHistoryItem(int age);
	float	GetChannelHistoryItem(int channel, int age);
	float	GetBandHistoryItem(int band, int age);

	float	GetChannelMaxBand(int channel) { return m_Channels[channel].GetMaxBand(); }

	float	GetBPM(int channel) { return m_Channels[channel].GetBPM(); }
	float	GetBPMConfidence(int channel) { return m_Channels[channel].GetBPMConfidence(); }

	static const int HISTORY_SIZE;
	static const int NUM_CHANNELS;
	static int		GetIndexForAge(int age);

private:
	typedef std::vector<CBeatChannel>	TBeatChannelList;	
	TBeatChannelList					m_Channels;

	std::vector<std::vector<float> >	m_BandHistory;
	static int						m_CurrHistoryIndex;

	friend class CBeatChannel;
};