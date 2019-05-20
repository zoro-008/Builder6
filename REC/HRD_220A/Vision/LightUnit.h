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
    ltDkmSerial  = 1   //���.
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

        //��ӿ�.
        struct TLightUserPara {
            TLightUserPara(){
            };
            virtual ~TLightUserPara(){
            };
        };

    protected:
        int m_iLightNo ;

    public:

        //��� ��Ʈ ȫȫȫ....
        virtual ELightType GetType         (                                                                        )=0;
        virtual bool       Init            (                                                                        )=0;
        virtual bool       Close           (                                                                        )=0;
        virtual bool       ApplyUserPara   (TLightUserPara * _pCamUserPara                                            )=0;
        virtual void       LoadPara        (bool         _bLoad   , String _sPath                                   )=0;
        virtual void       UpdateUserPara  (bool _bToTable , TValueListEditor * _vePara,TLightUserPara * _pCamUserPara)=0;
        virtual void       LoadSaveUserPara(bool _bLoad  , String _sFilePath,TLightUserPara * _pCamUserPara           )=0;
};

//2015.05.19 ��� ��ũ�� 3ä�� ������ Duration �� ����.
class CDkmSerial:public CLight
{
    private:
        TRS232C * Rs232c ;

    public:
        static bool InitDkmSerial();
        static bool CloseDkmSerial();

        CDkmSerial(void);
        ~CDkmSerial(void);


        struct TPara { //�ѹ� �ε� �ϸ� �ٲ����� ���� �Ķ�.
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
        TUserPara  SetUserPara ; //���������� ���õ� �����Ķ�.


    public:


        //��Ӽ�Ʈ ȫȫȫ.
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

        //��ӿ�.
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

        //��� ��Ʈ ȫȫȫ....

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
class CImi:public CCam  //Imi ī�޶���
{
    public:
        static int  m_iMaxCam ;
        static bool InitImi();
        static bool CloseImi();

    private:
        static bool m_bDllLoaded ;

        int  m_iImiCamNo  ;

    public:
        struct TPara { //�ѹ� �ε� �ϸ� �ٲ����� ���� �Ķ�.
            int iPhysicalNo  ;

            //�̰͵��� ī�޶� ��ž ��Ű�� �ٲܼ� �ִ�.
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
        TUserPara  SetUserPara ; //���������� ���õ� �����Ķ�.
        unsigned char * m_pImgRGBBuf ;

        //��������� ���� ���ϰ� �Ǿ� �־� Static ���� ī�޶� �������̰� �ѹ��� �׷��Ͽ� ������ 
        //���� ������ �ֳ�... pContext�� imi �����Ͷ� ������ �ٲ����� ����.
        static int CALLBACK ImiCallback(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize);
        void       SetBuffer   (unsigned char * pBuffer, int lBufferSize);

    public:
        CImi(void);
        ~CImi(void);

        //��Ӽ�Ʈ ȫȫȫ.
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
