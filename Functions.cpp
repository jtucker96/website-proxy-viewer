#include "Definitions.h"

unsigned int m_uiDlSuccess   = 0				;	// total number of times a file has successfully downloaded
unsigned int m_uiViewSuccess = 0				;	// total number of times we have successfully viewed the website


vector <string>     m_vProxy				    ;	// a proxy vector
vector <string>     m_vDownloadLinks			;	// a download link vector
vector <string>     m_vPlaylist					;	// a playlist vector

static const WORD MAX_CONSOLE_LINES = 500;
static const char alphanum[] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

inline void LoopProcedure(unsigned int m_uiMinRange, unsigned int m_uiMaxRange, unsigned char * m_ucInputURL)
{
	CURL *m_hCurl;
    CURLcode m_cRes;


	m_hCurl = curl_easy_init();

	EnterCriticalSection(&m_stCriticalSection1);
	bool m_DownloadFile = GenerateChangeOfFileDownload(m_uiDownloadFrequency);
	unsigned int m_uiGeneratedProxyIndex = GenerateProxyIndex(0);						// generate the index
	string m_szProxyIp = (m_vProxy[m_uiGeneratedProxyIndex]);							// temp proxy ip becomes the index value
	m_vProxy.erase((m_vProxy.begin() + m_uiGeneratedProxyIndex));					    // delete from the index
	unsigned int m_uiViewingTime = GenerateViewingTime(m_uiMinRange,m_uiMaxRange);		// generate viewing time from the values

	LeaveCriticalSection(&m_stCriticalSection1); 

	EnterCriticalSection(&m_stCriticalSection3);
	cout << "INITIATE : \t - attempting to connect to: " << m_szProxyIp << endl;
	LeaveCriticalSection(&m_stCriticalSection3); 


	curl_easy_setopt(m_hCurl, CURLOPT_URL, (char*)m_ucInputURL); 
    curl_easy_setopt(m_hCurl, CURLOPT_PROXY, m_szProxyIp.c_str()); 
    curl_easy_setopt(m_hCurl, CURLOPT_HTTPPROXYTUNNEL, 1); 
    curl_easy_setopt(m_hCurl, CURLOPT_CONNECT_ONLY, 1); 
    curl_easy_setopt(m_hCurl, CURLOPT_TIMEOUT, 3);
    curl_easy_setopt (m_hCurl, CURLOPT_POST, 1);
    curl_easy_setopt(m_hCurl,CURLOPT_USERAGENT,"Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.13) Gecko/20080311 Firefox/2.0.0.13");
    curl_easy_setopt(m_hCurl, CURLOPT_FRESH_CONNECT, true);


	m_cRes = curl_easy_perform(m_hCurl);

	if (m_cRes == CURLE_OK) // if it's OK
	{
	    EnterCriticalSection(&m_stCriticalSection3);
		cout << "----------------------------------------------------------------------------" << endl;
		cout << "SUCCESS : \t - connected to : " << m_szProxyIp << endl;
		cout << "----------------------------------------------------------------------------" << endl;
		LeaveCriticalSection(&m_stCriticalSection3); 

		EnterCriticalSection(&m_stCriticalWebViewSuccess);
		// update our variable for web connection success
		m_uiViewSuccess++;
		LeaveCriticalSection(&m_stCriticalWebViewSuccess); 

		if (InitiatePlaylist(m_szProxyIp))		// initiate the playlist
		{
			if (m_DownloadFile)					// if we should download the file
			{
				DownloadFile(m_szProxyIp);		// download the file
			}
			// now we should decrement the rounds here

		}
		else
		{
			cout << "The playlist failed" << endl;
		}
	}
	else // the proxy failed
	{
		EnterCriticalSection(&m_stCriticalSection3);
		cout << "FAILURE : \t - to connect to : " << m_szProxyIp << endl;
		LeaveCriticalSection(&m_stCriticalSection3);
	}
}

bool InitiatePlaylist(string m_szProxyIp)
{
	/*
	This will initiate a song from the vector
	using a valid proxy IP, you need to send
	cookies to the website for the song to play..
	*/
	string * m_szSongURL					= 0;
	unsigned int m_uiGeneratedPlaylistIndex = 0;

	EnterCriticalSection(&m_stCriticalPlaylist);
	unsigned int m_uiNumSongsToPlay = GenerateNumOfSongsToPlay(1);	 // how many songs to play in playlist?

	m_szSongURL = new string[m_uiNumSongsToPlay];
	for (unsigned int i = 0; i < m_uiNumSongsToPlay; i++)
	{	// get the URL strings of how many we generated
		m_uiGeneratedPlaylistIndex = GeneratePlaylistIndex(0);
		m_szSongURL[i] = (m_vPlaylist[m_uiGeneratedPlaylistIndex]);
	}
	LeaveCriticalSection(&m_stCriticalPlaylist);
	// we now have the array of URL's to songs, now do some other shit
	// (generate cookies, pass cURL ect..)

	for (int i = 0; i < m_uiNumSongsToPlay; i++)
	{
		string m_sURL = ;
		m_sURL.append("?url=");
		m_sURL.append(m_szSongURL[0]);
		m_sURL.append("?ip=");
		m_sURL.append(m_szProxyIp);
	}

	delete[] m_szSongURL;
	return true;
}

inline size_t WriteData(void *m_ptr, size_t m_size, size_t m_nmemb, FILE * m_stream)
{
	size_t m_written;
	m_written = fwrite(m_ptr, m_size, m_nmemb, m_stream);
	return m_written;
}

void DownloadFile(string m_szProxyIp)
{
	EnterCriticalSection(&m_stCriticalSection2);
	unsigned int m_uiDownloadIndex = GenerateDownloadIndex();
	FILE *m_oFp = NULL;										
    CURLcode m_cRes;							// generate a weighted index number
	char m_gchFileName[12] = {NULL};
	for (unsigned int i = 0; i < 11; i++)
	{
		m_gchFileName[i] = alphanum[rand() % 11];
	}
	string m_szDownloadLink = m_vDownloadLinks[m_uiDownloadIndex];
	LeaveCriticalSection(&m_stCriticalSection2);// now we download the file from the specified URL

	EnterCriticalSection(&m_stCriticalSection3);
	cout << m_gchFileName << endl;
	LeaveCriticalSection(&m_stCriticalSection3);

	m_oFp =  fopen((const char *)m_gchFileName,"wb");//fopen((const char *)m_chOutFileName,"wb");

	CURL * m_chCurl = curl_easy_init();
	if (m_chCurl)
	{
		EnterCriticalSection(&m_stCriticalSection2);
		cout << m_szDownloadLink << endl;
		LeaveCriticalSection(&m_stCriticalSection2);

		curl_easy_setopt(m_chCurl, CURLOPT_WRITEFUNCTION, WriteData);
		curl_easy_setopt(m_chCurl, CURLOPT_WRITEDATA, m_oFp);
		curl_easy_setopt(m_chCurl, CURLOPT_HEADER, true);
		curl_easy_setopt(m_chCurl, CURLOPT_URL,(char*)m_szDownloadLink.c_str());
		curl_easy_setopt(m_chCurl, CURLOPT_PROXY, (char*)m_szProxyIp.c_str()); 
		curl_easy_setopt(m_chCurl, CURLOPT_HTTPPROXYTUNNEL, 1); 
		curl_easy_setopt(m_chCurl, CURLOPT_FOLLOWLOCATION, TRUE);
		curl_easy_setopt(m_chCurl, CURLOPT_MAXCONNECTS, 10);

		m_cRes = curl_easy_perform(m_chCurl);

	    fclose(m_oFp);

		if (m_cRes != CURLE_OK)
		{
			DeleteFileA(m_gchFileName);
			EnterCriticalSection(&m_stCriticalSection3);
			cout << "===========================================================================" << endl;
			cout << "DOWNLOAD FAIL : \t proxy : "  << m_szProxyIp <<" could not bypass firewall" << endl;
			cout << "===========================================================================" << endl;
			LeaveCriticalSection(&m_stCriticalSection3);
		}
		else
		{
			DeleteFileA(m_gchFileName);
			EnterCriticalSection(&m_stCriticalSection3);
			cout << "----------------------------------------------------------------------------" << endl;
			cout << "DOWNLOAD SUCCESS : \t proxy : "  << m_szProxyIp << endl;
			cout << "----------------------------------------------------------------------------" << endl;
			LeaveCriticalSection(&m_stCriticalSection3);
			EnterCriticalSection(&m_stCriticalDlSuccess);
			// update our variable for DOWNLOAD success
			m_uiDlSuccess++;
			LeaveCriticalSection(&m_stCriticalDlSuccess); 

		}
    curl_easy_cleanup(m_chCurl);
	}
	// now we should delete the file we just downloaded
}


