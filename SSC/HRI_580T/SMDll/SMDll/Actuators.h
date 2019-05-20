//---------------------------------------------------------------------------
#ifndef ActuatorsH
#define ActuatorsH

//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <stdio.h>
#include <Io.h>
//---------------------------------------------------------------------------
#include "Timer.h"
#include "IOs.h"
#include "TEdit.h"
enum { ccBwd = 0 , ccFwd = 1};
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
        bool       GetAct   (void      );
        bool       Err      (void      );
        bool       Move     (bool _bCmd);

        CActuator::SPara * GetPara();

        //Read/Write Para.
        void Load(void);
        void Save(void);
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

        //Repeat Control.
        void       GoRpt   (int _iDelay , int _iActr1No , int _iActr2No = -1);
        bool       RptActr (void);
        bool       StopRpt (void);


        //Update.
        void Update(void);

        //Read/Write Para.
        void Load();
        void Save();
        void LoadDnmVar();
        void SaveDnmVar();

        //Display.
        void DisplayList    (TStringGrid *_sgTable);
        void DisplayStatus  (int _iActrNo , TLabel   *_lbFwdStat , TLabel  *_lbBwdStat , TLabel *_lbAlarm);
        void DisplayButton  (int _iActrNo , TButton  *_btBwdStat , TButton *_btFwdStat);
        void DisplayProperty(bool  _bToTble ,
                             TEdit * _edName , TEdit * _edComt ,
                             TEdit * _edFx   , TEdit * _edBx   ,
                             TEdit * _edFy   , TEdit * _edBy   ,
                             TEdit * _edFd   , TEdit * _edBd   ,
                             TEdit * _edFt   , TEdit * _edBt   ,
                             TStringGrid *_sgTable );
};

//Actuator Object.
extern CActuators AT;

//---------------------------------------------------------------------------
#endif


