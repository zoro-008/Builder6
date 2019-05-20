#if !defined(AFX_IMGDISPLAY2_H__1523E517_6B26_44A5_B20C_3C807D35E235__INCLUDED_)
#define AFX_IMGDISPLAY2_H__1523E517_6B26_44A5_B20C_3C807D35E235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImgDisplay2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ±§√‡ øµªÛ 

#include "..\\guisrc\\LineProc.h"
#include "setupwin.h"
#include "EtcWindow.h"
#include "External.h"

#define MAX_MINUTE_IMG2 80
#define DISTANCE_SCALE_IMG2 12.0


 // #define GAUSSIAN_FITTING_ENABLE

class CImgDisplay2 : public CDialog
{
// Construction
private:
	LOGPALETTE *m_pLogPalette;
	BITMAPINFOHEADER m_BitmapInfoHeader;
	PALETTEENTRY m_aBitmapPalette[256];
	HPALETTE m_hPal,m_hOldPal;
    CRITICAL_SECTION m_pImage2 ;
public:
	SETTING_DATA     m_cSettingData;
	SETTING_DATA_ADD m_cSettingDataAdd;
    SETTING_DATAEX   m_cSettingDataEx;

	double m_nCx,m_nCy;
	double m_nScaleFactor;
	int m_nFindFlag;
	RECT m_cBeamRect[3];
	usrPOINT m_cCpt[3];

	int m_nAxisBeamSize;
	
//for dvd data display
	double m_nAngle;
	double m_nTilt;
	usrPOINT m_cAngle;

	int m_nAngleOKNG;
#ifdef __SAMSUNG_TSST_VERSION
	double m_nAngleCD;
	double m_nTiltCD;
	usrPOINT m_cAngleCD;
#endif
	double m_nDPeak,m_nSigma;
	double m_nDCx,m_nDCy;
	int m_nSaveBmp;
	
public:
	LINE_DATA_POS m_cSHp[MAX_MINUTE_IMG2+1];
	LINE_DATA_POS m_cSHm[MAX_MINUTE_IMG2+1];
	LINE_DATA_POS m_cSVp[MAX_MINUTE_IMG2+1];
	LINE_DATA_POS m_cSVm[MAX_MINUTE_IMG2+1];
	
	LINE_DATA_POS m_cLHp[8];
	LINE_DATA_POS m_cLHm[8];
	LINE_DATA_POS m_cLVp[8];
	LINE_DATA_POS m_cLVm[8];

	LINE_DATA_POS m_cLHpG[8];
	LINE_DATA_POS m_cLHmG[8];
	LINE_DATA_POS m_cLVpG[8];
	LINE_DATA_POS m_cLVmG[8];
	usrPOINT m_cHsp,m_cHep,m_cVsp,m_cVep;

public:
	CImgDisplay2(CWnd* pParent = NULL);   // standard constructor
    unsigned char *m_pBitmapBuffer;
	unsigned char *m_pSrc;
	unsigned char *m_pOrigin;
	unsigned char *m_pOut;
	int m_nOkNgStatus;
	double m_nROffset,m_nTOffset;

    CRect m_wndRect;
	int  m_nDummy;
	int   m_nGridDisplayTag; // for display roi during continuous grab. 
	int   m_nWidth,m_nHeight;
/////////////////////////////////////////////////////////////////////
// coordinate system re-definition.
	usrPOINT m_cMinuteCoordinate[480][640];   // minute coordinate
	usrPOINT m_cDistanceCoordinate[480][640]; // distance coordinate
/////////////////////////////////////////////////////////////////////

	void BitmapInit();
	void ReSetBitmapInfo(int nWidth,int nHeight);
	void Draw256Image(CPaintDC *dc,unsigned char *pImg);
	void SetBuffer(unsigned char *pImg);
	void ImgUpdate(unsigned char *pImg);
	int  ProcessThreeBeam(EN_TEST_MODE iTestMode = tmNormal);

	void ComputeDvdData();
	void ComputeCDData();
	double ComputeAngle();

	void SettingParam();
	void SelectDisplayType();
	void MakeCoordTable();
	void MakeGridCoord();
	void DisplayGrid(CDC *memDC);
	void MinuteToPixel(double rValue,double tValue,POINT *coord);

	double LDGaussian(double s,double v,double m) ;
	double   ComputeCenter(double *nData,int count,int start,int end);
	void RunSaveBmp(char *fname);
	void SaveBmp(CBitmap *pBitmap,char *fname);

	int CheckErrorFlag();

	void ComputeGaussianCenter(RECT r,POINT cp,POINT *out);
	void ImageTanslate(int x,int y,unsigned char *pImg);

	inline void CImgDisplay2::ResetFindFlag()              { m_nFindFlag=0;             }
	inline void CImgDisplay2::SetOrgRect(CRect r)          { m_wndRect=r;               }
	inline void CImgDisplay2::SetGridDisplayTag(int nTag)  { m_nGridDisplayTag=nTag;    }
	inline void CImgDisplay2::SetBufSize(int nW,int nH)    { m_nWidth=nW; m_nHeight=nH; }
	inline int CImgDisplay2::SetBmpSaveFlag(int flag)      { m_nSaveBmp=flag ;          }
	inline void CImgDisplay2::ResetOkNgFlag()              { m_nOkNgStatus=1;           }
	inline void CImgDisplay2::SetOffset(double R, double T){ m_nROffset=R; m_nTOffset=T;}
// Dialog Data
	//{{AFX_DATA(CImgDisplay2)
	enum { IDD = IDD_IMG2_DISPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgDisplay2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImgDisplay2)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGDISPLAY2_H__1523E517_6B26_44A5_B20C_3C807D35E235__INCLUDED_)
