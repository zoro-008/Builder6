//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"
//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuffer.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
//Rs232 Part.
//---------------------------------------------------------------------------
#include "ProbeComUnit.h"
//---------------------------------------------------------------------------
//#include "HexaPot.h"
#include "Sequence.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
//#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "UserINI.h"
#include "LotUnit.h"
#include "TempConUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLoading *FrmLoading;
//---------------------------------------------------------------------------
__fastcall TFrmLoading::TFrmLoading(TComponent* Owner , bool _bLoad)
    : TForm(Owner)
{
    m_bLoad = _bLoad ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::FormShow(TObject *Sender)
{
    SetWindowRgn(Handle, CreateRoundRectRgn(ClientRect.Left, ClientRect.Top, ClientRect.Right + 1, ClientRect.Bottom + 1, 2, 2), FALSE);

    //Display Version.
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");
    lbVersion -> Caption = Date ;
    pbProcess->Position = 0;
    imgLogo -> Visible = true;
    if(m_bLoad) tmLoad->Enabled = true ;
    else        tmSave->Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmLoad->Enabled = false ;
    tmLoad->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::tmLoadTimer(TObject *Sender)
{
    tmLoad -> Enabled = false ;

    static int ProcessIndex = 0 ;

    imgLogo -> Visible = true;

    switch (ProcessIndex) {

        //case 0 : pbProcess->Position = 0  ; GetNormalEnd();                                                                  break ;
        case 0 : pbProcess->Position = 0  ; /*GetNormalEnd();  */                                                            break ;
        case 1 : pbProcess->Position = 5  ; OM   .Init(); lbName    -> Caption  = OM.EqpOptn.sModelName ; LoadSML("SMDll" ); break ;
        case 2 : pbProcess->Position = 10 ; /*LoadSVision("pSVision.dll" );*/                                                break ;
        case 3 : pbProcess->Position = 15 ; DM   .Init(     );                                                               break ;
        case 4 : pbProcess->Position = 20 ; PM   .Init(     );                                                               break ;
        case 5 : pbProcess->Position = 25 ; MM   .Init(     );                                                               break ;
        case 6 : pbProcess->Position = 30 ; OM   .Init(     );                                                               break ;
        case 7 : pbProcess->Position = 35 ; PCM  .Init(     );                                                               break ;
        case 8 : pbProcess->Position = 40 ; LDR  .Init(     );                                                               break ;
        case 9 : pbProcess->Position = 50 ; PRB  .Init(     );                                                               break ;
        case 10: pbProcess->Position = 60 ; WRK  .Init(     );                                                               break ;
        case 11: pbProcess->Position = 70 ; PSB  .Init(     );                                                               break ;
        case 12: pbProcess->Position = 80 ; ULD  .Init(     );                                                               break ;
        case 13: pbProcess->Position = 90 ; SPC  .Init(     );                                                               break ;
        case 14: pbProcess->Position = 90 ; LOT  .Init(     );                                                               break ;
        case 15: pbProcess->Position = 95 ; TCU  .Init(     ); /*PTT.Init();*/                                               break ;
        case 16: pbProcess->Position = 100; SEQ  .Init(     ); ModalResult = mrOk;                                           break ;
    }

    ProcessIndex++;
    tmLoad -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmLoading::tmSaveTimer(TObject *Sender)
{
    tmSave -> Enabled = false ;

    static int ProcessIndex = 0 ;

    switch (ProcessIndex) {
        case 0 : pbProcess->Position = 0  ; OM   .Close();                                         break ;
        case 1 : pbProcess->Position = 5  ; lbName    -> Caption  = OM.EqpOptn.sModelName ;        break ;
        case 2 : pbProcess->Position = 10 ; DM .Close();                                           break ;  //이게 오래걸림.
        case 3 : pbProcess->Position = 15 ; PM .Close();                                           break ;
        case 4 : pbProcess->Position = 20 ; MM .Close();                                           break ;
        case 5 : pbProcess->Position = 25 ; OM .Close();                                           break ;
        case 6 : pbProcess->Position = 30 ; PCM.Close();                                           break ;
        case 7 : pbProcess->Position = 30 ; LDR.Close();                                           break ;
        case 8 : pbProcess->Position = 40 ; PRB.Close();                                           break ;
        case 9 : pbProcess->Position = 50 ; WRK.Close();                                           break ;
        case 10: pbProcess->Position = 60 ; PSB.Close();                                           break ;
        case 11: pbProcess->Position = 70 ; ULD.Close();                                           break ;
        case 12: pbProcess->Position = 80 ; SPC.Close();                                           break ;
        case 13: pbProcess->Position = 90 ; LOT.Close();                                           break ;
        case 14: pbProcess->Position = 95 ; TCU.Close();                                           break ;
        case 15: pbProcess->Position = 95 ; SEQ.Close();                                           break ;
        case 16: pbProcess->Position = 100; ModalResult = mrOk;                                    break ;
    }

    ProcessIndex++;
    tmSave -> Enabled = true ;
}
//---------------------------------------------------------------------------


