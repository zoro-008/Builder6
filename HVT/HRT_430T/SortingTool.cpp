//---------------------------------------------------------------------------
#pragma hdrstop

#include "SortingTool.h"

#include "PostBuff.h"
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "ProbeComUnit.h"
#include "LotUnit.h"
#include "Array.h"
#include "PostBuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
CSortingTool STL;
//---------------------------------------------------------------------------
CSortingTool::CSortingTool(void)
{
    m_sPartName = "SortingTool " ;
    Reset();
    Load(true);
    TopRowCnt = -1;
    BtmRowCnt = -1;
}
//---------------------------------------------------------------------------
void CSortingTool::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}
//---------------------------------------------------------------------------
void CSortingTool::ResetTimer()
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
CSortingTool::~CSortingTool (void)
{
    Load(false);
}
//---------------------------------------------------------------------------
bool CSortingTool::FindChip(int &r , int &c , EN_CHIP_STAT Stat)
{
    DM.ARAY[riPS1].FindLastColFrstRow(Stat,r,c ) ;
    return (r > -1 && c > -1);
}
//---------------------------------------------------------------------------
/*
void CSortingTool::PickDir(int Cnt)
{
    int iCnt = DM.ARAY[riPS1].GetCntColStat(Cnt , csFail);

    if(iCnt == 0) {m_bDir = !m_bDir; m_bLtDir = false;}
}
*/
//---------------------------------------------------------------------------
double  CSortingTool::RejectMove(void)
{
//    bool bLastStrip1 = false;
//    bool bLastStrip2 = false;
//
//    AnsiString Temp1 = DM.ARAY[riWRK].GetLotNo();
//    AnsiString Temp2 = DM.ARAY[riPS1].GetLotNo();
//    AnsiString Temp3 = DM.ARAY[riPS2].GetLotNo();
//
//    bLastStrip1  = DM.ARAY[riWRK].GetLotNo() != "" && DM.ARAY[riWRK].GetLotNo() != DM.ARAY[riPS1].GetLotNo();
//    bLastStrip2  = DM.ARAY[riPS2].GetLotNo() != "" && DM.ARAY[riPS2].GetLotNo() != DM.ARAY[riPS1].GetLotNo();
//
//    if(bLastStrip1)  m_sLastID = DM.ARAY[riPS1].GetLotNo();
//    if(bLastStrip2)  m_sLastID = DM.ARAY[riPS2].GetLotNo();

    if((DM.ARAY[riPS1].GetLotNo() != m_sLastStrip) /*&& (m_sLastID != "" )*/)
    {
        m_iBinCnt++;
        if(m_iBinCnt >= MAXBIN)   m_iBinCnt = 0;
        m_sLastStrip = DM.ARAY[riPS1].GetLotNo();
    }

    m_dFailChipPos = PM.GetValue(miREJ_XBtm , pvREJ_XBWorkStartPs) + (OM.DevOptn.dREJPitch * m_iBinCnt) ;
    return m_dFailChipPos;

}
//---------------------------------------------------------------------------
double CSortingTool::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int Col , Col1 , Col2 , Col3;
    int Row , Row1 , Row2 , Row3;
    double dPos = 0.0;
    int r1 , c1 , r2 , c2 , r3 , c3;

    //FindChip(Row , Col , csFail);

    FindChip(Row1 , Col1 , csFail);
    FindChip(Row2 , Col2 , csWork);
    FindChip(Row3 , Col3 , csMask);
                    

    //���⼳�� ���� �ʿ伺 ��� ��.
    //DM.ARAY[riPS1].FindLastColFrstRow(csFail , Row , Col);
    //if(!m_bLtDir)  {m_iPreCol = Col; m_bLtDir = true;}
    //PickDir(m_iPreCol);
//    if(!m_bDir) DM.ARAY[riPS1].FindLastColFrstRow(csFail , Row , Col);
//    else        DM.ARAY[riPS1].FindLastColRow    (csFail , Row , Col);


    if(Step.iSeq == scDevChk) {Row = Row1;}
    if(Step.iSeq == scPick  ) {Row = OM.CmnOptn.bIgnrDecChk ? Row1 : Row2;}
    if(Step.iSeq == scTrmChk) {Row = Row3;}


//         if(Mode == csWork)  FindChip(Row , Col , csWork);
//    else if(Mode == csMask)  FindChip(Row , Col , csMask);
//    else                     FindChip(Row , Col , csFail);


    Row = OM.DevInfo.iRowCnt - Row - 1 ;  // �ǹ��� ��ũ ��ŸƮ ���������� �ٲ�.

    double dY1Pos;
    double dY2Pos;
    //X , Y Pos Setting.
    if(Step.iSeq == scDevChk || Step.iSeq == scTrmChk)
    {
        dY1Pos  = PM.GetValue(miSTL_YTop , pvSTL_YTDevCheckPs) + (OM.DevInfo.dRowPitch * Row) ;
    }
    else
    {
        dY1Pos  = PM.GetValue(miSTL_YTop , pvSTL_YTWorkStartPs) + (OM.DevInfo.dRowPitch * Row) ;
        dY2Pos  = PM.GetValue(miSTL_YBtm , pvSTL_YBWorkStartPs) + (OM.DevInfo.dRowPitch * Row) ;
    }

    if( OM.DevInfo.iRowGrCnt > 1 ) {
        //iColGrCnt��  fc  �� �׷찹��. OM.DevInfo.iColGrCnt�̰Ŵ� �ѱ׷�� LED ����.
        int iRowGrCnt = Row / OM.DevInfo.iRowGrCnt ;
        dY1Pos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt ;
        dY2Pos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt ;
    }

    double dYTStepPos;
    double dYBStepPos;

/*    //Sorting Tool Top Moter Position (1pitch Move)
    if(MT_GetEncPos(miSTL_YTop) < pvSTL_YTWorkStartPs)
    {
        TopRowCnt = -1;
    }
    else if(MT_GetEncPos(miSTL_YTop) >= pvSTL_YTWorkStartPs){
        TopRowCnt = MT_GetEncPos(miSTL_YTop) / OM.DevInfo.dRowPitch;
    }
    if(0 > TopRowCnt){
        dYTStepPos = 0;
        TopRowCnt  = -1;
    }
    else if(TopRowCnt < OM.DevInfo.iRowCnt)
    {
        dYTStepPos  = PM.GetValue(miSTL_YTop , pvSTL_YTWorkStartPs) + (OM.DevInfo.dRowPitch * TopRowCnt) ;

        if( OM.DevInfo.iRowGrCnt < TopRowCnt){
            dYTStepPos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch);
        }
    }
    else if(TopRowCnt >= OM.DevInfo.iRowCnt)
    {
        dYTStepPos = MT_GetEncPos(_iMotr);
        TopRowCnt  = OM.DevInfo.iRowCnt;
    }

    //Sorting Tool Buttom Moter Position (1pitch Move)
    if(MT_GetEncPos(miSTL_YBtm) < pvSTL_YBWorkStartPs)
    {
        BtmRowCnt = -1;
    }

    else if(MT_GetEncPos(miSTL_YTop) >= pvSTL_YTWorkStartPs){
        BtmRowCnt = MT_GetEncPos(miSTL_YBtm) / OM.DevInfo.dRowPitch;
    }

    if(BtmRowCnt < 0)
    {
        dYBStepPos = 0;
        BtmRowCnt  = -1;
    }
    else if(BtmRowCnt < OM.DevInfo.iRowCnt)
    {
        dYBStepPos  = PM.GetValue(miSTL_YBtm , pvSTL_YBWorkStartPs) + (OM.DevInfo.dRowPitch * TopRowCnt) ;

        if( OM.DevInfo.iRowGrCnt < BtmRowCnt){
            dYBStepPos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch);
        }
    }
    else if(BtmRowCnt >= OM.DevInfo.iRowCnt)
    {
        dYBStepPos = MT_GetEncPos(_iMotr);
        BtmRowCnt  = OM.DevInfo.iRowCnt;
    }
*/


    if(_iMotr == miSTL_YTop){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                             ); break ;
            case piSTL_YTWait        : dPos = PM.GetValue (_iMotr , pvSTL_YTWaitPs            ); break ;
            case piSTL_YTWorkStart   : dPos = PM.GetValue (_iMotr , pvSTL_YTWorkStartPs       ); break ;
            case piSTL_YTPlace       : dPos = PM.GetValue (_iMotr , pvSTL_YTPlacePs           ); break ;
            case piSTL_YTWork        : dPos = dY1Pos                                           ; break ;
            case piSTL_YTDevCheck    : dPos = PM.GetValue (_iMotr , pvSTL_YTDevCheckPs        ); break ;
        }
    }
    else if(_iMotr == miSTL_YBtm){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                             ); break ;
            case piSTL_YBWait        : dPos = PM.GetValue (_iMotr , pvSTL_YBWaitPs            ); break ;
            case piSTL_YBWorkStart   : dPos = PM.GetValue (_iMotr , pvSTL_YBWorkStartPs       ); break ;
            case piSTL_YBWork        : dPos = dY2Pos                                           ; break ;
            case piSTL_YBStep        : dPos = dYBStepPos                                       ; break ;
        }
    }
    else if(_iMotr == miREJ_XBtm){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                             ); break ;
            case piREJ_XBWait        : dPos = PM.GetValue (_iMotr , pvREJ_XBWaitPs            ); break ;
            case piREJ_XBWorkStart   : dPos = PM.GetValue (_iMotr , pvREJ_XBWorkStartPs       ); break ;
            case piREJ_XBWork        : dPos = m_dFailChipPos                                   ; break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}
//---------------------------------------------------------------------------
bool CSortingTool::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isSortingDown  =  AT_Complete(aiSTL_Top , ccFwd) ;
    bool isPunchingUp   =  AT_Complete(aiSTL_Btm , ccFwd) ;

    if(_iMotr == miSTL_YTop){
        if(isSortingDown) {sMsg = "During Punching"; bRet = false ;}
    }
    else if(_iMotr == miSTL_YBtm){
        if(isPunchingUp) {sMsg = "During Punching"; bRet = false ;}
    }
    else if(_iMotr == miREJ_XBtm){
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CSortingTool::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isXMovingTop = !MT_GetStopInpos(miSTL_YTop);
    bool isXMovingBtm = !MT_GetStopInpos(miSTL_YBtm);
    bool isPSBXMoving = !MT_GetStopInpos(miPSB_XCmp);

    if(_iActr == aiSTL_Top) {
        if( !_bFwd) {
            if(isXMovingTop) { sMsg = MT_GetName(miSTL_YTop)+" is Moving" ; bRet = false ;}
            if(isPSBXMoving) { sMsg = MT_GetName(miPSB_XCmp)+" is Moving" ; bRet = false ;}
        }
    }
    else if(_iActr == aiSTL_Btm) {
        if( !_bFwd) {
            if(isXMovingBtm) { sMsg = MT_GetName(miSTL_YBtm)+" is Moving" ; bRet = false ;}
            if(isPSBXMoving) { sMsg = MT_GetName(miPSB_XCmp)+" is Moving" ; bRet = false ;}

        }
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}
//---------------------------------------------------------------------------
bool CSortingTool::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}
//---------------------------------------------------------------------------
bool CSortingTool::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}
//---------------------------------------------------------------------------
bool CSortingTool::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiSTL_HomeTo);
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

        case 10: AT_MoveCyl(aiPSB_Cmp,ccFwd); //Ȥ�� �� ���� .
                 AT_MoveCyl(aiSTL_Top,ccBwd);
                 AT_MoveCyl(aiSTL_Btm,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiSTL_Top,ccBwd))return false ;
                 if(!AT_MoveCyl(aiSTL_Btm,ccBwd))return false ;
                 MT_DoHome(miSTL_YTop);
                 MT_DoHome(miSTL_YBtm);
                 MT_DoHome(miREJ_XBtm);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miSTL_YTop))return false;
                 if(!MT_GetHomeEnd(miSTL_YBtm))return false;
                 if(!MT_GetHomeEnd(miREJ_XBtm))return false;
                 MT_GoAbsMan(miSTL_YTop, GetMotrPos(miSTL_YTop,piSTL_YTWait));
                 MT_GoAbsMan(miSTL_YBtm, GetMotrPos(miSTL_YBtm,piSTL_YBWait));
                 MT_GoAbsMan(miREJ_XBtm, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miSTL_YTop, GetMotrPos(miSTL_YTop,piSTL_YTWait)))return false ;
                 if(!MT_GoAbsMan(miSTL_YBtm, GetMotrPos(miSTL_YBtm,piSTL_YBWait)))return false ;
                 if(!MT_GoAbsMan(miREJ_XBtm, m_dLastIndPos))return false ;
                 Step.iHome = 0;
                 return true ;
    }
}
//---------------------------------------------------------------------------
bool CSortingTool::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
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

/*
        int iMaxCol, fc, iReGribCol;
        int r1,c1,r2,c2,r3,c3;

        iMaxCol = DM.ARAY[riWRK].GetMaxCol();
        iReGribCol = OM.DevOptn.iPSBReGripCol ; //30�̶�� ������ 30���� �۾� �ϰ� 31���� ���׸�.   iMaxCol / 2 ; //��Ʈ�� �߰����� �۾� �ϰ� ���׸��Ѵ�.

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

        bool bExistFail     = FindChip(r1 , c1 , csFail);
        bool bExistWork     = DM.ARAY[riPS1].GetCntStat(csWork ); //FindChip(r2 , c2 , csWork);
        bool bExistMask     = DM.ARAY[riPS1].GetCntStat(csMask ); //FindChip(r3 , c3 , csMask);
        bool bFinished      =(!bExistFail && !bExistWork && !bExistMask) || OM.CmnOptn.bIgnrTrim ; //���� ��Ʈ���� �۾��� �������� Ȯ����.

        fc = (iMaxCol - c1) -1  ; //���� �����͸� ������ ��.

        bool isExstTrim1    =  bExistFail && fc <  iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //��Ʈ�� �������� Ʈ�� �� ���� �ִ��� �ϴ� �ɼ�.
        bool isExstTrim2    =  bExistFail && fc >= iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //��Ʈ�� �Ĺ����� Ʈ�� �� ���� �ִ��� �ϴ� �ɼ�.

        ((isExstTrim1 && m_iReGripCnt == 0) || (isExstTrim2 && m_iReGripCnt == 1)) &&
*/
        int iMaxCol, fc , iReGribCol;
        int c1 , r1 ;

        iMaxCol    = DM.ARAY[riWRK].GetMaxCol();
        iReGribCol = OM.DevOptn.iPSBReGripCol ; //30�̶�� ������ 30���� �۾� �ϰ� 31���� ���׸�.   iMaxCol / 2 ; //��Ʈ�� �߰����� �۾� �ϰ� ���׸��Ѵ�.
        bool bExistFail     = FindChip(r1 , c1 , csFail);

        fc = (iMaxCol - c1) -1  ; //���� �����͸� ������ ��.
        bool isExstTrim1    =  bExistFail && fc <  iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //��Ʈ�� �������� Ʈ�� �� ���� �ִ��� �ϴ� �ɼ�.
        bool isExstTrim2    =  bExistFail && fc >= iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //��Ʈ�� �Ĺ����� Ʈ�� �� ���� �ִ��� �ϴ� �ɼ�.

        bool isPSBFailReady =  PSB.GetReadyPos(csFail) && ((isExstTrim1 && PSB.GetRegribCnt() == 0) || (isExstTrim2 && PSB.GetRegribCnt() == 1)) &&
                               PSB.GetCycleStep() == CPostBuff::scIdle;




        bool isTrimCon     =  OM.CmnOptn.bIgnrDecChk ? DM.ARAY[riPS1].GetCntStat(csFail) : DM.ARAY[riPS1].GetCntStat(csWork) && PSB.GetCycleStep() == CPostBuff::scIdle ;  //MT_GetStop(miPSB_XCmp);
        bool isGetReadyPos =  OM.CmnOptn.bIgnrDecChk ? PSB.GetReadyPos(csFail) : PSB.GetReadyPos(csWork) && !PSB.GetReadyPos(csFail) && PSB.GetCycleStep() == CPostBuff::scIdle; //PSB.GetCycleStep() == CPstBuffer::scIdle �̰� �־�� Ȧ�� �ȳ�. 3�� �ε��� �������� �����߿�  ������ �νĵǾ� ����̽� üũ ��.

        //Ȯ�� ����..
        bool isCycDevChk   =  DM.ARAY[riPS1].GetCntStat(csFail) && isPSBFailReady && !OM.CmnOptn.bIgnrTrim  && !OM.CmnOptn.bIgnrDecChk && MT_GetStop(miPSB_XCmp)  ;
        bool isCycTrmChk   = !DM.ARAY[riPS1].GetCntStat(csWork) &&
                              PSB.GetReadyPos(csMask) && !OM.CmnOptn.bIgnrTrim && DM.ARAY[riPS1].GetCntStat(csMask) && !OM.CmnOptn.bIgnrTrimDecChk;


        ////


        bool isCyclePick   =  isTrimCon                           &&
                              DM.ARAY[riPS1].GetStep()            &&
                              DM.ARAY[riSTL].GetCntStat  (csNone) &&
                              isGetReadyPos                       &&
                             !OM.CmnOptn.bIgnrTrim                &&
                              PSB.GetSeqStep() == CPostBuff :: scIdle ;


        bool isCyclePlce = OM.CmnOptn.bIgnrDecChk ? DM.ARAY[riSTL].GetCntStat(csFail) : DM.ARAY[riSTL].GetCntStat(csWork);
        bool isCycleEnd  = DM.ARAY[riPS1].CheckAllStat(csNone) && DM.ARAY[riSTL].CheckAllStat(csNone)                    ;



        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycDevChk) {Trace(m_sPartName.c_str(),"CycleDevChk Stt"); Step.iSeq = scDevChk   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCyclePick) {Trace(m_sPartName.c_str(),"CyclePick Stt"  ); Step.iSeq = scPick     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCyclePlce) {Trace(m_sPartName.c_str(),"CyclePlce Stt"  ); Step.iSeq = scPlace    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycTrmChk) {Trace(m_sPartName.c_str(),"CycleTrmChk Stt"); Step.iSeq = scTrmChk   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleEnd ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }
    //Cycle.
    switch (Step.iSeq) {
        default        :                       Trace(m_sPartName.c_str(),"default   End"  );Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                          return false ;
        case scDevChk  : if(CycleDevCheck ()){ Trace(m_sPartName.c_str(),"CycleDevChk End");Step.iSeq = scIdle ;} return false ;
        case scPick    : if(CyclePick     ()){ Trace(m_sPartName.c_str(),"CyclePick End"  );Step.iSeq = scIdle ;} return false ;
        case scPlace   : if(CyclePlce     ()){ Trace(m_sPartName.c_str(),"CyclePlce End"  );Step.iSeq = scIdle ;} return false ;
        case scTrmChk  : if(CycleDevCheck ()){ Trace(m_sPartName.c_str(),"CycleTrmChk End");Step.iSeq = scIdle ;} return false ;
    }
    return false ;

}
//---------------------------------------------------------------------------
bool CSortingTool::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    //During the auto run, do not stop.
    Stat.bReqStop = true ;
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CSortingTool::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    //During the auto run, do not stop.
    if (Step.iSeq) return false; ///fail �� ��� �˻� �Ҽ���...  Stat.iReInspCnt = 0;

    Step.iToStart = 10 ;

    //Ok.
    return true;
}

bool CSortingTool::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) EM_SetErr(eiSTL_ToStartTO);

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

        case 10: MoveActr(aiSTL_Top , ccBwd);
                 MoveActr(aiSTL_Btm , ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiSTL_Top , ccBwd))return false ;
                 if(!MoveActr(aiSTL_Btm , ccBwd))return false ;

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CSortingTool::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErr(eiSTL_ToStopTO);

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

        case 10: AT_MoveCyl(aiSTL_Btm,ccBwd);
                 AT_MoveCyl(aiSTL_Top,ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiSTL_Btm,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSTL_Top,ccBwd)) return false ;
                 if(!MT_GetAlarm   (miREJ_XBtm) &&
                    !MT_GetNLimSnsr(miREJ_XBtm) &&
                    !MT_GetPLimSnsr(miREJ_XBtm) ) m_dLastIndPos = MT_GetCmdPos(miREJ_XBtm) ;
                 else                             m_dLastIndPos = GetMotrPos  (miREJ_XBtm , piREJ_XBWait) ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CSortingTool::CycleDevCheck(void)
{
    int r1 , c1 , r2 , c2 , r3 , c3;

    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiSTL_CycleTO);
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
    FindChip(r1 , c1 , csFail);
    FindChip(r2 , c2 , csWork);
    FindChip(r3 , c3 , csMask);

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  Step.iCycle++;
                  return false;

        case  12: MoveMotr(miSTL_YTop , piSTL_YTWork);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  13: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iTrimSnrDly)) return false;
                  if(!MoveMotr(miSTL_YTop , piSTL_YTWork)) return false;
                  if(IO_GetX(xSTL_DevDetect))
                  {
                      if(!OM.CmnOptn.bIgnrDecChk     && Step.iSeq == scDevChk){
                          DM.ARAY[riPS1].SetStat(r1,c1,csWork );
                          Trace("WorkMask",(AnsiString(r1) + "," + AnsiString(c1)).c_str());
                      }
                      if(!OM.CmnOptn.bIgnrTrimDecChk && Step.iSeq == scTrmChk)
                      {
                          EM_SetErr(eiSTL_Trim);
                          DM.ARAY[riPS1].SetStat(r3,c3,csFail);
                          IO_SetY(ySTL_Vaccum, false);
                          Step.iCycle = 0 ;
                          return true ;
                      }
                  }
                  else
                  {
                      if(!OM.CmnOptn.bIgnrDecChk     && Step.iSeq == scDevChk)  DM.ARAY[riPS1].SetStat(r1,c1,csEmpty);
                      if(!OM.CmnOptn.bIgnrTrimDecChk && Step.iSeq == scTrmChk)  DM.ARAY[riPS1].SetStat(r3,c3,csEmpty);
                  }
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//One Cycle.
bool CSortingTool::CyclePick(void)
{
    int r1 , c1 , r2 , c2 , r3 , c3;

    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiSTL_CycleTO);
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

    //���⼳��
//    if(!m_bDir) DM.ARAY[riPS1].FindLastColFrstRow(csFail , c , r);
//    else        DM.ARAY[riPS1].FindLastColRow    (csFail , c , r);
    //DM.ARAY[riPS1].FindLastColFrstRow(csFail , r , c);
    FindChip(r1 , c1 , csFail);
    FindChip(r2 , c2 , csWork);
    FindChip(r3 , c3 , csMask);

    if(OM.CmnOptn.bIgnrTrim)
    {
        Trace("IgnrTrimOption_Msg","Pick_ Cycle");
        Step.iCycle = 0;
        return true;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiSTL_Top , ccBwd);
                  MoveActr(aiSTL_Btm , ccBwd);
                  PSB.MoveActr(aiPSB_Cmp , ccFwd);
                  IO_SetY(ySTL_Vaccum, false);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  if(!MoveActr(aiSTL_Btm , ccBwd)) return false;
                  if(!PSB.MoveActr(aiPSB_Cmp , ccFwd)) return false;
                  if(OM.CmnOptn.bSortSensorDelay){
                      IO_SetY(ySTL_Vaccum, true);
                      m_tmTemp.Clear();
                      Step.iCycle++;
                      return false;
                  }
                  else {
                      Step.iCycle = 13;
                      return false;
                  }



        case  12: if(!m_tmTemp.OnDelay(true ,OM.CmnOptn.iSortSensorDelay)) return false;
                  if(IO_GetX(xSTL_Vacuum)){
                      EM_SetErr(eiSTL_MgzUnknow);
                      Step.iCycle = 0 ;
                      IO_SetY(ySTL_Vaccum, false);
                      return true ;
                  }
                  IO_SetY(ySTL_Vaccum, false);
                  Step.iCycle++;
                  return false;

        case  13: MoveMotr(miSTL_YTop , piSTL_YTWork);
                  MoveMotr(miSTL_YBtm , piSTL_YBWork);
                  IO_SetY(ySTL_Vaccum,true);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miSTL_YTop , piSTL_YTWork)) return false ;
                  if(!MoveMotr(miSTL_YBtm , piSTL_YBWork)) return false ;
                  MoveActr(aiSTL_Top , ccFwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiSTL_Top , ccFwd)) return false;
                  MoveActr(aiSTL_Btm , ccFwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiSTL_Btm , ccFwd)) return false;
//                  m_tmTemp.Clear();
//                  Step.iCycle++;
//                  return false;
//
//        case  15: if(!m_tmTemp.OnDelay(true ,1000)) return false ;
                  MoveActr(aiSTL_Btm , ccBwd);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiSTL_Btm , ccBwd)) return false;
                  MoveActr(aiSTL_Top , ccBwd);
                  //�������� ���� ��ĭ �ű�� �ϰ� �; PSB�ʿ��� ȣ���ϰ���.
                  //RejectMove(); //��ġ�� ������ ������. �ؿ��ִ°� �����.
                  MoveMotr(miREJ_XBtm , piREJ_XBWork  );
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveActr(aiSTL_Top  , ccBwd        )) return false;
                  if(!MoveMotr(miREJ_XBtm , piREJ_XBWork )) return false;
                  if(!IO_GetX(xSTL_Vacuum) && !OM.CmnOptn.bIgnrVacuum) {
                      EM_SetErr(eiSTL_PickUp)   ;
                      //if(!OM.CmnOptn.bIgnrDecChk) DM.ARAY[riPS1].SetStat(r2,c2,csWork);
                      //else                        DM.ARAY[riPS1].SetStat(r1,c1,csFail);

                      if(!OM.CmnOptn.bIgnrDecChk) DM.ARAY[riPS1].SetStat(r2,c2,csFail);
                      else                        DM.ARAY[riPS1].SetStat(r1,c1,csFail);

                      //IO_SetY(ySTL_Vaccum, false);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  if(!OM.CmnOptn.bIgnrDecChk && OM.CmnOptn.bIgnrTrimDecChk)
                  {
                      DM.ARAY[riPS1].SetStat(r2,c2,csEmpty);
                      DM.ARAY[riSTL].SetStat(csWork);
                  }
                  else if(OM.CmnOptn.bIgnrDecChk && !OM.CmnOptn.bIgnrTrimDecChk)
                  {
                      DM.ARAY[riPS1].SetStat(r1,c1,csMask);
                      DM.ARAY[riSTL].SetStat(csFail);
                  }
                  else if(OM.CmnOptn.bIgnrDecChk && OM.CmnOptn.bIgnrTrimDecChk)
                  {
                      DM.ARAY[riPS1].SetStat(r1,c1,csEmpty);
                      DM.ARAY[riSTL].SetStat(csFail);
                  }
                  else
                  {
                      DM.ARAY[riPS1].SetStat(r2,c2,csMask);
                      DM.ARAY[riSTL].SetStat(csWork);
                  }
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CSortingTool::CyclePlce()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiSTL_CycleTO);
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

        case  10: MoveActr(aiSTL_Top , ccBwd);
                  MoveActr(aiSTL_Btm , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  if(!MoveActr(aiSTL_Btm , ccBwd)) return false;
                  MoveMotr(miSTL_YTop , piSTL_YTPlace );
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miSTL_YTop , piSTL_YTPlace )) return false ;

                  if(                   IO_GetX(xREJ_FullCheck)) {EM_SetErr(eiREJ_BoxFull  ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 0 && !IO_GetX(xREJ_Box1     )) {EM_SetErr(eiREJ_Box1     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 1 && !IO_GetX(xREJ_Box2     )) {EM_SetErr(eiREJ_Box2     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 2 && !IO_GetX(xREJ_Box3     )) {EM_SetErr(eiREJ_Box3     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 3 && !IO_GetX(xREJ_Box4     )) {EM_SetErr(eiREJ_Box4     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 4 && !IO_GetX(xREJ_Box5     )) {EM_SetErr(eiREJ_Box5     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 5 && !IO_GetX(xREJ_Box6     )) {EM_SetErr(eiREJ_Box6     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 6 && !IO_GetX(xREJ_Box7     )) {EM_SetErr(eiREJ_Box7     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 7 && !IO_GetX(xREJ_Box8     )) {EM_SetErr(eiREJ_Box8     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 8 && !IO_GetX(xREJ_Box9     )) {EM_SetErr(eiREJ_Box9     ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 9 && !IO_GetX(xREJ_Box10    )) {EM_SetErr(eiREJ_Box10    ); Step.iCycle = 0 ; return true ; }

                  MoveActr(aiSTL_Top , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiSTL_Top , ccFwd)) return false ;
                  IO_SetY(ySTL_Vaccum,false);
                  IO_SetY(ySTL_VaccumEje,true);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if(!m_tmTemp.OnDelay(true ,OM.DevOptn.iTrimRjtDly)) return false ;
                  IO_SetY(ySTL_VaccumEje,false);
                  if(IO_GetX(xSTL_Vacuum)) //�� ���� �ɼ� �߰� ����...���� �ؾ� �ǳ� ����...
                  {
                      EM_SetErr(eiSTL_Place)   ;
//                      DM.ARAY[riSTL].SetStat(csNone);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  DM.ARAY[riSTL].SetStat(csNone);
                  MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiSTL_Top , ccBwd)) return false ;
                  Step.iCycle = 0 ;
                  return true ;

//                  MoveMotr(miSTL_YTop , piSTL_YTWait);  //������� ��� �߰�.
//                  MoveMotr(miSTL_YBtm , piSTL_YBWait);
//                  Step.iCycle++;
//                  return false;
//
//        case  16: if(!MoveMotr(miSTL_YTop , piSTL_YTWait)) return false ;
//                  if(!MoveMotr(miSTL_YBtm , piSTL_YBWait)) return false ;
//                  Step.iCycle = 0 ;
//                  return true ;
    }
}

void CSortingTool::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CSortingTool::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CSortingTool::CheckStop()
{
    if(!MT_GetStop(miSTL_YTop)) return false ;
    if(!MT_GetStop(miSTL_YBtm)) return false ;
    if(!MT_GetStop(miREJ_XBtm)) return false ;
    if(!AT_Done(aiSTL_Top    )) return false ;
    if(!AT_Done(aiSTL_Btm    )) return false ;

    return true ;
}

void CSortingTool::Load(bool IsLoad)   //����ؾ���.....�� ���̺� ����.....
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\SortingTool.INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_sLastID"     , m_sLastID     );
        UserINI.Load(sPath, "Member" , "m_iBinCnt"     , m_iBinCnt     );
        UserINI.Load(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
        UserINI.Load(sPath, "Member" , "m_sLastStrip"  , m_sLastStrip  );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sLastID"     , m_sLastID     );
        UserINI.Save(sPath, "Member" , "m_iBinCnt"     , m_iBinCnt     );
        UserINI.Save(sPath, "Member" , "m_dLastIndPos" , m_dLastIndPos );
        UserINI.Save(sPath, "Member" , "m_sLastStrip"  , m_sLastStrip  );
    }
}
//---------------------------------------------------------------------------

































