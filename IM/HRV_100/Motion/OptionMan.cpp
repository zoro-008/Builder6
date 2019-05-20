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

}

//---------------------------------------------------------------------------
void COptionMan::Init(void)
{
    //Init. Buffer.
    memset(&MstDevOptn , 0 , sizeof(MstDevOptn ));
    memset(&DevInfo    , 0 , sizeof(DevInfo    ));
    memset(&DevOptn    , 0 , sizeof(DevOptn    ));
    memset(&CmnOptn    , 0 , sizeof(CmnOptn    ));
    memset(&MstOptn    , 0 , sizeof(MstOptn    ));
    memset(&EqpOptn    , 0 , sizeof(EqpOptn    ));

    LoadLastInfo(          );
    LoadMstOptn (          );
    LoadEqpOptn (          );
    LoadCmnOptn (          );
    LoadJobFile (m_sCrntDev);

    for(int i = 0 ; i <MAX_STAGE_CNT ; i++)m_bSaved[i] = true ; //화면 그리는것 처음에 한번 그리기 위해.
}
void COptionMan::Close(void)
{
    SaveLastInfo();
}

void COptionMan::SetCrntDev (AnsiString _sName)
{
    m_sCrntDev = _sName ;
}

void COptionMan::LoadJobFile(AnsiString _sDevName)
{
    LoadMstDevOptn(_sDevName);
    LoadDevInfo   (_sDevName);
    LoadDevOptn   (_sDevName);
    //Set Current Device Name.
    SetCrntDev    (_sDevName);
}


void COptionMan::SaveJobFile(AnsiString _sDevName)
{
    SaveMstDevOptn(_sDevName);
    SaveDevInfo   (_sDevName);
    SaveDevOptn   (_sDevName);
    //Set Current Device Name.
    SetCrntDev    (_sDevName);
}

void COptionMan::LoadMstDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\MstDevOptn.INI" ;


    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dMstOfsX0"    , MstDevOptn.dMstOfsX[0]         );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dMstOfsX1"    , MstDevOptn.dMstOfsX[1]         );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dMstOfsX2"    , MstDevOptn.dMstOfsX[2]         );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dMstOfsX3"    , MstDevOptn.dMstOfsX[3]         );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dMstOfsX4"    , MstDevOptn.dMstOfsX[4]         );

    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dMstOfsY0"    , MstDevOptn.dMstOfsY[0]         );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dMstOfsY1"    , MstDevOptn.dMstOfsY[1]         );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dMstOfsY2"    , MstDevOptn.dMstOfsY[2]         );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dMstOfsY3"    , MstDevOptn.dMstOfsY[3]         );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dMstOfsY4"    , MstDevOptn.dMstOfsY[4]         );

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dGainPreLoad       " , MstDevOptn.dGainPreLoad       ); UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dOfstPreLoad       " , MstDevOptn.dOfstPreLoad       );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dGainPeakStroke    " , MstDevOptn.dGainPeakStroke    ); UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dOfstPeakStroke    " , MstDevOptn.dOfstPeakStroke    );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dGainCustomStroke  " , MstDevOptn.dGainCustomStroke  ); UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dOfstCustomStroke  " , MstDevOptn.dOfstCustomStroke  );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dGainSensitivity   " , MstDevOptn.dGainSensitivity   ); UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dOfstSensitivity   " , MstDevOptn.dOfstSensitivity   );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dGainLinearity     " , MstDevOptn.dGainLinearity     ); UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dOfstLinearity     " , MstDevOptn.dOfstLinearity     );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dGainImpedance     " , MstDevOptn.dGainImpedance     ); UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dOfstImpedance     " , MstDevOptn.dOfstImpedance     );
    UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dGainHysteresis    " , MstDevOptn.dGainHysteresis    ); UserINI.Load(sPath.c_str()  , "MstDevOptn"  , "dOfstHysteresis    " , MstDevOptn.dOfstHysteresis    );




    //MstDevOptn.d2GainPreLoad      =  ; d2OfstPreLoad       ;
    //MstDevOptn.d2GainPeakStroke   =  ; d2OfstPeakStroke    ;
    //MstDevOptn.d2GainCustomStroke =  ; d2OfstCustomStroke  ;
    //MstDevOptn.d2GainSensitivity  =  ; d2OfstSensitivity   ;
    //MstDevOptn.d2GainLinearity    =  ; d2OfstLinearity     ;
    //MstDevOptn.d2GainImpedance    =  ; d2OfstImpedance     ;
    //MstDevOptn.d2GainHysteresis   =  ; d2OfstHysteresis    ;

    //MstDevOptn.d2GaindTiltInitR   =  ; d2OfstdTiltInitR    ;
    //MstDevOptn.d2GaindTiltInitT   =  ; d2OfstdTiltInitT    ;
    //MstDevOptn.d2GaindTiltInit    =  ; d2OfstdTiltInit     ;
    //MstDevOptn.d2GaindTiltDrive   =  ; d2OfstdTiltDrive    ;
    //MstDevOptn.d2GaindTiltDriveAbs=  ; d2OfstdTiltDriveAbs ;
    //MstDevOptn.d2GaindTiltFull    =  ; d2OfstdTiltFull     ;
    //MstDevOptn.d2GaindTiltMax     =  ; d2OfstdTiltMax      ;










}

void COptionMan::SaveMstDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\MstDevOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dMstOfsX0"    , MstDevOptn.dMstOfsX[0]         );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dMstOfsX1"    , MstDevOptn.dMstOfsX[1]         );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dMstOfsX2"    , MstDevOptn.dMstOfsX[2]         );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dMstOfsX3"    , MstDevOptn.dMstOfsX[3]         );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dMstOfsX4"    , MstDevOptn.dMstOfsX[4]         );

    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dMstOfsY0"    , MstDevOptn.dMstOfsY[0]         );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dMstOfsY1"    , MstDevOptn.dMstOfsY[1]         );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dMstOfsY2"    , MstDevOptn.dMstOfsY[2]         );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dMstOfsY3"    , MstDevOptn.dMstOfsY[3]         );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dMstOfsY4"    , MstDevOptn.dMstOfsY[4]         );


    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dGainPreLoad      " , MstDevOptn.dGainPreLoad      ); UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dOfstPreLoad      " , MstDevOptn.dOfstPreLoad      );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dGainPeakStroke   " , MstDevOptn.dGainPeakStroke   ); UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dOfstPeakStroke   " , MstDevOptn.dOfstPeakStroke   );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dGainCustomStroke " , MstDevOptn.dGainCustomStroke ); UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dOfstCustomStroke " , MstDevOptn.dOfstCustomStroke );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dGainSensitivity  " , MstDevOptn.dGainSensitivity  ); UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dOfstSensitivity  " , MstDevOptn.dOfstSensitivity  );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dGainLinearity    " , MstDevOptn.dGainLinearity    ); UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dOfstLinearity    " , MstDevOptn.dOfstLinearity    );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dGainImpedance    " , MstDevOptn.dGainImpedance    ); UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dOfstImpedance    " , MstDevOptn.dOfstImpedance    );
    UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dGainHysteresis   " , MstDevOptn.dGainHysteresis   ); UserINI.Save(sPath.c_str()  , "MstDevOptn"  , "dOfstHysteresis   " , MstDevOptn.dOfstHysteresis   );


}

void COptionMan::LoadDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
    //UserINI.Load(sPath.c_str() , "DevInfo" , "dLDRColPitch " , DevInfo.dLDRColPitch  ); if(!DevInfo.dLDRColPitch   )DevInfo.dLDRColPitch    = 1.0 ;

}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
    //UserINI.Save(sPath.c_str() , "DevInfo" , "dLDRColPitch " , DevInfo.dLDRColPitch  );
}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseDuplex        " , DevOptn.bUseDuplex      );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iTestCrntMin      " , DevOptn.iTestCrntMin    ); 
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iTestCrnt         " , DevOptn.iTestCrntMax    ); //로컬세팅을 못바꿔서 "iTestCrnt"로 남겨둠.

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iStableDelay      " , DevOptn.iStableDelay    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iStepDelay        " , DevOptn.iStepDelay      );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iStepIncrement    " , DevOptn.iStepIncrement  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDrvTtStrtCrnt    " , DevOptn.iDrvTtStrtCrnt  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iDrvTtTestCrnt    " , DevOptn.iDrvTtTestCrnt  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iCustomStroke     " , DevOptn.iCustomStroke   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPreLoadCheckHt   " , DevOptn.iPreLoadCheckHt );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iImpdCheckCrnt    " , DevOptn.iImpdCheckCrnt  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSensiSttCrnt     " , DevOptn.iSensiSttCrnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSensiEndCrnt     " , DevOptn.iSensiEndCrnt   );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLinearSttCrnt    " , DevOptn.iLinearSttCrnt  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLinearEndCrnt    " , DevOptn.iLinearEndCrnt  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseBfCttOfs      " , DevOptn.bUseBfCttOfs    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iGraphMinY        " , DevOptn.iGraphMinY      ); //그래프 Y축은 정확하게 유추가 어려워 세팅으로 뺌.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iGraphMaxY        " , DevOptn.iGraphMaxY      );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iHysteSttCrnt     " , DevOptn.iHysteSttCrnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iHysteEndCrnt     " , DevOptn.iHysteEndCrnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPreHysteMaxOfst  " , DevOptn.dPreHysteMaxOfst);



    if(DevOptn.iGraphMinY == 0 && DevOptn.iGraphMaxY == 0 ){ //20150405 기존 잡파일은 세이브가 안되어 있으니 이렇게 처리....
        DevOptn.iGraphMinY = 0 ;
        DevOptn.iGraphMaxY = 300 ;
    }


    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseTiltInitR     " , DevOptn.bUseTiltInitR   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseTiltInitT     " , DevOptn.bUseTiltInitT   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseTiltInit      " , DevOptn.bUseTiltInit    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseTiltDrive     " , DevOptn.bUseTiltDrive   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseTiltDriveAbs  " , DevOptn.bUseTiltDriveAbs);
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseTiltFull      " , DevOptn.bUseTiltFull    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseTiltMax       " , DevOptn.bUseTiltMax     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseTiltLength    " , DevOptn.bUseTiltLength  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUsePreLoad       " , DevOptn.bUsePreLoad     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUsePeakStroke    " , DevOptn.bUsePeakStroke  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseCustomStroke  " , DevOptn.bUseCustomStroke);
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseSensitivity   " , DevOptn.bUseSensitivity );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseLinearity     " , DevOptn.bUseLinearity   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseImpedance     " , DevOptn.bUseImpedance   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseHysteresis    " , DevOptn.bUseHysteresis  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bUseCaseShort     " , DevOptn.bUseCaseShort   );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinTiltInitR     " , DevOptn.dMinTiltInitR   ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxTiltInitR     " , DevOptn.dMaxTiltInitR   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinTiltInitT     " , DevOptn.dMinTiltInitT   ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxTiltInitT     " , DevOptn.dMaxTiltInitT   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinTiltInit      " , DevOptn.dMinTiltInit    ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxTiltInit      " , DevOptn.dMaxTiltInit    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinTiltDrive     " , DevOptn.dMinTiltDrive   ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxTiltDrive     " , DevOptn.dMaxTiltDrive   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinTiltDriveAbs  " , DevOptn.dMinTiltDriveAbs); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxTiltDriveAbs  " , DevOptn.dMaxTiltDriveAbs);
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinTiltFull      " , DevOptn.dMinTiltFull    ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxTiltFull      " , DevOptn.dMaxTiltFull    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinTiltMax       " , DevOptn.dMinTiltMax     ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxTiltMax       " , DevOptn.dMaxTiltMax     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinTiltLength    " , DevOptn.dMinTiltLength  ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxTiltLength    " , DevOptn.dMaxTiltLength  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinPreLoad       " , DevOptn.dMinPreLoad     ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxPreLoad       " , DevOptn.dMaxPreLoad     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinPeakStroke    " , DevOptn.dMinPeakStroke  ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxPeakStroke    " , DevOptn.dMaxPeakStroke  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinCustomStroke  " , DevOptn.dMinCustomStroke); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxCustomStroke  " , DevOptn.dMaxCustomStroke);
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinSensitivity   " , DevOptn.dMinSensitivity ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxSensitivity   " , DevOptn.dMaxSensitivity );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinLinearity     " , DevOptn.dMinLinearity   ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxLinearity     " , DevOptn.dMaxLinearity   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinImpedance     " , DevOptn.dMinImpedance   ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxImpedance     " , DevOptn.dMaxImpedance   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMinHysteresis    " , DevOptn.dMinHysteresis  ); UserINI.Load(sPath.c_str()  , "DevOptn"  , "dMaxHysteresis    " , DevOptn.dMaxHysteresis  );


    
    
    
    
    
    

    
    
    
    
    
    
    

    for(int i = 0 ; i <MAX_STAGE_CNT ; i++)m_bSaved[i] = true ; //화면 그리는것  한번 그리기 위해.
}

void COptionMan::SaveDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Load Device.
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseDuplex       " , DevOptn.bUseDuplex      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iTestCrntMin     " , DevOptn.iTestCrntMin    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iTestCrnt        " , DevOptn.iTestCrntMax    ); //로컬세팅을 못바꿔서 "iTestCrnt"로 남겨둠.

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iStableDelay     " , DevOptn.iStableDelay    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iStepDelay       " , DevOptn.iStepDelay      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iStepIncrement   " , DevOptn.iStepIncrement  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDrvTtStrtCrnt   " , DevOptn.iDrvTtStrtCrnt  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iDrvTtTestCrnt   " , DevOptn.iDrvTtTestCrnt  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iCustomStroke    " , DevOptn.iCustomStroke   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPreLoadCheckHt  " , DevOptn.iPreLoadCheckHt );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iImpdCheckCrnt   " , DevOptn.iImpdCheckCrnt  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSensiSttCrnt    " , DevOptn.iSensiSttCrnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSensiEndCrnt    " , DevOptn.iSensiEndCrnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLinearSttCrnt   " , DevOptn.iLinearSttCrnt  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLinearEndCrnt   " , DevOptn.iLinearEndCrnt  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseBfCttOfs     " , DevOptn.bUseBfCttOfs    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iGraphMinY       " , DevOptn.iGraphMinY      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iGraphMaxY       " , DevOptn.iGraphMaxY      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iHysteSttCrnt    " , DevOptn.iHysteSttCrnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iHysteEndCrnt    " , DevOptn.iHysteEndCrnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPreHysteMaxOfst " , DevOptn.dPreHysteMaxOfst);


    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseTiltInitR    " , DevOptn.bUseTiltInitR   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseTiltInitT    " , DevOptn.bUseTiltInitT   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseTiltInit     " , DevOptn.bUseTiltInit    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseTiltDrive    " , DevOptn.bUseTiltDrive   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseTiltDriveAbs " , DevOptn.bUseTiltDriveAbs);
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseTiltFull     " , DevOptn.bUseTiltFull    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseTiltMax      " , DevOptn.bUseTiltMax     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseTiltLength   " , DevOptn.bUseTiltLength  );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUsePreLoad      " , DevOptn.bUsePreLoad     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUsePeakStroke   " , DevOptn.bUsePeakStroke  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseCustomStroke " , DevOptn.bUseCustomStroke);
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseSensitivity  " , DevOptn.bUseSensitivity );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseLinearity    " , DevOptn.bUseLinearity   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseImpedance    " , DevOptn.bUseImpedance   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseHysteresis   " , DevOptn.bUseHysteresis  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bUseCaseShort    " , DevOptn.bUseCaseShort   );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinTiltInitR    " , DevOptn.dMinTiltInitR   );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxTiltInitR    " , DevOptn.dMaxTiltInitR   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinTiltInitT    " , DevOptn.dMinTiltInitT   );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxTiltInitT    " , DevOptn.dMaxTiltInitT   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinTiltInit     " , DevOptn.dMinTiltInit    );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxTiltInit     " , DevOptn.dMaxTiltInit    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinTiltDrive    " , DevOptn.dMinTiltDrive   );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxTiltDrive    " , DevOptn.dMaxTiltDrive   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinTiltDriveAbs " , DevOptn.dMinTiltDriveAbs);  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxTiltDriveAbs " , DevOptn.dMaxTiltDriveAbs);
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinTiltFull     " , DevOptn.dMinTiltFull    );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxTiltFull     " , DevOptn.dMaxTiltFull    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinTiltMax      " , DevOptn.dMinTiltMax     );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxTiltMax      " , DevOptn.dMaxTiltMax     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinTiltLength   " , DevOptn.dMinTiltLength  );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxTiltLength   " , DevOptn.dMaxTiltLength  );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinPreLoad      " , DevOptn.dMinPreLoad     );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxPreLoad      " , DevOptn.dMaxPreLoad     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinPeakStroke   " , DevOptn.dMinPeakStroke  );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxPeakStroke   " , DevOptn.dMaxPeakStroke  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinCustomStroke " , DevOptn.dMinCustomStroke);  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxCustomStroke " , DevOptn.dMaxCustomStroke);
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinSensitivity  " , DevOptn.dMinSensitivity );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxSensitivity  " , DevOptn.dMaxSensitivity );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinLinearity    " , DevOptn.dMinLinearity   );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxLinearity    " , DevOptn.dMaxLinearity   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinImpedance    " , DevOptn.dMinImpedance   );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxImpedance    " , DevOptn.dMaxImpedance   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMinHysteresis   " , DevOptn.dMinHysteresis  );  UserINI.Save(sPath.c_str()  , "DevOptn"  , "dMaxHysteresis   " , DevOptn.dMaxHysteresis  );

    
    
    
    
    
    
    


    
    
    
    
    


    for(int i = 0 ; i <MAX_STAGE_CNT ; i++)m_bSaved[i] = true ; //화면 그리는것  한번 그리기 위해.
}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sDataSavePath    " , CmnOptn.sDataSavePath    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAutoSaveRsltData" , CmnOptn.bAutoSaveRsltData);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAutoSaveRsltImg " , CmnOptn.bAutoSaveRsltImg );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bAutoSaveRsltSc  " , CmnOptn.bAutoSaveRsltSc  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseStage1       " , CmnOptn.bUseStage1       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseStage2       " , CmnOptn.bUseStage2       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseStage3       " , CmnOptn.bUseStage3       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseStage4       " , CmnOptn.bUseStage4       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseStage5       " , CmnOptn.bUseStage5       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dRegiGain        " , CmnOptn.dRegiGain        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dRegiofset       " , CmnOptn.dRegiofset       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dVisnYMaxDpMin   " , CmnOptn.dVisnYMaxDpMin   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dCrntGain        " , CmnOptn.dCrntGain        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dCrntOfst        " , CmnOptn.dCrntOfst        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dHghtGain        " , CmnOptn.dHghtGain        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dHghtOfst        " , CmnOptn.dHghtOfst        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDoubleHol       " , CmnOptn.bDoubleHold      );



}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sDataSavePath    " , CmnOptn.sDataSavePath    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoSaveRsltData" , CmnOptn.bAutoSaveRsltData);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoSaveRsltImg " , CmnOptn.bAutoSaveRsltImg );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoSaveRsltSc  " , CmnOptn.bAutoSaveRsltSc  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseStage1       " , CmnOptn.bUseStage1       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseStage2       " , CmnOptn.bUseStage2       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseStage3       " , CmnOptn.bUseStage3       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseStage4       " , CmnOptn.bUseStage4       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseStage5       " , CmnOptn.bUseStage5       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dRegiGain        " , CmnOptn.dRegiGain        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dRegiofset       " , CmnOptn.dRegiofset       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dVisnYMaxDpMin   " , CmnOptn.dVisnYMaxDpMin   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dCrntGain        " , CmnOptn.dCrntGain        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dCrntOfst        " , CmnOptn.dCrntOfst        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dHghtGain        " , CmnOptn.dHghtGain        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dHghtOfst        " , CmnOptn.dHghtOfst        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDoubleHol       " , CmnOptn.bDoubleHold      );

}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode  " , MstOptn.bDebugMode          );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dVisnYMaxMin" , MstOptn.dVisnYMaxMin        ); //여기 CmnOptn 오타 있음... 나중에 현장갈일 있으면 MstOptn 교체하기....

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bSetUnitHeight" , MstOptn.bSetUnitHeight    );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainPreLoad      " , MstOptn.d2GainPreLoad      ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstPreLoad      " , MstOptn.d2OfstPreLoad      );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainPeakStroke   " , MstOptn.d2GainPeakStroke   ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstPeakStroke   " , MstOptn.d2OfstPeakStroke   );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainCustomStroke " , MstOptn.d2GainCustomStroke ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstCustomStroke " , MstOptn.d2OfstCustomStroke );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainSensitivity  " , MstOptn.d2GainSensitivity  ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstSensitivity  " , MstOptn.d2OfstSensitivity  );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainLinearity    " , MstOptn.d2GainLinearity    ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstLinearity    " , MstOptn.d2OfstLinearity    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainImpedance    " , MstOptn.d2GainImpedance    ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstImpedance    " , MstOptn.d2OfstImpedance    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainHysteresis   " , MstOptn.d2GainHysteresis   ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstHysteresis   " , MstOptn.d2OfstHysteresis   );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainTiltInitR    " , MstOptn.d2GainTiltInitR    ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstTiltInitR    " , MstOptn.d2OfstTiltInitR    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainTiltInitT    " , MstOptn.d2GainTiltInitT    ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstTiltInitT    " , MstOptn.d2OfstTiltInitT    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainTiltInit     " , MstOptn.d2GainTiltInit     ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstTiltInit     " , MstOptn.d2OfstTiltInit     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainTiltDrive    " , MstOptn.d2GainTiltDrive    ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstTiltDrive    " , MstOptn.d2OfstTiltDrive    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainTiltDriveAbs " , MstOptn.d2GainTiltDriveAbs ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstTiltDriveAbs " , MstOptn.d2OfstTiltDriveAbs );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainTiltFull     " , MstOptn.d2GainTiltFull     ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstTiltFull     " , MstOptn.d2OfstTiltFull     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainTiltMax      " , MstOptn.d2GainTiltMax      ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstTiltMax      " , MstOptn.d2OfstTiltMax      );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2GainTiltLength   " , MstOptn.d2GainTiltLength   ); UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2OfstTiltLength   " , MstOptn.d2OfstTiltLength   );
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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode  " , MstOptn.bDebugMode         );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dVisnYMaxMin" , MstOptn.dVisnYMaxMin       ); //여기 CmnOptn 오타 있음... 나중에 현장갈일 있으면 MstOptn 교체하기....

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainPreLoad     " , MstOptn.d2GainPreLoad     ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstPreLoad     " , MstOptn.d2OfstPreLoad     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainPeakStroke  " , MstOptn.d2GainPeakStroke  ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstPeakStroke  " , MstOptn.d2OfstPeakStroke  );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainCustomStroke" , MstOptn.d2GainCustomStroke); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstCustomStroke" , MstOptn.d2OfstCustomStroke);
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainSensitivity " , MstOptn.d2GainSensitivity ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstSensitivity " , MstOptn.d2OfstSensitivity );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainLinearity   " , MstOptn.d2GainLinearity   ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstLinearity   " , MstOptn.d2OfstLinearity   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainImpedance   " , MstOptn.d2GainImpedance   ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstImpedance   " , MstOptn.d2OfstImpedance   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainHysteresis  " , MstOptn.d2GainHysteresis  ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstHysteresis  " , MstOptn.d2OfstHysteresis  );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainTiltInitR   " , MstOptn.d2GainTiltInitR   ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstTiltInitR   " , MstOptn.d2OfstTiltInitR   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainTiltInitT   " , MstOptn.d2GainTiltInitT   ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstTiltInitT   " , MstOptn.d2OfstTiltInitT   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainTiltInit    " , MstOptn.d2GainTiltInit    ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstTiltInit    " , MstOptn.d2OfstTiltInit    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainTiltDrive   " , MstOptn.d2GainTiltDrive   ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstTiltDrive   " , MstOptn.d2OfstTiltDrive   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainTiltDriveAbs" , MstOptn.d2GainTiltDriveAbs); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstTiltDriveAbs" , MstOptn.d2OfstTiltDriveAbs);
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainTiltFull    " , MstOptn.d2GainTiltFull    ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstTiltFull    " , MstOptn.d2OfstTiltFull    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainTiltMax     " , MstOptn.d2GainTiltMax     ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstTiltMax     " , MstOptn.d2OfstTiltMax     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2GainTiltLength  " , MstOptn.d2GainTiltLength  ); UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2OfstTiltLength  " , MstOptn.d2OfstTiltLength  );





    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bSetUnitHeight" , MstOptn.bSetUnitHeight    );

}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRI_530V";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EqpOptn.INI" ;

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );

    if(EqpOptn.sModelName =="") EqpOptn.sModelName = "HRI_530V";

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
    UserINI.Save(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );
}

void COptionMan::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO" , "Device"  , m_sCrntDev );

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
    UserINI.Save(sPath, "LAST WORK INFO" , "Device"  , m_sCrntDev );
}



