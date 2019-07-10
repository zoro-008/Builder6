//---------------------------------------------------------------------------

#ifndef PkgMatchH
#define PkgMatchH

#include "BasePkg.h"

#include "UnitArea.h"
#include "UnitImage.h"

#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 선계원 제작.
class CMatch_V01 : public CPkgBase
{
    public  :
        enum EN_TRAIN_AREA {
            otUnknown = 0 ,
            otNoInsp  = 1 ,
            otDkInsp  = 2 ,
            otLtInsp  = 3 ,
            otTemp    = 4 ,
            otInsp    = 5 ,
            MAX_TRAIN_AREA
        };

        enum EN_RSLT_AREA {
            orNone    = 0 ,
            orInsp    = 1 ,
            MAX_RSLT_AREA
        };

        struct TPxPoint
        {
            int x,y;
            unsigned char px ;
            TPxPoint() {x=0;y=0;px=0;}
            TPxPoint(int _x, int _y , unsigned char _px) { x=_x; y=_y; px=_px;}
        };

        //Parameter.
        struct TMasterPara {
            CValue iTrThreshold ; //검사 임계값
            CValue iTrInspOft   ;
            CValue iTrNoInspOft ;
            CValue iTrInspFreq  ; //검사빈도수 높으면 높을수록 빨라짐.
            CValue aRefOfsXAdd  ;
            CValue aRefOfsYAdd  ;
            CValue aOutOfsXAdd  ; // OffsetValue X
            CValue aOutOfsYAdd  ; // OffsetValue Y

            TMasterPara(){
                iTrThreshold .SetProperty("Threashold         " , "검사임계값         " , CValue::vtInt    , 0 , 255);
                iTrInspOft   .SetProperty("InspectionOffset   " , "검사영역의 두께    " , CValue::vtInt    , 0 , 10 );
                iTrNoInspOft .SetProperty("NonInspectionOffset" , "비검사영역의 두께  " , CValue::vtInt    , 0 , 10 );
                iTrInspFreq  .SetProperty("InspectionFrequency" , "검사빈도           " , CValue::vtInt    , 0 , 10 );
                aRefOfsXAdd  .SetProperty("RefOffsetXAdd      " , "기준값 offsetX 주소" , CValue::vtAddress, 0 , 0  );
                aRefOfsYAdd  .SetProperty("RefOffsetYAdd      " , "기준값 offsetY 주소" , CValue::vtAddress, 0 , 0  );
                aOutOfsXAdd  .SetProperty("OutOffsetXAdd      " , "결과 OffsetX 주소  " , CValue::vtAddress, 0 , 0  );
                aOutOfsYAdd  .SetProperty("OutOffsetYAdd      " , "결과 OffsetY 주소  " , CValue::vtAddress, 0 , 0  );
            }
        };
        struct TCommonPara {
            CValue bSkip       ; //검사 수행 여부.
            CValue bFindNg      ; //검사 패턴 찾을때 NG 처리.
            CValue dSinc        ; //OK/NG싱크
            TCommonPara(){
                bSkip       .SetProperty("Skip                " , "검사 안함        " , CValue::vtBool   , 0 , 0   );
                bFindNg     .SetProperty("NG On Find          " , "찾았을때 NG처리  " , CValue::vtBool   , 0 , 0   );
                dSinc       .SetProperty("Find Match Rate     " , "메치 싱크로율    " , CValue::vtDouble , 0 , 100 );
            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool   bInspEnd ;
            bool   bRsltOk  ;
            double dCntPosX ;
            double dCntPosY ;
            int    iDkPxCnt ;
            int    iLtPxCnt ;
            double dSinc    ;
            TRect  tRect    ;
            TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;
                dCntPosX = 0   ;
                dCntPosY = 0   ;
                iDkPxCnt = 0   ;
                iLtPxCnt = 0   ;
                dSinc    = 0.0 ;
                tRect.left   = 0 ;
                tRect.right  = 0 ;
                tRect.top    = 0 ;
                tRect.bottom = 0 ;
            }
        };

        struct TOrig {
            double dCntPosX ;
            double dCntPosY ;
            TOrig(){
               dCntPosX = 0 ;
               dCntPosY = 0 ;

            }
        };

        //Functions
        CMatch_V01();
        ~CMatch_V01();



    //이PKG에 특화된 내부에서만 쓰는 변수 함수들.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt  Rslt ;
        TOrig  Orig ; //트레인시에 결과값을 저장 해서 기준위치로 삼는다.

        CLinkedList<TOutBandRectTracker*> m_lTrckInsp ;
        CLinkedList<TLocalPara         *> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.

        CArea  * m_pTrainArea ;
        CImage * m_pTrainImg  ;

        String m_sCrntJobFilePath ;


    private :
        void OnTrackerChanged();
        int  GetAutoThreshold(CImage * _pImg);
    //==========================================================================




    public : //순수 가상 함수 단================================================
        //클래스 네임 얻어오는 함수.
        virtual String GetPkgName(){return "CMatch_V01";/*typeid(this).name();*/} //나중에 짱구굴려보자... 인스턴스 없는 상태에서 클래스 이름 리턴.

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


