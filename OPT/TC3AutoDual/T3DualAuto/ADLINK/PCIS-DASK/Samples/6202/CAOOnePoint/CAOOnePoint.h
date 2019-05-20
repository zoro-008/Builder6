// CAOOnePoint.h : main header file for the CAOONEPOINT application
//

#if !defined(AFX_CAOONEPOINT_H__19A9B244_4221_4E4E_9EED_D2C192BF5B0F__INCLUDED_)
#define AFX_CAOONEPOINT_H__19A9B244_4221_4E4E_9EED_D2C192BF5B0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCAOOnePointApp:
// See CAOOnePoint.cpp for the implementation of this class
//

class CCAOOnePointApp : public CWinApp
{
public:
	CCAOOnePointApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCAOOnePointApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCAOOnePointApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAOONEPOINT_H__19A9B244_4221_4E4E_9EED_D2C192BF5B0F__INCLUDED_)
