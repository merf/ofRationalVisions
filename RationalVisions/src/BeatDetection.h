#pragma once
#include <vector>


//-------------------------------------------------------------------------------------------------------
class CBeatChannelHistoryItem
{
	float	m_Value;
	bool	m_IsBeat;
};

//-------------------------------------------------------------------------------------------------------
class CBeatChannel
{
	float									m_Average;
	float									m_Variance;

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
	float	GetAverage() { return m_Average; }
	float	GetVariance();

	float	GetHistoryItem(int age);
	float	GetHistoryItem(int band, int age);
	float	GetEntropy(int band);

	static const int HISTORY_SIZE;
private:
	int		GetIndexForAge(int age);

	//TODO - would be nicer to generalise the history and beats to be an arbitrary number of channels that data is streamed into..

	std::vector<float>				m_History;
	std::vector<std::vector<float>>	m_BandHistory;
	int								m_CurrHistoryIndex;

	std::vector<bool>				m_Beats;
	float							m_Average;
	float							m_Variance;
	float dummy;
};