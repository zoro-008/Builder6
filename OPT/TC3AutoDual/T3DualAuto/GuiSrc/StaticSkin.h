//#if !defined(AFX_STATICSKIN_H__49031DEE_8770_4CB1_9E87_98D56FC0E0B9__INCLUDED_)
//#define AFX_STATICSKIN_H__49031DEE_8770_4CB1_9E87_98D56FC0E0B9__INCLUDED_

#if !defined(_STATIC_SKIN_H)
#define _STATIC_SKIN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticSkin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticSkin window
#include "BmpRgn.h"

class CStaticSkin : public CStatic
{
// Construction
public:
	CStaticSkin();

// Attributes
public:
	CBmpRgn *pBmpRgn;
	HDC m_dcBkGrnd;
	long m_nWindowWidth,m_nWindowHeight;
	char *m_BmpFile;
	CFont *m_Font;
	char m_pMsg[256];
    char m_pContMsg[10][256];
	UINT m_nFgColor;
	void SetBmpFile(char *pBmpName);
	void OutText(char *pStr);
	void OutTextII(char *pStr);
	void SetFgColor(UINT nColor);
	void OutTextXY(int x, int y,char *pStr);
	void SetFontSize(int w,int h,char *fname);
	void OutTextContXY(int x, int y,char *pStr,int idx);

	int  m_nContMsgFlag;
	int  m_nContMsgCount;
	int  m_nX,m_nY;
	int  m_nCX[10],m_nCY[10];
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticSkin)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticSkin();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticSkin)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICSKIN_H__49031DEE_8770_4CB1_9E87_98D56FC0E0B9__INCLUDED_)
