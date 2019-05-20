/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Usage  :                                                                */
/* Date   : 2004.05.26                                                     */
/* Modify : 2004.07.24                                                     */
/* Programmed by Primer(YongSik, Lee).                                     */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//---------------------------------------------------------------------------
#ifndef ChipH
#define ChipH
#include <system.hpp>

//#include "stdio.h"

enum EN_CHIP_STAT {  //�ó��� ���� Ĩ�� ���µ�.
    csRetFail= -1 , //�̰� �׳� ���Ͽ����� ���� ��ҽÿ��� ���� �ȉ�.

    csRslt0  = 0  , //
    csRslt1  = 1  , //
    csRslt2  = 2  , //
    csRslt3  = 3  , //
    csRslt4  = 4  , //
    csRslt5  = 5  , //
    csRslt6  = 6  , //
    csRslt7  = 7  , //
    csRslt8  = 8  , //
    csRslt9  = 9  , //
    csRslt10 = 10 , //
    csRslt11 = 11 , //
    csRslt12 = 12 , //
    csRslt13 = 13 , //
    csRslt14 = 14 , //
    csRslt15 = 15 , //
    csRslt16 = 16 , //
    csNone   = 17 , ////��Ʈ���̳� ī��Ʈ�� ���� ����.                                            >
    csMask   = 18 , ////PKG�� ������ äũ�� �س��� ����.                                          >  ��� ������ PKG ���°�.
    csEmpty  = 19 , ////ī��Ʈ�� ��Ʈ���� ���� ���� Ȥ�� �����е忡 �����ڱ��� ���°��. ���κ곡 >
    csUnkwn  = 20 , ////�۾��� �ִ��� ������ �𸣴� ���.
    csWork   = 21 , //// �۾� ��.
    csWait   = 22 , //
    csFail   = 23 , //
    csEror   = 24 , //
    csGood   = 25 , //
    csEtc    = 26 , //
    csToBuf  = 27 , // To Buffer       �ǳ����� �ΰ� ���� spAll �϶��� Visible = true
    csFromBuf= 28 , // Buffer To Here  �ǳ����� �ΰ� ���� spAll �϶��� Visible = true
    csDrop   = 29 , // Drop ������ ���. ����ݵ�ü �߰� ��û. JS

    MAX_CHIP_STAT

};

#define MAX_SUBSTAT 5



class CChip
{
     public:

    private:
        EN_CHIP_STAT  m_iStat    ; //Chip Status.
        EN_CHIP_STAT  m_iSubStat[MAX_SUBSTAT] ;

        bool          m_iTrg     ;
        //Init.
        void __fastcall Init(void);

    public:
        __fastcall  CChip(void);
        __fastcall ~CChip(void);

        //Check Chip Status & Result.
        bool __fastcall CheckStat  (EN_CHIP_STAT _iStat) { return (m_iStat == _iStat); }
        bool __fastcall CheckExist (void               ) { return (m_iStat != csNone && m_iStat != csMask && m_iStat != csEmpty); }

        bool __fastcall CheckRst   (void               ) { return (m_iStat != csNone && m_iStat != csUnkwn && m_iStat != csWait && m_iStat != csEror &&
                                                                   m_iStat != csEtc  && m_iStat != csMask  && m_iStat != csWork && m_iStat != csGood ); }

        //Check Triger.
        bool __fastcall CheckTrg   (void               ) { return m_iTrg; }

        //Set
        void __fastcall SetStat(EN_CHIP_STAT _iStat) { m_iStat = _iStat ; }

        //Set Triger.
        void __fastcall SetTrg (bool _iTrg) { m_iTrg = _iTrg; }
        
        EN_CHIP_STAT __fastcall GetStat() { return m_iStat ; }

        //Sub Stat
        void __fastcall SetSubStat(int _iIdx , EN_CHIP_STAT _iStat) {
            if(_iIdx < 0 || _iIdx >= MAX_SUBSTAT)return ;
            m_iSubStat[_iIdx] = _iStat ;
        }
        EN_CHIP_STAT __fastcall GetSubStat(int _iIdx) {
            if(_iIdx < 0 || _iIdx >= MAX_SUBSTAT)return csNone;
            return m_iSubStat[_iIdx] ;
        }

        //Operator
        void operator = (CChip& rhs) { memcpy(this , &rhs ,sizeof(CChip)); /*m_iStat = rhs.GetStat() ;*/ }
};

//---------------------------------------------------------------------------
#endif
