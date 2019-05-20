//---------------------------------------------------------------------------

#ifndef PstnManH
#define PstnManH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <ExtCtrls.hpp>

#include "UtilDefine.h"
#include "TEdit.h"

#define MAX_PSTN 10


enum EN_VALUE_TYPE {
    vtPosition = 0 ,
    vtOffset       ,
    vtFixValue     ,

    MAX_VALUE_TYPE
};


const int PSTN_CNT[MAX_MOTR] = {
    MAX_PSTN_MOTR0  ,
    MAX_PSTN_MOTR1  ,
    MAX_PSTN_MOTR2  ,
};


class CPstnMan
{
    public :
        CPstnMan();
        ~CPstnMan();

    protected :
        struct SValue {
            AnsiString    sName  ;
            EN_VALUE_TYPE iType  ;
            double        dValue ;
        };

        struct SGridSelInfo {
            int  iGrid ;
            bool bDown ;
            int  iCol  ;
            int  iRow  ;
            int  iX    ;
            int  iY    ;
            int  iColX ;
            int  iRowY ;
        }GridInfo;

        //int m_iMaxMotor ;  ���߿� CMotors���� �޾ƿ´�.
        SValue Value[MAX_MOTR][MAX_PSTN] ;

        AnsiString m_sCrntDev ;

        //Grid & Event.
        TStringGrid * m_pPstnList[MAX_MOTR];
        void __fastcall pPstnListDrawCell (TObject *Sender, int ACol, int ARow , const TRect &Rect, TGridDrawState State) ;
        void __fastcall pPstnListMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall pPstnListMouseUp  (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

        //Edit & Event.
        TEdit  * m_pPstnEdit[MAX_MOTR] ;
        void __fastcall pPstnEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift);
        void __fastcall pPstnEditExit   (TObject *Sender);

        void LoadLastInfo() ;
        void SaveLastInfo() ;

    public :
        double        GetValue (int _iAxisNo , EN_PSTN_VALUE _iPstnNo);
        EN_VALUE_TYPE GetType  (int _iAxisNo , EN_PSTN_VALUE _iPstnNo);
        double        SetPos   (int _iAxisNo , EN_PSTN_VALUE _iPstnNo , double _dValue );

        void SetWindow(TPanel * _pnBase,int _iAxisNo);
        void SetParent(TPanel * _pnBase,int _iAxisNo);

        void Load(AnsiString sJobName);
        void Save(AnsiString sJobName);

        void UpdatePstn(bool _bToTable) ;


};
extern CPstnMan PM;
#endif