#ifndef __WAVEFILE_QURAM_H__
#define __WAVEFILE_QURAM_H__

#pragma once

#include <Windows.h>

#ifndef WAVE_FORMAT_PCM

/* OLD general waveform format structure (information common to all formats) */
typedef struct waveformat_tag {
	WORD    wFormatTag;        /* format type */
	WORD    nChannels;         /* number of channels (i.e. mono, stereo, etc.) */
	DWORD   nSamplesPerSec;    /* sample rate */
	DWORD   nAvgBytesPerSec;   /* for buffer estimation */
	WORD    nBlockAlign;       /* block size of data */
} WAVEFORMAT, *PWAVEFORMAT, NEAR *NPWAVEFORMAT, FAR *LPWAVEFORMAT;

/* flags for wFormatTag field of WAVEFORMAT */
#define WAVE_FORMAT_PCM     1


/* specific waveform format structure for PCM data */
typedef struct pcmwaveformat_tag {
	WAVEFORMAT  wf;
	WORD        wBitsPerSample;
} PCMWAVEFORMAT, *PPCMWAVEFORMAT, NEAR *NPPCMWAVEFORMAT, FAR *LPPCMWAVEFORMAT;
#endif /* WAVE_FORMAT_PCM */

class CWaveFile
{
public:
	CWaveFile(void) {
		m_pFile = NULL;
	}
	virtual ~CWaveFile(void) {
		if (m_pFile != NULL) {
			Exit();
		}
	}

public:
	BOOL Init(const TCHAR * pszFilePath, int nSamplesPerSec, int nChannels = 2, WORD wBitsPerSample = 16)
	{
		m_wfHeader.dwRiffFlag = 'FFIR';
		m_wfHeader.dwFileSize = sizeof(WAVE_FILE_HEADER) - 12;

		m_wfHeader.dwWaveFlag = 'EVAW';
		m_wfHeader.dwFmtFlag = ' tmf';
		m_wfHeader.dwFmtSize = sizeof(PCMWAVEFORMAT);

		m_wfHeader.pcmFormat.wBitsPerSample = wBitsPerSample;
		m_wfHeader.pcmFormat.wf.wFormatTag = WAVE_FORMAT_PCM;
		m_wfHeader.pcmFormat.wf.nChannels = nChannels;
		m_wfHeader.pcmFormat.wf.nSamplesPerSec = nSamplesPerSec;
		m_wfHeader.pcmFormat.wf.nBlockAlign = m_wfHeader.pcmFormat.wBitsPerSample * nChannels / 8;
		m_wfHeader.pcmFormat.wf.nAvgBytesPerSec = m_wfHeader.pcmFormat.wf.nBlockAlign * nSamplesPerSec;

		m_wfHeader.dwDataFlag = 'atad';
		m_wfHeader.dwDataSize = 0;

		// TEST CODE //
		//m_wfHeader.dwDataSize = 100000;
		//m_wfHeader.dwFileSize = m_wfHeader.dwDataSize + sizeof(WAVE_FILE_HEADER)-12;
		// TEST CODE //

		//_tfopen_s(&m_pFile, pszFilePath, _T("wb"));
		//if (NULL == m_pFile)
		//	return FALSE;

		//if (fwrite(&m_wfHeader, sizeof(WAVE_FILE_HEADER), 1, m_pFile) != 1) {
		//	fclose(m_pFile);
		//	return FALSE;
		//}
		return TRUE;
	}
	void Exit()
	{
		//if (m_pFile) {
		//	m_wfHeader.dwFileSize = m_wfHeader.dwDataSize + sizeof(WAVE_FILE_HEADER) - 12;

		//	fseek(m_pFile, 0, SEEK_SET);
		//	fwrite(&m_wfHeader, sizeof(m_wfHeader), 1, m_pFile);

		//	fclose(m_pFile);
		//	m_pFile = NULL;
		//}
	}
	BOOL Write(const BYTE * pData, int nSize)
	{
		//if (fwrite(pData, nSize,1,  m_pFile) != 1)
		//	return FALSE;

		//m_wfHeader.dwDataSize += nSize;
		return TRUE;
	}
	BOOL IsOpen() {
		return (m_pFile != NULL);
	}

public:
	typedef struct tagWAVE_FILE_HEADER {
		DWORD dwRiffFlag; // 'RIFF'
		DWORD dwFileSize;
		DWORD dwWaveFlag; // 'WAVE'
		DWORD dwFmtFlag;  // 'fmt'
		DWORD dwFmtSize;
		PCMWAVEFORMAT pcmFormat;
		DWORD dwDataFlag; // 'data'
		DWORD dwDataSize;
	} WAVE_FILE_HEADER, *PWAVE_FILE_HEADER;

	FILE * m_pFile;
	WAVE_FILE_HEADER m_wfHeader;

public:
	unsigned char* getHeader()
	{
		unsigned char* header = (unsigned char*)malloc(sizeof(WAVE_FILE_HEADER));
		memcpy(header, &m_wfHeader, sizeof(WAVE_FILE_HEADER));
		return header;
	}
	int getHeaderSize()
	{
		return sizeof(WAVE_FILE_HEADER);
	}
};

#endif // __WAVEFILE_QURAM_H__