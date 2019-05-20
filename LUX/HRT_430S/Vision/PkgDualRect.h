//---------------------------------------------------------------------------

#ifndef PkgDualRectH
#define PkgDualRectH

#include "BasePkg.h"
#include "aRect.h"
#include "UnitImage.h"
#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 선계원 제작.

#define DUAL_RECT_INSP_CNT 2
class CDualRect_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CDualRect_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(String,sRefOfsXAdd    );
            DECLARATION(String,sRefOfsYAdd    );

            DECLARATION(int   ,iLnSampleGap   );
            DECLARATION(int   ,iLnLineGap     );
            DECLARATION(int   ,iLnEndOffset   );
            DECLARATION(int   ,iLnCntrOffset  );

            DECLARATION(int   ,iEgAverage     );
            DECLARATION(int   ,iEgCompare     );
            DECLARATION(int   ,iEgChkPxLow    );
            DECLARATION(int   ,iEgChkPxHgh    );
            DECLARATION(int   ,iEgOfsGap      );
            DECLARATION(int   ,iEgOfsPxLow    );
            DECLARATION(int   ,iEgOfsPxHgh    );

            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd    ,"Ref OffsetX Add             " , "기준값 offsetX 주소         ");
                PROPERTY(List , String,sRefOfsYAdd    ,"Ref OffsetY Add             " , "기준값 offsetY 주소         ");

                PROPERTY(List , int   ,iLnSampleGap   ,"Line-Sample Gap             " , "라인 Edge 샘플링 간격       ");
                PROPERTY(List , int   ,iLnLineGap     ,"Line-Line In Range          " , "라인 인정 범위              ");
                PROPERTY(List , int   ,iLnEndOffset   ,"Line-Insp Offset Of End     " , "끝단 검사 무시 범위         ");
                PROPERTY(List , int   ,iLnCntrOffset  ,"Line-Insp Offset Of Center  " , "중간 검사 무시 범위         ");

                PROPERTY(List , int   ,iEgAverage     ,"Edge-Average Range          " , "엣지 평균평탄화 범위        ");
                PROPERTY(List , int   ,iEgCompare     ,"Edge-Diff Compare           " , "엣지 비교 범위              ");
                PROPERTY(List , int   ,iEgChkPxLow    ,"Edge-Check Pixel Low        " , "엣지 유효 최소 임계값       ");
                PROPERTY(List , int   ,iEgChkPxHgh    ,"Edge-Check Pixel High       " , "엣지 유효 최대 임계값       ");
                PROPERTY(List , int   ,iEgOfsGap      ,"Edge-Check Offset Pixel Pos " , "엣지 유효 오프셑 거리       ");
                PROPERTY(List , int   ,iEgOfsPxLow    ,"Edge-Check Offset Pixel Low " , "엣지 유효 오프셑 최소 임계값");
                PROPERTY(List , int   ,iEgOfsPxHgh    ,"Edge-Check Offset Pixel High" , "엣지 유효 오프셑 최대 임계값");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd     = "";
                sRefOfsYAdd     = "";

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
            DECLARATION(double,dXOffset        );
            DECLARATION(double,dYOffset        );
            DECLARATION(double,dOffsetTolerance);

            DECLARATION(double,dAngleTolerance );

            TCommonPara(){
                PROPERTY(List , double,dXOffset        ,"Main-Sub OffsetX" , "Main Center에서 Sub Center 까지의 X 거리"       );
                PROPERTY(List , double,dYOffset        ,"Main-Sub OffsetX" , "Main Center에서 Sub Center 까지의 Y 거리"       );
                PROPERTY(List , double,dOffsetTolerance,"Offset Tolerance" , "Main Center에서 Sub Center 까지의 거리 허용공차");
                PROPERTY(List , double,dAngleTolerance ,"Angle Tolerance " , "Main 기준 Sub Rect 각도"                        );
                SetDefault();
            }
            void SetDefault(){
                dXOffset         = 0.0 ;
                dYOffset         = 0.0 ;
                dOffsetTolerance = 0.0 ;
                dAngleTolerance  = 0.0 ;
            }

        };
        struct TLocalPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(bool  ,bLtOnDk        );
            DECLARATION(double,dWidth         );
            DECLARATION(double,dHeight        );
            DECLARATION(double,dTolerance     );


            TLocalPara(){
                PROPERTY(List , bool  ,bLtOnDk      ,"Find Light On Dark  " , "밝은 사각형 찾기");
                PROPERTY(List , double,dWidth       ,"Rect Width          " , "사각형 넓이     ");
                PROPERTY(List , double,dHeight      ,"Rect Height         " , "사각형 높이     ");
                PROPERTY(List , double,dTolerance   ,"Rect Size Tolerance " , "사이즈 허용공차 ");

                SetDefault();
            }
            void SetDefault(){
                bLtOnDk    = false ;
                dWidth     = 0.0   ;
                dHeight    = 0.0   ;
                dTolerance = 0.0   ;

            }
        };

        struct TRslt {
            bool    bInspEnd ;
            bool    bRsltOk  ;

            double  dOffsetX ; //메인 사각형 T를 0도로 맞추고 난후 서브 사각형 간의 X 오프셑.
            double  dOffsetY ; //메인 사각형 T를 0도로 맞추고 난후 서브 사각형 간의 Y 오프셑.
            double  dOffsetT ; //메인 사각형과 서브 사각형간의 각도 차이.

            double  dLengthL[DUAL_RECT_INSP_CNT];
            double  dLengthT[DUAL_RECT_INSP_CNT];
            double  dLengthR[DUAL_RECT_INSP_CNT];
            double  dLengthB[DUAL_RECT_INSP_CNT];

            CRect::TRslt RectRslt[DUAL_RECT_INSP_CNT] ; //귀찮아서 링크드리스트 안씀.

            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;

                dOffsetX = 0.0 ;
                dOffsetY = 0.0 ;
                dOffsetT = 0.0 ;

                for(int i=0 ; i < DUAL_RECT_INSP_CNT ;i++){
                    dLengthL[i]= 0.0 ;
                    dLengthT[i]= 0.0 ;
                    dLengthR[i]= 0.0 ;
                    dLengthB[i]= 0.0 ;
                    RectRslt[i].Clear();
                }
            }
        };

        //Functions
        CDualRect_V01();
        ~CDualRect_V01();



    //이PKG에 특화된 내부에서만 쓰는 변수 함수들.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt  Rslt ;

        CLinkedList<CTracker   *> m_lTrckInsp ;
        CLinkedList<TLocalPara *> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.

        TLocalPara * m_pActiveLPara ;


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


