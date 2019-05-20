#if !defined(AFX_DIALOGMESSAGE_H__116C089A_09CD_4351_B8B1_312CB9E3E4FA__INCLUDED_)
#define AFX_DIALOGMESSAGE_H__116C089A_09CD_4351_B8B1_312CB9E3E4FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogMessage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMessage dialog
#include "..\\guisrc\\bmprgn.h"
#include "..\\guisrc\\StaticSkin.h"
#include "..\\guisrc\\xSkinButton.h"
class CDialogMessage : public CDialog
{
// Construction
public:
	CDialogMessage(CWnd* pParent = NULL);   // standard constructor

	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
    CStaticSkin *m_wStatic;
	CxSkinButton *m_wButton[2];
	CFont *m_Font;
	char m_nStr[10][256];
	int m_nMCount;
	int m_nOperationMode;

    int m_nCharWidth,m_nCharHeight;
    
	
	void SetMessage(char *m,int idx);
	void DrawMessage();

	inline void CDialogMessage::SetOperationMode(int nflag) {m_nOperationMode=nflag; }
	inline void CDialogMessage::SetFontSize(int nWidth,int nHeight) { m_nCharWidth=nWidth; m_nCharHeight=nHeight;}
// Dialog Data
	//{{AFX_DATA(CDialogMessage)
	enum { IDD = IDD_DIALOG_MESSAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMessage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogMessage)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGMESSAGE_H__116C089A_09CD_4351_B8B1_312CB9E3E4FA__INCLUDED_)
