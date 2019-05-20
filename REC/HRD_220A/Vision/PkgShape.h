//---------------------------------------------------------------------------

#ifndef PkgShapeH
#define PkgShapeH

#include "BasePkg.h"
//#include "aRect.h"
#include "UnitImage.h"
#include "UnitArea.h"
//
#include "BaseMaths.h"

#include "aBlob.h"
//---------------------------------------------------------------------------
//20150227 선계원 제작.
class CShape_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CShape_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(String,sRefOfsXAdd   );
            DECLARATION(String,sRefOfsYAdd   );
            DECLARATION(String,sRefOfsTAdd   );
            DECLARATION(String,sRefAbsXAdd   );
            DECLARATION(String,sRefAbsYAdd   );

            DECLARATION(String,sOutCntOfsXAdd);
            DECLARATION(String,sOutCntOfsYAdd);
            DECLARATION(String,sOutAngleAdd  );

            DECLARATION(bool  ,bUseRoiRslt   );



            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd    ,"Ref OffsetX Add            " , "기준값 offsetX 주소         ");
                PROPERTY(List , String,sRefOfsYAdd    ,"Ref OffsetY Add            " , "기준값 offsetY 주소         ");
                PROPERTY(List , String,sRefOfsTAdd    ,"Ref Offset T Add           " , "기준값 offsetT 주소         ");
                PROPERTY(List , String,sRefAbsXAdd    ,"Ref Absolute X Add         " , "기준값 AbsoluteX 주소       ");
                PROPERTY(List , String,sRefAbsYAdd    ,"Ref Absolute Y Add         " , "기준값 AbsoluteY 주소       ");

                PROPERTY(List , String,sOutCntOfsXAdd ,"Out Center OffsetX Add     " , "결과값 Center offsetX 주소  ");
                PROPERTY(List , String,sOutCntOfsYAdd ,"Out Center OffsetY Add     " , "결과값 Center offsetY 주소  ");
                PROPERTY(List , String,sOutAngleAdd   ,"Out Angle Add              " , "결과값 Angle 주소           ");

                PROPERTY(List , bool  ,bUseRoiRslt    ,"Result From ROI With NoInsp" , "검사없이 ROI의 위치값을사용 ");



                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd="";
                sRefOfsYAdd="";
                sRefOfsTAdd="";
                sRefAbsXAdd="";
                sRefAbsYAdd="";

                sOutCntOfsXAdd ="";
                sOutCntOfsYAdd ="";
                sOutAngleAdd   ="";

                bUseRoiRslt    =false ;
            }
        };

        //Parameter.
        struct TMasterParaShape {
            CLinkedList<CValue2*> List ;

            DECLARATION(bool,bLtOnDk       );
            DECLARATION(int ,iLnSampleGap  );
            DECLARATION(int ,iLnLineGap    );
            DECLARATION(int ,iLnEndOffset  );
            DECLARATION(int ,iLnCntrOffset );
            DECLARATION(int ,iCcSampleCnt  );
            DECLARATION(int ,iEgAverage    );
            DECLARATION(int ,iEgCompare    );
            DECLARATION(int ,iEgChkPxLow   );
            DECLARATION(int ,iEgChkPxHgh   );
            DECLARATION(int ,iEgOfsGap     );
            DECLARATION(int ,iEgOfsPxLow   );
            DECLARATION(int ,iEgOfsPxHgh   );
            DECLARATION(bool,bUseLstSqure  );

            TMasterParaShape(){
                PROPERTY(List , bool ,bLtOnDk       ,"Find Light On Dark          " , "밝은 사각형 찾기            ");
                PROPERTY(List , int  ,iLnSampleGap  ,"Line-Sample Gap             " , "라인 Edge 샘플링 간격       ");
                PROPERTY(List , int  ,iLnLineGap    ,"Line-Line In Range          " , "라인 인정 범위              ");
                PROPERTY(List , int  ,iLnEndOffset  ,"Line-Insp Offset Of End     " , "끝단 검사 무시 범위         ");
                PROPERTY(List , int  ,iLnCntrOffset ,"Line-Insp Offset Of Center  " , "중간 검사 무시 범위         ");
                PROPERTY(List , int  ,iCcSampleCnt  ,"Circle-Insp Sample Count    " , "서클 검색시 구하는 엣지 갯수");
                //PROPERTY(List , int  ,iEgAverage    ,"Edge-Average Range          " , "엣지 평균평탄화 범위        ");
                //PROPERTY(List , int  ,iEgCompare    ,"Edge-Diff Compare           " , "엣지 비교 범위              ");
                //PROPERTY(List , int  ,iEgChkPxLow   ,"Edge-Check Pixel Low        " , "엣지 유효 최소 임계값       ");
                //PROPERTY(List , int  ,iEgChkPxHgh   ,"Edge-Check Pixel High       " , "엣지 유효 최대 임계값       ");
                //PROPERTY(List , int  ,iEgOfsGap     ,"Edge-Check Offset Pixel Pos " , "엣지 유효 오프셑 거리       ");
                //PROPERTY(List , int  ,iEgOfsPxLow   ,"Edge-Check Offset Pixel Low " , "엣지 유효 오프셑 최소 임계값");
                //PROPERTY(List , int  ,iEgOfsPxHgh   ,"Edge-Check Offset Pixel High" , "엣지 유효 오프셑 최대 임계값");
                PROPERTY(List , bool ,bUseLstSqure  ,"Use Least Squre             " , "최소자승법 사용            ");

                SetDefault();
            }
            void SetDefault(){
                bLtOnDk       = false ;
                iLnSampleGap  = 5 ;
                iLnLineGap    = 1 ;
                iLnEndOffset  = 0 ;
                iLnCntrOffset = 0 ;
                iCcSampleCnt  = 20;
                iEgAverage    = 1 ;
                iEgCompare    = 1 ;
                iEgChkPxLow   = 0 ;
                iEgChkPxHgh   = 0 ;
                iEgOfsGap     = 0 ;
                iEgOfsPxLow   = 0 ;
                iEgOfsPxHgh   = 0 ;
                bUseLstSqure  = true;
            }
            void operator =(const TMasterParaShape& rc)
            {
                bLtOnDk      = rc.bLtOnDk       ;
                iLnSampleGap = rc.iLnSampleGap  ;
                iLnLineGap   = rc.iLnLineGap    ;
                iLnEndOffset = rc.iLnEndOffset  ;
                iLnCntrOffset= rc.iLnCntrOffset ;
                iCcSampleCnt = rc.iCcSampleCnt  ;
                iEgAverage   = rc.iEgAverage    ;
                iEgCompare   = rc.iEgCompare    ;
                iEgChkPxLow  = rc.iEgChkPxLow   ;
                iEgChkPxHgh  = rc.iEgChkPxHgh   ;
                iEgOfsGap    = rc.iEgOfsGap     ;
                iEgOfsPxLow  = rc.iEgOfsPxLow   ;
                iEgOfsPxHgh  = rc.iEgOfsPxHgh   ;
                bUseLstSqure = rc.bUseLstSqure  ;
            }
        };

        struct TLocalParaPoly {
            CLinkedList<CValue2*> List ;
            DECLARATION(String , sLengths       );
            DECLARATION(String , sAngles        );
            DECLARATION(double , dLengTolerance );
            DECLARATION(double , dAngleTolerance);

            TLocalParaPoly(){
                PROPERTY(List , String , sLengths       ,"Lengths          " , "변의 길이들 스펙  ");
                PROPERTY(List , String , sAngles        ,"sAngles          " , "변의 각도들 스펙  ");
                PROPERTY(List , double , dLengTolerance ,"Length Tolerance " , "변의 길이 허용공차"); VdLengTolerance  .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , double , dAngleTolerance,"Angle Tolerance  " , "변의 각도 허용공차"); VdAngleTolerance .SetMinMax(0,360);

                SetDefault();
            }
            void SetDefault(){
                sLengths        = "100,100,100,100";
                sAngles         = "90,90,90,90";
                dLengTolerance  = 0.0 ;
                dAngleTolerance = 0.0 ;
            }
            void operator =(const TLocalParaPoly& rc)
            {
                sLengths         = rc.sLengths       ;
                sAngles          = rc.sAngles        ;
                dLengTolerance   = rc.dLengTolerance ;
                dAngleTolerance  = rc.dAngleTolerance;
            }
        };


        //Parameter.
        struct TLocalParaCircle {
            CLinkedList<CValue2*> List ;
            DECLARATION(double , dRadian       );
            DECLARATION(double , dRadTolerance );
            DECLARATION(double , dStartAng     );
            DECLARATION(double , dEndAng       );


            TLocalParaCircle(){
                PROPERTY(List , double , dRadian      ,"Radian           " , "반지름          "); VdRadian  .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , double , dRadTolerance,"Radian Tolerance " , "반지름 허용공차 ");
                PROPERTY(List , double , dStartAng    ,"Start Angle      " , "호 검색 시작각도"); VdStartAng.SetMinMax(0,360);
                PROPERTY(List , double , dEndAng      ,"End Angle        " , "호 검색 끝각도  "); VdEndAng  .SetMinMax(0,360);
                SetDefault();
            }
            void SetDefault(){
                dRadian       = 0.0 ;
                dRadTolerance = 0.0 ;
            }
            void operator =(const TLocalParaCircle& rc)
            {
                dRadian       = rc.dRadian      ;
                dRadTolerance = rc.dRadTolerance;
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;

            TCommonPara(){

                SetDefault();
            }
            void SetDefault(){

            }
        };

        struct TTrackerRslt {
            bool      bRsltOk  ;

            CTracker::ETrackerType eTrackerType ;
            //공용.
            int       iPointCnt ;
            TDPoint * pPoints   ; // 다각형 코너 포인트 배열.
            TDPoint   tCntPnt   ; // 포인트들의 중심.
            double    dCntOfsX  ; //중심에서의 거리.
            double    dCntOfsY  ;
            //Circle용.
            double    dRadius   ;

            //다각형용.
            double  * pLengths   ;
            double  * pAngles    ;

            CLinkedList<TDPoint> lEdgeList ;

            TTrackerRslt(){
                eTrackerType = CTracker::ttNone ;
                iPointCnt    = 0 ;
                pPoints      = NULL ;

                pLengths     = NULL ;
                pAngles      = NULL ;
            }
            ~TTrackerRslt(){
                Clear();
            }
            void Clear(){
                bRsltOk = false ;
                eTrackerType = CTracker::ttNone ;
                iPointCnt = 0 ;
                tCntPnt.Clear();
                dCntOfsX = 0;
                dCntOfsY = 0;
                if(pPoints) {
                    delete [] pPoints ;
                    pPoints = NULL;
                }
                if(pLengths) {
                    delete [] pLengths ;
                    pLengths = NULL;
                }
                if(pAngles) {
                    delete [] pAngles ;
                    pAngles = NULL;
                }

                lEdgeList.DeleteAll();
            }
        };

        struct TRslt {
            bool        bInspEnd ;
            bool        bRsltOk  ;

            int            iTrackerRsltCnt ;
            TTrackerRslt * pTrackerRslts   ;


            TRslt(){
                iTrackerRsltCnt = 0    ;
                pTrackerRslts   = NULL ;
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd     = false ;
                bRsltOk      = false ;

                iTrackerRsltCnt = 0 ;
                if(pTrackerRslts) {
                    delete [] pTrackerRslts ;
                    pTrackerRslts = NULL;
                }
            }
        };

        //Functions
        CShape_V01();
        ~CShape_V01();



    //이PKG에 특화된 내부에서만 쓰는 변수 함수들.===============================
    private :
        TMasterPara      MPara      ;
        TMasterParaShape MParaShape ;
        TCommonPara      CPara      ;

        TRslt  Rslt ;

        CLinkedList<CPopupTracker   *> m_lTrckInsp   ;
        CLinkedList<TLocalParaCircle*> m_lLParaCircle;   //갯수가 트레커에 갯수에 따라 늘어남.
        CLinkedList<TLocalParaPoly  *> m_lLParaPoly  ;   //갯수가 트레커에 갯수에 따라 늘어남.

        //검사 무시영역 이것은 로컬파라 생성하지 않는다.
        CLinkedList<CPopupTracker *> m_lTrckIgnr ;

        TLocalParaCircle * m_pActiveLParaCircle;
        TLocalParaPoly   * m_pActiveLParaPoly  ;

        void OnTrackerMenuItemClick(EMenuItem _iItemNo , CPopupTracker* _pTracker) ;

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


