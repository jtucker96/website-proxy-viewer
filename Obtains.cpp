#include "Definitions.h"

unsigned int m_uiRounds							;	// how many rounds they want to do
unsigned int m_uiDownloadFrequency				;	// generated number of download frequency
unsigned int m_uiProxyLines						;	// total lines in proxy file
unsigned int m_uiDownloadLines					;	// total lines in download file
unsigned int m_uiPlaylistLines					;	// total lines in playlist file


bool ObtainUserArguments(unsigned int * m_uiMinRange, unsigned int * m_uiMaxRange, unsigned char * m_ucInputURL)
{
	cout << "Complete URL to website, eg http://www.google.com/index" << endl;
	cin >> m_ucInputURL;
	cout << "Minimum number of seconds to wait: (recommended 50)" << endl;
	cin >> *m_uiMinRange;
	cout << "Maximum number of seconds to wait: (recommended 120)" << endl;
	cin >> *m_uiMaxRange;
	if  (*m_uiMinRange > *m_uiMaxRange)
	{
		cout << "The minimum range is greater than the maximum range\nPress any key" << endl;
		cin.get();
		return false;
	}
	if (*m_uiMinRange == *m_uiMaxRange)
	{
		cout << "Maximum range is equal to minimum range\nPress any key" << endl;
		cin.get();
		return false;
	}
	cout << "How many times would you like to view the website in total?\n(0 = until proxies run out)" << endl;
	cin >> m_uiRounds; 
	cout << "Frequency of download occurance : how often would you like the viewer to download a file? \nFrom 0 being never to 10 being always" << endl;
	cin >> m_uiDownloadFrequency;
	if (m_uiDownloadFrequency > 10 || m_uiDownloadFrequency < 0)
	{
		cout << "Download frequency is not between 0-10, or is not a valid number\nPress any key" << endl;
		cin.get();
		return false;
	}
	return true;
}

bool ObtainProxyList()
{	// load the list of proxies
	// 
	char    m_szProxyListPath[48]   = { NULL } ;
	cout << "#######################################################################\n#\t\t\t\t\t\t\t\t      #" << endl;
	cout << "#   Please enter the location to the list of proxies 'as a .txt file' # \n#   Ensure that the file is in the same directory\t\t      # \n#   Don't attempt to access the proxy .txt file while it is in use    # \n#   Press ESC key on the program to send an exit message\t      #" << endl;
	cout << "#\t\t\t\t\t\t\t\t      #\n#######################################################################" << endl;

	cin.getline(m_szProxyListPath,48);
	if (m_szProxyListPath)
	{
		ifstream m_iProxyList;
		m_iProxyList.open(m_szProxyListPath,ios::beg);
		if (m_iProxyList)
		{	// if we open the proxy list
			// get how many lines 
			string m_szTemp;
			while (getline(m_iProxyList,m_szTemp))
			{
				m_vProxy.push_back(m_szTemp);
				m_uiProxyLines++;
			}
			m_iProxyList.close();
			cout << "\tArguments gathered.." << endl;
			return true;
		}
		else
		{
			cout << "Error 121 - file not found\nMake sure the file is in the current directory\nPress any key..." << endl;
			cin.get();
			return false;
		}
	}
	else
	{
		cout << "Error 120" << endl;
		cin.get();
	}
	// all false control paths end here
	m_vProxy.push_back("NULL");
	return false;
}

bool ObtainDownloadList()
{	 
	char    m_szDownloadPath[48]   = { NULL } ;
	ifstream m_iDownloadList;
	cout << "Please enter the file name to the links to download" << endl;
	cin.clear();
	cin.getline(m_szDownloadPath,48);
	m_iDownloadList.open(m_szDownloadPath);
	if (m_iDownloadList) // if it's true
	{
			string m_szTemp;
			while (getline(m_iDownloadList,m_szTemp)) // while it's not eof
			{										  // get all the data
				m_vDownloadLinks.push_back(m_szTemp);
				m_uiDownloadLines++;
			}
			m_iDownloadList.close();				  // bit of cleanup
			cout << "\tDownload links successfully gathered.." << endl;
			return true;				  // return the vector
	}
	else // if the file couldn't be found / opened
	{
		cout << "Error 121 - file not found\nMake sure the file is in the current directory\nPress any key" << endl;
		cin.get();
	}

	m_iDownloadList.close();
	return false;
}

bool ObtainPlaylist()			// contains our playlist
{
	char    m_szPlaylistPath[48]   = { NULL } ;
	ifstream m_iPlaylistList;
	cout << "Enter the filename to the links of the playlist" << endl;
	cin.clear();
	cin.getline(m_szPlaylistPath,48);
	m_iPlaylistList.open(m_szPlaylistPath);
	if (m_iPlaylistList) // if it's true
	{
			string m_szTemp;
			while (getline(m_iPlaylistList,m_szTemp)) // while it's not eof
			{										  // get all the data
				m_vDownloadLinks.push_back(m_szTemp);
				m_uiPlaylistLines++;
			}
			m_iPlaylistList.close();				  // bit of cleanup
			cout << "Playlist links successfully gathered.." << endl;
			return true;				  // return the vector
	}
	else // if the file couldn't be found / opened
	{
		cout << "Error 121 - file not found\nMake sure the file is in the current directory\nPress any key" << endl;
		cin.get();
	}

	m_iPlaylistList.close();
	return false;
}
