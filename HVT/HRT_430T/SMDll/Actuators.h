//---------------------------------------------------------------------------
#ifndef ActuatorsH
#define ActuatorsH

//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <Buttons.hpp>
#include <stdio.h>
#include <Io.h>
//---------------------------------------------------------------------------
#include "SMDllDefine.h"
#include "Timer.h"
#include "IOs.h"
#include "TEdit.h"
enum { ccBwd = 0 , ccFwd = 1};
enum EN_ACTR_DIRECTION_TYPE {
    adLR = 0 , //정면에서   봤을때 Left 가 - Right가 +
    adRL     , //정면에서   봤을때 Right가 - Left 가 +
    adBF     , //정면에서   봤을때 Bwd  가 - Fwd  가 +
    adFB     , //정면에서   봤을때 Fwd  가 - Bwd  가 +
    adUD     , //정면에서   봤을때 Up   가 - Down 가 +
    adDU     , //정면에서   봤을때 Down 가 - Up   가 +
    adCA     , //회전축에서 봤을때 Clock가 - AntiC가 +
    adAC       //회전축에서 봤을때 AntiC가 - Clock가 +
};
//---------------------------------------------------------------------------
class CActuator {
    public :

        //Creator & Destroyer.
        CActuator(int _iActrNo);
        ~CActuator();

        struct SPara {
            AnsiString sName       ;
            AnsiString sComment    ;
            int        iFwdXAdd    ;
            int        iBwdXAdd    ;
            int        iFwdYAdd    ;
            int        iBwdYAdd    ;
            int        iFwdOnDelay ;
            int        iBwdOnDelay ;
            int        iFwdTimeOut ;
            int        iBwdTimeOut ;
            int        iDirType    ;
        };

        struct SStat {
            bool  bActPos ;
            bool  bCmdPos ;
            bool  bErr    ;
        };

    protected:
        int m_iActrNo;

        void Init();

        //Timer.
        CDelayTimer m_dtFwdTimeOut;
        CDelayTimer m_dtBwdTimeOut;
        CDelayTimer m_dtFwdOnDelay;
        CDelayTimer m_dtBwdOnDelay;

        SPara Para ;
        SStat Stat ;


    public :
        void Update ();

        //basic funcions.
        void       Reset    (void      );
        AnsiString GetName  (void      );
        bool       Complete (bool _bCmd);
        bool       Complete (          );
        bool       GetCmd   (void      );
        void       SetAct   (bool _bCmd);
        bool       GetAct   (void      );
        bool       Err      (void      );
        bool       Move     (bool _bCmd);

        CActuator::SPara * GetPara();

        //Read/Write Para.
        void Load(EN_LAN_SEL _iLang);
        void Save(EN_LAN_SEL _iLang);

    //UI Interface.
    protected :
        Graphics::TBitmap *pLtimg ; //Left
        Graphics::TBitmap *pRtimg ; //Right
        Graphics::TBitmap *pUpimg ; //Up
        Graphics::TBitmap *pDnimg ; //Down
        Graphics::TBitmap *pAwimg ; //AntiClockWise
        Graphics::TBitmap *pCwimg ; //ClockWise

        //SetWindow
        TStaticText  *m_pStCaption;
        TBitBtn      *m_pBtnBw    ;
        TBitBtn      *m_pBtnFw    ;

        bool   m_bSetCntrlPn ;

    public :

        void SetCntrlPn       ( TPanel * _pnBase );
        void DelCntrlPn       (                  );

        void SetBtnImg (Graphics::TBitmap *bmLtimg ,
                        Graphics::TBitmap *bmRtimg ,
                        Graphics::TBitmap *bmUpimg ,
                        Graphics::TBitmap *bmDnimg ,
                        Graphics::TBitmap *bmAwimg ,
                        Graphics::TBitmap *bmCwimg );


        //Event.
        void __fastcall btBwd(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btFwd(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

};

//-------------------------- CActuatorTable -------------------------
class CActuators
{
    public :
        CActuators(void);
        ~CActuators(void);

        struct SRptPara {
            int  iDelay    ;
            int  iFrstActr ;
            int  iScndActr ;
        };

        struct SPara {
            AnsiString sName       ;
            AnsiString sComment    ;
            int        iFwdXAdd    ;
            int        iBwdXAdd    ;
            int        iFwdYAdd    ;
            int        iBwdYAdd    ;
            int        iFwdOnDelay ;
            int        iBwdOnDelay ;
            int        iFwdTimeOut ;
            int        iBwdTimeOut ;

        };

    protected :
        CActuator ** Actuator ;

        EN_LAN_SEL m_iLangSel  ; //Languge Selection.

        int m_iMaxAct  ;

        bool m_bRpt    ;

        CDelayTimer m_dtRptFwd ;
        CDelayTimer m_dtRptBwd ;

        SRptPara RptPara;

    public :
//       CActuator * Actuators ;
        //Basic functions.

        //basic funcions.
        void       Reset    (void                     );
        AnsiString GetName  (int _iActrNo             );
        bool       Complete (int _iActrNo , bool _bCmd);
        bool       Complete (int _iActrNo             );
        bool       Err      (int _iActrNo             );
        bool       Move     (int _iActrNo , bool _bCmd);
        bool       GetCmd   (int _iActrNo             );
        void       SetAct   (int _iActrNo , bool _bCmd);

        //Repeat Control.
        void       GoRpt   (int _iDelay , int _iActr1No , int _iActr2No = -1);
        void       RptActr (void);
        void       StopRpt (void);


        //Update.
        void Update(void);

        //Read/Write Para.
        void Load();
        void Save();
        void LoadDnmVar();
        void SaveDnmVar();

        int  GetActrCnt(){return m_iMaxAct; }

    //Display.
    public :
        void DisplayList    (TStringGrid *_sgTable);
        void DisplayStatus  (int _iActrNo , TLabel   *_lbFwdStat , TLabel  *_lbBwdStat , TLabel *_lbAlarm);
        void DisplayButton  (int _iActrNo , TButton  *_btBwdStat , TButton *_btFwdStat);
        void DisplayProperty(bool  _bToTble ,
                             TEdit       * _edName , TEdit       * _edComt    ,
                             TEdit       * _edFx   , TEdit       * _edBx      ,
                             TEdit       * _edFy   , TEdit       * _edBy      ,
                             TEdit       * _edFd   , TEdit       * _edBd      ,
                             TEdit       * _edFt   , TEdit       * _edBt      ,
                             TStringGrid *_sgTable , TComboBox   * _cbDirType );

        void SetCntrlPn     (int _iActrNo , TPanel * _pnBase );
        void DelCntrlPn     (int _iActrNo                    );

        //Event.
        //void __fastcall btBwd(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        //void __fastcall btFwd(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);


        void SetBtnImg (Graphics::TBitmap *bmLtimg ,
                        Graphics::TBitmap *bmRtimg ,
                        Graphics::TBitmap *bmUpimg ,
                        Graphics::TBitmap *bmDnimg ,
                        Graphics::TBitmap *bmAwimg ,
                        Graphics::TBitmap *bmCwimg );
};

//Actuator Object.
extern CActuators AT;

//---------------------------------------------------------------------------
#endif


