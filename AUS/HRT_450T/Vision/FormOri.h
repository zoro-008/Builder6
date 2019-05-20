//---------------------------------------------------------------------------

#ifndef FormOriH
#define FormOriH
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
class TFrmOri : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TBitBtn *btSave;
        TBitBtn *btTrain;
        TPageControl *pcSet;
        TTabSheet *tsCamLight;
        TTabSheet *tsOrigin;
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
        TLabel *lbCh1;
        TLabel *lbCh2;
        TLabel *lbCh3;
        TPanel *pnTrain;
        TTimer *tmUpdate;
        TLabel *Label1;
        TComboBox *cbImageSave;
        TButton *btTrackerInit;
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall pcSetChange(TObject *Sender);
        void __fastcall btTrainClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall btTrackerInitClick(TObject *Sender);
private:	// User declarations
        EN_VISN_ID m_iVisnId ;

        TFrmTrainChk * FrmTrain ;

        //IImage * m_pMagImg ; //트레숄드 확인용 이미지.
        //트레숄드 확인용 이미지 화면 그리는 것 ....
        //TWndMethod OriginalProc;
        //void __fastcall PanelProc(Messages::TMessage &msg);
        //void OnPaintMag();


public:		// User declarations
        __fastcall TFrmOri(TComponent* Owner);

        void Init();
        void Close();

        void ApplyPara(bool        _bToTable);
        void FormShow (EN_PAGE_ID  _iPage   );
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOri *FrmOri;
//---------------------------------------------------------------------------
#endif
