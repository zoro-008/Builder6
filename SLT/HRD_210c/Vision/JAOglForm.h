//---------------------------------------------------------------------------

#ifndef JAOglFormH
#define JAOglFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//#include "JAVisionUnit.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

#include "Tracker.h"
#include "SVInterface.h"

//---------------------------------------------------------------------------
class TFOpenGL : public TForm
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
private:	// User declarations
public:		// User declarations
    TWndMethod OriginalProc;
    void __fastcall PanelProc(Messages::TMessage &msg);

    void __fastcall UpdateView();

    HGLRC hrc;               // Permanent rendering context
    HDC   hdc;               // Private GDI device context
    int PixelFormat;

    GLfloat RotX, RotY;
    bool IsDrag;
    TPoint mp;
    TRect rect;

    IImage       * m_pImage   ;
    TRectTracker * m_pTracker ;

    GLvoid __fastcall ReSize(GLsizei w, GLsizei h);
    void __fastcall SetupPixelFormat();
    int __fastcall InitGL();              // All setup for OpenGL goes here
    int __fastcall DrawGLScene();         // Here's where we do all the drawing

    __fastcall TFOpenGL(TComponent* Owner);
    __fastcall TFOpenGL(TComponent* Owner , IImage * _pImage , TRectTracker * _pTracker);
    void __fastcall OnSetImage  (IImage * _pImage) ;
    void __fastcall OnSetTracker(TRectTracker * _pTracker) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFOpenGL *FOpenGL;
//---------------------------------------------------------------------------
#endif

