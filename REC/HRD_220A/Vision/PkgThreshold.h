//---------------------------------------------------------------------------

#ifndef PkgThresholdH
#define PkgThresholdH

#include "BasePkg.h"

#include "UnitArea.h"
#include "UnitImage.h"

#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 선계원 제작.
class CThreshold_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CThreshold_V01)
    public  :

        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(String,sRefOfsXAdd );
            DECLARATION(String,sRefOfsYAdd );

            DECLARATION(String,sOutCntrdXAdd );
            DECLARATION(String,sOutCntrdYAdd );

            DECLARATION(String,sOutCntrdCntOfsXAdd );
            DECLARATION(String,sOutCntrdCntOfsYAdd );

            DECLARATION(bool  ,bOutOfsXInverse );
            DECLARATION(bool  ,bOutOfsYInverse );

            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd         ,"Ref OffsetX Add"     , "기준값 offsetX 주소");
                PROPERTY(List , String,sRefOfsYAdd         ,"Ref OffsetY Add"     , "기준값 offsetY 주소");

                PROPERTY(List , String,sOutCntrdXAdd       ,"Out CentroidX Add"   , "무게중심X 결과 주소");
                PROPERTY(List , String,sOutCntrdYAdd       ,"Out CentroidY Add"   , "무게중심Y 결과 주소");

                PROPERTY(List , String,sOutCntrdCntOfsXAdd ,"Out CentroidX Add"   , "무게중심X 중심 offset 결과 주소");
                PROPERTY(List , String,sOutCntrdCntOfsYAdd ,"Out CentroidY Add"   , "무게중심Y 중심 offset 결과 주소");

                PROPERTY(List , bool  ,bOutOfsXInverse     ,"Out OffsetX Inverse" , "결과값 offsetX 반전");
                PROPERTY(List , bool  ,bOutOfsYInverse     ,"Out OffsetY Inverse" , "결과값 offsetY 반전");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd="";
                sRefOfsYAdd="";

                sOutCntrdXAdd="";      
                sOutCntrdYAdd="";

                sOutCntrdCntOfsXAdd="";
                sOutCntrdCntOfsYAdd="";

                bOutOfsXInverse = false ;
                bOutOfsYInverse = false ;

            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(double ,dPercent      ); //설정 percent
            DECLARATION(int    ,iThresholdLow ); //설정 낮은값.
            DECLARATION(int    ,iThresholdHigh); //설정 높은값.

            TCommonPara(){
                PROPERTY(List , double,dPercent       ,"Percent      " , "세팅범위검사OK비율"); VdPercent      .SetMinMax(0,100);
                PROPERTY(List , int   ,iThresholdLow  ,"ThresholdLow " , "검사 쓰레숄드 Low "); ViThresholdLow .SetMinMax(0,255);
                PROPERTY(List , int   ,iThresholdHigh ,"ThresholdHigh" , "검사 쓰레숄드 High"); ViThresholdHigh.SetMinMax(0,255);
                SetDefault();
            }
            void SetDefault(){
                dPercent      =85.0 ;
                iThresholdLow =0    ;
                iThresholdHigh=255  ;
            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool   bInspEnd ;
            bool   bRsltOk  ;
            TRect  tRect    ;
            int    iPxCnt   ;
            double dAverage ;
            double dPercent ;

            double dCentroidX ;
            double dCentroidY ;

            double dCentroidOfsCntX ;
            double dCentroidOfsCntY ;

            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;
                iPxCnt   = 0  ;
                dAverage = 0.0;
                dPercent = 0  ;
                tRect.left   = 0 ;
                tRect.right  = 0 ;
                tRect.top    = 0 ;
                tRect.bottom = 0 ;

                dCentroidX = 0.0;
                dCentroidY = 0.0;

                dCentroidOfsCntX = 0.0 ;
                dCentroidOfsCntY = 0.0 ;

            }
        };

        //Functions
        CThreshold_V01();
        ~CThreshold_V01();



    //이PKG에 특화된 내부에서만 쓰는 변수 함수들.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt  Rslt ;

        CLinkedList<CTracker*>   m_lTrckInsp ;
        CLinkedList<TLocalPara*> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.

        CLinkedList<TPoint> m_lFailPoint  ;  //검사 페일.

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


