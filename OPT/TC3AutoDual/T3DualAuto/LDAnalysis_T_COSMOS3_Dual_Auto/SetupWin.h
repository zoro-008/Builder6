#if !defined(AFX_SETUPWIN_H__9453E458_AF84_4F90_9EEA_9A9FE93B18E6__INCLUDED_)
#define AFX_SETUPWIN_H__9453E458_AF84_4F90_9EEA_9A9FE93B18E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupWin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetupWin dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xskinbutton.h"
#include "..\\guisrc\\StaticSkin.h"

typedef struct 
{
	double	nPDist; // distance from beam center to peak center
	double  nPDist2;
	int		nDeckThr; // angle spot recognition
	int		nDispType; // display type
	double	nErrorLimit; // angle error limit
	int		nRDir;       // angle dir
	int		nTDir;       
	int		nMinute;     // angle scale by minute  
	int		nPixel;      // angle scale by pixel
	int		nBDispMode;  // beam display mode
	int		nBThr;       // beam threshold value
	double	nDiameterCenter; // diameter center
	double	nDiameterHigh;   // diameter upper limit
	double	nDiameterLow;    // diameter lower limit
	double	nLocation;       // beam location
	double	nParallel;       // peak beam parallelism
	double	nTiltCenter;     // tilt center limit
	double	nTiltHigh;       // tilt upper limit
	double	nTiltLow;        // tilt lower limit 
	double  nBSize;          // beam size scale
	int     nBPixel;         // beam pixel scale
	double  nRimHP;          // hrim
	int     nPThr;           // peak beam threshold
	double  nWCorrect;       // vertical width correction value
	double  nWCorrect2;      // horizontal width correction value
	double  nDataThr;        // data filtering value
	int     nRimLocate;      
	int     nRimInspect;
	double  nOlSize;
	double  nRimVP; // vrim
	int     nASt;
	int     nLSt;
	int     nPSt;
	int     nHRSt; //h rim
	int     nDSt;
	int     nTSt;
	int     nHSt;
	int     nVRSt; //v rim
	int     nACX;
	int     nACY;
	int     nACenterFlag;
//	double  nAngleErrorLimitHigh;
}SETTING_DATA;

typedef struct {
	double nRErrorLimitHigh;
	double nTErrorLimitLow;
	double nTErrorLimitHigh;

	double nRErrorLimitLow2; 
	double nRErrorLimitHigh2;
	double nTErrorLimitLow2;
	double nTErrorLimitHigh2;
	char rev[240];
}SETTING_DATA_ADD;

#define SETTING_FILE "setting.bin"
#define SETTING_FILE_ADD "setting2.bin"

class CSetupWin : public CDialog
{
// Construction
private:
	int   m_nASt;
	int   m_nLSt;
	int   m_nPSt;
	int   m_nHRSt;
	int   m_nVRSt;
	int   m_nDSt;
	int   m_nTSt;
	int   m_nHSt;

public:
	CSetupWin(CWnd* pParent = NULL);   // standard constructor
	CRect m_wndRect;
	CRect m_CtrlRect;
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
//original
	CStaticSkin     *m_pwStatic[11][2];
	CEdit           *m_pwEdit[11][2];
	CSpinButtonCtrl *m_pwSpin[11][2];

	CEdit           *m_pwRErrorHighEdit;
	CSpinButtonCtrl *m_pwRErrorHighSpin;

	CEdit           *m_pwTErrorLowEdit;
	CSpinButtonCtrl *m_pwTErrorLowSpin;
	CEdit           *m_pwTErrorHighEdit;
	CSpinButtonCtrl *m_pwTErrorHighSpin;

	CStaticSkin     *m_pwRT2           ;
	CEdit           *m_pwRErrorLowEdit2;
	CSpinButtonCtrl *m_pwRErrorLowSpin2;
	CEdit           *m_pwRErrorHighEdit2;
	CSpinButtonCtrl *m_pwRErrorHighSpin2;

	CEdit           *m_pwTErrorLowEdit2;
	CSpinButtonCtrl *m_pwTErrorLowSpin2;
	CEdit           *m_pwTErrorHighEdit2;
	CSpinButtonCtrl *m_pwTErrorHighSpin2;

	CEdit           *m_pwWcEdit;
	CSpinButtonCtrl *m_pwWcSpin;

	CEdit           *m_pwPdEdit;
	CSpinButtonCtrl *m_pwPdSpin;

    CxSkinButton    *m_pwButton[3];
	CxSkinButton    *m_pwCheckButton[8];
	CStaticSkin     *m_pwStaticEx[2];
	CEdit           *m_pwEditEx[2][3];
	CSpinButtonCtrl *m_pwSpinEx[2][3];
   
	CxSkinButton  *m_pwDefaultButton;


	SETTING_DATA      m_cActiveData;
	SETTING_DATA_ADD  m_cActiveDataAdd;
////////////////////////////////////////
	int   m_nACenterFlag;
	int   m_nACX;
	int   m_nACY;
// control function
	void MakeGui();
	void SetABSCenter();
	void GetSettingData();
	void SetSettingData();
	int  ReadSettingFile();
	void SaveSettingFile();
	void UpdateViewWindowData();
	void SettingDataCheck();
    void DeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult,CEdit *pEdit,int *pData,int low,int high,int inc); 
    void DeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult,CEdit *pEdit,double *pData,double low,double high,double inc); 
 
	inline void CSetupWin::SetWndRect(CRect r)      { m_wndRect=r;   }

// Dialog Data
	//{{AFX_DATA(CSetupWin)
	enum { IDD = IDD_DIALOG_TABSETUP };
	int		m_nDeckThr;
	int		m_nDispType;
	double	m_nErrorLimit;
	double  m_nRErrorLimitHigh;
	double  m_nTErrorLimitLow;
	double  m_nTErrorLimitHigh;

    double	m_nRErrorLimitLow2;
	double  m_nRErrorLimitHigh2;
	double  m_nTErrorLimitLow2;
	double  m_nTErrorLimitHigh2;




	int		m_nRDir;
	int		m_nTDir;
	int		m_nMinute;
	int		m_nPixel;
	int		m_nBDispMode;
	int		m_nBThr;
	double	m_nLocation;
	double	m_nParallel;
	double	m_nTiltCenter;
	double	m_nTiltHigh;
	double	m_nTiltLow;
	double	m_nDiameterCenter;
	double	m_nDiameterHigh;
	double	m_nDiameterLow;
	int		m_nBPixel;
	double	m_nBSize;
	double	m_nRimHP;
	int		m_nPThr;
	int		m_nRimInspect;
	double	m_nWCorrect;
	int		m_nRimLocate;
	double	m_nRimVP;
	double	m_nOlSize;
	double	m_nPDist;
	double	m_nWCorrect2;
	double	m_nDataThr;
	double	m_nPDist2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupWin)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupWin)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDeltaposSpinDisptype(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinTdir(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinRdir(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinError(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinError2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinError3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinError4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinDeckthr(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinMinute(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinPixel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSetupSave();
	afx_msg void OnButtonSetupApply();
	afx_msg void OnButtonPasswd();
	afx_msg void OnDeltaposSpinBdisp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinBthr(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinDiac(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinDiah(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinDial(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinLoc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinPara(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinTiltc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinTilth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinTiltl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinBsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinBpixel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinRimp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinPthr(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinDatathr(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinRiminspect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinRimlocate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinWidthcorrect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinOlsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinPdata(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckButtonAlimit();
	afx_msg void OnCheckButtonDiameter();
	afx_msg void OnCheckButtonDistance();
	afx_msg void OnCheckButtonParallelism();
	afx_msg void OnCheckButtonRim();
	afx_msg void OnCheckButtonTilt();
	afx_msg void OnDeltaposSpinPdist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckButtonPdist();
	afx_msg void OnCheckButtonRim2();
	afx_msg void OnDeltaposSpinWidthcorrect2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinPdist2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSetupDefault();
	afx_msg void OnOutofmemorySpinError5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOutofmemorySpinError6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOutofmemorySpinError7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOutofmemorySpinError8(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPWIN_H__9453E458_AF84_4F90_9EEA_9A9FE93B18E6__INCLUDED_)
