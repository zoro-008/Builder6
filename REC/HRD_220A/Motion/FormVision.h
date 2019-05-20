//---------------------------------------------------------------------------

#ifndef FormVisionH
#define FormVisionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class TFrmVision : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TImage *Image2;
    TLabel *lbStat;
    TPanel *Panel3;
        TPageControl *pcVision;
        TTabSheet *tsPickerDefect;
        TPanel *pnPickerDefect;
        TTabSheet *tsPickerAlign;
        TPanel *pnPickerAlign;
        TTabSheet *tsStageAlignBf;
        TPanel *pnStageAlignBf;
        TTabSheet *tsStageAlignAt;
        TPanel *pnStageAlignAt;
        TTimer *tmUpdate;
        TCheckBox *cbFixPage;
        TPanel *Panel4;
    void __fastcall FormShow(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall pcVisionChange(TObject *Sender);
        void __fastcall Panel4Click(TObject *Sender);

public:		// User declarations

private:	// User declarations

public:		// User declarations
    __fastcall TFrmVision(TComponent* Owner);
    void __fastcall Init(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmVision *FrmVision;
//---------------------------------------------------------------------------
#endif
