#if !defined(AFX_IMGDISPLAY_H__46A9D65E_F5BC_4BAD_B3FA_C4B47F235C3B__INCLUDED_)
#define AFX_IMGDISPLAY_H__46A9D65E_F5BC_4BAD_B3FA_C4B47F235C3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImgDisplay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImgDisplay dialog
#include "ROISet.h"
#include "..\\guisrc\\LineProc.h"
#include "setupwin.h"
#define  MAX_GRID_NUM 40

#define USE_CAMERA_1 //1/3ÀÎÄ¡ 

class CImgDisplay : public CDialog
{
// Construction
private:
	LOGPALETTE *m_pLogPalette;
	BITMAPINFOHEADER m_BitmapInfoHeader;
	PALETTEENTRY m_aBitmapPalette[256];
	HPALETTE m_hPal,m_hOldPal;
	int m_nMouseDoubleClick;
	double m_nScaleFactor;
    CRITICAL_SECTION m_pImage1 ;

public:

	usrPOINT m_cDistanceCoordinate[480][640]; // distance coordinate
	int m_nSaveBmp;
	double m_dDistance;
///////////////////////////////////
// normal beam
public:
	SETTING_DATA m_cSettingData;
	double m_nCx,m_nCy;
//////////////////////////////////
	CROISet m_cROI;
	int m_nRoiSet;
	int m_nMouseDown;
///////////////////////////////////
	RECT m_cBeamRect;
	usrPOINT m_nBeamPos;
	POINT m_cCpt;
	int m_nFindFlag;
///////////////////////////////////
	RECT m_cPBeamRect;
	usrPOINT m_nPBeamPos;
	POINT m_cPCpt;
	int m_nPFindFlag;

	double m_nPBeamArea;
    double m_nBeamDiameter;
	double m_nBeamArea;
	double m_nPBeamDiameter;
	double m_nPAreaRatio;
	double m_nParallelism;
	double m_nOlRatio;
	double m_nOlRadius;
	double m_nBeamRadius;

	LINE_STRUCT m_cLongLine,m_cShortLine;
/////////////////////////////////////
	unsigned char *m_nVValue;
	unsigned char *m_nHValue;
	int m_nVRimLowPos;
	int m_nVRimHighPos;
	int m_nHRimLowPos;
	int m_nHRimHighPos;
	int m_nVMaxValue;
	int m_nHMaxValue;
/////////////////////////////////////
	RECT m_cOlRect;
/////////////////////////////////////
	usrPOINT m_cIPDist;


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
	CImgDisplay(CWnd* pParent = NULL);   // standard constructor
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
	int  BeamDistributionAnalysis(int pflag=1);
	int  ProcessManualBeamDistribution(int flag=1);
	int  GetPeakBeam();
	void SettingParam();
	void SelectDisplayType();
	void SetVValue(POINT cpt);
	void SetHValue(POINT cpt);
	void MakeCoordTable();
	void MakeGridCoord();
	void DisplayGrid(CDC *memDC);
	void ChangePallete();
	void DrawPallete(CDC *pDC);

	void MoveDelta(int x,int y);

	void ImgUpdate(unsigned char *pImg);
	int DistanceToPixelPosY(double nDist);
	int DistanceToPixelPosX(double nDist);
	void SaveBmp(CBitmap *pBitmap,char *fname);
	void RunSaveBmp(char *fname);

	int CheckErrorFlag();

	inline void CImgDisplay::ResetFindFlag()             { m_nFindFlag=0; m_nPFindFlag=0; }
	inline void CImgDisplay::SetOrgRect(CRect r)         { m_wndRect=r;                   }
	inline CRect CImgDisplay::GetOrgRect()               { return m_wndRect; }
	inline void CImgDisplay::SetGridDisplayTag(int nTag) { m_nGridDisplayTag=nTag;        }
	inline void CImgDisplay::SetBufSize(int nW,int nH)   { m_nWidth=nW; m_nHeight=nH; }
	inline int  CImgDisplay::GetMode()                   { return (2-m_nMouseDoubleClick); }
	inline int  CImgDisplay::DistanceToPixel(double nD)  { return (int)(nD/m_nScaleFactor+0.5); }

	inline int CImgDisplay::SetBmpSaveFlag(int flag)     { m_nSaveBmp=flag ; }
	inline void CImgDisplay::ResetOkNgFlag()             { m_nOkNgStatus= 1; }

   
	// Dialog Data
	//{{AFX_DATA(CImgDisplay)
	enum { IDD = IDD_IMG_DISPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgDisplay)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImgDisplay)
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

#endif // !defined(AFX_IMGDISPLAY_H__46A9D65E_F5BC_4BAD_B3FA_C4B47F235C3B__INCLUDED_)
