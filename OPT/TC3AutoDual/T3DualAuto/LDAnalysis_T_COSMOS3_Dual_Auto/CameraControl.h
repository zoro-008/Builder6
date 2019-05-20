#if !defined(AFX_CAMERACONTROL_H__836A2ED3_AC44_4356_888C_2EAF50B3D551__INCLUDED_)
#define AFX_CAMERACONTROL_H__836A2ED3_AC44_4356_888C_2EAF50B3D551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CameraControl.h : header file
//
#include "ImiTech.h"
#include "afxwin.h"

//#ifdef  USE_IMI_1394
// CCameraControl 대화 상자입니다.
#define CAM_CTRL_FILE  "cam_ctrl.bin"
class CCamFeature
{
public:
	CCamFeature(){}
	~CCamFeature(){}
	enum
	{
 		BRIGHTNESS=0,		
		SHARPNESS	,	
		GAMMA		,	
		SHUTTER 	,	
		GAIN		
	};
};
/////////////////////////////////////////////////////////////////////////////
// CCameraControl dialog

class CCameraControl : public CDialog
{
// Construction
private:
	CStatic m_StRange[5];
public:
	CCameraControl(CWnd* pParent = NULL);   // standard constructor
    CImiTech *m_pImiBD;

	int m_Min[5];
	int m_Max[5];
 	int m_FeatureValue[3][5];
   	CEdit	m_Edit[5];
	CComboBox m_Combo;
	int  m_iCameraNum;

	int Load();
	void Save();

	void SetCamBD(CImiTech *pImi) { m_pImiBD=pImi; }

	void ChangeFeature(NMHDR *pNMHDR, LRESULT *pResult,int idx);
	void SetDefault(int Feature);

	void DeltaPosMove(NMHDR *pNMHDR, LRESULT *pResult,CEdit *pwEdit,double *dV,double low,double high,double offset);
	void DeltaPosMove(NMHDR *pNMHDR, LRESULT *pResult,CEdit *pwEdit,int *dV,int low,int high,int offset);

// Dialog Data
	//{{AFX_DATA(CCameraControl)
	enum { IDD = IDD_CAMERA_CONTROL };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCameraControl)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinCamctrl1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCamctrl2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCamctrl3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCamctrl4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCamctrl5(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButtonSetdefault1();
	afx_msg void OnBnClickedButtonSetdefault2();
	afx_msg void OnBnClickedButtonSetdefault3();
	afx_msg void OnBnClickedButtonSetdefault4();
	afx_msg void OnBnClickedButtonSetdefault5();

	afx_msg void OnCbnSelchangeComboCamera();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//#endif

#endif // !defined(AFX_CAMERACONTROL_H__836A2ED3_AC44_4356_888C_2EAF50B3D551__INCLUDED_)
