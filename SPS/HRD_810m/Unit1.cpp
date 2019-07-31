//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    String sBar = "C905512.005";
    AnsiString sTemp  = GetWaferId(sBar);
    AnsiString sTemp2 = GetWaferIdx(sBar);

}
//---------------------------------------------------------------------------

AnsiString CStage::GetWaferId(AnsiString _sBarcodeData)
{
    AnsiString sId1, sId2, sId3, sId;

    Trace("Barcode", _sBarcodeData.c_str());

    if(!OM.CmnOptn.bUse1stBarcode){
        //_sBarcodeData.Delete(1,_sBarcodeData.Pos(","));
        sId1 = _sBarcodeData.SubString(1, _sBarcodeData.Pos(",")-1);  // 20131227 바코드 불러 오기 실패해서 수정함...
        sId2 = _sBarcodeData.SubString(_sBarcodeData.Pos(",")  , 20); // 바코드 데이터 전부를 읽어 오도록 수정...
        sId3 = _sBarcodeData.SubString(_sBarcodeData.Pos(",0") , 10); // 기존에는 중간에 특정 부분만 읽어 옴...
        sId = sId1.Trim() + sId2.Trim() + sId3.Trim();                // 맵파일 이름에 형식이 아직 픽스 된게 아니라 추후에 수정 가능성 있음..,JH
    }
    else{
        sId1 = _sBarcodeData.SubString(1, _sBarcodeData.Pos(",")-1);  // 20131227 바코드 불러 오기 실패해서 수정함...
        sId2 = _sBarcodeData.SubString(_sBarcodeData.Pos(",")  , 20); // 바코드 데이터 전부를 읽어 오도록 수정...
        sId = sId1.Trim() + sId2.Trim();                              // 맵파일 이름에 형식이 아직 픽스 된게 아니라 추후에 수정 가능성 있음..,JH
    }


    return sId.Trim();

    //AnsiString sId = _sBarcodeData ;
    //return sId.Trim() ;
}

AnsiString CStage::GetWaferIdx(AnsiString _sBarcodeData)
{
    _sBarcodeData.Delete(1,_sBarcodeData.Pos(","));
    _sBarcodeData.Delete(1,_sBarcodeData.Pos(","));

    AnsiString sIdx = _sBarcodeData; //50번째 문자서 부터 50개의 문자가 웨이퍼 아이디.

    return sIdx.Trim();

}