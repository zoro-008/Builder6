//---------------------------------------------------------------------------

#ifndef PkgCameraH
#define PkgCameraH

#include "BasePkg.h"

#include "UnitArea.h"
#include "UnitImage.h"

#include "UnitGdi.h"

#include "CamUnit.h"
//---------------------------------------------------------------------------
//20150227 선계원 제작.
class CCamera_V01 : public CPkgBase
{
    public  :
        //static String GetClassName(String & _sClassName){ CCamera_V01;} 나중에 하자..

        //Parameter.
        struct TMasterPara {
            CValue iCamId      ;
            TMasterPara(){
                iCamId .SetProperty("iCamId" , "iCamId" , CValue::vtEnum  , 0 , 10);
            }
        };
        struct TCommonPara {

            CValue iBrightness ;
            CValue iSharpness  ;
            CValue iGain       ;
            CValue iShutter    ;

            TCommonPara(){

                iBrightness .SetProperty("Brightness" , "Brightness" , CValue::vtInt    , 0 , 1000);
                iSharpness  .SetProperty("Sharpness " , "Sharpness " , CValue::vtInt    , 0 , 1000);
                iGain       .SetProperty("Gain      " , "Gain      " , CValue::vtInt    , 0 , 1000);
                iShutter    .SetProperty("Shutter   " , "Shutter   " , CValue::vtInt    , 0 , 1000);
            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool   bRsltOk  ;
            TRslt(){
                Clear();
            }
            void Clear(){
                bRsltOk = false ;
            }
        };

        /*
        enum EN_DISP_MODE {
            doGray   = 0  ,
            doColor       ,
            doFalse       ,
            doBinary      ,
            doRed         ,
            doGreen       ,
            doBlue        ,
            MAX_DISP_OPTN
        };

        struct TDisp {
            bool         bCntrLine ;
            EN_DISP_MODE eMode     ;
        }; */

        //Functions
        CCamera_V01();
        ~CCamera_V01();


    //이PKG에 특화된 내부에서만 쓰는 변수 함수들.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt Rslt ;
        //TDisp Disp ;

        CLinkedList<TOutBandRectTracker*> m_lTrckInsp ;
        CLinkedList<TLocalPara         *> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.

        String m_sCrntJobFilePath ;

        //카메라
        //CCam * m_pCam ;   //카메라 인덱스로 바꿔야 된다.;;;;;
        //잡파일 바뀔때 마다 new delete 되서 테이블로 만들고 인덱스로 가져간다.

        //TRAIN이미지.
        CImage * m_pTrainImg ;

        //검사 혹은 Grab시에 이미지.
        CImage * m_pCrntImg ;

        HANDLE m_hGrabEnd ;

    private :
        void OnTrackerChanged(); //트레커 콜백.

        //라이트 컨트롤러 달리면 여기에 추가 해야함.
        //

        //bool m_bGrabEnd ;
        void CamCallback (int _iCamNo); //카메라 콜백..

    public : //카메라에게서 TrainImage 받아오는 함수.=======카메라 PKG만 있는함수.
        CImage * GetTrainImg(){return m_pTrainImg ;}
        CImage * GetCrntImg (){return m_pCrntImg  ;}
        bool LoadImg(String _sFileName){return m_pCrntImg->LoadBmp(_sFileName.c_str());}//VISN[m_iVisnId]->GetImg()->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
        bool SaveImg(String _sFileName){return m_pCrntImg->SaveBmp(_sFileName.c_str());}//VISN[m_iVisnId]->GetImg()->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
        //void SetDisp(TDisp _tDisp){Disp = _tDisp;} 
    //==========================================================================

        bool CaptureTrainImg(); //카메라 파트의 경우 이미지를 Train 이미지에 복사를 해둔다.
    //순수 가상 함수 단=========================================================
    public :
        //클래스 네임 얻어오는 함수.
        virtual String GetPkgName (){return "CCamera_V01";} //이거상속받으면 만들어야한다.
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
    //==========================================================================
};
#endif


