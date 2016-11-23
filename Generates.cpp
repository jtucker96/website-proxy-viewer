#include "Definitions.h"
#include "UserAgents.h"

inline unsigned int GenerateDownloadIndex()
{
	// put 50% more weight toward
	// the '0' index
	// right now it will download the first index
	unsigned int m_uiDownloadIndex = GenerateChangeOfFileDownload(7);
	// 70% of the time, it will display number 1
	return 0;
	//return m_uiDownloadIndex;
	//
}

inline unsigned int GenerateNumbSongsToPlayFromList()
{
	// we play between 5-15 per proxy
	const unsigned int m_uiPlayMax = 15;
	const unsigned int m_uiPlayMin = 5;
	unsigned int m_uiGeneratedPlays = ((((unsigned int)rand() % (m_uiPlayMax - m_uiPlayMin + 1)) +m_uiPlayMin)%m_uiPlayMax);
	if (m_uiGeneratedPlays == 0)
	{
		GenerateNumbSongsToPlayFromList();
	}
	else
	{
		return m_uiGeneratedPlays;
	}
}

inline unsigned int GeneratePlaylistIndex(unsigned int m_uiMinimum)
{
	// we don't need to assign a section
	// as we don't explicitly change the playlistlines
	// throughout the program
	unsigned int m_uiPlaylistIndex = ((((unsigned int)rand() % (m_uiPlaylistLines - m_uiMinimum + 1)) + m_uiMinimum)%m_uiPlaylistLines);

	return m_uiPlaylistIndex;
}

inline unsigned int GenerateProxyIndex(unsigned int m_uiMinimum)
{
	// seed a time
	// generate a number from that time
	EnterCriticalSection(&m_stCriticalSection4);
	unsigned int m_uiGeneratedProxyIndex =    ((((unsigned int)rand() % (m_uiProxyLines - m_uiMinimum + 1)) + m_uiMinimum)%m_uiProxyLines);
	m_uiProxyLines--;
	LeaveCriticalSection(&m_stCriticalSection4);

	return m_uiGeneratedProxyIndex;
}

inline unsigned int GenerateViewingTime(unsigned int m_uiMinimum, unsigned int m_uiMaximum)
{
	// no need to seed, we just seeded before
	unsigned int m_uiGeneratedRandTime = ((((unsigned int)rand() % (m_uiMaximum - m_uiMinimum + 1)) + m_uiMinimum)%m_uiMaximum);
	if (m_uiMinimum > m_uiGeneratedRandTime)
	{
			srand((unsigned int)time(NULL));							   // seed if we fail
			GenerateViewingTime(m_uiMinimum, m_uiMaximum); // try again...
	}
	return m_uiGeneratedRandTime;
}

inline unsigned int GenerateNumOfSongsToPlay(unsigned int m_uiMinimum)
{
	unsigned int m_uiSongsToPlay = ((((unsigned int)rand() % (5 - m_uiMinimum + 1)) + m_uiMinimum)%5);
	return m_uiSongsToPlay;
}

inline bool GenerateChangeOfFileDownload(unsigned int m_uiInput)
{	// a change out of 10
	// 0 being no download
	// 10 being constant download
	// generate a true or false value from number in between
	switch(m_uiInput)
	{
	case 0:
		{
			return false;
			break;
		}
	case 10:
		{
			return true;
			break;
		}
	default:
		{
			float f = rand()*1.0f/RAND_MAX;
			float vv= m_uiInput / 10.0f;
			return f < vv;
			break;
		}
	}
	return false;
}
