//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Rail.h"
#include "PreBuff.h"
#include "LaserEO.h"
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
CRail RAL;

CRail::CRail(void)
{

}

CRail::~CRail (void)
{

}

void CRail::Init()
{                               
    m_sPartName = "Rail " ;
    Reset();
    Load(true);

    InitCycleName();
}
void CRail::Close()
{
    Load(false);
}

void CRail::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    m_bNeedStrip = false ;
}

void CRail::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}



bool CRail::FindChip(int &r , int &c )
{
    //    r = DM.ARAY[riLD1].FindFrstRow(csUnkwn) ;
    return false;


    return false ;
}

double CRail::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = MT_GetCmdPos(_iMotr) ;


    if(_iMotr == miRAL_XLStp){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piRAL_XLStpWait  : dPos = PM.GetValue (_iMotr , pvRAL_XLStpWait      ); break ;
            case piRAL_XLStpWork  : dPos = PM.GetValue (_iMotr , pvRAL_XLStpWork      ); break ;
        }
    }

    else if(_iMotr == miRAL_XVStp){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piRAL_XVStpWait  : dPos = PM.GetValue (_iMotr , pvRAL_XVStpWait      ); break ;
            case piRAL_XVStpWork  : dPos = PM.GetValue (_iMotr , pvRAL_XVStpWork      ); break ;
        }
    }
    else if(_iMotr == miRAL_YCvsn){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piRAL_YCvsnWait  : dPos = PM.GetValue (_iMotr , pvRAL_YCvsnWait      ); break ;
            case piRAL_YCvsnWork  : dPos = PM.GetValue (_iMotr , pvRAL_YCvsnWork      ); break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CRail::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    /*
    if(_iMotr == miRAL_XIdx){
        if(isMotrNeg && isIdxUp) {sMsg = "Index Pinger is Up"; bRet = false ;}
        switch(_iPstnId) {
            default                :  break ;
            case piRAL_XIdxWait    :  break ;
            case piRAL_XIdxWorkStt :  break ;
            case piRAL_XIdxOut     :  break ;
            case piRAL_XIdxWork    :  break ;
        }
    }*/

    if(_iMotr == miRAL_XLStp){
        switch(_iPstnId) {
            default               :  { sMsg = "Not " +AnsiString(MT_GetName(miLDR_ZClmp))+"'s Postion" ; bRet = false ;} break ;
            case piRAL_XLStpWait  :  break ;
            case piRAL_XLStpWork  :  break ;
        }
    }

    else if(_iMotr == miRAL_XVStp){
        switch(_iPstnId) {
            default               :  { sMsg = "Not " +AnsiString(MT_GetName(miLDR_ZClmp))+"'s Postion" ; bRet = false ;} break ;
            case piRAL_XVStpWait  :  break ;
            case piRAL_XVStpWork  :  break ;
        }
    }
    else if(_iMotr == miRAL_YCvsn){
        if(IO_GetX(xRAL_LMask) || IO_GetX(xRAL_VMask)) { sMsg = "Remove Rail Mask!" ; bRet = false ; }
        switch(_iPstnId) {
            default               : { sMsg = "Not " +AnsiString(MT_GetName(miLDR_ZClmp))+"'s Postion" ; bRet = false ;} break ;
            case piRAL_YCvsnWait  : break ;
            case piRAL_YCvsnWork  : break ;
        }
    }
    else {
        bRet = false ;
    }
    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CRail::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd )) return true ;
    
    bool bRet = true ;
    AnsiString sMsg ;

    if(_iActr == aiRAL_LStpUpDn) {
        //if(_bFwd) {
        //    if(isXMoving) { sMsg = AnsiString(MT_GetName(miRAL_XIdx))+" is Moving" ; bRet = false ;}
        //}
    }
    else if(_iActr == aiRAL_LStnUpDn) {
    }
    else if(_iActr == aiRAL_LAlnFwBw) {
    }
    else if(_iActr == aiRAL_VStpUpDn) {
    }
    else if(_iActr == aiRAL_VStnUpDn) {
    }
    else if(_iActr == aiRAL_VAlnFwBw) {

    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}

bool CRail::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CRail::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CRail::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 15000 )) {
        EM_SetErrMsg(eiPRT_HomeTo , m_sPartName.c_str());//EM_SetErr(eiRAL_HomeTo);
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

        case 10: AT_MoveCyl(aiRAL_LAlnFwBw  ,ccBwd);
                 AT_MoveCyl(aiRAL_VAlnFwBw  ,ccBwd);
                 AT_MoveCyl(aiRAL_LStpUpDn  ,ccFwd);
                 AT_MoveCyl(aiRAL_VStpUpDn  ,ccFwd);
                 IO_SetY(yRAL_FeedAc , false);

                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiRAL_LAlnFwBw ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiRAL_VAlnFwBw ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiRAL_LStpUpDn ,ccFwd)) return false ;
                 if(!AT_MoveCyl(aiRAL_VStpUpDn ,ccFwd)) return false ;

                 AT_MoveCyl(aiRAL_LStnUpDn  ,ccBwd);
                 AT_MoveCyl(aiRAL_VStnUpDn  ,ccBwd);

                 Step.iHome++;
                 return false ;

        case 12: if(!AT_MoveCyl(aiRAL_LStnUpDn  ,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiRAL_VStnUpDn  ,ccBwd)) return false ;


                 MT_SetHomeEnd(miRAL_XLStp , false);
                 MT_SetHomeEnd(miRAL_XVStp , false);


                 MT_DoHome(miRAL_XLStp);
                 MT_DoHome(miRAL_XVStp);

                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miRAL_XLStp)) return false;
                 if(!MT_GetHomeEnd(miRAL_XVStp)) return false;

                 MT_GoAbsMan(miRAL_XLStp, PM.GetValue(miRAL_XLStp , pvRAL_XLStpWork));
                 MT_GoAbsMan(miRAL_XVStp, PM.GetValue(miRAL_XVStp , pvRAL_XVStpWork));
                 Step.iHome++ ;
                 return false ;

        case 14: if(!MT_GoAbsMan(miRAL_XLStp, PM.GetValue(miRAL_XLStp , pvRAL_XLStpWork)))return false ;
                 if(!MT_GoAbsMan(miRAL_XVStp, PM.GetValue(miRAL_XVStp , pvRAL_XVStpWork)))return false ;

                 if(IO_GetX(xRAL_LMask) || IO_GetX(xRAL_VMask)){ //||
                   //!DM.ARAY[riPRB].CheckAllStat(csNone) || !DM.ARAY[riLSR].CheckAllStat(csNone) ||
                   //!DM.ARAY[riVSN].CheckAllStat(csNone) || !DM.ARAY[riPSB].CheckAllStat(csNone) ||
                   // IO_GetX(xPRB_PkgIn1) || IO_GetX(xPRB_Pkg2  ) || IO_GetX(xRAL_LPkg3) ||
                   // IO_GetX(xRAL_VPkg4 ) || IO_GetX(xPSB_PkgIn5) || IO_GetX(xPSB_Pkg5 ) || IO_GetX(xPSB_PkgOut6 )){ //����ũ ������ Ȩ ������. ���� ������ �Ű� �Ⱦ���.

                     MT_SetHomeEnd(miRAL_YCvsn , true);
                     MT_SetPos(miRAL_YCvsn , PM.GetValue(miRAL_YCvsn , pvRAL_YCvsnWork));
                     Step.iHome = 0;
                     return true ;
                 }


                 

                 MT_SetHomeEnd(miRAL_YCvsn , false);
                 MT_DoHome(miRAL_YCvsn);

                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GetHomeEnd(miRAL_YCvsn)) return false;
                 MT_GoAbsMan(miRAL_YCvsn, PM.GetValue(miRAL_YCvsn , pvRAL_YCvsnWork));
                 Step.iHome++;
                 return false ;

        case 16: if(!MT_GoAbsMan(miRAL_YCvsn, PM.GetValue(miRAL_YCvsn , pvRAL_YCvsnWork)))return false ;
                 Step.iHome = 0;
                 return true ;



    }
}

bool CRail::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    //static CDelayTimer tmWait ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

//�������� �����˻� ����������  ��ŷ�˻��ʿ�  �����˻�
//Unkwn -> Mask     Work        Wait          Good
//                                            Fail

    if (Step.iSeq == 0) {
        if(Stat.bReqStop )return false ;

        //bool bLSRNeedToWork  = DM.ARAY[riLSR].CheckAllStat(csMask);
        bool bRailExstStrip  =!DM.ARAY[riPRB].CheckAllStat(csNone)||
                              !DM.ARAY[riLSR].CheckAllStat(csNone)||
                              !DM.ARAY[riVSN].CheckAllStat(csNone);
        bool bVSNNeedToWork  = DM.ARAY[riVSN].GetCntStat  (csWait);
        bool bLSRNeedToWork  = DM.ARAY[riLSR].CheckAllStat(csMask);

        bool bExstStripToWork = !PRB.GetWorkEnd() ;


        //ReadyToFeed�� Feeding ���� �ϸ鼭 ������ �������� �����̰ų� �ƿ����� ������ ��� �Ѵ�.
        //���� ��ġ�� ������ �ִ��� ������ �ǵ� �������� ���������� �ִ����� ��� �Ͽ� ������ ��´�.
        //�� �ǵ��Ϸ� �Ҷ� PSB���� �۾��� �ȳ������� VSN���� �ִ� ������ FEED�� ���� ���� ���� ���� ���ʴ�� �ڿ� ����� �۾��� �ȵȴ�.
                              //�����۾��� �ٳ����� ����Ʈ ���ۿ� ������ ���ų�                                �������� ������ ������
        bool bLSRSkipToFeed  =( DM.ARAY[riLSR].CheckAllStat(csMask) && OM.CmnOptn.bSkipLaser);
        bool bVSNReadyToFeed =(!DM.ARAY[riVSN].GetCntStat  (csWait) && DM.ARAY[riPSB].CheckAllStat(csNone)) /*&& !DM.ARAY[riVSN].CheckAllStat(csNone)*/|| DM.ARAY[riVSN].CheckAllStat(csNone); //
        bool bLSRReadyToFeed =((DM.ARAY[riLSR].CheckAllStat(csWork) && bVSNReadyToFeed) || bLSRSkipToFeed ) || DM.ARAY[riLSR].CheckAllStat(csNone);
        bool bPRBReadyToFeed =( DM.ARAY[riPRB].CheckAllStat(csMask) ||!bExstStripToWork                   );//|| DM.ARAY[riPRB].CheckAllStat(csNone)PRB�� �ȱ�ٸ����); //��Ʈ���� ���������� �����˻� �ݹ泡���ϱ� ��ٸ��� ����.


                              //�°� ���ϸ� �־�� �غ��.

        //�����ġ�� ������ �԰� OnDle

        bool isCycleFeed  =  bRailExstStrip && bPRBReadyToFeed && bLSRReadyToFeed && bVSNReadyToFeed ;  //��Ʈ�� ������.
        bool isCycleBind  = (bLSRNeedToWork && AT_Complete(aiRAL_LStnUpDn , ccBwd)) || //��Ʈ�� �����̼� �ø���.
                            (bVSNNeedToWork && AT_Complete(aiRAL_VStnUpDn , ccBwd)) ;

        bool isCycleEnd   = DM.ARAY[riLSR].CheckAllStat(csNone) &&
                            DM.ARAY[riVSN].CheckAllStat(csNone) && PRB.GetWorkEnd();


        //Unknown Strip
        if (IO_GetX(xRAL_LPkg3) && DM.ARAY[riLSR].CheckAllStat(csNone)) EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riLSR].GetName().c_str()) ;
        if (IO_GetX(xRAL_VPkg4) && DM.ARAY[riVSN].CheckAllStat(csNone)){
            EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riVSN].GetName().c_str()) ;
        }
        bool aa,dd;

        //Dissapear Strip
        if (!OM.MstOptn.bDryRun) {
            if (!IO_GetX(xRAL_LPkg3) && !DM.ARAY[riLSR].CheckAllStat(csNone)&& LSR.GetSeqStep()==CLaser::scIdle){ //������ ���̵� ���� ���� �׽�Ʈ �ؾ� ��...JH
                 aa= IO_GetX(xRAL_LPkg3);
                 dd= DM.ARAY[riLSR].CheckAllStat(csNone);
                 EM_SetErrMsg(eiPKG_Dispr , DM.ARAY[riLSR].GetName().c_str()) ;
            }
            bool bTemp = !IO_GetX(xRAL_VPkg4) && !DM.ARAY[riVSN].CheckAllStat(csNone) ;
            if (!IO_GetX(xRAL_VPkg4) && !DM.ARAY[riVSN].CheckAllStat(csNone)) EM_SetErrMsg(eiPKG_Dispr , DM.ARAY[riVSN].GetName().c_str()) ;
        }

        if(EM_IsErr()) return false ;
        //Normal Decide Step.
             if (isCycleFeed) {Trace(m_sPartName.c_str(),"CycleFeed Stt"); Step.iSeq = scFeed ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleBind) {Trace(m_sPartName.c_str(),"CycleBind Stt"); Step.iSeq = scBind ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default     :                  Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case scIdle :                                                                                   return false ;
        case scFeed : if(CycleFeed()){ Trace(m_sPartName.c_str(),"CycleFeed End");Step.iSeq = scIdle ;} return false ;
        case scBind : if(CycleBind()){ Trace(m_sPartName.c_str(),"CycleBind End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;

}

bool CRail::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;

    if (Step.iSeq) return false;

    Step.iToStop = 10;

    //Ok.
    return true;

}

bool CRail::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CRail::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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

        case 10: MoveActr(aiRAL_LStpUpDn , ccFwd);
                 MoveActr(aiRAL_VStpUpDn , ccFwd);
                 MoveActr(aiRAL_LAlnFwBw , ccBwd);
                 MoveActr(aiRAL_VAlnFwBw , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiRAL_LStpUpDn , ccFwd)) return false ;
                 if(!MoveActr(aiRAL_VStpUpDn , ccFwd)) return false ;
                 if(!MoveActr(aiRAL_LAlnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiRAL_VAlnFwBw , ccBwd)) return false ;

                 if((MT_GetCmdPos(miRAL_YCvsn) < GetMotrPos(miRAL_YCvsn , piRAL_YCvsnWork)-0.1) ||
                    (MT_GetCmdPos(miRAL_YCvsn) > GetMotrPos(miRAL_YCvsn , piRAL_YCvsnWork)+0.1)) { //���� Ȩ�� ��������.
                     EM_SetErr(eiRAL_NotWorkPos); //������ Ȩ���������� ����ũ �Ȱ� Ȩ��ƶ�.~

                 }
                 IO_SetY(yRAL_FeedAc , true);
                 m_tmDelay.Clear();
                 Step.iToStart++;
                 return false ;



        //��� ���� Ȯ��. ������ ���ھ� ����.
        case 12: //if(!m_tmDelay.OnDelay(true , 500)) return false ; ���ε� ����Ŭ�� ��.
                 //if(!DM.ARAY[riVSN].CheckAllStat(csNone)) MoveActr(aiRAL_VAlnFwBw , ccFwd);
                 //if(!DM.ARAY[riLSR].CheckAllStat(csNone)) MoveActr(aiRAL_LAlnFwBw , ccFwd);

                 Step.iToStart++ ;
                 return false ;

        case 13: //if(!AT_Done(aiRAL_VAlnFwBw)) return false ;
                 //if(!AT_Done(aiRAL_LAlnFwBw)) return false ;

                 IO_SetY(yRAL_FeedAc , false);

                 Step.iToStart = 0 ;



                 return true ;
    }
}

bool CRail::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() , 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str());//EM_SetErr(eiRAL_ToStopTO);

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

        case 10: IO_SetY(yRAL_FeedAc,false);
                 m_tmDelay.Clear();
                 Step.iToStop ++;
                 return false ;

        case 11: if(!m_tmDelay.OnDelay(true , 500)) return false ; //kye

                 //MoveActr(aiRAL_LStpUpDn , ccBwd);
                 //MoveActr(aiRAL_VStpUpDn , ccBwd);
                 MoveActr(aiRAL_LAlnFwBw , ccBwd);
                 MoveActr(aiRAL_VAlnFwBw , ccBwd);

                 MoveActr(aiRAL_LStnUpDn , ccBwd);
                 MoveActr(aiRAL_VStnUpDn , ccBwd);

                 Step.iToStop++;
                 return false ;

        case 12: //if(!MoveActr(aiRAL_LStpUpDn , ccBwd)) return false ;
                 //if(!MoveActr(aiRAL_VStpUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRAL_LAlnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiRAL_VAlnFwBw , ccBwd)) return false ;

                 if(!MoveActr(aiRAL_LStnUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRAL_VStnUpDn , ccBwd)) return false ;


                 Step.iToStop = 0 ;
                 return true ;
    }
}

//�� ~~~ ���� ����.
bool CRail::CycleFeed()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiRAL_CycleTO);
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



    static bool bVSNCanOut = false ; //�������� ������ �ִ� ����.
    static bool bLSRCanOut = false ;
    static bool bPRBCanOut = false ;

    static bool bVSNCanEnt = false ; //�������� ���͵� �Ǵ� ����.
    static bool bLSRCanEnt = false ;
    static bool bPRBCanEnt = false ;


    static int  iInspRowNo = 0 ; //��ŷ���� 1�ٰ˻��϶� ���.


    //Visn Zone
    if(AT_Complete(aiRAL_VStpUpDn , ccBwd)){ //������ ��������. ���� ���� �����Ͽ� ������ �ø�.
        if(!IO_GetX(xRAL_VPkg4)){
            MoveActr(aiRAL_VStpUpDn , ccFwd);
        }
    }

    //Laser Zone
    if(AT_Complete(aiRAL_LStpUpDn , ccBwd)){ //������ ��������.
        if(!IO_GetX(xRAL_LPkg3)){
            MoveActr(aiRAL_LStpUpDn , ccFwd);
        }
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //bVSNNeedOut = (!DM.ARAY[riVSN].GetCntStat  (csWork) && DM.ARAY[riPSB].CheckAllStat(csNone)) || DM.ARAY[riVSN].CheckAllStat(csNone); //
                  if(OM.CmnOptn.bSkipLaser && DM.ARAY[riLSR].CheckAllStat(csMask)) DM.ARAY[riLSR].SetStat(csWork);
                  bVSNCanEnt = (!DM.ARAY[riVSN].GetCntStat  (csWait) && DM.ARAY[riPSB].CheckAllStat(csNone)) || DM.ARAY[riVSN].CheckAllStat(csNone);
                  bLSRCanEnt = ( DM.ARAY[riLSR].CheckAllStat(csWork) && bVSNCanEnt                         ) || DM.ARAY[riLSR].CheckAllStat(csNone);
                  //bPRBCanEnt = ( DM.ARAY[riPRB].CheckAllStat(csMask) || PRB.GetWorkEnd()                   );//|| DM.ARAY[riPRB].CheckAllStat(csNone)


                  bVSNCanOut =  !DM.ARAY[riVSN].GetCntStat  (csWait) &&!DM.ARAY[riVSN].GetCntStat(csNone) && DM.ARAY[riPSB].CheckAllStat(csNone) ;
                  bLSRCanOut =   DM.ARAY[riLSR].CheckAllStat(csWork) && bVSNCanEnt;
                  bPRBCanOut =   DM.ARAY[riPRB].CheckAllStat(csMask) && bLSRCanEnt;//|| DM.ARAY[riPRB].CheckAllStat(csNone)PRB�� �ȱ�ٸ����); //��Ʈ���� ���������� �����˻� �ݹ泡���ϱ� ��ٸ��� ����.

                  MoveActr(aiRAL_VAlnFwBw , ccBwd);
                  MoveActr(aiRAL_LAlnFwBw , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiRAL_VAlnFwBw , ccBwd))return false ;
                  if(!MoveActr(aiRAL_LAlnFwBw , ccBwd))return false ;

                  //���� �ų��� ������~~~
                  IO_SetY(yRAL_FeedAc , true);

                  if(bVSNCanOut) { //������ ���� �۾� �� ���.
                      Step.iCycle = 20 ;
                      return false ;
                  }
                  else if(bLSRCanOut) { //������ �۾� �Ҳ� ���� LSR ���� �� ���.
                      Step.iCycle = 30 ;
                      return false ;
                  }
                  else if(bPRBCanOut) {
                      Step.iCycle = 40 ;
                      return false ;
                  }
                  else { //Ÿ�� �ȵǴ� �̻��� ���� ����.
                      IO_SetY(yRAL_FeedAc , false);
                      Step.iCycle = 0 ;
                      return true ;
                  }



        //������ ���� �۾� �Ҷ�.
        case  20: MoveActr(aiRAL_VStnUpDn , ccBwd);
                  MoveActr(aiRAL_VStpUpDn , ccBwd);



                  Step.iCycle++;
                  return false;

        case  21: //if(!MoveActr(aiRAL_VStnUpDn , ccBwd)) return false ;
                  //if(!MoveActr(aiRAL_VStpUpDn , ccBwd)) return false ;

                  DM.ShiftArrayData(riVSN , riPSB);

                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  22: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iRailStprDnDelay)) return false ;

                  if(bLSRCanOut) { //������ �� �۾� �� ����
                      Step.iCycle = 30 ;
                      return false ;
                  }
                  else if(bPRBCanOut) {
                      Step.iCycle = 40 ;
                      return false ;
                  }
                  else { //���� ����.

                      Step.iCycle=50;
                      return false ;
                  }




        //�������� �۾�.
        case  30: MoveActr(aiRAL_LStnUpDn , ccBwd);
                  MoveActr(aiRAL_LStpUpDn , ccBwd);

                  Step.iCycle++;
                  return false;

        case  31: if(!MoveActr(aiRAL_LStnUpDn , ccBwd)) return false ; //�������۴� �����̼��� ���
                  if(!MoveActr(aiRAL_LStpUpDn , ccBwd)) return false ; //Ȯ���� ���� ������ �ʹ� �پ ���´�.

                  DM.ShiftArrayData(riLSR , riVSN);
                  if(OM.CmnOptn.iMarkInsp == moOneRow) { //���� �˻��϶��� �ø���.
                      if(++iInspRowNo>=OM.DevInfo.iRowCnt){
                          iInspRowNo = 0 ;
                      }
                      for(int c = 0 ; c < DM.ARAY[riVSN].GetMaxCol() ; c++) DM.ARAY[riVSN].SetStat(iInspRowNo,c,csWait); //���ٸ� �����.
                  }
                  else if(OM.CmnOptn.iMarkInsp == moAllRow)  { //�ƴϸ� 0 �����ʹ� �ٳ����.
                      DM.ARAY[riVSN].SetStat(csWait);
                      iInspRowNo = 0 ;

                  }
                  else { //moNone
                  }

                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  32: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iRailStprDnDelay)) return false ;

                  if(bPRBCanOut) {
                      Step.iCycle = 40 ;
                      return false ;
                  }
                  else { //���� ����.
                      //IO_SetY(yRAL_FeedAc , false);
                      Step.iCycle=50;
                      return false ;
                  }




        //�������� �۾�.
        case  40: m_bNeedStrip = true ;
                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false;

        case  41: //if(m_tmDelay.OnDelay(true , 1000)) {
                  //    Step.iCycle=50;
                  //    return false ;
                  //}

                  if(!IO_GetX(xRAL_LPkg3)) return false ; //���� ������

                  Step.iCycle=50;
                  return false ;



        //��� ���� Ȯ��. ������ ���ھ� ����.
        case  50: if(!AT_Complete(aiRAL_VStpUpDn , ccFwd))return false ;
                  if(!AT_Complete(aiRAL_LStpUpDn , ccFwd))return false ;
                  //if(!AT_Complete(aiPRB_StprUpDn , ccFwd))return false ;
                  if(!AT_Complete(aiPRB_StprUpDn , ccFwd))return false ; //��Ŵ� �������� ����Ŭ���� ���� �Ѵ�.

                  //���� ���� Ȯ��.
                  if(bVSNCanOut&& IO_GetX(xPSB_PkgIn5))return false ; //
                  if(bLSRCanOut&&!IO_GetX(xRAL_VPkg4 ))return false ; //
                  if(bPRBCanOut&&!IO_GetX(xRAL_LPkg3 ))return false ; //



                  IO_SetY(yRAL_FeedAc , false);
                  m_bNeedStrip = false ;
                  Step.iCycle = 0 ;
                  return true ;


    }
}



//�۾� ��ġ�� �̵�.
bool CRail::CycleBind()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()); //EM_SetErr(eiRAL_CycleTO);
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

    static bool bLSRNeedToBind = false ;
    static bool bVSNNeedToBind = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiRAL_LStpUpDn , ccFwd);
                  MoveActr(aiRAL_VStpUpDn , ccFwd);
                  MoveActr(aiRAL_LAlnFwBw , ccBwd);
                  MoveActr(aiRAL_VAlnFwBw , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiRAL_LStpUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiRAL_VStpUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiRAL_LAlnFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiRAL_VAlnFwBw , ccBwd)) return false ;

                  IO_SetY(yRAL_FeedAc , true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;



        //���   ���� Ȯ��. ������ ���ھ� ����.
        case  12: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                  bLSRNeedToBind = (!DM.ARAY[riLSR].CheckAllStat(csNone) || IO_GetX(xRAL_LPkg3)) ;
                  bVSNNeedToBind = (!DM.ARAY[riVSN].CheckAllStat(csNone) || IO_GetX(xRAL_VPkg4)) ;


                  if(bLSRNeedToBind) MoveActr(aiRAL_LAlnFwBw , ccFwd);
                  if(bVSNNeedToBind) MoveActr(aiRAL_VAlnFwBw , ccFwd);


                  Step.iCycle++ ;
                  return false ;

        case  13: if(!AT_Done(aiRAL_LAlnFwBw)) return false ;
                  if(!AT_Done(aiRAL_VAlnFwBw)) return false ;




                  if(bLSRNeedToBind)MoveActr(aiRAL_LStnUpDn , ccFwd);
                  if(bVSNNeedToBind)MoveActr(aiRAL_VStnUpDn , ccFwd);

                  Step.iCycle++;
                  return false ;

        case  14: if(bLSRNeedToBind && !MoveActr(aiRAL_LStnUpDn , ccFwd)) return false ;
                  if(bVSNNeedToBind && !MoveActr(aiRAL_VStnUpDn , ccFwd)) return false ;

                  if(bLSRNeedToBind)MoveActr(aiRAL_LAlnFwBw , ccBwd);
                  if(bVSNNeedToBind)MoveActr(aiRAL_VAlnFwBw , ccBwd);

                  IO_SetY(yRAL_FeedAc , false);

                  Step.iCycle++;
                  return false ;

        case  15: if(!bLSRNeedToBind && !MoveActr(aiRAL_LAlnFwBw , ccBwd)) return false ;
                  if(!bVSNNeedToBind && !MoveActr(aiRAL_VAlnFwBw , ccBwd)) return false ;
                  Step.iCycle=0;
                  return true;
    }
}



void CRail::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CRail::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CRail::CheckStop()
{
    if(!MT_GetStop(miRAL_XLStp)) return false ;
    if(!MT_GetStop(miRAL_XVStp)) return false ;

    if(!AT_Done(aiRAL_LAlnFwBw)) return false ;
    if(!AT_Done(aiRAL_LStnUpDn)) return false ;
    if(!AT_Done(aiRAL_LStpUpDn)) return false ;
    if(!AT_Done(aiRAL_VAlnFwBw)) return false ;
    if(!AT_Done(aiRAL_VStnUpDn)) return false ;
    if(!AT_Done(aiRAL_VStpUpDn)) return false ;

    return true ;
}

void CRail::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\Rail.INI";

    //if ( _bLoad ) UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );

    //else {
    //    UserINI.ClearFile(sPath) ;
    //    UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
    //}

}
//---------------------------------------------------------------------------
