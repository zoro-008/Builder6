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
    String sSerialList = EQP_SERIAL_LIST ;
    String sSerial ;

    while(sSerialList !=""){
        if(sSerialList.Pos(",")){
            sSerial = sSerialList.SubString(1,sSerialList.Pos(",")-1);
            sSerialList.Delete(1,sSerialList.Pos(","));
            lbSerial -> AddItem(sSerial , NULL);
        }
    }



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


    /*
    어떤 프로그램이.. C++ 빌더 IDE 에서 실행되고 있는지를 알아 내려면...
상용 컴포넌트 제작사들이 하는 식으로... 패런트 프로세스 아이디를 알아내서...
그 프로세스가 "BDS.EXE"인지를 확인하는 게 가장 효과적인 방법임...
    http://cbuilder.borlandforum.com/impboard/impboard.dll?action=read&db=bcb_qna&no=68495
    */
    bool bTemp = IsDebuggerPresent() ;
    bool bRet = (sSerialList.Pos(sSerial+",") || IsDebuggerPresent()) ;
    return bRet ;
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

