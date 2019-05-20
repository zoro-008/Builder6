//---------------------------------------------------------------------------
#ifndef LotUnitH
#define LotUnitH

#include <ExtCtrls.hpp>
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include <Grids.hpp>

//---------------------------------------------------------------------------
#define MAX_LOT_CNT 30
#define AUTO_LOT "AutoLot"

//#define USE_SRIEAL_LOT

class CLot //�Ѱ��� ���� ������ ����.
{
    public :
        CLot(void);
        ~CLot(void);

        void Init();
        void Close();

    protected :
        String m_sLotNo     ;
        String m_sJobName   ;
        String m_sEpoxyLotNo;  //20160421 �߰� ����

        bool   m_bLotOpen   ;
        bool   m_bLotEnded  ;

        bool   m_bRqstLotEnd ; //������ ��û.

        void   LoadSave(bool _bLoad);

    public :
        //Lot Processing.
        void  LotOpen   (AnsiString _sLotNo , AnsiString _sJobName);
        bool  GetLotOpen();
        void  Reset     ();
        void  LotEnd    ();
        bool  GetLotEnd ();



        //20160421 �߰� ����
        String GetLotNo     () {return m_sLotNo      ; }
        String GetJobName   () {return m_sJobName    ; }

        String GetEpoxyLotNo(                   ) {return m_sEpoxyLotNo         ;}
        void   SetEpoxyLotNo(String _sEpoxyLotNo) {m_sEpoxyLotNo = _sEpoxyLotNo ;}

        bool  GetRqstLotEnd(          ){return m_bRqstLotEnd ;}
        void  SetRqstLotEnd(bool _bVal){m_bRqstLotEnd = _bVal;}
};
#ifndef USE_SRIEAL_LOT
extern CLot LOT ;
#endif



/*


//���� �۾� ������ LotQueŬ������ �׳� ť �� ���ѹ� ��� ���ٰ� �ް��� �ε��ÿ�  �����ٰ� ARAY�� �ٿ� �ִ� ��Ȱ�� �ϰ� ������ ������ SPC���� �Ѵ�.
class CLotQue
{
    public :
        CLotQue(void);
        ~CLotQue(void);

        void Init();
        void Close();

    protected :
        String m_sLotNo[MAX_LOT_CNT] ;
        String m_sCrntLotNo          ;
        String m_sAutoLotNo          ; //LotOpenâ�� Auto��ư Ŭ������ �ڵ����� ���� �ɶ� ���ѹ� �ص�.

        bool   m_bLotOpen  ;
        bool   m_bLotEnded ;

        bool   m_bRqstLotEnd ; //������ ��û.

        void   LoadSave(bool _bLoad);

    public :
        //Que ����.
        void   PushLotNo     (String _sLotNo); //ť�� ���� ����.
        String GetLotNo      (int    _iNo   ); //������.
        String PopLotNo      (void          ); //������ ����. ť����.
        void   DeleteLotNo   (String _sLotNo); //�ش� ���ѹ��� �����.
        void   DeleteLotNo   (int    _iLotNo); //�������ʸ� �����.
        void   DeleteLotNoAll(void          ); //ť�� ��� �����.
        int    GetLotCnt     (void          ); //���� ���� ����.
        bool   GetLotEnd     (void          ); //��
        String GetCrntLotNo  (void          ); //��

        String GetAutoLotNo  (void          );  void SetAutoLotNo (String _sAutoLotNo);


        //Lot Processing.
        void  LotOpen   ();
        bool  GetLotOpen();
        void  Reset     ();

        void  LotNext  (void);
        void  LotDelete(    );
        void  LotEnd   (void);

        bool  GetRqstLotEnd(          ){return m_bRqstLotEnd ;}
        void  SetRqstLotEnd(bool _bVal){m_bRqstLotEnd = _bVal;}

};
#ifdef USE_SRIEAL_LOT
extern CLotQue LTQ ;
#endif
*/
//---------------------------------------------------------------------------
#endif
