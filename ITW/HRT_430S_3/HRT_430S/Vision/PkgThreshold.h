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
    public  :

        //Parameter.
        struct TMasterPara {
            CValue aRefOfsXAdd  ;
            CValue aRefOfsYAdd  ;

            TMasterPara(){
                aRefOfsXAdd  .SetProperty("RefOffsetXAdd      " , "기준값 offsetX 주소" , CValue::vtAddress, 0 , 0  );
                aRefOfsYAdd  .SetProperty("RefOffsetYAdd      " , "기준값 offsetY 주소" , CValue::vtAddress, 0 , 0  );
            }
        };
        struct TCommonPara {
            CValue bSkip         ; //검사 수행 여부.
            CValue dPercent      ; //설정 percent
            CValue iThresholdHigh; //설정 높은값.
            CValue iThresholdLow ; //설정 낮은값.
            TCommonPara(){
                bSkip         .SetProperty("Skip         " , "검사 안함         " , CValue::vtBool   , 0 , 0   );
                dPercent      .SetProperty("Percent      " , "세팅범위검사OK비율" , CValue::vtDouble , 0 , 100 );
                iThresholdLow .SetProperty("ThresholdLow " , "검사 쓰레숄드 Low " , CValue::vtInt    , 0 , 255 );
                iThresholdHigh.SetProperty("ThresholdHigh" , "검사 쓰레숄드 High" , CValue::vtInt    , 0 , 255 );
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

            TRslt(){
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

        CLinkedList<TRectTracker*> m_lTrckInsp ;
        CLinkedList<TLocalPara  *> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.

        CLinkedList<TPoint> m_lFailPoint  ;  //검사 페일.

        String m_sCrntJobFilePath ;


    private :
        void OnTrackerChanged();

    public : //순수 가상 함수 단================================================
        //클래스 네임 얻어오는 함수.
        virtual String GetPkgName(){return "CThreshold_V01";/*typeid(this).name();*/} //나중에 짱구굴려보자... 인스턴스 없는 상태에서 클래스 이름 리턴.

        virtual bool Init();
        virtual bool Close();

        //검사 및 러닝 관련.
        virtual bool   GetTrainAvailable();
        virtual bool   Train            (CImage * _pImg);
        virtual void   RsltClear        (); //검사 결과값을 검사전에 클리어 한번 하고 한다.
        virtual bool   GetRslt          ();
        virtual bool   Run              (CImage * _pImg);
        virtual String GetErrMsg        ();

        //화면 출력.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //이미지 화면 클릭 이벤트
        virtual TTracker * MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual TTracker * MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void       MouseUp(TMouseButton Button,TShiftState Shift );

        //파라미터 디스플레이
        virtual void UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);

        //로드 세이브.
        virtual void LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName);
};
#endif


