//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Index.h"
#include "LoaderRear.h"
//#include "RunThread.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "Stage.h"
//#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"
//#include "Stage.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CIndex IDX;

#define GET_OFFSET_Z 6.0

CIndex::CIndex(void)
{

}

CIndex::~CIndex (void)
{

}

void CIndex::Init()
{
    m_sPartName = "Index" ;
    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

}

void CIndex::Close()
{
    Load(false);
}

void CIndex::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}



bool CIndex::FindChip(int &r ,int &c , EN_CHIP_STAT _eStat)
{    //FindChip(r,c,csUnkwn,riLDR_R);
    return LDR_R.FindChip(r,c,_eStat);
}

//bool CIndex::FindChip(int &c , EN_ARAY_ID &id)
//{
    //int iWorkC = DM.ARAY[Add.riIDX].FindFrstCol(Add.csAfterWork) ;
    //int iWaitC = DM.ARAY[Add.riIDX].FindFrstCol(Add.csWait     ) ;
    //int iC     = iWorkC > iWaitC ? iWorkC

//    int iWaitC   = DM.ARAY[riIDX].FindLastCol(csWait ) ;
//    int iUnknwnC = DM.ARAY[riIDX].FindLastCol(csUnkwn) ;
//
//    int iC ;
//    if(iWaitC != -1 && iUnknwnC != -1 ){
//        iC = iWaitC > iUnknwnC ? iWaitC : iUnknwnC ;
//    }
//    else if( iWaitC != -1){
//        iC = iWaitC ;
//    }
//    else if( iUnknwnC != -1){
//        iC = iUnknwnC ;
//    }
//    else {
//        iC = -1 ;
//    }
//
//    id = riIDX ;
//    c  = iC ;
//    return true ;
//}


double CIndex::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;
    int c ;
    EN_ARAY_ID _iAray;
//    FindChip(c , _iAray)    ; //���� �۾��ؾ��� ������.

//    double dWork = PM.GetValue (_iMotr , pvIDX_XIndxWorkStart) + (OM.DevInfo.iBoatColCnt-c-1) * OM.DevInfo.dBoatColPitch ;
    //double dGetPos = PM.GetValue(_iMotr, pvIDX_XIndxRGetFrst) + OM.DevInfo.dCmsWidth ;


    if(_iMotr == miIDX_XIndxR){
             if(_iPstnId ==  piIDX_XIndxRWait   ) dPos = PM.GetValue (_iMotr , pvIDX_XIndxRWait   );
        else if(_iPstnId ==  piIDX_XIndxRGetWait) dPos = PM.GetValue (_iMotr , pvIDX_XIndxRGetWait);
        else if(_iPstnId ==  piIDX_XIndxRCheck1 ) dPos = PM.GetValue (_iMotr , pvIDX_XIndxRCheck1 ); // CMOS �δ����� CMOS Index ���� ù ��° ��ġ
        else if(_iPstnId ==  piIDX_XIndxRCheck2 ) dPos = PM.GetValue (_iMotr , pvIDX_XIndxRCheck2 );
        else if(_iPstnId ==  piIDX_XIndxRRotator) dPos = PM.GetValue (_iMotr , pvIDX_XIndxRRotator);
        else                                      dPos = MT_GetCmdPos(_iMotr                      );
    }

    else if(_iMotr == miIDX_TRotat){
             if(_iPstnId ==  piIDX_TRotatWait  ) dPos = PM.GetValue (_iMotr , pvIDX_TRotatWait   );
        else if(_iPstnId ==  piIDX_TRotatRotate) dPos = PM.GetValue (_iMotr , pvIDX_TRotatRotate );
        else if(_iPstnId ==  piIDX_TRotatNormal) dPos = PM.GetValue (_iMotr , pvIDX_TRotatNormal );
        else                                     dPos = MT_GetCmdPos(_iMotr                      );
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CIndex::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}
void CIndex::Update()
{
//    if(IO_GetXDn(Add.xIDX_IndxNotOvLd)) {
//        EM_SetErrMsg(eiPRT_OverLoad , (m_sPartName + " Index���� �����ε尡 �߻��Ǿ����ϴ�.").c_str());
//        MT_EmgStop(Add.miIDX_XIndx);
//        MoveActr(Add.aiIDX_IndxUpDn , ccFwd);
//    }
//    if(DM.ARAY[Add.riPRI].CheckAllStat(csNone)){
//        m_tmHeat.Clear();
//    }
}

bool CIndex::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CIndex::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CIndex::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

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

        case 10: // ������Ʈ ����Ŭ�� �ƴѵ�, ������Ʈ �Ǹ����� �ö� ������ ���� ����
                 if(!DM.ARAY[riRTT].CheckAllStat(csNone)){
                     Step.iToStart = 0 ;
                     return false ;
                 }
                 MoveMotr(miIDX_TRotat , piIDX_TRotatWait);

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveMotr(miIDX_TRotat , piIDX_TRotatWait)) return false ;
                 MoveActr(aiIDX_RoatorUpDn , ccBwd);
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CIndex::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

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

        case 10: //MoveActr(aiIDX_RoatorUpDn, ccBwd);

                 Step.iToStop++;
                 return false ;

        case 11: //if(!MoveActr(aiIDX_RoatorUpDn, ccBwd)) return false ;
                 SetLastCmd();
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CIndex::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;                   // piIDX_XIndxRGetFrst        , // CMOS �δ����� CMOS Index ���� ù ��° ��ġ

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;


            int r,c ;

/*   20181029 ����ö���� �ε������� �浹������ ���� ��� ������ �ε��� Get������ �ȵǴ� ������ ������Ʈ.
            bool isWorking = STG.GetWorkingRC(r,c);

            int  iMaxModuleCnt    = OM.DevInfo.iMidColCnt * OM.DevInfo.iMidRowCnt ;
            int  iMidHeightCnt    = DM.ARAY[riSTG].GetCntStat(csMidREndHeight ); //1���۾� ���� ����.
            int  iCmsREndAlignCnt = DM.ARAY[riSTG].GetCntStat(csCmsREndAlign  );

            //Epoxy First üũ�ڽ����� �޺��ڽ��� �ɼ� ���� ����
            int  iWorkWaitCnt     = OM.DevOptn.bUseFstDisp ? DM.ARAY[riSTG].GetCntStat(csCmsLDisp1 )+DM.ARAY[riSTG].GetCntStat(csCmsRNeed) : DM.ARAY[riSTG].GetCntStat(csCmsRNeed) ;

            //2��°�� �۾� ���ʹ� ���� �۾��ϴ� ���� ž������ �۾� �Ҷ��� �������� ��Ÿ�� ����.
            bool bNeedNext        =  DM.ARAY[riSTG].GetCntStat(csCmsRNeed) &&
                                   ((iMidHeightCnt + iWorkWaitCnt + iCmsREndAlignCnt)==iMaxModuleCnt);//���߿� ��ŵ �Ѱ� ���� �� ���.
                                                                                                      //Fix �� TopUV�ϰ��ιٲ�� �Ѵ�

            bool isCycleGet     =  !OM.CmnOptn.bRLoadingStop &&
                                   MT_CmprPos(miLDR_ZElevR , LDR_R.GetMotrPos(miLDR_ZElevR , piLDR_ZElevRPick )) &&
                                  (DM.ARAY[riSTG  ].CheckAllStat(csNone) || bNeedNext  ) &&
                                   DM.ARAY[riTRF  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riIDX  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riRTT  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riLDR_R].GetCntStat  (csUnkwn)     &&
                                   DM.ARAY[riLDR_R].GetCntStat  (csDetect) ==0&&
                                   LDR_R.GetSeqStep() == CLoaderRear::scIdle ;//  !IO_GetX(xIDX_Detect)    ;
            bool isCyclePlace   = MT_CmprPos(miLDR_ZElevR , LDR_R.GetMotrPos(miLDR_ZElevR , piLDR_ZElevRPlce )) &&
                                  DM.ARAY[riRTT  ].CheckAllStat(csWorkEnd) &&
                                  LDR_R.GetSeqStep() == CLoaderRear::scIdle ;
*/
//20181029 ������... Ȧ�� ����.
            bool isWorking = STG.GetWorkingRC(r,c);

            int  iMidREndHeightCnt = DM.ARAY[riSTG].GetCntStat(csMidREndHeight ); //1���۾� ���� ����.
            int  iCmsREndAlignCnt  = DM.ARAY[riSTG].GetCntStat(csCmsREndAlign  ); //���� �������۾���(�����Ȯ�� Or ����ȭ��)�� �ε����� �δ������ ����������.
            int  iWorkWaitCnt      = DM.ARAY[riSTG].GetCntStat(csCmsLDisp1 )+DM.ARAY[riSTG].GetCntStat(csCmsRNeed);


            bool bStgNeedModule   =  DM.ARAY[riSTG].GetCntStat(csCmsRNeed) && //RNeed�� ���־�� ��. �̰� ������ �������̵�� �۾��ϰ� ��������.
                                   ((iMidREndHeightCnt + iWorkWaitCnt + iCmsREndAlignCnt)==OM.DevInfo.iMidColCnt * OM.DevInfo.iMidRowCnt);//�ٸ����´� �����ϰ� ������(�����Ȯ�� Or ����ȭ��)���ɷ���.

            //�̵���� �δ��� �����ְų� �������Ͽ� �۾��Ұ� �ִ°��.
            bool bExistMidBfStg =  DM.ARAY[riLDR_F].GetCntStat(csUnkwn   ) ||
                                   DM.ARAY[riPRL  ].GetCntStat(csUnkwn   ) ;

            bool isCycleGet     =  !OM.CmnOptn.bRLoadingStop &&
                                   MT_CmprPos(miLDR_ZElevR , LDR_R.GetMotrPos(miLDR_ZElevR , piLDR_ZElevRPick )) &&
                                  (bExistMidBfStg   || bStgNeedModule  )      &&
                                   DM.ARAY[riTRF  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riIDX  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riRTT  ].CheckAllStat(csNone)      &&
                                   DM.ARAY[riLDR_R].GetCntStat  (csUnkwn)     &&
                                   DM.ARAY[riLDR_R].GetCntStat  (csDetect) ==0&& //ó�����δ� �������� ���̽ó���.
                                   LDR_R.GetSeqStep() == CLoaderRear::scIdle ;//  !IO_GetX(xIDX_Detect)    ;
            bool isCyclePlace   = MT_CmprPos(miLDR_ZElevR , LDR_R.GetMotrPos(miLDR_ZElevR , piLDR_ZElevRPlce )) &&
                                  DM.ARAY[riRTT  ].CheckAllStat(csWorkEnd) &&
                                  LDR_R.GetSeqStep() == CLoaderRear::scIdle ;

    //�ٽ� �ؾ���..
    //bool isCycleRotator =  DM.ARAY[riIDX].CheckAllStat(csVisnAt) && DM.ARAY[riRTT].CheckAllStat(csNone) &&  IO_GetX(xIDX_RotatorDn) ;
    bool isCycleRotator = false;
    bool isCycleEnd     = DM.ARAY[riRTT].CheckAllStat(csNone) ;


    if(EM_IsErr()) return false ;

        //eiULD_MgzSupply �߰�
//        if (!OM.MstOptn.bDebugMode) {
//            if (!DM.ARAY[Add.riIDX   ].CheckAllStat(csNone ) &&
//                !DM.ARAY[Add.riULDTop].GetCntStat  (csEmpty) &&
//                !DM.ARAY[Add.riULDBtm].GetCntStat  (csEmpty)) EM_SetErrMsg(eiULD_MgzSupply , "UnLoder Mgz Supply Error.") ;
//        }

         //Normal Decide Step.
             if (isCycleGet    ) {Trace(m_sPartName.c_str(),"CycleGet      Stt"); Step.iSeq = scGet        ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCyclePlace  ) {Trace(m_sPartName.c_str(),"CyclePlace    Stt"); Step.iSeq = scPlace      ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleRotator) {Trace(m_sPartName.c_str(),"CycleRotator  Stt"); Step.iSeq = scRotator    ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
   }

    //Cycle.
    switch (Step.iSeq) {
        default           :                      Trace(m_sPartName.c_str(),"default     End");                                                   Step.iSeq = scIdle ;  return false ;
        case scIdle       :                                                                                                                                            return false ;
        case scGet        : if(CycleGet     ()){ Trace(m_sPartName.c_str(),"CycleGet    End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPlace      : if(CyclePlace   ()){ Trace(m_sPartName.c_str(),"CyclePlace  End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scRotator    : if(CycleRotator ()){ Trace(m_sPartName.c_str(),"Rotator     End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CIndex::IsWorkPos()
{
//    return MT_CmprPos(miIDX_XIndxR ,GetMotrPos(miIDX_XIndxR , piIDX_XIndxWork)) ;

}

bool CIndex::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iHome=%02d" , __FUNC__ , Step.iHome );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiPRT_HomeTo,sTemp.c_str());
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

        case 10: IO_SetY(yIDX_RttPumpOn , true);
                 m_tmDelay.Clear();
                 Step.iHome++;
                 return false ;

        case 11: if(!m_tmDelay.OnDelay(true , 1000))return false ;
                 if(!DM.ARAY[riRTT].CheckAllStat(csNone) ){ //���� �ִ°��.
                     Step.iHome=20 ;
                     return false ;
                 }
                 //���簡 ������ 30��.
                 IO_SetY(yIDX_RttPumpOn, false);
                 Step.iHome=30 ;
                 return false ;


        //������ ��.
        //Rotator ���簡 ������ ���.
        case 20: MT_DoHome(miIDX_XIndxR);
                 Step.iHome++;
                 return false ;

        case 21: if(!MT_GetHomeEnd(miIDX_XIndxR)) return false ;
                 MoveActr(aiIDX_RoatorUpDn, ccFwd);
                 //if(!AT_Complete(aiIDX_RoatorUpDn, ccFwd)) return false ;
                 Step.iHome++;
                 return false ;

        case 22: if(!MoveActr(aiIDX_RoatorUpDn, ccFwd))return false;
                 MT_DoHome(miIDX_TRotat);
                 Step.iHome++;
                 return false;

        case 23: if(!MT_GetHomeEnd(miIDX_TRotat)) return false;
                 //if(!OM.DevOptn.bNotUseRotate && DM.ARAY[riSTG].GetCntStat(csWork) % 2){ //Ȧ��  1,3,5
                 //    MoveMotr(miIDX_TRotat , piIDX_TRotatRotate);
                 //}
                 //else {
                 //    MoveMotr(miIDX_TRotat , piIDX_TRotatNormal);
                 //
                 //}

                 MoveMotr(miIDX_TRotat , piIDX_TRotatNormal);
                 Step.iHome++;
                 return false ;

        case 24: if(!MT_GetStopInpos(miIDX_TRotat)) return false ;
                 Step.iHome = 0;
                 return true;


        //������ ��.
        //Rotator ���� ������ ���.
        case 30: MoveActr(aiIDX_RoatorUpDn, ccBwd);
                 Step.iHome++;
                 return false ;

        case 31: if(!MoveActr(aiIDX_RoatorUpDn, ccBwd)) return false ;
                 MT_DoHome(miIDX_XIndxR);
                 Step.iHome++;
                 return false ;

        case 32: if(!MT_GetHomeEnd(miIDX_XIndxR)) return false ;
                 MT_DoHome(miIDX_TRotat);
                 Step.iHome++;
                 return false ;

        case 33: if(!MT_GetHomeEnd(miIDX_TRotat)) return false ;
                 MoveMotr(miIDX_TRotat , piIDX_TRotatWait);
                 Step.iHome++;
                 return false ;

        case 34: if(!MT_GetStopInpos(miIDX_TRotat)) return false ;
                 Step.iHome = 0;
                 return true;
    }
}

//One Cycle.
bool CIndex::CycleGet()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
        //Step.iCycle = 0;
        //return true ;
    }

    int r,c ;
    FindChip(r,c,csUnkwn);
    int iRc = OM.DevInfo.iLDRRMgzBayCnt - c - 1 ;
    String sModID ;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //if(IO_GetX(xIDX_Detect)) {
                 //     EM_SetErrMsg(eiPRT_Crash, (m_sPartName + "CMOS ���簡 �ֽ��ϴ�. ���縦 �������ּ���.").c_str());
                 //     Step.iCycle = 0 ;
                 //     return true ;
                 //}
                 MoveActr(aiIDX_RoatorUpDn , ccBwd);
                 MoveMotr(miIDX_TRotat , piIDX_TRotatWait);
                 LDR_R.MoveMotr(miLDR_YConvR,piLDR_YConvRWork);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiIDX_RoatorUpDn , ccBwd)) return false ;
                 if(!MoveMotr(miIDX_TRotat , piIDX_TRotatWait)) return false ;
                 if(!LDR_R.MoveMotr(miLDR_YConvR,piLDR_YConvRWork)) return false ;
                 //MoveMotr(miLDR_ZElevR, piLDR_ZElevRPickFrst);
                 Step.iCycle++;
                 return false ;

        case 12: //if(!MoveMotr(miLDR_ZElevR, piLDR_ZElevRPickFrst)) return false ;
                 //MoveMotr(miIDX_XIndxR, piIDX_XIndxRCheck1);
                 if(iRc == 0) {

                     MoveMotr(miIDX_XIndxR , piIDX_XIndxRCheck1);


                 }
                 if(iRc == 1) {
                     if(OM.CmnOptn.iWorkMode == wmHeight){
                         EM_SetErrMsg(eiPRT_Detect , "������������ε� �δ��� 1��° ���̿� ������ �ֽ��ϴ�.");
                         Step.iCycle++;
                         return false ;
                     }
                     MoveMotr(miIDX_XIndxR , piIDX_XIndxRCheck2);
                 }

                 Step.iCycle++;
                 return false ;

        case 13: if(IO_GetX(xIDX_PkgCrashDetect)){ //�ε��� ��Ʈ�Ӹ��� �ִ� �浹 �������� ���̽�.
                     Step.iCycle = 50 ;
                     return false ;
                 }


                 if(!MT_GetStopInpos(miIDX_XIndxR)) return false ;
                 IO_SetY(yIDX_IDXIonizer , true);
                 if(!IO_GetX(xIDX_Detect)) { //���ôµ� ���� �ȵȰ��.
                     MT_GoInc(miIDX_XIndxR , 20.0 , 3);
                     Step.iCycle++ ;
                     return false ;
                 }
                 Step.iCycle++;
                 return false ;

        case 14: if(IO_GetX(xIDX_Detect)) { //������ ������ ��.
                     MT_Stop(miIDX_XIndxR);
                 }
                 if(!MT_GetStopInpos(miIDX_XIndxR))return false ;
                 if(!IO_GetX(xIDX_Detect)) { //���ôµ� ������ ���� �ȵȰ��.
                     Step.iCycle =30 ;
                     return false ;
                 }


                 MT_GoInc(miIDX_XIndxR , -25.0 , 3);
                 Step.iCycle++;
                 return false ;

        case 15: if(!IO_GetX(xIDX_Detect)) { //������ ���� �ȵȰ��.
                     MT_Stop(miIDX_XIndxR);
                 }
                 if(!MT_GetStopInpos(miIDX_XIndxR))return false ;

                 MT_GoIncRun(miIDX_XIndxR , PM.GetValue(miIDX_XIndxR , poIDX_XIndxRGet));
                 MT_GoAbsMan(miLDR_YConvR , PM.GetValue(miLDR_YConvR , pvLDR_YConvRWork)-0.5);
                 Step.iCycle++;
                 return false ;

        case 16: if(!MT_GetStopInpos(miIDX_XIndxR)) return false ;
                 MT_GoIncRun(miLDR_ZElevR, -GET_OFFSET_Z );
                 Step.iCycle++;
                 return false ;

        case 17: if(!MT_GetStopInpos(miLDR_ZElevR)) return false ;
                 IO_SetY(yIDX_IdxVac , true);

                 MoveMotr(miIDX_XIndxR, piIDX_XIndxRRotator, true);
                 LDR_R.MoveMotr(miLDR_YConvR,piLDR_YConvRWork);
                 Step.iCycle++;
                 return false ;

        case 18: if(!MoveMotr(miIDX_XIndxR, piIDX_XIndxRRotator, true)) return false ;
                 if(!LDR_R.MoveMotr(miLDR_YConvR,piLDR_YConvRWork)) return false ; 
                 IO_SetY(yIDX_IdxVac , false ) ;
                 MoveActr(aiIDX_RoatorUpDn , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 19: if(!MoveActr(aiIDX_RoatorUpDn , ccFwd))return false ;
                 if(OM.CmnOptn.iWorkMode == wmNormal)IO_SetY(yIDX_RttPumpOn , true);
                 MoveMotr(miIDX_XIndxR , piIDX_XIndxRWait);
                 Step.iCycle++;
                 return false ;

        case 20: if(!MoveMotr(miIDX_XIndxR , piIDX_XIndxRWait)) return false ;
                 IO_SetY(yIDX_IDXIonizer , false);
                 //if(!OM.DevOptn.bNotUseRotate && DM.ARAY[riSTG].GetCntStat(csWork) % 2){ //Ȧ��  1,3,5

                 //}
                 MoveMotr(miIDX_TRotat , piIDX_TRotatNormal);

                 Step.iCycle++;
                 return false ;

        case 21: if(!MT_GetStopInpos(miIDX_TRotat)) return false ;
                 if(OM.CmnOptn.iWorkMode == wmNormal) {
                     sModID = DM.ARAY[riLDR_R].CHPS[r][c].GetID();
                     DM.ARAY[riLDR_R].SetStat(r,c,csEmpty);
                     DM.ARAY[riLDR_R].CHPS[r][c].SetID("");
                     DM.ARAY[riRTT].SetStat(csCmsLOri);
                     DM.ARAY[riRTT].CHPS[0][0].SetID(sModID);
                     DM.ARAY[riRTT].SetID(sModID);
                 }
                 else {
                     DM.ARAY[riLDR_R].SetStat(r,c,csMask);
                     DM.ARAY[riRTT].SetStat(csCmsLHeight);

                 }
                 Step.iCycle=0;
                 return true ;




        //��������.
        //�δ��� ������ ������ ����...
        case 30: MoveMotr(miIDX_XIndxR , piIDX_XIndxRGetWait);
                 Step.iCycle++;
                 return false ;

        case 31: if(!MoveMotr(miIDX_XIndxR , piIDX_XIndxRGetWait)) return false ;
                 IO_SetY(yIDX_IDXIonizer, false);

                 //������ ������ ó��.
                 //DM.ARAY[riLDR_R].SetStat(r,c,csEmpty);
                 EM_SetErrMsg(eiPKG_Dispr, "����δ��� ���Կ� ������ �����ϴ�.");
                 Step.iCycle = 0 ;
                 return true ;

        //��������.
        //�ε��� �浹 ���� ���� �۵�.
        case 50: MT_EmgStop(miIDX_XIndxR);
                 EM_SetErrMsg(eiPRT_Crash, "�δ����Խÿ� �ε��� �浹 ������ �����Ǿ����ϴ�.");
                 Step.iCycle++;
                 return false ;

        case 51: if(!MT_GetStopInpos(miIDX_XIndxR)) return false;
                 MoveMotr(miIDX_XIndxR , piIDX_XIndxRWait);
                 Step.iCycle=0;
                 return false ;




    }
}



bool CIndex::CyclePlace()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
        //Step.iCycle = 0;
        //return true ;
    }

    int r,c ;
    FindChip(r,c,csMask);
    int dPlaceStt ;
    int dPlace    ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //if(IO_GetX(xIDX_Detect)) {
                 //     EM_SetErrMsg(eiPRT_Crash, (m_sPartName + "CMOS ���簡 �ֽ��ϴ�. ���縦 �������ּ���.").c_str());
                 //     Step.iCycle = 0 ;
                 //     return true ;
                 //}

                 if(MT_GetCmdPos(miIDX_XIndxR) == GetMotrPos(miIDX_XIndxR , piIDX_XIndxRWait)){
                     Step.iCycle = 20 ;
                     return false ;
                 }

                 if(AT_Complete(aiIDX_RoatorUpDn , ccBwd)){
                     MoveMotr(miIDX_XIndxR , piIDX_XIndxRWait);
                     Step.iCycle = 20 ;
                     return false ;
                 }

                 EM_SetErrMsg(eiPRT_Crash, "�ε����� �����ġ�� �ƴ� �������Ͱ� �� �Ǿ� �ֽ��ϴ�.");
                 Step.iCycle = 0 ;
                 return true ;

        case 20: if(!MoveMotr(miIDX_XIndxR , piIDX_XIndxRWait)) return false ;
                 MoveActr(aiIDX_RoatorUpDn , ccFwd);
                 MoveMotr(miIDX_TRotat , piIDX_TRotatWait);
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveActr(aiIDX_RoatorUpDn , ccFwd)) return false ;
                 if(!MoveMotr(miIDX_TRotat , piIDX_TRotatWait)) return false ;
                 MoveMotr(miIDX_XIndxR , piIDX_XIndxRRotator);
                 Step.iCycle++;
                 return false ;

        case 22: if(!MoveMotr(miIDX_XIndxR , piIDX_XIndxRRotator)) return false ;
                 IO_SetY(yIDX_RttPumpOn , false);
                 Step.iCycle++;
                 return false ;

        case 23:
                 MoveActr(aiIDX_RoatorUpDn, ccBwd);
                 Step.iCycle++;
                 return false ;

        case 24: if(!MoveActr(aiIDX_RoatorUpDn, ccBwd)) return false ;
                 IO_SetY(yIDX_IdxVac , true);


                 //Loader Position�� Empty�� �������� �ؼ�.
                 //�������� ��� ������ ���� ���.
                 //MoveMotr(miIDX_XIndxR , piIDX_XIndxRGetWait);
                 Step.iCycle++;
                 return false ;

        case 25: if(!MT_GetStopInpos(miLDR_ZElevR)) return false ;
                 if(!MT_GetStopInpos(miIDX_XIndxR)) return false ;

                 MoveMotr(miIDX_XIndxR , piIDX_XIndxRCheck1);

                 Step.iCycle++ ;
                 return false ;

        case 26: if(!MT_GetStopInpos(miIDX_XIndxR)) return false ;
                 MT_GoIncSlow(miIDX_XIndxR , PM.GetValue(miIDX_XIndxR , poIDX_XIndxRGet)+1.0);
                 MT_GoAbsRun(miLDR_YConvR , PM.GetValue(miLDR_YConvR , pvLDR_YConvRWork)-1.0);
                 Step.iCycle++;
                 return false ;

        case 27: if(!MT_GetStopInpos(miIDX_XIndxR)) return false ;
                 if(!MT_GetStopInpos(miLDR_YConvR)) return false ;

                 MT_GoIncRun(miLDR_ZElevR, GET_OFFSET_Z );
                 IO_SetY(yIDX_IdxVac , false);
                 Step.iCycle++;
                 return false ;

        case 28: if(!MT_GetStopInpos(miLDR_ZElevR)) return false ;
                 MT_GoAbsRun(miLDR_YConvR , PM.GetValue(miLDR_YConvR , pvLDR_YConvRWork));
                 MoveMotr(miIDX_XIndxR , piIDX_XIndxRGetWait);

        case 29: if(MoveMotr(miIDX_XIndxR , piIDX_XIndxRGetWait)) return false ;


                 DM.ARAY[riRTT  ].SetStat(csNone);
                 DM.ARAY[riLDR_R].SetStat(r,c,csWork);

                 Step.iCycle = 0;
                 return true ;

    }
}

//�Ⱦ�.
bool CIndex::CycleRotator(void)//sss
{
/*
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
        //Step.iCycle = 0;
        //return true ;
    }

    int r ;
    EN_ARAY_ID riULD ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: MoveActr(aiIDX_RoatorUpDn, ccBwd);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiIDX_RoatorUpDn, ccBwd)) return false;
                 MoveMotr(miIDX_XIndxR, piIDX_XIndxRRotator);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miIDX_XIndxR, piIDX_XIndxRRotator)) return false;
                 if(IO_GetX(xIDX_Detect)){
                     MoveActr(aiIDX_RoatorUpDn, ccFwd);
                     Step.iCycle = 15;
                     return false;
                 }
                 //Error ID �ٲ�.
                 //EM_SetErrMsg(eiIDX_NotExistCMOS, (m_sPartName + "Index Transfer �κп� CMOS ���簡 �����ϴ�.").c_str());
                 Step.iCycle = 0;
                 return true ;

        case 15: if(!MoveActr(aiIDX_RoatorUpDn, ccFwd)) return false;
                 MoveMotr(miIDX_XIndxR, piIDX_TRotatWait);
                 Step.iCycle++;
                 return false;

        case 16: if(!MoveMotr(miIDX_XIndxR, piIDX_TRotatWait)) return false;
                 MoveMotr(miIDX_TRotat, piIDX_TRotatRotate);
                 Step.iCycle++;
                 return false;

        case 17: if(!MoveMotr(miIDX_TRotat, piIDX_TRotatRotate)) return false;
                 DM.ShiftArrayData(riRTT, riIDX);
                 Step.iCycle = 0;
                 return false;
    }
*/
}

//---------------------------------------------------------------------------
bool CIndex::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miIDX_XIndxR){
        if(!MT_GetStop(miLDR_ZElevR)){
            sMsg = "���� �δ� Z���� �̵����Դϴ�.";
            bRet = false;
        }
        else if(MT_GetCmdPos(miIDX_XIndxR) > PM.GetValue(miIDX_XIndxR,pvIDX_XIndxRRotator)){
            if(!AT_Complete(aiIDX_RoatorUpDn,ccBwd)){
                sMsg = "�ε����� �����̼������Ǻ��� ���ʿ� �ִµ� �������Ͱ� ������Դϴ�.";
                bRet = false;
            }
        }
        else if(MT_GetCmdPos(miRTL_YTrspr) < (PM.GetValue(miRTL_YTrspr , pvRTL_YTrsfCheck)+100) && MT_GetCmdPos(miRTL_ZTrsfr) > PM.GetValue(miRTL_ZTrsfr , pvRTL_ZTrsfrWait)){
            sMsg = "Ʈ�����۰� Wait������ ���� ���� ��ġ �Դϴ�.";
            bRet = false;
        }
    }

    else if(_iMotr == miIDX_TRotat){
        if(MT_GetCmdPos(miIDX_XIndxR) != PM.GetValue(miIDX_XIndxR,pvIDX_XIndxRWait)){
            if(!AT_Complete(aiIDX_RoatorUpDn,ccBwd)){
                sMsg = "�ε����� Wait��ġ�� �ƴմϴ�.";
                bRet = false;
            }
        }
    }

    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iSeq)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CIndex::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

//    bool bPushUpDnFwd = AT_GetCmd(Add.aiPushUpDn) == ccFwd;
    if(_iActr == aiIDX_RoatorUpDn){
        if(_bFwd == ccBwd) {
            //if(IO_GetX(xIDX_Detect)){ sMsg = AnsiString("CMOS ���� ���� ���� ��!"); bRet = false ;}
        }
    }

    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

bool CIndex::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        return MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }

    return false ;
}

void CIndex::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // ���͸� �����϶� ���� �Լ�.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CIndex::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CIndex::SetLastCmd()
{
    //m_dLastIdxPos = MT_GetTrgPos(miIDX_TRotat);
    return ;

}

double CIndex::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return dLastIdxPos ;
}

bool CIndex::CheckMoved()
{
//    if(m_dLastIdxPos != MT_GetCmdPos(miIDX_XIndxR)) return true  ; //����Ʈ�� �ʿ� ����.
//    else                                               return false ;

}

bool CIndex::CheckStop()
{
    if(!MT_GetStop(miIDX_XIndxR)) return false ;
    if(!MT_GetStop(miIDX_TRotat)) return false ;

    if(!AT_Done(aiIDX_RoatorUpDn)) return false ;

    return true ;
}

void CIndex::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}





