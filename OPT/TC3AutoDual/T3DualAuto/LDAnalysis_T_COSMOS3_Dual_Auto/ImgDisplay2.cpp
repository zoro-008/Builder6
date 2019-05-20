// ImgDisplay2.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "MainFrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "ImgDisplay2.h"
#include "..\\guisrc\\DisplayToGDI.h"

#include "External.h"
#include "HVSystemDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//d=2*f*tan(theta/2);

/////////////////////////////////////////////////////////////////////////////
// 광축 영상 처리 프로그램

char *gTextM[2][8]=
{
	{"-10","-20","-30","-40","-50","-60","-70","-80"},
	{"10","20","30","40","50","60","70","80"}
};
char *gTextP[2][8]=
{ 
	{"10","20","30","40","50","60","70","80"},
	{"-10","-20","-30","-40","-50","-60","-70","-80"}
};


CImgDisplay2::CImgDisplay2(CWnd* pParent /*=NULL*/)
	: CDialog(CImgDisplay2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImgDisplay2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nGridDisplayTag=0;
//	m_nGRoiTag=0;
	m_pLogPalette=NULL;
	m_pBitmapBuffer=NULL;
	m_pSrc=NULL;
	m_pOrigin=NULL;
	m_pOut=NULL;
	m_nWidth=640;
	m_nHeight=480;
	m_nSaveBmp=0;
	m_nOkNgStatus=1;
	m_nROffset=m_nTOffset=0.0;

#ifdef __SAMSUNG_TSST_VERSION
	m_nAngleCD  =0.0;
	m_nTiltCD   =0.0;
	m_cAngleCD.x=0.0;
	m_cAngleCD.y=0.0;
#endif


}


void CImgDisplay2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImgDisplay2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImgDisplay2, CDialog)
	//{{AFX_MSG_MAP(CImgDisplay2)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgDisplay2 message handlers

BOOL CImgDisplay2::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN) {
		switch(pMsg->wParam) {
			case VK_RETURN:
			case VK_ESCAPE: return true;
			default: 
				 break; 
		}
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

void CImgDisplay2::OnDestroy() 
{
	CDialog::OnDestroy();
	::DeleteObject(m_hPal);

	DeleteCriticalSection(&m_pImage2);
	
	if(m_pLogPalette!=NULL) delete[] m_pLogPalette;
	if(m_pBitmapBuffer!=NULL) delete[] m_pBitmapBuffer;
	if(m_pSrc!=NULL) delete[] m_pSrc;
	if(m_pOut!=NULL) delete[] m_pOut;
	if(m_pOrigin!=NULL) delete[] m_pOrigin;
	
	// TODO: Add your message handler code here
	
}

BOOL CImgDisplay2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	BitmapInit();

	m_pSrc=new unsigned char [m_nWidth*m_nHeight];
	memset(m_pSrc,0x00,m_nWidth*m_nHeight);
	m_pOrigin= new unsigned char [m_nWidth*m_nHeight];
	memset(m_pOrigin,0x00,m_nWidth*m_nHeight);
	m_pOut= new unsigned char [m_nWidth*m_nHeight];
	memset(m_pOut,0x00,m_nWidth*m_nHeight);
	
	m_nCx=(double)m_nWidth/2.;
    m_nCy=(double)m_nHeight/2.;

	InitializeCriticalSection(&m_pImage2);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CImgDisplay2::BitmapInit()
{
	int i;
	
//////////////////////////////////////////////////////
// bitmap header setting
//////////////////////////////////////////////////////
	m_BitmapInfoHeader.biSize         =(DWORD)40;
	m_BitmapInfoHeader.biWidth        =(LONG)m_nWidth;
	m_BitmapInfoHeader.biHeight       =(LONG)m_nHeight;
	m_BitmapInfoHeader.biPlanes       =(WORD)1;
	m_BitmapInfoHeader.biBitCount     =(WORD)8;
	m_BitmapInfoHeader.biCompression  =(DWORD)BI_RGB;
	m_nDummy=(m_nWidth%4);
	((4-m_nDummy)==4) ? m_nDummy=0:m_nDummy=4-m_nDummy;
	m_BitmapInfoHeader.biSizeImage    =(DWORD)((m_nWidth+m_nDummy)*m_nHeight);
	m_BitmapInfoHeader.biXPelsPerMeter=(LONG)0;
	m_BitmapInfoHeader.biYPelsPerMeter=(LONG)0;
	m_BitmapInfoHeader.biClrUsed      =(DWORD)0;
	m_BitmapInfoHeader.biClrImportant =(DWORD)0;

	m_pLogPalette=(LOGPALETTE*)new WORD [sizeof(WORD)+sizeof(PALETTEENTRY)*256];

	m_pLogPalette->palNumEntries=(WORD)256;
	m_pLogPalette->palVersion   =(WORD)0x300;
   
	for(i=0; i<256; i++)
	{
		m_pLogPalette->palPalEntry[i].peRed  =m_aBitmapPalette[i].peRed  =(BYTE)i;
		m_pLogPalette->palPalEntry[i].peGreen=m_aBitmapPalette[i].peGreen=(BYTE)i;
		m_pLogPalette->palPalEntry[i].peBlue =m_aBitmapPalette[i].peBlue =(BYTE)i;
		m_pLogPalette->palPalEntry[i].peFlags=m_aBitmapPalette[i].peFlags=(BYTE)0;
	}
	m_pBitmapBuffer=new unsigned char [(m_nWidth+m_nDummy)*m_nHeight];
	memset(m_pBitmapBuffer,0x00,(m_nWidth+m_nDummy)*m_nHeight);
	m_hPal=::CreatePalette(m_pLogPalette);

}

void CImgDisplay2::ReSetBitmapInfo(int nWidth,int nHeight)
{
	if(m_pBitmapBuffer!=NULL) delete[] m_pBitmapBuffer;

	m_BitmapInfoHeader.biWidth        =(LONG)nWidth;
	m_BitmapInfoHeader.biHeight       =(LONG)nHeight;
	m_nDummy=(nWidth%4);
	((4-m_nDummy)==4) ? m_nDummy=0:m_nDummy=4-m_nDummy;
	m_BitmapInfoHeader.biSizeImage    =(DWORD)((nWidth+m_nDummy)*nHeight);

	m_pBitmapBuffer=new unsigned char [(nWidth+m_nDummy)*nHeight];
	memset(m_pBitmapBuffer,0x00,(nWidth+m_nDummy)*nHeight);

	if(m_pSrc!=NULL) delete[] m_pSrc;
	m_pSrc=new unsigned char [nWidth*nHeight];
	memset(m_pSrc,0x00,nWidth*nHeight);
}
void CImgDisplay2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    EnterCriticalSection(&m_pImage2);
	Draw256Image(&dc,m_pSrc);
	LeaveCriticalSection(&m_pImage2);
	return;	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}
void CImgDisplay2::ImgUpdate(unsigned char *pImg)
{

	memcpy(m_pSrc,pImg,m_nWidth*m_nHeight);
	Invalidate(FALSE);
}

void CImgDisplay2::SetBuffer(unsigned char *pImg)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	

//	memcpy(m_pSrc,pImg,m_nWidth*m_nHeight);
//	memcpy(m_pOrigin,pImg,m_nWidth*m_nHeight);



	int xt_factor,yt_factor;
	yt_factor=(int)(-m_nROffset/m_nScaleFactor+0.5);
	xt_factor=(int)(m_nTOffset/m_nScaleFactor+0.5);

    EnterCriticalSection(&m_pImage2);
	if(m_pView->m_wTabWindow.m_pwEtc->GetWorkMode())
		memcpy(m_pSrc,pImg,m_nWidth*m_nHeight);
	else
	{
		ImageTanslate(xt_factor,yt_factor,pImg);
		memcpy(m_pSrc,m_pOut,m_nWidth*m_nHeight);
	}

	memcpy(m_pOrigin,pImg,m_nWidth*m_nHeight);
	LeaveCriticalSection(&m_pImage2);

	Invalidate(FALSE);
}

void CImgDisplay2::ImageTanslate(int x,int y,unsigned char *pImg)
{
	int i,j;

	memset(m_pOut,0x00,m_nWidth*m_nHeight);
	for(j=0; j<m_nHeight; j++)
	{
		if((j+y) <0 || (j+y)>=m_nHeight)	continue;
		for(i=0; i<m_nWidth; i++)
		{
			if((i+x) <0 || (i+x)>=m_nWidth)	continue;
			*(m_pOut+m_nWidth*(j+y)+i+x)= *(pImg+m_nWidth*j+i);

		}
	}
	return;
}

void CImgDisplay2::Draw256Image(CPaintDC *dc,unsigned char *pImg)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	
	int i;

	
	m_hOldPal=::SelectPalette(dc->GetSafeHdc(),m_hPal,FALSE);
	::RealizePalette(dc->GetSafeHdc());

	CDC m_MemDC;
	m_MemDC.CreateCompatibleDC(dc);

	int start=m_nHeight-1;
	int tmpWidth=m_nWidth+m_nDummy;
	

	for(i=start; i>=0; i--)
		memcpy((m_pBitmapBuffer+tmpWidth*(start-i)),(pImg+m_nWidth*i),m_nWidth);
    
    
	CBitmap m_Bitmap,*m_OldBitmap;

	 m_Bitmap.CreateCompatibleBitmap(dc,tmpWidth,m_nHeight);
	 m_OldBitmap=m_MemDC.SelectObject(&m_Bitmap);

	::SetStretchBltMode(m_MemDC.GetSafeHdc(),COLORONCOLOR);
	::SetDIBitsToDevice(m_MemDC.GetSafeHdc(),0,0,tmpWidth,m_nHeight,
		                             0,0,0,m_nHeight,
									 m_pBitmapBuffer,
									 (LPBITMAPINFO)&m_BitmapInfoHeader,
									 DIB_RGB_COLORS);
////////////////////////
	if(m_nGridDisplayTag==1)
		DisplayGrid(&m_MemDC);
	dc->BitBlt( 0,0,m_nWidth,m_nHeight,&m_MemDC,0,0,SRCCOPY);
}



void CImgDisplay2::SettingParam()
{
	double factor;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	

	memcpy(&m_cSettingDataAdd,&m_pView->m_cSettingDataAdd,sizeof(SETTING_DATA_ADD));
	memcpy(&m_cSettingData,&m_pView->m_cSettingData,sizeof(SETTING_DATA));
	memcpy(&m_cSettingDataEx,&m_pView->m_cSettingDataEx,sizeof(SETTING_DATAEX));

	MakeCoordTable();

	factor=((double)MAX_MINUTE_IMG2/m_nScaleFactor);
	m_cHsp.x=(m_nCx-factor);
	m_cHsp.y=m_nCy;
	m_cVsp.y=(m_nCy-factor);
	m_cVsp.x=m_nCx;

	m_cHep.x=(m_nCx+factor);
	m_cHep.y=m_nCy;
	m_cVep.y=(m_nCy+factor);
	m_cVep.x=m_nCx;

	MakeGridCoord();
	SelectDisplayType();
	
    m_pView->m_wDisplayComm.AddListData("parameter uploading success");
	return;
}


void CImgDisplay2::SelectDisplayType()
{
    SetGridDisplayTag(1);
	Invalidate(FALSE);
}


void CImgDisplay2::MakeCoordTable()
{
	int i,j;
	// X -> R, Y ->T
	m_nScaleFactor=(double)m_cSettingData.nMinute/(double)m_cSettingData.nPixel;

#ifndef SANTA_BEAM_AXIS_CHANGE
	if(m_cSettingData.nRDir ==0 && m_cSettingData.nTDir==0)
	{
		for(j=0; j<m_nHeight; j++)
			for(i=0; i<m_nWidth; i++)
			{
				m_cMinuteCoordinate[j][i].x=((double)i - m_nCx) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].y=(m_nCy-(double)j) * m_nScaleFactor;

				m_cDistanceCoordinate[j][i].x=((double)i - m_nCx) * DISTANCE_SCALE_IMG2;
				m_cDistanceCoordinate[j][i].y=(m_nCy-(double)j) * DISTANCE_SCALE_IMG2;

			}
		return;
	}

	if(m_cSettingData.nRDir ==1 && m_cSettingData.nTDir==0)
	{
		for(j=0; j<m_nHeight; j++)    //t
			for(i=0; i<m_nWidth; i++) //r
			{
				m_cMinuteCoordinate[j][i].x=(double)(m_nCx - (double)i) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].y=(double)(m_nCy-(double)j) * m_nScaleFactor;

				m_cDistanceCoordinate[j][i].x=(double)(m_nCx - (double)i) * DISTANCE_SCALE_IMG2;
				m_cDistanceCoordinate[j][i].y=(double)(m_nCy-(double)j) * DISTANCE_SCALE_IMG2;
			}
		return;
	}

	if(m_cSettingData.nRDir ==0 && m_cSettingData.nTDir==1)
	{
		for(j=0; j<m_nHeight; j++) //t
			for(i=0; i<m_nWidth; i++) //r
			{
				m_cMinuteCoordinate[j][i].x=((double)i - m_nCx) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].y=((double)j - m_nCy) * m_nScaleFactor;

				m_cDistanceCoordinate[j][i].x=((double)i - m_nCx) * DISTANCE_SCALE_IMG2;
				m_cDistanceCoordinate[j][i].y=((double)j - m_nCy) * DISTANCE_SCALE_IMG2;
			}

		return;
	}

	if(m_cSettingData.nRDir ==1 && m_cSettingData.nTDir==1)
	{
		for(j=0; j<m_nHeight; j++)
			for(i=0; i<m_nWidth; i++)
			{
				m_cMinuteCoordinate[j][i].x=(m_nCx - (double)i) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].y=((double)j-m_nCy) * m_nScaleFactor;

				m_cDistanceCoordinate[j][i].x=(m_nCx - (double)i) * DISTANCE_SCALE_IMG2;
				m_cDistanceCoordinate[j][i].y=((double)j-m_nCy) * DISTANCE_SCALE_IMG2;
			}
	}
#else // SANTA Beam axis changeed!!!!!!!
	if(m_cSettingData.nRDir ==0 && m_cSettingData.nTDir==0)
	{
		for(j=0; j<m_nHeight; j++)
			for(i=0; i<m_nWidth; i++)
			{
				m_cMinuteCoordinate[j][i].y=((double)i - m_nCx) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].x=(m_nCy-(double)j) * m_nScaleFactor;

				m_cDistanceCoordinate[j][i].y=((double)i - m_nCx) * DISTANCE_SCALE_IMG2;
				m_cDistanceCoordinate[j][i].x=(m_nCy-(double)j) * DISTANCE_SCALE_IMG2;

			}
		return;
	}

	if(m_cSettingData.nRDir ==1 && m_cSettingData.nTDir==0)
	{
		for(j=0; j<m_nHeight; j++)    //t
			for(i=0; i<m_nWidth; i++) //r
			{
				m_cMinuteCoordinate[j][i].y=(double)(m_nCx - (double)i) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].x=(double)(m_nCy-(double)j) * m_nScaleFactor;

				m_cDistanceCoordinate[j][i].y=(double)(m_nCx - (double)i) * DISTANCE_SCALE_IMG2;
				m_cDistanceCoordinate[j][i].x=(double)(m_nCy-(double)j) * DISTANCE_SCALE_IMG2;
			}
		return;
	}

	if(m_cSettingData.nRDir ==0 && m_cSettingData.nTDir==1)
	{
		for(j=0; j<m_nHeight; j++) //t
			for(i=0; i<m_nWidth; i++) //r
			{
				m_cMinuteCoordinate[j][i].y=((double)i - m_nCx) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].x=((double)j - m_nCy) * m_nScaleFactor;

				m_cDistanceCoordinate[j][i].y=((double)i - m_nCx) * DISTANCE_SCALE_IMG2;
				m_cDistanceCoordinate[j][i].x=((double)j - m_nCy) * DISTANCE_SCALE_IMG2;
			}

		return;
	}

	if(m_cSettingData.nRDir ==1 && m_cSettingData.nTDir==1)
	{
		for(j=0; j<m_nHeight; j++)
			for(i=0; i<m_nWidth; i++)
			{
				m_cMinuteCoordinate[j][i].y=(m_nCx - (double)i) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].x=((double)j-m_nCy) * m_nScaleFactor;

				m_cDistanceCoordinate[j][i].y=(m_nCx - (double)i) * DISTANCE_SCALE_IMG2;
				m_cDistanceCoordinate[j][i].x=((double)j-m_nCy) * DISTANCE_SCALE_IMG2;
			}
	}
#endif
	return;
}



void CImgDisplay2::MinuteToPixel(double rValue,double tValue,POINT *coord)
{
	// X -> R, Y ->T
#ifndef SANTA_BEAM_AXIS_CHANGE
	if(m_cSettingData.nRDir ==0 && m_cSettingData.nTDir==0)	
	{
		coord->x=(int)(m_nCx +rValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy -tValue/m_nScaleFactor+0.5);
		return;
	}

	if(m_cSettingData.nRDir ==1 && m_cSettingData.nTDir==0)	
	{
		coord->x=(int)(m_nCx -rValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy -tValue/m_nScaleFactor+0.5);
		return;
	}
	
	if(m_cSettingData.nRDir ==0 && m_cSettingData.nTDir==1)	
	{
		coord->x=(int)(m_nCx +rValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy +tValue/m_nScaleFactor+0.5);
		return;
	}
	
	if(m_cSettingData.nRDir ==1 && m_cSettingData.nTDir==1)	
	{
		coord->x=(int)(m_nCx -rValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy +tValue/m_nScaleFactor+0.5);
	}
#else
	if(m_cSettingData.nRDir ==0 && m_cSettingData.nTDir==0)	
	{
		coord->x=(int)(m_nCx +tValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy -rValue/m_nScaleFactor+0.5);
		return;
	}

	if(m_cSettingData.nRDir ==1 && m_cSettingData.nTDir==0)	
	{
		coord->x=(int)(m_nCx -tValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy -rValue/m_nScaleFactor+0.5);
		return;
	}
	
	if(m_cSettingData.nRDir ==0 && m_cSettingData.nTDir==1)	
	{
		coord->x=(int)(m_nCx +tValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy +rValue/m_nScaleFactor+0.5);
		return;
	}
	
	if(m_cSettingData.nRDir ==1 && m_cSettingData.nTDir==1)	
	{
		coord->x=(int)(m_nCx -tValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy +rValue/m_nScaleFactor+0.5);
	}
#endif

	return;
}


void CImgDisplay2::MakeGridCoord()
{
	double factor;
    usrPOINT tsp,tep;
	usrPOINT tspt,tept;
	int offset=2;
    int k,kk;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	



//	cDisp.DisplayLine(m_cHsp,m_cHep,RGB(0,255,0),1);
//	cDisp.DisplayLine(m_cVsp,m_cVep,RGB(0,255,0),1);

	factor=1.0/m_nScaleFactor;

	tsp.y=m_nCy-offset;
	tep.y=m_nCy+offset+1;
	double nnx=m_nCx;

	k=0; kk=0;
	//+ direction
	while(k<=MAX_MINUTE_IMG2)
	{
       tsp.x=nnx;   tep.x=nnx; 

   	   if(k>0 && (k%10)==0)  {
			tsp.y-=5;	tep.y+=5;

			memcpy(&m_cLHp[kk].sp,&tsp,sizeof(usrPOINT));
			memcpy(&m_cLHp[kk].ep,&tep,sizeof(usrPOINT));

			tspt.x=tsp.x;	tspt.y=m_cVsp.y;
			tept.x=tsp.x;	tept.y=m_cVep.y;
			memcpy(&m_cLHpG[kk].sp,&tspt,sizeof(usrPOINT));
			memcpy(&m_cLHpG[kk].ep,&tept,sizeof(usrPOINT));

			tsp.y+=5;	tep.y-=5;
			kk++;
	   }
	   memcpy(&m_cSHp[k].sp,&tsp,sizeof(usrPOINT));
	   memcpy(&m_cSHp[k].ep,&tep,sizeof(usrPOINT));
   	   nnx=nnx+factor;
	   k++;       
	}

	nnx=m_nCx;	k=0;	kk=0;
	//- direction
	while(k<=MAX_MINUTE_IMG2)
	{
       tsp.x=nnx; 	   tep.x=nnx; 
   	   if(k>0 && (k%10)==0)  {
			tsp.y-=5;	tep.y+=5;
			memcpy(&m_cLHm[kk].sp,&tsp,sizeof(usrPOINT));
			memcpy(&m_cLHm[kk].ep,&tep,sizeof(usrPOINT));

			tspt.x=tsp.x;	tspt.y=m_cVsp.y;
			tept.x=tsp.x;	tept.y=m_cVep.y;
			memcpy(&m_cLHmG[kk].sp,&tspt,sizeof(usrPOINT));
			memcpy(&m_cLHmG[kk].ep,&tept,sizeof(usrPOINT));
			tsp.y+=5;		tep.y-=5;
			kk++;
	   }
   	   memcpy(&m_cSHm[k].sp,&tsp,sizeof(usrPOINT));
	   memcpy(&m_cSHm[k].ep,&tep,sizeof(usrPOINT));
	   nnx=nnx-factor;
       k++;
	}
	
	// vertical direction..
	tsp.x=m_nCx-offset;	tep.x=m_nCx+offset+1;
	nnx=m_nCy;   k=0;	kk=0;
	while(k<=MAX_MINUTE_IMG2)
	{
       tsp.y=nnx;   tep.y=nnx;
   	   if(k>0 && (k%10)==0)
	   {
			tsp.x-=5;	tep.x+=5;

			memcpy(&m_cLVm[kk].sp,&tsp,sizeof(usrPOINT));
			memcpy(&m_cLVm[kk].ep,&tep,sizeof(usrPOINT));

			tspt.x=m_cHsp.x;	tspt.y=tsp.y;
			tept.x=m_cHep.x;	tept.y=tsp.y;
			memcpy(&m_cLVmG[kk].sp,&tspt,sizeof(usrPOINT));
			memcpy(&m_cLVmG[kk].ep,&tept,sizeof(usrPOINT));

			tsp.x+=5;		tep.x-=5;
			kk++;
	   }
   	   memcpy(&m_cSVm[k].sp,&tsp,sizeof(usrPOINT));
	   memcpy(&m_cSVm[k].ep,&tep,sizeof(usrPOINT));

	   nnx=nnx+factor;       
	   k++;
	}

	nnx=m_nCy;	k=0;	kk=0;
	while(k<=MAX_MINUTE_IMG2)
	{
       tsp.y=nnx; 	   tep.y=nnx; 
   	   if(k>0 && (k%10)==0)
	   {
			tsp.x-=5;	tep.x+=5;
			memcpy(&m_cLVp[kk].sp,&tsp,sizeof(usrPOINT));
			memcpy(&m_cLVp[kk].ep,&tep,sizeof(usrPOINT));

			tspt.x=m_cHsp.x;	tspt.y=tsp.y;
			tept.x=m_cHep.x;	tept.y=tsp.y;
			memcpy(&m_cLVpG[kk].sp,&tspt,sizeof(usrPOINT));
			memcpy(&m_cLVpG[kk].ep,&tept,sizeof(usrPOINT));

			tsp.x+=5;	tep.x-=5;
			kk++;
	   }
   	   memcpy(&m_cSVp[k].sp,&tsp,sizeof(usrPOINT));
	   memcpy(&m_cSVp[k].ep,&tep,sizeof(usrPOINT));
	   nnx=nnx-factor;
       k++;
	}
	m_pView->m_wDisplayComm.AddListData("grid data setting finished");
	return;
}

void CImgDisplay2::DisplayGrid(CDC *memDC)
{
	register int i;
	int nBlobSize;

	POINT zeropt,cCenter;
    CDisplayToGDI cDisp;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	


	zeropt.x=m_nWidth/2;
	zeropt.y=m_nHeight/2;
	
	memDC->SetBkMode(TRANSPARENT);
	if(m_cSettingData.nDispType!=2)
	{
		cDisp.DLine(memDC,m_cHsp,m_cHep,RGB(0,255,0));
		cDisp.DLine(memDC,m_cVsp,m_cVep,RGB(0,255,0));

		for(i=0; i<MAX_MINUTE_IMG2; i++)
		{
			cDisp.DLine(memDC,m_cSHp[i].sp,m_cSHp[i].ep,RGB(0,255,0),1);		
			cDisp.DLine(memDC,m_cSHm[i].sp,m_cSHm[i].ep,RGB(0,255,0),1);		
			cDisp.DLine(memDC,m_cSVp[i].sp,m_cSVp[i].ep,RGB(0,255,0),1);		
			cDisp.DLine(memDC,m_cSVm[i].sp,m_cSVm[i].ep,RGB(0,255,0),1);		
		}

		for(i=0; i<8; i++)
		{
			if(m_cSettingData.nDispType==0)
			{
				cDisp.DLine(memDC,m_cLHp[i].sp,m_cLHp[i].ep,RGB(255,255,0),1);
				cDisp.DLine(memDC,m_cLHm[i].sp,m_cLHm[i].ep,RGB(255,255,0),1);		
				cDisp.DLine(memDC,m_cLVp[i].sp,m_cLVp[i].ep,RGB(255,255,0),1);		
				cDisp.DLine(memDC,m_cLVm[i].sp,m_cLVm[i].ep,RGB(255,255,0),1);		
			}
			else
			{
				cDisp.DDotLine(memDC,m_cLHpG[i].sp,m_cLHpG[i].ep,RGB(255,128,32));
				cDisp.DDotLine(memDC,m_cLHmG[i].sp,m_cLHmG[i].ep,RGB(255,128,32));		
				cDisp.DDotLine(memDC,m_cLVpG[i].sp,m_cLVpG[i].ep,RGB(255,128,32));		
				cDisp.DDotLine(memDC,m_cLVmG[i].sp,m_cLVmG[i].ep,RGB(255,128,32));		
			}
		
			cDisp.DTextOut(memDC,(int)m_cLHp[i].sp.x,(int)m_cLHp[i].ep.y+5,gTextP[m_cSettingData.nRDir][i],RGB(0,255,0));
			cDisp.DTextOut(memDC,(int)m_cLHm[i].sp.x,(int)m_cLHm[i].ep.y+5,gTextM[m_cSettingData.nRDir][i],RGB(0,255,0));

			if(i==3)
			{
				cDisp.DTextOut(memDC,(int)m_cLVp[i].ep.x+16,(int)m_cLVp[i].sp.y+16,gTextP[m_cSettingData.nTDir][i],RGB(0,255,0));
				cDisp.DTextOut(memDC,(int)m_cLVm[i].ep.x+16,(int)m_cLVm[i].sp.y-16,gTextM[m_cSettingData.nTDir][i],RGB(0,255,0));	
			}
			else
			{
				// 정상적 vertical 상수 디스플레이 
				cDisp.DTextOut(memDC,(int)m_cLVp[i].ep.x+16,(int)m_cLVp[i].sp.y-4,gTextP[m_cSettingData.nTDir][i],RGB(0,255,0));
				cDisp.DTextOut(memDC,(int)m_cLVm[i].ep.x+16,(int)m_cLVm[i].sp.y-4,gTextM[m_cSettingData.nTDir][i],RGB(0,255,0));	
			}
		}
#ifndef SANTA_BEAM_AXIS_CHANGE
		cDisp.DTextOutWithFontII(memDC,280,5,12,24,"T",RGB(0,255,255));
		cDisp.DTextOutWithFontII(memDC,600,230,12,24,"R",RGB(0,255,255));

#else
		cDisp.DTextOutWithFontII(memDC,280,5,12,24,"R",RGB(0,255,255));
		cDisp.DTextOutWithFontII(memDC,600,230,12,24,"T",RGB(0,255,255));
#endif
	}
	
	char buf[256];
	if(m_nFindFlag==1)
	{
//		for(i=0; i<3; i++)
//		{
//			cDisp.DEllipse(memDC,m_cBeamRect[i],RGB(255,0,0));
//			cDisp.DCrossPoint(memDC,m_cCpt[i],4,RGB(0,255,0));
//		}
		cCenter.x=(int)(m_cCpt[0].x+0.5);
		cCenter.y=(int)(m_cCpt[0].y+0.5);
		cDisp.DCrossPoint(memDC,m_cCpt[0],4,RGB(0,255,0));
		cDisp.DLine(memDC,zeropt,cCenter,RGB(255,0,255));
//		cDisp.DLine(memDC,m_cCpt[1],m_cCpt[2],RGB(0,0,255));

#ifdef __SAMSUNG_TSST_VERSION
		cDisp.DTextOutWithFontII(memDC,10,30,10,18,"DVD",RGB(255,0,0));
#else
		cDisp.DTextOutWithFontII(memDC,10,30,10,18,"ANGLE",RGB(255,0,0));
#endif
		sprintf(buf,"%5.3f (R: %5.3f , T: %5.3f)",m_nAngle,m_cAngle.x,m_cAngle.y);//<--------20110124 lwb 이 부분을 모션에 보냄.m_nOkNgStatus의 결과도 보냄.
		cDisp.DTextOutWithFontII(memDC,80,30,10,18,buf,RGB(0,255,0));		
				
		cDisp.DTextOutWithFontII(memDC,10,50,10,18,"TILT",RGB(255,0,0));	
		sprintf(buf,"%5.3f",m_nTilt);
		cDisp.DTextOutWithFontII(memDC,80,50,10,18,buf,RGB(0,255,0));	

		nBlobSize =	m_nAxisBeamSize;
		cDisp.DTextOutWithFontII(memDC,10,70,10,18,"Blob Num",RGB(255,0,0));	
		sprintf(buf,"%10d",nBlobSize);
		cDisp.DTextOutWithFontII(memDC,100,70,10,18,buf,RGB(0,255,0));

#ifdef __SAMSUNG_TSST_VERSION


		cDisp.DTextOutWithFontII(memDC,10,380,10,18,"Angle",RGB(255,0,0));
		sprintf(buf,"  %5.3f",ComputeAngle());
		cDisp.DTextOutWithFontII(memDC,80,380,10,18,buf,RGB(0,255,0));	

		cDisp.DTextOutWithFontII(memDC,10,400,10,18,"CD",RGB(255,0,0));
		sprintf(buf,"%5.3f (R: %5.3f , T: %5.3f)",m_nAngleCD,m_cAngleCD.x,m_cAngleCD.y);

		cDisp.DTextOutWithFontII(memDC,80,400,10,18,buf,RGB(0,255,0));	

		cDisp.DTextOutWithFontII(memDC,10,420,10,18,"TILT",RGB(255,0,0));	
		sprintf(buf,"%5.3f",m_nTiltCD);
		cDisp.DTextOutWithFontII(memDC,80,420,10,18,buf,RGB(0,255,0));	

#endif

	}
//////////////////////////////////////////////
// TSST 요청에 따라서 변경 2009년 1월 13일 	

//	if(m_nFindFlag==2)
//	{
//		cDisp.DEllipse(memDC,m_cBeamRect[0],RGB(255,0,0));
//		cDisp.DCrossPoint(memDC,m_cCpt[0],4,RGB(0,255,0));
//		cDisp.DTextOutWithFontII(memDC,10,30,10,18,"ANGLE",RGB(255,0,0));
////#ifndef SATA_BEAM_AXIS_CHANGE
//		sprintf(buf,"%5.3f (R: %5.3f , T: %5.3f)",m_nAngle,m_cAngle.x,m_cAngle.y);
////#else
///		sprintf(buf,"%5.3f (R: %5.3f , T: %5.3f)",m_nAngle,m_cAngle.y,m_cAngle.x);
////#endif
//		cDisp.DTextOutWithFontII(memDC,80,30,10,18,buf,RGB(0,255,0));	
//	}

	POINT tlpoint,brpoint;
	RECT nCircle;

    MinuteToPixel(-6.0,6.0,&tlpoint);
	MinuteToPixel(6.0,-6.0,&brpoint);

	nCircle.left   = tlpoint.x;
	nCircle.top    = tlpoint.y;
	nCircle.right  = brpoint.x;
	nCircle.bottom = brpoint.y;
    cDisp.DEllipse(memDC,nCircle,RGB(255,255,0));

    MinuteToPixel(-12.0,12.0,&tlpoint);
	MinuteToPixel(12.0,-12.0,&brpoint);

	nCircle.left   = tlpoint.x;
	nCircle.top    = tlpoint.y;
	nCircle.right  = brpoint.x;
	nCircle.bottom = brpoint.y;
    cDisp.DEllipse(memDC,nCircle,RGB(255,255,0));

    MinuteToPixel(-24.0,24.0,&tlpoint);
	MinuteToPixel(24.0,-24.0,&brpoint);

	nCircle.left   = tlpoint.x;
	nCircle.top    = tlpoint.y;
	nCircle.right  = brpoint.x;
	nCircle.bottom = brpoint.y;
    cDisp.DEllipse(memDC,nCircle,RGB(255,255,0));
 //////////////////////////////////////////////////////////////  
//	double delta=(m_cSettingDataAdd.nRErrorLimitHigh - m_cSettingData.nErrorLimit)/2;
	MinuteToPixel(m_cSettingData.nErrorLimit,m_cSettingDataAdd.nTErrorLimitLow,&tlpoint);
	MinuteToPixel(m_cSettingDataAdd.nRErrorLimitHigh,m_cSettingDataAdd.nTErrorLimitHigh,&brpoint);
//	MinuteToPixel(-m_cSettingData.nErrorLimit,m_cSettingData.nErrorLimit,&tlpoint);
//	MinuteToPixel(m_cSettingData.nErrorLimit,-m_cSettingData.nErrorLimit,&brpoint);
//	MinuteToPixel(-m_cSettingData.nErrorLimit+m_nROffset,m_cSettingData.nErrorLimit+m_nTOffset,&tlpoint);
//	MinuteToPixel(m_cSettingData.nErrorLimit+m_nROffset,-m_cSettingData.nErrorLimit+m_nTOffset,&brpoint);

	nCircle.left   = tlpoint.x;
	nCircle.top    = tlpoint.y;
	nCircle.right  = brpoint.x;
	nCircle.bottom = brpoint.y;
    cDisp.DEllipse(memDC,nCircle,RGB(255,0,0));

//////////////////////////////////////////////////////////////////// 
/* append

	POINT pt_cp;
	POINT f_pt,e_pt;
	MinuteToPixel(m_nROffset,m_nTOffset,&pt_cp);
	f_pt.x=pt_cp.x;
	f_pt.y=nCircle.top;
	e_pt.x=pt_cp.x;
	e_pt.y=nCircle.bottom;
	cDisp.DLine(memDC,f_pt,e_pt,RGB(255,255,255),1);
	f_pt.x=nCircle.left;
	f_pt.y=pt_cp.y;
	e_pt.x=nCircle.right;
	e_pt.y=pt_cp.y;
	cDisp.DLine(memDC,f_pt,e_pt,RGB(255,255,255),1);
*/
	////////////////////////////////////////////////////////////////////g_bLeftRight
	cDisp.DTextOutWithFontII(memDC,10,10,12,20,"2: LASER BEAM AXIS",RGB(255,255,0));
	if(g_bLeftRight) {//Right
	    cDisp.DTextOutWithFontII(memDC,480,10,12,20,"Right Table",RGB(255,255,255));
	}
	else { //Left
        cDisp.DTextOutWithFontII(memDC,480,10,12,20,"Left Table",RGB(255,255,255)); 
	}
	if(m_nOkNgStatus==VISION_NG)
	{
		cDisp.DTextOutWithFontII(memDC,480,40,24,48,"A_NG",RGB(255,0,0));
	}
	else if(m_nOkNgStatus==VISION_SIZENG)
	{
		cDisp.DTextOutWithFontII(memDC,480,40,24,48,"S_NG",RGB(255,0,0));
	}
	else
	{
		cDisp.DTextOutWithFontII(memDC,510,40,24,48,"OK",RGB(0,255,0));
	}

	if(m_pView->m_wTabWindow.m_pwEtc->GetWorkMode())
		cDisp.DTextOutWithFont(memDC,320,400,20,40,"MASTER MODE",RGB(255,255,0));

	return;
}


int CImgDisplay2::ProcessThreeBeam(EN_TEST_MODE iTestMode)
{

	int  nfgclr=255;
	int  nbkclr=0;
	long nBlobNum=0;
	int nSizeObj=0;
	int i;
	int maxsize=0;
	int nReturnValue=0;

#ifdef GAUSSIAN_FITTING_ENABLE
	int offset[2]={0,0};
	int nBWidth,nBHeight;
	POINT cpt;
	POINT outpt;
#endif
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	
	int nThreeBeamFlag=0;
	m_nFindFlag=0;
	nfgclr=255;
	nbkclr=0;

/////////////////////////////////////////////////
	KIL_IDI nMaskId;
	RBLOB_SET_STRUCT scSetBlob;
	RBLOB_RESULT_STRUCT *scResult;

	KRB_Alloc(&nMaskId);

	scSetBlob.nImgHeight=m_nHeight;
	scSetBlob.nImgWidth=m_nWidth;
	scSetBlob.nOffsetX=2;
	scSetBlob.nOffsetY=2;
	scSetBlob.nThresh=m_cSettingData.nDeckThr;
	scSetBlob.nSizeFilter=9;

	scSetBlob.r.left=10;
	scSetBlob.r.top=10;
	scSetBlob.r.right=m_nWidth-10;
	scSetBlob.r.bottom=m_nHeight-10;
	

    m_nAxisBeamSize = 0 ;

	KRB_Set(nMaskId,&scSetBlob);
	nBlobNum=KRB_Run(nMaskId,m_pSrc,1);
////////////////////////////////////
// object가 없을 경우 
////////////////////////////////////
	if(nBlobNum==0)
	{
		KRB_Free(nMaskId);
		m_pView->m_wTabWindow.m_pwCurrent->SetListData(0,0,"-,-,-");
		m_pView->m_wTabWindow.m_pwCurrent->SetListData(0,1,"-");
		gMotionResultData.nAngleOkNg = VISION_ERROR; //2이면 빔을 못찾은것.
		return 0;
	}

/////////////////////////////////////////////
// size filtering...by turn table image size
/////////////////////////////////////////////
	scResult=new RBLOB_RESULT_STRUCT [nBlobNum];
	
	
	for(i=0; i<nBlobNum; i++)
		KRB_Get(nMaskId,i+1,&scResult[i]);

	
	if(nBlobNum>1)
	{
		KRB_SortByArea(scResult,0,nBlobNum-1,FALSE);
		
	}

	if(nBlobNum>0)
	{
        m_nAxisBeamSize = scResult[0].nArea;           //Blob Size
	}
/////////////////////////////////////////////////
// 주빔의 중심을 구한다.
    
	if(nBlobNum >=3)
	{
		for(i=0; i<3; i++)
		{
			m_cCpt[i].x=scResult[i].nCx;
			m_cCpt[i].y=scResult[i].nCy;
			memcpy(&m_cBeamRect[i],&scResult[i].r,sizeof(RECT));
		}
		
		m_nFindFlag=1;
/////////////////////////////////////////////////////////////////////////
// gaussian function definition
#ifdef GAUSSIAN_FITTING_ENABLE
		cpt.x=(int)(m_cCpt[0].x+0.5);
		cpt.y=(int)(m_cCpt[0].y+0.5);

		ComputeGaussianCenter(m_cBeamRect[0],cpt,&outpt);
		nBWidth=(m_cBeamRect[0].right-m_cBeamRect[0].left+1)/2;
		nBHeight=(m_cBeamRect[0].bottom-m_cBeamRect[0].top+1)/2;
	
		offset[0]=nBWidth*0.5;
		offset[1]=nBHeight*0.5;

		if(outpt.x >=m_cCpt[0].x-offset[0] && outpt.x <=m_cCpt[0].x+offset[0] &&
		   outpt.y >=m_cCpt[0].y-offset[1] && outpt.y <=m_cCpt[0].y+offset[1])
		{
			m_cCpt[0].x=(double)outpt.x;
			m_cCpt[0].y=(double)outpt.y;
		}
#endif
/////////////////////////////////////////////////////////////////////////	
#ifdef __SAMSUNG_TSST_VERSION
		if(m_pView->GetCDStatus())
		{
			ComputeCDData();
		}
		else
		{
			ComputeDvdData();
		}
#else
        ComputeDvdData();
#endif	
		nReturnValue=1;
		nThreeBeamFlag=1;
	}
	else
	{
		
		if(nBlobNum>=1)
		{
			m_nFindFlag=1;



			m_cCpt[0].x=scResult[0].nCx;
			m_cCpt[0].y=scResult[0].nCy;
			
			memcpy(&m_cBeamRect[0],&scResult[0].r,sizeof(RECT));
			
/////////////////////////////////////////////////////////////////////////
// gaussian function definition


#ifdef GAUSSIAN_FITTING_ENABLE
			cpt.x=(int)(m_cCpt[0].x+0.5);
			cpt.y=(int)(m_cCpt[0].y+0.5);

			ComputeGaussianCenter(m_cBeamRect[0],cpt,&outpt);

			nBWidth=(m_cBeamRect[0].right-m_cBeamRect[0].left+1)/2;
			nBHeight=(m_cBeamRect[0].bottom-m_cBeamRect[0].top+1)/2;
			
			offset[0]=nBWidth*0.5;
			offset[1]=nBHeight*0.5;

			if(outpt.x >=m_cCpt[0].x-offset[0] && outpt.x <=m_cCpt[0].x+offset[0] &&
			   outpt.y >=m_cCpt[0].y-offset[1] && outpt.y <=m_cCpt[0].y+offset[1])
			{
				m_cCpt[0].x=(double)outpt.x;
				m_cCpt[0].y=(double)outpt.y;
			}
#endif
	/////////////////////////////////////////////////////////////////////////		
 
 #ifdef __SAMSUNG_TSST_VERSION
			if(m_pView->GetCDStatus())
			{
				ComputeCDData();
			}
			else
			{
				ComputeDvdData();
			}
#else
		    ComputeDvdData();
#endif	
//////////////////////////////////////////////
			nReturnValue=1;
		}
		else
		{
			nReturnValue=0;
		}
	}

	delete[] scResult;
	KRB_Free(nMaskId);

	if(nReturnValue==1)
	{
		if(m_cSettingData.nASt==1)
		{
			if(iTestMode==tmNormal) 
			{
				if( ((m_cAngle.x <m_cSettingData.nErrorLimit       ) || (m_cAngle.x >m_cSettingDataAdd.nRErrorLimitHigh)) ||
					((m_cAngle.y <m_cSettingDataAdd.nTErrorLimitLow) || (m_cAngle.y >m_cSettingDataAdd.nTErrorLimitHigh))){
					m_pView->m_wOkNgWindow.m_pButton[0].SetCheck(0);//NG
                    m_nAngleOKNG = VISION_NG;//0이면 OK ////20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다. 
				}

				else if(m_nAxisBeamSize < m_cSettingDataEx.iBeamSize || m_nAxisBeamSize > m_cSettingDataEx.iBeamSizeHigh) {
			        m_pView->m_wOkNgWindow.m_pButton[0].SetCheck(0);//NG
				    m_nAngleOKNG = VISION_SIZENG;//0이면 OK ////20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다. 
				}

				
				else {
					m_pView->m_wOkNgWindow.m_pButton[0].SetCheck(1);//OK
                    m_nAngleOKNG = VISION_OK;//0이면 OK ////20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다. 
				}
					
			}
			else 
			{
				if( ((m_cAngle.x <m_cSettingDataAdd.nRErrorLimitLow2) || (m_cAngle.x >m_cSettingDataAdd.nRErrorLimitHigh2)) ||
					((m_cAngle.y <m_cSettingDataAdd.nTErrorLimitLow2) || (m_cAngle.y >m_cSettingDataAdd.nTErrorLimitHigh2))){
					m_pView->m_wOkNgWindow.m_pButton[0].SetCheck(0);//NG
                    m_nAngleOKNG = VISION_NG;//0이면 OK ////20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다. 
				}

				else if(m_nAxisBeamSize < m_cSettingDataEx.iBeamSize || m_nAxisBeamSize > m_cSettingDataEx.iBeamSizeHigh) {
			        m_pView->m_wOkNgWindow.m_pButton[0].SetCheck(0);//NG
				    m_nAngleOKNG = VISION_SIZENG;//0이면 OK ////20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다.
				}

				else {
					m_pView->m_wOkNgWindow.m_pButton[0].SetCheck(1);//OK
                    m_nAngleOKNG = VISION_OK;//0이면 OK ////20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다. 
				}
			}
		}
		if(m_cSettingData.nTSt==1)
		{
//////////////////////////////////////////////
// TSST 요청에 따라서 변경 2009년 1월 13일 	
//			if(nThreeBeamFlag==1)
			{
				if(m_nTilt>(m_cSettingData.nTiltCenter+m_cSettingData.nTiltLow) && m_nTilt<(m_cSettingData.nTiltCenter+m_cSettingData.nTiltHigh))
				{
					m_pView->m_wOkNgWindow.m_pButton[5].SetCheck(1);
					m_nOkNgStatus= VISION_OK;//OK
				}
				else
				{
					m_pView->m_wOkNgWindow.m_pButton[5].SetCheck(0);
					m_nOkNgStatus= VISION_NG;//OK
				}
			}
//			else
//				m_pView->m_wOkNgWindow.m_pButton[5].SetCheck(0);

		}
	}
	else
	{
		if(m_cSettingData.nASt==1)	{m_pView->m_wOkNgWindow.m_pButton[0].SetCheck(0); m_nAngleOKNG = VISION_OK; }
		if(m_cSettingData.nTSt==1)	{m_pView->m_wOkNgWindow.m_pButton[5].SetCheck(0); m_nAngleOKNG = VISION_OK; }
	}
	
//////////////////////////////////////////////
//display data
    m_nOkNgStatus = m_nAngleOKNG ;

    gMotionResultData.dAngleX = m_cAngle.x - (m_cSettingDataAdd.nRErrorLimitHigh + m_cSettingData   .nErrorLimit    )/2.0 ;//결과값에 옾셑을 주는 경우를 감안한다.
    gMotionResultData.dAngleY = m_cAngle.y - (m_cSettingDataAdd.nTErrorLimitHigh + m_cSettingDataAdd.nTErrorLimitLow)/2.0 ;
    gMotionResultData.nAngleOkNg = m_nAngleOKNG;

    /*옵셑 적용 되서 보정 하기 위해 위로 대체.
	gMotionResultData.dAngleX = m_cAngle.x;
	gMotionResultData.dAngleY = m_cAngle.y;
	gMotionResultData.nAngleOkNg = m_nAngleOKNG;
	*/

	Invalidate(FALSE);
	return nReturnValue;
}

void CImgDisplay2::ComputeDvdData()
{
	char buf[256];
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

    memcpy(&m_cAngle,&m_cMinuteCoordinate[(int)m_cCpt[0].y][(int)m_cCpt[0].x],sizeof(usrPOINT));
	m_nAngle=sqrt(m_cAngle.x*m_cAngle.x+m_cAngle.y*m_cAngle.y);
	sprintf(buf,"%6.3f,%6.3f,%6.3f",m_nAngle,m_cAngle.x,m_cAngle.y);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(0,0,buf);
//		if(m_cCpt[1].x!=m_cCpt[2].x)
//		{
//			m_nTilt=atan(-(m_cCpt[2].y-m_cCpt[1].y)/(m_cCpt[2].x-m_cCpt[1].x))*180.0/3.14;
//		}
//		else
//			m_nTilt=90.0;
//////////////////////////////////////////////
// TSST 요청에 따라서 변경 2009년 1월 13일 	
	
	if(m_cAngle.x!=0.0)
	     m_nTilt=atan(m_cAngle.y/m_cAngle.x)*180.0/3.141592;
	else
		m_nTilt=90.0;

	if(m_cAngle.x<0.0 && m_cAngle.y>0.0) m_nTilt+=180.0;
	else
	{
		if(m_cAngle.x>0.0 && m_cAngle.y<0.0) m_nTilt+=360.0;
	}
//////////////////////////////////////////////
	sprintf(buf,"%6.3f",m_nTilt);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(0,1,buf);
}

void CImgDisplay2::ComputeCDData()
{
#ifdef __SAMSUNG_TSST_VERSION
	char buf[256];
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();


    memcpy(&m_cAngleCD,&m_cMinuteCoordinate[(int)m_cCpt[0].y][(int)m_cCpt[0].x],sizeof(usrPOINT));
	m_nAngleCD=sqrt(m_cAngleCD.x*m_cAngleCD.x+m_cAngleCD.y*m_cAngleCD.y);
	sprintf(buf,"%6.3f,%6.3f,%6.3f",m_nAngleCD,m_cAngleCD.x,m_cAngleCD.y);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(0,0,buf);
//		if(m_cCpt[1].x!=m_cCpt[2].x)
//		{
//			m_nTilt=atan(-(m_cCpt[2].y-m_cCpt[1].y)/(m_cCpt[2].x-m_cCpt[1].x))*180.0/3.14;
//		}
//		else
//			m_nTilt=90.0;
//////////////////////////////////////////////
// TSST 요청에 따라서 변경 2009년 1월 13일 	
	if(m_cAngleCD.x!=0.0)	
		m_nTiltCD=atan(m_cAngleCD.y/m_cAngleCD.x)*180.0/3.141592;
	else
		m_nTiltCD=90.0;

	if(m_cAngleCD.x<0.0 && m_cAngleCD.y>0.0) m_nTiltCD+=180.0;
	else
	{
		if(m_cAngleCD.x>0.0 &&m_cAngleCD.y<0.0) m_nTiltCD+=360.0;
	}
//////////////////////////////////////////////
	sprintf(buf,"%6.3f",m_nTiltCD);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(0,1,buf);
#endif
}


double CImgDisplay2::ComputeAngle()
{
#ifdef __SAMSUNG_TSST_VERSION
	double dAngle=0.0;
	usrPOINT pointDiff;

	pointDiff.x=m_cAngleCD.x - m_cAngle.x;
	pointDiff.y=m_cAngleCD.y - m_cAngle.y;

	if(pointDiff.x!=0.0)	
		dAngle=atan(pointDiff.y/pointDiff.x)*180.0/3.141592;
	else
		dAngle=90.0;

	if(pointDiff.x<0.0 && pointDiff.y>0.0) dAngle+=180.0;
	else
	{
		if(pointDiff.y<0 && pointDiff.x>0.0) dAngle+=360.0;
	}

	return dAngle;
#else
	return 0.0;
#endif
}

int CImgDisplay2::CheckErrorFlag() //여기부터 하기.....
{

    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	if(m_cSettingData.nASt==1)
	{
		if(m_pView->m_wOkNgWindow.m_pButton[0].GetCheck()==0)
		{
			return 0;
		}
	}

	if(m_cSettingData.nTSt==1)
	{
		if(m_pView->m_wOkNgWindow.m_pButton[5].GetCheck()==0)
		{
			return 0;
		}
	}
	return 1;
}

void CImgDisplay2::ComputeGaussianCenter(RECT r,POINT cp,POINT *out)
{
	int i;
//	int j;
	double nData[640];
	int count=0;
	int start,endd;
	int sx,sy,ex,ey;
//	int xx,yy;
	int sum=0;

	sx=r.left-20;
	sy=r.top-20;
	ex=r.right+20;
	ey=r.bottom+20;

	if(sx<0) sx=0;
	if(sy<0) sy=0;
	if(ex>639) ex=639;
	if(ey>479) ey=479;
/*
	for(j=3;j<=476;j++)
		for(i=3;i<=636; i++)
		{
			sum=0;
			for(xx=-2;xx<=2; xx++)
				for(yy=-2;yy<=-2;yy++)
				{
					sum+=*(m_pSrc+m_nWidth*(j+yy)+i+xx);
				}
				
			*(m_pOut+m_nWidth*j+i)=(unsigned char)(sum/25);
		}

*/

	start=0;
	endd=639;

//	if(start <0) start=0;
//	if(endd>639)  endd=639;
	

	for(i=start; i<endd; i++)
	{

		nData[i]=(double)(*(m_pSrc+m_nWidth*cp.y+i));
		count++;
	}
	m_nDCx=ComputeCenter(nData,count,r.left,r.right);
	out->x=(int)(m_nDCx);

/////////////////////////////////////////////////////////////
	start=0;
	endd =479;

//	if(start <0) start=0;
//	if(endd>479)  endd=479;

	count=0;
	for(i=start; i<endd; i++)
	{
		nData[i]=(double)(*(m_pSrc+m_nWidth*i+cp.x));
		count++;
	}
	m_nDCy=ComputeCenter(nData,count,r.top,r.bottom);
	out->y=(int)(m_nDCy);

	return;
}

double CImgDisplay2::LDGaussian(double s,double v,double m) 
{
	return m_nDPeak*exp(-(v-m)*(v-m)/(2*s*s));
}


double CImgDisplay2::ComputeCenter(double *nData,int count,int start,int end)
{
	double sum=0.0;
	int i;
	double max_intensity=0;
//	char buf[256];
//	double nMinValue;
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

///////////////////////////////////////////////////////////////////////////////////
// y=A*Exp(-x*x/(2*r*r)) => ln(y)=ln(A)+B*x*x, B=-1/(2*r*r), A=Exp(a), lny=a+b*x*x
///////////////////////////////////////////////////////////////////////////////////

	double x4y_sum=0.0,ylny_sum=0.0,x2y_sum=0.0,x2ylny_sum=0.0,y_sum=0.0;
    double a_value,b_value;
	double tmp_data;
	double tmp_x2;

	double lny_sum=0,x4_sum=0,x2_sum=0,x2lny_sum=0;
	double error_sum=0;
    double nn=0;
	double result[640];
    int j;
	double min_err,min_sigma,min_cp,min_peak;

	for(j=start-2;j<end-2;j++)
	{
		nn=0;
		lny_sum=0;
		x4_sum=0;
		x2_sum=0;
		x2lny_sum=0;
		error_sum=0;
		for(i=start; i<end ;i++)
		{
			tmp_data=nData[i];
			tmp_x2=((double)i-(double)j)*((double)i-(double)j);
			
			lny_sum+=log(tmp_data);
			x4_sum+=(tmp_x2*tmp_x2);
			x2_sum+=tmp_x2;
			x2lny_sum+=(tmp_x2*log(tmp_data));
			nn+=1.0;
		}
		a_value=(lny_sum*x4_sum - x2_sum*x2lny_sum)/(nn*x4_sum - x2_sum*x2_sum);
		b_value=(nn*x2lny_sum - x2_sum*lny_sum)/(nn*x4_sum - x2_sum*x2_sum);
	// end gaussian fitting.
	///////////////////////////////////////////////////////////////////////////////////////
		if(b_value>0) b_value*=-1;
		if(b_value==0) b_value=-10E-10;

		m_nSigma=sqrt(-1/(2*b_value));
		m_nDPeak=exp(a_value);
		
		for(i=start; i<end;i++)
		{
			result[i]=LDGaussian(m_nSigma,(double)i,(double)j);
			error_sum+=((nData[i]-result[i])*(nData[i]-result[i]));
		}
		if(j==start-2)
		{
			min_err=error_sum;
			min_sigma=m_nSigma;
			min_cp=j;
			min_peak=m_nDPeak;
		}
		else
		{
			if(error_sum<min_err)
			{
				min_err=error_sum;
				min_sigma=m_nSigma;
				min_cp=j;
				min_peak=m_nDPeak;
			}
		}
	}
	
	return min_cp;
}

void CImgDisplay2::SaveBmp(CBitmap *pBitmap,char *fname)
{
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFO BitmapInfo;
    BITMAP Bitmap;

	pBitmap->GetBitmap(&Bitmap);

	memcpy(&BitmapInfo.bmiHeader,&m_BitmapInfoHeader,sizeof(BITMAPINFOHEADER));
	BitmapInfo.bmiHeader.biBitCount=Bitmap.bmBitsPixel;
	BitmapInfo.bmiHeader.biSizeImage=(m_nWidth*(Bitmap.bmBitsPixel/8))*m_nHeight;
	bmpHeader.bfType=((WORD)('M'<<8)|'B');
	bmpHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+
		                sizeof(BITMAPINFOHEADER);
	bmpHeader.bfSize=sizeof(bmpHeader)-sizeof(RGBQUAD)+sizeof(BITMAPINFO)+BitmapInfo.bmiHeader.biSizeImage;
	bmpHeader.bfReserved1=0;
	bmpHeader.bfReserved2=0;
	DWORD size,len;
	HANDLE fd=CreateFile(fname,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	WriteFile(fd,(LPSTR)&bmpHeader,sizeof(BITMAPFILEHEADER),&len,NULL);
	size=sizeof(BITMAPINFO)-sizeof(RGBQUAD);
	WriteFile(fd,&BitmapInfo,size,&len,NULL);

	unsigned char *m_pDib=new unsigned char [BitmapInfo.bmiHeader.biSizeImage];
	unsigned char *m_pDibTmp=new unsigned char [BitmapInfo.bmiHeader.biSizeImage];
	pBitmap->GetBitmapBits(BitmapInfo.bmiHeader.biSizeImage,m_pDib);
	
	int start=m_nHeight-1;
	int tmpWidth=m_nWidth;
	int i;
	

	for(i=start; i>=0; i--)
		memcpy((m_pDibTmp+tmpWidth*(Bitmap.bmBitsPixel/8)*(start-i)),(m_pDib+m_nWidth*(Bitmap.bmBitsPixel/8)*i),m_nWidth*(Bitmap.bmBitsPixel/8));

	WriteFile(fd,m_pDibTmp,BitmapInfo.bmiHeader.biSizeImage,&len,NULL);
	CloseHandle(fd);
	delete[] m_pDib;
	delete[] m_pDibTmp;
}

void CImgDisplay2::RunSaveBmp(char *fname)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	
	int i;
	CDC *dc=GetDC();

	m_hOldPal=::SelectPalette(dc->GetSafeHdc(),m_hPal,FALSE);
	::RealizePalette(dc->GetSafeHdc());

	CDC m_MemDC;
	m_MemDC.CreateCompatibleDC(dc);

	int start=m_nHeight-1;
	int tmpWidth=m_nWidth+m_nDummy;
	

	for(i=start; i>=0; i--)
		memcpy((m_pBitmapBuffer+tmpWidth*(start-i)),(m_pSrc+m_nWidth*i),m_nWidth);
    
    
	CBitmap m_Bitmap,*m_OldBitmap;

	 m_Bitmap.CreateCompatibleBitmap(dc,tmpWidth,m_nHeight);
	 m_OldBitmap=m_MemDC.SelectObject(&m_Bitmap);

	::SetStretchBltMode(m_MemDC.GetSafeHdc(),COLORONCOLOR);
	::SetDIBitsToDevice(m_MemDC.GetSafeHdc(),0,0,tmpWidth,m_nHeight,
		                             0,0,0,m_nHeight,
									 m_pBitmapBuffer,
									 (LPBITMAPINFO)&m_BitmapInfoHeader,
									 DIB_RGB_COLORS);

////////////////////////
	if(m_nGridDisplayTag==1)
		DisplayGrid(&m_MemDC);
	dc->BitBlt( 0,0,m_nWidth,m_nHeight,&m_MemDC,0,0,SRCCOPY);

	SaveBmp(&m_Bitmap,fname);

	DeleteObject(m_Bitmap);
	ReleaseDC(&m_MemDC);
	ReleaseDC(dc);
}
