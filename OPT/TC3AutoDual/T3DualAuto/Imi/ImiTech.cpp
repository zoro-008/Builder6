#include "StdAfx.h"
#include "ImiTech.h"

#pragma comment(lib,"ijl15l.lib") 
#pragma comment(lib,"ImCam.lib")


CImiTech::CImiTech()
{
	int i;
    char buf[256];

	g_iNumOfCam = 0;
	g_iCrntCam = 0;
	m_bCamFlag = FALSE;

	m_strCamSerial = "";


	for(i=0; i<2; i++)
	{
		m_pCImg[i] = NULL;
		m_pBuf[i] = NULL;
		m_nCam[i] = -1;
		sprintf_s(buf,256,"ev%d",i);
	    m_hGrabHnd[i] = CreateEvent(NULL, TRUE, FALSE,buf);
		::ResetEvent( m_hGrabHnd[i]);
	}
	
}

CImiTech::~CImiTech()
{
	int i;

	for(i=0; i<2; i++)
	{
		::CloseHandle(m_hGrabHnd[i]);
	}
}

int CALLBACK GrabCallback1(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize )
{
	CImiTech *pCam = (CImiTech*)pContext;
	pCam->m_pBuf[0] = pBuffer; 

	if(pCam->m_pCImg[0] != NULL)
		memcpy(pCam->m_pCImg[0], pCam->m_pBuf[0], pCam->m_nWidth*pCam->m_nHeight);

	::SetEvent(pCam->m_hGrabHnd[0]);

	return 0;
}

int CALLBACK GrabCallback2(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize )
{
	CImiTech *pCam = (CImiTech*)pContext;
	pCam->m_pBuf[1] = pBuffer; 

	if(pCam->m_pCImg[1] != NULL)
		memcpy(pCam->m_pCImg[1], pCam->m_pBuf[1], pCam->m_nWidth*pCam->m_nHeight);

	::SetEvent(pCam->m_hGrabHnd[1]);

	return 0;
}

BOOL CImiTech::InitCam()
{
	ImCamUninit();
	g_iNumOfCam = ImCamInit();

	if(g_iNumOfCam < 2)
	{
		m_nWidth = 640;
		m_nHeight = 480;


		m_bCamFlag= FALSE;
		AfxMessageBox("camera is not connected");
		return false;
	}

	m_bCamFlag = TRUE;


//	SetCamIndex();
	SetCamera();
	
	return true;
}


void CImiTech::SetCamIndex()
{
	int i;
	int nSize;
	char strModel[128];
	CString strSerial;


	if(!m_bCamFlag)
		return;

	for(i=0; i<g_iNumOfCam; i++)
	{
		ImCamGetName(i, &nSize, strModel);
		strSerial = (LPCSTR)(LPSTR)strModel;
		strSerial=  strSerial.Mid(nSize-8, 8); 

		if( strSerial == "43700170")
		{
			m_nCam[0] = i;	 
		}

		else if(strSerial == "43700115")
		{
			m_nCam[1] = i;
		}		
	}
}

void CImiTech::SetCamera()
{
	int i;
	int nRet;

	if(m_bCamFlag)
	{

		for(i=0; i<g_iNumOfCam; i++)
		{
			if(i==0)	ImCamSetCallback(i, GrabCallback1, this);
			if(i==1)	ImCamSetCallback(i, GrabCallback2, this);

    		nRet = ImCamOpen(i, TRUE, FALSE, NULL, IMC_VIEW_RESIZE, FALSE, IMC_GRAB_RAW, FALSE, IMC_BAYER_GB_RG);
			if ( nRet != IMC_SUCCESS )
			{
				AfxMessageBox("Error while Opening");
				return;
			}


//			ImCamSetVideoMode(i,IMC_MODE_640x480);
		}


		ImCamGetImageSize(0, &m_nWidth, &m_nHeight); 
	}


	for(i=0; i<2; i++)
	{
		m_pCImg[i]=new unsigned char [m_nWidth*m_nHeight];
	}
	
	for(i=0; i<g_iNumOfCam; i++)
	{
		nRet = ImCamStart(i);
		if ( nRet != IMC_SUCCESS )
		{
			AfxMessageBox("Error while starting");
			return;
		}

	}

//	SetTriggerMode();
}

void CImiTech::EndCam()
{
	int i;

	for (i=0; i<g_iNumOfCam; i++)
	{
		if (ImCamIsOpen(i))
		{
			ImCamSetCallback(i, NULL, NULL);
			ImCamStop(i);
            ImCamClose(i);

		}
	}

	
	for(i=0; i<2; i++)
	{
		if(m_pCImg[i] != NULL)	delete m_pCImg[i];
	}
#ifdef NO_IMCAMLIB_FILE
	UnloadImCamApi();
#endif
}

void CImiTech::SetTriggerMode()
{
    IMC_TRIGGER_PROPERTY TrigProp;

	TrigProp.bTriggerOn     =TRUE;
	TrigProp.nTriggerMode   =0;
	TrigProp.nTriggerParam  =0;
	TrigProp.nTriggerSource =7; // SW trigger

    ImCamSetTriggerMode(0, TrigProp);
    ImCamSetTriggerMode(1, TrigProp);

}

void CImiTech::Grab()
{

	::ResetEvent(m_hGrabHnd[0]);
	::ResetEvent(m_hGrabHnd[1]);

	if(!m_bCamFlag)
		return;

	SetTriggerMode();

	ImCamGenerateSWTrigger(0);
	ImCamGenerateSWTrigger(1);

	::WaitForSingleObject(m_hGrabHnd[0],INFINITE);
	::WaitForSingleObject(m_hGrabHnd[1],INFINITE);

//	ImCamWriteBroadcast(0, 0xF0F0062C, 80000000);
}

void CImiTech::ImgSaveBmp(char *path)
{
/*
	int ret;
	char buf[256];
	char *name;
	name = "_1.bmp";
	sprintfs(buf,256,"%s%s",path,name);

	ret = ImCamSaveBMP(0, buf, FALSE, m_nWidth, m_nHeight);

	name = "_2.bmp";
	sprintfs(buf,256,"%s%s",path,name);
	ret = ImCamSaveBMP(1, buf, FALSE, m_nWidth, m_nHeight);
*/
}
