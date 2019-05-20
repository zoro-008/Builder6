//---------------------------------------------------------------------------

#ifndef PkgRectH
#define PkgRectH

#include "BasePkg.h"
#include "aRect.h"
#include "UnitImage.h"
#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 선계원 제작.
class CRect_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CRect_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(String,sRefOfsXAdd   );
            DECLARATION(String,sRefOfsYAdd   );
            DECLARATION(String,sRefOfsTAdd   );
            DECLARATION(String,sRefAbsXAdd   );
            DECLARATION(String,sRefAbsYAdd   );

            DECLARATION(String,sOutOfsXAdd     );
            DECLARATION(String,sOutOfsYAdd     );
            DECLARATION(String,sOutAngleAdd    );

            DECLARATION(bool  ,bOutOfsXInverse );
            DECLARATION(bool  ,bOutOfsYInverse );
            DECLARATION(bool  ,bOutAngleInverse);

            DECLARATION(int   ,iLnSampleGap    );
            DECLARATION(int   ,iLnLineGap      );
            DECLARATION(int   ,iLnEndOffset    );
            DECLARATION(int   ,iLnCntrOffset   );

            DECLARATION(int   ,iEgAverage      );
            DECLARATION(int   ,iEgCompare      );
            DECLARATION(int   ,iEgChkPxLow     );
            DECLARATION(int   ,iEgChkPxHgh     );
            DECLARATION(int   ,iEgOfsGap       );
            DECLARATION(int   ,iEgOfsPxLow     );
            DECLARATION(int   ,iEgOfsPxHgh     );

            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd    ,"Ref OffsetX Add              " , "기준값 offsetX 주소         ");
                PROPERTY(List , String,sRefOfsYAdd    ,"Ref OffsetY Add              " , "기준값 offsetY 주소         ");
                PROPERTY(List , String,sRefOfsTAdd    ,"Ref Offset T Add             " , "기준값 offsetT 주소         ");
                PROPERTY(List , String,sRefAbsXAdd    ,"Ref Absolute X Add           " , "기준값 AbsoluteX 주소       ");
                PROPERTY(List , String,sRefAbsYAdd    ,"Ref Absolute Y Add           " , "기준값 AbsoluteY 주소       ");

                PROPERTY(List , String,sOutOfsXAdd     ,"Out OffsetX Add             " , "결과값 offsetX 주소         ");
                PROPERTY(List , String,sOutOfsYAdd     ,"Out OffsetY Add             " , "결과값 offsetY 주소         ");
                PROPERTY(List , String,sOutAngleAdd    ,"Out Angle Add               " , "결과값 Angle 주소           ");

                PROPERTY(List , bool  ,bOutOfsXInverse ,"Out OffsetX Inverse         " , "결과값 offsetX 반전         ");
                PROPERTY(List , bool  ,bOutOfsYInverse ,"Out OffsetY Inverse         " , "결과값 offsetY 반전         ");
                PROPERTY(List , bool  ,bOutAngleInverse,"Out Angle Inverse           " , "결과값 Angle 반전           ");

                PROPERTY(List , int   ,iLnSampleGap    ,"Line-Sample Gap             " , "라인 Edge 샘플링 간격       ");
                PROPERTY(List , int   ,iLnLineGap      ,"Line-Line In Range          " , "라인 인정 범위              ");
                PROPERTY(List , int   ,iLnEndOffset    ,"Line-Insp Offset Of End     " , "끝단 검사 무시 범위         ");
                PROPERTY(List , int   ,iLnCntrOffset   ,"Line-Insp Offset Of Center  " , "중간 검사 무시 범위         ");

                PROPERTY(List , int   ,iEgAverage      ,"Edge-Average Range          " , "엣지 평균평탄화 범위        ");
                PROPERTY(List , int   ,iEgCompare      ,"Edge-Diff Compare           " , "엣지 비교 범위              ");
                PROPERTY(List , int   ,iEgChkPxLow     ,"Edge-Check Pixel Low        " , "엣지 유효 최소 임계값       ");
                PROPERTY(List , int   ,iEgChkPxHgh     ,"Edge-Check Pixel High       " , "엣지 유효 최대 임계값       ");
                PROPERTY(List , int   ,iEgOfsGap       ,"Edge-Check Offset Pixel Pos " , "엣지 유효 오프셑 거리       ");
                PROPERTY(List , int   ,iEgOfsPxLow     ,"Edge-Check Offset Pixel Low " , "엣지 유효 오프셑 최소 임계값");
                PROPERTY(List , int   ,iEgOfsPxHgh     ,"Edge-Check Offset Pixel High" , "엣지 유효 오프셑 최대 임계값");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd="";
                sRefOfsYAdd="";
                sRefOfsTAdd="";
                sRefAbsXAdd="";
                sRefAbsYAdd="";

                sOutOfsXAdd     = "";
                sOutOfsYAdd     = "";
                sOutAngleAdd    = "";

                bOutOfsXInverse = false ;
                bOutOfsYInverse = false ;
                bOutAngleInverse= false ; 

                iLnSampleGap    = 5 ;
                iLnLineGap      = 1 ;
                iLnEndOffset    = 0 ;
                iLnCntrOffset   = 0 ;

                iEgAverage      = 1 ;
                iEgCompare      = 1 ;
                iEgChkPxLow     = 0 ;
                iEgChkPxHgh     = 0 ;
                iEgOfsGap       = 0 ;
                iEgOfsPxLow     = 0 ;
                iEgOfsPxHgh     = 0 ;
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(bool  ,bLtOnDk        );
            DECLARATION(double,dWidth         );
            DECLARATION(double,dHeight        );
            DECLARATION(double,dTolerance     );

            DECLARATION(double,dPosMax        );
            DECLARATION(double,dAngleMax      );

            TCommonPara(){
                PROPERTY(List , bool  ,bLtOnDk      ,"Find Light On Dark  " , "밝은 사각형 찾기");
                PROPERTY(List , double,dWidth       ,"Rect Width          " , "사각형 넓이     ");
                PROPERTY(List , double,dHeight      ,"Rect Height         " , "사각형 높이     ");
                PROPERTY(List , double,dTolerance   ,"Rect Size Tolerance " , "사이즈 허용공차 ");

                PROPERTY(List , double,dPosMax      ,"Position Tolerance  " , "이미지 센터에서 포지션 허용공차 ");
                PROPERTY(List , double,dAngleMax    ,"Angle Tolerance     " , "각도의 허용공차 ");

                SetDefault();
            }
            void SetDefault(){
                bLtOnDk    = false ;
                dWidth     = 0.0   ;
                dHeight    = 0.0   ;
                dTolerance = 0.0   ;

                dPosMax    = 0.0   ; //
                dAngleMax  = 0.0   ; //

            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool    bInspEnd   ;
            bool    bRsltOk    ;

            TDPoint CrossPntLT ; //교차점.
            TDPoint CrossPntRT ;
            TDPoint CrossPntLB ;
            TDPoint CrossPntRB ;
            TDPoint CenterPnt  ;

            double  dLengthL   ;
            double  dLengthT   ;
            double  dLengthR   ;
            double  dLengthB   ;

            double  dAngleL    ;
            double  dAngleT    ;
            double  dAngleR    ;
            double  dAngleB    ;
            double  dAngle     ;

            double  dOffsetX   ;
            double  dOffsetY   ;

            CLinkedList<TDPoint> lEdgeList ;

            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;

                CrossPntLT.Clear();
                CrossPntRT.Clear();
                CrossPntLB.Clear();
                CrossPntRB.Clear();
                CenterPnt .Clear();

                dLengthL  = 0.0 ;
                dLengthT  = 0.0 ;
                dLengthR  = 0.0 ;
                dLengthB  = 0.0 ;

                dAngleL   = 0.0 ;
                dAngleT   = 0.0 ;
                dAngleR   = 0.0 ;
                dAngleB   = 0.0 ;
                dAngle    = 0.0 ;

                dOffsetX = 0.0 ;
                dOffsetY = 0.0 ;

                lEdgeList.DeleteAll();

            }
        };

        //Functions
        CRect_V01();
        ~CRect_V01();



    //이PKG에 특화된 내부에서만 쓰는 변수 함수들.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt  Rslt ;

        CLinkedList<CTracker   *> m_lTrckInsp ;
        CLinkedList<TLocalPara *> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.


    private :
        void OnTrackerChanged();

    public : //순수 가상 함수 단================================================
        virtual bool Init();
        virtual bool Close();

        //검사 및 러닝 관련.
        virtual bool   Train            (CImage * _pImg);
        virtual void   RsltClear        (); //검사 결과값을 검사전에 클리어 한번 하고 한다.
        virtual bool   GetRslt          ();
        virtual bool   Run              (CImage * _pImg);
        virtual String GetErrMsg        ();

        //화면 출력.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //이미지 화면 클릭 이벤트
        virtual bool MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual bool MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void MouseUp(TMouseButton Button,TShiftState Shift );

        //파라미터 디스플레이
        virtual void UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);

        //로드 세이브.
        virtual void LoadSave(bool _bLoad , String _sVisnName);
};
#endif


