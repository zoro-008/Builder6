//---------------------------------------------------------------------------

#ifndef PstnManH
#define PstnManH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <ExtCtrls.hpp>

#include "UtilDefine.h"
#include "TEdit.h"

#define MAX_PSTN 15

const int PSTN_CNT[MAX_MOTR] = {
    MAX_PSTN_MOTR0   ,
    MAX_PSTN_MOTR1   ,
    MAX_PSTN_MOTR2   ,
    MAX_PSTN_MOTR3   ,
    MAX_PSTN_MOTR4   ,
    MAX_PSTN_MOTR5   ,
    MAX_PSTN_MOTR6   ,
    MAX_PSTN_MOTR7   ,
    MAX_PSTN_MOTR8   ,
    MAX_PSTN_MOTR9   ,
    MAX_PSTN_MOTR10  ,
    MAX_PSTN_MOTR11  ,
    MAX_PSTN_MOTR12  ,
    MAX_PSTN_MOTR13  ,
    MAX_PSTN_MOTR14  ,
    MAX_PSTN_MOTR15  ,
    MAX_PSTN_MOTR16  ,
    MAX_PSTN_MOTR17  ,
    MAX_PSTN_MOTR18  ,
    MAX_PSTN_MOTR19  ,
    MAX_PSTN_MOTR20  ,
    MAX_PSTN_MOTR21  ,
    MAX_PSTN_MOTR22  ,
    MAX_PSTN_MOTR23

};

enum EN_VALUE_TYPE {
    vtPosition = 0 ,
    vtOffset       ,

    MAX_VALUE_TYPE
};

//아직은 아닌듯....해서...
//typedef bool (__closure*CheckSafeCallback)(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId);


class CPstnMan
{
    public :
        CPstnMan();
        ~CPstnMan();

    protected :
        struct SValue {
            AnsiString    sName  ;
            EN_VALUE_TYPE iType  ;
            bool          bFix   ;
            bool          bCommon; //전디바이스 공용.
            double        dValue ;
            SValue(){
                sName  ="" ;
                iType  =vtPosition;
                bFix   =false ;
                bCommon=false ;
                dValue =0.0   ;
            }
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

        //int m_iMaxMotor ;  나중에 CMotors에서 받아온다.
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

    public :
        void Init();
        void Close();

        double        GetValue (int _iAxisNo , EN_PSTN_VALUE _iPstnNo);
        EN_VALUE_TYPE GetType  (int _iAxisNo , EN_PSTN_VALUE _iPstnNo);
        bool          SetType  (int _iAxisNo , EN_PSTN_VALUE _iPstnNo , EN_VALUE_TYPE _iType);
        double        SetValue (int _iAxisNo , EN_PSTN_VALUE _iPstnNo , double _dValue );

        void SetWindow(TPanel * _pnBase,int _iAxisNo);
        void SetParent(TPanel * _pnBase,int _iAxisNo);

        void Load(AnsiString sJobName);
        void Save(AnsiString sJobName);

        void UpdatePstn(bool _bToTable) ;


};
extern CPstnMan PM;
#endif
