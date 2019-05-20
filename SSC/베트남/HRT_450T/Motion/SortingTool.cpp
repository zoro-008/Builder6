//---------------------------------------------------------------------------
#pragma hdrstop

#include "SortingTool.h"

#include "PostBuff.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "ProbeComUnit.h"
#include "LotUnit.h"
#include "Array.h"
#include "PostBuff.h"
#include "LotUnit.h"
#include "SPCUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
CSortingTool STL;
//---------------------------------------------------------------------------
CSortingTool::CSortingTool(void)
{
    m_sPartName = "SortingTool " ;
    Reset();
    m_dFailChipPos = 0 ;
    Load(true);
    TopRowCnt = -1;
    BtmRowCnt = -1;

    InitCycleName();
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
bool CSortingTool::GetReadyPos(EN_CHIP_STAT Mode)
{

    //���� ������ �ǽɵǾ� ���������� ���� Ǯ�� ����. ��Ÿ� ���� ���ϸ��.
    double dXCmdPos    = MT_GetCmdPos(miPSB_XCmp);                            //��� ������
    double dXEncPos    = MT_GetEncPos(miPSB_XCmp);                            //���ڴ� ������
    double dXInpos     = 0.1 ;                                                //�˳��ϰ� �޴���� ������ ��������.
    double dFailPos    = PSB.GetMotrPos(miPSB_XCmp , piPSB_XWork , Mode);     //���������� �ִ� ������
    bool   bStoped     = MT_GetStop(miPSB_XCmp);                              //������������.
    bool   bXInposStop = bStoped && (abs(dXCmdPos - dXEncPos) < dXInpos) ;    //�޴���� ������ �������� ������ �������� ��ž Ȯ��.

    //bool   bNeedMove   = bXInposStop && dXCmdPos != dFailPos ;              //�������ǽ�ž�̰� ��������ߴ� �����ǰ� �������� �ִ� �������� �ٸ� ��� �̵��ʿ�.
    bool   bReadyPos   = bXInposStop && abs(dXCmdPos - dFailPos)< dXInpos ;   //�̻��ϴ� dXCmdPos == dFailPos �϶� ���� �ʴٰ� ���ϵ�.

    return bReadyPos ;
    ////////////////////////////////////////////////////////////////////////
}

bool CSortingTool::FindChip(int &r , int &c , EN_CHIP_STAT Stat)
{
    DM.ARAY[riPSB].FindLastColFrstRow(Stat,r,c ) ;
    return (r > -1 && c > -1);
}
//---------------------------------------------------------------------------
double  CSortingTool::RejectMove(void)
{
    double m_dFailChipPos ;

    if(!OM.MstOptn.bUsedBinSorting){
        if((DM.ARAY[riPSB].GetLotNo() != m_sLastStrip) /*&& (m_sLastID != "" )*/)
        {
            m_iBinCnt++;
            if(m_iBinCnt >= MAXBIN)   m_iBinCnt = 0;
            m_sLastStrip = DM.ARAY[riPSB].GetLotNo();
        }

        m_dFailChipPos = PM.GetValue(miREJ_XBtm , pvREJ_XBWorkStart) + (OM.DevOptn.dREJPitch * m_iBinCnt) ;

        return m_dFailChipPos;
    }
    else{
        m_iBinCnt = 10;
        int Row, Row1, Row2 = 0;
        int Col, Col1, Col2 = 0;
        double dRlstVf = 0.0;
        double dRlstVr = 0.0;

        FindChip(Row1 , Col1 , csFail);
        FindChip(Row2 , Col2 , csMask);

        //if(Step.iSeq == scPunch ) {
            Row = OM.CmnOptn.bIgnrDecChk ? Row1 : Row2;
            Col = OM.CmnOptn.bIgnrDecChk ? Col1 : Col2;
        //}

        dRlstVf = DM.ARAY[riPSB].GetVf(0,0);
        dRlstVf = DM.ARAY[riPSB].GetVf(Row, Col);
        dRlstVr = DM.ARAY[riPSB].GetVr(Row, Col);

        if(OM.DevOptn.bUseZener){
            if(OM.CmnOptn.bUsedBin1 ){      if((OM.CmnOptn.dMinVf1 <= 0.0     && OM.CmnOptn.dMaxVf1 <= 0.0     ) && (OM.CmnOptn.dMinVr1  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr1 )) m_iBinCnt = 0;
                                       else if((OM.CmnOptn.dMinVf1 <= 0.0     && OM.CmnOptn.dMaxVf1 <= 0.0     ) && (OM.CmnOptn.dMinVr1  <= 0.0     && 0.0     <= OM.CmnOptn.dMaxVr1 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf1 <= dRlstVf && OM.CmnOptn.dMaxVf1 >= dRlstVf ) m_iBinCnt = 0; }
            if(OM.CmnOptn.bUsedBin2 ){      if((OM.CmnOptn.dMinVf2 <= 0.0     && OM.CmnOptn.dMaxVf2 <= 0.0     ) && (OM.CmnOptn.dMinVr2  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr2 )) m_iBinCnt = 1;
                                       else if((OM.CmnOptn.dMinVf2 <= 0.0     && OM.CmnOptn.dMaxVf2 <= 0.0     ) && (OM.CmnOptn.dMinVr2  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr2 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf2 <= dRlstVf && OM.CmnOptn.dMaxVf2 >= dRlstVf ) m_iBinCnt = 1; }
            if(OM.CmnOptn.bUsedBin3 ){      if((OM.CmnOptn.dMinVf3 <= 0.0     && OM.CmnOptn.dMaxVf3 <= 0.0     ) && (OM.CmnOptn.dMinVr3  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr3 )) m_iBinCnt = 2;
                                       else if((OM.CmnOptn.dMinVf3 <= 0.0     && OM.CmnOptn.dMaxVf3 <= 0.0     ) && (OM.CmnOptn.dMinVr3  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr3 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf3 <= dRlstVf && OM.CmnOptn.dMaxVf3 >= dRlstVf ) m_iBinCnt = 2; }
            if(OM.CmnOptn.bUsedBin4 ){      if((OM.CmnOptn.dMinVf4 <= 0.0     && OM.CmnOptn.dMaxVf4 <= 0.0     ) && (OM.CmnOptn.dMinVr4  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr4 )) m_iBinCnt = 3;
                                       else if((OM.CmnOptn.dMinVf4 <= 0.0     && OM.CmnOptn.dMaxVf4 <= 0.0     ) && (OM.CmnOptn.dMinVr4  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr4 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf4 <= dRlstVf && OM.CmnOptn.dMaxVf4 >= dRlstVf ) m_iBinCnt = 3; }
            if(OM.CmnOptn.bUsedBin5 ){      if((OM.CmnOptn.dMinVf5 <= 0.0     && OM.CmnOptn.dMaxVf5 <= 0.0     ) && (OM.CmnOptn.dMinVr5  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr5 )) m_iBinCnt = 4;
                                       else if((OM.CmnOptn.dMinVf5 <= 0.0     && OM.CmnOptn.dMaxVf5 <= 0.0     ) && (OM.CmnOptn.dMinVr5  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr5 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf5 <= dRlstVf && OM.CmnOptn.dMaxVf5 >= dRlstVf ) m_iBinCnt = 4; }
            if(OM.CmnOptn.bUsedBin6 ){      if((OM.CmnOptn.dMinVf6 <= 0.0     && OM.CmnOptn.dMaxVf6 <= 0.0     ) && (OM.CmnOptn.dMinVr6  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr6 )) m_iBinCnt = 5;
                                       else if((OM.CmnOptn.dMinVf6 <= 0.0     && OM.CmnOptn.dMaxVf6 <= 0.0     ) && (OM.CmnOptn.dMinVr6  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr6 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf6 <= dRlstVf && OM.CmnOptn.dMaxVf6 >= dRlstVf ) m_iBinCnt = 5; }
            if(OM.CmnOptn.bUsedBin7 ){      if((OM.CmnOptn.dMinVf7 <= 0.0     && OM.CmnOptn.dMaxVf7 <= 0.0     ) && (OM.CmnOptn.dMinVr7  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr7 )) m_iBinCnt = 6;
                                       else if((OM.CmnOptn.dMinVf7 <= 0.0     && OM.CmnOptn.dMaxVf7 <= 0.0     ) && (OM.CmnOptn.dMinVr7  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr7 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf7 <= dRlstVf && OM.CmnOptn.dMaxVf7 >= dRlstVf ) m_iBinCnt = 6; }
            if(OM.CmnOptn.bUsedBin8 ){      if((OM.CmnOptn.dMinVf8 <= 0.0     && OM.CmnOptn.dMaxVf8 <= 0.0     ) && (OM.CmnOptn.dMinVr8  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr8 )) m_iBinCnt = 7;
                                       else if((OM.CmnOptn.dMinVf8 <= 0.0     && OM.CmnOptn.dMaxVf8 <= 0.0     ) && (OM.CmnOptn.dMinVr8  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr8 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf8 <= dRlstVf && OM.CmnOptn.dMaxVf8 >= dRlstVf ) m_iBinCnt = 7; }
            if(OM.CmnOptn.bUsedBin9 ){      if((OM.CmnOptn.dMinVf9 <= 0.0     && OM.CmnOptn.dMaxVf9 <= 0.0     ) && (OM.CmnOptn.dMinVr9  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr9 )) m_iBinCnt = 8;
                                       else if((OM.CmnOptn.dMinVf9 <= 0.0     && OM.CmnOptn.dMaxVf9 <= 0.0     ) && (OM.CmnOptn.dMinVr9  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr9 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf9 <= dRlstVf && OM.CmnOptn.dMaxVf9 >= dRlstVf ) m_iBinCnt = 8; }
            if(OM.CmnOptn.bUsedBin10){      if((OM.CmnOptn.dMinVf10<= 0.0     && OM.CmnOptn.dMaxVf10<= 0.0     ) && (OM.CmnOptn.dMinVr10 <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr10)) m_iBinCnt = 9;
                                       else if((OM.CmnOptn.dMinVf10<= 0.0     && OM.CmnOptn.dMaxVf10<= 0.0     ) && (OM.CmnOptn.dMinVr10 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr10)) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf10<= dRlstVf && OM.CmnOptn.dMaxVf10>= dRlstVf ) m_iBinCnt = 9; }
        }

        else{
            if(OM.CmnOptn.bUsedBin1 ){      if(OM.CmnOptn.dMinVf1 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf1 ) m_iBinCnt = 0;
                                       else if(OM.CmnOptn.dMinVf1 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf1 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin2 ){      if(OM.CmnOptn.dMinVf2 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf2 ) m_iBinCnt = 1;
                                       else if(OM.CmnOptn.dMinVf2 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf2 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin3 ){      if(OM.CmnOptn.dMinVf3 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf3 ) m_iBinCnt = 2;
                                       else if(OM.CmnOptn.dMinVf3 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf3 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin4 ){      if(OM.CmnOptn.dMinVf4 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf4 ) m_iBinCnt = 3;
                                       else if(OM.CmnOptn.dMinVf4 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf4 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin5 ){      if(OM.CmnOptn.dMinVf5 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf5 ) m_iBinCnt = 4;
                                       else if(OM.CmnOptn.dMinVf5 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf5 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin6 ){      if(OM.CmnOptn.dMinVf6 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf6 ) m_iBinCnt = 5;
                                       else if(OM.CmnOptn.dMinVf6 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf6 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin7 ){      if(OM.CmnOptn.dMinVf7 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf7 ) m_iBinCnt = 6;
                                       else if(OM.CmnOptn.dMinVf7 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf7 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin8 ){      if(OM.CmnOptn.dMinVf8 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf8 ) m_iBinCnt = 7;
                                       else if(OM.CmnOptn.dMinVf8 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf8 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin9 ){      if(OM.CmnOptn.dMinVf9 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf9 ) m_iBinCnt = 8;
                                       else if(OM.CmnOptn.dMinVf9 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf9 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin10){      if(OM.CmnOptn.dMinVf10<= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf10) m_iBinCnt = 9;
                                       else if(OM.CmnOptn.dMinVf10<= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf10) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
        }
        if(!OM.CmnOptn.bUsedBin1 && !OM.CmnOptn.bUsedBin2 &&
           !OM.CmnOptn.bUsedBin3 && !OM.CmnOptn.bUsedBin4 &&
           !OM.CmnOptn.bUsedBin5 && !OM.CmnOptn.bUsedBin6 &&
           !OM.CmnOptn.bUsedBin7 && !OM.CmnOptn.bUsedBin8 &&
           !OM.CmnOptn.bUsedBin9 && !OM.CmnOptn.bUsedBin10)
        m_iBinCnt = OM.CmnOptn.iBinDefaultNo;

        if(m_iBinCnt == 10){
            m_iBinCnt = 0;
        }


        m_dFailChipPos = PM.GetValue(miREJ_XBtm , pvREJ_XBWorkStart) + (OM.DevOptn.dREJPitch * m_iBinCnt);

        return m_dFailChipPos;
    }
}
//---------------------------------------------------------------------------
double CSortingTool::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int Col , Col1 , Col2 , Col3, Col4 = 0 ;
    int Row , Row1 , Row2 , Row3, Row4 = 0 ;
    double dPos = 0.0;
    int r1 , c1 , r2 , c2 , r3 , c3;

    FindChip(Row1 , Col1 , csFail);
    FindChip(Row2 , Col2 , csWork);
    FindChip(Row3 , Col3 , csMask);

    if(Step.iSeq == scDevChk) {Row = Row1;}
    if(Step.iSeq == scPunch ) {Row = OM.CmnOptn.bIgnrDecChk ? Row1 : Row2;}
    if(Step.iSeq == scTrmChk) {Row = Row3;}

    Row = OM.DevInfo.iRowCnt - Row - 1 ;  // �ǹ��� ��ũ ��ŸƮ ���������� �ٲ�.

    double dY1Pos, dX1Pos;
    //X , Y Pos Setting.
    if(Step.iSeq == scDevChk || Step.iSeq == scTrmChk) {
        dY1Pos  = PM.GetValue(miSTL_YTop , pvSTL_YTDevCheck) ;
    }
    else {
        dY1Pos  = PM.GetValue(miSTL_YTop , pvSTL_YTWorkStart) ;//+ (OM.DevInfo.dRowPitch * Row) ;
    }

    double dBtmYPos = PM.GetValue(miSTL_YBtm , pvSTL_YBWorkStart) ;//+ (OM.DevInfo.dRowPitch * Row) ;

    double dPosOffset = 0.0 ;//��ŸƮ �����ǿ��� ��ġ ����Ͽ� ���ؾ� �ϴ� ��.


    /* 20180511 ������.
    if( OM.DevInfo.iRowGrCnt > 1 || OM.DevInfo.iRowSubGrCnt > 1) {
        //iRowGrCnt�� ���� Row������ �׷��� ����. OM.DevInfo.iRowGrCnt�̰Ŵ� �ѱ׷�� LED ����.
        int iRowGrCnt    = OM.DevInfo.iRowGrCnt   >0 ? Row / OM.DevInfo.iRowGrCnt    : Row ;
        //iRowSubGrCnt�� ���� Row������ ����׷��� ����. OM.DevInfo.iRowSubGrCnt �Ѽ��� �׷�� LED����.
        int iRowSubGrCnt = OM.DevInfo.iRowSubGrCnt>0 ? Row / OM.DevInfo.iRowSubGrCnt : Row ;
        if(iRowGrCnt > 0 && iRowSubGrCnt > 1)  dY1Pos += (OM.DevInfo.dRowGrGap    - OM.DevInfo.dRowPitch) * iRowGrCnt -
                                                         (OM.DevInfo.dRowSubGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt ;
        if(iRowGrCnt > 0 && iRowSubGrCnt <= 1) dY1Pos += (OM.DevInfo.dRowGrGap    - OM.DevInfo.dRowPitch) * iRowGrCnt ;
        if(iRowSubGrCnt > 1)                   dY1Pos += (OM.DevInfo.dRowSubGrGap - OM.DevInfo.dRowPitch) * iRowSubGrCnt ;
    }
    */



    //iRowGrCnt�� ���� Row������ �׷��� ����. OM.DevInfo.iRowGrCnt�̰Ŵ� �ѱ׷�� LED ����.
    //�� 1�̻��̳ĸ� 0�̸� �ڵ����� 1�� ���õǰ� �Ǿ� �ִµ� 1�̸� �ǹ̰� ��� �׳� 0��.
    int iRowGrCnt    = OM.DevInfo.iRowGrCnt   >1 ? Row / OM.DevInfo.iRowGrCnt : 0 ;

    //���� Row������ ����׷�ī��Ʈ.
    //�� 1�̻��̳ĸ� 0�̸� �ڵ����� 1�� ���õǰ� �Ǿ� �ִµ� 1�̸� �ǹ̰� ��� �׳� 0��.
    int iRowSubGrCnt = OM.DevInfo.iRowSubGrCnt >1 ? (OM.DevInfo.iRowGrCnt / OM.DevInfo.iRowSubGrCnt -iRowGrCnt) : 0 ;

    //���� Row������ ����׷�ī��Ʈ.
    //�� 1�̻��̳ĸ� 0�̸� �ڵ����� 1�� ���õǰ� �Ǿ� �ִµ� 1�̸� �ǹ̰� ��� �׳� 0��.
    //int iRowSubGrCntPerGr = OM.DevInfo.iRowSubGrCnt >1 ? (OM.DevInfo.iRowGrCnt / OM.DevInfo.iRowSubGrCnt -iRowGrCnt) : 0 ;




    dPosOffset += Row          *  OM.DevInfo.dRowPitch ;                           //÷�� �ϴ� ��ġ�� ���ϰ�.
    dPosOffset += iRowGrCnt    * (OM.DevInfo.dRowGrGap    - OM.DevInfo.dRowPitch); //���� �ο���� �׷참 ����.
    dPosOffset += iRowSubGrCnt * (OM.DevInfo.dRowSubGrGap - OM.DevInfo.dRowPitch); //���� �ο���� ����׷참 ����.

    dY1Pos   += dPosOffset ;
    dBtmYPos += dPosOffset ;

    dX1Pos = RejectMove();
    

    if(_iMotr == miREJ_XBtm){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                           ); break ;
            case piREJ_XBWait        : dPos = PM.GetValue (_iMotr , pvREJ_XBWait            ); break ;
            case piREJ_XBWorkStart   : dPos = PM.GetValue (_iMotr , pvREJ_XBWorkStart       ); break ;
            case piREJ_XBWork        : dPos = OM.MstOptn.bUsedBinSorting ? dX1Pos : m_dFailChipPos ; break ;
        }
    }
    if(_iMotr == miSTL_YTop){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                           ); break ;
            case piSTL_YTWait        : dPos = PM.GetValue (_iMotr , pvSTL_YTWait            ); break ;
            case piSTL_YTWorkStart   : dPos = PM.GetValue (_iMotr , pvSTL_YTWorkStart       ); break ;
            case piSTL_YTDevCheck    : dPos = PM.GetValue (_iMotr , pvSTL_YTDevCheck        ); break ;
            case piSTL_YTPlace       : dPos = PM.GetValue (_iMotr , pvSTL_YTPlace           ); break ;
//            case piSTL_YVoid         : dPos = PM.GetValue (_iMotr , pvSTL_YVoid             ); break ;
            case piSTL_YTWork        : dPos = dY1Pos                                         ; break ;
           // case piSTL_YTWork        : dPos = dY1Pos                                         ; break ;
        }
    }
    //20180425 ����� ���ݺ�Ʈ���������߰�.
    else if(_iMotr == miSTL_YBtm){
        switch(_iPstnId) {
            case piSTL_YBWait        : dPos = PM.GetValue (_iMotr , pvSTL_YBWait            ); break ;
            case piSTL_YBWorkStart   : dPos = PM.GetValue (_iMotr , pvSTL_YBWorkStart       ); break ;
            case piSTL_YBWork        : dPos = dBtmYPos                                       ; break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    /* 20180425 ���� �̷��ʿ����. sun
    else if(OM.MstOptn.bUsedBinSorting){
        if(_iMotr == miREJ_XBtm){
            switch(_iPstnId) {
                default                  : dPos = MT_GetCmdPos(_iMotr                           ); break ;
                case piREJ_XBWait        : dPos = PM.GetValue (_iMotr , pvREJ_XBWait            ); break ;
                case piREJ_XBWorkStart   : dPos = PM.GetValue (_iMotr , pvREJ_XBWorkStart       ); break ;
                case piREJ_XBWork        : dPos = dX1Pos                                         ; break ;
            }
        }
        if(_iMotr == miSTL_YTop){
            switch(_iPstnId) {
                default                  : dPos = MT_GetCmdPos(_iMotr                           ); break ;
                case piSTL_YTWait        : dPos = PM.GetValue (_iMotr , pvSTL_YTWait            ); break ;
                case piSTL_YTWorkStart   : dPos = PM.GetValue (_iMotr , pvSTL_YTWorkStart       ); break ;
                case piSTL_YTDevCheck    : dPos = PM.GetValue (_iMotr , pvSTL_YTDevCheck        ); break ;
                case piSTL_YTPlace       : dPos = PM.GetValue (_iMotr , pvSTL_YTPlace           ); break ;
                case piSTL_YTWork        : dPos = dY1Pos                                         ; break ;
            }
        }
    }*/



    return dPos ;
}
//---------------------------------------------------------------------------
bool CSortingTool::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId))) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isSortingDown  =  AT_Complete(aiSTL_Top , ccFwd) ;
    bool isPunchingUp   =  AT_Complete(aiSTL_Btm , ccFwd) ;

    if(_iMotr == miSTL_YTop){
        if(isSortingDown) {sMsg = "During Punching"; bRet = false ;}
    }
    else if(_iMotr == miREJ_XBtm){
    }
    else if(_iMotr == miSTL_YBtm){  //20180425 ���ݺ�Ʈ��.
        if(isPunchingUp) {sMsg = "During Punching"; bRet = false ;}
    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart)FM_MsgOk("CheckSafe",sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CSortingTool::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bRet = true ;
    AnsiString sMsg ;
    int iPunchOfs = 3;

    bool isXMovingTop = !MT_GetStopInpos(miSTL_YTop);
    bool isPSBXMoving = !MT_GetStopInpos(miPSB_XCmp);
    //20180425 ���� ��Ʈ�� ������.
    bool isYMovingBtm = OM.MstOptn.bUseSortBtmY && !MT_GetStopInpos(miSTL_YBtm);
    //bool isYTopWaitPs = MT_GetStopInpos(miSTL_YTop) && (MT_GetCmdPos(miSTL_YTop) <= PM.GetValue(miSTL_YTop , pvSTL_YTWorkStart) - iPunchOfs);

    if(_iActr == aiSTL_Top) {
        if( !_bFwd) {
            //if(isYTopWaitPs) { sMsg = "Punching Damage Position "         ; bRet = false ;}
            if(isXMovingTop) { sMsg = MT_GetName(miSTL_YTop)+" is Moving" ; bRet = false ;}
            if(isPSBXMoving) { sMsg = MT_GetName(miPSB_XCmp)+" is Moving" ; bRet = false ;}
        }
    }
    else if(_iActr == aiSTL_Btm) {
        if( !_bFwd) {
            if(isPSBXMoving) { sMsg = MT_GetName(miPSB_XCmp)+" is Moving" ; bRet = false ;}

        }

        if(_bFwd)
        {
            if(isYMovingBtm ){
                if(isPSBXMoving) { sMsg = MT_GetName(miPSB_XCmp)+" is Moving" ; bRet = false ;}
            }
        }



    }
    else {
        bRet = false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart)FM_MsgOk("CheckSafe",sMsg);
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
        EM_SetErr(eiSTL_HomeTO);
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

        case 10: AT_MoveCyl(aiPSB_Cmp,ccBwd); //Ȥ�� �� ���� .
                 AT_MoveCyl(aiSTL_Top,ccBwd);
                 AT_MoveCyl(aiSTL_Btm,ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_Cmp,ccBwd))return false ;
                 if(!AT_MoveCyl(aiSTL_Top,ccBwd))return false ;
                 if(!AT_MoveCyl(aiSTL_Btm,ccBwd))return false ;
                 MT_DoHome(miSTL_YTop);
                 MT_DoHome(miREJ_XBtm);
                 if(OM.MstOptn.bUseSortBtmY){
                     MT_DoHome(miSTL_YBtm);
                 }
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miSTL_YTop))return false;
                 if(!MT_GetHomeEnd(miREJ_XBtm))return false;
                 if(OM.MstOptn.bUseSortBtmY){
                     if(!MT_GetHomeEnd(miSTL_YBtm))return false;
                 }

                 MT_GoAbsMan(miSTL_YTop, GetMotrPos(miSTL_YTop,piSTL_YTWait));
                 MT_GoAbsMan(miREJ_XBtm, m_dLastIndPos);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miSTL_YTop, GetMotrPos(miSTL_YTop,piSTL_YTWait)))return false ;
                 if(!MT_GoAbsMan(miREJ_XBtm, m_dLastIndPos))return false ;
                 if(OM.MstOptn.bUseSortBtmY){
                     if(!MT_GoAbsMan(miSTL_YBtm, GetMotrPos(miSTL_YBtm,piSTL_YBWait)))return false ;
                 }

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

        int iMaxCol, fc , iReGribCol;
        int c1 , r1 ;

        iMaxCol    = DM.ARAY[riWRK].GetMaxCol();
        iReGribCol = OM.DevOptn.iPSBReGripCol ; //30�̶�� ������ 30���� �۾� �ϰ� 31���� ���׸�.   iMaxCol / 2 ; //��Ʈ�� �߰����� �۾� �ϰ� ���׸��Ѵ�.
        bool bExistFail     = FindChip(r1 , c1 , csFail);

        fc = (iMaxCol - c1) -1  ; //���� �����͸� ������ ��.
        bool isExstTrim1    =  bExistFail && fc <  iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //��Ʈ�� �������� Ʈ�� �� ���� �ִ��� �ϴ� �ɼ�.
        bool isExstTrim2    =  bExistFail && fc >= iReGribCol && !OM.CmnOptn.bIgnrTrim  ; //��Ʈ�� �Ĺ����� Ʈ�� �� ���� �ִ��� �ϴ� �ɼ�.

        bool isPSBFailReady =  GetReadyPos(csFail) && ((isExstTrim1 && PSB.GetRegribCnt() == 0) || (isExstTrim2 && PSB.GetRegribCnt() == 1)) &&
                               PSB.GetCycleStep() == CPostBuff::scIdle;

        bool isTrimCon     =  OM.CmnOptn.bIgnrDecChk ? DM.ARAY[riPSB].GetCntStat(csFail) : DM.ARAY[riPSB].GetCntStat(csWork) && PSB.GetCycleStep() == CPostBuff::scIdle ;  //MT_GetStop(miPSB_XCmp);
        bool isGetReadyPos =  OM.CmnOptn.bIgnrDecChk ? GetReadyPos(csFail) : GetReadyPos(csWork) && !GetReadyPos(csFail) && PSB.GetCycleStep() == CPostBuff::scIdle; //PSB.GetCycleStep() == CPstBuffer::scIdle �̰� �־�� Ȧ�� �ȳ�. 3�� �ε��� �������� �����߿�  ������ �νĵǾ� ����̽� üũ ��.

        bool isCycDevChk   =  DM.ARAY[riPSB].GetCntStat(csFail) && isPSBFailReady && !OM.CmnOptn.bIgnrTrim  && !OM.CmnOptn.bIgnrDecChk && MT_GetStop(miPSB_XCmp)  ;
        bool isCycTrmChk   = !DM.ARAY[riPSB].GetCntStat(csWork) &&
                              GetReadyPos(csMask) && !OM.CmnOptn.bIgnrTrim && DM.ARAY[riPSB].GetCntStat(csMask) && !OM.CmnOptn.bIgnrTrimDecChk;


        ////


        bool isCyclePunch  = isTrimCon                           &&
                              DM.ARAY[riPSB].GetSubStep()         &&
                              //DM.ARAY[riSTL].GetCntStat(csNone)   &&
                              isGetReadyPos                       &&
                             !OM.CmnOptn.bIgnrTrim                &&
                              PSB.GetSeqStep() == CPostBuff :: scIdle ;

        if(OM.DevOptn.iPSBReGripCol == 0){
            isCyclePunch   = isCyclePunch && PSB.m_iReGripCnt == 1 ;
        }

        bool isCycleEnd  = DM.ARAY[riPSB].CheckAllStat(csNone) ;//&& //DM.ARAY[riSTL].CheckAllStat(csNone)                    ;



        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycDevChk ) {Trace(m_sPartName.c_str(),"CycleDevChk Stt"); Step.iSeq = scDevChk   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCyclePunch) {Trace(m_sPartName.c_str(),"CyclePunch  Stt"); Step.iSeq = scPunch    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycTrmChk ) {Trace(m_sPartName.c_str(),"CycleTrmChk Stt"); Step.iSeq = scTrmChk   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isCycleEnd  ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }
    //Cycle.
    switch (Step.iSeq) {
        default        :                       Trace(m_sPartName.c_str(),"default   End"  );Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                          return false ;
        case scDevChk  : if(CycleDevCheck ()){ Trace(m_sPartName.c_str(),"CycleDevChk End");Step.iSeq = scIdle ;} return false ;
        case scPunch   : if(CyclePunch    ()){ Trace(m_sPartName.c_str(),"CyclePunch  End");Step.iSeq = scIdle ;} return false ;
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
                 MoveMotr(miSTL_YTop , piSTL_YTWait);
                 MoveMotr(miREJ_XBtm , piREJ_XBWork);
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveMotr(miREJ_XBtm , piREJ_XBWork)) return false ;
                 if(!MoveMotr(miSTL_YTop , piSTL_YTWait)) return false ;
                 //DM.ARAY[riSTL].SetStat(csNone);

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
    int iLastCol = OM.DevInfo.iColCnt - 1;

    double d1,d2,d3;
    bool   b1,b2,b3;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiSTL_Top , ccBwd);
                  if(!OM.MstOptn.bUsed3040Pnch) MoveActr(aiSTL_Btm , ccFwd);
                  else                          MoveActr(aiSTL_Btm , ccBwd);

                  Step.iCycle++;
                  return false;

        case  11: if(                             !MoveActr(aiSTL_Top , ccBwd)) return false;
                  if(!OM.MstOptn.bUsed3040Pnch && !MoveActr(aiSTL_Btm , ccFwd)) return false;
                  if( OM.MstOptn.bUsed3040Pnch && !MoveActr(aiSTL_Btm , ccBwd)) return false; 

                  //����̽� üũ ������ �ؿ� ��ġ�� ������ �ݻ�Ǽ� ���� �ö�
                  if(OM.MstOptn.bUseSortBtmY && !OM.CmnOptn.bIgnrTrimDecChk){
                      if(!MoveActr(aiSTL_Btm , ccBwd)) return false;
                      d1 = GetMotrPos(miSTL_YBtm,piSTL_YBWork) - PM.GetValue(miSTL_YBtm,pvSTL_YBWorkStart);
                      d2 = GetMotrPos(miSTL_YTop,piSTL_YTWork) - PM.GetValue(miSTL_YTop,pvSTL_YTDevCheck );
                      b1 = MT_CmprPos(miSTL_YBtm,GetMotrPos(miSTL_YBtm,piSTL_YBWork));
                      if(d1 == d2 && b1) MT_GoIncRun(miSTL_YBtm,-10.0);
                  }

                  Step.iCycle++;
                  return false;

        case  12: MoveMotr(miSTL_YTop , piSTL_YTWork);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miSTL_YTop , piSTL_YTWork)) return false;
                  //����̽� üũ ������ �ؿ� ��ġ�� ������ �ݻ�Ǽ� ���� �ö�
                  if(!MT_GetStopInpos(miSTL_YBtm)) return false;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iTrimSnrDly)) return false;
                  if(IO_GetX(xSTL_DevDetect)) {
                      if(!OM.CmnOptn.bIgnrDecChk     && Step.iSeq == scDevChk){
                          DM.ARAY[riPSB].SetStat(r1,c1,csWork );
                          m_iLastDecCol = c1;
                          m_iLastDecRow = r1;
                          Trace("WorkMask",(AnsiString(r1) + "," + AnsiString(c1)).c_str());
                      }
                      if(!OM.CmnOptn.bIgnrTrimDecChk && Step.iSeq == scTrmChk) {
                          EM_SetErr(eiSTL_Trim);
                          DM.ARAY[riPSB].SetStat(r3,c3,csFail);
                          m_iLastTrmDecCol = c3 ;
                          m_iLastTrmDecRow = r3 ;
                          if(OM.MstOptn.bUsedBinSorting){
                              if(m_iBinCnt == 0) m_iBin0Cnt--;
                              if(m_iBinCnt == 1) m_iBin1Cnt--;
                              if(m_iBinCnt == 2) m_iBin2Cnt--;
                              if(m_iBinCnt == 3) m_iBin3Cnt--;
                              if(m_iBinCnt == 4) m_iBin4Cnt--;
                              if(m_iBinCnt == 5) m_iBin5Cnt--;
                              if(m_iBinCnt == 6) m_iBin6Cnt--;
                              if(m_iBinCnt == 7) m_iBin7Cnt--;
                              if(m_iBinCnt == 8) m_iBin8Cnt--;
                              if(m_iBinCnt == 9) m_iBin9Cnt--;
                          }
                          Step.iCycle = 0 ;
                          return true ;
                      }
                  }
                  else {
                      if(!OM.CmnOptn.bIgnrDecChk     && Step.iSeq == scDevChk) {
                          DM.ARAY[riPSB].SetStat(r1,c1,csEmpty);
                          m_iLastDecCol    = c1 ;
                          m_iLastDecRow    = r1 ;
                      }
                      if(!OM.CmnOptn.bIgnrTrimDecChk && Step.iSeq == scTrmChk) {
                          DM.ARAY[riPSB].SetStat(r3,c3,csEmpty);
                          //m_sLastLotNo = DM.ARAY[riPSB].GetLotNo();
                          m_iTrimCnt++;
                          //LT.WriteTrimInfo(DM.ARAY[riPSB].GetLotNo(), m_iTrimCnt, m_iBinCnt);
                          m_iLastTrmDecCol = c3 ;
                          m_iLastTrmDecRow = r3 ;
                      }
                  }
                  Step.iCycle++;
                  return false;

        case  15: if(!OM.CmnOptn.bIgnrDecChk && Step.iSeq == scDevChk) {
                      if(DM.ARAY[riPSB].FindLastCol(csFail) != m_iLastDecCol && !DM.ARAY[riPSB].GetCntStat(csWork)){
                          if(!OM.MstOptn.bIgnrVoidMotion)MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle++;
                          return false ;
                      }
                      else {
                          Step.iCycle = 0;
                          return true ;
                      }
                  }
                  else if(!OM.CmnOptn.bIgnrTrimDecChk && Step.iSeq == scTrmChk){
                      if(c3 == m_iLastTrmDecCol){
                          Step.iCycle = 0;
                          return true;
                      }
                      else if(c3 == m_iLastTrmDecCol && r3 == m_iLastTrmDecRow){
                          if(!OM.MstOptn.bIgnrVoidMotion)MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                      else {
                          if(!OM.MstOptn.bIgnrVoidMotion)MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                  }
                  else {
                      if(!OM.MstOptn.bIgnrVoidMotion)MoveMotr(miSTL_YTop , piSTL_YTWait);
                      Step.iCycle++;
                      return false;
                  }
                  /*
                  if(!OM.CmnOptn.bIgnrDecChk && Step.iSeq == scDevChk) {
                      if(c1 == m_iLastDecCol){
                          Step.iCycle = 0;
                          return true;
                      }
                      else if(c1 == m_iLastDecCol && r1 == m_iLastDecRow){
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                      else {
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                  }
                  else if(!OM.CmnOptn.bIgnrTrimDecChk && Step.iSeq == scTrmChk){
                      if(c3 == m_iLastTrmDecCol){
                          Step.iCycle = 0;
                          return true;
                      }
                      else if(c3 == m_iLastTrmDecCol && r3 == m_iLastTrmDecRow){
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                      else {
                          MoveMotr(miSTL_YTop , piSTL_YTWait);
                          Step.iCycle ++;
                          return false;
                      }
                  }
                  else {
                      MoveMotr(miSTL_YTop , piSTL_YTWait);
                      Step.iCycle++;
                      return false;
                  }
                  */

        case  16: MoveActr(aiSTL_Btm , ccBwd);
                  Step.iCycle++;
                  return false;

        case  17: if(!OM.MstOptn.bIgnrVoidMotion && !MoveMotr(miSTL_YTop , piSTL_YTWait)) return false;
                  if(!MoveActr(aiSTL_Btm , ccBwd        )) return false;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//One Cycle.
bool CSortingTool::CyclePunch(void)
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
    //FindChip(r3 , c3 , csMask);

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

        case  10: IO_SetY(ySTL_PnchVacc , false);
                  IO_SetY(ySTL_PnchEjct , false);

                  MoveMotr(miREJ_XBtm , piREJ_XBWork);
                  if(OM.CmnOptn.bUsedTopViewPnch) IO_SetY(ySTL_VaccSol , true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miREJ_XBtm , piREJ_XBWork)) return false;
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


                  //20180425 3���� ������ �ִ�.
                  // 1 !OM.CmnOptn.bUsedTopViewPnch && !OM.MstOptn.bUseSortBtmY
                  //   �ؿ���������+�򵥱�  �������� �ö� ���� ž���� �� ������ ������ Ÿ��.
                  //   ��κ� ��Ÿ��.
                  if(!OM.CmnOptn.bUsedTopViewPnch && !OM.MstOptn.bUseSortBtmY){
                      Step.iCycle = 20 ;
                      return false ;
                  }
                  // 2 OM.CmnOptn.bUsedTopViewPnch && !OM.MstOptn.bUseSortBtmY
                  //   �ؿ� ���������������� ž���� ��Ũ�Ѱ� ������ Ʈ���Ǿ ������� ������ �ܺα򵥱�� �̵��� ������ ���.
                  //   ���ݿ� 1�� ����. 3���� �ٸ����� �ؿ� ���������� ���Ͱ� ���� �׳� ������ ������ ������ ���� �ٴ� ����.
                  else if(OM.CmnOptn.bUsedTopViewPnch && !OM.MstOptn.bUseSortBtmY){
                      Step.iCycle = 40 ;
                      return false ;
                  }

                  // 3 OM.MstOptn.bUseSortBtmY
                  //   ������ �������� UpDn�� Btm Tool UpDn���� ���.
                  //   �ؿ� Y�� ���Ͱ� �ְ� ���Ʒ� ��� �۾� ��ġ�� �̵��Ͽ���Ũ�Ѱ� ������ Ʈ���Ǿ ������� ������ �ܺα򵥱�� �̵��� ������ ���.
                  //   ���ݺ�Ʈ�� 1�� ����.
                  else if(OM.MstOptn.bUseSortBtmY){
                      Step.iCycle = 60 ;
                      return false ;
                  }

                  return false ;

        //======================================================================
        //�ؿ���������+�򵥱�  �������� �ö� ���� ž���� �� ������ ������ Ÿ��.
        case  20: MoveActr    (aiSTL_Top , ccBwd);
                  PSB.MoveActr(aiPSB_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  21: if(!MoveActr    (aiSTL_Top , ccBwd )) return false;
                  if(!PSB.MoveActr(aiPSB_Cmp , ccFwd )) return false;
                  MoveMotr(miSTL_YTop , piSTL_YTWork);
                  MoveActr(aiSTL_Btm  , ccFwd       );
                  Step.iCycle++;
                  return false;

        case  22: if(!MoveMotr(miSTL_YTop , piSTL_YTWork)) return false ;
                  if(!MoveActr(aiSTL_Btm , ccFwd        )) return false ;
                  MoveActr(aiSTL_Top , ccFwd);
                  IO_SetY(ySTL_Ejecter , true);

                  Step.iCycle++;
                  return false;

        case  23: if(!MoveActr(aiSTL_Top , ccFwd)) return false;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  24: if(!m_tmTemp.OnDelay(true ,  OM.DevOptn.iTrimRjtDly)) return false ;
                  IO_SetY(ySTL_Ejecter , false);
                  MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false;

        case  25: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  26: if(!m_tmTemp.OnDelay(true, OM.DevOptn.iRejectMoveDeley)) return false;
                  Step.iCycle = 100;
                  return false ;


        //======================================================================
        //�ؿ� ���������������� ž���� ��Ũ�Ѱ� ������ Ʈ���Ǿ ������� ������ �ܺα򵥱�� �̵��� ������ ���.
        case  40: MoveActr    (aiSTL_Top , ccBwd);
                  PSB.MoveActr(aiPSB_Cmp , ccFwd);
                  Step.iCycle++;
                  return false;

        case  41: if(!MoveActr    (aiSTL_Top , ccBwd )) return false;
                  if(!PSB.MoveActr(aiPSB_Cmp , ccFwd )) return false;
                  MoveMotr(miSTL_YTop , piSTL_YTWork);
                  MoveActr(aiSTL_Btm  , ccFwd       );
                  Step.iCycle++;
                  return false;

        case  42: if(!MoveMotr(miSTL_YTop , piSTL_YTWork)) return false ;
                  if(!MoveActr(aiSTL_Btm , ccFwd        )) return false ;
                  MoveActr(aiSTL_Top , ccFwd);
                  IO_SetY(ySTL_PnchVacc , true );
                  IO_SetY(ySTL_PnchEjct , false);
                  Step.iCycle++;
                  return false ;

        case  43: if(!MoveActr(aiSTL_Top , ccFwd)) return false;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  44: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iTrimRjtDly)) return false;
                  MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  45: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  // 20180425 �۾��Ҷ� ������ ���Ƽ� �ְ� ������ ���� ����� ����
                  // Ȯ���� �ȵǾ� ���� ���ߴ�. ���߿� �ҽ� Ȯ�� �� �� ������ Ȯ�� �ϰ� ���� �ֱ�.
                  // Ȥ�� ��� ��Ũ ���� ����� �Ƚ� ���� ���ϰ���. �����.
                  // if(!IO_GetX(xSTL_VaccChk)){
                  //     EM_SetErr(eiSTL_PickUp);
                  //     Step.iCycle=0;
                  //     return true ;
                  // }
                  MoveMotr(miSTL_YTop , piSTL_YTPlace);
                  Step.iCycle++;
                  return false ;

        case  46: if(!MoveMotr(miSTL_YTop , piSTL_YTPlace)) return false;
                  MoveActr(aiSTL_Top , ccFwd);
                  Step.iCycle++;
                  return false;

        case  47: if(!MoveActr(aiSTL_Top , ccFwd))return false;
                  IO_SetY(ySTL_PnchVacc , false);
                  IO_SetY(ySTL_PnchEjct , true );
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  48: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iTrimRjtDly)) return false;
                  MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  49: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  IO_SetY(ySTL_PnchVacc , false);
                  IO_SetY(ySTL_PnchEjct , false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  50: if(!m_tmTemp.OnDelay(true, OM.DevOptn.iRejectMoveDeley)) return false;
                  Step.iCycle = 100;
                  return false ;


        //======================================================================
        //�ؿ� Y�� ���Ͱ� �ְ� ���Ʒ� ��� �۾� ��ġ�� �̵��Ͽ���Ũ�Ѱ� ������
        //Ʈ���Ǿ ������� ������ �ܺα򵥱�� �̵��� ������ ���.
        case  60: MoveActr    (aiSTL_Top , ccBwd);
                  PSB.MoveActr(aiPSB_Cmp , ccFwd);
                  MoveActr    (aiSTL_Btm , ccBwd);
                  Step.iCycle++;
                  return false;

        case  61: if(!MoveActr    (aiSTL_Top , ccBwd )) return false;
                  if(!PSB.MoveActr(aiPSB_Cmp , ccFwd )) return false;
                  if(!MoveActr    (aiSTL_Btm , ccBwd )) return false;
                  MoveMotr(miSTL_YTop , piSTL_YTWork);
                  MoveMotr(miSTL_YBtm , piSTL_YBWork);

                  Step.iCycle++;
                  return false;

        case  62: if(!MoveMotr(miSTL_YTop , piSTL_YTWork)) return false ;
                  if(!MoveMotr(miSTL_YBtm , piSTL_YBWork)) return false ;
                  //MoveActr(aiSTL_Btm  , ccFwd       );if(!MoveActr(aiSTL_Btm , ccFwd        )) return false ;
                  MoveActr(aiSTL_Top , ccFwd);
                  IO_SetY(ySTL_PnchVacc , true );
                  IO_SetY(ySTL_PnchEjct , false);
                  Step.iCycle++;
                  return false ;

        case  63: if(!MoveActr(aiSTL_Top , ccFwd)) return false;
                  MoveActr(aiSTL_Btm , ccFwd);
                  Step.iCycle++;
                  return false;

        case  64: if(!MoveActr(aiSTL_Btm , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  65: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iTrimRjtDly)) return false;

                  MoveActr(aiSTL_Btm , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  66: if(!MoveActr(aiSTL_Btm , ccBwd)) return false ;
                  //����̽� üũ ������ �ؿ� ��ġ�� ������ �ݻ�Ǽ� ���� �ö�
                  if(!OM.CmnOptn.bIgnrTrimDecChk){
                      if(MT_GetCmdPos(miSTL_YBtm) == PM.GetValue(miSTL_YBtm,pvSTL_YBWorkStart)){
                          MT_GoIncRun(miSTL_YBtm,-10.0);
                      }
                  }

                  MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  67: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  //���� ����ġ�� ���� �̸� ���� ���´�.

                  if(!IO_GetX(xSTL_VaccChk)){
                      EM_SetErr(eiSTL_PickUp);
                      Step.iCycle=0;
                      return true ;
                  }

                  MoveMotr(miSTL_YTop , piSTL_YTPlace);
                  Step.iCycle++;
                  return false ;

        case  68: if(!MoveMotr(miSTL_YTop , piSTL_YTPlace)) return false;

                  MoveActr(aiSTL_Top , ccFwd);
                  Step.iCycle++;
                  return false;

        case  69: if(!MoveActr(aiSTL_Top , ccFwd))return false;
                  IO_SetY(ySTL_PnchVacc , false);
                  IO_SetY(ySTL_PnchEjct , true );
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  70: if(!m_tmTemp.OnDelay(true , OM.DevOptn.iTrimRjtDly)) return false;
                  MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  71: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  IO_SetY(ySTL_PnchVacc , false);
                  IO_SetY(ySTL_PnchEjct , false);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case  72: if(!m_tmTemp.OnDelay(true, OM.DevOptn.iRejectMoveDeley)) return false;
                  Step.iCycle = 100;
                  return false ;



        case 100: if(!OM.CmnOptn.bIgnrDecChk && OM.CmnOptn.bIgnrTrimDecChk) {
                      DM.ARAY[riPSB].SetStat(r2,c2,csEmpty);
                      m_iLastWorkCol = c2;
                  }
                  else if(OM.CmnOptn.bIgnrDecChk && !OM.CmnOptn.bIgnrTrimDecChk) {
                      DM.ARAY[riPSB].SetStat(r1,c1,csMask);
                      m_iLastWorkCol = c1;
                  }
                  else if(OM.CmnOptn.bIgnrDecChk && OM.CmnOptn.bIgnrTrimDecChk) {
                      DM.ARAY[riPSB].SetStat(r1,c1,csEmpty);
                      m_iLastWorkCol = c1;
                  }
                  else {
                      DM.ARAY[riPSB].SetStat(r2,c2,csMask);
                      m_iLastWorkCol = c2;
                  }
                  if(OM.MstOptn.bUsedBinSorting){
                      if(m_iBinCnt == 0) m_iBin0Cnt++;
                      if(m_iBinCnt == 1) m_iBin1Cnt++;
                      if(m_iBinCnt == 2) m_iBin2Cnt++;
                      if(m_iBinCnt == 3) m_iBin3Cnt++;
                      if(m_iBinCnt == 4) m_iBin4Cnt++;
                      if(m_iBinCnt == 5) m_iBin5Cnt++;
                      if(m_iBinCnt == 6) m_iBin6Cnt++;
                      if(m_iBinCnt == 7) m_iBin7Cnt++;
                      if(m_iBinCnt == 8) m_iBin8Cnt++;
                      if(m_iBinCnt == 9) m_iBin9Cnt++;
                  }
                  Step.iCycle++;
                  return false;

        //�ؼ��� �Ӹ��� �ٺ�....����... ����� �Ѵ� �켱...���!!.
        case 101: if(!OM.CmnOptn.bIgnrDecChk && OM.CmnOptn.bIgnrTrimDecChk) {
                      if(c2 == m_iLastWorkCol){
                          Step.iCycle = 0;
                          return true;
                      }
                  }
                  else if(OM.CmnOptn.bIgnrDecChk && !OM.CmnOptn.bIgnrTrimDecChk) {
                      //if(c1 == m_iLastWorkCol){
                          Step.iCycle = 0;
                          return true;
                      //}
                  }
                  else if(OM.CmnOptn.bIgnrDecChk && OM.CmnOptn.bIgnrTrimDecChk) {
                      if(c1 == m_iLastWorkCol){
                          Step.iCycle = 0;
                          return true;
                      }
                  }
                  else {
                      //if(c2 == m_iLastWorkCol){
                          Step.iCycle = 0;
                          return true;
                      //}
                  }
                  MoveActr(aiSTL_Btm , ccBwd);
                  if(!OM.MstOptn.bIgnrVoidMotion) MoveMotr(miSTL_YTop , piSTL_YTWait);

                  Step.iCycle++;
                  return false;

        case 102: if(!MoveActr(aiSTL_Btm , ccBwd)) return false;
                  if(!OM.MstOptn.bIgnrVoidMotion && !MoveMotr(miSTL_YTop , piSTL_YTWait)) return false;
                  Step.iCycle = 0 ;
                  return true ;

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
    if(!MT_GetStop(miREJ_XBtm)) return false ;
    if(!MT_GetStop(miSTL_YBtm)) return false ;


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
        UserINI.Load(sPath, "Member" , "m_sLastID     "  , m_sLastID       );
        UserINI.Load(sPath, "Member" , "m_iBinCnt     "  , m_iBinCnt       );
        UserINI.Load(sPath, "Member" , "m_dLastIndPos "  , m_dLastIndPos   );
        UserINI.Load(sPath, "Member" , "m_sLastStrip  "  , m_sLastStrip    );
        UserINI.Load(sPath, "Member" , "m_dFailChipPos"  , m_dFailChipPos  );

        UserINI.Load(sPath, "Member" , "m_iTrimCnt    "  , m_iTrimCnt      );
        UserINI.Load(sPath, "Member" , "m_sLastLotNo  "  , m_sLastLotNo    );

        UserINI.Load(sPath, "Member" , "m_iBin0Cnt    "  , m_iBin0Cnt      );
        UserINI.Load(sPath, "Member" , "m_iBin1Cnt    "  , m_iBin1Cnt      );
        UserINI.Load(sPath, "Member" , "m_iBin2Cnt    "  , m_iBin2Cnt      );
        UserINI.Load(sPath, "Member" , "m_iBin3Cnt    "  , m_iBin3Cnt      );
        UserINI.Load(sPath, "Member" , "m_iBin4Cnt    "  , m_iBin4Cnt      );
        UserINI.Load(sPath, "Member" , "m_iBin5Cnt    "  , m_iBin5Cnt      );
        UserINI.Load(sPath, "Member" , "m_iBin6Cnt    "  , m_iBin6Cnt      );
        UserINI.Load(sPath, "Member" , "m_iBin7Cnt    "  , m_iBin7Cnt      );
        UserINI.Load(sPath, "Member" , "m_iBin8Cnt    "  , m_iBin8Cnt      );
        UserINI.Load(sPath, "Member" , "m_iBin9Cnt    "  , m_iBin9Cnt      );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sLastID"       , m_sLastID       );
        UserINI.Save(sPath, "Member" , "m_iBinCnt"       , m_iBinCnt       );
        UserINI.Save(sPath, "Member" , "m_dLastIndPos"   , m_dLastIndPos   );
        UserINI.Save(sPath, "Member" , "m_sLastStrip"    , m_sLastStrip    );
        UserINI.Save(sPath, "Member" , "m_dFailChipPos"  , m_dFailChipPos  );

        UserINI.Save(sPath, "Member" , "m_iTrimCnt    "  , m_iTrimCnt      );
        UserINI.Save(sPath, "Member" , "m_sLastLotNo  "  , m_sLastLotNo    );

        UserINI.Save(sPath, "Member" , "m_iBin0Cnt    "  , m_iBin0Cnt      );
        UserINI.Save(sPath, "Member" , "m_iBin1Cnt    "  , m_iBin1Cnt      );
        UserINI.Save(sPath, "Member" , "m_iBin2Cnt    "  , m_iBin2Cnt      );
        UserINI.Save(sPath, "Member" , "m_iBin3Cnt    "  , m_iBin3Cnt      );
        UserINI.Save(sPath, "Member" , "m_iBin4Cnt    "  , m_iBin4Cnt      );
        UserINI.Save(sPath, "Member" , "m_iBin5Cnt    "  , m_iBin5Cnt      );
        UserINI.Save(sPath, "Member" , "m_iBin6Cnt    "  , m_iBin6Cnt      );
        UserINI.Save(sPath, "Member" , "m_iBin7Cnt    "  , m_iBin7Cnt      );
        UserINI.Save(sPath, "Member" , "m_iBin8Cnt    "  , m_iBin8Cnt      );
        UserINI.Save(sPath, "Member" , "m_iBin9Cnt    "  , m_iBin9Cnt      );
    }
}
//---------------------------------------------------------------------------













