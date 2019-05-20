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

//---------------------------------------------------------------------------
#pragma package(smart_init)

#define SPC_FOLDER "d:\\Spc\\"

#ifndef USE_SRIEAL_LOT
CLot LOT ;
#endif
CLot::CLot(void)
{


}

CLot::~CLot(void)
{


}

void CLot::Init()
{
    m_sLotNo      =  "" ;
    m_sJobName    =  "" ;
    Trace("SEQ","Lot Init"   );
    m_bLotOpen    = false ;
    m_bLotEnded   = false ;
    m_bRqstLotEnd = false ;

    LoadSave(true);
}

void CLot::Close()
{
    LoadSave(false);
}

void CLot::LoadSave(bool _bLoad)
{
    //Local Var.
    TUserINI   UserINI;
    String Path;

    //Make Dir.
    Path = ExtractFilePath(Application->ExeName) + "SeqData\\" + "LotInfo.ini";

    //Current Lot Informations.
    if(_bLoad) {
        Trace("SEQ","Lot LoadSave"   );
        UserINI.Load(Path.c_str() , "Member " , "m_bLotOpen   " , m_bLotOpen   );
        UserINI.Load(Path.c_str() , "Member " , "m_bLotEnded  " , m_bLotEnded  );
        UserINI.Load(Path.c_str() , "Member " , "m_bRqstLotEnd" , m_bRqstLotEnd);
        UserINI.Load(Path.c_str() , "Member " , "m_sLotNo     " , m_sLotNo     );
        UserINI.Load(Path.c_str() , "Member " , "m_sJobName   " , m_sJobName   );
    }
    else {
        UserINI.Save(Path.c_str() , "Member " , "m_bLotOpen   " , m_bLotOpen   );
        UserINI.Save(Path.c_str() , "Member " , "m_bLotEnded  " , m_bLotEnded  );
        UserINI.Save(Path.c_str() , "Member " , "m_bRqstLotEnd" , m_bRqstLotEnd);
        UserINI.Save(Path.c_str() , "Member " , "m_sLotNo     " , m_sLotNo     );
        UserINI.Save(Path.c_str() , "Member " , "m_sJobName   " , m_sJobName   );
    }
}

//Lot Processing.
void CLot::LotOpen(AnsiString _sLotNo , AnsiString _sJobName)
{
    Trace("SEQ",("Lot Open : " + _sLotNo).c_str()  );
    m_bLotOpen  = true ;

    m_sLotNo   = _sLotNo   ;
    m_sJobName = _sJobName ;
}

bool CLot::GetLotOpen()
{
    return m_bLotOpen ;
}

void CLot::Reset()
{
    m_bLotEnded = false ;

}

void CLot::LotEnd()
{
    //Check already opened Lot.
    Trace("SEQ","Lot Finished"   );
    //if (!m_bLotOpen) return;
    //Trace("SEQ","Lot Finished2"   );

    //Reset Lot Flag.
    m_bLotOpen    = false;
    m_bRqstLotEnd = false ;
    m_bLotEnded   = true ;


    FM_MsgOk("Confirm" , "LOT IS FINISHED"   );

}

bool CLot::GetLotEnd (void)
{
    //Check already opened Lot.
    return m_bLotEnded ;

}


/*
#ifdef USE_SRIEAL_LOT
CLotQue LTQ;
#endif
CLotQue::CLotQue(void)
{



}
CLotQue::~CLotQue(void)
{

}

void CLotQue::Init()
{
    //Init. Flag.
    m_bLotOpen    = false ;
    //m_bLotEnded   = false ;
    m_bRqstLotEnd = false ;

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
    String Path;

    //Make Dir.
    Path = ExtractFilePath(Application->ExeName) + "SeqData\\" + "LotInfo.ini";

    //Current Lot Informations.
    if(_bLoad) {
        UserINI.Load(Path.c_str() , "Member " , "m_bLotOpen   " , m_bLotOpen   );
        UserINI.Load(Path.c_str() , "Member " , "m_bLotEnded  " , m_bLotEnded  );
        UserINI.Load(Path.c_str() , "Member " , "m_sCrntLotNo " , m_sCrntLotNo );
        UserINI.Load(Path.c_str() , "Member " , "m_sAutoLotNo " , m_sAutoLotNo );
        //UserINI.Load(Path.c_str() , "Member " , "m_bRqstLotEnd" , m_bRqstLotEnd);
        for(int i = 0 ; i < MAX_LOT_CNT ; i++)
            UserINI.Load(Path.c_str() , "LotNoQue" , "m_sLotNo"+String(i) , m_sLotNo[i]);
    }
    else {
        UserINI.Save(Path.c_str() , "Member " , "m_bLotOpen   " , m_bLotOpen   );
        UserINI.Save(Path.c_str() , "Member " , "m_bLotEnded  " , m_bLotEnded  );
        UserINI.Save(Path.c_str() , "Member " , "m_sCrntLotNo " , m_sCrntLotNo );
        UserINI.Save(Path.c_str() , "Member " , "m_sAutoLotNo " , m_sAutoLotNo );
        //UserINI.Save(Path.c_str() , "Member " , "m_bRqstLotEnd" , m_bRqstLotEnd);
        for(int i = 0 ; i < MAX_LOT_CNT ; i++)
            UserINI.Save(Path.c_str() , "LotNoQue" , "m_sLotNo"+String(i) , m_sLotNo[i]);
    }
}

void CLotQue::PushLotNo (String _sLotNo)
{
   for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
       if(m_sLotNo[i] == "") {
           m_sLotNo[i] = _sLotNo ;
           return;
       }
   }
}

String CLotQue::GetLotNo (int _iNo) //������.
{
    return m_sLotNo[_iNo] ;
}

String CLotQue::PopLotNo (void )
{
   m_sCrntLotNo = m_sLotNo[0] ;
   int i;
   for(i = 0 ; i < MAX_LOT_CNT - 1 ; i++) {
       m_sLotNo[i] = m_sLotNo[i+1] ;
   }
   m_sLotNo[i] = "";
   return m_sCrntLotNo ;
}

void CLotQue::DeleteLotNo(String _sLotNo)
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
void CLotQue::LotOpen ()
{
    m_bLotOpen  = true ;
}

bool CLotQue::GetLotOpen()
{
    return m_bLotOpen ;
}

void CLotQue::LotEnd (void)
{
    //Check already opened Lot.
    Trace("SEQ","Lot Finished"   );
    //if (!m_bLotOpen) return;
    //Trace("SEQ","Lot Finished2"   );

    //Reset Lot Flag.
    m_bLotOpen  = false;
    m_bLotEnded = true ;
    m_sCrntLotNo = "" ;


    FM_MsgOk("Confirm" , "LOT IS FINISHED"   );
}

bool CLotQue::GetLotEnd (void)
{
    //Check already opened Lot.
    return m_bLotEnded ;

}

String CLotQue::GetCrntLotNo  (void )//��
{
    return m_sCrntLotNo ;
}

String CLotQue::GetAutoLotNo(void)
{
    return m_sAutoLotNo ;
}


void CLotQue::SetAutoLotNo (String _sAutoLotNo)
{
    m_sAutoLotNo = _sAutoLotNo ;
}
 */