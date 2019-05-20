//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PostBuff.h"
#include "WorkZone.h"
#include "Loader.h"
//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "ProbeComUnit.h"
#include "LotUnit.h"
#include "Array.h"
#include "SVInterface.h"
#include "FormProbe.h"
#include "sVision.h"
#include "ManualMan.h"
//#include "FormRslt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CWorkZone WRK;

TRS232C *RS232C_Pb1ch  ;
TRS232C *RS232C_Pb2ch  ;

int iPinFailCnt[MAX_ARRAY_ROW][MAX_PROB_COL]  ;

String convertHex2Bin(char* hex);
int convertASCII2Number(char ascii);

String convertHex2Bin(char* hex)
{
    String result;
    for(int i = 0; hex[i]; i++)
    {
        if(hex[i] == '.'){result += ".";continue;}
        int num = convertASCII2Number(hex[i]);
        for(int bit = 3; bit >= 0; bit--)
        {
            result += num&(1<<bit)? "1" : "0";
        }
    }

    return result;
}
int convertASCII2Number(char ascii)
{
    if(isdigit(ascii)){ return ascii - '0'; }
    if(islower(ascii)){ return ascii - 'a' + 10; }
    return ascii - 'A' + 10;
}

CWorkZone::CWorkZone(void)
{
}

void CWorkZone::Init()
{
    m_sPartName = "WorkZone " ;

    RS232C_Pb1ch = new TRS232C ;
    RS232C_Pb2ch = new TRS232C ;

    if(!RS232C_Pb1ch->Open(0)) ShowMessage("RS-232C Probe 1 Ch Open Failed!");
    else                       RS232C_Pb1ch->CallBackReg(HeatProbe1Ch);
    if(!RS232C_Pb2ch->Open(1)) ShowMessage("RS-232C Probe 2 Ch Open Failed!");
    else                       RS232C_Pb2ch->CallBackReg(HeatProbe2Ch);

    PCM.SetDaegyeom();

    if(DM.ARAY[riWRK].CheckAllStat(csNone) || DM.ARAY[riWRK].CheckAllStat(csUnkwn)) m_iVisnErr = vsNomal;
    else                                                                            m_iVisnErr = vsGood ;

    Reset();
    Load(true);
}

void CWorkZone::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    memset(iPinFailCnt , 0 , sizeof(int)*MAX_ARRAY_ROW *  MAX_PROB_COL);
}

void CWorkZone::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CWorkZone::~CWorkZone (void)
{

}

void CWorkZone::Close()
{
    Load(false);
}

bool CWorkZone::FindChip(int &r , int &c )
{
    c = DM.ARAY[riWRK].FindLastCol(csUnkwn) ;

    return (c > -1 && c > -1);
}

double CWorkZone::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r ,c , iColGrCnt , iMaxCol;
    int fr , fc , rc , wc;
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();
    FindChip(r , c);
    fc = (iMaxCol - c) -1 ; //Ⱦ���� �ø��Ȱ�.

    rc = fc - OM.DevOptn.iContColCnt ;    //���׸� �÷�.

    double dWorkColptch = (OM.DevInfo.dColPitch + OM.DevOptn.dWorkPtchOfs);
    //X , Y Pos Setting.

    double dX1Pos = PM.GetValue(miWRK_XCmp , pvWRK_XWorkStartPs) + (dWorkColptch * fc) ;
    double dX2Pos = PM.GetValue(miWRK_XCmp , pvWRK_XWorkStartPs) + (dWorkColptch * fc) - OM.DevOptn.dReGrabPs1                         ;
    double dX3Pos = PM.GetValue(miWRK_XCmp , pvWRK_XWorkStartPs) + (dWorkColptch * fc) - OM.DevOptn.dReGrabPs1 - OM.DevOptn.dReGrabPs2 ;

    //dX2Pos += OM.DevOptn.iContColCnt     * dWorkColptch  ;
    //dX3Pos += OM.DevOptn.iContColCnt * 2 * dWorkColptch  ;

    //Regrib pos.
    double dX2PosRg = PM.GetValue(miWRK_XCmp , pvWRK_XWorkStartPs) + (dWorkColptch * rc) - OM.DevOptn.dReGrabPs1                         ;
    double dX3PosRg = PM.GetValue(miWRK_XCmp , pvWRK_XWorkStartPs) + (dWorkColptch * rc) - OM.DevOptn.dReGrabPs1 - OM.DevOptn.dReGrabPs2 ;

    double dPos   = 0.0 ;

    //���� ������ �ݴ�� ������ �ø��Ȱ����� ����Ѵ�.
    if( OM.DevInfo.iColGrCnt > 1 ) {
        //iColGrCnt��  fc  �� �׷찹��. OM.DevInfo.iColGrCnt�̰Ŵ� �ѱ׷�� LED ����.
        iColGrCnt = fc / OM.DevInfo.iColGrCnt ;
        dX1Pos += (OM.DevInfo.dColGrGap - dWorkColptch) * iColGrCnt ;
        dX2Pos += (OM.DevInfo.dColGrGap - dWorkColptch) * iColGrCnt ;
        dX3Pos += (OM.DevInfo.dColGrGap - dWorkColptch) * iColGrCnt ;

        iColGrCnt = rc / OM.DevInfo.iColGrCnt ;
        dX2PosRg += (OM.DevInfo.dColGrGap - dWorkColptch) * iColGrCnt ;
        dX3PosRg += (OM.DevInfo.dColGrGap - dWorkColptch) * iColGrCnt ;
    }

    if(_iMotr == miWRK_XCmp){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                                                      ); break ;
            case piWRK_XWait         : dPos = PM.GetValue (_iMotr , pvWRK_XWaitPs                                      ); break ;
            case piWRK_XGrip         : dPos = PM.GetValue (_iMotr , pvWRK_XGripPs                                      ); break ;
            case piWRK_XWorkStart    : dPos = PM.GetValue (_iMotr , pvWRK_XWorkStartPs                                 ); break ;
            case piWRK_XOut          : dPos = PM.GetValue (_iMotr , pvWRK_XOutPs                                       ); break ;
            case piWRK_XReGrip       :      if (m_iReGripCnt == 0) dPos = dX2PosRg ;
                                       else if (m_iReGripCnt == 1) dPos = dX3PosRg ;                                      break ;

            case piWRK_XWork         :      if (m_iReGripCnt == 0) dPos = dX1Pos ;
                                       else if (m_iReGripCnt == 1) dPos = dX2Pos ;
                                       else if (m_iReGripCnt == 2) dPos = dX3Pos ;                                        break ;
        }
    }
    else if(_iMotr == miWRK_ZTop){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                                                      ); break ;
            case piWRK_ZTWait        : dPos = PM.GetValue (_iMotr , pvWRK_ZTWaitPs                                     ); break ;
            case piWRK_ZTWork        : dPos = PM.GetValue (_iMotr , pvWRK_ZTWorkPs                                     ); break ;
            case piWRK_ZTChange      : dPos = PM.GetValue (_iMotr , pvWRK_ZTChangePs                                   ); break ;
        }
    }
    else if(_iMotr == miWRK_ZBtm){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                                                      ); break ;
            case piWRK_ZBWait        : dPos = PM.GetValue (_iMotr , pvWRK_ZBWaitPs                                     ); break ;
            case piWRK_ZBWork        : dPos = PM.GetValue (_iMotr , pvWRK_ZBWorkPs                                     ); break ;
            case piWRK_ZBChange      : dPos = PM.GetValue (_iMotr , pvWRK_ZBChangePs                                   ); break ;
        }
    }
    else if(_iMotr == miWRK_YBtm){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                                                      ); break ;
            case piWRK_YBWait        : dPos = PM.GetValue (_iMotr , pvWRK_YBWaitPs                                     ); break ;
            case piWRK_YBWork        : dPos = PM.GetValue (_iMotr , pvWRK_YBWorkPs                                     ); break ;
            case piWRK_YBContect     : dPos = PM.GetValue (_iMotr , pvWRK_YBContectPs                                  ); break ;
            case piWRK_YBChange      : dPos = PM.GetValue (_iMotr , pvWRK_YBChangePs                                   ); break ;

        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CWorkZone::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isContect  = !MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBWaitPs)) ||
                      !MT_CmprPos(miWRK_ZTop , PM.GetValue(miWRK_ZTop,pvWRK_ZTWaitPs)) ;
    bool isGripping =  AT_Complete(aiWRK_Cmp , ccFwd) ;
    bool isXMoving  = !MT_GetStopInpos(miWRK_XCmp);

    bool isZbtmWait =  MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBChangePs)) && MT_GetStop(miWRK_ZBtm);
    bool isZbtmWork =  MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBWorkPs  )) && MT_GetStop(miWRK_ZBtm);

    bool isRalClndr =  AT_Complete(aiPRB_HtBlk, ccBwd);

    if(_iMotr == miWRK_XCmp){
        if(isContect&&isGripping) {sMsg = "During Inspection"; bRet = false ;}
        switch(_iPstnId) {
            default                   :  break ;
            case piWRK_XWait          :  break ;
            case piWRK_XGrip          :  break ;
            case piWRK_XWorkStart     :  break ;
            case piWRK_XReGrip        :  break ;
            case piWRK_XOut           :  break ;
            case piWRK_XWork          :  break ;
        }
    }
    else if(_iMotr == miWRK_ZTop){
        switch(_iPstnId) {
            default                   :  break ;
            case piWRK_ZTWait         :  break ;
            case piWRK_ZTWork         :  if(isXMoving) {sMsg = MT_GetName(miWRK_XCmp)+" is Moving"  ; bRet = false ;} break ;
        }
    }
    else if(_iMotr == miWRK_YBtm){
        if(!isZbtmWait && !isZbtmWork) {sMsg = MT_GetName(miWRK_YBtm)+" is Not Wait Position"  ; bRet = false ;}
        //���̵� �� ���� Z�� Work ��ġ���� �̵� ���� �ּ�...JS
        switch(_iPstnId) {
            default                   :  break ;
            case piWRK_YBWait         :  break ;
            case piWRK_YBWork         :  break ;
            case piWRK_YBContect      :  break ;
            case piWRK_YBChange       :  if(!isZbtmWait) {sMsg = MT_GetName(miWRK_YBtm) + " is not Change Position" ; bRet = false;} break ;
        }
    }
    else if(_iMotr == miWRK_ZBtm){       
        switch(_iPstnId) {
            default                   :  break ;
            case piWRK_ZBWait         :  if(isRalClndr) {sMsg = AT_GetName(aiPRB_HtBlk)+" is Bwd"    ; bRet = false ;} break ;
            case piWRK_ZBWork         :  if(isXMoving ) {sMsg = MT_GetName(miWRK_XCmp )+" is Moving" ; bRet = false ;}
                                         if(isRalClndr) {sMsg = AT_GetName(aiPRB_HtBlk)+" is Bwd"    ; bRet = false ;} break ;
            case piWRK_ZBChange       :  if(isRalClndr) {sMsg = AT_GetName(aiPRB_HtBlk)+" is Bwd"  ; bRet = false ;} break ;
        }
    }

    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CWorkZone::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isXMoving = !MT_GetStopInpos(miWRK_XCmp);

    if(_iActr == aiWRK_Cmp) {
        if( !_bFwd) {
            if(isXMoving) { sMsg = MT_GetName(miWRK_XCmp)+" is Moving" ; bRet = false ;}
        }
    }
    else if(_iActr == aiPRB_HtBlk){
    
    }

    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

bool CWorkZone::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CWorkZone::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CWorkZone::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiWRK_HomeTo);
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
//                 Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiWRK_Cmp,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWRK_Cmp,ccBwd))return false ;
                 MT_DoHome(miWRK_ZBtm);
                 MT_DoHome(miWRK_ZTop);
                 MT_DoHome(miWRK_XCmp);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRK_ZBtm))return false;
                 if(!MT_GetHomeEnd(miWRK_ZTop))return false;
                 if(!MT_GetHomeEnd(miWRK_XCmp))return false;
                 MT_DoHome(miWRK_YBtm);
                 Step.iHome++;
                 return false ;


        case 13: if(!MT_GetHomeEnd(miWRK_YBtm))return false;
                 MT_GoAbsMan(miWRK_YBtm, GetMotrPos(miWRK_YBtm,piWRK_YBWait));
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miWRK_YBtm, GetMotrPos(miWRK_YBtm,piWRK_YBWait))) return false ;
                 MT_GoAbsMan(miWRK_ZBtm, GetMotrPos(miWRK_ZBtm,piWRK_ZBChange));
                 MT_GoAbsMan(miWRK_ZTop, GetMotrPos(miWRK_ZTop,piWRK_ZTWait));
                 //MT_GoAbsMan(miWRK_XCmp, GetMotrPos(miWRK_XCmp,piWRK_XWait ));
                 MT_GoAbsMan(miWRK_XCmp, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miWRK_ZBtm, GetMotrPos(miWRK_ZBtm,piWRK_ZBChange)))return false ;
                 if(!MT_GoAbsMan(miWRK_ZTop, GetMotrPos(miWRK_ZTop,piWRK_ZTWait)))return false ;
                 //if(!MT_GoAbsMan(miWRK_XCmp, GetMotrPos(miWRK_XCmp,piWRK_XWait )))return false ;
                 if(!MT_GoAbsMan(miWRK_XCmp, m_dLastIndPos))return false ;
                 AT_MoveCyl(aiPRB_HtBlk, ccBwd);
                 Step.iHome++;
                 return false ;

        case 16: if(!AT_MoveCyl(aiPRB_HtBlk,ccBwd))return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CWorkZone::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xWRK_Overload) ) EM_SetErr(eiWRK_Overload);

    int r,c,iMaxCol ;
    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //FindChip(r,c);
        iMaxCol = DM.ARAY[riWRK].GetMaxCol();
        FindChip(r , c);

        int fc = (iMaxCol - c) -1  ; //���� �����͸� ������ ��.

        bool isReGrip1       =  fc >= OM.DevOptn.iReGrabCnt1 - OM.DevOptn.iContColCnt && m_iReGripCnt == 0 ;
        bool isReGrip2       =  fc >= OM.DevOptn.iReGrabCnt2 - OM.DevOptn.iContColCnt && m_iReGripCnt == 1 ;

        bool isHalfWorked    =  fc > OM.DevInfo.iColCnt /2 ;
        bool is2Of3Worked    =  fc > OM.DevInfo.iColCnt*2 /3 ;
//        bool isPstEmpty      = !DM.ARAY[riPS1].GetCntExist() && !DM.ARAY[riPS2].GetCntExist();
        bool isPt1Empty      = !DM.ARAY[riPSB].GetCntExist() || DM.ARAY[riPSB].GetSubStep() == 1 ;
        bool isPstRegribed   =  PSB.GetRegribCnt() > 0 ;

        bool isNeedReGrap    =  isReGrip1 || isReGrip2 ;

        bool isCyclePreWork  =  DM.ARAY[riWRK].GetCntExist (       ) && DM.ARAY[riWRK].GetSubStep() == 0 ;

        //Vision �˻� ������ Cycle�߰�.
        bool isCycleVisn     =  DM.ARAY[riWRK].CheckAllStat(csUnkwn) && (m_iVisnErr == vsNomal || m_iVisnErr == vsErrReTry) && DM.ARAY[riWRK].GetSubStep() == 1 ;

        bool isCycleWork     =  DM.ARAY[riWRK].GetCntStat  (csUnkwn) &&( isPt1Empty || !isHalfWorked) && (m_iVisnErr != vsNomal && m_iVisnErr != vsErrReTry) && DM.ARAY[riWRK].GetSubStep() == 1 ; //!isHalfWorked ) ;
        bool bTempWork1 = (m_iVisnErr != vsNomal && m_iVisnErr != vsErrReTry) && DM.ARAY[riWRK].GetSubStep() == 1;
        bool bTempWork2 = (m_iVisnErr != vsNomal && m_iVisnErr != vsErrReTry);
        bool bTempWork3 = DM.ARAY[riWRK].GetSubStep() == 1;
        bool isCycleWkReGrip =  DM.ARAY[riWRK].GetCntStat  (csUnkwn) &&  isNeedReGrap && DM.ARAY[riWRK].GetSubStep() == 1 ;

        bool isCyclePostWork = !DM.ARAY[riWRK].GetCntStat  (csUnkwn) && !DM.ARAY[riWRK].CheckAllStat(csNone )&& DM.ARAY[riWRK].GetStep() == 0 && DM.ARAY[riPSB].CheckAllStat(csNone ) ;//&& // DM.ARAY[riPS2].CheckAllStat(csNone )&&
        //                               (DM.ARAY[riPR2].CheckAllStat(csUnkwn) ||  DM.ARAY[riPR1].CheckAllStat(csUnkwn)||!DM.ARAY[riLD1].GetCntStat(csUnkwn) || OM.CmnOptn.bLoadingStop ); //�����尡 �ȵ�ü�� ����Ʈ ��ũ ���� ����Ŭ����  LastStripäũ�� ���Ѵ�.
        bool isCycleEnd      =  DM.ARAY[riWRK].CheckAllStat(csNone );

        //�𸣴� ��Ʈ������.
        if(  DM.ARAY[riWRK].CheckAllStat(csNone) && !DM.ARAY[riPRB].GetStep() && (IO_GetX(xWRK_1Pkg) ||  IO_GetX(xWRK_2Pkg)) && isCycleWork) EM_SetErr(eiWRK_Unknown) ;

        //��Ʈ�� �����.
        if( DM.ARAY[riWRK].GetCntExist() && (!IO_GetX(xWRK_1Pkg) && !IO_GetX(xWRK_2Pkg) ) && !OM.CmnOptn.bDryRun ) EM_SetErr(eiWRK_Dispr)   ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCyclePostWork) {Trace(m_sPartName.c_str(),"CyclePostWork Stt"); Step.iSeq = scPostWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleWkReGrip) {Trace(m_sPartName.c_str(),"CycleWkReGrip Stt"); Step.iSeq = scWkReGrip   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork    ) {Trace(m_sPartName.c_str(),"CycleWork     Stt"); Step.iSeq = scWork       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleVisn    ) {Trace(m_sPartName.c_str(),"CycleVisn     Stt"); Step.iSeq = scVisn       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePreWork ) {Trace(m_sPartName.c_str(),"CyclePreWork  Stt"); Step.iSeq = scPreWork    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //

        /*
             if (isCyclePreWork ) {Trace(m_sPartName.c_str(),"CyclePreWork  Stt"); Step.iSeq = scPreWork    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleReGrip  ) {Trace(m_sPartName.c_str(),"CycleReGrip   Stt"); Step.iSeq = scReGrip     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork    ) {Trace(m_sPartName.c_str(),"CycleWork     Stt"); Step.iSeq = scWorkZone   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePostWork) {Trace(m_sPartName.c_str(),"CyclePostWork Stt"); Step.iSeq = scPostWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        */
        else if (isCycleEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default          :                      Trace(m_sPartName.c_str(),"default       End");Step.iSeq = scIdle ;  return false ;
        case scIdle      :                                                                                           return false ;
        case scPreWork   : if(CyclePreWork ()){ Trace(m_sPartName.c_str(),"CyclePreWork  End");Step.iSeq = scIdle ;} return false ;
        case scVisn      : if(CycleVisn    ()){ Trace(m_sPartName.c_str(),"CycleVisn     End");Step.iSeq = scIdle ;} return false ;
        case scWork      : if(CycleWork    ()){ Trace(m_sPartName.c_str(),"CycleWork     End");Step.iSeq = scIdle ;} return false ;
        case scWkReGrip  : if(CycleWkReGrip()){ Trace(m_sPartName.c_str(),"CycleWkReGrip End");Step.iSeq = scIdle ;} return false ;
        case scPostWork  : if(CyclePostWork()){ Trace(m_sPartName.c_str(),"CyclePostWork End");Step.iSeq = scIdle ;} return false ;
    }

    return false ;

}

bool CWorkZone::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    //During the auto run, do not stop.
    Stat.bReqStop = true ;
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CWorkZone::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    //During the auto run, do not stop.
    if (Step.iSeq) return false; ///fail �� ��� �˻� �Ҽ���...  Stat.iReInspCnt = 0;

    Step.iToStart = 10 ;

    //Ok.
    return true;
}

bool CWorkZone::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErr(eiWRK_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: if(DM.ARAY[riWRK].CheckAllStat(csNone)) {
                     MoveActr(aiWRK_Cmp , ccBwd);
                 }

                 Step.iToStart ++;
                 return false;

        case 11: if(!AT_Done(aiWRK_Cmp)) return false ;
                 MoveActr(aiPRB_HtBlk, ccFwd);
                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveActr(aiPRB_HtBlk, ccFwd))return false;
                 MoveMotr(miWRK_YBtm,piWRK_YBWork);
                 Step.iToStart ++;
                 return false;


        case 13: if(!MoveMotr(miWRK_YBtm,piWRK_YBWork)) return false ;
                 MoveMotr(miWRK_ZTop,piWRK_ZTWait);
                 MoveMotr(miWRK_ZBtm,piWRK_ZBWait);

                 Step.iToStart++;
                 return false ;

        case 14: if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait))return false ;
                 if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait))return false ;
                 Step.iToStart = 0 ;
                 return false ;

    }
}

bool CWorkZone::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErr(eiWRK_ToStopTO);

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

        case 10: Step.iToStop ++ ;
                 return false ;

        case 11: if(!MT_GetAlarm   (miWRK_XCmp) &&
                    !MT_GetNLimSnsr(miWRK_XCmp) &&
                    !MT_GetPLimSnsr(miWRK_XCmp) ) m_dLastIndPos = MT_GetCmdPos(miWRK_XCmp) ;
                 else                             m_dLastIndPos = GetMotrPos  (miWRK_XCmp , piWRK_XWait) ;
                 MoveMotr(miWRK_ZBtm , piWRK_ZBChange);
                 Step.iToStop++;
                 return false ;

        case 12: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBChange)) return false ;
                 MoveActr(aiPRB_HtBlk, ccBwd);
                 Step.iToStop++;
                 return false ;

        case 13: if(!MoveActr(aiPRB_HtBlk, ccBwd))return false;
                 OM.SaveDevOptn(OM.m_sCrntDev);  //��ī��Ʈ ������ ���ؼ�.
                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CWorkZone::CyclePreWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiWRK_Cmp,ccBwd);

                  if(DM.ARAY[riWRK].GetLotNo() != m_sLastStrip) {
                      LT.LotOpen(DM.ARAY[riWRK].GetLotNo()) ;
                      m_sLastStrip = DM.ARAY[riWRK].GetLotNo() ;
                      Trace("First Strip",("LotOpen_" + DM.ARAY[riWRK].GetLotNo()).c_str());
                  }

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiWRK_Cmp,ccBwd)) return false ;

                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);
                  MoveMotr(miWRK_XCmp,piWRK_XGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
                  if(!MoveMotr(miWRK_XCmp,piWRK_XGrip)) return false ;
                  MoveActr(aiWRK_Cmp,ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiWRK_Cmp,ccFwd)) return false ;
                  MoveMotr(miWRK_XCmp,piWRK_XWorkStart);
                
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_XCmp,piWRK_XWorkStart)) return false ;
                  PCM.ClearData();
                  //DM.ShiftArrayData(riWRP,riWRK);
                  DM.ARAY[riWRK].SetSubStep(1);
                  m_iReGripCnt = 0 ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CWorkZone::CycleVisn()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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

    //TRetResult OriRslt ;
    //AnsiString sRslt;
    bool bRslt ;
    char sRslt[32];


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(OM.CmnOptn.bVisionSkip) {
                      Step.iCycle = 13;
                      return false;
                  }
                  SV_Grab(0);
                  Step.iCycle++;
                  return false ;

        case  11: if(!SV_GetGrabEnd(0)) return false ;
                  SV_Inspect(0); //������ �Ⱦ�.
                  Step.iCycle++;
                  return false ;

        case  12: if(!SV_GetInspEnd(0)) return false ;
                  Step.iCycle++;
                  return false;

        case  13: //bRslt = SV_GetRslt(0 , sRslt);
                  bRslt = SV_GetRslt(0 , sRslt);

                  if(!bRslt && !OM.CmnOptn.bVisionSkip){
                      EM_SetErr(eiWRK_VisnNG);
                      m_bVisnErrDisp = true;
                  }
                  else {
                      m_iVisnErr = vsGood;
                  }
                  Step.iCycle=0;
                  return true;
    }
}
bool CWorkZone::CycleWork()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    int iLedTime = OM.DevOptn.i1ChLedTime > OM.DevOptn.i2ChLedTime ? OM.DevOptn.i1ChLedTime * 100 : OM.DevOptn.i2ChLedTime * 100;
    int iZrTime  = OM.DevOptn.i1ChZenTime > OM.DevOptn.i2ChZenTime ? OM.DevOptn.i1ChZenTime * 100 : OM.DevOptn.i2ChZenTime * 100;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, iLedTime + iZrTime + 1000)) {
        EM_SetErr(eiWRK_CycleTO);
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
    bool r1,r2 ;
    int iMaxRow,iMaxCol,icsWork,iFail;
    iMaxRow = DM.ARAY[riWRK].GetMaxRow();
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();

    //if(iReInspCnt <= 0) iReInspCnt = OM.DevOptn.iReInspCnt;

    static int  iReInspCnt = 0 ;
    static int  iCmmErrCnt = 0 ;
    AnsiString sPinCount ;

    //static bool bRepeatAtStart = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //bRepeatAtStart = Stat.bRepeat ;
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);
                  MoveActr(aiWRK_Cmp,ccFwd);
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        //Case 19������ �� �ٲܶ� ����.
        case  11: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
                  if(!MoveActr(aiWRK_Cmp,ccFwd))         return false ;
                  MoveMotr(miWRK_XCmp,piWRK_XWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XCmp,piWRK_XWork))  return false ;
                  //MoveMotr(miWRK_ZBtm,piWRK_ZBWork);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWork);
                  Step.iCycle++;
                  return false;

        case  13: //if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWork)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWork)) return false ;
                  if(OM.MstOptn.bGripTest) MoveActr(aiWRK_Cmp,ccBwd); //����ö ����� ��û����.
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWork)) return false ;
                  if(PM.GetValue(miWRK_YBtm ,pvWRK_YBWorkPs) != PM.GetValue(miWRK_YBtm ,pvWRK_YBContectPs)){
                      MoveMotr(miWRK_YBtm, piWRK_YBContect) ;
                      Step.iCycle++;
                      return false ;
                  }
                  else {
                      Step.iCycle = 16 ;
                      return false ;
                  }

        case  15: if(!MoveMotr(miWRK_YBtm, piWRK_YBContect)) return false ;
                  Step.iCycle++;
                  return false;

        case  16: if(OM.MstOptn.bGripTest) { if(!MoveActr(aiWRK_Cmp,ccBwd)) return false ; } //����ö ����� ��û����.
                  OM.DevOptn.iContactPinCnt++;
                  if(OM.DevOptn.iLimContactPinCnt < OM.DevOptn.iContactPinCnt) {
                      EM_SetErr(eiWRK_LimPinCnt);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  /*MT_GoIncRun(miWRK_ZBtm , -2.0);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MT_GetStopInpos(miWRK_ZBtm)) return false ;
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWork)) return false ;*/
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;




        case  17: if(!m_tmTemp.OnDelay(true ,OM.DevOptn.iPrbSttDelay)) return false ;

                  if(OM.CmnOptn.bIgnrInsp) {
                      bPb1Rslt = true ;
                      bPb2Rslt = true ;
                  }
                  else {
                      bPb1Rslt = false ;
                      bPb2Rslt = false ;
                      //IO_SetY(yWRK_Probe1,true);
                      //IO_SetY(yWRK_Probe2,true);
                      if(OM.MstOptn.bByChInsp) PCM.Start(1);
                      else                     PCM.Start( );
                  }
                  m_tmCont1.Clear();
                  Step.iCycle++;
                  return false;

        case  18: if(m_tmCont1.OnDelay(true, (OM.DevOptn.i1ChLedTime*100) + (OM.DevOptn.i1ChZenTime*100) + 1000)) {
                  //if(m_tmCont1.OnDelay(true, 3000)) {
                      if(!bPb1Rslt){
                          MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                          Step.iCycle = 30 ;
                          return false ;
                      }
                      /*
                          iCmmErrCnt++;
                          MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                          MoveMotr(miWRK_ZTop,piWRK_ZTWait);
                          MoveActr(aiWRK_Cmp,ccFwd);

                          if(iCmmErrCnt < 4) { //�˻��� ���� �� �ȳ��� �ͼ� ��Ʈ���� ����.

                              Step.iCycle = 11 ;
                              Trace("Cmmunication","g_bPb1Rslt ComRetry");
                              return false ;
                          }

                          if(iCmmErrCnt < 5) { //�� �ȵŸ� ���� �ٽ��غ�.

                              Step.iCycle = 11 ;
                              Trace("Cmmunication","g_bPb1Rslt ComRetry && SetDaegyeom()");
                              if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //��� Eng ���κ� ����.
                              return false ;
                          }

                          //�׷��� �ȵǸ� ����.
                          EM_SetErr(eiWRK_FailProbeComm1) ;
                          Step.iCycle = 0 ;
                          return false ;
                      }
                      */
                  }
                  if(!bPb1Rslt) return false ;
                  if(OM.MstOptn.bByChInsp) PCM.Start(2);
                  //if(OM.MstOptn.bByChInsp) PCM.Start();
                  m_tmCont2.Clear();
                  Step.iCycle++;
                  return false;

        case  19: if(m_tmCont2.OnDelay(true, (OM.DevOptn.i2ChLedTime*100) + (OM.DevOptn.i2ChZenTime*100) + 1000)) {
                  //if(m_tmCont2.OnDelay(true, 3000)) {
                      if(!bPb2Rslt){
                          MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                          Step.iCycle = 30 ;
                          return false ;
                      }
                      /*
                          iCmmErrCnt++;
                          MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                          MoveMotr(miWRK_ZTop,piWRK_ZTWait);
                          MoveActr(aiWRK_Cmp,ccFwd);

                          if(iCmmErrCnt < 4) { //�˻��� ���� �� �ȳ��� �ͼ� ��Ʈ���� ����.

                              Step.iCycle = 11 ;
                              Trace("Cmmunication","g_bPb2Rslt ComRetry");
                              return false ;
                          }

                          if(iCmmErrCnt < 5) { //�� �ȵŸ� ���� �ٽ��غ�.

                              Step.iCycle = 11 ;
                              Trace("Cmmunication","g_bPb2Rslt ComRetry && SetDaegyeom()");
                              if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //��� Eng ���κ� ����.
                              return false ;
                          }

                          //�׷��� �ȵǸ� ����.
                          EM_SetErr(eiWRK_FailProbeComm2) ;
                          Step.iCycle = 0 ;
                          return false ;
                      }
                      */
                  }
                  if(!bPb2Rslt) return false ;

                  if(OM.MstOptn.bGripTest) MoveActr(aiWRK_Cmp,ccFwd); //����ö ����� ��û����.
                  MoveMotr(miWRK_YBtm , piWRK_YBWork);
                  Step.iCycle++;
                  return false ;


        case  20: if(OM.MstOptn.bGripTest) { if(!MoveActr(aiWRK_Cmp,ccFwd)) return false ; } //����ö ����� ��û����.
                  if(!MoveMotr(miWRK_YBtm , piWRK_YBWork)) return false;
                  //����ŷ ���� �� �� �˻� ����.
                  FindChip(r,c);
                  if(OM.CmnOptn.bIgnrInsp) {
                      for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                          for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRK].SetStat(i , c - j, csGood); //����Ÿ ����ŷ.
                      }
                  }
                  else {
                      r1 = PCM.CheckDataCnt(riWRK , sPb1Msg);
                      r2 = PCM.CheckDataCnt(riWRK , sPb2Msg);

                      if(!r1){ Step.iCycle = 10 ; m_iDataErrCnt++ ; return false ;} //EM_SetErr(eiWRK_Probe1DataWrong) ; //������ ����ŷ.  ���߿� PCM�� ARAY �Ⱦ� ���� ���⼭ ������ �޾Ƽ� ����ŷ �Ѵ�.
                      if(!r2){ Step.iCycle = 10 ; m_iDataErrCnt++ ; return false ;} //EM_SetErr(eiWRK_Probe2DataWrong) ;

                      if(r1 && r2) {
                          PCM.ClearDataCrnt();
                          PCM.GetDataCh(riWRK , sPb1Msg , PROBE_CH1 ,Stat.bRepeat);
                          PCM.GetDataCh(riWRK , sPb2Msg , PROBE_CH2 ,Stat.bRepeat);
                      }
                  }
                  iFail = 0;
                  for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                      iFail += DM.ARAY[riWRK].GetCntColStat(c - j, csFail); //����Ÿ ����ŷ.
                  }

                  if(OM.MstOptn.bGripTest) MoveActr(aiWRK_Cmp,ccFwd); //����ö ����� ��û����.
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  //��˻� ���� �� ����Ʈ �׽�Ʈ ����.
                  if (iFail >= OM.DevOptn.iLimInsFailCnt || Stat.bRepeat){
                      if(iReInspCnt < OM.DevOptn.iReInspCnt || Stat.bRepeat){
                          for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                              for(int i =0 ; i < iMaxRow ; i++) DM.ARAY[riWRK].SetStat(i ,c - j, csUnkwn) ;
                          }

                          Step.iCycle = 11 ;
                          iReInspCnt++;
                          return false;
                      }
                  }

                  //���κ� ���� �ɿ� ���� ī��Ʈ����
                  //MAX_PROB_COL == 4 have to check.
                  for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                      for(int i =0 ; i < iMaxRow ; i++) {
                          if(DM.ARAY[riWRK].GetStat(i ,c - j)==csFail) {
                              iPinFailCnt[i][OM.DevOptn.iContColCnt-j]++ ;
                              if(OM.DevOptn.iPinCnt && iPinFailCnt[i][OM.DevOptn.iContColCnt - j] > OM.DevOptn.iPinCnt) {
                                  EM_SetErr(eiWRK_PinFailCntOver);
                                  sPinCount = "(Front Left is 0,0) Fail Pin is"+AnsiString(i+1)+","+AnsiString(OM.DevOptn.iContColCnt-j) ;
                                  FM_MsgOk("Pin Check",sPinCount);
                                  iPinFailCnt[i][OM.DevOptn.iContColCnt-j] = 0 ;
                              }
                          }
                      }
                  }

                  Step.iCycle++;
                  return false;

        case  21: if(OM.MstOptn.bGripTest) { if(!MoveActr(aiWRK_Cmp,ccFwd)) return false ; } //����ö ����� ��û����.
                  if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
                  if (!DM.ARAY[riWRK].GetCntStat(csUnkwn) && OM.CmnOptn.bCheckFailCnt && DM.ARAY[riWRK].GetCntStat(csFail) >= OM.CmnOptn.iCheckFailCnt ) EM_SetErr(eiWRK_FailCntOver);
                  Step.iCycle=0;
                  return true;

        case  30: if(!MoveMotr(miWRK_YBtm, piWRK_YBWork)) return false ;
                  iCmmErrCnt++;
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);
                  MoveActr(aiWRK_Cmp,ccFwd);

                  if(iCmmErrCnt < 4) { //�˻��� ���� �� �ȳ��� �ͼ� ��Ʈ���� ����.

                      Step.iCycle = 11 ;
                      if(!bPb1Rslt             ) Trace("Cmmunication","bPb1Rslt ComRetry");
                      if( bPb1Rslt && !bPb2Rslt) Trace("Cmmunication","bPb2Rslt ComRetry");
                      return false ;
                  }

                  if(iCmmErrCnt < 5) { //�� �ȵŸ� ���� �ٽ��غ�.

                      Step.iCycle = 11 ;
                      if(!bPb1Rslt             ) Trace("Cmmunication","bPb1Rslt ComRetry && SetDaegyeom()");
                      if( bPb1Rslt && !bPb2Rslt) Trace("Cmmunication","bPb2Rslt ComRetry && SetDaegyeom()");

                      if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //��� Eng ���κ� ����.
                      return false ;
                  }

                  //�׷��� �ȵǸ� ����.
                  if(!bPb1Rslt             ) EM_SetErr(eiWRK_FailProbeComm1);
                  if( bPb1Rslt && !bPb2Rslt) EM_SetErr(eiWRK_FailProbeComm2) ;
                  Step.iCycle = 0 ;
                  return false ;
    }
}

bool CWorkZone::CycleWkReGrip()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    int iLedTime = OM.DevOptn.i1ChLedTime > OM.DevOptn.i2ChLedTime ? OM.DevOptn.i1ChLedTime * 100 : OM.DevOptn.i2ChLedTime * 100;
    int iZrTime  = OM.DevOptn.i1ChZenTime > OM.DevOptn.i2ChZenTime ? OM.DevOptn.i1ChZenTime * 100 : OM.DevOptn.i2ChZenTime * 100;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, iLedTime + iZrTime + 1000)) {
    //if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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

    int r,c;
    bool r1,r2 ;
    FindChip(r,c);
    int iMaxRow,iMaxCol,iFail;
    iMaxRow = DM.ARAY[riWRK].GetMaxRow();
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();
    //static bool bErr = false ;
    AnsiString sPinCount ;

    static int iCmmErrCnt = 0 ;

    static int iInspCnt = 0 ;
    //if(iReInspCnt <= 0) iReInspCnt = OM.DevOptn.iReInspCnt;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);
                  MoveActr(aiWRK_Cmp,ccFwd);
                  iInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        //Case 18������ �� �ٲܶ� ����.
        case  11: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
                  if(!MoveActr(aiWRK_Cmp,ccFwd))         return false ;
                  MoveMotr(miWRK_XCmp,piWRK_XWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XCmp,piWRK_XWork))  return false ;
                  MoveMotr(miWRK_ZTop,piWRK_ZTWork);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_ZTop,piWRK_ZTWork)) return false ;
                  if(OM.MstOptn.bGripTest) MoveActr(aiWRK_Cmp,ccBwd);  //����ö ����� ��û����.
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWork)) return false ;
                  if(PM.GetValue(miWRK_YBtm ,pvWRK_YBWorkPs) != PM.GetValue(miWRK_YBtm ,pvWRK_YBContectPs)){
                      MoveMotr(miWRK_YBtm, piWRK_YBContect) ;
                      Step.iCycle++;
                      return false ;
                  }
                  else {
                      Step.iCycle = 16 ;
                      return false ;
                  }

        case  15: if(!MoveMotr(miWRK_YBtm, piWRK_YBContect)) return false ;
                  Step.iCycle++;
                  return false;


        case  16: if(OM.MstOptn.bGripTest) { if(!MoveActr(aiWRK_Cmp,ccBwd)) return false ; } //����ö ����� ��û����.
                  OM.DevOptn.iContactPinCnt++;
                  if(OM.DevOptn.iLimContactPinCnt < OM.DevOptn.iContactPinCnt) {
                      EM_SetErr(eiWRK_LimPinCnt);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;


        case  17: if(!m_tmTemp.OnDelay(true ,OM.DevOptn.iPrbSttDelay)) return false ;

                  if(OM.CmnOptn.bIgnrInsp) {
                      bPb1Rslt = true ;
                      bPb2Rslt = true ;
                  }
                  else {
                      bPb1Rslt = false ;
                      bPb2Rslt = false ;
                      if(OM.MstOptn.bByChInsp) PCM.Start(1);
                      else                     PCM.Start( );
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  18: if(m_tmTemp.OnDelay(true,OM.DevOptn.i1ChLedTime*100 + 1000)) {
                      if(!bPb2Rslt){
                          MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                          Step.iCycle = 30 ;
                          return false ;
                      }
                  }

                  if(!bPb1Rslt) return false ;
                  if(OM.MstOptn.bByChInsp) PCM.Start(2);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  19: if(m_tmTemp.OnDelay(true,OM.DevOptn.i2ChLedTime*100 + 1000)) {
                      if(!bPb2Rslt){
                          MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                          Step.iCycle = 30 ;
                          return false ;
                      }
                  }
                  if(!bPb2Rslt) return false ;

                  if(OM.MstOptn.bGripTest) MoveActr(aiWRK_Cmp,ccFwd); //����ö ����� ��û����.
                  MoveMotr(miWRK_YBtm , piWRK_YBWork) ;      // JH
                  Step.iCycle++;
                  return false ;

        case  20: if(OM.MstOptn.bGripTest) { if(!MoveActr(aiWRK_Cmp,ccFwd)) return false ; } //����ö ����� ��û����.
                  //����ŷ ���� �� �� �˻� ����.
                  if(!MoveMotr(miWRK_YBtm , piWRK_YBWork)) return false;  //JH
                  FindChip(r,c);
                  if(OM.CmnOptn.bIgnrInsp) {
                      for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                          for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRK].SetStat(i , c - j, csGood); //����Ÿ ����ŷ.
                      }
                  }
                  else {
                      r1 = PCM.CheckDataCnt(riWRK , sPb1Msg);
                      r2 = PCM.CheckDataCnt(riWRK , sPb2Msg);

                      //if(!r1)EM_SetErr(eiWRK_Probe1DataWrong) ; //������ ����ŷ.  ���߿� PCM�� ARAY �Ⱦ� ���� ���⼭ ������ �޾Ƽ� ����ŷ �Ѵ�.
                      //if(!r2)EM_SetErr(eiWRK_Probe2DataWrong) ;
                      if(!r1){ Step.iCycle = 10 ; m_iDataErrCntRe++ ; return false ;} //EM_SetErr(eiWRK_Probe1DataWrong) ; //������ ����ŷ.  ���߿� PCM�� ARAY �Ⱦ� ���� ���⼭ ������ �޾Ƽ� ����ŷ �Ѵ�.
                      if(!r2){ Step.iCycle = 10 ; m_iDataErrCntRe++ ; return false ;} //EM_SetErr(eiWRK_Probe2DataWrong) ;



                      if(r1 && r2) {
                          PCM.ClearDataCrnt();
                          PCM.GetDataCh(riWRK , sPb1Msg , PROBE_CH1 ,Stat.bRepeat);
                          PCM.GetDataCh(riWRK , sPb2Msg , PROBE_CH2 ,Stat.bRepeat);
                      }

                  }
                  iFail = 0;
                  for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                      iFail += DM.ARAY[riWRK].GetCntColStat(c - j, csFail); //����Ÿ ����ŷ.
                  }


                  //��˻� ����.

                  if (iFail >= OM.DevOptn.iLimInsFailCnt){
                      if(iInspCnt < OM.DevOptn.iReInspCnt){
                          for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                              for(int i =0 ; i < iMaxRow ; i++) DM.ARAY[riWRK].SetStat(i ,c - j, csUnkwn) ;
                          }
                          if(OM.MstOptn.bGripTest) MoveActr(aiWRK_Cmp,ccFwd); //����ö ����� ��û����.
                          MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                          MoveMotr(miWRK_ZTop,piWRK_ZTWait);
                          Step.iCycle = 11 ;
                          iInspCnt++;
                          return false;
                      }
                      //else {
                      //    if (OM.CmnOptn.bCheckFailCnt && DM.ARAY[riWRK].GetCntStat(csFail) >= OM.CmnOptn.iCheckFailCnt ) EM_SetErr(eiWRK_FailCntOver);
                      //    //Step.iCycle = 0;
                      //    //return true;
                      //}
                  }

                  //���κ� ���� �ɿ� ���� ī��Ʈ����
                  //MAX_PROB_COL == 4 have to check.
                  for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                      for(int i =0 ; i < iMaxRow ; i++) {
                          if(DM.ARAY[riWRK].GetStat(i ,c - j)==csFail) {
                              iPinFailCnt[i][OM.DevOptn.iContColCnt-j]++ ;
                              if(OM.DevOptn.iPinCnt && iPinFailCnt[i][OM.DevOptn.iContColCnt - j] > OM.DevOptn.iPinCnt) {
                                  EM_SetErr(eiWRK_PinFailCntOver);
                                  sPinCount = "(Front Left is 0,0) Fail Pin is"+AnsiString(i+1)+","+AnsiString(OM.DevOptn.iContColCnt-j) ;
                                  FM_MsgOk("Pin Check",sPinCount);
                                  iPinFailCnt[i][OM.DevOptn.iContColCnt-j] = 0 ;
                              }
                          }
//                          else {
//                              iPinFailCnt[i][j] = 0 ;
//                          }
                      }
                  }

                  //regrip ���� ����.
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  Step.iCycle++;
                  return false ;

        case  21: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  MoveActr(aiWRK_Cmp ,ccBwd  );
                  Step.iCycle++;
                  return false ;

        case  22: if(!MoveActr(aiWRK_Cmp , ccBwd )) return false ;
                  MoveMotr(miWRK_XCmp,piWRK_XReGrip);
                  Step.iCycle++;
                  return false ;

        case  23: if(!MoveMotr(miWRK_XCmp,piWRK_XReGrip)) return false ;

                  m_iReGripCnt++;

                  MoveActr(aiWRK_Cmp ,ccFwd  );
                  Step.iCycle++;
                  return false ;


        case  24: if(!MoveActr(aiWRK_Cmp ,ccFwd)) return false ;

                  //if(!OM.CmnOptn.bIgnrInsp && IO_GetY(yWRK_Probe1)) return false; //����� GetY�� �̷��� ���� ������.
                  //if(!OM.CmnOptn.bIgnrInsp && IO_GetY(yWRK_Probe2)) return false;

//                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait) ;
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait) ;

                  Step.iCycle++;
                  return false;

        case  25: if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
//                  if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;

                  //if (!DM.ARAY[riWRK].GetCntStat(csUnkwn) && DM.ARAY[riWRK].GetCntStat(csFail) >= OM.DevOptn.iLimAllFailCnt ) EM_SetErr(eiWRK_FailCntOver);
                  if (!DM.ARAY[riWRK].GetCntStat(csUnkwn) && OM.CmnOptn.bCheckFailCnt && DM.ARAY[riWRK].GetCntStat(csFail) >= OM.CmnOptn.iCheckFailCnt ) EM_SetErr(eiWRK_FailCntOver);

                  Step.iCycle=0;
                  return true;

        case  30: if(!MoveMotr(miWRK_YBtm , piWRK_YBWork)) return false;
                  iCmmErrCnt++;
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);
                  MoveActr(aiWRK_Cmp,ccFwd);

                  if(iCmmErrCnt < 4) { //�˻��� ���� �� �ȳ��� �ͼ� ��Ʈ���� ����.

                      Step.iCycle = 11 ;
                      if(!bPb1Rslt             ) Trace("Cmmunication","bPb1Rslt ComRetry");
                      if( bPb1Rslt && !bPb2Rslt) Trace("Cmmunication","bPb2Rslt ComRetry");
                      return false ;
                  }

                  if(iCmmErrCnt < 5) { //�� �ȵŸ� ���� �ٽ��غ�.

                      Step.iCycle = 11 ;
                      if(!bPb1Rslt             ) Trace("Cmmunication","bPb1Rslt ComRetry && SetDaegyeom()");
                      if( bPb1Rslt && !bPb2Rslt) Trace("Cmmunication","bPb2Rslt ComRetry && SetDaegyeom()");

                      if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //��� Eng ���κ� ����.
                      return false ;
                  }

                  //�׷��� �ȵǸ� ����.
                  if(!bPb1Rslt             ) EM_SetErr(eiWRK_FailProbeComm1);
                  if( bPb1Rslt && !bPb2Rslt) EM_SetErr(eiWRK_FailProbeComm2) ;
                  Step.iCycle = 0 ;
                  return false ;


    }

}

bool CWorkZone::CyclePostWork ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_CycleTO);
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

    bool bTemp1;
    bool bTemp2;
    bool bTemp3;
    bool bTemp4;
    bool bTemp5;

    bool bLastStrip ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: LT.AddDayInfoWorkStrp(1);
                  LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                  LT.AddDayInfoFailChip(DM.ARAY[riWRK].GetCntStat(csFail));

                  LT.AddLotInfoWorkStrp(1);
                  LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                  LT.AddLotInfoFailChip(DM.ARAY[riWRK].GetCntStat(csFail));

/*
                  bLastStrip = (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR2].GetLotNo() && !DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR1].GetLotNo() && !DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)) ||
//                               (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riLD1].GetLotNo() && !DM.ARAY[riLD1].CheckAllStat(csNone) && DM.ARAY[riPR1].CheckAllStat(csNone) &&  DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riLD1].GetLotNo() &&  DM.ARAY[riPR1].CheckAllStat(csNone) &&  DM.ARAY[riPR2].CheckAllStat(csNone)) || //�� �ּ� ó�� �κ� ����.
                               (LD1.GetWorkEnd() && DM.ARAY[riPR1].CheckAllStat(csNone) &&  DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               (DM.ARAY[riLD1].CheckAllStat(csEmpty) && LD1._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPR1].CheckAllStat(csNone) &&  DM.ARAY[riPR2].CheckAllStat(csNone)) ; //�������� �߿� �ϳ� �ɾ����.
*/
/*
                  bTemp1 = (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPRB].GetLotNo() && !DM.ARAY[riPRB].CheckAllStat(csNone));
                  bTemp2 = (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riLDR].GetLotNo() &&  DM.ARAY[riPRB].CheckAllStat(csNone) && !DM.ARAY[riLDR].CheckAllStat(csNone));
                  bTemp3 = (LDR.GetWorkEnd() && DM.ARAY[riPRB].CheckAllStat(csNone));
                  bTemp4 = (LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && !DM.ARAY[riLDR].GetCntStat(csUnkwn) && DM.ARAY[riPRB].CheckAllStat(csNone));
                  bTemp5 = (DM.ARAY[riLDR].CheckAllStat(csEmpty) && LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPRB].CheckAllStat(csNone));

                  bLastStrip = ( bTemp1 || bTemp2 || bTemp3 || bTemp4 || bTemp5 );
*/
                  bLastStrip = (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPRB].GetLotNo() && !DM.ARAY[riPRB].CheckAllStat(csNone)) ||
                               (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riLDR].GetLotNo() &&  DM.ARAY[riPRB].CheckAllStat(csNone) && !DM.ARAY[riLDR].CheckAllStat(csNone)) ||
                               (LDR.GetWorkEnd() && DM.ARAY[riPRB].CheckAllStat(csNone)) ||
                               (LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && !DM.ARAY[riLDR].GetCntStat(csUnkwn) && DM.ARAY[riPRB].CheckAllStat(csNone)) ||
                               (DM.ARAY[riLDR].CheckAllStat(csEmpty) && LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPRB].CheckAllStat(csNone)) ; //�������� �߿� �ϳ� �ɾ����.     */

                  if(bLastStrip) {
                      /*
                      Form = new TFrmRslt(Application);
                      Form -> edTime   -> Text = Now().FormatString("hh:nn:ss")     ;
                      Form -> edLotNo  -> Text = DM.ARAY[riWRK].GetLotNo  (        );
                      Form -> edFail   -> Text = LT.LotInfo.iFailChip ;//DM.ARAY[riWK2].GetCntStat(csFail  );
                      Form -> edWork   -> Text = LT.LotInfo.iWorkChip ;//DM.ARAY[riWK2].GetCntStat(csWork  );
                      Form -> Show() ;
                      */
                      m_bDispLotEnded = true ;


                      //memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

                      EndedLot.sLotNo     = LT.LotInfo.sLotNo     ;
                      EndedLot.iCrntLotNo = LT.LotInfo.iCrntLotNo ;
                      EndedLot.iLastLotNo = LT.LotInfo.iLastLotNo ;
                      EndedLot.sJobFile   = LT.LotInfo.sJobFile   ;
                      EndedLot.sOperator  = LT.LotInfo.sOperator  ;
                      EndedLot.iLotMgzCnt = LT.LotInfo.iLotMgzCnt ;
                      EndedLot.dStartTime = LT.LotInfo.dStartTime ;
                      EndedLot.dEndTime   = LT.LotInfo.dEndTime   ;
                      EndedLot.dRunTime   = LT.LotInfo.dRunTime   ;
                      EndedLot.dIdleTime  = LT.LotInfo.dIdleTime  ;
                      EndedLot.dJamTime   = LT.LotInfo.dJamTime   ;
                      EndedLot.dTotalTime = LT.LotInfo.dTotalTime ;
                      EndedLot.iStrpUPEH  = LT.LotInfo.iStrpUPEH  ;
                      EndedLot.iChipUPEH  = LT.LotInfo.iChipUPEH  ;
                      EndedLot.iStrpUPH   = LT.LotInfo.iStrpUPH   ;
                      EndedLot.iChipUPH   = LT.LotInfo.iChipUPH   ;
                      EndedLot.iWorkStrp  = LT.LotInfo.iWorkStrp  ;
                      EndedLot.iWorkChip  = LT.LotInfo.iWorkChip  ;
                      EndedLot.iFailChip  = LT.LotInfo.iFailChip  ;

                      Trace("LastStrip",DM.ARAY[riWRK].GetLotNo().c_str());
                      LT.LotInfo.dEndTime = Now() ;
                      LT.WriteLotLog   ();
                      LT.WriteLotDayLog(); // �������� ������ ������Ʈ���� �ϰ� ���⼭ ����.
                      LT.bMESEnded = true;

                  }

                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);
                  MoveActr(aiWRK_Cmp,ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
                  if(!MoveActr(aiWRK_Cmp,ccFwd        )) return false ;
                  MoveMotr(miWRK_XCmp,piWRK_XOut);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XCmp,piWRK_XOut)) return false ;
                  MoveActr(aiWRK_Cmp,ccBwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiWRK_Cmp,ccBwd)) return false ;
                  if(MM.GetManNo() == mcNoneCycle) PCM.ShiftArrayData(riWRK);
                  //else                             DM.ShiftArrayData(riWRK, riTmp);
        //          DM.ShiftArrayData(riWRK,riPSB); // juhyeon
                  m_iVisnErr = vsNomal;
//                  PSB._iReGripCnt = 0 ;
                  MoveMotr(miWRK_XCmp,piWRK_XGrip);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_XCmp,piWRK_XGrip)) return false ;
                  DM.ARAY[riWRK].SetStep(1);
//                  LT.WorkInfo.iFrmCnt++;
                  //DM.ARAY[riWRP].SetStat(csNone);
                  Step.iCycle = 0;
                  return true ;
    }
}

void CWorkZone::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CWorkZone::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CWorkZone::CheckStop()
{
    if(!MT_GetStop(miWRK_XCmp)) return false ;
    if(!MT_GetStop(miWRK_ZBtm)) return false ;
    if(!MT_GetStop(miWRK_ZTop)) return false ;
    if(!MT_GetStop(miWRK_YBtm)) return false ;
    if(!AT_Done(aiWRK_Cmp    )) return false ;

    return true ;
}

void CWorkZone::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\WorkZone.INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_iReGripCnt"  , m_iReGripCnt  );
        UserINI.Load(sPath, "Member" , "m_sLastStrip"  , m_sLastStrip  );
        UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iReGripCnt"  , m_iReGripCnt  );
        UserINI.Save(sPath, "Member" , "m_sLastStrip"  , m_sLastStrip  );
        UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    }
}
//---------------------------------------------------------------------------

void CWorkZone::HeatProbe1Ch(DWORD _cbInQue) // Juhyeon �ּ� ����
{
    //Sleep(1);
    int iLen; // int�� ���� ����
    const int iBuffSize = 1024 ; // ������ int�� ���� ������ ũ�� ����
    BYTE RcvBuff[iBuffSize]; // BYTE ��ü RcvBuff ����, ���� ������ ���� ������ ũ�⸸ŭ �迭�� ����
    String sRcvMsg     = "" ; // ���ڿ� sRcvMsg ����
    String sRcvMsgDrop = "" ; // ���ڿ� sRcvMsgDrop ����, ����� ���°���?

    static BYTE sRcvMsgByte[iBuffSize]; // ���� ���� sRcvMsgByte ����
    static int  iRcvMsgIndx = 0 ; // ���� ���� int�� iRcvMsgIndx ����

    if(iRcvMsgIndx ==0){ // iRcvMsgIndx ���� 0�� ��
        memset(&sRcvMsgByte, 0 , iBuffSize); // sRcvMsgByte �迭�� iBuffSize ���� 1024 ũ�⸸ŭ �޸� �ʱ�ȭ
    }

    memset(&RcvBuff, 0 , iBuffSize); // RcvBuff �迭�� iBuffSize ���� 1024 ũ�⸸ŭ �޸� �ʱ�ȭ
    iLen = RS232C_Pb1ch->ReadData(_cbInQue, RcvBuff);

    AnsiString sTempMsg = (char *)RcvBuff ;
    if(sTempMsg.Pos("OK\r")) return ;

    for(int i = 0 ; i < iLen ; i++){
        sRcvMsgByte[iRcvMsgIndx+i] = RcvBuff[i];
    }
    iRcvMsgIndx += iLen ;

    bool bFinished = false ;
    for(int i = 0 ; i < iBuffSize ; i++) {
        if(sRcvMsgByte[i] == '[') {
            bFinished = true ;
            break ;
        }
    }

    if(!bFinished/*!sRcvMsg.Pos("[")*/) return ; //------------------------------ �޼��� ���� �Ϸ�.
    sRcvMsgDrop = (char*)sRcvMsgByte ;

    char cTemp;
    int  iBuffCnt   = sRcvMsgDrop.Pos("RDROP") + 4;
    int  iBuffTemp  = sRcvMsgDrop.Pos("RDROP") - 2; //RDROP <<�� ��� �κ� ���� ��ġ.
    int  iHexCnt    = iBuffCnt + 5;                 //BuffCnt + 5 ���� ��簪�� �Է� �Ǿ� �ִ�.
    for(int i = 0 ; i < iBuffTemp; i++){            //Drop�� �������� �ޱ�...
        cTemp = sRcvMsgByte[i] ;
        sRcvMsg += cTemp;
    }
    for(int j = iHexCnt ; j < iBuffSize ; j++){  //Drop���� ��� ���̶� ������ �κ� �Է�.
        cTemp = sRcvMsgByte[j];
        sRcvMsg += cTemp;
    }


    String sRcvTemp = "" ;
    String sRcvQue[4];
    String sTemp ;


    for(int i = 0 ; i <4 ; i++){
        sRcvQue[i] = sRcvTemp.sprintf("%02x", sRcvMsgByte[iBuffCnt+i]);
    }
    for(int j = 0 ; j < 4 ; j++){
        sRcvQue[j].sprintf("%s", convertHex2Bin(sRcvQue[j].c_str()));
    }
    sTemp = sRcvQue[1] + sRcvQue[0];
    WRK.sPb1Bin[0] = sTemp;
    //Label1 -> Caption = sTemp;
    sTemp = sRcvQue[3] + sRcvQue[2];
    WRK.sPb1Bin[1] = sTemp;


    //�ӽ� ȭ�� ���÷��̿�... 1Ĩ�� ��� �ؼ� ����.
    //Ȯ�� �ϰ� ���߿� �ּ� �ϴ��� ���ļ� 3Ĩ ���� ���� �ְ� �ϴ���...
    WRK.sPb1Vf[0] = "" ;
    WRK.sPb1Vf[1] = "" ;

    AnsiString sCh ;
    AnsiString sChipData ;
    AnsiString sRcvDataTemp = sRcvMsg ;
    double     dChipData ;
    int iCnt = -1 ;

    for(int j=0; j> -OM.DevOptn.iContColCnt / 2; j-- ) { // iContColCnt = 4
       for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i--) {
           for(int k=0; k< OM.DevOptn.iDgChipCnt; k++ ) {//��� Ĩ ī��Ʈ�� �ƴ϶� �׳� Ĩī��Ʈ�� �ٲٰ� GL ������ ����ؾߵ�.
               iCnt++ ;
               sCh.sprintf("RL%02d",iCnt);
               sChipData = sRcvDataTemp.SubString(sRcvDataTemp.Pos(sCh)+5,4); //���� �����͸� ©��.
               sRcvDataTemp.Delete(sRcvDataTemp.Pos(sCh),10);
               sChipData = sChipData.SubString(0,2) + "." + sChipData.SubString(3,4) + " ";
               WRK.sPb1Vf[-j] += sChipData ;

               // juhyeon
               WRK.sTempPb1Vf[-j] = WRK.sPb1Vf[-j] ;
           }
       }
    }

    if(FrmProbe)FrmProbe -> edPb1ChData  -> Text = sRcvMsg ;
    //if(FrmProbe)FrmProbe -> edPb1ChData2 -> Text = sRcvMsg ;

    if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb1ch_Msg",sRcvMsg.c_str());

    WRK.sPb1Msg = sRcvMsg ;
        sRcvMsg = "" ;
    WRK.bPb1Rslt = true ;
    iRcvMsgIndx = 0 ;
}

void CWorkZone::HeatProbe2Ch(DWORD _cbInQue)
{
//Sleep(1);
    int iLen;
    const int iBuffSize = 1024 ;
    BYTE RcvBuff[iBuffSize];
    String sRcvMsg     = "" ;
    String sChipMsg    = "" ;
    String sZnrMsg     = "" ; 
    String sRcvMsgDrop = "" ;

    static BYTE sRcvMsgByte[iBuffSize];
    static int  iRcvMsgIndx = 0 ;

    if(iRcvMsgIndx ==0){
        memset(&sRcvMsgByte, 0 , iBuffSize);
    }

    memset(&RcvBuff, 0 , iBuffSize);
    iLen = RS232C_Pb2ch->ReadData(_cbInQue, RcvBuff);

    AnsiString sTempMsg = (char *)RcvBuff ;
    if(sTempMsg.Pos("OK\r")) return ;

    for(int i = 0 ; i < iLen ; i++){
        sRcvMsgByte[iRcvMsgIndx+i] = RcvBuff[i];
    }
    iRcvMsgIndx += iLen ;

    bool bFinished = false ;
    for(int i = 0 ; i < iBuffSize ; i++) {
        if(sRcvMsgByte[i] == '[') {
            bFinished = true ;
            break ;
        }
    }

    if(!bFinished/*!sRcvMsg.Pos("[")*/) return ; //------------------------------ �޼��� ���� �Ϸ�.
    sRcvMsgDrop = (char*)sRcvMsgByte ;

    char cTemp;
    int  iBuffCnt   = sRcvMsgDrop.Pos("RDROP") + 4;
    int  iBuffTemp  = sRcvMsgDrop.Pos("RDROP") - 2; //RDROP <<�� ��� �κ� ���� ��ġ.
    int  iHexCnt    = iBuffCnt + 5;                 //BuffCnt + 5 ���� ��簪�� �Է� �Ǿ� �ִ�.
    for(int i = 0 ; i < iBuffTemp; i++){            //Drop�� �������� �ޱ�...
        cTemp = sRcvMsgByte[i] ;
        sRcvMsg += cTemp;
    }
    for(int j = iHexCnt ; j < iBuffSize ; j++){  //Drop���� ��� ���̶� ������ �κ� �Է�.
        cTemp = sRcvMsgByte[j];
        sRcvMsg += cTemp;
    }

    String sRcvTemp = "" ;
    String sRcvQue[4];
    String sTemp ;


    for(int i = 0 ; i <4 ; i++){
        sRcvQue[i] = sRcvTemp.sprintf("%02x", sRcvMsgByte[iBuffCnt+i]);
    }
    for(int j = 0 ; j < 4 ; j++){
        sRcvQue[j].sprintf("%s", convertHex2Bin(sRcvQue[j].c_str()));
    }
    sTemp = sRcvQue[1] + sRcvQue[0];
    WRK.sPb2Bin[0] = sTemp;
    //Label1 -> Caption = sTemp;
    sTemp = sRcvQue[3] + sRcvQue[2];
    WRK.sPb2Bin[1] = sTemp;


    //�ӽ� ȭ�� ���÷��̿�... 1Ĩ�� ��� �ؼ� ����.
    //Ȯ�� �ϰ� ���߿� �ּ� �ϴ��� ���ļ� 3Ĩ ���� ���� �ְ� �ϴ���...
    WRK.sPb2Vf[0] = "" ;
    WRK.sPb2Vf[1] = "" ;

    AnsiString sCh ;
    AnsiString sChipData , sZnrData;
    AnsiString sRcvDataTemp = sRcvMsg ;
    double     dChipData ;
    int iCnt = -1 ;
    for(int j=0; j>-OM.DevOptn.iContColCnt / 2; j-- ) {
       for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --) {
           for(int k=0; k< OM.DevOptn.iDgChipCnt; k++ ) {//��� Ĩ ī��Ʈ�� �ƴ϶� �׳� Ĩī��Ʈ�� �ٲٰ� GL ������ ����ؾߵ�.
               iCnt++ ;
               sCh.sprintf("RL%02d",iCnt);
               sChipData = sRcvDataTemp.SubString(sRcvDataTemp.Pos(sCh)+5,4); //���� �����͸� ©��.
               sRcvDataTemp.Delete(sRcvDataTemp.Pos(sCh),10);
               sChipData = sChipData.SubString(0,2) + "." + sChipData.SubString(3,4) + " ";
               WRK.sPb2Vf[-j] += sChipData ;
           }
       }
    }

    if(FrmProbe)FrmProbe -> edPb2ChData  -> Text = sRcvMsg ;
    //if(FrmProbe)FrmProbe -> edPb2ChData2 -> Text = sRcvMsg ;

    if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb2ch_Msg",sRcvMsg.c_str());

    WRK.sPb2Msg = sRcvMsg ;
    sRcvMsg = "" ;
    WRK.bPb2Rslt = true ;
    iRcvMsgIndx = 0 ;
}
