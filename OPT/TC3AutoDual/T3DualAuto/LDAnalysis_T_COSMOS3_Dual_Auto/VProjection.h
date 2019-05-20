#if !defined(AFX_VPROJECTION_H__279C5FF5_8AE0_4D3C_A6D3_BB577E88AB17__INCLUDED_)
#define AFX_VPROJECTION_H__279C5FF5_8AE0_4D3C_A6D3_BB577E88AB17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VProjection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVProjection dialog
#include "SetupWin.h"

class CVProjection : public CDialog
{
// Construction
private:
	BITMAPINFOHEADER m_BitmapInfoHeader;
	int m_nWidth,m_nHeight;
	int m_nDummy;
	SETTING_DATA m_cSettingData;
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
	double m_nRimLowValue,m_nRimHighValue;
	int m_nGaussianDrawTag;
	unsigned char m_nValue[480];
	double m_nSigma;
	double m_nAverage;
	double m_nMaxIntensity;
	int m_nOriginLow,m_nOriginHigh;
	POINT m_cBCpt;
	double m_nDataThr;
	int m_nStartDataLoc,m_nEndDataLoc;

	RGBQUAD m_cColorIndex[256];
public:
	CVProjection(CWnd* pParent = NULL);   // standard constructor
    double m_nVRimValue;

	unsigned char *m_pBitmapBuffer;
	void BitmapInit();
	void DrawGrid(CPaintDC *dc);
	void DrawGaussian(CDC *pDC);
	void DrawRim(CDC *pDC,int nLow,int nHigh);
	void ReCompute(double datathr,double wcorrect);
	void RimLocate(int nValue);
	void SetParam();
	void SetGaussianData(unsigned char *pIn,POINT cpt,POINT bcpt,int maxv,int low, int high);
	double NormGaussian(double s,double v,double m);
	void   SortValue(double *pArr, int d, int h, BOOL bAscending);
	double LDGaussian(double s,double v,double m) ;
	void   ComputeData(double *nData,int count,int flag,double wcc=0.0);

	void DrawLevel(CDC *pDC,int v);
	void MakeColor();

	void PaintEx();

	inline double CVProjection::GetDistanceToPixel(double nD) { return (int)(nD/m_nScaleFactor+0.5); }
	inline double CVProjection::Gaussian(double nMaxV,double nW,double nU){	return (nMaxV*exp(-2.0*nU*nU/(nW*nW/4)));}
	inline int CVProjection::GetCenter() { return m_nDCy; }
	inline int CVProjection::GetMaxIntensity() { return m_nMaxIntensity; }

// Dialog Data
	//{{AFX_DATA(CVProjection)
	enum { IDD = IDD_DIALOG_VPROJ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVProjection)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVProjection)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VPROJECTION_H__279C5FF5_8AE0_4D3C_A6D3_BB577E88AB17__INCLUDED_)
