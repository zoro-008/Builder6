//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "UserIni.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#include "DataMan.h"
#include "Head.h"
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
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColGrCnt    " , DevInfo.iColGrCnt    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowGrCnt    " , DevInfo.iRowGrCnt    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColGrGap    " , DevInfo.dColGrGap    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowGrGap    " , DevInfo.dRowGrGap    );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColCnt      " , DevInfo.iColCnt      ); if(!DevInfo.iColCnt   )DevInfo.iColCnt    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowCnt      " , DevInfo.iRowCnt      ); if(!DevInfo.iRowCnt   )DevInfo.iRowCnt    = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iCsSlCnt     " , DevInfo.iCsSlCnt     ); if(!DevInfo.iCsSlCnt  )DevInfo.iCsSlCnt   = 1 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dCsSlPitch   " , DevInfo.dCsSlPitch   ); if(!DevInfo.dCsSlPitch)DevInfo.dCsSlPitch = 1 ;
    //Vision
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "sVisnIndexId " , DevInfo.sVisnIndexId );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColInspCnt  " , DevInfo.iColInspCnt  );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowInspCnt  " , DevInfo.iRowInspCnt  );

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iRowSubGroupCount" , DevInfo.iRowSubGroupCount  ); //if(!DevInfo.iRowSubGroupCount)DevInfo.dRowSubGroupGap = 0 ;
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dRowSubGroupGap  " , DevInfo.dRowSubGroupGap    ); //if(!DevInfo.dRowSubGroupGap  )DevInfo.dRowSubGroupGap   = 0 ;

    UserINI.Load(sPath.c_str()  , "DevInfo"  , "iColSubGroupCount" , DevInfo.iColSubGroupCount  );
    UserINI.Load(sPath.c_str()  , "DevInfo"  , "dColSubGroupGap  " , DevInfo.dColSubGroupGap    );

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
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColPitch    " , DevInfo.dColPitch    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowPitch    " , DevInfo.dRowPitch    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColGrCnt    " , DevInfo.iColGrCnt    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowGrCnt    " , DevInfo.iRowGrCnt    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColGrGap    " , DevInfo.dColGrGap    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowGrGap    " , DevInfo.dRowGrGap    );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColCnt      " , DevInfo.iColCnt      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowCnt      " , DevInfo.iRowCnt      );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iCsSlCnt     " , DevInfo.iCsSlCnt     );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dCsSlPitch   " , DevInfo.dCsSlPitch   );
    //Vision
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "sVisnIndexId " , DevInfo.sVisnIndexId );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColInspCnt  " , DevInfo.iColInspCnt  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowInspCnt  " , DevInfo.iRowInspCnt  );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iRowSubGroupCount" , DevInfo.iRowSubGroupCount  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dRowSubGroupGap  " , DevInfo.dRowSubGroupGap    );

    UserINI.Save(sPath.c_str()  , "DevInfo"  , "iColSubGroupCount" , DevInfo.iColSubGroupCount  );
    UserINI.Save(sPath.c_str()  , "DevInfo"  , "dColSubGroupGap  " , DevInfo.dColSubGroupGap    );


}

void COptionMan::LoadDevOptn(AnsiString _sDevName)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "JobFile\\" + _sDevName + "\\DevOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt    " , DevOptn.iLotEndMgzCnt   );
    UserINI.Load(sPath.c_str()  , "DevOptn"  , "bDWidth          " , DevOptn.bDWidth         );

    UserINI.Load(sPath.c_str()  , "DevOptn"  , "iPunchingDelay   " , DevOptn.iPunchingDelay  );
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
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iLotEndMgzCnt    " , DevOptn.iLotEndMgzCnt    );
    UserINI.Save(sPath.c_str()  , "DevOptn"  , "bDWidth          " , DevOptn.bDWidth          );

    UserINI.Save(sPath.c_str()  , "DevOptn"  , "iPunchingDelay   " , DevOptn.iPunchingDelay   );


}

void COptionMan::LoadCmnOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\CmnOptn.INI" ;

    //Load Device.
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bCam1Skip           " , CmnOptn.bCam1Skip           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bCam2Skip           " , CmnOptn.bCam2Skip           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bTrm1Skip           " , CmnOptn.bTrm1Skip           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bTrmAllSkip         " , CmnOptn.bTrmAllSkip         );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bLoadingStop        " , CmnOptn.bLoadingStop        );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs1_1Skip          " , CmnOptn.bVs1_1Skip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs2_1Skip          " , CmnOptn.bVs2_1Skip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs3_1Skip          " , CmnOptn.bVs3_1Skip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs1_2Skip          " , CmnOptn.bVs1_2Skip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs2_2Skip          " , CmnOptn.bVs2_2Skip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVs3_2Skip          " , CmnOptn.bVs3_2Skip          );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bDoorSkip           " , CmnOptn.bDoorSkip           );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPRBStprDnDelay     " , CmnOptn.iPRBStprDnDelay     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iWK1StprDnDelay     " , CmnOptn.iWK1StprDnDelay     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iWK2StprDnDelay     " , CmnOptn.iWK2StprDnDelay     );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iPSBSortMoveDelay   " , CmnOptn.iPSBSortMoveDelay   );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iCheckFailCnt       " , CmnOptn.iCheckFailCnt       );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bVisionAllSkip      " , CmnOptn.bVisionAllSkip      );
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "bIgnrEmptyTrm       " , CmnOptn.bIgnrEmptyTrm       );


    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel0         " , CmnOptn.iRsltLevel0         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt0",CmnOptn.iCheckFailCnt0); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName0",CmnOptn.sRsltName0); UserINI.Load(sPath.c_str(),"CmnOptn","iSort0",CmnOptn.iSort0); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor0",CmnOptn.iRsltColor0);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel1         " , CmnOptn.iRsltLevel1         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt1",CmnOptn.iCheckFailCnt1); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName1",CmnOptn.sRsltName1); UserINI.Load(sPath.c_str(),"CmnOptn","iSort1",CmnOptn.iSort1); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor1",CmnOptn.iRsltColor1);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel2         " , CmnOptn.iRsltLevel2         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt2",CmnOptn.iCheckFailCnt2); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName2",CmnOptn.sRsltName2); UserINI.Load(sPath.c_str(),"CmnOptn","iSort2",CmnOptn.iSort2); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor2",CmnOptn.iRsltColor2);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel3         " , CmnOptn.iRsltLevel3         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt3",CmnOptn.iCheckFailCnt3); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName3",CmnOptn.sRsltName3); UserINI.Load(sPath.c_str(),"CmnOptn","iSort3",CmnOptn.iSort3); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor3",CmnOptn.iRsltColor3);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel4         " , CmnOptn.iRsltLevel4         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt4",CmnOptn.iCheckFailCnt4); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName4",CmnOptn.sRsltName4); UserINI.Load(sPath.c_str(),"CmnOptn","iSort4",CmnOptn.iSort4); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor4",CmnOptn.iRsltColor4);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel5         " , CmnOptn.iRsltLevel5         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt5",CmnOptn.iCheckFailCnt5); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName5",CmnOptn.sRsltName5); UserINI.Load(sPath.c_str(),"CmnOptn","iSort5",CmnOptn.iSort5); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor5",CmnOptn.iRsltColor5);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel6         " , CmnOptn.iRsltLevel6         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt6",CmnOptn.iCheckFailCnt6); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName6",CmnOptn.sRsltName6); UserINI.Load(sPath.c_str(),"CmnOptn","iSort6",CmnOptn.iSort6); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor6",CmnOptn.iRsltColor6);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel7         " , CmnOptn.iRsltLevel7         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt7",CmnOptn.iCheckFailCnt7); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName7",CmnOptn.sRsltName7); UserINI.Load(sPath.c_str(),"CmnOptn","iSort7",CmnOptn.iSort7); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor7",CmnOptn.iRsltColor7);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel8         " , CmnOptn.iRsltLevel8         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt8",CmnOptn.iCheckFailCnt8); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName8",CmnOptn.sRsltName8); UserINI.Load(sPath.c_str(),"CmnOptn","iSort8",CmnOptn.iSort8); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor8",CmnOptn.iRsltColor8);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel9         " , CmnOptn.iRsltLevel9         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt9",CmnOptn.iCheckFailCnt9); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName9",CmnOptn.sRsltName9); UserINI.Load(sPath.c_str(),"CmnOptn","iSort9",CmnOptn.iSort9); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor9",CmnOptn.iRsltColor9);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelA         " , CmnOptn.iRsltLevelA         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntA",CmnOptn.iCheckFailCntA); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameA",CmnOptn.sRsltNameA); UserINI.Load(sPath.c_str(),"CmnOptn","iSortA",CmnOptn.iSortA); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorA",CmnOptn.iRsltColorA);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelB         " , CmnOptn.iRsltLevelB         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntB",CmnOptn.iCheckFailCntB); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameB",CmnOptn.sRsltNameB); UserINI.Load(sPath.c_str(),"CmnOptn","iSortB",CmnOptn.iSortB); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorB",CmnOptn.iRsltColorB);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelC         " , CmnOptn.iRsltLevelC         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntC",CmnOptn.iCheckFailCntC); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameC",CmnOptn.sRsltNameC); UserINI.Load(sPath.c_str(),"CmnOptn","iSortC",CmnOptn.iSortC); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorC",CmnOptn.iRsltColorC);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelD         " , CmnOptn.iRsltLevelD         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntD",CmnOptn.iCheckFailCntD); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameD",CmnOptn.sRsltNameD); UserINI.Load(sPath.c_str(),"CmnOptn","iSortD",CmnOptn.iSortD); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorD",CmnOptn.iRsltColorD);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelE         " , CmnOptn.iRsltLevelE         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntE",CmnOptn.iCheckFailCntE); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameE",CmnOptn.sRsltNameE); UserINI.Load(sPath.c_str(),"CmnOptn","iSortE",CmnOptn.iSortE); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorE",CmnOptn.iRsltColorE);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelF         " , CmnOptn.iRsltLevelF         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntF",CmnOptn.iCheckFailCntF); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameF",CmnOptn.sRsltNameF); UserINI.Load(sPath.c_str(),"CmnOptn","iSortF",CmnOptn.iSortF); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorF",CmnOptn.iRsltColorF);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelG         " , CmnOptn.iRsltLevelG         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntG",CmnOptn.iCheckFailCntG); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameG",CmnOptn.sRsltNameG); UserINI.Load(sPath.c_str(),"CmnOptn","iSortG",CmnOptn.iSortG); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorG",CmnOptn.iRsltColorG);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelH         " , CmnOptn.iRsltLevelH         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntH",CmnOptn.iCheckFailCntH); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameH",CmnOptn.sRsltNameH); UserINI.Load(sPath.c_str(),"CmnOptn","iSortH",CmnOptn.iSortH); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorH",CmnOptn.iRsltColorH);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelI         " , CmnOptn.iRsltLevelI         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntI",CmnOptn.iCheckFailCntI); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameI",CmnOptn.sRsltNameI); UserINI.Load(sPath.c_str(),"CmnOptn","iSortI",CmnOptn.iSortI); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorI",CmnOptn.iRsltColorI);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelJ         " , CmnOptn.iRsltLevelJ         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntJ",CmnOptn.iCheckFailCntJ); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameJ",CmnOptn.sRsltNameJ); UserINI.Load(sPath.c_str(),"CmnOptn","iSortJ",CmnOptn.iSortJ); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorJ",CmnOptn.iRsltColorJ);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelK         " , CmnOptn.iRsltLevelK         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntK",CmnOptn.iCheckFailCntK); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameK",CmnOptn.sRsltNameK); UserINI.Load(sPath.c_str(),"CmnOptn","iSortK",CmnOptn.iSortK); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorK",CmnOptn.iRsltColorK);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevelL         " , CmnOptn.iRsltLevelL         ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCntL",CmnOptn.iCheckFailCntL); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltNameL",CmnOptn.sRsltNameL); UserINI.Load(sPath.c_str(),"CmnOptn","iSortL",CmnOptn.iSortL); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColorL",CmnOptn.iRsltColorL);

    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel22         " , CmnOptn.iRsltLevel22       ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt22",CmnOptn.iCheckFailCnt22); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName22",CmnOptn.sRsltName22); UserINI.Load(sPath.c_str(),"CmnOptn","iSort22",CmnOptn.iSort22); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor22",CmnOptn.iRsltColor22);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel24         " , CmnOptn.iRsltLevel24       ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt24",CmnOptn.iCheckFailCnt24); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName24",CmnOptn.sRsltName24); UserINI.Load(sPath.c_str(),"CmnOptn","iSort24",CmnOptn.iSort24); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor24",CmnOptn.iRsltColor24);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel25         " , CmnOptn.iRsltLevel25       ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt25",CmnOptn.iCheckFailCnt25); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName25",CmnOptn.sRsltName25); UserINI.Load(sPath.c_str(),"CmnOptn","iSort25",CmnOptn.iSort25); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor25",CmnOptn.iRsltColor25);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel26         " , CmnOptn.iRsltLevel26       ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt26",CmnOptn.iCheckFailCnt26); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName26",CmnOptn.sRsltName26); UserINI.Load(sPath.c_str(),"CmnOptn","iSort26",CmnOptn.iSort26); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor26",CmnOptn.iRsltColor26);
    UserINI.Load(sPath.c_str()  , "CmnOptn"  , "iRsltLevel27         " , CmnOptn.iRsltLevel27       ); UserINI.Load(sPath.c_str(),"CmnOptn","iCheckFailCnt27",CmnOptn.iCheckFailCnt27); UserINI.Load(sPath.c_str(),"CmnOptn","sRsltName27",CmnOptn.sRsltName27); UserINI.Load(sPath.c_str(),"CmnOptn","iSort27",CmnOptn.iSort27); UserINI.Load(sPath.c_str(),"CmnOptn","iRsltColor27",CmnOptn.iRsltColor27);

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
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bCam1Skip           " , CmnOptn.bCam1Skip           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bCam2Skip           " , CmnOptn.bCam2Skip           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bTrm1Skip           " , CmnOptn.bTrm1Skip           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bTrmAllSkip         " , CmnOptn.bTrmAllSkip         );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bLoadingStop        " , CmnOptn.bLoadingStop        );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs1_1Skip          " , CmnOptn.bVs1_1Skip          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs2_1Skip          " , CmnOptn.bVs2_1Skip          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs3_1Skip          " , CmnOptn.bVs3_1Skip          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs1_2Skip          " , CmnOptn.bVs1_2Skip          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs2_2Skip          " , CmnOptn.bVs2_2Skip          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVs3_2Skip          " , CmnOptn.bVs3_2Skip          );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bDoorSkip           " , CmnOptn.bDoorSkip           );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bAutoVsChange       " , CmnOptn.bAutoVsChange       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iWK1StprDnDelay     " , CmnOptn.iWK1StprDnDelay     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iWK2StprDnDelay     " , CmnOptn.iWK2StprDnDelay     );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iPSBSortMoveDelay   " , CmnOptn.iPSBSortMoveDelay   );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iCheckFailCnt       " , CmnOptn.iCheckFailCnt       );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bVisionAllSkip      " , CmnOptn.bVisionAllSkip      );
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "bIgnrEmptyTrm       " , CmnOptn.bIgnrEmptyTrm       );


    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel0         " , CmnOptn.iRsltLevel0         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt0",CmnOptn.iCheckFailCnt0); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName0",CmnOptn.sRsltName0); UserINI.Save(sPath.c_str(),"CmnOptn","iSort0",CmnOptn.iSort0); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor0",CmnOptn.iRsltColor0);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel1         " , CmnOptn.iRsltLevel1         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt1",CmnOptn.iCheckFailCnt1); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName1",CmnOptn.sRsltName1); UserINI.Save(sPath.c_str(),"CmnOptn","iSort1",CmnOptn.iSort1); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor1",CmnOptn.iRsltColor1);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel2         " , CmnOptn.iRsltLevel2         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt2",CmnOptn.iCheckFailCnt2); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName2",CmnOptn.sRsltName2); UserINI.Save(sPath.c_str(),"CmnOptn","iSort2",CmnOptn.iSort2); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor2",CmnOptn.iRsltColor2);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel3         " , CmnOptn.iRsltLevel3         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt3",CmnOptn.iCheckFailCnt3); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName3",CmnOptn.sRsltName3); UserINI.Save(sPath.c_str(),"CmnOptn","iSort3",CmnOptn.iSort3); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor3",CmnOptn.iRsltColor3);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel4         " , CmnOptn.iRsltLevel4         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt4",CmnOptn.iCheckFailCnt4); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName4",CmnOptn.sRsltName4); UserINI.Save(sPath.c_str(),"CmnOptn","iSort4",CmnOptn.iSort4); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor4",CmnOptn.iRsltColor4);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel5         " , CmnOptn.iRsltLevel5         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt5",CmnOptn.iCheckFailCnt5); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName5",CmnOptn.sRsltName5); UserINI.Save(sPath.c_str(),"CmnOptn","iSort5",CmnOptn.iSort5); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor5",CmnOptn.iRsltColor5);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel6         " , CmnOptn.iRsltLevel6         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt6",CmnOptn.iCheckFailCnt6); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName6",CmnOptn.sRsltName6); UserINI.Save(sPath.c_str(),"CmnOptn","iSort6",CmnOptn.iSort6); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor6",CmnOptn.iRsltColor6);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel7         " , CmnOptn.iRsltLevel7         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt7",CmnOptn.iCheckFailCnt7); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName7",CmnOptn.sRsltName7); UserINI.Save(sPath.c_str(),"CmnOptn","iSort7",CmnOptn.iSort7); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor7",CmnOptn.iRsltColor7);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel8         " , CmnOptn.iRsltLevel8         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt8",CmnOptn.iCheckFailCnt8); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName8",CmnOptn.sRsltName8); UserINI.Save(sPath.c_str(),"CmnOptn","iSort8",CmnOptn.iSort8); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor8",CmnOptn.iRsltColor8);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel9         " , CmnOptn.iRsltLevel9         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt9",CmnOptn.iCheckFailCnt9); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName9",CmnOptn.sRsltName9); UserINI.Save(sPath.c_str(),"CmnOptn","iSort9",CmnOptn.iSort9); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor9",CmnOptn.iRsltColor9);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelA         " , CmnOptn.iRsltLevelA         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntA",CmnOptn.iCheckFailCntA); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameA",CmnOptn.sRsltNameA); UserINI.Save(sPath.c_str(),"CmnOptn","iSortA",CmnOptn.iSortA); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorA",CmnOptn.iRsltColorA);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelB         " , CmnOptn.iRsltLevelB         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntB",CmnOptn.iCheckFailCntB); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameB",CmnOptn.sRsltNameB); UserINI.Save(sPath.c_str(),"CmnOptn","iSortB",CmnOptn.iSortB); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorB",CmnOptn.iRsltColorB);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelC         " , CmnOptn.iRsltLevelC         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntC",CmnOptn.iCheckFailCntC); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameC",CmnOptn.sRsltNameC); UserINI.Save(sPath.c_str(),"CmnOptn","iSortC",CmnOptn.iSortC); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorC",CmnOptn.iRsltColorC);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelD         " , CmnOptn.iRsltLevelD         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntD",CmnOptn.iCheckFailCntD); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameD",CmnOptn.sRsltNameD); UserINI.Save(sPath.c_str(),"CmnOptn","iSortD",CmnOptn.iSortD); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorD",CmnOptn.iRsltColorD);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelE         " , CmnOptn.iRsltLevelE         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntE",CmnOptn.iCheckFailCntE); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameE",CmnOptn.sRsltNameE); UserINI.Save(sPath.c_str(),"CmnOptn","iSortE",CmnOptn.iSortE); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorE",CmnOptn.iRsltColorE);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelF         " , CmnOptn.iRsltLevelF         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntF",CmnOptn.iCheckFailCntF); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameF",CmnOptn.sRsltNameF); UserINI.Save(sPath.c_str(),"CmnOptn","iSortF",CmnOptn.iSortF); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorF",CmnOptn.iRsltColorF);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelG         " , CmnOptn.iRsltLevelG         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntG",CmnOptn.iCheckFailCntG); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameG",CmnOptn.sRsltNameG); UserINI.Save(sPath.c_str(),"CmnOptn","iSortG",CmnOptn.iSortG); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorG",CmnOptn.iRsltColorG);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelH         " , CmnOptn.iRsltLevelH         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntH",CmnOptn.iCheckFailCntH); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameH",CmnOptn.sRsltNameH); UserINI.Save(sPath.c_str(),"CmnOptn","iSortH",CmnOptn.iSortH); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorH",CmnOptn.iRsltColorH);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelI         " , CmnOptn.iRsltLevelI         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntI",CmnOptn.iCheckFailCntI); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameI",CmnOptn.sRsltNameI); UserINI.Save(sPath.c_str(),"CmnOptn","iSortI",CmnOptn.iSortI); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorI",CmnOptn.iRsltColorI);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelJ         " , CmnOptn.iRsltLevelJ         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntJ",CmnOptn.iCheckFailCntJ); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameJ",CmnOptn.sRsltNameJ); UserINI.Save(sPath.c_str(),"CmnOptn","iSortJ",CmnOptn.iSortJ); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorJ",CmnOptn.iRsltColorJ);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelK         " , CmnOptn.iRsltLevelK         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntK",CmnOptn.iCheckFailCntK); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameK",CmnOptn.sRsltNameK); UserINI.Save(sPath.c_str(),"CmnOptn","iSortK",CmnOptn.iSortK); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorK",CmnOptn.iRsltColorK);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevelL         " , CmnOptn.iRsltLevelL         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCntL",CmnOptn.iCheckFailCntL); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltNameL",CmnOptn.sRsltNameL); UserINI.Save(sPath.c_str(),"CmnOptn","iSortL",CmnOptn.iSortL); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColorL",CmnOptn.iRsltColorL);

    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel22         " , CmnOptn.iRsltLevel22         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt22",CmnOptn.iCheckFailCnt22); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName22",CmnOptn.sRsltName22); UserINI.Save(sPath.c_str(),"CmnOptn","iSort22",CmnOptn.iSort22); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor22",CmnOptn.iRsltColor22);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel24         " , CmnOptn.iRsltLevel24         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt24",CmnOptn.iCheckFailCnt24); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName24",CmnOptn.sRsltName24); UserINI.Save(sPath.c_str(),"CmnOptn","iSort24",CmnOptn.iSort24); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor24",CmnOptn.iRsltColor24);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel25         " , CmnOptn.iRsltLevel25         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt25",CmnOptn.iCheckFailCnt25); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName25",CmnOptn.sRsltName25); UserINI.Save(sPath.c_str(),"CmnOptn","iSort25",CmnOptn.iSort25); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor25",CmnOptn.iRsltColor25);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel26         " , CmnOptn.iRsltLevel26         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt26",CmnOptn.iCheckFailCnt26); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName26",CmnOptn.sRsltName26); UserINI.Save(sPath.c_str(),"CmnOptn","iSort26",CmnOptn.iSort26); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor26",CmnOptn.iRsltColor26);
    UserINI.Save(sPath.c_str()  , "CmnOptn"  , "iRsltLevel27         " , CmnOptn.iRsltLevel27         ); UserINI.Save(sPath.c_str(),"CmnOptn","iCheckFailCnt27",CmnOptn.iCheckFailCnt27); UserINI.Save(sPath.c_str(),"CmnOptn","sRsltName27",CmnOptn.sRsltName27); UserINI.Save(sPath.c_str(),"CmnOptn","iSort27",CmnOptn.iSort27); UserINI.Save(sPath.c_str(),"CmnOptn","iRsltColor27",CmnOptn.iRsltColor27);    

}

void COptionMan::LoadMstOptn()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "Util\\MstOptn.INI" ;

    //Load Device.                                            
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDebugMode    " , MstOptn.bDebugMode   );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUld_YHome    " , MstOptn.bUld_YHome   );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bLdr_YHome    " , MstOptn.bLdr_YHome   );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDataAccEng   " , MstOptn.bDataAccEng  );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sVisnPath     " , MstOptn.sVisnPath    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "dTrigerOffset " , MstOptn.dTrigerOffset);
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bPsbVsErrIgnr " , MstOptn.bPsbVsErrIgnr);
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bDryRun       " , MstOptn.bDryRun      );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUseRamVsData " , MstOptn.bUseRamVsData  );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "sPstEquipPath " , MstOptn.sPstEquipPath  );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUseLotAuto   " , MstOptn.bUseLotAuto    );
    UserINI.Load(sPath.c_str()  , "MstOptn"  , "bUseZMarkingMT" , MstOptn.bUseZMarkingMT );


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
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDebugMode    " , MstOptn.bDebugMode     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUld_YHome    " , MstOptn.bUld_YHome     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bLdr_YHome    " , MstOptn.bLdr_YHome     );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDataAccEng   " , MstOptn.bDataAccEng    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sVisnPath     " , MstOptn.sVisnPath      );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "dTrigerOffset " , MstOptn.dTrigerOffset  );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bPsbVsErrIgnr " , MstOptn.bPsbVsErrIgnr  );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bDryRun       " , MstOptn.bDryRun        );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUseRamVsData " , MstOptn.bUseRamVsData  );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "sPstEquipPath " , MstOptn.sPstEquipPath  );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUseLotAuto   " , MstOptn.bUseLotAuto    );
    UserINI.Save(sPath.c_str()  , "MstOptn"  , "bUseZMarkingMT" , MstOptn.bUseZMarkingMT );


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
    UserINI.Load(sPath, "EqpOptn"  , "bExistLoader  " , EqpOptn.bExistLoader   );
    UserINI.Load(sPath, "EqpOptn"  , "bExistUnLoader" , EqpOptn.bExistUnLoader );

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
    UserINI.Save(sPath, "EqpOptn"  , "bExistLoader  " ,  EqpOptn.bExistLoader   );
    UserINI.Save(sPath, "EqpOptn"  , "bExistUnLoader" ,  EqpOptn.bExistUnLoader );
}

void COptionMan::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO    " , "Device        "  , m_sCrntDev     );
    UserINI.Load(sPath, "TOTAL CHIP        " , "iTotalChip    "  , iTotalChip     );
    UserINI.Load(sPath, "TOTAL FAIL CHIP   " , "iTotalFailChip"  , iTotalFailChip );

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
    UserINI.Save(sPath, "LAST WORK INFO    " , "Device        "  , m_sCrntDev     );
    UserINI.Save(sPath, "TOTAL CHIP        " , "iTotalChip    "  , iTotalChip     );
    UserINI.Save(sPath, "TOTAL FAIL CHIP   " , "iTotalFailChip"  , iTotalFailChip );

}

void COptionMan::DMSetMaxColRow(bool _bClear)
{
    DM.ARAY[riLDR   ].SetMaxColRow(1                  , OM.DevInfo.iCsSlCnt);
    DM.ARAY[riPRB   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riWR1   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riVS1   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riWR2   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riVS2   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riWR3   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riVS3   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riPSB   ].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riULD   ].SetMaxColRow(1                  , OM.DevInfo.iCsSlCnt);

    HED.DpAray1.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    HED.DpAray2.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    HED.DpAray3.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );

    if(_bClear) DM.ClearMap();

}
