// 3DModelWindow.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "MainFrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "3DModelWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define C3DFILE ".\\data\\m3dmodel.bin"
/////////////////////////////////////////////////////////////////////////////
// C3DModelWindow dialog


C3DModelWindow::C3DModelWindow(CWnd* pParent /*=NULL*/)
	: CDialog(C3DModelWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3DModelWindow)
	m_nFillMode = 0;
	m_nZTrans = 1.5f;
	m_nLightXPos = 50;
	m_nLightYPos = 80;
	m_nLightZPos = 100;
	m_nAmbientLight = 50;
	m_nDiffuseLight = 100;
	m_nSpecularLight = 100;
	m_nAmbientMaterial = 60;
	m_nDiffuseMaterial = 50;
	m_nSpecularMaterial = 80;
	m_nEmission = 10;
	m_nShinness = 0;
	//}}AFX_DATA_INIT
	m_wSysMenu=NULL;
	m_pwSaveButton=NULL;
	int i;

	for(i=0; i<13; i++)
	{
		m_pwStatic[i]=NULL;
		m_pwEdit[i]=NULL;
		m_pwSpin[i]=NULL;
	}
}


void C3DModelWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DModelWindow)
	DDX_Text(pDX, IDC_EDIT_3DSET1, m_nFillMode);
	DDX_Text(pDX, IDC_EDIT_3DSET2, m_nZTrans);
	DDX_Text(pDX, IDC_EDIT_3DSET3, m_nLightXPos);
	DDX_Text(pDX, IDC_EDIT_3DSET4, m_nLightYPos);
	DDX_Text(pDX, IDC_EDIT_3DSET5, m_nLightZPos);
	DDX_Text(pDX, IDC_EDIT_3DSET6, m_nAmbientLight);
	DDX_Text(pDX, IDC_EDIT_3DSET7, m_nDiffuseLight);
	DDX_Text(pDX, IDC_EDIT_3DSET8, m_nSpecularLight);
	DDX_Text(pDX, IDC_EDIT_3DSET9, m_nAmbientMaterial);
	DDX_Text(pDX, IDC_EDIT_3DSET10, m_nDiffuseMaterial);
	DDX_Text(pDX, IDC_EDIT_3DSET11, m_nSpecularMaterial);
	DDX_Text(pDX, IDC_EDIT_3DSET13, m_nEmission);
	DDX_Text(pDX, IDC_EDIT_3DSET12, m_nShinness);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3DModelWindow, CDialog)
	//{{AFX_MSG_MAP(C3DModelWindow)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET1, OnDeltaposSpinFillMode)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET2, OnDeltaposSpinZTrans)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET3, OnDeltaposSpinLightXPos)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET4, OnDeltaposSpinLightYPos)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET5, OnDeltaposSpinLightZPos)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET6, OnDeltaposSpinAmbientLight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET7, OnDeltaposSpinDiffuseLight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET8, OnDeltaposSpinSpecularLight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET9, OnDeltaposSpinAmbientMaterial)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET10, OnDeltaposSpinDiffuseMaterial)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET11, OnDeltaposSpinSpecularMaterial)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET12, OnDeltaposSpinShinness)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3DSET13, OnDeltaposSpinEmission)
	ON_BN_CLICKED(IDC_3DMODEL_SAVE, On3dmodelSave)
	ON_BN_CLICKED(IDC_BUTTON_SYSMENU_3DMODEL, OnButtonSysmenu3dmodel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DModelWindow message handlers

BOOL C3DModelWindow::PreTranslateMessage(MSG* pMsg) 
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

void C3DModelWindow::OnDestroy() 
{
	CDialog::OnDestroy();
	int i;

	m_w3DDisp.DestroyWindow();
    for(i=0; i<13; i++)
	{
		if(m_pwStatic[i]!=NULL) delete m_pwStatic[i];
		if(m_pwEdit[i]!=NULL) delete m_pwEdit[i];
		if(m_pwSpin[i]!=NULL) delete m_pwSpin[i];
	}
	if(m_wSysMenu!=NULL) delete m_wSysMenu;
	if(m_pwSaveButton!=NULL) delete m_pwSaveButton;
	delete pBmpRgn;	
	// TODO: Add your message handler code here
}

BOOL C3DModelWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
/************************************/
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\3dmodelwindow.bmp", 0x00ff00ff);
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
/*************************************/	
	m_wSysMenu=new CxSkinButton;
	m_wSysMenu->SubclassDlgItem(IDC_BUTTON_SYSMENU_3DMODEL,this);
	m_wSysMenu->SetSkin(IDB_TITLE_SYSMENU,IDB_TITLE_SYSMENUD,IDB_TITLE_SYSMENU,0,0,IDB_TITLE_SYSMENUMASK,0,0,0);
	m_wSysMenu->MoveWindow(2,2,20,20);

	m_pwSaveButton= new CxSkinButton;
	m_pwSaveButton->SubclassDlgItem(IDC_3DMODEL_SAVE,this);
	m_pwSaveButton->SetSkin(IDB_3DSAVEN,IDB_3DSAVED,IDB_3DSAVEN,0,0,IDB_3DSAVEM,0,0,0);
    m_pwSaveButton->MoveWindow(2,412,120,40);

	MakeViewParamWindow();

	CRect m_c3DWndRect;
	m_c3DWndRect.SetRect(130,543,639,983);
	m_w3DDisp.SetBufSize(m_nWidth,m_nHeight);
	m_w3DDisp.Create(IDD_DIALOG_3DDISPLAY,this);
	m_w3DDisp.MoveWindow(&m_c3DWndRect);

	m_w3DDisp.SetOrgRect(m_c3DWndRect);

	ReadParam();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3DModelWindow::ChangeModelWindowView(int cmdShow)
{
	m_w3DDisp.ShowWindow(cmdShow);
	return;
}

void C3DModelWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void C3DModelWindow::SetBuffer(unsigned char *pImg)
{
	m_w3DDisp.SetBuffer(pImg);
	
	return;
}

void C3DModelWindow::MakeViewParamWindow()
{
	char *fname=".\\guiimage\\st128x26_256.BMP";
	int i;
	char *pTitle[13]=
	{
		"FILL",
		"ZOOM",
		"L-XPOS",
		"L-YPOS",
		"L-ZPOS",
		"L-AMB",
		"L-DIF",
		"L-SPEC",
		"M-AMB",
		"M-DIF",
		"M-SPEC",
		"M-SHIN",
        "M-EMIS"
	};
	UINT nSIDs[13]=
	{
		IDC_ST_3DSET1,
		IDC_ST_3DSET2,
		IDC_ST_3DSET3,
		IDC_ST_3DSET4,
		IDC_ST_3DSET5,
		IDC_ST_3DSET6,
		IDC_ST_3DSET7,
		IDC_ST_3DSET8,
		IDC_ST_3DSET9,
		IDC_ST_3DSET10,
		IDC_ST_3DSET11,
		IDC_ST_3DSET12,
		IDC_ST_3DSET13
	};
	UINT nEIDs[13]=
	{
		IDC_EDIT_3DSET1,
		IDC_EDIT_3DSET2,
		IDC_EDIT_3DSET3,
		IDC_EDIT_3DSET4,
		IDC_EDIT_3DSET5,
		IDC_EDIT_3DSET6,
		IDC_EDIT_3DSET7,
		IDC_EDIT_3DSET8,
		IDC_EDIT_3DSET9,
		IDC_EDIT_3DSET10,
		IDC_EDIT_3DSET11,
		IDC_EDIT_3DSET12,
		IDC_EDIT_3DSET13
	};

	UINT nSPIDs[13]=
	{
		IDC_SPIN_3DSET1,
		IDC_SPIN_3DSET2,
		IDC_SPIN_3DSET3,
		IDC_SPIN_3DSET4,
		IDC_SPIN_3DSET5,
		IDC_SPIN_3DSET6,
		IDC_SPIN_3DSET7,
		IDC_SPIN_3DSET8,
		IDC_SPIN_3DSET9,
		IDC_SPIN_3DSET10,
		IDC_SPIN_3DSET11,
		IDC_SPIN_3DSET12,
		IDC_SPIN_3DSET13
	};


	for(i=0; i<13; i++)
	{
/////////////////////////////////////////////////////////////////
// CStaticSkin
		m_pwStatic[i]=new CStaticSkin;
		m_pwStatic[i]->SetBmpFile(fname);
		m_pwStatic[i]->SubclassDlgItem(nSIDs[i],this);
		m_pwStatic[i]->MoveWindow(2,28+29*i,128,26); 
		m_pwStatic[i]->OutTextXY(2,4,pTitle[i]);

/////////////////////////////////////////////////////////////////
// CEdit
		m_pwEdit[i]=new CEdit;
		m_pwEdit[i]->SubclassDlgItem(nEIDs[i],this);
		m_pwEdit[i]->MoveWindow(69,28+29*i,48,26); 

/////////////////////////////////////////////////////////////////
// CSpinButtonCtrl
		m_pwSpin[i]=new CSpinButtonCtrl;
		m_pwSpin[i]->SubclassDlgItem(nSPIDs[i],this);
		m_pwSpin[i]->MoveWindow(117,28+29*i,12,24); 
	
	}
}

void C3DModelWindow::GetParam()
{
/*
	int		m_nFillMode;
	float	m_nZTrans;
	int		m_nLightXPos;
	int		m_nLightYPos;
	int		m_nLightZPos;
	int		m_nAmbientLight;
	int		m_nDiffuseLight;
	int		m_nSpecularLight;
	int		m_nAmbientMaterial;
	int		m_nDiffuseMaterial;
	int		m_nSpecularMaterial;
	int		m_nShinnesss;
	int		m_nEmission;
*/
	UpdateData(TRUE);

	m_cModel.nf=m_nFillMode;
	m_cModel.nzf=m_nZTrans;
	
	m_cModel.nlp[0]=m_nLightXPos;
    m_cModel.nlp[1]=m_nLightYPos;
	m_cModel.nlp[2]=m_nLightZPos;
	m_cModel.nlp[3]=m_nAmbientLight;
	m_cModel.nlp[4]=m_nDiffuseLight;
	m_cModel.nlp[5]=m_nSpecularLight;
	m_cModel.nlp[6]=m_nAmbientMaterial;
	m_cModel.nlp[7]=m_nDiffuseMaterial;
	m_cModel.nlp[8]=m_nSpecularMaterial;
    m_cModel.nlp[9]=m_nShinness;
	m_cModel.nlp[10]=m_nEmission;

	UpdateData(FALSE);

	m_w3DDisp.SetFillMode(m_cModel.nf);
	m_w3DDisp.SetZoom(m_cModel.nzf);
	m_w3DDisp.SetLightParam(&m_cModel.nlp[0]);

	return;
}
void C3DModelWindow::SetParam()
{
	m_nFillMode        =m_cModel.nf;
	m_nZTrans          =m_cModel.nzf;
	m_nLightXPos       =m_cModel.nlp[0];
    m_nLightYPos       =m_cModel.nlp[1];
	m_nLightZPos       =m_cModel.nlp[2];
	m_nAmbientLight    =m_cModel.nlp[3];
	m_nDiffuseLight    =m_cModel.nlp[4];
	m_nSpecularLight   =m_cModel.nlp[5];
	m_nAmbientMaterial =m_cModel.nlp[6];
	m_nDiffuseMaterial =m_cModel.nlp[7];
	m_nSpecularMaterial=m_cModel.nlp[8];
    m_nShinness        =m_cModel.nlp[9];
	m_nEmission        =m_cModel.nlp[10];

	m_w3DDisp.SetFillMode(m_cModel.nf);
	m_w3DDisp.SetZoom(m_cModel.nzf);
	m_w3DDisp.SetLightParam(&m_cModel.nlp[0]);

	return;
}

void C3DModelWindow::ReadParam()
{

	FILE *fp;
	fp=fopen(C3DFILE,"r+b");
    if(fp!=NULL)
	{
		fread(&m_cModel,sizeof(PARAM_3DMODEL),1,fp);
		fclose(fp);
        SetParam();
		UpdateData(FALSE);
	}
	return ;
}

void C3DModelWindow::SaveParam()
{
	GetParam();
    FILE *fp;
	fp=fopen(C3DFILE,"w+b");
	fwrite(&m_cModel,sizeof(PARAM_3DMODEL),1,fp);
	fclose(fp);
	return;
}

void C3DModelWindow::On3dmodelSave() 
{
	SaveParam();
	return;
}


void C3DModelWindow::OnDeltaposSpinFillMode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[0]->GetWindowText(buf);
    m_nFillMode=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nFillMode+=1;
		if(m_nFillMode>2)	m_nFillMode=2;
	}
	else {
		m_nFillMode-=1;
		if(m_nFillMode<0) m_nFillMode=0;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeFillMode(m_nFillMode);
	return;
}




void C3DModelWindow::OnDeltaposSpinZTrans(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[1]->GetWindowText(buf);
    m_nZTrans=atof(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nZTrans+=(float)0.05;
		if(m_nZTrans>(float)10.0)	m_nZTrans=(float)10.0;
	}
	else {
		m_nZTrans-=(float)0.05;
		if(m_nZTrans<(float)0.05) m_nZTrans=(float)0.05;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeZTrans(m_nZTrans);
	return;
}


void C3DModelWindow::OnDeltaposSpinLightXPos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[2]->GetWindowText(buf);
    m_nLightXPos=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nLightXPos+=5;
		if(m_nLightXPos>200)	m_nLightXPos=200;
	}
	else {
		m_nLightXPos-=5;
		if(m_nLightXPos<-200) m_nLightXPos=-200;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightXPos(m_nLightXPos);
	return;
}

void C3DModelWindow::OnDeltaposSpinLightYPos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[3]->GetWindowText(buf);
    m_nLightYPos=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nLightYPos+=5;
		if(m_nLightYPos>200)	m_nLightYPos=200;
	}
	else {
		m_nLightYPos-=5;
		if(m_nLightYPos<-200) m_nLightYPos=-200;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightYPos(m_nLightYPos);
	return;
}

void C3DModelWindow::OnDeltaposSpinLightZPos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[4]->GetWindowText(buf);
    m_nLightZPos=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nLightZPos+=5;
		if(m_nLightZPos>200)	m_nLightZPos=200;
	}
	else {
		m_nLightZPos-=5;
		if(m_nLightZPos<-200) m_nLightZPos=-200;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightZPos(m_nLightZPos);
	return;
}

void C3DModelWindow::OnDeltaposSpinAmbientLight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[5]->GetWindowText(buf);
    m_nAmbientLight=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nAmbientLight+=1;
		if(m_nAmbientLight>200)	m_nAmbientLight=200;
	}
	else {
		m_nAmbientLight-=1;
		if(m_nAmbientLight<-200) m_nAmbientLight=-200;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightAmbient(m_nAmbientLight);
	return;
}

void C3DModelWindow::OnDeltaposSpinDiffuseLight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[6]->GetWindowText(buf);
    m_nDiffuseLight=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nDiffuseLight+=5;
		if(m_nDiffuseLight>200)	m_nDiffuseLight=200;
	}
	else {
		m_nDiffuseLight-=5;
		if(m_nDiffuseLight<-200) m_nDiffuseLight=-200;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightDiffuse(m_nDiffuseLight);
	return;
}

void C3DModelWindow::OnDeltaposSpinSpecularLight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[7]->GetWindowText(buf);
    m_nSpecularLight=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nSpecularLight+=5;
		if(m_nSpecularLight>200)	m_nSpecularLight=200;
	}
	else {
		m_nSpecularLight-=5;
		if(m_nSpecularLight<-200) m_nSpecularLight=-2100;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightSpecular(m_nSpecularLight);
	return;
}

void C3DModelWindow::OnDeltaposSpinAmbientMaterial(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[8]->GetWindowText(buf);
    m_nAmbientMaterial=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nAmbientMaterial+=5;
		if(m_nAmbientMaterial>200)	m_nAmbientMaterial=200;
	}
	else {
		m_nAmbientMaterial-=5;
		if(m_nAmbientMaterial<-200) m_nAmbientMaterial=-200;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightAmbientMaterial(m_nAmbientMaterial);
	return;
}


void C3DModelWindow::OnDeltaposSpinDiffuseMaterial(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[9]->GetWindowText(buf);
    m_nDiffuseMaterial=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nDiffuseMaterial+=5;
		if(m_nDiffuseMaterial>200)	m_nDiffuseMaterial=200;
	}
	else {
		m_nDiffuseMaterial-=5;
		if(m_nDiffuseMaterial<-200) m_nDiffuseMaterial=-200;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightDiffuseMaterial(m_nDiffuseMaterial);
	return;
}

void C3DModelWindow::OnDeltaposSpinSpecularMaterial(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[10]->GetWindowText(buf);
    m_nSpecularMaterial=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nSpecularMaterial+=5;
		if(m_nSpecularMaterial>200)	m_nSpecularMaterial=200;
	}
	else {
		m_nSpecularMaterial-=5;
		if(m_nSpecularMaterial<-200) m_nSpecularMaterial=-200;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightSpecularMaterial(m_nSpecularMaterial);
	return;
}

void C3DModelWindow::OnDeltaposSpinShinness(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[11]->GetWindowText(buf);
    m_nShinness=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nShinness+=1;
		if(m_nShinness>100)	m_nShinness=100;
	}
	else {
		m_nShinness-=1;
		if(m_nShinness<0) m_nShinness=0;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightShinness(m_nShinness);
	return;
}

void C3DModelWindow::OnDeltaposSpinEmission(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	m_pwEdit[12]->GetWindowText(buf);
    m_nEmission=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		m_nEmission+=1;
		if(m_nEmission>100)	m_nEmission=100;
	}
	else {
		m_nEmission-=1;
		if(m_nEmission<0) m_nEmission=0;
	}
	*pResult = 1;
	UpdateData(FALSE);
	m_w3DDisp.ChangeLightEmission(m_nEmission);
	return;
}


void C3DModelWindow::OnButtonSysmenu3dmodel() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	SetBuffer(m_pView->m_wImgDisp1.m_pOrigin);
	Invalidate(FALSE);
	
}
