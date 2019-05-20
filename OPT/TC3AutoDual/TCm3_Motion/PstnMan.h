//---------------------------------------------------------------------------

#ifndef PstnManH
#define PstnManH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <ExtCtrls.hpp>

#include "UtilEnums.h"
#include "TEdit.h"

#define MAX_PSTN 10
//Totabl Table
enum EN_PSTN_Z {
    pi_Z_Wait = 0 ,
    pi_Z_Insp     ,
    pi_Z_Disp
};

enum EN_PSTN_Y {
    pi_Y_Wait = 0 ,
    pi_Y_Insp     ,
    pi_Y_Disp
};

enum EN_PSTN_I {
    pi_I_Wait = 0 ,
    pi_I_Insp = 1 ,
};


//Right Table.
enum EN_PSTN_LT_Z {
    piLT_Z_Wait = 0 ,
    piLT_Z_Insp     ,
    piLT_Z_Disp     ,

    MAX_PSTN_MOTR1
};

enum EN_PSTN_LT_Y {
    piLT_Y_Wait = 0 ,
    piLT_Y_Insp     ,
    piLT_Y_Disp     ,

    MAX_PSTN_MOTR2
};

enum EN_PSTN_LT_I {
    piLT_I_Wait = 0 ,
    piLT_I_Insp     ,

    MAX_PSTN_MOTR3
};

//Right Table.
enum EN_PSTN_RT_Z {
    piRT_Z_Wait = 0 ,
    piRT_Z_Insp     ,
    piRT_Z_Disp     ,

    MAX_PSTN_MOTR4
};

enum EN_PSTN_RT_Y {
    piRT_Y_Wait = 0 ,
    piRT_Y_Insp     ,
    piRT_Y_Disp     ,

    MAX_PSTN_MOTR5
};

enum EN_PSTN_RT_I {
    piRT_I_Wait = 0 ,
    piRT_I_Insp     ,

    MAX_PSTN_MOTR6
};

const int PSTN_CNT[] = {MAX_PSTN_MOTR1 , MAX_PSTN_MOTR2 , MAX_PSTN_MOTR3 , MAX_PSTN_MOTR4 , MAX_PSTN_MOTR5 , MAX_PSTN_MOTR6 };

class CPstnMan
{
    public :
        CPstnMan();
        ~CPstnMan();

    protected :
        struct TPstn {
            AnsiString sName ;
            double     dPstn ;
        };

        struct TGridSelInfo {
            int    iGrid ;
            bool   bDown ;
            int    iCol  ;
            int    iRow  ;
            int    iX    ;
            int    iY    ;
            int    iColX ;
            int    iRowY ;
        }GridInfo;

        //int m_iMaxMotor ;  나중에 CMotors에서 받아온다.
        TPstn m_pPstn[MAX_MOTR][MAX_PSTN] ;

        //Grid & Event.
        TStringGrid * m_pPstnList[MAX_MOTR];
        void __fastcall pPstnListDrawCell (TObject *Sender, int ACol, int ARow , const TRect &Rect, TGridDrawState State) ;
        void __fastcall pPstnListMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall pPstnListMouseUp  (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

        //Edit & Event.
        TEdit  * m_pPstnEdit[MAX_MOTR] ;
        void __fastcall pPstnEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift);
        void __fastcall pPstnEditExit   (TObject *Sender);

    public :
        double GetPos (int _iAxisNo , int _iPstnNo);
        bool   CmprPos(int _iAxisNo , int _iPstnNo , double _dRange = 0.0);

        void SetWindow(TPanel * _pnBase,int _iAxisNo);

        void Load();
        void Save();

        void UpdatePstn(bool _bToTable) ;


};
extern CPstnMan PM;
#endif
