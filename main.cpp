/*
	Application created by d4rkj4y, please, if
	you modify source code, be sure to give credits, or I will 
	find you and beat you, okay?
	Three inputs : proxies, playlist URLS, and download URLS,
	please input these as DOT TEXT files (*.txt)
	Be careful with thread synchronization, if you tamper with
	the placement of functions you may find yourself in a bad situation..
	Uses CRT, and basic windows functions,  don't link against it...
*/		

/*

The soundclick system works in a way in which it only detects unique song plays,
it also seems to disregard plays that arrive too soon after each other so some sort of time delay may be required.
It is a flash mp3 player, and downloads from links are also tracked, and used in the chart ranking.

Top move up the charts:
- Increase plays
- Increase downloads
- Increase page views

*/

/*
-automatically fetch the proxies
-change the user agent
-change the referrer
-change the proxy then visit your site after that it will wait randomly then click on a random link then again random wait
-some scroll and close the website. Clear cookies and same loop again. Every hit will download the favicon.ico
-Maybe not..
*/

#include "Definitions.h"


	string					m_szScriptURL				;	// url to uploaded php file

	CRITICAL_SECTION	 	m_stCriticalSection1	    ;	// for accessing global proxy vector, generating the index, erasing a value, generating viewing time
	CRITICAL_SECTION		m_stCriticalSection2        ;	// for generating download index
	CRITICAL_SECTION		m_stCriticalSection3        ;	// for accessing the console output (through cout ect..)
	CRITICAL_SECTION		m_stCriticalSection4        ;	// altering proxy lines global
	CRITICAL_SECTION		m_stCriticalDlSuccess       ;	// altering download success global
	CRITICAL_SECTION        m_stCriticalWebViewSuccess	;	// altering web view success global
	CRITICAL_SECTION        m_stCriticalPlaylist		;	//

	bool					m_bEscFalse	  =		true    ;


	unsigned int     m_uiMaxRange      ;
	unsigned int     m_uiMinRange      ;
	unsigned char    m_ucInputURL[128] ;

	int b = 0;

int main()
{
	srand((unsigned int)time(NULL));
	if (curl_global_init(CURL_GLOBAL_ALL))
	{
		cout << "Error 124 - failed to initialize cURL\nProgram terminating" << endl;
		cin.get();
		return 0;
	}

	HANDLE			 m_hThread1	 = NULL ;
	HANDLE			 m_hThread2	 = NULL ;
	HANDLE			 m_hThread3	 = NULL ;
	HANDLE			 m_hThread4	 = NULL ;
	HANDLE			 m_hThread5	 = NULL ;
	HANDLE			 m_hThread6	 = NULL ;
	HANDLE			 m_hThread7	 = NULL ;
	HANDLE			 m_hThread8	 = NULL ;
	HANDLE			 m_hThread9	 = NULL ;
	HANDLE			 m_hThread10 = NULL ;

	unsigned int     m_uiThread1	    ;
	unsigned int     m_uiThread2	    ;
	unsigned int     m_uiThread3	    ;
	unsigned int     m_uiThread4	    ;
	unsigned int     m_uiThread5	    ;
	unsigned int     m_uiThread6	    ;
	unsigned int     m_uiThread7	    ;
	unsigned int     m_uiThread8	    ;
	unsigned int     m_uiThread9	    ;
	unsigned int     m_uiThread10	    ;


	if(!ObtainProxyList())								 // obtain the proxy list array
		return 0;
	if(!ObtainDownloadList())
		return 0;
	if(!ObtainPlaylist())
		return 0;

	if(!ObtainUserArguments(&m_uiMinRange,&m_uiMaxRange,m_ucInputURL))	 // obtain the user settings
		return 0; // our program failed
	// we have all the parameters, now begin the loop

	if (m_uiRounds > m_uiProxyLines) // if it is greater than count lines
		m_uiRounds = m_uiProxyLines;


	InitializeCriticalSection(&m_stCriticalSection1);
	InitializeCriticalSection(&m_stCriticalSection2);
	InitializeCriticalSection(&m_stCriticalSection3);
	InitializeCriticalSection(&m_stCriticalSection4);
	InitializeCriticalSection(&m_stCriticalDlSuccess);
	InitializeCriticalSection(&m_stCriticalWebViewSuccess); 
	InitializeCriticalSection(&m_stCriticalPlaylist);

	// initialize our threads

	m_hThread1 = (HANDLE)_beginthreadex(0,0,&m_tThread,0,CREATE_SUSPENDED,&m_uiThread1);
	m_hThread2 = (HANDLE)_beginthreadex(0,0,&m_tThread,0,CREATE_SUSPENDED,&m_uiThread2);
	m_hThread3 = (HANDLE)_beginthreadex(0,0,&m_tThread,0,CREATE_SUSPENDED,&m_uiThread3);
	m_hThread4 = (HANDLE)_beginthreadex(0,0,&m_tThread,0,CREATE_SUSPENDED,&m_uiThread4);
	m_hThread5 = (HANDLE)_beginthreadex(0,0,&m_tThread,0,CREATE_SUSPENDED,&m_uiThread5);
	m_hThread6 = (HANDLE)_beginthreadex(0,0,&m_tThread,0,CREATE_SUSPENDED,&m_uiThread6);
	m_hThread7 = (HANDLE)_beginthreadex(0,0,&m_tThread,0,CREATE_SUSPENDED,&m_uiThread7);
	m_hThread8 = (HANDLE)_beginthreadex(0,0,&m_tThread,0,CREATE_SUSPENDED,&m_uiThread8);
	m_hThread9 = (HANDLE)_beginthreadex(0,0,&m_tThread,0,CREATE_SUSPENDED,&m_uiThread9);
	m_hThread10 =(HANDLE)_beginthreadex(0,0,&m_tThread,0,CREATE_SUSPENDED,&m_uiThread10);

	ResumeThread(m_hThread1) ;	// should have access to shit first
	ResumeThread(m_hThread2) ;
	ResumeThread(m_hThread3) ;
	ResumeThread(m_hThread4) ;
	ResumeThread(m_hThread5) ;
	ResumeThread(m_hThread6) ;
	ResumeThread(m_hThread7) ;
	ResumeThread(m_hThread8) ;
	ResumeThread(m_hThread9) ;
	ResumeThread(m_hThread10);

	bool m_bExit = false;

    while(m_bExit == false)
    {
        if (GetAsyncKeyState(VK_ESCAPE))
        {
            m_bExit = true;
        }
		Sleep(1000);
    }
	cout << "Exiting program now" << endl;
	cout << "Writing non-used proxies to example.txt" << endl;
	std::ofstream output_file("example.txt");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(m_vProxy.begin(), m_vProxy.end(), output_iterator);
	output_file.close();
	/*
	We kill all our running threads here
	(sorry threads)
	*/

	cout << "Successful downloads : " << m_uiDlSuccess << endl;
	cout << "Successful webpage views : " << m_uiViewSuccess << endl;
	cout << "Enter any key to terminate program" << endl;
	// this is undesirable way to end the threads
	// but it will do for now..

	SuspendThread(m_hThread1);
	SuspendThread(m_hThread2);
	SuspendThread(m_hThread3);
	SuspendThread(m_hThread4);
	SuspendThread(m_hThread5);
	SuspendThread(m_hThread6);
	SuspendThread(m_hThread7);
	SuspendThread(m_hThread8);
	SuspendThread(m_hThread9);
	SuspendThread(m_hThread10);

	// clean that shit up nigga
	DeleteCriticalSection(&m_stCriticalSection1);
	DeleteCriticalSection(&m_stCriticalSection2);
	DeleteCriticalSection(&m_stCriticalSection3);
	DeleteCriticalSection(&m_stCriticalSection4);
	DeleteCriticalSection(&m_stCriticalWebViewSuccess);
	DeleteCriticalSection(&m_stCriticalDlSuccess);
	DeleteCriticalSection(&m_stCriticalPlaylist);

	cin.ignore();
	cin.get();

	return 0;
}

// thread function

static unsigned __stdcall m_tThread(LPVOID m_lpParam)
{
	while(m_bEscFalse)
	{
		const unsigned int m_constProxyLines = m_uiProxyLines; // we must initially set a constant value, as 
			switch (m_uiRounds)									   // proxy lines will continuously change
			{													   // throughout the 
			case 0: // loop until finish (until all the proxies are read)
			{
				for (unsigned int i = 0; i < m_constProxyLines; i++)
				{
					LoopProcedure(m_uiMinRange,m_uiMaxRange,m_ucInputURL);
				}
			break;
			}
			default:
			{	// loop until user defined rounds is over
				for (unsigned int i = m_uiRounds; i > 0 ;) //  i--
														  // we only decrement on success
				{
					LoopProcedure(m_uiMinRange,m_uiMaxRange,m_ucInputURL);
					cout << i << endl;

				}
				break;
			}
		}
	}
	return 0; // probably never reaches lul
}

