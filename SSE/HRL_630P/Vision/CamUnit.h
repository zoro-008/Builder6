//---------------------------------------------------------------------------

#ifndef CamUnitH
#define CamUnitH
#include <vcl.h>


#include "Common.h"



//---------------------------------------------------------------------------
enum EN_CAM_TYPE {
    ctNone = 0 ,
    ctImi  = 1 ,
    ctEur  = 2
};

enum EN_TRG_MODE {
    tmSw   = 0 ,
    tmHw   = 1 ,
    tmLive = 2
};


typedef void (__closure * CamCallbackFunc)(int);
class CCam
{
    private:

    protected:
        int  m_iCamNo     ;
        bool m_bInitialed ;
        EN_CAM_TYPE  m_iCamType  ;


        unsigned char * m_pImgBuf       ;
        int             m_iImgWidth     ;
        int             m_iImgHeight    ;
        int             m_iImgBit       ;

        float           m_fGrabOnTime   ;
        float           m_fGrabTime     ;

        CamCallbackFunc m_pCallbackFunc ;
        EN_TRG_MODE     m_iTrgMode      ;

        int GetCamNo(){return m_iCamNo ;}
    public:
        CCam::CCam(void){
            static int iNo = 0 ;
            m_iCamNo = iNo;
            iNo++;
            m_iCamType = ctNone;
        }
        virtual CCam::~CCam(void){}
        bool GetTrgMode(){return m_iTrgMode;}

        //��� ��Ʈ ȫȫȫ....
        virtual bool        GetImg      (unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt){return false;}
        virtual EN_CAM_TYPE GetCamType  (                      ){ShowMessage("The camera dosen't have this Function!"); return ctNone ;}
        virtual bool        Init        (                      ){ShowMessage("The camera dosen't have this Function!"); return false  ;}
        virtual bool        Close       (                      ){ShowMessage("The camera dosen't have this Function!"); return false  ;}
        virtual bool        Grab        (                      ){ShowMessage("The camera dosen't have this Function!"); return false  ;}
        virtual bool        SetTrgMode  (EN_TRG_MODE     _iMode){ShowMessage("The camera dosen't have this Function!"); return false  ;}
        virtual void        SetGrabFunc (CamCallbackFunc _pFunc){ShowMessage("The camera dosen't have this Function!"); return ;       }
        virtual bool        ShowParaForm(TPanel *       _pnBase){ShowMessage("The camera dosen't have this Function!"); return false  ;}
        virtual bool        ApplyPara   (                      ){ShowMessage("The camera dosen't have this Function!"); return false  ;}
        virtual void        UpdatePara  (bool         _bToTable){ShowMessage("The camera dosen't have this Function!");                }
        virtual void        LoadPara    (bool         _bLoad   , AnsiString _sPath     ){
                                                                 ShowMessage("The camera dosen't have this Function!");                }
        virtual void        SetExposure (int          _iTime   ){ShowMessage("The camera dosen't have this Function!");                }
        virtual float       GetGrabTime (                      ){ShowMessage("The camera dosen't have this Function!"); return 0.0    ;}
};

#include "ApiImport.h"
class CImi:public CCam  //Imi ī�޶���
{
    private:
        bool m_bDllLoaded ;
        struct TParaUi {
            TPanel * pnBase ;

            TLabel * lbVideoFormat ; TComboBox * cbVideoFormat ;
            TLabel * lbVideoMode   ; TComboBox * cbVideoMode   ;
            TLabel * lbFrameRate   ; TComboBox * cbFrameRate   ;

            TLabel * lbBrightness  ; TEdit     * edBrightness  ;
            TLabel * lbSharpness   ; TEdit     * edSharpness   ;
            TLabel * lbGamma       ; TEdit     * edGamma       ;
            TLabel * lbExposure    ; TEdit     * edExposure    ;
            TLabel * lbShutter     ; TEdit     * edShutter     ;
            TLabel * lbGain        ; TEdit     * edGain        ;
            TLabel * lbPan         ; TEdit     * edPan         ;
            TLabel * lbTilt        ; TEdit     * edTilt        ;
        };
        TParaUi    ParaUi ;
        void       MakeUI     ();
        void       SetArrange (TControl * _tcControl , int _iCol , int _iRow);

        int   m_iImiCamNo  ;

    public:


        struct TPara {
            int iVideoFormat ;
            int iVideoMode   ;
            int iFrameRate   ;
        };

        struct TUserPara {
            int iBrightness ;
            int iSharpness  ;
            int iGamma      ;
            int iExposure   ;
            int iShutter    ;
            int iGain       ;
            int iPan        ;
            int iTilt       ;
        };

    protected:
        TPara      Para ;
        TUserPara  UserPara ;

        unsigned char * m_pImgRGBBuf ;

        static int CALLBACK ImiCallback(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize);
        void       SetBuffer   (unsigned char * pBuffer, int lBufferSize);

    public:
        CImi::CImi(void);
        CImi::~CImi(void);




        //��Ӽ�Ʈ ȫȫȫ.
        bool        GetImg      (unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt);
        EN_CAM_TYPE GetCamType  (                      ){return ctImi;}
        bool        Init        (                      );
        bool        Close       (                      );
        bool        Grab        (                      );
        bool        SetTrgMode  (EN_TRG_MODE     _iMode);
        void        SetGrabFunc (CamCallbackFunc _pFunc);
        bool        ShowParaForm(TPanel *       _pnBase);
        bool        ApplyPara   (                      );
        void        UpdatePara  (bool         _bToTable);
        void        LoadPara    (bool         _bLoad   , AnsiString _sPath     );
        void        SetExposure (int          _iTime   );
        float       GetGrabTime (                      );
};


#include "multicam.h"
class CEur:public CCam //�����ý� ����̽������
{
    private:
        struct TParaUi {
            TPanel * pnBase ;

            TLabel * lbStrobeDelay   ; TEdit * edStrobeDelay   ;
            TLabel * lbTriggerDelay  ; TEdit * edTriggerDelay  ;
            TLabel * lbShutter       ; TEdit * edShutter       ;
            TLabel * lbExposure      ; TEdit * edExposure      ;
            TLabel * lbCamFile       ; TEdit * edCamFile       ;
            TLabel * lbConnector     ; TEdit * edConnector     ;
        };
        TParaUi    ParaUi ;
        void       MakeUI     ();
        void       SetArrange (TControl * _tcControl , int _iCol , int _iRow);

        unsigned int m_iEurCamNo  ; //ī�޶� �ε���.
        unsigned int m_hChanel    ; //ī�޶� ä�� �ڵ�.

        int m_iSizeX ; //�̹��� ������
        int m_iSizeY ;


    public:


        struct TPara {
            String sCamFile         ;
            String sConnector       ;
        };

        struct TUserPara {
            int    iStrobeDelay     ;
            int    iTriggerDelay    ;
            int    iExposure        ;

            //int iSharpness       ;
            //int iBrightness      ;
            //int ExternalTrigger  ;
            //int InternalTrigger  ;
            //int ImmediateTrigger ;
        };

    protected:
        TPara      Para ;
        TUserPara  UserPara ;

        unsigned char * m_pImgRGBBuf ;

        static void WINAPI EureCallback (PMCSIGNALINFO SigInfo);
        //void WINAPI EureCallback (PMCSIGNALINFO SigInfo);
        void        SetBuffer   (unsigned char * pBuffer, int lBufferSize);

    public:
        CEur::CEur(void);
        CEur::~CEur(void);


        //��Ӽ�Ʈ ȫȫȫ.
        bool        GetImg      (unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt);
        EN_CAM_TYPE GetCamType  (                      ){return ctImi;}
        bool        Init        (                      );
        bool        Close       (                      );
        bool        Grab        (                      );
        bool        SetTrgMode  (EN_TRG_MODE     _iMode);
        void        SetGrabFunc (CamCallbackFunc _pFunc);
        bool        ShowParaForm(TPanel *       _pnBase);
        bool        ApplyPara   (                      );
        void        UpdatePara  (bool         _bToTable);
        void        LoadPara    (bool         _bLoad   , AnsiString _sPath     );
        void        SetExposure (int          _iTime   );
        float       GetGrabTime (                      );
};

extern CCam * Cam[MAX_CAM_ID] ;
//void InitCam (EN_CAM_TYPE * _iCamType);
//void CloseCam();


#endif
