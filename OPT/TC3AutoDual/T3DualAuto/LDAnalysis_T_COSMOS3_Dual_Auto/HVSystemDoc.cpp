// HVSystemDoc.cpp : implementation of the CHVSystemDoc class
//

#include "stdafx.h"
#include "HVSystem.h"

#include "HVSystemDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHVSystemDoc

IMPLEMENT_DYNCREATE(CHVSystemDoc, CDocument)

BEGIN_MESSAGE_MAP(CHVSystemDoc, CDocument)
	//{{AFX_MSG_MAP(CHVSystemDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHVSystemDoc construction/destruction

CHVSystemDoc::CHVSystemDoc()
{
	// TODO: add one-time construction code here

}

CHVSystemDoc::~CHVSystemDoc()
{
}

BOOL CHVSystemDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHVSystemDoc serialization

void CHVSystemDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHVSystemDoc diagnostics

#ifdef _DEBUG
void CHVSystemDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHVSystemDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHVSystemDoc commands
