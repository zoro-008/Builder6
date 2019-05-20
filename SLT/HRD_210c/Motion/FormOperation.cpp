//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormDeviceSet.h"
#include "FormLotOpen.h"
#include "UtilDefine.h"

#include "DataMan.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "SLogUnit.h"
#include "MapUnit.h"


#include "Loader.h"
#include "Epoxy.h"
#include "RailEpx.h"
#include "Head.h"
#include "Stage.h"
#include "PreBuff.h"
#include "RailDie.h"
#include "PostBuff.h"
#include "UnLoader.h"
#include "UserINI.h"

#include "SpcSubUnit.h"
#include "SAInterface.h"

#include "UserFile.h"
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
    if(!OM.MstOptn.bDebugMode) SEQ.Reset(false);
    EM_SetDisp(false);

    DM_Init();
    Man_Init();

    pcManual  -> ActivePageIndex = 0;
    pcControl -> ActivePageIndex = 0;

    pcWRE -> ActivePageIndex = 0;
    pcWRD -> ActivePageIndex = 0;
    pcPSB -> ActivePageIndex = 0;
    pcMap -> ActivePageIndex = 0;

    dUnit     = 0.0 ;
    dPitch    = 0.0 ;
    iUnitType = 0   ;

    SetUnitInit();

//    VSN.SetVisnFrm(ciOri , pnVisnOri) ;
    //VSN.SetVisnFrm(ciWfr , pnVisnWfr) ;
    //VSN.SetVisnFrm(ciRal , pnVisnRal) ;

    //FrmVisnMain -> Parent = pnVisnBase ;
    //FrmVisnMain -> Align  = alClient ;
    //FrmVisnMain -> Show();


}
//---------------------------------------------------------------------------
void TFrmOperation::DM_Init ()
{
    DM.ARAY[riLDR].SetParent(pnLDR ); DM.ARAY[riLDR].SetConfig(false , "LDR "); DM.ARAY[riLDR].ClearDispItem();
    DM.ARAY[riPRB].SetParent(pnPRB ); DM.ARAY[riPRB].SetConfig(false , "PRB "); DM.ARAY[riPRB].ClearDispItem();
    DM.ARAY[riREV].SetParent(pnREV ); DM.ARAY[riREV].SetConfig(false , "REV "); DM.ARAY[riREV].ClearDispItem();
    DM.ARAY[riREF].SetParent(pnREF ); DM.ARAY[riREF].SetConfig(false , "REF "); DM.ARAY[riREF].ClearDispItem();
    DM.ARAY[riRDV].SetParent(pnRDV ); DM.ARAY[riRDV].SetConfig(false , "RDV "); DM.ARAY[riRDV].ClearDispItem();
    DM.ARAY[riRDF].SetParent(pnRDF ); DM.ARAY[riRDF].SetConfig(false , "RDF "); DM.ARAY[riRDF].ClearDispItem();
    DM.ARAY[riHED].SetParent(pnHED ); DM.ARAY[riHED].SetConfig(false , "HED "); DM.ARAY[riHED].ClearDispItem();
    DM.ARAY[riCST].SetParent(pnCST ); DM.ARAY[riCST].SetConfig(false , "CST "); DM.ARAY[riCST].ClearDispItem();
    DM.ARAY[riSTG].SetParent(pnSTG ); DM.ARAY[riSTG].SetConfig(true  , "STG "); DM.ARAY[riSTG].ClearDispItem();
    DM.ARAY[riPSV].SetParent(pnPSV ); DM.ARAY[riPSV].SetConfig(false , "PSB "); DM.ARAY[riPSV].ClearDispItem();
    DM.ARAY[riPSF].SetParent(pnPSF ); DM.ARAY[riPSF].SetConfig(false , "RAP "); DM.ARAY[riPSF].ClearDispItem();
    DM.ARAY[riULD].SetParent(pnULD ); DM.ARAY[riULD].SetConfig(false , "ULD "); DM.ARAY[riULD].ClearDispItem();
    DM.ARAY[riMAP].SetParent(pnMAP ); DM.ARAY[riMAP].SetConfig(true  , "MAP "); DM.ARAY[riMAP].ClearDispItem();

    DM.ARAY[riLDR].SetDispItem(csNone   ,"자제없음"   ) ; DM.ARAY[riULD].SetDispItem(csNone   ,"메가진없음" ) ;
    DM.ARAY[riLDR].SetDispItem(csUnkwn  ,"작업전"     ) ; DM.ARAY[riULD].SetDispItem(csEmpty  ,"빈슬롯"     ) ;
    DM.ARAY[riLDR].SetDispItem(csEmpty  ,"빈슬롯"     ) ; DM.ARAY[riULD].SetDispItem(csWork   ,"찬슬롯"     ) ;


    DM.ARAY[riPRB].SetDispItem(csNone   ,"자제없음"   ) ;
    DM.ARAY[riPRB].SetDispItem(csUnkwn  ,"작업전"     ) ;
    DM.ARAY[riPRB].SetDispItem(csWork   ,"작업끝"     ) ;
    DM.ARAY[riPRB].SetDispItem(csRslt0  ,"비젼페일"   ) ;

    DM.ARAY[riREF].SetDispItem(csNone   ,"자제없음"   ) ; DM.ARAY[riREV].SetDispItem(csNone   ,"자제없음"   ) ;
    DM.ARAY[riREF].SetDispItem(csUnkwn  ,"작업전"     ) ; DM.ARAY[riREV].SetDispItem(csUnkwn  ,"작업전"     ) ;
    DM.ARAY[riREF].SetDispItem(csWork   ,"작업끝"     ) ; DM.ARAY[riREV].SetDispItem(csWork   ,"작업끝"     ) ;
/*DM.ARAY[riREF].SetDispItem(csRslt0  ,"V Ori Fail ") ;*/ DM.ARAY[riREV].SetDispItem(csRslt0  ,"방향불량"   ) ;

    DM.ARAY[riRDF].SetDispItem(csNone   ,"자제없음"   ) ; DM.ARAY[riRDV].SetDispItem(csNone   ,"자제없음"   ) ;
    DM.ARAY[riRDF].SetDispItem(csUnkwn  ,"작업전"     ) ; DM.ARAY[riRDV].SetDispItem(csUnkwn  ,"작업전"     ) ;
    DM.ARAY[riRDF].SetDispItem(csWait   ,"BF비전대기" ) ; DM.ARAY[riRDV].SetDispItem(csRslt0  ,"방향불량"   ) ;
    DM.ARAY[riRDF].SetDispItem(csMask   ,"칩놓기대기" ) ; DM.ARAY[riRDV].SetDispItem(csRslt1  ,"에폭시불량" ) ;
    DM.ARAY[riRDF].SetDispItem(csWait2  ,"AT비전대기" ) ; DM.ARAY[riRDV].SetDispItem(csRslt2  ,"칩위치불량" ) ;
    DM.ARAY[riRDF].SetDispItem(csWork   ,"작업끝"     ) ; DM.ARAY[riRDV].SetDispItem(csRslt3  ,"기준점불량" ) ;
                                                          DM.ARAY[riRDV].SetDispItem(csWork   ,"작업끝"     ) ;
    DM.ARAY[riHED].SetDispItem(csNone   ,"자제없음"   ) ;
    DM.ARAY[riHED].SetDispItem(csGood   ,"자제있음"   ) ;

    DM.ARAY[riCST].SetDispItem(csNone   ,"자제없음"   ) ;
    DM.ARAY[riCST].SetDispItem(csUnkwn  ,"작업전"     ) ;
    DM.ARAY[riCST].SetDispItem(csEmpty  ,"빈슬롯"     ) ;
    DM.ARAY[riCST].SetDispItem(csMask   ,"작업중"     ) ;
    DM.ARAY[riCST].SetDispItem(csWork   ,"작업끝"     ) ;


    DM.ARAY[riSTG].SetDispItem(csNone   ,"자제없음"    ) ; DM.ARAY[riMAP].SetDispItem(csNone   ,"자제없음"    ) ;
    DM.ARAY[riSTG].SetDispItem(csUnkwn  ,"작업전"      ) ; DM.ARAY[riMAP].SetDispItem(csUnkwn  ,"작업전"      ) ;
    DM.ARAY[riSTG].SetDispItem(csWait   ,"WF비전대기"  ) ; DM.ARAY[riMAP].SetDispItem(csWait   ,"WF비전대기"  ) ;
    DM.ARAY[riSTG].SetDispItem(csEmpty  ,"빈자제"      ) ; DM.ARAY[riMAP].SetDispItem(csEmpty  ,"빈자제"      ) ;
    DM.ARAY[riSTG].SetDispItem(csGood   ,"비전완료"    ) ; DM.ARAY[riMAP].SetDispItem(csGood   ,"비전완료"    ) ;
    DM.ARAY[riSTG].SetDispItem(csFail   ,"페일자제"    ) ; DM.ARAY[riMAP].SetDispItem(csFail   ,"페일자제"    ) ;
    DM.ARAY[riSTG].SetDispItem(csEror   ,"비전검사불량") ; DM.ARAY[riMAP].SetDispItem(csEror   ,"비전검사불량") ;
//DM.ARAY[riSTG].SetDispItem(csWork   ,"Work "      ) ; DM.ARAY[riMAP].SetDispItem(csWork   ,"Work "      ) ;

    DM.ARAY[riPSF].SetDispItem(csNone   ,"자제없음"   ) ; DM.ARAY[riPSV].SetDispItem(csNone   ,"자제없음"   ) ;
    DM.ARAY[riPSF].SetDispItem(csUnkwn  ,"작업전"     ) ; DM.ARAY[riPSV].SetDispItem(csUnkwn  ,"작업전"     ) ;
    DM.ARAY[riPSF].SetDispItem(csWork   ,"작업끝"     ) ; DM.ARAY[riPSV].SetDispItem(csWork   ,"작업끝"     ) ;
/*DM.ARAY[riPSF].SetDispItem(csRslt0  ,"V Ori Fail ") ;*/ DM.ARAY[riPSV].SetDispItem(csRslt0  ,"방향불량"   ) ;
/*DM.ARAY[riPSF].SetDispItem(csRslt1  ,"V Epx Fail ") ;*/ DM.ARAY[riPSV].SetDispItem(csRslt1  ,"에폭시불량" ) ;
/*DM.ARAY[riPSF].SetDispItem(csRslt2  ,"V Die Fail ") ;*/ DM.ARAY[riPSV].SetDispItem(csRslt2  ,"칩위치불량" ) ;
                                                          DM.ARAY[riPSV].SetDispItem(csRslt3  ,"기준점불량" ) ;


    //PopupMenuEnable
    for(int i =0 ; i < MAX_ARAY ; i++ ) {
        DM.ARAY[i].PopupMenuEnable(true);
    }
    DM.ARAY[riLDR].PopupMenuEnable(true);
    DM.ARAY[riULD].PopupMenuEnable(true);
    DM.ARAY[riCST].PopupMenuEnable(true);
    //DM.ARAY[riSTG].PopupMenuEnable(false);
    DM.ARAY[riSTG].PopupMenuEnable(true);
}
void TFrmOperation::Man_Init()
{

    //Manual Button Caption
    String sTemp ;
    int    iTag = 0 ;
    /*
    iTag = mcLDR_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "CycleHome         "; SetManualBtn(btMan1_1 , sTemp , iTag) ;
    iTag = mcLDR_CyclePick        ; sTemp = "[" + AnsiString(iTag) + "]" + "CyclePick         "; SetManualBtn(btMan1_2 , sTemp , iTag) ;
    iTag = mcLDR_CycleWork        ; sTemp = "[" + AnsiString(iTag) + "]" + "CycleWork         "; SetManualBtn(btMan1_3 , sTemp , iTag) ;
    iTag = mcLDR_CyclePush        ; sTemp = "[" + AnsiString(iTag) + "]" + "CyclePush         "; SetManualBtn(btMan1_4 , sTemp , iTag) ;
    iTag = mcLDR_CyclePlace       ; sTemp = "[" + AnsiString(iTag) + "]" + "CyclePlace        "; SetManualBtn(btMan1_5 , sTemp , iTag) ;

    iTag = mcPRB_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "CycleHome         "; SetManualBtn(btMan2_1 , sTemp , iTag) ;
    iTag = mcPRB_CycleWorkStt     ; sTemp = "[" + AnsiString(iTag) + "]" + "CycleWorkStt      "; SetManualBtn(btMan2_2 , sTemp , iTag) ;
    iTag = mcPRB_CycleWork        ; sTemp = "[" + AnsiString(iTag) + "]" + "CycleWork         "; SetManualBtn(btMan2_3 , sTemp , iTag) ;
    iTag = mcPRB_CycleReGrip      ; sTemp = "[" + AnsiString(iTag) + "]" + "CycleReGrip       "; SetManualBtn(btMan2_4 , sTemp , iTag) ;
    iTag = mcPRB_CycleWorkEnd     ; sTemp = "[" + AnsiString(iTag) + "]" + "CycleWorkEnd      "; SetManualBtn(btMan2_5 , sTemp , iTag) ;

    iTag = mcRAE_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleHome    "; SetManualBtn(btMan3_1 , sTemp , iTag) ;
    iTag = mcRAE_CycleWorkStt     ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleWorkStt "; SetManualBtn(btMan3_2 , sTemp , iTag) ;
    iTag = mcRAE_CycleReGrip      ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleReGrip  "; SetManualBtn(btMan3_3 , sTemp , iTag) ;
    iTag = mcRAE_CycleWork        ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleWork    "; SetManualBtn(btMan3_4 , sTemp , iTag) ;
    iTag = mcRAE_CycleWorkEnd     ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleWorkEnd "; SetManualBtn(btMan3_5 , sTemp , iTag) ;

    iTag = mcEPX_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "EPX CycleHome     "; SetManualBtn(btMan3_8 , sTemp , iTag) ;
    iTag = mcEPX_CycleClean       ; sTemp = "[" + AnsiString(iTag) + "]" + "EPX CycleClean    "; SetManualBtn(btMan3_9 , sTemp , iTag) ;
//    iTag = mcEPX_CycleOri         ; sTemp = "[" + AnsiString(iTag) + "]" + "EPX CycleOri      "; SetManualBtn(btMan3_A , sTemp , iTag) ;
    iTag = mcEPX_CycleWork        ; sTemp = "[" + AnsiString(iTag) + "]" + "EPX CycleWork     "; SetManualBtn(btMan3_B , sTemp , iTag) ;

    iTag = mcRAD_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleHome    "; SetManualBtn(btMan4_1 , sTemp , iTag) ;
    iTag = mcRAD_CycleWorkStt     ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleWorkStt "; SetManualBtn(btMan4_2 , sTemp , iTag) ;
    iTag = mcRAD_CycleReGrip      ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleReGrip  "; SetManualBtn(btMan4_3 , sTemp , iTag) ;
    iTag = mcRAD_CycleWork        ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleWork    "; SetManualBtn(btMan4_4 , sTemp , iTag) ;
    iTag = mcRAD_CycleAtVisn      ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleAtVisn  "; SetManualBtn(btMan4_5 , sTemp , iTag) ;
    iTag = mcRAD_CycleBfVisn      ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleBfVisn  "; SetManualBtn(btMan4_6 , sTemp , iTag) ;
    iTag = mcRAD_CycleWorkEnd     ; sTemp = "[" + AnsiString(iTag) + "]" + "RAIL CycleWorkEnd "; SetManualBtn(btMan4_7 , sTemp , iTag) ;

    iTag = mcHED_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "HED CycleHome     "; SetManualBtn(btMan4_8 , sTemp , iTag) ;
    iTag = mcHED_CyclePick        ; sTemp = "[" + AnsiString(iTag) + "]" + "HED CyclePick     "; SetManualBtn(btMan4_9 , sTemp , iTag) ;
    iTag = mcHED_CyclePlace       ; sTemp = "[" + AnsiString(iTag) + "]" + "HED CyclePlace    "; SetManualBtn(btMan4_A , sTemp , iTag) ;

    iTag = mcSTG_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "STG CycleHome     "; SetManualBtn(btMan5_1 , sTemp , iTag) ;
    iTag = mcSTG_CycleCsSupply    ; sTemp = "[" + AnsiString(iTag) + "]" + "STG CycleCsSupply "; SetManualBtn(btMan5_2 , sTemp , iTag) ;
    iTag = mcSTG_CycleStgExpend   ; sTemp = "[" + AnsiString(iTag) + "]" + "STG CycleStgExpend"; SetManualBtn(btMan5_3 , sTemp , iTag) ;
    iTag = mcSTG_CycleStgVision   ; sTemp = "[" + AnsiString(iTag) + "]" + "STG CycleStgVision"; SetManualBtn(btMan5_4 , sTemp , iTag) ;
    //iTag = mcSTG_CycleStgWork     ; sTemp = "[" + AnsiString(iTag) + "]" + "STG CycleStgWork  "; SetManualBtn(btMan5_5 , sTemp , iTag) ;
    iTag = mcSTG_CycleCsPlace     ; sTemp = "[" + AnsiString(iTag) + "]" + "STG CycleCsPlace  "; SetManualBtn(btMan5_9 , sTemp , iTag) ;
    iTag = mcSTG_CycleCenterMove  ; sTemp = "[" + AnsiString(iTag) + "]" + "STG Chip Align    "; SetManualBtn(btMan5_7 , sTemp , iTag) ;
    iTag = mcSTG_CycleUnExpend    ; sTemp = "[" + AnsiString(iTag) + "]" + "STG Cycle UnExpend"; SetManualBtn(btMan5_A , sTemp , iTag) ;
    iTag = mcSTG_CycleCenterMoveT ; sTemp = "[" + AnsiString(iTag) + "]" + "STG Chip Align(T) "; SetManualBtn(btMan5_E , sTemp , iTag) ;

    iTag = mcSTG_CycleStgTAlign   ; sTemp = "[" + AnsiString(iTag) + "]" + "STG CycleStgTAlign"; SetManualBtn(btMan5_5 , sTemp , iTag) ;

    iTag = mcPSB_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "PSB CycleHome     "; SetManualBtn(btMan6_1 , sTemp , iTag) ;
    iTag = mcPSB_CyclePull        ; sTemp = "[" + AnsiString(iTag) + "]" + "PSB CyclePull     "; SetManualBtn(btMan6_2 , sTemp , iTag) ;
    iTag = mcPSB_CycleBackOut     ; sTemp = "[" + AnsiString(iTag) + "]" + "PSB CycleBackOut  "; SetManualBtn(btMan6_3 , sTemp , iTag) ;

    iTag = mcULD_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "ULD CycleHome     "; SetManualBtn(btMan7_1 , sTemp , iTag) ;
    iTag = mcULD_CyclePick        ; sTemp = "[" + AnsiString(iTag) + "]" + "ULD CyclePick     "; SetManualBtn(btMan7_2 , sTemp , iTag) ;
    iTag = mcULD_CycleWork        ; sTemp = "[" + AnsiString(iTag) + "]" + "ULD CycleWork     "; SetManualBtn(btMan7_3 , sTemp , iTag) ;
//    iTag = mcULD_CyclePush        ; sTemp = "[" + AnsiString(iTag) + "]" + "ULD CyclePush     "; SetManualBtn(btMan7_4 , sTemp , iTag) ;
    iTag = mcULD_CyclePlace       ; sTemp = "[" + AnsiString(iTag) + "]" + "ULD CyclePlace    "; SetManualBtn(btMan7_4 , sTemp , iTag) ;





    //급추가.
//    iTag = mcPickPlaceHome      ; sTemp = "[" + AnsiString(iTag) + "]" + "PickPlaceHome     "; SetManualBtn(btMan1_C , sTemp , iTag) ;
//    iTag = mcPickPlaceTest      ; sTemp = "[" + AnsiString(iTag) + "]" + "PickPlaceTest     "; SetManualBtn(btMan1_D , sTemp , iTag) ;



    iTag = mcAllHome            ; sTemp = "[" + AnsiString(mcAllHome ) + "]" + "ALL HOME      "; SetManualBtn(btMan1_E , sTemp , iTag) ;
*/


    iTag = mcLDR_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "LDR 홈";                 SetManualBtn(btMan1_1 , sTemp , iTag) ;
    iTag = mcLDR_CyclePick        ; sTemp = "[" + AnsiString(iTag) + "]" + "LDR 공급";               SetManualBtn(btMan1_2 , sTemp , iTag) ;
    iTag = mcLDR_CyclePlace       ; sTemp = "[" + AnsiString(iTag) + "]" + "LDR 배출";               SetManualBtn(btMan1_3 , sTemp , iTag) ;
    iTag = mcLDR_CyclePitchUp     ; sTemp = "[" + AnsiString(iTag) + "]" + "LDR 피치업";             SetManualBtn(btMan1_4 , sTemp , iTag) ;
    iTag = mcLDR_CyclePitchDn     ; sTemp = "[" + AnsiString(iTag) + "]" + "LDR 피치다운";           SetManualBtn(btMan1_5 , sTemp , iTag) ;
    iTag = 1000+aiLDR_Pusher*10   ; sTemp = "[" + AnsiString(iTag) + "]" + "LDR 푸셔";               SetManualBtn(btMan1_6 , sTemp , iTag) ;
    iTag = 1000+aiPRB_Stopper*10  ; sTemp = "[" + AnsiString(iTag) + "]" + "PRB 스토퍼";             SetManualBtn(btMan1_7 , sTemp , iTag) ;
    iTag = mcULD_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "ULD 홈";                 SetManualBtn(btMan1_8 , sTemp , iTag) ;
    iTag = mcULD_CyclePick        ; sTemp = "[" + AnsiString(iTag) + "]" + "ULD 공급";               SetManualBtn(btMan1_9 , sTemp , iTag) ;
    iTag = mcULD_CyclePlace       ; sTemp = "[" + AnsiString(iTag) + "]" + "ULD 배출";               SetManualBtn(btMan1_A , sTemp , iTag) ;
    iTag = mcULD_CyclePitchUp     ; sTemp = "[" + AnsiString(iTag) + "]" + "ULD 피치업";             SetManualBtn(btMan1_B , sTemp , iTag) ;
    iTag = mcULD_CyclePitchDn     ; sTemp = "[" + AnsiString(iTag) + "]" + "ULD 피치다운";           SetManualBtn(btMan1_C , sTemp , iTag) ;
    iTag = mcAllHome              ; sTemp = "[" + AnsiString(iTag) + "]" + "ALL HOME";               SetManualBtn(btMan1_E , sTemp , iTag) ;


    iTag = mcPRB_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "PRB 홈";                 SetManualBtn(btMan2_1 , sTemp , iTag) ;
    iTag = 1000+aiPRB_Clamp*10    ; sTemp = "[" + AnsiString(iTag) + "]" + "PRB 클램프";             SetManualBtn(btMan2_2 , sTemp , iTag) ;
    iTag = 1000+aiLDR_Pusher*10   ; sTemp = "[" + AnsiString(iTag) + "]" + "LDR 푸셔";               SetManualBtn(btMan2_3 , sTemp , iTag) ;
    iTag = 1000+aiPRB_Stopper*10  ; sTemp = "[" + AnsiString(iTag) + "]" + "PRB 스토퍼";             SetManualBtn(btMan2_4 , sTemp , iTag) ;


    iTag = mcRAE_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "EPX 레일 홈";            SetManualBtn(btMan3_1 , sTemp , iTag) ;
    iTag = 1000+aiWRE_Clamp*10    ; sTemp = "[" + AnsiString(iTag) + "]" + "EPX 클램프";             SetManualBtn(btMan3_2 , sTemp , iTag) ;
    iTag = mcEPX_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "EPX 헤드 홈";            SetManualBtn(btMan3_8 , sTemp , iTag) ;
    iTag = mcEPX_CycleClean       ; sTemp = "[" + AnsiString(iTag) + "]" + "EPX 헤드 클린";          SetManualBtn(btMan3_9 , sTemp , iTag) ;
    iTag = mcETC_ToolChange       ; sTemp = "[" + AnsiString(iTag) + "]" + "툴 체인지";              SetManualBtn(btMan3_A,  sTemp , iTag) ;
    iTag = mcEPX_CycleWork        ; sTemp = "[" + AnsiString(iTag) + "]" + "EPX 헤드 작업";          SetManualBtn(btMan3_B , sTemp , iTag) ;
    iTag = mcETC_EpoxyOneShot     ; sTemp = "[" + AnsiString(iTag) + "]" + "EPX 원샷     ";          SetManualBtn(btMan3_C , sTemp , iTag) ;


    iTag = mcRAD_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "HED RAIL 홈";            SetManualBtn(btMan4_1 , sTemp , iTag) ;
    iTag = 1000+aiWRD_Clamp*10    ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 클램프";             SetManualBtn(btMan4_2 , sTemp , iTag) ;

    iTag = mcHED_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 홈";                 SetManualBtn(btMan4_8 , sTemp , iTag) ;
    iTag = mcHED_CyclePick        ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 픽";                 SetManualBtn(btMan4_9 , sTemp , iTag) ;
    iTag = mcHED_CyclePlace       ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 플레이스";           SetManualBtn(btMan4_A , sTemp , iTag) ;
    //iTag = mcHED_CycleWfrPlace    ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 웨이퍼플레이스";    SetManualBtn(btMan4_B , sTemp , iTag) ;
    //iTag = mcHED_CycleWfrCal      ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 웨이퍼 CAL";        SetManualBtn(btMan4_C , sTemp , iTag) ;
    //iTag = mcHED_CycleStpCal      ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 보트 CAL";          SetManualBtn(btMan4_D , sTemp , iTag) ;

    iTag = mcSTG_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 홈";                 SetManualBtn(btMan5_1 , sTemp , iTag) ;
    iTag = mcSTG_CycleCsSupply    ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 공급";               SetManualBtn(btMan5_2 , sTemp , iTag) ;
    iTag = mcSTG_CycleStgExpend   ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 익스펜딩";           SetManualBtn(btMan5_3 , sTemp , iTag) ;
    iTag = mcSTG_CycleStgVision   ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 비전동작";           SetManualBtn(btMan5_4 , sTemp , iTag) ;
    iTag = mcSTG_CycleStgTAlign   ; sTemp = "[" + AnsiString(iTag) + "]" + "STG T얼라인";            SetManualBtn(btMan5_5 , sTemp , iTag) ;
    iTag = mcSTG_CycleStartSetMc  ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 첫칩으로이동";       SetManualBtn(btMan5_6 , sTemp , iTag) ;
    iTag = mcSTG_CycleCsPlace     ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 배출";               SetManualBtn(btMan5_9 , sTemp , iTag) ;
    //iTag = mcSTG_CycleCenterMove  ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 비전센터이동";       SetManualBtn(btMan5_7 , sTemp , iTag) ;
    iTag = mcSTG_CycleUnExpend    ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 언익스펜딩";         SetManualBtn(btMan5_A , sTemp , iTag) ;
    //iTag = mcSTG_CycleCenterMoveT ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 비전센터이동T";      SetManualBtn(btMan5_E , sTemp , iTag) ;

    iTag = mcPSB_CycleHome        ; sTemp = "[" + AnsiString(iTag) + "]" + "PSB 홈";                 SetManualBtn(btMan6_1 , sTemp , iTag) ;
    iTag = 1000+aiPSB_Clamp*10    ; sTemp = "[" + AnsiString(iTag) + "]" + "PSB 클램프";             SetManualBtn(btMan6_2 , sTemp , iTag) ;

    iTag = mcHED_CycleWfrCal      ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 웨이퍼 CAL";         SetManualBtn(btMan7_1 , sTemp , iTag) ;
    iTag = mcHED_CycleStpCal      ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 보트 CAL";           SetManualBtn(btMan7_2 , sTemp , iTag) ;
    iTag = mcHED_CyclePick        ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 픽";                 SetManualBtn(btMan7_4 , sTemp , iTag) ;
    iTag = mcHED_CyclePickNoT     ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 보정없이픽";         SetManualBtn(btMan7_5 , sTemp , iTag) ;
    iTag = mcHED_CycleWfrPlace    ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 웨이퍼플레이스";     SetManualBtn(btMan7_6 , sTemp , iTag) ;
    iTag = mcHED_CycleRalPlace    ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 레일 플레이스";      SetManualBtn(btMan7_7 , sTemp , iTag) ;
    iTag = mcHED_CyclePlace       ; sTemp = "[" + AnsiString(iTag) + "]" + "HED 플레이스";           SetManualBtn(btMan7_8 , sTemp , iTag) ;
    iTag = mcRAD_CycleBfVisn      ; sTemp = "[" + AnsiString(iTag) + "]" + "RAL BF비전";             SetManualBtn(btMan7_9 , sTemp , iTag) ;
    iTag = mcSTG_CycleCenterMove  ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 비전센터이동";       SetManualBtn(btMan7_D , sTemp , iTag) ;
    iTag = mcSTG_CycleCenterMoveT ; sTemp = "[" + AnsiString(iTag) + "]" + "STG 비전센터이동T";      SetManualBtn(btMan7_E , sTemp , iTag) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::SetManualBtn(TBitBtn *_btn , String _sCaption , int _iTag)
{
    _btn -> Caption = _sCaption.Trim() ;
    _btn -> Tag     = _iTag     ;
    _btn -> Visible = true      ;
}

//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    if(SEQ._iSeqStat == ssError) {
        FM_MsgTime("Warning","에러 리셋을 해주십시오.",1000) ;
        return ;
    }

    if(SEQ._iSeqStat != ssStop) {
        FM_MsgTime("Warning","장비가 정지상태가 아닙니다.",1000) ;
        return ;
    }

    int iManNo ;
    int iActNo ;
    iManNo = ((TBitBtn *)Sender) -> Tag ;

    if(iManNo / 1000 == 1){//Actuator
        iActNo = (iManNo - 1000) / 10 ;

             if(iActNo == aiLDR_Stopper) LDR.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiLDR_MgzGrip) LDR.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiLDR_Out    ) LDR.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiLDR_Pusher ) LDR.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiPRB_Stopper) PRB.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiPRB_Clamp  ) PRB.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiWRE_Clamp  ) RAE.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiWRD_Clamp  ) RAD.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiPSB_Clamp  ) PSB.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiEJT_UpDn   ) STG.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiTRS_Chuck  ) STG.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiULD_Stopper) ULD.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiULD_MgzGrip) ULD.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiULD_Out    ) ULD.MoveActr(iActNo , !AT_GetCmd(iActNo));
        else if(iActNo == aiTRS_UpDn   ) STG.MoveActr(iActNo , !AT_GetCmd(iActNo));
        return ;
    }






    if(iManNo == mcSTG_CycleStartSetMc && OM.DevOptn.iOriMathod != omMatch) iManNo = mcSTG_CycleStartSetEg ;

    MM.SetManCycle((EN_MANUAL_CYCLE)iManNo);
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
/*
    switch (iLevel) {
        case lvOperator : if( iLev != lvOperator) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvEngineer ;
                          }
                          break ;
        case lvMaster   : if( iLev != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iLev = lvMaster ;
                          }
                          break ;
    }
*/
}
void TFrmOperation::LT_Display()
{

    LT.DispLotInfo(
        pnLotNo        ,
        pnLotJobFile   ,
        NULL           ,
        pnLotStartTime ,
        NULL           ,
        pnLotRunTime   ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        pnDayChipUpeh  ,
        NULL           ,
        pnLotChipUph   ,  //
        NULL           ,
        pnLotWorkChip  ,
        pnLotFailChip  );

    LT.DispDayInfo(
        pnDayRunTime    ,
        pnDayIdleTime   ,
        pnDayJamTime    ,
        pnDayTotalTime  ,
        NULL            ,
        pnLotChipUpeh   ,
        NULL            ,
        pnDayChipUph    ,
        NULL            ,
        pnDayWorkChip   ,
        pnDayFailChip   ,
        pnDayLotCount   );
}
void TFrmOperation::OP_Dispaly()
{

    pnOptn0 -> Color = OM.CmnOptn.bLoadingStop  ? clLime : clRed ; pnOptn0 -> Caption = OM.CmnOptn.bLoadingStop ? "ON" : "OFF" ;
    pnOptn1 -> Color = OM.CmnOptn.bStageStop    ? clLime : clRed ; pnOptn1 -> Caption = OM.CmnOptn.bStageStop   ? "ON" : "OFF" ;
    pnOptn2 -> Color = OM.CmnOptn.bUseMapFile   ? clLime : clRed ; pnOptn2 -> Caption = OM.CmnOptn.bUseMapFile  ? "ON" : "OFF" ;
    pnOptn3 -> Color = OM.CmnOptn.bEpoxyStop    ? clLime : clRed ; pnOptn3 -> Caption = OM.CmnOptn.bEpoxyStop   ? "ON" : "OFF" ;
    //pnOptn4 -> Color = OM.CmnOptn.bInkDieWork   ? clLime : clRed ; pnOptn4 -> Caption = OM.CmnOptn.bInkDieWork  ? "ON" : "OFF" ;

}
void __fastcall TFrmOperation::pnOptn1Click(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

//    if(FM_GetLevel() == lvOperator) return ;

    switch(iIndex) {
        case 0 :                                                       break ;
        case 1 :                                                       break ;
        case 2 :                                                       break ;
        case 3 :                                                       break ;
        case 4 :                                                       break ;
        case 5 :                                                       break ;
        case 6 :                                                       break ;
        case 7 :                                                       break ;
    }

    //if (FM_MsgYesNo("Confirm", "Would you like to invert the flag?") != mrYes) return ;

    switch(iIndex) {
        case 0 : OM.CmnOptn.bLoadingStop  = !OM.CmnOptn.bLoadingStop ; break ;
        case 1 : OM.CmnOptn.bStageStop    = !OM.CmnOptn.bStageStop   ; break ;
        case 2 : OM.CmnOptn.bUseMapFile   = !OM.CmnOptn.bUseMapFile  ; break ;
        case 3 : OM.CmnOptn.bEpoxyStop    = !OM.CmnOptn.bEpoxyStop   ; break ;
//        case 4 : OM.CmnOptn.bInkDieWork   = !OM.CmnOptn.bInkDieWork  ; break ;
//        case 1 : OM.CmnOptn.bLoadingStop  = !OM.CmnOptn.bLoadingStop ; break ;
//        case 2 : OM.CmnOptn.bIgnrOriVs    = !OM.CmnOptn.bIgnrOriVs   ; break ;

    }

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
//    if(LT.m_bLotOpen) {
//        FM_MsgOk("Error" , "LotOpen Please.");
//        return;
//    }

    SEQ._bBtnStart = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btStopClick(TObject *Sender)
{
    SEQ._bBtnStop = true ;
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
//    //스트립 사라짐 에러.
//    if (DM.ARAY[riPR2].CheckAllStat(csNone)&& DM.ARAY[riPR1].CheckAllStat(csNone) && IO_GetX(xPRB_3Pkg) ) {
//        if(FM_MsgYesNo("Err " , "[PreBuffer2 Strip Inserted] \nDo you want to Set Strip Data?"     ) == mrYes ) {
//            DM.ARAY[riPR2].SetStat(csUnkwn);
//            if(DM.ARAY[riLD1].CheckAllStat(csNone) || DM.ARAY[riLD1].CheckAllStat(csUnkwn)) DM.ARAY[riPR2].SetLotNo(PRB.GetWorkedLotNo()     ) ;
//            else                                                                            DM.ARAY[riPR2].SetLotNo(DM.ARAY[riLD1].GetLotNo()) ;
//            SEQ._bBtnStart = true ;
//        }
//    }
//
    return true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPasswordCloseClick(TObject *Sender)
{
    plPassWord -> Visible = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
    //FM_Update();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)
{

    SV_SetVisnForm(pnVisnBase -> Handle);



    tmUpdate->Enabled = true;
    tmUpdateAray->Enabled = true ;













//    FrmDeviceSet->FraMotor[miWRD_XHed]->Parent = pnHED_X ;
//    FrmDeviceSet->FraMotor[miWRD_YHed]->Parent = pnHED_Y ;

//    PM.SetWindow(pnHED_Xp ,miWRD_XHed);
//    PM.SetWindow(pnHED_Yp ,miWRD_YHed);
//    PM.UpdatePstn (toTabl);
//    PM.SetParent(pnHED_Xp ,miWRD_XHed);
//    PM.SetParent(pnHED_Yp ,miWRD_YHed);

//    if(rgJogUnit->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utJog ,0   );
//    if(rgJogUnit->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,1   );
//    if(rgJogUnit->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,0.5 );
//    if(rgJogUnit->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,0.1 );
//    if(rgJogUnit->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,0.05);
//    if(rgJogUnit->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,0.01);
//    if(rgJogUnit->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
//    if(rgJogUnit->ItemIndex == 7) for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utPitch,0  );

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
    tmUpdateAray->Enabled = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
    tmUpdateAray -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::edJogUnitChange(TObject *Sender)
{
//    if(rgJogUnit->ItemIndex == 5)  for(int i=0;i<MAX_MOTR;i++) FrmDeviceSet->FraMotor[i]->SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSourceClick(TObject *Sender)
{
    String sDir = OM.CmnOptn.sSourcePath ;

    if(!DirectoryExists(sDir)) UserFile.CreateDir(sDir.c_str());

    HWND hwnd = NULL;
    LPCTSTR lpOperation = "open";
    LPCTSTR lpFile = NULL;
    LPCTSTR lpParameters = NULL;
    LPCTSTR lpDirectory = sDir.c_str();
    int nShowCmd = SW_SHOWDEFAULT;

    ShellExecute(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOutputClick(TObject *Sender)
{
    String sDir = OM.CmnOptn.sWorkedPath ;

    if(!DirectoryExists(sDir)) UserFile.CreateDir(sDir.c_str());

    HWND hwnd = NULL;
    LPCTSTR lpOperation = "open";
    LPCTSTR lpFile = NULL;
    LPCTSTR lpParameters = NULL;
    LPCTSTR lpDirectory = sDir.c_str();
    int nShowCmd = SW_SHOWDEFAULT;

    ShellExecute(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::sgOutput1Click(TObject *Sender)
{
    //UserFile.GridSearchFile(OM.CmnOptn.sSourcePath , sgOutput1 , 0 , false);   // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

         if(OM.DevInfo.iMapType == 0){ if(!MAPU.ConvertCmn  (sgOutput1 , riMAP)){ FM_MsgOk("CONFIRM","MAP CONVERT FAIL"); }}
    else if(OM.DevInfo.iMapType == 1){ if(!MAPU.ConvertNxt  (sgOutput1 , riMAP)){ FM_MsgOk("CONFIRM","MAP CONVERT FAIL"); }}
    //else if(OM.DevInfo.iMapType == 2){ if(!MAPU.ConvertCoordi2  (sgOutput1 , riMAP)){ FM_MsgOk("CONFIRM","MAP CONVERT FAIL"); }}
    //else                             { if(!MAPU.ConvertCoordi8in(sgOutput1 , riMAP)){ FM_MsgOk("CONFIRM","MAP CONVERT FAIL"); }}

    iTemp = sgOutput1 -> Row;
    lbGoodCnt -> Caption = DM.ARAY[riMAP].GetCntStat(csGood);
    lbFailCnt -> Caption = DM.ARAY[riMAP].GetCntStat(csFail);
    lbNoneCnt -> Caption = DM.ARAY[riMAP].GetCntStat(csNone);

    int r,c ;
    DM.ARAY[riMAP].FindFrstRowCol(csGood,r,c);

    if(r>-1 && c>-1) {
        lbFstRow  -> Caption = r + 1;
        lbFstCol  -> Caption = c + 1;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
//    FrmLotOpen -> Show();
    if(LT.m_bLotOpen) {
        if(FM_MsgYesNo("Confirm","기존랏을 종료하고 랏오픈 하시겠습니까?") != mrYes) {
            return ;
        }
        LT.LotEnd();
        DM.ARAY[riLDR].SetStat(csNone);
        DM.ARAY[riPRB].SetStat(csNone);
        DM.ARAY[riREV].SetStat(csNone);
        DM.ARAY[riREF].SetStat(csNone);
        DM.ARAY[riRDV].SetStat(csNone);
        DM.ARAY[riRDF].SetStat(csNone);
        DM.ARAY[riHED].SetStat(csNone);
        //DM.ARAY[riCST].SetStat(csNone); 이놈들은 클리어 되면 안됌.
        //DM.ARAY[riSTG].SetStat(csNone);
        DM.ARAY[riPSV].SetStat(csNone);
        DM.ARAY[riPSF].SetStat(csNone);
        DM.ARAY[riULD].SetStat(csNone);
        DM.ARAY[riMAP].SetStat(csNone);

        FM_MsgTime("Check","스트립과 메거진을 꺼내주세요.",2000) ;
    }
    else {
        if(FM_MsgYesNo("Confirm","랏오픈 하시겠습니까?") != mrYes) return ;
    }

    TDateTime tTemp ;
    tTemp.Val = Now();
    AnsiString sLotNo = tTemp.FormatString("mm_dd_hh_nn_ss") ;
    LT.LotOpen(sLotNo , OM.m_sCrntDev );

    LT.AddDayInfoLotCnt(1);

    LDR._iMgzCnt = 0 ;

    LT.m_bRqstLotEnd = false ;

    /*MGZ*/
//    EPX.SetEpxLotCnt(edEpxSet->Text.ToIntDef(0) );
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotEndClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm", "랏엔드를 하겠습니까?") != mrYes) return ;

    LT.m_bRqstLotEnd = true ;

    Trace("btLotEndClick","");

    //String sLotNo = LT.GetCrntLot() ;
    //LT.LotEnd();

    //DM.ClearMap();
    /*
    DM.ARAY[riLDR].SetStat(csNone);
    DM.ARAY[riPRB].SetStat(csNone);
    DM.ARAY[riREV].SetStat(csNone);
    DM.ARAY[riREF].SetStat(csNone);
    DM.ARAY[riRDV].SetStat(csNone);
    DM.ARAY[riRDF].SetStat(csNone);
    DM.ARAY[riHED].SetStat(csNone);
    //DM.ARAY[riCST].SetStat(csNone); 이놈들은 클리어 되면 안됌.
    //DM.ARAY[riSTG].SetStat(csNone);
    DM.ARAY[riPSV].SetStat(csNone);
    DM.ARAY[riPSF].SetStat(csNone);
    DM.ARAY[riULD].SetStat(csNone);
    DM.ARAY[riMAP].SetStat(csNone);
    */

}
//---------------------------------------------------------------------------






void __fastcall TFrmOperation::Button8Click(TObject *Sender)
{
     double dMoveX , dMoveY , dMoveT ;
     STG.GetNextPos(MT_GetCmdPos(miWRD_XStg),MT_GetCmdPos(miWRD_YStg),MT_GetCmdPos(miWRD_TStg) , dMoveX , dMoveY , dMoveT );
     MT_GoAbsRun(miWRD_XStg , dMoveX);
     MT_GoAbsRun(miWRD_YStg , dMoveY);
     MT_GoAbsRun(miWRD_TStg , dMoveT);
}
//---------------------------------------------------------------------------


void TFrmOperation::SetPitch(double _dUnit)
{
    dPitch = _dUnit ;
}
void TFrmOperation::SetUnit(EN_UNIT_TYPE _iUnitType , double _dUnit)
{
    dUnit     = _dUnit     ;
    iUnitType = _iUnitType ;
}
//---------------------------------------------------------------------------
void TFrmOperation::SetUnitInit()
{
    if(rgJogUnit->ItemIndex == 0) SetUnit(utJog ,0   );
    if(rgJogUnit->ItemIndex == 1) SetUnit(utMove,1   );
    if(rgJogUnit->ItemIndex == 2) SetUnit(utMove,0.5 );
    if(rgJogUnit->ItemIndex == 3) SetUnit(utMove,0.1 );
    if(rgJogUnit->ItemIndex == 4) SetUnit(utMove,0.05);
    if(rgJogUnit->ItemIndex == 5) SetUnit(utMove,0.01);
    if(rgJogUnit->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
    if(rgJogUnit->ItemIndex == 7) SetUnit(utPitch,0  );

    btHed_ZUp -> Tag = (int)miWRD_ZHed ;
    btHed_ZDn -> Tag = (int)miWRD_ZHed ;
    btSTG_TUc -> Tag = (int)miWRD_TStg ;
    btSTG_Tcl -> Tag = (int)miWRD_TStg ;
    btWRD_YUp -> Tag = (int)miWRD_YIns ;
    btWRD_YDn -> Tag = (int)miWRD_YIns ;
    btHED_YUp -> Tag = (int)miWRD_YHed ;
    btHED_YDn -> Tag = (int)miWRD_YHed ;
    btHED_XLt -> Tag = (int)miWRD_XHed ;
    btHED_XRt -> Tag = (int)miWRD_XHed ;
    btSTG_YUp -> Tag = (int)miWRD_YStg ;
    btSTG_YDn -> Tag = (int)miWRD_YStg ;
    btSTG_XLt -> Tag = (int)miWRD_XStg ;
    btSTG_XRt -> Tag = (int)miWRD_XStg ;

}

void __fastcall TFrmOperation::rgJogUnitClick(TObject *Sender)
{
    if(rgJogUnit->ItemIndex == 0) SetUnit(utJog ,0   );
    if(rgJogUnit->ItemIndex == 1) SetUnit(utMove,1   );
    if(rgJogUnit->ItemIndex == 2) SetUnit(utMove,0.5 );
    if(rgJogUnit->ItemIndex == 3) SetUnit(utMove,0.1 );
    if(rgJogUnit->ItemIndex == 4) SetUnit(utMove,0.05);
    if(rgJogUnit->ItemIndex == 5) SetUnit(utMove,0.01);
    if(rgJogUnit->ItemIndex == 6) SetUnit(utMove,StrToFloatDef(edJogUnit->Text,0.0));
    if(rgJogUnit->ItemIndex == 7) SetUnit(utPitch,0  );
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSetDirctionRightClick(TObject *Sender)
{
    STG._dLastWorkPosX = MT_GetCmdPos(miWRD_XStg);
    STG._dLastWorkPosY = MT_GetCmdPos(miWRD_YStg);               //m_bWfrAligned
    STG._dLastWorkPosT = MT_GetCmdPos(miWRD_TStg);
    STG.SetStartSet(true); //스타트셑을 강제로 온시킴.
    STG.SetWfrTAligned(true); //얼라인 강제로 온시킴.

    bool bDirctLeft = ((TButton *)Sender) -> Tag == 1  ;

    STG.SetDirction(bDirctLeft);

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btJogNMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    MT_Stop((EN_MOTR_ID)m_iId) ;

         if(m_iId == miWRD_YIns) dPitch = OM.DevInfo.dRowPitch;
    else if(m_iId == miWRD_XHed) dPitch = OM.DevInfo.dWf_PitchX;
    else if(m_iId == miWRD_YHed) dPitch = OM.DevInfo.dWf_PitchY;
    else if(m_iId == miWRD_XStg) dPitch = OM.DevInfo.dWf_PitchX;
    else if(m_iId == miWRD_YStg) dPitch = OM.DevInfo.dWf_PitchY;

         if(iUnitType == utJog   ) MT_JogN    ((EN_MOTR_ID)m_iId) ;
    else if(iUnitType == utMove  ) MT_GoIncMan((EN_MOTR_ID)m_iId , -dUnit ) ;
    else if(iUnitType == utPitch ) MT_GoIncMan((EN_MOTR_ID)m_iId , -dPitch) ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btJogNMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int m_iId = ((TBitBtn *)Sender) -> Tag;

    if(iUnitType == utJog   ) MT_Stop((EN_MOTR_ID)m_iId) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btJogPMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    MT_Stop((EN_MOTR_ID)m_iId) ;

         if(m_iId == miWRD_YIns) dPitch = OM.DevInfo.dRowPitch;
    else if(m_iId == miWRD_XHed) dPitch = OM.DevInfo.dWf_PitchX;
    else if(m_iId == miWRD_YHed) dPitch = OM.DevInfo.dWf_PitchY;
    else if(m_iId == miWRD_XStg) dPitch = OM.DevInfo.dWf_PitchX;
    else if(m_iId == miWRD_YStg) dPitch = OM.DevInfo.dWf_PitchY;

         if(iUnitType == utJog   ) MT_JogP    ((EN_MOTR_ID)m_iId) ;
    else if(iUnitType == utMove  ) MT_GoIncMan((EN_MOTR_ID)m_iId ,  dUnit ) ;
    else if(iUnitType == utPitch ) MT_GoIncMan((EN_MOTR_ID)m_iId ,  dPitch) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btJogPMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int m_iId = ((TBitBtn *)Sender) -> Tag;
    if(iUnitType == utJog   ) MT_Stop((EN_MOTR_ID)m_iId) ;
}
//---------------------------------------------------------------------------







typedef void (__closure *Func)();
void TFrmOperation::UpdateAray()
{
//    DM.UpdateAray();
}
void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
//    Trace("Operation Form","Update Start");
    tmUpdate->Enabled = false;
    SEQ.m_dLastTime = Now();

    /*
    static bool bFirst = true ;
    if(bFirst) SV_SetVisnForm(pnVisnBase -> Handle);
    bFirst = false ;
    */

    for(int i = 0 ; i < MAX_ARAY ; i++) {
        for(int j = 0 ; j < MAX_CHIP_STAT ; j++) {
            if(DM.ARAY[i].m_sMenuName[j] == "" || DM.ARAY[i].m_sMenuName[j] == NULL)
                ShowMessage(AnsiString(i) +"Aray " + AnsiString(j) + "Stat is null");
        }
    }


    //Oper Level Display
    LV_Display();

    //Lot Info Display
    LT_Display();

    //Error Display
    ER_Display();

    Panel5 -> Color = IO_GetX(xWRD_Vaccum) ? clLime :clGray ;




    //Option Display
    OP_Dispaly();

    //Manual Enabled
    pcManual -> Enabled = !SEQ._bRun ;


    lbLotNo1 -> Caption = DM.ARAY[riLDR].GetLotNo() ;
    lbLotNo2 -> Caption = DM.ARAY[riPRB].GetLotNo() ;
    lbLotNo3 -> Caption = DM.ARAY[riREF].GetLotNo() ;
    lbLotNo4 -> Caption = DM.ARAY[riRDF].GetLotNo() ;
    lbLotNo5 -> Caption = DM.ARAY[riPSF].GetLotNo() ;
    lbLotNo6 -> Caption = DM.ARAY[riULD].GetLotNo() ;

    lbId1    -> Caption = DM.ARAY[riLDR].GetID() ;
    lbId2    -> Caption = DM.ARAY[riPRB].GetID() ;
    lbId3    -> Caption = DM.ARAY[riREF].GetID() ;
    lbId4    -> Caption = DM.ARAY[riRDF].GetID() ;
    lbId5    -> Caption = DM.ARAY[riPSF].GetID() ;
    lbId6    -> Caption = DM.ARAY[riULD].GetID() ;

    lb1      -> Caption = STG._dLastWorkPosX;
    lb2      -> Caption = STG._dLastWorkPosY;
    lb3      -> Caption = STG._dLastWorkPosT;


    if(DM.ARAY[riSTG].GetID() == "9999") pnWafer -> Caption = "NOT HAVE MAP DATA"   ;
    else                                 pnWafer -> Caption = DM.ARAY[riSTG].GetID();

    if(OM.CmnOptn.bUseMapFile) {
        lbGoodCntWf  -> Caption = DM.ARAY[riSTG].GetCntStat(csGood ) + DM.ARAY[riSTG].GetCntStat(csUnkwn) + DM.ARAY[riSTG].GetCntStat(csWait) ;
        lbFailCntWf  -> Caption = DM.ARAY[riSTG].GetCntStat(csFail );
        lbEmptyCntWf -> Caption = DM.ARAY[riSTG].GetCntStat(csEmpty);
    }
    else {
        lbGoodCntWf  -> Caption = "" ;
        lbFailCntWf  -> Caption = "" ;
        lbEmptyCntWf -> Caption = "" ;
    }

    AnsiString sTemp2 ;
    sTemp2 = sTemp2.sprintf("%.3f",HED.m_dPickCycleTime);
    pnPickCycleTime -> Caption    = sTemp2 ;

    btDieVauum -> Font -> Color = IO_GetY(yHED_Vaccum) ? clLime : clBlack ;
    btDieVauum -> Caption       = IO_GetY(yHED_Vaccum) ? "ON"   : "OFF"   ;

    btEjtVacuum -> Font -> Color = IO_GetY(yEJT_Suction) ? clLime : clBlack ;
    btEjtVacuum -> Caption       = IO_GetY(yEJT_Suction) ? "ON"   : "OFF"   ;

    Label1 -> Caption = sgOutput1 -> Cells[1][sgOutput1 -> Row];

    btLotEnd -> Enabled = LT.m_bLotOpen ;

    //
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

void __fastcall TFrmOperation::btChipAlignClick(TObject *Sender)
{
    int iManNo = mcSTG_CycleCenterMove;
    MM.SetManCycle((EN_MANUAL_CYCLE)iManNo);
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btEjtVacuumClick(TObject *Sender)
{
    int iAdd = ((TBitBtn *)Sender) -> Tag ;



    IO_SetY(iAdd , !IO_GetY(iAdd));
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button16Click(TObject *Sender)
{
    if(AT_Complete(aiEJT_UpDn , ccBwd)) STG.MoveActr(aiEJT_UpDn , ccFwd );
    else                                STG.MoveActr(aiEJT_UpDn , ccBwd );

}
//---------------------------------------------------------------------------




void __fastcall TFrmOperation::tmUpdateArayTimer(TObject *Sender)
{
//    Trace("Operation Form","UpdateAray Start");
    tmUpdateAray->Enabled = false ;


    //if(!EM_IsErr())
    DM.UpdateAray();



    tmUpdateAray->Enabled = true ;
//    Trace("Operation Form","UpdateAray End");
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button20Click(TObject *Sender)
{
    STG.SetStartSet(false); //스타트셑을 강제로 오프시킴.

    DM.ARAY[riSTG].ChangeStat(csWait , csUnkwn);
    DM.ARAY[riSTG].ChangeStat(csGood , csUnkwn);



}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button21Click(TObject *Sender)
{
    STG.SetWfrTAligned(false); //얼라인 강제로 온시킴.
    STG.SetStartSet(false); //스타트셑을 강제로 온시킴.
	DM.ARAY[riSTG].ChangeStat(csWait , csUnkwn);
        DM.ARAY[riSTG].ChangeStat(csGood , csUnkwn);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btEPXPassClick(TObject *Sender)
{
    if(SEQ._bRun) return ;

    int r,c ;
    if( DM.ARAY[riRDF].GetCntStat(csMask )) {
        if(FM_MsgYesNo("Confirm","칩을 내려 놓을까요?(에폭시 페일)") == mrNo) {
            DM.ARAY[riRDF].FindLastColRow(csMask  , r , c) ;
            DM.ARAY[riRDF].SetStat(r,c,csWork );
            DM.ARAY[riRDV].SetStat(r,c,csRslt1);
            RAD.m_bEpxFailIgnr = false ;
            return ;
        }

    }
    if(!DM.ARAY[riRDF].GetCntStat(csWait)) return ;

//    int r,c ;
//    RAD.FindChip(riRAD,r,c);
//    if(DM.ARAY[riRDV].GetStat(r,c) == csRslt0) {
//        FM_MsgTime("CONFIRM","CHIP ORIENTATION FAIL",1000);
//        return ;
//    }

    DM.ARAY[riRDF].FindLastColRow(csWait  , r , c) ;

    if(FM_MsgYesNo("Confirm","칩을 내려 놓을까요?(에폭시 페일)") == mrYes) {

    //    if(DM.ARAY[riRDV].GetStat(r,c) == csRslt0) {
    //        if(FM_MsgYesNo("Confirm","CHIP ORIENTATION IS FAIL , EPOXY FAIL PASS?") != mrYes) return ;
    //    }
        DM.ARAY[riRDF].SetStat(r,c,csMask );
        DM.ARAY[riRDV].SetStat(r,c,csUnkwn);
        RAD.m_bEpxFailIgnr = true ;
    }
    else {
        DM.ARAY[riRDF].SetStat(r,c,csWork );
        DM.ARAY[riRDV].SetStat(r,c,csRslt1);
        RAD.m_bEpxFailIgnr = false ;
    }

}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::Button22Click(TObject *Sender)
{
    if(AT_Complete(aiTRS_UpDn , ccBwd)) STG.MoveActr(aiTRS_UpDn , ccFwd );
    else                                STG.MoveActr(aiTRS_UpDn , ccBwd );
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStopStgClick(TObject *Sender)
{
    MT_Stop(miWRD_XStg) ;
    MT_Stop(miWRD_YStg) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btStopHedClick(TObject *Sender)
{
    MT_Stop(miWRD_XHed) ;
    MT_Stop(miWRD_YHed) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSaveClick(TObject *Sender)
{
    if(!OM.CmnOptn.bUseMapFile) {FM_MsgOk("ERR" , "Use Map Mode에만 사용가능합니다."); return ;}
    if(!SaveDialog1->Execute()) return ;

    MAPU.SaveHanraMap(riSTG , SaveDialog1->FileName);

    Trace("Map Save",SaveDialog1->FileName.c_str()) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOpenClick(TObject *Sender)
{
    if(!OM.CmnOptn.bUseMapFile) {FM_MsgOk("ERR" , "Use Map Mode에만 사용가능합니다."); return ;}
    if(!OpenDialog1->Execute()) return ;

    String sPath = OpenDialog1->FileName;
    if(!FileExists(sPath)) {FM_MsgOk("ERR" , "존재하지 않는 Path입니다."); return ;}

    //DM.ARAY[riSTG].SetMaxColRow(iCol,iRow );

    if(sPath.Pos(".HRM")) {
        MAPU.LoadHanraMap(riSTG , sPath);
    }
    else if(OM.DevInfo.iMapType == 0){ if(!MAPU.ConvertCmn(sPath, riSTG)) return ;}
    else if(OM.DevInfo.iMapType == 1){ if(!MAPU.ConvertNxt(sPath, riSTG)) return ;}

    Trace("Map Open",sPath.c_str()) ;

    DM.ARAY[riSTG].ChangeStat(csWait , csUnkwn);
    DM.ARAY[riSTG].ChangeStat(csGood , csUnkwn);

    return ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOriPassClick(TObject *Sender)
{
    if(SEQ._bRun) return ;
    if(!DM.ARAY[riPRB].GetCntStat(csRslt0)) return ;

//    int r,c ;
//    RAD.FindChip(riRAD,r,c);
//    if(DM.ARAY[riRDV].GetStat(r,c) == csRslt0) {
//        FM_MsgTime("CONFIRM","CHIP ORIENTATION FAIL",1000);
//        return ;
//    }


    if(FM_MsgYesNo("Confirm","역삽을 무시 할까요?(오리엔테이션 페일)") == mrYes) {
        //PRB.bOriFailIgnr = true ;    의도는 알겠으나 그냥 에폭시 처럼 가는 게 실제로는 유용한듯.

        int r,c ;
        c = DM.ARAY[riPRB].FindFrstCol(csRslt0);
        if(c % 2) DM.ARAY[riPRB].FindFrstColLastRow(csRslt0 , r , c) ;
        else      DM.ARAY[riPRB].FindFrstColRow(csRslt0 , r , c) ;

        if(r > -1 && c > -1) DM.ARAY[riPRB].SetStat(r,c,csWork);





    }
}
//---------------------------------------------------------------------------







void __fastcall TFrmOperation::btPrbSkipClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","이보트는 작업을 하지 않겠습니까??") != mrYes) return ;

    TBitBtn *pBtn = dynamic_cast<TBitBtn *>(Sender);
    String sName = pBtn->Name;
    if(sName == "btPrbSkip"){
        if(!DM.ARAY[riPRB].CheckAllStat(csNone)) {
            PRB.m_bOriSkip = true ;
        }
    }
    else if(sName == "btWreSkip"){
        if(!DM.ARAY[riREF].CheckAllStat(csNone)) {

            DM.ARAY[riREV].SetStat(csRslt0);

            //DM.ARAY[riREV].ChangeStat(csUnkwn,csRslt0);
            //DM.ARAY[riREV].ChangeStat(csNone ,csRslt0);
            RAE.bRaeSkip = true ;
        }
    }
    else if(sName == "btWrdSkip"){
        if(!DM.ARAY[riRDF].CheckAllStat(csNone)) {
//            DM.ARAY[riRAD].ChangeStat(csUnkwn,csWork );
            DM.ARAY[riRDF].ChangeStat(csWait ,csWork );
            DM.ARAY[riRDF].ChangeStat(csWait2,csWork );
            DM.ARAY[riRDF].ChangeStat(csMask ,csWork );

            DM.ARAY[riRDV].SetStat(csRslt0);
            //DM.ARAY[riRDV].ChangeStat(csUnkwn,csRslt0);
            //DM.ARAY[riRDV].ChangeStat(csNone ,csRslt0);
        }
    }


}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btPrbClearClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","강제로 보트를 제거 하겠습니까?") != mrYes) return ;
    TBitBtn *pBtn = dynamic_cast<TBitBtn *>(Sender);
    String sName = pBtn->Name;
    if(sName == "btPrbClear"){
        DM.ARAY[riPRB].SetStat(csNone);
    }
    else if(sName == "btEpxClear"){
        DM.ARAY[riREF].SetStat(csNone);
        DM.ARAY[riREV].SetStat(csNone);
    }
    else if(sName == "btDieClear"){
        DM.ARAY[riRDF].SetStat(csNone);
        DM.ARAY[riRDV].SetStat(csNone);
    }
    else if(sName == "btPsbClear"){
        DM.ARAY[riPSF].SetStat(csNone);
        DM.ARAY[riPSV].SetStat(csNone);
    }


}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::BitBtn1Click(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","이번 보트는 방향검사에러를 무시하겠습니까?") != mrYes) return ;
    PRB.m_bBoatSkip = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button15Click(TObject *Sender)
{
    if(!OM.CmnOptn.bUseMapFile) {FM_MsgTime("Error","맵파일 모드에서만 사용가능!" , 500 ); return ;}
    if(FM_MsgYesNo( "Confirm","잉크다이만 작업하시겠습니까?") != mrYes) return ;

    DM.ARAY[riSTG].ChangeStat(csFail  , csRsltL);
    DM.ARAY[riSTG].ChangeStat(csUnkwn , csFail );
    DM.ARAY[riSTG].ChangeStat(csRsltL , csUnkwn);


}
//---------------------------------------------------------------------------







void __fastcall TFrmOperation::FormCreate(TObject *Sender)
{
    MAPU.SetMemo(mmMemo);




}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::Button26Click(TObject *Sender)
{
    UserFile.GridSearchFile(OM.CmnOptn.sSourcePath , sgOutput1 , 0 , false);   // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSourceFolderClick(TObject *Sender)
{
    String sDir = OM.CmnOptn.sSourcePath ;

    if(!DirectoryExists(sDir)) UserFile.CreateDir(sDir.c_str());

    HWND hwnd = NULL;
    LPCTSTR lpOperation = "open";
    LPCTSTR lpFile = NULL;
    LPCTSTR lpParameters = NULL;
    LPCTSTR lpDirectory = sDir.c_str();
    int nShowCmd = SW_SHOWDEFAULT;

    ShellExecute(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pcControlChange(TObject *Sender)
{
    if(pcControl -> TabIndex == 2){
        UserFile.GridSearchFile(OM.CmnOptn.sSourcePath , sgOutput1 , 0 , false);   // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btWorkedFolderClick(TObject *Sender)
{
    String sDir = OM.CmnOptn.sWorkedPath ;

    if(!DirectoryExists(sDir)) UserFile.CreateDir(sDir.c_str());

    HWND hwnd = NULL;
    LPCTSTR lpOperation = "open";
    LPCTSTR lpFile = NULL;
    LPCTSTR lpParameters = NULL;
    LPCTSTR lpDirectory = sDir.c_str();
    int nShowCmd = SW_SHOWDEFAULT;

    ShellExecute(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);

}
//---------------------------------------------------------------------------









void __fastcall TFrmOperation::Panel15Click(TObject *Sender)
{
    int r,c ;
    STG.FindFrstChip(r,c);

    int a ;
    a++;

}
//---------------------------------------------------------------------------

