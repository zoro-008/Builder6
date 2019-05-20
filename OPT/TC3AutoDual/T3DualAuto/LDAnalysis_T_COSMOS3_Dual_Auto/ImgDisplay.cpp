// ImgDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "MainFrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "HVSystemDefine.h"
#include "ImgDisplay.h"
#include "External.h"
#include "..\\guisrc\\DisplayToGDI.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImgDisplay dialog
char *gITextM[2][4]={ {"-1.0","-2.0","-3.0","-4.0"},{"1.0","2.0","3.0","4.0"}};
char *gITextP[2][4]={ {"1.0","2.0","3.0","4.0"},{"-1.0","-2.0","-3.0","-4.0"}};

CImgDisplay::CImgDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CImgDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImgDisplay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nGridDisplayTag=0;
//	m_nGRoiTag=0;
	m_pLogPalette=NULL;
	m_pBitmapBuffer=NULL;
	m_pSrc=NULL;
	m_pOrigin=NULL;
	m_pOut=NULL;
	m_pTmp=NULL;
	m_nWidth=640;
	m_nHeight=480;
	m_nMouseDoubleClick=0;
	m_nFindFlag=0;
	m_nPFindFlag=0;
	m_nVValue=NULL;
	m_nHValue=NULL;
	m_nMouseDown=0;
	m_nRoiSet=0;
	m_cIPDist.x=0.0;
	m_cIPDist.y=0.0;
	m_nBeamPos.x=0.0;
	m_nBeamPos.y=0.0;
	m_nPBeamPos.x=0.0;
	m_nPBeamPos.y=0.0;
	m_nSaveBmp=0;
	m_dDistance=0.0;

	m_nPBeamArea=0.0;
    m_nBeamDiameter=0.0;
	m_nBeamArea=0.0;
	m_nPBeamDiameter=0.0;
	m_nPAreaRatio=0.0;
	m_nParallelism=0.0;
	m_nOlRatio=0.0;
	m_nOlRadius=0.0;
	m_nBeamRadius=0.0;
	m_nOkNgStatus=1;
	m_nHMaxValue=0;

	

}


void CImgDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImgDisplay)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImgDisplay, CDialog)
	//{{AFX_MSG_MAP(CImgDisplay)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgDisplay message handlers
BOOL CImgDisplay::PreTranslateMessage(MSG* pMsg) 
{
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

void CImgDisplay::BitmapInit()
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
	m_hPal=::CreatePalette(m_pLogPalette);
	m_pBitmapBuffer=new unsigned char [(m_nWidth+m_nDummy)*m_nHeight];
	memset(m_pBitmapBuffer,0x00,(m_nWidth+m_nDummy)*m_nHeight);
}

void CImgDisplay::ChangePallete()
{
	int i;
	int rr,gg,bb;
	if(m_pLogPalette!=NULL)
	{
		::DeleteObject(m_hPal);
		delete[] m_pLogPalette;
		m_pLogPalette=NULL;
	}

	if(m_cSettingData.nBDispMode==1)
	{
		m_pLogPalette=(LOGPALETTE*)new WORD [sizeof(WORD)+sizeof(PALETTEENTRY)*256];
		m_pLogPalette->palNumEntries=(WORD)256;
		m_pLogPalette->palVersion   =(WORD)0x300;

		for(i=0; i<256; i++)
		{
			rr=-(i-255);
			if(i<128) gg=2*i ;
			else      gg=-2*(i-255);
			bb=i;


			m_pLogPalette->palPalEntry[i].peRed  =m_aBitmapPalette[i].peRed  =(BYTE)rr;
			m_pLogPalette->palPalEntry[i].peGreen=m_aBitmapPalette[i].peGreen=(BYTE)gg;
			m_pLogPalette->palPalEntry[i].peBlue =m_aBitmapPalette[i].peBlue =(BYTE)bb;
			m_pLogPalette->palPalEntry[i].peFlags=m_aBitmapPalette[i].peFlags=(BYTE)0;
		}
		
		m_hPal=::CreatePalette(m_pLogPalette);
	}
	else
	{
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
		m_hPal=::CreatePalette(m_pLogPalette);
	}
	Invalidate(FALSE);
}

void CImgDisplay::ReSetBitmapInfo(int nWidth,int nHeight)
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

void CImgDisplay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	EnterCriticalSection(&m_pImage1);
	Draw256Image(&dc,m_pOrigin);
	LeaveCriticalSection(&m_pImage1);
	return;
}

void CImgDisplay::SetBuffer(unsigned char *pImg)
{
	EnterCriticalSection(&m_pImage1);
	memcpy(m_pSrc,pImg,m_nWidth*m_nHeight);
	KIP_Smoothing(pImg,m_pOrigin,m_nWidth,m_nHeight,3);
	LeaveCriticalSection(&m_pImage1);
	Invalidate(FALSE);
}

void CImgDisplay::ImgUpdate(unsigned char *pImg)
{

	memcpy(m_pOrigin,pImg,m_nWidth*m_nHeight);
	Invalidate(FALSE);
}

void CImgDisplay::Draw256Image(CPaintDC *dc,unsigned char *pImg)
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
	DrawPallete(&m_MemDC);
	dc->BitBlt( 0,0,m_nWidth,m_nHeight,&m_MemDC,0,0,SRCCOPY);
	
	DeleteObject(m_Bitmap);
	ReleaseDC(&m_MemDC);
}

void CImgDisplay::Draw256Image(CDC *dc,unsigned char *pImg)
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
	DrawPallete(&m_MemDC);
	dc->BitBlt( 0,0,m_nWidth,m_nHeight,&m_MemDC,0,0,SRCCOPY);

	DeleteObject(m_Bitmap);
	ReleaseDC(&m_MemDC);
}



BOOL CImgDisplay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	BitmapInit();

	m_pSrc=new unsigned char [m_nWidth*m_nHeight];
	memset(m_pSrc,0x00,m_nWidth*m_nHeight);
	m_pOrigin= new unsigned char [m_nWidth*m_nHeight];
	memset(m_pOrigin,0x00,m_nWidth*m_nHeight);
	m_pOut=new unsigned char [m_nWidth*m_nHeight];
	memset(m_pOut,0x00,m_nWidth*m_nHeight);
	m_pTmp=new unsigned char [m_nWidth*m_nHeight];
	memset(m_pTmp,0x00,m_nWidth*m_nHeight);

	m_nVValue=new unsigned char [m_nHeight];
	m_nHValue= new unsigned char [m_nWidth];
	m_nCx=(double)m_nWidth/2.0;
	m_nCy=(double)m_nHeight/2.0;

	InitializeCriticalSection(&m_pImage1);

	return TRUE;  
}

void CImgDisplay::OnDestroy() 
{
	CDialog::OnDestroy();
	::DeleteObject(m_hPal);

	DeleteCriticalSection(&m_pImage1);
	
	if(m_pLogPalette!=NULL) delete[] m_pLogPalette;
	if(m_pBitmapBuffer!=NULL) delete[] m_pBitmapBuffer;
	if(m_pSrc!=NULL) delete[] m_pSrc;
	if(m_pOrigin!=NULL) delete[] m_pOrigin;
	if(m_pOut!=NULL) delete[] m_pOut;
	if(m_pTmp!=NULL) delete[] m_pTmp;
	if(m_nVValue!=NULL) delete[] m_nVValue;
	if(m_nHValue!=NULL) delete[] m_nHValue;
}

void CImgDisplay::SettingParam()
{
	double factor;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	

	memcpy(&m_cSettingData,&m_pView->m_cSettingData,sizeof(SETTING_DATA));
    if(m_cSettingData.nACenterFlag==1)
	{
		m_nCx=(double)m_cSettingData.nACX;
		m_nCy=(double)m_cSettingData.nACY;
	}
	else
	{
		m_nCx=(double)m_nWidth/2.0;
		m_nCy=(double)m_nHeight/2.0;
	}

	ChangePallete();
	MakeCoordTable();

	factor=((double)4.0/m_nScaleFactor);
	m_cHsp.x=(m_nCx-factor);
	m_cHsp.y=m_nCy;
	m_cVsp.y=(m_nCy-factor);
	m_cVsp.x=m_nCx;

	m_cHep.x=(m_nCx+factor);
	m_cHep.y=m_nCy;
	m_cVep.y=(m_nCy+factor);
	m_cVep.x=m_nCx;
    
	m_nOlRatio=m_cSettingData.nOlSize/m_cSettingData.nBSize;

	MakeGridCoord();
	SelectDisplayType();
	
    m_pView->m_wDisplayComm.AddListData("parameter uploading success");
	return;
}


int CImgDisplay::DistanceToPixelPosX(double nDist)
{
	int nValue;

	if(m_cSettingData.nRDir ==0 )	nValue=(int)(m_nCx+nDist/m_nScaleFactor);
	else							nValue=(int)(m_nCx-nDist/m_nScaleFactor);

	return nValue;
}

int CImgDisplay::DistanceToPixelPosY(double nDist)
{
	int nValue;
	
	if(m_cSettingData.nTDir ==0 )	nValue=(int)(m_nCy-nDist/m_nScaleFactor);
	else							nValue=(int)(m_nCy+nDist/m_nScaleFactor);

	return nValue;
}


void CImgDisplay::MakeCoordTable()
{
	int i,j;
	// X -> R, Y ->T
//	m_nScaleFactor=(double)m_cSettingData.nBSize/(double)m_cSettingData.nBPixel;
#ifndef USE_CAMERA_1
	m_nScaleFactor=((double)8.91/(double)640.0)+(double)m_cSettingData.nBPixel/10000.0;
#else
	m_nScaleFactor=((double)4.8/(double)640.0)+(double)m_cSettingData.nBPixel/10000.0;
#endif


	if(m_cSettingData.nRDir ==0 && m_cSettingData.nTDir==0)
	{
		for(j=0; j<m_nHeight; j++)
			for(i=0; i<m_nWidth; i++)
			{
				m_cDistanceCoordinate[j][i].x=((double)i - m_nCx) * m_nScaleFactor;
				m_cDistanceCoordinate[j][i].y=(m_nCy-(double)j) * m_nScaleFactor;
			}
		return;
	}

	if(m_cSettingData.nRDir ==1 && m_cSettingData.nTDir==0)
	{
		for(j=0; j<m_nHeight; j++)    //t
			for(i=0; i<m_nWidth; i++) //r
			{
				m_cDistanceCoordinate[j][i].x=(double)(m_nCx - (double)i) * m_nScaleFactor;
				m_cDistanceCoordinate[j][i].y=(double)(m_nCy-(double)j) * m_nScaleFactor;
			}
		return;
	}

	if(m_cSettingData.nRDir ==0 && m_cSettingData.nTDir==1)
	{
		for(j=0; j<m_nHeight; j++) //t
			for(i=0; i<m_nWidth; i++) //r
			{
				m_cDistanceCoordinate[j][i].x=((double)i - m_nCx) * m_nScaleFactor;
				m_cDistanceCoordinate[j][i].y=((double)j - m_nCy) * m_nScaleFactor;
			}

		return;
	}

	if(m_cSettingData.nRDir ==1 && m_cSettingData.nTDir==1)
	{
		for(j=0; j<m_nHeight; j++)
			for(i=0; i<m_nWidth; i++)
			{
				m_cDistanceCoordinate[j][i].x=(m_nCx - (double)i) * m_nScaleFactor;
				m_cDistanceCoordinate[j][i].y=((double)j-m_nCy) * m_nScaleFactor;
			}
	}
	return;
}

void CImgDisplay::MakeGridCoord()
{
	double factor;
    usrPOINT tsp,tep;
	usrPOINT tspt,tept;
	int offset=2;
    int k,kk;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	

	factor=0.1/m_nScaleFactor;

	tsp.y=m_nCy-offset;
	tep.y=m_nCy+offset+1;
	double nnx=m_nCx;

	k=0; kk=0;
	//+ direction
	while(k<=MAX_GRID_NUM)
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
	while(k<=MAX_GRID_NUM)
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
	while(k<=MAX_GRID_NUM)
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
	while(k<=MAX_GRID_NUM)
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

void CImgDisplay::DrawPallete(CDC *pDC)
{
   CDisplayToGDI cDisp;
   int i;
   POINT p1, p2;


   for(i=0; i<256; i++)
   {
	   p1.x=10;
	   p1.y=456-i;

	   p2.x=60;
	   p2.y=456-i;
	   cDisp.DLine(pDC,p1,p2,RGB(m_pLogPalette->palPalEntry[i].peBlue,
		                         m_pLogPalette->palPalEntry[i].peGreen,
								 m_pLogPalette->palPalEntry[i].peRed));
   }
   RECT r;
   r.left=10;
   r.right=60;
   r.top=200;
   r.bottom=456;

   cDisp.DRect(pDC,r,RGB(0,0,0));
}

void CImgDisplay::SelectDisplayType()
{
    SetGridDisplayTag(1);
	Invalidate(FALSE);
}

void CImgDisplay::DisplayGrid(CDC *memDC)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	

	register int i;
	POINT zeropt;
	int nSize;
    CDisplayToGDI cDisp;
	RECT r;
	POINT cAbCenter;
	char buf[256];
    
//	usrPOINT tpt1,tpt2;

	zeropt.x=m_nWidth/2;
	zeropt.y=m_nHeight/2;
	
	memDC->SetBkMode(TRANSPARENT);
	if(m_cSettingData.nDispType!=2)
	{
		cDisp.DLine(memDC,m_cHsp,m_cHep,RGB(0,255,0));
		cDisp.DLine(memDC,m_cVsp,m_cVep,RGB(0,255,0));

		for(i=0; i<MAX_GRID_NUM; i++)
		{
			cDisp.DLine(memDC,m_cSHp[i].sp,m_cSHp[i].ep,RGB(0,255,0));		
			cDisp.DLine(memDC,m_cSHm[i].sp,m_cSHm[i].ep,RGB(0,255,0));		
			cDisp.DLine(memDC,m_cSVp[i].sp,m_cSVp[i].ep,RGB(0,255,0));		
			cDisp.DLine(memDC,m_cSVm[i].sp,m_cSVm[i].ep,RGB(0,255,0));		
		}

		for(i=0; i<4; i++)
		{
			if(m_cSettingData.nDispType==0)
			{
				cDisp.DLine(memDC,m_cLHp[i].sp,m_cLHp[i].ep,RGB(255,255,0));
				cDisp.DLine(memDC,m_cLHm[i].sp,m_cLHm[i].ep,RGB(255,255,0));		
				cDisp.DLine(memDC,m_cLVp[i].sp,m_cLVp[i].ep,RGB(255,255,0));		
				cDisp.DLine(memDC,m_cLVm[i].sp,m_cLVm[i].ep,RGB(255,255,0));		
			}
			else
			{
				cDisp.DDotLine(memDC,m_cLHpG[i].sp,m_cLHpG[i].ep,RGB(255,128,32));
				cDisp.DDotLine(memDC,m_cLHmG[i].sp,m_cLHmG[i].ep,RGB(255,128,32));		
				cDisp.DDotLine(memDC,m_cLVpG[i].sp,m_cLVpG[i].ep,RGB(255,128,32));		
				cDisp.DDotLine(memDC,m_cLVmG[i].sp,m_cLVmG[i].ep,RGB(255,128,32));		
			}
			if((i%2)==0)
			{
				cDisp.DTextOut(memDC,(int)m_cLHp[i].sp.x,(int)m_cLHp[i].ep.y+5,gITextP[0][i],RGB(0,255,0));
				cDisp.DTextOut(memDC,(int)m_cLHm[i].sp.x,(int)m_cLHm[i].ep.y+5,gITextM[0][i],RGB(0,255,0));

				if(i==3)
				{
					cDisp.DTextOut(memDC,(int)m_cLVp[i].ep.x+16,(int)m_cLVp[i].sp.y+16,gITextP[0][i],RGB(0,255,0));
					cDisp.DTextOut(memDC,(int)m_cLVm[i].ep.x+16,(int)m_cLVm[i].sp.y-16,gITextM[0][i],RGB(0,255,0));	
				}
				else
				{
					// 정상적 vertical 상수 디스플레이 
					cDisp.DTextOut(memDC,(int)m_cLVp[i].ep.x+16,(int)m_cLVp[i].sp.y-4,gITextP[0][i],RGB(0,255,0));
					cDisp.DTextOut(memDC,(int)m_cLVm[i].ep.x+16,(int)m_cLVm[i].sp.y-4,gITextM[0][i],RGB(0,255,0));	
				}
			}
		
		}
	}
    nSize=DistanceToPixel(m_cSettingData.nDiameterCenter/2.0);
	r.left=(int)m_nCx -nSize;
	r.right=(int)m_nCx+nSize;
	r.top=(int)m_nCy-nSize;
	r.bottom=(int)m_nCy+nSize;
	cAbCenter.x=(int)m_nCx;
	cAbCenter.y=(int)m_nCy;

	cDisp.DEllipse(memDC,r,RGB(64,64,64));
	cDisp.DCrossPointWithEllipse(memDC,cAbCenter,10,RGB(255,0,0),RGB(0,0,0));


	if(m_nFindFlag==1)
	{
		
		cDisp.DEllipse(memDC,m_cBeamRect,RGB(0,255,0));
		cDisp.DEllipse(memDC,m_cOlRect,RGB(255,0,255));
		cDisp.DCrossPointWithFillEllipse(memDC,m_cCpt,0,RGB(0,255,0),RGB(0,255,0));
	}
	if(m_nPFindFlag==1)
	{
		cDisp.DCrossPointWithFillEllipse(memDC,m_cPCpt,0,RGB(255,0,0),RGB(255,0,0));
		cDisp.DLine(memDC,m_cCpt,m_cPCpt,RGB(255,0,0));
	}
	cDisp.DTextOutWithFontII(memDC,10,10,12,20,"1: LASER BEAM DISTRIBUTION",RGB(255,255,0));
	if(m_pView->m_wTabWindow.GetSetupActive())
	{ // 작업자는 수치를 볼수 없게 한다. 
		sprintf(buf,"Beam Center (%6.3f, %6.3f)",m_nBeamPos.x,m_nBeamPos.y);
		cDisp.DTextOutWithFontII(memDC,10,40,10,18,buf,RGB(0,255,0));
		sprintf(buf,"Peak Beam Center (%6.3f, %6.3f)",m_nPBeamPos.x,m_nPBeamPos.y);
		cDisp.DTextOutWithFontII(memDC,10,60,10,18,buf,RGB(255,128,64));
	}
	sprintf(buf,"BS-R : %6.3f mm",m_cIPDist.x);   //혹시 나중에 수치 보낼일 일ㅆ을때.
	cDisp.DTextOutWithFontII(memDC,64,400,14,28,buf,RGB(255,255,255));
	sprintf(buf,"BS-T : %6.3f mm",m_cIPDist.y);
	cDisp.DTextOutWithFontII(memDC,64,430,14,28,buf,RGB(255,255,255));


    if(g_bLeftRight) {//Right
	    cDisp.DTextOutWithFontII(memDC,480,5,12,20,"Right Table",RGB(255,255,255));
	}
	else { //Left
        cDisp.DTextOutWithFontII(memDC,480,5,12,20,"Left Table",RGB(255,255,255)); 
	}

	//20110124 lwb OK/NG 부분은 표시 안함. 다시함.
	if(g_iTestMode == tmNormal)
	{
		if(m_nOkNgStatus==1) cDisp.DTextOutWithFontII(memDC,510,40,30,60,"NG",RGB(255,0,0));
		else                 cDisp.DTextOutWithFontII(memDC,510,40,30,60,"OK",RGB(0,255,0));
	}
	//이부분도 표시안함.
//	if((m_nHMaxValue >=140 && m_nHMaxValue <=230) &&(m_nVMaxValue >=140 && m_nVMaxValue <=230))
//		cDisp.DTextOutWithFontII(memDC,410,440,12,24,"Beam Status : OK",RGB(0,255,0));
//	else
//		cDisp.DTextOutWithFontII(memDC,410,440,12,24,"Beam Status : NG",RGB(255,0,0));

	if(m_nRoiSet==1)
		m_cROI.DrawRoiEx(memDC);

	return;
}




int CImgDisplay::BeamDistributionAnalysis(int pflag)
{
	int nFlag;
//	int i;
//	double nMinValue;
	char buf[256];
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

//	m_nOkNgStatus=1;

	if(m_nRoiSet==0)		nFlag=ProcessBeamDistribution(pflag);
	else                    nFlag=ProcessManualBeamDistribution(pflag);

	if(nFlag==1)
	{
		nFlag=GetPeakBeam();
//nFlag=1;
//memcpy(&m_cPCpt,&m_cCpt,sizeof(POINT));
//m_nPFindFlag=1;
		if(nFlag==1)
		{
//			for(i=0; i<7; i++)
			{
				SetHValue(m_cCpt);
				SetVValue(m_cCpt);
				m_cPCpt.x=m_pView->m_wHProj.GetCenter();
				m_cPCpt.y=m_pView->m_wVProj.GetCenter();
			}


			memcpy(&m_nPBeamPos,&m_cDistanceCoordinate[m_cPCpt.y][m_cPCpt.x],sizeof(usrPOINT));	

			sprintf(buf,"%6.3f,%6.3f",m_nPBeamPos.x,m_nPBeamPos.y);
			m_pView->m_wTabWindow.m_pwCurrent->SetListData(2,2,buf);

			m_cIPDist.x=(m_nPBeamPos.x-m_nBeamPos.x);
			m_cIPDist.y=(m_nPBeamPos.y-m_nBeamPos.y);

			m_dDistance=sqrt(m_cIPDist.x*m_cIPDist.x+m_cIPDist.y*m_cIPDist.y);


//20101216 lwb Debug모드와 Release 모드가 같게 하기 위해 주석 처리
// #ifdef _DEBUG
// 			sprintf(buf,"1step : %f",m_cIPDist.x);
// 			m_pView->m_wDisplayComm.AddListData(buf);
// #endif

			sprintf(buf,"%6.3f,%6.3f,%6.3f",
					sqrt((m_nBeamPos.x-m_nPBeamPos.x)*(m_nBeamPos.x-m_nPBeamPos.x)+
						 (m_nBeamPos.y-m_nPBeamPos.y)*(m_nBeamPos.y-m_nPBeamPos.y)),m_cIPDist.x,m_cIPDist.y);
			m_pView->m_wTabWindow.m_pwCurrent->SetListData(2,3,buf);


			if(m_cSettingData.nHRSt==1)
			{
				if(m_pView->m_wHProj.m_nHRimValue > m_cSettingData.nRimHP)
				{
					
					m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(1);
				}
				else 
				{
					m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(0);
				}
			}

			if(m_cSettingData.nVRSt==1)
			{
				if(m_pView->m_wVProj.m_nVRimValue  > m_cSettingData.nRimVP)
				{
					m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(1);
				}
				else 
				{
					m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(0);
//					nCheck=1;
				}
			}

/****************************/
			if(m_cSettingData.nPSt==1)
			{
				if(m_nParallelism >m_cSettingData.nParallel)
				{
					m_pView->m_wOkNgWindow.m_pButton[2].SetCheck(1);

				}
				else
				{
					m_pView->m_wOkNgWindow.m_pButton[2].SetCheck(0);
				}

			}
			if(m_cSettingData.nHSt==1)
			{
	/////////////////////////////////////////////////////////
	// DW08/DW12, WALZ1 일경우만 적용한다. 
	/////////////////////////////////////////////////////////
	//			m_pView->m_wDisplayComm.AddListData("Found!");

//20101216 lwb Debug모드와 Release 모드가 같게 하기 위해 주석 처리
// #ifdef _DEBUG
// 				char buff[256];
// 				sprintf(buff,"%f",m_cIPDist.x);
// 				m_pView->m_wDisplayComm.AddListData(buff);
// #endif

				if((fabs(m_cIPDist.x) <=m_cSettingData.nPDist) && (fabs(m_cIPDist.y) <=m_cSettingData.nPDist2))
					m_pView->m_wOkNgWindow.m_pButton[6].SetCheck(1);
				else
				{
					m_pView->m_wOkNgWindow.m_pButton[6].SetCheck(0);
				}
			}

/****************************/
		}
		else
		{
			if(m_cSettingData.nHRSt==1)	{ m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(0);	}
			if(m_cSettingData.nVRSt==1)	{ m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(0);	}
			if(m_cSettingData.nPSt==1)  { m_pView->m_wOkNgWindow.m_pButton[2].SetCheck(0);  }
			if(m_cSettingData.nHSt==1)  { m_pView->m_wOkNgWindow.m_pButton[6].SetCheck(0);  }
			m_nHMaxValue=0;
		}
	}
	else
	{
		if(m_cSettingData.nHRSt==1)	{ m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(0);	}
		if(m_cSettingData.nVRSt==1)	{ m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(0);	}
		if(m_cSettingData.nPSt==1) { m_pView->m_wOkNgWindow.m_pButton[2].SetCheck(0);  }
		if(m_cSettingData.nHSt==1) { m_pView->m_wOkNgWindow.m_pButton[6].SetCheck(0);  }
		m_nHMaxValue=0;
	}	
	if(CheckErrorFlag()) m_nOkNgStatus=0; //OK
	else                 m_nOkNgStatus=1; //NG 

    gMotionResultData.nBeamOkNg = m_nOkNgStatus ;

	gMotionResultData.dBeam_Shift_R = m_cIPDist.x;
	gMotionResultData.dBeam_Shift_T = m_cIPDist.y;

	Invalidate(FALSE);
	return nFlag;
}

int CImgDisplay::CheckErrorFlag()
{

/*
	int     nASt;
	int     nLSt;
	int     nPSt;
	int     nHRSt; //h rim
	int     nDSt;
	int     nTSt;
	int     nHSt;
	int     nVRSt; //v rim
*/
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	if(m_cSettingData.nLSt==1)
	{
		if(m_pView->m_wOkNgWindow.m_pButton[1].GetCheck()==0)
		{
			return 0;
		}
	}

	if(m_cSettingData.nPSt==1)
	{
		if(m_pView->m_wOkNgWindow.m_pButton[2].GetCheck()==0)
		{
			return 0;
		}
	}
	if(m_cSettingData.nHRSt==1)
	{
		if(m_pView->m_wOkNgWindow.m_pButton[3].GetCheck()==0)
		{
			return 0;
		}
	}
	if(m_cSettingData.nVRSt==1)
	{
		if(m_pView->m_wOkNgWindow.m_pButton[3].GetCheck()==0)
		{
			return 0;
		}
	}

	if(m_cSettingData.nDSt==1)
	{
		if(m_pView->m_wOkNgWindow.m_pButton[4].GetCheck()==0)
		{
			return 0;
		}
	}

	if(m_cSettingData.nHSt==1)
	{
		if(m_pView->m_wOkNgWindow.m_pButton[6].GetCheck()==0)
		{
			return 0;
		}
	}

	return 1;
}

int CImgDisplay::ProcessBeamDistribution(int flag)
{

	int  nfgclr=255;
	int  nbkclr=0;
	long nBlobNum=0;
	int nSizeObj=0;
//	int nCheck=0;
	int i;
//	int j;
//////////////////////
//	int maxy;
//	int maxv;
//	int startx,endx;
//////////////////////
	int maxsize=0;
	int nReturnValue=0;
	POINT cpt;
	char buf[256];
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	
    POINT *pts;
    double tcx,tcy,tradius;
//	double sigma;
//	int bWidth;


//	m_nFindFlag=0;
	nfgclr=255;
	nbkclr=0;
	
	RECT r;
	r.left=10;
	r.right=m_nWidth-10;
	r.top=10;
	r.bottom=m_nHeight-10;
    int nRefSize=DistanceToPixel(m_cSettingData.nDiameterCenter/2.0);
//	double radius;
//	double cxx,cyy;

//	KIP_Smoothing(m_pOrigin,m_pOut,m_nWidth,m_nHeight,3);
//	memcpy(m_pOrigin,m_pOut,m_nWidth*m_nHeight);
//	if(flag==1)
//		memcpy(m_pSrc,m_pOut,m_nWidth*m_nHeight);

/////////////////////////////////////////////////
	KIL_IDI nMaskId;
	RBLOB_SET_STRUCT scSetBlob;
	RBLOB_RESULT_STRUCT *scResult;

	KRB_Alloc(&nMaskId);

	scSetBlob.nImgHeight=m_nHeight;
	scSetBlob.nImgWidth=m_nWidth;
	scSetBlob.nOffsetX=5;
	scSetBlob.nOffsetY=5;
	scSetBlob.nThresh=m_cSettingData.nBThr;
	scSetBlob.nSizeFilter=20000;

	scSetBlob.r.left=10;
	scSetBlob.r.top=10;
	scSetBlob.r.right=m_nWidth-10;
	scSetBlob.r.bottom=m_nHeight-10;
	

	KRB_Set(nMaskId,&scSetBlob);
	nBlobNum=KRB_Run(nMaskId,m_pOrigin,1);
////////////////////////////////////
// object가 없을 경우 
////////////////////////////////////
	if(nBlobNum==0)
	{
		KRB_Free(nMaskId);
		nReturnValue=0;;
		m_nFindFlag=0;
	}
	else
	{
		scResult=new RBLOB_RESULT_STRUCT [nBlobNum];

		for(i=0; i<nBlobNum; i++)
			KRB_Get(nMaskId,i+1,&scResult[i]);

		if(nBlobNum>1)
		{
			KRB_SortByArea(scResult,0,nBlobNum-1,FALSE);
		}
////////////////////////
        memset(m_pOut,0x00,m_nWidth*m_nHeight);
		pts=new POINT [scResult[0].nArea];
		KRB_GetPoint(nMaskId,scResult[0].nIdx,pts);
		for(i=0; i<scResult[0].nArea;i++)
			*(m_pOut+m_nWidth*pts[i].y+pts[i].x)=255;
//		bWidth=(scResult[0].r.right -scResult[0].r.left)/2;		
    	if(KFT_CircleFittingByLS(scResult[0].r,m_pOut,m_nWidth,&tcx,&tcy,&tradius,0)==0)
		{
			delete[] scResult;
			KRB_Free(nMaskId);
			m_nFindFlag=0;	
			return 0;
		}

//		KFT_CirleFittingByLs_Edge(scResult[0].r,m_pOrigin,m_nWidth,&tcx,&tcy,&tradius,12,0);
		
        scResult[0].r.left=  (int)(tcx-tradius+0.5);
		scResult[0].r.top=   (int)(tcy-tradius+0.5);
		scResult[0].r.right= (int)(tcx+tradius+0.5)-1;
		scResult[0].r.bottom=(int)(tcy+tradius+0.5)-1;
	
		delete[] pts;
////////////////////////
		if(scResult[0].r.right >=m_nWidth-1) scResult[0].r.right=m_nWidth-1;
		if(scResult[0].r.left <=1)           scResult[0].r.left=1;
		if(scResult[0].r.top <=1)            scResult[0].r.top=1;
		if(scResult[0].r.bottom >=m_nHeight-1)  scResult[0].r.bottom=m_nHeight-1;


		cpt.x=(int)(tcx+0.5);
		cpt.y=(int)(tcy+0.5);
        if(cpt.x <1) cpt.x=1;
		if(cpt.x > m_nWidth-2) cpt.x=m_nWidth-2;
		if(cpt.y <1) cpt.y=1;
		if(cpt.y >m_nHeight-2) cpt.y=m_nHeight-2;

		int nWidth;
		nWidth=scResult[0].r.right - scResult[0].r.left+1;
		m_nOlRadius=(m_nOlRatio*(double)nWidth/2.0);
		m_nBeamRadius=(double)nWidth/2.0;

		m_cOlRect.left =cpt.x - m_nOlRadius;
		m_cOlRect.right=cpt.x +m_nOlRadius;
		m_cOlRect.top =cpt.y -m_nOlRadius;
		m_cOlRect.bottom=cpt.y +m_nOlRadius;

		if(m_cOlRect.left <1) m_cOlRect.left=1;
		if(m_cOlRect.right>m_nWidth-2) m_cOlRect.right=m_nWidth-2;
		if(m_cOlRect.top <1) m_cOlRect.top=1;
		if(m_cOlRect.bottom>m_nHeight-2) m_cOlRect.bottom=m_nHeight-2;


		memcpy(&m_cBeamRect,&scResult[0].r,sizeof(RECT));
  		memcpy(&m_cCpt,&cpt,sizeof(POINT));

		memcpy(&m_nBeamPos,&m_cDistanceCoordinate[cpt.y][cpt.x],sizeof(usrPOINT));	
		
		m_nBeamArea=(double)scResult[0].nArea*m_nScaleFactor*m_nScaleFactor;
		m_nBeamDiameter=(double)(scResult[0].r.right - scResult[0].r.left)*m_nScaleFactor;

		sprintf(buf,"%6.3f",m_nBeamDiameter);
		m_pView->m_wTabWindow.m_pwCurrent->SetListData(0,2,buf);
		sprintf(buf,"%6.3f",m_nBeamArea);
		m_pView->m_wTabWindow.m_pwCurrent->SetListData(1,0,buf);

		sprintf(buf,"%6.3f,%6.3f",m_cDistanceCoordinate[(int)m_nCy][(int)m_nCx].x,m_cDistanceCoordinate[(int)m_nCy][(int)m_nCx].y);
		m_pView->m_wTabWindow.m_pwCurrent->SetListData(2,0,buf);

		sprintf(buf,"%6.3f,%6.3f",m_nBeamPos.x,m_nBeamPos.y);
		m_pView->m_wTabWindow.m_pwCurrent->SetListData(2,1,buf);

		delete[] scResult;
		KRB_Free(nMaskId);
		m_nFindFlag=1;	
		nReturnValue=1;
	}

	double nDistance;
	usrPOINT acpt;
	memcpy(&acpt,&m_cDistanceCoordinate[(int)m_nCy][(int)m_nCx],sizeof(usrPOINT));

	if(nReturnValue==1)
	{
		if(m_cSettingData.nLSt==1)
		{
			nDistance=sqrt((acpt.x-m_nBeamPos.x)*(acpt.x-m_nBeamPos.x)+(acpt.y-m_nBeamPos.y)*(acpt.y-m_nBeamPos.y));
			if(nDistance <m_cSettingData.nLocation)
			{
				m_pView->m_wOkNgWindow.m_pButton[1].SetCheck(1);
			}
			else
			{
				m_pView->m_wOkNgWindow.m_pButton[1].SetCheck(0);
//				nCheck=1;

			}
		}

		if(m_cSettingData.nDSt==1)
		{
			if(m_nBeamDiameter>(m_cSettingData.nDiameterCenter+m_cSettingData.nDiameterLow) &&
			   m_nBeamDiameter<(m_cSettingData.nDiameterCenter+m_cSettingData.nDiameterHigh))
				m_pView->m_wOkNgWindow.m_pButton[4].SetCheck(1);
			else
			{
				m_pView->m_wOkNgWindow.m_pButton[4].SetCheck(0);
//				nCheck=1;
			}
		}
	}
	else
	{
		if(m_cSettingData.nLSt==1)	{	m_pView->m_wOkNgWindow.m_pButton[1].SetCheck(0);  }
		if(m_cSettingData.nDSt==1)	{	m_pView->m_wOkNgWindow.m_pButton[4].SetCheck(0);  }
	}


	return nReturnValue;
}


int CImgDisplay::ProcessManualBeamDistribution(int flag)
{

	int nReturnValue=0;
	POINT cpt;
	char buf[256];
	int nWidth;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	
    
//	m_nFindFlag=0;

	
	RECT r;


	KIP_Smoothing(m_pOrigin,m_pOut,m_nWidth,m_nHeight,3);
	memcpy(m_pOrigin,m_pOut,m_nWidth*m_nHeight);
	if(flag==1)
		memcpy(m_pSrc,m_pOut,m_nWidth*m_nHeight);
///////////////////////////////////////////////////////
// data region validation check.
    r.left  =m_cROI.m_r.left;
	r.top   =m_cROI.m_r.top;
	r.right =m_cROI.m_r.right;
	r.bottom=m_cROI.m_r.bottom;

	if(r.left < 10)           { r.left=10;             m_cROI.SetRectData(r);}
	if(r.right >m_nWidth -10) { r.right=m_nWidth-10;   m_cROI.SetRectData(r);}
	if(r.top<10)              { r.top =10;             m_cROI.SetRectData(r);}
	if(r.bottom>m_nHeight -10){ r.bottom=m_nHeight-10; m_cROI.SetRectData(r);}
	
///////////////////////////////////////////////////////
	cpt.x=(int)(m_cROI.m_r.right+m_cROI.m_r.left)/2;
	cpt.y=(int)(m_cROI.m_r.bottom+m_cROI.m_r.top)/2;
	nWidth=m_cROI.m_r.right-m_cROI.m_r.left+1;


	m_nOlRadius=(m_nOlRatio*(double)nWidth/2.0);
	m_nBeamRadius=(double)nWidth/2.0;


	m_cOlRect.left =cpt.x - m_nOlRadius;
	m_cOlRect.right=cpt.x +m_nOlRadius;
	m_cOlRect.top =cpt.y -m_nOlRadius;
	m_cOlRect.bottom=cpt.y +m_nOlRadius;

	m_cBeamRect.left=m_cROI.m_r.left;
	m_cBeamRect.top=m_cROI.m_r.top;
	m_cBeamRect.right=m_cROI.m_r.right;
	m_cBeamRect.bottom=m_cROI.m_r.bottom;

	memcpy(&m_cCpt,&cpt,sizeof(POINT));
	memcpy(&m_nBeamPos,&m_cDistanceCoordinate[cpt.y][cpt.x],sizeof(usrPOINT));	
	
	m_nBeamArea=(double)PI_VALUE*nWidth*nWidth*m_nScaleFactor*m_nScaleFactor/4;
	m_nBeamDiameter=(double)(nWidth)*m_nScaleFactor;

	sprintf(buf,"%6.3f",m_nBeamDiameter);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(0,2,buf);
	sprintf(buf,"%6.3f",m_nBeamArea);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(1,0,buf);

	sprintf(buf,"%6.3f,%6.3f",m_cDistanceCoordinate[(int)m_nCy][(int)m_nCx].x,m_cDistanceCoordinate[(int)m_nCy][(int)m_nCx].y);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(2,0,buf);

	sprintf(buf,"%6.3f,%6.3f",m_nBeamPos.x,m_nBeamPos.y);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(2,1,buf);

	m_nFindFlag=1;	
	nReturnValue=1;
	
	double nDistance;
	usrPOINT acpt;
	memcpy(&acpt,&m_cDistanceCoordinate[(int)m_nCy][(int)m_nCx],sizeof(usrPOINT));

	if(nReturnValue==1)
	{
		if(m_cSettingData.nLSt==1)
		{
			nDistance=sqrt((acpt.x-m_nBeamPos.x)*(acpt.x-m_nBeamPos.x)+(acpt.y-m_nBeamPos.y)*(acpt.y-m_nBeamPos.y));
			if(nDistance <m_cSettingData.nLocation)
				m_pView->m_wOkNgWindow.m_pButton[1].SetCheck(1);
			else
			{
				m_pView->m_wOkNgWindow.m_pButton[1].SetCheck(0);
			}
		}

		if(m_cSettingData.nDSt==1)
		{
			if(m_nBeamDiameter>(m_cSettingData.nDiameterCenter+m_cSettingData.nDiameterLow) &&
			   m_nBeamDiameter<(m_cSettingData.nDiameterCenter+m_cSettingData.nDiameterHigh))
				m_pView->m_wOkNgWindow.m_pButton[4].SetCheck(1);
			else
			{
				m_pView->m_wOkNgWindow.m_pButton[4].SetCheck(0);
			}
		}
	}
	else
	{
		if(m_cSettingData.nLSt==1){		m_pView->m_wOkNgWindow.m_pButton[1].SetCheck(0);  }
		if(m_cSettingData.nDSt==1){		m_pView->m_wOkNgWindow.m_pButton[4].SetCheck(0);  }
		
	}
	
//	Invalidate(FALSE);
	return nReturnValue;
}


void CImgDisplay::SetVValue(POINT cpt)
{
	int i,k;
	int j;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	memset(m_nVValue,0x00,m_nHeight);

	int tmpMax=0,tmpValue=0;
	int sum=0;
//	int preMax;
//	int postMax;
	int max_value=0;
	m_nVMaxValue=0;
	for(j=4; j<m_nHeight-4; j++)
	{
		sum=0;
		max_value=0;
		for(k=-1;k<=1; k++)
			for(i=-2;i<=2;i++)
			{
//				sum=(int)*(m_pSrc+m_nWidth*(j+k)+cpt.x+i);
//				if(sum>max_value) max_value=sum;
				sum+=(int)*(m_pOrigin+m_nWidth*(j+k)+cpt.x+i);
//				if(sum>max_value) max_value=sum;

			}

		max_value=sum/15;

		m_nVValue[j]=(unsigned char)max_value;
//		m_nVValue[j]=tmpMax;
		if(j==0) m_nVMaxValue=m_nVValue[j];
		if(m_nVValue[j] > m_nVMaxValue ) m_nVMaxValue=m_nVValue[j];
	}
	m_nVRimLowPos=(int)(-sqrt(m_nOlRadius*m_nOlRadius-(m_cPCpt.x - m_cCpt.x)*(m_cPCpt.x - m_cCpt.x)) +m_cCpt.y);
	m_nVRimHighPos=(int)(sqrt(m_nOlRadius*m_nOlRadius-(m_cPCpt.x - m_cCpt.x)*(m_cPCpt.x - m_cCpt.x)) +m_cCpt.y);

    if(m_nVRimLowPos <1) m_nVRimLowPos=1;
	if(m_nVRimHighPos>m_nHeight-2) m_nVRimHighPos=m_nHeight-2; 


	m_pView->m_wVProj.SetGaussianData(m_nVValue,cpt,m_cCpt,m_nVMaxValue,m_nVRimLowPos,m_nVRimHighPos);
	m_nVMaxValue=m_pView->m_wVProj.GetMaxIntensity();

	m_pView->m_wVProj.Invalidate(FALSE);

}


void CImgDisplay::SetHValue(POINT cpt)
{
//	int i,k;
	int i;
	int j;
	int k;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
//    int tmpMax;
//	int preMax;
//	int postMax;
	int sum=0;
	int max_value;
	m_nHMaxValue=0;
	memset(m_nHValue,0x00,m_nWidth);

	for(i=4; i<m_nWidth-4; i++)
	{
		sum=0;
		max_value=0;
		for(k=-2; k<=2; k++)
			for(j=-1; j<=1; j++)
			{
//				sum=*(m_pSrc+m_nWidth*(cpt.y+k)+i+j);
//				if(sum>max_value) max_value=sum;

				sum+=*(m_pOrigin+m_nWidth*(cpt.y+k)+i+j);
//				if(sum>max_value) max_value=sum;			
			}
		max_value=sum/15;
		m_nHValue[i]=(unsigned char)max_value;
//		m_nHValue[i]=tmpMax;
		if(i==0) m_nHMaxValue=m_nHValue[i];
		if(m_nHValue[i] > m_nHMaxValue ) m_nHMaxValue=m_nHValue[i];
	}

	m_nHRimLowPos=(int)(-sqrt(m_nOlRadius*m_nOlRadius-(m_cPCpt.y - m_cCpt.y)*(m_cPCpt.y - m_cCpt.y)) +m_cCpt.x);
	m_nHRimHighPos=(int)(sqrt(m_nOlRadius*m_nOlRadius-(m_cPCpt.y - m_cCpt.y)*(m_cPCpt.y - m_cCpt.y)) +m_cCpt.x);

	if(m_nHRimLowPos < 1) m_nHRimLowPos=1;
	if(m_nHRimHighPos>m_nWidth-2) m_nHRimHighPos=m_nWidth-2;

	m_pView->m_wHProj.SetGaussianData(m_nHValue,cpt,m_cCpt,m_nHMaxValue,m_nHRimLowPos,m_nHRimHighPos);
	m_nHMaxValue=m_pView->m_wHProj.GetMaxIntensity();

    if(m_nHMaxValue >= m_pView->m_cSettingDataEx.iApcOk && m_nHMaxValue<=m_pView->m_cSettingDataEx.iApcOkHigh)
	{
		gMotionResultData.nGauHorOkNg = VISION_OK;		
	}
	else
	{
		gMotionResultData.nGauHorOkNg = VISION_NG;			
	}		

	gMotionResultData.dHosizontalCMD = (m_pView->m_cSettingDataEx.iApcOk + m_pView->m_cSettingDataEx.iApcOkHigh)/2 - m_nHMaxValue;	

	m_pView->m_wHProj.Invalidate(FALSE);

}


int CImgDisplay::GetPeakBeam()
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	int  nfgclr=255;
	int  nbkclr=0;
	long nBlobNum=0;
	int nSizeObj=0;
	int i;
	int maxsize=0;
	int nReturnValue=0;
	POINT cpt;
	int nSpotNum;
	CLineProc lProc;
	char buf[256];
//	int nCheck=1;
     

//	m_nPFindFlag=0;
	
	nfgclr=255;
	nbkclr=0;

/////////////////////////////////////////////////
	KIL_IDI nMaskId;
	RBLOB_SET_STRUCT scSetBlob;
	RBLOB_RESULT_STRUCT *scResult;

	KRB_Alloc(&nMaskId);

	scSetBlob.nImgHeight=m_nHeight;
	scSetBlob.nImgWidth=m_nWidth;
	scSetBlob.nOffsetX=5;
	scSetBlob.nOffsetY=5;
	scSetBlob.nThresh=m_cSettingData.nPThr;
	scSetBlob.nSizeFilter=100;

	memcpy(&scSetBlob.r,&m_cOlRect,sizeof(RECT));
	
	KIP_ProcessInnerCircle(m_pOrigin,m_pOut,m_cBeamRect,m_nWidth,m_cCpt.x,m_cCpt.y,m_nBeamRadius,m_cSettingData.nPThr,2);
	KRB_Set(nMaskId,&scSetBlob);
	nBlobNum=KRB_Run(nMaskId,m_pOut,1);
////////////////////////////////////
// object가 없을 경우 
////////////////////////////////////
	if(nBlobNum==0)
	{
		KRB_Free(nMaskId);
		nReturnValue=0;
		m_nPFindFlag=0;
	}
	else
	{
		scResult=new RBLOB_RESULT_STRUCT [nBlobNum];
		
		for(i=0; i<nBlobNum; i++)
			KRB_Get(nMaskId,i+1,&scResult[i]);

		if(nBlobNum>1)
		{
			KRB_SortByArea(scResult,0,nBlobNum-1,FALSE);
			nSpotNum=0;
		}


	///////////////////////////////////////////////
	// 위치 좌표로부터 데이터를 얻는다.
		POINT *scpt;
		int j;
		int sumx=0,sumy=0;
		int pointcount=0;

		for(j=0; j<nBlobNum; j++)
		{
			scpt=new POINT [scResult[j].nArea];
			KRB_GetPoint(nMaskId,scResult[j].nIdx,scpt);

			for(i=0; i<scResult[j].nArea; i++)
			{
				sumx+=scpt[i].x;
				sumy+=scpt[i].y;
				pointcount++;
			}
			delete[] scpt;
		}

		cpt.x=sumx/pointcount;
		cpt.y=sumy/pointcount;
//		cpt.x=(int)(scResult[0].nCx +0.5);
//		cpt.y=(int)(scResult[0].nCy+0.5);

		memcpy(&m_cPBeamRect,&scResult[0].r,sizeof(RECT));
  
		memcpy(&m_cPCpt,&cpt,sizeof(POINT));

		memcpy(&m_nPBeamPos,&m_cDistanceCoordinate[cpt.y][cpt.x],sizeof(usrPOINT));	
		m_nPBeamArea=(double)scResult[0].nArea*m_nScaleFactor*m_nScaleFactor;

		double nWidth=(double)(scResult[0].r.right - scResult[0].r.left+1);
		double nHeight=(double)(scResult[0].r.bottom - scResult[0].r.top+1);

		m_nPBeamDiameter=nWidth*m_nScaleFactor;
	///////////////////////////////////////////////	
		m_nPAreaRatio=(m_nPBeamArea/m_nBeamArea)*100.0;
		sprintf(buf,"%6.3f",m_nPAreaRatio);
		m_pView->m_wTabWindow.m_pwCurrent->SetListData(1,1,buf);
	///////////////////////////////////////////////
		m_nParallelism=(nWidth/nHeight)*100.0;
		sprintf(buf,"%6.3f",m_nParallelism);
		m_pView->m_wTabWindow.m_pwCurrent->SetListData(1,2,buf);
	///////////////////////////////////////////////
		m_nPFindFlag=1;	
		nReturnValue=1;
		
		delete[] scResult;

		KRB_Free(nMaskId);
	}

	return nReturnValue;
}

void CImgDisplay::SaveBmp(CBitmap *pBitmap,char *fname)
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

void CImgDisplay::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_nRoiSet==1)
	{
		m_cROI.UIMouseLBDown(point);
		m_nMouseDown=1;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CImgDisplay::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_nRoiSet==1)
	{ 
		if(m_cROI.m_ctlEnable==1)
		{
			m_cROI.UIMouseLBUp(point); 
			Invalidate(FALSE);
		}
		m_nMouseDown=0;
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CImgDisplay::MoveDelta(int x,int y)
{
	m_cROI.MoveOffset(x,y);
	Invalidate(FALSE);
}

void CImgDisplay::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect r;
	int xf,yf;

	if(m_nRoiSet==1)
	{
		if(m_nMouseDown==1) {
			m_cROI.UIMouseMove2(point);
			r=m_cROI.m_r;
			xf=r.Width()/3;
			yf=r.Height()/3;
		
			r.InflateRect(xf,yf);
			InvalidateRect(&r,FALSE);
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CImgDisplay::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_nRoiSet==1)
	{
		m_cROI.UIMouseRBDown(); 
		
		Invalidate(FALSE);
	}	
	CDialog::OnRButtonDown(nFlags, point);
}



void CImgDisplay::RunSaveBmp(char *fname)
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
	DrawPallete(&m_MemDC);
	dc->BitBlt( 0,0,m_nWidth,m_nHeight,&m_MemDC,0,0,SRCCOPY);

	SaveBmp(&m_Bitmap,fname);

	DeleteObject(m_Bitmap);
	ReleaseDC(&m_MemDC);
	ReleaseDC(dc);
}