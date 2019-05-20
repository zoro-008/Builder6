//---------------------------------------------------------------------------

#ifndef FormAfterPickH
#define FormAfterPickH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "CSPIN.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>

#include "VisnDefine.h"
//---------------------------------------------------------------------------
class TFrmAfterPick : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TPanel *pnBase;
        TLabel *Label3;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label1;
        TComboBox *cbImageSave;
        TLabel *Label2;
        TLabel *Label4;
        TLabel *Label5;
        TBitBtn *btSave;
        TBitBtn *btCamLight;
        TLabel *Label6;
        TEdit *edAtpPxMinPer;
        TScrollBar *sbAtpPxMinPer;
        TLabel *Label7;
        TLabel *Label8;
        TEdit *edThresholdLow;
        TScrollBar *sbThresholdLow;
        TLabel *Label9;
        TEdit *edThresholdHigh;
        TScrollBar *sbThresholdHigh;
        TEdit *edCountX;
        TEdit *edCountY;
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btCamLightClick(TObject *Sender);
        void __fastcall sbThresholdLowChange(TObject *Sender);
        void __fastcall sbThresholdHighChange(TObject *Sender);
        void __fastcall sbAtpPxMinPerChange(TObject *Sender);
private:	// User declarations
        EN_VISN_ID m_iVisnId ;
public:		// User declarations
        __fastcall TFrmAfterPick(TComponent* Owner);

        void Init();
        void Close();

        void ApplyPara(bool _bToTable);
        void FormShow (int  _iPage   );
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAfterPick *FrmAfterPick;
//---------------------------------------------------------------------------
#endif
