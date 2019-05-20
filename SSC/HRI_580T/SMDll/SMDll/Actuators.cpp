//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Actuators.h"
#include "FormMsgOk.h"
#include "UserIni.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//Actuator Object.
CActuators AT;

CActuator::CActuator(int _iActrNo)
{
    m_iActrNo = _iActrNo ;

    m_dtFwdTimeOut.Clear();
    m_dtBwdTimeOut.Clear();
    m_dtFwdOnDelay.Clear();
    m_dtBwdOnDelay.Clear();

    memset(&Para , 0 , sizeof(SPara ));
    memset(&Stat , 0 , sizeof(SStat ));

    Para.iFwdXAdd = -1 ;
    Para.iBwdXAdd = -1 ;
    Para.iFwdYAdd = -1 ;
    Para.iBwdYAdd = -1 ;

    Para.iFwdTimeOut = 4000 ;
    Para.iBwdTimeOut = 4000 ;

    Load();

    Init();
}

CActuator::~CActuator()
{

}
void CActuator::Init ()
{
    //I/O.
    if      (Para.iFwdXAdd > -1) {Stat.bCmdPos = IO.GetX(Para.iFwdXAdd,true);}
    else if (Para.iBwdXAdd > -1) {Stat.bCmdPos =!IO.GetX(Para.iBwdXAdd,true);}
    else if (Para.iFwdYAdd > -1) {Stat.bCmdPos = IO.GetY(Para.iFwdYAdd,true);}
    else if (Para.iBwdYAdd > -1) {Stat.bCmdPos =!IO.GetY(Para.iBwdYAdd,true);}
}

void CActuator::Update ()
{
    //Local Var.
    bool bExistFwdX ;
    bool bExistBwdX ;
    bool bExistFwdY ;
    bool bExistBwdY ;
    int  iFwdX      ;
    int  iBwdX      ;
    int  iFwdY      ;
    int  iBwdY      ;
    bool bFwdX      ;
    bool bBwdX      ;
    bool bFwdY      ;
    bool bBwdY      ;

    bool bVtrlFwdX  ;
    bool bVtrlBwdX  ;
    bool bVtrlFwdY  ;
    bool bVtrlBwdY  ;

    bool bDelayedFwd ;
    bool bDelayedBwd ;

    //Set I/O ID.
    iFwdX = Para.iFwdXAdd ;
    iBwdX = Para.iBwdXAdd ;
    iFwdY = Para.iFwdYAdd ;
    iBwdY = Para.iBwdYAdd ;

    //Check SKIP.
    if (iFwdY < 0 && iBwdY < 0) {
        Reset();
        return ;
    }

    //Set exist ID.
    bExistFwdX = (iFwdX > -1) ;
    bExistBwdX = (iBwdX > -1) ;
    bExistFwdY = (iFwdY > -1) ;
    bExistBwdY = (iBwdY > -1) ;

    //I/O.
    if (bExistFwdX) bFwdX = IO.GetX(iFwdX); //Detect X/Y Val.
    if (bExistBwdX) bBwdX = IO.GetX(iBwdX);
    if (bExistFwdY) bFwdY = IO.GetY(iFwdY);
    if (bExistBwdY) bBwdY = IO.GetY(iBwdY);

    //Virtual Input Sensor
    if      (bExistFwdX) bVtrlFwdX =  bFwdX ;
    else if (bExistBwdX) bVtrlFwdX = !bBwdX ;
    else if (bExistFwdY) bVtrlFwdX =  bFwdY ;
    else if (bExistBwdY) bVtrlFwdX = !bBwdY ;
    else                 return             ;

    if      (bExistBwdX) bVtrlBwdX =  bBwdX ;
    else if (bExistFwdX) bVtrlBwdX = !bFwdX ;
    else if (bExistBwdY) bVtrlBwdX =  bBwdY ;
    else if (bExistFwdY) bVtrlBwdX = !bFwdY ;
    else                 return             ;

    //Virtual Output Sensor
    if (bExistFwdY) bVtrlFwdY =  bFwdY;
    else            bVtrlFwdY = !bBwdY;
    if (bExistBwdY) bVtrlBwdY =  bBwdY;
    else            bVtrlBwdY = !bFwdY;


    bDelayedFwd = m_dtFwdOnDelay.OnDelay(bVtrlFwdX , Para.iFwdOnDelay );
    bDelayedBwd = m_dtBwdOnDelay.OnDelay(bVtrlBwdX , Para.iBwdOnDelay );

    if(bDelayedFwd)Stat.bActPos = ccFwd ;
    if(bDelayedBwd)Stat.bActPos = ccBwd ;

    //OnDelay Timer.
    m_dtFwdTimeOut.OnDelay(Stat.bCmdPos == ccFwd && Stat.bActPos != ccFwd , Para.iFwdTimeOut);
    m_dtBwdTimeOut.OnDelay(Stat.bCmdPos == ccBwd && Stat.bActPos != ccBwd , Para.iBwdTimeOut);

    Stat.bErr = m_dtFwdTimeOut.m_bOut || m_dtBwdTimeOut.m_bOut ;
}

void CActuator::Reset()
{
    m_dtFwdTimeOut.Clear();
    m_dtBwdTimeOut.Clear();
}

AnsiString CActuator::GetName (void)
{
    return Para.sName ;
}

bool CActuator::Complete(bool _bCmd)
{
    if(Stat.bCmdPos != Stat.bActPos) return false ;
    return Stat.bCmdPos == _bCmd ;
}

bool CActuator::Complete()
{
    if(Stat.bCmdPos != Stat.bActPos) return false ;
    return true ;
}

bool CActuator::GetCmd(void)
{
    return Stat.bCmdPos ;
}

bool CActuator::GetAct(void)
{
    return Stat.bActPos ;
}

bool CActuator::Err(void)
{
    return Stat.bErr ;
}

bool CActuator::Move(bool _bCmd)
{
    Stat.bCmdPos = _bCmd ;

    if( _bCmd == ccFwd) {
        if(Para.iFwdYAdd>=0) IO.SetY(Para.iFwdYAdd , true );
        if(Para.iBwdYAdd>=0) IO.SetY(Para.iBwdYAdd , false);
    }
    else {
        if(Para.iFwdYAdd>=0) IO.SetY(Para.iFwdYAdd , false);
        if(Para.iBwdYAdd>=0) IO.SetY(Para.iBwdYAdd , true );
    }

    return Complete(_bCmd);
}

CActuator::SPara * CActuator::GetPara()
{
    //memcpy(&Para , &_Para , sizeof(SPara));
    return &Para ;
}

void CActuator::Load()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sItem;
    AnsiString sPath;

    //Set Path.
    sPath  = EXE_FOLDER + "Util\\Actuator.INI";

    if(!FileExists(sPath)) return ;

    //Load.
    sItem.printf("ACTUATOR(%03d)" , m_iActrNo);
    UserINI.Load(sPath.c_str() , "sName      " , sItem.c_str() , Para.sName      );
    UserINI.Load(sPath.c_str() , "sComment   " , sItem.c_str() , Para.sComment   );
    UserINI.Load(sPath.c_str() , "iFwdXAdd   " , sItem.c_str() , Para.iFwdXAdd   );
    UserINI.Load(sPath.c_str() , "iBwdXAdd   " , sItem.c_str() , Para.iBwdXAdd   );
    UserINI.Load(sPath.c_str() , "iFwdYAdd   " , sItem.c_str() , Para.iFwdYAdd   );
    UserINI.Load(sPath.c_str() , "iBwdYAdd   " , sItem.c_str() , Para.iBwdYAdd   );
    UserINI.Load(sPath.c_str() , "iFwdOnDelay" , sItem.c_str() , Para.iFwdOnDelay);
    UserINI.Load(sPath.c_str() , "iBwdOnDelay" , sItem.c_str() , Para.iBwdOnDelay);
    UserINI.Load(sPath.c_str() , "iFwdTimeOut" , sItem.c_str() , Para.iFwdTimeOut);
    UserINI.Load(sPath.c_str() , "iBwdTimeOut" , sItem.c_str() , Para.iBwdTimeOut);
}

void CActuator::Save()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sItem;
    AnsiString sPath;

    //Set Path.
    sPath  = EXE_FOLDER + "Util\\Actuator.INI";

    //Load.
    sItem.printf("ACTUATOR(%03d)" , m_iActrNo);
    UserINI.Save(sPath.c_str() , "sName      " , sItem.c_str() , Para.sName      );
    UserINI.Save(sPath.c_str() , "sComment   " , sItem.c_str() , Para.sComment   );
    UserINI.Save(sPath.c_str() , "iFwdXAdd   " , sItem.c_str() , Para.iFwdXAdd   );
    UserINI.Save(sPath.c_str() , "iBwdXAdd   " , sItem.c_str() , Para.iBwdXAdd   );
    UserINI.Save(sPath.c_str() , "iFwdYAdd   " , sItem.c_str() , Para.iFwdYAdd   );
    UserINI.Save(sPath.c_str() , "iBwdYAdd   " , sItem.c_str() , Para.iBwdYAdd   );
    UserINI.Save(sPath.c_str() , "iFwdOnDelay" , sItem.c_str() , Para.iFwdOnDelay);
    UserINI.Save(sPath.c_str() , "iBwdOnDelay" , sItem.c_str() , Para.iBwdOnDelay);
    UserINI.Save(sPath.c_str() , "iFwdTimeOut" , sItem.c_str() , Para.iFwdTimeOut);
    UserINI.Save(sPath.c_str() , "iBwdTimeOut" , sItem.c_str() , Para.iBwdTimeOut);

}

/***************************************************************************/
/* CActuator Array Class                                                   */
/***************************************************************************/
CActuators::CActuators(void)
{
    LoadDnmVar() ;

    m_bRpt = false ;

    Actuator = new CActuator * [m_iMaxAct] ;


    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i] = new CActuator(i) ;
    }

    memset(&RptPara , 0 , sizeof(SRptPara));

    m_bRpt = false ;

    Load();

}
CActuators::~CActuators(void)
{
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        delete Actuator[i] ;
        Actuator[i] = NULL ;
    }
    delete [] Actuator ;
    Actuator = NULL ;
    Trace("","");    
}

//basic funcions.
void CActuators::Reset(void)
{
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i] -> Reset();
    }
}

AnsiString CActuators::GetName(int _iActrNo)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return "Error" ;

    return Actuator[_iActrNo] -> GetName();

}

bool CActuators::Complete(int _iActrNo , bool _bCmd)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return false ;

    return Actuator[_iActrNo] -> Complete(_bCmd);
}

bool CActuators::Complete (int _iActrNo)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return false ;

    return Actuator[_iActrNo] -> Complete();
}

bool CActuators::Err(int _iActrNo)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return true ;

    return Actuator[_iActrNo] -> Err();
}

bool CActuators::Move(int _iActrNo , bool _bCmd)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return false ;

    return Actuator[_iActrNo] -> Move(_bCmd);
}

bool CActuators::GetCmd(int _iActrNo)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return false ;

    return Actuator[_iActrNo] -> GetCmd();

}

//Repeat Control.
void CActuators::GoRpt(int _iDelay , int _iActr1No , int _iActr2No)
{
    if(_iActr1No<0 || m_iMaxAct <= _iActr1No) { MsgOk("Error","실린더 범위가 벗어났습니다."); return ;}
    if(_iActr2No<0 || m_iMaxAct <= _iActr2No) { _iActr2No = -1 ;                                      }

    RptPara.iDelay    = _iDelay   ;
    RptPara.iFrstActr = _iActr1No ;
    RptPara.iScndActr = _iActr2No ;
    m_bRpt = true ;
}

bool CActuators::RptActr (void)
{
    //Check Motor Status.
    if (                            Err(RptPara.iFrstActr)){m_bRpt = false; MsgOk("Error" , "실린더 알람중입니다."  ); }
    if ( RptPara.iScndActr != -1 && Err(RptPara.iScndActr)){m_bRpt = false; MsgOk("Error" , "실린더 알람중입니다."  ); }

    //Changing Timer.
    if (RptPara.iScndActr != -1) {
        if (Actuator[RptPara.iFrstActr] -> GetCmd() != Actuator[RptPara.iScndActr] -> GetCmd()){Move(RptPara.iFrstActr , ccBwd); Move(RptPara.iScndActr , ccBwd);}
        if (m_dtRptFwd.OnDelay(Complete(RptPara.iFrstActr , ccFwd)&&Complete(RptPara.iScndActr , ccFwd) , RptPara.iDelay )) {Move(RptPara.iFrstActr , ccBwd); Move(RptPara.iScndActr , ccBwd);}
        if (m_dtRptBwd.OnDelay(Complete(RptPara.iFrstActr , ccBwd)&&Complete(RptPara.iScndActr , ccBwd) , RptPara.iDelay )) {Move(RptPara.iFrstActr , ccFwd); Move(RptPara.iScndActr , ccFwd);}
    }
    else {
        if (m_dtRptFwd.OnDelay(Complete(RptPara.iFrstActr , ccFwd)                                      , RptPara.iDelay )) Move(RptPara.iFrstActr , ccBwd);
        if (m_dtRptBwd.OnDelay(Complete(RptPara.iFrstActr , ccBwd)                                      , RptPara.iDelay )) Move(RptPara.iFrstActr , ccFwd);
    }


    //Ok.
    return true;
}

bool CActuators::StopRpt (void)
{
    m_bRpt = false ;
}


//Update.
void CActuators::Update(void)
{
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i] -> Update();
    }
    if (m_bRpt ) RptActr ();
}

//Read/Write Para.
void CActuators::Load()
{
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i] -> Load();
    }
}
void CActuators::Save()
{
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i] -> Save();
    }

}
void CActuators::LoadDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Load(sPath , "CActuators"  , "m_iMaxAct " , m_iMaxAct );

}
void CActuators::SaveDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Save(sPath , "CActuators"  , "m_iMaxAct " , m_iMaxAct );
}

//Display.
void CActuators::DisplayList(TStringGrid *_sgTable)
{
    //Local Var.
    int        rCount;
    AnsiString Temp;

    //Check Error.
    if (_sgTable == NULL) return;

    _sgTable -> Options << goRowSelect ;
    _sgTable -> DefaultRowHeight = 20 ;

        //Set StringGrid.
    rCount                 = m_iMaxAct;
    _sgTable -> RowCount      = rCount + 1;  if(_sgTable -> RowCount == 1) _sgTable -> RowCount = 2 ;
    _sgTable -> ColCount      = 11;
    _sgTable -> ColWidths[0 ] = 20;
    _sgTable -> ColWidths[1 ] = 80;
    _sgTable -> ColWidths[2 ] = 100;
    _sgTable -> ColWidths[3 ] = 40;
    _sgTable -> ColWidths[4 ] = 40;
    _sgTable -> ColWidths[5 ] = 40;
    _sgTable -> ColWidths[6 ] = 40;
    _sgTable -> ColWidths[7 ] = 40;
    _sgTable -> ColWidths[8 ] = 40;
    _sgTable -> ColWidths[9 ] = 40;
    _sgTable -> ColWidths[10] = 40;

        //Set Col.
    _sgTable -> Cells[0 ][0] = "No"      ;
    _sgTable -> Cells[1 ][0] = "Name"    ;
    _sgTable -> Cells[2 ][0] = "Comment" ;
    _sgTable -> Cells[3 ][0] = "xFwdID"  ;
    _sgTable -> Cells[4 ][0] = "xBwdID"  ;
    _sgTable -> Cells[5 ][0] = "yFwdID"  ;
    _sgTable -> Cells[6 ][0] = "yBwdID"  ;
    _sgTable -> Cells[7 ][0] = "FwdOD"   ;
    _sgTable -> Cells[8 ][0] = "BwdOD"   ;
    _sgTable -> Cells[9 ][0] = "FwdTO"   ;
    _sgTable -> Cells[10][0] = "BwdTO"   ;

    _sgTable -> FixedCols = 1  ;
    _sgTable -> FixedRows = 1  ;

        //Index No. Cell.
        //Display.

    CActuator::SPara * Para ;

    for (int i = 0 ; i < rCount ; i++) {
        Para = Actuator[i] -> GetPara() ;
        //Display.
        _sgTable -> Cells[0 ][i + 1] = i;
        _sgTable -> Cells[1 ][i + 1] = Para->sName;
        _sgTable -> Cells[2 ][i + 1] = Para->sComment;
        _sgTable -> Cells[3 ][i + 1] = Para->iFwdXAdd;
        _sgTable -> Cells[4 ][i + 1] = Para->iBwdXAdd;
        _sgTable -> Cells[5 ][i + 1] = Para->iFwdYAdd;
        _sgTable -> Cells[6 ][i + 1] = Para->iBwdYAdd;
        _sgTable -> Cells[7 ][i + 1] = Para->iFwdOnDelay;
        _sgTable -> Cells[8 ][i + 1] = Para->iBwdOnDelay;
        _sgTable -> Cells[9 ][i + 1] = Para->iFwdTimeOut;
        _sgTable -> Cells[10][i + 1] = Para->iBwdTimeOut;
    }

    //Selected Row to 0.
    static bool bFirst = false ;
    if(bFirst) {_sgTable -> TopRow = 1; _sgTable -> Row = 1 ; }
    bFirst = false ;

}
void CActuators::DisplayStatus(int _iActrNo , TLabel   *_lbFwdStat , TLabel  *_lbBwdStat , TLabel *_lbAlarm)
{
    //Check Null.
    if ((_lbFwdStat == NULL) || (_lbBwdStat == NULL) || (_lbAlarm == NULL)) return;

    //Display.
    if (Actuator[_iActrNo]->Complete(ccFwd)) _lbFwdStat ->Color = clLime;
    else                                     _lbFwdStat ->Color = clGray;
    if (Actuator[_iActrNo]->Complete(ccBwd)) _lbBwdStat ->Color = clLime;
    else                                     _lbBwdStat ->Color = clGray;
    if (Actuator[_iActrNo]->Err()          ) _lbAlarm   ->Color = clRed ;
    else                                     _lbAlarm   ->Color = clGray;

}

void CActuators::DisplayButton(int _iActrNo , TButton  *_btBwdStat , TButton *_btFwdStat)
{
    //Check Null.
    if ((_btFwdStat == NULL) || (_btBwdStat == NULL)) return;

    //Display.
    if (Actuator[_iActrNo]->Complete(ccFwd)) {_btFwdStat -> Caption  = "DONE"; }
    else                                     {_btFwdStat -> Caption  = "OFF" ; }
    if (Actuator[_iActrNo]->Complete(ccBwd)) {_btBwdStat -> Caption  = "DONE"; }
    else                                     {_btBwdStat -> Caption  = "OFF" ; }

}

void CActuators::DisplayProperty(bool  _bToTble ,
                                 TEdit * _edName , TEdit * _edComt ,
                                 TEdit * _edFx   , TEdit * _edBx   ,
                                 TEdit * _edFy   , TEdit * _edBy   ,
                                 TEdit * _edFd   , TEdit * _edBd   ,
                                 TEdit * _edFt   , TEdit * _edBt   ,
                                 TStringGrid *_sgTable )
{
    //Local Var.
    AnsiString Temp;
    int        iActrNo;
    CActuator::SPara * Para ;
    //    SPara Para ;

    //Check Null.
    if (_edName == NULL ||
        _edComt == NULL ||
        _edFx   == NULL ||
        _edBx   == NULL ||
        _edFy   == NULL ||
        _edBy   == NULL ||
        _edFd   == NULL ||
        _edBd   == NULL ||
        _edFt   == NULL ||
        _edBt   == NULL ||
        _sgTable == NULL ) return ;



    //Get Index.
    iActrNo = _sgTable -> Row - 1;

    if(iActrNo >= m_iMaxAct) return ;

    Para = Actuator[iActrNo] -> GetPara() ;

    //Display.
    if (_bToTble) {
        _edName -> Text = Para->sName       ;
        _edComt -> Text = Para->sComment    ;
        _edFx   -> Text = Para->iFwdXAdd    ;
        _edBx   -> Text = Para->iBwdXAdd    ;
        _edFy   -> Text = Para->iFwdYAdd    ;
        _edBy   -> Text = Para->iBwdYAdd    ;
        _edFd   -> Text = Para->iFwdOnDelay ;
        _edBd   -> Text = Para->iBwdOnDelay ;
        _edFt   -> Text = Para->iFwdTimeOut ;
        _edBt   -> Text = Para->iBwdTimeOut ;
    }
    else {
        Para->sName         = _edName -> Text                ;
        Para->sComment      = _edComt -> Text                ;
        Para->iFwdXAdd      = _edFx   -> Text.ToIntDef(-1)   ;
        Para->iBwdXAdd      = _edBx   -> Text.ToIntDef(-1)   ;
        Para->iFwdYAdd      = _edFy   -> Text.ToIntDef(-1)   ;
        Para->iBwdYAdd      = _edBy   -> Text.ToIntDef(-1)   ;
        Para->iFwdOnDelay   = _edFd   -> Text.ToIntDef(0)    ;
        Para->iBwdOnDelay   = _edBd   -> Text.ToIntDef(0)    ;
        Para->iFwdTimeOut   = _edFt   -> Text.ToIntDef(4000) ;
        Para->iBwdTimeOut   = _edBt   -> Text.ToIntDef(4000) ;
//        Actuator[iActrNo]  -> GetPara();

    }

}



























































































