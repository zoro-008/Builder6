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

        bool CheckCode(TStrings * _slSourse);
        void StartSeq  () {m_bSeqInit = false ; Resume();}
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
            fcOR_SETSTART      ,

            MAX_FUNC
        };


        struct TFuncInfo {
            EN_FUNC iFunc  ;
            double  dPara1 ;
            double  dPara2 ;
            double  dPara3 ;
            double  dPara4 ;
            double  dPara5 ;
            double  dPara6 ;
            double  dPara7 ;
            double  dPara8 ;
            double  dPara9 ;
            double  dPara10 ;
        };

        int        m_dORSpeed[100] ;
        int        m_dORTime[100]  ;
        int        m_iMaxVal       ;
        int        m_iMaxTime      ;

        TStringList * m_slSeq    ;
        TFuncInfo   * m_FuncInfo ;
        AnsiString    m_sErrName ;

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
