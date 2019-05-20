//---------------------------------------------------------------------------
#ifndef UnitTrackerH
#define UnitTrackerH
#include <Graphics.hpp>
#include <Controls.hpp>

#include <Menus.hpp>

#include "BaseDefine.h"


/*
C++ 빌더에서는 C++에는 없는 __property 라는 예약어가 있습니다. 
보통 클래스에서 멤버를 읽고 쓰는 일상적인 과정에서 
읽고 쓰는 동작을 구현할 함수를 지정할 수 있게 하기 위해 사용합니다. 
이는 단순히 가상멤버변수를 대입하거나 대입 받을때 원하는 동작이 일어나게 함으로써 
직관적인 코딩이 가능하게 하며, 값의 오대입을 막거나 원하는 부가 처리를 하기 위한 용도로 사용합니다. 
특히 외부에 내부를 공개하지 않는 COM 플밍에 있어서는 거의 필수처럼 사용합니다. 
C++언어 스펙중에 좀 아쉬운 부분이 바로 get set 을 정의할 수 있는 프로퍼티를 표준으로 설정하지 않았다는 점입니다. 

하지만 C++빌더에서는 __property라는 예약어를 추가함으로써 이와 같은 문제를 해결했으며, 
디자인 타임에 프로퍼티 변수를 노출 했을때 안전한 세팅을 가능케 합니다. 

하지만 VC++에서도 VC++.NET으로 버전업 되면서 property 를 지원하기 시작했으며,
이 property는 C++빌더에서도 역시 지원하므로 마찬가지로 사용할 수 있습니다. 
property 는 디자인 타임용이 아닌 COM 플밍에서와 같이 코딩 레벨에서 사용하기 위한 것입니다. 

아래는 C++빌더, VC++ 양쪽에서 동작하는 프로퍼티의 예입니다. 
중요한 점은 컴포넌트 만들때 디자인 타임에 프로퍼티 창에 나타나는 용도가 아닌 
일반 클래스의 설계시에 이용할 수 있다는 것입니다. 

물론, 빌더의 __property도 public: 영역지정자에 두는 식으로 일반 클래스 설계에도 이용할수 있지만 
C++빌더, VC++ 양쪽에서 모두 사용 가능하다는 점이 틀리죠. 

class Cproperty 
{ 
private: 
    int m_ID; 
public: 
    __property int ID2 = {read = GetID, write = PutID }; // 볼랜드만의 __property 경우. VC++에서는 삭제필요. 
    __declspec(property(get=GetID, put=PutID)) int ID;  // 가상멤버 프로퍼티변수, VC++에서도 동작함. 

    int GetID() 
    { 
        Form1->Memo1->Lines->Add("GetID"); 
        return m_ID; 
    } 
    void PutID(int ID) 
    { 
        Form1->Memo1->Lines->Add("PutID"); 
        m_ID = ID; 
    } 
}; 

실행해서 적당한 값을 넣거나 가져올때 메모장에 GetID 와 PutID 라는 
메시지가 찍히는 것을 볼수 있습니다. 
__property와 거의 비슷하죠. 

생소한 사항이라 참고적으로 올립니다.
*/

/*
1. Overlay 영상 위에 그리실때는 Wincontrol 형태로 작업하는게 부적합합니다. 
    영상의 Render를 수행할 함수 안에서 Overlay Surface 위에 직접 그려주는 형태로 구현하면 되겠습니다만, 
    일반적인 메시징을 타시기 힘드실테고, Draw쪽과 Mouse Event등 이것 저것을 직접 다시 구현해야 될겁니다. 
    만약 카메라로 부터 얻어온 영상을 Window 위에 직접 그리시는 경우엔 상관이없겠지요. 

2. 위의 문제와 상관없이, 획득한 영상을 직접 Window Control위에 그리신다면, 
    배경 스크린 윈도우 위에 테두리나 내용을 반투명 처리하여 그려야 할 필요가 있을터이므로 
    TGraphicControl 정도로 무난합니다. 

3. 이 경우 트래커의 그립 부분과 그 외의 부분의 마우스 클릭을 판단하는 효과적인 방법은 
    원의 경우, 원의 중심으로 부터 마우스 커서가 반지름 보다 가까운지 먼지 판단하는 것이고, 
    (피타고라스의 정리) 
    혹은, 원이든 다각형이든 모두 Polygon 으로 만들어 그린 다음 폴리곤의 내부가 클릭되었는지 
    판단하는 방법이 되겠지요.

bool __fastcall isPointInPolygon(TPoint *Points, TPoint P, int N){ 
    TPoint P1, P2; 
    bool Result = false; 

    P1 = Points[0]; 
    P2 = Points[N - 1]; 

    for(int i = 1; i <= N; i++){ 
        Result ^= (((P.y >= P1.y) ^ (P.y >= P2.y)) && (P.x - P1.x < MulDiv(P2.x - P1.x, P.y - P1.y, P2.y - P1.y))); 
        P2 = P1; 
        P1 = Points[i % N]; 
    } 
    return Result; 
} 

    이 함수를 쓰시면, TPoint의 배열(Points)과, 그 갯수(N)로 넘긴 폴리곤의 내부에 마우스 좌표(P)가 
    있는지 판단할 수 있습니다. 

4. 위와 같이 구현하였는데, 트래커를 그리는 과정에서 깜박인다. (물론 깜박이겠죠) 
    그럴 경우엔 트래커를 올려 놓은 배경스크린 컨트롤의 DoubleBuffered = true; 로 설정하시면 OK.
*/



//---------------------------------------------------------------------------
//CLASS TTracker
//SUN Kye Won 2011.10.24
//------------------------------------------------------------------------------

//using namespace Graphics ;


typedef void (__closure *FOnPaint )();

//트렉커 클래스.
class CTracker {
    public :
        //컴파일 당시에 결정 하는 것이 기준으로 나눠짐....
        //런타임에 바꾸는 것은 넣지 않는다.
        enum ETrackerType {
            ttNone=0     , //로딩 하지 않고 픽스로 사용 한다.
            ttRect       , //기본적인 사각형 트렉커.
            ttLine       , //기본 적인 라인 트렉커.
            ttCircle     , //기본 원형 트레커.
            ttPolygon    , //다각형 트레커.
            ttPolyline   , //나중에 필요할때 하자..여러포인트를 가지고 있는 라인 트레커.
            MAX_TRAKCER_TYPE
        };
        enum EDownType {
            dtLT = -100 , dtCT = -101 , dtRT = -102 ,
            dtLC = -103 , dtMV = -104 , dtRC = -105  ,
            dtLB = -106 , dtCB = -107 , dtRB = -108  ,

            dtBand = -5 ,

            dtRoi  = -4 ,

            dtStt  = -3 ,
            dtEnd  = -2 ,

            dtNone = -1 ,

            //0부터는 폴리트레커 포인트 아이디.
        };
        enum EPointId {
            //Rect용       Line용          Circle용
            piRectLT = 0 , piLineStt = 0 , piCircleCnt = 0 ,
            piRectRT = 1 , piLineEnd = 1 ,
            piRectRB = 2 ,
            piRectLB = 3 ,
        };

        //생성자 소멸자.
        CTracker(){Init();}
        virtual ~CTracker(){Close();}

        void Init(){
            m_bMoved          = false   ;
            m_bInRange        = false   ;
            m_bPreInRange     = false   ;
            m_bSelected       = false   ;
            m_fpOnPaint       = NULL    ;

            m_aPoints         = NULL    ;
            m_iPointCount     = 0       ;

            FVisible          = true    ;
            FEnabled          = true    ;
            FBandEnabled      = false   ;
            FRoiEnabled       = false   ;
            FTrackerType      = ttRect  ;
            FLineColor        = clLime  ;
            FCircleRadian     = 0.0     ;
            FRoiWidth         = 20.0    ;
            FBandWidth        = 20.0    ;
            FRectPointColor   = clRed   ;
            FPointColor       = clGreen ;
            FLineWidth        = 1       ;
            FRectPointSize    = 6       ;
            FPointSize        = 6       ;
            FCaption          = ""      ;

            m_iPointCount = 4 ;
            m_aPoints = new TDPoint[m_iPointCount];
            m_aPoints[piRectLT].x = 10 ;
            m_aPoints[piRectLT].y = 10 ;

            m_aPoints[piRectRT].x = 90 ;
            m_aPoints[piRectRT].y = 10 ;

            m_aPoints[piRectRB].x = 90 ;
            m_aPoints[piRectRB].y = 90 ;

            m_aPoints[piRectLB].x = 10 ;
            m_aPoints[piRectLB].y = 90 ;






        }

        void Close(){
            if(m_aPoints) {
                delete [] m_aPoints ;
                m_aPoints = NULL ;
            }
        }

    protected : //맴버.
        bool CheckRectIn    (double _dPntX      , double _dPntY       , double _dX , double _dY , double _dPntHfSize); //사각포인트 중간크기를 넘김.
        bool CheckCircleIn  (double _dPntX      , double _dPntY       , double _dX , double _dY , double _dRad      ); //원의 반지름 넘김.
        bool CheckLineIn    (TDPoint * _aPoints , int    _iPointCount , double _dX , double _dY , double _dMargin   ); //라인 클릭으로 판단할 마진 넘김.
        void GetLineBandPnts(TDPoint _tPnt1     , TDPoint _tPnt2      , double _dMargin , TDPoint * _a4Points       ); //_aPoints 4개 포인트 배열 넘겨야함.


        struct TMoveStat {
            EDownType eDnType ; //클릭다운된 아이디.
            double    dX, dY  ; //클릭하고 이동한 양.
            EDownType eLastDnType ; //마지막으로 클릭다운된 아이디. 팝업띄우기 위해 존재.
            bool      bRight  ;    //오른클릭으로 팝업을 띄우는데 자꾸 포지션이 바뀌어서 삽입.
            TMoveStat(){
                Clear();
            }
            void Clear(){
                eDnType = dtNone ;
                dX = dY = 0.0    ;
                eLastDnType = dtNone ;
                bRight  = false  ;
            }
        }MoveStat ;
        bool      m_bSelected   ;
        bool      m_bMoved      ; //움직이고 세이브 안한상태.
        bool      m_bInRange    ; //라인 선택 범위 안에 있음.
        bool      m_bPreInRange ;

        FOnPaint  m_fpOnPaint   ;

    protected : //포인트 팝업 관련.


//저장 해야 하는 것들.==========================================================
    protected : //프로그램에서 선택되는 것들.
        TDPoint *      m_aPoints        ;
        int            m_iPointCount    ;

        int            FTag             ; void  SetTag            (int          value);   //테그로 쓴다.
        bool           FVisible         ; void  SetVisible        (bool         value);   //보이지 않음.
        bool           FEnabled         ; void  SetEnabled        (bool         value);   //클릭이 안되고 포인트들이 안보임.
        bool           FBandEnabled     ; void  SetBandEnabled    (bool         value);   //밴드 사용 여부.
        bool           FRoiEnabled      ; void  SetRoiEnabled     (bool         value);   //ROI 사용 여부.

    protected : //User UI Setting Var.
        ETrackerType   FTrackerType     ; void  SetTrackerType    (ETrackerType value);   //트렉커의 타입 선정.
        double         FCircleRadian    ; void  SetCircleRadian   (double       value);
        double         FBandWidth       ; void  SetBandWidth      (double       value);
        double         FRoiWidth        ; void  SetRoiWidth       (double       value);   //
        TColor         FLineColor       ; void  SetLineColor      (TColor       value);   //선의 색깔.
        TColor         FRectPointColor  ; void  SetRectPointColor (TColor       value);   //점의 색깔.
        TColor         FPointColor      ; void  SetPointColor     (TColor       value);   //점의 색깔.
        int            FLineWidth       ; void  SetLineWidth      (int          value);
        int            FPointSize       ; void  SetPointSize      (int          value);
        int            FRectPointSize   ; void  SetRectPointSize  (int          value);
        String         FCaption         ; void  SetCaption        (String       value);
//==============================================================================

        //유저 선택 하지 않는 것들....





    public :
        //__property int ID2 = {read = GetID, write = PutID }; // 볼랜드만의 __property 경우. VC++에서는 삭제필요.
        //__declspec(property(get=GetID, put=PutID)) int ID;  // 가상멤버 프로퍼티변수, VC++에서도 동작함.

        //Property
        __property int          Tag            = { read = FTag            , write = SetTag            };
        __property bool         Visible        = { read = FVisible        , write = SetVisible        };
        __property bool         Enabled        = { read = FEnabled        , write = SetEnabled        };
        __property bool         BandEnabled    = { read = FBandEnabled    , write = SetBandEnabled    };
        __property bool         RoiEnabled     = { read = FRoiEnabled     , write = SetRoiEnabled     };



        __property ETrackerType TrackerType    = { read = FTrackerType    , write = SetTrackerType    };

        __property double       CircleRadian   = { read = FCircleRadian   , write = SetCircleRadian   };
        __property double       BandWidth      = { read = FBandWidth      , write = SetBandWidth      };
        __property double       RoiWidth       = { read = FRoiWidth       , write = SetRoiWidth       };
        __property TColor       LineColor      = { read = FLineColor      , write = SetLineColor      };
        __property TColor       RectPointColor = { read = FRectPointColor , write = SetRectPointColor };
        __property TColor       PointColor     = { read = FPointColor     , write = SetPointColor     };
        __property int          LineWidth      = { read = FLineWidth      , write = SetLineWidth      };
        __property int          PointSize      = { read = FPointSize      , write = SetPointSize      };
        __property int          RectPointSize  = { read = FRectPointSize  , write = SetRectPointSize  };
        __property String       Caption        = { read = FCaption        , write = SetCaption        };

        TDPoint *  GetPoints   ();
        TRect      GetRect     (); void SetRect (TRect  _tRect );
        TDRect     GetDRect    (); void SetDRect(TDRect _tDRect);
        TDRect     GetRoiDRect ();
        bool       GetSelected ();
        void       SetSelected (bool _bSelected);
        bool       GetMoved    ();

        bool    InsertPoint  (int _iNo) ;
        bool    DeletePoint  (int _iNo) ;
        double  GetPointX    (int _iNo) ;
        double  GetPointY    (int _iNo) ;
        int     GetPointCount(        ) ;
        TDRect  GetBandDRect  (EPointId _piPnt1 , EPointId _piPnt2 );

        TMoveStat GetMoveStat();


        //event link
        void  SetOnPaint   (FOnPaint    function);
        bool  TrackerDw    (TMouseButton Button, TShiftState Shift , int x, int y , double _dScaleX, double _dScaleY , double _dOffsetX , double _dOffsetY);
        bool  TrackerMv    (                     TShiftState Shift , int x, int y , double _dScaleX, double _dScaleY , double _dOffsetX , double _dOffsetY);
        bool  TrackerUp    (TMouseButton Button, TShiftState Shift  );
        void  Paint        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //Load Save.
        void  LoadSave(bool _bLoad,AnsiString _sFilePath,AnsiString _sCaption);
        //======================================================================

        /*
        void operator =(const CTracker& rc)
        {
            m_iPointCount = rc.m_iPointCount ;
            if(m_aPoints) delete [] m_aPoints ;
            m_aPoints = new TDPoint[m_iPointCount] ;
            memcpy(m_aPoints , rc.m_aPoints , m_iPointCount * sizeof(TDPoint));


            FVisible        = rc.FVisible        ;
            FEnabled        = rc.FEnabled        ;
            FBandEnabled    = rc.FBandEnabled    ;
            FRoiEnabled     = rc.FRoiEnabled     ;
            FTrackerType    = rc.FTrackerType    ;
            FCircleRadian   = rc.FCircleRadian   ;
            FBandWidth      = rc.FBandWidth      ;
            FRoiWidth       = rc.FRoiWidth       ;
            FLineColor      = rc.FLineColor      ;
            FRectPointColor = rc.FRectPointColor ;
            FPointColor     = rc.FPointColor     ;
            FLineWidth      = rc.FLineWidth      ;
            FPointSize      = rc.FPointSize      ;
            FRectPointSize  = rc.FRectPointSize  ;
            FCaption        = rc.FCaption        ;
        }*/
        void Copy(CTracker * _pTracker)
        {
            m_iPointCount = _pTracker->m_iPointCount ;
            if(m_aPoints) delete [] m_aPoints ;
            m_aPoints = new TDPoint[m_iPointCount] ;
            memcpy(m_aPoints , _pTracker->m_aPoints , m_iPointCount * sizeof(TDPoint));


            FVisible        = _pTracker->FVisible        ;
            FEnabled        = _pTracker->FEnabled        ;
            FBandEnabled    = _pTracker->FBandEnabled    ;
            FRoiEnabled     = _pTracker->FRoiEnabled     ;
            FTrackerType    = _pTracker->FTrackerType    ;
            FCircleRadian   = _pTracker->FCircleRadian   ;
            FBandWidth      = _pTracker->FBandWidth      ;
            FRoiWidth       = _pTracker->FRoiWidth       ;
            FLineColor      = _pTracker->FLineColor      ;
            FRectPointColor = _pTracker->FRectPointColor ;
            FPointColor     = _pTracker->FPointColor     ;
            FLineWidth      = _pTracker->FLineWidth      ;
            FPointSize      = _pTracker->FPointSize      ;
            FRectPointSize  = _pTracker->FRectPointSize  ;
            FCaption        = _pTracker->FCaption        ;
        }

};



//class CPopupTracker;
class CPopupTracker ;
enum EMenuItem {
    miAddPoint     = 0 ,
    miDelPoint     = 1 ,
    //-----------
    miTypeRect     = 2 ,
    miTypeCircle   = 3 ,
    miTypePolygon  = 4 ,
    miSeperator    = 5 ,
    miNewInsp      = 6 ,
    miNewIgnr      = 7 ,
    miDelete       = 8 ,
    miCopyAndPaste = 9 ,
    MAX_MENU_ITEM
};
typedef void (__closure *FOnMenuItemClick)(EMenuItem , CPopupTracker *);
class CPopupTracker : public CTracker
{

    public:
        CPopupTracker(){
            Init();
        }
        virtual ~CPopupTracker(){
        }



        void SetOnMenuItemClick(FOnMenuItemClick _OnMenuItemClick){
            OnMenuItemClick = _OnMenuItemClick ;
        }

        void Popup(){
            tagPOINT      tpPosition ;
            GetCursorPos(&tpPosition);

            if(TrackerType == CTracker::ttRect   ){
                miMenuItems[miTypeRect   ]->Checked = true;
            }
            if(TrackerType == CTracker::ttCircle ){miMenuItems[miTypeCircle ]->Checked = true; }
            if(TrackerType == CTracker::ttPolygon){miMenuItems[miTypePolygon]->Checked = true; }

            if(TrackerType == CTracker::ttPolygon) {
                miMenuItems[miAddPoint    ]->Visible = MoveStat.eLastDnType > CTracker::dtNone ;
                miMenuItems[miDelPoint    ]->Visible = MoveStat.eLastDnType > CTracker::dtNone ;

                miMenuItems[miTypeRect    ]->Visible = MoveStat.eLastDnType < CTracker::dtNone ;
                miMenuItems[miTypeCircle  ]->Visible = MoveStat.eLastDnType < CTracker::dtNone ;
                miMenuItems[miTypePolygon ]->Visible = MoveStat.eLastDnType < CTracker::dtNone ;
                miMenuItems[miSeperator   ]->Visible = MoveStat.eLastDnType < CTracker::dtNone ;

                miMenuItems[miNewInsp     ]->Visible = MoveStat.eLastDnType < CTracker::dtNone ;
                miMenuItems[miNewIgnr     ]->Visible = MoveStat.eLastDnType < CTracker::dtNone ;
                miMenuItems[miDelete      ]->Visible = MoveStat.eLastDnType < CTracker::dtNone ;
                miMenuItems[miCopyAndPaste]->Visible = MoveStat.eLastDnType < CTracker::dtNone ;
            }
            else {
                miMenuItems[miAddPoint    ]->Visible = false ;
                miMenuItems[miDelPoint    ]->Visible = false ;

                miMenuItems[miTypeRect    ]->Visible = true ;
                miMenuItems[miTypeCircle  ]->Visible = true ;
                miMenuItems[miTypePolygon ]->Visible = true ;
                miMenuItems[miSeperator   ]->Visible = true ;

                miMenuItems[miNewInsp     ]->Visible = true ;
                miMenuItems[miNewIgnr     ]->Visible = true ;
                miMenuItems[miDelete      ]->Visible = true ;
                miMenuItems[miCopyAndPaste]->Visible = true ;
            }

            pmMenu -> Popup(tpPosition.x, tpPosition.y) ;
        }

    protected:

        //Tracker Popup
        TPopupMenu *  pmMenu                         ;
        TMenuItem  *  miMenuItems    [MAX_MENU_ITEM] ;

        FOnMenuItemClick OnMenuItemClick ;
        //---------------------------------------------------------------------------
        void __fastcall OnItemClick(TObject *Sender)
        {
            EMenuItem eTag = (EMenuItem)(((TMenuItem *)Sender) -> Tag);

                 if(eTag == miAddPoint   ){InsertPoint(MoveStat.eLastDnType);    }
            else if(eTag == miDelPoint   ){DeletePoint(MoveStat.eLastDnType);    }

            else if(eTag == miTypeRect   ){TrackerType = CTracker::ttRect   ; }
            else if(eTag == miTypeCircle ){TrackerType = CTracker::ttCircle ; }
            else if(eTag == miTypePolygon){TrackerType = CTracker::ttPolygon; }

            OnMenuItemClick(eTag , this);      
            //miCopyAndPaste 이놈들은 콜백으로 외부에서 작업해줘야 한다.
            //miDelete
        }

        void Init()
        {
            OnMenuItemClick = NULL ;
            pmMenu = new TPopupMenu(NULL);
            pmMenu -> OwnerDraw = true ;

            for(int i = 0 ; i < MAX_MENU_ITEM ; i++) {
                miMenuItems[i] = new TMenuItem(pmMenu);
                //miMenuItem[i]-> OnDrawItem = MenuDrawItem ; 메뉴 직접 그릴때.
                miMenuItems[i]-> OnClick = OnItemClick ;
                miMenuItems[i]-> Tag = i ;
                     if(i == miAddPoint    ) {miMenuItems[i]-> Caption = "Add Point"          ;  }
                else if(i == miDelPoint    ) {miMenuItems[i]-> Caption = "Del Point"          ;  }

                else if(i == miTypeRect    ) {miMenuItems[i]-> Caption = "Type-Rect"          ; miMenuItems[i]->GroupIndex = 99 ; miMenuItems[i]-> RadioItem = true ; miMenuItems[i]->Checked = true;}
                else if(i == miTypeCircle  ) {miMenuItems[i]-> Caption = "Type-Circle"        ; miMenuItems[i]->GroupIndex = 99 ; miMenuItems[i]-> RadioItem = true ; }
                else if(i == miTypePolygon ) {miMenuItems[i]-> Caption = "Type-Polygon"       ; miMenuItems[i]->GroupIndex = 99 ; miMenuItems[i]-> RadioItem = true ; }
                else if(i == miSeperator   )  miMenuItems[i]-> Caption = "-"                  ;

                else if(i == miNewInsp     )  miMenuItems[i]-> Caption = "New Inspect Tracker";
                else if(i == miNewIgnr     )  miMenuItems[i]-> Caption = "New Ignore Tracker" ;
                else if(i == miDelete      )  miMenuItems[i]-> Caption = "Delete"             ;
                else if(i == miCopyAndPaste)  miMenuItems[i]-> Caption = "Copy and Paste"     ;


                pmMenu -> Items -> Insert(i  , miMenuItems[i]);
                miMenuItems[i]-> AutoHotkeys = maManual ;
            }

        }

        void Close()
        {
            OnMenuItemClick = NULL ;

            for(int i = 0 ; i < MAX_MENU_ITEM ; i++) {
                delete miMenuItems[i] ;
                miMenuItems[i] = NULL ;
            }

            if(pmMenu) {
                delete pmMenu ;
                pmMenu = NULL ;
            }
        }

        //나중에 연산자 오버라이딩을 확인해보자 ... 지금은 필요 없어서...
        //bool operator =(const CTracker& rc)
        //{
        //    CTracker::=(rc);
        //}
};

#endif

