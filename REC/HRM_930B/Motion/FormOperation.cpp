//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "stdlib.h"
#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"
#include "FormHexaPot.h"
#include "FormDispCh.h"
#include "FormEpoxyChange.h"
//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "LoaderFront.h"
#include "LoaderRear.h"
#include "LeftTool.h"
#include "RightTool.h"
#include "Index.h"
#include "Stage.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
//Rs232 Part.
//---------------------------------------------------------------------------
#include "Rs232Dispr.h"
#include "Rs232Keyence.h"
#include "Rs232LoadCl.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#include "LotUnit.h"
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "UserINI.h"
#include "DispLogUnit.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)                       // 실행 시킬 때, 한 번만 실행하는 부분
    : TForm(Owner)
{

    DM.ARAY[riLDR_F].SetParent(pnLDR_F  );  DM.ARAY[riLDR_F].ClearDispItem();  DM.ARAY[riLDR_F].SetCellSize(false); DM.ARAY[riLDR_F].SetName("riLDR_F");
    DM.ARAY[riLDR_R].SetParent(pnLDR_R  );  DM.ARAY[riLDR_R].ClearDispItem();  DM.ARAY[riLDR_R].SetCellSize(false); DM.ARAY[riLDR_R].SetName("riLDR_R");
    DM.ARAY[riIDX  ].SetParent(pnIDX    );  DM.ARAY[riIDX  ].ClearDispItem();  DM.ARAY[riIDX  ].SetCellSize(false); DM.ARAY[riIDX  ].SetName("riIDX  ");
    DM.ARAY[riRTT  ].SetParent(pnRTT    );  DM.ARAY[riRTT  ].ClearDispItem();  DM.ARAY[riRTT  ].SetCellSize(false); DM.ARAY[riRTT  ].SetName("riRTT  ");
    DM.ARAY[riTRF  ].SetParent(pnTRF    );  DM.ARAY[riTRF  ].ClearDispItem();  DM.ARAY[riTRF  ].SetCellSize(false); DM.ARAY[riTRF  ].SetName("riTRF  ");
    DM.ARAY[riPRL  ].SetParent(pnPRL    );  DM.ARAY[riPRL  ].ClearDispItem();  DM.ARAY[riPRL  ].SetCellSize(false); DM.ARAY[riPRL  ].SetName("riPRL  ");
    DM.ARAY[riSTG  ].SetParent(pnSTG    );  DM.ARAY[riSTG  ].ClearDispItem();  DM.ARAY[riSTG  ].SetCellSize(false); DM.ARAY[riSTG  ].SetName("riSTG  ");
    DM.ARAY[riWKO  ].SetParent(pnWrkOut );  DM.ARAY[riWKO  ].ClearDispItem();  DM.ARAY[riWKO  ].SetCellSize(false); DM.ARAY[riWKO  ].SetName("riWKO  ");
    DM.ARAY[riPTL  ].SetParent(pnPstRail);  DM.ARAY[riPTL  ].ClearDispItem();  DM.ARAY[riPTL  ].SetCellSize(false); DM.ARAY[riPTL  ].SetName("riPTL  ");
    DM.ARAY[riULD  ].SetParent(pnULD    );  DM.ARAY[riULD  ].ClearDispItem();  DM.ARAY[riULD  ].SetCellSize(false); DM.ARAY[riULD  ].SetName("riULD  ");

    //3개는 DSP용 야매 ....
//    DM.ARAY[riDSP     ].SetParent(pnDSP     );  DM.ARAY[riDSP     ].ClearDispItem();  DM.ARAY[riDSP     ].SetCellSize(false); DM.ARAY[riDSP     ].SetName("DSP     ");
//    DM.ARAY[riVSN_R   ].SetParent(pnVSN_R   );  DM.ARAY[riVSN_R   ].ClearDispItem();  DM.ARAY[riVSN_R   ].SetCellSize(false); DM.ARAY[riVSN_R   ].SetName("riVSN_R ");
//    DM.ARAY[riVSN_F   ].SetParent(pnVSN_F   );  DM.ARAY[riVSN_F   ].ClearDispItem();  DM.ARAY[riVSN_F   ].SetCellSize(false); DM.ARAY[riVSN_F   ].SetName("riVSN_F ");
    /*
    m_iClStat[csNone   ] = clWhite           ;//csWhite           = 17 ,//스트립이나 카세트가 없는 상태.                                            >
    m_iClStat[csMask   ] = clSkyBlue         ;//csSkyBlue         = 18 ,//PKG가 없지만 채크를 해놓은 상태.                                          >  요거 세개가 PKG 없는거.
    m_iClStat[csEmpty  ] = clSilver          ;//csSilver          = 19 ,//카세트에 스트립이 없는 상태 혹은 더미패드에 토출자국이 없는경우. 프로브끝 >
    m_iClStat[csUnkwn  ] = clAqua            ;//csAqua            = 20 ,//작업전 있는지 없는지 모르는 경우.
    m_iClStat[csWork   ] = clBlue            ;//csBlue            = 21 ,// 작업 끝.
    m_iClStat[csWait   ] = clYellow          ;//csYellow          = 22 ,
    m_iClStat[csFail   ] = clFuchsia         ;//csFuchsia         = 23 ,
    m_iClStat[csEror   ] = clRed             ;//csRed             = 24 ,
    m_iClStat[csGood   ] = clLime            ;//csLime            = 25 ,
    m_iClStat[csEtc    ] = clNone            ;//etc               = 26 ,
    m_iClStat[csToBuf  ] = clBlack           ;//clBlack           = 27,
    m_iClStat[csFromBuf] = clBlack           ;//clBlack           = 28,
    m_iClStat[csDrop   ] = clGray            ;//clGray            = 29,

    */

    //Display Setting.
    DM.ARAY[riLDR_F].SetName("riLDR_F");
    DM.ARAY[riLDR_F].ClearDispItem();
    DM.ARAY[riLDR_F].SetDispItem(csNone   ,"None"    ) ; DM.ARAY[riLDR_F].SetDispColor(csNone  , clWhite  ) ;
    DM.ARAY[riLDR_F].SetDispItem(csEmpty  ,"Empty"   ) ; DM.ARAY[riLDR_F].SetDispColor(csEmpty , clSilver ) ;
    DM.ARAY[riLDR_F].SetDispItem(csUnkwn  ,"Unknown" ) ; DM.ARAY[riLDR_F].SetDispColor(csUnkwn , clAqua   ) ;
    DM.ARAY[riLDR_F].SetDispItem(csDetect ,"Detect"  ) ; DM.ARAY[riLDR_F].SetDispColor(csDetect, clCream  ) ;





    DM.ARAY[riLDR_R].SetName("riLDR_R");
    DM.ARAY[riLDR_R].ClearDispItem();
    DM.ARAY[riLDR_R].SetDispItem(csNone   ,"None"    ) ; DM.ARAY[riLDR_R].SetDispColor(csNone  , clWhite      ) ;
    DM.ARAY[riLDR_R].SetDispItem(csEmpty  ,"Empty"   ) ; DM.ARAY[riLDR_R].SetDispColor(csEmpty , clSilver     ) ;
    DM.ARAY[riLDR_R].SetDispItem(csUnkwn  ,"Unknown" ) ; DM.ARAY[riLDR_R].SetDispColor(csUnkwn , clAqua       ) ;
    DM.ARAY[riLDR_R].SetDispItem(csDetect ,"Detect"  ) ; DM.ARAY[riLDR_R].SetDispColor(csDetect, clCream      ) ;
    DM.ARAY[riLDR_R].SetDispItem(csMask   ,"Mask"    ) ; DM.ARAY[riLDR_R].SetDispColor(csMask  , clMoneyGreen ) ;
    DM.ARAY[riLDR_R].SetDispItem(csWork   ,"Work"    ) ; DM.ARAY[riLDR_R].SetDispColor(csWork  , clBlue       ) ;



    DM.ARAY[riIDX  ].SetName("riIDX");
    DM.ARAY[riIDX  ].ClearDispItem();
    DM.ARAY[riIDX  ].SetDispItem(csNone   ,"NoCMOS"  ) ; DM.ARAY[riIDX  ].SetDispColor(csNone  , clWhite  ) ;
    DM.ARAY[riIDX  ].SetDispItem(csEmpty  ,"Empty"   ) ; DM.ARAY[riIDX  ].SetDispColor(csEmpty , clSilver ) ;
    DM.ARAY[riIDX  ].SetDispItem(csUnkwn  ,"Unknown" ) ; DM.ARAY[riIDX  ].SetDispColor(csUnkwn , clAqua   ) ;


    


    DM.ARAY[riRTT  ].SetName("riRTT");
    DM.ARAY[riRTT  ].ClearDispItem();
    DM.ARAY[riRTT  ].SetDispItem(csNone      ,"NoCMOS"    ) ; DM.ARAY[riRTT  ].SetDispColor(csNone       , clWhite  ) ;
    DM.ARAY[riRTT  ].SetDispItem(csEmpty     ,"Empty"     ) ; DM.ARAY[riRTT  ].SetDispColor(csEmpty      , clSilver ) ;
    DM.ARAY[riRTT  ].SetDispItem(csCmsLOri   ,"CmsOri"    ) ; DM.ARAY[riRTT  ].SetDispColor(csCmsLOri    , clOlive  ) ;
    DM.ARAY[riRTT  ].SetDispItem(csAttach    ,"Attach"    ) ; DM.ARAY[riRTT  ].SetDispColor(csAttach     , clInactiveCaption  ) ;
    DM.ARAY[riRTT  ].SetDispItem(csWorkEnd   ,"WorkEnd"   ) ; DM.ARAY[riRTT  ].SetDispColor(csWorkEnd    , clRed    ) ;
    DM.ARAY[riRTT  ].SetDispItem(csCmsLHeight,"Height"    ) ; DM.ARAY[riRTT  ].SetDispColor(csCmsLHeight , clGreen  ) ;


    DM.ARAY[riTRF  ].SetName("riTRF");
    DM.ARAY[riTRF  ].ClearDispItem();
    DM.ARAY[riTRF  ].SetDispItem(csNone   ,"NoCMOS"  ) ; DM.ARAY[riTRF  ].SetDispColor(csNone   , clWhite  ) ;
    DM.ARAY[riTRF  ].SetDispItem(csAttach ,"Attach"  ) ; DM.ARAY[riTRF  ].SetDispColor(csAttach , clInactiveCaption  ) ;





    DM.ARAY[riPRL  ].SetName("riPRL");
    DM.ARAY[riPRL  ].ClearDispItem();
    DM.ARAY[riPRL  ].SetDispItem(csNone   ,"NoMidBlk") ; DM.ARAY[riPRL  ].SetDispColor(csNone  , clWhite  ) ;
    DM.ARAY[riPRL  ].SetDispItem(csUnkwn  ,"Unknown" ) ; DM.ARAY[riPRL  ].SetDispColor(csEmpty , clSilver ) ;
    DM.ARAY[riPRL  ].SetDispItem(csEmpty  ,"Empty"   ) ; DM.ARAY[riPRL  ].SetDispColor(csUnkwn , clAqua   ) ;





    DM.ARAY[riSTG  ].SetName("riSTG");
    DM.ARAY[riSTG  ].ClearDispItem();
    DM.ARAY[riSTG  ].SetDispItem(csNone         ,"NoPkg        ") ; DM.ARAY[riSTG  ].SetDispColor(csNone          , clWhite          ) ;
    //DM.ARAY[riSTG  ].SetDispItem(csWork         ,"Work         ") ; DM.ARAY[riSTG  ].SetDispColor(csWork          , clBlue           ) ;
    DM.ARAY[riSTG  ].SetDispItem(csMidLOri      ,"MidLOri      ") ; DM.ARAY[riSTG  ].SetDispColor(csMidLOri       , clYellow         ) ;
    DM.ARAY[riSTG  ].SetDispItem(csMidLAlign    ,"MidLAlign    ") ; DM.ARAY[riSTG  ].SetDispColor(csMidLAlign     , clMaroon         ) ;
    DM.ARAY[riSTG  ].SetDispItem(csMidRHeight   ,"MidRHeight   ") ; DM.ARAY[riSTG  ].SetDispColor(csMidRHeight    , clPurple         ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsLDisp1    ,"CmsLDisp1    ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsLDisp1     , clTeal           ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsLDisp2    ,"CmsLDisp2    ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsLDisp2     , 0x002EA0AD       ) ;
    //DM.ARAY[riSTG  ].SetDispItem(csCmsLOri      ,"CmsLOri      ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsLOri       , clOlive          ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsRNeed     ,"CmsRNeed     ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsRNeed      , clNavy           ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsLAlign    ,"CmsLAlign    ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsLAlign     , clHotLight       ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsRPreAlign ,"CmsRPreAlign ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsRPreAlign  , 0x00EA6FAC       ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsRHeight   ,"CmsRHeight   ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsRHeight    , clGreen          ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsRAlign    ,"CmsRAlign    ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsRAlign     , clFuchsia        ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsRZHeight  ,"CmsZHeight   ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsRZHeight   , clSilver         ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsREndAlign ,"CmsREndAlign ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsREndAlign  , clMoneyGreen     ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsREndHeight,"CmsREndHeight") ; DM.ARAY[riSTG  ].SetDispColor(csCmsREndHeight , clCream          ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsFix       ,"CmsFix       ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsFix        , clLime           ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsLTopUV1   ,"CmsLTopUV1   ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsLTopUV1    , clHighlight      ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsLTopUV2   ,"CmsLTopUV2   ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsLTopUV2    , 0x006F631E       ) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsRFixAlign ,"CmsRFixAlign ") ; DM.ARAY[riSTG  ].SetDispColor(csCmsRFixAlign  , clInactiveCaption) ;
    DM.ARAY[riSTG  ].SetDispItem(csCmsRFixHeight,"CmsRFixHeight") ; DM.ARAY[riSTG  ].SetDispColor(csCmsRFixHeight , clAqua           ) ;
    DM.ARAY[riSTG  ].SetDispItem(csMidREndHeight,"MidREndHeight") ; DM.ARAY[riSTG  ].SetDispColor(csMidREndHeight , clActiveCaption  ) ;
    DM.ARAY[riSTG  ].SetDispItem(csWorkEnd      ,"WorkEnd      ") ; DM.ARAY[riSTG  ].SetDispColor(csWorkEnd       , clRed            ) ;









    DM.ARAY[riWKO  ].SetName("riWRO");
    DM.ARAY[riWKO  ].ClearDispItem();
    DM.ARAY[riWKO  ].SetDispItem(csNone    ,"None"    ) ; DM.ARAY[riWKO  ].SetDispColor(csNone   , clWhite  ) ;
    DM.ARAY[riWKO  ].SetDispItem(csEmpty   ,"Empty"   ) ; DM.ARAY[riWKO  ].SetDispColor(csEmpty  , clSilver ) ;
    DM.ARAY[riWKO  ].SetDispItem(csWork    ,"Work"    ) ; DM.ARAY[riWKO  ].SetDispColor(csWork   , clBlue   ) ;





    DM.ARAY[riPTL  ].SetName("riPTL");
    DM.ARAY[riPTL  ].ClearDispItem();
    DM.ARAY[riPTL  ].SetDispItem(csNone     ,"None"   ) ; DM.ARAY[riPTL  ].SetDispColor(csNone   , clWhite  ) ;
    DM.ARAY[riPTL  ].SetDispItem(csWork     ,"Work"   ) ; DM.ARAY[riPTL  ].SetDispColor(csWork   , clBlue   ) ;




    DM.ARAY[riULD  ].SetName("riULD");
    DM.ARAY[riULD  ].ClearDispItem();
    DM.ARAY[riULD  ].SetDispItem(csNone   ,"NoMgz"   ) ; DM.ARAY[riULD  ].SetDispColor(csNone  , clWhite  ) ;
    DM.ARAY[riULD  ].SetDispItem(csEmpty  ,"Empty"   ) ; DM.ARAY[riULD  ].SetDispColor(csEmpty , clSilver ) ;
    DM.ARAY[riULD  ].SetDispItem(csWork   ,"Work"    ) ; DM.ARAY[riULD  ].SetDispColor(csWork  , clBlue   ) ;






    //DM.SaveMap();
    DM.LoadMap();

    TUserINI   UserINI;
    AnsiString sPath  ;
    int iLang         ;
    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    UserINI.Load(sPath , "Member"     , "m_iLangSel"  , iLang);

    sgDayInfo -> DoubleBuffered = true ;
    sgLotInfo -> DoubleBuffered = true ;

    btLotEnd  -> Enabled = LOT.GetLotOpen();
    btStart   -> Enabled = LOT.GetLotOpen();
    btLotOpen -> Enabled =!LOT.GetLotOpen();
    //LoadSave(true);


}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)                         // 오퍼레이션 화면을 한 번 보여줄 때마다, 이 함수를 탐
{
    if(OM.DevOptn.iFstDisp == 1 || OM.DevOptn.iSecDisp == 1){
        Rs232_DisprRr.GetDispData(OM.DevOptn.iDspChRr+1);
        //CDelayTimer TimeOut;
        //TimeOut.Clear();
        //while(!Rs232_DisprRr.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        //    ::Sleep(1);
        //    if(TimeOut.OnDelay(true , 3000)){
        //        break ;
        //    }
        //}
    }
    else {
        Rs232_DisprFt.GetDispData(OM.DevOptn.iDspChFt+1);
        //CDelayTimer TimeOut;
        //TimeOut.Clear();
        //while(!Rs232_DisprFt.GetMsgEnd()){ //메세지 다 주고 받을때까지 기다림.
        //    ::Sleep(1);
        //    if(TimeOut.OnDelay(true , 3000)){
        //        break ;
        //    }
        //}
    }


    SetLogHandle(FrmOperation -> Handle);

    tmUpdate->Enabled = true;

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
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    //LoadSave(false);
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;
    Trace("<@SETBUFF>","1,FormOperTimer Stt");

    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " ADMIN  "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }

    btLight      -> Font -> Color = IO_GetY(yETC_LightOnOff) ? clLime : clBlack ;
    btNiddleZCal -> Font -> Color = mcLTLCycleManNdlCheck== MM.GetManNo() ? clLime : clBlack ;
    btRAL_Cnvrs  -> Font -> Color = mcETCCycleCnvrs      == MM.GetManNo() ? clLime : clBlack ;

    /*
    switch (iLevel) {
        case lvOperator : if( iLev != lvOperator) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(OM.MstOptn.bDataAccEng);
                              }
                              iLev = lvEngineer ;
                          }
                          break ;
        case lvMaster   : if( iLev != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iLev = lvMaster ;
                          }
                          break ;
    }
    */



    static bool bPreLotOpen = false ;
    if(bPreLotOpen != LOT.GetLotOpen()){
        btLotEnd  -> Enabled = LOT.GetLotOpen();
        btStart   -> Enabled = LOT.GetLotOpen();
        btLotOpen -> Enabled =!LOT.GetLotOpen();
        bPreLotOpen = LOT.GetLotOpen();
    }

    Trace("<@SETBUFF>","1,FormOperTimer 1");
    SPC.LOT.DispLotInfo(sgLotInfo);
    SPC.DAY.DispDayInfo(sgDayInfo);

    //EM_Disp  TListBox * lbErr 일단 필요 없다고 해서 삭제.
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


    pnSTGVccm1      -> Color   =  IO_GetX(xSTG_StgVcum1    ) ? clLime : clRed ;
    pnSTGVccm2      -> Color   =  IO_GetX(xSTG_StgVcum2    ) ? clLime : clRed ;
    pnSTGVccm3      -> Color   =  IO_GetX(xSTG_StgVcum3    ) ? clLime : clRed ;
    pnRTLTrsprVccm1 -> Color   =  IO_GetX(xRTL_TransfrVcum1) ? clLime : clRed ;
    pnRTLTrsprVccm2 -> Color   =  IO_GetX(xRTL_TransfrVcum2) ? clLime : clRed ;
    pnIDXEjct       -> Color   =  IO_GetX(xIDX_IdxVcumSnr  ) ? clLime : clRed ;

    pnSTGVccm1      -> Caption =  IO_GetX(xSTG_StgVcum1    ) ? "STAGE VAC 1 ON"    : "STAGE VAC 1 OFF" ;
    pnSTGVccm2      -> Caption =  IO_GetX(xSTG_StgVcum2    ) ? "STAGE VAC 2 ON"    : "STAGE VAC 2 OFF" ;
    pnSTGVccm3      -> Caption =  IO_GetX(xSTG_StgVcum3    ) ? "STAGE VAC 3 ON"    : "STAGE VAC 3 OFF" ;
    pnRTLTrsprVccm1 -> Caption =  IO_GetX(xRTL_TransfrVcum1) ? "TRANSFER VAC 1 ON" : "TRANSFER VAC 1 OFF" ;
    pnRTLTrsprVccm2 -> Caption =  IO_GetX(xRTL_TransfrVcum2) ? "TRANSFER VAC 2 ON" : "TRANSFER VAC 2 OFF" ;
    pnIDXEjct       -> Caption =  IO_GetX(xIDX_IdxVcumSnr  ) ? "INDEX VAC ON"      : "INDEX VAC ON" ;

    pnCycleMsg -> Color   = SEQ._bRun && SEQ._bFlick ? clLime : clSilver;

    String sCycleTimeSec ;
    int iCycleTimeMs ;



    //인제 필요 없다. ARAY에서 알아서 함.
    for(int i = 0 ; i < MAX_ARAY ; i++) {
        DM.ARAY[i].UpdateAray();
    }

    bool bAllArayNone = DM.ARAY[riLDR_F].CheckAllStat(csNone) && DM.ARAY[riLDR_F].CheckAllStat(csNone) &&
                        DM.ARAY[riLDR_R].CheckAllStat(csNone) && DM.ARAY[riLDR_R].CheckAllStat(csNone) &&
                        DM.ARAY[riIDX  ].CheckAllStat(csNone) && DM.ARAY[riIDX  ].CheckAllStat(csNone) &&
                        DM.ARAY[riRTT  ].CheckAllStat(csNone) && DM.ARAY[riRTT  ].CheckAllStat(csNone) &&
                        DM.ARAY[riTRF  ].CheckAllStat(csNone) && DM.ARAY[riTRF  ].CheckAllStat(csNone) &&
                        DM.ARAY[riSTG  ].CheckAllStat(csNone) && DM.ARAY[riSTG  ].CheckAllStat(csNone) &&
                        DM.ARAY[riULD  ].CheckAllStat(csNone) && DM.ARAY[riULD  ].CheckAllStat(csNone) ;

    Trace("<@SETBUFF>","1,FormOperTimer 5");

        //Door Sensor.
    bool isAllCloseDoor = IO_GetX(xETC_DoorFt) &&
                          IO_GetX(xETC_DoorLt) &&
                          IO_GetX(xETC_DoorRt) &&
                          IO_GetX(xETC_DoorRr) ;
    if(FM_GetLevel()!=lvOperator && isAllCloseDoor && SEQ._bRun){
        //FM_SetLevel(lvOperator);
    }

    if(!MT_GetHomeEndAll()){
        btAllHome -> Font -> Color = SEQ._bFlick ? clBlack : clRed ;
    }
    else {
        btAllHome -> Font -> Color = clBlack  ;
    }

    Trace("<@SETBUFF>","1,FormOperTimer 6");

    String sTemp ;
    if(!SEQ._bRun) sTemp = "STOP" ;
         if(LTL.GetSeqStep() == CLeftTool::scMidSupply     ) sTemp = "Middle Block Supply     " ;
    else if(LTL.GetSeqStep() == CLeftTool::scMidToWork     ) sTemp = "Middle Block To Work    " ;
    else if(LTL.GetSeqStep() == CLeftTool::scClean         ) sTemp = "Niddle Clean            " ;
    else if(LTL.GetSeqStep() == CLeftTool::scMidLOri       ) sTemp = "Middle Block Orientation" ;
    else if(LTL.GetSeqStep() == CLeftTool::scMidLAlign     ) sTemp = "Middle Block Align      " ;
    else if(LTL.GetSeqStep() == CLeftTool::scCmsLTopUV     ) sTemp = "Cmos Top UV             " ;
    else if(LTL.GetSeqStep() == CLeftTool::scCmsLDisp      ) sTemp = "Cmos Dispense           " ;
    else if(LTL.GetSeqStep() == CLeftTool::scCmsLAlign     ) sTemp = "Cmos Pre Align          " ;
    else if(LTL.GetSeqStep() == CLeftTool::scCmsLOri       ) sTemp = "Cmos Orientation        " ;
    else if(LTL.GetSeqStep() == CLeftTool::scWorkOut       ) sTemp = "WorkOut                 " ;

    else if(RTL.GetSeqStep() == CRightTool::scMidRHeight   ) sTemp = "Middle Block HeightCheck" ;
    else if(RTL.GetSeqStep() == CRightTool::scTrsfPick     ) sTemp = "Transfer Pick           " ;
    else if(RTL.GetSeqStep() == CRightTool::scTrsfPlace    ) sTemp = "Transfer Place          " ;
    else if(RTL.GetSeqStep() == CRightTool::scCmsRHeight   ) sTemp = "Cmos Height Check       " ;
    else if(RTL.GetSeqStep() == CRightTool::scCmsRAlign    ) sTemp = "Cmos Align              " ;
    else if(RTL.GetSeqStep() == CRightTool::scCmsREndAlign ) sTemp = "Cmos Right End Algin    " ;
    else if(RTL.GetSeqStep() == CRightTool::scCmsRFixEnd   ) sTemp = "Cmos Right Fix End      " ;
    else if(RTL.GetSeqStep() == CRightTool::scMidREndHeight) sTemp = "End Height Check        " ;
    else if(RTL.GetSeqStep() == CRightTool::scWkoToPtl     ) sTemp = "WorkOut To Post Buffer  " ;
    else if(RTL.GetSeqStep() == CRightTool::scOut          ) sTemp = "Out To Unloader         " ;

    pnCycleMsg -> Caption = sTemp   ;

    lbWorkInfo1 -> Caption = OM.DevOptn.sEpoxyName ;
    //lbWorkInfo2 -> Caption = OM.DevOptn.dDspPrsPres;
    //lbWorkInfo3 -> Caption = OM.DevOptn.dDspVacPres;

    lbWorkInfo6 -> Caption = OM.DevOptn.iUVTopBrightness ;
    lbWorkInfo7 -> Caption = String(OM.DevOptn.iUVBtmBrightness1) +","+ OM.DevOptn.iUVBtmBrightness2 +","+ OM.DevOptn.iUVBtmBrightness3 +","+ OM.DevOptn.iUVBtmBrightness4 ;
    lbWorkInfo8 -> Caption = LTL.m_iDispCntF ;
    lbWorkInfo10-> Caption = LTL.m_iDispCntR ;

    //탑 UV 사용 시간
    lbWorkInfo4 -> Caption = (int)(Rs232UV.GetTopUvUseTime_ms()/(1000 * 60 * 60)) ; //시간 단위임.

    //Bottom UV 사용시간.
    lbWorkInfo9 -> Caption = (int)(Rs232UV.GetBtmUvUseTime_ms()/(1000 * 60 * 60)) ;



    if(OM.DevOptn.bUseFstDisp){
        int iAfterBtUVSec = (GetTime() - LTL.m_iLastUVSttTime) / 1000 ;
        int iMin        = iAfterBtUVSec / 60 ;
        int iSec        = iAfterBtUVSec % 60 ;
        lbWorkInfo5 -> Caption = String(iAfterBtUVSec) + "/" + OM.DevOptn.iFixDelay/ 1000 ;

        if(DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsRNeed      ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsLAlign     ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsRPreAlign  ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsRHeight    ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsRAlign     ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsLDisp2     ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsREndAlign  ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsREndHeight ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsRZHeight   ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsLTopUV1    ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsLTopUV2    ||
           DM.ARAY[riSTG].GetStat(LTL.m_iLastUVRow , LTL.m_iLastUVCol) ==csCmsFix        ){
           int iAfterBtUVSec = (GetTime() - LTL.m_iLastUVSttTime) / 1000 ;
           int iMin        = iAfterBtUVSec / 60 ;
           int iSec        = iAfterBtUVSec % 60 ;
           lbWorkInfo5 -> Caption = String(iAfterBtUVSec) + "/" + OM.DevOptn.iFixDelay/ 1000 ;
        }

        //int iAfterBtUVSec = (GetTime() - LTL.m_iLastUVSttTime) / 1000 ;
        //int iMin        = iAfterBtUVSec / 60 ;
        //int iSec        = iAfterBtUVSec % 60 ;
        //lbWorkInfo5 -> Caption = String(iAfterBtUVSec) + "/" + OM.DevOptn.iFixDelay/ 1000 ;

    }
    else {
        lbWorkInfo5 -> Caption = ""; 
    }
    

    if(OM.DevOptn.iFstDisp == 1 || OM.DevOptn.iFstDisp == 1){
        lbWorkInfo3   -> Caption = Rs232_DisprRr.GetDispData().dVacPres   ;
        lbWorkInfo2   -> Caption = Rs232_DisprRr.GetDispData().dPrsPres   ;
    }
    else if(OM.DevOptn.iFstDisp == 0 || OM.DevOptn.iSecDisp == 0){
        lbWorkInfo3   -> Caption = Rs232_DisprFt.GetDispData().dVacPres   ;
        lbWorkInfo2   -> Caption = Rs232_DisprFt.GetDispData().dPrsPres   ;
    }


    Trace("<@SETBUFF>","1,FormOperTimer End");

    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{

        SEQ.Reset();//리셑 버튼 안누르고 그냥 창에서만 누룸.
        //FrmEpoxyChange -> ShowModal() ;
        FrmLotOpen     -> ShowModal() ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
    SEQ._bBtnStart = true;


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
    if(FM_MsgYesNo("Confirm","Do you really want to LotEnd?") != mrYes) return ;

    /*
    bool LDRMgzDetect ;
    bool ULDMgzDetect ;

    LDRMgzDetect = IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2);
    ULDMgzDetect = IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2);

    if(LDRMgzDetect && ULDMgzDetect) MM.SetManCycle(mcETC_LDRULDPlace);
    else if( LDRMgzDetect && !ULDMgzDetect) MM.SetManCycle(mcLDR_Place);
    else if(!LDRMgzDetect &&  ULDMgzDetect) MM.SetManCycle(mcULD_Place);
    */

    //kyekye 얼로더 메거진 체인지 더 일찍 하기 랏바뀌었을때.


    LOT.LotEnd ();
    
    DM.ARAY[riLDR_F].SetStat(csNone );
    DM.ARAY[riLDR_R].SetStat(csNone );
    DM.ARAY[riIDX  ].SetStat(csNone );
    DM.ARAY[riRTT  ].SetStat(csNone );
    DM.ARAY[riTRF  ].SetStat(csNone );
    DM.ARAY[riSTG  ].SetStat(csNone );
    DM.ARAY[riWKO  ].SetStat(csNone );
    DM.ARAY[riPTL  ].SetStat(csNone );
    DM.ARAY[riULD  ].SetStat(csNone );

    
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPR1InputClick(TObject *Sender)
{
//    int iTag = ((TButton*)Sender) -> Tag ;
//
//    switch (iTag)
//    {
//        default :
//            break ;
//
//        case 1 : DM.ARAY[riPR1].SetStat(csUnkwn);
//                 if(DM.ARAY[riLDR].CheckAllStat(csNone) || DM.ARAY[riLDR].CheckAllStat(csUnkwn)) DM.ARAY[riPR1].SetLotNo(PR1.GetWorkedLotNo()     ) ;
//                 else                                                                            DM.ARAY[riPR1].SetLotNo(DM.ARAY[riLDR].GetLotNo()) ;
//                 break;
//
//        case 2 : DM.ARAY[riPR1].SetStat(csUnkwn);
//                 if(DM.ARAY[riLDR].CheckAllStat(csNone) || DM.ARAY[riLDR].CheckAllStat(csUnkwn)) DM.ARAY[riPR1].SetLotNo(PR1.GetWorkedLotNo()     ) ;
//                 else                                                                            DM.ARAY[riPR1].SetLotNo(DM.ARAY[riLDR].GetLotNo()) ;
//                 break;
//    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnDayRunTimeDblClick(TObject *Sender)
{
    if(FM_GetLevel() != lvMaster) return ;

    if(FM_MsgYesNo("Confirm","Clear Day Info") != mrYes) return ;

    SPC.DAY.ClearData() ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btAllHomeClick(TObject *Sender)
{
//    if(IO_GetX(xSTG_Vccm)){
//        FM_MsgOk("Error" , "작업 Stage에 자제를 제거하여 주십시오");
//        return ;
//    }
    if (FM_MsgYesNo("Confirm" , "전체 홈을 잡으시겠습니까?") != mrYes)  return;
    MM.SetManCycle(mcAllHome);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnDataViewClick(TObject *Sender)
{
    if(FM_GetLevel() != lvMaster) return ;
    pcDataView -> ActivePageIndex = pcDataView -> ActivePageIndex == 0 ? 1 : 0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnPRI_R_Click(TObject *Sender)
{
    if(FM_MsgYesNo("Conform" ,"데이터를 지우시겠습니까?") != mrYes) return ;

    int iAray = ((TPanel *)Sender) -> Tag ;
    DM.ARAY[iAray].SetStat(csNone);
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btNiddleZCalClick(TObject *Sender)
{
    //int iManNo = ((TSpeedButton*)Sender) -> Tag ;
    if(FM_MsgYesNo("Confirm" ,"니들 Z축을 보정 하겠습니까?") != mrYes) return ;
    MM.SetManCycle(mcLTLCycleManNdlCheck);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMonitorClick(TObject *Sender)
{
//    IO_SetY(yETC_MonitorOff ,!IO_GetY(yETC_MonitorOff));
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btRAL_CnvrsClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm" ,"레일을 컨버젼 하시겠습니까?")!= mrYes) return ;
    MM.SetManCycle(mcETCCycleCnvrs);
}
//---------------------------------------------------------------------------




void __fastcall TFrmOperation::btLTL_DispCntClearFClick(TObject *Sender)
{
    int iSelBtn = ((TButton *)Sender) -> Tag ;
    if(iSelBtn == 0) {//프론트
        if(FM_MsgYesNo("Confirm" ,"프론트 디스펜서 토출카운트를 초기화 하겠습니까")!= mrYes) return ;
        LTL.m_iDispCntF = 0 ;
    }
    else if(iSelBtn == 1) {//리어
        if(FM_MsgYesNo("Confirm" ,"리어 디스펜서 토출카운트를 초기화 하겠습니까")!= mrYes) return ;
        LTL.m_iDispCntR = 0 ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLightClick(TObject *Sender)
{
    IO_SetY(yETC_LightOnOff , !IO_GetY(yETC_LightOnOff));        
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btHexaPotClick(TObject *Sender)
{
    FrmHexaPot -> ShowModal() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::OnCopyDataEvent(TMessage& Msg)
{
    COPYDATASTRUCT  *pCds;
    pCds = (PCOPYDATASTRUCT)Msg.LParam;

    if( pCds == NULL) return;

    AnsiString str   = (char*)pCds->lpData;

    while( lbMessage->Items->Count  > 1000 ) {
       lbMessage->Items->Delete(0);
    }

    lbMessage -> AddItem(str,NULL);
    SendMessage( lbMessage->Handle, WM_VSCROLL, SB_BOTTOM, 0 );

}



void __fastcall TFrmOperation::pnSTGVccm1Click(TObject *Sender)
{
    String sTemp = ((TPanel *)Sender)-> Caption ;

   // SendLog(sTemp);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btFtDispChSettingClick(TObject *Sender)
{
    FrmDispCh -> Show(&Rs232_DisprFt) ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btRrDispChSettingClick(TObject *Sender)
{
    FrmDispCh -> Show(&Rs232_DisprRr) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btSTGChangeClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm" ,"스테이지를 교체 하시겠습니까?")!= mrYes) return ;
    MM.SetManCycle(mcCycleSTGChange);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btEpoxyLotChangeClick(TObject *Sender)
{
    FrmEpoxyChange -> ShowModal();
}
//---------------------------------------------------------------------------




//void TFrmOperation::LoadSave(bool _bToTable)
//{
//}


//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button1Click(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm" , "Top UV 사용시간을 리셋하시겠습니까?")!=mrYes) return;

    Rs232UV.ClearTopUvUseTime();

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button2Click(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm" , "Bottom UV 사용시간을 리셋하시겠습니까?")!=mrYes) return;
    Rs232UV.ClearBtmUvUseTime();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::Button3Click(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm" , "Fix Delay Time을 리셋하시겠습니까?")!=mrYes) return;

    Trace(" LTL.m_iLastUVSttTime bf" , String(LTL.m_iLastUVSttTime).c_str());
    LTL.m_iLastUVSttTime = GetTime() ;
    Trace(" LTL.m_iLastUVSttTime at" , String(LTL.m_iLastUVSttTime).c_str());

}
//---------------------------------------------------------------------------
                                                   
