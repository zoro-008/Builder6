// CamcFS20.h: interface for the CCamcFS20 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMCFS20_H__5EAF66B2_5105_4793_952A_3517862BC562__INCLUDED_)
#define AFX_CAMCFS20_H__5EAF66B2_5105_4793_952A_3517862BC562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////////////////////////////////////////////////////////////////////
//CamcFS20.h
//Hanra Precision All Right Reserved.
//
//SUN Kye Won 2010.12.22 Ver 1.0
///////////////////////////////////////////////////////////////////////


#include "..\\IncludeAxt/AxtLIBDef.h"
#include "..\\IncludeAxt/CAMCFSDef.h"
class CCamcFS20
{
public:
	CCamcFS20(int _iAxisNo);
	virtual ~CCamcFS20();

	struct CAxtPara{ //Board Level Direct Access Para. (INI)
        EXTERNAL_COUNTER_INPUT  iEncInput     ;   //CFS20set_enc_input_method // 엔코더 입력 방식 설정(체배설정)
		PULSE_OUTPUT            iPulseOutput  ;   //CFS20set_pulse_out_method // 펄스 출력 방식 설정
		bool                    bEndLimEnable ;   //CFS20set_end_limit_enable // 급정지 리미트 기능 사용유무를 설정
		bool                    bPEndLimLevel ;   //CFS20set_pend_limit_level // 정방향 리미트(+End limit)의 액티브레벨 설정
		bool                    bNEndLimLevel ;   //CFS20set_nend_limit_level // 역방향 리미트(-End limit)의 액티브레벨 설정
		bool                    bAlramEnable  ;   //CFS20set_alarm_enable     // 서보 알람 입력신호 기능의 사용유무를 설정
		bool                    bAlramLevel   ;   //CFS20set_alarm_level      // 알람(Alarm) 신호 액티브레벨 설정
        bool                    bInposEnable  ;   //CFS20set_inposition_enable// 인포지션(Inposition) 신호 사용유무 설정
		bool                    bInposLevel   ;   //CFS20set_inposition_level // 인포지션(Inposition) 신호 액티브레벨 설정
        bool                    bEncReverse   ;   //CFS20set_enc_reverse      // 엔코더 입력 수치 반전.
		double                  dMaxSpeed     ;   //CFS20set_max_speed        // 구동스피드 한계 설정.
	    double                  dSCurveAcPer  ;   //CFS20set_s_rate           // S-Curve구동 Acc Percent , default 100%
        double                  dSCurveDcPer  ;   //CFS20set_s_rate           // S-Curve구동 Dec Percent , default 100%		
	} ;

	struct CPara { //Class Level Para. (INI)
		bool    bStepMotr    ; //스텝모터인지 서보모터인지.
		int     iPulsePerRot ; //한바퀴의 펄스수.
        double  dUnitPerRot  ; //한바퀴당 이동거리 혹은 각도.
		bool    bHomeLevel   ; //홈센서의 레벨.
		bool    bUseHomeSnsr ; //홈센서로 홈을 잡을 것인지 -로 홈을 잡을 것인지.
		bool    bUseSCurve   ; //S-Curve Profile 이동사용.
		bool    bSrvOnLevel  ; //ServoOn Output Level설정. 
		double  dMaxPos      ; //MAX Position.
		double  dMinPos      ; //MIN Position.
	} ;

	//lwb SetDis
	struct CUsrPara { //User Display Para. (UI)
		double  dAccTime    ; //가감속시간.
		double  dRunSpeed   ; //구동속도
		double  dManSpeed   ; //메뉴얼 구동속도
		double  dHomeSpeed  ; //홈구동 속도.
		double  dJogSpeed   ; //조그구동 속도.
		double  dInposition ; //인포지션.
		double  dHomeOffset ; //홈 오프셑.
	} ;
	//lwb SetDis
	struct CRptPara {  //Repeat Test Para (UI)
		double  dFstPos      ; //첫위치
		double  dScdPos      ; //두번째위치.
		double  dStopDelay   ; //모션돈 후 정지 딜레이.
	} ;

	//lwb 디스플레이만
	struct CStat { //Motor Status for Display.
        //Motion Status.
        bool   bServoOn      ;
        bool   bHomeSnsr     ;
        bool   bStop         ;
        bool   bBusy         ;
        bool   bHomeEnded    ;
        bool   bPackInPosn   ;
        bool   bAlarm        ;
        bool   bPEndLimSnsr  ;
        bool   bNEndLimSnsr  ;
        bool   bReady        ;
        bool   bOk           ;
		bool   bJogP         ;
		bool   bJogN         ;

        //Pos Stat
        double dTrgPos       ;
        double dCmdPos       ;
        double dEncPos       ;     
	} ;

	struct CHomeStat{
        int  iHomeStep     ;
        bool bGoHome       ;
		bool bHomeTimeout  ;
	} ;

	struct CRptStat{
		bool bGoRpt        ;
	} ;

private:
protected:
	bool InitLib  () ; //Library Initial.
	bool CycleHome() ; //Home Cycle Processing.
	void CycleRpt () ; //Repeat Cycle Processing.
    bool GetStop  () ; //Motion Done. 서보팩 튜닝이 안된상태에서 외부에서 쓰면 위험할수 있어서 protected로 선언. GetStopInpos로 쓸것.
	
	CAxtPara   AxtPara  ;
    CPara      Para     ;
	CUsrPara   UsrPara  ;
	CStat      Stat     ;
	CHomeStat  HomeStat ;
	CRptPara   RptPara  ;
	CRptStat   RptStat  ;

	int   m_iAxisNo  ; 

public:
	//Thread Cycle Func.
	void Update (); 

	//Apply AXT Liblary Directly Access Para 
	void ApplyAxtPara ();

	//Set Stat Directly Func.
    void SetHomeEnd (bool _bValue ); 
    void SetServo   (bool _bOn    ); 
    void SetAlarm   (bool _bOn    );

    //Update Motor Status.
	bool GetStopInpos() ; //Check Motion Done with User Inposition.
    bool GetReady    () ; //Check ready to move.
    bool GetOk       () ; //Check No Limit & Servo Enable.

    //Position Functions.
    double GetTrgPos();                                       //Get Target  Position.
    double GetCmdPos();                                       //Get Command Position.
    double GetEncPos();                                       //Get Encoder Position.
    double GetErrPos();                                       //Get Control Error Position.
    void   ClearPos ();                                       //Clear Position.
    void   SetPos   (double _dPos = 0.0);                     //Set Any Position.	

    //Motion Functions.
    void Reset   (); //Reset
    bool Stop    (); 
	bool EmgStop (); //Stop Without Deceleration.

    bool MoveHome(); //Start Home Cycle for Sequense Cycle.
    void DoHome  (); //Start Home Cycle for Click Event.

	void DoRpt   (); //Start Home Cycle for Click Event.

    bool MoveJogP(); //Jogging to CW.
    bool MoveJogN(); //Jogging to CCW.

	bool MoveAbs (double _dPos , double _dVel);  //abs move 
	void MoveInc (double _dPos , double _dVel);  //Inc move  모션돈 확인은 GetStopInpos() 함수로.

	bool MoveAbsRun (double _dPos );  //abs move with AutoRun Vel.
	void MoveIncRun (double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.

	bool MoveAbsMan (double _dPos );  //abs move with Manual Vel.
	void MoveIncMan (double _dPos );  //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.

	//Parameter Memory Handling Fuctions.
	void SetAxtPara (CAxtPara &_AxtPara); void GetAxtPara  (CAxtPara &_AxtPara );
	void SetPara    (CPara    &_Para   ); void GetPara     (CPara    &_Para    );
	void SetUsrPara (CUsrPara &_UsrPara); void GetUsrPara  (CUsrPara &_UsrPara );
	void SetRptPara (CRptPara &_RptPara); void GetRptPara  (CRptPara &_RptPara );
	                                      void GetStat     (CStat    &_Stat    );
										  void GetHomeStat (CStat    &_HomeStat);
	                                      void GetRptStat  (CRptPara &_RptPara );

	//Save Para Functions.
    void SavePara(CString _sFilePath); 
	void LoadPara(CString _sFilePath);
};

#endif // !defined(AFX_CAMCFS20_H__5EAF66B2_5105_4793_952A_3517862BC562__INCLUDED_)
