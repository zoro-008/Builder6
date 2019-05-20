//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "MarkVisn.h"
#include "Rail.h"
#include "RunThread.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"

#include "PstnMan.h"
#include "OptionMan.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CMarkVisn VSN;

bool g_bMarkChangeFinish = true;

CMarkVisn::CMarkVisn(void)
{

}

CMarkVisn::~CMarkVisn (void)
{

}

//�׳� ���⼭ Vision  Reset�� 3���� ����.

bool CMarkVisn::SaveVisnDevice(AnsiString _sDevice)
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sData       ;
    int        sState      ;

    //Set Path.

    sPath = OM.MstOptn.sVisnDeviceFile ;
    sData = _sDevice;

    //���� �ձ� ����.
    if (!FileExists(sPath)) CreateDir(sPath);

    //���� �ִ����� �����.
    if (FileExists(sPath)) DeleteFile(sPath) ;
    hFile = FileCreate(sPath.c_str());
    if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }

    //AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'�� 'n'��' s'�� \r\n'");
    FileSeek (hFile , 0                , SEEK_END         );
    FileWrite(hFile , _sDevice.c_str() , _sDevice.Length());

    //Close File.
    FileClose(hFile);

    return true;
}

typedef void ( *Func )();
void ThreadVisnReset()
{


    static bool bAlreadyDoing = false ;
    CDelayTimer Timer ;
    CDelayTimer RetryTimer ;

    if(bAlreadyDoing) return ;



    bAlreadyDoing = true ;



    int iStep    = 10 ;
    int iPreStep = 0  ;
    int iSendCnt = 0  ;

    while(1) {
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (Timer.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 10000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiVSN_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("Vision Communction",sTemp.c_str());
            IO_SetY(yVSC_Reset , false) ;
            bAlreadyDoing = false ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("Vision Communction",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("Vision Communction", sTemp.c_str());
                      IO_SetY(yVSC_Reset , false) ;
                      bAlreadyDoing = false ;
                      iStep = 0 ;
                      return ;

            case  10: //iSendCnt = 0 ;
                      IO_SetY(yVSC_Reset , false) ;
                      if(iSendCnt > 1) {//�����ʿ� ���� ��Ǯ���� �ѹ� �� ������.
                          EM_SetErr(eiVSN_TimeOut);
                          bAlreadyDoing = false ;
                          iStep = 0 ;
                          return ;
                      }
                      iStep++;
                      continue ;

            case  11: IO_SetY(yVSC_Reset , true ) ;
                      RetryTimer.Clear();
                      iStep++;
                      continue ;

            case  12: if(RetryTimer.OnDelay(true , 2000)){
                          IO_SetY(yVSC_Reset , false) ;
                          iStep = 10 ;
                          iSendCnt++;
                          continue ;
                      }

                      if(IO_GetX(xVSC_OriBusy)) continue ;
                      if(IO_GetX(xVSC_AlnBusy)) continue ;
                      if(IO_GetX(xVSC_MrkBusy)) continue ;

                      IO_SetY(yVSC_Reset , false ) ;
                      bAlreadyDoing = false ;
                      iStep = 0 ;
                      return ;
        }
    }
}

void ThreadVisnJobChange()
{
    g_bMarkChangeFinish = false ;

    CDelayTimer Timer ;


    int iStep    = 10 ;
    int iPreStep = 0  ;

    while(1) {
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (Timer.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 10000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiVSN_TimeOut);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("Vision Communction",sTemp.c_str());
            IO_SetY(yVSC_JobChange , false) ;
            g_bMarkChangeFinish = true ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("Vision Communction",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("Vision Communction", sTemp.c_str());
                      IO_SetY(yVSC_JobChange , false) ;
                      iStep = 0 ;
                      return ;

            case  10: IO_SetY(yVSC_JobChange , false) ;
                      iStep++;
                      continue ;

            case  11: if(IO_GetX(xVSC_AlnBusy)) continue ;
                      if(IO_GetX(xVSC_MrkBusy)) continue ;
                      if(IO_GetX(xVSC_OriBusy)) continue ;


                      IO_SetY(yVSC_JobChange , true ) ;
                      iStep++;
                      continue ;

            case  12: //if(!IO_GetX(xVSC_AlnBusy)) continue ;
                      if(!IO_GetX(xVSC_MrkBusy)) continue ;
                      //if(!IO_GetX(xVSC_OriBusy)) continue ;

                      IO_SetY(yVSC_JobChange , false ) ;
                      iStep++;
                      continue ;

            case  13: //if(IO_GetX(xVSC_AlnBusy)) continue ;
                      if(IO_GetX(xVSC_MrkBusy)) continue ;
                      //if(IO_GetX(xVSC_OriBusy)) continue ;
                      g_bMarkChangeFinish = true ;
                      iStep = 0 ;
                      return ;
        }
    }
}

bool CMarkVisn::VisnJobChange()
{
    if(!SaveVisnDevice(OM.GetCrntDev())) {
        FM_MsgOk("ERR","Vision Jobfile ���� ����");
        return false ;
    }
    CRunThread<Func> * func = new CRunThread<Func>(&ThreadVisnJobChange) ;
}

void CMarkVisn::Init()
{
    m_sPartName = "MarkVisn " ;
    Reset();
    Load(true);

    InitCycleName();
}
void CMarkVisn::Close()
{
    Load(false);
}

void CMarkVisn::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    //��� ������ ��ũ�������� ������.
    if((IO_GetX(xVSC_OriBusy) && !OM.CmnOptn.bSkipOriVisn) ||
       (IO_GetX(xVSC_AlnBusy) && !OM.CmnOptn.bSkipAlnVisn) ||
       (IO_GetX(xVSC_MrkBusy) &&  OM.CmnOptn.iMarkInsp != moNone) ){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadVisnReset) ;
    }
}

void CMarkVisn::ResetTimer()
{
    //Clear Timer.
    m_tmMain    .Clear();
    m_tmCycle   .Clear();
    m_tmHome    .Clear();
    m_tmToStop  .Clear();
    m_tmToStart .Clear();
    m_tmDelay   .Clear();
}

bool CMarkVisn::GetLtToRt()
{
    int r, c ;
    bool bFindChip = FindChip(riVSN , r , c ) ;
    bool bLeftToRight ;

    if(OM.CmnOptn.iMarkInsp == moOneRow) { //���� �˻�� ������ ���ʿ�������.
        bLeftToRight  =true ;

    }
    else { //��� �˻� �϶��� ���� ���.
        bLeftToRight = !(r % 2) ;
    }

    return bLeftToRight ;
}

bool CMarkVisn::FindChip(EN_ARAY_ID _iAray , int &r , int &c )
{
//�������� �����˻� ���������� �����˻�
//Unkwn -> Mask     Work       Good
//                             Fail
    //r = DM.ARAY[_iAray].FindFrstRow(csWork) ;
    //r = DM.ARAY[_iAray].FindFrstRow(csWait) ;
    DM.ARAY[_iAray].FindFrstRowCol(csWait , r , c) ;
    return (r > -1);
}

double CMarkVisn::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;
    double dPickPos = 0.0 ;
    int r,c ;

    bool bFindChip = FindChip(riVSN , r , c ) ;
    if(!bFindChip){
        r = 0 ;
        c = 0 ;
    }


    const double dOfsforTrg = 5.0 ; //���� ���ͷ�Ʈ Ʈ���Ÿ� ���� �əV.
    double dRowGrPtchGap = OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch ;
    double dColGrPtchGap = OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ;

    int iColGr = OM.DevInfo.iColGrCnt ;
    int iRowGr = OM.DevInfo.iRowGrCnt ;
    if(iColGr==0) iColGr=1;
    if(iRowGr==0) iRowGr=1;
    int    iColGrCnt = OM.DevInfo.iColCnt / iColGr;
    int    iRowGrCnt = OM.DevInfo.iRowCnt / iRowGr;

    //tpstj
    double dYPos = PM.GetValue(miVSN_YInsp , pvVSN_YInspWorkStt ) + //ù������ ���ϰ�.
                   r * OM.DevInfo.dRowPitch +    //�˻翭�� �����ؼ� �ش� �ο� ���� �Ÿ�����ؼ� ���ϰ�.
                   (OM.DevInfo.iRowGrCnt ? dRowGrPtchGap * (r / iRowGrCnt) : 0);
                   //(OM.DevInfo.iRowGrCnt?(r)/OM.DevInfo.iRowGrCnt : 0 ) ;//�׷� ī��Ʈ�� ������ ���.

    double dLeftPos  = PM.GetValue(miVSN_XInsp , pvVSN_XInspWorkStt) + dOfsforTrg ;

    double dRightPos = PM.GetValue(miVSN_XInsp , pvVSN_XInspWorkStt) -  //ù������ ��.
                       OM.DevInfo.dColPitch * OM.DevInfo.iColCnt -      //��ī��Ʈ�� �����ؼ� ������ �ο� ���� �Ÿ�����ؼ� ��.
                       (OM.DevInfo.iColGrCnt ? dColGrPtchGap * iColGrCnt : 0) - dOfsforTrg;
                       //dColGrPtchGap * (c / OM.DevInfo.iColGrCnt) - dOfsforTrg;

                       //(OM.DevInfo.iColGrCnt ? (OM.DevInfo.iColGrCnt - 1) : 0) - //�׷� �� ��.
                       //- dOfsforTrg ;


    //��ŸƮ �����ǰ� ���� ������ ����.
    double dXSttPos ;
    double dXEndPos ;
    if(GetLtToRt()) {
        dXSttPos = dLeftPos  ;
        dXEndPos = dRightPos ;
    }
    else {
        dXSttPos = dRightPos ;
        dXEndPos = dLeftPos  ;
    }

    /* ������ �ƿ� ������ ������ ���� ������ �����Ǽ� �����ؼ� �׳� ��.
    double dWaitPosX , dWaitPosY ;

    if(bFindChip) {
        dWaitPosX = dXSttPos ;
        dWaitPosY = dYPos    ;
    }
    else {
        if(OM.CmnOptn.iMarkInsp == moAllRow) {
            dWaitPosX = dXSttPos ;
            dWaitPosY = dYPos    ;
        }
        else if(OM.CmnOptn.iMarkInsp == moOneRow) {
            dWaitPosX = dLeftPos ;
            dWaitPosY = MT_GetCmdPos(miVSN_YInsp);
        }
        else {
            dWaitPosX = PM.GetValue (miVSN_XInsp , pvVSN_XInspWait    );
            dWaitPosY = PM.GetValue (miVSN_YInsp , pvVSN_YInspWait    );
        }
    }
    */

    if(_iMotr == miVSN_XInsp){
        switch(_iPstnId) {
            default                   : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piVSN_XInspWait      : dPos = PM.GetValue (_iMotr , pvVSN_XInspWait     ); break ;
            case piVSN_XInspWorkStt   : dPos = PM.GetValue (_iMotr , pvVSN_XInspWorkStt  ); break ;
            case piVSN_XInspMskChange : dPos = PM.GetValue (_iMotr , pvVSN_XInspMskChange); break ;
            case piVSN_XInspTrgStt    : dPos = dXSttPos                                   ; break ;
            case piVSN_XInspTrgEnd    : dPos = dXEndPos                                   ; break ;

            //case piVSN_XInspWorkWait: dPos = dWaitPosX                                 ; break ;
        }
    }
    else if(_iMotr == miVSN_YInsp){
        switch(_iPstnId) {
            default                   : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piVSN_YInspWait      : dPos = PM.GetValue (_iMotr , pvVSN_XInspWait     ); break ;
            case piVSN_YInspWorkStt   : dPos = PM.GetValue (_iMotr , pvVSN_YInspWorkStt  ); break ;
            case piVSN_YInspMskChange : dPos = PM.GetValue (_iMotr , pvVSN_YInspMskChange); break ;
            case piVSN_YInspTrg       : dPos = dYPos                                      ; break ;

            //case piVSN_YInspWait   : dPos = PM.GetValue (_iMotr , pvVSN_YInspWait    ); break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CMarkVisn::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miVSN_XInsp){
        switch(_iPstnId) {
            default                   :  break ;
            case piVSN_XInspWait      :  break ;
            case piVSN_XInspWorkStt   :  break ;
            case piVSN_XInspMskChange :  break ;
            case piVSN_XInspTrgStt    :  break ;
            case piVSN_XInspTrgEnd    :  break ;

            //case piVSN_XInspWorkWait:  break ;
        }
    }
    else if(_iMotr == miVSN_YInsp){
        switch(_iPstnId) {
            default                   :  break ;
            case piVSN_YInspWait      :  break ;
            case piVSN_YInspWorkStt   :  break ;
            case piVSN_YInspMskChange :  break ;
            case piVSN_YInspTrg       :  break ;

        }
    }
    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CMarkVisn::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    bool bRet = true ;
    AnsiString sMsg ;

    //if(aiPSB_RailOpCl){}
    //else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    //}

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CMarkVisn::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow ) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }
}

bool CMarkVisn::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CMarkVisn::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiVSN_HomeTo);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    //if(Stat.bReqStop) {
    //    Step.iHome = 0;
    //    return true ;
    //}

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_SetHomeEnd(miVSN_XInsp , false);
                 MT_SetHomeEnd(miVSN_YInsp , false);
                 MT_DoHome(miVSN_XInsp);
                 MT_DoHome(miVSN_YInsp);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miVSN_XInsp)) return false;
                 if(!MT_GetHomeEnd(miVSN_YInsp)) return false;
                 MT_GoAbsMan(miVSN_XInsp, PM.GetValue(miVSN_XInsp , pvVSN_XInspWait));
                 MT_GoAbsMan(miVSN_YInsp, PM.GetValue(miVSN_YInsp , pvVSN_YInspWait));
                 Step.iHome++ ;
                 return false ;

        case 12: if(!MT_GoAbsMan(miVSN_XInsp, PM.GetValue(miVSN_XInsp , pvVSN_XInspWait))) return false ;
                 if(!MT_GoAbsMan(miVSN_YInsp, PM.GetValue(miVSN_YInsp , pvVSN_YInspWait))) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

bool CMarkVisn::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{

    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if (Step.iSeq == 0) {
        if(Stat.bReqStop)return false ;

        bool isCycleTrain =  AT_Complete(aiRAL_VStnUpDn , ccFwd) &&  DM.ARAY[riVSN].GetCntStat(csWait) &&  DM.ARAY[riVSN].GetLotNo() != m_sTrainedLot && OM.CmnOptn.iMarkInsp != moNone;
        bool isCycleVisn  =  AT_Complete(aiRAL_VStnUpDn , ccFwd) &&  !isCycleTrain && DM.ARAY[riVSN].GetCntStat(csWait) ;
        bool isCycleEnd   =  DM.ARAY[riVSN].CheckAllStat(csNone) && RAL.GetWorkEnd();

        //Unknown Strip
        /*
        if(isCycleTrain || isCycleVisn) {
            if (IO_GetX(xRAL_VPkg4) && DM.ARAY[riVSN].CheckAllStat(csNone)){
                 EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riVSN].GetName().c_str()) ;
            }

            //Dissapear Strip
            if (!OM.MstOptn.bDryRun) {
                if (!IO_GetX(xRAL_VPkg4) && !DM.ARAY[riVSN].CheckAllStat(csNone)) EM_SetErrMsg(eiPKG_Dispr , DM.ARAY[riVSN].GetName().c_str()) ;
            }
        }
        */
        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleTrain) {Trace(m_sPartName.c_str(),"CycleTrain Stt"); Step.iSeq = scTrain ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleVisn ) {Trace(m_sPartName.c_str(),"CycleVisn  Stt"); Step.iSeq = scVisn  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd  ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                   Trace(m_sPartName.c_str(),"default    End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                     return false ;
        case scTrain : if(CycleTrain()){ Trace(m_sPartName.c_str(),"CycleTrain End");Step.iSeq = scIdle ;} return false ;
        case scVisn  : if(CycleVisn ()){ Trace(m_sPartName.c_str(),"CycleVisn  End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CMarkVisn::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CMarkVisn::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CMarkVisn::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str());

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    bool bExstUnKnown = false ;

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10:

                 IO_SetY(yVSC_MrkStart  , false);
                 IO_SetY(yVSC_Reset     , false);
                 IO_SetY(yVSC_AutoTrain , false);
                 IO_SetY(yVSC_JobChange , false);
                 IO_SetY(yVSC_Spear3    , false);




                 MoveMotr(miVSN_XInsp , piVSN_XInspWait);
                 MoveMotr(miVSN_YInsp , piVSN_YInspWait);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveMotr(miVSN_XInsp , piVSN_XInspWait)) return false ;
                 if(!MoveMotr(miVSN_YInsp , piVSN_YInspWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CMarkVisn::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiVSN_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: IO_SetY(yVSC_AutoTrain , false);
                 MoveMotr(miVSN_XInsp , piVSN_XInspWait);
                 MoveMotr(miVSN_YInsp , piVSN_YInspWait);
                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveMotr(miVSN_XInsp , piVSN_XInspWait)) return false ;
                 if(!MoveMotr(miVSN_YInsp , piVSN_YInspWait)) return false ;

                 Step.iToStop = 0 ;
                 return true ;
    }
}


void CMarkVisn::SetTrgPos()
{
    int iTrgCnt      = OM.DevInfo.iColCnt ;
    double dTemp    ;
    int r,c ;

    AnsiString sTemp ;

    double * dTrgPos = new double[iTrgCnt] ;



    FindChip(riVSN,  r,c );

    bool bLeftToRight = GetLtToRt();

    int iColGr  = OM.DevInfo.iColGrCnt;
    if( iColGr == 0) iColGr = 1;

    double dColGrPtchGap = OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ;
    int    iGrCnt = iColGr ? OM.DevInfo.iColCnt / iColGr : OM.DevInfo.iColCnt;

    double dLeftSttPos  = PM.GetValue(miVSN_XInsp , pvVSN_XInspWorkStt) ;
    for(int i = 0 ; i < iTrgCnt ; i++) {
        dTemp = dLeftSttPos -
                OM.DevInfo.dColPitch * i -
               (OM.DevInfo.iColGrCnt ? dColGrPtchGap * (i / iGrCnt) : 0);

        //dTemp      =  dLeftSttPos -                                                                                                                              //ù������
        //              OM.DevInfo.dColPitch * i -
        //              dColGrPtchGap * (i / OM.DevInfo.iColGrCnt);

                      //(OM.DevInfo.iColGrCnt ? (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch)* i  / OM.DevInfo.iColGrCnt : 0 ) ;

        //Ʈ���� ������ ������ �ɼ�. ���� �ӵ��� ���� �̹��� �и�.
        if(bLeftToRight) dTrgPos[i              ] = dTemp + OM.MstOptn.dTrigerOffset ;
        else             dTrgPos[iTrgCnt - 1 - i] = dTemp - OM.MstOptn.dTrigerOffset ;
        sTemp += AnsiString(dTemp) + AnsiString(" ") ;
    }

    Trace("Trigger" , sTemp.c_str());

    MT_SetAxtTrgPos(miVSN_XInsp , iTrgCnt , dTrgPos , 1000 , true , true) ;

    //m_mmm = iTrgCnt;

    delete [] dTrgPos ;
}

void CMarkVisn::ResetTrgPos()
{
    MT_ResetAxtTrgPos(miVSN_XInsp) ;
}

bool CMarkVisn::GetVisnRslt(EN_ARAY_ID _iAray , int _iRow)
{
    //Local Var.
    int hFile       ;
    int iFileHandle ;
    int iFileLength ;
    int divpos      ;

    char *pszBuffer ;
    AnsiString AllField ;
    AnsiString sPath, Temp ;

    int iFailCnt    ;
    int iFailRow    ;
    int iFailCol    ;
    int iFailItm    ;

    bool bLtToRt = GetLtToRt() ;

    for(int c = 0 ; c < DM.ARAY[_iAray].GetMaxCol() ; c++)
        DM.ARAY[riVSN].SetStat(_iRow,c,csRslt0); //�̸� ������ ĥ�ϰ�,

    EN_CHIP_STAT iRslt ;

    sPath = OM.MstOptn.sVisnRsltFile ;

    if(!FileExists(sPath)) { Trace("Err",(sPath + " no file").c_str());  return false ;}

    iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    pszBuffer = new char[iFileLength+1];
    memset(pszBuffer , 0 , sizeof(char)*(iFileLength+1));

    //�޸� ���� �Ҵ� ���߿� �����ϱ�...!
    FileRead (iFileHandle, pszBuffer, iFileLength);
    FileClose(iFileHandle);

    AllField = pszBuffer;

    Trace("Visn Data File" , AllField.c_str());

    iFailCnt = AllField.SubString(1,3).ToIntDef(-1) ;
    if(iFailCnt == -1) { Trace("Err","Fail Cnt is not a No"); return false ; }

    AllField.Delete(1,3);

    for (int i = 0; i < iFailCnt; i ++ ) {
        //Col.
        iFailCol = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailCol <= 0                ) { Trace("Err","iFailCol < 0"      ); return false ; }
        if(iFailCol > OM.DevInfo.iColCnt) { Trace("Err","iFailCol > iColCnt"); return false ; }
        AllField.Delete(1,2);

        //Row.
        iFailRow = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailRow <= 0                ) { Trace("Err","iFailRow < 0"      ); return false ; }
        if(iFailRow > OM.DevInfo.iRowCnt) { Trace("Err","iFailRow > iRowCnt"); return false ; }
        AllField.Delete(1,2);


        iFailItm = StrToIntDef("0x"+AllField.SubString(1,1),0);
        if(iFailItm <  0             ) { Trace("Err","iFailItm < 0"            ); return false ; }
        if(iFailItm >= MAX_CHIP_STAT ) { Trace("Err","iFailItm > MAX_CHIP_STAT"); return false ; }

        AllField.Delete(1,1);

        //���� ��ü�˻� ���ٰ˻�� �����ʿ��� �������� �ΰ����� �������� �ϳ�
        //�����ҵ� �Ͽ� ������ ���� �˻�� ���� �ϰ� �츮�ʿ��� �˾Ƽ� ��������.
        if(bLtToRt) DM.ARAY[_iAray].SetStat(_iRow ,iFailCol-1 , (EN_CHIP_STAT)iFailItm) ;
        else        DM.ARAY[_iAray].SetStat(_iRow ,OM.DevInfo.iColCnt - (iFailCol-1)-1 , (EN_CHIP_STAT)csFail ) ;  //�ݴ���� ����� ��� ������.
    }

    delete [] pszBuffer;


    return true ;
}


bool CMarkVisn::CycleVisn()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiVSN_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    int r,c ;
    int iFailCnt ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: if(!Step.iSeq){ //�޴��� �϶�.
                      if(OM.CmnOptn.iMarkInsp == moOneRow) {
                          if(!DM.ARAY[riVSN].GetCntStat(csWait)) //�˻� �ҿ��� ������
                              for(int c = 0 ; c < DM.ARAY[riVSN].GetMaxCol() ; c++) DM.ARAY[riVSN].SetStat(m_iLastInspRow,c,csWait); //���ٸ� ����
                      }
                      else {
                          if(!DM.ARAY[riVSN].GetCntStat(csWait)){
                              DM.ARAY[riVSN].SetStat(csWait);
                          }
                      }
                  }
                  Step.iCycle++;
                  return false ;

        case  11: MoveMotr(miVSN_YInsp , piVSN_YInspTrg   );
                  MoveMotr(miVSN_XInsp , piVSN_XInspTrgStt);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miVSN_YInsp , piVSN_YInspTrg   )) return false ;
                  if(!MoveMotr(miVSN_XInsp , piVSN_XInspTrgStt)) return false ;

                  //������ �۾��� ����.
                  if(FindChip(riVSN , r,c)){
                      m_iLastInspRow = r ;
                  }

                  SetTrgPos();

                  MoveMotr(miVSN_XInsp , piVSN_XInspTrgEnd , true);

                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miVSN_XInsp , piVSN_XInspTrgEnd, true)) return false ;
                  ResetTrgPos();



                  Step.iCycle++ ;
                  return false ;

        case  14: if(!IO_GetX(xVSC_MrkInspEnd)) return false ;
                  GetVisnRslt(riVSN , m_iLastInspRow);

                  //if(DM.ARAY[riVSN].GetCntStat(csWait)){
                  //    Step.iCycle = 11 ;
                  //    return false ;
                  //}
                  if(!DM.ARAY[riVSN].GetCntStat(csWait)) MoveMotr(miVSN_XInsp , piVSN_XInspWait);

                  Step.iCycle++;
                  return false ;







        case  15: if(!DM.ARAY[riVSN].GetCntStat(csWait)&&!MoveMotr(miVSN_XInsp , piVSN_XInspWait)) return false ;
                  iFailCnt = DM.ARAY[riVSN].GetCntStat  (csRslt1) +
                             DM.ARAY[riVSN].GetCntStat  (csRslt2) +
                             DM.ARAY[riVSN].GetCntStat  (csRslt3) +
                             DM.ARAY[riVSN].GetCntStat  (csRslt4) +
                             DM.ARAY[riVSN].GetCntStat  (csRslt5) +
                             DM.ARAY[riVSN].GetCntStat  (csRslt6) +
                             DM.ARAY[riVSN].GetCntStat  (csFail ) ;

                  if(Step.iSeq && !DM.ARAY[riVSN].GetCntStat(csWait) ){ //���䷱�� �϶���.
                      if(OM.DevOptn.iMarkInspFailCnt < iFailCnt) {
                           EM_SetErr(eiVSN_FailCnt) ;
                      }
                  }

                  Step.iCycle=0;
                  return true;
    }
}


//��Ī�� ���� �� �ϱ�.
bool CMarkVisn::CycleTrain(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiVSN_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    int r,c ;
    String sTemp2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yVSC_AutoTrain , true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        case  11: MoveMotr(miVSN_YInsp , piVSN_YInspTrg   );
                  MoveMotr(miVSN_XInsp , piVSN_XInspTrgStt);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miVSN_YInsp , piVSN_YInspTrg   )) return false ;
                  if(!MoveMotr(miVSN_XInsp , piVSN_XInspTrgStt)) return false ;
                  //if(!m_tmDelay.OnDelay(true , 300)) return false ; //���� Ʈ���θ�� ��ȯ ���.  //JH Ȯ���ؼ� Ready �� ���� �� ��ü.
                  if(!IO_GetX(xVSC_MrkReady)) return false;

                  //������ �۾��� ����.
                  if(FindChip(riVSN , r,c)){
                      m_iLastInspRow = r ;
                  }

                  SetTrgPos();

                  MoveMotr(miVSN_XInsp , piVSN_XInspTrgEnd , true);

                  sTemp = "Bf Trained Lot:" + m_sTrainedLot + "At Train Lot:" + DM.ARAY[riVSN].GetLotNo() ;
                  Trace("Train",sTemp.c_str());

                  m_sTrainedLot = DM.ARAY[riVSN].GetLotNo() ;

                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miVSN_XInsp , piVSN_XInspTrgEnd, true)) return false ;
                  ResetTrgPos();
                  IO_SetY(yVSC_AutoTrain , false);

                  Step.iCycle++ ;
                  return false ;

        case  14: //if(!IO_GetX(xVSC_MrkInspEnd)) return false ;


                  EM_SetErr(eiVSN_TrainSel) ;

                  Step.iCycle=0;
                  return true;
    }
}


void CMarkVisn::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CMarkVisn::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CMarkVisn::CheckStop()
{
    if(!MT_GetStop(miVSN_XInsp)) return false ;
    if(!MT_GetStop(miVSN_YInsp)) return false ;

    //if(!AT_Done(aiVSN_RtrZgRl)) return false ;



    return true ;
}

void CMarkVisn::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\MarkVisn.INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_sTrainedLot"  , m_sTrainedLot  );
        UserINI.Load(sPath, "Member" , "m_iLastInspRow" , m_iLastInspRow );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sTrainedLot"  , m_sTrainedLot );
        UserINI.Save(sPath, "Member" , "m_iLastInspRow" , m_iLastInspRow);
    }

}

//---------------------------------------------------------------------------
