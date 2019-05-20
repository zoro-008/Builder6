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
        EXTERNAL_COUNTER_INPUT  iEncInput     ;   //CFS20set_enc_input_method // ���ڴ� �Է� ��� ����(ü�輳��)
		PULSE_OUTPUT            iPulseOutput  ;   //CFS20set_pulse_out_method // �޽� ��� ��� ����
		bool                    bEndLimEnable ;   //CFS20set_end_limit_enable // ������ ����Ʈ ��� ��������� ����
		bool                    bPEndLimLevel ;   //CFS20set_pend_limit_level // ������ ����Ʈ(+End limit)�� ��Ƽ�극�� ����
		bool                    bNEndLimLevel ;   //CFS20set_nend_limit_level // ������ ����Ʈ(-End limit)�� ��Ƽ�극�� ����
		bool                    bAlramEnable  ;   //CFS20set_alarm_enable     // ���� �˶� �Է½�ȣ ����� ��������� ����
		bool                    bAlramLevel   ;   //CFS20set_alarm_level      // �˶�(Alarm) ��ȣ ��Ƽ�극�� ����
        bool                    bInposEnable  ;   //CFS20set_inposition_enable// ��������(Inposition) ��ȣ ������� ����
		bool                    bInposLevel   ;   //CFS20set_inposition_level // ��������(Inposition) ��ȣ ��Ƽ�극�� ����
        bool                    bEncReverse   ;   //CFS20set_enc_reverse      // ���ڴ� �Է� ��ġ ����.
		double                  dMaxSpeed     ;   //CFS20set_max_speed        // �������ǵ� �Ѱ� ����.
	    double                  dSCurveAcPer  ;   //CFS20set_s_rate           // S-Curve���� Acc Percent , default 100%
        double                  dSCurveDcPer  ;   //CFS20set_s_rate           // S-Curve���� Dec Percent , default 100%		
	} ;

	struct CPara { //Class Level Para. (INI)
		bool    bStepMotr    ; //���ܸ������� ������������.
		int     iPulsePerRot ; //�ѹ����� �޽���.
        double  dUnitPerRot  ; //�ѹ����� �̵��Ÿ� Ȥ�� ����.
		bool    bHomeLevel   ; //Ȩ������ ����.
		bool    bUseHomeSnsr ; //Ȩ������ Ȩ�� ���� ������ -�� Ȩ�� ���� ������.
		bool    bUseSCurve   ; //S-Curve Profile �̵����.
		bool    bSrvOnLevel  ; //ServoOn Output Level����. 
		double  dMaxPos      ; //MAX Position.
		double  dMinPos      ; //MIN Position.
	} ;

	//lwb SetDis
	struct CUsrPara { //User Display Para. (UI)
		double  dAccTime    ; //�����ӽð�.
		double  dRunSpeed   ; //�����ӵ�
		double  dManSpeed   ; //�޴��� �����ӵ�
		double  dHomeSpeed  ; //Ȩ���� �ӵ�.
		double  dJogSpeed   ; //���ױ��� �ӵ�.
		double  dInposition ; //��������.
		double  dHomeOffset ; //Ȩ �����V.
	} ;
	//lwb SetDis
	struct CRptPara {  //Repeat Test Para (UI)
		double  dFstPos      ; //ù��ġ
		double  dScdPos      ; //�ι�°��ġ.
		double  dStopDelay   ; //��ǵ� �� ���� ������.
	} ;

	//lwb ���÷��̸�
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
    bool GetStop  () ; //Motion Done. ������ Ʃ���� �ȵȻ��¿��� �ܺο��� ���� �����Ҽ� �־ protected�� ����. GetStopInpos�� ����.
	
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
	void MoveInc (double _dPos , double _dVel);  //Inc move  ��ǵ� Ȯ���� GetStopInpos() �Լ���.

	bool MoveAbsRun (double _dPos );  //abs move with AutoRun Vel.
	void MoveIncRun (double _dPos );  //Inc move with AutoRun ManualVel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.

	bool MoveAbsMan (double _dPos );  //abs move with Manual Vel.
	void MoveIncMan (double _dPos );  //Inc move with Manual Vel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.

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
