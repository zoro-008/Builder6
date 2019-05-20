//---------------------------------------------------------------------------

#ifndef LightUnitH
#define LightUnitH

#include "uRs232c.h"
#include "VisnDefine.h"


#include <vcl.h>
//---------------------------------------------------------------------------
enum EN_LIGHT_TYPE {
    ltNone = 0 ,
    ltDkm  = 1 ,  //措扳.

};

class CLight
{
    private:

    protected:
        int m_iLightNo ;
        EN_LIGHT_TYPE m_iLightType ;
        int GetLightNo(){return m_iLightNo ;}

        TRS232C * Rs232c ;

    public:
        CLight(void){
            static int iNo = 0 ;
            m_iLightNo = iNo;
            iNo++;
            m_iLightType = ltNone;
        }
        virtual ~CLight(void){}

        //惑加 技飘 全全全....
        virtual EN_LIGHT_TYPE GetLightType(                                ){ShowMessage("The Light dosen't have this Function!"); return ltNone ;}
        virtual bool          Init        (                                ){ShowMessage("The Light dosen't have this Function!"); return false  ;}
        virtual bool          Close       (                                ){ShowMessage("The Light dosen't have this Function!"); return false  ;}
        virtual bool          SetBright   (int _iCh , int _iVal            ){ShowMessage("The Light dosen't have this Function!"); return false  ;}
        virtual void          LoadPara    (bool _bLoad , AnsiString _sPath ){ShowMessage("The Light dosen't have this Function!");                }
};




class CDkm:public CLight
{
    private:
        struct TParaUi {
            TPanel * pnBase ;
        };
        TParaUi    ParaUi ;
        void       MakeUI     ();
        void       SetArrange (TControl * _tcControl , int _iCol , int _iRow);

    public:
        struct TPara {
        };

        struct TUserPara {
        };

    protected:
        TPara      Para ;
        TUserPara  UserPara ;


    public:
        CDkm(void);
        ~CDkm(void);

        //惑加技飘 全全全.
        EN_LIGHT_TYPE GetLightType(                                );
        bool          Init        (                                );
        bool          Close       (                                );
        bool          SetBright   (int _iCh    , int _iVal         );
        void          LoadPara    (bool _bLoad , AnsiString _sPath );
};

extern CLight * Light[MAX_LIGHT_ID] ;
#endif
