// StaticSkin.cpp : implementation file
//

#include "stdafx.h"
#include "StaticSkin.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticSkin

CStaticSkin::CStaticSkin()
{
	pBmpRgn=NULL;
	m_BmpFile=NULL;
	m_nFgColor=RGB(0,0,0);
	m_Font=NULL;
	m_Font=new CFont;
	m_Font->CreateFont(16,8, 0, 0, FW_BOLD, FALSE, FALSE,0,0,0,0,0,0, "±¼¸²Ã¼");
	memset(m_pMsg,0x00,256);
	m_nX=4;
	m_nY=4;
	int i;
	m_nContMsgFlag=0;
	m_nContMsgCount=0;
	for(i=0;i<10; i++)
		m_pContMsg[i][0]='0';

}

CStaticSkin::~CStaticSkin()
{
	if(pBmpRgn!=NULL) delete pBmpRgn;
	if(m_Font!=NULL)  delete m_Font;
}



BEGIN_MESSAGE_MAP(CStaticSkin, CStatic)
	//{{AFX_MSG_MAP(CStaticSkin)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticSkin message handlers
void CStaticSkin::SetBmpFile(char *pBmpName)
{
	m_BmpFile=pBmpName;
}
void CStaticSkin::OutTextII(char *pStr)
{
	strcpy(m_pMsg,pStr);
	Invalidate();
}

void CStaticSkin::SetFontSize(int w,int h,char *fname)
{
	delete m_Font;

	m_Font=new CFont;
	m_Font->CreateFont(h,w, 0, 0, FW_BOLD, FALSE, FALSE,0,0,0,0,0,0, fname);
}

void CStaticSkin::OutTextXY(int x, int y,char *pStr)
{
	m_nX=x;
	m_nY=y;
	strcpy(m_pMsg,pStr);
	Invalidate();
}

void CStaticSkin::OutTextContXY(int x, int y,char *pStr,int idx)
{
	m_nCX[idx]=x;
	m_nCY[idx]=y;
	m_nContMsgFlag=1;
	m_nContMsgCount=idx+1;
	strcpy(m_pContMsg[idx],pStr);
}


void CStaticSkin::OutText(char *pStr)
{
	strcpy(m_pMsg,pStr);
}


void CStaticSkin::SetFgColor(UINT nColor)
{
	m_nFgColor=nColor;
}
void CStaticSkin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CFont *oldFont;
    int i;
	if(m_dcBkGrnd)
	{
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
		if(m_Font!=NULL) {
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(m_nFgColor);
			oldFont=dc.SelectObject(m_Font);
			if(m_nContMsgFlag==0)
				dc.TextOut(m_nX,m_nY,m_pMsg,strlen(m_pMsg));
			else
			{
				for(i=0; i<m_nContMsgCount;i++)
					dc.TextOut(m_nCX[i],m_nCY[i],m_pContMsg[i],strlen(m_pContMsg[i]));
			}
			dc.SelectObject(oldFont);
		}
	}
}

void CStaticSkin::PreSubclassWindow() 
{
	pBmpRgn=new CBmpRgn();

	pBmpRgn->SetParentWindow(this);
	HRGN rgnH = pBmpRgn->BuildRegion(m_BmpFile, 0x00000000);
	if(rgnH)
	{
		SetWindowRgn(rgnH, TRUE);
		DeleteObject(rgnH);
	}	
	CDC *pDC = GetDC();
	m_dcBkGrnd = CreateCompatibleDC(pDC->m_hDC);
	ReleaseDC(pDC);
	// select background image
	HBITMAP hBmp = pBmpRgn->GetBitmap();
	SelectObject(m_dcBkGrnd, hBmp);
	m_nWindowWidth = pBmpRgn->GetBitmapWidth();
	m_nWindowHeight = pBmpRgn->GetBitmapHeight();

	CStatic::PreSubclassWindow();
}
