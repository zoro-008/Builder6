// HProjection.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "MainFrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "HProjection.h"
#include "..\\GuiSrc\\DisplayToGDI.h"
#include <math.h>
#include "External.h"
#include "HVSystemDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHProjection dialog


CHProjection::CHProjection(CWnd* pParent /*=NULL*/)
: CDialog(CHProjection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHProjection)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nWidth=640;
	m_nHeight=256;
	m_nScaleFactor=8.0/480.0;
	m_nBWidthCriteria=5.0/m_nScaleFactor;
	m_nCx=320.0;
	m_nCy=240.0;
	m_nGaussianDrawTag=0;
	m_nOriginLow=0;
	m_nOriginHigh=0;
	m_nLow=0;
	m_nHigh=0;
	m_nHRimValue=0.0;
}


void CHProjection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHProjection)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHProjection, CDialog)
//{{AFX_MSG_MAP(CHProjection)
ON_WM_DESTROY()
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHProjection message handlers

BOOL CHProjection::PreTranslateMessage(MSG* pMsg) 
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

void CHProjection::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pBitmapBuffer!=NULL) delete[] m_pBitmapBuffer;
	
}

BOOL CHProjection::OnInitDialog() 
{
	CDialog::OnInitDialog();
	BitmapInit();	
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHProjection::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawGrid(&dc);
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CHProjection::PaintEx()
{
    OnPaint() ;
}

void CHProjection::BitmapInit()
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
    
	MakeColor();
}

void CHProjection::DrawGrid(CPaintDC *dc)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

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
	
	//DrawGaussian(&m_MemDC);
	if(m_nGaussianDrawTag==1)	DrawGaussian(&m_MemDC);
	
	dc->BitBlt( 0,0,m_nWidth,m_nHeight,&m_MemDC,0,0,SRCCOPY);
}

//////////////////////////////////////
// parameter setup
void CHProjection::SetParam()
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	
	
	memcpy(&m_cSettingData  ,&m_pView->m_cSettingData  ,sizeof(SETTING_DATA  ));
    memcpy(&m_cSettingDataEx,&m_pView->m_cSettingDataEx,sizeof(SETTING_DATAEX));

	MakeColor();

    //여기 부터 스펙 넣기....sunsunsun.



	m_nScaleFactor=((double)8.91/(double)640.0)+(double)m_cSettingData.nBPixel/10000.0;
	
	//	m_nScaleFactor=m_cSettingData.nBSize/m_cSettingData.nBPixel;
	
	m_nBWidthCriteria=GetDistanceToPixel(m_cSettingData.nDiameterCenter);
	m_nBWidthUpper=m_nCx+m_nBWidthCriteria/2;//+GetDistanceToPixel(m_cSettingData.nDiameterHigh)/2;
    m_nBWidthLower=m_nCx-m_nBWidthCriteria/2;//+GetDistanceToPixel(m_cSettingData.nDiameterHigh)/2;
	return;
}

double CHProjection::NormGaussian(double s,double v,double m)//s는 표준 편차 , v는 X m은 평균. 
{
	return (1.0/(s*sqrt(2*PI_VALUE)))   *exp(-(v-m)*(v-m)/(2*s*s));
}

double CHProjection::LDGaussian(double s,double v,double m) 
{
	return m_nDPeak   *exp(-(v-m)*(v-m)/(2*s*s));
}

void CHProjection::SetGaussianData(unsigned char *pIn,POINT cpt,POINT bcpt,int maxv,int low, int high)
{
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	double nTmpValue[640];
	
	memcpy(&m_cBCpt,&bcpt,sizeof(POINT));
	
	m_nDWidth=(double)(high - low);
	m_nOriginLow=low;
	m_nOriginHigh=high;
    m_nLow=low-m_cSettingData.nRimLocate;
	m_nHigh=high+m_cSettingData.nRimLocate;
	if(m_nLow < 0) m_nLow=0;
	if(m_nHigh>639) m_nHigh=639;
	
	m_nDCx=cpt.x;
	m_nDCy=cpt.y;
	memcpy(m_nValue,pIn,640);
	
	int count=0;
	int i;
	int offset;
	m_nDataThr= m_cSettingData.nDataThr;
	
	offset=(int)GetDistanceToPixel(m_nDataThr/2.0);
	//	m_nStartDataLoc=m_nDCx-offset;
	//	m_nEndDataLoc=m_nDCx+offset;
	
	m_nStartDataLoc=m_nOriginLow-offset;
	m_nEndDataLoc=m_nOriginHigh+offset;
	
	if(m_nStartDataLoc <1) m_nStartDataLoc=1;
	if(m_nStartDataLoc >638) m_nStartDataLoc=638;
	if(m_nEndDataLoc <1) m_nEndDataLoc=1;
	if(m_nEndDataLoc >638) m_nEndDataLoc=638;
	
	if(m_nStartDataLoc>=m_nEndDataLoc) return;
	
	for(i=0; i<=639; i++)
	{
		nTmpValue[count]=m_nValue[i];
        count++;
	}
	if(count>0)
	{
		ComputeData(nTmpValue,count,0);
	}
	Invalidate(FALSE);
	return;
}

void CHProjection::ReCompute(double datathr,double wcorrect)
{
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	int count=0;
	int i;
	double nTmpValue[640];
	int offset;
	m_nDataThr=datathr;
	
	offset=(int)GetDistanceToPixel(m_nDataThr/2.0);
	//	m_nStartDataLoc=m_nDCx-offset;
	//	m_nEndDataLoc=m_nDCx+offset;
	
	m_nStartDataLoc=m_nOriginLow-offset;
	m_nEndDataLoc=m_nOriginHigh+offset;
	
	if(m_nStartDataLoc <1) m_nStartDataLoc=1;
	if(m_nStartDataLoc >638) m_nStartDataLoc=638;
	if(m_nEndDataLoc <1) m_nEndDataLoc=1;
	if(m_nEndDataLoc >638) m_nEndDataLoc=638;
	
	if(m_nStartDataLoc>=m_nEndDataLoc) return;
	
	for(i=0; i<=639; i++)
	{
		nTmpValue[count]=m_nValue[i];
        count++;
	}
	if(count >0)
	{
		ComputeData(nTmpValue,count,0,wcorrect);
	}
	
	return;
}

/**********************************

**********************************/
void CHProjection::ComputeData(double *nData,int count,int flag,double wcc)
{
	double sum=0.0;
	int i;
	double max_intensity=0;
	char buf[256];
	double nMinValue;
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	
	///////////////////////////////////////////////////////////////////////////////////
	// y=A*Exp(-x*x/(2*r*r)) => ln(y)=ln(A)+B*x*x, B=-1/(2*r*r), A=Exp(a), lny=a+b*x*x
	///////////////////////////////////////////////////////////////////////////////////
	
	double x4y_sum=0.0,ylny_sum=0.0,x2y_sum=0.0,x2ylny_sum=0.0,y_sum=0.0;
    double tct=(double)m_nDCx;
	double a_value,b_value;
	double tmp_data;
	double tmp_x2;
	
	double lny_sum=0,x4_sum=0,x2_sum=0,x2lny_sum=0;
	double error_sum=0;
    double nn=0;
	double result[640];
    int j;
	double min_err,min_sigma,min_cp,min_peak;
	/////////////////////
	// 2씩 증가한다 ... 속도를 증가 시키기 위해.
	/////////////////////
	for(j=m_nStartDataLoc;j<=m_nEndDataLoc;j+=2)
	{
		nn=0;
		lny_sum=0;
		x4_sum=0;
		x2_sum=0;
		x2lny_sum=0;
		error_sum=0;
		for(i=m_nStartDataLoc; i<=m_nEndDataLoc ;i+=2)
		{
			tmp_data=nData[i];
			if(tmp_data >=m_cSettingData.nRimInspect)
			{
				tmp_x2=((double)i-(double)j)*((double)i-(double)j);
				
				lny_sum+=log(tmp_data);
				x4_sum+=(tmp_x2*tmp_x2);
				x2_sum+=tmp_x2;
				x2lny_sum+=(tmp_x2*log(tmp_data));
				nn+=1.0;
			}
		}
		a_value=(lny_sum*x4_sum - x2_sum*x2lny_sum)/(nn*x4_sum - x2_sum*x2_sum);
		b_value=(nn*x2lny_sum - x2_sum*lny_sum)/(nn*x4_sum - x2_sum*x2_sum);
		// end gaussian fitting.
		///////////////////////////////////////////////////////////////////////////////////////
		if(b_value>0) b_value*=-1;
		
		m_nSigma=sqrt(-1/(2*b_value));
		
		m_nDPeak=exp(a_value);
		
		for(i=m_nStartDataLoc; i<=m_nEndDataLoc ;i+=2)
		{
			if(nData[i] >=m_cSettingData.nRimInspect)
			{
				result[i]=LDGaussian(m_nSigma,(double)i,(double)j);
				error_sum+=((nData[i]-result[i])*(nData[i]-result[i]));
			}
		}
		if(j==m_nStartDataLoc)
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
	if(wcc==0)
		m_nSigma=min_sigma+ GetDistanceToPixel(m_cSettingData.nWCorrect2);
	else       
		m_nSigma=min_sigma+ GetDistanceToPixel(wcc) ;

//	m_nSigma=min_sigma;
	m_nDCx=min_cp;
	m_nDPeak=min_peak;

	m_nMaxIntensity=LDGaussian(m_nSigma,(double)m_nDCx,(double)m_nDCx);//max_intensity;
    if(m_nLow<0) m_nLow=0;
	if(m_nHigh>639) m_nHigh=639;

	m_nLowValue=LDGaussian(m_nSigma,(double)m_nLow,(double)m_nDCx);//m_nValue[m_nLow];
	m_nHighValue=LDGaussian(m_nSigma,(double)m_nHigh,(double)m_nDCx);//m_nValue[m_nHigh];

	sprintf(buf,"%d, %d",m_nLow,m_nHigh);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(3,0,buf);


	nMinValue=(m_nLowValue< m_nHighValue) ? m_nLowValue:m_nHighValue;
	m_nHRimValue=(nMinValue/m_nMaxIntensity)*100.0;

	m_nRimHighValue=(m_nHighValue/m_nMaxIntensity)*100.0;
	m_nRimLowValue=(m_nLowValue/m_nMaxIntensity)*100.0;
	sprintf(buf,"%6.3f",m_nHRimValue);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(3,2,buf);

	m_nGaussianDrawTag=1;
	return;
}


void CHProjection::RimLocate(int nValue)
{
	char buf[256];
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	double nMinValue;


	m_nLow=m_nOriginLow-nValue;
	m_nHigh=m_nOriginHigh+nValue;

    if(m_nLow<0) m_nLow=0;
	if(m_nHigh>639) m_nHigh=639;

	m_nLowValue=LDGaussian(m_nSigma,(double)m_nLow,(double)m_nDCx);//m_nValue[m_nLow];
	m_nHighValue=LDGaussian(m_nSigma,(double)m_nHigh,(double)m_nDCx);//m_nValue[m_nHigh];

	sprintf(buf,"%d, %d",m_nLow,m_nHigh);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(3,1,buf);

	nMinValue=(m_nLowValue < m_nHighValue) ? m_nLowValue:m_nHighValue;
	m_nHRimValue=(nMinValue/m_nMaxIntensity)*100.0;

	m_nRimHighValue=(m_nHighValue/m_nMaxIntensity)*100.0;
	m_nRimLowValue=(m_nLowValue/m_nMaxIntensity)*100.0;
	sprintf(buf,"%6.3f",m_nHRimValue);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(3,3,buf);


	return;
}

void CHProjection::MakeColor()
{
	int i;
	int rr,gg,bb;

	for(i=0; i<256; i++)
	{
//		rr=-(i-255);
//		if(i<128) gg=2*i ;
//		else      gg=-2*(i-255);
//		bb=i;

		//sun if(i<140) 
		if(i<m_cSettingDataEx.iApcOk)
		{ 
			rr=255;gg=0;bb=0;
		}
		//sun else if(i<240)
		else if(i<m_cSettingDataEx.iApcOkHigh)
		{
			rr=0;gg=0;bb=255;
		}
		else
		{
			rr=255;gg=0;bb=0;
		}
		m_cColorIndex[i].rgbRed =(BYTE)rr;
		m_cColorIndex[i].rgbGreen =(BYTE)gg;
		m_cColorIndex[i].rgbBlue =(BYTE)bb;
		m_cColorIndex[i].rgbReserved =(BYTE)0;

	}
}
		

void CHProjection::DrawLevel(CDC *pDC,int v)
{
   CDisplayToGDI cDisp;
   int i;
   POINT p1, p2;



   for(i=0; i<v; i++)
   {
	   p1.x=2;
	   p1.y=255-i;

	   p2.x=32;
	   p2.y=255-i;
	   cDisp.DLine(pDC,p1,p2,RGB(m_cColorIndex[i].rgbRed,
							     m_cColorIndex[i].rgbGreen,
								 m_cColorIndex[i].rgbBlue));
   }
   RECT r;
   r.left=2;
   r.right=32;
   r.top=255-v;
   r.bottom=255;

   cDisp.DRect(pDC,r,RGB(255,0,0));
}

void CHProjection::DrawGaussian(CDC *pDC)
{
	int i;
	CDisplayToGDI cGDI;
	usrPOINT pt,prev;
	char buf[256];
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	double nGauss;
	double nMaxGauss=0;
	double nDeviation = 0.0;
	
	for(i=0; i<640; i++)
	{
		nGauss=LDGaussian(m_nSigma,(double)i,(double)m_nDCx);
		pt.x=(double)i;
		pt.y=256-nGauss;
		if(pt.y < 0) pt.y=0;
		if(i>0)
		{
			cGDI.DLine(pDC,prev,pt,RGB(255,0,0));
		}
		memcpy(&prev,&pt,sizeof(usrPOINT));
		if(nGauss>nMaxGauss)
			nMaxGauss=nGauss;
	}
	
	for(i=0; i<640; i++)
	{
		pt.x=(double)i;
		pt.y=256.0-m_nValue[i];
		if(i>0)
		{
			cGDI.DLine(pDC,prev,pt,RGB(128,255,0));
		}
		memcpy(&prev,&pt,sizeof(usrPOINT));
	}
	
	prev.x=0;
	prev.y=255-m_cSettingDataEx.iApcOkHigh ; //25;
	pt.x=640;
	pt.y=255-m_cSettingDataEx.iApcOkHigh;
	cGDI.DLine(pDC,prev,pt,RGB(255,255,255));
	
	prev.y=255-m_cSettingDataEx.iApcOk ; //115;
	pt.y=255-m_cSettingDataEx.iApcOk ; //115;
	cGDI.DLine(pDC,prev,pt,RGB(255,255,255));
	
	prev.x=m_nLow;
	prev.y=240;
	pt.x=m_nLow;
	pt.y=0;
	cGDI.DLine(pDC,prev,pt,RGB(255,0,255));
	
	prev.x=0;
	prev.y=256-m_cSettingData.nRimInspect;
	pt.x=640;
	pt.y=256-m_cSettingData.nRimInspect;
	
	cGDI.DDotLine(pDC,prev,pt,RGB(200,200,200));
	
	
	prev.x=m_nHigh;
	prev.y=240;
	pt.x=m_nHigh;
	pt.y=0;
	cGDI.DLine(pDC,prev,pt,RGB(255,0,255));
	// display data threshold
	prev.x=m_nStartDataLoc;
	prev.y=240;
	pt.x=m_nStartDataLoc;
	pt.y=0;
	
	cGDI.DDotLine(pDC,prev,pt,RGB(0,0,255));
	prev.x=m_nEndDataLoc;
	prev.y=240;
	pt.x=m_nEndDataLoc;
	pt.y=0;
	
	cGDI.DDotLine(pDC,prev,pt,RGB(0,0,255));
	// display data center
	prev.x=m_nDCx;
	prev.y=0.0;
	pt.x=m_nDCx;
	pt.y=240.0;
	cGDI.DLine(pDC,prev,pt,RGB(255,0,0));
	
	prev.x=(double)m_cBCpt.x;
	prev.y=0.0;
	pt.x=(double)m_cBCpt.x;
	pt.y=240.0;
	cGDI.DLine(pDC,prev,pt,RGB(0,255,0));			
	
	if(m_pView->m_wTabWindow.GetSetupActive())
	{
		cGDI.DTextOutWithFontII(pDC,10,40,10,18,"HORIZONTAL RIM",RGB(255,255,0));
		sprintf(buf," : %6.2f%%",m_nHRimValue);
		cGDI.DTextOutWithFontII(pDC,180,40,10,18,buf,RGB(255,255,0));
		
		cGDI.DTextOutWithFontII(pDC,50,60,10,18,"LOW",RGB(255,0,0));
		sprintf(buf," : %6.2f%%",m_nRimLowValue);
		cGDI.DTextOutWithFontII(pDC,100,60,10,18,buf,RGB(255,255,255));
		
		cGDI.DTextOutWithFontII(pDC,50,80,10,18,"HIGH",RGB(255,0,0));
		sprintf(buf," : %6.2f%%",m_nRimHighValue);
		cGDI.DTextOutWithFontII(pDC,100,80,10,18,buf,RGB(255,255,255));
	}
	else
	{
		//141 - 240
		cGDI.DTextOutWithFontII(pDC,50,40,10,18,"HORIZONTAL MAX",RGB(255,255,0));
		sprintf(buf," : %6.2f",nMaxGauss);
		cGDI.DTextOutWithFontII(pDC,220,40,10,18,buf,RGB(0,255,0));		
				
		DrawLevel(pDC,(int)nMaxGauss);
	}	

    if(g_bLeftRight) {//Right
	    cGDI.DTextOutWithFontII(pDC,480,5,12,20,"Right Table",RGB(255,255,255));
	}
	else { //Left
        cGDI.DTextOutWithFontII(pDC,480,5,12,20,"Left Table",RGB(255,255,255)); 
	}
	

	if(g_iTestMode == tmNormal)
	{
		if(nMaxGauss >= m_cSettingDataEx.iApcOk && nMaxGauss<=m_cSettingDataEx.iApcOkHigh )
		{
			cGDI.DTextOutWithFontII(pDC,500,20,25,50,"OK",RGB(0,255,0));		
		}
		else
		{
			cGDI.DTextOutWithFontII(pDC,500,20,25,50,"NG",RGB(255,0,0));
		}
	}
	
	//gMotionResultData.dHosizontalCMD = CENTER_OF_GAUSSIAN_HIGH_LOW - nMaxGauss;	

/*
	sprintf(buf,"MaxGauss : %6.2f",nMaxGauss);
	m_pView -> m_wDisplayComm.AddListData(buf);

	sprintf(buf,"Center : %6.2f",CENTER_OF_GAUSSIAN_HIGH_LOW);
	m_pView -> m_wDisplayComm.AddListData(buf);

	sprintf(buf,"CMD : %6.2f",gMotionResultData.dHosizontalCMD);
	m_pView -> m_wDisplayComm.AddListData(buf);
*/
	return;
}


void  CHProjection::SortValue(double *pArr, int d, int h, BOOL bAscending)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int i,j;
	double str;

	i = h;
	j = d;

	str= pArr[((int) ((d+h) / 2))];

	do {

		if (bAscending) {
			while (pArr[j] < str) j++;
			while (pArr[i] > str) i--;
		} else {
			while (pArr[j] > str) j++;
			while (pArr[i] < str) i--;
		}

		if ( i >= j ) {

			if ( i != j ) {
				double zal;
				zal=pArr[i];
				pArr[i]=pArr[j];
				pArr[j]=zal;
			}

			i--;
			j++;
		}
	} while (j <= i);

	if (d < i) SortValue(pArr,d,i,bAscending);
	if (j < h) SortValue(pArr,j,h,bAscending);

	return;
}
