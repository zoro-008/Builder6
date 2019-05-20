//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "UserINI.h"
#include "Motor.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
CMotors MS;

//---------------------------------------------------------------------------
void CMotors::Init (void )
{
    for(register int i = 0 ; i < MAX_MOTR ; i++) {
        CAMC[i].Open(i);
    }

    //Servo On.
    //SetServoAll(true);
}

//---------------------------------------------------------------------------
void CMotors::Reset(int _iAxisNo)
{
    SetAlarm(_iAxisNo , true );
    CAMC[_iAxisNo].Reset();
    SetAlarm(_iAxisNo , false);
}

//---------------------------------------------------------------------------
void CMotors::ResetAll(void )
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) {
        Reset((int)_iAxisNo);
        }
}

//---------------------------------------------------------------------------
void CMotors::SetServo(int _iAxisNo , bool _bOn)
{
    CAMC[_iAxisNo].SetServo(_bOn);
}

//---------------------------------------------------------------------------
void CMotors::SetServoAll(bool _bOn)
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) SetServo((int)_iAxisNo , _bOn);
}

//---------------------------------------------------------------------------
void CMotors::SetAlarm(int _iAxisNo , bool _bOn)
{
    CAMC[_iAxisNo].SetAlarm(_bOn);
}

//---------------------------------------------------------------------------
void CMotors::SetAlarmAll(bool _bOn)
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) SetAlarm((int)_iAxisNo , _bOn);
}

//---------------------------------------------------------------------------
void CMotors::ClearPos(int _iAxisNo , double Pos)
{
    CAMC[_iAxisNo].ClearPos(Pos);
}

//---------------------------------------------------------------------------
void CMotors::ClearPosAll(double Pos)
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) ClearPos((int)_iAxisNo , Pos);
}

//---------------------------------------------------------------------------
void CMotors::SetPos(int _iAxisNo , double Pos)
{
    CAMC[_iAxisNo].SetPos(Pos);
}

//---------------------------------------------------------------------------
void CMotors::SetHomeEnd (int _iAxisNo , bool _bOn )
{
    CAMC[_iAxisNo].SetHomeEnd(_bOn);

}

//---------------------------------------------------------------------------
void CMotors::ClearHomeEnd(int _iAxisNo)
{
    CAMC[_iAxisNo].ClearHomeEnd();
}

//---------------------------------------------------------------------------
void CMotors::ClearHomeEndAll(void  )
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) ClearHomeEnd((int)_iAxisNo);
}

//---------------------------------------------------------------------------
bool CMotors::IsAllAlarm(void)
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) {
        if (!CAMC[_iAxisNo].GetAlarm()) return false;
        }
    return true;
}

//---------------------------------------------------------------------------
bool CMotors::IsAllServoOff(void)
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) {
        if (CAMC[_iAxisNo].GetServo()) return false;
        }
    return true;
}

//---------------------------------------------------------------------------
bool CMotors::IsAllServoOn(void)
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) {
        if (!CAMC[_iAxisNo].GetServo()) return false;
        }
    return true;
}

//---------------------------------------------------------------------------
bool CMotors::IsAllHomeEnd(void)
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) {
        if (!CAMC[_iAxisNo].GetHomeEnd()) {
            int a;
            a++;
            return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------
bool CMotors::CmprPos(int _iAxisNo , double Pos)
{
    bool isOk = (fabs(CAMC[_iAxisNo].GetEncPos() - Pos) < CAMC[_iAxisNo].GetInPos()) &&
                (fabs(CAMC[_iAxisNo].GetCmdPos() - Pos) < CAMC[_iAxisNo].GetInPos()) ;

    return isOk;
}

//---------------------------------------------------------------------------
bool CMotors::CmprPos(int _iAxisNo , int PosId)
{
    double dEncPos = CAMC[_iAxisNo].GetEncPos() ;
    double dInPos  = CAMC[_iAxisNo].GetInPos () ;
    double dCmdPos = CAMC[_iAxisNo].GetCmdPos() ;
    double dSetPos = CAMC[_iAxisNo].dPosn[PosId] ; //여기부터.



    bool isOk = (fabs(CAMC[_iAxisNo].GetEncPos() - CAMC[_iAxisNo].dPosn[PosId]) < CAMC[_iAxisNo].GetInPos()) &&
                (fabs(CAMC[_iAxisNo].GetCmdPos() - CAMC[_iAxisNo].dPosn[PosId]) < CAMC[_iAxisNo].GetInPos()) ;
    return isOk;
}

//---------------------------------------------------------------------------
void CMotors::StopAll(void)
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) CAMC[_iAxisNo].Stop(true);
}

//---------------------------------------------------------------------------
bool CMotors::Stop(int _iAxisNo)
{
    return CAMC[_iAxisNo].Stop(true);
}

//---------------------------------------------------------------------------
void CMotors::EmrgStopAll(void)
{
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) CAMC[_iAxisNo].Stop(false);
}

//---------------------------------------------------------------------------
bool CMotors::EmrgStop(int _iAxisNo)
{
    return CAMC[_iAxisNo].Stop(false);
}

bool CMotors::CheckDstb(int _iAxisNo)
{
    bool ret = true ;

    for(register int i = 0 ; i < MAX_MOTR_DSTB ; i++) {
        if(!DSTB[_iAxisNo].iCheckMotr[i]) continue ;

        if(DSTB[_iAxisNo].bSmall[i]) {
            ret &= GetEncPos((int)DSTB[_iAxisNo].iCheckMotr[i]) < DSTB[_iAxisNo].dDstbPos[i] ;
            ret &= GetCmdPos((int)DSTB[_iAxisNo].iCheckMotr[i]) < DSTB[_iAxisNo].dDstbPos[i] ;
        }

        else {
            ret &= GetEncPos((int)DSTB[_iAxisNo].iCheckMotr[i]) > DSTB[_iAxisNo].dDstbPos[i] ;
            ret &= GetCmdPos((int)DSTB[_iAxisNo].iCheckMotr[i]) > DSTB[_iAxisNo].dDstbPos[i] ;
        }
    }

    return ret ;
}

//---------------------------------------------------------------------------
bool CMotors::MoveHome(int _iAxisNo , double V )
{
    //Move Home.
    //if(CAMC[_iAxisNo].GetAlarm()) CAMC[_iAxisNo].Reset   (    ) ;
    if(!CAMC[_iAxisNo].GetServo()) CAMC[_iAxisNo].SetServo(true) ;
    return CAMC[_iAxisNo].MoveHome(V);
}

bool CMotors::MoveHome(int _iAxisNo , int V )
{
    //Move Home.
    if(!CAMC[_iAxisNo].GetServo()) CAMC[_iAxisNo].SetServo(true) ;
    return CAMC[_iAxisNo].MoveHome(V);
}

bool CMotors::MoveHome(int _iAxisNo )
{
    if(!CAMC[_iAxisNo].GetServo()) CAMC[_iAxisNo].SetServo(true) ;
    return CAMC[_iAxisNo].MoveHome();
}

//---------------------------------------------------------------------------
bool CMotors::MotnDone(int _iAxisNo , bool CheckEn , double Inp)
{
    return CAMC[_iAxisNo].GetStop(CheckEn , Inp);
}

//---------------------------------------------------------------------------
bool  CMotors::MoveMotr(int _iAxisNo , double P , double V , double A , bool R)
{
    //Go Move.
    if (R) return CAMC[_iAxisNo].MoveR(P , V , A);
    else   return CAMC[_iAxisNo].MoveA(P , V , A);
}

bool  CMotors::MoveMotr(int _iAxisNo , double P , int V , int A , bool R)
{
    //Go Move.
    if (R) return CAMC[_iAxisNo].MoveR(P , V , A);
    else   return CAMC[_iAxisNo].MoveA(P , V , A);                             
}

bool  CMotors::MoveMotr(int _iAxisNo , double P , bool R)
{
    //Go Move.
    if (R)
    {
        switch (m_iSpedMode)
        {
            case viRunN : return CAMC[_iAxisNo].MoveR(P , CAMC[_iAxisNo].dVel[viRunN] , CAMC[_iAxisNo].dAcc[aiNorm]);
            case viManl : return CAMC[_iAxisNo].MoveR(P , CAMC[_iAxisNo].dVel[viManl] , CAMC[_iAxisNo].dAcc[aiNorm]);
        }
    }

    else
    {
        switch (m_iSpedMode)
        {
            case viRunN : return CAMC[_iAxisNo].MoveA(P , CAMC[_iAxisNo].dVel[viRunN] , CAMC[_iAxisNo].dAcc[aiNorm]);
            case viManl : return CAMC[_iAxisNo].MoveA(P , CAMC[_iAxisNo].dVel[viManl] , CAMC[_iAxisNo].dAcc[aiNorm]);
        }
    }

    return false ;
}

//bool  CMotors::MoveAsCmd(int _iAxisNo , int P , int V , int A)
//{
//    //Go Move.
//    return CAMC[_iAxisNo].MoveA(P , V , A);
//}
//
//bool  CMotors::MoveAsCmd(int _iAxisNo , int P )
//{
//    //Go Move.
//    switch (m_iSpedMode) {
//        case viRunN : return CAMC[_iAxisNo].MoveA(P , viRunN , aiNorm);
//        case viManl : return CAMC[_iAxisNo].MoveA(P , viManl , aiNorm);
//        }
//}

//---------------------------------------------------------------------------
bool  CMotors::MoveJog(int _iAxisNo , double V , double A , bool Dir )
{
    if (Dir) { return CAMC[_iAxisNo].MoveJogP(V,A); }
    else     { return CAMC[_iAxisNo].MoveJogN(V,A); }
}

bool  CMotors::MoveJog(int _iAxisNo , int V , int A , bool Dir )
{
    if (Dir) { return CAMC[_iAxisNo].MoveJogP(V,A); }
    else     { return CAMC[_iAxisNo].MoveJogN(V,A); }
}

bool CMotors::MoveJog(int _iAxisNo ,bool Dir)
{
    return Dir? CAMC[_iAxisNo].MoveJogP() : CAMC[_iAxisNo].MoveJogN() ;
}

//Update.
void CMotors::Update()
{
    m_iSpedMode = Run ? viRunN : viManl;

    for (register int i = 0 ; i < m_iMaxAxtFS20 ; i++) {
        CAMC[i].Update();
    }
}

//---------------------------------------------------------------------------
void CMotors::UpdateStat(int _iAxisNo , TLabelPlus *S1 , TLabelPlus *S2 , TLabelPlus *S3 , TLabelPlus *S4 , TLabelPlus *S5)
{
    //Local Var.
    AnsiString Temp;

    //Check Null.
    if ((S1  == NULL) || (S2  == NULL) || (S3 == NULL) || (S4 == NULL) || (S5 == NULL)) return;

    //Display Status.
    S1 -> Color->StartColor = CAMC[_iAxisNo].GetServo  () ? clGreen : clRed;
    S2 -> Color->StartColor = CAMC[_iAxisNo].GetAlarm  () ? clGreen : clRed;
    S3 -> Color->StartColor = CAMC[_iAxisNo].GetHomeEnd() ? clGreen : clRed;
    S4 -> Color->StartColor = CAMC[_iAxisNo].GetCW     () ? clGreen : clRed;
    S5 -> Color->StartColor = CAMC[_iAxisNo].GetCCW    () ? clGreen : clRed;
}

//---------------------------------------------------------------------------
void CMotors::SelStat(int _iAxisNo , TStaticLabelPlus *Cmd , TStaticLabelPlus *Enc , TLabelPlus *S1 , TLabelPlus *S2 , TLabelPlus *S3 , TLabelPlus *S4 , TLabelPlus *S5 , TLabelPlus *S6 , TLabelPlus *S7 , TLabelPlus *S8)
{
    //Local Var.
    AnsiString Temp;

    //Check Null.
    if ((Cmd == NULL) || (Enc == NULL)                                                                ) return;
    if ((S1  == NULL) || (S2  == NULL) || (S3 == NULL) || (S4 == NULL) || (S5 == NULL) || (S6 == NULL) || (S7 == NULL) ||(S8 == NULL)) return;

    //Display Position.
    Cmd -> Caption = Temp.sprintf("%.3f" , MOTR.GetCmdPos(_iAxisNo));
    Enc -> Caption = Temp.sprintf("%.3f" , MOTR.GetEncPos(_iAxisNo));

    //Display Status.
    S1 -> Color->StartColor = CAMC[_iAxisNo].GetServo     () ? clYellow : clGray ;
    S2 -> Color->StartColor = CAMC[_iAxisNo].GetAlarm     () ? clYellow : clGray ;
    S3 -> Color->StartColor = CAMC[_iAxisNo].GetHome      () ? clYellow : clGray ;
    S4 -> Color->StartColor = CAMC[_iAxisNo].GetCW        () ? clYellow : clGray ;
    S5 -> Color->StartColor = CAMC[_iAxisNo].GetCCW       () ? clYellow : clGray ;
    S6 -> Color->StartColor = CAMC[_iAxisNo].GetReady     () ? clYellow : clGray ;
    S7 -> Color->StartColor = CAMC[_iAxisNo].GetPackInPosn() ? clYellow : clGray ;
    S8 -> Color->StartColor = CAMC[_iAxisNo].GetHomeEnd   () ? clYellow : clGray ;
}
//---------------------------------------------------------------------------
void CMotors::MtrStatus(int _iAxisNo , TLabelPlus *Enc , TLabelPlus *S1 , TLabelPlus *S2 , TLabelPlus *S3 , TLabelPlus *S4 , TLabelPlus *S5 ,TLabelPlus *S6)
{
    //Local Var.
    AnsiString Temp;

    //Check Null.
    if  (Enc == NULL)                                                                                   return;
    if ((S1  == NULL) || (S2  == NULL) || (S3 == NULL) || (S4 == NULL) || (S5 == NULL) || (S6 == NULL)) return;

    //Display Position.
    Enc -> Caption = Temp.sprintf("%.3f" , MOTR.GetEncPos(_iAxisNo));

    //Display Status.
    S1 -> Color->StartColor = CAMC[_iAxisNo].GetServo   () ? clGreen : clGray ;
    S2 -> Color->StartColor = CAMC[_iAxisNo].GetAlarm   () ? clRed   : clGray ;
    S3 -> Color->StartColor = CAMC[_iAxisNo].GetHome    () ? clGreen : clGray ;
    S4 -> Color->StartColor = CAMC[_iAxisNo].GetCW      () ? clRed   : clGray ;
    S5 -> Color->StartColor = CAMC[_iAxisNo].GetCCW     () ? clRed   : clGray ;
    S6 -> Color->StartColor = CAMC[_iAxisNo].GetHomeEnd () ? clGreen : clGray ;

    S1 -> Color->EndColor = clBtnFace ;
    S2 -> Color->EndColor = clBtnFace ;
    S3 -> Color->EndColor = clBtnFace ;
    S4 -> Color->EndColor = clBtnFace ;
    S5 -> Color->EndColor = clBtnFace ;
    S6 -> Color->EndColor = clBtnFace ;
}

//---------------------------------------------------------------------------
void CMotors::MtrStatus (int _iAxisNo , TLabelPlus *Enc , TLabelPlus *Sv , TLabelPlus *Ar , TLabelPlus *Cw , TLabelPlus *Cc , TLabelPlus *He)
{
    //Local Var.
    AnsiString Temp;

    //Check Null.
    if  (Enc == NULL)                                                                   return;
    if ((Sv  == NULL) || (Ar  == NULL) || (Cw == NULL) || (Cc == NULL) || (He == NULL)) return;

    //Display Position.
    Enc -> Caption = Temp.sprintf("%.3f" , MOTR.GetEncPos(_iAxisNo));

    //Display Status.
    Sv -> Color->StartColor = CAMC[_iAxisNo].GetServo   () ? clGreen : clGray ;
    Ar -> Color->StartColor = CAMC[_iAxisNo].GetAlarm   () ? clRed   : clGray ;
    Cw -> Color->StartColor = CAMC[_iAxisNo].GetCW      () ? clRed   : clGray ;
    Cc -> Color->StartColor = CAMC[_iAxisNo].GetCCW     () ? clRed   : clGray ;
    He -> Color->StartColor = CAMC[_iAxisNo].GetHomeEnd () ? clGreen : clGray ;

    Sv -> Color->EndColor = clBtnFace ;
    Ar -> Color->EndColor = clBtnFace ;
    Cw -> Color->EndColor = clBtnFace ;
    Cc -> Color->EndColor = clBtnFace ;
    He -> Color->EndColor = clBtnFace ;
}

//---------------------------------------------------------------------------
void CMotors::LoadPara(bool IsLoad , int iAxis)
{
    CAMC[iAxis].LoadPara(IsLoad , iAxis);
}
//---------------------------------------------------------------------------
void CMotors::LoadAxis(bool IsLoad, AnsiString FolderName)
{
    AnsiString FileName ;
    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++) {
        FileName = "Motor#"+ (AnsiString)_iAxisNo ;
        CAMC[_iAxisNo].LoadAxis(IsLoad, FileName , FolderName);
       }
}
//---------------------------------------------------------------------------
void CMotors::LoadDstb (bool IsLoad)
{
    AnsiString FileName ;


    //Local Var.
    TUserINI   UserINI  ;
    AnsiString FilePath ;
    AnsiString AxisId   ;

    int     iCheckMotr ;
    double  dDstbPos   ;
    bool    bSmall     ;

    //Make Dir.
    FilePath = ExtractFilePath(Application -> ExeName)  + "Util" ;
    if (!DirectoryExists(FilePath)) CreateDir(FilePath);
    FileName = FilePath + "\\" + "MotrDstb" + ".INI";

    UserINI.Save(FileName.c_str() , "도움말" , "iCheckMotr축이 dDstbPos보다 bSmall해야지만 구동이됨",  iCheckMotr );

    for (register int _iAxisNo = 0 ; _iAxisNo < MAX_MOTR ; _iAxisNo++)
    {
        AxisId = "Axis-" + (AnsiString)_iAxisNo ;
        for(register int DstbId = 0 ; DstbId < MAX_MOTR_DSTB ; DstbId++)
        {
            iCheckMotr = DSTB[_iAxisNo].iCheckMotr[DstbId] ;
            dDstbPos   = DSTB[_iAxisNo].dDstbPos  [DstbId] ;
            bSmall     = DSTB[_iAxisNo].bSmall    [DstbId] ;
            if (IsLoad)
            {
                 UserINI.Load(FileName.c_str() , AxisId.c_str() , ((AnsiString)DstbId + "-iCheckMotr").c_str() , &iCheckMotr );
                 UserINI.Load(FileName.c_str() , AxisId.c_str() , ((AnsiString)DstbId + "-dDstbPos  ").c_str() , &dDstbPos   );
                 UserINI.Load(FileName.c_str() , AxisId.c_str() , ((AnsiString)DstbId + "-bSmall    ").c_str() , &bSmall     );
            }

            else
            {
                 UserINI.Save(FileName.c_str() , AxisId.c_str() , ((AnsiString)DstbId + "-iCheckMotr").c_str() ,  iCheckMotr );
                 UserINI.Save(FileName.c_str() , AxisId.c_str() , ((AnsiString)DstbId + "-dDstbPos  ").c_str() ,  dDstbPos   );
                 UserINI.Save(FileName.c_str() , AxisId.c_str() , ((AnsiString)DstbId + "-bSmall    ").c_str() ,  bSmall     );
            }
            DSTB[_iAxisNo].iCheckMotr[DstbId] = iCheckMotr ;
            DSTB[_iAxisNo].dDstbPos  [DstbId] = dDstbPos   ;
            DSTB[_iAxisNo].bSmall    [DstbId] = bSmall     ;
        }
    }
}
