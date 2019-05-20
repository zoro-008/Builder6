//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "DataMan.h"
#include "SlogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CDataMan DM ;

/***************************************************************************/
/* ������ & �Ҹ���. (Constructor & Destructor)                             */
/***************************************************************************/
//---------------------------------------------------------------------------
__fastcall CDataMan::CDataMan(void)
{
    //Clear Map.
    ClearMap();

    LoadMap();

    //tmUpdate = new TTimer(Application);
    //tmUpdate -> Interval = 200  ;
    //tmUpdate -> OnTimer  = tmUpdateTimer ;
    //tmUpdate -> Enabled  = true ;

//    AxtParaUi.edPhysicalNo   = new TEdit    (AxtParaUi.pnBase); AxtParaUi.edPhysicalNo   -> Parent = AxtParaUi.pnBase ;
}
//---------------------------------------------------------------------------
__fastcall CDataMan::~CDataMan(void)
{
    Trace( "", "~CDataMan") ;
//    tmUpdate -> Enabled  = false ;

//    Sleep(1000);

//    Application -> ProcessMessages() ;
//    if(tmUpdate)delete tmUpdate ;
    SaveMap();
}

//void __fastcall CDataMan::tmUpdateTimer(TObject *Sender)
//{
//    tmUpdate -> Enabled = false ;
//    for(int i = 0 ; i < MAX_ARAY ; i++) {
//        ARAY[i].UpdateAray();
//    }
//
//    tmUpdate -> Enabled = true ;
//}

//---------------------------------------------------------------------------
void __fastcall CDataMan::ClearMap(void)
{
    for (register int t = 0 ; t < MAX_ARAY ; t++) ARAY[t].ClearMap() ;
}

//---------------------------------------------------------------------------
void __fastcall CDataMan::ShiftArrayData(EN_ARAY_ID _iSrcId , EN_ARAY_ID _iDstId)
{
    if(ARAY[_iSrcId].GetMaxCol() != ARAY[_iSrcId].GetMaxCol()) return ;
    if(ARAY[_iSrcId].GetMaxRow() != ARAY[_iSrcId].GetMaxRow()) return ;

    //for (register int r = 0 ; r < ARAY[_iSrcId].GetMaxRow()  ; r++) {
    //    for (register int c = 0 ; c < ARAY[_iSrcId].GetMaxCol() ; c++) {
    //        ARAY[_iDstId].SetStat(r,c,ARAY[_iSrcId].GetStat(r,c)) ;
    //    }
    //}

    ARAY[_iDstId] = ARAY[_iSrcId] ;
    ARAY[_iDstId].SetStep (0     ); //������ ���� �ִ� �����ǿ����� �ǹ� �����Ƿ� �Ѿ� ���� Ŭ���� ��Ų��.

    //Clear Source Tray.
    ARAY[_iSrcId].SetStat (csNone);
    ARAY[_iSrcId].SetID   (""    );
    ARAY[_iSrcId].SetLotNo(""    );
    ARAY[_iSrcId].SetStep (0     );
}

//---------------------------------------------------------------------------
void __fastcall CDataMan::LoadMap()
{
    //Local Var.
    FILE       *fp;
    //Make Dir.
    AnsiString  DevName;
    AnsiString  Path =   ExtractFilePath(Application -> ExeName) + "SeqData\\DataMap.DAT";

    //File Open.
    fp = fopen(Path.c_str() , "rb");
    if (fp == NULL) {
        fp = fopen(Path.c_str() , "wb");
        return ;
    }

    //Read&Write.
    for (int i = 0 ; i < MAX_ARAY ; i++) ARAY[i].Load(true , fp , i);
    fclose(fp);

}

//---------------------------------------------------------------------------
void __fastcall CDataMan::SaveMap()
{
    //Local Var.
    FILE       *fp;
    //Make Dir.
    AnsiString  DevName;
    AnsiString  Path =   ExtractFilePath(Application -> ExeName) + "SeqData\\DataMap.DAT";

    //File Open.
    fp = fopen(Path.c_str() , "wb");
    if (fp == NULL) return;

    //Read&Write.
    for (int i = 0 ; i < MAX_ARAY ; i++) {
        ARAY[i].Load(false , fp , i);
    }
    fclose(fp);

}

void __fastcall CDataMan::UpdateAray()
{
    for(int i  = 0 ; i < MAX_ARAY ; i++) {
        ARAY[i].UpdateAray();
    }
}


