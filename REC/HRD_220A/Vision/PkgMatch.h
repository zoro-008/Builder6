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
    REGIST_STATICLLINK_HEADER(CMatch_V01)
    public  :
        enum ETrainArea {
            otUnknown = 0 ,
            otNoInsp  = 1 ,
            otDkInsp  = 2 ,
            otLtInsp  = 3 ,
            otTemp    = 4 ,
            otInsp    = 5 ,
            MAX_TRAIN_AREA
        };

        enum ERsltArea {
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
            CLinkedList<CValue2*> List ;
            DECLARATION(int   ,iTrThreshold);
            DECLARATION(int   ,iTrInspOft  );
            DECLARATION(int   ,iTrNoInspOft);
            DECLARATION(int   ,iTrInspFreq );
            DECLARATION(String,sRefOfsXAdd );
            DECLARATION(String,sRefOfsYAdd );
            DECLARATION(String,sOutOfsXAdd );
            DECLARATION(String,sOutOfsYAdd );

            TMasterPara(){
                PROPERTY(List , int   ,iTrThreshold,"Threashold          " , "검사임계값         ");
                PROPERTY(List , int   ,iTrInspOft  ,"Inspection Offset   " , "검사영역의 두께    ");
                PROPERTY(List , int   ,iTrNoInspOft,"NonInspection Offset" , "비검사영역의 두께  ");
                PROPERTY(List , int   ,iTrInspFreq ,"Inspection Frequency" , "검사빈도           ");
                PROPERTY(List , String,sRefOfsXAdd ,"Ref OffsetX Add     " , "기준값 offsetX 주소");
                PROPERTY(List , String,sRefOfsYAdd ,"Ref OffsetY Add     " , "기준값 offsetY 주소");
                PROPERTY(List , String,sOutOfsXAdd ,"Out OffsetX Add     " , "결과 OffsetX 주소  ");
                PROPERTY(List , String,sOutOfsYAdd ,"Out OffsetY Add     " , "결과 OffsetY 주소  ");
                SetDefault();
            }
            void SetDefault(){
                iTrThreshold = 0 ;
                iTrInspOft   = 2 ;
                iTrNoInspOft = 1 ;
                iTrInspFreq  = 1 ;
                sRefOfsXAdd  = "";
                sRefOfsYAdd  = "";
                sOutOfsXAdd  = "";
                sOutOfsYAdd  = "";
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(bool   ,bFindNg);
            DECLARATION(double ,dSinc  );
            TCommonPara(){
                PROPERTY(List , bool   ,bFindNg,"NG On Find     " , "찾았을때 NG처리");
                PROPERTY(List , double ,dSinc  ,"Find Match Rate" , "메치 싱크로율  ");
                SetDefault();
            }
            void SetDefault(){
                bFindNg=false;
                dSinc  =85.0;
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
            ~TRslt(){
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

        CLinkedList<CTracker*>   m_lTrckInsp ;
        CLinkedList<TLocalPara*> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.

        CArea  * m_pTrainArea ;
        CImage * m_pTrainImg  ;

    private :
        void OnTrackerChanged();
        //int  GetAutoThreshold(CImage * _pImg);
    //==========================================================================




    public : //순수 가상 함수 단================================================
        //클래스 네임 얻어오는 함수.
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


