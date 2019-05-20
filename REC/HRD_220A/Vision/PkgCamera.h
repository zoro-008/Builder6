//---------------------------------------------------------------------------

#ifndef PkgCameraH
#define PkgCameraH

#include "BasePkg.h"

#include "UnitArea.h"
#include "UnitImage.h"

#include "UnitGdi.h"

#include "CamUnit.h"
#include "LightUnit.h"
#include "Calibration.h"
//---------------------------------------------------------------------------
//20150227 선계원 제작.
class CCamera_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CCamera_V01)
    public  :

        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(int ,iGrabDelay    );
            DECLARATION(int ,iCamId        );
            DECLARATION(int ,iLightId      );
            DECLARATION(bool,bSaveLastImg  );
            DECLARATION(int ,iSaveInspImg  );
            //

            TMasterPara(){
                PROPERTY(List , int ,iGrabDelay   ,"Grab Delay(ms)    " , "카메라 Grab Delay" );
                PROPERTY(List , int ,iCamId       ,"Camera Id         " , "카메라 Index"      );
                PROPERTY(List , int ,iLightId     ,"Light Id          " , "조명 Index"        );
                PROPERTY(List , bool,bSaveLastImg ,"Last Img Save(c:) " , "마지막 이미지 저장");
                PROPERTY(List , int ,iSaveInspImg ,"Insp Img Save(d:) " , "검사 이미지 저장"  ); ViSaveInspImg.SetSelList("NotUse,NgOnly,OkOnly,All");
                //imi 카메라 총갯수 받아서 다시 세팅한다.    일단 카메라 끊어졌을때 문제의 소지가 있어서 .... 주석.
                //iCamId.SetMaxVal(m_pCam -> GetMaxCamCnt()) ;


            }
            void SetDefault(){
                iGrabDelay =0;
                iCamId     =0;
                iLightId   =0;
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            TCommonPara(){
            }
            void SetDefault(){
            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool   bRsltOk  ;
            TRslt(){
                Clear();
            }
            ~TRslt(){
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

        CCam  ::TCamUserPara   * CamPara   ;
        CLight::TLightUserPara * LightPara ;

        TRslt Rslt ;
        //TDisp Disp ;

        CLinkedList<CTracker*>   m_lTrckInsp ;
        CLinkedList<TLocalPara*> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.

        //TRAIN이미지.
        CImage * m_pTrainImg ;

        //검사 혹은 Grab시에 이미지.
        CImage * m_pCrntImg ;

        CCalibration Cal ;

        HANDLE m_hGrabEnd ;

    private :
        void OnTrackerChanged(); //트레커 콜백.

        void CamCallback (int _iCamNo); //카메라 콜백..

    public : //카메라에게서 TrainImage 받아오는 함수.=======카메라 PKG만 있는함수.
        CImage * GetTrainImg(){return m_pTrainImg ;}
        CImage * GetCrntImg (){return m_pCrntImg  ;}
        bool LoadImg(String _sFileName){return m_pCrntImg->LoadBmp(_sFileName.c_str());}//VISN[m_iVisnId]->GetImg()->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
        bool SaveImg(String _sFileName){return m_pCrntImg->SaveBmp(_sFileName.c_str());}//VISN[m_iVisnId]->GetImg()->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;

        CCalibration * GetCal(){return &Cal;}
        bool   Ready();
        //void SetDisp(TDisp _tDisp){Disp = _tDisp;} 
    //==========================================================================

        bool CaptureTrainImg(); //카메라 파트의 경우 이미지를 Train 이미지에 복사를 해둔다.
        TMasterPara * GetMPara();

    //순수 가상 함수 단=========================================================
    public :
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
    //==========================================================================
};
#endif


