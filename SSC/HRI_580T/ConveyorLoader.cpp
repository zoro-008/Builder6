//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ConveyorLoader.h"
#include "Rail.h"

#include "ErrProc.h"
#include "FileManager.h"
#include "Function.h"
#include "PositionMap.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
TConveyorLoader  VL;
const AnsiString sPartName = "Conveyor Loader " ;

/***************************************************************************/
/* 생성자 & 소멸자. (Constructor & Destructor)                             */
/***************************************************************************/
//---------------------------------------------------------------------------
__fastcall TConveyorLoader::TConveyorLoader(void)
{
    //Init.
    Init();
}

//---------------------------------------------------------------------------
__fastcall TConveyorLoader::~TConveyorLoader(void)
{

}

/***************************************************************************/
/* Init.                                                                   */
/***************************************************************************/
//---------------------------------------------------------------------------
void __fastcall TConveyorLoader::Init(void) //장비 시작시에 한번만 타는 함수. 초기화.
{

    //Init. Var.
    m_bWorkEnd      = false; //Work End Flag.
    m_bToStart      = false; //To... Flag.
    m_bToStop       = false;
    m_bReqStop      = false;

    m_iSeqStep      = 0    ; //Step.
	m_iCycleStep    = 0    ;
    m_iPreCycleStep = 0    ;
    m_iHomeStep     = 0    ;

    //Clear Timer.
    m_MainCyleTimer.Clear();
    m_OneCyleTimer .Clear();
    m_HomeTimer    .Clear();
    m_ToStopTimer  .Clear();
    m_ToStartTimer .Clear();
    m_TempTimer    .Clear();

    sPreMsg = "";
    sMsg    = "";

    //Init. Buffers
    memset(&SORT , 0 , sizeof(SORT));
    memset(&Aply , 0 , sizeof(Aply));
	memset(&Rqst , 0 , sizeof(Rqst));
	memset(&Stat , 0 , sizeof(Stat));

}

//---------------------------------------------------------------------------
void __fastcall TConveyorLoader::Reset(void)  //장비 리셑 버튼 눌러주는 상황 같은 상황 같은 상황버섯.
{
    //Init. Var.
	m_bToStart      = false; //To... Flag.
    m_bToStop       = false;
    m_bReqStop      = false;

    m_iSeqStep      = 0    ; //Step.
    m_iCycleStep    = 0    ;
    m_iPreCycleStep = 0    ;
    m_iHomeStep     = 0    ;

    //Clear Timer.
    m_HomeTimer   .Clear();
    m_ToStopTimer .Clear();
    m_ToStartTimer.Clear();
    m_OneCyleTimer.Clear();

	memset(&Aply , 0 , sizeof(Aply));
	memset(&Rqst , 0 , sizeof(Rqst));
	memset(&Stat , 0 , sizeof(Stat));
}

//---------------------------------------------------------------------------
SORT_INFO __fastcall TConveyorLoader::GetSlotInfo(EN_ARAY_ID Aray , EN_FIND_MODE FindMode)
{
	//Local Var.
	SORT_INFO Sort;

	DM.InitSortInfo(Sort);

	Sort.iSndr = Aray;

	if(DM.ARAY[Aray].FindFrstRowCol (FindMode , Sort.iFindRow , Sort.iFindCol)) Sort.FLG_Find = true;

	//Ok.
	return Sort;
}
//---------------------------------------------------------------------------
/***************************************************************************/
/* Direct Accessable Functions1.                                           */
/***************************************************************************/
//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::MoveHome(void)
{

    //Local Var.
	bool r1 , r2;

    //Time Out.
	if (m_HomeTimer.OnDelay(m_iHomeStep > 10 , 30000)) ERR.SetErr(eVLDHomeTO);

		//Move Home.
    switch (m_iHomeStep) {
        case 10: ERR.SetErr(eVLD_ZHoming , true);
                 MOTR       .ClearHomeEnd(miVLD_Z);
                 MOTR       .ClearHomeEnd(miVLD_Y);
                 m_HomeTimer.Clear        (     );
                 MoveActr(aiVLP_FB,ccBwd);
                 m_iHomeStep++;
                 return false ;

        case 11: if(!MoveActr(aiVLP_FB,ccBwd))return false ;
                 MoveMotr (miVLD_Y , mcHome) ;
                 m_iHomeStep++;
                 return false ;

        case 12: if (!MOTR.CAMC[miVLD_Y].GetHomeEnd()) return false ;
                 MoveMotr (miVLD_Z , mcHome) ;
                 m_iHomeStep++;
                 return false ;

        case 13: if (!MOTR.CAMC[miVLD_Z].GetHomeEnd()) return false ;

                 MoveMotr(miVLD_Y , mcPosition , mpCPWait);
                 m_iHomeStep++ ;
                 return false ;

        case 14: if (!MoveMotr(miVLD_Y , mcPosition , mpCPWait))return false ;
                 MoveMotr(miVLD_Z , mcPosition , mpCPWait);
                 m_iHomeStep++ ;
                 return false ;

        case 15: if (!MoveMotr(miVLD_Z , mcPosition , mpCPWait))return false ;
                 ToStartCon() ;
                 m_iHomeStep++;
                 return false;

        case 16: if (!ToStart()) return false;
                 ERR.SetErr(eVLD_ZHoming , false);
                 m_iHomeStep = 0;
                 return true ;
    }

	//Ok.
	return true;

}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::MoveToLastWorkPosn(void)
{
		return false;
}

/***************************************************************************/
/* Direct Accessable Functions2.                                           */
/***************************************************************************/
double __fastcall TConveyorLoader::GetMotrPos(EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId)
{
		double dPosition = 0.0 ;

		if(Motr == miVLD_Z)
		{
			if (Cmd == mcOneStepF) dPosition =  FM.DevInfo.dCsSlotPitch ;
			if (Cmd == mcOneStepB) dPosition = -FM.DevInfo.dCsSlotPitch ;
			if (Cmd == mcPosition)
			{
				   	 if(PstnId == mpCPWait       ) dPosition = MOTR.CAMC[Motr].dPosn[msCPWaitPs       ] ;
				else if(PstnId == mpCPHome       ) dPosition = 0.0                                      ;
				else if(PstnId == mpVLD_ZPickFwd ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_ZPickFwdPs ] ;
				else if(PstnId == mpVLD_ZClampOn ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_ZClampOnPs ] ;
				else if(PstnId == mpVLD_ZPickBwd ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_ZPickBwdPs ] ;
				else if(PstnId == mpVLD_ZWorkStt ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_ZWorkSttPs ] ;
				else if(PstnId == mpVLD_ZPlceFwd ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_ZPlceFwdPs ] ;
				else if(PstnId == mpVLD_ZClampOff) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_ZClampOffPs] ;
				else if(PstnId == mpVLD_ZPlceBwd ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_ZPlceBwdPs ] ;

			}

			if (Cmd == mcFindStep)
			{
				SORT_INFO SORT = GetSlotInfo(riVLD , fmUnkwn) ;

				if(PstnId == mpVLD_ZWork    ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_ZWorkSttPs] - SORT.iFindRow * FM.DevInfo.dCsSlotPitch  ;

			}
		}

		else if(Motr == miVLD_Y)
		{
			if (Cmd == mcPosition)
			{
					 if(PstnId == mpCPWait       ) dPosition = MOTR.CAMC[Motr].dPosn[msCPWaitPs       ] ;
				else if(PstnId == mpCPHome       ) dPosition = 0.0                                      ;
				else if(PstnId == mpVLD_YPick    ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_YPickPs    ] ;
				else if(PstnId == mpVLD_YWork    ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_YWorkPs    ] ;
				else if(PstnId == mpVLD_YPlce    ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_YPlcePs    ] ;

			}
		}

		return dPosition ;
}
//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::CheckDstb(EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId , double dPstn)
{
    if(m_iHomeStep) return true ;

    //Local Var.
    bool   bDspMsg         = !SEQ._bRun && (SEQ._iSeqStat != ssInit) ;

    bool   isFwPusher      =  ACTR.Complete (aiVLP_FB , ccFwd       ) ;
    bool   isRailGateCnsr  =  XV[xLDInCheck] ;
    bool   isVLD_YWorkZone =  MOTR.CmprPos(miVLD_Y , mpVLD_YWork) ;
    bool   isVLD_ZSplyZone =  MOTR.CmprPos(miVLD_Z , mpVLD_ZPickFwd) || MOTR.CmprPos(miVLD_Z , mpVLD_ZClampOn ) || MOTR.CmprPos(miVLD_Z , mpVLD_ZPickBwd) ;
    bool   isVLD_ZStckZone =  MOTR.CmprPos(miVLD_Z , mpVLD_ZPlceFwd) || MOTR.CmprPos(miVLD_Z , mpVLD_ZClampOff) || MOTR.CmprPos(miVLD_Z , mpVLD_ZPlceBwd) ;
    bool   isVLD_ZWorkZone =  GetMotrPos(miVLD_Z , mcPosition , mpVLD_ZWorkStt)- (FM.DevInfo.iCsMaxSlot-1) * FM.DevInfo.dCsSlotPitch <= MOTR.GetCmdPos(miVLD_Z) &&
                              MOTR.GetCmdPos(miVLD_Z) <= GetMotrPos(miVLD_Z , mcPosition , mpVLD_ZWorkStt);
    bool   isVLD_YCmdNeg   =  Motr == miVLD_Y && Cmd == mcPosition && MOTR.GetEncPos(Motr) > GetMotrPos(Motr , Cmd , PstnId ) ;
    bool   isVLD_YCmdWorkPs=  Motr == miVLD_Y && Cmd == mcPosition && PstnId == mpVLD_YWork ;
    bool   isClampSplyZone = !isVLD_YWorkZone && isVLD_ZSplyZone ;

    bool   isClampStckZone = !isVLD_YWorkZone && isVLD_ZStckZone ;
    bool   isZGotoStckZone = (Motr == miVLD_Z) && (PstnId == mpVLD_ZClampOff || PstnId == mpVLD_ZPlceBwd || PstnId == mpVLD_ZPlceFwd) ;
    bool   isZGotoSplyZone = (Motr == miVLD_Z) && (PstnId == mpVLD_ZClampOn  || PstnId == mpVLD_ZPickBwd || PstnId == mpVLD_ZPickFwd) ;

    bool   isYJogMove      =  (dPstn && abs(MOTR.GetCmdPos(miVLD_Y)) - dPstn < 10.0) || Cmd == mcJogN || Cmd == mcJogP ;


    //Check Motor Alarm & HomeEnd.
    if (MOTR.CAMC[Motr].GetAlarm()                                ) { if (bDspMsg) UserMsg2("Error" , sMotrName[Motr] + " 모터가  ALARM 상태에 있어 모터를 이동 할 수 없습니다." ); return false; }
    if (MOTR.CAMC[Motr].GetCW   () && MOTR.CAMC[Motr].GetHomeEnd()) { if (bDspMsg) UserMsg2("Error" , sMotrName[Motr] + " 모터가 +LIMIT 상태에 있어 모터를 이동 할 수 없습니다." ); return false; }
    if (MOTR.CAMC[Motr].GetCCW  () && MOTR.CAMC[Motr].GetHomeEnd()) { if (bDspMsg) UserMsg2("Error" , sMotrName[Motr] + " 모터가 -LIMIT 상태에 있어 모터를 이동 할 수 없습니다." ); return false; }

    if(!MOTR.CheckDstb(Motr)) {if (bDspMsg) UserMsg2("Error" , sMotrName[Motr] + " 기구 혹은 모터 간섭으로 인해 움직일수 없습니다." ); return false;}

    if(!MOTR.CAMC[Motr].GetHomeEnd() && mcHome != Cmd) {if (bDspMsg) UserMsg2("Error" , sMotrName[Motr] + " HomeEnd가 되지 않아 움직일수 없습니다." ); return false;}

    //Checking.
    if (Motr == miVLD_Z)
    {
        if(isFwPusher)
        {
            MOTR.Stop(Motr);
            if (bDspMsg) UserMsg2("Error" , sActrName[aiVLP_FB]+"가 이동중 입니다." );
            return false;
        }
        if(isRailGateCnsr)
        {
            MOTR.Stop(Motr);
            if (bDspMsg) UserMsg2("Error" , "스트립이 공급중이라 모터를 이동 할 수 없습니다." );
            return false ;
        }
        if(isClampStckZone && isZGotoSplyZone)
        {
            MOTR.Stop(Motr);
            if (bDspMsg) UserMsg2("Error" , sMotrName[Motr] + "기구 간섭에 의해 움직일수 없습니다.");
            return false ;
        }
        if(isClampSplyZone && isZGotoStckZone)
        {
            MOTR.Stop(Motr);
            if (bDspMsg) UserMsg2("Error" , sMotrName[Motr] + "기구 간섭에 의해 움직일수 없습니다.");
            return false ;
        }
    }

    else if (Motr == miVLD_Y)
    {
        if(isVLD_ZWorkZone && !isYJogMove && !isVLD_YCmdNeg && !isVLD_YCmdWorkPs)
        {
            MOTR.Stop(Motr);
            if (bDspMsg) UserMsg2("Error" , sMotrName[miVLD_Z] + "이 작업위치에 있어서 움직일수 없습니다.");
            return false ;
        }
    }


    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::MoveMotr(EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId , double dPstn)
{
    //Check Command. (Stop은 간섭을 확인 하지 않음)
    if (Cmd == mcStop  ) return MOTR.Stop    (Motr);
    if (Cmd == mcEStop ) return MOTR.EmrgStop(Motr);
    if (Cmd == mcJogP  ) return MOTR.MoveJog (Motr , true);
    if (Cmd == mcJogN  ) return MOTR.MoveJog (Motr , true);

    if (!CheckDstb(Motr , Cmd , PstnId , dPstn )) return false;

    double dPosition ;

    if(Motr == miVLD_Z)
    {
        if (Cmd == mcHome    ) return MOTR.MoveHome (Motr                                         ) ;
        if (Cmd == mcOneStepF) return MOTR.MoveMotr (Motr , GetMotrPos(Motr , Cmd , PstnId) , true) ;
        if (Cmd == mcOneStepB) return MOTR.MoveMotr (Motr , GetMotrPos(Motr , Cmd , PstnId) , true) ;
        if (Cmd == mcDirect  ) return MOTR.MoveMotr (Motr , dPstn                                 ) ;
        if (Cmd == mcPosition) return MOTR.MoveMotr (Motr , GetMotrPos(Motr , Cmd , PstnId)       ) ;
        if (Cmd == mcFindStep) return MOTR.MoveMotr (Motr , GetMotrPos(Motr , Cmd , PstnId)       ) ;
    }
    if(Motr == miVLD_Y)
    {
        if (Cmd == mcHome    ) return MOTR.MoveHome (Motr                                         ) ;
        if (Cmd == mcOneStepF) return MOTR.MoveMotr (Motr , GetMotrPos(Motr , Cmd , PstnId) , true) ;
        if (Cmd == mcOneStepB) return MOTR.MoveMotr (Motr , GetMotrPos(Motr , Cmd , PstnId) , true) ;
        if (Cmd == mcDirect  ) return MOTR.MoveMotr (Motr , dPstn                                 ) ;
        if (Cmd == mcPosition) return MOTR.MoveMotr (Motr , GetMotrPos(Motr , Cmd , PstnId)       ) ;
        if (Cmd == mcFindStep) return MOTR.MoveMotr (Motr , GetMotrPos(Motr , Cmd , PstnId)       ) ;
    }

    return false ;
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::MoveActr (EN_ACTR_ID Actr , bool On)
{
    return ACTR.MoveCyl(Actr , On );
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::ToStartCon(void)
{
    //Clear Timer.
    m_bToStart = false    ;
    m_ToStartTimer.Clear();

    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::ToStopCon(void)
{
    //Clear Timer.
    m_bToStop  = false   ;
    m_bReqStop = true    ;
    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
    if (m_iSeqStep) return false;

    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::ToStart(void)
{
    //Check Time Out.
    if (m_bToStart                                                             ) return true;
    if (m_ToStartTimer.OnDelay(!m_bToStart && (SEQ._iSeqStat != ssInit) , 5000)) { ERR.SetErr(eVLDStartTO); return false; }

    //Init.
    m_iSeqStep = 0    ; //Clear Step.
    m_bToStart = true ;

    //Clear Timer.
    m_MainCyleTimer.Clear(); //Clear Main  Cycle Timer.
    m_OneCyleTimer .Clear();

    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::ToStop(void)
{
    //Check Time Out.
    if (m_bToStop                               ) return true;
    if (m_ToStopTimer.OnDelay(!m_bToStop , 5000)) { ERR.SetErr(eVLDStopTO); return false; }

    YV[yVlSplyBeltOn] = false ;

    if(!MoveActr(aiVLP_FB , ccBwd                ))return false ;

    //Init.
    m_iSeqStep = 0   ; //Clear Step.
    m_bToStop  = true;

    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::AutoRun(void)
{
    //Check Cycle Time Out.
    m_MainCyleTimer.OnDelay((m_iSeqStep != 0) && !FM.MstOptn.bDebugMode && !ERR._bHasErr , 20000);

    if (ERR.SetErr( eVLDRunTO , m_MainCyleTimer.Out)){
      Temp = sPartName + Temp.sprintf( "MAIN CYCLE TIMEOUT STATUS : m_iSeqStep=%03d" , m_iSeqStep );
      Trace(sPartName.c_str(),Temp.c_str());
      Reset();
      return false;
    }

    //Check Error & Decide Step.
    if (m_iSeqStep == 0)
    {
      if (m_bReqStop)return false ;

      //모르는 카세트에러.
      ERR.SetErr(eVLClmpUnkwnCast  , DM.ARAY[riVLD].IsAllNone() &&  XV[xVlClmpCstChck] && !FM.ComOptn.bIdleRun && !FM.ComOptn.bManualRun) ;

      //카세트 사라짐.
      ERR.SetErr(eVLClmpDisaprCast ,!DM.ARAY[riVLD].IsAllNone() && !XV[xVlClmpCstChck] && !FM.ComOptn.bIdleRun && !FM.ComOptn.bManualRun) ;

      //카세트 사라짐.
      ERR.SetErr(eVLCastFull       , XV[xVlStckCstFull] && !FM.ComOptn.bManualRun) ;

      if(ERR._bHasErr) return false ;

      //Vars.
      bool isZPushPos = MOTR.CmprPos(miVLD_Z ,GetMotrPos(miVLD_Z , mcFindStep , mpVLD_ZWork)) ;

      //Step Condition Flag.
      bool isConMove = DM.ARAY[riVLD].IsAllNone  () && !XV[xVlSplyCstChck] && !Stat.bChckSplyEmpty ;
      bool isConSply = DM.ARAY[riVLD].IsAllNone  () && (XV[xVlSplyCstChck] || FM.ComOptn.bIdleRun) ;
      bool isConStep = DM.ARAY[riVLD].GetCntUnkwn() && !isZPushPos         ;
      bool isConPush = DM.ARAY[riVLD].GetCntUnkwn() &&  isZPushPos         && RL.Rqst.bCLDStep;
      bool isConStck = DM.ARAY[riVLD].IsAllEmpty () && !XV[xVlStckCstFull] ;
      bool isConEnd  = DM.ARAY[riVLD].IsAllNone  () && !XV[xVlSplyCstChck] && Stat.bChckSplyEmpty  ;

      //Sequence Stop Flag.
      if (SEQ._bLtStop) return false;

      //Decide Stpe.
           if (isConMove ) {Trace(sPartName.c_str(),"CycleMove Stt"); m_iSeqStep =  10 ; m_iCycleStep = 10 ; }
      else if (isConSply ) {Trace(sPartName.c_str(),"CycleSply Stt"); m_iSeqStep =  20 ; m_iCycleStep = 10 ; }
      else if (isConStep ) {Trace(sPartName.c_str(),"CycleStep Stt"); m_iSeqStep =  30 ; m_iCycleStep = 10 ; }
      else if (isConPush ) {Trace(sPartName.c_str(),"CyclePush Stt"); m_iSeqStep =  40 ; m_iCycleStep = 10 ; }
      else if (isConStck ) {Trace(sPartName.c_str(),"CycleStck Stt"); m_iSeqStep =  50 ; m_iCycleStep = 10 ; }
      else if (isConEnd  ) {m_bWorkEnd = true; return true       ; }
      m_bWorkEnd = false ;
    }

    //Cycle.
    switch (m_iSeqStep)
    {
      case   0:                                                          m_iSeqStep = 0 ;  return false ;
      case  10: if(CycleMove()){Trace(sPartName.c_str(),"CycleMove End");m_iSeqStep = 0 ;} return false ;
      case  20: if(CycleSply()){Trace(sPartName.c_str(),"CycleSply End");m_iSeqStep = 0 ;} return false ;
      case  30: if(CycleStep()){Trace(sPartName.c_str(),"CycleStep End");m_iSeqStep = 0 ;} return false ;
      case  40: if(CyclePush()){Trace(sPartName.c_str(),"CyclePush End");m_iSeqStep = 0 ;} return false ;
      case  50: if(CycleStck()){Trace(sPartName.c_str(),"CycleStck End");m_iSeqStep = 0 ;} return false ;

    }

    return false ;
}

//---------------------------------------------------------------------------



bool __fastcall TConveyorLoader::CycleMove(void) //컨베이어 벨트를 구동하여 카세트를 옮기는 함수.
{

    Temp = sPartName + Temp.sprintf("MOVE ONE CYCLE : m_iCycleStep=%03d" , m_iCycleStep );
    if(FM.MstOptn.bDebugMode && Temp != sMsg)
    {
        Trace(sPartName.c_str(),Temp.c_str());
        sMsg = Temp ;
    }

    //Check Cycle Time Out.
    if (ERR.SetErr(eVLDCycleTO , m_OneCyleTimer.OnDelay(m_iCycleStep == m_iPreCycleStep  && !FM.MstOptn.bDebugMode && !SEQ._bStepRun, 5000)))
    {
        Temp = sPartName + Temp.sprintf("MOVE ONE CYCLE TIMEOUT STATUS : m_iCycleStep=%03d" , m_iCycleStep );
        Trace(sPartName.c_str(),Temp.c_str());
        m_iCycleStep = 0 ;
        return true;
    }

    m_iPreCycleStep = m_iCycleStep ;

    if(m_bReqStop)
    {
        m_iCycleStep = 0;
        return true ;
    }

    //Cycle.
    switch (m_iCycleStep)
    {
        case   0: m_iCycleStep = 0;
              return true;

        case  10: YV[yVlSplyBeltOn] = true ;
              m_TempTimer.Clear();
              m_iCycleStep++;
              return false;

        case  11: if(!m_TempTimer.OnDelay(true , 4000)&&!XV[xVlSplyCstChck] ) return false ;
              if(!XV[xVlSplyCstChck])Stat.bChckSplyEmpty = true ;
              YV[yVlSplyBeltOn] = false ;
              m_iCycleStep = 0 ;
              return true ;

    }

    return false;
}




bool __fastcall TConveyorLoader::CycleSply(void)//클렘프로 서플라이존에 있는 카세트를 찝어 오는 함수.
{
    Temp = sPartName + Temp.sprintf("SPLY ONE CYCLE : m_iCycleStep=%03d" , m_iCycleStep );
    if(FM.MstOptn.bDebugMode && Temp != sMsg)
    {
        Trace(sPartName.c_str(),Temp.c_str());
        sMsg = Temp ;
    }

    //Check Cycle Time Out.
    if (ERR.SetErr(eVLDCycleTO , m_OneCyleTimer.OnDelay(m_iCycleStep == m_iPreCycleStep  && !FM.MstOptn.bDebugMode && !SEQ._bStepRun, 3000)))
    {
        Temp = sPartName + Temp.sprintf("SPLY ONE CYCLE TIMEOUT STATUS : m_iCycleStep=%03d" , m_iCycleStep );
        Trace(sPartName.c_str(),Temp.c_str());
        m_iCycleStep = 0 ;
        return true;
    }

    m_iPreCycleStep = m_iCycleStep ;

    if(m_bReqStop)
    {
        if(m_iCycleStep != 15 && m_iCycleStep != 16) {m_iCycleStep = 0; return true ;}
    }

    //Cycle.
    switch (m_iCycleStep)
    {
        case   0: m_iCycleStep = 0;
                  return true;

        case  10: MoveActr(aiVLC_UD , ccFwd);
                  MoveMotr(miVLD_Y , mcPosition , mpVLD_YWork) ;
                  YV[yVlSplyBeltOn] = true ;
                  m_iCycleStep++;
                  return false;

        case  11: if(!MoveActr(aiVLC_UD , ccFwd                   )) return false ;
                  if(!MoveMotr(miVLD_Y  , mcPosition , mpVLD_YWork)) return false ;
                  MoveMotr(miVLD_Z , mcPosition , mpVLD_ZPickFwd);
                  m_iCycleStep++;
                  return false;

        case  12: if(!MoveMotr(miVLD_Z , mcPosition , mpVLD_ZPickFwd))return false ;
                  MoveMotr(miVLD_Y , mcPosition , mpVLD_YPick) ;
                  m_iCycleStep++;
                  return false ;

        case  13: if(!MoveMotr(miVLD_Y , mcPosition , mpVLD_YPick   ))return false ;
                  if(!XV[xVlClmpCstChck] && !FM.ComOptn.bIdleRun     )return false ;
                  YV[yVlSplyBeltOn] = false ;
                  MoveMotr(miVLD_Z , mcPosition , mpVLD_ZClampOn ) ;
                  m_iCycleStep++ ;
                  return false ;

        case  14: if(!MoveMotr(miVLD_Z , mcPosition , mpVLD_ZClampOn)) return false ;
                  MoveActr(aiVLC_UD , ccBwd);
                  DM.ARAY[riVLD].SetStat(csUnkwn) ;
                  Stat.bChckSplyEmpty = false ;
                  m_iCycleStep++;
                  return false ;

        case  15: if(!MoveActr(aiVLC_UD , ccBwd)) return false ;
                  MoveMotr(miVLD_Z , mcPosition , mpVLD_ZPickBwd ) ;
                  m_iCycleStep++;
                  return false ;

        case  16: if(!MoveMotr(miVLD_Z , mcPosition , mpVLD_ZPickBwd )) return false ;
                  MoveMotr(miVLD_Y , mcPosition , mpVLD_YWork ) ;
                  m_iCycleStep++;
                  return false ;

        case  17: if(!MoveMotr(miVLD_Y , mcPosition , mpVLD_YWork )) return false ;
                  m_iCycleStep = 0 ;
                  return true ;

    }

    return false;
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::CycleStep(void) //Z축 - 카세트의 워크포지션으로 이동하는 함수.
{
    Temp = sPartName + Temp.sprintf("STEP ONE CYCLE : m_iCycleStep=%03d" , m_iCycleStep );
    if(FM.MstOptn.bDebugMode && Temp != sMsg)
    {
        Trace(sPartName.c_str(),Temp.c_str());
        sMsg = Temp ;
    }

    //Check Cycle Time Out.
    if (ERR.SetErr(eVLDCycleTO , m_OneCyleTimer.OnDelay(m_iCycleStep == m_iPreCycleStep  && !FM.MstOptn.bDebugMode && !SEQ._bStepRun, 3000)))
    {
        Temp = sPartName + Temp.sprintf("STEP ONE CYCLE TIMEOUT STATUS : m_iCycleStep=%03d" , m_iCycleStep );
        Trace(sPartName.c_str(),Temp.c_str());
        m_iCycleStep = 0 ;
        return true;
    }

    m_iPreCycleStep = m_iCycleStep ;

    if(m_bReqStop)
    {
        m_iCycleStep = 0;
        return true ;
    }

    //SORT_INFO Sort = GetSlotInfo(riVLD,fmUnkwn) ;  //DM.ARAY[iWorkCst].FindFrstRow(fmUnkwn);

    //Cycle.
    switch (m_iCycleStep)
    {
        case   0: m_iCycleStep = 0;
                  return true;

        case  10: MoveMotr(miVLD_Y , mcPosition , mpVLD_YWork) ;
                  m_iCycleStep++;
                  return false;

        case  11: if(!MoveMotr(miVLD_Y  , mcPosition , mpVLD_YWork)) return false ;
                  MoveMotr(miVLD_Z , mcFindStep , mpVLD_ZWork);
                  m_iCycleStep++;
                  return false;

        case  12: if(!MoveMotr(miVLD_Z , mcFindStep  , mpVLD_ZWork)) return false ;
                  m_iCycleStep = 0 ;
                  return true ;

    }

    return false;
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::CyclePush(void)  //로더 푸셔로 스트립을 푸쉬 하는 함수.
{

    Temp = sPartName + Temp.sprintf("PUSH ONE CYCLE : m_iCycleStep=%03d" , m_iCycleStep );
    if(FM.MstOptn.bDebugMode && Temp != sMsg)
    {
        Trace(sPartName.c_str(),Temp.c_str());
        sMsg = Temp ;
    }

    //Check Cycle Time Out.
    if (ERR.SetErr(eVLDCycleTO , m_OneCyleTimer.OnDelay(m_iCycleStep == m_iPreCycleStep  && !FM.MstOptn.bDebugMode && !SEQ._bStepRun, 5000)))
    {
        Temp = sPartName + Temp.sprintf("PUSH ONE CYCLE TIMEOUT STATUS : m_iCycleStep=%03d" , m_iCycleStep );
        Trace(sPartName.c_str(),Temp.c_str());
        m_iCycleStep = 0 ;
        return true;
    }

    m_iPreCycleStep = m_iCycleStep ;

    if(m_bReqStop)
    {
        if(m_iCycleStep != 13) {
            Aply.bRBZStep = false ;
            m_iCycleStep = 0;
            return true ;
        }
    }

    //Cycle.
    switch (m_iCycleStep)
    {
        case   0: m_iCycleStep = 0;
                  return true;

        case  10: MoveMotr(miVLD_Y , mcPosition , mpVLD_YWork) ;
                  m_iCycleStep++;
                  return false;

        case  11: if(!MoveMotr(miVLD_Y  , mcPosition , mpVLD_YWork)) return false ;
                  MoveMotr(miVLD_Z , mcFindStep , mpVLD_ZWork);
                  m_iCycleStep++;
                  return false;

        case  12: if(!MoveMotr(miVLD_Z , mcFindStep  , mpVLD_ZWork)) return false ;
                  Aply.bRBZStep = true ;                         //실린더 포워드 백워드는 레일에서 함.
                  m_iCycleStep++;
                  return false;

        case  13: if(RL.Rqst.bCLDStep)return false ;
                  Aply.bRBZStep = false ;
                  m_iCycleStep = 0 ;
                  return true ;


    }

    return false;
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::CycleStck(void)  //클램프에 있는 카세트를 스탁존에 내려 놓고 오는 함수.
{

    Temp = sPartName + Temp.sprintf("STCK ONE CYCLE : m_iCycleStep=%03d" , m_iCycleStep );
    if(FM.MstOptn.bDebugMode && Temp != sMsg)
    {
        Trace(sPartName.c_str(),Temp.c_str());
        sMsg = Temp ;
    }

    //Check Cycle Time Out.
    if (ERR.SetErr(eVLDCycleTO , m_OneCyleTimer.OnDelay(m_iCycleStep == m_iPreCycleStep  && !FM.MstOptn.bDebugMode && !SEQ._bStepRun, 3000)))
    {
        Temp = sPartName + Temp.sprintf("STCK ONE CYCLE TIMEOUT STATUS : m_iCycleStep=%03d" , m_iCycleStep );
        Trace(sPartName.c_str(),Temp.c_str());
        m_iCycleStep = 0 ;
        return true;
    }

    m_iPreCycleStep = m_iCycleStep ;

    if(m_bReqStop)
    {
        //m_iCycleStep = 0;
        //return true ;
    }

    //Cycle.
    switch (m_iCycleStep)
    {
        case   0: m_iCycleStep = 0;
                  return true;

        case  10: MoveMotr(miVLD_Y , mcPosition , mpVLD_YWork) ;
                  YV[yVlSplyBeltOn] = true ;
                  m_iCycleStep++;
                  return false;

        case  11: if(!MoveMotr(miVLD_Y  , mcPosition , mpVLD_YWork)) return false ;
                  MoveMotr(miVLD_Z , mcPosition , mpVLD_ZPlceFwd);
                  m_iCycleStep++;
                  return false;

        case  12: if(!MoveMotr(miVLD_Z , mcPosition , mpVLD_ZPlceFwd))return false ;
                  MoveMotr(miVLD_Y , mcPosition , mpVLD_YPlce) ;
                  m_iCycleStep++;
                  return false ;

        case  13: if(!MoveMotr(miVLD_Y , mcPosition , mpVLD_YPlce   ))return false ;
                  MoveMotr(miVLD_Z , mcPosition , mpVLD_ZClampOff ) ;
                  m_iCycleStep++ ;
                  return false ;

        case  14: if(!MoveMotr(miVLD_Z , mcPosition , mpVLD_ZClampOff)) return false ;
                  MoveActr(aiVLC_UD , ccFwd);
                  DM.ARAY[riVLD].SetStat(csNone) ;
                  m_iCycleStep++;
                  return false ;

        case  15: if(!MoveActr(aiVLC_UD , ccFwd)) return false ;
                  MoveMotr(miVLD_Z , mcPosition , mpVLD_ZPlceBwd ) ;
                  m_iCycleStep++;
                  return false ;

        case  16: if(!MoveMotr(miVLD_Z , mcPosition , mpVLD_ZPlceBwd )) return false ;
                  MoveMotr(miVLD_Y , mcPosition , mpVLD_YWork ) ;
                  m_iCycleStep++;
                  return false ;

        case  17: if(!MoveMotr(miVLD_Y , mcPosition , mpVLD_YWork )) return false ;
                  YV[yVlSplyBeltOn] = false ; 
                  m_iCycleStep = 0 ;
                  return true ;

    }

    return false;
}

//---------------------------------------------------------------------------
bool __fastcall TConveyorLoader::IsWorkEnd(void)
{
    return m_bWorkEnd;
}

//---------------------------------------------------------------------------
void __fastcall TConveyorLoader::Load(bool IsLoad , FILE *fp)
{
}
