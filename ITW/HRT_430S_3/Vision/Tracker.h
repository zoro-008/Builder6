//---------------------------------------------------------------------------
#ifndef TrackerH
#define TrackerH
#include <Graphics.hpp>
#include <Controls.hpp>
#include "BaseDefine.h"





//---------------------------------------------------------------------------
//CLASS TTracker
//SUN Kye Won 2011.10.24
//------------------------------------------------------------------------------

//using namespace Graphics ;

enum TTrackerType { ttNone = 0 ,ttRect ,ttOutBandRect , ttLine , ttCircle , ttPoly , ttPolyLine , //완성.
                    ttRoundRect , ttElipse ,                                        //추후 작업.
                    MAX_SHAPE_TYPE};

//typedef SLinkedList<TPoint> TPointList ;
typedef void (__closure *TOnChanged )();


/* 해야할것듣.
나중에 TRectTracker에서 밴드 빼고 TBandRectTracker만들고 TRectTracker 상속베이스로 만들기..
OutBandTracker , LineTracker , RectTracker 빼고 다 Paint와 TrackerDw , TrackerMove 확인 하기 스케일 오프셑 확인 해야함.



*/




//트렉커 클래스.
class TTracker {
    private :


    public : //생성자 소멸자.
        TTracker();
        virtual ~TTracker();

    protected : //Var.
        TTrackerType   m_iTrackerType ;

        bool m_bMoved  ; //움직이고 아직 세이브 안됨.

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

        TTrackerType __fastcall GetTrackerType(){ return m_iTrackerType ;}

        bool GetMoved (){
            return m_bMoved ;
        }

        //부모 클래스에서는 본체가 없음.
        //원래 TTracker * 로 하위 클레스 제어 하려 했지만 파생이 많아서 배보다 배꼽이 더커서 포기.
        //======================================================================
        //event link
        virtual void __fastcall Paint    (HDC _Dc , float _fScaleX , float _fScaleY , float _fOffsetX , float _fOffsetY)=0;
        virtual bool __fastcall TrackerDw(TMouseButton Button, TShiftState Shift , int x, int y , float _fScaleX, float _fScaleY , float _fOffsetX , float _fOffsetY)=0;
        virtual bool __fastcall TrackerMv(                     TShiftState Shift , int x, int y , float _fScaleX, float _fScaleY , float _fOffsetX , float _fOffsetY)=0;
        virtual void __fastcall TrackerUp(TMouseButton Button, TShiftState Shift  )=0;

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
            float  mx, my ;
        }Stat ;
        void __fastcall UpdateRect();

    protected :
        float FLeft        ; void __fastcall SetLeft        (float value);
        float FWidth       ; void __fastcall SetWidth       (float value);
        float FTop         ; void __fastcall SetTop         (float value);
        float FHeight      ; void __fastcall SetHeight      (float value);

        //Band
        bool  FBandVisible ; void __fastcall SetBandVisible (bool  value);
        float FBand        ; void __fastcall SetBand        (float value);
        float FBandMax     ; void __fastcall SetBandMax     (float value);
        bool  FBandEnable  ; void __fastcall SetBandEnable  (bool  value);
        float FLeftOffset  ; void __fastcall SetLeftOffset  (float value);
        float FTopOffset   ; void __fastcall SetTopOffset   (float value);
        float FRightOffset ; void __fastcall SetRightOffset (float value);
        float FBottomOffset; void __fastcall SetBottomOffset(float value);
        bool  FSizeEnable  ; void __fastcall SetSizeEnable  (bool  value);

    public :

        //Property
        __property float Left         = { read = FLeft          , write = SetLeft         };
        __property float Top          = { read = FTop           , write = SetTop          };
        __property float Height       = { read = FHeight        , write = SetHeight       };
        __property float Width        = { read = FWidth         , write = SetWidth        };

        __property bool  BandVisible  = { read = FBandVisible   , write = SetBandVisible  };
        __property float Band         = { read = FBand          , write = SetBand         };
        __property float BandMax      = { read = FBandMax       , write = SetBandMax      };
        __property bool  BandEnable   = { read = FBandEnable    , write = SetBandEnable   };
        __property float LeftOffset   = { read = FLeftOffset    , write = SetLeftOffset   };
        __property float TopOffset    = { read = FTopOffset     , write = SetTopOffset    };
        __property float RightOffset  = { read = FRightOffset   , write = SetRightOffset  };
        __property float BottomOffset = { read = FBottomOffset  , write = SetBottomOffset };
        __property bool  SizeEnable   = { read = FSizeEnable    , write = SetSizeEnable   };

        //Interface
        float __fastcall GetRight  ();
        float __fastcall GetBottom ();
        float __fastcall GetCenterX();
        float __fastcall GetCenterY();

        TFRect __fastcall GetLTFRect(); TRect __fastcall GetLTRect();
        TFRect __fastcall GetTTFRect(); TRect __fastcall GetTTRect();
        TFRect __fastcall GetRTFRect(); TRect __fastcall GetRTRect();
        TFRect __fastcall GetBTFRect(); TRect __fastcall GetBTRect();

        TFPoint __fastcall GetLTPnt();
        TFPoint __fastcall GetRTPnt();
        TFPoint __fastcall GetLBPnt();
        TFPoint __fastcall GetRBPnt();

        TFRect __fastcall GetTFRect(){TFRect Rect ; Rect.left = FLeft ; Rect.top = FTop ; Rect.right = GetRight(); Rect.bottom = GetBottom(); return Rect ;}
        TRect  __fastcall GetTRect (){TRect Rect ; Rect.left = FLeft ; Rect.top = FTop ; Rect.right = GetRight(); Rect.bottom = GetBottom(); return Rect ;}

        void    __fastcall SetArea(int _iLeft , int _iTop , int _iWidth , int _iHeight);

        //event link
        virtual void __fastcall Paint    (HDC _Dc , float _fScaleX=0.0 , float _fScaleY=0.0 , float _fOffsetX=0.0 , float _fOffsetY=0.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0 , float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0 , float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift  );

        //Load Save.
        void OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption);

};

//OutBandTracker 나중에 TRectTracker에서 밴드 빼고 TBandRectTracker만들고 TRectTracker 상속베이스로 만들기..
class TOutBandRectTracker:public TTracker {

    public : //생성자 소멸자.
        TOutBandRectTracker();
        virtual ~TOutBandRectTracker();

    private : //Var.
        struct TStat {
            bool IsLTM  , IsCTM , IsRTM ;
            bool IsRCM  , IsMove, IsLCM ;
            bool IsLBM  , IsCBM , IsRBM ;
            bool IsBND  ;
            float  mx, my ;
        }Stat ;
        void __fastcall UpdateRect();

    protected :
        float FLeft        ; void __fastcall SetLeft        (float value);
        float FWidth       ; void __fastcall SetWidth       (float value);
        float FTop         ; void __fastcall SetTop         (float value);
        float FHeight      ; void __fastcall SetHeight      (float value);
        float FWidthMax    ; void __fastcall SetWidthMax    (float value);
        float FHeightMax   ; void __fastcall SetHeightMax   (float value);


        //Band
        float FBand        ; void __fastcall SetBand        (float value);
        bool   FBandEnable  ; void __fastcall SetBandEnable  (bool   value);
        float FBandMax     ; void __fastcall SetBandMax     (float value);

    public :

        //Property
        __property float Left         = { read = FLeft       , write = SetLeft      };
        __property float Top          = { read = FTop        , write = SetTop       };
        __property float Height       = { read = FHeight     , write = SetHeight    };
        __property float Width        = { read = FWidth      , write = SetWidth     };
        __property float HeightMax    = { read = FHeightMax  , write = SetHeightMax };
        __property float WidthMax     = { read = FWidthMax   , write = SetWidthMax  };


        __property float Band           = { read = FBand       , write = SetBand      };
        __property bool   BandEnable     = { read = FBandEnable , write = SetBandEnable};
        __property float BandMax        = { read = FBandMax    , write = SetBandMax   };


        //Interface
        float  __fastcall GetRight  ();
        float  __fastcall GetBottom ();
        float  __fastcall GetCenterX();
        float  __fastcall GetCenterY();

        TFRect  __fastcall GetOutFRect();
        TRect   __fastcall GetOutRect ();

        TFPoint __fastcall GetLTPnt  ();
        TFPoint __fastcall GetRTPnt  ();
        TFPoint __fastcall GetLBPnt  ();
        TFPoint __fastcall GetRBPnt  ();

        TFRect  __fastcall GetTFRect(){TFRect Rect ; Rect.left = FLeft ; Rect.top = FTop ; Rect.right = GetRight(); Rect.bottom = GetBottom(); return Rect ;}
        TRect   __fastcall GetTRect (){TRect  Rect ; Rect.left = FLeft ; Rect.top = FTop ; Rect.right = GetRight(); Rect.bottom = GetBottom(); return Rect ;}

        void    __fastcall SetArea(int _iLeft , int _iTop , int _iWidth , int _iHeight);

        //event link
        virtual void __fastcall Paint    (HDC _Dc , float _fScaleX=0.0 , float _fScaleY=0.0 , float _fOffsetX=0.0 , float _fOffsetY=0.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift  );

        //Load Save.
        void OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption);

};

//Rect 트렉커 클래스.
class TLineTracker:public TTracker {

    public : //생성자 소멸자.
        TLineTracker();
        virtual ~TLineTracker();

    private : //Var.
        float FStartX ; void __fastcall SetStartX (float value);
        float FStartY ; void __fastcall SetStartY (float value);
        float FEndX   ; void __fastcall SetEndX   (float value);
        float FEndY   ; void __fastcall SetEndY   (float value);

        struct TStat {
            bool IsStt  , IsMove, IsEnd ;
            float  mx, my ;
        }Stat ;

    protected :
    public :

        //Property
        __property float StartX = { read = FStartX , write = SetStartX };
        __property float StartY = { read = FStartY , write = SetStartY };
        __property float EndX   = { read = FEndX   , write = SetEndX   };
        __property float EndY   = { read = FEndY   , write = SetEndY   };

        //Interface
        TFPoint GetStartPnt() { TFPoint Pnt(FStartX , FStartY) ; return Pnt ;}
        TFPoint GetEndPnt  () { TFPoint Pnt(FEndX   , FEndY  ) ; return Pnt ;}

        //event link
        virtual void __fastcall Paint    (HDC _Dc , float _fScaleX=0.0 , float _fScaleY=0.0 , float _fOffsetX=0.0 , float _fOffsetY=0.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift );


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
            float mx, my ;
        }Stat ;

    protected :
        float FX      ; void __fastcall SetX      (float value);
        float FY      ; void __fastcall SetY      (float value);
        float FRadius ; void __fastcall SetRadius (float value);
        float FBand   ; void __fastcall SetBand   (float value);

    public :

        //Property
        __property float X      = { read = FX      , write = SetX      };
        __property float Y      = { read = FY      , write = SetY      };
        __property float Radius = { read = FRadius , write = SetRadius };
        __property float Band   = { read = FBand   , write = SetBand   };

        //Interface
        float   __fastcall GetLeft    ();
        float   __fastcall GetTop     ();
        float   __fastcall GetRight   ();
        float   __fastcall GetBottom  ();
        TCircle  __fastcall GetTCircle ();
        TFCircle __fastcall GetTFCircle();

        //event link
        virtual void __fastcall Paint    (HDC _Dc , float _fScaleX=0.0 , float _fScaleY=0.0 , float _fOffsetX=0.0 , float _fOffsetY=0.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift );


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

            float mx, my ;
        }Stat ;

    protected :
        TFPoint * FPoints ;
        int       FPointCount ;

    public :

        //Property
        //Interface
        bool   InsertPoint  (int _iNo) ;
        bool   DeletePoint  (int _iNo) ;
        float  GetPointX    (int _iNo) ;
        float  GetPointY    (int _iNo) ;
        float  GetPointCount(        ) ;

        //event link
        virtual void __fastcall Paint    (HDC _Dc , float _fScaleX=0.0 , float _fScaleY=0.0 , float _fOffsetX=0.0 , float _fOffsetY=0.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift  );


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

            float mx, my ;
        }Stat ;

    protected :
        TFPoint * FPoints     ;
        int       FPointCount ;
        //int      FBandWidth  ; 나중에 추가.

    public :

        //Property
        //Interface
        bool  InsertPoint  (int _iNo) ;
        bool  DeletePoint  (int _iNo) ;
        float GetPointX    (int _iNo) ;
        float GetPointY    (int _iNo) ;
        float GetPointCount(        ) ;

        //event link
        virtual void __fastcall Paint    (HDC _Dc , float _fScaleX=0.0 , float _fScaleY=0.0 , float _fOffsetX=0.0 , float _fOffsetY=0.0);

        virtual bool __fastcall TrackerDw(TMouseButton Button,TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual bool __fastcall TrackerMv(                    TShiftState Shift ,int x, int y , float _fScaleX = 1.0, float _fScaleY = 1.0 , float _fOffsetX = 0.0, float _fOffsetY= 0.0);
        virtual void __fastcall TrackerUp(TMouseButton Button,TShiftState Shift  );


        //Load Save.
        void OnLoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption);

};


#endif

