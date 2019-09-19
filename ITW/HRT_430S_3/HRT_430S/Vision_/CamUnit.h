//---------------------------------------------------------------------------

#ifndef CamUnitH
#define CamUnitH

//#include "VisnDefine.h"


//#include <system.hpp>
#include <vcl.h>
//---------------------------------------------------------------------------
enum EN_CAM_TYPE {
    ctNone = 0 ,
    ctImi  = 1 ,
    ctEur  = 2 ,
};

enum EN_TRG_MODE {
    tmSw   = 0 ,
    tmHw   = 1 ,
    tmLive = 2 ,
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

        //惑加 技飘 全全全....

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
class CImi:public CCam  //Imi 墨皋扼侩侩
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
            int iPhysicalNo  ;

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
        static int m_iMaxCam ;
        static LoadCamCnt();

        TPara      Para ;
        TUserPara  UserPara ;



        unsigned char * m_pImgRGBBuf ;

        static int CALLBACK ImiCallback(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize);
        void       SetBuffer   (unsigned char * pBuffer, int lBufferSize);



    public:




        CImi::CImi(void);
        CImi::~CImi(void);

        int GetMaxCamCnt() {return m_iMaxCam;}


        //惑加技飘 全全全.
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

        void        SetBrightness(int          _iVal   );
        void        SetSharpness (int          _iVal   );
        void        SetGain      (int          _iVal   );
        void        SetShutter   (int          _iVal   );


};


extern CImi ** Imi ;
extern int  g_iMaxImi ;
extern bool InitImi();
extern bool CloseImi();



#endif
