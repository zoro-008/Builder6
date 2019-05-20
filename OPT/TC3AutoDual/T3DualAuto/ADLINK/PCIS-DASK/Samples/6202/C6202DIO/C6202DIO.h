// C6202DIO.h : main header file for the C6202DIO application
//

#if !defined(AFX_C6202DIO_H__74EFE087_B46F_4E7F_8218_F37BB2FBC0DA__INCLUDED_)
#define AFX_C6202DIO_H__74EFE087_B46F_4E7F_8218_F37BB2FBC0DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CC6202DIOApp:
// See C6202DIO.cpp for the implementation of this class
//

class CC6202DIOApp : public CWinApp
{
public:
	CC6202DIOApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC6202DIOApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CC6202DIOApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C6202DIO_H__74EFE087_B46F_4E7F_8218_F37BB2FBC0DA__INCLUDED_)
