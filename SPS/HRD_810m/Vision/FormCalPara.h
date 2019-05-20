//---------------------------------------------------------------------------

#ifndef FormCalParaH
#define FormCalParaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmCalPara : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnBase;
        TPanel *Panel1;
        TBitBtn *btClose;
        TBitBtn *btCal;
        TBitBtn *btSave;
        void __fastcall btCloseClick(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall btCalClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        int m_iVisnId ;
public:		// User declarations
        __fastcall TFrmCalPara(TComponent* Owner);

//        __fastcall TFrmCalPara(TComponent* Owner , int _iVisnId , bool _bInit);

        void ApplyPara(bool _bToTable);

        void Show(int _iVisnId);

        int  m_iOriViewMode ;


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCalPara *FrmCalPara;
//---------------------------------------------------------------------------
#endif
