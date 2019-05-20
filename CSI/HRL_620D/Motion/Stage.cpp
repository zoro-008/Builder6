//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <math.hpp>
#pragma hdrstop

#include "Stage.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
//#include "ManualMan.h"
#include "LaserConUnit.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.

#define CST_OFFSET 5
//---------------------------------------------------------------------------
CStage STG;

CStage::CStage()
{

}
CStage::~CStage (void)
{
    //메인폼으로 이동.
    //Close();
}
void CStage::Init()
{
    InitCycleName();

    m_sPartName   = "Stage " ;
    m_dLastIdxPos = 0.0 ;


    //에러 정하고 나중에...JH
    m_iHomeTO  = (int)eiETC_AllHomeTO ;
    m_iStartTO = (int)eiETC_ToStartTO ;
    m_iStopTO  = (int)eiETC_ToStopTO  ;
    m_iCycleTO = (int)eiSTG_CycleTO   ;


    m_bDispStartForm = false ;
    m_bHomeEnd       = false ;

    Reset();
    Load(true);

}

void CStage::Close()
{
    Load(false);
}
void CStage::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CStage::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}
//---------------------------------------------------------------------------

double CStage::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{

    double dPos  = MT_GetCmdPos(_iMotr);
    int r = -1 , c = -1 ;

    double dStgStartPosX ;
    double dStgStartPosY ;

    if(_iMotr == miSTG_TBase){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piSTG_TBaseOrign : dPos = PM.GetValue (_iMotr , pvSTG_TBaseOrign ); break ;
            case piSTG_TBaseRvers : dPos = PM.GetValue (_iMotr , pvSTG_TBaseRvers ); break ;
        }
    }

    else if(_iMotr == miWRK_UBrd1){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piWRK_UBrd1_Wait : dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wait ); break ;
            case piWRK_UBrd1_Wrk1 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wrk1 ); break ;
            case piWRK_UBrd1_Wrk2 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wrk2 ); break ;
            case piWRK_UBrd1_Wrk3 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wrk3 ); break ;
            case piWRK_UBrd1_Wrk4 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wrk4 ); break ;
            case piWRK_UBrd1_Wrk5 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wrk5 ); break ;
            case piWRK_UBrd1_Wrk6 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wrk6 ); break ;
            case piWRK_UBrd1_Wrk7 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wrk7 ); break ;
            case piWRK_UBrd1_Wrk8 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wrk8 ); break ;
            case piWRK_UBrd1_Wrk9 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wrk9 ); break ;
            case piWRK_UBrd1_Wrk10: dPos = PM.GetValue (_iMotr , pvWRK_UBrd1_Wrk10); break ;
        }
    }

    else if(_iMotr == miWRK_VBrd1){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piWRK_VBrd1_Wait : dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wait ); break ;
            case piWRK_VBrd1_Wrk1 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wrk1 ); break ;
            case piWRK_VBrd1_Wrk2 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wrk2 ); break ;
            case piWRK_VBrd1_Wrk3 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wrk3 ); break ;
            case piWRK_VBrd1_Wrk4 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wrk4 ); break ;
            case piWRK_VBrd1_Wrk5 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wrk5 ); break ;
            case piWRK_VBrd1_Wrk6 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wrk6 ); break ;
            case piWRK_VBrd1_Wrk7 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wrk7 ); break ;
            case piWRK_VBrd1_Wrk8 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wrk8 ); break ;
            case piWRK_VBrd1_Wrk9 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wrk9 ); break ;
            case piWRK_VBrd1_Wrk10: dPos = PM.GetValue (_iMotr , pvWRK_VBrd1_Wrk10); break ;
        }
    }

    else if(_iMotr == miWRK_UBrd2){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piWRK_UBrd2_Wait : dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wait ); break ;
            case piWRK_UBrd2_Wrk1 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wrk1 ); break ;
            case piWRK_UBrd2_Wrk2 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wrk2 ); break ;
            case piWRK_UBrd2_Wrk3 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wrk3 ); break ;
            case piWRK_UBrd2_Wrk4 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wrk4 ); break ;
            case piWRK_UBrd2_Wrk5 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wrk5 ); break ;
            case piWRK_UBrd2_Wrk6 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wrk6 ); break ;
            case piWRK_UBrd2_Wrk7 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wrk7 ); break ;
            case piWRK_UBrd2_Wrk8 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wrk8 ); break ;
            case piWRK_UBrd2_Wrk9 : dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wrk9 ); break ;
            case piWRK_UBrd2_Wrk10: dPos = PM.GetValue (_iMotr , pvWRK_UBrd2_Wrk10); break ;
        }
    }

    else if(_iMotr == miWRK_VBrd2){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                    ); break ;
            case piWRK_VBrd2_Wait : dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wait ); break ;
            case piWRK_VBrd2_Wrk1 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wrk1 ); break ;
            case piWRK_VBrd2_Wrk2 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wrk2 ); break ;
            case piWRK_VBrd2_Wrk3 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wrk3 ); break ;
            case piWRK_VBrd2_Wrk4 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wrk4 ); break ;
            case piWRK_VBrd2_Wrk5 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wrk5 ); break ;
            case piWRK_VBrd2_Wrk6 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wrk6 ); break ;
            case piWRK_VBrd2_Wrk7 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wrk7 ); break ;
            case piWRK_VBrd2_Wrk8 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wrk8 ); break ;
            case piWRK_VBrd2_Wrk9 : dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wrk9 ); break ;
            case piWRK_VBrd2_Wrk10: dPos = PM.GetValue (_iMotr , pvWRK_VBrd2_Wrk10); break ;
        }
    }

    return dPos ;
}

//---------------------------------------------------------------------------

bool CStage::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{

    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isNotVc1stL = !IO_GetX(xWRK_Brd1VccL);
    bool isNotVc1stR = !IO_GetX(xWRK_Brd1VccR);
    bool isNotVc2ndL = !IO_GetX(xWRK_Brd2VccL);
    bool isNotVc2ndR = !IO_GetX(xWRK_Brd2VccR);

    bool isNotWrkPst = !IO_GetX(xSTG_WrkPstDetect)  ;

    //bool isNotWaitU1 =  MT_GetCmdPos(miWRK_UBrd1) != PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait) && MT_GetStop(miWRK_UBrd1);
    //bool isNotWaitV1 = (MT_GetCmdPos(miWRK_VBrd1) != PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait) && MT_GetStop(miWRK_VBrd1)) && !OM.CmnOptn.bNotUsedVAxis;
    //bool isNotWaitU2 =  MT_GetCmdPos(miWRK_UBrd2) != PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait) && MT_GetStop(miWRK_UBrd2);
    //bool isNotWaitV2 = (MT_GetCmdPos(miWRK_VBrd2) != PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait) && MT_GetStop(miWRK_VBrd2)) && !OM.CmnOptn.bNotUsedVAxis;

    bool isNotWaitU1 = MT_GetCmdPos(miWRK_UBrd1) >= PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait)+0.1 ||
                       MT_GetCmdPos(miWRK_UBrd1) <= PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait)-0.1 ;
    bool isNotWaitV1 = MT_GetCmdPos(miWRK_VBrd1) >= PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait)+0.1 ||
                       MT_GetCmdPos(miWRK_VBrd1) <= PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait)-0.1 ;
    bool isNotWaitU2 = MT_GetCmdPos(miWRK_UBrd2) >= PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait)+0.1 ||
                       MT_GetCmdPos(miWRK_UBrd2) <= PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait)-0.1 ;
    bool isNotWaitV2 = MT_GetCmdPos(miWRK_VBrd2) >= PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait)+0.1 ||
                       MT_GetCmdPos(miWRK_VBrd2) <= PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait)-0.1 ;

    bool isNotWait   = false;

    if(OM.CmnOptn.bNotUsedVAxis) isNotWait = isNotWaitU1 || isNotWaitU1;
    else                         isNotWait = isNotWaitU1 || isNotWaitU2 || isNotWaitV1 || isNotWaitV2;


    if(_iMotr == miSTG_TBase){
        if(isNotWait  ) {sMsg = "All Work Motor Is Not Wait Position.";  bRet = false ;}
        //if(isNotWrkPst) {sMsg = "Wrok Zone Sensor Is Nor Detect."     ;  bRet = false ;}
        switch(_iPstnId){
            default                : sMsg = "Is Not Motr Position" ; bRet = false ; break ;
            case piSTG_TBaseOrign  : break ;
            case piSTG_TBaseRvers  : break ;
            bRet = bRet;
        }
    }

    else if(_iMotr == miWRK_UBrd1) {
        if(isNotWrkPst               ) { sMsg = "Is Not Move Position."      ; bRet = false ;}
        //if(isNotWaitV1               ) { sMsg = "1st V Is Not Wait Position."; bRet = false ;}
        //if(isNotVc1stL || isNotVc1stR) { sMsg = "The Vaccum Is Off."         ; bRet = false ;}
        switch(_iPstnId){
            default               : sMsg = "Is Not Motr Positon"; bRet = false; break ;
            case piWRK_UBrd1_Wait : break ;
            case piWRK_UBrd1_Wrk1 : break ;
            case piWRK_UBrd1_Wrk2 : break ;
            case piWRK_UBrd1_Wrk3 : break ;
            case piWRK_UBrd1_Wrk4 : break ;
            case piWRK_UBrd1_Wrk5 : break ;
            case piWRK_UBrd1_Wrk6 : break ;
            case piWRK_UBrd1_Wrk7 : break ;
            case piWRK_UBrd1_Wrk8 : break ;
            case piWRK_UBrd1_Wrk9 : break ;
            case piWRK_UBrd1_Wrk10: break ;

        }
    }
    else if(_iMotr == miWRK_VBrd1) {
        if(isNotWrkPst               ) { sMsg = "Is Not Move Position."      ; bRet = false ;}
        //if(isNotWaitU1               ) { sMsg = "1st V Is Not Wait Position."; bRet = false ;}
        //if(isNotVc1stL || isNotVc1stR) { sMsg = "The Vaccum Is Off."         ; bRet = false ;}
        switch(_iPstnId){
            default               : sMsg = "Is Not Motr Position"; bRet = false; break ;
            case piWRK_VBrd1_Wait : break ;
            case piWRK_VBrd1_Wrk1 : break ;
            case piWRK_VBrd1_Wrk2 : break ;
            case piWRK_VBrd1_Wrk3 : break ;
            case piWRK_VBrd1_Wrk4 : break ;
            case piWRK_VBrd1_Wrk5 : break ;
            case piWRK_VBrd1_Wrk6 : break ;
            case piWRK_VBrd1_Wrk7 : break ;
            case piWRK_VBrd1_Wrk8 : break ;
            case piWRK_VBrd1_Wrk9 : break ;
            case piWRK_VBrd1_Wrk10: break ;
        }
    }
    else if(_iMotr == miWRK_UBrd2) {
        if(isNotWrkPst               ) { sMsg = "Is Not Move Position."      ; bRet = false ;}
        //if(isNotWaitV2               ) { sMsg = "1st V Is Not Wait Position."; bRet = false ;}
        //if(isNotVc2ndL || isNotVc2ndR) { sMsg = "The Vaccum Is Off."         ; bRet = false ;}
        switch(_iPstnId){
            default               : sMsg = "Is Not Motr Position"; bRet = false; break ;
            case piWRK_UBrd2_Wait : break ;
            case piWRK_UBrd2_Wrk1 : break ;
            case piWRK_UBrd2_Wrk2 : break ;
            case piWRK_UBrd2_Wrk3 : break ;
            case piWRK_UBrd2_Wrk4 : break ;
            case piWRK_UBrd2_Wrk5 : break ;
            case piWRK_UBrd2_Wrk6 : break ;
            case piWRK_UBrd2_Wrk7 : break ;
            case piWRK_UBrd2_Wrk8 : break ;
            case piWRK_UBrd2_Wrk9 : break ;
            case piWRK_UBrd2_Wrk10: break ;
        }
    }
    else if(_iMotr == miWRK_VBrd2) {
        if(isNotWrkPst               ) { sMsg = "Is Not Move Position."      ; bRet = false ;}
        //if(isNotWaitU2               ) { sMsg = "1st V Is Not Wait Position."; bRet = false ;}
        //if(isNotVc2ndL || isNotVc2ndR) { sMsg = "The Vaccum Is Off."         ; bRet = false ;}
        switch(_iPstnId){
            default                 : sMsg = "Is Not Motr Position"; bRet = false; break ;
            case piWRK_VBrd2_Wait : break ;
            case piWRK_VBrd2_Wrk1 : break ;
            case piWRK_VBrd2_Wrk2 : break ;
            case piWRK_VBrd2_Wrk3 : break ;
            case piWRK_VBrd2_Wrk4 : break ;
            case piWRK_VBrd2_Wrk5 : break ;
            case piWRK_VBrd2_Wrk6 : break ;
            case piWRK_VBrd2_Wrk7 : break ;
            case piWRK_VBrd2_Wrk8 : break ;
            case piWRK_VBrd2_Wrk9 : break ;
            case piWRK_VBrd2_Wrk10: break ;
        }
    }
    else {
        sMsg = AnsiString(MT_GetName(_iMotr)) + " is Not this part Motr!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}
//---------------------------------------------------------------------------

bool CStage::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    AnsiString sTemp = dPosition ;
    if(_iMotr == miSTG_TBase) Trace("11",sTemp.c_str());
    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}
//-----------------------------------------------------------------------------

bool CStage::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmHome.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() /*&&!OM.MstOptn.bDebugMode*/, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iHomeTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iHome = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    if(IO_GetX(xETC_AreaSensor)){
        if(!MT_GetStop(miSTG_TBase)) {
            MT_SetServo(miSTG_TBase, false) ;
            EM_SetErr(eiETC_AreaSensor);
            m_bHomeEnd = false;
            Step.iHome = 0;
            return true;
        }
    }

    //씨부랄. dd
    if(IO_GetX(xSTG_TltDetect)){
        MT_SetServo(miSTG_TBase, false) ;
        EM_SetErr(eiSTG_TiltDetect);
        m_bHomeEnd = false;
        Step.iHome = 0;
        return true;
    }


    /*
    static bool bLaserChanged = false;
    if(Step.iHome == 10){
        LCU.CycleGA(OM.DevOptn.iLsrDevU1,true);
        bLaserChanged = false;
    }
    else{
        bLaserChanged = LCU.CycleGA(OM.DevOptn.iLsrDevU1);
    }*/

    if(Stat.bReqStop) {
        //Step.iHome = 0;
        //return true ;
    }



    switch (Step.iHome) {

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: if(!IO_GetX(xSTG_WrkPstDetect)){
                     EM_SetErr(eiSTG_WrkPstDetect);
                     Step.iHome = 0;
                     return true;
                 }

                 LCU.SetGABlockId(OM.DevOptn.iOriLsrDevU1);
                 LCU.SetSendMsgId(lmGA);

                 //m_tmTemp.Clear(); //?JS
                 Step.iHome++;
                 return false ;

        case 11: //if(!m_tmTemp.OnDelay(true , 100)) return false ; JS?
                 //MT_SetHomeEnd(miSTG_TBase,false);
                 //MT_SetHomeEnd(miWRK_UBrd1,false);
                 //MT_SetHomeEnd(miWRK_UBrd2,false);
                 if(!OM.CmnOptn.bNotUsedVAxis){
                     MT_SetHomeEnd(miWRK_VBrd1 , false);
                     MT_SetHomeEnd(miWRK_VBrd2 , false);
                 }
                 Step.iHome++;
                 return false ;

        case 12: if(!OM.CmnOptn.bNotUsedVAxis){
                     MT_DoHome(miWRK_VBrd1);
                     MT_DoHome(miWRK_VBrd2);
                 }

                 Step.iHome++;
                 return false ;


        case 13: if(!OM.CmnOptn.bNotUsedVAxis && (!MT_GetHomeEnd(miWRK_VBrd1) || !MT_GetHomeEnd(miWRK_VBrd2))) return false ;
                 if(!OM.CmnOptn.bNotUsedVAxis){
                     MT_GoAbsMan(miWRK_VBrd1 , PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait)) ;
                     MT_GoAbsMan(miWRK_VBrd2 , PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait)) ;
                 }
                 MT_SetHomeEnd(miWRK_UBrd1 , false);
                 MT_SetHomeEnd(miWRK_UBrd2 , false);

                 Step.iHome++;
                 return false ;

        case 14: if(!OM.CmnOptn.bNotUsedVAxis){
                     if(!MT_GoAbsMan(miWRK_VBrd1 , PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait))) return false ;
                     if(!MT_GoAbsMan(miWRK_VBrd2 , PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait))) return false ;
                 }
                 MT_DoHome(miWRK_UBrd1);
                 MT_DoHome(miWRK_UBrd2);

                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GetHomeEnd(miWRK_UBrd1) || !MT_GetHomeEnd(miWRK_UBrd2)) return false ;
                 MT_GoAbsMan(miWRK_UBrd1 , PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait)) ;
                 MT_GoAbsMan(miWRK_UBrd2 , PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait)) ;
                 MT_SetHomeEnd(miSTG_TBase , false);

                 Step.iHome++;
                 return false ;

        case 16: if(!MT_GoAbsMan(miWRK_UBrd1 , PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait))) return false ;
                 if(!MT_GoAbsMan(miWRK_UBrd2 , PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait))) return false ;
                 MT_DoHome(miSTG_TBase);

                 Step.iHome++;
                 return false ;

        case 17: if(!MT_GetHomeEnd(miSTG_TBase)) return false ;
                 MT_GoAbsMan(miSTG_TBase , PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign));

                 Step.iHome++;
                 return false ;

        case 18: if(!MT_GoAbsMan(miSTG_TBase , PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign))) return false ;

                 Step.iHome++;
                 return false;

        case 19: if(!OM.CmnOptn.bLaserSkip && !LCU.GetGAFinished()) return false ;
                 m_bHomeEnd = true;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CStage::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CStage::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CStage::ToStart(void) //스타트를 하기 위한 함수.  .... 여기 디퐅트 시작은 그냥 메뉴얼 사이클로
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: 
                 Step.iToStart = 0 ;
                 return true ;

    }
}

bool CStage::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 15000)) EM_SetErr((EN_ERR_ID)m_iStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10:
                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CStage::Autorun(void) //오토런닝시에 계속 타는 함수.
{

}

bool CStage::CycleWork()
{
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.CmnOptn.bDryRun, OM.MstOptn.iCycleTm )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    //에리어 센서 감지 에러... 10초 넘으면 에러다.
    if (m_tmCycle.OnDelay(Step.iCycle == 14 && MT_GetStop(miSTG_TBase) , 10000 )) {
        EM_SetErr(eiETC_AreaSensor);
        MT_SetServo(miSTG_TBase , false);
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    static bool         bOrigPstn       = true             ;
    static bool         bRvrsPstn       = true             ;
    static EN_MOTR_ID   miWRK_UBrd      = miWRK_UBrd1      ;
    static EN_MOTR_ID   miWRK_VBrd      = miWRK_VBrd1      ;
    static EN_OUTPUT_ID yWRK_BrdVccL    = yWRK_Brd1VccL    ;
    static EN_OUTPUT_ID yWRK_BrdVccR    = yWRK_Brd1VccR    ;
    static EN_INPUT_ID  xWRK_BrdVccL    = xWRK_Brd1VccL    ;
    static EN_INPUT_ID  xWRK_BrdVccR    = xWRK_Brd1VccR    ;
    static EN_ERR_ID    eiWRK_BrdVccL   = eiWRK_Brd1VccL   ;
    static EN_ERR_ID    eiWRK_BrdVccR   = eiWRK_Brd1VccR   ;
    static EN_PSTN_ID   piSTG_TBase     = piSTG_TBaseRvers ;
    static EN_PSTN_ID   piWRK_VBrd_Wrk1 = piWRK_VBrd1_Wrk1 ;
    static EN_PSTN_ID   piWRK_UBrd_Wrk1 = piWRK_UBrd1_Wrk1 ;

    int iChekCount      = 0 ;

    if(OM.DevOptn.bUsePstU1 ) iChekCount++;
    if(OM.DevOptn.bUsePstU2 ) iChekCount++;
    if(OM.DevOptn.bUsePstU3 ) iChekCount++;
    if(OM.DevOptn.bUsePstU4 ) iChekCount++;
    if(OM.DevOptn.bUsePstU5 ) iChekCount++;
    if(OM.DevOptn.bUsePstU6 ) iChekCount++;
    if(OM.DevOptn.bUsePstU7 ) iChekCount++;
    if(OM.DevOptn.bUsePstU8 ) iChekCount++;
    if(OM.DevOptn.bUsePstU9 ) iChekCount++;
    if(OM.DevOptn.bUsePstU10) iChekCount++;

    const  int  iMaxLaserWrkCnt = iChekCount;           //총 작업하는 카운트 체크 된 체크 박스 수
    static int  iLaserWorkCnt   = 0 ;                   //레이저 작업 한 횟수
    static int  iRotateCnt      = 0 ;                   //레이저 스텝 반복 횟수 , 포지션 아이디 옵셑
    static int  iLaserWorkDev   = OM.DevOptn.iOriLsrDevU1 ;//OM.DevOptn.iLsrDevU에 값들, 레이저 디바이스 값
    static bool bPstnUsedRslt   = true ;                //OM.DevOptn.bUsePstU에 값 ,이걸로 반복 스텝 탈지 안탈지 정함

    static DWord dStartTime ;

    PreStep.iCycle = Step.iCycle ;
    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //현재 작업할 놈들 세팅.
                  IO_SetY(yETC_LaserWrkVcc , true); // 집진기 매뉴얼 버튼 추가로 인해 시작 시에 무전거 집진기 온 ...JH

                  dStartTime = GetTickCount();
                  aTemp11 = 0;
                  bOrigPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) - 1.0) <= MT_GetCmdPos(miSTG_TBase) &&
                                 (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) + 1.0) ;



                  bRvrsPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )-1.0  <= MT_GetCmdPos(miSTG_TBase)) &&
                                 (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )+1.0 ) ;

                  if(bOrigPstn) { //초기화 상태 1번에 있는놈들 작업할 차례
                      miWRK_UBrd      = miWRK_UBrd1     ;
                      miWRK_VBrd      = miWRK_VBrd1     ;
                      yWRK_BrdVccL    = yWRK_Brd2VccL   ;
                      yWRK_BrdVccR    = yWRK_Brd2VccR   ;
                      xWRK_BrdVccL    = xWRK_Brd1VccL   ;
                      xWRK_BrdVccR    = xWRK_Brd1VccR   ;
                      piSTG_TBase     = piSTG_TBaseRvers;
                      eiWRK_BrdVccL   = eiWRK_Brd1VccL  ;
                      eiWRK_BrdVccR   = eiWRK_Brd1VccR  ;
                      piWRK_VBrd_Wrk1 = piWRK_VBrd1_Wrk1;
                      piWRK_UBrd_Wrk1 = piWRK_UBrd1_Wrk1;

                      m_bNotWrkPstn = false;
                  }

                  else if(bRvrsPstn){
                      miWRK_UBrd      = miWRK_UBrd2     ;
                      miWRK_VBrd      = miWRK_VBrd2     ;
                      yWRK_BrdVccL    = yWRK_Brd1VccL   ;
                      yWRK_BrdVccR    = yWRK_Brd1VccR   ;
                      xWRK_BrdVccL    = xWRK_Brd2VccL   ;
                      xWRK_BrdVccR    = xWRK_Brd2VccR   ;
                      piSTG_TBase     = piSTG_TBaseOrign;
                      eiWRK_BrdVccL   = eiWRK_Brd2VccL  ;
                      eiWRK_BrdVccR   = eiWRK_Brd2VccR  ;
                      piWRK_VBrd_Wrk1 = piWRK_VBrd2_Wrk1;
                      piWRK_UBrd_Wrk1 = piWRK_UBrd2_Wrk1;

                      m_bNotWrkPstn = false;
                  }
                  else{
                      m_bNotWrkPstn = true;
                      //FM_MsgOk("Warning" , "베이스 판을 대기포지션이나 리버스포지션으로 옮겨주세요." );
                      Step.iCycle = 0;
                      return true;
                  }

                  IO_SetY(yWRK_Brd1VccL , true);
                  IO_SetY(yWRK_Brd1VccR , true);
                  IO_SetY(yWRK_Brd2VccL , true);
                  IO_SetY(yWRK_Brd2VccR , true);

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(!OM.CmnOptn.bVaccumSkip){
                      if(m_tmTemp.OnDelay(true , 1000)){
                          if(!OM.CmnOptn.bUseOnlyRgh && !IO_GetX(xWRK_BrdVccL)) EM_SetErr(eiWRK_BrdVccL);
                          if(!IO_GetX(xWRK_BrdVccR)) EM_SetErr(eiWRK_BrdVccR); // 옵션 ONLY USE LEFT 가 false 일때만 탄다...
                          if(!OM.CmnOptn.bUseOnlyRgh){
                              Step.iCycle = 0;
                              return true;
                          }
                      }
                      if(!OM.CmnOptn.bUseOnlyRgh){
                          if(!IO_GetX(xWRK_BrdVccL) || !IO_GetX(xWRK_BrdVccR)) return false ;
                      }
                      else{
                          if(bOrigPstn){
                              if( !IO_GetX(xWRK_BrdVccR)) return false ;
                          }
                      }
                  }
                  if(!OM.CmnOptn.bNotUsedVAxis){
                      MoveMotr(miWRK_VBrd1 , piWRK_VBrd1_Wait);
                      MoveMotr(miWRK_VBrd2 , piWRK_VBrd2_Wait);
                  }

                  Step.iCycle++;
                  return false;

        case  12: if(!OM.CmnOptn.bNotUsedVAxis){
                      if(!MoveMotr(miWRK_VBrd1 , piWRK_VBrd1_Wait)) return false;
                      if(!MoveMotr(miWRK_VBrd2 , piWRK_VBrd2_Wait)) return false;
                  }
                  MoveMotr(miWRK_UBrd1 , piWRK_UBrd1_Wait);
                  MoveMotr(miWRK_UBrd2 , piWRK_UBrd2_Wait);

                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_UBrd1 , piWRK_UBrd1_Wait)) return false;
                  if(!MoveMotr(miWRK_UBrd2 , piWRK_UBrd2_Wait)) return false;
                  Step.iCycle++;
                  return false;

        //위에 에러에서 사용함.
        case  14: if(!MT_GetHomeSnsr(miWRK_UBrd1) || !MT_GetHomeSnsr(miWRK_UBrd1)){
                      EM_SetErr(eiWRK_NotBrdPstn);
                      Step.iCycle = 0;
                      return true;
                  }
                  if(IO_GetX(xETC_AreaSensor)){
                      if(!MT_GetStop(miSTG_TBase)) MT_Stop(miSTG_TBase);
                      return false;
                  }
                  if(IO_GetX(xSTG_TltDetect)){    // 팅팅 센서 확인 JH
                      MT_Stop(miSTG_TBase);
                      EM_SetErr(eiSTG_TiltDetect);
                      Step.iCycle = 0;
                      return true;
                  }

                  if(!MoveMotr(miSTG_TBase , piSTG_TBase)) return false;
                  Step.iCycle ++;
                  return false;


        //에어리어 센서 검사 끝...  스텝을 위에서 쓴다...
        case  15: if(!MoveMotr(miSTG_TBase , piSTG_TBase)) return false; //

                  if(bRvrsPstn && OM.CmnOptn.bBrd2LsrSkip ){
                      if(!OM.CmnOptn.bDryRun){
                          LCU.SetGABlockId(OM.DevOptn.iOriLsrDevU1);
                          LCU.SetSendMsgId(lmGA);
                      }
                      Step.iCycle = 23;
                      return false;
                    // 2번 레이저 스킵... JH

                      //전작업마지막에 GA블럭체인지 보낸것 잘 바뀌었나 확인.
                      if(!OM.CmnOptn.bLaserSkip && !OM.CmnOptn.bDryRun) {
                          if(!LCU.GetGAFinished()) return false ;
                      }
                  }
                  IO_SetY(yWRK_BrdVccL , false);
                  IO_SetY(yWRK_BrdVccR , false);

                  iRotateCnt    = 0 ;
                  iLaserWorkCnt = 0 ;

                  if(bOrigPstn) {
                      iLaserWorkDev = OM.DevOptn.iOriLsrDevU1 ;
                      if(!OM.CmnOptn.bLaserSkip && LCU.GetBlockNoID() != iLaserWorkDev && !OM.CmnOptn.bDryRun){
                          LCU.SetGABlockId(OM.DevOptn.iOriLsrDevU1);
                          LCU.SetSendMsgId(lmGA);
                          EM_SetErr(eiSTG_LaserBlock);
                      }
                  }
                  else {
                      iLaserWorkDev = OM.DevOptn.iRvsLsrDevU1 ;
                      if(!OM.CmnOptn.bLaserSkip && LCU.GetBlockNoID() != iLaserWorkDev && !OM.CmnOptn.bDryRun){
                          LCU.SetGABlockId(OM.DevOptn.iRvsLsrDevU1);
                          LCU.SetSendMsgId(lmGA);
                          EM_SetErr(eiSTG_LaserBlock);
                      }
                  }
                  bPstnUsedRslt = OM.DevOptn.bUsePstU1 ;

                  Step.iCycle++;
                  return false ;

                  //여기부터 레이저 작업....  스탭 밑에서 사용...
        case  16: if(bPstnUsedRslt) { //OM.DevOptn.bUsePstU 이 true 이면 다음 스텝으로...
                      Step.iCycle++;
                      return false;
                  }
                  else {
                      Step.iCycle = 21;    //false 이면 20번으로
                      return false;
                  }

        case  17: if(!OM.CmnOptn.bNotUsedVAxis) MoveMotr(miWRK_VBrd , (int)piWRK_VBrd_Wrk1 + iRotateCnt );
                  MoveMotr(miWRK_UBrd , (int)piWRK_UBrd_Wrk1 + iRotateCnt );
                  if(iRotateCnt != 0 && !OM.CmnOptn.bLaserSkip && !OM.CmnOptn.bDryRun){
                      LCU.SetGABlockId(iLaserWorkDev ); //레이저 작업
                      LCU.SetSendMsgId(lmGA);

                  }


                  Step.iCycle++;
                  return false;

        case  18: if(!OM.CmnOptn.bNotUsedVAxis && !MoveMotr(miWRK_VBrd , (int)piWRK_VBrd_Wrk1 + iRotateCnt)) return false;
                  if(!MoveMotr(miWRK_UBrd , (int)piWRK_UBrd_Wrk1 + iRotateCnt)) return false;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  19:
                  if(!OM.CmnOptn.bLaserSkip && !OM.CmnOptn.bDryRun){
                      if(iRotateCnt != 0){
                          if(!LCU.GetGAFinished()) return false ;
                      }
                      LCU.SetSendMsgId(lmTX);
                  }
                  iLaserWorkCnt++;
                  Step.iCycle++;
                  return false;

        case  20: if(!OM.CmnOptn.bLaserSkip && !OM.CmnOptn.bDryRun){
                      if(LCU.GetLaserMsgId() != lmNone)return false ;
                  }
                  Step.iCycle++;
                  return false;

                  //스텝 위에서 사용....
        case  21: if(iLaserWorkCnt >= iMaxLaserWrkCnt) { //총 작업해야 할 레이저 카운트와 현재 작업한 레이저 카운트 비교
                      iRotateCnt    = 0 ;
                      iLaserWorkCnt = 0 ;

                      if(bOrigPstn) { iLaserWorkDev = OM.DevOptn.iOriLsrDevU1 ; }
                      if(bRvrsPstn) { iLaserWorkDev = OM.DevOptn.iRvsLsrDevU1 ; }

                      bPstnUsedRslt = OM.DevOptn.bUsePstU1 ;
                      if(!OM.CmnOptn.bLaserSkip && !OM.CmnOptn.bDryRun){//여기 나누자
                          if(!OM.CmnOptn.bBrd2LsrSkip){
                              if(bOrigPstn){
                                  LCU.SetGABlockId(OM.DevOptn.iRvsLsrDevU1);
                                  LCU.SetSendMsgId(lmGA);
                              }
                              if(bRvrsPstn){
                                  LCU.SetGABlockId(OM.DevOptn.iOriLsrDevU1);
                                  LCU.SetSendMsgId(lmGA);
                              }
                          }
                          else{
                              LCU.SetGABlockId(OM.DevOptn.iOriLsrDevU1);
                              LCU.SetSendMsgId(lmGA);
                          }
                      }
                      Step.iCycle++; //작업종료 절차.
                      return false;
                  }
                  //로테이트 카운트에 따라 옵션값 가져 오기....
                  if(bOrigPstn){
                      if(iRotateCnt == 0) { bPstnUsedRslt = OM.DevOptn.bUsePstU2 ; iLaserWorkDev = OM.DevOptn.iOriLsrDevU2 ; }
                      if(iRotateCnt == 1) { bPstnUsedRslt = OM.DevOptn.bUsePstU3 ; iLaserWorkDev = OM.DevOptn.iOriLsrDevU3 ; }
                      if(iRotateCnt == 2) { bPstnUsedRslt = OM.DevOptn.bUsePstU4 ; iLaserWorkDev = OM.DevOptn.iOriLsrDevU4 ; }
                      if(iRotateCnt == 3) { bPstnUsedRslt = OM.DevOptn.bUsePstU5 ; iLaserWorkDev = OM.DevOptn.iOriLsrDevU5 ; }
                      if(iRotateCnt == 4) { bPstnUsedRslt = OM.DevOptn.bUsePstU6 ; iLaserWorkDev = OM.DevOptn.iOriLsrDevU6 ; }
                      if(iRotateCnt == 5) { bPstnUsedRslt = OM.DevOptn.bUsePstU7 ; iLaserWorkDev = OM.DevOptn.iOriLsrDevU7 ; }
                      if(iRotateCnt == 6) { bPstnUsedRslt = OM.DevOptn.bUsePstU8 ; iLaserWorkDev = OM.DevOptn.iOriLsrDevU8 ; }
                      if(iRotateCnt == 7) { bPstnUsedRslt = OM.DevOptn.bUsePstU9 ; iLaserWorkDev = OM.DevOptn.iOriLsrDevU9 ; }
                      if(iRotateCnt == 8) { bPstnUsedRslt = OM.DevOptn.bUsePstU10; iLaserWorkDev = OM.DevOptn.iOriLsrDevU10; }
                      iRotateCnt++;
                  }
                  if(bRvrsPstn){
                      if(iRotateCnt == 0) { bPstnUsedRslt = OM.DevOptn.bUsePstU2 ; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU2 ; }
                      if(iRotateCnt == 1) { bPstnUsedRslt = OM.DevOptn.bUsePstU3 ; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU3 ; }
                      if(iRotateCnt == 2) { bPstnUsedRslt = OM.DevOptn.bUsePstU4 ; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU4 ; }
                      if(iRotateCnt == 3) { bPstnUsedRslt = OM.DevOptn.bUsePstU5 ; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU5 ; }
                      if(iRotateCnt == 4) { bPstnUsedRslt = OM.DevOptn.bUsePstU6 ; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU6 ; }
                      if(iRotateCnt == 5) { bPstnUsedRslt = OM.DevOptn.bUsePstU7 ; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU7 ; }
                      if(iRotateCnt == 6) { bPstnUsedRslt = OM.DevOptn.bUsePstU8 ; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU8 ; }
                      if(iRotateCnt == 7) { bPstnUsedRslt = OM.DevOptn.bUsePstU9 ; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU9 ; }
                      if(iRotateCnt == 8) { bPstnUsedRslt = OM.DevOptn.bUsePstU10; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU10; }
                      iRotateCnt++;
                  }

                  Step.iCycle = 16;
                  return false    ;

                  // 레이저 작업이 다 끝나고 대기 포지션으로 이동하고 베큠 끄기
        case  22: if(!OM.CmnOptn.bNotUsedVAxis)MoveMotr(miWRK_VBrd , piWRK_VBrd1_Wait);
                  MoveMotr(miWRK_UBrd , piWRK_UBrd1_Wait);
                  Step.iCycle++;
                  return false;

        case  23: if(!OM.CmnOptn.bNotUsedVAxis && !MoveMotr(miWRK_VBrd , piWRK_VBrd1_Wait))return false;
                  if(!MoveMotr(miWRK_UBrd , piWRK_UBrd1_Wait))return false;

                  IO_SetY(yWRK_Brd1VccL , false);
                  IO_SetY(yWRK_Brd1VccR , false);
                  IO_SetY(yWRK_Brd2VccL , false);
                  IO_SetY(yWRK_Brd2VccR , false);

                  LotInfoAdd();
                  m_dLaserWorkTime = GetTickCount() - dStartTime ;
                  Step.iCycle = 0;
                  return true;


    }
}

/*
bool CStage::CycleWork()
{
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() , 16000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    //에리어 센서 감지 에러... 10초 넘으면 에러다.
    if (m_tmCycle.OnDelay(Step.iCycle == 14 && MT_GetStop(miSTG_TBase) , 10000 )) {
        EM_SetErr(eiETC_AreaSensor);
        MT_SetServo(miSTG_TBase , false);
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    static bool         bOrigPstn       = true             ;
    static bool         bRvrsPstn       = true             ;
    static EN_MOTR_ID   miWRK_UBrd      = miWRK_UBrd1      ;
    static EN_MOTR_ID   miWRK_VBrd      = miWRK_VBrd1      ;
    static EN_OUTPUT_ID yWRK_BrdVccL    = yWRK_Brd1VccL    ;
    static EN_OUTPUT_ID yWRK_BrdVccR    = yWRK_Brd1VccR    ;
    static EN_INPUT_ID  xWRK_BrdVccL    = xWRK_Brd1VccL    ;
    static EN_INPUT_ID  xWRK_BrdVccR    = xWRK_Brd1VccR    ;
    static EN_ERR_ID    eiWRK_BrdVccL   = eiWRK_Brd1VccL   ;
    static EN_ERR_ID    eiWRK_BrdVccR   = eiWRK_Brd1VccR   ;
    static EN_PSTN_ID   piSTG_TBase     = piSTG_TBaseRvers ;
    static EN_PSTN_ID   piWRK_VBrd_Wrk1 = piWRK_VBrd1_Wrk1 ;
    static EN_PSTN_ID   piWRK_UBrd_Wrk1 = piWRK_UBrd1_Wrk1 ;

    int iChekCount      = 0 ;

    if(OM.DevOptn.bUsePstU1) iChekCount++;
    if(OM.DevOptn.bUsePstU2) iChekCount++;
    if(OM.DevOptn.bUsePstU3) iChekCount++;
    if(OM.DevOptn.bUsePstU4) iChekCount++;
    if(OM.DevOptn.bUsePstU5) iChekCount++;
    if(OM.DevOptn.bUsePstU6) iChekCount++;

    const  int  iMaxLaserWrkCnt = iChekCount;           //총 작업하는 카운트 체크 된 체크 박스 수
    static int  iLaserWorkCnt   = 0 ;                   //레이저 작업 한 횟수
    static int  iRotateCnt      = 0 ;                   //레이저 스텝 반복 횟수 , 포지션 아이디 옵셑
    static int  iLaserWorkDev   = OM.DevOptn.iOriLsrDevU1 ;//OM.DevOptn.iLsrDevU에 값들, 레이저 디바이스 값
    static bool bPstnUsedRslt   = true ;                //OM.DevOptn.bUsePstU에 값 ,이걸로 반복 스텝 탈지 안탈지 정함

    static DWord dStartTime ;

    PreStep.iCycle = Step.iCycle ;
    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //현재 작업할 놈들 세팅.
                  dStartTime = GetTickCount();
                  aTemp11 = 0;
                  bOrigPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) - 1.0) <= MT_GetCmdPos(miSTG_TBase) &&
                                 (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) + 1.0) ;



                  bRvrsPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )-1.0  <= MT_GetCmdPos(miSTG_TBase)) &&
                                 (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )+1.0 ) ;

                  if(bOrigPstn) { //초기화 상태 1번에 있는놈들 작업할 차례
                      miWRK_UBrd      = miWRK_UBrd1     ;
                      miWRK_VBrd      = miWRK_VBrd1     ;
                      yWRK_BrdVccL    = yWRK_Brd2VccL   ;
                      yWRK_BrdVccR    = yWRK_Brd2VccR   ;
                      xWRK_BrdVccL    = xWRK_Brd1VccL   ;
                      xWRK_BrdVccR    = xWRK_Brd1VccR   ;
                      piSTG_TBase     = piSTG_TBaseRvers;
                      eiWRK_BrdVccL   = eiWRK_Brd1VccL  ;
                      eiWRK_BrdVccR   = eiWRK_Brd1VccR  ;
                      piWRK_VBrd_Wrk1 = piWRK_VBrd1_Wrk1;
                      piWRK_UBrd_Wrk1 = piWRK_UBrd1_Wrk1;

                      m_bNotWrkPstn = false;
                  }

                  else if(bRvrsPstn){
                      miWRK_UBrd      = miWRK_UBrd2     ;
                      miWRK_VBrd      = miWRK_VBrd2     ;
                      yWRK_BrdVccL    = yWRK_Brd1VccL   ;
                      yWRK_BrdVccR    = yWRK_Brd1VccR   ;
                      xWRK_BrdVccL    = xWRK_Brd2VccL   ;
                      xWRK_BrdVccR    = xWRK_Brd2VccR   ;
                      piSTG_TBase     = piSTG_TBaseOrign;
                      eiWRK_BrdVccL   = eiWRK_Brd2VccL  ;
                      eiWRK_BrdVccR   = eiWRK_Brd2VccR  ;
                      piWRK_VBrd_Wrk1 = piWRK_VBrd2_Wrk1;
                      piWRK_UBrd_Wrk1 = piWRK_UBrd2_Wrk1;

                      m_bNotWrkPstn = false;
                  }
                  else{
                      m_bNotWrkPstn = true;
                      //FM_MsgOk("Warning" , "베이스 판을 대기포지션이나 리버스포지션으로 옮겨주세요." );
                      Step.iCycle = 0;
                      return true;
                  }




                  IO_SetY(yWRK_Brd1VccL , true);
                  IO_SetY(yWRK_Brd1VccR , true);
                  IO_SetY(yWRK_Brd2VccL , true);
                  IO_SetY(yWRK_Brd2VccR , true);

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  11: if(!OM.CmnOptn.bVaccumSkip){
                      if(m_tmTemp.OnDelay(true , 1000)){
                          if(!IO_GetX(xWRK_BrdVccL)) EM_SetErr(eiWRK_BrdVccL);
                          if(!IO_GetX(xWRK_BrdVccR)) EM_SetErr(eiWRK_BrdVccR);
                          Step.iCycle = 0;
                          return true;
                      }

                      if(!IO_GetX(xWRK_BrdVccL) || !IO_GetX(xWRK_BrdVccR)) return false ;
                  }
                  MoveMotr(miWRK_VBrd1 , piWRK_VBrd1_Wait);
                  MoveMotr(miWRK_VBrd2 , piWRK_VBrd2_Wait);

                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_VBrd1 , piWRK_VBrd1_Wait)) return false;
                  if(!MoveMotr(miWRK_VBrd2 , piWRK_VBrd2_Wait)) return false;
                  MoveMotr(miWRK_UBrd1 , piWRK_UBrd1_Wait);
                  MoveMotr(miWRK_UBrd2 , piWRK_UBrd2_Wait);

                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_UBrd1 , piWRK_UBrd1_Wait)) return false;
                  if(!MoveMotr(miWRK_UBrd2 , piWRK_UBrd2_Wait)) return false;
                  Step.iCycle++;
                  return false;

        //위에 에러에서 사용함.
        case  14: if(!MT_GetHomeSnsr(miWRK_UBrd1) || !MT_GetHomeSnsr(miWRK_UBrd1)){
                      EM_SetErr(eiWRK_NotBrdPstn);
                      Step.iCycle = 0;
                      return true;
                  }
                  if(IO_GetX(xETC_AreaSensor)){
                      if(!MT_GetStop(miSTG_TBase)) MT_Stop(miSTG_TBase);
                      return false;
                  }
                  if(IO_GetX(xSTG_TltDetect)){    // 팅팅 센서 확인 JH
                      MT_Stop(miSTG_TBase);
                      EM_SetErr(eiSTG_TiltDetect);
                      Step.iCycle = 0;
                      return true;
                  }

                  if(!MoveMotr(miSTG_TBase , piSTG_TBase)) return false;
                  Step.iCycle ++;
                  return false;


        //에어리어 센서 검사 끝...  스텝을 위에서 쓴다...
        case  15: if(!MoveMotr(miSTG_TBase , piSTG_TBase)) return false; //

                  //전작업마지막에 GA블럭체인지 보낸것 잘 바뀌었나 확인.
                  if(!OM.CmnOptn.bLaserSkip) {
                      if(!LCU.GetGAFinished()) return false ;
                  }
                  IO_SetY(yWRK_BrdVccL , false);
                  IO_SetY(yWRK_BrdVccR , false);

                  iRotateCnt    = 0 ;
                  iLaserWorkCnt = 0 ;

                  if(bOrigPstn) {
                      iLaserWorkDev = OM.DevOptn.iOriLsrDevU1 ;
                      if(!OM.CmnOptn.bLaserSkip && LCU.GetBlockNoID() != iLaserWorkDev){
                          LCU.SetGABlockId(OM.DevOptn.iOriLsrDevU1);
                          LCU.SetSendMsgId(lmGA);
                          EM_SetErr(eiSTG_LaserBlock);
                      }
                  }
                  else {
                      iLaserWorkDev = OM.DevOptn.iRvsLsrDevU1 ;
                      if(!OM.CmnOptn.bLaserSkip && LCU.GetBlockNoID() != iLaserWorkDev ){
                          LCU.SetGABlockId(OM.DevOptn.iOriLsrDevU1);
                          LCU.SetSendMsgId(lmGA);
                          EM_SetErr(eiSTG_LaserBlock);
                      }

                  }
                  bPstnUsedRslt = OM.DevOptn.bUsePstU1 ;

                  Step.iCycle++;
                  return false ;

                  //여기부터 레이저 작업....  스탭 밑에서 사용...
        case  16: if(bPstnUsedRslt) { //OM.DevOptn.bUsePstU 이 true 이면 다음 스텝으로...
                      Step.iCycle++;
                      return false;
                  }
                  else {
                      Step.iCycle = 21;    //false 이면 20번으로
                      return false;
                  }

        case  17: MoveMotr(miWRK_VBrd , (int)piWRK_VBrd_Wrk1 + iRotateCnt );
                  MoveMotr(miWRK_UBrd , (int)piWRK_UBrd_Wrk1 + iRotateCnt );
                  if(iRotateCnt != 0 && !OM.CmnOptn.bLaserSkip){
                      LCU.SetGABlockId(iLaserWorkDev ); //레이저 작업
                      LCU.SetSendMsgId(lmGA);

                  }


                  Step.iCycle++;
                  return false;

        case  18: if(!MoveMotr(miWRK_VBrd , (int)piWRK_VBrd_Wrk1 + iRotateCnt)) return false;
                  if(!MoveMotr(miWRK_UBrd , (int)piWRK_UBrd_Wrk1 + iRotateCnt)) return false;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  19:
                  if(!OM.CmnOptn.bLaserSkip){
                      if(iRotateCnt != 0){
                          if(!LCU.GetGAFinished()) return false ;
                      }
                      LCU.SetSendMsgId(lmTX);
                  }
                  iLaserWorkCnt++;
                  Step.iCycle++;
                  return false;

        case  20: if(!OM.CmnOptn.bLaserSkip){
                          if(LCU.GetLaserMsgId() != lmNone)return false ;
                  }
                  Step.iCycle++;
                  return false;

                  //스텝 위에서 사용....
        case  21: if(iLaserWorkCnt >= iMaxLaserWrkCnt) { //총 작업해야 할 레이저 카운트와 현재 작업한 레이저 카운트 비교
                      iRotateCnt    = 0 ;
                      iLaserWorkCnt = 0 ;

                      if(bOrigPstn) { iLaserWorkDev = OM.DevOptn.iOriLsrDevU1 ; }
                      if(bRvrsPstn) { iLaserWorkDev = OM.DevOptn.iRvsLsrDevU1 ; }

                      bPstnUsedRslt = OM.DevOptn.bUsePstU1 ;
                      if(!OM.CmnOptn.bLaserSkip){//여기 나누자
                          if(bOrigPstn){
                              LCU.SetGABlockId(OM.DevOptn.iRvsLsrDevU1);
                              LCU.SetSendMsgId(lmGA);
                          }
                          if(bRvrsPstn){
                              LCU.SetGABlockId(OM.DevOptn.iOriLsrDevU1);
                              LCU.SetSendMsgId(lmGA);
                          }
                      }
                      Step.iCycle++; //작업종료 절차.
                      return false;
                  }
                  //로테이트 카운트에 따라 옵션값 가져 오기....
                  if(bOrigPstn){
                      if(iRotateCnt == 0) { bPstnUsedRslt = OM.DevOptn.bUsePstU2; iLaserWorkDev = OM.DevOptn.iOriLsrDevU2; }
                      if(iRotateCnt == 1) { bPstnUsedRslt = OM.DevOptn.bUsePstU3; iLaserWorkDev = OM.DevOptn.iOriLsrDevU3; }
                      if(iRotateCnt == 2) { bPstnUsedRslt = OM.DevOptn.bUsePstU4; iLaserWorkDev = OM.DevOptn.iOriLsrDevU4; }
                      if(iRotateCnt == 3) { bPstnUsedRslt = OM.DevOptn.bUsePstU5; iLaserWorkDev = OM.DevOptn.iOriLsrDevU5; }
                      if(iRotateCnt == 4) { bPstnUsedRslt = OM.DevOptn.bUsePstU6; iLaserWorkDev = OM.DevOptn.iOriLsrDevU6; }
                      iRotateCnt++;
                  }
                  if(bRvrsPstn){
                      if(iRotateCnt == 0) { bPstnUsedRslt = OM.DevOptn.bUsePstU2; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU2; }
                      if(iRotateCnt == 1) { bPstnUsedRslt = OM.DevOptn.bUsePstU3; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU3; }
                      if(iRotateCnt == 2) { bPstnUsedRslt = OM.DevOptn.bUsePstU4; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU4; }
                      if(iRotateCnt == 3) { bPstnUsedRslt = OM.DevOptn.bUsePstU5; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU5; }
                      if(iRotateCnt == 4) { bPstnUsedRslt = OM.DevOptn.bUsePstU6; iLaserWorkDev = OM.DevOptn.iRvsLsrDevU6; }
                      iRotateCnt++;
                  }

                  Step.iCycle = 16;
                  return false    ;

                  // 레이저 작업이 다 끝나고 대기 포지션으로 이동하고 베큠 끄기
        case  22: MoveMotr(miWRK_VBrd , piWRK_VBrd1_Wait);
                  MoveMotr(miWRK_UBrd , piWRK_UBrd1_Wait);
                  Step.iCycle++;
                  return false;

        case  23: if(!MoveMotr(miWRK_VBrd , piWRK_VBrd1_Wait))return false;
                  if(!MoveMotr(miWRK_UBrd , piWRK_UBrd1_Wait))return false;

                  IO_SetY(yWRK_Brd1VccL , false);
                  IO_SetY(yWRK_Brd1VccR , false);
                  IO_SetY(yWRK_Brd2VccL , false);
                  IO_SetY(yWRK_Brd2VccR , false);

                  LotInfoAdd();
                  m_dLaserWorkTime = GetTickCount() - dStartTime ;
                  Step.iCycle = 0;
                  return true;


    }
}
*/

bool CStage::CyclePlace()
{
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() /*&& !OM.MstOptn.bDebugMode*/, 15000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if (m_tmCycle.OnDelay(Step.iCycle == 13 && MT_GetStop(miSTG_TBase) , 10000 )) {
        EM_SetErr(eiETC_AreaSensor);
        MT_SetServo(miSTG_TBase , false);
        Step.iCycle = 0 ;
        return true;
    }
    
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }
    static bool bOrigPstn = true ;
    static bool bRvrsPstn = true ;
    static EN_PSTN_ID   piSTG_TBase  = piSTG_TBaseRvers ;
    static EN_OUTPUT_ID yWRK_BrdVccL = yWRK_Brd1VccL    ;
    static EN_OUTPUT_ID yWRK_BrdVccR = yWRK_Brd1VccR    ;

    PreStep.iCycle = Step.iCycle ;
    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;



        case  10: bOrigPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) - 1.0) <= MT_GetCmdPos(miSTG_TBase) &&
                                 (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseOrign) + 1.0) ;

                  bRvrsPstn    = (PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )-1.0  <= MT_GetCmdPos(miSTG_TBase)) &&
                                 (MT_GetCmdPos(miSTG_TBase) <= PM.GetValue(miSTG_TBase , pvSTG_TBaseRvers )+1.0 ) ;

                  if(bOrigPstn) { piSTG_TBase     = piSTG_TBaseRvers; }
                  else          { piSTG_TBase     = piSTG_TBaseOrign; }

                  if(bOrigPstn) { //초기화 상태 1번에 있는놈들 작업할 차례
                      yWRK_BrdVccL    = yWRK_Brd2VccL   ;
                      yWRK_BrdVccR    = yWRK_Brd2VccR   ;
                      piSTG_TBase     = piSTG_TBaseRvers;

                      m_bNotWrkPstn = false;
                  }
                  else if(bRvrsPstn){ //2번에 있는 놈들 작업할 차례
                      yWRK_BrdVccL    = yWRK_Brd1VccL   ;
                      yWRK_BrdVccR    = yWRK_Brd1VccR   ;
                      piSTG_TBase     = piSTG_TBaseOrign;

                      m_bNotWrkPstn = false;
                  }
                  else {
                      m_bNotWrkPstn = true;
                      //FM_MsgOk("Warning" , "베이스 T축을 대기 포지션이나 리버스 포지션으로 옮겨주세요." );
                      //EM_SetErr(eiWRK_TBaseWrkps);
                      Step.iCycle = 0;
                      return true;   
                  }

                  IO_SetY(yWRK_BrdVccL , true);
                  IO_SetY(yWRK_BrdVccR , true);
                  if(!OM.CmnOptn.bNotUsedVAxis){
                      MoveMotr(miWRK_VBrd1 , piWRK_VBrd1_Wait);
                      MoveMotr(miWRK_VBrd2 , piWRK_VBrd2_Wait);
                  }

                  Step.iCycle++;
                  return false;

        case  11: if(!OM.CmnOptn.bNotUsedVAxis){
                      if(!MoveMotr(miWRK_VBrd1 , piWRK_VBrd1_Wait)) return false;
                      if(!MoveMotr(miWRK_VBrd2 , piWRK_VBrd2_Wait)) return false;
                  }
                  MoveMotr(miWRK_UBrd1 , piWRK_UBrd1_Wait) ;
                  MoveMotr(miWRK_UBrd2 , piWRK_UBrd2_Wait) ;
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_UBrd1 , piWRK_UBrd1_Wait)) return false;
                  if(!MoveMotr(miWRK_UBrd2 , piWRK_UBrd2_Wait)) return false;

                  //MoveMotr(miSTG_TBase , piSTG_TBase);
                  Step.iCycle++;
                  return false;

        case  13: if(!MT_GetHomeSnsr(miWRK_UBrd1) || !MT_GetHomeSnsr(miWRK_UBrd1)){
                      EM_SetErr(eiWRK_NotBrdPstn);
                      Step.iCycle = 0;
                      return true;
                  }
                  if(IO_GetX(xETC_AreaSensor)){
                      if(!MT_GetStop(miSTG_TBase)) MT_Stop(miSTG_TBase);
                      return false;
                  }
                  if(IO_GetX(xSTG_TltDetect)){    // 틸팅 센서 확인 JH
                      MT_Stop(miSTG_TBase);
                      EM_SetErr(eiSTG_TiltDetect);
                      Step.iCycle = 0;
                      return true;
                  }

                  if(!MoveMotr(miSTG_TBase , piSTG_TBase)) return false;
                  Step.iCycle ++;
                  return false;


        //에어리어 센서 검사 끝...  스텝을 위에서 쓴다...
        case  14: if(!MoveMotr(miSTG_TBase , piSTG_TBase)) return false;
                  IO_SetY(yWRK_BrdVccL , false);
                  IO_SetY(yWRK_BrdVccR , false);

                  //전작업마지막에 GA블럭체인지 보낸것 잘 바뀌었나 확인.
                  if(!OM.CmnOptn.bLaserSkip && !OM.CmnOptn.bDryRun) {
                      if(!LCU.GetGAFinished()) return false ;
                  }



                  if(!OM.CmnOptn.bLaserSkip && !OM.CmnOptn.bDryRun){//여기 나누자
                      if(bOrigPstn){
                          LCU.SetGABlockId(OM.DevOptn.iRvsLsrDevU1);
                          LCU.SetSendMsgId(lmGA);
                      }
                      if(bRvrsPstn){
                          LCU.SetGABlockId(OM.DevOptn.iOriLsrDevU1);
                          LCU.SetSendMsgId(lmGA);
                      }
                  }


                  Step.iCycle = 0;
                  return true;

    }
}




void CStage::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CStage::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CStage::CheckStop()
{
    if(!MT_GetStop(miSTG_TBase)) return false ;
    if(!MT_GetStop(miWRK_UBrd1)) return false ;
    if(!MT_GetStop(miWRK_UBrd2)) return false ;
    if(!OM.CmnOptn.bNotUsedVAxis){
        if(!MT_GetStop(miWRK_VBrd1)) return false ;
        if(!MT_GetStop(miWRK_VBrd2)) return false ;
    }


    return true ;
}

void CStage::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {

    }
    else {
    }
}
//---------------------------------------------------------------------------
bool CStage::LotInfoAdd()
{
    LT.WriteLotDayLog();

    if(OM.CmnOptn.bUseOnlyRgh){LT.AddTodayWorkCnt(1);}
    else                      {LT.AddTodayWorkCnt(2);}

}



