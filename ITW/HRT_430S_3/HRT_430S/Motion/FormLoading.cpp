//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"

#include "Loader.h"
#include "PreBuff.h"
#include "PostBuff.h"
#include "WorkZone.h"
#include "SortingTool.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
#include "DataMan.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "PstnMan.h"
#include "LanguageUnit.h"
//---------------------------------------------------------------------------
#include "SPCUnit.h"
#include "LotUnit.h"
#include "Rs232Man.h"
#include "ProbeComUnit.h"
#include "Sequence.h"
#include "PstnMan.h"
#include "TempConUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLoading *FrmLoading;
//---------------------------------------------------------------------------
__fastcall TFrmLoading::TFrmLoading(TComponent* Owner)
    : TForm(Owner)
{
    m_bLoad = true;
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
        case 0 : pbProcess->Position = 10; OM  .Init();
                                           lbName    -> Caption  = "HRT_430S";
                                                                         break ;
        case 1 : pbProcess->Position = 20; TCU.Init();                   break ;
        case 2 : pbProcess->Position = 25; DM .Init();                   break ;
        case 3 : pbProcess->Position = 30; PM .Init();                   break ;
        case 4 : pbProcess->Position = 35; MM .Init();                   break ;
        case 5 : pbProcess->Position = 40; LAG.Init();                   break ;
        case 6 : pbProcess->Position = 45; PCM.Init();                   break ;
        case 7 : pbProcess->Position = 50; LDR.Init();                   break ;
        case 8 : pbProcess->Position = 55; PRB.Init();                   break ;
        case 9 : pbProcess->Position = 60; WRK.Init();                   break ;
        case 10: pbProcess->Position = 65; PSB.Init();                   break ;
        case 11: pbProcess->Position = 70; STL.Init();                   break ;
        case 12: pbProcess->Position = 85; ULD.Init();                   break ;
        case 13: pbProcess->Position = 90; SEQ.Init();                   break ;
        case 14: pbProcess->Position = 95; SPC.Init();                   break ;
        case 15: pbProcess->Position = 99; LT .Init();
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
        case 0 : pbProcess->Position = 10; lbName -> Caption  = "HRT_430S" ;
                                           OM  .Close();                break ;
        case 1 : pbProcess->Position = 15; /*FreeSVA("pSVApp");*/       break ;
        case 2 : pbProcess->Position = 20; DM .Close();                 break ;
        case 3 : pbProcess->Position = 25; PM .Close();                 break ; 
        case 4 : pbProcess->Position = 30; MM .Close();                 break ; 
        case 5 : pbProcess->Position = 35; LAG.Close();                 break ;
        case 6 : pbProcess->Position = 40; PCM.Close();                 break ;
        case 7 : pbProcess->Position = 45; LDR.Close();                 break ;
        case 8 : pbProcess->Position = 50; PRB.Close();                 break ; 
        case 9 : pbProcess->Position = 55; WRK.Close();                 break ; 
        case 10: pbProcess->Position = 60; PSB.Close();                 break ; 
        case 11: pbProcess->Position = 65; STL.Close();                 break ; 
        case 12: pbProcess->Position = 70; ULD.Close();                 break ; 
        case 13: pbProcess->Position = 75; SEQ.Close();                 break ; 
        case 14: pbProcess->Position = 80; SPC.Close();                 break ; 
        case 15: pbProcess->Position = 85; LT .Close();                 
                 ModalResult = mrOk;                                    break ; 
    }
    ProcessIndex++;
    tmSave -> Enabled = true ;
}
//---------------------------------------------------------------------------

