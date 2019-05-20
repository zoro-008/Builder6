// DialogTabMMotion.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "DialogTabMMotion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTabMMotion dialog


CDialogTabMMotion::CDialogTabMMotion(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTabMMotion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTabMMotion)
	
	//}}AFX_DATA_INIT
}


void CDialogTabMMotion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTabMMotion)
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTabMMotion, CDialog)
	//{{AFX_MSG_MAP(CDialogTabMMotion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTabMMotion message handlers

BOOL CDialogTabMMotion::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
