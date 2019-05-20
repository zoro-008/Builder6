//---------------------------------------------------------------------------

#ifndef FormVisnMainH
#define FormVisnMainH
//---------------------------------------------------------------------------
//#include <Classes.hpp>
//#include <Controls.hpp>
//#include <StdCtrls.hpp>
//#include <Forms.hpp>
//#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
#include "FormImage.h"
#include "FormJob.h"
//#include "VisnDefine.h"
#include "Vision.h"
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
    TButton *btSetup;
        TTimer *tmYame;
        TLabel *Label1;
    void __fastcall btSetupClick(TObject *Sender);
        void __fastcall btJobFileClick(TObject *Sender);
        void __fastcall pcVisnBaseChange(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall tmYameTimer(TObject *Sender);
private:	// User declarations
    void UpdateVisnBaseStat(int _iPageIndex); //밖에서 건들이기 있긔없긔
public:		// User declarations
    __fastcall TFrmVisnMain(TComponent* Owner);

    void Init();

    TWinControl * OriParent ;

    TFrmImage * FrmImg [MAX_VISN_ID] ;
    TTabSheet * tsVisn [MAX_VISN_ID] ;

    void SetLevel(int _iLevel);

    int m_iSendIndex ; 

    //Message Process.
    	void __fastcall WmUser(TMessage &Message);

        //Message Handler.
        BEGIN_MESSAGE_MAP
            MESSAGE_HANDLER(WM_USER, TMessage, WmUser)
        END_MESSAGE_MAP(TForm)







};
//---------------------------------------------------------------------------
extern PACKAGE TFrmVisnMain *FrmVisnMain;
//---------------------------------------------------------------------------
#endif
