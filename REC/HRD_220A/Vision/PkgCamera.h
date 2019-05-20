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
//20150227 ����� ����.
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
                PROPERTY(List , int ,iGrabDelay   ,"Grab Delay(ms)    " , "ī�޶� Grab Delay" );
                PROPERTY(List , int ,iCamId       ,"Camera Id         " , "ī�޶� Index"      );
                PROPERTY(List , int ,iLightId     ,"Light Id          " , "���� Index"        );
                PROPERTY(List , bool,bSaveLastImg ,"Last Img Save(c:) " , "������ �̹��� ����");
                PROPERTY(List , int ,iSaveInspImg ,"Insp Img Save(d:) " , "�˻� �̹��� ����"  ); ViSaveInspImg.SetSelList("NotUse,NgOnly,OkOnly,All");
                //imi ī�޶� �Ѱ��� �޾Ƽ� �ٽ� �����Ѵ�.    �ϴ� ī�޶� ���������� ������ ������ �־ .... �ּ�.
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


    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        CCam  ::TCamUserPara   * CamPara   ;
        CLight::TLightUserPara * LightPara ;

        TRslt Rslt ;
        //TDisp Disp ;

        CLinkedList<CTracker*>   m_lTrckInsp ;
        CLinkedList<TLocalPara*> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.

        //TRAIN�̹���.
        CImage * m_pTrainImg ;

        //�˻� Ȥ�� Grab�ÿ� �̹���.
        CImage * m_pCrntImg ;

        CCalibration Cal ;

        HANDLE m_hGrabEnd ;

    private :
        void OnTrackerChanged(); //Ʈ��Ŀ �ݹ�.

        void CamCallback (int _iCamNo); //ī�޶� �ݹ�..

    public : //ī�޶󿡰Լ� TrainImage �޾ƿ��� �Լ�.=======ī�޶� PKG�� �ִ��Լ�.
        CImage * GetTrainImg(){return m_pTrainImg ;}
        CImage * GetCrntImg (){return m_pCrntImg  ;}
        bool LoadImg(String _sFileName){return m_pCrntImg->LoadBmp(_sFileName.c_str());}//VISN[m_iVisnId]->GetImg()->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
        bool SaveImg(String _sFileName){return m_pCrntImg->SaveBmp(_sFileName.c_str());}//VISN[m_iVisnId]->GetImg()->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;

        CCalibration * GetCal(){return &Cal;}
        bool   Ready();
        //void SetDisp(TDisp _tDisp){Disp = _tDisp;} 
    //==========================================================================

        bool CaptureTrainImg(); //ī�޶� ��Ʈ�� ��� �̹����� Train �̹����� ���縦 �صд�.
        TMasterPara * GetMPara();

    //���� ���� �Լ� ��=========================================================
    public :
        //Ŭ���� ���� ������ �Լ�.
        virtual bool Init();
        virtual bool Close();

        //�˻� �� ���� ����.
        virtual bool   Train            (CImage * _pImg);
        virtual void   RsltClear        (); //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
        virtual bool   GetRslt          ();
        virtual bool   Run              (CImage * _pImg);
        virtual String GetErrMsg        ();

        //ȭ�� ���.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
        virtual bool MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual bool MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void MouseUp(TMouseButton Button,TShiftState Shift );

        //�Ķ���� ���÷���
        virtual void UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);

        //�ε� ���̺�.
        virtual void LoadSave(bool _bLoad , String _sVisnName);
    //==========================================================================
};
#endif


