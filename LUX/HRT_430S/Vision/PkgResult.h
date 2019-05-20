//---------------------------------------------------------------------------

#ifndef PkgResultH
#define PkgResultH

#include "BasePkg.h"

#include "UnitArea.h"
#include "UnitImage.h"

#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 선계원 제작.
class CResult_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CResult_V01)
    public  :

        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            TMasterPara(){
                SetDefault();
            }
            void SetDefault(){
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(String,sResultMsg );
            TCommonPara(){
                PROPERTY(List , String,sResultMsg   ,"Result Msg" , "결과 메세지 ValueList항목을 <>로 표현한다.");
                SetDefault();
            }
            void SetDefault(){
                sResultMsg="";
            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool   bInspEnd ;
            bool   bRsltOk  ;

            String sResultMsg ;


            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd   = false ;
                bRsltOk    = false ;
                sResultMsg = ""    ;
            }
        };

        //Functions
        CResult_V01();
        ~CResult_V01();



    //이PKG에 특화된 내부에서만 쓰는 변수 함수들.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt  Rslt ;

        CLinkedList<CTracker*>   m_lTrckInsp ;
        CLinkedList<TLocalPara*> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.

    private :
        void OnTrackerChanged();

    public : //Result PKG에서만 있는 퍼블릭.
        String GetRsltMsg();

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


