//-------------------------------------------------------------+--------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "WorkTable.h"
#include "ErrMan.h"
#include "Motors.h"
#include "IOs.h"
#include "Actuators.h"
#include "FormMain.h"
#include "FormMsgOk.h"
#include "FormMsgYesNo.h"
#include "UserIni.h"

#include <math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

//Vision Data.
double dAngleX  ,dRowAngleX  ;
double dAngleY  ,dRowAngleY  ;
int    iAngleNG              ; //0:OK , 1:AngleNG , 2:NotFound ,
bool   bSizeNG               ;

double dShift   ,dRowShift   ;
bool   bShiftNG              ;

bool   bVerNG                ;
double dVerGap  ,dRowVerGap  ;

bool   bHorNG                ;
double dHorGap  ,dRowHorGap  ;


double           dRowBeamX   ;
double           dRowBeamY   ;
bool   bBeamNG               ;



//Vision [DVD]
AnsiString sAngle             ;
AnsiString sFA_R              ;
AnsiString sFA_T              ;
AnsiString sThree_Beam_Tilt   ;
AnsiString sBeam_Size         ;
AnsiString sGT_Shift          ;
AnsiString sArea              ;
AnsiString sPeak_Ratio        ;
AnsiString sParallelism       ;
AnsiString sAbsolute_Center_X ;
AnsiString sAbsolute_Center_Y ;
AnsiString sBeam_Center_X     ;
AnsiString sBeam_Center_Y     ;
AnsiString sPeak_Center_X     ;
AnsiString sPeak_Center_Y     ;
AnsiString sBeam_Shift        ;
AnsiString sBeam_Shift_R      ;
AnsiString sBeam_Shift_T      ;
//[COMMON]
AnsiString sEvalTime          ;

bool   bRcvdRslt;

bool   bUseAPC  ;

double dLastAPCOut ;


//---------------------------------------------------------------------------
TWorkTable::TWorkTable()
{
    //Init.

    m_bBarcodeScan = false ;

}

//---------------------------------------------------------------------------
TWorkTable::~TWorkTable()
{

}

//---------------------------------------------------------------------------
void _fastcall TWorkTable::Init()
{
    //Init. Var.
    memset(&Done , 0 , sizeof(TDone )) ;

    m_iSeqStat = ssStop;

    bUseAPC = false ;
    m_bStartRev = false ;

    m_InspStartTime = 0.0 ;
    m_dLastIPos     = 0.0 ;

    Reset();

    m_dPreCycleTime = GetTickTime() ;
}

//---------------------------------------------------------------------------
void __fastcall TWorkTable::Reset(void)
{
    //Check running flag.
    if (m_iSeqStat == ssInit   ) return;
    if (m_iSeqStat == ssRunning && !Stat.bStoped ) return;

    //Init. Var.
    memset(&Stat    , 0 , sizeof(TStat )) ;
    memset(&Step    , 0 , sizeof(TStep )) ;
    memset(&PreStep , 0 , sizeof(TStep )) ;
    memset(&Rqst    , 0 , sizeof(TRqst )) ;

    m_bStartRev = false ;

    Cycle = NULL ;


    //Clear Timer.
    m_CycleTimer  .Clear();
    m_HomeTimer   .Clear();
    m_ToStopTimer .Clear();
    m_ToStartTimer.Clear();
    m_MainTimer   .Clear();

    m_PickUpBtTimer.Clear();
    m_UpDownBtTimer.Clear();
    m_VaccumBtTimer.Clear();
    m_ChuckBtTimer .Clear();
    m_StartBtTimer .Clear();
    m_StopBtTimer  .Clear();

    EM.ClearPart(m_iPartId);

    MT.Reset(miZ);
    MT.Reset(miY);
    MT.Reset(miI);

    AT.Reset(aiMain   );
    AT.Reset(aiSub    );
    AT.Reset(aiLdCal  );
    AT.Reset(aiGp1    );
    AT.Reset(aiSupport);
    AT.Reset(aiPcbCmp );
    AT.Reset(aiGtCal  );
    AT.Reset(aiUvFB   );
    AT.Reset(aiUvUD   );
    AT.Reset(aiDp1IO  );
    AT.Reset(aiDp2IO  );
    AT.Reset(aiDp1UD  );
    AT.Reset(aiDp2UD  );
    AT.Reset(aiPadUD  );
    AT.Reset(aiLdIO   );

    //AT.MoveCyl(aiLdCal , ccFwd) ;

    IO.SetY(yHdStart,false );



    //FrmMain -> OnSendAPCMsg("S1","6"); //Off   뻑남.

    //bUseAPC = false ; //요거 문제되는데.....
    //if(Cycle == CycleInsp || Cycle == CycleDisp || Cycle == CycleReInsp || Cycle == CycleDpIp) bUseAPC = false ;
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::CheckSafe(EN_MOTR_ID _iMotr , int _iPstn , bool _bDispMsg)
{
    if(MT.CmprPos(_iMotr , PM.GetPos(_iMotr , _iPstn)) && MT.GetStopInpos(_iMotr)) return true ;

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

    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd , bool _bDispMsg)
{
    if(AT.GetCylStat(_iActr , _bFwd)) return true ;

    if(!MT.GetStopInpos(miZ)) {Trace(AT.GetName(_iActr).c_str(),"!MT.GetStopInpos(miZ)"); return false ; }
    if(!MT.GetStopInpos(miY)) {Trace(AT.GetName(_iActr).c_str(),"!MT.GetStopInpos(miY)"); return false ; }
    //탈조 때문에 홈을 잡는데 어쩔수 없이 주석.
    //if(!MT.GetStopInpos(miI)) {Trace(AT.GetName(_iActr).c_str(),"!MT.GetStopInpos(miI)"); return false ; }

    bool isI_Wait = PM.CmprPos(miI , pi_I_Wait) || PM.GetPos(miI, pi_I_Wait) > MT.GetCmdPos(miI);
    bool isI_Insp = PM.CmprPos(miI , pi_I_Insp , 5.0);
    bool _bBwd = ! _bFwd ;
    //if( _iActr != aiGtCal) return false ;

         if(_iActr ==aiMain   ) {
    }

    else if(_iActr ==aiSub    ) {

    }

    else if(_iActr ==aiLdCal  ) {
        if(isI_Insp){Trace(AT.GetName(miI).c_str(),"isI_Insp"); MsgOkNotRun("CheckSafe","I Motor is in WorkZone"); return false ; }
        if(_bFwd) {

            if(!AT.GetCylStat(aiUvFB  , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiUvFB  , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiUvFB )+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiGp1   , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiGp1   , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiGp1  )+" is not Bwd"); return false ; }
          //if(!AT.GetCylStat(aiDp1IO , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp1IO , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp1IO)+" is not Bwd"); return false ; }
          //if(!AT.GetCylStat(aiDp2IO , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiDp2IO , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiDp2IO)+" is not Bwd"); return false ; }
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

    else if(_iActr ==aiLdIO   ) {
        if(isI_Insp){Trace(AT.GetName(miI).c_str(),"isI_Insp"); MsgOkNotRun("CheckSafe","I Motor is in WorkZone"); return false ; }
        if(!AT.GetCylStat(aiLdCal  , ccFwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiLdCal  , ccFwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiLdCal )+" is not Fwd"); return false ; }
        if(_bFwd) {

            if(!AT.GetCylStat(aiUvFB  , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiUvFB  , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiUvFB )+" is not Bwd"); return false ; }
            if(!AT.GetCylStat(aiGp1   , ccBwd)) {Trace(AT.GetName(_iActr).c_str(),"!AT.GetCylStat(aiGp1   , ccBwd)"); MsgOkNotRun("CheckSafe",AT.GetName(aiGp1  )+" is not Bwd"); return false ; }
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
            if( isI_Insp){Trace(AT.GetName(miI).c_str(),"is I_Insp"); MsgOkNotRun("CheckSafe","I Motor is in WorkZone"); return false ; }
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

    else if(_iActr ==aiDp1IO  ) {  //여기서 메세지 뜸.
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
    //else if(_iActr == aiLdIO ) {
    //
    //}

    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::MoveMotr(EN_MOTR_ID _iMotr , int _iPstn , bool _bDispMsg)
{
    if (!CheckSafe(_iMotr, _iPstn , _bDispMsg)) return false;

    return MT.GoAbsRun(_iMotr, PM.GetPos(_iMotr,_iPstn));
}

bool __fastcall TWorkTable::StopMotr (EN_MOTR_ID _iMotr , bool _bEmgStop)
{
    if(_bEmgStop) return MT.EmgStop(_iMotr) ;
    else          return MT.Stop   (_iMotr) ;
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::MoveActr (EN_ACTR_ID _iActr , bool _bFwd ,bool _bDispMsg )
{
    if (!CheckSafe(_iActr, _bFwd , _bDispMsg)) return false;

   // if(_iActr == aiDp1IO && _bFwd) Trace("dp1IO","Fwd");

    return AT.MoveCyl(_iActr , _bFwd );
}


void TWorkTable::ReqHome()
{
    if(Step.iSeq || EM.IsErr()) return ;

    EM.m_bShowForm = false ;

    Stat.bReqHome = true ;
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::ToStartCon(void)
{
    //Clear Timer.
    Stat.bToStart = false ;
    m_ToStartTimer.Clear();

    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::ToStart(void)
{
    //Check Time Out.
    if (Stat.bToStart                                                    ) return true;
    if (m_ToStartTimer.OnDelay(PreStep.iToStart == Step.iToStart , 10000)) EM.SetErr(eiStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0;
                 return true ;

        case 10: MoveActr(aiDp1UD , ccBwd) ;
                 MoveActr(aiDp2UD , ccBwd) ;
                 //MoveActr(aiPadUD , ccBwd) ;
                 //FrmMain -> OnSendAPCMsg("S1","7"); //Auto
                 if(!bUseAPC) FrmMain -> OnSendAPCMsg("S1","7"); //Auto

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiDp1UD , ccBwd))return false ;
                 if(!MoveActr(aiDp2UD , ccBwd))return false ;
                 //if(!MoveActr(aiPadUD , ccBwd))return false ;
                 if(!bUseAPC) FrmMain -> OnSendAPCMsg("S1","6"); //Off

                 MoveActr(aiDp1IO , ccBwd) ;
                 MoveActr(aiDp2IO , ccBwd) ;
                 MoveActr(aiUvUD  , ccFwd) ;
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveActr(aiDp1IO , ccBwd))return false ;   //여기서 메세지 뜸...
                 if(!MoveActr(aiDp2IO , ccBwd))return false ;
                 if(!MoveActr(aiUvUD  , ccFwd))return false ;
                 MoveActr(aiUvFB  , ccBwd) ;

                 //녀석 MoveActr(aiPadUD , ccFwd) ;
                 Step.iToStart ++ ;
                 return false ;

        case 13: if(!MoveActr(aiUvFB  , ccBwd)) return false ;
                 //녀석 if(!MoveActr(aiPadUD , ccFwd)) return false ;
                 MoveActr(aiGtCal , ccFwd) ;
                 Step.iToStart ++ ;
                 return false ;

        case 14: if(!MoveActr(aiGtCal , ccFwd)) return false ;

                 MoveMotr(miI,pi_I_Wait);
                 Step.iToStart++;
                 return false ;

        case 15: if(!OM.DevOptn.bUseHomming && !MoveMotr(miI,pi_I_Wait) ) return false ;
                 Step.iToStart = 0   ;
                 Stat.bToStart = true ;
                 return true ;
    }
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::ToStopCon(void)
{
    if(Step.iCycle) return false ;

    //Clear Timer.
    Stat.bToStop  = false   ;

    m_ToStopTimer.Clear()   ;

    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::ToStop(void)
{
    //Check Time Out.
    if (Stat.bToStop                                                   ) return true;
    if (m_ToStopTimer.OnDelay(!PreStep.iToStop == Step.iToStop , 10000)) EM.SetErr(eiStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: MoveActr(aiDp1UD , ccBwd) ;
                 MoveActr(aiDp2UD , ccBwd) ;
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiDp1UD , ccBwd))return false ;
                 if(!MoveActr(aiDp2UD , ccBwd))return false ;
                 MoveActr(aiDp1IO , ccBwd) ;
                 MoveActr(aiDp2IO , ccBwd) ;
                 MoveActr(aiUvUD  , ccFwd) ;
                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveActr(aiDp1IO , ccBwd))return false ;
                 if(!MoveActr(aiDp2IO , ccBwd))return false ;
                 if(!MoveActr(aiUvUD  , ccFwd))return false ;

                 MoveActr(aiUvFB , ccBwd) ;
                 MoveActr(aiGtCal , ccFwd);
                 Step.iToStop ++ ;
                 return false ;

        case 13: if(!MoveActr(aiUvFB , ccBwd )) return false ;
                 if(!MoveActr(aiGtCal , ccFwd)) return false ;
                 Step.iToStop = 0   ;
                 Stat.bToStop = true;
                 return true ;
    }
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::IsWorkEnd(void)
{
    return Stat.bWorkEnd;
}

//---------------------------------------------------------------------------
void __fastcall TWorkTable::Update (void)
{
    //Local Var.
    bool isCmErr       = EM.IsPartErr(piBase   );
    bool isErrPart     = EM.IsPartErr(m_iPartId);
    bool isErr         = isCmErr || isErrPart ;

    bool isBarcodeScan = !m_bPreBarcode && m_bBarcodeScan ;
    m_bPreBarcode = m_bBarcodeScan ;



    int  iCycleRet     ;

    bool bPickupSw     = IO.GetXUp(xPickupSw);
    bool bUpDnSw       = IO.GetXUp(xUpDnSw  );
    bool bVacSw        = IO.GetXUp(xVacSw   );
    bool bChuckSw      = IO.GetXUp(xChuckSw );
    bool bStartSw      = IO.GetXUp(xStartSw );
    bool bStopSw       = IO.GetXUp(xStopSw  );
    bool bResetSw      = IO.GetXUp(xResetSw );

    bool bKeepPickupSw = m_PickUpBtTimer.OnDelay(IO.GetX(xPickupSw),500 ) ; if(bKeepPickupSw) m_PickUpBtTimer.Clear();
    bool bKeepUpDnSw   = m_UpDownBtTimer.OnDelay(IO.GetX(xUpDnSw  ),500 ) ; if(bKeepUpDnSw  ) m_UpDownBtTimer.Clear();
    bool bKeepVacSw    = m_VaccumBtTimer.OnDelay(IO.GetX(xVacSw   ),500 ) ; if(bKeepVacSw   ) m_VaccumBtTimer.Clear();
    bool bKeepChuckSw  = m_ChuckBtTimer .OnDelay(IO.GetX(xChuckSw ),500 ) ; if(bKeepChuckSw ) m_ChuckBtTimer .Clear();
    bool bKeepStartSw  = m_StartBtTimer .OnDelay(IO.GetX(xStartSw ),500 ) ; if(bKeepStartSw ) m_StartBtTimer .Clear();
    bool bKeepStopSw   = m_StopBtTimer  .OnDelay(IO.GetX(xStopSw  ),500 ) ; if(bKeepStopSw  ) m_StopBtTimer  .Clear();

    IO.SetY(yStartLp , IO.GetX(xStartSw )||(m_bStartRev && m_bFlick));
    IO.SetY(yStopLp  , IO.GetX(xStopSw  )||(isErr       && m_bFlick));
    IO.SetY(yResetLp , IO.GetX(xResetSw ));

    if(OM.DevOptn.bDryRun) {
        IO.SetY(yPickupLp, m_bFlick);
        IO.SetY(yUpDnLp  , m_bFlick);
        IO.SetY(yVacLp   , m_bFlick);
        IO.SetY(yChuckLp , m_bFlick);
    }
    else if(OM.DevOptn.bDataCheck) {
        IO.SetY(yPickupLp, IO.GetX(xPickupSw)|| Done.bDataCheck);
        IO.SetY(yUpDnLp  , m_bFlick);
        IO.SetY(yVacLp   , m_bFlick);
        IO.SetY(yChuckLp , m_bFlick);
    }

    else {
        IO.SetY(yPickupLp, IO.GetX(xPickupSw)|| Done.bPickUp);
        IO.SetY(yUpDnLp  , IO.GetX(xUpDnSw  )|| Done.bUpDown);
        IO.SetY(yVacLp   , IO.GetX(xVacSw   )|| Done.bVaccum);
        IO.SetY(yChuckLp , IO.GetX(xChuckSw )|| Done.bChuck );
    }

    if(bStartSw) {
        if(!m_bBarcodeScan && OM.DevOptn.bBarcodeScan) {
            MsgOk("ERROR","Plz Scan Barcode");
            return ;
        }
        InspectHomeEnd() ;
    }


    if(bUpDnSw) {
        Trace("Up/Dn Sw","Pushed");
        Trace("Step.iHome"  ,AnsiString(Step.iHome  ).c_str());
        Trace("Done.bUpDown",Done.bUpDown?"true":"false");
        Trace("Done.bPickUp",Done.bPickUp?"true":"false");
    }
                                                                                                 //20121128 Ivan 요청. 디스펜스 펄스트 모드 사용 한다함.
    m_bStartRev |= (bStartSw || isBarcodeScan)  && !Step.iHome && !Done.bInsp   &&  Done.bChuck  /*&& OM.DevOptn.bInspBefrDisp*/ && bUseAPC && !Step.iCycle;

    bool isCycleDryRun      = bStartSw  && !Step.iHome && !Done.bPickUp    &&  OM.DevOptn.bDryRun   ;
    bool isCycleDataCheck   = bPickupSw && !Step.iHome && !Done.bDataCheck &&  OM.DevOptn.bDataCheck;
    bool isCycleUnDataCheck = bPickupSw && !Step.iHome &&  Done.bDataCheck &&  OM.DevOptn.bDataCheck;

    bool isCycleRelease     = bPickupSw && !Step.iHome &&  Done.bChuck  ;


    bool isCyclePickup      = bPickupSw && !Step.iHome && !Done.bPickUp                  ;
    bool isCycleUpDn        = bUpDnSw   && !Step.iHome && !Done.bUpDown &&  Done.bPickUp ;
    bool isCycleVac         = bVacSw    && !Step.iHome && !Done.bVaccum                  ;
    bool isCycleChuck       = bChuckSw  && !Step.iHome && !Done.bChuck  &&  Done.bVaccum ;
                                                                                                                        //dddd
    bool isCycleInsp        =(bStartSw || isBarcodeScan ||m_bStartRev) && !Step.iHome && !bUseAPC && !Done.bInsp && Done.bChuck &&  OM.DevOptn.bInspBefrDisp && (!OM.DevOptn.bDispFirst || Done.bDisp);
    bool isCycleDpIp        =(bStartSw )                               && !Step.iHome && !bUseAPC && !Done.bDisp && Done.bChuck &&  OM.DevOptn.bInspDrngDisp &&  !OM.DevOptn.bDispFirst; //dddd //CycleDisp는 Insp끝나면 바로 자동으로 타게 되어 있다. 하지만 에러가 나면 스타트버튼으로 시작하게 됨.
    bool isCycleDisp        =(bStartSw || isBarcodeScan ||m_bStartRev) && !Step.iHome && !bUseAPC && !Done.bDisp && Done.bChuck                              &&   OM.DevOptn.bDispFirst; //dddd //CycleDisp는 Insp끝나면 바로 자동으로 타게 되어 있다. 하지만 에러가 나면 스타트버튼으로 시작하게 됨.

    bool isCycleReInsp      = false ;//bStartSw  && !Step.iHome && !Done.bReInsp &&  Done.bChuck  && OM.DevOptn.bInspAftrDisp ; 현재 안쓴다.
    bool isCycleUv          = bStartSw  && !Step.iHome && Done.bDisp    && !Done.bUV     &&  Done.bChuck  ;

    bool isCycleUnPickup    = bPickupSw && !Step.iHome &&  Done.bPickUp && !Done.bUpDown ;
    bool isCycleUnUpDn      = bUpDnSw   && !Step.iHome &&  Done.bUpDown                  ;
    bool isCycleUnVac       = bVacSw    && !Step.iHome &&  Done.bVaccum && !Done.bChuck  ;
    bool isCycleUnChuck     = bChuckSw  && !Step.iHome &&  Done.bChuck                   ;
    bool isCycleDispOnly    = false     ;


    //모터 탈조때문에
    //if(isCyclePickup) {MT.DoHome(miI);}


    //Request.
    isCycleInsp     |= Rqst.bInspCycle ; //Rqst.bInspCycle = false ; //여기서 플레그 클리어하면 계속 다음동작을 하게 되므로 Cycle이 끝났을때
    isCycleDisp     |= Rqst.bDispCycle ; //Rqst.bDispCycle = false ; //연결동작 끊어주고 클리어하게 변칙 방법씀.
    isCycleUv       |= Rqst.bUVCycle   ; //Rqst.bUVCycle   = false ;
    isCycleDispOnly |= Rqst.bDisp      ; //Rqst.bDisp      = false ;

    InspectMotor    ();
    InspectActuator ();


    double dCycleTime ;
    m_dCycleTime    = GetTickTime();
    dCycleTime      = m_dCycleTime - m_dPreCycleTime ;
    m_dPreCycleTime = m_dCycleTime ;
         if(Cycle == CycleInsp || Cycle == CycleReInsp) DSP.Data.dAdjTime  += dCycleTime ;
    else if(Cycle == CycleDisp || Cycle == CycleDpIp  ) DSP.Data.dBondTime += dCycleTime ;
    else if(Cycle == CycleUv                          ) DSP.Data.dUVTime   += dCycleTime ;
    else                                                DSP.Data.dWaitTime += dCycleTime ;

    if(Cycle == CycleInsp || Cycle == CycleReInsp || Cycle == CycleDisp || Cycle == CycleUv || Cycle == CycleDpIp)
        //DSP.Data.dWorkTime = DSP.Data.dAdjTime + DSP.Data.dBondTime + DSP.Data.dUVTime + DSP.Data.dWaitTime ;   //이식으로 하면 오른쪽 테이블이 작업자 특성상 더 오래 나온다.
        DSP.Data.dWorkTime = DSP.Data.dAdjTime + DSP.Data.dBondTime + DSP.Data.dUVTime ;//+ DSP.Data.dWaitTime ;

    // homming  관련.
    if((Stat.bReqHome || Stat.bHomming)) {
        if(!isErr) {
            if(Stat.bReqHome) {
                InspectMotor();
                Stat.bReqHome = false ;
                Stat.bHomming = true ;
                Step.iHome = 10 ;
            }
            CycleHome();
        }
        else {
            Stat.bReqHome = false ;
            Stat.bHomming = false ;
            Step.iHome = 0 ;
        }
    }

    if(isCycleUnChuck && Stat.bStoped) {  //하도 이동작을 해서 야매로 하나 빼놓음.
         Stat.bStoped = false ;
         bUseAPC = false ;
         Step.iSeq = 0 ;
         m_CycleTimer.Clear();
    }


    if(isErr && Step.iSeq<=13 && !Stat.bDispensing) { //토출중에는 멈추지 않게 한다.
        Stat.bReqStop  = true ;
        if(Cycle == CycleInsp || Cycle == CycleDisp || Cycle == CycleReInsp || Cycle == CycleDpIp){
            bUseAPC = false ;
        }
    }

    else if (bStopSw && Step.iSeq == 13 && !Stat.bDispensing) { //토출중에는 일시정지 못하게 한다.
         Stat.bStoped = true ;
         m_CycleTimer.Clear();
    }
    else if (bStartSw && Stat.bStoped && Step.iSeq == 13) {
         Stat.bStoped = false ;
         m_CycleTimer.Clear();
    }

    if(Stat.bStoped) return ;

    if(Step.iSeq==0) {
        if(bKeepStopSw  || bResetSw ) {Reset(); /*FrmMain -> OnSendAPCMsg("S1","6");여기에 넣으면 반대쪽또 꺼질수 있음.*/ return ;}

        //For Log & Disp
        if(!Done.bInsp  &&!Done.bDisp  &&!Done.bReInsp && !DSP.m_bWorkStart && //작업이 처음시작될때 클리어.
           (isCycleInsp || isCycleDisp || isCycleReInsp || isCycleDpIp)){
            LOG.ClearData();
            LOG.ClearServerData();
            LOG.Data.StartTime = Now().CurrentTime().FormatString("hh:mm:ss");
            LOG.Data.Table     = m_sPartName ;
            m_InspStartTime    = Now() ;
            m_dInspStartTimems = GetTickTime() ;

            //For Fail & Retry Status.
            DSP.Data.dAdjTime  = 0.0 ;
            DSP.Data.dBondTime = 0.0 ;
            DSP.Data.dUVTime   = 0.0 ;
            DSP.Data.dWorkTime = 0.0 ;

            //DSP.Data.AvrTime
        }

        if(isCyclePickup) {   //Reset.
            DSP.Data.dAdjTime  = 0.0 ;
            DSP.Data.dBondTime = 0.0 ;
            DSP.Data.dUVTime   = 0.0 ;
            DSP.Data.dWorkTime = 0.0 ;
        }

             if(isErr || bStopSw  ) {return ; }
        else if(isCycleDryRun     ) {Cycle = CycleDryRun     ;                  Trace(GetPartName().c_str(),"CycleDryRun   ");}
        else if(isCycleDataCheck  ) {Cycle = CycleDataCheck  ;                  Trace(GetPartName().c_str(),"CycleDataCheck");}
        else if(isCycleUnDataCheck) {Cycle = CycleUnDataCheck;                  Trace(GetPartName().c_str(),"CycleDataCheck");}

        else if(isCycleRelease    ) {Cycle = CycleRelease    ;                  Trace(GetPartName().c_str(),"CycleRelease  ");}

        else if(isCyclePickup     ) {Cycle = CyclePickup     ;                  Trace(GetPartName().c_str(),"CyclePickup   ");}
        else if(isCycleUpDn       ) {Cycle = CycleUpDn       ;                  Trace(GetPartName().c_str(),"CycleUpDn     ");}
        else if(isCycleVac        ) {Cycle = CycleVac        ;                  Trace(GetPartName().c_str(),"CycleVac      ");}
        else if(isCycleChuck      ) {Cycle = CycleChuck      ;                  Trace(GetPartName().c_str(),"CycleChuck    ");}

        else if(isCycleInsp       ) {Cycle = CycleInsp       ; bUseAPC = true ; Trace(GetPartName().c_str(),"CycleInsp     "); if(!DSP.m_bWorkStart)DSP.Data.iTotalCnt++; DSP.m_bWorkStart = true ; m_bStartRev = false ;}
        else if(isCycleDpIp       ) {Cycle = CycleDpIp       ; bUseAPC = true ; Trace(GetPartName().c_str(),"CycleDpIp     ");}
        else if(isCycleDisp       ) {Cycle = CycleDisp       ; bUseAPC = true ; Trace(GetPartName().c_str(),"CycleDisp     ");}
        else if(isCycleReInsp     ) {Cycle = CycleReInsp     ; bUseAPC = true ; Trace(GetPartName().c_str(),"CycleReInsp   "); if(!DSP.m_bWorkStart)DSP.Data.iTotalCnt++; DSP.m_bWorkStart = true ;}
        else if(isCycleUv         ) {Cycle = CycleUv         ;                  Trace(GetPartName().c_str(),"CycleUv       ");}

        else if(isCycleUnPickup   ) {Cycle = CycleUnPickup   ;                  Trace(GetPartName().c_str(),"CycleUnPickup ");}
        else if(isCycleUnUpDn     ) {Cycle = CycleUnUpDn     ;                  Trace(GetPartName().c_str(),"CycleUnUpDn   ");}
        else if(isCycleUnVac      ) {Cycle = CycleUnVac      ;                  Trace(GetPartName().c_str(),"CycleUnVac    ");}
        else if(isCycleUnChuck    ) {Cycle = CycleUnChuck    ;                  Trace(GetPartName().c_str(),"CycleUnChuck  "); Done.bInsp = false ;Done.bReInsp = false ;}
        else if(isCycleDispOnly   ) {Cycle = CycleDispOnly   ;                  Trace(GetPartName().c_str(),"CycleDispOnly ");}
        else                        {return ;                                                                               }

        //For Log & Disp
        /*
        if(!Done.bInsp  &&!Done.bDisp  &&!Done.bReInsp && !DSP.m_bWorkStart
           (isCycleInsp || isCycleDisp || isCycleReInsp )){
            LOG.ClearData();
            LOG.ClearServerData();
            LOG.Data.StartTime = Now().CurrentTime().FormatString("hh:mm:ss");
            LOG.Data.Table     = m_sPartName ;
            m_InspStartTime = Now() ;
            m_dInspStartTimems = GetTickTime() ;

            //For Fail & Retry Status.
            DSP.Data.dAdjTime  = 0.0 ;
            DSP.Data.dBondTime = 0.0 ;
            DSP.Data.dUVTime   = 0.0 ;
            DSP.Data.dWorkTime = 0.0 ;

            //DSP.Data.AvrTime
        }*/

        if(isCyclePickup) {   //Reset.
            DSP.Data.dAdjTime  = 0.0 ;
            DSP.Data.dBondTime = 0.0 ;
            DSP.Data.dUVTime   = 0.0 ;
            DSP.Data.dWorkTime = 0.0 ;
        }

        Step.iSeq   = 10 ;
        Step.iCycle = 10 ;

        EM.m_bShowForm = false ;

        m_MainTimer.Clear() ;
    }

    if (m_MainTimer.OnDelay(Step.iSeq && PreStep.iSeq == Step.iSeq && Step.iSeq != 13 , 10000)) {
        //Trace Log.
        EM.SetErr (eiCkBtnTO);
        memset(&Step , 0 , sizeof(TStep));
        Stat.bRun  = false;
        //bUseAPC = false ;
        return ;
    }

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iSeq=%02d" , Step.iSeq );

    if(Step.iSeq != PreStep.iSeq) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }
                                                                                                   
    PreStep.iSeq = Step.iSeq ;

    //Running Step.
    switch (Step.iSeq) {
        case   0: Step.iSeq = 0;
                  return;

        case 10 : if (!InspectMotor    ()) { Step.iSeq = 0; return; }
                  if (!InspectActuator ()) { Step.iSeq = 0; return; }
                  if (Cycle != CyclePickup   &&  //모터탈조때문에 중간에 GT 홈을 잡는다.
                      Cycle != CycleUpDn     &&
                      Cycle != CycleVac      &&
                      Cycle != CycleChuck    &&
                      Cycle != CycleUnPickup &&
                      Cycle != CycleUnUpDn   &&
                      Cycle != CycleUnVac    &&
                      Cycle != CycleUnChuck  && !InspectHomeEnd  ()) { Step.iSeq = 0; return; }
                  Step.iSeq++;
                  return ;

        case 11 : if(!ToStartCon()) return ;
                  Step.iToStart = 10 ;
                  Step.iSeq++;
                  return ;

        case 12 : if(!ToStart()) return ;
                  Stat.bRun  = true;
                  Step.iSeq++;
                  return ;

        case 13 : if(Stat.bReqStop && ToStopCon()) {
                      Stat.bRun     = false ;
                      Stat.bReqStop = false ;
                      Step.iToStop  = 10    ;
                      Step.iSeq++;
                      return ;
                  }

                  iCycleRet = Cycle() ;

                  if (iCycleRet == ERR) {
                      Stat.bRun     = false ;
                      Stat.bReqStop = false ;
                      Step.iToStop  = 10    ;
                      Cycle = NULL ;
                      Step.iSeq++;
                      return ;
                  }

                 else if (iCycleRet == END) {
                      if(Cycle == CycleDryRun    ) {Step.iCycle = 10     ; return ;} //드라이런일때는 계속 뺑뺑이.
                      if(Cycle == CyclePickup    ) {Done.bPickUp = true  ;}
                      if(Cycle == CycleUpDn      ) {Done.bUpDown = true  ;
                                                    Cycle = CycleVac ; //요청사항으로 바로 CycleVac를 타게 함.
                                                    Step.iCycle = 10  ;
                                                    return ;
                                                   }
                      if(Cycle == CycleVac       ) {Done.bVaccum = true  ;}
                      if(Cycle == CycleChuck     ) {Done.bChuck  = true  ;}
                      if(Cycle == CycleInsp      ) {if(OM.DevOptn.bInfinityInsp && !Rqst.bInspCycle) {  //존나 계속 검사일경우...
                                                        Step.iCycle = 10  ;
                                                        return ;
                                                    }
                                                    if(!OM.DevOptn.bAskInsp && !Rqst.bInspCycle ) { //검사만 하고 멈추는 옵션.   메뉴얼로 Insp만 하는경우.
                                                        Done.bInsp   = true  ;
                                                        isCmErr       = EM.IsPartErr(piBase   );
                                                        if(!isCmErr || EM.GetLastErr() != eiVisnBeam){ //인스펙션 사이클 마지막에 빔검사 에러시에 띄운다.

                                                                 if(OM.DevOptn.bDispFirst   ){Cycle = CycleUv   ; bUseAPC = false ; }
                                                            else if(OM.DevOptn.bInspDrngDisp) Cycle = CycleDpIp ;
                                                            else                              Cycle = CycleDisp ; //예외적으로 바로 Disp를 타게 함.
                                                            Step.iCycle = 10  ;
                                                            return ;
                                                        }
                                                    }
                                                    bUseAPC = false ;
                                                    Rqst.bInspCycle = false ;
                                                   }

                      if(Cycle == CycleDisp      ) {Done.bDisp   = true  ;  //요거 DpIn에서 디스펜싱 끝나고 에러 났을경우 때문에 Cycle에서 한번 온시킨다.

                                                    //bUseAPC      = false ;
                                                    if(!Rqst.bDispCycle) { //메뉴얼로 동작시킨것이 아니면.
                                                             if(OM.DevOptn.bInspAftrDisp) {Cycle = CycleReInsp ; }//예외적으로
                                                        else if(OM.DevOptn.bDispFirst   ) {Cycle = CycleInsp   ; }
                                                        else                              {Cycle = CycleUv     ; bUseAPC = false ; }
                                                        Step.iCycle = 10    ;
                                                        return ;
                                                    }
                                                    bUseAPC = false ;
                                                    Rqst.bDispCycle = false ;
                                                   }
                      if(Cycle == CycleDpIp      ) {Done.bDisp   = true  ;  //요거 DpIn에서 디스펜싱 끝나고 에러 났을경우 때문에 Cycle에서 한번 온시킨다.
                                                    Done.bInsp   = true  ;  //인스펙션도 했기때문.

                                                    //bUseAPC      = false ;
                                                    if(!Rqst.bDispCycle) { //메뉴얼로 동작시킨것이 아니면.
                                                        if(OM.DevOptn.bInspAftrDisp) {Cycle = CycleReInsp ; }//예외적으로 바로 Disp를 타게 함.
                                                        else                         {Cycle = CycleUv     ; bUseAPC = false ; }
                                                        Step.iCycle = 10    ;
                                                        return ;
                                                    }
                                                    bUseAPC = false ;
                                                    Rqst.bDispCycle = false ;
                                                   }

                      if(Cycle == CycleReInsp    ) {Done.bReInsp = true ;
                                                    bUseAPC = false ;
                                                    Cycle = CycleUv     ;
                                                    Step.iCycle = 10    ;
                                                    return ;
                                                   }

                      if(Cycle == CycleUv        ) {
                                                    if(!Rqst.bUVCycle) {
                                                        SetEtcLog();
                                                        LOG.WriteLog();
                                                        Trace("____ServerLog____","-------------------------------------------");
                                                        LOG.WriteServerLog(m_iPartId == piLT) ;
                                                        //display.
                                                        m_bBarcodeScan = false ;
                                                    }

                                                    Rqst.bUVCycle   = false ;

                                                    //UV
                                                    Done.bPickUp = false ;
                                                    Done.bUpDown = false ;
                                                    Done.bVaccum = false ;
                                                    Done.bChuck  = false ;
                                                    Done.bInsp   = false ;
                                                    Done.bDisp   = false ;
                                                    Done.bReInsp = false ;
                                                    Done.bUV     = true  ;}


                      if(Cycle == CycleUnPickup  ) {if(DSP.m_bWorkStart) {
                                                        DSP.m_bWorkStart = false ;
                                                        SetEtcLog();
                                                        LOG.WriteLog();
                                                        LOG.WriteServerLog(m_iPartId == piLT) ;
                                                    }


                                                    Done.bPickUp = false ;
                                                    Done.bUpDown = false ;
                                                    Done.bVaccum = false ;
                                                    Done.bChuck  = false ;
                                                    Done.bInsp   = false ;
                                                    Done.bDisp   = false ;
                                                    Done.bReInsp = false ;
                                                    Done.bUV     = false ;

                                                    m_bBarcodeScan = false ;
                                                    }
                      if(Cycle == CycleRelease   ) {if(DSP.m_bWorkStart) {
                                                        DSP.m_bWorkStart = false ;
                                                        SetEtcLog();
                                                        LOG.WriteLog();
                                                        LOG.WriteServerLog(m_iPartId == piLT) ;
                                                    }


                                                    Done.bPickUp = false ;
                                                    Done.bUpDown = false ;
                                                    Done.bVaccum = false ;
                                                    Done.bChuck  = false ;
                                                    Done.bInsp   = false ;
                                                    Done.bDisp   = false ;
                                                    Done.bReInsp = false ;
                                                    Done.bUV     = false ;

                                                    m_bBarcodeScan = false ;
                                                    }
                      if(Cycle == CycleUnUpDn    ) {Done.bUpDown = false ;}
                      if(Cycle == CycleUnVac     ) {Done.bVaccum = false ;}
                      if(Cycle == CycleUnChuck   ) {Done.bChuck  = false ;
                                                   }
                      if(Cycle == CycleDispOnly  ) {Rqst.bDisp   = false ;}

                      if(Cycle == CycleDataCheck ) {Done.bDataCheck = true  ;}
                      if(Cycle == CycleUnDataCheck){Done.bDataCheck = false ;}

                      Cycle = NULL ;
                      //bUseAPC = false ; //dddd
                      Stat.bRun     = false ;
                      Stat.bReqStop = false ;
                      Step.iToStop  = 10    ;
                      Step.iSeq++;
                      return ;
                  }

                  return ;

        case 14 : if (!ToStop()) return;
                  Step.iSeq  = 0 ;
                  return;
    }
}



void __fastcall TWorkTable::SetEtcLog() //요건 시간 데이터가 있어서 UV끝나고 수행.
{
//DSP.Data.dWorkTime = DSP.Data.dAdjTime +  ;
    m_dInspTimems = GetTickTime() - m_dInspStartTimems ;
    //DSP.Data.dTotalTime += m_dInspTimems ; 보이는 DispForm 하고 통일 할려고 식을 바꿈.
    DSP.Data.dTotalTime += m_dInspTimems ;

    DSP.m_bWorkStart = false ;

    //시점바꿀때 요놈 주의.
    if(LOG.ServerLogData.Judge != "NG")DSP.Data.iPassCnt ++ ;

    Trace("LOG.ServerLogData.Judge", LOG.ServerLogData.Judge.c_str());


    if(DSP.Data.iTotalCnt ) DSP.Data.dAvrTime =       DSP.Data.dTotalTime / (double)DSP.Data.iPassCnt  ;
    if(DSP.Data.iTotalCnt ) DSP.Data.dYield   = 100 * DSP.Data.iPassCnt   / (double)DSP.Data.iTotalCnt ;

    //Log.
    m_InspTime = Now() - m_InspStartTime  ;
    LOG.Data.EndTime    = Now().CurrentTime().FormatString("hh:mm:ss");
    LOG.Data.WorkTime   = (AnsiString)((int)(DSP.Data.dWorkTime /100)/ 10.0);
    LOG.Data.InspTime   = (AnsiString)((int)(DSP.Data.dAdjTime  /100)/ 10.0);
    LOG.Data.BondTime   = (AnsiString)((int)(DSP.Data.dBondTime /100)/ 10.0);
    LOG.Data.UVTime     = (AnsiString)((int)(DSP.Data.dUVTime   /100)/ 10.0);
    LOG.Data.WaitTime   = (AnsiString)((int)(DSP.Data.dWaitTime /100)/ 10.0);
    LOG.Data.TotalYield =  AnsiString (DSP.Data.dYield) + "%"               ;


    //Clear.
    DSP.Data.dWaitTime = 0.0 ; //웨이트 타임은 먼져 클리어해서 로그  데이터 남기기 좋게 한다.

}

//요놈은 UV끝나면 반대쪽 테이블 데이터가 들어 올수 있어서. 검사끝날때 마다 수행.
/*void __fastcall TWorkTable::SetServerData(//AnsiString sSerial     , //요놈은 다이렉트로 입력됌.
                                          AnsiString sJudge      ,
                                          AnsiString sFirst_NG   ,
                                          AnsiString sLD_Motor_Y ,
                                          AnsiString sLD_MOtor_Z ,
                                          AnsiString sGTX_Motor_X)
{
    LOG.ServerLogData.Angle             = sAngle            ;
    LOG.ServerLogData.FA_R              = sFA_R             ;
    LOG.ServerLogData.FA_T              = sFA_T             ;
    LOG.ServerLogData.Three_Beam_Tilt   = sThree_Beam_Tilt  ;
    LOG.ServerLogData.Beam_Size         = sBeam_Size        ;
    LOG.ServerLogData.GT_Shift          = sGT_Shift         ;
    LOG.ServerLogData.Area              = sArea             ;
    LOG.ServerLogData.Peak_Ratio        = sPeak_Ratio       ;
    LOG.ServerLogData.Parallelism       = sParallelism      ;
    LOG.ServerLogData.Absolute_Center_X = sAbsolute_Center_X;
    LOG.ServerLogData.Absolute_Center_Y = sAbsolute_Center_Y;
    LOG.ServerLogData.Beam_Center_X     = sBeam_Center_X    ;
    LOG.ServerLogData.Beam_Center_Y     = sBeam_Center_Y    ;
    LOG.ServerLogData.Peak_Center_X     = sPeak_Center_X    ;
    LOG.ServerLogData.Peak_Center_Y     = sPeak_Center_Y    ;
    LOG.ServerLogData.Beam_Shift        = sBeam_Shift       ;
    LOG.ServerLogData.Beam_Shift_R      = sBeam_Shift_R     ;
    LOG.ServerLogData.Beam_Shift_T      = sBeam_Shift_T     ;
    LOG.ServerLogData.LD_Motor_Y        = sLD_Motor_Y       ;
    LOG.ServerLogData.LD_MOtor_Z        = sLD_MOtor_Z       ;
    LOG.ServerLogData.GTX_Motor_X       = sGTX_Motor_X      ;//  GT축인데 X로 표시하게 됐음.

    LOG.ServerLogData.Judge             = sJudge            ;
//  LOG.ServerLogData.Serial            = sSerial           ;
    LOG.ServerLogData.First_NG          = sFirst_NG         ;
    LOG.ServerLogData.AdjEqNum          = "0"               ;
    LOG.ServerLogData.EvalTime          = Now().FormatString("yyyy-mm-dd hh:nn");
}*/

//요놈은 UV끝나면 반대쪽 테이블 데이터가 들어 올수 있어서. 검사끝날때 마다 수행.
void __fastcall TWorkTable::SetServerData(EN_INSP_MODE ipMode      ,
                                          AnsiString   sJudge      ,
                                          AnsiString   sFirst_NG   ,
                                          AnsiString   sLD_Motor_Y ,
                                          AnsiString   sLD_MOtor_Z ,
                                          AnsiString   sGTX_Motor_X)
{
    LOG.ServerLogData.Angle             = sAngle            ;
    LOG.ServerLogData.FA_R              = sFA_R             ;
    LOG.ServerLogData.FA_T              = sFA_T             ;
    LOG.ServerLogData.Three_Beam_Tilt   = sThree_Beam_Tilt  ;

    LOG.ServerLogData.Area              = sArea             ;
    LOG.ServerLogData.Peak_Ratio        = sPeak_Ratio       ;
    LOG.ServerLogData.Parallelism       = sParallelism      ;
    LOG.ServerLogData.Absolute_Center_X = sAbsolute_Center_X;
    LOG.ServerLogData.Absolute_Center_Y = sAbsolute_Center_Y;

    if(ipMode == imAll) {
    LOG.ServerLogData.Beam_Size         = sBeam_Size        ;
    LOG.ServerLogData.GT_Shift          = sGT_Shift         ;
    LOG.ServerLogData.Beam_Center_X     = sBeam_Center_X    ;
    LOG.ServerLogData.Beam_Center_Y     = sBeam_Center_Y    ;
    LOG.ServerLogData.Peak_Center_X     = sPeak_Center_X    ;
    LOG.ServerLogData.Peak_Center_Y     = sPeak_Center_Y    ;
    LOG.ServerLogData.Beam_Shift        = sBeam_Shift       ;
    LOG.ServerLogData.Beam_Shift_R      = sBeam_Shift_R     ;
    LOG.ServerLogData.Beam_Shift_T      = sBeam_Shift_T     ;
    }

    LOG.ServerLogData.LD_Motor_Y        = sLD_Motor_Y       ;
    LOG.ServerLogData.LD_MOtor_Z        = sLD_MOtor_Z       ;

    if(ipMode == imAll||ipMode == imAnGt) {
    LOG.ServerLogData.GTX_Motor_X       = sGTX_Motor_X      ;//  GT축인데 X로 표시하게 됐음.
    }

    LOG.ServerLogData.Judge             = sJudge            ;
//  LOG.ServerLogData.Serial            = sSerial           ;
    LOG.ServerLogData.First_NG          = sFirst_NG         ;
    LOG.ServerLogData.AdjEqNum          = "0"               ;
    LOG.ServerLogData.EvalTime          = Now().FormatString("yyyy-mm-dd hh:nn");
}

void __fastcall TWorkTable::SetServerDataSerial(AnsiString sSerial)
{

    LOG.ServerLogData.Serial            = sSerial           ;
}


void __fastcall TWorkTable::UpdateSeqState(void)
{
    //Local Var.
    bool isErr     = EM.IsPartErr(m_iPartId) ;
    bool isRun     = GetStepSeq  (         ) ;
    bool isHome    = GetStepHome (         ) ;

    //Flicking Timer.
    if (m_bFlick) { m_FlickOnTimer .Clear(); if (m_FlickOffTimer.OnDelay( m_bFlick , 500)) m_bFlick = false; }
    else          { m_FlickOffTimer.Clear(); if (m_FlickOnTimer .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }

    //Set Stat.
         if (isHome ) { m_iSeqStat = ssInit    ;}
    else if (isErr  ) { m_iSeqStat = ssError   ;}
    else if (isRun  ) { m_iSeqStat = ssRunning ;}
    else              { m_iSeqStat = ssStop    ;}
}
//---------------------------------------------------------------------------
int TWorkTable::CycleHome(void)
{
    //Time Out.
    if (m_HomeTimer.OnDelay(PreStep.iHome == Step.iHome , 30000)) {
        EM.SetErr(eiHomeTO);
        Step.iHome = 0;
        Stat.bHomming = false ;
        return ERR ;
    }

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iHome=%02d" , Step.iHome );
    if(Step.iHome != PreStep.iHome) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }


    PreStep.iHome = Step.iHome ;

    //Move Home.
    switch (Step.iHome) {
        default: Step.iHome = 0;
                 return END ;

        case 10: MT.SetServo(miZ,true) ;
                 MT.SetServo(miY,true) ;
                 MT.SetServo(miI,true) ;

                 MoveActr(aiPadUD  , ccBwd , false);
                 Step.iHome++;
                 return CNT ;

        case 11: if(!MoveActr(aiPadUD  , ccBwd , false)) return CNT ;

                 MoveActr(aiUvUD  , ccFwd , false);
                 MoveActr(aiDp1UD , ccBwd , false);
                 MoveActr(aiDp2UD , ccBwd , false);
                 MoveActr(aiGp1   , ccBwd , false);
                 //MoveActr(aiLdCal , ccFwd , false);
                 Step.iHome++;
                 return CNT ;

        case 12: if(!MoveActr(aiUvUD  , ccFwd , false)) return CNT ;
                 if(!MoveActr(aiDp1UD , ccBwd , false)) return CNT ;
                 if(!MoveActr(aiDp2UD , ccBwd , false)) return CNT ;
                 if(!MoveActr(aiGp1   , ccBwd , false)) return CNT ;
                 //if(!MoveActr(aiLdCal , ccFwd , false)) return CNT ;
                 
                 MoveActr(aiDp1IO   , ccBwd , false);
                 MoveActr(aiDp2IO   , ccBwd , false);

                 MoveActr(aiUvFB    , ccBwd , false);
                 //MoveActr(aiMain    , ccBwd , false);
                 MoveActr(aiGtCal   , ccFwd , false);

                 MoveActr(aiSupport , ccFwd , false);
                 MoveActr(aiPcbCmp  , ccFwd , false);

                 Step.iHome++;
                 return CNT ;

        case 13: if(!MoveActr(aiDp1IO   , ccBwd , false)) return CNT ;
                 if(!MoveActr(aiDp2IO   , ccBwd , false)) return CNT ;

                 if(!MoveActr(aiUvFB    , ccBwd , false)) return CNT ;
                 //if(!MoveActr(aiMain    , ccBwd , false)) return CNT ;
                 if(!MoveActr(aiGtCal   , ccFwd , false)) return CNT ;

                 if(!MoveActr(aiSupport , ccFwd , false)) return CNT ;
                 if(!MoveActr(aiPcbCmp  , ccFwd , false)) return CNT ;

                 MoveActr(aiPadUD  , ccFwd , false);

                 Step.iHome++;
                 return CNT ;

        case 14: if(!MoveActr(aiPadUD , ccFwd , false))return CNT ;
                 if(!MT.Home(miI)) return CNT ;
                 MoveActr(aiLdCal   , ccFwd , false);
                 Step.iHome++ ;
                 return CNT ;

        case 15: if(!MT.Home(miY)) return CNT ;
                 if(!MoveActr(aiLdCal  , ccFwd , false)) return CNT ;
                 MoveActr(aiLdIO   , ccBwd , false);
                 Step.iHome++ ;
                 return CNT ;

        case 16: if(!MT.Home(miZ)) return CNT ;
                 if(!MoveActr(aiLdIO   , ccBwd , false))return CNT ;
                 MoveActr(aiMain  , ccFwd );
                 MoveActr(aiSub   , ccFwd );
                 Step.iHome++ ;
                 return CNT ;

        case 17: if(!MoveActr(aiMain , ccFwd )) return CNT ;
                 if(!MoveActr(aiSub  , ccFwd )) return CNT ;
                 MoveMotr(miI , pi_I_Wait) ;
                 MoveMotr(miZ , pi_Z_Wait) ;
                 MoveMotr(miY , pi_Y_Wait) ;
                 MoveActr(aiSupport , ccBwd);

                 Step.iHome ++ ;
                 return CNT ;

        case 18: if(!MoveMotr(miI       , pi_I_Wait))return CNT ;
                 if(!MoveMotr(miZ       , pi_Z_Wait))return CNT ;
                 if(!MoveMotr(miY       , pi_Y_Wait))return CNT ;
                 if(!MoveActr(aiSupport , ccBwd    ))return CNT ;
                 Step.iHome = 0;
                 Stat.bHomming = false ;
                 memset(&Done , 0 , sizeof(TDone )) ;
                 return END ;
    }
}


int TWorkTable::CycleInsp(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , Step.iCycle == 10 ? 90000 : 10000 )) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        FrmMain -> OnSendAPCMsg("S1","6"); //Off
        MoveActr(aiGtCal,ccFwd);
        //bUseAPC = false ;
        Step.iCycle = 0 ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    double dTemp= 0.0;
    double dOffsetCntX ;
    double dOffsetCntY ;
    double dOffSetPosX ;
    double dOffSetPosY ;
    double dTempHorGap ;

    bool bR1 , bR2 , bR3 ;
    CAxtFS20::CUsrPara UsrPara ;

    bool bFastDP = !OM.DevOptn.bInspAftrDisp && !Rqst.bInspCycle && !OM.DevOptn.bAskInsp && !OM.DevOptn.bInfinityInsp && !OM.DevOptn.bDispFirst;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;
                                                              //  aiLDP_FB  : 로더 푸셔
        case  10: //bbbb if(bUseAPC) return CNT ;
                  //bUseAPC = true ;

                  iAngleNG = 1 ;   bSizeNG = true ;
                  bShiftNG = bHorNG = bBeamNG = true ;
                  if(bFastDP) MoveActr(aiPadUD,ccBwd) ; //Take Time 줄이기 위해.   //재검사 옵션이거나 메뉴얼 동작 일경우는 안나간다.

                  MoveActr(aiGtCal,ccFwd);
                  if(OM.DevOptn.bGrabMode) FrmMain -> OnSendVisnMsg(mvAuto) ;
                  else                       FrmMain -> OnSendVisnMsg(mvTest) ;
                  FrmMain -> OnSendAPCMsg("S1","7"); //Auto

                  Step.iCycle++;
                  return CNT;

        //Step 바꿀때 타임아웃 확인.
        case  11: FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  Step.iCycle++;
                  return CNT;

        case  12: FrmMain -> OnSendAPCMsg("S2","3", AnsiString(OM.DevOptn.dAPCSetVal).c_str()); //APC POWER SETTING

                  Step.iCycle++;
                  return CNT;

        case  13: if(m_iPartId == piLT) FrmMain -> OnSendAPCMsg("S1","3"); //1P/U
                  if(m_iPartId == piRT) FrmMain -> OnSendAPCMsg("S1","4"); //2P/U
                  Step.iCycle++;
                  return CNT;

        case  14: if(OM.DevOptn.bROMType) FrmMain -> OnSendAPCMsg("S1","2"); //R/W
                  else                    FrmMain -> OnSendAPCMsg("S1","1"); //R/W
                  //FrmMain -> OnSendAPCMsg("S2","3", AnsiString(OM.DevOptn.dAPCSetVal).c_str()); //APC POWER SETTING
                  Step.iCycle++;
                  return CNT;

        case  15: FrmMain -> OnSendAPCMsg("S1","5"); //On
                  MoveActr(aiGtCal,ccFwd);
                  m_TempTimer2.Clear();
                  Step.iCycle++;
                  return CNT;

        case  16: if(!MoveActr(aiGtCal,ccFwd)) return CNT ;
                  if(!m_bInspMaxErr)MoveMotr(miI,pi_I_Insp );  //검사 맥스 에러시에는 그자리에서 다시 검사하기 위해.
                  if(!m_bInspMaxErr)MoveMotr(miY,pi_Y_Insp );
                  if(!m_bInspMaxErr)MoveMotr(miZ,pi_Z_Insp );
                  Step.iCycle++;
                  return CNT;

        case  17: if(!m_bInspMaxErr&&!MoveMotr(miI,pi_I_Insp ))return CNT ;
                  if(!m_bInspMaxErr&&!MoveMotr(miY,pi_Y_Insp ))return CNT ;
                  if(!m_bInspMaxErr&&!MoveMotr(miZ,pi_Z_Insp ))return CNT ;
                  if(bFastDP && !MoveActr(aiPadUD,ccBwd)) return CNT  ; //Take Time 줄이기 위해.
                  if(bFastDP) { MoveActr(aiDp1IO,ccFwd) ; MoveActr(aiDp2IO,ccFwd) ;}


                  MoveActr(aiGtCal , ccBwd);
                  Step.iCycle++;
                  return CNT;

        case  18: if(!MoveActr(aiGtCal , ccBwd)) return CNT ;
                  MT.GetUsrPara(miI,UsrPara) ;
                  MT.Vibration(miI , OM.DevOptn.dGTDnVibRng , UsrPara.dRunSpeed , OM.DevOptn.iGTDnVibCnt , true ) ;
                  Step.iCycle++;
                  return CNT ;

        case  19: MT.GetUsrPara(miI,UsrPara) ;
                  if(!MT.Vibration(miI , OM.DevOptn.dGTDnVibRng , UsrPara.dRunSpeed , OM.DevOptn.iGTDnVibCnt,false))return CNT ;
                  m_TempTimer.Clear() ;
                  m_iCcInspInspCnt = 0 ;
                  Step.iCycle++;
                  return CNT;

        case  20: if(!m_TempTimer .OnDelay(true , OM.DevOptn.iInspDelay )) return CNT ;
                  //if(!m_TempTimer2.OnDelay(true , 3500)) return CNT ;
                  if(!m_TempTimer2.OnDelay(true , OM.DevOptn.iAPCOnDelay)) return CNT ;
                  if(bFastDP &&!MoveActr(aiPadUD,ccBwd)) return CNT ;

                  m_iCcInspTMInspCnt = 0 ;
                  m_iCcInspInspCnt++;
                  Trace("Vision Inspection",("Vision Cnt = " + AnsiString(m_iCcInspInspCnt)).c_str());
                  Step.iCycle++;
                  return CNT;


        //스텝바꿀때 주의. 30번에서쓴다.
        case  21: bRcvdRslt = false ;
                  m_dCcInspGTOriPos = MT.GetCmdPos(miI) ;
                  FrmMain -> OnSendVisnMsg(mvInsp) ;
                  m_TempTimer.Clear();

                  Step.iCycle++;
                  return CNT;

        case  22: if(m_TempTimer.OnDelay(true , 500)&& m_iCcInspTMInspCnt < 3  ){ //dk......
                      m_iCcInspTMInspCnt++;
                      Trace("Visn Err","Visn Rslt Delayed over 500");
                      Step.iCycle -- ;
                      return CNT ;
                  }
                  if(!bRcvdRslt) return CNT ;   //eiVisnInspFail   iInspDelay


                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {          //초기 값에서 차이를 구해야 한다. 기구 문제로 조정 안되는 상태면 밑도끝도 없이 감.
                          dTemp = OM.DevOptn.dMaxYPos ;
                          dTemp = PM.GetPos(miY,pi_Y_Insp) ;
                          dTemp = MT.GetCmdPos(miY) ;
                          dTemp = dAngleX ;

                          //dOffsetCntX = (dAngleXRow + dAngleXHigh) /2.0 ;
                          //dOffsetCntY = (dAngleYRow + dAngleYHigh) /2.0 ;

                          //dOffSetPosX = dAngleX + dOffsetCntX ;
                          //dOffSetPosY = dAngleY + dOffsetCntY ;

                          dTemp = fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ;
                          dTemp = fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ;

                          if(OM.DevOptn.dMaxYPos < fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ) {EM.SetErr  (eiVisnRangOver); return ERR ; }
                          else                                                                                    {MT.GoIncRun(miY, dAngleX  );              }

                          if(OM.DevOptn.dMaxZPos < fabs(PM.GetPos(miZ,pi_Z_Insp)-(dAngleY + MT.GetCmdPos(miZ))) ) {EM.SetErr  (eiVisnRangOver); return ERR ; } //Err
                          else                                                                                    {MT.GoIncRun(miZ, dAngleY  );              }


                          /*
                          if(OM.DevOptn.dMaxYPos < fabs(PM.GetPos(miY,pi_Y_Insp)-(dOffSetPosX + MT.GetCmdPos(miY))) ) {EM.SetErr  (eiVisnRangOver  ); return ERR ; }
                          else                                                                                        {MT.GoIncRun(miY, dOffSetPosX);              }

                          if(OM.DevOptn.dMaxZPos < fabs(PM.GetPos(miZ,pi_Z_Insp)-(dOffSetPosY + MT.GetCmdPos(miZ))) ) {EM.SetErr  (eiVisnRangOver  ); return ERR ; } //Err
                          else                                                                                        {MT.GoIncRun(miZ, dOffSetPosY);              }*/

                      }
                  }

                  if(OM.DevOptn.bUseAPCCal) {
                      if(bHorNG) {
                          //if(OM.DevOptn.dAPCCalLim > fabs(dHorGap))
                          if(OM.DevOptn.dAPCCalLim > dHorGap)
                              FrmMain -> OnSendAPCMsg("S2","3", dHorGap , true ); //APC POWER SETTING
                          else
                              FrmMain -> OnSendAPCMsg("S2","3", dHorGap > 0 ? OM.DevOptn.dAPCCalLim : -OM.DevOptn.dAPCCalLim , true ); //APC POWER SETTING
                      }
                  }

                  if(OM.DevOptn.bUseShiftCal && !iAngleNG && !bHorNG) {
                      if(bShiftNG) {           //초기 값에서 차이를 구해야 한다. 기구 문제로 조정 안되는 상태면 밑도끝도 없이 감.
                          if(OM.DevOptn.dMaxIPos < fabs(PM.GetPos(miI,pi_I_Insp)-(dShift + MT.GetCmdPos(miI))) ) {EM.SetErr(eiShiftRangOver); return ERR ; }
                          else                                                                                   {MT.GoIncRun(miI, dShift ) ;              }
                      }
                  }

                  //Sleep(1000);

                  Step.iCycle++;
                  return CNT;

        case  23: //Trace("sun","GetStop");
                  if(!MT.GetStopInpos(miZ))return CNT ;
                  if(!MT.GetStopInpos(miY))return CNT ;             //
                  if(!MT.GetStopInpos(miI))return CNT ;             //

                  //Angle
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {
                          if(dAngleX) {
                              MT.GetUsrPara(miY,UsrPara) ;
                              MT.Vibration(miY , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngYVibCnt , true );
                              //MT.GoIncRun(miY, dAngleX) ;
                          }
                          if(dAngleY) {
                              MT.GetUsrPara(miZ,UsrPara) ;
                              MT.Vibration(miZ , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngZVibCnt , true );
                              //MT.GoIncRun(miZ, dAngleY) ;
                          }
                      }
                  }

                  //GT
                  if(OM.DevOptn.bUseShiftCal && !iAngleNG && !bHorNG) {
                      if(bShiftNG) {
                          if(dShift){
                              MT.GetUsrPara(miI,UsrPara) ;
                              MT.Vibration(miI , OM.DevOptn.dSfVibRng , UsrPara.dRunSpeed , OM.DevOptn.iSfVibCnt , true ) ;
                              //MT.GoIncRun(miI, dShift ) ;
                          }
                      }
                  }


                  Step.iCycle++;
                  return CNT;


        case  24: bR1 = bR2 = bR3 = true ;
                  //Angle
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {
                          if(dAngleX) {
                              MT.GetUsrPara(miY,UsrPara) ;
                              bR1 = MT.Vibration(miY , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngYVibCnt , false );
                          }
                          if(dAngleY) {
                              MT.GetUsrPara(miZ,UsrPara) ;
                              bR2 = MT.Vibration(miZ , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngZVibCnt , false );
                          }
                      }
                  }

                  //GT
                  if(OM.DevOptn.bUseShiftCal&& !iAngleNG && !bHorNG) {
                      if(bShiftNG) {
                          if(dShift){
                              MT.GetUsrPara(miI,UsrPara) ;
                              bR3 = MT.Vibration(miI , OM.DevOptn.dSfVibRng , UsrPara.dRunSpeed , OM.DevOptn.iSfVibCnt , false ) ;
                          }
                      }
                  }

                  if(!bR1 || !bR2 || !bR3) return CNT ;
                  Step.iCycle++;
                  return CNT;

        case  25: //LOG.ServerLogData.LD_Motor_Y  = MT.GetCmdPos(miY) ;
                  //LOG.ServerLogData.LD_MOtor_Z  = MT.GetCmdPos(miZ) ;
                  //LOG.ServerLogData.GTX_Motor_X = MT.GetCmdPos(miI) ; //요거 이름 바꿔야 함. .나중에 디벅할일있을때.  GTX_Motor_X -> GTX_Motor_Y

                  LOG.Data.AngleX = dRowAngleX  ;
                  LOG.Data.AngleY = dRowAngleY  ;
                  LOG.Data.Shift  = dRowShift   ;
                  LOG.Data.APC    = dRowHorGap  ;

                  if((iAngleNG && OM.DevOptn.bUseAngleCal) ||
                     (bShiftNG && OM.DevOptn.bUseShiftCal) ||
                     (bHorNG   && OM.DevOptn.bUseAPCCal  )){

                      LOG.Data.Result = "1_NG"      ;

                           if(iAngleNG)SetServerData(imAll , "NG" , "Angle"    , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),MT.GetCmdPos(miI)) ;
                      else if(bShiftNG)SetServerData(imAll , "NG" , "ShiftGT"  , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),MT.GetCmdPos(miI)) ;
                      else if(bHorNG  )SetServerData(imAll , "NG" , "APC"      , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),MT.GetCmdPos(miI)) ;
                      //else if(bBeamNG )LOG.ServerLogData.First_NG = "Beam"     ;     빔은 및에서 재정의





                      if(m_iCcInspInspCnt < OM.DevOptn.iMaxInspCnt) {   //지정 횟수보다 검사가 작을때.

                          m_TempTimer.Clear() ;
                          Step.iCycle -= 5 ;
                          return CNT ;
                      }
                      else {                                            //지정 횟수보다 검사가 클때.

                         //Angle Beam 찾기 옵션에 따른 처리.
                         if(OM.DevOptn.dFindAngle && iAngleNG == 2/*Angle Beam Out Err*/) {
                              Step.iCycle = 26 ;
                              return CNT ;
                         }

                         else {
                              EM.SetErr(eiVisnInspFail) ;
                              m_bInspMaxErr = true ;
                              FrmMain -> OnSendVisnMsg(mvTest) ;
                              return ERR ;
                         }

                      }
                  }
                  else { //굿으로 끝났을경우.
                      LOG.Data.Result = "1_OK"      ;
                      SetServerData(imAll , "OK" , "None" , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),MT.GetCmdPos(miI)) ;
                  }


                  Step.iCycle=40 ;
                  return CNT ;


        //여기부터 범위 밖에 찾기.
        case  26: m_iCcInspFindCnt = 0 ;
                  Step.iCycle++;
                  return CNT ;

        case  27: MT.GoIncRun(miY, m_iCcInspFindCnt ? -OM.DevOptn.dFindAngle * 2: OM.DevOptn.dFindAngle ) ;   //두번 이상 하면 고쳐야함.
                  Step.iCycle++;
                  return CNT ;

        case  28: if(!MT.GetStopInpos(miY))return CNT ;
                  m_iCcInspTMInspCnt = 0 ;
                  Step.iCycle++;
                  return CNT ;

        case  29: bRcvdRslt = false ;
                  FrmMain -> OnSendVisnMsg(mvInsp) ;
                  m_TempTimer.Clear();

                  Step.iCycle++;
                  return CNT;

        case  30: if(m_TempTimer.OnDelay(true , 500)&& m_iCcInspTMInspCnt < 3  ){ //dk......
                      m_iCcInspTMInspCnt++;
                      Trace("VisnErr","Visn Rslt Delayed over 500");
                      Step.iCycle -- ;
                      return CNT ;
                  }
                  if(!bRcvdRslt) return CNT ;   //eiVisnInspFail   iInspDelay

                  if(iAngleNG != 2) {  //find옵션으로 이동하여 찾아봤을때 빔아웃 에러가 아니면 찾은것 이니 이동하여 검사.
                      FrmMain -> OnSendAPCMsg("S2","3", AnsiString(OM.DevOptn.dAPCSetVal).c_str()); //APC POWER SETTING
                      m_iCcInspInspCnt = 0 ;
                      Step.iCycle = 21 ;
                      return CNT ;

                  }

                  if(m_iCcInspFindCnt <1) {//두번만 함.
                      m_iCcInspFindCnt++;
                      Step.iCycle -=3 ;
                      return CNT ;
                  }

                  EM.SetErr(eiVisnInspFail) ;
                  FrmMain -> OnSendVisnMsg(mvTest) ;

                  return ERR ;





        //마무리 작업
        case  40: m_dLastIPos = MT.GetCmdPos(miI) ;



                       if(dShift < 0) MT.GoIncRun(miI ,  OM.DevOptn.dGTUpOffset) ;
                  else if(dShift > 0) MT.GoIncRun(miI , -OM.DevOptn.dGTUpOffset) ;

                 // MoveActr(aiGtCal , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  41: if(!MT.GetStopInpos(miI))return CNT ;
                  MoveActr(aiGtCal , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  42: if(!MoveActr(aiGtCal , ccFwd)) return CNT ;
                  //FrmMain -> OnSendAPCMsg("S1","6"); //Off 본드 디스펜싱 끝나구 끈다. 옆테이블로 APC 안빼끼려고 넣은 패턴.
                  /*(if(OM.DevOptn.bGrabMode)*/

                  if(OM.DevOptn.bUseBeamRslt && bBeamNG){
                      LOG.Data.Result = "1_NG"        ;

                      SetServerData(imAll , "NG" , "Beam" , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),m_dLastIPos) ;
                      EM.SetErr(eiVisnBeam) ;
                  }

                  if(bSizeNG) { //2012.04.10 추가 빅빔 걸러 내기.
                      LOG.Data.Result = "1_NG"        ;

                      SetServerData(imAll , "NG" , "Angle" , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),m_dLastIPos) ;
                      EM.SetErr(eiVisnBeam) ;
                  }

                  if(!OM.DevOptn.bInspDrngDisp /*&& !OM.DevOptn.bAdjGT2ndInsp*/)FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  if( OM.DevOptn.bDispFirst   )FrmMain -> OnSendAPCMsg("S1","6"); //Off

                  Step.iCycle = 0 ;
                  //bUseAPC = false ;
                  return END ;
        }
}

int TWorkTable::CycleDpIp(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 15000 )) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        IO.SetY(yHdStart,false );
        Stat.bDispensing = false ;
        Step.iCycle = 0 ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool bTemp = false ;
    bool bR1 , bR2 , bR3 , bR4 , bR5 , bR6 ;
    int  bR11, bR22, bR33, bR44, bR55, bR66  ;
    int iTemp= 0 ;
    double dTemp ;
    double dGTOriPos ;

    CAxtFS20::CUsrPara UsrPara ;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;

        case  10: MoveActr(aiLdIO   , ccFwd ) ;
                  Step.iCycle++;
                  return CNT;

        case  11: if(!MoveActr(aiLdIO   , ccFwd )) return CNT ;
                  MoveActr(aiDp1UD , ccBwd);
                  MoveActr(aiDp2UD , ccBwd);
                  MoveActr(aiLdCal , ccBwd);
                  IO.SetY(yHdStart,false ); //UV Init.
                  Step.iCycle++;
                  return CNT;

        case  12: if(!MoveActr(aiLdCal , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp1UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp2UD , ccBwd)) return CNT ;
                  MoveActr(aiGtCal , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  13: if(!MoveActr(aiGtCal , ccFwd)) return CNT ;
                  Step.iCycle++;
                  return CNT ;

        case  14: MoveMotr(miI,pi_I_Wait ) ;
                  if(!OM.DevOptn.bInspBefrDisp || OM.DevOptn.bDispFirst) {
                      MoveMotr(miY , pi_Y_Disp) ;
                      MoveMotr(miZ , pi_Z_Disp) ;
                  }


                  Step.iCycle++;
                  return CNT;

        case  15: if(!MoveMotr(miI,pi_I_Wait ))return CNT ;

                  //탈조때문에.
                  //if(OM.DevOptn.bUseHomming ) {
                  //    MT.DoHome(miI) ;
                  //    Trace("Homming","while Disp");
                  //}

                  if(!OM.DevOptn.bInspBefrDisp || OM.DevOptn.bDispFirst) {
                      if(!MoveMotr(miY , pi_Y_Disp)) return CNT ;
                      if(!MoveMotr(miZ , pi_Z_Disp)) return CNT ;
                  }



                  MoveActr(aiPadUD , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  16: if(!MoveActr(aiPadUD , ccBwd)) return CNT ;
                  MoveActr(aiDp1IO , ccFwd);
                  MoveActr(aiDp2IO , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  17: if(!MoveActr(aiDp1IO , ccFwd)) return CNT ;
                  if(!MoveActr(aiDp2IO , ccFwd)) return CNT ;
                  MoveActr(aiDp1UD , ccFwd);
                  MoveActr(aiDp2UD , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  18: if(!MoveActr(aiDp1UD , ccFwd)) return CNT ;
                  if(!MoveActr(aiDp2UD , ccFwd)) return CNT ;

                  //Dispensing pattern
                  m_iCcDpIpDispCnt = 0 ;
                  m_TempTimer.Clear() ;
                  Step.iCycle++;
                  return CNT ;

        case  19: if(!m_TempTimer.OnDelay(true , 100)) return CNT ;
                  Step.iCycle++;
                  return CNT ;

        case  20: Stat.bDispensing = true ;
                  if(!OM.DevOptn.bIgnrDisp) {
                      IO.SetY(ySol1 , iOptnDisp[0][m_iCcDpIpDispCnt]);
                      IO.SetY(ySol2 , iOptnDisp[1][m_iCcDpIpDispCnt]);
                      IO.SetY(ySol3 , iOptnDisp[2][m_iCcDpIpDispCnt]);
                      IO.SetY(ySol4 , iOptnDisp[3][m_iCcDpIpDispCnt]);
                      IO.SetY(ySol5 , iOptnDisp[4][m_iCcDpIpDispCnt]);
                      IO.SetY(ySol6 , iOptnDisp[5][m_iCcDpIpDispCnt]);
                  }
                  m_DispTimer1.Clear() ;
                  m_DispTimer2.Clear() ;
                  m_DispTimer3.Clear() ;
                  m_DispTimer4.Clear() ;
                  m_DispTimer5.Clear() ;
                  m_DispTimer6.Clear() ;

                  Step.iCycle++;
                  return CNT;

        case  21: //for(int i = 0 ; i < MAX_DISP_CNT ; i++) {
                  //    if(iTemp < iOptnDisp[5][iDispCnt]) iTemp = iOptnDisp[5][iDispCnt] ;
                  //    if(iTemp < iOptnDisp[5][iDispCnt]) iTemp = iOptnDisp[5][iDispCnt] ;
                  //    if(iTemp < iOptnDisp[5][iDispCnt]) iTemp = iOptnDisp[5][iDispCnt] ;
                  //    if(iTemp < iOptnDisp[5][iDispCnt]) iTemp = iOptnDisp[5][iDispCnt] ;
                  //    if(iTemp < iOptnDisp[5][iDispCnt]) iTemp = iOptnDisp[5][iDispCnt] ;
                  //}

                  bR11 = iOptnDisp[0][m_iCcDpIpDispCnt] ;
                  bR22 = iOptnDisp[1][m_iCcDpIpDispCnt] ;
                  bR33 = iOptnDisp[2][m_iCcDpIpDispCnt] ;
                  bR44 = iOptnDisp[3][m_iCcDpIpDispCnt] ;
                  bR55 = iOptnDisp[4][m_iCcDpIpDispCnt] ;
                  bR66 = iOptnDisp[5][m_iCcDpIpDispCnt] ;


                  bR1 = m_DispTimer1.OnDelay(true , iOptnDisp[0][m_iCcDpIpDispCnt]);
                  bR2 = m_DispTimer2.OnDelay(true , iOptnDisp[1][m_iCcDpIpDispCnt]);
                  bR3 = m_DispTimer3.OnDelay(true , iOptnDisp[2][m_iCcDpIpDispCnt]);
                  bR4 = m_DispTimer4.OnDelay(true , iOptnDisp[3][m_iCcDpIpDispCnt]);
                  bR5 = m_DispTimer5.OnDelay(true , iOptnDisp[4][m_iCcDpIpDispCnt]);
                  bR6 = m_DispTimer6.OnDelay(true , iOptnDisp[5][m_iCcDpIpDispCnt]);

                  if(bR1)IO.SetY(ySol1 , false);
                  if(bR2)IO.SetY(ySol2 , false);
                  if(bR3)IO.SetY(ySol3 , false);
                  if(bR4)IO.SetY(ySol4 , false);
                  if(bR5)IO.SetY(ySol5 , false);
                  if(bR6)IO.SetY(ySol6 , false);

                  if(!bR1 || !bR2 || !bR3 || !bR4 || !bR5 || !bR6 ) return CNT ;

                  m_iCcDpIpDispCnt++ ;
                  if(m_iCcDpIpDispCnt < MAX_DISP_CNT) {
                      Step.iCycle -= 2;
                      m_TempTimer.Clear() ;
                      return CNT ;
                  }
                  Done.bDisp   = true  ; // 이이후에 에러 나도 그냥 다시 CycleDpIn 안타게 하기 위해.

                  m_TempTimer.Clear() ;
                  Step.iCycle++ ;
                  return CNT ;

        case  22: if(!m_TempTimer.OnDelay(true , OM.DevOptn.iDispEndDelay))return CNT ;
                  Stat.bDispensing = false ;

                  bR1 = MoveActr(aiDp1UD , ccBwd);
                  bR2 = MoveActr(aiDp2UD , ccBwd);
                  bR3 = MoveActr(aiUvUD  , ccFwd);

                  if(OM.DevOptn.bInspAfterDpDone) {
                      if(!bR1 || !bR2 || !bR3) return CNT ;

                  }

                  Step.iCycle++;
                  return CNT ;










        //DpIp여기부터 인스펙션.
        //검사를 하면 먼저 보정값 만큼 가고 바이브레이션을 한다.
        case  23: m_iCcDpIpTMInspCnt = 0 ;
                  m_iCcDpIpInspCnt   = 0 ;
                  Step.iCycle++;
                  return CNT ;

        case  24: /*여기부터 삽입*/
                  if(MoveActr(aiDp1UD , ccBwd) && MoveActr(aiDp2UD , ccBwd)) {
                      MoveActr(aiDp1IO , ccBwd);
                      MoveActr(aiDp2IO , ccBwd);
                  }

                  bRcvdRslt = false ;

                  m_iCcDpIpInspCnt++;

                  FrmMain -> OnSendVisnMsg(mvReInsp) ;
                  m_TempTimer.Clear() ;

                  Step.iCycle++;
                  return CNT;

        case  25: if(m_TempTimer.OnDelay(true , 500)&& m_iCcDpIpTMInspCnt < 3  ){  //아 안습... 가끔 응답이 없는 경우 있다.
                      m_iCcDpIpTMInspCnt++;
                      Step.iCycle -- ;
                      return CNT ;
                  }
                  if(!bRcvdRslt) return CNT ;   //eiVisnInspFail   iInspDelay
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {          //초기 값에서 차이를 구해야 한다. 기구 문제로 조정 안되는 상태면 밑도끝도 없이 감.

                          dTemp = fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ;
                          dTemp = fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ;


                          if(OM.DevOptn.dMaxYPos < fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ) {EM.SetErr  (eiVisnRangOver); return ERR ; }
                          else                                                                                    {MT.GoIncRun(miY, dAngleX  );              }

                          if(OM.DevOptn.dMaxZPos < fabs(PM.GetPos(miZ,pi_Z_Insp)-(dAngleY + MT.GetCmdPos(miZ))) ) {EM.SetErr  (eiVisnRangOver); return ERR ; } //Err
                          else                                                                                    {MT.GoIncRun(miZ, dAngleY  );              }
                      }
                  }


                  //여기서는 APC는 건드리지 않는 게 좋음. 옵션으로 뺐음.
                  if(OM.DevOptn.bUseAPCCal && OM.DevOptn.bAPCOnReadjust) {
                      if(bHorNG) {
                          if(OM.DevOptn.dAPCCalLim > fabs(dHorGap))
                              FrmMain -> OnSendAPCMsg("S2","3", dHorGap , true ); //APC POWER SETTING
                          else
                              FrmMain -> OnSendAPCMsg("S2","3", dHorGap > 0 ? OM.DevOptn.dAPCCalLim : -OM.DevOptn.dAPCCalLim , true ); //APC POWER SETTING
                      }
                  }






                  Step.iCycle++;
                  return CNT;

        case  26: if(!MT.GetStopInpos(miZ))return CNT ;
                  if(!MT.GetStopInpos(miY))return CNT ;

                  //Angle
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {
                          if(dAngleX) {
                              MT.GetUsrPara(miY,UsrPara) ;
                              MT.Vibration(miY , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngYVibCnt , true );
                              //MT.GoIncRun(miY, dAngleX) ;
                          }
                          if(dAngleY) {
                              MT.GetUsrPara(miZ,UsrPara) ;
                              MT.Vibration(miZ , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngZVibCnt , true );
                              //MT.GoIncRun(miZ, dAngleY) ;
                          }
                      }
                  }

                  Step.iCycle++;
                  return CNT;


        case  27: bR1 = bR2 = true ;
                  //Angle
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {
                          if(dAngleX) {
                              MT.GetUsrPara(miY,UsrPara) ;
                              bR1 = MT.Vibration(miY , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngYVibCnt , false );
                          }
                          if(dAngleY) {
                              MT.GetUsrPara(miZ,UsrPara) ;
                              bR2 = MT.Vibration(miZ , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngZVibCnt , false );
                          }
                      }
                  }




                  if(!bR1 || !bR2) return CNT ;

                  LOG.Data.ReAngleX = dRowAngleX  ;
                  LOG.Data.ReAngleY = dRowAngleY  ;
                  LOG.Data.ReShift  = dRowShift   ;
                  LOG.Data.ReAPC    = dRowHorGap  ;

                  if(iAngleNG && OM.DevOptn.bUseAngleCal) {
                      LOG.Data.ReResult = "2_NG"        ;
                      SetServerData(imAn , "NG" , "Angle" , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),m_dLastIPos) ;

                      if(m_iCcDpIpInspCnt < OM.DevOptn.iMaxInspCnt) {
                          m_TempTimer.Clear() ;
                          Step.iCycle -= 3 ;
                          return CNT ;
                      }

                      //어차피 에러 띄워 봐짜 픽업은 본드묻은 병신 이라 재조정으로 가게 한다.
                      //else {
                      //    EM.SetErr(eiVisnInspFail) ;
                      //    FrmMain -> OnSendVisnMsg(mvTest) ;
                      //    return ERR ;
                      //
                      //}
                  }
                  else {
                      LOG.Data.ReResult = "2_OK"        ;
                      SetServerData(imAn , "OK" , "None" , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),m_dLastIPos) ;
                  }


                  //Angle은 조정해서 완료 됐지만 GT는 이시점에 조정을 못하기 때문에 GT페일일 경우. GT재조정 모드로 들어감.
                  m_iCcDpIpGTCnt = 0 ;
                  if((bShiftNG || iAngleNG) && OM.DevOptn.bAdjGT2ndInsp) {
                      SetServerData(imAnGt , "2_NG" , "None" , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),m_dLastIPos) ;
                      Step.iCycle =   50 ;
                      return CNT;
                  }

                  Step.iCycle = 90 ;
                  return CNT;

                  /*여기까지 삽입*/








        //DPIP GT엔지 여서 재조정 모드
        case  50: MoveActr(aiDp1UD , ccBwd);
                  MoveActr(aiDp2UD , ccBwd);
                  MoveActr(aiGtCal , ccFwd);
                  m_TempTimer2.Clear();
                  Step.iCycle++;
                  return CNT;

        case  51: if(!MoveActr(aiDp1UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp2UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiGtCal , ccFwd)) return CNT ;
                  /*if(!m_iCcDpIpGTCnt)*/MoveMotr(miI,pi_I_Insp );
                  Step.iCycle++;
                  return CNT;

        case  52: if(/*!m_iCcDpIpGTCnt && */!MoveMotr(miI,pi_I_Insp ))return CNT ;
                  //탈조때문에 잠시 주석.
                  //if(m_dLastIPos>0.0) MT.GoAbsRun(miI,m_dLastIPos) ;
                  Step.iCycle++;
                  return CNT;

        case  53: //if(!MT.GetStopInpos(miI)) return CNT ;
                  /*if(!m_iCcDpIpGTCnt)*/ MoveActr(aiGtCal , ccBwd);
                  Step.iCycle++;
                  return CNT;

        case  54: if(/*!m_iCcDpIpGTCnt &&*/ !MoveActr(aiGtCal , ccBwd)) return CNT ;
                  MT.GetUsrPara(miI,UsrPara) ;
                  MT.Vibration(miI , OM.DevOptn.dGTDnVibRng , UsrPara.dRunSpeed , OM.DevOptn.iGTDnVibCnt , true ) ;
                  Step.iCycle++;
                  return CNT ;

        case  55: MT.GetUsrPara(miI,UsrPara) ;
                  if(!MT.Vibration(miI , OM.DevOptn.dGTDnVibRng , UsrPara.dRunSpeed , OM.DevOptn.iGTDnVibCnt,false))return CNT ;

                  Step.iCycle++;
                  return CNT ;

        case  56: m_TempTimer.Clear() ;
                  m_iCcDpIpInspCnt = 0 ;
                  Step.iCycle++;
                  return CNT;

        case  57: if(!m_TempTimer .OnDelay(true , OM.DevOptn.iInspDelay )) return CNT ;

                  m_iCcDpIpInspCnt++;
                  Trace("Vision Inspection",("Vision Cnt = " + AnsiString(m_iCcDpIpInspCnt)).c_str());
                  m_iCcDpIpTMInspCnt = 0 ;
                  Step.iCycle++;
                  return CNT;

        case  58: if(MoveActr(aiDp1UD , ccBwd) && MoveActr(aiDp2UD , ccBwd)) { //디스펜서 실린더를 빨리 빼놓기 위함.
                      MoveActr(aiDp1IO , ccBwd);
                      MoveActr(aiDp2IO , ccBwd);
                  }
                  bRcvdRslt = false ;
                  dGTOriPos = PM.GetPos(miI,pi_I_Insp) ;//MT.GetCmdPos(miI) ;
                  FrmMain -> OnSendVisnMsg(mvInsp) ;
                  m_TempTimer.Clear();

                  Step.iCycle++;
                  return CNT;

        case  59: if(m_TempTimer.OnDelay(true , 500)&& m_iCcDpIpTMInspCnt < 3  ){ //dk......  가끔 결과가 않들어 오는 경우가 있어서....
                      m_iCcDpIpTMInspCnt++ ;
                      Step.iCycle -- ;
                      return CNT ;
                  }
                  if(!bRcvdRslt) return CNT ;   //eiVisnInspFail   iInspDelay
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {          //초기 값에서 차이를 구해야 한다. 기구 문제로 조정 안되는 상태면 밑도끝도 없이 감.
                          dTemp = OM.DevOptn.dMaxYPos ;
                          dTemp = PM.GetPos(miY,pi_Y_Insp) ;
                          dTemp = MT.GetCmdPos(miY) ;
                          dTemp = dAngleX ;

                          //dOffsetCntX = (dAngleXRow + dAngleXHigh) /2.0 ;
                          //dOffsetCntY = (dAngleYRow + dAngleYHigh) /2.0 ;

                          //dOffSetPosX = dAngleX + dOffsetCntX ;
                          //dOffSetPosY = dAngleY + dOffsetCntY ;

                          dTemp = fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ;
                          dTemp = fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ;

                          if(OM.DevOptn.dMaxYPos < fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ) {EM.SetErr  (eiVisnRangOver); return ERR ; }
                          else                                                                                    {MT.GoIncRun(miY, dAngleX  );              }

                          if(OM.DevOptn.dMaxZPos < fabs(PM.GetPos(miZ,pi_Z_Insp)-(dAngleY + MT.GetCmdPos(miZ))) ) {EM.SetErr  (eiVisnRangOver); return ERR ; } //Err
                          else                                                                                    {MT.GoIncRun(miZ, dAngleY  );              }

                      }
                  }

                  //여기서는 APC는 건드리지 않는 게 좋음. 옵션으로 뺐음.
                  if(OM.DevOptn.bUseAPCCal && OM.DevOptn.bAPCOnReadjust) {
                      if(bHorNG) {
                          if(OM.DevOptn.dAPCCalLim > fabs(dHorGap))
                              FrmMain -> OnSendAPCMsg("S2","3", dHorGap , true ); //APC POWER SETTING
                          else
                              FrmMain -> OnSendAPCMsg("S2","3", dHorGap > 0 ? OM.DevOptn.dAPCCalLim : -OM.DevOptn.dAPCCalLim , true ); //APC POWER SETTING
                      }
                  }

                  if(OM.DevOptn.bUseShiftCal && !iAngleNG) { //m_iCcDpIpGTCnt는 GT빼고 하는 마지막 검사시에 Fail이여서 조정하는 경우는 Angle만 조정
                      if(bShiftNG /*&& !m_iCcDpIpGTCnt*/) { //m_iCcDpIpGTCnt는 GT빼고 하는 마지막 검사시에 Fail이여서 조정하는 경우는 Angle만 조정 초기 값에서 차이를 구해야 한다. 기구 문제로 조정 안되는 상태면 밑도끝도 없이 감.
                          if(OM.DevOptn.dMaxIPos < fabs(PM.GetPos(miI,pi_I_Insp)-(dShift + MT.GetCmdPos(miI))) ) {EM.SetErr(eiShiftRangOver); return ERR ; }
                          else                                                                                   {MT.GoIncRun(miI, dShift ) ;              }
                      }
                  }

                  Step.iCycle++;
                  return CNT;

        case  60: //Trace("sun","GetStop");
                  if(!MT.GetStopInpos(miZ))return CNT ;
                  if(!MT.GetStopInpos(miY))return CNT ;             //
                  if(!MT.GetStopInpos(miI))return CNT ;             //

                  //Angle
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {
                          if(dAngleX) {
                              MT.GetUsrPara(miY,UsrPara) ;
                              MT.Vibration(miY , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngYVibCnt , true );
                              //MT.GoIncRun(miY, dAngleX) ;
                          }
                          if(dAngleY) {
                              MT.GetUsrPara(miZ,UsrPara) ;
                              MT.Vibration(miZ , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngZVibCnt , true );
                              //MT.GoIncRun(miZ, dAngleY) ;
                          }
                      }
                  }

                  //GT
                  if(OM.DevOptn.bUseShiftCal && !iAngleNG) {
                      if(bShiftNG /*&& !m_iCcDpIpGTCnt*/) { //m_iCcDpIpGTCnt는 GT빼고 하는 마지막 검사시에 Fail이여서 조정하는 경우는 Angle만 조정) {
                          if(dShift){
                              MT.GetUsrPara(miI,UsrPara) ;
                              MT.Vibration(miI , OM.DevOptn.dSfVibRng , UsrPara.dRunSpeed , OM.DevOptn.iSfVibCnt , true ) ;
                              //MT.GoIncRun(miI, dShift ) ;
                          }
                      }
                  }


                  Step.iCycle++;
                  return CNT;


        case  61: bR1 = bR2 = bR3 = true ;
                  //Angle
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {
                          if(dAngleX) {
                              MT.GetUsrPara(miY,UsrPara) ;
                              bR1 = MT.Vibration(miY , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngYVibCnt , false );
                          }
                          if(dAngleY) {
                              MT.GetUsrPara(miZ,UsrPara) ;
                              bR2 = MT.Vibration(miZ , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngZVibCnt , false );
                          }
                      }
                  }

                  //GT
                  if(OM.DevOptn.bUseShiftCal&& !iAngleNG ) {
                      if(bShiftNG/*&& !m_iCcDpIpGTCnt*/) { //m_iCcDpIpGTCnt는 GT빼고 하는 마지막 검사시에 Fail이여서 조정하는 경우는 Angle만 조정
                          if(dShift){
                              MT.GetUsrPara(miI,UsrPara) ;
                              bR3 = MT.Vibration(miI , OM.DevOptn.dSfVibRng , UsrPara.dRunSpeed , OM.DevOptn.iSfVibCnt , false ) ;
                          }
                      }
                  }

                  if(!bR1 || !bR2 || !bR3) return CNT ;

                  //LOG.ServerLogData.LD_Motor_Y  = MT.GetCmdPos(miY) ;
                  //LOG.ServerLogData.LD_MOtor_Z  = MT.GetCmdPos(miZ) ;
                  //LOG.ServerLogData.GTX_Motor_X = MT.GetCmdPos(miI) ; //요거 이름 바꿔야 함.서버 담당자랑 같이 해야함.나중에 디벅할일있을때.  GTX_Motor_X -> GTX_Motor_Y
                  LOG.Data.ReAngleX = dRowAngleX  ;
                  LOG.Data.ReAngleY = dRowAngleY  ;
                  LOG.Data.ReShift  = dRowShift   ;
                  LOG.Data.ReAPC    = dRowHorGap  ;

                  if((iAngleNG && OM.DevOptn.bUseAngleCal) ||
                     (bShiftNG && OM.DevOptn.bUseShiftCal /*&& !m_iCcDpIpGTCnt*/) ){
                     //(bHorNG   && OM.DevOptn.bUseAPCCal  )){

                      LOG.Data.ReResult = "3_NG"      ;

                           if(iAngleNG)SetServerData(imAll , "NG" , "Angle"    , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),MT.GetCmdPos(miI)) ;
                      else if(bShiftNG)SetServerData(imAll , "NG" , "ShiftGT"  , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),MT.GetCmdPos(miI)) ;

                      if(m_iCcDpIpInspCnt < OM.DevOptn.iMaxInspCnt) {
                          m_TempTimer.Clear() ;
                          Step.iCycle -= 4 ;
                          return CNT ;
                      }
                      else {
                          EM.SetErr(eiVisnInspFail) ;           //검사 횟수 초과.
                          FrmMain -> OnSendVisnMsg(mvTest) ;

                          return ERR ;

                      }
                  }

                  LOG.Data.ReResult = "3_OK" ;
                  SetServerData(imAll , "OK" , "None" , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),MT.GetCmdPos(miI)) ;

                       if(dShift < 0) MT.GoIncRun(miI ,  OM.DevOptn.dGTUpOffset) ;
                  else if(dShift > 0) MT.GoIncRun(miI , -OM.DevOptn.dGTUpOffset) ;

                 // MoveActr(aiGtCal , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  62: if(!MT.GetStopInpos(miI))return CNT ;
                  MoveActr(aiGtCal , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  63: if(!MoveActr(aiGtCal , ccFwd)) return CNT ;

                  //자꾸 검사 페일 났을때 왔다갔다 그래서 바꿈. 2012.06.11 선계원 ,  광일이 요청.
                  //MoveMotr(miI , pi_I_Wait);
                  Step.iCycle++;
                  return CNT ;





 //=======================================================================
                  //여기서 검사 패턴. 입력. 2012.01.30
        case  64: m_iCcDpIpTMInspCnt = 0 ;
                  Step.iCycle++;
                  return CNT ;

        case  65: if(MoveActr(aiDp1UD , ccBwd) && MoveActr(aiDp2UD , ccBwd)) {
                      MoveActr(aiDp1IO , ccBwd);
                      MoveActr(aiDp2IO , ccBwd);
                  }


                  bRcvdRslt = false ;

                  FrmMain -> OnSendVisnMsg(mv3Insp) ;
                  m_TempTimer.Clear() ;

                  Step.iCycle++;
                  return CNT;

        case  66: if(m_TempTimer.OnDelay(true , 500)&& m_iCcDpIpTMInspCnt < 3  ){  //아 안습... 가끔 응답이 없는 경우 있다.
                      m_iCcDpIpTMInspCnt++;
                      Step.iCycle -- ;
                      return CNT ;
                  }
                  if(!bRcvdRslt) return CNT ;   //eiVisnInspFail   iInspDelay

                  LOG.Data.ReAngleX = dRowAngleX  ;
                  LOG.Data.ReAngleY = dRowAngleY  ;
                  LOG.Data.ReShift  = dRowShift   ;
                  LOG.Data.ReAPC    = dRowHorGap  ;

                  if(iAngleNG || bShiftNG) {  //앵글만 검사하여 재 조정 여부 결정 이였는데 bShifNG도 보게함 2012.05.13 김극요청.
                      LOG.Data.ReResult = "4_NG"      ;
                      SetServerData(imAll , "NG" , "Angle"    , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),MT.GetCmdPos(miI)) ;

                      if(m_iCcDpIpGTCnt < OM.DevOptn.iGT3rdInspCnt) {
                          m_iCcDpIpInspCnt = 0 ;
                          m_iCcDpIpGTCnt++;
                          Step.iCycle = 50 ;
                          return CNT;
                      }
                      else {
                          EM.SetErr(eiVisnInspFail) ;
                          FrmMain -> OnSendVisnMsg(mvTest) ;
                          return ERR ;
                      }
                  }

                  LOG.Data.ReResult = "4_OK" ;
                  SetServerData(imAll , "OK" , "None" , MT.GetCmdPos(miY),MT.GetCmdPos(miZ),MT.GetCmdPos(miI)) ;

                  MoveMotr(miI , pi_I_Wait);
                  Step.iCycle++;
                  return CNT;

        case  67: if(!MoveMotr(miI , pi_I_Wait)) return CNT ;
                  Step.iCycle = 90 ;
                  return CNT;


//===========================================================




















        //마무리 동작.
        case  90 :if(!MoveActr(aiDp1UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp2UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiUvUD  , ccFwd)) return CNT ;
                  MoveActr(aiDp1IO , ccBwd);
                  MoveActr(aiDp2IO , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  91: if(!MoveActr(aiDp1IO , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp2IO , ccBwd)) return CNT ;

                  MoveActr(aiPadUD , ccFwd);
                  //if(!Rqst.bDispCycle)MoveActr(aiUvFB  , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  92: //if(!MoveActr(aiPadUD , ccFwd)) return CNT ;
                  FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  Step.iCycle = 0 ;
                  return END ;
    }


}

int TWorkTable::CycleDisp(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 15000 )) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        IO.SetY(yHdStart,false );
        Stat.bDispensing = false ;
        Step.iCycle = 0 ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool bR1 , bR2 , bR3 , bR4 , bR5 , bR6 ;
    int  bR11, bR22, bR33, bR44, bR55, bR66  ;
    int iTemp= 0 ;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;

        case  10: MoveActr(aiLdIO   , ccFwd ) ;
                  Step.iCycle++;
                  return CNT;

        case  11: if(!MoveActr(aiLdIO   , ccFwd )) return CNT ;
                  MoveActr(aiDp1UD , ccBwd);
                  MoveActr(aiDp2UD , ccBwd);
                  MoveActr(aiLdCal , ccBwd);
                  IO.SetY(yHdStart,false ); //UV Init.
                  Step.iCycle++;
                  return CNT;

        case  12: if(!MoveActr(aiLdCal , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp1UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp2UD , ccBwd)) return CNT ;
                  MoveActr(aiGtCal , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  13: if(!MoveActr(aiGtCal , ccFwd)) return CNT ;
                  Step.iCycle++;
                  return CNT ;

        case  14: MoveMotr(miI,pi_I_Wait ) ;

                  if(!OM.DevOptn.bInspBefrDisp || OM.DevOptn.bDispFirst) {
                      MoveMotr(miY , pi_Y_Disp) ;
                      MoveMotr(miZ , pi_Z_Disp) ;
                  }


                  Step.iCycle++;
                  return CNT;

        case  15: if(!MoveMotr(miI,pi_I_Wait ))return CNT ;

                  if(!OM.DevOptn.bInspBefrDisp || OM.DevOptn.bDispFirst) {
                      if(!MoveMotr(miY , pi_Y_Disp)) return CNT ;
                      if(!MoveMotr(miZ , pi_Z_Disp)) return CNT ;
                  }



                  MoveActr(aiPadUD , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  16: if(!MoveActr(aiPadUD , ccBwd)) return CNT ;
                  MoveActr(aiDp1IO , ccFwd);
                  MoveActr(aiDp2IO , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  17: if(!MoveActr(aiDp1IO , ccFwd)) return CNT ;
                  if(!MoveActr(aiDp2IO , ccFwd)) return CNT ;
                  MoveActr(aiDp1UD , ccFwd);
                  MoveActr(aiDp2UD , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  18: if(!MoveActr(aiDp1UD , ccFwd)) return CNT ;
                  if(!MoveActr(aiDp2UD , ccFwd)) return CNT ;

                  //Dispensing pattern
                  m_iCcDispDispCnt = 0 ;
                  m_TempTimer.Clear() ;
                  Step.iCycle++;
                  return CNT ;

        case  19: if(!m_TempTimer.OnDelay(true , 100)) return CNT ;
                  Step.iCycle++;
                  return CNT ;

        case  20: Stat.bDispensing = true ;
                  if(!OM.DevOptn.bIgnrDisp) {
                      IO.SetY(ySol1 , iOptnDisp[0][m_iCcDispDispCnt]);
                      IO.SetY(ySol2 , iOptnDisp[1][m_iCcDispDispCnt]);
                      IO.SetY(ySol3 , iOptnDisp[2][m_iCcDispDispCnt]);
                      IO.SetY(ySol4 , iOptnDisp[3][m_iCcDispDispCnt]);
                      IO.SetY(ySol5 , iOptnDisp[4][m_iCcDispDispCnt]);
                      IO.SetY(ySol6 , iOptnDisp[5][m_iCcDispDispCnt]);
                  }
                  m_DispTimer1.Clear() ;
                  m_DispTimer2.Clear() ;
                  m_DispTimer3.Clear() ;
                  m_DispTimer4.Clear() ;
                  m_DispTimer5.Clear() ;
                  m_DispTimer6.Clear() ;

                  Step.iCycle++;
                  return CNT;

        case  21: //for(int i = 0 ; i < MAX_DISP_CNT ; i++) {
                  //    if(iTemp < iOptnDisp[5][iDispCnt]) iTemp = iOptnDisp[5][iDispCnt] ;
                  //    if(iTemp < iOptnDisp[5][iDispCnt]) iTemp = iOptnDisp[5][iDispCnt] ;
                  //    if(iTemp < iOptnDisp[5][iDispCnt]) iTemp = iOptnDisp[5][iDispCnt] ;
                  //    if(iTemp < iOptnDisp[5][iDispCnt]) iTemp = iOptnDisp[5][iDispCnt] ;
                  //    if(iTemp < iOptnDisp[5][iDispCnt]) iTemp = iOptnDisp[5][iDispCnt] ;
                  //}

                  bR11 = iOptnDisp[0][m_iCcDispDispCnt] ;
                  bR22 = iOptnDisp[1][m_iCcDispDispCnt] ;
                  bR33 = iOptnDisp[2][m_iCcDispDispCnt] ;
                  bR44 = iOptnDisp[3][m_iCcDispDispCnt] ;
                  bR55 = iOptnDisp[4][m_iCcDispDispCnt] ;
                  bR66 = iOptnDisp[5][m_iCcDispDispCnt] ;


                  bR1 = m_DispTimer1.OnDelay(true , iOptnDisp[0][m_iCcDispDispCnt]);
                  bR2 = m_DispTimer2.OnDelay(true , iOptnDisp[1][m_iCcDispDispCnt]);
                  bR3 = m_DispTimer3.OnDelay(true , iOptnDisp[2][m_iCcDispDispCnt]);
                  bR4 = m_DispTimer4.OnDelay(true , iOptnDisp[3][m_iCcDispDispCnt]);
                  bR5 = m_DispTimer5.OnDelay(true , iOptnDisp[4][m_iCcDispDispCnt]);
                  bR6 = m_DispTimer6.OnDelay(true , iOptnDisp[5][m_iCcDispDispCnt]);

                  if(bR1)IO.SetY(ySol1 , false);
                  if(bR2)IO.SetY(ySol2 , false);
                  if(bR3)IO.SetY(ySol3 , false);
                  if(bR4)IO.SetY(ySol4 , false);
                  if(bR5)IO.SetY(ySol5 , false);
                  if(bR6)IO.SetY(ySol6 , false);

                  if(!bR1 || !bR2 || !bR3 || !bR4 || !bR5 || !bR6 ) return CNT ;

                  m_iCcDispDispCnt++ ;
                  if(m_iCcDispDispCnt < MAX_DISP_CNT) {
                      Step.iCycle -= 2;
                      m_TempTimer.Clear() ;
                      return CNT ;
                  }
                  m_TempTimer.Clear() ;
                  Step.iCycle++ ;
                  return CNT ;

        case  22: if(!m_TempTimer.OnDelay(true , OM.DevOptn.iDispEndDelay))return CNT ;
                  Stat.bDispensing = false ;
                  MoveActr(aiDp1UD , ccBwd);
                  MoveActr(aiDp2UD , ccBwd);
                  MoveActr(aiUvUD  , ccFwd);
                  m_TempTimer.Clear() ;

                  //IO.SetY(yVac1 , true);
                  //IO.SetY(yVac2 , true);
                  //IO.SetY(yVac3 , true);
                  //IO.SetY(yVac4 , true);
                  //IO.SetY(yVac5 , true);
                  //IO.SetY(yVac6 , true);

                  Step.iCycle++;
                  return CNT ;

        case  23:
                  if(!MoveActr(aiDp1UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp2UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiUvUD  , ccFwd)) return CNT ;
                  MoveActr(aiDp1IO , ccBwd);
                  MoveActr(aiDp2IO , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  24: if(!MoveActr(aiDp1IO , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp2IO , ccBwd)) return CNT ;

                  MoveActr(aiPadUD , ccFwd);
                  //if(!Rqst.bDispCycle)MoveActr(aiUvFB  , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  25: //속도 업을 위해 if(!MoveActr(aiPadUD , ccFwd)) return CNT ;
                  //FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  Step.iCycle = 0 ;
                  return END ;
        }
}

int TWorkTable::CyclePickup(void)
{

    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 5000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return ERR;
    }

    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;
                                                              //  aiLDP_FB  : 로더 푸셔
        case  10: MoveActr(aiMain , ccBwd);
                  if(OM.DevOptn.bUseHomming && MT.GetHomeEnd(miI)) {
                      MT.DoHome(miI) ;
                      Trace("Homming","while Disp");
                  }
                  Step.iCycle++;
                  return CNT;

        case  11: if(!MoveActr(aiMain , ccBwd)) return CNT ;
                  MoveActr(aiSub     , ccBwd) ;
                  MoveActr(aiSupport , ccBwd) ;
                  Step.iCycle++;
                  return CNT;

        case  12: if(!MoveActr(aiSub     , ccBwd)) return CNT ;
                  if(!MoveActr(aiSupport , ccBwd)) return CNT ;
                  Step.iCycle = 0 ;
                  Trace("CyclePickup","End");
                  return END ;
    }
}

int TWorkTable::CycleUnPickup(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 5000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;
                                                              //  aiLDP_FB  : 로더 푸셔
        case  10: //MoveActr(aiSupport , ccBwd) ;
                  MoveActr(aiSub     , ccFwd) ;
                  Step.iCycle ++ ;
                  return CNT ;

        case  11: //if(!MoveActr(aiSupport , ccBwd)) return CNT ;
                  if(!MoveActr(aiSub     , ccFwd)) return CNT ;
                  MoveActr(aiMain , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  12: if(!MoveActr(aiMain , ccFwd)) return CNT ;
                  Step.iCycle = 0 ;
                  return END ;
    }
}

int TWorkTable::CycleUpDn(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 5000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());

        MoveActr(aiLdCal , ccFwd) ;

        Step.iCycle = 0 ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {
        default : Step.iCycle = 0;
                  return END;
                                                              //  aiLDP_FB  : 로더 푸셔
        case  10: //MoveMotr(miI , pi_I_Wait ) ;
                  MoveMotr(miZ , pi_Z_Wait ) ;
                  MoveMotr(miY , pi_Y_Wait ) ;
                  MoveActr(aiLdIO , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  11: if(!MoveActr(aiLdIO , ccFwd ))return CNT ;
                  //if(!MoveMotr(miI , pi_I_Wait))return CNT ;
                  if(!MoveMotr(miZ , pi_Z_Wait))return CNT ;
                  if(!MoveMotr(miY , pi_Y_Wait))return CNT ;

                  MoveActr(aiLdCal , ccBwd);
                  Step.iCycle++;
                  return CNT;

        case  12: if(!MoveActr(aiLdCal , ccBwd)) return CNT ;
                  Step.iCycle = 0 ;
                  Trace("CycleUpDn","End");
                  return END ;
    }

}

int TWorkTable::CycleUnUpDn (void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 5000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return CNT;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;
     /*

    MoveActr(aiDp1UD,ccBwd ) ;
    MoveActr(aiDp2UD,ccBwd ) ;
    MoveActr(aiDp1UD,ccBwd ) ;
    MoveActr(aiDp1UD,ccBwd ) ;
    MoveActr(aiDp1UD,ccBwd ) ;
    MoveActr(aiDp1UD,ccBwd ) ;     */

    //Cycle.
    switch (Step.iCycle) {
        default : Step.iCycle = 0;
                  return END;

        case  10: MoveActr(aiLdCal , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  11: if(!MoveActr(aiLdCal , ccFwd)) return CNT ;
                  MoveActr(aiLdIO , ccBwd);
                  MoveMotr(miI , pi_I_Wait ) ;
                  MoveMotr(miZ , pi_Z_Wait ) ;
                  MoveMotr(miY , pi_Y_Wait ) ;
                  Step.iCycle++;
                  return CNT;

        case  12: if(!MoveActr(aiLdIO , ccBwd)) return CNT ;
                  if(!MoveMotr(miI , pi_I_Wait )) return CNT ;
                  if(!MoveMotr(miZ , pi_Z_Wait )) return CNT ;
                  if(!MoveMotr(miY , pi_Y_Wait )) return CNT ;

                  Step.iCycle = 0 ;
                  return END ;
    }
}

int TWorkTable::CycleVac(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 5000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {
        default : Step.iCycle = 0;
                  return END;

        case  10: MoveActr(aiPcbCmp , ccBwd) ;
                  Step.iCycle++;
                  return CNT;

        case  11: if(!MoveActr(aiPcbCmp , ccBwd)) return CNT ;
                  IO.SetY(ySuckOn , true) ;
                  Step.iCycle = 0 ;
                  return END ;
    }
}


int TWorkTable::CycleUnVac(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 5000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {
        default : Step.iCycle = 0;
                  return END;
                                                              //  aiLDP_FB  : 로더 푸셔
        case  10: //FrmMain -> OnSendVisnMsg(mvAuto) ;
                  m_TempTimer.Clear();
                  Step.iCycle++;
                  return CNT ;

        case  11: if(!m_TempTimer.OnDelay(true , 100))return CNT ;
                  m_TempTimer.Clear();
                  Step.iCycle++;
                  return CNT ;

        case  12: if(!m_TempTimer.OnDelay(true , 500))return CNT ;
                  MoveActr(aiPcbCmp , ccFwd) ;
                  Step.iCycle++;
                  return CNT;

        case  13: if(!MoveActr(aiPcbCmp , ccFwd)) return CNT ;
                  IO.SetY(ySuckOn , false) ;
                  Step.iCycle = 0 ;
                  return END ;
    }
}


int TWorkTable::CycleChuck(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 5000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return ERR;
    }

    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {
        default : Step.iCycle = 0;
                  return END;
                                                              //  aiLDP_FB  : 로더 푸셔
        case  10: MoveActr(aiGp1 , ccFwd);
                  //MoveMotr(miI , pi_I_Wait ) ; //요거 OM.DevOptn.bUseHomming시에 CycleUpDn에서 렉 발생해서 이쪽으로 옮김.
                  Step.iCycle++;
                  return CNT;

        case  11: if(!MoveActr(aiGp1 , ccFwd)) return CNT ;
                  //if(!MoveMotr(miI , pi_I_Wait )) return CNT ;
                  MoveActr(aiGp1 , ccBwd);
                  Step.iCycle++;
                  return CNT;

        case  12: if(!MoveActr(aiGp1 , ccBwd)) return CNT ;
                  MoveActr(aiGp1 , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  13: if(!MoveActr(aiGp1 , ccFwd)) return CNT ;
                  m_TempTimer.Clear();
                  Step.iCycle++;
                  return CNT;

        case  14: if(!m_TempTimer.OnDelay(true , OM.DevOptn.iSptrDnDelay)) return CNT ;
                  IO.SetY (ySuckOn  , false );
                  MoveActr(aiSupport , ccFwd );
                  Step.iCycle++;
                  return CNT;

        case  15: if(!MoveActr(aiSupport , ccFwd)) return CNT ;
                  MoveMotr(miI , pi_I_Wait ) ; //요거 OM.DevOptn.bUseHomming시에 CycleUpDn에서 렉 발생해서 이쪽으로 옮김.
                  m_bInspMaxErr = false ; // 본딩전 검사에 검사 횟수 오버 에러시에 리셋스타트 하면 그자리에서 작업 다시시작되게..//20120726 김극->이상희->선계원 요청.
                  Step.iCycle = 0 ;
                  return END ;
    }
}

int TWorkTable::CycleUnChuck (void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 5000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return ERR;
    }

    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {
        default : Step.iCycle = 0;
                  return END;

        case  10: MoveActr(aiSupport , ccBwd) ;
                  Step.iCycle++ ;
                  return CNT ;

        case  11: if(!MoveActr(aiSupport , ccBwd)) return CNT ;
                  IO.SetY (ySuckOn  , true );
                  m_TempTimer.Clear();
                  Step.iCycle++ ;
                  return CNT ;

        case  12: if(!m_TempTimer.OnDelay(true , OM.DevOptn.iSptrDnDelay ))return CNT ;
                  MoveActr(aiGp1 , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  13: if(!MoveActr(aiGp1 , ccBwd)) return CNT ;
                  Step.iCycle = 0 ;
                  return END ;
    }
}

int TWorkTable::CycleReInsp(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle ,Step.iCycle == 11 ? 90000 : 10000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        FrmMain -> OnSendAPCMsg("S1","6"); //Off
        MoveActr(aiGtCal,ccFwd);
        Step.iCycle = 0 ;
        //bUseAPC = false ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool bR1 , bR2 , bR3 ;
    CAxtFS20::CUsrPara UsrPara ;

    double dOffsetCntX ;
    double dOffsetCntY ;

    double dOffSetPosX ;
    double dOffSetPosY ;

    double dTemp ;


    //static iTMInspCnt = 0 ;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;
                                                              //  aiLDP_FB  : 로더 푸셔
        case  10: //if(bUseAPC) return CNT ;
                  bUseAPC = true ;
                  MoveActr(aiGtCal,ccFwd);
                  if(OM.DevOptn.bGrabMode) FrmMain -> OnSendVisnMsg(mvAuto) ;
                  else                     FrmMain -> OnSendVisnMsg(mvTest) ;

                  FrmMain -> OnSendAPCMsg("S1","7"); //Auto

                  //FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  //if(OM.DevOptn.bGrabMode) FrmMain -> OnSendAPCMsg("S1","6"); //Off

                  //iTMInspCnt = 0 ;

                  Step.iCycle++;
                  return CNT;

        case  11: FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  Step.iCycle++;
                  return CNT;

        case  12: FrmMain -> OnSendAPCMsg("S2","3", AnsiString(OM.DevOptn.dAPCSetVal).c_str()); //APC POWER SETTING
                  Step.iCycle++;
                  return CNT;

        case  13: if(m_iPartId == piLT) FrmMain -> OnSendAPCMsg("S1","3"); //1P/U
                  if(m_iPartId == piRT) FrmMain -> OnSendAPCMsg("S1","4"); //2P/U
                  Step.iCycle++;
                  return CNT;

        case  14: //FrmMain -> OnSendAPCMsg("S2","3", AnsiString(OM.DevOptn.dAPCSetVal).c_str()); //APC POWER SETTING
                  if(OM.DevOptn.bROMType) FrmMain -> OnSendAPCMsg("S1","2"); //R/W
                  else                    FrmMain -> OnSendAPCMsg("S1","1"); //R/W
                  Step.iCycle++;
                  return CNT;

        case  15: FrmMain -> OnSendAPCMsg("S1","5"); //On
                  MoveActr(aiGtCal,ccFwd);
                  m_TempTimer2.Clear();
                  Step.iCycle++;
                  return CNT;

        case  16: if(!MoveActr(aiGtCal,ccFwd)) return CNT ;

                  if(OM.DevOptn.bInspBefrDisp){
                      MoveMotr(miI,pi_I_Wait );
                  }
                  else {
                      MoveMotr(miI,pi_I_Insp );
                      MoveMotr(miY,pi_Y_Insp );
                      MoveMotr(miZ,pi_Z_Insp );
                  }

                  Step.iCycle++;
                  return CNT;

        case  17: if( OM.DevOptn.bInspBefrDisp) {
                      if(!MoveMotr(miI,pi_I_Wait ))return CNT ;
                  }
                  else {
                      if(!MoveMotr(miI,pi_I_Insp ))return CNT ;
                      if(!MoveMotr(miY,pi_Y_Insp ))return CNT ;
                      if(!MoveMotr(miZ,pi_Z_Insp ))return CNT ;
                  }

                  if(!OM.DevOptn.bInspBefrDisp) MoveActr(aiGtCal,ccBwd);

                  Step.iCycle++;
                  return CNT;

        case  18: if(!OM.DevOptn.bInspBefrDisp) {
                      if(!MoveActr(aiGtCal,ccBwd))return CNT ;
                      MT.GetUsrPara(miI,UsrPara) ;
                      MT.Vibration(miI , OM.DevOptn.dGTDnVibRng , UsrPara.dRunSpeed , OM.DevOptn.iGTDnVibCnt , true ) ;
                  }
                  Step.iCycle++;
                  return CNT ;

        case  19: if(!OM.DevOptn.bInspBefrDisp) {
                      MT.GetUsrPara(miI,UsrPara) ;
                      if(!MT.Vibration(miI , OM.DevOptn.dGTDnVibRng , UsrPara.dRunSpeed , OM.DevOptn.iGTDnVibCnt,false))return CNT ;
                  }
                  m_TempTimer.Clear() ;
                  m_iCcReIpInspCnt = 0 ;
                  Step.iCycle++;
                  return CNT;

        case  20: if(!m_TempTimer .OnDelay(true , OM.DevOptn.iInspDelay )) return CNT ;
                  //if(!m_TempTimer2.OnDelay(true , 3500)) return CNT ;
                  if(!m_TempTimer2.OnDelay(true , OM.DevOptn.iAPCOnDelay)) return CNT ;
                  m_iCcReIpInspCnt++;
                  Trace("Vision Re Inspection",("Vision Cnt = " + AnsiString(m_iCcReIpInspCnt)).c_str());
                  Step.iCycle++;
                  return CNT;

        case  21: bRcvdRslt = false ;
                  m_dCcReIpGTOriPos = MT.GetCmdPos(miI) ;
                  FrmMain -> OnSendVisnMsg(mvInsp) ;

                  Step.iCycle++;
                  return CNT;

        case  22: if(!bRcvdRslt) return CNT ;   //eiVisnInspFail   iInspDelay
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {          //초기 값에서 차이를 구해야 한다. 기구 문제로 조정 안되는 상태면 밑도끝도 없이 감.

                          //dOffsetCntX = (dAngleXRow + dAngleXHigh) /2.0 ;
                          //dOffsetCntY = (dAngleYRow + dAngleYHigh) /2.0 ;
                          //
                          //dOffSetPosX = dAngleX + dOffsetCntX ;
                          //dOffSetPosY = dAngleY + dOffsetCntY ;

                          dTemp = fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ;
                          dTemp = fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ;


                          if(OM.DevOptn.dMaxYPos < fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ) {EM.SetErr  (eiVisnRangOver); return ERR ; }
                          else                                                                                    {MT.GoIncRun(miY, dAngleX  );              }

                          if(OM.DevOptn.dMaxZPos < fabs(PM.GetPos(miZ,pi_Z_Insp)-(dAngleY + MT.GetCmdPos(miZ))) ) {EM.SetErr  (eiVisnRangOver); return ERR ; } //Err
                          else                                                                                    {MT.GoIncRun(miZ, dAngleY  );              }

                          /*
                          if(OM.DevOptn.dMaxYPos < fabs(PM.GetPos(miY,pi_Y_Insp)-(dOffSetPosX + MT.GetCmdPos(miY))) ) {EM.SetErr  (eiVisnRangOver  ); return ERR ; }
                          else                                                                                        {MT.GoIncRun(miY, dOffSetPosX);              }

                          if(OM.DevOptn.dMaxZPos < fabs(PM.GetPos(miZ,pi_Z_Insp)-(dOffSetPosY + MT.GetCmdPos(miZ))) ) {EM.SetErr  (eiVisnRangOver  ); return ERR ; } //Err
                          else                                                                                        {MT.GoIncRun(miZ, dOffSetPosY);              }
                          */



                          /*
                          if(OM.DevOptn.dMaxYPos < fabs(PM.GetPos(miY,pi_Y_Insp)-(dAngleX + MT.GetCmdPos(miY))) ) {EM.SetErr  (eiVisnRangOver); return ERR ; }
                          else                                                                                    {MT.GoIncRun(miY, dAngleX  );              }

                          if(OM.DevOptn.dMaxZPos < fabs(PM.GetPos(miZ,pi_Z_Insp)-(dAngleY + MT.GetCmdPos(miZ))) ) {EM.SetErr  (eiVisnRangOver); return ERR ; } //Err
                          else                                                                                    {MT.GoIncRun(miZ, dAngleY) ;               }   */
                      }
                  }

                  if(!OM.DevOptn.bInspBefrDisp) {
                      if(OM.DevOptn.bUseShiftCal && !iAngleNG && !bHorNG) {
                          if(bShiftNG) {           //초기 값에서 차이를 구해야 한다. 기구 문제로 조정 안되는 상태면 밑도끝도 없이 감.
                              if(OM.DevOptn.dMaxZPos < fabs(PM.GetPos(miI,pi_I_Insp)-(dShift + MT.GetCmdPos(miI))) ) {EM.SetErr(eiShiftRangOver); return ERR ; }
                              else                                                                                  {MT.GoIncRun(miI, dShift ) ;               }
                          }
                      }
                  }

                  if(OM.DevOptn.bUseAPCCal) {
                      if(bHorNG) {
                          if(OM.DevOptn.dAPCCalLim > fabs(dHorGap))
                              FrmMain -> OnSendAPCMsg("S2","3", dHorGap , true ); //APC POWER SETTING
                      }
                  }



                  Step.iCycle++;
                  return CNT;

        case  23: if(!MT.GetStopInpos(miZ))return CNT ;
                  if(!MT.GetStopInpos(miY))return CNT ;
                  if(!MT.GetStopInpos(miI))return CNT ;             

                  //Angle
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {
                          if(dAngleX) {
                              MT.GetUsrPara(miY,UsrPara) ;
                              MT.Vibration(miY , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngYVibCnt , true );
                              //MT.GoIncRun(miY, dAngleX) ;
                          }
                          if(dAngleY) {
                              MT.GetUsrPara(miZ,UsrPara) ;
                              MT.Vibration(miZ , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngZVibCnt , true );
                              //MT.GoIncRun(miZ, dAngleY) ;
                          }
                      }
                  }

                  //GT
                  if(!OM.DevOptn.bInspBefrDisp) {
                      if(OM.DevOptn.bUseShiftCal&& !iAngleNG && !bHorNG) {
                          if(bShiftNG) {
                              if(dShift){
                                  MT.GetUsrPara(miI,UsrPara) ;
                                  MT.Vibration(miI , OM.DevOptn.dSfVibRng , UsrPara.dRunSpeed , OM.DevOptn.iSfVibCnt , true ) ;
                                  //MT.GoIncRun(miI, dShift ) ;
                              }
                          }
                      }
                  }

                  Step.iCycle++;
                  return CNT;


        case  24: bR1 = bR2 = bR3 = true ;
                  //Angle
                  if(OM.DevOptn.bUseAngleCal) {
                      if(iAngleNG) {
                          if(dAngleX) {
                              MT.GetUsrPara(miY,UsrPara) ;
                              bR1 = MT.Vibration(miY , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngYVibCnt , false );
                          }
                          if(dAngleY) {
                              MT.GetUsrPara(miZ,UsrPara) ;
                              bR2 = MT.Vibration(miZ , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngZVibCnt , false );
                          }
                      }
                  }

                  //GT
                  if(!OM.DevOptn.bInspBefrDisp) {
                      if(OM.DevOptn.bUseShiftCal&& !iAngleNG && !bHorNG) {
                          if(bShiftNG) {
                              if(dShift){
                                  MT.GetUsrPara(miI,UsrPara) ;
                                  bR3 = MT.Vibration(miI , OM.DevOptn.dSfVibRng , UsrPara.dRunSpeed , OM.DevOptn.iSfVibCnt , false ) ;
                              }
                          }
                      }
                  }

                  if(!bR1 || !bR2 || !bR3) return CNT ;

                  if((iAngleNG && OM.DevOptn.bUseAngleCal) ||
                     (bHorNG   && OM.DevOptn.bUseAPCCal  )){
                      if(m_iCcReIpInspCnt < OM.DevOptn.iMaxInspCnt) {
                          m_TempTimer.Clear() ;
                          Step.iCycle -= 4 ;
                          return CNT ;
                      }
                      else {
                          EM.SetErr(eiVisnInspFail) ;
                          FrmMain -> OnSendVisnMsg(mvTest) ;
                          return ERR ;

                      }
                  }

                  LOG.Data.ReAngleX = dRowAngleX  ;
                  LOG.Data.ReAngleY = dRowAngleY  ;
                  LOG.Data.ReShift  = dRowShift   ;
                  LOG.Data.ReAPC    = dRowHorGap  ;

                  LOG.Data.ReAngleX   = dRowAngleX  ;
                  LOG.Data.ReAngleY   = dRowAngleY  ;
                  LOG.Data.ReShift    = dRowShift   ;
                  LOG.Data.ReAPC      = dRowHorGap  ;
                  LOG.Data.ReResult   = "OK"        ;







                  if(!OM.DevOptn.bInspBefrDisp) {
                           if(dShift < 0) MT.GoIncRun(miI ,  OM.DevOptn.dGTUpOffset) ;
                      else if(dShift > 0) MT.GoIncRun(miI , -OM.DevOptn.dGTUpOffset) ;
                  }
                  Step.iCycle++;
                  return CNT;

        case  25: if(!MT.GetStopInpos(miI))return CNT ;
                  MoveActr(aiGtCal , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  26: if(!MoveActr(aiGtCal , ccFwd)) return CNT ;
                  //FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  /*(if(OM.DevOptn.bGrabMode)*/ FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  MoveMotr(miI,pi_I_Wait );
                  Step.iCycle++;
                  return CNT;

        case  27: if(!MoveMotr(miI,pi_I_Wait ))return CNT ;

                  //귀찮을 것 같아서.
                  //if(OM.DevOptn.bUseBeamRslt && bBeamNG)EM.SetErr(eiVisnBeam) ;
                  Step.iCycle = 0 ;
                  //bUseAPC = false ;
                  return END ;
        }
}

int TWorkTable::CycleUv(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 20000 )) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        IO.SetY(yHdStart,false );
        Step.iCycle = 0 ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool bR1 , bR2 , bR3 , bR4 , bR5 , bR6 ;
    int  bR11, bR22, bR33, bR44, bR55, bR66  ;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;

        case  10: MoveActr(aiUvUD  , ccFwd);
                  MoveMotr(miI , pi_I_Wait);
                  Step.iCycle++;
                  return CNT ;

        case  11: if(!MoveActr(aiUvUD , ccFwd)) return CNT ;
                  if(!MoveMotr(miI , pi_I_Wait)) return CNT ;
                  MoveActr(aiUvFB , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  12: if(!MoveActr(aiUvFB , ccFwd)) return CNT ;
                  MoveActr(aiUvUD  , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  13: if(!MoveActr(aiUvUD  , ccBwd)) return CNT ;
                  m_TempTimer.Clear() ;
                  IO.SetY(yHdStart,true );
                  IO.SetY (ySuckOn , false) ;
                  Step.iCycle++ ;
                  return CNT ;

        //바꿀때 타임아웃조건 확인.
        case  14: if(!m_TempTimer.OnDelay(true , 1000)) return CNT ;
                  if(!IO.GetX(xHdEnd))return CNT ;
                  IO.SetY(yHdStart,false );
                  MoveActr(aiGp1 , ccBwd);
                  MoveActr(aiUvUD  , ccFwd);

                  Step.iCycle++;
                  return CNT ;

        case  15: if(!MoveActr(aiGp1   , ccBwd)) return CNT ;
                  if(!MoveActr(aiUvUD  , ccFwd)) return CNT ;
                  MoveActr(aiUvFB   , ccBwd);
                  MoveActr(aiPcbCmp , ccFwd);
                  MoveActr(aiSub    , ccFwd);
                  MoveActr(aiLdCal  , ccFwd);

                  Step.iCycle++;
                  return CNT ;

        case  16: if(MoveActr(aiLdCal  , ccFwd))MoveActr(aiLdIO , ccBwd);

                  if(!MoveActr(aiUvFB   , ccBwd)) return CNT ;
                  if(!MoveActr(aiPcbCmp , ccFwd)) return CNT ;
                  if(!MoveActr(aiSub    , ccFwd)) return CNT ;
                  if(!MoveActr(aiLdCal  , ccFwd)) return CNT ;
                  MoveActr(aiMain   , ccFwd);

                  Step.iCycle++;
                  return CNT ;

        case  17: if(!MoveActr(aiMain   , ccFwd)) return CNT ;
                  MoveActr(aiLdIO , ccBwd);
                  MoveMotr(miI , pi_I_Wait ) ;
                  MoveMotr(miZ , pi_Z_Wait ) ;
                  MoveMotr(miY , pi_Y_Wait ) ;
                  Step.iCycle++ ;
                  return CNT ;

        case  18: if(!MoveActr(aiLdIO , ccBwd  )) return CNT ;
                  if(!MoveMotr(miI , pi_I_Wait )) return CNT ;
                  if(!MoveMotr(miZ , pi_Z_Wait )) return CNT ;
                  if(!MoveMotr(miY , pi_Y_Wait )) return CNT ;
                  m_TempTimer.Clear();
                  Step.iCycle++;
                  return CNT;

        case  19: if(!m_TempTimer.OnDelay(true,OM.DevOptn.iSptrEndUpDelay)) return CNT ;
                  MoveActr(aiSupport , ccBwd);

                  Step.iCycle++;
                  return CNT;

        case  20: if(!MoveActr(aiSupport , ccBwd)) return false ;
                  if(!Stat.bPosReset) { //혹시 중간에 포지션 리셑이 일어나지 않았으면   현재 21번 부터 의 스텝은 사용하지 않는다.
                      Step.iCycle = 0 ; //원래는 자제가 삐꾸일때 서보 플어서 세팅 하고 쓰면 작업 종료후 21번 스텝 타서. 올홈 잡도록 해놓은 것인데... 안씀....
                      return END ;
                  }
                  Step.iCycle++;
                  return CNT;

        case  21: Step.iCycle++;
                  return CNT;

        case  22: Step.iCycle++ ;
                  return CNT ;

                  //여기부턴 모터 포지션 중간에 메뉴얼 조정 했을때  안쓴다.
        case  23: if(!MT.Home(miI)) return CNT ;
                 Step.iCycle++ ;
                 return CNT ;

        case  24: if(!MT.Home(miY)) return CNT ;
                  Step.iCycle++ ;
                  return CNT ;

        case  25: if(!MT.Home(miZ)) return CNT ;
                  //MoveActr(aiMain  , ccFwd );
                  Step.iCycle++ ;
                  return CNT ;

        case  26: //if(!MoveActr(aiMain , ccFwd )) return CNT ;

                  MoveMotr(miI , pi_I_Wait) ;
                  MoveMotr(miZ , pi_Z_Wait) ;
                  MoveMotr(miY , pi_Y_Wait) ;

                  Step.iCycle ++ ;
                  return CNT ;

        case  27: if(!MoveMotr(miI , pi_I_Wait))return CNT ;     //여기가 노멀 종료가 아니다. 22번 스텝 확인.
                  if(!MoveMotr(miZ , pi_Z_Wait))return CNT ;
                  if(!MoveMotr(miY , pi_Y_Wait))return CNT ;

                  Step.iCycle = 0;
                  return END ;
    }
}

int TWorkTable::CycleRelease(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 20000 )) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        IO.SetY(yHdStart,false );
        Step.iCycle = 0 ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool bR1 , bR2 , bR3 , bR4 , bR5 , bR6 ;
    int  bR11, bR22, bR33, bR44, bR55, bR66  ;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;

        case  10: MoveActr(aiGp1 , ccBwd);
                  MoveActr(aiUvUD , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  11: if(!MoveActr(aiGp1   , ccBwd)) return CNT ;
                  if(!MoveActr(aiUvUD  , ccFwd)) return CNT ;
                  MoveActr(aiUvFB   , ccBwd);
                  MoveActr(aiPcbCmp , ccFwd);
                  MoveActr(aiSub    , ccFwd);
                  MoveActr(aiLdCal  , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  12: if(MoveActr(aiLdCal  , ccFwd))MoveActr(aiLdIO , ccBwd);

                  if(!MoveActr(aiUvFB   , ccBwd)) return CNT ;
                  if(!MoveActr(aiPcbCmp , ccFwd)) return CNT ;
                  if(!MoveActr(aiSub    , ccFwd)) return CNT ;
                  if(!MoveActr(aiLdCal  , ccFwd)) return CNT ;

                  MoveActr(aiMain   , ccFwd);

                  Step.iCycle++ ;
                  return CNT ;

        case  13: if(!MoveActr(aiMain   , ccFwd)) return CNT ;
                  MoveActr(aiLdIO , ccBwd);
                  MoveMotr(miI , pi_I_Wait ) ;
                  MoveMotr(miZ , pi_Z_Wait ) ;
                  MoveMotr(miY , pi_Y_Wait ) ;
                  MoveActr(aiSupport , ccBwd) ;

                  Step.iCycle++;
                  return CNT;

        case  14: if(!MoveActr(aiLdIO , ccBwd   )) return CNT ;
                  if(!MoveMotr(miI , pi_I_Wait  )) return CNT ;
                  if(!MoveMotr(miZ , pi_Z_Wait  )) return CNT ;
                  if(!MoveMotr(miY , pi_Y_Wait  )) return CNT ;
                  if(!MoveActr(aiSupport , ccBwd)) return CNT ;

                  Step.iCycle = 0 ;
                  return END ;
        }


}

int TWorkTable::CycleDispOnly(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 15000 )) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        FrmMain -> OnSendAPCMsg("S1","6"); //Off
        IO.SetY(yHdStart,false );
        Step.iCycle = 0 ;
        return ERR;
    }


    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool bR1 , bR2 , bR3 , bR4 , bR5 , bR6 ;
    bool bR11, bR22, bR33, bR44, bR55, bR66;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;
                  
        case  10: //MoveActr(aiPadUD , ccBwd);
                  if(m_iCcDpOlSetDispCnt < 1) m_iCcDpOlSetDispCnt = 1 ;
                  m_iCcDpOlTtDispCnt = 0 ;
                  Step.iCycle++;
                  return CNT ;

        case  11: //if(!MoveActr(aiPadUD , ccBwd)) return CNT ;
                  m_iCcDpOlDispCnt = 0 ;




                  m_TempTimer.Clear() ;
                  Step.iCycle++;
                  return CNT ;

        case  12: if(!m_TempTimer.OnDelay(true , 100)) return CNT ;
                  Step.iCycle++;
                  return CNT ;

        case  13: if(Rqst.iDispNzl == -1) {
                      IO.SetY(ySol1 , iOptnDisp[0][m_iCcDpOlDispCnt]);
                      IO.SetY(ySol2 , iOptnDisp[1][m_iCcDpOlDispCnt]);
                      IO.SetY(ySol3 , iOptnDisp[2][m_iCcDpOlDispCnt]);
                      IO.SetY(ySol4 , iOptnDisp[3][m_iCcDpOlDispCnt]);
                      IO.SetY(ySol5 , iOptnDisp[4][m_iCcDpOlDispCnt]);
                      IO.SetY(ySol6 , iOptnDisp[5][m_iCcDpOlDispCnt]);
                  }
                  else {
                      IO.SetY(ySol1 + Rqst.iDispNzl , iOptnDisp[Rqst.iDispNzl][m_iCcDpOlDispCnt]);
                  }
                  m_DispTimer1.Clear() ;
                  m_DispTimer2.Clear() ;
                  m_DispTimer3.Clear() ;
                  m_DispTimer4.Clear() ;
                  m_DispTimer5.Clear() ;
                  m_DispTimer6.Clear() ;
                  Step.iCycle++;
                  return CNT;

        case  14: bR11 = iOptnDisp[0][m_iCcDpOlDispCnt] ;
                  bR22 = iOptnDisp[1][m_iCcDpOlDispCnt] ;
                  bR33 = iOptnDisp[2][m_iCcDpOlDispCnt] ;
                  bR44 = iOptnDisp[3][m_iCcDpOlDispCnt] ;
                  bR55 = iOptnDisp[4][m_iCcDpOlDispCnt] ;
                  bR66 = iOptnDisp[5][m_iCcDpOlDispCnt] ;

                  //bR1 = bR2 = bR3 = bR4 = bR5 = bR6 = true ;

                  if(Rqst.iDispNzl == -1) {
                      bR1 = m_DispTimer1.OnDelay(true , iOptnDisp[0][m_iCcDpOlDispCnt]);
                      bR2 = m_DispTimer2.OnDelay(true , iOptnDisp[1][m_iCcDpOlDispCnt]);
                      bR3 = m_DispTimer3.OnDelay(true , iOptnDisp[2][m_iCcDpOlDispCnt]);
                      bR4 = m_DispTimer4.OnDelay(true , iOptnDisp[3][m_iCcDpOlDispCnt]);
                      bR5 = m_DispTimer5.OnDelay(true , iOptnDisp[4][m_iCcDpOlDispCnt]);
                      bR6 = m_DispTimer6.OnDelay(true , iOptnDisp[5][m_iCcDpOlDispCnt]);
                  }
                  else {
                      bR1 = Rqst.iDispNzl == 0 ? m_DispTimer1.OnDelay(true , iOptnDisp[0][m_iCcDpOlDispCnt]) : true ;
                      bR2 = Rqst.iDispNzl == 1 ? m_DispTimer2.OnDelay(true , iOptnDisp[1][m_iCcDpOlDispCnt]) : true ;
                      bR3 = Rqst.iDispNzl == 2 ? m_DispTimer3.OnDelay(true , iOptnDisp[2][m_iCcDpOlDispCnt]) : true ;
                      bR4 = Rqst.iDispNzl == 3 ? m_DispTimer4.OnDelay(true , iOptnDisp[3][m_iCcDpOlDispCnt]) : true ;
                      bR5 = Rqst.iDispNzl == 4 ? m_DispTimer5.OnDelay(true , iOptnDisp[4][m_iCcDpOlDispCnt]) : true ;
                      bR6 = Rqst.iDispNzl == 5 ? m_DispTimer6.OnDelay(true , iOptnDisp[5][m_iCcDpOlDispCnt]) : true ;
                  }

                  if(bR1)IO.SetY(ySol1 , false);
                  if(bR2)IO.SetY(ySol2 , false);
                  if(bR3)IO.SetY(ySol3 , false);
                  if(bR4)IO.SetY(ySol4 , false);
                  if(bR5)IO.SetY(ySol5 , false);
                  if(bR6)IO.SetY(ySol6 , false);

                  if(!bR1 || !bR2 || !bR3 || !bR4 || !bR5 || !bR6 ) return CNT ;

                  m_iCcDpOlDispCnt++ ; //한번 토출에 반복횟수.
                  if(m_iCcDpOlDispCnt < MAX_DISP_CNT) {
                      Step.iCycle -= 2;
                      m_TempTimer.Clear() ;
                      return CNT ;
                  }

                  m_iCcDpOlTtDispCnt++; //전체 토출싸이클 반복 횟수.
                  if(m_iCcDpOlTtDispCnt < m_iCcDpOlSetDispCnt) {
                      Step.iCycle -= 3;
                      return CNT ;
                  }


                  m_TempTimer.Clear() ;
                  Step.iCycle++ ;
                  return CNT ;

        case  15: if(!m_TempTimer.OnDelay(true , OM.DevOptn.iDispEndDelay))return CNT ;
                  MoveActr(aiDp1UD , ccBwd);
                  MoveActr(aiDp2UD , ccBwd);
                  m_TempTimer.Clear() ;

                  //IO.SetY(yVac1 , true);
                  //IO.SetY(yVac2 , true);
                  //IO.SetY(yVac3 , true);
                  //IO.SetY(yVac4 , true);
                  //IO.SetY(yVac5 , true);
                  //IO.SetY(yVac6 , true);

                  Step.iCycle++;
                  return CNT ;

        case  16: //if(!m_TempTimer.OnDelay(true , OM.DevOptn.iSuckTime))return CNT ;
                  //IO.SetY(yVac1 , false);
                  //IO.SetY(yVac2 , false);
                  //IO.SetY(yVac3 , false);
                  //IO.SetY(yVac4 , false);
                  //IO.SetY(yVac5 , false);
                  //IO.SetY(yVac6 , false);

                  //MoveActr(aiPadUD , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  17: //if(!MoveActr(aiPadUD , ccFwd)) return CNT ;
                  Step.iCycle = 0 ;
                  return END ;
    }
}

int TWorkTable::CycleDataCheck(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 5000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return ERR;
    }

    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;
                                                              //  aiLDP_FB  : 로더 푸셔
        case  10: MoveActr(aiSupport , ccFwd); //올라와 있으면 이미 본딩된 것들이라 안됌...
                  Step.iCycle++;
                  return CNT;

        case  11: if(!MoveActr(aiSupport , ccFwd)) return false ;
                  MoveActr(aiMain , ccBwd);
                  FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  Step.iCycle++;
                  return CNT;

        case  12: if(!MoveActr(aiMain , ccBwd)) return CNT ;
                  FrmMain -> OnSendAPCMsg("S1","7"); //Auto
                  FrmMain -> OnSendVisnMsg(mvTest) ; //비젼 메뉴얼 모드 전환.
                  MoveActr(aiSub    , ccBwd) ;
                  MoveActr(aiPcbCmp , ccBwd) ;
                  Step.iCycle++;
                  return CNT;

        case  13: if(!MoveActr(aiSub    , ccBwd)) return CNT ;
                  if(!MoveActr(aiPcbCmp , ccBwd)) return CNT ;
                  Step.iCycle++;
                  return CNT;


        case  14: FrmMain -> OnSendAPCMsg("S2","3", AnsiString(OM.DevOptn.dAPCSetVal).c_str()); //APC POWER SETTING

                  Step.iCycle++;
                  return CNT;

        case  15: if(m_iPartId == piLT) FrmMain -> OnSendAPCMsg("S1","3"); //1P/U
                  if(m_iPartId == piRT) FrmMain -> OnSendAPCMsg("S1","4"); //2P/U
                  Step.iCycle++;
                  return CNT;

        case  16: if(OM.DevOptn.bROMType) FrmMain -> OnSendAPCMsg("S1","2"); //R/W
                  else                    FrmMain -> OnSendAPCMsg("S1","1"); //R/W
                  Step.iCycle++;
                  return CNT;

        case  17: FrmMain -> OnSendAPCMsg("S1","5"); //On
                  Step.iCycle = 0 ;
                  return END ;
    }
}

int TWorkTable::CycleUnDataCheck(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 5000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return ERR;
    }

    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;
                                                              //  aiLDP_FB  : 로더 푸셔
        case  10:
                  FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  MoveActr(aiSub  , ccFwd);
                  MoveActr(aiMain , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  11: if(!MoveActr(aiMain , ccFwd)) return CNT ;
                  if(!MoveActr(aiSub  , ccFwd)) return CNT ;
                  MoveActr(aiPcbCmp  , ccFwd);
                  m_TempTimer.Clear();
                  Step.iCycle++;
                  return CNT;

        case  12: if(!m_TempTimer.OnDelay(true , OM.DevOptn.iSptrEndUpDelay)) return CNT ;
                  MoveActr(aiSupport , ccFwd); //올라와 있으면 이미 본딩된 것들이라 안됌... 모드 바꾸고 세이브 버튼 누를때 변동.
                  Step.iCycle++;
                  return CNT;

        case  13: if(!MoveActr(aiPcbCmp  , ccFwd)) return CNT ;
                  if(!MoveActr(aiSupport , ccFwd)) return CNT ;
                  Step.iCycle = 0 ;
                  return END;

    }
}








//---------------------------------------------------------------------------
bool __fastcall TWorkTable::InspectActuator(void)
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if (AT.Err(aiMain   )){EM.SetErr(eiMainTO   ); isOk = false;}
    if (AT.Err(aiSub    )){EM.SetErr(eiSubTO    ); isOk = false;}
    if (AT.Err(aiLdCal  )){EM.SetErr(eiLdCalTO  ); isOk = false;}
    if (AT.Err(aiLdIO   )){EM.SetErr(eiLdIOTO   ); isOk = false;}

    if (AT.Err(aiGp1    )){EM.SetErr(eiGp1TO    ); isOk = false;}
    if (AT.Err(aiSupport)){EM.SetErr(eiSupportTO); isOk = false;}
    if (AT.Err(aiPcbCmp )){EM.SetErr(eiPcbCmpTO ); isOk = false;}
    if (AT.Err(aiGtCal  )){EM.SetErr(eiGtCalTO  ); isOk = false;}
    if (AT.Err(aiUvFB   )){EM.SetErr(eiUvFBTO   ); isOk = false;}
    if (AT.Err(aiUvUD   )){EM.SetErr(eiUvUDTO   ); isOk = false;}
    if (AT.Err(aiDp1IO  )){EM.SetErr(eiDp1IOTO  );
        isOk = false;
        }
    if (AT.Err(aiDp2IO  )){EM.SetErr(eiDp2IOTO  );
        isOk = false;
        }
    if (AT.Err(aiDp1UD  )){EM.SetErr(eiDp1UDTO  ); isOk = false;}
    if (AT.Err(aiDp2UD  )){EM.SetErr(eiDp2UDTO  ); isOk = false;}
    if (AT.Err(aiPadUD  )){EM.SetErr(eiPadUDTO  ); isOk = false;}

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

    //Inspect Motor Alarm.
    if (MT.GetAlarm(miZ )){EM.SetErr(eiXAlarm ); isOk = false; }
    if (MT.GetAlarm(miY )){EM.SetErr(eiYAlarm ); isOk = false; }
    if (MT.GetAlarm(miI )){EM.SetErr(eiIAlarm ); isOk = false; }

    if (MT.GetNLim (miZ )){EM.SetErr(eiXNegLim ); isOk = false; }
    if (MT.GetNLim (miY )){EM.SetErr(eiYNegLim ); isOk = false; }
    if (MT.GetNLim (miI )){EM.SetErr(eiINegLim ); isOk = false; }

    if (MT.GetPLim (miZ )){EM.SetErr(eiXPosLim ); isOk = false; }
    if (MT.GetPLim (miY )){EM.SetErr(eiYPosLim ); isOk = false; }
    if (MT.GetPLim (miI )){EM.SetErr(eiIPosLim ); isOk = false; }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall TWorkTable::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    if (!MT.GetHomeEnd(miZ )){EM.SetErr(eiXHomeEnd); isOk = false; }
    if (!MT.GetHomeEnd(miY )){EM.SetErr(eiYHomeEnd); isOk = false; }
    if (!MT.GetHomeEnd(miI )){EM.SetErr(eiIHomeEnd); isOk = false; }

    //Reset Flag for Updated Error Form.
    //if (!isOk) EM._bUpdatedErrForm = false;
    //Ok.
    return isOk;
}









//드라이런 모드.
int TWorkTable::CycleDryRun(void)
{

    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_CycleTimer.OnDelay(Step.iCycle == PreStep.iCycle , 20000)) {
        EM.SetErr(eiCycleTO);
        sTemp = sTemp.sprintf("CYCLE TIMEOUT STATUS : Step.iCycle=%02d" , Step.iCycle );
        Trace(GetPartName().c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return ERR;
    }

    sTemp = sTemp.sprintf("Step.iCycle=%02d" , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(GetPartName().c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    CAxtFS20::CUsrPara UsrPara ;
    bool bR1  , bR2  , bR3  , bR4  , bR5  , bR6  ;
    bool bR11 , bR22 , bR33 , bR44 , bR55 , bR66 ;

    //Cycle.
    switch (Step.iCycle)
    {
        default : Step.iCycle = 0;
                  return END;

        //Pick Cycle.
        case  10: MoveActr(aiMain , ccBwd);
                  Step.iCycle++;
                  return CNT;

        case  11: if(!MoveActr(aiMain , ccBwd)) return CNT ;
                  MoveActr(aiSub     , ccBwd) ;
                  MoveActr(aiSupport , ccBwd) ;
                  Step.iCycle++;
                  return CNT;

        case  12: if(!MoveActr(aiSub     , ccBwd)) return CNT ;
                  if(!MoveActr(aiSupport , ccBwd)) return CNT ;
                  Step.iCycle=20;
                  return CNT ;

        //Up/Dn Cycle.
        case  20: MoveMotr(miI , pi_I_Wait ) ;
                  MoveMotr(miZ , pi_Z_Wait ) ;
                  MoveMotr(miY , pi_Y_Wait ) ;
                  MoveActr(aiLdIO , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  21: if(!MoveActr(aiLdIO , ccFwd)) return CNT ;
                  if(!MoveMotr(miI , pi_I_Wait ))return CNT ;
                  if(!MoveMotr(miZ , pi_Z_Wait ))return CNT ;
                  if(!MoveMotr(miY , pi_Y_Wait ))return CNT ;

                  MoveActr(aiLdCal , ccBwd);
                  Step.iCycle++;
                  return CNT;

        case  22: if(!MoveActr(aiLdCal , ccBwd)) return CNT ;
                  Step.iCycle=30;
                  return CNT ;

        //Vacuum Cycle.
        case  30: MoveActr(aiPcbCmp , ccBwd) ;
                  Step.iCycle++;
                  return CNT;

        case  31: if(!MoveActr(aiPcbCmp , ccBwd)) return CNT ;
                  //IO.SetY(ySuckOn , true) ;
                  Step.iCycle=40;
                  return CNT ;

        //Chucking Cycle.
        case  40: MoveActr(aiGp1 , ccFwd);
                  Step.iCycle++;
                  return CNT;


        case  41: if(!MoveActr(aiGp1 , ccFwd)) return CNT ;
                  MoveActr(aiGp1 , ccBwd);
                  Step.iCycle++;
                  return CNT;

        case  42: if(!MoveActr(aiGp1 , ccBwd)) return CNT ;
                  MoveActr(aiGp1 , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  43: if(!MoveActr(aiGp1 , ccFwd)) return CNT ;
                  m_TempTimer.Clear();
                  Step.iCycle++;
                  return CNT;

        case  44: if(!m_TempTimer.OnDelay(true , OM.DevOptn.iSptrDnDelay)) return CNT ;
                  //IO.SetY (ySuckOn  , false );
                  MoveActr(aiSupport , ccFwd );
                  Step.iCycle++;
                  return CNT;

        case  45: if(!MoveActr(aiSupport , ccFwd)) return CNT ;
                  Step.iCycle=50;
                  return CNT ;

        //Insp Cycle

        case  50: MoveActr(aiPadUD,ccBwd) ; //Take Time 줄이기 위해.   //재검사 옵션이거나 메뉴얼 동작 일경우는 안나간다.
                  MoveActr(aiGtCal,ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  51: if(!MoveActr(aiGtCal,ccFwd)) return CNT ;
                  MoveMotr(miI,pi_I_Insp );
                  MoveMotr(miY,pi_Y_Insp );
                  MoveMotr(miZ,pi_Z_Insp );
                  Step.iCycle++;
                  return CNT;

        case  52: if(!MoveMotr(miI,pi_I_Insp ))return CNT ;
                  if(!MoveMotr(miY,pi_Y_Insp ))return CNT ;
                  if(!MoveMotr(miZ,pi_Z_Insp ))return CNT ;
                  if(!MoveActr(aiPadUD,ccBwd ))return CNT ; //Take Time 줄이기 위해.
                  MoveActr(aiDp1IO,ccFwd) ;
                  MoveActr(aiDp2IO,ccFwd) ;
                  MoveActr(aiGtCal , ccBwd);
                  Step.iCycle++;
                  return CNT;

        case  53: if(!MoveActr(aiGtCal , ccBwd)) return CNT ;
                  MT.GetUsrPara(miI,UsrPara) ;
                  MT.Vibration(miI , OM.DevOptn.dGTDnVibRng , UsrPara.dRunSpeed , OM.DevOptn.iGTDnVibCnt , true ) ;
                  Step.iCycle++;
                  return CNT ;

        case  54: MT.GetUsrPara(miI,UsrPara) ;
                  if(!MT.Vibration(miI , OM.DevOptn.dGTDnVibRng , UsrPara.dRunSpeed , OM.DevOptn.iGTDnVibCnt,false))return CNT ;
                  m_TempTimer.Clear() ;
                  Step.iCycle++;
                  return CNT;

        case  55: if(!m_TempTimer .OnDelay(true , OM.DevOptn.iInspDelay )) return CNT ;
                  if(!MoveActr(aiPadUD,ccBwd)) return CNT ;
                  //Angle
                  if(OM.DevOptn.bUseAngleCal) {
                          if(1) {
                              MT.GetUsrPara(miY,UsrPara) ;
                              MT.Vibration(miY , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngYVibCnt , true );
                              //MT.GoIncRun(miY, dAngleX) ;
                          }
                          if(1) {
                              MT.GetUsrPara(miZ,UsrPara) ;
                              MT.Vibration(miZ , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngZVibCnt , true );
                              //MT.GoIncRun(miZ, dAngleY) ;
                          }
                  }

                  Step.iCycle++;
                  return CNT;


        case  56: bR1 = bR2 = bR3 = true ;
                  //Angle
                  if(OM.DevOptn.bUseAngleCal) {
                          if(1) {
                              MT.GetUsrPara(miY,UsrPara) ;
                              bR1 = MT.Vibration(miY , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngYVibCnt , false );
                          }
                          if(1) {
                              MT.GetUsrPara(miZ,UsrPara) ;
                              bR2 = MT.Vibration(miZ , OM.DevOptn.dAngVibRng , UsrPara.dRunSpeed , OM.DevOptn.iAngZVibCnt , false );
                          }
                  }

                  if(!bR1 || !bR2) return CNT ;
                  MoveActr(aiGtCal , ccFwd) ;
                  Step.iCycle++;
                  return CNT ;

        case  57: if(!MoveActr(aiGtCal , ccFwd)) return CNT ;
                  Step.iCycle=60 ;
                  return CNT ;



        //DispCycle
        case  60: MoveActr(aiLdIO   , ccFwd ) ;
                  Step.iCycle++;
                  return CNT;

        case  61: if(!MoveActr(aiLdIO   , ccFwd )) return CNT ;
                  MoveActr(aiDp1UD , ccBwd);
                  MoveActr(aiDp2UD , ccBwd);
                  MoveActr(aiLdCal , ccBwd);
                  IO.SetY(yHdStart,false ); //UV Init.
                  Step.iCycle++;
                  return CNT;

        case  62: if(!MoveActr(aiLdCal , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp1UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp2UD , ccBwd)) return CNT ;
                  MoveActr(aiGtCal , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  63: if(!MoveActr(aiGtCal , ccFwd)) return CNT ;
                  Step.iCycle++;
                  return CNT ;

        case  64: MoveMotr(miI,pi_I_Wait ) ;

                  if(!OM.DevOptn.bInspBefrDisp) {
                      MoveMotr(miY , pi_Y_Disp) ;
                      MoveMotr(miZ , pi_Z_Disp) ;
                  }

                  Step.iCycle++;
                  return CNT;

        case  65: if(!MoveMotr(miI,pi_I_Wait ))return CNT ;

                  if(!OM.DevOptn.bInspBefrDisp) {
                      if(!MoveMotr(miY , pi_Y_Disp)) return CNT ;
                      if(!MoveMotr(miZ , pi_Z_Disp)) return CNT ;
                  }

                  MoveActr(aiPadUD , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  66: if(!MoveActr(aiPadUD , ccBwd)) return CNT ;
                  MoveActr(aiDp1IO , ccFwd);
                  MoveActr(aiDp2IO , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  67: if(!MoveActr(aiDp1IO , ccFwd)) return CNT ;
                  if(!MoveActr(aiDp2IO , ccFwd)) return CNT ;
                  MoveActr(aiDp1UD , ccFwd);
                  MoveActr(aiDp2UD , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  68: if(!MoveActr(aiDp1UD , ccFwd)) return CNT ;
                  if(!MoveActr(aiDp2UD , ccFwd)) return CNT ;

                  //Dispensing pattern
                  m_iCcDrRnDispCnt = 0 ;
                  m_TempTimer.Clear() ;
                  Step.iCycle++;
                  return CNT ;

        case  69: if(!m_TempTimer.OnDelay(true , 100)) return CNT ;
                  Step.iCycle++;
                  return CNT ;

        case  70: Stat.bDispensing = true ;
                  m_DispTimer1.Clear() ;
                  m_DispTimer2.Clear() ;
                  m_DispTimer3.Clear() ;
                  m_DispTimer4.Clear() ;
                  m_DispTimer5.Clear() ;
                  m_DispTimer6.Clear() ;

                  Step.iCycle++;
                  return CNT;

        case  71: bR11 = iOptnDisp[0][m_iCcDrRnDispCnt] ;
                  bR22 = iOptnDisp[1][m_iCcDrRnDispCnt] ;
                  bR33 = iOptnDisp[2][m_iCcDrRnDispCnt] ;
                  bR44 = iOptnDisp[3][m_iCcDrRnDispCnt] ;
                  bR55 = iOptnDisp[4][m_iCcDrRnDispCnt] ;
                  bR66 = iOptnDisp[5][m_iCcDrRnDispCnt] ;


                  bR1 = m_DispTimer1.OnDelay(true , iOptnDisp[0][m_iCcDrRnDispCnt]);
                  bR2 = m_DispTimer2.OnDelay(true , iOptnDisp[1][m_iCcDrRnDispCnt]);
                  bR3 = m_DispTimer3.OnDelay(true , iOptnDisp[2][m_iCcDrRnDispCnt]);
                  bR4 = m_DispTimer4.OnDelay(true , iOptnDisp[3][m_iCcDrRnDispCnt]);
                  bR5 = m_DispTimer5.OnDelay(true , iOptnDisp[4][m_iCcDrRnDispCnt]);
                  bR6 = m_DispTimer6.OnDelay(true , iOptnDisp[5][m_iCcDrRnDispCnt]);

                  if(bR1)IO.SetY(ySol1 , false);
                  if(bR2)IO.SetY(ySol2 , false);
                  if(bR3)IO.SetY(ySol3 , false);
                  if(bR4)IO.SetY(ySol4 , false);
                  if(bR5)IO.SetY(ySol5 , false);
                  if(bR6)IO.SetY(ySol6 , false);

                  if(!bR1 || !bR2 || !bR3 || !bR4 || !bR5 || !bR6 ) return CNT ;

                  m_iCcDrRnDispCnt++ ;
                  if(m_iCcDrRnDispCnt < MAX_DISP_CNT) {
                      Step.iCycle -= 2;
                      m_TempTimer.Clear() ;
                      return CNT ;
                  }
                  m_TempTimer.Clear() ;
                  Step.iCycle++ ;
                  return CNT ;

        case  72: if(!m_TempTimer.OnDelay(true , OM.DevOptn.iDispEndDelay))return CNT ;
                  Stat.bDispensing = false ;
                  MoveActr(aiDp1UD , ccBwd);
                  MoveActr(aiDp2UD , ccBwd);
                  MoveActr(aiUvUD  , ccFwd);
                  m_TempTimer.Clear() ;

                  //IO.SetY(yVac1 , true);
                  //IO.SetY(yVac2 , true);
                  //IO.SetY(yVac3 , true);
                  //IO.SetY(yVac4 , true);
                  //IO.SetY(yVac5 , true);
                  //IO.SetY(yVac6 , true);

                  Step.iCycle++;
                  return CNT ;

        case  73:
                  if(!MoveActr(aiDp1UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp2UD , ccBwd)) return CNT ;
                  if(!MoveActr(aiUvUD  , ccFwd)) return CNT ;
                  MoveActr(aiDp1IO , ccBwd);
                  MoveActr(aiDp2IO , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  74: if(!MoveActr(aiDp1IO , ccBwd)) return CNT ;
                  if(!MoveActr(aiDp2IO , ccBwd)) return CNT ;

                  MoveActr(aiPadUD , ccFwd);
                  //if(!Rqst.bDispCycle)MoveActr(aiUvFB  , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  75: if(!MoveActr(aiPadUD , ccFwd)) return CNT ;
                  //FrmMain -> OnSendAPCMsg("S1","6"); //Off
                  Step.iCycle=80;
                  return CNT ;


        //UvCycle.
        case  80: MoveActr(aiUvUD  , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  81: if(!MoveActr(aiUvUD , ccFwd)) return CNT ;
                  MoveActr(aiUvFB , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  82: if(!MoveActr(aiUvFB , ccFwd)) return CNT ;
                  MoveActr(aiUvUD  , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  83: if(!MoveActr(aiUvUD  , ccBwd)) return CNT ;
                  m_TempTimer.Clear() ;
                  //IO.SetY(yHdStart,true );
                  Step.iCycle++ ;
                  return CNT ;

        //바꿀때 타임아웃조건 확인.
        case  84: if(!m_TempTimer.OnDelay(true , 5000)) return CNT ;
                  //if(!IO.GetX(xHdEnd))return CNT ;
                  IO.SetY(yHdStart,false );

                  MoveActr(aiUvUD  , ccFwd);
                  //MoveActr(aiMain  , ccFwd);
                  Step.iCycle++;
                  return CNT ;

        case  85: if(!MoveActr(aiUvUD  , ccFwd)) return CNT ;
                  //if(!MoveActr(aiMain  , ccFwd)) return CNT ;

                  //MoveActr(aiSupport , ccBwd) ;

                  MoveActr(aiUvFB    , ccBwd);
                  //MoveActr(aiSupport , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  86: if(!MoveActr(aiUvFB    , ccBwd)) return CNT ;
                  //if(!MoveActr(aiSupport , ccBwd)) return CNT ;
                  MoveActr(aiGp1 , ccBwd);
                  Step.iCycle++;
                  return CNT ;

        case  87: if(!MoveActr(aiGp1 , ccBwd)) return CNT ;
                  m_TempTimer.Clear() ;
                  Step.iCycle++ ;
                  return CNT ;

        case  88: if(!m_TempTimer.OnDelay(true , 500))return CNT ;
                  MoveActr(aiPcbCmp , ccFwd) ;
                  MoveActr(aiSub    , ccFwd) ;
                  Step.iCycle++;
                  return CNT;

        case  89: if(!MoveActr(aiPcbCmp , ccFwd)) return CNT ;
                  if(!MoveActr(aiSub    , ccFwd)) return CNT ;
                  IO.SetY(ySuckOn , false) ;
                  MoveActr(aiLdCal , ccFwd);

                  Step.iCycle++;
                  return CNT;

        case  90: if(!MoveActr(aiLdCal , ccFwd)) return CNT ;
                  MoveActr(aiMain  , ccFwd);
                  Step.iCycle++;
                  return CNT;

        case  91: if(!MoveActr(aiMain  , ccFwd)) return CNT ;

                  //MoveActr(aiSupport , ccBwd) ;

                  MoveMotr(miI , pi_I_Wait ) ;
                  MoveMotr(miZ , pi_Z_Wait ) ;
                  MoveMotr(miY , pi_Y_Wait ) ;
                  MoveActr(aiLdIO , ccBwd);
                  Step.iCycle++;
                  return CNT;

        case  92: if(!MoveActr(aiLdIO , ccBwd  )) return CNT ;
                  if(!MoveMotr(miI , pi_I_Wait )) return CNT ;
                  if(!MoveMotr(miZ , pi_Z_Wait )) return CNT ;
                  if(!MoveMotr(miY , pi_Y_Wait )) return CNT ;
                  MoveActr(aiSupport , ccBwd) ;
                  m_TempTimer.Clear() ;
                  Step.iCycle++;
                  return CNT;

        case  93: if(!MoveActr(aiSupport , ccBwd)) return CNT ;
                  if(!m_TempTimer.OnDelay(true , 1000))return CNT ;
                  Step.iCycle=0 ;
                  return END ;

    }
}


void TWorkTable::PushData(double dDataY , double dDataZ , int iCnt)
{
    if(iCnt > MAX_PRE_POS) iCnt = MAX_PRE_POS ;
    if(iCnt < 3          ) iCnt = 3           ;
    for(int i = 0 ; i < iCnt ; i++){
        dPrePstnY[iCnt - i - 1] = dPrePstnY[iCnt - i - 2] ;
        dPrePstnZ[iCnt - i - 1] = dPrePstnZ[iCnt - i - 2] ;
    }
    dPrePstnY[0] = dDataY ;
    dPrePstnZ[0] = dDataZ ;

};

double TWorkTable::GetAvrPosY(int iCnt)
{
    if(iCnt > MAX_PRE_POS) iCnt = MAX_PRE_POS ;
    if(iCnt < 3          ) iCnt = 3           ;
    double dSum =  0.0 ;
    double dMax = -100 ;
    double dMin =  100 ;
    for(int i = 0 ; i < iCnt ; i++){
        if(dPrePstnY[i] > dMax) dMax = dPrePstnY[i] ;
        if(dPrePstnY[i] < dMin) dMin = dPrePstnY[i] ;
        dSum += dPrePstnY[i] ;
    }

    dSum = dSum - dMax - dMin ;
    return dSum / (double)(iCnt-2) ;
}

double TWorkTable::GetAvrPosZ(int iCnt)
{
    if(iCnt > MAX_PRE_POS) iCnt = MAX_PRE_POS ;
    if(iCnt < 3          ) iCnt = 3           ;
    double dSum =  0.0 ;
    double dMax = -100 ;
    double dMin =  100 ;
    for(int i = 0 ; i < iCnt ; i++){
        if(dPrePstnZ[i] > dMax) dMax = dPrePstnZ[i] ;
        if(dPrePstnZ[i] < dMin) dMin = dPrePstnZ[i] ;
        dSum += dPrePstnZ[i] ;
    }

    dSum = dSum - dMax - dMin ;
    return dSum / (double)(iCnt-2) ;
}

void TWorkTable::LoadAvrPos(bool bLeft)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    AnsiString _sDevName = "NONE" ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\AvrPos.INI" ;

    for(int i = 0 ; i < MAX_PRE_POS ; i++) {
//        UserINI.Load(sPath, "AvrPosY"  , AnsiString(i).c_str() , DevOptn.iSptrDnDelay );
//        UserINI.Load(sPath, "AvrPosZ"  , AnsiString(i).c_str() , DevOptn.iSptrDnDelay );
    }

}

void TWorkTable::SaveAvrPos(bool bLeft)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    AnsiString _sDevName = "NONE" ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\AvrPos.INI" ;

    for(int i = 0 ; i < MAX_PRE_POS ; i++) {
//        UserINI.Save(sPath, "AvrPosY"  , AnsiString(i).c_str() , DevOptn.iSptrDnDelay );
//        UserINI.Save(sPath, "AvrPosZ"  , AnsiString(i).c_str() , DevOptn.iSptrDnDelay );

    }
}

