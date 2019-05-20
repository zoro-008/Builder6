// CameraControl.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "mainfrm.h"
#include "CameraControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCameraControl dialog

CCameraControl::CCameraControl(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraControl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCameraControl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	int i;
	int j;
	for(i=0; i<5; i++)
	{
		for(j=0; j<3; j++)
			m_FeatureValue[j][i]=1;
		m_Min[i]=1;
		m_Max[i]=2000;
	}
	m_iCameraNum=0;
}


void CCameraControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCameraControl)
	DDX_Control(pDX, IDC_EDIT_CAMCTRL1, m_Edit[0]);
	DDX_Control(pDX, IDC_EDIT_CAMCTRL2, m_Edit[1]);
	DDX_Control(pDX, IDC_EDIT_CAMCTRL3, m_Edit[2]);
	DDX_Control(pDX, IDC_EDIT_CAMCTRL4, m_Edit[3]);
	DDX_Control(pDX, IDC_EDIT_CAMCTRL5, m_Edit[4]);

	DDX_Control(pDX, IDC_COMBO_CAMERA, m_Combo);
	DDX_Control(pDX, IDC_STATIC_CAMCTLEX1, m_StRange[0]);
	DDX_Control(pDX, IDC_STATIC_CAMCTLEX2, m_StRange[1]);
	DDX_Control(pDX, IDC_STATIC_CAMCTLEX3, m_StRange[2]);
	DDX_Control(pDX, IDC_STATIC_CAMCTLEX4, m_StRange[3]);
	DDX_Control(pDX, IDC_STATIC_CAMCTLEX5, m_StRange[4]);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCameraControl, CDialog)
	//{{AFX_MSG_MAP(CCameraControl)
		// NOTE: the ClassWizard will add message map macros here
	ON_WM_DESTROY()

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAMCTRL1, OnDeltaposSpinCamctrl1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAMCTRL2, OnDeltaposSpinCamctrl2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAMCTRL3, OnDeltaposSpinCamctrl3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAMCTRL4, OnDeltaposSpinCamctrl4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAMCTRL5, OnDeltaposSpinCamctrl5)

	ON_BN_CLICKED(IDC_BUTTON_SETDEFAULT1, OnBnClickedButtonSetdefault1)
	ON_BN_CLICKED(IDC_BUTTON_SETDEFAULT2, OnBnClickedButtonSetdefault2)
	ON_BN_CLICKED(IDC_BUTTON_SETDEFAULT3, OnBnClickedButtonSetdefault3)
	ON_BN_CLICKED(IDC_BUTTON_SETDEFAULT4, OnBnClickedButtonSetdefault4)
	ON_BN_CLICKED(IDC_BUTTON_SETDEFAULT5, OnBnClickedButtonSetdefault5)

	ON_CBN_SELCHANGE(IDC_COMBO_CAMERA, OnCbnSelchangeComboCamera)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCameraControl::OnDestroy()
{
	CDialog::OnDestroy();
	Save();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
/////////////////////////////////////////////////////////////////////////////
// CCameraControl message handlers

int CCameraControl::Load()
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

void CCameraControl::Save()
{
	FILE *fp;

	fp=fopen(CAM_CTRL_FILE,"w+b");
	if(fp==NULL) return ;

	fwrite(&m_FeatureValue[0][0],sizeof(int),5,fp);
	fwrite(&m_FeatureValue[1][0],sizeof(int),5,fp);
	fwrite(&m_FeatureValue[2][0],sizeof(int),5,fp);

	fclose(fp);
	return ;
}

BOOL CCameraControl::OnInitDialog()
{
	CDialog::OnInitDialog();
	int i;
	CString str;
	int j;
    int k=0;
	RECT r;

    r.left  =132;
	r.right =264;
	r.top   =8;
	r.bottom=102;

	m_Combo.MoveWindow(&r);

	m_Combo.SetCurSel(2);
	m_iCameraNum=2;

	for(i=0; i<5; i++)
	{
		if(i>2) k=5;
		m_pImiBD->GetFeatureRange(m_iCameraNum,i+k,&m_Min[i],&m_Max[i]);
		str.Format(_T("(%4d ~ %6d)"),m_Min[i],m_Max[i]);
		m_StRange[i].SetWindowTextA(str);
	}
 

	if(Load())
	{
		for(j=0; j<3; j++)
		{
			 k=0;

			for(i=0;i<5; i++)
			{
				if(i>2) k=5;

				m_pImiBD->SetFeatureValue(j,i+k,m_FeatureValue[j][i]);
				str.Format("%d",m_FeatureValue[j][i]);
				m_Edit[i].SetWindowText(str);
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

				m_pImiBD->GetCurrentFeature(j,i+k,&m_FeatureValue[j][i]);
				str.Format("%d",m_FeatureValue[j][i]);
				m_Edit[i].SetWindowText(str);
			}
		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCameraControl::OnCbnSelchangeComboCamera()
{
	m_iCameraNum=m_Combo.GetCurSel();
    int i;
	CString str;
	int k=0;

	for(i=0; i<5; i++)
	{
		if(i>2) k=5;

		m_pImiBD->SetFeatureValue(m_iCameraNum,i+k,m_FeatureValue[m_iCameraNum][i]);
		str.Format("%d",m_FeatureValue[m_iCameraNum][i]);
		m_Edit[i].SetWindowText(str);
	}

}

void CCameraControl::SetDefault(int Feature)
{
	CString str;
	int k=0;

	if(Feature>2) k=5;
	m_pImiBD->SetFeatureDefault(m_iCameraNum,Feature+k);
	m_pImiBD->GetCurrentFeature(m_iCameraNum,Feature+k,&m_FeatureValue[m_iCameraNum][Feature]);

	str.Format("%d",m_FeatureValue[m_iCameraNum][Feature]);
	m_Edit[Feature].SetWindowText(str);
}

void CCameraControl::OnBnClickedButtonSetdefault1(){SetDefault(CCamFeature::BRIGHTNESS);}
void CCameraControl::OnBnClickedButtonSetdefault2(){SetDefault(CCamFeature::SHARPNESS);}
void CCameraControl::OnBnClickedButtonSetdefault3(){SetDefault(CCamFeature::GAMMA);}
void CCameraControl::OnBnClickedButtonSetdefault4(){SetDefault(CCamFeature::SHUTTER);}
void CCameraControl::OnBnClickedButtonSetdefault5(){SetDefault(CCamFeature::GAIN);} 

// CCameraControl 메시지 처리기입니다.
void CCameraControl::DeltaPosMove(NMHDR *pNMHDR, LRESULT *pResult,CEdit *pwEdit,double *dV,double low,double high,double offset)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString buf;

	UpdateData(TRUE);    
	pwEdit->GetWindowText(buf);
	*dV=atof(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0)	
	{
		*dV+=(offset*(-pNMUpDown->iDelta));
		if(*dV>high)	*dV=high;
	}
	else
	{
		*dV-=(offset*pNMUpDown->iDelta);
		if(*dV<low) *dV=low;
	}
	*pResult = 1;
	UpdateData(FALSE);
}

void CCameraControl::DeltaPosMove(NMHDR *pNMHDR, LRESULT *pResult,CEdit *pwEdit,int *dV,int low,int high,int offset)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString buf;

	UpdateData(TRUE);    
	pwEdit->GetWindowText(buf);
	*dV=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0)
	{
		*dV+=(offset*(-pNMUpDown->iDelta));
		if(*dV>high)	*dV=high;
	}
	else 
	{
		*dV-=(offset*pNMUpDown->iDelta);
		if(*dV<low) *dV=low;
	}
	*pResult = 1;

	UpdateData(FALSE);
}


void CCameraControl::ChangeFeature(NMHDR *pNMHDR, LRESULT *pResult,int idx)
{
	CString str;
	int k=0;
	DeltaPosMove(pNMHDR,pResult,&m_Edit[idx],&m_FeatureValue[m_iCameraNum][idx],m_Min[idx],m_Max[idx],1);

	if(idx>2) k=5;

	m_pImiBD->SetFeatureValue(m_iCameraNum,idx+k,m_FeatureValue[m_iCameraNum][idx]);

	str.Format("%d",m_FeatureValue[m_iCameraNum][idx]);
	m_Edit[idx].SetWindowText(str);
}
 
void CCameraControl::OnDeltaposSpinCamctrl1(NMHDR *pNMHDR, LRESULT *pResult){ChangeFeature(pNMHDR,pResult,CCamFeature::BRIGHTNESS);}
void CCameraControl::OnDeltaposSpinCamctrl2(NMHDR *pNMHDR, LRESULT *pResult){ChangeFeature(pNMHDR,pResult,CCamFeature::SHARPNESS);}
void CCameraControl::OnDeltaposSpinCamctrl3(NMHDR *pNMHDR, LRESULT *pResult){ChangeFeature(pNMHDR,pResult,CCamFeature::GAMMA);}
void CCameraControl::OnDeltaposSpinCamctrl4(NMHDR *pNMHDR, LRESULT *pResult){ChangeFeature(pNMHDR,pResult,CCamFeature::SHUTTER);}
void CCameraControl::OnDeltaposSpinCamctrl5(NMHDR *pNMHDR, LRESULT *pResult){ChangeFeature(pNMHDR,pResult,CCamFeature::GAIN);}