#include "StdAfx.h"
#include "ImiTech.h"
#include "..\\Cximagedll\\ximage.h"

#pragma comment(lib,"ijl15l.lib") 
#pragma comment(lib,"ImCam.lib")
#pragma comment(lib,"cximage")


CImiTech::CImiTech()
{
	int i;

	g_iNumOfCam = 0;
	g_iCrntCam = 0;
	m_bCamFlag = FALSE;

	m_strCamSerial = "";
	for(i=0; i<3; i++)
	{
		m_pCImg[i] = NULL;
		m_pBuf[i] = NULL;
		m_nCam[i] = -1;
	    m_hGrabHnd[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

}

CImiTech::~CImiTech()
{

}
int CALLBACK GrabCallback1(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize )
{
	CImiTech *pCam = (CImiTech*)pContext;
	pCam->m_pBuf[0] = pBuffer; 

	if(pCam->m_pCImg[0] != NULL)
		memcpy(pCam->m_pCImg[0], pCam->m_pBuf[0], pCam->m_nWidth[0]*pCam->m_nHeight[0]);

	SetEvent(pCam->m_hGrabHnd[0]);

	return 0;
}

int CALLBACK GrabCallback2(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize )
{
	CImiTech *pCam = (CImiTech*)pContext;
	pCam->m_pBuf[1] = pBuffer; 

	if(pCam->m_pCImg[1]!= NULL)
		memcpy(pCam->m_pCImg[1], pCam->m_pBuf[1], pCam->m_nWidth[1]*pCam->m_nHeight[1]);

	SetEvent(pCam->m_hGrabHnd[1]);

	return 0;
}

int CALLBACK GrabCallback3(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize )
{
	CImiTech *pCam = (CImiTech*)pContext;
	pCam->m_pBuf[2] = pBuffer; 

	if(pCam->m_pCImg[2]!= NULL)
		memcpy(pCam->m_pCImg[2], pCam->m_pBuf[2], pCam->m_nWidth[2]*pCam->m_nHeight[2]);

	SetEvent(pCam->m_hGrabHnd[2]);

	return 0;
}




BOOL CImiTech::InitCam()
{
	int i;
	char sMsg[256] ;

	ImCamUninit();
	g_iNumOfCam = ImCamInit();

	if(g_iNumOfCam < 3)
	{
		m_nWidth[0]=m_nWidth[1] = 640;
		m_nWidth[2] =640;

		m_nHeight[0]=m_nHeight[1] = 480;
		m_nHeight[2] = 480;

		m_bCamFlag= FALSE;

		sprintf(sMsg,"%d ea Camera is not connected",(3-g_iNumOfCam));

		//AfxMessageBox("camera is not connected");
		AfxMessageBox(sMsg);
	}
	else
	{
		g_iNumOfCam=3;
		m_bCamFlag = TRUE;
		SetCamIndex();
		SetCamera();

/////////////////////////////////////////////
		PreSet();
//////////////////////////////////////////////
	}
	for(i=0;i<3; i++)
	{
		m_pCImg[i]=new unsigned char [m_nWidth[i]*m_nHeight[i]];
		memset(m_pCImg[i], 0x00, m_nWidth[i]*m_nHeight[i]);
	}
	return true;
}


void CImiTech::SetCamIndex()
{
	int i;
	int nSize;
	char strModel[128];
	CString strSerial[3];
	int iSerial[3];
	int max;
	int max_index=2;
	int min;
    int min_index=0;
	int middle_index=1;

///////////////////////////////////
	if(!m_bCamFlag)
		return;

	for(i=0; i<3; i++)
	{
		ImCamGetName(i, &nSize, strModel);
		strSerial[i] = (LPCSTR)(LPSTR)strModel;
		strSerial[i]=  strSerial[i].Mid(nSize-8, 8); 

		iSerial[i]=atoi(strSerial[i]);
		if(i==0)
		{
			max=iSerial[i];
			min=iSerial[i];
			max_index=min_index=0;
		}

		if(iSerial[i] > max)
		{
			max=iSerial[i];
			max_index=i;
		}

		if(iSerial[i] < min)
		{
			min=iSerial[i];
			min_index=i;
		}
	}
	for(i=0; i<3; i++)
	{
		if(iSerial[i] >min && iSerial[i] <max)
           middle_index=i;
	}

//	char pTemp[256];

    //sprintf(pTemp,"cam0_%d",min_index);
    //AfxMessageBox((char *)pTemp      );

	//sprintf(pTemp,"cam1_%d",middle_index );
	//AfxMessageBox((char *)pTemp);

	//sprintf(pTemp,"cam2_%d",max_index );
	//AfxMessageBox((char *)pTemp   );



    m_nCam[0]=min_index;
	m_nCam[1]=middle_index;
	m_nCam[2]=max_index;
}

void CImiTech::SetCamera()
{
	int i;
	int nRet;
	char pTemp[256];
	
    IMC_TRIGGER_PROPERTY TrigProp;

	if(m_bCamFlag)
	{
		for(i=0; i<3; i++)
		{
//			if(i<2)
			{
			nRet = ImCamSetVideoFormat(m_nCam[i], IMC_FORMAT_Y800);
			if ( nRet != IMC_SUCCESS ) {
                sprintf(pTemp,"cam_%d Error ImCamSetVideoFormat",i );
				AfxMessageBox(pTemp);
				return;
			}
            nRet = ImCamSetVideoMode(m_nCam[i], IMC_MODE_640x480);
			if ( nRet != IMC_SUCCESS ) {
                sprintf(pTemp,"cam_%d Error ImCamSetVideoMode",i );
				AfxMessageBox(pTemp);
				return;
			}

			}
//			else
//				ImCamSetVideoFormat(m_nCam[i], IMC_FORMAT_Y800);

			if(i==0)	    ImCamSetCallback(m_nCam[i], GrabCallback1, this);
			else if(i==1)	ImCamSetCallback(m_nCam[i], GrabCallback2, this);
			else 	        ImCamSetCallback(m_nCam[i], GrabCallback3, this);

			nRet = ImCamOpen(m_nCam[i], TRUE, FALSE, NULL, IMC_VIEW_RESIZE, FALSE, IMC_GRAB_RAW, FALSE, IMC_BAYER_GB_RG);
			if ( nRet != IMC_SUCCESS )
			{
                sprintf(pTemp,"cam_%d Error While Opening",i );
				AfxMessageBox(pTemp);
				return;
			}

			nRet = ImCamStart(m_nCam[i]);

			if ( nRet != IMC_SUCCESS )
			{
				sprintf(pTemp,"cam_%d Error While Starting",i );
				AfxMessageBox(pTemp);
				return;
			}
			ImCamGetImageSize(m_nCam[i], &m_nWidth[i], &m_nHeight[i]); 

		}

		TrigProp.bTriggerOn = TRUE;
		TrigProp.nTriggerMode = 0;
		TrigProp.nTriggerParam = 0;
		TrigProp.nTriggerSource = 7; // SW trigger
	//순차적으로 grab ..
		ImCamSetTriggerMode(0, TrigProp);
		ImCamSetTriggerMode(1, TrigProp);
		ImCamSetTriggerMode(2, TrigProp);

	}
}

void CImiTech::EndCam()
{
	int i;

	for(i=0; i<3; i++)
	{
		if (ImCamIsOpen(m_nCam[i]))
		{
			ImCamSetCallback(m_nCam[i], NULL, NULL);
			ImCamStop(m_nCam[i]);
			ImCamClose(m_nCam[i]);
		}

		if(m_pCImg[i] != NULL)	
			delete[] m_pCImg[i];
	}

#ifdef NO_IMCAMLIB_FILE
	UnloadImCamApi();
#endif

}

double CImiTech::UserGetTime()
{
	SYSTEMTIME t;
	double ct=0.0;

	::GetLocalTime(&t);

	ct=(double)t.wHour*3600.0+(double)t.wMinute*60.0+(double)t.wSecond+(double)t.wMilliseconds/1000.0;
	return ct;
}

////////////////////////////////////////////////////////////////////////////
//
// for 8bit image image handling
//
void CImiTech::GetImageFromFile(char *fname,unsigned char *p1,int iW,int iH)
{
	CxImage xi(fname,CXIMAGE_FORMAT_BMP);

	int i;
	int j;
    int k=0;
	int idx;
	
	WORD bpp=xi.GetBpp();
	for(j=0; j<iH; j++)
	{
		k=0;
		idx=iH-j-1;
		for(i=0; i<iW; i++)
		{
			*(p1+iW*j+k)=*(xi.GetBits(idx)+i); // R

			k++;
		}
	}
}

void CImiTech::SaveImageToFile(char *fname,unsigned char *p1,int iW,int iH)
{
    CxImage xi;

    xi.CreateFromArray(p1,iW,iH,8,iW,1);
	xi.Save(fname,CXIMAGE_FORMAT_BMP);
}
//
/////////////////////////////////////////////////////////////////////////////
//
// for 24bit image handling
// 같은 크기의 영상이여야한다.
//
void CImiTech::GetImageFromFile(char *fname,unsigned char *p1,unsigned char *p2,unsigned char *p3)
{
/*	CxImage xi(fname,CXIMAGE_FORMAT_BMP);

	int i;
	int j;
	int iW=m_nWidth[0]*3;
	int iH=m_nHeight[0];
    int k=0;
	int idx;
	
	WORD bpp=xi.GetBpp();
	for(j=0; j<iH; j++)
	{
		k=0;
		idx=iH-j-1;  
		for(i=0; i<iW; i+=3)
		{
			
			*(p3+m_nWidth[0]*j+k)=*(xi.GetBits(idx)+i); // B
			*(p2+m_nWidth[0]*j+k)=*(xi.GetBits(idx)+i+1); // G
			*(p1+m_nWidth[0]*j+k)=*(xi.GetBits(idx)+i+2); // R

			k++;
		}
	}
*/

	CxImage xi(fname,CXIMAGE_FORMAT_BMP);

	int i;
	int j;
	int iW=m_nWidth[0]*3;
	int iH=m_nHeight[0];
    int k=0;
	int idx;
	
	WORD bpp=xi.GetBpp();
	for(j=0; j<iH; j++)
	{
		k=0;
		idx=iH-j-1;  
		for(i=0; i<iW; i+=3)
		{
			
			*(p3+m_nWidth[0]*j+k)=*(xi.GetBits(idx)+i); // B
			*(p2+m_nWidth[0]*j+k)=*(xi.GetBits(idx)+i+1); // G
			*(p1+m_nWidth[0]*j+k)=*(xi.GetBits(idx)+i+2); // R

			k++;
		}
	}

}



void CImiTech::SaveImageToFile(char *fname,unsigned char *p1,unsigned char *p2,unsigned char *p3)
{
	int iW=m_nWidth[0];
	int iH=m_nHeight[0];
	CxImage xi(iW,iH,24,CXIMAGE_FORMAT_BMP);

	int i;
	int j;
	
	for(j=0; j<iH; j++)
	{
		for(i=0; i<iW; i++)
		{
			xi.SetPixelColor(i,iH-j-1,RGB(*(p1+iW*j+i),*(p2+iW*j+i),*(p3+iW*j+i)));
		}
	}

	xi.Save(fname,CXIMAGE_FORMAT_BMP);
}
//
// 24bit image handling
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void CImiTech::Grab()
{
	DWORD dwTimeout;

	ResetEvent(m_hGrabHnd[0]);
	ResetEvent(m_hGrabHnd[1]);
	ResetEvent(m_hGrabHnd[2]);

	if(!m_bCamFlag)
		return;

//	ImCamGetTriggerMode(0,&TrigProp);

//
	dwTimeout = 1000;


//	ImCamWriteBroadcast(m_nCam[0],0xF0F0062C,0x80000000); //
//	ImCamWriteBroadcast(m_nCam[1],0xF0F0062C,0x80000000);
//	ImCamWriteBroadcast(m_nCam[2],0xF0F0062C,0x80000000);

//	ImCamWriteBroadcast(0,0xF0F0062C,0x00000000); //

	ImCamGenerateSWTrigger(m_nCam[0]);
	ImCamGenerateSWTrigger(m_nCam[1]);
	ImCamGenerateSWTrigger(m_nCam[2]);


	WaitForSingleObject(m_hGrabHnd[0], dwTimeout);
	WaitForSingleObject(m_hGrabHnd[1], dwTimeout);
	WaitForSingleObject(m_hGrabHnd[2], dwTimeout);
}


void CImiTech::GetFeatureRange(int iCamNum,int Feature,int *iMin,int *iMax)
{
	if(m_bCamFlag)
	{
//		ImCamGetFeatureRange(m_nCam[iCamNum],Feature,iMin,iMax);	// New Version convert
		IMC_FEATURE_PROPERTY	FeatureProp;
		ImCamGetFeatureValue(m_nCam[iCamNum],Feature, &FeatureProp);
		*iMin = FeatureProp.nMin;
		*iMax = FeatureProp.nMax;
	}
}
void CImiTech::GetCurrentFeature(int iCamNum,int Feature,int *iV)
{
	if(m_bCamFlag)
	{
		IMC_FEATURE_PROPERTY	FeatureProp;
		
		memset(&FeatureProp, 0, sizeof(IMC_FEATURE_PROPERTY));
		if(ImCamGetFeatureValue(m_nCam[iCamNum], Feature, &FeatureProp) == IMC_SUCCESS)
		{
			*iV = FeatureProp.nValue;
		}

// 		if(ImCamIsFeatureSupport(m_nCam[iCamNum],Feature)==IMC_SUCCESS)		// New Version convert
// 		{
// 			ImCamGetFeatureValue(m_nCam[iCamNum],Feature,iV);
//  	}
	}
}

void CImiTech::SetFeatureValue(int iCamNum,int Feature,int iV)
{
	if(m_bCamFlag)
	{
		IMC_FEATURE_PROPERTY	FeatureProp;
		
		memset(&FeatureProp, 0, sizeof(IMC_FEATURE_PROPERTY));
		if(ImCamGetFeatureValue(m_nCam[iCamNum], Feature, &FeatureProp) == IMC_SUCCESS)
		{
			// auto support / auto state
			if( FeatureProp.bAutoCapable )
			{
				FeatureProp.bAuto = FALSE;
			}
			FeatureProp.nValue = iV;
			ImCamSetFeatureValue(m_nCam[iCamNum], Feature, &FeatureProp);
		}


// 		if(ImCamIsFeatureSupport(m_nCam[iCamNum],Feature)==IMC_SUCCESS)
// 		{
// 			if(ImCamIsFeatureAuto(m_nCam[iCamNum],Feature)==1)
// 					ImCamSetFeatureAuto(m_nCam[iCamNum],Feature,FALSE);
// 			ImCamSetFeatureValue(m_nCam[iCamNum],Feature,iV);
//  	}
	}

}

void CImiTech::SetFeatureDefault(int iCamNum,int Feature)
{
	if(m_bCamFlag)
	{
		ImCamSetFeatureDefault(m_nCam[iCamNum],Feature);
	}
}



int CImiTech::Load()
{
	FILE *fp;

	fp=fopen(CAM_CTRL_FILE,"r+b");
	if(fp==NULL) return 0;

	fread(&m_FeatureValue[0][0],sizeof(int),5,fp);
	fread(&m_FeatureValue[1][0],sizeof(int),5,fp);
	fread(&m_FeatureValue[2][0],sizeof(int),5,fp);
	fclose(fp);
	return 1;
}

void CImiTech::PreSet()
{
	int i;
	int j;
	int k=0;

	if(Load())
	{
		for(j=0; j<3; j++)
		{
			 k=0;

			for(i=0;i<5; i++)
			{
				if(i>2) k=5;

				SetFeatureValue(j,i+k,m_FeatureValue[j][i]);
			}
		}
	}
	else
	{
		for(j=0; j<3; j++)
		{
			k=0;
			for(i=0; i<5; i++)
			{
				if(i>2) k=5;

				GetCurrentFeature(j,i+k,&m_FeatureValue[j][i]);
			}
		}
	}
}