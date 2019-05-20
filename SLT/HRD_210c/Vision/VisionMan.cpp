//---------------------------------------------------------------------------


#pragma hdrstop

#include "VisionMan.h"
#include "Common.h"


#include "SVInterface.h"
#include "UserFile.h"
#include "SVMaths.h"
#include "VOptnMan.h"
#include "SLogUnit.h"
#include "UserFile.h"

#include "FormVisnMain.h"
#include "FormAlgoPara.h"
#include "FormVisnPara.h"





//---------------------------------------------------------------------------

#pragma package(smart_init)
CVisionMan VSM ;
AnsiString g_sJobFileName ;
AnsiString g_sJobFilePath ;

//정수 오브더 야매..... 누가 해결좀....
#include "FormVisnMain.h"
void __fastcall CVisionMan::tmYameTimer(TObject *Sender) //타이머 야매.
{
    if(!FrmVisnMain || !FrmVisnMain -> Showing) return ;
    tmYame -> Enabled = false ;
    AnsiString sBitmapPath ;
    static int i = 0 ; //MAX_VISN_ID - 1 ;



    VISN[i]->Img -> LoadBmp(GetExeDir("Util\\Images\\" + AnsiString(rand()%6)).c_str());
    VISN[i]->PaintCallback();
    sBitmapPath = g_sJobFilePath + VISN[i]->m_sVisnName + AnsiString(".bmp") ;
    VISN[i]->Img -> LoadBmp(sBitmapPath.c_str());
    FrmVisnMain -> FrmImg[i] -> SetInspCallbackFunc(&Inspect) ;
    VISN[i]->PaintCallback();
    if(VISN[i]->m_iYameRet == 0) {tmYame -> Enabled = true ; return ;}

    if(MAX_VISN_ID-1==i){
        SetActiveVisn(0);
        return ;

    }
    i++;

    tmYame -> Enabled = true ;
    return ;

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

#define RAIL_BEFORE_PKG_TSD_TRACKER_CNT 1
#define RAIL_BEFORE_DISP_TSD_TRACKER_CNT 4
#define WAFER_FAILCHIP_TSD_TRACKER_CNT 1
#define WAFER_NOCHIP_TSD_TRACKER_CNT   1

enum EN_INSP_ID {
    iiOriRot = 0 ,
    MAX_ORI_INSP ,    //자제 Rotation

    iiWfrPos = 0 ,    //자제 Position
    iiWfrFcp = 1 ,    //Fail Chip 검사.
    MAX_WFR_INSP ,

    iiRbfPkg = 0 ,    //자제 유무.
    iiRbfPos = 1 ,    //자제 포지션.
    iiRbfDsp = 2 ,    //자제 토출확인.
    MAX_RBF_INSP ,

    iiRatPos = 0 ,    //자제 포지션.
    iiRatChp = 1 ,    //칩 포지션.
    MAX_RAT_INSP ,

    iiWfoPos = 0 ,    //피듀셜 마크.
    MAX_WFO_INSP
};

bool CVisionMan::InitVisn(AnsiString _sDevName)
{
    LoadSVL("pSVL");

    Cam  [ciOri] = new CImi() ;
    Cam  [ciWfr] = new CEur() ;
    Cam  [ciRal] = new CEur() ;

    Light[liAll] = new CDkm() ;

    VISN[viOri] = new CVision(); VISN[viOri] -> Init("ORIENTATION" );
    VISN[viWfr] = new CVision(); VISN[viWfr] -> Init("WAFER CHIP"  );
    VISN[viRbf] = new CVision(); VISN[viRbf] -> Init("RAIL BF"     );
    VISN[viRat] = new CVision(); VISN[viRat] -> Init("RAIL AT"     );
    VISN[viWfo] = new CVision(); VISN[viWfo] -> Init("WAFER ORI"   );















    m_iInspectionId = 0 ;

    /*Orientation Vision*/
    EN_PROC_TYPE iPtOri     [MAX_ORI_INSP];
    int          iIntParaOri[MAX_ORI_INSP];
    iPtOri[iiOriRot] = ptOcv  ; iIntParaOri[iiOriRot] = 0 ;
    VISN[viOri  ]->ProcPkg.SetProcList(MAX_ORI_INSP , iPtOri , iIntParaOri) ;

    /*Wafer Vision*/
    EN_PROC_TYPE iPtWfr     [MAX_WFR_INSP];
    int          iIntParaWfr[MAX_WFR_INSP];
    iPtWfr[iiWfrPos] = ptWrt ; iIntParaWfr[iiWfrPos] = 0 ;
    iPtWfr[iiWfrFcp] = ptTsd ; iIntParaWfr[iiWfrFcp] = WAFER_FAILCHIP_TSD_TRACKER_CNT ;
    VISN[viWfr  ]->ProcPkg.SetProcList(MAX_WFR_INSP , iPtWfr , iIntParaWfr) ;

    /*Rail Before Vision*/
    EN_PROC_TYPE iPtRalBf     [MAX_RBF_INSP];
    int          iIntParaRalBf[MAX_RBF_INSP];
    iPtRalBf[iiRbfPkg] = ptTsd ; iIntParaRalBf[iiRbfPkg] = RAIL_BEFORE_PKG_TSD_TRACKER_CNT ;
    iPtRalBf[iiRbfPos] = ptLcp ; iIntParaRalBf[iiRbfPos] = 0 ;
    iPtRalBf[iiRbfDsp] = ptTsd ; iIntParaRalBf[iiRbfDsp] = RAIL_BEFORE_DISP_TSD_TRACKER_CNT ;
    VISN[viRbf   ]->ProcPkg.SetProcList(MAX_RBF_INSP , iPtRalBf , iIntParaRalBf) ;

    /*Rail After Vision*/
    EN_PROC_TYPE iPtRalAt     [MAX_RAT_INSP];
    int          iIntParaRalAt[MAX_RAT_INSP];
    iPtRalAt[iiRatPos] = ptLcp ; iIntParaRalAt[iiRatPos] = 0 ;
    iPtRalAt[iiRatChp] = ptRct ; iIntParaRalAt[iiRatChp] = 0 ;
    VISN[viRat   ]->ProcPkg.SetProcList(MAX_RAT_INSP , iPtRalAt , iIntParaRalAt) ;

    /*Wafer Fiducial Mark Vision*/
    EN_PROC_TYPE iPtWfo     [MAX_WFO_INSP];
    int          iIntParaWfo[MAX_WFO_INSP];
    //iPtOri[0] = ptOcv ;
    iPtOri[iiWfoPos] = ptOcv  ; iIntParaWfo[iiWfoPos] = 0 ;
    VISN[viWfo]->ProcPkg.SetProcList(MAX_WFO_INSP , iPtOri , iIntParaWfo) ;

    LoadJobFile(true ,_sDevName) ;


    //MakeWindows();


}

void CVisionMan::MakeWindows()
{
    FrmVisnMain = new TFrmVisnMain(NULL);
    FrmAlgoPara = new TFrmAlgoPara(FrmVisnMain);
    FrmVisnPara = new TFrmVisnPara(FrmVisnMain);

    FrmJob      = new TFrmJob     (FrmVisnMain);

    SetActiveVisn(0);





    //tmYame -> Enabled = true ;
}

void CVisionMan::DelWindows()
{
    //오너로 지정 되어 있거나 페어런트로 지정 되어 있으면 알아서 제거됨.
    //delete FrmJob     

    //delete FrmVisnPara ;
    //delete FrmAlgoPara ;

    //FrmVisnMain -> Parent = NULL ;
    delete FrmVisnMain ;
}


bool CVisionMan::SetVisnForm(void * _hWnd)
{
    //pOriParent = FrmVisnMain -> Parent ;
    if(_hWnd == NULL) return false ;
    m_hParentWnd = _hWnd ;

    static bool bFirst = true ;
    if(bFirst) {
        Application -> Handle = m_hParentWnd ;


        MakeWindows();



        //::SetParent(FrmVisnMain -> Handle , m_hParentWnd);
        FrmVisnMain -> ParentWindow = m_hParentWnd ;

        FrmVisnMain -> Init();
        FrmAlgoPara -> Init();

        //
        FrmVisnMain -> Show();

        //FrmVisnMain -> Align = alClient ;

        RECT   rt     ;
        //LPRECT lpRect ;
        GetClientRect(m_hParentWnd , &rt);

        FrmVisnMain -> Left = 0 ;
        FrmVisnMain -> Top  = 0 ;
        FrmVisnMain -> Width = rt.right  ;
        FrmVisnMain -> Height= rt.bottom ;

        tmYame = new TTimer(Application);
        tmYame -> Interval = 20 ;
        tmYame -> OnTimer = tmYameTimer ;
        tmYame -> Enabled = true ;


        bFirst = false ;
    }
    else {
        //::SetParent(FrmVisnMain -> Handle , m_hParentWnd);
        FrmVisnMain -> ParentWindow = m_hParentWnd ;

         FrmVisnMain -> Show();
        RECT   rt     ;
        //LPRECT lpRect ;
        GetClientRect(m_hParentWnd , &rt);

        FrmVisnMain -> Left = 0 ;
        FrmVisnMain -> Top  = 0 ;
        FrmVisnMain -> Width = rt.right ;
        FrmVisnMain -> Height= rt.bottom ;

        /*
        //FrmVisnMain -> Parent = _pBase ;
        FrmVisnMain -> ParentWindow = _hWnd ;

        FrmVisnMain -> Align = alClient ;
        FrmVisnMain -> Show();
        */
    }





/*
    static bool bFirst = true ;
    if(bFirst) {
        MakeWindows();

        FrmVisnMain -> ParentWindow = _hWnd ;
        FrmVisnMain -> Init();
        FrmAlgoPara -> Init();


        tmYame = new TTimer(Application);
        tmYame -> Interval = 20 ;
        tmYame -> OnTimer = tmYameTimer ;

        bFirst = false ;

    }

    FrmVisnMain -> ParentWindow = _hWnd ;

    LPRECT lpRect ;
    GetClientRect(_hWnd , lpRect);

    FrmVisnMain -> Left = 0 ;
    FrmVisnMain -> Top  = 0 ;
    FrmVisnMain -> Width = lpRect-> right ;
    FrmVisnMain -> Height= lpRect-> bottom ;

    FrmVisnMain -> Show();
*/
}

bool CVisionMan::DelVisn()
{
    //FrmVisnMain -> Parent = pOriParent ;
    //FrmVisnMain -> Align  = alNone ;
    DelWindows();

    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        VISN[i] -> Live(false);
    }
    Sleep(100); //혹시 그랩 터진직후면 이미지 들어오는시간 대기.

    for(int i = 0 ; i < MAX_VISN_ID ; i++) {

        VISN[i] -> close();
        delete VISN[i] ;
    }

    for(int i = 0 ; i < MAX_CAM_ID ; i++) {
        Cam[i]->Close();
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

}

void CVisionMan::Live (EN_VISN_ID _iVisnId , bool _bOn)
{
    VISN[_iVisnId]->Live(_bOn);
}

bool CVisionMan::GetGrabEnd(EN_VISN_ID _iVisnId )
{
    return VISN[_iVisnId]->GetGrabEnd();

}

int g_iSaveImgId = 0 ;
void CVisionMan::SaveImage()
{
    //옮겨담아놓는다.   쓰레드 돌릴때 삑사리 날까봐.
    int iSaveImgId = g_iSaveImgId ;
    AnsiString sSaveFolderPath , sSavePath;

         if(iSaveImgId == viOri) sSaveFolderPath = AnsiString("d:\\Image\\Ori\\");
    else if(iSaveImgId == viWfr) sSaveFolderPath = AnsiString("d:\\Image\\Wfr\\");
    else if(iSaveImgId == viRbf) sSaveFolderPath = AnsiString("d:\\Image\\RalBf\\");
    else if(iSaveImgId == viRat) sSaveFolderPath = AnsiString("d:\\Image\\RalAt\\");
    else if(iSaveImgId == viWfo) sSaveFolderPath = AnsiString("d:\\Image\\WfrOr\\");

    if(!UserFile.DirExists(sSaveFolderPath)) UserFile.CreateDir(sSaveFolderPath) ;

    sSavePath = sSaveFolderPath + Now().FormatString("yyyymmdd_hhnnss_zzz.bmp");
    VISN[iSaveImgId]->Img -> SaveBmp(sSavePath.c_str());
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
    WRT_Rslt WrtRslt ;

    AnsiString sTemp ;

    float fX , fY , fT ;

    bool bRet ;

    VISN[iInspId]->SetViewMode(vmAll) ;



    if(iInspId == viOri   ) {
        bRet = VISN[iInspId]->ProcPkg.Process(iiOriRot , VISN[iInspId]->Img , &OcvRslt);

        //스펙 확인.
        if(bRet == false ) {
            Rslt[iInspId].iRet   = veOriOcvFail ;
            sTemp = "1.FAIL OCV" ;
            //VISN[iInspId]->SetRsltMsg(false , "1.FAIL OCV");
            goto __GOTO_ORI_END__ ;
        }
        if(OcvRslt.fSinc < VOM.VDevOptn.fOriSinc) {

            Rslt[iInspId].iRet   = veOriOcv     ;
            sTemp.sprintf("2.NG OCV Sinc : %.2f" , OcvRslt.fSinc );
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG OCV Sinc : %.2f" , OcvRslt.fSinc ));
            goto __GOTO_ORI_END__ ;
        }

        fX = OcvRslt.iPosX - VISN[iInspId]->GetCenterX();
        fY = OcvRslt.iPosY - VISN[iInspId]->GetCenterY();

        Rslt[iInspId].fRsltX = fX * VISN[iInspId]->Cal->Rslt.fXPxRes ;
        Rslt[iInspId].fRsltY = fY * VISN[iInspId]->Cal->Rslt.fYPxRes ;

        if(Rslt[iInspId].iRet == veOriOk){
            sTemp.sprintf("OK Sinc : %.2f OfsX:%.2f , OfsY:%.2f" , OcvRslt.fSinc , Rslt[iInspId].fRsltX, Rslt[iInspId].fRsltY);
            //VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK Sinc : %.2f OfsX:%.2f , OfsY:%.2f" , OcvRslt.fSinc , Rslt[iInspId].fRsltX, Rslt[iInspId].fRsltY));
        }

        __GOTO_ORI_END__ : //----------------------------------------------------
        VISN[iInspId]->SetRsltMsg(Rslt[iInspId].iRet == veOriOk , sTemp);
        Rslt[iInspId].bInspEnd = true ;
        VISN[iInspId]->PaintCallback();

        if((VOM.VCmnOptn.iOriImageSave == 1 && Rslt[iInspId].iRet != 0) ||
           (VOM.VCmnOptn.iOriImageSave == 2 && Rslt[iInspId].iRet == 0) ||
           (VOM.VCmnOptn.iOriImageSave == 3                           ) ){
            g_iSaveImgId = iInspId ;
            CRunThread<Func> * func = new CRunThread<Func>(&SaveImage) ;
        }
    }


    else if(iInspId == viWfr   ) {
        Rslt[iInspId].fRsltX = 0.0 ;
        Rslt[iInspId].fRsltY = 0.0 ;
        Rslt[iInspId].fRsltT = 0.0 ;

        bool bWfrNeedRetest = false ;
        bool bWfrRetestFail = false ;

        RCT_Rslt RctRslt2 ;

        bRet = VISN[iInspId]->ProcPkg.Process(iiWfrPos , VISN[iInspId]->Img , &RctRslt);

        float fLength , fWid , fHei , fAngle , fAngL , fAngT , fAngR , fAngB , fAngMax , fAngMin;  //각변 결과값.
        //치수 픽셀값으로 환산.
        float fWfrWidthPx  = VOM.VDevOptn.fWfrWidth   / VISN[iInspId]->Cal->Rslt.fXPxRes ;
        float fWfrHeightPx = VOM.VDevOptn.fWfrHeight  / VISN[iInspId]->Cal->Rslt.fYPxRes ;
        float fWfrGapXPx   = VOM.VDevOptn.fWfrGap     / VISN[iInspId]->Cal->Rslt.fXPxRes ;
        float fWfrGapYPx   = VOM.VDevOptn.fWfrGap     / VISN[iInspId]->Cal->Rslt.fYPxRes ;

        float fX ;
        float fY ;
        float fT ;

        //리테스트 검사... 웨이퍼 빈자제 때문에 하는 거임...빈자제 있을때는 밝은물체 찾는거일때 어두운 물체 찾기로 바꾸면 어느정도 찾음.
        bWfrNeedRetest = !bRet ;
        fHei = MATH_GetLineLenth(RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y , RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y); //왼쪽변.
        if(fWfrHeightPx + fWfrGapYPx < fHei || fHei < fWfrHeightPx - fWfrGapYPx) bWfrNeedRetest = true ;
        fWid = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
        if(fWfrWidthPx  + fWfrGapXPx < fWid || fWid < fWfrWidthPx  - fWfrGapXPx) bWfrNeedRetest = true ;
        fHei = MATH_GetLineLenth(RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y , RctRslt.CrossPntRB.x , RctRslt.CrossPntRB.y); //오른쪽변
        if(fWfrHeightPx + fWfrGapYPx < fHei || fHei < fWfrHeightPx - fWfrGapYPx) bWfrNeedRetest = true ;
        fWid = MATH_GetLineLenth(RctRslt.CrossPntRB.x , RctRslt.CrossPntRB.y , RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y); //아래변
        if(fWfrWidthPx + fWfrGapXPx  < fWid || fWid < fWfrWidthPx  - fWfrGapXPx) bWfrNeedRetest = true ;

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
        if(VOM.VDevOptn.fWfrAngle < fAngMax - fAngMin) bWfrNeedRetest = true ; //가장삐투러진 +,-의 차를 구한다.

        //리테스트 옆에 자제 있는 빈자제때는 밝은물체 찾는거일때 어두운 물체 찾기로 바꾸면 어느정도 찾음.
        if(bWfrNeedRetest) {
            const int iBladeOffset = 3 ; //한쪽에 3픽셀 정도인데 양쪽에 해야해서 6픽셀.
            ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrPos]) -> Para.bLtOnDk = !((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrPos]) -> Para.bLtOnDk ;
            bWfrRetestFail = !VISN[iInspId]->ProcPkg.Process(iiWfrPos , VISN[iInspId]->Img , &RctRslt2);
            ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrPos]) -> Para.bLtOnDk = !((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrPos]) -> Para.bLtOnDk ;

            RctRslt2.CrossPntLB.x += iBladeOffset; RctRslt2.CrossPntLB.y -= iBladeOffset;
            RctRslt2.CrossPntLT.x += iBladeOffset; RctRslt2.CrossPntLT.y += iBladeOffset;
            RctRslt2.CrossPntRB.x -= iBladeOffset; RctRslt2.CrossPntRB.y -= iBladeOffset;
            RctRslt2.CrossPntRT.x -= iBladeOffset; RctRslt2.CrossPntRT.y += iBladeOffset;



            fHei = MATH_GetLineLenth(RctRslt2.CrossPntLB.x , RctRslt2.CrossPntLB.y , RctRslt2.CrossPntLT.x , RctRslt2.CrossPntLT.y) ; //왼쪽변.
            if(fWfrHeightPx + fWfrGapYPx < fHei || fHei < fWfrHeightPx - fWfrGapYPx) bWfrRetestFail = true ;
            fWid = MATH_GetLineLenth(RctRslt2.CrossPntLT.x , RctRslt2.CrossPntLT.y , RctRslt2.CrossPntRT.x , RctRslt2.CrossPntRT.y) ; //위쪽 변
            if(fWfrWidthPx  + fWfrGapXPx < fWid || fWid < fWfrWidthPx  - fWfrGapXPx) bWfrRetestFail = true ;
            fHei = MATH_GetLineLenth(RctRslt2.CrossPntRT.x , RctRslt2.CrossPntRT.y , RctRslt2.CrossPntRB.x , RctRslt2.CrossPntRB.y) ; //오른쪽변
            if(fWfrHeightPx + fWfrGapYPx < fHei || fHei < fWfrHeightPx - fWfrGapYPx) bWfrRetestFail = true ;
            fWid = MATH_GetLineLenth(RctRslt2.CrossPntRB.x , RctRslt2.CrossPntRB.y , RctRslt2.CrossPntLB.x , RctRslt2.CrossPntLB.y) ; //아래변
            if(fWfrWidthPx + fWfrGapXPx  < fWid || fWid < fWfrWidthPx  - fWfrGapXPx) bWfrRetestFail = true ;

            //각도 검사 최고 작은애랑 최고 큰애랑 격차를 비교 하여 넘어 가면 못찾은걸로 한다.
            fAngL = MATH_GetLineAngle(RctRslt2.LinRsltL.StartPnt.x , RctRslt2.LinRsltL.StartPnt.y , RctRslt2.LinRsltL.EndPnt.x   , RctRslt2.LinRsltL.EndPnt.y  ) - 270 ;
            fAngT = MATH_GetLineAngle(RctRslt2.LinRsltT.EndPnt.x   , RctRslt2.LinRsltT.EndPnt.y   , RctRslt2.LinRsltT.StartPnt.x , RctRslt2.LinRsltT.StartPnt.y) - 180 ;
            fAngR = MATH_GetLineAngle(RctRslt2.LinRsltR.StartPnt.x , RctRslt2.LinRsltR.StartPnt.y , RctRslt2.LinRsltR.EndPnt.x   , RctRslt2.LinRsltR.EndPnt.y  ) - 270 ;
            fAngB = MATH_GetLineAngle(RctRslt2.LinRsltB.EndPnt.x   , RctRslt2.LinRsltB.EndPnt.y   , RctRslt2.LinRsltB.StartPnt.x , RctRslt2.LinRsltB.StartPnt.y) - 180 ;
            fAngMin = fAngL ;
            if(fAngT < fAngMin) fAngMin = fAngT ;
            if(fAngR < fAngMin) fAngMin = fAngR ;
            if(fAngB < fAngMin) fAngMin = fAngB ;
            fAngMax = fAngL ;
            if(fAngT > fAngMax) fAngMax = fAngT ;
            if(fAngR > fAngMax) fAngMax = fAngR ;
            if(fAngB > fAngMax) fAngMax = fAngB ;
            if(VOM.VDevOptn.fWfrAngle < fAngMax - fAngMin) bWfrRetestFail = true ; //가장삐투러진 +,-의 차를 구한다.

            if(!bWfrRetestFail) { //리테스트 굿이면
                bRet = true ;
                memcpy(&RctRslt , &RctRslt2 , sizeof(RCT_Rslt));
                ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrPos]) -> Rslt = RctRslt; //blade offsets
            }
            else { //디스플레이 마지막 검사로 무조건 되기 때문에 디스플레이 복사함.
                ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrPos]) -> Rslt = RctRslt;
            }
        }

        if(bRet == false) {
            Rslt[iInspId].iRet = veWfrRctFail ;
            sTemp = "1.FAIL RCT" ;
            //VISN[iInspId]->SetRsltMsg(false , "1.FAIL RCT");
            goto __GOTO_WFR_END__ ;
        }

        fHei = MATH_GetLineLenth(RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y , RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y); //왼쪽변.
        if(fWfrHeightPx + fWfrGapYPx < fHei || fHei < fWfrHeightPx - fWfrGapYPx) {
            Rslt[iInspId].iRet = veWfrRct ;
            sTemp.sprintf("2.NG RCT Left Line : %.2f" , fHei* VISN[iInspId]->Cal->Rslt.fYPxRes);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Left Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fYPxRes) );
            goto __GOTO_WFR_END__ ;
        }

        fWid = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
        if(fWfrWidthPx + fWfrGapXPx < fWid || fWid < fWfrWidthPx - fWfrGapXPx) {
            Rslt[iInspId].iRet = veWfrRct ;
            sTemp.sprintf("2.NG RCT Top Line : %.2f" , fWid* VISN[iInspId]->Cal->Rslt.fXPxRes);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Top Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
            goto __GOTO_WFR_END__ ;
        }

        fHei = MATH_GetLineLenth(RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y , RctRslt.CrossPntRB.x , RctRslt.CrossPntRB.y); //오른쪽변
        if(fWfrHeightPx + fWfrGapYPx < fHei || fHei < fWfrHeightPx - fWfrGapYPx) {
            Rslt[iInspId].iRet = veWfrRct ;
            sTemp.sprintf("2.NG RCT Right Line : %.2f" ,  fHei* VISN[iInspId]->Cal->Rslt.fYPxRes);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Right Line : %.2f" ,  fLength* VISN[iInspId]->Cal->Rslt.fYPxRes));
            goto __GOTO_WFR_END__ ;
        }

        fWid = MATH_GetLineLenth(RctRslt.CrossPntRB.x , RctRslt.CrossPntRB.y , RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y); //아래변
        if(fWfrWidthPx + fWfrGapXPx < fWid || fWid < fWfrWidthPx - fWfrGapXPx) {
            Rslt[iInspId].iRet = veWfrRct ;
            sTemp.sprintf("2.NG RCT Bottom Line : %.2f" , fWid* VISN[iInspId]->Cal->Rslt.fXPxRes);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG RCT Bottom Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
            goto __GOTO_WFR_END__ ;
        }


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

        if(VOM.VDevOptn.fWfrAngle < fAngMax - fAngMin) { //가장삐투러진 +,-의 차를 구한다.
            Rslt[iInspId].iRet = veWfrAngle ;
            sTemp.sprintf("3.NG Angle Min Max Gap : %.2f" , fAngMax - fAngMin);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("3.NG Angle Min Max Gap : %.2f" , fAngMax - fAngMin));
            goto __GOTO_WFR_END__ ;
        }

        fX = VOM.VCmnOptn.bWfrFlipX ? -(RctRslt.CenterPnt.x - VISN[iInspId]->GetCenterX()) : (RctRslt.CenterPnt.x - VISN[iInspId]->GetCenterX()) ;
        fY = VOM.VCmnOptn.bWfrFlipY ? -(RctRslt.CenterPnt.y - VISN[iInspId]->GetCenterY()) : (RctRslt.CenterPnt.y - VISN[iInspId]->GetCenterY()) ;
        fT = VOM.VCmnOptn.bWfrFlipT ? -(RctRslt.fAngle     ) : (RctRslt.fAngle     ) ;


        Rslt[iInspId].fRsltX = fX * VISN[iInspId]->Cal->Rslt.fXPxRes ;
        Rslt[iInspId].fRsltY = fY * VISN[iInspId]->Cal->Rslt.fYPxRes ;
        Rslt[iInspId].fRsltT = fT ;


        //10프로미만 남고 다크랙이면 없는 자제..
        if( 90.0 < (RctRslt.iChkPxOutCnt * 100)/ RctRslt.iTtlChkPxCnt) {
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

        //트레커 오프셑 주는 거 넣기. 아.. 존나 짜증.   ]/////////////////
        float fGetX    , fGetY    , fGetT    ;
        float fGetOfsX , fGetOfsY , fGetOfsT ;
        float fOriX    , fOriY    ;
        float fSetX    , fSetY    ;
        float fSetOfsX , fSetOfsY ;
        VISN[iInspId]->ProcPkg.GetRefValOfs(iiWfrPos , fGetOfsX , fGetOfsY , fGetOfsT) ;
        VISN[iInspId]->ProcPkg.GetRefVal   (iiWfrPos , fGetX    , fGetY    , fGetT   ) ;

        fOriX = ((CTsd *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrFcp]) -> Tracker[0] -> Left  ;
        fOriY = ((CTsd *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrFcp]) -> Tracker[0] -> Top   ;
        MATH_GetRotPntFromCntrPnt(fOriX , fOriY , fGetOfsT , fGetX , fGetY , &fSetX , &fSetY);
        fSetX += fGetOfsX ;
        fSetY += fGetOfsY ;
        fSetOfsX = -(fOriX - fSetX) ;
        fSetOfsY = -(fOriY - fSetY) ;
        VISN[iInspId]->ProcPkg.SetInspOfs(iiWfrFcp, fSetOfsX , fSetOfsY);
        ////////////////////////////////////////////////////

        //가운데 점있는 Fail Device 검사.
        bRet = VISN[iInspId]->ProcPkg.Process(iiWfrFcp , VISN[iInspId]->Img , &TsdRslt);
        if(bRet == false) {
            Rslt[iInspId].iRet = veWfrTsdFail ;
            sTemp = "5.FAIL TSD" ;
            //VISN[iInspId]->SetRsltMsg(false , "5.FAIL TSD");
            goto __GOTO_WFR_END__ ;
        }

        for(int i = 0 ; i < WAFER_FAILCHIP_TSD_TRACKER_CNT ; i++) {
            ((CTsd *)VISN[iInspId]->ProcPkg.m_pProc[iiWfrFcp])->GetRslt(i , &TsdRslt);
            if(VOM.VDevOptn.iWfrFcpPxCnt && TsdRslt.iPxCnt > VOM.VDevOptn.iWfrFcpPxCnt){
               Rslt[iInspId].iRet = veWfrDot ;     //Fail Device.
               sTemp.sprintf("6.NG CHIP %d Pixel Cnt : %d", i ,TsdRslt.iPxCnt);
               //VISN[iInspId]->SetRsltMsg(false , "6.NG CHIP" + AnsiString(i) + " Pixel Cnt : " + AnsiString(TsdRslt.iPxCnt));
               goto __GOTO_WFR_END__ ;
            }
        }


        if(Rslt[iInspId].iRet == veWfrOk){
            fWid = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
            fHei = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y); //위쪽 변
            sTemp.sprintf("OK RCT Width : %.2f Height : %.2f" , fWid * VISN[iInspId]->Cal->Rslt.fXPxRes , fHei * VISN[iInspId]->Cal->Rslt.fYPxRes);
            //VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK RCT Top Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
        }

        __GOTO_WFR_END__ : //----------------------------------------------------
        if(bWfrNeedRetest) sTemp += "_R" ;
        VISN[iInspId]->SetRsltMsg(Rslt[iInspId].iRet == veWfrOk , sTemp);
        Rslt[iInspId].bInspEnd = true ;
        VISN[iInspId]->PaintCallback();

        if((VOM.VCmnOptn.iWfrImageSave == 1 && Rslt[iInspId].iRet != 0) ||
           (VOM.VCmnOptn.iWfrImageSave == 2 && Rslt[iInspId].iRet == 0) ||
           (VOM.VCmnOptn.iWfrImageSave == 3                           ) ){
            g_iSaveImgId = iInspId ;
            CRunThread<Func> * func = new CRunThread<Func>(&SaveImage) ;
        }
    }




    else if(iInspId == viRbf   ) {
        float fSumX  ;
        float fSumY  ;
        float fSumT  ;

        float fRsltX ;
        float fRsltY ;
        float fRsltT ;

        //여기서 트렉커 핸들링.
        //트레커 오프셑 주는 거 넣기. 아.. 존나 짜증.   ]/////////////////////
        float fGetX    , fGetY    , fGetT    ;
        float fGetOfsX , fGetOfsY , fGetOfsT ;
        float fOriX    , fOriY    ;
        float fSetX    , fSetY    ;
        float fSetOfsX  , fSetOfsY ;


        //자제 유무검사.
        bRet = VISN[iInspId]->ProcPkg.Process(iiRbfPkg , VISN[iInspId]->Img , &TsdRslt);
        if(bRet == false) {
            Rslt[iInspId].iRet = veRbfTsdFail ;
            sTemp = "1.FAIL TSD" ;
            goto __GOTO_RBF_END__ ;
        }

        for(int i = 0 ; i < RAIL_BEFORE_PKG_TSD_TRACKER_CNT ; i++) {
            ((CTsd *)VISN[iInspId]->ProcPkg.m_pProc[iiRbfPkg])->GetRslt(i , &TsdRslt);
            if(TsdRslt.fAverage > VOM.VDevOptn.iRlbPkgPxAvr){
               Rslt[iInspId].iRet = veRbfNoPkg ; //그냥 귀찮아서 에폭시 페일로 때림.
               sTemp.sprintf("2.NO PKG_%d Pixel Average : %.2f" ,i, TsdRslt.fAverage);
               //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NO PKG_%d Pixel Average : %.2f" ,i, TsdRslt.fAverage));
               goto __GOTO_RBF_END__ ;
            }
        }

        //라인 크로스 포인트 검사.
        bRet = VISN[iInspId]->ProcPkg.Process(iiRbfPos , VISN[iInspId]->Img , &LcpRslt);
        if(bRet == false) {
            Rslt[iInspId].iRet = veRbfLCPFail ;
            sTemp = "3.FAIL LINE CROSS PNT" ;
            //VISN[iInspId]->SetRsltMsg(false , "3.FAIL LINE CROSS PNT");
            goto __GOTO_RBF_END__ ;
        }

        fSumX = (VOM.VDevOptn.fRlbOffsetX / VISN[iInspId]->Cal->Rslt.fXPxRes) + LcpRslt.CrossPnt.x ;  //각도가 0일때 X,Y오프셑.
        fSumY = (VOM.VDevOptn.fRlbOffsetY / VISN[iInspId]->Cal->Rslt.fYPxRes) + LcpRslt.CrossPnt.y ;
        fSumT = LcpRslt.LinRslt2.fAngle ; //Line1이 270도 기준.


        //돌리기.
        if(VOM.VCmnOptn.bRlbNotUseAng){ //각도사용해서 X Y 오프셑 주는 것 사용 안함.
            fRsltX = fSumX ;
            fRsltY = fSumY ;
        }
        else {
            MATH_GetRotPntFromCntrPnt(fSumX , fSumY , fSumT , LcpRslt.CrossPnt.x , LcpRslt.CrossPnt.y , &fRsltX , &fRsltY);
        }

        //결과값 화면표시
        tPnt.x = fRsltX ;
        tPnt.y = fRsltY ;
        VISN[iInspId]->SetRsltPnt(tPnt);


        fRsltX = fRsltX - VISN[iInspId]->GetCenterX() ;
        fRsltY = fRsltY - VISN[iInspId]->GetCenterY() ;
        fRsltT = fSumT  ;

        fRsltX = VOM.VCmnOptn.bRlbFlipX ? -fRsltX : fRsltX ;
        fRsltY = VOM.VCmnOptn.bRlbFlipY ? -fRsltY : fRsltY ;
        fRsltT = VOM.VCmnOptn.bRlbFlipT ? -fRsltT : fRsltT ;

        Rslt[iInspId].fRsltX = fRsltX * VISN[iInspId]->Cal->Rslt.fXPxRes ;
        Rslt[iInspId].fRsltY = fRsltY * VISN[iInspId]->Cal->Rslt.fYPxRes ;
        Rslt[iInspId].fRsltT = fRsltT ;




        //트렉커 돌리기.
        VISN[iInspId]->ProcPkg.GetRefValOfs(iiRbfPos , fGetOfsX , fGetOfsY , fGetOfsT) ;
        VISN[iInspId]->ProcPkg.GetRefVal   (iiRbfPos , fGetX    , fGetY    , fGetT   ) ;
        fOriX = ((CTsd *)VISN[iInspId]->ProcPkg.m_pProc[iiRbfDsp]) -> Tracker[0] -> Left  ;
        fOriY = ((CTsd *)VISN[iInspId]->ProcPkg.m_pProc[iiRbfDsp]) -> Tracker[0] -> Top   ;
        MATH_GetRotPntFromCntrPnt(fOriX , fOriY , fGetOfsT , fGetX , fGetY , &fSetX , &fSetY);
        fSetX += fGetOfsX ;
        fSetY += fGetOfsY ;
        fSetOfsX = -(fOriX - fSetX) ;
        fSetOfsY = -(fOriY - fSetY) ;
        VISN[iInspId]->ProcPkg.SetInspOfs(iiRbfDsp, fSetOfsX , fSetOfsY);
        ////////////////////////////////////////////////////////////////////


        bRet = VISN[iInspId]->ProcPkg.Process(iiRbfDsp , VISN[iInspId]->Img , &TsdRslt);
        if(bRet == false) {
            Rslt[iInspId].iRet = veRbfTsdFail2 ;
            sTemp = "4.FAIL TSD Epoxy" ;
            //VISN[iInspId]->SetRsltMsg(false , "4.FAIL TSD Epoxy");
            goto __GOTO_RBF_END__ ;
        }

        for(int i = 0 ; i < RAIL_BEFORE_DISP_TSD_TRACKER_CNT ; i++) {
            ((CTsd *)VISN[iInspId]->ProcPkg.m_pProc[iiRbfDsp])->GetRslt(i , &TsdRslt);
            if(TsdRslt.iPxCnt < VOM.VDevOptn.iRlbPxCnt){
               Rslt[iInspId].iRet = veRbfEpoxy ;
               sTemp.sprintf("5.EPOXY NG_%d Pixel Average : %.2f", i , TsdRslt.fAverage);
               //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("5.EPOXY NG_%d Pixel Average : %.2f" , i , TsdRslt.fAverage));
               goto __GOTO_RBF_END__ ;
            }
        }

        if(Rslt[iInspId].iRet == veRbfOk){
            sTemp.sprintf("OK X : %.3f Y : %.3f T : %.2f" , Rslt[iInspId].fRsltX , Rslt[iInspId].fRsltY , Rslt[iInspId].fRsltT);
        }
        /*
        if(Rslt[iInspId].iRet == veRatOk){
            //fLength = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
            //sTemp =sTemp.sprintf("OK RCT Top Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes);
            fWid = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
            fHei = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y); //위쪽 변
            sTemp.sprintf("OK RCT Width : %.2f Height : %.2f" , fWid * VISN[iInspId]->Cal->Rslt.fXPxRes , fHei * VISN[iInspId]->Cal->Rslt.fYPxRes);


            //VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK RCT Top Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
            goto __GOTO_RAT_END__ ;
        }
        */

        __GOTO_RBF_END__ : ///////////------------------------------------------
        VISN[iInspId]->SetRsltMsg(Rslt[iInspId].iRet == veRbfOk , sTemp);
        Rslt[iInspId].bInspEnd = true ;
        VISN[iInspId]->PaintCallback();
        if((VOM.VCmnOptn.iRlbImageSave == 1 && Rslt[iInspId].iRet != 0) ||
           (VOM.VCmnOptn.iRlbImageSave == 2 && Rslt[iInspId].iRet == 0) ||
           (VOM.VCmnOptn.iRlbImageSave == 3                           ) ){
            g_iSaveImgId = iInspId ;
            CRunThread<Func> * func = new CRunThread<Func>(&SaveImage) ;
        }

    }
    else if(iInspId == viRat  ) {
        //여기서 트렉커 핸들링.
        //트레커 오프셑 주는 거 넣기. 아.. 존나 짜증.   ]/////////////////
        float fGetX    , fGetY    , fGetT    ;
        float fGetOfsX , fGetOfsY , fGetOfsT ;
        float fOriX    , fOriY    ;
        float fSetX    , fSetY    ;
        float fSetOfsX , fSetOfsY ;



        float fLength , fWid , fHei;
        //치수 픽셀값으로 환산.
        float fRalAtWidthPx  ;
        float fRalAtHeightPx ;
        float fRalAtGapXPx   ;
        float fRalAtGapYPx   ;

        float fSumX ;
        float fSumY ;
        float fSumT ;
        float fRotX ;
        float fRotY ;

        float fGapX ;
        float fGapY ;
        float fGapT ;

        float fGapXPx ;
        float fGapYPx ;

        AnsiString sTemp ;


        bRet = VISN[iInspId]->ProcPkg.Process(iiRatPos , VISN[iInspId]->Img , &LcpRslt);
        if(bRet == false ) {
            Rslt[iInspId].iRet = veRatLcpFail ;
            sTemp = "1.FAIL LINE CROSS PNT" ;
            //VISN[iInspId]->SetRsltMsg(false , "1.FAIL LINE CROSS PNT");
            goto __GOTO_RAT_END__ ;
        }

        //여기서 트렉커 핸들링.
        //트레커 오프셑 주는 거 넣기. 아.. 존나 짜증.   ]/////////////////
        VISN[iInspId]->ProcPkg.GetRefValOfs(iiRatPos , fGetOfsX , fGetOfsY , fGetOfsT) ;
        VISN[iInspId]->ProcPkg.GetRefVal   (iiRatPos , fGetX    , fGetY    , fGetT   ) ;
        fOriX = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiRatChp]) -> Tracker -> Left  ;
        fOriY = ((CRct *)VISN[iInspId]->ProcPkg.m_pProc[iiRatChp]) -> Tracker -> Top   ;
        MATH_GetRotPntFromCntrPnt(fOriX , fOriY , fGetOfsT , fGetX , fGetY , &fSetX , &fSetY);
        fSetX += fGetOfsX ;
        fSetY += fGetOfsY ;
        fSetOfsX = -(fOriX - fSetX) ;
        fSetOfsY = -(fOriY - fSetY) ;
        VISN[iInspId]->ProcPkg.SetInspOfs(iiRatChp, fSetOfsX , fSetOfsY);
        ////////////////////////////////////////////////////

        bRet = VISN[iInspId]->ProcPkg.Process(iiRatChp , VISN[iInspId]->Img , &RctRslt);
        if(bRet == false){
            Rslt[iInspId].iRet = veRatRctFail ;
            sTemp = "2.FAIL RCT" ;
            //VISN[iInspId]->SetRsltMsg(false , "2.FAIL RCT");
            goto __GOTO_RAT_END__ ;
        }

        //치수 픽셀값으로 환산.
        fRalAtWidthPx  = VOM.VDevOptn.fRlaWidth  / VISN[iInspId]->Cal->Rslt.fXPxRes ;
        fRalAtHeightPx = VOM.VDevOptn.fRlaHeight / VISN[iInspId]->Cal->Rslt.fYPxRes ;
        fRalAtGapXPx   = VOM.VDevOptn.fRlaGap    / VISN[iInspId]->Cal->Rslt.fXPxRes ;
        fRalAtGapYPx   = VOM.VDevOptn.fRlaGap    / VISN[iInspId]->Cal->Rslt.fYPxRes ;

        fLength = MATH_GetLineLenth(RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y , RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y); //왼쪽변.
        if(fRalAtHeightPx + fRalAtGapYPx < fLength || fLength < fRalAtHeightPx - fRalAtGapYPx) {
            Rslt[iInspId].iRet = veRatLen ;
            sTemp = "3.NG RCT Left Line : " + AnsiString(fLength* VISN[iInspId]->Cal->Rslt.fYPxRes) ;
            //VISN[iInspId]->SetRsltMsg(false , "3.NG RCT Left Line : " + AnsiString(fLength* VISN[iInspId]->Cal->Rslt.fYPxRes));
            goto __GOTO_RAT_END__ ;
        }

        fLength = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
        if(fRalAtWidthPx + fRalAtGapXPx < fLength || fLength < fRalAtWidthPx - fRalAtGapXPx) {
            Rslt[iInspId].iRet = veRatLen ;
            sTemp ="3.NG RCT Top Line : " + AnsiString(fLength* VISN[iInspId]->Cal->Rslt.fXPxRes) ;
            VISN[iInspId]->SetRsltMsg(false , "3.NG RCT Top Line : " + AnsiString(fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
            goto __GOTO_RAT_END__ ;
        }

        fLength = MATH_GetLineLenth(RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y , RctRslt.CrossPntRB.x , RctRslt.CrossPntRB.y); //오른쪽변
        if(fRalAtHeightPx + fRalAtGapYPx < fLength || fLength < fRalAtHeightPx - fRalAtGapYPx) {
            Rslt[iInspId].iRet = veRatLen ;
            sTemp ="3.NG RCT Right Line : " + AnsiString(fLength* VISN[iInspId]->Cal->Rslt.fYPxRes) ;
            VISN[iInspId]->SetRsltMsg(false , "3.NG RCT Right Line : " + AnsiString(fLength* VISN[iInspId]->Cal->Rslt.fYPxRes));
            goto __GOTO_RAT_END__ ;
        }

        fLength = MATH_GetLineLenth(RctRslt.CrossPntRB.x , RctRslt.CrossPntRB.y , RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y); //아래변
        if(fRalAtWidthPx + fRalAtGapXPx < fLength || fLength < fRalAtWidthPx - fRalAtGapXPx) {
            Rslt[iInspId].iRet = veRatLen ;
            sTemp ="3.NG RCT Bottom Line : " + AnsiString(fLength* VISN[iInspId]->Cal->Rslt.fXPxRes);
            VISN[iInspId]->SetRsltMsg(false , "3.NG RCT Bottom Line : " + AnsiString(fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
            goto __GOTO_RAT_END__ ;
        }




                       //그냥 비포껄로 통일하여 쓴다.
        fSumX = (/*VOM.VDevOptn.fRlaOffsetX*/VOM.VDevOptn.fRlbOffsetX / VISN[iInspId]->Cal->Rslt.fXPxRes) + LcpRslt.CrossPnt.x ;  //각도가 0일때 X,Y오프셑.
        fSumY = (/*VOM.VDevOptn.fRlaOffsetY*/VOM.VDevOptn.fRlbOffsetY / VISN[iInspId]->Cal->Rslt.fYPxRes) + LcpRslt.CrossPnt.y ;
        fSumT = LcpRslt.LinRslt2.fAngle ; //Line1이 270도 기준.

        //돌리기.
        if(VOM.VCmnOptn.bRlbNotUseAng){ //각도사용해서 X Y 오프셑 주는 것 사용 안함.
            fRotX = fSumX ;
            fRotY = fSumY ;
        }
        else {
            MATH_GetRotPntFromCntrPnt(fSumX , fSumY , fSumT , LcpRslt.CrossPnt.x , LcpRslt.CrossPnt.y , &fRotX , &fRotY);
            //MATH_GetRotPntFromCntrPnt(fSumX , fSumY , fSumT , LcpRslt.CrossPnt.x , LcpRslt.CrossPnt.y , &fRsltX , &fRsltY);
        }


        //결과값 화면표시
        tPnt.x = fRotX ;
        tPnt.y = fRotY ;
        VISN[iInspId]->SetRsltPnt(tPnt);

        fGapX = RctRslt.CenterPnt.x - fRotX ; //크로스포인트와 칩센터간의 X거리.
        fGapY = RctRslt.CenterPnt.y - fRotY ; //크로스포인트와 칩센터간의 Y거리.
        fGapT = RctRslt.fAngle - LcpRslt.LinRslt2.fAngle ; //Line2이 0도 기준.

        Rslt[iInspId].fRsltX = fGapX * VISN[iInspId]->Cal->Rslt.fXPxRes;
        Rslt[iInspId].fRsltY = fGapY * VISN[iInspId]->Cal->Rslt.fYPxRes;
        Rslt[iInspId].fRsltT = fGapT ;

        fGapXPx = VOM.VDevOptn.fRlaOffsetGap / VISN[iInspId]->Cal->Rslt.fXPxRes ;
        fGapYPx = VOM.VDevOptn.fRlaOffsetGap / VISN[iInspId]->Cal->Rslt.fYPxRes ;

        if(fabs(fGapX) > fGapXPx ) {
            Rslt[iInspId].iRet = veRatRctOfs ;
            sTemp.sprintf("4.NG RCT OffsetX : %.2f" , fGapX * VISN[iInspId]->Cal->Rslt.fXPxRes) ;
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("4.NG RCT OffsetX : %.2f" , fGapX * VISN[iInspId]->Cal->Rslt.fXPxRes));
            goto __GOTO_RAT_END__ ;
        }
        if(fabs(fGapY) > fGapYPx ) {
            Rslt[iInspId].iRet = veRatRctOfs ;
            sTemp.sprintf("4.NG RCT OffsetY : %.2f" , fGapY * VISN[iInspId]->Cal->Rslt.fYPxRes);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("4.NG RCT OffsetY : %.2f" , fGapY * VISN[iInspId]->Cal->Rslt.fYPxRes));
            goto __GOTO_RAT_END__ ;
        }


        Rslt[iInspId].iRet == veRatOk ? sTemp = "GOOD" : sTemp = "NG  " ;
        sTemp += AnsiString(" X=")+ Rslt[iInspId].fRsltX + AnsiString(" Y=") + Rslt[iInspId].fRsltY + AnsiString(" T=") + fGapT ;
        Trace("AfterVisn",sTemp.c_str());


        if(fGapT < VOM.VDevOptn.fRlaAngle - VOM.VDevOptn.fRlaAngleGap  || fGapT > VOM.VDevOptn.fRlaAngle   + VOM.VDevOptn.fRlaAngleGap ) {
            Rslt[iInspId].iRet = veRatRctAng ;
            sTemp = "5.NG RCT Angle : " + AnsiString(fGapT) ;
            //VISN[iInspId]->SetRsltMsg(false , "5.NG RCT Angle : " + AnsiString(fGapT));
            goto __GOTO_RAT_END__ ;
        }

        if(Rslt[iInspId].iRet == veRatOk){
            //fLength = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
            //sTemp =sTemp.sprintf("OK RCT Top Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes);
            fWid = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //위쪽 변
            fHei = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y); //위쪽 변
            sTemp.sprintf("OK RCT Width : %.2f Height : %.2f" , fWid * VISN[iInspId]->Cal->Rslt.fXPxRes , fHei * VISN[iInspId]->Cal->Rslt.fYPxRes);


            //VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK RCT Top Line : %.2f" , fLength* VISN[iInspId]->Cal->Rslt.fXPxRes));
            goto __GOTO_RAT_END__ ;
        }


        __GOTO_RAT_END__ : ///////////------------------------------------------
        VISN[iInspId]->SetRsltMsg(Rslt[iInspId].iRet == veRatOk , sTemp);
        Rslt[iInspId].bInspEnd = true ;
        VISN[iInspId]->PaintCallback();
        if((VOM.VCmnOptn.iRlaImageSave == 1 && Rslt[iInspId].iRet != 0) ||
           (VOM.VCmnOptn.iRlaImageSave == 2 && Rslt[iInspId].iRet == 0) ||
           (VOM.VCmnOptn.iRlaImageSave == 3                           ) ){
            g_iSaveImgId = iInspId ;
            CRunThread<Func> * func = new CRunThread<Func>(&SaveImage) ;
        }

    }

    if(iInspId == viWfo   ) {
        float fX ;
        float fY ;

        bRet = VISN[iInspId]->ProcPkg.Process(iiWfoPos , VISN[iInspId]->Img , &OcvRslt);

        //스펙 확인.
        if(bRet == false ) {
            Rslt[iInspId].iRet   = veWfoOcvFail     ;
            sTemp = "1.FAIL OCV" ;
            //VISN[iInspId]->SetRsltMsg(false , "1.FAIL OCV");
            goto __GOTO_WFO_END__ ;
        }
        if(OcvRslt.fSinc < VOM.VDevOptn.fWfoSinc) {
            Rslt[iInspId].iRet   = veWfoOcv     ;
            sTemp.sprintf("2.NG OCV Sinc : %.2f" ,OcvRslt.fSinc);
            //VISN[iInspId]->SetRsltMsg(false , sTemp.sprintf("2.NG OCV Sinc : %.2f" ,OcvRslt.fSinc));
            goto __GOTO_WFO_END__ ;
        }

        fX = VOM.VCmnOptn.bWfoFlipX ? -(OcvRslt.iPosX - VISN[iInspId]->GetCenterX()) : (OcvRslt.iPosX - VISN[iInspId]->GetCenterX()) ;
        fY = VOM.VCmnOptn.bWfoFlipY ? -(OcvRslt.iPosY - VISN[iInspId]->GetCenterY()) : (OcvRslt.iPosY - VISN[iInspId]->GetCenterY()) ;

        Rslt[iInspId].fRsltX = fX * VISN[iInspId]->Cal->Rslt.fXPxRes ;
        Rslt[iInspId].fRsltY = fY * VISN[iInspId]->Cal->Rslt.fYPxRes ;

        if(Rslt[iInspId].iRet == veWfoOk){
            sTemp.sprintf("OK Sinc : %.2f  Offset : %.2f , %.2f" ,OcvRslt.fSinc ,  Rslt[iInspId].fRsltX , Rslt[iInspId].fRsltY) ;
            //VISN[iInspId]->SetRsltMsg(true , sTemp.sprintf("OK Offset : %.2f , %.2f" , Rslt[iInspId].fRsltX , Rslt[iInspId].fRsltY));
        }

        __GOTO_WFO_END__ : ///////////------------------------------------------
        VISN[iInspId]->SetRsltMsg(Rslt[iInspId].iRet == veWfoOk , sTemp);
        Rslt[iInspId].bInspEnd = true ;
        VISN[iInspId]->PaintCallback();
        if((VOM.VCmnOptn.iWfoImageSave == 1 && Rslt[iInspId].iRet != 0) ||
           (VOM.VCmnOptn.iWfoImageSave == 2 && Rslt[iInspId].iRet == 0) ||
           (VOM.VCmnOptn.iWfoImageSave == 3                           ) ){
            g_iSaveImgId = iInspId ;
            CRunThread<Func> * func = new CRunThread<Func>(&SaveImage) ;
        }
    }
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

void CVisionMan::SetLevel(EN_OPER_LEVEL _iLevel)
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
    FrmVisnMain -> pcVisnBase -> ActivePageIndex = _iVisnId ;
}








