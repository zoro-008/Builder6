// AngleWindow.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "MainFrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "AngleWindow.h"
#include "..\\GuiSrc\\DisplayToGdi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAngleWindow dialog
char *gpTextM[2][4]={ {"-10","-20","-30","-40"},{"10","20","30","40"}};
char *gpTextP[2][4]={ {"10","20","30","40"},{"-10","-20","-30","-40"}};

CAngleWindow::CAngleWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CAngleWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAngleWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nWidth=320;
	m_nHeight=240;
	m_nCx=160;
	m_nCy=120;
	m_nMouseDown=0;
	m_nPrevFlag=0;
	m_nRDir=0;
	m_nTDir=0;
	m_nDrawTag=0;

	m_cCenter.x=160;
	m_cCenter.y=120;

	m_nRMinute=0;
	m_nTMinute=0;
}


void CAngleWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAngleWindow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAngleWindow, CDialog)
	//{{AFX_MSG_MAP(CAngleWindow)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAngleWindow message handlers

BOOL CAngleWindow::PreTranslateMessage(MSG* pMsg) 
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

void CAngleWindow::OnDestroy() 
{
	CDialog::OnDestroy();
	if(m_pBitmapBuffer!=NULL) delete[] m_pBitmapBuffer;

	// TODO: Add your message handler code here
	
}

BOOL CAngleWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	BitmapInit();	
	SettingCoordinate();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAngleWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawGrid(&dc);
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CAngleWindow::BitmapInit()
{
//	int i;
	
//////////////////////////////////////////////////////
// bitmap header setting
//////////////////////////////////////////////////////
	m_BitmapInfoHeader.biSize         =(DWORD)40;
	m_BitmapInfoHeader.biWidth        =(LONG)m_nWidth;
	m_BitmapInfoHeader.biHeight       =(LONG)m_nHeight;
	m_BitmapInfoHeader.biPlanes       =(WORD)1;
	m_BitmapInfoHeader.biBitCount     =(WORD)24;
	m_BitmapInfoHeader.biCompression  =(DWORD)BI_RGB;
	m_nDummy=(m_nWidth%4);
	((4-m_nDummy)==4) ? m_nDummy=0:m_nDummy=4-m_nDummy;
	m_BitmapInfoHeader.biSizeImage    =(DWORD)((m_nWidth+m_nDummy)*m_nHeight*3);
	m_BitmapInfoHeader.biXPelsPerMeter=(LONG)0;
	m_BitmapInfoHeader.biYPelsPerMeter=(LONG)0;
	m_BitmapInfoHeader.biClrUsed      =(DWORD)0;
	m_BitmapInfoHeader.biClrImportant =(DWORD)0;

	m_pBitmapBuffer=new unsigned char [(m_nWidth+m_nDummy)*m_nHeight*3];
	memset(m_pBitmapBuffer,0x00,(m_nWidth+m_nDummy)*m_nHeight*3);

}

void CAngleWindow::DrawGrid(CPaintDC *dc)
{
	int start=m_nHeight-1;
	int tmpWidth=m_nWidth+m_nDummy;
	CDC m_MemDC;
	CBitmap m_Bitmap,*m_OldBitmap;

	m_MemDC.CreateCompatibleDC(dc);
    
	m_Bitmap.CreateCompatibleBitmap(dc,tmpWidth,m_nHeight);
	m_OldBitmap=m_MemDC.SelectObject(&m_Bitmap);

	::SetStretchBltMode(m_MemDC.GetSafeHdc(),COLORONCOLOR);
	::SetDIBitsToDevice(m_MemDC.GetSafeHdc(),0,0,tmpWidth,m_nHeight,
		                             0,0,0,m_nHeight,
									 m_pBitmapBuffer,
									 (LPBITMAPINFO)&m_BitmapInfoHeader,
									 DIB_RGB_COLORS);
	DisplayGrid(&m_MemDC);
	dc->BitBlt( 0,0,m_nWidth,m_nHeight,&m_MemDC,0,0,SRCCOPY);
}


void CAngleWindow::SettingCoordinate()
{
	double factor;

	MakeCoordTable(m_nRDir,m_nTDir);

	factor=(40.0/m_nScaleFactor);
	m_cHsp.x=(m_nCx-factor);
	m_cHsp.y=m_nCy;
	m_cVsp.y=(m_nCy-factor);
	m_cVsp.x=m_nCx;

	m_cHep.x=(m_nCx+factor);
	m_cHep.y=m_nCy;
	m_cVep.y=(m_nCy+factor);
	m_cVep.x=m_nCx;
	MakeGridCoord();
	Invalidate();
	return ;
}
/************************************************************
prototype : void MakeCoordTable()
function  : 스케일 팩터를 감안하여 픽셀 좌표계를 분단위 좌표계로 
            변환을 한 후 메모리에 저장한다. 
			(방향을 고려하여 )
parameter : none
return    : none
date      : 2003.6
author    : 김양주 
remark    : none
************************************************************/
void CAngleWindow::MakeCoordTable(int rDir,int tDir)
{
	int i,j;
	// X -> R, Y ->T, 스케일 팩터를 구한다. 
	m_nScaleFactor=(double)80.0/(double)240.0;

	// 정방향, 정방향 
	if(rDir ==0 && tDir==0)
	{
		for(j=0; j<m_nHeight; j++)
			for(i=0; i<m_nWidth; i++)
			{
				m_cMinuteCoordinate[j][i].x=((double)i - m_nCx) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].y=(m_nCy-(double)j) * m_nScaleFactor;
			}
		return;
	}
    // 역방향 , 정방향 
	if(rDir ==1 && tDir==0)
	{
		for(j=0; j<m_nHeight; j++)    //t
			for(i=0; i<m_nWidth; i++) //r
			{
				m_cMinuteCoordinate[j][i].x=(double)(m_nCx - (double)i) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].y=(double)(m_nCy-(double)j) * m_nScaleFactor;
			}
		return;
	}
    // 정방향, 역방향 
	if(rDir ==0 && tDir==1)
	{
		for(j=0; j<m_nHeight; j++) //t
			for(i=0; i<m_nWidth; i++) //r
			{
				m_cMinuteCoordinate[j][i].x=((double)i - m_nCx) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].y=((double)j - m_nCy) * m_nScaleFactor;
			}

		return;
	}
    // 역방향 , 역방향 
	if(rDir ==1 && tDir==1)
	{
		for(j=0; j<m_nHeight; j++)
			for(i=0; i<m_nWidth; i++)
			{
				m_cMinuteCoordinate[j][i].x=(m_nCx - (double)i) * m_nScaleFactor;
				m_cMinuteCoordinate[j][i].y=((double)j-m_nCy) * m_nScaleFactor;
			}
	}
	return;
}


/*****************************************************
prototype : void MakeGridCoord()
function  : 화면에 표시할 그리드 좌표계를 생성하고 
            표시한다. (+/- 40분 ) 
parameter : none
return    : none
date      : 2003.6
author    : 김양주 
remark    : none
*****************************************************/
void CAngleWindow::MakeGridCoord()
{
	double factor;
    usrPOINT tsp,tep;
	usrPOINT tspt,tept;
	int offset=1;
    int k,kk;


//	DisplayLine(m_cHsp,m_cHep,RGB(0,255,0),1);
//	DisplayLine(m_cVsp,m_cVep,RGB(0,255,0),1);

	factor=1.0/m_nScaleFactor;

	tsp.y=m_nCy-offset;
	tep.y=m_nCy+offset+1;
	double nnx=m_nCx;

	k=0; kk=0;
	//+ direction
	while(k<=40)
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
	while(k<=40)
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
	while(k<=40)
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
	while(k<=40)
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
	return;
}

void CAngleWindow::DisplayGrid(CDC *memDC)
{
	register int i;
	CDisplayToGDI cGDI;
	
	memDC->SetBkMode(TRANSPARENT);
	cGDI.DLine(memDC,m_cHsp,m_cHep,RGB(0,255,0));
	cGDI.DLine(memDC,m_cVsp,m_cVep,RGB(0,255,0));

	for(i=0; i<40; i++)
	{
		cGDI.DLine(memDC,m_cSHp[i].sp,m_cSHp[i].ep,RGB(0,255,0));		
		cGDI.DLine(memDC,m_cSHm[i].sp,m_cSHm[i].ep,RGB(0,255,0));		
		cGDI.DLine(memDC,m_cSVp[i].sp,m_cSVp[i].ep,RGB(0,255,0));		
		cGDI.DLine(memDC,m_cSVm[i].sp,m_cSVm[i].ep,RGB(0,255,0));		
	}

	for(i=0; i<4; i++)
	{
		cGDI.DDotLine(memDC,m_cLHpG[i].sp,m_cLHpG[i].ep,RGB(255,128,32));
		cGDI.DDotLine(memDC,m_cLHmG[i].sp,m_cLHmG[i].ep,RGB(255,128,32));		
		cGDI.DDotLine(memDC,m_cLVpG[i].sp,m_cLVpG[i].ep,RGB(255,128,32));		
		cGDI.DDotLine(memDC,m_cLVmG[i].sp,m_cLVmG[i].ep,RGB(255,128,32));		
		
		if((i%2)==0)
		{
			cGDI.DTextOut(memDC,(int)m_cLHp[i].sp.x,(int)m_cLHp[i].ep.y,gpTextP[m_nRDir][i],RGB(0,255,0));
			cGDI.DTextOut(memDC,(int)m_cLHm[i].sp.x,(int)m_cLHm[i].ep.y,gpTextM[m_nRDir][i],RGB(0,255,0));

			if(i==3)
			{
				cGDI.DTextOut(memDC,(int)m_cLVp[i].ep.x+2,(int)m_cLVp[i].sp.y+5,gpTextP[m_nTDir][i],RGB(0,255,0));
				cGDI.DTextOut(memDC,(int)m_cLVm[i].ep.x+2,(int)m_cLVm[i].sp.y-20,gpTextM[m_nTDir][i],RGB(0,255,0));	
			}
			else
			{
				cGDI.DTextOut(memDC,(int)m_cLVp[i].ep.x+2,(int)m_cLVp[i].sp.y-4,gpTextP[m_nTDir][i],RGB(0,255,0));
				cGDI.DTextOut(memDC,(int)m_cLVm[i].ep.x+2,(int)m_cLVm[i].sp.y-10,gpTextM[m_nTDir][i],RGB(0,255,0));	
			}
		}
	}

	if(m_nDrawTag==1)
		DisplayAngle(memDC,m_nRMinute,m_nTMinute);
	return;
}

void CAngleWindow::SetMinute(double rv,double tv)
{
	m_nRMinute=rv;
	m_nTMinute=tv;

	return;
}

void CAngleWindow::DisplayAngle(CDC *pDC,double xpos,double ypos)
{
	CDisplayToGDI cGDI;	
	RECT r;
	POINT pt;

	MinuteToPixel(xpos,ypos,&pt);

	r.left  =pt.x-5;
	r.top   =pt.y-5;
	r.right =pt.x+5;
	r.bottom=pt.y+5;

	cGDI.DSolidEllipse(pDC,r,RGB(255,255,0));
	cGDI.DCrossPoint(pDC,pt,5,RGB(255,0,0));
	cGDI.DLine(pDC,m_cCenter,pt,RGB(0,255,0));
}

void CAngleWindow::MinuteToPixel(double rValue,double tValue,POINT *coord)
{
	// X -> R, Y ->T
	if(m_nRDir ==0 && m_nTDir==0)	
	{
		coord->x=(int)(m_nCx +rValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy -tValue/m_nScaleFactor+0.5);
		return;
	}

	if(m_nRDir ==1 && m_nTDir==0)	
	{
		coord->x=(int)(m_nCx -rValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy -tValue/m_nScaleFactor+0.5);
		return;
	}
	
	if(m_nRDir ==0 && m_nTDir==1)	
	{
		coord->x=(int)(m_nCx +rValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy +tValue/m_nScaleFactor+0.5);
		return;
	}
	
	if(m_nRDir ==1 && m_nTDir==1)	
	{
		coord->x=(int)(m_nCx -rValue/m_nScaleFactor+0.5);	
		coord->y=(int)(m_nCy +tValue/m_nScaleFactor+0.5);
	}
	return;
}

void CAngleWindow::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_nPrevFlag=0;
	CDialog::OnLButtonDown(nFlags, point);
}

void CAngleWindow::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_nPrevFlag=0;
	CDialog::OnLButtonUp(nFlags, point);
}

void CAngleWindow::OnMouseMove(UINT nFlags, CPoint point) 
{
	int dxx,dyy;

	if( nFlags==MK_LBUTTON)
	{
		ClientToScreen(&point);
        if(m_nPrevFlag==1)
		{
			dxx=point.x-m_cPrevPoint.x;
			dyy=point.y-m_cPrevPoint.y;
			
			m_cWndMPos.left+=dxx;
			m_cWndMPos.right+=dxx;
			m_cWndMPos.top+=dyy;
			m_cWndMPos.bottom+=dyy;
			
			m_cPrevPoint=point;
			MoveWindow(&m_cWndMPos);
		}
		m_cPrevPoint=point;
		m_nPrevFlag=1;
		ScreenToClient(&point);
	}
	CDialog::OnMouseMove(nFlags, point);
}


void CAngleWindow::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	MoveWindow(&m_cWndPos);
	CDialog::OnLButtonDblClk(nFlags, point);
}
