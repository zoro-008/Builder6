//---------------------------------------------------------------------------

#ifndef PstnManH
#define PstnManH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <ExtCtrls.hpp>

#include "UtilDefine.h"
#include "TEdit.h"

#define MAX_PSTN 12

const int PSTN_CNT[MAX_MOTR] = {
    MAX_PSTN_MOTR0  ,
};

enum EN_VALUE_TYPE {
    vtPosition = 0 ,
    vtOffset       ,
    vtFixValue     ,

    MAX_VALUE_TYPE
};

//������ �ƴѵ�....�ؼ�...
typedef bool (__closure*CheckSafeCallbackPM)(EN_MOTR_ID _iAxisNo , EN_PSTN_VALUE _iPstnValId);
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

        CheckSafeCallbackPM CheckSafe[MAX_MOTR];

        void LoadLastInfo() ;
        void SaveLastInfo() ;

    public :
        void Init();
        void Close();

        double        GetValue (EN_MOTR_ID _iAxisNo , EN_PSTN_VALUE _iPstnNo);
        EN_VALUE_TYPE GetType  (EN_MOTR_ID _iAxisNo , EN_PSTN_VALUE _iPstnNo);
        bool          SetType  (EN_MOTR_ID _iAxisNo , EN_PSTN_VALUE _iPstnNo , EN_VALUE_TYPE _iType);
        double        SetPos   (EN_MOTR_ID _iAxisNo , EN_PSTN_VALUE _iPstnNo , double _dValue );

        void SetCheckSafe(EN_MOTR_ID _iAxisNo , CheckSafeCallbackPM _pCallback);

        void SetWindow(TPanel * _pnBase,EN_MOTR_ID _iAxisNo);
        void SetParent(TPanel * _pnBase,EN_MOTR_ID _iAxisNo);

        void Load(AnsiString sJobName);
        void Save(AnsiString sJobName);

        void UpdatePstn(bool _bToTable) ;


};
extern CPstnMan PM;
#endif
