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
    //Load
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

    UserINI.Load(sPath.c_str() , "DevInfo" , "dLDRFMgzSlotPitch" , DevInfo.dLDRFMgzSlotPitch); if(!DevInfo.dLDRFMgzSlotPitch)DevInfo.dLDRFMgzSlotPitch= 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iLDRFMgzSlotCnt  " , DevInfo.iLDRFMgzSlotCnt  ); if(!DevInfo.iLDRFMgzSlotCnt  )DevInfo.iLDRFMgzSlotCnt  = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iLDRFMgzBayCnt   " , DevInfo.iLDRFMgzBayCnt   ); if(!DevInfo.iLDRFMgzBayCnt   )DevInfo.iLDRFMgzBayCnt   = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dLDRRMgzSlotPitch" , DevInfo.dLDRRMgzSlotPitch); if(!DevInfo.dLDRRMgzSlotPitch)DevInfo.dLDRRMgzSlotPitch= 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iLDRRMgzSlotCnt  " , DevInfo.iLDRRMgzSlotCnt  ); if(!DevInfo.iLDRRMgzSlotCnt  )DevInfo.iLDRRMgzSlotCnt  = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iLDRRMgzBayCnt   " , DevInfo.iLDRRMgzBayCnt   ); if(!DevInfo.iLDRRMgzBayCnt   )DevInfo.iLDRRMgzBayCnt   = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dULDMgzSlotPitch " , DevInfo.dULDMgzSlotPitch ); if(!DevInfo.dULDMgzSlotPitch )DevInfo.dULDMgzSlotPitch = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iULDMgzSlotCnt   " , DevInfo.iULDMgzSlotCnt   ); if(!DevInfo.iULDMgzSlotCnt   )DevInfo.iULDMgzSlotCnt   = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iULDMgzBayCnt    " , DevInfo.iULDMgzBayCnt    ); if(!DevInfo.iULDMgzBayCnt    )DevInfo.iULDMgzBayCnt    = 1 ;
//    UserINI.Load(sPath.c_str() , "DevInfo" , "dMidWidth        " , DevInfo.dMidWidth        ); if(!DevInfo.dMidWidth        )DevInfo.dMidWidth        = 1 ;
//    UserINI.Load(sPath.c_str() , "DevInfo" , "dMidHeight       " , DevInfo.dMidHeight       ); if(!DevInfo.dMidHeight       )DevInfo.dMidHeight       = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iMidColCnt       " , DevInfo.iMidColCnt       ); if(!DevInfo.iMidColCnt       )DevInfo.iMidColCnt       = 1 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iMidRowCnt       " , DevInfo.iMidRowCnt       ); if(!DevInfo.iMidRowCnt       )DevInfo.iMidRowCnt       = 1 ;
//    UserINI.Load(sPath.c_str() , "DevInfo" , "dCmsWidth        " , DevInfo.dCmsWidth        ); if(!DevInfo.dCmsWidth        )DevInfo.dCmsWidth        = 1 ;
//    UserINI.Load(sPath.c_str() , "DevInfo" , "dCmsHeight       " , DevInfo.dCmsHeight       ); if(!DevInfo.dCmsHeight       )DevInfo.dCmsHeight       = 1 ;
//    UserINI.Load(sPath.c_str() , "DevInfo" , "dPcbWidth        " , DevInfo.dPcbWidth        ); if(!DevInfo.dPcbWidth        )DevInfo.dPcbWidth        = 1 ;
//    UserINI.Load(sPath.c_str() , "DevInfo" , "dPcbHeight       " , DevInfo.dPcbHeight       ); if(!DevInfo.dPcbHeight       )DevInfo.dPcbHeight       = 1 ;
}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Save Device.
    UserINI.Save(sPath.c_str() , "DevInfo" , "dLDRFMgzSlotPitch" , DevInfo.dLDRFMgzSlotPitch);
    UserINI.Save(sPath.c_str() , "DevInfo" , "iLDRFMgzSlotCnt  " , DevInfo.iLDRFMgzSlotCnt  );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iLDRFMgzBayCnt   " , DevInfo.iLDRFMgzBayCnt   );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dLDRRMgzSlotPitch" , DevInfo.dLDRRMgzSlotPitch);
    UserINI.Save(sPath.c_str() , "DevInfo" , "iLDRRMgzSlotCnt  " , DevInfo.iLDRRMgzSlotCnt  );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iLDRRMgzBayCnt   " , DevInfo.iLDRRMgzBayCnt   );
    UserINI.Save(sPath.c_str() , "DevInfo" , "dULDMgzSlotPitch " , DevInfo.dULDMgzSlotPitch );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iULDMgzSlotCnt   " , DevInfo.iULDMgzSlotCnt   );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iULDMgzBayCnt    " , DevInfo.iULDMgzBayCnt    );
//    UserINI.Save(sPath.c_str() , "DevInfo" , "dMidWidth        " , DevInfo.dMidWidth        );
//    UserINI.Save(sPath.c_str() , "DevInfo" , "dMidHeight       " , DevInfo.dMidHeight       );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iMidColCnt       " , DevInfo.iMidColCnt       );
    UserINI.Save(sPath.c_str() , "DevInfo" , "iMidRowCnt       " , DevInfo.iMidRowCnt       );
//    UserINI.Save(sPath.c_str() , "DevInfo" , "dCmsWidth        " , DevInfo.dCmsWidth        );
//    UserINI.Save(sPath.c_str() , "DevInfo" , "dCmsHeight       " , DevInfo.dCmsHeight       );
//    UserINI.Save(sPath.c_str() , "DevInfo" , "dPcbWidth        " , DevInfo.dPcbWidth        );
//    UserINI.Save(sPath.c_str() , "DevInfo" , "dPcbHeight       " , DevInfo.dPcbHeight       );
}




void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    //UserINI.Load(sPath.c_str() , "DevOptn" , "dStageTolXY     " , DevOptn.dStageTolXY      );
    //UserINI.Load(sPath.c_str() , "DevOptn" , "dStageTolAng    " , DevOptn.dStageTolAng     );
    //UserINI.Load(sPath.c_str() , "DevOptn" , "dPickerTolXY    " , DevOptn.dPickerTolXY     );
    //UserINI.Load(sPath.c_str() , "DevOptn" , "dPickerTolAng   " , DevOptn.dPickerTolAng    );
    //UserINI.Load(sPath.c_str() , "DevOptn" , "dPlaceTolXY     " , DevOptn.dPlaceTolXY      );
    //UserINI.Load(sPath.c_str() , "DevOptn" , "dPlaceTolAng    " , DevOptn.dPlaceTolAng     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dPlaceZOfs       " , DevOptn.dPlaceZOfs       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachSpeed1    " , DevOptn.dAttachSpeed1    );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachSpeed2    " , DevOptn.dAttachSpeed2    );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachForce     " , DevOptn.dAttachForce     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachForceOfs  " , DevOptn.dAttachForceOfs  );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dAttachDelay     " , DevOptn.dAttachDelay     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iFixDelay        " , DevOptn.iFixDelay        );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dDispZOfs        " , DevOptn.dDispZOfs        );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dStageInspOfsX   " , DevOptn.dStageInspOfsX   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dStageInspOfsY   " , DevOptn.dStageInspOfsY   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iDspChFt         " , DevOptn.iDspChFt         );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iDspChRr         " , DevOptn.iDspChRr         );

    UserINI.Load(sPath.c_str() , "DevOptn" , "iDspMinAmount    " , DevOptn.iDspMinAmount    );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dMidCmsGapFrst   " , DevOptn.dMidCmsGapFrst   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dMidCmsGap       " , DevOptn.dMidCmsGap       );
    //UserINI.Load(sPath.c_str() , "DevOptn" , "bEpoxyFirst      " , DevOptn.bEpoxyFirst      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUsePush         " , DevOptn.bUsePush         );
    //UserINI.Load(sPath.c_str() , "DevOptn" , "bUseDispRear     " , DevOptn.bUseDispRear     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dDispAccDcc      " , DevOptn.dDispAccDcc      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iUVTopBrightness " , DevOptn.iUVTopBrightness );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iUVBtmBrightness1" , DevOptn.iUVBtmBrightness1);
    UserINI.Load(sPath.c_str() , "DevOptn" , "iUVBtmBrightness2" , DevOptn.iUVBtmBrightness2);
    UserINI.Load(sPath.c_str() , "DevOptn" , "iUVBtmBrightness3" , DevOptn.iUVBtmBrightness3);
    UserINI.Load(sPath.c_str() , "DevOptn" , "iUVBtmBrightness4" , DevOptn.iUVBtmBrightness4);
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseBtmUV        " , DevOptn.bUseBtmUV        );

    UserINI.Load(sPath.c_str() , "DevOptn" , "dLVisnTolXY      " , DevOptn.dLVisnTolXY      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dLVisnTolAng     " , DevOptn.dLVisnTolAng     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dRVisnTolXY      " , DevOptn.dRVisnTolXY      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dRVisnTolAng     " , DevOptn.dRVisnTolAng     );

    UserINI.Load(sPath.c_str() , "DevOptn" , "dREndVisnTolXY   " , DevOptn.dREndVisnTolXY   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dREndVisnTolAng  " , DevOptn.dREndVisnTolAng  );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dRHgtTolZ        " , DevOptn.dRHgtTolZ        );

    UserINI.Load(sPath.c_str() , "DevOptn" , "dCmsAtachGapX    " , DevOptn.dCmsAtachGapX    );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dCmsAtachGapY    " , DevOptn.dCmsAtachGapY    );

    UserINI.Load(sPath.c_str() , "DevOptn" , "dActiveEdgeGapX  " , DevOptn.dActiveEdgeGapX  );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dActiveEdgeGapY  " , DevOptn.dActiveEdgeGapY  );



    UserINI.Load(sPath.c_str() , "DevOptn" , "iLineHeightCnt   " , DevOptn.iLineHeightCnt   );

    UserINI.Load(sPath.c_str() , "DevOptn" , "iEpoxyDispCntF   " , DevOptn.iEpoxyDispCntF   );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iEpoxyDispCntR   " , DevOptn.iEpoxyDispCntR   );

    UserINI.Load(sPath.c_str() , "DevOptn" , "sEpoxyName       " , DevOptn.sEpoxyName       );

    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Fix1     " , DevOptn.bUseSTG_Fix1     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Fix2     " , DevOptn.bUseSTG_Fix2     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Fix3     " , DevOptn.bUseSTG_Fix3     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Fix4     " , DevOptn.bUseSTG_Fix4     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Fix5     " , DevOptn.bUseSTG_Fix5     );

    UserINI.Load(sPath.c_str() , "DevOptn" , "dDspVacPres      " , DevOptn.dDspVacPres      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dDspPrsPres      " , DevOptn.dDspPrsPres      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "cbNotUseRotate   " , DevOptn.bNotUseRotate    );

    UserINI.Load(sPath.c_str() , "DevOptn" , "iUvCntFrst       " , DevOptn.iUvCntFrst       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iUvCntScnd       " , DevOptn.iUvCntScnd       );

    UserINI.Load(sPath.c_str() , "DevOptn" , "dSlaveHtOfsZ     " , DevOptn.dSlaveHtOfsZ     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dHexaTopHeightOfs" , DevOptn.dHexaTopHeightOfs);

    UserINI.Load(sPath.c_str() , "DevOptn" , "iHeightMethod    " , DevOptn.iHeightMethod    );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dHeightPitchX    " , DevOptn.dHeightPitchX    );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dHeightPitchY    " , DevOptn.dHeightPitchY    );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iHeightStepCnt   " , DevOptn.iHeightStepCnt   );

    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Vac1     " , DevOptn.bUseSTG_Vac1     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Vac2     " , DevOptn.bUseSTG_Vac2     );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Vac3     " , DevOptn.bUseSTG_Vac3     );

    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Vac1Frst " , DevOptn.bUseSTG_Vac1Frst );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Vac2Frst " , DevOptn.bUseSTG_Vac2Frst );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSTG_Vac3Frst " , DevOptn.bUseSTG_Vac3Frst );



    UserINI.Load(sPath.c_str() , "DevOptn" , "dBfUVOfsX00      " , DevOptn.dBfUVOfsX00      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dBfUVOfsX10      " , DevOptn.dBfUVOfsX10      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dBfUVOfsX01      " , DevOptn.dBfUVOfsX01      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dBfUVOfsX11      " , DevOptn.dBfUVOfsX11      );

    UserINI.Load(sPath.c_str() , "DevOptn" , "dBfUVOfsY00      " , DevOptn.dBfUVOfsY00      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dBfUVOfsY10      " , DevOptn.dBfUVOfsY10      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dBfUVOfsY01      " , DevOptn.dBfUVOfsY01      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dBfUVOfsY11      " , DevOptn.dBfUVOfsY11      );

    UserINI.Load(sPath.c_str() , "DevOptn" , "dUVTestSpeed     " , DevOptn.dUVTestSpeed     );

    UserINI.Load(sPath.c_str() , "DevOptn" , "dMBStdHght       " , DevOptn.dMBStdHght       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dMBHghtTol       " , DevOptn.dMBHghtTol       );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dHexaTopStdHght  " , DevOptn.dHexaTopStdHght  );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dHexaTopHghtTol  " , DevOptn.dHexaTopHghtTol  );
    UserINI.Load(sPath.c_str() , "DevOptn" , "dCmsHeightOfs    " , DevOptn.dCmsHeightOfs    );
    //.UserINI.Load(sPath.c_str() , "DevOptn" , "iDispSeq         " , DevOptn.iDispSeq         );

    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseFstDisp      " , DevOptn.bUseFstDisp      );
    UserINI.Load(sPath.c_str() , "DevOptn" , "bUseSecDisp      " , DevOptn.bUseSecDisp      );

    UserINI.Load(sPath.c_str() , "DevOptn" , "iFstDisp         " , DevOptn.iFstDisp         );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iSecDisp         " , DevOptn.iSecDisp         );


}

void COptionMan::SaveDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    //UserINI.Save(sPath.c_str() , "DevOptn" , "dStageTolXY      " , DevOptn.dStageTolXY      );
    //UserINI.Save(sPath.c_str() , "DevOptn" , "dStageTolAng     " , DevOptn.dStageTolAng     );
    //UserINI.Save(sPath.c_str() , "DevOptn" , "dPickerTolXY     " , DevOptn.dPickerTolXY     );
    //UserINI.Save(sPath.c_str() , "DevOptn" , "dPickerTolAng    " , DevOptn.dPickerTolAng    );
    //UserINI.Save(sPath.c_str() , "DevOptn" , "dPlaceTolXY      " , DevOptn.dPlaceTolXY      );
    //UserINI.Save(sPath.c_str() , "DevOptn" , "dPlaceTolAng     " , DevOptn.dPlaceTolAng     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dPlaceZOfs       " , DevOptn.dPlaceZOfs       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachSpeed1    " , DevOptn.dAttachSpeed1    );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachSpeed2    " , DevOptn.dAttachSpeed2    );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachForce     " , DevOptn.dAttachForce     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachForceOfs  " , DevOptn.dAttachForceOfs  );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dAttachDelay     " , DevOptn.dAttachDelay     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iFixDelay        " , DevOptn.iFixDelay        );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dDispZOfs        " , DevOptn.dDispZOfs        );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dStageInspOfsX   " , DevOptn.dStageInspOfsX   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dStageInspOfsY   " , DevOptn.dStageInspOfsY   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iDspChFt         " , DevOptn.iDspChFt         );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iDspChRr         " , DevOptn.iDspChRr         );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iDspMinAmount    " , DevOptn.iDspMinAmount    );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dMidCmsGapFrst   " , DevOptn.dMidCmsGapFrst   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dMidCmsGap       " , DevOptn.dMidCmsGap       );
    //UserINI.Save(sPath.c_str() , "DevOptn" , "bEpoxyFirst      " , DevOptn.bEpoxyFirst      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUsePush         " , DevOptn.bUsePush         );
    //UserINI.Save(sPath.c_str() , "DevOptn" , "bUseDispRear     " , DevOptn.bUseDispRear     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dDispAccDcc      " , DevOptn.dDispAccDcc      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iUVTopBrightness " , DevOptn.iUVTopBrightness );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iUVBtmBrightness1" , DevOptn.iUVBtmBrightness1);
    UserINI.Save(sPath.c_str() , "DevOptn" , "iUVBtmBrightness2" , DevOptn.iUVBtmBrightness2);
    UserINI.Save(sPath.c_str() , "DevOptn" , "iUVBtmBrightness3" , DevOptn.iUVBtmBrightness3);
    UserINI.Save(sPath.c_str() , "DevOptn" , "iUVBtmBrightness4" , DevOptn.iUVBtmBrightness4);
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseBtmUV        " , DevOptn.bUseBtmUV        );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dLVisnTolXY      " , DevOptn.dLVisnTolXY      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dLVisnTolAng     " , DevOptn.dLVisnTolAng     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dRVisnTolXY      " , DevOptn.dRVisnTolXY      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dRVisnTolAng     " , DevOptn.dRVisnTolAng     );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dREndVisnTolXY   " , DevOptn.dREndVisnTolXY   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dREndVisnTolAng  " , DevOptn.dREndVisnTolAng  );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dRHgtTolZ        " , DevOptn.dRHgtTolZ        );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dCmsAtachGapX    " , DevOptn.dCmsAtachGapX    );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dCmsAtachGapY    " , DevOptn.dCmsAtachGapY    );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dActiveEdgeGapX  " , DevOptn.dActiveEdgeGapX  );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dActiveEdgeGapY  " , DevOptn.dActiveEdgeGapY  );

    UserINI.Save(sPath.c_str() , "DevOptn" , "iLineHeightCnt   " , DevOptn.iLineHeightCnt   );

    UserINI.Save(sPath.c_str() , "DevOptn" , "iEpoxyDispCntF   " , DevOptn.iEpoxyDispCntF   );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iEpoxyDispCntR   " , DevOptn.iEpoxyDispCntR   );

    UserINI.Save(sPath.c_str() , "DevOptn" , "sEpoxyName       " , DevOptn.sEpoxyName       );

    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Fix1     " , DevOptn.bUseSTG_Fix1     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Fix2     " , DevOptn.bUseSTG_Fix2     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Fix3     " , DevOptn.bUseSTG_Fix3     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Fix4     " , DevOptn.bUseSTG_Fix4     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Fix5     " , DevOptn.bUseSTG_Fix5     );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dDspVacPres      " , DevOptn.dDspVacPres      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dDspPrsPres      " , DevOptn.dDspPrsPres      );

    UserINI.Save(sPath.c_str() , "DevOptn" , "cbNotUseRotate   " , DevOptn.bNotUseRotate    );

    UserINI.Save(sPath.c_str() , "DevOptn" , "iUvCntFrst       " , DevOptn.iUvCntFrst       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iUvCntScnd       " , DevOptn.iUvCntScnd       );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dSlaveHtOfsZ     " , DevOptn.dSlaveHtOfsZ     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dHexaTopHeightOfs" , DevOptn.dHexaTopHeightOfs);

    UserINI.Save(sPath.c_str() , "DevOptn" , "iHeightMethod    " , DevOptn.iHeightMethod    );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dHeightPitchX    " , DevOptn.dHeightPitchX    );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dHeightPitchY    " , DevOptn.dHeightPitchY    );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iHeightStepCnt   " , DevOptn.iHeightStepCnt   );

    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Vac1     " , DevOptn.bUseSTG_Vac1     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Vac2     " , DevOptn.bUseSTG_Vac2     );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Vac3     " , DevOptn.bUseSTG_Vac3     );

    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Vac1Frst " , DevOptn.bUseSTG_Vac1Frst );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Vac2Frst " , DevOptn.bUseSTG_Vac2Frst );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSTG_Vac3Frst " , DevOptn.bUseSTG_Vac3Frst );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dBfUVOfsX00      " , DevOptn.dBfUVOfsX00      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dBfUVOfsX10      " , DevOptn.dBfUVOfsX10      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dBfUVOfsX01      " , DevOptn.dBfUVOfsX01      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dBfUVOfsX11      " , DevOptn.dBfUVOfsX11      );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dBfUVOfsY00      " , DevOptn.dBfUVOfsY00      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dBfUVOfsY10      " , DevOptn.dBfUVOfsY10      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dBfUVOfsY01      " , DevOptn.dBfUVOfsY01      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dBfUVOfsY11      " , DevOptn.dBfUVOfsY11      );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dUVTestSpeed     " , DevOptn.dUVTestSpeed     );

    UserINI.Save(sPath.c_str() , "DevOptn" , "dMBStdHght       " , DevOptn.dMBStdHght       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dMBHghtTol       " , DevOptn.dMBHghtTol       );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dHexaTopStdHght  " , DevOptn.dHexaTopStdHght  );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dHexaTopHghtTol  " , DevOptn.dHexaTopHghtTol  );
    UserINI.Save(sPath.c_str() , "DevOptn" , "dCmsHeightOfs    " , DevOptn.dCmsHeightOfs    );
    //UserINI.Save(sPath.c_str() , "DevOptn" , "iDispSeq         " , DevOptn.iDispSeq         );

    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseFstDisp    " , DevOptn.bUseFstDisp      );
    UserINI.Save(sPath.c_str() , "DevOptn" , "bUseSecDisp      " , DevOptn.bUseSecDisp      );

    UserINI.Save(sPath.c_str() , "DevOptn" , "iFstDisp         " , DevOptn.iFstDisp         );
    UserINI.Save(sPath.c_str() , "DevOptn" , "iSecDisp         " , DevOptn.iSecDisp         );

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
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bFLoadingStop    " , CmnOptn.bFLoadingStop   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bRLoadingStop    " , CmnOptn.bRLoadingStop   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor        " , CmnOptn.bIgnrDoor      );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iDisprClnTime    " , CmnOptn.iDisprClnTime  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sLotFilePath     " , CmnOptn.sLotFilePath   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxPlaceZOfs1   " , CmnOptn.dMaxPlaceZOfs1 );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxPlaceZOfs2   " , CmnOptn.dMaxPlaceZOfs2 );

    //UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dMaxAttachForce  " , CmnOptn.dMaxAttachForce);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iWorkMode        " , iTemp                  );     CmnOptn.iWorkMode = iTemp;

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRightVisnDelay  " , CmnOptn.iRightVisnDelay  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRightHeightDelay" , CmnOptn.iRightHeightDelay);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRightHeightStepDelay" , CmnOptn.iRightHeightStepDelay);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iHexaPotRelDelay " , CmnOptn.iHexaPotRelDelay );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseConfocal     "  , CmnOptn.bUseConfocal      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrCmsREndAlign"  , CmnOptn.bIgnrCmsREndAlign );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrCmsREndHeight" , CmnOptn.bIgnrCmsREndHeight);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrCmsRFixAlign"  , CmnOptn.bIgnrCmsRFixAlign );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrCmsRFixHeight" , CmnOptn.bIgnrCmsRFixHeight);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrMidREndHeight" , CmnOptn.bIgnrMidREndHeight);

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iTopUVLimitTime"    , CmnOptn.iTopUVLimitTime   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iBtmUVLimitTime"    , CmnOptn.iBtmUVLimitTime   );


}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bFLoadingStop    " , CmnOptn.bFLoadingStop    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bRLoadingStop    " , CmnOptn.bRLoadingStop    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor        " , CmnOptn.bIgnrDoor        );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iDisprClnTime    " , CmnOptn.iDisprClnTime    );
    UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "sLotFilePath     " , CmnOptn.sLotFilePath     );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxPlaceZOfs1   " , CmnOptn.dMaxPlaceZOfs1   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dMaxPlaceZOfs2   " , CmnOptn.dMaxPlaceZOfs2   );
    //UserINI.Save(sPath.c_str() ,  "CmnOptn"  , "dMaxAttachForce  " , CmnOptn.dMaxAttachForce  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iWorkMode        " , CmnOptn.iWorkMode        );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRightVisnDelay  " , CmnOptn.iRightVisnDelay  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRightHeightDelay" , CmnOptn.iRightHeightDelay);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRightHeightStepDelay" , CmnOptn.iRightHeightStepDelay);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iHexaPotRelDelay " , CmnOptn.iHexaPotRelDelay );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseConfocal      " , CmnOptn.bUseConfocal      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrCmsREndAlign " , CmnOptn.bIgnrCmsREndAlign );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrCmsREndHeight" , CmnOptn.bIgnrCmsREndHeight);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrCmsRFixAlign " , CmnOptn.bIgnrCmsRFixAlign );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrCmsRFixHeight" , CmnOptn.bIgnrCmsRFixHeight);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrMidREndHeight" , CmnOptn.bIgnrMidREndHeight);

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iTopUVLimitTime"    , CmnOptn.iTopUVLimitTime   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iBtmUVLimitTime"    , CmnOptn.iBtmUVLimitTime   );
}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
     UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode          " , MstOptn.bDebugMode           );
     UserINI.Load(sPath.c_str()  , "MstOptn"  , "dLTL_RTL_Stroke     " , MstOptn.dLTL_RTL_Stroke      );
     UserINI.Load(sPath.c_str()  , "MstOptn"  , "dLTL_VISN_DISP_Strke" , MstOptn.dLTL_VISN_DISP_Strke );
     UserINI.Load(sPath.c_str()  , "MstOptn"  , "dRTL_TRNS_VISN_Strke" , MstOptn.dRTL_TRNS_VISN_Strke );
     UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDonUseMasterT      " , MstOptn.bDonUseMasterT       );
     UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUseLotInfo         " , MstOptn.bUseLotInfo          );
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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode          " , MstOptn.bDebugMode           );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dLTL_RTL_Stroke     " , MstOptn.dLTL_RTL_Stroke      );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dLTL_VISN_DISP_Strke" , MstOptn.dLTL_VISN_DISP_Strke );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dRTL_TRNS_VISN_Strke" , MstOptn.dRTL_TRNS_VISN_Strke );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDonUseMasterT      " , MstOptn.bDonUseMasterT       );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUseLotInfo         " , MstOptn.bUseLotInfo          );
}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRM_930B";

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



