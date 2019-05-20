//---------------------------------------------------------------------------

#ifndef FormAlgoParaH
#define FormAlgoParaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmAlgoPara : public TForm
{
__published:	// IDE-managed Components
        TPageControl *pcAlgPara;
        TTabSheet *tsWfr;
        TTabSheet *tsCal;
        TTabSheet *tsWfo;
        TPanel *pnInterface;
        TButton *btParaSave;
        TButton *btPre;
        TButton *btNext;
        TButton *btInsp;
        TButton *btTrain;
        TButton *btCenter;
        TPanel *pnParaWfo;
        TPanel *pnParaCal;
        TPanel *pnParaWfr;
        TTabSheet *tsWfp;
        TPanel *pnParaWfp;
        TPanel *Panel1;
        TTimer *tmUpdate;
        TLabel *lbMessage;
        void __fastcall btTrainClick(TObject *Sender);
        void __fastcall btPreClick(TObject *Sender);
        void __fastcall btInspClick(TObject *Sender);
        void __fastcall btParaSaveClick(TObject *Sender);
        void __fastcall btNextClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Panel1Click(TObject *Sender);
        void __fastcall btCenterClick(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmAlgoPara(TComponent* Owner);

        void Init();

        void ApplyPara(bool _bToTable);
        void FormShow (int  _iPage   );

        void SetControlHide(bool _bHide);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAlgoPara *FrmAlgoPara;
//---------------------------------------------------------------------------
#endif
