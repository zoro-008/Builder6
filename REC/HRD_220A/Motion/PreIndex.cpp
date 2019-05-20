//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "PreIndex.h"
//#include "RunThread.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CPreIndex PRI_F;
CPreIndex PRI_R;

CPreIndex::CPreIndex(void)
{

}

CPreIndex::~CPreIndex (void)
{

}

void CPreIndex::Init(bool _bFront )
{
    if(_bFront){
        m_sPartName          = "Front Pre Index";
        Add.iPartId          = piIDX_F          ;

        Add.riPRI            = riPRI_F          ;
        Add.miPRI_XIndx      = miPRI_XIndxF     ;
        Add.pvPRI_XIndxWait  = pvPRI_XIndxFWait ;
        Add.pvPRI_XIndxGet   = pvPRI_XIndxFGet  ;
        Add.pvPRI_XIndxOut   = pvPRI_XIndxFOut  ;
        Add.pvPRI_XIndxOutChckOn   = pvPRI_XIndxFOutChckOn  ;
        Add.pvPRI_XIndxOutChckOff  = pvPRI_XIndxFOutChckOff ;

        Add.piPRI_XIndxWait  = piPRI_XIndxFWait ;
        Add.piPRI_XIndxGet   = piPRI_XIndxFGet  ;
        Add.piPRI_XIndxOut   = piPRI_XIndxFOut  ;
        Add.piPRI_XIndxOutChckOn  = piPRI_XIndxFOutChckOn  ;
        Add.piPRI_XIndxOutChckOff = piPRI_XIndxFOutChckOff ;

        Add.xPRI_Detect1     = xPRI_FDetect1    ;
        Add.xPRI_Detect2     = xPRI_FDetect2    ;
        Add.xPRI_IndxNotOvLd = xPRI_FIndxNotOvLd;
        Add.aiPRI_IndxUpDn   = aiPRI_FIndxUpDn  ;
        Add.yIDX_Ionizer     = yIDX_FIonizer    ;

        Add.LDR              = &LDR_F           ;
        Add.riLDRTop         = riLDR_FTop       ;
        Add.riLDRBtm         = riLDR_FBtm       ;

        Add.riIDX            = riIDX_F          ;
    }
    else {
        m_sPartName          = "Rear Pre Index" ;
        Add.iPartId          = piIDX_R          ;
        Add.riPRI            = riPRI_R          ;
        Add.miPRI_XIndx      = miPRI_XIndxR     ;
        Add.pvPRI_XIndxWait  = pvPRI_XIndxRWait ;
        Add.pvPRI_XIndxGet   = pvPRI_XIndxRGet  ;
        Add.pvPRI_XIndxOut   = pvPRI_XIndxROut  ;
        Add.pvPRI_XIndxOutChckOn  = pvPRI_XIndxROutChckOn  ;
        Add.pvPRI_XIndxOutChckOff = pvPRI_XIndxROutChckOff ;

        Add.piPRI_XIndxWait  = piPRI_XIndxRWait ;
        Add.piPRI_XIndxGet   = piPRI_XIndxRGet  ;
        Add.piPRI_XIndxOut   = piPRI_XIndxROut  ;
        Add.piPRI_XIndxOutChckOn  = piPRI_XIndxROutChckOn  ;
        Add.piPRI_XIndxOutChckOff = piPRI_XIndxROutChckOff ;

        Add.xPRI_Detect1     = xPRI_RDetect1    ;
        Add.xPRI_Detect2     = xPRI_RDetect2    ;
        Add.xPRI_IndxNotOvLd = xPRI_RIndxNotOvLd;
        Add.aiPRI_IndxUpDn   = aiPRI_RIndxUpDn  ;
        Add.yIDX_Ionizer     = yIDX_RIonizer    ;

        Add.LDR              = &LDR_R           ;
        Add.riLDRTop         = riLDR_RTop       ;
        Add.riLDRBtm         = riLDR_RBtm       ;

        Add.riIDX            = riIDX_R          ;

    }
    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

}

void CPreIndex::Close()
{
    Load(false);
}

void CPreIndex::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CPreIndex::FindChip(int &r , EN_ARAY_ID &id)
{
    return false ;
}

double CPreIndex::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dWorkStt ; //�۾��ؾ��� �ް����� �� �Ʒ� ���� ��ġ.
    double dWork    ; //���� �۾��ؾ��� ������.

    if(_iMotr == Add.miPRI_XIndx){
             if(_iPstnId ==  Add.piPRI_XIndxWait      ) dPos = PM.GetValue (_iMotr , Add.pvPRI_XIndxWait      );
        else if(_iPstnId ==  Add.piPRI_XIndxGet       ) dPos = PM.GetValue (_iMotr , Add.pvPRI_XIndxGet       );
        else if(_iPstnId ==  Add.piPRI_XIndxOut       ) dPos = PM.GetValue (_iMotr , Add.pvPRI_XIndxOut       );
        else if(_iPstnId ==  Add.piPRI_XIndxOutChckOn ) dPos = PM.GetValue (_iMotr , Add.pvPRI_XIndxOutChckOn );
        else if(_iPstnId ==  Add.piPRI_XIndxOutChckOff) dPos = PM.GetValue (_iMotr , Add.pvPRI_XIndxOutChckOff);
        else                                            dPos = MT_GetCmdPos(_iMotr                       );
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CPreIndex::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}
void CPreIndex::Update()
{
    if(IO_GetXDn(Add.xPRI_IndxNotOvLd)) {
        //���� ���� �ö� ��.
        m_bOverload = true ;
        EM_SetErrMsg(eiPRT_OverLoad , (m_sPartName + "PreIndex���� �����ε尡 �߻��Ǿ����ϴ�.").c_str());
        MT_EmgStop(Add.miPRI_XIndx);
        MoveActr(Add.aiPRI_IndxUpDn , ccFwd);
    }


}

bool CPreIndex::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CPreIndex::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPreIndex::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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

        case 10: MoveActr(Add.aiPRI_IndxUpDn , ccFwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(Add.aiPRI_IndxUpDn , ccFwd)) return false ;
                 MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxWait);
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPreIndex::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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

        case 10: MoveActr(Add.aiPRI_IndxUpDn , ccFwd);
                 IO_SetY(Add.yIDX_Ionizer , false);
                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveActr(Add.aiPRI_IndxUpDn , ccFwd)) return false ;
                 MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxWait);
                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxWait)) return false ;
                 Step.iToStop = 0 ;
    }
}

bool CPreIndex::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
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

        bool isLDRWorkPos = Add.LDR -> IsWorkPos();

        //�������� �����ε��� �����̳�....
        bool isCycleWork = (DM.ARAY[Add.riLDRTop].GetCntStat  (csUnkwn) || DM.ARAY[Add.riLDRBtm].GetCntStat (csUnkwn)) &&
                            DM.ARAY[Add.riPRI   ].CheckAllStat(csNone ) && !OM.CmnOptn.bLoadingStop &&
                            isLDRWorkPos && Add.LDR -> GetSeqStep() == CLoader::scIdle ;

        bool isCycleEnd  = Add.LDR -> GetStat().bWorkEnd && DM.ARAY[Add.riPRI].CheckAllStat(csNone) ;

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                        Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                               return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CPreIndex::CycleHome()     //sun DLL direct access.
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

        case 10: MoveActr(Add.aiPRI_IndxUpDn , ccFwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(Add.aiPRI_IndxUpDn , ccFwd)) return false ;
                 if(!IO_GetX(Add.xPRI_IndxNotOvLd)) {
                     EM_SetErrMsg(eiPRT_OverLoad , (m_sPartName + "PreIndex���� �����ε尡 �߻��Ǿ����ϴ�.").c_str());
                     return false ;
                 }

                 MT_DoHome(Add.miPRI_XIndx);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(Add.miPRI_XIndx)) return false ;
                 MT_GoAbsMan(Add.miPRI_XIndx , PM.GetValue(Add.miPRI_XIndx , Add.pvPRI_XIndxWait ));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(Add.miPRI_XIndx , PM.GetValue(Add.miPRI_XIndx , Add.pvPRI_XIndxWait ))) return false ;
                 SetLastCmd();
                 Step.iHome = 0;
                 return true ;

    }
}

bool CPreIndex::CycleWork(void)
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

    int r ;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: MoveActr(Add.aiPRI_IndxUpDn , ccFwd);
                 m_bOverload = false ;
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(Add.aiPRI_IndxUpDn , ccFwd)) return false ;
                 MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxGet);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxGet)) return false ;
                 IO_SetY(Add.yIDX_Ionizer, true);
                 MoveActr(Add.aiPRI_IndxUpDn , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveActr(Add.aiPRI_IndxUpDn , ccBwd)) return false ;
                 MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxOutChckOn);
                 m_bPassedDetect1 = false ;
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxOutChckOn)) return false ;
                 if(IO_GetX(Add.xPRI_Detect1)){
                     m_bPassedDetect1 = true ;
                 }
                 else {
                     m_bPassedDetect1 = false ;
                 }

                 MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxOutChckOff);
                 Step.iCycle++;
                 return false ;

        case 15: if(!MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxOutChckOff)) return false ;
                 if(m_bPassedDetect1){
                     if(IO_GetX(Add.xPRI_Detect1)){
                         EM_SetErrMsg(eiPRT_BoatMiss , (m_sPartName + " ���Ժ� ���� ���� ������ �����ε����� ������ ���ƽ��ϴ�.").c_str());
                         MoveActr(Add.aiPRI_IndxUpDn , ccFwd);
                         Step.iCycle=0 ;
                         return false ;
                     }
                 }
                 MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxOut);
                 Step.iCycle++;
                 return false ;

        case 16: if(!MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxOut)) return false ;


                 //������� �Դµ� �����ε� ����·� �ü��� �־�
                 //�����ε� ���� ó����.
                 if(m_bOverload){ //if(!IO_GetX(Add.xPRI_IndxNotOvLd)) {
                     Step.iCycle=0;
                     return true ;

                 }

                 if(m_bPassedDetect1){
                     if(!IO_GetX(Add.xPRI_Detect2)){
                         EM_SetErrMsg(eiPRT_BoatMiss , (m_sPartName + " 2�� ������ ������ ���� ���� �ʽ��ϴ�.").c_str());
                         MoveActr(Add.aiPRI_IndxUpDn , ccFwd);
                         Step.iCycle=0 ;
                         return false ;
                     }
                 }

                 GetData(m_bPassedDetect1);


                 MoveActr(Add.aiPRI_IndxUpDn , ccFwd);
                 Step.iCycle++;
                 return false ;

                     /*//�����½ÿ� �̹� �δ��� ������ ��Ʈ�ø��� �־����.
                     //������ �̵�.
                     if(Add.LDR->FindChip(r , riLDR)){
                         DM.ARAY[riLDR].SetStat(r,0,csEmpty);
                         DM.ARAY[Add.riPRI].SetStat(csUnkwn);
                         DM.ARAY[Add.riPRI].SetLotNo(DM.ARAY[riLDR].GetLotNo()) ;
                         //sTemp = sTemp.sprintf("%2d",(r + (riLDR == Add.riLDRBtm ? 0 : OM.DevInfo.iMgzSlotCnt)));
                         //DM.ARAY[Add.riPRI].SetID(sTemp);


                         String sBoatSerial ;
                         String sPkgSerial  ;
                         if(Add.iPartId == piIDX_F) {
                             //20160922 ������ �����½ÿ� ���� ���� ��Ʈ���� ���� �����Ͱ� �־�� �Ǽ� �����Ѵ�.


                             //�����½ÿ� �ް��� ���Կ� ���̵�� �ٿ�����.
                             sBoatSerial = DM.ARAY[riLDR].CHPS[r][0].GetID();
                             //�װ� ������ �����ε��� ���̵�� ����.
                             DM.ARAY[Add.riPRI].SetID(sBoatSerial);

                             //���� Empty �ʱ�ȭ �ϰ�.
                             DM.ARAY[Add.riPRI].SetStat(csEmpty);
                             for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
                                 //��ü �����͸� �� �m� �ش� �ް��� ����(r) ���� ������ ���� �����.
                                 if(g_LotData.GetCellByColTitle("FrontMagazineSlotNo",i) == String(r+1)){
                                     sPkgSerial = g_LotData.GetCellByColTitle("FrontBoatSlotNo",i);
                                     DM.ARAY[Add.riPRI].CHPS[0][OM.DevInfo.iBoatColCnt - i - 1].SetID(sPkgSerial);
                                 }
                             }


                         }
                         else {

                             //�����½ÿ� �ް��� ���Կ� ���̵�� �ٿ�����.
                             sBoatSerial = DM.ARAY[riLDR].CHPS[r][0].GetID();
                             //�װ� ������ �����ε��� ���̵�� ����.
                             DM.ARAY[Add.riPRI].SetID(sBoatSerial);

                             //���� Empty �ʱ�ȭ �ϰ�.
                             DM.ARAY[Add.riPRI].SetStat(csEmpty);
                             for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
                                 //��ü �����͸� �� �m� �ش� �ް��� ����(r) ���� ������ ���� �����.
                                 if(g_LotData.GetCellByColTitle("RearMagazineSlotNo",i) == String(r+1)){
                                     sPkgSerial = g_LotData.GetCellByColTitle("RearBoatSlotNo",i);
                                     DM.ARAY[Add.riPRI].CHPS[0][OM.DevInfo.iBoatColCnt - i - 1].SetID(sPkgSerial);
                                 }
                             }
                         }
                     }
                 }  */



        case 17: if(!MoveActr(Add.aiPRI_IndxUpDn , ccFwd)) return false ;
                 IO_SetY(Add.yIDX_Ionizer , false);
                 MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxWait);
                 Step.iCycle++;
                 return false ;

        case 18: if(!MoveMotr(Add.miPRI_XIndx , Add.piPRI_XIndxWait)) return false ;
                 Step.iCycle = 0;
                 return true ;
    }

}



//---------------------------------------------------------------------------
bool CPreIndex::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(Add.iPartId == piPRI_F){
    }



    if(_iMotr == Add.miPRI_XIndx){
        if(_iPstnId == Add.piPRI_XIndxGet){
            if(!Add.LDR -> IsWorkPos()) {
                sMsg = "�δ��� WorkPosion�� �ƴմϴ�.";
                bRet = true ;
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
bool CPreIndex::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == Add.aiPRI_IndxUpDn){
        //if(_bFwd == ccFwd) {
        //    if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray ���� ������!"); bRet = false ;}
        //}
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

bool CPreIndex::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
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







void CPreIndex::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // ���͸� �����϶� ���� �Լ�.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CPreIndex::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CPreIndex::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(Add.miPRI_XIndx);
    return ; //����Ʈ�� �ʿ� ����.

}

bool CPreIndex::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(Add.miPRI_XIndx)) return true  ; //����Ʈ�� �ʿ� ����.
    else                                               return false ;

}

bool CPreIndex::CheckStop()
{
    if(!MT_GetStop(Add.miPRI_XIndx)) return false ;

    if(!AT_Done(Add.aiPRI_IndxUpDn)) return false ;

    return true ;
}

void CPreIndex::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos"  , m_dLastIdxPos  );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos"  , m_dLastIdxPos  );
    }
}


//�ó������� ���� ��Ʈ�Ͽ��� �׽�Ʈ �����ϰ�.. ��.
void CPreIndex::GetData(bool _bPassed)
{
    EN_ARAY_ID riLDR ;
    int r ;

    String sBoatSerial ;
    String sPkgSerial  ;
    String sBoatSlot   ;
    int    iBoatSlot   ;
    int    iMgzSlot    ;

    if(_bPassed) { //���� ���� ����.
        if(!Add.LDR->FindChip(r , riLDR)) return ;
        DM.ARAY[riLDR].SetStat(r,0,csEmpty);
        DM.ARAY[Add.riPRI].SetLotNo(DM.ARAY[riLDR].GetLotNo()) ;
        //sTemp = sTemp.sprintf("%2d",(r + (riLDR == Add.riLDRBtm ? 0 : OM.DevInfo.iMgzSlotCnt)));
        //DM.ARAY[Add.riPRI].SetID(sTemp);

        if(Add.iPartId == piIDX_F) {
            //�����½ÿ� �ް��� ���Կ� ���̵�� �ٿ�����.
            sBoatSerial = DM.ARAY[riLDR].CHPS[r][0].GetID();
            //�װ� ������ �����ε��� ���̵�� ����.
            DM.ARAY[Add.riPRI].SetID(sBoatSerial);

            //���� Empty �ʱ�ȭ �ϰ�.
            DM.ARAY[Add.riPRI].SetStat(csEmpty);
            for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
                //��ü �����͸� �� �m� �ش� �ް��� ����(r) ���� ������ ���� �����.
                //�ش� �ް����� �������� ���.
                iMgzSlot = g_LotData.GetCellByColTitle("FrontMagazineSlotNo",i).ToIntDef(-1) ;
                if(riLDR == riLDR_FTop){//��Ž�� �ƴ� ž�� ��� 11~20������ �ް��� ������ �Ǿ� �ִ�.
                    iMgzSlot -= OM.DevInfo.iMgzSlotCnt ;
                    if(iMgzSlot <= 0) continue ;
                    if(iMgzSlot >  OM.DevInfo.iMgzSlotCnt) continue ;
                }
                if(iMgzSlot == OM.DevInfo.iMgzSlotCnt - r){
                    //��Ʈ ���� �ѹ� �̾Ƴ���.
                    sBoatSlot  = g_LotData.GetCellByColTitle("FrontBoatSlotNo",i);
                    iBoatSlot  = sBoatSlot.ToIntDef(-999);
                    //�ش� PKG ID �̾� ����.
                    sPkgSerial = g_LotData.GetCellByColTitle("FrontSerialNo",i);

                    //�ش���� �ȿ� �ִ��� üũ.
                    if(iBoatSlot > 0 && iBoatSlot <= OM.DevInfo.iBoatColCnt){
                        DM.ARAY[Add.riPRI].CHPS[0][OM.DevInfo.iBoatColCnt - iBoatSlot].SetID(sPkgSerial);
                        DM.ARAY[Add.riPRI].SetStat(0 , OM.DevInfo.iBoatColCnt - iBoatSlot, csUnkwn);
                    }
                }
            }
        }
        else {
            //�����½ÿ� �ް��� ���Կ� ���̵�� �ٿ�����.
            sBoatSerial = DM.ARAY[riLDR].CHPS[r][0].GetID();
            //�װ� ������ �����ε��� ���̵�� ����.
            DM.ARAY[Add.riPRI].SetID(sBoatSerial);
            //���� Empty �ʱ�ȭ �ϰ�.
            DM.ARAY[Add.riPRI].SetStat(csEmpty);
            for(int i = 1 ; i < g_LotData.GetMaxRow() ; i++) {
                //��ü �����͸� �� �m� �ش� �ް��� ����(r) ���� ������ ���� �����.
                //�ش� �ް����� �������� ���.
                iMgzSlot = g_LotData.GetCellByColTitle("RearMagazineSlotNo",i).ToIntDef(-1) ;
                if(riLDR == riLDR_RTop){//��Ž�� �ƴ� ž�� ��� 11~20������ �ް��� ������ �Ǿ� �ִ�.
                    iMgzSlot -= OM.DevInfo.iMgzSlotCnt ;
                    if(iMgzSlot <= 0) continue ;
                    if(iMgzSlot >  OM.DevInfo.iMgzSlotCnt) continue ;
                }
                if(iMgzSlot == OM.DevInfo.iMgzSlotCnt - r){
                    //��Ʈ ���� �ѹ� �̾Ƴ���.
                    sBoatSlot  = g_LotData.GetCellByColTitle("RearBoatSlotNo",i);
                    iBoatSlot  = sBoatSlot.ToIntDef(-999);
                    //�ش� PKG ID �̾� ����.
                    sPkgSerial = g_LotData.GetCellByColTitle("RearSerialNo",i);

                    //�ش���� �ȿ� �ִ��� üũ.
                    if(iBoatSlot > 0 && iBoatSlot <= OM.DevInfo.iBoatColCnt){
                        DM.ARAY[Add.riPRI].CHPS[0][OM.DevInfo.iBoatColCnt - iBoatSlot].SetID(sPkgSerial);
                        DM.ARAY[Add.riPRI].SetStat(0 , OM.DevInfo.iBoatColCnt - iBoatSlot, csUnkwn);
                    }
                }
            }
        }
    }

    else { //�������� ����... �󽽷��� ���.
        if(Add.LDR->FindChip(r , riLDR)){
            DM.ARAY[riLDR].SetStat(r,0,csEmpty);
        }
    }


}





