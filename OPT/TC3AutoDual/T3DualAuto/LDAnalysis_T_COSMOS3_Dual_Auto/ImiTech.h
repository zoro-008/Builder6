//#pragma once

#ifndef __IMITECH_H
#define __IMITECH_H

#ifdef NO_IMCAMLIB_FILE
#include "ApiImport.h"
#else
// use if compile with ImCam.Lib 
#include "..\\Imi\\ImCamApi.h"
#endif

#define CAM_CTRL_FILE  "cam_ctrl.bin"

#include "..\\Imi\\ijl.h"

class CImiTech
{
public:
	CImiTech();

public:
	~CImiTech();
	
	int m_FeatureValue[3][5];

	int g_iNumOfCam ;
	int g_iCrntCam;

	int m_nWidth[3];
	int m_nHeight[3];
	int m_nCam[3];

	CString m_strCamSerial;
	HANDLE m_hGrabHnd[3];

	BOOL   m_bCamFlag;

	BYTE *m_pBuf[3];
	BYTE *m_pCImg[3];

	int     Load();
	void    PreSet();
	BOOL	InitCam();
	void	SetCamIndex();
	void    SetCamera();
	void	EndCam();
	double  UserGetTime();
	void	Grab();
///////////////////////////////////////////////////////////////////////
// for 8bit image
	void GetImageFromFile(char *fname,unsigned char *p1,int iW,int iH);
	void SaveImageToFile(char *fname,unsigned char *p1,int iW,int iH);
/////////////////////////////////////////////////////////////////////////
// for 24bit image
	void GetImageFromFile(char *fname,unsigned char *p1,unsigned char *p2,unsigned char *p3);
	void SaveImageToFile(char *fname,unsigned char *p1,unsigned char *p2,unsigned char *p3);

	void GetFeatureRange(int iCamNum,int Feature,int *iMin,int *iMax);
	void GetCurrentFeature(int iCamNum,int Feature,int *iV);
	void SetFeatureDefault(int iCamNum,int Feature);
	void SetFeatureValue(int iCamNum,int Feature,int iV);

    inline long CImiTech::GetImgWidth(int idx)            { return m_nWidth[idx];    }
    inline long CImiTech::GetImgHeight(int idx)           { return m_nHeight[idx];   }

};
#endif 