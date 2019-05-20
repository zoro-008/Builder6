//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "DataMan.h"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
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
    //ClearMap();



    tmUpdate = new TTimer(Application);
    tmUpdate -> Interval = 200  ;
    tmUpdate -> OnTimer  = tmUpdateTimer ;
    tmUpdate -> Enabled  = false ;  //���� ����.

//    AxtParaUi.edPhysicalNo   = new TEdit    (AxtParaUi.pnBase); AxtParaUi.edPhysicalNo   -> Parent = AxtParaUi.pnBase ;
}
//---------------------------------------------------------------------------
__fastcall CDataMan::~CDataMan(void)
{
}

void __fastcall CDataMan::Close()
{
    Trace( "", "DataMan Close") ;
    SaveMap();
}

void __fastcall CDataMan::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
    for(int i = 0 ; i < MAX_ARAY ; i++) {
        ARAY[i].UpdateAray();
    }

    tmUpdate -> Enabled = true ;
}

//---------------------------------------------------------------------------
void __fastcall CDataMan::ClearMap(void)
{
    for (register int t = 0 ; t < MAX_ARAY ; t++) ARAY[t].ClearMap() ;
}

//---------------------------------------------------------------------------
void __fastcall CDataMan::ShiftArrayData(EN_ARAY_ID _iSrcId , EN_ARAY_ID _iDstId)
{
    if(ARAY[_iSrcId].GetMaxCol() != ARAY[_iDstId].GetMaxCol()) return ;
    if(ARAY[_iSrcId].GetMaxRow() != ARAY[_iDstId].GetMaxRow()) return ;

    //for (register int r = 0 ; r < ARAY[_iSrcId].GetMaxRow()  ; r++) {
    //    for (register int c = 0 ; c < ARAY[_iSrcId].GetMaxCol() ; c++) {
    //        ARAY[_iDstId].SetStat(r,c,ARAY[_iSrcId].GetStat(r,c)) ;
    //    }
    //}

    ARAY[_iDstId] = ARAY[_iSrcId] ;
    ARAY[_iDstId].SetStep   (0     ); //������ ���� �ִ� �����ǿ����� �ǹ� �����Ƿ� �Ѿ� ���� Ŭ���� ��Ų��.
    ARAY[_iDstId].SetSubStep(0     ); //������ ���� �ִ� �����ǿ����� �ǹ� �����Ƿ� �Ѿ� ���� Ŭ���� ��Ų��.
    //Clear Source Tray.
    ARAY[_iSrcId].SetStat   (csNone);
    ARAY[_iSrcId].SetID     (""    );
    ARAY[_iSrcId].SetLotNo  (""    );
    ARAY[_iSrcId].SetStep   (0     );
    ARAY[_iSrcId].SetSubStep(0     );
    ARAY[_iSrcId].SetCheck  (false ); //Ver 1.0.2.3
}

//---------------------------------------------------------------------------
void __fastcall CDataMan::LoadMap()
{
    //Read&Write.
    for (int i = 0 ; i < MAX_ARAY ; i++)
        ARAY[i].Load(true);
}

//---------------------------------------------------------------------------
void __fastcall CDataMan::SaveMap()
{
    //Read&Write.
    for (int i = 0 ; i < MAX_ARAY ; i++) {
        ARAY[i].Load(false);
    }
}


