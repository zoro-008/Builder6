//---------------------------------------------------------------------------
#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
//모션 관련 Include
#include "nmc2.h"
//#include "NMC410S.h"
//#include "MyUtil.h"
#include "PaixMotion.h"
#include "Timer.h"
//---------------------------------------------------------------------------

//enum EN_OUTPUT_ID  {
//    yETC_StartSw        =  0 , //
//    yETC_StopSw         =  1 , //
//    yETC_HomeSw         =  2 , //
//    yETC_CoolingSw      =  3 , //
//    yETC_CoolingFan     =  4 , //
//    y005                =  5 , //
//    y006                =  6 , //
//    y007                =  7 , //
//    y008                =  8 , //
//};
//enum EN_INPUT_ID {
//    xETC_StartSw        =  0 , //
//    xETC_StopSw         =  1 , //
//    xETC_HomeSw         =  2 , //
//    xETC_CoolingSw      =  3 , //
//    xETC_Emergency      =  4 , //
//    x005                =  5 , //
//    x006                =  6 , //
//    x007                =  7 , //
//    x008                =  8 , //
//};
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
        TTimer *tmUpdate;
        TPanel *pnBase;
        TPanel *PnButton;
        TPanel *pnTiltMTShow;
        TPanel *pnSettingShow;
        TPanel *pnSpinMTShow;
        TPanel *pnValveShow;
        TPanel *pnMainShow;
        TPanel *pnExit;
        TPanel *pnDeviceShow;
        TTimer *Timer1;
//    void __fastcall HomeX(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    //모션 관련.
    void __fastcall ReadLogic(void);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall pnExitClick(TObject *Sender);
    void __fastcall pnMainShowClick(TObject *Sender);
        void __fastcall pnMainShowMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall pnMainShowMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);


private:	// User declarations
//    typedef unsigned char U8 ;
    int iBtnNum;

    struct TStQuery {
        unsigned char  id;
        unsigned char  func;
        unsigned char  start_addr_hi;
        unsigned char  start_addr_lo;
        unsigned char  size_hi;
        unsigned char  size_lo;
        unsigned short CRC16;
    };
    struct TStResponse {
        unsigned char  id;
        unsigned char  func;
        unsigned char  start_addr_hi;
        unsigned char  start_addr_lo;
        unsigned char  size_hi;
        unsigned char  size_lo;
        unsigned short CRC16;
    };

    struct TData {
        unsigned char *hi;
        unsigned char *lo;
    };
    struct TStep {
        int iSeq     ; //Step.
        int iCycle   ; //싸이클동작의 스텝.
        int iHome    ; //홈 잡는 스텝.
        int iToStart ; //시작준비 스텝.
        int iToStop  ; //정지준비 스텝.
    } Step , PreStep ;


    TStQuery    m_StQuery   ;
    TStResponse m_StResponse  ;
    TData       m_Data      ;

    int         m_iData     ;


    bool        m_bComplite ;

    CDelayTimer m_CycleTimer;


    AnsiString GetLastJobName ( );

    int RowCount;
    bool m_bOpen;
    AnsiString	m_strModelName;
    AnsiString	m_strDeviceID;
    AnsiString  sMsg   [100] ;

    bool        m_bRecive ;
    TWndMethod  OriginalProc;

    void __fastcall ContinuousShot();
    //모션 관련.
    //bool __fastcall InputChanged  (NMCBIT   NmcBit      );   JS
    //void __fastcall OutputChanged (BYTE     Output      );   JS
    bool __fastcall CheckHeat     (int _iId             );
    bool __fastcall SetHeatTimer  (int _iId, int _iData );
    BYTE OutputIO;
  	UINT m_nBit[8];
    int m_nDistMode[4];
    //
//---------------------------------------------------------------------------
public:		// User declarations

    int m_iPageIndex;

    bool m_bAxis1Stop;
    AnsiString CurrntDevice;

    int m_nMotionMode[2];
    UINT OutputOld[8];

    short m_nStopInfo[8];

    bool m_bStop;
    bool m_bAlrmClear;
    bool m_bEmgClear ;

    __fastcall TFrmMain(TComponent* Owner);

    void __fastcall ShowPage     (int index);
    void __fastcall ClosePage    (int index);

    AnsiString sTemp;

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
