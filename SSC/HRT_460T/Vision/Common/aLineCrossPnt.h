//---------------------------------------------------------------------------
#ifndef aLineCrossPntH
#define aLineCrossPntH
//---------------------------------------------------------------------------
#include "aLine.h"

//


//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------
class CLineCrossPnt {
    public :
        struct TPara {
            TPara(){
            }

            CLine::TPara LinPara1 ;
            CLine::TPara LinPara2 ;
        } ;
        struct TRslt{
            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }

            void Clear(){

            }


            double   dInspTime    ; //검사시간.
            String   sErrMsg      ;
        } ;
    private :
    protected :
        CLine Line1 , Line2 ;
        CLine::TRslt LinRslt1 ;
        CLine::TRslt LinRslt2 ;

    public :
        //Functions
        CLineCrossPnt();
        ~CLineCrossPnt();

    public  :
        bool Inspect  (CImage * _pImg  , TRect _tRect1 , TRect _tRect2 , TPara _tPara , TRslt * _pRslt);
        //void PaintRslt(HDC _hHdc , TRslt * _pRslt , LCP_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);
};
//---------------------------------------------------------------------------
#endif