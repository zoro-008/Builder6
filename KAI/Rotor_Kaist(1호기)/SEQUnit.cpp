//---------------------------------------------------------------------------
#pragma hdrstop
#include "SEQUnit.h"
#include "PaixMotion.h"
#include "BarslerUnit.h"
#include "Timer.h"

#include <math.h>
#include "FormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
CSEQ *pSeq;

#define Msg if(m_pMsgFunc)Msg
__fastcall CSEQ::CSEQ(void):TThread(TRUE)
{
    m_slSeq    = new TStringList();

//    m_dORSpeed = new double[100] ;
//    m_dORTime  = new double[100] ;
    m_FuncInfo = NULL ;
    m_pMsgFunc = NULL ;

    m_iMaxLines = 0 ;
    m_iMaxVal   = 0 ;

    //Time Check
    dTotalTime  = 0 ;
    iForStartCnt= 0 ;
    iForEndCnt  = 0 ;
    memset(&iForStart , 0 , sizeof(iForStart));
    memset(&iForEnd   , 0 , sizeof(iForEnd  ));
    //
}

__fastcall CSEQ::~CSEQ(void)
{

    if(m_slSeq) delete m_slSeq ;

}
void CSEQ::StartSeq  ()
{
    CurrDateTime = Now();
//    sSeqStartTime          = CurrDateTime.FormatString("(yy.mm.dd)hh:nn:ss");
    sSeqStartTime          = CurrDateTime.FormatString("hh:nn:ss");

    m_bSeqInit = false ;
    Resume();
}
double CSEQ::GetTotalTime ()
{
    return dTotalTime ;
}
double CSEQ::GetRemainTime()
{
//시간 체크.
    double dRemainTime = 0 ;
    for(int i = 0 ; i < m_iMaxLines ; i++) {
        dRemainTime += m_FuncInfo[i].dReMainTime ;
        dRemainTime += m_FuncInfo[i].dTime       ;
    }
    return dRemainTime ;
}

bool CSEQ::GetFuncName(AnsiString &_sLine , EN_FUNC &_fcName )
{
         if (_sLine.Pos   ("//"            ) == 1) { _fcName = fcCOMMENT       ; return true ; }
    else if (_sLine.Length(                ) == 0) { _fcName = fcEMPTY         ; return true ; }
    else if (_sLine.Pos   ("CHECKFINISH"   ) == 1) { _fcName = fcCHECKFINISH   ; _sLine.Delete(1,AnsiString("CHECKFINISH"   ).Length()) ; return true ; }
    else if (_sLine.Pos   ("MOVE0RPM"      ) == 1) { _fcName = fcMOVE0RPM      ; _sLine.Delete(1,AnsiString("MOVE0RPM"      ).Length()) ; return true ; }
    else if (_sLine.Pos   ("MOVEHOME"      ) == 1) { _fcName = fcMOVEHOME      ; _sLine.Delete(1,AnsiString("MOVEHOME"      ).Length()) ; return true ; }
    else if (_sLine.Pos   ("MOVEABS"       ) == 1) { _fcName = fcMOVEABS       ; _sLine.Delete(1,AnsiString("MOVEABS"       ).Length()) ; return true ; }
    else if (_sLine.Pos   ("MOVEINC"       ) == 1) { _fcName = fcMOVEINC       ; _sLine.Delete(1,AnsiString("MOVEINC"       ).Length()) ; return true ; }
    else if (_sLine.Pos   ("LASER"         ) == 1) { _fcName = fcLASER         ; _sLine.Delete(1,AnsiString("LASER"         ).Length()) ; return true ; }
    else if (_sLine.Pos   ("DELAY"         ) == 1) { _fcName = fcDELAY         ; _sLine.Delete(1,AnsiString("DELAY"         ).Length()) ; return true ; }
    else if (_sLine.Pos   ("CAMERA"        ) == 1) { _fcName = fcCAMERA        ; _sLine.Delete(1,AnsiString("CAMERA"        ).Length()) ; return true ; }
    else if (_sLine.Pos   ("SERVO0"        ) == 1) { _fcName = fcSERVO0        ; _sLine.Delete(1,AnsiString("SERVO0"        ).Length()) ; return true ; }
    else if (_sLine.Pos   ("SHAKE0"        ) == 1) { _fcName = fcSHAKE0        ; _sLine.Delete(1,AnsiString("SHAKE0"        ).Length()) ; return true ; }
    else if (_sLine.Pos   ("OR_SETSPEED0"  ) == 1) { _fcName = fcOR_SETSPEED0  ; _sLine.Delete(1,AnsiString("OR_SETSPEED0"  ).Length()) ; return true ; }
    else if (_sLine.Pos   ("OR_SETTIME0"   ) == 1) { _fcName = fcOR_SETTIME0   ; _sLine.Delete(1,AnsiString("OR_SETTIME0"   ).Length()) ; return true ; }
    else if (_sLine.Pos   ("OR_SETSTART0"  ) == 1) { _fcName = fcOR_SETSTART0  ; _sLine.Delete(1,AnsiString("OR_SETSTART0"  ).Length()) ; return true ; }
    else if (_sLine.Pos   ("COOLING"       ) == 1) { _fcName = fcCOOLING       ; _sLine.Delete(1,AnsiString("COOLING"       ).Length()) ; return true ; }
    else if (_sLine.Pos   ("CAMLIVE"       ) == 1) { _fcName = fcCAMLIVE       ; _sLine.Delete(1,AnsiString("CAMLIVE"       ).Length()) ; return true ; }
    else if (_sLine.Pos   ("SETHEAT"       ) == 1) { _fcName = fcSETHEAT       ; _sLine.Delete(1,AnsiString("SETHEAT"       ).Length()) ; return true ; }
    else if (_sLine.Pos   ("FORSTART"      ) == 1) { _fcName = fcFORSTART      ; _sLine.Delete(1,AnsiString("FORSTART"      ).Length()) ; return true ; }
    else if (_sLine.Pos   ("FOREND"        ) == 1) { _fcName = fcFOREND        ; _sLine.Delete(1,AnsiString("FOREND"        ).Length()) ; return true ; }
    else                                           {                                                                                     return false ; }
}

bool CSEQ::GetPara(AnsiString &_sLine , double &_dPara)
{
    AnsiString sTemp ;

    if( _sLine.Pos("(")==1) _sLine.Delete(1,1) ;


         if(_sLine.Pos(",")){sTemp  = _sLine.SubString(1,_sLine.Pos(",")-1) ; _sLine.Delete(1,_sLine.Pos(",")-1) ;}
    else if(_sLine.Pos(")")){sTemp  = _sLine.SubString(1,_sLine.Pos(")")-1) ; _sLine.Delete(1,_sLine.Pos(")")-1) ;} //Same ?

         if(_sLine.Pos(",")==1) _sLine.Delete(1,1) ;
    else if(_sLine.Pos(")")==1) _sLine.Delete(1,1) ;

    _dPara = StrToFloatDef(sTemp , -99999) ;

    if(_dPara == -99999) return false ;

    return true ;
}

AnsiString CSEQ::GetErrName()
{
    return m_sErrName ;
}

bool CSEQ::CheckCode(TStrings * _slSourse)
{
    AnsiString sTemp ;
    EN_FUNC    iFunc ;
    double     dPara ;

    //
    memset(&iForStart  , 0 , sizeof(iForStart  ));
    memset(&iForEnd    , 0 , sizeof(iForEnd    ));
    memset(&iForCnt    , 0 , sizeof(iForCnt    ));

    iForStartCnt = 0 ;
    iForEndCnt   = 0 ; // 아오 ...하기 싫은데...이거는...아 중국가면 안해줘야 겟다...
//    int iTotalTime = 0 ;
    dTotalTime = 0 ;
    //

    m_sErrName = "No Error." ;

    m_iMaxLines = _slSourse -> Count ;

    if(!m_FuncInfo) delete m_FuncInfo ;
    m_FuncInfo = new TFuncInfo[m_iMaxLines] ;
    memset(m_FuncInfo , 0 , sizeof(TFuncInfo)*m_iMaxLines);

//    double dTemp , dTemp1 , dTemp2 ,dTemp3;
    double dTemp ;

    for(int i = 0 ; i < _slSourse -> Count ; i++) {
        sTemp = _slSourse -> Strings[i] ;
        while(sTemp.Pos(" ")) {
            sTemp.Delete(sTemp.Pos(" "),1) ;
        }

        if(!GetFuncName(sTemp , iFunc)) {
            m_sErrName = "Line " + AnsiString(i) + "-알수 없는 함수명입니다.";
            return false ;
        }
        m_FuncInfo[i].iFunc = iFunc ;

        if(!sTemp.Pos(";") && (iFunc != fcEMPTY && iFunc != fcCOMMENT )) {
            m_sErrName = "Line " + AnsiString(i) + "-종료문자 ';'가 없습니다.";
            return false ;
        }
        sTemp = sTemp.SubString(1,sTemp.Pos(";")-1) ;

        switch(iFunc) {
            case fcCHECKFINISH   : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;
                                   break ;

            case fcMOVE0RPM      : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터2의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara2 = dPara ; //가감속시간.
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터3의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara3 = dPara ; //정속시간.
                                   m_FuncInfo[i].dTime = m_FuncInfo[i].dPara2 * 2 + m_FuncInfo[i].dPara3 ;
                                   break ;

            case fcMOVEHOME      : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;
                                   m_FuncInfo[i].dTime = 5000; //아이 이건 도저히 귀찮아서 못하겟다....
                                   break ;

            case fcMOVEABS       : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ; //모터
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터2의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara2 = dPara ; //가속도
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터3의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara3 = dPara ; //속도
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터4의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara4 = dPara ; //거리
/*
                                   dTemp3 = m_FuncInfo[i].dPara3 / m_FuncInfo[i].dPara2 ; // Time
                                   dTemp  = 0.5 * m_FuncInfo[i].dPara2 * dTemp3 * dTemp3 ;
//                                   dTemp  = m_FuncInfo[i].dPara3 * m_FuncInfo[i].dPara3 / 2* m_FuncInfo[i].dPara2 ; // S
                                   dTemp2 = m_FuncInfo[i].dPara4 - ( dTemp * 2 ) ;
                                   m_FuncInfo[i].dTime  = dTemp2 / m_FuncInfo[i].dPara3 ;
                                   m_FuncInfo[i].dTime += sqrt(2 * dTemp / m_FuncInfo[i].dPara2) ; // s= v0t + 1/2at^2
*/
                                   dTemp = m_FuncInfo[i].dPara3 ? m_FuncInfo[i].dPara3 : 1 ;
                                   m_FuncInfo[i].dTime  = m_FuncInfo[i].dPara4 / dTemp * 1000;

                                   break ;

            case fcMOVEINC       : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터2의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara2 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터3의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara3 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터4의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara4 = dPara ;
                                   dTemp = m_FuncInfo[i].dPara3 ? m_FuncInfo[i].dPara3 : 1 ;
                                   m_FuncInfo[i].dTime  = m_FuncInfo[i].dPara4 / dTemp * 1000;
                                   break ;

            case fcLASER         : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;

            case fcDELAY         : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;
                                   m_FuncInfo[i].dTime = m_FuncInfo[i].dPara1 ;
                                   break ;

            case fcCOMMENT       :                                                                                                                                                      break ;

            case fcEMPTY         :                                                                                                                                                      break ;


            case fcCAMERA        : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;
            case fcSERVO0        : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;
            case fcSHAKE0        : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터2의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara2 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터3의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara3 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터4의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara4 = dPara ;
                                   m_FuncInfo[i].dTime = m_FuncInfo[i].dPara1 ;
                                   break ;

            case fcOR_SETSPEED0  : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_iMaxVal = dPara ;
                                   if( m_iMaxVal != dPara && m_iMaxVal  ) {m_sErrName = "Line " + AnsiString(i) + "OR_SETSPEED0 의 Max 값과 SETTIME0 의 Max 값이 다릅니다." ; return false ;}

                                   for(int iCnt = 0 ; iCnt< m_iMaxVal ; iCnt++ ) {
                                       if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터" + AnsiString(iCnt) + "의 형태가 이상합니다." ; return false ;}
                                       m_dORSpeed[iCnt] = dPara ;
                                   }
                                   break ;

            case fcOR_SETTIME0   : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;}
                                   if( m_iMaxVal != dPara && m_iMaxVal  ) {m_sErrName = "Line " + AnsiString(i) + "OR_SETSPEED0 의 Max 값과 SETTIME0 의 Max 값이 다릅니다." ; return false ;}
                                   m_iMaxTime = 0 ;
                                   for(int iCnt = 0 ; iCnt< m_iMaxVal ; iCnt++ ) {
                                       if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터" + AnsiString(iCnt) + "의 형태가 이상합니다." ; return false ;}
                                       m_dORTime[iCnt] = dPara ;
                                       m_iMaxTime += dPara ;
                                   }
                                   m_FuncInfo[i].dTime = m_iMaxTime ;
                                   break ;

            case fcOR_SETSTART0  : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;

            case fcFORSTART      : iForStart[iForStartCnt] = i ; iForStartCnt++; break ;//if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;
            case fcFOREND        : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;}
                                   m_FuncInfo[i].dPara1 = dPara ;
                                   iForEnd[iForEndCnt]  = i ;
                                   iForCnt[iForEndCnt]  = dPara ;
                                   iForEndCnt++;
                                   break ;

            case fcCOOLING       : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;
            case fcCAMLIVE       : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;
            case fcSETHEAT       : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;}
                                   if( dPara != 1 && dPara != 2 && dPara != 3 ) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 값이 1,2,3 이 아닙니다." ; return false ;}
                                   m_FuncInfo[i].dPara1 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터2의 형태가 이상합니다." ; return false ;}
                                   if( dPara < 0 || dPara > 1550 ) {m_sErrName = "Line " + AnsiString(i) + "파라미터2의 값이 0과 1550의 사이가 아닙니다." ; return false ;}
                                   m_FuncInfo[i].dPara2 = (int)dPara ;
                                   break ;

            break ;
        }

        if(sTemp.Length() && iFunc != fcCOMMENT ) {
            m_sErrName = "Line " + AnsiString(i) + "파라미터가 많거나 알수 없는 문자가 있습니다." ;
            return false ;
        }
    }

//총 시간 체크.
    if(iForEndCnt > 0) { //여기서 포문애들 곱하고... 나중에 자기를 빼면 되긴하겟다...
        for(int i = 0 ; i < iForEndCnt ; i++) {
//            for( int k = 0 ; k < iForCnt[i] ; k++ ) {
                for( int j = iForStart[i] ; j < iForEnd[i] ; j++ ) {
//                    m_FuncInfo[j].iTime += m_FuncInfo[j].iTime ;
//                    m_FuncInfo[j].dReMainTime = m_FuncInfo[j].dTime * ( iForCnt[i] + 1 );
                    m_FuncInfo[j].dReMainTime = m_FuncInfo[j].dTime * ( iForCnt[i] );
    //                iTotalTime += m_FuncInfo[j].iTime ;
                }
//            }
        }
    }

    for(int i = 0 ; i < _slSourse -> Count ; i++) {
//        if(m_FuncInfo[i].dReMainTime) dTotalTime += m_FuncInfo[i].dReMainTime ;
//        else                          dTotalTime += m_FuncInfo[i].dTime       ;
        dTotalTime += m_FuncInfo[i].dTime       ;
        dTotalTime += m_FuncInfo[i].dReMainTime ;
    }



    return true ;


}


//---------------------------------------------------------------------------
void __fastcall CSEQ::Execute()
{
    static int  iStep    = 0     ;
    static int  iPreStep = 0     ;
    static int  iForStep = 0     ;
    //static bool bInit    = false ;

    int iTimeOutMs = 0 ;
                              //Resume()

    CDelayTimer TimeOutTimer ;
    CDelayTimer TempTimer    ;

    while( !Terminated )
    {
        if(!m_bSeqInit ) {
            TimeOutTimer.Clear();
            TempTimer.Clear();
            iPreStep = -1 ;
            iStep = 0 ;
            m_bSeqInit = true ;
            continue ;
        }

        if(iStep != iPreStep) m_pMsgFunc(AnsiString(iStep) + "_Line Start(" + GetFuncNameString(m_FuncInfo[iStep].iFunc) + ")");

             if(m_FuncInfo[iStep].iFunc == fcCHECKFINISH) iTimeOutMs = 360000  ;
        else if(m_FuncInfo[iStep].iFunc == fcDELAY      ) iTimeOutMs = 4000000 ;
        else if(m_FuncInfo[iStep].iFunc == fcSHAKE0     ) iTimeOutMs = 300000  ;
        else if(m_FuncInfo[iStep].iFunc == fcCAMERA     ) iTimeOutMs = 300000  ;
        else                                              iTimeOutMs = 10000   ;

        if(TimeOutTimer.OnDelay(iStep == iPreStep , iTimeOutMs )) {
            TimeOutTimer.Clear();
            m_bSeqInit=false ;
            m_bReqStop = false ;
            m_pMsgFunc(AnsiString(iStep) + "_Line TimeOut");
            iStep = 0 ;
            Suspend() ;
            pPaix -> StopMotor(0);
            pPaix -> StopMotor(1);
            continue ;
            //pPaix->Stat[
        }

        if(m_bReqStop) {
            m_bReqStop = false ;
            pGrabRun -> StopGrab();
//            pGrabRun -> TriggerHW(); //이거 미확인. (트리거 모드 바뀔때 카메라 제대로 작동 하는지)
            pPaix->SetOutputBit(yETC_CoolingFan,1);
//            pGrabRun -> MakeAviThread() ;
            pPaix -> SetServoOnOff(Axis1 , true) ; //servooff.
            m_bSeqInit=false ; //이거 안하고 그냥 스타트 ???
            m_pMsgFunc(AnsiString(iStep) + "_Line SEQ Stopped");
            iStep = 0 ;
            Suspend() ;
            pPaix -> StopMotor(0);
            pPaix -> StopMotor(1);
            continue ;
        }

        iPreStep = iStep ;

        switch(m_FuncInfo[iStep].iFunc) {
            case fcCHECKFINISH   : if(!pPaix -> GetbHomeDone (m_FuncInfo[iStep].dPara1)) continue;
                                   if( pPaix -> GetBusy      (m_FuncInfo[iStep].dPara1)) continue;
                                   if(!pPaix -> GetInpo      (m_FuncInfo[iStep].dPara1)) continue;
                                   if( pPaix -> GetbShaking  (m_FuncInfo[iStep].dPara1)) continue;
                                   if( pPaix -> GetbOverRide (m_FuncInfo[iStep].dPara1)) continue;
                                   break ;

            case fcMOVE0RPM      :     pPaix -> MoveRPM    (Axis1 ,m_FuncInfo[iStep].dPara1 ,m_FuncInfo[iStep].dPara2,m_FuncInfo[iStep].dPara3 );
                                   break ;

            case fcMOVEHOME      :     pPaix -> MoveHome   (m_FuncInfo[iStep].dPara1 ) ;
                                   break ;

            case fcMOVEABS       : if(m_FuncInfo[iStep].dPara1 == 0) pPaix -> RotateReset(Axis1) ;
                                       pPaix -> SetSpeedDist (m_FuncInfo[iStep].dPara1,1,m_FuncInfo[iStep].dPara2, m_FuncInfo[iStep].dPara3,0 );    //StartSpeed , Acc , Speed , Jerk
                                       pPaix -> AbsMove      (m_FuncInfo[iStep].dPara1,m_FuncInfo[iStep].dPara4 ) ;
                                   break ;

            case fcMOVEINC       : //if(m_FuncInfo[iStep].dPara1 == 0) pPaix -> RotateReset(Axis1) ;
                                       pPaix -> SetSpeedDist (m_FuncInfo[iStep].dPara1,1,m_FuncInfo[iStep].dPara2, m_FuncInfo[iStep].dPara3,0 );    //StartSpeed , Acc , Speed , Jerk
                                       pPaix -> RelMove      (m_FuncInfo[iStep].dPara1,m_FuncInfo[iStep].dPara4 ) ;
                                   break ;

            case fcLASER         : if(m_FuncInfo[iStep].dPara1 == 0) pPaix->SetOutputBit(2,1);
                                   else                              pPaix->SetOutputBit(2,0);
                                   break ;

            case fcDELAY         : if(!TempTimer.OnDelay(true,m_FuncInfo[iStep].dPara1)) continue;
                                   break ;

            case fcCOMMENT       : //할일 없음.
                                   break ;

            case fcEMPTY         : //할일 없음.
                                   break ;

            case fcCAMERA        : if(m_FuncInfo[iStep].dPara1 == 0) {
                                       pGrabRun->StopGrab();
//                                       pGrabRun->MakeAviThread();              //사진찍은거 동영상 제작 시작.  pGrabRun->IsMakingAvi() 요놈이 동영상다만들때 까지 리턴 트루임.
                                   }
                                   else {
//                                       if(pGrabRun->IsMakingAvi())continue ;   //전에 사진찍은게 아직 동영상이 안만들어 졌으면 기달림.
                                       pGrabRun->Countbmp = 0 ; 
                                       pGrabRun->TriggerHW();
                                       pGrabRun->ContinuousShot();
                                   }
                                   break ;
            case fcSERVO0        : pPaix-> SetServoOnOff(0,!m_FuncInfo[iStep].dPara1) ;
                                   break ;

            case fcSHAKE0        : pPaix -> SetSpeedDist (0,1,m_FuncInfo[iStep].dPara2, m_FuncInfo[iStep].dPara3,0 );    //Axis , StartSpeed , Acc , Speed , Jerk
                                   pPaix -> MoveShake(0,m_FuncInfo[iStep].dPara1 , m_FuncInfo[iStep].dPara4);
                                   break ;
            case fcOR_SETSPEED0  : //할일 없음.
                                   break ;

            case fcOR_SETTIME0   : //할일 없음.
                                   break ;

            case fcOR_SETSTART0  : pPaix -> OR_Start(m_iMaxTime,m_iMaxVal,m_dORSpeed,m_dORTime,m_FuncInfo[iStep].dPara1);
                                   break ;

            case fcCOOLING       : if(m_FuncInfo[iStep].dPara1 == 0) pPaix->SetOutputBit(yETC_CoolingFan,1);
                                   else                              pPaix->SetOutputBit(yETC_CoolingFan,0);
                                   break ;
            case fcCAMLIVE       : if(m_FuncInfo[iStep].dPara1 == 0) {
                                       pGrabRun->TriggerHW(); //확실하게 되는지 모르겟음.
                                       FrmMain->tmLiveCam -> Enabled = false ;
                                   }//pGrabRun->TriggerSW(); pGrabRun->ContinuousShot(); pGrabRun->Resume (); }
                                   else                              {
                                       pGrabRun->ContinuousShot(); //확실하게 되는지 모르겟음.
                                       pGrabRun->TriggerSW(); //확실하게 되는지 모르겟음.
                                       FrmMain->tmLiveCam -> Enabled = true  ;
                                   }//pGrabRun->Suspend(); }
                                   break ;

            case fcSETHEAT       : FrmMain->SetHeat(m_FuncInfo[iStep].dPara1 , m_FuncInfo[iStep].dPara2 );
                                   break ;

            case fcFORSTART      : iForStep = iStep ;
                                   break ;

            case fcFOREND        : if(m_FuncInfo[iStep].dPara1 > 0) {
                                       m_FuncInfo[iStep].dPara1 -= 1 ;
                                       iStep = iForStep ;
                                   }
                                   break ;

//            case fcSETDRIVESPD0: pPaix -> SetDriveSpeed(0,m_FuncInfo[iStep].dPara1,m_FuncInfo[iStep].dPara2,m_FuncInfo[iStep].dPara3);    //Axis , StartSpeed , Acc , Speed , Jerk

            default            : break ;
        }
        m_pMsgFunc(AnsiString(iStep) + "_Line Finished");
//        m_FuncInfo[iStep].iTime = 0 ;  //For 있으면...안되겟네...
        if(m_FuncInfo[iStep].dReMainTime) m_FuncInfo[iStep].dReMainTime -= m_FuncInfo[iStep].dTime ; 
        else                              m_FuncInfo[iStep].dTime       -= m_FuncInfo[iStep].dTime ;
        iStep++ ;

        if(iStep <  m_iMaxLines){

            TempTimer.Clear();
        }
        else {
            CurrDateTime = Now();
//            sSeqEndTime  = CurrDateTime.FormatString("(yy.mm.dd)hh:nn:ss");
            sSeqEndTime  = CurrDateTime.FormatString("hh:nn:ss");
            m_pMsgFunc("SEQ Finished");
            iStep = 0 ;
            Suspend() ;
        }
        Sleep(1);
    }
}


void CSEQ::DisplayFunc ( TStringGrid * _slValue)
{
    if(!_slValue || !m_FuncInfo) return  ;

    _slValue -> RowCount = m_iMaxLines ;
    _slValue -> ColCount = 6 ;

    _slValue -> ColWidths[0] = 100 ;


    for(int i = 0 ; i <  m_iMaxLines ; i++) {

        _slValue -> Cells[0][i] = GetFuncNameString(m_FuncInfo[i].iFunc) ;
       /*switch(m_FuncInfo[i].iFunc) {
            case fcCHECKFINISH : _slValue -> Cells[0][i] = "CHECKFINISH"  ; break ;
            case fcMOVE0RPM    : _slValue -> Cells[0][i] = "MOVE0RPM   "  ; break ;
            case fcMOVEHOME    : _slValue -> Cells[0][i] = "MOVEHOME   "  ; break ;
            case fcMOVEABS     : _slValue -> Cells[0][i] = "MOVEABS    "  ; break ;
            case fcMOVEINC     : _slValue -> Cells[0][i] = "MOVEINC    "  ; break ;
            case fcCAMERA      : _slValue -> Cells[0][i] = "CAMERA     "  ; break ;
            case fcLASER       : _slValue -> Cells[0][i] = "LASER      "  ; break ;
            case fcDELAY       : _slValue -> Cells[0][i] = "DELAY      "  ; break ;
            case fcCOMMENT     : _slValue -> Cells[0][i] = "COMMENT    "  ; break ;
            case fcEMPTY       : _slValue -> Cells[0][i] = "EMPTY      "  ; break ;
            case fcSERVO0      : _slValue -> Cells[0][i] = "SERVO0     "  ; break ;
            case fcSHAKE0      : _slValue -> Cells[0][i] = "SHAKE0     "  ; break ;

        }*/
        _slValue -> Cells[1][i] = m_FuncInfo[i].dPara1 ;
        _slValue -> Cells[2][i] = m_FuncInfo[i].dPara2 ;
        _slValue -> Cells[3][i] = m_FuncInfo[i].dPara3 ;
        _slValue -> Cells[4][i] = m_FuncInfo[i].dPara4 ;
        _slValue -> Cells[5][i] = m_FuncInfo[i].dPara5 ;
    }
}

AnsiString CSEQ::GetFuncNameString(EN_FUNC iFunc)
{
     AnsiString sRet ;
     switch(iFunc) {
         case fcCHECKFINISH : sRet = "CHECKFINISH  "  ; break ;
         case fcMOVE0RPM    : sRet = "MOVE0RPM     "  ; break ;
         case fcMOVEHOME    : sRet = "MOVEHOME     "  ; break ;
         case fcMOVEABS     : sRet = "MOVEABS      "  ; break ;
         case fcMOVEINC     : sRet = "MOVEINC      "  ; break ;
         case fcCAMERA      : sRet = "CAMERA       "  ; break ;
         case fcLASER       : sRet = "LASER        "  ; break ;
         case fcDELAY       : sRet = "DELAY        "  ; break ;
         case fcCOMMENT     : sRet = "COMMENT      "  ; break ;
         case fcEMPTY       : sRet = "EMPTY        "  ; break ;
         case fcSERVO0      : sRet = "SERVO0       "  ; break ;
         case fcSHAKE0      : sRet = "SHAKE0       "  ; break ;
         case fcOR_SETSPEED0: sRet = "OR_SETSPEED0 "  ; break ;
         case fcOR_SETTIME0 : sRet = "OR_SETTIME0  "  ; break ;
         case fcOR_SETSTART0: sRet = "OR_SETSTART0 "  ; break ;
         case fcCOOLING     : sRet = "COOLING      "  ; break ;
         case fcCAMLIVE     : sRet = "CAMLIVE      "  ; break ;
         case fcSETHEAT     : sRet = "SETHEAT      "  ; break ;
         case fcFORSTART    : sRet = "FORSTART     "  ; break ;
         case fcFOREND      : sRet = "FOREND       "  ; break ;
         default            : sRet = ""               ; break ;
     }

     return sRet.Trim() ;
}
