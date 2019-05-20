//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmOperation.h"
#include "FrmLimitCnt.h"
#include "FrmVisionRlstSet.h"
#include "FrmMain.h"
//---------------------------------------------------------------------------
#include "DataMan.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Head.h"
#include "Rail.h"
#include "PstBuff.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOperation *FormOperation;
//---------------------------------------------------------------------------
__fastcall TFormOperation::TFormOperation(TComponent* Owner)
    : TForm(Owner)
{
    pcVsR1  ->DoubleBuffered = true ;
    pcVsR2  ->DoubleBuffered = true ;
    pcVsR3  ->DoubleBuffered = true ;
    pcVsR4  ->DoubleBuffered = true ;

    pnLDR      ->DoubleBuffered = true ;

    pn1B1      ->DoubleBuffered = true ;
    pnInsp1    ->DoubleBuffered = true ;
    pnVisn1    ->DoubleBuffered = true ;

    pn2B2      ->DoubleBuffered = true ;
    pnInsp2    ->DoubleBuffered = true ;
    pnVisn2    ->DoubleBuffered = true ;

    pn3B3      ->DoubleBuffered = true ;
    pnInsp3    ->DoubleBuffered = true ;
    pnVisn3    ->DoubleBuffered = true ;

    pnULD      ->DoubleBuffered = true ;

    tsV1R1     ->DoubleBuffered = true ;
    tsV1Result ->DoubleBuffered = true ;
    tsVs1      ->DoubleBuffered = true ;

    tsV2R2     ->DoubleBuffered = true ;
    tsV2Result ->DoubleBuffered = true ;
    tsVs2      ->DoubleBuffered = true ;

    tsV3R3     ->DoubleBuffered = true ;
    tsV3Result ->DoubleBuffered = true ;
    tsVs3      ->DoubleBuffered = true ;

    tsV4Result ->DoubleBuffered = true ;

    SetDispItem();

    SEQ.Reset();

}
String TFormOperation::GetRsltName(int _iStat)
{
    String sRsltName = "";
    
    if(_iStat == csRslt0) sRsltName = OM.CmnOptn.sRsltName0 ;
    if(_iStat == csRslt1) sRsltName = OM.CmnOptn.sRsltName1 ;
    if(_iStat == csRslt2) sRsltName = OM.CmnOptn.sRsltName2 ;
    if(_iStat == csRslt3) sRsltName = OM.CmnOptn.sRsltName3 ;
    if(_iStat == csRslt4) sRsltName = OM.CmnOptn.sRsltName4 ;
    if(_iStat == csRslt5) sRsltName = OM.CmnOptn.sRsltName5 ;
    if(_iStat == csRslt6) sRsltName = OM.CmnOptn.sRsltName6 ;
    if(_iStat == csRslt7) sRsltName = OM.CmnOptn.sRsltName7 ;
    if(_iStat == csRslt8) sRsltName = OM.CmnOptn.sRsltName8 ;
    if(_iStat == csRslt9) sRsltName = OM.CmnOptn.sRsltName9 ;
    if(_iStat == csRsltA) sRsltName = OM.CmnOptn.sRsltNameA ;
    if(_iStat == csRsltB) sRsltName = OM.CmnOptn.sRsltNameB ;
    if(_iStat == csRsltC) sRsltName = OM.CmnOptn.sRsltNameC ;
    if(_iStat == csRsltD) sRsltName = OM.CmnOptn.sRsltNameD ;
    if(_iStat == csRsltE) sRsltName = OM.CmnOptn.sRsltNameE ;
    if(_iStat == csRsltF) sRsltName = OM.CmnOptn.sRsltNameF ;
    if(_iStat == csRsltG) sRsltName = OM.CmnOptn.sRsltNameG ;
    if(_iStat == csRsltH) sRsltName = OM.CmnOptn.sRsltNameH ;
    if(_iStat == csRsltI) sRsltName = OM.CmnOptn.sRsltNameI ;
    if(_iStat == csRsltJ) sRsltName = OM.CmnOptn.sRsltNameJ ;
    if(_iStat == csRsltK) sRsltName = OM.CmnOptn.sRsltNameK ;
    if(_iStat == csRsltL) sRsltName = OM.CmnOptn.sRsltNameL ;

    return sRsltName;


}
bool TFormOperation::SetDispItem(EN_ARAY_ID _Aray)
{
    DM.ARAY[_Aray ].SetDispItem(csNone   ,"None "         ) ;
    DM.ARAY[_Aray ].SetDispItem(csUnkwn  ,"Unkwn "        ) ;
    DM.ARAY[_Aray ].SetDispItem(csWork   ,"Work "         ) ;
    if(_Aray == riPSB) DM.ARAY[_Aray ].SetDispItem(csEmpty  ,"Empty "         ) ;
    if(OM.CmnOptn.sRsltName0 != "") DM.ARAY[_Aray ].SetDispItem(csRslt0  ,"V "+OM.CmnOptn.sRsltName0+" ") ;
    if(OM.CmnOptn.sRsltName1 != "") DM.ARAY[_Aray ].SetDispItem(csRslt1  ,"V "+OM.CmnOptn.sRsltName1+" ") ;
    if(OM.CmnOptn.sRsltName2 != "") DM.ARAY[_Aray ].SetDispItem(csRslt2  ,"V "+OM.CmnOptn.sRsltName2+" ") ;
    if(OM.CmnOptn.sRsltName3 != "") DM.ARAY[_Aray ].SetDispItem(csRslt3  ,"V "+OM.CmnOptn.sRsltName3+" ") ;
    if(OM.CmnOptn.sRsltName4 != "") DM.ARAY[_Aray ].SetDispItem(csRslt4  ,"V "+OM.CmnOptn.sRsltName4+" ") ;
    if(OM.CmnOptn.sRsltName5 != "") DM.ARAY[_Aray ].SetDispItem(csRslt5  ,"V "+OM.CmnOptn.sRsltName5+" ") ;
    if(OM.CmnOptn.sRsltName6 != "") DM.ARAY[_Aray ].SetDispItem(csRslt6  ,"V "+OM.CmnOptn.sRsltName6+" ") ;
    if(OM.CmnOptn.sRsltName7 != "") DM.ARAY[_Aray ].SetDispItem(csRslt7  ,"V "+OM.CmnOptn.sRsltName7+" ") ;
    if(OM.CmnOptn.sRsltName8 != "") DM.ARAY[_Aray ].SetDispItem(csRslt8  ,"V "+OM.CmnOptn.sRsltName8+" ") ;
    if(OM.CmnOptn.sRsltName9 != "") DM.ARAY[_Aray ].SetDispItem(csRslt9  ,"V "+OM.CmnOptn.sRsltName9+" ") ;
    if(OM.CmnOptn.sRsltNameA != "") DM.ARAY[_Aray ].SetDispItem(csRsltA  ,"V "+OM.CmnOptn.sRsltNameA+" ") ;
    if(OM.CmnOptn.sRsltNameB != "") DM.ARAY[_Aray ].SetDispItem(csRsltB  ,"V "+OM.CmnOptn.sRsltNameB+" ") ;
    if(OM.CmnOptn.sRsltNameC != "") DM.ARAY[_Aray ].SetDispItem(csRsltC  ,"V "+OM.CmnOptn.sRsltNameC+" ") ;
    if(OM.CmnOptn.sRsltNameD != "") DM.ARAY[_Aray ].SetDispItem(csRsltD  ,"V "+OM.CmnOptn.sRsltNameD+" ") ;
    if(OM.CmnOptn.sRsltNameE != "") DM.ARAY[_Aray ].SetDispItem(csRsltE  ,"V "+OM.CmnOptn.sRsltNameE+" ") ;
    if(OM.CmnOptn.sRsltNameF != "") DM.ARAY[_Aray ].SetDispItem(csRsltF  ,"V "+OM.CmnOptn.sRsltNameF+" ") ;
    if(OM.CmnOptn.sRsltNameG != "") DM.ARAY[_Aray ].SetDispItem(csRsltG  ,"V "+OM.CmnOptn.sRsltNameG+" ") ;
    if(OM.CmnOptn.sRsltNameH != "") DM.ARAY[_Aray ].SetDispItem(csRsltH  ,"V "+OM.CmnOptn.sRsltNameH+" ") ;
    if(OM.CmnOptn.sRsltNameI != "") DM.ARAY[_Aray ].SetDispItem(csRsltI  ,"V "+OM.CmnOptn.sRsltNameI+" ") ;
    if(OM.CmnOptn.sRsltNameJ != "") DM.ARAY[_Aray ].SetDispItem(csRsltJ  ,"V "+OM.CmnOptn.sRsltNameJ+" ") ;
    if(OM.CmnOptn.sRsltNameK != "") DM.ARAY[_Aray ].SetDispItem(csRsltK  ,"V "+OM.CmnOptn.sRsltNameK+" ") ;
    if(OM.CmnOptn.sRsltNameL != "") DM.ARAY[_Aray ].SetDispItem(csRsltL  ,"V "+OM.CmnOptn.sRsltNameL+" ") ;
    DM.ARAY[_Aray ].SetDispItem(csToBuf  ,"ToBuf "        ) ;
    DM.ARAY[_Aray ].SetDispItem(csFromBuf,"FromBuf "      ) ;

}
bool TFormOperation::SetDispItem(CArray * riAray)
{
    riAray->SetDispItem(csNone   ,"None "         ) ;
    riAray->SetDispItem(csUnkwn  ,"Unkwn "        ) ;
    riAray->SetDispItem(csWork   ,"Work "         ) ;
    if(OM.CmnOptn.sRsltName0 != "") riAray->SetDispItem(csRslt0  ,"V "+OM.CmnOptn.sRsltName0+" ") ;
    if(OM.CmnOptn.sRsltName1 != "") riAray->SetDispItem(csRslt1  ,"V "+OM.CmnOptn.sRsltName1+" ") ;
    if(OM.CmnOptn.sRsltName2 != "") riAray->SetDispItem(csRslt2  ,"V "+OM.CmnOptn.sRsltName2+" ") ;
    if(OM.CmnOptn.sRsltName3 != "") riAray->SetDispItem(csRslt3  ,"V "+OM.CmnOptn.sRsltName3+" ") ;
    if(OM.CmnOptn.sRsltName4 != "") riAray->SetDispItem(csRslt4  ,"V "+OM.CmnOptn.sRsltName4+" ") ;
    if(OM.CmnOptn.sRsltName5 != "") riAray->SetDispItem(csRslt5  ,"V "+OM.CmnOptn.sRsltName5+" ") ;
    if(OM.CmnOptn.sRsltName6 != "") riAray->SetDispItem(csRslt6  ,"V "+OM.CmnOptn.sRsltName6+" ") ;
    if(OM.CmnOptn.sRsltName7 != "") riAray->SetDispItem(csRslt7  ,"V "+OM.CmnOptn.sRsltName7+" ") ;
    if(OM.CmnOptn.sRsltName8 != "") riAray->SetDispItem(csRslt8  ,"V "+OM.CmnOptn.sRsltName8+" ") ;
    if(OM.CmnOptn.sRsltName9 != "") riAray->SetDispItem(csRslt9  ,"V "+OM.CmnOptn.sRsltName9+" ") ;
    if(OM.CmnOptn.sRsltNameA != "") riAray->SetDispItem(csRsltA  ,"V "+OM.CmnOptn.sRsltNameA+" ") ;
    if(OM.CmnOptn.sRsltNameB != "") riAray->SetDispItem(csRsltB  ,"V "+OM.CmnOptn.sRsltNameB+" ") ;
    if(OM.CmnOptn.sRsltNameC != "") riAray->SetDispItem(csRsltC  ,"V "+OM.CmnOptn.sRsltNameC+" ") ;
    if(OM.CmnOptn.sRsltNameD != "") riAray->SetDispItem(csRsltD  ,"V "+OM.CmnOptn.sRsltNameD+" ") ;
    if(OM.CmnOptn.sRsltNameE != "") riAray->SetDispItem(csRsltE  ,"V "+OM.CmnOptn.sRsltNameE+" ") ;
    if(OM.CmnOptn.sRsltNameF != "") riAray->SetDispItem(csRsltF  ,"V "+OM.CmnOptn.sRsltNameF+" ") ;
    if(OM.CmnOptn.sRsltNameG != "") riAray->SetDispItem(csRsltG  ,"V "+OM.CmnOptn.sRsltNameG+" ") ;
    if(OM.CmnOptn.sRsltNameH != "") riAray->SetDispItem(csRsltH  ,"V "+OM.CmnOptn.sRsltNameH+" ") ;
    if(OM.CmnOptn.sRsltNameI != "") riAray->SetDispItem(csRsltI  ,"V "+OM.CmnOptn.sRsltNameI+" ") ;
    if(OM.CmnOptn.sRsltNameJ != "") riAray->SetDispItem(csRsltJ  ,"V "+OM.CmnOptn.sRsltNameJ+" ") ;
    if(OM.CmnOptn.sRsltNameK != "") riAray->SetDispItem(csRsltK  ,"V "+OM.CmnOptn.sRsltNameK+" ") ;
    if(OM.CmnOptn.sRsltNameL != "") riAray->SetDispItem(csRsltL  ,"V "+OM.CmnOptn.sRsltNameL+" ") ;
    riAray->SetDispItem(csToBuf  ,"ToBuf "        ) ;
    riAray->SetDispItem(csFromBuf,"FromBuf "      ) ;

}


bool TFormOperation::SetDispItem()
{

    DM.ARAY[riLDR].SetParent(pnLDR   ); DM.ARAY[riLDR].SetConfig(false , "pnLDR");
    DM.ARAY[riPRB].SetParent(pnPRB   ); DM.ARAY[riPRB].SetConfig(false , "pnPRB");
    DM.ARAY[riWR1].SetParent(pnInsp1 ); DM.ARAY[riWR1].SetConfig(false , "pnWR1");
    DM.ARAY[riVS1].SetParent(pnVisn1 ); DM.ARAY[riVS1].SetConfig(false , "pnVS1");
    DM.ARAY[riWR2].SetParent(pnInsp2 ); DM.ARAY[riWR2].SetConfig(false , "pnWR2");
    DM.ARAY[riVS2].SetParent(pnVisn2 ); DM.ARAY[riVS2].SetConfig(false , "pnVS2");
    DM.ARAY[riWR3].SetParent(pnInsp3 ); DM.ARAY[riWR3].SetConfig(false , "pnWR3");
    DM.ARAY[riVS3].SetParent(pnVisn3 ); DM.ARAY[riVS3].SetConfig(false , "pnVS3");
    DM.ARAY[riPSB].SetParent(pnPSB   ); DM.ARAY[riPSB].SetConfig(false , "pnPSB");
    DM.ARAY[riULD].SetParent(pnULD   ); DM.ARAY[riULD].SetConfig(false , "pnULD");

    for(int i=0; i<MAX_ARAY; i++ ) DM.ARAY[i].ClearDispItem(); //잠시제거.

    //보여주기용 버퍼.
    HED.DpAray1.SetParent(pn1B1      ); HED.DpAray1.SetConfig(false , "pn1B1     "); HED.DpAray1.ClearDispItem();
    HED.DpAray2.SetParent(pn2B2      ); HED.DpAray2.SetConfig(false , "pn2B2     "); HED.DpAray2.ClearDispItem();
    HED.DpAray3.SetParent(pn3B3      ); HED.DpAray3.SetConfig(false , "pn3B3     "); HED.DpAray3.ClearDispItem();

    DM.ARAY[riLDR ].SetDispItem(csEmpty  ,"Empty "     ) ;
    DM.ARAY[riLDR ].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riLDR ].SetDispItem(csNone   ,"None "      ) ;

    DM.ARAY[riPRB ].SetDispItem(csNone   ,"None "      ) ;
    DM.ARAY[riPRB ].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
    DM.ARAY[riPRB ].SetDispItem(csToBuf  ,"ToBuf "     ) ;
    DM.ARAY[riPRB ].SetDispItem(csFromBuf,"FromBuf "   ) ;

    DM.ARAY[riVS1 ].SetDispItem(csNone   ,"None "      ) ;  DM.ARAY[riVS2 ].SetDispItem(csNone   ,"None "  ) ; DM.ARAY[riVS3 ].SetDispItem(csNone   ,"None "  ) ;
    DM.ARAY[riVS1 ].SetDispItem(csUnkwn  ,"Unkwn "     ) ;  DM.ARAY[riVS2 ].SetDispItem(csUnkwn  ,"Unkwn " ) ; DM.ARAY[riVS3 ].SetDispItem(csUnkwn  ,"Unkwn " ) ;
    DM.ARAY[riVS1 ].SetDispItem(csWork   ,"Work "      ) ;  DM.ARAY[riVS2 ].SetDispItem(csWork   ,"Work "  ) ; DM.ARAY[riVS3 ].SetDispItem(csWork   ,"Work "  ) ;

    DM.ARAY[riULD ].SetDispItem(csEmpty  ,"Empty "     ) ;
    DM.ARAY[riULD ].SetDispItem(csWork   ,"Work "      ) ;
    DM.ARAY[riULD ].SetDispItem(csNone   ,"None "      ) ;

    SetDispItem(riWR1);
    SetDispItem(riWR2);
    SetDispItem(riWR3);
    SetDispItem(riPSB);

    SetDispItem(&HED.DpAray1);
    SetDispItem(&HED.DpAray2);
    SetDispItem(&HED.DpAray3);

    HED.DpAray1.PopupMenuEnable(false);
    HED.DpAray2.PopupMenuEnable(false);
    HED.DpAray3.PopupMenuEnable(false);

    String sTemp = "";

    LbResultChange();
}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

    Label17 -> Caption = DM.ARAY[riLDR].GetID();

    if(FormVisionRlstSet -> m_bChanged) {

    }

    LT.DispLotInfo(
        pnLotNo        ,
        pnLotJobFile   ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        pnLotWorkChip  ,
        NULL           );
//        pnLotFailChip  );

    LT.DispDayInfo(
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           ,
        NULL           );

    //EM_
    EM_DisplayList(lbErr);

    tsVs1 -> TabVisible = FM_GetLevel() >= lvMaster ;
    tsVs2 -> TabVisible = FM_GetLevel() >= lvMaster ;
    tsVs3 -> TabVisible = FM_GetLevel() >= lvMaster ;
//    tsVs4 -> TabVisible = FM_GetLevel() >= lvMaster ;

    tsV1Result -> TabVisible = FM_GetLevel() >= lvMaster ;
    tsV2Result -> TabVisible = FM_GetLevel() >= lvMaster ;
    tsV3Result -> TabVisible = FM_GetLevel() >= lvMaster ;

    lbPSB_Row     -> Caption = PSB.m_iReviewR ;
    lbPSB_Col     -> Caption = PSB.m_iReviewC ;
    lbPSB_Status  -> Caption = GetRsltName(DM.ARAY[riPSB].GetStat(PSB.m_iReviewR,PSB.m_iReviewC));


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
    if(SEQ._bRun && !bPreRun){
        pcVsR1 -> ActivePageIndex = 0 ; //pcVsR1 ->Refresh() ;
        pcVsR2 -> ActivePageIndex = 0 ; //pcVsR2 ->Refresh() ;
        pcVsR3 -> ActivePageIndex = 0 ; //pcVsR3 ->Refresh() ;
        pcVsR4 -> ActivePageIndex = 0 ; //pcVsR4 ->Refresh() ; tsVs4->Refresh() ; tsV4Result->Refresh() ;
//        if(!DM.ARAY[riLDB].CheckAllStat(csNone)) {pgLDR->TabIndex = 1 ; pgLDR->Refresh() ;TabSheet3->Refresh() ;}//TabSheet2->Refresh() ;}
//        else                                     {pgLDR->TabIndex = 0 ; pgLDR->Refresh() ;TabSheet2->Refresh() ;}
//        if(!DM.ARAY[riULB].CheckAllStat(csNone)) {pgULD->TabIndex = 1 ; pgULD->Refresh() ;TabSheet4->Refresh() ;}
//        else                                     {pgULD->TabIndex = 0 ; pgULD->Refresh() ;TabSheet1->Refresh() ;}
    }
    bPreRun = SEQ._bRun ;

    btWk1Input  -> Enabled  = !SEQ._bRun ;
    btWk2Input  -> Enabled  = !SEQ._bRun ;
    btWk3Input  -> Enabled  = !SEQ._bRun ;
    btWk4Input  -> Enabled  = !SEQ._bRun ;

    btVS1Reset  -> Enabled  = !SEQ._bRun ;
    btVS2Reset  -> Enabled  = !SEQ._bRun ;
    btVS3Reset  -> Enabled  = !SEQ._bRun ;
    btPSBReset  -> Enabled  = !SEQ._bRun ;

    btWK1Clear  -> Enabled  = !SEQ._bRun ;
    btWK2Clear  -> Enabled  = !SEQ._bRun ;
    btWK3Clear  -> Enabled  = !SEQ._bRun ;
    btPSBClear  -> Enabled  = !SEQ._bRun ;

    edWk1LotNo  -> Enabled = FM_GetLevel() >= lvEngineer ;
    edWk1Id     -> Enabled = FM_GetLevel() >= lvEngineer ;
    edWk2LotNo  -> Enabled = FM_GetLevel() >= lvEngineer ;
    edWk2Id     -> Enabled = FM_GetLevel() >= lvEngineer ;
    edWk3LotNo  -> Enabled = FM_GetLevel() >= lvEngineer ;
    edWk3Id     -> Enabled = FM_GetLevel() >= lvEngineer ;
    edWk4LotNo  -> Enabled = FM_GetLevel() >= lvEngineer ;
    edWk4Id     -> Enabled = FM_GetLevel() >= lvEngineer ;
    edPrbLotNo  -> Enabled = FM_GetLevel() >= lvEngineer ;
    edPrbId     -> Enabled = FM_GetLevel() >= lvEngineer ;

    //디스플레이
    pnLotMGZCnt1  -> Caption = AnsiString(LDR._iMgzCnt) + AnsiString("/") + AnsiString(OM.DevOptn.iLotEndMgzCnt) ;
    pnBadLimCnt   -> Caption = OM.CmnOptn.iCheckFailCnt ;

//    gbVsSkip   -> Enabled >= FM_GetLevel() == lvEngineer ;
//    gbSkipInfo -> Enabled >= FM_GetLevel() == lvEngineer ;
    cbVs1_1Skip -> Enabled = FM_GetLevel() >= lvEngineer ;
    cbVs1_2Skip -> Enabled = FM_GetLevel() >= lvEngineer ;
    cbVs2_1Skip -> Enabled = FM_GetLevel() >= lvEngineer ;
    cbVs2_2Skip -> Enabled = FM_GetLevel() >= lvEngineer ;
    cbVs3_1Skip -> Enabled = FM_GetLevel() >= lvEngineer ;
    cbVs3_2Skip -> Enabled = FM_GetLevel() >= lvEngineer ;
    //
    cbVsTrmSkip        -> Enabled = FM_GetLevel() >= lvEngineer ;
    cbCam1Skip         -> Enabled = FM_GetLevel() >= lvEngineer ;
    cbCam2Skip         -> Enabled = FM_GetLevel() >= lvEngineer ;

    cbTrm1Skip         -> Enabled = FM_GetLevel() >= lvEngineer ;
    cbTrmAllSkip       -> Enabled = FM_GetLevel() >= lvEngineer && !SEQ._bRun;
    cbDoorSkip         -> Enabled = FM_GetLevel() >= lvEngineer ;
    cbVisionAllSkip    -> Enabled = FM_GetLevel() >= lvEngineer ;
    cbIdleRun          -> Enabled = FM_GetLevel() >= lvEngineer ;
    //
    btWk1Input -> Visible = FM_GetLevel() == lvMaster ;
    btWk2Input -> Visible = FM_GetLevel() == lvMaster ;
    btWk3Input -> Visible = FM_GetLevel() == lvMaster ; 
    btWk4Input -> Visible = FM_GetLevel() == lvMaster ; 

//TODO::    if(RAL.bTrmOneSkip) {
//        RAL.bTrmOneSkip = false ;
//        UpdateCmmOptn(toTabl);
//    }

    lbVs1FailCnt  -> Caption = HED.DpAray1   .GetCntExist() - HED.DpAray1   .GetCntStat(csWork) ;
    lbVs2FailCnt  -> Caption = HED.DpAray2   .GetCntExist() - HED.DpAray2   .GetCntStat(csWork) ;
    lbVs3FailCnt  -> Caption = HED.DpAray3   .GetCntExist() - HED.DpAray3   .GetCntStat(csWork) ;
    lbTrmFailCnt  -> Caption = DM.ARAY[riPSB].GetCntExist() - DM.ARAY[riPSB].GetCntStat(csWork) ;

    pnLotFailChip -> Caption = LT.LotInfo.iFailChip - LT.LotInfo.iCntRslt1 ; //Empty etc .

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

//    pnLotFailChip -> Caption = LT.LotInfo.iFailChip - LT.LotInfo.iCntRslt1 ; //Empty etc .


    pnLotGoodChip -> Caption = iLotWrkChip   - iLotFailChip      ;
    pnTotalGoodCnt-> Caption = OM.iTotalChip - OM.iTotalFailChip ;
    //Map Update
    HED.DpAray1.UpdateAray();
    HED.DpAray2.UpdateAray();
    HED.DpAray3.UpdateAray();
    DM.UpdateAray();

    //Trim One Skip 550 이랑 똑같이 하긴 하는데 즞나 더럽다.dd
    if(RAL.bTrmOneSkip) {
        RAL.bTrmOneSkip = false ;
        UpdateCmmOptn(toTabl);
    }


    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormOperation::UpdateCmmOptn(bool bTable)
{
    if(bTable) { //Checked 속성 건드리면 OnClick 이벤트가 발생되나 그래서 이렇게 함. dd
        CbChecked(cbTrm1Skip       , OM.CmnOptn.bTrm1Skip       );
        CbChecked(cbTrm1Skip       , OM.CmnOptn.bTrm1Skip       );
        CbChecked(cbTrmAllSkip     , OM.CmnOptn.bTrmAllSkip     );
        CbChecked(cbLoadingStop    , OM.CmnOptn.bLoadingStop    );
        CbChecked(cbVs1_1Skip      , OM.CmnOptn.bVs1_1Skip      );
        CbChecked(cbVs2_1Skip      , OM.CmnOptn.bVs2_1Skip      );
        CbChecked(cbVs3_1Skip      , OM.CmnOptn.bVs3_1Skip      );
        CbChecked(cbVs1_2Skip      , OM.CmnOptn.bVs1_2Skip      );
        CbChecked(cbVs2_2Skip      , OM.CmnOptn.bVs2_2Skip      );
        CbChecked(cbVs3_2Skip      , OM.CmnOptn.bVs3_2Skip      );
        CbChecked(cbVsTrmSkip      , OM.CmnOptn.bVsTrmSkip      );
        CbChecked(cbCam1Skip       , OM.CmnOptn.bCam1Skip       );
        CbChecked(cbCam2Skip       , OM.CmnOptn.bCam2Skip       );
        CbChecked(cbDoorSkip       , OM.CmnOptn.bDoorSkip       );
        CbChecked(cbIdleRun        , OM.CmnOptn.bIdleRun        );
    }
    else {
        OM.CmnOptn.bTrm1Skip       = cbTrm1Skip       -> Checked  ;
        OM.CmnOptn.bTrmAllSkip     = cbTrmAllSkip     -> Checked  ;
        OM.CmnOptn.bLoadingStop    = cbLoadingStop    -> Checked  ;
        OM.CmnOptn.bVs1_1Skip      = cbVs1_1Skip      -> Checked  ;
        OM.CmnOptn.bVs2_1Skip      = cbVs2_1Skip      -> Checked  ;
        OM.CmnOptn.bVs3_1Skip      = cbVs3_1Skip      -> Checked  ;
        OM.CmnOptn.bVs1_2Skip      = cbVs1_2Skip      -> Checked  ;
        OM.CmnOptn.bVs2_2Skip      = cbVs2_2Skip      -> Checked  ;
        OM.CmnOptn.bVs3_2Skip      = cbVs3_2Skip      -> Checked  ;
        OM.CmnOptn.bVsTrmSkip      = cbVsTrmSkip      -> Checked  ;
        OM.CmnOptn.bCam1Skip       = cbCam1Skip       -> Checked  ;
        OM.CmnOptn.bCam2Skip       = cbCam2Skip       -> Checked  ;
        OM.CmnOptn.bDoorSkip       = cbDoorSkip       -> Checked  ;
        OM.CmnOptn.bIdleRun        = cbIdleRun        -> Checked  ;
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


void __fastcall TFormOperation::LbResultChange()
{
    lbR0 -> Caption = OM.CmnOptn.sRsltName0; pnColor0 -> Color = (TColor)OM.CmnOptn.iRsltColor0 ;
    lbR1 -> Caption = OM.CmnOptn.sRsltName1; pnColor1 -> Color = (TColor)OM.CmnOptn.iRsltColor1 ;
    lbR2 -> Caption = OM.CmnOptn.sRsltName2; pnColor2 -> Color = (TColor)OM.CmnOptn.iRsltColor2 ;
    lbR3 -> Caption = OM.CmnOptn.sRsltName3; pnColor3 -> Color = (TColor)OM.CmnOptn.iRsltColor3 ;
    lbR4 -> Caption = OM.CmnOptn.sRsltName4; pnColor4 -> Color = (TColor)OM.CmnOptn.iRsltColor4 ;
    lbR5 -> Caption = OM.CmnOptn.sRsltName5; pnColor5 -> Color = (TColor)OM.CmnOptn.iRsltColor5 ;
    lbR6 -> Caption = OM.CmnOptn.sRsltName6; pnColor6 -> Color = (TColor)OM.CmnOptn.iRsltColor6 ;
    lbR7 -> Caption = OM.CmnOptn.sRsltName7; pnColor7 -> Color = (TColor)OM.CmnOptn.iRsltColor7 ;
    lbR8 -> Caption = OM.CmnOptn.sRsltName8; pnColor8 -> Color = (TColor)OM.CmnOptn.iRsltColor8 ;
    lbR9 -> Caption = OM.CmnOptn.sRsltName9; pnColor9 -> Color = (TColor)OM.CmnOptn.iRsltColor9 ;
    lbRA -> Caption = OM.CmnOptn.sRsltNameA; pnColorA -> Color = (TColor)OM.CmnOptn.iRsltColorA ;
    lbRB -> Caption = OM.CmnOptn.sRsltNameB; pnColorB -> Color = (TColor)OM.CmnOptn.iRsltColorB ;
    lbRC -> Caption = OM.CmnOptn.sRsltNameC; pnColorC -> Color = (TColor)OM.CmnOptn.iRsltColorC ;
    lbRD -> Caption = OM.CmnOptn.sRsltNameD; pnColorD -> Color = (TColor)OM.CmnOptn.iRsltColorD ;
    lbRE -> Caption = OM.CmnOptn.sRsltNameE; pnColorE -> Color = (TColor)OM.CmnOptn.iRsltColorE ;
    lbRF -> Caption = OM.CmnOptn.sRsltNameF; pnColorF -> Color = (TColor)OM.CmnOptn.iRsltColorF ;
    lbRG -> Caption = OM.CmnOptn.sRsltNameG; pnColorG -> Color = (TColor)OM.CmnOptn.iRsltColorG ;
    lbRH -> Caption = OM.CmnOptn.sRsltNameH; pnColorH -> Color = (TColor)OM.CmnOptn.iRsltColorH ;
    lbRI -> Caption = OM.CmnOptn.sRsltNameI; pnColorI -> Color = (TColor)OM.CmnOptn.iRsltColorI ;
    lbRJ -> Caption = OM.CmnOptn.sRsltNameJ; pnColorJ -> Color = (TColor)OM.CmnOptn.iRsltColorJ ;
    lbRK -> Caption = OM.CmnOptn.sRsltNameK; pnColorK -> Color = (TColor)OM.CmnOptn.iRsltColorK ;
    lbRL -> Caption = OM.CmnOptn.sRsltNameL; pnColorL -> Color = (TColor)OM.CmnOptn.iRsltColorL ;

//    pnColor22 -> Color = (TColor)OM.CmnOptn.iRsltColor22 ; // lbR22 -> Caption = OM.CmnOptn.sRsltName22;
//    pnColor24 -> Color = (TColor)OM.CmnOptn.iRsltColor24 ; // lbR24 -> Caption = OM.CmnOptn.sRsltName24;
//    pnColor25 -> Color = (TColor)OM.CmnOptn.iRsltColor25 ; // lbR25 -> Caption = OM.CmnOptn.sRsltName25;
//    pnColor26 -> Color = (TColor)OM.CmnOptn.iRsltColor26 ; // lbR26 -> Caption = OM.CmnOptn.sRsltName26;

    clStat[csRslt0 ] = (TColor)OM.CmnOptn.iRsltColor0;
    clStat[csRslt1 ] = (TColor)OM.CmnOptn.iRsltColor1;
    clStat[csRslt2 ] = (TColor)OM.CmnOptn.iRsltColor2;
    clStat[csRslt3 ] = (TColor)OM.CmnOptn.iRsltColor3;
    clStat[csRslt4 ] = (TColor)OM.CmnOptn.iRsltColor4;
    clStat[csRslt5 ] = (TColor)OM.CmnOptn.iRsltColor5;
    clStat[csRslt6 ] = (TColor)OM.CmnOptn.iRsltColor6;
    clStat[csRslt7 ] = (TColor)OM.CmnOptn.iRsltColor7;
    clStat[csRslt8 ] = (TColor)OM.CmnOptn.iRsltColor8;
    clStat[csRslt9 ] = (TColor)OM.CmnOptn.iRsltColor9;
    clStat[csRsltA ] = (TColor)OM.CmnOptn.iRsltColorA;
    clStat[csRsltB ] = (TColor)OM.CmnOptn.iRsltColorB;
    clStat[csRsltC ] = (TColor)OM.CmnOptn.iRsltColorC;
    clStat[csRsltD ] = (TColor)OM.CmnOptn.iRsltColorD;
    clStat[csRsltE ] = (TColor)OM.CmnOptn.iRsltColorE;
    clStat[csRsltF ] = (TColor)OM.CmnOptn.iRsltColorF;
    clStat[csRsltG ] = (TColor)OM.CmnOptn.iRsltColorG;
    clStat[csRsltH ] = (TColor)OM.CmnOptn.iRsltColorH;
    clStat[csRsltI ] = (TColor)OM.CmnOptn.iRsltColorI;
    clStat[csRsltJ ] = (TColor)OM.CmnOptn.iRsltColorJ;
    clStat[csRsltK ] = (TColor)OM.CmnOptn.iRsltColorK;
    clStat[csRsltL ] = (TColor)OM.CmnOptn.iRsltColorL;

    clStat[csNone  ] = (TColor)OM.CmnOptn.iRsltColor22 ;//clInactiveBorder  ;//csWhite
    clStat[csEmpty ] = (TColor)OM.CmnOptn.iRsltColor24 ;//clSilver          ;//csSilver
    clStat[csUnkwn ] = (TColor)OM.CmnOptn.iRsltColor25 ;//clMoneyGreen      ;//csAqua
    clStat[csWork  ] = (TColor)OM.CmnOptn.iRsltColor26 ;//(TColor)0x00FF9595;//clHighlight       ;//csBlue
    clStat[csGood  ] = (TColor)OM.CmnOptn.iRsltColor27 ;//(TColor)0x00FF9595;//clHighlight       ;//csBlue

    clStat[csFail  ] = clPurple          ;//csFuchsia
    clStat[csEtc   ] = clNone            ;//etc*/
}

void __fastcall TFormOperation::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true ;

    OM.LoadCmnOptn();
    UpdateCmmOptn(toTabl);
    //LbResultChange();
}
//---------------------------------------------------------------------------

void __fastcall TFormOperation::cbTrm1SkipClick(TObject *Sender)
{                                                                             
    String sTemp;
                                                                              
    TCheckBox *pCbox = dynamic_cast <TCheckBox *> (Sender);                   
    sTemp = pCbox->Name;                                                      
    if(sTemp == "cbTrm1Skip") {                                               
        if( DM.ARAY[riPSB].GetCntExist() ) {                                  
            DM.ARAY[riPSB].SetStat(csWork) ;
            OM.CmnOptn.bTrm1Skip = false ;                                    
            UpdateCmmOptn(toTabl);
            return;
        }                                                                     
    }
    //너희 둘은 공존해선 안되기에 여기에 너희를 분리 시켜 놓구 시퀀스에서도   
    //처리를 해야 할듯 하네요. dd                                             
    UpdateCmmOptn(toBuff);
    OM.SaveCmnOptn();

}/*
//---------------------------------------------------------------------------
void __fastcall TFormOperation::cbTrmAllSkipClick(TObject *Sender)
{
    String sTemp;
    if(sTemp == "cbTrmAllSkip") {
       OM.CmnOptn.bTrmAllSkip = false ;
        UpdateCmmOptn(toTabl);
        return ;
        }
    UpdateCmmOptn(toBuff);
    OM.SaveCmnOptn();
}
*/
//---------------------------------------------------------------------------
void __fastcall TFormOperation::btWK1ClearClick(TObject *Sender)
{
    String sTemp;

    TButton *pBtn = dynamic_cast <TButton *> (Sender);
    sTemp = pBtn->Name;

         if(sTemp == "btWK1Clear") { DM.ARAY[riWR1].SetStat(csNone) ; DM.ARAY[riVS1].SetStat(csNone) ;}
    else if(sTemp == "btWK2Clear") { DM.ARAY[riWR2].SetStat(csNone) ; DM.ARAY[riVS2].SetStat(csNone) ;}
    else if(sTemp == "btWK3Clear") { DM.ARAY[riWR3].SetStat(csNone) ; DM.ARAY[riVS3].SetStat(csNone) ;}
    else if(sTemp == "btPRBClear") { DM.ARAY[riPRB].SetStat(csNone) ; }
    else if(sTemp == "btPSBClear") {
        if(GetLastStrip() && DM.ARAY[riPSB].GetCntExist() && LT.m_bLotOpen) {
            PSB.m_bLotEnded = true ;
            memcpy(&PSB.EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

            Trace("LotEnd",DM.ARAY[riPSB].GetLotNo().c_str());
            LT.LotInfo.dEndTime = Now();
            LT.WriteLotLog   ();
            LT.WriteLotDayLog();
            Trace("WriteLotLog",DM.ARAY[riPSB].GetLotNo().c_str());
            //LT.LotEnd();
        }
        DM.ARAY[riPSB].SetStat(csNone) ;
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
        if(!OM.CmnOptn.bVs1_1Skip)VC.SendReset(vsVisn1_1);
        if(!OM.CmnOptn.bVs1_2Skip)VC.SendReset(vsVisn1_2);
        if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
        if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].ChangeStat(csWork,csUnkwn) ; //TODO :: 아직 내가 헤드를 안봐서 삭제 못함.dd
    }
    else if(sTemp == "btVS2Reset") {
        if(!OM.CmnOptn.bVs2_1Skip)VC.SendReset(vsVisn3_1);
        if(!OM.CmnOptn.bVs2_2Skip)VC.SendReset(vsVisn3_2);
        if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
        if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].ChangeStat(csWork,csUnkwn) ;
    }
    else if(sTemp == "btVS3Reset") {
        if(!OM.CmnOptn.bVs3_1Skip)VC.SendReset(vsVisn3_1);
        if(!OM.CmnOptn.bVs3_2Skip)VC.SendReset(vsVisn3_2);
        if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat(csUnkwn) ;
        if(DM.ARAY[riWR3].GetCntExist())DM.ARAY[riWR3].ChangeStat(csWork,csUnkwn) ;
    }
    else if(sTemp == "btPSBReset") {
//        if(!OM.CmnOptn.bVsTrmSkip)VC.SendReset(vrVisn4);
        if(DM.ARAY[riPSB].GetCntExist())DM.ARAY[riPSB].SetStat(csUnkwn) ;
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
    if( !DM.ARAY[riLDR ].CheckAllStat(csNone) && (!IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2)) && !OM.MstOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Loader MGZ Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riLDR ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if( DM.ARAY[riPRB ].CheckExist(0,0) && !IO_GetX(xPRB_Pkg) && !OM.MstOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[PreBuffer Strip Disappear] \nDo you want to clear Strip?"     ) == mrYes ) { DM.ARAY[riPRB ].SetStat(csNone); SEQ._bBtnStart = true ;}
    }
    else if( DM.ARAY[riWR1 ].CheckExist(0,0) && !IO_GetX(xWR1_Pkg)) {
    }
    else if( DM.ARAY[riWR2 ].CheckExist(0,0) && !IO_GetX(xWR2_Pkg) && AT_Complete(aiWK2_SttnUpDn , ccBwd) && !OM.MstOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision2 Buffer Strip Disappear] \nDo you want to clear Strip?") == mrYes ) { DM.ARAY[riWR2 ].SetStat(csNone); DM.ARAY[riVS2 ].SetStat(csNone);SEQ._bBtnStart = true ;}
    }
    else if( DM.ARAY[riWR3 ].CheckExist(0,0) && !IO_GetX(xWR3_Pkg) && AT_Complete(aiWK3_SttnUpDn , ccBwd) && !OM.MstOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Vision3 Buffer Strip Disappear] \nDo you want to clear Strip?") == mrYes ) { DM.ARAY[riWR3 ].SetStat(csNone); DM.ARAY[riVS3 ].SetStat(csNone);SEQ._bBtnStart = true ;}
    }
    else if(!DM.ARAY[riULD ].CheckAllStat(csNone) && (!IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2)) && !OM.MstOptn.bDryRun) {
        if(FM_MsgYesNo("Err " , "[Unloader Mgz Disappear] \nDo you want to clear Mgz?") == mrYes ) { DM.ARAY[riULD ].SetStat(csNone); SEQ._bBtnStart = true ;}
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

    cbTrmAllSkip  -> OnClick = NULL ;
    cbVs1_1Skip   -> OnClick = NULL ;
    cbVs2_1Skip   -> OnClick = NULL ;
    cbVs3_1Skip   -> OnClick = NULL ;
    cbVs1_2Skip   -> OnClick = NULL ;
    cbVs2_2Skip   -> OnClick = NULL ;
    cbVs3_2Skip   -> OnClick = NULL ;
    cbVsTrmSkip   -> OnClick = NULL ;
    cbCam1Skip    -> OnClick = NULL ;
    cbCam2Skip    -> OnClick = NULL ;
    cbIdleRun     -> OnClick = NULL ;
    bool iRet = cbIdleRun -> Checked ;
    if(iRet) {
        OM.CmnOptn.bTrmAllSkip    = iRet  ;
        OM.CmnOptn.bVs1_1Skip     = iRet  ;
        OM.CmnOptn.bVs2_1Skip     = iRet  ;
        OM.CmnOptn.bVs3_1Skip     = iRet  ;
        OM.CmnOptn.bVs1_2Skip     = iRet  ;
        OM.CmnOptn.bVs2_2Skip     = iRet  ;
        OM.CmnOptn.bVs3_2Skip     = iRet  ;
        OM.CmnOptn.bVsTrmSkip     = iRet  ;
        OM.CmnOptn.bCam2Skip      = iRet  ;
        OM.CmnOptn.bCam1Skip      = iRet  ;
        OM.CmnOptn.bIdleRun       = iRet  ;

        cbTrmAllSkip ->Checked = iRet ; cbTrmAllSkip -> Enabled = !iRet ; //가라 나중에 바꾸자 껏다 키면 안될듯
        cbVs1_1Skip  ->Checked = iRet ; cbVs1_1Skip  -> Enabled = !iRet ;
        cbVs2_1Skip  ->Checked = iRet ; cbVs2_1Skip  -> Enabled = !iRet ;
        cbVs3_1Skip  ->Checked = iRet ; cbVs3_1Skip  -> Enabled = !iRet ;
        cbVs1_2Skip  ->Checked = iRet ; cbVs1_2Skip  -> Enabled = !iRet ;
        cbVs2_2Skip  ->Checked = iRet ; cbVs2_2Skip  -> Enabled = !iRet ;
        cbVs3_2Skip  ->Checked = iRet ; cbVs3_2Skip  -> Enabled = !iRet ;
        cbVsTrmSkip  ->Checked = iRet ; cbVsTrmSkip  -> Enabled = !iRet ;
        cbCam1Skip   ->Checked = iRet ; cbCam1Skip   -> Enabled = !iRet ;
        cbCam2Skip   ->Checked = iRet ; cbCam2Skip   -> Enabled = !iRet ;
    }
    else {
        OM.CmnOptn.bTrmAllSkip    = iRet ;
        OM.CmnOptn.bVs1_1Skip     = iRet ;
        OM.CmnOptn.bVs2_1Skip     = iRet ;
        OM.CmnOptn.bVs3_1Skip     = iRet ;
        OM.CmnOptn.bVs1_2Skip     = iRet ;
        OM.CmnOptn.bVs2_2Skip     = iRet ;
        OM.CmnOptn.bVs3_2Skip     = iRet ;
        OM.CmnOptn.bVsTrmSkip     = iRet ;
        OM.CmnOptn.bCam1Skip      = iRet ;
        OM.CmnOptn.bCam2Skip      = iRet ;
        OM.CmnOptn.bIdleRun       = iRet ;

        cbTrmAllSkip ->Checked = iRet ; cbTrmAllSkip -> Enabled = !iRet ;
        cbVs1_1Skip  ->Checked = iRet ; cbVs1_1Skip  -> Enabled = !iRet ;
        cbVs2_1Skip  ->Checked = iRet ; cbVs2_1Skip  -> Enabled = !iRet ;
        cbVs3_1Skip  ->Checked = iRet ; cbVs3_1Skip  -> Enabled = !iRet ;
        cbVs1_2Skip  ->Checked = iRet ; cbVs1_2Skip  -> Enabled = !iRet ;
        cbVs2_2Skip  ->Checked = iRet ; cbVs2_2Skip  -> Enabled = !iRet ;
        cbVs3_2Skip  ->Checked = iRet ; cbVs3_2Skip  -> Enabled = !iRet ;
        cbVsTrmSkip  ->Checked = iRet ; cbVsTrmSkip  -> Enabled = !iRet ;
        cbCam1Skip   ->Checked = iRet ; cbCam1Skip   -> Enabled = !iRet ;
        cbCam2Skip   ->Checked = iRet ; cbCam2Skip   -> Enabled = !iRet ;
//        cbIdleRun    ->Checked = iRet ; cbIdleRun    -> Enabled = iRet ;

    }

    cbTrmAllSkip  -> OnClick = cbTrm1SkipClick;
    cbVs1_1Skip   -> OnClick = cbTrm1SkipClick;
    cbVs2_1Skip   -> OnClick = cbTrm1SkipClick;
    cbVs3_1Skip   -> OnClick = cbTrm1SkipClick;
    cbVs1_2Skip   -> OnClick = cbTrm1SkipClick;
    cbVs2_2Skip   -> OnClick = cbTrm1SkipClick;
    cbVs3_2Skip   -> OnClick = cbTrm1SkipClick;
    cbVsTrmSkip   -> OnClick = cbTrm1SkipClick;
    cbCam1Skip    -> OnClick = cbTrm1SkipClick;
    cbCam2Skip    -> OnClick = cbTrm1SkipClick;
    cbIdleRun     -> OnClick = cbIdleRunClick ;

//    UpdateCmmOptn(toBuff);
    OM.SaveCmnOptn();

}
//---------------------------------------------------------------------------


void __fastcall TFormOperation::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------
bool TFormOperation::CbChecked(TCheckBox *cbBox)
{
    TNotifyEvent ChkBoxOnClick=cbBox->OnClick;
    cbBox->OnClick = NULL ;
    bool bChecked = false ;
    bChecked = cbBox->Checked ;

    cbBox->OnClick = ChkBoxOnClick ;
    return bChecked ;
}
bool TFormOperation::CbChecked(TCheckBox *cbBox,bool bChecked)
{
    TNotifyEvent ChkBoxOnClick=cbBox->OnClick;
    cbBox->OnClick = NULL ;
    cbBox->Checked = bChecked ;

    cbBox->OnClick = ChkBoxOnClick ;
    return true ;
}

void __fastcall TFormOperation::cbVisionAllSkipClick(TObject *Sender)
{
    bool iRet = CbChecked(cbVisionAllSkip) ;

    OM.CmnOptn.bVs1_1Skip     = iRet  ;
    OM.CmnOptn.bVs2_1Skip     = iRet  ;
    OM.CmnOptn.bVs3_1Skip     = iRet  ;
    OM.CmnOptn.bVs1_2Skip     = iRet  ;
    OM.CmnOptn.bVs2_2Skip     = iRet  ;
    OM.CmnOptn.bVs3_2Skip     = iRet  ;
    OM.CmnOptn.bVsTrmSkip     = iRet  ;
    OM.CmnOptn.bVisionAllSkip = iRet  ;

    CbChecked(cbVs1_1Skip,iRet) ; cbVs1_1Skip->Enabled = !iRet ;
    CbChecked(cbVs2_1Skip,iRet) ; cbVs2_1Skip->Enabled = !iRet ;
    CbChecked(cbVs3_1Skip,iRet) ; cbVs3_1Skip->Enabled = !iRet ;
    CbChecked(cbVs1_2Skip,iRet) ; cbVs1_2Skip->Enabled = !iRet ;
    CbChecked(cbVs2_2Skip,iRet) ; cbVs2_2Skip->Enabled = !iRet ;
    CbChecked(cbVs3_2Skip,iRet) ; cbVs3_2Skip->Enabled = !iRet ;
    CbChecked(cbVsTrmSkip,iRet) ; cbVsTrmSkip->Enabled = !iRet ;

//    UpdateCmmOptn(toBuff);
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------


void __fastcall TFormOperation::pnColor1Click(TObject *Sender)
{
    FormVisionRlstSet->ShowModal();

    SetDispItem(riWR1);
    SetDispItem(riWR2);
    SetDispItem(riWR3);
    SetDispItem(riPSB);

    SetDispItem(&HED.DpAray1);
    SetDispItem(&HED.DpAray2);
    SetDispItem(&HED.DpAray3);

    LbResultChange();

//    SetDispItem();
}
//---------------------------------------------------------------------------


void __fastcall TFormOperation::Button1Click(TObject *Sender)
{
    HED.GetMotrPos(miWRK_XVsn , piWRK_XVsnWorkEnd);
}
//---------------------------------------------------------------------------

