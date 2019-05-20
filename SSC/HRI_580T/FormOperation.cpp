//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormLotOpen.h"
#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "CommUnit.h"
#include "ManualMan.h"
#include "VisnComUnit.h"
#include "AdlinkTenkey.h"
#include "Rail.h"
#include "SPCUnit.h"
#include "EqpComUnit.h"
#include "UserINI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)
    : TForm(Owner)
{
    DM.ARAY[riLDR].SetParent(pnLoader  ); DM.ARAY[riPSB].SetConfig(false , "Loader  "); DM.ARAY[riLDR].ClearDispItem();
    DM.ARAY[riPRB].SetParent(pnPreBuff ); DM.ARAY[riPSB].SetConfig(false , "PostBuff"); DM.ARAY[riPSB].ClearDispItem();
    DM.ARAY[riWK1].SetParent(pnInsp1   ); DM.ARAY[riWK1].SetConfig(false , "Insp1   "); DM.ARAY[riWK1].ClearDispItem();
    DM.ARAY[riVS1].SetParent(pnVisn1   ); DM.ARAY[riVS1].SetConfig(false , "Visn1   "); DM.ARAY[riVS1].ClearDispItem();
    DM.ARAY[riWK2].SetParent(pnInsp2   ); DM.ARAY[riWK2].SetConfig(false , "Insp2   "); DM.ARAY[riWK2].ClearDispItem();
    DM.ARAY[riVS2].SetParent(pnVisn2   ); DM.ARAY[riVS2].SetConfig(false , "Visn2   "); DM.ARAY[riVS2].ClearDispItem();
    DM.ARAY[riPSB].SetParent(pnPostBuff); DM.ARAY[riPSB].SetConfig(false , "PreBuff "); DM.ARAY[riPSB].ClearDispItem();
    DM.ARAY[riULD].SetParent(pnUnLoader); DM.ARAY[riULD].SetConfig(false , "UnLoader"); DM.ARAY[riULD].ClearDispItem();

    DM.ARAY[riLDR ].SetDispItem(csEmpty   ,"Empty "        ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite           );
    DM.ARAY[riLDR ].SetDispItem(csUnkwn   ,"Unknown "      ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen      );
    DM.ARAY[riLDR ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riLDR ].SetDispColor(csNone  ,clWhite           );

    DM.ARAY[riPRB ].SetDispItem(csFail    ,"Heat Fail "    ) ; //DM.ARAY[riPRB ].SetDispColor(csFail  ,clPurple          );
    DM.ARAY[riPRB ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riPRB ].SetDispColor(csNone  ,clWhite           );
    DM.ARAY[riPRB ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riPRB ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riPRB ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riWK1 ].SetDispItem(csFail    ,"Heat Fail "    ) ; //DM.ARAY[riWK1 ].SetDispColor(csFail  ,clPurple          );
    DM.ARAY[riWK1 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riWK1 ].SetDispColor(csNone  ,clWhite           );
    DM.ARAY[riWK1 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riWK1 ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riWK1 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riWK1 ].SetDispColor(csWork  ,clHighlight       );
    DM.ARAY[riWK1 ].SetDispItem(csRslt1   ,"V Empty "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt1 ,clBlack           );
    DM.ARAY[riWK1 ].SetDispItem(csRslt2   ,"V Chip "       ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt2 ,clRed             );
    DM.ARAY[riWK1 ].SetDispItem(csRslt3   ,"V Dust "       ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt3 ,clGreen           );
    DM.ARAY[riWK1 ].SetDispItem(csRslt4   ,"V LFlow "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt4 ,clBlue            );
    DM.ARAY[riWK1 ].SetDispItem(csRslt5   ,"V Broken "     ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt5 ,clLime            );
    DM.ARAY[riWK1 ].SetDispItem(csRslt6   ,"V Crack "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt6 ,clFuchsia         );
    DM.ARAY[riWK1 ].SetDispItem(csRslt7   ,"V Burr "       ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt7 ,clWhite           );
    DM.ARAY[riWK1 ].SetDispItem(csRslt8   ,"V Zener "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt8 ,clMaroon          );
    DM.ARAY[riWK1 ].SetDispItem(csRslt9   ,"V Wire "       ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt9 ,clTeal            );
    DM.ARAY[riWK1 ].SetDispItem(csRslt10  ,"V Orient "     ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt10,clAqua            );
    DM.ARAY[riWK1 ].SetDispItem(csRslt11  ,"V Pmeasure "   ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt11,clNavy            );
    DM.ARAY[riWK1 ].SetDispItem(csRslt12  ,"V Pdist "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt12,clOlive           );
    DM.ARAY[riWK1 ].SetDispItem(csRslt13  ,"V Match "      ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt13,clYellow          );
    DM.ARAY[riWK1 ].SetDispItem(csRslt14  ,"V Flow "       ) ; //DM.ARAY[riWK1 ].SetDispColor(csRslt14,clSkyBlue         );
    DM.ARAY[riWK1 ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riWK1 ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riVS1 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS1 ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riVS1 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS1 ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riVS1 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riVS1 ].SetDispColor(csWork  ,clHighlight       );

    DM.ARAY[riWK2 ].SetDispItem(csFail    ,"Heat Fail "    ) ; //DM.ARAY[riWK2 ].SetDispColor(csFail  ,clPurple          );
    DM.ARAY[riWK2 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riWK2 ].SetDispColor(csNone  ,clWhite           );
    DM.ARAY[riWK2 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riWK2 ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riWK2 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riWK2 ].SetDispColor(csWork  ,clHighlight       );
    DM.ARAY[riWK2 ].SetDispItem(csRslt1   ,"V Empty "      ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt1 ,clBlack           );
    DM.ARAY[riWK2 ].SetDispItem(csRslt2   ,"V Chip "       ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt2 ,clRed             );
    DM.ARAY[riWK2 ].SetDispItem(csRslt3   ,"V Dust "       ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt3 ,clGreen           );
    DM.ARAY[riWK2 ].SetDispItem(csRslt4   ,"V LFlow "      ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt4 ,clBlue            );
    DM.ARAY[riWK2 ].SetDispItem(csRslt5   ,"V Broken "     ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt5 ,clLime            );
    DM.ARAY[riWK2 ].SetDispItem(csRslt6   ,"V Crack "      ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt6 ,clFuchsia         );
    DM.ARAY[riWK2 ].SetDispItem(csRslt7   ,"V Burr "       ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt7 ,clWhite           );
    DM.ARAY[riWK2 ].SetDispItem(csRslt8   ,"V Zener "      ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt8 ,clMaroon          );
    DM.ARAY[riWK2 ].SetDispItem(csRslt9   ,"V Wire "       ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt9 ,clTeal            );
    DM.ARAY[riWK2 ].SetDispItem(csRslt10  ,"V Orient "     ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt10,clAqua            );
    DM.ARAY[riWK2 ].SetDispItem(csRslt11  ,"V Pmeasure "   ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt11,clNavy            );
    DM.ARAY[riWK2 ].SetDispItem(csRslt12  ,"V Pdist "      ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt12,clOlive           );
    DM.ARAY[riWK2 ].SetDispItem(csRslt13  ,"V Match "      ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt13,clYellow          );
    DM.ARAY[riWK2 ].SetDispItem(csRslt14  ,"V Flow "       ) ; //DM.ARAY[riWK2 ].SetDispColor(csRslt14,clSkyBlue         );
    DM.ARAY[riWK2 ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riWK2 ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riVS2 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS2 ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riVS2 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS2 ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riVS2 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riVS2 ].SetDispColor(csWork  ,clHighlight       );

    DM.ARAY[riPSB ].SetDispItem(csEmpty   ,"Empty "        ) ; //DM.ARAY[riPSB ].SetDispColor(csEmpty ,clSilver          );
    DM.ARAY[riPSB ].SetDispItem(csFail    ,"Heat Fail "    ) ; //DM.ARAY[riPSB ].SetDispColor(csFail  ,clPurple          );
    DM.ARAY[riPSB ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riPSB ].SetDispColor(csNone  ,clWhite           );
    DM.ARAY[riPSB ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riPSB ].SetDispColor(csWork  ,clHighlight       );
    DM.ARAY[riPSB ].SetDispItem(csRslt1   ,"V Empty "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt1 ,clBlack           );
    DM.ARAY[riPSB ].SetDispItem(csRslt2   ,"V Chip "       ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt2 ,clRed             );
    DM.ARAY[riPSB ].SetDispItem(csRslt3   ,"V Dust "       ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt3 ,clGreen           );
    DM.ARAY[riPSB ].SetDispItem(csRslt4   ,"V LFlow "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt4 ,clBlue            );
    DM.ARAY[riPSB ].SetDispItem(csRslt5   ,"V Broken "     ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt5 ,clLime            );
    DM.ARAY[riPSB ].SetDispItem(csRslt6   ,"V Crack "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt6 ,clFuchsia         );
    DM.ARAY[riPSB ].SetDispItem(csRslt7   ,"V Burr "       ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt7 ,clWhite           );
    DM.ARAY[riPSB ].SetDispItem(csRslt8   ,"V Zener "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt8 ,clMaroon          );
    DM.ARAY[riPSB ].SetDispItem(csRslt9   ,"V Wire "       ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt9 ,clTeal            );
    DM.ARAY[riPSB ].SetDispItem(csRslt10  ,"V Orient "     ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt10,clAqua            );
    DM.ARAY[riPSB ].SetDispItem(csRslt11  ,"V Pmeasure "   ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt11,clNavy            );
    DM.ARAY[riPSB ].SetDispItem(csRslt12  ,"V Pdist "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt12,clOlive           );
    DM.ARAY[riPSB ].SetDispItem(csRslt13  ,"V Match "      ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt13,clYellow          );
    DM.ARAY[riPSB ].SetDispItem(csRslt14  ,"V Flow "       ) ; //DM.ARAY[riPSB ].SetDispColor(csRslt14,clSkyBlue         );
    DM.ARAY[riPSB ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riPSB ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riULD ].SetDispItem(csEmpty   ,"Empty "        ) ; //DM.ARAY[riULD ].SetDispColor(csEmpty ,clWhite           );
    DM.ARAY[riULD ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riULD ].SetDispColor(csWork  ,clHighlight       );
    DM.ARAY[riULD ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riULD ].SetDispColor(csNone  ,clWhite           );

        //Default Set.
    clStat[csRslt0 ] = clInactiveCaption ;//csInactiveCaption
    clStat[csRslt1 ] = clBlack           ;//csMaroon
    clStat[csRslt2 ] = clRed             ;//csPurple
    clStat[csRslt3 ] = clGreen           ;//csTeal
    clStat[csRslt4 ] = clBlue            ;//csOlive
    clStat[csRslt5 ] = clLime            ;//csNavy
    clStat[csRslt6 ] = clFuchsia         ;//csMoneyGreen
    clStat[csRslt7 ] = clWhite           ;//csGreen
    clStat[csRslt8 ] = clMaroon          ;//clHighlight
    clStat[csRslt9 ] = clTeal            ;//csBlack
    clStat[csRslt10] = clAqua            ;//csBlack
    clStat[csRslt11] = clNavy            ;//csBlack
    clStat[csRslt12] = clOlive           ;//csBlack
    clStat[csRslt13] = clYellow          ;//csBlack
    clStat[csRslt14] = clSkyBlue         ;//csBlack
    clStat[csRslt16] = (TColor)0x009D12E0;//csBlack

    clStat[csNone  ] = clWhite           ;//csWhite
    clStat[csEmpty ] = clSilver          ;//csSilver
    clStat[csUnkwn ] = clMoneyGreen      ;//csAqua
    clStat[csWork  ] = (TColor)0x00FF9595;//clHighlight       ;//csBlue
    clStat[csFail  ] = clPurple          ;//csFuchsia
    clStat[csEtc   ] = clNone            ;//etc*/

    pcMan -> ActivePageIndex = 4 ;

    btMan1_1  -> Tag = mcLDR_Home         ;btMan1_1  -> Visible = true  ;btMan1_1  -> Caption = "[" + AnsiString(mcLDR_Home         ) + "]Home  ";
    btMan1_2  -> Tag = mcLDR_Supply       ;btMan1_2  -> Visible = true  ;btMan1_2  -> Caption = "[" + AnsiString(mcLDR_Supply       ) + "]Supply";
    btMan1_3  -> Tag = mcLDR_Pick         ;btMan1_3  -> Visible = true  ;btMan1_3  -> Caption = "[" + AnsiString(mcLDR_Pick         ) + "]Pick  ";
    btMan1_4  -> Tag = mcLDR_Work         ;btMan1_4  -> Visible = true  ;btMan1_4  -> Caption = "[" + AnsiString(mcLDR_Work         ) + "]Work  ";
    btMan1_5  -> Tag = mcLDR_Place        ;btMan1_5  -> Visible = true  ;btMan1_5  -> Caption = "[" + AnsiString(mcLDR_Place        ) + "]Place ";
    btMan1_6  -> Tag = mcLDR_InAc         ;btMan1_6  -> Visible = true  ;btMan1_6  -> Caption = "[" + AnsiString(mcLDR_InAc         ) + "]In AC ";
    btMan1_7  -> Tag = mcLDR_OutAc        ;btMan1_7  -> Visible = true  ;btMan1_7  -> Caption = "[" + AnsiString(mcLDR_OutAc        ) + "]Out AC";
    btMan1_8  -> Tag = mcLDR_CmpUD        ;btMan1_8  -> Visible = true  ;btMan1_8  -> Caption = "[" + AnsiString(mcLDR_CmpUD        ) + "]Clamp ";
    btMan1_9  -> Tag = mcLDR_PusherFB     ;btMan1_9  -> Visible = true  ;btMan1_9  -> Caption = "[" + AnsiString(mcLDR_PusherFB     ) + "]Pusher";

    btMan2_1  -> Tag = mcRAL_Home         ;btMan2_1  -> Visible = true  ;btMan2_1  -> Caption = "[" + AnsiString(mcRAL_Home         ) + "]Home"       ;
    btMan2_2  -> Tag = mcRAL_Bind         ;btMan2_2  -> Visible = true  ;btMan2_2  -> Caption = "[" + AnsiString(mcRAL_Bind         ) + "]Bind"       ;
    btMan2_3  -> Tag = mcRAL_UnBind       ;btMan2_3  -> Visible = true  ;btMan2_3  -> Caption = "[" + AnsiString(mcRAL_UnBind       ) + "]UnBind"     ;
    btMan2_4  -> Tag = mcRAL_PreStprUD    ;btMan2_4  -> Visible = true  ;btMan2_4  -> Caption = "[" + AnsiString(mcRAL_PreStprUD    ) + "]Stopper1 UD";
    btMan2_5  -> Tag = mcRAL_Wk1StprUD    ;btMan2_5  -> Visible = true  ;btMan2_5  -> Caption = "[" + AnsiString(mcRAL_Wk1StprUD    ) + "]Stopper2 UD";
    btMan2_6  -> Tag = mcRAL_Wk2StprUD    ;btMan2_6  -> Visible = true  ;btMan2_6  -> Caption = "[" + AnsiString(mcRAL_Wk2StprUD    ) + "]Stopper3 UD";
    btMan2_7  -> Tag = mcRAL_Wk1LiftUD    ;btMan2_7  -> Visible = true  ;btMan2_7  -> Caption = "[" + AnsiString(mcRAL_Wk1LiftUD    ) + "]Lift1 UD"   ;
    btMan2_8  -> Tag = mcRAL_Wk2LiftUD    ;btMan2_8  -> Visible = true  ;btMan2_8  -> Caption = "[" + AnsiString(mcRAL_Wk2LiftUD    ) + "]Lift2 UD"   ;
  //btMan2_9  -> Tag = mcRAL_In           ;btMan2_9  -> Visible = true  ;btMan2_9  -> Caption = "[" + AnsiString(mcRAL_In           ) + "]In     "    ;
  //btMan2_10 -> Tag = mcRAL_MvAuto       ;btMan2_10 -> Visible = true  ;btMan2_10 -> Caption = "[" + AnsiString(mcRAL_MvAuto       ) + "]Move   "    ;

    btMan3_1   -> Tag = mcHED_Home        ;btMan3_1  -> Visible = true  ;btMan3_1  -> Caption = "[" + AnsiString(mcHED_Home         ) + "]Home"       ;
    btMan3_2   -> Tag = mcHED_PchXp       ;btMan3_2  -> Visible = true  ;btMan3_2  -> Caption = "[" + AnsiString(mcHED_PchXp        ) + "]Head X+"    ;
    btMan3_3   -> Tag = mcHED_PchXn       ;btMan3_3  -> Visible = true  ;btMan3_3  -> Caption = "[" + AnsiString(mcHED_PchXn        ) + "]Head X-"    ;
    btMan3_4   -> Tag = mcHED_PchYp       ;btMan3_4  -> Visible = true  ;btMan3_4  -> Caption = "[" + AnsiString(mcHED_PchYp        ) + "]Head Y+"    ;
    btMan3_5   -> Tag = mcHED_PchYn       ;btMan3_5  -> Visible = true  ;btMan3_5  -> Caption = "[" + AnsiString(mcHED_PchYn        ) + "]Head Y-"    ;
    btMan3_6   -> Tag = mcHED_StartPos    ;btMan3_6  -> Visible = true  ;btMan3_6  -> Caption = "[" + AnsiString(mcHED_Insp         ) + "]Start Pos"  ;
  //btMan3_7   -> Tag = mcHED_Insp        ;btMan3_7  -> Visible = true  ;btMan3_7  -> Caption = "[" + AnsiString(mcHED_Insp         ) + "]Insp"       ;
  //btMan3_8   -> Tag = mcHED_InspMan     ;btMan3_8  -> Visible = true  ;btMan3_8  -> Caption = "[" + AnsiString(mcHED_InspMan      ) + "]Insp Man"   ;


    btMan4_1  -> Tag = mcPSB_Home         ;btMan4_1  -> Visible = true  ;btMan4_1  -> Caption = "[" + AnsiString(mcPSB_Home         ) + "]Home"      ;
    btMan4_2  -> Tag = mcPSB_Work         ;btMan4_2  -> Visible = true  ;btMan4_2  -> Caption = "[" + AnsiString(mcPSB_Work         ) + "]Trim One"  ;
    btMan4_3  -> Tag = mcPSB_FingerFB     ;btMan4_3  -> Visible = true  ;btMan4_3  -> Caption = "[" + AnsiString(mcPSB_FingerFB     ) + "]Finger FB" ;
    btMan4_4  -> Tag = mcPSB_AlignFB      ;btMan4_4  -> Visible = true  ;btMan4_4  -> Caption = "[" + AnsiString(mcPSB_AlignFB      ) + "]Align FB"  ;
    btMan4_5  -> Tag = mcPSB_FlprFB       ;btMan4_5  -> Visible = true  ;btMan4_5  -> Caption = "[" + AnsiString(mcPSB_FlprFB       ) + "]Flipper FB";
    btMan4_6  -> Tag = mcPSB_FlprCmpFB    ;btMan4_6  -> Visible = true  ;btMan4_6  -> Caption = "[" + AnsiString(mcPSB_FlprCmpFB    ) + "]Clamp"     ;
    btMan4_7  -> Tag = mcPSB_PusherUD     ;btMan4_7  -> Visible = true  ;btMan4_7  -> Caption = "[" + AnsiString(mcPSB_PusherUD     ) + "]Pusher UD" ;
    btMan4_8  -> Tag = mcPSB_PusherFB     ;btMan4_8  -> Visible = true  ;btMan4_8  -> Caption = "[" + AnsiString(mcPSB_PusherFB     ) + "]Pusher FB" ;
//  btMan4_9  -> Tag = mcPSB_Flip         ;btMan4_9  -> Visible = true  ;btMan4_9  -> Caption = "[" + AnsiString(mcPSB_Flip         ) + "]Flip"      ;
//  btMan4_10 -> Tag = mcPSB_Out          ;btMan4_10 -> Visible = true  ;btMan4_10 -> Caption = "[" + AnsiString(mcPSB_Out          ) + "]Out"       ;


    btMan7_1  -> Tag = mcPSB_Home         ;btMan7_1  -> Visible = true  ;btMan7_1  -> Caption = "[" + AnsiString(mcPSB_Home         ) + "]Home    "  ;
    btMan7_2  -> Tag = mcPSB_PchXp        ;btMan7_2  -> Visible = true  ;btMan7_2  -> Caption = "[" + AnsiString(mcPSB_PchXp        ) + "]Trim X+ "  ;
    btMan7_3  -> Tag = mcPSB_PchXn        ;btMan7_3  -> Visible = true  ;btMan7_3  -> Caption = "[" + AnsiString(mcPSB_PchXn        ) + "]Trim X- "  ;
    btMan7_4  -> Tag = mcPSB_PchYp        ;btMan7_4  -> Visible = true  ;btMan7_4  -> Caption = "[" + AnsiString(mcPSB_PchYp        ) + "]Trim Y+ "  ;
    btMan7_5  -> Tag = mcPSB_PchYn        ;btMan7_5  -> Visible = true  ;btMan7_5  -> Caption = "[" + AnsiString(mcPSB_PchYn        ) + "]Trim Y- "  ;
    btMan7_6  -> Tag = mcPSB_TrmFB        ;btMan7_6  -> Visible = true  ;btMan7_6  -> Caption = "[" + AnsiString(mcPSB_TrmFB        ) + "]Trim UD "  ;
    btMan7_7  -> Tag = mcPSB_Work         ;btMan7_7  -> Visible = true  ;btMan7_7  -> Caption = "[" + AnsiString(mcPSB_Work         ) + "]Trim One"  ;
    btMan7_8  -> Tag = mcPSB_WorkStartPos ;btMan7_8  -> Visible = true  ;btMan7_8  -> Caption = "[" + AnsiString(mcPSB_WorkStartPos ) + "]Start Pos" ;

    btMan5_1  -> Tag = mcULD_Home         ;btMan5_1  -> Visible = true  ;btMan5_1  -> Caption = "[" + AnsiString(mcULD_Home         ) + "]Home  "  ;
    btMan5_2  -> Tag = mcULD_Supply       ;btMan5_2  -> Visible = true  ;btMan5_2  -> Caption = "[" + AnsiString(mcULD_Supply       ) + "]Supply"  ;
    btMan5_3  -> Tag = mcULD_Pick         ;btMan5_3  -> Visible = true  ;btMan5_3  -> Caption = "[" + AnsiString(mcULD_Pick         ) + "]Pick  "  ;
    btMan5_4  -> Tag = mcULD_Work         ;btMan5_4  -> Visible = true  ;btMan5_4  -> Caption = "[" + AnsiString(mcULD_Work         ) + "]Work  "  ;
    btMan5_5  -> Tag = mcULD_Place        ;btMan5_5  -> Visible = true  ;btMan5_5  -> Caption = "[" + AnsiString(mcULD_Place        ) + "]Place "  ;
    btMan5_6  -> Tag = mcULD_InAc         ;btMan5_6  -> Visible = true  ;btMan5_6  -> Caption = "[" + AnsiString(mcULD_InAc         ) + "]In AC "  ;
    btMan5_7  -> Tag = mcULD_OutAc        ;btMan5_7  -> Visible = true  ;btMan5_7  -> Caption = "[" + AnsiString(mcULD_OutAc        ) + "]Out AC"  ;
    btMan5_8  -> Tag = mcULD_CmpUD        ;btMan5_8  -> Visible = true  ;btMan5_8  -> Caption = "[" + AnsiString(mcULD_CmpUD        ) + "]Clamp "  ;
    btMan5_9  -> Tag = mcULD_WorkPos      ;btMan5_9  -> Visible = true  ;btMan5_9  -> Caption = "[" + AnsiString(mcULD_WorkPos      ) + "]Work Pos";
    btMan5_10 -> Tag = mcULD_CmpPchUp     ;btMan5_10 -> Visible = true  ;btMan5_10 -> Caption = "[" + AnsiString(mcULD_CmpUD        ) + "]Pitch UP";
    btMan5_11 -> Tag = mcULD_CmpPchDn     ;btMan5_11 -> Visible = true  ;btMan5_11 -> Caption = "[" + AnsiString(mcULD_CmpUD        ) + "]Pitch DN";

    btMan6_1  -> Tag = mcAllHome          ;btMan6_1  -> Visible = true  ;btMan6_1  -> Caption = "[" + AnsiString(mcAllHome          ) + "]ALL HOME";

    pcMan -> ActivePage = tsETC ;

    //Load Language
//    LanguageSel(FM_GetLan());

    SEQ.Reset();
}




//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true;

    /*if(FM_GetLevel() != lvControl)*/
    FM_SetLevel(lvOperator);

    lbLoader -> Visible    = OM.EqpOptn.bExistLoader ;
    pnLoader -> Visible    = OM.EqpOptn.bExistLoader ;
    tsLoader -> TabVisible = OM.EqpOptn.bExistLoader ;

    edCheckSFailCnt -> Text = OM.CmnOptn.iCheckSFailCnt;
    edCheckFailCnt  -> Text = OM.CmnOptn.iCheckFailCnt ;


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
//    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag)) ;
//    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag));
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


//    Panel12 -> Color = IO_GetY(yPRB_Belt) ? clLime : clGray ;

//    btPrbLotInput -> Visible = !SEQ._bRun;
//    btPsbLotInput -> Visible = !SEQ._bRun;
//    btVs1LotInput -> Visible = !SEQ._bRun;
//    btVs2LotInput -> Visible = !SEQ._bRun;

    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
/*    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR";
//                          if( iLev != lvOperator) {
//                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
//                                  DM.ARAY[i].PopupMenuEnable(false);
//                              }
//                              iLev = lvOperator ;
//                          }
                          break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER";
//                          if( iLev != lvEngineer) {
//                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
//                                  DM.ARAY[i].PopupMenuEnable(true);
//                              }
//                              iLev = lvEngineer ;
//                          }
                          break ;
        case lvMaster   : btOperator -> Caption = " MASTER ";
//                          if( iLev != lvMaster) {
//                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
//                                  DM.ARAY[i].PopupMenuEnable(true);
//                              }
//                              iLev = lvMaster ;
//                          }
                          break ;
        //case lvControl  : btOperatior -> Caption = " CONTROL" ;
        //                  break ;

        default         : btOperator -> Caption = " ERROR  ";
                          break ;
    }
*/
    switch (iLevel) {
        case lvOperator : if( iLev != lvOperator) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if( i != (EN_ARAY_ID)riULD ) DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvOperator ;
                          }
                          btOperator -> Caption = "OPERATOR";
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if( i != (EN_ARAY_ID)riULD ) DM.ARAY[i].PopupMenuEnable(false);
                                  //DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iLev = lvEngineer ;
                          }
                          btOperator -> Caption = "ENGINEER";
                          break ;
        case lvMaster   : if( iLev != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if( i != (EN_ARAY_ID)riULD ) DM.ARAY[i].PopupMenuEnable(true);
                                  //DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iLev = lvMaster ;
                          }
                          btOperator -> Caption = " MASTER ";
/*
        case lvControl  : if( iLev != lvControl) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if( i != (EN_ARAY_ID)riULD ) DM.ARAY[i].PopupMenuEnable(true);
                                  //DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iLev = lvMaster ;
                          }
*/
    }


    //Trim Button
//    btNoTrimPSB -> Visible = FM_GetLevel() != lvOperator ;
    btNoTrimPSB -> Visible = false ;
    btNoTrimWK2 -> Visible = false ; //FM_GetLevel() != lvOperator ;
    btNoTrimWK1 -> Visible = false ; //FM_GetLevel() != lvOperator ;

    tsVs1 -> TabVisible = FM_GetLevel() >= lvMaster ;
    tsVs2 -> TabVisible = FM_GetLevel() >= lvMaster ;

    btPrbLotInput -> Visible = FM_GetLevel() >= lvMaster ;
    btPsbLotInput -> Visible = FM_GetLevel() >= lvMaster ;
    btVs1LotInput -> Visible = FM_GetLevel() >= lvMaster ;
    btVs2LotInput -> Visible = FM_GetLevel() >= lvMaster ;

    //Option Trim
    pnNoTrmFail -> Visible = FM_GetLevel() != lvOperator ;

    //Clear Button
    btPrbClear  -> Visible = true ;//FM_GetLevel() != lvOperator ;  //꺼낸 는 것은 굳이 상관없겠다 싶다. 찌그러지면 오퍼도 빼야 하니깐.
    btPsbClear  -> Visible = true ;//FM_GetLevel() != lvOperator ;
    btVs1Clear  -> Visible = true ;//FM_GetLevel() != lvOperator ;
    btVs2Clear  -> Visible = true ;//FM_GetLevel() != lvOperator ;

    //Lot No , Mgz No , Slot No
//    edPrbLotNo  -> Visible = false ; //!SEQ._bRun;
//    edPrbMgzNo  -> Visible = false ; //!SEQ._bRun;
//    edPrbSlotNo -> Visible = false ; //!SEQ._bRun;
//
//    edPsbLotNo  -> Visible = false ; //!SEQ._bRun;
//    edPsbMgzNo  -> Visible = false ; //!SEQ._bRun;
//    edPsbSlotNo -> Visible = false ; //!SEQ._bRun;
//
//    edVs1LotNo  -> Visible = false ; //!SEQ._bRun;
//    edVs1MgzNo  -> Visible = false ; //!SEQ._bRun;
//    edVs1SlotNo -> Visible = false ; //!SEQ._bRun;
//
//    edVs2LotNo  -> Visible = false ; //!SEQ._bRun;
//    edVs2MgzNo  -> Visible = false ; //!SEQ._bRun;
//    edVs2SlotNo -> Visible = false ; //!SEQ._bRun;
//
//    btPrbLotInput -> Visible = false ; //!SEQ._bRun;
//    btPsbLotInput -> Visible = false ; //!SEQ._bRun;
//    btVs1LotInput -> Visible = false ; //!SEQ._bRun;
//    btVs2LotInput -> Visible = false ; //!SEQ._bRun;

//    if(SEQ._bRun) edPrbLotNo -> Text = //편하게 해주고 싶지만... 쩜쩜점

    pcMan -> Enabled = !SEQ._bRun ;

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
        pnDayDayJamTime ,
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

    pnNoTrimFail    -> Color    =  OM.CmnOptn.bNoTrimFail    ? clLime : clRed ;
    pnIgnrDoor      -> Color    =  OM.CmnOptn.bIgnrDoor      ? clLime : clRed ;
    pnVisn1Skip     -> Color    =  OM.CmnOptn.bVisn1Skip     ? clLime : clRed ;
    pnVisn2Skip     -> Color    =  OM.CmnOptn.bVisn2Skip     ? clLime : clRed ;
    pnCheckFailCnt  -> Color    =  OM.CmnOptn.bCheckFailCnt  ? clLime : clRed ;
    pnCheckSFailCnt -> Color    =  OM.CmnOptn.bCheckSFailCnt ? clLime : clRed ;
    pnTrimCheck     -> Color    =  OM.CmnOptn.bTrimCheck     ? clLime : clRed ;

    pnNoTrimFail    -> Caption  =  OM.CmnOptn.bNoTrimFail    ? "ON"   : "OFF" ;
    pnIgnrDoor      -> Caption  =  OM.CmnOptn.bIgnrDoor      ? "ON"   : "OFF" ;
    pnVisn1Skip     -> Caption  =  OM.CmnOptn.bVisn1Skip     ? "ON"   : "OFF" ;
    pnVisn2Skip     -> Caption  =  OM.CmnOptn.bVisn2Skip     ? "ON"   : "OFF" ;
    pnCheckFailCnt  -> Caption  =  OM.CmnOptn.bCheckFailCnt  ? "ON"   : "OFF" ;
    pnCheckSFailCnt -> Caption  =  OM.CmnOptn.bCheckSFailCnt ? "ON"   : "OFF" ;
    pnTrimCheck     -> Caption  =  OM.CmnOptn.bTrimCheck     ? "ON"   : "OFF" ;


    lbSetNo    -> Caption  = ADL.GetSetNo    () ;
    lbInputCnt -> Caption  = ADL.GetInputCnt () ;
    lbInputChar -> Caption = ADL.GetInputChar() ;

//    lbPRBLotNo -> Caption = "LOTNO:"+DM.ARAY[riPRB].GetLotNo() + " MGZNO:" + DM.ARAY[riPRB].GetID().ToIntDef(9999)/100+ " SLOTNO:" + DM.ARAY[riPRB].GetID().ToIntDef(99)%100 ;
//    lbWK1LotNo -> Caption = "LOTNO:"+DM.ARAY[riWK1].GetLotNo() + " MGZNO:" + DM.ARAY[riWK1].GetID().ToIntDef(9999)/100+ " SLOTNO:" + DM.ARAY[riWK1].GetID().ToIntDef(99)%100 ;
//    lbWK2LotNo -> Caption = "LOTNO:"+DM.ARAY[riWK2].GetLotNo() + " MGZNO:" + DM.ARAY[riWK2].GetID().ToIntDef(9999)/100+ " SLOTNO:" + DM.ARAY[riWK2].GetID().ToIntDef(99)%100 ;
//    lbPSBLotNo -> Caption = "LOTNO:"+DM.ARAY[riPSB].GetLotNo() + " MGZNO:" + DM.ARAY[riPSB].GetID().ToIntDef(9999)/100+ " SLOTNO:" + DM.ARAY[riPSB].GetID().ToIntDef(99)%100 ;
    if(DM.ARAY[riPRB].GetCntExist() && !edPrbLotNo -> Focused() && !edPrbMgzNo->Focused() && !edPrbSlotNo->Focused() &&!btPrbLotInput->Focused() ) {
//        lbPre   -> Caption = "LOTNO:" + DM.ARAY[riPRB].GetLotNo() + " MGZNO:" +DM.ARAY[riPRB].GetID().ToIntDef(9999)/100 + " SLOTNO:" + DM.ARAY[riPRB].GetID().ToIntDef(99)%100;
        edPrbLotNo  -> Text = DM.ARAY[riPRB].GetLotNo() ;
        edPrbMgzNo  -> Text = DM.ARAY[riPRB].GetID().ToIntDef(9999)/100 ;
        edPrbSlotNo -> Text = DM.ARAY[riPRB].GetID().ToIntDef(99)%100;
    }
    if(DM.ARAY[riWK1].GetCntExist() && !edVs1LotNo -> Focused() && !edVs1MgzNo->Focused() && !edVs1SlotNo->Focused() &&!btVs1LotInput->Focused()) {
//        lbPre   -> Caption = "LOTNO:" + DM.ARAY[riWK1].GetLotNo() + " MGZNO:" +DM.ARAY[riWK1].GetID().ToIntDef(9999)/100 + " SLOTNO:" + DM.ARAY[riWK1].GetID().ToIntDef(99)%100;
        edVs1LotNo  -> Text = DM.ARAY[riWK1].GetLotNo() ;
        edVs1MgzNo  -> Text = DM.ARAY[riWK1].GetID().ToIntDef(9999)/100 ;
        edVs1SlotNo -> Text = DM.ARAY[riWK1].GetID().ToIntDef(99)%100;
    }
    if(DM.ARAY[riWK2].GetCntExist() && !edVs2LotNo->Focused() && !edVs2MgzNo->Focused() && !edVs2SlotNo->Focused() && !btVs2LotInput->Focused() ) {
//        lbInsp  -> Caption  = "LOTNO:" + DM.ARAY[riWK2].GetLotNo() + " MGZNO:" +DM.ARAY[riWK2].GetID().ToIntDef(9999)/100 + " SLOTNO:" + DM.ARAY[riWK2].GetID().ToIntDef(99)%100;
        edVs2LotNo  -> Text = DM.ARAY[riWK2].GetLotNo() ;
        edVs2MgzNo  -> Text = DM.ARAY[riWK2].GetID().ToIntDef(9999)/100 ;
        edVs2SlotNo -> Text = DM.ARAY[riWK2].GetID().ToIntDef(99)%100;
    }
    if(DM.ARAY[riPSB].GetCntExist() && !edPsbLotNo->Focused() && !edPsbMgzNo->Focused() && !edPsbSlotNo->Focused() && !btPsbLotInput->Focused() ) {
//        lbInsp -> Caption   = "LOTNO:" + DM.ARAY[riPSB].GetLotNo() + " MGZNO:" +DM.ARAY[riPSB].GetID().ToIntDef(9999)/100 + " SLOTNO:" + DM.ARAY[riPSB].GetID().ToIntDef(99)%100;
        edPsbLotNo  -> Text = DM.ARAY[riPSB].GetLotNo() ;
        edPsbMgzNo  -> Text = DM.ARAY[riPSB].GetID().ToIntDef(9999)/100 ;
        edPsbSlotNo -> Text = DM.ARAY[riPSB].GetID().ToIntDef(99)%100;
    }


    if(SEQ._bInspDispr) { CheckStripDisappear(); SEQ._bInspDispr = false; }
    tmUpdate->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnDataMapMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
//    DM.SetPopupMenu(Panel2,X,Y);
}
//---------------------------------------------------------------------------


//int iSelWhre ;
//TImage * pSelImage ;
//bool     bMouseDown ;
//int iSelX1 ;
//int iSelY1 ;
//int iSelX2 ;
//int iSelY2 ;
//
//int iSelR1 , iSelC1 ;
//int iSelR2 , iSelC2 ;


void __fastcall TFrmOperation::imBuff1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //if (SEQ._bRun) return ;

//    iSelWhre    = ((TImage *)Sender) -> Tag ;

    //tmMapUpdate -> Enabled = false ;

/*    bMouseDown = true;

    pSelImage ->Canvas->MoveTo(X, Y);
    iSelX1 = X;
    iSelY1 = Y;
    iSelX2 = X;
    iSelY2 = Y;

    DM.ImageArayRC((EN_ARAY_ID)iSelWhre , pSelImage , iSelX1 , iSelY1 , iSelR1 , iSelC1 );*/
//    DM.ImageArayRC((EN_ARAY_ID)iSelWhre , pSelImage , iSelX2 , iSelY2 , iSelR2 , iSelC2 );

//    pSelImage   = (TImage *)Sender ;
//    DM.MouseDown(pSelImage,X,Y);


}

//---------------------------------------------------------------------------

void __fastcall TFrmOperation::imBuff1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
/*    if(!bMouseDown) return ;
 //   if(iSelX1 == X && iSelY1 == Y) return  ;

    TPenMode  OriMode  = pSelImage->Canvas->Pen->Mode ;
    TColor    OriColor = pSelImage->Canvas->Pen->Color;
    TPenStyle OriStyle = pSelImage->Canvas->Pen->Style;
    int       OriWidth = pSelImage->Canvas->Pen->Width;

    pSelImage->Canvas->Pen->Mode  = pmNotXor;
    pSelImage->Canvas->Pen->Color = clBlack;
    pSelImage->Canvas->Pen->Style = psDot ;
    pSelImage->Canvas->Pen->Width = 1;
    pSelImage->Canvas->Rectangle(iSelX1, iSelY1, iSelX2, iSelY2);

    iSelX2 = X ;
    iSelY2 = Y ;

    DM.ImageArayRC((EN_ARAY_ID)iSelWhre , pSelImage , iSelX2 , iSelY2 , iSelR2 , iSelC2 );
    pSelImage->Canvas->Rectangle(iSelX1, iSelY1, iSelX2, iSelY2);

    pSelImage->Canvas->Pen->Mode  = OriMode  ;
    pSelImage->Canvas->Pen->Color = OriColor ;
    pSelImage->Canvas->Pen->Style = OriStyle ;
    pSelImage->Canvas->Pen->Width = OriWidth ;
*/
//    pSelImage   = (TImage *)Sender ;
//    DM.MouseMove(pSelImage,X,Y);
    //DM.UpdateAray(imBuff1 , aiBuff1 );
}

//---------------------------------------------------------------------------

void __fastcall TFrmOperation::imBuff1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
//    if(!bMouseDown) return ;
//    bMouseDown = false;

    //tmMapUpdate -> Enabled = true ;
//    DM.UpdateAray(imBuff1 , aiBuff1 );
//    DM.MouseUp(X , Y , Shift);
}

//---------------------------------------------------------------------------


void __fastcall TFrmOperation::pcManChange(TObject *Sender)
{
//.=
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
    if(SEQ._bRun) {FM_MsgOk("ERROR" , "Can't Use While Running"); return ;}
    FrmLotOpen -> ShowModal() ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
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

void __fastcall TFrmOperation::btLotEndClick(TObject *Sender)
{
    if(!LT.m_bLotOpen) {FM_MsgOk("ERROR" , "Can't Use While Work Closed"); return ;}
    if(FM_MsgYesNo("Confirm","Do you really want to LotEnd?") != mrYes) return ;

    LT.LotEnd();
    DM.ClearMap();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnDryRunDblClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

    AnsiString sMsg ;

    switch(iIndex) {
        case 0 : if(FM_GetLevel() == lvOperator) return ; //bNoTrimFail
        case 1 :                                 break  ; //bIgnrDoor
        case 2 :                                 break  ; //bVisn1Skip
        case 3 :                                 break  ; //bVisn2Skip
        case 4 :                                 break  ; //bCheckSFailCnt
        case 5 :                                 break  ; //bCheckFailCnt
    }

    if (FM_MsgYesNo("Confirm", "Would you like to invert the flag?") != mrYes) return ;

    switch(iIndex) {
        case 0 : OM.CmnOptn.bNoTrimFail    = !OM.CmnOptn.bNoTrimFail    ; sMsg = "NoTrimFail "    + AnsiString(OM.CmnOptn.bNoTrimFail    ? "On" : "Off") ; break ;
        case 1 : OM.CmnOptn.bIgnrDoor      = !OM.CmnOptn.bIgnrDoor      ; sMsg = "IgnrDoor "      + AnsiString(OM.CmnOptn.bIgnrDoor      ? "On" : "Off") ; break ;
        case 2 : OM.CmnOptn.bVisn1Skip     = !OM.CmnOptn.bVisn1Skip     ; sMsg = "Visn1Skip "     + AnsiString(OM.CmnOptn.bVisn1Skip     ? "On" : "Off") ; break ;
        case 3 : OM.CmnOptn.bVisn2Skip     = !OM.CmnOptn.bVisn2Skip     ; sMsg = "Visn2Skip "     + AnsiString(OM.CmnOptn.bVisn2Skip     ? "On" : "Off") ; break ;
        case 4 : OM.CmnOptn.bCheckSFailCnt = !OM.CmnOptn.bCheckSFailCnt ; sMsg = "CheckSFailCnt " + AnsiString(OM.CmnOptn.bCheckSFailCnt ? "On" : "Off") ; break ;
        case 5 : OM.CmnOptn.bCheckFailCnt  = !OM.CmnOptn.bCheckFailCnt  ; sMsg = "CheckFailCnt "  + AnsiString(OM.CmnOptn.bCheckFailCnt  ? "On" : "Off") ; break ;
        case 6 : OM.CmnOptn.bTrimCheck     = !OM.CmnOptn.bTrimCheck     ; sMsg = "bTrimCheck   "  + AnsiString(OM.CmnOptn.bTrimCheck     ? "On" : "Off") ; break ;
    }

    AnsiString sLotNo , sSlotNo ;
    sLotNo  = "PreBuff:" + DM.ARAY[riPRB].GetLotNo() + " ";
    sLotNo += "Visn1:"   + DM.ARAY[riWK1].GetLotNo() + " ";
    sLotNo += "Visn2:"   + DM.ARAY[riWK2].GetLotNo() + " ";
    sLotNo += "PstBuff:" + DM.ARAY[riPSB].GetLotNo() + " ";

    sSlotNo  = "PreBuff:" + AnsiString(DM.ARAY[riPRB].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "Visn1:"   + AnsiString(DM.ARAY[riWK1].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "Visn2:"   + AnsiString(DM.ARAY[riWK2].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "PstBuff:" + AnsiString(DM.ARAY[riPSB].GetID().ToIntDef(99)%100) + " ";
    SPC.SaveOptionLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    OM.SaveCmnOptn();

    //NoTrimFail
    if(iIndex == 1 && OM.CmnOptn.bNoTrimFail && !DM.ARAY[riPSB].CheckAllStat(csNone) ) {
        if( mrYes ==FM_MsgYesNo("Confirm","Would you like to Skip Trimming PostBuffer" )) {
            DM.ARAY[riPSB].SetStat(csWork);
        }
    }


}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btManClick(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btVisn1ResetClick(TObject *Sender)
{
    if(!OM.CmnOptn.bVisn1Skip)VC.SendReset(vrVisn1);
    if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
    if(DM.ARAY[riWK1].GetCntExist())DM.ARAY[riWK1].ChangeStat(csWork,csUnkwn) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button5Click(TObject *Sender)
{
    if(!OM.CmnOptn.bVisn2Skip)VC.SendReset(vrVisn2);
    if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
    if(DM.ARAY[riWK2].GetCntExist())DM.ARAY[riWK2].ChangeStat(csWork,csUnkwn) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPrbClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo ,sMsg;
    sLotNo   = "PreBuff:" + DM.ARAY[riPRB].GetLotNo() + " ";
    sSlotNo  = "PreBuff:" + AnsiString(DM.ARAY[riPRB].GetID().ToIntDef(99)%100) + " ";
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    DM.ARAY[riPRB].SetStat(csNone) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPsbClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo , sMsg;
    sLotNo   = "PstBuff:" + DM.ARAY[riPSB].GetLotNo() + " ";
    sSlotNo  = "PstBuff:" + AnsiString(DM.ARAY[riPSB].GetID().ToIntDef(99)%100) + " ";
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    DM.ARAY[riPSB].SetStat(csNone) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btVs1ClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo ,sMsg;
    sLotNo   = "Vsn1Buff:" + DM.ARAY[riWK1].GetLotNo() + " ";
    sSlotNo  = "Vsn1Buff:" + AnsiString(DM.ARAY[riWK1].GetID().ToIntDef(99)%100) + " ";
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    if(!OM.CmnOptn.bVisn1Skip)VC.SendReset(vrVisn1);
    DM.ARAY[riVS1].SetStat(csNone) ;
    DM.ARAY[riWK1].SetStat(csNone) ;
    if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
    if(DM.ARAY[riWK2].GetCntExist())DM.ARAY[riWK2].ChangeStat(csWork , csUnkwn) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btVs2ClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo , sMsg;
    sLotNo   = "Vsn2Buff:" + DM.ARAY[riWK2].GetLotNo() + " ";
    sSlotNo  = "Vsn2Buff:" + AnsiString(DM.ARAY[riWK2].GetID().ToIntDef(99)%100) + " ";
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    if(!OM.CmnOptn.bVisn2Skip)VC.SendReset(vrVisn2);
    DM.ARAY[riVS2].SetStat(csNone) ;
    DM.ARAY[riWK2].SetStat(csNone) ;
    if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
    if(DM.ARAY[riWK1].GetCntExist())DM.ARAY[riWK1].ChangeStat(csWork , csUnkwn) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::FormCreate(TObject *Sender)
{
    double dTrgPos[1] ={1.0};
    MT_SetAxtTrgPos(miWRK_XVsn , 1 , dTrgPos , 1000 , true , true) ;
    MT_ResetAxtTrgPos(miWRK_XVsn) ;

    if(DM.ARAY[riVS1].GetCntExist()) DM.ARAY[riVS1].SetStat(csUnkwn);
    if(DM.ARAY[riVS2].GetCntExist()) DM.ARAY[riVS2].SetStat(csUnkwn);

    //그냥 홈잡을때만 하자.
//         if(!OM.CmnOptn.bVisn1Skip && !OM.CmnOptn.bVisn2Skip)VC.SendReset(vrBoth ) ;
//    else if( OM.CmnOptn.bVisn1Skip && !OM.CmnOptn.bVisn2Skip)VC.SendReset(vrVisn2) ;
//    else if(!OM.CmnOptn.bVisn1Skip &&  OM.CmnOptn.bVisn2Skip)VC.SendReset(vrVisn1) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button6Click(TObject *Sender)
{
//    LT.WriteLotLog();
    SPC.WriteErrData(EM_GetLastErr(),true);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button7Click(TObject *Sender)
{
//    SPC.WriteErrData(EM_GetLastErr(),false);
    LT.WriteLotDayLog();
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btNoTrimWK2Click(TObject *Sender)
{
    if(DM.ARAY[riWK2].CheckAllStat(csNone)) return ;

    DM.ARAY[riWK2].SetStat(csWork);
    DM.ARAY[riVS2].SetStat(csWork);

    AnsiString sMsg ;
    sMsg = "VS2 Trim Skip" ;

    AnsiString sLotNo , sSlotNo ;
    sLotNo  = "PreBuff:" + DM.ARAY[riPRB].GetLotNo() + " ";
    sLotNo += "Visn1:"   + DM.ARAY[riWK1].GetLotNo() + " ";
    sLotNo += "Visn2:"   + DM.ARAY[riWK2].GetLotNo() + " ";
    sLotNo += "PstBuff:" + DM.ARAY[riPSB].GetLotNo() + " ";

    sSlotNo  = "PreBuff:" + AnsiString(DM.ARAY[riPRB].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "Visn1:"   + AnsiString(DM.ARAY[riWK1].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "Visn2:"   + AnsiString(DM.ARAY[riWK2].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "PstBuff:" + AnsiString(DM.ARAY[riPSB].GetID().ToIntDef(99)%100) + " ";
    SPC.SaveOptionLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btNoTrimWK1Click(TObject *Sender)
{
    if(DM.ARAY[riWK1].CheckAllStat(csNone)) return ;

    DM.ARAY[riWK1].SetStat(csWork);
    DM.ARAY[riVS1].SetStat(csWork);

    AnsiString sMsg ;
    sMsg = "VS1 Trim Skip" ;

    AnsiString sLotNo , sSlotNo ;
    sLotNo  = "PreBuff:" + DM.ARAY[riPRB].GetLotNo() + " ";
    sLotNo += "Visn1:"   + DM.ARAY[riWK1].GetLotNo() + " ";
    sLotNo += "Visn2:"   + DM.ARAY[riWK2].GetLotNo() + " ";
    sLotNo += "PstBuff:" + DM.ARAY[riPSB].GetLotNo() + " ";

    sSlotNo  = "PreBuff:" + AnsiString(DM.ARAY[riPRB].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "Visn1:"   + AnsiString(DM.ARAY[riWK1].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "Visn2:"   + AnsiString(DM.ARAY[riWK2].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "PstBuff:" + AnsiString(DM.ARAY[riPSB].GetID().ToIntDef(99)%100) + " ";
    SPC.SaveOptionLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btNoTrimPSBClick(TObject *Sender)
{
    if(DM.ARAY[riPSB].CheckAllStat(csNone)) return ;

    DM.ARAY[riPSB].SetStat(csWork);


    AnsiString sMsg ;
    sMsg = "Post Buffer Trim Skip" ;

    AnsiString sLotNo , sSlotNo ;
    sLotNo  = "PreBuff:" + DM.ARAY[riPRB].GetLotNo() + " ";
    sLotNo += "Visn1:"   + DM.ARAY[riWK1].GetLotNo() + " ";
    sLotNo += "Visn2:"   + DM.ARAY[riWK2].GetLotNo() + " ";
    sLotNo += "PstBuff:" + DM.ARAY[riPSB].GetLotNo() + " ";

    sSlotNo  = "PreBuff:" + AnsiString(DM.ARAY[riPRB].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "Visn1:"   + AnsiString(DM.ARAY[riWK1].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "Visn2:"   + AnsiString(DM.ARAY[riWK2].GetID().ToIntDef(99)%100) + " ";
    sSlotNo += "PstBuff:" + AnsiString(DM.ARAY[riPSB].GetID().ToIntDef(99)%100) + " ";
    SPC.SaveOptionLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);
}
//---------------------------------------------------------------------------



/*
    OM.CmnOptn.iCheckSFailCnt = edCheckSFailCnt -> Text.ToIntDef(OM.CmnOptn.iCheckSFailCnt);
    OM.CmnOptn.iCheckFailCnt  = edCheckFailCnt  -> Text.ToIntDef(OM.CmnOptn.iCheckFailCnt );

    edCheckSFailCnt -> Text = OM.CmnOptn.iCheckSFailCnt;
    edCheckFailCnt  -> Text = OM.CmnOptn.iCheckFailCnt ;

    OM.SaveCmnOptn();
*/
void __fastcall TFrmOperation::edCheckSFailCntChange(TObject *Sender)
{
    OM.CmnOptn.iCheckSFailCnt = edCheckSFailCnt -> Text.ToIntDef(OM.CmnOptn.iCheckSFailCnt);

    edCheckSFailCnt -> Text = OM.CmnOptn.iCheckSFailCnt;

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::edCheckFailCntChange(TObject *Sender)
{
    OM.CmnOptn.iCheckFailCnt  = edCheckFailCnt  -> Text.ToIntDef(OM.CmnOptn.iCheckFailCnt );

    edCheckFailCnt  -> Text = OM.CmnOptn.iCheckFailCnt ;

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
bool __fastcall TFrmOperation::CheckStripDisappear()
{
    //스트립 사라짐 에러.
    if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[PreBuffer Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riPRB ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if( DM.ARAY[riWK1 ].CheckExist(0,0) && !IO_GetX(xWK1_Pkg) && AT_Complete(aiWK1_Lift , ccBwd) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision1 Buffer Strip Disappear] \nDo you want to clear Strip?") == mrYes ) { DM.ARAY[riWK1 ].SetStat(csNone); DM.ARAY[riVS1 ].SetStat(csNone);SEQ._bBtnStart = true ;}
    }
    else if( DM.ARAY[riWK2 ].CheckExist(0,0) && !IO_GetX(xWK2_Pkg) && AT_Complete(aiWK2_Lift , ccBwd) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision2 Buffer Strip Disappear] \nDo you want to clear Strip?") == mrYes ) { DM.ARAY[riWK2 ].SetStat(csNone); DM.ARAY[riVS2 ].SetStat(csNone);SEQ._bBtnStart = true ;}
    }

    //    else if(!DM.ARAY[riULD ].CheckAllStat(csNone) && (!IO_GetX(xULD_CmpMgz1) || !IO_GetX(xULD_CmpMgz2)) return false ;
    else if(!DM.ARAY[riULD ].CheckAllStat(csNone) && (!IO_GetX(xULD_CmpMgz1) && !IO_GetX(xULD_CmpMgz2)) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Unloader Buffer Mgz Disappear] \nDo you want to clear Mgz?") == mrYes ) { DM.ARAY[riULD ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }



    //else if(DM.ARAY[riPSB].GetCntExist () && !IO_GetX(xPSB_Pkg) && !OM.CmnOptn.bDryRun) {
    //    if(FM_MsgYesNo("Err " , "[PostBuffer Strip Disappear] \nDo you want to clear Strip?"    ) == mrYes ) { DM.ARAY[riPSB ].SetStat(csNone); SEQ._bBtnStart = true ;}
    //}
    return true ;
}

void __fastcall TFrmOperation::btPrbLotInputClick(TObject *Sender)
{
    if(SEQ._bRun) return ;

    if(FM_MsgYesNo("Confirm","Did you want to enter Lot information ") != mrYes) return ;

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
            DM.ARAY[riPRB].SetLotNo(sLotNo) ;
            DM.ARAY[riPRB].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 2 :
            sLotNo  = edVs1LotNo ->Text ;
            iMgzNo  = edVs1MgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edVs1SlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riWK1].SetLotNo(sLotNo) ;
            DM.ARAY[riWK1].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 3 :
            sLotNo  = edVs2LotNo ->Text ;
            iMgzNo  = edVs2MgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edVs2SlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riWK2].SetLotNo(sLotNo) ;
            DM.ARAY[riWK2].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 4 :
            sLotNo  = edPsbLotNo ->Text ;
            iMgzNo  = edPsbMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edPsbSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riPSB].SetLotNo(sLotNo) ;
            DM.ARAY[riPSB].SetID(iMgzNo *100 + iSlotNo);
            break ;
     }


}
//---------------------------------------------------------------------------
bool __fastcall TFrmOperation::LanguageSel(EN_LAN_SEL lvType)
{
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sName    , sNameTemp    ;
    AnsiString sCaption , sCaptionTemp ;
    AnsiString sComName ;
    bool       bOri     ;

    for (int i = 0; i <= MAX_LAN_SEL   ; i++)
    {
             if(i == (int)lsEnglish ) {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_E.ini" ; bOri = false ;}
        else if(i == (int)lsKorean  ) {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_K.ini" ; bOri = false ;}
        else if(i == (int)lsChinese ) {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_C.ini" ; bOri = false ;}
        else                          {sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + ".ini"   ;
                                       UserINI.ClearFile(sPath) ;
                                       bOri  = true ;                                                  }

        for (int i = 0; i < ComponentCount; i++)
        {
            if (Components[i]->ClassNameIs("TLabel"))
            {
                sComName = Components[i]->ClassName();
                sName    = ((TLabel*)FindComponent(Components[i]->Name))->Name          ;
                sCaption = ((TLabel*)FindComponent(Components[i]->Name))->Caption       ;
            }
            else if (Components[i]->ClassNameIs("TCheckBox")) {
                sComName = Components[i]->ClassName();
                sName    = ((TCheckBox*)FindComponent(Components[i]->Name))->Name       ;
                sCaption = ((TCheckBox*)FindComponent(Components[i]->Name))->Caption    ;
            }
            else if (Components[i]->ClassNameIs("TButton")) {
                sComName = Components[i]->ClassName();
                sName    = ((TButton*)FindComponent(Components[i]->Name))->Name         ;
                sCaption = ((TButton*)FindComponent(Components[i]->Name))->Caption      ;
            }
            else if (Components[i]->ClassNameIs("TBitBtn")) {
                sComName = Components[i]->ClassName();
                sName    = ((TBitBtn*)FindComponent(Components[i]->Name))->Name         ;
                sCaption = ((TBitBtn*)FindComponent(Components[i]->Name))->Caption      ;
            }
            else if (Components[i]->ClassNameIs("TSpeedButton")) {
                sComName = Components[i]->ClassName();
                sName    = ((TSpeedButton*)FindComponent(Components[i]->Name))->Name    ;
                sCaption = ((TSpeedButton*)FindComponent(Components[i]->Name))->Caption ;
            }
            else if (Components[i]->ClassNameIs("TTabSheet")) {
                sComName = Components[i]->ClassName();
                sName    = ((TTabSheet*)FindComponent(Components[i]->Name))->Name       ;
                sCaption = ((TTabSheet*)FindComponent(Components[i]->Name))->Caption    ;
            }
            else if (Components[i]->ClassNameIs("TGroupBox")) {
                sComName = Components[i]->ClassName();
                sName    = ((TGroupBox*)FindComponent(Components[i]->Name))->Name       ;
                sCaption = ((TGroupBox*)FindComponent(Components[i]->Name))->Caption    ;
            }
            else continue ;

            if(sComName == "" || sName == "") continue ;

            //Save Device.
            if(bOri) UserINI.Save(sPath.c_str()  , sComName  , sName , sCaption);
            else if(sCaption != "") {
                UserINI.Load(sPath.c_str()  , sComName  , sName , sCaptionTemp);
                if( sCaptionTemp == "" ) UserINI.Save(sPath.c_str()  , sComName  , sName , sCaption);
            }
        }
    }

    //Set Dir. Load
         if(lvType == lsEnglish ) sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_E.ini" ;
    else if(lvType == lsKorean  ) sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_K.ini" ;
    else if(lvType == lsChinese ) sPath = EXE_FOLDER + "Language\\" + Caption + "\\" + Caption + "_C.ini" ;
    else                          return true ;

    for (int i = 0; i < ComponentCount; i++)
    {
        if (Components[i]->ClassNameIs("TLabel"))
        {
            sComName = Components[i]->ClassName();
            sName    = ((TLabel*)FindComponent(Components[i]->Name))->Name          ;
            sCaption = ((TLabel*)FindComponent(Components[i]->Name))->Caption       ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TLabel*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TLabel*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
        else if (Components[i]->ClassNameIs("TCheckBox")) {
            sComName = Components[i]->ClassName();
            sName    = ((TCheckBox*)FindComponent(Components[i]->Name))->Name       ;
            sCaption = ((TCheckBox*)FindComponent(Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TCheckBox*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TCheckBox*)FindComponent(Components[i]->Name))->Caption = sCaption       ;

        }
        else if (Components[i]->ClassNameIs("TButton")) {
            sComName = Components[i]->ClassName();
            sName    = ((TButton*)FindComponent(Components[i]->Name))->Name         ;
            sCaption = ((TButton*)FindComponent(Components[i]->Name))->Caption      ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TButton*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TButton*)FindComponent(Components[i]->Name))->Caption = sCaption       ;

        }
        else if (Components[i]->ClassNameIs("TBitBtn")) {
            sComName = Components[i]->ClassName();
            sName    = ((TBitBtn*)FindComponent(Components[i]->Name))->Name         ;
            sCaption = ((TBitBtn*)FindComponent(Components[i]->Name))->Caption      ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TBitBtn*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TBitBtn*)FindComponent(Components[i]->Name))->Caption = sCaption       ;

        }
        else if (Components[i]->ClassNameIs("TSpeedButton")) {
            sComName = Components[i]->ClassName();
            sName    = ((TSpeedButton*)FindComponent(Components[i]->Name))->Name    ;
            sCaption = ((TSpeedButton*)FindComponent(Components[i]->Name))->Caption ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TSpeedButton*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TSpeedButton*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
        else if (Components[i]->ClassNameIs("TTabSheet")) {
            sComName = Components[i]->ClassName();
            sName    = ((TTabSheet*)FindComponent(Components[i]->Name))->Name       ;
            sCaption = ((TTabSheet*)FindComponent(Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TTabSheet*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TTabSheet*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
        else if (Components[i]->ClassNameIs("TGroupBox")) {
            sComName = Components[i]->ClassName();
            sName    = ((TGroupBox*)FindComponent(Components[i]->Name))->Name       ;
            sCaption = ((TGroupBox*)FindComponent(Components[i]->Name))->Caption    ;
            if(sComName == "" || sName == "") continue ;
            UserINI.Load(sPath.c_str()  , sComName  , sName , &sCaption);
            ((TGroupBox*)FindComponent(Components[i]->Name))->Name    = sName          ;
            ((TGroupBox*)FindComponent(Components[i]->Name))->Caption = sCaption       ;
        }
    }

    return true ;
}
//---------------------------------------------------------------------------







