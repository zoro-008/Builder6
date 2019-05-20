//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"
#include "SLogUnit.h"

#include "OptionMan.h"
#include "Sequence.h"
#include "PstnMan.h"
#include "DataMan.h"
#include "Stage.h"

#include "ManualMan.h"
#include "UserINI.h"


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

        case 0 : pbProcess->Position = 0  ; GetNormalEnd();               break ;
        case 1 : pbProcess->Position = 5  ; OM .Init(); LoadSML("SMDll" );break ; //lbName    -> Caption  = OM.EqpOptn.sModelName ;  break ;
        case 2 : pbProcess->Position = 10 ; DM .Init();                   break ;
        case 3 : pbProcess->Position = 15 ; PM .Init();                   break ;    //OM .Init(); lbName    -> Caption  = OM.EqpOptn.sModelName ; LoadSML("SMDll" ); break ;
        case 4 : pbProcess->Position = 20 ;                               break ;
        case 5 : pbProcess->Position = 25 ;                               break ;
        case 6 : pbProcess->Position = 30 ;                               break ;
        case 7 : pbProcess->Position = 35 ; STG.Init();                   break ;
        case 8 : pbProcess->Position = 40 ;                               break ;
        case 9 : pbProcess->Position = 45 ;                               break ;
        case 10: pbProcess->Position = 50 ;                               break ;
        case 11: pbProcess->Position = 55 ;                               break ;
        case 12: pbProcess->Position = 60 ;                               break ;
        case 13: pbProcess->Position = 65 ;                               break ;
        case 14: pbProcess->Position = 70 ;                               break ;
        case 15: pbProcess->Position = 75 ;                               break ;
        case 16: pbProcess->Position = 80 ;                               break ;
        case 17: pbProcess->Position = 85 ; SEQ.Init();                   break ;
        case 18: pbProcess->Position = 90 ;                               break ;
        case 19: pbProcess->Position = 95 ;                               break ;
        case 20: pbProcess->Position = 100; ModalResult = mrOk;           break ;
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
        case 0 : pbProcess->Position = 0  ; OM  .Close();                break ;
        case 1 : pbProcess->Position = 5  ; /*FreeSVA("pSVApp");*/       break ;
        case 2 : pbProcess->Position = 10 ; DM.Close();                  break ;
        case 3 : pbProcess->Position = 15 ; PM .Close();                 break ;
        case 4 : pbProcess->Position = 20 ; /*MM .Close(); */                break ;
        case 5 : pbProcess->Position = 25 ;                              break ;
        case 6 : pbProcess->Position = 30 ; STG.Close();                 break ;
        case 7 : pbProcess->Position = 35 ;                              break ;
        case 8 : pbProcess->Position = 40 ;                              break ;
        case 9 : pbProcess->Position = 45 ;                              break ;
        case 10: pbProcess->Position = 50 ;                              break ;
        case 11: pbProcess->Position = 55 ;                              break ;
        case 12: pbProcess->Position = 60 ;                              break ;
        case 13: pbProcess->Position = 65 ;                              break ;
        case 14: pbProcess->Position = 70 ;                              break ;
        case 15: pbProcess->Position = 75 ;                              break ;
        case 16: pbProcess->Position = 80 ;                              break ;
        case 17: pbProcess->Position = 85 ; SEQ.Close();                 break ;
        case 18: pbProcess->Position = 90 ;                              break ;
        case 19: pbProcess->Position = 95 ; SetNormalEnd();              break ;
        case 20: pbProcess->Position = 100; ModalResult = mrOk;          break ;
    }

    ProcessIndex++;
    tmSave -> Enabled = true ;
}

//---------------------------------------------------------------------------
bool TFrmLoading::GetNormalEnd()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    sPath= EXE_FOLDER + "SeqData\\NormalEnd.INI" ;

    //Load
    AnsiString sIndex   ;
    AnsiString sTemp    ;

    int iNormalEnd ;
    UserINI.Load(sPath, "NormalEnd" , "__NormalEnded__", iNormalEnd );
    if(!iNormalEnd) {
        ShowMessage("프로그램이 정상종료 되지 않았습니다. 트레이 및 툴 데이터를 초기화해주세요");
        Trace("Warning","프로그램이 정상종료 되지 않았습니다. 트레이 및 툴 데이터를 초기화해주세요");
    }
    UserINI.Save(sPath, "NormalEnd" , "__NormalEnded__", 0 );

    return true ;
}

bool TFrmLoading::SetNormalEnd()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    sPath= EXE_FOLDER + "SeqData\\NormalEnd.INI" ;

    //Load
    UserINI.Save(sPath, "NormalEnd" , "__NormalEnded__", 1 );

    return true ;
}

