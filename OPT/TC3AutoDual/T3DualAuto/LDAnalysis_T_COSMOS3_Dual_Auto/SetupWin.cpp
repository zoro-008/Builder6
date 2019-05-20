// SetupWin.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "Mainfrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "SetupWin.h"
#include "PasswdModify.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupWin dialog


CSetupWin::CSetupWin(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupWin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupWin)
	m_nDeckThr = 128;
	m_nDispType = 1;
	m_nErrorLimit =-3.0;
	m_nRDir = 0;
	m_nTDir = 0;
	m_nMinute = 80;
	m_nPixel = 476;
	m_nBDispMode = 0;
	m_nBThr = 50;
	m_nLocation = 10.0;
	m_nParallel = 50.0;
	m_nTiltCenter = 5.0;
	m_nTiltHigh = 0.01;
	m_nTiltLow = -0.01;
	m_nDiameterCenter = 5.0;
	m_nDiameterHigh = 0.01;
	m_nDiameterLow = -0.01;
	m_nBPixel = 0;
	m_nBSize = 5.0;
	m_nRimHP = 75.0;
	m_nPThr = 100;
	m_nRimInspect = 60;
	m_nWCorrect = 0.0;
	m_nRimLocate = 0;
	m_nRimVP = 0;
	m_nOlSize = 4.0;
	m_nPDist = 0.17;
	m_nWCorrect2 = 0.0;
	m_nDataThr = 0.6;
	m_nPDist2 = 0.17;
	m_nRErrorLimitHigh=3.0;
	m_nTErrorLimitLow=-3.0;
	m_nTErrorLimitHigh=3.0;

    m_nRErrorLimitLow2=-3.0;
	m_nRErrorLimitHigh2=3.0;
	m_nTErrorLimitLow2=-3.0;
	m_nTErrorLimitHigh2=3.0;








	//}}AFX_DATA_INIT
	int i,j;

	for(j=0; j<11; j++)
		for(i=0; i<2; i++)
		{
			m_pwStatic[j][i]=NULL;
			m_pwEdit[j][i]=NULL;
			m_pwSpin[j][i]=NULL;
		}


	for(j=0; j<2; j++)
	{
		m_pwStaticEx[j]=NULL;
		for(i=0; i<3; i++)
		{

			m_pwEditEx[j][i]=NULL;
			m_pwSpinEx[j][i]=NULL;
		}
	}
	m_pwWcEdit=NULL;
	m_pwWcSpin=NULL;
	m_pwPdEdit=NULL;
	m_pwPdSpin=NULL;

	m_pwRErrorHighEdit=NULL;
	m_pwRErrorHighSpin=NULL;
	m_pwTErrorLowEdit=NULL;
	m_pwTErrorLowSpin=NULL;
	m_pwTErrorHighEdit=NULL;
	m_pwTErrorHighSpin=NULL;


	m_pwRT2           = NULL;
	m_pwRErrorLowEdit2=NULL;
	m_pwRErrorLowSpin2=NULL;
	m_pwRErrorHighEdit2=NULL;
	m_pwRErrorHighSpin2=NULL;
	m_pwTErrorLowEdit2=NULL;
	m_pwTErrorLowSpin2=NULL;
	m_pwTErrorHighEdit2=NULL;
	m_pwTErrorHighSpin2=NULL;

	for(i=0; i<3; i++) m_pwButton[i]=NULL;
	for(i=0; i<8; i++) m_pwCheckButton[i]=NULL;

	m_pwDefaultButton=NULL;

	pBmpRgn=NULL;

	m_nASt=0;
	m_nLSt=0;
	m_nPSt=0;
	m_nHRSt=0;
	m_nVRSt=0;
	m_nDSt=0;
	m_nTSt=0;
	m_nACenterFlag=0;
	m_nACX=0;
	m_nACY=0;
}


void CSetupWin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupWin)
	DDX_Text(pDX, IDC_EDIT_DECKTHR, m_nDeckThr);
	DDV_MinMaxInt(pDX, m_nDeckThr, 0, 255);
	DDX_Text(pDX, IDC_EDIT_DISPTYPE, m_nDispType);
	DDV_MinMaxInt(pDX, m_nDispType, 0, 2);
	DDX_Text(pDX, IDC_EDIT_ERROR, m_nErrorLimit);
	DDX_Text(pDX, IDC_EDIT_ERROR2, m_nRErrorLimitHigh);
	DDX_Text(pDX, IDC_EDIT_ERROR3, m_nTErrorLimitLow);
	DDX_Text(pDX, IDC_EDIT_ERROR4, m_nTErrorLimitHigh);
	DDX_Text(pDX, IDC_EDIT_ERROR5, m_nRErrorLimitLow2);
	DDX_Text(pDX, IDC_EDIT_ERROR6, m_nRErrorLimitHigh2);
	DDX_Text(pDX, IDC_EDIT_ERROR7, m_nTErrorLimitLow2);
	DDX_Text(pDX, IDC_EDIT_ERROR8, m_nTErrorLimitHigh2);
	DDX_Text(pDX, IDC_EDIT_RDIR, m_nRDir);
	DDV_MinMaxInt(pDX, m_nRDir, 0, 1);
	DDX_Text(pDX, IDC_EDIT_TDIR, m_nTDir);
	DDV_MinMaxInt(pDX, m_nTDir, 0, 1);
	DDX_Text(pDX, IDC_EDIT_MINUTE, m_nMinute);
	DDX_Text(pDX, IDC_EDIT_PIXEL, m_nPixel);
	DDX_Text(pDX, IDC_EDIT_BDISP, m_nBDispMode);
	DDX_Text(pDX, IDC_EDIT_BTHR, m_nBThr);
	DDX_Text(pDX, IDC_EDIT_LOC, m_nLocation);
	DDX_Text(pDX, IDC_EDIT_PARA, m_nParallel);
	DDX_Text(pDX, IDC_EDIT_TILTC, m_nTiltCenter);
	DDX_Text(pDX, IDC_EDIT_TILTH, m_nTiltHigh);
	DDX_Text(pDX, IDC_EDIT_TILTL, m_nTiltLow);
	DDX_Text(pDX, IDC_EDIT_DIAC, m_nDiameterCenter);
	DDX_Text(pDX, IDC_EDIT_DIAH, m_nDiameterHigh);
	DDX_Text(pDX, IDC_EDIT_DIAL, m_nDiameterLow);
	DDX_Text(pDX, IDC_EDIT_BPIXEL, m_nBPixel);
	DDX_Text(pDX, IDC_EDIT_BSIZE, m_nBSize);
	DDX_Text(pDX, IDC_EDIT_RIMP, m_nRimHP);
	DDX_Text(pDX, IDC_EDIT_PTHR, m_nPThr);
	DDX_Text(pDX, IDC_EDIT_RIMINSPECT, m_nRimInspect);
	DDX_Text(pDX, IDC_EDIT_WIDTHCORRECT, m_nWCorrect);
	DDX_Text(pDX, IDC_EDIT_RIMLOCATE, m_nRimLocate);
	DDX_Text(pDX, IDC_EDIT_PDATA, m_nRimVP);
	DDX_Text(pDX, IDC_EDIT_OLSIZE, m_nOlSize);
	DDX_Text(pDX, IDC_EDIT_PDIST, m_nPDist);
	DDX_Text(pDX, IDC_EDIT_WIDTHCORRECT2, m_nWCorrect2);
	DDX_Text(pDX, IDC_EDIT_DATATHR, m_nDataThr);
	DDV_MinMaxDouble(pDX, m_nDataThr, -5., 5.);
	DDX_Text(pDX, IDC_EDIT_PDIST2, m_nPDist2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupWin, CDialog)
	//{{AFX_MSG_MAP(CSetupWin)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DISPTYPE, OnDeltaposSpinDisptype)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TDIR, OnDeltaposSpinTdir)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RDIR, OnDeltaposSpinRdir)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ERROR, OnDeltaposSpinError)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ERROR2, OnDeltaposSpinError2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ERROR3, OnDeltaposSpinError3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ERROR4, OnDeltaposSpinError4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DECKTHR, OnDeltaposSpinDeckthr)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MINUTE, OnDeltaposSpinMinute)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PIXEL, OnDeltaposSpinPixel)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_SAVE, OnButtonSetupSave)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_APPLY, OnButtonSetupApply)
	ON_BN_CLICKED(IDC_BUTTON_PASSWD, OnButtonPasswd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BDISP, OnDeltaposSpinBdisp)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BTHR, OnDeltaposSpinBthr)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIAC, OnDeltaposSpinDiac)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIAH, OnDeltaposSpinDiah)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIAL, OnDeltaposSpinDial)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LOC, OnDeltaposSpinLoc)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PARA, OnDeltaposSpinPara)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TILTC, OnDeltaposSpinTiltc)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TILTH, OnDeltaposSpinTilth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TILTL, OnDeltaposSpinTiltl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BSIZE, OnDeltaposSpinBsize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BPIXEL, OnDeltaposSpinBpixel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RIMP, OnDeltaposSpinRimp)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PTHR, OnDeltaposSpinPthr)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DATATHR, OnDeltaposSpinDatathr)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RIMINSPECT, OnDeltaposSpinRiminspect)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RIMLOCATE, OnDeltaposSpinRimlocate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WIDTHCORRECT, OnDeltaposSpinWidthcorrect)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_OLSIZE, OnDeltaposSpinOlsize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PDATA, OnDeltaposSpinPdata)
	ON_BN_CLICKED(IDC_CHECK_BUTTON_ALIMIT, OnCheckButtonAlimit)
	ON_BN_CLICKED(IDC_CHECK_BUTTON_DIAMETER, OnCheckButtonDiameter)
	ON_BN_CLICKED(IDC_CHECK_BUTTON_DISTANCE, OnCheckButtonDistance)
	ON_BN_CLICKED(IDC_CHECK_BUTTON_PARALLELISM, OnCheckButtonParallelism)
	ON_BN_CLICKED(IDC_CHECK_BUTTON_RIM, OnCheckButtonRim)
	ON_BN_CLICKED(IDC_CHECK_BUTTON_TILT, OnCheckButtonTilt)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PDIST, OnDeltaposSpinPdist)
	ON_BN_CLICKED(IDC_CHECK_BUTTON_PDIST, OnCheckButtonPdist)
	ON_BN_CLICKED(IDC_CHECK_BUTTON_RIM2, OnCheckButtonRim2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WIDTHCORRECT2, OnDeltaposSpinWidthcorrect2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PDIST2, OnDeltaposSpinPdist2)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_DEFAULT, OnButtonSetupDefault)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SPIN_ERROR5, OnOutofmemorySpinError5)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SPIN_ERROR6, OnOutofmemorySpinError6)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SPIN_ERROR7, OnOutofmemorySpinError7)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SPIN_ERROR8, OnOutofmemorySpinError8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupWin message handlers

BOOL CSetupWin::PreTranslateMessage(MSG* pMsg) 
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

void CSetupWin::OnDestroy() 
{
	CDialog::OnDestroy();
    int i,j;

	for(j=0; j<11; j++)
		for(i=0; i<2; i++)
		{
			if(m_pwStatic[j][i]!=NULL) delete m_pwStatic[j][i];
			if(m_pwEdit[j][i]!=NULL)   delete m_pwEdit[j][i];
			if(m_pwSpin[j][i]!=NULL)   delete m_pwSpin[j][i];
		}

	for(j=0; j<2; j++)
	{
		if(m_pwStaticEx[j]!=NULL) delete m_pwStaticEx[j];
		for(i=0; i<3; i++)
		{

			if(m_pwEditEx[j][i]!=NULL) delete m_pwEditEx[j][i];
			if(m_pwSpinEx[j][i]!=NULL) delete m_pwSpinEx[j][i];
		}
	}

	if(m_pwWcEdit!=NULL)	delete m_pwWcEdit;
	if(m_pwWcSpin!=NULL)    delete m_pwWcSpin;
	if(m_pwPdEdit!=NULL)    delete m_pwPdEdit;
	if(m_pwPdSpin!=NULL)	delete m_pwPdSpin;

	if(m_pwRErrorHighEdit!=NULL)	delete m_pwRErrorHighEdit;
	if(m_pwRErrorHighSpin!=NULL)	delete m_pwRErrorHighSpin;
	if(m_pwTErrorLowEdit!=NULL)	    delete m_pwTErrorLowEdit;
	if(m_pwTErrorLowSpin!=NULL)	    delete m_pwTErrorLowSpin;
	if(m_pwTErrorHighEdit!=NULL)	delete m_pwTErrorHighEdit;
	if(m_pwTErrorHighSpin!=NULL)	delete m_pwTErrorHighSpin;

	if(m_pwRT2           !=NULL)    delete m_pwRT2           ;
	if(m_pwRErrorLowEdit2!=NULL)	delete m_pwRErrorLowEdit2;
	if(m_pwRErrorLowSpin2!=NULL)	delete m_pwRErrorLowSpin2;
	if(m_pwRErrorHighEdit2!=NULL)	delete m_pwRErrorHighEdit2;
	if(m_pwRErrorHighSpin2!=NULL)	delete m_pwRErrorHighSpin2;
	if(m_pwTErrorLowEdit2!=NULL)	delete m_pwTErrorLowEdit2;
	if(m_pwTErrorLowSpin2!=NULL)	delete m_pwTErrorLowSpin2;
	if(m_pwTErrorHighEdit2!=NULL)	delete m_pwTErrorHighEdit2;
	if(m_pwTErrorHighSpin2!=NULL)	delete m_pwTErrorHighSpin2;

	for(i=0; i<3; i++) 	if(m_pwButton[i]!=NULL) delete m_pwButton[i];
	for(i=0; i<8; i++)  if(m_pwCheckButton[i]!=NULL) delete m_pwCheckButton[i];

	if(m_pwDefaultButton!=NULL) delete m_pwDefaultButton;

	delete pBmpRgn;	
}

BOOL CSetupWin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\SetupWindowU_256.bmp", 0x00ff00ff);
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
	m_nWindowHeight= pBmpRgn->GetBitmapHeight();	

	MakeGui();
	ReadSettingFile();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CSetupWin::MakeGui()
{
	int i,j;
	int nStHeight;
    char *fname=".\\guiimage\\st128x26_256.BMP";
	char *pTitle[11][2]=
	{
		{"MINUTE","PIXEL"},//0
		{"R-DIR" ,"T-DIR"},//1
		{"FA R/T","D-TYPE"},//2
		{"B-DIST","D-THR"},//3
		{"BS(mm)","BPIXEL"},//4
		{"B-DISP","B-THR"},//5
		{"P-THR" ,"OLSIZE"},//6
		{"DIS(mm)","PARA(%)"},//7
		{"WC(mm)","DTHR"},//8
		{"RIMH(%)","RIML"},//9
		{"RIMV(%)","RIMI"}//10

	};

	UINT nSIDs[11][2]=
	{
		{IDC_ST_MINUTE      ,IDC_ST_PIXEL},//0
		{IDC_ST_RDIR        ,IDC_ST_TDIR},//1
		{IDC_ST_ERROR       ,IDC_ST_DISPTYPE    },//2
		{IDC_ST_PDIST       ,IDC_ST_DECKTHR},//3
		{IDC_ST_BSIZE       ,IDC_ST_BPIXEL},//4
		{IDC_ST_BDISP       ,IDC_ST_BTHR},//5
		{IDC_ST_PTHR        ,IDC_ST_OLSIZE},//6
		{IDC_ST_LOC         ,IDC_ST_PARA},//7
		{IDC_ST_WIDTHCORRECT,IDC_ST_DATATHR},//8
		{IDC_ST_RIMP        ,IDC_ST_RIMLOCATE },//9
		{IDC_ST_PDATA       ,IDC_ST_RIMINSPECT}//10
	};

	UINT nEIDs[11][2]=
	{
		{IDC_EDIT_MINUTE      ,IDC_EDIT_PIXEL}, // 0
		{IDC_EDIT_RDIR        ,IDC_EDIT_TDIR},//1
		{IDC_EDIT_ERROR       ,IDC_EDIT_DISPTYPE},//2
		{IDC_EDIT_PDIST       ,IDC_EDIT_DECKTHR},//3
		{IDC_EDIT_BSIZE       ,IDC_EDIT_BPIXEL},//4
		{IDC_EDIT_BDISP       ,IDC_EDIT_BTHR},//5
		{IDC_EDIT_PTHR        ,IDC_EDIT_OLSIZE},//6
		{IDC_EDIT_LOC         ,IDC_EDIT_PARA},//7
		{IDC_EDIT_WIDTHCORRECT,IDC_EDIT_DATATHR},//8
		{IDC_EDIT_RIMP        ,IDC_EDIT_RIMLOCATE },//9
		{IDC_EDIT_PDATA       ,IDC_EDIT_RIMINSPECT}//10
	};

	UINT nSPIDs[11][2]=
	{
		{IDC_SPIN_MINUTE      ,IDC_SPIN_PIXEL},//0
		{IDC_SPIN_RDIR        ,IDC_SPIN_TDIR},//1
		{IDC_SPIN_ERROR       ,IDC_SPIN_DISPTYPE},//2
		{IDC_SPIN_PDIST       ,IDC_SPIN_DECKTHR },//3
		{IDC_SPIN_BSIZE       ,IDC_SPIN_BPIXEL},//4
		{IDC_SPIN_BDISP       ,IDC_SPIN_BTHR},//5
		{IDC_SPIN_PTHR        ,IDC_SPIN_OLSIZE},//6
		{IDC_SPIN_LOC         ,IDC_SPIN_PARA},//7
		{IDC_SPIN_WIDTHCORRECT,IDC_SPIN_DATATHR},//8
		{IDC_SPIN_RIMP        ,IDC_SPIN_RIMLOCATE },//9
		{IDC_SPIN_PDATA       ,IDC_SPIN_RIMINSPECT}//10
	};



	CRect cbRect[8];
	for(j=0; j<11; j++)
		for(i=0; i<2; i++)
		{
/////////////////////////////////////////////////////////////////
// CStaticSkin
			m_pwStatic[j][i]=new CStaticSkin;
			m_pwStatic[j][i]->SetBmpFile(fname);
			m_pwStatic[j][i]->SubclassDlgItem(nSIDs[j][i],this);
			m_pwStatic[j][i]->MoveWindow(30+178*i,2+26*j,128,26); 
			m_pwStatic[j][i]->OutTextXY(1,4,pTitle[j][i]);

/////////////////////////////////////////////////////////////////
// CEdit
			if(j==8 && i==0)
			{
				m_pwEdit[j][i]=new CEdit;
				m_pwEdit[j][i]->SubclassDlgItem(nEIDs[j][i],this);
				m_pwEdit[j][i]->MoveWindow(97+178*i,2+26*j,48,26); 

	/////////////////////////////////////////////////////////////////
	// CSpinButtonCtrl
				m_pwSpin[j][i]=new CSpinButtonCtrl;
				m_pwSpin[j][i]->SubclassDlgItem(nSPIDs[j][i],this);
				m_pwSpin[j][i]->MoveWindow(145+178*i,2+26*j,12,24); 

				m_pwWcEdit=new CEdit;
				m_pwWcEdit->SubclassDlgItem(IDC_EDIT_WIDTHCORRECT2,this);
				m_pwWcEdit->MoveWindow(157+178*i,2+26*j,36,26); 

	/////////////////////////////////////////////////////////////////
	// CSpinButtonCtrl
				m_pwWcSpin=new CSpinButtonCtrl;
				m_pwWcSpin->SubclassDlgItem(IDC_SPIN_WIDTHCORRECT2,this);
				m_pwWcSpin->MoveWindow(193+178*i,2+26*j,12,24); 
			}
			else if(j==3 && i==0)
			{
				m_pwEdit[j][i]=new CEdit;
				m_pwEdit[j][i]->SubclassDlgItem(nEIDs[j][i],this);
				m_pwEdit[j][i]->MoveWindow(97+178*i,2+26*j,48,26); 

	/////////////////////////////////////////////////////////////////
	// CSpinButtonCtrl
				m_pwSpin[j][i]=new CSpinButtonCtrl;
				m_pwSpin[j][i]->SubclassDlgItem(nSPIDs[j][i],this);
				m_pwSpin[j][i]->MoveWindow(145+178*i,2+26*j,12,24); 

				m_pwPdEdit=new CEdit;
				m_pwPdEdit->SubclassDlgItem(IDC_EDIT_PDIST2,this);
				m_pwPdEdit->MoveWindow(157+178*i,2+26*j,36,26); 

	/////////////////////////////////////////////////////////////////
	// CSpinButtonCtrl
				m_pwPdSpin=new CSpinButtonCtrl;
				m_pwPdSpin->SubclassDlgItem(IDC_SPIN_PDIST2,this);
				m_pwPdSpin->MoveWindow(193+178*i,2+26*j,12,24); 
			}
			else if(j==2 && i==0)
			{
				m_pwEdit[j][i]=new CEdit;
				m_pwEdit[j][i]->SubclassDlgItem(nEIDs[j][i],this);
				m_pwEdit[j][i]->MoveWindow(97+178*i,2+26*j,48,26); 

	/////////////////////////////////////////////////////////////////
	// CSpinButtonCtrl
				m_pwSpin[j][i]=new CSpinButtonCtrl;
				m_pwSpin[j][i]->SubclassDlgItem(nSPIDs[j][i],this);
				m_pwSpin[j][i]->MoveWindow(145+178*i,2+26*j,12,24); 

				m_pwRErrorHighEdit=new CEdit;
				m_pwRErrorHighEdit->SubclassDlgItem(IDC_EDIT_ERROR2,this);
				m_pwRErrorHighEdit->MoveWindow(157+178*i,2+26*j,40,26); 
				m_pwRErrorHighSpin=new CSpinButtonCtrl;
				m_pwRErrorHighSpin->SubclassDlgItem(IDC_SPIN_ERROR2,this);
				m_pwRErrorHighSpin->MoveWindow(197+178*i,2+26*j,12,24); 		

				m_pwTErrorLowEdit=new CEdit;
				m_pwTErrorLowEdit->SubclassDlgItem(IDC_EDIT_ERROR3,this);
				m_pwTErrorLowEdit->MoveWindow(209+178*i,2+26*j,40,26); 
				m_pwTErrorLowSpin=new CSpinButtonCtrl;
				m_pwTErrorLowSpin->SubclassDlgItem(IDC_SPIN_ERROR3,this);
				m_pwTErrorLowSpin->MoveWindow(249+178*i,2+26*j,12,24); 		

				m_pwTErrorHighEdit=new CEdit;
				m_pwTErrorHighEdit->SubclassDlgItem(IDC_EDIT_ERROR4,this);
				m_pwTErrorHighEdit->MoveWindow(261+178*i,2+26*j,40,26); 
				m_pwTErrorHighSpin=new CSpinButtonCtrl;
				m_pwTErrorHighSpin->SubclassDlgItem(IDC_SPIN_ERROR4,this);
				m_pwTErrorHighSpin->MoveWindow(301+178*i,2+26*j,12,24); 		
			}
			else if(j==2 && i==1)
			{
		    	m_pwStatic[j][i]->MoveWindow(30,2+26*9,128,26); 

				m_pwEdit[j][i]=new CEdit;
				m_pwEdit[j][i]->SubclassDlgItem(nEIDs[j][i],this);
				m_pwEdit[j][i]->MoveWindow(97,2+26*9,48,26); 

	/////////////////////////////////////////////////////////////////
	// CSpinButtonCtrl
				m_pwSpin[j][i]=new CSpinButtonCtrl;
				m_pwSpin[j][i]->SubclassDlgItem(nSPIDs[j][i],this);
				m_pwSpin[j][i]->MoveWindow(145,2+26*9,12,24); 

			}
			else
			{
				m_pwEdit[j][i]=new CEdit;
				m_pwEdit[j][i]->SubclassDlgItem(nEIDs[j][i],this);
				m_pwEdit[j][i]->MoveWindow(97+178*i,2+26*j,48,26); 

	/////////////////////////////////////////////////////////////////
	// CSpinButtonCtrl
				m_pwSpin[j][i]=new CSpinButtonCtrl;
				m_pwSpin[j][i]->SubclassDlgItem(nSPIDs[j][i],this);
				m_pwSpin[j][i]->MoveWindow(145+178*i,2+26*j,12,24); 

			}
/////////////////////////////////////////////////////////////////
			if(j==2 && i==0)	cbRect[0].SetRect(30+178*i-20,2+26*j+3,30+178*i,2+26*j+23);
			if(j==3 && i==0)    cbRect[6].SetRect(30+178*i-20,2+26*j+3,30+178*i,2+26*j+23);
			if(j==7 && i==0)	cbRect[1].SetRect(30+178*i-20,2+26*j+3,30+1758*i,2+26*j+23);
			if(j==7 && i==1)	cbRect[2].SetRect(30+178*i-20,2+26*j+3,30+178*i,2+26*j+23);
			if(j==9 && i==0)
			{
				cbRect[3].SetRect(30+178*i-20,2+26*j+3,30+178*i,2+26*j+23);
				m_pwStatic[j][i]->ShowWindow(SW_HIDE);
				m_pwEdit[j][i]->ShowWindow(SW_HIDE);
				m_pwSpin[j][i]->ShowWindow(SW_HIDE);
			}
			if(j==10 && i==0)   cbRect[7].SetRect(30+178*i-20,2+26*j+3,30+178*i,2+26*j+23);
		}
		nStHeight=2+26*j;
/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////
// extra static, edit, spin re-allocation
	char *pTitleEx[2]={"Beam S" ,"TILT" };
	UINT nSIDsEx[2]  ={IDC_ST_DIAMETER,IDC_ST_TILT};
	UINT nEIDsEx[2][3]=
	{
		{IDC_EDIT_DIAC ,IDC_EDIT_DIAL ,IDC_EDIT_DIAH},
		{IDC_EDIT_TILTC,IDC_EDIT_TILTL,IDC_EDIT_TILTH}
	};

	UINT nSPIDsEx[2][3]=
	{
		{IDC_SPIN_DIAC ,IDC_SPIN_DIAL ,IDC_SPIN_DIAH},
		{IDC_SPIN_TILTC,IDC_SPIN_TILTL,IDC_SPIN_TILTH}
	};
	int ofx=5;	
	for(j=0; j<2; j++)
	{
/////////////////////////////////////////////////////////////////
// CStaticSkin
		m_pwStaticEx[j]=new CStaticSkin;
		m_pwStaticEx[j]->SetBmpFile(fname);
		m_pwStaticEx[j]->SubclassDlgItem(nSIDsEx[j],this);
		m_pwStaticEx[j]->MoveWindow(30,nStHeight+26*j,128,26); 
		m_pwStaticEx[j]->OutTextXY(1,4,pTitleEx[j]);
/////////////////////////////////////////////////////////////////
		cbRect[4+j].SetRect(10,nStHeight+26*j+3,30,nStHeight+26*j+23);
/////////////////////////////////////////////////////////////////

		for(i=0; i<3; i++)
		{
/////////////////////////////////////////////////////////////////
// CEdit
			if(i>0) ofx=1;
			else    ofx=0;

			m_pwEditEx[j][i]=new CEdit;
			m_pwEditEx[j][i]->SubclassDlgItem(nEIDsEx[j][i],this);
			m_pwEditEx[j][i]->MoveWindow(97+ofx+61*i,nStHeight+26*j,48,26); 

/////////////////////////////////////////////////////////////////
// CSpinButtonCtrl
			m_pwSpinEx[j][i]=new CSpinButtonCtrl;
			m_pwSpinEx[j][i]->SubclassDlgItem(nSPIDsEx[j][i],this);
			m_pwSpinEx[j][i]->MoveWindow(145+ofx+61*i,nStHeight+26*j,12,24); 
		}
	}
	nStHeight=nStHeight+26+26;


    
            m_pwStatic[j][i]=new CStaticSkin;
			m_pwStatic[j][i]->SetBmpFile(fname);
			m_pwStatic[j][i]->SubclassDlgItem(IDC_ST_ERROR5,this);
			m_pwStatic[j][i]->MoveWindow(30+178*0,nStHeight,128,26); 
			m_pwStatic[j][i]->OutTextXY(1,4,"FA2 R/T");

				m_pwRErrorLowEdit2=new CEdit;
				m_pwRErrorLowEdit2->SubclassDlgItem(IDC_EDIT_ERROR5,this);
				m_pwRErrorLowEdit2->MoveWindow(97,nStHeight,48,26); 
				m_pwRErrorLowSpin2=new CSpinButtonCtrl;
				m_pwRErrorLowSpin2->SubclassDlgItem(IDC_SPIN_ERROR5,this);
				m_pwRErrorLowSpin2->MoveWindow(145,nStHeight,12,24); 

				m_pwRErrorHighEdit2=new CEdit;
				m_pwRErrorHighEdit2->SubclassDlgItem(IDC_EDIT_ERROR6,this);
				m_pwRErrorHighEdit2->MoveWindow(157,nStHeight,40,26); 
				m_pwRErrorHighSpin2=new CSpinButtonCtrl;
				m_pwRErrorHighSpin2->SubclassDlgItem(IDC_SPIN_ERROR6,this);
				m_pwRErrorHighSpin2->MoveWindow(197,nStHeight,12,24); 		

				m_pwTErrorLowEdit2=new CEdit;
				m_pwTErrorLowEdit2->SubclassDlgItem(IDC_EDIT_ERROR7,this);
				m_pwTErrorLowEdit2->MoveWindow(209,nStHeight,40,26); 
				m_pwTErrorLowSpin2=new CSpinButtonCtrl;
				m_pwTErrorLowSpin2->SubclassDlgItem(IDC_SPIN_ERROR7,this);
				m_pwTErrorLowSpin2->MoveWindow(249,nStHeight,12,24); 		

				m_pwTErrorHighEdit2=new CEdit;
				m_pwTErrorHighEdit2->SubclassDlgItem(IDC_EDIT_ERROR8,this);
				m_pwTErrorHighEdit2->MoveWindow(261,nStHeight,40,26); 
				m_pwTErrorHighSpin2=new CSpinButtonCtrl;
				m_pwTErrorHighSpin2->SubclassDlgItem(IDC_SPIN_ERROR8,this);
				m_pwTErrorHighSpin2->MoveWindow(301,nStHeight,12,24); 	





     nStHeight=nStHeight+26;



























/////////////////////////////////////////////////////////////////
	m_pwDefaultButton=new CxSkinButton ;
	m_pwDefaultButton->SubclassDlgItem(IDC_BUTTON_SETUP_DEFAULT, this);
	m_pwDefaultButton->SetToolTipText("");
	m_pwDefaultButton->SetSkin(IDB_DEFAULTN,IDB_DEFAULTD,IDB_DEFAULTN,0,0,IDB_OKNG_MASK,0,0,0);
	m_pwDefaultButton->MoveWindow(285,295,50,40);

/////////////////////////////////////////////////////////////////
// Inspection Enable/Disable check box construction
	UINT nCBIds[8]={
		IDC_CHECK_BUTTON_ALIMIT,
		IDC_CHECK_BUTTON_DISTANCE,
		IDC_CHECK_BUTTON_PARALLELISM,
		IDC_CHECK_BUTTON_RIM,
		IDC_CHECK_BUTTON_DIAMETER,
		IDC_CHECK_BUTTON_TILT,
		IDC_CHECK_BUTTON_PDIST,
		IDC_CHECK_BUTTON_RIM2
	};
	char *CBStr[8]=
	{
		"",
		"",
		"",
		"",
		"",
		"",
		""
	};
	char *strIns[8]={"A","L","P","R","S","T","H","R"};

	for(i=0;i<8; i++)
	{
		m_pwCheckButton[i]=new CxSkinButton;
		m_pwCheckButton[i]->SubclassDlgItem(nCBIds[i], this);
		m_pwCheckButton[i]->SetToolTipText(CBStr[i]);
		m_pwCheckButton[i]->SetSkin(IDB_IOOFF,IDB_IOON,IDB_IOOFF,0,0,IDB_IOMASK,0,0,0);
		m_pwCheckButton[i]->MoveWindow(&cbRect[i]);

		m_pwCheckButton[i]->SetTextColor(RGB(0,255,0));
		m_pwCheckButton[i]->SetWindowText(strIns[i]);
		if(i==3) {
			m_pwCheckButton[i]->SetCheck(0);
			m_pwCheckButton[i]->ShowWindow(SW_HIDE);
		}

	}


/////////////////////////////////////////////////////////////////
// Button Displaying
	UINT nBTNIDs[3]={IDC_BUTTON_SETUP_SAVE,IDC_BUTTON_SETUP_APPLY,IDC_BUTTON_PASSWD};
	UINT nIDBMPs[3][9]=
	{
		{IDB_SETUP_SAVEN ,IDB_SETUP_SAVED ,IDB_SETUP_SAVEU ,0,IDB_SETUP_SAVEU,IDB_SETUP_BTNMASK,0,0,0},
		{IDB_SETUP_APPLYN,IDB_SETUP_APPLYD,IDB_SETUP_APPLYU,0,IDB_SETUP_APPLYU,IDB_SETUP_BTNMASK,0,0,0},
		{IDB_PASSWDN,IDB_PASSWDD,0,0,0,IDB_SETUP_BTNMASK,0,0,0}
	};

	for(i=0; i<3; i++)
	{
		m_pwButton[i]=new CxSkinButton;
		m_pwButton[i]->SubclassDlgItem(nBTNIDs[i], this);
		m_pwButton[i]->SetToolTipText("");
		m_pwButton[i]->SetSkin(nIDBMPs[i][0],nIDBMPs[i][1],nIDBMPs[i][2],
			                     nIDBMPs[i][3],nIDBMPs[i][4],nIDBMPs[i][5],
							     nIDBMPs[i][6],nIDBMPs[i][7],nIDBMPs[i][8]);
		m_pwButton[i]->MoveWindow(30+100*i,nStHeight,80,40);
	}
	return;	
}


void CSetupWin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
}

void CSetupWin::GetSettingData()
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	UpdateData(TRUE);    
	m_cActiveData.nPDist           =m_nPDist;
	m_cActiveData.nPDist2          =m_nPDist2;
	m_cActiveData.nDeckThr         =m_nDeckThr;
	m_cActiveData.nDispType        =m_nDispType;
	m_cActiveData.nErrorLimit      =m_nErrorLimit;
	m_cActiveData.nRDir            =m_nRDir;
	m_cActiveData.nTDir            =m_nTDir;
	m_cActiveData.nMinute          =m_nMinute;
	m_cActiveData.nPixel           =m_nPixel;
	m_cActiveData.nBDispMode       =m_nBDispMode;
	m_cActiveData.nBThr            =m_nBThr;
	m_cActiveData.nDiameterCenter  =m_nDiameterCenter;
	m_cActiveData.nDiameterHigh    =m_nDiameterHigh;
	m_cActiveData.nDiameterLow     =m_nDiameterLow;
	m_cActiveData.nLocation        =m_nLocation;
	m_cActiveData.nParallel        =m_nParallel;
	m_cActiveData.nTiltCenter      =m_nTiltCenter;
	m_cActiveData.nTiltHigh        =m_nTiltHigh;
	m_cActiveData.nTiltLow         =m_nTiltLow;
	m_cActiveData.nBSize           =m_nBSize;
	m_cActiveData.nBPixel          =m_nBPixel;
	m_cActiveData.nRimHP            =m_nRimHP;
	m_cActiveData.nPThr            =m_nPThr;
	m_cActiveData.nWCorrect        =m_nWCorrect;
	m_cActiveData.nWCorrect2       =m_nWCorrect2;
	m_cActiveData.nDataThr         =m_nDataThr;
	m_cActiveData.nRimLocate       =m_nRimLocate;
	m_cActiveData.nRimInspect      =m_nRimInspect;
	m_cActiveData.nOlSize          =m_nOlSize;
	m_cActiveData.nRimVP           =m_nRimVP;
	m_cActiveData.nASt             =m_nASt;
	m_cActiveData.nLSt             =m_nLSt;
	m_cActiveData.nPSt             =m_nPSt;
	m_cActiveData.nHRSt            =m_nHRSt;
	m_cActiveData.nVRSt            =m_nVRSt;
	m_cActiveData.nDSt             =m_nDSt;
	m_cActiveData.nTSt             =m_nTSt;
	m_cActiveData.nHSt             =m_nHSt;
	m_cActiveData.nACenterFlag     =m_nACenterFlag;
	m_cActiveData.nACX             =m_nACX;
	m_cActiveData.nACY             =m_nACY;


	m_cActiveDataAdd.nRErrorLimitHigh=m_nRErrorLimitHigh;
	m_cActiveDataAdd.nTErrorLimitLow=m_nTErrorLimitLow;
	m_cActiveDataAdd.nTErrorLimitHigh=m_nTErrorLimitHigh;

    m_cActiveDataAdd.nRErrorLimitLow2=m_nRErrorLimitLow2;
    m_cActiveDataAdd.nRErrorLimitHigh2=m_nRErrorLimitHigh2;
	m_cActiveDataAdd.nTErrorLimitLow2=m_nTErrorLimitLow2;
	m_cActiveDataAdd.nTErrorLimitHigh2=m_nTErrorLimitHigh2;



	UpdateData(FALSE);    

   	m_pView->m_wOkNgWindow.m_pButton[0].SetCheck(1-m_nASt);
	m_pView->m_wOkNgWindow.m_pButton[1].SetCheck(1-m_nLSt);
	m_pView->m_wOkNgWindow.m_pButton[2].SetCheck(1-m_nPSt);
	m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(1-m_nHRSt);
	m_pView->m_wOkNgWindow.m_pButton[4].SetCheck(1-m_nDSt);
	m_pView->m_wOkNgWindow.m_pButton[5].SetCheck(1-m_nTSt);
	m_pView->m_wOkNgWindow.m_pButton[6].SetCheck(1-m_nHSt);
	m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(1-m_nVRSt);


	return;
}

void CSetupWin::SetSettingData()
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	UpdateData(TRUE);    
	m_nPDist         =m_cActiveData.nPDist;
	m_nPDist2        =m_cActiveData.nPDist2;
	m_nDeckThr       =m_cActiveData.nDeckThr;
	m_nDispType      =m_cActiveData.nDispType;
	m_nErrorLimit    =m_cActiveData.nErrorLimit;
	m_nMinute        =m_cActiveData.nMinute;
	m_nRDir          =m_cActiveData.nRDir;
	m_nPixel         =m_cActiveData.nPixel;
	m_nTDir          =m_cActiveData.nTDir;
	m_nBDispMode     =m_cActiveData.nBDispMode;
	m_nBThr          =m_cActiveData.nBThr;
	m_nDiameterCenter=m_cActiveData.nDiameterCenter;
	m_nDiameterHigh  =m_cActiveData.nDiameterHigh;
	m_nDiameterLow   =m_cActiveData.nDiameterLow;
	m_nLocation      =m_cActiveData.nLocation;
	m_nParallel      =m_cActiveData.nParallel;
	m_nTiltCenter    =m_cActiveData.nTiltCenter;
	m_nTiltHigh      =m_cActiveData.nTiltHigh;
	m_nTiltLow       =m_cActiveData.nTiltLow;
	m_nBSize         =m_cActiveData.nBSize;
	m_nBPixel        =m_cActiveData.nBPixel;
    m_nRimHP          =m_cActiveData.nRimHP;
	m_nPThr          =m_cActiveData.nPThr;
	m_nWCorrect      =m_cActiveData.nWCorrect;
	m_nWCorrect2     =m_cActiveData.nWCorrect2;
	m_nDataThr       =m_cActiveData.nDataThr;
	m_nRimLocate     =m_cActiveData.nRimLocate;
	m_nRimInspect    =m_cActiveData.nRimInspect;
    m_nOlSize        =m_cActiveData.nOlSize;
	m_nRimVP         =m_cActiveData.nRimVP;
	m_nASt			 =m_cActiveData.nASt;
	m_nLSt			 =m_cActiveData.nLSt;
	m_nPSt			 =m_cActiveData.nPSt;
	m_nHRSt			 =m_cActiveData.nHRSt;
	m_nVRSt			 =m_cActiveData.nVRSt;
	m_nDSt			 =m_cActiveData.nDSt;
	m_nTSt			 =m_cActiveData.nTSt;
	m_nHSt			 =m_cActiveData.nHSt;
	m_nACenterFlag   =m_cActiveData.nACenterFlag;
	m_nACX           =m_cActiveData.nACX;
	m_nACY           =m_cActiveData.nACY;
	
	m_pwCheckButton[0]->SetCheck(m_nASt);
	m_pwCheckButton[1]->SetCheck(m_nLSt);
	m_pwCheckButton[2]->SetCheck(m_nPSt);
	m_pwCheckButton[3]->SetCheck(m_nHRSt);
	m_pwCheckButton[4]->SetCheck(m_nDSt);
	m_pwCheckButton[5]->SetCheck(m_nTSt);
	m_pwCheckButton[6]->SetCheck(m_nHSt);
	m_pwCheckButton[7]->SetCheck(m_nVRSt);

   	m_pView->m_wOkNgWindow.m_pButton[0].SetCheck(1-m_nASt);
	m_pView->m_wOkNgWindow.m_pButton[1].SetCheck(1-m_nLSt);
	m_pView->m_wOkNgWindow.m_pButton[2].SetCheck(1-m_nPSt);
	m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(1-m_nHRSt);
	m_pView->m_wOkNgWindow.m_pButton[4].SetCheck(1-m_nDSt);
	m_pView->m_wOkNgWindow.m_pButton[5].SetCheck(1-m_nTSt);
	m_pView->m_wOkNgWindow.m_pButton[6].SetCheck(1-m_nHSt);
	m_pView->m_wOkNgWindow.m_pButton[3].SetCheck(1-m_nVRSt);

	if(m_nACenterFlag==1)
	{
		m_pView->m_wTabWindow.m_pTabButton[4]->SetCheck(1);
	}
	else
		m_pView->m_wTabWindow.m_pTabButton[4]->SetCheck(0);

//////////////////////////////////////////////////////////////////
// corrected ..
	m_nRErrorLimitHigh=m_cActiveDataAdd.nRErrorLimitHigh;
	m_nTErrorLimitLow=m_cActiveDataAdd.nTErrorLimitLow;
	m_nTErrorLimitHigh=m_cActiveDataAdd.nTErrorLimitHigh;

	m_nRErrorLimitLow2=m_cActiveDataAdd.nRErrorLimitLow2;
	m_nRErrorLimitHigh2=m_cActiveDataAdd.nRErrorLimitHigh2;
	m_nTErrorLimitLow2=m_cActiveDataAdd.nTErrorLimitLow2;
	m_nTErrorLimitHigh2=m_cActiveDataAdd.nTErrorLimitHigh2;

	UpdateData(FALSE);    

	return;
}

void CSetupWin::SetABSCenter()
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	m_nACX=(int)(m_pView->m_wImgDisp1.m_cCpt.x+0.5);
	m_nACY=(int)(m_pView->m_wImgDisp1.m_cCpt.y+0.5);
	return;
}

void CSetupWin::SaveSettingFile()
{
	FILE *fp;
	FILE *fp2;

	GetSettingData();

	fp=fopen(SETTING_FILE,"w+b");
	if(fp==NULL) return;
	fwrite(&m_cActiveData,sizeof(SETTING_DATA),1,fp);
	fclose(fp);

	fp2=fopen(SETTING_FILE_ADD,"w+b");
	if(fp2==NULL) return;
	fwrite(&m_cActiveDataAdd,sizeof(SETTING_DATA_ADD),1,fp2);
	fclose(fp2);
}

void CSetupWin::OnButtonPasswd() 
{
	CPasswdModify *pPassModify=new CPasswdModify;
	pPassModify->DoModal();
	delete pPassModify;
}

void CSetupWin::OnButtonSetupApply() 
{
	UpdateViewWindowData();
	return;
}


void CSetupWin::OnButtonSetupSave() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	SaveSettingFile();

	m_pView->SettingParam(m_cActiveDataAdd);
	m_pView->SettingParam(m_cActiveData);

	m_pView->m_wImgDisp1.Invalidate(FALSE);
	m_pView->m_wImgDisp2.Invalidate(FALSE);
	
	return;
}
void CSetupWin::OnButtonSetupDefault() 
{
	m_cActiveData.nPDist=0.17;
	m_cActiveData.nPDist2=0.17;
	m_cActiveData.nDeckThr=50;
	m_cActiveData.nDispType=1;
	m_cActiveData.nErrorLimit = -3.0;
	m_cActiveData.nMinute = 80;
	m_cActiveData.nRDir=0;
	m_cActiveData.nPixel=476;
	m_cActiveData.nTDir=0;
	m_cActiveData.nBDispMode=0;
    m_cActiveData.nBThr=50;
	m_cActiveData.nDiameterCenter=5.0;
    m_cActiveData.nDiameterHigh=0.5;
	m_cActiveData.nDiameterLow=-0.5;
	m_cActiveData.nLocation=0.5;
	m_cActiveData.nParallel=50.0;
	m_cActiveData.nTiltCenter=0.0;
	m_cActiveData.nTiltHigh= 0.5;
	m_cActiveData.nTiltLow=-0.5;
	m_cActiveData.nBSize=5.0;
	m_cActiveData.nBPixel=0;
    m_cActiveData.nRimHP=75.0;
	m_cActiveData.nPThr=110;
	m_cActiveData.nWCorrect=0.0;
	m_cActiveData.nWCorrect2=0.0;
	m_cActiveData.nDataThr=0.6;
	m_cActiveData.nRimLocate=0.0;
	m_cActiveData.nRimInspect=80;
	m_cActiveData.nOlSize=4.0;
	m_cActiveData.nRimVP=50.0;

	m_cActiveData.nASt=1;
	m_cActiveData.nLSt=1;
	m_cActiveData.nPSt=1;
	m_cActiveData.nHRSt=1;
	m_cActiveData.nVRSt=1;
	m_cActiveData.nDSt=1;
	m_cActiveData.nTSt=1;
	m_cActiveData.nHSt=1;


	m_cActiveDataAdd.nRErrorLimitHigh=3.0;
	m_cActiveDataAdd.nTErrorLimitLow=-3.0;
	m_cActiveDataAdd.nTErrorLimitHigh=3.0;

    m_cActiveDataAdd.nRErrorLimitLow2=-3.0;
	m_cActiveDataAdd.nRErrorLimitHigh2=3.0;
	m_cActiveDataAdd.nTErrorLimitLow2=-3.0;
	m_cActiveDataAdd.nTErrorLimitHigh2=3.0;






	SetSettingData();
}



int CSetupWin::ReadSettingFile()
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	FILE *fp;
	FILE *fp2;


	fp2=fopen(SETTING_FILE_ADD,"r+b");
	if(fp2==NULL)
	{
		m_nRErrorLimitHigh=m_cActiveDataAdd.nRErrorLimitHigh=3.0;
		m_nTErrorLimitLow=m_cActiveDataAdd.nTErrorLimitLow=-3.0;
		m_nTErrorLimitHigh=m_cActiveDataAdd.nTErrorLimitHigh=3.0;

        m_nRErrorLimitLow2=m_cActiveDataAdd.nRErrorLimitLow2=-3.0;
	    m_nRErrorLimitHigh2=m_cActiveDataAdd.nRErrorLimitHigh2=3.0;
	    m_nTErrorLimitLow2=m_cActiveDataAdd.nTErrorLimitLow2=-3.0;
	    m_nTErrorLimitHigh2=m_cActiveDataAdd.nTErrorLimitHigh2=3.0;

		m_pView->SettingParam(m_cActiveDataAdd);
		return 0;
	}
	else
	{
		fread(&m_cActiveDataAdd,sizeof(SETTING_DATA_ADD),1,fp2);
		fclose(fp2);
	}


	fp=fopen(SETTING_FILE,"r+b");
	if(fp==NULL)
	{
		GetSettingData();
		m_pView->SettingParam(m_cActiveData);
		return 0;
	}
	fread(&m_cActiveData,sizeof(SETTING_DATA),1,fp);
	fclose(fp);



	SettingDataCheck();
	SetSettingData();
	m_pView->SettingParam(m_cActiveDataAdd);
	m_pView->SettingParam(m_cActiveData);
	return 1;
}

void CSetupWin::SettingDataCheck()
{
	if(m_cActiveData.nPDist  <0.0        || m_cActiveData.nPDist>10.0)			 m_cActiveData.nPDist=-0.17;
	if(m_cActiveData.nPDist2 <0.0        || m_cActiveData.nPDist2>10.0)			 m_cActiveData.nPDist2=0.17;
	if(m_cActiveData.nDeckThr <0		 || m_cActiveData.nDeckThr > 255)		 m_cActiveData.nDeckThr=128;
	if(m_cActiveData.nDispType <0		 || m_cActiveData.nDispType >3)			 m_cActiveData.nDispType=1;
	if(m_cActiveData.nErrorLimit <-100.0 || m_cActiveData.nErrorLimit >100.0)	 m_cActiveData.nErrorLimit = -3.0;
	if(m_cActiveData.nMinute <0			 || m_cActiveData.nMinute > 400)		 m_cActiveData.nMinute = 80;
	if(m_cActiveData.nRDir <0			 || m_cActiveData.nRDir >1)				 m_cActiveData.nRDir=0;
	if(m_cActiveData.nPixel <1			 || m_cActiveData.nPixel >1000)			 m_cActiveData.nPixel=508;
	if(m_cActiveData.nTDir <0			 || m_cActiveData.nTDir >1)				 m_cActiveData.nTDir=0;
	if(m_cActiveData.nBDispMode<0		 || m_cActiveData.nBDispMode > 1)		 m_cActiveData.nBDispMode=0;
    if(m_cActiveData.nBThr<0             || m_cActiveData.nBThr>255)		     m_cActiveData.nBThr=50;
	if(m_cActiveData.nDiameterCenter<0.0 || m_cActiveData.nDiameterCenter >25.0) m_cActiveData.nDiameterCenter=5.0;
    if(m_cActiveData.nDiameterHigh < 0.0 || m_cActiveData.nDiameterHigh > 25.0)  m_cActiveData.nDiameterHigh=0.5;
	if(m_cActiveData.nDiameterLow>0.0	 || m_cActiveData.nDiameterLow <-25.0)   m_cActiveData.nDiameterLow=-0.5;
	if(m_cActiveData.nLocation<0.0		 || m_cActiveData.nLocation >12.0)		 m_cActiveData.nLocation=0.5;
	if(m_cActiveData.nParallel<0.0		 || m_cActiveData.nParallel > 200.0)	 m_cActiveData.nParallel=50.0;
	if(m_cActiveData.nTiltCenter<-360.0	 || m_cActiveData.nTiltCenter>360.0)	 m_cActiveData.nTiltCenter=0.0;
	if(m_cActiveData.nTiltHigh<0.0		 || m_cActiveData.nTiltHigh > 360.0)	 m_cActiveData.nTiltHigh= 0.5;
	if(m_cActiveData.nTiltLow>0.0		 || m_cActiveData.nTiltLow <-360.0)		 m_cActiveData.nTiltLow=-0.5;
	if(m_cActiveData.nBSize<0.0			 || m_cActiveData.nBSize > 12.0)		 m_cActiveData.nBSize=5.0;
	if(m_cActiveData.nBPixel<-1000  	 || m_cActiveData.nBPixel > 1000)		 m_cActiveData.nBPixel=0;
    if(m_cActiveData.nRimHP<0.0			 || m_cActiveData.nRimHP>100.0)			 m_cActiveData.nRimHP=75.0;
	if(m_cActiveData.nPThr<0			 || m_cActiveData.nPThr>255)			 m_cActiveData.nPThr=128;
	if(m_cActiveData.nWCorrect <-1000.0  || m_cActiveData.nWCorrect > 1000.0)	 m_cActiveData.nWCorrect=10.0;
	if(m_cActiveData.nWCorrect2 <-1000.0  || m_cActiveData.nWCorrect2 > 1000.0)	 m_cActiveData.nWCorrect2=-10.0;
	if(m_cActiveData.nDataThr < -5.0		 || m_cActiveData.nDataThr > 5.0)		 m_cActiveData.nDataThr=0.0;
	if(m_cActiveData.nRimLocate < -1000.0|| m_cActiveData.nRimLocate>1000.0)	 m_cActiveData.nRimLocate=0.0;
	if(m_cActiveData.nRimInspect<0		 || m_cActiveData.nRimInspect>255)		 m_cActiveData.nRimInspect=80;
	if(m_cActiveData.nOlSize <0			 || m_cActiveData.nOlSize >20.0)		 m_cActiveData.nOlSize=4.0;
	if(m_cActiveData.nRimVP  <0.0		 || m_cActiveData.nRimVP >100.0)		 m_cActiveData.nRimVP=50.0;
	if(m_cActiveData.nASt <0			 || m_cActiveData.nASt>1)				 m_cActiveData.nASt=1;
	if(m_cActiveData.nLSt <0			 || m_cActiveData.nLSt>1)				 m_cActiveData.nLSt=1;
	if(m_cActiveData.nPSt <0			 || m_cActiveData.nPSt>1)				 m_cActiveData.nPSt=1;
	if(m_cActiveData.nHRSt <0			 || m_cActiveData.nHRSt>1)				 m_cActiveData.nHRSt=1;
	if(m_cActiveData.nVRSt <0			 || m_cActiveData.nVRSt>1)				 m_cActiveData.nVRSt=1;
	if(m_cActiveData.nDSt <0			 || m_cActiveData.nDSt>1)				 m_cActiveData.nDSt=1;
	if(m_cActiveData.nTSt <0			 || m_cActiveData.nTSt>1)				 m_cActiveData.nTSt=1;
	if(m_cActiveData.nHSt <0			 || m_cActiveData.nHSt>1)				 m_cActiveData.nHSt=1;

	
	return;
}


void CSetupWin::UpdateViewWindowData()
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	GetSettingData();

	m_pView->SettingParam(m_cActiveDataAdd);
	m_pView->SettingParam(m_cActiveData);

	m_pView->m_wHProj.SetParam();
	m_pView->m_wHProj.Invalidate(FALSE);

	m_pView->m_wVProj.SetParam();
	m_pView->m_wVProj.Invalidate(FALSE);

	m_pView->m_wImgDisp1.Invalidate(FALSE);
	m_pView->m_wImgDisp2.Invalidate(FALSE);
	
	return;
}

void CSetupWin::DeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult,CEdit *pEdit,int *pData,int low,int high,int inc) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	pEdit->GetWindowText(buf);
    *pData=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		*pData+=(-inc*pNMUpDown->iDelta);
		if(*pData>high)	*pData=high;
	}
	else {
		*pData-=(inc*pNMUpDown->iDelta);
		if(*pData<low) *pData=low;
	}
	*pResult = 1;
	UpdateData(FALSE);
	return;
}
void CSetupWin::DeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult,CEdit *pEdit,double *pData,double low,double high,double inc) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	pEdit->GetWindowText(buf);
    *pData=atof(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		*pData+=(-inc*pNMUpDown->iDelta);
		if(*pData>high)	*pData=high;
	}
	else {
		*pData-=(inc*pNMUpDown->iDelta);
		if(*pData<low) *pData=low;
	}
	*pResult = 1;
	UpdateData(FALSE);
	return;
}
void CSetupWin::OnDeltaposSpinMinute(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[0][0],&m_nMinute,0,800,1);
	UpdateViewWindowData();
	return;
}

void CSetupWin::OnDeltaposSpinPixel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[0][1],&m_nPixel,0,960,1);
	UpdateViewWindowData();
	return;
}

void CSetupWin::OnDeltaposSpinRdir(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[1][0],&m_nRDir,0,1,1);
	UpdateViewWindowData();
	return;
}

void CSetupWin::OnDeltaposSpinTdir(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[1][1],&m_nTDir,0,1,1);
	UpdateViewWindowData();
	return;
}

void CSetupWin::OnDeltaposSpinDisptype(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[2][1],&m_nDispType,0,2,1);
	UpdateViewWindowData();

	return;
}

void CSetupWin::OnDeltaposSpinPdist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[3][0],&m_nPDist,0.0,6.0,0.01);
	return;
}
void CSetupWin::OnDeltaposSpinPdist2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwPdEdit,&m_nPDist2,0.0,6.0,0.01);
	return;
}

void CSetupWin::OnDeltaposSpinDeckthr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaposSpin(pNMHDR,pResult,m_pwEdit[3][1],&m_nDeckThr,0,255,1);
 
	KIP_BinarizeAll(m_pView->m_wImgDisp2.m_pOrigin,m_pView->m_wImgDisp2.m_pOut,m_pView->m_nWidth,m_pView->m_nHeight,1,m_nDeckThr);
	m_pView->m_wImgDisp2.ImgUpdate(m_pView->m_wImgDisp2.m_pOut);

	return;
}




/////////////////////////////////////////////////////////////////
void CSetupWin::OnDeltaposSpinBsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[4][0],&m_nBSize,0.01,3000.0,0.01);
	UpdateViewWindowData();	
	return;
}

void CSetupWin::OnDeltaposSpinBpixel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[4][1],&m_nBPixel,-1000,1000,1);
	UpdateViewWindowData();
	return;
}

void CSetupWin::OnDeltaposSpinBdisp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaposSpin(pNMHDR,pResult,m_pwEdit[5][0],&m_nBDispMode,0,1,1);

	UpdateViewWindowData();
	m_pView->m_wImgDisp1.ChangePallete();
	m_pView->m_wImgDisp1.Invalidate();
	return;
}

void CSetupWin::OnDeltaposSpinBthr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaposSpin(pNMHDR,pResult,m_pwEdit[5][1],&m_nBThr,0,255,1);

	KIP_BinarizeAll(m_pView->m_wImgDisp1.m_pSrc,m_pView->m_wImgDisp1.m_pOut,m_pView->m_nWidth,m_pView->m_nHeight,1,m_nBThr);
	m_pView->m_wImgDisp1.ImgUpdate(m_pView->m_wImgDisp1.m_pOut);

	return ;
}


void CSetupWin::OnDeltaposSpinPthr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaposSpin(pNMHDR,pResult,m_pwEdit[6][0],&m_nPThr,0,255,1);
	
 	KIP_BinarizeAll(m_pView->m_wImgDisp1.m_pSrc,m_pView->m_wImgDisp1.m_pOut,m_pView->m_nWidth,m_pView->m_nHeight,1,m_nPThr);
	m_pView->m_wImgDisp1.ImgUpdate(m_pView->m_wImgDisp1.m_pOut);

	return ;
}

void CSetupWin::OnDeltaposSpinOlsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[6][1],&m_nOlSize,0.0,20.0,0.01);
	return;
}

void CSetupWin::OnDeltaposSpinLoc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[7][0],&m_nLocation,0.0,12.0,0.01);
	return ;
}

void CSetupWin::OnDeltaposSpinPara(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[7][1],&m_nParallel,0.0,200.0,0.01);
	return ;
}


void CSetupWin::OnDeltaposSpinWidthcorrect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaposSpin(pNMHDR,pResult,m_pwEdit[8][0],&m_nWCorrect,-1000.0,1000.0,0.01);
	
	m_pView->m_wVProj.ReCompute(m_nDataThr,m_nWCorrect);
	UpdateViewWindowData();

	return ;
}


void CSetupWin::OnDeltaposSpinWidthcorrect2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaposSpin(pNMHDR,pResult,m_pwWcEdit,&m_nWCorrect2,-1000.0,1000.0,0.01);

	m_pView->m_wHProj.ReCompute(m_nDataThr,m_nWCorrect2);
	UpdateViewWindowData();

	return ;
}

void CSetupWin::OnDeltaposSpinDatathr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaposSpin(pNMHDR,pResult,m_pwEdit[8][1],&m_nDataThr,-5.0,5.0,0.01);

	m_pView->m_wHProj.ReCompute(m_nDataThr,m_nWCorrect2);
	m_pView->m_wVProj.ReCompute(m_nDataThr,m_nWCorrect);

	UpdateViewWindowData();
	return ;
}

void CSetupWin::OnDeltaposSpinRimp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[9][0],&m_nRimHP,0.0,100.0,0.01);
	return;
}




void CSetupWin::OnDeltaposSpinRimlocate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaposSpin(pNMHDR,pResult,m_pwEdit[9][1],&m_nRimLocate,-128,128,1);

	m_pView->m_wHProj.RimLocate(m_nRimLocate);
	m_pView->m_wVProj.RimLocate(m_nRimLocate);
    UpdateViewWindowData();


	return ;
}

void CSetupWin::OnDeltaposSpinPdata(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[10][0],&m_nRimVP,0.0,100.0,0.01);
	return ;
}

void CSetupWin::OnDeltaposSpinRiminspect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaposSpin(pNMHDR,pResult,m_pwEdit[10][1],&m_nRimInspect,0,255,1);

	m_pView->m_wHProj.ReCompute(m_nDataThr,m_nWCorrect2);
	m_pView->m_wVProj.ReCompute(m_nDataThr,m_nWCorrect);

	UpdateViewWindowData();
	return ;
}



void CSetupWin::OnDeltaposSpinDiac(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEditEx[0][0],&m_nDiameterCenter,0.0,12.0,0.01);
	UpdateViewWindowData();
	return;
}



void CSetupWin::OnDeltaposSpinDial(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEditEx[0][1],&m_nDiameterLow,-12.0,0.0,0.01);
	return;
}

void CSetupWin::OnDeltaposSpinDiah(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEditEx[0][2],&m_nDiameterHigh,0.0,12.0,0.01);
	UpdateViewWindowData();
	return;
}



void CSetupWin::OnDeltaposSpinTiltc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEditEx[1][0],&m_nTiltCenter,-360.0,360.0,0.01);
	return;
}

void CSetupWin::OnDeltaposSpinTiltl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEditEx[1][1],&m_nTiltLow,-360.0,0.0,0.01);
	return;
}
void CSetupWin::OnDeltaposSpinTilth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEditEx[1][2],&m_nTiltHigh,0.0,360.0,0.01);
	return;
}
/////////////////////////////////////////////////////////
// inspection status changing, "A","L","P","R","S","T"
/////////////////////////////////////////////////////////
void CSetupWin::OnCheckButtonAlimit() 
{
	if(m_pwCheckButton[0]->GetCheck()) 	m_nASt=1;
	else								m_nASt=0;
	return;
}

void CSetupWin::OnCheckButtonDiameter() 
{
	if(m_pwCheckButton[4]->GetCheck()) 	m_nDSt=1;
	else								m_nDSt=0;
	return;
}

void CSetupWin::OnCheckButtonDistance() 
{
	if(m_pwCheckButton[1]->GetCheck()) 	m_nLSt=1;
	else								m_nLSt=0;

	return;
}

void CSetupWin::OnCheckButtonParallelism() 
{
	if(m_pwCheckButton[2]->GetCheck()) 	m_nPSt=1;
	else								m_nPSt=0;

	return;
}

void CSetupWin::OnCheckButtonRim() 
{

	if(m_pwCheckButton[3]->GetCheck()) 	m_nHRSt=1;
	else								m_nHRSt=0;
	return;
}

void CSetupWin::OnCheckButtonTilt() 
{
	if(m_pwCheckButton[5]->GetCheck()) 	m_nTSt=1;
	else								m_nTSt=0;
	return;
}

//////////////////////////////////////////////////////////


void CSetupWin::OnCheckButtonPdist() 
{
	if(m_pwCheckButton[6]->GetCheck()) 	m_nHSt=1;
	else								m_nHSt=0;
	return;
}

void CSetupWin::OnCheckButtonRim2() 
{
	if(m_pwCheckButton[7]->GetCheck()) 	m_nVRSt=1;
	else								m_nVRSt=0;
	return;
	
}





void CSetupWin::OnDeltaposSpinError(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwEdit[2][0],&m_nErrorLimit,-100.0,100.0,0.1);
	return;
}

void CSetupWin::OnDeltaposSpinError2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwRErrorHighEdit,&m_nRErrorLimitHigh,-100.0,100.0,0.1);
	return;
}
void CSetupWin::OnDeltaposSpinError3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwTErrorLowEdit,&m_nTErrorLimitLow,-100.0,100.0,0.1);
	return;
}
void CSetupWin::OnDeltaposSpinError4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwTErrorHighEdit,&m_nTErrorLimitHigh,-100.0,100.0,0.1);
	return;
}

void CSetupWin::OnOutofmemorySpinError5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwRErrorLowEdit2,&m_nRErrorLimitLow2,-100.0,100.0,0.1);
	return;
}

void CSetupWin::OnOutofmemorySpinError6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwRErrorHighEdit2,&m_nRErrorLimitHigh2,-100.0,100.0,0.1);
	return;
}

void CSetupWin::OnOutofmemorySpinError7(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwTErrorLowEdit2,&m_nTErrorLimitLow2,-100.0,100.0,0.1);
	return;
}

void CSetupWin::OnOutofmemorySpinError8(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaposSpin(pNMHDR,pResult,m_pwTErrorHighEdit2,&m_nTErrorLimitHigh2,-100.0,100.0,0.1);
	return;
}
