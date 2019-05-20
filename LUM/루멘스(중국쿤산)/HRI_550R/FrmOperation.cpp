//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmOperation.h"
#include "FrmLimitCnt.h"
#include "FrmVisionRlstSet.h"
#include "FrmReviewSelect.h"
#include "FrmMain.h"
//---------------------------------------------------------------------------
#include "DataMan.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "Loader.h"
#include "PstnMan.h"
#include "Head.h"
#include "Rail.h"
#include "PstBuff.h"
#include "SLogUnit.h"
#include "VisnComUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOperation *FormOperation;
//---------------------------------------------------------------------------
__fastcall TFormOperation::TFormOperation(TComponent* Owner)
    : TForm(Owner)
{

    DM.ARAY[riLDR].SetParent(pnLoader   ); DM.ARAY[riLDR].SetConfig(false , "pnLoader  "); DM.ARAY[riLDR].ClearDispItem();
    DM.ARAY[riPRB].SetParent(pnPrb      ); DM.ARAY[riPRB].SetConfig(false , "pnPreBuff "); DM.ARAY[riLDR].ClearDispItem();
                                           DM.ARAY[riPRB].SetConfig(false , "pnPreBuff "); DM.ARAY[riPRB].ClearDispItem();
    DM.ARAY[riWR1].SetParent(pnInsp1    ); DM.ARAY[riWR1].SetConfig(false , "pnInsp1   "); DM.ARAY[riWR1].ClearDispItem();
    DM.ARAY[riVS1].SetParent(pnVisn1    ); DM.ARAY[riVS1].SetConfig(false , "pnVisn1   "); DM.ARAY[riVS1].ClearDispItem();
    DM.ARAY[riWR2].SetParent(pnInsp2    ); DM.ARAY[riWR2].SetConfig(false , "pnInsp2   "); DM.ARAY[riWR2].ClearDispItem();
    DM.ARAY[riVS2].SetParent(pnVisn2    ); DM.ARAY[riVS2].SetConfig(false , "pnVisn2   "); DM.ARAY[riVS2].ClearDispItem();
    DM.ARAY[riWR3].SetParent(pnInsp3    ); DM.ARAY[riWR3].SetConfig(false , "pnInsp3   "); DM.ARAY[riWR3].ClearDispItem();
    DM.ARAY[riVS3].SetParent(pnVisn3    ); DM.ARAY[riVS3].SetConfig(false , "pnVisn3   "); DM.ARAY[riVS3].ClearDispItem();
    DM.ARAY[riVS4].SetParent(pnVisn4    ); DM.ARAY[riVS4].SetConfig(false , "pnVisn4   "); DM.ARAY[riVS4].ClearDispItem();
    DM.ARAY[riPSB].SetParent(pnPsb      ); DM.ARAY[riPSB].SetConfig(false , "pnPsb     "); DM.ARAY[riPSB].ClearDispItem();
    DM.ARAY[riULD].SetParent(pnUnLoader ); DM.ARAY[riULD].SetConfig(false , "pnUnLoader"); DM.ARAY[riULD].ClearDispItem();

    //보여주기용 버퍼.
    HED.DpAray1.SetParent(pn1B1      ); HED.DpAray1.SetConfig(false , "pn1B1     "); HED.DpAray1.ClearDispItem();
    HED.DpAray2.SetParent(pn2B2      ); HED.DpAray2.SetConfig(false , "pn2B2     "); HED.DpAray2.ClearDispItem();
    HED.DpAray3.SetParent(pn3B3      ); HED.DpAray3.SetConfig(false , "pn3B3     "); HED.DpAray3.ClearDispItem();



    DM.ARAY[riLDR ].SetDispItem(csEmpty  ,"Empty "     ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite      );
    DM.ARAY[riLDR ].SetDispItem(csUnkwn  ,"Unkwn "     ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen );
    DM.ARAY[riLDR ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riLDR ].SetDispColor(csNone  ,clWhite      );

    DM.ARAY[riPRB ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riPRB ].SetDispColor(csNone  ,clWhite      );
    DM.ARAY[riPRB ].SetDispItem(csUnkwn  ,"Unkwn "     ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen );
    DM.ARAY[riPRB ].SetDispItem(csToBuf  ,"ToBuf "     ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );
    DM.ARAY[riPRB ].SetDispItem(csFromBuf,"FromBuf "   ) ; //DM.ARAY[riPRB ].SetDispColor(csUnkwn ,clMoneyGreen      );

    DM.ARAY[riWR1 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riWR1 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riWR1 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riWR1 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riWR1 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riWR1 ].SetDispItem(csWork    ,"Work "         ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt1   ,"V "+OM.CmnOptn.sCheckR1FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt1   ,"V Empty "      ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt2   ,"V "+OM.CmnOptn.sCheckR2FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt2   ,"V Chip "       ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt3   ,"V "+OM.CmnOptn.sCheckR3FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt3   ,"V Dust "       ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt4   ,"V "+OM.CmnOptn.sCheckR4FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt4   ,"V LFlow "      ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt5   ,"V "+OM.CmnOptn.sCheckR5FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt5   ,"V Broken "     ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt6   ,"V "+OM.CmnOptn.sCheckR6FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt6   ,"V Bubble "     ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt7   ,"V "+OM.CmnOptn.sCheckR7FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt7   ,"V Burr "       ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt8   ,"V "+OM.CmnOptn.sCheckR8FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt8   ,"V Zener "      ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt9   ,"V "+OM.CmnOptn.sCheckR9FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt9   ,"V Wire "       ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt10  ,"V "+OM.CmnOptn.sCheckR10FailName+" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt10  ,"V Orient "     ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt11  ,"V "+OM.CmnOptn.sCheckR11FailName+" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt11  ,"V Pmeasure "   ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt12  ,"V "+OM.CmnOptn.sCheckR12FailName+" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt12  ,"V Pdist "      ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt13  ,"V "+OM.CmnOptn.sCheckR13FailName+" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt13  ,"V Match "      ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt14  ,"V "+OM.CmnOptn.sCheckR14FailName+" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt14  ,"V Flow "       ) ;
    DM.ARAY[riWR1 ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riWR1 ].SetDispItem(csToBuf   ,"ToBuf "        ) ;
    DM.ARAY[riWR1 ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riWR1 ].SetDispItem(csFromBuf ,"FromBuf "      ) ;

    DM.ARAY[riVS1 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS1 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riVS1 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS1 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riVS1 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riVS1 ].SetDispItem(csWork    ,"Work "         ) ;

    DM.ARAY[riWR2 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riWR2 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riWR2 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riWR2 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riWR2 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riWR2 ].SetDispItem(csWork    ,"Work "         ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt1   ,"V "+OM.CmnOptn.sCheckR1FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt1   ,"V Empty "      ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt2   ,"V "+OM.CmnOptn.sCheckR2FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt2   ,"V Chip "       ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt3   ,"V "+OM.CmnOptn.sCheckR3FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt3   ,"V Dust "       ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt4   ,"V "+OM.CmnOptn.sCheckR4FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt4   ,"V LFlow "      ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt5   ,"V "+OM.CmnOptn.sCheckR5FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt5   ,"V Broken "     ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt6   ,"V "+OM.CmnOptn.sCheckR6FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt6   ,"V Bubble "     ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt7   ,"V "+OM.CmnOptn.sCheckR7FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt7   ,"V Burr "       ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt8   ,"V "+OM.CmnOptn.sCheckR8FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt8   ,"V Zener "      ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt9   ,"V "+OM.CmnOptn.sCheckR9FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt9   ,"V Wire "       ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt10  ,"V "+OM.CmnOptn.sCheckR10FailName+" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt10  ,"V Orient "     ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt11  ,"V "+OM.CmnOptn.sCheckR11FailName+" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt11  ,"V Pmeasure "   ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt12  ,"V "+OM.CmnOptn.sCheckR12FailName+" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt12  ,"V Pdist "      ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt13  ,"V "+OM.CmnOptn.sCheckR13FailName+" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt13  ,"V Match "      ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt14  ,"V "+OM.CmnOptn.sCheckR14FailName+" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt14  ,"V Flow "       ) ;
    DM.ARAY[riWR2 ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riWR2 ].SetDispItem(csToBuf   ,"ToBuf "        ) ;
    DM.ARAY[riWR2 ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riWR2 ].SetDispItem(csFromBuf ,"FromBuf "      ) ;

    DM.ARAY[riVS2 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS2 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riVS2 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS2 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riVS2 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riVS2 ].SetDispItem(csWork    ,"Work "         ) ;

    DM.ARAY[riWR3 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riWR3 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riWR3 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riWR3 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riWR3 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riWR3 ].SetDispItem(csWork    ,"Work "         ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt1   ,"V "+OM.CmnOptn.sCheckR1FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt1   ,"V Empty "      ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt2   ,"V "+OM.CmnOptn.sCheckR2FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt2   ,"V Chip "       ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt3   ,"V "+OM.CmnOptn.sCheckR3FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt3   ,"V Dust "       ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt4   ,"V "+OM.CmnOptn.sCheckR4FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt4   ,"V LFlow "      ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt5   ,"V "+OM.CmnOptn.sCheckR5FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt5   ,"V Broken "     ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt6   ,"V "+OM.CmnOptn.sCheckR6FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt6   ,"V Bubble "     ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt7   ,"V "+OM.CmnOptn.sCheckR7FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt7   ,"V Burr "       ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt8   ,"V "+OM.CmnOptn.sCheckR8FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt8   ,"V Zener "      ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt9   ,"V "+OM.CmnOptn.sCheckR9FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt9   ,"V Wire "       ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt10  ,"V "+OM.CmnOptn.sCheckR10FailName+" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt10  ,"V Orient "     ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt11  ,"V "+OM.CmnOptn.sCheckR11FailName+" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt11  ,"V Pmeasure "   ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt12  ,"V "+OM.CmnOptn.sCheckR12FailName+" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt12  ,"V Pdist "      ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt13  ,"V "+OM.CmnOptn.sCheckR13FailName+" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt13  ,"V Match "      ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt14  ,"V "+OM.CmnOptn.sCheckR14FailName+" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt14  ,"V Flow "       ) ;
    DM.ARAY[riWR3 ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riWR3 ].SetDispItem(csToBuf   ,"ToBuf "        ) ;
    DM.ARAY[riWR3 ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riWR3 ].SetDispItem(csFromBuf ,"FromBuf "      ) ;

    DM.ARAY[riVS3 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS3 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riVS3 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS3 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riVS3 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riVS3 ].SetDispItem(csWork    ,"Work "         ) ;

    DM.ARAY[riVS4 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS4 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riVS4 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS4 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riVS4 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riVS4 ].SetDispItem(csWork    ,"Work "         ) ;

    DM.ARAY[riPSB ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riPSB ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riPSB ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riPSB ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riPSB ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riPSB ].SetDispItem(csWork    ,"Work "         ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt1   ,"V "+OM.CmnOptn.sCheckR1FailName +" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt1   ,"V Empty "      ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt2   ,"V "+OM.CmnOptn.sCheckR2FailName +" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt2   ,"V Chip "       ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt3   ,"V "+OM.CmnOptn.sCheckR3FailName +" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt3   ,"V Dust "       ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt4   ,"V "+OM.CmnOptn.sCheckR4FailName +" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt4   ,"V LFlow "      ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt5   ,"V "+OM.CmnOptn.sCheckR5FailName +" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt5   ,"V Broken "     ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt6   ,"V "+OM.CmnOptn.sCheckR6FailName +" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt6   ,"V Bubble "     ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt7   ,"V "+OM.CmnOptn.sCheckR7FailName +" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt7   ,"V Burr "       ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt8   ,"V "+OM.CmnOptn.sCheckR8FailName +" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt8   ,"V Zener "      ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt9   ,"V "+OM.CmnOptn.sCheckR9FailName +" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt9   ,"V Wire "       ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt10  ,"V "+OM.CmnOptn.sCheckR10FailName+" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt10  ,"V Orient "     ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt11  ,"V "+OM.CmnOptn.sCheckR11FailName+" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt11  ,"V Pmeasure "   ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt12  ,"V "+OM.CmnOptn.sCheckR12FailName+" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt12  ,"V Pdist "      ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt13  ,"V "+OM.CmnOptn.sCheckR13FailName+" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt13  ,"V Match "      ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt14  ,"V "+OM.CmnOptn.sCheckR14FailName+" ") ; //DM.ARAY[riPSB ].SetDispItem(csRslt14  ,"V Flow "       ) ;
    DM.ARAY[riPSB ].SetDispItem(csFail    ,"Fail "         ) ; //DM.ARAY[riPSB ].SetDispItem(csFail    ,"V Review "     ) ;
    DM.ARAY[riPSB ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riPSB ].SetDispItem(csToBuf   ,"ToBuf "        ) ;
    DM.ARAY[riPSB ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riPSB ].SetDispItem(csFromBuf ,"FromBuf "      ) ;

    DM.ARAY[riULD ].SetDispItem(csEmpty  ,"Empty "     ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite      );
    DM.ARAY[riULD ].SetDispItem(csWork   ,"Work "      ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen );
    DM.ARAY[riULD ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riLDR ].SetDispColor(csNone  ,clWhite      );


    HED.DpAray1.SetDispItem(csNone    ,"None "         ) ;
    HED.DpAray1.SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    HED.DpAray1.SetDispItem(csWork    ,"Work "         ) ;
    HED.DpAray1.SetDispItem(csRslt1   ,"V "+OM.CmnOptn.sCheckR1FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt2   ,"V "+OM.CmnOptn.sCheckR2FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt3   ,"V "+OM.CmnOptn.sCheckR3FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt4   ,"V "+OM.CmnOptn.sCheckR4FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt5   ,"V "+OM.CmnOptn.sCheckR5FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt6   ,"V "+OM.CmnOptn.sCheckR6FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt7   ,"V "+OM.CmnOptn.sCheckR7FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt8   ,"V "+OM.CmnOptn.sCheckR8FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt9   ,"V "+OM.CmnOptn.sCheckR9FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt10  ,"V "+OM.CmnOptn.sCheckR10FailName+" ") ;
    HED.DpAray1.SetDispItem(csRslt11  ,"V "+OM.CmnOptn.sCheckR11FailName+" ") ;
    HED.DpAray1.SetDispItem(csRslt12  ,"V "+OM.CmnOptn.sCheckR12FailName+" ") ;
    HED.DpAray1.SetDispItem(csRslt13  ,"V "+OM.CmnOptn.sCheckR13FailName+" ") ;
    HED.DpAray1.SetDispItem(csRslt14  ,"V "+OM.CmnOptn.sCheckR14FailName+" ") ;
    HED.DpAray1.SetDispItem(csToBuf   ,"ToBuf "        ) ;
    HED.DpAray1.SetDispItem(csFromBuf ,"FromBuf "      ) ;

    HED.DpAray2.SetDispItem(csNone    ,"None "         ) ;
    HED.DpAray2.SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    HED.DpAray2.SetDispItem(csWork    ,"Work "         ) ;
    HED.DpAray2.SetDispItem(csRslt1   ,"V "+OM.CmnOptn.sCheckR1FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt2   ,"V "+OM.CmnOptn.sCheckR2FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt3   ,"V "+OM.CmnOptn.sCheckR3FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt4   ,"V "+OM.CmnOptn.sCheckR4FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt5   ,"V "+OM.CmnOptn.sCheckR5FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt6   ,"V "+OM.CmnOptn.sCheckR6FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt7   ,"V "+OM.CmnOptn.sCheckR7FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt8   ,"V "+OM.CmnOptn.sCheckR8FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt9   ,"V "+OM.CmnOptn.sCheckR9FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt10  ,"V "+OM.CmnOptn.sCheckR10FailName+" ") ;
    HED.DpAray2.SetDispItem(csRslt11  ,"V "+OM.CmnOptn.sCheckR11FailName+" ") ;
    HED.DpAray2.SetDispItem(csRslt12  ,"V "+OM.CmnOptn.sCheckR12FailName+" ") ;
    HED.DpAray2.SetDispItem(csRslt13  ,"V "+OM.CmnOptn.sCheckR13FailName+" ") ;
    HED.DpAray2.SetDispItem(csRslt14  ,"V "+OM.CmnOptn.sCheckR14FailName+" ") ;
    HED.DpAray2.SetDispItem(csToBuf   ,"ToBuf "        ) ;
    HED.DpAray2.SetDispItem(csFromBuf ,"FromBuf "      ) ;

    HED.DpAray3.SetDispItem(csNone    ,"None "         ) ;
    HED.DpAray3.SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    HED.DpAray3.SetDispItem(csWork    ,"Work "         ) ;
    HED.DpAray3.SetDispItem(csRslt1   ,"V "+OM.CmnOptn.sCheckR1FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt2   ,"V "+OM.CmnOptn.sCheckR2FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt3   ,"V "+OM.CmnOptn.sCheckR3FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt4   ,"V "+OM.CmnOptn.sCheckR4FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt5   ,"V "+OM.CmnOptn.sCheckR5FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt6   ,"V "+OM.CmnOptn.sCheckR6FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt7   ,"V "+OM.CmnOptn.sCheckR7FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt8   ,"V "+OM.CmnOptn.sCheckR8FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt9   ,"V "+OM.CmnOptn.sCheckR9FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt10  ,"V "+OM.CmnOptn.sCheckR10FailName+" ") ;
    HED.DpAray3.SetDispItem(csRslt11  ,"V "+OM.CmnOptn.sCheckR11FailName+" ") ;
    HED.DpAray3.SetDispItem(csRslt12  ,"V "+OM.CmnOptn.sCheckR12FailName+" ") ;
    HED.DpAray3.SetDispItem(csRslt13  ,"V "+OM.CmnOptn.sCheckR13FailName+" ") ;
    HED.DpAray3.SetDispItem(csRslt14  ,"V "+OM.CmnOptn.sCheckR14FailName+" ") ;
    HED.DpAray3.SetDispItem(csToBuf   ,"ToBuf "        ) ;
    HED.DpAray3.SetDispItem(csFromBuf ,"FromBuf "      ) ;

    HED.DpAray1.PopupMenuEnable(false);
    HED.DpAray2.PopupMenuEnable(false);
    HED.DpAray3.PopupMenuEnable(false);

    //메인폼 타이머에서 레벨 바꿀시에 자꾸 뻑이나서 아예 시작 할때 픽스 .
    for(int i =0 ; i < MAX_ARAY ; i++ ) {
        if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(false);
    }

    for(int i =0 ; i < MAX_ARAY ; i++ ) DM.ARAY[i].PopupMenuEnable(true);


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

    clStat[csNone  ] = clInactiveBorder  ;//csWhite
    clStat[csEmpty ] = clSilver          ;//csSilver
    clStat[csUnkwn ] = clMoneyGreen      ;//csAqua
    clStat[csWork  ] = (TColor)0x00FF9595;//clHighlight       ;//csBlue
    clStat[csFail  ] = clPurple          ;//csFuchsia
    clStat[csEtc   ] = clNone            ;//etc*/

    SEQ.Reset();

}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;


    LT.DispLotInfo(
        pnLotNo        ,
        pnLotJobFile   ,
        NULL           ,
//        pnLotStartTime ,
        NULL           ,
        NULL           ,
//        pnLotRunTime   ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
//        pnLotChipUpeh  ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        pnLotWorkChip  ,
        pnLotFailChip  );

    LT.DispDayInfo(
//        pnDayRunTime    ,
//        pnDayIdleTime   ,
//        pnDayJamTime    ,
//        pnDayTotalTime  ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL            ,
//        pnDayChipUpeh   ,
        NULL           ,
        NULL            ,
        NULL            ,
        NULL            ,
        NULL            ,
        NULL            ,
//        pnDayWorkChip   ,
//        pnDayFailChip   ,
        NULL            );
//        pnDayLotCount   );

    //EM_
    EM_DisplayList(lbErr);

    tsVs1 -> TabVisible = FM_GetLevel() >= lvMaster ;
    tsVs2 -> TabVisible = FM_GetLevel() >= lvMaster ;
    tsVs3 -> TabVisible = FM_GetLevel() >= lvMaster ;
    tsVs4 -> TabVisible = FM_GetLevel() >= lvMaster ;

    tsV1Result -> TabVisible = FM_GetLevel() >= lvMaster ;
    tsV2Result -> TabVisible = FM_GetLevel() >= lvMaster ;
    tsV3Result -> TabVisible = FM_GetLevel() >= lvMaster ;

//TODO: PRB 판넬보여주는데가 없어서 여기서 해야 할듯
    if(SEQ._bInspDispr) { CheckStripDisappear(); SEQ._bInspDispr = false; } //함수 끝나고 자동 스타트 됍.
    if(SEQ._bInspUnkwn) { CheckStripUnknwon  (); SEQ._bInspUnkwn = false; } //함수 끝나고 자동 스타트 됍.

//Lot 엔드시 화면에 보여주기 용.
//TODO:    if(WRK.m_bDispLotEnded) {
//        WRK.m_bDispLotEnded = false ;
//        edTime       -> Text = Now().FormatString("hh:nn:ss")     ;
//        edLotNo      -> Text = WRK.EndedLot.sLotNo       ;//DM.ARAY[riWK2].GetLotNo  (        );
//        edFailTotal  -> Text = WRK.EndedLot.iFailChip    ;
//        edWork       -> Text = WRK.EndedLot.iWorkChip    ;//DM.ARAY[riWK2].GetCntStat(csWork  );
//    }
//
    if(/*DM.ARAY[riWR1].GetCntExist() &&*/ !edWk1LotNo -> Focused() && !edWk1Id->Focused() && !btWk1Input->Focused() ) {
        edWk1LotNo  -> Text = DM.ARAY[riWR1].GetLotNo() ;
        edWk1Id     -> Text = DM.ARAY[riWR1].GetID()    ;
    }
    if(/*DM.ARAY[riWR2].GetCntExist() &&*/ !edWk2LotNo -> Focused() && !edWk2Id->Focused() && !btWk2Input->Focused() ) {
        edWk2LotNo  -> Text = DM.ARAY[riWR2].GetLotNo() ;
        edWk2Id     -> Text = DM.ARAY[riWR2].GetID()    ;
    }
    if(/*DM.ARAY[riWR3].GetCntExist() &&*/ !edWk3LotNo -> Focused() && !edWk3Id->Focused() && !btWk3Input->Focused() ) {
        edWk3LotNo  -> Text = DM.ARAY[riWR3].GetLotNo() ;
        edWk3Id     -> Text = DM.ARAY[riWR3].GetID()    ;
    }
    if(/*DM.ARAY[riPSB].GetCntExist() &&*/ !edWk4LotNo -> Focused() && !edWk4Id->Focused() && !btWk4Input->Focused() ) {
        edWk4LotNo  -> Text = DM.ARAY[riPSB].GetLotNo() ;
        edWk4Id     -> Text = DM.ARAY[riPSB].GetID()    ;
    }
    if(/*DM.ARAY[riPRB].GetCntExist() &&*/ !edPrbLotNo -> Focused() && !edPrbId->Focused() ) {
        edPrbLotNo  -> Text = DM.ARAY[riPRB].GetLotNo() ;
        edPrbId     -> Text = DM.ARAY[riPRB].GetID()    ;
    }

    static bool bPreRun = SEQ._bRun ;
    if(SEQ._bRun && !bPreRun) {
        pcVsR1 -> TabIndex = 0 ;
        pcVsR2 -> TabIndex = 0 ;
        pcVsR3 -> TabIndex = 0 ;
        pcVsR4 -> TabIndex = 0 ;
    }
    bPreRun = SEQ._bRun ;

    btWk1Input  -> Visible  = !SEQ._bRun ;
    btWk2Input  -> Visible  = !SEQ._bRun ;
    btWk3Input  -> Visible  = !SEQ._bRun ;
    btWk4Input  -> Visible  = !SEQ._bRun ;

    btVS1Reset  -> Enabled  = !SEQ._bRun ;
    btVS2Reset  -> Enabled  = !SEQ._bRun ;
    btVS3Reset  -> Enabled  = !SEQ._bRun ;
    btPSBReset  -> Enabled  = !SEQ._bRun ;

    btWK1Clear  -> Enabled  = !SEQ._bRun ;
    btWK2Clear  -> Enabled  = !SEQ._bRun ;
    btWK3Clear  -> Enabled  = !SEQ._bRun ;
    btPSBClear  -> Enabled  = !SEQ._bRun ;
    
    //디스플레이
    pnLotMGZCnt1  -> Caption = AnsiString(LDR._iMgzCnt) + AnsiString("/") + AnsiString(OM.DevOptn.iLotEndMgzCnt) ;
    pnBadLimCnt   -> Caption = OM.CmnOptn.iCheckFailCnt ;

    if(RAL.bMrkOneSkip) {
        RAL.bMrkOneSkip = false ;
        UpdateCmmOptn(toTabl);
    }

    lbVs1FailCnt  -> Caption = HED.DpAray1   .GetCntExist() - HED.DpAray1   .GetCntStat(csWork) ;
    lbVs2FailCnt  -> Caption = HED.DpAray2   .GetCntExist() - HED.DpAray2   .GetCntStat(csWork) ;
    lbVs3FailCnt  -> Caption = HED.DpAray3   .GetCntExist() - HED.DpAray3   .GetCntStat(csWork) ;
    lbMrkFailCnt  -> Caption = DM.ARAY[riLOT].GetStep(); //DM.ARAY[riPSB].GetCntExist() - DM.ARAY[riPSB].GetCntStat(csWork) ;

    int iLotWrkChip  = pnLotWorkChip -> Caption.ToIntDef(0) ;
    int iLotFailChip = pnLotFailChip -> Caption.ToIntDef(1) ;
    double dTemp = 0.0 ;
    String sTemp ;

    dTemp = iLotWrkChip ? (iLotWrkChip - iLotFailChip ) / (float)iLotWrkChip : 0.0 ;
    dTemp = dTemp * 100 ;
    sTemp.sprintf("%0.2f",dTemp);
    pnLotYeild     -> Caption = sTemp ;

    dTemp = OM.iTotalChip ? (OM.iTotalChip - OM.iTotalFailChip) / (float)OM.iTotalChip : 0.0  ;
    dTemp = dTemp * 100;
    sTemp.sprintf("%0.2f",dTemp);
    pnTotalYield   -> Caption = sTemp ;

    pnDayWorkChip -> Caption = OM.iTotalChip ;
    pnDayFailChip -> Caption = OM.iTotalFailChip ;

    pnLotGoodChip -> Caption = iLotWrkChip   - iLotFailChip      ;
    pnTotalGoodCnt-> Caption = OM.iTotalChip - OM.iTotalFailChip ;
    //Map Update
    HED.DpAray1.UpdateAray();
    HED.DpAray2.UpdateAray();
    HED.DpAray3.UpdateAray();
    DM.UpdateAray();


    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::UpdateCmmOptn(bool bTable)
{
    if(bTable) {
        TNotifyEvent ChkBoxOnClick=cbMrk1Skip->OnClick;
        cbMrk1Skip    -> OnClick=NULL;
        cbMrkSam1Skip -> OnClick=NULL;
        cbMrkAllSkip  -> OnClick=NULL;
        cbIgnrVsErr   -> OnClick=NULL;
        cbVsAllSkip   -> OnClick=NULL;
        cbLoadingStop -> OnClick=NULL;
        cbVs1Skip     -> OnClick=NULL;
        cbVs2Skip     -> OnClick=NULL;
        cbVs3Skip     -> OnClick=NULL;
        cbVsMrkSkip   -> OnClick=NULL;
        cbDoorSkip    -> OnClick=NULL;
        cbIdleRun     -> OnClick=NULL;

        cbMrk1Skip    -> Checked = OM.CmnOptn.bMrk1Skip     ;
        cbMrkSam1Skip -> Checked = OM.CmnOptn.bMrkSam1Skip  ;
        cbMrkAllSkip  -> Checked = OM.CmnOptn.bMrkAllSkip   ;
        cbIgnrVsErr   -> Checked = OM.CmnOptn.bIgnrVsErr    ;
        cbVsAllSkip   -> Checked = OM.CmnOptn.bVsAllSkip    ;
        cbLoadingStop -> Checked = OM.CmnOptn.bLoadingStop  ;
        cbVs1Skip     -> Checked = OM.CmnOptn.bVs1Skip      ;
        cbVs2Skip     -> Checked = OM.CmnOptn.bVs2Skip      ;
        cbVs3Skip     -> Checked = OM.CmnOptn.bVs3Skip      ;
        cbVsMrkSkip   -> Checked = OM.CmnOptn.bVsMrkSkip    ;
        cbDoorSkip    -> Checked = OM.CmnOptn.bDoorSkip     ;
        cbIdleRun     -> Checked = OM.CmnOptn.bIdleRun      ;

//        if(OM.CmnOptn.bIdleRun) {
            cbMrkAllSkip -> Enabled = !OM.CmnOptn.bIdleRun ; //가라 나중에 바꾸자 껏다 키면 안될듯
            cbVs1Skip    -> Enabled = !OM.CmnOptn.bIdleRun ;
            cbVs2Skip    -> Enabled = !OM.CmnOptn.bIdleRun ;
            cbVs3Skip    -> Enabled = !OM.CmnOptn.bIdleRun ;
            cbVsMrkSkip  -> Enabled = !OM.CmnOptn.bIdleRun ;
//        }

        cbMrk1Skip    -> OnClick=ChkBoxOnClick;
        cbMrkSam1Skip -> OnClick=ChkBoxOnClick;
        cbMrkAllSkip  -> OnClick=ChkBoxOnClick;
        cbIgnrVsErr   -> OnClick=ChkBoxOnClick;
        cbVsAllSkip   -> OnClick=ChkBoxOnClick;
        cbLoadingStop -> OnClick=ChkBoxOnClick;
        cbVs1Skip     -> OnClick=ChkBoxOnClick;
        cbVs2Skip     -> OnClick=ChkBoxOnClick;
        cbVs3Skip     -> OnClick=ChkBoxOnClick;
        cbVsMrkSkip   -> OnClick=ChkBoxOnClick;
        cbDoorSkip    -> OnClick=ChkBoxOnClick;
        cbIdleRun     -> OnClick=cbIdleRunClick;

    }
    else {
        OM.CmnOptn.bMrk1Skip      = cbMrk1Skip    -> Checked  ;
        OM.CmnOptn.bMrkSam1Skip   = cbMrkSam1Skip -> Checked  ;
        OM.CmnOptn.bMrkAllSkip    = cbMrkAllSkip  -> Checked  ;
        OM.CmnOptn.bIgnrVsErr     = cbIgnrVsErr   -> Checked  ;
        OM.CmnOptn.bVsAllSkip     = cbVsAllSkip   -> Checked  ;
        OM.CmnOptn.bLoadingStop   = cbLoadingStop -> Checked  ;
        OM.CmnOptn.bVs1Skip       = cbVs1Skip     -> Checked  ;
        OM.CmnOptn.bVs2Skip       = cbVs2Skip     -> Checked  ;
        OM.CmnOptn.bVs3Skip       = cbVs3Skip     -> Checked  ;
        OM.CmnOptn.bVsMrkSkip     = cbVsMrkSkip   -> Checked  ;
        OM.CmnOptn.bDoorSkip      = cbDoorSkip    -> Checked  ;
        OM.CmnOptn.bIdleRun       = cbIdleRun     -> Checked  ;

    }

}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::btnOKClick(TObject *Sender)
{
    if(OM.DevOptn.iLotEndMgzCnt != edMeasure -> Text.ToIntDef(0)){
       OM.DevOptn.iLotEndMgzCnt  = edMeasure -> Text.ToIntDef(0);
    }
    if(edMeasure -> Text.ToIntDef(1) <= 0 ) OM.DevOptn.iLotEndMgzCnt = 1 ;
    else                                    OM.DevOptn.iLotEndMgzCnt = edMeasure -> Text.ToIntDef(1);

    OM.SaveDevOptn(OM.m_sCrntDev);
    pnLotMGZCnt2->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::pnLotMGZCnt1Click(TObject *Sender)
{
    edMeasure -> Text = OM.DevOptn.iLotEndMgzCnt ;
    pnLotMGZCnt2 ->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::pnBadLimCntClick(TObject *Sender)
{
    FormLimitCnt->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::pnColor1Click(TObject *Sender)
{
    FormVisionRlstSet->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::btVb4SelClick(TObject *Sender)
{
    FormReviewSelect->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::LbResultChange()
{
    lbR1  -> Caption = OM.CmnOptn.sCheckR1FailName ;
    lbR2  -> Caption = OM.CmnOptn.sCheckR2FailName ;
    lbR3  -> Caption = OM.CmnOptn.sCheckR3FailName ;
    lbR4  -> Caption = OM.CmnOptn.sCheckR4FailName ;
    lbR5  -> Caption = OM.CmnOptn.sCheckR5FailName ;
    lbR6  -> Caption = OM.CmnOptn.sCheckR6FailName ;
    lbR7  -> Caption = OM.CmnOptn.sCheckR7FailName ;
    lbR8  -> Caption = OM.CmnOptn.sCheckR8FailName ;
    lbR9  -> Caption = OM.CmnOptn.sCheckR9FailName ;
    lbR10 -> Caption = OM.CmnOptn.sCheckR10FailName ;
    lbR11 -> Caption = OM.CmnOptn.sCheckR11FailName ;
    lbR12 -> Caption = OM.CmnOptn.sCheckR12FailName ;
    lbR13 -> Caption = OM.CmnOptn.sCheckR13FailName ;
    lbR14 -> Caption = OM.CmnOptn.sCheckR14FailName ;
}

void __fastcall TFormOperation::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true ;

//    FormMain -> btnRun -> Enabled = true ;
//    FormMain -> pnRun  -> Enabled = true ;

    OM.LoadCmnOptn();
    UpdateCmmOptn(toTabl);
    LbResultChange();
}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::cbMrk1SkipClick(TObject *Sender)
{
    String sTemp;

    TCheckBox *pCbox = dynamic_cast <TCheckBox *> (Sender);
    sTemp = pCbox->Name;
    if(sTemp == "cbMrk1Skip") {
        if( DM.ARAY[riPSB].GetCntExist() ) {
            DM.ARAY[riPSB].SetStat(csWork) ;
            OM.CmnOptn.bMrk1Skip = false ;
            UpdateCmmOptn(toTabl);
            return ;
        }
        else {
            UpdateCmmOptn(toBuff);
            OM.SaveCmnOptn();
            return ;
        }
    }
    UpdateCmmOptn(toBuff);
    OM.SaveCmnOptn();

}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::btWK1ClearClick(TObject *Sender)
{
//    if(SEQ._bRun) return ;
    String sTemp;

    TButton *pBtn = dynamic_cast <TButton *> (Sender);
    sTemp = pBtn->Name;

    Trace("btWK1ClearClick",sTemp.c_str());
         if(sTemp == "btWK1Clear") { DM.ARAY[riWR1].SetStat(csNone) ; DM.ARAY[riVS1].SetStat(csNone) ;}
    else if(sTemp == "btWK2Clear") { DM.ARAY[riWR2].SetStat(csNone) ; DM.ARAY[riVS2].SetStat(csNone) ;}
    else if(sTemp == "btWK3Clear") { DM.ARAY[riWR3].SetStat(csNone) ; DM.ARAY[riVS3].SetStat(csNone) ;}
    else if(sTemp == "btPRBClear") { DM.ARAY[riPRB].SetStat(csNone) ; }
    else if(sTemp == "btPSBClear") {
        if(GetLastStrip() && DM.ARAY[riPSB].GetCntExist() && LT.m_bLotOpen) {
            PSB.m_bLotEnded = true ;
            memcpy(&PSB.EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

            Trace("LotEnd",DM.ARAY[riLOT].GetLotNo().c_str());
            LT.LotInfo.dEndTime = Now();
            LT.WriteLotLog   ();
            LT.WriteLotDayLog();
            Trace("WriteLotLog",DM.ARAY[riLOT].GetLotNo().c_str());
            //LT.LotEnd();
        }
        DM.ARAY[riLOT].SetStat(csNone) ;
        DM.ARAY[riPSB].SetStat(csNone) ;
        DM.ARAY[riVS4].SetStat(csNone) ;        
    }

}
bool TFormOperation::GetLastStrip()
{
    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWR1Exist = !DM.ARAY[riWR1].CheckAllStat(csNone) ;
    bool bWR2Exist = !DM.ARAY[riWR2].CheckAllStat(csNone) ;
    bool bWR3Exist = !DM.ARAY[riWR3].CheckAllStat(csNone) ;

    bool bLDRDiff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riLDR].GetLotNo() ;
    bool bPRBDiff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riPRB].GetLotNo() ;
    bool bWR1Diff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riWR1].GetLotNo() ;
    bool bWR2Diff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riWR2].GetLotNo() ;
    bool bWR3Diff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riWR3].GetLotNo() ;

    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

         if(bWR3Exist) return bWR3Diff ;
    else if(bWR2Exist) return bWR2Diff ;
    else if(bWR1Exist) return bWR1Diff ;
    else if(bPRBExist) return bPRBDiff ;
    else if(bLDRExist) return bLDRDiff ;
    else if(!bLastMgz) return false    ;
    else               return true     ;
}


//---------------------------------------------------------------------------
void __fastcall TFormOperation::btVS1ResetClick(TObject *Sender)
{
    String sTemp;

    TButton *pBtn = dynamic_cast <TButton *> (Sender);
    sTemp = pBtn->Name;

    if(sTemp == "btVS1Reset") {
        if(!OM.CmnOptn.bVs1Skip)VC.SendReset(vrVisn1);
        if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
        if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].ChangeStat(csWork,csUnkwn) ;
    }
    else if(sTemp == "btVS2Reset") {
        if(!OM.CmnOptn.bVs2Skip)VC.SendReset(vrVisn2);
        if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
        if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].ChangeStat(csWork,csUnkwn) ;
    }
    else if(sTemp == "btVS3Reset") {
        if(!OM.CmnOptn.bVs3Skip)VC.SendReset(vrVisn3);
        if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat(csUnkwn) ;
        if(DM.ARAY[riWR3].GetCntExist())DM.ARAY[riWR3].ChangeStat(csWork,csUnkwn) ;
    }
    else if(sTemp == "btPSBReset") {
        if(!OM.CmnOptn.bVsMrkSkip)VC.SendReset(vrVisn4);
        if(DM.ARAY[riVS4].GetCntExist())DM.ARAY[riVS4].SetStat(csUnkwn) ;
        if(DM.ARAY[riPSB].GetCntExist())DM.ARAY[riPSB].ChangeStat(csWork,csUnkwn) ;
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::btWk1InputClick(TObject *Sender)
{
    if(SEQ._bRun) return ;

//    if(FM_MsgYesNo("Confirm","Did you want to enter Lot information ") != mrYes) return ;

    int iTag = ((TButton*)Sender) -> Tag ;
    AnsiString sLotNo ;
    String sId = "";

    switch (iTag)
    {
        default :
            break ;

        case 1 :
            sLotNo  = edWk1LotNo ->Text ;
            sId     = edWk1Id    ->Text ;
            DM.ARAY[riWR1].SetLotNo(sLotNo) ;
            DM.ARAY[riWR1].SetID(sId.ToIntDef(9999));
            break ;

        case 2 :
            sLotNo  = edWk2LotNo ->Text ;
            sId     = edWk2Id    ->Text ;
            DM.ARAY[riWR2].SetLotNo(sLotNo) ;
            DM.ARAY[riWR2].SetID(sId.ToIntDef(9999));
            break ;

        case 3 :
            sLotNo  = edWk3LotNo ->Text ;
            sId     = edWk3Id    ->Text ;
            DM.ARAY[riWR3].SetLotNo(sLotNo) ;
            DM.ARAY[riWR3].SetID(sId.ToIntDef(9999));
            break ;

        case 4 :
            sLotNo  = edWk4LotNo ->Text ;
            sId     = edWk4Id    ->Text ;
            DM.ARAY[riPSB].SetLotNo(sLotNo) ;
            DM.ARAY[riPSB].SetID(sId.ToIntDef(9999));
            break ;
     }

}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::lbErrClick(TObject *Sender)
{
    String Temp ;
    int iTemp ;
    int Cnt,iCount ;
    char CChar ;
    Cnt   = 0  ;
    CChar = '*';

    iTemp = lbErr->ItemIndex ;
    Temp  = lbErr->Items->Strings[iTemp];
    if(Temp.Pos("[ERR"))
    {
        iTemp = Temp.SubString(Temp.Pos("[ERR")+4,3).ToIntDef(0);
        Temp  = EM_GetErrAction(iTemp);
        for(register int i = 1 ; i <= Temp.Length() ; i++) if (Temp[i] == CChar) iCount++;
        Cnt = iCount;
        mmAction->Clear();
        for (int i=0; i <= Cnt ; i++) {
            AnsiString Line  = "";
            int n = Temp.Pos("*");
            Line  = Temp.SubString(1, n ? n-1 : n);
            Temp  = Temp.Delete(1, n);
            if (!n) { mmAction -> Lines -> Add(Temp); break; }
            else    { mmAction -> Lines -> Add(Line);        }
        }
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false ;
}
//---------------------------------------------------------------------------
bool __fastcall TFormOperation::CheckStripDisappear()
{
    //스트립 사라짐 에러.
    if( !DM.ARAY[riLDR ].CheckAllStat(csNone) && (!IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2)) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Loader MGZ Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riLDR ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[PreBuffer Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riPRB ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if( DM.ARAY[riWR1 ].CheckExist(0,0) && !IO_GetX(xWR1_Pkg) &&
            (MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormalPs ))   ||
             MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpInversePs)) ) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision1 Buffer Strip Disappear] \nDo you want to clear Strip?") == mrYes ) { DM.ARAY[riWR1 ].SetStat(csNone); DM.ARAY[riVS1 ].SetStat(csNone);SEQ._bBtnStart = true ;}
    }
    else if( DM.ARAY[riWR2 ].CheckExist(0,0) && !IO_GetX(xWR2_Pkg) && AT_Complete(aiWK2_Lift , ccBwd) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision2 Buffer Strip Disappear] \nDo you want to clear Strip?") == mrYes ) { DM.ARAY[riWR2 ].SetStat(csNone); DM.ARAY[riVS2 ].SetStat(csNone);SEQ._bBtnStart = true ;}
    }
    else if( DM.ARAY[riWR3 ].CheckExist(0,0) && !IO_GetX(xWR3_Pkg) && AT_Complete(aiWK3_Lift , ccBwd) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision3 Buffer Strip Disappear] \nDo you want to clear Strip?") == mrYes ) { DM.ARAY[riWR3 ].SetStat(csNone); DM.ARAY[riVS3 ].SetStat(csNone);SEQ._bBtnStart = true ;}
    }
    else if(!DM.ARAY[riULD ].CheckAllStat(csNone) && (!IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2)) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Unloader Mgz Disappear] \nDo you want to clear Mgz?") == mrYes ) { DM.ARAY[riULD ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if(!DM.ARAY[riPSB ].CheckAllStat(csNone) && (!IO_GetX(xPSB_Pkg) && !IO_GetX(xPSB_PkgOut3) && !IO_GetX(xPSB_PkgOut4))&& AT_Complete(aiPSB_Lift , ccBwd) &&!OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Marking Buffer Disappear] \nDo you want to clear Mgz?") == mrYes ) { DM.ARAY[riPSB ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }

    return true ;
}
//---------------------------------------------------------------------------
bool __fastcall TFormOperation::CheckStripUnknwon()
{
    return true ;
}





void __fastcall TFormOperation::pnDayWorkChipDblClick(TObject *Sender)
{
    OM.iTotalChip     = 0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::pnDayFailChipDblClick(TObject *Sender)
{
    OM.iTotalFailChip = 0 ;
}
//---------------------------------------------------------------------------


void __fastcall TFormOperation::cbIdleRunClick(TObject *Sender)
{
//    TNotifyEvent ChkBoxOnClick=cbMrk1Skip->OnClick;

    cbMrkAllSkip  -> OnClick= NULL ;
    cbVs1Skip     -> OnClick= NULL ;
    cbVs2Skip     -> OnClick= NULL ;
    cbVs3Skip     -> OnClick= NULL ;
    cbVsMrkSkip   -> OnClick= NULL ;
    cbIdleRun     -> OnClick= NULL ;
    bool iRet = cbIdleRun -> Checked ;
    if(iRet) {
        OM.CmnOptn.bMrkAllSkip    = iRet  ;
        OM.CmnOptn.bVs1Skip       = iRet  ;
        OM.CmnOptn.bVs2Skip       = iRet  ;
        OM.CmnOptn.bVs3Skip       = iRet  ;
        OM.CmnOptn.bVsMrkSkip     = iRet  ;
        OM.CmnOptn.bIdleRun       = iRet  ;

        cbMrkAllSkip ->Checked = iRet ; cbMrkAllSkip -> Enabled = !iRet ; //가라 나중에 바꾸자 껏다 키면 안될듯
        cbVs1Skip    ->Checked = iRet ; cbVs1Skip    -> Enabled = !iRet ;
        cbVs2Skip    ->Checked = iRet ; cbVs2Skip    -> Enabled = !iRet ;
        cbVs3Skip    ->Checked = iRet ; cbVs3Skip    -> Enabled = !iRet ;
        cbVsMrkSkip  ->Checked = iRet ; cbVsMrkSkip  -> Enabled = !iRet ;
//        cbIdleRun    ->Checked = iRet ; cbIdleRun    -> Enabled = iRet ;
    }
    else {
        OM.CmnOptn.bMrkAllSkip    = iRet ;
        OM.CmnOptn.bVs1Skip       = iRet ;
        OM.CmnOptn.bVs2Skip       = iRet ;
        OM.CmnOptn.bVs3Skip       = iRet ;
        OM.CmnOptn.bVsMrkSkip     = iRet ;
        OM.CmnOptn.bIdleRun       = iRet ;

        cbMrkAllSkip ->Checked = iRet ; cbMrkAllSkip -> Enabled = !iRet ;
        cbVs1Skip    ->Checked = iRet ; cbVs1Skip    -> Enabled = !iRet ;
        cbVs2Skip    ->Checked = iRet ; cbVs2Skip    -> Enabled = !iRet ;
        cbVs3Skip    ->Checked = iRet ; cbVs3Skip    -> Enabled = !iRet ;
        cbVsMrkSkip  ->Checked = iRet ; cbVsMrkSkip  -> Enabled = !iRet ;
//        cbIdleRun    ->Checked = iRet ; cbIdleRun    -> Enabled = iRet ;

    }

    cbMrkAllSkip  -> OnClick=cbMrk1SkipClick;
    cbVs1Skip     -> OnClick=cbMrk1SkipClick;
    cbVs2Skip     -> OnClick=cbMrk1SkipClick;
    cbVs3Skip     -> OnClick=cbMrk1SkipClick;
    cbVsMrkSkip   -> OnClick=cbMrk1SkipClick;
    cbIdleRun     -> OnClick=cbIdleRunClick ;

//    UpdateCmmOptn(toBuff);
    OM.SaveCmnOptn();

}
//---------------------------------------------------------------------------


void __fastcall TFormOperation::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;        
}
//---------------------------------------------------------------------------





void __fastcall TFormOperation::Button1Click(TObject *Sender)
{
    HED.SetTrgPos();        
}
//---------------------------------------------------------------------------

