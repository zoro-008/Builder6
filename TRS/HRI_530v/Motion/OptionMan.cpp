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
    memset(&DevOptn  , 0 , sizeof(DevOptn ));  //sOSTPinName �� String �̶� ��� �ϸ� �޸� ���� ������ �ϴ� �ѹ��� �ϱ⿡ ���ܵ�.
    memset(&CmnOptn  , 0 , sizeof(CmnOptn ));  //sRj1Name  �� String �̶� ��� �ϸ� �޸� ���� ������ �ϴ� �ѹ��� �ϱ⿡ ���ܵ�.
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
    UserINI.Load(sPath.c_str() , "DevInfo" , "dLDRColPitch " , DevInfo.dLDRColPitch  ); if(!DevInfo.dLDRColPitch   )DevInfo.dLDRColPitch    = 1.0 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iLDRColCnt   " , DevInfo.iLDRColCnt    ); if(!DevInfo.iLDRColCnt     )DevInfo.iLDRColCnt      = 1   ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dLDRRowPitch " , DevInfo.dLDRRowPitch  ); if(!DevInfo.dLDRRowPitch   )DevInfo.dLDRRowPitch    = 1.0 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iLDRRowCnt   " , DevInfo.iLDRRowCnt    ); if(!DevInfo.iLDRRowCnt     )DevInfo.iLDRRowCnt      = 1   ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dScktPitch   " , DevInfo.dScktPitch    ); if(!DevInfo.dScktPitch     )DevInfo.dScktPitch      = 1.0 ;

    UserINI.Load(sPath.c_str() , "DevInfo" , "dULDColPitch " , DevInfo.dULDColPitch  ); if(!DevInfo.dULDColPitch   )DevInfo.dULDColPitch    = 1.0 ;   // YOU
    UserINI.Load(sPath.c_str() , "DevInfo" , "iULDColCnt   " , DevInfo.iULDColCnt    ); if(!DevInfo.iULDColCnt     )DevInfo.iULDColCnt      = 1   ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "dULDRowPitch " , DevInfo.dULDRowPitch  ); if(!DevInfo.dULDRowPitch   )DevInfo.dULDRowPitch    = 1.0 ;
    UserINI.Load(sPath.c_str() , "DevInfo" , "iULDRowCnt   " , DevInfo.iULDRowCnt    ); if(!DevInfo.iULDRowCnt     )DevInfo.iULDRowCnt      = 1   ;

}

void COptionMan::SaveDevInfo(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevInfo.INI";

    //Load Device.
    UserINI.Save(sPath.c_str() , "DevInfo" , "dLDRColPitch " , DevInfo.dLDRColPitch  ); 
    UserINI.Save(sPath.c_str() , "DevInfo" , "iLDRColCnt   " , DevInfo.iLDRColCnt    ); 
    UserINI.Save(sPath.c_str() , "DevInfo" , "dLDRRowPitch " , DevInfo.dLDRRowPitch  ); 
    UserINI.Save(sPath.c_str() , "DevInfo" , "iLDRRowCnt   " , DevInfo.iLDRRowCnt    ); 
    UserINI.Save(sPath.c_str() , "DevInfo" , "dScktPitch   " , DevInfo.dScktPitch    ); 

    UserINI.Save(sPath.c_str() , "DevInfo" , "dULDColPitch " , DevInfo.dULDColPitch  ); 
    UserINI.Save(sPath.c_str() , "DevInfo" , "iULDColCnt   " , DevInfo.iULDColCnt    ); 
    UserINI.Save(sPath.c_str() , "DevInfo" , "dULDRowPitch " , DevInfo.dULDRowPitch  ); 
    UserINI.Save(sPath.c_str() , "DevInfo" , "iULDRowCnt   " , DevInfo.iULDRowCnt    );




    /*
    UserINI.Save(sPath.c_str() , "DevInfo" , "dColPitch " , DevInfo.dColPitch  ); if(!DevInfo.dColPitch   )DevInfo.dColPitch    = 1.0 ;
    UserINI.Save(sPath.c_str() , "DevInfo" , "iColCnt   " , DevInfo.iColCnt    ); if(!DevInfo.iColCnt     )DevInfo.iColCnt      = 1   ;
    UserINI.Save(sPath.c_str() , "DevInfo" , "dRowPitch " , DevInfo.dRowPitch  ); if(!DevInfo.dRowPitch   )DevInfo.dRowPitch    = 1.0 ;
    UserINI.Save(sPath.c_str() , "DevInfo" , "iRowCnt   " , DevInfo.iRowCnt    ); if(!DevInfo.iRowCnt     )DevInfo.iRowCnt      = 1   ;
    UserINI.Save(sPath.c_str() , "DevInfo" , "dScktPitch" , DevInfo.dScktPitch ); if(!DevInfo.dScktPitch  )DevInfo.dScktPitch   = 1.0 ;

    UserINI.Save(sPath.c_str() , "DevInfo" , "dColPitch2 " , DevInfo.dColPitch2  ); if(!DevInfo.dColPitch2   )DevInfo.dColPitch2    = 1.0 ;    // YOU
    UserINI.Save(sPath.c_str() , "DevInfo" , "iColCnt2   " , DevInfo.iColCnt2    ); if(!DevInfo.iColCnt2     )DevInfo.iColCnt2      = 1   ;
    UserINI.Save(sPath.c_str() , "DevInfo" , "dRowPitch2 " , DevInfo.dRowPitch2  ); if(!DevInfo.dRowPitch2   )DevInfo.dRowPitch2    = 1.0 ;
    UserINI.Save(sPath.c_str() , "DevInfo" , "iRowCnt2   " , DevInfo.iRowCnt2    ); if(!DevInfo.iRowCnt2     )DevInfo.iRowCnt2      = 1   ;
    */
}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str() , "DevOptn" , "iPickDelay" , DevOptn.iPickDelay );
    UserINI.Load(sPath.c_str() , "DevOptn" , "iPlceDelay" , DevOptn.iPlceDelay );

    UserINI.Load(sPath.c_str() , "DevInfo" , "iLotTrayCnt", DevOptn.iLotTrayCnt);


    UserINI.Load(sPath.c_str() , "DevInfo" , "iOSTDeviceNo", DevOptn.iOSTDeviceNo);

    UserINI.Load(sPath.c_str() , "DevInfo" , "dIDD_AVDDOkMin", DevOptn.dIDD_AVDDOkMin); UserINI.Load(sPath.c_str() , "DevInfo" , "dIDD_AVDDOkMax", DevOptn.dIDD_AVDDOkMax);
    UserINI.Load(sPath.c_str() , "DevInfo" , "dIDD_CVDDOkMin", DevOptn.dIDD_CVDDOkMin); UserINI.Load(sPath.c_str() , "DevInfo" , "dIDD_CVDDOkMax", DevOptn.dIDD_CVDDOkMax);
    UserINI.Load(sPath.c_str() , "DevInfo" , "dIDD_HVDDOkMin", DevOptn.dIDD_HVDDOkMin); UserINI.Load(sPath.c_str() , "DevInfo" , "dIDD_HVDDOkMax", DevOptn.dIDD_HVDDOkMax);
    UserINI.Load(sPath.c_str() , "DevInfo" , "dIDD_DVDDOkMin", DevOptn.dIDD_DVDDOkMin); UserINI.Load(sPath.c_str() , "DevInfo" , "dIDD_DVDDOkMax", DevOptn.dIDD_DVDDOkMax);

    UserINI.Load(sPath.c_str() , "DevInfo" , "dIDS_AVDDOkMin", DevOptn.dIDS_AVDDOkMin); UserINI.Load(sPath.c_str() , "DevInfo" , "dIDS_AVDDOkMax", DevOptn.dIDS_AVDDOkMax);
    UserINI.Load(sPath.c_str() , "DevInfo" , "dIDS_CVDDOkMin", DevOptn.dIDS_CVDDOkMin); UserINI.Load(sPath.c_str() , "DevInfo" , "dIDS_CVDDOkMax", DevOptn.dIDS_CVDDOkMax);
    UserINI.Load(sPath.c_str() , "DevInfo" , "dIDS_HVDDOkMin", DevOptn.dIDS_HVDDOkMin); UserINI.Load(sPath.c_str() , "DevInfo" , "dIDS_HVDDOkMax", DevOptn.dIDS_HVDDOkMax);
    UserINI.Load(sPath.c_str() , "DevInfo" , "dIDS_DVDDOkMin", DevOptn.dIDS_DVDDOkMin); UserINI.Load(sPath.c_str() , "DevInfo" , "dIDS_DVDDOkMax", DevOptn.dIDS_DVDDOkMax);

    UserINI.Load(sPath.c_str() , "DevInfo" , "iOST_I2cFailTime"  , DevOptn.iOST_I2cFailTime);
    UserINI.Load(sPath.c_str() , "DevInfo" , "bVTPwrAllwaysOn"   , DevOptn.bVTPwrAllwaysOn );

    for(int i = 0 ; i < MAX_OSP_PIN ; i++){
        UserINI.Load(sPath.c_str() , "DevInfo" , "sOSTPinName"+String(i), DevOptn.sOSTPinName[i] );
    }

    for(int p = 0 ; p < MAX_TOOL_SOCK_COL ; p++) {
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dAVDD_IDDGain"+String(p) , DevOptn.dAVDD_IDDGain[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCVDD_IDDGain"+String(p) , DevOptn.dCVDD_IDDGain[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dHVDD_IDDGain"+String(p) , DevOptn.dHVDD_IDDGain[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dDVDD_IDDGain"+String(p) , DevOptn.dDVDD_IDDGain[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dAVDD_IDSGain"+String(p) , DevOptn.dAVDD_IDSGain[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCVDD_IDSGain"+String(p) , DevOptn.dCVDD_IDSGain[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dHVDD_IDSGain"+String(p) , DevOptn.dHVDD_IDSGain[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dDVDD_IDSGain"+String(p) , DevOptn.dDVDD_IDSGain[p]);

        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dAVDD_IDDOfst"+String(p) , DevOptn.dAVDD_IDDOfst[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCVDD_IDDOfst"+String(p) , DevOptn.dCVDD_IDDOfst[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dHVDD_IDDOfst"+String(p) , DevOptn.dHVDD_IDDOfst[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dDVDD_IDDOfst"+String(p) , DevOptn.dDVDD_IDDOfst[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dAVDD_IDSOfst"+String(p) , DevOptn.dAVDD_IDSOfst[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCVDD_IDSOfst"+String(p) , DevOptn.dCVDD_IDSOfst[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dHVDD_IDSOfst"+String(p) , DevOptn.dHVDD_IDSOfst[p]);
        UserINI.Load(sPath.c_str()  , "DevInfo"  , "dDVDD_IDSOfst"+String(p) , DevOptn.dDVDD_IDSOfst[p]);
    }




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
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPickDelay" , DevOptn.iPickDelay );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPlceDelay" , DevOptn.iPlceDelay );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iLotTrayCnt", DevOptn.iLotTrayCnt);

    UserINI.Save(sPath.c_str() , "DevInfo" , "iOSTDeviceNo", DevOptn.iOSTDeviceNo);

    UserINI.Save(sPath.c_str() , "DevInfo" , "dIDD_AVDDOkMin", DevOptn.dIDD_AVDDOkMin); UserINI.Save(sPath.c_str() , "DevInfo" , "dIDD_AVDDOkMax", DevOptn.dIDD_AVDDOkMax);
    UserINI.Save(sPath.c_str() , "DevInfo" , "dIDD_CVDDOkMin", DevOptn.dIDD_CVDDOkMin); UserINI.Save(sPath.c_str() , "DevInfo" , "dIDD_CVDDOkMax", DevOptn.dIDD_CVDDOkMax);
    UserINI.Save(sPath.c_str() , "DevInfo" , "dIDD_HVDDOkMin", DevOptn.dIDD_HVDDOkMin); UserINI.Save(sPath.c_str() , "DevInfo" , "dIDD_HVDDOkMax", DevOptn.dIDD_HVDDOkMax);
    UserINI.Save(sPath.c_str() , "DevInfo" , "dIDD_DVDDOkMin", DevOptn.dIDD_DVDDOkMin); UserINI.Save(sPath.c_str() , "DevInfo" , "dIDD_DVDDOkMax", DevOptn.dIDD_DVDDOkMax);

    UserINI.Save(sPath.c_str() , "DevInfo" , "dIDS_AVDDOkMin", DevOptn.dIDS_AVDDOkMin); UserINI.Save(sPath.c_str() , "DevInfo" , "dIDS_AVDDOkMax", DevOptn.dIDS_AVDDOkMax);
    UserINI.Save(sPath.c_str() , "DevInfo" , "dIDS_CVDDOkMin", DevOptn.dIDS_CVDDOkMin); UserINI.Save(sPath.c_str() , "DevInfo" , "dIDS_CVDDOkMax", DevOptn.dIDS_CVDDOkMax);
    UserINI.Save(sPath.c_str() , "DevInfo" , "dIDS_HVDDOkMin", DevOptn.dIDS_HVDDOkMin); UserINI.Save(sPath.c_str() , "DevInfo" , "dIDS_HVDDOkMax", DevOptn.dIDS_HVDDOkMax);
    UserINI.Save(sPath.c_str() , "DevInfo" , "dIDS_DVDDOkMin", DevOptn.dIDS_DVDDOkMin); UserINI.Save(sPath.c_str() , "DevInfo" , "dIDS_DVDDOkMax", DevOptn.dIDS_DVDDOkMax);

    UserINI.Save(sPath.c_str() , "DevInfo" , "iOST_I2cFailTime"  , DevOptn.iOST_I2cFailTime);
    UserINI.Save(sPath.c_str() , "DevInfo" , "bVTPwrAllwaysOn"   , DevOptn.bVTPwrAllwaysOn );

    for(int i = 0 ; i < MAX_OSP_PIN ; i++){
        UserINI.Save(sPath.c_str() , "DevInfo" , "sOSTPinName"+String(i), DevOptn.sOSTPinName[i] );
    }


    for(int p = 0 ; p < MAX_TOOL_SOCK_COL ; p++) {
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dAVDD_IDDGain"+String(p) , DevOptn.dAVDD_IDDGain[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCVDD_IDDGain"+String(p) , DevOptn.dCVDD_IDDGain[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dHVDD_IDDGain"+String(p) , DevOptn.dHVDD_IDDGain[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dDVDD_IDDGain"+String(p) , DevOptn.dDVDD_IDDGain[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dAVDD_IDSGain"+String(p) , DevOptn.dAVDD_IDSGain[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCVDD_IDSGain"+String(p) , DevOptn.dCVDD_IDSGain[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dHVDD_IDSGain"+String(p) , DevOptn.dHVDD_IDSGain[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dDVDD_IDSGain"+String(p) , DevOptn.dDVDD_IDSGain[p]);

        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dAVDD_IDDOfst"+String(p) , DevOptn.dAVDD_IDDOfst[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCVDD_IDDOfst"+String(p) , DevOptn.dCVDD_IDDOfst[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dHVDD_IDDOfst"+String(p) , DevOptn.dHVDD_IDDOfst[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dDVDD_IDDOfst"+String(p) , DevOptn.dDVDD_IDDOfst[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dAVDD_IDSOfst"+String(p) , DevOptn.dAVDD_IDSOfst[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCVDD_IDSOfst"+String(p) , DevOptn.dCVDD_IDSOfst[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dHVDD_IDSOfst"+String(p) , DevOptn.dHVDD_IDSOfst[p]);
        UserINI.Save(sPath.c_str()  , "DevInfo"  , "dDVDD_IDSOfst"+String(p) , DevOptn.dDVDD_IDSOfst[p]);
    }

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
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop    " , CmnOptn.bLoadingStop    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor       " , CmnOptn.bIgnrDoor       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkipOST        " , CmnOptn.bSkipOST        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkipVT         " , CmnOptn.bSkipVT         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkipVisn1      " , CmnOptn.bSkipVisn1      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkipVisn2      " , CmnOptn.bSkipVisn2      );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iOSTOSTBin      " , CmnOptn.iOSTOSTBin      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iOSTCrntBin     " , CmnOptn.iOSTCrnBin      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVTDIMBin       " , CmnOptn.iVTDIMBin       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVTBLKBin       " , CmnOptn.iVTBLKBin       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVTVHBin        " , CmnOptn.iVTVHBin        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVTDFTBin       " , CmnOptn.iVTDFTBin       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVTDFT2Bin      " , CmnOptn.iVTDFT2Bin      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVSNFailBin     " , CmnOptn.iVSNFailBin     );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkip1stTol     " , CmnOptn.bSkip1stTol     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkip2ndTol     " , CmnOptn.bSkip2ndTol     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkip3thTol     " , CmnOptn.bSkip3thTol     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bSkip4thTol     " , CmnOptn.bSkip4thTol     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bOSTNotUseCrnt  " , CmnOptn.bOSTNotUseCrnt  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bOSTNotUseOst   " , CmnOptn.bOSTNotUseOst   );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iBfrVTPowrDelay " , CmnOptn.iBfrVTPowrDelay );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iBfrVTInspDelay " , CmnOptn.iBfrVTInspDelay );  
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVTContBlkCnt   " , CmnOptn.iVTContBlkCnt   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVTContDftCnt   " , CmnOptn.iVTContDftCnt   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVTContDft2Cnt  " , CmnOptn.iVTContDft2Cnt  );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVTContVHCnt    " , CmnOptn.iVTContVHCnt    );



    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iVSContNGCnt    " , CmnOptn.iVSContNGCnt    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iOSContNGCnt    " , CmnOptn.iOSContNGCnt    );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iStgAftStopDelay" , CmnOptn.iStgAftStopDelay);

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sRj1Name        " , CmnOptn.sRj1Name        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sRj2Name        " , CmnOptn.sRj2Name        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sRj3Name        " , CmnOptn.sRj3Name        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "sRj4Name        " , CmnOptn.sRj4Name        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVisnAutoConv   " , CmnOptn.bVisnAutoConv   );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "dStgWorkOffset  " , CmnOptn.dStgWorkOffset  );

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iOSTLogDays     " , CmnOptn.iOSTLogDays     );


}

void COptionMan::SaveCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadingStop    " , CmnOptn.bLoadingStop    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrDoor       " , CmnOptn.bIgnrDoor       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkipOST        " , CmnOptn.bSkipOST        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkipVT         " , CmnOptn.bSkipVT         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkipVisn1      " , CmnOptn.bSkipVisn1      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkipVisn2      " , CmnOptn.bSkipVisn2      );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iOSTOSTBin      " , CmnOptn.iOSTOSTBin      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iOSTCrntBin     " , CmnOptn.iOSTCrnBin      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVTDIMBin       " , CmnOptn.iVTDIMBin       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVTBLKBin       " , CmnOptn.iVTBLKBin       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVTVHBin        " , CmnOptn.iVTVHBin        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVTDFTBin       " , CmnOptn.iVTDFTBin       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVTDFT2Bin      " , CmnOptn.iVTDFT2Bin      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVSNFailBin     " , CmnOptn.iVSNFailBin     );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkip1stTol     " , CmnOptn.bSkip1stTol     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkip2ndTol     " , CmnOptn.bSkip2ndTol     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkip3thTol     " , CmnOptn.bSkip3thTol     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bSkip4thTol     " , CmnOptn.bSkip4thTol     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bOSTNotUseCrnt  " , CmnOptn.bOSTNotUseCrnt  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bOSTNotUseOst   " , CmnOptn.bOSTNotUseOst   );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iBfrVTPowrDelay " , CmnOptn.iBfrVTPowrDelay );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iBfrVTInspDelay " , CmnOptn.iBfrVTInspDelay );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVTContBlkCnt   " , CmnOptn.iVTContBlkCnt   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVTContDftCnt   " , CmnOptn.iVTContDftCnt   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVTContDft2Cnt  " , CmnOptn.iVTContDft2Cnt  );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVTContVHCnt    " , CmnOptn.iVTContVHCnt    );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iVSContNGCnt    " , CmnOptn.iVSContNGCnt    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iOSContNGCnt    " , CmnOptn.iOSContNGCnt    );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iStgAftStopDelay" , CmnOptn.iStgAftStopDelay);

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sRj1Name        " , CmnOptn.sRj1Name        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sRj2Name        " , CmnOptn.sRj2Name        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sRj3Name        " , CmnOptn.sRj3Name        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "sRj4Name        " , CmnOptn.sRj4Name        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVisnAutoConv   " , CmnOptn.bVisnAutoConv   );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "dStgWorkOffset  " , CmnOptn.dStgWorkOffset  );

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iOSTLogDays     " , CmnOptn.iOSTLogDays     );


}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode      " , MstOptn.bDebugMode          );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bVacErrIgnr     " , MstOptn.bVacErrIgnr         );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "iInspRandMask   " , MstOptn.iInspRandMask       );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVisnRsltFile1  " , MstOptn.sVisnRsltFile1      );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVisnRsltFile2  " , MstOptn.sVisnRsltFile2      );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVisnDeviceFile " , MstOptn.sVisnDeviceFile     );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVisnLotNoFile  " , MstOptn.sVisnLotNoFile      );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVTDataFile1    " , MstOptn.sVTDataFile1        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVTDataFile2    " , MstOptn.sVTDataFile2        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVTDataFile3    " , MstOptn.sVTDataFile3        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVTDataFile4    " , MstOptn.sVTDataFile4        );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dOSTOffset1     " , MstOptn.dOSTOffset1         );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dOSTOffset2     " , MstOptn.dOSTOffset2         );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dOSTOffset3     " , MstOptn.dOSTOffset3         );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dOSTOffset4     " , MstOptn.dOSTOffset4         );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dOSTGain1       " , MstOptn.dOSTGain1           );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dOSTGain2       " , MstOptn.dOSTGain2           );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dOSTGain3       " , MstOptn.dOSTGain3           );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dOSTGain4       " , MstOptn.dOSTGain4           );

    if(MstOptn.dOSTGain1 <= 0.0) MstOptn.dOSTGain1 = 1.0 ;
    if(MstOptn.dOSTGain2 <= 0.0) MstOptn.dOSTGain2 = 1.0 ;
    if(MstOptn.dOSTGain3 <= 0.0) MstOptn.dOSTGain3 = 1.0 ;
    if(MstOptn.dOSTGain4 <= 0.0) MstOptn.dOSTGain4 = 1.0 ;

    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dTrigerOffset   " , MstOptn.dTrigerOffset       );

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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode      " , MstOptn.bDebugMode          );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bVacErrIgnr     " , MstOptn.bVacErrIgnr         );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "iInspRandMask   " , MstOptn.iInspRandMask       );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVisnRsltFile1  " , MstOptn.sVisnRsltFile1      );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVisnRsltFile2  " , MstOptn.sVisnRsltFile2      );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVisnDeviceFile " , MstOptn.sVisnDeviceFile     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVisnLotNoFile  " , MstOptn.sVisnLotNoFile      );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVTDataFile1    " , MstOptn.sVTDataFile1        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVTDataFile2    " , MstOptn.sVTDataFile2        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVTDataFile3    " , MstOptn.sVTDataFile3        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVTDataFile4    " , MstOptn.sVTDataFile4        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dOSTOffset1     " , MstOptn.dOSTOffset1         );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dOSTOffset2     " , MstOptn.dOSTOffset2         );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dOSTOffset3     " , MstOptn.dOSTOffset3         );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dOSTOffset4     " , MstOptn.dOSTOffset4         );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dOSTGain1       " , MstOptn.dOSTGain1           );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dOSTGain2       " , MstOptn.dOSTGain2           );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dOSTGain3       " , MstOptn.dOSTGain3           );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dOSTGain4       " , MstOptn.dOSTGain4           );

    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dTrigerOffset   " , MstOptn.dTrigerOffset       );
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


AnsiString COptionMan::GetBinName(int _iStat)
{
    AnsiString sRet = "" ;

    if(OM.CmnOptn.iOSTOSTBin  + 1== _iStat) {
        if(sRet == "") sRet = "OST_OST"  ;
        else           sRet+= "&&OST_OST" ;
    }
    if(OM.CmnOptn.iOSTCrnBin  + 1== _iStat) {
        if(sRet == "") sRet = "OST_Crnt"  ;
        else           sRet+= "&&OST_Crnt" ;
    }
    if(OM.CmnOptn.iVSNFailBin + 1== _iStat) {
        if(sRet == "") sRet = "VSN"  ;
        else           sRet+= "&&VSN" ;
    }
    if(OM.CmnOptn.iVTBLKBin   + 1== _iStat) {
        if(sRet == "") sRet = "VT_Blk"  ;
        else           sRet+= "&&VT_Blk" ;
    }
    if(OM.CmnOptn.iVTDFTBin   + 1== _iStat) {
        if(sRet == "") sRet = "VT_Dft"  ;
        else           sRet+= "&&VT_Dft" ;
    }
    if(OM.CmnOptn.iVTDFT2Bin  + 1== _iStat) {
        if(sRet == "") sRet = "VT_Dft2"  ;
        else           sRet+= "&&VT_Dft2" ;
    }
    if(OM.CmnOptn.iVTDIMBin   + 1== _iStat) {
        if(sRet == "") sRet = "VT_Dim"  ;
        else           sRet+= "&&VT_Dim" ;
    }
    if(OM.CmnOptn.iVTVHBin    + 1== _iStat) {
        if(sRet == "") sRet = "VT_Vh"  ;
        else           sRet+= "&&VT_Vh" ;
    }

    return sRet ;

}