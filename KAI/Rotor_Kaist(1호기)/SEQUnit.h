//---------------------------------------------------------------------------
#ifndef SEQUnitH
#define SEQUnitH

#include <Classes.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
//void Msg(AnsiString _sMsg)
typedef void (__closure *SeqMsgFunc)(AnsiString);

class CSEQ : public TThread
{
    public:
        __fastcall CSEQ(void);
        __fastcall ~CSEQ(void);

        //Time Check
        String sSeqStartTime , sSeqEndTime;
        int iForStart[100] ;
        int iForEnd  [100] ;
        int iForCnt  [100] ;

        int iForStartCnt ;
        int iForEndCnt   ;

        double dTotalTime  ; //double ;;; sorry
//        double dRemainTime ; //double ;;; sorry

        String GetStartTime () { return sSeqStartTime;};
        String GetEndTime   () { return sSeqEndTime  ;};
        double GetTotalTime () ;
        double GetRemainTime() ;

        //
        bool CheckCode(TStrings * _slSourse);
        void StartSeq  ();
        void DisplayFunc ( TStringGrid * _slValue) ;
        AnsiString GetErrName();

        void SetMsgFunc(SeqMsgFunc Func) { m_pMsgFunc = Func ; }

        bool IsRun (void) { return !Suspended; }
        bool ReqStop() {m_bReqStop = true ;}


    private:
        enum EN_FUNC {
            fcCHECKFINISH  = 0 ,
            fcMOVE0RPM         ,
            fcMOVEHOME         ,
            fcMOVEABS          ,
            fcMOVEINC          ,
            fcLASER            ,
            fcDELAY            ,
            fcCOMMENT          ,
            fcEMPTY            ,
            fcCAMERA           ,
            fcSERVO0           ,
            fcSHAKE0           ,
            fcOR_SETSPEED0     ,
            fcOR_SETTIME0      ,
            fcOR_SETSTART0     ,
            fcCOOLING          ,
            fcCAMLIVE          ,
            fcSETHEAT          ,
            fcFORSTART         ,
            fcFOREND           ,
            MAX_FUNC
        };

        struct TFuncInfo {
            EN_FUNC iFunc         ;
            double  dPara1        ;
            double  dPara2        ;
            double  dPara3        ;
            double  dPara4        ;
            double  dPara5        ;
            double  dPara6        ;
            double  dPara7        ;
            double  dPara8        ;
            double  dPara9        ;
            double  dPara10       ;
            double  dTime         ;
            double  dReMainTime   ;
        };

        int        m_dORSpeed[100] ;
        int        m_dORTime[100]  ;
        int        m_iMaxVal       ;
        int        m_iMaxTime      ;

        TStringList * m_slSeq     ;
        TFuncInfo   * m_FuncInfo  ;
        AnsiString    m_sErrName  ;

        TDateTime CurrDateTime ;
//        AnsiString    m_sFuncInfo ;

        int           m_iMaxLines ;

        bool GetFuncName(AnsiString &_sLine , EN_FUNC &_fcName ) ;
        bool GetPara    (AnsiString &_sLine , double &_dPara) ;

        SeqMsgFunc m_pMsgFunc ;

        bool    m_bReqStop ;

        bool    m_bSeqInit ;

        void __fastcall Execute();

        AnsiString GetFuncNameString(EN_FUNC iFunc) ;

};
extern CSEQ *pSeq;
#endif
