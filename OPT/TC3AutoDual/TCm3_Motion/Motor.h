//---------------------------------------------------------------------------
#ifndef MotorH
#define MotorH

//---------------------------------------------------------------------------
#include "Axt_FS20.h"


#define _CHECK_RANGE         if( _iAxisNo < 0 || _iAxisNo > m_iMaxAxtFS20)
#define _CHECK_RETURN_VOID   _CHECK_RANGE return       ;
#define _CHECK_RETURN_DOUBLE _CHECK_RANGE return 0.0   ;
#define _CHECK_RETURN_BOOL   _CHECK_RANGE return false ;


//---------------------------------------------------------------------------
class CMotors
{
    protected:
        CAxtFS20 * AxtFS20 ; int m_iMaxAxtFS20 ;

    public:

        __fastcall  CMotors(void) {  }
        __fastcall ~CMotors(void) {  }

        void Init (void );

        //Status.
        void Reset           (int _iAxisNo                     );
        void ResetAll        (void                             );
        void SetServo        (int _iAxisNo , bool   _bOn       ); //Each Servo _bOn/Off.
        void SetServoAll     (               bool   _bOn       ); //All  Servo _bOn/Off
        void ClearPos        (int _iAxisNo , double _dPos = 0.0); //Clear each axis position.
        void ClearPosAll     (               double _dPos = 0.0); //Clear all  asis position.
        void SetPos          (int _iAxisNo , double _dPos = 0.0); //Set Any Position.
        void SetHomeEnd      (int _iAxisNo , bool   _bOn       ); //Set HomeEnd Position.
        void ClearHomeEnd    (int _iAxisNo                     ); //Kill the each home end flag.
        void ClearHomeEndAll (void                             ); //Kill the all  home end flag.

        bool IsAllServoOn    (void                             ); //Is all servo off?
        bool IsAllHomeEnd    (void                             ); //Is all home  end?

        bool GetAlarm    (int _iAxisNo){ return CAMC[_iAxisNo].GetAlarm  (); }
        bool GetCCW      (int _iAxisNo){ return CAMC[_iAxisNo].GetCCW    (); }
        bool GetCW       (int _iAxisNo){ return CAMC[_iAxisNo].GetCW     (); }
        bool GetHomeEnd  (int _iAxisNo){ return CAMC[_iAxisNo].GetHomeEnd(); }
        bool GetHome     (int _iAxisNo){ return CAMC[_iAxisNo].GetHome   (); }
        bool GetServo    (int _iAxisNo){ return CAMC[_iAxisNo].GetServo  (); }

        bool CmprPos     (int _iAxisNo , double _dPos  ) ;

        //Motor Position.
        double GetCmdPos (int _iAxisNo , ) { if (_iAxisNo < 0) return 0; return CAMC[_iAxisNo].GetCmdPos(); }
        double GetTrgPos (int _iAxisNo , ) { if (_iAxisNo < 0) return 0; return CAMC[_iAxisNo].GetTrgPos(); }
        double GetEncPos (int _iAxisNo , ) { if (_iAxisNo < 0) return 0; return CAMC[_iAxisNo].GetEncPos(); }

        //Motion Stop
        bool EmrgStop     (int _iAxisNo ); //지정 비상 정지.
        void EmrgStopAll  (void         ); //전제 비상 정지.
        bool Stop         (int _iAxisNo ); //지정 모터 감속 정지.
        void StopAll      (void         ); //전제 모터 감속 정지.

        //Check DSTB Each Axis.
        bool CheckDstb    (int _iAxisNo );

        //MoveMotor
        bool MoveHome     (int _iAxisNo );
        bool MoveMotr     (int _iAxisNo , double _dPos , bool _bInc = false);
        bool MoveJogP     (int _iAxisNo );
        bool MoveJogN     (int _iAxisNo );


        bool MotnDone     (int _iAxisNo );

        //Update.
        void Update();

        void UpdateStat(int _iAxisNo ,                             TLabel *S1 , TLabel *S2 , TLabel *S3 , TLabel *S4 , TLabel *S5                                       );
        void SelStat   (int _iAxisNo , TLabel *Cmd , TLabel *Enc , TLabel *S1 , TLabel *S2 , TLabel *S3 , TLabel *S4 , TLabel *S5 , TLabel *S6 , TLabel *S7 , TLabel *S8);
        void MtrStatus (int _iAxisNo ,               TLabel *Enc , TLabel *S1 , TLabel *S2 , TLabel *S3 , TLabel *S4 , TLabel *S5 , TLabel *S6                          );
        void MtrStatus (int _iAxisNo ,               TLabel *Enc , TLabel *Sv , TLabel *Ar , TLabel *Cw , TLabel *Cc , TLabel *He                                       );

        void LoadPara (bool IsLoad,  int iAxis           );
        void LoadAxis (bool IsLoad, AnsiString FolderName);
        void LoadDstb (bool IsLoad                       );


        void GetPara(int _iAxisNo , CAxtFS20::TPara &para) {CAMC[_iAxisNo].GetPara(para);};
        void GetStat(int _iAxisNo , CAxtFS20::TStat &stat) {CAMC[_iAxisNo].GetStat(stat);};
        void GetLimt(int _iAxisNo , CAxtFS20::TLimt &limt) {CAMC[_iAxisNo].GetLimt(limt);};
        void SetPara(int _iAxisNo , CAxtFS20::TPara &para) {CAMC[_iAxisNo].SetPara(para);};
        void SetStat(int _iAxisNo , CAxtFS20::TStat &stat) {CAMC[_iAxisNo].SetStat(stat);};
        void SetLimt(int _iAxisNo , CAxtFS20::TLimt &limt) {CAMC[_iAxisNo].SetLimt(limt);};

        //Parameter Memory Handling Fuctions.
        void SetAxtPara  (int _iAxisNo , CAxtFS20::CAxtPara &_AxtPara ){SetAxtPara (CAxtFS20::CAxtPara &_AxtPara )};
        void GetAxtPara  (int _iAxisNo , CAxtFS20::CAxtPara &_AxtPara ){GetAxtPara (CAxtFS20::CAxtPara &_AxtPara )};

        void SetPara     (int _iAxisNo , CAxtFS20::CPara    &_Para    ){SetPara    (CAxtFS20::CPara    &_Para    )};
        void GetPara     (int _iAxisNo , CAxtFS20::CPara    &_Para    ){GetPara    (CAxtFS20::CPara    &_Para    )};

        void SetUsrPara  (int _iAxisNo , CAxtFS20::CUsrPara &_UsrPara ){SetUsrPara (CAxtFS20::CUsrPara &_UsrPara )};
        void GetUsrPara  (int _iAxisNo , CAxtFS20::CUsrPara &_UsrPara ){GetUsrPara (CAxtFS20::CUsrPara &_UsrPara )};

        void SetRptPara  (int _iAxisNo , CAxtFS20::CRptPara &_RptPara ){SetRptPara (CAxtFS20::CRptPara &_RptPara )};
        void GetRptPara  (int _iAxisNo , CAxtFS20::CRptPara &_RptPara ){GetRptPara (CAxtFS20::CRptPara &_RptPara )};

        void GetStat     (int _iAxisNo , CAxtFS20::CStat    &_Stat    ){GetStat    (CAxtFS20::CStat    &_Stat    )};

        void GetHomeStat (int _iAxisNo , CAxtFS20::CStat    &_HomeStat){GetHomeStat(CAxtFS20::CStat    &_HomeStat)};

        void GetRptStat  (int _iAxisNo , CAxtFS20::CRptPara &_RptPara ){GetRptStat (CAxtFS20::CRptPara &_RptPara )};

};

//---------------------------------------------------------------------------
extern CMotors MS;

//---------------------------------------------------------------------------
#endif
