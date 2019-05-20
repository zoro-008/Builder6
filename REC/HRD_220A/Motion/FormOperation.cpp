//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "stdlib.h"


#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"

//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Index.h"
#include "Picker.h"
#include "Stage.h"
#include "Dispensor.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
//Rs232 Part.
//---------------------------------------------------------------------------
#include "Rs232Dispr.h"
#include "Rs232Keyence.h"
#include "Rs232LoadCl.h"
#include "Rs232Temp.h"
//---------------------------------------------------------------------------
#include "Pattern.h"
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


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)                       // 실행 시킬 때, 한 번만 실행하는 부분
    : TForm(Owner)
{
    DM.ARAY[riLDR_FTop].SetParent(pnLDR_FTop);  DM.ARAY[riLDR_FTop].ClearDispItem();  DM.ARAY[riLDR_FTop].SetCellSize(false); DM.ARAY[riLDR_FTop].SetName("LDR_FTop");
    DM.ARAY[riLDR_FBtm].SetParent(pnLDR_FBtm);  DM.ARAY[riLDR_FBtm].ClearDispItem();  DM.ARAY[riLDR_FBtm].SetCellSize(false); DM.ARAY[riLDR_FBtm].SetName("LDR_FBtm");
    DM.ARAY[riLDR_RTop].SetParent(pnLDR_RTop);  DM.ARAY[riLDR_RTop].ClearDispItem();  DM.ARAY[riLDR_RTop].SetCellSize(false); DM.ARAY[riLDR_RTop].SetName("LDR_RTop");
    DM.ARAY[riLDR_RBtm].SetParent(pnLDR_RBtm);  DM.ARAY[riLDR_RBtm].ClearDispItem();  DM.ARAY[riLDR_RBtm].SetCellSize(false); DM.ARAY[riLDR_RBtm].SetName("LDR_RBtm");
    DM.ARAY[riPRI_F   ].SetParent(pnPRI_F   );  DM.ARAY[riPRI_F   ].ClearDispItem();  DM.ARAY[riPRI_F   ].SetCellSize(false); DM.ARAY[riPRI_F   ].SetName("PRI_F   ");
    DM.ARAY[riPRI_R   ].SetParent(pnPRI_R   );  DM.ARAY[riPRI_R   ].ClearDispItem();  DM.ARAY[riPRI_R   ].SetCellSize(false); DM.ARAY[riPRI_R   ].SetName("PRI_R   ");
    DM.ARAY[riIDX_F   ].SetParent(pnIDX_F   );  DM.ARAY[riIDX_F   ].ClearDispItem();  DM.ARAY[riIDX_F   ].SetCellSize(false); DM.ARAY[riIDX_F   ].SetName("IDX_F   ");
    DM.ARAY[riIDX_R   ].SetParent(pnIDX_R   );  DM.ARAY[riIDX_R   ].ClearDispItem();  DM.ARAY[riIDX_R   ].SetCellSize(false); DM.ARAY[riIDX_R   ].SetName("IDX_R   ");
    DM.ARAY[riPSI_F   ].SetParent(pnPSI_F   );  DM.ARAY[riPSI_F   ].ClearDispItem();  DM.ARAY[riPSI_F   ].SetCellSize(false); DM.ARAY[riPSI_F   ].SetName("PSI_F   ");
    DM.ARAY[riPSI_R   ].SetParent(pnPSI_R   );  DM.ARAY[riPSI_R   ].ClearDispItem();  DM.ARAY[riPSI_R   ].SetCellSize(false); DM.ARAY[riPSI_R   ].SetName("PSI_R   ");
    DM.ARAY[riULD_FTop].SetParent(pnULD_FTop);  DM.ARAY[riULD_FTop].ClearDispItem();  DM.ARAY[riULD_FTop].SetCellSize(false); DM.ARAY[riULD_FTop].SetName("ULD_FTop");
    DM.ARAY[riULD_FBtm].SetParent(pnULD_FBtm);  DM.ARAY[riULD_FBtm].ClearDispItem();  DM.ARAY[riULD_FBtm].SetCellSize(false); DM.ARAY[riULD_FBtm].SetName("ULD_FBtm");
    DM.ARAY[riULD_RTop].SetParent(pnULD_RTop);  DM.ARAY[riULD_RTop].ClearDispItem();  DM.ARAY[riULD_RTop].SetCellSize(false); DM.ARAY[riULD_RTop].SetName("ULD_RTop");
    DM.ARAY[riULD_RBtm].SetParent(pnULD_RBtm);  DM.ARAY[riULD_RBtm].ClearDispItem();  DM.ARAY[riULD_RBtm].SetCellSize(false); DM.ARAY[riULD_RBtm].SetName("ULD_RBtm");
    DM.ARAY[riPCK     ].SetParent(pnPCK     );  DM.ARAY[riPCK     ].ClearDispItem();  DM.ARAY[riPCK     ].SetCellSize(false); DM.ARAY[riPCK     ].SetName("PCK     ");
    DM.ARAY[riALN     ].SetParent(pnALN     );  DM.ARAY[riALN     ].ClearDispItem();  DM.ARAY[riALN     ].SetCellSize(false); DM.ARAY[riALN     ].SetName("ALN     ");
    DM.ARAY[riSTG     ].SetParent(pnSTG     );  DM.ARAY[riSTG     ].ClearDispItem();  DM.ARAY[riSTG     ].SetCellSize(false); DM.ARAY[riSTG     ].SetName("STG     ");

    //3개는 DSP용 야매 ....
    DM.ARAY[riDSP     ].SetParent(pnDSP     );  DM.ARAY[riDSP     ].ClearDispItem();  DM.ARAY[riDSP     ].SetCellSize(false); DM.ARAY[riDSP     ].SetName("DSP     ");
    DM.ARAY[riVSN_R   ].SetParent(pnVSN_R   );  DM.ARAY[riVSN_R   ].ClearDispItem();  DM.ARAY[riVSN_R   ].SetCellSize(false); DM.ARAY[riVSN_R   ].SetName("riVSN_R ");
    DM.ARAY[riVSN_F   ].SetParent(pnVSN_F   );  DM.ARAY[riVSN_F   ].ClearDispItem();  DM.ARAY[riVSN_F   ].SetCellSize(false); DM.ARAY[riVSN_F   ].SetName("riVSN_F ");


        //Display Setting.
    DM.ARAY[riLDR_FTop].ClearDispItem();
    DM.ARAY[riLDR_FTop].SetDispItem(csNone   ,"NoMgz"   ) ;
    DM.ARAY[riLDR_FTop].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riLDR_FTop].SetDispItem(csUnkwn  ,"Unknown" ) ;

    DM.ARAY[riLDR_FBtm].ClearDispItem();
    DM.ARAY[riLDR_FBtm].SetDispItem(csNone   ,"NoMgz"   ) ;
    DM.ARAY[riLDR_FBtm].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riLDR_FBtm].SetDispItem(csUnkwn  ,"Unknown" ) ;

    DM.ARAY[riLDR_RTop].ClearDispItem();
    DM.ARAY[riLDR_RTop].SetDispItem(csNone   ,"NoMgz"   ) ;
    DM.ARAY[riLDR_RTop].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riLDR_RTop].SetDispItem(csUnkwn  ,"Unknown" ) ;

    DM.ARAY[riLDR_RBtm].ClearDispItem();
    DM.ARAY[riLDR_RBtm].SetDispItem(csNone   ,"NoMgz"   ) ;
    DM.ARAY[riLDR_RBtm].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riLDR_RBtm].SetDispItem(csUnkwn  ,"Unknown" ) ;

    DM.ARAY[riPRI_F   ].ClearDispItem();
    DM.ARAY[riPRI_F   ].SetDispItem(csNone   ,"NoBoat"  ) ;
    DM.ARAY[riPRI_F   ].SetDispItem(csEmpty  ,"Unknown" ) ;
    DM.ARAY[riPRI_F   ].SetDispItem(csUnkwn  ,"Empty"   ) ;

    DM.ARAY[riPRI_R   ].ClearDispItem();
    DM.ARAY[riPRI_R   ].SetDispItem(csNone   ,"NoBoat"  ) ;
    DM.ARAY[riPRI_R   ].SetDispItem(csEmpty  ,"Unknown" ) ;
    DM.ARAY[riPRI_R   ].SetDispItem(csUnkwn  ,"Empty"   ) ;

    DM.ARAY[riIDX_F   ].ClearDispItem();
    DM.ARAY[riIDX_F   ].SetDispItem(csNone   ,"NoBoat"  ) ;
    DM.ARAY[riIDX_F   ].SetDispItem(csUnkwn  ,"Unknown" ) ;
    DM.ARAY[riIDX_F   ].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riIDX_F   ].SetDispItem(csSkip   ,"Skip"    ) ;
    DM.ARAY[riIDX_F   ].SetDispItem(csWait   ,"Wait"    ) ;

    DM.ARAY[riIDX_R   ].ClearDispItem();
    DM.ARAY[riIDX_R   ].SetDispItem(csNone   ,"NoBoat"  ) ;
    DM.ARAY[riIDX_R   ].SetDispItem(csUnkwn  ,"Unknown" ) ;
    DM.ARAY[riIDX_R   ].SetDispItem(csWork   ,"WorkEnd" ) ;
    DM.ARAY[riIDX_R   ].SetDispItem(csVisnNG ,"VisnNg"  ) ;
    DM.ARAY[riIDX_R   ].SetDispItem(csSkip   ,"Skip"    ) ;
    DM.ARAY[riIDX_R   ].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riIDX_R   ].SetDispItem(csWait   ,"Wait"    ) ;

    DM.ARAY[riPSI_F   ].ClearDispItem();
    DM.ARAY[riPSI_F   ].SetDispItem(csNone   ,"NoBoat"  ) ;
    DM.ARAY[riPSI_F   ].SetDispItem(csUnkwn  ,"Unknown" ) ;
    DM.ARAY[riPSI_F   ].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riPSI_F   ].SetDispItem(csSkip   ,"Skip"    ) ;
    DM.ARAY[riPSI_F   ].SetDispItem(csWait   ,"Wait"    ) ;

    DM.ARAY[riPSI_R   ].ClearDispItem();
    DM.ARAY[riPSI_R   ].SetDispItem(csNone   ,"NoBoat"  ) ;
    DM.ARAY[riPSI_R   ].SetDispItem(csWork   ,"WorkEnd" ) ;
    DM.ARAY[riPSI_R   ].SetDispItem(csVisnNG ,"VisnNg"  ) ;
    DM.ARAY[riPSI_R   ].SetDispItem(csSkip   ,"Skip"    ) ;
    DM.ARAY[riPSI_R   ].SetDispItem(csEmpty  ,"Empty"   ) ;

    DM.ARAY[riPCK     ].ClearDispItem();
    DM.ARAY[riPCK     ].SetDispItem(csNone   ,"NoPkg"   ) ;
    DM.ARAY[riPCK     ].SetDispItem(csUnkwn  ,"Align"   ) ;
    DM.ARAY[riPCK     ].SetDispItem(csAttach ,"Attach"  ) ;
    DM.ARAY[riPCK     ].SetDispItem(csVisn   ,"Vision"  ) ;
    DM.ARAY[riPCK     ].SetDispItem(csSkip   ,"Skip"    ) ;

    DM.ARAY[riALN     ].ClearDispItem();
    DM.ARAY[riALN     ].SetDispItem(csNone   ,"NoPkg"   ) ;
    DM.ARAY[riALN     ].SetDispItem(csUnkwn  ,"Align"   ) ;
    DM.ARAY[riALN     ].SetDispItem(csSkip   ,"Skip"    ) ;

    DM.ARAY[riSTG     ].ClearDispItem();
    DM.ARAY[riSTG     ].SetDispItem(csNone    ,"NoPkg  ") ;
    DM.ARAY[riSTG     ].SetDispItem(csDisp    ,"Disp   ") ;
    DM.ARAY[riSTG     ].SetDispItem(csVisn    ,"VisnBf ") ;
    DM.ARAY[riSTG     ].SetDispItem(csAttach  ,"Attach ") ;
    DM.ARAY[riSTG     ].SetDispItem(csPush    ,"Push   ") ;
    DM.ARAY[riSTG     ].SetDispItem(csVisnAt  ,"VisnAt ") ;
    DM.ARAY[riSTG     ].SetDispItem(csWorkEnd ,"WorkEnd") ;
    DM.ARAY[riSTG     ].SetDispItem(csVisnNG  ,"VisnNg ") ;
    DM.ARAY[riSTG     ].SetDispItem(csSkip    ,"Skip   ") ;

    DM.ARAY[riULD_FTop].ClearDispItem();
    DM.ARAY[riULD_FTop].SetDispItem(csNone   ,"NoMgz"   ) ;
    DM.ARAY[riULD_FTop].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riULD_FTop].SetDispItem(csWork   ,"Work"    ) ;

    DM.ARAY[riULD_FBtm].ClearDispItem();
    DM.ARAY[riULD_FBtm].SetDispItem(csNone   ,"NoMgz"   ) ;
    DM.ARAY[riULD_FBtm].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riULD_FBtm].SetDispItem(csWork   ,"Work"    ) ;

    DM.ARAY[riULD_RTop].ClearDispItem();
    DM.ARAY[riULD_RTop].SetDispItem(csNone   ,"NoMgz"   ) ;
    DM.ARAY[riULD_RTop].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riULD_RTop].SetDispItem(csWork   ,"Work"    ) ;

    DM.ARAY[riULD_RBtm].ClearDispItem();
    DM.ARAY[riULD_RBtm].SetDispItem(csNone   ,"NoMgz"   ) ;
    DM.ARAY[riULD_RBtm].SetDispItem(csEmpty  ,"Empty"   ) ;
    DM.ARAY[riULD_RBtm].SetDispItem(csWork   ,"Work"    ) ;

    DM.ARAY[riDSP     ].ClearDispItem();
    DM.ARAY[riDSP     ].SetDispItem(csNone   ,"NoPkg"   ) ;

    DM.ARAY[riVSN_F   ].ClearDispItem();
    DM.ARAY[riVSN_F   ].SetDispItem(csNone   ,"NoPkg"   ) ;

    DM.ARAY[riVSN_R   ].ClearDispItem();
    DM.ARAY[riVSN_R   ].SetDispItem(csNone   ,"NoPkg"   ) ;



    DM.LoadMap();

    TUserINI   UserINI;
    AnsiString sPath  ;
    int iLang         ;
    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    UserINI.Load(sPath , "Member"     , "m_iLangSel"  , iLang);

    const bool bKorean = true;


    sgDayInfo -> DoubleBuffered = true ;
    sgLotInfo -> DoubleBuffered = true ;

    btLotEnd  -> Enabled = LOT.GetLotOpen();
    btStart   -> Enabled = LOT.GetLotOpen();
    btLotOpen -> Enabled =!LOT.GetLotOpen();

}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)                         // 오퍼레이션 화면을 한 번 보여줄 때마다, 이 함수를 탐
{
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

    btLight      -> Font -> Color = IO_GetY(yETC_Light     ) ? clLime : clBlack ;
    btMonitor    -> Font -> Color = IO_GetY(yETC_MonitorOff) ? clLime : clBlack ;
    btNiddleZCal -> Font -> Color = mcDSPCycleManNdlCheck== MM.GetManNo() ? clLime : clBlack ;

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
    //AnsiString Str;
    //static int iPreErrCnt  ;
    //int        iCrntErrCnt ;
    //for (int i = 0 ; i < EM_GetMaxErr() ; i++) {
    //    if (EM_GetErr(i)) iCrntErrCnt++;
    //}
    //if (iPreErrCnt != iCrntErrCnt ) {
    //    lbErr -> Clear();
    //    AnsiString Str;
    //    lbErr -> Items -> Clear();
    //    int iErrNo = EM_GetLastErr();
    //    for (int i = 0 ; i < EM_GetMaxErr() ; i++) {
    //        if (EM_GetErr(i)) {
    //            Str.sprintf("[ERR%03d]" , i) ;
    //            Str += EM_GetErrName(i) + AnsiString(" ") + EM_GetErrMsg(i);
    //            lbErr -> Items -> Add(Str);
    //            if (iErrNo == i) lbErr -> ItemIndex = lbErr -> Count - 1 ;
    //        }
    //    }
    //}
    //iPreErrCnt = iCrntErrCnt ;

    Trace("<@SETBUFF>","1,FormOperTimer 2");


    pnSTGVccm      -> Color   =  IO_GetX(xSTG_Vccm)      ? clLime : clRed ;
    pnPCKVccm      -> Color   =  IO_GetX(xPCK_Vccm)      ? clLime : clRed ;
    pnSTGVccm      -> Caption =  IO_GetX(xSTG_Vccm)      ? "STAGE VAC ON"   : "STAGE VAC OFF" ;
    pnPCKVccm      -> Caption =  IO_GetX(xPCK_Vccm)      ? "PICKER VAC ON"  : "PICKER VAC OFF" ;

    pnCycleMsg -> Color   = SEQ._bRun && SEQ._bFlick ? clLime : clSilver;

    String sCycleTimeSec ;
    int iCycleTimeMs ;
    if(!PCK.GetSeqStep()) {
        iCycleTimeMs = DSP.GetCycleTime(DSP.GetSeqStep());
        sCycleTimeSec = iCycleTimeMs / 1000.0 ;

        pnCycleMsg -> Caption = DSP.GetCycleName(DSP.GetSeqStep());// + " : " + sCycleTimeSec + "sec";
    }
    else {
        iCycleTimeMs = DSP.GetCycleTime(PCK.GetSeqStep());
        sCycleTimeSec = iCycleTimeMs / 1000.0 ;

        pnCycleMsg -> Caption = PCK.GetCycleName(PCK.GetSeqStep());// + " : " + sCycleTimeSec + "sec";
    }

    //김원식 부장... 요청
    //방법이 없어 존내 야매임.
    //기존에 데이터 맵 자체가 자제 유무에 따른 것인데...
    //동작할때 위치를 바꿔주게 요청 ;;--;
    if(PCK.GetSeqStep() == CPicker::scVisn){
        if(pnPCK -> Parent != gbVSN_F){
            pnPCK  -> Parent = gbVSN_F ;
            pnVSN_F-> Parent = gbPCK   ;
        }
    }
    else{
        if(pnPCK -> Parent != gbPCK){
            pnPCK   -> Parent = gbPCK   ;
            pnVSN_F -> Parent = gbVSN_F ;
        }
    }

    if(DSP.GetSeqStep() == CDispensor::scVisnBf || DSP.GetSeqStep() == CDispensor::scVisnAt){
        if(pnSTG -> Parent != gbVSN_R){
            pnSTG -> Parent   = gbVSN_R ;
            pnDSP -> Parent   = gbDSP ;
            pnVSN_R -> Parent = gbSTG ;
        }
    }
    else if(DSP.GetSeqStep() == CDispensor::scDisp){
        if(pnSTG -> Parent != gbDSP){
            pnSTG -> Parent   = gbDSP ;
            pnDSP -> Parent   = gbSTG ;
            pnVSN_R -> Parent = gbVSN_R ;
        }
    }
    else {
        if(pnSTG -> Parent != gbSTG){
            pnSTG -> Parent   = gbSTG ;
            pnDSP -> Parent   = gbDSP ;
            pnVSN_R -> Parent = gbVSN_R ;
        }
    }


    //인제 필요 없다. ARAY에서 알아서 함.
    for(int i = 0 ; i < MAX_ARAY ; i++) {
        DM.ARAY[i].UpdateAray();
    }

    bool bAllArayNone = DM.ARAY[riLDR_FTop].CheckAllStat(csNone) && DM.ARAY[riLDR_FBtm].CheckAllStat(csNone) &&
                        DM.ARAY[riLDR_RTop].CheckAllStat(csNone) && DM.ARAY[riLDR_RBtm].CheckAllStat(csNone) &&
                        DM.ARAY[riPRI_F   ].CheckAllStat(csNone) && DM.ARAY[riPRI_R   ].CheckAllStat(csNone) &&
                        DM.ARAY[riIDX_F   ].CheckAllStat(csNone) && DM.ARAY[riIDX_R   ].CheckAllStat(csNone) &&
                        DM.ARAY[riPSI_F   ].CheckAllStat(csNone) && DM.ARAY[riPSI_R   ].CheckAllStat(csNone) &&
                        DM.ARAY[riULD_FTop].CheckAllStat(csNone) && DM.ARAY[riULD_FBtm].CheckAllStat(csNone) &&
                        DM.ARAY[riULD_RTop].CheckAllStat(csNone) && DM.ARAY[riULD_RBtm].CheckAllStat(csNone) &&
                        DM.ARAY[riPCK     ].CheckAllStat(csNone) && DM.ARAY[riALN     ].CheckAllStat(csNone) &&
                        DM.ARAY[riDSP     ].CheckAllStat(csNone) && DM.ARAY[riSTG     ].CheckAllStat(csNone) ;

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
    lbStgThickness        -> Caption = sTemp.sprintf("%.3f",STG.GetPkgThickness   ());
    lbStgWorkEndThickness -> Caption = sTemp.sprintf("%.3f",STG.GetFinishThickness());
    lbPckThickness        -> Caption = sTemp.sprintf("%.3f",PCK.GetPkgThickness   ());
    lbPckEmptyToolHeight  -> Caption = sTemp.sprintf("%.3f",PCK.GetEmptyToolHeight());
    lbStgEpoxyThickness   -> Caption = sTemp.sprintf("%.3f",STG.GetEpoxyThickness ());
    lbDisprShowTime       -> Caption = String((int)DSP.GetDisprTime())+ String("ms");

    lbDispPressure -> Caption = OM.DevOptn.dDspPrsPres  ;
    lbPushWeight   -> Caption = OM.DevOptn.dAttachForce ;
    lbAttachGap    -> Caption = OM.DevOptn.dPlaceZOfs1 + OM.DevOptn.dPlaceZOfs2 ;


    if(PCK.GetEmptyToolHeight()==0) lbPckEmptyToolHeight -> Color = clRed ;
    else                            lbPckEmptyToolHeight -> Color = clScrollBar ;

    //lbFinishThickness    -> Caption = STG.GetFinishThickness() - STG.GetStageHeight() ;

    Label12              -> Caption = DSP.GetReptCnt();

    pnRecipe -> Caption = OM.GetCrntDev() ;


    //if(




    Trace("<@SETBUFF>","1,FormOperTimer End");




    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
    SEQ.Reset();//리셑 버튼 안누르고 그냥 창에서만 누룸.
    FrmLotOpen -> ShowModal() ;
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
    
    DM.ARAY[riLDR_FTop].SetStat(csNone );
    DM.ARAY[riLDR_FBtm].SetStat(csNone );
    DM.ARAY[riLDR_RTop].SetStat(csNone );
    DM.ARAY[riLDR_RBtm].SetStat(csNone );
    DM.ARAY[riPRI_F   ].SetStat(csNone );
    DM.ARAY[riPRI_R   ].SetStat(csNone );
    DM.ARAY[riIDX_F   ].SetStat(csNone );
    DM.ARAY[riIDX_R   ].SetStat(csNone );
    DM.ARAY[riPSI_F   ].SetStat(csNone );
    DM.ARAY[riPSI_R   ].SetStat(csNone );
    DM.ARAY[riULD_FTop].SetStat(csNone );
    DM.ARAY[riULD_FBtm].SetStat(csNone );
    DM.ARAY[riULD_RTop].SetStat(csNone );
    DM.ARAY[riULD_RBtm].SetStat(csNone );
    DM.ARAY[riPCK     ].SetStat(csNone );
    DM.ARAY[riALN     ].SetStat(csNone );
    DM.ARAY[riSTG     ].SetStat(csNone );

    DM.ARAY[riDSP     ].SetStat(csNone );
    DM.ARAY[riVSN_F   ].SetStat(csNone );
    DM.ARAY[riVSN_R   ].SetStat(csNone );

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
    if(IO_GetX(xSTG_Vccm)){
        FM_MsgOk("Error" , "작업 Stage에 자제를 제거하여 주십시오");
        return ;
    }
    if (FM_MsgYesNo("Confirm" , "전체 홈을 잡으시겠습니까?") != mrYes)  return;
    MM.SetManCycle(mcAllHome);
}
//---------------------------------------------------------------------------






void __fastcall TFrmOperation::GroupBox23Click(TObject *Sender)
{
    //PRI_F.GetData(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::GroupBox24Click(TObject *Sender)
{
//    DM.ShiftArrayData(riPRI_F , riIDX_F);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::GroupBox11Click(TObject *Sender)
{
//    STG.WriteTest();
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

void __fastcall TFrmOperation::btLightClick(TObject *Sender)
{
    IO_SetY(yETC_Light ,!IO_GetY(yETC_Light));
    //MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btNiddleZCalClick(TObject *Sender)
{
    int iTemp ;
    if(!OM.CmnOptn.bTempIgnrErrorChk) {
        iTemp = Rs232_TempCh.GetCrntTemp(3);//스테이지.
        if(iTemp > OM.DevOptn.iStageTemp + OM.CmnOptn.dTempErrorTol) {
            FM_MsgOk("ERROR",(String("Stage 온도가 설정 온도 보다 ")+ OM.CmnOptn.dTempErrorTol + "도 이상 높습니다.").c_str());
            return ;
        }

        if(iTemp < OM.DevOptn.iStageTemp - OM.CmnOptn.dTempErrorTol) {
            FM_MsgOk("ERROR",(String("Stage 온도가 설정 온도 보다 ")+ OM.CmnOptn.dTempErrorTol + "도 이상 낮습니다.").c_str());
            return ;
        }
    }

    if(FM_MsgYesNo("Confirm" ,"니들 Z축을 보정 하겠습니까?") != mrYes) return ;

    MM.SetManCycle(mcDSPCycleManNdlCheck);
    //MM.SetManCycle(mcDSPCycleManStdCal);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMonitorClick(TObject *Sender)
{
    IO_SetY(yETC_MonitorOff ,!IO_GetY(yETC_MonitorOff));
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::GroupBox1Click(TObject *Sender)
{
//PRI_R.GetData(true);
}
//---------------------------------------------------------------------------

#include "SVInterface.h"
void __fastcall TFrmOperation::pnCycleMsgClick(TObject *Sender)
{
//FM_MsgOk("Check","언로더에 보트없음을 확인 하시고 니들 터치 센서를 닦아주세요");
SV_SetPkgID(viStageAlignBf , "");
SV_SetPkgID(viStageAlignBf , DM.ARAY[riSTG].CHPS[0][0].GetID().c_str());
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btLdrUldMoveWaitClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm" ,"Loader/Unloader를 Wait 포지션으로 이동하시겠습니까?") != mrYes) return ;
    MM.SetManCycle(mcLdrUldMoveWait);        
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStgFlatnessClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm" ,"스테이지 평탄도 측정을 시작하시겠습니까?") != mrYes) return ;
    MM.SetManCycle(mcStgFlatness);
}
//---------------------------------------------------------------------------

