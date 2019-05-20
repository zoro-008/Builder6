// C6202GPTC.h : main header file for the C6202GPTC application
//

#if !defined(AFX_C6202GPTC_H__5985750C_A487_4DA8_8C11_FD6E168F19A1__INCLUDED_)
#define AFX_C6202GPTC_H__5985750C_A487_4DA8_8C11_FD6E168F19A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CC6202GPTCApp:
// See C6202GPTC.cpp for the implementation of this class
//

class CC6202GPTCApp : public CWinApp
{
public:
	CC6202GPTCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC6202GPTCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CC6202GPTCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C6202GPTC_H__5985750C_A487_4DA8_8C11_FD6E168F19A1__INCLUDED_)
