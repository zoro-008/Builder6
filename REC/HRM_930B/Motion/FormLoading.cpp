//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "LoaderFront.h"
#include "LoaderRear.h"
#include "Index.h"
#include "LeftTool.h"
#include "RightTool.h"
#include "Stage.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
//Rs232 Part.
//---------------------------------------------------------------------------
#include "Rs232Dispr.h"
#include "Rs232Keyence.h"
#include "Rs232LoadCl.h"
//---------------------------------------------------------------------------
//#include "HexaPot.h"
#include "VisionCom.h"
#include "CadPosition.h"
//---------------------------------------------------------------------------

#include "Sequence.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "UserINI.h"
#include "LotUnit.h"

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

        //case 0 : pbProcess->Position = 0  ; GetNormalEnd();                                                                  break ;
        case 0 : pbProcess->Position = 0  ; OM   .Init();
                                            lbName    -> Caption  = OM.EqpOptn.sModelName ;
                                            LoadSML("SMDll" );
                                            MT_SetServoAll(true);
                                            break ;
        case 1 : pbProcess->Position = 5  ; Rs232_DisprFt.Init(rsDispensorFt); 
                                            Rs232_DisprRr.Init(rsDispensorRr); 
                                            break ;

        case 2 : pbProcess->Position = 10 ; CAD  .Init();
                                            CAD_Rtt.Init();
                                            CAD  .SetRowCol(OM.DevInfo.iMidColCnt , OM.DevInfo.iMidRowCnt);                  break ;
        case 3 : pbProcess->Position = 15 ; DM   .Init(     );                                                               break ;
        case 4 : pbProcess->Position = 20 ; PM   .Init(     );                                                               break ;
        case 5 : pbProcess->Position = 25 ; MM   .Init(     );                                                               break ;
        case 6 : pbProcess->Position = 30 ; OM   .Init(     );                                                               break ;
        case 7 : pbProcess->Position = 35 ; VSN_L.Init(     );                                                     break ;
        case 8 : pbProcess->Position = 40 ; VSN_R.Init(     );                                                               break ;
        case 9 : pbProcess->Position = 45 ; LDR_F.Init(     );                                                               break ;
        case 10: pbProcess->Position = 50 ; LDR_R.Init(     );                                                               break ;
        case 11: pbProcess->Position = 55 ; IDX  .Init(     );                                                               break ;
        case 12: pbProcess->Position = 60 ; LTL  .Init(     );                                                               break ;
        case 13: pbProcess->Position = 65 ; RTL  .Init(     );                                                               break ;
        case 14: pbProcess->Position = 70 ; STG  .Init(     );                                                               break ;
        case 15: pbProcess->Position = 85 ; ULD  .Init(     );                                                               break ;
        case 16: pbProcess->Position = 90 ; SPC  .Init(     );                                                               break ;
        case 17: pbProcess->Position = 95 ; LOT  .Init(     );                                                               break ;
        case 18: pbProcess->Position = 100; SEQ  .Init(     ); ModalResult = mrOk;                                           break ;
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
        case 0 : pbProcess->Position = 0  ; OM   .Close();                break ;
        case 1 : pbProcess->Position = 5  ; 
                                            CAD  .Close();
                                            CAD_Rtt .Close();
                                            Rs232_DisprFt.Close();
                                            Rs232_DisprRr.Close();
                                                                          break ;
        case 2 : pbProcess->Position = 10 ; DM   .Close();                break ;  //이게 오래걸림.
        case 3 : pbProcess->Position = 15 ; PM   .Close();                break ;
        case 4 : pbProcess->Position = 20 ; MM   .Close();                break ;
        case 5 : pbProcess->Position = 25 ; OM   .Close();                break ;
        case 6 : pbProcess->Position = 35 ; VSN_L.Close();                break ;
        case 7 : pbProcess->Position = 40 ; VSN_L.Close();                break ;
        case 8 : pbProcess->Position = 45 ; LDR_F.Close();                break ;
        case 9 : pbProcess->Position = 50 ; LDR_R.Close();                break ;
        case 10: pbProcess->Position = 55 ; IDX  .Close();                break ;
        case 11: pbProcess->Position = 60 ; LTL  .Close();                break ;
        case 12: pbProcess->Position = 65 ; RTL  .Close();                break ;
        case 13: pbProcess->Position = 70 ; STG  .Close();                break ;
        case 14: pbProcess->Position = 75 ; ULD  .Close();                break ;
        case 15: pbProcess->Position = 80 ; SPC  .Close();                break ;
        case 16: pbProcess->Position = 90 ; LOT  .Close();                break ;
        case 17: pbProcess->Position = 95 ; /*SetNormalEnd();*/           break ;
        case 18: pbProcess->Position = 100; ModalResult = mrOk;           break ;
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


