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
    LoadJobFile (m_sCrntDev);
}

//---------------------------------------------------------------------------
COptionMan::~COptionMan(void)
{
    SaveLastInfo();
}

//---------------------------------------------------------------------------
void COptionMan::Init(void)
{
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
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColPitch    " , DevInfo.dColPitch    ); if(!DevInfo.dColPitch )DevInfo.dColPitch  = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowPitch    " , DevInfo.dRowPitch    ); if(!DevInfo.dRowPitch )DevInfo.dRowPitch  = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColCnt      " , DevInfo.iColCnt      ); if(!DevInfo.iColCnt   )DevInfo.iColCnt    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowCnt      " , DevInfo.iRowCnt      ); if(!DevInfo.iRowCnt   )DevInfo.iRowCnt    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCsSlPitch   " , DevInfo.dCsSlPitch   ); if(!DevInfo.dCsSlPitch)DevInfo.dCsSlPitch = 1 ;

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
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColPitch    " , DevInfo.dColPitch    ); if(!DevInfo.dColPitch )DevInfo.dColPitch  = 1 ;
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowPitch    " , DevInfo.dRowPitch    ); if(!DevInfo.dRowPitch )DevInfo.dRowPitch  = 1 ;
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColCnt      " , DevInfo.iColCnt      ); if(!DevInfo.iColCnt   )DevInfo.iColCnt    = 1 ;
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowCnt      " , DevInfo.iRowCnt      ); if(!DevInfo.iRowCnt   )DevInfo.iRowCnt    = 1 ;
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCsSlPitch   " , DevInfo.dCsSlPitch   ); if(!DevInfo.dCsSlPitch)DevInfo.dCsSlPitch = 1 ;


}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI UserINI;
    String   sPath  ;
    String   sTemp  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    for(int i=0;i<MAX_OK_GRADE;i++) {
        sTemp = "dMinBn" + (String)i ;
        UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.dMinBn [i] );
        sTemp = "dMaxBn" + (String)i ;
        UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.dMaxBn [i] );

        sTemp = "iBnLSel" + (String)i ;
        UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iBnLSel[i] );
    }

    for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
        sTemp = "dMasterOffset" + AnsiString(i+1) ;
        UserINI.Load(sPath.c_str()  , "DevOptn"  , sTemp.c_str() , DevOptn.dMasterOffset[i]);
    }


}

void COptionMan::SaveDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    String     sPath  ;
    String     sTemp  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    //Load Device.
    for(int i=0;i<MAX_OK_GRADE;i++) {
        sTemp = "dMinBn" + (String)i ;
        UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.dMinBn [i] );
        sTemp = "dMaxBn" + (String)i ;
        UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.dMaxBn [i] );

        sTemp = "iBnLSel" + (String)i ;
        UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp , DevOptn.iBnLSel[i] );

    }

    for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
        sTemp = "dMasterOffset" + AnsiString(i+1) ;
        UserINI.Save(sPath.c_str()  , "DevOptn"  , sTemp.c_str() , DevOptn.dMasterOffset[i]);
    }

}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVacDelay     " , CmnOptn.iVacDelay     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPlaceDelay   " , CmnOptn.iPlaceDelay   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPlaceEjtTime " , CmnOptn.iPlaceEjtTime );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRezeroTrayCnt" , CmnOptn.iRezeroTrayCnt);

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop  " , CmnOptn.bLoadingStop  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bEmptyIgnr    " , CmnOptn.bEmptyIgnr    ); 
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bChuckFailIgnore   " , CmnOptn.bChuckFailIgnore   ); 
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseBinFullIgnore  " , CmnOptn.bUseBinFullIgnore  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bUseFailBinIgnore  " , CmnOptn.bUseFailBinIgnore  ); 

    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor0" ,CmnOptn.iRsltColor0 );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor1" ,CmnOptn.iRsltColor1 );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor2" ,CmnOptn.iRsltColor2 );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor3" ,CmnOptn.iRsltColor3 );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor4" ,CmnOptn.iRsltColor4 );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor5" ,CmnOptn.iRsltColor5 );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor6" ,CmnOptn.iRsltColor6 );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor7" ,CmnOptn.iRsltColor7 );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor8" ,CmnOptn.iRsltColor8 );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor9" ,CmnOptn.iRsltColor9 );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorA" ,CmnOptn.iRsltColorA );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorB" ,CmnOptn.iRsltColorB );
    UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorC" ,CmnOptn.iRsltColorC );
    UserINI.Load(sPath.c_str(),"CmnOptn","bNotUseTool1",CmnOptn.bNotUseTool1);
    UserINI.Load(sPath.c_str(),"CmnOptn","bNotUseTool2",CmnOptn.bNotUseTool2);
    UserINI.Load(sPath.c_str(),"CmnOptn","bNotUseTool3",CmnOptn.bNotUseTool3);

}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath.c_str(),"CmnOptn","iVacDelay     " , CmnOptn.iVacDelay     );
    UserINI.Save(sPath.c_str(),"CmnOptn","iPlaceDelay   " , CmnOptn.iPlaceDelay   );
    UserINI.Save(sPath.c_str(),"CmnOptn","iPlaceEjtTime " , CmnOptn.iPlaceEjtTime );
    UserINI.Save(sPath.c_str(),"CmnOptn","iRezeroTrayCnt" , CmnOptn.iRezeroTrayCnt);

    UserINI.Save(sPath.c_str(),"CmnOptn","bLoadingStop  " , CmnOptn.bLoadingStop  );
    UserINI.Save(sPath.c_str(),"CmnOptn","bEmptyIgnr    " , CmnOptn.bEmptyIgnr    );
    UserINI.Save(sPath.c_str(),"CmnOptn","bChuckFailIgnore   " , CmnOptn.bChuckFailIgnore   );
    UserINI.Save(sPath.c_str(),"CmnOptn","bUseBinFullIgnore  " , CmnOptn.bUseBinFullIgnore  );
    UserINI.Save(sPath.c_str(),"CmnOptn","bUseFailBinIgnore  " , CmnOptn.bUseFailBinIgnore  );

    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor0" ,CmnOptn.iRsltColor0);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor1" ,CmnOptn.iRsltColor1);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor2" ,CmnOptn.iRsltColor2);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor3" ,CmnOptn.iRsltColor3);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor4" ,CmnOptn.iRsltColor4);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor5" ,CmnOptn.iRsltColor5);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor6" ,CmnOptn.iRsltColor6);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor7" ,CmnOptn.iRsltColor7);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor8" ,CmnOptn.iRsltColor8);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor9" ,CmnOptn.iRsltColor9);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorA" ,CmnOptn.iRsltColorA);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorB" ,CmnOptn.iRsltColorB);
    UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorC" ,CmnOptn.iRsltColorC);

    UserINI.Save(sPath.c_str(),"CmnOptn","bNotUseTool1",CmnOptn.bNotUseTool1);
    UserINI.Save(sPath.c_str(),"CmnOptn","bNotUseTool2",CmnOptn.bNotUseTool2);
    UserINI.Save(sPath.c_str(),"CmnOptn","bNotUseTool3",CmnOptn.bNotUseTool3);

}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode     " , MstOptn.bDebugMode     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bRandomSort    " , MstOptn.bRandomSort    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bRandomSupply  " , MstOptn.bRandomSupply  );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bCheckCycle    " , MstOptn.bCheckCycle    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "iMgzShowTime   " , MstOptn.iMgzShowTime   );
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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode     " , MstOptn.bDebugMode     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bRandomSort    " , MstOptn.bRandomSort    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bRandomSupply  " , MstOptn.bRandomSupply  );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bCheckCycle    " , MstOptn.bCheckCycle    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "iMgzShowTime   " , MstOptn.iMgzShowTime   );
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
    UserINI.Load(sPath, "EqpOptn"  , "sModelName    " , EqpOptn.sModelName     );

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
    UserINI.Save(sPath, "EqpOptn"  , "sModelName    " ,  EqpOptn.sModelName     );
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

