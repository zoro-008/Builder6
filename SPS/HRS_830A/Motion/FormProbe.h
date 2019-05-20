//---------------------------------------------------------------------------

#ifndef FormProbeH
#define FormProbeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
//---------------------------------------------------------------------------
class TFrmProbe : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TImage *Image2;
    TLabel *lbDate;
    TLabel *lbStat;
    TLabel *Label6;
    TLabel *lbDevice;
    TLabel *lbName;
    TPanel *Panel3;
    TTimer *tmUpdate;
    TOpenDialog *OpenDialog1;
    TSaveDialog *SaveDialog1;
        TPanel *Panel100;
        TPanel *Panel200;
    void __fastcall FormShow(TObject *Sender);

public:		// User declarations
    //Vars.
    struct SStat {
        double VfMin;
        double VfMax;
        double VfAvr;
        double VfSum;
        int    VfCnt;
        double VrMin;
        double VrMax;
        double VrAvr;
        double VrSum;
        int    VrCnt;
    };

private:	// User declarations
    SStat Stat ;

public:		// User declarations
    __fastcall TFrmProbe(TComponent* Owner);
    void __fastcall Init(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmProbe *FrmProbe;
//---------------------------------------------------------------------------
#endif
