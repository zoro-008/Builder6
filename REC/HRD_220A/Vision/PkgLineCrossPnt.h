//---------------------------------------------------------------------------

#ifndef PkgLineCrossPntH
#define PkgLineCrossPntH

#include "BasePkg.h"
#include "UnitImage.h"
#include "aLine.h"
//---------------------------------------------------------------------------
//20150227 선계원 제작.
#define LINE_CROSSPNT_INSP_CNT 2
class CLineCrossPnt_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CLineCrossPnt_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(String,sRefOfsXAdd     );
            DECLARATION(String,sRefOfsYAdd     );

            DECLARATION(String,sOutOfsXAdd     );
            DECLARATION(String,sOutOfsYAdd     );
            DECLARATION(String,sOutOfsTAdd     );
            DECLARATION(String,sOutAbsXAdd     );
            DECLARATION(String,sOutAbsYAdd     );


            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd     ,"Ref OffsetX Add             " , "기준값 offsetX 주소         ");
                PROPERTY(List , String,sRefOfsYAdd     ,"Ref OffsetY Add             " , "기준값 offsetY 주소         ");

                PROPERTY(List , String,sOutOfsXAdd     ,"Out OffsetX Add             " , "결과값 offsetX 주소         ");
                PROPERTY(List , String,sOutOfsYAdd     ,"Out OffsetY Add             " , "결과값 offsetY 주소         ");
                PROPERTY(List , String,sOutOfsTAdd     ,"Out OffsetT Add             " , "결과값 Offset Angle 주소    ");

                PROPERTY(List , String,sOutAbsXAdd     ,"Out Absolute X Add          " , "결과값 Offset Angle 주소    ");
                PROPERTY(List , String,sOutAbsYAdd     ,"Out Absolute Y Add          " , "결과값 Offset Angle 주소    ");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd     = "";
                sRefOfsYAdd     = "";

                sOutOfsXAdd     = "";
                sOutOfsYAdd     = "";
                sOutOfsTAdd     = "";

                sOutAbsXAdd     = "";
                sOutAbsYAdd     = "";
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(double,dPosMax    );
            DECLARATION(double,dAngleMax  );
            DECLARATION(double,dOffsetX   );
            DECLARATION(double,dOffsetY   );


            TCommonPara(){
                PROPERTY(List , double,dPosMax      ,"Position Tolerance  " , "이미지 센터에서 포지션 허용공차 ");
                PROPERTY(List , double,dAngleMax    ,"Angle Tolerance     " , "각도의 허용공차  ");
                PROPERTY(List , double,dOffsetX     ,"Offset X            " , "결과값의 오프셑 X");
                PROPERTY(List , double,dOffsetY     ,"Offset Y            " , "결과값의 오프셑 Y");

                SetDefault();
            }
            void SetDefault(){
                dPosMax    = 0.0 ; //
                dAngleMax  = 0.0 ; //
                dOffsetX   = 0.0 ;
                dOffsetY   = 0.0 ;
            }
        };
        struct TLocalPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(bool  ,bLnLtToDk       );
            DECLARATION(int   ,iLnScanDirection);
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




            TLocalPara(){
                PROPERTY(List , bool  ,bLnLtToDk         ,"Line-Find Light To Dark     " , "밝았다가 어두워지는 선 찾기" );

                PROPERTY(List , int   ,iLnScanDirection  ,"Line-Scan Direction         " , "라인 검색 방향              "); ViLnScanDirection.SetSelList("UpToDn,DnToUp,LeftToRight,RightToLeft");

                PROPERTY(List , int   ,iLnSampleGap      ,"Line-Sample Gap             " , "라인 Edge 샘플링 간격       ");
                PROPERTY(List , int   ,iLnLineGap        ,"Line-Line In Range          " , "라인 인정 범위              ");
                PROPERTY(List , int   ,iLnEndOffset      ,"Line-Insp Offset Of End     " , "끝단 검사 무시 범위         ");
                PROPERTY(List , int   ,iLnCntrOffset     ,"Line-Insp Offset Of Center  " , "중간 검사 무시 범위         ");

                PROPERTY(List , int   ,iEgAverage        ,"Edge-Average Range          " , "엣지 평균평탄화 범위        ");
                PROPERTY(List , int   ,iEgCompare        ,"Edge-Diff Compare           " , "엣지 비교 범위              ");
                PROPERTY(List , int   ,iEgChkPxLow       ,"Edge-Check Pixel Low        " , "엣지 유효 최소 임계값       ");
                PROPERTY(List , int   ,iEgChkPxHgh       ,"Edge-Check Pixel High       " , "엣지 유효 최대 임계값       ");
                PROPERTY(List , int   ,iEgOfsGap         ,"Edge-Check Offset Pixel Pos " , "엣지 유효 오프셑 거리       ");
                PROPERTY(List , int   ,iEgOfsPxLow       ,"Edge-Check Offset Pixel Low " , "엣지 유효 오프셑 최소 임계값");
                PROPERTY(List , int   ,iEgOfsPxHgh       ,"Edge-Check Offset Pixel High" , "엣지 유효 오프셑 최대 임계값");
                SetDefault();
            }

            void SetDefault(){
                bLnLtToDk    = false ;

                iLnScanDirection = CLine::lsUpToDn ;

                iLnSampleGap     = 5 ;
                iLnLineGap       = 1 ;
                iLnEndOffset     = 0 ;
                iLnCntrOffset    = 0 ;

                iEgAverage       = 1 ;
                iEgCompare       = 1 ;
                iEgChkPxLow      = 0 ;
                iEgChkPxHgh      = 0 ;
                iEgOfsGap        = 0 ;
                iEgOfsPxLow      = 0 ;
                iEgOfsPxHgh      = 0 ;

            }



        };

        struct TRslt {
            bool    bInspEnd ;
            bool    bRsltOk  ;

            double  dOffsetX ;
            double  dOffsetY ;
            double  dOffsetT ;

            double  dX ;
            double  dY ;
            double  dT ;

            CLine::TRslt tMainRslt ;
            CLine::TRslt tSubRslt  ;

            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;

                dOffsetX = dX = 0.0 ;
                dOffsetY = dY = 0.0 ;
                dOffsetT = dT = 0.0 ;

                tMainRslt.Clear();
                tSubRslt .Clear();

            }
        };

        struct TOrig {
            double dPosX ;
            double dPosY ;
            double dAngle;
            TOrig(){
               dPosX  = 0 ;
               dPosY  = 0 ;
               dAngle = 0 ;

            }
        };

        //Functions
        CLineCrossPnt_V01();
        ~CLineCrossPnt_V01();



    //이PKG에 특화된 내부에서만 쓰는 변수 함수들.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TOrig  Orig ;
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


