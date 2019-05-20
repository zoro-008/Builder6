//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------

#include <stdio.h>

#include "LotUnit.h"
#include "SLogUnit.h"
#include "UserIni.h"
#include "UserFile.h"
#include "OptionMan.h"
#include "Timer.h"
#include "DataMan.h"
#include "Sequence.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define SPC_FOLDER "d:\\Spc\\"

CLotQue LTQ;
CLotQue::CLotQue(void)
{



}
CLotQue::~CLotQue(void)
{

}

void CLotQue::Init()
{
    //Init. Flag.
    m_bLotOpen  = false;

    LoadSave(true);

}

void CLotQue::Close()
{
    LoadSave(false);
}

void CLotQue::Reset()
{
    m_bLotEnded = false ;
}

void CLotQue::LoadSave(bool _bLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;

    //Make Dir.
    Path = ExtractFilePath(Application->ExeName) + "SeqData\\" + "LotInfo.ini";

    //Current Lot Informations.
    if(_bLoad) {
        UserINI.Load(Path.c_str() , "Member " , "m_bLotOpen " , m_bLotOpen);
        for(int i = 0 ; i < MAX_LOT_CNT ; i++)
            UserINI.Load(Path.c_str() , "LotNoQue" , "m_sLotNo"+AnsiString(i) , m_sLotNo[i]);
    }
    else {
        UserINI.Save(Path.c_str() , "Member " , "m_bLotOpen " , m_bLotOpen);
        for(int i = 0 ; i < MAX_LOT_CNT ; i++)
            UserINI.Save(Path.c_str() , "LotNoQue" , "m_sLotNo"+AnsiString(i) , m_sLotNo[i]);
    }




}

void CLotQue::PushLotNo (AnsiString _sLotNo)
{
   for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
       if(m_sLotNo[i] == "") {
           m_sLotNo[i] = _sLotNo ;
           return;
       }
   }
}

AnsiString CLotQue::GetLotNo (int _iNo) //ÂüÁ¶¸¸.
{
    AnsiString dd;

    dd = m_sLotNo[_iNo];
                              
    return m_sLotNo[_iNo] ;
}

AnsiString CLotQue::PopLotNo (void )
{
   AnsiString sLotNo = m_sLotNo[0] ;
   int i;
   for(i = 0 ; i < MAX_LOT_CNT - 1 ; i++) {
       m_sLotNo[i] = m_sLotNo[i+1] ;
   }
   m_sLotNo[i] = "";
   return sLotNo ;
}

void CLotQue::DeleteLotNo(AnsiString _sLotNo)
{
   for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
       if(_sLotNo == m_sLotNo[i]){
           m_sLotNo[i] = "" ;
           for(int j = i ; j < MAX_LOT_CNT - 1; i++) {
               m_sLotNo[j] = m_sLotNo[j+1] ;
           }
       }
   }
}

void CLotQue::DeleteLotNo(int _iLotNo)
{
   for(int i = _iLotNo ; i < MAX_LOT_CNT - 1 ; i++) {
       m_sLotNo[i] = m_sLotNo[i+1] ;
   }
}

void CLotQue::DeleteLotNoAll (void )
{
   for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
       m_sLotNo[i] = "";
   }
}

int CLotQue::GetLotCnt(void)
{
   int i  ;
   for(i = 0 ; i < MAX_LOT_CNT ; i++) {
       if(m_sLotNo[i] == "") break ;
   }

   return i ;
}

//Lot Processing.
void CLotQue::LotOpen (AnsiString _sLotNo , AnsiString _sJobFile , AnsiString _sOperator)
{

    
    //AnsiString sTempJobFile  = LotInfo.sJobFile  ;
    //AnsiString sTempOperator = LotInfo.sOperator ;
    //ClearLotInfo();
    //
    //LotInfo.sLotNo     = _sLotNo    ;
    //if(_sJobFile  == "" ) LotInfo.sJobFile  = sTempJobFile  ;
    //else                  LotInfo.sJobFile  = _sJobFile     ;
    //if(_sOperator == "" ) LotInfo.sOperator = sTempOperator ;
    //else                  LotInfo.sOperator = _sOperator    ;
    //
    //LotInfo.iCrntLotNo = 0 ;
    //
    //LotInfo.dStartTime = Now();

    Trace("SEQ","Lot Open"   );


    m_bLotOpen  = true ;
}

bool CLotQue::GetLotOpen()
{
    return m_bLotOpen ;
}

void CLotQue::LotEnd (void)
{
    //Check already opened Lot.
    if (!m_bLotOpen) return;

    //Reset Lot Flag.
    m_bLotOpen  = false;
    m_bLotEnded = true ;
    SPC.m_iUserCnt     = 0;
    SPC.m_iUserWorkCnt = 0;

    Trace("SEQ","Lot Finished"   );
    FM_MsgOk("Confirm" , "LOT IS FINISHED"   );


}


