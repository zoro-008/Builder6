//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"
#include "FormVisionRlstSet.h"
#include "FrmLimitCnt.h"
#include "SLogUnit.h"

#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "UtilDefine.h"
#include "VisnComUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "Rail.h"
#include "Head.h"
#include "ManualMan.h"
#include "SPCUnit.h"
#include "PstnMan.h"
#include "Loader.h"
#include "PstBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)
    : TForm(Owner)
{
    pcWorkZone1->DoubleBuffered = true ;
    pcWorkZone1->ActivePageIndex = 0 ;
    pn1B1->DoubleBuffered = true ;
    DM.ARAY[riLDR].SetParent(pnLoader   ); DM.ARAY[riLDR].SetConfig(false , "pnLoader   "); DM.ARAY[riLDR].ClearDispItem();
    DM.ARAY[riPRB].SetParent(pnPRB      ); DM.ARAY[riPRB].SetConfig(false , "pnPRB      "); DM.ARAY[riPRB].ClearDispItem();
    DM.ARAY[riWR1].SetParent(pnInsp1    ); DM.ARAY[riWR1].SetConfig(false , "pnInsp1    "); DM.ARAY[riWR1].ClearDispItem();
    DM.ARAY[riVS1].SetParent(pnVisn1    ); DM.ARAY[riVS1].SetConfig(false , "pnVisn1    "); DM.ARAY[riVS1].ClearDispItem();
    DM.ARAY[riWR2].SetParent(pnInsp2    ); DM.ARAY[riWR2].SetConfig(false , "pnInsp2    "); DM.ARAY[riWR2].ClearDispItem();
    DM.ARAY[riVS2].SetParent(pnVisn2    ); DM.ARAY[riVS2].SetConfig(false , "pnVisn2    "); DM.ARAY[riVS2].ClearDispItem();
    DM.ARAY[riWR3].SetParent(pnInsp3    ); DM.ARAY[riWR3].SetConfig(false , "pnInsp3    "); DM.ARAY[riWR3].ClearDispItem();
    DM.ARAY[riVS3].SetParent(pnVisn3    ); DM.ARAY[riVS3].SetConfig(false , "pnVisn3    "); DM.ARAY[riVS3].ClearDispItem();
    DM.ARAY[riVS4].SetParent(pnVisn4    ); DM.ARAY[riVS4].SetConfig(false , "pnVisn4    "); DM.ARAY[riVS4].ClearDispItem();
    DM.ARAY[riPSB].SetParent(pnPSB      ); DM.ARAY[riPSB].SetConfig(false , "pnPSB      "); DM.ARAY[riPSB].ClearDispItem();
    DM.ARAY[riULD].SetParent(pnULD      ); DM.ARAY[riULD].SetConfig(false , "pnULD      "); DM.ARAY[riULD].ClearDispItem();
    DM.ARAY[riTRY].SetParent(pnUnLoader ); DM.ARAY[riTRY].SetConfig(false , "pnUnLoader "); DM.ARAY[riTRY].ClearDispItem();
    DM.ARAY[riUPK].SetParent(pnWorkTray ); DM.ARAY[riUPK].SetConfig(false , "pnWorkTray "); DM.ARAY[riUPK].ClearDispItem();

    //보여주기용 버퍼.
    HED.DpAray1.SetParent(pn1B1      ); HED.DpAray1.SetConfig(false , "pn1B1     "); HED.DpAray1.ClearDispItem();
    HED.DpAray2.SetParent(pn2B2      ); HED.DpAray2.SetConfig(false , "pn2B2     "); HED.DpAray2.ClearDispItem();
    HED.DpAray3.SetParent(pn3B3      ); HED.DpAray3.SetConfig(false , "pn3B3     "); HED.DpAray3.ClearDispItem();



    DM.ARAY[riLDR ].SetDispItem(csEmpty  ,"Empty "     ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite      );
    DM.ARAY[riLDR ].SetDispItem(csUnkwn  ,"Unkwn "     ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen );
    DM.ARAY[riLDR ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riLDR ].SetDispColor(csNone  ,clWhite      );

    DM.ARAY[riPRB ].SetDispItem(csNone   ,"None  "     ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite      );
    DM.ARAY[riPRB ].SetDispItem(csUnkwn  ,"Unkwn "     ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen );

    DM.ARAY[riWR1 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riWR1 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riWR1 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riWR1 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riWR1 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riWR1 ].SetDispItem(csWork    ,"Work "         ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt1   ,"V "+OM.DevOptn.sCheckR1FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt1   ,"V Empty "      ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt2   ,"V "+OM.DevOptn.sCheckR2FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt2   ,"V Chip "       ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt3   ,"V "+OM.DevOptn.sCheckR3FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt3   ,"V Dust "       ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt4   ,"V "+OM.DevOptn.sCheckR4FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt4   ,"V LFlow "      ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt5   ,"V "+OM.DevOptn.sCheckR5FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt5   ,"V Broken "     ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt6   ,"V "+OM.DevOptn.sCheckR6FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt6   ,"V Bubble "     ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt7   ,"V "+OM.DevOptn.sCheckR7FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt7   ,"V Burr "       ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt8   ,"V "+OM.DevOptn.sCheckR8FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt8   ,"V Zener "      ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt9   ,"V "+OM.DevOptn.sCheckR9FailName +" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt9   ,"V Wire "       ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt10  ,"V "+OM.DevOptn.sCheckR10FailName+" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt10  ,"V Orient "     ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt11  ,"V "+OM.DevOptn.sCheckR11FailName+" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt11  ,"V Pmeasure "   ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt12  ,"V "+OM.DevOptn.sCheckR12FailName+" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt12  ,"V Pdist "      ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt13  ,"V "+OM.DevOptn.sCheckR13FailName+" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt13  ,"V Match "      ) ;
    DM.ARAY[riWR1 ].SetDispItem(csRslt14  ,"V "+OM.DevOptn.sCheckR14FailName+" ") ; //DM.ARAY[riWR1 ].SetDispItem(csRslt14  ,"V Flow "       ) ;
    DM.ARAY[riWR1 ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riWR1 ].SetDispItem(csToBuf   ,"ToBuf "        ) ;
    DM.ARAY[riWR1 ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riWR1 ].SetDispItem(csFromBuf ,"FromBuf "      ) ;

    DM.ARAY[riVS1 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS1 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riVS1 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS1 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riVS1 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riVS1 ].SetDispItem(csWork    ,"Work "         ) ;

    DM.ARAY[riWR2 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riWR2 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riWR2 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riWR2 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riWR2 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riWR2 ].SetDispItem(csWork    ,"Work "         ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt1   ,"V "+OM.DevOptn.sCheckR1FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt1   ,"V Empty "      ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt2   ,"V "+OM.DevOptn.sCheckR2FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt2   ,"V Chip "       ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt3   ,"V "+OM.DevOptn.sCheckR3FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt3   ,"V Dust "       ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt4   ,"V "+OM.DevOptn.sCheckR4FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt4   ,"V LFlow "      ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt5   ,"V "+OM.DevOptn.sCheckR5FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt5   ,"V Broken "     ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt6   ,"V "+OM.DevOptn.sCheckR6FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt6   ,"V Bubble "     ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt7   ,"V "+OM.DevOptn.sCheckR7FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt7   ,"V Burr "       ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt8   ,"V "+OM.DevOptn.sCheckR8FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt8   ,"V Zener "      ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt9   ,"V "+OM.DevOptn.sCheckR9FailName +" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt9   ,"V Wire "       ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt10  ,"V "+OM.DevOptn.sCheckR10FailName+" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt10  ,"V Orient "     ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt11  ,"V "+OM.DevOptn.sCheckR11FailName+" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt11  ,"V Pmeasure "   ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt12  ,"V "+OM.DevOptn.sCheckR12FailName+" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt12  ,"V Pdist "      ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt13  ,"V "+OM.DevOptn.sCheckR13FailName+" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt13  ,"V Match "      ) ;
    DM.ARAY[riWR2 ].SetDispItem(csRslt14  ,"V "+OM.DevOptn.sCheckR14FailName+" ") ; //DM.ARAY[riWR2 ].SetDispItem(csRslt14  ,"V Flow "       ) ;
    DM.ARAY[riWR2 ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riWR2 ].SetDispItem(csToBuf   ,"ToBuf "        ) ;
    DM.ARAY[riWR2 ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riWR2 ].SetDispItem(csFromBuf ,"FromBuf "      ) ;

    DM.ARAY[riVS2 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS2 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riVS2 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS2 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riVS2 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riVS2 ].SetDispItem(csWork    ,"Work "         ) ;

    DM.ARAY[riWR3 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riWR3 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riWR3 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riWR3 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riWR3 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riWR3 ].SetDispItem(csWork    ,"Work "         ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt1   ,"V "+OM.DevOptn.sCheckR1FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt1   ,"V Empty "      ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt2   ,"V "+OM.DevOptn.sCheckR2FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt2   ,"V Chip "       ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt3   ,"V "+OM.DevOptn.sCheckR3FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt3   ,"V Dust "       ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt4   ,"V "+OM.DevOptn.sCheckR4FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt4   ,"V LFlow "      ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt5   ,"V "+OM.DevOptn.sCheckR5FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt5   ,"V Broken "     ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt6   ,"V "+OM.DevOptn.sCheckR6FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt6   ,"V Bubble "     ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt7   ,"V "+OM.DevOptn.sCheckR7FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt7   ,"V Burr "       ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt8   ,"V "+OM.DevOptn.sCheckR8FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt8   ,"V Zener "      ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt9   ,"V "+OM.DevOptn.sCheckR9FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt9   ,"V Wire "       ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt10  ,"V "+OM.DevOptn.sCheckR10FailName+" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt10  ,"V Orient "     ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt11  ,"V "+OM.DevOptn.sCheckR11FailName+" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt11  ,"V Pmeasure "   ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt12  ,"V "+OM.DevOptn.sCheckR12FailName+" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt12  ,"V Pdist "      ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt13  ,"V "+OM.DevOptn.sCheckR13FailName+" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt13  ,"V Match "      ) ;
    DM.ARAY[riWR3 ].SetDispItem(csRslt14  ,"V "+OM.DevOptn.sCheckR14FailName+" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt14  ,"V Flow "       ) ;
    DM.ARAY[riWR3 ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riWR3 ].SetDispItem(csToBuf   ,"ToBuf "        ) ;
    DM.ARAY[riWR3 ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riWR3 ].SetDispItem(csFromBuf ,"FromBuf "      ) ;

    DM.ARAY[riVS3 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS3 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riVS3 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS3 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riVS3 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riVS3 ].SetDispItem(csWork    ,"Work "         ) ;

    DM.ARAY[riVS4 ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riVS4 ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riVS4 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riVS4 ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riVS4 ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riVS4 ].SetDispItem(csWork    ,"Work "         ) ;
    DM.ARAY[riVS4 ].SetDispItem(csMarking ,"Marking "      ) ; //DM.ARAY[riVS4 ].SetDispItem(csMarking ,"Marking "      ) ;

    DM.ARAY[riPSB ].SetDispItem(csNone    ,"None "         ) ; //DM.ARAY[riPSB ].SetDispItem(csNone    ,"None "         ) ;
    DM.ARAY[riPSB ].SetDispItem(csUnkwn   ,"Unkwn "        ) ; //DM.ARAY[riPSB ].SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    DM.ARAY[riPSB ].SetDispItem(csRslt1   ,"V "+OM.DevOptn.sCheckR1FailName +" ") ; //DM.ARAY[riWR3 ].SetDispItem(csRslt1   ,"V Empty "      ) ;
    DM.ARAY[riPSB ].SetDispItem(csWork    ,"Work "         ) ; //DM.ARAY[riPSB ].SetDispItem(csWork    ,"Work "         ) ;
    DM.ARAY[riPSB ].SetDispItem(csMarking ,"Marking "      ) ; //DM.ARAY[riPSB ].SetDispItem(csMarking ,"Marking "      ) ;
    DM.ARAY[riPSB ].SetDispItem(csToBuf   ,"ToBuf "        ) ; //DM.ARAY[riPSB ].SetDispItem(csToBuf   ,"ToBuf "        ) ;
    DM.ARAY[riPSB ].SetDispItem(csFromBuf ,"FromBuf "      ) ; //DM.ARAY[riPSB ].SetDispItem(csFromBuf ,"FromBuf "      ) ;

    DM.ARAY[riULD ].SetDispItem(csNone   ,"None  "     ) ; //DM.ARAY[riLDR ].SetDispColor(csNone  ,clWhite      );
    DM.ARAY[riULD ].SetDispItem(csEmpty  ,"Empty "     ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite      );
    DM.ARAY[riULD ].SetDispItem(csWork   ,"Work  "     ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite      );

    DM.ARAY[riTRY ].SetDispItem(csNone   ,"None "      ) ; //DM.ARAY[riLDR ].SetDispColor(csNone  ,clWhite      );
    DM.ARAY[riTRY ].SetDispItem(csEmpty  ,"Empty "     ) ; //DM.ARAY[riLDR ].SetDispColor(csEmpty ,clWhite      );
    DM.ARAY[riTRY ].SetDispItem(csWait   ,"Wait "      ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen );
    DM.ARAY[riTRY ].SetDispItem(csWork   ,"Work "      ) ; //DM.ARAY[riLDR ].SetDispColor(csWork  ,clMoneyGreen );

    DM.ARAY[riUPK ].SetDispItem(csNone   ,"None  "     ) ;
    DM.ARAY[riUPK ].SetDispItem(csEmpty  ,"Empty "     ) ;
    DM.ARAY[riUPK ].SetDispItem(csMask   ,"Paper "     ) ;
    DM.ARAY[riUPK ].SetDispItem(csWork   ,"Work  "     ) ;


    HED.DpAray1.SetDispItem(csNone    ,"None "         ) ;
    HED.DpAray1.SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    HED.DpAray1.SetDispItem(csWork    ,"Work "         ) ;
    HED.DpAray1.SetDispItem(csRslt1   ,"V "+OM.DevOptn.sCheckR1FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt2   ,"V "+OM.DevOptn.sCheckR2FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt3   ,"V "+OM.DevOptn.sCheckR3FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt4   ,"V "+OM.DevOptn.sCheckR4FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt5   ,"V "+OM.DevOptn.sCheckR5FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt6   ,"V "+OM.DevOptn.sCheckR6FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt7   ,"V "+OM.DevOptn.sCheckR7FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt8   ,"V "+OM.DevOptn.sCheckR8FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt9   ,"V "+OM.DevOptn.sCheckR9FailName +" ") ;
    HED.DpAray1.SetDispItem(csRslt10  ,"V "+OM.DevOptn.sCheckR10FailName+" ") ;
    HED.DpAray1.SetDispItem(csRslt11  ,"V "+OM.DevOptn.sCheckR11FailName+" ") ;
    HED.DpAray1.SetDispItem(csRslt12  ,"V "+OM.DevOptn.sCheckR12FailName+" ") ;
    HED.DpAray1.SetDispItem(csRslt13  ,"V "+OM.DevOptn.sCheckR13FailName+" ") ;
    HED.DpAray1.SetDispItem(csRslt14  ,"V "+OM.DevOptn.sCheckR14FailName+" ") ;
    HED.DpAray1.SetDispItem(csToBuf   ,"ToBuf "        ) ;
    HED.DpAray1.SetDispItem(csFromBuf ,"FromBuf "      ) ;

    HED.DpAray2.SetDispItem(csNone    ,"None "         ) ;
    HED.DpAray2.SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    HED.DpAray2.SetDispItem(csWork    ,"Work "         ) ;
    HED.DpAray2.SetDispItem(csRslt1   ,"V "+OM.DevOptn.sCheckR1FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt2   ,"V "+OM.DevOptn.sCheckR2FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt3   ,"V "+OM.DevOptn.sCheckR3FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt4   ,"V "+OM.DevOptn.sCheckR4FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt5   ,"V "+OM.DevOptn.sCheckR5FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt6   ,"V "+OM.DevOptn.sCheckR6FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt7   ,"V "+OM.DevOptn.sCheckR7FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt8   ,"V "+OM.DevOptn.sCheckR8FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt9   ,"V "+OM.DevOptn.sCheckR9FailName +" ") ;
    HED.DpAray2.SetDispItem(csRslt10  ,"V "+OM.DevOptn.sCheckR10FailName+" ") ;
    HED.DpAray2.SetDispItem(csRslt11  ,"V "+OM.DevOptn.sCheckR11FailName+" ") ;
    HED.DpAray2.SetDispItem(csRslt12  ,"V "+OM.DevOptn.sCheckR12FailName+" ") ;
    HED.DpAray2.SetDispItem(csRslt13  ,"V "+OM.DevOptn.sCheckR13FailName+" ") ;
    HED.DpAray2.SetDispItem(csRslt14  ,"V "+OM.DevOptn.sCheckR14FailName+" ") ;
    HED.DpAray2.SetDispItem(csToBuf   ,"ToBuf "        ) ;
    HED.DpAray2.SetDispItem(csFromBuf ,"FromBuf "      ) ;

    HED.DpAray3.SetDispItem(csNone    ,"None "         ) ;
    HED.DpAray3.SetDispItem(csUnkwn   ,"Unkwn "        ) ;
    HED.DpAray3.SetDispItem(csWork    ,"Work "         ) ;
    HED.DpAray3.SetDispItem(csRslt1   ,"V "+OM.DevOptn.sCheckR1FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt2   ,"V "+OM.DevOptn.sCheckR2FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt3   ,"V "+OM.DevOptn.sCheckR3FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt4   ,"V "+OM.DevOptn.sCheckR4FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt5   ,"V "+OM.DevOptn.sCheckR5FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt6   ,"V "+OM.DevOptn.sCheckR6FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt7   ,"V "+OM.DevOptn.sCheckR7FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt8   ,"V "+OM.DevOptn.sCheckR8FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt9   ,"V "+OM.DevOptn.sCheckR9FailName +" ") ;
    HED.DpAray3.SetDispItem(csRslt10  ,"V "+OM.DevOptn.sCheckR10FailName+" ") ;
    HED.DpAray3.SetDispItem(csRslt11  ,"V "+OM.DevOptn.sCheckR11FailName+" ") ;
    HED.DpAray3.SetDispItem(csRslt12  ,"V "+OM.DevOptn.sCheckR12FailName+" ") ;
    HED.DpAray3.SetDispItem(csRslt13  ,"V "+OM.DevOptn.sCheckR13FailName+" ") ;
    HED.DpAray3.SetDispItem(csRslt14  ,"V "+OM.DevOptn.sCheckR14FailName+" ") ;
    HED.DpAray3.SetDispItem(csToBuf   ,"ToBuf "        ) ;
    HED.DpAray3.SetDispItem(csFromBuf ,"FromBuf "      ) ;

    HED.DpAray1.PopupMenuEnable(false);
    HED.DpAray2.PopupMenuEnable(false);
    HED.DpAray3.PopupMenuEnable(false);

    //메인폼 타이머에서 레벨 바꿀시에 자꾸 뻑이나서 아예 시작 할때 픽스 .
    for(int i =0 ; i < MAX_ARAY ; i++ ) {
        if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riUPK) DM.ARAY[i].PopupMenuEnable(true);
    }

    //TODO :: 타이머에서 PopupMenuEnble 동작시 뻑남.. 시작할때 true로 시작하여 모든 레벨에서 변경 가능하도록 수정. JS
    //for(int i =0 ; i < MAX_ARAY ; i++ ) {
    //    if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riUPK) DM.ARAY[i].PopupMenuEnable(false);
    //}

    SEQ.Reset();


    //Manual Button Caption
    btMan1_1  -> Caption = "[" + AnsiString(mcLDR_Home             ) + "]" + "home Cycle       ";  btMan1_1  -> Tag = (int)mcLDR_Home              ; btMan1_1  -> Visible = true ;
    btMan1_2  -> Caption = "[" + AnsiString(mcLDR_Supply           ) + "]" + "Supply Cycle     ";  btMan1_2  -> Tag = (int)mcLDR_Supply            ; btMan1_2  -> Visible = true ;
    btMan1_3  -> Caption = "[" + AnsiString(mcLDR_Pick             ) + "]" + "Pick Cycle       ";  btMan1_3  -> Tag = (int)mcLDR_Pick              ; btMan1_3  -> Visible = true ;
    btMan1_4  -> Caption = "[" + AnsiString(mcLDR_Out              ) + "]" + "Out Cycle        ";  btMan1_4  -> Tag = (int)mcLDR_Out               ; btMan1_4  -> Visible = true ;
    btMan1_5  -> Caption = "[" + AnsiString(mcLDR_WorkSttPs        ) + "]" + "Work Start Pos   ";  btMan1_5  -> Tag = (int)mcLDR_WorkSttPs         ; btMan1_5  -> Visible = true ;
    btMan1_6  -> Caption = "[" + AnsiString(mcLDR_PusherUpDn       ) + "]" + "Pusher Up/Dn     ";  btMan1_6  -> Tag = (int)mcLDR_PusherUpDn        ; btMan1_6  -> Visible = true ;
    btMan1_7  -> Caption = "[" + AnsiString(mcLDR_RotateFwBw       ) + "]" + "Rotate Fw/Bw     ";  btMan1_7  -> Tag = (int)mcLDR_RotateFwBw        ; btMan1_7  -> Visible = true ;
    btMan1_8  -> Caption = "[" + AnsiString(mcRAL_PreStprDn        ) + "]" + "PRB Stpr Up/Dn   ";  btMan1_8  -> Tag = (int)mcRAL_PreStprDn         ; btMan1_8  -> Visible = true ;
    btMan1_14 -> Caption = "[" + AnsiString(mcAllHome              ) + "]" + "All Home         ";  btMan1_14 -> Tag = (int)mcAllHome               ; btMan1_14 -> Visible = true ;

    btMan2_1  -> Caption = "[" + AnsiString(mcHED_Wk1CmpFB         ) + "]" + "Clamp Fw/Bw      ";  btMan2_1  -> Tag = (int)mcHED_Wk1CmpFB          ; btMan2_1  -> Visible = true ;
    btMan2_2  -> Caption = "[" + AnsiString(mcHED_Wk1InsFB         ) + "]" + "Insert Up/Dn     ";  btMan2_2  -> Tag = (int)mcHED_Wk1InsFB          ; btMan2_2  -> Visible = true ;
    btMan2_3  -> Caption = "[" + AnsiString(mcHED_Wk1AlgnFB        ) + "]" + "Align Fw/Bw      ";  btMan2_3  -> Tag = (int)mcHED_Wk1AlgnFB         ; btMan2_3  -> Visible = true ;
    btMan2_4  -> Caption = "[" + AnsiString(mcHED_Wk1StrpFB        ) + "]" + "Stopper Up/Dn    ";  btMan2_4  -> Tag = (int)mcHED_Wk1StrpFB         ; btMan2_4  -> Visible = true ;
    btMan2_5  -> Caption = "[" + AnsiString(mcHED_Wk1InversePs     ) + "]" + "Inverse Pos      ";  btMan2_5  -> Tag = (int)mcHED_Wk1InversePs      ; btMan2_5  -> Visible = true ;
    btMan2_6  -> Caption = "[" + AnsiString(mcHED_Wk1NormalPs      ) + "]" + "Normal Pos       ";  btMan2_6  -> Tag = (int)mcHED_Wk1NormalPs       ; btMan2_6  -> Visible = true ;
    btMan2_14 -> Caption = "[" + AnsiString(mcAllHome              ) + "]" + "All Home         ";  btMan2_14 -> Tag = (int)mcAllHome               ; btMan2_14 -> Visible = true ;

    btMan3_1  -> Caption = "[" + AnsiString(mcHED_Home             ) + "]" + "home Cycle       ";  btMan3_1  -> Tag = (int)mcHED_Home              ; btMan3_1  -> Visible = true ;
    btMan3_2  -> Caption = "[" + AnsiString(mcHED_InspMan          ) + "]" + "Visn Inspection  ";  btMan3_2  -> Tag = (int)mcHED_InspMan           ; btMan3_2  -> Visible = true ;
    btMan3_3  -> Caption = "[" + AnsiString(mcHED_Wk2AlgnFB        ) + "]" + "WK2 Algn Fw/Bw   ";  btMan3_3  -> Tag = (int)mcHED_Wk2AlgnFB         ; btMan3_3  -> Visible = true ;
    btMan3_4  -> Caption = "[" + AnsiString(mcHED_Wk2StrpFB        ) + "]" + "WK2 Stpr Up/Dn   ";  btMan3_4  -> Tag = (int)mcHED_Wk2StrpFB         ; btMan3_4  -> Visible = true ;
    btMan3_5  -> Caption = "[" + AnsiString(mcHED_Wk3AlgnFB        ) + "]" + "WK3 Algn Fw/Bw   ";  btMan3_5  -> Tag = (int)mcHED_Wk3AlgnFB         ; btMan3_5  -> Visible = true ;
    btMan3_6  -> Caption = "[" + AnsiString(mcHED_Wk3StrpFB        ) + "]" + "WK3 Stpr Up/Dn   ";  btMan3_6  -> Tag = (int)mcHED_Wk3StrpFB         ; btMan3_6  -> Visible = true ;
    btMan3_7  -> Caption = "[" + AnsiString(mcHED_FeedingOnOff     ) + "]" + "WRK Feed On/Off  ";  btMan3_7  -> Tag = (int)mcHED_FeedingOnOff      ; btMan3_7  -> Visible = true ;
    btMan3_14 -> Caption = "[" + AnsiString(mcAllHome              ) + "]" + "All Home         ";  btMan3_14 -> Tag = (int)mcAllHome               ; btMan3_14 -> Visible = true ;

    btMan4_1  -> Caption = "[" + AnsiString(mcPSB_Home             ) + "]" + "home Cycle       ";  btMan4_1  -> Tag = (int)mcPSB_Home              ; btMan4_1  -> Visible = true ;
    btMan4_2  -> Caption = "[" + AnsiString(mcPSB_InitPos          ) + "]" + "Init Pos         ";  btMan4_2  -> Tag = (int)mcPSB_InitPos           ; btMan4_2  -> Visible = true ;
    btMan4_3  -> Caption = "[" + AnsiString(mcPSB_MrkSttPs         ) + "]" + "Marking Pos      ";  btMan4_3  -> Tag = (int)mcPSB_MrkSttPs          ; btMan4_3  -> Visible = true ;
    btMan4_4  -> Caption = "[" + AnsiString(mcPSB_VisnSttPs        ) + "]" + "Vision Pos       ";  btMan4_4  -> Tag = (int)mcPSB_VisnSttPs         ; btMan4_4  -> Visible = true ;
    btMan4_5  -> Caption = "[" + AnsiString(mcPSB_AlgnFB           ) + "]" + "Align Fw/Bw      ";  btMan4_5  -> Tag = (int)mcPSB_AlgnFB            ; btMan4_5  -> Visible = true ;
    btMan4_6  -> Caption = "[" + AnsiString(mcPSB_StprUD           ) + "]" + "Stopper Up/Dn    ";  btMan4_6  -> Tag = (int)mcPSB_StprUD            ; btMan4_6  -> Visible = true ;
    btMan4_7  -> Caption = "[" + AnsiString(mcPSB_MarkingUD        ) + "]" + "Marking Up/Dn    ";  btMan4_7  -> Tag = (int)mcPSB_MarkingUD         ; btMan4_7  -> Visible = true ;
    btMan4_8  -> Caption = "[" + AnsiString(mcPSB_MrkChangPs       ) + "]" + "Mrk Change Pos   ";  btMan4_8  -> Tag = (int)mcPSB_MrkChangPs        ; btMan4_8  -> Visible = true ;
    btMan4_9  -> Caption = "[" + AnsiString(mcPSB_TestMarking      ) + "]" + "Test Marking     ";  btMan4_9  -> Tag = (int)mcPSB_TestMarking       ; btMan4_9  -> Visible = true ;
    btMan4_10 -> Caption = "[" + AnsiString(mcPSB_FeedingOnOff     ) + "]" + "Feeding On/Off   ";  btMan4_10 -> Tag = (int)mcPSB_FeedingOnOff      ; btMan4_10 -> Visible = true ;
    btMan4_11 -> Caption = "[" + AnsiString(mcPSB_Remove           ) + "]" + "ReMoveCycle      ";  btMan4_11 -> Tag = (int)mcPSB_Remove            ; btMan4_11 -> Visible = true ;
    btMan4_14 -> Caption = "[" + AnsiString(mcAllHome              ) + "]" + "All Home         ";  btMan4_14 -> Tag = (int)mcAllHome               ; btMan4_14 -> Visible = true ;

    btMan5_1  -> Caption = "[" + AnsiString(mcULD_Home             ) + "]" + "Home Cycle       ";  btMan5_1  -> Tag = (int)mcULD_Home              ; btMan5_1  -> Visible = true ;
    btMan5_2  -> Caption = "[" + AnsiString(mcULD_StprFwPs         ) + "]" + "Stopper Fw Pos   ";  btMan5_2  -> Tag = (int)mcULD_StprFwPs          ; btMan5_2  -> Visible = true ;
    btMan5_3  -> Caption = "[" + AnsiString(mcULD_StprBwPs         ) + "]" + "Stopper Bw Pos   ";  btMan5_3  -> Tag = (int)mcULD_StprBwPs          ; btMan5_3  -> Visible = true ;
    btMan5_4  -> Caption = "[" + AnsiString(mcULD_VaccumOnOff      ) + "]" + "Vaccum On/Off    ";  btMan5_4  -> Tag = (int)mcULD_VaccumOnOff       ; btMan5_4  -> Visible = true ;
    btMan5_5  -> Caption = "[" + AnsiString(mcULD_EjectOnOff       ) + "]" + "Ejectr On/Off    ";  btMan5_5  -> Tag = (int)mcULD_EjectOnOff        ; btMan5_5  -> Visible = true ;
    btMan5_6  -> Caption = "[" + AnsiString(mcULD_TrayMove         ) + "]" + "Tray Place       ";  btMan5_6  -> Tag = (int)mcULD_TrayMove          ; btMan5_6  -> Visible = true ;
    btMan5_7  -> Caption = "[" + AnsiString(mcULD_TrayOpen         ) + "]" + "Tray Open        ";  btMan5_7  -> Tag = (int)mcULD_TrayOpen          ; btMan5_7  -> Visible = true ;
    btMan5_8  -> Caption = "[" + AnsiString(mcULD_PaperSupply      ) + "]" + "Paper Supply     ";  btMan5_8  -> Tag = (int)mcULD_PaperSupply       ; btMan5_8  -> Visible = true ;
    btMan5_9  -> Caption = "[" + AnsiString(mcULD_PaperWorkStt     ) + "]" + "Paper Work Ps    ";  btMan5_9  -> Tag = (int)mcULD_PaperWorkStt      ; btMan5_9  -> Visible = true ;
    btMan5_10 -> Caption = "[" + AnsiString(mcULD_PickPs           ) + "]" + "Pick Pos         ";  btMan5_10 -> Tag = (int)mcULD_PickPs            ; btMan5_10 -> Visible = true ;
    btMan5_11 -> Caption = "[" + AnsiString(mcULD_FrstPlacePs      ) + "]" + "Frst Place Pos   ";  btMan5_11 -> Tag = (int)mcULD_FrstPlacePs       ; btMan5_11 -> Visible = true ;
    btMan5_12 -> Caption = "[" + AnsiString(mcULD_PaperPs          ) + "]" + "Paper Pos        ";  btMan5_12 -> Tag = (int)mcULD_PaperPs           ; btMan5_12 -> Visible = true ;
    btMan5_13 -> Caption = "[" + AnsiString(mcULD_Pick             ) + "]" + "Pick Action      ";  btMan5_13 -> Tag = (int)mcULD_Pick              ; btMan5_13 -> Visible = true ;
    btMan5_14 -> Caption = "[" + AnsiString(mcULD_Place            ) + "]" + "Place Action     ";  btMan5_14 -> Tag = (int)mcULD_Place             ; btMan5_14 -> Visible = true ;
    //btMan5_14 -> Caption = "[" + AnsiString(mcAllHome              ) + "]" + "All Home         ";  btMan5_14 -> Tag = (int)mcAllHome               ; btMan5_14 -> Visible = true ;

    btMan6_1  -> Caption = "[" + AnsiString(mcRAL_Home             ) + "]" + "RAL Home         ";  btMan6_1  -> Tag = (int)mcRAL_Home              ; btMan6_1  -> Visible = true ;
    btMan6_2  -> Caption = "[" + AnsiString(mcULM_Home             ) + "]" + "ULM Home         ";  btMan6_2  -> Tag = (int)mcULM_Home              ; btMan6_2  -> Visible = true ;
    btMan6_3  -> Caption = "[" + AnsiString(mcETC_AllBeltOn        ) + "]" + "All Belt On      ";  btMan6_3  -> Tag = (int)mcETC_AllBeltOn         ; btMan6_3  -> Visible = true ;
    btMan6_4  -> Caption = "[" + AnsiString(mcETC_AllBeltOff       ) + "]" + "All Belt Off     ";  btMan6_4  -> Tag = (int)mcETC_AllBeltOff        ; btMan6_4  -> Visible = true ;
    btMan6_5  -> Caption = "[" + AnsiString(mcHED_AllStprUp        ) + "]" + "All Stpr Up      ";  btMan6_5  -> Tag = (int)mcHED_AllStprUp         ; btMan6_5  -> Visible = true ;
    btMan6_6  -> Caption = "[" + AnsiString(mcETC_ALlStprDn        ) + "]" + "All Stpr Dn      ";  btMan6_6  -> Tag = (int)mcETC_ALlStprDn         ; btMan6_6  -> Visible = true ;
    btMan6_7  -> Caption = "[" + AnsiString(mcETC_MainAirOff       ) + "]" + "Main Air Off     ";  btMan6_7  -> Tag = (int)mcETC_MainAirOff        ; btMan6_7  -> Visible = true ;
    btMan6_8  -> Caption = "[" + AnsiString(mcETC_InTrayStt        ) + "]" + "In Tray Start Ps ";  btMan6_8  -> Tag = (int)mcETC_InTrayStt         ; btMan6_8  -> Visible = true ;
    btMan6_9  -> Caption = "[" + AnsiString(mcETC_OutTrayStt       ) + "]" + "Out Tray Start Ps";  btMan6_9  -> Tag = (int)mcETC_OutTrayStt        ; btMan6_9  -> Visible = true ;
    btMan6_10 -> Caption = "[" + AnsiString(mcETC_PaperStt         ) + "]" + "Paper Start Ps   ";  btMan6_10 -> Tag = (int)mcETC_PaperStt          ; btMan6_10 -> Visible = true ;
    btMan6_11 -> Caption = "[" + AnsiString(mcETC_PaperSupply      ) + "]" + "Paper Supply     ";  btMan6_11 -> Tag = (int)mcETC_PaperSupply       ; btMan6_11 -> Visible = true ;
    btMan6_14 -> Caption = "[" + AnsiString(mcAllHome              ) + "]" + "All Home         ";  btMan6_14 -> Tag = (int)mcAllHome               ; btMan6_14 -> Visible = true ;
//    btMan5_8  -> Caption = "[" + AnsiString(mcETC_ManLotEnd     ) + "]" + "Lot End          " ;  btMan5_8  -> Tag = (int)mcETC_ManLotEnd       ; btMan5_8  -> Visible = true ;

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::LbResultChange()
{
    lbR1  -> Caption = OM.DevOptn.sCheckR1FailName  ;
    lbR2  -> Caption = OM.DevOptn.sCheckR2FailName  ;
    lbR3  -> Caption = OM.DevOptn.sCheckR3FailName  ;
    lbR4  -> Caption = OM.DevOptn.sCheckR4FailName  ;
    lbR5  -> Caption = OM.DevOptn.sCheckR5FailName  ;
    lbR6  -> Caption = OM.DevOptn.sCheckR6FailName  ;
    lbR7  -> Caption = OM.DevOptn.sCheckR7FailName  ;
    lbR8  -> Caption = OM.DevOptn.sCheckR8FailName  ;
    lbR9  -> Caption = OM.DevOptn.sCheckR9FailName  ;
    lbR10 -> Caption = OM.DevOptn.sCheckR10FailName ;
    lbR11 -> Caption = OM.DevOptn.sCheckR11FailName ;
    lbR12 -> Caption = OM.DevOptn.sCheckR12FailName ;
    lbR13 -> Caption = OM.DevOptn.sCheckR13FailName ;
    lbR14 -> Caption = OM.DevOptn.sCheckR14FailName ;
}

void __fastcall TFrmOperation::FormShow(TObject *Sender)
{
    tmUpdate1->Enabled = true ;

    OM.LoadCmnOptn();
    LbResultChange();

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
    tmUpdate1->Enabled = false;
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
    if (!MT_GetHomeEnd(miLDR_XPsh)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miLDR_YFed)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miLDR_XPck)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miLDR_ZPck)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miWK1_YFlp)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miWK1_XIns)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miWRK_XVsn)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miWRK_YVsn)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miPSB_XMrk)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miPSB_YMrk)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miULD_ZPck)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miULD_YPck)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miULD_ZTrI)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miULD_ZTrO)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miULD_XStp)){isAllHomeEnd = false; }
    if (!MT_GetHomeEnd(miULD_ZPpr)){isAllHomeEnd = false; }

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

    bool LDRMGZDetect = DM.ARAY[riLDR].GetCntExist() && IO_GetX(xLDR_MgzWork);
    bool ULDMGZDetect = DM.ARAY[riTRY].GetCntExist() && IO_GetX(xULD_LTrayDetect);

    if(LDRMGZDetect && ULDMGZDetect) MM.SetManCycle(mcETC_ManLotEnd);
    else if( LDRMGZDetect && !ULDMGZDetect) MM.SetManCycle(mcLDR_Out      );
    else if(!LDRMGZDetect &&  ULDMGZDetect) MM.SetManCycle(mcULD_TrayMove );

    LT.LotEnd ();
    DM.ClearMap();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnMarkingSkipDblClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

//    if(FM_GetLevel() == lvOperator) return ;

    switch(iIndex) {
        case 0 :                                 break  ;
        case 1 : if(FM_GetLevel() == lvOperator) return ;
        case 2 : if(FM_GetLevel() == lvOperator) return ;
        case 3 : if(FM_GetLevel() == lvOperator) return ;
        case 4 : if(FM_GetLevel() == lvOperator) return ;
        case 5 : if(FM_GetLevel() == lvOperator) return ;
        case 6 : if(FM_GetLevel() == lvOperator) return ;
        case 7 : if(FM_GetLevel() == lvOperator) return ;
        case 8 : if(FM_GetLevel() == lvOperator) return ;
    }

    if (FM_MsgYesNo("Confirm", "Would you like to invert the flag?") != mrYes) return ;

    switch(iIndex) {
        case 0 :                                                            break ;
        case 1 : OM.CmnOptn.bDoorSkip       = !OM.CmnOptn.bDoorSkip       ; break ;
        case 2 : OM.CmnOptn.bAfterMrkRemove = !OM.CmnOptn.bAfterMrkRemove ; break ;
        case 3 : OM.CmnOptn.bMrkAllSkip     = !OM.CmnOptn.bMrkAllSkip     ; break ;
        case 4 : OM.CmnOptn.bVs1Skip        = !OM.CmnOptn.bVs1Skip        ; break ;
        case 5 : OM.CmnOptn.bVs2Skip        = !OM.CmnOptn.bVs2Skip        ; break ;
        case 6 : OM.CmnOptn.bVs3Skip        = !OM.CmnOptn.bVs3Skip        ; break ;
        case 7 : OM.CmnOptn.bVsMrkSkip      = !OM.CmnOptn.bVsMrkSkip      ; break ;
        case 8 : OM.CmnOptn.bCheckFailCnt   = !OM.CmnOptn.bCheckFailCnt   ; break ;
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

bool __fastcall TFrmOperation::CheckStripDisappear()
{
    //스트립 사라짐 에러.
    if(( DM.ARAY[riWR1].GetCntExist() || DM.ARAY[riVS1].GetCntExist()) && !IO_GetX(xWR1_Pkg) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision 1 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riWR1 ].SetStat(csNone); DM.ARAY[riVS1 ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if(( DM.ARAY[riWR2].GetCntExist() || DM.ARAY[riVS2].GetCntExist()) && !IO_GetX(xWR2_Pkg) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision 2 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riWR2 ].SetStat(csNone); DM.ARAY[riVS2 ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if(( DM.ARAY[riWR3].GetCntExist() || DM.ARAY[riVS3].GetCntExist()) && !IO_GetX(xWR3_Pkg) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision 3 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riWR3 ].SetStat(csNone); DM.ARAY[riVS3 ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if(( DM.ARAY[riPSB].GetCntExist() || DM.ARAY[riVS4].GetCntExist()) && !IO_GetX(xPSB_Pkg) && !OM.CmnOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision 4 Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riPSB ].SetStat(csNone); DM.ARAY[riVS4 ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    //else if (!DM.ARAY[riLDR].CheckAllStat(csNone) && !IO_GetX(xLDR_MgzWork) && !OM.CmnOptn.bDryRun) {
    //    if(FM_MsgYesNo("Err " , "[Loader MGZ Disappear] \nDo you want to clear Strip?"         ) == mrYes ) { DM.ARAY[riLDR ].SetStat(csNone); SEQ._bBtnStart = true ;}
    //}
    //else if (!DM.ARAY[riULD].CheckAllStat(csNone) && !IO_GetX(xULD_TrayPushDetect) && !OM.CmnOptn.bDryRun) {
    //    if(FM_MsgYesNo("Err " , "[UnLoader Tray Disappear] \nDo you want to clear Strip?"      ) == mrYes ) { DM.ARAY[riULD ].SetStat(csNone); SEQ._bBtnStart = true ;}
    //}

    return true ;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmOperation::CheckStripUnknwon()
{
    return true ;
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
void __fastcall TFrmOperation::btWK1ClearClick(TObject *Sender)
{
    String sTemp;

    TButton *pBtn = dynamic_cast <TButton *> (Sender);
    sTemp = pBtn->Name;
    int iUPKCnt = 0 ;

         if(sTemp == "btWK1Clear") { DM.ARAY[riWR1].SetStat(csNone) ; DM.ARAY[riVS1].SetStat(csNone) ;}
    else if(sTemp == "btWK2Clear") { DM.ARAY[riWR2].SetStat(csNone) ; DM.ARAY[riVS2].SetStat(csNone) ;}
    else if(sTemp == "btWK3Clear") { DM.ARAY[riWR3].SetStat(csNone) ; DM.ARAY[riVS3].SetStat(csNone) ;}
    else if(sTemp == "btPRBClear") { DM.ARAY[riPRB].SetStat(csNone) ; }
    else if(sTemp == "btPSBClear") { DM.ARAY[riPSB].SetStat(csNone) ; }
    else if(sTemp == "btULDClear") {
       if(GetLastStrip() && DM.ARAY[riULD].GetCntExist() && LT.m_bLotOpen) {
//            ULD.m_bLotEnded = true ; //dd주석
            //memcpy(&ULD.EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo)); ?? 쓰는데가 없음. dd 주석처리
            //TODO :: Last Tray Cnt 넣어주는데가 없어서 넣음.
            //LT.LotInfo.iLastTrayCnt = (DM.ARAY[riTRY].GetCntStat(csWork) * OM.DevInfo.iULDPkgCnt) + DM.ARAY[riUPK].GetCntStat(csWork);

            if(DM.ARAY[riUPK].GetCntStat(csWork)) iUPKCnt = DM.ARAY[riUPK].GetCntStat(csWork) - 1 ;
            else                                  iUPKCnt = 0 ;
            LT.LotInfo.iLastTrayCnt = (DM.ARAY[riTRY].GetCntStat(csWork) * (OM.DevInfo.iULDPkgCnt - 1 )) + iUPKCnt; //TODO :: ?? 고준선의 폐해!!! 11개로 하자고 그래서 그냥함 ㅈㅅ


            Trace("LotEnd",DM.ARAY[riULD].GetLotNo().c_str());
            LT.LotInfo.dEndTime = Now();
            LT.WriteLotLog   ();
            LT.WriteLotDayLog();
            Trace("WriteLotLog",DM.ARAY[riULD].GetLotNo().c_str());
            //LT.LotEnd();
        }
        DM.ARAY[riULD].SetStat(csNone) ;
    }
}
bool TFrmOperation::GetLastStrip()
{
    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWR1Exist = !DM.ARAY[riWR1].CheckAllStat(csNone) ;
    bool bWR2Exist = !DM.ARAY[riWR2].CheckAllStat(csNone) ;
    bool bWR3Exist = !DM.ARAY[riWR3].CheckAllStat(csNone) ;
    bool bPSBExist = !DM.ARAY[riPSB].CheckAllStat(csNone) ;
    bool bULDExist = !DM.ARAY[riULD].CheckAllStat(csNone) ;

    bool bLDRDiff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riLDR].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bPRBDiff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riPRB].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bWR1Diff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riWR1].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bWR2Diff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riWR2].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bWR3Diff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riWR3].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bPSBDiff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riPSB].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bULDDiff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riULD].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;

    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

         if(bULDExist) return bULDDiff ;
    else if(bPSBExist) return bPSBDiff ;
    else if(bWR3Exist) return bWR3Diff ;
    else if(bWR2Exist) return bWR2Diff ;
    else if(bWR1Exist) return bWR1Diff ;
    else if(bPRBExist) return bPRBDiff ;
    else if(bLDRExist) return bLDRDiff ;
    else if(!bLastMgz) return false    ;
    else               return true     ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btWK2ClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo ,sMsg;
    sLotNo   = "WorkZone 2 :" + DM.ARAY[riWR2].GetLotNo() + "Vision 2 :" + DM.ARAY[riVS2].GetLotNo() + " ";;
    sSlotNo  = "WorkZone 2 :" + AnsiString(DM.ARAY[riWR2].GetID().ToIntDef(99)%100) + "Vision 2 :" + AnsiString(DM.ARAY[riVS2].GetID().ToIntDef(99)%100) + " ";
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    DM.ARAY[riWR2].SetStat(csNone);
    DM.ARAY[riVS2].SetStat(csNone);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btWK3ClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo ,sMsg;
    sLotNo   = "WorkZone 3 :" + DM.ARAY[riWR3].GetLotNo() + "Vision 3 :" + DM.ARAY[riVS3].GetLotNo() + " ";;
    sSlotNo  = "WorkZone 3 :" + AnsiString(DM.ARAY[riWR3].GetID().ToIntDef(99)%100) + "Vision 3 :" + AnsiString(DM.ARAY[riVS3].GetID().ToIntDef(99)%100) + " ";
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    DM.ARAY[riWR3].SetStat(csNone);
    DM.ARAY[riVS3].SetStat(csNone);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPSBClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo ,sMsg;
    sLotNo   = "PostBuff1:" + DM.ARAY[riPSB].GetLotNo() + "Vision 4 :" + DM.ARAY[riVS4].GetLotNo() + " ";;
    sSlotNo  = "PostBuff1:" + AnsiString(DM.ARAY[riPSB].GetID().ToIntDef(99)%100) + "Vision 4 :" + AnsiString(DM.ARAY[riVS4].GetID().ToIntDef(99)%100) + " ";
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    DM.ARAY[riPSB].SetStat(csNone);
    DM.ARAY[riVS4].SetStat(csNone);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::gbChipColorClick(TObject *Sender)
{
    FormVisionRlstSet->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::Label27Click(TObject *Sender)
{
    FormLimitCnt -> ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPRBClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo ,sMsg;
    sLotNo   = "Prebuffer :" + DM.ARAY[riPRB].GetLotNo() ;
    sSlotNo  = "Prebuffer :" + AnsiString(DM.ARAY[riPRB].GetID().ToIntDef(99)%100);
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    DM.ARAY[riPRB].SetStat(csNone);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btULDClearClick(TObject *Sender)
{
    AnsiString sLotNo , sSlotNo ,sMsg;
    sLotNo   = "UnLoader :" + DM.ARAY[riULD].GetLotNo();
    sSlotNo  = "UnLoader :" + AnsiString(DM.ARAY[riULD].GetID().ToIntDef(99)%100);
    sMsg     = "Clear Bt Clicked" ;
    SPC.SaveClearLog(OM.m_sCrntDev , sLotNo , sSlotNo , sMsg);

    DM.ARAY[riULD].SetStat(csNone);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnLotNoClick(TObject *Sender)
{
    edMeasure -> Text = OM.DevOptn.iLotEndMgzCnt ;
    pnLotMGZCnt2 ->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btnOKClick(TObject *Sender)
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
void __fastcall TFrmOperation::btWk1InputClick(TObject *Sender)
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
            DM.ARAY[riWR1].SetID(sId.ToIntDef(99999));
            break ;

        case 2 :
            sLotNo  = edWk2LotNo ->Text ;
            sId     = edWk2Id    ->Text ;
            DM.ARAY[riWR2].SetLotNo(sLotNo) ;
            DM.ARAY[riWR2].SetID(sId.ToIntDef(99999));
            break ;

        case 3 :
            sLotNo  = edWk3LotNo ->Text ;
            sId     = edWk3Id    ->Text ;
            DM.ARAY[riWR3].SetLotNo(sLotNo) ;
            DM.ARAY[riWR3].SetID(sId.ToIntDef(99999));
            break ;

        case 4 :
            sLotNo  = edWk4LotNo ->Text ;
            sId     = edWk4Id    ->Text ;
            DM.ARAY[riPSB].SetLotNo(sLotNo) ;
            DM.ARAY[riPSB].SetID(sId.ToIntDef(99999));
            break ;
     }

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmUpdate1Timer(TObject *Sender)
{
    tmUpdate1->Enabled = false;

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

    static int iLev = -1;
    int iLevel = FM_GetLevel();

    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }

/*
    switch (iLevel) {
        case lvOperator : if( iLev != lvOperator) {
                              iLev = lvOperator ;
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)jriUPK) DM.ARAY[i].PopupMenuEnable(false);
                              }
                          }
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
                              iLev = lvEngineer ;
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riUPK) DM.ARAY[i].PopupMenuEnable(OM.MstOptn.bDataAccEng);
                              }
                          }
                          break ;
        case lvMaster   : if( iLev != lvMaster) {
                              iLev = lvMaster ;
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riUPK) DM.ARAY[i].PopupMenuEnable(true);
                              }
                          }
                          break ;
    }
*/

    lbVs1FailCnt  -> Caption = HED.DpAray1   .GetCntExist() - HED.DpAray1   .GetCntStat(csWork) ;
    lbVs2FailCnt  -> Caption = HED.DpAray2   .GetCntExist() - HED.DpAray2   .GetCntStat(csWork) ;
    lbVs3FailCnt  -> Caption = HED.DpAray3   .GetCntExist() - HED.DpAray3   .GetCntStat(csWork) ;
    lbMrkFailCnt  -> Caption = DM.ARAY[riLT1].GetStep();
    pnLotCnt      -> Caption = LDR.iMgzCnt ;

    //Manual Enabled
    pcManual -> Enabled = !SEQ._bRun ;

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

    int iLotWrkChip  = pnLotWorkChip -> Caption.ToIntDef(0) ;
    int iLotFailChip = pnLotFailChip -> Caption.ToIntDef(1) ;

    pnLotGoodChip -> Caption = iLotWrkChip   - iLotFailChip;

    if(OM.CmnOptn.bVs1Skip && OM.CmnOptn.bVs2Skip &&
       OM.CmnOptn.bVs3Skip && OM.CmnOptn.bVsMrkSkip){
           OM.CmnOptn.bVsAllSkip = true;
       }
    else {
       OM.CmnOptn.bVsAllSkip = false;
    }

    //EM_
    EM_DisplayList(lbErr);

    pnDoorSkip      -> Color    =  OM.CmnOptn.bDoorSkip      ? clLime : clRed ;
    pnAfterMrkRemove-> Color    =  OM.CmnOptn.bAfterMrkRemove? clLime : clRed ;
    pnMarkingSkip   -> Color    =  OM.CmnOptn.bMrkAllSkip    ? clLime : clRed ;
    pnVisn1NotUse   -> Color    =  OM.CmnOptn.bVs1Skip       ? clLime : clRed ;
    pnVisn2NotUse   -> Color    =  OM.CmnOptn.bVs2Skip       ? clLime : clRed ;
    pnVisn3NotUse   -> Color    =  OM.CmnOptn.bVs3Skip       ? clLime : clRed ;
    pnVisnMrkNotUse -> Color    =  OM.CmnOptn.bVsMrkSkip     ? clLime : clRed ;
    pnCheckFailCnt  -> Color    =  OM.CmnOptn.bCheckFailCnt  ? clLime : clRed ;

    pnDoorSkip      -> Caption  =  OM.CmnOptn.bDoorSkip      ? "ON" : "OFF" ;
    pnAfterMrkRemove-> Caption  =  OM.CmnOptn.bAfterMrkRemove? "ON" : "OFF" ;
    pnMarkingSkip   -> Caption  =  OM.CmnOptn.bMrkAllSkip    ? "ON" : "OFF" ;
    pnVisn1NotUse   -> Caption  =  OM.CmnOptn.bVs1Skip       ? "ON" : "OFF" ;
    pnVisn2NotUse   -> Caption  =  OM.CmnOptn.bVs2Skip       ? "ON" : "OFF" ;
    pnVisn3NotUse   -> Caption  =  OM.CmnOptn.bVs3Skip       ? "ON" : "OFF" ;
    pnVisnMrkNotUse -> Caption  =  OM.CmnOptn.bVsMrkSkip     ? "ON" : "OFF" ;
    pnCheckFailCnt  -> Caption  =  OM.CmnOptn.bCheckFailCnt  ? "ON" : "OFF" ;

    if(OM.CmnOptn.bCheckFailCnt) edLimAllFailCnt -> Text = OM.DevOptn.iCheckFailCnt;
    else edLimAllFailCnt -> Text = "";

    static bool bPreRun = SEQ._bRun ;
    if(SEQ._bRun && !bPreRun){
        pcWorkZone1 -> ActivePageIndex = 0 ;
        pcWorkZone2 -> ActivePageIndex = 0 ;
        pcWorkZone3 -> ActivePageIndex = 0 ;
        pcPostBuff  -> ActivePageIndex = 0 ;
    }
    bPreRun = SEQ._bRun ;
    if(SEQ._bRun){
        if(DM.ARAY[riVS4].CheckAllStat(csNone) || (DM.ARAY[riVS4].GetCntStat(csWork)+DM.ARAY[riVS4].GetCntStat(csGood)) == (OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt) ) {
            if(pcPostBuff -> ActivePageIndex != 0 ) pcPostBuff -> ActivePageIndex = 0 ;
        }
        else {
            if(pcPostBuff -> ActivePageIndex != 1 ) pcPostBuff -> ActivePageIndex = 1 ;
        }
    }



    btWk1Input  -> Visible = !SEQ._bRun ;
    btWk2Input  -> Visible = !SEQ._bRun ;
    btWk3Input  -> Visible = !SEQ._bRun ;
    btWk4Input  -> Visible = !SEQ._bRun ;

    if(SEQ._bInspDispr) { CheckStripDisappear(); SEQ._bInspDispr = false; } //함수 끝나고 자동 스타트 됍.
    if(SEQ._bInspUnkwn) { CheckStripUnknwon  (); SEQ._bInspUnkwn = false; } //함수 끝나고 자동 스타트 됍.

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

    DM.UpdateAray();
    HED.DpAray1.UpdateAray() ;
    HED.DpAray2.UpdateAray() ;
    HED.DpAray3.UpdateAray() ;

    edRslt1       -> Text = LT.LotInfo.iCntRslt1   ;
    edRslt2       -> Text = LT.LotInfo.iCntRslt2   ;
    edRslt3       -> Text = LT.LotInfo.iCntRslt3   ;
    edRslt4       -> Text = LT.LotInfo.iCntRslt4   ;
    edRslt5       -> Text = LT.LotInfo.iCntRslt5   ;
    edRslt6       -> Text = LT.LotInfo.iCntRslt6   ;
    edRslt7       -> Text = LT.LotInfo.iCntRslt7   ;
    edRslt8       -> Text = LT.LotInfo.iCntRslt8   ;
    edRslt9       -> Text = LT.LotInfo.iCntRslt9   ;
    edRslt10      -> Text = LT.LotInfo.iCntRslt10  ;
    edRslt11      -> Text = LT.LotInfo.iCntRslt11  ;
    edRslt12      -> Text = LT.LotInfo.iCntRslt12  ;
    edRslt13      -> Text = LT.LotInfo.iCntRslt13  ;
    edRslt14      -> Text = LT.LotInfo.iCntRslt14  ;
    edFail        -> Text = LT.LotInfo.iFailChip   ;
    edTotal       -> Text = LT.LotInfo.iWorkChip   ;
    edLastTrayCnt -> Text = LT.LotInfo.iLastTrayCnt;

    lbFailName1  -> Caption = OM.DevOptn.sCheckR1FailName    ;
    lbFailName2  -> Caption = OM.DevOptn.sCheckR2FailName    ;
    lbFailName3  -> Caption = OM.DevOptn.sCheckR3FailName    ;
    lbFailName4  -> Caption = OM.DevOptn.sCheckR4FailName    ;
    lbFailName5  -> Caption = OM.DevOptn.sCheckR5FailName    ;
    lbFailName6  -> Caption = OM.DevOptn.sCheckR6FailName    ;
    lbFailName7  -> Caption = OM.DevOptn.sCheckR7FailName    ;
    lbFailName8  -> Caption = OM.DevOptn.sCheckR8FailName    ;
    lbFailName9  -> Caption = OM.DevOptn.sCheckR9FailName    ;
    lbFailName10 -> Caption = OM.DevOptn.sCheckR10FailName   ;
    lbFailName11 -> Caption = OM.DevOptn.sCheckR11FailName   ;
    lbFailName12 -> Caption = OM.DevOptn.sCheckR12FailName   ;
    lbFailName13 -> Caption = OM.DevOptn.sCheckR13FailName   ;
    lbFailName14 -> Caption = OM.DevOptn.sCheckR14FailName   ;

    tmUpdate1 -> Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::FormDestroy(TObject *Sender)
{
    tmUpdate1 -> Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btVS1ResetClick(TObject *Sender)
{
    String sTemp;

    TButton *pBtn = dynamic_cast <TButton *> (Sender);
    sTemp = pBtn->Name;

    if(sTemp == "btVS1Reset") {
        if(!OM.CmnOptn.bVs1Skip)VC.SendReset(vsVisn1);
        if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
        if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].ChangeStat(csWork,csUnkwn) ;
    }
    else if(sTemp == "btVS2Reset") {
        if(!OM.CmnOptn.bVs2Skip)VC.SendReset(vsVisn2);
        if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
        if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].ChangeStat(csWork,csUnkwn) ;
    }
    else if(sTemp == "btVS3Reset") {
        if(!OM.CmnOptn.bVs3Skip)VC.SendReset(vsVisn3);
        if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat(csUnkwn) ;
        if(DM.ARAY[riWR3].GetCntExist())DM.ARAY[riWR3].ChangeStat(csWork,csUnkwn) ;
    }
    else if(sTemp == "btPSBReset") {
        if(!OM.CmnOptn.bVsMrkSkip)VC.SendReset(vsVisn4);
        if(DM.ARAY[riVS4].GetCntExist())DM.ARAY[riVS4].SetStat(csUnkwn) ;
        if(DM.ARAY[riPSB].GetCntExist())DM.ARAY[riPSB].ChangeStat(csWork,csUnkwn) ;
    }
}
//---------------------------------------------------------------------------

