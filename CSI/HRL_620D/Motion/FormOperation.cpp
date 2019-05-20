//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "UtilDefine.h"

#include "Sequence.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "LaserConUnit.h"
#include "Stage.h"
#include "UserINI.h"


#define PATH_IMAGE "D:\\BC6Works\\HRL_620D\\bin\\Util\\CAMERA\\"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
//#include "Loader.h"
//#include "PreBuff.h"
//#include "RailEpx.h"
//#include "Epoxy.h"
//#include "RailDie.h"                                     
//#include "Head.h"
//#include "Stage.h"
//#include "PostBuff.h"
//#include "UnLoader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)
    : TForm(Owner)
{
    EM_SetDisp(false);

    Panel7 -> DoubleBuffered = true;
    dUnit     = 0.0 ;
    dPitch    = 0.0 ;
    iUnitType = 0   ;

    ImBaseT -> Picture -> LoadFromFile("D:\\BC6Works\\HRL_620D\\bin\\Util\\CAMERA\\1.bmp");
}


//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    //if(SEQ._iSeqStat == ssError) {
    //    FM_MsgTime("Warning","에러 리셋을 해주십시오.",1000) ;
    //    return ;
    //}
    //
    //if(SEQ._iSeqStat != ssStop) {
    //    FM_MsgTime("Warning","장비가 정지상태가 아닙니다.",1000) ;
    //    return ;
    //}

    //int iManNo ;
    //int iActNo ;
    //iManNo = ((TBitBtn *)Sender) -> Tag ;

    //MM.SetManCycle((EN_MANUAL_CYCLE)iManNo);
}
//---------------------------------------------------------------------------

void TFrmOperation::LT_Display()
{
    LT.DispEqpInfo(pnDayTotalTime, pnTodayWork );
}

void TFrmOperation::OP_Dispaly()
{
    pnDoor         -> Color = OM.CmnOptn.bDoorSkip    ? clLime : clRed ; pnDoor         -> Caption = OM.CmnOptn.bDoorSkip     ? "ON" : "OFF" ;
    pnLaser        -> Color = OM.CmnOptn.bLaserSkip   ? clLime : clRed ; pnLaser        -> Caption = OM.CmnOptn.bLaserSkip    ? "ON" : "OFF" ;
    pnVaccum       -> Color = OM.CmnOptn.bVaccumSkip  ? clLime : clRed ; pnVaccum       -> Caption = OM.CmnOptn.bVaccumSkip   ? "ON" : "OFF" ;
    pnBrd2Skip     -> Color = OM.CmnOptn.bBrd2LsrSkip ? clLime : clRed ; pnBrd2Skip     -> Caption = OM.CmnOptn.bBrd2LsrSkip  ? "ON" : "OFF" ;
    pnUseRgh       -> Color = OM.CmnOptn.bUseOnlyRgh  ? clLime : clRed ; pnUseRgh       -> Caption = OM.CmnOptn.bUseOnlyRgh   ? "ON" : "OFF" ;
    pnDryRun       -> Color = OM.CmnOptn.bDryRun      ? clLime : clRed ; pnDryRun       -> Caption = OM.CmnOptn.bDryRun       ? "ON" : "OFF" ;
    pnNotUsedVAxis -> Color = OM.CmnOptn.bNotUsedVAxis? clLime : clRed ; pnNotUsedVAxis -> Caption = OM.CmnOptn.bNotUsedVAxis ? "ON" : "OFF" ;

}

//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
    SEQ._bBtnStart = true ;
    //MM.SetManCycle(mcCycleStart);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btStopClick(TObject *Sender)
{
    //SEQ._bBtnStop = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btResetClick(TObject *Sender)
{
    SEQ._bBtnReset = true ;


}
//---------------------------------------------------------------------------
bool __fastcall TFrmOperation::CheckStripDisappear()
{
    return true ;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmOperation::CheckStripUnknwon()
{
    return true ;
}


//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)
{

    tmUpdate->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
}




//---------------------------------------------------------------------------
typedef void (__closure *Func)();
void TFrmOperation::UpdateAray()
{
//    DM.UpdateAray();
}
void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;
    SEQ.m_dLastTime = Now();


    static bool bFirst = true;
    if(bFirst){
       if(IO_GetX(xSTG_WrkPstDetect)) {
           MT_SetServo(miSTG_TBase , true);
       }
       else {
           FM_MsgOk("WARNING" , "베이스 T축을 워킹 위치로 이동 후 홈을 수행 하세요.");
       }
    }
    bFirst = false ;

    lbBlockNo -> Caption = LCU.GetBlockNoID();



    //Lot Info Display
    LT_Display();

    //Error Display
    ER_Display();

    //Option Display
    OP_Dispaly();

    LV_Display();

    //Button Enabled
    btStart -> Enabled = !SEQ._bRun ;
    btRotate-> Enabled = !SEQ._bRun ;
    btHome  -> Enabled = !SEQ._bRun ;

    if(IO_GetX(xSTG_WrkPstDetect)) { spBaseDetect -> Visible = true ; }
    else                           { spBaseDetect -> Visible = false; }

    int iLevel;
    iLevel = FM_GetLevel();

    pnDoor       -> Enabled = iLevel == lvMaster;
    pnLaser      -> Enabled = iLevel == lvMaster;
    pnVaccum     -> Enabled = iLevel == lvMaster;
    btMasterOptn -> Enabled = iLevel == lvMaster;
    //pnTest  -> Visible = iLevel == lvMaster; //지우지 말고 /....

    int iTime = STG.m_dLaserWorkTime /100 ;

    AnsiString sTime = iTime / 10.0 ;

    pnWorkTime -> Caption = sTime ;

           bool bWrkFinish    = STG.GetCycleStep() == 0 ;
    static bool bPreWrkFinish = STG.GetCycleStep() == 0 ;

    if(bWrkFinish && !bPreWrkFinish && !EM_IsErr()){
        FM_MsgTime("알림" , "모든 작업이 완료 되었습니다." , 2000);
    }
    bPreWrkFinish = bWrkFinish;

    tmUpdate->Enabled = true;
}

void TFrmOperation::ER_Display()
{


    if(!lbErr) return ;

    //Local Var.
    AnsiString Str;
    static int iPreErrCnt  ;
    int        iCrntErrCnt ;

    for (int i = 0 ; i < EM_GetMaxErr() ; i++) {
        if (EM_GetErr(i)) iCrntErrCnt++;
    }
    
    if (iPreErrCnt != iCrntErrCnt ) {
        lbErr -> Clear();
                                                 
        AnsiString Str;
        lbErr -> Items -> Clear();
        int iErrNo = EM_GetLastErr();

        for (int i = 0 ; i < EM_GetMaxErr() ; i++) {
            if (EM_GetErr(i)) {
                Str.sprintf("[ERR%03d]" , i) ;
                Str += EM_GetErrName(i);
                lbErr -> Items -> Add(Str);
                if (iErrNo == i) lbErr -> ItemIndex = lbErr -> Count - 1 ;
            }
        }
    }
    iPreErrCnt = iCrntErrCnt ;
}


//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btTotalTmResetClick(TObject *Sender)
{
    int iTag = ((TButton *)Sender) -> Tag ;
    if(iTag==0){ LT.EqpInfo.dRunTime   = 0.0; pnDayTotalTime -> Caption = "00:00:00"; }
    if(iTag==1){ LT.EqpInfo.iTodayWork =   0; pnTodayWork    -> Caption =        "0"; }


}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmShowImgTimer(TObject *Sender)
{
    tmShowImg -> Enabled = false;


    //Image1    -> Picture = NULL;

    //ilShowImg->GetBitmap(m_iCount,Image1->Picture->Bitmap);
    //m_iCount++;

    //이미지 뿌려주기...
    int iRvrsPstn = PM.GetValue(miSTG_TBase, pvSTG_TBaseRvers);
    int iOrigPstn = PM.GetValue(miSTG_TBase, pvSTG_TBaseOrign);
    int iCmdPos   = MT_GetCmdPos(miSTG_TBase);
    int iAllMove  = iRvrsPstn - iOrigPstn    ;
    int iOneMove  = iAllMove / 11            ;
    int iCount ;



    AnsiString sPath = PATH_IMAGE ;
    //sPath = sPath + iCount +".bmp";
    if(iCmdPos == iOrigPstn){
        iCount = 1 ;
        sPath = sPath + iCount +".bmp";
        ImBaseT -> Picture -> LoadFromFile(sPath);
        pnBoard1 -> Top = 584 ;
        pnBoard2 -> Top = 42  ;

        sPath = PATH_IMAGE;

    }
    if(iCmdPos == iRvrsPstn){
        iCount = 13;
        sPath = sPath + iCount +".bmp";
        ImBaseT -> Picture -> LoadFromFile(sPath);
        pnBoard1 -> Top = 42 ;
        pnBoard2 -> Top = 584;

        sPath = PATH_IMAGE;
    }
    if(iCmdPos != iOrigPstn && iCmdPos != iRvrsPstn){
        iCount = iCmdPos / (iAllMove / 13) + 1;    //JH 이거 확인...
        if(iCount < 1 ) iCount = 1 ;
        if(iCount > 12) iCount = 13;
        sPath = sPath + iCount +".bmp";
        ImBaseT -> Picture -> LoadFromFile(sPath);

        sPath = PATH_IMAGE;
    }

    if(!OM.CmnOptn.bLaserSkip && LCU.m_bWrkLaser) { spLaser -> Visible = true ; }//SEQ._bFlick; }
    else                                          { spLaser -> Visible = false; }

    if(OM.CmnOptn.bDryRun && !EM_IsErr() && MM.GetManNo() == mcNoneCycle){
        MM.SetManCycle(mcCycleStart);
    }
    else if(OM.CmnOptn.bDryRun && EM_IsErr()){
        OM.CmnOptn.bDryRun = false;
    }              //JS 여기서 작업시작.


    tmShowImg -> Enabled = true;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btRotateClick(TObject *Sender)
{
/*
    if(SEQ._bRun) {
        FM_MsgTime("Warning","장비가 정지상태가 아닙니다.",500) ;
        return ;
    }

    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle((EN_MANUAL_CYCLE)iManNo);
*/
    SEQ._bBtnRotate = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button3Click(TObject *Sender)
{
    AnsiString sTemp =  LCU.GetChksum(Edit1->Text);

    ShowMessage(sTemp);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btHomeClick(TObject *Sender)
{
    SEQ._bBtnHome = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
    Trace("Operator", "Operation Form_Oper Button Click") ;
}
//---------------------------------------------------------------------------

void TFrmOperation::LV_Display()
{
    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }
}


void __fastcall TFrmOperation::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;
    Trace("Operator", "Operation Form_Operator Button Click") ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnDoorClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    switch(iSel){
        case 0 : OM.CmnOptn.bDoorSkip   = !OM.CmnOptn.bDoorSkip   ; break;
        case 1 : OM.CmnOptn.bLaserSkip  = !OM.CmnOptn.bLaserSkip  ; break;
        case 2 : OM.CmnOptn.bVaccumSkip = !OM.CmnOptn.bVaccumSkip ; break;
    }
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button2Click(TObject *Sender)
{
    LCU.SendMsg(Edit2 -> Text) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btMasterOptnClick(TObject *Sender)
{
    pnMaster -> Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btCloseClick(TObject *Sender)
{
    pnMaster -> Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::pnBrd2SkipClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    switch(iSel){
        case 0 : OM.CmnOptn.bBrd2LsrSkip  = !OM.CmnOptn.bBrd2LsrSkip ; break;
        case 1 : OM.CmnOptn.bUseOnlyRgh   = !OM.CmnOptn.bUseOnlyRgh  ; break;
        case 2 : OM.CmnOptn.bDryRun       = !OM.CmnOptn.bDryRun      ; break;
        case 3 : OM.CmnOptn.bNotUsedVAxis = !OM.CmnOptn.bNotUsedVAxis; break;
    }
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------


