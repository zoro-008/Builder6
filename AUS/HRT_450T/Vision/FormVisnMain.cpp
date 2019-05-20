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
    //Application->HintColor=clRed;                            //��Ʈ ����
    Application->HintPause=0;                                  //��Ʈ �ߴ� �ð�
    Application->HintHidePause=Application->HintHidePause*2;   //��Ʈ ���� �ð�

    // �������� TabSheet �����ϱ�

    for(int i = 0 ; i < MAX_VISN_ID ; i++) {

        tsVisn[i] = new TTabSheet(pcVisnBase);
        tsVisn[i]->PageControl = pcVisnBase  ; // �̺κ��� �ݵ�� �ʿ�
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

