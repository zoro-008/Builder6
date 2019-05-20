//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormVisnMain.h"
#include "FormVisnPara.h"
#include "FormAlgoPara.h"
#include "FormImage.h"
#include "SVInterface.h"
#include "SLogUnit.h"
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

        Sleep(1); Application -> ProcessMessages() ;
        tsVisn[i] = new TTabSheet(pcVisnBase);
        Sleep(1); Application -> ProcessMessages() ;
        tsVisn[i]->PageControl = pcVisnBase  ; // 이부분이 반드시 필요
        Sleep(1); Application -> ProcessMessages() ;
        tsVisn[i]->Caption = VISN[i]->m_sVisnName  ;
        Sleep(1); Application -> ProcessMessages() ;
        tsVisn[i]->Visible = true ;
        Sleep(1); Application -> ProcessMessages() ;
        tsVisn[i]->Align   = alClient ;
        Sleep(1); Application -> ProcessMessages() ;



        //TTabSheet *pTabSheet = new TTabSheet(PageControl2);  // Initial TabSheet Component
        //pTabSheet->PageControl = PageControl2;    // Initial TabSheet Component

        Sleep(1); Application -> ProcessMessages() ;
        FrmImg[i] = new TFrmImage(pcVisnBase);
        Sleep(1); Application -> ProcessMessages() ;
        FrmImg[i] -> Parent = tsVisn[i] ;
        Sleep(1); Application -> ProcessMessages() ;
        FrmImg[i] -> Left = 0 ;
        Sleep(1); Application -> ProcessMessages() ;
        FrmImg[i] -> Top  = 0 ;
        Sleep(1); Application -> ProcessMessages() ;
        FrmImg[i] -> SetVisnId(i) ;
        Sleep(1); Application -> ProcessMessages() ;
        FrmImg[i] -> Show() ;

    }
    pcVisnBase -> ActivePageIndex = 3 ; //MAX_VISN_ID - 1 ;
    pcVisnBase -> OnChange = pcVisnBaseChange ;

}






void __fastcall TFrmVisnMain::btSetupClick(TObject *Sender)
{
    if(Parent == Application -> MainForm) {
        Sleep(100);
        Parent = OriParent ;
        //Application -> ProcessMessages() ;
        btSetup -> Caption = "Setup";
        for(int i = 0 ; i < MAX_VISN_ID ; i++) {
            FrmImg[i] -> SetFormEnable(false);
        }
        Trace("Operator", "FormVision Form_Set Up Button Click");

    }
    else {
        OriParent = Parent ;
        Parent = Application -> MainForm ;
        btSetup -> Caption = "Return";

        for(int i = 0 ; i < MAX_VISN_ID ; i++) {
            FrmImg[i] -> SetFormEnable(true);
        }
        Trace("Operator", "FormVision Form_Return Button Click");
    }
}
//---------------------------------------------------------------------------





void __fastcall TFrmVisnMain::btJobFileClick(TObject *Sender)
{
    FrmJob -> Show() ;
    Trace("Operator", "FormVision Form_JobFile Button Click");

}
//---------------------------------------------------------------------------



void TFrmVisnMain::SetLevel(int _iLevel)
{
    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        FrmImg[i] -> SetLevel(_iLevel) ;
    }

    if(_iLevel ==0) {
        btJobFile -> Enabled = false ;
        btSetup   -> Enabled = false ;
    }
    else {
        btJobFile -> Enabled = true  ;
        btSetup   -> Enabled = true  ;

    }



}

void __fastcall TFrmVisnMain::pcVisnBaseChange(TObject *Sender)
{
    UpdateVisnBaseStat(pcVisnBase -> ActivePageIndex);
}
//---------------------------------------------------------------------------
void TFrmVisnMain::UpdateVisnBaseStat(int _iPageIndex)
{
   //파라창들 떠있으면 인덱스 변경한다.
    if(FrmVisnPara -> Showing)FrmVisnPara -> FormShow(pcVisnBase -> ActivePageIndex);
    if(FrmAlgoPara -> Showing)FrmAlgoPara -> FormShow(pcVisnBase -> ActivePageIndex);


    for(int i = 0 ; i < MAX_VISN_ID ; i++) {

        VISN[i]->Live(false) ;

        FrmImg[i] -> tmUpdate -> Enabled = (i == _iPageIndex);
        //FrmImg[i] -> SetPaintLock(i != _iPageIndex);
    }
}

void __fastcall TFrmVisnMain::FormDestroy(TObject *Sender)
{
    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;

}
//---------------------------------------------------------------------------

void __fastcall TFrmVisnMain::FormShow(TObject *Sender)
{
    //FrmVisnMain -> Align = alClient ;
    //LPRECT lpRect ;
    //GetClientRect(_hWnd , lpRect);
    //
    //FrmVisnMain -> Left = 0 ;
    //FrmVisnMain -> Top  = 0 ;
    //FrmVisnMain -> Width = lpRect-> right ;
    //FrmVisnMain -> Height= lpRect-> bottom ;
}
//---------------------------------------------------------------------------

