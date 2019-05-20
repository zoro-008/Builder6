//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "FrameCylOneBt.h"
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel6;
        TPanel *Panel7;
        TBitBtn *btStart;
        TBitBtn *btStop;
        TBitBtn *btReset;
        TBitBtn *btOperator;
        TBitBtn *btAllHome;
        TPanel *plPassWord;
        TPanel *Panel19;
        TLabel *Label17;
        TBitBtn *btPasswordClose;
        TBitBtn *btEngr;
        TBitBtn *btOper;
        TBitBtn *btMast;
        TPanel *pnStg;
        TMainMenu *MainMenu1;
        TMenuItem *Data1;
        TMenuItem *Model1;
        TBitBtn *btExit;
        TMenuItem *Setting1;
        TMenuItem *SaveMeasureData1;
        TMenuItem *SaveImage1;
        TMenuItem *SavePrintScreen1;
        TMenuItem *ModelTestCondition1;
        TMenuItem *ModelPosition1;
        TMenuItem *Utility1;
        TTimer *tmUpdate;
        TMenuItem *MotionSettng1;
        TMenuItem *CameraSetting1;
        TMenuItem *MasterSetting1;
        TPanel *pnStgAray;
        TLabel *lbInspTime;
        TMenuItem *CommonOption1;
        TPanel *pnAtcr0;
        TPanel *pnAtcr2;
        TPanel *pnAtcr1;
        TBitBtn *btJogN;
        TBitBtn *btJogP;
        TPanel *Panel1;
        TLabel *Label6;
        TLabel *lbDevice;
        TImage *Image2;
        TLabel *lbDate;
        TLabel *lbVer;
        TLabel *lbName;
        TLabel *lbDllVer;
        TLabel *lbDllAge;
        TPanel *Panel2;
        TLabel *Label1;
        TLabel *lbDataFileName;
        TMenuItem *SaveAllData1;
        TPanel *pnShort;
        void __fastcall btExitClick(TObject *Sender);
        void __fastcall btOperatorClick(TObject *Sender);
        void __fastcall btOperClick(TObject *Sender);
        void __fastcall btResetClick(TObject *Sender);
        void __fastcall btStartClick(TObject *Sender);
        void __fastcall btStopClick(TObject *Sender);
        void __fastcall btAllHomeClick(TObject *Sender);
        void __fastcall Model1Click(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall ModelPosition1Click(TObject *Sender);
        void __fastcall ModelTestCondition1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall MotionSettng1Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall CameraSetting1Click(TObject *Sender);
        void __fastcall MasterSetting1Click(TObject *Sender);
        void __fastcall SavePrintScreen1Click(TObject *Sender);
        void __fastcall Multi1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Graph51Click(TObject *Sender);
        void __fastcall CommonOption1Click(TObject *Sender);
        void __fastcall btJogNClick(TObject *Sender);
        void __fastcall btJogPClick(TObject *Sender);
        void __fastcall SaveMeasureData1Click(TObject *Sender);
        void __fastcall SaveImage1Click(TObject *Sender);
        void __fastcall SaveAllData1Click(TObject *Sender);
        void __fastcall lbDataFileNameClick(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall lbDllAgeClick(TObject *Sender);
        void __fastcall lbNameClick(TObject *Sender);
private:	// User declarations
    void __fastcall GetPanelScreenShot(TPanel * _pPanel , String _sPath);
public:		// User declarations
        __fastcall TFrmMain(TComponent* Owner);
        __fastcall ~TFrmMain();

        TFraCylOneBt  *FraCylinder[MAX_ACTR];

        String m_sLastSaveData ;



            // InitÇÔ¼ö
    void __fastcall Init(void);

    void AutoSave();
    bool SaveRsltData (AnsiString _sDestName);

    int  m_iNeedInspCnt ;

};
AnsiString OnGetVer(const AnsiString &sQuery);
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
