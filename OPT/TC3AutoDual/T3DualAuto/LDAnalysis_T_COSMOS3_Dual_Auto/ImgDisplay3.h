#if !defined(AFX_IMGDISPLAY3_H__B1E6E7E6_8CB3_4D2F_9849_741A00DE102B__INCLUDED_)
#define AFX_IMGDISPLAY3_H__B1E6E7E6_8CB3_4D2F_9849_741A00DE102B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImgDisplay3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImgDisplay3 dialog
#include "ROISet.h"
#include "..\\guisrc\\LineProc.h"
#include "setupwin.h"
#include "etcWindow.h"
#include "..\\include\\Kmorph.h"
#include "External.h"
#define  MAX_GRID_NUM 40

class CImgDisplay3 : public CDialog
{
// Construction
private:
	LOGPALETTE *m_pLogPalette;
	BITMAPINFOHEADER m_BitmapInfoHeader;
	PALETTEENTRY m_aBitmapPalette[256];
	HPALETTE m_hPal,m_hOldPal;
	int m_nMouseDoubleClick;
	double m_nScaleFactor;
    CRITICAL_SECTION m_pImage3 ;
public:
	usrPOINT m_cDistanceCoordinate[480][640]; // distance coordinate
	int m_nSaveBmp;
///////////////////////////////////
// normal beam

	SETTING_DATA m_cSettingData;
	SETTING_DATAEX m_cSettingDataEx;
	double m_nCx,m_nCy;
//////////////////////////////////
	RECT m_cBeamRect;
	usrPOINT m_nBeamPos;
	POINT m_cCpt;
	POINT m_cGTPoint;
	usrPOINT m_cGTPos;
	double m_dGTShift;
	int m_nFindFlag;
	int *m_iProj;

	LINE_STRUCT m_cGTLine;
public:
	CROISet m_cROI;
	int m_iRoiSet;
	int m_iMouseDown;

	inline void CImgDisplay3::SetRoiFlag(int flag) { m_iRoiSet=flag; }

public:

	int m_nOkNgStatus;

	LINE_DATA_POS m_cSHp[MAX_GRID_NUM+1];
	LINE_DATA_POS m_cSHm[MAX_GRID_NUM+1];
	LINE_DATA_POS m_cSVp[MAX_GRID_NUM+1];
	LINE_DATA_POS m_cSVm[MAX_GRID_NUM+1];
	
	LINE_DATA_POS m_cLHp[4];
	LINE_DATA_POS m_cLHm[4];
	LINE_DATA_POS m_cLVp[4];
	LINE_DATA_POS m_cLVm[4];

	LINE_DATA_POS m_cLHpG[4];
	LINE_DATA_POS m_cLHmG[4];
	LINE_DATA_POS m_cLVpG[4];
	LINE_DATA_POS m_cLVmG[4];
	usrPOINT m_cHsp,m_cHep,m_cVsp,m_cVep;

public:
	CImgDisplay3(CWnd* pParent = NULL);   // standard constructor
    unsigned char *m_pBitmapBuffer;
	unsigned char *m_pSrc;
	unsigned char *m_pOrigin;
	unsigned char *m_pOut;
	unsigned char *m_pTmp;

    CRect m_wndRect;
	int  m_nDummy;
	int   m_nGridDisplayTag; // for display roi during continuous grab. 
	int   m_nWidth,m_nHeight;

	void BitmapInit();
	void ReSetBitmapInfo(int nWidth,int nHeight);
	void Draw256Image(CPaintDC *dc,unsigned char *pImg);
	void Draw256Image(CDC *dc,unsigned char *pImg);
	void SetBuffer(unsigned char *pImg);
	int  ProcessBeamDistribution(int flag=1);
	int  BeamDistributionAnalysis(int pflag=1,EN_TEST_MODE iTestMode=tmNormal);
	int  GetGTLocation(POINT pt);
	void SettingParam();
	void SelectDisplayType();
	void MakeCoordTable();
	void MakeGridCoord();
	void DisplayGrid(CDC *memDC);
	void ChangePallete();
	void DrawPallete(CDC *pDC);

void MorphImage(unsigned char *pSrc,unsigned char *pOut,RECT r);
void HorizEdge(unsigned char *pSrc,unsigned char *pTgt,
						 RECT r,int nWidth,int nThresh);
	void ImgUpdate(unsigned char *pImg);
	int DistanceToPixelPosY(double nDist);
	int DistanceToPixelPosX(double nDist);
	void SaveBmp(CBitmap *pBitmap,char *fname);
	void RunSaveBmp(char *fname);

	int CheckErrorFlag();

	inline void CImgDisplay3::ResetFindFlag()             { m_nFindFlag=0;  }
	inline void CImgDisplay3::SetOrgRect(CRect r)         { m_wndRect=r;                   }
	inline CRect CImgDisplay3::GetOrgRect()               { return m_wndRect;  }
	inline void CImgDisplay3::SetGridDisplayTag(int nTag) { m_nGridDisplayTag=nTag;        }
	inline void CImgDisplay3::SetBufSize(int nW,int nH)   { m_nWidth=nW; m_nHeight=nH; }
	inline int  CImgDisplay3::GetMode()                   { return (2-m_nMouseDoubleClick); }
	inline int  CImgDisplay3::DistanceToPixel(double nD)  { return (int)(nD/m_nScaleFactor+0.5); }

	inline int CImgDisplay3::SetBmpSaveFlag(int flag)     { m_nSaveBmp=flag ; }
	inline void CImgDisplay3::ResetOkNgFlag()             { m_nOkNgStatus= 1; }


// Dialog Data
	//{{AFX_DATA(CImgDisplay3)
	enum { IDD = IDD_IMG3_DISPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgDisplay3)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImgDisplay3)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGDISPLAY3_H__B1E6E7E6_8CB3_4D2F_9849_741A00DE102B__INCLUDED_)
