// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__38EBED23_7FAA_4C92_B24D_E484A3136C81__INCLUDED_)
#define AFX_MAINFRM_H__38EBED23_7FAA_4C92_B24D_E484A3136C81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    void OnSendData(int cmd,  
		            double Para1  = 0.0 , double Para2 = 0.0 , int    Para3  = 0  , double Para4  = 0.0, int    Para5  = 0  , double Para6  = 0  , int    Para7  = 0  , double Para8  = 0  , int    Para9  = 0  , double Para10  = 0.0, double Para11 = 0.0 , int Para12 = 0, 
		            double SPara1 = 0.0 , double SPara2 = 0.0, double SPara3 = 0.0, double SPara4 = 0.0, double SPara5 = 0.0, double SPara6 = 0.0, double SPara7 = 0.0, double SPara8 = 0.0, double SPara9 = 0.0, double SPara10 = 0.0,
					double SPara11= 0.0 , double SPara12= 0.0, double SPara13= 0.0, double SPara14= 0.0, double SPara15= 0.0, double SPara16= 0.0, double SPara17= 0.0, double SPara18= 0.0);//, CString SPara19="" );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__38EBED23_7FAA_4C92_B24D_E484A3136C81__INCLUDED_)
