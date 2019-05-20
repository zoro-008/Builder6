//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormVision.h"

#include "UtilDefine.h"
#include "Sequence.h"
#include "SVInterface.h"
#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmVision *FrmVision;
//---------------------------------------------------------------------------
__fastcall TFrmVision::TFrmVision(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TFrmVision::FormShow(TObject *Sender)
{
    Top    = 0 ;
    Left   = 1280 ;
    Width  = 1280 ;
    Height = 1024 ;


    Init();

    tmUpdate->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TFrmVision::Init(void)
{
    //JS
    if(!SKIP_VISN)SV_SetVisnForm(0,pnPickerDefect->Handle);
    if(!SKIP_VISN)SV_SetVisnForm(1,pnPickerAlign ->Handle);
    if(!SKIP_VISN)SV_SetVisnForm(2,pnStageAlignBf->Handle);
    if(!SKIP_VISN)SV_SetVisnForm(3,pnStageAlignAt->Handle);

    pcVision -> ActivePageIndex = 0 ;

    

}


void __fastcall TFrmVision::FormCreate(TObject *Sender)
{
    if(!LoadSVision("pSVision.dll")){
        ShowMessage("Dll Loading failed");
    }
    //JS
    if(!SKIP_VISN)SV_InitVisn();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVision::FormDestroy(TObject *Sender)
{
    //JS
    if(!SKIP_VISN)SV_DelVisn();
    //JS
    FreeSVision();
}
//---------------------------------------------------------------------------

void __fastcall TFrmVision::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

    EN_LEVEL iLevel;
    static EN_LEVEL iPreLevel = FM_GetLevel();

    iLevel = FM_GetLevel();

    if(iLevel != iPreLevel) {
        //JS
        Trace("Lock", "SV_SetLevel Stt");
        if(!SKIP_VISN)SV_SetLevel(iLevel);
        Trace("Lock", "SV_SetLevel End");
        iPreLevel = iLevel ;
    }

    EN_SEQ_STAT iSeq ;
    static EN_SEQ_STAT iPreSeq = SEQ._iSeqStat ;

    iSeq = SEQ._iSeqStat ;


    if(iSeq !=iPreSeq) {
        Trace("Lock", "SV_SetAutorun Stt");
        if(!SKIP_VISN)SV_SetAutorun(iSeq == ssRunning);
        Trace("Lock", "SV_SetAutorun End");
        iPreSeq = iSeq ;
    }

    int iActiveVisn = 0 ;
    static int iPreActiveVisn = 0 ;
    if(SEQ._bRun || EM_IsErr()) {
        if(!SKIP_VISN){
            iActiveVisn = SV_GetLastInsp();
        }
        if(!cbFixPage-> Checked &&  iActiveVisn != iPreActiveVisn){
            pcVision -> ActivePageIndex = iActiveVisn ;
            iPreActiveVisn = iActiveVisn ;
        }
    }





    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmVision::pcVisionChange(TObject *Sender)
{
    //JS
    Trace("Lock", "pcVisionChange Stt");
    if(!SKIP_VISN)SV_LiveOffAll();
    Trace("Lock", "pcVisionChange End");
}
//---------------------------------------------------------------------------


void __fastcall TFrmVision::Panel4Click(TObject *Sender)
{
    if(this -> BorderStyle == bsNone){
        this -> BorderStyle = bsSizeable ;
    }
    else {
        this -> BorderStyle = bsNone ;
    }

}
//---------------------------------------------------------------------------

