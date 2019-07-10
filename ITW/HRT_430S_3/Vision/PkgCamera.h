//---------------------------------------------------------------------------

#ifndef PkgCameraH
#define PkgCameraH

#include "BasePkg.h"

#include "UnitArea.h"
#include "UnitImage.h"

#include "UnitGdi.h"

#include "CamUnit.h"
//---------------------------------------------------------------------------
//20150227 ����� ����.
class CCamera_V01 : public CPkgBase
{
    public  :
        //static String GetClassName(String & _sClassName){ CCamera_V01;} ���߿� ����..

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


    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt Rslt ;
        //TDisp Disp ;

        CLinkedList<TOutBandRectTracker*> m_lTrckInsp ;
        CLinkedList<TLocalPara         *> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.

        String m_sCrntJobFilePath ;

        //ī�޶�
        //CCam * m_pCam ;   //ī�޶� �ε����� �ٲ�� �ȴ�.;;;;;
        //������ �ٲ� ���� new delete �Ǽ� ���̺��� ����� �ε����� ��������.

        //TRAIN�̹���.
        CImage * m_pTrainImg ;

        //�˻� Ȥ�� Grab�ÿ� �̹���.
        CImage * m_pCrntImg ;

        HANDLE m_hGrabEnd ;

    private :
        void OnTrackerChanged(); //Ʈ��Ŀ �ݹ�.

        //����Ʈ ��Ʈ�ѷ� �޸��� ���⿡ �߰� �ؾ���.
        //

        //bool m_bGrabEnd ;
        void CamCallback (int _iCamNo); //ī�޶� �ݹ�..

    public : //ī�޶󿡰Լ� TrainImage �޾ƿ��� �Լ�.=======ī�޶� PKG�� �ִ��Լ�.
        CImage * GetTrainImg(){return m_pTrainImg ;}
        CImage * GetCrntImg (){return m_pCrntImg  ;}
        bool LoadImg(String _sFileName){return m_pCrntImg->LoadBmp(_sFileName.c_str());}//VISN[m_iVisnId]->GetImg()->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
        bool SaveImg(String _sFileName){return m_pCrntImg->SaveBmp(_sFileName.c_str());}//VISN[m_iVisnId]->GetImg()->SaveBmp(SavePictureDialog1 -> FileName.c_str()) ;
        //void SetDisp(TDisp _tDisp){Disp = _tDisp;} 
    //==========================================================================

        bool CaptureTrainImg(); //ī�޶� ��Ʈ�� ��� �̹����� Train �̹����� ���縦 �صд�.
    //���� ���� �Լ� ��=========================================================
    public :
        //Ŭ���� ���� ������ �Լ�.
        virtual String GetPkgName (){return "CCamera_V01";} //�̰Ż�ӹ����� �������Ѵ�.
        virtual bool Init();
        virtual bool Close();

        //�˻� �� ���� ����.
        virtual bool   GetTrainAvailable();
        virtual bool   Train            (CImage * _pImg);
        virtual void   RsltClear        (); //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
        virtual bool   GetRslt          ();
        virtual bool   Run              (CImage * _pImg);
        virtual String GetErrMsg        ();

        //ȭ�� ���.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
        virtual TTracker * MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual TTracker * MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void       MouseUp(TMouseButton Button,TShiftState Shift );

        //�Ķ���� ���÷���
        virtual void UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);

        //�ε� ���̺�.
        virtual void LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName);
    //==========================================================================
};
#endif

