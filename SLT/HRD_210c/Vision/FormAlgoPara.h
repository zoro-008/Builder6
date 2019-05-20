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
        TPanel *pnInterface;
        TButton *btParaSave;
        TButton *btPre;
        TButton *btNext;
        TButton *Button5;
        TButton *btTrain;
        TButton *btClose;
        TTimer *Timer1;
        TPageControl *pcAlgPara;
        TTabSheet *tsOri;
        TPanel *pnParaOri;
        TTabSheet *tsWfr;
        TPanel *pnParaWfr;
        TTabSheet *tsRlb;
        TPanel *pnParaRalBf;
        TTabSheet *tsRla;
        TPanel *pnParaRalAt;
        TTabSheet *tsWfo;
        TPanel *pnParaWfrOr;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btTrainClick(TObject *Sender);
        void __fastcall btCloseClick(TObject *Sender);
        void __fastcall btPreClick(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall btParaSaveClick(TObject *Sender);
        void __fastcall btNextClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmAlgoPara(TComponent* Owner);

        void Init();

        void ApplyPara(bool _bToTable);
        void FormShow (int  _iPage   );


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAlgoPara *FrmAlgoPara;
//---------------------------------------------------------------------------
#endif