#if !defined(AFX_OPENGL3D_H__FE27B72C_9F21_4EE0_93F4_9A22FC0FC918__INCLUDED_)
#define AFX_OPENGL3D_H__FE27B72C_9F21_4EE0_93F4_9A22FC0FC918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGl3D.h : header file
//
//#include <vector>
/////////////////////////////////////////////////////////////////////////////
// COpenGl3D dialog
class CPoint3D
{
public:
	float x, y, z;
	CPoint3D () { x=y=z=0; }
	CPoint3D (float c1, float c2, float c3)
	{
		x = c1;		z = c2;		y = c3;
	}
	CPoint3D& operator=(const CPoint3D& pt)
	{
		x = pt.x;	z = pt.z;	y = pt.y;
		return *this;
	}
	CPoint3D (const CPoint3D& pt)
	{
		*this = pt;
	}
};

class COpenGl3D : public CDialog
{
// Construction
public:
	COpenGl3D(CWnd* pParent = NULL);   // standard constructor
    CRect m_wndRect;
	int m_nWidth,m_nHeight;
    unsigned char *m_pSrc;
	unsigned char *m_pOrigin;
	unsigned char *m_pGLImg;

	long	m_BkClr;	// Background color
	HGLRC	m_hRC;		// Rendering context OpenGL
	HDC	m_hdc;		// Windows device context
	GLfloat	m_AngleX;	// Rotation angle (around X-axis)
	GLfloat m_AngleY;	// Rotation angle (around Y-axis)
	GLfloat	m_AngleView;	// Perspective angle
	GLfloat	m_fRangeX;	// Graphics dimension (along X-axis)
	GLfloat m_fRangeY;	// Graphics dimension (along Y-axis)
	GLfloat m_fRangeZ;	// Graphics dimension (along Z-axis)
	GLfloat	m_dx;		// Displacement quantum (along X-axis)
	GLfloat m_dy;		// Displacement quantum (along Y-axis)
	GLfloat	m_xTrans;	// Displacement (along X-axis)
	GLfloat m_yTrans;	// Displacement (along Y-axis)
	GLfloat m_zTrans;	// Displacement (along Z-axis)
	GLenum	m_FillMode;	// Polygon filling mode
	GLfloat m_zTransZoom;
	bool	m_bCaptured;	// Mouse capture flag
	bool	m_bRightButton;	// Right mouse button flag
	bool	m_bQuad;	// Flag of using GL_QUAD (instead of GL_QUAD_STRIP)
	CPoint	m_pt;		// Current mouse position
	UINT	m_xSize;	// Current client window sixe (along X-axis)
	UINT	m_zSize;	// Current client window sixe (along -axis)
	vector < CPoint3D > m_cPoints;	// Graphics dimension (along X-axis)
	vector <CPoint3D> m_cColor;
	int	m_LightParam[11];	// Graphics dimension (along X-axis)
	int m_nRedrawFlag;

	//======== Public methods
	void SetBuffer(unsigned char *pImg);
	void OpenGlInit();
	//======== New methods
	void DrawScene();	// Prepare and store the image
	void DefaultGraphic();	// Create and save the default plot
	//===== Take the data from buffer and store in m_cPoints
	void SetGraphPoints(BYTE* buff, DWORD nSize);
	void SetLightParam (short lp, int nPos);	// Set lighting parameters
	void GetLightParams(int *pPos);			// Get lighting parameters
	void SetLight();				// Set the lighting
	void SetBkColor();				// Set background color
	void LimitAngles();				// Limit rotation angles
	void SetGraphicColor();
/////////////////////
	void ChangeFillMode(int v);
	void ChangeZTrans(float v);
	void ChangeLightXPos(int v);
	void ChangeLightYPos(int v);
	void ChangeLightZPos(int v);
	void ChangeLightAmbient(int v);
	void ChangeLightDiffuse(int v);
	void ChangeLightSpecular(int v);
	void ChangeLightAmbientMaterial(int v);
	void ChangeLightDiffuseMaterial(int v);
	void ChangeLightSpecularMaterial(int v);
	void ChangeLightShinness(int v);
	void ChangeLightEmission(int v);
/////////////////////////////////////////////////
	inline void COpenGl3D::SetOrgRect(CRect r)       { m_wndRect=r;                                }
	inline void COpenGl3D::SetBufSize(int nW,int nH) { m_nWidth=nW; m_nHeight=nH;                  }
	inline void COpenGl3D::SetFillMode(int v)        { m_FillMode =v;                              }
	inline void COpenGl3D::SetZoom(float v)          { m_zTransZoom =v;                            }
	inline void COpenGl3D::SetLightParam(int *v)     { memcpy(&m_LightParam[0], v,sizeof(int)*11); }
/////////////////////////////////////////////////
// Dialog Data
	//{{AFX_DATA(COpenGl3D)
	enum { IDD = IDD_DIALOG_3DDISPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGl3D)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenGl3D)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGL3D_H__FE27B72C_9F21_4EE0_93F4_9A22FC0FC918__INCLUDED_)
