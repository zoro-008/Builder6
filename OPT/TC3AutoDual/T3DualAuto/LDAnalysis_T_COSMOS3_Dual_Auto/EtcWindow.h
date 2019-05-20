#if !defined(AFX_ETCWINDOW_H__5D21156A_91F1_4A5C_A4C2_DAD15827A7EE__INCLUDED_)
#define AFX_ETCWINDOW_H__5D21156A_91F1_4A5C_A4C2_DAD15827A7EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EtcWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEtcWindow dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\xSkinButton.h"
#include "..\\guisrc\\StaticSkin.h"

typedef struct
{
	int    iPixel;
	double dDistance;
	int    iDispMode;
	int    iBThr;
	int    iEThr;
	double dGTShift;
	double dGTOffset;
	RECT   r;
	double dGTShiftHigh;
	double dGTShift2;
	double dGTShiftHigh2;
	double dGTShift3;
	double dGTShiftHigh3;
	int    iBeamSize ;
	int    iBeamSizeHigh ;
	int    iApcOk ;
	int    iApcOkHigh ;

}SETTING_DATAEX;

typedef struct
{
	int iBPort;
	int iSaveMode; // 0: after UV, 1: before UV, 2: both
	char rev[252];
}SETTING_DATAEX2;

#define SET_ETC_FILE "Setting_etc.bin"
#define SET_ETC2_FILE "Setting_etc2.bin"


class CEtcWindow : public CDialog
{
// Construction
private:
    CRect m_wndRect;
	int m_nWorkMode;
    SETTING_DATAEX m_setEtc;
    SETTING_DATAEX2 m_setEtc2;
	CFont *m_pFont;
public:
	
	CEtcWindow(CWnd* pParent = NULL);   // standard constructor
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;

	CStaticSkin     *m_pwName[2];
	CEdit           *m_pwRT[2];
	CSpinButtonCtrl *m_pwSpin[2];

	CStaticSkin		*m_pwStaticEx[11];
	CEdit			*m_pwEditEx[11];
	CSpinButtonCtrl *m_pwSpinEx[11];


	CStaticSkin     *m_pwStaticExAdd[5];
	CEdit           *m_pwEditExAdd[5];
	CSpinButtonCtrl *m_pwSpinExAdd[5];

	CxSkinButton *m_pwButtonMaster;
	CxSkinButton *m_pwButtonRoi;

	CStatic      *m_pwEtched;
	CxSkinButton *m_pwButton[2];

	CStaticSkin  *m_pwStCom[2];
	CComboBox    *m_pwCombo[2];

	void MakeGui_1();
	void MakeGui_12();
	void MakeGui_2();
	int  LoadFile();
	void SetDefault();
	void GetData(SETTING_DATAEX *p);
	void SetData(SETTING_DATAEX *p);
	void GetData2(SETTING_DATAEX2 *p);
	void SetData2(SETTING_DATAEX2 *p);

	void UpdateViewWindowData();

	void DeltaPos(NMHDR* pNMHDR, LRESULT* pResult,CEdit *pEdit,double *v,double low,double high,double inc);
	void DeltaPos(NMHDR* pNMHDR, LRESULT* pResult,CEdit *pEdit,int *v,int low,int high,int inc);

	inline void   CEtcWindow::SetWindowRect(CRect r) { m_wndRect = r;      }
	inline double CEtcWindow::GetRoffset()           { return m_nROffset;  }
	inline double CEtcWindow::GetTOffset()           { return m_nTOffset;  }
	inline int    CEtcWindow::GetWorkMode()          { return m_nWorkMode; } 

	// Dialog Data
	//{{AFX_DATA(CEtcWindow)
	enum { IDD = IDD_DIALOG_TABETC };
	double	m_nROffset;
	double	m_nTOffset;
	int     m_iPixel;
	double  m_dDistance;
	int     m_iDispMode;
	int     m_iBThr;
	int     m_iEThr;
	double  m_dGTShift;
	double  m_dGTShiftHigh;
	double  m_dGTShift2;//재검사할때 옵션.
	double  m_dGTShiftHigh2;//재검사 할때 씀.
	double  m_dGTShift3;//재검사할때 옵션.
	double  m_dGTShiftHigh3;//재검사 할때 씀.
	double  m_dGTOffset;
	double  m_iApcOk ;
	double  m_iApcOkHigh ;
	int     m_iBeamSize;
	int     m_iBeamSizeHigh;
	int     m_iBcPort;
	int     m_iSaveMode;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtcWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEtcWindow)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonApply();
	afx_msg void OnDeltaposSpinToffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinRoffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonMastermode();
	afx_msg void OnDeltaposSpinEtc1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc8(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckEtcRoi();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnDeltaposSpinEtc9(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc10(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc11(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc12(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc13(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc14(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc16(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEtc15(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETCWINDOW_H__5D21156A_91F1_4A5C_A4C2_DAD15827A7EE__INCLUDED_)
