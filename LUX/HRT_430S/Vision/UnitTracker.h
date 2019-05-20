//---------------------------------------------------------------------------
#ifndef UnitTrackerH
#define UnitTrackerH
#include <Graphics.hpp>
#include <Controls.hpp>

#include <Menus.hpp>

#include "BaseDefine.h"


/*
C++ ���������� C++���� ���� __property ��� ���� �ֽ��ϴ�. 
���� Ŭ�������� ����� �а� ���� �ϻ����� �������� 
�а� ���� ������ ������ �Լ��� ������ �� �ְ� �ϱ� ���� ����մϴ�. 
�̴� �ܼ��� ������������ �����ϰų� ���� ������ ���ϴ� ������ �Ͼ�� �����ν� 
�������� �ڵ��� �����ϰ� �ϸ�, ���� �������� ���ų� ���ϴ� �ΰ� ó���� �ϱ� ���� �뵵�� ����մϴ�. 
Ư�� �ܺο� ���θ� �������� �ʴ� COM �ùֿ� �־�� ���� �ʼ�ó�� ����մϴ�. 
C++��� �����߿� �� �ƽ��� �κ��� �ٷ� get set �� ������ �� �ִ� ������Ƽ�� ǥ������ �������� �ʾҴٴ� ���Դϴ�. 

������ C++���������� __property��� ���� �߰������ν� �̿� ���� ������ �ذ�������, 
������ Ÿ�ӿ� ������Ƽ ������ ���� ������ ������ ������ ������ �մϴ�. 

������ VC++������ VC++.NET���� ������ �Ǹ鼭 property �� �����ϱ� ����������,
�� property�� C++���������� ���� �����ϹǷ� ���������� ����� �� �ֽ��ϴ�. 
property �� ������ Ÿ�ӿ��� �ƴ� COM �ùֿ����� ���� �ڵ� �������� ����ϱ� ���� ���Դϴ�. 

�Ʒ��� C++����, VC++ ���ʿ��� �����ϴ� ������Ƽ�� ���Դϴ�. 
�߿��� ���� ������Ʈ ���鶧 ������ Ÿ�ӿ� ������Ƽ â�� ��Ÿ���� �뵵�� �ƴ� 
�Ϲ� Ŭ������ ����ÿ� �̿��� �� �ִٴ� ���Դϴ�. 

����, ������ __property�� public: ���������ڿ� �δ� ������ �Ϲ� Ŭ���� ���迡�� �̿��Ҽ� ������ 
C++����, VC++ ���ʿ��� ��� ��� �����ϴٴ� ���� Ʋ����. 

class Cproperty 
{ 
private: 
    int m_ID; 
public: 
    __property int ID2 = {read = GetID, write = PutID }; // �����常�� __property ���. VC++������ �����ʿ�. 
    __declspec(property(get=GetID, put=PutID)) int ID;  // ������ ������Ƽ����, VC++������ ������. 

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

�����ؼ� ������ ���� �ְų� �����ö� �޸��忡 GetID �� PutID ��� 
�޽����� ������ ���� ���� �ֽ��ϴ�. 
__property�� ���� �������. 

������ �����̶� ���������� �ø��ϴ�.
*/

/*
1. Overlay ���� ���� �׸��Ƕ��� Wincontrol ���·� �۾��ϴ°� �������մϴ�. 
    ������ Render�� ������ �Լ� �ȿ��� Overlay Surface ���� ���� �׷��ִ� ���·� �����ϸ� �ǰڽ��ϴٸ�, 
    �Ϲ����� �޽�¡�� Ÿ�ñ� ������װ�, Draw�ʰ� Mouse Event�� �̰� ������ ���� �ٽ� �����ؾ� �ɰ̴ϴ�. 
    ���� ī�޶�� ���� ���� ������ Window ���� ���� �׸��ô� ��쿣 ����̾�������. 

2. ���� ������ �������, ȹ���� ������ ���� Window Control���� �׸��Ŵٸ�, 
    ��� ��ũ�� ������ ���� �׵θ��� ������ ������ ó���Ͽ� �׷��� �� �ʿ䰡 �������̹Ƿ� 
    TGraphicControl ������ �����մϴ�. 

3. �� ��� Ʈ��Ŀ�� �׸� �κа� �� ���� �κ��� ���콺 Ŭ���� �Ǵ��ϴ� ȿ������ ����� 
    ���� ���, ���� �߽����� ���� ���콺 Ŀ���� ������ ���� ������� ���� �Ǵ��ϴ� ���̰�, 
    (��Ÿ����� ����) 
    Ȥ��, ���̵� �ٰ����̵� ��� Polygon ���� ����� �׸� ���� �������� ���ΰ� Ŭ���Ǿ����� 
    �Ǵ��ϴ� ����� �ǰ�����.

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

    �� �Լ��� ���ø�, TPoint�� �迭(Points)��, �� ����(N)�� �ѱ� �������� ���ο� ���콺 ��ǥ(P)�� 
    �ִ��� �Ǵ��� �� �ֽ��ϴ�. 

4. ���� ���� �����Ͽ��µ�, Ʈ��Ŀ�� �׸��� �������� �����δ�. (���� �����̰���) 
    �׷� ��쿣 Ʈ��Ŀ�� �÷� ���� ��潺ũ�� ��Ʈ���� DoubleBuffered = true; �� �����Ͻø� OK.
*/



//---------------------------------------------------------------------------
//CLASS TTracker
//SUN Kye Won 2011.10.24
//------------------------------------------------------------------------------

//using namespace Graphics ;


typedef void (__closure *FOnPaint )();

//Ʈ��Ŀ Ŭ����.
class CTracker {
    public :
        //������ ��ÿ� ���� �ϴ� ���� �������� ������....
        //��Ÿ�ӿ� �ٲٴ� ���� ���� �ʴ´�.
        enum ETrackerType {
            ttNone=0     , //�ε� ���� �ʰ� �Ƚ��� ��� �Ѵ�.
            ttRect       , //�⺻���� �簢�� Ʈ��Ŀ.
            ttLine       , //�⺻ ���� ���� Ʈ��Ŀ.
            ttCircle     , //�⺻ ���� Ʈ��Ŀ.
            ttPolygon    , //�ٰ��� Ʈ��Ŀ.
            ttPolyline   , //���߿� �ʿ��Ҷ� ����..��������Ʈ�� ������ �ִ� ���� Ʈ��Ŀ.
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

            //0���ʹ� ����Ʈ��Ŀ ����Ʈ ���̵�.
        };
        enum EPointId {
            //Rect��       Line��          Circle��
            piRectLT = 0 , piLineStt = 0 , piCircleCnt = 0 ,
            piRectRT = 1 , piLineEnd = 1 ,
            piRectRB = 2 ,
            piRectLB = 3 ,
        };

        //������ �Ҹ���.
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

    protected : //�ɹ�.
        bool CheckRectIn    (double _dPntX      , double _dPntY       , double _dX , double _dY , double _dPntHfSize); //�簢����Ʈ �߰�ũ�⸦ �ѱ�.
        bool CheckCircleIn  (double _dPntX      , double _dPntY       , double _dX , double _dY , double _dRad      ); //���� ������ �ѱ�.
        bool CheckLineIn    (TDPoint * _aPoints , int    _iPointCount , double _dX , double _dY , double _dMargin   ); //���� Ŭ������ �Ǵ��� ���� �ѱ�.
        void GetLineBandPnts(TDPoint _tPnt1     , TDPoint _tPnt2      , double _dMargin , TDPoint * _a4Points       ); //_aPoints 4�� ����Ʈ �迭 �Ѱܾ���.


        struct TMoveStat {
            EDownType eDnType ; //Ŭ���ٿ�� ���̵�.
            double    dX, dY  ; //Ŭ���ϰ� �̵��� ��.
            EDownType eLastDnType ; //���������� Ŭ���ٿ�� ���̵�. �˾����� ���� ����.
            bool      bRight  ;    //����Ŭ������ �˾��� ���µ� �ڲ� �������� �ٲ� ����.
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
        bool      m_bMoved      ; //�����̰� ���̺� ���ѻ���.
        bool      m_bInRange    ; //���� ���� ���� �ȿ� ����.
        bool      m_bPreInRange ;

        FOnPaint  m_fpOnPaint   ;

    protected : //����Ʈ �˾� ����.


//���� �ؾ� �ϴ� �͵�.==========================================================
    protected : //���α׷����� ���õǴ� �͵�.
        TDPoint *      m_aPoints        ;
        int            m_iPointCount    ;

        int            FTag             ; void  SetTag            (int          value);   //�ױ׷� ����.
        bool           FVisible         ; void  SetVisible        (bool         value);   //������ ����.
        bool           FEnabled         ; void  SetEnabled        (bool         value);   //Ŭ���� �ȵǰ� ����Ʈ���� �Ⱥ���.
        bool           FBandEnabled     ; void  SetBandEnabled    (bool         value);   //��� ��� ����.
        bool           FRoiEnabled      ; void  SetRoiEnabled     (bool         value);   //ROI ��� ����.

    protected : //User UI Setting Var.
        ETrackerType   FTrackerType     ; void  SetTrackerType    (ETrackerType value);   //Ʈ��Ŀ�� Ÿ�� ����.
        double         FCircleRadian    ; void  SetCircleRadian   (double       value);
        double         FBandWidth       ; void  SetBandWidth      (double       value);
        double         FRoiWidth        ; void  SetRoiWidth       (double       value);   //
        TColor         FLineColor       ; void  SetLineColor      (TColor       value);   //���� ����.
        TColor         FRectPointColor  ; void  SetRectPointColor (TColor       value);   //���� ����.
        TColor         FPointColor      ; void  SetPointColor     (TColor       value);   //���� ����.
        int            FLineWidth       ; void  SetLineWidth      (int          value);
        int            FPointSize       ; void  SetPointSize      (int          value);
        int            FRectPointSize   ; void  SetRectPointSize  (int          value);
        String         FCaption         ; void  SetCaption        (String       value);
//==============================================================================

        //���� ���� ���� �ʴ� �͵�....





    public :
        //__property int ID2 = {read = GetID, write = PutID }; // �����常�� __property ���. VC++������ �����ʿ�.
        //__declspec(property(get=GetID, put=PutID)) int ID;  // ������ ������Ƽ����, VC++������ ������.

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
            //miCopyAndPaste �̳���� �ݹ����� �ܺο��� �۾������ �Ѵ�.
            //miDelete
        }

        void Init()
        {
            OnMenuItemClick = NULL ;
            pmMenu = new TPopupMenu(NULL);
            pmMenu -> OwnerDraw = true ;

            for(int i = 0 ; i < MAX_MENU_ITEM ; i++) {
                miMenuItems[i] = new TMenuItem(pmMenu);
                //miMenuItem[i]-> OnDrawItem = MenuDrawItem ; �޴� ���� �׸���.
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

        //���߿� ������ �������̵��� Ȯ���غ��� ... ������ �ʿ� ���...
        //bool operator =(const CTracker& rc)
        //{
        //    CTracker::=(rc);
        //}
};

#endif

