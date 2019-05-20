//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
#include "FormProbe.h"
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff1.h"
#include "PreBuff2.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "SortingTool.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "ProbeComUnit.h"
#include "RS232Man.h"
#include "UserINI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{

    PM.SetWindow(pnLDR_YCmpPos , miLDR_YCmp);
    PM.SetWindow(pnLDR_ZCmpPos , miLDR_ZCmp);
    PM.SetWindow(pnPR1_XIdxPos , miPR1_XCmp);
    PM.SetWindow(pnPR2_XIdxPos , miPR2_XCmp);
    PM.SetWindow(pnPSB_XIdxPos , miPSB_XCmp);
    PM.SetWindow(pnWRK_XIdxPos , miWRK_XCmp);
    PM.SetWindow(pnWRK_ZTopPos , miWRK_ZTop);
    PM.SetWindow(pnWRK_ZBtmPos , miWRK_ZBtm);
    PM.SetWindow(pnWRK_YBtmPos , miWRK_YBtm);
    PM.SetWindow(pnSTL_YTopPos , miSTL_YTop);
    PM.SetWindow(pnRJT_XBtmPos , miREJ_XBtm);
    PM.SetWindow(pnULD_ZCmpPos , miULD_ZCmp);
    PM.SetWindow(pnULD_YCmpPos , miULD_YCmp);
    PM.SetWindow(pnWRK_XBtmPos , miWRK_XBtm);
//    PM.SetWindow(pnLd2Z,miLD2_Z);

  //  pcDeviceSet -> ActivePageIndex = 0 ;
    btLDR_Cmp       -> Tag = (int)aiLDR_Cmp      ;
    btLDR_Psh       -> Tag = (int)aiLDR_Psh      ;

    btLDR_In1       -> Tag = (int)yLDR_MgzInAC   ;
    btLDR_Out1      -> Tag = (int)yLDR_MgzOutAC  ;
    btLDR_In2       -> Tag = (int)yLDR_MgzInAC   ;
    btLDR_Out2      -> Tag = (int)yLDR_MgzOutAC  ;

    btPR1_Cmp       -> Tag = (int)aiPR1_Cmp      ;
    btPR2_Cmp       -> Tag = (int)aiPR2_Cmp      ;
    //btPRB_IdCk      -> Tag = (int)aiPRB_IdChk    ;
    btWRK_Cmp       -> Tag = (int)aiWRK_Cmp      ;
    btPSB_Cmp       -> Tag = (int)aiPSB_Cmp      ;
    btPSB_Psh       -> Tag = (int)aiPSB_Psh      ;

    btPRB_In1       -> Tag = (int)yPRB_FeedingAC ;
    btPRB_In2       -> Tag = (int)yPRB_FeedingAC ;

    btPR1_Cmp2      -> Tag = (int)aiPR1_Cmp      ;
    btPR2_Cmp2      -> Tag = (int)aiPR2_Cmp      ;
    btWRK_Cmp2      -> Tag = (int)aiWRK_Cmp      ;
    btPSB_Cmp2      -> Tag = (int)aiPSB_Cmp      ;
    btWRK_Hold      -> Tag = (int)aiWRK_Hold     ;

    btSTL_TopPch    -> Tag = (int)aiSTL_Top      ;
    btSTL_BtmDie    -> Tag = (int)aiSTL_Btm      ;
    btPSB_Cmp3      -> Tag = (int)aiPSB_Cmp      ;
    btPSB_Psh2      -> Tag = (int)aiPSB_Psh      ;
    btPSB_Out       -> Tag = (int)yPSB_FeedingAC ;
    btSTL_Eje       -> Tag = (int)ySTL_Ejecter   ;

    btPSB_Out2      -> Tag = (int)yPSB_FeedingAC ;
    btSTL_Eje2      -> Tag = (int)ySTL_Ejecter   ;

    btPSB_Rolr      -> Tag = (int)aiPSB_Rolr     ;
    btULD_Cmp       -> Tag = (int)aiULD_Cmp      ;
    btULD_Out1      -> Tag = (int)yULD_MgzOutAC  ;
    btULD_In1       -> Tag = (int)yULD_MgzInAC   ;

    btULD_Out2      -> Tag = (int)yULD_MgzOutAC  ;
    btULD_In2       -> Tag = (int)yULD_MgzInAC   ;

    btToolChange    -> Tag = (int)mcWRK_ToolChng ;
    btWRKNextMove   -> Tag = (int)mcWRK_NextMove ;
    btPSBNextMove   -> Tag = (int)mcPSB_NextMove ;

    btSTL_YPchFw    -> Tag = (int)mcSTL_YPchUp1Pitch ;
    btSTL_YPchBw    -> Tag = (int)mcSTL_YPchDn1Pitch ;

    btSTL_YDevFw    -> Tag = (int)mcSTL_YDevUp1Pitch ;
    btSTL_YDevBw    -> Tag = (int)mcSTL_YDevDn1Pitch ;


    //Ver 1.0.2.3
    WRK.ArraySSI.SetParent    (pnSSI);
    WRK.ArraySSI.SetConfig    (false , "AraySSI");
    WRK.ArraySSI.ClearDispItem();

//    WRK.ArraySSI.SetDispItem(csEmpty  ,"Empty "     ) ;
//    WRK.ArraySSI.SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    WRK.ArraySSI.SetDispItem(csWork   ,"Insp "      ) ;
    WRK.ArraySSI.SetDispItem(csNone   ,"None "      ) ;


    //Motor Window Set to Panel
    MtWindowSet();
    AtWindowSet();

    ShowSet();

}


void __fastcall TFrmDeviceSet::MtWindowSet()
{

    for(int i=0 ; i<MAX_MOTR ; i++) {
        FraMotor  [i] = new TFraMotr();
        FraMotor  [i] ->SetIdType(i ,MT_GetDirType(i));
        FraMotor  [i] ->Show();
        delete FraMotr;
    }

}


void __fastcall TFrmDeviceSet::AtWindowSet()
{
    //Cylinder Window Set to Panel
    for(int i=0 ; i<MAX_ACTR ; i++) {
        FraCylinder[i] = new TFraCyl();
        FraCylinder[i] ->SetIdType(i , AT_GetDirType(i));
        FraCylinder[i] ->Show();
        delete FraCyl;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
                                                                        
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;                  

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    if(DM.ARAY[riLDR].GetMaxRow() != OM.DevInfo.iCsSlCnt                                                       ) DM.ARAY[riLDR].SetMaxColRow(1                      , OM.DevInfo.iCsSlCnt);
    if(DM.ARAY[riULD].GetMaxRow() != OM.DevInfo.iCsSlCnt                                                       ) DM.ARAY[riULD].SetMaxColRow(1                      , OM.DevInfo.iCsSlCnt);
    if(DM.ARAY[riPR1].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riPR1].GetMaxCol() != OM.DevInfo.iColCnt    ) DM.ARAY[riPR1].SetMaxColRow(OM.DevInfo.iColCnt     , OM.DevInfo.iRowCnt );
    if(DM.ARAY[riPR2].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riPR2].GetMaxCol() != OM.DevInfo.iColCnt    ) DM.ARAY[riPR2].SetMaxColRow(OM.DevInfo.iColCnt     , OM.DevInfo.iRowCnt );
    if(DM.ARAY[riPSB].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riPSB].GetMaxCol() != OM.DevInfo.iColCnt    ) DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt     , OM.DevInfo.iRowCnt );
    if(DM.ARAY[riWRK].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riWRK].GetMaxCol() != OM.DevInfo.iColCnt    ) DM.ARAY[riWRK].SetMaxColRow(OM.DevInfo.iColCnt     , OM.DevInfo.iRowCnt );
    //if(DM.ARAY[riTmp].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riTmp].GetMaxCol() != OM.DevOptn.iContColCnt) DM.ARAY[riTmp].SetMaxColRow(OM.DevOptn.iContColCnt , OM.DevInfo.iRowCnt );
    if(DM.ARAY[riLDR].GetMaxRow() != OM.DevInfo.iCsSlCnt                                                       ) DM.ARAY[riULD].SetMaxColRow(1                      , OM.DevInfo.iCsSlCnt);
    if(DM.ARAY[riOPT].GetMaxRow() != OM.DevInfo.iRowCnt || DM.ARAY[riPR1].GetMaxCol() != OM.DevInfo.iColCnt    ) DM.ARAY[riOPT].SetMaxColRow(OM.DevInfo.iColCnt     , OM.DevInfo.iRowCnt );
    //Ver 1.0.2.3
    //로더 SetMaxColRow 뒤쪽에 있어야 함
    if(WRK.ArraySSI.GetMaxRow() != OM.DevInfo.iCsSlCnt ) WRK.ArraySSI.SetMaxColRow(1 , OM.DevInfo.iCsSlCnt);
    WRK.ArrayLoad(false);

    //Option으로 검사 설정 바꿀수 있도록 수정함 JS

   // DM.ARAY[riLDR].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
   // DM.ARAY[riPR1].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
   // DM.ARAY[riPR2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
   // DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
   // DM.ARAY[riWRK].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
   // DM.ARAY[riULD].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    //DM.ARAY[riSTL].SetMaxColRow(1                 ,1                  );

    //DM.ARAY[riPR2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    //DM.ARAY[riWRP].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    //DM.ARAY[riPS2].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );

    /*
    if(OM.DevOptn.bLtHeaterNum2) IO_SetY(yETC_Heater1_2,true );
    else                         IO_SetY(yETC_Heater1_2,false);
    if(OM.DevOptn.bLtHeaterNum3) IO_SetY(yETC_Heater1_3,true );
    else                         IO_SetY(yETC_Heater1_3,false);
    if(OM.DevOptn.bRtHeaterNum2) IO_SetY(yETC_Heater2_2,true );
    else                         IO_SetY(yETC_Heater2_2,false);
    if(OM.DevOptn.bRtHeaterNum3) IO_SetY(yETC_Heater2_3,true );
    else                         IO_SetY(yETC_Heater2_3,false);
    광폭에서 협폭 조절시 히터 센서 끄는 용도인데 정부과제에서 빠짐. JS
    */
    FrmProbe -> Init() ;

    PCM.SetDaegyeom();
    //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //대겸 Eng 프로브 사용시.
    //else                             FrmMain -> tmMainSet -> Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;

    //Ver 1.0.2.3
    WRK.ArraySSI.UpdateAray();


    bool bFoward;
    AnsiString sMsg;

    btSaveDevice -> Enabled = !SEQ._bRun;

    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 :
        break;

        case 1 : btLDR_In1        ->Font->Color   = IO_GetY(yLDR_MgzInAC )           ? clLime    : clBlack  ;
                 btLDR_In1        ->Caption       = IO_GetY(yLDR_MgzInAC )           ? "ON"      : "OFF"    ;

                 btLDR_In2        ->Font->Color   = IO_GetY(yLDR_MgzInAC )           ? clLime    : clBlack  ;
                 btLDR_In2        ->Caption       = IO_GetY(yLDR_MgzInAC )           ? "ON"      : "OFF"    ;

                 btLDR_Out1       ->Font->Color   = IO_GetY(yLDR_MgzOutAC)           ? clLime    : clBlack  ;
                 btLDR_Out1       ->Caption       = IO_GetY(yLDR_MgzOutAC)           ? "ON"      : "OFF"    ;

                 btLDR_Out2       ->Font->Color   = IO_GetY(yLDR_MgzOutAC)           ? clLime    : clBlack  ;
                 btLDR_Out2       ->Caption       = IO_GetY(yLDR_MgzOutAC)           ? "ON"      : "OFF"    ;

                 btLDR_Psh        ->Font->Color   = AT_Complete(aiLDR_Psh   , ccFwd) ? clLime    : clBlack  ;
                 btLDR_Psh        ->Caption       = AT_Complete(aiLDR_Psh   , ccFwd) ? "FWD"     : "BWD"    ;

                 btLDR_Cmp        ->Font->Color   = AT_Complete(aiLDR_Cmp   , ccFwd) ? clLime    : clBlack  ;
                 btLDR_Cmp        ->Caption       = AT_Complete(aiLDR_Cmp   , ccFwd) ? "UnClamp" : "Clamp";
        break;

        case 2 : btPRB_In1        ->Font->Color   = IO_GetY(yPRB_FeedingAC)          ? clLime  : clBlack  ;
                 btPRB_In1        ->Caption       = IO_GetY(yPRB_FeedingAC)          ? "ON"    : "OFF"    ;

                 btPRB_In2        ->Font->Color   = IO_GetY(yPRB_FeedingAC)          ? clLime  : clBlack  ;
                 btPRB_In2        ->Caption       = IO_GetY(yPRB_FeedingAC)          ? "ON"    : "OFF"    ;

                 btPR1_Cmp        ->Font->Color   = AT_Complete(aiPR1_Cmp   , ccFwd) ? clLime  : clBlack  ;
                 btPR1_Cmp        ->Caption       = AT_Complete(aiPR1_Cmp   , ccFwd) ? "Clamp" : "Unclamp";

                 btPR2_Cmp        ->Font->Color   = AT_Complete(aiPR2_Cmp   , ccFwd) ? clLime  : clBlack  ;
                 btPR2_Cmp        ->Caption       = AT_Complete(aiPR2_Cmp   , ccFwd) ? "Clamp" : "Unclamp";

                 //btPRB_IdCk       ->Font->Color   = AT_Complete(aiPRB_IdChk , ccFwd) ? clLime : clBlack   ;
                 //btPRB_IdCk       ->Caption       = AT_Complete(aiPRB_IdChk , ccFwd) ? "Fwd"  : "Bwd"     ;

                 btWRK_Cmp        ->Font->Color   = AT_Complete(aiWRK_Cmp   , ccFwd) ? clLime  : clBlack  ;
                 btWRK_Cmp        ->Caption       = AT_Complete(aiWRK_Cmp   , ccFwd) ? "Clamp" : "Unclamp";

                 btPSB_Cmp        ->Font->Color   = AT_Complete(aiPSB_Cmp   , ccFwd) ? clLime  : clBlack  ;
                 btPSB_Cmp        ->Caption       = AT_Complete(aiPSB_Cmp   , ccFwd) ? "Clamp" : "Unclamp";

                 btPSB_Psh        ->Font->Color   = AT_Complete(aiPSB_Psh   , ccFwd) ? clLime  : clBlack  ;
                 btPSB_Psh        ->Caption       = AT_Complete(aiPSB_Psh   , ccFwd) ? "Fwd"   : "Bwd"    ;
        break;

        case 3 : btPR1_Cmp2       ->Font->Color   = AT_Complete(aiPR1_Cmp   , ccFwd) ? clLime  : clBlack  ;
                 btPR1_Cmp2       ->Caption       = AT_Complete(aiPR1_Cmp   , ccFwd) ? "Clamp" : "Unclamp";

                 btPR2_Cmp2       ->Font->Color   = AT_Complete(aiPR2_Cmp   , ccFwd) ? clLime  : clBlack  ;
                 btPR2_Cmp2       ->Caption       = AT_Complete(aiPR2_Cmp   , ccFwd) ? "Clamp" : "Unclamp";

                 btWRK_Cmp2       ->Font->Color   = AT_Complete(aiWRK_Cmp   , ccFwd) ? clLime  : clBlack  ;
                 btWRK_Cmp2       ->Caption       = AT_Complete(aiWRK_Cmp   , ccFwd) ? "Clamp" : "Unclamp";

                 btPSB_Cmp2       ->Font->Color   = AT_Complete(aiPSB_Cmp   , ccFwd) ? clLime  : clBlack  ;
                 btPSB_Cmp2       ->Caption       = AT_Complete(aiPSB_Cmp   , ccFwd) ? "Clamp" : "Unclamp";

                 btWRK_Hold       ->Font->Color   = AT_Complete(aiWRK_Hold  , ccFwd) ? clLime  : clBlack  ;
                 btWRK_Hold       ->Caption       = AT_Complete(aiWRK_Hold  , ccFwd) ? "Fwd"   : "Bwd"    ;

        break;

        case 4 : btPSB_Out        ->Font->Color   = IO_GetY(yPSB_FeedingAC )         ? clLime  : clBlack  ;
                 btPSB_Out        ->Caption       = IO_GetY(yPSB_FeedingAC )         ? "ON"    : "OFF"    ;

                 btPSB_Out2       ->Font->Color   = IO_GetY(yPSB_FeedingAC )         ? clLime  : clBlack  ;
                 btPSB_Out2       ->Caption       = IO_GetY(yPSB_FeedingAC )         ? "ON"    : "OFF"    ;

                 btSTL_Eje        ->Font->Color   = IO_GetY(ySTL_Ejecter   )         ? clLime  : clBlack  ;
                 btSTL_Eje        ->Caption       = IO_GetY(ySTL_Ejecter   )         ? "ON"    : "OFF"    ;

                 btSTL_Eje2       ->Font->Color   = IO_GetY(ySTL_Ejecter   )         ? clLime  : clBlack  ;
                 btSTL_Eje2       ->Caption       = IO_GetY(ySTL_Ejecter   )         ? "ON"    : "OFF"    ;

                 btSTL_TopPch     ->Font->Color   = AT_Complete(aiSTL_Top   , ccFwd) ? clLime  : clBlack  ;
                 btSTL_TopPch     ->Caption       = AT_Complete(aiSTL_Top   , ccFwd) ? "Fwd"   : "Bwd"    ;

                 btSTL_BtmDie     ->Font->Color   = AT_Complete(aiSTL_Btm   , ccFwd) ? clLime  : clBlack  ;
                 btSTL_BtmDie     ->Caption       = AT_Complete(aiSTL_Btm   , ccFwd) ? "Fwd"   : "Bwd"    ;

                 btPSB_Cmp3       ->Font->Color   = AT_Complete(aiPSB_Cmp   , ccFwd) ? clLime  : clBlack  ;
                 btPSB_Cmp3       ->Caption       = AT_Complete(aiPSB_Cmp   , ccFwd) ? "Clamp" : "Unclamp";

                 btPSB_Psh2       ->Font->Color   = AT_Complete(aiPSB_Psh   , ccFwd) ? clLime  : clBlack  ;
                 btPSB_Psh2       ->Caption       = AT_Complete(aiPSB_Psh   , ccFwd) ? "Fwd"   : "Bwd"    ;
        break;

        case 5 : btPSB_Rolr       ->Font->Color   = IO_GetY(yPSB_RollerFw)           ? clLime  : clBlack  ;
                 btPSB_Rolr       ->Caption       = IO_GetY(yPSB_RollerFw)           ? "Fwd"   : "Bwd"    ;

                 btULD_In1        ->Font->Color   = IO_GetY(yULD_MgzInAC )           ? clLime  : clBlack  ;
                 btULD_In1        ->Caption       = IO_GetY(yULD_MgzInAC )           ? "ON"    : "OFF"    ;

                 btULD_In2        ->Font->Color   = IO_GetY(yULD_MgzInAC )           ? clLime  : clBlack  ;
                 btULD_In2        ->Caption       = IO_GetY(yULD_MgzInAC )           ? "ON"    : "OFF"    ;

                 btULD_Out1       ->Font->Color   = IO_GetY(yULD_MgzOutAC)           ? clLime  : clBlack  ;
                 btULD_Out1       ->Caption       = IO_GetY(yULD_MgzOutAC)           ? "ON"    : "OFF"    ;

                 btULD_Out2       ->Font->Color   = IO_GetY(yULD_MgzOutAC)           ? clLime  : clBlack  ;
                 btULD_Out2       ->Caption       = IO_GetY(yULD_MgzOutAC)           ? "ON"    : "OFF"    ;

                 btULD_Cmp        ->Font->Color   = AT_Complete(aiULD_Cmp ,ccFwd)    ? clLime  : clBlack  ;
                 btULD_Cmp        ->Caption       = AT_Complete(aiULD_Cmp ,ccBwd)    ? "Clamp" : "Unclamp";

        break;

        case 6 :
        break;
    }

    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormDestroy(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevInfo(bool bTo)
{
    if(bTo)
    {
        edColPitch       -> Text = OM.DevInfo.dColPitch    ;
        edRowPitch       -> Text = OM.DevInfo.dRowPitch    ;
        edColGroupCount  -> Text = OM.DevInfo.iColGrCnt    ;
        edRowGroupCount  -> Text = OM.DevInfo.iRowGrCnt    ;

        edColGroupGap    -> Text = OM.DevInfo.dColGrGap    ;
        edRowGroupGap    -> Text = OM.DevInfo.dRowGrGap    ;

        edColCount       -> Text = OM.DevInfo.iColCnt      ;
        edRowCount       -> Text = OM.DevInfo.iRowCnt      ;

        edCsSlotCnt      -> Text = OM.DevInfo.iCsSlCnt     ;
        edCsSlotPitch    -> Text = OM.DevInfo.dCsSlPitch   ;

        edRowSubGroupGap -> Text = OM.DevInfo.dRowSubGrGap ;
        edRowSubGrCount  -> Text = OM.DevInfo.iRowSubGrCnt ;
        cbSSI            -> Checked = OM.DevInfo.bSSI      ; //Ver 1.0.2.3
    }
    else
    {
        OM.DevInfo.dColPitch    = edColPitch      -> Text.ToDouble() ;
        OM.DevInfo.dRowPitch    = edRowPitch      -> Text.ToDouble() ;

        OM.DevInfo.iColGrCnt    = edColGroupCount -> Text.ToInt()    ;
        OM.DevInfo.iRowGrCnt    = edRowGroupCount -> Text.ToInt()    ;

        OM.DevInfo.dColGrGap    = edColGroupGap   -> Text.ToDouble() ;
        OM.DevInfo.dRowGrGap    = edRowGroupGap   -> Text.ToDouble() ;

        OM.DevInfo.iColCnt      = edColCount      -> Text.ToInt()    ;
        OM.DevInfo.iRowCnt      = edRowCount      -> Text.ToInt()    ;

        OM.DevInfo.iCsSlCnt     = edCsSlotCnt     -> Text.ToInt()    ;
        OM.DevInfo.dCsSlPitch   = edCsSlotPitch   -> Text.ToDouble() ;

        OM.DevInfo.dRowSubGrGap = edRowSubGroupGap -> Text.ToDouble();
        OM.DevInfo.iRowSubGrCnt = edRowSubGrCount  -> Text.ToInt()   ;

        OM.DevInfo.bSSI         = cbSSI            -> Checked        ;  //Ver 1.0.2.3
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
    if(bTo)
    {
        //Probe.

        ed1ChHigh            -> Text      = OM.DevOptn.d1ChHigh          ;
        ed1ChLow             -> Text      = OM.DevOptn.d1ChLow           ;
        ed2ChHigh            -> Text      = OM.DevOptn.d2ChHigh          ;
        ed2ChLow             -> Text      = OM.DevOptn.d2ChLow           ;
        edZennerHigh         -> Text      = OM.DevOptn.dZenerHigh        ;
        edZennerLow          -> Text      = OM.DevOptn.dZenerLow         ;
        edPinCnt             -> Text      = OM.DevOptn.iPinCnt           ;
        edLimInsFailCnt      -> Text      = OM.DevOptn.iLimInsFailCnt    ;
        edReInspCnt          -> Text      = OM.DevOptn.iReInspCnt        ;
        edFrtPrcFailCnt      -> Text      = OM.DevOptn.iFrtPrcFailCnt    ;
        edContactCnt         -> Text      = OM.DevOptn.iContColCnt       ;
        cbUse8ColTo4ColProbe -> Checked   = OM.DevOptn.bUse8ColTo4ColProbe;

        edPR1MoveCnt         -> Text      = OM.DevOptn.iPR1MoveCnt       ;
        edPR1OutOfs          -> Text      = OM.DevOptn.dPR1OutOfs        ;
        edPR2MoveCnt         -> Text      = OM.DevOptn.iPR2MoveCnt       ;
        edPR2OutOfs          -> Text      = OM.DevOptn.dPR2OutOfs        ;
        edWRKMoveCnt         -> Text      = OM.DevOptn.iWRKMoveCnt       ;
        edPrbSttDelay        -> Text      = OM.DevOptn.iPrbSttDelay      ;
        cbUseZenner          -> Checked   = OM.DevOptn.bUseZener         ;
        cb1Ch1P              -> ItemIndex = OM.DevOptn.i1Ch1P            ;
        cb1Ch1N              -> ItemIndex = OM.DevOptn.i1Ch1N            ;
        cb1Ch2P              -> ItemIndex = OM.DevOptn.i1Ch2P            ;
        cb1Ch2N              -> ItemIndex = OM.DevOptn.i1Ch2N            ;
        cb1Ch3P              -> ItemIndex = OM.DevOptn.i1Ch3P            ;
        cb1Ch3N              -> ItemIndex = OM.DevOptn.i1Ch3N            ;
        ed1ChLedCur          -> Text      = OM.DevOptn.i1ChLedCur        ;
        ed1ChZenCur          -> Text      = OM.DevOptn.i1ChZenCur        ;
        cb2Ch1P              -> ItemIndex = OM.DevOptn.i2Ch1P            ;
        cb2Ch1N              -> ItemIndex = OM.DevOptn.i2Ch1N            ;
        cb2Ch2P              -> ItemIndex = OM.DevOptn.i2Ch2P            ;
        cb2Ch2N              -> ItemIndex = OM.DevOptn.i2Ch2N            ;
        cb2Ch3P              -> ItemIndex = OM.DevOptn.i2Ch3P            ;
        cb2Ch3N              -> ItemIndex = OM.DevOptn.i2Ch3N            ;
        ed2ChLedCur          -> Text      = OM.DevOptn.i2ChLedCur        ;
        ed2ChZenCur          -> Text      = OM.DevOptn.i2ChZenCur        ;
        cbUseTwiceInsp       -> Checked   = OM.DevOptn.bUseTwiceInsp     ;
        cbSideLEDControl     -> Checked   = OM.DevOptn.bSideLEDControl   ;

        if(OM.DevOptn.bSideLEDControl){
            ed1ChDelay           -> Text      = OM.DevOptn.i1ChDelay        ;
            ed1ChLedTime         -> Text      = OM.DevOptn.i1ChLedTime      ;
            ed1ChZenTime         -> Text      = OM.DevOptn.i1ChZenTime      ;
            ed2ChDelay           -> Text      = OM.DevOptn.i2ChDelay        ;
            ed2ChLedTime         -> Text      = OM.DevOptn.i2ChLedTime      ;
            ed2ChZenTime         -> Text      = OM.DevOptn.i2ChZenTime      ;
        }
        else {
            ed1ChDelay           -> Text      = OM.DevOptn.i1ChDelay        ;
            ed1ChLedTime         -> Text      = OM.DevOptn.i1ChLedTime / 100;
            ed1ChZenTime         -> Text      = OM.DevOptn.i1ChZenTime / 100;
            ed2ChDelay           -> Text      = OM.DevOptn.i2ChDelay        ;
            ed2ChLedTime         -> Text      = OM.DevOptn.i2ChLedTime / 100;
            ed2ChZenTime         -> Text      = OM.DevOptn.i2ChZenTime / 100;
        }




        cbChRvsCrnt          -> Checked   = OM.DevOptn.bChRvsCrnt        ;

        //Daegyoem
        edDgChipCnt          -> Text      = OM.DevOptn.iDgChipCnt        ;
        edSwitchingDly       -> Text      = OM.DevOptn.iSwitchingDly     ;


        //Dev Option
        edPreRailOutVel      -> Text      = OM.DevOptn.dPreRailOutVel    ;
        edPostRailOutVel     -> Text      = OM.DevOptn.dPostRailOutVel   ;

        edLotEndMgzCnt       -> Text      = OM.DevOptn.iLotEndMgzCnt     ;
        edWaitDelay          -> Text      = OM.DevOptn.iWaitDelay        ;

        //cbLtHeaterNum2       -> Checked   = OM.DevOptn.bLtHeaterNum2     ;
        //cbLtHeaterNum3       -> Checked   = OM.DevOptn.bLtHeaterNum3     ;
        //cbRtHeaterNum2       -> Checked   = OM.DevOptn.bRtHeaterNum2     ;
        //cbRtHeaterNum3       -> Checked   = OM.DevOptn.bRtHeaterNum3     ;

        edREJPitch           -> Text      = OM.DevOptn.dREJPitch         ;

        edTrimPtchOfs        -> Text      = OM.DevOptn.dTrimPtchOfs      ;
        edWRKPtchOfs         -> Text      = OM.DevOptn.dWRKPtchOfs       ;
        edPSBPtchOfs         -> Text      = OM.DevOptn.dPSBPtchOfs       ;
        //edTrimMoveOfs        -> Text      = OM.DevOptn.dTrimMoveOfs      ;

        edPSBReGripOfs       -> Text      = OM.DevOptn.dPSBReGripOfs     ;
        edPSBReGripCol       -> Text      = OM.DevOptn.iPSBReGripCol     ;

        edTrimRjtDly         -> Text      = OM.DevOptn.iTrimRjtDly       ;
        cbIgnrVoidMotion     -> Checked   = OM.DevOptn.bIgnrVoidMotion   ;
        //edTrimSnrDly         -> Text      = OM.DevOptn.iTrimSnrDly       ;

        edContactPinCnt      -> Text      = OM.DevOptn.iContactPinCnt    ;
        edLimContactPinCnt   -> Text      = OM.DevOptn.iLimContactPinCnt ;
        edPSBPshDeley        -> Text      = OM.DevOptn.dPSBPshDeley      ;

        edGripOfs            -> Text      = OM.DevOptn.dGripOfs          ;

        edRejectMoveDeley    -> Text      = OM.DevOptn.iRejectMoveDeley  ;

    }
    else
    {
        // Probe

        OM.DevOptn.d1ChHigh          =  ed1ChHigh           -> Text.ToDouble()  ;
        OM.DevOptn.d1ChLow           =  ed1ChLow            -> Text.ToDouble()  ;
        OM.DevOptn.d2ChHigh          =  ed2ChHigh           -> Text.ToDouble()  ;
        OM.DevOptn.d2ChLow           =  ed2ChLow            -> Text.ToDouble()  ;
        OM.DevOptn.dZenerHigh        =  edZennerHigh        -> Text.ToDouble()  ;
        OM.DevOptn.iPrbSttDelay      =  edPrbSttDelay       -> Text.ToIntDef(0) ;
        OM.DevOptn.dZenerLow         =  edZennerLow         -> Text.ToDouble()  ;
        OM.DevOptn.iPinCnt           =  edPinCnt            -> Text.ToIntDef(0) ;
        OM.DevOptn.iLimInsFailCnt    =  edLimInsFailCnt     -> Text.ToIntDef(0) ;
        OM.DevOptn.iReInspCnt        =  edReInspCnt         -> Text.ToIntDef(0) ;
        OM.DevOptn.iFrtPrcFailCnt    =  edFrtPrcFailCnt     -> Text.ToIntDef(100);
        OM.DevOptn.bUse8ColTo4ColProbe = cbUse8ColTo4ColProbe -> Checked ;

        OM.DevOptn.iContColCnt       =  edContactCnt        -> Text.ToIntDef(0) ;
        OM.DevOptn.iPR1MoveCnt       =  edPR1MoveCnt        -> Text.ToIntDef(0) ;
        OM.DevOptn.dPR1OutOfs        =  edPR1OutOfs         -> Text.ToDouble( ) ;
        OM.DevOptn.iPR2MoveCnt       =  edPR2MoveCnt        -> Text.ToIntDef(0) ;
        OM.DevOptn.dPR2OutOfs        =  edPR2OutOfs         -> Text.ToDouble( ) ;
        OM.DevOptn.iWRKMoveCnt       =  edWRKMoveCnt        -> Text.ToIntDef(0) ;
        OM.DevOptn.bUseZener         =  cbUseZenner         -> Checked          ;
        OM.DevOptn.i1Ch1P            =  cb1Ch1P             -> ItemIndex        ;
        OM.DevOptn.i1Ch1N            =  cb1Ch1N             -> ItemIndex        ;
        OM.DevOptn.i1Ch2P            =  cb1Ch2P             -> ItemIndex        ;
        OM.DevOptn.i1Ch2N            =  cb1Ch2N             -> ItemIndex        ;
        OM.DevOptn.i1Ch3P            =  cb1Ch3P             -> ItemIndex        ;
        OM.DevOptn.i1Ch3N            =  cb1Ch3N             -> ItemIndex        ;
        OM.DevOptn.i1ChLedCur        =  ed1ChLedCur         -> Text.ToIntDef(0) ;
        OM.DevOptn.i1ChZenCur        =  ed1ChZenCur         -> Text.ToIntDef(0) ;
        OM.DevOptn.i2Ch1P            =  cb2Ch1P             -> ItemIndex        ;
        OM.DevOptn.i2Ch1N            =  cb2Ch1N             -> ItemIndex        ;
        OM.DevOptn.i2Ch2P            =  cb2Ch2P             -> ItemIndex        ;
        OM.DevOptn.i2Ch2N            =  cb2Ch2N             -> ItemIndex        ;
        OM.DevOptn.i2Ch3P            =  cb2Ch3P             -> ItemIndex        ;
        OM.DevOptn.i2Ch3N            =  cb2Ch3N             -> ItemIndex        ;
        OM.DevOptn.i2ChLedCur        =  ed2ChLedCur         -> Text.ToIntDef(0) ;
        OM.DevOptn.i2ChZenCur        =  ed2ChZenCur         -> Text.ToIntDef(0) ;
        OM.DevOptn.bSideLEDControl   =  cbSideLEDControl    -> Checked          ;

        if(OM.DevOptn.bSideLEDControl){
            OM.DevOptn.i1ChDelay         =  ed1ChDelay          -> Text.ToIntDef(0) ;
            OM.DevOptn.i1ChLedTime       =  ed1ChLedTime        -> Text.ToIntDef(0) ;
            OM.DevOptn.i1ChZenTime       =  ed1ChZenTime        -> Text.ToIntDef(0) ;
            OM.DevOptn.i2ChDelay         =  ed2ChDelay          -> Text.ToIntDef(0) ;
            OM.DevOptn.i2ChLedTime       =  ed2ChLedTime        -> Text.ToIntDef(0) ;
            OM.DevOptn.i2ChZenTime       =  ed2ChZenTime        -> Text.ToIntDef(0) ;
        }
        else {
            OM.DevOptn.i1ChDelay         =  ed1ChDelay          -> Text.ToIntDef(0) ;
            OM.DevOptn.i1ChLedTime       =  ed1ChLedTime        -> Text.ToIntDef(0) * 100;
            OM.DevOptn.i1ChZenTime       =  ed1ChZenTime        -> Text.ToIntDef(0) * 100;
            OM.DevOptn.i2ChDelay         =  ed2ChDelay          -> Text.ToIntDef(0) ;
            OM.DevOptn.i2ChLedTime       =  ed2ChLedTime        -> Text.ToIntDef(0) * 100;
            OM.DevOptn.i2ChZenTime       =  ed2ChZenTime        -> Text.ToIntDef(0) * 100;
        }
        OM.DevOptn.bChRvsCrnt        =  cbChRvsCrnt         -> Checked          ;
        OM.DevOptn.bUseTwiceInsp     =  cbUseTwiceInsp      -> Checked          ;

        //Daegyoem
        OM.DevOptn.iDgChipCnt        =  edDgChipCnt         -> Text.ToIntDef(1) ;
        OM.DevOptn.iSwitchingDly     =  edSwitchingDly      -> Text.ToIntDef(1) ;

        //Dev Option
        OM.DevOptn.dPreRailOutVel    =  edPreRailOutVel     -> Text.ToDouble( ) ;
        OM.DevOptn.dPostRailOutVel   =  edPostRailOutVel    -> Text.ToDouble( ) ;

        //OM.DevOptn.bLtHeaterNum2     =  cbLtHeaterNum2      -> Checked          ;
        //OM.DevOptn.bLtHeaterNum3     =  cbLtHeaterNum3      -> Checked          ;
        //OM.DevOptn.bRtHeaterNum2     =  cbRtHeaterNum2      -> Checked          ;
        //OM.DevOptn.bRtHeaterNum3     =  cbRtHeaterNum3      -> Checked          ;

        OM.DevOptn.dREJPitch         =  edREJPitch          -> Text.ToDouble( ) ;

        if(OM.DevOptn.iLotEndMgzCnt != edLotEndMgzCnt       -> Text.ToIntDef(0)){
            LDR._iMgzCnt             = edLotEndMgzCnt       -> Text.ToIntDef(0);
        }
        OM.DevOptn.iLotEndMgzCnt     =  edLotEndMgzCnt      -> Text.ToIntDef(0) ;
        OM.DevOptn.iWaitDelay        =  edWaitDelay         -> Text.ToIntDef(0) ;

        OM.DevOptn.dPSBReGripOfs     =  edPSBReGripOfs      -> Text.ToDouble( ) ;
        OM.DevOptn.iPSBReGripCol     =  edPSBReGripCol      -> Text.ToIntDef(0) ;

        OM.DevOptn.dTrimPtchOfs      =  edTrimPtchOfs       -> Text.ToDouble( ) ;
        OM.DevOptn.dWRKPtchOfs       =  edWRKPtchOfs        -> Text.ToDouble( ) ;
        OM.DevOptn.dPSBPtchOfs       =  edPSBPtchOfs        -> Text.ToDouble( ) ;
        //OM.DevOptn.dTrimMoveOfs      =  edTrimMoveOfs       -> Text.ToDouble( ) ;

        OM.DevOptn.iTrimRjtDly       =  edTrimRjtDly        -> Text.ToIntDef(0) ;
        OM.DevOptn.bIgnrVoidMotion   =  cbIgnrVoidMotion    -> Checked          ;          
        //OM.DevOptn.iTrimSnrDly       =  edTrimSnrDly        -> Text.ToIntDef(0) ;

        OM.DevOptn.iContactPinCnt    =  edContactPinCnt     -> Text.ToIntDef(0) ;
        OM.DevOptn.iLimContactPinCnt =  edLimContactPinCnt  -> Text.ToIntDef(0) ;
        OM.DevOptn.dPSBPshDeley      =  edPSBPshDeley       -> Text.ToDouble( ) ;
        OM.DevOptn.dGripOfs          =  edGripOfs           -> Text.ToDouble( ) ;

        OM.DevOptn.iRejectMoveDeley  =  edRejectMoveDeley   -> Text.ToIntDef(0) ;

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);

    //Ver 1.0.2.3
    WRK.ArrayLoad(true);

    pcDeviceSet -> ActivePageIndex = 0 ;


    switch ( pcDeviceSet -> ActivePageIndex ) {
        default: rgUnitUser = rgLDRJogUnit ;
                 edUnitUser = edLDRJogUnit ; break ;
        case 0 : rgUnitUser = rgLDRJogUnit ;
                 edUnitUser = edLDRJogUnit ; break ;
        case 1 : rgUnitUser = rgLDRJogUnit ;
                 edUnitUser = edLDRJogUnit ; break ;
        case 2 : rgUnitUser = rgRALJogUnit ;
                 edUnitUser = edRALJogUnit ; break ;
        case 3 : rgUnitUser = rgWRKJogUnit ;
                 edUnitUser = edWRKJogUnit ; break ;
        case 4 : rgUnitUser = rgSTLJogUnit ;
                 edUnitUser = edSTLJogUnit ; break ;
        case 5 : rgUnitUser = rgULDJogUnit ;
                 edUnitUser = edULDJogUnit ; break ;
        case 6 : rgUnitUser = rgULDJogUnit ;
                 edUnitUser = edULDJogUnit ; break ;
     }

    if(rgUnitUser->ItemIndex == 0)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5)
        for(int i=0;i<MAX_MOTR;i++)
            FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));

    ShowSet();

    if(!OM.MstOptn.bUsedXAxis){
        gbWRK_Hold  -> Visible = false;
        gbWRK_Hold2 -> Visible = false;
    }
    else{
        gbWRK_Hold  -> Visible = true ;
        gbWRK_Hold2 -> Visible = true ;
    }



    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 :

        break;

        case 1 : FraMotor[miLDR_YCmp]->Parent = pnLDR_YCmp  ;
                 FraMotor[miLDR_ZCmp]->Parent = pnLDR_ZCmp  ;

                 FraCylinder[aiLDR_Cmp]->Parent = pnLDR_Cmp ;
                 FraCylinder[aiLDR_Psh]->Parent = pnLDR_Psh ;

                 PM.SetParent(pnLDR_YCmpPos , miLDR_YCmp);
                 PM.SetParent(pnLDR_ZCmpPos , miLDR_ZCmp);
        break;

        case 2 : FraMotor[miPR1_XCmp]->Parent = pnPR1_XIdx ;
                 FraMotor[miPR2_XCmp]->Parent = pnPR2_XIdx ;
                 FraMotor[miWRK_XCmp]->Parent = pnWRK_XIdx ;
                 FraMotor[miPSB_XCmp]->Parent = pnPSB_XIdx ;

                 FraCylinder[aiPR1_Cmp  ]->Parent = pnPR1_Cmp  ;
                 FraCylinder[aiPR2_Cmp  ]->Parent = pnPR2_Cmp  ;
                 //FraCylinder[aiPRB_IdChk]->Parent = pnPRB_IdCk ;
                 FraCylinder[aiWRK_Cmp  ]->Parent = pnWRK_Cmp  ;
                 FraCylinder[aiPSB_Cmp  ]->Parent = pnPSB_Cmp  ;
                 FraCylinder[aiPSB_Psh  ]->Parent = pnPSB_Psh  ;

                 PM.SetParent(pnPR1_XIdxPos , miPR1_XCmp);
                 PM.SetParent(pnPR2_XIdxPos , miPR2_XCmp);
                 PM.SetParent(pnWRK_XIdxPos , miWRK_XCmp);
                 PM.SetParent(pnPSB_XIdxPos , miPSB_XCmp);
        break;

        case 3 : if(OM.MstOptn.bUsedXAxis){
                     GroupBox70 -> Visible = true;
                     GroupBox71 -> Visible = true;
                 }
                 else{
                     GroupBox70 -> Visible = false;
                     GroupBox71 -> Visible = false;
                 }
                 FraMotor[miWRK_ZTop]->Parent = pnWRK_ZTop ;
                 FraMotor[miWRK_ZBtm]->Parent = pnWRK_ZBtm ;
                 FraMotor[miWRK_YBtm]->Parent = pnWRK_YBtm ;
                 FraMotor[miWRK_XBtm]->Parent = pnWRK_XBtm ;

                 FraCylinder[aiPR1_Cmp ]->Parent = pnPR1_Cmp2 ;
                 FraCylinder[aiPR2_Cmp ]->Parent = pnPR2_Cmp2 ;
                 FraCylinder[aiWRK_Cmp ]->Parent = pnWRK_Cmp2 ;
                 FraCylinder[aiPSB_Cmp ]->Parent = pnPSB_Cmp2 ;
                 FraCylinder[aiWRK_Hold]->Parent = pnWRK_Hold;

                 PM.SetParent(pnWRK_ZTopPos , miWRK_ZTop);
                 PM.SetParent(pnWRK_ZBtmPos , miWRK_ZBtm);
                 PM.SetParent(pnWRK_YBtmPos , miWRK_YBtm);
                 PM.SetParent(pnWRK_XBtmPos , miWRK_XBtm);
        break;

        case 4 : FraMotor[miSTL_YTop]->Parent = pnSTL_YTop ;
                 FraMotor[miREJ_XBtm]->Parent = pnRJT_XBtm ;

                 FraCylinder[aiSTL_Top]->Parent = pnSTL_TopPch ;
                 FraCylinder[aiSTL_Btm]->Parent = pnSTL_BtmDie ;
                 FraCylinder[aiPSB_Cmp]->Parent = pnPSB_Cmp3   ;
                 FraCylinder[aiPSB_Psh]->Parent = pnPSB_Psh2   ;

                 PM.SetParent(pnSTL_YTopPos , miSTL_YTop);
                 PM.SetParent(pnRJT_XBtmPos , miREJ_XBtm);
        break;

        case 5 : FraMotor[miULD_YCmp]->Parent = pnULD_YCmp ;
                 FraMotor[miULD_ZCmp]->Parent = pnULD_ZCmp ;


                 FraCylinder[aiPSB_Rolr]->Parent = pnPSB_Rolr ;
                 FraCylinder[aiULD_Cmp ]->Parent = pnULD_Cmp  ;

                 PM.SetParent(pnULD_YCmpPos , miULD_YCmp);
                 PM.SetParent(pnULD_ZCmpPos , miULD_ZCmp);


        break;
    }

    switch ( pcDeviceSet -> ActivePageIndex ) {
         case 0 :                             break ;
         case 1 : rgUnitUser = rgLDRJogUnit ;
                  edUnitUser = edLDRJogUnit ; break ;
         case 2 : rgUnitUser = rgRALJogUnit ;
                  edUnitUser = edRALJogUnit ; break ;
         case 3 : rgUnitUser = rgWRKJogUnit ;
                  edUnitUser = edWRKJogUnit ; break ;
         case 4 : rgUnitUser = rgSTLJogUnit ;
                  edUnitUser = edSTLJogUnit ; break ;
         case 5 : rgUnitUser = rgULDJogUnit ;
                  edUnitUser = edULDJogUnit ; break ;
         case 6 : rgUnitUser = rgULDJogUnit ;
                  edUnitUser = edULDJogUnit ; break ;
     }

    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btStartTestClick(TObject *Sender)
{
//    g_bPb1Rslt = true ;
//   g_bPb2Rslt = true ;
//    tmProbeTest -> Enabled = true ;
    AnsiString sTemp , sMsg;
    sMsg = "TEST,";

    sTemp.sprintf("%02d,",edPinNumber -> Text.ToIntDef(0));

    sMsg += sTemp ;

    sTemp.sprintf("%04d#",edProbeTime -> Text.ToIntDef(0));

    sMsg += sTemp ;

    RS232C_Pb1ch->SendData(sMsg.Length(),sMsg.c_str());
    RS232C_Pb2ch->SendData(sMsg.Length(),sMsg.c_str());

    
//    m_tmProbeTest.Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::mmProbeTestDblClick(TObject *Sender)
{
    mmProbeTest->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btToolChangeClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btResetRptClick(TObject *Sender)
{
    //SEQ.Reset();
    CWorkZone::SStat Stat = WRK.GetStat(); Stat.bRepeat = false ; WRK.SetStat(Stat);
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btCntctRptClick(TObject *Sender)
{
    if(DM.ARAY[riWRK].CheckAllStat(csNone)) {FM_MsgOk("ERR","No Strip in the WorkZone"); return ;}

    MM.SetManCycle(mcWRK_RptCntct) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btLDR_InClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    bool bState =  IO_GetY(iSel);
    IO_SetY(iSel , !bState);
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btInspStartClick(TObject *Sender)
{
    int iSel = ((TButton *)Sender) -> Tag ;
//    AnsiString sMsg, sTemp;

    //SET,12,0400,0100,15,T#
    AnsiString sMsg  = "" ;
    AnsiString sTemp ;
    int i1ChLedCur, i2ChLedCur , i1ChZenCur, i2ChZenCur ;
    i1ChLedCur = (OM.DevOptn.i1ChLedCur * 1000 ) / 50 ;
    i1ChZenCur = (OM.DevOptn.i1ChZenCur * 1000 ) / 50 ;

    i2ChLedCur = (OM.DevOptn.i2ChLedCur * 1000 ) / 50 ;
    i2ChZenCur = (OM.DevOptn.i2ChZenCur * 1000 ) / 50 ;

    switch(iSel) {
        case 0 : RS232C_Pb1ch->Open(0);
                 RS232C_Pb2ch->Open(1);
                 break;
        case 1 : RS232C_Pb1ch->Close();
                 RS232C_Pb2ch->Close();
                 break;
        case 2 : PCM.SetDaegyeom();
                 //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();
                 //else                             FrmMain-> tmMainSet -> Enabled = true ;
                 break;
        case 3 : PCM.Start(1);
                 PCM.Start(2);
                 break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btStopTestClick(TObject *Sender)
{
    AnsiString sTemp , sMsg;
    sMsg = "STOP#";

    RS232C_Pb1ch->SendData(sMsg.Length(),sMsg.c_str());
    RS232C_Pb2ch->SendData(sMsg.Length(),sMsg.c_str());

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btPSBAutoCALClick(TObject *Sender)
{
    double dReGrabPs ;
    int    iReGrabCol;

    iReGrabCol = edPSBReGripCol -> Text.ToIntDef(0) ;
    dReGrabPs  = iReGrabCol * (OM.DevInfo.dColPitch + OM.DevOptn.dTrimPtchOfs)  ;

    edPSBReGripOfs -> Text = dReGrabPs ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::brContactCntResetClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Do you really want to Reset?") != mrYes) return ;
    edContactPinCnt -> Text = 0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btSTL_YPchFwClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::rgLDRJogUnitClick(TObject *Sender)
{
    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::ShowSet()
{
    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 :

        break;

        case 1 : FraMotor[miLDR_YCmp]->Parent = pnLDR_YCmp  ;
                 FraMotor[miLDR_ZCmp]->Parent = pnLDR_ZCmp  ;

                 FraCylinder[aiLDR_Cmp]->Parent = pnLDR_Cmp ;
                 FraCylinder[aiLDR_Psh]->Parent = pnLDR_Psh ;

                 PM.SetParent(pnLDR_YCmpPos , miLDR_YCmp);
                 PM.SetParent(pnLDR_ZCmpPos , miLDR_ZCmp);

        break;

        case 2 : FraMotor[miPR1_XCmp]->Parent = pnPR1_XIdx ;
                 FraMotor[miPR2_XCmp]->Parent = pnPR2_XIdx ;
                 FraMotor[miWRK_XCmp]->Parent = pnWRK_XIdx ;
                 FraMotor[miPSB_XCmp]->Parent = pnPSB_XIdx ;

                 FraCylinder[aiPR1_Cmp  ]->Parent = pnPR1_Cmp  ;
                 FraCylinder[aiPR2_Cmp  ]->Parent = pnPR2_Cmp  ;
                 //FraCylinder[aiPRB_IdChk]->Parent = pnPRB_IdCk ;
                 FraCylinder[aiWRK_Cmp  ]->Parent = pnWRK_Cmp  ;
                 FraCylinder[aiPSB_Cmp  ]->Parent = pnPSB_Cmp  ;
                 FraCylinder[aiPSB_Psh  ]->Parent = pnPSB_Psh  ;

                 PM.SetParent(pnPR1_XIdxPos , miPR1_XCmp);
                 PM.SetParent(pnPR2_XIdxPos , miPR2_XCmp);
                 PM.SetParent(pnWRK_XIdxPos , miWRK_XCmp);
                 PM.SetParent(pnPSB_XIdxPos , miPSB_XCmp);
        break;

        case 3 : FraMotor[miWRK_ZTop]->Parent = pnWRK_ZTop ;
                 FraMotor[miWRK_ZBtm]->Parent = pnWRK_ZBtm ;
                 FraMotor[miWRK_YBtm]->Parent = pnWRK_YBtm ;
                 FraMotor[miWRK_XBtm]->Parent = pnWRK_XBtm ;

                 FraCylinder[aiPR1_Cmp ]->Parent = pnPR1_Cmp2 ;
                 FraCylinder[aiPR2_Cmp ]->Parent = pnPR2_Cmp2 ;
                 FraCylinder[aiWRK_Cmp ]->Parent = pnWRK_Cmp2 ;
                 FraCylinder[aiPSB_Cmp ]->Parent = pnPSB_Cmp2 ;
                 FraCylinder[aiWRK_Hold]->Parent = pnWRK_Hold;

                 PM.SetParent(pnWRK_ZTopPos , miWRK_ZTop);
                 PM.SetParent(pnWRK_ZBtmPos , miWRK_ZBtm);
                 PM.SetParent(pnWRK_YBtmPos , miWRK_YBtm);
                 PM.SetParent(pnWRK_XBtmPos , miWRK_XBtm);
        break;

        case 4 : FraMotor[miSTL_YTop]->Parent = pnSTL_YTop ;
                 FraMotor[miREJ_XBtm]->Parent = pnRJT_XBtm ;

                 FraCylinder[aiSTL_Top]->Parent = pnSTL_TopPch ;
                 FraCylinder[aiSTL_Btm]->Parent = pnSTL_BtmDie ;
                 FraCylinder[aiPSB_Cmp]->Parent = pnPSB_Cmp3   ;
                 FraCylinder[aiPSB_Psh]->Parent = pnPSB_Psh2   ;

                 PM.SetParent(pnSTL_YTopPos , miSTL_YTop);
                 PM.SetParent(pnRJT_XBtmPos , miREJ_XBtm);
        break;

        case 5 : FraMotor[miULD_YCmp]->Parent = pnULD_YCmp ;
                 FraMotor[miULD_ZCmp]->Parent = pnULD_ZCmp ;

                 FraCylinder[aiULD_Cmp]->Parent = pnULD_Cmp ;

                 PM.SetParent(pnULD_YCmpPos , miULD_YCmp);
                 PM.SetParent(pnULD_ZCmpPos , miULD_ZCmp);


        break;
    }
}
//-------------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btLDR_CmpClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    bool bState =  AT_GetCmd(iSel);

    bool bRet ;

    if(iSel == aiPR1_Cmp      )  bRet = PR1.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPR2_Cmp    /* ||
       iSel == aiPRB_IdChk  */  )  bRet = PR2.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiWRK_Cmp      )  bRet = WRK.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPSB_Cmp      )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiSTL_Top      )  bRet = STL.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiSTL_Btm      )  bRet = STL.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiLDR_Psh      )  bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPSB_Psh      )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiPSB_Rolr     )  bRet = PSB.CheckSafe((EN_ACTR_ID)iSel , !bState) ;
    if(iSel == aiWRK_Hold     )  bRet = WRK.CheckSafe((EN_ACTR_ID)iSel , !bState) ;

    if(iSel == aiLDR_Cmp      ) {
        if(bState){
            if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) return ;
            bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , bState) ;
        }
        else { bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , !bState);}
    }
    if(iSel == aiULD_Cmp      ) {
        if(bState) {
            if(IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) return ;
            bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , bState) ;
        }
        else { bRet = ULD.CheckSafe((EN_ACTR_ID)iSel , !bState) ;}
    }


    if(!bRet) return ;

    AT_MoveCyl(iSel , !bState) ;        
}
//---------------------------------------------------------------------------







