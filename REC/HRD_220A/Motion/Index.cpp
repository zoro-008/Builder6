//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Index.h"
//#include "RunThread.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "SpcUnit.h"
#include "UserIni.h"
#include "UserFile.h"
#include "Dispensor.h"
//#include "Stage.h"
//#include "Picker.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CIndex IDX_F;
CIndex IDX_R;

CIndex::CIndex(void)
{

}

CIndex::~CIndex (void)
{

}

void CIndex::Init(bool _bFront )
{
    if(_bFront){
        m_sPartName               = "Front Index"         ;
        Add.iPartId               = piIDX_F               ;
        Add.riIDX                 = riIDX_F               ;
        Add.miIDX_XIndx           = miIDX_XIndxF          ;
        Add.pvIDX_XIndxWait       = pvIDX_XIndxFWait      ;
        Add.pvIDX_XIndxGet        = pvIDX_XIndxFGet       ;
        Add.pvIDX_XIndxWorkStart  = pvIDX_XIndxFWorkStart ;
        Add.pvIDX_XIndxOut        = pvIDX_XIndxFOut       ;
        Add.piIDX_XIndxWait       = piIDX_XIndxFWait      ;
        Add.piIDX_XIndxGet        = piIDX_XIndxFGet       ;
        Add.piIDX_XIndxWorkStart  = piIDX_XIndxFWorkStart ;
        Add.piIDX_XIndxOut        = piIDX_XIndxFOut       ;
        Add.piIDX_XIndxWork       = piIDX_XIndxFWork      ;
        Add.xIDX_WrkDetect3       = xIDX_FWrkDetect3      ;
        Add.xIDX_Detect4          = xIDX_FDetect4         ;
        Add.xIDX_IndxNotOvLd      = xIDX_FIndxNotOvLd     ;
        Add.aiIDX_IndxUpDn        = aiIDX_FIndxUpDn       ;
        Add.csAfterWork           = csEmpty               ;

        Add.riPSI                 = riPSI_F               ;

        Add.ULD                   =&ULD_F                 ;
        Add.riULDTop              = riULD_FTop            ;
        Add.riULDBtm              = riULD_FBtm            ;
        Add.aiPushLtRt            = aiULD_FPushLtRt       ;
        Add.aiPushUpDn            = aiULD_FPushUpDn       ;
        Add.xULD_Detect5          = xULD_FDetect5         ;


        Add.PRI                   =&PRI_F                 ;
        Add.riPRI                 = riPRI_F               ;
        //����Ʈ������ Picker�� Z���� ����.
        Add.miTool                = miPCK_ZPickr          ;
        Add.pvToolWait            = pvPCK_ZPickrWait      ;

    }
    else {
        m_sPartName               = "Rear Index"          ;
        Add.iPartId               = piIDX_R               ;
        Add.riIDX                 = riIDX_R               ;
        Add.miIDX_XIndx           = miIDX_XIndxR          ;
        Add.pvIDX_XIndxWait       = pvIDX_XIndxRWait      ;
        Add.pvIDX_XIndxGet        = pvIDX_XIndxRGet       ;
        Add.pvIDX_XIndxWorkStart  = pvIDX_XIndxRWorkStart ;
        Add.pvIDX_XIndxOut        = pvIDX_XIndxROut       ;
        Add.piIDX_XIndxWait       = piIDX_XIndxRWait      ;
        Add.piIDX_XIndxGet        = piIDX_XIndxRGet       ;
        Add.piIDX_XIndxWorkStart  = piIDX_XIndxRWorkStart ;
        Add.piIDX_XIndxOut        = piIDX_XIndxROut       ;
        Add.piIDX_XIndxWork       = piIDX_XIndxRWork      ;
        Add.xIDX_WrkDetect3       = xIDX_RWrkDetect3      ;
        Add.xIDX_Detect4          = xIDX_RDetect4         ;
        Add.xULD_Detect5          = xULD_FDetect5         ;
        Add.xIDX_IndxNotOvLd      = xIDX_RIndxNotOvLd     ;
        Add.aiIDX_IndxUpDn        = aiIDX_RIndxUpDn       ;
        Add.csAfterWork           = csWork                ;

        Add.riPSI                 = riPSI_R               ;

        Add.ULD                   =&ULD_R                 ;
        Add.riULDTop              = riULD_RTop            ;
        Add.riULDBtm              = riULD_RBtm            ;
        Add.aiPushLtRt            = aiULD_RPushLtRt       ;
        Add.aiPushUpDn            = aiULD_RPushUpDn       ;
        Add.xULD_Detect5          = xULD_RDetect5         ;

        Add.PRI                   =&PRI_R                 ;
        Add.riPRI                 = riPRI_R               ;

        //������� Stage�� Z���� ����.
        Add.miTool                = miSTG_ZStage          ;
        Add.pvToolWait            = pvSTG_ZStagWait       ;
    }

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


/*
bool CIndex::FindChip(int &c , EN_ARAY_ID &id)
{
    //int iWorkC = DM.ARAY[Add.riIDX].FindFrstCol(Add.csAfterWork) ;
    //int iWaitC = DM.ARAY[Add.riIDX].FindFrstCol(Add.csWait     ) ;
    //int iC     = iWorkC > iWaitC ? iWorkC

    int iSkipC = DM.ARAY[Add.riIDX].FindFrstCol(csSkip         ) ;
    int iWorkC = DM.ARAY[Add.riIDX].FindFrstCol(Add.csAfterWork) ;

    int iC ;
    if(iSkipC != -1 && iWorkC != -1 ){
        iC = iSkipC < iWorkC ? iSkipC : iWorkC ;
    }
    else if( iSkipC != -1){
        iC = iSkipC ;
    }
    else if( iWorkC != -1){
        iC = iWorkC ;
    }
    else {
        iC = -1 ;
    }


    if(-1 != iC){
        id = Add.riIDX ;
        c  = iC ;
        return true ;
    }
    else {
        //��Ʈ�� ������.
        if(DM.ARAY[Add.riIDX].CheckAllStat(csNone)) {
            id = Add.riIDX ;
            c  = -1        ;
            return false ;
        }
        //��Ʈ�� ������
        else {
            id = Add.riIDX ;
            c  = OM.DevInfo.iBoatColCnt ;
            return true ;
        }
    }
}*/

bool CIndex::FindChip(int &c , EN_ARAY_ID &id)
{
    //int iWorkC = DM.ARAY[Add.riIDX].FindFrstCol(Add.csAfterWork) ;
    //int iWaitC = DM.ARAY[Add.riIDX].FindFrstCol(Add.csWait     ) ;
    //int iC     = iWorkC > iWaitC ? iWorkC

    int iWaitC   = DM.ARAY[Add.riIDX].FindLastCol(csWait ) ;
    int iUnknwnC = DM.ARAY[Add.riIDX].FindLastCol(csUnkwn) ;

    int iC ;
    if(iWaitC != -1 && iUnknwnC != -1 ){
        iC = iWaitC > iUnknwnC ? iWaitC : iUnknwnC ;
    }
    else if( iWaitC != -1){
        iC = iWaitC ;
    }
    else if( iUnknwnC != -1){
        iC = iUnknwnC ;
    }
    else {
        iC = -1 ;
    }


    id = Add.riIDX ;
    c  = iC ;
    return true ;
}


double CIndex::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dWorkStt ; //�۾��ؾ��� �ް����� �� �Ʒ� ���� ��ġ.

    int c ;
    EN_ARAY_ID _iAray;
    FindChip(c , _iAray)    ; //���� �۾��ؾ��� ������.

    double dWork = PM.GetValue (_iMotr , Add.pvIDX_XIndxWorkStart) + (OM.DevInfo.iBoatColCnt-c-1) * OM.DevInfo.dBoatColPitch ;

    if(_iMotr == Add.miIDX_XIndx){
             if(_iPstnId ==  Add.piIDX_XIndxWait       ) dPos = PM.GetValue (_iMotr , Add.pvIDX_XIndxWait     );
        else if(_iPstnId ==  Add.piIDX_XIndxGet        ) dPos = PM.GetValue (_iMotr , Add.pvIDX_XIndxGet      );
        else if(_iPstnId ==  Add.piIDX_XIndxWorkStart  ) dPos = PM.GetValue (_iMotr , Add.pvIDX_XIndxWorkStart);
        else if(_iPstnId ==  Add.piIDX_XIndxOut        ) dPos = PM.GetValue (_iMotr , Add.pvIDX_XIndxOut      );
        else if(_iPstnId ==  Add.piIDX_XIndxWork       ) dPos = dWork                                          ;
        else                                             dPos = MT_GetCmdPos(_iMotr                           );
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
    if(IO_GetXDn(Add.xIDX_IndxNotOvLd)) {
        EM_SetErrMsg(eiPRT_OverLoad , (m_sPartName + " Index���� �����ε尡 �߻��Ǿ����ϴ�.").c_str());
        MT_EmgStop(Add.miIDX_XIndx);
        MoveActr(Add.aiIDX_IndxUpDn , ccFwd);
    }
    if(DM.ARAY[Add.riPRI].CheckAllStat(csNone)){
        m_tmHeat.Clear();
    }
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

        case 10: if(DM.ARAY[Add.riIDX].CheckAllStat(csNone)) { //���� ������.
                     Step.iToStart = 20 ;
                     return false ;
                 }
                 else if(MT_CmprPos(Add.miIDX_XIndx ,m_dLastIdxPos)) {//���� �ְ� �����ǵ� ������.
                     Step.iToStart = 30 ;
                     return false ;
                 }
                 else { //���� �ִµ� �������� �ȸ�����.
                     Step.iToStart = 40 ;
                     return false ;
                 }


                 Step.iToStart ++ ;
                 return false ;

        //���� ������ ����.
        case 20: MoveActr(Add.aiIDX_IndxUpDn  ,ccFwd);
                 Step.iToStart ++ ;
                 return false ;

        case 21: if(!MoveActr(Add.aiIDX_IndxUpDn  ,ccFwd))return false ;
                 MoveMotr(Add.miIDX_XIndx,Add.piIDX_XIndxWait);
                 Step.iToStart ++ ;
                 return false ;

        case 22: if(!MoveMotr(Add.miIDX_XIndx,Add.piIDX_XIndxWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;

        //���� �ְ� ������ ������ ����.
        case 30: MoveActr(Add.aiIDX_IndxUpDn  ,ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 31: if(!MoveActr(Add.aiIDX_IndxUpDn  ,ccBwd))return false ;
                 Step.iToStart = 0 ;
                 return true ;


        //���� �ְ� �������� �ȸ����� ����.
        case 40: MoveActr(Add.aiIDX_IndxUpDn  ,ccFwd);
                 Step.iToStart ++ ;
                 return false ;

        case 41: if(!MoveActr(Add.aiIDX_IndxUpDn  ,ccFwd))return false ;
                 //if(m_dLastIdxPos)MT_GoAbsMan(Add.miIDX_XIndx , m_dLastIdxPos-0.1);
                 //else             MT_GoAbsMan(Add.miIDX_XIndx , m_dLastIdxPos    );

                 //�۾� ������ �������� �����ö� ��ž�ϸ� ��縮��.
                 //MT_GoAbsMan(Add.miIDX_XIndx , GetMotrPos(Add.miIDX_XIndx,Add.piIDX_XIndxWork) -0.1);
                 MT_GoAbsMan(Add.miIDX_XIndx , m_dLastIdxPos -0.1);


                 Step.iToStart ++ ;
                 return false ;

        case 42: if(!MT_GetStop(Add.miIDX_XIndx)) return false;
                 //if(!MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxWork)) return false ;
                 Step.iToStart++;
                 return false ;

        case 43: MoveActr(Add.aiIDX_IndxUpDn  ,ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 44: if(!MoveActr(Add.aiIDX_IndxUpDn  ,ccBwd))return false ;
                 //MT_GoAbsMan(Add.miIDX_XIndx , m_dLastIdxPos);
                 //MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxWork);
                 Step.iToStart++;
                 return false ;

        case 45: //if(!MT_GoAbsMan(Add.miIDX_XIndx , m_dLastIdxPos)) return false;
                 //if(!MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxWork)) return false ;
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

        case 10: SetLastCmd();
                 Step.iToStop++;
                 return false ;

        case 11: MoveActr(Add.aiIDX_IndxUpDn  ,ccFwd);

                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveActr(Add.aiIDX_IndxUpDn  ,ccBwd)) return false ;
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

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isIdxWorkPos  =  IsWorkPos();
        bool isZULDWorkPos = Add.ULD->IsWorkPos();
        bool isGetEnable   = Add.iPartId == piIDX_F  || //����Ʈ�� ���Ͱ� ����.
                             m_tmHeat.OnDelay(true,OM.DevOptn.iFrstPkgDelay) ; //���ͻ󿡼� ���ð� ���� �Ǿ������ �۾� ���� �Ѵ�.



        bool isCycleGet  =  DM.ARAY[Add.riIDX].CheckAllStat (csNone ) && !DM.ARAY[Add.riPRI   ].CheckAllStat(csNone) && isGetEnable ;
        bool isCycleWork =  DM.ARAY[Add.riIDX].GetCntStat   (csUnkwn) && !isIdxWorkPos ;
        bool isCycleOut  = !DM.ARAY[Add.riIDX].CheckAllStat (csNone ) &&
                            DM.ARAY[Add.riIDX].GetCntStat   (csUnkwn) == 0 && DM.ARAY[Add.riIDX].GetCntStat   (csWait) == 0  &&
                            DM.ARAY[Add.riPSI].CheckAllStat (csNone ) &&
                           (DM.ARAY[Add.riULDTop].GetCntStat(csEmpty) || DM.ARAY[Add.riULDBtm].GetCntStat(csEmpty)) &&
                            //Add.miTool
                           //(Add.iPartId == piIDX_R && STG.GetSeqStep() == CUnLoader::scIdle)
                            isZULDWorkPos &&
                            Add.ULD -> GetSeqStep() == CUnLoader::scIdle ;

        bool isCycleEnd  = Add.PRI -> GetStat().bWorkEnd && DM.ARAY[Add.riIDX].CheckAllStat(csNone) ;

        if(EM_IsErr()) return false ;

        //eiULD_MgzSupply �߰�
        if (!OM.MstOptn.bDebugMode) {
            if (!DM.ARAY[Add.riIDX   ].CheckAllStat(csNone ) &&
                !DM.ARAY[Add.riULDTop].GetCntStat  (csEmpty) &&
                !DM.ARAY[Add.riULDBtm].GetCntStat  (csEmpty)) EM_SetErrMsg(eiULD_MgzSupply , "UnLoder Mgz Supply Error.") ;
        }



         //Normal Decide Step.
             if (isCycleGet   ) {Trace(m_sPartName.c_str(),"CycleGet  Stt"); Step.iSeq = scGet    ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork Stt"); Step.iSeq = scWork   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleOut   ) {Trace(m_sPartName.c_str(),"CycleOut  Stt"); Step.iSeq = scOut    ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                        Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                               return false ;
        case scGet   : if(CycleGet   ()){ Trace(m_sPartName.c_str(),"CycleGet    End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scOut   : if(CycleOut   ()){ Trace(m_sPartName.c_str(),"CycleOut    End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CIndex::IsWorkPos()
{
    return MT_CmprPos(Add.miIDX_XIndx ,GetMotrPos(Add.miIDX_XIndx , Add.piIDX_XIndxWork)) ;

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

        case 10: if(!MT_GetHomeEnd(Add.miTool)) return false ;  //�����ϰ�� Stage Z�� ����Ʈ�ϰ�� Picker Z��.
                 if(!MT_GetCmdPos (Add.miTool) == PM.GetValue(Add.miTool,Add.pvToolWait)) return false ;
                 //MoveActr(Add.aiIDX_IndxUpDn , ccFwd);
                 AT_MoveCyl(Add.aiIDX_IndxUpDn , ccFwd);
                 Step.iHome++;
                 return false ;

        case 11: //if(!MoveActr(Add.aiIDX_IndxUpDn , ccFwd)) return false ;
                 if(!AT_MoveCyl(Add.aiIDX_IndxUpDn , ccFwd)) return false ;
                 MT_DoHome(Add.miIDX_XIndx);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(Add.miIDX_XIndx)) return false ;
                 //if(!DM.ARAY[Add.riIDX].CheckAllStat(csNone)) AT_MoveCyl(Add.aiIDX_IndxUpDn , ccBwd);
                 if(DM.ARAY[Add.riIDX].CheckAllStat(csNone)) {MT_GoAbsMan(Add.miIDX_XIndx , PM.GetValue(Add.miIDX_XIndx , Add.pvIDX_XIndxWait)); }
                 else                                        {MT_GoAbsMan(Add.miIDX_XIndx , m_dLastIdxPos); }

                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetStopInpos(Add.miIDX_XIndx )) return false ;
                 if(!DM.ARAY[Add.riIDX].CheckAllStat(csNone)) AT_MoveCyl(Add.aiIDX_IndxUpDn , ccBwd);
                 Step.iHome++;
                 return false;

        case 14: if(!AT_Done(Add.aiIDX_IndxUpDn)) return false ;
                 Step.iHome = 0;
                 return true ;
/*
        case 12: if(!MT_GetHomeEnd(Add.miIDX_XIndx)) return false ;
                 if(DM.ARAY[Add.riIDX].CheckAllStat(csNone)) {MT_GoAbsMan(Add.miIDX_XIndx , PM.GetValue(Add.miIDX_XIndx , Add.pvIDX_XIndxWait)); }
                 else                                        {MT_GoAbsMan(Add.miIDX_XIndx , GetMotrPos (Add.miIDX_XIndx , Add.piIDX_XIndxWork)); }

                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetStopInpos(Add.miIDX_XIndx )) return false ;
                 if(!DM.ARAY[Add.riIDX].CheckAllStat(csNone)) AT_MoveCyl(Add.aiIDX_IndxUpDn , ccBwd);

                 Step.iHome++;

        case 14: if(!AT_Done(Add.aiIDX_IndxUpDn)) return false ;

                 Step.iHome = 0;
                 return true ;
*/
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

    int r ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: MoveActr(Add.aiIDX_IndxUpDn , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(Add.aiIDX_IndxUpDn , ccFwd)) return false ;
                 // juhyeon
                // if(IO_GetX(xPRI_FDetect1) || IO_GetX(xPRI_FDetect2)) {
                //     MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxGet, true);
               //  }
               //  else {
                     MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxGet);
               //  }
                 Step.iCycle++;
                 return false ;

        case 12:
                 if(!MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxGet)) return false ;
                 MoveActr(Add.aiIDX_IndxUpDn , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveActr(Add.aiIDX_IndxUpDn , ccBwd)) return false ;
                 MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxWorkStart , true);
                 Step.iCycle++;
                 return false ;

        case 14: if(!IO_GetX(Add.xIDX_IndxNotOvLd)) {
                     EM_SetErrMsg(eiPRT_OverLoad , (m_sPartName + " Index���� �����ε尡 �߻��Ǿ����ϴ�.").c_str());
                     MT_EmgStop(Add.miIDX_XIndx);
                     MoveActr(Add.aiIDX_IndxUpDn , ccFwd);
                     Step.iCycle= 0 ;
                     return true ;
                 }

                 if(!MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxWorkStart , true)) return false ;

                 DM.ShiftArrayData(Add.riPRI , Add.riIDX);

                 SPC.ERR.ClearLastErr();
                 Step.iCycle = 0;
                 return true ;
    }
}

bool CIndex::CycleWork(void)
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

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: MoveActr(Add.aiIDX_IndxUpDn , ccBwd); //��ũ�� ���� ���� ���� ToStart���� ó����.
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(Add.aiIDX_IndxUpDn , ccBwd)) return false ;
                 MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxWork , true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!IO_GetX(Add.xIDX_IndxNotOvLd)) {
                     EM_SetErrMsg(eiPRT_OverLoad , (m_sPartName + " Index���� �����ε尡 �߻��Ǿ����ϴ�.").c_str());
                     MT_EmgStop(Add.miIDX_XIndx);
                     MoveActr(Add.aiIDX_IndxUpDn , ccFwd);
                     Step.iCycle= 0 ;
                     return true ;
                 }
                 if(!MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxWork , true)) return false ;
                 SPC.ERR.ClearLastErr();
                 Step.iCycle = 0;
                 return true ;
    }

}

bool CIndex::CycleOut(void)
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
    EN_ARAY_ID riULD ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: if(Add.iPartId == piIDX_R) DSP.AddDisprBoatCnt();
                 MoveActr(Add.aiIDX_IndxUpDn , ccBwd); //��ũ�� ���� ���� ���� ToStart���� ó����.
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(Add.aiIDX_IndxUpDn , ccBwd)) return false ;
                 MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxOut , true);
                 m_bPassedDetect5 = false ;
                 Step.iCycle++;
                 return false ;

        case 12: if(IO_GetX(Add.xULD_Detect5)){
                     m_bPassedDetect5 = true ;
                 }

                 if(!IO_GetX(Add.xIDX_IndxNotOvLd)) {
                     EM_SetErrMsg(eiPRT_OverLoad , (m_sPartName + " ���� �����ε尡 �߻��Ǿ����ϴ�.").c_str());
                     MT_EmgStop(Add.miIDX_XIndx);
                     MoveActr(Add.aiIDX_IndxUpDn , ccFwd);
                     Step.iCycle= 0 ;
                     return true ;
                 }
                 if(!MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxOut, true)) return false ;

                 DM.ShiftArrayData(Add.riIDX , Add.riPSI);

                 if(!m_bPassedDetect5){
                     EM_SetErrMsg(eiPRT_BoatMiss , (m_sPartName + " ���� " + Add.xULD_Detect5 + "�� ������ ��Ʈ�� �������� ���� �������� �ʾҽ��ϴ�.").c_str());
                 }

                 MoveActr(Add.aiIDX_IndxUpDn , ccFwd);
                 Step.iCycle++;
                 return false;

        case 13: if(!MoveActr(Add.aiIDX_IndxUpDn , ccFwd)) return false;
                 MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxWait);
                 Step.iCycle++;
                 return false;

        case 14: if(!MoveMotr(Add.miIDX_XIndx , Add.piIDX_XIndxWait)) return false;
                 SPC.ERR.ClearLastErr();
                 Step.iCycle = 0;
                 return true ;
    }

}

//---------------------------------------------------------------------------
bool CIndex::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bWorkingZ ;
    bool bDirection ;
    bool bPRI_IndxDn ;
    bool bWRK_IndxDn ;



    if(Add.iPartId == piIDX_R){
        bWorkingZ   = MT_GetTrgPos(miSTG_ZStage) > PM.GetValue(miSTG_ZStage , pvSTG_ZStagWait);
        bDirection  = MT_GetTrgPos(_iMotr) < GetMotrPos(_iMotr , _iPstnId);
        bPRI_IndxDn = AT_Complete(aiPRI_RIndxUpDn , ccBwd);
        bWRK_IndxDn = AT_Complete(aiIDX_RIndxUpDn , ccBwd);
    }
    else {
        bWorkingZ = MT_GetTrgPos(miPCK_ZPickr) > PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrAlign);
        bDirection  = MT_GetTrgPos(_iMotr) < GetMotrPos(_iMotr , _iPstnId);
        bPRI_IndxDn = AT_Complete(aiPRI_FIndxUpDn , ccBwd);
        bWRK_IndxDn = AT_Complete(aiIDX_FIndxUpDn , ccBwd);

    }

    if(_iMotr == Add.miIDX_XIndx){
        if(Stat.bDontMove){
            sMsg = "Tool is Working Position." ;
            bRet = false ;
        }

        if(_iPstnId == Add.piIDX_XIndxOut) {
            if(!DM.ARAY[Add.riIDX].CheckAllStat(csNone) && !AT_Complete(Add.aiPushUpDn , ccBwd)){
                sMsg = "Cylinder " + AnsiString(AT_GetName(Add.aiPushUpDn)) + " is Fwd." ;
                bRet = false ;
            }
            if(!DM.ARAY[Add.riIDX].CheckAllStat(csNone) && !Add.ULD->IsWorkPos()){
                sMsg = "��δ� Z�� ��ġ�� �۾� ��ġ�� �ƴմϴ�.";
                bRet = false;
            }
        }

        if(bWorkingZ){
            if(Add.iPartId == piIDX_R)sMsg = "���������� Wait �����Ǻ��� ���Ƽ� �浹������ �ֽ��ϴ�." ;
            else                      sMsg = "��Ŀ�� Align ������ ���� ���Ƽ� �浹������ �ֽ��ϴ�." ;

            bRet = false ;
        }

        if(bPRI_IndxDn) {
            //��... �ָ� �ϳ�.. ���䷱���� �۾��� �ȉ�.
            //if(Add.iPartId == piIDX_R)sMsg = "���� �ε����� Up/Dn�Ǹ����� Down �Ǿ� �ֽ��ϴ�." ;
            //else                      sMsg = "���� �ε����� Up/Dn�Ǹ����� Down �Ǿ� �ֽ��ϴ�." ;
            //
            //bRet = false ;
        }

        //��ũ�ε��� ������ �ְ� ���̳ʽ� �������� ������ ��Ʈ�� ���� ������ ó����.
        //20170202 ��
        if(bWRK_IndxDn && !bDirection) {
            sMsg = "��ũ �ε��� Finger�� �ٿ���¿��� �ڷ� �̵��Ҽ� �����ϴ�." ;

            bRet = false ;

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

    bool bPushUpDnFwd = AT_GetCmd(Add.aiPushUpDn) == ccFwd;
    if(_iActr == Add.aiIDX_IndxUpDn){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray ���� ������!"); bRet = false ;}
        }
    }
    else if(_iActr == Add.aiPushLtRt){
        if(_bFwd == ccFwd) {
            if(!bPushUpDnFwd){sMsg = "Cylinder PushUpDn is not Fwd.";bRet = false ;}
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray ���� ������!"); bRet = false ;}
        }
    }
    else if(_iActr == Add.aiPushUpDn){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray ���� ������!"); bRet = false ;}
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
    m_dLastIdxPos = MT_GetTrgPos(Add.miIDX_XIndx);
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
    if(m_dLastIdxPos != MT_GetCmdPos(Add.miIDX_XIndx)) return true  ; //����Ʈ�� �ʿ� ����.
    else                                               return false ;

}

bool CIndex::CheckStop()
{
    if(!MT_GetStop(Add.miIDX_XIndx)) return false ;

    if(!AT_Done(Add.aiIDX_IndxUpDn)) return false ;

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




                                                     
