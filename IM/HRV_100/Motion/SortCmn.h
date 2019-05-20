//---------------------------------------------------------------------------

#ifndef SortCmnH
#define SortCmnH

//#include <WINDOWS.H>

#include "DataMan.h"
#include "UtilDefine.h"
//#include "PstnMan.h"
#include "OptionMan.h"
//#include "SMInterfaceUnit.h"

//enum EN_STG_ANGLE {
//    saNone =-1 ,
//    sa0    = 0 ,
//    sa90   = 1 ,
//    sa180  = 2 ,
//    sa270  = 3
//};
//
//
//
//EN_STG_ANGLE ScmGetStgAngle()
//{
//         if(PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+  0-45 <= MT_GetCmdPos(miSTG_TTble) && MT_GetCmdPos(miSTG_TTble) < PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+  0+45) return sa0    ;
//    else if(PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+ 90-45 <= MT_GetCmdPos(miSTG_TTble) && MT_GetCmdPos(miSTG_TTble) < PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+ 90+45) return sa90   ;
//    else if(PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+180-45 <= MT_GetCmdPos(miSTG_TTble) && MT_GetCmdPos(miSTG_TTble) < PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+180+45) return sa180  ;
//    else if(PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+270-45 <= MT_GetCmdPos(miSTG_TTble) && MT_GetCmdPos(miSTG_TTble) < PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+270+45) return sa270  ;
//    else if(PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+360-45 <= MT_GetCmdPos(miSTG_TTble) && MT_GetCmdPos(miSTG_TTble) < PM.GetValue(miSTG_TTble , pvSTG_TTbleWork)+360+45) return sa0    ;
//    else                                                                                                                                                                     return saNone ;
//}
//
//EN_ARAY_ID ScmGetFrntSTGAray()
//{
//    EN_ARAY_ID iArray;
//
//         if(sa0   == ScmGetStgAngle()) iArray = riSG1;
//    else if(sa90  == ScmGetStgAngle()) iArray = riSG4;
//    else if(sa180 == ScmGetStgAngle()) iArray = riSG3;
//    else if(sa270 == ScmGetStgAngle()) iArray = riSG2;
//
//    return iArray ;
//}
//
////---------------------------------------------------------------------------
//EN_ARAY_ID ScmGetLeftSTGAray()
//{
//    EN_ARAY_ID iArray;
//
//         if(sa0   == ScmGetStgAngle()) iArray = riSG2;
//    else if(sa90  == ScmGetStgAngle()) iArray = riSG1;
//    else if(sa180 == ScmGetStgAngle()) iArray = riSG4;
//    else if(sa270 == ScmGetStgAngle()) iArray = riSG3;
//
//    return iArray ;
//}
//
////---------------------------------------------------------------------------
//EN_ARAY_ID ScmGetRearSTGAray()
//{
//    EN_ARAY_ID iArray;
//
//         if(sa0   == ScmGetStgAngle()) iArray = riSG3;
//    else if(sa90  == ScmGetStgAngle()) iArray = riSG2;
//    else if(sa180 == ScmGetStgAngle()) iArray = riSG1;
//    else if(sa270 == ScmGetStgAngle()) iArray = riSG4;
//
//    return iArray ;
//}
//
////---------------------------------------------------------------------------
//EN_ARAY_ID ScmGetRghtSTGAray()
//{
//
//    EN_ARAY_ID iArray;
//
//         if(sa0   == ScmGetStgAngle()) iArray = riSG4;
//    else if(sa90  == ScmGetStgAngle()) iArray = riSG3;
//    else if(sa180 == ScmGetStgAngle()) iArray = riSG2;
//    else if(sa270 == ScmGetStgAngle()) iArray = riSG1;
//
//    return iArray ;
//}
//
//
//
//
////해당 포켓이 앞쪽 에 있다고 생각 하고 때려 넣으면 현재 각도에 따라 알아서 들어감.
//EN_CHIP_STAT ScmGetSckStat(EN_ARAY_ID _iAray , int r , int c)
//{
//    int ni ; //normal index
//    int ri ; //Reverse index.
//
//    if(_iAray == riSG1) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(0,ni);
//        else if(sa90  == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(ni,0);
//        else if(sa180 == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(0,ri);
//        else if(sa270 == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(ri,0);
//    }
//    else if(_iAray == riSG2) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(ni,0);
//        else if(sa90  == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(0,ri);
//        else if(sa180 == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(ri,0);
//        else if(sa270 == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(0,ni);
//    }
//    else if(_iAray == riSG3) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(0,ri);
//        else if(sa90  == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(ri,0);
//        else if(sa180 == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(0,ni);
//        else if(sa270 == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(ni,0);
//    }
//    else if(_iAray == riSG4) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(ri,0);
//        else if(sa90  == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(0,ni);
//        else if(sa180 == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(ni,0);
//        else if(sa270 == ScmGetStgAngle())return DM.ARAY[_iAray].GetStat(0,ri);
//    }
//
//    return csNone ;
//
//}
//
////해당 포켓이 앞쪽 에 있다고 생각 하고 때려 넣으면 현재 각도에 따라 알아서 들어감.
//bool ScmSetSckStat(EN_ARAY_ID _iAray , int r , int c , EN_CHIP_STAT _iStat)
//{
//    int ni ; //normal index
//    int ri ; //Reverse index.
//
//    if(_iAray == riSG1) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(0,ni,_iStat);
//        else if(sa90  == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(ni,0,_iStat);
//        else if(sa180 == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(0,ri,_iStat);
//        else if(sa270 == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(ri,0,_iStat);
//    }
//    else if(_iAray == riSG2) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(ni,0,_iStat);
//        else if(sa90  == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(0,ri,_iStat);
//        else if(sa180 == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(ri,0,_iStat);
//        else if(sa270 == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(0,ni,_iStat);
//    }
//    else if(_iAray == riSG3) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(0,ri,_iStat);
//        else if(sa90  == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(ri,0,_iStat);
//        else if(sa180 == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(0,ni,_iStat);
//        else if(sa270 == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(ni,0,_iStat);
//    }
//    else if(_iAray == riSG4) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(ri,0,_iStat);
//        else if(sa90  == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(0,ni,_iStat);
//        else if(sa180 == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(ni,0,_iStat);
//        else if(sa270 == ScmGetStgAngle()) DM.ARAY[_iAray].SetStat(0,ri,_iStat);
//    }
//    return csNone ;
//}
//
////해당 포켓이 앞쪽 에 있다고 생각 하고 때려 넣으면 현재 각도에 따라 알아서 들어감.
//bool ScmSetSckSubStat(EN_ARAY_ID _iAray , int r , int c , int idx , EN_CHIP_STAT _iStat)
//{
//    int ni ; //normal index
//    int ri ; //Reverse index.
//
//    if(_iAray == riSG1) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(0,ni,idx,_iStat);
//        else if(sa90  == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(ni,0,idx,_iStat);
//        else if(sa180 == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(0,ri,idx,_iStat);
//        else if(sa270 == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(ri,0,idx,_iStat);
//    }
//    else if(_iAray == riSG2) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(ni,0,idx,_iStat);
//        else if(sa90  == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(0,ri,idx,_iStat);
//        else if(sa180 == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(ri,0,idx,_iStat);
//        else if(sa270 == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(0,ni,idx,_iStat);
//    }
//    else if(_iAray == riSG3) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(0,ri,idx,_iStat);
//        else if(sa90  == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(ri,0,idx,_iStat);
//        else if(sa180 == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(0,ni,idx,_iStat);
//        else if(sa270 == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(ni,0,idx,_iStat);
//    }
//    else if(_iAray == riSG4) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(ri,0,idx,_iStat);
//        else if(sa90  == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(0,ni,idx,_iStat);
//        else if(sa180 == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(ni,0,idx,_iStat);
//        else if(sa270 == ScmGetStgAngle()) DM.ARAY[_iAray].SetSubStat(0,ri,idx,_iStat);
//    }
//    return true ;
//}
//
//
////해당 포켓이 앞쪽 에 있다고 생각 하고 때려 넣으면 현재 각도에 따라 알아서 들어감.
//EN_CHIP_STAT ScmGetSckSubStat(EN_ARAY_ID _iAray , int r , int c , int idx)
//{
//    int ni ; //normal index
//    int ri ; //Reverse index.
//
//    if(_iAray == riSG1) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(0,ni,idx);
//        else if(sa90  == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(ni,0,idx);
//        else if(sa180 == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(0,ri,idx);
//        else if(sa270 == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(ri,0,idx);
//    }
//    else if(_iAray == riSG2) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(ni,0,idx);
//        else if(sa90  == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(0,ri,idx);
//        else if(sa180 == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(ri,0,idx);
//        else if(sa270 == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(0,ni,idx);
//    }
//    else if(_iAray == riSG3) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(0,ri,idx);
//        else if(sa90  == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(ri,0,idx);
//        else if(sa180 == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(0,ni,idx);
//        else if(sa270 == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(ni,0,idx);
//    }
//    else if(_iAray == riSG4) {
//        ni = c ;
//        ri = MAX_TOOL_SOCK_COL - ni - 1 ;
//             if(sa0   == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(ri,0,idx);
//        else if(sa90  == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(0,ni,idx);
//        else if(sa180 == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(ni,0,idx);
//        else if(sa270 == ScmGetStgAngle())return DM.ARAY[_iAray].GetSubStat(0,ri,idx);
//    }
//    return csNone ;
//}
//
//bool ScmGetSckSubFail(EN_ARAY_ID _iAray, int r , int c , int idx )
//{
//    if(ScmGetSckSubStat(_iAray ,r,c ,idx)==csRslt1) return true ;
//    if(ScmGetSckSubStat(_iAray ,r,c ,idx)==csRslt2) return true ;
//    if(ScmGetSckSubStat(_iAray ,r,c ,idx)==csRslt3) return true ;
//    if(ScmGetSckSubStat(_iAray ,r,c ,idx)==csRslt4) return true ;
//    if(ScmGetSckSubStat(_iAray ,r,c ,idx)==csRslt5) return true ;
//    if(ScmGetSckSubStat(_iAray ,r,c ,idx)==csRslt6) return true ;
//    if(ScmGetSckSubStat(_iAray ,r,c ,idx)==csRslt7) return true ;
//    if(ScmGetSckSubStat(_iAray ,r,c ,idx)==csRslt8) return true ;
//    return false ;
//}
//
//







//---------------------------------------------------------------------------
bool ScmCheckAllStat(EN_ARAY_ID _iAray , EN_CHIP_STAT _iStat)
{
    
    const bool bPckrUsage[MAX_TOOL_SOCK_COL] = {!OM.CmnOptn.bSkip1stTol,
                                                !OM.CmnOptn.bSkip2ndTol,
                                                !OM.CmnOptn.bSkip3thTol,
                                                !OM.CmnOptn.bSkip4thTol};

    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(0,i)!=_iStat) return false ; //스테이지가 돌아가면 bPckrUsage[i]가 돌아간다.
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
    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        if(bPckrUsage[i] && DM.ARAY[_iAray].GetStat(0,i)==_iStat) iCnt++ ;
    }

    return iCnt ;
}

//---------------------------------------------------------------------------


#endif


