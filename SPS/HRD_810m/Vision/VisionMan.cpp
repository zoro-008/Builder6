//---------------------------------------------------------------------------


#pragma hdrstop

#include "VisionMan.h"


#include "SVInterface.h"
#include "UserFile.h"
#include "SVMaths.h"
#include "VOptnMan.h"
#include "SLogUnit.h"
#include "UserFile.h"

#include "FormVisnMain.h"
#include "FormAlgoPara.h"
#include "FormVisnPara.h"

//검사시에 Rect검사를 시행할 것인지..  true
//아니면 그냥 OCV좌표만 검사할 것인지. false
#define USE_RECT_INSP false



//---------------------------------------------------------------------------

#pragma package(smart_init)
CVisionMan VSM ;
AnsiString g_sJobFileName ;
AnsiString g_sJobFilePath ;
bool       g_bSettingMode = false;

bool       g_bNeedWfrCenter    = false ;
bool       g_bNeedWfrOcv1Train = false ;
bool       g_bNeedWfrOcv2Train = false ;
bool       g_bNeedWfrSave      = false ;
bool       g_bNeedWfrOcv1Save  = false ;
bool       g_bNeedWfrOcv2Save  = false ;

bool       g_bNeedWfoCenter    = false ;
bool       g_bNeedWfoOcv1Train = false ;
bool       g_bNeedWfoOcv2Train = false ;
bool       g_bNeedWfoSave      = false ;
bool       g_bNeedWfoOcv1Save  = false ;
bool       g_bNeedWfoOcv2Save  = false ;

bool       g_bNeedWfpSave      = false ;

bool       g_bNeedCalSave      = false ;


#include "FormVisnMain.h"

void __fastcall CVisionMan::tmTrackerMovedTimer(TObject *Sender)
{
    if(!g_bSettingMode) return ;
    tmTrackerMoved -> Enabled = false ;

    //웨이퍼 칩==============================================================
    static bool bPreWfrRctTrackerMoved = false ;
    bool bWfrRctTrackerMoved = ((CRct *)VISN[viWfr]->ProcPkg.m_pProc[iiWfrRct])->Tracker->GetMoved() ;
    if(bWfrRctTrackerMoved && !bPreWfrRctTrackerMoved) { //렉트 트렉커 움직였으면
        g_bNeedWfrCenter    = true ; //센터 다시 잡아야 한다.
        g_bNeedWfrSave      = true ; //센터 다시 잡아야 한다.
        g_bNeedWfrOcv1Train = true ; //센터 다시 잡아야 한다.
        g_bNeedWfrOcv2Train = true ; //센터 다시 잡아야 한다.
        g_bNeedWfrOcv1Save  = true ; //센터 다시 잡아야 한다.
        g_bNeedWfrOcv2Save  = true ; //센터 다시 잡아야 한다.
    }
    bPreWfrRctTrackerMoved = bWfrRctTrackerMoved ;

    static bool bPreWfrOcv1TrackerMoved = false ;
    bool bWfrOcv1TrackerMoved = ((COcv *)VISN[viWfr]->ProcPkg.m_pProc[iiWfrOcv1])->Tracker->GetMoved() ;
    if(bWfrOcv1TrackerMoved && !bPreWfrOcv1TrackerMoved) { //렉트 트렉커 움직였으면
        g_bNeedWfrOcv1Train = true ; //센터 다시 잡아야 한다.
        g_bNeedWfrOcv2Train = true ; //센터 다시 잡아야 한다.
        g_bNeedWfrOcv1Save  = true ; //센터 다시 잡아야 한다.
        g_bNeedWfrOcv2Save  = true ; //센터 다시 잡아야 한다.
    }
    bPreWfrOcv1TrackerMoved = bWfrOcv1TrackerMoved ;

    static bool bPreWfrOcv2TrackerMoved = false ;
    bool bWfrOcv2TrackerMoved = ((COcv *)VISN[viWfr]->ProcPkg.m_pProc[iiWfrOcv2])->Tracker->GetMoved() ;
    if(bWfrOcv2TrackerMoved && !bPreWfrOcv2TrackerMoved) { //렉트 트렉커 움직였으면
        g_bNeedWfrOcv2Train = true ; //센터 다시 잡아야 한다.
        g_bNeedWfrOcv2Save  = true ; //센터 다시 잡아야 한다.
    }
    bPreWfrOcv2TrackerMoved = bWfrOcv2TrackerMoved ;




    //웨이퍼 기준==============================================================
    static bool bPreWfoRctTrackerMoved = false ;
    bool bWfoRctTrackerMoved = ((CRct *)VISN[viWfo]->ProcPkg.m_pProc[iiWfoRct])->Tracker->GetMoved() ;
    if(bWfoRctTrackerMoved && !bPreWfoRctTrackerMoved) { //렉트 트렉커 움직였으면
        g_bNeedWfoCenter    = true ; //센터 다시 잡아야 한다.
        g_bNeedWfoSave      = true ; //센터 다시 잡아야 한다.
        g_bNeedWfoOcv1Train = true ; //센터 다시 잡아야 한다.
        g_bNeedWfoOcv2Train = true ; //센터 다시 잡아야 한다.
        g_bNeedWfoOcv1Save  = true ; //센터 다시 잡아야 한다.
        g_bNeedWfoOcv2Save  = true ; //센터 다시 잡아야 한다.
    }
    bPreWfoRctTrackerMoved = bWfoRctTrackerMoved ;

    static bool bPreWfoOcv1TrackerMoved = false ;
    bool bWfoOcv1TrackerMoved = ((COcv *)VISN[viWfo]->ProcPkg.m_pProc[iiWfoOcv1])->Tracker->GetMoved() ;
    if(bWfoOcv1TrackerMoved && !bPreWfoOcv1TrackerMoved) { //렉트 트렉커 움직였으면
        g_bNeedWfoOcv1Train = true ; //센터 다시 잡아야 한다.
        g_bNeedWfoOcv2Train = true ; //센터 다시 잡아야 한다.
        g_bNeedWfoOcv1Save  = true ; //센터 다시 잡아야 한다.
        g_bNeedWfoOcv2Save  = true ; //센터 다시 잡아야 한다.
    }
    bPreWfoOcv1TrackerMoved = bWfoOcv1TrackerMoved ;

    static bool bPreWfoOcv2TrackerMoved = false ;
    bool bWfoOcv2TrackerMoved = ((COcv *)VISN[viWfo]->ProcPkg.m_pProc[iiWfoOcv2])->Tracker->GetMoved() ;
    if(bWfoOcv2TrackerMoved && !bPreWfoOcv2TrackerMoved) { //렉트 트렉커 움직였으면
        g_bNeedWfoOcv2Train = true ; //센터 다시 잡아야 한다.
        g_bNeedWfoOcv2Save  = true ; //센터 다시 잡아야 한다.
    }
    bPreWfoOcv2TrackerMoved = bWfoOcv2TrackerMoved ;


    //웨이퍼 펜==============================================================
    // TSD 트렉커가 더블 포인터이다 ;;
    static bool bPreWfpRctTrackerMoved = false ;
    bool bWfpRctTrackerMoved = ((CTsd *)VISN[viWfp]->ProcPkg.m_pProc[iiWfpTsd])->Tracker[0]->GetMoved() ;
    if(bWfpRctTrackerMoved && !bPreWfpRctTrackerMoved) { //렉트 트렉커 움직였으면
        g_bNeedWfpSave      = true ; //센터 다시 잡아야 한다.
    }
    bPreWfpRctTrackerMoved = bWfpRctTrackerMoved ;

    //켈리브레이션==============================================================
    static bool bPreCalRctTrackerMoved = false ;
    bool bCalRctTrackerMoved = ((CCal *)VISN[viCal]->ProcPkg.m_pProc[iiCalCal])->Tracker->GetMoved() ;
    if(bCalRctTrackerMoved && !bPreCalRctTrackerMoved) { //렉트 트렉커 움직였으면
        g_bNeedCalSave      = true ; //센터 다시 잡아야 한다.
    }
    bPreCalRctTrackerMoved = bCalRctTrackerMoved ;



    tmTrackerMoved -> Enabled = true ;
}

bool CVisionMan::SetRectCenter()
{



}



CVisionMan::CVisionMan()
{
}



CVisionMan::~CVisionMan()
{
    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;
}


bool CVisionMan::InitVisn(AnsiString _sDevName)
{
    LoadSVL("pSVL");



    Cam  [ciWfr] = new CImi() ;

    Cam  [ciEnd] = new CImi() ;



    Light[liWfr] = new CDkm() ;

    VISN[viWfr] = new CVision(); VISN[viWfr] -> Init("WAFER CHIP" );
    VISN[viWfo] = new CVision(); VISN[viWfo] -> Init("WAFER ORI"  );
    VISN[viWfp] = new CVision(); VISN[viWfp] -> Init("WAFER PEN"  );
    VISN[viCal] = new CVision(); VISN[viCal] -> Init("CALIBRATION");

    m_iInspectionId = 0 ;

    /*Wafer Vision*/
    EN_PROC_TYPE iPtWfr     [MAX_WFR_INSP];
    int          iIntParaWfr[MAX_WFR_INSP];
    iPtWfr[iiWfrRct ] = ptRct ; iIntParaWfr[iiWfrRct ] = 0 ;
    iPtWfr[iiWfrOcv1] = ptOcv ; iIntParaWfr[iiWfrOcv1] = 0 ;
    iPtWfr[iiWfrOcv2] = ptOcv ; iIntParaWfr[iiWfrOcv2] = 0 ;
    VISN[viWfr  ]->ProcPkg.SetProcList(MAX_WFR_INSP , iPtWfr , iIntParaWfr) ;
    //((CRct*)VISN[viWfr  ]->ProcPkg.m_pProc[iiWfrRct])->Tracker -> Band = 20 ;
    ((CRct*)VISN[viWfr  ]->ProcPkg.m_pProc[iiWfrRct])->Tracker -> BandMax    = 20   ;
    ((CRct*)VISN[viWfr  ]->ProcPkg.m_pProc[iiWfrRct])->Tracker -> BandEnable = true ;


    ((COcv*)VISN[viWfr  ]->ProcPkg.m_pProc[iiWfrOcv1])->Tracker -> HeightMax = 80 ;
    ((COcv*)VISN[viWfr  ]->ProcPkg.m_pProc[iiWfrOcv1])->Tracker -> WidthMax  = 80 ;

    ((COcv*)VISN[viWfr  ]->ProcPkg.m_pProc[iiWfrOcv2])->Tracker -> Band = 5 ;
    ((COcv*)VISN[viWfr  ]->ProcPkg.m_pProc[iiWfrOcv2])->Tracker -> HeightMax = 80 ;
    ((COcv*)VISN[viWfr  ]->ProcPkg.m_pProc[iiWfrOcv2])->Tracker -> WidthMax  = 80 ;
    ((COcv*)VISN[viWfr  ]->ProcPkg.m_pProc[iiWfrOcv2])->Tracker -> BandEnable = false ;


    /*Wafer Ori Vision*/
/*
    EN_PROC_TYPE iPtWfo     [MAX_WFO_INSP];
    int          iIntParaWfo[MAX_WFO_INSP];
    iPtWfo[iiWfo] = ptOcv ; iIntParaWfo[iiWfo] = 0 ;
    VISN[viWfo ]->ProcPkg.SetProcList(MAX_WFO_INSP , iPtWfo , iIntParaWfo) ;
*/
    EN_PROC_TYPE iPtWfo     [MAX_WFO_INSP];
    int          iIntParaWfo[MAX_WFO_INSP];
    iPtWfo[iiWfoRct ] = ptRct ; iIntParaWfo[iiWfoRct ] = 0 ;
    iPtWfo[iiWfoOcv1] = ptOcv ; iIntParaWfo[iiWfoOcv1] = 0 ;
    iPtWfo[iiWfoOcv2] = ptOcv ; iIntParaWfo[iiWfoOcv2] = 0 ;
    VISN[viWfo  ]->ProcPkg.SetProcList(MAX_WFO_INSP , iPtWfo , iIntParaWfo) ;
    //((CRct*)VISN[viWfo  ]->ProcPkg.m_pProc[iiWfoRct])->Tracker -> Band = 20 ;
    ((CRct*)VISN[viWfo  ]->ProcPkg.m_pProc[iiWfoRct])->Tracker -> BandMax    = 20   ;
    ((CRct*)VISN[viWfo  ]->ProcPkg.m_pProc[iiWfoRct])->Tracker -> BandEnable = true ;


    ((COcv*)VISN[viWfo  ]->ProcPkg.m_pProc[iiWfoOcv1])->Tracker -> HeightMax = 80 ;
    ((COcv*)VISN[viWfo  ]->ProcPkg.m_pProc[iiWfoOcv1])->Tracker -> WidthMax  = 80 ;

    ((COcv*)VISN[viWfo  ]->ProcPkg.m_pProc[iiWfoOcv2])->Tracker -> Band = 5 ;
    ((COcv*)VISN[viWfo  ]->ProcPkg.m_pProc[iiWfoOcv2])->Tracker -> HeightMax = 80 ;
    ((COcv*)VISN[viWfo  ]->ProcPkg.m_pProc[iiWfoOcv2])->Tracker -> WidthMax  = 80 ;
    ((COcv*)VISN[viWfo  ]->ProcPkg.m_pProc[iiWfoOcv2])->Tracker -> BandEnable = false ;







    /*Wafer Pen Vision*/
    EN_PROC_TYPE iPtWfp     [MAX_WFP_INSP];
    int          iIntParaWfp[MAX_WFP_INSP];
    iPtWfp[iiWfpTsd] = ptTsd ; iIntParaWfp[iiWfpTsd] = 1 ;
    VISN[viWfp ]->ProcPkg.SetProcList(MAX_WFP_INSP , iPtWfp , iIntParaWfp) ;

    /*Cal Vision*/
    EN_PROC_TYPE iPtCal     [MAX_CAL_INSP];
    int          iIntParaCal[MAX_CAL_INSP];
    iPtCal[iiCalCal] = ptCal ; iIntParaCal[iiCalCal] = 0 ;
    VISN[viCal ]->ProcPkg.SetProcList(MAX_CAL_INSP , iPtCal , iIntParaCal) ;

    FrmAlgoPara -> Init() ;




    LoadJobFile(true ,_sDevName) ;

    tmTrackerMoved = new TTimer(Application);
    tmTrackerMoved -> Interval = 100 ;
    tmTrackerMoved -> OnTimer = tmTrackerMovedTimer ;

    tmTrackerMoved -> Enabled = true ;

}

bool CVisionMan::SetParentVisnMain(TPanel * _pBase)
{

    //if(!VISN[0]->GetGrabEnd()) Trace("sunsunsu","Graibbing");


    //for(int i=0; i < MAX_VISN_ID ; i++) {
    //    VISN[i]->Live(false);
    //}


    static int iGrabbingCnt = 0 ;
    bool bGrabbing = false ;
    while(1){
        for(int i=0; i < MAX_VISN_ID ; i++) {
            if(!VISN[i]->GetGrabEnd()){
                Trace("sunsunsu","Graibbing");
                bGrabbing = true ;
            }
        }
        if( bGrabbing) {
            bGrabbing = false ;
            continue ;
        }
        break ;
    }



    FrmVisnMain -> Parent = _pBase ;
    if(_pBase != NULL)FrmVisnMain -> Show();
    else              FrmVisnMain -> Close();
    FrmVisnMain -> Align = alClient ;

    static bool bFirst = true ;
    if(bFirst) {
        FrmVisnMain -> Init();

        for(int i = 0 ; i < MAX_VISN_ID ; i++) {
            FrmVisnMain -> FrmImg[i] -> SetInspCallbackFunc(&Inspect) ;
        }
        SetActiveVisn(0);


    }
    bFirst = false ;
}

bool CVisionMan::SetParentVisnPara(TPanel * _pBase)
{
    FrmVisnPara -> BorderStyle = bsNone ;
    FrmVisnPara -> Parent = _pBase ;
    //FrmVisnPara -> Show();
    if(_pBase != NULL)FrmVisnPara -> FormShow(GetActiveVisn());
    else              FrmVisnPara -> Close();
    FrmVisnPara -> Left = 0 ;
    FrmVisnPara -> Top  = 0 ;

    //FrmVisnPara -> Align = alClient ;

}

bool CVisionMan::SetParentAlgoPara(TPanel * _pBase)
{
    FrmAlgoPara -> BorderStyle = bsNone ;
    FrmAlgoPara -> Parent = _pBase ;
    //FrmAlgoPara -> Show();
    int iVisnActivePage = GetActiveVisn() ;
    if(_pBase != NULL) FrmAlgoPara -> FormShow(iVisnActivePage);
    else               FrmAlgoPara -> Close();
    FrmAlgoPara -> Left = 0 ;
    FrmAlgoPara -> Top  = 0 ;

    //FrmVisnPara -> Align = alClient ;

}

bool CVisionMan::DelVisn()
{


    LiveAllOff(true);

    delete FrmVisnMain ;
    for(int i = 0 ; i < MAX_VISN_ID ; i++) {

        VISN[i] -> close();
        delete VISN[i] ;
    }

    for(int i = 0 ; i < MAX_CAM_ID ; i++) {
        delete Cam [i] ;
    }

    for(int i = 0 ; i < MAX_LIGHT_ID ; i++) {
        delete Light[i] ;
    }
    FreeSVL() ;

//
//    Application -> ProcessMessages() ;
}

void CVisionMan::LoadJobFile(bool _bLoad , AnsiString _sDevName)
{
    g_sJobFileName = _sDevName ;
    g_sJobFilePath = GetExeDir("JobFile\\") + _sDevName + "\\Vision\\";

    //알고리즘 파라 로드.
    VOM.LoadJobFile(_bLoad);

    for( int i = 0 ; i < MAX_VISN_ID ; i++) {
        if(VISN[i] != NULL)VISN[i]->LoadJobFile(_bLoad);
    }
}


void CVisionMan::Grab (EN_VISN_ID _iVisnId )
{
    //SetActiveVisn(_iVisnId);

    VISN[_iVisnId]->Grab() ;
    VISN[_iVisnId]->SetViewMode(vmNone) ;

}

void CVisionMan::Live (EN_VISN_ID _iVisnId , bool _bOn)
{
    VISN[_iVisnId]->Live(_bOn);
}

void CVisionMan::LiveAllOff(bool _bCheckGrabEnd )
{
    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        VISN[i]->Live(false);
    }

    if(!_bCheckGrabEnd) return ;


    bool bGrabbing = false ;
    while(1){
        for(int i=0; i < MAX_VISN_ID ; i++) {
            if(!VISN[i]->GetGrabEnd()){
                bGrabbing = true ;
            }
        }
        if( bGrabbing) {
            bGrabbing = false ;
            continue ;
        }
        break ;

    }
}

bool CVisionMan::GetGrabEnd(EN_VISN_ID _iVisnId )
{
    return VISN[_iVisnId]->GetGrabEnd();

}

int g_iInspId = 0 ;
void CVisionMan::SaveImage()
{
    //옮겨담아놓는다.   쓰레드 돌릴때 삑사리 날까봐.
    int iInspId = g_iInspId ;
    AnsiString sSaveFolderPath , sSavePath;

         if(iInspId == viWfr) sSaveFolderPath = AnsiString("d:\\Image\\Wfr\\");
    else if(iInspId == viWfo) sSaveFolderPath = AnsiString("d:\\Image\\Wfo\\");
    else if(iInspId == viWfp) sSaveFolderPath = AnsiString("d:\\Image\\Wfp\\");
    else if(iInspId == viCal) sSaveFolderPath = AnsiString("d:\\Image\\Cal\\");

    if(!UserFile.DirExists(sSaveFolderPath)) UserFile.CreateDir(sSaveFolderPath) ;

    sSavePath = sSaveFolderPath + Now().FormatString("yyyymmdd_hhnnss_zzz");
    VISN[iInspId]->Img -> SaveBmp(sSavePath.c_str());
}


void CVisionMan::Inspection()
{
    //옮겨담아놓는다.   쓰레드 돌릴때 삑사리 날까봐.
    int iInspId = m_iInspectionId ;

    Rslt[iInspId].bInspEnd = false ;
    Rslt[iInspId].iRet     = 0     ;
    Rslt[iInspId].fRsltT   = 0.0   ;
    Rslt[iInspId].fRsltX   = 0.0   ;
    Rslt[iInspId].fRsltY   = 0.0   ;
    VISN[iInspId]->SetRsltMsg(true , ""  );
    TFPoint tPnt ;
    tPnt.x = 0 ;
    tPnt.y = 0 ;
    VISN[iInspId]->SetRsltPnt(tPnt);

    EDG_Rslt EdgRslt ;
    LIN_Rslt LinRslt ;
    LCP_Rslt LcpRslt ;
    RCT_Rslt RctRslt ;
    TSD_Rslt TsdRslt ;
    BLB_Rslt BlbRslt ;
    CAL_Rslt CalRslt ;
    CCL_Rslt CclRslt ;
    OCV_Rslt OcvRslt ;

    AnsiString sTemp ;

    float fX , fY , fT ;

    bool bRet ;

    VISN[iInspId]->SetViewMode(vmAll) ;

    if(iInspId == viWfr   ) {
        float fLength , fLength2, fWid , fHei , fAngle , fAngL , fAngT , fAngR , fAngB , fAngMax , fAngMin;  //각변 결과값.
        float fWfrWidthPx , fWfrHeightPx , fWfrGapXPx , fWfrGapYPx ;

        //Ocv1
        int iWfrTrackerXOffset ;
        int iWfrTrackerYOffset ;

        iWfrTrackerXOffset = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrRct])->Tracker->GetRefX() ;
        iWfrTrackerYOffset = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrRct])->Tracker->GetRefY() ;

        VISN[iInspId]->ProcPkg.SetInspOfs(iiWfrOcv1 , iWfrTrackerXOffset , iWfrTrackerYOffset);

        if(m_bWfrOri1WideInspection) {
            TOutBandRectTracker * Tracker = ((COcv *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrOcv1])->Tracker ;
            int iOldBand = Tracker->Band ;
            //넓이와 높이중 작은걸로 광역 검사용 밴드를 임시 생성 한다.
            //넓은것으로 하면 2개 들어올 확율이 있다.
            bool bMinHeight = VOM.VDevOptn.fWfrHeight < VOM.VDevOptn.fWfrWidth ;
            int iMinValue   = bMinHeight ?
                            VOM.VDevOptn.fWfrHeight / VISN[iInspId]->Cal->Rslt.fYPxRes :
                            VOM.VDevOptn.fWfrWidth  / VISN[iInspId]->Cal->Rslt.fXPxRes ;

            int iNewBand = (iMinValue - (bMinHeight ? Tracker -> Height : Tracker -> Width)) /2 ; //밴드는 2로 나눠 줘야 된다.
            Tracker -> Band = iNewBand ;
            bRet = VISN[iInspId]->ProcPkg.Process(iiWfrOcv1 , VISN[iInspId]->Img , &OcvRslt);
            Tracker -> Band = iOldBand ;
        }
        else {
            bRet = VISN[iInspId]->ProcPkg.Process(iiWfrOcv1 , VISN[iInspId]->Img , &OcvRslt);
        }

        if(bRet == false ) {
            Rslt[iInspId].iRet   = veWfrOcv1Fail      ;
            VISN[iInspId]->SetRsltMsg(false , "1.FAIL OCV1=" + AnsiString(SVL_GetErrMsg()));
            goto __GOTO_WFR_END__ ;
        }
        if(OcvRslt.fSinc < VOM.VDevOptn.fWfrOriSinc) {

            Rslt[iInspId].iRet   = veWfrOcv1Sinc      ;
            VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG OCV Sinc : %.2f" , OcvRslt.fSinc ));
            goto __GOTO_WFR_END__ ;
        }

        //Ocv2
        iWfrTrackerXOffset = OcvRslt.iPosX - ((COcv *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrOcv1])->Tracker->GetCenterX() ;
        iWfrTrackerYOffset = OcvRslt.iPosY - ((COcv *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrOcv1])->Tracker->GetCenterY() ;

        VISN[iInspId]->ProcPkg.SetInspOfs(iiWfrOcv2 , iWfrTrackerXOffset , iWfrTrackerYOffset);
        bRet = VISN[iInspId]->ProcPkg.Process(iiWfrOcv2 , VISN[iInspId]->Img , &OcvRslt);

        //스펙 확인.
        if(bRet == false ) {
            Rslt[iInspId].iRet   = veWfrOcv2Fail      ;
            VISN[iInspId]->SetRsltMsg(false , "1.FAIL OCV2=" + AnsiString(SVL_GetErrMsg()));
            goto __GOTO_WFR_END__ ;
        }
        if(OcvRslt.fSinc < VOM.VDevOptn.fWfrOriSinc) {

            Rslt[iInspId].iRet   = veWfrOcv2Sinc      ;
            VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG OCV Sinc : %.2f" , OcvRslt.fSinc ));
            goto __GOTO_WFR_END__ ;
        }


        //Rect Chip //sunkyewon
        if(!VOM.VDevOptn.bWfrIgnRctInsp) {
            VISN[iInspId]->ProcPkg.SetInspOfs(iiWfrRct , iWfrTrackerXOffset , iWfrTrackerYOffset);
            bRet = VISN[iInspId]->ProcPkg.Process(iiWfrRct , VISN[iInspId]->Img , &RctRslt);

            if(bRet == false ) { //검사 실패.
                Rslt[iInspId].iRet  = veWfrRctFail ;
                VISN[iInspId]->SetRsltMsg(false , "1.FAIL RCT=" + AnsiString(SVL_GetErrMsg()));
                goto __GOTO_WFR_END__ ;
            }
            //검사 완료 됐으면 설정스펙 비교.

            //치수 픽셀값으로 환산.
            fWfrWidthPx  = VOM.VDevOptn.fWfrWidth  / VISN[iInspId]->Cal->Rslt.fXPxRes ;
            fWfrHeightPx = VOM.VDevOptn.fWfrHeight / VISN[iInspId]->Cal->Rslt.fYPxRes ;
            fWfrGapXPx   = VOM.VDevOptn.fWfrGap    / VISN[iInspId]->Cal->Rslt.fXPxRes ;
            fWfrGapYPx   = VOM.VDevOptn.fWfrGap    / VISN[iInspId]->Cal->Rslt.fYPxRes ;


            fLength = MATH_GetLineLenth(RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y , RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y); //왼쪽변.
            if(fWfrHeightPx + fWfrGapYPx < fLength || fLength < fWfrHeightPx - fWfrGapYPx) {
                Rslt[iInspId].iRet = veWfrRct       ;
                VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Left Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fYPxRes));
                goto __GOTO_WFR_END__ ;
            }

            fLength = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
            if(fWfrWidthPx + fWfrGapXPx < fLength || fLength < fWfrWidthPx - fWfrGapXPx) {
                Rslt[iInspId].iRet = veWfrRct       ;
                VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Top Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
                goto __GOTO_WFR_END__ ;
            }

            fLength = MATH_GetLineLenth(RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y , RctRslt.CrossPntRB.x , RctRslt.CrossPntRB.y); //오른쪽변
            if(fWfrHeightPx + fWfrGapYPx < fLength || fLength < fWfrHeightPx - fWfrGapYPx) {
                Rslt[iInspId].iRet = veWfrRct       ;
                VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Right Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fYPxRes));
                goto __GOTO_WFR_END__ ;
            }

            fLength = MATH_GetLineLenth(RctRslt.CrossPntRB.x , RctRslt.CrossPntRB.y , RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y); //아래변
            if(fWfrWidthPx + fWfrGapXPx < fLength || fLength < fWfrWidthPx - fWfrGapXPx) {
                Rslt[iInspId].iRet = veWfrRct       ;
                VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Bottom Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
                goto __GOTO_WFR_END__ ;
            }


            //칩 자체의 각도는 검사 안함. 일단...
            //각도 검사 최고 작은애랑 최고 큰애랑 격차를 비교 하여 넘어 가면 못찾은걸로 한다.
            fAngL = MATH_GetLineAngle(RctRslt.LinRsltL.StartPnt.x , RctRslt.LinRsltL.StartPnt.y , RctRslt.LinRsltL.EndPnt.x   , RctRslt.LinRsltL.EndPnt.y  ) - 270 ;
            fAngT = MATH_GetLineAngle(RctRslt.LinRsltT.EndPnt.x   , RctRslt.LinRsltT.EndPnt.y   , RctRslt.LinRsltT.StartPnt.x , RctRslt.LinRsltT.StartPnt.y) - 180 ;
            fAngR = MATH_GetLineAngle(RctRslt.LinRsltR.StartPnt.x , RctRslt.LinRsltR.StartPnt.y , RctRslt.LinRsltR.EndPnt.x   , RctRslt.LinRsltR.EndPnt.y  ) - 270 ;
            fAngB = MATH_GetLineAngle(RctRslt.LinRsltB.EndPnt.x   , RctRslt.LinRsltB.EndPnt.y   , RctRslt.LinRsltB.StartPnt.x , RctRslt.LinRsltB.StartPnt.y) - 180 ;

            fAngMin = fAngL ;
            if(fAngT < fAngMin) fAngMin = fAngT ;
            if(fAngR < fAngMin) fAngMin = fAngR ;
            if(fAngB < fAngMin) fAngMin = fAngB ;

            fAngMax = fAngL ;
            if(fAngT > fAngMax) fAngMax = fAngT ;
            if(fAngR > fAngMax) fAngMax = fAngR ;
            if(fAngB > fAngMax) fAngMax = fAngB ;

            if(VOM.VDevOptn.fWfrAngle < fAngMax - fAngMin) { //변끼리 90도 유지가 되어야 하는데 유지 되는 폭을 확인 하여 넘어가면 페일.
                Rslt[iInspId].iRet = veWfrAngle     ;
                VISN[iInspId]->SetRsltMsg(false ,  sTemp.sprintf("3.NG Angle Min Max Gap : %.2f" , fAngMax - fAngMin));
                goto __GOTO_WFR_END__ ;
            }

            //if(Rslt[iInspId].iRet == 1){//렉트 못찾아서 리턴.
            //    Rslt[iInspId].bInspEnd = true ;
            //    VISN[iInspId]->PaintCallback();
            //    return ;
            //}

            fX = VOM.VCmnOptn.bWfrFlipX ? -(RctRslt.CenterPnt.x - VISN[iInspId]->GetCenterX()) : (RctRslt.CenterPnt.x - VISN[iInspId]->GetCenterX()) ;
            fY = VOM.VCmnOptn.bWfrFlipY ? -(RctRslt.CenterPnt.y - VISN[iInspId]->GetCenterY()) : (RctRslt.CenterPnt.y - VISN[iInspId]->GetCenterY()) ;
            fT = VOM.VCmnOptn.bWfrFlipT ? -(         RctRslt.fAngle     ) : (         RctRslt.fAngle     ) ;
        }
        else {
            fX = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrRct])->Tracker->GetCenterX() + iWfrTrackerXOffset;
            fY = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrRct])->Tracker->GetCenterY() + iWfrTrackerYOffset;

            fX = VOM.VCmnOptn.bWfrFlipX ? -(fX - VISN[iInspId]->GetCenterX()) : (fX - VISN[iInspId]->GetCenterX()) ;
            fY = VOM.VCmnOptn.bWfrFlipY ? -(fY - VISN[iInspId]->GetCenterY()) : (fY - VISN[iInspId]->GetCenterY()) ;
            fT = VOM.VCmnOptn.bWfrFlipT ? -(         0.0     ) : (         0.0     ) ;
        }


        Rslt[iInspId].fRsltX = fX * VISN[iInspId]->Cal->Rslt.fXPxRes ;
        Rslt[iInspId].fRsltY = fY * VISN[iInspId]->Cal->Rslt.fYPxRes ;
        Rslt[iInspId].fRsltT = fT ;

        /* 크랙 노페키지 관련 검사 안함.
        if( VOM.VDevOptn.iWfrCCPxCnt && 90.0 < (RctRslt.iChkPxOutCnt * 100)/ RctRslt.iTtlChkPxCnt) {
            Rslt[iInspId].iRet = veWfrNoPkg ;
            sTemp.sprintf("7.NoPkg Pixel Cnt : %d W:%.2f H:%.2f" , RctRslt.iChkPxOutCnt,fWid * VISN[iInspId]->Cal->Rslt.fXPxRes , fHei * VISN[iInspId]->Cal->Rslt.fYPxRes);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("7.NoPkg Pixel Cnt : %d" , RctRslt.iChkPxOutCnt));
            goto __GOTO_WFR_END__ ;
        }

        if( VOM.VDevOptn.iWfrCCPxCnt && VOM.VDevOptn.iWfrCCPxCnt < RctRslt.iChkPxOutCnt) {
            Rslt[iInspId].iRet = veWfrCrack ; //외곽이 잘 형성이 안되거나 깨진칩.
            sTemp.sprintf("4.CRACK Pixel Cnt : %d" , RctRslt.iChkPxOutCnt);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("4.CRACK Pixel Cnt : %d" , RctRslt.iChkPxOutCnt));
            goto __GOTO_WFR_END__ ;
        }
        */

        //최종 Good
        if(Rslt[iInspId].iRet == veWfrOk       ){
            fLength  = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
            fLength2 = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y); //왼쪽 변
            //sunkyewon
            if(!VOM.VDevOptn.bWfrIgnRctInsp){
                VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK CHIP X : %.3f Y : %.3f" , Rslt[iInspId].fRsltX , Rslt[iInspId].fRsltY));
                VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK RCT Line Top : %.2f Left : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes , fLength2* VISN[iInspId]->Cal->Rslt.fYPxRes));
            }
            VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK CHIP X : %.3f Y : %.3f" , Rslt[iInspId].fRsltX , Rslt[iInspId].fRsltY));
        }


        __GOTO_WFR_END__: //----------------------------------------------------
        Rslt[iInspId].bInspEnd = true ;            //

        // 와이드 검사시에 렉트 검사 안되면 그냥 트렉커 센터 위치 보내 주기 하자.

        if(m_bWfrOri1WideInspection){
            if(Rslt[iInspId].iRet == veWfrRctFail  ||
               Rslt[iInspId].iRet == veWfrRct      ||
               Rslt[iInspId].iRet == veWfrAngle    ||
               Rslt[iInspId].iRet == veWfrCrack    ||
               Rslt[iInspId].iRet == veWfrDot      ||
               Rslt[iInspId].iRet == veWfrNoPkg    ) {
                TRectTracker * Tracker = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrRct])->Tracker ;
                //조 위에서 이미 트레커에 오프셑 넣어 놓음.

                //오프셑먹은 트렉커 센터 가져 오기.
                TPoint Pnt , OfsPnt;
                Pnt.x = Tracker -> GetCenterX() + iWfrTrackerXOffset ;
                Pnt.y = Tracker -> GetCenterY() + iWfrTrackerYOffset ;

                fX = VOM.VCmnOptn.bWfrFlipX ? -(Pnt.x - VISN[iInspId]->GetCenterX()) : (Pnt.x - VISN[iInspId]->GetCenterX()) ;
                fY = VOM.VCmnOptn.bWfrFlipY ? -(Pnt.y - VISN[iInspId]->GetCenterY()) : (Pnt.y - VISN[iInspId]->GetCenterY()) ;
                fT = 0.0 ;


                Rslt[iInspId].fRsltX = fX * VISN[iInspId]->Cal->Rslt.fXPxRes ;
                Rslt[iInspId].fRsltY = fY * VISN[iInspId]->Cal->Rslt.fYPxRes ;
                Rslt[iInspId].fRsltT = fT ;

                VISN[iInspId]->SetRsltMsg(true , "OK"  );
                Rslt[iInspId].iRet = veWfrOk ;
            }
        }

        VISN[iInspId]->PaintCallback();

        if((VOM.VCmnOptn.iWfrImageSave == 1 && Rslt[iInspId].iRet != 0) ||
           (VOM.VCmnOptn.iWfrImageSave == 2 && Rslt[iInspId].iRet == 0) ||
           (VOM.VCmnOptn.iWfrImageSave == 3                           ) ){
            g_iInspId = iInspId ;
            CRunThread<Func> * func = new CRunThread<Func>(&SaveImage) ;
        }
    }

//New Wafer Ori
    else if(iInspId == viWfo   ) {
        float fLength , fWid , fHei , fAngle , fAngL , fAngT , fAngR , fAngB , fAngMax , fAngMin;  //각변 결과값.
        float fWfoWidthPx , fWfoHeightPx , fWfoGapXPx , fWfoGapYPx ;

        //Ocv1
        int iWfoTrackerXOffset ;
        int iWfoTrackerYOffset ;

        iWfoTrackerXOffset = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfoRct])->Tracker->GetRefX() ;
        iWfoTrackerYOffset = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfoRct])->Tracker->GetRefY() ;

        VISN[iInspId]->ProcPkg.SetInspOfs(iiWfoOcv1 , iWfoTrackerXOffset , iWfoTrackerYOffset);

        if(m_bWfoOri1WideInspection) {
            TOutBandRectTracker * Tracker = ((COcv *)VISN[iInspId]->ProcPkg.m_pProc[iiWfoOcv1])->Tracker ;
            int iOldBand = Tracker->Band ;
            //넓이와 높이중 작은걸로 광역 검사용 밴드를 임시 생성 한다.
            //넓은것으로 하면 2개 들어올 확율이 있다.
            bool bMinHeight = VOM.VDevOptn.fWfrHeight < VOM.VDevOptn.fWfrWidth ;
            int iMinValue   = bMinHeight ?
                            VOM.VDevOptn.fWfrHeight / VISN[iInspId]->Cal->Rslt.fYPxRes :
                            VOM.VDevOptn.fWfrWidth  / VISN[iInspId]->Cal->Rslt.fXPxRes ;

            int iNewBand = (iMinValue - (bMinHeight ? Tracker -> Height : Tracker -> Width)) /2 ; //밴드는 2로 나눠 줘야 된다.
            Tracker -> Band = iNewBand ;
            bRet = VISN[iInspId]->ProcPkg.Process(iiWfoOcv1 , VISN[iInspId]->Img , &OcvRslt);
            Tracker -> Band = iOldBand ;
        }
        else {
            bRet = VISN[iInspId]->ProcPkg.Process(iiWfoOcv1 , VISN[iInspId]->Img , &OcvRslt);
        }

        if(bRet == false ) {
            Rslt[iInspId].iRet   = veWfoOcv1Fail      ;
            VISN[iInspId]->SetRsltMsg(false , "1.FAIL OCV1=" + AnsiString(SVL_GetErrMsg()));
            goto __GOTO_WFO_END__ ;
        }
        if(OcvRslt.fSinc < VOM.VDevOptn.fWfoOriSinc) {

            Rslt[iInspId].iRet   = veWfoOcv1Sinc      ;
            VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG OCV Sinc : %.2f" , OcvRslt.fSinc ));
            goto __GOTO_WFO_END__ ;
        }

        //Ocv2
        iWfoTrackerXOffset = OcvRslt.iPosX - ((COcv *)VISN[iInspId]->ProcPkg.m_pProc[iiWfoOcv1])->Tracker->GetCenterX() ;
        iWfoTrackerYOffset = OcvRslt.iPosY - ((COcv *)VISN[iInspId]->ProcPkg.m_pProc[iiWfoOcv1])->Tracker->GetCenterY() ;

        VISN[iInspId]->ProcPkg.SetInspOfs(iiWfoOcv2 , iWfoTrackerXOffset , iWfoTrackerYOffset);
        bRet = VISN[iInspId]->ProcPkg.Process(iiWfoOcv2 , VISN[iInspId]->Img , &OcvRslt);

        //스펙 확인.
        if(bRet == false ) {
            Rslt[iInspId].iRet   = veWfoOcv2Fail      ;
            VISN[iInspId]->SetRsltMsg(false , "1.FAIL OCV2=" + AnsiString(SVL_GetErrMsg()));
            goto __GOTO_WFO_END__ ;
        }
        if(OcvRslt.fSinc < VOM.VDevOptn.fWfoOriSinc) {

            Rslt[iInspId].iRet   = veWfoOcv2Sinc      ;
            VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG OCV Sinc : %.2f" , OcvRslt.fSinc ));
            goto __GOTO_WFO_END__ ;
        }


        //Rect Chip   //sunkyewon
        if(!VOM.VDevOptn.bWfoIgnRctInsp){
            VISN[iInspId]->ProcPkg.SetInspOfs(iiWfoRct , iWfoTrackerXOffset , iWfoTrackerYOffset);
            bRet = VISN[iInspId]->ProcPkg.Process(iiWfoRct , VISN[iInspId]->Img , &RctRslt);

            if(bRet == false ) { //검사 실패.
                Rslt[iInspId].iRet  = veWfoRctFail   ;
                VISN[iInspId]->SetRsltMsg(false , "1.FAIL RCT=" + AnsiString(SVL_GetErrMsg()));
                goto __GOTO_WFO_END__ ;
            }
            //검사 완료 됐으면 설정스펙 비교.

            //치수 픽셀값으로 환산.
            fWfoWidthPx  = VOM.VDevOptn.fWfrWidth   / VISN[iInspId]->Cal->Rslt.fXPxRes ;
            fWfoHeightPx = VOM.VDevOptn.fWfrHeight  / VISN[iInspId]->Cal->Rslt.fYPxRes ;
            fWfoGapXPx   = VOM.VDevOptn.fWfrGap     / VISN[iInspId]->Cal->Rslt.fXPxRes ;
            fWfoGapYPx   = VOM.VDevOptn.fWfrGap     / VISN[iInspId]->Cal->Rslt.fYPxRes ;


            fLength = MATH_GetLineLenth(RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y , RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y); //왼쪽변.
            if(fWfoHeightPx + fWfoGapYPx < fLength || fLength < fWfoHeightPx - fWfoGapYPx) {
                Rslt[iInspId].iRet = veWfoRct       ;
                VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Left Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fYPxRes));
                goto __GOTO_WFO_END__ ;
            }

            fLength = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
            if(fWfoWidthPx + fWfoGapXPx < fLength || fLength < fWfoWidthPx - fWfoGapXPx) {
                Rslt[iInspId].iRet = veWfoRct       ;
                VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Top Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
                goto __GOTO_WFO_END__ ;
            }

            fLength = MATH_GetLineLenth(RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y , RctRslt.CrossPntRB.x , RctRslt.CrossPntRB.y); //오른쪽변
            if(fWfoHeightPx + fWfoGapYPx < fLength || fLength < fWfoHeightPx - fWfoGapYPx) {
                Rslt[iInspId].iRet = veWfoRct       ;
                VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Right Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fYPxRes));
                goto __GOTO_WFO_END__ ;
            }

            fLength = MATH_GetLineLenth(RctRslt.CrossPntRB.x , RctRslt.CrossPntRB.y , RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y); //아래변
            if(fWfoWidthPx + fWfoGapXPx < fLength || fLength < fWfoWidthPx - fWfoGapXPx) {
                Rslt[iInspId].iRet = veWfoRct       ;
                VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Bottom Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
                goto __GOTO_WFO_END__ ;
            }


            //칩 자체의 각도는 검사 안함. 일단...
            //각도 검사 최고 작은애랑 최고 큰애랑 격차를 비교 하여 넘어 가면 못찾은걸로 한다.
            fAngL = MATH_GetLineAngle(RctRslt.LinRsltL.StartPnt.x , RctRslt.LinRsltL.StartPnt.y , RctRslt.LinRsltL.EndPnt.x   , RctRslt.LinRsltL.EndPnt.y  ) - 270 ;
            fAngT = MATH_GetLineAngle(RctRslt.LinRsltT.EndPnt.x   , RctRslt.LinRsltT.EndPnt.y   , RctRslt.LinRsltT.StartPnt.x , RctRslt.LinRsltT.StartPnt.y) - 180 ;
            fAngR = MATH_GetLineAngle(RctRslt.LinRsltR.StartPnt.x , RctRslt.LinRsltR.StartPnt.y , RctRslt.LinRsltR.EndPnt.x   , RctRslt.LinRsltR.EndPnt.y  ) - 270 ;
            fAngB = MATH_GetLineAngle(RctRslt.LinRsltB.EndPnt.x   , RctRslt.LinRsltB.EndPnt.y   , RctRslt.LinRsltB.StartPnt.x , RctRslt.LinRsltB.StartPnt.y) - 180 ;

            fAngMin = fAngL ;
            if(fAngT < fAngMin) fAngMin = fAngT ;
            if(fAngR < fAngMin) fAngMin = fAngR ;
            if(fAngB < fAngMin) fAngMin = fAngB ;

            fAngMax = fAngL ;
            if(fAngT > fAngMax) fAngMax = fAngT ;
            if(fAngR > fAngMax) fAngMax = fAngR ;
            if(fAngB > fAngMax) fAngMax = fAngB ;

            if(VOM.VDevOptn.fWfrAngle < fAngMax - fAngMin) { //변끼리 90도 유지가 되어야 하는데 유지 되는 폭을 확인 하여 넘어가면 페일.
                Rslt[iInspId].iRet = veWfoAngle     ;
                VISN[iInspId]->SetRsltMsg(false ,  sTemp.sprintf("3.NG Angle Min Max Gap : %.2f" , fAngMax - fAngMin));
                goto __GOTO_WFO_END__ ;
            }

            //if(Rslt[iInspId].iRet == 1){//렉트 못찾아서 리턴.
            //    Rslt[iInspId].bInspEnd = true ;
            //    VISN[iInspId]->PaintCallback();
            //    return ;
            //}

            fX = VOM.VCmnOptn.bWfoFlipX ? -(RctRslt.CenterPnt.x - VISN[iInspId]->GetCenterX()) : (RctRslt.CenterPnt.x - VISN[iInspId]->GetCenterX()) ;
            fY = VOM.VCmnOptn.bWfoFlipY ? -(RctRslt.CenterPnt.y - VISN[iInspId]->GetCenterY()) : (RctRslt.CenterPnt.y - VISN[iInspId]->GetCenterY()) ;
            fT = VOM.VCmnOptn.bWfoFlipT ? -(         RctRslt.fAngle     ) : (         RctRslt.fAngle     ) ;
        }
        else {
            fX = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfoRct])->Tracker->GetCenterX() + iWfoTrackerXOffset;
            fY = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfoRct])->Tracker->GetCenterY() + iWfoTrackerYOffset;

            fX = VOM.VCmnOptn.bWfrFlipX ? -(fX - VISN[iInspId]->GetCenterX()) : (fX - VISN[iInspId]->GetCenterX()) ;
            fY = VOM.VCmnOptn.bWfrFlipY ? -(fY - VISN[iInspId]->GetCenterY()) : (fY - VISN[iInspId]->GetCenterY()) ;
            fT = VOM.VCmnOptn.bWfrFlipT ? -(         0.0     ) : (         0.0     ) ;
        }

        Rslt[iInspId].fRsltX = fX * VISN[iInspId]->Cal->Rslt.fXPxRes ;
        Rslt[iInspId].fRsltY = fY * VISN[iInspId]->Cal->Rslt.fYPxRes ;
        Rslt[iInspId].fRsltT = fT ;

        /*
        if( VOM.VDevOptn.iWfoCCPxCnt && 90.0 < (RctRslt.iChkPxOutCnt * 100)/ RctRslt.iTtlChkPxCnt) {
            Rslt[iInspId].iRet = veWfoNoPkg ;
            sTemp.sprintf("7.NoPkg Pixel Cnt : %d W:%.2f H:%.2f" , RctRslt.iChkPxOutCnt,fWid * VISN[iInspId]->Cal->Rslt.fXPxRes , fHei * VISN[iInspId]->Cal->Rslt.fYPxRes);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("7.NoPkg Pixel Cnt : %d" , RctRslt.iChkPxOutCnt));
            goto __GOTO_WFO_END__ ;
        }

        if( VOM.VDevOptn.iWfoCCPxCnt && VOM.VDevOptn.iWfoCCPxCnt < RctRslt.iChkPxOutCnt) {
            Rslt[iInspId].iRet = veWfoCrack ; //외곽이 잘 형성이 안되거나 깨진칩.
            sTemp.sprintf("4.CRACK Pixel Cnt : %d" , RctRslt.iChkPxOutCnt);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("4.CRACK Pixel Cnt : %d" , RctRslt.iChkPxOutCnt));
            goto __GOTO_WFO_END__ ;
        }
        */

        //최종 Good
        if(Rslt[iInspId].iRet == veWfoOk       ){
            fLength = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
            if(!VOM.VDevOptn.bWfoIgnRctInsp){ //sunkyewon
                VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK RCT Top Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
            }
            VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK CHIP X : %.3f Y : %.3f" , Rslt[iInspId].fRsltX , Rslt[iInspId].fRsltY));
        }


        __GOTO_WFO_END__: //----------------------------------------------------
        Rslt[iInspId].bInspEnd = true ;            //

        // 와이드 검사시에 렉트 검사 안되면 그냥 트렉커 센터 위치 보내 주기 하자.

        if(m_bWfoOri1WideInspection){
            if(Rslt[iInspId].iRet == veWfoRctFail  ||
               Rslt[iInspId].iRet == veWfoRct      ||
               Rslt[iInspId].iRet == veWfoAngle    ||
               Rslt[iInspId].iRet == veWfoCrack    ||
               Rslt[iInspId].iRet == veWfoDot      ||
               Rslt[iInspId].iRet == veWfoNoPkg    ) {
                TRectTracker * Tracker = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfoRct])->Tracker ;
                //조 위에서 이미 트레커에 오프셑 넣어 놓음.

                //오프셑먹은 트렉커 센터 가져 오기.
                TPoint Pnt , OfsPnt;
                Pnt.x = Tracker -> GetCenterX() + iWfoTrackerXOffset ;
                Pnt.y = Tracker -> GetCenterY() + iWfoTrackerYOffset ;

                fX = VOM.VCmnOptn.bWfoFlipX ? -(Pnt.x - VISN[iInspId]->GetCenterX()) : (Pnt.x - VISN[iInspId]->GetCenterX()) ;
                fY = VOM.VCmnOptn.bWfoFlipY ? -(Pnt.y - VISN[iInspId]->GetCenterY()) : (Pnt.y - VISN[iInspId]->GetCenterY()) ;
                fT = 0.0 ;


                Rslt[iInspId].fRsltX = fX * VISN[iInspId]->Cal->Rslt.fXPxRes ;
                Rslt[iInspId].fRsltY = fY * VISN[iInspId]->Cal->Rslt.fYPxRes ;
                Rslt[iInspId].fRsltT = fT ;

                VISN[iInspId]->SetRsltMsg(true , "OK"  );
                Rslt[iInspId].iRet = veWfoOk ;
            }
        }

        VISN[iInspId]->PaintCallback();

        if((VOM.VCmnOptn.iWfoImageSave == 1 && Rslt[iInspId].iRet != 0) ||
           (VOM.VCmnOptn.iWfoImageSave == 2 && Rslt[iInspId].iRet == 0) ||
           (VOM.VCmnOptn.iWfoImageSave == 3                           ) ){
            g_iInspId = iInspId ;
            CRunThread<Func> * func = new CRunThread<Func>(&SaveImage) ;
        }
    }

/*
    else if(iInspId == viWfo   ) {


        bRet = VISN[iInspId]->ProcPkg.Process(iiWfo , VISN[iInspId]->Img , &OcvRslt);

        //스펙 확인.
        if(bRet == false ) {
            Rslt[iInspId].iRet   = veWfoOcvFail       ;
            VISN[iInspId]->SetRsltMsg(false , "1.FAIL OCV=" + AnsiString(SVL_GetErrMsg()));
            goto __GOTO_WFO_END__ ;
        }
        if(OcvRslt.fSinc < VOM.VDevOptn.fWfoOriSinc) {

            Rslt[iInspId].iRet   = veWfoOcvSinc       ;
            VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG OCV Sinc : %.2f" , OcvRslt.fSinc ));
            goto __GOTO_WFO_END__ ;
        }

        fX = VOM.VCmnOptn.bWfoFlipX ? -(OcvRslt.iPosX - VISN[iInspId]->GetCenterX()) : (OcvRslt.iPosX - VISN[iInspId]->GetCenterX()) ;
        fY = VOM.VCmnOptn.bWfoFlipY ? -(OcvRslt.iPosY - VISN[iInspId]->GetCenterY()) : (OcvRslt.iPosY - VISN[iInspId]->GetCenterY()) ;

        Rslt[iInspId].fRsltX = fX * VISN[iInspId]->Cal->Rslt.fXPxRes ;
        Rslt[iInspId].fRsltY = fY * VISN[iInspId]->Cal->Rslt.fYPxRes ;

        if(Rslt[iInspId].iRet == veWfoOk       ){
            VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK Sinc : %.2f OfsX:%.2f , OfsY:%.2f" , OcvRslt.fSinc , Rslt[iInspId].fRsltX, Rslt[iInspId].fRsltY));

        }

        __GOTO_WFO_END__ : //----------------------------------------------------
        Rslt[iInspId].bInspEnd = true ;
        VISN[iInspId]->PaintCallback();

        if((VOM.VCmnOptn.iWfoImageSave == 1 && Rslt[iInspId].iRet != 0) ||
           (VOM.VCmnOptn.iWfoImageSave == 2 && Rslt[iInspId].iRet == 0) ||
           (VOM.VCmnOptn.iWfoImageSave == 3                           ) ){
            g_iInspId = iInspId ;
            CRunThread<Func> * func = new CRunThread<Func>(&SaveImage) ;
        }

    }
*/

    else if(iInspId == viWfp   ) {

        ((CTsd *)VISN[iInspId]->ProcPkg.m_pProc[iiWfpTsd]) -> Para.iThresholdHigh = VOM.VDevOptn.iWfpPxHigh ;

        bRet = VISN[iInspId]->ProcPkg.Process(iiWfpTsd , VISN[iInspId]->Img , &TsdRslt);

        //스펙 확인.
        if(bRet == false ) {
            Rslt[iInspId].iRet   = veWfpTsdFail       ;
            VISN[iInspId]->SetRsltMsg(false , "1.FAIL TSD=" + AnsiString(SVL_GetErrMsg()));
            goto __GOTO_WFO_END__ ;
        }

        //쓰레숄드 중간값을 넣어준다.
        fX = VOM.VCmnOptn.bWfpFlipX ? -(TsdRslt.fCntrX - VISN[iInspId]->GetCenterX()) : TsdRslt.fCntrX - VISN[iInspId]->GetCenterX() ;
        fY = VOM.VCmnOptn.bWfpFlipY ? -(TsdRslt.fCntrY - VISN[iInspId]->GetCenterY()) : TsdRslt.fCntrY - VISN[iInspId]->GetCenterY() ;
        Rslt[iInspId].fRsltX = fX * VISN[iInspId]->Cal->Rslt.fXPxRes ;
        Rslt[iInspId].fRsltY = fY * VISN[iInspId]->Cal->Rslt.fYPxRes ;


        if(TsdRslt.iPxCnt < VOM.VDevOptn.iWfpPxCnt) {

            Rslt[iInspId].iRet   = veWfpTsdPxCnt      ;
            VISN[iInspId]->SetRsltMsg(false , "2.NG PX CNT : " + AnsiString(TsdRslt.iPxCnt));
            goto __GOTO_WFO_END__ ;
        }

        if(Rslt[iInspId].iRet == veWfpOk       ){
            VISN[iInspId]->SetRsltMsg(true , "OK PX CNT : " + AnsiString(TsdRslt.iPxCnt));
        }

        __GOTO_WFP_END__ : //----------------------------------------------------
        Rslt[iInspId].bInspEnd = true ;
        VISN[iInspId]->PaintCallback();

        if((VOM.VCmnOptn.iWfpImageSave == 1 && Rslt[iInspId].iRet != 0) ||
           (VOM.VCmnOptn.iWfpImageSave == 2 && Rslt[iInspId].iRet == 0) ||
           (VOM.VCmnOptn.iWfpImageSave == 3                           ) ){
            g_iInspId = iInspId ;
            CRunThread<Func> * func = new CRunThread<Func>(&SaveImage) ;
        }

    }

    else if(iInspId == viCal   ) {

        bRet = VISN[iInspId]->ProcPkg.Process(iiCalCal , VISN[iInspId]->Img , &CalRslt);

        //스펙 확인.
        if(bRet == false ) {
            Rslt[iInspId].iRet   = veCalCalFail       ;
            VISN[iInspId]->SetRsltMsg(false , "1.FAIL CAL=" + AnsiString(SVL_GetErrMsg()));
            goto __GOTO_CAL_END__ ;
        }


        for(int i = 0 ; i < MAX_VISN_ID ; i++) {
            //각 비젼별 캘 데이터 복사.
            VISN[i]->Cal->Rslt.fXPxRes = CalRslt.fXPxRes ;
            VISN[i]->Cal->Rslt.fYPxRes = CalRslt.fYPxRes ;

            VISN[i]->LoadCalPara(false);
        }

        Rslt[iInspId].fRsltX = CalRslt.fXPxRes ;
        Rslt[iInspId].fRsltY = CalRslt.fYPxRes ;



        if(Rslt[iInspId].iRet == veCalOk       ){
            VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK Cal X : %.4f , Cal Y : %.4f" , Rslt[iInspId].fRsltX , Rslt[iInspId].fRsltY));
        }

        __GOTO_CAL_END__:

        Rslt[iInspId].bInspEnd = true ;
        VISN[iInspId]->PaintCallback();

        if((VOM.VCmnOptn.iCalImageSave == 1 && Rslt[iInspId].iRet != 0) ||
           (VOM.VCmnOptn.iCalImageSave == 2 && Rslt[iInspId].iRet == 0) ||
           (VOM.VCmnOptn.iCalImageSave == 3                           ) ){
            g_iInspId = iInspId ;
            CRunThread<Func> * func = new CRunThread<Func>(&SaveImage) ;
        }

    }

    if(g_bSettingMode) VISN[iInspId]->tmMode -> Enabled = true ;

}

bool CVisionMan::GetInspEnd(EN_VISN_ID _iVisnId )
{
    return Rslt[_iVisnId].bInspEnd ;

}

void CVisionMan::SetLight  (EN_VISN_ID _iVisnId )
{
    VISN[_iVisnId]->SetLight();


 //

}



void CVisionMan::Inspect(EN_VISN_ID _iVisnId , bool _bUseThread)
{
    bool bRslt ;
    float fRsltX , fRsltY ;

    Rslt[_iVisnId].bInspEnd = false ;

    m_iInspectionId = _iVisnId ;

    if(!_bUseThread) {
        return Inspection() ;
    }
    else {
        CRunThread<Func> * func = new CRunThread<Func>(&Inspection) ;
    }
}

int CVisionMan::GetRslt (EN_VISN_ID _iVisnId , TRetResult * _tRslt)
{
    memcpy(_tRslt , &Rslt[_iVisnId] , sizeof(TRetResult));
    return Rslt[_iVisnId].iRet ;
} //결과.

void CVisionMan::SetLevel(int _iLevel)
{
    FrmVisnMain -> SetLevel(_iLevel) ;





}
        /*
        0 = Oper
        1 = Engineer
        2 = Master
        */

void CVisionMan::SetAutorun(bool _bAutorun)
{




}


//비젼 탭 바꾸기.
void CVisionMan::SetActiveVisn(int _iVisnId)
{
    Trace("csPaint",("SetActiveVisn" + AnsiString(_iVisnId)).c_str());
    //FrmVisnMain -> pcVisnBase -> ActivePageIndex = _iVisnId ;

    FrmVisnMain -> m_iSendIndex = _iVisnId ;
    PostMessage(FrmVisnMain -> Handle , WM_USER , _iVisnId , 0);
}


int  CVisionMan::GetActiveVisn()
{

    return g_iShowingVisnId ;

}


void CVisionMan::SetSettingMode(bool _bSettingMode)
{
    g_bSettingMode = _bSettingMode ;

    //FrmAlgoPara -> pnInterface -> Visible = !_bSettingMode ;
    FrmAlgoPara -> Align = _bSettingMode ? alClient : alNone;
    for(int i= 0 ; i < MAX_VISN_ID ; i++) {
        if(!_bSettingMode)VISN[i]->SetViewMode(vmNone) ;
    //
    }

    FrmVisnPara -> btSave -> Visible = !_bSettingMode ;
}

void CVisionMan::SaveButtonClick()
{
    VISN[GetActiveVisn()]->ProcPkg.ApplyPara(false);
    VISN[GetActiveVisn()]->LoadJobFile(false );

    FrmVisnPara -> ApplyPara(false);
    VOM.LoadVDevOptn(false);
    VOM.LoadVCmnOptn(false);

    Trace("Operator", "SaveButtonClick");

    FrmVisnMain -> FrmImg[GetActiveVisn()]->OnPaintMsg();

}

void CVisionMan::SetWfrOri1WideInspection(bool _bWfrOri1WideInspection)
{
    m_bWfrOri1WideInspection = _bWfrOri1WideInspection ;
}

void CVisionMan::SetWfoOri1WideInspection(bool _bWfoOri1WideInspection)
{
    m_bWfoOri1WideInspection = _bWfoOri1WideInspection ;
}





