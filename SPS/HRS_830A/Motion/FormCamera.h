//---------------------------------------------------------------------------

#ifndef FormCameraH
#define FormCameraH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <DBCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFrmCamera : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnTitle;
        TImage *Image2;
        TLabel *lbDate;
        TLabel *lbStat;
        TLabel *Label6;
        TLabel *lbDevice;
        TLabel *lbName;
        TPanel *Panel3;
        TPanel *Panel1;
        TPanel *Panel2;
        TImage *Image1;
        TLabel *lbDisprLTime;
        TLabel *lbDisprRTime;
        TTimer *tmUpdate;
        void __fastcall tmUpdateTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmCamera(TComponent* Owner);
        void Init();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCamera *FrmCamera;
//---------------------------------------------------------------------------
#endif
