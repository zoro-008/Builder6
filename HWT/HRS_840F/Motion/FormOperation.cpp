//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"
#include "FormDevice.h"
#include "Sequence.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "SLogUnit.h"

#include "ManualMan.h"
#include "SPCUnit.h"
#include "PstnMan.h"
#include "UserIni.h"

#include "Unloader.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)                       // 실행 시킬 때, 한 번만 실행하는 부분
    : TForm(Owner)
{

    DM.ARAY[riLDR_F].SetParent(pnLDR_F    );  DM.ARAY[riLDR_F].ClearDispItem();  DM.ARAY[riLDR_F].SetCellSize(false); DM.ARAY[riLDR_F].SetName("pnLDR_F    ");
    DM.ARAY[riLDR_R].SetParent(pnLDR_R    );  DM.ARAY[riLDR_R].ClearDispItem();  DM.ARAY[riLDR_R].SetCellSize(false); DM.ARAY[riLDR_R].SetName("pnLDR_R    ");
//    DM.ARAY[riPRE_F].SetParent(pnPRE_F    );  DM.ARAY[riPRE_F].ClearDispItem();  DM.ARAY[riPRE_F].SetCellSize(false); DM.ARAY[riPRE_F].SetName("pnPRE_F    ");
//    DM.ARAY[riPRE_R].SetParent(pnPRE_R    );  DM.ARAY[riPRE_R].ClearDispItem();  DM.ARAY[riPRE_R].SetCellSize(false); DM.ARAY[riPRE_R].SetName("pnPRE_R    ");
//    DM.ARAY[riPST_F].SetParent(pnPST_F    );  DM.ARAY[riPST_F].ClearDispItem();  DM.ARAY[riPST_F].SetCellSize(false); DM.ARAY[riPST_F].SetName("pnPST_F    ");
//    DM.ARAY[riPST_R].SetParent(pnPST_R    );  DM.ARAY[riPST_R].ClearDispItem();  DM.ARAY[riPST_R].SetCellSize(false); DM.ARAY[riPST_R].SetName("pnPST_R    ");
    DM.ARAY[riULD_F].SetParent(pnULD_F    );  DM.ARAY[riULD_F].ClearDispItem();  DM.ARAY[riULD_F].SetCellSize(false); DM.ARAY[riULD_F].SetName("pnULD_F    ");
    DM.ARAY[riULD_R].SetParent(pnULD_R    );  DM.ARAY[riULD_R].ClearDispItem();  DM.ARAY[riULD_R].SetCellSize(false); DM.ARAY[riULD_R].SetName("pnULD_R    ");
    DM.ARAY[riPCK_F].SetParent(pnULD_FPick);  DM.ARAY[riPCK_F].ClearDispItem();  DM.ARAY[riPCK_F].SetCellSize(false); DM.ARAY[riPCK_F].SetName("pnULD_FPick");
    DM.ARAY[riPCK_R].SetParent(pnULD_RPick);  DM.ARAY[riPCK_R].ClearDispItem();  DM.ARAY[riPCK_R].SetCellSize(false); DM.ARAY[riPCK_R].SetName("pnULD_RPick");


    DM.ARAY[riLDR_F].ClearDispItem();                    DM.ARAY[riLDR_R].ClearDispItem();
    DM.ARAY[riLDR_F].SetDispItem(csNone   ,"NoMask"  ) ; DM.ARAY[riLDR_R].SetDispItem(csNone   ,"NoMask"  ) ;
    DM.ARAY[riLDR_F].SetDispItem(csEmpty  ,"Empty"   ) ; DM.ARAY[riLDR_R].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riLDR_F].SetDispItem(csUnkwn  ,"Unknown" ) ; DM.ARAY[riLDR_R].SetDispItem(csUnkwn  ,"Unknown" ) ;

//    DM.ARAY[riPRE_F].ClearDispItem();                    DM.ARAY[riPRE_R].ClearDispItem();
//    DM.ARAY[riPRE_F].SetDispItem(csNone   ,"NoMask"  ) ; DM.ARAY[riPRE_R].SetDispItem(csNone   ,"NoMask"  ) ;
//    DM.ARAY[riPRE_F].SetDispItem(csUnkwn  ,"Unknown" ) ; DM.ARAY[riPRE_R].SetDispItem(csUnkwn  ,"Unknown" ) ;
//    DM.ARAY[riPRE_F].SetDispItem(csWork   ,"Work"    ) ; DM.ARAY[riPRE_R].SetDispItem(csWork   ,"Work"    ) ;

//    DM.ARAY[riPST_F].ClearDispItem();                    DM.ARAY[riPST_R].ClearDispItem();
//    DM.ARAY[riPST_F].SetDispItem(csNone   ,"NoMask"  ) ; DM.ARAY[riPST_R].SetDispItem(csNone   ,"NoMask"  ) ;
//    DM.ARAY[riPST_F].SetDispItem(csUnkwn  ,"Unknown" ) ; DM.ARAY[riPST_R].SetDispItem(csUnkwn  ,"Unknown" ) ;
//    DM.ARAY[riPST_F].SetDispItem(csWork   ,"Work"    ) ; DM.ARAY[riPST_R].SetDispItem(csWork   ,"Work"    ) ;

    DM.ARAY[riULD_F].ClearDispItem();                    DM.ARAY[riULD_R].ClearDispItem();
    DM.ARAY[riULD_F].SetDispItem(csNone   ,"None"    ) ; DM.ARAY[riULD_R].SetDispItem(csNone   ,"None"    ) ;
    DM.ARAY[riULD_F].SetDispItem(csUnkwn  ,"Unknown" ) ; DM.ARAY[riULD_R].SetDispItem(csUnkwn  ,"Unknown" ) ;
    DM.ARAY[riULD_F].SetDispItem(csWork   ,"csWork"  ) ; DM.ARAY[riULD_R].SetDispItem(csWork   ,"csWork"  ) ;

    DM.ARAY[riPCK_F].ClearDispItem();                    DM.ARAY[riPCK_R].ClearDispItem();
    DM.ARAY[riPCK_F].SetDispItem(csNone   ,"NoPack"  ) ; DM.ARAY[riPCK_R].SetDispItem(csNone   ,"NoPack"  ) ;
    DM.ARAY[riPCK_F].SetDispItem(csEmpty  ,"Empty"   ) ; DM.ARAY[riPCK_R].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riPCK_F].SetDispItem(csWork   ,"Work"    ) ; DM.ARAY[riPCK_R].SetDispItem(csWork   ,"Work"    ) ;

    DM.LoadMap();

    TUserINI   UserINI;                                                                     
    AnsiString sPath  ;                                                                     
    int iLang         ;                                                                     
    //Set Dir.                                                                              
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;                                               
                                                                                            
    UserINI.Load(sPath , "Member", "m_iLangSel"  , iLang);                                  
                                                                                            
    const bool bKorean = iLang == 1;                                                        
    iTemp = 0;                                                                              
                                                                                            
    sgDayInfo -> DoubleBuffered = true ;                                                    
    sgLotInfo -> DoubleBuffered = true ;

    //Manual Button Caption
    btMan1_1  -> Caption = "[" + AnsiString(mcLDR_FCycleWork       ) + "]" + (bKorean ? "F싸이클 워크"     : "F싸이클 워크"    );  btMan1_1  -> Tag = (int)mcLDR_FCycleWork       ; btMan1_1  -> Visible = true ;
    btMan1_2  -> Caption = "[" + AnsiString(mcLDR_FCycleSupply     ) + "]" + (bKorean ? "F싸이클 공급"     : "F싸이클 공급"    );  btMan1_2  -> Tag = (int)mcLDR_FCycleSupply     ; btMan1_2  -> Visible = true ;
    btMan1_3  -> Caption = "[" + AnsiString(mcLDR_FGrprFwBw        ) + "]" + (bKorean ? "F그리퍼 전후진"   : "F그리퍼 전후진"  );  btMan1_3  -> Tag = (int)mcLDR_FGrprFwBw        ; btMan1_3  -> Visible = true ;
    btMan1_4  -> Caption = "[" + AnsiString(mcLDR_FPickUpFwBw      ) + "]" + (bKorean ? "F픽 업다운"       : "F픽 업다운"      );  btMan1_4  -> Tag = (int)mcLDR_FPickUpFwBw      ; btMan1_4  -> Visible = true ;
    btMan1_5  -> Caption = "[" + AnsiString(mcLDR_FGrprUpDn        ) + "]" + (bKorean ? "F그리퍼 업다운"   : "F그리퍼 업다운"  );  btMan1_5  -> Tag = (int)mcLDR_FGrprUpDn        ; btMan1_5  -> Visible = true ;
    btMan1_8  -> Caption = "[" + AnsiString(mcLDR_FGrprXFwBw       ) + "]" + (bKorean ? "F그리퍼 X 전후진" : "F그리퍼 X 전후진");  btMan1_8  -> Tag = (int)mcLDR_FGrprXFwBw       ; btMan1_8  -> Visible = true ;
    btMan1_9  -> Caption = "[" + AnsiString(mcLDR_FLoadingFwBw     ) + "]" + (bKorean ? "F로딩 전후진"     : "F로딩 전후진"    );  btMan1_9  -> Tag = (int)mcLDR_FLoadingFwBw     ; btMan1_9  -> Visible = true ;
    btMan1_10 -> Caption = "[" + AnsiString(mcLDR_FAirBlow         ) + "]" + (bKorean ? "F에어블로우"      : "F에어블로우"     );  btMan1_10 -> Tag = (int)mcLDR_FAirBlow         ; btMan1_10  -> Visible = true ;
    btMan1_12 -> Caption = "[" + AnsiString(mcAllHome              ) + "]" + (bKorean ? "전체 홈"          : "전체 홈"         );  btMan1_12 -> Tag = (int)mcAllHome              ; btMan1_12 -> Visible = true ;

    btMan2_1  -> Caption = "[" + AnsiString(mcLDR_RCycleWork       ) + "]" + (bKorean ? "R싸이클 워크"     : "R싸이클 워크"    );  btMan2_1  -> Tag = (int)mcLDR_RCycleWork       ; btMan2_1  -> Visible = true ;
    btMan2_2  -> Caption = "[" + AnsiString(mcLDR_RCycleSupply     ) + "]" + (bKorean ? "R싸이클 공급"     : "R싸이클 공급"    );  btMan2_2  -> Tag = (int)mcLDR_RCycleSupply     ; btMan2_2  -> Visible = true ;
    btMan2_3  -> Caption = "[" + AnsiString(mcLDR_RGrprFwBw        ) + "]" + (bKorean ? "R그리퍼 전후진"   : "R그리퍼 전후진"  );  btMan2_3  -> Tag = (int)mcLDR_RGrprFwBw        ; btMan2_3  -> Visible = true ;
    btMan2_4  -> Caption = "[" + AnsiString(mcLDR_RPickUpFwBw      ) + "]" + (bKorean ? "R픽 업다운"       : "R픽 업다운"      );  btMan2_4  -> Tag = (int)mcLDR_RPickUpFwBw      ; btMan2_4  -> Visible = true ;
    btMan2_5  -> Caption = "[" + AnsiString(mcLDR_RGrprUpDn        ) + "]" + (bKorean ? "R그리퍼 업다운"   : "R그리퍼 업다운"  );  btMan2_5  -> Tag = (int)mcLDR_RGrprUpDn        ; btMan2_5  -> Visible = true ;
    btMan2_8  -> Caption = "[" + AnsiString(mcLDR_RGrprXFwBw       ) + "]" + (bKorean ? "R그리퍼 X 전후진" : "R그리퍼 X 전후진");  btMan2_8  -> Tag = (int)mcLDR_RGrprXFwBw       ; btMan2_8  -> Visible = true ;
    btMan2_9  -> Caption = "[" + AnsiString(mcLDR_RLoadingFwBw     ) + "]" + (bKorean ? "R로딩 전후진"     : "R로딩 전후진"    );  btMan2_9  -> Tag = (int)mcLDR_RLoadingFwBw     ; btMan2_9  -> Visible = true ;
    btMan2_10 -> Caption = "[" + AnsiString(mcLDR_RAirBlow         ) + "]" + (bKorean ? "R에어블로우"      : "R에어블로우"     );  btMan2_10 -> Tag = (int)mcLDR_RAirBlow         ; btMan2_10 -> Visible = true ;
    btMan2_12 -> Caption = "[" + AnsiString(mcAllHome              ) + "]" + (bKorean ? "전체 홈"          : "전체 홈"         );  btMan2_12 -> Tag = (int)mcAllHome              ; btMan2_12 -> Visible = true ;

    btMan3_1  -> Caption = "[" + AnsiString(mcRAL_FFstAirBlower    ) + "]" + (bKorean ? "F 1번 에어블로우" : "F 1번 에어블로우");  btMan3_1  -> Tag = (int)mcRAL_FFstAirBlower     ; btMan3_1  -> Visible = true ;
    btMan3_2  -> Caption = "[" + AnsiString(mcRAL_FSndAirBlower    ) + "]" + (bKorean ? "F 2번 에어블로우" : "F 2번 에어블로우");  btMan3_2  -> Tag = (int)mcRAL_FSndAirBlower     ; btMan3_2  -> Visible = true ;
//    btMan3_14 -> Caption = "[" + AnsiString(mcAllHome              ) + "]" + (bKorean ? "전체 홈"          : "ALL HOME"        );  btMan3_14 -> Tag = (int)mcAllHome              ; btMan3_14 -> Visible = true ;

    btMan4_1  -> Caption = "[" + AnsiString(mcRAL_RFstAirBlower    ) + "]" + (bKorean ? "R 1번 에어블로우" : "R 1번 에어 블로우");  btMan4_1  -> Tag = (int)mcRAL_RFstAirBlower     ; btMan4_1  -> Visible = true ;
    btMan4_2  -> Caption = "[" + AnsiString(mcRAL_RSndAirBlower    ) + "]" + (bKorean ? "R 2번 에어블로우" : "R 2번 에어 블로우");  btMan4_2  -> Tag = (int)mcRAL_RSndAirBlower     ; btMan4_2  -> Visible = true ;
//    btMan4_14 -> Caption = "[" + AnsiString(mcAllHome              ) + "]" + (bKorean ? "전체 홈"          : "ALL HOME");  btMan4_14 -> Tag = (int)mcAllHome              ; btMan4_14 -> Visible = true ;

    btMan5_1  -> Caption = "[" + AnsiString(mcULD_FCycleWork        ) + "]" + (bKorean ? "F싸이클 워크"     : "F싸이클 워크"   );  btMan5_1  -> Tag = (int)mcULD_FCycleWork    ; btMan5_1  -> Visible = true ;
    btMan5_2  -> Caption = "[" + AnsiString(mcULD_FCycleSupply      ) + "]" + (bKorean ? "F싸이클 공급"     : "F싸이클 공급"   );  btMan5_2  -> Tag = (int)mcULD_FCycleSupply  ; btMan5_2  -> Visible = true ;
    btMan5_3  -> Caption = "[" + AnsiString(mcULD_FCycleIn          ) + "]" + (bKorean ? "F싸이클 인"       : "F싸이클 인"     );  btMan5_3  -> Tag = (int)mcULD_FCycleIn      ; btMan5_3  -> Visible = true ;
    btMan5_4  -> Caption = "[" + AnsiString(mcULD_FFoldUpDn         ) + "]" + (bKorean ? "F폴드 업다운"     : "F폴드 업다운"   );  btMan5_4  -> Tag = (int)mcULD_FFoldUpDn     ; btMan5_4  -> Visible = true ;
    btMan5_5  -> Caption = "[" + AnsiString(mcULD_FPickUpFwBw       ) + "]" + (bKorean ? "F픽 업다운"       : "F픽 업다운"     );  btMan5_5  -> Tag = (int)mcULD_FPickUpFwBw   ; btMan5_5  -> Visible = true ;
    btMan5_8  -> Caption = "[" + AnsiString(mcULD_FRotateFwBw       ) + "]" + (bKorean ? "F로테이트 CwCcw"  : "F로테이트 CwCcw");  btMan5_8  -> Tag = (int)mcULD_FRotateFwBw   ; btMan5_8  -> Visible = true ;
    btMan5_9  -> Caption = "[" + AnsiString(mcULD_FPackOpenFwBw     ) + "]" + (bKorean ? "F팩 오픈 전후진"  : "F팩 오픈 전후진");  btMan5_9  -> Tag = (int)mcULD_FPackOpenFwBw ; btMan5_9  -> Visible = true ;
    btMan5_10 -> Caption = "[" + AnsiString(mcULD_FPackSrtFwBw      ) + "]" + (bKorean ? "F팩소터 전후진"   : "F팩소터 전후진" );  btMan5_10 -> Tag = (int)mcULD_FPackSrtFwBw  ; btMan5_10 -> Visible = true ;
    btMan5_12 -> Caption = "[" + AnsiString(mcAllHome               ) + "]" + (bKorean ? "전체 홈"          : "전체 홈"        );  btMan5_12 -> Tag = (int)mcAllHome           ; btMan5_12 -> Visible = true ;

    btMan6_1  -> Caption = "[" + AnsiString(mcULD_RCycleWork        ) + "]" + (bKorean ? "R싸이클 워크"     : "R싸이클 워크"   );  btMan6_1  -> Tag = (int)mcULD_RCycleWork    ; btMan6_1  -> Visible = true ;
    btMan6_2  -> Caption = "[" + AnsiString(mcULD_RCycleSupply      ) + "]" + (bKorean ? "R싸이클 공급"     : "R싸이클 공급"   );  btMan6_2  -> Tag = (int)mcULD_RCycleSupply  ; btMan6_2  -> Visible = true ;
    btMan6_3  -> Caption = "[" + AnsiString(mcULD_RCycleIn          ) + "]" + (bKorean ? "R싸이클 인"       : "R싸이클 인"     );  btMan6_3  -> Tag = (int)mcULD_RCycleIn      ; btMan6_3  -> Visible = true ;
    btMan6_4  -> Caption = "[" + AnsiString(mcULD_RFoldUpDn         ) + "]" + (bKorean ? "R폴드 업다운"     : "R폴드 업다운"   );  btMan6_4  -> Tag = (int)mcULD_RFoldUpDn     ; btMan6_4  -> Visible = true ;
    btMan6_5  -> Caption = "[" + AnsiString(mcULD_RPickUpFwBw       ) + "]" + (bKorean ? "R픽 업다운"       : "R픽 업다운"     );  btMan6_5  -> Tag = (int)mcULD_RPickUpFwBw   ; btMan6_5  -> Visible = true ;
    btMan6_8  -> Caption = "[" + AnsiString(mcULD_RRotateFwBw       ) + "]" + (bKorean ? "R로테이트 CwCcw"  : "R로테이트 CwCcw");  btMan6_8  -> Tag = (int)mcULD_RRotateFwBw   ; btMan6_8  -> Visible = true ;
    btMan6_9  -> Caption = "[" + AnsiString(mcULD_RPackOpenFwBw     ) + "]" + (bKorean ? "R팩 오픈 전후진"  : "R팩 오픈 전후진");  btMan6_9  -> Tag = (int)mcULD_RPackOpenFwBw ; btMan6_9  -> Visible = true ;
    btMan6_10 -> Caption = "[" + AnsiString(mcULD_RPackSrtFwBw      ) + "]" + (bKorean ? "R팩소터 전후진"   : "R팩소터 전후진" );  btMan6_10 -> Tag = (int)mcULD_RPackSrtFwBw  ; btMan6_10 -> Visible = true ;
    btMan6_12 -> Caption = "[" + AnsiString(mcAllHome               ) + "]" + (bKorean ? "전체 홈"          : "전체 홈"        );  btMan6_12 -> Tag = (int)mcAllHome           ; btMan6_12 -> Visible = true ;


    pnFMaskDetect -> Tag = (int)xLDR_FMaskDetect;
    pnFOpenVccm   -> Tag = (int)xULD_FPackVccm ;
    pnFPickVccm   -> Tag = (int)xULD_FPickVccm ;

    btLdrAirBlw_F -> Tag = (int)yLDR_FAirBlowOn ;
    btAirBlwr1_F  -> Tag = (int)yRAL_FFoldAirBlow1;
    btAirBlwr2_F  -> Tag = (int)yRAL_FFoldAirBlow2;
    btFPickVccm   -> Tag = (int)yULD_FPickUpVccm;
    btFPickEjct   -> Tag = (int)yULD_FPickUpEjct;
    btFPackVccm   -> Tag = (int)yULD_FPackOpenVccm;
    btFPackEjct   -> Tag = (int)yULD_FPackOpenEjct;

    pnRMaskDetect -> Tag = (int)xLDR_RMaskDetect;
    pnROpenVccm   -> Tag = (int)xULD_RPackVccm ;
    pnRPickVccm   -> Tag = (int)xULD_RPickVccm ;

    btLdrAirBlw_R -> Tag = (int)yLDR_RAirBlowOn ;
    btAirBlwr1_R  -> Tag = (int)yRAL_RFoldAirBlow1;
    btAirBlwr2_R  -> Tag = (int)yRAL_RFoldAirBlow2;
    btRPickVccm   -> Tag = (int)yULD_RPickUpVccm;
    btRPickEjct   -> Tag = (int)yULD_RPickUpEjct;
    btRPackVccm   -> Tag = (int)yULD_RPackOpenVccm;
    btRPackEjct   -> Tag = (int)yULD_RPackOpenEjct;



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

void __fastcall TFrmOperation::btAllHomeClick(TObject *Sender)
{
    MM.SetManCycle(mcAllHome);        
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPasswordCloseClick(TObject *Sender)
{
    plPassWord -> Visible = false ;        
}
//---------------------------------------------------------------------------                                     

void __fastcall TFrmOperation::btEngrClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;        
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnLoadingStopDblClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

    if(FM_GetLevel() == lvOperator) return ;

    if (FM_MsgYesNo("Confirm", "Would you like to invert the Option?") != mrYes) return ;

    switch(iIndex) {
        case 0 :                                                           break ;
        case 1 : OM.CmnOptn.bLoadingStop   = !OM.CmnOptn.bLoadingStop    ; break ;
        case 2 : OM.CmnOptn.bFRunSkip      = !OM.CmnOptn.bFRunSkip       ; break ;
        case 3 : OM.CmnOptn.bRRunSkip      = !OM.CmnOptn.bRRunSkip       ; break ;

    }

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false;
    Trace("<@SETBUFF>", "1,FormOperTimer Stt");
                                                     
    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "오퍼레이터"; break ;
        case lvEngineer : btOperator -> Caption = " 엔지니어 "; break ;
        case lvMaster   : btOperator -> Caption = "  마스터  "; break ;
        default         : btOperator -> Caption = "   에러   "; break ;
    }

    if(OM.DevOptn.iEpmtyBin != 1) { pnBinCnt1 -> Caption = ULD_F.GetBin1Cnt(); }
    if(OM.DevOptn.iEpmtyBin != 2) { pnBinCnt2 -> Caption = ULD_F.GetBin2Cnt(); }

    if(OM.DevOptn.iEpmtyBin == 1) { pnBinCnt1 -> Caption = "비워 있음";}
    if(OM.DevOptn.iEpmtyBin == 2) { pnBinCnt2 -> Caption = "비워 있음";}

    // Tower Lamp 관련. yETC_BuzzerVoice
    if(SEQ._iSeqStat == ssInit    &&  bFlip ) {lbRed -> Visible = false; lbYel -> Visible = false; lbGrn -> Visible = false;}

    if(SEQ._iSeqStat == ssError   &&  bFlip ) {lbRed -> Visible = false; lbYel -> Visible = true ; lbGrn -> Visible = true ;}

    if(SEQ._iSeqStat == ssRunning &&  bFlip ) {lbRed -> Visible = true ; lbYel -> Visible = true ; lbGrn -> Visible = false;}

    if(SEQ._iSeqStat == ssStop    &&  bFlip ) {lbRed -> Visible = false; lbYel -> Visible = true ; lbGrn -> Visible = true ;}

    if(!bFlip) {lbRed -> Visible = true; lbYel -> Visible = true; lbGrn -> Visible = true;                                  }

    pnLoadingStop -> Color = OM.CmnOptn.bLoadingStop   ? clLime : clRed ;
    pnFRunSkip    -> Color = OM.CmnOptn.bFRunSkip      ? clLime : clRed ;
    pnRRunSkip    -> Color = OM.CmnOptn.bRRunSkip      ? clLime : clRed ;
    pnRMaskDetect -> Color = IO_GetX(xLDR_RMaskDetect) ? clLime : clRed ;
    pnFMaskDetect -> Color = IO_GetX(xLDR_FMaskDetect) ? clLime : clRed ;
    pnROpenVccm   -> Color = IO_GetX(xULD_RPackVccm)   ? clLime : clRed ;
    pnFOpenVccm   -> Color = IO_GetX(xULD_FPackVccm)   ? clLime : clRed ;
    pnRPickVccm   -> Color = IO_GetX(xULD_RPickVccm)   ? clLime : clRed ;
    pnFPickVccm   -> Color = IO_GetX(xULD_FPickVccm)   ? clLime : clRed ;

    pnRMaskDetect -> Color = IO_GetX(xLDR_RMaskDetect) ? clLime : clRed ;
    pnROpenVccm   -> Color = IO_GetX(xULD_RPackVccm)   ? clLime : clRed ;
    pnRPickVccm   -> Color = IO_GetX(xULD_RPickVccm)   ? clLime : clRed ;

    pnLoadingStop -> Caption = OM.CmnOptn.bLoadingStop   ? "ON" : "OFF" ;
    pnFRunSkip    -> Caption = OM.CmnOptn.bFRunSkip      ? "ON" : "OFF" ;
    pnRRunSkip    -> Caption = OM.CmnOptn.bRRunSkip      ? "ON" : "OFF" ;
    
    pnFMaskDetect -> Caption = IO_GetX(xLDR_FMaskDetect) ? "ON" : "OFF" ;
    pnFOpenVccm   -> Caption = IO_GetX(xULD_FPackVccm)   ? "ON" : "OFF" ;
    pnFPickVccm   -> Caption = IO_GetX(xULD_FPickVccm)   ? "ON" : "OFF" ;

    pnRMaskDetect -> Caption = IO_GetX(xLDR_RMaskDetect) ? "ON" : "OFF" ;
    pnFMaskDetect -> Caption = IO_GetX(xLDR_FMaskDetect) ? "ON" : "OFF" ;
    pnROpenVccm   -> Caption = IO_GetX(xULD_RPackVccm)   ? "ON" : "OFF" ;
    pnFOpenVccm   -> Caption = IO_GetX(xULD_FPackVccm)   ? "ON" : "OFF" ;
    pnRPickVccm   -> Caption = IO_GetX(xULD_RPickVccm)   ? "ON" : "OFF" ;
    pnFPickVccm   -> Caption = IO_GetX(xULD_FPickVccm)   ? "ON" : "OFF" ;

    btLdrAirBlw_F -> Font -> Color = IO_GetY(yLDR_FAirBlowOn   ) ? clLime : clBlack ;
    btAirBlwr1_F  -> Font -> Color = IO_GetY(yRAL_FFoldAirBlow1) ? clLime : clBlack ;
    btAirBlwr2_F  -> Font -> Color = IO_GetY(yRAL_FFoldAirBlow2) ? clLime : clBlack ;
    btFPickVccm   -> Font -> Color = IO_GetY(yULD_FPickUpVccm  ) ? clLime : clBlack ;
    btFPickEjct   -> Font -> Color = IO_GetY(yULD_FPickUpEjct  ) ? clLime : clBlack ;
    btFPackVccm   -> Font -> Color = IO_GetY(yULD_FPackOpenVccm) ? clLime : clBlack ;
    btFPackEjct   -> Font -> Color = IO_GetY(yULD_FPackOpenEjct) ? clLime : clBlack ;

    btLdrAirBlw_R -> Font -> Color = IO_GetY(yLDR_RAirBlowOn   ) ? clLime : clBlack ;
    btAirBlwr1_R  -> Font -> Color = IO_GetY(yRAL_RFoldAirBlow1) ? clLime : clBlack ;
    btAirBlwr2_R  -> Font -> Color = IO_GetY(yRAL_RFoldAirBlow2) ? clLime : clBlack ;
    btRPickVccm   -> Font -> Color = IO_GetY(yULD_RPickUpVccm  ) ? clLime : clBlack ;
    btRPickEjct   -> Font -> Color = IO_GetY(yULD_RPickUpEjct  ) ? clLime : clBlack ;
    btRPackVccm   -> Font -> Color = IO_GetY(yULD_RPackOpenVccm) ? clLime : clBlack ;
    btRPackEjct   -> Font -> Color = IO_GetY(yULD_RPackOpenEjct) ? clLime : clBlack ;

    btLotEnd  -> Enabled = LOT.GetLotOpen() ;
    btStart   -> Enabled = LOT.GetLotOpen() ;
    btLotOpen -> Enabled =!LOT.GetLotOpen() ;

    Trace("<@SETBUFF>","1,FormOperTimer 1");
    SPC.LOT.DispLotInfo(sgLotInfo);
    SPC.DAY.DispDayInfo(sgDayInfo);

    //EM_Disp
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
                Str += EM_GetErrName(i) + AnsiString(" ") + EM_GetErrMsg(i);
                lbErr -> Items -> Add(Str);
                if (iErrNo == i) lbErr -> ItemIndex = lbErr -> Count - 1 ;
            }
        }
    }

    iPreErrCnt = iCrntErrCnt ;

    Trace("<@SETBUFF>","1,FormOperTimer 2");

    tmUpdate -> Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btRPackVccmClick(TObject *Sender)
{
    int iIONo = ((TBitBtn *)Sender) -> Tag ;
    IO_SetY(iIONo , !IO_GetY(iIONo));
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
    SEQ.Reset();
    FrmLotOpen -> ShowModal() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotEndClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Do you really want to LotEnd?") != mrYes) return ;

    LOT.LotEnd ();

    DM.ARAY[riLDR_F].SetStat(csNone);
    DM.ARAY[riLDR_R].SetStat(csNone);
    DM.ARAY[riPCK_F].SetStat(csNone);
    DM.ARAY[riPCK_R].SetStat(csNone);
//    DM.ARAY[riPRE_F].SetStat(csNone);
//    DM.ARAY[riPRE_R].SetStat(csNone);
//    DM.ARAY[riPST_F].SetStat(csNone);
//    DM.ARAY[riPST_R].SetStat(csNone);
    DM.ARAY[riULD_F].SetStat(csNone);
    DM.ARAY[riULD_R].SetStat(csNone);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btMastClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::pnLoadingStopClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

    if (FM_MsgYesNo("Confirm", "Would you like to invert the flag?") != mrYes) return ;

    switch(iIndex) {
        case 0 : OM.CmnOptn.bLoadingStop = !OM.CmnOptn.bLoadingStop ; break ;
        case 1 : OM.CmnOptn.bFRunSkip    = !OM.CmnOptn.bFRunSkip    ; break ;
        case 2 : OM.CmnOptn.bRRunSkip    = !OM.CmnOptn.bRRunSkip    ; break ;

    }

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::pnBinCnt2DblClick(TObject *Sender)
{
    if( FM_MsgYesNo( "Confirm" , "2번 Bin을 비우시겠습니까?") != mrYes) return ;

    ULD_F.ClearBin2Cnt();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnBinCnt1DblClick(TObject *Sender)
{
    if( FM_MsgYesNo( "Confirm" , "1번 Bin을 비우시겠습니까?") != mrYes) return ;

    ULD_F.ClearBin1Cnt();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btAllBinClearClick(TObject *Sender)
{
    if( FM_MsgYesNo( "Confirm" , "모든 Bin을 비우시겠습니까?") != mrYes) return ;

    ULD_F.ClearBinCnt();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmTowerLampUpdate(TObject *Sender)
{
    tmTowerLamp -> Enabled = false ;

    bFlip = !bFlip;

    tmTowerLamp -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
    SEQ._bBtnStart = true ;
}
//---------------------------------------------------------------------------

