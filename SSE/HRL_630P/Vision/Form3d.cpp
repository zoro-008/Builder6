//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Form3d.h"
//#include "JAMainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrm3d *Frm3d=NULL;
//---------------------------------------------------------------------------
__fastcall TFrm3d::TFrm3d(TComponent* Owner)
    : TForm(Owner)
{
    PView->Align = alClient;
    PView->DoubleBuffered = true;
    m_pImage   = NULL ;
    m_pTracker = NULL ;

}

__fastcall TFrm3d::TFrm3d(TComponent* Owner , IImage * _pImage , TRectTracker * _pTracker , double * _pRefOstX , double * _pRefOstY)
    : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;

    PView->Align = alClient;
    PView->DoubleBuffered = true;

    m_pImage   = _pImage   ;
    m_pTracker = _pTracker ;

    m_pRefOstX = _pRefOstX ;
    m_pRefOstY = _pRefOstY ;
}

void __fastcall TFrm3d::OnSetImage(IImage * _pImage)
{
    m_pImage   = _pImage   ;
}

void __fastcall TFrm3d::OnSetTracker(TRectTracker * _pTracker)
{
    m_pTracker = _pTracker ;
}

//---------------------------------------------------------------------------
void __fastcall TFrm3d::FormCreate(TObject *Sender)
{
    OriginalProc = PView->WindowProc;
    PView->WindowProc = PanelProc;

    m_hDc = GetDC(PView->Handle);
    SetupPixelFormat();
    if((m_hRc = wglCreateContext(m_hDc))==NULL) ShowMessage("Can't create a GL rendering context"    );
    if(!wglMakeCurrent(m_hDc, m_hRc)          ) ShowMessage("Can't activate the GL rendering context");
    InitGL();
}
//---------------------------------------------------------------------------
void __fastcall TFrm3d::FormDestroy(TObject *Sender)
{
    PView->WindowProc = OriginalProc;

    wglMakeCurrent(m_hDc, NULL);
    wglDeleteContext(m_hRc);
}
//---------------------------------------------------------------------------
void __fastcall TFrm3d::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT) UpdateView();
}
//---------------------------------------------------------------------------
void __fastcall TFrm3d::UpdateView()
{
    DrawGLScene();
    SwapBuffers(m_hDc);
}
//---------------------------------------------------------------------------
void __fastcall TFrm3d::FormResize(TObject *Sender)
{
    ReSize(PView->Width, PView->Height);
}
//---------------------------------------------------------------------------
GLvoid __fastcall TFrm3d::ReSize(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);                                     // Reset the current viewport
    glMatrixMode(GL_PROJECTION);                                // Select the projection matrix
    glLoadIdentity();                                           // Reset the projection matrix
    gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);                                 // Select the modelview matrix
    glLoadIdentity();                                           // Reset the modelview matrix
}
//---------------------------------------------------------------------------
void __fastcall TFrm3d::SetupPixelFormat()
{
    static PIXELFORMATDESCRIPTOR pfd =      // pfd tells windows how we want things to be
	{
	    sizeof(PIXELFORMATDESCRIPTOR),  // Size of this pixel format descriptor
	    1,				    // Version number
	    PFD_DRAW_TO_WINDOW |	    // Format must support window
	    PFD_SUPPORT_OPENGL |	    // Format must support OpenGL
	    PFD_DOUBLEBUFFER,		    // Must support double buffering
	    PFD_TYPE_RGBA,		    // Request an RGBA format
	    16,				    // Select our color depth
	    0, 0, 0, 0, 0, 0,	            // Color bits ignored
	    0,				    // No alpha buffer
	    0,				    // Shift bit ignored
	    0,				    // No accumulation buffer
	    0, 0, 0, 0,			    // Accumulation bits ignored
	    16,				    // 16Bit Z-Buffer (Depth buffer)
	    0,				    // No stencil buffer
	    0,				    // No auxiliary buffer
	    PFD_MAIN_PLANE,		    // Main drawing layer
	    0,				    // Reserved
	    0, 0, 0			    // Layer masks ignored
	};

    if ((m_iPixelFormat = ChoosePixelFormat(m_hDc, &pfd))==NULL) ShowMessage("Can't find a suitable pixelformat");
    if (!SetPixelFormat(m_hDc, m_iPixelFormat, &pfd)           ) ShowMessage("Can't set the pixelformat"        );
}
//---------------------------------------------------------------------------
int __fastcall TFrm3d::InitGL()
{
    m_gfScale = 1.0 ;
    m_gfRotX  = 0.0 ;
    m_gfRotY  = 0.0 ;
    glShadeModel(GL_SMOOTH);                                // Enable smooth shading  //GL_FLAT�� �ָ� ���� ���δ�.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   // Black background
    glClearDepth(1.0f);                                     // Depth buffer setup
    glEnable(GL_DEPTH_TEST);                                // �ڿ� ���������� �Ⱥ��̰��� Enables depth testing
    glDepthFunc(GL_LEQUAL);                                 // The type of depth testing to do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Really nice perspective calculations


   /*
 glEnable(GL_LIGHTING);     // ������ Ȱ��ȭ
 glEnable(GL_COLOR_MATERIAL);
 float ambientLight[]  = {-11.0f, -11.0f, 1.0f, 100.0f};
 float diffuseLight[]  = {1.0f, 1.0f, 1.0f, 100.0f};
 float specular[]      = {1.0f, 1.0f, 1.0f, 1.0f};
 float lightPosition[] = {10.0f, 10.0f, 10.0f, 1.0f};
 float spotlightPosition[] = {6.0f, 0.5f, 0.0f, 1.0f};
 float spotlightDirection[] = {10.0f, 10.0f, 10.0f};
 // LIGHT0�� ���� ������ ����
 glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
 glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
 //glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
 //glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);

 // LIGHT0�� ����
 glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);  // �ֺ��� ������ ����
 glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);  // �л걤 ������ ����
 glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); // ������ ��ġ�� ����
 //glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
 // LIGHT0�� �Ҵ�
 glEnable(GL_LIGHT0);
 //glEnable(GL_LIGHT7);


 glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0f);
 glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 30.0f);
 glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotlightDirection);
 */












/*

    //LIGHT
    float ambLight[] = {1.0f, 1.0f  , 1.0f, 0.1f}; //Ambient ���� ������ �ش�.
    float specular[] = {1.0f, 1.0f  , 1.0f, 1.0f}; //Specular ���� ������ �ش�.
    float specref [] = {1.0f, 1.0f  , 1.0f, 1.0f}; //Specular �������� �����Ѵ�.
    float lightPos[] = {1.0f, 1.0f  , 1.0f, 0.1f}; //������ ��ġ�� �����Ѵ�.

    glEnable(GL_LIGHTING); //������ ����ϵ��� �Ѵ�.

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLight); //����� ������ �ֺ����� �����Ѵ�.

    glLightfv(GL_LIGHT0, GL_AMBIENT , ambientLight);  // �ֺ��� ������ ����
    glLightfv(GL_LIGHT0, GL_DIFFUSE , ambLight); //������ �Ӽ� Diffuse����Ambient�� ������ ������ �൵ �ǰ� �ٸ� ���� �൵ �ȴ�.
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular); //Specular ����
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos); //��ġ ����


    glEnable(GL_COLOR_MATERIAL); //���� ���߾ ������ ���� ��Ų��. ������ ������ �޵��� �Ѵ�.
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); //���� ������ ������Ʈ�� �������� �Ѵ�.

    glMaterialfv(GL_FRONT, GL_SPECULAR, specref); //Specular ����
    glMateriali(GL_FRONT, GL_SHININESS, 128);
    glEnable(GL_LIGHT0); //0�� ���� ��� glDisable(GL_LIGHT0);�� Ư�� ���� �� �� �ִ�.

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
*/

    return 1;                                               // Initialization went OK
}
//---------------------------------------------------------------------------
int __fastcall TFrm3d::DrawGLScene()
{
    if(m_pImage   == NULL) return 0 ;
    if(m_pTracker == NULL) return 0 ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer       ȭ�鿡 ���� �ѷ��ֱ� ���ؼ��� �켱 Į����ۿ� ���̹��۾ȿ� �ִ� ��⸦ ������� ����.
    glLoadIdentity();                                   // Reset the current modelview matrix  ��ĸ� �ʱ�ȭ ��Ų�̴ϴ�. ��, ���� ���·� ���ƿ°���.

    glTranslatef(0.0f, 0.0f, -60.0f);    //Z������ 60��ŭ �ڷ� ������ �Ҹ��Դϴ�. ��, ������ ���� ���̴� ȭ�鿡�� -60��ŭ �ڷ� ���°���.
    glRotatef(m_gfRotX, 1.0f, 0.0f, 0.0f);
    glRotatef(m_gfRotY, 0.0f, 1.0f, 0.0f);
    glScalef (m_gfScale , m_gfScale , m_gfScale);

    //glBegin(GL_POINTS);

    //glBegin(GL_POLYGON);
    //glBegin(GL_LINE_STRIP);
    //glBegin(GL_TRIANGLE_STRIP);
    //glBegin(GL_TRIANGLES);


    int iLeft   = m_pTracker -> Left       ;//+ *m_pRefOstX ;
    int iTop    = m_pTracker -> Top        ;//+ *m_pRefOstY ;
    int iRight  = m_pTracker -> GetRight ();//+ *m_pRefOstX ;
    int iBottom = m_pTracker -> GetBottom();//+ *m_pRefOstY ;

    GLfloat gfRed, gfGreen, gfBlue ;
    GLfloat gfZ , gfZL , gfZLT , gfZT ;
    GLfloat gfX , gfXL , gfXLT , gfXT ;
    GLfloat gfY , gfYL , gfYLT , gfYT ;

    int     iPx , iPxL , iPxLT , iPxT ;
    int     iBigPx ;

    for (register int y=iTop+1; y<iBottom; y++) {
        //glBegin(GL_QUADS);
        glBegin(GL_TRIANGLES);
        for (register int x=iLeft+1; x<iRight; x++) {
            iPx   = m_pImage -> GetPixel(x  ,y  ) ;
            iPxL  = m_pImage -> GetPixel(x-1,y  ) ;
            iPxLT = m_pImage -> GetPixel(x-1,y-1) ;
            iPxT  = m_pImage -> GetPixel(x  ,y-1) ;
            iBigPx = 0 ;
            if(iBigPx < iPx  ) iBigPx = iPx   ;
            if(iBigPx < iPxL ) iBigPx = iPxL  ;
            if(iBigPx < iPxLT) iBigPx = iPxLT ;
            if(iBigPx < iPxT ) iBigPx = iPxT  ;


                 if (iBigPx>=  0 && iBigPx<= 51){gfRed = 255               ; gfGreen = iBigPx*5          ; gfBlue = 0             ;}
            else if (iBigPx>= 52 && iBigPx<=102){gfRed = 255-(iBigPx-52)*5 ; gfGreen = 255               ; gfBlue = 0             ;}
            else if (iBigPx>=103 && iBigPx<=153){gfRed = 0                 ; gfGreen = 255               ; gfBlue = (iBigPx-103)*5;}
            else if (iBigPx>=154 && iBigPx<=203){gfRed = 0                 ; gfGreen = 255-(iBigPx-154)*5; gfBlue = 255           ;}
            else if (iBigPx>=204 && iBigPx<=255){gfRed = (iBigPx-204)*5    ; gfGreen = 0                 ; gfBlue = 255           ;}
            else                                {gfRed = 0                 ; gfGreen = 0                 ; gfBlue = 0             ;}

            glColor3f(gfRed/255.0, gfGreen/255.0, gfBlue/255.0);  //3���� Į���Լ��� ��ǥ���� 3���� ���ڸ� �޴� �Լ��� �����ϴ�.
            //glColor3f(iBigPx/255.0, iBigPx/255.0, iBigPx/255.0);  //3���� Į���Լ��� ��ǥ���� 3���� ���ڸ� �޴� �Լ��� �����ϴ�.

            //���� �ȼ�.
            gfX = (x-iLeft-(iRight -iLeft)/2)*0.1;
            gfY = (y-iTop -(iBottom-iTop )/2)*0.1;
            gfZ = iPx *0.1 ;

            //���� �ȼ�
            gfXL = (x-iLeft-1-(iRight -iLeft)/2)*0.1;
            gfYL = (y-iTop   -(iBottom-iTop )/2)*0.1;
            gfZL = iPxL *0.1 ;

            //���� �� �ȼ�.
            gfXLT = (x-iLeft-1-(iRight -iLeft)/2)*0.1;
            gfYLT = (y-iTop -1-(iBottom-iTop )/2)*0.1;
            gfZLT = iPxLT *0.1 ;

            //�� �ȼ�.
            gfXT = (x-iLeft  -(iRight -iLeft)/2)*0.1;
            gfYT = (y-iTop -1-(iBottom-iTop )/2)*0.1;
            gfZT = iPxT *0.1 ;


            glVertex3f(gfXLT, -gfYLT, gfZLT);
            glVertex3f(gfXL , -gfYL , gfZL );
            glVertex3f(gfXT , -gfYT , gfZT );

            glVertex3f(gfX  , -gfY  , gfZ  );
            glVertex3f(gfXL , -gfYL , gfZL );
            glVertex3f(gfXT , -gfYT , gfZT );


        }
        glEnd();
    }





    return 1;
}

//---------------------------------------------------------------------------

void __fastcall TFrm3d::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Key == VK_UP   ) m_gfRotX-=1.0f;
    if(Key == VK_DOWN ) m_gfRotX+=1.0f;
    if(Key == VK_LEFT ) m_gfRotY-=1.0f;
    if(Key == VK_RIGHT) m_gfRotY+=1.0f;

    UpdateView();
}
//---------------------------------------------------------------------------
void __fastcall TFrm3d::PViewMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button==mbLeft) {
        IsDrag = true;
        mp.x = abs(m_gfRotY-X);
        mp.y = abs(m_gfRotX-Y);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrm3d::PViewMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if (Shift.Contains(ssLeft) && IsDrag) {
        m_gfRotY = X-mp.x;
        m_gfRotX = Y-mp.y;
        UpdateView();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrm3d::PViewMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button==mbLeft && IsDrag) IsDrag = false;
}
//---------------------------------------------------------------------------


void __fastcall TFrm3d::FormMouseWheel(TObject *Sender, TShiftState Shift,
      int WheelDelta, TPoint &MousePos, bool &Handled)
{
    m_gfScale += WheelDelta/1000.0 ;
    UpdateView();
}
//---------------------------------------------------------------------------

