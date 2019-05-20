//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Actuators.h"
//#include "IOs.h"
#include "UserIni.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//Actuator Object.
CActuators AT;

/***************************************************************************/
/* CKaBit                                                                  */
/***************************************************************************/
//---------------------------------------------------------------------------
__fastcall CKaBit::CKaBit(void)
{
    Init();
}

//---------------------------------------------------------------------------
__fastcall CKaBit::~CKaBit(void)
{
}

//---------------------------------------------------------------------------
void __fastcall CKaBit::Init(void)
{
    Clear();
    Axis = 0;
}

//---------------------------------------------------------------------------
void __fastcall CKaBit::Clear(void)
{
    On  = false;
    Off = true;
    Enabled = true;
    Tag = 0;
}

//---------------------------------------------------------------------------
void __fastcall CKaBit::Out(bool SeqCondition) {
    //if (!Enabled) return;
    if (SeqCondition) {On=true; Off=false;}
    else {On=false; Off=true;}
}

//---------------------------------------------------------------------------
void __fastcall CKaBit::Flk(void)
{
    if (On) Rst();
    else Set();
}

//---------------------------------------------------------------------------
void __fastcall CKaBit::Set(void)
{
    Out(true);
}

//---------------------------------------------------------------------------
void __fastcall CKaBit::Rst(void)
{
    Out  (false);
    Clear(     );
}

//---------------------------------------------------------------------------
void __fastcall CKaBit::KeepOn(void)
{
    Enabled = true;
    Set();
    Enabled = false;
}

//---------------------------------------------------------------------------
void __fastcall CKaBit::KeepOff(void)
{
    Enabled = true;
    Rst();
    Enabled = false;
}

//---------------------------------------------------------------------------
void __fastcall CKaBit::Difu(int Condition)
{
    if (!On) {
        if (Condition) {
            if (!DifuFlag) {
                On       = true ;
                Off      = false;
                DifuFlag = true ;
                }
            }
        }
    else {
        On  = false;
        Off = true;
        }

    if (!Condition) DifuFlag = false;
}

/***************************************************************************/
/* TActuator                                                               */
/***************************************************************************/
//---------------------------------------------------------------------------
void __fastcall TActuator::Init(void)
{
    xfwdId = xfwdIndex = 0;
    xbwdId = xbwdIndex = 0;
    yfwdId = yfwdIndex = 0;
    ybwdId = ybwdIndex = 0;

    //options.
    ApplyTimeout = 0;
    ApplyOutComplete = 0;

    FwdTimeOutDelayTime = 0.0;
    BwdTimeOutDelayTime = 0.0;

    Name = "NONE";

    dfy = dfx = fx = vfx = odfx = ftoe = Ltftoe = 0;
    dby = dbx = bx = vbx = odbx = btoe = Ltbtoe = 0;

    LastOutCommand = -1;

    FwdOnDelayTime = 0.0;
    BwdOnDelayTime = 0.0;


}

//---------------------------------------------------------------------------
void __fastcall TActuator::SetAllId (int  xf, int xb, int yf, int yb)
{
    SetxfwdId(xf);
    SetxbwdId(xb);
    SetyfwdId(yf);
    SetybwdId(yb);
}

//---------------------------------------------------------------------------
int  __fastcall TActuator::Input(int n)
{
    return IO.GetX(n);
}

//---------------------------------------------------------------------------
int  __fastcall TActuator::Output(int n)
{
    return IO.GetY(n) ;
}

//---------------------------------------------------------------------------
void __fastcall TActuator::Output(int n , int on)
{
    IO.SetY(n,on);
}

//---------------------------------------------------------------------------
bool __fastcall TActuator::Trg (int Cmd)
{
    return (LastOutCommand == Cmd) ? true : false;
}

//---------------------------------------------------------------------------
bool __fastcall TActuator::Out(int on)
{
    int          yf     ;
    int          yb     ;
    int yfIndex;
    int ybIndex;

    //Inverse Out.
    if (iInv) {
        yf = GetybwdId(); yfIndex = GetybwdIndex();
        yb = GetyfwdId(); ybIndex = GetyfwdIndex();
        }
    else {
        yf = GetyfwdId(); yfIndex = GetyfwdIndex();
        yb = GetybwdId(); ybIndex = GetybwdIndex();
        }

    //Set.
    EnableLastOutCmdTOut = true;

    if(On != LastOutCommand) {
        Trace(Name.c_str() , on ? "Fwd" : "Bwd" );
    }

    if (on){
        LastOutCommand = 1;
        if (yf != 0) Output(yfIndex , 1);
        if (yb != 0) Output(ybIndex , 0);
        return dfy && !dby;
        }
    else {
        LastOutCommand = 0;
        if (yf != 0) Output(yfIndex , 0);
        if (yb != 0) Output(ybIndex , 1);
        return !dfy && dby;
        }
}

//---------------------------------------------------------------------------
int __fastcall TActuator::Run(int Cmd)
{
    Out(Cmd);
    int r = Complete(Cmd);
    return r;
}

//---------------------------------------------------------------------------
int __fastcall TActuator::Complete(int Cmd)
{
    if (Cmd) return  fx && !bx;
    else     return !fx &&  bx;
}

//---------------------------------------------------------------------------
bool __fastcall TActuator::GetTimeOut(void)
{
    if (ftoe  ) return true;
    if (btoe  ) return true;
    if (Ltftoe) return true;
    if (Ltbtoe) return true;
    return false;
}

//---------------------------------------------------------------------------
void __fastcall TActuator::UpdateInput(void)
{
    //Local Var.
    bool         isXFwdID  ;
    bool         isXBwdID  ;
    bool         isYFwdID  ;
    bool         isYBwdID  ;
    int          iXfwdId   ;
    int          iXbwdId   ;
    int          iYfwdId   ;
    int          iYbwdId   ;
    int  iXfwdIndex;
    int  iXbwdIndex;
    int  iYfwdIndex;
    int  iYbwdIndex;

    //Set I/O ID.
    iXfwdId = iInv ? xbwdId : xfwdId;
    iXbwdId = iInv ? xfwdId : xbwdId;
    iYfwdId = iInv ? ybwdId : yfwdId;
    iYbwdId = iInv ? yfwdId : ybwdId;

    //Set I/O Index.
    iXfwdIndex = iInv ? xbwdIndex : xfwdIndex;
    iXbwdIndex = iInv ? xfwdIndex : xbwdIndex;
    iYfwdIndex = iInv ? ybwdIndex : yfwdIndex;
    iYbwdIndex = iInv ? yfwdIndex : ybwdIndex;

    //Check SKIP.
    if (iYfwdId == 0 && iYbwdId == 0) {
        Rst();
        return ;
        }

    //Set exist ID.
    isXFwdID = (iXfwdId != 0)  ;
    isXBwdID = (iXbwdId != 0)  ;
    isYFwdID = (iYfwdId != 0)  ;
    isYBwdID = (iYbwdId != 0)  ;

    //I/O.
    if (isXFwdID) dfx =/*Input (iXfwdId);//sun*/ Input (iXfwdIndex); //Detect X/Y Val.
    if (isXBwdID) dbx =/*Input (iXbwdId);//sun*/ Input (iXbwdIndex);
    if (isYFwdID) dfy =/*Output(iYfwdId);//sun*/ Output(iYfwdIndex);
    if (isYBwdID) dby =/*Output(iYbwdId);//sun*/ Output(iYbwdIndex);

    //Virtual Input Sensor
    if      (isXFwdID) vfx =  dfx;
    else if (isXBwdID) vfx = !dbx;
    else if (isYFwdID) vfx =  dfy;
    else if (isYBwdID) vfx = !dby;
    else               vfx = !dby;

    if      (isXBwdID) vbx =  dbx;
    else if (isXFwdID) vbx = !dfx;
    else if (isYBwdID) vbx =  dby;
    else if (isYFwdID) vbx = !dfy;
    else               vbx = !dfy;

    //Virtual Output Sensor
    if (isYFwdID) vfy =  dfy;
    else          vfy = !dby;
    if (isYBwdID) vby =  dby;
    else          vby = !dfy;

    //OnDelay Timer.
    odfx = FwdOnDelayTimer.OnDelay(vfx , FwdOnDelayTime);
    odbx = BwdOnDelayTimer.OnDelay(vbx , BwdOnDelayTime);

    //Check time out by ondelay result.
        //Time out By Input
    int r = 1;
    if ( odfx && !odbx) r = 0;
    if (!odfx &&  odbx) r = 0;
    if ( odfx &&  odbx) r = 1;
    if (!odfx && !odbx) r = 1;
        //Time out By Output
    if (/*dfy*/vfy &&  odbx) r = 1;
    if (/*dfy*/vfy && !odfx) r = 1;
    if (/*dby*/vby &&  odfx) r = 1;
    if (/*dby*/vby && !odbx) r = 1;

    //Is timeout.
    ftoe = FwdTimeOutTimer.OnDelay(ApplyTimeout && r , FwdTimeOutDelayTime);
    btoe = BwdTimeOutTimer.OnDelay(ApplyTimeout && r , BwdTimeOutDelayTime);
    if (ftoe) Ltftoe = 1;
    if (btoe) Ltbtoe = 1;

    //Error Action.
    if (ftoe || btoe) { fx = false; bx = false; }
    else              { fx = odfx ; bx = odbx ; }

    //Apply Out Complete.
    if (ApplyOutComplete) {
//        Rst();
//        fx = dfy;
//        bx = dby;
//
//        if      (isYFwdID) fx =  dfy;
//        else if (isYBwdID) fx = !dby;
//        else               fx =  !bx;
//
//        if      (isYBwdID) bx =  dby;
//        else if (isYFwdID) bx = !dfy;
//        else               bx =  !fx;
        }
}

//---------------------------------------------------------------------------
int __fastcall TActuator::Err(void)
{
    if (Ltftoe) return 1;
    if (Ltbtoe) return 2;
    return 0;
}

//---------------------------------------------------------------------------
void __fastcall TActuator::Rst(void)
{
    ftoe = 0; Ltftoe = 0;
    btoe = 0; Ltbtoe = 0;

    //Timer Reset.
    FwdTimeOutTimer.Clear();
    BwdTimeOutTimer.Clear();
}

//---------------------------------------------------------------------------
void __fastcall TActuator::Update(void)
{
    UpdateInput();
}

//---------------------------------------------------------------------------
void __fastcall TActuator::Load(int Act , AnsiString Path)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Item;
    AnsiString Temp;

    //Load.
    Item.printf("ACTUATOR(%03d)" , Act);
    UserINI.Load(Path.c_str() , "Name            " , Item.c_str() , &Name               );
    UserINI.Load(Path.c_str() , "Comment         " , Item.c_str() , &Comt               );
    UserINI.Load(Path.c_str() , "xFwdID          " , Item.c_str() , &Temp               ); xfwdId = ("0x" + Temp.Delete(1 , 1)).ToIntDef(0);
    UserINI.Load(Path.c_str() , "xBwdID          " , Item.c_str() , &Temp               ); xbwdId = ("0x" + Temp.Delete(1 , 1)).ToIntDef(0);
    UserINI.Load(Path.c_str() , "yFwdID          " , Item.c_str() , &Temp               ); yfwdId = ("0x" + Temp.Delete(1 , 1)).ToIntDef(0);
    UserINI.Load(Path.c_str() , "yBwdID          " , Item.c_str() , &Temp               ); ybwdId = ("0x" + Temp.Delete(1 , 1)).ToIntDef(0);
    UserINI.Load(Path.c_str() , "ApplyTimeOut    " , Item.c_str() , &ApplyTimeout       );
    UserINI.Load(Path.c_str() , "ThreadApply     " , Item.c_str() , &ThreadApplyTime    );
    UserINI.Load(Path.c_str() , "FwdTimeOutDelay " , Item.c_str() , &FwdTimeOutDelayTime);
    UserINI.Load(Path.c_str() , "BwdTimeOutDelay " , Item.c_str() , &BwdTimeOutDelayTime);
    UserINI.Load(Path.c_str() , "FwdOnDelay      " , Item.c_str() , &FwdOnDelayTime     );
    UserINI.Load(Path.c_str() , "BwdOnDelay      " , Item.c_str() , &BwdOnDelayTime     );
    UserINI.Load(Path.c_str() , "ApplyOutComplete" , Item.c_str() , &ApplyOutComplete   );
    UserINI.Load(Path.c_str() , "Inverse         " , Item.c_str() , &iInv               );
    UserINI.Load(Path.c_str() , "iInitStat       " , Item.c_str() , &iInitStat          );
    //Index를 재설정 하기 위해...(2004/05/11)
    SetxfwdId(xfwdId);
    SetxbwdId(xbwdId);
    SetyfwdId(yfwdId);
    SetybwdId(ybwdId);

    //Run(iInitStat);
}

void __fastcall TActuator::Save(int Act , AnsiString Path)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Item;
    AnsiString Temp;

    //Load.
    Item.printf("ACTUATOR(%03d)" , Act);
                                            UserINI.Save(Path.c_str() , "Name            "  , Item.c_str(), Name               );
                                            UserINI.Save(Path.c_str() , "Comment         "  , Item.c_str(), Comt               );
    Temp = "X" + Temp.IntToHex(xfwdId , 4); UserINI.Save(Path.c_str() , "xFwdID          "  , Item.c_str(), Temp               );
    Temp = "X" + Temp.IntToHex(xbwdId , 4); UserINI.Save(Path.c_str() , "xBwdID          "  , Item.c_str(), Temp               );
    Temp = "Y" + Temp.IntToHex(yfwdId , 4); UserINI.Save(Path.c_str() , "yFwdID          "  , Item.c_str(), Temp               );
    Temp = "Y" + Temp.IntToHex(ybwdId , 4); UserINI.Save(Path.c_str() , "yBwdID          "  , Item.c_str(), Temp               );
                                            UserINI.Save(Path.c_str() , "ApplyTimeOut    "  , Item.c_str(), ApplyTimeout       );
                                            UserINI.Save(Path.c_str() , "ThreadApply     "  , Item.c_str(), ThreadApplyTime    );
                                            UserINI.Save(Path.c_str() , "FwdTimeOutDelay "  , Item.c_str(), FwdTimeOutDelayTime);
                                            UserINI.Save(Path.c_str() , "BwdTimeOutDelay "  , Item.c_str(), BwdTimeOutDelayTime);
                                            UserINI.Save(Path.c_str() , "FwdOnDelay      "  , Item.c_str(), FwdOnDelayTime     );
                                            UserINI.Save(Path.c_str() , "BwdOnDelay      "  , Item.c_str(), BwdOnDelayTime     );
                                            UserINI.Save(Path.c_str() , "ApplyOutComplete"  , Item.c_str(), ApplyOutComplete   );
                                            UserINI.Save(Path.c_str() , "Inverse         "  , Item.c_str(), iInv               );
                                            UserINI.Save(Path.c_str() , "iInitStat       "  , Item.c_str(), iInitStat          );

}

/***************************************************************************/
/* TActuatorTabal                                                          */
/***************************************************************************/
__fastcall CActuators::CActuators(void)
{
    LoadDnmVar() ;
    Actuator = new TActuator[m_iMaxAct] ;

    memset(Actuator , 0 , sizeof(TActuator)*m_iMaxAct);

    //Load();

}
__fastcall CActuators::~CActuators(void)
{
    delete [] Actuator ;
}
//---------------------------------------------------------------------------
void __fastcall CActuators::Init(void)
{
    AnsiString Name , Comt;
    for (register int i = 0 ; i < m_iMaxAct ; i++) {
        Name.printf("ACTUATOR(%03d)" , i);
        Comt.printf("COMMENT(%03d)"  , i);
        Actuator[i].Init();
        Actuator[i].SetName(Name);
        Actuator[i].SetComt(Comt);
        Actuator[i].SetxfwdId(i);
        Actuator[i].SetxbwdId(i);
        Actuator[i].SetyfwdId(i);
        Actuator[i].SetybwdId(i);
        }
}

//---------------------------------------------------------------------------
bool  __fastcall CActuators::MoveCyl(int aNum , int Act)
{
    //Run Actuator.
    return Actuator[aNum].Run(Act);
}

//---------------------------------------------------------------------------
void __fastcall CActuators::Update(void)
{
    for (register int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i].UpdateInput();
        }
}

//---------------------------------------------------------------------------
void __fastcall CActuators::Load()
{
    //Local Var.
    AnsiString Temp;
    AnsiString sPath;
    AnsiString sPath2;

    //Set Path.
    sPath  = EXE_FOLDER + "Util\\Actuator.INI";

    for (register int i = 0 ; i < m_iMaxAct  ; i++) {
        Actuator[i].Load(i , sPath);
        Application -> ProcessMessages();
        }


   // Actuator[2].Run(ccBwd);
}

void __fastcall CActuators::Save()
{
    //Local Var.
    AnsiString Temp ;
    AnsiString sPath;

    //Make Dir.
    sPath  = EXE_FOLDER + "Util\\Actuator.INI";

    //Input.
    for (register int i = 0 ; i < m_iMaxAct  ; i++) {
        Actuator[i].Save(i , sPath);
        //To remove system's deadlock.
        Application -> ProcessMessages();
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
//---------------------------------------------------------------------------
void __fastcall CActuators::DisplayList(TStringGrid *Table)
{
    //Local Var.
    int        rCount;
    AnsiString Temp;

    //Check Error.
    if (Table == NULL) return;

    Table -> DefaultRowHeight = 20 ;
    Table -> Options << goRowSelect ;

    //ID.
        //Set StringGrid.
    rCount                 = m_iMaxAct;
    Table -> RowCount      = rCount + 1;
    Table -> ColCount      = 15;
    Table -> ColWidths[0 ] = 20;
    Table -> ColWidths[1 ] = 80;
    Table -> ColWidths[2 ] = 100;
    Table -> ColWidths[3 ] = 40;
    Table -> ColWidths[4 ] = 40;
    Table -> ColWidths[5 ] = 40;
    Table -> ColWidths[6 ] = 40;
    Table -> ColWidths[7 ] = 40;
    Table -> ColWidths[8 ] = 40;
    Table -> ColWidths[9 ] = 40;
    Table -> ColWidths[10] = 40;
    Table -> ColWidths[11] = 0;
    Table -> ColWidths[12] = 0;
    Table -> ColWidths[13] = 20;
    Table -> ColWidths[14] = 0;

        //Set Col.
    Table -> Cells[0 ][0] = "No"      ;
    Table -> Cells[1 ][0] = "Name"    ;
    Table -> Cells[2 ][0] = "Comment" ;
    Table -> Cells[3 ][0] = "xFwdID"  ;
    Table -> Cells[4 ][0] = "xBwdID"  ;
    Table -> Cells[5 ][0] = "yFwdID"  ;
    Table -> Cells[6 ][0] = "yBwdID"  ;
    Table -> Cells[7 ][0] = "FwdOD"   ;
    Table -> Cells[8 ][0] = "BwdOD"   ;
    Table -> Cells[9 ][0] = "FwdTO"   ;
    Table -> Cells[10][0] = "BwdTO"   ;
    Table -> Cells[11][0] = "ApplyTO" ;
    Table -> Cells[12][0] = "ApplyOC" ;
    Table -> Cells[13][0] = "Inv"     ;
    Table -> Cells[14][0] = "InitStat";
        //Index No. Cell.
        //Display.
    for (int i = 0 ; i < rCount ; i++) {
        //Display.
        Table -> Cells[0 ][i + 1] = i;
        Table -> Cells[1 ][i + 1] = Actuator[i].GetName();
        Table -> Cells[2 ][i + 1] = Actuator[i].GetComt();
        Temp = "X" + Temp.IntToHex(Actuator[i].GetxfwdId() , 4); Table -> Cells[3][i + 1] = Temp;
        Temp = "X" + Temp.IntToHex(Actuator[i].GetxbwdId() , 4); Table -> Cells[4][i + 1] = Temp;
        Temp = "Y" + Temp.IntToHex(Actuator[i].GetyfwdId() , 4); Table -> Cells[5][i + 1] = Temp;
        Temp = "Y" + Temp.IntToHex(Actuator[i].GetybwdId() , 4); Table -> Cells[6][i + 1] = Temp;
        Table -> Cells[7 ][i + 1] = Actuator[i].GetFwdOnDelayTime     ();
        Table -> Cells[8 ][i + 1] = Actuator[i].GetBwdOnDelayTime     ();
        Table -> Cells[9 ][i + 1] = Actuator[i].GetFwdTimeOutDelayTime();
        Table -> Cells[10][i + 1] = Actuator[i].GetBwdTimeOutDelayTime();
        Table -> Cells[11][i + 1] = Actuator[i].GetApplyTimeout       ();
        Table -> Cells[12][i + 1] = Actuator[i].GetApplyOutComplete   ();
        Temp = Actuator[i].GetInv(); Table -> Cells[13][i + 1] = Temp;

        Table -> Cells[14][i + 1] = Actuator[i].GetInitStat() ;


    }

    //Selected Row to 0.
    //Table -> TopRow = 1;
}
//---------------------------------------------------------------------------
void __fastcall CActuators::DisplayStatus(int Index , TLabel *FwdStat , TLabel *BwdStat , TLabel *Alarm)
{
    //Check Null.
    if ((FwdStat == NULL) || (BwdStat == NULL) || (Alarm == NULL)) return;

    //Display.
    if (Actuator[Index].Complete(ccFwd)) FwdStat ->Color = clLime;
    else                                 FwdStat ->Color = clGray;
    if (Actuator[Index].Complete(ccBwd)) BwdStat ->Color = clLime;
    else                                 BwdStat ->Color = clGray;
    if (Actuator[Index].GetTimeOut   ()) Alarm   ->Color = clRed ;
    else                                 Alarm   ->Color = clGray;
}

//---------------------------------------------------------------------------
void __fastcall CActuators::DisplayButton (int Index , TButton *BwdStat , TButton *FwdStat)
{
    //Check Null.
    if ((FwdStat == NULL) || (BwdStat == NULL)) return;

    //Display.
    if (Actuator[Index].Complete(ccFwd)) {FwdStat -> Caption  = "DONE"; }
    else                                 {FwdStat -> Caption  = "OFF" ; }
    if (Actuator[Index].Complete(ccBwd)) {BwdStat -> Caption  = "DONE"; }
    else                                 {BwdStat -> Caption  = "OFF" ; }
}

//---------------------------------------------------------------------------
void __fastcall CActuators::DisplayProperty(bool ToTble   ,
                                            TEdit * EName , TEdit * EComt ,
                                            TEdit * EFx   , TEdit * EBx   ,
                                            TEdit * EFy   , TEdit * EBy   ,
                                            TEdit * EFd   , TEdit * EBd   ,
                                            TEdit * EFt   , TEdit * EBt   ,
                                            TEdit * ETo   , TEdit * EOc   ,
                                            TEdit * EIv   , TEdit * EIs   , TStringGrid *Table)
{
    //Local Var.
    AnsiString Temp;
    int        iIndex;

    //Check Null.
    if (EName == NULL ||
        EComt == NULL ||
        EFx   == NULL ||
        EBx   == NULL ||
        EFy   == NULL ||
        EBy   == NULL ||
        EFd   == NULL ||
        EBd   == NULL ||
        EFt   == NULL ||
        EBt   == NULL ||
        ETo   == NULL ||
        EOc   == NULL ||
        EIv   == NULL ||
        EIs   == NULL ||
        Table == NULL ) return ;



    //Get Index.
    iIndex = Table -> Row - 1;

    //Display.
    if (ToTble) {
        Temp = Actuator[iIndex].GetName                      ();      EName -> Text = Temp;
        Temp = Actuator[iIndex].GetComt                      ();      EComt -> Text = Temp;
        Temp = "X" + Temp.IntToHex(Actuator[iIndex].GetxfwdId() , 4); EFx   -> Text = Temp;
        Temp = "X" + Temp.IntToHex(Actuator[iIndex].GetxbwdId() , 4); EBx   -> Text = Temp;
        Temp = "Y" + Temp.IntToHex(Actuator[iIndex].GetyfwdId() , 4); EFy   -> Text = Temp;
        Temp = "Y" + Temp.IntToHex(Actuator[iIndex].GetybwdId() , 4); EBy   -> Text = Temp;
        Temp = Actuator[iIndex].GetFwdOnDelayTime            ();      EFd   -> Text = Temp;
        Temp = Actuator[iIndex].GetBwdOnDelayTime            ();      EBd   -> Text = Temp;
        Temp = Actuator[iIndex].GetFwdTimeOutDelayTime       ();      EFt   -> Text = Temp;
        Temp = Actuator[iIndex].GetBwdTimeOutDelayTime       ();      EBt   -> Text = Temp;
        Temp = Actuator[iIndex].GetApplyTimeout              ();      ETo   -> Text = Temp;
        Temp = Actuator[iIndex].GetApplyOutComplete          ();      EOc   -> Text = Temp;
        Temp = Actuator[iIndex].GetInv                       ();      EIv   -> Text = Temp;
        Temp = Actuator[iIndex].GetInitStat                  ();      EIs   -> Text = Temp;

    }
    else {
        Temp = EName  -> Text; Actuator[iIndex].SetName(Temp);
        Temp = EComt  -> Text; Actuator[iIndex].SetComt(Temp);
        Temp = EFx    -> Text; Actuator[iIndex].SetxfwdId             (("0x" + Temp.Delete(1 , 1)).ToIntDef(Actuator[iIndex].GetxfwdId             ()));
        Temp = EBx    -> Text; Actuator[iIndex].SetxbwdId             (("0x" + Temp.Delete(1 , 1)).ToIntDef(Actuator[iIndex].GetxbwdId             ()));
        Temp = EFy    -> Text; Actuator[iIndex].SetyfwdId             (("0x" + Temp.Delete(1 , 1)).ToIntDef(Actuator[iIndex].GetyfwdId             ()));
        Temp = EBy    -> Text; Actuator[iIndex].SetybwdId             (("0x" + Temp.Delete(1 , 1)).ToIntDef(Actuator[iIndex].GetybwdId             ()));
        Temp = EFd    -> Text; Actuator[iIndex].SetFwdOnDelayTime     (        Temp.ToIntDef               (Actuator[iIndex].GetFwdOnDelayTime     ()));
        Temp = EBd    -> Text; Actuator[iIndex].SetBwdOnDelayTime     (        Temp.ToIntDef               (Actuator[iIndex].GetBwdOnDelayTime     ()));
        Temp = EFt    -> Text; Actuator[iIndex].SetFwdTimeOutDelayTime(        Temp.ToIntDef               (Actuator[iIndex].GetFwdTimeOutDelayTime()));
        Temp = EBt    -> Text; Actuator[iIndex].SetBwdTimeOutDelayTime(        Temp.ToIntDef               (Actuator[iIndex].GetBwdTimeOutDelayTime()));
        Temp = ETo    -> Text; Actuator[iIndex].SetApplyTimeout       (        Temp.ToIntDef               (Actuator[iIndex].GetApplyTimeout       ()));
        Temp = EOc    -> Text; Actuator[iIndex].SetApplyOutComplete   (        Temp.ToIntDef               (Actuator[iIndex].GetApplyOutComplete   ()));
        Temp = EIv    -> Text; Actuator[iIndex].SetInv                (        Temp.ToIntDef               (Actuator[iIndex].GetInv                ()));
        Temp = EIs    -> Text; Actuator[iIndex].SetInitStat           (        Temp.ToIntDef               (Actuator[iIndex].GetInitStat           ()));
    }
}
//---------------------------------------------------------------------------

