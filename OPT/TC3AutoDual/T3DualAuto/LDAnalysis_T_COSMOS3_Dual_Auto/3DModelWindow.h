#if !defined(AFX_3DMODELWINDOW_H__35F5774C_F379_4D1D_B9F4_799C4C829EC1__INCLUDED_)
#define AFX_3DMODELWINDOW_H__35F5774C_F379_4D1D_B9F4_799C4C829EC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DModelWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DModelWindow dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\StaticSkin.h"
#include "..\\guisrc\\xSkinButton.h"
#include "OpenGl3D.h"

typedef struct {
	int    nf;
	float nzf;
	int nlp[11];
}PARAM_3DMODEL;

class C3DModelWindow : public CDialog
{
// Construction
private:
	CRect m_wndRect;
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	long m_nMsgIndex;
	COpenGl3D m_w3DDisp;

	CStaticSkin     *m_pwStatic[13];
	CEdit           *m_pwEdit[13];
	CSpinButtonCtrl *m_pwSpin[13];
    CxSkinButton    *m_pwSaveButton;

	PARAM_3DMODEL m_cModel;

public:
	C3DModelWindow(CWnd* pParent = NULL);   // standard constructor
	CxSkinButton *m_wSysMenu;

	int m_nWidth,m_nHeight;
	void SetBuffer(unsigned char *pImg);
	void MakeViewParamWindow();
	void GetParam();
	void SetParam();
	void SaveParam();
	void ReadParam();
	void ChangeModelWindowView(int cmdShow);
	inline void C3DModelWindow::SetBufSize(int xw,int yw) { m_nWidth=xw; m_nHeight=yw; }
	inline void C3DModelWindow::SetWndRect(CRect r)  { m_wndRect=r;  }
	inline void C3DModelWindow::SetWndRect(int nL,int nT,int nR,int nB){ m_wndRect.SetRect(nL,nT,nR,nB); }
	inline void C3DModelWindow::GetWndRect(CRect *r) { *r=m_wndRect; }

// Dialog Data
	//{{AFX_DATA(C3DModelWindow)
	enum { IDD = IDD_DIALOG_3DMODEL };
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
	int		m_nEmission;
	int		m_nShinness;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DModelWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(C3DModelWindow)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDeltaposSpinFillMode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinZTrans(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinLightXPos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinLightYPos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinLightZPos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinAmbientLight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinDiffuseLight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSpecularLight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinAmbientMaterial(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinDiffuseMaterial(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSpecularMaterial(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinShinness(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinEmission(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void On3dmodelSave();
	afx_msg void OnButtonSysmenu3dmodel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DMODELWINDOW_H__35F5774C_F379_4D1D_B9F4_799C4C829EC1__INCLUDED_)
