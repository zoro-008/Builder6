//---------------------------------------------------------------------------

#ifndef SortCmnH
#define SortCmnH

//#include <WINDOWS.H>

#include "DataMan.h"
#include "UtilDefine.h"
//#include "PstnMan.h"
#include "OptionMan.h"




//---------------------------------------------------------------------------
bool ScmCheckAllStat(EN_ARAY_ID _iAray , EN_CHIP_STAT _iStat)
{

    const bool bPckrUsage[MAX_TOOL_SOCK_COL] = {!OM.CmnOptn.bSkip1stTol,
                                                !OM.CmnOptn.bSkip2ndTol,
                                                !OM.CmnOptn.bSkip3thTol,
                                                !OM.CmnOptn.bSkip4thTol};

    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        if(_iAray == riSTL) {
            if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(i,0)!=_iStat) return false ; //스테이지가 돌아가면 bPckrUsage[i]가 돌아간다.
        }
        else if(_iAray == riSTR){
            if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(MAX_TOOL_SOCK_COL-i-1,0)!=_iStat) return false ; //스테이지가 돌아가면 bPckrUsage[i]가 돌아간다.
        }
        else if(_iAray == riSTB){
            if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(0,MAX_TOOL_SOCK_COL-i-1)!=_iStat) return false ; //스테이지가 돌아가면 bPckrUsage[i]가 돌아간다.
        }
        else {
            if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(0,i)!=_iStat) return false ; //스테이지가 돌아가면 bPckrUsage[i]가 돌아간다.
        }


    }

    return true ;
}

int ScmGetCntStat(EN_ARAY_ID _iAray , EN_CHIP_STAT _iStat)
{

    const bool bPckrUsage[MAX_TOOL_SOCK_COL] = {!OM.CmnOptn.bSkip1stTol,
                                                !OM.CmnOptn.bSkip2ndTol,
                                                !OM.CmnOptn.bSkip3thTol,
                                                !OM.CmnOptn.bSkip4thTol};

    //4개짜리들 만 쓰는 함수이다. 스테이지 포켓들, 지그  , 픽커.
    //if(DM.ARAY[_iAray].GetMaxCol() != 4) return false ;
    //if(DM.ARAY[_iAray].GetMaxRow() != 1) return false ;
    int iCnt = 0 ;
    //for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
    //    if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(0,i)==_iStat) iCnt++ ;
    //}

    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        if(_iAray == riSTL) {
            if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(i,0)==_iStat) iCnt++ ;
        }
        else if(_iAray == riSTR){
            if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(MAX_TOOL_SOCK_COL-i-1,0)==_iStat) iCnt++ ;
        }
        else if(_iAray == riSTB){
            if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(0,MAX_TOOL_SOCK_COL-i-1)==_iStat) iCnt++ ;
        }
        else {
            if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(0,i)==_iStat) iCnt++ ;
        }
    }




    return iCnt ;
}

//---------------------------------------------------------------------------


#endif


