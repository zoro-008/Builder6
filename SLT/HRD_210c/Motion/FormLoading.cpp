//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"
#include "DataMan.h"
#include "Rs232Man.h"
#include "MapUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "SpcSubUnit.h"
#include "SpcUnit.h"
#include "LotUnit.h"
#include "ManualMan.h"
#include "Loader.h"
#include "PreBuff.h"
#include "RailEpx.h"
#include "Epoxy.h"
#include "RailDie.h"
#include "Head.h"
#include "Stage.h"
#include "PostBuff.h"
#include "UnLoader.h"
#include "Sequence.h"

#include "SMInterfaceUnit.h"
#include "SAInterface.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLoading *FrmLoading;
//---------------------------------------------------------------------------
bool g_bLoad = true ;
int  g_iStep = 0    ;
__fastcall TFrmLoading::TFrmLoading(TComponent* Owner , bool _bLoad)
    : TForm(Owner)
{
    g_bLoad = _bLoad ;
    g_iStep = 0      ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::FormShow(TObject *Sender)
{
    SetWindowRgn(Handle, CreateRoundRectRgn(ClientRect.Left, ClientRect.Top, ClientRect.Right + 1, ClientRect.Bottom + 1, 2, 2), FALSE);

    //Display Version.
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'년 'm'월 'd'일 'AM/PM' 'h'시 'n'분'");
    lbVersion -> Caption  = Date ;
    pbProcess -> Position = 0;
    tmProcess -> Enabled  = true ;

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

    if(g_bLoad) {

        switch (g_iStep) {
            case  0: pbProcess->Position =  5;  OM  .Init();
                                                lbName    -> Caption  = OM.EqpOptn.sModelName ;
                                                LoadSML("SMDll" );

                                                                    break ;
            case  1: pbProcess->Position = 10;  LoadSVA("pSVApp");
                                                
                                                                    break ;
            case  2: pbProcess->Position = 15;  DM  .Init();        break ;
            case  3: pbProcess->Position = 20;  RSM .Init();        break ;
            case  4: pbProcess->Position = 25;  MAPU.Init();        break ;
            case  5: pbProcess->Position = 30;  PM  .Init();        break ;
            case  6: pbProcess->Position = 35;  SPCS.Init();        break ;
            case  7: pbProcess->Position = 40;  SPC .Init();        break ;
            case  8: pbProcess->Position = 45;  LT  .Init();        break ;
            case  9: pbProcess->Position = 50;  MM  .Init();        break ;
            case 10: pbProcess->Position = 55;  LDR .Init();        break ;
            case 11: pbProcess->Position = 60;  PRB .Init();        break ;
            case 12: pbProcess->Position = 65;  RAE .Init();        break ;
            case 13: pbProcess->Position = 70;  EPX .Init();        break ;
            case 14: pbProcess->Position = 75;  RAD .Init();        break ;
            case 15: pbProcess->Position = 80;  HED .Init();        break ;
            case 16: pbProcess->Position = 85;  STG .Init();        break ;
            case 17: pbProcess->Position = 90;  PSB .Init();        break ;
            case 18: pbProcess->Position = 95;  ULD .Init();        break ;
            case 19: pbProcess->Position =100;  SEQ .Init();        break ;
            case 20: pbProcess->Position =100;  ModalResult = mrOk; break ;
        }
    }
    else {
        switch (g_iStep) {
            case  0: pbProcess->Position =  5;  lbName    -> Caption  = OM.EqpOptn.sModelName ;

                                                
                                                
                                                       break ;
            case  1: pbProcess->Position = 10;  
                                                
                                                                    break ;
            case  2: pbProcess->Position = 15;                      break ;
            case  3: pbProcess->Position = 20;                      break ;
            case  4: pbProcess->Position = 25;                      break ;
            case  5: pbProcess->Position = 30;                      break ;
            case  6: pbProcess->Position = 35;                      break ;
            case  7: pbProcess->Position = 40;                      break ;
            case  8: pbProcess->Position = 45;                      break ;
            case  9: pbProcess->Position = 50;                      break ;
            case 10: pbProcess->Position = 55;                      break ;
            case 11: pbProcess->Position = 60;                      break ;
            case 12: pbProcess->Position = 65;                      break ;
            case 13: pbProcess->Position = 70;                      break ;
            case 14: pbProcess->Position = 75;                      break ;
            case 15: pbProcess->Position = 80;                      break ;
            case 16: pbProcess->Position = 85;                      break ;
            case 17: pbProcess->Position = 90;                      break ;
            case 18: pbProcess->Position = 95;                      break ;
            case 19: pbProcess->Position =100;                      break ;
            case 20: pbProcess->Position =100;  ModalResult = mrOk; break ;
        }
    }
    g_iStep++;
    tmProcess -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::FormDestroy(TObject *Sender)
{
    tmProcess->Enabled = false ;
}
//---------------------------------------------------------------------------

