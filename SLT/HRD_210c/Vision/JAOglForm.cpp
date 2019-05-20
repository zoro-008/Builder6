//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "JAOglForm.h"
//#include "JAMainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFOpenGL *FOpenGL;
//---------------------------------------------------------------------------
__fastcall TFOpenGL::TFOpenGL(TComponent* Owner)
    : TForm(Owner)
{
    PView->Align = alClient;
    PView->DoubleBuffered = true;
    m_pImage   = NULL ;
    m_pTracker = NULL ;

}

__fastcall TFOpenGL::TFOpenGL(TComponent* Owner , IImage * _pImage , TRectTracker * _pTracker)
    : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;

    PView->Align = alClient;
    PView->DoubleBuffered = true;

    m_pImage   = _pImage   ;
    m_pTracker = _pTracker ;
}

void __fastcall TFOpenGL::OnSetImage(IImage * _pImage)
{
    m_pImage   = _pImage   ;
}

void __fastcall TFOpenGL::OnSetTracker(TRectTracker * _pTracker)
{
    m_pTracker = _pTracker ;
}

//---------------------------------------------------------------------------
void __fastcall TFOpenGL::FormCreate(TObject *Sender)
{
    OriginalProc = PView->WindowProc;
    PView->WindowProc = PanelProc;

    hdc = GetDC(PView->Handle);
    SetupPixelFormat();
    if((hrc = wglCreateContext(hdc))==NULL) ShowMessage("Can't create a GL rendering context"    );
    if(!wglMakeCurrent(hdc, hrc)          ) ShowMessage("Can't activate the GL rendering context");
    InitGL();
}
//---------------------------------------------------------------------------
void __fastcall TFOpenGL::FormDestroy(TObject *Sender)
{
    PView->WindowProc = OriginalProc;

    wglMakeCurrent(hdc, NULL);
    wglDeleteContext(hrc);
}
//---------------------------------------------------------------------------
void __fastcall TFOpenGL::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT) UpdateView();
}
//---------------------------------------------------------------------------
void __fastcall TFOpenGL::UpdateView()
{
    DrawGLScene();
    SwapBuffers(hdc);
}
//---------------------------------------------------------------------------
void __fastcall TFOpenGL::FormResize(TObject *Sender)
{
    ReSize(PView->Width, PView->Height);
}
//---------------------------------------------------------------------------
GLvoid __fastcall TFOpenGL::ReSize(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);                                     // Reset the current viewport
    glMatrixMode(GL_PROJECTION);                                // Select the projection matrix
    glLoadIdentity();                                           // Reset the projection matrix
    gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);                                 // Select the modelview matrix
    glLoadIdentity();                                           // Reset the modelview matrix
}
//---------------------------------------------------------------------------
void __fastcall TFOpenGL::SetupPixelFormat()
{
    static PIXELFORMATDESCRIPTOR pfd =             // pfd tells windows how we want things to be
	{
		sizeof(PIXELFORMATDESCRIPTOR),             // Size of this pixel format descriptor
		1,					                       // Version number
		PFD_DRAW_TO_WINDOW |		               // Format must support window
		PFD_SUPPORT_OPENGL |		               // Format must support OpenGL
		PFD_DOUBLEBUFFER,			               // Must support double buffering
		PFD_TYPE_RGBA,				               // Request an RGBA format
		16,					                       // Select our color depth
		0, 0, 0, 0, 0, 0,	                       // Color bits ignored
		0,					                       // No alpha buffer
		0,					                       // Shift bit ignored
		0,					                       // No accumulation buffer
		0, 0, 0, 0,			                       // Accumulation bits ignored
		16,					                       // 16Bit Z-Buffer (Depth buffer)
		0,					                       // No stencil buffer
		0,					                       // No auxiliary buffer
		PFD_MAIN_PLANE,		                       // Main drawing layer
		0,					                       // Reserved
		0, 0, 0					                   // Layer masks ignored
	};

    if ((PixelFormat = ChoosePixelFormat(hdc, &pfd))==NULL) ShowMessage("Can't find a suitable pixelformat");
    if (!SetPixelFormat(hdc, PixelFormat, &pfd)           ) ShowMessage("Can't set the pixelformat"        );
}
//---------------------------------------------------------------------------
int __fastcall TFOpenGL::InitGL()
{
	glShadeModel(GL_SMOOTH);                                // Enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   // Black background
	glClearDepth(1.0f);                                     // Depth buffer setup
	glEnable(GL_DEPTH_TEST);                                // Enables depth testing
	glDepthFunc(GL_LEQUAL);                                 // The type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Really nice perspective calculations
	return 1;                                               // Initialization went OK
}
//---------------------------------------------------------------------------
int __fastcall TFOpenGL::DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer
    glLoadIdentity();                                   // Reset the current modelview matrix

    glTranslatef(0.0f, 0.0f, -60.0f);
    glRotatef(RotX, 1.0f, 0.0f, 0.0f);
    glRotatef(RotY, 0.0f, 1.0f, 0.0f);

    glBegin(GL_POINTS);
    int dx = rect.left;
    int dy = rect.top;
    int dw = rect.right;
    int dh = rect.bottom;

    if(m_pImage == NULL) return 0 ;

    for (register int h=dy; h<dh; h++) {
        for (register int w=dx; w<dw; w++) {
            GLfloat nz = m_pImage -> GetPixel(w,h)*0.1 ;
            GLfloat nw = (w-dx-(dw-dx)/2)*0.1;
            GLfloat nh = (h-dy-(dh-dy)/2)*0.1;

            GLfloat Red, Green, Blue;

            if (nz>= 0.0 && nz<= 5.1) {Red = 1.0             ; Green = nz*0.5           ; Blue = 0.0          ;}
            if (nz>= 5.2 && nz<=10.2) {Red = 1.0-(nz-5.2)*0.5; Green = 1.0              ; Blue = 0.0          ;}
            if (nz>=10.3 && nz<=15.3) {Red = 0.0             ; Green = 1.0              ; Blue = (nz-10.3)*0.5;}
            if (nz>=15.4 && nz<=20.3) {Red = 0.0             ; Green = 1.0-(nz-15.4)*0.5; Blue = 1.0          ;}
            if (nz>=20.4 && nz<=25.4) {Red = (nz-20.4)*0.5   ; Green = 0.0              ; Blue = 1.0          ;}
            if (nz>=25.5            ) {Red = 0.0             ; Green = 0.0              ; Blue = 0.0          ;}

            glColor3f(Red, Green, Blue);

            glVertex3f(nw, -nh, nz);
        }
    }
    glEnd();

    return 1;
}

//---------------------------------------------------------------------------

void __fastcall TFOpenGL::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Key == VK_UP   ) RotX-=1.0f;
    if(Key == VK_DOWN ) RotX+=1.0f;
    if(Key == VK_LEFT ) RotY-=1.0f;
    if(Key == VK_RIGHT) RotY+=1.0f;

    UpdateView();
}
//---------------------------------------------------------------------------
void __fastcall TFOpenGL::PViewMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button==mbLeft) {
        IsDrag = true;
        mp.x = abs(RotY-X);
        mp.y = abs(RotX-Y);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFOpenGL::PViewMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if (Shift.Contains(ssLeft) && IsDrag) {
        RotY = X-mp.x;
        RotX = Y-mp.y;
        UpdateView();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFOpenGL::PViewMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button==mbLeft && IsDrag) IsDrag = false;
}
//---------------------------------------------------------------------------


