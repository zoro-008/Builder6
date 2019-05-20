//#pragma once

#ifndef __IMITECH_H
#define __IMITECH_H

#ifdef NO_IMCAMLIB_FILE
#include "ApiImport.h"
#else
// use if compile with ImCam.Lib 
#include "ImCamApi.h"
#endif

#include "ijl.h"

class CImiTech
{
public:
	CImiTech();

public:
	~CImiTech();


	int g_iNumOfCam ;
	int g_iCrntCam;

	int m_nWidth;;
	int m_nHeight;
	int m_nCam[3];

	CString m_strCamSerial;
	HANDLE m_hGrabHnd[2];

	BOOL   m_bCamFlag;

	BYTE *m_pBuf[2];
	BYTE *m_pCImg[2];


	BOOL	InitCam();
	void	SetCamIndex();
	void    SetCamera();
	void	EndCam();
	void	Grab();

	void    SetTriggerMode();
	void	ImgSaveBmp(char *path);

    inline long CImiTech::GetImgWidth()            { return m_nWidth;    }
    inline long CImiTech::GetImgHeight()           { return m_nHeight;   }

};
#endif 