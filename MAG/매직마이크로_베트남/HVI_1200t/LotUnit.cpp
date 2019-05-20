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
#include "Loader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define SPC_FOLDER "d:\\Spc\\"

AnsiString g_sLotNo[MAX_LOT_CNT] ;
void PushLotNo (AnsiString _sLotNo)
{
   for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
       if(g_sLotNo[i] == "") {
           g_sLotNo[i] = _sLotNo ;
           return;
       }
   }
}
//Add Push Top
void PushTopLotNo (AnsiString _sLotNo)
{
    for(int i = MAX_LOT_CNT  ; i > 0 ; i--) {
        g_sLotNo[i] = g_sLotNo[i-1] ;
    }
    g_sLotNo[0] = _sLotNo ;
    return ;
}

AnsiString GetLotNo (void ) //참조만.
{
    return g_sLotNo[0] ;
}

AnsiString PopLotNo (void )
{
   AnsiString sLotNo = g_sLotNo[0] ;
   for(int i = 0 ; i < MAX_LOT_CNT - 1 ; i++) {
       g_sLotNo[i] = g_sLotNo[i+1] ;
   }
   return sLotNo ;
}

void DeleteLotNo(AnsiString _sLotNo)
{
   for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
       if(_sLotNo == g_sLotNo[i]){
           g_sLotNo[i] = "" ;
           for(int j = i ; j < MAX_LOT_CNT - 1; i++) {
               g_sLotNo[j] = g_sLotNo[j+1] ;
           }
       }
   }
}

void DeleteLotNo(int _iLotNo)
{
   for(int i = _iLotNo ; i < MAX_LOT_CNT - 1 ; i++) {
       g_sLotNo[i] = g_sLotNo[i+1] ;
   }
}

void DeleteLotNoAll (void )
{
   for(int i = 0 ; i < MAX_LOT_CNT ; i++) {
       g_sLotNo[i] = "";
   }
}

int GetLotCnt(void)
{
   int i  ;
   for(i = 0 ; i < MAX_LOT_CNT ; i++) {
       if(g_sLotNo[i] == "") break ;
   }

   return i ;
}

CLot LT;

__fastcall CLot::CLot(void)
{
    //Init. Flag.
    m_bLotOpen  = false;

    //Init. Buffer.
    ClearEqpInfo();
    ClearLotInfo();
    ClearDayInfo();


    LoadEqpInfo();
    LoadLotInfo();
    LoadDayInfo();

    if(!FileExists(LOG_FOLDER)) CreateDir(LOG_FOLDER);
}
__fastcall CLot::~CLot(void)
{
//    SaveEqpInfo();
//    SaveLotInfo();
//    SaveDayInfo();
//    Trace("","");
}
void __fastcall CLot::Close(void)
{
    SaveEqpInfo();
    SaveLotInfo();
    SaveDayInfo();
}
void __fastcall CLot::ClearEqpInfo(void)
{
    memset(&EqpInfo , 0 , sizeof(SEqpInfo));
}

void __fastcall CLot::ClearLotInfo(void)
{
    memset(&LotInfo , 0 , sizeof(SLotInfo));

}
void __fastcall CLot::ClearDayInfo(void)
{
    memset(&DayInfo , 0 , sizeof(SDayInfo));
}

//Lot Processing.
void __fastcall CLot::LotOpen (AnsiString _sLotNo , AnsiString _sJobFile , AnsiString _sOperator)
{
    AnsiString sTempJobFile  = LotInfo.sJobFile  ;
    AnsiString sTempOperator = LotInfo.sOperator ;
    ClearLotInfo();

    LotInfo.sLotNo     = _sLotNo    ;
    if(_sJobFile == "") LotInfo.sJobFile   = sTempJobFile  ;
    else                LotInfo.sJobFile   = _sJobFile     ;
    if(_sOperator== "") LotInfo.sOperator  = sTempOperator ;
    else                LotInfo.sOperator  = _sOperator    ;

    LotInfo.iCrntLotNo = 0 ;

    LotInfo.dStartTime = Now();
    Trace("LotOpen",_sLotNo.c_str());

    //for(int i = 0 ; i < MAX_ARAY ; i++) {
    //    DM.ARAY[i].SetStat(csEmpty) ;
    //}

    //Set Lot Open Flag.
    m_bLotOpen  = true ;
}

AnsiString __fastcall CLot::GetCrntLot ()
{
    return LotInfo.sLotNo ;

}


void __fastcall CLot::LotEnd (void)
{
    //Check already opened Lot.
    if (!m_bLotOpen) return;

    //Set EndTime.
    LotInfo.dEndTime = Now();

    //Reset Lot Flag.
    m_bLotOpen = false;
    m_bLotEnd  = true ;

//    LDR._iMgzCnt = 0 ; //안대서 추가 하긴 하는데 이게 여기가 맞는지 모르겟넹.

    //WriteLotLog();

    //Check Retest Mode.
    Trace("SEQ","Lot Work Finished"   );
    FM_MsgOk("Confirm" , "LOT IS FINISHED"   );

    if(DM.ARAY[riLDR].CheckAllStat(csNone) &&
       DM.ARAY[riPRB].CheckAllStat(csNone) &&
       DM.ARAY[riWR1].CheckAllStat(csNone) &&
       DM.ARAY[riWR2].CheckAllStat(csNone) &&
       DM.ARAY[riWR3].CheckAllStat(csNone) &&
       DM.ARAY[riPSB].CheckAllStat(csNone) &&
       DM.ARAY[riULD].CheckAllStat(csNone)){
       LDR.SetMgzCnt(0);
    }

//       DM.ARAY[riULR


    //여기서 하면 부져가 안울림 그래서 리셑에 역음.
    //m_bLotEnd = false ;

}

void __fastcall CLot::DispEqpInfo (TPanel * _pnTotalCnt ,
                                   TPanel * _pnFailCnt  ,
                                   TPanel * _pnGoodCnt  ,
                                   TPanel * _pnYield    )
{
    if(_pnTotalCnt) _pnTotalCnt -> Caption = EqpInfo.iTotalChip     ;
    if(_pnFailCnt ) _pnFailCnt  -> Caption = EqpInfo.iTotalFailChip ;
    if(_pnGoodCnt ) _pnGoodCnt  -> Caption = EqpInfo.iTotalChip - EqpInfo.iTotalFailChip ;

    float dTemp = (EqpInfo.iTotalChip - EqpInfo.iTotalFailChip)* 100/(float)EqpInfo.iTotalChip ;
    dTemp       = ((int)dTemp)/100.0 ;
    if(_pnYield   ) _pnYield    -> Caption = dTemp    ;
}

void __fastcall CLot::DispLotInfo(TPanel * _pnLotNo    ,
                                  TPanel * _pnJobFile  ,
                                  TPanel * _pnOperator ,
                                  TPanel * _pnStartTime,
                                  TPanel * _pnEndTime  ,
                                  TPanel * _pnRunTime  ,
                                  TPanel * _pnIdleTime ,
                                  TPanel * _pnJamTime  ,
                                  TPanel * _pnTotalTime,
                                  TPanel * _pnStrpUPEH ,
                                  TPanel * _pnChipUPEH ,
                                  TPanel * _pnStrpUPH  ,
                                  TPanel * _pnChipUPH  ,
                                  TPanel * _pnWorkStrp ,
                                  TPanel * _pnWorkChip ,
                                  TPanel * _pnFailChip )
{
    TDateTime tTemp ;

    if(_pnLotNo   ) _pnLotNo     -> Caption = LotInfo.sLotNo     ;
    if(_pnJobFile ) _pnJobFile   -> Caption = LotInfo.sJobFile   ;
    if(_pnOperator) _pnOperator  -> Caption = LotInfo.sOperator  ;
    tTemp.Val = LotInfo.dStartTime ; if(_pnStartTime) _pnStartTime -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dEndTime   ; if(_pnEndTime  ) _pnEndTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dRunTime   ; if(_pnRunTime  ) _pnRunTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dIdleTime  ; if(_pnIdleTime ) _pnIdleTime  -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dJamTime   ; if(_pnJamTime  ) _pnJamTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = LotInfo.dTotalTime ; if(_pnTotalTime) _pnTotalTime -> Caption = tTemp.FormatString("hh:nn:ss") ;
    if(_pnStrpUPEH) _pnStrpUPEH -> Caption = LotInfo.iStrpUPEH  ;
    if(_pnChipUPEH) _pnChipUPEH -> Caption = LotInfo.iChipUPEH  ;
    if(_pnStrpUPH ) _pnStrpUPH  -> Caption = LotInfo.iStrpUPH   ;
    if(_pnChipUPH ) _pnChipUPH  -> Caption = LotInfo.iChipUPH   ;
    if(_pnWorkStrp) _pnWorkStrp -> Caption = LotInfo.iWorkStrp  ;
    if(_pnWorkChip) _pnWorkChip -> Caption = LotInfo.iWorkChip  ;
    if(_pnFailChip) _pnFailChip -> Caption = LotInfo.iFailChip  ;

}

void __fastcall CLot::DispDayInfo(TPanel * _pnRunTime   ,
                                  TPanel * _pnIdleTime  ,
                                  TPanel * _pnJamTime   ,
                                  TPanel * _pnTotalTime ,
                                  TPanel * _pnStrpUPEH  ,
                                  TPanel * _pnChipUPEH  ,
                                  TPanel * _pnStrpUPH   ,
                                  TPanel * _pnChipUPH   ,
                                  TPanel * _pnWorkStrp  ,
                                  TPanel * _pnWorkChip  ,
                                  TPanel * _pnFailChip  ,
                                  TPanel * _pnWorkLot   )
{
    TDateTime tTemp ;

    tTemp.Val = DayInfo.dRunTime   ; if(_pnRunTime  ) _pnRunTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = DayInfo.dIdleTime  ; if(_pnIdleTime ) _pnIdleTime  -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = DayInfo.dJamTime   ; if(_pnJamTime  ) _pnJamTime   -> Caption = tTemp.FormatString("hh:nn:ss") ;
    tTemp.Val = DayInfo.dTotalTime ; if(_pnTotalTime) _pnTotalTime -> Caption = tTemp.FormatString("hh:nn:ss") ;
    if(_pnStrpUPEH) _pnStrpUPEH -> Caption = DayInfo.iStrpUPEH  ;
    if(_pnChipUPEH) _pnChipUPEH -> Caption = DayInfo.iChipUPEH  ;
    if(_pnStrpUPH ) _pnStrpUPH  -> Caption = DayInfo.iStrpUPH   ;
    if(_pnChipUPH ) _pnChipUPH  -> Caption = DayInfo.iChipUPH   ;
    if(_pnWorkStrp) _pnWorkStrp -> Caption = DayInfo.iWorkStrp  ;
    if(_pnWorkChip) _pnWorkChip -> Caption = DayInfo.iWorkChip  ;
    if(_pnFailChip) _pnFailChip -> Caption = DayInfo.iFailChip  ;
    if(_pnWorkLot ) _pnWorkLot  -> Caption = DayInfo.iLotCnt    ;
}

void __fastcall CLot::LoadEqpInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "EqpInfo.ini";

    //Current Lot Informations.
    UserINI.Load(Path.c_str() , "LotInfo" , "iTotalChip    " , EqpInfo.iTotalChip    );
    UserINI.Load(Path.c_str() , "LotInfo" , "iTotalFailChip" , EqpInfo.iTotalFailChip);
}

void __fastcall CLot::SaveEqpInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "EqpInfo.ini";

    //Current Lot Informations.
    UserINI.Save(Path.c_str() , "LotInfo" , "iTotalChip    " , EqpInfo.iTotalChip    );
    UserINI.Save(Path.c_str() , "LotInfo" , "iTotalFailChip" , EqpInfo.iTotalFailChip);

}

//File Processing.
void __fastcall CLot::LoadLotInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "LotInfo.ini";

    //Current Lot Informations.
    UserINI.Load(Path.c_str() , "Member " , "m_bLotOpen  " , m_bLotOpen          );
    UserINI.Load(Path.c_str() , "LotInfo" , "sLotNo      " , LotInfo.sLotNo      );
    UserINI.Load(Path.c_str() , "LotInfo" , "sJobFile    " , LotInfo.sJobFile    );
    UserINI.Load(Path.c_str() , "LotInfo" , "iLotMgzCnt  " , LotInfo.iLotMgzCnt  );
    UserINI.Load(Path.c_str() , "LotInfo" , "sOperator   " , LotInfo.sOperator   );
    UserINI.Load(Path.c_str() , "LotInfo" , "dStartTime  " , LotInfo.dStartTime  );
    UserINI.Load(Path.c_str() , "LotInfo" , "dEndTime    " , LotInfo.dEndTime    );
    UserINI.Load(Path.c_str() , "LotInfo" , "dRunTime    " , LotInfo.dRunTime    );
    UserINI.Load(Path.c_str() , "LotInfo" , "dIdleTime   " , LotInfo.dIdleTime   );
    UserINI.Load(Path.c_str() , "LotInfo" , "dJamTime    " , LotInfo.dJamTime    );
    UserINI.Load(Path.c_str() , "LotInfo" , "dTotalTime  " , LotInfo.dTotalTime  );
    UserINI.Load(Path.c_str() , "LotInfo" , "iStrpUPEH   " , LotInfo.iStrpUPEH   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iChipUPEH   " , LotInfo.iChipUPEH   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iStrpUPH    " , LotInfo.iStrpUPH    );
    UserINI.Load(Path.c_str() , "LotInfo" , "iChipUPH    " , LotInfo.iChipUPH    );
    UserINI.Load(Path.c_str() , "LotInfo" , "iWorkStrp   " , LotInfo.iWorkStrp   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iWorkChip   " , LotInfo.iWorkChip   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iFailChip   " , LotInfo.iFailChip   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRslt0   " , LotInfo.iCntRslt0   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRslt1   " , LotInfo.iCntRslt1   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRslt2   " , LotInfo.iCntRslt2   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRslt3   " , LotInfo.iCntRslt3   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRslt4   " , LotInfo.iCntRslt4   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRslt5   " , LotInfo.iCntRslt5   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRslt6   " , LotInfo.iCntRslt6   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRslt7   " , LotInfo.iCntRslt7   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRslt8   " , LotInfo.iCntRslt8   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRslt9   " , LotInfo.iCntRslt9   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltA   " , LotInfo.iCntRsltA   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltB   " , LotInfo.iCntRsltB   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltC   " , LotInfo.iCntRsltC   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltD   " , LotInfo.iCntRsltD   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltE   " , LotInfo.iCntRsltE   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltF   " , LotInfo.iCntRsltF   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltG   " , LotInfo.iCntRsltG   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltH   " , LotInfo.iCntRsltH   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltI   " , LotInfo.iCntRsltI   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltJ   " , LotInfo.iCntRsltJ   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltK   " , LotInfo.iCntRsltK   );
    UserINI.Load(Path.c_str() , "LotInfo" , "iCntRsltL   " , LotInfo.iCntRsltL   );

//    UserINI.Load(Path.c_str() , "LotInfo" , "iCntHeatFail" , LotInfo.iCntHeatFail);
}

void __fastcall CLot::SaveLotInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "LotInfo.ini";

    if(LotInfo.sJobFile =="") LotInfo.sJobFile = OM.GetCrntDev() ;  /*sun*/

    //Current Lot Informations.
    UserINI.Save(Path.c_str() , "Member " , "m_bLotOpen  " , m_bLotOpen          );
    UserINI.Save(Path.c_str() , "LotInfo" , "sLotNo      " , LotInfo.sLotNo      );
    UserINI.Save(Path.c_str() , "LotInfo" , "sJobFile    " , LotInfo.sJobFile    );
    UserINI.Save(Path.c_str() , "LotInfo" , "iLotMgzCnt  " , LotInfo.iLotMgzCnt  );
    UserINI.Save(Path.c_str() , "LotInfo" , "sOperator   " , LotInfo.sOperator   );
    UserINI.Save(Path.c_str() , "LotInfo" , "dStartTime  " , LotInfo.dStartTime  );
    UserINI.Save(Path.c_str() , "LotInfo" , "dEndTime    " , LotInfo.dEndTime    );
    UserINI.Save(Path.c_str() , "LotInfo" , "dRunTime    " , LotInfo.dRunTime    );
    UserINI.Save(Path.c_str() , "LotInfo" , "dIdleTime   " , LotInfo.dIdleTime   );
    UserINI.Save(Path.c_str() , "LotInfo" , "dJamTime    " , LotInfo.dJamTime    );
    UserINI.Save(Path.c_str() , "LotInfo" , "dTotalTime  " , LotInfo.dTotalTime  );
    UserINI.Save(Path.c_str() , "LotInfo" , "iStrpUPEH   " , LotInfo.iStrpUPEH   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iChipUPEH   " , LotInfo.iChipUPEH   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iStrpUPH    " , LotInfo.iStrpUPH    );
    UserINI.Save(Path.c_str() , "LotInfo" , "iChipUPH    " , LotInfo.iChipUPH    );
    UserINI.Save(Path.c_str() , "LotInfo" , "iWorkStrp   " , LotInfo.iWorkStrp   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iWorkChip   " , LotInfo.iWorkChip   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iFailChip   " , LotInfo.iFailChip   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRslt0   " , LotInfo.iCntRslt0   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRslt1   " , LotInfo.iCntRslt1   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRslt2   " , LotInfo.iCntRslt2   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRslt3   " , LotInfo.iCntRslt3   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRslt4   " , LotInfo.iCntRslt4   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRslt5   " , LotInfo.iCntRslt5   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRslt6   " , LotInfo.iCntRslt6   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRslt7   " , LotInfo.iCntRslt7   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRslt8   " , LotInfo.iCntRslt8   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRslt9   " , LotInfo.iCntRslt9   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltA   " , LotInfo.iCntRsltA   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltB   " , LotInfo.iCntRsltB   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltC   " , LotInfo.iCntRsltC   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltD   " , LotInfo.iCntRsltD   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltE   " , LotInfo.iCntRsltE   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltF   " , LotInfo.iCntRsltF   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltG   " , LotInfo.iCntRsltG   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltH   " , LotInfo.iCntRsltH   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltI   " , LotInfo.iCntRsltI   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltJ   " , LotInfo.iCntRsltJ   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltK   " , LotInfo.iCntRsltK   );
    UserINI.Save(Path.c_str() , "LotInfo" , "iCntRsltL   " , LotInfo.iCntRsltL   );


//    UserINI.Save(Path.c_str() , "LotInfo" , "iCntHeatFail" , LotInfo.iCntHeatFail);
}

void __fastcall CLot::LoadDayInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "DayInfo.ini";

    //Current Lot Informations.
    UserINI.Load(Path.c_str() , "DayInfo" , "dRunTime    " , DayInfo.dRunTime    );
    UserINI.Load(Path.c_str() , "DayInfo" , "dIdleTime   " , DayInfo.dIdleTime   );
    UserINI.Load(Path.c_str() , "DayInfo" , "dJamTime    " , DayInfo.dJamTime    );
    UserINI.Load(Path.c_str() , "DayInfo" , "dTotalTime  " , DayInfo.dTotalTime  );
    UserINI.Load(Path.c_str() , "DayInfo" , "iStrpUPEH   " , DayInfo.iStrpUPEH   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iChipUPEH   " , DayInfo.iChipUPEH   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iStrpUPH    " , DayInfo.iStrpUPH    );
    UserINI.Load(Path.c_str() , "DayInfo" , "iChipUPH    " , DayInfo.iChipUPH    );
    UserINI.Load(Path.c_str() , "DayInfo" , "iWorkStrp   " , DayInfo.iWorkStrp   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iWorkChip   " , DayInfo.iWorkChip   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iFailChip   " , DayInfo.iFailChip   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iLotCnt     " , DayInfo.iLotCnt     );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRslt0   " , DayInfo.iCntRslt0   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRslt1   " , DayInfo.iCntRslt1   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRslt2   " , DayInfo.iCntRslt2   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRslt3   " , DayInfo.iCntRslt3   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRslt4   " , DayInfo.iCntRslt4   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRslt5   " , DayInfo.iCntRslt5   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRslt6   " , DayInfo.iCntRslt6   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRslt7   " , DayInfo.iCntRslt7   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRslt8   " , DayInfo.iCntRslt8   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRslt9   " , DayInfo.iCntRslt9   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltA   " , DayInfo.iCntRsltA   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltB   " , DayInfo.iCntRsltB   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltC   " , DayInfo.iCntRsltC   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltD   " , DayInfo.iCntRsltD   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltE   " , DayInfo.iCntRsltE   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltF   " , DayInfo.iCntRsltF   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltG   " , DayInfo.iCntRsltG   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltH   " , DayInfo.iCntRsltH   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltI   " , DayInfo.iCntRsltI   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltJ   " , DayInfo.iCntRsltJ   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltK   " , DayInfo.iCntRsltK   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltL   " , DayInfo.iCntRsltL   );

//    UserINI.Load(Path.c_str() , "DayInfo" , "iCntHeatFail" , DayInfo.iCntHeatFail);
}

void __fastcall CLot::SaveDayInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Path;
    AnsiString TempStr ;
    TDateTime  TempTime;

    //Make Dir.
    Path = EXE_FOLDER + "SeqData\\" + "DayInfo.ini";

    //Current Lot Informations.
    UserINI.Save(Path.c_str() , "DayInfo" , "dRunTime    " , DayInfo.dRunTime    );
    UserINI.Save(Path.c_str() , "DayInfo" , "dIdleTime   " , DayInfo.dIdleTime   );
    UserINI.Save(Path.c_str() , "DayInfo" , "dJamTime    " , DayInfo.dJamTime    );
    UserINI.Save(Path.c_str() , "DayInfo" , "dTotalTime  " , DayInfo.dTotalTime  );
    UserINI.Save(Path.c_str() , "DayInfo" , "iStrpUPEH   " , DayInfo.iStrpUPEH   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iChipUPEH   " , DayInfo.iChipUPEH   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iStrpUPH    " , DayInfo.iStrpUPH    );
    UserINI.Save(Path.c_str() , "DayInfo" , "iChipUPH    " , DayInfo.iChipUPH    );
    UserINI.Save(Path.c_str() , "DayInfo" , "iWorkStrp   " , DayInfo.iWorkStrp   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iWorkChip   " , DayInfo.iWorkChip   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iFailChip   " , DayInfo.iFailChip   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iLotCnt     " , DayInfo.iLotCnt     );
    UserINI.Save(Path.c_str() , "DayInfo" , "iCntRslt0   " , DayInfo.iCntRslt0   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iCntRslt1   " , DayInfo.iCntRslt1   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iCntRslt2   " , DayInfo.iCntRslt2   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iCntRslt3   " , DayInfo.iCntRslt3   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iCntRslt4   " , DayInfo.iCntRslt4   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iCntRslt5   " , DayInfo.iCntRslt5   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iCntRslt6   " , DayInfo.iCntRslt6   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iCntRslt7   " , DayInfo.iCntRslt7   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iCntRslt8   " , DayInfo.iCntRslt8   );
    UserINI.Save(Path.c_str() , "DayInfo" , "iCntRslt9   " , DayInfo.iCntRslt9   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltA   " , DayInfo.iCntRsltA   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltB   " , DayInfo.iCntRsltB   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltC   " , DayInfo.iCntRsltC   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltD   " , DayInfo.iCntRsltD   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltE   " , DayInfo.iCntRsltE   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltF   " , DayInfo.iCntRsltF   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltG   " , DayInfo.iCntRsltG   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltH   " , DayInfo.iCntRsltH   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltI   " , DayInfo.iCntRsltI   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltJ   " , DayInfo.iCntRsltJ   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltK   " , DayInfo.iCntRsltK   );
    UserINI.Load(Path.c_str() , "DayInfo" , "iCntRsltL   " , DayInfo.iCntRsltL   );

//    UserINI.Save(Path.c_str() , "DayInfo" , "iCntHeatFail" , DayInfo.iCntHeatFail);
}
void __fastcall CLot::WriteLotDayLog()
{
    //Local Var.
    int        hFile   ;
    AnsiString Path    ;
    AnsiString Temp    ;
    TDateTime  CurrDateTime;

    DelPastLotLog();

    //Set Path.
    CurrDateTime = Now();
    Temp         = CurrDateTime.CurrentDate().FormatString("yyyymmdd");
    Path         = LOG_FOLDER ;//".csv";

    if(!UserFile.FileExist(Path)) CreateDir(Path);

    Path         = LOG_FOLDER + Temp + ".csv";
    UserFile.DeleteFiles(Path);
    //File Open.
    hFile = FileOpen(Path.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(Path.c_str());
        if (hFile == -1) return;
        Temp  = "Day(Time),"    ;
        Temp += "RunTime,"     ;
        Temp += "IdleTime,"    ;
        Temp += "JamTime,"     ;
        Temp += "TotalTime,"   ;
        Temp += "StrpUPEH,"    ;
        Temp += "ChipUPEH,"    ;
        Temp += "StrpUPH,"     ;
        Temp += "ChipUPH,"     ;
        Temp += "WorkStrp,"    ;
        Temp += "WorkChip,"    ;
        Temp += "FailChip,"    ;
        Temp += "LotCnt,"      ;
        Temp += OM.CmnOptn.sRsltName0 + ","; //CntRslt0,
        Temp += OM.CmnOptn.sRsltName1 + ","; //CntRslt1,
        Temp += OM.CmnOptn.sRsltName2 + ","; //CntRslt2,
        Temp += OM.CmnOptn.sRsltName3 + ","; //CntRslt3,
        Temp += OM.CmnOptn.sRsltName4 + ","; //CntRslt4,
        Temp += OM.CmnOptn.sRsltName5 + ","; //CntRslt5,
        Temp += OM.CmnOptn.sRsltName6 + ","; //CntRslt6,
        Temp += OM.CmnOptn.sRsltName7 + ","; //CntRslt7,
        Temp += OM.CmnOptn.sRsltName8 + ","; //CntRslt8,
        Temp += OM.CmnOptn.sRsltName9 + ","; //CntRslt9,
        Temp += OM.CmnOptn.sRsltNameA + ","; //CntRslt10,
        Temp += OM.CmnOptn.sRsltNameB + ","; //CntRslt11,
        Temp += OM.CmnOptn.sRsltNameC + ","; //CntRslt12,
        Temp += OM.CmnOptn.sRsltNameD + ","; //CntRslt13,
        Temp += OM.CmnOptn.sRsltNameE + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameF + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameG + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameH + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameI + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameJ + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameK + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameL + "\r\n"; //CntRslt14,

        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }

    TDateTime tTemp ;

    //Save.
    Temp  = CurrDateTime.FormatString("yy.mm.dd(hh:nn:ss)") + "," ;

    tTemp.Val = DayInfo.dRunTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = DayInfo.dIdleTime  ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = DayInfo.dJamTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = DayInfo.dTotalTime ; Temp += tTemp.FormatString("hh:nn:ss") + ",";

    Temp += (AnsiString)DayInfo.iStrpUPEH    + ",";
    Temp += (AnsiString)DayInfo.iChipUPEH    + ",";
    Temp += (AnsiString)DayInfo.iStrpUPH     + ",";
    Temp += (AnsiString)DayInfo.iChipUPH     + ",";
    Temp += (AnsiString)DayInfo.iWorkStrp    + ",";
    Temp += (AnsiString)DayInfo.iWorkChip    + ",";
    Temp += (AnsiString)DayInfo.iFailChip    + ",";
    Temp += (AnsiString)DayInfo.iLotCnt      + ",";
    Temp += (AnsiString)DayInfo.iCntRslt0    + ",";
    Temp += (AnsiString)DayInfo.iCntRslt1    + ",";
    Temp += (AnsiString)DayInfo.iCntRslt2    + ",";
    Temp += (AnsiString)DayInfo.iCntRslt3    + ",";
    Temp += (AnsiString)DayInfo.iCntRslt4    + ",";
    Temp += (AnsiString)DayInfo.iCntRslt5    + ",";
    Temp += (AnsiString)DayInfo.iCntRslt6    + ",";
    Temp += (AnsiString)DayInfo.iCntRslt7    + ",";
    Temp += (AnsiString)DayInfo.iCntRslt8    + ",";
    Temp += (AnsiString)DayInfo.iCntRslt9    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltA    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltB    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltC    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltD    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltE    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltF    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltG    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltH    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltI    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltJ    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltK    + ",";
    Temp += (AnsiString)DayInfo.iCntRsltL    + ",";

//    Temp += (AnsiString)DayInfo.iCntHeatFail + ",";
    Temp += (AnsiString)"\r\n"                    ;

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);
}

void __fastcall CLot::WriteLotLog()
{
    //Local Var.
    int        hFile   ;
    AnsiString Path    ;
    AnsiString Temp    ;
    TDateTime  CurrDateTime;

    DelPastLotLog();

    //Set Path.
    CurrDateTime = Now();
    Temp         = CurrDateTime.CurrentDate().FormatString("yyyymmdd");
    Path         = LOG_FOLDER + Temp ;//".csv";



    if(!UserFile.FileExist(Path))  {
        CreateDir(Path);

        ClearDayInfo() ;

        DayInfo.dRunTime    = LotInfo.dRunTime     ;
        DayInfo.dIdleTime   = LotInfo.dIdleTime    ;
        DayInfo.dJamTime    = LotInfo.dJamTime     ;
        DayInfo.dTotalTime  = LotInfo.dTotalTime   ;

        DayInfo.iStrpUPEH   = LotInfo.iStrpUPEH    ;
        DayInfo.iChipUPEH   = LotInfo.iChipUPEH    ;
        DayInfo.iStrpUPH    = LotInfo.iStrpUPH     ;
        DayInfo.iChipUPH    = LotInfo.iChipUPH     ;

        DayInfo.iWorkStrp   = LotInfo.iWorkStrp    ;
        DayInfo.iWorkChip   = LotInfo.iWorkChip    ;
        DayInfo.iFailChip   = LotInfo.iFailChip    ;

        DayInfo.iLotCnt     = 1                    ;

        DayInfo.iCntRslt0   = LotInfo.iCntRslt0    ;
        DayInfo.iCntRslt1   = LotInfo.iCntRslt1    ;
        DayInfo.iCntRslt2   = LotInfo.iCntRslt2    ;
        DayInfo.iCntRslt3   = LotInfo.iCntRslt3    ;
        DayInfo.iCntRslt4   = LotInfo.iCntRslt4    ;
        DayInfo.iCntRslt5   = LotInfo.iCntRslt5    ;
        DayInfo.iCntRslt6   = LotInfo.iCntRslt6    ;
        DayInfo.iCntRslt7   = LotInfo.iCntRslt7    ;
        DayInfo.iCntRslt8   = LotInfo.iCntRslt8    ;
        DayInfo.iCntRslt9   = LotInfo.iCntRslt9    ;
        DayInfo.iCntRsltA   = LotInfo.iCntRsltA    ;
        DayInfo.iCntRsltB   = LotInfo.iCntRsltB    ;
        DayInfo.iCntRsltC   = LotInfo.iCntRsltC    ;
        DayInfo.iCntRsltD   = LotInfo.iCntRsltD    ;
        DayInfo.iCntRsltE   = LotInfo.iCntRsltE    ;
        DayInfo.iCntRsltF   = LotInfo.iCntRsltF    ;
        DayInfo.iCntRsltG   = LotInfo.iCntRsltG    ;
        DayInfo.iCntRsltH   = LotInfo.iCntRsltH    ;
        DayInfo.iCntRsltI   = LotInfo.iCntRsltI    ;
        DayInfo.iCntRsltJ   = LotInfo.iCntRsltJ    ;
        DayInfo.iCntRsltK   = LotInfo.iCntRsltK    ;
        DayInfo.iCntRsltL   = LotInfo.iCntRsltL    ;


//        DayInfo.iCntHeatFail= LotInfo.iCntHeatFail ;
    }

    TDateTime tTemp ;
    String    sDate ;
    tTemp.Val = LotInfo.dStartTime ;
//    sDate = tTemp.FormatString("hh_nn_ss") ;

//    Path = LOG_FOLDER + Temp + "\\" + "(" +sDate + ")" + LotInfo.sLotNo + ".csv";
    Path = LOG_FOLDER + Temp + "\\" + LotInfo.sLotNo + ".csv";

    //File Open.
    hFile = FileOpen(Path.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(Path.c_str());
        if (hFile == -1) return;
        Temp  = "Day(Time),";
        Temp += "LotNo,"    ;
        Temp += "JobFile,"  ;
        Temp += "Operator," ;
        Temp += "StartTime,";
        Temp += "EndTime,"  ;
        Temp += "RunTime,"  ;
        Temp += "IdleTime," ;
        Temp += "JamTime,"  ;
        Temp += "TotalTime,";
        Temp += "StrpUPEH," ;
        Temp += "ChipUPEH," ;
        Temp += "StrpUPH,"  ;
        Temp += "ChipUPH,"  ;
        Temp += "WorkStrp," ;
        Temp += "WorkChip," ;
        Temp += "FailChip," ;

        Temp += OM.CmnOptn.sRsltName0 + ","; //CntRslt0,
        Temp += OM.CmnOptn.sRsltName1 + ","; //CntRslt1,
        Temp += OM.CmnOptn.sRsltName2 + ","; //CntRslt2,
        Temp += OM.CmnOptn.sRsltName3 + ","; //CntRslt3,
        Temp += OM.CmnOptn.sRsltName4 + ","; //CntRslt4,
        Temp += OM.CmnOptn.sRsltName5 + ","; //CntRslt5,
        Temp += OM.CmnOptn.sRsltName6 + ","; //CntRslt6,
        Temp += OM.CmnOptn.sRsltName7 + ","; //CntRslt7,
        Temp += OM.CmnOptn.sRsltName8 + ","; //CntRslt8,
        Temp += OM.CmnOptn.sRsltName9 + ","; //CntRslt9,
        Temp += OM.CmnOptn.sRsltNameA + ","; //CntRslt10,
        Temp += OM.CmnOptn.sRsltNameB + ","; //CntRslt11,
        Temp += OM.CmnOptn.sRsltNameC + ","; //CntRslt12,
        Temp += OM.CmnOptn.sRsltNameD + ","; //CntRslt13,
        Temp += OM.CmnOptn.sRsltNameE + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameF + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameG + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameH + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameI + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameJ + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameK + ","; //CntRslt14,
        Temp += OM.CmnOptn.sRsltNameL + "\r\n"; //CntRslt14,

        FileSeek (hFile , 0            , SEEK_END     );
        FileWrite(hFile , Temp.c_str() , Temp.Length());
    }

//    TDateTime tTemp ;

    //Save.
    Temp  = CurrDateTime.FormatString("yy.mm.dd(hh:nn:ss)") + "," ;
    Temp += (AnsiString)LotInfo.sLotNo       + ",";
    Temp += (AnsiString)LotInfo.sJobFile     + ",";
    Temp += (AnsiString)LotInfo.sOperator    + ",";

    tTemp.Val = LotInfo.dStartTime ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dEndTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dRunTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dIdleTime  ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dJamTime   ; Temp += tTemp.FormatString("hh:nn:ss") + ",";
    tTemp.Val = LotInfo.dTotalTime ; Temp += tTemp.FormatString("hh:nn:ss") + ",";

    Temp += (AnsiString)LotInfo.iStrpUPEH    + ",";
    Temp += (AnsiString)LotInfo.iChipUPEH    + ",";
    Temp += (AnsiString)LotInfo.iStrpUPH     + ",";
    Temp += (AnsiString)LotInfo.iChipUPH     + ",";
    Temp += (AnsiString)LotInfo.iWorkStrp    + ",";
    Temp += (AnsiString)LotInfo.iWorkChip    + ",";
    Temp += (AnsiString)LotInfo.iFailChip    + ",";

    Temp += (AnsiString)LotInfo.iCntRslt0    + ",";
    Temp += (AnsiString)LotInfo.iCntRslt1    + ",";
    Temp += (AnsiString)LotInfo.iCntRslt2    + ",";
    Temp += (AnsiString)LotInfo.iCntRslt3    + ",";
    Temp += (AnsiString)LotInfo.iCntRslt4    + ",";
    Temp += (AnsiString)LotInfo.iCntRslt5    + ",";
    Temp += (AnsiString)LotInfo.iCntRslt6    + ",";
    Temp += (AnsiString)LotInfo.iCntRslt7    + ",";
    Temp += (AnsiString)LotInfo.iCntRslt8    + ",";
    Temp += (AnsiString)LotInfo.iCntRslt9    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltA    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltB    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltC    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltD    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltE    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltF    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltG    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltH    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltI    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltJ    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltK    + ",";
    Temp += (AnsiString)LotInfo.iCntRsltL    + ",";

//    Temp += (AnsiString)LotInfo.iCntHeatFail + ",";

    Temp += (AnsiString)"\r\n"                    ;

    FileSeek (hFile , 0            , SEEK_END     );
    FileWrite(hFile , Temp.c_str() , Temp.Length());

    //Close File.
    FileClose(hFile);
}

void __fastcall CLot::DelPastLotLog()
{
    UserFile.ClearDirDate(LOG_FOLDER , Now() - 30);
}

//Update Lot Efficiency.
void __fastcall CLot::Update(EN_SEQ_STAT Stat)
{
    static int iCnt = 0 ;

    iCnt++;
//    if(iCnt < 100) return ;
    iCnt = 0 ;

    static TDateTime tPreTime = Now();
    TDateTime tCrntTime = Now();
    double dGapTime = tCrntTime.Val - tPreTime.Val ;

    switch(Stat) {
        case ssInit     : DayInfo.dIdleTime += dGapTime ; LotInfo.dIdleTime += dGapTime ; break ;
        case ssWarning  : DayInfo.dIdleTime += dGapTime ; LotInfo.dIdleTime += dGapTime ; break ;
        case ssError    : DayInfo.dJamTime  += dGapTime ; LotInfo.dJamTime  += dGapTime ; break ;
        case ssRunning  : DayInfo.dRunTime  += dGapTime ; LotInfo.dRunTime  += dGapTime ; break ;
        case ssStop     : DayInfo.dIdleTime += dGapTime ; LotInfo.dIdleTime += dGapTime ; break ;
        case ssMaint    : DayInfo.dIdleTime += dGapTime ; LotInfo.dIdleTime += dGapTime ; break ;
        case ssRunWarn  : DayInfo.dRunTime  += dGapTime ; LotInfo.dRunTime  += dGapTime ; break ;
        case ssWorkEnd  : DayInfo.dIdleTime += dGapTime ; LotInfo.dIdleTime += dGapTime ; break ;
    }

    double dTotalTime = LotInfo.dIdleTime + LotInfo.dJamTime + LotInfo.dRunTime ;

    LotInfo.dTotalTime += dGapTime ;
    DayInfo.dTotalTime += dGapTime ;


    //Lot Info
    if(LotInfo.dRunTime  ) {LotInfo.iChipUPH  = LotInfo.iWorkChip / (LotInfo.dRunTime * 24) ;}
    else                   {LotInfo.iChipUPH  = 0 ;                                          }
    if(LotInfo.dRunTime  ) {LotInfo.iStrpUPH  = LotInfo.iWorkStrp / (LotInfo.dRunTime * 24) ;}
    else                   {LotInfo.iStrpUPH  = 0 ;                                          }

    if(LotInfo.dTotalTime) {LotInfo.iChipUPEH = LotInfo.iWorkChip / (LotInfo.dTotalTime * 24) ;}
    else                   {LotInfo.iChipUPEH = 0 ;                                            }
    if(LotInfo.dTotalTime) {LotInfo.iStrpUPEH = LotInfo.iWorkStrp / (LotInfo.dTotalTime * 24) ;}
    else                   {LotInfo.iStrpUPEH = 0 ;                                            }

    //Day Info
    if(DayInfo.dRunTime)   {DayInfo.iChipUPH  = DayInfo.iWorkChip / (DayInfo.dRunTime * 24) ;}
    else                   {DayInfo.iChipUPH  = 0 ;                                          }
    if(DayInfo.dRunTime)   {DayInfo.iStrpUPH  = DayInfo.iWorkStrp / (DayInfo.dRunTime * 24) ;}
    else                   {DayInfo.iStrpUPH  = 0 ;                                          }

    if(DayInfo.dTotalTime) {DayInfo.iChipUPEH = DayInfo.iWorkChip / (DayInfo.dTotalTime * 24) ;}
    else                   {DayInfo.iChipUPEH = 0 ;                                            }
    if(DayInfo.dTotalTime) {DayInfo.iStrpUPEH = DayInfo.iWorkStrp / (DayInfo.dTotalTime * 24) ;}
    else                   {DayInfo.iStrpUPEH = 0 ;                                            }

    tPreTime = tCrntTime ;
}


void __fastcall CLot::UpdateDate(TStringGrid * _sgDate)
{
    UserFile.GridSearchDir(LOG_FOLDER , _sgDate , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}

void __fastcall CLot::UpdateLotName(TStringGrid * _sgDate , TStringGrid * _sgLot)
{
    AnsiString sDay ,sPath ;
    sDay = _sgDate->Cells[1][_sgDate -> Row] ;
    sPath = LOG_FOLDER + sDay + "\\" ;
    UserFile.GridSearchFile(sPath , _sgLot , 1 , true);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
}

void __fastcall CLot::DispLotDate(TStringGrid * _sgDate , TStringGrid * _sgLot , TStringGrid * _sgLotInfo)
{

    AnsiString sDay  = _sgDate -> Cells[1][ _sgDate -> Row] ;
    AnsiString sPath = LOG_FOLDER + sDay + "\\" + _sgLot -> Cells[1][ _sgLot -> Row];

    //StringGrid Clear
    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount         = 33  ;
    _sgLotInfo -> DefaultColWidth  = 80  ;
    _sgLotInfo -> DefaultRowHeight = 25  ;
    _sgLotInfo -> ColWidths[0]     = 30  ;
    _sgLotInfo -> ColWidths[1]     = 90  ;
    _sgLotInfo -> ColWidths[2]     = 210 ;

    AnsiString  strName, str = "";
    AnsiString  sRowStr = "";
    AnsiString  sItmStr = "";

    int hwnd, flen;
    int iRowCnt = 0 ;
    int iColCnt = 0 ;
    char    *pfbuf;

    hwnd = FileOpen(sPath.c_str(), fmOpenRead) ;
    if(hwnd == NULL) return;

    flen = FileSeek(hwnd,0,2);
    FileSeek(hwnd,0,0);
    pfbuf = new char[flen+1];
    memset(pfbuf , 0 , sizeof(char)*(flen+1));
    FileRead(hwnd, pfbuf, flen);
    FileClose(hwnd);

    str = pfbuf ;

    while (str.Pos("\r\n")) {
        str.Delete(str.Pos("\r\n") , 2 ) ;
        if(iRowCnt > 100 ) return ;
        iRowCnt++ ;
    }

    _sgLotInfo -> RowCount = iRowCnt ;

    str = pfbuf ;
    iRowCnt = 0 ;

    while (str.Pos("\r\n")) {
        sRowStr = str.SubString(1 , str.Pos("\r\n")) ;
        str.Delete(1,str.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = iRowCnt  ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }

        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt++;
    }
    _sgLotInfo -> RowCount = iRowCnt ;
}

void __fastcall CLot::DispLotDate(AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath ;
    AnsiString sFPath , sFile ,sData = "";
    AnsiString sTemp;
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen;
    char *pfbuf;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    //StringGrid Clear
    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount =  33 ;
    _sgLotInfo -> DefaultColWidth  = 80 ;
    _sgLotInfo -> DefaultRowHeight = 25 ;
    _sgLotInfo -> ColWidths[0] = 30  ;
    _sgLotInfo -> ColWidths[1] = 90  ;
    _sgLotInfo -> ColWidths[2] = 210 ;

    m_pLotFolder = new TStringGrid(Application);
    m_pLotFile   = new TStringGrid(Application);

    sPath = LOG_FOLDER ;
    UserFile.GridSearchDir(sPath , m_pLotFolder , 0 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
    iRowCnt = m_pLotFolder->RowCount ;

    for(int i=0 ; i<iRowCnt ; i++){
        if( m_pLotFolder->Cells[1][i] == sFDataTime ) iFCol = i ;
        if( m_pLotFolder->Cells[1][i] == sUDataTime ) iLCol = i ;
    }

    if(iFCol == -1    ) {
        for(int i=0 ; i<iRowCnt ; i++){
            if(m_pLotFolder->Cells[1][i].ToIntDef(0) > sFDataTime.ToIntDef(0) && m_pLotFolder->Cells[1][i].ToIntDef(0) <= sUDataTime.ToIntDef(0)) {
                iFCol = i ;
                break;
            }
        }
    }

    if(iLCol == -1    ) {
        for(int i=iRowCnt -1 ; i >= 1 ; i--){
            if(m_pLotFolder->Cells[1][i].ToIntDef(0) < sUDataTime.ToIntDef(0) && m_pLotFolder->Cells[1][i].ToIntDef(0) >= sFDataTime.ToIntDef(0)) {
                iLCol = i ;
                break;
            }
        }
    }

    if(iFCol == -1    ) return ;
    if(iLCol == -1    ) return ;

    AnsiString sTemp1,sTemp2;
    for(int i=iFCol ; i<=iLCol ; i++){
        sFPath= sPath + m_pLotFolder->Cells[1][i] + "\\";
        UserFile.GridSearchFile(sFPath , m_pLotFile , 1 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬
        for(int j=0 ; j<m_pLotFile->RowCount ; j++) {
//            sTemp1 = m_pLotFile  ->Cells[1][j]          ;
            sFile += sFPath + m_pLotFile  ->Cells[1][j] + ",";
        }

    }

//    for(int i=iFCol ; i<=iLCol ; i++){
    AnsiString sFileSub ;
    while(sFile.Pos(","))
    {
        sFileSub = sFile.SubString(1,sFile.Pos(",")-1);
        sFile.Delete(1, sFile.Pos(",")  );
        sFPath= sFileSub;

        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) return ;

        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);
        pfbuf = new char[flen+1];
        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sTemp = pfbuf ;
        sTemp.Delete(1,sTemp.Pos("\r\n")+1);
        sData += sTemp;
    }
//    }

//    int iRowCnt = 0, iColCnt =0 ;
    AnsiString sRowStr, sItmStr ;

    iRowCnt = 1;
    while(sData.Pos("\r\n")){
        sRowStr = sData.SubString(1, sData.Pos("\r\n")) ;
        sData.Delete(1,sData.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo->Cells[iColCnt][iRowCnt] = iRowCnt ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }
        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt ++ ;
    }
    _sgLotInfo -> RowCount = iRowCnt ;
    return ;

}

void __fastcall CLot::SaveLotData (AnsiString sFDataTime , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    TDateTime  CurrDateTime = Now();

    //Set Path.
    sTemp = "LotInfo" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + sFDataTime + "-" + sUDataTime + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    //Set Data.
    sData = sFDataTime + "," + sUDataTime + "," + CurrDateTime.CurrentDateTime().FormatString("yyyymmdd(hh:nn)") + "\r\n";

    for(int i=0; i<_sgLotInfo->RowCount; i++){
        for(int j=0; j<_sgLotInfo->ColCount; j++){
            sData += _sgLotInfo -> Cells[j][i] + "," ;
        }
        sData += "\r\n" ;
    }

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
    }

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;
}
void __fastcall CLot::DispLotDayDate (TStringGrid * _sgLotName , TStringGrid * _sgLotInfo)
{
    AnsiString sPath  = LOG_FOLDER ;
    AnsiString sFPath = LOG_FOLDER + _sgLotName -> Cells[1][ _sgLotName -> Row] ;

    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount = 29 ;
    _sgLotInfo -> DefaultColWidth  = 80 ;
    _sgLotInfo -> DefaultRowHeight = 25 ;
    _sgLotInfo -> ColWidths[0] = 30  ;
    _sgLotInfo -> ColWidths[1] = 90  ;
    _sgLotInfo -> ColWidths[2] = 90  ;

    AnsiString  strName, str = "";
    AnsiString  sRowStr = "";
    AnsiString  sItmStr = "";

    int hwnd, flen;
    int iRowCnt = 0 ;
    int iColCnt = 0 ;
    char    *pfbuf;

    hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
    if(hwnd == NULL) return;

    flen = FileSeek(hwnd,0,2);
    FileSeek(hwnd,0,0);
    pfbuf = new char[flen+1];
    memset(pfbuf , 0 , sizeof(char)*(flen+1));
    FileRead(hwnd, pfbuf, flen);
    FileClose(hwnd);

    str = pfbuf ;

    iRowCnt = 0 ;

    while (str.Pos("\r\n")) {
        sRowStr = str.SubString(1 , str.Pos("\r\n")) ;
        str.Delete(1,str.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = iRowCnt  ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }

        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt++;
    }
    _sgLotInfo -> RowCount = iRowCnt ;    
}
void __fastcall CLot::DispLotDayDate (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath;
    AnsiString sFPath , sFile ,sData = "";
    AnsiString sTemp;
    int iRowCnt , iColCnt ;
    int iR , iFCol = -1 , iLCol = -1 ;
    int hwnd, flen;
    char *pfbuf;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    for(int i =0 ; i < _sgLotInfo->ColCount ; i++ ) {
        for(int j =1 ; j < _sgLotInfo->RowCount ; j++ ) {
            _sgLotInfo -> Cells[i][j] = "" ;
        }
    }

    _sgLotInfo -> ColCount = 33 ;

    _sgLotInfo -> DefaultColWidth  = 80 ;
    _sgLotInfo -> DefaultRowHeight = 25 ;
    _sgLotInfo -> ColWidths[0] = 30  ;
    _sgLotInfo -> ColWidths[1] = 90  ;
    _sgLotInfo -> ColWidths[2] = 90  ;

    m_pLotFile   = new TStringGrid(Application);

    sPath = LOG_FOLDER ;

    UserFile.GridSearchFile(sPath , m_pLotFile , 0 , false);     // 디렉토리 읽어와서 날짜와 알파벳 순으로 정렬

    iRowCnt = m_pLotFile->RowCount ;
//    sFDataTime += ".csv" ;
//    sUDataTime += ".csv" ;

    for(int i=0 ; i<iRowCnt ; i++){
        if( m_pLotFile->Cells[1][i].SubString(0,8) == sFDataTime ) iFCol = i ;
        if( m_pLotFile->Cells[1][i].SubString(0,8) == sUDataTime ) iLCol = i ;
    }

    if(iFCol == -1    ) {
        for(int i=0 ; i<iRowCnt ; i++){
            sTemp = m_pLotFile->Cells[1][i].SubString(0,8);
            if(sTemp.ToIntDef(0) > sFDataTime.ToIntDef(0) && sTemp.ToIntDef(0) <= sUDataTime.ToIntDef(0)) {
                iFCol = i ;
                break;
            }
        }
    }

    if(iLCol == -1    ) {
        for(int i=iRowCnt -1 ; i >= 1 ; i--){
            sTemp = m_pLotFile->Cells[1][i].SubString(0,8);
            if(sTemp.ToIntDef(0) < sUDataTime.ToIntDef(0) && sTemp.ToIntDef(0) >= sFDataTime.ToIntDef(0)) {
                iLCol = i ;
                break;
            }
        }
    }

    if(iFCol == -1    ) return ;
    if(iLCol == -1    ) return ;

    AnsiString sTemp1,sTemp2;
    for(int i=iFCol ; i<=iLCol ; i++){
        sFPath= sPath + m_pLotFile->Cells[1][i] ;
        sFile += sFPath + ",";
    }

//    for(int i=iFCol ; i<=iLCol ; i++){
    AnsiString sFileSub ;
    while(sFile.Pos(","))
    {
        sFileSub = sFile.SubString(1,sFile.Pos(",")-1);
        sFile.Delete(1, sFile.Pos(",")  );
        sFPath= sFileSub;

        hwnd = FileOpen(sFPath.c_str(), fmOpenRead) ;
        if(hwnd == NULL) continue ;

        flen = FileSeek(hwnd,0,2);
        FileSeek(hwnd,0,0);
        pfbuf = new char[flen+1];
        memset(pfbuf , 0 , sizeof(char)*(flen+1));
        FileRead(hwnd, pfbuf, flen);
        FileClose(hwnd);

        sTemp = pfbuf ;
        sTemp.Delete(1,sTemp.Pos("\r\n")+1);
        sData += sTemp;
    }
//    }

//    int iRowCnt = 0, iColCnt =0 ;
    AnsiString sRowStr, sItmStr ;

    iRowCnt = 1;
    while(sData.Pos("\r\n")){
        sRowStr = sData.SubString(1, sData.Pos("\r\n")) ;
        sData.Delete(1,sData.Pos("\r\n")+1) ;
        iColCnt = 0 ;
        _sgLotInfo->Cells[iColCnt][iRowCnt] = iRowCnt ;
        while (sRowStr.Pos(",")){
            iColCnt++ ;
            sItmStr = sRowStr.SubString(1,sRowStr.Pos(",")-1) ;
            sRowStr.Delete(1,sRowStr.Pos(",")) ;
            _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;
        }
        iColCnt++ ;
        sItmStr = sRowStr.SubString(1,sRowStr.Pos("\r\n" - 1 )) ;
        sRowStr.Delete(1,sRowStr.Pos("\r\n" - 1 )) ;
        _sgLotInfo -> Cells[iColCnt][iRowCnt] = sItmStr ;

        iRowCnt ++ ;
    }
    _sgLotInfo -> RowCount = iRowCnt ;
    return ;
}
void __fastcall CLot::SaveLotDayData (AnsiString sFDataTime    , AnsiString sUDataTime , TStringGrid * _sgLotInfo)
{
    AnsiString sPath , sTemp , sData , sFPath;
    int hFile;
    int hwnd, flen;

    if(sFDataTime.ToIntDef(-1) > sUDataTime.ToIntDef(-2)) return ;

    TDateTime  CurrDateTime = Now();

    //Set Path.
    sTemp = "LotDayInfo" ;
    sPath = SPC_FOLDER + sTemp;//".csv";

    //Set File Path.
    sFPath = sPath + "\\" + sFDataTime + "-" + sUDataTime + ".csv";

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    //Set Data.
    sData = sFDataTime + "," + sUDataTime + "," + CurrDateTime.CurrentDateTime().FormatString("yyyymmdd(hh:nn)") + "\r\n";

    for(int i=0; i<_sgLotInfo->RowCount; i++){
        for(int j=0; j<_sgLotInfo->ColCount; j++){
            sData += _sgLotInfo -> Cells[j][i] + "," ;
        }
        sData += "\r\n" ;
    }

    hFile = FileOpen(sFPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFPath.c_str());
        if (hFile == -1) { Trace("Err",(sFPath + "is Can't made").c_str());  return ; }
    }

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return ;
}

bool __fastcall CLot::WriteArayData(CArray * riAray) //패스를 빼야 되나 ...매번 고쳐야 되자나...
{
    Trace("WriteArayData","1");
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  , sArayName ;
    AnsiString sLotNo , sMgzNo , sSlotNo;
    AnsiString sTemp  ;
    AnsiString sTemp2 ;
    AnsiString sRslt  ;
    TDateTime  CurrDateTime;
    int        iMgzNo , iSlotNo ;

    DelPastLotLog();

    if( riAray->GetLotNo() == "" ) sLotNo = "NoLot";
    else                           sLotNo = riAray->GetLotNo();

    iSlotNo = riAray->GetID().ToIntDef(9999)%100 ;
    sSlotNo.sprintf("%02d",iSlotNo);
    iMgzNo  = riAray->GetID().ToIntDef(9999)/100 ;


    sPath = LOG_FOLDER + CurrDateTime.CurrentDate().FormatString("yyyymmdd") + "\\" + sLotNo + "\\" + (String)iMgzNo ;

    if(!DirectoryExists(sPath)) UserFile.CreateDir(sPath.c_str());

    sPath = sPath + "\\" + sSlotNo + ".ini" ;

    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            sTemp2 = (int)riAray->GetStat(r,c) ;
            sTemp2 = sTemp2.sprintf("%02d",(int)riAray->GetStat(r,c)) + "_";
            sRslt += sTemp2 ;
        }
        sTemp.printf("R%02d", r);
        UserINI.Save(sPath.c_str()  , "Data"  , sTemp , sRslt );
    }

    UserINI.Save(sPath.c_str()  , "ETC"  , "ID"    , riAray->GetID().ToIntDef(9999) );
    UserINI.Save(sPath.c_str()  , "ETC"  , "LotNo" , riAray->GetLotNo()             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Row"   , OM.DevInfo.iRowCnt             );
    UserINI.Save(sPath.c_str()  , "ETC"  , "Col"   , OM.DevInfo.iColCnt             );

    return true ;
    Trace("WriteArayData","2");

}

bool __fastcall CLot::ReadArayData (AnsiString sPath , CArray * riAray) //매거진 1개 일때만 랏드당 매거진 2개 이상 고려 안함.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sTemp  ;
    AnsiString sVal   ;
    AnsiString sRslt  ;

    AnsiString sId    ;
    AnsiString sLotNo ;

    int iRow , iCol   ;

    AnsiString sTemp2= "" ;

    if(!FileExists(sPath)) return false ;

    UserINI.Load(sPath.c_str()  , "ETC"  , "ID"    , &sId    );
    UserINI.Load(sPath.c_str()  , "ETC"  , "LotNo" , &sLotNo );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Row"   , &iRow   );
    UserINI.Load(sPath.c_str()  , "ETC"  , "Col"   , &iCol   );

    riAray -> SetMaxColRow(iCol,iRow );

    for(int r = 0 ; r < iRow ; r++) {
        sTemp.printf("R%02d", r);
        UserINI.Load(sPath.c_str()  , "Data"  , sTemp , sRslt );
        for(int c = 0 ; c < iCol ; c++) {
            //sVal = "0x"+sRslt.SubString(c+1,1) ;
            sVal = sRslt.SubString(1,2) ;
            sRslt.Delete(1,3) ;
            riAray->SetStat(r,c,(EN_CHIP_STAT)StrToInt(sVal)  );
            sTemp2 = sVal+"," ;
        }
    }

    riAray->SetID   (sId   );
    riAray->SetLotNo(sLotNo);

    Trace("Rcv Array",sTemp2.c_str()) ;

    return true ;
}

