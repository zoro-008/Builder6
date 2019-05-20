//---------------------------------------------------------------------------

#ifndef CamUnitH
#define CamUnitH

//#include <Sytems.hpp>
#include <Dialogs.hpp>
#include <ValEdit.hpp>
#include "BaseDefine.h"
#include "BaseClass.h"
//---------------------------------------------------------------------------
enum ETrgMode {
    tmSw   = 0 ,
    tmHw   = 1 ,
    tmLive = 2 ,
};
enum ECamType {
    ctNone = 0 ,
    ctImi  = 1 
};




typedef void (__closure * CamCallbackFunc)(int);

class CCam
{
    private:

        static int  m_iMaxCam ;

    public:
        static int  m_iMaxImi ;
        static CCam** m_apCam ;

        static int  InitCam();
        static bool CloseCam();

        //상속용.
        struct TCamUserPara {
            TCamUserPara(){
            };
            virtual ~TCamUserPara(){
            };

        };




    protected:
        int  m_iCamNo     ;

        unsigned char * m_pImgBuf       ;
        int             m_iImgWidth     ;
        int             m_iImgHeight    ;
        int             m_iImgBit       ;

        double          m_dGrabOnTime   ;
        double          m_dGrabTime     ;

        CamCallbackFunc m_pCallbackFunc ;
        ETrgMode        m_eTrgMode      ;

        int GetCamNo(){return m_iCamNo ;}
    public:
        CCam::CCam(void){
            static int iNo = 0 ;
            m_iCamNo = iNo;
            iNo++;
            m_iMaxCam = iNo ;
        }
        virtual CCam::~CCam(void){}
        bool GetTrgMode(){return m_eTrgMode;}

        //상속 세트 홍홍홍....

        virtual ECamType GetType         (                      )=0;
        virtual bool     GetImg          (unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt)=0;
        virtual bool     Init            (                      )=0;
        virtual bool     Close           (                      )=0;
        virtual bool     Grab            (                      )=0;
        virtual bool     SetTrgMode      (ETrgMode        _iMode)=0;
        virtual void     SetGrabFunc     (CamCallbackFunc _pFunc)=0;
        virtual bool     ApplyUserPara   (TCamUserPara * _pCamUserPara)=0;
        virtual void     LoadPara        (bool         _bLoad   , String _sPath     )=0;
        virtual void     UpdateUserPara  (bool _bToTable , TValueListEditor * _vePara,TCamUserPara * _pCamUserPara)=0;
        virtual void     LoadSaveUserPara(bool _bLoad , String _sFilePath,TCamUserPara * _pCamUserPara)=0;

        virtual double   GetGrabTime   (                      )=0;
};

#include "NeptuneImport.h"
class CImi:public CCam  //Imi 카메라용용
{
    public:
        static int  m_iMaxCam ;
        static bool InitImi();
        static bool CloseImi();

        static PNEPTUNE_CAM_INFO m_pCamInfo;

    private:
        static bool m_bDllLoaded ;

        int  m_iImiCamNo  ;
        NeptuneCamHandle m_hCamHandle;

    public:
        struct TPara { //한번 로딩 하면 바뀔일이 없는 파라.
            int iPhysicalNo  ;

            //이것들은 카메라 스탑 시키고 바꿀수 있다.
            int iVideoFormat ;
            int iVideoMode   ;
            int iFrameRate   ;
        };

        /*
        struct TUserPara : public CamUserPara{
            int iBrightness ;//
            int iSharpness  ;//
            int iGamma      ;//
            int iExposure   ;
            int iShutter    ;//
            int iGain       ;//
            int iPan        ;
            int iTilt       ;
        };
        */
        struct TUserPara : public TCamUserPara{
            CLinkedList<CValue2*> List ;

            DECLARATION(int,iBrightness);
            DECLARATION(int,iSharpness );
            DECLARATION(int,iGamma     );
                        int iExposure   ;//DECLARATION(int,iExposure  );
            DECLARATION(int,iShutter   );
            DECLARATION(int,iGain      );
                        int iPan        ;//DECLARATION(int,iPan       );
                        int iTilt       ;//DECLARATION(int,iTilt      );

            TUserPara(){
                PROPERTY(List , int,iBrightness,"Brightness" , "Brightness");ViBrightness.SetMinMax(0,1023); //0~1023
                PROPERTY(List , int,iSharpness ,"Sharpness " , "Sharpness ");ViSharpness .SetMinMax(0,1016); //0~1016
                PROPERTY(List , int,iGamma     ,"Gamma     " , "Gamma     ");ViGamma     .SetMinMax(4,25  ); //4~25
               //PROPERTY(List , int,iExposure  ,"Exposure  " , "Exposure  ");ViExposure  .SetMinMax(0,100 ); //0~100
                PROPERTY(List , int,iShutter   ,"Shutter   " , "Shutter   ");ViShutter   .SetMinMax(1,2000); //1~2000  1us~3600sec 인데 좀만쓰자.
                PROPERTY(List , int,iGain      ,"Gain      " , "Gain      ");ViGain      .SetMinMax(0,511 ); //0~511
               //PROPERTY(List , int,iPan       ,"Pan       " , "Pan       ");ViPan       .SetMinMax(0,1600); //0~1600
               //PROPERTY(List , int,iTilt      ,"Tilt      " , "Tilt      ");ViTilt      .SetMinMax(0,1230); //0~1230
                SetDefault();
            }
            void SetDefault(){
                iBrightness = 0   ;//0~1023
                iSharpness  = 0   ;//0~1016
                iGamma      = 4   ;//4~25
                iExposure   = 0   ;//0~100
                iShutter    = 400 ;//1~2000
                iGain       = 0   ;//0~511
                iPan        = 0   ;//0~1600
                iTilt       = 0   ;//0~1230
            }
            virtual ~TUserPara(){

            }
        };





    protected:

        TPara      Para ;
        TUserPara  SetUserPara ; //마지막으로 세팅된 유저파라.

        unsigned char * m_pImgRGBBuf ;

        //멤버변수를 넣지 못하게 되어 있어 Static 선언 카메라가 여러개이고 한번에 그랩하여 들어오면 
        //병목 있을수 있네... pContext가 imi 포인터라서 데이터 바뀔일은 없음.
        static void CALLBACK ImiCallback(PNEPTUNE_IMAGE pImage, void *pContext);//static int CALLBACK ImiCallback(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize);
        void       SetBuffer   (unsigned char * pBuffer, int lBufferSize);

    public:
        CImi(void);
        ~CImi(void);



        //상속세트 홍홍홍.
        ECamType GetType         (                      );
        bool     GetImg          (unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt);
        bool     Init            (                      );
        bool     Close           (                      );
        bool     Grab            (                      );
        bool     SetTrgMode      (ETrgMode        _iMode);
        void     SetGrabFunc     (CamCallbackFunc _pFunc);
        bool     ApplyUserPara   (TCamUserPara * _pCamUserPara);
        void     LoadPara        (bool         _bLoad   , String _sPath     );
        void     UpdateUserPara  (bool _bToTable , TValueListEditor * _vePara,TCamUserPara * _pCamUserPara);
        void     LoadSaveUserPara(bool _bLoad  , String _sFilePath,TCamUserPara * _pCamUserPara);
        double   GetGrabTime     (                      );


};









#endif








        //요것들 4개는 일단 쓰지 말자...
        //void        SetBrightness(int          _iVal   );
        //void        SetSharpness (int          _iVal   );
        //void        SetGain      (int          _iVal   );
        //void        SetShutter   (int          _iVal   );




/*
void CImi::SetExposure (int _iTime )
{
    IMC_FEATURE_PROPERTY	FeatureProp;
    bool  bRet ;
    memset(&FeatureProp, 0, sizeof(IMC_FEATURE_PROPERTY));

    FeatureProp.nValue = _iTime  ;

    bRet = (ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_EXPOSURE   , &FeatureProp) == IMC_SUCCESS) ;

}

void CImi::SetBrightness(int _iVal )
{
    IMC_FEATURE_PROPERTY	FeatureProp;
    bool  bRet ;
    memset(&FeatureProp, 0, sizeof(IMC_FEATURE_PROPERTY));

    FeatureProp.nValue = _iVal  ;

    bRet = (ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_BRIGHTNESS   , &FeatureProp) == IMC_SUCCESS) ;
}

void CImi::SetSharpness (int _iVal )
{
    IMC_FEATURE_PROPERTY	FeatureProp;
    bool  bRet ;
    memset(&FeatureProp, 0, sizeof(IMC_FEATURE_PROPERTY));

    FeatureProp.nValue = _iVal  ;

    bRet = (ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_SHARPNESS   , &FeatureProp) == IMC_SUCCESS) ;
}

void CImi::SetGain      (int _iVal )
{
    IMC_FEATURE_PROPERTY	FeatureProp;
    bool  bRet ;
    memset(&FeatureProp, 0, sizeof(IMC_FEATURE_PROPERTY));

    FeatureProp.nValue = _iVal  ;

    bRet = (ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_GAIN   , &FeatureProp) == IMC_SUCCESS) ;
}

void CImi::SetShutter   (int _iVal )
{
    IMC_FEATURE_PROPERTY	FeatureProp;
    bool  bRet ;
    memset(&FeatureProp, 0, sizeof(IMC_FEATURE_PROPERTY));

    FeatureProp.nValue = _iVal  ;

    bRet = (ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_SHUTTER   , &FeatureProp) == IMC_SUCCESS) ;
}
*/
