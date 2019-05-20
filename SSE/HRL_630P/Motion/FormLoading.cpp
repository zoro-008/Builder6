//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"

#include "OptionMan.h"
#include "ManualMan.h"
#include "PstnMan.h"
#include "DataMan.h"
#include "Sequence.h"
#include "FrontFeed.h"
#include "LearFeed.h"
#include "FrontLaser.h"
#include "LearLaser.h"
#include "SPCUnit.h"
#include "LotUnit.h"
#include "LaserConUnit.h"


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
    if(m_bLoad) tmLoad->Enabled = true ;
    else        tmSave->Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmLoad->Enabled = false ;
    tmSave->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::tmLoadTimer(TObject *Sender)
{
    tmLoad -> Enabled = false ;

    static int ProcessIndex = 0 ;

    switch (ProcessIndex) {
        case 0 : pbProcess->Position =  8;  OM.Init();
                                           lbName    -> Caption  = OM.EqpOptn.sModelName ;
                                           LoadSML("SMDll" );       break ;
        case 1 : pbProcess->Position = 16;  DM.Init();              break ;
        case 2 : pbProcess->Position = 24;  PM.Init();              break ;
        case 3 : pbProcess->Position = 32;  MM.Init();              break ;
        case 4 : pbProcess->Position = 40; FFD.Init();              break ;
        case 5 : pbProcess->Position = 48; FLS.Init();              break ;
        case 6 : pbProcess->Position = 56; LFD.Init();              break ;
        case 7 : pbProcess->Position = 64; LLS.Init();              break ;
        case 8 : pbProcess->Position = 72; SEQ.Init();              break ;
        case 9 : pbProcess->Position = 80; SPC.Init();              break ;
        case 10: pbProcess->Position = 90; LCU.Init();              break ;
        case 11: pbProcess->Position = 99; LTQ.Init();
                 ModalResult = mrOk;                                break ;
    }
    ProcessIndex++;
    tmLoad -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::tmSaveTimer(TObject *Sender)
{
    tmSave -> Enabled = false ;

    static int ProcessIndex = 0 ;

    lbName    -> Caption  = OM.EqpOptn.sModelName ;

    switch (ProcessIndex) {
        case 0:  pbProcess->Position =  8;  OM.Close();             break ;
        case 1 : pbProcess->Position = 16;  DM.Close();             break ;
        case 2 : pbProcess->Position = 24;  PM.Close();             break ;
        case 3 : pbProcess->Position = 32;  MM.Close();             break ;
        case 4 : pbProcess->Position = 40; FFD.Close();             break ;
        case 5 : pbProcess->Position = 48; FLS.Close();             break ;
        case 6 : pbProcess->Position = 56; LFD.Close();             break ;
        case 7 : pbProcess->Position = 64; LLS.Close();             break ;
        case 8 : pbProcess->Position = 72; SEQ.Close();             break ;
        case 9 : pbProcess->Position = 80; SPC.Close();             break ;
        case 10: pbProcess->Position = 90; LCU.Close();             break ;
        case 11: pbProcess->Position = 99; LTQ.Close();
                 //FreeSML();
                 ModalResult = mrOk;                                break ;
    }
    ProcessIndex++;
    tmSave -> Enabled = true ;
}
//---------------------------------------------------------------------------

