// C6202AODoubleBuffer.h : main header file for the C6202AODOUBLEBUFFER application
//

#if !defined(AFX_C6202AODOUBLEBUFFER_H__24D00059_F4F8_4631_AE42_BF27FFA12009__INCLUDED_)
#define AFX_C6202AODOUBLEBUFFER_H__24D00059_F4F8_4631_AE42_BF27FFA12009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CC6202AODoubleBufferApp:
// See C6202AODoubleBuffer.cpp for the implementation of this class
//

class CC6202AODoubleBufferApp : public CWinApp
{
public:
	CC6202AODoubleBufferApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC6202AODoubleBufferApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CC6202AODoubleBufferApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C6202AODOUBLEBUFFER_H__24D00059_F4F8_4631_AE42_BF27FFA12009__INCLUDED_)
