//---------------------------------------------------------------------------

#ifndef Form3dH
#define Form3dH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//#include "JAVisionUnit.h"

//#pragma comment(lib, "opengl32_bc.lib")
//#pragma comment(lib, "GLU32_bc.lib")
//#pragma comment(lib, "glut32_bc.lib")


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

#include "Tracker.h"
#include "SVInterface.h"

//---------------------------------------------------------------------------
class TFrm3d : public TForm
{
__published:	// IDE-managed Components
    TPanel *PView;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall PViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall PViewMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall PViewMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
private:	// User declarations
    GLvoid __fastcall ReSize(GLsizei w, GLsizei h);
    void __fastcall SetupPixelFormat();
    int __fastcall InitGL();              // All setup for OpenGL goes here
    int __fastcall DrawGLScene();         // Here's where we do all the drawing

    double * m_pRefOstX ; //이미지 X오프셑.
    double * m_pRefOstY ; //이미지 Y오프셑.

public:		// User declarations
    TWndMethod OriginalProc;
    void __fastcall PanelProc(Messages::TMessage &msg);

    void __fastcall UpdateView();

    HGLRC m_hRc;               // Permanent rendering context
    HDC   m_hDc;               // Private GDI device context
    int   m_iPixelFormat;

    GLfloat m_gfRotX, m_gfRotY;
    GLfloat m_gfScale ;
    bool IsDrag;
    TPoint mp;
    TRect rect;

    IImage       * m_pImage   ;
    TRectTracker * m_pTracker ;



    __fastcall TFrm3d(TComponent* Owner);
    __fastcall TFrm3d(TComponent* Owner , IImage * _pImage , TRectTracker * _pTracker , double * _pRefOstX , double * _pRefOstY);
    void __fastcall OnSetImage  (IImage * _pImage) ;
    void __fastcall OnSetTracker(TRectTracker * _pTracker) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrm3d *Frm3d;
//---------------------------------------------------------------------------
#endif

