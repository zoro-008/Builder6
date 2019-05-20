//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UtilDefine.h"
#include "UserIni.h"
#include "FormSerial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define SERIAL_PATH "C:\\Serial.srl"

TFrmSerial *FrmSerial;




//---------------------------------------------------------------------------
__fastcall TFrmSerial::TFrmSerial(TComponent* Owner)
        : TForm(Owner)
{
    /*
    String sSerialList = EQP_SERIAL_LIST ;
    String sSerial ;

    while(sSerialList !=""){
        if(sSerialList.Pos(",")){
            sSerial = sSerialList.SubString(1,sSerialList.Pos(",")-1);
            sSerialList.Delete(1,sSerialList.Pos(","));
            lbSerial -> AddItem(sSerial , NULL);
        }
    }
    */


}
//---------------------------------------------------------------------------
void __fastcall TFrmSerial::btSetSerialClick(TObject *Sender)
{                                       //[lbSerial -> ItemIndex]

    AnsiString sTemp = lbSerial -> Items -> Strings[lbSerial -> ItemIndex] ;
    edSerial -> Text = sTemp ;

    TUserINI UserINI;

    //Make Dir.
    String sFilePath = SERIAL_PATH;
    AnsiString sSerial = sTemp ;

    UserINI.Save(sFilePath.c_str() , "Data" , "Serial" , sSerial );

}
//---------------------------------------------------------------------------

bool TFrmSerial::CheckSourceSerial()
{
/*
    TUserINI UserINI;

    //Make Dir.
    String sFilePath = SERIAL_PATH;
    AnsiString sSerial ;
    if(!FileExists(sFilePath)) {
        edSerial -> Text = "";
        return false ;
    }

    UserINI.Load(sFilePath.c_str() , "Data" , "Serial" , sSerial );

    edSerial -> Text = sSerial ;

    String sSerialList = EQP_SERIAL_LIST ;
    sSerial     = edSerial -> Text ;



    bool bTemp = IsDebuggerPresent() ;
    bool bRet = (sSerialList.Pos(sSerial+",") || IsDebuggerPresent()) ;
    return bRet ;
*/
}



void __fastcall TFrmSerial::btCloseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmSerial::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        ModalResult = mrOk ;
}
//---------------------------------------------------------------------------

