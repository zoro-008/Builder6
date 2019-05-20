//---------------------------------------------------------------------------

#ifndef CalibrationUnitH
#define CalibrationUnitH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <ComCtrls.hpp>

#include "Common.h"
#include "SVInterface.h"
#include "Tracker.h"

class CCal
{
    public :
        CCal();
        ~CCal();
        void Init  (EN_CAL_ID _iCalId , bool _bCommonUse); //_bCommonUse true:�� ������ ������ , false:�����Ϻ� ���.
        void Close ();

    protected :
        EN_CAL_ID m_iCalId ;
        bool      m_bCommonUse ; //true:�� ������ ������ , false:�����Ϻ� ���.

        CAL_Para  Para ;
        CAL_Rslt  Rslt ;
        CAL_Disp  Disp ;

        TRectTracker * Tracker ;

    public :

        void Load        (bool _bLoad ) ;
        void ApplyPara   (bool _bTable) ;

        void SetTrackerPaintCallback(TOnChanged ChangeFunc); //Ʈ��Ŀ �ݹ�.
        bool OnMouseDw              (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        bool OnMouseMv              (                     TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void OnMouseUp              (TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY);
        void PaintTracker           (HDC           _hHdc                                  , float _fScaleX , float _fScaleY);
        void ShowPara               (TWinControl * _wcBase ) ;
        bool Inspection             (IImage * _pImg , CAL_Rslt * _pRslt);
        void PaintRslt              (IImage * _pImg , HDC _hHdc);

        float GetXRes();
        float GetYRes();

};

extern CCal * Cal[MAX_CAL_ID] ;
void InitCal ();
void CloseCal();



#endif
