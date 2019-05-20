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
#define MAX_LOT_CNT 1
#define AUTO_LOT "AutoLot"

//���� �۾� ������ LotŬ������ �׳� ť �� ���ѹ� ��� ���ٰ� �ް��� �ε��ÿ� �����ٰ� ARAY�� �ٿ� �ִ� ��Ȱ�� �ϰ� ������ ������ SPC���� �Ѵ�.
class CLotQue
{
    public :
        CLotQue(void);
        ~CLotQue(void);

        void Init();
        void Close();

    protected :
        AnsiString m_sLotNo[MAX_LOT_CNT] ;
        bool       m_bLotOpen ;
        bool       m_bLotEnded ;

        void       LoadSave(bool _bLoad);

    public :
        //Que ����.
        void       PushLotNo     (AnsiString _sLotNo); //ť�� ���� ����.
        AnsiString GetLotNo      (int _iNo          ); //������.
        AnsiString PopLotNo      (void              ); //������ ����. ť����.
        void       DeleteLotNo   (AnsiString _sLotNo); //�ش� ���ѹ��� �����.
        void       DeleteLotNo   (int        _iLotNo); //�������ʸ� �����.
        void       DeleteLotNoAll(void              ); //ť�� ��� �����.
        int        GetLotCnt     (void              ); //���� ���� ����.

        //Lot Processing.
        void  LotOpen   (AnsiString _sLotNo , AnsiString _sJobFile = ""  , AnsiString _sOperator ="");
        bool  GetLotOpen();
        void  Reset     ();

        void  LotNext  (void);
        void  LotDelete(    );
        void  LotEnd   (void);
};

extern CLotQue LTQ ;

//---------------------------------------------------------------------------
#endif
