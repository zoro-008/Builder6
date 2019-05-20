/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Usage  :                                                                */
/* Date   : 2004.05.26                                                     */
/* Modify : 2004.10.15                                                     */
/* Programmed by Primer(YongSik, Lee).                                     */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
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

//���� �۾� ������ LotŬ������ �׳� ť �� ���ѹ� ��� ���ٰ� �ް��� �ε��ÿ�  �����ٰ� ARAY�� �ٿ� �ִ� ��Ȱ�� �ϰ� ������ ������ SPC���� �Ѵ�.
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
};

extern CLotQue LTQ ;

//---------------------------------------------------------------------------
#endif
