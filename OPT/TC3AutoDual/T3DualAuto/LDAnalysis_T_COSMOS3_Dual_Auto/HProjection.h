#if !defined(AFX_HPROJECTION_H__FCC6D044_C911_4A69_A441_721D4F11DFBE__INCLUDED_)
#define AFX_HPROJECTION_H__FCC6D044_C911_4A69_A441_721D4F11DFBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HProjection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHProjection dialog
#include "SetupWin.h"
#include "EtcWindow.h"

class CHProjection : public CDialog
{
// Construction
private:
	BITMAPINFOHEADER m_BitmapInfoHeader;
	int m_nWidth,m_nHeight;
	int m_nDummy;
	SETTING_DATA    m_cSettingData   ;
	SETTING_DATAEX  m_cSettingDataEx ;

	double m_nScaleFactor;
	int m_nBWidthCriteria;
	int m_nBWidthUpper;
	int m_nBWidthLower;
	double m_nCx,m_nCy;

	int  m_nDCx,m_nDCy;
	double m_nDPeak;
	double m_nDWidth;
	int m_nLow,m_nHigh;
	double m_nLowValue,m_nHighValue;
	int m_nGaussianDrawTag;
	unsigned char m_nValue[640];
    double m_nMaxIntensity;
	double m_nSigma;
	int m_nOriginLow,m_nOriginHigh;
	POINT m_cBCpt;
	double m_nDataThr;
	int    m_nStartDataLoc,m_nEndDataLoc;
	double m_nRimLowValue,m_nRimHighValue;

	RGBQUAD m_cColorIndex[256];
public:
	CHProjection(CWnd* pParent = NULL);   // standard constructor
	double m_nHRimValue;
    unsigned char *m_pBitmapBuffer;
	void   RimLocate(int nValue);
	void   ReCompute(double datathr,double wcorrect);
	void   BitmapInit();
	void   DrawGrid(CPaintDC *dc);
	void   DrawGaussian(CDC *pDC);
    double NormGaussian(double s, double v, double m);
	void   SetGaussianData(unsigned char *pIn,POINT cpt,POINT bcpt,int maxv,int low, int high);
	void   SetParam();
	void   SortValue(double *pArr, int d, int h, BOOL bAscending);
	double LDGaussian(double s,double v,double m) ;
	void   ComputeData(double *nData,int count,int flag,double wcc=0.0);

	void DrawLevel(CDC *pDC,int v);
	void MakeColor();

	void PaintEx();

	inline double CHProjection::Gaussian(double nMaxV,double nW,double nU){	return (nMaxV*exp(-2.0*nU*nU/(nW*nW/4)));}
	inline int CHProjection::GetDistanceToPixel(double nD) { return (int)(nD/m_nScaleFactor+0.5);}
	inline int CHProjection::GetCenter() { return m_nDCx; }
	inline int CHProjection::GetMaxIntensity() { return m_nMaxIntensity; }

// Dialog Data
	//{{AFX_DATA(CHProjection)
	enum { IDD = IDD_DIALOG_HPROJ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHProjection)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHProjection)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HPROJECTION_H__FCC6D044_C911_4A69_A441_721D4F11DFBE__INCLUDED_)
