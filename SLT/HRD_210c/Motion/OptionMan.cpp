//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "UserIni.h"
#include "SLogUnit.h"
#include "SAInterface.h"
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
    memset(&DevInfo  , 0 , sizeof(DevInfo ));
    memset(&DevOptn  , 0 , sizeof(DevOptn ));
    memset(&CmnOptn  , 0 , sizeof(CmnOptn ));
    memset(&MstOptn  , 0 , sizeof(MstOptn ));
    memset(&EqpOptn  , 0 , sizeof(EqpOptn ));

    LoadLastInfo(          );
    LoadMstOptn (          );
    LoadEqpOptn (          );
    LoadCmnOptn (          );
    LoadDevInfo (m_sCrntDev);
    LoadDevOptn (m_sCrntDev);
    //LoadJobFile (m_sCrntDev); //아직 비전이 없음.
}

void COptionMan::SetCrntDev (AnsiString _sName)
{
    m_sCrntDev = _sName ;
}

void COptionMan::LoadJobFile(AnsiString _sDevName)
{
    LoadDevInfo (_sDevName);
    LoadDevOptn (_sDevName);
    SV_LoadJobFile(true , _sDevName.c_str());
    //Set Current Device Name.
    m_sCrntDev = _sDevName;
}


void COptionMan::SaveJobFile(AnsiString _sDevName)
{
    SaveDevInfo (_sDevName);
    SaveDevOptn (_sDevName);
    SV_LoadJobFile(false , _sDevName.c_str());
    //Set Current Device Name.
    m_sCrntDev = _sDevName;
}

void COptionMan::LoadDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColPitch     " , DevInfo.dColPitch    ); if(!DevInfo.dColPitch )DevInfo.dColPitch  = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowPitch     " , DevInfo.dRowPitch    ); if(!DevInfo.dRowPitch )DevInfo.dRowPitch  = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColGrCnt     " , DevInfo.iColGrCnt    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowGrCnt     " , DevInfo.iRowGrCnt    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColGrGap     " , DevInfo.dColGrGap    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowGrGap     " , DevInfo.dRowGrGap    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColCnt       " , DevInfo.iColCnt      ); if(!DevInfo.iColCnt   )DevInfo.iColCnt    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowCnt       " , DevInfo.iRowCnt      ); if(!DevInfo.iRowCnt   )DevInfo.iRowCnt    = 1 ;

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iMgzSlotCnt   " , DevInfo.iMgzSlotCnt  );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dMgzSlotPitch " , DevInfo.dMgzSlotPitch);

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iCsSlotCnt    " , DevInfo.iCsSlotCnt   );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCsSlotPitch  " , DevInfo.dCsSlotPitch );

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dWf_PitchX    " , DevInfo.dWf_PitchX   );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dWf_PitchY    " , DevInfo.dWf_PitchY   );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dWf_BldOfs    " , DevInfo.dWf_BldOfs   );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dWf_Diameter  " , DevInfo.dWf_Diameter );

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iMapType      " , DevInfo.iMapType      );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iWColCnt      " , DevInfo.iWColCnt      );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iWRowCnt      " , DevInfo.iWRowCnt      );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iFlatAngle    " , DevInfo.iFlatAngle    );

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iWXOffset     " , DevInfo.iWXOffset     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iWYOffset     " , DevInfo.iWYOffset     );

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "sGoodStr1     " , DevInfo.sGoodStr1     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "sEmptStr1     " , DevInfo.sEmptStr1     );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iStrSttRow    " , DevInfo.iStrSttRow    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iStrSttCol    " , DevInfo.iStrSttCol    );




}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColPitch     " , DevInfo.dColPitch      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowPitch     " , DevInfo.dRowPitch      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColGrCnt     " , DevInfo.iColGrCnt      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowGrCnt     " , DevInfo.iRowGrCnt      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColGrGap     " , DevInfo.dColGrGap      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowGrGap     " , DevInfo.dRowGrGap      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColCnt       " , DevInfo.iColCnt        );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowCnt       " , DevInfo.iRowCnt        );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iMgzSlotCnt   " , DevInfo.iMgzSlotCnt    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dMgzSlotPitch " , DevInfo.dMgzSlotPitch  );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iCsSlotCnt    " , DevInfo.iCsSlotCnt     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCsSlotPitch  " , DevInfo.dCsSlotPitch   );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dWf_PitchX    " , DevInfo.dWf_PitchX     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dWf_PitchY    " , DevInfo.dWf_PitchY     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dWf_BldOfs    " , DevInfo.dWf_BldOfs     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dWf_Diameter  " , DevInfo.dWf_Diameter   );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iMapType      " , DevInfo.iMapType       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iWColCnt      " , DevInfo.iWColCnt       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iWRowCnt      " , DevInfo.iWRowCnt       );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iWFrstRow     " , DevInfo.iWFrstRow      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iWFrstCol     " , DevInfo.iWFrstCol      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iFlatAngle    " , DevInfo.iFlatAngle     );


    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iWXOffset     " , DevInfo.iWXOffset      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iWYOffset     " , DevInfo.iWYOffset      );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "sGoodStr1     " , DevInfo.sGoodStr1      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "sEmptStr1     " , DevInfo.sEmptStr1      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iStrSttRow    " , DevInfo.iStrSttRow     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iStrSttCol    " , DevInfo.iStrSttCol     );

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
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt  " , DevOptn.iLotEndMgzCnt   ); if(!DevOptn.iLotEndMgzCnt) DevOptn.iLotEndMgzCnt = 1;
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEpxEjtOnTime  " , DevOptn.iEpxEjtOnTime   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEpxEjtOffTime " , DevOptn.iEpxEjtOffTime  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dWRowPitch     " , DevOptn.dWRowPitch      );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dWColPitch     " , DevOptn.dWColPitch      );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dVoiceCoilStrip" , DevOptn.dVoiceCoilStrip );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dVoiceCoilStrip1", DevOptn.dVoiceCoilStrip1);
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dVoiceCoilMove " , DevOptn.dVoiceCoilMove  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dVoiceCoilWafer" , DevOptn.dVoiceCoilWafer );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dVoiceCoilWafer1", DevOptn.dVoiceCoilWafer1);

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEpxClenCnt     " , DevOptn.iEpxClenCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEpxCleanDly    " , DevOptn.iEpxCleanDly    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEpxCleanShotCnt" , DevOptn.iEpxCleanShotCnt);

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEpxTime       " , DevOptn.iEpxTime        );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iVacuumOnDly   " , DevOptn.iVacuumOnDly    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iVacuumOffDly  " , DevOptn.iVacuumOffDly   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEjtSuckOnDly  " , DevOptn.iEjtSuckOnDly   );

    for(int i =0 ; i < MAX_REGRIP_ARRAY ; i++ ) {
        sTemp = "iPRBRgCol[" + (String)i +"]"; UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iPRBRgCol[i] );
        sTemp = "iPRBRgDis[" + (String)i +"]"; UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iPRBRgDis[i] );
        sTemp = "iWRERgCol[" + (String)i +"]"; UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iWRERgCol[i] );
        sTemp = "iWRERgDis[" + (String)i +"]"; UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iWRERgDis[i] );
        sTemp = "iWRDRgCol[" + (String)i +"]"; UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iWRDRgCol[i] );
        sTemp = "iWRDRgDis[" + (String)i +"]"; UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iWRDRgDis[i] );
        sTemp = "iPSBRgCol[" + (String)i +"]"; UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iPSBRgCol[i] );
        sTemp = "iPSBRgDis[" + (String)i +"]"; UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iPSBRgDis[i] );
    }

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPRB_XRgCnt    " , DevOptn.iPRB_XRgCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWRE_XRgCnt    " , DevOptn.iWRE_XRgCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWRD_XRgCnt    " , DevOptn.iWRD_XRgCnt     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPSB_XRgCnt    " , DevOptn.iPSB_XRgCnt     );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPRB_XWkCon    " , DevOptn.iPRB_XWkCon     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWRE_XWkCon    " , DevOptn.iWRE_XWkCon     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWRD_XWkCon    " , DevOptn.iWRD_XWkCon     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPSB_XWkCon    " , DevOptn.iPSB_XWkCon     );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLdrOrder      " , DevOptn.iLdrOrder       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iUdrOrder      " , DevOptn.iUdrOrder       );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dDFMDiameter   " , DevOptn.dDFMDiameter    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dDFMRadius     " , DevOptn.dDFMRadius      );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dDFMWidth      " , DevOptn.dDFMWidth       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dDFMHeight     " , DevOptn.dDFMHeight      );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dDFMSttOfsX    " , DevOptn.dDFMSttOfsX     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dDFMSttOfsY    " , DevOptn.dDFMSttOfsY     );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPickWaitAc    " , DevOptn.dPickWaitAc   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPickWaitDc    " , DevOptn.dPickWaitDc   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPickWaitVel   " , DevOptn.dPickWaitVel  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPlaceWaitAc   " , DevOptn.dPlaceWaitAc   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPlaceWaitDc   " , DevOptn.dPlaceWaitDc   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "dPlaceWaitVel  " , DevOptn.dPlaceWaitVel  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEjectOnDly    " , DevOptn.iEjectOnDly  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iEjectorBfDnDly" , DevOptn.iEjectorBfDnDly); 

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriVsFailCnt  " , DevOptn.iOriVsFailCnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriVsBfDly    " , DevOptn.iOriVsBfDly     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriVsGrabTO   " , DevOptn.iOriVsGrabTO    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriVsInspTO   " , DevOptn.iOriVsInspTO    );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWfrVsFailCnt  " , DevOptn.iWfrVsFailCnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWfrVsBfDly    " , DevOptn.iWfrVsBfDly     );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWfrVsGrabTO   " , DevOptn.iWfrVsGrabTO    );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWfrVsInspTO   " , DevOptn.iWfrVsInspTO    );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRalBfVsFailCnt" , DevOptn.iRalBfVsFailCnt );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRalBfVsBfDly  " , DevOptn.iRalBfVsBfDly   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRalBfVsGrabTO " , DevOptn.iRalBfVsGrabTO  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRalBfVsInspTO " , DevOptn.iRalBfVsInspTO  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRalAtVsFailCnt" , DevOptn.iRalAtVsFailCnt );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRalAtVsBfDly  " , DevOptn.iRalAtVsBfDly   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRalAtVsGrabTO " , DevOptn.iRalAtVsGrabTO  );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iRalAtVsInspTO " , DevOptn.iRalAtVsInspTO  );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iOriMathod       " , DevOptn.iOriMathod       );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bOperChckAtMatch " , DevOptn.bOperChckAtMatch );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWfFullRctCntX   " , DevOptn.iWfFullRctCntX   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWfFullRctCntY   " , DevOptn.iWfFullRctCntY   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWfFullRctMapOfsX" , DevOptn.iWfFullRctMapOfsX);
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iWfFullRctMapOfsY" , DevOptn.iWfFullRctMapOfsY);
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iSecondFindPos   " , DevOptn.iSecondFindPos   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLdrCanHomePrbCnt" , DevOptn.iLdrCanHomePrbCnt);
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

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt  " , DevOptn.iLotEndMgzCnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEpxEjtOnTime  " , DevOptn.iEpxEjtOnTime   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEpxEjtOffTime " , DevOptn.iEpxEjtOffTime  );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dWRowPitch     " , DevOptn.dWRowPitch      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dWColPitch     " , DevOptn.dWColPitch      );


    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dVoiceCoilStrip" , DevOptn.dVoiceCoilStrip );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dVoiceCoilStrip1", DevOptn.dVoiceCoilStrip1);
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dVoiceCoilMove " , DevOptn.dVoiceCoilMove  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dVoiceCoilWafer" , DevOptn.dVoiceCoilWafer );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dVoiceCoilWafer1", DevOptn.dVoiceCoilWafer1);

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEpxClenCnt    " , DevOptn.iEpxClenCnt     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEpxCleanDly   " , DevOptn.iEpxCleanDly    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEpxCleanShotCnt", DevOptn.iEpxCleanShotCnt);

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEpxTime       " , DevOptn.iEpxTime        );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iVacuumOnDly   " , DevOptn.iVacuumOnDly    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iVacuumOffDly  " , DevOptn.iVacuumOffDly   );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEjtSuckOnDly  " , DevOptn.iEjtSuckOnDly   );

    for(int i =0 ; i < MAX_REGRIP_ARRAY ; i++ ) {
        sTemp = "iPRBRgCol[" + (String)i +"]"; UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iPRBRgCol[i] );
        sTemp = "iPRBRgDis[" + (String)i +"]"; UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iPRBRgDis[i] );
        sTemp = "iWRERgCol[" + (String)i +"]"; UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iWRERgCol[i] );
        sTemp = "iWRERgDis[" + (String)i +"]"; UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iWRERgDis[i] );
        sTemp = "iWRDRgCol[" + (String)i +"]"; UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iWRDRgCol[i] );
        sTemp = "iWRDRgDis[" + (String)i +"]"; UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iWRDRgDis[i] );
        sTemp = "iPSBRgCol[" + (String)i +"]"; UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iPSBRgCol[i] );
        sTemp = "iPSBRgDis[" + (String)i +"]"; UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iPSBRgDis[i] );
    }

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPRB_XRgCnt    " , DevOptn.iPRB_XRgCnt     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWRE_XRgCnt    " , DevOptn.iWRE_XRgCnt     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWRD_XRgCnt    " , DevOptn.iWRD_XRgCnt     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPSB_XRgCnt    " , DevOptn.iPSB_XRgCnt     );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPRB_XWkCon    " , DevOptn.iPRB_XWkCon     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWRE_XWkCon    " , DevOptn.iWRE_XWkCon     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWRD_XWkCon    " , DevOptn.iWRD_XWkCon     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPSB_XWkCon    " , DevOptn.iPSB_XWkCon     );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLdrOrder      " , DevOptn.iLdrOrder       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iUdrOrder      " , DevOptn.iUdrOrder       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dDFMDiameter   " , DevOptn.dDFMDiameter    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dDFMRadius     " , DevOptn.dDFMRadius      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dDFMWidth      " , DevOptn.dDFMWidth       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dDFMHeight     " , DevOptn.dDFMHeight      );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dDFMSttOfsX    " , DevOptn.dDFMSttOfsX     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dDFMSttOfsY    " , DevOptn.dDFMSttOfsY     );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPickWaitAc    " , DevOptn.dPickWaitAc   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPickWaitDc    " , DevOptn.dPickWaitDc   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPickWaitVel   " , DevOptn.dPickWaitVel  );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPlaceWaitAc   " , DevOptn.dPlaceWaitAc  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPlaceWaitDc   " , DevOptn.dPlaceWaitDc  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "dPlaceWaitVel  " , DevOptn.dPlaceWaitVel );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEjectOnDly    " , DevOptn.iEjectOnDly  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iEjectorBfDnDly" , DevOptn.iEjectorBfDnDly); 

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriVsFailCnt  " , DevOptn.iOriVsFailCnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriVsBfDly    " , DevOptn.iOriVsBfDly     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriVsGrabTO   " , DevOptn.iOriVsGrabTO    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriVsInspTO   " , DevOptn.iOriVsInspTO    );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWfrVsFailCnt  " , DevOptn.iWfrVsFailCnt   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWfrVsBfDly    " , DevOptn.iWfrVsBfDly     );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWfrVsGrabTO   " , DevOptn.iWfrVsGrabTO    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWfrVsInspTO   " , DevOptn.iWfrVsInspTO    );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRalBfVsFailCnt" , DevOptn.iRalBfVsFailCnt );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRalBfVsBfDly  " , DevOptn.iRalBfVsBfDly   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRalBfVsGrabTO " , DevOptn.iRalBfVsGrabTO  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRalBfVsInspTO " , DevOptn.iRalBfVsInspTO  );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRalAtVsFailCnt" , DevOptn.iRalAtVsFailCnt );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRalAtVsBfDly  " , DevOptn.iRalAtVsBfDly   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRalAtVsGrabTO " , DevOptn.iRalAtVsGrabTO  );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iRalAtVsInspTO " , DevOptn.iRalAtVsInspTO  );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iOriMathod       " , DevOptn.iOriMathod       );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bOperChckAtMatch " , DevOptn.bOperChckAtMatch );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWfFullRctCntX   " , DevOptn.iWfFullRctCntX   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWfFullRctCntY   " , DevOptn.iWfFullRctCntY   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWfFullRctMapOfsX" , DevOptn.iWfFullRctMapOfsX);
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iWfFullRctMapOfsY" , DevOptn.iWfFullRctMapOfsY);
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iSecondFindPos   " , DevOptn.iSecondFindPos   );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLdrCanHomePrbCnt" , DevOptn.iLdrCanHomePrbCnt);

    SaveLastInfo();
}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDoorSkip              " , CmnOptn.bDoorSkip              );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop           " , CmnOptn.bLoadingStop           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bEpoxyStop             " , CmnOptn.bEpoxyStop             ); 

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrOriVs             " , CmnOptn.bIgnrOriVs             );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrRalAtVs           " , CmnOptn.bIgnrRalAtVs           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrVcSsr             " , CmnOptn.bIgnrVcSsr             );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseStgTheta           " , CmnOptn.bUseStgTheta           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrRalBfVsEpoxyFail  " , CmnOptn.bIgnrRalBfVsEpoxyFail  );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sSourcePath            " , CmnOptn.sSourcePath            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sWorkedPath            " , CmnOptn.sWorkedPath            );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseBtAlign            " , CmnOptn.bUseBtAlign            );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrHedOverload       " , CmnOptn.bIgnrHedOverload       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseHeadTWfr           " , CmnOptn.bUseHeadTWfr           );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseHeadTRal           " , CmnOptn.bUseHeadTRal           );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dStageTLmtErr          " , CmnOptn.dStageTLmtErr          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dHedTLmtErr            " , CmnOptn.dHedTLmtErr            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iEjctrDnAfterDelay     " , CmnOptn.iEjctrDnAfterDelay     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iEjctrUpAfterDelay     " , CmnOptn.iEjctrUpAfterDelay     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bWfrVisnRetest         " , CmnOptn.bWfrVisnRetest         );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseMapFile            " , CmnOptn.bUseMapFile            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bInkDieWork            " , CmnOptn.bInkDieWork            );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bStageStop             " , CmnOptn.bStageStop             );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iLotEndChipCnt         " , CmnOptn.iLotEndChipCnt         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseVcCoil             " , CmnOptn.bUseVcCoil             );
}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDoorSkip              " , CmnOptn.bDoorSkip              );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadingStop           " , CmnOptn.bLoadingStop           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bEpoxyStop             " , CmnOptn.bEpoxyStop             ); 

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrOriVs             " , CmnOptn.bIgnrOriVs             );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrRalAtVs           " , CmnOptn.bIgnrRalAtVs           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrVcSsr             " , CmnOptn.bIgnrVcSsr             );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseStgTheta           " , CmnOptn.bUseStgTheta           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrRalBfVsEpoxyFail  " , CmnOptn.bIgnrRalBfVsEpoxyFail  );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sSourcePath            " , CmnOptn.sSourcePath            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sWorkedPath            " , CmnOptn.sWorkedPath            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseBtAlign            " , CmnOptn.bUseBtAlign            );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrHedOverload       " , CmnOptn.bIgnrHedOverload       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseHeadTWfr           " , CmnOptn.bUseHeadTWfr           );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseHeadTRal           " , CmnOptn.bUseHeadTRal           );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dStageTLmtErr          " , CmnOptn.dStageTLmtErr          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dHedTLmtErr            " , CmnOptn.dHedTLmtErr            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iEjctrDnAfterDelay     " , CmnOptn.iEjctrDnAfterDelay     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iEjctrUpAfterDelay     " , CmnOptn.iEjctrUpAfterDelay     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bWfrVisnRetest         " , CmnOptn.bWfrVisnRetest         );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseMapFile            " , CmnOptn.bUseMapFile            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bInkDieWork            " , CmnOptn.bInkDieWork            );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bStageStop             " , CmnOptn.bStageStop             );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iLotEndChipCnt         " , CmnOptn.iLotEndChipCnt         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bUseVcCoil             " , CmnOptn.bUseVcCoil             );

}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode        " , MstOptn.bDebugMode        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bIgnrEpoxy        " , MstOptn.bIgnrEpoxy        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dEpxOriSpd        " , MstOptn.dEpxOriSpd        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dEpxOriOfs        " , MstOptn.dEpxOriOfs        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bIgnrDieAttach    " , MstOptn.bIgnrDieAttach    );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dVoiceCoilOffset  " , MstOptn.dVoiceCoilOffset  );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dVoiceCoilA       " , MstOptn.dVoiceCoilA       );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dVoiceCoilB       " , MstOptn.dVoiceCoilB       );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d1stDis           " , MstOptn.d1stDis           );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d2stDis           " , MstOptn.d2stDis           );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "d3stDis           " , MstOptn.d3stDis           );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dStgXEjtCt        " , MstOptn.dStgXEjtCt        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dStgYEjtCt        " , MstOptn.dStgYEjtCt        );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bIgnrBfVision     " , MstOptn.bIgnrBfVision     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bIgnrWfVision     " , MstOptn.bIgnrWfVision     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dStgDia           " , MstOptn.dStgDia           );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dEjtDia           " , MstOptn.dEjtDia           );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dStgYMaxExdAble   " , MstOptn.dStgYMaxExdAble   );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sDeviceBackUpPath " , MstOptn.sDeviceBackUpPath );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dToolCrashWfrY    " , MstOptn.dToolCrashWfrY    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dToolCrashRalY    " , MstOptn.dToolCrashRalY    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dToolCrashZ       " , MstOptn.dToolCrashZ       );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bPickUpTestMode   " , MstOptn.bPickUpTestMode   );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bEjectorDnFirst   " , MstOptn.bEjectorDnFirst   );

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDryRun           " , MstOptn.bDryRun           ); 
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dRegripOfs        " , MstOptn.dRegripOfs        ); 


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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode        " , MstOptn.bDebugMode        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bIgnrEpoxy        " , MstOptn.bIgnrEpoxy        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dEpxOriSpd        " , MstOptn.dEpxOriSpd        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dEpxOriOfs        " , MstOptn.dEpxOriOfs        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bIgnrDieAttach    " , MstOptn.bIgnrDieAttach    );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dVoiceCoilOffset  " , MstOptn.dVoiceCoilOffset  );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dVoiceCoilA       " , MstOptn.dVoiceCoilA       );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dVoiceCoilB       " , MstOptn.dVoiceCoilB       );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d1stDis           " , MstOptn.d1stDis           );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d2stDis           " , MstOptn.d2stDis           );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "d3stDis           " , MstOptn.d3stDis           );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dStgXEjtCt        " , MstOptn.dStgXEjtCt        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dStgYEjtCt        " , MstOptn.dStgYEjtCt        );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bIgnrBfVision     " , MstOptn.bIgnrBfVision     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bIgnrWfVision     " , MstOptn.bIgnrWfVision     );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dStgDia           " , MstOptn.dStgDia           );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dEjtDia           " , MstOptn.dEjtDia           );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dStgYMaxExdAble   " , MstOptn.dStgYMaxExdAble   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sDeviceBackUpPath " , MstOptn.sDeviceBackUpPath );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dToolCrashWfrY    " , MstOptn.dToolCrashWfrY    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dToolCrashRalY    " , MstOptn.dToolCrashRalY    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dToolCrashZ       " , MstOptn.dToolCrashZ       );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bPickUpTestMode   " , MstOptn.bPickUpTestMode   );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bEjectorDnFirst   " , MstOptn.bEjectorDnFirst   ); 

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDryRun           " , MstOptn.bDryRun           );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dRegripOfs        " , MstOptn.dRegripOfs        );


}
void COptionMan::LoadEqpOptn()
{

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sModelName = "HRT-430S";

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\EpqOptn.INI" ;

    //Load.
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName    );
    UserINI.Load(sPath, "EqpOptn"  , "iTotalChip    " , EqpOptn.iTotalChip    );
    UserINI.Load(sPath, "EqpOptn"  , "iTotalFailChip" , EqpOptn.iTotalFailChip);

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
    UserINI.Save(sPath, "EqpOptn"  , "iTotalChip    " ,  EqpOptn.iTotalChip    );
    UserINI.Save(sPath, "EqpOptn"  , "iTotalFailChip" ,  EqpOptn.iTotalFailChip);
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

