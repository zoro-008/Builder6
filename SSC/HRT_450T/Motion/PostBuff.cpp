//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "PostBuff.h"
#include "Loader.h"
#include "SortingTool.h"
#include "UnLoader.h"
#include "WorkZone.h"
//---------------------------------------------------------------------------
#include "Rs232Man.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "ProbeComUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CPostBuff PSB;
//---------------------------------------------------------------------------
CPostBuff::CPostBuff(void)
{
    m_sPartName = "PostBuff " ;
    Reset();
    Load(true);
    m_iCrntCol = -1;

    InitCycleName();
}
//---------------------------------------------------------------------------
void CPostBuff::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}
//---------------------------------------------------------------------------
void CPostBuff::ResetTimer()
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
CPostBuff::~CPostBuff (void)
{
    Load(false);
}
//---------------------------------------------------------------------------
void CPostBuff::Close()
{
    Load(false);
}

//---------------------------------------------------------------------------
bool CPostBuff::FindChip(int &r , int &c , EN_CHIP_STAT Stat)
{
    c = DM.ARAY[riPSB].FindLastCol(Stat) ;
    return (c > -1 );
}
bool CPostBuff::FindChipWRK(int &r , int &c)
{
    c = DM.ARAY[riWRK].FindLastCol(csUnkwn) ;
    return (c > -1 && c > -1);
}
//---------------------------------------------------------------------------
bool CPostBuff::GetReadyPos(EN_CHIP_STAT Mode)
{
//    double dv1,dv2,dv3;
//    AnsiString sv1, sv2;
////    dv1 = MT_GetCmdPos(miPSB_XCmp);
//    dv1 = MT_GetCmdPos(miPSB_XCmp);
//    dv2 = GetMotrPos(miPSB_XCmp , piPSB_XWork , Mode);
//    dv3 = MT_GetEncPos(miPSB_XCmp);
//
//    bool bRet = MT_CmprPos(miPSB_XCmp , dv2);
//
//    sv1.sprintf("%0.3f",dv1);
//    sv2.sprintf("%0.3f",dv2);
//
//    return bRet ;
//    �̻��ϴ� dXCmdPos == dFailPos �϶� ���� �ʴٰ� ���ϵ�.

    //���� ������ �ǽɵǾ� ���������� ���� Ǯ�� ����. ��Ÿ� ���� ���ϸ��.
    double dXCmdPos    = MT_GetCmdPos(miPSB_XCmp);                            //���� ������
    double dXEncPos    = MT_GetEncPos(miPSB_XCmp);                            //���ڴ� ������
    double dXInpos     = 0.1 ;                                                //�˳��ϰ� �޴���� ������ ��������.
    double dFailPos    = GetMotrPos(miPSB_XCmp , piPSB_XWork , Mode);         //���������� �ִ� ������
    bool   bStoped     = MT_GetStop(miPSB_XCmp);                              //������������.
    bool   bXInposStop = bStoped && (abs(dXCmdPos - dXEncPos) < dXInpos) ;    //�޴���� ������ �������� ������ �������� ��ž Ȯ��.

    //bool   bNeedMove   = bXInposStop && dXCmdPos != dFailPos ;                //�������ǽ�ž�̰� ���������ߴ� �����ǰ� �������� �ִ� �������� �ٸ� ��� �̵��ʿ�.
    bool   bReadyPos   = bXInposStop && abs(dXCmdPos - dFailPos)< dXInpos ;     //�̻��ϴ� dXCmdPos == dFailPos �϶� ���� �ʴٰ� ���ϵ�.

    return bReadyPos ;
    ////////////////////////////////////////////////////////////////////////
}

//---------------------------------------------------------------------------
double CPostBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , EN_CHIP_STAT Mode)
{
    int  r ,c , iColGrCnt , iMaxCol, HalfCol ;
    int fc, hc;
    int    iGrCnt ;
    bool   bGrGrip;
    iMaxCol = DM.ARAY[riPSB].GetMaxCol();

         if(Mode == csWork)  FindChip(r , c , csWork);
    else if(Mode == csMask)  FindChip(r , c , csMask);
    else                     FindChip(r , c , csFail);


    fc = (iMaxCol - c) -1 ; //Ⱦ���� �ø��� ��.
    hc =  iMaxCol / 2;
    double dTrimColptch = OM.DevInfo.dColPitch + OM.DevOptn.dTrimPtchOfs ;

    double dX1Pos     = PM.GetValue(miPSB_XCmp , pvPSB_XWorkStart) + (dTrimColptch * fc)                                                  ;
    double dX2Pos     = PM.GetValue(miPSB_XCmp , pvPSB_XWorkStart) + (dTrimColptch * fc)                       - OM.DevOptn.dPSBReGripOfs ; //���׸��ϸ� 1��ġ�� ������ +1
    double dReGribPos = PM.GetValue(miPSB_XCmp , pvPSB_XWorkStart) + (dTrimColptch * OM.DevOptn.iPSBReGripCol) ;//- OM.DevOptn.dPSBReGripOfs ;
//    double dReGribPos = 0.0 ;
    double dPos       = 0.0 ;
    double dReGripOfs = hc * OM.DevInfo.dColPitch ;


    //���� ������ �ݴ�� ������ �ø��Ȱ����� ����Ѵ�.
    if( OM.DevInfo.iColGrCnt > 1 ) { //�׷�� ����Ͽ� ���ϱ�.
        //iColGrCnt��  fc  �� �׷찹��. OM.DevInfo.iColGrCnt�̰Ŵ� �ѱ׷�� LED ����.

        dX1Pos     += (OM.DevInfo.dColGrGap - dTrimColptch) * fc / OM.DevInfo.iColGrCnt ;
        dX2Pos     += (OM.DevInfo.dColGrGap - dTrimColptch) * fc / OM.DevInfo.iColGrCnt ;
        dReGribPos += (OM.DevInfo.dColGrGap - dTrimColptch) * OM.DevOptn.iPSBReGripCol / OM.DevInfo.iColGrCnt ;
    }

    if(OM.DevInfo.iColGrCnt > 0){
        iGrCnt      =  OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt ;
        bGrGrip     =  fc % iGrCnt ;
    }
    else  bGrGrip = false;

    double dCmpMovePos   = PM.GetValue(miPSB_XCmp , pvPSB_XWRKGrip) +
                           OM.DevInfo.dColPitch * OM.DevOptn.iContColCnt + OM.DevOptn.dPSBPtchOfs ;

    double dCmpGrGap     = OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ;

    double dGripOfs      = PM.GetValue(miPSB_XCmp , pvPSB_XGrip) - OM.DevOptn.dGripOfs;

    if(OM.MstOptn.bUsedXAxis){
        if(_iMotr == miWRK_XBtm){
            switch(_iPstnId){
                default                : dPos = MT_GetCmdPos(_iMotr                         ); break ;
                case piWRK_XBWait      : dPos = PM.GetValue (_iMotr , pvWRK_XBWait          ); break ;
                case piWRK_XBWork      : dPos = PM.GetValue (_iMotr , pvWRK_XBWork          ); break ;
            }
        }


        else if(_iMotr == miPSB_XCmp){
            switch(_iPstnId) {   //Pull�� ù��° �׸����� Drag �Ҷ� ������ ���� ������ Drag�� �ι�° �׸� �ϰ� ���� ���� ������.
                default               : dPos = MT_GetCmdPos(_iMotr                   ); break ;
                case piPSB_XWait      : dPos = PM.GetValue (_iMotr , pvPSB_XWait     ); break ;
                case piPSB_XGrip      : dPos = PM.GetValue (_iMotr , pvPSB_XGrip     ); break ;
                case piPSB_XPull      : dPos = PM.GetValue (_iMotr , pvPSB_XPull     ); break ; //CycleIn���� ó�� ������ ���� ���� ��ġ.
                case piPSB_XBack      : dPos = PM.GetValue (_iMotr , pvPSB_XBack     ); break ;
                case piPSB_XWorkStart : dPos = PM.GetValue (_iMotr , pvPSB_XWorkStart); break ;
                case piPSB_XOut       : dPos = PM.GetValue (_iMotr , pvPSB_XOut      ); break ;
                case piPSB_XWRKGrip   : dPos = PM.GetValue (_iMotr , pvPSB_XWRKGrip  ); break ;//WorkZone���� ����ϴ� ������.
                case piPSB_XReGrip    : dPos = dReGribPos ;                             break ;
                case piPSB_XWork      :      if (m_iReGripCnt == 0) dPos = dX1Pos       ;
                                        else if (m_iReGripCnt == 1) dPos = dX2Pos       ; break ;
                case piPSB_XDrag      : dPos = PM.GetValue (_iMotr , pvPSB_XPull ) - OM.DevOptn.dPSBReGripOfs; break ;
                case piPSB_XReBack    :      if (m_iReGripCnt == 0) dPos = PM.GetValue (_iMotr , pvPSB_XBack );
                                        else if (m_iReGripCnt == 1) dPos = PM.GetValue (_iMotr , pvPSB_XBack ) + OM.DevOptn.dPSBReGripOfs; break ;

                case piPSB_XWRKWork   : if (bGrGrip) dPos = dCmpMovePos + dCmpGrGap   ;
                                        else         dPos = dCmpMovePos               ; break ; //WorkZone���� ����ϴ� ������.
                case piPSB_XGripOfs   : dPos = dGripOfs                               ; break ;
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
        if(_iMotr == miPSB_XCmp){
            switch(_iPstnId) {   //Pull�� ù��° �׸����� Drag �Ҷ� ������ ���� ������ Drag�� �ι�° �׸� �ϰ� ���� ���� ������.
                default               : dPos = MT_GetCmdPos(_iMotr                   ); break ;
                case piPSB_XWait      : dPos = PM.GetValue (_iMotr , pvPSB_XWait     ); break ;
                case piPSB_XGrip      : dPos = PM.GetValue (_iMotr , pvPSB_XGrip     ); break ;
                case piPSB_XPull      : dPos = PM.GetValue (_iMotr , pvPSB_XPull     ); break ; //CycleIn���� ó�� ������ ���� ���� ��ġ.
                case piPSB_XBack      : dPos = PM.GetValue (_iMotr , pvPSB_XBack     ); break ;
                case piPSB_XWorkStart : dPos = PM.GetValue (_iMotr , pvPSB_XWorkStart); break ;
                case piPSB_XOut       : dPos = PM.GetValue (_iMotr , pvPSB_XOut      ); break ;
                case piPSB_XWRKGrip   : dPos = PM.GetValue (_iMotr , pvPSB_XWRKGrip  ); break ;//WorkZone���� ����ϴ� ������.
                case piPSB_XReGrip    : dPos = dReGribPos ;                             break ;
                case piPSB_XWork      :      if (m_iReGripCnt == 0) dPos = dX1Pos       ;
                                        else if (m_iReGripCnt == 1) dPos = dX2Pos       ; break ;
                case piPSB_XDrag      : dPos = PM.GetValue (_iMotr , pvPSB_XPull ) - OM.DevOptn.dPSBReGripOfs; break ;
                case piPSB_XReBack    :      if (m_iReGripCnt == 0) dPos = PM.GetValue (_iMotr , pvPSB_XBack );
                                        else if (m_iReGripCnt == 1) dPos = PM.GetValue (_iMotr , pvPSB_XBack ) + OM.DevOptn.dPSBReGripOfs; break ;

                case piPSB_XWRKWork   : if (bGrGrip) dPos = dCmpMovePos + dCmpGrGap   ;
                                        else         dPos = dCmpMovePos               ; break ; //WorkZone���� ����ϴ� ������.
                case piPSB_XGripOfs   : dPos = dGripOfs                               ; break ;
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
bool CPostBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isDrngPunch  = (AT_Complete(aiSTL_Top , ccFwd) || AT_Complete(aiSTL_Btm , ccFwd))  ;

    bool isXCmpMove   = !MT_GetStopInpos(miPSB_XCmp);
    bool isZbtmChange =  MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBChange)) && MT_GetStop(miWRK_ZBtm);
    bool isZbtmWork   =  MT_CmprPos(miWRK_ZBtm , PM.GetValue(miWRK_ZBtm,pvWRK_ZBWork  )) && MT_GetStop(miWRK_ZBtm);
    bool isYBtmFwd    = !AT_Complete(aiSTL_Btm , ccBwd);
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
        else if(_iMotr == miPSB_XCmp){
            if(isDrngPunch) {sMsg = "During Punching"; bRet = false ;}
            if(isYBtmFwd  ) {sMsg = AT_GetName(aiSTL_Btm )+" is Fwd"; bRet = false ; }
        }

        else if(_iMotr == miWRK_ZTop){
            switch(_iPstnId) {
                default                   :  break ;
                case piWRK_ZTWait         :  break ;
                case piWRK_ZTWork         :  if(isXCmpMove) {sMsg = MT_GetName(miWRK_XCmp)+" is Moveing"   ; bRet = false ; } break ;
            }
        }
        else if(_iMotr == miWRK_ZBtm){
            switch(_iPstnId) {
                default                   :  break ;
                case piWRK_ZBWait         :  break ;
                case piWRK_ZBChange       :  break ;
                case piWRK_ZBWork         :  if(isXCmpMove   ) {sMsg = MT_GetName(miWRK_XCmp)+" is Moveing"            ; bRet = false ; }break ;
            }
        }
        else if(_iMotr == miWRK_YBtm){
            if(!isZbtmChange && !isZbtmWork) {sMsg = MT_GetName(miWRK_YBtm)+" is Not Wait Position"  ; bRet = false ;}
            //���̵� �� ���� Z�� Work ��ġ���� �̵� ���� �ּ�...JS
            switch(_iPstnId) {
                default                   :  break ;
                case piWRK_YBWait         :  break ;
                case piWRK_YBWork         :  break ;
                case piWRK_YBChange       :  break ;
            }
        }
    }
    else if(!OM.MstOptn.bUsedXAxis){
        if(_iMotr == miPSB_XCmp){
            if(isDrngPunch) {sMsg = "During Punching"; bRet = false ;}
            if(isYBtmFwd  ) {sMsg = AT_GetName(aiSTL_Btm )+" is Fwd"; bRet = false ; }
        }
        else if(_iMotr == miWRK_ZTop){
            switch(_iPstnId) {
                default                   :  break ;
                case piWRK_ZTWait         :  break ;
                case piWRK_ZTWork         :  if(isXCmpMove) {sMsg = MT_GetName(miWRK_XCmp)+" is Moveing"   ; bRet = false ; } break ;
            }
        }
        else if(_iMotr == miWRK_ZBtm){
            switch(_iPstnId) {
                default                   :  break ;
                case piWRK_ZBWait         :  break ;
                case piWRK_ZBChange       :  break ;
                case piWRK_ZBWork         :  if(isXCmpMove   ) {sMsg = MT_GetName(miWRK_XCmp)+" is Moveing"            ; bRet = false ; }break ;
            }
        }
        else if(_iMotr == miWRK_YBtm){
            if(!isZbtmChange && !isZbtmWork) {sMsg = MT_GetName(miWRK_YBtm)+" is Not Wait Position"  ; bRet = false ;}
            //���̵� �� ���� Z�� Work ��ġ���� �̵� ���� �ּ�...JS
            switch(_iPstnId) {
                default                   :  break ;
                case piWRK_YBWait         :  break ;
                case piWRK_YBWork         :  break ;
                case piWRK_YBChange       :  break ;
            }
        }
    }

    else { bRet = false; }

    if(!bRet){
        Trace(AT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}
//---------------------------------------------------------------------------
bool  CPostBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isXMoving = MT_GetStopInpos(miPSB_XCmp);

    if(_iActr == aiPSB_Cmp) {
        if(!_bFwd) {
            if(!isXMoving) { sMsg = MT_GetName(miPSB_XCmp)+" is Moving" ; bRet = false ; }
        }
    }


    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}
//---------------------------------------------------------------------------
bool CPostBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;


    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}
//---------------------------------------------------------------------------
bool CPostBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}
//---------------------------------------------------------------------------
bool CPostBuff::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiPSB_HomeTO);
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

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 Step.iHome = 0 ;
                 return true ;

        case 10: IO_SetY(yPSB_FeedingAC,false);
                 AT_MoveCyl(aiPSB_Cmp , ccBwd);
                 AT_MoveCyl(aiPSB_Psh , ccBwd);
                 AT_MoveCyl(aiPSB_Rolr, ccFwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_Cmp , ccBwd))return false ;
                 if(!AT_MoveCyl(aiPSB_Psh , ccBwd))return false ;
                 if(!AT_MoveCyl(aiPSB_Rolr, ccFwd))return false ;
                 MT_DoHome(miPSB_XCmp);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPSB_XCmp))return false;
                 MT_GoAbsMan(miPSB_XCmp, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miPSB_XCmp, m_dLastIndPos))return false;
                 Step.iHome = 0;
                 return true ;
    }
}
//---------------------------------------------------------------------------
bool CPostBuff::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xPSB_Overload) ) EM_SetErr(eiPSB_Overload);

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        //sun kye won.
        //Cycle���ۿ� ���� �Ǿ� ������ SetSubStep���� ���.
        //Cycle���۸��� ����� ���� ������ SetStep���� �����. JS 2013. 9. 11.
        if (Stat.bReqStop)return false ;

        int iMaxCol, fc, iReGribCol;
        int r1,c1,r2,c2,r3,c3;

        iMaxCol = DM.ARAY[riWRK].GetMaxCol();
        iReGribCol = OM.DevOptn.iPSBReGripCol ; //30�̶�� ������ 30���� �۾� �ϰ� 31���� ���׸�.   iMaxCol / 2 ; //��Ʈ�� �߰����� �۾� �ϰ� ���׸��Ѵ�.

        //���� ������ �ǽɵǾ� ���������� ���� Ǯ�� ����. ��Ÿ� ���� ���ϸ��.
        double dXCmdPos    = MT_GetCmdPos(miPSB_XCmp);                            //���� ������
        double dXEncPos    = MT_GetEncPos(miPSB_XCmp);                            //���ڴ� ������
        double dXInpos     = 0.1 ;                                                //�˳��ϰ� �޴���� ������ ��������.
        double dFailPos    = GetMotrPos(miPSB_XCmp , piPSB_XWork , csFail);       //���������� �ִ� ������
        bool   bStoped     = MT_GetStop(miPSB_XCmp);                              //������������.
        bool   bXInposStop = bStoped && (abs(dXCmdPos - dXEncPos) < dXInpos) ;    //�޴���� ������ �������� ������ �������� ��ž Ȯ��.

        //bool   bNeedMove   = bXInposStop && dXCmdPos != dFailPos ;                //�������ǽ�ž�̰� ���������ߴ� �����ǰ� �������� �ִ� �������� �ٸ� ��� �̵��ʿ�.
        bool   bNeedMove   = bXInposStop && abs(dXCmdPos - dFailPos)> dXInpos ;                //�̻��ϴ� dXCmdPos == dFailPos �϶� ���� �ʴٰ� ���ϵ�.

        /////////////////f///////////////////////////////////////////////////////

        bool bExistFail     = FindChip(r1 , c1 , csFail);
        bool bExistWork     = DM.ARAY[riPSB].GetCntStat(csWork ); //FindChip(r2 , c2 , csWork);
        bool bExistMask     = DM.ARAY[riPSB].GetCntStat(csMask ); //FindChip(r3 , c3 , csMask);
        bool bFinished      =(!bExistFail && !bExistWork && !bExistMask) || OM.CmnOptn.bIgnrTrim ; //���� ��Ʈ���� �۾��� �������� Ȯ����.

        bool isExistWork    = !DM.ARAY[riPSB].GetCntStat(csWork ) && !DM.ARAY[riPSB].GetCntStat(csMask ) && !DM.ARAY[riPSB].GetCntStat(csFail ) ;

        fc = (iMaxCol - c1) -1  ; //���� �����͸� ������ ��.

        bool isExstTrim1    =  bExistFail && fc <  iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //��Ʈ�� �������� Ʈ�� �� ���� �ִ��� �ϴ� �ɼ�.
        bool isExstTrim2    =  bExistFail && fc >= iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //��Ʈ�� �Ĺ����� Ʈ�� �� ���� �ִ��� �ϴ� �ɼ�.


        //Cycle
        bool isCycleContact  = DM.ARAY[riWRK].GetCntStat  (csUnkwn) &&  DM.ARAY[riWRK].GetStep() >= OM.DevOptn.iWRKMoveCnt && DM.ARAY[riPSB].CheckAllStat(csNone);

        bool isCycleIn       =!DM.ARAY[riPSB].GetCntStat(csNone) && DM.ARAY[riPSB].GetSubStep() == 0; //ó���� ������ �μ� �ް� ��ŷ������ ���� ���� ���� �����͸� �ϳ� ����� �Ⱦ ������ ����.
        bool isCycleDrag     = bFinished && !DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riPSB].GetStep() == 0 && STL.GetSeqStep() == CSortingTool::scIdle  ;//�۾��� �� �������� �巹��.
        bool isCycleReGrip   =!isExstTrim1 && isExstTrim2 &&      // 1���� fail ����. 2���� fail�ְ�
                              !bExistWork  && !bExistMask &&      // Work�� Mask�� ������ �ȵ�.
                               m_iReGripCnt == 0 &&               // ���׸��� ���߾�߸� �Ѵ�.
                               ULD.GetSeqStep() == CUnLoader    :: scIdle && DM.ARAY[riULD].GetCntStat(csEmpty) && // ��δ� �غ� �Ǿ� �־�߸� �Ѵ�. �׷��� �����ҵ�. ������ ����...JS
                               STL.GetSeqStep() != CSortingTool :: scDevChk &&
                               STL.GetSeqStep() != CSortingTool :: scPunch  &&
                               STL.GetSeqStep() != CSortingTool :: scTrmChk &&
                               STL.GetSeqStep() == CSortingTool :: scIdle ;                                        // �������� ���� ����� �ϰ� �̾� �ϴ� �̷��� ����.;  ������ ����...JS
        bool isCycleWork     =((isExstTrim1 && m_iReGripCnt == 0) || (isExstTrim2 && m_iReGripCnt == 1)) && //regripcnt�� �־�߸� �ް��� ü�����ÿ� �Ĺݺο���  ���� ���� ������ ���� ����.
                               !bExistWork && !bExistMask &&                      //��Ʈ������ Work�� Mask�� ������ �������� �ʰ� �ٲ�.
                                STL.GetSeqStep() != CSortingTool :: scDevChk &&
                                STL.GetSeqStep() != CSortingTool :: scPunch  &&    //�������� Pick�ൿ�� �ϰ� ������ ��������. ��� �Ǵµ� ������.
                                STL.GetSeqStep() != CSortingTool :: scTrmChk &&   //������ ���� Ȯ�ν� �������ܼ� Ȯ���ϰ� �ִ� ���.
                                bExistFail && bNeedMove ;                         //���ϵ��� �ִµ� , ���� ��ġ�� �ٸ����.

        bool isCycleOut      = !DM.ARAY[riPSB].GetCntStat(csNone )   &&
                               (isExistWork || OM.CmnOptn.bIgnrTrim) &&
                                DM.ARAY[riULD].GetCntStat(csEmpty)   &&
                                STL.GetSeqStep() == CSortingTool::scIdle &&
                                DM.ARAY[riPSB].GetStep() == 1            &&
                                ULD.GetSeqStep() == CUnLoader :: scIdle ;

        bool isCycleEnd      =  DM.ARAY[riPSB].GetCntStat(csNone) ;




        //�𸣴� ��Ʈ������.
        if((isCycleIn || isCycleDrag || isCycleReGrip || isCycleWork) && DM.ARAY[riPSB].CheckAllStat(csNone) && !OM.CmnOptn.bDryRun) {
            if(IO_GetX(xWRK_Detect2) || IO_GetX(xPSB_Pkg1) || IO_GetX(xPSB_Pkg2) || IO_GetX(xPSB_PkgOut1) || IO_GetX(xPSB_PkgOut2) ) EM_SetErr(eiPSB_Unknown) ;
        }

        //strip �����.
        if( DM.ARAY[riPSB].GetCntExist() && !OM.CmnOptn.bDryRun) {
            if(!IO_GetX(xWRK_Detect2) && !IO_GetX(xPSB_Pkg1) && !IO_GetX(xPSB_Pkg2) && !IO_GetX(xPSB_PkgOut1) )EM_SetErr(eiPSB_Dispr) ;
        }
        //if( isCycleOut && DM.ARAY[riPS2].GetCntExist() && !OM.CmnOptn.bDryRun) {
        //    if(!IO_GetX(xPSB_3Pkg) && !IO_GetX(xPSB_4Pkg) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2))EM_SetErr(eiPSB_Dispr) ;
        //}

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleIn      ) {Trace(m_sPartName.c_str(),"CycleIn       Stt" ); Step.iSeq = scIn       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleDrag    ) {Trace(m_sPartName.c_str(),"CycleDrag     Stt" ); Step.iSeq = scDrag     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleReGrip  ) {Trace(m_sPartName.c_str(),"CycleReGrip   Stt" ); Step.iSeq = scReGrip   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork    ) {Trace(m_sPartName.c_str(),"CycleWork     Stt" ); Step.iSeq = scWork     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut     ) {Trace(m_sPartName.c_str(),"CycleOut      Stt" ); Step.iSeq = scOut      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleContact ) {Trace(m_sPartName.c_str(),"CycleContact  Stt" ); Step.iSeq = scContact  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default         :                     Trace(m_sPartName.c_str(),"default        End");Step.iSeq = scIdle ;  return false ;
        case scIdle     :                                                                                           return false ;
        case scIn       : if(CycleIn      ()){ Trace(m_sPartName.c_str(),"CycleIn       End");Step.iSeq = scIdle ;} return false ;
        case scDrag     : if(CycleDrag    ()){ Trace(m_sPartName.c_str(),"CycleDrag     End");Step.iSeq = scIdle ;} return false ;
        case scReGrip   : if(CycleReGrip  ()){ Trace(m_sPartName.c_str(),"CycleReGrip   End");Step.iSeq = scIdle ;} return false ;
        case scWork     : if(CycleWork    ()){ Trace(m_sPartName.c_str(),"CycleWork     End");Step.iSeq = scIdle ;} return false ;
        case scOut      : if(CycleOut     ()){ Trace(m_sPartName.c_str(),"CycleOut      End");Step.iSeq = scIdle ;} return false ;
        case scContact  : if(CycleContact ()){ Trace(m_sPartName.c_str(),"CycleContact  End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}


//---------------------------------------------------------------------------
bool CPostBuff::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;
}
//---------------------------------------------------------------------------
bool CPostBuff::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}
//---------------------------------------------------------------------------
bool CPostBuff::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr(eiPSB_ToStartTO);

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

        case 10: IO_SetY(yPSB_FeedingAC,false);
                 if(!DM.ARAY[riPSB].CheckAllStat(csNone)){
                     Step.iToStart = 0;
                     return true;
                 }
                 MoveActr(aiPSB_Cmp  , ccBwd);
                 MoveActr(aiPSB_Rolr , ccFwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_Cmp  , ccBwd)) return false ;
                 if(!MoveActr(aiPSB_Rolr , ccFwd)) return false ;
                 MoveMotr(miPSB_XCmp , piPSB_XWait);

                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveMotr(miPSB_XCmp , piPSB_XWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}
//---------------------------------------------------------------------------
bool CPostBuff::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErr(eiPSB_ToStopTO);

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

        case 10: IO_SetY(yPSB_FeedingAC, false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: IO_SetY(yPSB_FeedingAC, false);
                 if(IO_GetY(yPSB_FeedingAC)) return false;
                 if(!MT_GetAlarm   (miPSB_XCmp) &&  //���������� ������ ������ ��� �ϱ����ؼ� �ε� ������ ġ�� �ְų� �˶����̸� ���V�ϵ��� .
                    !MT_GetNLimSnsr(miPSB_XCmp) &&
                    !MT_GetPLimSnsr(miPSB_XCmp) ) m_dLastIndPos = MT_GetCmdPos(miPSB_XCmp) ;
                 else                             m_dLastIndPos = GetMotrPos  (miPSB_XCmp , piPSB_XWait) ;

                 Step.iToStop = 0   ;
                 return true ;
    }
}
//---------------------------------------------------------------------------
//One Cycle.
bool CPostBuff::CycleContact()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    int iLedTime = OM.DevOptn.i1ChLedTime > OM.DevOptn.i2ChLedTime ? OM.DevOptn.i1ChLedTime * 100 : OM.DevOptn.i2ChLedTime * 100;
    int iZrTime  = OM.DevOptn.i1ChZenTime > OM.DevOptn.i2ChZenTime ? OM.DevOptn.i1ChZenTime * 100 : OM.DevOptn.i2ChZenTime * 100;

    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, iLedTime + iZrTime + 1000 )) {
        EM_SetErr(eiPSB_CycleTO);
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

    int r,c ;
    bool r1,r2 ;
    int iMaxRow,iMaxCol,icsWork,iFail;
    iMaxRow = DM.ARAY[riWRK].GetMaxRow();
    iMaxCol = DM.ARAY[riWRK].GetMaxCol();
    CWorkZone::SStat Stat = WRK.GetStat();

    static int  iReInspCnt = 0 ;
    static int  iCmmErrCnt = 0 ;
    AnsiString sPinCount ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;
        case  10: if(OM.MstOptn.bUsedXAxis) {
                      MoveMotr(miWRK_XBtm , piWRK_XBWait);
                      MoveActr(aiWRK_Hold , ccBwd);
                  }
                  Step.iCycle++;
                  return false;

        case  11: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait)) return false;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccBwd)) return false;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait);
                  MoveActr(aiPSB_Cmp  , ccBwd);
                  if(DM.ARAY[riWRK].CheckAllStat(csUnkwn))PCM.ClearData();
                  iReInspCnt = 0 ;
                  iCmmErrCnt = 0 ;
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait)) return false ;
                  if(!MoveActr(aiPSB_Cmp , ccBwd    )) return false ;
                  //MoveMotr(miPSB_XCmp , piPSB_XGripOfs);
                  MoveMotr(miPSB_XCmp , piPSB_XWRKGrip);
                  Step.iCycle++;
                  return false;

        case  14: //if(!MoveMotr(miPSB_XCmp , piPSB_XGripOfs))  return false ;
                  if(!MoveMotr(miPSB_XCmp , piPSB_XWRKGrip))  return false ;
                  MoveActr(aiPSB_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiPSB_Cmp , ccFwd)) return false;
                  MoveMotr(miPSB_XCmp , piPSB_XWRKWork);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miPSB_XCmp , piPSB_XWRKWork)) return false;
                  //Ver 1.0.2.3
                  if(DM.ARAY[riWRK].GetCheck())
                  {
                      FindChipWRK(r,c);
                      if(OM.CmnOptn.bIgnrInsp || DM.ARAY[riWRK].GetCheck()) {
                          for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                              for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRK].SetStat(i , c - j, csGood); //����Ÿ ����ŷ.
                          }
                      }
                      Step.iCycle = 27;
                      return false;
                  }

                  MoveMotr(miWRK_ZTop , piWRK_ZTWork);
                  if(OM.MstOptn.bUsedXAxis)MoveActr(aiWRK_Hold , ccFwd);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWork)) return false ;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccFwd)) return false;
                  if(!OM.MstOptn.bUseReContact) MoveActr(aiPSB_Cmp  , ccBwd );
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWork);
                  Step.iCycle++;
                  return false;

        case  18: if(!OM.MstOptn.bUseReContact && !MoveActr(aiPSB_Cmp  , ccBwd)) return false;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWork)) return false ;
                  MoveMotr(miWRK_YBtm, piWRK_YBWork) ;
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveMotr(miWRK_YBtm, piWRK_YBWork)) return false ;
                  if(!OM.MstOptn.bUseReContact) MoveMotr(miPSB_XCmp , piPSB_XWRKGrip);
                  if(OM.MstOptn.bUsedXAxis) MoveMotr(miWRK_XBtm , piWRK_XBWork);
                  Step.iCycle++;
                  return false ;

        case  20: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWork)) return false;
                  if(!OM.MstOptn.bUseReContact && !MoveMotr(miPSB_XCmp , piPSB_XWRKGrip)) return false;
                  //if(!OM.MstOptn.bUseReContact && !MoveMotr(miPSB_XCmp , piPSB_XGripOfs)) return false;
                  Step.iCycle++;
                  return false;

        //���� �ؿ��� ���.
        case  21: OM.DevOptn.iContactPinCnt++;
                  if(OM.DevOptn.iLimContactPinCnt < OM.DevOptn.iContactPinCnt) {
                      EM_SetErr(eiWRK_LimPinCnt);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++ ;
                  return false ;

        case  22: if(!m_tmTemp.OnDelay(true ,OM.DevOptn.iPrbSttDelay)) return false ;
                  if(OM.CmnOptn.bIgnrInsp || DM.ARAY[riWRK].GetCheck()) {
                      WRK.bPb1Rslt = true ;
                      WRK.bPb2Rslt = true ;
                  }
                  else {
                      WRK.bPb1Rslt = false ;
                      WRK.bPb2Rslt = false ;
                      if(OM.MstOptn.bByChInsp) PCM.Start(1);
                      else                     PCM.Start( );
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  23: if(m_tmTemp.OnDelay(true , OM.DevOptn.i1ChLedTime * 100 + 1000)) {
                      if(!WRK.bPb1Rslt) {
                          if( OM.MstOptn.bUsedXAxis){
                              MoveMotr(miWRK_XBtm, piWRK_XBWait) ;
                              MoveActr(aiWRK_Hold , ccBwd);
                          }
                          if(!OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_YBtm, piWRK_YBWait) ;
                          MoveActr(aiWRK_Cmp , ccFwd);
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }
                  if(!WRK.bPb1Rslt)return false;
                  if(OM.MstOptn.bByChInsp) PCM.Start(2);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  24: if(m_tmTemp.OnDelay(true,OM.DevOptn.i2ChLedTime * 100 + 1000)) {
                      if(!WRK.bPb2Rslt) {
                          if( OM.MstOptn.bUsedXAxis){
                              MoveMotr(miWRK_XBtm, piWRK_XBWait) ;
                              MoveActr(aiWRK_Hold , ccBwd);
                          }
                          if(!OM.MstOptn.bUsedXAxis)MoveMotr(miWRK_YBtm, piWRK_YBWait) ;
                          MoveActr(aiPSB_Cmp , ccFwd);
                          Step.iCycle = 50 ;
                          return false ;
                      }
                  }
                  //if(!g_bPb2Rslt) return false ;
                  if(!WRK.bPb2Rslt)return false;
                  if(OM.MstOptn.bUsedXAxis) {
                      MoveMotr(miWRK_XBtm ,  piWRK_XBWait);
                      MoveActr(aiWRK_Hold , ccBwd);
                  }
                  Step.iCycle++;
                  return false ;

        case  25: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait)) return false;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccBwd       )) return false;
                  MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  Step.iCycle++;
                  return false ;

        case  26: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false;
                  //����ŷ ���� �� �� �˻� ����.
                  FindChipWRK(r,c);
                  if(OM.CmnOptn.bIgnrInsp || DM.ARAY[riWRK].GetCheck()) {
                      for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                          for(int i = 0 ; i < iMaxRow ; i++ ) DM.ARAY[riWRK].SetStat(i , c - j, csGood); //����Ÿ ����ŷ.
                      }
                  }
                  else {
                      r1 = PCM.CheckDataCnt(riWRK , WRK.sPb1Msg);
                      r2 = PCM.CheckDataCnt(riWRK , WRK.sPb2Msg);

                      if(!r1){ Step.iCycle = 30 ; m_iDataErrCnt++ ; MoveActr(aiPSB_Cmp , ccFwd); return false ;} //EM_SetErr(eiWRK_Probe1DataWrong) ; //������ ����ŷ.  ���߿� PCM�� ARAY �Ⱦ� ���� ���⼭ ������ �޾Ƽ� ����ŷ �Ѵ�.
                      if(!r2){ Step.iCycle = 30 ; m_iDataErrCnt++ ; MoveActr(aiPSB_Cmp , ccFwd); return false ;} //EM_SetErr(eiWRK_Probe2DataWrong) ;

                      if(r1 && r2) {
                          PCM.ClearDataCrnt();
                          PCM.GetDataCh(riWRK , WRK.sPb1Msg , PROBE_CH1 ,Stat.bRepeat);
                          PCM.GetDataCh(riWRK , WRK.sPb2Msg , PROBE_CH2 ,Stat.bRepeat);
                      }
                  }
                  iFail = 0;
                  for( int j =0 ; j < OM.DevOptn.iContColCnt ; j++ ) {
                      iFail += DM.ARAY[riWRK].GetCntColStat(c - j, csFail); //����Ÿ ����ŷ.
                  }

                  //��˻� ���� �� ����Ʈ �׽�Ʈ ����.
                  if (iFail >= OM.DevOptn.iLimInsFailCnt || Stat.bRepeat){
                      if(iReInspCnt < OM.DevOptn.iReInspCnt || Stat.bRepeat){
                          for(int j = 0 ; j < OM.DevOptn.iContColCnt ;j++) {
                              for(int i =0 ; i < iMaxRow ; i++) DM.ARAY[riWRK].SetStat(i ,c - j, csUnkwn) ;
                          }
                          MoveActr(aiPSB_Cmp , ccFwd);
                          Step.iCycle = 32 ;
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

                  //�ɼ� �߰��� ����. JS...
                  if(iFail >= OM.DevOptn.iFrtPrcFailCnt){
                      WRK.bOverFail = true;
                      WRK.bOverErr  = true;
                      WRK.iOverFail += iFail;
                  }

                  Step.iCycle++;
                  return false;

                  //Ver 1.0.2.3 ������ ��� �˻� �н� ó��
        case  27: if(!MoveMotr(miWRK_ZBtm,piWRK_ZBWait)) return false ;
                  if(!MoveMotr(miWRK_ZTop,piWRK_ZTWait)) return false ;
                  MoveActr(aiPSB_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  28: if(!MoveActr(aiPSB_Cmp , ccBwd)) return false;
                  MoveMotr(miPSB_XCmp , piPSB_XWRKGrip);
                  //MoveMotr(miPSB_XCmp , piPSB_XGripOfs);
                  Step.iCycle++;
                  return false;

        case  29: if(!MoveMotr(miPSB_XCmp , piPSB_XWRKGrip)) return false;
                  //if(!MoveMotr(miPSB_XCmp , piPSB_XGripOfs)) return false;
                  if (!DM.ARAY[riWRK].GetCntStat(csUnkwn) && OM.CmnOptn.bCheckFailCnt &&
                  (DM.ARAY[riWRK].GetCntStat(csFail) >= OM.CmnOptn.iCheckFailCnt ||
                   DM.ARAY[riPSB].GetCntStat(csFail)>=OM.CmnOptn.iCheckFailCnt   )){
                      EM_SetErr(eiWRK_FailCntOver);
                  }
                  DM.ARAY[riWRK].SetStep(DM.ARAY[riWRK].GetStep() + OM.DevOptn.iContColCnt);
                  Step.iCycle=0;
                  return true;


        //PCM.CheckDataCnt ������ üũ Fail�� ��� ����.
        case  30: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait)) return false;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccBwd       )) return false;
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

        //�ؿ��� ���. ��˻� �ɼ� �� �˻� ���н� ���.
        case  32: if(!MoveActr(aiPSB_Cmp , ccFwd)) return false;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait)) return false ;
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
                  Step.iCycle = 21;
                  return false;

        //�˻� ���н� ��� �Ǵ� ����.
        case  50: if(OM.MstOptn.bUsedXAxis && !MoveMotr(miWRK_XBtm , piWRK_XBWait)) return false;
                  if(OM.MstOptn.bUsedXAxis && !MoveActr(aiWRK_Hold , ccBwd       )) return false;
                  Step.iCycle++;
                  return false;

        case  51: if(!MoveMotr(miWRK_YBtm, piWRK_YBWait)) return false ;
                  if(!MoveActr(aiPSB_Cmp , ccFwd)) return false;
                  iCmmErrCnt++;
                  MoveMotr(miWRK_ZBtm,piWRK_ZBWait);
                  MoveMotr(miWRK_ZTop,piWRK_ZTWait);

                  if(iCmmErrCnt < 4) { //�˻��� ���� �� �ȳ��� �ͼ� ��Ʈ���� ����.
                      Step.iCycle = 32 ;
                      if(!WRK.bPb1Rslt                 ) Trace("Cmmunication","g_bPb1Rslt ComRetry");
                      if( WRK.bPb1Rslt && !WRK.bPb2Rslt) Trace("Cmmunication","g_bPb2Rslt ComRetry");
                      return false ;
                  }

                  if(iCmmErrCnt < 5) { //�� �ȵŸ� ���� �ٽ��غ�.

                      Step.iCycle = 32 ;
                      if(!WRK.bPb1Rslt                 ) Trace("Cmmunication","g_bPb1Rslt ComRetry && SetDaegyeom()");
                      if( WRK.bPb1Rslt && !WRK.bPb2Rslt) Trace("Cmmunication","g_bPb2Rslt ComRetry && SetDaegyeom()");

                      PCM.SetDaegyeom();
                      //if(OM.MstOptn.bUseDaegyeomProbe) PCM.SetDaegyeom();  //��� Eng ���κ� ����.
                      return false ;
                  }

                  //�׷��� �ȵǸ� ����.
                  if(!WRK.bPb1Rslt                 ) EM_SetErr(eiWRK_FailProbeComm1);
                  if( WRK.bPb1Rslt && !WRK.bPb2Rslt) EM_SetErr(eiWRK_FailProbeComm2);
                  MoveActr(aiPSB_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  52: if(!MoveActr(aiPSB_Cmp , ccBwd)) return false;
                  Step.iCycle = 0 ;
                  return false ;

                  if(!OM.MstOptn.bUseReContact && !MoveMotr(miPSB_XCmp , piPSB_XWRKGrip)) return false;
                  //if(!OM.MstOptn.bUseReContact && !MoveMotr(miPSB_XCmp , piPSB_XGripOfs)) return false;
                  Step.iCycle++;
                  return false;
    }
}


bool CPostBuff::CycleIn() //WorkZone ���� ���� ���� ���� ������  �ѹ� Ÿ�� �۾� ���� ��ġ�� ���� ����.
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    int r,c;
    int iMaxRow = DM.ARAY[riPSB].GetMaxRow();
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiPSB_CycleTO);
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

        case  10: MoveActr(aiPSB_Cmp, ccBwd);
                  if(OM.CmnOptn.bUsedInsOptn){
                      for(int r = 0; r < OM.DevInfo.iRowCnt ; r++){
                          for(int c = 0; c < OM.DevInfo.iColCnt; c++){
                              if(DM.ARAY[riOPT].GetStat(r, c) != csNone){
                                  DM.ARAY[riPSB].SetStat(r, c, DM.ARAY[riOPT].GetStat(r, c));
                              }
                          }
                      }
                  }
                  if(STL.m_sLastLotNo != DM.ARAY[riPSB].GetLotNo()){
                      STL.m_sLastLotNo = DM.ARAY[riPSB].GetLotNo();
                      STL.m_iTrimCnt = 0;
                      STL.m_iBin0Cnt = 0;
                      STL.m_iBin1Cnt = 0;
                      STL.m_iBin2Cnt = 0;
                      STL.m_iBin3Cnt = 0;
                      STL.m_iBin4Cnt = 0;
                      STL.m_iBin5Cnt = 0;
                      STL.m_iBin6Cnt = 0;
                      STL.m_iBin7Cnt = 0;
                      STL.m_iBin8Cnt = 0;
                      STL.m_iBin9Cnt = 0;

                  }
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Cmp, ccBwd)) return false;
                  //Lot�̸����� Ȯ��.JS
                  MoveMotr(miPSB_XCmp, piPSB_XGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XCmp, piPSB_XGrip)) return false ;
                  MoveActr(aiPSB_Cmp, ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  MoveMotr(miPSB_XCmp, piPSB_XWorkStart);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miPSB_XCmp, piPSB_XWorkStart)) return false ;
                  //DM.ShiftArrayData(riPS1,riPS2);

                  MoveActr(aiPSB_Cmp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPSB_Cmp , ccBwd)) return false ;
                  MoveActr(aiPSB_Cmp , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiPSB_Cmp , ccFwd)) return false ;
                  DM.ARAY[riPSB].SetSubStep(1);
                  DM.ARAY[riPSB].SetStep(0);

                  //SortBinOption ������ �ּ�. JS
                  //STL.RejectMove(); //������ �ϳ��� �ȳ��� �ް�����

                  FindChip(r , c , csFail);
                  m_iCrntCol = c;
                  m_iReGripCnt = 0;
                  Step.iCycle = 0;
                  return true ;
        }
}

//---------------------------------------------------------------------------
bool CPostBuff::CycleDrag() //PS1���� ���̻� ������ ���� ���� �ʰų� �߰��� NoTrimFail �ɼ��� ������. ���׸� ���� ��� ���� �˾Ƽ� ������ ����ؼ� ź��.
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiPSB_CycleTO);
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

        case  10: MoveActr(aiPSB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Cmp,ccFwd)) return false ;
                  //if(!m_iReGripCnt)MoveMotr(miPSB_XCmp , piPSB_XPull) ;
                  //else             MoveMotr(miPSB_XCmp , piPSB_XDrag) ;
                  //else             MoveMotr(miPSB_XCmp , piPSB_XPull) ;
                  MoveMotr(miPSB_XCmp , piPSB_XPull) ;
                  Step.iCycle++;
                  return false ;

        case  12: //if (!m_iReGripCnt && !MoveMotr(miPSB_XCmp , piPSB_XPull)) return false ;
                  //if ( m_iReGripCnt && !MoveMotr(miPSB_XCmp , piPSB_XPull)) return false ;
                  if(!MoveMotr(miPSB_XCmp , piPSB_XPull)) return false ;
                  MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPSB_Cmp,ccBwd     )) return false ;
                  MoveMotr(miPSB_XCmp,piPSB_XReBack ) ;
                  //DM.ShiftArrayData(riPS1,riPS2);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XCmp,piPSB_XReBack )) return false ;
                  //MoveMotr(miPSB_XCmp,piPSB_XOut) ;
                  MoveActr(aiPSB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPSB_Cmp,ccFwd)) return false ;
                  DM.ARAY[riPSB].SetStep(1);
                  Step.iCycle = 0 ;
                  return true ;
    }
}
//---------------------------------------------------------------------------

bool CPostBuff::CycleWork()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    int r,c;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiPSB_CycleTO);
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

        case  10: MoveActr(aiPSB_Cmp, ccFwd);
                  MoveActr(aiSTL_Btm, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  if(!MoveActr(aiSTL_Btm, ccBwd)) return false;
                  MoveMotr(miPSB_XCmp , piPSB_XWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XCmp , piPSB_XWork)) return false;
//                  FindChip(r , c , csFail);
//                  m_iCrntCol = c;
                  Step.iCycle = 0;
                  return true ;
        }
}
//---------------------------------------------------------------------------
bool CPostBuff::CycleReGrip()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiPSB_CycleTO);
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

        case  10: MoveActr(aiPSB_Cmp , ccFwd);
                  STL.MoveMotr(miSTL_YTop,piSTL_YTWait);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  if(!STL.MoveMotr(miSTL_YTop,piSTL_YTWait)) return false;
                  MoveMotr(miPSB_XCmp,piPSB_XReGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XCmp,piPSB_XReGrip)) return false;
                  MoveActr(aiPSB_Cmp, ccBwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_Cmp, ccBwd)) return false;
                  MT_GoIncRun(miPSB_XCmp , -OM.DevOptn.dPSBReGripOfs) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MT_GetStop(miPSB_XCmp)) return false ;
                  MoveActr(aiPSB_Cmp, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPSB_Cmp , ccFwd)) return false ;
                  m_iReGripCnt= 1 ;
                  Step.iCycle = 0;
                  return true ;
    }

}
//---------------------------------------------------------------------------
bool CPostBuff::CycleOut()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErr(eiPSB_CycleTO);
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

    static bool bLastStrip ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: STL.MoveActr(aiSTL_Top, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!STL.MoveActr(aiSTL_Top, ccBwd)) return false;
                  STL.MoveMotr(miSTL_YTop,piSTL_YTWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!STL.MoveMotr(miSTL_YTop, piSTL_YTWait)) return false;
                  LT.WriteTrimInfo(DM.ARAY[riPSB].GetLotNo(), STL.m_iTrimCnt, STL._iBinCnt);
                  if(OM.MstOptn.bUsedBinSorting) {
                      LT.WriteBinInfo (DM.ARAY[riPSB].GetLotNo(), STL.m_iBin0Cnt, STL.m_iBin1Cnt, STL.m_iBin2Cnt, STL.m_iBin3Cnt, STL.m_iBin4Cnt,
                      STL.m_iBin5Cnt, STL.m_iBin6Cnt, STL.m_iBin7Cnt, STL.m_iBin8Cnt, STL.m_iBin9Cnt);
                  }
                  IO_SetY(yPSB_FeedingAC,true);
                  MoveActr(aiPSB_Cmp, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  if(ULD.GetSeqStep() != CUnLoader :: scIdle) return false  ; //����¥������ 1����Ŭ ���� �� ��� �ü� �־ ���Ƴ���.
                  MoveMotr(miPSB_XCmp,piPSB_XOut);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XCmp,piPSB_XOut)) return false ;
                  MoveActr(aiPSB_Cmp  , ccBwd);
                  MoveActr(aiPSB_Rolr , ccBwd);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;


        case  15: if(!MoveActr(aiPSB_Cmp  , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_Rolr , ccBwd)) return false ;
                  if(m_tmTemp.OnDelay(true, 4000)){
                      EM_SetErr(eiPSB_Striphitch);
                      IO_SetY(yPSB_FeedingAC, false);
                      Step.iCycle = 0;
                      return true;
                  }
                  if(IO_GetX(xPSB_PkgOut1)) return false ;
                  m_tmTemp.Clear();
                  //m_tmTemp.OnDelay(true , OM.DevOptn.dPSBPshDeley);
                  Step.iCycle++;
                  return false;

        case  16: if(!m_tmTemp.OnDelay(true , OM.DevOptn.dPSBPshDeley)) return false;
                  MoveActr(aiPSB_Psh,ccFwd);
                  MoveMotr(miPSB_XCmp,piPSB_XWait);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiPSB_Psh,ccFwd)) return false ;
                  IO_SetY(yPSB_FeedingAC,false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  18: if(IO_GetX(xPSB_PkgOut2)){EM_SetErr(eiPSB_Out2Detected); return false; }
                  MoveActr(aiPSB_Psh  , ccBwd);
                  MoveActr(aiPSB_Rolr , ccFwd);
                  DM.ARAY[riPSB].SetStat(csNone) ;
                  if(DM.ARAY[riULD].CheckAllStat(csEmpty)) DM.ARAY[riULD].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);
                  Step.iCycle++;
                  return false;

        case  19: if(!MoveActr(aiPSB_Psh  , ccBwd      )) return false ;
                  if(!MoveActr(aiPSB_Rolr , ccFwd      )) return false ;
                  if(!MoveMotr(miPSB_XCmp , piPSB_XWait)) return false ;
                  DM.ARAY[riPSB].SetSubStep(0);
                  DM.ARAY[riPSB].SetStep(0);
                  Step.iCycle = 0;
                  return true;
    }
}
bool CPostBuff::CycleNextMove()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
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

    double dCmpMovePos = PM.GetValue(miPSB_XCmp , pvPSB_XWRKGrip) + OM.DevInfo.dColPitch * OM.DevOptn.iContColCnt + OM.DevOptn.dWorkPtchOfs;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miWRK_YBtm , piWRK_YBWait);
                  MoveActr(aiPSB_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRK_YBtm , piWRK_YBWait)) return false ;
                  if(!MoveActr(aiPSB_Cmp , ccBwd)) return false;
                  MoveMotr(miWRK_ZTop , piWRK_ZTWait  );
                  MoveMotr(miWRK_ZBtm , piWRK_ZBWait  );
                  MoveMotr(miPSB_XCmp , piPSB_XWRKGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRK_ZTop , piWRK_ZTWait  )) return false ;
                  if(!MoveMotr(miWRK_ZBtm , piWRK_ZBWait  )) return false ;
                  if(!MoveMotr(miPSB_XCmp , piPSB_XWRKGrip)) return false ;
                  MoveActr(aiPSB_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_Cmp , ccFwd)) return false ;
                  //MoveMotr(miPSB_XCmp , piPSB_XWRKWork);
                  MT_GoAbsMan(miPSB_XCmp , dCmpMovePos);
                  Step.iCycle++;
                  return false;

        case  14: //if(!MoveMotr(miPSB_XCmp , piPSB_XWRKWork)) return false ;
                  if(!MT_GoAbsMan(miPSB_XCmp , dCmpMovePos)) return false;
                  MoveActr(aiPSB_Cmp , ccBwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiPSB_Cmp , ccBwd)) return false ;
                  MoveMotr(miPSB_XCmp , piPSB_XWRKGrip);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miPSB_XCmp , piPSB_XWRKGrip)) return false;
                  Step.iCycle=0;
                  return true;
    }

}
//---------------------------------------------------------------------------
void CPostBuff::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.
}
//---------------------------------------------------------------------------
bool CPostBuff::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}
//---------------------------------------------------------------------------
bool CPostBuff::CheckStop()
{
    if(!MT_GetStop(miPSB_XCmp)) return false ;
    if(!AT_Done(aiPSB_Cmp    )) return false ;

    return true ;
}
//---------------------------------------------------------------------------
void CPostBuff::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PstBuff.INI";

    if ( IsLoad ) {
//        UserINI.Load(sPath, "Member" , "m_dFailChipPos" , m_dFailChipPos );
        UserINI.Load(sPath, "Member" , "m_dLastIndPos  " , m_dLastIndPos  );
        UserINI.Load(sPath, "Member" , "m_iReGripCnt   " , m_iReGripCnt   );
        UserINI.Load(sPath, "Member" , "m_iCrntCol     " , m_iCrntCol     );
        UserINI.Load(sPath, "Member" , "m_sWorkedLotNo " , m_sWorkedLotNo );


    }
    else {
        UserINI.ClearFile(sPath) ;
//        UserINI.Save(sPath, "Member" , "m_dFailChipPos" , m_dFailChipPos );
        UserINI.Save(sPath, "Member" , "m_dLastIndPos  " , m_dLastIndPos  );
        UserINI.Save(sPath, "Member" , "m_iReGripCnt   " , m_iReGripCnt   );
        UserINI.Save(sPath, "Member" , "m_iCrntCol     " , m_iCrntCol     );
        UserINI.Save(sPath, "Member" , "m_sWorkedLotNo " , m_sWorkedLotNo );


    }
}
//---------------------------------------------------------------------------

bool CPostBuff::GetLastStrip()
{
    AnsiString sLastLotNo = DM.ARAY[riPSB].GetCntExist() ? DM.ARAY[riPSB].GetLotNo() : m_sWorkedLotNo ;

    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPR1Exist = !DM.ARAY[riPR1].CheckAllStat(csNone) ;
    bool bPR2Exist = !DM.ARAY[riPR2].CheckAllStat(csNone) ;
    bool bWRKExist = !DM.ARAY[riWRK].CheckAllStat(csNone) ;

    bool bLDRDiff  = sLastLotNo != DM.ARAY[riLDR].GetLotNo() ;
    bool bPR1Diff  = sLastLotNo != DM.ARAY[riPR1].GetLotNo() ;
    bool bPR2Diff  = sLastLotNo != DM.ARAY[riPR2].GetLotNo() ;
    bool bWRKDiff  = sLastLotNo != DM.ARAY[riWRK].GetLotNo() ;

    bool bLastMgz  = LDR._iMgzCnt > OM.DevOptn.iLotEndMgzCnt ;

         if( bWRKExist) return bWRKDiff ;
    else if( bPR2Exist) return bPR2Diff ;
    else if( bPR1Exist) return bPR1Diff ;
    else if( bLDRExist) return bLDRDiff ;
    else if(!bLastMgz ) return false    ;
    else                return true     ;
}