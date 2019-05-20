//---------------------------------------------------------------------------
#ifndef TrackerH
#define TrackerH
#include <Graphics.hpp>
#include <Controls.hpp>
#include "SVInterface.h"
//---------------------------------------------------------------------------
//CLASS TTracker
//SUN Kye Won 2011.10.24
//------------------------------------------------------------------------------

//using namespace Graphics ;

enum TTrackerType { ttNone = 0 ,ttRect  , ttLine , ttCircle , ttPoly , ttPolyLine , //완성.
                    ttRoundRect , ttElipse ,                                        //추후 작업.
                    MAX_SHAPE_TYPE};

//typedef SLinkedList<TPoint> TPointList ;
typedef void (__closure *TOnChanged )();

//트렉커 클래스.
class TTracker {
    private :


    public : //생성자 소멸자.
        TTracker(){
            m_iTrackerType = ttNone;
            FOnChanged     = NULL  ;

            FVisible       = true  ;
            FEnabled       = true  ;
            FFocused       = false ;

            FLineColor     = clBlue ;
            FPointColor    = clLime ;
            FLineWidth     = 1      ;
            FPointSize     = 5      ;
            FCaption       = "Tracker" ;

            FOffsetX       = 0 ;
            FOffsetY       = 0 ;

        }
        //TTracker(TOnChanged _OnChanged);
        virtual ~TTracker(){};

    protected : //Var.
        TTrackerType   m_iTrackerType ;

        int m_iStdRefX ; //참조 기준값 티칭 할때 넣어줌.
        int m_iStdRefY ;
        //float m_fStdRefT ; //세타는 나중에 하자 .... 

        TOnChanged FOnChanged  ;

        bool       FVisible    ; void __fastcall SetVisible    (bool       value);   //보이지 않음.
        bool       FEnabled    ; void __fastcall SetEnabled    (bool       value);   //클릭이 안되고 실선으로 보임.
        bool       FFocused    ; void __fastcall SetFocused    (bool       value);   //포커스 먹고 있음.

        TColor     FLineColor  ; void __fastcall SetLineColor  (TColor     value);
        TColor     FPointColor ; void __fastcall SetPointColor (TColor     value);
        int        FLineWidth  ; void __fastcall SetLineWidth  (int        value);
        int        FPointSize  ; void __fastcall SetPointSize  (int        value);
        AnsiString FCaption    ; void __fastcall SetCaption    (AnsiString value);

        int        FOffsetX    ; void __fastcall SetOffsetX    (int        value);
        int        FOffsetY    ; void __fastcall SetOffsetY    (int        value);

    public :


        //Property
        __property bool       Visible    = { read = FVisible    , write = SetVisible    };
        __property bool       Enabled    = { read = FEnabled    , write = SetEnabled    };
        __property bool       Focused    = { read = FFocused    , write = SetFocused    };

        __property TColor     LineColor  = { read = FLineColor  , write = SetLineColor  };
        __property TColor     PointColor = { read = FPointColor , write = SetPointColor };
        __property int        LineWidth  = { read = FLineWidth  , write = SetLineWidth  };
        __property int        PointSize  = { read = FPointSize  , write = SetPointSize  };
        __property AnsiString Caption    = { read = FCaption    , write = SetCaption    };

        __property int        OffsetX    = { read = FOffsetX    , write = SetOffsetX    };
        __property int        OffsetY    = { read = FOffsetY    , write = SetOffsetY    };

        TTrackerType __fastcall GetTrackerType(){ return m_iTrackerType ;}

        void SetStdRef (int _iStdRefX , int _iStdRefY )  //결과값을 참조하여 트렉커의 위치를 자동으로 바꿔주려함.
        {
            m_iStdRefX = _iStdRefX ;
            m_iStdRefY = _iStdRefY ;
        }

        //부모 클래스에서는 본체가 없음.
        //원래 TTracker * 로 하위 클레스 제어 하려 했지만 파생이 많아서 배보다 배꼽이 더커서 포기.
        //======================================================================
        //event link
        virtual void __fastcall Paint    (HDC _Dc , float _fScaleX , float _fScaleY)=0;
        virtual bool __fastcall TrackerDw(TMouseButton Button, TShiftState Shift , int x, int y , float _fScaleX, float _fScaleY)=0;
        virtual bool __fastcall TrackerMv(                     TShiftState Shift , int x, int y , float _fScaleX, float _fScaleY)=0;
        virtual void __fastcall TrackerUp(TMouseButton Button, TShiftState Shift  )=0;
        virtual void SetRefOfs(int _iRefX , int _iRefY)=0;

        //Load Save.
        void OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption);
        //======================================================================

        //Set.
        void __fastcall SetFunc(TOnChanged ChangeFunc){
            FOnChanged = ChangeFunc ;
        };

};

//Rect 트렉커 클래스.
class TRectTracker:public TTracker {

    public : //생성자 소멸자.
        TRectTracker();
        virtual ~TRectTracker();

    private : //Var.
        struct TStat {
            bool IsLTM  , IsCTM , IsRTM ;
            bool IsRCM  , IsMove, IsLCM ;
            bool IsLBM  , IsCBM , IsRBM ;
            bool IsBND  ;
            int  mx, my ;
        }Stat ;
        void __fastcall UpdateRect();

    protected :
        int  FLeft        ; void __fastcall SetLeft        (int  value);
        int  FWidth       ; void __fastcall SetWidth       (int  value);
        int  FTop         ; void __fastcall SetTop         (int  value);
        int  FHeight      ; void __fastcall SetHeight      (int  value);

        //Band
        bool FBandVisible ; void __fastcall SetBandVisible (bool value);
        int  FBand        ; void __fastcall SetBand        (int  value);
        int  FLeftOffset  ; void __fastcall SetLeftOffset  (int  value);
        int  FTopOffset   ; void __fastcall SetTopOffset   (int  value);
        int  FRightOffset ; void __fastcall SetRightOffset (int  value);
        int  FBottomOffset; void __fastcall SetBottomOffset(int  value);

    public :

        //Property
        __property int  Left         = { read = FLeft          , write = SetLeft         };
        __property int  Top          = { read = FTop           , write = SetTop          };
        __property int  Height       = { read = FHeight        , write = SetHeight       };
        __property int  Width        = { read = FWidth         , write = SetWidth        };

        __property bool BandVisible  = { read = FBandVisible   , write = SetBandVisible  };
        __property int  Band         = { read = FBand          , write = SetBand         };
        __property int  LeftOffset   = { read = FLeftOffset    , write = SetLeftOffset   };
        __property int  TopOffset    = { read = FTopOffset     , write = SetTopOffset    };
        __property int  RightOffset  = { read = FRightOffset   , write = SetRightOffset  };
        __property int  BottomOffset = { read = FBottomOffset  , write = SetBottomOffset };

        //Interface
        int __fastcall GetRight  ();
        int __fastcall GetBottom ();
        int __fastcall GetCenterX();
        int __fastcall GetCenterY();

        TRect __fastcall GetLTRect();
        TRect __fastcall GetTTRect();
        TRect __fastcall GetRTRect();
        TRect __fastcall GetBTRect();

        TPoint __fastcall GetLTPnt();
        TPoint __fastcall GetRTPnt();
        TPoint __fastcall GetLBPnt();
        TPoint __fastcall GetRBPnt();

        TRect __fastcall GetTRect(){TRect Rect ; Rect.left = FLeft ; Rect.top = FTop ; Rect.right = GetRight(); Rect.bottom = GetBottom(); return Rect ;}

        void __fastcall SetArea(int _iLeft , int _iTop , int _iWidth , int _iHeight);

        //event link
        virtual void __fastcall Paint (HDC _Dc , float _fScaleX = 1.0, float _fScaleY = 1.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift  );
        virtual void SetRefOfs(int _iRefX , int _iRefY);

        //Load Save.
        void OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption);

};

//Rect 트렉커 클래스.
class TLineTracker:public TTracker {

    public : //생성자 소멸자.
        TLineTracker();
        virtual ~TLineTracker();

    private : //Var.
        int FStartX ; void __fastcall SetStartX (int value);
        int FStartY ; void __fastcall SetStartY (int value);
        int FEndX   ; void __fastcall SetEndX   (int value);
        int FEndY   ; void __fastcall SetEndY   (int value);

        struct TStat {
            bool IsStt  , IsMove, IsEnd ;
            int  mx, my ;
        }Stat ;

    protected :
    public :

        //Property
        __property int StartX = { read = FStartX , write = SetStartX };
        __property int StartY = { read = FStartY , write = SetStartY };
        __property int EndX   = { read = FEndX   , write = SetEndX   };
        __property int EndY   = { read = FEndY   , write = SetEndY   };

        //Interface
        TPoint GetStartPnt() { TPoint Pnt(FStartX , FStartY) ; return Pnt ;}
        TPoint GetEndPnt  () { TPoint Pnt(FEndX   , FEndY  ) ; return Pnt ;}

        //event link
        virtual void __fastcall Paint    (HDC _Dc , float _fScaleX = 1.0, float _fScaleY = 1.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift );

        virtual void SetRefOfs(int _iRefX , int _iRefY);

        //Load Save.
        void OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption);

};

//Circle 트렉커 클래스.
class TCircleTracker:public TTracker {

    public : //생성자 소멸자.
        TCircleTracker();
        virtual ~TCircleTracker();

    private : //Var.
        struct TStat {
            bool          IsTP          ;
            bool IsLT   , IsMove , IsRT ;
            bool          IsBT          ;
            int  mx, my ;
        }Stat ;

    protected :
        int FX      ; void __fastcall SetX      (int value);
        int FY      ; void __fastcall SetY      (int value);
        int FRadius ; void __fastcall SetRadius (int value);
        int FBand   ; void __fastcall SetBand   (int value);

    public :

        //Property
        __property int X      = { read = FX      , write = SetX      };
        __property int Y      = { read = FY      , write = SetY      };
        __property int Radius = { read = FRadius , write = SetRadius };
        __property int Band   = { read = FBand   , write = SetBand   };

        //Interface
        int     __fastcall GetLeft      ();
        int     __fastcall GetTop       ();
        int     __fastcall GetRight     ();
        int     __fastcall GetBottom    ();
        TCircle __fastcall GetTCircle   (); TFCircle __fastcall GetTFCircle   ();

        //event link
        virtual void __fastcall Paint (HDC _Dc , float _fScaleX = 1.0, float _fScaleY = 1.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift  );

        virtual void SetRefOfs(int _iRefX , int _iRefY);

        //Load Save.
        void OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption);

};

//Poly 트렉커 클래스.
class TPolyTracker:public TRectTracker {

    public : //생성자 소멸자.
        TPolyTracker();
        virtual ~TPolyTracker();

    private : //Var.
        struct TStat {
            //외부 점들.
            bool IsLTM  , IsCTM , IsRTM ;
            bool IsRCM  , IsMove, IsLCM ;
            bool IsLBM  , IsCBM , IsRBM ;

            //내부 점 클릭된 포인트번호.
            int  iClickNo  ;

            int    mx, my ;
        }Stat ;

    protected :
        TPoint * FPoints ;
        int      FPointCount ;

    public :

        //Property
        //Interface
        bool InsertPoint  (int _iNo) ;
        bool DeletePoint  (int _iNo) ;
        int  GetPointX    (int _iNo) ;
        int  GetPointY    (int _iNo) ;
        int  GetPointCount(        ) ;

        //event link
        virtual void __fastcall Paint (HDC _Dc , float _fScaleX = 1.0, float _fScaleY = 1.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift  );

        virtual void SetRefOfs(int _iRefX , int _iRefY);

        //Load Save.
        void OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption);

};

//Poly Line 트렉커 클래스.
class TPolyLineTracker:public TRectTracker {

    public : //생성자 소멸자.
        TPolyLineTracker();
        virtual ~TPolyLineTracker();

    private : //Var.
        struct TStat {
            //외부 점들.
            bool IsLTM  , IsCTM , IsRTM ;
            bool IsRCM  , IsMove, IsLCM ;
            bool IsLBM  , IsCBM , IsRBM ;

            //내부 점 클릭된 포인트번호.
            int  iClickNo  ;

            int    mx, my ;
        }Stat ;

    protected :
        TPoint * FPoints     ;
        int      FPointCount ;
        //int      FBandWidth  ; 나중에 추가.

    public :

        //Property
        //Interface
        bool InsertPoint  (int _iNo) ;
        bool DeletePoint  (int _iNo) ;
        int  GetPointX    (int _iNo) ;
        int  GetPointY    (int _iNo) ;
        int  GetPointCount(        ) ;

        //event link
        virtual void __fastcall Paint (HDC _Dc , float _fScaleX = 1.0, float _fScaleY = 1.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift  );

        virtual void SetRefOfs(int _iRefX , int _iRefY);

        //Load Save.
        void OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption);

};


#endif

