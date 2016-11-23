#include <iostream>
#include <fstream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <curl.h>
#include <ctime>
#include <iomanip>
#include <process.h>
#include <CONIO.H>
#include <fcntl.h>
#include <io.h>

//#include <math.h>

using namespace std;

extern unsigned int			m_uiDlSuccess				;
extern unsigned int			m_uiViewSuccess				;
extern unsigned int			m_uiRounds					;	// our global variables
extern unsigned int			m_uiDownloadFrequency		;	// which will be used across the source files
extern unsigned int			m_uiProxyLines				;	// how many lines in the proxy file
extern unsigned int		    m_uiDownloadLines			;	// how many lines in the download file
extern unsigned int			m_uiPlaylistLines			;

extern	vector <string>     m_vProxy				    ;
extern	vector <string>     m_vDownloadLinks			;
extern 	vector <string>     m_vPlaylist					;

extern	string				m_szScriptURL				;

extern CRITICAL_SECTION		m_stCriticalSection1		;	
extern CRITICAL_SECTION		m_stCriticalSection2		;
extern CRITICAL_SECTION		m_stCriticalSection3		;	// output critical section
extern CRITICAL_SECTION		m_stCriticalSection4		;	// special
extern CRITICAL_SECTION     m_stCriticalDlSuccess		;	// updating DOWNLOAD success
extern CRITICAL_SECTION     m_stCriticalWebViewSuccess	;	// updating WEBSITE VIEW success
extern CRITICAL_SECTION		m_stCriticalPlaylist		;	// manipulating global playlist vector

extern bool					m_bEscFalse					;

extern inline void LoopProcedure(unsigned int, unsigned int, unsigned char *);
											// our two vectors
bool ObtainProxyList();			// contains the proxy list
bool ObtainDownloadList();		// contains the download list
bool ObtainPlaylist();			// contains our playlist

// runtime generation
extern inline unsigned int   GenerateProxyIndex(unsigned int);
extern inline unsigned int   GenerateViewingTime(unsigned int, unsigned int);
extern inline unsigned int   GeneratePlaylistIndex(unsigned int);
extern inline unsigned int   GenerateNumOfSongsToPlay(unsigned int);

inline bool GenerateChangeOfFileDownload(unsigned int);
inline size_t WriteData(void *, size_t, size_t, FILE*);

extern unsigned int   GenerateDownloadIndex();
extern void			  DownloadFile(string);
extern bool			  ObtainUserArguments(unsigned int *, unsigned int *, unsigned char *);
extern bool			  InitiatePlaylist(string);

//threads

static unsigned __stdcall m_tThread(LPVOID m_lpParam);
