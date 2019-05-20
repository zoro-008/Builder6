/******************************************************
 Project Name :
 User         :
 Environment  :
 System spec. :
 Function     : 
 PRJ finished : 
*******************************************************/
// HVSystem.h : main header file for the HVSYSTEM application
//

#if !defined(AFX_HVSYSTEM_H__DA933D7A_37B6_4A21_8BF8_B0CDA9B93A5E__INCLUDED_)
#define AFX_HVSYSTEM_H__DA933D7A_37B6_4A21_8BF8_B0CDA9B93A5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


#define WINDOW_XSIZE 1280		
#define WINDOW_YSIZE 1024

//100 -> 50
#define PI_VALUE       3.1415926535897932384626433832795
/*****************************************/
/* File load o/*****************************************/
/*peration instead of camera */

//20101216 lwb �¾��ÿ��� FILE_LOAD_PROCESS/__MATROX_VISION_WITH_VGA ��� ������.
//#define FILE_LOAD_PROCESS 
//#define __MATROX_VISION_WITH_VGA


//20101216 lwb lockkey�� ����Ʈ����� ��ü�Ѵ�.
// #ifndef FILE_LOAD_PROCESS
// #define __HASP_KEY
// #endif


#define UV_HOYA


//#define SANTA_BEAM_AXIS_CHANGE

// #define ���� ���� ��� ���� 1 �˻�� 

 
//#define _WALTZ2 // ���� 2�� ���� �� ���� �˻��  
//#define _MOVE_IMAGE


//#define __SAMSUNG_TSST_VERSION  // 2009�� 2�� 9��  ��� ���� 


//#define __LG_SIMPLE_VERSION // �ܼ� �˻�� 

#define SETUP_FILE ".\\setting\\paramsetup.bin"
#define SETUP_DIR  ".\\setting\\"
#define PASSWORD_FILE "passwd.txt"

class CHVSystemApp : public CWinApp
{
public:
	CHVSystemApp();

// Overrides
	// ClassWizard generated virtual function overrides3
	//{{AFX_VIRTUAL(CHVSystemApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	int m_nTestCount;
// Implementation
	//{{AFX_MSG(CHVSystemApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_HVSYSTEM_H__DA933D7A_37B6_4A21_8BF8_B0CDA9B93A5E__INCLUDED_)
