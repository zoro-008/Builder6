// OpenGl3D.cpp : implementation file
//

#include "stdafx.h"
#include "hvsystem.h"
#include "OpenGl3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGl3D dialog
#pragma comment(lib,"C:\\OGLSDK\\lib\\glu.lib")
#pragma comment(lib,"c:\\OGLSDK\\lib\\opengl.lib")

inline void MinMax (float d, float& Min, float& Max)
{
	if (d > Max)
		Max = d;
	else if (d < Min)
		Min = d;
}

COpenGl3D::COpenGl3D(CWnd* pParent /*=NULL*/)
	: CDialog(COpenGl3D::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenGl3D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSrc=NULL;
	m_pOrigin=NULL;
    m_nWidth=640;
    m_nHeight=480;
	m_pSrc=new unsigned char [m_nWidth*m_nHeight];
	memset(m_pSrc,0x00,m_nWidth*m_nHeight);
	m_pOrigin= new unsigned char [m_nWidth*m_nHeight];
	memset(m_pOrigin,0x00,m_nWidth*m_nHeight);
	m_pGLImg=new unsigned char [m_nWidth*m_nHeight/4];
	memset(m_pGLImg,0x00,m_nWidth*m_nHeight/4);

	OpenGlInit();
}


void COpenGl3D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenGl3D)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenGl3D, CDialog)
	//{{AFX_MSG_MAP(COpenGl3D)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpenGl3D message handlers
void COpenGl3D::OpenGlInit()
{
	//====== Rendering context does not exist yet
	m_hRC = 0;
	m_zTransZoom=1.5f;
	//====== Initial image turn
	m_AngleX = 60.f;
	m_AngleY =0.f;

	//====== Projection matrix view angle
	m_AngleView = 45.f;

	//====== Initial bkcolor
	m_BkClr = RGB(0, 0, 0);

	// Initial mode to fill the inner polygons (quads) points
	m_FillMode = GL_FILL;

	//====== Initial plot creation
	DefaultGraphic();

	//====== Initial image shift
	//====== One and a half object size (backward)
	m_zTrans = -1.f*m_fRangeX;

	m_xTrans = m_yTrans = 0.f;

	//== Initial shift quantums (for rotation animation)
	m_dx = m_dy = 0.f;

	//====== Mouse is not captured
	m_bCaptured = false;
	//====== Right mouse button has not been pressed
	m_bRightButton = false;
	//====== We use quads to create the surface
	m_bQuad = false;

	//====== Initial lighting params
	m_LightParam[0] = 50;	// X position
	m_LightParam[1] = 80;	// Y position
	m_LightParam[2] = 100;	// Z position
	m_LightParam[3] = 50;	// Ambient light
	m_LightParam[4] = 100;	// Diffuse light
	m_LightParam[5] = 100;	// Specular light
	m_LightParam[6] = 60;	// Ambient material
	m_LightParam[7] = 50;	// Diffuse material
	m_LightParam[8] = 80;	// Specular material
	m_LightParam[9] = 10;	// Shininess material
	m_LightParam[10] = 10;	// Emission material
	m_nRedrawFlag=0;

	return;
}
int COpenGl3D::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
/*	SetGraphicColor();

	PIXELFORMATDESCRIPTOR pfd;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
						PFD_SUPPORT_OPENGL |
						PFD_DOUBLEBUFFER |
						PFD_STEREO_DONTCARE;

	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cRedShift = 16;
	pfd.cGreenBits = 8;
	pfd.cGreenShift = 8;
	pfd.cBlueBits = 8;
	pfd.cBlueShift = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAccumBits = 64;
	pfd.cAccumRedBits = 16;
	pfd.cAccumGreenBits = 16;
	pfd.cAccumBlueBits = 16;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.cAuxBuffers = 1;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;


	//====== Get current Windows context
	m_hdc = ::GetDC(GetSafeHwnd());

	//====== Ask to find the nearest compatible pixel-format
	int iD = ChoosePixelFormat(m_hdc, &pfd);
	if ( !iD )
	{
		MessageBox("ChoosePixelFormat::Error");
		return -1;
	}

	//====== Try to set this format
	if ( !SetPixelFormat (m_hdc, iD, &pfd) )
	{
		MessageBox("SetPixelFormat::Error");
		return -1;
	}

	//====== Try to create the OpenGL rendering context
	if ( !(m_hRC = wglCreateContext (m_hdc)))
	{
		MessageBox("wglCreateContext::Error");
		return -1;
	}

	//====== Try to put it in action
	if ( !wglMakeCurrent (m_hdc, m_hRC))
	{
		MessageBox("wglMakeCurrent::Error");
		return -1;
	}

	//====== Now you can issue OpenGL commands (i.e. call gl*** functions)
	glEnable(GL_LIGHTING);		// Lighting will be used
	//====== Only one (first) source of light
	glEnable(GL_LIGHT0);
	//====== The depth of the Z-buffer will be taken into account
	glEnable(GL_DEPTH_TEST);
	//====== Material colors will be taken into account
//	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    //glEnable(GL_CULL_FACE); // culling face.....
	//glCullFace(GL_BACK);
//	glPolygonMode(GL_FRONT,GL_FILL);
//	glPolygonMode(GL_BACK,GL_FILL);
	// Gouraud
	glShadeModel(GL_SMOOTH);	

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LOGIC_OP);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_2D);

	//====== Our function to set the background
	SetBkColor();


	OpenGlInit();

	//====== Create and store the image in a special list of OpenGL commands
	DrawScene();
*/	
	// TODO: Add your specialized creation code here
	
	return 0;
}
BOOL COpenGl3D::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetGraphicColor();

	PIXELFORMATDESCRIPTOR pfd;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
						PFD_SUPPORT_OPENGL |
						PFD_DOUBLEBUFFER |
						PFD_STEREO_DONTCARE;

	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cRedShift = 16;
	pfd.cGreenBits = 8;
	pfd.cGreenShift = 8;
	pfd.cBlueBits = 8;
	pfd.cBlueShift = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAccumBits = 64;
	pfd.cAccumRedBits = 16;
	pfd.cAccumGreenBits = 16;
	pfd.cAccumBlueBits = 16;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.cAuxBuffers = 1;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;


	//====== Get current Windows context
	m_hdc = ::GetDC(GetSafeHwnd());

	//====== Ask to find the nearest compatible pixel-format
	int iD = ChoosePixelFormat(m_hdc, &pfd);
	if ( !iD )
	{
		MessageBox("ChoosePixelFormat::Error");
		return -1;
	}

	//====== Try to set this format
	if ( !SetPixelFormat (m_hdc, iD, &pfd) )
	{
		MessageBox("SetPixelFormat::Error");
		return -1;
	}

	//====== Try to create the OpenGL rendering context
	if ( !(m_hRC = wglCreateContext (m_hdc)))
	{
		MessageBox("wglCreateContext::Error");
		return -1;
	}

	//====== Try to put it in action
	if ( !wglMakeCurrent (m_hdc, m_hRC))
	{
		MessageBox("wglMakeCurrent::Error");
		return -1;
	}

	//====== Now you can issue OpenGL commands (i.e. call gl*** functions)
	glEnable(GL_LIGHTING);		// Lighting will be used
	//====== Only one (first) source of light
	glEnable(GL_LIGHT0);
	//====== The depth of the Z-buffer will be taken into account
	glEnable(GL_DEPTH_TEST);
	//====== Material colors will be taken into account
//	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    //glEnable(GL_CULL_FACE); // culling face.....
	//glCullFace(GL_BACK);
//	glPolygonMode(GL_FRONT,GL_FILL);
//	glPolygonMode(GL_BACK,GL_FILL);
	// Gouraud
	glShadeModel(GL_SMOOTH);	

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LOGIC_OP);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_2D);

	//====== Our function to set the background
	SetBkColor();


	OpenGlInit();

	//====== Create and store the image in a special list of OpenGL commands
	DrawScene();
	
	// TODO: Add your specialized creation code here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void COpenGl3D::OnDestroy() 
{
	CDialog::OnDestroy();
	if(m_pSrc!=NULL)    delete[] m_pSrc;
	if(m_pOrigin!=NULL) delete[] m_pOrigin;
	if(m_pGLImg!=NULL) delete[] m_pGLImg;
	
}

void COpenGl3D::SetBuffer(unsigned char *pImg)
{

	int sum=0;
	int l=0;
	int i,j;


	memcpy(m_pSrc,pImg,m_nWidth*m_nHeight);
//	memcpy(m_pGLImg,pImg,m_nWidth*m_nHeight);
	
	for(j=0; j<m_nHeight; j+=2)
		for(i=0; i<m_nWidth; i+=2)
		{
			m_pGLImg[l]=*(pImg+m_nWidth*j+i);
			l++;
		}


	DefaultGraphic();
	m_nRedrawFlag=0;
	Invalidate(FALSE);
}

void COpenGl3D::SetGraphicColor()
{
	m_cColor.resize(256);
	int i;
	int rr,gg,bb;
	for(i=0; i<256; i++)
	{
		bb=-(i-255);
		if(i<128) gg=2*i ;
		else      gg=-2*(i-255);
		rr=i;


		m_cColor[i]=CPoint3D((float)rr/255.0f,(float)gg/255.0f,(float)bb/255.0f);
	}
}

void COpenGl3D::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//========== Clean the modeling matrix (make it equal the unity matrix)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//======= At first set the light (otherwise it will rotate with the image)
	SetLight();

	//====== Change the modeling matrix coefficients in order
	glTranslatef(m_xTrans,m_yTrans,m_zTransZoom*m_zTrans);	// to shift

	glRotatef (m_AngleX, 1.0f, 0.0f, 0.0f );	// and to rotate
	glRotatef (m_AngleY, 0.0f, 1.0f, 0.0f );

	//====== the following vertices coordinates (they are being multiplied by matrix)
	DrawScene();
	glCallList(1);

	//====== Switch back and front buffers (to show what happened)
	SwapBuffers(m_hdc);	
	// Do not call CDialog::OnPaint() for painting messages
}


BOOL COpenGl3D::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN) {
		switch(pMsg->wParam) {
			case VK_RETURN:
			case VK_ESCAPE: return true;
			default: 
				 break; 
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}


void COpenGl3D::SetBkColor()
{
	//====== Split the color to 3 components
	GLclampf red	= GetRValue(m_BkClr)/255.f,
			 green	= GetGValue(m_BkClr)/255.f,
			 blue	= GetBValue(m_BkClr)/255.f;
	//====== Set the clear (background) color
	glClearColor (red, green, blue, 0.f);

	//====== Actual background erasure
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void COpenGl3D::DrawScene()
{
	//====== Create the new list of OpenGL commands
	if(m_nRedrawFlag==0)
	{
		glNewList(1, GL_COMPILE);

		//====== Set the polygon filling mode
		glPolygonMode(GL_FRONT_AND_BACK, m_FillMode);
		
		//====== Image grid sizes
		UINT	nx = m_xSize-1,
			nz = m_zSize-1;

		float colorr,colorg,colorb;
		int gvalue;
		//====== Turn on the primitive connection mode (not connected)
		if (m_bQuad)
			glBegin (GL_QUADS);

		for (UINT z=0, i=0;  z < nz;  z++, i++)
		{
			//====== Turn on the primitive connection mode (connected)
			//====== The strip of connected quads begins here
			if (!m_bQuad)
				glBegin(GL_QUAD_STRIP);

			for (UINT x=0;  x < nx;  x++, i++)
			{
				// i, j, k, n — 4 indices of a quad
				// Counter Clockwise direction

				int	j = i + m_xSize,	// Other vertices indices
					k = j+1,
					n = i+1;

				//=== Get coordinates of 4 vertices
				float
					xi = m_cPoints[i].x,
					yi = m_cPoints[i].y,
					zi = m_cPoints[i].z,

					xj = m_cPoints[j].x,
					yj = m_cPoints[j].y,
					zj = m_cPoints[j].z,

					xk = m_cPoints[k].x,
					yk = m_cPoints[k].y,
					zk = m_cPoints[k].z,

					xn = m_cPoints[n].x,
					yn = m_cPoints[n].y,
					zn = m_cPoints[n].z,

					//=== Quad side lines vectors coordinates
					ax = xi-xn,
					ay = yi-yn,

					by = yj-yi,
					bz = zj-zi,

					//====== Normal vector coordinates
					vx = ay*bz,
					vy = -bz*ax,
					vz = ax*by,

					//====== Normal vector length
					v  = float(sqrt(vx*vx + vy*vy + vz*vz));
					
				//====== Scale to unity
				vx /= v;
				vy /= v;
				vz /= v;

				//====== Set the normal vector
				glNormal3f (vx,vy,vz);
				gvalue=m_pGLImg[m_xSize*z+x];
				colorr=m_cColor[gvalue].x;
				colorg=m_cColor[gvalue].z;
				colorb=m_cColor[gvalue].y;

				//===== Not connected quads branch
				if (m_bQuad)
				{
					//====== Vertices are given in counter clockwise direction order
					glColor3f (colorr,colorg,colorb);
					glVertex3f (xi, yi, zi);
					glColor3f (colorr,colorg,colorb);
					glVertex3f (xj, yj, zj);
					glColor3f (colorr,colorg,colorb);
					glVertex3f (xk, yk, zk);
					glColor3f (colorr,colorg,colorb);
					glVertex3f (xn, yn, zn);
				}
				else
				//===== Connected quads branch
				{


					glColor3f (colorr,colorg,colorb);
					glVertex3f (xi, yi, zi);
					glColor3f (colorr,colorg,colorb);
					glVertex3f (xj, yj, zj);

				}
			
			}
			//====== Close block of GL_QUAD_STRIP commands
			if (!m_bQuad)
				glEnd();
		}
			//====== Close block of GL_QUADS commands
		if (m_bQuad)
			glEnd();

		//====== Close the list of OpenGL commands
		glEndList();
		m_nRedrawFlag=1;
	}

}	



void COpenGl3D::LimitAngles()
{
	while (m_AngleX < -360.f)
		m_AngleX += 360.f;
	while (m_AngleX > 360.f)
		m_AngleX -= 360.f;
	while (m_AngleY < -360.f)
		m_AngleY += 360.f;
	while (m_AngleY > 360.f)
		m_AngleY -= 360.f;
}

void COpenGl3D::OnLButtonDown(UINT nFlags, CPoint point)
{
	//====== Stop rotation
	KillTimer(1);

	//====== Zero the quantums
	m_dx = 0.f;
	m_dy = 0.f;

	//====== Capture the mouse messages and direct them in our window
	SetCapture();
	//====== Remember the fact
	m_bCaptured = true;
	//====== and where it happened
	m_pt = point;
}


void COpenGl3D::OnRButtonDown(UINT nFlags, CPoint point)
{
	//====== Remember the fact
	m_bRightButton = true;

	//====== and reproduce the left button response
	OnLButtonDown(nFlags, point);
}

void COpenGl3D::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bCaptured)
	{
		// Desired rotation speed components
		m_dy = float(point.y - m_pt.y)/40.f;
		m_dx = float(point.x - m_pt.x)/40.f;

		//====== If Ctrl was pressed
		if (nFlags & MK_CONTROL)
		{
			//=== we shift (translate) the image
			m_xTrans += m_dx;
			m_yTrans -= m_dy;
		}
		else
		{
			//====== If the right mouse button is pressed
			if (m_bRightButton)
				//====== we shift along the z-axis
				m_zTrans += (m_dx + m_dy)/2.f;
			else
			{
				//====== otherwise we rotate the image
				LimitAngles();
				double a = fabs(m_AngleX);
				if (90. < a && a < 270.)
					m_dx = -m_dx;
				m_AngleX += m_dy;
				m_AngleY += m_dx;
			}
		}
		//=== In any case we should store the coordinates
		m_pt = point;
		Invalidate(FALSE);
	}
}

void COpenGl3D::OnLButtonUp(UINT nFlags, CPoint point)
{
	//====== If we captured the mouse,
	if (m_bCaptured)
	{
		//=== query the desired quantum value
		//=== if it exeeds the sensativity threshold
		if (fabs(m_dx) > 0.5f || fabs(m_dy) > 0.5f)
			//=== Turn on the constant rotation
			SetTimer(1,10,0);
		else
			//=== Turn off the constant rotation
			KillTimer(1);

		//====== Clear the capture flag
		m_bCaptured = false;
		ReleaseCapture();
	}
}


void COpenGl3D::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_bRightButton = m_bCaptured = false;
	ReleaseCapture();
}

void COpenGl3D::OnTimer(UINT nIDEvent)
{
	LimitAngles();
//	//====== Increase the angles
	m_AngleX += m_dy;
	m_AngleY += m_dx;
	Invalidate(FALSE);
}

void COpenGl3D::DefaultGraphic()
{
	//====== Coordinate grid dimensions
	m_xSize=m_nWidth/2;
	m_zSize=m_nHeight/2;
	//====== Number of meshes is less than number of nodes
	UINT	nz = m_zSize - 1,
			nx = m_xSize - 1;

	// File size in bytes
	DWORD nSize = m_xSize * m_zSize * sizeof(float)	+ 2*sizeof(UINT);

	//====== Temporary buffer
	BYTE *buff = new BYTE[nSize+1];

	//====== Point at the start of it with UINT-type pointer
	UINT *p = (UINT*)buff;

	//====== Place the two UINTs
	*p++ = m_xSize;
	*p++ = m_zSize;

	//====== Change the pointer type to continue with the floating numbers
	float *pf = (float*)p;

	//====== For all the grid nodes calculate
	int i,j;


	//=== calculate and place default function values in the same buffer
	for (i=0;  i < m_zSize;  i++)
	{
		for (j=0;  j < m_xSize;  j++)
		{
			//====== Sample function
			*pf++ = *(m_pGLImg+m_xSize*i+j);
		}
	}


	SetGraphPoints (buff, nSize);

	//====== Free temporary buffer
	delete [] buff;
}



void COpenGl3D::SetGraphPoints(BYTE* buff, DWORD nSize)
{
	UINT *p = (UINT*)buff;

	m_xSize = *p;
	m_zSize = *++p;

	if (m_xSize<2 || m_zSize<2 || m_xSize*m_zSize*sizeof(float) + 2 * sizeof(UINT) != nSize)
	{
		MessageBox(_T("Wrong data format"));
		return;
	}

	m_cPoints.resize(m_xSize*m_zSize);

	if (m_cPoints.empty())
	{
		MessageBox(_T("Can not allocate the data"));
		return;
	}

	float	x, z,
			*pf   = (float*)++p,
			fMinY = *pf,
			fMaxY = *pf,
			right = (m_xSize-1)/2.f,
			left  = -right,
			rear  = (m_zSize-1)/2.f,
			front = -rear, 
			range = (right + rear)/2.f;

	UINT	i, j, n;

	m_fRangeY = range;
	m_fRangeX = float(m_xSize);
	m_fRangeZ = float(m_zSize);

	m_zTrans = -1.f * m_fRangeZ;

	for (z=front, i=0, n=0;  i<m_zSize;  i++, z+=1.f)
	{
		for (x=left, j=0;  j<m_xSize;  j++, x+=1.f, n++)
		{
			MinMax (*pf, fMinY, fMaxY);
			m_cPoints[n] = CPoint3D(x,z,*pf++);
		}
	}

	float zoom = fMaxY > fMinY ? range/(fMaxY-fMinY) : 1.f;

	for (n=0;  n<m_xSize*m_zSize;  n++)
	{
		m_cPoints[n].y = zoom * (m_cPoints[n].y - fMinY) - range/2.f;
	}
}


void COpenGl3D::SetLight()
{
	//====== Both surface sides are considered when calculating
	//====== each pixel color with the lighting formula

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);

	//====== Light source position depends on the object sizes scaled to (0,100)

	float fPos[] =
	{
		(m_LightParam[0]-50)*m_fRangeX/100,
		(m_LightParam[1]-50)*m_fRangeY/100,
		(m_LightParam[2]-50)*m_fRangeZ/100,
		1.f
	};
	glLightfv(GL_LIGHT0, GL_POSITION, fPos);

	//====== Ambient light intensity
	float f = m_LightParam[3]/100.f;
	float fAmbient[4] = { f, f, f, 0.f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbient);

	//====== Diffuse light intensity
	f = m_LightParam[4]/100.f;	
	float fDiffuse[4] = { f, f, f, 0.f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffuse);

	//====== Specular light intensity
	f = m_LightParam[5]/100.f;
	float fSpecular[4] = { f, f, f, 0.f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecular);

	//====== Surface material reflection properties for each light component
	f = m_LightParam[6]/100.f;
	float fAmbMat[4] = { f, f, f, 0.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fAmbMat);

	f = m_LightParam[7]/100.f;
	float fDifMat[4] = { f, f, f, 1.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fDifMat);

	f = m_LightParam[8]/100.f;
	float fSpecMat[4] = { f, f, f, 0.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fSpecMat);

	//====== Material shininess
	float fShine = 128 * m_LightParam[9]/100.f;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, fShine);

	//====== Material light emission property
	f = m_LightParam[10]/100.f;
	float fEmission[4] = { f, f, f, 0.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, fEmission);
}




BOOL COpenGl3D::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CDialog::PreCreateWindow(cs);
}

void COpenGl3D::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	double dAspect = cx<=cy ? double(cy)/cx : double(cx)/cy;
	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (m_AngleView, dAspect, 1.0, 10000.);
	glViewport(0, 0, cx, cy);	
	// TODO: Add your message handler code here
	Invalidate(FALSE);
}


void COpenGl3D::ChangeFillMode(int v)
{
	if(v==0)
		m_FillMode = GL_FILL;
	else if(v==1)
		m_FillMode = GL_LINE;
	else
		m_FillMode = GL_POINT;
	m_nRedrawFlag=0;
	Invalidate(FALSE);
	return ;
}

void COpenGl3D::ChangeZTrans(float v)
{
	m_zTransZoom=v;
	Invalidate(FALSE);
	return ;
}

void COpenGl3D::ChangeLightXPos(int v)
{
	m_LightParam[0]=v;
	Invalidate(FALSE);
	return;
}

void COpenGl3D::ChangeLightYPos(int v)
{
	m_LightParam[1]=v;
	Invalidate(FALSE);
	return;
}
void COpenGl3D::ChangeLightZPos(int v)
{
	m_LightParam[2]=v;
	Invalidate(FALSE);
	return;
}

void COpenGl3D::ChangeLightAmbient(int v)
{
	m_LightParam[3]=v;
	Invalidate(FALSE);
	return;
}

void COpenGl3D::ChangeLightDiffuse(int v)
{
	m_LightParam[4]=v;
	Invalidate(FALSE);
	return;
}

void COpenGl3D::ChangeLightSpecular(int v)
{
	m_LightParam[5]=v;
	Invalidate(FALSE);
	return;
}

void COpenGl3D::ChangeLightAmbientMaterial(int v)
{
	m_LightParam[6]=v;
	Invalidate(FALSE);
	return;
}

void COpenGl3D::ChangeLightDiffuseMaterial(int v)
{
	m_LightParam[7]=v;
	Invalidate(FALSE);
	return;
}

void COpenGl3D::ChangeLightSpecularMaterial(int v)
{
	m_LightParam[8]=v;
	Invalidate(FALSE);
	return;
}

void COpenGl3D::ChangeLightShinness(int v)
{
	m_LightParam[9]=v;
	Invalidate(FALSE);
	return;
}

void COpenGl3D::ChangeLightEmission(int v)
{
	m_LightParam[10]=v;
	Invalidate(FALSE);
	return;
}





