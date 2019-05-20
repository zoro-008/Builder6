//---------------------------------------------------------------------------

#ifndef FormDeviceH
#define FormDeviceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//#include "FormDeviceSet.h"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
#include "SMDllDefine.h"
//#include "TEdit.h"
class TFrmDevice : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TStringGrid *sgDevice;
    TPanel *Panel4;
    TLabel *Label3;
    TLabel *Label4;
    TPanel *Panel5;
    TBitBtn *btDownload;
    TProgressBar *pbStatus;
    TPanel *lbCrntDevice;
    TPanel *lbSelDevice;
    TPanel *Panel6;
    TLabel *lbFromName;
    TLabel *lbToName;
    TPanel *Panel7;
    TBitBtn *btNew;
    TBitBtn *btRename;
    TBitBtn *btCopy;
    TBitBtn *btDelete;
    TEdit *edFromName;
    TEdit *edToName;
    TImage *Image1;
    TLabel *Label1;
    TBitBtn *btSetting;
    TTimer *tmUpdate;
    TImage *Image2;
    TImage *Image3;
    TImage *Image4;
    TLabel *Label5;
    TLabel *Label2;
    void __fastcall btSettingClick(TObject *Sender);

    void __fastcall sgDeviceSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
    void __fastcall btDownloadClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btNewClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);

private:	// User declarations
    bool __fastcall LanguageSel(EN_LAN_SEL lvType);
public:		// User declarations
    __fastcall TFrmDevice(TComponent* Owner);

    int iFileProcIndex;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDevice *FrmDevice;
//---------------------------------------------------------------------------
#endif