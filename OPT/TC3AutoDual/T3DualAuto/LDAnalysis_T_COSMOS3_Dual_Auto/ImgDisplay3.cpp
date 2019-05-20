// ImgDisplay3.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "MainFrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "ImgDisplay3.h"
#include "..\\guisrc\\DisplayToGDI.h"
#include <math.h>
#include "External.h"
#include "HVSystemDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImgDisplay3 dialog

char *gITextM3[2][4]={ {"-1.0","-2.0","-3.0","-4.0"},{"1.0","2.0","3.0","4.0"}};
char *gITextP3[2][4]={ {"1.0","2.0","3.0","4.0"},{"-1.0","-2.0","-3.0","-4.0"}};

CImgDisplay3::CImgDisplay3(CWnd* pParent /*=NULL*/)
	: CDialog(CImgDisplay3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImgDisplay3)
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
	m_nBeamPos.x=0.0;
	m_nBeamPos.y=0.0;
	m_nSaveBmp=0;


	m_nOkNgStatus=1;
	m_iProj=NULL;
	m_dGTShift=0;

	m_iRoiSet=0;

}


void CImgDisplay3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImgDisplay3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImgDisplay3, CDialog)
	//{{AFX_MSG_MAP(CImgDisplay3)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgDisplay3 message handlers
BOOL CImgDisplay3::PreTranslateMessage(MSG* pMsg) 
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

void CImgDisplay3::BitmapInit()
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

void CImgDisplay3::ChangePallete()
{
	int i;
	int rr,gg,bb;
	if(m_pLogPalette!=NULL)
	{
		::DeleteObject(m_hPal);
		delete[] m_pLogPalette;
		m_pLogPalette=NULL;
	}

	if(m_cSettingDataEx.iDispMode==1)
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

void CImgDisplay3::ReSetBitmapInfo(int nWidth,int nHeight)
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

void CImgDisplay3::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    EnterCriticalSection(&m_pImage3);
	Draw256Image(&dc,m_pOrigin);
    LeaveCriticalSection(&m_pImage3);

	return;
}

void CImgDisplay3::SetBuffer(unsigned char *pImg)
{
    EnterCriticalSection(&m_pImage3);
	memcpy(m_pSrc,pImg,m_nWidth*m_nHeight);
	KIP_Smoothing(pImg,m_pOrigin,m_nWidth,m_nHeight,3);
    LeaveCriticalSection(&m_pImage3);

	Invalidate(FALSE);
}

void CImgDisplay3::ImgUpdate(unsigned char *pImg)
{

	memcpy(m_pOrigin,pImg,m_nWidth*m_nHeight);
	Invalidate(FALSE);
}

void CImgDisplay3::Draw256Image(CPaintDC *dc,unsigned char *pImg)
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

void CImgDisplay3::Draw256Image(CDC *dc,unsigned char *pImg)
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



BOOL CImgDisplay3::OnInitDialog() 
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

	m_iProj=new int [m_nWidth];
	
	m_nCx=(double)m_nWidth/2.0;
	m_nCy=(double)m_nHeight/2.0;

	InitializeCriticalSection(&m_pImage3);

	return TRUE;  
}

void CImgDisplay3::OnDestroy() 
{
	CDialog::OnDestroy();
	::DeleteObject(m_hPal);

	DeleteCriticalSection(&m_pImage3);
	
	if(m_iProj!=NULL) delete[] m_iProj;
	if(m_pLogPalette!=NULL) delete[] m_pLogPalette;
	if(m_pBitmapBuffer!=NULL) delete[] m_pBitmapBuffer;
	if(m_pSrc!=NULL) delete[] m_pSrc;
	if(m_pOrigin!=NULL) delete[] m_pOrigin;
	if(m_pOut!=NULL) delete[] m_pOut;
	if(m_pTmp!=NULL) delete[] m_pTmp;
}

void CImgDisplay3::SettingParam()
{
	double factor;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	

	memcpy(&m_cSettingData,&m_pView->m_cSettingData,sizeof(SETTING_DATA));
	memcpy(&m_cSettingDataEx,&m_pView->m_cSettingDataEx,sizeof(SETTING_DATAEX));

	m_nCx=(double)m_nWidth/2.0;
	m_nCy=(double)m_nHeight/2.0;


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
    

	MakeGridCoord();
	SelectDisplayType();
	
    m_pView->m_wDisplayComm.AddListData("parameter uploading success for GT ");
	return;
}


int CImgDisplay3::DistanceToPixelPosX(double nDist)
{
	int nValue;

	if(m_cSettingData.nRDir ==0 )	nValue=(int)(m_nCx+nDist/m_nScaleFactor);
	else							nValue=(int)(m_nCx-nDist/m_nScaleFactor);

	return nValue;
}

int CImgDisplay3::DistanceToPixelPosY(double nDist)
{
	int nValue;
	
	if(m_cSettingData.nTDir ==0 )	nValue=(int)(m_nCy-nDist/m_nScaleFactor);
	else							nValue=(int)(m_nCy+nDist/m_nScaleFactor);

	return nValue;
}


void CImgDisplay3::MakeCoordTable()
{
	int i,j;
	// X -> R, Y ->T
//	m_nScaleFactor=(double)m_cSettingData.nBSize/(double)m_cSettingData.nBPixel;
	m_nScaleFactor=((double)m_cSettingDataEx.dDistance/(double)m_cSettingDataEx.iPixel);


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

void CImgDisplay3::MakeGridCoord()
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

void CImgDisplay3::DrawPallete(CDC *pDC)
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

void CImgDisplay3::SelectDisplayType()
{
    SetGridDisplayTag(1);
	Invalidate(FALSE);
}


void CImgDisplay3::MorphImage(unsigned char *pSrc,unsigned char *pOut,RECT r)
{
    MORPH_PARAM mp;
//	RECT r;

	mp.nBkColor=0;
	mp.nCopyBuffer=0;
	mp.nHeight= m_nHeight;
	mp.nKHeight=1;
	mp.nKType=1;
	mp.nKWidth=1;
	mp.nMode=1;
	mp.nObjColor=255;
	mp.nWidth=m_nWidth;


	memcpy(&mp.r,&r,sizeof(RECT));

	KmorphSet(mp);
	KmorphOpenOpt(pSrc,pOut,3);
}

void CImgDisplay3::HorizEdge(unsigned char *pSrc,unsigned char *pTgt,
						 RECT r,int nWidth,int nThresh)
{
	register int i,j;
	int nAddr;
	int nSx,nSy,nEx,nEy;
	int nV1[3],nV2[3];
	int result;

	nSx=r.left+1;
	nEx=r.right-1;
	nSy=r.top+2;
	nEy=r.bottom-2;
    
	for(j=nSy; j<nEy; j++)
	{
		nAddr=nWidth*j;
		for(i=nSx; i<nEx; i++)
		{
			nV1[0]=(int)*(pSrc+nAddr-nWidth*2+i-2);
			nV1[1]=(int)*(pSrc+nAddr+i-2);
			nV1[2]=(int)*(pSrc+nAddr+nWidth*2+i-2);

			nV2[0]=(int)*(pSrc+nAddr-nWidth*2+i+2);
			nV2[1]=(int)*(pSrc+nAddr+nWidth*2+i+2);
			nV2[2]=(int)*(pSrc+nAddr+nWidth*2+i+2);

			result=abs(nV2[0]+2*nV2[1]+nV2[2]-(nV1[0]+2*nV1[1]+nV1[2]));
	
			if((result>>2) > nThresh) *(pTgt+nAddr+i)=255;
		}
	}

	return;
}


int CImgDisplay3::BeamDistributionAnalysis(int pflag,EN_TEST_MODE iTestMode)
{
	int nFlag;
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
    char buf[256];
	POINT *pts;
    int i;
	int k;
	POINT tmp;
	int sum=0;
	int min_x;
	int max_x;
	int offset=81; //121
	double slope;
	double constants;
	CLineProc line;
//	LINE_STRUCT ctline;

	 
	nFlag=ProcessBeamDistribution(pflag);


	if(nFlag==1)
	{
		offset=(m_cBeamRect.bottom-m_cBeamRect.top)/2;
		pts=new POINT [offset];

		nFlag=0;
		k=0;
		sum=0;
		
		for(i=0; i<offset; i++)
		{
			tmp.x=m_cCpt.x;
			tmp.y=m_cCpt.y-((int)(offset/2)-i);
			if(GetGTLocation(tmp))
			{
				pts[k].x=m_cGTPoint.x;
				pts[k].y=m_cGTPoint.y;
				sum+=pts[k].x;

				if(k==0) max_x=min_x=pts[k].x;
				else
				{
					if(pts[k].x > max_x) max_x=pts[k].x;
					if(pts[k].x < min_x) min_x=pts[k].x;
				}
				k++;
				nFlag=1;
			}
////////////////////////////////////////////////////////////
		}
		if(nFlag==1)
		{

			if(k>(offset/2))
			{
				m_cGTPoint.x=(int)((double)(sum-max_x-min_x)/(double)(k-2) + 0.5);
//				m_cGTPoint.y=m_cCpt.y;

				::KFT_LineFittingByMED(pts,k,&slope,&constants);
				line.SetLine(slope,constants,m_cBeamRect,&m_cGTLine);
				line.GetPointToLineDistance(&m_cGTLine,m_cCpt,&m_cGTPoint);
			}
			else
			{
				m_cGTPoint.x=(int)((double)sum/(double)k + 0.5);
				m_cGTPoint.y=m_cCpt.y;
			}
	//		memcpy(&m_nBeamPos,&m_cDistanceCoordinate[cpt.y][cpt.x],sizeof(usrPOINT));	
			memcpy(&m_cGTPos,&m_cDistanceCoordinate[m_cGTPoint.y][m_cGTPoint.x],sizeof(usrPOINT));	
			double ddx;
			double ddy;
			ddx=(m_cGTPos.x-m_nBeamPos.x);
			ddy=(m_cGTPos.y-m_nBeamPos.y);
			
			m_dGTShift=ddx+m_cSettingDataEx.dGTOffset;

			sprintf(buf,"%6.3f",m_dGTShift);
			m_pView->m_wTabWindow.m_pwCurrent->SetListData(0,3,buf);

            


			int iGTShift ;
			int iSetGTShift ;
			int iSetGTShiftHigh ; //double형이 딱 안떨어 져서 삽질함... 30.00000 이 아니고 29.99999999 이런식...

			if(m_dGTShift < 0 ) iGTShift = (int)((m_dGTShift        - 0.00001) * 1000);
			else                iGTShift = (int)((m_dGTShift        + 0.00001) * 1000);
			
			if(iTestMode==tmNormal) {//AUTO장비 리테스트 시에는 GT Shift Spec을 다르게 가져간다. 김극 과장 요청.
			    iSetGTShift     = (int)((m_cSettingDataEx.dGTShift      - 0.00001) * 1000);
				iSetGTShiftHigh = (int)((m_cSettingDataEx.dGTShiftHigh  + 0.00001) * 1000);
            }
			else if(iTestMode==tmReTest){
			    iSetGTShift     = (int)((m_cSettingDataEx.dGTShift2     - 0.00001) * 1000);
				iSetGTShiftHigh = (int)((m_cSettingDataEx.dGTShiftHigh2 + 0.00001) * 1000);
			}
			else {
			    iSetGTShift     = (int)((m_cSettingDataEx.dGTShift3     - 0.00001) * 1000);
				iSetGTShiftHigh = (int)((m_cSettingDataEx.dGTShiftHigh3 + 0.00001) * 1000);
			}

            if(iGTShift<iSetGTShift||iGTShift>iSetGTShiftHigh ) m_nOkNgStatus=1;//NG
			else                                                m_nOkNgStatus=0;//OK

			m_nFindFlag=2;
		}
		else
			m_nOkNgStatus=1;

    	delete [] pts;
	}
	else
		m_nOkNgStatus=1;

	if(m_nOkNgStatus)
	{
		gMotionResultData.nInlineROkNg = VISION_NG;//1이면 NG ////20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다.
	}
	else
	{
		gMotionResultData.nInlineROkNg = VISION_OK;//0이면 OK ////20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다.
	}

	gMotionResultData.dInlineRShift = m_dGTShift;

	Invalidate(FALSE);

	return nFlag;
}

int CImgDisplay3::CheckErrorFlag()
{
	return m_nOkNgStatus;
}

int CImgDisplay3::ProcessBeamDistribution(int flag)
{

	int  nfgclr=255;
	int  nbkclr=0;
	long nBlobNum=0;
	int nSizeObj=0;
	int i;
	int maxsize=0;
	int nReturnValue=0;
	POINT cpt;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	
    POINT *pts;
    double tcx,tcy,tradius;
	nfgclr=255;
	nbkclr=0;
	
	RECT r;
	r.left=10;
	r.right=m_nWidth-10;
	r.top=10;
	r.bottom=m_nHeight-10;
 
/////////////////////////////////////////////////
	KIL_IDI nMaskId;
	RBLOB_SET_STRUCT scSetBlob;
	RBLOB_RESULT_STRUCT *scResult;

	KRB_Alloc(&nMaskId);

	scSetBlob.nImgHeight=m_nHeight;
	scSetBlob.nImgWidth=m_nWidth;
	scSetBlob.nOffsetX=5;
	scSetBlob.nOffsetY=5;
	scSetBlob.nThresh=m_cSettingDataEx.iBThr;
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
    	if(KFT_CircleFittingByLS(scResult[0].r,m_pOut,m_nWidth,&tcx,&tcy,&tradius,0)==0)
		{
			delete[] scResult;
			KRB_Free(nMaskId);
			m_nFindFlag=0;	
			return 0;
		}

	
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


		memcpy(&m_cBeamRect,&scResult[0].r,sizeof(RECT));
  		memcpy(&m_cCpt,&cpt,sizeof(POINT));

		memcpy(&m_nBeamPos,&m_cDistanceCoordinate[cpt.y][cpt.x],sizeof(usrPOINT));	
		

		delete[] scResult;
		KRB_Free(nMaskId);
		m_nFindFlag=1;	
		nReturnValue=1;
	}

	return nReturnValue;
}





int CImgDisplay3::GetGTLocation(POINT pt)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	int i;
	int j;
	int k;
	int sum=0;
	int max_value=0;
	int min_value=255;
    int start;
	int end;
	int obj_start;
	int obj_end;
	int width;
	int ithreshold=28;

	memset(m_iProj,0x00,m_nWidth);

	RECT r;
	r.left =m_cBeamRect.left +(m_cBeamRect.right - m_cBeamRect.left+1)/4;
	r.right=m_cBeamRect.right-(m_cBeamRect.right - m_cBeamRect.left+1)/4;

	width=(m_cBeamRect.right - m_cBeamRect.left+1)/5;
	for(i=4; i<m_nWidth-4; i++)
	{
		sum=0;
		for(k=-2; k<=2; k++)
			for(j=-1; j<=1; j++)
			{
				sum+=*(m_pOrigin+m_nWidth*(pt.y+k)+i+j);
			}
		m_iProj[i]=sum/15;
		if(i==4){ min_value=max_value=m_iProj[i]; }
		if(m_iProj[i] < min_value) min_value=m_iProj[i];
		if(m_iProj[i] > max_value) max_value=m_iProj[i];

	}
//k=m_cSettingDataEx.iEThr;
	for(k=m_cSettingDataEx.iEThr; k<250; k++)
	{
		if((k - ithreshold)< ithreshold) continue;

		start=0;
		for(i=m_cBeamRect.left; i<m_cBeamRect.right; i++)
		{
			if(m_iProj[i] >=k)
			{
				start=i;
				break;
			}
		}
       if(start==0) continue;

	   end=0;
	   for(i=m_cBeamRect.right-1;i>m_cBeamRect.left; i--)
	   {
			if(m_iProj[i] >=k)
			{
				end=i;
				break;
			}
	   }
       if(end==0) continue;
	   if(end - start < 15) continue;

       obj_start=0;
	   for(i=start; i<end; i++)
	   {
			if(m_iProj[i] <k-ithreshold)
			{
				obj_start=i;
				break;
			}
	   }
	   if(obj_start==0) continue;
       obj_end=0;
	   for(i=end; i>start; i--)
	   {
			if(m_iProj[i] <k-ithreshold)
			{
				obj_end=i;
				break;
			}
	   }
	   if(obj_end==0) continue;

	   if((obj_end - obj_start) < 10 || (obj_end - obj_start) > width ) continue;

       m_cGTPoint.x=(obj_end+obj_start)/2;
	   m_cGTPoint.y=pt.y;
	   return 1;

	}
	return 0;
}

void CImgDisplay3::SaveBmp(CBitmap *pBitmap,char *fname)
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





void CImgDisplay3::RunSaveBmp(char *fname)
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

void CImgDisplay3::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_iRoiSet==1)
	{
		m_cROI.UIMouseLBDown(point);
		m_iMouseDown=1;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CImgDisplay3::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_iRoiSet==1)
	{ 
		if(m_cROI.m_ctlEnable==1)
		{
			m_cROI.UIMouseLBUp(point); 
			Invalidate(FALSE);
		}
		m_iMouseDown=0;
	}
	CDialog::OnLButtonUp(nFlags, point);
}


void CImgDisplay3::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect r;
	int xf,yf;

	if(m_iRoiSet==1)
	{
		if(m_iMouseDown==1)
		{
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

void CImgDisplay3::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_iRoiSet==1)
	{
		m_cROI.UIMouseRBDown(); 
		
		Invalidate(FALSE);
	}	
	CDialog::OnRButtonDown(nFlags, point);
}


void CImgDisplay3::DisplayGrid(CDC *memDC)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	

	register int i;
	POINT zeropt;
    CDisplayToGDI cDisp;
	//20101216 lwb 참조되지 않은 변수는 주석처리.
//	POINT cAbCenter;
	char buf[256];

    
	usrPOINT tpt1,tpt2;

	zeropt.x=m_nWidth/2;
	zeropt.y=m_nHeight/2;
	
	memDC->SetBkMode(TRANSPARENT);

	if(m_iRoiSet==1)
		m_cROI.DrawRoi(memDC);

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
				cDisp.DTextOut(memDC,(int)m_cLHp[i].sp.x,(int)m_cLHp[i].ep.y+5,gITextP3[0][i],RGB(0,255,0));
				cDisp.DTextOut(memDC,(int)m_cLHm[i].sp.x,(int)m_cLHm[i].ep.y+5,gITextM3[0][i],RGB(0,255,0));

				if(i==3)
				{
					cDisp.DTextOut(memDC,(int)m_cLVp[i].ep.x+16,(int)m_cLVp[i].sp.y+16,gITextP3[0][i],RGB(0,255,0));
					cDisp.DTextOut(memDC,(int)m_cLVm[i].ep.x+16,(int)m_cLVm[i].sp.y-16,gITextM3[0][i],RGB(0,255,0));	
				}
				else
				{
					// 정상적 vertical 상수 디스플레이 
					cDisp.DTextOut(memDC,(int)m_cLVp[i].ep.x+16,(int)m_cLVp[i].sp.y-4,gITextP3[0][i],RGB(0,255,0));
					cDisp.DTextOut(memDC,(int)m_cLVm[i].ep.x+16,(int)m_cLVm[i].sp.y-4,gITextM3[0][i],RGB(0,255,0));	
				}
			}
		
		}
	}
//	cAbCenter.x=(int)m_nCx;
//	cAbCenter.y=(int)m_nCy;

//	cDisp.DCrossPointWithEllipse(memDC,cAbCenter,10,RGB(255,0,0),RGB(0,0,0));


	if(m_nFindFlag==1)
	{
		
		cDisp.DEllipse(memDC,m_cBeamRect,RGB(0,255,0));
		cDisp.DCrossPointWithFillEllipse(memDC,m_cCpt,0,RGB(0,255,0),RGB(0,255,0));
	}
	if(m_nFindFlag==2)
	{
		
		cDisp.DEllipse(memDC,m_cBeamRect,RGB(0,255,0));
		cDisp.DCrossPointWithEllipse(memDC,m_cCpt,10,3,RGB(0,255,0),RGB(0,255,0));
		cDisp.DCrossPointWithEllipse(memDC,m_cGTPoint,20,3,RGB(0,0,255),RGB(0,0,0));

		sprintf(buf,"GT-SHIFT : %6.3f mm",m_dGTShift);//<--------20110124 lwb 이 부분을 모션에 보냄.m_nOkNgStatus의 결과도 보냄.
		cDisp.DTextOutWithFontII(memDC,64,430,14,28,buf,RGB(255,255,255));		

		tpt1.x=m_cGTLine.sx;
		tpt1.y=m_cGTLine.sy;
		tpt2.x=m_cGTLine.ex;
		tpt2.y=m_cGTLine.ey;	
		cDisp.DLine(memDC,m_cCpt,m_cGTPoint,RGB(0,0,0));
//		cDisp.DLine(memDC,tpt1,tpt2,RGB(255,0,0));

	}
	cDisp.DTextOutWithFontII(memDC,10,10,12,20,"3: GT DETECTION",RGB(255,255,0));
	if(m_pView->m_wTabWindow.GetSetupActive())
	{
		sprintf(buf,"Beam Center (%6.3f, %6.3f)",m_nBeamPos.x,m_nBeamPos.y);
		cDisp.DTextOutWithFontII(memDC,10,40,10,18,buf,RGB(0,255,0));
	}

	if(g_bLeftRight) {//Right
	    cDisp.DTextOutWithFontII(memDC,480,10,12,20,"Right Table",RGB(255,255,255));
	}
	else { //Left
        cDisp.DTextOutWithFontII(memDC,480,10,12,20,"Left Table",RGB(255,255,255)); 
	}


	if(m_nOkNgStatus==1)
	{
		cDisp.DTextOutWithFontII(memDC,510,40,30,60,"NG",RGB(255,0,0));
		//20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다.
		//gMotionResultData.nInlineOkNg = VISION_NG;//1이면 NG ////20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다.
	}
	else
	{
		cDisp.DTextOutWithFontII (memDC,510,40,30,60,"OK",RGB(0,255,0));
		//gMotionResultData.nInlineOkNg = VISION_OK;//0이면 OK ////20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다.
	}
	//m_nOkNgStatus=0;
	//20110124 lwb Motion Dual Auto 사용하기 위해 정보를 건네준다.
	//gMotionResultData.dInlineShift = m_dGTShift;
	return;
}




