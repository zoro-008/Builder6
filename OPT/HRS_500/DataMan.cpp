//---------------------------------------------------------------------------
#pragma hdrstop

//---------------------------------------------------------------------------
#include "DataMan.h"
#include "SlogUnit.h"
#include "UserINI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CDataMan DM ;

/***************************************************************************/
/* 생성자 & 소멸자. (Constructor & Destructor)                             */
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
    ARAY[_iDstId].SetStep (0     ); //스텝은 원래 있던 포지션에서나 의미 있으므로 넘어 가면 클리어 시킨다.

    //Clear Source Tray.
    ARAY[_iSrcId].SetStat (csNone);
    ARAY[_iSrcId].SetID   (""    );
    ARAY[_iSrcId].SetLotNo(""    );
    ARAY[_iSrcId].SetStep (0     );
}

//---------------------------------------------------------------------------
void __fastcall CDataMan::LoadMap()
{
    //Make Dir.
    AnsiString  DevName ;
    AnsiString  BinPath ;
    AnsiString  IniPath ;
    AnsiString  sTemp1  ;
    AnsiString  sTemp2  ;

    //Read&Write.
    for (int i = 0 ; i < MAX_ARAY ; i++) {
        sTemp1 = (AnsiString)i+ AnsiString(".dat");
        sTemp2 = (AnsiString)i+ AnsiString(".ini");
        BinPath = EXE_FOLDER + "SeqData\\DataMap"+sTemp1 ;
        IniPath = EXE_FOLDER + "SeqData\\DataMap"+sTemp2 ;

        ARAY[i].Load(true , BinPath , IniPath , i);
    }

}

//---------------------------------------------------------------------------
void __fastcall CDataMan::SaveMap()
{
    //Make Dir.
    AnsiString  DevName ;
    AnsiString  BinPath ;
    AnsiString  IniPath ;
    AnsiString  sTemp1  ;
    AnsiString  sTemp2  ;

    //Read&Write.
    for (int i = 0 ; i < MAX_ARAY ; i++) {
        sTemp1 = (AnsiString)i+ AnsiString(".dat");
        sTemp2 = (AnsiString)i+ AnsiString(".ini");
        BinPath = EXE_FOLDER + "SeqData\\DataMap"+sTemp1 ;
        IniPath = EXE_FOLDER + "SeqData\\DataMap"+sTemp2 ;

        ARAY[i].Load(false , BinPath , IniPath , i);
    }
}

void __fastcall CDataMan::UpdateAray()
{
    for(int i  = 0 ; i < MAX_ARAY ; i++) {
        ARAY[i].UpdateAray();
    }
}



