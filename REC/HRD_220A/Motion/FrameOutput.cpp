//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrameOutput.h"
//---------------------------------------------------------------------------
//Dll Reference Header
//Log Manager
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SlogUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFraOutput *FraOutput;
//---------------------------------------------------------------------------
//__fastcall TFrameCyl::TFrameCyl(TComponent* Owner)
//        : TFrame(Owner)
__fastcall TFraOutput::TFraOutput(void) : TFrame(Owner)
{
    m_iYadd = -1;
}
//---------------------------------------------------------------------------
void TFraOutput::SetConfig(EN_OUTPUT_ID   _iYadd   ,
                           AnsiString     _sTitle  ,
                           TWinControl *  _wcParent)
{
    AnsiString sActrName;
    sActrName = _sTitle ;
    sActrName = sActrName.Delete(1 ,5);

    m_iYadd              = _iYadd          ;
    stTitle -> Caption   = sActrName       ;
    this -> Parent       = _wcParent       ;
    this -> Align        = alClient        ;

    m_bPreDone = false ;
    tmUpdate -> Enabled = true ;

    //m_bSetSize = false ;


}

void TFraOutput::SetSize()
{
    btAction -> Top      = stTitle -> Top + stTitle -> Height + 1 ;
    btAction -> Height   = this -> Height - btAction -> Top ;
    btAction -> Width    = this -> Width ;
    btAction -> Left     = 0 ;
}

void __fastcall TFraOutput::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    //if(!m_bSetSize && this -> Showing) {
    //    SetSize();
    //    m_bSetSize = false ;
    //
    //}

    bool bOut  = IO_GetY(m_iYadd) ;

    if(bOut){
        btAction -> Font -> Color = clLime ;
        btAction -> Caption       = "ON"   ;
    }
    else {
        btAction -> Font -> Color = clBlack;
        btAction -> Caption       = "OFF"  ;
    }

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFraOutput::btActionClick(TObject *Sender)
{

    IO_SetY(m_iYadd , !IO_GetY(m_iYadd));

    AnsiString sMsg ;
    sMsg = stTitle -> Caption + " Button Click " + AnsiString(IO_GetY(m_iYadd)? "(ON)" : "(OFF)") ;

    Trace("Operator", sMsg.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFraOutput::FrameResize(TObject *Sender)
{
    SetSize();
}
//---------------------------------------------------------------------------

