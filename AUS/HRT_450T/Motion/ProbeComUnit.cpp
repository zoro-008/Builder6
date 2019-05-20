//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ProbeComUnit.h"
#include "UserINI.h"
#include "sLogUnit.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "FormOperation.h"
#include "FormProbe.h"
#include "UserFile.h"
#include "SLogUnit.h"
#include "SPCUnit.h"
#include "Rs232Man.h"
#include "WorkZone.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define LOT_LOG_FOLDER "d:\\LotLog\\"

CProbeComUnit PCM;

EN_PROB_RSLT iCChipRsltCrnt[MAX_ARRAY_ROW][MAX_CONTACT_COL][MAX_CHIPCNT] ;
double       dCChipVfCrnt  [MAX_ARRAY_ROW][MAX_CONTACT_COL][MAX_CHIPCNT] ;
double       dCChipVrCrnt  [MAX_ARRAY_ROW][MAX_CONTACT_COL][MAX_CHIPCNT] ;

EN_PROB_RSLT iCChipRslt[MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];
double       dCChipVf  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];
double       dCChipVr  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];

EN_PROB_RSLT iPChipRslt[MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];
double       dPChipVf  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];
double       dPChipVr  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];


//---------------------------------------------------------------------------
__fastcall CProbeComUnit::CProbeComUnit(void)
{
    m_iMaxRow = MAX_ARRAY_ROW;
    m_iMaxCol = MAX_ARRAY_COL;
    ClearDataCrnt();
    ClearData    ();
    ClearPstData ();
}
//---------------------------------------------------------------------------
__fastcall CProbeComUnit::~CProbeComUnit(void)
{

}
//---------------------------------------------------------------------------
void __fastcall CProbeComUnit::ShiftArrayData(EN_ARAY_ID _iId)
{
//    LT.WriteArayData (_iId);
    SPC.WriteLotMap(_iId); //iPChipRslt ����ϰ� ����.

//    WriteProbeDataIni(_iId);
    WriteProbeDataCsv(_iId);

    memcpy(iPChipRslt , iCChipRslt , sizeof(int   ) * MAX_ARRAY_ROW * MAX_ARRAY_COL * MAX_CHIPCNT);
    memcpy(dPChipVf   , dCChipVf   , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL * MAX_CHIPCNT);
    memcpy(dPChipVr   , dCChipVr   , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL * MAX_CHIPCNT);

    ClearData ();
}
//---------------------------------------------------------------------------
void __fastcall CProbeComUnit::CChip (int iR,int iC,int iChip,double dVf,double dVr)
{
    dCChipVf[iR][iC][iChip] = dVf ;
    dCChipVr[iR][iC][iChip] = dVr ;
}
//---------------------------------------------------------------------------
bool __fastcall CProbeComUnit::Start(int iCh)
{
    AnsiString sMsgGl = "START#" ;
    AnsiString sMsgDg = "SRT0000#" ;   //�������.

    if     (iCh == 1) {
        WRK.sPb1Msg = "";


        RS232C_Pb1ch->SendData(sMsgDg.Length(),sMsgDg.c_str());
        //if(OM.MstOptn.bUseDaegyeomProbe) RS232C_Pb1ch->WriteData(sMsgDg.Length(),sMsgDg.c_str());
        //else                             RS232C_Pb1ch->WriteData(sMsgGl.Length(),sMsgGl.c_str());
    }
    else if(iCh == 2) {
        WRK.sPb2Msg = "";

        RS232C_Pb2ch->SendData(sMsgDg.Length(),sMsgDg.c_str());
        //if(OM.MstOptn.bUseDaegyeomProbe) RS232C_Pb2ch->WriteData(sMsgDg.Length(),sMsgDg.c_str());
        //else                             RS232C_Pb2ch->WriteData(sMsgGl.Length(),sMsgGl.c_str());
    }
    else              {
        WRK.sPb1Msg = "";
        WRK.sPb2Msg = "";

        RS232C_Pb1ch->SendData(sMsgDg.Length(),sMsgDg.c_str());
        RS232C_Pb2ch->SendData(sMsgDg.Length(),sMsgDg.c_str());
        //if(OM.MstOptn.bUseDaegyeomProbe) {
        //    RS232C_Pb1ch->WriteData(sMsgDg.Length(),sMsgDg.c_str());
        //    RS232C_Pb2ch->WriteData(sMsgDg.Length(),sMsgDg.c_str());
        //}
        //else {
        //    RS232C_Pb1ch->WriteData(sMsgGl.Length(),sMsgGl.c_str());
        //    RS232C_Pb2ch->WriteData(sMsgGl.Length(),sMsgGl.c_str());
        //}
    }
    return true ;
}

bool __fastcall CProbeComUnit::GetDataCh(EN_ARAY_ID aiWhere , AnsiString sMsg , int _iCh , bool _bRpt)
{
    BYTE TempBuff[500];
    AnsiString RcvData = "";
    AnsiString sTemp   = "";
    int iPos , c;

    RcvData = sMsg ;
    //test
//    RcvData = "PUT:RL01N2000RL02N2001RL03N2002RL04N2003RL05N2004RL06N2005RL07N2006RL08N2007RL09N2008RL10N2009RL11N2010RL12N2011RL13N2012RL14N2013RL15N2014RL16N2015RL17N2016RL18N2017RL19N2018RL20N2019RL21N2020RL22N2021RL23N2022RL24N2023RL25N2024RL26N2025RL27N2026RL28N2027RL29N2028RL30N2029RL31N2030RL32N2031RL33N2032RL34N2033";

    Trace("1Ch Rcv" , sMsg.c_str());

    if(RcvData == "OK") return true ;
    if(DM.ARAY[aiWhere].FindLastCol(csUnkwn)<0 ) {Trace("GetDataCh Err","DM.ARAY[aiWhere].FindLastCol(csUnkwn)<0");   return false;}

    c = DM.ARAY[aiWhere].FindLastCol(csUnkwn) ;


    if(                        !RcvData.Pos("L")) {Trace("GetDataCh Err","!RcvData.Pos(L)");   return false;}
    if(OM.DevOptn.bUseZener && !RcvData.Pos("Z")) {Trace("GetDataCh Err","!RcvData.Pos(Z)");   return false;}

    //Set Cnt.
    RcvData = sMsg ;
    RcvData = RcvData.Delete(RcvData.Pos("PUT:"),4); //Delete Put:
    AnsiString sChipData ;
    double     dChipData ;
    bool       bVfRslt   = false ;
    bool       bVrRslt   = false ;
    bool       bDrRslt   = false ;
    bool       bVfFail   = false ;
    bool       bVrFail   = false ;


    int iCh = 0 ;
    String sCh ;

    int iTemp[16];

    if(OM.DevOptn.bSideLEDControl){
        if(WRK.bPb2Rslt && _iCh == PROBE_CH2){
            for(int j=0; j>-OM.DevOptn.iContColCnt / 2; j-- ){
            sArray[OM.DevOptn.iContColCnt / 2 - j] = WRK.sPb2Bin[-j];
            //sArray[OM.DevOptn.iContColCnt - 1 + j] = g_sPb1Bin[-j];
                for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --){
                    iArray[OM.DevOptn.iContColCnt / 2 - j][i] = StrToIntDef(sArray[OM.DevOptn.iContColCnt / 2 - j].SubString(i+1 , 1), 9.0);//sArray[OM.DevOptn.iContColCnt / 2 - j].SubString(i+1 , 1).ToInt();
                }
            }
        }
        else if(WRK.bPb1Rslt && _iCh == PROBE_CH1){
            for(int j=0; j>-OM.DevOptn.iContColCnt / 2; j-- ){
            sArray[- j] = WRK.sPb1Bin[-j];
                for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --){
                iArray[-j][i] = StrToIntDef(sArray[-j].SubString(i+1 , 1), 9.0);//sArray[-j].SubString(i+1 , 1).ToInt();
                }
            }
        }
    }

    /*
    if(g_bPbRelse2Rslt){
        if(_iCh == PROBE_CH2){
            for(int ArrColCnt=0; ArrColCnt < OM.DevOptn.iContColCnt / 2; ArrColCnt++){
                sArray[ArrColCnt] = g_sPb1Bin[ArrColCnt];
                for(int ArrRowCnt = 0 ; ArrRowCnt < 16 ; ArrRowCnt++){
                    iArray[ArrColCnt][ArrRowCnt] = sArray[ArrColCnt].SubString(16 - ArrRowCnt , 1).ToInt();
                }
            }
        }
    }
    else if(g_bPbRelse1Rslt){
        if(_iCh == PROBE_CH1){
            for(int ArrColCnt = 2 ; ArrColCnt < OM.DevOptn.iContColCnt ; ArrColCnt++){
                sArray[ArrColCnt] = g_sPb2Bin[ArrColCnt];
                for(int ArrRowCnt = 0 ; ArrRowCnt < 16 ; ArrRowCnt++){
                    iArray[ArrColCnt][ArrRowCnt] = sArray[ArrColCnt].SubString(16 - ArrRo
                    wCnt , 1).ToInt();
                }
            }
        }
    }
    */
    //RL00N0279RL01N0177
    // ��� �׽�Ʈ                   ex iContColCnt = 2 iRowCnt = 8 ;
    int iCnt = -1 ;
    for(int j=0; j>-OM.DevOptn.iContColCnt / 2; j-- )
    {
        for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --)
        {
            bVfFail = true;
            bVrFail = true;
            for(int k=0; k< OM.DevOptn.iDgChipCnt; k++ ) //��� Ĩ ī��Ʈ�� �ƴ϶� �׳� Ĩī��Ʈ�� �ٲٰ� GL ������ ����ؾߵ�.
            {
                iCnt++ ;
                //if(OM.MstOptn.bUseDaegyeomProbe) { //
                    sCh.sprintf("RL%02d",iCnt);
                    sChipData = RcvData.SubString(RcvData.Pos(sCh)+5,4); //���� �����͸� ©��.
                    RcvData.Delete(RcvData.Pos(sCh),10);
                    sChipData = sChipData.SubString(0,2) + "." + sChipData.SubString(3,4);
                    dChipData = StrToFloatDef(sChipData, 30.0);// sChipData.ToDouble() ;
                    //dChipData = sChipData.ToDouble() ;
                //}
                //else {
                //    sChipData = RcvData.SubString(RcvData.Pos("L")+1,4); //���� �����͸� ©��.
                //    RcvData.Delete(RcvData.Pos("L"),5);
                //    dChipData = 0.0012 * sChipData.ToIntDef(-1) * 3;
                //}

                if(_iCh == PROBE_CH1) {
                    if(OM.MstOptn.d1chLedGain) dChipData = dChipData * OM.MstOptn.d1chLedGain   ;
                    dChipData = dChipData + OM.MstOptn.d1chLedOffset ;

                    if(OM.MstOptn.bShortMonitor && dChipData < 2.0) EM_SetErr(eiWRK_ShrtMonitor) ;
                    dCChipVfCrnt[i][OM.DevOptn.iContColCnt-1 + j][k] = dChipData ;


                }
                if(_iCh == PROBE_CH2) {
                    if(OM.MstOptn.d2chLedGain) dChipData = dChipData * OM.MstOptn.d2chLedGain   ;
                    dChipData = dChipData + OM.MstOptn.d2chLedOffset ;

                    if(OM.MstOptn.bShortMonitor && dChipData < 2.0) EM_SetErr(eiWRK_ShrtMonitor) ;
                    dCChipVfCrnt[i][OM.DevOptn.iContColCnt/2 - 1 + j][k] = dChipData ;
                }

                //LED Data Cal.
                if(iCChipRslt[i][c+j][k] != prOk || _bRpt) {
                    dCChipVf[i][c+j][k] = dChipData ;
                }

                if(OM.CmnOptn.bIgnrDrop && _iCh == PROBE_CH1){
                    iTemp[i]= iArray[-j][i];
                    bDrRslt = !iArray[-j][i];
                }
                else if(OM.CmnOptn.bIgnrDrop && _iCh == PROBE_CH2){
                    iTemp[i]= iArray[OM.DevOptn.iContColCnt - 2 -j][i];
                    bDrRslt = !iArray[OM.DevOptn.iContColCnt - 2 -j][i];
                }
                else {
                    bDrRslt = true;
                }
                    /*
                    if(_iCh == PROBE_CH1){
                        for(int ArrColCnt = 0 ; ArrColCnt < 2 ; ArrColCnt++){
                            sArray[ArrColCnt] = g_sPb1Bin[ArrColCnt];
                            for(int ArrRowCnt = 0 ; ArrRowCnt < 16 ; ArrRowCnt++){
                                iArray[ArrColCnt][ArrRowCnt] = sArray[ArrColCnt].SubString(16 - ArrRowCnt , 1).ToInt();
                            }
                        }
                    }
                    if(_iCh == PROBE_CH2){
                        for(int ArrColCnt = 0 ; ArrColCnt < 2 ; ArrColCnt++){
                            sArray[ArrColCnt] = g_sPb2Bin[ArrColCnt];
                            for(int ArrRowCnt = 0 ; ArrRowCnt < 16 ; ArrRowCnt++){
                                iArray[ArrColCnt][ArrRowCnt] = sArray[ArrColCnt].SubString(16 - ArrRowCnt , 1).ToInt();
                            }
                        }
                    }

                }
                */

                bVfRslt = OM.DevOptn.d1ChLow   <= dCChipVf[i][c+j][k] && dCChipVf[i][c+j][k] <= OM.DevOptn.d1ChHigh   ;

                //20150330Ȯ�� JS
                if(bVfFail){
                    DM.ARAY[aiWhere].SetVf(i,c+j,dCChipVf[i][c+j][k]);
                }
                bVfFail = bVfRslt;



                //Zenner Cnt===========================================
                bVrRslt = true ;
                if(OM.DevOptn.bUseZener) {
                   //RcvData.Delete(RcvData.Pos("Z") , 1);
                    //if(OM.MstOptn.bUseDaegyeomProbe) {
                        sCh.sprintf("RZ%02d",iCnt);
                        sChipData = RcvData.SubString(RcvData.Pos(sCh)+5,4); //���� �����͸� ©��.
                        RcvData.Delete(RcvData.Pos(sCh),9);
                        sChipData = sChipData.SubString(0,2) + "." + sChipData.SubString(3,4);
                        dChipData = StrToFloatDef(sChipData , 30.0);
                        //dChipData = sChipData.ToDouble();
                    //}
                    //else {
                    //    sChipData = RcvData.SubString(RcvData.Pos("Z")+1,4); //���� �����͸� ©��.
                    //    RcvData.Delete(RcvData.Pos("Z"),5);
                    //    dChipData = 0.0012 * sChipData.ToIntDef(-1) * 3;
                    //}

                    if(_iCh == PROBE_CH1) {
                        if(OM.MstOptn.d1chZennerGain) dChipData = dChipData * OM.MstOptn.d1chZennerGain   ;
                        dChipData = dChipData + OM.MstOptn.d1chZennerOffset ;
                        dCChipVrCrnt[i][OM.DevOptn.iContColCnt-1 + j][k] = dChipData ;
                    }
                    if(_iCh == PROBE_CH2) {
                        if(OM.MstOptn.d2chZennerGain) dChipData = dChipData * OM.MstOptn.d2chZennerGain   ;
                        dChipData = dChipData + OM.MstOptn.d2chZennerOffset ;
                        dCChipVrCrnt[i][OM.DevOptn.iContColCnt/2 - 1 + j][k] = dChipData ;
                    }

                    //Zenner Data Cal.
                    if(iCChipRslt[i][c+j][k] != prOk || _bRpt) {
                        dCChipVr[i][c+j][k] = dChipData ;
                    }


                    bVrRslt = OM.DevOptn.dZenerLow <= dCChipVr[i][c+j][k] && dCChipVr[i][c+j][k] <= OM.DevOptn.dZenerHigh ;

                    //20150330Ȯ�� JS
                    if(bVrFail){
                        DM.ARAY[aiWhere].SetVr(i,c+j,dCChipVr[i][c+j][k]);
                    }
                    bVrFail = bVfRslt;


                }

                //if(OM.MstOptn.bUseDaegyeomProbe) { //

                     if ( bVfRslt &&  bVrRslt &&  bDrRslt) {iCChipRslt[i][c+j][k] = prOk     ;  }
                else if ( bVfRslt &&  bVrRslt && !bDrRslt) {iCChipRslt[i][c+j][k] = prDRNg   ;  }
                else if ( bVfRslt && !bVrRslt &&  bDrRslt) {iCChipRslt[i][c+j][k] = prVRNg   ;  }
                else if ( bVfRslt && !bVrRslt && !bDrRslt) {iCChipRslt[i][c+j][k] = prVRNg   ;  }
                else if (!bVfRslt &&  bVrRslt &&  bDrRslt) {iCChipRslt[i][c+j][k] = prVFNg   ;  }
                else if (!bVfRslt &&  bVrRslt && !bDrRslt) {iCChipRslt[i][c+j][k] = prVFNg   ;  }
                else if (!bVfRslt && !bVrRslt &&  bDrRslt) {iCChipRslt[i][c+j][k] = prVFVRNg ;  }
                else if (!bVfRslt && !bVrRslt && !bDrRslt) {iCChipRslt[i][c+j][k] = prVFVRNg ;  }


                int iTemp = iCChipRslt[i][c+j][k] ;

                if(!OM.MstOptn.bHeattingTest) //�� ����ȭ �Ϸ��� ���⼭ �ٲٸ� �ǰٳ׿�.
                {
                    if(OM.DevOptn.iDgChipCnt - 1 == k )
                    {
                        if(OM.DevOptn.iDgChipCnt == 1 )
                        {
                            if     (iCChipRslt[i][c+j][k] == prOk  ) DM.ARAY[aiWhere].SetStat(i,c+j,csGood);
                            else if(iCChipRslt[i][c+j][k] == prDRNg) DM.ARAY[aiWhere].SetStat(i,c+j,csDrop);
                            else                                     DM.ARAY[aiWhere].SetStat(i,c+j,csFail);
                        }
                        else if(OM.DevOptn.iDgChipCnt == 2 )
                        {
                                 if(iCChipRslt[i][c+j][k] == prOk   && iCChipRslt[i][c+j][k-1] == prOk  ) DM.ARAY[aiWhere].SetStat(i,c+j,csGood);
                            else if(iCChipRslt[i][c+j][k] == prDRNg || iCChipRslt[i][c+j][k-1] == prDRNg) DM.ARAY[aiWhere].SetStat(i,c+j,csDrop);
                            else                                                                          DM.ARAY[aiWhere].SetStat(i,c+j,csFail);
                        }
                        else if(OM.DevOptn.iDgChipCnt == 3 )
                        {
                                 if(iCChipRslt[i][c+j][k] == prOk   && iCChipRslt[i][c+j][k-1] == prOk   && iCChipRslt[i][c+j][k-2] == prOk   ) DM.ARAY[aiWhere].SetStat(i,c+j,csGood);
                            else if(iCChipRslt[i][c+j][k] == prDRNg || iCChipRslt[i][c+j][k-1] == prDRNg || iCChipRslt[i][c+j][k-2] == prDRNg ) DM.ARAY[aiWhere].SetStat(i,c+j,csDrop);
                            else                                                                                                                DM.ARAY[aiWhere].SetStat(i,c+j,csFail);
                        }
                    }
                }
            }
        }
    }


    return true ;
}

bool __fastcall CProbeComUnit::CheckDataCnt(EN_ARAY_ID aiWhere , AnsiString sMsg)
{
    BYTE TempBuff[500];
    AnsiString RcvData = "";
    AnsiString sTemp   = "";
    int iPos , c;

    RcvData = sMsg ;

    //Trace("1Ch Rcv" , sMsg.c_str());

    if(RcvData == "OK") return true ;
    if(DM.ARAY[aiWhere].FindLastCol(csUnkwn)<0 ) {Trace("Return","DM.ARAY[aiWhere].FindLastCol(csUnkwn)<0 "); return false;}

    c = DM.ARAY[aiWhere].FindLastCol(csUnkwn) ;


    if(                        !RcvData.Pos("L")) {Trace("Return","!RcvData.Pos(L)"); return false;}
    if(OM.DevOptn.bUseZener && !RcvData.Pos("Z")) {Trace("Return","!RcvData.Pos(Z)"); return false;}


    //Check Cnt.
    for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --) {
        for(int j=0; j>-OM.DevOptn.iContColCnt / 2; j-- ){
            //if(OM.MstOptn.bUseDaegyeomProbe) {
                for(int k=0; k < OM.DevOptn.iDgChipCnt; k++ ) {
                    //LED Cnt
                    if(!RcvData.Pos("RL")) return false ;   //Ĩ���� �����Ͱ� ���� ���
                    RcvData.Delete(RcvData.Pos("RL") , 2);
                }

                //Zener Cnt
                if(OM.DevOptn.bUseZener) {
                    for(int k=0; k < OM.DevOptn.iDgChipCnt; k++ ) {
                        if(!RcvData.Pos("RZ")) return false ; //Ĩ���� �����Ͱ� ���� ���
                        RcvData.Delete(RcvData.Pos("RZ") , 2);
                    }
                }
            //}
            //else {
                //LED Cnt
            //    if(!RcvData.Pos("L")) return false ;   //Ĩ���� �����Ͱ� ���� ���
            //    RcvData.Delete(RcvData.Pos("L") , 1);
            //
            //    //Zener Cnt
            //    if(OM.DevOptn.bUseZener) {
            //        if(!RcvData.Pos("Z")) return false ; //Ĩ���� �����Ͱ� ���� ���
            //        RcvData.Delete(RcvData.Pos("Z") , 1);
            //    }
            //}
        }
    }
    if(                        RcvData.Pos("L")) return false ; //Ĩ���� �����Ͱ� �������.
    if(OM.DevOptn.bUseZener && RcvData.Pos("Z")) return false ;

    return true ;
}


/*    RcvData = sMsg ;





    //Set Data
    if(OM.DevOptn.iContColCnt == 2 ){
        //for(int i=0; i<OM.DevInfo.iRowCnt; i++)
        for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --)
        {
            if(!RcvData.Pos("Z")) {
                if( RcvData == "\r" || !RcvData.Pos("L") ) return ;
            }
            iPos = RcvData.Pos("L") + 1;
            RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
            sTemp = RcvData;
            sTemp = sTemp.SubString(1,4);
            dCChipVf[i][c] = 0.0012 * sTemp.ToIntDef(0) * 3;
            if(OM.DevOptn.d1ChLow <= dCChipVf[i][c] && dCChipVf[i][c] <= OM.DevOptn.d1ChHigh         ) {
                DM.ARAY[aiWhere].SetStat(i,c,csGood);
                iCChipVf[i][c] = true;
            }
            else {
                DM.ARAY[aiWhere].SetStat(i,c,csFail);
                iCChipVf[i][c] = false;
            }
        }

        if(OM.DevOptn.bUseZener){
            for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --)
            {
                if(RcvData == "\r" || !RcvData.Pos("Z")) return ;
                iPos = RcvData.Pos("Z") + 1;
                RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
                sTemp = RcvData;
                sTemp = sTemp.SubString(1,4);
                dCChipVr[i][c] = 0.0012 * sTemp.ToIntDef(0) * 3;
                if(OM.DevOptn.d1ChLow   <= dCChipVf[i][c] && dCChipVf[i][c] <= OM.DevOptn.d1ChHigh &&
                   OM.DevOptn.dZenerLow <= dCChipVr[i][c] && dCChipVr[i][c] <= OM.DevOptn.dZenerHigh ) {
                    //DM.ARAY[aiWhere].SetStat(i,c,csGood);
                    iCChipVr[i][c] = true;
                }
                else {
                    DM.ARAY[aiWhere].SetStat(i,c,csFail);
                    iCChipVr[i][c] = false;
                }
            }
        }
    }

    if(OM.DevOptn.iContColCnt == 4 ) {
        for(int j=0; j<2; j++ ){
            for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --)
            {
                if(!RcvData.Pos("Z")) {
                    if( RcvData == "\r" || !RcvData.Pos("L") ) return ;
                }
                iPos = RcvData.Pos("L") + 1;
                RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
                sTemp = RcvData;
                sTemp = sTemp.SubString(1,4);
                if(!RcvData.Pos("L")) sTemp = 0.0000;
                dCChipVf[i][c - j] = 0.0012 * sTemp.ToIntDef(0) * 3;
                if(OM.DevOptn.d1ChLow <= dCChipVf[i][c - j] && dCChipVf[i][c - j] <= OM.DevOptn.d1ChHigh ) {
                    DM.ARAY[aiWhere].SetStat(i,c-j,csGood);
                    iCChipVf[i][c] = true;
                }
                else {
                    DM.ARAY[aiWhere].SetStat(i,c-j,csFail);
                    iCChipVf[i][c] = false;
                }
            }
        }

        if(OM.DevOptn.bUseZener )
        {
            for(int j=0; j<2; j++ ){
                for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --)
                {
                    if(RcvData == "\r" || !RcvData.Pos("Z")) return ;
                    iPos = RcvData.Pos("Z") + 1;
                    RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
                    sTemp = RcvData;
                    sTemp = sTemp.SubString(1,4);
                    dCChipVr[i][c-j] = 0.0012 * sTemp.ToIntDef(0) * 3;
                    if(OM.DevOptn.d1ChLow   <= dCChipVf[i][c-j] && dCChipVf[i][c-j] <= OM.DevOptn.d1ChHigh &&
                       OM.DevOptn.dZenerLow <= dCChipVr[i][c-j] && dCChipVr[i][c-j] <= OM.DevOptn.dZenerHigh ) {
                        //DM.ARAY[aiWhere].SetStat(i,c-j,csGood);
                        iCChipVr[i][c] = true;
                    }
                    else {
                        DM.ARAY[aiWhere].SetStat(i,c-j,csFail);
                        iCChipVr[i][c] = false;
                    }
                }
            }
        }
    }
    return ;


//    RcvData.c_str();



//    memcpy(dPChipVf , dCChipVf , sizeof(double) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
//    memcpy(dPChipVr , dCChipVr , sizeof(double) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
*/
//---------------------------------------------------------------------------
/*void __fastcall CProbeComUnit::GetData2Ch(EN_ARAY_ID aiWhere , AnsiString sMsg)
{
     BYTE TempBuff[500];
    AnsiString RcvData = "";
    AnsiString sTemp   = "";
    int iPos , c;

    RcvData = sMsg ;

    if(RcvData == "OK") return;
    //for(int i=0; i<iLen; i++)
    //{
    //    RcvData = RcvData + (char )bData[i];
    //}

//    ShowMessage("return false");

    //if(DM.ARAY[aiWhere].FindLastCol(csWork)<0 ) return ;
    //else c = DM.ARAY[aiWhere].FindLastCol(csWork) ;
    if(DM.ARAY[aiWhere].FindLastCol(csUnkwn)<0 ) return ;
    else c = DM.ARAY[aiWhere].FindLastCol(csUnkwn) ;

    if(OM.DevOptn.iContColCnt == 2 ){
        for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --)
        {
            if(!RcvData.Pos("Z")) {
                if( RcvData == "\r" || !RcvData.Pos("L") ) return ;
            }
            iPos = RcvData.Pos("L") + 1;
            RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
            sTemp = RcvData;
            sTemp = sTemp.SubString(1,4);
            dCChipVf[i][c] = 0.0012 * sTemp.ToIntDef(0) * 3;
            if(OM.DevOptn.d1ChLow <= dCChipVf[i][c] && dCChipVf[i][c] <= OM.DevOptn.d1ChHigh         ) {
                DM.ARAY[aiWhere].SetStat(i,c,csGood);
                iCChipVf[i][c] = true;
            }
            else {
                DM.ARAY[aiWhere].SetStat(i,c,csFail);
                iCChipVf[i][c] = false;
            }
        }

        if(OM.DevOptn.bUseZener){
            for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --)
            {
                if(RcvData == "\r" || !RcvData.Pos("Z")) return ;
                iPos = RcvData.Pos("Z") + 1;
                RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
                sTemp = RcvData;
                sTemp = sTemp.SubString(1,4);
                dCChipVr[i][c] = 0.0012 * sTemp.ToIntDef(0) * 3;
                if(OM.DevOptn.d1ChLow   <= dCChipVf[i][c] && dCChipVf[i][c] <= OM.DevOptn.d1ChHigh &&
                   OM.DevOptn.dZenerLow <= dCChipVr[i][c] && dCChipVr[i][c] <= OM.DevOptn.dZenerHigh ) {
                    DM.ARAY[aiWhere].SetStat(i,c,csGood);
                    iCChipVr[i][c] = true;
                }
                else {
                    DM.ARAY[aiWhere].SetStat(i,c,csFail);
                    iCChipVr[i][c] = false;
                }
            }
        }
    }

    if(OM.DevOptn.iContColCnt == 4 ) {
        for(int j=0; j<2; j++ ){
            for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --)
            {
                if(!RcvData.Pos("Z")) {
                    if( RcvData == "\r" || !RcvData.Pos("L") ) return ;
                }
                iPos = RcvData.Pos("L") + 1;
                RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
                sTemp = RcvData;
                sTemp = sTemp.SubString(1,4);
                if(!RcvData.Pos("L")) sTemp = 0.0000;
                dCChipVf[i][c - j] = 0.0012 * sTemp.ToIntDef(0) * 3;
                if(OM.DevOptn.d1ChLow <= dCChipVf[i][c - j] && dCChipVf[i][c - j] <= OM.DevOptn.d1ChHigh ) {
                    DM.ARAY[aiWhere].SetStat(i,c-j,csGood);
                    iCChipVf[i][c] = true;
                }
                else {
                    DM.ARAY[aiWhere].SetStat(i,c-j,csFail);
                    iCChipVf[i][c] = false;
                }
            }
        }

        if(OM.DevOptn.bUseZener )
        {
            for(int j=0; j<2; j++ ){
                for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --)
                {
                    if(RcvData == "\r" || !RcvData.Pos("Z")) return ;
                    iPos = RcvData.Pos("Z") + 1;
                    RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
                    sTemp = RcvData;
                    sTemp = sTemp.SubString(1,4);
                    dCChipVr[i][c-j] = 0.0012 * sTemp.ToIntDef(0) * 3;
                    if(OM.DevOptn.d1ChLow   <= dCChipVf[i][c-j] && dCChipVf[i][c-j] <= OM.DevOptn.d1ChHigh &&
                       OM.DevOptn.dZenerLow <= dCChipVr[i][c-j] && dCChipVr[i][c-j] <= OM.DevOptn.dZenerHigh ) {
                        DM.ARAY[aiWhere].SetStat(i,c-j,csGood);
                        iCChipVr[i][c] = true;
                    }
                    else {
                        DM.ARAY[aiWhere].SetStat(i,c-j,csFail);
                        iCChipVr[i][c] = false;
                    }
                }
            }
        }
    }
    return ;

}   */


void __fastcall CProbeComUnit::DelPastData()
{
    TUserFile UserFile ;
    TDateTime Time ;
    Time = Now() ;
    Time.Val = Time.Val - 30 * 3 ; //30�� ���� ������ ����.

    UserFile.ClearDirDate(LOT_LOG_FOLDER , Time) ;
}
void __fastcall CProbeComUnit::WritePastProbeDataCsv(AnsiString sPath)
{
    //Local Var.
    int        hFile       ;
    AnsiString Path        ;
    AnsiString Temp ,sTime ;
    TDateTime  CurrDateTime;
    AnsiString sTemp       ;
    int        iMgzCnt     ;

    //File Open.
    hFile = FileOpen(sPath.c_str() , fmOpenWrite);

    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) return;
//        FileSeek (hFile , 0            , SEEK_END     );
//        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }



    Temp  = "\r\n==================================================================================================\r\n";
    Temp += "Frame ID,";
    Temp += sTemp + ",";
    Temp += "Finish Time,";
    Temp += CurrDateTime.FormatString("yyyy_mm_dd hh:nn:ss,") ;
    Temp += "\r\n"   ;

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());
    //Save.

    for(int k=0; k< OM.DevOptn.iDgChipCnt; k++ )
    {
        Temp.sprintf("VF CHIP%02d \r\n",k+1);
        for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
            for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
                Temp += (AnsiString)dPChipVf[r][c][k] + "," ;
            }
            Temp += "\r\n" ;
        }
        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());

        Temp.sprintf("VR CHIP%02d \r\n",k+1);
        for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
            for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
                Temp += (AnsiString)dPChipVr[r][c][k] + "," ;
            }
            Temp += "\r\n" ;
        }
        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }

    //Close File.
    FileClose(hFile);
}
void __fastcall CProbeComUnit::WriteProbeDataCsv(EN_ARAY_ID _iId)
{
    //Local Var.
    int        hFile       ;
    AnsiString Path        ;
    AnsiString Temp ,sTime ;
    TDateTime  CurrDate    ;
    TDateTime  CurrTime    ;
    AnsiString sTemp       ;
    int        iMgzCnt     ;

    DelPastData();

    sTemp         = CurrDate.CurrentDate().FormatString("yyyymmdd");
    Path         = LOT_LOG_FOLDER + sTemp ;

    if(DM.ARAY[_iId].GetLotNo() != "")  Path += "\\" + DM.ARAY[_iId].GetLotNo() + "\\Log";//   LT.GetCrntLot() ;
    else                                Path += "\\NoLot"                                ;

    if(!DirectoryExists(Path)) CreateDir(Path);

    iMgzCnt = DM.ARAY[riWRK].GetID().ToIntDef(9999) / 100;

    if(LT.m_bLotOpen) Path += "\\" + (AnsiString)iMgzCnt + ".csv" ;
    else              Path += "\\" + sTime   + ".csv" ;

    //File Open.
    hFile = FileOpen(Path.c_str() , fmOpenWrite);

    if (hFile == -1) {
        hFile = FileCreate(Path.c_str());
        if (hFile == -1) return;
        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }



    Temp  = "\r\n==================================================================================================\r\n";
    Temp += "Frame ID,";
    Temp += sTemp + ",";
    Temp += "Finish Time,";
    Temp += Now().FormatString("yyyy_mm_dd hh:nn:ss,") ;
    Temp += "LotNo,";
    Temp += DM.ARAY[_iId].GetLotNo() ;
    Temp += ",MgzNo,";
    Temp += DM.ARAY[_iId].GetID().ToIntDef(9999)/100 ;
    Temp += ",SlotNo,";
    Temp += DM.ARAY[_iId].GetID().ToIntDef(99)%100 ;

    Temp += "\r\n"   ;

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());
    //Save.

    for(int k=0; k< OM.DevOptn.iDgChipCnt; k++ )
    {
        Temp.sprintf("VF CHIP%02d \r\n",k+1);
//        Temp. = "Vf\r\n";
        for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
            for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
                Temp += (AnsiString)dCChipVf[r][c][k] + "," ;
            }
            Temp += "\r\n" ;
        }
        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());

        Temp.sprintf("VR CHIP%02d \r\n",k+1);
//        Temp = "Vr\r\n";
        for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
            for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
                Temp += (AnsiString)dCChipVr[r][c][k] + "," ;
            }
            Temp += "\r\n" ;
        }
        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());

        Temp = "Result\r\n";
        for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
            for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
                //Temp += (AnsiString)(DM.ARAY[_iId].GetStat(r,c)==csWork ? "OK" : "NG") + "," ;
                     if(iCChipRslt[r][c][k] == prReady ) sTemp = "READY" ;
                else if(iCChipRslt[r][c][k] == prOk    ) sTemp = "     " ;
                else if(iCChipRslt[r][c][k] == prVFNg  ) sTemp = "VF   " ;
                else if(iCChipRslt[r][c][k] == prVRNg  ) sTemp = "VR   " ;
                else if(iCChipRslt[r][c][k] == prVFVRNg) sTemp = "NG   " ;
                else if(iCChipRslt[r][c][k] == prDRNg  ) sTemp = "DR   " ;

                Temp += sTemp + "," ;


            }
            Temp += "\r\n" ;
        }

        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }

    //Close File.
    FileClose(hFile);
}
void __fastcall CProbeComUnit::WriteProbeDataIni(EN_ARAY_ID _iId)
{
/*    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    AnsiString sTemp2 ;
    AnsiString sTemp3 ;
    AnsiString sTime  ;
    int        iMgzCnt;
    int        iSltCnt;
    TDateTime  tLotStartTime;
    TDateTime  CurrDateTime;
//    DelPastLog();

    //Set Dir.
    //Set Path.
    CurrDateTime = Now();
    sTime = CurrDateTime.FormatString("hhnnss");

    sTemp         = CurrDateTime.CurrentDate().FormatString("yyyymmdd");

    sPath         = LOT_LOG_FOLDER + sTemp ;

    if(LT.m_bLotOpen)  sPath += "\\" + DM.ARAY[_iId].GetLotNo() + "\\Log";//   LT.GetCrntLot() ;
    else               sPath += "\\NoLot"                      ;

    if(!DirectoryExists(sPath)) CreateDir(sPath);

    iMgzCnt = DM.ARAY[riWRK].GetID().ToIntDef(9999) / 100;

    if(LT.m_bLotOpen) sPath += "\\" + (AnsiString)iMgzCnt + ".ini" ;
    else               sPath += "\\" + sTime   + ".ini" ;


//    sTime = Now().FormatString("hh:nn:ss") ;
//    sTime   = CurrDateTime.CurrentTime().FormatString("hh:nn:ss");

    AnsiString sVf,sVr;
    AnsiString sRslt1,sRslt2 ;
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            sVf = dCChipVf[r][c] ;
            sVr = dCChipVr[r][c] ;
            if(OM.DevOptn.bUseZener) sRslt1 += sVf + "|" + sVr + "_" ;
            else                     sRslt1 += sVr +             "_" ;
        }
    }
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            if(sVf == 1) sVf = "OK" ;
            else         sVf = "NG" ;
            if(sVr == 1) sVr = "OK" ;
            else         sVr = "NG" ;
//            sVf = iCC hipVf[r][c] ? "OK" : "NG" ;
//            sVr = iCChipVr[r][c] ? "OK" : "NG" ;
            if(OM.DevOptn.bUseZener) sRslt2 += sVf + "|" + sVr + "_" ;
            else                     sRslt2 += sVf +             "_" ;
        }
    }

    iSltCnt = DM.ARAY[riWRK].GetID().ToIntDef(99) % 100;

    UserINI.Save(sPath.c_str()  , iSltCnt , "Rst1" , sRslt1 );
    UserINI.Save(sPath.c_str()  , iSltCnt , "Rst2" , sRslt2 );
*/
}

void __fastcall CProbeComUnit::ReadProbeData(AnsiString sPath)
{
//    TUserINI   UserINI;

//    if(!sPath.Pos("NoLot")) sPath.Pos(LOT_LOG_FOLDER)

//    UserINI.(sPath.c_str()  , sTemp , iSltCnt , sRslt );

}

void __fastcall CProbeComUnit::Set()
{
/*
    //SET,12,0400,0100,15,T#
    AnsiString sMsg  = "" ;
    AnsiString sTemp ;
    int i1ChLedCur, i2ChLedCur , i1ChZenCur, i2ChZenCur ;
    i1ChLedCur = (OM.DevOptn.i1ChLedCur * 1000 ) / 50 ;
    i1ChZenCur = (OM.DevOptn.i1ChZenCur * 1000 ) / 50 ;

    i2ChLedCur = (OM.DevOptn.i2ChLedCur * 1000 ) / 50 ;
    i2ChZenCur = (OM.DevOptn.i2ChZenCur * 1000 ) / 50 ;

//    CDelayTimer tmTemp;
//    sMsg = "SET,12,0400,0100,15,T#";

    g_bPb1Rslt = false;
    sMsg = "SET,RLL0800#";
    RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
    while (1) {
        Application->ProcessMessages();
        if(g_bPb1Rslt) break ;
    }

    g_bPb1Rslt = false;
    sMsg = "SET,RLH1200#";
    RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
    while (1) {
        Application->ProcessMessages();
        if(g_bPb1Rslt) break ;
    }

    g_bPb1Rslt = false;
    if( OM.DevOptn.bUseZener ) {
        g_bPb1Rslt = false;
        sMsg = "SET,RZL0300#";
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        while (1) {
            Application->ProcessMessages();
            if(g_bPb1Rslt) break ;
        }
    }

    g_bPb1Rslt = false;
    if( OM.DevOptn.bUseZener ) {
        g_bPb1Rslt = false;
        sMsg = "SET,RZH0600#";
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        while (1) {
            Application->ProcessMessages();
            if(g_bPb1Rslt) break ;
        }
    }


    sMsg = "SET,";
    if( OM.DevOptn.i1Ch1P && OM.DevOptn.i1Ch1N ){ sMsg = sMsg + sTemp.sprintf("%01d%01d,",OM.DevOptn.i1Ch1P,OM.DevOptn.i1Ch1N ) ; }
    if( OM.DevOptn.i1Ch2P && OM.DevOptn.i1Ch2N ){ sMsg = sMsg + sTemp.sprintf("%01d%01d,",OM.DevOptn.i1Ch2P,OM.DevOptn.i1Ch2N ) ; }
    if( OM.DevOptn.i1Ch3P && OM.DevOptn.i1Ch3N ){ sMsg = sMsg + sTemp.sprintf("%01d%01d,",OM.DevOptn.i1Ch3P,OM.DevOptn.i1Ch3N ) ; }
    if( OM.DevOptn.i1ChLedCur                  ){ sMsg = sMsg + sTemp.sprintf("%04d,"    ,i1ChLedCur                          ) ; }
    if( OM.DevOptn.i1ChZenCur                  ){ sMsg = sMsg + sTemp.sprintf("%04d,"    ,i1ChZenCur                          ) ; }
    if( OM.DevInfo.iRowCnt                     ){ sMsg = sMsg + sTemp.sprintf("%02d,"    ,OM.DevInfo.iRowCnt                  ) ; }
    if( OM.DevOptn.bUseZener                   ){ sMsg = sMsg + "T#"                                                            ; }
    else                                        { sMsg = sMsg + "F#"                                                            ; }

    g_bPb1Rslt = false;
    RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
    while (1) {
        Application->ProcessMessages();
        if(g_bPb1Rslt) break ;
    }

    g_bPb2Rslt = false;
    sMsg = "SET,RLL0800#";
    RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
    while (1) {
        Application->ProcessMessages();
        if(g_bPb2Rslt) break ;
    }

    g_bPb2Rslt = false;
    sMsg = "SET,RLH1200#";
    RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
    while (1) {
        Application->ProcessMessages();
        if(g_bPb2Rslt) break ;
    }

    g_bPb2Rslt = false;
    if( OM.DevOptn.bUseZener ) {
        g_bPb1Rslt = false;
        sMsg = "SET,RZL0300#";
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        while (1) {
            Application->ProcessMessages();
            if(g_bPb2Rslt) break ;
        }
    }

    g_bPb2Rslt = false;
    if( OM.DevOptn.bUseZener ) {
        g_bPb1Rslt = false;
        sMsg = "SET,RZH0600#";
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        while (1) {
            Application->ProcessMessages();
            if(g_bPb2Rslt) break ;
        }
    }

    sMsg = "SET,";
    if( OM.DevOptn.i2Ch1P && OM.DevOptn.i2Ch1N ){ sMsg = sMsg + sTemp.sprintf("%01d%01d,",OM.DevOptn.i2Ch1P,OM.DevOptn.i2Ch1N ) ; }
    if( OM.DevOptn.i2Ch2P && OM.DevOptn.i2Ch2N ){ sMsg = sMsg + sTemp.sprintf("%01d%01d,",OM.DevOptn.i2Ch2P,OM.DevOptn.i2Ch2N ) ; }
    if( OM.DevOptn.i2Ch3P && OM.DevOptn.i2Ch3N ){ sMsg = sMsg + sTemp.sprintf("%01d%01d,",OM.DevOptn.i2Ch3P,OM.DevOptn.i2Ch3N ) ; }
    if( OM.DevOptn.i2ChLedCur                  ){ sMsg = sMsg + sTemp.sprintf("%04d,"    ,i2ChLedCur                          ) ; }
    if( OM.DevOptn.i2ChZenCur                  ){ sMsg = sMsg + sTemp.sprintf("%04d,"    ,i2ChZenCur                          ) ; }
    if( OM.DevInfo.iRowCnt                     ){ sMsg = sMsg + sTemp.sprintf("%02d,"    ,OM.DevInfo.iRowCnt                  ) ; }
    if( OM.DevOptn.bUseZener                   ){ sMsg = sMsg + "T#"                                                            ; }
    else                                        { sMsg = sMsg + "F#"                                                            ; }

    g_bPb2Rslt = false;
    RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
    while (1) {
        Application->ProcessMessages();
        if(g_bPb2Rslt) break ;
    }
*/
}
void __fastcall CProbeComUnit::SetDaegyeom ()
{
    AnsiString sMsg  = "" ;
    AnsiString sTemp ;
    int iLedChip = 0 ;
    int i1ChLedCur, i2ChLedCur , i1ChZenCur, i2ChZenCur ;

//    if( OM.DevOptn.i1Ch1P && OM.DevOptn.i1Ch1N ){ iLedChip = 1 ; }
//    if( OM.DevOptn.i1Ch2P && OM.DevOptn.i1Ch2N ){ iLedChip = 2 ; }
//    if( OM.DevOptn.i1Ch3P && OM.DevOptn.i1Ch3N ){ iLedChip = 3 ; } ȥ���� �ȵ�.
    iLedChip = OM.DevOptn.iDgChipCnt ;

    //i1ChLedCur = (OM.DevOptn.i1ChLedCur * 1000 ) / 50 ;
    //i1ChZenCur = (OM.DevOptn.i1ChZenCur * 1000 ) / 50 ;

    //i1ChLedCur = (OM.DevOptn.i1ChLedCur * 5 * 4095) / 2000 ;
    //i1ChZenCur = (OM.DevOptn.i1ChZenCur * 5 * 4095) / 2000 ;

    i1ChLedCur = (OM.DevOptn.i1ChLedCur * 0.01 * 4096) + (OM.DevOptn.i1ChLedCur * OM.MstOptn.d1chLedSetOfs   );
    i1ChZenCur = (OM.DevOptn.i1ChZenCur * 0.01 * 4096) + (OM.DevOptn.i1ChZenCur * OM.MstOptn.d1chZennerOffset);
    //(OM.DevOptn.i1ChLedCur * 0.9) = ���� �ɼ°�.
    //20mA�ΰ� �� 19.6���� ������ 1mA�� 0.9�ɼ��ΰ��� 20.05������ ����.JS


    //i2ChLedCur = (OM.DevOptn.i2ChLedCur * 1000 ) / 50 ;
    //i2ChZenCur = (OM.DevOp    tn.i2ChZenCur * 1000 ) / 50 ;

    //i2ChLedCur = (OM.DevOptn.i2ChLedCur * 5 * 4095) / 2000 ;
    //i2ChZenCur = (OM.DevOptn.i2ChZenCur * 5 * 4095) / 2000 ;

    i2ChLedCur = (OM.DevOptn.i2ChLedCur * 0.01 * 4096) + (OM.DevOptn.i2ChLedCur * OM.MstOptn.d2chLedSetOfs   );
    i2ChZenCur = (OM.DevOptn.i2ChZenCur * 0.01 * 4096) + (OM.DevOptn.i2ChZenCur * OM.MstOptn.d2chZennerOffset);

    //1Ch Setting
    if(OM.DevOptn.bUseZener) {
        sMsg = OM.DevOptn.bChRvsCrnt ? "PIN000T#" : "PIN111T#" ;                                                                // F / T
        sMsg +="CUL" + sTemp.sprintf("%04d",i1ChLedCur                       ) + "#" ;    // 0000-4095
        sMsg +="CUZ" + sTemp.sprintf("%04d",i1ChZenCur                       ) + "#" ;    // 0000-4095
        sMsg +="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt * (OM.DevOptn.iContColCnt /2) ) + "#" ;    // n=1,2,3 0001-0016
//        sMsg +="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt ) + "#" ;    // n=1,2,3 0001-0016

        if(OM.DevOptn.bSideLEDControl){
            sMsg +="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i1ChDelay         ) + "#" ;    // Delay
            sMsg +="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly     ) + "#" ;    // Delay
            sMsg +="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i1ChLedTime       ) + "#" ;    // 0000-9999
            sMsg +="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i1ChZenTime       ) + "#" ;    // 0000-9999
        }
        else {
            sMsg +="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i1ChDelay     * 10) + "#" ;    // Delay
            sMsg +="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly * 10) + "#" ;    // Delay
            sMsg +="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i1ChLedTime   * 10) + "#" ;    // 0000-9999
            sMsg +="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i1ChZenTime   * 10) + "#" ;    // 0000-9999
        }
        RS232C_Pb1ch->SendData(sMsg.Length(),sMsg.c_str());
    }
    else {
        sMsg = OM.DevOptn.bChRvsCrnt ? "PIN000F#" : "PIN111F#" ;                                                                //
        sMsg +="CUL" + sTemp.sprintf("%04d",i1ChLedCur                       ) + "#" ;    //
//�ؿ����
        sMsg +="CUZ" + sTemp.sprintf("%04d",i1ChZenCur                       ) + "#" ;  //
        sMsg +="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt * (OM.DevOptn.iContColCnt /2) ) + "#" ;    //
//        sMsg +="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt  ) + "#" ;    //

        if(OM.DevOptn.bSideLEDControl){
            sMsg +="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i1ChDelay         ) + "#" ;    //
            sMsg +="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly     ) + "#" ;    //
            sMsg +="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i1ChLedTime       ) + "#" ;    //
            //�ؿ����
            sMsg +="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChZenTime       ) + "#" ;  //
        }
        else {
            sMsg +="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i1ChDelay     * 10) + "#" ;    //
            sMsg +="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly * 10) + "#" ;    //
            sMsg +="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i1ChLedTime   * 10) + "#" ;    //
            //�ؿ����
            sMsg +="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChZenTime   * 10) + "#" ;  //
        }
        RS232C_Pb1ch->SendData(sMsg.Length(),sMsg.c_str());
    }

    //2Ch Setting
    if(OM.DevOptn.bUseZener) {
        sMsg = OM.DevOptn.bChRvsCrnt ? "PIN000T#" : "PIN111T#" ;                                                               //
        sMsg  +="CUL" + sTemp.sprintf("%04d",i2ChLedCur                       ) + "#" ;    //
        sMsg  +="CUZ" + sTemp.sprintf("%04d",i2ChZenCur                       ) + "#" ;    //
        sMsg  +="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt * (OM.DevOptn.iContColCnt /2) ) + "#" ;    //
//        sMsg  +="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt  ) + "#" ;    //
        if(OM.DevOptn.bSideLEDControl){
            sMsg  +="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i2ChDelay            ) + "#" ;    //
            sMsg  +="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly        ) + "#" ;    //
            sMsg  +="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChLedTime          ) + "#" ;    //
            sMsg  +="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChZenTime          ) + "#" ;    //
        }
        else{
            sMsg  +="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i2ChDelay     * 10   ) + "#" ;    //
            sMsg  +="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly * 10   ) + "#" ;    //
            sMsg  +="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChLedTime   * 10   ) + "#" ;    //
            sMsg  +="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChZenTime   * 10   ) + "#" ;    //
        }
        RS232C_Pb2ch->SendData(sMsg.Length(),sMsg.c_str());
    }
    else {
        sMsg = OM.DevOptn.bChRvsCrnt ? "PIN000F#" : "PIN111F#" ;                                                               //
        sMsg  +="CUL" + sTemp.sprintf("%04d",i2ChLedCur                       ) + "#" ;    //
//�ؿ����
        sMsg  +="CUZ" + sTemp.sprintf("%04d",i2ChZenCur                       ) + "#" ;  //
        sMsg  +="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt * (OM.DevOptn.iContColCnt /2) ) + "#" ;    //

//        sMsg  +="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt  ) + "#" ;    //
        if(OM.DevOptn.bSideLEDControl){
            sMsg  +="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i2ChDelay          ) + "#" ;    //
            sMsg  +="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly      ) + "#" ;    //
            sMsg  +="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChLedTime        ) + "#" ;    //
            //�ؿ����
            sMsg  +="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChZenTime        ) + "#" ;  //
        }
        else {
            sMsg  +="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i2ChDelay     * 10 ) + "#" ;    //
            sMsg  +="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly * 10 ) + "#" ;    //
            sMsg  +="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChLedTime   * 10 ) + "#" ;    //
            sMsg  +="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChZenTime   * 10 ) + "#" ;  //
        }

        RS232C_Pb2ch->SendData(sMsg.Length(),sMsg.c_str());
    }

/*
    int iSeep = 100 ;
    //1Ch Setting
    if(OM.DevOptn.bUseZener) {
        sMsg  ="PIN111T#" ;                                                                // F / T
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg ="CUL" + sTemp.sprintf("%04d",i1ChLedCur                       ) + "#" ;    // 0000-4095
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg ="CUZ" + sTemp.sprintf("%04d",i1ChZenCur                       ) + "#" ;    // 0000-4095
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg ="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt * (OM.DevOptn.iContColCnt /2) ) + "#" ;    // n=1,2,3 0001-0016
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg ="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i1ChDelay     * 10    ) + "#" ;    // Delay
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg ="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly * 10    ) + "#" ;    // Delay
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg ="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i1ChLedTime   * 10    ) + "#" ;    // 0000-9999
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg ="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i1ChZenTime   * 10    ) + "#" ;    // 0000-9999
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
    }
    else {
        sMsg  ="PIN111T#" ;                                                                //
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg  ="CUL" + sTemp.sprintf("%04d",i1ChLedCur                       ) + "#" ;    //
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
//        sMsg +="CUZ" + sTemp.sprintf("%04d",i1ChZenCur                       ) + "#" ;  //
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg  ="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt * (OM.DevOptn.iContColCnt /2) ) + "#" ;    //
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg  ="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i1ChDelay     * 10    ) + "#" ;    //
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg  ="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly * 10    ) + "#" ;    //
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
        sMsg  ="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i1ChLedTime   * 10    ) + "#" ;    //
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        ::Sleep(iSeep);
//        sMsg +="ZIT" + sTemp.sprintf("%04d,",OM.DevOptn.i1ChZenTime   * 10    ) + "#" ;  //
        ::Sleep(iSeep);
        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
    }

    //2Ch Setting
    if(OM.DevOptn.bUseZener) {
        sMsg  ="PIN111T#" ;                                                                //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="CUL" + sTemp.sprintf("%04d",i2ChLedCur                       ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="CUZ" + sTemp.sprintf("%04d",i2ChZenCur                       ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt * (OM.DevOptn.iContColCnt /2) ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i2ChDelay     * 10    ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly * 10    ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChLedTime   * 10    ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChZenTime   * 10    ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
    }
    else {
        sMsg  ="PIN111T#" ;                                                                //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="CUL" + sTemp.sprintf("%04d",i2ChLedCur                       ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
//        sMsg +="CUZ" + sTemp.sprintf("%04d,",i2ChZenCur                       ) + "#" ;  //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="NUM" + sTemp.sprintf("%01d%03d",iLedChip, OM.DevInfo.iRowCnt * (OM.DevOptn.iContColCnt /2) ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="DLY" + sTemp.sprintf("%04d",OM.DevOptn.i2ChDelay     * 10    ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="SDY" + sTemp.sprintf("%04d",OM.DevOptn.iSwitchingDly * 10    ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
        sMsg  ="LIT" + sTemp.sprintf("%04d",OM.DevOptn.i2ChLedTime   * 10    ) + "#" ;    //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
//        sMsg +="ZIT" + sTemp.sprintf("%04d",OM.DevOptn.i1ChZenTime   * 10    ) + "#" ;  //
        ::Sleep(iSeep);
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
    }
*/

}
void __fastcall CProbeComUnit::ClearDataCrnt()
{
    for(int r = 0 ; r < MAX_ARRAY_ROW ; r++ ) {
        for(int c = 0 ; c < MAX_CONTACT_COL ; c++) {
            for(int k= 0 ; k < MAX_CHIPCNT ; k++) {
                iCChipRsltCrnt[r][c][k] = prReady ;
            }
        }
    }

    memset(dCChipVfCrnt , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_CONTACT_COL * MAX_CHIPCNT) ;
    memset(dCChipVrCrnt , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_CONTACT_COL * MAX_CHIPCNT) ;

}

void __fastcall CProbeComUnit::ClearData()
{
    for(int r = 0 ; r < MAX_ARRAY_ROW ; r++ ) {
        for(int c = 0 ; c < MAX_ARRAY_COL ; c++) {
            for(int k= 0 ; k < MAX_CHIPCNT ; k++) {
                iCChipRslt[r][c][k] = prReady ;
            }
        }
    }

    memset(dCChipVf , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL * MAX_CHIPCNT) ;
    memset(dCChipVr , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL * MAX_CHIPCNT) ;

}

void __fastcall CProbeComUnit::ClearPstData()
{
    for(int r = 0 ; r < MAX_ARRAY_ROW ; r++ ) {
        for(int c = 0 ; c < MAX_ARRAY_COL ; c++) {
            for(int k= 0 ; k < MAX_CHIPCNT ; k++) {
                iPChipRslt[r][c][k] = -1 ;
            }
        }
    }

    memset(dPChipVf , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL * MAX_CHIPCNT) ;
    memset(dPChipVr , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL * MAX_CHIPCNT) ;
}

