// C6202AOSingleBuffer.h : main header file for the C6202AOSINGLEBUFFER application
//

#if !defined(AFX_C6202AOSINGLEBUFFER_H__5F53D893_72E1_4142_A3C0_B4DDF9EE5740__INCLUDED_)
#define AFX_C6202AOSINGLEBUFFER_H__5F53D893_72E1_4142_A3C0_B4DDF9EE5740__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CC6202AOSingleBufferApp:
// See C6202AOSingleBuffer.cpp for the implementation of this class
//

class CC6202AOSingleBufferApp : public CWinApp
{
public:
	CC6202AOSingleBufferApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC6202AOSingleBufferApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CC6202AOSingleBufferApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C6202AOSINGLEBUFFER_H__5F53D893_72E1_4142_A3C0_B4DDF9EE5740__INCLUDED_)
