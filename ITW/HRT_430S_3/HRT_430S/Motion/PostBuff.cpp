//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PostBuff.h"
#include "WorkZone.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
//#include "ULD_HRT430S.h"
#include "UnLoader430t.h"
#include "SortingTool.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CPostBuff PSB;
//---------------------------------------------------------------------------
CPostBuff::CPostBuff(void)
{
}
//---------------------------------------------------------------------------
void CPostBuff::Init()
{
    m_sPartName = "PostBuff " ;
    Reset();
    Load(true);
    m_iCrntCol = -1;
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

}

void CPostBuff::Close()
{
     Load(false);
}

//---------------------------------------------------------------------------
bool CPostBuff::FindChip(int &r , int &c , EN_CHIP_STAT Stat)
{
// TODO :: ?    c = DM.ARAY[riPS1].FindLastCol(Stat) ;
    c = DM.ARAY[riPSB].FindLastCol(Stat) ;
    return (c > -1 );
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
    double dXCmdPos    = MT_GetCmdPos(miPSB_XCmp);                            //��� ������
    double dXEncPos    = MT_GetEncPos(miPSB_XCmp);                            //���ڴ� ������
    double dXInpos     = 0.1 ;                                                //�˳��ϰ� �޴���� ������ ��������.
    double dFailPos    = GetMotrPos(miPSB_XCmp , piPSB_XWork , Mode);         //���������� �ִ� ������
    bool   bStoped     = MT_GetStop(miPSB_XCmp);                              //������������.
    bool   bXInposStop = bStoped && (abs(dXCmdPos - dXEncPos) < dXInpos) ;    //�޴���� ������ �������� ������ �������� ��ž Ȯ��.

    //bool   bNeedMove   = bXInposStop && dXCmdPos != dFailPos ;                //�������ǽ�ž�̰� ��������ߴ� �����ǰ� �������� �ִ� �������� �ٸ� ��� �̵��ʿ�.
    bool   bReadyPos   = bXInposStop && abs(dXCmdPos - dFailPos)< dXInpos ;     //�̻��ϴ� dXCmdPos == dFailPos �϶� ���� �ʴٰ� ���ϵ�.

    return bReadyPos ;
    ////////////////////////////////////////////////////////////////////////
}

//---------------------------------------------------------------------------
double CPostBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , EN_CHIP_STAT Mode)
{
    int  r ,c , iColGrCnt , iMaxCol, HalfCol ;
    int fc, hc;
    iMaxCol = DM.ARAY[riPSB].GetMaxCol();

         if(Mode == csWork)  FindChip(r , c , csWork);
    else if(Mode == csMask)  FindChip(r , c , csMask);
    else                     FindChip(r , c , csFail);


    fc = (iMaxCol - c) -1 ; //Ⱦ���� �ø��� ��.
    hc =  iMaxCol / 2;
    double dTrimColptch = OM.DevInfo.dColPitch + OM.DevOptn.dTrimPtchOfs ;

    double dX1Pos     = PM.GetValue(miPSB_XCmp , pvPSB_XWorkStartPs) + (dTrimColptch * fc)                                                  ;
    double dX2Pos     = PM.GetValue(miPSB_XCmp , pvPSB_XWorkStartPs) + (dTrimColptch * fc)                  - OM.DevOptn.dPSBReGripOfs ; //���׸��ϸ� 1��ġ�� ������ +1
    double dReGribPos = PM.GetValue(miPSB_XCmp , pvPSB_XWorkStartPs) + (dTrimColptch * OM.DevOptn.iPSBReGripCol) ;//- OM.DevOptn.dPSBReGripOfs ;
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

    if(_iMotr == miPSB_XCmp){
        switch(_iPstnId) {   //Pull�� ù��° �׸����� Drag �Ҷ� ������ ���� ������ Drag�� �ι�° �׸� �ϰ� ���� ���� ������.
            default               : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piPSB_XWait      : dPos = PM.GetValue (_iMotr , pvPSB_XWaitPs     ); break ;
            case piPSB_XGrip      : dPos = PM.GetValue (_iMotr , pvPSB_XGripPs     ); break ;
            case piPSB_XPull      : dPos = PM.GetValue (_iMotr , pvPSB_XPullPs     ); break ;
            case piPSB_XBack      : dPos = PM.GetValue (_iMotr , pvPSB_XBackPs     ); break ;
            case piPSB_XWorkStart : dPos = PM.GetValue (_iMotr , pvPSB_XWorkStartPs); break ;
            case piPSB_XOut       : dPos = PM.GetValue (_iMotr , pvPSB_XOutPs      ); break ;
            case piPSB_XReGrip    : dPos = dReGribPos ;                               break ;
            case piPSB_XWork      :      if (m_iReGripCnt == 0) dPos = dX1Pos       ;
                                    else if (m_iReGripCnt == 1) dPos = dX2Pos       ; break ;
            case piPSB_XDrag      : dPos = PM.GetValue (_iMotr , pvPSB_XPullPs ) - OM.DevOptn.dPSBReGripOfs; break ;
            case piPSB_XReBack    :      if (m_iReGripCnt == 0) dPos = PM.GetValue (_iMotr , pvPSB_XBackPs );
                                    else if (m_iReGripCnt == 1) dPos = PM.GetValue (_iMotr , pvPSB_XBackPs ) + OM.DevOptn.dPSBReGripOfs; break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}
//---------------------------------------------------------------------------
/*
double CPostBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r ,c , iColGrCnt , iMaxCol, HalfCol ;
    int fc, hc;
    iMaxCol = DM.ARAY[riPS2].GetMaxCol();
    FindChip(r , c);
    fc = (iMaxCol - c) -1 ; //Ⱦ���� �ø��� ��.
    hc = (iMaxCol - 1) / 2;

    double dX1Pos = PM.GetValue(miPSB_XCmp , pvPSB_XWorkStartPs) + (OM.DevInfo.dColPitch * fc)                               ;
    double dX2Pos = PM.GetValue(miPSB_XCmp , pvPSB_XWorkStartPs) + (OM.DevInfo.dColPitch * fc) - (OM.DevInfo.dColPitch * hc) ;
    double dPos   = 0.0 ;

    //���� ������ �ݴ�� ������ �ø��Ȱ����� ����Ѵ�.
    if( OM.DevInfo.iColGrCnt > 1 ) {
        //iColGrCnt��  fc  �� �׷찹��. OM.DevInfo.iColGrCnt�̰Ŵ� �ѱ׷�� LED ����.
        iColGrCnt = fc / OM.DevInfo.iColGrCnt ;
        dX1Pos += (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * iColGrCnt ;
        dX2Pos += (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * iColGrCnt ;
    }

    if(_iMotr == miPSB_XCmp){
        switch(_iPstnId) {
            default               : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piPSB_XWait      : dPos = PM.GetValue (_iMotr , pvPSB_XWaitPs     ); break ;
            case piPSB_XGrip      : dPos = PM.GetValue (_iMotr , pvPSB_XGripPs     ); break ;
            case piPSB_XPull      :      if (m_iReGripCnt == 0) dPos = PM.GetValue (_iMotr , pvPSB_XPullPs );
                                    else if (m_iReGripCnt == 1) dPos = PM.GetValue (_iMotr , pvPSB_XPullPs ) - PM.GetValue(_iMotr , pvPSB_XReGripPs); break ;
            case piPSB_XBack      : dPos = PM.GetValue (_iMotr , pvPSB_XBackPs     ); break ;
            case piPSB_XWorkStart : dPos = PM.GetValue (_iMotr , pvPSB_XWorkStartPs); break ;
            case piPSB_XOut       :      if (m_iReGripCnt == 0) dPos = PM.GetValue (_iMotr , pvPSB_XOutPs  );
                                    else if (m_iReGripCnt == 1) dPos = PM.GetValue (_iMotr , pvPSB_XOutPs  ) - PM.GetValue(_iMotr , pvPSB_XReGripPs); break ;
            case piPSB_XReGrip    : dPos = PM.GetValue (_iMotr , pvPSB_XReGripPs   ); break ;
            case piPSB_XWork      :      if (m_iReGripCnt == 0) dPos = dX1Pos       ;
                                    else if (m_iReGripCnt == 1) dPos = dX2Pos       ; break ;
            case piPSB_XDrag      :      if (m_iReGripCnt == 0) dPos = PM.GetValue (_iMotr , pvPSB_XPullPs );
                                    else if (m_iReGripCnt == 1) dPos = PM.GetValue (_iMotr , pvPSB_XPullPs  ) - PM.GetValue(_iMotr , pvPSB_XReGripPs); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}*/
//---------------------------------------------------------------------------
bool CPostBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isDrngPunch    = (AT_Complete(aiSTL_Top , ccFwd) || AT_Complete(aiSTL_Btm , ccFwd))  ;

    if(_iMotr == miPSB_XCmp){
        if(isDrngPunch) {sMsg = "During Punching"; bRet = false ;}
    }
    else {
        bRet = false ;
    }

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
            if(!isXMoving) { sMsg = MT_GetName(miPRB_XCmp)+" is Moving" ; bRet = false ; }
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
        EM_SetErr(eiPSB_HomeTo);
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

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                 Step.iHome = 0 ;
                 return true ;

        case 10: IO_SetY(yPSB_FeedingAC,false);
                 AT_MoveCyl(aiPSB_Cmp, ccBwd);
                 AT_MoveCyl(aiPSB_Psh, ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_Cmp, ccBwd))return false ;
                 if(!AT_MoveCyl(aiPSB_Psh, ccBwd))return false ;
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
        if (Stat.bReqStop)return false ;

        int iMaxCol, fc, iReGribCol;
        int r1,c1,r2,c2,r3,c3;

        iMaxCol = DM.ARAY[riWRK].GetMaxCol();
        iReGribCol = OM.DevOptn.iPSBReGripCol ; //30�̶�� ������ 30���� �۾� �ϰ� 31���� ���׸�.   iMaxCol / 2 ; //��Ʈ�� �߰����� �۾� �ϰ� ���׸��Ѵ�.

        //20190628 ����Ʈ���ۿ����� ��δ� �Ű��� �ε� �ȵ������� �ε��ϵ��� �ϱ� ���� �߰�
        //ULD.ResetChckSplyEmpty();

        //���� ������ �ǽɵǾ� ���������� ���� Ǯ�� ����. ��Ÿ� ���� ���ϸ��.
        double dXCmdPos    = MT_GetCmdPos(miPSB_XCmp);                            //��� ������
        double dXEncPos    = MT_GetEncPos(miPSB_XCmp);                            //���ڴ� ������
        double dXInpos     = 0.1 ;                                                //�˳��ϰ� �޴���� ������ ��������.
        double dFailPos    = GetMotrPos(miPSB_XCmp , piPSB_XWork , csFail);       //���������� �ִ� ������
        bool   bStoped     = MT_GetStop(miPSB_XCmp);                              //������������.
        bool   bXInposStop = bStoped && (abs(dXCmdPos - dXEncPos) < dXInpos) ;    //�޴���� ������ �������� ������ �������� ��ž Ȯ��.

        //bool   bNeedMove   = bXInposStop && dXCmdPos != dFailPos ;                //�������ǽ�ž�̰� ��������ߴ� �����ǰ� �������� �ִ� �������� �ٸ� ��� �̵��ʿ�.
        bool   bNeedMove   = bXInposStop && abs(dXCmdPos - dFailPos)> dXInpos ;                //�̻��ϴ� dXCmdPos == dFailPos �϶� ���� �ʴٰ� ���ϵ�.

        ////////////////////////////////////////////////////////////////////////

        bool bExistFail     = DM.ARAY[riPSB].GetCntStat(csFail ); //FindChip(r1 , c1 , csFail);
        bool bExistWork     = DM.ARAY[riPSB].GetCntStat(csWork ); //FindChip(r2 , c2 , csWork);
        bool bExistMask     = DM.ARAY[riPSB].GetCntStat(csMask ); //FindChip(r3 , c3 , csMask);
        bool bFinished      =((!bExistFail && !bExistWork && !bExistMask) || OM.CmnOptn.bIgnrTrim) && STL.GetSeqStep() == scIdle; //���� ��Ʈ���� �۾��� �������� Ȯ����.

        bool isExistWork    = !DM.ARAY[riPSB].GetCntStat(csWork ) && !DM.ARAY[riPSB].GetCntStat(csMask ) && !DM.ARAY[riPSB].GetCntStat(csFail ) ;

        FindChip(r1 , c1 , csFail);
        fc = (iMaxCol - c1) -1  ; //���� �����͸� ������ ��.

        bool isExstTrim1    =  bExistFail && fc <  iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //��Ʈ�� �������� Ʈ�� �� ���� �ִ��� �ϴ� �ɼ�.
        bool isExstTrim2    =  bExistFail && fc >= iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //��Ʈ�� �Ĺ����� Ʈ�� �� ���� �ִ��� �ϴ� �ɼ�.

        // Test juhyeon
//        bool b1 = !DM.ARAY[riWRK].GetCntStat(csNone) && !DM.ARAY[riWRK].GetCntStat(csUnkwn) ;// && DM.ARAY[riWRK].GetStep() == 1 ;
//        bool b2 = !DM.ARAY[riPSB].GetStep() && WRK.GetSeqStep() != CWorkZone :: scIdle ;

        //bool bTemp1 = !DM.ARAY[riWRK].GetCntStat(csNone) && !DM.ARAY[riWRK].GetCntStat(csUnkwn);
        //bool bTemp2 = DM.ARAY[riWRK].GetStep() == 1;
        //bool bTemp3 = WRK.GetSeqStep() != CWorkZone :: scIdle ;
        bool isCyclePreDrag = !DM.ARAY[riWRK].GetCntStat(csNone) && !DM.ARAY[riWRK].GetCntStat(csUnkwn) && DM.ARAY[riWRK].GetStep() == 1 &&
                               WRK.GetSeqStep() == CWorkZone :: scIdle ;
                              // ������ �ִ� ��...
                              //!DM.ARAY[riWRK].GetCntStat(csNone) && !DM.ARAY[riWRK].GetCntStat(csUnkwn) && DM.ARAY[riWRK].GetStep() == 1 &&
                              //!DM.ARAY[riPSB].GetStep() && WRK.GetSeqStep() != CWorkZone :: scIdle ; //ó���� ������ �μ� �ް� ��ŷ������ ���� ���� ���� �����͸� �ϳ� ����� �Ⱦ ������ ����.
        bool isCycleDrag    = bFinished && !DM.ARAY[riPSB].CheckAllStat(csNone) && !DM.ARAY[riPSB].GetSubStep() ; //�۾��� �� �������� �巹��.
//TODO :: �����ϴµ� ���׸��� �Ÿ��� �� �¾Ƽ� �ٲ۴�. dd �ӽ�

        bool isCycleReGrip  =!isExstTrim1 && isExstTrim2 &&      //  1���� fail ����. 2���� fail�ְ�
                             !bExistWork  && !bExistMask &&      //  Work�� Mask�� ������ �ȵ�.
                              m_iReGripCnt == 0 &&                                             //���׸��� ���߾�߸� �Ѵ�.
                              ULD.GetSeqStep() == CUnLoader    :: scIdle && DM.ARAY[riULD].GetCntStat(csEmpty) && // ��δ� �غ� �Ǿ� �־�߸� �Ѵ�. �׷��� �����ҵ�.
                              STL.GetSeqStep() == CSortingTool :: scIdle ;                                        // �������� ���� ����� �ϰ� �̾� �ϴ� �̷��� ����.;
//        bool isCycleReGrip  = false ;

        bool isCycleWork    =((isExstTrim1 && m_iReGripCnt == 0) || (isExstTrim2 && m_iReGripCnt == 1)) && //regripcnt�� �־�߸� �ް��� ü�����ÿ� �Ĺݺο���  ���� ���� ������ ���� ����.
                             !bExistWork && !bExistMask &&                      //��Ʈ������ Work�� Mask�� ������ �������� �ʰ� �ٲ�.
                              STL.GetSeqStep() != CSortingTool :: scPick &&     //�������� Pick�ൿ�� �ϰ� ������ ��������. ��� �Ǵµ� ������.
                              bExistFail && bNeedMove ;                         //���ϵ��� �ִµ� , ���� ��ġ�� �ٸ����.

        bool isCycleOut     = (!DM.ARAY[riPSB].GetCntStat(csNone )  && DM.ARAY[riPSB].GetSubStep() == 1 ) &&
                              (isExistWork || OM.CmnOptn.bIgnrTrim) &&
                               DM.ARAY[riULD].GetCntStat(csEmpty)   &&
                               ULD.GetSeqStep() == CUnLoader :: scIdle ;

        bool isCycleEnd     =  DM.ARAY[riPSB].GetCntStat(csNone) ;//&&  DM.ARAY[riPS2].GetCntStat(csNone) ;




        //�𸣴� ��Ʈ������.
        if( DM.ARAY[riPSB].CheckAllStat(csNone) && !OM.CmnOptn.bDryRun) {        //�������� ��ũ���� ������ ���� ���� �������־ ���� �� .
            if(IO_GetX(xPSB_3Pkg) || IO_GetX(xPSB_4Pkg) || IO_GetX(xPSB_PkgOut1) /*|| IO_GetX(xPSB_PkgOut2)*/ ) EM_SetErr(eiPSB_Unknown) ;
        }

        //strip �����.
//        if( DM.ARAY[riPSB].GetCntExist() && !OM.CmnOptn.bDryRun) {
//            if(!IO_GetX(xPSB_1Pkg) && !IO_GetX(xPSB_2Pkg) && !IO_GetX(xPSB_3Pkg) && !IO_GetX(xPSB_4Pkg) && !IO_GetX(xPSB_PkgOut1) )EM_SetErr(eiPSB_Dispr) ;
//        }
//        if( isCycleOut && DM.ARAY[riPSB].GetCntExist() && !OM.CmnOptn.bDryRun) {
//            if(!IO_GetX(xPSB_3Pkg) && !IO_GetX(xPSB_4Pkg) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2))EM_SetErr(eiPSB_Dispr) ;
//        }

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCyclePreDrag) {Trace(m_sPartName.c_str(),"CyclePreDrag Stt" ); Step.iSeq = scPreDrag; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleDrag   ) {Trace(m_sPartName.c_str(),"CycleDrag    Stt" ); Step.iSeq = scDrag   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleReGrip ) {Trace(m_sPartName.c_str(),"CycleReGrip  Stt" ); Step.iSeq = scReGrip ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork   ) {Trace(m_sPartName.c_str(),"CycleWork    Stt" ); Step.iSeq = scWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut    ) {Trace(m_sPartName.c_str(),"CycleOut     Stt" ); Step.iSeq = scOut    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default    :                        Trace(m_sPartName.c_str(),"default      End");Step.iSeq = scIdle ;  return false ;
        case scIdle   :                                                                                         return false ;
        case scPreDrag: if(CyclePreDrag()){ Trace(m_sPartName.c_str(),"CyclePreDrag End");Step.iSeq = scIdle ;} return false ;
        case scDrag   : if(CycleDrag   ()){ Trace(m_sPartName.c_str(),"CycleDrag    End");Step.iSeq = scIdle ;} return false ;
        case scReGrip : if(CycleReGrip ()){ Trace(m_sPartName.c_str(),"CycleReGrip  End");Step.iSeq = scIdle ;} return false ;
        case scWork   : if(CycleWork   ()){ Trace(m_sPartName.c_str(),"CycleWork    End");Step.iSeq = scIdle ;} return false ;
        case scOut    : if(CycleOut    ()){ Trace(m_sPartName.c_str(),"CycleOut     End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}


//---------------------------------------------------------------------------
bool CPostBuff::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.             if (Step.iSeq) return false;
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
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 10000)) EM_SetErr(eiPSB_ToStartTO);

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
                 if(DM.ARAY[riPSB].CheckAllStat(csNone))  MoveActr(aiPSB_Cmp , ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(DM.ARAY[riPSB].CheckAllStat(csNone) && !MoveActr(aiPSB_Cmp , ccBwd)) return false ;

                 if(DM.ARAY[riPSB].CheckAllStat(csNone))  MoveMotr(miPSB_XCmp , piPSB_XWait);

                 Step.iToStart++;
                 return false ;

        case 12: if(DM.ARAY[riPSB].CheckAllStat(csNone) && !MoveMotr(miPSB_XCmp , piPSB_XWait)) return false ;


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

        case 11: if(!MT_GetAlarm   (miPSB_XCmp) &&  //���������� ������ ������ ��� �ϱ����ؼ� �ε� ������ ġ�� �ְų� �˶����̸� ���V�ϵ��� .
                    !MT_GetNLimSnsr(miPSB_XCmp) &&
                    !MT_GetPLimSnsr(miPSB_XCmp) ) m_dLastIndPos = MT_GetCmdPos(miPSB_XCmp) ;
                 else                             m_dLastIndPos = GetMotrPos  (miPSB_XCmp , piPSB_XWait) ;

                 Step.iToStop = 0   ;
                 return true ;
    }
}
//---------------------------------------------------------------------------
//One Cycle.

bool CPostBuff::CyclePreDrag() //WorkZone ���� ���� ���� ���� ������  �ѹ� Ÿ�� �۾� ���� ��ġ�� ���� ����.
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
                  DM.ShiftArrayData(riWRK, riPSB);
                  if(STL.m_sLastLotNo != DM.ARAY[riPSB].GetLotNo()){
                      STL.m_sLastLotNo = DM.ARAY[riPSB].GetLotNo();
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
                  MoveMotr(miPSB_XCmp, piPSB_XGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XCmp, piPSB_XGrip)) return false ;
                  MoveActr(aiPSB_Cmp, ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  MoveMotr(miPRB_XCmp, piPSB_XWorkStart);
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
                  DM.ARAY[riPSB].SetStep(1);

                  STL.RejectMove(); //������ �ϳ��� �ȳ��� �ް�����

                  m_iReGripCnt = 0;
                  Step.iCycle = 0;
                  FindChip(r , c , csFail);
                  //for (int i = 0; i < iMaxRow; i++)
                  //{
                  //    if(DM.ARAY[riPS1].GetStat(i , c) == csFail)
                  //    {
                  //        DM.ARAY[riPS1].SetStat(i,c,csUnkwn);
                  //    }
                  //}
                  m_iCrntCol = c;
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
                  if(!m_iReGripCnt)MoveMotr(miPSB_XCmp , piPSB_XPull) ;
                  //else             MoveMotr(miPSB_XCmp , piPSB_XDrag) ;
                  else             MoveMotr(miPSB_XCmp , piPSB_XPull) ;
                  Step.iCycle++;
                  return false ;

        case  12: if (!m_iReGripCnt && !MoveMotr(miPSB_XCmp , piPSB_XPull)) return false ;
                  if ( m_iReGripCnt && !MoveMotr(miPSB_XCmp , piPSB_XPull)) return false ;
                  MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPSB_Cmp,ccBwd     )) return false ;
                  MoveMotr(miPSB_XCmp,piPSB_XReBack ) ;
//                  DM.ShiftArrayData(riPS1,riPS2);
                  DM.ARAY[riPSB].SetSubStep(1);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XCmp,piPSB_XReBack )) return false ;
                  //MoveMotr(miPSB_XCmp,piPSB_XOut) ;
                  MoveActr(aiPSB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPSB_Cmp,ccFwd)) return false ;


                  Step.iCycle = 0 ;
                  return true ;
    }
}
//---------------------------------------------------------------------------
/*
bool CPostBuff::CycleGood()
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

        case  10: MoveActr(aiPSB_Cmp, ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  MoveMotr(miPSB_XCmp , piPSB_XPull) ;
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miPSB_XCmp , piPSB_XPull)) return false ;
                  MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiPSB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPSB_XCmp , piPSB_XBack) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XCmp , piPSB_XBack)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
        }

}
*/

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
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  MoveMotr(miPSB_XCmp, piPSB_XWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XCmp, piPSB_XWork)) return false ;
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

        case  10: //MoveActr(aiPSB_Cmp, ccFwd);
                  //STL.MoveMotr(miSTL_YTop,piSTL_YTWorkStart); //�̾� ��ø� ����...
                  //STL.MoveMotr(miSTL_YBtm,piSTL_YBWorkStart); //�̾� ��ø� ����...
                  Step.iCycle++;
                  return false ;

        case  11: //if(!MoveActr(aiPSB_Cmp, ccFwd)) return false ;
                  //MT_GoAbsRun(miPSB_XCmp , OM.DevInfo.dColPitch);
                  //if(!STL.MoveMotr(miSTL_YTop,piSTL_YTWorkStart)) return false ; //�̾� ��ø� ����...
                  //if(!STL.MoveMotr(miSTL_YBtm,piSTL_YBWorkStart)) return false ; //�̾� ��ø� ����...
                  Step.iCycle++;
                  return false ;


        case  12: //if(!MT_GetStop(miPSB_XCmp)) return false ;
                  //MoveActr(aiPSB_Cmp, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: //if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  MoveActr(aiPSB_Cmp , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  MoveMotr(miPSB_XCmp,piPSB_XReGrip);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miPSB_XCmp,piPSB_XReGrip)) return false;
                  MoveActr(aiPSB_Cmp, ccBwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiPSB_Cmp, ccBwd)) return false;
                  MT_GoIncRun(miPSB_XCmp , -OM.DevOptn.dPSBReGripOfs) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MT_GetStop(miPSB_XCmp)) return false ;
                  MoveActr(aiPSB_Cmp, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveActr(aiPSB_Cmp , ccFwd)) return false ;



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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yPSB_FeedingAC,true);
                  MoveActr(aiPSB_Cmp, ccFwd);
                  LT.WriteBinInfo (DM.ARAY[riPSB].GetLotNo(), STL.m_iBin0Cnt, STL.m_iBin1Cnt, STL.m_iBin2Cnt, STL.m_iBin3Cnt, STL.m_iBin4Cnt,
                                                              STL.m_iBin5Cnt, STL.m_iBin6Cnt, STL.m_iBin7Cnt, STL.m_iBin8Cnt, STL.m_iBin9Cnt);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  if(ULD.GetSeqStep() != CUnLoader :: scIdle) return false  ; //����¥������ 1����Ŭ ���� �� ��� �ü� �־ ���Ƴ���.
                  MoveMotr(miPSB_XCmp,piPSB_XOut);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miPSB_XCmp,piPSB_XOut)) return false ;
                  MoveActr(aiPSB_Cmp,ccBwd);

                  Step.iCycle++;
                  return false ;


        case  13: if(!MoveActr(aiPSB_Cmp,ccBwd)) return false ;
                  Step.iCycle++;
                  return false ;

        case  14: if(IO_GetX(xPSB_PkgOut1)) return false ;
                  MoveActr(aiPSB_Psh,ccFwd);
                  MoveMotr(miPSB_XCmp,piPSB_XWait);

                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiPSB_Psh,ccFwd)) return false ;
                  IO_SetY(yPSB_FeedingAC,false);

                  DM.ARAY[riPSB].SetStat(csNone) ;
                  if(DM.ARAY[riULD].CheckAllStat(csEmpty)) DM.ARAY[riULD].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);

                  if(IO_GetX(xPSB_PkgOut2)){ AT_MoveCyl(aiPSB_Psh,ccBwd); EM_SetErr(eiPSB_Out2Detected); return true; }
                  AT_MoveCyl(aiPSB_Psh,ccBwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!AT_MoveCyl(aiPSB_Psh,ccBwd)) return false ;
                  if(!MoveMotr(miPSB_XCmp,piPSB_XWait)) return false ;
                  Step.iCycle = 0;
                  return true;
    }
}
/*bool CPostBuff::CycleOut()
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

        case  10: MoveActr(aiPSB_Cmp, ccBwd);
                  MoveActr(aiSTL_Top, ccBwd);
                  MoveActr(aiSTL_Btm, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Cmp, ccBwd)) return false;
                  if(!MoveActr(aiSTL_Top, ccBwd)) return false;
                  if(!MoveActr(aiSTL_Btm, ccBwd)) return false;
                  MoveMotr(miPSB_XCmp,piPSB_XGrip);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XCmp,piPSB_XGrip)) return false;
                  MoveActr(aiPSB_Cmp, ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_Cmp, ccFwd)) return false;
                  MoveMotr(miPSB_XCmp , piPSB_XPull) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XCmp , piPSB_XPull)) return false ;
                  MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPSB_Cmp,ccBwd)) return false ;
                  MoveMotr(miPSB_XCmp , piPSB_XBack) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miPSB_XCmp , piPSB_XBack)) return false ;
                  MoveActr(aiPSB_Cmp,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPSB_Cmp,ccFwd)) return false ;
                  MT_GoAbs(miPSB_XCmp,GetMotrPos(miPSB_XCmp,piPSB_XOut),OM.DevOptn.dPostRailOutVel);
//                  DM.ARAY[riPS2].SetStat(csNone);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MT_GetStopInpos(miPSB_XCmp)) return false ;
                  if(!IO_GetX(xPSB_PkgOut1      )) { EM_SetErr(eiPSB_Out1Sensor); return false; }
                  MoveActr(aiPSB_Cmp,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveActr(aiPSB_Cmp,ccBwd)) return false ;
                  IO_SetY(yPSB_FeedingAC,true);
                  Step.iCycle++;
                  return false ;

        case  20: if(IO_GetX(xPSB_PkgOut1)) return false ;
                  IO_SetY(yPSB_FeedingAC,false);
                  MoveActr(aiPSB_Psh, ccFwd);
                  Step.iCycle++;
                  return false ;

        case  21: if(!MoveActr(aiPSB_Psh, ccFwd)) return false;
                  if(IO_GetX(xPSB_PkgOut2)) { EM_SetErr(eiPSB_Out2Sensor); return false; }
                  DM.ARAY[riPS2].SetStat(csNone);
                  if(DM.ARAY[riULD].CheckAllStat(csEmpty)) DM.ARAY[riULD].SetLotNo(DM.ARAY[riPS2].GetLotNo());
                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);

                  MoveActr(aiPSB_Psh, ccBwd);
                  MoveMotr(miPSB_XCmp,piPSB_XWait);
                  Step.iCycle++;
                  return false;

        case  22: if(!MoveActr(aiPSB_Psh, ccBwd)      ) return false ;
                  if(!MoveMotr(miPSB_XCmp,piPSB_XWait)) return false ;
                  IO_SetY(yPSB_FeedingAC, false);
                  Step.iCycle = 0 ;
                  return true ;


    }
}
*/
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
        UserINI.Load(sPath, "Member" , "m_dLastIndPos " , m_dLastIndPos  );
        UserINI.Load(sPath, "Member" , "m_iReGripCnt  " , m_iReGripCnt   );
        UserINI.Load(sPath, "Member" , "m_iCrntCol    " , m_iCrntCol     );

    }
    else {
        UserINI.ClearFile(sPath) ;
//        UserINI.Save(sPath, "Member" , "m_dFailChipPos" , m_dFailChipPos );
        UserINI.Save(sPath, "Member" , "m_dLastIndPos " , m_dLastIndPos  );
        UserINI.Save(sPath, "Member" , "m_iReGripCnt  " , m_iReGripCnt   );
        UserINI.Save(sPath, "Member" , "m_iCrntCol    " , m_iCrntCol     );

    }
}
//---------------------------------------------------------------------------


