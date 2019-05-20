//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "UserIni.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
COptionMan OM;

//---------------------------------------------------------------------------
COptionMan::COptionMan(void)
{

}

//---------------------------------------------------------------------------
COptionMan::~COptionMan(void)
{
    SaveLastInfo();
}

//---------------------------------------------------------------------------
void COptionMan::Init(void)
{
    //Init. Buffer.
    memset(&DevOptn  , 0 , sizeof(DevOptn ));
    memset(&EqpOptn  , 0 , sizeof(EqpOptn ));

    LoadLastInfo(          );
    LoadMstOptn (          );
    LoadEqpOptn (          );
    LoadCmnOptn (          );
    LoadDevInfo (m_sCrntDev);
    LoadDevOptn (m_sCrntDev);
    LoadJobFile (m_sCrntDev); //아직 비전이 없음.
}

void COptionMan::SetCrntDev (AnsiString _sName)
{
    m_sCrntDev = _sName ;
}

void COptionMan::LoadJobFile(AnsiString _sDevName)
{
    LoadDevInfo (_sDevName);
    LoadDevOptn (_sDevName);
    //Set Current Device Name.
    m_sCrntDev = _sDevName;
}


void COptionMan::SaveJobFile(AnsiString _sDevName)
{
    SaveDevInfo (_sDevName);
    SaveDevOptn (_sDevName);
    //Set Current Device Name.
    m_sCrntDev = _sDevName;
}

void COptionMan::LoadDevInfo(AnsiString _sDevName)
{


}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{


}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;
    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "b1stUsePstU1 " , DevOptn.bUsePstU1  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "b1stUsePstU2 " , DevOptn.bUsePstU2  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "b1stUsePstU3 " , DevOptn.bUsePstU3  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "b1stUsePstU4 " , DevOptn.bUsePstU4  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "b1stUsePstU5 " , DevOptn.bUsePstU5  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "b1stUsePstU6 " , DevOptn.bUsePstU6  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "b1stUsePstU7 " , DevOptn.bUsePstU7  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "b1stUsePstU8 " , DevOptn.bUsePstU8  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "b1stUsePstU9 " , DevOptn.bUsePstU9  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "b1stUsePstU10" , DevOptn.bUsePstU10 );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev1 " , DevOptn.iOriLsrDevU1  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev2 " , DevOptn.iOriLsrDevU2  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev3 " , DevOptn.iOriLsrDevU3  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev4 " , DevOptn.iOriLsrDevU4  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev5 " , DevOptn.iOriLsrDevU5  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev6 " , DevOptn.iOriLsrDevU6  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev7 " , DevOptn.iOriLsrDevU7  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev8 " , DevOptn.iOriLsrDevU8  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev9 " , DevOptn.iOriLsrDevU9  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev10" , DevOptn.iOriLsrDevU10 );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev1 " , DevOptn.iRvsLsrDevU1  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev2 " , DevOptn.iRvsLsrDevU2  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev3 " , DevOptn.iRvsLsrDevU3  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev4 " , DevOptn.iRvsLsrDevU4  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev5 " , DevOptn.iRvsLsrDevU5  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev6 " , DevOptn.iRvsLsrDevU6  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev7 " , DevOptn.iRvsLsrDevU7  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev8 " , DevOptn.iRvsLsrDevU8  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev9 " , DevOptn.iRvsLsrDevU9  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev10" , DevOptn.iRvsLsrDevU10 );
}


void COptionMan::SaveDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "b1stUsePstU1 " , DevOptn.bUsePstU1  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "b1stUsePstU2 " , DevOptn.bUsePstU2  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "b1stUsePstU3 " , DevOptn.bUsePstU3  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "b1stUsePstU4 " , DevOptn.bUsePstU4  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "b1stUsePstU5 " , DevOptn.bUsePstU5  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "b1stUsePstU6 " , DevOptn.bUsePstU6  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "b1stUsePstU7 " , DevOptn.bUsePstU7  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "b1stUsePstU8 " , DevOptn.bUsePstU8  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "b1stUsePstU9 " , DevOptn.bUsePstU9  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "b1stUsePstU10" , DevOptn.bUsePstU10 );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev1 " , DevOptn.iOriLsrDevU1  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev2 " , DevOptn.iOriLsrDevU2  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev3 " , DevOptn.iOriLsrDevU3  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev4 " , DevOptn.iOriLsrDevU4  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev5 " , DevOptn.iOriLsrDevU5  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev6 " , DevOptn.iOriLsrDevU6  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev7 " , DevOptn.iOriLsrDevU7  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev8 " , DevOptn.iOriLsrDevU8  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev9 " , DevOptn.iOriLsrDevU9  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriU1stLsrDev10" , DevOptn.iOriLsrDevU10 );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev1 " , DevOptn.iRvsLsrDevU1  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev2 " , DevOptn.iRvsLsrDevU2  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev3 " , DevOptn.iRvsLsrDevU3  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev4 " , DevOptn.iRvsLsrDevU4  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev5 " , DevOptn.iRvsLsrDevU5  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev6 " , DevOptn.iRvsLsrDevU6  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev7 " , DevOptn.iRvsLsrDevU7  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev8 " , DevOptn.iRvsLsrDevU8  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev9 " , DevOptn.iRvsLsrDevU9  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRvsU1stLsrDev10" , DevOptn.iRvsLsrDevU10 );


    SaveLastInfo();
}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bDoorSkip     ", CmnOptn.bDoorSkip     );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bLaserSkip    ", CmnOptn.bLaserSkip    );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bVaccumSkip   ", CmnOptn.bVaccumSkip   );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bBrd2LsrSkip  ", CmnOptn.bBrd2LsrSkip  );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bUseOnlyRgh   ", CmnOptn.bUseOnlyRgh   );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bDryRun       ", CmnOptn.bDryRun       );
    UserINI.Load(sPath.c_str() , "CmnOptn" , "bNotUsedVAxis ", CmnOptn.bNotUsedVAxis );


}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    UserINI.ClearFile(sPath) ;
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bDoorSkip     ", CmnOptn.bDoorSkip     );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bLaserSkip    ", CmnOptn.bLaserSkip    );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bVaccumSkip   ", CmnOptn.bVaccumSkip   );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bBrd2LsrSkip  ", CmnOptn.bBrd2LsrSkip  );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bUseOnlyRgh   ", CmnOptn.bUseOnlyRgh   );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bDryRun       ", CmnOptn.bDryRun       );
    UserINI.Save(sPath.c_str() , "CmnOptn" , "bNotUsedVAxis ", CmnOptn.bNotUsedVAxis );


}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str() , "MstOptn" , "iCycleTm"   , MstOptn.iCycleTm   );


}

void COptionMan::SaveMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str() , "MstOptn" , "iCycleTm"   , MstOptn.iCycleTm   );


}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRL-620D";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName    );
    UserINI.Load(sPath, "EqpOptn"  , "iTotalWork    " , EqpOptn.iTotalWork    );

}
void COptionMan::SaveEqpOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save.
    UserINI.Save(sPath, "EqpOptn"  , "sModelName    " ,  EqpOptn.sModelName    );
    UserINI.Save(sPath, "EqpOptn"  , "iTotalWork    " ,  EqpOptn.iTotalWork    );
}

void COptionMan::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO" , "Device "  , m_sCrntDev );

    if (m_sCrntDev == "") m_sCrntDev  = "NONE";
}

void COptionMan::SaveLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Save
    UserINI.Save(sPath, "LAST WORK INFO" , "Device "  , m_sCrntDev );

}

