//---------------------------------------------------------------------------

#ifndef FormDeviceH
#define FormDeviceH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include "FrameMotr.h"
//---------------------------------------------------------------------------
class TFrmDevice : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel4;
    TLabel *Label3;
    TLabel *Label4;
    TBitBtn *btDownload;
    TPanel *lbCrntDevice;
    TPanel *lbSelDevice;
    TPanel *Panel2;
    TStringGrid *sgDevice;
    TPanel *Panel6;
    TLabel *lbFromName;
    TEdit *edFromName;
    TBitBtn *btCopy;
    TBitBtn *btDelete;
    TBitBtn *btNew;
    TBitBtn *btRename;
    TLabel *lbToName;
    TEdit *edToName;
    TProgressBar *pbStatus;
    TPanel *Panel7;
    TPanel *Panel5;
    TPanel *Panel8;
    TTimer *tmUpdate;
    TImage *Image3;

    void __fastcall sgDeviceSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
    void __fastcall btDownloadClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btNewClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btSaveDeviceClick(TObject *Sender);



private:	// User declarations
    double m_dUnit    ;
    double m_dPitch   ;
    int    m_iUnitType;

    void SetUnit(EN_UNIT_TYPE _iUnitType , double _dUnit);
public:		// User declarations
     TFraMotr *FraMotor   [MAX_MOTR];

    __fastcall TFrmDevice(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDevice *FrmDevice;
//---------------------------------------------------------------------------
#endif
