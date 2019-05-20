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
//sun AT.AllDone�����.
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



    if(_bPick) { //�δ� �������� ���.
        _tInfo.isPick = true ;
        _tInfo.iTargetStat = csUnkwn ;

        bRet = FindChip(_tInfo.iAray , _tInfo.iTargetStat , _tInfo.iRow , _tInfo.iCol);

        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //���� Ĩ�� ��� �ִ°��. �������� ����Ʈ ��Ų��.
            if(bPckrUsage[i] && DM.ARAY[_tInfo.iTool].GetStat(0,i)==csEmpty)break ;
            _tInfo.iToolShft++;

        }

        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
            iPickerNo = i + _tInfo.iToolShft ;

            if(iPickerNo       >= MAX_TOOL_SOCK_COL          ) break ;
            if(_tInfo.iCol + i >= DM.ARAY[_iAray].GetMaxCol()) break ;

            if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==csEmpty                             ) _tInfo.bDn[iPickerNo] = true  ; //������� ���ش�.
            if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)!=_tInfo.iTargetStat) _tInfo.bDn[iPickerNo] = false ; //�������� Ÿ���� �ƴ�.
            if(!bPckrUsage[iPickerNo]                                                          ) _tInfo.bDn[iPickerNo] = false ; //�� �Ⱦ�.
        }

    }
    else { //Place
        _tInfo.isPick = false ;

        //�������� ��ġ�� �̷��� �Ǿ� �ִ� �� ���ʿ� �ִ� �� ���� ����.
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

        //���� ����.
        if(_tInfo.iTargetStat == csRslt1 || //���� �ڿ� ����� ����� �ÿ� ��ƴ�� ���� ä�� �ֱ����� bFindFrstChip�� �̿��Ѵ�.
           _tInfo.iTargetStat == csRslt2 ||
           _tInfo.iTargetStat == csRslt3 ||
           _tInfo.iTargetStat == csRslt4 ){
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //�������� ��Ĩ�� �ƴѰ��.
                if(DM.ARAY[_tInfo.iTool].GetStat(0,i)==_tInfo.iTargetStat) break ;
                _tInfo.iToolShft++;
            }
            bool bFindFrstChip = false;
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
                iPickerNo = i + _tInfo.iToolShft ;
                if(iPickerNo       >= MAX_TOOL_SOCK_COL          ) break ;
                if(_tInfo.iCol + i >= DM.ARAY[_iAray].GetMaxCol()) break ;
                if(bFindFrstChip && DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)!=_tInfo.iTargetStat) break ; //�߰��� ��� ����� �� ����.

                if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==_tInfo.iTargetStat        ){_tInfo.bDn[iPickerNo] = true  ; bFindFrstChip = true ;}//�� Ÿ���ΰ��� ����.
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)!=csEmpty ) _tInfo.bDn[iPickerNo] = false ; //�������� ����� �ƴ�.
                if(!bPckrUsage[iPickerNo]                                                ) _tInfo.bDn[iPickerNo] = false ; //�� �Ⱦ�.
            }
        }
        //���ʱ���
        else if(_tInfo.iTargetStat == csGood){ //Good�� �ϴ� ��ƴ�� �� ���ܵ� �Ǹ��������� ������� �ȵɻ��̶� �׳� ��ƴ�� ����.
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //�������� ��Ĩ�� �ƴѰ��.
                if(DM.ARAY[_tInfo.iTool].GetStat(0,i)==_tInfo.iTargetStat) break ;
                _tInfo.iToolShft++;
            }
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
                iPickerNo = i + _tInfo.iToolShft ;
                if(iPickerNo       >= MAX_TOOL_SOCK_COL          ) break ;
                if(_tInfo.iCol + i >= DM.ARAY[_iAray].GetMaxCol()) break ;

                if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==_tInfo.iTargetStat        ) _tInfo.bDn[iPickerNo] = true  ; //�� Ÿ���ΰ��� ����.
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)!=csEmpty ) _tInfo.bDn[iPickerNo] = false ; //�������� ����� �ƴ�.
                if(!bPckrUsage[iPickerNo]                                                ) _tInfo.bDn[iPickerNo] = false ; //�� �Ⱦ�.
            }
        }
        //�����ʾƷ� ����.
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
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
                iPickerNo = _tInfo.iToolShft - i;
                if(iPickerNo       < 0 ) continue ;
                if(_tInfo.iCol - i < 0 ) continue ;
                if(bFindFrstChip && DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)!=_tInfo.iTargetStat) break ; //�߰��� ��� ����� �� ����.

                if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==_tInfo.iTargetStat        ){_tInfo.bDn[iPickerNo] = true  ; bFindFrstChip = true;}//�� Ÿ���ΰ��� ����.
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol - i)!=csEmpty ) _tInfo.bDn[iPickerNo] = false ; //�������� ����� �ƴ�.
                if(!bPckrUsage[iPickerNo]                                                ) _tInfo.bDn[iPickerNo] = false ; //�� �Ⱦ�.
            }

        }
    }

    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��� �ٿ��Ų��.
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

    if(_bPick) { //�δ� �������� ���.
        _tInfo.isPick = true ;
             if(_tInfo.iAray == riZIG) _tInfo.iTargetStat = csWork ;
        else if(_tInfo.iAray == riSTF) _tInfo.iTargetStat = csNone ; //���� �� ;;; ������ ���� ;
        else if(_tInfo.iAray == riSTL) _tInfo.iTargetStat = csNone ;
        else if(_tInfo.iAray == riSTR) _tInfo.iTargetStat = csNone ;
        else if(_tInfo.iAray == riSTB) _tInfo.iTargetStat = csNone ;

        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
            iPickerNo = i ;

            if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==csEmpty                             ) _tInfo.bDn[iPickerNo] = true  ; //������� ���ش�.
            if(_iAray == riZIG){
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)!=_tInfo.iTargetStat) _tInfo.bDn[iPickerNo] = false ; //�������� Ÿ���� �ƴ�.
            }
            else {
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)==csWork ) _tInfo.bDn[iPickerNo] = false ; //�������� Ÿ���� �ƴ�.
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)==csUnkwn) _tInfo.bDn[iPickerNo] = false ; //�������� Ÿ���� �ƴ�.
                if(DM.ARAY[_tInfo.iAray].GetStat(_tInfo.iRow , _tInfo.iCol + i)==csEmpty) _tInfo.bDn[iPickerNo] = false ; //�������� Ÿ���� �ƴ�.
            }

            if(!bPckrUsage[iPickerNo]                                                   ) _tInfo.bDn[iPickerNo] = false ; //�� �Ⱦ�.
        }
    }
    else { //Place
        _tInfo.isPick = false ;

             if(_tInfo.iAray == riZIG) _tInfo.iTargetStat = csUnkwn ;
        else if(_tInfo.iAray == riSTF) _tInfo.iTargetStat = csWork  ;
        else if(_tInfo.iAray == riSTL) _tInfo.iTargetStat = csWork  ;
        else if(_tInfo.iAray == riSTB) _tInfo.iTargetStat = csWork  ;
        else if(_tInfo.iAray == riSTR) _tInfo.iTargetStat = csWork  ;

        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
            iPickerNo = i ;


            if(DM.ARAY[_tInfo.iTool].GetStat(0,iPickerNo)==_tInfo.iTargetStat) _tInfo.bDn[iPickerNo] = true  ; //
            if(DM.ARAY[_tInfo.iAray].GetStat(0,iPickerNo)!=csEmpty           ) _tInfo.bDn[iPickerNo] = false ; //Ÿ���� �ƴ�.
            if(!bPckrUsage[iPickerNo]                                        ) _tInfo.bDn[iPickerNo] = false ; //�� �Ⱦ�.
        }
    }

    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��� �ٿ��Ų��.
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

    if(_tInfo.isPick) { //�δ� �������� ���.
        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
            iPickerNo = i + _tInfo.iToolShft ;

            if(iPickerNo       >= MAX_TOOL_SOCK_COL                ) break ; //����Ʈ ��ģ�� �Ѿ�� �귺
            if(_tInfo.iCol + i >= DM.ARAY[_tInfo.iAray].GetMaxCol()) break ; //Ʈ���� ���� �Ѿ�� �귺
            if(!_tInfo.bDn[iPickerNo]                              ) continue ; //�ٿ��÷��� �ƴϸ� ��Ƽ��,
            if( bVacErr[iPickerNo]                                 ) continue ; //���޿���.

            DM.ARAY[_tInfo.iTool].SetStat(0           , iPickerNo       , _tInfo.iTargetStat);
            DM.ARAY[_tInfo.iAray].SetStat(_tInfo.iRow , _tInfo.iCol + i , csEmpty           );

        }

    }
    else { //Place
        //������ �ؿ��� �����ξֵ�..
        if(_tInfo.iTargetStat == csRslt5 ||
           _tInfo.iTargetStat == csRslt6 ||
           _tInfo.iTargetStat == csRslt7 ||
           _tInfo.iTargetStat == csRslt8 ){
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
                iPickerNo = _tInfo.iToolShft - i ;
                if(iPickerNo       < 0 ) break ;
                if(_tInfo.iCol - i < 0 ) break ;
                if(!_tInfo.bDn[iPickerNo]) continue ; //�ٿ��÷��� �ƴϸ� ��Ƽ��,

                DM.ARAY[_tInfo.iTool].SetStat(0           , iPickerNo       , csEmpty           );
                DM.ARAY[_tInfo.iAray].SetStat(_tInfo.iRow , _tInfo.iCol - i , _tInfo.iTargetStat);
            }
        }
        //���� ������ �����ξֵ�.
        else {
            for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
                iPickerNo = i + _tInfo.iToolShft ;
                if(iPickerNo       >= MAX_TOOL_SOCK_COL                ) break ;
                if(_tInfo.iCol + i >= DM.ARAY[_tInfo.iAray].GetMaxCol()) break ;
                if(!_tInfo.bDn[iPickerNo]                              ) continue ; //�ٿ��÷��� �ƴϸ� ��Ƽ��,

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

    if(_tInfo.isPick) { //�δ� �������� ���.
        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
            iPickerNo = i ;

            if(!_tInfo.bDn[iPickerNo]                              ) continue ; //�ٿ��÷��� �ƴϸ� ��Ƽ��,
            if( bVacErr[i]                                         ) continue ; //���޿���.

            DM.ARAY[_tInfo.iTool].SetStat(0 , iPickerNo , DM.ARAY[_tInfo.iAray].GetStat(0 , iPickerNo));
            DM.ARAY[_tInfo.iAray].SetStat(0 , iPickerNo , csEmpty                                     );



        }

    }
    else { //Place
        for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
            iPickerNo = i ;
            if(!_tInfo.bDn[iPickerNo]                              ) continue ; //�ٿ��÷��� �ƴϸ� ��Ƽ��,
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

        //Rslt1~Rslt4���� ����ϴ°��� �׳� ǮƮ���� ����
        //Rslt5~Rslt8�� ��� �Ͽ� ���� �ϴ� ���� 5~8�� �ؿ��� ���� ä���.
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
                dPos-= SortInfo.iToolShft * OM.DevInfo.dLDRColPitch ; //���� ��°�� Ĩ�� �������.
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

bool CSort::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CSort::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CSort::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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

bool CSort::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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
    const int iRjtNo = _iRjtNo - 1 ; //�ۿ����� 1,2,3,4 �̷��� ���ڸ� �ִ´�.
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



bool CSort::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
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

        //�ó��� ���� ���� �߿���..
        //�ٲܶ� �����ؾ� �Ѵ�.
        //�ϳ��� ���� �ٲٸ� Ȧ�� �� �� ����.
        //****��� ���нÿ� SrtCheckAllStat(riSRT , csEmpty) �̰� ������ �÷��̽� ���� �ϰ� �ٽ� ����. ****************
        //������������ ���=====================================================
        bool isCyclePickSTG_Srt =  ScmCheckAllStat (riSRT , csEmpty) ;
        bool isCyclePickSTG_Stg = !ScmCheckAllStat (riSTF , csEmpty) &&
                                  !ScmGetCntStat   (riSTF , csUnkwn) &&
                                  !ScmGetCntStat   (riSTF , csWork ) &&
                                   CStage::scIdle == STG.GetSeqStep();
        bool isCyclePickSTG_Zig = ScmGetCntStat(riZIG , csWork) || //�����϶� ���׿� �־�� ���������� ��µ�.
                                 (LOT.GetRqstLotEnd() && !bLDRExistWork); //������ �ÿ� ��� ��´�.
        bool isCyclePickSTG_Rtr = m_bRetryStgPick ;  //CheckRetryStgPick() ; //�÷��� ���ְ� �Լ��� ��ü. CheckRetryStgPick() �ؼ�.
        bool isCyclePickSTG = (isCyclePickSTG_Srt && isCyclePickSTG_Stg && isCyclePickSTG_Zig) ||
                               isCyclePickSTG_Rtr ;


        //���� ��������===========================================================
        //���� ���� Ʈ���� ������ �÷��̽��ÿ� �ѹ� ���� ����Ŭ ���� ���� ���� Ʈ��.
        bool isCyclePlceRJT_Srt = DM.ARAY[riSRT].GetCntStat(csRslt1) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt2) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt3) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt4) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt5) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt6) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt7) ||
                                  DM.ARAY[riSRT].GetCntStat(csRslt8) ;
        bool isCyclePlceRJT = isCyclePlceRJT_Srt ;

        //�� ��������=============================================================
        bool isCyclePlceULD_Uld = DM.ARAY[riULD].GetCntStat(csEmpty) ;
        bool isCyclePlceULD_Srt = DM.ARAY[riSRT].GetCntStat(csGood) ;
        bool isCyclePlceULD = isCyclePlceULD_Uld && isCyclePlceULD_Srt;

        //���׿� �ϳ� �� ������� ���� ��ó ���� ���� ������ ������ Ȧ��.
        //���׿� ������� ���� �δ����� ������ �ȉ�.. Ȧ��.
        //������ ���׿� ������ ���� ���� ������� ��.
        //Zig���� ���===========================================================
        bool isCyclePickZIG_Zig = ScmGetCntStat  (riZIG , csWork ) ;
        bool isCyclePickZIG_Srt = ScmCheckAllStat(riSRT , csEmpty) ;
        bool isCyclePickZIG_Stg = !ScmGetCntStat  (riSTF , csWork) && CStage::scIdle == STG.GetSeqStep();
        bool isCyclePickZIG_Rtr = m_bRetryZigPick ;  //CheckRetryZigPick();
        bool isCyclePickZIG = (isCyclePickZIG_Zig && isCyclePickZIG_Srt && isCyclePickZIG_Stg) ||
                               isCyclePickZIG_Rtr; //�÷��� ���ְ� �Լ��� ��ü. CheckRetryZigPick() �ؼ�.

        //���������� ����===========================================================
        bool isCyclePlceSTG_Stg = ScmCheckAllStat(riSTF , csEmpty)&&
                                  CStage::scIdle == STG.GetSeqStep();
        bool isCyclePlceSTG_Srt = ScmGetCntStat(riSRT , csWork) ;
        bool isCyclePlceSTG = isCyclePlceSTG_Stg && isCyclePlceSTG_Srt;


        //�δ����� ���.========================================================
        bool bCyclePickLDR_Zig = ScmCheckAllStat(riZIG , csEmpty) ;//SrtGetCntStat(riZIG , csEmpty) ; //
        bool bCyclePickLDR_Ldr = DM.ARAY[riLDR].GetCntStat(csUnkwn) /*�ӵ� ������ �ó��� ������ �̵�.&& LDR.GetInWorkPos()*/ ;
        bool bTemp1 =  ScmGetCntStat  (riSRT , csEmpty) ;
        bool bTemp2 = !ScmGetCntStat  (riSRT , csWork ) ;
        bool bTemp3 = !ScmGetCntStat  (riSRT , csRslt1) && !ScmGetCntStat(riSRT , csRslt5) ;
        bool bTemp4 = !ScmGetCntStat  (riSRT , csRslt2) && !ScmGetCntStat(riSRT , csRslt6) ;
        bool bTemp5 = !ScmGetCntStat  (riSRT , csRslt3) && !ScmGetCntStat(riSRT , csRslt7) ;
        bool bTemp6 = !ScmGetCntStat  (riSRT , csRslt4) && !ScmGetCntStat(riSRT , csRslt8) ;
        bool bTemp7 = !ScmGetCntStat  (riSRT , csGood ) ;

        bool bCyclePickLDR_Srt = ScmGetCntStat  (riSRT , csEmpty) &&  //���� ���� ���� �־�� ��.
                                !ScmGetCntStat  (riSRT , csWork ) &&
                                !ScmGetCntStat  (riSRT , csRslt1) && !ScmGetCntStat(riSRT , csRslt5) &&
                                !ScmGetCntStat  (riSRT , csRslt2) && !ScmGetCntStat(riSRT , csRslt6) &&
                                !ScmGetCntStat  (riSRT , csRslt3) && !ScmGetCntStat(riSRT , csRslt7) &&
                                !ScmGetCntStat  (riSRT , csRslt4) && !ScmGetCntStat(riSRT , csRslt8) &&
                                !ScmGetCntStat  (riSRT , csGood ) ;
        bool isCyclePickLDR =  bCyclePickLDR_Zig &&
                               bCyclePickLDR_Ldr &&
                               bCyclePickLDR_Srt ;

        //Zig�� ����.===========================================================
        bool isCyclePlceZIG_Zig = ScmGetCntStat(riZIG , csEmpty) ; //�ѵΰ� �������� ���.
        bool isCyclePlceZIG_Srt = ScmCheckAllStat(riSRT , csUnkwn) || //���� �÷ο�.
                                 (ScmGetCntStat(riSRT , csUnkwn) && LOT.GetRqstLotEnd() && !bLDRExistWork); //������ �÷ο�.
        bool isCyclePlceZIG = isCyclePlceZIG_Zig &&
                              isCyclePlceZIG_Srt ;


        //2014.12.16 ������ �븮 ��û Ȧ��ÿ� ����ó��.
        //���������� �����ؾ� �� �˻糡�� ���� �ְ�
        //���׿� ����ε� ���� �ְ�
        //���� ����� �� ���� �ְų� , UnKown���� ������ Ȧ�峭��. �۾��ڰ� ������ ���� �ؼ� �׷�.
        //Ȧ���Ȳ==============================================================
        //case 1 : ���������� �����ؾ� �Ұ� ���� �ְ� && ���� ����ε�(csWork)�� ���� �ְ� ;
        //case 2 : ���������� �����ؾ� �Ұ� ���� �ְ� && ���� ������(csUnkn)�� ���� �ְ� && ����ξ ���� �� �ִ°��.
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


        //�𸣴� ��Ʈ������.
        if(!OM.MstOptn.bVacErrIgnr) {
            if(DM.ARAY[riSRT].GetStat(0,0)==csEmpty&& IO_GetX(xSRT_VccSnsr1)) EM_SetErrMsg(eiPKG_Unknwn , (DM.ARAY[riSRT].GetName()+"_1").c_str());
            if(DM.ARAY[riSRT].GetStat(0,1)==csEmpty&& IO_GetX(xSRT_VccSnsr2)) EM_SetErrMsg(eiPKG_Unknwn , (DM.ARAY[riSRT].GetName()+"_2").c_str());
            if(DM.ARAY[riSRT].GetStat(0,2)==csEmpty&& IO_GetX(xSRT_VccSnsr3)) EM_SetErrMsg(eiPKG_Unknwn , (DM.ARAY[riSRT].GetName()+"_3").c_str());
            if(DM.ARAY[riSRT].GetStat(0,3)==csEmpty&& IO_GetX(xSRT_VccSnsr4)) EM_SetErrMsg(eiPKG_Unknwn , (DM.ARAY[riSRT].GetName()+"_4").c_str());
        }
        //ī��Ʈ �����.
        if(!OM.MstOptn.bVacErrIgnr) {
            if( DM.ARAY[riSRT].GetStat(0,0)!=csEmpty&&!IO_GetX(xSRT_VccSnsr1)) EM_SetErrMsg(eiPKG_Dispr , (DM.ARAY[riSRT].GetName()+"_1").c_str());
            if( DM.ARAY[riSRT].GetStat(0,1)!=csEmpty&&!IO_GetX(xSRT_VccSnsr2)) EM_SetErrMsg(eiPKG_Dispr , (DM.ARAY[riSRT].GetName()+"_2").c_str());
            if( DM.ARAY[riSRT].GetStat(0,2)!=csEmpty&&!IO_GetX(xSRT_VccSnsr3)) EM_SetErrMsg(eiPKG_Dispr , (DM.ARAY[riSRT].GetName()+"_3").c_str());
            if( DM.ARAY[riSRT].GetStat(0,3)!=csEmpty&&!IO_GetX(xSRT_VccSnsr4)) EM_SetErrMsg(eiPKG_Dispr , (DM.ARAY[riSRT].GetName()+"_4").c_str());
        }


        //�ް��� ����.
        if(!IO_GetX(xSRT_RjtTray1)) EM_SetErrMsg(eiSRT_RJTNeedTray,"Reject Tray 1");
        if(!IO_GetX(xSRT_RjtTray2)) EM_SetErrMsg(eiSRT_RJTNeedTray,"Reject Tray 2");
        if(!IO_GetX(xSRT_RjtTray3)) EM_SetErrMsg(eiSRT_RJTNeedTray,"Reject Tray 3");
        if(!IO_GetX(xSRT_RjtTray4)) EM_SetErrMsg(eiSRT_RJTNeedTray,"Reject Tray 4");

        //ī��Ʈ OUT �Ű��� Ǯ
        if(DM.ARAY[riRJ1].GetCntStat(csEmpty) == 0) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 1");
        if(DM.ARAY[riRJ2].GetCntStat(csEmpty) == 0) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 2");
        if(DM.ARAY[riRJ3].GetCntStat(csEmpty) == 0) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 3");
        if(DM.ARAY[riRJ4].GetCntStat(csEmpty) == 0) EM_SetErrMsg(eiSRT_RejectFull,"Reject Tray 4");


        //���� ���ÿ� ������ ���� ���� ���� �ؾ� ������ ����.
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

        case  10: if(m_bRetryStgPick) { //�׳� ���� ���� ��� ���� �ϴ°� Ŭ����.
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
                  if(bVacErr[0]){sTemp1+="1��";}//IO_SetY(ySRT_Vcc1 , false);}
                  if(bVacErr[1]){sTemp1+="2��";}//IO_SetY(ySRT_Vcc2 , false);}
                  if(bVacErr[2]){sTemp1+="3��";}//IO_SetY(ySRT_Vcc3 , false);}
                  if(bVacErr[3]){sTemp1+="4��";}//IO_SetY(ySRT_Vcc4 , false);}
                  sTemp1 = "Socket " + sTemp1 ;

                  //��Ũ���� ó�� ������ ������� ����.
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

                      //sun ���۷��̼� Ÿ�̸ӿ��� FormVacErr ���÷���.
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

                  //������Ʈ ������ÿ� ��
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


                  //������Ʈ ������ÿ� ��
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
                  //if(!MoveMotr(miSRT_YPckr , piSRT_YPckrPocket)) return false ; //�浹 �ɱ� �Ĵ޸�.
                  if(!MoveMotr(miSRT_XExpd , piSRT_XExpdPocket)) return false ;

                  MoveMotr(miSRT_YPckr , piSRT_YPckrPocket);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveMotr(miSRT_YPckr , piSRT_YPckrPocket)) return false ; //�浹 �ɱ� �Ĵ޸�.
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

        case  10: if(m_bRetryZigPick) { //�׳� ���� ���� ��� ���� �ϴ°� Ŭ����.
                      if(DM.ARAY[riZIG].CheckAllStat(csEmpty)) m_bRetryZigPick = false ;

                  }




                  GetSockSortInfo(true , riSRT , riZIG , SortInfo);
                  MoveMotr(miSRT_ZPckr , piSRT_ZPckrMove);
                  MoveActr(aiSRT_AlgnFwBw , ccFwd);

                  m_bRetryStgPick = false ;

                  //MoveActr(aiSRT_FrstDnUp , !OM.CmnOptn.bSkip1stTol ? ccFwd : ccBwd);
                  //MoveActr(aiSRT_ScndDnUp , !OM.CmnOptn.bSkip2ndTol ? ccFwd : ccBwd);
                  //MoveActr(aiSRT_ThrdDnUp , !OM.CmnOptn.bSkip3thTol ? ccFwd : ccBwd);
                  //MoveActr(aiSRT_FrthDnUp , !OM.CmnOptn.bSkip4thTol ? ccFwd : ccBwd); //sun ������ �ɼ� �߰��ϱ�.

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

                  //�ӽ÷�.
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
                  if(bVacErr1){sTemp1+="1��";}//IO_SetY(ySRT_Vcc1 , false);}
                  if(bVacErr2){sTemp1+="2��";}//IO_SetY(ySRT_Vcc2 , false);}
                  if(bVacErr3){sTemp1+="3��";}//IO_SetY(ySRT_Vcc3 , false);}
                  if(bVacErr4){sTemp1+="4��";}//IO_SetY(ySRT_Vcc4 , false);}
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
                      //sun ���۷��̼� Ÿ�̸ӿ��� FormVacErr ���÷���.
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
    bool bSkipVacErrBySel ; //��Ũ���� â���� ������ �Ϳ� ���� ��Ȳ�� �°� ��Ũ���� ��ŵ.
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


                  //�ι����� ������ ���� ��� ���ܸ� ����.
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
                  //��Ũ���� ī��Ʈ ��.
                  if(bVacErr1)iVacErrCnt++;
                  if(bVacErr2)iVacErrCnt++;
                  if(bVacErr3)iVacErrCnt++;
                  if(bVacErr4)iVacErrCnt++;

                  //��Ŀ�� �ö���� ��Ũ�� ������ ������ 1�ʰ� ��ٷ� ����.
                  if(iVacErrCnt && !m_tmDelay.OnDelay(true, m_iVacErrStat == vsNomal ? 1000 : 0)) return false ;

                  sTemp1 = "" ;
                  if(bVacErr1){sTemp1+="1��";}//IO_SetY(ySRT_Vcc1 , false);}
                  if(bVacErr2){sTemp1+="2��";}//IO_SetY(ySRT_Vcc2 , false);}
                  if(bVacErr3){sTemp1+="3��";}//IO_SetY(ySRT_Vcc3 , false);}
                  if(bVacErr4){sTemp1+="4��";}//IO_SetY(ySRT_Vcc4 , false);}
                  sTemp1 = "Loader " + sTemp1 ;

                  ShiftTraySortInfo(SortInfo , bVacErr1,
                                               bVacErr2,
                                               bVacErr3,
                                               bVacErr4); //��������� ���� �ȳ����� ����.

                  bSkipVacErrBySel = m_iVacErrStat != vsNomal ;
                  if(!bSkipVacErrBySel) { //��ֻ��¿��� ��Ũ���� ���� ����.
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
                          //sun ���۷��̼� Ÿ�̸ӿ��� FormVacErr ���÷���.
                      }
                  }
                  else { //��Ũ���� ��ŵ���� ���ؼ� ���� ���� ������. Ȥ�� �������� ���� �� �Ѱ�� �̹� ������ �ѹ��� ����°ǵ� �׳� ����.
                      for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) { //��̿� �������� ���³� �ٿ� ����.
                          iPickerNo = i + SortInfo.iToolShft ;

                          if(iPickerNo         >= MAX_TOOL_SOCK_COL                  ) break ; //����Ʈ ��ģ�� �Ѿ�� �귺
                          if(SortInfo.iCol + i >= DM.ARAY[SortInfo.iAray].GetMaxCol()) break ; //Ʈ���� ���� �Ѿ�� �귺
                          if(!SortInfo.bDn[iPickerNo]                                ) continue ; //�ٿ��÷��� �ƴϸ� ��Ƽ��,

                          DM.ARAY[SortInfo.iAray].SetStat(SortInfo.iRow , SortInfo.iCol + i , csEmpty             );
                      }
                  }

                  ///////////////////////////
                  if(m_iVacErrStat == vsErrSkipPick) { //�̹����� ��ŵ
                      m_iVacErrStat = vsNomal ;
                  }
                  else if(m_iVacErrStat == vsErrSkipRow ) { //�̹��ٸ� ��ŵ
                      if(DM.ARAY[riLDR].GetCntStat(csUnkwn)%OM.DevInfo.iLDRColCnt){
                          m_iVacErrStat = vsNomal ;
                      }
                  }
                  else if(m_iVacErrStat == vsErrSkipTray) { //�̹� Ʈ���̽�ŵ
                      if(DM.ARAY[riLDR].GetCntStat(csUnkwn)==0){
                          m_iVacErrStat = vsNomal ;
                      }
                  }
                  else if(m_iVacErrStat == vsErrSkipRowOut) { //�̹��ٸ� ��ŵ�� Ʈ���� �ƿ�.
                      if(DM.ARAY[riLDR].GetCntStat(csUnkwn)%OM.DevInfo.iLDRColCnt){
                          DM.ARAY[riLDR].SetStat(csEmpty);
                          m_iVacErrStat = vsNomal ;
                      }
                  }
                  else if(m_iVacErrStat == vsErrSkipRowOutLE) { //�̹��ٸ� ��ŵ�� Ʈ���� �ƿ� �� ������ ��û.
                      //��� �������͵��� ��������� ��Row�� ������ Ĩ������ �� Row ��ŵ ������ �븻���·� ��ȯ.
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
                  }   //sun Pick ����Ŭ ������ �Ա� ���� �µǸ� �ø���.
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
    bool bSRT_ZPckrMove = MT_GetCmdPos(miSRT_ZPckr) <= PM.GetValue(miSRT_ZPckr , pvSRT_ZPckrMove)+0.1 ; //���갡 ����Ʈ ���� ��ġ�� ������ �ȵȴ�.
    bool bCrossBorder = false ;
    bool bToolMoving  = !MT_GetStop(miSRT_XPckr) || !MT_GetStop(miSRT_YPckr) ;


    if(_iMotr == miSRT_YPckr || _iMotr == miSRT_XPckr) {
        // �� ��踦 �������� Z���� Wait���� �Ѿ�� �Ѵ�.
        // ��� �������� ���ƴٴҶ��� Move�� �̵�.
        //     |  STG  |
        //      -------
        //LDR  |  ZIG  |  ULD
        //     |  RJT  |
        const int iLeftZone = 0 ;
        const int iRearZone = 1 ;
        const int iFrntZone = 2 ;
        const int iRghtZone = 3 ;

        int iCrntZone ; //���� ��ġ.
        const double dLeftMargin = OM.DevInfo.dULDColPitch * (MAX_TOOL_SOCK_COL - 1) + //��������� ������ ��Ŀ�� Rject1 0�� Col ������.
                                   0.1 ;
                                                                            //�������� ����.
        const double dRghtMargin = OM.DevInfo.dULDColPitch * (OM.DevInfo.iULDColCnt - 1) +
                                   0.1 ;
                                   
        const double dRearMargin = 10.0 ;

             if(MT_GetCmdPos(miSRT_XPckr) <  PM.GetValue(miSRT_XPckr , pvSRT_XPckrReject1Stt) - dLeftMargin) iCrntZone = iLeftZone ;
        else if(MT_GetCmdPos(miSRT_XPckr) >  PM.GetValue(miSRT_XPckr , pvSRT_XPckrReject4Stt) + dRghtMargin) iCrntZone = iRghtZone ;
        else if(MT_GetCmdPos(miSRT_YPckr) <= PM.GetValue(miSRT_YPckr , pvSRT_YPckrZig       ) + dRearMargin) iCrntZone = iFrntZone ;
        else                                                                                                 iCrntZone = iRearZone ;

        int iTrgtZone ; //������ ��ġ.
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
      = 0 ,  //1��° ��Ŀ
          ,  //2��° ��Ŀ
          ,  //3��° ��Ŀ
          ,  //4��° ��Ŀ
          ,  //����� 1,2��
    aiSRT_Al34UpDn      ,  //����� 3,4��

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
bool CSort::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CSort::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // ���͸� �����϶� ���� �Լ�.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CSort::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CSort::SetLastCmd()
{
    //m_dLastIdxPos = MT_GetCmdPos(miLDR_YIndx);
    return ; //����Ʈ�� �ʿ� ����.

}

bool CSort::CheckMoved()
{
    //if(m_dLastIdxPos != MT_GetCmdPos(miLDR_YIndx)) return true  ; //����Ʈ�� �ʿ� ����.
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



