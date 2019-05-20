//---------------------------------------------------------------------------

#ifndef FormVisnMainH
#define FormVisnMainH
//---------------------------------------------------------------------------
#include "VisionBase.h"
#include <Buttons.hpp>
#include <Mask.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
class TFrmVisnMain : public TForm
{
__published:	// IDE-managed Components
    TPageControl *pcVisnBase;
        TButton *btJobFile;
        TTimer *tmUpdate;
        void __fastcall btJobFileClick(TObject *Sender);
        void __fastcall pcVisnBaseChange(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmVisnMain(TComponent* Owner);
    void Init();
    void Del();

    TTabSheet * tsVisn [MAX_VISN_ID] ;


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmVisnMain *FrmVisnMain;
//---------------------------------------------------------------------------
#endif
