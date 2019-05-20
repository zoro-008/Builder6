//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"

#include "LotUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "PstnMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLoading *FrmLoading;
//---------------------------------------------------------------------------
__fastcall TFrmLoading::TFrmLoading(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::FormShow(TObject *Sender)
{
    SetWindowRgn(Handle, CreateRoundRectRgn(ClientRect.Left, ClientRect.Top, ClientRect.Right + 1, ClientRect.Bottom + 1, 2, 2), FALSE);

    //Display Version.
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");
    lbVersion -> Caption = Date ;
    pbProcess->Position = 0;
    tmProcess->Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmProcess->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::tmProcessTimer(TObject *Sender)
{
    tmProcess -> Enabled = false ;

    static int ProcessIndex = 0 ;

    switch (ProcessIndex) {
        case 0: pbProcess->Position = 10;                     break ;
        case 1: pbProcess->Position = 20;                     break ;
        case 2: pbProcess->Position = 30;                     break ;
        case 3: pbProcess->Position = 40;                     break ;
        case 4: pbProcess->Position = 50;                     break ;
        case 5: pbProcess->Position = 60;                     break ;
        case 6: pbProcess->Position = 70;                     break ;
        case 7: pbProcess->Position = 80;                     break ;
        case 8: pbProcess->Position = 90;                     break ;
        case 9: pbProcess->Position = 99;
                ModalResult = mrOk;                           break ;
    }
    ProcessIndex++;
    tmProcess -> Enabled = true ;
}
//---------------------------------------------------------------------------

