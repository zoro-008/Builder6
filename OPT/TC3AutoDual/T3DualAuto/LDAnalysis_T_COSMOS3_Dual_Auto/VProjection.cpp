// VProjection.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "MainFrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "VProjection.h"
#include "..\\guisrc\\DisplayToGDI.h"
#include <math.h>
#include "External.h"
#include "HVSystemDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define G_SCALE 256
/////////////////////////////////////////////////////////////////////////////
// CVProjection dialog


CVProjection::CVProjection(CWnd* pParent /*=NULL*/)
	: CDialog(CVProjection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVProjection)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nWidth=256;
	m_nHeight=480;
	m_nScaleFactor=8.7/480.0;
	m_nBWidthCriteria=5.0/m_nScaleFactor;
	m_nCx=320.0;
	m_nCy=240.0;
	m_nGaussianDrawTag=0;
	m_nOriginLow=0;
	m_nOriginHigh=0;
	m_nLow=0;
	m_nHigh=0;
	m_nVRimValue=0.0;
}


void CVProjection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVProjection)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVProjection, CDialog)
	//{{AFX_MSG_MAP(CVProjection)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVProjection message handlers

BOOL CVProjection::PreTranslateMessage(MSG* pMsg) 
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

void CVProjection::OnDestroy() 
{
	CDialog::OnDestroy();
	if(m_pBitmapBuffer!=NULL) delete[] m_pBitmapBuffer;	
}

BOOL CVProjection::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BitmapInit();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVProjection::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawGrid(&dc);			
}

void CVProjection::PaintEx()
{
    OnPaint() ;
}

void CVProjection::BitmapInit()
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

void CVProjection::DrawGrid(CPaintDC *dc)
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

	if(m_nGaussianDrawTag==1)	DrawGaussian(&m_MemDC);
	dc->BitBlt( 0,0,m_nWidth,m_nHeight,&m_MemDC,0,0,SRCCOPY);
}



void CVProjection::SetParam()
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();	

	memcpy(&m_cSettingData,&m_pView->m_cSettingData,sizeof(SETTING_DATA));

	m_nScaleFactor=((double)8.91/(double)640.0)+(double)m_cSettingData.nBPixel/10000.0;
	return;
}

double CVProjection::NormGaussian(double s,double v,double m) 
{
	return exp(-(v-m)*(v-m)/(2*s*s))/(m_nSigma*sqrt(2*PI_VALUE));
}

double CVProjection::LDGaussian(double s,double v,double m) 
{
	return m_nDPeak*exp(-(v-m)*(v-m)/(2*s*s));
}



void CVProjection::SetGaussianData(unsigned char *pIn,POINT cpt,POINT bcpt,int maxv,int low, int high)
{
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	double nTmpValue[480];

	memcpy(&m_cBCpt,&bcpt,sizeof(POINT));
	m_nDWidth=(double)(high-low) ;
	m_nOriginLow=low;
	m_nOriginHigh=high;
    m_nLow=low-m_cSettingData.nRimLocate;
	m_nHigh=high+m_cSettingData.nRimLocate;
	if(m_nLow<0) m_nLow=0;
	if(m_nHigh>479) m_nHigh=479;
	m_nDCx=cpt.x;
	m_nDCy=cpt.y;
	memcpy(m_nValue,pIn,480);
////////////////
	int    count=0;
	int i;
	int offset;
	
	m_nDataThr=m_cSettingData.nDataThr;
	offset=(int)GetDistanceToPixel(m_nDataThr/2.0);
//	m_nStartDataLoc=m_nDCy-offset;
//	m_nEndDataLoc=m_nDCy+offset;

	m_nStartDataLoc=m_nOriginLow-offset;
	m_nEndDataLoc=m_nOriginHigh+offset;

	if(m_nStartDataLoc <1) m_nStartDataLoc=1;
	if(m_nStartDataLoc >479) m_nStartDataLoc=479;
	if(m_nEndDataLoc <1) m_nEndDataLoc=1;
	if(m_nEndDataLoc >479) m_nEndDataLoc=479;
    
	if(m_nStartDataLoc>=m_nEndDataLoc) return;

	for(i=0; i<=479; i++)
	{
		nTmpValue[count]=(double)m_nValue[i];
        count++;
	}
	if(count>0)
		ComputeData(nTmpValue,count,0);

	Invalidate(FALSE);
	return;
}

void CVProjection::ReCompute(double datathr,double wcorrect)
{
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	double nTmpValue[480];
	int count=0;
	int i;
	int offset;

	m_nDataThr=datathr;
	offset=(int)GetDistanceToPixel(m_nDataThr/2.0);
//	m_nStartDataLoc=m_nDCy-offset;
//	m_nEndDataLoc=m_nDCy+offset;

	m_nStartDataLoc=m_nOriginLow-offset;
	m_nEndDataLoc=m_nOriginHigh+offset;

	if(m_nStartDataLoc <1) m_nStartDataLoc=1;
	if(m_nStartDataLoc >479) m_nStartDataLoc=479;
	if(m_nEndDataLoc <1) m_nEndDataLoc=1;
	if(m_nEndDataLoc >479) m_nEndDataLoc=479;

	if(m_nStartDataLoc>=m_nEndDataLoc) return;

	for(i=0; i<=479; i++)
	{
		nTmpValue[count]=(double)m_nValue[i];
        count++;
	}
	if(count >0)
		ComputeData(nTmpValue,count,0,wcorrect);
	return;
}

void CVProjection::ComputeData(double *nData,int count,int flag,double wcc)
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
//////////////////////////////////////////////////////////////////////////////
//	for(i=m_nStartDataLoc; i<=m_nEndDataLoc ;i++)
//	{
//		tmp_data=nData[i];
//		tmp_x2=((double)i-tct)*((double)i-tct);
//		x4y_sum+=( tmp_x2*tmp_x2*tmp_data );
//		ylny_sum+=(tmp_data*log(tmp_data) );
//		x2y_sum+=( tmp_x2*tmp_data );
//		x2ylny_sum+=( tmp_x2*tmp_data*log(tmp_data) );
//		y_sum+=tmp_data;
//	}
//	a_value=(x4y_sum*ylny_sum - x2y_sum*x2ylny_sum)/(y_sum*x4y_sum - x2y_sum*x2y_sum);
//	b_value=(y_sum*x2ylny_sum -x2y_sum*ylny_sum)/(y_sum*x4y_sum - x2y_sum*x2y_sum);

//////////////////////////////////////////////////////////////////////////////////
	double lny_sum=0,x4_sum=0,x2_sum=0,x2lny_sum=0;
	double error_sum=0;
    double nn=0;
	double result[480];
    int j;
	double min_err,min_sigma,min_cp,min_peak;

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
			if(tmp_data >= m_cSettingData.nRimInspect)
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
	
		m_nSigma=sqrt(-1/(2*b_value)) ;
		m_nDPeak=exp(a_value);
		
		for(i=m_nStartDataLoc; i<=m_nEndDataLoc ;i+=2)
		{
			if(nData[i] >= m_cSettingData.nRimInspect)
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
		m_nSigma=min_sigma+ GetDistanceToPixel(m_cSettingData.nWCorrect) ;
	else
        m_nSigma=min_sigma+ GetDistanceToPixel(wcc) ;

	m_nDCy=min_cp;
	m_nDPeak=min_peak;

////////////////////////
//	char bufff[256];
//	sprintf(bufff,"a=%f, b=%f",a_value, b_value);
//	m_pView->m_wDisplayComm.AddListData(bufff);
//	sprintf(bufff,"s=%f,p=%f",m_nSigma,m_nDPeak);
//	m_pView->m_wDisplayComm.AddListData(bufff);

//	sprintf(bufff,"s=%d,e=%d",m_nStartDataLoc,m_nEndDataLoc);
//	m_pView->m_wDisplayComm.AddListData(bufff);
/////////////////////////////////////////////////////////////////////////
	m_nMaxIntensity=LDGaussian(m_nSigma,m_nDCy,m_nDCy);//max_intensity;
  /////////////////
	if(m_nLow <0) m_nLow=0;
	if(m_nHigh >479) m_nHigh=479;

	m_nLowValue=LDGaussian(m_nSigma,m_nLow,m_nDCy);//m_nValue[m_nLow];
	m_nHighValue=LDGaussian(m_nSigma,m_nHigh,m_nDCy);//m_nValue[m_nHigh];
	sprintf(buf,"%d, %d",m_nLow,m_nHigh);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(3,1,buf);

	nMinValue=(m_nLowValue < m_nHighValue) ? m_nLowValue:m_nHighValue;
	m_nVRimValue=(nMinValue/m_nMaxIntensity)*100.0;
	sprintf(buf,"%6.3f",m_nVRimValue);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(3,3,buf);

	m_nRimLowValue=(m_nLowValue/m_nMaxIntensity)*100.0;
	m_nRimHighValue=(m_nHighValue/m_nMaxIntensity)*100.0;


	m_nGaussianDrawTag=1;
	return;
}



void CVProjection::RimLocate(int nValue)
{
	char buf[256];
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	double nMinValue;
//	double max_intensity;


	m_nLow=m_nOriginLow-nValue;
	m_nHigh=m_nOriginHigh+nValue;
	if(m_nLow <0) m_nLow=0;
	if(m_nHigh >479) m_nHigh=479;

	m_nLowValue=LDGaussian(m_nSigma,m_nLow,m_nDCy);//nValue[m_nLow];
	m_nHighValue=LDGaussian(m_nSigma,m_nHigh,m_nDCy);//nValue[m_nHigh];

	sprintf(buf,"%d, %d",m_nLow,m_nHigh);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(3,1,buf);


	nMinValue=(m_nLowValue < m_nHighValue) ? m_nLowValue:m_nHighValue;
	m_nVRimValue=(nMinValue/m_nMaxIntensity)*100.0;
	
	m_nRimLowValue=(m_nLowValue/m_nMaxIntensity)*100.0;
	m_nRimHighValue=(m_nHighValue/m_nMaxIntensity)*100.0;

	sprintf(buf,"%6.3f",m_nVRimValue);
	m_pView->m_wTabWindow.m_pwCurrent->SetListData(3,3,buf);


	return;
}
void CVProjection::MakeColor()
{
	int i;
	int rr,gg,bb;

	for(i=0; i<256; i++)
	{
//		rr=-(i-255);
//		if(i<128) gg=2*i ;
//		else      gg=-2*(i-255);
//		bb=i;

		if(i<140) 
		{ 
			rr=255;gg=0;bb=0;
		}
		else if(i<240)
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
		

void CVProjection::DrawLevel(CDC *pDC,int v)
{
   CDisplayToGDI cDisp;
   int i;
   POINT p1, p2;



   for(i=0; i<v; i++)
   {
	   p1.x=i;
	   p1.y=2;

	   p2.x=i;
	   p2.y=32;
	   cDisp.DLine(pDC,p1,p2,RGB(m_cColorIndex[i].rgbRed,
							     m_cColorIndex[i].rgbGreen,
								 m_cColorIndex[i].rgbBlue));
   }
   RECT r;
   r.left=0;
   r.right=v;
   r.top=2;
   r.bottom=32;

   cDisp.DRect(pDC,r,RGB(255,0,0));
}

void CVProjection::DrawGaussian(CDC *pDC)
{

	int i;
	CDisplayToGDI cGDI;
	usrPOINT pt,prev;
	char buf[256];
    CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	double nGauss;
	double nMaxGauss=0;

	for(i=0; i<480; i++)
	{
		nGauss=LDGaussian(m_nSigma,i,m_nDCy);
		pt.y=(double)i;
		pt.x=nGauss;

		if(i>0)
		{
			cGDI.DLine(pDC,prev,pt,RGB(255,0,0));
		}
		memcpy(&prev,&pt,sizeof(usrPOINT));
		if(nGauss>nMaxGauss)
			nMaxGauss=nGauss;
	}

	for(i=0; i<480; i++)
	{
		pt.y=(double)i;
		pt.x=m_nValue[i];
		if(i>0)
		{
			cGDI.DLine(pDC,prev,pt,RGB(128,255,0));
		}
		memcpy(&prev,&pt,sizeof(usrPOINT));
	}
////////////////////////////////////
	prev.x=140.0;
	prev.y=0.0;
	pt.x=140.0;
	pt.y=479;
	cGDI.DLine(pDC,prev,pt,RGB(255,255,255));
	prev.x=230.0;
	prev.y=0.0;
	pt.x=230.0;
	pt.y=479;
	cGDI.DLine(pDC,prev,pt,RGB(255,255,255));
////////////////////////////////////

	prev.x=255.0;
	prev.y=m_nLow;
	pt.x=0.0;
	pt.y=m_nLow;
	cGDI.DLine(pDC,prev,pt,RGB(255,0,255));

	prev.x=m_cSettingData.nRimInspect;
	prev.y=0;
	pt.x=m_cSettingData.nRimInspect;
	pt.y=480;
	cGDI.DDotLine(pDC,prev,pt,RGB(200,200,200));


	prev.x=255.0;
	prev.y=m_nHigh;
	pt.x=0;
	pt.y=m_nHigh;
	cGDI.DLine(pDC,prev,pt,RGB(255,0,255));
// display data threshold
	prev.x=255.0;
	prev.y=m_nStartDataLoc;
	pt.x=0;
	pt.y=m_nStartDataLoc;
	cGDI.DDotLine(pDC,prev,pt,RGB(0,0,255));
	prev.x=255.0;
	prev.y=m_nEndDataLoc;
	pt.x=0;
	pt.y=m_nEndDataLoc;
	cGDI.DDotLine(pDC,prev,pt,RGB(0,0,255));
// display data center
	prev.x=0.0;
	prev.y=m_nDCy;
	pt.x=255;
	pt.y=m_nDCy;
	cGDI.DLine(pDC,prev,pt,RGB(255,0,0));

	prev.x=0.0;
	prev.y=(double)m_cBCpt.y;
	pt.x=255;
	pt.y=(double)m_cBCpt.y;
	cGDI.DLine(pDC,prev,pt,RGB(0,255,0));
	
	gMotionResultData.dVerticalCMD = (m_pView->m_cSettingDataEx.iApcOk + m_pView->m_cSettingDataEx.iApcOkHigh)/2 - nMaxGauss;		

	if(m_pView->m_wTabWindow.GetSetupActive())
	{
		
		cGDI.DTextOutWithFontII(pDC,10,10,10,18,"VERTICAL RIM",RGB(255,255,0));
		sprintf(buf," : %6.2f%%",m_nVRimValue);
		cGDI.DTextOutWithFontII(pDC,150,10,10,18,buf,RGB(255,255,0));
		
		cGDI.DTextOutWithFontII(pDC,100,40,10,18,"LOW",RGB(255,0,0));	
		sprintf(buf," : %6.2f%%",m_nRimLowValue);
		cGDI.DTextOutWithFontII(pDC,150,40,10,18,buf,RGB(255,255,255));
		
		cGDI.DTextOutWithFontII(pDC,100,60,10,18,"HIGH",RGB(255,0,0));	
		sprintf(buf," : %6.2f%%",m_nRimHighValue);
		cGDI.DTextOutWithFontII(pDC,150,60,10,18,buf,RGB(255,255,255));
	}
	else
	{
		cGDI.DTextOutWithFontII(pDC,10,40,10,18,"VERTICAL MAX",RGB(255,255,0));
		sprintf(buf," : %6.2f",nMaxGauss);
		cGDI.DTextOutWithFontII(pDC,150,40,10,18,buf,RGB(0,255,0));
		

//		cGDI.DTextOutWithFontII(pDC,100,80,10,18,"LOW",RGB(255,0,0));	
//		cGDI.DTextOutWithFontII(pDC,150,80,10,18," :140.00",RGB(255,255,255));

//		cGDI.DTextOutWithFontII(pDC,100,100,10,18,"HIGH",RGB(255,0,0));	
//		cGDI.DTextOutWithFontII(pDC,150,100,10,18," :230.00",RGB(255,255,255));
		
		//gMotionResultData.dVerticalCMD = CENTER_OF_GAUSSIAN_HIGH_LOW - nMaxGauss;		
		
		if(g_iTestMode == tmNormal)
		{
			if(nMaxGauss >= 140 && nMaxGauss<=230)
			{
				
				cGDI.DTextOutWithFontII(pDC,150,400,25,50,"OK",RGB(0,255,0));
				gMotionResultData.nGauVirOkNg = VISION_OK;			
			}
			else
			{
				cGDI.DTextOutWithFontII(pDC,150,400,25,50,"NG",RGB(255,0,0));
				gMotionResultData.nGauHorOkNg = VISION_NG;			
			}
		}
		DrawLevel(pDC,(int)nMaxGauss);
	}
	return;

}

void  CVProjection::SortValue(double *pArr, int d, int h, BOOL bAscending)
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
