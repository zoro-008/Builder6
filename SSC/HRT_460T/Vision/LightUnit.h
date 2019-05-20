//---------------------------------------------------------------------------

#ifndef LightUnitH
#define LightUnitH

#include <Dialogs.hpp>
#include <ValEdit.hpp>
#include "BaseDefine.h"
#include "BaseClass.h"

#include "uRs232c.h"




//#include <vcl.h>
//---------------------------------------------------------------------------
enum ELightType {
    ltNone       = 0 ,
    ltDkmSerial  = 1   //대겸.
};


class CLight
{
    private:
        static int m_MaxLight ;

    public:
        static int      m_iMaxDkmSerial ;
        static CLight** m_apLight ;

        static int  InitLight ();
        static bool CloseLight();

        CLight(void){
            static int iNo = 0 ;
            m_iLightNo = iNo;
            iNo++;
        }
        virtual ~CLight(void){}

        //상속용.
        struct TLightUserPara {
            TLightUserPara(){
            };
            virtual ~TLightUserPara(){
            };
        };

    protected:
        int m_iLightNo ;

    public:

        //상속 세트 홍홍홍....
        virtual ELightType GetType         (                                                                        )=0;
        virtual bool       Init            (                                                                        )=0;
        virtual bool       Close           (                                                                        )=0;
        virtual bool       ApplyUserPara   (TLightUserPara * _pCamUserPara                                            )=0;
        virtual void       LoadPara        (bool         _bLoad   , String _sPath                                   )=0;
        virtual void       UpdateUserPara  (bool _bToTable , TValueListEditor * _vePara,TLightUserPara * _pCamUserPara)=0;
        virtual void       LoadSaveUserPara(bool _bLoad  , String _sFilePath,TLightUserPara * _pCamUserPara           )=0;
};

//2015.05.19 대겸 벌크용 3채널 광량은 Duration 을 조절.
class CDkmSerial:public CLight
{
    private:
        TRS232C * Rs232c ;

    public:
        static bool InitDkmSerial();
        static bool CloseDkmSerial();

        CDkmSerial(void);
        ~CDkmSerial(void);


        struct TPara { //한번 로딩 하면 바뀔일이 없는 파라.
            int iSerialId  ;
            int iPortNo    ;
            int iBaudRate  ;
            int iByteSize  ;
            int iStopBits  ;
            int iParity    ;
        };

        struct TUserPara : public TLightUserPara{
            CLinkedList<CValue2*> List ;

            DECLARATION(int,iCh1);
            DECLARATION(int,iCh2);
            DECLARATION(int,iCh3);

            TUserPara(){
                PROPERTY(List , int,iCh1,"Ch1 Brightness" , "Ch1 Brightness"); ViCh1.SetMinMax(0,255);
                PROPERTY(List , int,iCh2,"Ch2 Brightness" , "Ch2 Brightness"); ViCh2.SetMinMax(0,255);
                PROPERTY(List , int,iCh3,"Ch3 Brightness" , "Ch3 Brightness"); ViCh3.SetMinMax(0,255);
                SetDefault();
            }
            void SetDefault(){
                iCh1 = 0 ;
                iCh2 = 0 ;
                iCh3 = 0 ;
            }
            virtual ~TUserPara(){

            }
        };

    protected:

        TPara      Para ;
        TUserPara  UserPara ;
        TUserPara  SetUserPara ; //마지막으로 세팅된 유저파라.


    public:


        //상속세트 홍홍홍.
        ELightType GetType         (                                                                        );
        bool       Init            (                                                                        );
        bool       Close           (                                                                        );
        bool       ApplyUserPara   (TLightUserPara * _pCamUserPara                                            );
        void       LoadPara        (bool         _bLoad   , String _sPath                                   );
        void       UpdateUserPara  (bool _bToTable , TValueListEditor * _vePara,TLightUserPara * _pCamUserPara);
        void       LoadSaveUserPara(bool _bLoad  , String _sFilePath,TLightUserPara * _pCamUserPara           );





};

/*

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
        struct TLightUserPara {
            TLightUserPara(){
            };
            virtual ~TLightUserPara(){
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
        EN_TRG_MODE     m_iTrgMode      ;

        int GetCamNo(){return m_iCamNo ;}
    public:
        CCam::CCam(void){
            static int iNo = 0 ;
            m_iCamNo = iNo;
            iNo++;
            m_iMaxCam = iNo ;
        }
        virtual CCam::~CCam(void){}
        bool GetTrgMode(){return m_iTrgMode;}

        //상속 세트 홍홍홍....

        virtual EN_CAM_TYPE GetType         (                      )=0;
        virtual bool        GetImg          (unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt)=0;
        virtual bool        Init            (                      )=0;
        virtual bool        Close           (                      )=0;
        virtual bool        Grab            (                      )=0;
        virtual bool        SetTrgMode      (EN_TRG_MODE     _iMode)=0;
        virtual void        SetGrabFunc     (CamCallbackFunc _pFunc)=0;
        virtual bool        ApplyUserPara   (TLightUserPara * _pCamUserPara)=0;
        virtual void        LoadPara        (bool         _bLoad   , String _sPath     )=0;
        virtual void        UpdateUserPara  (bool _bToTable , TValueListEditor * _vePara,TLightUserPara * _pCamUserPara)=0;
        virtual void        LoadSaveUserPara(bool _bLoad , String _sFilePath,TLightUserPara * _pCamUserPara)=0;

        virtual double      GetGrabTime   (                      )=0;
};

#include "ApiImport.h"
class CImi:public CCam  //Imi 카메라용용
{
    public:
        static int  m_iMaxCam ;
        static bool InitImi();
        static bool CloseImi();

    private:
        static bool m_bDllLoaded ;

        int  m_iImiCamNo  ;

    public:
        struct TPara { //한번 로딩 하면 바뀔일이 없는 파라.
            int iPhysicalNo  ;

            //이것들은 카메라 스탑 시키고 바꿀수 있다.
            int iVideoFormat ;
            int iVideoMode   ;
            int iFrameRate   ;
        };


        struct TUserPara : public TLightUserPara{
            CLinkedList<CValue2*> List ;

            DECLARATION(int,iBrightness);
            DECLARATION(int,iSharpness );
            DECLARATION(int,iGamma     );
            DECLARATION(int,iExposure  );
            DECLARATION(int,iShutter   );
            DECLARATION(int,iGain      );
            DECLARATION(int,iPan       );
            DECLARATION(int,iTilt      );

            TUserPara(){
                PROPERTY(List , int,iBrightness,"Brightness" , "Brightness");
                PROPERTY(List , int,iSharpness ,"Sharpness " , "Sharpness ");
                PROPERTY(List , int,iGamma     ,"Gamma     " , "Gamma     ");
                PROPERTY(List , int,iExposure  ,"Exposure  " , "Exposure  ");
                PROPERTY(List , int,iShutter   ,"Shutter   " , "Shutter   ");
                PROPERTY(List , int,iGain      ,"Gain      " , "Gain      ");
                PROPERTY(List , int,iPan       ,"Pan       " , "Pan       ");
                PROPERTY(List , int,iTilt      ,"Tilt      " , "Tilt      ");
            }
            void SetDefault(){
                iBrightness = 0   ;
                iSharpness  = 0   ;
                iGamma      = 0   ;
                iExposure   = 0   ;
                iShutter    = 500 ;
                iGain       = 0   ;
                iPan        = 0   ;
                iTilt       = 0   ;
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
        static int CALLBACK ImiCallback(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize);
        void       SetBuffer   (unsigned char * pBuffer, int lBufferSize);

    public:
        CImi(void);
        ~CImi(void);

        //상속세트 홍홍홍.
        EN_CAM_TYPE GetType         (                      );
        bool        GetImg          (unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt);
        bool        Init            (                      );
        bool        Close           (                      );
        bool        Grab            (                      );
        bool        SetTrgMode      (EN_TRG_MODE     _iMode);
        void        SetGrabFunc     (CamCallbackFunc _pFunc);
        bool        ApplyUserPara   (TLightUserPara * _pCamUserPara);
        void        LoadPara        (bool         _bLoad   , String _sPath     );
        void        UpdateUserPara  (bool _bToTable , TValueListEditor * _vePara,TLightUserPara * _pCamUserPara);
        void        LoadSaveUserPara(bool _bLoad  , String _sFilePath,TLightUserPara * _pCamUserPara);
        double      GetGrabTime     (                      );


};
*/

#endif
