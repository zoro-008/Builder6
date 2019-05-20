//---------------------------------------------------------------------------

#ifndef PkgDefectH
#define PkgDefectH

#include "BasePkg.h"
//#include "aRect.h"
#include "UnitImage.h"
#include "UnitArea.h"
//
#include "BaseMaths.h"

#include "aBlob.h"
//---------------------------------------------------------------------------
//20150227 선계원 제작.
class CDefect_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CDefect_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(String,sRefOfsXAdd      );
            DECLARATION(String,sRefOfsYAdd      );
            DECLARATION(String,sRefOfsTAdd      );
            DECLARATION(String,sRefAbsXAdd      );
            DECLARATION(String,sRefAbsYAdd      );
            DECLARATION(String,sOutDefectCntAdd );

            DECLARATION(bool  ,bUsePreEdge   );

            DECLARATION(int   ,iFrstMorpMeth );
            DECLARATION(int   ,iFrstMorpSize );
            DECLARATION(int   ,iScndMorpMeth );
            DECLARATION(int   ,iScndMorpSize );
            DECLARATION(int   ,iThrdMorpMeth );
            DECLARATION(int   ,iThrdMorpSize );

            DECLARATION(int   ,iInspMathod   );


            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd     ,"Ref OffsetX Add         " , "기준값 offsetX 주소         ");
                PROPERTY(List , String,sRefOfsYAdd     ,"Ref OffsetY Add         " , "기준값 offsetY 주소         ");
                PROPERTY(List , String,sRefOfsTAdd     ,"Ref Offset T Add        " , "기준값 offsetT 주소         ");
                PROPERTY(List , String,sRefAbsXAdd     ,"Ref Absolute X Add      " , "기준값 AbsoluteX 주소       ");
                PROPERTY(List , String,sRefAbsYAdd     ,"Ref Absolute Y Add      " , "기준값 AbsoluteY 주소       ");
                PROPERTY(List , String,sOutDefectCntAdd,"Out DefectCnt Add       " , "Defect의 갯수 주소          ");

                PROPERTY(List , bool  ,bUsePreEdge     ,"Use Edge before Insp    " , "엦지로 형상추출 하여 검사   ");

                PROPERTY(List , int   ,iFrstMorpMeth   ,"First Morphology Method " , "첫번째 Morphology 방법      "); ViFrstMorpMeth.SetSelList("None,Erosion,Dilation");
                PROPERTY(List , int   ,iFrstMorpSize   ,"First Morphology Size   " , "첫번째 Morphology 크기      "); ViFrstMorpSize.SetMinMax(1,100);
                PROPERTY(List , int   ,iScndMorpMeth   ,"Second Morphology Method" , "두번째 Morphology 방법      "); ViScndMorpMeth.SetSelList("None,Erosion,Dilation");
                PROPERTY(List , int   ,iScndMorpSize   ,"Second Morphology Size  " , "두번째 Morphology 크기      "); ViScndMorpSize.SetMinMax(1,100);
                PROPERTY(List , int   ,iThrdMorpMeth   ,"Third Morphology Method " , "세번째 Morphology 방법      "); ViThrdMorpMeth.SetSelList("None,Erosion,Dilation");
                PROPERTY(List , int   ,iThrdMorpSize   ,"Third Morphology Size   " , "세번째 Morphology 크기      "); ViThrdMorpSize.SetMinMax(1,100);

                PROPERTY(List , int   ,iInspMathod     ,"Inspection Mathod       " , "검사 방법                   "); ViInspMathod  .SetSelList("Threshold,Edge");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd="";
                sRefOfsYAdd="";
                sRefOfsTAdd="";
                sRefAbsXAdd="";
                sRefAbsYAdd="";
                sOutDefectCntAdd = "";

                bUsePreEdge   = false ;

                iFrstMorpMeth = 0 ;
                iFrstMorpSize = 0 ;
                iScndMorpMeth = 0 ;
                iScndMorpSize = 0 ;
                iThrdMorpMeth = 0 ;
                iThrdMorpSize = 0 ;

                iInspMathod   = 0 ;

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

                SetDefault();
            }
            void SetDefault(){
                bLtOnDk       = false ;
                iLnSampleGap  = 5 ;
                iLnLineGap    = 1 ;
                iLnEndOffset  = 0 ;
                iLnCntrOffset = 0 ;
                iEgAverage    = 1 ;
                iEgCompare    = 1 ;
                iEgChkPxLow   = 0 ;
                iEgChkPxHgh   = 0 ;
                iEgOfsGap     = 0 ;
                iEgOfsPxLow   = 0 ;
                iEgOfsPxHgh   = 0 ;
            }
            void operator =(const TMasterParaShape& rc)
            {
                bLtOnDk      = rc.bLtOnDk       ;
                iLnSampleGap = rc.iLnSampleGap  ;
                iLnLineGap   = rc.iLnLineGap    ;
                iLnEndOffset = rc.iLnEndOffset  ;
                iLnCntrOffset= rc.iLnCntrOffset ;
                iEgAverage   = rc.iEgAverage    ;
                iEgCompare   = rc.iEgCompare    ;
                iEgChkPxLow  = rc.iEgChkPxLow   ;
                iEgChkPxHgh  = rc.iEgChkPxHgh   ;
                iEgOfsGap    = rc.iEgOfsGap     ;
                iEgOfsPxLow  = rc.iEgOfsPxLow   ;
                iEgOfsPxHgh  = rc.iEgOfsPxHgh   ;
            }
        };

        struct TLocalPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(int   ,iInspOffset   );
            TLocalPara(){
                PROPERTY(List , int   ,iInspOffset    ,"Inspection Area Offset  " , "검사 영역 오프셑            ");

                SetDefault();
            }
            void SetDefault(){
                iInspOffset   = 0 ;

            }
            void operator =(const TLocalPara& rc)
            {
                iInspOffset = rc.iInspOffset ;

            }
        };


        //Parameter.
        struct TLocalParaPoly {
            CLinkedList<CValue2*> List ;

            TLocalParaPoly(){
                SetDefault();
            }
            void SetDefault(){
            }
            void operator =(const TLocalParaPoly& rc)
            {
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(int , iMinHeight     );
            DECLARATION(int , iMaxHeight     );
            DECLARATION(int , iMinWidth      );
            DECLARATION(int , iMaxWidth      );

            DECLARATION(int , dMinBoundary   );
            DECLARATION(int , dMaxBoundary   );
            DECLARATION(int , dMinCompact    );
            DECLARATION(int , dMaxCompact    );

            DECLARATION(int , iMinArea       );
            DECLARATION(int , iMaxArea       );
            DECLARATION(int , iMinTotalCnt   );

            TCommonPara(){
                PROPERTY(List , int ,iMinHeight    ,"Min Height     " , "최소 높이       "); ViMinHeight  .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , int ,iMaxHeight    ,"Max Height     " , "최대 높이       "); ViMaxHeight  .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , int ,iMinWidth     ,"Min Width      " , "최소 넓이       "); ViMinWidth   .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , int ,iMaxWidth     ,"Max Width      " , "최대 넓이       "); ViMaxWidth   .SetMinMax(0,MAX_INT_VALUE);

                PROPERTY(List , int ,dMinBoundary  ,"Min Boundary   " , "최소 테두리 길이"); VdMinBoundary.SetMinMax(0,FLT_MAX);
                PROPERTY(List , int ,dMaxBoundary  ,"Max Boundary   " , "최대 테두리 길이"); VdMaxBoundary.SetMinMax(0,FLT_MAX);
                PROPERTY(List , int ,dMinCompact   ,"Min Compact    " , "최소 밀도       "); VdMinCompact .SetMinMax(0,FLT_MAX);
                PROPERTY(List , int ,dMaxCompact   ,"Max Compact    " , "최대 밀도       "); VdMaxCompact .SetMinMax(0,FLT_MAX);

                PROPERTY(List , int ,iMinArea      ,"Min Area       " , "최소 면적       "); ViMinArea    .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , int ,iMaxArea      ,"Max Area       " , "최대 면적       "); ViMaxArea    .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , int ,iMinTotalCnt  ,"Min TotalCnt   " , "최소 판단 갯수  "); ViMinTotalCnt.SetMinMax(0,MAX_INT_VALUE);

                SetDefault();
            }
            void SetDefault(){
                iMinHeight   = 0 ;
                iMaxHeight   = 0 ;
                iMinWidth    = 0 ;
                iMaxWidth    = 0 ;

                dMinBoundary = 0 ;
                dMaxBoundary = 0 ;
                dMinCompact  = 0 ;
                dMaxCompact  = 0 ;

                iMinArea     = 0 ;
                iMaxArea     = 0 ;
                iMinTotalCnt = 0 ;

            }
        };
        struct TCommonParaThres { //쓰레숄딩 방법 파라.
            CLinkedList<CValue2*> List ;

            DECLARATION(int , iLowThreshold  );
            DECLARATION(int , iHighThreshold );

            TCommonParaThres(){
                PROPERTY(List , int ,iLowThreshold     ,"Low Threshold      " , "Threshold 최소값 "); ViLowThreshold .SetMinMax(0,256);
                PROPERTY(List , int ,iHighThreshold    ,"High Threshold     " , "Threshold 높은값 "); ViHighThreshold.SetMinMax(0,256);

                SetDefault();
            }
            void SetDefault(){
                iLowThreshold  = 0 ;
                iHighThreshold = 0 ;

            }
        };
        struct TCommonParaEdge {// 엣지 방법 파라.
            CLinkedList<CValue2*> List ;

            TCommonParaEdge(){
                SetDefault();
            }
            void SetDefault(){
            }
        };



        struct TTrackerRslt {
            CTracker::ETrackerType eTrackerType ;
            int       iRadius   ;
            int       iPointCnt ;
            TDPoint * pPoints   ; // 다각형 코너 포인트 배열.
            CLinkedList<TDPoint> lLineEdgeList ;

            TTrackerRslt(){
                eTrackerType = CTracker::ttNone ;
                iPointCnt    = 0 ;
                pPoints      = NULL ;
            }
            ~TTrackerRslt(){
                Clear();
            }
            void Clear(){
                eTrackerType = CTracker::ttNone ;
                iPointCnt = 0 ;
                if(pPoints) {
                    delete [] pPoints ;
                    pPoints = NULL;
                }
                lLineEdgeList.DeleteAll();
            }
        };

        struct TRslt {
            bool        bInspEnd ;
            bool        bRsltOk  ;

            int         iDefectCnt ;

            int            iTrackerRsltCnt ;
            TTrackerRslt * pTrackerRslts   ;

            CArea       InspArea       ;
            CArea       DefectArea     ;

            CBlob::TRslt BlobRslt ;

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

                InspArea      .ClrArea();
                DefectArea    .ClrArea();
                BlobRslt      .Clear  ();
            }
        };

        //Functions
        CDefect_V01();
        ~CDefect_V01();



    //이PKG에 특화된 내부에서만 쓰는 변수 함수들.===============================
    private :
        TMasterPara      MPara      ;
        TMasterParaShape MParaShape ;
        TCommonPara      CPara      ;
        TCommonParaThres CParaThres ;
        TCommonParaEdge  CParaEdge  ;

        TRslt  Rslt ;

        CLinkedList<CPopupTracker *> m_lTrckInsp ;
        CLinkedList<TLocalPara    *> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.
        CLinkedList<TLocalParaPoly*> m_lLParaPoly;   //갯수가 트레커에 갯수에 따라 늘어남.

        //검사 무시영역 이것은 로컬파라 생성하지 않는다.
        CLinkedList<CPopupTracker *> m_lTrckIgnr ;

        TLocalPara     * m_pActiveLPara     ;
        TLocalParaPoly * m_pActiveLParaPoly ;

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


