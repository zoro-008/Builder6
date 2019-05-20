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
    memset(&DevInfo  , 0 , sizeof(DevInfo ));  //
    memset(&DevOptn  , 0 , sizeof(DevOptn ));  //sOSTPinName 가 String 이라 계속 하면 메모리 누수 되지만 일단 한번만 하기에 냄겨둠.
    memset(&CmnOptn  , 0 , sizeof(CmnOptn ));  //sRj1Name  가 String 이라 계속 하면 메모리 누수 되지만 일단 한번만 하기에 냄겨둠.
    memset(&MstOptn  , 0 , sizeof(MstOptn ));
    memset(&EqpOptn  , 0 , sizeof(EqpOptn ));

    LoadLastInfo(          );
    LoadMstOptn (          );
    LoadEqpOptn (          );
    LoadCmnOptn (          );
    LoadJobFile (m_sCrntDev);
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
    LoadDevInfo (_sDevName);
    LoadDevOptn (_sDevName);
    //Set Current Device Name.
    SetCrntDev  (_sDevName);
}


void COptionMan::SaveJobFile(AnsiString _sDevName)
{
    SaveDevInfo (_sDevName);
    SaveDevOptn (_sDevName);
    //Set Current Device Name.
    SetCrntDev  (_sDevName);
}

void COptionMan::LoadDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
    int iTemp ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iWorkMode        " , iTemp                      ); DevInfo.iWorkMode = iTemp;

    UserINI.Load(sPath.c_str() , "DevInfo" , "dMgzSlotPitch    " , DevInfo.dMgzSlotPitch      ); if(!DevInfo.dMgzSlotPitch       )DevInfo.dMgzSlotPitch        = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iMgzSlotCnt      " , DevInfo.iMgzSlotCnt        ); if(!DevInfo.iMgzSlotCnt         )DevInfo.iMgzSlotCnt          = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dMgzTopToFrstSlot" , DevInfo.dMgzTopToFrstSlot  ); if(!DevInfo.dMgzTopToFrstSlot   )DevInfo.dMgzTopToFrstSlot    = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dMgzBtmToLastSlot" , DevInfo.dMgzBtmToLastSlot  ); if(!DevInfo.dMgzBtmToLastSlot   )DevInfo.dMgzBtmToLastSlot    = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iBoatColCnt      " , DevInfo.iBoatColCnt        ); if(!DevInfo.iBoatColCnt         )DevInfo.iBoatColCnt          = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dBoatColPitch    " , DevInfo.dBoatColPitch      ); if(!DevInfo.dBoatColPitch       )DevInfo.dBoatColPitch        = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dFosWidth        " , DevInfo.dFosWidth          ); if(!DevInfo.dFosWidth           )DevInfo.dFosWidth            = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dFosHeight       " , DevInfo.dFosHeight         ); if(!DevInfo.dFosHeight          )DevInfo.dFosHeight           = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dFosChamper      " , DevInfo.dFosChamper        ); if(!DevInfo.dFosChamper         )DevInfo.dFosChamper          = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dHicWidth        " , DevInfo.dHicWidth          ); if(!DevInfo.dHicWidth           )DevInfo.dHicWidth            = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dHicHeight       " , DevInfo.dHicHeight         ); if(!DevInfo.dHicHeight          )DevInfo.dHicHeight           = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dHicChamper      " , DevInfo.dHicChamper        ); if(!DevInfo.dHicChamper         )DevInfo.dHicChamper          = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dDieWidth        " , DevInfo.dDieWidth          ); if(!DevInfo.dDieWidth           )DevInfo.dDieWidth            = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dDieHeight       " , DevInfo.dDieHeight         ); if(!DevInfo.dDieHeight          )DevInfo.dDieHeight           = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dDieChamper      " , DevInfo.dDieChamper        ); if(!DevInfo.dDieChamper         )DevInfo.dDieChamper          = 1 ;
}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
    UserINI.Save(sPath.c_str() , "DevInfo" , "iWorkMode            " , DevInfo.iWorkMode         );

    UserINI.Save(sPath.c_str() , "DevInfo" , "dMgzSlotPitch        " , DevInfo.dMgzSlotPitch     );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iMgzSlotCnt          " , DevInfo.iMgzSlotCnt       );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dMgzTopToFrstSlot    " , DevInfo.dMgzTopToFrstSlot );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dMgzBtmToLastSlot    " , DevInfo.dMgzBtmToLastSlot );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iBoatColCnt          " , DevInfo.iBoatColCnt       );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dBoatColPitch        " , DevInfo.dBoatColPitch     );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dFosWidth            " , DevInfo.dFosWidth         );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dFosHeight           " , DevInfo.dFosHeight        );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dFosChamper          " , DevInfo.dFosChamper       );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dHicWidth            " , DevInfo.dHicWidth         );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dHicHeight           " , DevInfo.dHicHeight        );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dHicChamper          " , DevInfo.dHicChamper       );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dDieWidth            " , DevInfo.dDieWidth         );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dDieHeight           " , DevInfo.dDieHeight        );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dDieChamper          " , DevInfo.dDieChamper       );
}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str() , "DevOptn" , "bStIgnrVisnBefore " , DevOptn.bStIgnrVisnBefore  );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bStIgnrVisnAfter  " , DevOptn.bStIgnrVisnAfter   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bPkIgnrAlign      " , DevOptn.bPkIgnrAlign       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bPkIgnrVisnAlign  " , DevOptn.bPkIgnrVisnAlign   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bPkIgnrVisnDefect " , DevOptn.bPkIgnrVisnDefect  );

    UserINI.Load(sPath.c_str() , "DevOptn" , "iPkPickDelay      " , DevOptn.iPkPickDelay       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iFrstPkgDelay     " , DevOptn.iFrstPkgDelay      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dPlaceYOfs        " , DevOptn.dPlaceYOfs         );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dPlaceXOfs        " , DevOptn.dPlaceXOfs         );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dStageTolXY       " , DevOptn.dStageTolXY        );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dStageTolAng      " , DevOptn.dStageTolAng       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dPickerTolXY      " , DevOptn.dPickerTolXY       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dPickerTolAng     " , DevOptn.dPickerTolAng      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dPlaceTolXY       " , DevOptn.dPlaceTolXY        );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dPlaceTolAng      " , DevOptn.dPlaceTolAng       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dFosPlaceSttYOfs  " , DevOptn.dFosPlaceSttYOfs   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dFosPlaceSttYSpd  " , DevOptn.dFosPlaceSttYSpd   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iFosPlaceBfDelay  " , DevOptn.iFosPlaceBfDelay   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dPlaceZOfs1       " , DevOptn.dPlaceZOfs1        );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dPlaceZOfs2       " , DevOptn.dPlaceZOfs2        );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachSpeed1     " , DevOptn.dAttachSpeed1      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachSpeed2     " , DevOptn.dAttachSpeed2      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachForce      " , DevOptn.dAttachForce       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachForceOfs   " , DevOptn.dAttachForceOfs    );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dDispZOfs         " , DevOptn.dDispZOfs          );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iPkPushDelay      " , DevOptn.iPkPushDelay       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachStageOfsX  " , DevOptn.dAttachStageOfsX   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachStageOfsY  " , DevOptn.dAttachStageOfsY   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachStageOfsT  " , DevOptn.dAttachStageOfsT   );

    UserINI.Load(sPath.c_str() , "DevOptn" , "iRailTopTemp      " , DevOptn.iRailTopTemp       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iRailBtmTemp      " , DevOptn.iRailBtmTemp       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iStageTemp        " , DevOptn.iStageTemp         );

    UserINI.Load(sPath.c_str() , "DevOptn" , "dDspPrsPres       " , DevOptn.dDspPrsPres        );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dDspVacPres       " , DevOptn.dDspVacPres        );

    UserINI.Load(sPath.c_str() , "DevOptn" , "iDspCh            " , DevOptn.iDspCh             );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iDspMinAmount     " , DevOptn.iDspMinAmount      );


    UserINI.Load(sPath.c_str() , "DevOptn" , "dReptSttXPos      " , DevOptn.dReptSttXPos       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dReptSttYPos      " , DevOptn.dReptSttYPos       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dReptEndXPos      " , DevOptn.dReptEndXPos       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dReptEndYPos      " , DevOptn.dReptEndYPos       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dReptSpd          " , DevOptn.dReptSpd           );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iReptCnt          " , DevOptn.iReptCnt           );

    UserINI.Load(sPath.c_str() , "DevOptn" , "sEpoxyName        " , DevOptn.sEpoxyName         );
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
    UserINI.Save(sPath.c_str() , "DevOptn" , "bStIgnrVisnBefore " , DevOptn.bStIgnrVisnBefore  );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bStIgnrVisnAfter  " , DevOptn.bStIgnrVisnAfter   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bPkIgnrAlign      " , DevOptn.bPkIgnrAlign       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bPkIgnrVisnAlign  " , DevOptn.bPkIgnrVisnAlign   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bPkIgnrVisnDefect " , DevOptn.bPkIgnrVisnDefect  );

    UserINI.Save(sPath.c_str() , "DevOptn" , "iPkPickDelay      " , DevOptn.iPkPickDelay       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iFrstPkgDelay     " , DevOptn.iFrstPkgDelay      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dPlaceYOfs        " , DevOptn.dPlaceYOfs         );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dPlaceXOfs        " , DevOptn.dPlaceXOfs         );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dStageTolXY       " , DevOptn.dStageTolXY        );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dStageTolAng      " , DevOptn.dStageTolAng       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dPickerTolXY      " , DevOptn.dPickerTolXY       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dPickerTolAng     " , DevOptn.dPickerTolAng      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dPlaceTolXY       " , DevOptn.dPlaceTolXY        );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dPlaceTolAng      " , DevOptn.dPlaceTolAng       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dFosPlaceSttYOfs  " , DevOptn.dFosPlaceSttYOfs   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dFosPlaceSttYSpd  " , DevOptn.dFosPlaceSttYSpd   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iFosPlaceBfDelay  " , DevOptn.iFosPlaceBfDelay   );            
    UserINI.Save(sPath.c_str() , "DevOptn" , "dPlaceZOfs1       " , DevOptn.dPlaceZOfs1        );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dPlaceZOfs2       " , DevOptn.dPlaceZOfs2        );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachSpeed1     " , DevOptn.dAttachSpeed1      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachSpeed2     " , DevOptn.dAttachSpeed2      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachForce      " , DevOptn.dAttachForce       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachForceOfs   " , DevOptn.dAttachForceOfs    );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dDispZOfs         " , DevOptn.dDispZOfs          ); 
    UserINI.Save(sPath.c_str() , "DevOptn" , "iPkPushDelay      " , DevOptn.iPkPushDelay       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachStageOfsX  " , DevOptn.dAttachStageOfsX   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachStageOfsY  " , DevOptn.dAttachStageOfsY   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachStageOfsT  " , DevOptn.dAttachStageOfsT   );

    UserINI.Save(sPath.c_str() , "DevOptn" , "iRailTopTemp      " , DevOptn.iRailTopTemp       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iRailBtmTemp      " , DevOptn.iRailBtmTemp       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iStageTemp        " , DevOptn.iStageTemp         );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dDspPrsPres       " , DevOptn.dDspPrsPres        );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dDspVacPres       " , DevOptn.dDspVacPres        );

    UserINI.Save(sPath.c_str() , "DevOptn" , "iDspCh            " , DevOptn.iDspCh             );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iDspMinAmount     " , DevOptn.iDspMinAmount      );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dReptSttXPos      " , DevOptn.dReptSttXPos       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dReptSttYPos      " , DevOptn.dReptSttYPos       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dReptEndXPos      " , DevOptn.dReptEndXPos       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dReptEndYPos      " , DevOptn.dReptEndYPos       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dReptSpd          " , DevOptn.dReptSpd           );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iReptCnt          " , DevOptn.iReptCnt           );

    UserINI.Save(sPath.c_str() , "DevOptn" , "sEpoxyName        " , DevOptn.sEpoxyName         );

}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;
                                                                         
    //Load Device.
    int iTemp ;
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop      " , CmnOptn.bLoadingStop     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor         " , CmnOptn.bIgnrDoor        );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "bTempIgnrErrorChk " , CmnOptn.bTempIgnrErrorChk);
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dTempErrorTol     " , CmnOptn.dTempErrorTol    );

    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "iDisprClnCnt      " , CmnOptn.iDisprClnCnt     );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "bVisnNgStop       " , CmnOptn.bVisnNgStop      );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "sLotFilePath      " , CmnOptn.sLotFilePath     );

    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dMaxPlaceZOfs1    " , CmnOptn.dMaxPlaceZOfs1   );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dMaxPlaceZOfs2    " , CmnOptn.dMaxPlaceZOfs2   );
    UserINI.Load(sPath.c_str() ,  "CmnOptn"  , "dMaxAttachForce   " , CmnOptn.dMaxAttachForce  );

}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadingStop      " , CmnOptn.bLoadingStop      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor         " , CmnOptn.bIgnrDoor         );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "bTempIgnrErrorChk " , CmnOptn.bTempIgnrErrorChk );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dTempErrorTol     " , CmnOptn.dTempErrorTol     );

    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "iDisprClnCnt      " , CmnOptn.iDisprClnCnt      );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "bVisnNgStop       " , CmnOptn.bVisnNgStop       );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "sLotFilePath      " , CmnOptn.sLotFilePath      );

    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dMaxPlaceZOfs1    " , CmnOptn.dMaxPlaceZOfs1    );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dMaxPlaceZOfs2    " , CmnOptn.dMaxPlaceZOfs2    );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dMaxAttachForce   " , CmnOptn.dMaxAttachForce   );
}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
     UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode     " , MstOptn.bDebugMode      );
     UserINI.Load(sPath.c_str()  , "MstOptn"  , "dPckDspStroke  " , MstOptn.dPckDspStroke   );
     UserINI.Load(sPath.c_str()  , "MstOptn"  , "bSkipPickrDispr" , MstOptn.bSkipPickrDispr );
     UserINI.Load(sPath.c_str()  , "MstOptn"  , "dPckToStgAngleOfs" , MstOptn.dPckToStgAngleOfs );


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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode   " , MstOptn.bDebugMode    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dPckDspStroke" , MstOptn.dPckDspStroke );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bSkipPickrDispr" , MstOptn.bSkipPickrDispr );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dPckToStgAngleOfs" , MstOptn.dPckToStgAngleOfs );

}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRD_220A";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EqpOptn.INI" ;

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );

    if(EqpOptn.sModelName == "") EqpOptn.sModelName = sModelName;


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



