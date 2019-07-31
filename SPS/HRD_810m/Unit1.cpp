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
        sId1 = _sBarcodeData.SubString(1, _sBarcodeData.Pos(",")-1);  // 20131227 ���ڵ� �ҷ� ���� �����ؼ� ������...
        sId2 = _sBarcodeData.SubString(_sBarcodeData.Pos(",")  , 20); // ���ڵ� ������ ���θ� �о� ������ ����...
        sId3 = _sBarcodeData.SubString(_sBarcodeData.Pos(",0") , 10); // �������� �߰��� Ư�� �κи� �о� ��...
        sId = sId1.Trim() + sId2.Trim() + sId3.Trim();                // ������ �̸��� ������ ���� �Ƚ� �Ȱ� �ƴ϶� ���Ŀ� ���� ���ɼ� ����..,JH
    }
    else{
        sId1 = _sBarcodeData.SubString(1, _sBarcodeData.Pos(",")-1);  // 20131227 ���ڵ� �ҷ� ���� �����ؼ� ������...
        sId2 = _sBarcodeData.SubString(_sBarcodeData.Pos(",")  , 20); // ���ڵ� ������ ���θ� �о� ������ ����...
        sId = sId1.Trim() + sId2.Trim();                              // ������ �̸��� ������ ���� �Ƚ� �Ȱ� �ƴ϶� ���Ŀ� ���� ���ɼ� ����..,JH
    }


    return sId.Trim();

    //AnsiString sId = _sBarcodeData ;
    //return sId.Trim() ;
}

AnsiString CStage::GetWaferIdx(AnsiString _sBarcodeData)
{
    _sBarcodeData.Delete(1,_sBarcodeData.Pos(","));
    _sBarcodeData.Delete(1,_sBarcodeData.Pos(","));

    AnsiString sIdx = _sBarcodeData; //50��° ���ڼ� ���� 50���� ���ڰ� ������ ���̵�.

    return sIdx.Trim();

}