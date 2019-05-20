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
#include "FormMain.h"
#include "FormOperation.h"
#include "FormProbe.h"
#include "UserFile.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define LOT_LOG_FOLDER "d:\\LotLog\\"

CProbeComUnit PCM;

EN_PROB_RSLT iCChipRsltCrnt[MAX_ARRAY_ROW][MAX_CONTACT_COL] ;
double       dCChipVfCrnt  [MAX_ARRAY_ROW][MAX_CONTACT_COL] ;
double       dCChipVrCrnt  [MAX_ARRAY_ROW][MAX_CONTACT_COL] ;

EN_PROB_RSLT iCChipRslt[MAX_ARRAY_ROW][MAX_ARRAY_COL];
double       dCChipVf  [MAX_ARRAY_ROW][MAX_ARRAY_COL];
double       dCChipVr  [MAX_ARRAY_ROW][MAX_ARRAY_COL];

EN_PROB_RSLT iPChipRslt[MAX_ARRAY_ROW][MAX_ARRAY_COL];
double       dPChipVf  [MAX_ARRAY_ROW][MAX_ARRAY_COL];
double       dPChipVr  [MAX_ARRAY_ROW][MAX_ARRAY_COL];


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
    LT.WriteArayData (_iId);

//    WriteProbeDataIni(_iId);
    WriteProbeDataCsv(_iId);

    memcpy(iPChipRslt , iCChipRslt , sizeof(int   ) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
    memcpy(dPChipVf   , dCChipVf   , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL);
    memcpy(dPChipVr   , dCChipVr   , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL);

    ClearData ();
}
//---------------------------------------------------------------------------
void __fastcall CProbeComUnit::CChip (int iR,int iC,double dVf,double dVr)
{
    dCChipVf[iR][iC] = dVf ;
    dCChipVr[iR][iC] = dVr ;
}
//---------------------------------------------------------------------------
bool __fastcall CProbeComUnit::Start(int iCh)
{
    AnsiString sMsg  = "START#" ;

    if     (iCh == 1) {
        g_sPb1Msg = "";
//        g_sPb2Msg = "";

        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
//       RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
    }
    else if(iCh == 2) {
//        g_sPb1Msg = "";
        g_sPb2Msg = "";

//        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
    }
    else              {
        g_sPb1Msg = "";
        g_sPb2Msg = "";

        RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
        RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
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
    bool       bVfRslt = false ;
    bool       bVrRslt = false ;
    for(int j=0; j>-OM.DevOptn.iContColCnt / 2; j-- ){
        for(int i = OM.DevInfo.iRowCnt - 1 ; i >= 0 ; i --) {
            //LED Cnt===========================================
            //RcvData.Delete(RcvData.Pos("L") , 1);
            sChipData = RcvData.SubString(RcvData.Pos("L")+1,4); //숫자 데이터만 짤라냄.
            RcvData.Delete(RcvData.Pos("L"),5);

            if(_iCh == PROBE_CH1) {
                dChipData = 0.0012 * sChipData.ToIntDef(-1) * 3;
                if(OM.MstOptn.d1chLedGain)
                    dChipData = dChipData * OM.MstOptn.d1chLedGain   ;
                dChipData = dChipData + OM.MstOptn.d1chLedOffset ;
                dCChipVfCrnt[i][OM.DevOptn.iContColCnt-1 + j] = dChipData ;
            }
            if(_iCh == PROBE_CH2) {
                dChipData = 0.0012 * sChipData.ToIntDef(-1) * 3;
                if(OM.MstOptn.d2chLedGain)
                    dChipData = dChipData * OM.MstOptn.d2chLedGain   ;
                dChipData = dChipData + OM.MstOptn.d2chLedOffset ;
                dCChipVfCrnt[i][OM.DevOptn.iContColCnt/2 - 1 + j] = dChipData ;

            }

            //LED Data Cal.
            if(iCChipRslt[i][c+j] != prOk || _bRpt) {
                dCChipVf[i][c+j] = dChipData ;
            }

//            if(iCChipRslt[i][c+j] != prOk)dCChipVf[i][c+j] = 0.0012 * sChipData.ToIntDef(-1) * 3;
            bVfRslt = OM.DevOptn.d1ChLow   <= dCChipVf[i][c+j] && dCChipVf[i][c+j] <= OM.DevOptn.d1ChHigh   ;

            //Zenner Cnt===========================================
            bVrRslt = true ;
            if(OM.DevOptn.bUseZener) {
                //RcvData.Delete(RcvData.Pos("Z") , 1);
                sChipData = RcvData.SubString(RcvData.Pos("Z")+1,4); //숫자 데이터만 짤라냄.
                RcvData.Delete(RcvData.Pos("Z"),5);

                if(_iCh == PROBE_CH1) {
                    dChipData = 0.0012 * sChipData.ToIntDef(-1) * 3;
                    if(OM.MstOptn.d1chZennerGain)
                        dChipData = dChipData * OM.MstOptn.d1chZennerGain   ;
                    dChipData = dChipData + OM.MstOptn.d1chZennerOffset ;
                    dCChipVrCrnt[i][OM.DevOptn.iContColCnt-1 + j] = dChipData ;
                }
                if(_iCh == PROBE_CH2) {
                    dChipData = 0.0012 * sChipData.ToIntDef(-1) * 3;
                    if(OM.MstOptn.d2chZennerGain)
                        dChipData = dChipData * OM.MstOptn.d2chZennerGain   ;
                    dChipData = dChipData + OM.MstOptn.d2chZennerOffset ;
                    dCChipVrCrnt[i][OM.DevOptn.iContColCnt/2 - 1 + j] = dChipData ;
                }

                //Zenner Data Cal.
                if(iCChipRslt[i][c+j] != prOk || _bRpt) {
                    dCChipVr[i][c+j] = dChipData ;
                }

                /*
                if(iCChipRslt[i][c+j] != prOk) {
                    if(_iCh = PROBE_CH1) {
                        dCChipVr[i][c+j] = 0.0012 * sChipData.ToIntDef(-1) * 3;
                        if(OM.MstOptn.d1chZennerGain) dCChipVr[i][c+j] = dCChipVr[i][c+j] * OM.MstOptn.d1chZennerGain ;
                        dCChipVr[i][c+j] = dCChipVr[i][c+j] + OM.MstOptn.d1chZennerOffset ;

                        dCChipVrCrnt[i][OM.DevOptn.iContColCnt-1 + j] = dCChipVr[i][c+j] ;
                    }

                    if(_iCh = PROBE_CH2) {
                        dCChipVr[i][c+j] = 0.0012 * sChipData.ToIntDef(-1) * 3;
                        if(OM.MstOptn.d2chZennerGain) dCChipVr[i][c+j] = dCChipVr[i][c+j] * OM.MstOptn.d2chZennerGain ;
                        dCChipVr[i][c+j] = dCChipVr[i][c+j] + OM.MstOptn.d2chZennerOffset ;

                        dCChipVrCrnt[i][OM.DevOptn.iContColCnt/2 - 1 + j] = dCChipVr[i][c+j] ;
                    }
                }*/

                bVrRslt = OM.DevOptn.dZenerLow <= dCChipVr[i][c+j] && dCChipVr[i][c+j] <= OM.DevOptn.dZenerHigh ;
            }

                 if ( bVfRslt &&  bVrRslt) {iCChipRslt[i][c+j] = prOk     ;  }
            else if (!bVfRslt &&  bVrRslt) {iCChipRslt[i][c+j] = prVFNg   ;  }
            else if ( bVfRslt && !bVrRslt) {iCChipRslt[i][c+j] = prVRNg   ;  }
            else if (!bVfRslt && !bVrRslt) {iCChipRslt[i][c+j] = prVFVRNg ;  }

            /*
            if(_iCh = PROBE_CH1) {
                iCChipRsltCrnt[i][OM.DevOptn.iContColCnt-1 + j] = iCChipRslt[i][c+j] ;
            }

            else {
                iCChipRsltCrnt[i][OM.DevOptn.iContColCnt/2 - 1 + j] = iCChipRslt[i][c+j] ;
            }
            */
            int iTemp = iCChipRslt[i][c+j] ;

            if(!OM.MstOptn.bHeattingTest){
                if(iCChipRslt[i][c+j] == prOk) DM.ARAY[aiWhere].SetStat(i,c+j,csGood);
                else                           DM.ARAY[aiWhere].SetStat(i,c+j,csFail);
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


            //LED Cnt
            if(!RcvData.Pos("L")) return false ;   //칩보다 데이터가 적은 경우
            RcvData.Delete(RcvData.Pos("L") , 1);

            //Zener Cnt
            if(OM.DevOptn.bUseZener) {
                if(!RcvData.Pos("Z")) return false ; //칩보다 데이터가 적은 경우
                RcvData.Delete(RcvData.Pos("Z") , 1);
            }
        }
    }
    if(                        RcvData.Pos("L")) return false ; //칩보다 데이터가 많은경우.
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
    Time.Val = Time.Val - 30 ; //30일 이전 데이터 지움.

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

    Temp = "Vf\r\n";
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            Temp += (AnsiString)dPChipVf[r][c] + "," ;
        }
        Temp += "\r\n" ;
    }
    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    Temp = "Vr\r\n";
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            Temp += (AnsiString)dPChipVr[r][c] + "," ;
        }
        Temp += "\r\n" ;
    }
    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

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

    Temp = "Vf\r\n";
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            Temp += (AnsiString)dCChipVf[r][c] + "," ;
        }
        Temp += "\r\n" ;
    }
    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    Temp = "Vr\r\n";
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            Temp += (AnsiString)dCChipVr[r][c] + "," ;
        }
        Temp += "\r\n" ;
    }
    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    Temp = "Result\r\n";
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //Temp += (AnsiString)(DM.ARAY[_iId].GetStat(r,c)==csWork ? "OK" : "NG") + "," ;
                 if(iCChipRslt[r][c] == prReady ) sTemp = "READY" ;
            else if(iCChipRslt[r][c] == prOk    ) sTemp = "     " ;
            else if(iCChipRslt[r][c] == prVFNg  ) sTemp = "VF   " ;
            else if(iCChipRslt[r][c] == prVRNg  ) sTemp = "VR   " ;
            else if(iCChipRslt[r][c] == prVFVRNg) sTemp = "NG   " ;

            Temp += sTemp + "," ;


        }
        Temp += "\r\n" ;
    }

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);
}
void __fastcall CProbeComUnit::WriteProbeDataIni(EN_ARAY_ID _iId)
{
    //Local Var.
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
    else              sPath += "\\" + sTime   + ".ini" ;


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
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {       //이상한짓 하네 이놈...쓰지 말자...그냥...
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

void __fastcall CProbeComUnit::ClearDataCrnt()
{
    for(int r = 0 ; r < MAX_ARRAY_ROW ; r++ ) {
        for(int c = 0 ; c < MAX_CONTACT_COL ; c++) {
            iCChipRsltCrnt[r][c] = prReady ;
        }
    }

    memset(dCChipVfCrnt , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_CONTACT_COL) ;
    memset(dCChipVrCrnt , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_CONTACT_COL) ;

}

void __fastcall CProbeComUnit::ClearData()
{
    for(int r = 0 ; r < MAX_ARRAY_ROW ; r++ ) {
        for(int c = 0 ; c < MAX_ARRAY_COL ; c++) {
            iCChipRslt[r][c] = prReady ;
        }
    }

    memset(dCChipVf , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL) ;
    memset(dCChipVr , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL) ;

}

void __fastcall CProbeComUnit::ClearPstData()
{
    for(int r = 0 ; r < MAX_ARRAY_ROW ; r++ ) {
        for(int c = 0 ; c < MAX_ARRAY_COL ; c++) {
            iPChipRslt[r][c] = -1 ;
        }
    }

    memset(dPChipVf , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL) ;
    memset(dPChipVr , 0 , sizeof(double) * MAX_ARRAY_ROW * MAX_ARRAY_COL) ;
}
