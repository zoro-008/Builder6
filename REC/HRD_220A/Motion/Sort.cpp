//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Sort.h"
#include "SortCmn.h"
#include "Loader.h"
#include "Stage.h"
#include "LoaderSupply.h"
#include "Sequence.h"

#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CSort SRT;

CSort::CSort(void)
{

}

CSort::~CSort (void)
{

}

bool CSort::GetTraySortInfo(bool _bPick , EN_ARAY_ID _iTool , EN_ARAY_ID _iAray , TSortInfo & _tInfo)
{
    int r , c ;
    int iPickerNo ;
    bool bRet ;
    memset(&_tInfo , 0 , sizeof(TSortInfo));
    _tInfo.iAray  = _iAray ;
    _tInfo.iTool  = _iTool ;

    const bool bPckrUsage[MAX_TOOL_SOCK_COL] = {!OM.CmnOptn.bSkip1stTol,
                                                !OM.CmnOptn.bSkip2ndTol,
                                                !OM.CmnOptn.bSkip3thTol,
                                                !OM.CmnOptn.bSkip4thTol};



    if(_bPick) { //로더 픽인포일 경우.
        _tInfo.isPick = true ;
        _tInfo.iTargetStat = csUnkwn ;

        bRet = FindChip(_tInfo.iAray , _tInfo.iTargetStat , _tInfo.iRow , _tInfo.iCol);

        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //툴에 칩이 들려 있는경우. 툴기준을 쉬프트 시킨다.
            if(bPckrUsage[i] && DM.ARAY[_tInfo.iTool].GetStat(0,i)==csEmpty)break ;
            _tInfo.iToolShft++;

        }

        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
            iPickerNo = i + _tInfo.iToolShft ;

            if(iPickerNo       >= MAX_TOOL_SOCK_COL          ) break ;
            if(_tInfo.iCol + i >= DM.ARAY[_iAray].GetMaxCol()) break ;

            if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==csEmpty                             ) _tInfo.bDn[iPickerNo] = true  ; //툴빈곳은 켜준다.
            if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)!=_tInfo.iTargetStat) _tInfo.bDn[iPickerNo] = false ; //집으려는 타겟이 아님.
            if(!bPckrUsage[iPickerNo]                                                          ) _tInfo.bDn[iPickerNo] = false ; //툴 안씀.
        }

    }
    else { //Place
        _tInfo.isPick = false ;

        //리젝빈의 위치가 이렇게 되어 있다 맨 왼쪽에 있는 것 부터 소팅.
             if(DM.ARAY[riSRT].GetCntStat(csRslt1))_tInfo.iTargetStat = csRslt1 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt5))_tInfo.iTargetStat = csRslt5 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt2))_tInfo.iTargetStat = csRslt2 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt6))_tInfo.iTargetStat = csRslt6 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt3))_tInfo.iTargetStat = csRslt3 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt7))_tInfo.iTargetStat = csRslt7 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt4))_tInfo.iTargetStat = csRslt4 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt8))_tInfo.iTargetStat = csRslt8 ;
        else if(DM.ARAY[riSRT].GetCntStat(csGood ))_tInfo.iTargetStat = csGood  ;

        bRet = FindChip(_tInfo.iAray , csEmpty , _tInfo.iRow , _tInfo.iCol);

        //왼쪽 기준.
        if(_tInfo.iTargetStat == csRslt1 || //리젝 뒤에 놈들은 얼라인 시에 빈틈이 없게 채워 넣기위해 bFindFrstChip를 이용한다.
           _tInfo.iTargetStat == csRslt2 ||
           _tInfo.iTargetStat == csRslt3 ||
           _tInfo.iTargetStat == csRslt4 ){
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //내려놓을 툴칩이 아닌경우.
                if(DM.ARAY[_tInfo.iTool].GetStat(0,i)==_tInfo.iTargetStat) break ;
                _tInfo.iToolShft++;
            }
            bool bFindFrstChip = false;
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
                iPickerNo = i + _tInfo.iToolShft ;
                if(iPickerNo       >= MAX_TOOL_SOCK_COL          ) break ;
                if(_tInfo.iCol + i >= DM.ARAY[_iAray].GetMaxCol()) break ;
                if(bFindFrstChip && DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)!=_tInfo.iTargetStat) break ; //중간에 빈곳 생기는 것 방지.

                if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==_tInfo.iTargetStat        ){_tInfo.bDn[iPickerNo] = true  ; bFindFrstChip = true ;}//툴 타겟인곳은 내림.
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)!=csEmpty ) _tInfo.bDn[iPickerNo] = false ; //집으려는 빈곳이 아님.
                if(!bPckrUsage[iPickerNo]                                                ) _tInfo.bDn[iPickerNo] = false ; //툴 안씀.
            }
        }
        //왼쪽기준
        else if(_tInfo.iTargetStat == csGood){ //Good은 일단 빈틈이 좀 생겨도 맨마지막에만 얼라인이 안될뿐이라 그냥 빈틈을 쓴다.
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //내려놓을 툴칩이 아닌경우.
                if(DM.ARAY[_tInfo.iTool].GetStat(0,i)==_tInfo.iTargetStat) break ;
                _tInfo.iToolShft++;
            }
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
                iPickerNo = i + _tInfo.iToolShft ;
                if(iPickerNo       >= MAX_TOOL_SOCK_COL          ) break ;
                if(_tInfo.iCol + i >= DM.ARAY[_iAray].GetMaxCol()) break ;

                if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==_tInfo.iTargetStat        ) _tInfo.bDn[iPickerNo] = true  ; //툴 타겟인곳은 내림.
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)!=csEmpty ) _tInfo.bDn[iPickerNo] = false ; //집으려는 빈곳이 아님.
                if(!bPckrUsage[iPickerNo]                                                ) _tInfo.bDn[iPickerNo] = false ; //툴 안씀.
            }
        }
        //오른쪽아래 기준.
        if(_tInfo.iTargetStat == csRslt5 ||
           _tInfo.iTargetStat == csRslt6 ||
           _tInfo.iTargetStat == csRslt7 ||
           _tInfo.iTargetStat == csRslt8 ){
            FindChip(_tInfo.iAray , csEmpty , _tInfo.iRow , _tInfo.iCol);

            int iTrayShift = MAX_TOOL_SOCK_COL - 1 ;
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
                if(DM.ARAY[_tInfo.iTool].GetStat(0 ,i)==_tInfo.iTargetStat) _tInfo.iToolShft=i ;
            }

            bool bFindFrstChip = false;
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
                iPickerNo = _tInfo.iToolShft - i;
                if(iPickerNo       < 0 ) continue ;
                if(_tInfo.iCol - i < 0 ) continue ;
                if(bFindFrstChip && DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)!=_tInfo.iTargetStat) break ; //중간에 빈곳 생기는 것 방지.

                if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==_tInfo.iTargetStat        ){_tInfo.bDn[iPickerNo] = true  ; bFindFrstChip = true;}//툴 타겟인곳은 내림.
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol - i)!=csEmpty ) _tInfo.bDn[iPickerNo] = false ; //집으려는 빈곳이 아님.
                if(!bPckrUsage[iPickerNo]                                                ) _tInfo.bDn[iPickerNo] = false ; //툴 안씀.
            }

        }
    }

    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //빈놈만 다운시킨다.
        if(_tInfo.bDn[i]) _tInfo.iDnCnt++;

    }

    if(_tInfo.iDnCnt && bRet) _tInfo.bFindOk = true ;

    return _tInfo.bFindOk ;
}

bool CSort::GetSockSortInfo(bool _bPick , EN_ARAY_ID _iTool , EN_ARAY_ID _iAray , TSortInfo & _tInfo)
{
    int r , c ;
    int iPickerNo ;
    bool bRet ;
    memset(&_tInfo , 0 , sizeof(TSortInfo));
    _tInfo.iAray  = _iAray ;
    _tInfo.iTool  = _iTool ;

    const bool bPckrUsage[MAX_TOOL_SOCK_COL] = {!OM.CmnOptn.bSkip1stTol,
                                                !OM.CmnOptn.bSkip2ndTol,
                                                !OM.CmnOptn.bSkip3thTol,
                                                !OM.CmnOptn.bSkip4thTol};

    if(_bPick) { //로더 픽인포일 경우.
        _tInfo.isPick = true ;
             if(_tInfo.iAray == riZIG) _tInfo.iTargetStat = csWork ;
        else if(_tInfo.iAray == riSTF) _tInfo.iTargetStat = csNone ; //골고루 찝어서 ;;; 설정이 없네 ;
        else if(_tInfo.iAray == riSTL) _tInfo.iTargetStat = csNone ;
        else if(_tInfo.iAray == riSTR) _tInfo.iTargetStat = csNone ;
        else if(_tInfo.iAray == riSTB) _tInfo.iTargetStat = csNone ;

        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
            iPickerNo = i ;

            if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==csEmpty                             ) _tInfo.bDn[iPickerNo] = true  ; //툴빈곳은 켜준다.
            if(_iAray == riZIG){
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)!=_tInfo.iTargetStat) _tInfo.bDn[iPickerNo] = false ; //집으려는 타겟이 아님.
            }
            else {
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)==csWork ) _tInfo.bDn[iPickerNo] = false ; //집으려는 타겟이 아님.
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)==csUnkwn) _tInfo.bDn[iPickerNo] = false ; //집으려는 타겟이 아님.
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)==csEmpty) _tInfo.bDn[iPickerNo] = false ; //집으려는 타겟이 아님.
            }

            if(!bPckrUsage[iPickerNo]                                                   ) _tInfo.bDn[iPickerNo] = false ; //툴 안씀.
        }
    }
    else { //Place
        _tInfo.isPick = false ;

             if(_tInfo.iAray == riZIG) _tInfo.iTargetStat = csUnkwn ;
        else if(_tInfo.iAray == riSTF) _tInfo.iTargetStat = csWork  ;
        else if(_tInfo.iAray == riSTL) _tInfo.iTargetStat = csWork  ;
        else if(_tInfo.iAray == riSTB) _tInfo.iTargetStat = csWork  ;
        else if(_tInfo.iAray == riSTR) _tInfo.iTargetStat = csWork  ;

        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
            iPickerNo = i ;


            if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==_tInfo.iTargetStat) _tInfo.bDn[iPickerNo] = true  ; //
            if(DM.ARAY[_tInfo.iAray].GetStat(0,iPickerNo)!=csEmpty           ) _tInfo.bDn[iPickerNo] = false ; //타겟이 아님.
            if(!bPckrUsage[iPickerNo]                                        ) _tInfo.bDn[iPickerNo] = false ; //툴 안씀.
        }
    }

    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //빈놈만 다운시킨다.
        if(_tInfo.bDn[i]) _tInfo.iDnCnt++;

    }

    if(_tInfo.iDnCnt) _tInfo.bFindOk = true ;

    return _tInfo.bFindOk ;
}

bool CSort::ShiftTraySortInfo(TSortInfo & _tInfo , bool _bVacErr1 , bool _bVacErr2 ,bool _bVacErr3 ,bool _bVacErr4 )
{
    int r , c ;
    int iPickerNo ;

    if(!_tInfo.bFindOk) return false ;

    bool bVacErr[MAX_TOOL_SOCK_COL] ;

    bVacErr[0]=_bVacErr1;
    bVacErr[1]=_bVacErr2;
    bVacErr[2]=_bVacErr3;
    bVacErr[3]=_bVacErr4;

    if(_tInfo.isPick) { //로더 픽인포일 경우.
        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
            iPickerNo = i + _tInfo.iToolShft ;

            if(iPickerNo       >= MAX_TOOL_SOCK_COL                ) break ; //쉬프트 합친게 넘어가면 브렉
            if(_tInfo.iCol + i >= DM.ARAY[_tInfo.iAray].GetMaxCol()) break ; //트레이 영역 넘어가면 브렉
            if(!_tInfo.bDn[iPickerNo]                              ) continue ; //다운플레그 아니면 컨티뉴,
            if( bVacErr[iPickerNo]                                 ) continue ; //배콤에러.

            DM.ARAY[_tInfo.iTool].SetStat(0           , iPickerNo       , _tInfo.iTargetStat);
            DM.ARAY[_tInfo.iAray].SetStat(_tInfo.iRow , _tInfo.iCol + i , csEmpty           );

        }

    }
    else { //Place
        //오른쪽 밑에가 기준인애들..
        if(_tInfo.iTargetStat == csRslt5 ||
           _tInfo.iTargetStat == csRslt6 ||
           _tInfo.iTargetStat == csRslt7 ||
           _tInfo.iTargetStat == csRslt8 ){
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
                iPickerNo = _tInfo.iToolShft - i ;
                if(iPickerNo       < 0 ) break ;
                if(_tInfo.iCol - i < 0 ) break ;
                if(!_tInfo.bDn[iPickerNo]) continue ; //다운플레그 아니면 컨티뉴,

                DM.ARAY[_tInfo.iTool].SetStat(0           , iPickerNo       , csEmpty           );
                DM.ARAY[_tInfo.iAray].SetStat(_tInfo.iRow , _tInfo.iCol - i , _tInfo.iTargetStat);
            }
        }
        //왼쪽 위에가 기준인애들.
        else {
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
                iPickerNo = i + _tInfo.iToolShft ;
                if(iPickerNo       >= MAX_TOOL_SOCK_COL                ) break ;
                if(_tInfo.iCol + i >= DM.ARAY[_tInfo.iAray].GetMaxCol()) break ;
                if(!_tInfo.bDn[iPickerNo]                              ) continue ; //다운플레그 아니면 컨티뉴,

                DM.ARAY[_tInfo.iTool].SetStat(0           , iPickerNo       , csEmpty           );
                DM.ARAY[_tInfo.iAray].SetStat(_tInfo.iRow , _tInfo.iCol + i , _tInfo.iTargetStat);
            }
        }
    }
    return true ;
}

bool CSort::ShiftSockSortInfo(TSortInfo & _tInfo, bool _bVacErr1 , bool _bVacErr2 ,bool _bVacErr3 ,bool _bVacErr4 )
{
    int r , c ;
    int iPickerNo ;

    if(!_tInfo.bFindOk) return false ;

    bool bVacErr[MAX_TOOL_SOCK_COL] ;

    bVacErr[0]=_bVacErr1;
    bVacErr[1]=_bVacErr2;
    bVacErr[2]=_bVacErr3;
    bVacErr[3]=_bVacErr4;

    DM.ARAY[_tInfo.iAray].SetDispLock(true);
    DM.ARAY[_tInfo.iTool].SetDispLock(true);

    if(_tInfo.isPick) { //로더 픽인포일 경우.
        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
            iPickerNo = i ;

            if(!_tInfo.bDn[iPickerNo]                              ) continue ; //다운플레그 아니면 컨티뉴,
            if( bVacErr[i]                                         ) continue ; //배콤에러.

            DM.ARAY[_tInfo.iTool].SetStat(0 , iPickerNo , DM.ARAY[_tInfo.iAray].GetStat(0 , iPickerNo));
            DM.ARAY[_tInfo.iAray].SetStat(0 , iPickerNo , csEmpty                                     );



        }

    }
    else { //Place
        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
            iPickerNo = i ;
            if(!_tInfo.bDn[iPickerNo]                              ) continue ; //다운플레그 아니면 컨티뉴,
            iPickerNo = i ;
            DM.ARAY[_tInfo.iAray].SetStat(0 , iPickerNo , DM.ARAY[_tInfo.iTool].GetStat(0 , iPickerNo));
            DM.ARAY[_tInfo.iTool].SetStat(0 , iPickerNo , csEmpty                                     );


        }
    }
    DM.ARAY[_tInfo.iAray].SetDispLock(false);
    DM.ARAY[_tInfo.iTool].SetDispLock(false);

    return true ;
}

bool CSort::CheckRetryStgPick()
{
    const bool bPckrUsage[MAX_TOOL_SOCK_COL] = {!OM.CmnOptn.bSkip1stTol,
                                                !OM.CmnOptn.bSkip2ndTol,
                                                !OM.CmnOptn.bSkip3thTol,
                                                !OM.CmnOptn.bSkip4thTol};


    EN_ARAY_ID riSock = riSTF ;

    bool bExistPick[MAX_TOOL_SOCK_COL] ;

    bExistPick[0] = DM.ARAY[riSock].GetStat(0,0)!=csEmpty && DM.ARAY[riSock].GetStat(0,0)!= csUnkwn && DM.ARAY[riSock].GetStat(0,0)!= csWork ;
    bExistPick[1] = DM.ARAY[riSock].GetStat(0,1)!=csEmpty && DM.ARAY[riSock].GetStat(0,1)!= csUnkwn && DM.ARAY[riSock].GetStat(0,1)!= csWork ;
    bExistPick[2] = DM.ARAY[riSock].GetStat(0,2)!=csEmpty && DM.ARAY[riSock].GetStat(0,2)!= csUnkwn && DM.ARAY[riSock].GetStat(0,2)!= csWork ;
    bExistPick[3] = DM.ARAY[riSock].GetStat(0,3)!=csEmpty && DM.ARAY[riSock].GetStat(0,3)!= csUnkwn && DM.ARAY[riSock].GetStat(0,3)!= csWork ;

    bool bRet = true ;
    if(!bExistPick[0]||!bPckrUsage[0]||DM.ARAY[riSRT].GetStat(0,0)!=csEmpty)bRet = false ;
    if(!bExistPick[1]||!bPckrUsage[1]||DM.ARAY[riSRT].GetStat(0,1)!=csEmpty)bRet = false ;
    if(!bExistPick[2]||!bPckrUsage[2]||DM.ARAY[riSRT].GetStat(0,2)!=csEmpty)bRet = false ;
    if(!bExistPick[3]||!bPckrUsage[3]||DM.ARAY[riSRT].GetStat(0,3)!=csEmpty)bRet = false ;

    return bRet ;
}
bool CSort::CheckRetryZigPick()
{
    const bool bPckrUsage[MAX_TOOL_SOCK_COL] = {!OM.CmnOptn.bSkip1stTol,
                                                !OM.CmnOptn.bSkip2ndTol,
                                                !OM.CmnOptn.bSkip3thTol,
                                                !OM.CmnOptn.bSkip4thTol};


    EN_ARAY_ID riSock = riZIG ;

    bool bExistPick[MAX_TOOL_SOCK_COL] ;

    bExistPick[0] = DM.ARAY[riSock].GetStat(0,0)!=csEmpty && DM.ARAY[riSock].GetStat(0,0)!= csUnkwn && DM.ARAY[riSock].GetStat(0,0)!= csWork ;
    bExistPick[1] = DM.ARAY[riSock].GetStat(0,1)!=csEmpty && DM.ARAY[riSock].GetStat(0,1)!= csUnkwn && DM.ARAY[riSock].GetStat(0,1)!= csWork ;
    bExistPick[2] = DM.ARAY[riSock].GetStat(0,2)!=csEmpty && DM.ARAY[riSock].GetStat(0,2)!= csUnkwn && DM.ARAY[riSock].GetStat(0,2)!= csWork ;
    bExistPick[3] = DM.ARAY[riSock].GetStat(0,3)!=csEmpty && DM.ARAY[riSock].GetStat(0,3)!= csUnkwn && DM.ARAY[riSock].GetStat(0,3)!= csWork ;

    bool bRet = true ;
    if(!bExistPick[0]||!bPckrUsage[0]||DM.ARAY[riSRT].GetStat(0,0)!=csEmpty)bRet = false ;
    if(!bExistPick[1]||!bPckrUsage[1]||DM.ARAY[riSRT].GetStat(0,1)!=csEmpty)bRet = false ;
    if(!bExistPick[2]||!bPckrUsage[2]||DM.ARAY[riSRT].GetStat(0,2)!=csEmpty)bRet = false ;
    if(!bExistPick[3]||!bPckrUsage[3]||DM.ARAY[riSRT].GetStat(0,3)!=csEmpty)bRet = false ;

    return bRet ;
}

void CSort::Init()
{
    m_sPartName = "Sorter " ;
    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

    memset(&SortInfo , 0 , sizeof(TSortInfo));

    m_bLdrPickErrDisp = false ;
    m_bZigPickErrDisp = false ;
    m_bStgPickErrDisp = false ;

    m_iVacErrStat = vsNomal ;

    IO_SetY(ySRT_Vcc , true);

    memset(&m_bVacErr , 0 , sizeof(bool));

    m_bRetryZigPick = false ;
    m_bRetryStgPick = false ;


}

void CSort::Close()
{
    Load(false);
}

void CSort::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CSort::FindChip(EN_ARAY_ID _iArray , EN_CHIP_STAT _iStat , int &r , int &c   )
{
    //r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    EN_CHIP_STAT csTool ;

    if(_iArray == riLDR) {
        return DM.ARAY[_iArray].FindFrstRowCol(_iStat , r,c) ;
    }
    else if(_iArray == riSRT) {
        return DM.ARAY[_iArray].FindFrstRowCol(_iStat , r,c) ;
    }
    else if(_iArray == riRJ1 ||
            _iArray == riRJ2 ||
            _iArray == riRJ3 ||
            _iArray == riRJ4 ){

        /*
        for(int c = 0 ; c < MAX_TOOL_SOCK_COL ; c++){
                 if(DM.ARAY[riSRT].GetStat(0,c)==csRslt1) {csTool = DM.ARAY[riSRT].GetStat(0,c) ; break ;}
            else if(DM.ARAY[riSRT].GetStat(0,c)==csRslt5) {csTool = DM.ARAY[riSRT].GetStat(0,c) ; break ;}

            else if(DM.ARAY[riSRT].GetStat(0,c)==csRslt2) {csTool = DM.ARAY[riSRT].GetStat(0,c) ; break ;}
            else if(DM.ARAY[riSRT].GetStat(0,c)==csRslt6) {csTool = DM.ARAY[riSRT].GetStat(0,c) ; break ;}

            else if(DM.ARAY[riSRT].GetStat(0,c)==csRslt3) {csTool = DM.ARAY[riSRT].GetStat(0,c) ; break ;}
            else if(DM.ARAY[riSRT].GetStat(0,c)==csRslt7) {csTool = DM.ARAY[riSRT].GetStat(0,c) ; break ;}

            else if(DM.ARAY[riSRT].GetStat(0,c)==csRslt4) {csTool = DM.ARAY[riSRT].GetStat(0,c) ; break ;}
            else if(DM.ARAY[riSRT].GetStat(0,c)==csRslt8) {csTool = DM.ARAY[riSRT].GetStat(0,c) ; break ;}
        }*/
             if(DM.ARAY[riSRT].GetCntStat(csRslt1))csTool = csRslt1 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt5))csTool = csRslt5 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt2))csTool = csRslt2 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt6))csTool = csRslt6 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt3))csTool = csRslt3 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt7))csTool = csRslt7 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt4))csTool = csRslt4 ;
        else if(DM.ARAY[riSRT].GetCntStat(csRslt8))csTool = csRslt8 ;
        else                                       csTool = csRslt1 ;

        //Rslt1~Rslt4까지 사용하는것은 그냥 풀트레이 쓰고
        //Rslt5~Rslt8을 사용 하여 분할 하는 경우는 5~8은 밑에서 부터 채운다.
        if(csTool == csRslt1 ||
           csTool == csRslt2 ||
           csTool == csRslt3 ||
           csTool == csRslt4 ) return DM.ARAY[_iArray].FindFrstRowCol(_iStat , r,c) ;
        else                   return DM.ARAY[_iArray].FindLastRowCol(_iStat , r,c) ;

    }
    else if(_iArray == riULD) {
        return DM.ARAY[_iArray].FindLastRowFrstCol(_iStat , r,c) ;

    }
    else {
        r=0; c=0;
        return false ;
    }
}

double CSort::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;
    double dCrntPos = MT_GetCmdPos( _iMotr);

    if(_iMotr == miSRT_YPckr){
             if(_iPstnId == piSRT_YPckrWait      ){dPos = PM.GetValue (_iMotr , pvSRT_YPckrWait      );}
        else if(_iPstnId == piSRT_YPckrPick      ){dPos = PM.GetValue (_iMotr , pvSRT_YPckrPick      );}
        else if(_iPstnId == piSRT_YPckrZig       ){dPos = PM.GetValue (_iMotr , pvSRT_YPckrZig       );}
        else if(_iPstnId == piSRT_YPckrPocket    ){dPos = PM.GetValue (_iMotr , pvSRT_YPckrPocket    );}
        else if(_iPstnId == piSRT_YPckrReject1Stt){dPos = PM.GetValue (_iMotr , pvSRT_YPckrReject1Stt);}
        else if(_iPstnId == piSRT_YPckrReject2Stt){dPos = PM.GetValue (_iMotr , pvSRT_YPckrReject2Stt);}
        else if(_iPstnId == piSRT_YPckrReject3Stt){dPos = PM.GetValue (_iMotr , pvSRT_YPckrReject3Stt);}
        else if(_iPstnId == piSRT_YPckrReject4Stt){dPos = PM.GetValue (_iMotr , pvSRT_YPckrReject4Stt);}
        else if(_iPstnId == piSRT_YPckrPlace     ){dPos = PM.GetValue (_iMotr , pvSRT_YPckrPlace     );}
        else if(_iPstnId == piSRT_YPckrReject1   ){
            if(!SortInfo.bFindOk || SortInfo.isPick || SortInfo.iAray != riRJ1){
                dPos = dCrntPos ;
            }
            else {
                dPos = PM.GetValue (_iMotr , pvSRT_YPckrReject1Stt);
                dPos-= SortInfo.iRow * OM.DevInfo.dULDRowPitch ;
            }
        }
        else if(_iPstnId == piSRT_YPckrReject2   ){
            if(!SortInfo.bFindOk || SortInfo.isPick || SortInfo.iAray != riRJ2){
                dPos = dCrntPos ;
            }
            else {
                dPos = PM.GetValue (_iMotr , pvSRT_YPckrReject2Stt);
                dPos-= SortInfo.iRow * OM.DevInfo.dULDRowPitch ;
            }
        }
        else if(_iPstnId == piSRT_YPckrReject3   ){
            if(!SortInfo.bFindOk || SortInfo.isPick || SortInfo.iAray != riRJ3){
                dPos = dCrntPos ;
            }
            else {
                dPos = PM.GetValue (_iMotr , pvSRT_YPckrReject3Stt);
                dPos-= SortInfo.iRow * OM.DevInfo.dULDRowPitch ;
            }
        }
        else if(_iPstnId == piSRT_YPckrReject4   ){
            if(!SortInfo.bFindOk || SortInfo.isPick || SortInfo.iAray != riRJ4){
                dPos = dCrntPos ;
            }
            else {
                dPos = PM.GetValue (_iMotr , pvSRT_YPckrReject4Stt);
                dPos-= SortInfo.iRow * OM.DevInfo.dULDRowPitch ;
            }
        }
        else {
            dPos = dCrntPos ;
        }
    }
    else if(_iMotr == miSRT_ZPckr){
             if(_iPstnId == piSRT_ZPckrWait       ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrWait       );}
        else if(_iPstnId == piSRT_ZPckrMove       ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrMove       );}
        else if(_iPstnId == piSRT_ZPckrSTGPick    ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrSTGPick    );}
        else if(_iPstnId == piSRT_ZPckrSTGPlace   ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrSTGPlace   );}
        else if(_iPstnId == piSRT_ZPckrZIGPick    ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrZIGPick    );}
        else if(_iPstnId == piSRT_ZPckrZIGPlace   ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrZIGPlace   );}
        else if(_iPstnId == piSRT_ZPckrRJTPick    ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrRJTPick    );}
        else if(_iPstnId == piSRT_ZPckrRJTPlace   ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrRJTPlace   );}
        else if(_iPstnId == piSRT_ZPckrLDRPick    ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrLDRPick    );}
        else if(_iPstnId == piSRT_ZPckrLDRPlace   ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrLDRPlace   );}
        else if(_iPstnId == piSRT_ZPckrULDPick    ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrULDPick    );}
        else if(_iPstnId == piSRT_ZPckrULDPlace   ){dPos = PM.GetValue (_iMotr , pvSRT_ZPckrULDPlace   );}
        else                                       {dPos = dCrntPos ;                                    }

    }
    else if(_iMotr == miSRT_XPckr){
             if(_iPstnId == piSRT_XPckrWait      ){dPos = PM.GetValue (_iMotr , pvSRT_XPckrWait      );}
        else if(_iPstnId == piSRT_XPckrPickStt   ){dPos = PM.GetValue (_iMotr , pvSRT_XPckrPickStt   );}
        else if(_iPstnId == piSRT_XPckrZig       ){dPos = PM.GetValue (_iMotr , pvSRT_XPckrZig       );}
        else if(_iPstnId == piSRT_XPckrPocket    ){dPos = PM.GetValue (_iMotr , pvSRT_XPckrPocket    );}
        else if(_iPstnId == piSRT_XPckrReject1Stt){dPos = PM.GetValue (_iMotr , pvSRT_XPckrReject1Stt);}
        else if(_iPstnId == piSRT_XPckrReject2Stt){dPos = PM.GetValue (_iMotr , pvSRT_XPckrReject2Stt);}
        else if(_iPstnId == piSRT_XPckrReject3Stt){dPos = PM.GetValue (_iMotr , pvSRT_XPckrReject3Stt);}
        else if(_iPstnId == piSRT_XPckrReject4Stt){dPos = PM.GetValue (_iMotr , pvSRT_XPckrReject4Stt);}
        else if(_iPstnId == piSRT_XPckrPlaceStt  ){dPos = PM.GetValue (_iMotr , pvSRT_XPckrPlaceStt  );}
        else if(_iPstnId == piSRT_XPckrPick      ){
            if(!SortInfo.bFindOk || !SortInfo.isPick || SortInfo.iAray != riLDR){
                dPos = dCrntPos ;
            }
            else {
                dPos = PM.GetValue (_iMotr , pvSRT_XPckrPickStt);
                dPos+= SortInfo.iCol      * OM.DevInfo.dLDRColPitch ;
                dPos-= SortInfo.iToolShft * OM.DevInfo.dLDRColPitch ; //툴이 쳣째에 칩이 있을경우.
            }

        }
        else if(_iPstnId == piSRT_XPckrReject1   ){
            if(!SortInfo.bFindOk || SortInfo.isPick || SortInfo.iAray != riRJ1){
                dPos = dCrntPos ;
            }
            else {
                dPos = PM.GetValue (_iMotr , pvSRT_XPckrReject1Stt);
                dPos+= SortInfo.iCol      * OM.DevInfo.dULDColPitch ;
                dPos-= SortInfo.iToolShft * OM.DevInfo.dULDColPitch ;
            }
        }
        else if(_iPstnId == piSRT_XPckrReject2   ){
            if(!SortInfo.bFindOk || SortInfo.isPick || SortInfo.iAray != riRJ2){
                dPos = dCrntPos ;
            }
            else {
                dPos = PM.GetValue (_iMotr , pvSRT_XPckrReject2Stt);
                dPos+= SortInfo.iCol      * OM.DevInfo.dULDColPitch ;
                dPos-= SortInfo.iToolShft * OM.DevInfo.dULDColPitch ;
            }
        }
        else if(_iPstnId == piSRT_XPckrReject3   ){
            if(!SortInfo.bFindOk || SortInfo.isPick || SortInfo.iAray != riRJ3){
                dPos = dCrntPos ;
            }
            else {
                dPos = PM.GetValue (_iMotr , pvSRT_XPckrReject3Stt);
                dPos+= SortInfo.iCol      * OM.DevInfo.dULDColPitch ;
                dPos-= SortInfo.iToolShft * OM.DevInfo.dULDColPitch ;
            }
        }
        else if(_iPstnId == piSRT_XPckrReject4   ){
            if(!SortInfo.bFindOk || SortInfo.isPick || SortInfo.iAray != riRJ4){
                dPos = dCrntPos ;
            }
            else {
                dPos = PM.GetValue (_iMotr , pvSRT_XPckrReject4Stt);
                dPos+= SortInfo.iCol      * OM.DevInfo.dULDColPitch ;
                dPos-= SortInfo.iToolShft * OM.DevInfo.dULDColPitch ;
            }
        }
        else if(_iPstnId == piSRT_XPckrPlace     ){
            if(!SortInfo.bFindOk || SortInfo.isPick || SortInfo.iAray != riULD){
                dPos = dCrntPos ;
            }
            else {
                dPos = PM.GetValue (_iMotr , pvSRT_XPckrPlaceStt);
                dPos+= SortInfo.iCol      * OM.DevInfo.dULDColPitch ;
                dPos-= SortInfo.iToolShft * OM.DevInfo.dULDColPitch ;
            }
        }
        else {
            dPos = dCrntPos ;
        }
    }
    else if(_iMotr == miSRT_XExpd){
             if(_iPstnId == piSRT_XExpdWait      ){dPos = PM.GetValue (_iMotr , pvSRT_XExpdWait      );}
        else if(_iPstnId == piSRT_XExpdLDRTray   ){dPos = PM.GetValue (_iMotr , pvSRT_XExpdLDRTray   );}
        else if(_iPstnId == piSRT_XExpdPocket    ){dPos = PM.GetValue (_iMotr , pvSRT_XExpdPocket    );}
        else if(_iPstnId == piSRT_XExpdReject    ){dPos = PM.GetValue (_iMotr , pvSRT_XExpdReject    );}
        else if(_iPstnId == piSRT_XExpdZig       ){dPos = PM.GetValue (_iMotr , pvSRT_XExpdZig       );}
        else if(_iPstnId == piSRT_XExpdULDTray   ){dPos = PM.GetValue (_iMotr , pvSRT_XExpdULDTray   );}
        else                                      {dPos = dCrntPos;                                    }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CSort::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CSort::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CSort::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CSort::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                 IO_SetY(ySRT_Vcc , true);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CSort::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                 MoveActr(aiSRT_AlgnFwBw , ccBwd);
                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false ;
                 if(!MoveActr(aiSRT_AlgnFwBw , ccBwd)) return false  ;

                 MoveActr(aiSRT_FrstDnUp , ccFwd);
                 MoveActr(aiSRT_ScndDnUp , ccFwd);
                 MoveActr(aiSRT_ThrdDnUp , ccFwd);
                 MoveActr(aiSRT_FrthDnUp , ccFwd);

                 Step.iToStop++;
                 return false ;

        case 12: if(!MoveActr(aiSRT_FrstDnUp , ccFwd)) return false ;
                 if(!MoveActr(aiSRT_ScndDnUp , ccFwd)) return false ;
                 if(!MoveActr(aiSRT_ThrdDnUp , ccFwd)) return false ;
                 if(!MoveActr(aiSRT_FrthDnUp , ccFwd)) return false ;

                 if(EM_GetErr(eiSRT_RejectFull    ) ||
                    EM_GetErr(eiSRT_RJTNeedTray   ) ||
                    EM_GetErr(eiSRT_RJTTrayRelease) ||
                    SEQ._iSeqStat == ssWorkEnd      ){
                     MoveMotr(miSRT_XPckr , piSRT_XPckrPocket);
                     MoveMotr(miSRT_YPckr , piSRT_YPckrPocket);
                 }

                 Step.iToStop++;
                 return false ;

        case 13: if(!MT_GetStop(miSRT_XPckr)) return false ;
                 if(!MT_GetStop(miSRT_YPckr)) return false ;

                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool GetUseDualBin(int _iRjtNo)
{
    int iCnt = 0 ;
    const int iRjtNo = _iRjtNo - 1 ; //밖에서는 1,2,3,4 이렇게 인자를 넣는다.
                                                       // bin
    const int iOSTOSTBin  = OM.CmnOptn.iOSTOSTBin ;       //6 ;   6
    const int iOSTCrnBin  = OM.CmnOptn.iOSTCrnBin ;       //6 ;   6
    const int iVTDIMBin   = OM.CmnOptn.iVTDIMBin  ;       //1 ;   1
    const int iVTBLKBin   = OM.CmnOptn.iVTBLKBin  ;       //2 ;   3
    const int iVTVHBin    = OM.CmnOptn.iVTVHBin   ;       //4 ;   7
    const int iVTDFTBin   = OM.CmnOptn.iVTDFTBin  ;       //8 ;   0
    const int iVTDFT2Bin  = OM.CmnOptn.iVTDFT2Bin ;       //8 ;   4
    const int iVSNFailBin = OM.CmnOptn.iVSNFailBin;       //7 ;   2


    //_iRjtNo = 1,2,3,4

    if(iOSTOSTBin %4 == iRjtNo) iCnt++;
    if(iOSTCrnBin %4 == iRjtNo) iCnt++;
    if(iVTDIMBin  %4 == iRjtNo) iCnt++;
    if(iVTBLKBin  %4 == iRjtNo) iCnt++;
    if(iVTVHBin   %4 == iRjtNo) iCnt++;
    if(iVTDFTBin  %4 == iRjtNo) iCnt++;                //sunsun RjtTray clear button.
    if(iVTDFT2Bin %4 == iRjtNo) iCnt++;
    if(iVSNFailBin%4 == iRjtNo) iCnt++;

    if(iCnt >=2) return true ;
    return false ;

}



bool CSort::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    static int iCycleSttTime = 0;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool bLDRExistWork = DM.ARAY[riLDR].GetCntStat(csUnkwn) ||
                             DM.ARAY[riPRL].GetCntStat(csUnkwn) ||
                            (IO_GetX(xLSP_Tray5Sply) && !LOT.GetRqstLotEnd()) ;

        //시퀜스 순서 존나 중요함..
        //바꿀때 신중해야 한다.
        //하나만 보고 바꾸면 홀드 날 수 있음.
        //****찝기 실패시에 SrtCheckAllStat(riSRT , csEmpty) 이것 때문에 플레이스 먼저 하고 다시 찝음. ****************
        //스테이지에서 찝기=====================================================
        bool isCyclePickSTG_Srt =  ScmCheckAllStat (riSRT , csEmpty) ;
        bool isCyclePickSTG_Stg = !ScmCheckAllStat (riSTF , csEmpty) &&
                                  !ScmGetCntStat   (riSTF , csUnkwn) &&
                                  !ScmGetCntStat   (riSTF , csWork ) &&
                                   CStage::scIdle == STG.GetSeqStep();
        bool isCyclePickSTG_Zig = ScmGetCntStat(riZIG , csWork) || //정상일땐 지그에 있어야 스테이지를 찝는데.
                                 (LOT.GetRqstLotEnd() && !bLDRExistWork); //랏엔드 시엔 없어도 찝는다.
        bool isCyclePickSTG_Rtr = m_bRetryStgPick ;  //CheckRetryStgPick() ; //플레그 없애고 함수로 대체. CheckRetryStgPick() 준선.
        bool isCyclePickSTG = (isCyclePickSTG_Srt && isCyclePickSTG_Stg && isCyclePickSTG_Zig) ||
                               isCyclePickSTG_Rtr ;


        //리젝 내려놓기===========================================================
        //만약 리젝 트레이 만땅은 플레이스시에 한번 띄우고 싸이클 들어가서 띄우고 리턴 트루.
        bool isCyclePlceRJT_Srt = DM.ARAY[riSRT].GetCntStat(csRslt1) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt2) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt3) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt4) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt5) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt6) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt7) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt8) ;
        bool isCyclePlceRJT = isCyclePlceRJT_Srt ;

        //굿 내려놓기=============================================================
        bool isCyclePlceULD_Uld = DM.ARAY[riULD].GetCntStat(csEmpty) ;
        bool isCyclePlceULD_Srt = DM.ARAY[riSRT].GetCntStat(csGood) ;
        bool isCyclePlceULD = isCyclePlceULD_Uld && isCyclePlceULD_Srt;

        //지그에 하나 라도 있을경우 지그 먼처 제거 하지 않으면 꼬여서 홀드.
        //지그에 있을경우 절대 로더에서 찝으면 안됌.. 홀드.
        //무조건 지그에 있으면 지그 부터 비워내야 함.
        //Zig에서 찝기===========================================================
        bool isCyclePickZIG_Zig = ScmGetCntStat  (riZIG , csWork ) ;
        bool isCyclePickZIG_Srt = ScmCheckAllStat(riSRT , csEmpty) ;
        bool isCyclePickZIG_Stg = !ScmGetCntStat  (riSTF , csWork) && CStage::scIdle == STG.GetSeqStep();
        bool isCyclePickZIG_Rtr = m_bRetryZigPick ;  //CheckRetryZigPick();
        bool isCyclePickZIG = (isCyclePickZIG_Zig && isCyclePickZIG_Srt && isCyclePickZIG_Stg) ||
                               isCyclePickZIG_Rtr; //플레그 없애고 함수로 대체. CheckRetryZigPick() 준선.

        //스테이지에 놓기===========================================================
        bool isCyclePlceSTG_Stg = ScmCheckAllStat(riSTF , csEmpty)&&
                                  CStage::scIdle == STG.GetSeqStep();
        bool isCyclePlceSTG_Srt = ScmGetCntStat(riSRT , csWork) ;
        bool isCyclePlceSTG = isCyclePlceSTG_Stg && isCyclePlceSTG_Srt;


        //로더에서 찝기.========================================================
        bool bCyclePickLDR_Zig = ScmCheckAllStat(riZIG , csEmpty) ;//SrtGetCntStat(riZIG , csEmpty) ; //
        bool bCyclePickLDR_Ldr = DM.ARAY[riLDR].GetCntStat(csUnkwn) /*속도 때문에 시퀜스 안으로 이동.&& LDR.GetInWorkPos()*/ ;
        bool bTemp1 =  ScmGetCntStat  (riSRT , csEmpty) ;
        bool bTemp2 = !ScmGetCntStat  (riSRT , csWork ) ;
        bool bTemp3 = !ScmGetCntStat  (riSRT , csRslt1) && !ScmGetCntStat(riSRT , csRslt5) ;
        bool bTemp4 = !ScmGetCntStat  (riSRT , csRslt2) && !ScmGetCntStat(riSRT , csRslt6) ;
        bool bTemp5 = !ScmGetCntStat  (riSRT , csRslt3) && !ScmGetCntStat(riSRT , csRslt7) ;
        bool bTemp6 = !ScmGetCntStat  (riSRT , csRslt4) && !ScmGetCntStat(riSRT , csRslt8) ;
        bool bTemp7 = !ScmGetCntStat  (riSRT , csGood ) ;

        bool bCyclePickLDR_Srt = ScmGetCntStat  (riSRT , csEmpty) &&  //툴에 찝을 곳이 있어야 함.
                                !ScmGetCntStat  (riSRT , csWork ) &&
                                !ScmGetCntStat  (riSRT , csRslt1) && !ScmGetCntStat(riSRT , csRslt5) &&
                                !ScmGetCntStat  (riSRT , csRslt2) && !ScmGetCntStat(riSRT , csRslt6) &&
                                !ScmGetCntStat  (riSRT , csRslt3) && !ScmGetCntStat(riSRT , csRslt7) &&
                                !ScmGetCntStat  (riSRT , csRslt4) && !ScmGetCntStat(riSRT , csRslt8) &&
                                !ScmGetCntStat  (riSRT , csGood ) ;
        bool isCyclePickLDR =  bCyclePickLDR_Zig &&
                               bCyclePickLDR_Ldr &&
                               bCyclePickLDR_Srt ;

        //Zig에 놓기.===========================================================
        bool isCyclePlceZIG_Zig = ScmGetCntStat(riZIG , csEmpty) ; //한두개 없을때도 고려.
        bool isCyclePlceZIG_Srt = ScmCheckAllStat(riSRT , csUnkwn) || //정상 플로우.
                                 (ScmGetCntStat(riSRT , csUnkwn) && LOT.GetRqstLotEnd() && !bLDRExistWork); //랏엔드 플로우.
        bool isCyclePlceZIG = isCyclePlceZIG_Zig &&
                              isCyclePlceZIG_Srt ;


        //2014.12.16 여연수 대리 요청 홀드시에 리젝처리.
        //스테이지에 소팅해야 할 검사끝난 자제 있고
        //지그에 얼라인된 자제 있고
        //툴에 얼라인 된 자제 있거나 , UnKown자제 있을때 홀드난다. 작업자가 데이터 조작 해서 그럼.
        //홀드상황==============================================================
        //case 1 : 스테이지에 소팅해야 할것 들이 있고 && 툴이 얼라인된(csWork)를 집고 있고 ;
        //case 2 : 스테이지에 소팅해야 할것 들이 있고 && 툴이 언노운을(csUnkn)을 집고 있고 && 얼라인어에 자제 가 있는경우.
        bool isNothingToDo = !isCyclePickSTG &&
                             !isCyclePlceRJT &&
                             !isCyclePlceULD &&
                             !isCyclePickZIG &&
                             !isCyclePlceSTG &&
                             !isCyclePickLDR &&
                             !isCyclePlceZIG ;
        bool isHoldStg  = !ScmCheckAllStat (riSTF , csEmpty) &&
                          !ScmGetCntStat   (riSTF , csUnkwn) &&
                          !ScmGetCntStat   (riSTF , csWork ) &&
                          CStage::scIdle == STG.GetSeqStep();

        bool isHoldCase1 = isHoldStg && ScmGetCntStat (riSRT , csWork ) ;
        bool isHoldCase2 = isHoldStg && ScmGetCntStat (riSRT , csUnkwn) && ScmGetCntStat (riZIG , csWork);


        bool isHoldStat = isNothingToDo && (isHoldCase1 || isHoldCase2) ;
        if(isHoldStat){
            for(int c = 0 ; c < MAX_TOOL_SOCK_COL ; c++) {
                if(DM.ARAY[riSRT].GetStat(0,c)!=csEmpty) {
                    Trace("Hold Stat" , (String(c+1) + " Sorter Changed OSTFail").c_str());
                    DM.ARAY[riSRT].SetStat(0,c,(EN_CHIP_STAT)(OM.CmnOptn.iOSTOSTBin +1));
                }
            }
        }


        //End===================================================================
        bool isCycleEnd  =  DM.ARAY[riSRT].CheckAllStat(csEmpty ) && DM.ARAY[riZIG].CheckAllStat(csEmpty ) ;


        //모르는 스트립에러.
        if(!OM.MstOptn.bVacErrIgnr) {
            if(DM.ARAY[riSRT].GetStat(0,0)==csEmpty&& IO_GetX(xSRT_VccSnsr1)) EM_SetErrMsg(eiPKG_Unknwn , (DM.ARAY[riSRT].GetName()+"_1").c_str());
            if(DM.ARAY[riSRT].GetStat(0,1)==csEmpty&& IO_GetX(xSRT_VccSnsr2)) EM_SetErrMsg(eiPKG_Unknwn , (DM.ARAY[riSRT].GetName()+"_2").c_str());
            if(DM.ARAY[riSRT].GetStat(0,2)==csEmpty&& IO_GetX(xSRT_VccSnsr3)) EM_SetErrMsg(eiPKG_Unknwn , (DM.ARAY[riSRT].GetName()+"_3").c_str());
            if(DM.ARAY[riSRT].GetStat(0,3)==csEmpty&& IO_GetX(xSRT_VccSnsr4)) EM_SetErrMsg(eiPKG_Unknwn , (DM.ARAY[riSRT].GetName()+"_4").c_str());
        }
        //카세트 사라짐.
        if(!OM.MstOptn.bVacErrIgnr) {
            if( DM.ARAY[riSRT].GetStat(0,0)!=csEmpty&&!IO_GetX(xSRT_VccSnsr1)) EM_SetErrMsg(eiPKG_Dispr , (DM.ARAY[riSRT].GetName()+"_1").c_str());
            if( DM.ARAY[riSRT].GetStat(0,1)!=csEmpty&&!IO_GetX(xSRT_VccSnsr2)) EM_SetErrMsg(eiPKG_Dispr , (DM.ARAY[riSRT].GetName()+"_2").c_str());
            if( DM.ARAY[riSRT].GetStat(0,2)!=csEmpty&&!IO_GetX(xSRT_VccSnsr3)) EM_SetErrMsg(eiPKG_Dispr , (DM.ARAY[riSRT].GetName()+"_3").c_str());
            if( DM.ARAY[riSRT].GetStat(0,3)!=csEmpty&&!IO_GetX(xSRT_VccSnsr4)) EM_SetErrMsg(eiPKG_Dispr , (DM.ARAY[riSRT].GetName()+"_4").c_str());
        }


        //메거진 없음.
        if(!IO_GetX(xSRT_RjtTray1)) EM_SetErrMsg(eiSRT_RJTNeedTray,"Reject Tray 1");
        if(!IO_GetX(xSRT_RjtTray2)) EM_SetErrMsg(eiSRT_RJTNeedTray,"Reject Tray 2");
        if(!IO_GetX(xSRT_RjtTray3)) EM_SetErrMsg(eiSRT_RJTNeedTray,"Reject Tray 3");
        if(!IO_GetX(xSRT_RjtTray4)) EM_SetErrMsg(eiSRT_RJTNeedTray,"Reject Tray 4");

        //카세트 OUT 매거진 풀
        if(DM.ARAY[riRJ1].GetCntStat(csEmpty) == 0) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 1");
        if(DM.ARAY[riRJ2].GetCntStat(csEmpty) == 0) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 2");
        if(DM.ARAY[riRJ3].GetCntStat(csEmpty) == 0) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 3");
        if(DM.ARAY[riRJ4].GetCntStat(csEmpty) == 0) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 4");


        //듀얼빈 사용시에 에러는 한줄 남겨 놓고 해야 구분이 간다.
        if(GetUseDualBin(1) && DM.ARAY[riRJ1].GetCntStat(csEmpty) <= OM.DevInfo.iULDColCnt) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 1");
        if(GetUseDualBin(2) && DM.ARAY[riRJ2].GetCntStat(csEmpty) <= OM.DevInfo.iULDColCnt) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 2");
        if(GetUseDualBin(3) && DM.ARAY[riRJ3].GetCntStat(csEmpty) <= OM.DevInfo.iULDColCnt) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 3");
        if(GetUseDualBin(4) && DM.ARAY[riRJ4].GetCntStat(csEmpty) <= OM.DevInfo.iULDColCnt) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 4");

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCyclePickSTG) {Trace(m_sPartName.c_str(),"CyclePickSTG Stt"); Step.iSeq = scPickSTG ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCyclePlceRJT) {Trace(m_sPartName.c_str(),"CyclePlceRJT Stt"); Step.iSeq = scPlceRJT ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCyclePlceULD) {Trace(m_sPartName.c_str(),"CyclePlceULD Stt"); Step.iSeq = scPlceULD ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCyclePickZIG) {Trace(m_sPartName.c_str(),"CyclePickZIG Stt"); Step.iSeq = scPickZIG ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCyclePlceSTG) {Trace(m_sPartName.c_str(),"CyclePlceSTG Stt"); Step.iSeq = scPlceSTG ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCyclePickLDR) {Trace(m_sPartName.c_str(),"CyclePickLDR Stt"); Step.iSeq = scPickLDR ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCyclePlceZIG) {Trace(m_sPartName.c_str(),"CyclePlceZIG Stt"); Step.iSeq = scPlceZIG ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default        :                     Trace(m_sPartName.c_str(),"default      End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle    :                                                                                                                                               return false ;
        case scPickSTG : if(CyclePickSTG()){ Trace(m_sPartName.c_str(),"CyclePickSTG End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPlceRJT : if(CyclePlceRJT()){ Trace(m_sPartName.c_str(),"CyclePlceRJT End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPlceULD : if(CyclePlceULD()){ Trace(m_sPartName.c_str(),"CyclePlceULD End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPickZIG : if(CyclePickZIG()){ Trace(m_sPartName.c_str(),"CyclePickZIG End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPlceSTG : if(CyclePlceSTG()){ Trace(m_sPartName.c_str(),"CyclePlceSTG End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPickLDR : if(CyclePickLDR()){ Trace(m_sPartName.c_str(),"CyclePickLDR End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPlceZIG : if(CyclePlceZIG()){ Trace(m_sPartName.c_str(),"CyclePlceZIG End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CSort::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 25000 )) {
        EM_SetErrMsg(eiPRT_HomeTo,m_sPartName.c_str());
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

    if(Stat.bReqStop) {
        //Step.iHome = 0;
        //return true ;
    }

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_DoHome(miSRT_ZPckr);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miSRT_ZPckr)) return false ;
                 MT_GoAbsMan(miSRT_ZPckr , PM.GetValue(miSRT_ZPckr , pvSRT_ZPckrWait));
                 MoveActr(aiSRT_FrstDnUp , ccFwd);
                 MoveActr(aiSRT_ScndDnUp , ccFwd);
                 MoveActr(aiSRT_ThrdDnUp , ccFwd);
                 MoveActr(aiSRT_FrthDnUp , ccFwd);
                 Step.iHome++;
                 return false ;

        case 12: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait)) return false ;
                 if(!MoveActr(aiSRT_FrstDnUp , ccFwd)) return false ;
                 if(!MoveActr(aiSRT_ScndDnUp , ccFwd)) return false ;
                 if(!MoveActr(aiSRT_ThrdDnUp , ccFwd)) return false ;
                 if(!MoveActr(aiSRT_FrthDnUp , ccFwd)) return false ;
                 MT_DoHome(miSRT_XExpd);
                 MT_DoHome(miSRT_XPckr);
                 MT_DoHome(miSRT_YPckr);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miSRT_XExpd)) return false ;
                 if(!MT_GetHomeEnd(miSRT_XPckr)) return false ;
                 if(!MT_GetHomeEnd(miSRT_YPckr)) return false ;
                 MT_GoAbsMan(miSRT_XExpd , PM.GetValue(miSRT_XExpd , pvSRT_XExpdWait));
                 MT_GoAbsMan(miSRT_XPckr , PM.GetValue(miSRT_XPckr , pvSRT_XPckrWait));
                 MT_GoAbsMan(miSRT_YPckr , PM.GetValue(miSRT_YPckr , pvSRT_YPckrWait));
                 Step.iHome++;
                 return false ;


        case 14: if(!MT_GoAbsMan(miSRT_XExpd , PM.GetValue(miSRT_XExpd , pvSRT_XExpdWait))) return false ;
                 if(!MT_GoAbsMan(miSRT_XPckr , PM.GetValue(miSRT_XPckr , pvSRT_XPckrWait))) return false ;
                 if(!MT_GoAbsMan(miSRT_YPckr , PM.GetValue(miSRT_YPckr , pvSRT_YPckrWait))) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

/*
        bool CyclePlceZIG();
        bool CyclePickZIG();
        bool CyclePlceSTG();
        bool CyclePickSTG();
        bool CyclePlceRJT();
        bool CyclePlceULD();
*/
//---------------------------------------------------------------------------
bool CSort::CyclePickSTG(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;
    AnsiString sTemp1 ;
    bool bVacErr[MAX_TOOL_SOCK_COL] ;
    CLotData::TData LotData ;
    bool bExistVacErr ;



    static bool bDown[MAX_TOOL_SOCK_COL] = {false ,false ,false ,false};

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(m_bRetryStgPick) { //그냥 제거 했을 경우 삽질 하는것 클리어.
                      if(DM.ARAY[riSTF].CheckAllStat(csEmpty)) m_bRetryStgPick = false ;

                  }

                  GetSockSortInfo(true , riSRT , riSTF , SortInfo);
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);

                  m_bRetryZigPick = false ;

                  MoveActr(aiSRT_FrstDnUp , SortInfo.bDn[0]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ScndDnUp , SortInfo.bDn[1]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ThrdDnUp , SortInfo.bDn[2]?ccFwd:ccBwd);
                  MoveActr(aiSRT_FrthDnUp , SortInfo.bDn[3]?ccFwd:ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false;
                  MoveMotr(miSRT_XExpd , piSRT_XExpdPocket);
                  MoveMotr(miSRT_XPckr , piSRT_XPckrPocket);
                  MoveMotr(miSRT_YPckr , piSRT_YPckrPocket);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miSRT_XExpd , piSRT_XExpdPocket)) return false;
                  if(!MoveMotr(miSRT_XPckr , piSRT_XPckrPocket)) return false;
                  if(!MoveMotr(miSRT_YPckr , piSRT_YPckrPocket)) return false;
                  if(!AT_Done (aiSRT_FrstDnUp )) return false ;
                  if(!AT_Done (aiSRT_ScndDnUp )) return false ;
                  if(!AT_Done (aiSRT_ThrdDnUp )) return false ;
                  if(!AT_Done (aiSRT_FrthDnUp )) return false ;
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrSTGPick );
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrSTGPick)) return false;
                  IO_SetY(ySRT_Ejct1 , false);
                  IO_SetY(ySRT_Ejct2 , false);
                  IO_SetY(ySRT_Ejct3 , false);
                  IO_SetY(ySRT_Ejct4 , false);

                  if(SortInfo.bDn[0]) IO_SetY(ySRT_Vcc1 , true);
                  if(SortInfo.bDn[1]) IO_SetY(ySRT_Vcc2 , true);
                  if(SortInfo.bDn[2]) IO_SetY(ySRT_Vcc3 , true);
                  if(SortInfo.bDn[3]) IO_SetY(ySRT_Vcc4 , true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if((!SortInfo.bDn[0]||IO_GetX(xSRT_VccSnsr1))&&
                     (!SortInfo.bDn[1]||IO_GetX(xSRT_VccSnsr2))&&
                     (!SortInfo.bDn[2]||IO_GetX(xSRT_VccSnsr3))&&
                     (!SortInfo.bDn[3]||IO_GetX(xSRT_VccSnsr4))){
                      MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                      Step.iCycle++;
                      return false ;
                  }
                  if(!m_tmDelay.OnDelay(true, OM.DevOptn.iPickDelay)) return false;
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  Step.iCycle++;
                  return false ;     

        case  15: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false ;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        case  16: bVacErr[0] = SortInfo.bDn[0]&&!IO_GetX(xSRT_VccSnsr1) && !OM.MstOptn.bVacErrIgnr ;
                  bVacErr[1] = SortInfo.bDn[1]&&!IO_GetX(xSRT_VccSnsr2) && !OM.MstOptn.bVacErrIgnr ;
                  bVacErr[2] = SortInfo.bDn[2]&&!IO_GetX(xSRT_VccSnsr3) && !OM.MstOptn.bVacErrIgnr ;
                  bVacErr[3] = SortInfo.bDn[3]&&!IO_GetX(xSRT_VccSnsr4) && !OM.MstOptn.bVacErrIgnr ;
                  bExistVacErr = bVacErr[0] || bVacErr[1] || bVacErr[2] || bVacErr[3] ;
                  if(bExistVacErr && !m_tmDelay.OnDelay(true , 1000))return false ;

                  sTemp1 = "" ;
                  if(bVacErr[0]){sTemp1+="1번";}//IO_SetY(ySRT_Vcc1 , false);}
                  if(bVacErr[1]){sTemp1+="2번";}//IO_SetY(ySRT_Vcc2 , false);}
                  if(bVacErr[2]){sTemp1+="3번";}//IO_SetY(ySRT_Vcc3 , false);}
                  if(bVacErr[3]){sTemp1+="4번";}//IO_SetY(ySRT_Vcc4 , false);}
                  sTemp1 = "Socket " + sTemp1 ;

                  //배큠에러 처리 때문에 사용하지 못함.
                  ShiftSockSortInfo(SortInfo , bVacErr[0],
                                               bVacErr[1],
                                               bVacErr[2],
                                               bVacErr[3]);

                  //if(SortInfo.bDn[0]&&IO_GetX(xSRT_VccSnsr1)){DM.ARAY[riSRT].SetStat(0,0,DM.ARAY[ScmGetFrntSTGAray()].GetStat(0,0)); DM.ARAY[ScmGetFrntSTGAray()].SetStat(0,0,csEmpty);}
                  //if(SortInfo.bDn[1]&&IO_GetX(xSRT_VccSnsr2)){DM.ARAY[riSRT].SetStat(0,1,DM.ARAY[ScmGetFrntSTGAray()].GetStat(0,1)); DM.ARAY[ScmGetFrntSTGAray()].SetStat(0,1,csEmpty);}
                  //if(SortInfo.bDn[2]&&IO_GetX(xSRT_VccSnsr3)){DM.ARAY[riSRT].SetStat(0,2,DM.ARAY[ScmGetFrntSTGAray()].GetStat(0,2)); DM.ARAY[ScmGetFrntSTGAray()].SetStat(0,2,csEmpty);}
                  //if(SortInfo.bDn[3]&&IO_GetX(xSRT_VccSnsr4)){DM.ARAY[riSRT].SetStat(0,3,DM.ARAY[ScmGetFrntSTGAray()].GetStat(0,3)); DM.ARAY[ScmGetFrntSTGAray()].SetStat(0,3,csEmpty);}


                  if(sTemp1 != "" && !OM.MstOptn.bVacErrIgnr &&
                     (bVacErr[0]||
                      bVacErr[1]||
                      bVacErr[2]||
                      bVacErr[3])) {
                      EM_SetDisp(false);
                      EM_SetErrMsg(eiSRT_VaccMiss , sTemp1.c_str());
                      m_bStgPickErrDisp = true ;
                      m_bVacErr[0] = bVacErr[0] ;
                      m_bVacErr[1] = bVacErr[1] ;
                      m_bVacErr[2] = bVacErr[2] ;
                      m_bVacErr[3] = bVacErr[3] ;

                      //sun 오퍼레이션 타이머에서 FormVacErr 디스플레이.
                  }


                  Step.iCycle = 0;
                  return true;
    }
}
//---------------------------------------------------------------------------
bool CSort::CyclePlceRJT(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;
    bool bReject1 , bReject2 , bReject3 , bReject4 ;
    bool bExistRjct ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(!IO_GetX(xSRT_TrayDetect)) {
                      EM_SetErr(eiSRT_RJTTrayRelease);
                      Step.iCycle=0;
                      return true ;
                  }
                  if(!IO_GetX(xSRT_RjtTray1)){
                      EM_SetErrMsg(eiSRT_RJTNeedTray , "Reject1");
                      Step.iCycle=0;
                      return true ;
                  }
                  if(!IO_GetX(xSRT_RjtTray2)){
                      EM_SetErrMsg(eiSRT_RJTNeedTray , "Reject2");
                      Step.iCycle=0;
                      return true ;
                  }
                  if(!IO_GetX(xSRT_RjtTray3)){
                      EM_SetErrMsg(eiSRT_RJTNeedTray , "Reject3");
                      Step.iCycle=0;
                      return true ;
                  }
                  if(!IO_GetX(xSRT_RjtTray4)){
                      EM_SetErrMsg(eiSRT_RJTNeedTray , "Reject4");
                      Step.iCycle=0;
                      return true ;
                  }


                  bReject1 = DM.ARAY[riSRT].GetCntStat(csRslt1) || DM.ARAY[riSRT].GetCntStat(csRslt5) ;
                  bReject2 = DM.ARAY[riSRT].GetCntStat(csRslt2) || DM.ARAY[riSRT].GetCntStat(csRslt6) ;
                  bReject3 = DM.ARAY[riSRT].GetCntStat(csRslt3) || DM.ARAY[riSRT].GetCntStat(csRslt7) ;
                  bReject4 = DM.ARAY[riSRT].GetCntStat(csRslt4) || DM.ARAY[riSRT].GetCntStat(csRslt8) ;

                       if(bReject1) GetTraySortInfo(false , riSRT , riRJ1 , SortInfo);
                  else if(bReject2) GetTraySortInfo(false , riSRT , riRJ2 , SortInfo);
                  else if(bReject3) GetTraySortInfo(false , riSRT , riRJ3 , SortInfo);
                  else if(bReject4) GetTraySortInfo(false , riSRT , riRJ4 , SortInfo);

                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false;
                  Step.iCycle++;
                  return false;

        case  12: MoveActr(aiSRT_FrstDnUp , SortInfo.bDn[0]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ScndDnUp , SortInfo.bDn[1]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ThrdDnUp , SortInfo.bDn[2]?ccFwd:ccBwd);
                  MoveActr(aiSRT_FrthDnUp , SortInfo.bDn[3]?ccFwd:ccBwd);

                  MoveMotr(miSRT_XExpd , piSRT_XExpdReject);

                       if(SortInfo.iAray == riRJ1) {MoveMotr(miSRT_XPckr , piSRT_XPckrReject1); MoveMotr(miSRT_YPckr , piSRT_YPckrReject1); }
                  else if(SortInfo.iAray == riRJ2) {MoveMotr(miSRT_XPckr , piSRT_XPckrReject2); MoveMotr(miSRT_YPckr , piSRT_YPckrReject2); }
                  else if(SortInfo.iAray == riRJ3) {MoveMotr(miSRT_XPckr , piSRT_XPckrReject3); MoveMotr(miSRT_YPckr , piSRT_YPckrReject3); }
                  else if(SortInfo.iAray == riRJ4) {MoveMotr(miSRT_XPckr , piSRT_XPckrReject4); MoveMotr(miSRT_YPckr , piSRT_YPckrReject4); }
                  Step.iCycle++;
                  return false;


        case  13: if(!AT_Done (aiSRT_FrstDnUp )) return false ;
                  if(!AT_Done (aiSRT_ScndDnUp )) return false ;
                  if(!AT_Done (aiSRT_ThrdDnUp )) return false ;
                  if(!AT_Done (aiSRT_FrthDnUp )) return false ;

                  if(!MT_GetStopInpos(miSRT_XPckr)) return false ;
                  if(!MT_GetStopInpos(miSRT_YPckr)) return false ;
                  if(!MoveMotr(miSRT_XExpd , piSRT_XExpdReject)) return false ;

                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrRJTPlace);

                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrRJTPlace)) return false;

                  if(SortInfo.bDn[0]) {IO_SetY(ySRT_Vcc1 , false); IO_SetY(ySRT_Ejct1 , true); }
                  if(SortInfo.bDn[1]) {IO_SetY(ySRT_Vcc2 , false); IO_SetY(ySRT_Ejct2 , true); }
                  if(SortInfo.bDn[2]) {IO_SetY(ySRT_Vcc3 , false); IO_SetY(ySRT_Ejct3 , true); }
                  if(SortInfo.bDn[3]) {IO_SetY(ySRT_Vcc4 , false); IO_SetY(ySRT_Ejct4 , true); }

                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if(!m_tmDelay.OnDelay(true, OM.DevOptn.iPlceDelay)) return false;
                  bExistRjct = DM.ARAY[riSRT].GetCntStat(csRslt1) ||
                               DM.ARAY[riSRT].GetCntStat(csRslt2) ||
                               DM.ARAY[riSRT].GetCntStat(csRslt3) ||
                               DM.ARAY[riSRT].GetCntStat(csRslt4) ||
                               DM.ARAY[riSRT].GetCntStat(csRslt5) ||
                               DM.ARAY[riSRT].GetCntStat(csRslt6) ||
                               DM.ARAY[riSRT].GetCntStat(csRslt7) ||
                               DM.ARAY[riSRT].GetCntStat(csRslt8) ;



                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);

                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove))return false ;

                  ShiftTraySortInfo(SortInfo);
                  if(SortInfo.bDn[0]) {IO_SetY(ySRT_Ejct1 , false); }
                  if(SortInfo.bDn[1]) {IO_SetY(ySRT_Ejct2 , false); }
                  if(SortInfo.bDn[2]) {IO_SetY(ySRT_Ejct3 , false); }
                  if(SortInfo.bDn[3]) {IO_SetY(ySRT_Ejct4 , false); }

                  //리퀘스트 랏엔드시에 조
                  if( DM.ARAY[riSRT].CheckAllStat(csEmpty) &&
                      DM.ARAY[riZIG].CheckAllStat(csEmpty)){
                      MoveMotr(miSRT_XPckr , piSRT_XPckrPocket);
                      MoveMotr(miSRT_YPckr , piSRT_YPckrPocket);
                      MoveMotr(miSRT_XExpd , piSRT_XExpdPocket);
                  }
                  else {
                      Step.iCycle = 0;
                      return true;
                  }

        case  17: if(!MoveMotr(miSRT_XPckr , piSRT_XPckrPocket)) return false ;
                  if(!MoveMotr(miSRT_YPckr , piSRT_YPckrPocket)) return false ;
                  if(!MoveMotr(miSRT_XExpd , piSRT_XExpdPocket)) return false ;
                  Step.iCycle = 0;
                  return true;

    }
}
//---------------------------------------------------------------------------
bool CSort::CyclePlceULD(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10:
                  GetTraySortInfo(false , riSRT , riULD , SortInfo);

                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false;
                  Step.iCycle++;
                  return false;

        case  12: MoveActr(aiSRT_FrstDnUp , SortInfo.bDn[0]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ScndDnUp , SortInfo.bDn[1]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ThrdDnUp , SortInfo.bDn[2]?ccFwd:ccBwd);
                  MoveActr(aiSRT_FrthDnUp , SortInfo.bDn[3]?ccFwd:ccBwd);

                  MoveMotr(miSRT_YPckr , piSRT_YPckrPlace);
                  MoveMotr(miSRT_XPckr , piSRT_XPckrPlace);

                  MoveMotr(miSRT_XExpd , piSRT_XExpdULDTray);
                  Step.iCycle++;
                  return false;

        case  13: if(!AT_Done (aiSRT_FrstDnUp )) return false ;
                  if(!AT_Done (aiSRT_ScndDnUp )) return false ;
                  if(!AT_Done (aiSRT_ThrdDnUp )) return false ;
                  if(!AT_Done (aiSRT_FrthDnUp )) return false ;

                  if(!MT_GetStopInpos(miSRT_XPckr)) return false ;
                  if(!MT_GetStopInpos(miSRT_YPckr)) return false ;
                  if(!MoveMotr(miSRT_XExpd , piSRT_XExpdULDTray)) return false ;

                  if(!ULD.GetInWorkPos()) return false ;


                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrULDPlace);

                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrULDPlace)) return false;

                  if(SortInfo.bDn[0]){IO_SetY(ySRT_Vcc1 , false ); IO_SetY(ySRT_Ejct1 , true);}
                  if(SortInfo.bDn[1]){IO_SetY(ySRT_Vcc2 , false ); IO_SetY(ySRT_Ejct2 , true);}
                  if(SortInfo.bDn[2]){IO_SetY(ySRT_Vcc3 , false ); IO_SetY(ySRT_Ejct3 , true);}
                  if(SortInfo.bDn[3]){IO_SetY(ySRT_Vcc4 , false ); IO_SetY(ySRT_Ejct4 , true);}

                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if(!m_tmDelay.OnDelay(true, OM.DevOptn.iPlceDelay)) return false;
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false ;
                  ShiftTraySortInfo(SortInfo);
                  if(SortInfo.bDn[0]) {IO_SetY(ySRT_Ejct1 , false); }
                  if(SortInfo.bDn[1]) {IO_SetY(ySRT_Ejct2 , false); }
                  if(SortInfo.bDn[2]) {IO_SetY(ySRT_Ejct3 , false); }
                  if(SortInfo.bDn[3]) {IO_SetY(ySRT_Ejct4 , false); }


                  //리퀘스트 랏엔드시에 조
                  if( DM.ARAY[riSRT].CheckAllStat(csEmpty) &&
                      DM.ARAY[riZIG].CheckAllStat(csEmpty)){
                      MoveMotr(miSRT_XPckr , piSRT_XPckrPocket);
                      //MoveMotr(miSRT_YPckr , piSRT_YPckrPocket);
                      MoveMotr(miSRT_XExpd , piSRT_XExpdPocket);
                  }
                  else {
                      Step.iCycle = 0;
                      return true;
                  }

        case  17: if(!MoveMotr(miSRT_XPckr , piSRT_XPckrPocket)) return false ;
                  //if(!MoveMotr(miSRT_YPckr , piSRT_YPckrPocket)) return false ; //충돌 될까 후달림.
                  if(!MoveMotr(miSRT_XExpd , piSRT_XExpdPocket)) return false ;

                  MoveMotr(miSRT_YPckr , piSRT_YPckrPocket);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveMotr(miSRT_YPckr , piSRT_YPckrPocket)) return false ; //충돌 될까 후달림.
                  Step.iCycle=0 ;
                  return true ;






    }
}
//---------------------------------------------------------------------------
bool CSort::CyclePickZIG(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;
    AnsiString sTemp1 ;
    bool bVacErr1 ;
    bool bVacErr2 ;
    bool bVacErr3 ;
    bool bVacErr4 ;
    bool bExistVacErr ;

    static bool bDown[MAX_TOOL_SOCK_COL] = {false ,false ,false ,false};

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(m_bRetryZigPick) { //그냥 제거 했을 경우 삽질 하는것 클리어.
                      if(DM.ARAY[riZIG].CheckAllStat(csEmpty)) m_bRetryZigPick = false ;

                  }




                  GetSockSortInfo(true , riSRT , riZIG , SortInfo);
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  MoveActr(aiSRT_AlgnFwBw , ccFwd);

                  m_bRetryStgPick = false ;

                  //MoveActr(aiSRT_FrstDnUp , !OM.CmnOptn.bSkip1stTol ? ccFwd : ccBwd);
                  //MoveActr(aiSRT_ScndDnUp , !OM.CmnOptn.bSkip2ndTol ? ccFwd : ccBwd);
                  //MoveActr(aiSRT_ThrdDnUp , !OM.CmnOptn.bSkip3thTol ? ccFwd : ccBwd);
                  //MoveActr(aiSRT_FrthDnUp , !OM.CmnOptn.bSkip4thTol ? ccFwd : ccBwd); //sun 툴유즈 옵션 추가하기.

                  MoveActr(aiSRT_FrstDnUp , SortInfo.bDn[0]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ScndDnUp , SortInfo.bDn[1]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ThrdDnUp , SortInfo.bDn[2]?ccFwd:ccBwd);
                  MoveActr(aiSRT_FrthDnUp , SortInfo.bDn[3]?ccFwd:ccBwd);

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false;
                  MoveMotr(miSRT_XExpd , piSRT_XExpdZig);
                  MoveMotr(miSRT_XPckr , piSRT_XPckrZig);
                  MoveMotr(miSRT_YPckr , piSRT_YPckrZig);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miSRT_XExpd , piSRT_XExpdZig)) return false;
                  if(!MoveMotr(miSRT_XPckr , piSRT_XPckrZig)) return false;
                  if(!MoveMotr(miSRT_YPckr , piSRT_YPckrZig)) return false;
                  if(!AT_Done (aiSRT_FrstDnUp )) return false ;
                  if(!AT_Done (aiSRT_ScndDnUp )) return false ;
                  if(!AT_Done (aiSRT_ThrdDnUp )) return false ;
                  if(!AT_Done (aiSRT_FrthDnUp )) return false ;

                  if(!MoveActr(aiSRT_AlgnFwBw , ccFwd)) return false ;

                  IO_SetY(ySRT_Ejct1 , false);
                  IO_SetY(ySRT_Ejct2 , false);
                  IO_SetY(ySRT_Ejct3 , false);
                  IO_SetY(ySRT_Ejct4 , false);

                  //임시로.
                  //if(SortInfo.bDn[0]) IO_SetY(ySRT_Vcc1 , true);
                  //if(SortInfo.bDn[1]) IO_SetY(ySRT_Vcc2 , true);
                  if(SortInfo.bDn[2]) IO_SetY(ySRT_Vcc3 , true);
                  //if(SortInfo.bDn[3]) IO_SetY(ySRT_Vcc4 , true);



                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrZIGPick);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrZIGPick)) return false;


                  if(SortInfo.bDn[0]) IO_SetY(ySRT_Vcc1 , true);
                  if(SortInfo.bDn[1]) IO_SetY(ySRT_Vcc2 , true);
                  if(SortInfo.bDn[2]) IO_SetY(ySRT_Vcc3 , true);
                  if(SortInfo.bDn[3]) IO_SetY(ySRT_Vcc4 , true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        case  14: if(!m_tmDelay.OnDelay(true , 50)) return false ;
                  MoveActr(aiSRT_AlgnFwBw , ccBwd) ;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  15: if((!SortInfo.bDn[0]||IO_GetX(xSRT_VccSnsr1))&&
                     (!SortInfo.bDn[1]||IO_GetX(xSRT_VccSnsr2))&&
                     (!SortInfo.bDn[2]||IO_GetX(xSRT_VccSnsr3))&&
                     (!SortInfo.bDn[3]||IO_GetX(xSRT_VccSnsr4))){
                      MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                      Step.iCycle++;
                      return false;
                  }
                  if(!m_tmDelay.OnDelay(true, OM.DevOptn.iPickDelay)) return false;
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false ;
                  if(!MoveActr(aiSRT_AlgnFwBw , ccBwd)) return false  ;
                  m_tmDelay.Clear();

                  Step.iCycle++;
                  return false ;

        case  17: bVacErr1 = SortInfo.bDn[0]&&!IO_GetX(xSRT_VccSnsr1) && !OM.MstOptn.bVacErrIgnr ;
                  bVacErr2 = SortInfo.bDn[1]&&!IO_GetX(xSRT_VccSnsr2) && !OM.MstOptn.bVacErrIgnr ;
                  bVacErr3 = SortInfo.bDn[2]&&!IO_GetX(xSRT_VccSnsr3) && !OM.MstOptn.bVacErrIgnr ;
                  bVacErr4 = SortInfo.bDn[3]&&!IO_GetX(xSRT_VccSnsr4) && !OM.MstOptn.bVacErrIgnr ;

                  bExistVacErr = bVacErr1 || bVacErr2 || bVacErr3 || bVacErr4 ;

                  if(bExistVacErr && !m_tmDelay.OnDelay(true , 1000)) return false ;

                  sTemp1 = "" ;
                  if(bVacErr1){sTemp1+="1번";}//IO_SetY(ySRT_Vcc1 , false);}
                  if(bVacErr2){sTemp1+="2번";}//IO_SetY(ySRT_Vcc2 , false);}
                  if(bVacErr3){sTemp1+="3번";}//IO_SetY(ySRT_Vcc3 , false);}
                  if(bVacErr4){sTemp1+="4번";}//IO_SetY(ySRT_Vcc4 , false);}
                  sTemp1 = "Align Zig " + sTemp1 ;

                  ShiftSockSortInfo(SortInfo , bVacErr1,
                                               bVacErr2,
                                               bVacErr3,
                                               bVacErr4);
                  //if(SortInfo.bDn[0]&&IO_GetX(xSRT_VccSnsr1)){DM.ARAY[riSRT].SetStat(0,0,DM.ARAY[ScmGetFrntSTGAray()].GetStat(0,0)); DM.ARAY[ScmGetFrntSTGAray()].SetStat(0,0,csEmpty);}
                  //if(SortInfo.bDn[1]&&IO_GetX(xSRT_VccSnsr2)){DM.ARAY[riSRT].SetStat(0,1,DM.ARAY[ScmGetFrntSTGAray()].GetStat(0,1)); DM.ARAY[ScmGetFrntSTGAray()].SetStat(0,1,csEmpty);}
                  //if(SortInfo.bDn[2]&&IO_GetX(xSRT_VccSnsr3)){DM.ARAY[riSRT].SetStat(0,2,DM.ARAY[ScmGetFrntSTGAray()].GetStat(0,2)); DM.ARAY[ScmGetFrntSTGAray()].SetStat(0,2,csEmpty);}
                  //if(SortInfo.bDn[3]&&IO_GetX(xSRT_VccSnsr4)){DM.ARAY[riSRT].SetStat(0,3,DM.ARAY[ScmGetFrntSTGAray()].GetStat(0,3)); DM.ARAY[ScmGetFrntSTGAray()].SetStat(0,3,csEmpty);}


                  if(sTemp1 != "" && !OM.MstOptn.bVacErrIgnr &&
                     (bVacErr1||
                      bVacErr2||
                      bVacErr3||
                      bVacErr4)){
                      m_bVacErr[0] = bVacErr1 ;
                      m_bVacErr[1] = bVacErr2 ;
                      m_bVacErr[2] = bVacErr3 ;
                      m_bVacErr[3] = bVacErr4 ;
                      EM_SetDisp(false);
                      EM_SetErrMsg(eiSRT_VaccMiss , sTemp1.c_str());
                      m_bZigPickErrDisp = true ;
                      //sun 오퍼레이션 타이머에서 FormVacErr 디스플레이.
                  }


                  Step.iCycle = 0;
                  return true;


    }

}
//---------------------------------------------------------------------------
bool CSort::CyclePlceSTG(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: GetSockSortInfo(false , riSRT , riSTF , SortInfo);
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  MoveActr(aiSRT_FrstDnUp , SortInfo.bDn[0]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ScndDnUp , SortInfo.bDn[1]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ThrdDnUp , SortInfo.bDn[2]?ccFwd:ccBwd);
                  MoveActr(aiSRT_FrthDnUp , SortInfo.bDn[3]?ccFwd:ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false;
                  MoveMotr(miSRT_XExpd , piSRT_XExpdPocket);
                  MoveMotr(miSRT_XPckr , piSRT_XPckrPocket);
                  MoveMotr(miSRT_YPckr , piSRT_YPckrPocket);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miSRT_XExpd , piSRT_XExpdPocket)) return false;
                  if(!MoveMotr(miSRT_XPckr , piSRT_XPckrPocket)) return false;
                  if(!MoveMotr(miSRT_YPckr , piSRT_YPckrPocket)) return false;
                  if(!AT_Done (aiSRT_FrstDnUp )) return false ;
                  if(!AT_Done (aiSRT_ScndDnUp )) return false ;
                  if(!AT_Done (aiSRT_ThrdDnUp )) return false ;
                  if(!AT_Done (aiSRT_FrthDnUp )) return false ;


                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrSTGPlace );
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrSTGPlace))return false;

                  if(SortInfo.bDn[0]) { IO_SetY(ySRT_Vcc1 , false); IO_SetY(ySRT_Ejct1 , true); }
                  if(SortInfo.bDn[1]) { IO_SetY(ySRT_Vcc2 , false); IO_SetY(ySRT_Ejct2 , true); }
                  if(SortInfo.bDn[2]) { IO_SetY(ySRT_Vcc3 , false); IO_SetY(ySRT_Ejct3 , true); }
                  if(SortInfo.bDn[3]) { IO_SetY(ySRT_Vcc4 , false); IO_SetY(ySRT_Ejct4 , true); }

                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPlceDelay)) return false;
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);

                  /*
                  const int iOSTRslt = 0 ;
                  const int iVTRslt  = 1 ;
                  const int iVSNRslt = 2 ;
                  */

                  //sub Stat Clear.
                  DM.ARAY[riSTF].SetSubStat(0,csGood);
                  DM.ARAY[riSTF].SetSubStat(1,csGood);
                  DM.ARAY[riSTF].SetSubStat(2,csGood);




                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false;
                  ShiftSockSortInfo(SortInfo);
                  IO_SetY(ySRT_Ejct1 , false);
                  IO_SetY(ySRT_Ejct2 , false);
                  IO_SetY(ySRT_Ejct3 , false);
                  IO_SetY(ySRT_Ejct4 , false);

                  Step.iCycle = 0;
                  return true;
    }
}
//---------------------------------------------------------------------------
//One Cycle.
bool CSort::CyclePickLDR(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;
    int rSRT, cSRT, rLDR, cLDR;
    AnsiString sTemp1 ;
    bool bVacErr1 , bVacErr2 , bVacErr3 , bVacErr4 ; //[MAX_TOOL_SOCK_COL] ;
    static double dCurTickTime = GetTickTime();
    static double dPreTickTime = GetTickTime();
    bool bSkipVacErrBySel ; //배큠에러 창에서 선택한 것에 따라 상황에 맞게 배큠에러 스킵.
    int  iPickerNo ;
    int  iVacErrCnt ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //if(DM.ARAY[riLDR].GetCntStat(csUnkwn) < 10) DM.ARAY[riLDR].SetStat(csUnkwn);
                  //if(DM.ARAY[riRJ1].GetCntStat(csEmpty) < 20) DM.ARAY[riRJ1].SetStat(csEmpty);
                  //if(DM.ARAY[riRJ2].GetCntStat(csEmpty) < 20) DM.ARAY[riRJ2].SetStat(csEmpty);
                  //if(DM.ARAY[riRJ3].GetCntStat(csEmpty) < 20) DM.ARAY[riRJ3].SetStat(csEmpty);
                  //if(DM.ARAY[riRJ4].GetCntStat(csEmpty) < 20) DM.ARAY[riRJ4].SetStat(csEmpty);
                  //if(DM.ARAY[riULD].GetCntStat(csEmpty) < 10) DM.ARAY[riULD].SetStat(csEmpty);

                  GetTraySortInfo(true , riSRT , riLDR , SortInfo);


                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  MoveActr(aiSRT_FrstDnUp , SortInfo.bDn[0]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ScndDnUp , SortInfo.bDn[1]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ThrdDnUp , SortInfo.bDn[2]?ccFwd:ccBwd);
                  MoveActr(aiSRT_FrthDnUp , SortInfo.bDn[3]?ccFwd:ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false ;
                  MoveMotr(miSRT_XExpd , piSRT_XExpdLDRTray);
                  MoveMotr(miSRT_XPckr , piSRT_XPckrPick  );
                  MoveMotr(miSRT_YPckr , piSRT_YPckrPick   );

                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miSRT_XExpd , piSRT_XExpdLDRTray)) return false ;
                  if(!MoveMotr(miSRT_XPckr , piSRT_XPckrPick   )) return false ;
                  if(!MoveMotr(miSRT_YPckr , piSRT_YPckrPick   )) return false ;
                  if(!AT_Done (aiSRT_FrstDnUp )) return false ;
                  if(!AT_Done (aiSRT_ScndDnUp )) return false ;
                  if(!AT_Done (aiSRT_ThrdDnUp )) return false ;
                  if(!AT_Done (aiSRT_FrthDnUp )) return false ;

                  IO_SetY(ySRT_Ejct1 , false);
                  IO_SetY(ySRT_Ejct2 , false);
                  IO_SetY(ySRT_Ejct3 , false);
                  IO_SetY(ySRT_Ejct4 , false);

                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrLDRPick);


                  //두번으로 나눠서 찝을 경우 제외를 위해.
                  if(!DM.ARAY[riSRT].GetCntStat(csUnkwn)) {
                      dCurTickTime = GetTickTime();
                      m_dTickTime = dCurTickTime - dPreTickTime;
                      dPreTickTime = dCurTickTime ;
                  }



                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrLDRPick)) return false ;

                  if(SortInfo.bDn[0])IO_SetY(ySRT_Vcc1 ,true );
                  if(SortInfo.bDn[1])IO_SetY(ySRT_Vcc2 ,true );
                  if(SortInfo.bDn[2])IO_SetY(ySRT_Vcc3 ,true );
                  if(SortInfo.bDn[3])IO_SetY(ySRT_Vcc4 ,true );

                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if((!SortInfo.bDn[0]||IO_GetX(xSRT_VccSnsr1))&&
                     (!SortInfo.bDn[1]||IO_GetX(xSRT_VccSnsr2))&&
                     (!SortInfo.bDn[2]||IO_GetX(xSRT_VccSnsr3))&&
                     (!SortInfo.bDn[3]||IO_GetX(xSRT_VccSnsr4))){
                      MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                      Step.iCycle++;
                      return false ;
                  }

                  if(!m_tmDelay.OnDelay(true, OM.DevOptn.iPickDelay)) return false;
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false ;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        case  16: bVacErr1 = SortInfo.bDn[0]&&!IO_GetX(xSRT_VccSnsr1) && !OM.MstOptn.bVacErrIgnr ;
                  bVacErr2 = SortInfo.bDn[1]&&!IO_GetX(xSRT_VccSnsr2) && !OM.MstOptn.bVacErrIgnr ;
                  bVacErr3 = SortInfo.bDn[2]&&!IO_GetX(xSRT_VccSnsr3) && !OM.MstOptn.bVacErrIgnr ;
                  bVacErr4 = SortInfo.bDn[3]&&!IO_GetX(xSRT_VccSnsr4) && !OM.MstOptn.bVacErrIgnr ;

                  iVacErrCnt = 0 ;
                  //배큠에러 카운트 샘.
                  if(bVacErr1)iVacErrCnt++;
                  if(bVacErr2)iVacErrCnt++;
                  if(bVacErr3)iVacErrCnt++;
                  if(bVacErr4)iVacErrCnt++;

                  //픽커가 올라오고도 배큠이 안켜져 있으면 1초간 기다려 본다.
                  if(iVacErrCnt && !m_tmDelay.OnDelay(true, m_iVacErrStat == vsNomal ? 1000 : 0)) return false ;

                  sTemp1 = "" ;
                  if(bVacErr1){sTemp1+="1번";}//IO_SetY(ySRT_Vcc1 , false);}
                  if(bVacErr2){sTemp1+="2번";}//IO_SetY(ySRT_Vcc2 , false);}
                  if(bVacErr3){sTemp1+="3번";}//IO_SetY(ySRT_Vcc3 , false);}
                  if(bVacErr4){sTemp1+="4번";}//IO_SetY(ySRT_Vcc4 , false);}
                  sTemp1 = "Loader " + sTemp1 ;

                  ShiftTraySortInfo(SortInfo , bVacErr1,
                                               bVacErr2,
                                               bVacErr3,
                                               bVacErr4); //여기까지가 에러 안났을때 패턴.

                  bSkipVacErrBySel = m_iVacErrStat != vsNomal ;
                  if(!bSkipVacErrBySel) { //노멀상태여서 배큠에러 에러 띄운다.
                      if(sTemp1 != "" && !OM.MstOptn.bVacErrIgnr &&  
                         (bVacErr1||
                          bVacErr2||
                          bVacErr3||
                          bVacErr4)) {

                          m_bVacErr[0] = bVacErr1 ;
                          m_bVacErr[1] = bVacErr2 ;
                          m_bVacErr[2] = bVacErr3 ;
                          m_bVacErr[3] = bVacErr4 ;
                          EM_SetDisp(false);
                          EM_SetErrMsg(eiLDR_VaccMiss , sTemp1.c_str());
                          m_bLdrPickErrDisp = true ;
                          //sun 오퍼레이션 타이머에서 FormVacErr 디스플레이.
                      }
                  }
                  else { //배큠에러 스킵으로 인해서 자제 없음 판정시. 혹은 에러없이 픽을 잘 한경우 이미 없지만 한번더 지우는건데 그냥 나둠.
                      for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //어레이에 찝을것이 없는놈 다운 해제.
                          iPickerNo = i + SortInfo.iToolShft ;

                          if(iPickerNo         >= MAX_TOOL_SOCK_COL                  ) break ; //쉬프트 합친게 넘어가면 브렉
                          if(SortInfo.iCol + i >= DM.ARAY[SortInfo.iAray].GetMaxCol()) break ; //트레이 영역 넘어가면 브렉
                          if(!SortInfo.bDn[iPickerNo]                                ) continue ; //다운플레그 아니면 컨티뉴,

                          DM.ARAY[SortInfo.iAray].SetStat(SortInfo.iRow , SortInfo.iCol + i , csEmpty             );
                      }
                  }

                  ///////////////////////////
                  if(m_iVacErrStat == vsErrSkipPick) { //이번픽은 스킵
                      m_iVacErrStat = vsNomal ;
                  }
                  else if(m_iVacErrStat == vsErrSkipRow ) { //이번줄만 스킵
                      if(DM.ARAY[riLDR].GetCntStat(csUnkwn)%OM.DevInfo.iLDRColCnt){
                          m_iVacErrStat = vsNomal ;
                      }
                  }
                  else if(m_iVacErrStat == vsErrSkipTray) { //이번 트레이스킵
                      if(DM.ARAY[riLDR].GetCntStat(csUnkwn)==0){
                          m_iVacErrStat = vsNomal ;
                      }
                  }
                  else if(m_iVacErrStat == vsErrSkipRowOut) { //이번줄만 스킵후 트레이 아웃.
                      if(DM.ARAY[riLDR].GetCntStat(csUnkwn)%OM.DevInfo.iLDRColCnt){
                          DM.ARAY[riLDR].SetStat(csEmpty);
                          m_iVacErrStat = vsNomal ;
                      }
                  }
                  else if(m_iVacErrStat == vsErrSkipRowOutLE) { //이번줄말 스킵후 트레이 아웃 후 랏엔드 요청.
                      //찝고 에러난것들을 계산했을때 그Row의 마지막 칩까지면 그 Row 스킵 끝나서 노말상태로 전환.
                      if(DM.ARAY[riLDR].GetCntStat(csUnkwn)%OM.DevInfo.iLDRColCnt){
                          DM.ARAY[riLDR].SetStat(csEmpty);
                          m_iVacErrStat = vsNomal ;
                          LOT.SetRqstLotEnd(true);
                      }
                  }
                  /////////////////////////////////

                  
                  

                  Step.iCycle = 0 ;
                  return true ;
    }
}
//---------------------------------------------------------------------------
bool CSort::CyclePlceZIG(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: GetSockSortInfo(false , riSRT , riZIG , SortInfo);
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  MoveActr(aiSRT_FrstDnUp , SortInfo.bDn[0]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ScndDnUp , SortInfo.bDn[1]?ccFwd:ccBwd);
                  MoveActr(aiSRT_ThrdDnUp , SortInfo.bDn[2]?ccFwd:ccBwd);
                  MoveActr(aiSRT_FrthDnUp , SortInfo.bDn[3]?ccFwd:ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false;
                  MoveMotr(miSRT_XExpd , piSRT_XExpdZig);
                  MoveMotr(miSRT_XPckr , piSRT_XPckrZig);
                  MoveMotr(miSRT_YPckr , piSRT_YPckrZig);
                  Step.iCycle++;                                              
                  return false;

        case  12: if(!MoveMotr(miSRT_XExpd , piSRT_XExpdZig)) return false;
                  if(!MoveMotr(miSRT_XPckr , piSRT_XPckrZig)) return false;
                  if(!MoveMotr(miSRT_YPckr , piSRT_YPckrZig)) return false;
                  if(!AT_Done (aiSRT_FrstDnUp )) return false ;
                  if(!AT_Done (aiSRT_ScndDnUp )) return false ;
                  if(!AT_Done (aiSRT_ThrdDnUp )) return false ;
                  if(!AT_Done (aiSRT_FrthDnUp )) return false ;

                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrZIGPlace);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrZIGPlace))return false;

                  if(SortInfo.bDn[0]) { IO_SetY(ySRT_Vcc1 , false); IO_SetY(ySRT_Ejct1 , true ); }
                  if(SortInfo.bDn[1]) { IO_SetY(ySRT_Vcc2 , false); IO_SetY(ySRT_Ejct2 , true ); }
                  if(SortInfo.bDn[2]) { IO_SetY(ySRT_Vcc3 , false); IO_SetY(ySRT_Ejct3 , true ); }
                  if(SortInfo.bDn[3]) { IO_SetY(ySRT_Vcc4 , false); IO_SetY(ySRT_Ejct4 , true ); }

                  /*
                  if(!IO_GetX(xSRT_VccSnsr1) &&
                     !IO_GetX(xSRT_VccSnsr2) &&
                     !IO_GetX(xSRT_VccSnsr3) &&
                     !IO_GetX(xSRT_VccSnsr4)){
                      MoveMotr(miSRT_ZPckr , piSRT_ZPckrWait);
                      ShiftSockSortInfo(SortInfo);
                      Step.iCycle=15;
                      return false;
                  }   //sun Pick 싸이클 딜레이 먹기 전에 온되면 올리기.
                  */
                  /*
                  IO_SetY(ySRT_Vcc1 , SortInfo.bDn[0]);
                  IO_SetY(ySRT_Vcc2 , SortInfo.bDn[1]);
                  IO_SetY(ySRT_Vcc3 , SortInfo.bDn[2]);
                  IO_SetY(ySRT_Vcc4 , SortInfo.bDn[3]);

                  IO_SetY(ySRT_Ejct1 , SortInfo.bDn[0]);
                  IO_SetY(ySRT_Ejct2 , SortInfo.bDn[1]);
                  IO_SetY(ySRT_Ejct3 , SortInfo.bDn[2]);
                  IO_SetY(ySRT_Ejct4 , SortInfo.bDn[3]);
                  */
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  14: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPlceDelay)) return false;
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);




                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove)) return false;
                  ShiftSockSortInfo(SortInfo);
                  DM.ARAY[riZIG].ChangeStat(csUnkwn,csWork);
                  IO_SetY(ySRT_Ejct1 , false);
                  IO_SetY(ySRT_Ejct2 , false);
                  IO_SetY(ySRT_Ejct3 , false);
                  IO_SetY(ySRT_Ejct4 , false);

                  MoveMotr(miSRT_XPckr , piSRT_XPckrZig);
                  MoveMotr(miSRT_YPckr , piSRT_YPckrZig);
                  MoveMotr(miSRT_XExpd , piSRT_XExpdZig);

                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miSRT_XPckr , piSRT_XPckrZig)) return false ;
                  if(!MoveMotr(miSRT_YPckr , piSRT_YPckrZig)) return false ;
                  if(!MoveMotr(miSRT_XExpd , piSRT_XExpdZig)) return false ;


                  Step.iCycle = 0;
                  return true;
    }
}


//---------------------------------------------------------------------------
bool CSort::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bSRT_ZPckrWait = MT_GetCmdPos(miSRT_ZPckr) <= PM.GetValue(miSRT_ZPckr , pvSRT_ZPckrWait)+0.1 ;
    bool bSRT_ZPckrMove = MT_GetCmdPos(miSRT_ZPckr) <= PM.GetValue(miSRT_ZPckr , pvSRT_ZPckrMove)+0.1 ; //무브가 웨이트 보다 수치가 적으면 안된다.
    bool bCrossBorder = false ;
    bool bToolMoving  = !MT_GetStop(miSRT_XPckr) || !MT_GetStop(miSRT_YPckr) ;


    if(_iMotr == miSRT_YPckr || _iMotr == miSRT_XPckr) {
        // 각 경계를 넘을때는 Z축이 Wait으로 넘어가야 한다.
        // 경계 안쪽으로 돌아다닐때는 Move로 이동.
        //     |  STG  |
        //      -------
        //LDR  |  ZIG  |  ULD
        //     |  RJT  |
        const int iLeftZone = 0 ;
        const int iRearZone = 1 ;
        const int iFrntZone = 2 ;
        const int iRghtZone = 3 ;

        int iCrntZone ; //현재 위치.
        const double dLeftMargin = OM.DevInfo.dULDColPitch * (MAX_TOOL_SOCK_COL - 1) + //여기까지가 마지막 픽커가 Rject1 0번 Col 포지션.
                                   0.1 ;
                                                                            //인포지션 마진.
        const double dRghtMargin = OM.DevInfo.dULDColPitch * (OM.DevInfo.iULDColCnt - 1) +
                                   0.1 ;
                                   
        const double dRearMargin = 10.0 ;

             if(MT_GetCmdPos(miSRT_XPckr) <  PM.GetValue(miSRT_XPckr , pvSRT_XPckrReject1Stt) - dLeftMargin) iCrntZone = iLeftZone ;
        else if(MT_GetCmdPos(miSRT_XPckr) >  PM.GetValue(miSRT_XPckr , pvSRT_XPckrReject4Stt) + dRghtMargin) iCrntZone = iRghtZone ;
        else if(MT_GetCmdPos(miSRT_YPckr) <= PM.GetValue(miSRT_YPckr , pvSRT_YPckrZig       ) + dRearMargin) iCrntZone = iFrntZone ;
        else                                                                                                 iCrntZone = iRearZone ;

        int iTrgtZone ; //가려는 위치.
        double dSRT_XPckrTrgPos ;
        double dSRT_YPckrTrgPos ;

        if(_iMotr == miSRT_YPckr) {
            dSRT_XPckrTrgPos = MT_GetCmdPos(miSRT_XPckr);
            dSRT_YPckrTrgPos = GetMotrPos(_iMotr , _iPstnId);
        }
        else {
            dSRT_XPckrTrgPos = GetMotrPos(_iMotr , _iPstnId);
            dSRT_YPckrTrgPos = MT_GetCmdPos(miSRT_YPckr);
        }

             if(dSRT_XPckrTrgPos <  PM.GetValue(miSRT_XPckr , pvSRT_XPckrReject1Stt) - dLeftMargin) iTrgtZone = iLeftZone ;
        else if(dSRT_XPckrTrgPos >  PM.GetValue(miSRT_XPckr , pvSRT_XPckrReject4Stt) + dRghtMargin) iTrgtZone = iRghtZone ;
        else if(dSRT_YPckrTrgPos <= PM.GetValue(miSRT_YPckr , pvSRT_YPckrZig       ) + dRearMargin) iTrgtZone = iFrntZone ;
        else                                                                                        iTrgtZone = iRearZone ;

        bCrossBorder = (iTrgtZone != iCrntZone) ;

    }

    if(_iMotr == miSRT_YPckr){
        //if(bCrossBorder && !bSRT_ZPckrWait) { sMsg = AnsiString(MT_GetName(miSRT_ZPckr))+" is not Wait Postion" ; bRet = false ;}

        if(!bSRT_ZPckrMove) { sMsg = AnsiString(MT_GetName(miSRT_ZPckr))+" is not Move Postion" ; bRet = false ;}
             if(_iPstnId == piSRT_YPckrWait      ){}
        else if(_iPstnId == piSRT_YPckrPick      ){}
        else if(_iPstnId == piSRT_YPckrZig       ){}
        else if(_iPstnId == piSRT_YPckrPocket    ){}
        else if(_iPstnId == piSRT_YPckrReject1Stt){}
        else if(_iPstnId == piSRT_YPckrReject2Stt){}
        else if(_iPstnId == piSRT_YPckrReject3Stt){}
        else if(_iPstnId == piSRT_YPckrReject4Stt){}
        else if(_iPstnId == piSRT_YPckrPlace     ){}
        else if(_iPstnId == piSRT_YPckrReject1   ){}
        else if(_iPstnId == piSRT_YPckrReject2   ){}
        else if(_iPstnId == piSRT_YPckrReject3   ){}
        else if(_iPstnId == piSRT_YPckrReject4   ){}
        else                                      {sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion"        ; bRet = false ; }
    }
    else if(_iMotr == miSRT_ZPckr){
        if(bToolMoving) { sMsg = AnsiString("Tool is Moving") ; bRet = false ;}
             if(_iPstnId == piSRT_ZPckrWait       ){}
        else if(_iPstnId == piSRT_ZPckrMove       ){}
        else if(_iPstnId == piSRT_ZPckrSTGPick    ){}
        else if(_iPstnId == piSRT_ZPckrSTGPlace   ){}
        else if(_iPstnId == piSRT_ZPckrSTGPlace   ){}
        else if(_iPstnId == piSRT_ZPckrZIGPick    ){}
        else if(_iPstnId == piSRT_ZPckrZIGPlace   ){}
        else if(_iPstnId == piSRT_ZPckrRJTPick    ){}
        else if(_iPstnId == piSRT_ZPckrRJTPlace   ){}
        else if(_iPstnId == piSRT_ZPckrLDRPick    ){}
        else if(_iPstnId == piSRT_ZPckrLDRPlace   ){}
        else if(_iPstnId == piSRT_ZPckrULDPick    ){}
        else if(_iPstnId == piSRT_ZPckrULDPlace   ){}
        else                                       {sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ; }

    }
    else if(_iMotr == miSRT_XPckr){
        //if(bCrossBorder && !bSRT_ZPckrWait) { sMsg = AnsiString(MT_GetName(miSRT_ZPckr))+" is not Wait Postion" ; bRet = false ;}
        if(!bSRT_ZPckrMove) { sMsg = AnsiString(MT_GetName(miSRT_ZPckr))+" is not Move Postion" ; bRet = false ;}
             if(_iPstnId == piSRT_XPckrWait      ){}
        else if(_iPstnId == piSRT_XPckrPickStt   ){}
        else if(_iPstnId == piSRT_XPckrZig       ){}
        else if(_iPstnId == piSRT_XPckrPocket    ){}
        else if(_iPstnId == piSRT_XPckrReject1Stt){}
        else if(_iPstnId == piSRT_XPckrReject2Stt){}
        else if(_iPstnId == piSRT_XPckrReject3Stt){}
        else if(_iPstnId == piSRT_XPckrReject4Stt){}
        else if(_iPstnId == piSRT_XPckrPlaceStt  ){}
        else if(_iPstnId == piSRT_XPckrPick      ){}
        else if(_iPstnId == piSRT_XPckrReject1   ){}
        else if(_iPstnId == piSRT_XPckrReject2   ){}
        else if(_iPstnId == piSRT_XPckrReject3   ){}
        else if(_iPstnId == piSRT_XPckrReject4   ){}
        else if(_iPstnId == piSRT_XPckrPlace     ){}
        else                                      {sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion"        ; bRet = false ; }
    }
    else if(_iMotr == miSRT_XExpd){
        if(!bSRT_ZPckrMove) { sMsg = AnsiString(MT_GetName(miSRT_ZPckr))+" is Down Postion" ; bRet = false ;}
             if(_iPstnId == piSRT_XExpdWait      ){}
        else if(_iPstnId == piSRT_XExpdLDRTray   ){}
        else if(_iPstnId == piSRT_XExpdPocket    ){}
        else if(_iPstnId == piSRT_XExpdReject    ){}
        else if(_iPstnId == piSRT_XExpdZig       ){}
        else if(_iPstnId == piSRT_XExpdULDTray   ){}
        else                                      {sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion"        ; bRet = false ; }
    }

    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}
//---------------------------------------------------------------------------

bool CSort::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;


    /*
      = 0 ,  //1번째 피커
          ,  //2번째 피커
          ,  //3번째 피커
          ,  //4번째 피커
          ,  //얼라인 1,2번
    aiSRT_Al34UpDn      ,  //얼라인 3,4번

    */

    if(_iActr == aiSRT_FrstDnUp){
        if(_bFwd == ccFwd) {
        }
    }
    else if(_iActr == aiSRT_ScndDnUp){
        if(_bFwd == ccFwd) {
        }
    }
    else if(_iActr == aiSRT_ThrdDnUp){
        if(_bFwd == ccFwd) {
        }
    }
    else if(_iActr == aiSRT_FrthDnUp){
        if(_bFwd == ccFwd) {
        }
    }
    else if(_iActr == aiSRT_AlgnFwBw){
        if(_bFwd == ccFwd) {
        }
    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }
    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }
    return bRet ;
}
bool CSort::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CSort::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CSort::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CSort::SetLastCmd()
{
    //m_dLastIdxPos = MT_GetCmdPos(miLDR_YIndx);
    return ; //이파트는 필요 없다.

}

bool CSort::CheckMoved()
{
    //if(m_dLastIdxPos != MT_GetCmdPos(miLDR_YIndx)) return true  ; //이파트는 필요 없다.
    //else                                           return false ;

}

bool CSort::CheckStop()
{
    if(!MT_GetStop(miLDR_YIndx)) return false ;

    //if(!AT_Done(aiLDR_IndxUpUp)) return false ;

    return true ;
}

void CSort::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";

    if ( _bLoad ) {
        //UserINI.Load(sPath, "Member" , "m_dLastIdxPos"  , m_dLastIdxPos   );
        UserINI.Load(sPath, "Member" , "m_iVacErrStat"  , m_iVacErrStat   );
        UserINI.Load(sPath, "Member" , "m_bRetryZigPick", m_bRetryZigPick );
        UserINI.Load(sPath, "Member" , "m_bRetryStgPick", m_bRetryStgPick );

    }
    else {
        UserINI.ClearFile(sPath) ;
        //.Save(sPath, "Member" , "m_dLastIdxPos"  , m_dLastIdxPos   );
        UserINI.Save(sPath, "Member" , "m_iVacErrStat"  , m_iVacErrStat   );
        UserINI.Save(sPath, "Member" , "m_bRetryZigPick", m_bRetryZigPick );
        UserINI.Save(sPath, "Member" , "m_bRetryStgPick", m_bRetryStgPick );

    }
}



