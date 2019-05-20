//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "WorkZone.h"
#include "PostBuff.h"
//---------------------------------------------------------------------------
#include "Rs232Man.h"
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
#include "SAInterface.h"
#include "FormProbe.h"
//#include "FormRslt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
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
    m_sPartName = "WorkZone " ;

    RS232C_Pb1ch   = new TRS232C;
    RS232C_Pb2ch   = new TRS232C;

    if(!RS232C_Pb1ch->Open(0)) ShowMessage("RS-232C Probe 1 Ch Open Failed!");
    else                       RS232C_Pb1ch->CallBackReg(HeatProbe1Ch);
    if(!RS232C_Pb2ch->Open(1)) ShowMessage("RS-232C Probe 2 Ch Open Failed!");
    else                       RS232C_Pb2ch->CallBackReg(HeatProbe2Ch);

    PCM.SetDaegyeom();
    
    Reset();
    Load(true);

    DM.ARAY[riWRK].SetStep(m_iLastStep);
    InitCycleName();

    bOverFail = false;
    bYieldErr = false;

    iOverFail = 0;

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
    m_iLastStep = DM.ARAY[riWRK].GetStep();
    Load(false);

    delete RS232C_Pb1ch  ;
    delete RS232C_Pb2ch  ;
}

bool CWorkZone::FindChip(int &r , int &c )
{
    c = DM.ARAY[riWRK].FindLastCol(csUnkwn) ;

    return (c > -1 && c > -1);
}

double CWorkZone::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r , c , fc , iMaxCol;
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();
    FindChip(r , c);
    fc = (iMaxCol - c) -1 ; //횡으로 플립된값.

    double dPos = 0.0;
    int    iGrCnt ;
    bool   bGrGrip;
    double dReGripPos;

    if(OM.DevInfo.iColGrCnt > 0){
        iGrCnt      =  OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt ;
        bGrGrip     =  fc % iGrCnt ;
    }
    else {
        bGrGrip = false;
    }

    double dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip) +
                         (OM.DevInfo.dColPitch * OM.DevOptn.iContColCnt) + OM.DevOptn.dWRKPtchOfs;

    double dCmpGrGap   = OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ;

    /*
    if(DM.ARAY[riWRK].GetStep() == 0) { dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip); dReGripPos  = PM.GetValue(miWRK_XCmp , pvWRK_XGrip); }
    else                                dReGripPos  = PM.GetValue(miWRK_XCmp , pvWRK_XGrip) - OM.DevOptn.dWRKReGripOfs;

    if(fc == 0) dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip);

    if(_iMotr == miWRK_XCmp){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piWRK_XWait       : dPos = PM.GetValue (_iMotr , pvWRK_XWait      ); break ;
            case piWRK_XGrip       : dPos = dReGripPos                              ; break ;
            case piWRK_XWorkStart  : dPos = PM.GetValue (_iMotr , pvWRK_XWorkStart ); break ;
            case piWRK_XWork       : if (bGrGrip) dPos = dCmpMovePos + dCmpGrGap    ;
                                     else         dPos = dCmpMovePos                ; break ;
        }
    }
    */
    double dXGripOfs = PM.GetValue(miWRK_XCmp , pvWRK_XGrip) - OM.DevOptn.dGripOfs;
    if(fc == 0) dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip);

    if(OM.MstOptn.bUsedXAxis){
        if(_iMotr == miWRK_XBtm){
            switch(_iPstnId){
                default            : dPos = MT_GetCmdPos(_iMotr                         ); break ;
                case piWRK_XBWait  : dPos = PM.GetValue (_iMotr , pvWRK_XBWait          ); break ;
                case piWRK_XBWork  : dPos = PM.GetValue (_iMotr , pvWRK_XBWork          ); break ;
            }
        }

        if(_iMotr == miWRK_XCmp){
            switch(_iPstnId) {
                default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
                case piWRK_XWait       : dPos = PM.GetValue (_iMotr , pvWRK_XWait      ); break ;
                case piWRK_XGrip       : dPos = PM.GetValue (_iMotr , pvWRK_XGrip      ); break ;
                case piWRK_XWorkStart  : dPos = PM.GetValue (_iMotr , pvWRK_XWorkStart ); break ;
                case piWRK_XWork       : if (bGrGrip) dPos = dCmpMovePos + dCmpGrGap    ;
                                         else         dPos = dCmpMovePos                ; break ;
                case piWRK_XGripOfs    : dPos = dXGripOfs                               ; break ;
            }
        }

        else if(_iMotr == miWRK_ZTop){
            switch(_iPstnId) {
                default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
                case piWRK_ZTWait      : dPos = PM.GetValue (_iMotr , pvWRK_ZTWait          ); break ;
                case piWRK_ZTWork      : dPos = PM.GetValue (_iMotr , pvWRK_ZTWork          ); break ;
            }
        }
        else if(_iMotr == miWRK_ZBtm){
            switch(_iPstnId) {
                default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
                case piWRK_ZBWait      : dPos = PM.GetValue (_iMotr , pvWRK_ZBWait          ); break ;
                case piWRK_ZBWork      : dPos = PM.GetValue (_iMotr , pvWRK_ZBWork          ); break ;
                case piWRK_ZBChange    : dPos = PM.GetValue (_iMotr , pvWRK_ZBChange        ); break ;
            }
        }
        else if(_iMotr == miWRK_YBtm){
            switch(_iPstnId){
                default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
                case piWRK_YBWait      : dPos = PM.GetValue (_iMotr , pvWRK_YBWait          ); break ;
                case piWRK_YBWork      : dPos = PM.GetValue (_iMotr , pvWRK_YBWork          ); break ;
                case piWRK_YBChange    : dPos = PM.GetValue (_iMotr , pvWRK_YBChange        ); break ;
            }
        }
    }
    else if(!OM.MstOptn.bUsedXAxis){
        if(_iMotr == miWRK_XCmp){
            switch(_iPstnId) {
                default                : dPos = MT_GetCmdPos(_iMotr                    ); break ;
                case piWRK_XWait       : dPos = PM.GetValue (_iMotr , pvWRK_XWait      ); break ;
                case piWRK_XGrip       : dPos = PM.GetValue (_iMotr , pvWRK_XGrip      ); break ;
                case piWRK_XWorkStart  : dPos = PM.GetValue (_iMotr , pvWRK_XWorkStart ); break ;
                case piWRK_XWork       : if (bGrGrip) dPos = dCmpMovePos + dCmpGrGap    ;
                                         else         dPos = dCmpMovePos                ; break ;
                case piWRK_XGripOfs    : dPos = dXGripOfs                               ; break ;
            }
        }

        else if(_iMotr == miWRK_ZTop){
            switch(_iPstnId) {
                default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
                case piWRK_ZTWait      : dPos = PM.GetValue (_iMotr , pvWRK_ZTWait          ); break ;
                case piWRK_ZTWork      : dPos = PM.GetValue (_iMotr , pvWRK_ZTWork          ); break ;
            }
        }
        else if(_iMotr == miWRK_ZBtm){
            switch(_iPstnId) {
                default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
                case piWRK_ZBWait      : dPos = PM.GetValue (_iMotr , pvWRK_ZBWait          ); break ;
                case piWRK_ZBWork      : dPos = PM.GetValue (_iMotr , pvWRK_ZBWork          ); break ;
                case piWRK_ZBChange    : dPos = PM.GetValue (_iMotr , pvWRK_ZBChange        ); break ;
            }
        }
        else if(_iMotr == miWRK_YBtm){
            switch(_iPstnId){
                default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
                case piWRK_YBWait      : dPos = PM.GetValue (_iMotr , pvWRK_YBWait          ); break ;
                case piWRK_YBWork      : dPos = PM.GetValue (_iMotr , pvWRK_YBWork          ); break ;
                case piWRK_YBChange    : dPos = PM.GetValue (_iMotr , pvWRK_YBChange        ); break ;
            }
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

    bool isContect    = !MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBWait)) ||
                        !MT_CmprPos(miWRK_ZTop , PM.GetValue(miWRK_ZTop,pvWRK_ZTWait)) ;

    bool isXCmpMove   = !MT_GetStopInpos(miWRK_XCmp);
    bool isXCmpWorkStt=  MT_CmprPos(miWRK_XCmp , PM.GetValue(miWRK_XCmp,pvWRK_XWorkStart)) && MT_GetStop(miWRK_XCmp);

    bool isZbtmChange =  MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBChange)) && MT_GetStop(miWRK_ZBtm);
    bool isZbtmWork   =  MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBWork  )) && MT_GetStop(miWRK_ZBtm);

    bool isYbtmWait   =  MT_CmprPos(miWRK_YBtm , PM.GetValue(miWRK_YBtm,pvWRK_YBWait  )) && MT_GetStop(miWRK_YBtm);
    bool isXHoldFwd   = !AT_Complete(aiWRK_Hold , ccBwd);

    if(OM.MstOptn.bUsedXAxis){
        if(_iMotr == miWRK_XBtm){
            //if(isXHoldFwd){ sMsg = "Work Zone Hold Cylinder is Fwd"; bRet = false;}
            switch(_iPstnId){
                default                : break ;
                case piWRK_XBWait      : break ;
                case piWRK_XBWork      : break ;
            }
        }
    }

    else if(_iMotr == miWRK_XCmp){
        switch(_iPstnId) {
            default                   :                                                                            break ;
            case piWRK_XWait          :                                                                            break ;
            case piWRK_XGrip          :                                                                            break ;
            case piWRK_XWorkStart     :  if(isContect) {sMsg = "During Inspection"               ; bRet = false; } break ;
            case piWRK_XWork          :                                                                            break ;
            case piWRK_XGripOfs       :                                                                            break ;
        }
    }
    else if(_iMotr == miWRK_ZTop){
        switch(_iPstnId) {
            default                   :  break ;
            case piWRK_ZTWait         :  break ;
            case piWRK_ZTWork         :  if(isXCmpMove   ) {sMsg = MT_GetName(miWRK_XCmp)+" is Moveing"            ; bRet = false ; } break;
        }
    }
    else if(_iMotr == miWRK_ZBtm){
        if(!isYbtmWait  ) {sMsg = MT_GetName(miWRK_YBtm)+" is Not Wait Position"  ; bRet = false ; }
        switch(_iPstnId) {
            default                   :  break ;
            case piWRK_ZBWait         :  break ;
            case piWRK_ZBChange       :  break ;
            case piWRK_ZBWork         :  if(isXCmpMove   ) {sMsg = MT_GetName(miWRK_XCmp)+" is Moveing"            ; bRet = false ; } break ;
        }
    }
    else if(_iMotr == miWRK_YBtm){
        if(!isZbtmChange && !isZbtmWork) {sMsg = MT_GetName(miWRK_YBtm)+" is Not Wait Position"  ; bRet = false ;}
        //사이드 뷰 땜에 Z축 Work 위치에서 이동 떔에 주석...JS
        switch(_iPstnId) {
            default                   :  break ;
            case piWRK_YBWait         :  break ;
            case piWRK_YBWork         :  break ;
            case piWRK_YBChange       :  if(!isZbtmChange) {sMsg = MT_GetName(miWRK_YBtm) + " is not Change Position" ; bRet = false; break ;}
        }
    }

    else{
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

    else if(_iActr == aiWRK_Hold){
        if( !_bFwd) {
            if(isXMoving) { sMsg = MT_GetName(miWRK_XCmp)+" is Moving" ; bRet = false ;}
        }
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

bool CWorkZone::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CWorkZone::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CWorkZone::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiWRK_HomeTO);
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
                 if(OM.MstOptn.bUsedXAxis){
                     MT_DoHome(miWRK_XBtm);
                     AT_MoveCyl(aiWRK_Hold , ccBwd);
                 }
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiWRK_Cmp , ccBwd))return false ;
                 if(OM.MstOptn.bUsedXAxis && !AT_MoveCyl(aiWRK_Hold , ccBwd))return false;
                 if(OM.MstOptn.bUsedXAxis && !MT_GetHomeEnd(miWRK_XBtm))return false;
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
                 return false;

        case 13: if(!MT_GetHomeEnd(miWRK_YBtm))return false;
                 MT_GoAbsMan(miWRK_YBtm, GetMotrPos(miWRK_YBtm,piWRK_YBWait));
                 Step.iHome++;
                 return false;

        case 14: if(!MT_GoAbsMan(miWRK_YBtm, GetMotrPos(miWRK_YBtm,piWRK_YBWait)))return false;
                 MT_GoAbsMan(miWRK_ZBtm, GetMotrPos(miWRK_ZBtm,piWRK_ZBWait));
                 MT_GoAbsMan(miWRK_ZTop, GetMotrPos(miWRK_ZTop,piWRK_ZTWait));
                 MT_GoAbsMan(miWRK_XCmp, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miWRK_ZBtm, GetMotrPos(miWRK_ZBtm,piWRK_ZBWait)))return false ;
                 if(!MT_GoAbsMan(miWRK_ZTop, GetMotrPos(miWRK_ZTop,piWRK_ZTWait)))return false ;
                 if(OM.MstOptn.bUsedXAxis) MT_GoAbsMan(miWRK_XBtm, GetMotrPos(miWRK_XBtm,piWRK_XBWait));
                 Step.iHome++;
                 return false;

        case 16: if(OM.MstOptn.bUsedXAxis && !MT_GoAbsMan(miWRK_XBtm, GetMotrPos(miWRK_XBtm,piWRK_XBWait))) return false;
                 if(!MT_GoAbsMan(miWRK_XCmp, m_dLastIndPos))return false ;
                 Step.iHome = 0;
                 return true ;


    }
}

bool CWorkZone::Autorun(void) //오토런닝시에 계속 타는 함수.
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
        //Cycle동작에 관련 되어 있으면 SetSubStep으로 사용.
        //Cycle동작말고 자재와 관련 있으면 SetStep으로 사용함. JS 2013. 9. 11.
        if (Stat.bReqStop)return false ;

        FindChip(r,c);

        iMaxCol = DM.ARAY[riWRK].GetMaxCol();
        FindChip(r , c);

        int fc = (iMaxCol - c) -1  ; //원래 데이터를 뒤집은 것.

        bool bExstWorkOkPst   = !DM.ARAY[riPSB].CheckAllStat(csNone) && (PSB.m_iReGripCnt == 1 || DM.ARAY[riPSB].GetStep() == 1 || DM.ARAY[riWRK].GetStep() < OM.DevOptn.iWRKMoveCnt / 2);
        //bool isPSBIdle        =
        bool isPSBWorkZone    = PSB.GetSeqStep() == CPostBuff::scContact ;

        bool isCycleIn        = DM.ARAY[riWRK].CheckAllStat(csUnkwn) && DM.ARAY[riWRK].GetSubStep() == 0;
        bool isCycleWork      = DM.ARAY[riWRK].GetCntStat  (csUnkwn) && DM.ARAY[riWRK].GetStep() < OM.DevOptn.iWRKMoveCnt && (DM.ARAY[riPSB].CheckAllStat(csNone) || bExstWorkOkPst);
        bool isCycleOut       =!DM.ARAY[riWRK].GetCntStat  (csUnkwn) && !DM.ARAY[riWRK].CheckAllStat(csNone ) && DM.ARAY[riPSB].CheckAllStat(csNone) && !isPSBWorkZone;

        bool isCycleEnd      =  DM.ARAY[riWRK].CheckAllStat(csNone ) ;

        //모르는 스트립에러.
        if(isCycleIn || isCycleWork){
            if(DM.ARAY[riWRK].CheckAllStat(csNone) && !DM.ARAY[riPSB].GetSubStep() && (IO_GetX(xWRK_Detect1) || IO_GetX(xWRK_Detect2))) EM_SetErr(eiWRK_Unknown) ;
        }

        //스트립 사라짐.
        if(isCycleOut && !OM.CmnOptn.bDryRun){
            if(DM.ARAY[riWRK].GetCntExist() && !IO_GetX(xWRK_Detect1) && !IO_GetX(xWRK_Detect2)) EM_SetErr(eiWRK_Dispr) ;
        }

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleIn        ) {Trace(m_sPartName.c_str(),"CycleIn         Stt"); Step.iSeq = scIn        ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleWork      ) {Trace(m_sPartName.c_str(),"CycleWork       Stt"); Step.iSeq = scWork      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut       ) {Trace(m_sPartName.c_str(),"CycleOut        Stt"); Step.iSeq = scOut       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd       ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default          :                       Trace(m_sPartName.c_str(),"default        End");Step.iSeq = scIdle ;  return false ;
        case scIdle      :                                                                                             return false ;
        case scIn        : if(CycleIn       ()){ Trace(m_sPartName.c_str(),"CycleIn        End");Step.iSeq = scIdle ;} return false ;
        case scWork      : if(CycleWork     ()){ Trace(m_sPartName.c_str(),"CycleWork      End");Step.iSeq = scIdle ;} return false ;
        case scOut       : if(CycleOut      ()){ Trace(m_sPartName.c_str(),"CycleOut       End");Step.iSeq = scIdle ;} return false ;
    }

    return false ;

}

bool CWorkZone::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    Stat.bReqStop = true ;
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CWorkZone::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    //During the auto run, do not stop.
    if (Step.iSeq) return false; ///fail 시 계속 검사 할수도...  Stat.iReInspCnt = 0;

    Step.iToStart = 10 ;

    //Ok.
    return true;
}

bool CWorkZone::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: if(DM.ARAY[riWRK].CheckAllStat(csNone)) MoveActr(aiWRK_Cmp , ccBwd);
                 if(OM.MstOptn.bUsedXAxis) MoveMotr(miWRK_XBtm , piWRK_XBWait);
                 Step.iToStart++;
                 return false;

        case 11: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait))return false;
                 MoveMotr(miWRK_YBtm , piWRK_YBWait);
                 Step.iToStart ++;
                 return false ;

        case 12: if(DM.ARAY[riWRK].CheckAllStat(csNone) && !MoveActr(aiWRK_Cmp , ccBwd)) return false ;
                 if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                 MoveMotr(miWRK_ZTop,piWRK_ZTWait) ;
                 MoveMotr(miWRK_ZBtm,piWRK_ZBWait) ;
                 Step.iToStart ++ ;
                 return false ;

        case 13: if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait))return false ;
                 if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait))return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CWorkZone::ToStop(void) //스탑을 하기 위한 함수.
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

                 OM.SaveDevOptn(OM.m_sCrntDev);  //핀카운트 저장을 위해서.
                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CWorkZone::CycleIn(void)
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

        case  10: MoveActr(aiWRK_Cmp , ccBwd);
                  if(DM.ARAY[riWRK].GetLotNo() != m_sLastStrip) {
                      LT.LotOpen(DM.ARAY[riWRK].GetLotNo()) ;
                      m_sLastStrip = DM.ARAY[riWRK].GetLotNo() ;
                      bYieldErr = false;
                      Trace("First Strip",("LotOpen_" + DM.ARAY[riWRK].GetLotNo()).c_str());
                  }
                  if(OM.MstOptn.bUsedXAxis){
                      MoveMotr(miWRK_XBtm , piWRK_XBWait);
                  }
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiWRK_Cmp , ccBwd)) return false;
                  if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait))return false;
                  MoveMotr(miWRK_XCmp , piWRK_XWait );
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_XCmp , piWRK_XWait )) return false;
                  if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false;
                  MoveActr(aiWRK_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiWRK_Cmp , ccFwd)) return false;
                  MoveMotr(miWRK_XCmp , piWRK_XWorkStart);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miWRK_XCmp , piWRK_XWorkStart)) return false;
                  PCM.ClearData();
                  DM.ARAY[riWRK].ClearAllVf();
                  DM.ARAY[riWRK].ClearAllVr();
                  DM.ARAY[riWRK].SetSubStep(1);
                  DM.ARAY[riWRK].SetStep(0);
                  iOverFail = 0;
                  Step.iCycle = 0;
                  return true ;

        /*
        case  16: SV_Grab(viOri);
                  Step.iCycle++;
                  return false ;

        case  17: if(!SV_GetGrabEnd(viOri)) return false ;
                  SV_Inspect(viOri , false); //쓰레드 안씀.
                  SV_GetRslt(viOri , &OriRslt);

                  if(OriRslt.iRet != veOriOk) {
                      EM_SetErr(eiWRK_VisnNG);
                  }
                  */



                  Step.iCycle = 0;
                  return true ;

    }
}

bool CWorkZone::CycleWork()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    int iLedTime = OM.DevOptn.i1ChLedTime > OM.DevOptn.i2ChLedTime ? OM.DevOptn.i1ChLedTime * 100 : OM.DevOptn.i2ChLedTime * 100;
    int iZrTime  = OM.DevOptn.i1ChZenTime > OM.DevOptn.i2ChZenTime ? OM.DevOptn.i1ChZenTime * 100 : OM.DevOptn.i2ChZenTime * 100;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, iLedTime + iZrTime + 2000 )) {
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

    TRetResult OriRslt ;

    int r,c ;
    bool r1,r2 ;
    int iMaxRow,iMaxCol,icsWork,iFail;
    iMaxRow = DM.ARAY[riWRK].GetMaxRow();
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();

    //if(iReInspCnt <= 0) iReInspCnt = OM.DevOptn.iReInspCnt;

    static int  iReInspCnt = 0 ;
    static int  iCmmErrCnt = 0 ;
    static int  iChipCnt = OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt;
    int iChipTemp = iChipCnt - DM.ARAY[riWRK].GetCntStat(csUnkwn);
    AnsiString sPinCount ;

    //static bool bRepeatAtStart = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(DM.ARAY[riWRK].CheckAllStat(csUnkwn) && OM.MstOptn.bUsedVision){ //1호기 비전 없어서 Option처리 JH
                      SV_Grab(viOri);
                      m_tmTemp.Clear();
                      Step.iCycle++;
                      return false ;
                  }
                  Step.iCycle = 12;
                  return false;

        case  11: if(m_tmTemp.OnDelay(true , 1000)){
                      EM_SetErr(eiWRK_VisnReady);
                      Step.iCycle = 0 ;
                      return true;
                  }
                  if(!SV_GetGrabEnd(viOri)) return false ;
                  SV_Inspect(viOri , false); //쓰레드 안씀.
                  SV_GetRslt(viOri , &OriRslt);

                  if(OriRslt.iRet != veOriOk) {
                      EM_SetErr(eiWRK_VisnNG);
                      Step.iCycle = 0;
                      return true;
                  }
                  Step.iCycle++;
                  return false;

        //주의. 위에서 사용.
        case  12: if(OM.MstOptn.bUsedXAxis) {
                      MoveMotr(miWRK_XBtm , piWRK_XBWait);
                      MoveActr(aiWRK_Hold , ccBwd);
                  }
                  Step.iCycle++;
                  return false;

        case  13: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait)) return false;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccBwd)) return false;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveActr(aiWRK_Cmp  , ccBwd       );
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  if(!MoveActr(aiWRK_Cmp  , ccBwd       )) return false ;
                  if(iChipTemp < OM.DevInfo.iRowCnt * OM.DevOptn.iContColCnt) MoveMotr(miWRK_XCmp , piWRK_XGrip);
                  else                                                                                  MoveMotr(miWRK_XCmp , piWRK_XGripOfs);
                  Step.iCycle++;
                  return false;

        case  16: if(iChipTemp < OM.DevInfo.iRowCnt * OM.DevOptn.iContColCnt){
                      if(!MoveMotr(miWRK_XCmp , piWRK_XGrip   ))  return false ;
                  }
                  else {
                      if(!MoveMotr(miWRK_XCmp , piWRK_XGripOfs))  return false ;
                  }
                  MoveActr(aiWRK_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiWRK_Cmp , ccFwd)) return false;
                  MoveMotr(miWRK_XCmp , piWRK_XWork);
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveMotr(miWRK_XCmp , piWRK_XWork)) return false;
                  if(OM.MstOptn.bUsedXAxis)MoveActr(aiWRK_Hold , ccFwd);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  Step.iCycle++;
                  return false;

        case  19: if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccFwd)) return false;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  if(!OM.MstOptn.bUseReContact) MoveActr(aiWRK_Cmp  , ccBwd );
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  20: if(!OM.MstOptn.bUseReContact && !MoveActr(aiWRK_Cmp  , ccBwd)) return false;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWork)) return false ;
                  //if(!OM.MstOptn.bUseReContact) MoveMotr(miWRK_XCmp , piWRK_XGrip); 기존
                  if(!OM.MstOptn.bUseReContact) MoveMotr(miWRK_XCmp , piWRK_XGripOfs);
                  MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                  Step.iCycle++;
                  return false ;

        case  21: if(!MoveMotr(miWRK_YBtm, piWRK_YBWork)) return false ;
                  //if(!OM.MstOptn.bUseReContact && !MoveMotr(miWRK_XCmp , piWRK_XGrip)) return false;
                  if(!OM.MstOptn.bUseReContact && !MoveMotr(miWRK_XCmp , piWRK_XGripOfs)) return false;
                  if(OM.MstOptn.bUsedXAxis){
                      MoveMotr(miWRK_XBtm, piWRK_XBWork) ;
                  }
                  Step.iCycle++;
                  return false;

        case  22: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWork)) return false;
                  Step.iCycle++;
                  return false;

        //주의 밑에서 사용.
        case  23: OM.DevOptn.iContactPinCnt++;
                  if(OM.DevOptn.iLimContactPinCnt < OM.DevOptn.iContactPinCnt) {
                      EM_SetErr(eiWRK_LimPinCnt);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  24: if(!m_tmTemp.OnDelay(true ,OM.DevOptn.iPrbSttDelay)) return false ;
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

        case  25: if(m_tmTemp.OnDelay(true , OM.DevOptn.i1ChLedTime * 100 + 1000)) {
                      if(!bPb1Rslt) {
                      //if(!g_bPb1Rslt){
                      //EM_SetErr(eiWRK_FailProbeComm1) ;
                          if( OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_XBtm, piWRK_XBWait) ;
                          if(!OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_YBtm, piWRK_YBWait) ;
                          MoveActr(aiWRK_Cmp , ccFwd);
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }
                  //if(!g_bPbr1Rslt) return false;

                  if(!bPb1Rslt)return false;
                  Trace("11111","OKOKOKOKOK");
                  if(OM.MstOptn.bByChInsp) PCM.Start(2);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  26: if(m_tmTemp.OnDelay(true , OM.DevOptn.i2ChLedTime * 100 + 1000)) {
                      if(!bPb2Rslt) {
                      //if(!g_bPb2Rslt) {
                        //EM_SetErr(eiWRK_FailProbeComm2);
                          if( OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_XBtm, piWRK_XBWait) ;
                          if(!OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_YBtm, piWRK_YBWait) ;
                          MoveActr(aiWRK_Cmp , ccFwd);
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }
                  //if(!g_bPb2Rslt)return false;

                  if(!bPb2Rslt) return false;
                  Trace("222222","OKOKOKOKOK");
                  if(OM.MstOptn.bUsedXAxis) {
                      MoveMotr(miWRK_XBtm , piWRK_XBWait);
                      MoveActr(aiWRK_Hold , ccBwd);
                  }
                  Step.iCycle++;
                  return false ;

        case  27: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait)) return false;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccBwd       )) return false;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;

        case  28: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false;
                  //마스킹 패턴 및 재 검사 패턴.
                  FindChip(r,c);
                  if(OM.CmnOptn.bIgnrInsp) {
                      for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                          for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRK].SetStat(i , c - j, csGood); //데이타 마스킹.
                      }
                  }
                  else {
                      r1 = PCM.CheckDataCnt(riWRK , sPb1Msg);
                      r2 = PCM.CheckDataCnt(riWRK , sPb2Msg);

                      if(!r1){ Step.iCycle = 40 ; m_iDataErrCnt++ ; MoveActr(aiWRK_Cmp , ccFwd); return false ;} //EM_SetErr(eiWRK_Probe1DataWrong) ; //데이터 마스킹.  나중에 PCM에 ARAY 걷어 내고 여기서 데이터 받아서 마스킹 한다.
                      if(!r2){ Step.iCycle = 40 ; m_iDataErrCnt++ ; MoveActr(aiWRK_Cmp , ccFwd); return false ;} //EM_SetErr(eiWRK_Probe2DataWrong) ;

                      if(r1 && r2) {
                          PCM.ClearDataCrnt();
                          PCM.GetDataCh(riWRK , sPb1Msg , PROBE_CH1 ,Stat.bRepeat);
                          PCM.GetDataCh(riWRK , sPb2Msg , PROBE_CH2 ,Stat.bRepeat);
                      }
                  }
                  iFail = 0;
                  for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                      iFail += DM.ARAY[riWRK].GetCntColStat(c - j, csFail); //데이타 마스킹.
                  }

                  //재검사 패턴 및 리피트 테스트 패턴.
                  if (iFail >= OM.DevOptn.iLimInsFailCnt || Stat.bRepeat){
                      if(iReInspCnt < OM.DevOptn.iReInspCnt || Stat.bRepeat){
                          for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                              for(int i =0 ; i < iMaxRow ; i++) DM.ARAY[riWRK].SetStat(i ,c - j, csUnkwn) ;
                          }
                          MoveActr(aiWRK_Cmp , ccFwd);
                          Step.iCycle = 42 ;
                          iReInspCnt++;
                          return false;
                      }
                  }

                  //프로브 개별 핀에 대한 카운트에러
                  //MAX_PROB_COL == 4 have to check.
                  for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                      for(int i =0 ; i < iMaxRow ; i++) {
                          if(DM.ARAY[riWRK].GetStat(i ,c - j)==csFail) {
                              iPinFailCnt[i][OM.DevOptn.iContColCnt-j]++ ;
                              if(OM.DevOptn.iPinCnt && !OM.DevOptn.bSkipPinCnt && iPinFailCnt[i][OM.DevOptn.iContColCnt - j] > OM.DevOptn.iPinCnt) {
                                  EM_SetErr(eiWRK_PinFailCntOver);
                                  sPinCount = "(Front Left is 0,0) Fail Pin is"+AnsiString(i+1)+","+AnsiString(OM.DevOptn.iContColCnt-j) ;
                                  FM_MsgOk("Pin Check",sPinCount);
                                  iPinFailCnt[i][OM.DevOptn.iContColCnt-j] = 0 ;
                              }
                          }
                      }
                  }

                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  //옵션 추가후 변경. JS...
                  if(iFail >= OM.DevOptn.iFrtPrcFailCnt){
                      bOverFail = true;
                      bOverErr  = true;
                      iOverFail += iFail;
                  }
                  Step.iCycle++;
                  return false;

        case  29: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
                  MoveActr(aiWRK_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  30: if(!MoveActr(aiWRK_Cmp , ccBwd)) return false;
                  //MoveMotr(miWRK_XCmp , piWRK_XGrip);
                  MoveMotr(miWRK_XCmp , piWRK_XGripOfs);
                  Step.iCycle++;
                  return false;

        case  31: //if(!MoveMotr(miWRK_XCmp , piWRK_XGrip)) return false;
                  if(!MoveMotr(miWRK_XCmp , piWRK_XGripOfs)) return false;
                  if (!DM.ARAY[riWRK].GetCntStat(csUnkwn) && OM.CmnOptn.bCheckFailCnt && DM.ARAY[riWRK].GetCntStat(csFail) >= OM.CmnOptn.iCheckFailCnt ) EM_SetErr(eiWRK_FailCntOver);
                  DM.ARAY[riWRK].SetStep(DM.ARAY[riWRK].GetStep() + OM.DevOptn.iContColCnt);
                  m_iLastStep = DM.ARAY[riWRK].GetStep();
                  Step.iCycle=0;
                  return true;


        //PCM.CheckDataCnt 데이터 체크 Fail시 사용 스텝.
        case  40: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait))return false;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccBwd       )) return false;
                  Step.iCycle++;
                  return false;

        case  41: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        //밑에서 사용. 재검사 옵션 및 검사 실패시 사용.
        case  42: if(!MoveActr(aiWRK_Cmp , ccFwd)) return false;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  Step.iCycle++;
                  return false;

        case  43: MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  if(OM.MstOptn.bUsedXAxis)MoveActr(aiWRK_Hold , ccFwd);
                  Step.iCycle++;
                  return false;

        case  44: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccFwd )) return false;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  45: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWork)) return false ;
                  MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                  Step.iCycle++;
                  return false ;

        case  46: if(!MoveMotr(miWRK_YBtm, piWRK_YBWork)) return false ;
                  MoveMotr(miWRK_XBtm , piWRK_XBWork);
                  Step.iCycle++;
                  return false;

        case  47: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWork))return false;
                  Step.iCycle = 23;
                  return false;

        //검사 실패시 사용 되는 스텝.
        case  50: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait)) return false;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccBwd       )) return false;
                  Step.iCycle++;
                  return false;

        case  51: if(!MoveMotr(miWRK_YBtm, piWRK_YBWait)) return false ;
                  if(!MoveActr(aiWRK_Cmp , ccFwd)) return false;
                  iCmmErrCnt++;
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  if(iCmmErrCnt < 4) { //검사결과 가끔 잘 안날라 와서 리트라이 넣음.
                      Step.iCycle = 42 ;
                      if(!bPb1Rslt             ) Trace("Cmmunication","g_bPb1Rslt ComRetry");
                      if( bPb1Rslt && !bPb2Rslt) Trace("Cmmunication","g_bPb2Rslt ComRetry");
                      return false ;
                  }

                  if(iCmmErrCnt < 5) { //정 안돼면 세팅 다시해봄.

                      Step.iCycle = 42 ;
                      if(!bPb1Rslt             ) Trace("Cmmunication","g_bPb1Rslt ComRetry && SetDaegyeom()");
                      if( bPb1Rslt && !bPb2Rslt) Trace("Cmmunication","g_bPb2Rslt ComRetry && SetDaegyeom()");

                      PCM.SetDaegyeom();
                      //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //대겸 Eng 프로브 사용시.
                      return false ;
                  }

                  //그래도 안되면 에러.
                  if(!bPb1Rslt             ) EM_SetErr(eiWRK_FailProbeComm1);
                  if( bPb1Rslt && !bPb2Rslt) EM_SetErr(eiWRK_FailProbeComm2);
                  MoveActr(aiWRK_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  52: if(!MoveActr(aiWRK_Cmp , ccBwd)) return false;
                  Step.iCycle = 0 ;
                  return false ;
    }
}
bool CWorkZone::CycleOut()
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

    int r,c;
    bool r1,r2 ;
    FindChip(r,c);
    int iMaxRow,iMaxCol,iFail;
    iMaxRow = DM.ARAY[riWRK].GetMaxRow();
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();
    //static bool bErr = false ;
    AnsiString sPinCount ;

    int iWorkCnt, iGoodCnt, iFailCnt;

    double dYield;

    static int iCmmErrCnt = 0 ;

    static int iInspCnt = 0 ;
    static bool bLastStrip;
    //if(iReInspCnt <= 0) iReInspCnt = OM.DevOptn.iReInspCnt;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(bOverErr){
                      EM_SetErr(eiWRK_OverFail);
                      bOverErr = false;
                      Step.iCycle = 0;
                      return true;
                  }
                  Step.iCycle++;
                  return false;

        case  11: if(bOverErr) return false;
                  if(bOverFail){
                      iWorkCnt = DM.ARAY[riWRK].GetCntExist();
                      iGoodCnt = DM.ARAY[riWRK].GetCntStat(csGood);
                      iFailCnt = iWorkCnt - (iGoodCnt + iOverFail);
                      LT.AddDayInfoWorkStrp(1);
                      LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                      LT.AddDayInfoFailChip(iFailCnt);
                      LT.AddDayInfoDropChip(DM.ARAY[riWRK].GetCntStat(csDrop));
                      LT.AddDayInfoGoodChip(DM.ARAY[riWRK].GetCntStat(csGood));

                      LT.AddLotInfoWorkStrp(1);
                      LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                      LT.AddLotInfoFailChip(iFailCnt);
                      LT.AddLotInfoDropChip(DM.ARAY[riWRK].GetCntStat(csDrop));
                      LT.AddLotInfoGoodChip(DM.ARAY[riWRK].GetCntStat(csGood));

                      bOverFail = false;
                  }
                  else{
                      LT.AddDayInfoWorkStrp(1);
                      LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                      LT.AddDayInfoFailChip(DM.ARAY[riWRK].GetCntExist() - DM.ARAY[riWRK].GetCntStat(csGood));
                      LT.AddDayInfoDropChip(DM.ARAY[riWRK].GetCntStat(csDrop));
                      LT.AddDayInfoGoodChip(DM.ARAY[riWRK].GetCntStat(csGood));

                      LT.AddLotInfoWorkStrp(1);
                      LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
                      LT.AddLotInfoFailChip(DM.ARAY[riWRK].GetCntExist() - DM.ARAY[riWRK].GetCntStat(csGood));
                      LT.AddLotInfoDropChip(DM.ARAY[riWRK].GetCntStat(csDrop));
                      LT.AddLotInfoGoodChip(DM.ARAY[riWRK].GetCntStat(csGood));

                  }


                  bLastStrip = (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR2].GetLotNo() && !DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR1].GetLotNo() && !DM.ARAY[riPR1].CheckAllStat(csNone)  && DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               (DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riLDR].GetLotNo() &&(!DM.ARAY[riLDR].CheckAllStat(csNone)  && DM.ARAY[riPR1].CheckAllStat(csNone)  && DM.ARAY[riPR2].CheckAllStat(csNone))) ||
                               (LDR.GetWorkEnd() && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)) || //&& LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt ||
                               (DM.ARAY[riLDR].CheckAllStat(csEmpty) && LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)); //내려놓는 중에 하나 걸어놓음.


                               //기존.
                               //(DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR1].GetLotNo() && !DM.ARAY[riPR1].CheckAllStat(csNone)) ||
                               //(DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPR2].GetLotNo() && !DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               //(DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riLDR].GetLotNo() && (DM.ARAY[riPR1].CheckAllStat(csNone)  && DM.ARAY[riPR2].CheckAllStat(csNone))) ||
                               //(LDR.GetWorkEnd() && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)) ||
                               //(DM.ARAY[riLDR].CheckAllStat(csEmpty) && LDR._iMgzCnt == OM.DevOptn.iLotEndMgzCnt && DM.ARAY[riPR1].CheckAllStat(csNone) && DM.ARAY[riPR2].CheckAllStat(csNone)); //내려놓는 중에 하나 걸어놓음.

                  if(bLastStrip) {
                      m_bDispLotEnded = true ;
                      memcpy(&LT.EndInfo , &LT.LotInfo , sizeof(CLot::SLotInfo));

                      Trace("LastStrip",DM.ARAY[riWRK].GetLotNo().c_str());

                      //LotEnd할려고 하는데 PostBuff에서
                      //LT.LotInfo.dEndTime = Now() ;
                      LT.LotInfo.dEndTime = Now() ;
                      LT.WriteLotLog   ();
                      LT.WriteLotDayLog(); // 데이인포 갱신은 업데이트에서 하고 여기서 쓴다.
                      if(OM.DevOptn.bSkipPinCnt){
                          OM.DevOptn.bSkipPinCnt = !OM.DevOptn.bSkipPinCnt ;
                      }
                  }

                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  Step.iCycle++;
                  return false;

        //Case 18번에서 씀 바꿀때 주의.
        case  13: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  Step.iCycle++;
                  return false;

        case  14: MoveActr(aiWRK_Cmp , ccBwd);
                  //for(int t = 1; t < 43 ; t += 4){
                  //    DM.ARAY[riWRK].SetStat(0, t, csGood);
                  //}//대기 JS
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiWRK_Cmp , ccBwd)) return false;
                  MoveMotr(miWRK_XCmp , piWRK_XWait);
                  PCM.ShiftArrayData(riWRK);
                  DM.ShiftArrayData(riWRK,riPSB);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miWRK_XCmp , piWRK_XWait)) return false;
                  //DM.ARAY[riWRK].SetSubStep(0);
                  dYield = LT.LotInfo.iWorkChip ? (LT.LotInfo.iWorkChip - LT.LotInfo.iFailChip) / (float)LT.LotInfo.iWorkChip * 100 : 100 ;
                  //옵션 추가. 2개다 누가? 진섭이가
                  //1개는 bool 1개는 double ㅇㅋ?
                  if(dYield < OM.CmnOptn.dStandardYield && !OM.CmnOptn.bSkipYieldErr && !bYieldErr){
                      EM_SetErr(eiETC_YeildLow);
                      bYieldErr = true;
                  }

                  Step.iCycle=0;
                  return true;
    }
}

bool CWorkZone::CycleToolWait()
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


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait );
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait )) return false ;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait )) return false ;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait );
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait )) return false ;
                  Step.iCycle=0;
                  return true;

    }

}
bool CWorkZone::CycleToolChange()
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(OM.MstOptn.bUsedXAxis){
                      MoveMotr(miWRK_XBtm , piWRK_XBWait);
                      MoveActr(aiWRK_Hold , ccBwd);
                  }
                  Step.iCycle++;
                  return false;

        case  11: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold, ccBwd)) return false;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait  );
                  MoveMotr(miWRK_ZBtm , piWRK_ZBChange);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait  )) return false ;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBChange)) return false ;
                  MoveMotr(miWRK_YBtm , piWRK_YBChange);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_YBtm , piWRK_YBChange)) return false ;
                  Step.iCycle=0;
                  return true;

    }

}

bool CWorkZone::CycleNextMove()
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

    double dCmpMovePos = PM.GetValue(miWRK_XCmp , pvWRK_XGrip) + OM.DevInfo.dColPitch * OM.DevOptn.iContColCnt + OM.DevOptn.dWorkPtchOfs;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_XBtm , piWRK_XBWait);
                  Step.iCycle++;
                  return false;

        case  11: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  MoveActr(aiWRK_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  if(!MoveActr(aiWRK_Cmp , ccBwd)) return false ;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_XCmp , piWRK_XGrip );
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_XCmp , piWRK_XGrip )) return false ;
                  MoveActr(aiWRK_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiWRK_Cmp , ccFwd)) return false ;
                  MT_GoAbsMan(miWRK_XCmp , dCmpMovePos);
                  //MoveMotr(miWRK_XCmp , piWRK_XWork);
                  Step.iCycle++;
                  return false;

        case  15: //if(!MoveMotr(miWRK_XCmp , piWRK_XWork)) return false ;
                  if(!MT_GoAbsMan(miWRK_XCmp , dCmpMovePos)) return false;
                  MoveActr(aiWRK_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiWRK_Cmp , ccBwd)) return false ;
                  MoveMotr(miWRK_XCmp , piWRK_XGrip);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveMotr(miWRK_XCmp , piWRK_XGrip)) return false;
                  Step.iCycle=0;
                  return true;
    }

}
bool CWorkZone::CycleRptCntct()
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


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

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

        case  10: if(OM.MstOptn.bUsedXAxis) MoveMotr(miWRK_XBtm , piWRK_XBWait);
                  Step.iCycle++;
                  return false;

        case  11: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait)) return false;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWork)) return false ;
                  MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miWRK_YBtm, piWRK_YBWork)) return false ;
                  if(OM.MstOptn.bUsedXAxis) MoveMotr(miWRK_XBtm , piWRK_XBWait);
                  Step.iCycle++;
                  return false;


        case  17: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait)) return false;
                  Step.iCycle++;
                  return false;

        //주의 밑에서 사용.
        case  18: OM.DevOptn.iContactPinCnt++;
                  if(OM.DevOptn.iLimContactPinCnt < OM.DevOptn.iContactPinCnt) {
                      EM_SetErr(eiWRK_LimPinCnt);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  19: if(!m_tmTemp.OnDelay(true ,OM.DevOptn.iPrbSttDelay)) return false ;
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

        case  20: if(m_tmTemp.OnDelay(true , OM.DevOptn.i1ChLedTime * 100 + 1000)) {
                      if(!bPb1Rslt) {
                      //if(!g_bPb1Rslt){
                          //EM_SetErr(eiWRK_FailProbeComm1) ;
                          if( OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_XBtm, piWRK_XBWait) ;
                          if(!OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_YBtm, piWRK_YBWait) ;
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }
                  //if(!g_bPbr1Rslt) return false;
                  if(!bPb1Rslt) return false;
                  if( OM.MstOptn.bByChInsp) PCM.Start(2);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;


        case  21: if(m_tmTemp.OnDelay(true , OM.DevOptn.i2ChLedTime * 100 + 1000)) {
                      if(!bPb2Rslt) {
                      //if(!g_bPb2Rslt) {
                          //EM_SetErr(eiWRK_FailProbeComm2);
                          if( OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_XBtm, piWRK_XBWait) ;
                          if(!OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_YBtm, piWRK_YBWait) ;
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }

                  //if(!g_bPb2Rslt)return false;
                  if(!bPb2Rslt) return false;
                  if( OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_XBtm, piWRK_XBWait) ;
                  Step.iCycle++;
                  return false;

        case  22: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm, piWRK_XBWait)) return false;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;


        case  23: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false;
                  //마스킹 패턴 및 재 검사 패턴.
                  FindChip(r,c);
                  if(OM.CmnOptn.bIgnrInsp) {
                      for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                          for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRK].SetStat(i , c - j, csGood); //데이타 마스킹.
                      }
                  }
                  else {
                      r1 = PCM.CheckDataCnt(riWRK , sPb1Msg);
                      r2 = PCM.CheckDataCnt(riWRK , sPb2Msg);

                      if(!r1){ Step.iCycle = 30 ; m_iDataErrCnt++ ; return false ;} //EM_SetErr(eiWRK_Probe1DataWrong) ; //데이터 마스킹.  나중에 PCM에 ARAY 걷어 내고 여기서 데이터 받아서 마스킹 한다.
                      if(!r2){ Step.iCycle = 30 ; m_iDataErrCnt++ ; return false ;} //EM_SetErr(eiWRK_Probe2DataWrong) ;

                      if(r1 && r2) {
                          PCM.ClearDataCrnt();
                          PCM.GetDataCh(riWRK , sPb1Msg , PROBE_CH1 ,Stat.bRepeat);
                          PCM.GetDataCh(riWRK , sPb2Msg , PROBE_CH2 ,Stat.bRepeat);
                      }
                  }
                  iFail = 0;
                  for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                      iFail += DM.ARAY[riWRK].GetCntColStat(c - j, csFail); //데이타 마스킹.
                  }

                  //재검사 패턴 및 리피트 테스트 패턴.
                  if (iFail >= OM.DevOptn.iLimInsFailCnt || Stat.bRepeat){
                      if(iReInspCnt < OM.DevOptn.iReInspCnt || Stat.bRepeat){
                          for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                              for(int i =0 ; i < iMaxRow ; i++) DM.ARAY[riWRK].SetStat(i ,c - j, csUnkwn) ;
                          }
                          Step.iCycle = 32 ;
                          iReInspCnt++;
                          return false;
                      }
                  }

                  //프로브 개별 핀에 대한 카운트에러
                  //MAX_PROB_COL == 4 have to check.
                  for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                      for(int i =0 ; i < iMaxRow ; i++) {
                          if(DM.ARAY[riWRK].GetStat(i ,c - j)==csFail) {
                              iPinFailCnt[i][OM.DevOptn.iContColCnt-j]++ ;
                              if(OM.DevOptn.iPinCnt && !OM.DevOptn.bSkipPinCnt && iPinFailCnt[i][OM.DevOptn.iContColCnt - j] > OM.DevOptn.iPinCnt) {
                                  EM_SetErr(eiWRK_PinFailCntOver);
                                  sPinCount = "(Front Left is 0,0) Fail Pin is"+AnsiString(i+1)+","+AnsiString(OM.DevOptn.iContColCnt-j) ;
                                  FM_MsgOk("Pin Check",sPinCount);
                                  iPinFailCnt[i][OM.DevOptn.iContColCnt-j] = 0 ;
                              }
                          }
                      }
                  }

                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  Step.iCycle++;
                  return false;

        case  24: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
                  Step.iCycle++;
                  return false;

        case  25: if (!DM.ARAY[riWRK].GetCntStat(csUnkwn) && OM.CmnOptn.bCheckFailCnt && DM.ARAY[riWRK].GetCntStat(csFail) >= OM.CmnOptn.iCheckFailCnt ) EM_SetErr(eiWRK_FailCntOver);
                  //DM.ARAY[riWRK].SetStep(DM.ARAY[riWRK].GetStep() + OM.DevOptn.iContColCnt);
                  Step.iCycle=0;
                  return true;


        //PCM.CheckDataCnt 데이터 체크 Fail시 사용 스텝.
        case  30: if( OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm, piWRK_XBWait))return false;
                  Step.iCycle++;
                  return false;

        case  31: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        //밑에서 사용. 재검사 옵션 및 검사 실패시 사용.
        case  32: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  Step.iCycle++;
                  return false;

        case  33: MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  Step.iCycle++;
                  return false;

        case  34: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  35: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWork)) return false ;
                  MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                  Step.iCycle++;
                  return false ;

        case  36: if(!MoveMotr(miWRK_YBtm, piWRK_YBWork)) return false ;
                  if(OM.MstOptn.bUsedXAxis) MoveMotr(miWRK_XBtm , piWRK_XBWork);
                  Step.iCycle++;
                  return false;

        case  37: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWork))return false;
                  Step.iCycle = 18;
                  return false;

        //검사 실패시 사용 되는 스텝.
        case  50: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWork))return false;
                  Step.iCycle++;
                  return false;
                  
        case  51: if(!MoveMotr(miWRK_YBtm, piWRK_YBWait)) return false ;
                  iCmmErrCnt++;
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  if(iCmmErrCnt < OM.DevOptn.iReInspCnt) { //검사결과 가끔 잘 안날라 와서 리트라이 넣음.

                      Step.iCycle = 32 ;
                      if(!bPb1Rslt             ) Trace("Cmmunication","g_bPb1Rslt ComRetry");
                      if( bPb1Rslt && !bPb2Rslt) Trace("Cmmunication","g_bPb2Rslt ComRetry");
                      return false ;
                  }

                  if(iCmmErrCnt < OM.DevOptn.iReInspCnt+1) { //정 안돼면 세팅 다시해봄.

                      Step.iCycle = 32 ;
                      if(!bPb1Rslt             ) Trace("Cmmunication","g_bPb1Rslt ComRetry && SetDaegyeom()");
                      if( bPb1Rslt && !bPb2Rslt) Trace("Cmmunication","g_bPb2Rslt ComRetry && SetDaegyeom()");

                      PCM.SetDaegyeom();
                      //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //대겸 Eng 프로브 사용시.
                      return false ;
                  }

                  //그래도 안되면 에러.
                  if(!bPb1Rslt             ) EM_SetErr(eiWRK_FailProbeComm1);
                  if( bPb1Rslt && !bPb2Rslt) EM_SetErr(eiWRK_FailProbeComm2);
                  Step.iCycle = 0 ;
                  return false ;
        }
    }
}

void CWorkZone::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CWorkZone::CheckMoved()
{
    return true ; //이파트는 필요 없다.
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
        UserINI.Load(sPath, "Member" , "m_iLastStep"   , m_iLastStep   );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iReGripCnt"  , m_iReGripCnt  );
        UserINI.Save(sPath, "Member" , "m_sLastStrip"  , m_sLastStrip  );
        UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
        UserINI.Save(sPath, "Member" , "m_iLastStep"   , m_iLastStep   );
    }
}
//---------------------------------------------------------------------------

void CWorkZone::HeatProbe1Ch(DWORD _cbInQue)
{
    //Sleep(1);
    int iLen;
    const int iBuffSize = 1024 ;
    BYTE RcvBuff[iBuffSize];
    String sRcvMsg     = "" ;
    String sRcvMsgDrop = "" ;

    static BYTE sRcvMsgByte[iBuffSize];
    static int  iRcvMsgIndx = 0 ;

    if(iRcvMsgIndx ==0){
        memset(&sRcvMsgByte, 0 , iBuffSize);
    }

    memset(&RcvBuff, 0 , iBuffSize);
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



    if(!bFinished/*!sRcvMsg.Pos("[")*/) return ; //------------------------------ 메세지 수신 완료.
    sRcvMsgDrop = (char*)sRcvMsgByte ;

    char cTemp;
    int  iBuffCnt   = sRcvMsgDrop.Pos("RDROP") + 4;
    int  iBuffTemp  = sRcvMsgDrop.Pos("RDROP") - 2; //RDROP <<의 모든 부분 삭제 위치.
    int  iHexCnt    = iBuffCnt + 5;                 //BuffCnt + 5 까지 헥사값이 입력 되어 있다.
    for(int i = 0 ; i < iBuffTemp; i++){            //Drop값 전까지만 받기...
        cTemp = sRcvMsgByte[i] ;
        sRcvMsg += cTemp;
    }
    for(int j = iHexCnt ; j < iBuffSize ; j++){  //Drop값이 헥사 값이라서 제외한 부분 입력.
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


    //임시 화면 디스플레이용... 1칩만 고려 해서 만듬.
    //확인 하고 나중에 주석 하던가 고쳐서 3칩 까지 쓸수 있게 하던가...
    WRK.sPb1Vf[0] = "" ;
    WRK.sPb1Vf[1] = "" ;

    AnsiString sCh ;
    AnsiString sChipData ;
    AnsiString sRcvDataTemp = sRcvMsg ;
    double     dChipData ;
    int iCnt = -1 ;
    for(int j=0; j>-OM.DevOptn.iContColCnt / 2; j-- ) {
       for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --) {
           for(int k=0; k< OM.DevOptn.iDgChipCnt; k++ ) {//대겸 칩 카운트가 아니라 그냥 칩카운트로 바꾸고 GL 에서도 사용해야됨.
               iCnt++ ;
               sCh.sprintf("RL%02d",iCnt);
               sChipData = sRcvDataTemp.SubString(sRcvDataTemp.Pos(sCh)+5,4); //숫자 데이터만 짤라냄.
               sRcvDataTemp.Delete(sRcvDataTemp.Pos(sCh),10);
               sChipData = sChipData.SubString(0,2) + "." + sChipData.SubString(3,4) + " ";
               WRK.sPb1Vf[-j] += sChipData ;
           }
       }
    }




    if(FrmProbe)FrmProbe -> edPb1ChData  -> Text = sRcvMsg ;
    if(FrmProbe)FrmProbe -> edPb1ChData2 -> Text = sRcvMsg ;

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

if(!bFinished/*!sRcvMsg.Pos("[")*/) return ; //------------------------------ 메세지 수신 완료.
    sRcvMsgDrop = (char*)sRcvMsgByte ;

    char cTemp;
    int  iBuffCnt   = sRcvMsgDrop.Pos("RDROP") + 4;
    int  iBuffTemp  = sRcvMsgDrop.Pos("RDROP") - 2; //RDROP <<의 모든 부분 삭제 위치.
    int  iHexCnt    = iBuffCnt + 5;                 //BuffCnt + 5 까지 헥사값이 입력 되어 있다.
    for(int i = 0 ; i < iBuffTemp; i++){            //Drop값 전까지만 받기...
        cTemp = sRcvMsgByte[i] ;
        sRcvMsg += cTemp;
    }
    for(int j = iHexCnt ; j < iBuffSize ; j++){  //Drop값이 헥사 값이라서 제외한 부분 입력.
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


    //임시 화면 디스플레이용... 1칩만 고려 해서 만듬.
    //확인 하고 나중에 주석 하던가 고쳐서 3칩 까지 쓸수 있게 하던가...
    WRK.sPb2Vf[0] = "" ;
    WRK.sPb2Vf[1] = "" ;

    AnsiString sCh ;
    AnsiString sChipData ;
    AnsiString sRcvDataTemp = sRcvMsg ;
    double     dChipData ;
    int iCnt = -1 ;
    for(int j=0; j>-OM.DevOptn.iContColCnt / 2; j-- ) {
       for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --) {
           for(int k=0; k< OM.DevOptn.iDgChipCnt; k++ ) {//대겸 칩 카운트가 아니라 그냥 칩카운트로 바꾸고 GL 에서도 사용해야됨.
               iCnt++ ;
               sCh.sprintf("RL%02d",iCnt);
               sChipData = sRcvDataTemp.SubString(sRcvDataTemp.Pos(sCh)+5,4); //숫자 데이터만 짤라냄.
               sRcvDataTemp.Delete(sRcvDataTemp.Pos(sCh),10);
               sChipData = sChipData.SubString(0,2) + "." + sChipData.SubString(3,4) + " ";
               WRK.sPb2Vf[-j] += sChipData ;
           }
       }
    }




    if(FrmProbe)FrmProbe -> edPb2ChData  -> Text = sRcvMsg ;
    if(FrmProbe)FrmProbe -> edPb2ChData2 -> Text = sRcvMsg ;

    if(OM.MstOptn.bDebugMode)Trace("RS232C_Pb2ch_Msg",sRcvMsg.c_str());

    WRK.sPb2Msg = sRcvMsg ;
    sRcvMsg = "" ;
    WRK.bPb2Rslt = true ;
    iRcvMsgIndx = 0 ;
}
