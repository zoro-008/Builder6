//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnLoader.h"
#include "PstBuff.h"
#include "Loader.h"
#include "UnLoaderMotor.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CUnLoader ULD;

CUnLoader::CUnLoader(void)
{
//    AT_MoveCyl(aiULD_TrayClamp , ccFwd);

    Init();
}
void CUnLoader::Init()
{
    m_sPartName = "UnLoader " ;
    iLastTrayPkgCnt    = 0 ;
    //iTrayCnt           = 0 ;
    iTrayGoodCavityCnt = 0 ;
    iTrayBadCavityCnt  = 0 ;
    Reset();
    Load(true);
}
void CUnLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CUnLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CUnLoader::~CUnLoader (void)
{
    Close();
}
void CUnLoader::Close (void)
{
    Load(false);
}
//---------------------------------------------------------------------------
bool CUnLoader::FindChipTray(int &r , int &c )
{
    if(!DM.ARAY[riTRY].GetCntStat(csWait)){
        DM.ARAY[riTRY].FindFrstColRow(csEmpty,r,c);
        //DM.ARAY[riTRY].FindFrstColLastRow(csEmpty ,r,c) ;
    }
    if( DM.ARAY[riTRY].GetCntStat(csWait)){
        DM.ARAY[riTRY].FindFrstColRow(csWait,r,c);
        //DM.ARAY[riTRY].FindFrstColLastRow(csWait ,r,c) ;
    }
//    return (r == -1 && c == -1);
    return (r > -1 && c > -1);
}
//---------------------------------------------------------------------------
bool CUnLoader::FindChipUpk(int &r , int &c )
{
    if(!DM.ARAY[riUPK].GetCntStat(csMask)){
        r = DM.ARAY[riUPK].FindLastRow(csEmpty) ;
        c = 0 ;
    }
    else {
        r = DM.ARAY[riUPK].FindLastRow(csMask);
        c = 0 ;
    }
    return (r > -1 && c > -1);
//    return (r == -1 && r == -1);
}
//---------------------------------------------------------------------------
double CUnLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r  , c  ;
    bool bOddRow ;

    FindChipTray(r , c);

    //X , Y Pos Setting.
    double dYPlacePos = PM.GetValue(miULD_YPck , pvULD_YPckPlaceStt) + (r * OM.DevInfo.dULDRowPitch) ;
    double dPos  = 0.0 ;

    if(_iMotr == miULD_XStp){
        switch(_iPstnId) {
            default                 : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piULD_XStpWait     : dPos = PM.GetValue (_iMotr , pvULD_XStpWait        ); break ;
            case piULD_XStpBwd      : dPos = PM.GetValue (_iMotr , pvULD_XStpBwd         ); break ;
            case piULD_XStpFwd      : dPos = PM.GetValue (_iMotr , pvULD_XStpFwd         ); break ;
        }
    }

    else if(_iMotr == miULD_YPck){
        switch(_iPstnId) {
            default                 : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piULD_YPckWait     : dPos = PM.GetValue (_iMotr , pvULD_YPckWait        ); break ;
            case piULD_YPckPick     : dPos = PM.GetValue (_iMotr , pvULD_YPckPick        ); break ;
            case piULD_YPckPaper    : dPos = PM.GetValue (_iMotr , pvULD_YPckPaper       ); break ;
            case piULD_YPckPlaceStt : dPos = PM.GetValue (_iMotr , pvULD_YPckPlaceStt    ); break ;
            case piULD_YPckPlace    : dPos = dYPlacePos                                   ; break ;
        }
    }

    else if(_iMotr == miULD_ZPck){
        switch(_iPstnId){
            default                 : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piULD_ZPckWait     : dPos = PM.GetValue (_iMotr , pvULD_ZPckWait        ); break ;
            case piULD_ZPckMove     : dPos = PM.GetValue (_iMotr , pvULD_ZPckMove        ); break ;
            case piULD_ZPckPick     : dPos = PM.GetValue (_iMotr , pvULD_ZPckPick        ); break ;
            case piULD_ZPckPlace    : dPos = PM.GetValue (_iMotr , pvULD_ZPckPlace       ); break ;
            case piULD_ZPckPaper    : dPos = PM.GetValue (_iMotr , pvULD_ZPckPaper       ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isBwTrayMove   = AT_Complete(aiULD_TrayClampMove , ccBwd);
//    bool isPickYMovePs  = MT_CmprPos(miULD_ZPck , piULD_ZPckPick ) || MT_CmprPos(miULD_ZPck , piULD_ZPckPaper) || MT_CmprPos(miULD_ZPck , piULD_ZPckPlace);
//    bool isPickZMovePs  = MT_CmprPos(miULD_YPck , piULD_YPckPaper) || MT_CmprPos(miULD_YPck , piULD_YPckPick)  || MT_CmprPos(miULD_YPck , piULD_YPckPlace);
    //bool isPickZMovePs  = MT_GetStop(miULD_ZPck) && MT_CmprPos(miULD_ZPck,PM.GetValue(miULD_ZPck,pvULD_ZPckMove)) ; //������ ������ �ؼ��̰� ���� �ݴ���(�ټ��� ����ϴ���)
    bool isZPckWaitPs  = MT_GetStop(miULD_ZPck) && (MT_CmprPos(miULD_ZPck , PM.GetValue(miULD_ZPck,pvULD_ZPckMove)) || MT_CmprPos(miULD_ZPck , PM.GetValue(miULD_ZPck,pvULD_ZPckWait)));
    bool isPickYStop     = MT_GetStop(miULD_YPck) ;//

    bool isPprRotatorBw = AT_Complete(aiULD_PprRotator , ccBwd);

    if(_iMotr == miULD_YPck){
        if(!isBwTrayMove  ) { sMsg = "Tray Move Cylinder is not Bwd"    ; bRet = false ;}
        switch(_iPstnId){
            default                 : break;
            case piULD_YPckWait     : if(!isZPckWaitPs                  ){ sMsg = MT_GetName(miULD_ZPck) + "is not Move Position"   ; bRet = false ;} break;
            case piULD_YPckPick     : if(!isZPckWaitPs                  ){ sMsg = MT_GetName(miULD_ZPck) + "is not Move Position"   ; bRet = false ;} break;
            case piULD_YPckPaper    : if(!isZPckWaitPs ||!isPprRotatorBw){ sMsg = MT_GetName(miULD_ZPck) + "is not Move Position || PprRttr is Not Bwd"   ; bRet = false ;} break;
            case piULD_YPckPlaceStt : if(!isZPckWaitPs                  ){ sMsg = MT_GetName(miULD_ZPck) + "is not Move Position"   ; bRet = false ;} break;
            case piULD_YPckPlace    : if(!isZPckWaitPs                  ){ sMsg = MT_GetName(miULD_ZPck) + "is not Move Position"   ; bRet = false ;} break;


        }
    }
    else if(_iMotr == miULD_ZPck){
        if(!isPickYStop){ sMsg = MT_GetName(miULD_YPck) + "Y Axis is Moving" ; bRet = false ;}
        switch(_iPstnId){
            default               : break;
            case piULD_ZPckWait   : break;
            case piULD_ZPckMove   : break;
            case piULD_ZPckPick   : break;
            case piULD_ZPckPlace  : break;
            case piULD_ZPckPaper  : break;
        }
    }
    else if(_iMotr == miULD_XStp){
        if(!isZPckWaitPs ){ sMsg = MT_GetName(miULD_ZPck) + "is not Move Position"   ; bRet = false ;}
        switch(_iPstnId){
            default               : break;
            case piULD_XStpWait   : break;
            case piULD_XStpBwd    : break;
            case piULD_XStpFwd    : break;
        }
    }




    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CUnLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;
    
    bool isBwTrayPick  = AT_Complete(aiULD_TrayClampPick , ccBwd);
    bool isYPckPlacePs = MT_GetStop(miULD_YPck) && (MT_CmprPos(miULD_YPck , PM.GetValue(miULD_YPck,pvULD_YPckWait)) ||
                                                    MT_CmprPos(miULD_YPck , PM.GetValue(miULD_YPck,pvULD_YPckPick)) ||
                                                    MT_CmprPos(miULD_YPck , PM.GetValue(miULD_YPck,pvULD_YPckPaper))    );
    bool isYPckPaperPs = MT_GetStop(miULD_YPck) &&  MT_CmprPos(miULD_YPck , PM.GetValue(miULD_YPck,pvULD_YPckPaper));
    bool isZPckWaitPs  = MT_GetStop(miULD_ZPck) && (MT_CmprPos(miULD_ZPck , PM.GetValue(miULD_ZPck,pvULD_ZPckMove)) || MT_CmprPos(miULD_ZPck , PM.GetValue(miULD_ZPck,pvULD_ZPckWait)));

    if(_iActr == aiULD_TrayClampMove) {
        if(!isBwTrayPick  ) { sMsg = "Tray Pick Cylinder is not Bwd"  ; bRet = false ;}
        if(!isYPckPlacePs ) { sMsg = "Y Pick Motor is not place position" ; bRet = false ;}
    }
    else if(_iActr == aiULD_PprRotator){
        if(isYPckPaperPs) { sMsg = "Y Pick Motor is Paper Position" ; bRet = false ;}
    }
    else if(_iActr == aiULD_TrayColSel){
        if(!isZPckWaitPs) { sMsg = "Z Pick Motor is Not Wait Position" ; bRet = false ;}
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CUnLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CUnLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}
bool CUnLoader::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 15000 )) {
        EM_SetErr(eiULD_HomeTO);
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

    if(Stat.bReqStop) {
        //Step.iHome = 0;
        //return true ;
    }
    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: //IO_SetY(yULD_PickVaccum1 , false);
                 //IO_SetY(yULD_PickVaccum2 , false);
                 //AT_MoveCyl(aiULD_Hold      , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: //if(!AT_MoveCyl(aiULD_Hold      , ccBwd)) return false ;

                 AT_MoveCyl(aiULD_TrayOpen      , ccBwd);
                 AT_MoveCyl(aiULD_TrayClampPick , ccBwd);
                 AT_MoveCyl(aiULD_PaperClamp    , ccBwd);
                 AT_MoveCyl(aiULD_TrayClampMove , ccBwd);
                 AT_MoveCyl(aiULD_PprRotator    , ccBwd);
//                 AT_MoveCyl(aiULD_TrayClamp , ccFwd); //�ʱ���°� Fwd ��� �����Ѵ�.
                 //AT_MoveCyl(aiULD_TrayClampMove , ccBwd);
                 Step.iHome++;
                 return false;

        case 12: if(!AT_MoveCyl(aiULD_TrayOpen      , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiULD_TrayClampPick , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiULD_PaperClamp    , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiULD_TrayClampMove , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiULD_PprRotator    , ccBwd)) return false ;

                 MT_DoHome(miULD_ZPck);
                 MT_DoHome(miULD_XStp);
                 AT_MoveCyl(aiULD_TrayClampMove , ccBwd);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miULD_ZPck)) return false;
                 if(!MT_GetHomeEnd(miULD_XStp)) return false;
                 if(!AT_MoveCyl(aiULD_TrayClampMove , ccBwd)) return false ;

                 MT_DoHome(miULD_YPck);
                 MT_GoAbsMan(miULD_ZPck , GetMotrPos(miULD_ZPck , piULD_ZPckWait));
                 MT_GoAbsMan(miULD_XStp , GetMotrPos(miULD_XStp , piULD_XStpWait));
                 Step.iHome++;
                 return false;

        case 14: if(!MT_GetHomeEnd(miULD_YPck)) return false;
                 if(!MT_GoAbsMan(miULD_ZPck , GetMotrPos(miULD_ZPck , piULD_ZPckWait))) return false;
                 if(!MT_GoAbsMan(miULD_XStp , GetMotrPos(miULD_XStp , piULD_XStpWait))) return false;
                 MT_GoAbsMan(miULD_YPck , GetMotrPos(miULD_YPck , piULD_YPckWait));
                 Step.iHome++;
                 return false;

        case 15: if(!MT_GoAbsMan(miULD_YPck , GetMotrPos(miULD_YPck , piULD_YPckWait))) return false;
                 AT_MoveCyl(aiULD_TrayColSel , ccBwd);

                 //AT_MoveCyl(aiULD_TrayClampPick , ccBwd);
                 Step.iHome++;
                 return false;

        case 16: //if(!AT_MoveCyl(aiULD_TrayClampPick , ccBwd)) return false;
                 if(!AT_MoveCyl(aiULD_TrayColSel , ccBwd)) return false ;
                 //AT_MoveCyl(aiULD_TrayClampMove , ccBwd);
                 Step.iHome++;
                 return false;

        case 17: //if(!AT_MoveCyl(aiULD_TrayClampMove , ccBwd)) return false;
                 //��δ����� ��û.
                 CUnLoaderMotor::SStat Stat ;
                 Stat = ULM.GetStat() ;
                 Stat.bReqPaper   = true ;
                 Stat.bReqInTray  = true ;
                 Stat.bReqOutTray = true ;
                 ULM.SetStat(Stat);

                 Step.iHome = 0;
                 return false;
    }
}

bool CUnLoader::GetLastStrip()
{
    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWR1Exist = !DM.ARAY[riWR1].CheckAllStat(csNone) ;
    bool bWR2Exist = !DM.ARAY[riWR2].CheckAllStat(csNone) ;
    bool bWR3Exist = !DM.ARAY[riWR3].CheckAllStat(csNone) ;
    bool bPSBExist = !DM.ARAY[riPSB].CheckAllStat(csNone) ;
    bool bULDExist = !DM.ARAY[riULD].CheckAllStat(csNone) ;

    bool bLDRDiff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riLDR].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bPRBDiff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riPRB].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bWR1Diff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riWR1].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bWR2Diff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riWR2].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bWR3Diff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riWR3].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bPSBDiff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riPSB].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;
    bool bULDDiff  = DM.ARAY[riUPK].GetLotNo() != DM.ARAY[riULD].GetLotNo() && DM.ARAY[riUPK].GetLotNo()!="" ;

    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

         if(bULDExist) return bULDDiff ;
    else if(bPSBExist) return bPSBDiff ;
    else if(bWR3Exist) return bWR3Diff ;
    else if(bWR2Exist) return bWR2Diff ;
    else if(bWR1Exist) return bWR1Diff ;
    else if(bPRBExist) return bPRBDiff ;
    else if(bLDRExist) return bLDRDiff ;
    else if(!bLastMgz) return false    ;
    else               return true     ;
}

bool CUnLoader::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        CUnLoaderMotor::SStat Stat ;
        Stat = ULM.GetStat() ;

        bool isExstStrip   = !DM.ARAY[riLDR].CheckAllStat(csNone) || !DM.ARAY[riPRB].CheckAllStat(csNone) ||
                             !DM.ARAY[riWR1].CheckAllStat(csNone) || !DM.ARAY[riWR2].CheckAllStat(csNone) ||
                             !DM.ARAY[riWR3].CheckAllStat(csNone) || !DM.ARAY[riPSB].CheckAllStat(csNone) ||
                             !DM.ARAY[riULD].CheckAllStat(csNone) || !DM.ARAY[riTRY].CheckAllStat(csNone) ||
                             !DM.ARAY[riUPK].CheckAllStat(csNone) ;

        bool isLastStrip     =  GetLastStrip();
        bool isTrayEnd       =  DM.ARAY[riTRY].CheckAllStat(csWork) && !DM.ARAY[riTRY].GetCntStat(csMask);
        bool isMgzChanged    = (DM.ARAY[riTRY].GetLotNo()!="" && DM.ARAY[riTRY].GetID().ToIntDef(999999)/1000 != DM.ARAY[riULD].GetID().ToIntDef(999999)/1000 && !DM.ARAY[riULD].CheckAllStat(csNone)) ||
                               (DM.ARAY[riTRY].GetLotNo()!="" && DM.ARAY[riTRY].GetID().ToIntDef(999999)/1000 != DM.ARAY[riPSB].GetID().ToIntDef(999999)/1000 && !DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riULD].CheckAllStat(csNone)) ;
        bool isPSBOutCycle   =  PSB.GetSeqStep() == CPstBuff::scOut ;

        bool isPaperWorking   = ULM.GetCycleStep() == CUnLoaderMotor::scPaper   || ULM.GetCycleStep() == CUnLoaderMotor::scPaperStt   || Stat.bReqPaper;
        bool isInTrayWorking  = ULM.GetCycleStep() == CUnLoaderMotor::scInTray  || ULM.GetCycleStep() == CUnLoaderMotor::scInTrayStt  || Stat.bReqInTray;
        bool isOutTrayWorking = ULM.GetCycleStep() == CUnLoaderMotor::scOutTray || ULM.GetCycleStep() == CUnLoaderMotor::scOutTrayStt || Stat.bReqOutTray;

        // Cycle.
        bool isCycleTrayHold = IO_GetX(xULD_TrayPushDetect) && !AT_Complete(aiULD_Hold , ccFwd);
        bool isCycleIn       = isPSBOutCycle && !MT_CmprPos(miULD_YPck , PM.GetValue(miULD_YPck , pvULD_YPckPaper));     //(!IO_GetX(xULD_Pkg1) || !IO_GetX(xULD_Pkg2) || !IO_GetX(xULD_Pkg3)) &&
        bool isCycleWorkPpr  = (DM.ARAY[riTRY].GetCntStat(csEmpty) || DM.ARAY[riTRY].GetCntStat(csWait)) && (DM.ARAY[riUPK].CheckAllStat(csNone) || DM.ARAY[riUPK].GetCntStat(csMask)) && IO_GetX(xULD_PaperDetect) &&
                               !isPaperWorking && !isInTrayWorking && DM.ARAY[riTRY].GetStep() == 0 ;
        bool isCycleWorkPkg  = !DM.ARAY[riULD].CheckAllStat(csNone) && DM.ARAY[riUPK].GetCntStat(csEmpty) && !DM.ARAY[riUPK].GetCntStat(csMask) && AT_Complete(aiULD_Hold , ccFwd) && !isInTrayWorking;

        //bool isCycleWorkPkg  =!DM.ARAY[riULD].CheckAllStat(csNone) && !DM.ARAY[riUPK].GetCntStat(csMask) && (DM.ARAY[riTRY].GetCntStat(csEmpty) || DM.ARAY[riUPK].GetCntStat(csEmpty)) &&
        //                       AT_Complete(aiULD_Hold , ccFwd) && !isInTrayWoking; //TODO :: �̰� ���� �̻��Ѱ� ������...riULD �� csWork �� ��������� ������ ����.
        //bool isCycleWorkPpr  = DM.ARAY[riUPK].GetCntStat(csMask ) && IO_GetX(xULD_PaperDetect) && !isPaperWoking && DM.ARAY[riTRY].GetStep() == 0;
        bool isCycleTrayMove = AT_Complete(aiULD_Hold , ccFwd) && (isTrayEnd || isMgzChanged && //(!isExstStrip && LDR.GetStat().bWorkEnd)) &&
                             (!isOutTrayWorking && !isInTrayWorking)) || DM.ARAY[riTRY].GetStep() == 1 ;

        bool isCycleEnd      = DM.ARAY[riTRY].CheckAllStat(csNone ) && !isExstStrip && LDR.GetStat().bWorkEnd;//  isPreEqpEnd  ;


        //�𸣴� ��Ʈ������.
        //ī��Ʈ �����.
        if(!DM.ARAY[riULD].CheckAllStat(csNone) && !IO_GetX(xULD_Pkg1) && !IO_GetX(xULD_Pkg2) && !IO_GetX(xULD_Pkg3))   EM_SetErr(eiULD_PkgDispr  ) ; //sun bDryRun �ɼ� �߰�.
        //ī��Ʈ OUT �Ű��� Ǯ
        if( DM.ARAY[riULD].CheckAllStat(csNone) && (IO_GetX(xULD_Pkg1) ||  IO_GetX(xULD_Pkg2) ||  IO_GetX(xULD_Pkg3)))  EM_SetErr(eiULD_PkgUnknown) ; //sun bDryRun �ɼ� �߰�.


        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleTrayHold) {Trace(m_sPartName.c_str(),"CycleTrayHold Stt"); Step.iSeq = scTrayHold ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleIn      ) {Trace(m_sPartName.c_str(),"CycleIn       Stt"); Step.iSeq = scIn       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWorkPkg ) {Trace(m_sPartName.c_str(),"CycleWorkPkg  Stt"); Step.iSeq = scWorkPkg  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWorkPpr ) {Trace(m_sPartName.c_str(),"CycleWorkPpr  Stt"); Step.iSeq = scWorkPpr  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleTrayMove) {Trace(m_sPartName.c_str(),"CycleTrayMove Stt"); Step.iSeq = scTrayMove ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default         :                      Trace(m_sPartName.c_str(),"default       End");Step.iSeq = scIdle ;  return false ;
        case scIdle     :                                                                                           return false ;
        case scTrayHold : if(CycleTrayHold()){ Trace(m_sPartName.c_str(),"CycleTrayHold End");Step.iSeq = scIdle ;} return false ;
        case scIn       : if(CycleIn      ()){ Trace(m_sPartName.c_str(),"CycleIn       End");Step.iSeq = scIdle ;} return false ;
        case scWorkPkg  : if(CycleWorkPkg ()){ Trace(m_sPartName.c_str(),"CycleWorkPkg  End");Step.iSeq = scIdle ;} return false ;
        case scWorkPpr  : if(CycleWorkPpr ()){ Trace(m_sPartName.c_str(),"CycleWorkPpr  End");Step.iSeq = scIdle ;} return false ;
        case scTrayMove : if(CycleTrayMove()){ Trace(m_sPartName.c_str(),"CycleTrayMove End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CUnLoader::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CUnLoader::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CUnLoader::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiULD_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    int iRow , iCol;
    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: //MoveActr(aiULD_TrayClamp , ccBwd);
                 MoveMotr(miULD_ZPck,piULD_ZPckMove);
                 //MoveActr(aiULD_Hold , ccBwd);
                 Step.iToStart++ ;
                 return false ;

        case 11: //if(!MoveActr(aiULD_TrayClamp , ccBwd)) return false;
                 if(!MoveMotr(miULD_ZPck,piULD_ZPckMove)) return false ;
                 //if(!MoveActr(aiULD_Hold      , ccBwd)) return false;
                 MoveActr(aiULD_TrayClampPick , ccBwd);
                 MoveActr(aiULD_TrayOpen      , ccBwd);
                 Step.iToStart++;
                 return false;

        case 12: if(!MoveActr(aiULD_TrayClampPick , ccBwd)) return false;
                 if(!MoveActr(aiULD_TrayOpen      , ccBwd)) return false;
                 MoveActr(aiULD_TrayClampMove , ccBwd);

                 Step.iToStart++;
                 return false;

        case 13: if(!MoveActr(aiULD_TrayClampMove , ccBwd)) return false;
                 FindChipTray(iRow , iCol);
                 if(iCol == 0) { MoveMotr(miULD_XStp , piULD_XStpFwd); }
                 else          { MoveMotr(miULD_XStp , piULD_XStpBwd); }
                 Step.iToStart++;
                 return false;

        case 14: FindChipTray(iRow , iCol);
                 if     (iCol == 0 && !MoveMotr(miULD_XStp , piULD_XStpFwd)) return false;
                 else if(iCol != 0 && !MoveMotr(miULD_XStp , piULD_XStpBwd)) return false;
                 CUnLoaderMotor::SStat Stat ;
                 Stat = ULM.GetStat() ;
                 Stat.bReqPaper   = true ;
                 Stat.bReqInTray  = true ;
                 Stat.bReqOutTray = true ;
                 ULM.SetStat(Stat);

                 Step.iToStart = 0;
                 return true;
    }
}

bool CUnLoader::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 8000)) EM_SetErr(eiULD_ToStopTO);

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

        case 10: //IO_SetY(yULD_PickVaccum1 , false);
                 //IO_SetY(yULD_PickVaccum2 , false);
                 //MoveActr(aiULD_TrayClamp , ccBwd);
                 MoveMotr(miULD_ZPck,piULD_ZPckMove);
                 //MoveActr(aiULD_Hold , ccBwd);
                 Step.iToStop ++;
                 return false;

        case 11: //if(!MoveActr(aiULD_TrayClamp , ccBwd)) return false;
                 if(!MoveMotr(miULD_ZPck,piULD_ZPckMove)) return false ;
                 //if(!MoveActr(aiULD_Hold      , ccBwd)) return false;
                 MoveActr(aiULD_TrayClampPick , ccBwd);
                 MoveActr(aiULD_TrayOpen      , ccBwd);
                 Step.iToStop ++;
                 return false;

        case 12: if(!MoveActr(aiULD_TrayClampPick , ccBwd)) return false;
                 if(!MoveActr(aiULD_TrayOpen      , ccBwd)) return false;
                 MoveActr(aiULD_TrayClampMove , ccBwd);
                 Step.iToStop ++;
                 return false;

        case 13: if(!MoveActr(aiULD_TrayClampMove , ccBwd)) return false;
                 IO_SetY(yULD_FeedingAC , false);
                 Step.iToStop = 0;
                 return true ;
    }
}

bool CUnLoader::CycleTrayHold(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        EM_SetErr(eiLDR_CycleTO);
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
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiULD_TrayOpen , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiULD_TrayOpen , ccBwd)) return false;
                  if(!IO_GetX(xULD_TrayPushDetect)){
                      EM_SetErr(eiULD_TrayPshDetect);
                      Step.iCycle = 0;
                      return true;
                  }
                  MoveActr(aiULD_Hold , ccFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiULD_Hold , ccFwd)) return false;
                  DM.ARAY[riTRY].SetStat(csNone);
                  DM.ARAY[riUPK].SetStat(csNone);

                  //��δ����� ��û.
                  CUnLoaderMotor::SStat Stat ;
                  Stat = ULM.GetStat() ;
                  Stat.bReqInTray  = true ;
                  Stat.bReqOutTray = true ;
                  ULM.SetStat(Stat);

                  Step.iCycle = 0;
                  return true;
    }
}

//One Cycle.
bool CUnLoader::CycleIn(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiULD_CycleTO);
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

    static int iRow , iCol;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yULD_FeedingAC , true);
                  if( DM.ARAY[riTRY].GetCntStat(csWait)                                           ) DM.ARAY[riTRY].FindFrstColRow(csWait  , iRow, iCol);
                  //TODO :: �Ʒ��� ���� �߰� �ȵ���.. ���߿� �߰� ������...JS
                  else if(!DM.ARAY[riTRY].GetCntStat(csWait) && DM.ARAY[riTRY].GetCntStat(csEmpty)) DM.ARAY[riTRY].FindFrstColRow(csEmpty , iRow, iCol);
                  else                                                                              iCol = 0 ;

                  Step.iCycle++;
                  return false;

        case  11: //FindChipTray(iRow , iCol);
                  if(iCol == 0){MoveMotr(miULD_XStp , piULD_XStpFwd);}
                  else         {MoveMotr(miULD_XStp , piULD_XStpBwd);}
                  Step.iCycle++;
                  return false;

        case  12: //FindChipTray(iRow , iCol);
                  if     (iCol == 0 && !MoveMotr(miULD_XStp , piULD_XStpFwd)) return false;
                  else if(iCol != 0 && !MoveMotr(miULD_XStp , piULD_XStpBwd)) return false;
                  IO_SetY(yULD_FeedingAC , true);
                  //IO_SetY(yULD_FeedingAC , true);
                  //m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  13: //FindChipTray(iRow , iCol);
                  if     (iCol == 0 && !IO_GetX(xULD_Pkg1)) return false ;
                  else if(iCol != 0 && !IO_GetX(xULD_Pkg3)) return false ;
                  //if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPsbToOutDelay))return false;
                  if(IO_GetX(xULD_PkgIn)){
                      EM_SetErr(eiULD_PkgDispr);
                      IO_SetY(yULD_FeedingAC , false);
                      Step.iCycle = 0;
                      return true ;
                  }

                  m_tmTemp.Clear() ;

                  Step.iCycle++;
                  return false ;

        case  14: //��δ����� ��û.
                  CUnLoaderMotor::SStat Stat ;
                  Stat = ULM.GetStat() ;
                  Stat.bReqPaper  = true ;
                  ULM.SetStat(Stat);

                  if(!m_tmTemp.OnDelay(true, 500)) return false ; //TODO :: �̽��׸� #1
                  Step.iCycle = 0;
                  return true;
    }
}

bool CUnLoader::CycleWorkPkg ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 7000 )) {
        EM_SetErr(eiULD_CycleTO);
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

    int iTRYRow , iTRYCol;
    int iUPKRow , iUPKCol ;
    int iTrayTemp;

    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(!MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  FindChipTray(iTRYRow, iTRYCol);
                  IO_SetY(yULD_PickEjectr1 , false);
                  IO_SetY(yULD_PickEjectr2 , false);
                  IO_SetY(yULD_PickVaccum1 , false);
                  IO_SetY(yULD_PickVaccum2 , false);
                  IO_SetY(yULD_FeedingAC   , true );
                  if(iTRYCol == 0) {
                      MoveMotr(miULD_XStp , piULD_XStpFwd) ;
                      MoveActr(aiULD_TrayColSel , ccBwd);
                  }
                  else {
                      MoveMotr(miULD_XStp , piULD_XStpBwd) ;
                      MoveActr(aiULD_TrayColSel , ccFwd);
                  }
                  MoveMotr(miULD_YPck , piULD_YPckPick);
                  Step.iCycle++;
                  return false ;

        case  11: //if(!MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false ;
                  FindChipTray(iTRYRow, iTRYCol);
                       if(iTRYCol == 0 && !MoveMotr(miULD_XStp , piULD_XStpFwd)) return false;
                  else if(iTRYCol != 0 && !MoveMotr(miULD_XStp , piULD_XStpBwd)) return false;
                  Step.iCycle++;
                  return false;

        case  12: FindChipTray(iTRYRow, iTRYCol);
                       if(iTRYCol == 0 && !MoveActr(aiULD_TrayColSel , ccBwd))return false;
                  else if(iTRYCol != 0 && !MoveActr(aiULD_TrayColSel , ccFwd))return false;

                  if(!MoveMotr(miULD_YPck , piULD_YPckPick)) return false ;
                  MoveMotr(miULD_ZPck , piULD_ZPckPick);

                  IO_SetY(yULD_PickVaccum1 , true);
                  IO_SetY(yULD_PickVaccum2 , true);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miULD_ZPck , piULD_ZPckPick)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.dULDVaccumDeley)) return false;

                  MoveMotr(miULD_ZPck, piULD_ZPckMove);
                  IO_SetY(yULD_FeedingAC , false);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  if(!IO_GetX(xULD_PkgVaccum)){ /*IO_GetX(xULD_Pkg1) || IO_GetX(xULD_Pkg2) || IO_GetX(xULD_Pkg3) ||*/
                      EM_SetErr(eiULD_PkgVaccumMiss);
                      Step.iCycle = 0;
                      return true ;
                  }
                  MoveMotr(miULD_YPck , piULD_YPckPlace);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miULD_YPck , piULD_YPckPlace)) return false;
                  MoveMotr(miULD_ZPck , piULD_ZPckPlace);
                  Step.iCycle++;
                  return false;

        case  17: LT.WriteArayData(riULD); //�� ��ũ �̰ٴ�. SPC �� ����Ʈ ����. ���� ���� �ȳ������ ���⼭ ��.
                  LotInfoAdd (); //TODO :: ���⵵ ���� �̻���. ���⼭ ����� �ؿ��� �ȸ���?

                  //���⿡�� SPC ������ ����..JS
                  iTrayGoodCavityCnt = DM.ARAY[riULD].GetCntStat(csWork);
                  LT.TrayInfo.iTrayGoodCavity += iTrayGoodCavityCnt;

                  iTrayBadCavityCnt = (OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt) - iTrayGoodCavityCnt;
                  LT.TrayInfo.iTrayBadCavity += iTrayBadCavityCnt;

                  Step.iCycle++;
                  return false;

        case  18: if(!MoveMotr(miULD_ZPck , piULD_ZPckPlace)) return false;
                  /*
                  if(IO_GetX(xULD_Vaccum1 , false) || IO_GetX(xULD_Vaccum2 , false)){
                      EM_SetErr(eiULD_VaccumMiss);
                      Step.iCycle = 0;
                      return true;
                  }*/
                  IO_SetY(yULD_PickEjectr1 , true);
                  IO_SetY(yULD_PickEjectr2 , true);

                  IO_SetY(yULD_PickVaccum1 , false);
                  IO_SetY(yULD_PickVaccum2 , false);

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  19: if(!m_tmTemp.OnDelay(true ,OM.CmnOptn.iUldPkgEjtTime )) return false ;
                  MoveMotr(miULD_ZPck , piULD_ZPckMove);
                  //FindChipTray(iTRYRow , iTRYCol);
                  //if(!DM.ARAY[riTRY].GetCntStat(csWait)){
                  //    DM.ARAY[riTRY].SetStat(iTRYRow , iTRYCol , csWait);
                  //}
                  Step.iCycle++;
                  return false;

        case  20: if(!MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  IO_SetY(yULD_PickEjectr1 , false);
                  IO_SetY(yULD_PickEjectr2 , false);
                  //if(DM.ARAY[riUPK].CheckAllStat(csNone)){
                  //    DM.ARAY[riUPK].SetStat(csEmpty);
                  //    DM.ARAY[riUPK].SetStat(1,1,csMask);
                  //}
                  //if(DM.ARAY[riUPK].GetCntStat(csNone)){
                  //    DM.ARAY[riUPK].ChangeStat(csNone , csEmpty);
                  //}
                  //TODO :: ������ �Ϸ� �ű� DM.ARAY[riULD].SetStat(csNone) ;
                  DM.ARAY[riULD].SetStat(csNone) ;
                  FindChipUpk(iUPKRow , iUPKCol);
                  DM.ARAY[riUPK].SetStat(iUPKRow , iUPKCol , csMask);
                  //MoveMotr(miULD_YPck , piULD_YPckPaper);
                  MoveActr(aiULD_TrayColSel , ccBwd);
                  Step.iCycle = 0;
                  return true ;

    }

}
bool CUnLoader::CycleWorkPpr ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 7000 )) {
        EM_SetErr(eiULD_CycleTO);
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
    int iTRYRow , iTRYCol ;
    int iUPKRow , iUPKCol ;
    //static int iVaccumChek;

    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(!MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  MoveActr(aiULD_PprRotator , ccBwd);
                  FindChipTray(iTRYRow , iTRYCol);
                  if(!DM.ARAY[riTRY].GetCntStat(csWait)){
                      DM.ARAY[riTRY].SetStat(iTRYRow , iTRYCol , csWait);
                  }
                  IO_SetY(yULD_PickEjectr1 , false);
                  IO_SetY(yULD_PickEjectr2 , false);
                  IO_SetY(yULD_PickVaccum1 , false);
                  IO_SetY(yULD_PickVaccum2 , false);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiULD_PprRotator , ccBwd)) return false;
                  if(DM.ARAY[riUPK].CheckAllStat(csNone)){
                      DM.ARAY[riUPK].SetStat(csEmpty);
//                      DM.ARAY[riUPK].SetStat(OM.DevInfo.iULDPkgCnt - 1 ,0,csMask); //���� ó���� ������� �׷��Ŵ�?
                  }
                  if(DM.ARAY[riUPK].GetCntStat(csNone)){
                      DM.ARAY[riUPK].ChangeStat(csNone , csEmpty);
                  }
                  MoveMotr(miULD_YPck , piULD_YPckPaper);
                  MoveActr(aiULD_TrayColSel , ccBwd    ); // �ٽ� �ٲ�޶�. �����.
                  //MoveActr(aiULD_TrayColSel , ccBwd    );
                  //IO_SetY(yULD_PickEjectr1 , false);
                  //IO_SetY(yULD_PickEjectr2 , false);
                  //m_tmTemp.Clear();
                  //;
                  //;
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_YPck , piULD_YPckPaper)) return false;
//                  MoveActr(aiULD_TrayColSel , ccBwd    );
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiULD_TrayColSel , ccBwd    )) return false;

                  MoveActr(aiULD_PaperClamp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiULD_PaperClamp , ccFwd)) return false;
                  MoveMotr(miULD_ZPck , piULD_ZPckPaper);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miULD_ZPck , piULD_ZPckPaper)) return false;
                  IO_SetY(yULD_PickVaccum1 , true);
                  IO_SetY(yULD_PickVaccum2 , true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  16: if(!m_tmTemp.OnDelay(true, OM.CmnOptn.dULDVaccumDeley)) return false ;
                  MoveMotr(miULD_ZPck , piULD_ZPckMove);
                  MoveActr(aiULD_PaperClamp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  if(!MoveActr(aiULD_PaperClamp , ccBwd   )) return false;
                  if(!IO_GetX(xULD_PprVaccum)) {
                      FindChipUpk(iUPKRow , iUPKCol);
                      if(!DM.ARAY[riUPK].GetCntStat(csMask)) {
                          DM.ARAY[riUPK].SetStat(iUPKRow, iUPKCol, csMask);
                      }
                      MoveMotr(miULD_YPck , piULD_YPckPick);
                      EM_SetErr(eiULD_PaperVaccumMiss);
                      IO_SetY(yULD_PickVaccum1 , false);
                      IO_SetY(yULD_PickVaccum2 , false);
                      Step.iCycle=0;
                      return true;
                  }
                  FindChipTray(iTRYRow , iTRYCol);
                  if(iTRYCol == 0) { MoveActr(aiULD_TrayColSel , ccBwd); }
                  if(iTRYCol != 0) { MoveActr(aiULD_TrayColSel , ccFwd); }
                  MoveMotr(miULD_YPck , piULD_YPckPlace); // �������û.
                  Step.iCycle++;
                  return false ;

        case  18: FindChipTray(iTRYRow , iTRYCol);
                  if(iTRYCol == 0 && !MoveActr(aiULD_TrayColSel , ccBwd)) return false ;
                  if(iTRYCol != 0 && !MoveActr(aiULD_TrayColSel , ccFwd)) return false ;
                  //MoveMotr(miULD_YPck , piULD_YPckPlace);
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveMotr(miULD_YPck , piULD_YPckPlace)) return false;
                  MoveActr(aiULD_PprRotator , ccFwd    );
                  MoveMotr(miULD_ZPck , piULD_ZPckPlace);
                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveMotr(miULD_ZPck , piULD_ZPckPlace)) return false;
                  if(!MoveActr(aiULD_PprRotator , ccFwd    )) return false;
                  IO_SetY(yULD_PickVaccum1 , false);
                  IO_SetY(yULD_PickVaccum2 , false);
                  IO_SetY(yULD_PickEjectr1 , true );
                  IO_SetY(yULD_PickEjectr2 , true );
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  21: if(!m_tmTemp.OnDelay(true,OM.CmnOptn.iUldPprEjtTime)) return false ;
                  //IO_SetY(yULD_PickEjectr1 , false);
                  //IO_SetY(yULD_PickEjectr2 , false);
                  MoveMotr(miULD_ZPck , piULD_ZPckMove);
                  Step.iCycle++;
                  return false ;

        case  22: if(!MoveMotr(miULD_ZPck , piULD_ZPckMove)) return false;
                  IO_SetY(yULD_PickEjectr1 , false);
                  IO_SetY(yULD_PickEjectr2 , false);
                  //������ ã�ƺ���
                  FindChipUpk(iUPKRow , iUPKCol);
                  DM.ARAY[riUPK].SetStat(iUPKRow , iUPKCol , csWork);
                  Step.iCycle++;
                  return false;

        case  23: //������ �����̸�
                  if(DM.ARAY[riUPK].CheckAllStat(csWork)) {
                      //Ʈ���̿��� ��Ƽ Ȥ�� ����Ʈ  ã�ƺ���.
                      FindChipTray(iTRYRow , iTRYCol);  //����ũ Ȥ�� ��Ƽ ����.
                      DM.ARAY[riTRY].SetStat(iTRYRow , iTRYCol , csWork);
                      DM.ARAY[riUPK].SetStat(csNone);
                  }
                  if(GetLastStrip()){
                      DM.ARAY[riTRY].SetStep(1);
                      Step.iCycle++;
                      return false;
                  }
                  MoveMotr(miULD_YPck , piULD_YPckPick);
                  Step.iCycle++;
                  return false ;

        case  24: if(!MoveMotr(miULD_YPck , piULD_YPckPick)) return false;
                  Step.iCycle = 0;
                  return true ;

    }
}

bool CUnLoader::CycleTrayMove ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiULD_CycleTO);
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

    int iUPKCnt;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //MoveActr(aiULD_TrayClampMove  , ccBwd);
                  //MoveActr(aiULD_TrayClamp      , ccBwd);
                  MoveActr(aiULD_TrayClampPick  , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: //if(!MoveActr(aiULD_TrayClampMove  , ccBwd)) return false;
                  //if(!MoveActr(aiULD_TrayClamp      , ccBwd)) return false;
                  if(!MoveActr(aiULD_TrayClampPick  , ccBwd)) return false;
                  MoveActr(aiULD_TrayClampMove  , ccBwd);
                  MoveActr(aiULD_TrayClamp      , ccBwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiULD_TrayClampMove  , ccBwd)) return false;
                  if(!MoveActr(aiULD_TrayClamp      , ccBwd)) return false;
                  MoveActr(aiULD_TrayClampMove , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiULD_TrayClampMove , ccFwd)) return false;
                  MoveActr(aiULD_TrayClampPick , ccFwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiULD_TrayClampPick , ccFwd)) return false;
                  MoveActr(aiULD_TrayClamp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiULD_TrayClamp , ccFwd)) return false;
                  MoveActr(aiULD_TrayClampPick , ccBwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiULD_TrayClampPick , ccBwd)) return false;
                  MoveActr(aiULD_TrayClampMove , ccBwd);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiULD_TrayClampMove , ccBwd)) return false;
                  MoveActr(aiULD_TrayClampPick , ccFwd);
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveActr(aiULD_TrayClampPick , ccFwd)) return false;
                  MoveActr(aiULD_TrayClamp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  19: if(!MoveActr(aiULD_TrayClamp , ccBwd)) return false;
                  MoveActr(aiULD_TrayClampPick , ccBwd);
                  //LOT END ���� ����.
                  sTemp = "UPK csWork : " +  (String)DM.ARAY[riUPK].GetCntStat(csWork) + "TRY csWork : " +  (String)DM.ARAY[riTRY].GetCntStat(csWork) + "ULDPkgCnt : " + (String)OM.DevInfo.iULDPkgCnt ;
                  Trace(m_sPartName.c_str(),sTemp.c_str());
                  if(DM.ARAY[riUPK].GetCntStat(csWork)) iUPKCnt = DM.ARAY[riUPK].GetCntStat(csWork) - 1 ;
                  else                                  iUPKCnt = 0 ;
                  LT.LotInfo.iLastTrayCnt = (DM.ARAY[riTRY].GetCntStat(csWork) * (OM.DevInfo.iULDPkgCnt - 1 )) + iUPKCnt; //TODO :: ?? ���ؼ��� ����!!! 11���� ���ڰ� �׷��� �׳��� ����
                  LT.WriteTrayLog  ();

                  DM.ARAY[riUPK].SetStat(csNone);
                  DM.ARAY[riTRY].SetStat(csNone);
                  Step.iCycle++;
                  return false;

        case  20: if(!MoveActr(aiULD_TrayClampPick , ccBwd)) return false;
                  DM.ARAY[riTRY].SetStep(0);
                  LT.TrayInfo.iTrayNo++;
                  LT.TrayInfo.iTrayGoodCavity = 0;
                  LT.TrayInfo.iTrayBadCavity  = 0;
                  if(GetLastStrip()){
//TODO :: dd ���䷱���� ������ ��Ű�µ� �� ���⼭ �ϴ��� �𸣰ٳ� �� ���� ���� �ϴµ� �ٽ� �־�� �Ǹ� �־�.
//�̷��� �ѸŰ��� �����θ� �������ߵ�. �ϴ� �� LOT END �ϰ� ������ �ִ°͵�...
//                    LT.LotEnd();
//                    DM.ClearMap();

//                      m_bLotEnded = true ; //�̰� �Ⱦ��°� �ƴѰ�? �ϴ� �ּ�.
//                      memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo)); //�ʵ�...

                      Trace("LotEnd",DM.ARAY[riULD].GetLotNo().c_str());
                      LT.LotInfo.dEndTime = Now();

//                      ULD.EndedLot.iLastTrayCnt = iLastTrayPkgCnt; // ���� �־��ִµ��� ����...iLastTrayPkgCnt �̰� �ٵ� ���µ��� ����...

                      LT.WritePSBLog   ();
                      LT.WriteLotLog   ();
                      LT.WriteLotDayLog();
                      Trace("WriteLotLog",DM.ARAY[riULD].GetLotNo().c_str());
                      //LT.LotEnd();
                      LT.PSBInfo.iPSBStripCnt   = 0;
                      LT.PSBInfo.iPSBGoodCavity = 0;
                      LT.PSBInfo.iPSBBadCavity  = 0;

//                      ULD.EndedLot.iLastTrayCnt = iLastTrayPkgCnt;
                      DM.ARAY[riTRY].SetLotNo("");
                      //TODO :: ���� dd
                      //�̸��Ͽ� �ϴ��� PSB �� ���� ����� LT1���� �����...
                      //������ ������ ���� �ȹٲ��� �����ʹ� ����ɰ� �����ѵ� ġ�����̿��� �ű�� ���� �ٲ�. ������ ����� �����̶� �ѱ�� ������ �ȸ´°� � �־ �Ű�µ�
                      //�� �ȵǼ� �Ű����...�ѹ� ����ߴ�...View �� TODO ����Ʈ �����.
                      //AUTORUN ���ǿ� �ϳ� ULD csWork ������ �����ϴ°� �ϳ� �־��µ� �̰� csNone �ƴҶ��� ������. ULD �� Work �� �������� ����...�ٵ����� �� �����̰ų�
                      //������ ����°� �ϴ� ���⿡ ���ѹ� �տ� ��¥ ������ ���̱淡 �״�� �ؼ� �ٸ��͵� ������.
                      //SPC �� �׽�Ʈ ��ư �ϳ� ���� Visible �� ����. (��ư���� �ϸ� ���°Ŵ� Ȯ��)
                      //iLastTrayCnt �̰� ...�� �̻���... LT.LotInfo.iLastTrayCnt �� �ٷ� �ѱ�� �ؿ��� �����ϰ���.
                      //��Ÿ �ð�����... ����

                      //PSB FAIL MAP �߰�
                      //�� �����̳� ������ �󺧷� ��޶�� �ϸ� ���ּ�. dd

                  }
                  else {
                        //��δ����� ��û.
                        CUnLoaderMotor::SStat Stat ;
                        Stat = ULM.GetStat() ;
                        Stat.bReqInTray  = true ;
                        Stat.bReqOutTray = true ;
                        ULM.SetStat(Stat);
                  }

                  Step.iCycle = 0;
                  return true;
    }
}


void CUnLoader::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CUnLoader::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CUnLoader::CheckStop()
{
    if(!MT_GetStop(miULD_XStp           )) return false ;
    if(!MT_GetStop(miULD_YPck           )) return false ;
//    if(!MT_GetStop(miULD_ZPpr           )) return false ;
    if(!MT_GetStop(miULD_ZPck           )) return false ;
//    if(!MT_GetStop(miULD_ZTrI           )) return false ;
//    if(!MT_GetStop(miULD_ZTrO           )) return false ;

    if(!AT_Done   (aiULD_Hold           )) return false ;     //aiULD_TrayClamp     = 19 ,  // UnLoader Clamp
    if(!AT_Done   (aiULD_TrayColSel     )) return false ;     //aiULD_TrayClampPick = 20 ,  // UnLoader Pick Up
    if(!AT_Done   (aiULD_TrayOpen       )) return false ;     //aiULD_TrayClampMove = 21 ,  // UnLoader Moving
    if(!AT_Done   (aiULD_TrayClamp      )) return false ;     //aiULD_TrayColSel    = 22 ,  // xULD_TrayColSelRt
    if(!AT_Done   (aiULD_TrayClampMove  )) return false ;     //aiULD_TrayOpen      = 23 ,  // xULD_TrayOpenFtFw
    if(!AT_Done   (aiULD_TrayClampPick  )) return false ;     //aiULD_Hold          = 24 ,  // UnLoader Hold
    if(!AT_Done   (aiULD_PaperClamp     )) return false ;     //aiULD_PaperClamp    = 25 ,  // UnLoader Paper Clamp Cylinder
    if(!AT_Done   (aiULD_PprRotator     )) return false ;     //aiULD_PprRotator    = 26 ,  // UnLoader Paper Detece Rotator Cylinder
    return true ;
}

void CUnLoader::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iLastTrayPkgCnt    " , &iLastTrayPkgCnt   );
    }
    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iLastTrayPkgCnt    " ,  iLastTrayPkgCnt   );
    }

}
//---------------------------------------------------------------------------
bool CUnLoader::LotInfoAdd  ()
{
//    DM.ARAY[riLOT].ChangeStat(csUnkwn,csWork);
    LT.AddDayInfoWorkStrp(1);
    LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

    LT.AddLotInfoWorkStrp(1);
    LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

    LT.AddDayInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riULD].GetCntStat(csWork));
    LT.AddLotInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riULD].GetCntStat(csWork));
//    iVsFailCnt         = OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riLOT].GetCntStat(csWork);
    OM.iTotalChip     += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt ;
    OM.iTotalFailChip += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riULD].GetCntStat(csWork) ;

    if(OM.iTotalChip < 0 || OM.iTotalFailChip < 0) {//OverFlow
        OM.iTotalChip     = 0 ;
        OM.iTotalFailChip = 0 ;
    }
    LT.AddDayInfoChips(DM.ARAY[riULD].GetCntStat(csRslt1 ) , //"V Empty"
                       DM.ARAY[riULD].GetCntStat(csRslt2 ) , //"V Chip"
                       DM.ARAY[riULD].GetCntStat(csRslt3 ) , //"V Dust"
                       DM.ARAY[riULD].GetCntStat(csRslt4 ) , //"V LFlow"
                       DM.ARAY[riULD].GetCntStat(csRslt5 ) , //"V Broken"
                       DM.ARAY[riULD].GetCntStat(csRslt6 ) , //"V Crack"
                       DM.ARAY[riULD].GetCntStat(csRslt7 ) , //"V Burr"
                       DM.ARAY[riULD].GetCntStat(csRslt8 ) , //"V Zener"
                       DM.ARAY[riULD].GetCntStat(csRslt9 ) , //"V Wire"
                       DM.ARAY[riULD].GetCntStat(csRslt10) , //"V Orient"
                       DM.ARAY[riULD].GetCntStat(csRslt11) , //"V Pmeasure"
                       DM.ARAY[riULD].GetCntStat(csRslt12) , //"V Pdist"
                       DM.ARAY[riULD].GetCntStat(csRslt13) , //"V Match"
                       DM.ARAY[riULD].GetCntStat(csRslt14) , //"V Flow"
                       DM.ARAY[riULD].GetCntStat(csFail  ) ); //"Heat Fail");

    LT.AddLotInfoChips(DM.ARAY[riULD].GetCntStat(csRslt1 ) , //"V Empty"
                       DM.ARAY[riULD].GetCntStat(csRslt2 ) , //"V Chip"
                       DM.ARAY[riULD].GetCntStat(csRslt3 ) , //"V Dust"
                       DM.ARAY[riULD].GetCntStat(csRslt4 ) , //"V LFlow"
                       DM.ARAY[riULD].GetCntStat(csRslt5 ) , //"V Broken"
                       DM.ARAY[riULD].GetCntStat(csRslt6 ) , //"V Crack"
                       DM.ARAY[riULD].GetCntStat(csRslt7 ) , //"V Burr"
                       DM.ARAY[riULD].GetCntStat(csRslt8 ) , //"V Zener"
                       DM.ARAY[riULD].GetCntStat(csRslt9 ) , //"V Wire"
                       DM.ARAY[riULD].GetCntStat(csRslt10) , //"V Orient"
                       DM.ARAY[riULD].GetCntStat(csRslt11) , //"V Pmeasure"
                       DM.ARAY[riULD].GetCntStat(csRslt12) , //"V Pdist"
                       DM.ARAY[riULD].GetCntStat(csRslt13) , //"V Match"
                       DM.ARAY[riULD].GetCntStat(csRslt14) ,
                       DM.ARAY[riULD].GetCntStat(csFail  ) ); //"Heat Fail");//"V Flow"
                  //!DM.ARAY[riWK2].CheckAllStat(csNone) &&

/*
    if(GetLastStrip()) {
        m_bLotEnded = true ;
        memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

        Trace("LotEnd",DM.ARAY[riULD].GetLotNo().c_str());
        LT.LotInfo.dEndTime = Now();
        LT.WriteLotLog   ();
        LT.WriteLotDayLog();
        Trace("WriteLotLog",DM.ARAY[riULD].GetLotNo().c_str());
        //LT.LotEnd();
    }
    //TODO :: �̰� ���� �̻��ؼ� �ϴ� �ű�µ� ���� ������ �������ּ���.
    //DM.ARAY[riULD].SetStat(csNone) ;
*/
}
