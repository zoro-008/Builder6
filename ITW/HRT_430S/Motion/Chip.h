/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Usage  :                                                                */
/* Date   : 2004.05.26                                                     */
/* Modify : 2004.07.24                                                     */
/* Programmed by Primer(YongSik, Lee).                                     */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//---------------------------------------------------------------------------
#ifndef ChipH
#define ChipH

#include "stdio.h"

enum EN_CHIP_STAT {  //�ó��� ���� Ĩ�� ���µ�.
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
    csRslt16 = 16 ,
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

enum EN_FAIL_STAT {
    fsGood = 0 ,
    fsLed  = 1 , //
    fsZnnr = 2 , //
    fsDrop = 3 ,
    fsAll  = 4 ,

    MAX_FAIL_STAT
};


class CChip
{
     public:

    private:
        EN_CHIP_STAT  m_iStat    ; //Chip Status.
        EN_FAIL_STAT  m_iFsStat  ; //Fail Status.

        double m_dVf ;  //Chip Vf Value.
        double m_dVr ; //Chip Zenner Value.

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

        //Set
        void __fastcall SetStat    (EN_CHIP_STAT _iStat  ) { m_iStat   = _iStat   ;}
        void __fastcall SetFailStat(EN_FAIL_STAT _iFsStat) { m_iFsStat = _iFsStat ;}
        void __fastcall SetVf      (double _dVf          ) { m_dVf     = _dVf     ;}
        void __fastcall SetVr      (double _dVr          ) { m_dVr     = _dVr     ;}

        //Get
        EN_CHIP_STAT __fastcall GetStat    () { return m_iStat   ; }
        EN_FAIL_STAT __fastcall GetFailStat() { return m_iFsStat ; }
        double       __fastcall GetVf      () { return m_dVf   ; }
        double       __fastcall GetVr      () { return m_dVr   ; }



        //Load.
        void __fastcall Load(bool IsLoad , FILE *fp);

        //Operator
        void operator = (CChip& rhs) { m_iStat = rhs.GetStat() ; }
};
                                                                                                        
//---------------------------------------------------------------------------
#endif
