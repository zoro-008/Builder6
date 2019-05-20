//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"

#include "PreBuff.h"
//#include "Loader.h"

#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Sequence.h"
//#include "CommUnit.h"
#include "FormProbe.h"
#include "ProbeComUnit.h"
#include "ManualMan.h"
#include "SPCUnit.h"
#include "PstnMan.h"
#include "Loader.h"
#include "SortingTool.h"
#include "WorkZone.h"
#include "LanguageUnit.h"
#include "PostBuffer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)
    : TForm(Owner)
{
    DM.ARAY[riLD1].SetParent(pnLoader   ); DM.ARAY[riLD1].SetConfig(false , "pnLoader    "); DM.ARAY[riLD1].ClearDispItem();
    DM.ARAY[riPR1].SetParent(pnPreBuff1 ); DM.ARAY[riPR1].SetConfig(false , "pnPreBuff1  "); DM.ARAY[riPR1].ClearDispItem();
    DM.ARAY[riPR2].SetParent(pnPreBuff2 ); DM.ARAY[riPR2].SetConfig(false , "pnPreBuff2  "); DM.ARAY[riPR2].ClearDispItem();
    DM.ARAY[riWRP].SetParent(pnWorkZone1); DM.ARAY[riWRP].SetConfig(false , "pnWorkZone1 "); DM.ARAY[riWRP].ClearDispItem();
    DM.ARAY[riWRK].SetParent(pnWorkZone2); DM.ARAY[riWRK].SetConfig(false , "pnWorkZone2 "); DM.ARAY[riWRK].ClearDispItem();
    DM.ARAY[riPS1].SetParent(pnPostBuff1); DM.ARAY[riPS1].SetConfig(false , "pnPostBuff1 "); DM.ARAY[riPS1].ClearDispItem();
    DM.ARAY[riPS2].SetParent(pnPostBuff2); DM.ARAY[riPS2].SetConfig(false , "pnPostBuff2 "); DM.ARAY[riPS2].ClearDispItem();
    DM.ARAY[riULD].SetParent(pnUnLoader ); DM.ARAY[riULD].SetConfig(false , "pnUnLoader  "); DM.ARAY[riULD].ClearDispItem();

    DM.ARAY[riLD1 ].SetDispItem(csEmpty  ,"Empty "     ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite      );
    DM.ARAY[riLD1 ].SetDispItem(csUnkwn  ,"Unkwn "     ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen );
    DM.ARAY[riLD1 ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riLDR ].SetDispColor(csNone  ,clWhite      );

    DM.ARAY[riPR1 ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riPRB ].SetDispColor(csNone  ,clWhite      );
    DM.ARAY[riPR1 ].SetDispItem(csUnkwn  ,"Unkwn "     ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen );
    DM.ARAY[riPR1 ].SetDispItem(csToBuf  ,"ToBuf "     ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riPR1 ].SetDispItem(csFromBuf,"FromBuf "   ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riPR2 ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riPRB ].SetDispColor(csNone  ,clWhite      );
    DM.ARAY[riPR2 ].SetDispItem(csUnkwn  ,"Unkwn "     ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen );
    DM.ARAY[riPR2 ].SetDispItem(csToBuf  ,"ToBuf "     ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riPR2 ].SetDispItem(csFromBuf,"FromBuf "   ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riWRP ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csNone  ,clWhite      );
    DM.ARAY[riWRP ].SetDispItem(csUnkwn  ,"Unkwn "     ) ; //DM.ARAY[riWK1 ].SetDispColor(csUnkwn ,clMoneyGreen );
    DM.ARAY[riWRP ].SetDispItem(csWork   ,"Work "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csUnkwn ,clMoneyGreen );
    DM.ARAY[riWRP ].SetDispItem(csGood   ,"Good "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riWRP ].SetDispItem(csFail   ,"Fail "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riWRP ].SetDispItem(csToBuf  ,"ToBuf "     ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riWRP ].SetDispItem(csFromBuf,"FromBuf "   ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riWRK ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csNone  ,clWhite      );
    DM.ARAY[riWRK ].SetDispItem(csUnkwn  ,"Unkwn "     ) ; //DM.ARAY[riWK1 ].SetDispColor(csUnkwn ,clMoneyGreen );
    DM.ARAY[riWRK ].SetDispItem(csWork   ,"Work "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csUnkwn ,clMoneyGreen );
    DM.ARAY[riWRK ].SetDispItem(csGood   ,"Good "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riWRK ].SetDispItem(csFail   ,"Fail "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riWRK ].SetDispItem(csToBuf  ,"ToBuf "     ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riWRK ].SetDispItem(csFromBuf,"FromBuf "   ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riPS1 ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riPSB ].SetDispColor(csNone  ,clWhite      );
    DM.ARAY[riPS1 ].SetDispItem(csGood   ,"Good "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riPS1 ].SetDispItem(csFail   ,"Fail "      ) ; //DM.ARAY[riPSB ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riPS1 ].SetDispItem(csEmpty  ,"Empty "     ) ; //DM.ARAY[riPSB ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riPS1 ].SetDispItem(csWork   ,"Work "      ) ; //DM.ARAY[riPSB ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riPS1 ].SetDispItem(csMask   ,"Mask "      ) ; //DM.ARAY[riPSB ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riPS1 ].SetDispItem(csToBuf  ,"ToBuf "     ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riPS1 ].SetDispItem(csFromBuf,"FromBuf "   ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riPS2 ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riPSB ].SetDispColor(csNone  ,clWhite      );
    DM.ARAY[riPS2 ].SetDispItem(csGood   ,"Good "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riPS2 ].SetDispItem(csFail   ,"Fail "      ) ; //DM.ARAY[riPSB ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riPS2 ].SetDispItem(csEmpty  ,"Empty "     ) ; //DM.ARAY[riPSB ].SetDispColor(csFail  ,clPurple     );
    DM.ARAY[riPS2 ].SetDispItem(csToBuf  ,"ToBuf "     ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riPS2 ].SetDispItem(csFromBuf,"FromBuf "   ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riULD ].SetDispItem(csEmpty  ,"Empty "     ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite      );
    DM.ARAY[riULD ].SetDispItem(csWork   ,"Work "     ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen );
    DM.ARAY[riULD ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riLDR ].SetDispColor(csNone  ,clWhite      );

    //Default Set.
    clStat[csRslt0 ] = clInactiveCaption ;//csInactiveCaption
    clStat[csRslt1 ] = clBlack           ;//csMaroon
    clStat[csRslt2 ] = clRed             ;//csPurple
    clStat[csRslt3 ] = clGreen           ;//csTeal

    clStat[csNone  ] = clWhite           ;//csWhite
    clStat[csEmpty ] = clSilver          ;//csSilver
    clStat[csUnkwn ] = clMoneyGreen      ;//csAqua
    clStat[csWork  ] = (TColor)0x00FF9595;//clHighlight       ;//csBlue
//    clStat[csWork  ] = clHighlight       ;//csBlue
    clStat[csFail  ] = clPurple          ;//csFuchsia
    clStat[csEtc   ] = clNone            ;//etc*/

    //Manual Button Caption
    btMan1_1  -> Caption = "[" + AnsiString(mcLDR_Home          ) + "]" + "Loading home"     ;  btMan1_1  -> Tag = (int)mcLDR_Home           ; btMan1_1  -> Visible = true  ;
    btMan1_2  -> Caption = "[" + AnsiString(mcLDR_Supply        ) + "]" + "Loading Supply"   ;  btMan1_2  -> Tag = (int)mcLDR_Supply         ; btMan1_2  -> Visible = true  ;
    btMan1_3  -> Caption = "[" + AnsiString(mcLDR_Pick          ) + "]" + "Loading Pick"     ;  btMan1_3  -> Tag = (int)mcLDR_Pick           ; btMan1_3  -> Visible = true  ;
    btMan1_4  -> Caption = "[" + AnsiString(mcLDR_SttPstn       ) + "]" + "Move Start Pos"   ;  btMan1_4  -> Tag = (int)mcLDR_SttPstn        ; btMan1_4  -> Visible = true  ;
    btMan1_5  -> Caption = "[" + AnsiString(mcLDR_Place         ) + "]" + "Loading Place"    ;  btMan1_5  -> Tag = (int)mcLDR_Place          ; btMan1_5  -> Visible = true  ;
    btMan1_6  -> Caption = "[" + AnsiString(mcLDR_UpSlPitch     ) + "]" + "Pitch Up"         ;  btMan1_6  -> Tag = (int)mcLDR_UpSlPitch      ; btMan1_6  -> Visible = true  ;
    btMan1_7  -> Caption = "[" + AnsiString(mcLDR_DnSlPitch     ) + "]" + "Pitch Dn"         ;  btMan1_7  -> Tag = (int)mcLDR_DnSlPitch      ; btMan1_7  -> Visible = true  ;
    btMan1_8  -> Caption = "[" + AnsiString(mcLDR_PusherFw      ) + "]" + "Pusher Fwd"       ;  btMan1_8  -> Tag = (int)mcLDR_PusherFw       ; btMan1_8  -> Visible = true  ;
    btMan1_9  -> Caption = "[" + AnsiString(mcLDR_PusherBw      ) + "]" + "Pusher Bwd"       ;  btMan1_9  -> Tag = (int)mcLDR_PusherBw       ; btMan1_9  -> Visible = true  ;
    btMan1_10 -> Caption = "[" + AnsiString(mcLDR_UnClamp       ) + "]" + "Clamp UP"         ;  btMan1_10 -> Tag = (int)mcLDR_UnClamp        ; btMan1_10 -> Visible = true  ;
    btMan1_11 -> Caption = "[" + AnsiString(mcLDR_Clamp         ) + "]" + "Clamp DN"         ;  btMan1_11 -> Tag = (int)mcLDR_Clamp          ; btMan1_11 -> Visible = true  ;
    btMan1_12 -> Caption = "[" + AnsiString(mcLDR_TranInAC      ) + "]" + "Feeding In"       ;  btMan1_12 -> Tag = (int)mcLDR_TranInAC       ; btMan1_12 -> Visible = false ;
    btMan1_13 -> Caption = "[" + AnsiString(mcLDR_TranOutAC     ) + "]" + "Feeding Out"      ;  btMan1_13 -> Tag = (int)mcLDR_TranOutAC      ; btMan1_13 -> Visible = false ;
    btMan1_14 -> Caption = "[" + AnsiString(mcAllHome           ) + "]" + "ALL HOME"         ;  btMan1_14 -> Tag = (int)mcAllHome            ; btMan1_14 -> Visible = true  ;

    btMan2_1  -> Caption = "[" + AnsiString(mcPRE_Home          ) + "]" + "PreRail Home"     ;  btMan2_1  -> Tag = (int)mcPRE_Home           ; btMan2_1  -> Visible = true  ;
    btMan2_2  -> Caption = "[" + AnsiString(mcPRE_Clamp         ) + "]" + "Clamp UP/DN"      ;  btMan2_2  -> Tag = (int)mcPRE_Clamp          ; btMan2_2  -> Visible = true  ;
    btMan2_3  -> Caption = "[" + AnsiString(mcPRE_IDCyl         ) + "]" + "ID Cyl UP/DN"     ;  btMan2_3  -> Tag = (int)mcPRE_IDCyl          ; btMan2_3  -> Visible = true  ;
    btMan2_4  -> Caption = "[" + AnsiString(mcPRE_Feeding       ) + "]" + "Feeding On/Off"   ;  btMan2_4  -> Tag = (int)mcPRE_Feeding        ; btMan2_4  -> Visible = true  ;
    btMan2_5  -> Caption = "[" + AnsiString(mcPRE_In            ) + "]" + "PreRail In"       ;  btMan2_5  -> Tag = (int)mcPRE_In             ; btMan2_5  -> Visible = false ;
    btMan2_6  -> Caption = "[" + AnsiString(mcPRE_Drag          ) + "]" + "PreRail Drag"     ;  btMan2_6  -> Tag = (int)mcPRE_Drag           ; btMan2_6  -> Visible = false ;
    btMan2_7  -> Caption = "[" + AnsiString(mcPRE_Out           ) + "]" + "PreRail Out"      ;  btMan2_7  -> Tag = (int)mcPRE_Out            ; btMan2_7  -> Visible = false ;
    btMan2_14 -> Caption = "[" + AnsiString(mcAllHome           ) + "]" + "ALL HOME"         ;  btMan2_14 -> Tag = (int)mcAllHome            ; btMan2_14 -> Visible = true  ;
/*
    btMan2_1  -> Caption = "[" + AnsiString(mcPRE_Home          ) + "]" + "PreRail Home"     ;  btMan2_1  -> Tag = (int)mcPRE_Home           ; btMan2_1  -> Visible = true ;
    btMan2_2  -> Caption = "[" + AnsiString(mcPRE_In            ) + "]" + "PreRail In"       ;  btMan2_2  -> Tag = (int)mcPRE_In             ; btMan2_2  -> Visible = true ;
    btMan2_3  -> Caption = "[" + AnsiString(mcPRE_Drag          ) + "]" + "PreRail Drag"     ;  btMan2_3  -> Tag = (int)mcPRE_Drag           ; btMan2_3  -> Visible = true ;
    btMan2_4  -> Caption = "[" + AnsiString(mcPRE_Out           ) + "]" + "PreRail Out"      ;  btMan2_4  -> Tag = (int)mcPRE_Out            ; btMan2_4  -> Visible = true ;
    btMan2_5  -> Caption = "[" + AnsiString(mcPRE_Clamp         ) + "]" + "Clamp U/D"        ;  btMan2_5  -> Tag = (int)mcPRE_Clamp          ; btMan2_5  -> Visible = true ;
    btMan2_6  -> Caption = "[" + AnsiString(mcPRE_IDCyl         ) + "]" + "ID Cyl U/D"       ;  btMan2_6  -> Tag = (int)mcPRE_IDCyl          ; btMan2_6  -> Visible = true ;
    btMan2_7  -> Caption = "[" + AnsiString(mcPRE_Feeding       ) + "]" + "Feeding On/Off"   ;  btMan2_7  -> Tag = (int)mcPRE_Feeding        ; btMan2_7  -> Visible = true ;
    btMan2_8  -> Caption = "[" + AnsiString(mcAllHome           ) + "]" + "ALL HOME"         ;  btMan2_8  -> Tag = (int)mcAllHome            ; btMan2_8  -> Visible = true ;
*/

    btMan3_1  -> Caption = "[" + AnsiString(mcWRK_Home          ) + "]" + "Probe Home"       ;  btMan3_1  -> Tag = (int)mcWRK_Home           ; btMan3_1  -> Visible = true  ;
    btMan3_2  -> Caption = "[" + AnsiString(mcWRK_R1Pitch       ) + "]" + "Go Right 1 Pitch" ;  btMan3_2  -> Tag = (int)mcWRK_R1Pitch        ; btMan3_2  -> Visible = true  ;
    btMan3_3  -> Caption = "[" + AnsiString(mcWRK_L1Pitch       ) + "]" + "Go Left 1 Pitch"  ;  btMan3_3  -> Tag = (int)mcWRK_L1Pitch        ; btMan3_3  -> Visible = true  ;
    btMan3_4  -> Caption = "[" + AnsiString(mcWRK_Clamp         ) + "]" + "Probe Clamp UP/DN";  btMan3_4  -> Tag = (int)mcWRK_Clamp          ; btMan3_4  -> Visible = true  ;
    btMan3_5  -> Caption = "[" + AnsiString(mcWRK_ToolChng      ) + "]" + "Tool Change "     ;  btMan3_5  -> Tag = (int)mcWRK_ToolChng       ; btMan3_5  -> Visible = true  ;
    btMan3_14 -> Caption = "[" + AnsiString(mcAllHome           ) + "]" + "ALL HOME"         ;  btMan3_14 -> Tag = (int)mcAllHome            ; btMan3_14 -> Visible = true  ;
    btMan3_7  -> Caption = "[" + AnsiString(mcWRK_PostWork      ) + "]" + "Probe Out"        ;  btMan3_7  -> Tag = (int)mcWRK_PostWork       ; btMan3_7  -> Visible = false ;
    btMan3_8  -> Caption = "[" + AnsiString(mcWRK_PreWork       ) + "]" + "Probe In"         ;  btMan3_8  -> Tag = (int)mcWRK_PreWork        ; btMan3_8  -> Visible = false ;
//  btMan3_3  -> Caption = "[" + AnsiString(mcWRK_Work          ) + "]" + "Probe Work"       ;  btMan3_3  -> Tag = (int)mcWRK_Work           ; btMan3_3  -> Visible = false;
//  btMan3_4  -> Caption = "[" + AnsiString(mcWRK_ReGrip        ) + "]" + "Probe ReGrip"     ;  btMan3_4  -> Tag = (int)mcWRK_ReGrip         ; btMan3_4  -> Visible = false;
//  btMan3_5  -> Caption = "[" + AnsiString(mcWRK_ReWork        ) + "]" + "Probe ReWork"     ;  btMan3_5  -> Tag = (int)mcWRK_ReWork         ; btMan3_5  -> Visible = true ;


    btMan4_1  -> Caption = "[" + AnsiString(mcPSB_Home          ) + "]" + "PostRail Home"          ;  btMan4_1  -> Tag = (int)mcPSB_Home           ; btMan4_1  -> Visible = true  ;
    btMan4_2  -> Caption = "[" + AnsiString(mcPSB_Clamp         ) + "]" + "Clamp U/D"              ;  btMan4_2  -> Tag = (int)mcPSB_Clamp          ; btMan4_2  -> Visible = true  ;
    btMan4_3  -> Caption = "[" + AnsiString(mcPSB_Feeding       ) + "]" + "Feeding On/Off"         ;  btMan4_3  -> Tag = (int)mcPSB_Feeding        ; btMan4_3  -> Visible = true  ;
    btMan4_4  -> Caption = "[" + AnsiString(mcPSB_R1Pitch       ) + "]" + "Go Right 1 Pitch"       ;  btMan4_4  -> Tag = (int)mcPSB_R1Pitch        ; btMan4_4  -> Visible = true  ;
    btMan4_5  -> Caption = "[" + AnsiString(mcPSB_L1Pitch       ) + "]" + "Go Left 1 Pitch"        ;  btMan4_5  -> Tag = (int)mcPSB_L1Pitch        ; btMan4_5  -> Visible = true  ;
    btMan4_6  -> Caption = "[" + AnsiString(mcSTL_Pick          ) + "]" + "Sorting Pick"           ;  btMan4_6  -> Tag = (int)mcSTL_Pick           ; btMan4_6  -> Visible = true  ;
    btMan4_7  -> Caption = "[" + AnsiString(mcSTL_Place         ) + "]" + "Sorting Place"          ;  btMan4_7  -> Tag = (int)mcSTL_Place          ; btMan4_7  -> Visible = true  ;
    btMan4_8  -> Caption = "[" + AnsiString(mcSTL_YTopClamp     ) + "]" + "Sorting UP\nCYL UP/DN"  ;  btMan4_8  -> Tag = (int)mcSTL_YTopClamp      ; btMan4_8  -> Visible = true  ;
    btMan4_9  -> Caption = "[" + AnsiString(mcSTL_YBtmClamp     ) + "]" + "Sorting DN\nCYL UP/DN"  ;  btMan4_9  -> Tag = (int)mcSTL_YBtmClamp      ; btMan4_9  -> Visible = true  ;
    btMan4_10 -> Caption = "[" + AnsiString(mcSTL_YTopUp1Pitch  ) + "]" + "Sort YTop Up 1 pith"    ;  btMan4_10 -> Tag = (int)mcSTL_YTopUp1Pitch   ; btMan4_10 -> Visible = true  ;
    btMan4_11 -> Caption = "[" + AnsiString(mcSTL_YTopDn1Pitch  ) + "]" + "Sort YTop Dn 1 pith"    ;  btMan4_11 -> Tag = (int)mcSTL_YTopDn1Pitch   ; btMan4_11 -> Visible = true  ;
    btMan4_12 -> Caption = "[" + AnsiString(mcSTL_YBtmUp1pitch  ) + "]" + "Sort YBtm Up 1 pith"    ;  btMan4_12 -> Tag = (int)mcSTL_YBtmUp1pitch   ; btMan4_12 -> Visible = true  ;
    btMan4_13 -> Caption = "[" + AnsiString(mcSTL_YBtmDn1pitch  ) + "]" + "Sort YBtm Dn 1 pith"    ;  btMan4_13 -> Tag = (int)mcSTL_YBtmDn1pitch   ; btMan4_13 -> Visible = true  ;
    btMan4_14 -> Caption = "[" + AnsiString(mcAllHome           ) + "]" + "ALL HOME"               ;  btMan4_14 -> Tag = (int)mcAllHome            ; btMan4_14 -> Visible = true  ;

    btMan5_1  -> Caption = "[" + AnsiString(mcULD_Home          ) + "]" + "UnLoading home"   ;  btMan5_1  -> Tag = (int)mcULD_Home           ; btMan5_1  -> Visible = true  ;
    btMan5_2  -> Caption = "[" + AnsiString(mcULD_Supply        ) + "]" + "UnLoading Supply" ;  btMan5_2  -> Tag = (int)mcULD_Supply         ; btMan5_2  -> Visible = true  ;
    btMan5_3  -> Caption = "[" + AnsiString(mcULD_Pick          ) + "]" + "UnLoading Pick"   ;  btMan5_3  -> Tag = (int)mcULD_Pick           ; btMan5_3  -> Visible = true  ;
    btMan5_4  -> Caption = "[" + AnsiString(mcULD_SttPstn       ) + "]" + "Move Start Pos"   ;  btMan5_4  -> Tag = (int)mcULD_SttPstn        ; btMan5_4  -> Visible = true  ;
    btMan5_5  -> Caption = "[" + AnsiString(mcULD_Place         ) + "]" + "UnLoading Place"  ;  btMan5_5  -> Tag = (int)mcULD_Place          ; btMan5_5  -> Visible = true  ;
    btMan5_6  -> Caption = "[" + AnsiString(mcULD_UpSlPitch     ) + "]" + "Pitch Up"         ;  btMan5_6  -> Tag = (int)mcULD_UpSlPitch      ; btMan5_6  -> Visible = true  ;
    btMan5_7  -> Caption = "[" + AnsiString(mcULD_DnSlPitch     ) + "]" + "Pitch Dn"         ;  btMan5_7  -> Tag = (int)mcULD_DnSlPitch      ; btMan5_7  -> Visible = true  ;
    btMan5_8  -> Caption = "[" + AnsiString(mcULD_Clamp         ) + "]" + "Clamp U/D"        ;  btMan5_8  -> Tag = (int)mcULD_Clamp          ; btMan5_8  -> Visible = true  ;
    btMan5_10 -> Caption = "[" + AnsiString(mcPSB_PusherFw      ) + "]" + "PusherFw"         ;  btMan5_10 -> Tag = (int)mcPSB_PusherFw       ; btMan5_10 -> Visible = true  ;
    btMan5_11 -> Caption = "[" + AnsiString(mcPSB_PusherBw      ) + "]" + "PusherBw"         ;  btMan5_11 -> Tag = (int)mcPSB_PusherBw       ; btMan5_11 -> Visible = true  ;
    btMan5_14 -> Caption = "[" + AnsiString(mcAllHome           ) + "]" + "ALL HOME"         ;  btMan5_14 -> Tag = (int)mcAllHome            ; btMan5_14 -> Visible = true  ;

    btMan7_1  -> Caption = "[" + AnsiString(mcETC_AllClampOff   ) + "]" + "AllClampOff"      ;  btMan7_1  -> Tag = (int)mcETC_AllClampOff    ; btMan7_1  -> Visible = true ;
//    btMan7_2  -> Caption = "[" + AnsiString(                    ) + "]" + "PreRemoveStrip"   ;  btMan7_2  -> Tag = (int)mcETC_PreRemoveStrip ; btMan7_2  -> Visible = false;
//    btMan7_3  -> Caption = "[" + AnsiString(                    ) + "]" + "WrkRemoveStrip"   ;  btMan7_3  -> Tag = (int)mcETC_WrkRemoveStrip ; btMan7_3  -> Visible = false;
//    btMan7_4  -> Caption = "[" + AnsiString(                    ) + "]" + "PstRemoveStrip"   ;  btMan7_4  -> Tag = (int)mcETC_PstRemoveStrip ; btMan7_4  -> Visible = false;
    btMan7_5  -> Caption = "[" + AnsiString(mcETC_AirBlower     ) + "]" + "AirBlower     "   ;  btMan7_5  -> Tag = (int)mcETC_AirBlower      ; btMan7_5  -> Visible = true ;
    btMan7_6  -> Caption = "[" + AnsiString(mcETC_6             ) + "]" + ""                 ;  btMan7_6  -> Tag = (int)mcETC_6              ; btMan7_6  -> Visible = false;
//    btMan7_7  -> Caption = "[" + AnsiString(                    ) + "]" + "RemoveStrip"      ;  btMan7_7  -> Tag = (int)mcETC_RemoveStrip    ; btMan7_7  -> Visible = false;
    btMan7_14 -> Caption = "[" + AnsiString(mcAllHome           ) + "]" + "ALL HOME"         ;  btMan7_14 -> Tag = (int)mcAllHome            ; btMan7_14 -> Visible = true ;

    LAG.ReadFrmOperation();    //작업용?
    switch(FM_GetLan())
    {
        default        :                             break ;
        case lsEnglish :                             break ;
        case lsKorean  : LAG.LoadFrmOperationHan();  break ;
        case lsChinese :                             break ;
    }
//    if(FM_GetLan() == lsKorean) LAG.LoadFrmOperationHan();

    SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true;

    edLimAllFailCnt -> Text = OM.CmnOptn.iCheckFailCnt;

         if(DM.ARAY[riPR1].GetCntExist() && pcPbz -> ActivePageIndex != 0) pcPbz -> ActivePageIndex = 0 ;
    else if(DM.ARAY[riPR2].GetCntExist() && pcPbz -> ActivePageIndex != 1) pcPbz -> ActivePageIndex = 1 ;

         if(DM.ARAY[riWRP].GetCntExist() && pcIns -> ActivePageIndex != 0) pcIns -> ActivePageIndex = 0 ;
    else if(DM.ARAY[riWRK].GetCntExist() && pcIns -> ActivePageIndex != 1) pcIns -> ActivePageIndex = 1 ;

         if(DM.ARAY[riPS1].GetCntExist() && pcPsb -> ActivePageIndex != 0) pcPsb -> ActivePageIndex = 0 ;
    else if(DM.ARAY[riPS2].GetCntExist() && pcPsb -> ActivePageIndex != 1) pcPsb -> ActivePageIndex = 1 ;

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

void __fastcall TFrmOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

//    Edit1 -> Text = STL._sLastID ;
    int iBinCnt = STL._iBinCnt + 1;
    edRejectBinNo -> Text = iBinCnt ; 

    lbCnt   -> Caption = WRK.m_iDataErrCnt   ;
    lbCntRe -> Caption = WRK.m_iDataErrCntRe ;

    if(DM.ARAY[riPR1].GetCntExist() && !edPrbLotNo -> Focused() && !edPrbMgzNo->Focused() && !edPrbSlotNo->Focused() &&!btPrbLotInput->Focused() ) {
//        lbPre   -> Caption = "LOTNO:" + DM.ARAY[riPR1].GetLotNo() + " MGZNO:" +DM.ARAY[riPR1].GetID().ToIntDef(9999)/100 + " SLOTNO:" + DM.ARAY[riPR1].GetID().ToIntDef(99)%100;
        edPrbLotNo  -> Text = DM.ARAY[riPR1].GetLotNo() ;
        edPrbMgzNo  -> Text = DM.ARAY[riPR1].GetID().ToIntDef(9999)/100 ;
        edPrbSlotNo -> Text = DM.ARAY[riPR1].GetID().ToIntDef(99)%100;
    }
    else if(DM.ARAY[riPR2].GetCntExist() && !edPrbLotNo -> Focused() && !edPrbMgzNo->Focused() && !edPrbSlotNo->Focused() &&!btPrbLotInput->Focused()) {
//        lbPre   -> Caption = "LOTNO:" + DM.ARAY[riPR2].GetLotNo() + " MGZNO:" +DM.ARAY[riPR2].GetID().ToIntDef(9999)/100 + " SLOTNO:" + DM.ARAY[riPR2].GetID().ToIntDef(99)%100;
        edPrbLotNo  -> Text = DM.ARAY[riPR2].GetLotNo() ;
        edPrbMgzNo  -> Text = DM.ARAY[riPR2].GetID().ToIntDef(9999)/100 ;
        edPrbSlotNo -> Text = DM.ARAY[riPR2].GetID().ToIntDef(99)%100;
    }
    if(DM.ARAY[riWRP].GetCntExist() && !edInspLotNo->Focused() && !edInspMgzNo->Focused() && !edInspSlotNo->Focused() && !btInspLotInput->Focused() ) {
//        lbInsp  -> Caption  = "LOTNO:" + DM.ARAY[riWRP].GetLotNo() + " MGZNO:" +DM.ARAY[riWRP].GetID().ToIntDef(9999)/100 + " SLOTNO:" + DM.ARAY[riWRP].GetID().ToIntDef(99)%100;
        edInspLotNo  -> Text = DM.ARAY[riWRP].GetLotNo() ;
        edInspMgzNo  -> Text = DM.ARAY[riWRP].GetID().ToIntDef(9999)/100 ;
        edInspSlotNo -> Text = DM.ARAY[riWRP].GetID().ToIntDef(99)%100;
    }
    else if(DM.ARAY[riWRK].GetCntExist() && !edInspLotNo->Focused() && !edInspMgzNo->Focused() && !edInspSlotNo->Focused() && !btInspLotInput->Focused() ) {
//        lbInsp -> Caption   = "LOTNO:" + DM.ARAY[riWRK].GetLotNo() + " MGZNO:" +DM.ARAY[riWRK].GetID().ToIntDef(9999)/100 + " SLOTNO:" + DM.ARAY[riWRK].GetID().ToIntDef(99)%100;
        edInspLotNo  -> Text = DM.ARAY[riWRK].GetLotNo() ;
        edInspMgzNo  -> Text = DM.ARAY[riWRK].GetID().ToIntDef(9999)/100 ;
        edInspSlotNo -> Text = DM.ARAY[riWRK].GetID().ToIntDef(99)%100;
    }
    if(DM.ARAY[riPS1].GetCntExist() && !edPsbLotNo->Focused() && !edPsbMgzNo->Focused() && !edPsbSlotNo->Focused() && !btPsbLotInput->Focused() ) {
//        lbPost -> Caption   = "LOTNO:" + DM.ARAY[riPS1].GetLotNo() + " MGZNO:" +DM.ARAY[riPS1].GetID().ToIntDef(9999)/100 + " SLOTNO:" + DM.ARAY[riPS1].GetID().ToIntDef(99)%100;
        edPsbLotNo  -> Text  = DM.ARAY[riPS1].GetLotNo() ;
        edPsbMgzNo  -> Text = DM.ARAY[riPS1].GetID().ToIntDef(9999)/100 ;
        edPsbSlotNo -> Text = DM.ARAY[riPS1].GetID().ToIntDef(99)%100;
    }
    else if(DM.ARAY[riPS2].GetCntExist() && !edPsbLotNo->Focused() && !edPsbMgzNo->Focused() && !edPsbSlotNo->Focused() && !btPsbLotInput->Focused() ) {
//        lbPost -> Caption   = "LOTNO:" + DM.ARAY[riPS2].GetLotNo() + " MGZNO:" +DM.ARAY[riPS2].GetID().ToIntDef(9999)/100 + " SLOTNO:" + DM.ARAY[riPS2].GetID().ToIntDef(99)%100;
        edPsbLotNo  -> Text  = DM.ARAY[riPS2].GetLotNo() ;
        edPsbMgzNo  -> Text = DM.ARAY[riPS2].GetID().ToIntDef(9999)/100 ;
        edPsbSlotNo -> Text = DM.ARAY[riPS2].GetID().ToIntDef(99)%100;
    }

    //
    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }

    switch (iLevel) {
        case lvOperator : if( iLev != lvOperator) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLD1 && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLD1 && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(OM.MstOptn.bDataAccEng);
                              }
                              iLev = lvEngineer ;
                          }
                          break ;
        case lvMaster   : if( iLev != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLD1 && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iLev = lvMaster ;
                          }
                          break ;
    }

    edRjtBinNoInput ->Visible = FM_GetLevel() >= lvMaster ;

    //Clear Button Visible
//    btPrbClear  -> Visible = true ;//FM_GetLevel() != lvOperator ; //꺼내는 것은 오퍼가 할 수 있어야 될듯 하다.
//    btPsbClear  -> Visible = true ;//FM_GetLevel() != lvOperator ;
//    btInspClear -> Visible = true ;//FM_GetLevel() != lvOperator ;

    //Lot No , Mgz No , Slot No Visible
//    edPrbLotNo  -> Visible = false ; //!SEQ._bRun;
//    edPrbMgzNo  -> Visible = false ; //!SEQ._bRun;
//    edPrbSlotNo -> Visible = false ; //!SEQ._bRun;

//    edPsbLotNo  -> Visible = false ; //!SEQ._bRun;
//    edPsbMgzNo  -> Visible = false ; //!SEQ._bRun;
//    edPsbSlotNo -> Visible = false ; //!SEQ._bRun;

//    edInspLotNo  -> Visible = false ; //!SEQ._bRun;
//    edInspMgzNo  -> Visible = false ; //!SEQ._bRun;
//    edInspSlotNo -> Visible = false ; //!SEQ._bRun;

//    btPrbLotInput  -> Visible = false ; //!SEQ._bRun;
//    btPsbLotInput  -> Visible = false ; //!SEQ._bRun;
//    btInspLotInput -> Visible = false ; //!SEQ._bRun;


    //Manual Enabled
    pcManual -> Enabled = !SEQ._bRun ;

    //Button Start Enabled
    if(OM.MstOptn.bNotUstLot) btStart  -> Enabled = true          ;
    else                      btStart  -> Enabled = LT.m_bLotOpen ;

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
        pnLotChipUpeh  ,
        NULL           ,
        NULL           ,
        NULL           ,
        pnLotWorkChip  ,
        pnLotFailChip  );

    LT.DispDayInfo(
        pnDayRunTime    ,
        pnDayIdleTime   ,
        pnDayJamTime    ,
        pnDayTotalTime  ,
        NULL            ,
        pnDayChipUpeh   ,
        NULL            ,
        NULL            ,
        NULL            ,
        pnDayWorkChip   ,
        pnDayFailChip   ,
        pnDayLotCount   );

    //EM_
    EM_DisplayList(lbErr);

    pnIgnoreTrim    -> Color  =  OM.CmnOptn.bIgnrTrim       ? clLime : clRed ;
    pnIgnrDoor      -> Color  =  OM.CmnOptn.bIgnrDoor       ? clLime : clRed ;
    pnInspSkip      -> Color  =  OM.CmnOptn.bIgnrInsp       ? clLime : clRed ;
    pnIgnrIdCheck   -> Color  =  OM.CmnOptn.bIgnrIdCehck    ? clLime : clRed ;
    pnIgnrVacuum    -> Color  =  OM.CmnOptn.bIgnrVacuum     ? clLime : clRed ;
    pnIgnrStrAlm    -> Color  =  OM.CmnOptn.bIgnrStrOverAlm ? clLime : clRed ;
    pnLoadingStop   -> Color  =  OM.CmnOptn.bLoadingStop    ? clLime : clRed ;
    pnCheckFailCnt  -> Color  =  OM.CmnOptn.bCheckFailCnt   ? clLime : clRed ;
    pnIgnoreTrim    -> Color  =  OM.CmnOptn.bIgnrTrim       ? clLime : clRed ;
//  pnAutoConvesion -> Color  =  OM.CmnOptn.bAutoConvesion  ? clLime : clRed ;

    pnIgnoreTrim    -> Caption  =  OM.CmnOptn.bIgnrTrim       ? "ON" : "OFF" ;
    pnIgnrDoor      -> Caption  =  OM.CmnOptn.bIgnrDoor       ? "ON" : "OFF" ;
    pnInspSkip      -> Caption  =  OM.CmnOptn.bIgnrInsp       ? "ON" : "OFF" ;
    pnIgnrIdCheck   -> Caption  =  OM.CmnOptn.bIgnrIdCehck    ? "ON" : "OFF" ;
    pnIgnrVacuum    -> Caption  =  OM.CmnOptn.bIgnrVacuum     ? "ON" : "OFF" ;
    pnIgnrStrAlm    -> Caption  =  OM.CmnOptn.bIgnrStrOverAlm ? "ON" : "OFF" ;
    pnLoadingStop   -> Caption  =  OM.CmnOptn.bLoadingStop    ? "ON" : "OFF" ;
    pnCheckFailCnt  -> Caption  =  OM.CmnOptn.bCheckFailCnt   ? "ON" : "OFF" ;
    pnIgnoreTrim    -> Caption  =  OM.CmnOptn.bIgnrTrim       ? "ON" : "OFF" ;
//  pnAutoOperation -> Caption  =  OM.CmnOptn.bAutoOperation  ? "ON" : "OFF" ;
//  pnAutoConvesion -> Caption  =  OM.CmnOptn.bAutoConvesion  ? "ON" : "OFF" ;

    if(SEQ._bRun) {
             if(DM.ARAY[riPR1].GetCntExist() && pcPbz -> ActivePageIndex != 0) pcPbz -> ActivePageIndex = 0 ;
        else if(DM.ARAY[riPR2].GetCntExist() && pcPbz -> ActivePageIndex != 1) pcPbz -> ActivePageIndex = 1 ;

             if(DM.ARAY[riWRP].GetCntExist() && pcIns -> ActivePageIndex != 0) pcIns -> ActivePageIndex = 0 ;
        else if(DM.ARAY[riWRK].GetCntExist() && pcIns -> ActivePageIndex != 1) pcIns -> ActivePageIndex = 1 ;

             if(DM.ARAY[riPS1].GetCntExist() && pcPsb -> ActivePageIndex != 0) pcPsb -> ActivePageIndex = 0 ;
        else if(DM.ARAY[riPS2].GetCntExist() && pcPsb -> ActivePageIndex != 1) pcPsb -> ActivePageIndex = 1 ;
    }

    btPrbLotInput  -> Visible = !SEQ._bRun ;
    btInspLotInput -> Visible = !SEQ._bRun ;
    btPsbLotInput  -> Visible = !SEQ._bRun ;

    if(SEQ._bInspDispr) { CheckStripDisappear(); SEQ._bInspDispr = false; } //함수 끝나고 자동 스타트 됍.
    if(SEQ._bInspUnkwn) { CheckStripUnknwon  (); SEQ._bInspUnkwn = false; } //함수 끝나고 자동 스타트 됍.
//Lot 엔드시 화면에 보여주기 용.
    if(WRK.m_bDispLotEnded) {
        WRK.m_bDispLotEnded = false ;
        edTime       -> Text = Now().FormatString("hh:nn:ss")     ;
        edLotNo      -> Text = WRK.EndedLot.sLotNo       ;//DM.ARAY[riWK2].GetLotNo  (        );
        edFailTotal  -> Text = WRK.EndedLot.iFailChip    ;
        edWork       -> Text = WRK.EndedLot.iWorkChip    ;//DM.ARAY[riWK2].GetCntStat(csWork  );
    }                      
//

    //bool isLotChanged = (DM.ARAY[riULD].GetLotNo()!="" && DM.ARAY[riULD].GetLotNo() != DM.ARAY[riPS2].GetLotNo() && !DM.ARAY[riPS2].CheckAllStat(csNone)) ||
    //                    (DM.ARAY[riULD].GetLotNo()!="" && DM.ARAY[riULD].GetLotNo() != DM.ARAY[riPS1].GetLotNo() && !DM.ARAY[riPS1].CheckAllStat(csNone) && DM.ARAY[riPS2].CheckAllStat(csNone));

    btLotOpen -> Enabled = !OM.MstOptn.bNotUstLot ;
    btLotEnd  -> Enabled = !OM.MstOptn.bNotUstLot ;




    AnsiString sBtnName ;
    TBitBtn *  pBtn     ;

    for(int i = 1 ; i < 8/*페이지6개*/ ; i++) {
        for(int j = 1 ; j < 15 /*버튼14개*/; j++) {
            sBtnName = "btMan" + AnsiString(i) + "_" + AnsiString(j) ;
            pBtn = ((TBitBtn*)FindComponent(sBtnName)) ;
            if(pBtn == NULL)  continue ;
            if(pBtn -> Tag == MM.GetManNo()) pBtn -> Font -> Color = clLime  ;
            else                             pBtn -> Font -> Color = clBlack ;
        }
    }





    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
    //if(SEQ._bRun) {FM_MsgOk("ERROR" , "Can't Use While Running"); return ;}
    FrmLotOpen -> ShowModal() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
    bool isAllHomeEnd = true;

    //Inspect.
    if (!MT_GetHomeEnd(miPRB_XCmp)){isAllHomeEnd = false; }//EM_SetErr(eiPRB_XCmpAlarm); isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miWRK_XCmp)){isAllHomeEnd = false; }//EM_SetErr(eiWRK_XCmpAlarm); isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miPSB_XCmp)){isAllHomeEnd = false; }//EM_SetErr(eiPSB_XCmpAlarm); isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miWRK_ZTop)){isAllHomeEnd = false; }//EM_SetErr(eiWRK_ZTopAlarm); isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miWRK_ZBtm)){isAllHomeEnd = false; }//EM_SetErr(eiWRK_ZBtmAlarm); isAllHomeEnd = false; }
    if(OM.EqpOptn.bExistLoader  ){ if (!MT_GetHomeEnd(miLD1_ZCmp)){ isAllHomeEnd = false; } }//EM_SetErr(eiLD1_ZCmpAlarm); isAllHomeEnd = false; } }
    if(OM.EqpOptn.bExistLoader  ){ if (!MT_GetHomeEnd(miLD1_YCmp)){ isAllHomeEnd = false; } }//EM_SetErr(eiLD1_YCmpAlarm); isAllHomeEnd = false; } }
    if(OM.EqpOptn.bExistUnLoader){ if (!MT_GetHomeEnd(miULD_ZCmp)){ isAllHomeEnd = false; } }//EM_SetErr(eiLD1_ZCmpAlarm); isAllHomeEnd = false; } }
    if(OM.EqpOptn.bExistUnLoader){ if (!MT_GetHomeEnd(miULD_YCmp)){ isAllHomeEnd = false; } }//EM_SetErr(eiLD1_YCmpAlarm); isAllHomeEnd = false; } }

    if(isAllHomeEnd) SEQ._bBtnStart = true ;
    else FM_MsgOk("ERROR","PLEASE All Home ") ;

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

void __fastcall TFrmOperation::btLotEndClick(TObject *Sender)
{
    if(!LT.m_bLotOpen) {FM_MsgOk("ERROR" , "Can't Use While Lot Closed"); return ;}
    if(FM_MsgYesNo("Confirm","Do you really want to LotEnd?") != mrYes) return ;

    if(DM.ARAY[riLD1].GetCntExist() && DM.ARAY[riULD].GetCntExist()) MM.SetManCycle(mcETC_LDRULDPlace);
    else if(DM.ARAY[riLD1].GetCntExist()) MM.SetManCycle(mcLDR_Place);
    else if(DM.ARAY[riULD].GetCntExist()) MM.SetManCycle(mcULD_Place);

    LT.LotEnd ();
    DM.ClearMap();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnIgnoreTrimDblClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

//    if(FM_GetLevel() == lvOperator) return ;

    switch(iIndex) {
        case 0 :                                 break  ;
        case 1 :                                 break  ;
        case 2 : if(FM_GetLevel() == lvOperator) return ;
        case 3 : if(FM_GetLevel() == lvOperator) return ;
        case 4 : if(FM_GetLevel() == lvOperator) return ;
        case 5 : if(FM_GetLevel() == lvOperator) return ;
        case 6 : if(FM_GetLevel() == lvOperator) return ;
        case 7 :                                 break  ;
        case 10: if(FM_GetLevel() == lvOperator) return ;
        case 11: if(FM_GetLevel() == lvOperator) return ;
    }

    if (FM_MsgYesNo("Confirm", "Would you like to invert the flag?") != mrYes) return ;

    switch(iIndex) {
        case 0 :                                                            break ;
        case 1 : OM.CmnOptn.bIgnrDoor       = !OM.CmnOptn.bIgnrDoor       ; break ;
        case 2 : OM.CmnOptn.bIgnrIdCehck    = !OM.CmnOptn.bIgnrIdCehck    ; break ;
        case 3 : OM.CmnOptn.bIgnrInsp       = !OM.CmnOptn.bIgnrInsp       ; break ;
//        case 4 : OM.CmnOptn.bIgnrHeatAlm    = !OM.CmnOptn.bIgnrHeatAlm    ; break ;
        case 5 : OM.CmnOptn.bIgnrStrOverAlm = !OM.CmnOptn.bIgnrStrOverAlm ; break ;
//        case 5 : OM.CmnOptn.bIgnrStrOverAlm = !OM.CmnOptn.bIgnrStrOverAlm ; break ;
        case 6 : OM.CmnOptn.bLoadingStop    = !OM.CmnOptn.bLoadingStop    ; break ;
        case 7 : OM.CmnOptn.bCheckFailCnt   = !OM.CmnOptn.bCheckFailCnt   ; break ;
        case 10: OM.CmnOptn.bIgnrTrim       = !OM.CmnOptn.bIgnrTrim       ; break ;
        case 11: OM.CmnOptn.bIgnrVacuum     = !OM.CmnOptn.bIgnrVacuum     ; break ;        
        //case 6 : OM.CmnOptn.bAutoOperation  = !OM.CmnOptn.bAutoOperation  ; break ;
        //case 7 : OM.CmnOptn.bAutoConvesion  = !OM.CmnOptn.bAutoConvesion  ; break ;
    }

    SPC.SaveOptionLog(iIndex);
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPrbClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo ,sMsg;
    sLotNo   = "PreBuff1:" + DM.ARAY[riPR1].GetLotNo() + " PreBuff2:" + DM.ARAY[riPR2].GetLotNo() + " ";;
    sSlotNo  = "PreBuff1:" + AnsiString(DM.ARAY[riPR1].GetID().ToIntDef(99)%100) + " PreBuff2:" + AnsiString(DM.ARAY[riPR2].GetID().ToIntDef(99)%100) + " ";
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    DM.ARAY[riPR1].SetStat(csNone);
    DM.ARAY[riPR2].SetStat(csNone);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btInspClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo ,sMsg;
    sLotNo   = "WrkBuff1:" + DM.ARAY[riWRP].GetLotNo() + " WrkBuff2:" + DM.ARAY[riWRK].GetLotNo() + " ";;
    sSlotNo  = "WrkBuff1:" + AnsiString(DM.ARAY[riWRP].GetID().ToIntDef(99)%100) + " WrkBuff2:" + AnsiString(DM.ARAY[riWRK].GetID().ToIntDef(99)%100) + " ";
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    DM.ARAY[riWRP].SetStat(csNone);
    DM.ARAY[riWRK].SetStat(csNone);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPsbClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo ,sMsg;
    sLotNo   = "PstBuff1:" + DM.ARAY[riPS1].GetLotNo() + " PstBuff2:" + DM.ARAY[riPS2].GetLotNo() + " ";;
    sSlotNo  = "PstBuff2:" + AnsiString(DM.ARAY[riPS1].GetID().ToIntDef(99)%100) + " PstBuff2:" + AnsiString(DM.ARAY[riPS2].GetID().ToIntDef(99)%100) + " ";
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    DM.ARAY[riPS1].SetStat(csNone);
    DM.ARAY[riPS2].SetStat(csNone);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPrbLotInputClick(TObject *Sender)
{
    if(SEQ._bRun) return ;

//    if(FM_MsgYesNo("Confirm","Did you want to enter Lot information ") != mrYes) return ;

    int iTag = ((TButton*)Sender) -> Tag ;
    AnsiString sLotNo ;
    int iMgzNo , iSlotNo ;

    switch (iTag)
    {
        default :
            break ;

        case 1 :
            sLotNo  = edPrbLotNo ->Text ;
            iMgzNo  = edPrbMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edPrbSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riPR1].SetLotNo(sLotNo) ;
            DM.ARAY[riPR2].SetLotNo(sLotNo) ;
            DM.ARAY[riPR1].SetID(iMgzNo *100 + iSlotNo);
            DM.ARAY[riPR2].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 2 :
            sLotNo  = edInspLotNo ->Text ;
            iMgzNo  = edInspMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edInspSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riWRP].SetLotNo(sLotNo) ;
            DM.ARAY[riWRK].SetLotNo(sLotNo) ;
            DM.ARAY[riWRP].SetID(iMgzNo *100 + iSlotNo);
            DM.ARAY[riWRK].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 3 :
            sLotNo  = edPsbLotNo ->Text ;
            iMgzNo  = edPsbMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edPsbSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riPS1].SetLotNo(sLotNo) ;
            DM.ARAY[riPS2].SetLotNo(sLotNo) ;
            DM.ARAY[riPS1].SetID(iMgzNo *100 + iSlotNo);
            DM.ARAY[riPS2].SetID(iMgzNo *100 + iSlotNo);
            break ;
     }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::edLimAllFailCntChange(TObject *Sender)
{
    OM.CmnOptn.iCheckFailCnt = edLimAllFailCnt -> Text.ToIntDef(OM.CmnOptn.iCheckFailCnt);

    edLimAllFailCnt -> Text = OM.CmnOptn.iCheckFailCnt;

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
bool __fastcall TFrmOperation::CheckStripDisappear()
{
    //스트립 사라짐 에러.
//    if (!DM.ARAY[riPR2].CheckAllStat(csNone) && !IO_GetX(xPRB_3Pkg) ) {
//        if(FM_MsgYesNo("Err " , "[PreBuffer2 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riPR2 ].SetStat(csNone); SEQ._bBtnStart = true ;}
//    }
    if(( DM.ARAY[riWRP].GetCntExist() || DM.ARAY[riWRK].GetCntExist()) &&  (!IO_GetX(xWRK_1Pkg) && !IO_GetX(xWRK_2Pkg) )  && !OM.CmnOptn.bDryRun ) {
        if(FM_MsgYesNo("Err " , "[ProbeBuffer1,2 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riWRP ].SetStat(csNone); DM.ARAY[riWRK ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if(( DM.ARAY[riPS1].GetCntExist() || DM.ARAY[riPS2].GetCntExist()) && !OM.CmnOptn.bDryRun) {
        if((!IO_GetX(xPSB_1Pkg) && !IO_GetX(xPSB_2Pkg) && !IO_GetX(xPSB_3Pkg) && !IO_GetX(xPSB_4Pkg) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2)) ) {
            if(FM_MsgYesNo("Err " , "[PostBuffer2 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riPS1 ].SetStat(csNone); DM.ARAY[riPS2 ].SetStat(csNone); SEQ._bBtnStart = true ;}
        }
    }
//    else if ( DM.ARAY[riPS2].GetCntExist() && !IO_GetX(xPSB_PkgOut1) && !OM.CmnOptn.bDryRun) {
//        if(FM_MsgYesNo("Err " , "[PostBuffer2 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riPS2 ].SetStat(csNone); SEQ._bBtnStart = true ;}
//    }
    else if ( !DM.ARAY[riLD1].CheckAllStat(csNone) && ( !IO_GetX(xLD1_MgzDetect1) && !IO_GetX(xLD1_MgzDetect2) ) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Loader MGZ Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riLD1 ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if ( !DM.ARAY[riULD].CheckAllStat(csNone) && ( !IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2) ) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[UnLoader MGZ Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riULD ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }

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
void __fastcall TFrmOperation::btPRBInputClick(TObject *Sender)
{
    DM.ARAY[riPR2].SetStat(csUnkwn);
    if(DM.ARAY[riLD1].CheckAllStat(csNone) || DM.ARAY[riLD1].CheckAllStat(csUnkwn)) DM.ARAY[riPR2].SetLotNo(PRB.GetWorkedLotNo()     ) ;
    else                                                                            DM.ARAY[riPR2].SetLotNo(DM.ARAY[riLD1].GetLotNo()) ;

    pcPbz -> ActivePageIndex = 1 ;
}
//---------------------------------------------------------------------------








void __fastcall TFrmOperation::pnDayRunTimeDblClick(TObject *Sender)
{
    if(FM_GetLevel() != lvMaster) return ;

    if(FM_MsgYesNo("Confirm","Clear Day Info") != mrYes) return ;

    LT.ClearDayInfo() ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::GroupBox1Click(TObject *Sender)
{
    LT.WriteLotDayLog();    
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSetBinCntClick(TObject *Sender)
{
         if(edRjtBinNoInput -> Text.ToIntDef(0) >= 10 ) STL._iBinCnt = 9 ;
    else if(edRjtBinNoInput -> Text.ToIntDef(0) <= 1  ) STL._iBinCnt = 0 ;
    else STL._iBinCnt = edRjtBinNoInput -> Text.ToIntDef(0) - 1;
}
//---------------------------------------------------------------------------

