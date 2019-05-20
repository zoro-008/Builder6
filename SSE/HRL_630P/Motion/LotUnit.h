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

//연속 작업 때문에 Lot클래스는 그냥 큐 에 랏넘버 담아 놨다가 메가진 로딩시에 꺼내다가 ARAY에 붙여 주는 역활만 하고 데이터 관리는 SPC에서 한다.
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
        //Que 관련.
        void       PushLotNo     (AnsiString _sLotNo); //큐에 집어 넣음.
        AnsiString GetLotNo      (int _iNo          ); //참조만.
        AnsiString PopLotNo      (void              ); //실제로 꺼냄. 큐에서.
        void       DeleteLotNo   (AnsiString _sLotNo); //해당 랏넘버를 지운다.
        void       DeleteLotNo   (int        _iLotNo); //다음차례만 지운다.
        void       DeleteLotNoAll(void              ); //큐를 모두 지운다.
        int        GetLotCnt     (void              ); //랏의 수자 리턴.

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
