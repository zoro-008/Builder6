//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMapViewer.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMapViewer *FrmMapViewer;
//---------------------------------------------------------------------------
__fastcall TFrmMapViewer::TFrmMapViewer(TComponent* Owner)
        : TForm(Owner)
{
    Memo1 -> DoubleBuffered = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMapViewer::btLoadHexMapFileClick(TObject *Sender)
{
    int iMapType = OM.DevInfo.iMapType;
         if(iMapType == 0) odMap -> Filter = "�������|*.*|sinf����|*.sinf|wmap_a����|*.wmap_a|xml����|*.xml" ;  //Text
    else if(iMapType == 1) odMap -> Filter = "wmap����|*.wmap|�������|*.*|" ;
    else if(iMapType == 2) odMap -> Filter = "log2����|*.log2|�������|*.*|" ;
    else if(iMapType == 3) odMap -> Filter = "�������|*.*|sinf����|*.sinf|wmap_a����|*.wmap_a|xml����|*.xml" ;  //hexa
    else if(iMapType == 4) odMap -> Filter = "w05����|*.w05|�������|*.*|" ;

    if(!odMap->Execute()) return ;

    //Local Var.
    String sPath = odMap->FileName;

    Memo1 -> Lines -> LoadFromFile(sPath) ;
    Memo2 -> Lines -> Clear();


    TMemoryStream *ms = new TMemoryStream;

    ms -> LoadFromFile(sPath);
    ms -> Position = 0;

    AnsiString str = "";
    BYTE bOneChar ;

    //Memo1 -> ScrollBy()

    for(int i = 0; i < ms->Size ; i++){
        ms->Read(&bOneChar , 1);
        //str += bOneChar ;//"0x" + IntToHex(bOneChar,2) + " ";
        str += IntToHex(bOneChar,2) ;
        if(str.Pos("0D0A")) { //�ٹٲ�
            Memo2 -> Lines -> Add(str.Trim());
            str = "";


        }

    }
    delete ms;



}
//---------------------------------------------------------------------------











