//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"

#include "OptionMan.h"
#include "Sequence.h"
#include "PstnMan.h"
#include "DataMan.h"

#include "Loader.h"
#include "PreBuff.h"
#include "Work.h"
#include "PostBuff.h"
#include "Unloader.h"

#include "SPCUnit.h"
#include "LotUnit.h"
#include "ManualMan.h"


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

void __fastcall TFrmLoading::tmLoadTimer(TObject *Sender)   //바인딩 메뉴얼 버튼 만들기.
{
    tmLoad -> Enabled = false ;

    static int ProcessIndex = 0 ;

    switch (ProcessIndex) {
        case 0 : pbProcess->Position = 10; OM  .Init();
                                           lbName    -> Caption  = OM.EqpOptn.sModelName ;
                                           LoadSML("SMDll" );            break ;
        case 1 : pbProcess->Position = 20;                               break ;
        case 2 : pbProcess->Position = 30; DM .Init();                   break ;
        case 3 : pbProcess->Position = 40; PM .Init();                   break ;
        case 4 : pbProcess->Position = 50; MM .Init();                   break ;
        case 5 : pbProcess->Position = 55; PRB.Init();                   break ;
        case 6 : pbProcess->Position = 60; WRK.Init();                   break ;
        case 7 : pbProcess->Position = 65; PSB.Init();                   break ;
        case 8 : pbProcess->Position = 70; LDR.Init();                   break ;
        case 9 : pbProcess->Position = 75; ULD.Init();                   break ;
        case 10: pbProcess->Position = 80; SEQ.Init();                   break ;
        case 11: pbProcess->Position = 90; SPC.Init();                   break ;
        case 12: pbProcess->Position = 99; LTQ.Init();
                 ModalResult = mrOk;                                     break ;
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
        case 0 : pbProcess->Position = 10; OM  .Close();                break ;
        case 1 : pbProcess->Position = 20; /*FreeSVA("pSVApp");*/       break ;
        case 2 : pbProcess->Position = 30; DM .Close();                 break ;
        case 3 : pbProcess->Position = 40; PM .Close();                 break ;
        case 4 : pbProcess->Position = 50; MM .Close();                 break ;
        case 5 : pbProcess->Position = 55; PRB.Close();                 break ;
        case 6 : pbProcess->Position = 60; WRK.Close();                 break ;
        case 7 : pbProcess->Position = 65; PSB.Close();                 break ;
        case 8 : pbProcess->Position = 70; LDR.Close();                 break ;
        case 9 : pbProcess->Position = 75; ULD.Close();                 break ;
        case 10: pbProcess->Position = 80; SEQ.Close();                 break ;
        case 11: pbProcess->Position = 90; SPC.Close();                 break ;
        case 12: pbProcess->Position = 99; LTQ.Close();                 
                 ModalResult = mrOk;                                    break ;
    }
    ProcessIndex++;
    tmSave -> Enabled = true ;
}
//---------------------------------------------------------------------------

