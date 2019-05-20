//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ProbeComm.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "FormOperation.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CProbeComm PCM;

double dCChipVf[MAX_ARRAY_ROW][MAX_ARRAY_COL];
double dCChipVr[MAX_ARRAY_ROW][MAX_ARRAY_COL];
double dPChipVf[MAX_ARRAY_ROW][MAX_ARRAY_COL];
double dPChipVr[MAX_ARRAY_ROW][MAX_ARRAY_COL];
//---------------------------------------------------------------------------
__fastcall CProbeComm::CProbeComm(void)
{
    m_iMaxRow = MAX_ARRAY_ROW;
    m_iMaxCol = MAX_ARRAY_COL;
}
//---------------------------------------------------------------------------
__fastcall CProbeComm::~CProbeComm(void)
{

}
//---------------------------------------------------------------------------
void __fastcall CProbeComm::ShiftArrayData()
{
    memcpy(dPChipVf , dCChipVf , sizeof(double) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
    memcpy(dPChipVr , dCChipVr , sizeof(double) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
}
//---------------------------------------------------------------------------
void __fastcall CProbeComm::CChip (int iR,int iC,double dVf,double dVr)
{
    dCChipVf[iR][iC] = dVf ;
    dCChipVr[iR][iC] = dVr ;
}
//---------------------------------------------------------------------------
bool __fastcall CProbeComm::Start()
{
    //SET,12,0400,0100,15,T#
    AnsiString sMsg  = "START#" ;
    RS232C_Pb1ch->WriteData(sMsg.Length(),sMsg.c_str());
//    RS232C_Pb2ch->WriteData(sMsg.Length(),sMsg.c_str());
}
bool __fastcall CProbeComm::GetData1Ch(BYTE *bData , int iLen)
{
    BYTE TempBuff[500];
    AnsiString RcvData = "";
    AnsiString sTemp   = "";
    int iPos , c;

    for(int i=0; i<iLen; i++)
    {
        RcvData = RcvData + (char )bData[i];
    }


    if(RcvData == "OK\r") return true;

//    ShowMessage("return false");

    if( DM.ARAY[riWRK].FindFrstCol(csUnkwn)<0 ) return false;
    else c = DM.ARAY[riWRK].FindFrstCol(csUnkwn) ;

    for(int i=0; i<OM.DevInfo.iRowCnt; i++)
    {
        if(!RcvData.Pos("Z")) {
            if( RcvData == "\r" || !RcvData.Pos("L") ) return false;
        }
        iPos = RcvData.Pos("L") + 1;
        RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
        sTemp = RcvData;
        sTemp = sTemp.SubString(1,4);
        dCChipVf[i][c] = 0.0012 * sTemp.ToIntDef(0) * 3;
        if(OM.DevOptn.d1ChLow <= dCChipVf[i][c] <= OM.DevOptn.d2ChHigh ) DM.ARAY[riWRK].SetStat(i,c,csGood);
        else DM.ARAY[riWRK].SetStat(i,c,csFail);
    }

    if( OM.DevOptn.bUseZener )
    {
        for(int i=0; i<OM.DevInfo.iRowCnt; i++)
        {
            if(RcvData == "\r" || !RcvData.Pos("Z")) return false;
            iPos = RcvData.Pos("Z") + 1;
            RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
            sTemp = RcvData;
            sTemp = sTemp.SubString(1,4);
            dCChipVr[i][c] = 0.0012 * sTemp.ToIntDef(0) * 3;
            if(OM.DevOptn.d1ChLow <= dCChipVf[i][c] <= OM.DevOptn.d2ChHigh ) DM.ARAY[riWRK].SetStat(i,c,csGood);
            else DM.ARAY[riWRK].SetStat(i,c,csFail);
        }
    }
    return true;


//    RcvData.c_str();



//    memcpy(dPChipVf , dCChipVf , sizeof(double) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
//    memcpy(dPChipVr , dCChipVr , sizeof(double) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
}
//---------------------------------------------------------------------------
bool __fastcall CProbeComm::GetData2Ch(BYTE *bData , int iLen)
{
    BYTE TempBuff[500];
    AnsiString RcvData = "";
    AnsiString sTemp   = "";
    int iPos , c;

    for(int i=0; i<iLen; i++)
    {
        RcvData = RcvData + (char )bData[i];
    }


    if(RcvData == "OK\r") return true;

//    ShowMessage("return false");

    if( DM.ARAY[riWRK].FindFrstCol(csUnkwn)<1 ) return false;
    else c = DM.ARAY[riWRK].FindFrstCol(csUnkwn) -1;

    for(int i=0; i<OM.DevInfo.iRowCnt; i++)
    {
        if(RcvData == "\r" || !RcvData.Pos("L")) return false;
        iPos = RcvData.Pos("L") + 1;
        RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
        sTemp = RcvData;
        sTemp = sTemp.SubString(1,4);
        dCChipVf[i][c] = 0.0012 * sTemp.ToIntDef(0) * 3;
    }

    if( OM.DevOptn.bUseZener )
    {
        for(int i=0; i<OM.DevInfo.iRowCnt; i++)
        {
            if(RcvData == "\r" || !RcvData.Pos("Z")) return false;
            iPos = RcvData.Pos("Z") + 1;
            RcvData = RcvData.SubString(iPos, RcvData.Length() - (iPos -1));
            sTemp = RcvData;
            sTemp = sTemp.SubString(1,4);
            dCChipVr[i][c] = 0.0012 * sTemp.ToIntDef(0) * 3;
        }
    }
    return true;


//    RcvData.c_str();



//    memcpy(dPChipVf , dCChipVf , sizeof(double) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
//    memcpy(dPChipVr , dCChipVr , sizeof(double) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
}
