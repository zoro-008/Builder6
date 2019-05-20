//---------------------------------------------------------------------------

#ifndef VisionComH
#define VisionComH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "UtilDefine.h"


//---------------------------------------------------------------------------
enum EN_VISN_SEND {
    vsInsp      = 0 ,
    vsReset         ,
    vsCommand       ,
    vsJobChange     ,
    MAX_VISN_SEND
};

struct TAlignResult {
    double dMainX ;
    double dMainY ;
    double dSubX  ;
    double dSubY  ;
};

struct TGapResult {
    double dMain  ;
    double dSub   ;
};


class CVisionCom
{
    public: //초기화 등등.
        //Constructor
        CVisionCom ();
        ~CVisionCom ();

        void Init ();
        void Close();

    protected:
        //Timer.
        struct TVisnIO{
            EN_INPUT_ID  xVisn_Ready     ;
            EN_INPUT_ID  xVisn_Busy      ;
            EN_INPUT_ID  xVisn_InspOk    ;
            EN_INPUT_ID  xVisn_Spare1    ;
            EN_INPUT_ID  xVisn_Spare2    ;

            EN_OUTPUT_ID yVisn_InspStart ;
            EN_OUTPUT_ID yVisn_Reset     ;
            EN_OUTPUT_ID yVisn_Command   ;
            EN_OUTPUT_ID yVisn_JobChange ;
            EN_OUTPUT_ID yVisn_Live      ;
        }VIO;

        struct TCycle{
            int         iStep     ;
            int         iPreStep  ;
            CDelayTimer tmStep    ;
            CDelayTimer tmDelay   ;
            int         iResetCnt ;
            TCycle(){
                iStep     = 0 ;
                iPreStep  = 0 ;
                tmStep .Clear();
                tmDelay.Clear();
                iResetCnt = 0;
            }
        };

        int m_iVisnId ; //0:Left , 1:Right

        TCycle VisnSendCycle[MAX_VISN_SEND]     ;

        TAlignResult m_tAlign ;
        TGapResult   m_tGap   ;
        bool         m_bOk    ;


        bool SaveVisnJobFile(AnsiString _sDevice );
        bool SaveVisnLotNo  (AnsiString _sLotNo  );//안씀.
        bool SaveVisnCommand(AnsiString _sCommand);

        //One Cycle.
        bool CycleInsp     ();
        bool CycleReset    ();
        bool CycleCommand  ();
        bool CycleJobChange();

    public:
        void Update();

        void Reset();

        bool SendInsp     ();                 bool GetSendInspEnd     ();
	bool SendReset    ();                 bool GetSendResetEnd    ();
        bool SendCommand  (String _sCommand); bool GetSendCommandEnd  ();
        bool SendJobChange(String _sJobName); bool GetSendJobChangeEnd();


        bool LoadAlignRslt(); void DeleteAlignRslt();
        bool LoadGapRslt  (); void DeleteGapRslt  ();

        bool         GetInspOk   ();
        TAlignResult GetAlignRslt();
        TGapResult   GetGapRslt  ();

};

//---------------------------------------------------------------------------
extern CVisionCom VSN_L;
extern CVisionCom VSN_R;
//---------------------------------------------------------------------------
#endif
