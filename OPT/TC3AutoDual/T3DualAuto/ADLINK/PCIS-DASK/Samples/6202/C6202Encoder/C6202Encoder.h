// C6202Encoder.h : main header file for the C6202ENCODER application
//

#if !defined(AFX_C6202ENCODER_H__8A48C31B_1C80_4309_8B3A_9DEEB6E7963B__INCLUDED_)
#define AFX_C6202ENCODER_H__8A48C31B_1C80_4309_8B3A_9DEEB6E7963B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CC6202EncoderApp:
// See C6202Encoder.cpp for the implementation of this class
//

class CC6202EncoderApp : public CWinApp
{
public:
	CC6202EncoderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC6202EncoderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CC6202EncoderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C6202ENCODER_H__8A48C31B_1C80_4309_8B3A_9DEEB6E7963B__INCLUDED_)
