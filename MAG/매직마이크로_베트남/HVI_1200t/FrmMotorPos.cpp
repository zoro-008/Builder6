//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmMotorPos.h"                                                              
//---------------------------------------------------------------------------         
#include "SLogUnit.h"                                                                 
#include "UtilDefine.h"                                                               
#include "OptionMan.h"                                                                
#include "Loader.h"                                                                   
#include "Head.h"
#include "Rail.h"
#include "PstBuff.h"
#include "UnLoader.h"
#include "PstnMan.h"
#include "DataMan.h"
#include "SMInterfaceUnit.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMotorPos *FormMotorPos;
int    iTrmManCol = -1;            
int    iTrmManRow = -1;

//---------------------------------------------------------------------------
__fastcall TFormMotorPos::TFormMotorPos(TComponent* Owner)
    : TForm(Owner)
{
    //Position Window Set to Panel
    PM.SetWindow (pnPs_WRK_XVsn,miWRK_XVsn);
    PM.SetWindow (pnPs_WRK_YVsn,miWRK_YVsn);
    PM.SetWindow (pnPs_PSB_XTrm,miPSB_XTrm);
    PM.SetWindow (pnPs_PSB_YTrm,miPSB_YTrm);
    PM.SetWindow (pnPs_PSB_TFlp,miPSB_TFlp);
    PM.SetWindow (pnPs_PSB_XIns,miPSB_XIns);
    PM.SetWindow (pnPs_WR1_XCvr,miWR1_XCvr);
    PM.SetWindow (pnPs_WR2_XCvr,miWR2_XCvr);
    PM.SetWindow (pnPs_WR3_XCvr,miWR3_XCvr);
    PM.SetWindow (pnPs_LDR_YCmp,miLDR_YCmp);
    PM.SetWindow (pnPs_LDR_ZCmp,miLDR_ZCmp);
    PM.SetWindow (pnPs_ULD_YCmp,miULD_YCmp);
    PM.SetWindow (pnPs_ULD_ZCmp,miULD_ZCmp);

    PM.SetWindow (pnSpare1,miSpare);

    //Motor Window Set to Panel
    for(int i=0 ; i<MAX_MOTR ; i++) FrmMotor[i] = new TFrameMotr();

    FrmMotor[miWRK_XVsn]->SetIdType(miWRK_XVsn,dtCCwCw_X); //하고나서 보니 나밖에 모르겟다 ...dd
    FrmMotor[miWRK_YVsn]->SetIdType(miWRK_YVsn,dtCCwCw_Y); //추후에 dt 이거 수정 예정 ... dd
    FrmMotor[miPSB_XTrm]->SetIdType(miPSB_XTrm,dtCwCCw_X); //센서 위치 바꿈.dd
    FrmMotor[miPSB_YTrm]->SetIdType(miPSB_YTrm,dtCCwCw_Y);
    FrmMotor[miPSB_TFlp]->SetIdType(miPSB_TFlp,dtCwCCw_L);
    FrmMotor[miPSB_XIns]->SetIdType(miPSB_XIns,dtCCwCw_X);
    FrmMotor[miWR1_XCvr]->SetIdType(miWR1_XCvr,dtCwCCw_X);
    FrmMotor[miWR2_XCvr]->SetIdType(miWR2_XCvr,dtCwCCw_X);
    FrmMotor[miWR3_XCvr]->SetIdType(miWR3_XCvr,dtCwCCw_X);
    FrmMotor[miLDR_YCmp]->SetIdType(miLDR_YCmp,dtCCwCw_Y);
    FrmMotor[miLDR_ZCmp]->SetIdType(miLDR_ZCmp,dtCCwCw_Z);
    FrmMotor[miULD_YCmp]->SetIdType(miULD_YCmp,dtCCwCw_Y);
    FrmMotor[miULD_ZCmp]->SetIdType(miULD_ZCmp,dtCCwCw_Z);

    FrmMotor[miWRK_XVsn]->Parent = pnMt_WRK_XVsn ;
    FrmMotor[miWRK_YVsn]->Parent = pnMt_WRK_YVsn ;
    FrmMotor[miPSB_XTrm]->Parent = pnMt_PSB_XTrm ;
    FrmMotor[miPSB_YTrm]->Parent = pnMt_PSB_YTrm ;                                             
    FrmMotor[miPSB_TFlp]->Parent = pnMt_PSB_TFlp ;
    FrmMotor[miPSB_XIns]->Parent = pnMt_PSB_XIns ;
    FrmMotor[miWR1_XCvr]->Parent = pnMt_WR1_XCvr ;
    FrmMotor[miWR2_XCvr]->Parent = pnMt_WR2_XCvr ;
    FrmMotor[miWR3_XCvr]->Parent = pnMt_WR3_XCvr ;
    FrmMotor[miLDR_YCmp]->Parent = pnMt_LDR_YCmp ;
    FrmMotor[miLDR_ZCmp]->Parent = pnMt_LDR_ZCmp ;
    FrmMotor[miULD_YCmp]->Parent = pnMt_ULD_YCmp ;
    FrmMotor[miULD_ZCmp]->Parent = pnMt_ULD_ZCmp ;

    for(int i=0 ; i<MAX_MOTR ; i++) FrmMotor[i]->Show();                                       

    //Cylinder Window Set to Panel
    for(int i=0 ; i<MAX_ACTR ; i++) FrmCylinder[i] = new TFrameCyl();
    //CCw 가 빽이라고 생각해주세요 ㅠㅠ.
    //나중에 고칠게요 ㅠㅠ ...나중에...dd
//    FrmCylinder[aiPRB_Stopper ]->SetIdType(aiPRB_Stopper,dtCwCCw_Z);    FrmCylinder[aiPRB_Stopper ]->Parent = pnPRB_Stpr      ;  //없어서 하나 추가함.dd 밑에 있넹/.dd

    FrmCylinder[aiLDR_ClmpUpDn   ]->SetIdType(aiLDR_ClmpUpDn   ,dtCwCCw_Z);   FrmCylinder[aiLDR_ClmpUpDn   ]->Parent = pnLDR_ClmpUpDn    ;
    FrmCylinder[aiLDR_PshrFwBw   ]->SetIdType(aiLDR_PshrFwBw   ,dtCCwCw_X);   FrmCylinder[aiLDR_PshrFwBw   ]->Parent = pnLDR_PshrFwBw    ;
    FrmCylinder[aiPRB_StprUpDn   ]->SetIdType(aiPRB_StprUpDn   ,dtCwCCw_Z);   FrmCylinder[aiPRB_StprUpDn   ]->Parent = pnPRB_StprUpDn    ;
    FrmCylinder[aiWK1_StprUpDn   ]->SetIdType(aiWK1_StprUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK1_StprUpDn   ]->Parent = pnWK1_StprUpDn    ;
    FrmCylinder[aiWK1_AlgnFwBw   ]->SetIdType(aiWK1_AlgnFwBw   ,dtCwCCw_Y);   FrmCylinder[aiWK1_AlgnFwBw   ]->Parent = pnWK1_AlgnFwBw    ;
    FrmCylinder[aiWK1_SttnUpDn   ]->SetIdType(aiWK1_SttnUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK1_SttnUpDn   ]->Parent = pnWK1_SttnUpDn    ;
    FrmCylinder[aiWK2_StprUpDn   ]->SetIdType(aiWK2_StprUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK2_StprUpDn   ]->Parent = pnWK2_StprUpDn    ;
    FrmCylinder[aiWK2_AlgnFwBw   ]->SetIdType(aiWK2_AlgnFwBw   ,dtCwCCw_Y);   FrmCylinder[aiWK2_AlgnFwBw   ]->Parent = pnWK2_AlgnFwBw    ;
    FrmCylinder[aiWK2_SttnUpDn   ]->SetIdType(aiWK2_SttnUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK2_SttnUpDn   ]->Parent = pnWK2_SttnUpDn    ;
    FrmCylinder[aiWK3_StprUpDn   ]->SetIdType(aiWK3_StprUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK3_StprUpDn   ]->Parent = pnWK3_StprUpDn    ;
    FrmCylinder[aiWK3_AlgnFwBw   ]->SetIdType(aiWK3_AlgnFwBw   ,dtCwCCw_Y);   FrmCylinder[aiWK3_AlgnFwBw   ]->Parent = pnWK3_AlgnFwBw    ;
    FrmCylinder[aiWK3_SttnUpDn   ]->SetIdType(aiWK3_SttnUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK3_SttnUpDn   ]->Parent = pnWK3_SttnUpDn    ;

    FrmCylinder[aiWK1_StprUpDn   ]->SetIdType(aiWK1_StprUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK1_StprUpDn   ]->Parent = pnWK1_StprUpDn2   ;
    FrmCylinder[aiWK1_AlgnFwBw   ]->SetIdType(aiWK1_AlgnFwBw   ,dtCwCCw_Y);   FrmCylinder[aiWK1_AlgnFwBw   ]->Parent = pnWK1_AlgnFwBw2   ;
    FrmCylinder[aiWK1_SttnUpDn   ]->SetIdType(aiWK1_SttnUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK1_SttnUpDn   ]->Parent = pnWK1_SttnUpDn2   ;
    FrmCylinder[aiWK2_StprUpDn   ]->SetIdType(aiWK2_StprUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK2_StprUpDn   ]->Parent = pnWK2_StprUpDn2   ;
    FrmCylinder[aiWK2_AlgnFwBw   ]->SetIdType(aiWK2_AlgnFwBw   ,dtCwCCw_Y);   FrmCylinder[aiWK2_AlgnFwBw   ]->Parent = pnWK2_AlgnFwBw2   ;
    FrmCylinder[aiWK2_SttnUpDn   ]->SetIdType(aiWK2_SttnUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK2_SttnUpDn   ]->Parent = pnWK2_SttnUpDn2   ;
    FrmCylinder[aiWK3_StprUpDn   ]->SetIdType(aiWK3_StprUpDn   ,dtCwCCw_Z);   FrmCylinder[aiWK3_StprUpDn   ]->Parent = pnWK3_StprUpDn2   ;
    FrmCylinder[aiWK3_AlgnFwBw   ]->SetIdType(aiWK3_AlgnFwBw   ,dtCwCCw_Y);   FrmCylinder[aiWK3_AlgnFwBw   ]->Parent = pnWK3_AlgnFwBw2   ;

    FrmCylinder[aiPSB_AlgnFwBw   ]->SetIdType(aiPSB_AlgnFwBw   ,dtCwCCw_Y);   FrmCylinder[aiPSB_AlgnFwBw   ]->Parent = pnPSB_AlgnFwBw    ;
    FrmCylinder[aiPSB_IndxUpDn   ]->SetIdType(aiPSB_IndxUpDn   ,dtCwCCw_Z);   FrmCylinder[aiPSB_IndxUpDn   ]->Parent = pnPSB_IndxUpDn    ;
    FrmCylinder[aiPSB_FlprOpCl   ]->SetIdType(aiPSB_FlprOpCl   ,dtCCwCw_Z);   FrmCylinder[aiPSB_FlprOpCl   ]->Parent = pnPSB_FlprOpCl    ;
    FrmCylinder[aiPSB_GripUpDn   ]->SetIdType(aiPSB_GripUpDn   ,dtCwCCw_Z);   FrmCylinder[aiPSB_GripUpDn   ]->Parent = pnPSB_GripUpDn    ;
    FrmCylinder[aiPSB_PshrFwBw   ]->SetIdType(aiPSB_PshrFwBw   ,dtCwCCw_X);   FrmCylinder[aiPSB_PshrFwBw   ]->Parent = pnPSB_PshrFwBw    ;
    FrmCylinder[aiPSB_PshrDnUp   ]->SetIdType(aiPSB_PshrDnUp   ,dtCCwCw_Z);   FrmCylinder[aiPSB_PshrDnUp   ]->Parent = pnPSB_PshrDnUp    ;

    FrmCylinder[aiPSB_AlgnFwBw   ]->SetIdType(aiPSB_AlgnFwBw   ,dtCwCCw_Y);   FrmCylinder[aiPSB_AlgnFwBw   ]->Parent = pnPSB_AlgnFwBw2   ;
    FrmCylinder[aiPSB_IndxUpDn   ]->SetIdType(aiPSB_IndxUpDn   ,dtCwCCw_Z);   FrmCylinder[aiPSB_IndxUpDn   ]->Parent = pnPSB_IndxUpDn2   ;
    FrmCylinder[aiPSB_FlprOpCl   ]->SetIdType(aiPSB_FlprOpCl   ,dtCCwCw_Z);   FrmCylinder[aiPSB_FlprOpCl   ]->Parent = pnPSB_FlprOpCl2   ;
    FrmCylinder[aiPSB_GripUpDn   ]->SetIdType(aiPSB_GripUpDn   ,dtCwCCw_Z);   FrmCylinder[aiPSB_GripUpDn   ]->Parent = pnPSB_GripUpDn2   ;
    FrmCylinder[aiPSB_PnchDnUp   ]->SetIdType(aiPSB_PnchDnUp   ,dtCCwCw_Z);   FrmCylinder[aiPSB_PnchDnUp   ]->Parent = pnPSB_PnchDnUp    ;
    FrmCylinder[aiPSB_SortFwBw   ]->SetIdType(aiPSB_SortFwBw   ,dtCwCCw_X);   FrmCylinder[aiPSB_SortFwBw   ]->Parent = pnPSB_SortFwBw    ;
    FrmCylinder[aiPSB_SrtStprFwBw]->SetIdType(aiPSB_SrtStprFwBw,dtCwCCw_Z);   FrmCylinder[aiPSB_SrtStprFwBw]->Parent = pnPSB_SrtStprFwBw ;


    FrmCylinder[aiULD_ClmpUpDn   ]->SetIdType(aiULD_ClmpUpDn   ,dtCwCCw_Z);   FrmCylinder[aiULD_ClmpUpDn   ]->Parent = pnULD_ClmpUpDn    ;

    for(int i=0 ; i<MAX_ACTR ; i++) FrmCylinder[i]->Show();

    btPSB_AllLiftUp1  -> Tag = (int)mcETC_AllAlignLiftUp ;
    btPSB_AllLiftUp2  -> Tag = (int)mcETC_AllAlignLiftUp ;

    btPSB_AllLiftDn1  -> Tag = (int)mcETC_AllLiftDn ;
    btPSB_AllLiftDn2  -> Tag = (int)mcETC_AllLiftDn ;

    btWRK_feedingOn1  -> Tag = (int)yWRK_FeedingAC ;
    btWRK_feedingOn2  -> Tag = (int)yWRK_FeedingAC ;

    btPRB_feedingOn   -> Tag = (int)yPRB_FeedingAC ;

    btLDR_InAcMotor   -> Tag = (int)yLDR_MgzInAC   ;
    btLDR_OutAcMotor  -> Tag = (int)yLDR_MgzOutAC  ;

    btULD_InAcMotor   -> Tag = (int)yULD_MgzInAC   ;
    btULD_OutAcMotor  -> Tag = (int)yULD_MgzOutAC  ;

}
//---------------------------------------------------------------------
bool __fastcall TFormMotorPos::MotorCheck(EN_MOTR_ID _iMotor)
{
    bool bRet1 = true;

         if(_iMotor == miWRK_XVsn) {}
    else if(_iMotor == miWRK_YVsn) {}
    else if(_iMotor == miPSB_XTrm) {
        if(!AT_Complete(aiPSB_PnchDnUp , ccBwd)) {
            AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);
            bRet1 = false;
        }
    }
    else if(_iMotor == miPSB_YTrm) {
        if(!AT_Complete(aiPSB_PnchDnUp , ccBwd)) {
            AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);
            bRet1 = false;
        }
    }
    else if(_iMotor == miPSB_TFlp) {}
    else if(_iMotor == miPSB_XIns) {}
    else if(_iMotor == miWR1_XCvr) {}
    else if(_iMotor == miWR2_XCvr) {}
    else if(_iMotor == miWR3_XCvr) {}
    else if(_iMotor == miLDR_YCmp) {}
    else if(_iMotor == miLDR_ZCmp) {}
    else if(_iMotor == miULD_YCmp) {}
    else if(_iMotor == miULD_ZCmp) {}

    return bRet1 ; // FM_MsgYesNo("Confirm","Mgz Detected, Are you Open the Clamp ?") != mrYes  return ;
}
//---------------------------------------------------------------------------
void __fastcall TFormMotorPos::JobFileBackUp()
{
    String sSorc  = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
    String sPath  = ExtractFilePath(Application -> ExeName) + "BackUp\\JobFile\\" ;
    String SDest  = sPath + Now().FormatString("yyyymmdd") + "\\" + Now().FormatString("hh'h'nn'm'ss's'") + "\\" ;

    UserFile.ClearDirDate(sPath , Now() - 90);

    sPath += Now().FormatString("yyyymmdd") + "\\" + Now().FormatString("hh'h'nn'm'ss's'") + "\\" ;

    UserFile.CopyDir(sSorc , SDest);
}
//---------------------------------------------------------------------------
void __fastcall TFormMotorPos::btSaveDeviceClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    JobFileBackUp();

    PM.UpdatePstn(toBuff);

    PM.Save(OM.m_sCrntDev);

    OM.DMSetMaxColRow();
}
//---------------------------------------------------------------------------
void __fastcall TFormMotorPos::FormShow(TObject *Sender)
{
    PM.UpdatePstn (toTabl);
    PageControl->ActivePageIndex = 0;
    //dd 아 함수로 빼고 페이지 체인지에 넣어야 ...
    FrmCylinder[aiWK1_StprUpDn ]->Parent = pnWK1_StprUpDn   ;
    FrmCylinder[aiWK1_AlgnFwBw ]->Parent = pnWK1_AlgnFwBw   ;
    FrmCylinder[aiWK1_SttnUpDn ]->Parent = pnWK1_SttnUpDn   ;
    FrmCylinder[aiWK2_StprUpDn ]->Parent = pnWK2_StprUpDn   ;
    FrmCylinder[aiWK2_AlgnFwBw ]->Parent = pnWK2_AlgnFwBw   ;
    FrmCylinder[aiWK2_SttnUpDn ]->Parent = pnWK2_SttnUpDn   ;
    FrmCylinder[aiWK3_StprUpDn ]->Parent = pnWK3_StprUpDn   ;
    FrmCylinder[aiWK3_AlgnFwBw ]->Parent = pnWK3_AlgnFwBw   ;
    FrmCylinder[aiWK3_SttnUpDn ]->Parent = pnWK3_SttnUpDn   ;

    //if(!OM.MstOptn.bUseZMarkingMT) TabSheet4 -> Visible = false;
    tmUpdateTimer->Enabled = true ;
}
//-------------------------------------------------------------------------
void __fastcall TFormMotorPos::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
    bool AllLiftUp   = AT_Complete(aiWK1_SttnUpDn , ccBwd) && AT_Complete(aiWK2_SttnUpDn , ccBwd) && AT_Complete(aiWK3_SttnUpDn , ccBwd);
    bool AllLiftDn   = AT_Complete(aiWK1_SttnUpDn , ccFwd) && AT_Complete(aiWK2_SttnUpDn , ccFwd) && AT_Complete(aiWK3_SttnUpDn , ccFwd);

    //bool XTrmWorkPs  = MT_CmprPos(miPSB_XTrm, PM.GetValue(miPSB_XTrm, pvPSB_XTrmWorkStart)) ;
    //bool YTrmWorkPs  = MT_CmprPos(miPSB_YTrm, PM.GetValue(miPSB_XTrm, pvPSB_XTrmWorkStart)) ;
    //bool XTrmWorkNPs = MT_CmprPos(miPSB_XTrm, PM.GetValue(miPSB_XTrm, pvPSB_XTrmWorkStart) - MaxColPich) ;
    //bool YTrmWorkPPs = MT_CmprPos(miPSB_YTrm, PM.GetValue(miPSB_YTrm, pvPSB_YTrmWorkStart) + MaxRowPich) ;

    //bool TrmLeftUp   = XTrmWorkPs  && YTrmWorkPs  ;
    //bool TrmLeftDn   = XTrmWorkPs  && YTrmWorkPPs ;
    //bool TrmRightUp  = XTrmWorkNPs && YTrmWorkPs  ;
    //bool TrmRightDn  = XTrmWorkNPs && YTrmWorkPPs ;


    switch ( PageControl -> ActivePageIndex ) {
       case 0 : btPSB_AllLiftDn1  -> Font -> Color =  AllLiftUp ? clLime : clWindowText ;
                btPSB_AllLiftUp1  -> Font -> Color = !AllLiftUp ? clLime : clWindowText ;
                btWRK_feedingOn1  -> Font -> Color = IO_GetY(yWRK_FeedingAC) ? clLime : clWindowText ;
                btWRK_feedingOn1  -> Caption = IO_GetY(yWRK_FeedingAC) ? "2nd Feeding On" : "2nd Feeding Off";
                break ;

       case 1 : btPSB_AllLiftDn2  -> Font -> Color =  AllLiftUp ? clLime : clWindowText ;
                btPSB_AllLiftUp2  -> Font -> Color = !AllLiftUp ? clLime : clWindowText ;
                btWRK_feedingOn2  -> Font -> Color = IO_GetY(yWRK_FeedingAC) ? clLime : clWindowText ;
                btWRK_feedingOn2  -> Caption = IO_GetY(yWRK_FeedingAC) ? "2nd Feeding On" : "2nd Feeding Off";  break ;

       case 2 :                                                                                         break ;

       case 3 :                                                                                         break ;

       case 4 :
                btPRB_feedingOn   -> Font -> Color = IO_GetY(yPRB_FeedingAC) ? clLime : clWindowText ;
                btPRB_feedingOn   -> Caption = IO_GetY(yPRB_FeedingAC) ? "1st Feeding On" : "1st Feeding Off";
                btLDR_InAcMotor   -> Font -> Color = IO_GetY(yLDR_MgzInAC) ? clLime : clWindowText ;
                btLDR_InAcMotor   -> Caption = IO_GetY(yLDR_MgzInAC) ? "LDR In Mgz On" : "LDR In Mgz Off";
                btLDR_OutAcMotor  -> Font -> Color = IO_GetY(yLDR_MgzOutAC) ? clLime : clWindowText ;
                btLDR_OutAcMotor  -> Caption = IO_GetY(yLDR_MgzOutAC) ? "LDR Out Mgz On" : "LDR Out Mgz Off"; break ;

       case 5 : btULD_InAcMotor   -> Font -> Color = IO_GetY(yULD_MgzInAC) ? clLime : clWindowText ;
                btULD_InAcMotor   -> Caption = IO_GetY(yULD_MgzInAC) ? "ULD In Mgz On" : "ULD In Mgz Off";
                btULD_OutAcMotor  -> Font -> Color = IO_GetY(yULD_MgzOutAC) ? clLime : clWindowText ;
                btULD_OutAcMotor  -> Caption = IO_GetY(yULD_MgzOutAC) ? "ULD Out Mgz On" : "ULD Out Mgz Off";
                                                                                               break ;
    }


    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdateTimer ->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::PageControlChange(TObject *Sender)
{
     switch ( PageControl -> ActivePageIndex ) {
         case 0 :
                  FrmCylinder[aiWK1_StprUpDn ]->Parent = pnWK1_StprUpDn  ;
                  FrmCylinder[aiWK1_AlgnFwBw ]->Parent = pnWK1_AlgnFwBw  ;
                  FrmCylinder[aiWK1_SttnUpDn ]->Parent = pnWK1_SttnUpDn  ;
                  FrmCylinder[aiWK2_StprUpDn ]->Parent = pnWK2_StprUpDn  ;
                  FrmCylinder[aiWK2_AlgnFwBw ]->Parent = pnWK2_AlgnFwBw  ;
                  FrmCylinder[aiWK2_SttnUpDn ]->Parent = pnWK2_SttnUpDn  ;
                  FrmCylinder[aiWK3_StprUpDn ]->Parent = pnWK3_StprUpDn  ;
                  FrmCylinder[aiWK3_AlgnFwBw ]->Parent = pnWK3_AlgnFwBw  ;
                  FrmCylinder[aiWK3_SttnUpDn ]->Parent = pnWK3_SttnUpDn  ;
         break ;
         
         case 1 :
                  FrmCylinder[aiWK1_StprUpDn ]->Parent = pnWK1_StprUpDn2 ;
                  FrmCylinder[aiWK1_AlgnFwBw ]->Parent = pnWK1_AlgnFwBw2 ;
                  FrmCylinder[aiWK1_SttnUpDn ]->Parent = pnWK1_SttnUpDn2 ;
                  FrmCylinder[aiWK2_StprUpDn ]->Parent = pnWK2_StprUpDn2 ;
                  FrmCylinder[aiWK2_AlgnFwBw ]->Parent = pnWK2_AlgnFwBw2 ;
                  FrmCylinder[aiWK2_SttnUpDn ]->Parent = pnWK2_SttnUpDn2 ;
                  FrmCylinder[aiWK3_StprUpDn ]->Parent = pnWK3_StprUpDn2 ;
                  FrmCylinder[aiWK3_AlgnFwBw ]->Parent = pnWK3_AlgnFwBw2 ;
                  FrmCylinder[aiWK3_SttnUpDn ]->Parent = pnWK3_SttnUpDn2 ;
         break ;

         case 2 : FrmCylinder[aiPSB_AlgnFwBw   ]->Parent = pnPSB_AlgnFwBw    ;
                  FrmCylinder[aiPSB_IndxUpDn   ]->Parent = pnPSB_IndxUpDn    ;
                  FrmCylinder[aiPSB_FlprOpCl   ]->Parent = pnPSB_FlprOpCl    ;
                  FrmCylinder[aiPSB_GripUpDn   ]->Parent = pnPSB_GripUpDn    ;
                  FrmCylinder[aiPSB_PshrFwBw   ]->Parent = pnPSB_PshrFwBw    ;
                  FrmCylinder[aiPSB_PshrDnUp   ]->Parent = pnPSB_PshrDnUp    ;
         break ;

         case 3 : FrmCylinder[aiPSB_AlgnFwBw   ]->Parent = pnPSB_AlgnFwBw2   ;
                  FrmCylinder[aiPSB_IndxUpDn   ]->Parent = pnPSB_IndxUpDn2   ;
                  FrmCylinder[aiPSB_FlprOpCl   ]->Parent = pnPSB_FlprOpCl2   ;
                  FrmCylinder[aiPSB_GripUpDn   ]->Parent = pnPSB_GripUpDn2   ;
                  FrmCylinder[aiPSB_PnchDnUp   ]->Parent = pnPSB_PnchDnUp    ;
                  FrmCylinder[aiPSB_SortFwBw   ]->Parent = pnPSB_SortFwBw    ;
                  FrmCylinder[aiPSB_SrtStprFwBw]->Parent = pnPSB_SrtStprFwBw ;
         break ;

         case 5 :
         break ;

         case 6 :
         break ;
     }

}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btPSB_AllLiftUp1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------
void __fastcall TFormMotorPos::btWRK_feedingOn2Click(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    IO_SetY(iSel , !IO_GetY(iSel));
}
//---------------------------------------------------------------------------

double TFormMotorPos::GetMotrPosTable(EN_MOTR_ID _iMotr , int _iRow , int _iCol)
{
    int    r , c ;
    double dStartYPos ;
    double dStartXPos ;
    double dYPos      ;
    double dXPos      ;

    r = _iRow ;//OM.DevOptn.iRowInspCnt ? _iRow/OM.DevOptn.iRowInspCnt * OM.DevOptn.iRowInspCnt : _iRow ;
    c = _iCol ;//OM.DevOptn.iColInspCnt ? _iCol/OM.DevOptn.iColInspCnt * OM.DevOptn.iColInspCnt : _iCol ;

    dStartYPos = PM.GetValue(_iMotr , pvPSB_YTrmWorkStart ) ;
    dStartXPos = PM.GetValue(_iMotr , pvPSB_XTrmWorkStart ) ;

    if(_iMotr != miPSB_YTrm && _iMotr != miPSB_XTrm) return MT_GetCmdPos(_iMotr) ;

    dXPos = dStartXPos + c * OM.DevInfo.dColPitch ;
    dYPos = dStartYPos + r * OM.DevInfo.dRowPitch ;

    if(OM.DevInfo.dColGrGap && OM.DevInfo.iColGrCnt) {
        dXPos = dXPos + (c / OM.DevInfo.iColGrCnt) * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) ;
    }


    if(OM.DevInfo.dRowGrGap && OM.DevInfo.iRowGrCnt) {
        dYPos = dYPos + (r / OM.DevInfo.iRowGrCnt) * (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) ;
    }

    if     (_iMotr == miPSB_YTrm) return dYPos ;
    else if(_iMotr == miPSB_XTrm) return dXPos ;
}

void __fastcall TFormMotorPos::btPSB_TrmXnClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);

    if(!MotorCheck(miPSB_XTrm)) return ;
    if(!MotorCheck(miPSB_YTrm)) return ;

    iTrmManCol -- ;
    if(iTrmManCol < 0 ) iTrmManCol = 0 ;
    if(iTrmManRow < 0 ) iTrmManRow = 0 ;
    if(iTrmManCol >= OM.DevInfo.iColCnt) iTrmManCol = 0;
    if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = 0;
    MT_GoAbsMan(miPSB_XTrm , GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol)) ;
    if(!MT_CmprPos(miPSB_XTrm , GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol))) return ;
    MT_GoAbsMan(miPSB_YTrm , GetMotrPosTable(miPSB_YTrm , iTrmManRow , iTrmManCol)) ;

}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btPSB_TrmYnClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);

    if(!MotorCheck(miPSB_XTrm)) return ;
    if(!MotorCheck(miPSB_YTrm)) return ;

    iTrmManRow -- ;
    if(iTrmManCol < 0 ) iTrmManCol = 0 ;
    if(iTrmManRow < 0 ) iTrmManRow = 0 ;
//    if(iTrmManCol >= OM.DevInfo.iColCnt) iTrmManCol = OM.DevInfo.iColCnt - 1;
//    if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = OM.DevInfo.iRowCnt - 1;
    if(iTrmManCol >= OM.DevInfo.iColCnt) iTrmManCol = 0;
    if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = 0;
    MT_GoAbsMan(miPSB_XTrm , GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol)) ;
    if(!MT_CmprPos(miPSB_XTrm , GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol))) return ;
    MT_GoAbsMan(miPSB_YTrm , GetMotrPosTable(miPSB_YTrm , iTrmManRow , iTrmManCol)) ;

}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btPSB_TrmYpClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);

    if(!MotorCheck(miPSB_XTrm)) return ;
    if(!MotorCheck(miPSB_YTrm)) return ;

    iTrmManRow ++ ;
    if(iTrmManCol < 0 ) iTrmManCol = 0 ;
    if(iTrmManRow < 0 ) iTrmManRow = 0 ;
    if(iTrmManCol >= OM.DevInfo.iColCnt) iTrmManCol = 0;
    if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = 0;
    MT_GoAbsMan(miPSB_XTrm , GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol)) ;
    if(!MT_CmprPos(miPSB_XTrm , GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol))) return;
    MT_GoAbsMan(miPSB_YTrm , GetMotrPosTable(miPSB_YTrm , iTrmManRow , iTrmManCol)) ;

}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btPSB_TrmXpClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_PnchDnUp , ccBwd);

    if(!MotorCheck(miPSB_XTrm)) return ;
    if(!MotorCheck(miPSB_YTrm)) return ;

    iTrmManCol ++ ;
    if(iTrmManCol < 0 ) iTrmManCol = 0 ;
    if(iTrmManRow < 0 ) iTrmManRow = 0 ;
    if(iTrmManCol >= OM.DevInfo.iColCnt) iTrmManCol = 0;
    if(iTrmManRow >= OM.DevInfo.iRowCnt) iTrmManRow = 0;
    MT_GoAbsMan(miPSB_XTrm , GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol)) ;
    if(!MT_CmprPos(miPSB_XTrm , GetMotrPosTable(miPSB_XTrm , iTrmManRow , iTrmManCol))) return;
    MT_GoAbsMan(miPSB_YTrm , GetMotrPosTable(miPSB_YTrm , iTrmManRow , iTrmManCol)) ;

}
//---------------------------------------------------------------------------

void __fastcall TFormMotorPos::btTrmLeftUpPsClick(TObject *Sender)
{
    //0: lt  1: rt  2:lb  3:rb
    int iTag = ((TButton *)Sender) -> Tag ;

    double dSttXPos , dSttYPos = 0.0 ;
    double dMaxXPos , dMaxYPos = 0.0 ;


    if(!MotorCheck(miPSB_XTrm)) return ;
    if(!MotorCheck(miPSB_YTrm)) return ;

    dSttXPos =  PM.GetValue(miPSB_XTrm , pvPSB_XTrmWorkStart) ;
    dSttYPos =  PM.GetValue(miPSB_YTrm , pvPSB_YTrmWorkStart) ;
    dMaxXPos =  PM.GetValue(miPSB_XTrm , pvPSB_XTrmWorkStart) +
                (OM.DevInfo.iColCnt - 1) * OM.DevInfo.dColPitch ;
    dMaxYPos =  PM.GetValue(miPSB_YTrm , pvPSB_YTrmWorkStart) +
                (OM.DevInfo.iRowCnt - 1) * OM.DevInfo.dRowPitch ;

    int gc , gr ;
    int c, r ;

    if( OM.DevInfo.iColGrCnt) {
        gc = c / OM.DevInfo.iColGrCnt ;
        dMaxXPos = dMaxXPos + (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * gc ;
    }

    if( OM.DevInfo.iRowGrCnt) {
        gr = r / OM.DevInfo.iRowGrCnt ;
        dMaxYPos = dMaxYPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * gr;
    }

    if(!AT_Complete(aiPSB_PnchDnUp    , ccBwd)) {FM_MsgOk("Error","Punching Tip is Fwd!!");  return ; } // 넌또 왜 여기또ㅓ 잇냔.ㄴㅇ러너ㅣdd

    if (iTag == 0 ) { //lt
        MT_GoAbsMan(miPSB_XTrm, dSttXPos);
        if(!MT_CmprPos(miPSB_XTrm , dSttXPos)) return;
        MT_GoAbsMan(miPSB_YTrm, dSttYPos);
        iTrmManCol = 0 ;
        iTrmManRow = 0 ;
    }
    else if (iTag == 1) { //rt
        MT_GoAbsMan(miPSB_XTrm, dMaxXPos ) ;
        if(!MT_CmprPos(miPSB_XTrm , dMaxXPos)) return;
        MT_GoAbsMan(miPSB_YTrm, dMaxYPos ) ;
        iTrmManCol = OM.DevInfo.iColCnt - 1 ;
        iTrmManRow = 0 ;
    }
    else if (iTag == 2) { //lb
        MT_GoAbsMan(miPSB_XTrm, dSttXPos );
        if(!MT_CmprPos(miPSB_XTrm , dSttXPos)) return;
        MT_GoAbsMan(miPSB_YTrm, dMaxYPos );
        iTrmManCol = 0 ;
        iTrmManRow = OM.DevInfo.iRowCnt - 1 ;
    }

    else if (iTag == 3) { //rb
        MT_GoAbsMan(miPSB_XTrm, dMaxXPos );
        if(!MT_CmprPos(miPSB_XTrm , dMaxXPos)) return;
        MT_GoAbsMan(miPSB_YTrm, dMaxYPos );
        iTrmManCol = OM.DevInfo.iColCnt - 1 ;
        iTrmManRow = OM.DevInfo.iRowCnt - 1 ;
    }
}
//---------------------------------------------------------------------------




      

