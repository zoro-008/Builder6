//---------------------------------------------------------------------------

#ifndef FormEpoxyH
#define FormEpoxyH
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
#include "FormCamLightPara.h"
#include "FormTrainChk.h"
#include "SVInterface.h"
//---------------------------------------------------------------------------
class TFrmEpoxy : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TBitBtn *btSave;
        TBitBtn *btTrain;
        TPageControl *pcSet;
        TTabSheet *tsCamLight;
        TTabSheet *tsOrigin;
        TTabSheet *tsEpoxy;
        TLabel *Label3;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TComboBox *cbImageSave;
        TCSpinEdit *edCountX;
        TCSpinEdit *edCountY;
        TEdit *edPxMinPer;
        TScrollBar *sbPxMinPer;
        TEdit *edThresholdLow;
        TScrollBar *sbThresholdLow;
        TEdit *edThresholdHigh;
        TScrollBar *sbThresholdHigh;
        TLabel *Label10;
        TLabel *Label11;
        TCSpinEdit *edOriSinc;
        TGroupBox *GroupBox2;
        TLabel *Label12;
        TLabel *lbUseLightCh1;
        TLabel *lbUseLightCh2;
        TLabel *lbUseLightCh3;
        TEdit *edUseLightId;
        TGroupBox *GroupBox1;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TEdit *edUseCamId;
        TCSpinEdit *edLtBright1;
        TCSpinEdit *edLtBright2;
        TCSpinEdit *edLtBright3;
        TCSpinEdit *edCamExposure;
        TCSpinEdit *edCntOffsetX;
        TCSpinEdit *edCntOffsetY;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TPanel *pnTrain;
        TPanel *pnMag;
        TTimer *tmUpdate;
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall sbThresholdLowChange(TObject *Sender);
        void __fastcall sbThresholdHighChange(TObject *Sender);
        void __fastcall sbPxMinPerChange(TObject *Sender);
        void __fastcall pcSetChange(TObject *Sender);
        void __fastcall btTrainClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
private:	// User declarations
        EN_VISN_ID m_iVisnId ;

        TFrmTrainChk * FrmTrain ;

        IImage * m_pMagImg ; //트레숄드 확인용 이미지.

        //트레숄드 확인용 이미지 화면 그리는 것 ....
        TWndMethod OriginalProc;
        void __fastcall PanelProc(Messages::TMessage &msg);
        void OnPaintMag(); 


public:		// User declarations
        __fastcall TFrmEpoxy(TComponent* Owner);

        void Init();
        void Close();

        void ApplyPara(bool        _bToTable);
        void FormShow (EN_PAGE_ID  _iPage   );
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmEpoxy *FrmEpoxy;
//---------------------------------------------------------------------------
#endif
