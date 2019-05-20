//---------------------------------------------------------------------------
#pragma hdrstop
#include "SEQUnit.h"
#include "PaixMotion.h"
#include "BarslerUnit.h"
#include "Timer.h"
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
}

__fastcall CSEQ::~CSEQ(void)
{

    if(m_slSeq) delete m_slSeq ;

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
    else if (_sLine.Pos   ("OR_SETSTART"   ) == 1) { _fcName = fcOR_SETSTART   ; _sLine.Delete(1,AnsiString("OR_SETSTART"   ).Length()) ; return true ; }
    else                                        {                           return false ; }
}

bool CSEQ::GetPara(AnsiString &_sLine , double &_dPara)
{
    AnsiString sTemp ;

    if( _sLine.Pos("(")==1) _sLine.Delete(1,1) ;


         if(_sLine.Pos(",")){sTemp  = _sLine.SubString(1,_sLine.Pos(",")-1) ; _sLine.Delete(1,_sLine.Pos(",")-1) ;}
    else if(_sLine.Pos(")")){sTemp  = _sLine.SubString(1,_sLine.Pos(")")-1) ; _sLine.Delete(1,_sLine.Pos(")")-1) ;}

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

    m_sErrName = "No Error." ;

    m_iMaxLines = _slSourse -> Count ;

    if(!m_FuncInfo) delete m_FuncInfo ;
    m_FuncInfo = new TFuncInfo[m_iMaxLines] ;
    memset(m_FuncInfo , 0 , sizeof(TFuncInfo)*m_iMaxLines);

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
            case fcCHECKFINISH   : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;

            case fcMOVE0RPM      : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터2의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara2 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터3의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara3 = dPara ;break ;

            case fcMOVEHOME      : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;

            case fcMOVEABS       : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터2의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara2 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터3의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara3 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터4의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara4 = dPara ;break ;

            case fcMOVEINC       : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터2의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara2 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터3의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara3 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터4의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara4 = dPara ;break ;

            case fcLASER         : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;

            case fcDELAY         : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;

            case fcCOMMENT       :                                                                                                                                                      break ;

            case fcEMPTY         :                                                                                                                                                      break ;


            case fcCAMERA        : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;
            case fcSERVO0        : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;
            case fcSHAKE0        : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터2의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara2 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터3의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara3 = dPara ;
                                   if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터4의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara4 = dPara ;break ;

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
                                   break ;

            case fcOR_SETSTART   : if(!GetPara(sTemp , dPara)) {m_sErrName = "Line " + AnsiString(i) + "파라미터1의 형태가 이상합니다." ; return false ;} m_FuncInfo[i].dPara1 = dPara ;break ;


            break ;
        }

        if(sTemp.Length() && iFunc != fcCOMMENT ) {
            m_sErrName = "Line " + AnsiString(i) + "파라미터가 많거나 알수 없는 문자가 있습니다." ;
            return false ;
        }
    }

    return true ;


}


//---------------------------------------------------------------------------
void __fastcall CSEQ::Execute()
{
    static int  iStep    = 0     ;
    static int  iPreStep = 0     ;
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
        else if(m_FuncInfo[iStep].iFunc == fcDELAY      ) iTimeOutMs = 30000   ;
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
            pGrabRun -> MakeAviThread() ;
            pPaix -> SetServoOnOff(Axis1 , true) ; //servooff.
            m_bSeqInit=false ;
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
                                       pPaix -> SetSpeedDist (m_FuncInfo[iStep].dPara1,40,m_FuncInfo[iStep].dPara2, m_FuncInfo[iStep].dPara3,0 );    //StartSpeed , Acc , Speed , Jerk
                                       pPaix -> AbsMove      (m_FuncInfo[iStep].dPara1,m_FuncInfo[iStep].dPara4 ) ;
                                   break ;

            case fcMOVEINC       : //if(m_FuncInfo[iStep].dPara1 == 0) pPaix -> RotateReset(Axis1) ;
                                       pPaix -> SetSpeedDist (m_FuncInfo[iStep].dPara1,40,m_FuncInfo[iStep].dPara2, m_FuncInfo[iStep].dPara3,0 );    //StartSpeed , Acc , Speed , Jerk
                                       pPaix -> RelMove      (m_FuncInfo[iStep].dPara1,m_FuncInfo[iStep].dPara4 ) ;
                                   break ;

            case fcLASER         : if(m_FuncInfo[iStep].dPara1 == 0)
                                        pPaix->SetOutputBit(2,1);
                                   else
                                        pPaix->SetOutputBit(2,0);

                                   break ;

            case fcDELAY         : if(!TempTimer.OnDelay(true,m_FuncInfo[iStep].dPara1)) continue;
                                   break ;

            case fcCOMMENT       : //할일 없음.
                                   break ;

            case fcEMPTY         : //할일 없음.
                                   break ;

            case fcCAMERA        : if(m_FuncInfo[iStep].dPara1 == 0) {
                                       pGrabRun->StopGrab();
                                       pGrabRun->MakeAviThread();              //사진찍은거 동영상 제작 시작.  pGrabRun->IsMakingAvi() 요놈이 동영상다만들때 까지 리턴 트루임.
                                   }
                                   else {
                                       if(pGrabRun->IsMakingAvi())continue ;   //전에 사진찍은게 아직 동영상이 안만들어 졌으면 기달림.
                                       pGrabRun->ContinuousShot();
                                   }
                                   break ;
            case fcSERVO0        : pPaix-> SetServoOnOff(0,!m_FuncInfo[iStep].dPara1) ;
                                   break ;

            case fcSHAKE0        : pPaix -> SetSpeedDist (0,40,m_FuncInfo[iStep].dPara2, m_FuncInfo[iStep].dPara3,0 );    //Axis , StartSpeed , Acc , Speed , Jerk
                                   pPaix -> MoveShake(0,m_FuncInfo[iStep].dPara1 , m_FuncInfo[iStep].dPara4);
                                   break ;
            case fcOR_SETSPEED0  : //할일 없음.
                                   break ;

            case fcOR_SETTIME0   : //할일 없음.
                                   break ;

            case fcOR_SETSTART   : pPaix -> OR_Start(m_iMaxTime,m_iMaxVal,m_dORSpeed,m_dORTime);
                                   break ;



//            case fcSETDRIVESPD0: pPaix -> SetDriveSpeed(0,m_FuncInfo[iStep].dPara1,m_FuncInfo[iStep].dPara2,m_FuncInfo[iStep].dPara3);    //Axis , StartSpeed , Acc , Speed , Jerk

            default            : break ;
        }
        m_pMsgFunc(AnsiString(iStep) + "_Line Finished");
        iStep++ ;
        
        if(iStep <  m_iMaxLines){

            TempTimer.Clear();
        }
        else {
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
         case fcOR_SETSTART : sRet = "OR_SETSTART  "  ; break ;
         default            : sRet = ""               ; break ;
     }

     return sRet.Trim() ;
}




