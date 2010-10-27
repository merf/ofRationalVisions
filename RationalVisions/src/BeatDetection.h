#pragma once
#include <vector>

//-------------------------------------------------------------------------------------------------------
class CBeatDetective
{
public:
	CBeatDetective() { Init(); }

	void Init();
	void Update();

	float GetHistoryItem(int band, int i);

	static const int HISTORY_SIZE;
private:

	std::vector<std::vector<float>>	m_History;
	int								m_CurrHistoryIndex;
};