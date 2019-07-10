//---------------------------------------------------------------------------

#ifndef TowerLampH
#define TowerLampH
#include <Grids.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include "Timer.h"
#include "TEdit.h"

#include "SMDllDefine.h"
//---------------------------------------------------------------------------

enum EN_LAMP_OPER {
    loLampOff   = 0 ,
    loLampOn        ,
    loLampFlick     ,

    MAX_LAMP_OPER
};

enum EN_BUZZ_OPER {
    boBuzOff  = 0 ,
    boBuzOn       ,

    MAX_BUZZ_OPER
};

typedef struct _LAMP_INFO {
    EN_LAMP_OPER  iRed    ;
    EN_LAMP_OPER  iYel    ;
    EN_LAMP_OPER  iGrn    ;
    EN_BUZZ_OPER  iBuzz   ;

} LAMP_INFO ;

typedef struct _LAMP_OUTPUT_ADD {
    int iRedAdd ;
    int iYelAdd ;
    int iGrnAdd ;
    int iSndAdd ;
} LAMP_OUTPUT_ADD ;

//---------------------------------------------------------------------------
class CTowerLamp
{
    private:
        CDelayTimer  m_FlickOnTimer  ;
        CDelayTimer  m_FlickOffTimer ;

        EN_SEQ_STAT       m_iSeqStat                ;

        LAMP_INFO         m_LampInfo [MAX_SEQ_STAT] ;
        LAMP_OUTPUT_ADD   m_LampAdd                 ;
        bool              m_bBuzzOff                ;
        bool              m_bTestLampBuzz           ;
        bool              m_bFlick                  ;
        EN_SEQ_STAT       m_iTestStat               ;

    public:
        __fastcall  CTowerLamp(void) {Init(); }
        __fastcall ~CTowerLamp(void) { }

        void __fastcall Init(void);

        __property bool         _bBuzzOff      = { read = m_bBuzzOff      , write = m_bBuzzOff      };
        __property bool         _bTestLampBuzz = { read = m_bTestLampBuzz , write = m_bTestLampBuzz };
        __property bool         _bFlick        = { read = m_bFlick        , write = m_bFlick        };
        __property EN_SEQ_STAT  _iTestStat     = { read = m_iTestStat     , write = m_iTestStat     };

        //Lamp
        void __fastcall Update  (EN_SEQ_STAT  Stat );

        //Read/Write Para.
        void __fastcall Load    (bool IsLoad       );

        void __fastcall DisplayList    (TStringGrid *Table );
        void __fastcall DisplayStatus  (TLabel * Red  , TLabel * Yel  , TLabel * Grn  , TPanel * Snd);

        void __fastcall DisplayProperty(int ToTble , TRadioButton *RedOn , TRadioButton *RedOff , TRadioButton *RedFlick ,
                                                     TRadioButton *YelOn , TRadioButton *YelOff , TRadioButton *YelFlick ,
                                                     TRadioButton *GrnOn , TRadioButton *GrnOff , TRadioButton *GrnFlick ,
                                                     TRadioButton *BuzOn , TRadioButton *BuzOff ,
                                                     TEdit        *RedAdd, TEdit        *YelAdd , TEdit        *GrnAdd   , TEdit        *SndAdd ,
                                                     TStringGrid  *Table );

        void __fastcall SetTestMode    (bool IsTest) { m_bTestLampBuzz = IsTest; }

};
extern CTowerLamp TL;
//---------------------------------------------------------------------------
#endif


