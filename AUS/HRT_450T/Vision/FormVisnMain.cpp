//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormVisnMain.h"
#include "FormVisnPara.h"
#include "FormAlgoPara.h"
#include "FormImage.h"
#include "FormJob.h"
#include "SVInterface.h"
#include "SLogUnit.h"

#include "VisionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmVisnMain *FrmVisnMain=NULL;
//---------------------------------------------------------------------------
__fastcall TFrmVisnMain::TFrmVisnMain(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmVisnMain::Init()
{
    Left = 0 ;
    Top  = 0 ;
    //Application->HintColor=clRed;                            //힌트 색상
    Application->HintPause=0;                                  //힌트 뜨는 시간
    Application->HintHidePause=Application->HintHidePause*2;   //힌트 지속 시간

    // 동적으로 TabSheet 생성하기

    for(int i = 0 ; i < MAX_VISN_ID ; i++) {

        tsVisn[i] = new TTabSheet(pcVisnBase);
        tsVisn[i]->PageControl = pcVisnBase  ; // 이부분이 반드시 필요
        tsVisn[i]->Caption = VISN[i]->GetVisnStaticPara().sVisnName  ;
        tsVisn[i]->Visible = true ;
        tsVisn[i]->Align   = alClient ;
    }
    pcVisnBase -> ActivePageIndex = 0 ;
    pcVisnBase -> OnChange = pcVisnBaseChange ;

}

//---------------------------------------------------------------------------
void TFrmVisnMain::Del()
{

    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        delete tsVisn[i] ;
    }
}


void __fastcall TFrmVisnMain::btJobFileClick(TObject *Sender)
{
    FrmJob -> Show() ;
    //Trace("Operator", "FormVision Form_JobFile Button Click");

}
//---------------------------------------------------------------------------



void __fastcall TFrmVisnMain::pcVisnBaseChange(TObject *Sender)
{
   VSM.SetActiveVisn(pcVisnBase->ActivePageIndex);
}
void __fastcall TFrmVisnMain::FormDestroy(TObject *Sender)
{
    static int i = 0 ;
    //Trace("",AnsiString(i).c_str());
    i++;

}
//---------------------------------------------------------------------------


void __fastcall TFrmVisnMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    btJobFile -> Enabled = g_iCrntLevel == olMaster ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

