//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PartUnit.h"
#include "Rail.h"

#include "ErrProc.h"
#include "FileManager.h"
#include "Function.h"
#include "PositionMap.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
//CPart  VL;
const AnsiString sPartName = "Part " ;

CPart::CPart (void)
{
    Reset();
}

void CPart::Reset()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat   ));
    memset(&Step    , 0 , sizeof(SStep   ));
    memset(&PreStep , 0 , sizeof(SPreStep));
}



CPart::~CPart (void)
{

}
double CPart::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstn )
{
/*
    double dPosition = 0.0 ;

    if(Motr == miVLD_Z){
        if (Cmd == mcOneStepF) dPosition =  FM.DevInfo.dCsSlotPitch ;
        if (Cmd == mcOneStepB) dPosition = -FM.DevInfo.dCsSlotPitch ;
        if (Cmd == mcPosition){
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

        if (Cmd == mcFindStep) {
            SORT_INFO SORT = GetSlotInfo(riVLD , fmUnkwn) ;

            if(PstnId == mpVLD_ZWork    ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_ZWorkSttPs] - SORT.iFindRow * FM.DevInfo.dCsSlotPitch  ;

        }
    }

    else if(Motr == miVLD_Y) {
        if (Cmd == mcPosition){
                 if(PstnId == mpCPWait       ) dPosition = MOTR.CAMC[Motr].dPosn[msCPWaitPs       ] ;
            else if(PstnId == mpCPHome       ) dPosition = 0.0                                      ;
            else if(PstnId == mpVLD_YPick    ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_YPickPs    ] ;
            else if(PstnId == mpVLD_YWork    ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_YWorkPs    ] ;
            else if(PstnId == mpVLD_YPlce    ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_YPlcePs    ] ;
        }
    }

    return dPosition ;
*/

}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::CheckSafe(EN_MOTR_ID _iMotr , int _iPstn , bool _bDispMsg)
{
/*    if(MT.CmprPos(_iMotr , PM.GetPos(_iMotr , _iPstn)) && MT.GetStopInpos(_iMotr)) return true ;

    if(_iMotr == miZ) {
    }

    else if(_iMotr == miY) {
        //if(!AT.GetCylStat(aiMain  , ccBwd)) {Trace(MT.GetName(_iMotr).c_str(),"!AT.GetCylStat(aiMain  , ccBwd)"); MsgOkNotRun(AT.GetName(aiMain)+" is not Fwd"); return false ; }
    }

    else if(_iMotr == miI) {
        if(!AT.GetCylStat(aiGtCal , ccFwd)) {Trace(MT.GetName(_iMotr).c_str(),"!AT.GetCylStat(aiGtCal , ccFwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiGtCal)+" is not Fwd"); return false ; }
        if(!AT.GetCylStat(aiUvFB  , ccBwd)) {Trace(MT.GetName(_iMotr).c_str(),"!AT.GetCylStat(aiUvFB  , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiUvFB )+" is not Bwd"); return false ; }
       // if(!AT.GetCylStat(aiDp1IO , ccBwd)) {Trace(MT.GetName(_iMotr).c_str(),"!AT.GetCylStat(aiDp1IO , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp1IO)+" is not Bwd"); return false ; }
       // if(!AT.GetCylStat(aiDp2IO , ccBwd)) {Trace(MT.GetName(_iMotr).c_str(),"!AT.GetCylStat(aiDp2IO , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp2IO)+" is not Bwd"); return false ; }
        if(!AT.GetCylStat(aiDp1UD , ccBwd)) {Trace(MT.GetName(_iMotr).c_str(),"!AT.GetCylStat(aiDp1UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp1UD)+" is not Bwd"); return false ; }
        if(!AT.GetCylStat(aiDp2UD , ccBwd)) {Trace(MT.GetName(_iMotr).c_str(),"!AT.GetCylStat(aiDp2UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp2UD)+" is not Bwd"); return false ; }
        //if(!AT.GetCylStat(aiLdCal , ccBwd)) {Trace(MT.GetName(_iMotr).c_str(),"!AT.GetCylStat(aiLdCal , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiLdCal)+" is not Bwd"); return false ; }
    }
*/
    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd , bool _bDispMsg)
{
/*    if(AT.GetCylStat(_iActr , _bFwd)) return true ;

    if(!MT.GetStopInpos(miZ)) {Trace(AT.GetName(_iActr).c_str(),"!MT.GetStopInpos(miZ)"); return false ; }
    if(!MT.GetStopInpos(miY)) {Trace(AT.GetName(_iActr).c_str(),"!MT.GetStopInpos(miY)"); return false ; }
    if(!MT.GetStopInpos(miI)) {Trace(AT.GetName(_iActr).c_str(),"!MT.GetStopInpos(miI)"); return false ; }

    bool isI_Wait = PM.CmprPos(miI , pi_I_Wait);
    bool isI_Insp = PM.CmprPos(miI , pi_I_Insp);
    bool _bBwd = ! _bFwd ;
    //if( _iActr != aiGtCal) return false ;

         if(_iActr ==aiMain   ) {
    }

    else if(_iActr ==aiSub    ) {

    }
                                                                     
    else if(_iActr ==aiLdCal  ) {
        if(isI_Insp){Trace(AT.GetName(_iActr).c_str(),"isI_Insp"); MsgOkNotRun("CheckSafe","I Motor is in WorkZone"); return false ; }
        if(_bFwd) {

            if(!AT.GetCylStat(aiUvFB  , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiUvFB  , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiUvFB )+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiDp1IO , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp1IO , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp1IO)+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiDp2IO , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp2IO , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp2IO)+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiDp1UD , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp1UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp1UD)+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiDp2UD , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp2UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp2UD)+" is not Bwd"); return false ; }
        }
        else {

            if(!AT.GetCylStat(aiUvFB  , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp2UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiUvFB )+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiUvFB  , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp2UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiUvFB )+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiDp1UD , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp1UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp1UD)+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiDp2UD , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp2UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp2UD)+" is not Bwd"); return false ; }
        }
    }

    else if(_iActr ==aiGp1    ) {
    }
    else if(_iActr ==aiSupport) {}
    else if(_iActr ==aiPcbCmp ) {}
    else if(_iActr ==aiGtCal  ) {}
    else if(_iActr ==aiUvFB   ) {
        if(_bFwd) {
            if( isI_Insp){Trace(AT.GetName(_iActr).c_str(),"is I_Insp"); MsgOkNotRun("CheckSafe","I Motor is in WorkZone"); return false ; }
            if(!AT.GetCylStat(aiLdCal , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiLdCal , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiLdCal)+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiDp1IO , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp1IO , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp1IO)+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiDp2IO , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp2IO , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp2IO)+" is not Bwd"); return false ; }

        }

        if(_bBwd) {
            if(!AT.GetCylStat(aiUvUD , ccFwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiUvUD , ccFwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiUvUD)+" is not Fwd"); return false ; }

        }

    }
    else if(_iActr ==aiUvUD   ) {
       if(!_bFwd) {
           if(!AT.GetCylStat(aiUvFB  , ccFwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp2UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp2UD)+" is not Bwd"); return false ; }
       }
    }

    else if(_iActr ==aiDp1IO  ) {
        if(!AT.GetCylStat(aiDp1UD , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp1UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp1UD )+" is not Bwd"); return false ; }
        if(!AT.GetCylStat(aiPadUD , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiPdUD  , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiPadUD )+" is not Bwd"); return false ; }
    }
    else if(_iActr ==aiDp2IO  ) {
        if(!AT.GetCylStat(aiDp2UD , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp2UD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp2UD)+" is not Bwd"); return false ; }
        if(!AT.GetCylStat(aiPadUD , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiPadUD , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiPadUD)+" is not Bwd"); return false ; }

    }
    else if(_iActr ==aiDp1UD  ) {
        if(_bFwd) {
           if(!AT.GetCylStat(aiUvFB  , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiUvFB  , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiUvFB )+" is not Bwd"); return false ; }
           if(!AT.GetCylStat(aiLdCal , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiLdCal , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiLdCal)+" is not Bwd"); return false ; }
           if(!AT.GetCylStat(aiDp1IO , ccFwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp1IO , ccFwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp1IO)+" is not Fwd"); return false ; }
        }
    }
    else if(_iActr ==aiDp2UD  ) {
        if(_bFwd) {
           if(!AT.GetCylStat(aiUvFB  , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiUvFB  , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiUvFB )+" is not Bwd"); return false ; }
           if(!AT.GetCylStat(aiLdCal , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiLdCal , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiLdCal)+" is not Bwd"); return false ; }
           if(!AT.GetCylStat(aiDp2IO , ccFwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp2IO , ccFwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp2IO)+" is not Fwd"); return false ; }
        }
    }
*/
    //Ok.
    return true;
}

bool CPart::MoveHome()
{
/*
    //Local Var.
    bool r1 , r2 , r3 , r4 , r5 , r6 , r7 , r8 , r9;

    //Time Out.
    if (m_HomeTimer.OnDelay(m_iHomeStep > 10  , 30000)) ERR.SetErr(eDHTHomeTO);

    TCamc::TStat Stat ;

    switch (m_iHomeStep) {
        case  0: m_iHomeStep = 0;
                 break;
        case 10: ERR.SetErr(eDIS_XHoming , true);
                 ERR.SetErr(eDIS_YHoming , true);
                 ERR.SetErr(eHT1_XHoming , true);
                 ERR.SetErr(eHT3_XHoming , true);
                 ERR.SetErr(eHT1_YHoming , true);
                 ERR.SetErr(eHT3_YHoming , true);
                 ERR.SetErr(eHT1_ZHoming , true);
                 ERR.SetErr(eHT2_ZHoming , true);
                 ERR.SetErr(eHT3_ZHoming , true);
                 ERR.SetErr(eDH1_ZHoming , true);
                 ERR.SetErr(eDH2_ZHoming , true);
                 ERR.SetErr(eDH3_ZHoming , true);

                 MOTR.ClearHomeEnd(miDIS_X) ;
                 MOTR.ClearHomeEnd(miDIS_Y) ;
                 MOTR.ClearHomeEnd(miHT1_X) ;
                 MOTR.ClearHomeEnd(miHT3_X) ;
                 MOTR.ClearHomeEnd(miHT1_Y) ;
                 MOTR.ClearHomeEnd(miHT3_Y) ;
                 MOTR.ClearHomeEnd(miHT1_Z) ;
                 MOTR.ClearHomeEnd(miHT2_Z) ;
                 MOTR.ClearHomeEnd(miHT3_Z) ;
                 MOTR.ClearHomeEnd(miDH1_Z) ;
                 MOTR.ClearHomeEnd(miDH2_Z) ;
                 MOTR.ClearHomeEnd(miDH3_Z) ;
                 m_HomeTimer.Clear();

                 MoveMotr(miHT1_Z,mcHome) ;
                 MoveMotr(miHT2_Z,mcHome) ;
                 MoveMotr(miHT3_Z,mcHome) ;

                 m_iHomeStep++;
                 return false ;

        case 11: r1 = MOTR.CAMC[miHT1_Z].GetHomeEnd() ;
                 r2 = MOTR.CAMC[miHT2_Z].GetHomeEnd() ;
                 r3 = MOTR.CAMC[miHT3_Z].GetHomeEnd() ;

                 if(!r1 || !r2 || !r3) return false ;

                 MoveMotr(miHT1_Z,mcPosition , mpCPWait ) ;
                 MoveMotr(miHT2_Z,mcPosition , mpCPWait ) ;
                 MoveMotr(miHT3_Z,mcPosition , mpCPWait ) ;
                 m_iHomeStep++;
                 return false ;

        case 12: r1 = MoveMotr(miHT1_Z,mcPosition , mpCPWait ) ;
                 r2 = MoveMotr(miHT2_Z,mcPosition , mpCPWait ) ;
                 r3 = MoveMotr(miHT3_Z,mcPosition , mpCPWait ) ;

                 if(!r1 ||!r2 ||!r3) return false;

                 MoveMotr(miDIS_X,mcHome) ;
                 MoveMotr(miDIS_Y,mcHome) ;
                 MoveMotr(miHT1_X,mcHome) ;
                 MoveMotr(miHT3_X,mcHome) ;
                 MoveMotr(miHT1_Y,mcHome) ;
                 MoveMotr(miHT3_Y,mcHome) ;

                 m_iHomeStep++;
                 return false;

        case 13: r1 = MOTR.CAMC[miDIS_X].GetHomeEnd() ;
                 r2 = MOTR.CAMC[miDIS_Y].GetHomeEnd() ;
                 r3 = MOTR.CAMC[miHT1_X].GetHomeEnd() ;
                 r4 = MOTR.CAMC[miHT3_X].GetHomeEnd() ;
                 r5 = MOTR.CAMC[miHT1_Y].GetHomeEnd() ;
                 r6 = MOTR.CAMC[miHT3_Y].GetHomeEnd() ;

                 if (!r1 || !r2 || !r3 || !r4 || !r5 || !r6) return false ;

                 SetToolServo(true) ;

                 ERR.SetErr(eDIS_XHoming , false);
                 ERR.SetErr(eDIS_YHoming , false);
                 ERR.SetErr(eHT1_XHoming , false);
                 ERR.SetErr(eHT3_XHoming , false);
                 ERR.SetErr(eHT1_YHoming , false);
                 ERR.SetErr(eHT3_YHoming , false);
                 ERR.SetErr(eHT1_ZHoming , false);
                 ERR.SetErr(eHT2_ZHoming , false);
                 ERR.SetErr(eHT3_ZHoming , false);
                 ERR.SetErr(eDH1_ZHoming , false);
                 ERR.SetErr(eDH2_ZHoming , false);
                 ERR.SetErr(eDH3_ZHoming , false);

                 MoveMotr(miDIS_X,mcPosition , mpCPWait ) ;
                 MoveMotr(miDIS_Y,mcPosition , mpCPWait ) ;
                 MoveMotr(miHT1_X,mcPosition , mpCPWait ) ;
                 MoveMotr(miHT3_X,mcPosition , mpCPWait ) ;
                 MoveMotr(miHT1_Y,mcPosition , mpCPWait ) ;
                 MoveMotr(miHT3_Y,mcPosition , mpCPWait ) ;
                 MoveMotr(miHT1_Z,mcPosition , mpCPWait ) ;
                 MoveMotr(miHT2_Z,mcPosition , mpCPWait ) ;
                 MoveMotr(miHT3_Z,mcPosition , mpCPWait ) ;

                 m_iHomeStep++;
                 return false;

        case 14: r1 = MoveMotr(miDIS_X,mcPosition , mpCPWait ) ;
                 r2 = MoveMotr(miDIS_Y,mcPosition , mpCPWait ) ;
                 r3 = MoveMotr(miHT1_X,mcPosition , mpCPWait ) ;
                 r4 = MoveMotr(miHT3_X,mcPosition , mpCPWait ) ;
                 r5 = MoveMotr(miHT1_Y,mcPosition , mpCPWait ) ;
                 r6 = MoveMotr(miHT3_Y,mcPosition , mpCPWait ) ;

                 if(!r1 || !r2 || !r3 || !r4 || !r5 || !r6) return false ;

                 m_iHomeStep = 0;
                 return true ;
        }

    //Ok.
    return true;

*/
}

bool CPart::MoveMotr(EN_MOTR_ID Motr , EN_PSTN_ID Cmd) // 모터를 움직일때 쓰는 함수.
{
/*    //Check Command. (Stop은 간섭을 확인 하지 않음)
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
*/
    return false ;
}

bool CPart::MoveActr(EN_ACTR_ID Actr , bool On) //실린더를 움직일때 쓰는 함수.
{

}

bool CPart::CycleMove(void) //컨베이어 벨트를 구동하여 카세트를 옮기는 함수.
{

}

bool CPart::Update(void) //오토런닝시에 계속 타는 함수.
{

}

bool CPart::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{

}

bool CPart::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

}

bool CPart::ToStart(void) //스타트를 하기 위한 함수.
{

}

bool CPart::ToStop(void) //스탑을 하기 위한 함수.
{

}

void CPart::Load(bool IsLoad , FILE *fp)
{

}


































//---------------------------------------------------------------------------
__fastcall CPart::CPart(void)
{
    //Init.
    Init();
}

//---------------------------------------------------------------------------
__fastcall CPart::~CPart(void)
{

}

//---------------------------------------------------------------------------
void __fastcall CPart::Init(void) //장비 시작시에 한번만 타는 함수. 초기화.
{

}

//---------------------------------------------------------------------------
void __fastcall CPart::Reset(void)  //장비 리셑 버튼 눌러주는 상황 같은 상황 같은 상황버섯.
{

}

//---------------------------------------------------------------------------
bool __fastcall CPart::MoveHome(void)
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
double __fastcall CPart::GetMotrPos(EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId)
{
    double dPosition = 0.0 ;

    if(Motr == miVLD_Z){
        if (Cmd == mcOneStepF) dPosition =  FM.DevInfo.dCsSlotPitch ;
        if (Cmd == mcOneStepB) dPosition = -FM.DevInfo.dCsSlotPitch ;
        if (Cmd == mcPosition){
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

        if (Cmd == mcFindStep) {
            SORT_INFO SORT = GetSlotInfo(riVLD , fmUnkwn) ;

            if(PstnId == mpVLD_ZWork    ) dPosition = MOTR.CAMC[Motr].dPosn[msVLD_ZWorkSttPs] - SORT.iFindRow * FM.DevInfo.dCsSlotPitch  ;

        }
    }

    else if(Motr == miVLD_Y) {
        if (Cmd == mcPosition){
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
bool __fastcall CPart::CheckDstb(EN_MOTR_ID Motr , EN_MOTR_COMD_ID Cmd , int PstnId , double dPstn)
{

}

//---------------------------------------------------------------------------
bool __fastcall CPart::MoveMotr(EN_MOTR_ID Motr , EN_PSTN_ID Cmd)
{
/*    //Check Command. (Stop은 간섭을 확인 하지 않음)
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
*/
    return false ;
}

//---------------------------------------------------------------------------
bool __fastcall CPart::MoveActr (EN_ACTR_ID Actr , bool On)
{
//    return ACTR.MoveCyl(Actr , On );
return false ;
}

//---------------------------------------------------------------------------
bool __fastcall CPart::ToStartCon(void)
{
/*    //Clear Timer.
    m_bToStart = false    ;
    m_ToStartTimer.Clear();
*/
    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall CPart::ToStopCon(void)
{
/*    //Clear Timer.
    m_bToStop  = false   ;
    m_bReqStop = true    ;
    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
    if (m_iSeqStep) return false;
*/
    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall CPart::ToStart(void)
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
bool __fastcall CPart::ToStop(void)
{
    //Check Time Out.
    if (m_bToStop                               ) return true;
    if (m_ToStopTimer.OnDelay(!m_bToStop , 5000)) { ERR.SetErr(eVLDStopTO); return false; }

    YV[yVlSplyBeltOn] = false ;

    if(!MoveActr(aiVLP_FB , ccBwd ))return false ;

    //Init.
    m_iSeqStep = 0   ; //Clear Step.
    m_bToStop  = true;

    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall CPart::AutoRun(void)
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



bool __fastcall CPart::CycleMove(void) //컨베이어 벨트를 구동하여 카세트를 옮기는 함수.
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




bool __fastcall CPart::CycleSply(void)//클렘프로 서플라이존에 있는 카세트를 찝어 오는 함수.
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
bool __fastcall CPart::CycleStep(void) //Z축 - 카세트의 워크포지션으로 이동하는 함수.
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
bool __fastcall CPart::CyclePush(void)  //로더 푸셔로 스트립을 푸쉬 하는 함수.
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
bool __fastcall CPart::CycleStck(void)  //클램프에 있는 카세트를 스탁존에 내려 놓고 오는 함수.
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
bool __fastcall CPart::IsWorkEnd(void)
{
    return m_bWorkEnd;
}

//---------------------------------------------------------------------------
void __fastcall CPart::Load(bool IsLoad , FILE *fp)
{
}
