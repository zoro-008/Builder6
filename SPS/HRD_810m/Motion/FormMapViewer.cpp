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
         if(iMapType == 0) odMap -> Filter = "모든파일|*.*|sinf형식|*.sinf|wmap_a형식|*.wmap_a|xml형식|*.xml" ;  //Text
    else if(iMapType == 1) odMap -> Filter = "wmap형식|*.wmap|모든파일|*.*|" ;
    else if(iMapType == 2) odMap -> Filter = "log2형식|*.log2|모든파일|*.*|" ;
    else if(iMapType == 3) odMap -> Filter = "모든파일|*.*|sinf형식|*.sinf|wmap_a형식|*.wmap_a|xml형식|*.xml" ;  //hexa
    else if(iMapType == 4) odMap -> Filter = "w05형식|*.w05|모든파일|*.*|" ;

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
        if(str.Pos("0D0A")) { //줄바뀜
            Memo2 -> Lines -> Add(str.Trim());
            str = "";


        }

    }
    delete ms;



}
//---------------------------------------------------------------------------











