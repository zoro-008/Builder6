//---------------------------------------------------------------------------

#ifndef FormOptionH
#define FormOptionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TFrmOption : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TCheckBox *cbLoadingStop;
        TCheckBox *cbDoorSkip;
    TBitBtn *btSave;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *edStageTLmtErr;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *edVsBfGrabDly;
        TEdit *edPenAtDnDly;
        TPanel *Panel6;
        TLabel *Label15;
        TLabel *Label16;
        TPanel *Panel7;
        TEdit *edServerPath;
        TEdit *edWorkedPath;
        TCheckBox *cbUseBarcode;
        TLabel *Label13;
        TLabel *Label14;
        TEdit *edPenDnCnt;
        TButton *btServerFolder;
        TButton *btOutputFolder;
        TLabel *Label11;
        TLabel *Label12;
        TEdit *edVisnPenYOft;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TCheckBox *cbLoadOnlyOneMapPath;
    TEdit *edLoadOnlyOneMapPath;
    TOpenDialog *odLoadOnlyOneMapPath;
    TButton *btLoadOnlyOneMap;
        TCheckBox *cbUse1stBarcode;
        TCheckBox *cbUseEndCam;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *edPenDnOverCnt;
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall cbLoadingStopClick(TObject *Sender);
        void __fastcall btOutputFolderClick(TObject *Sender);
        void __fastcall btServerFolderClick(TObject *Sender);
    void __fastcall btLoadOnlyOneMapClick(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateComOptn(bool _bToTable);
public:		// User declarations
    __fastcall TFrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOption *FrmOption;
//---------------------------------------------------------------------------
#endif
