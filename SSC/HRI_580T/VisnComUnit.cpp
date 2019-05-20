//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "VisnComUnit.h"
#include "UserFile.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"

#include "UserIni.h"
#include "LotUnit.h"
#include "SLogUnit.h"
#include "Timer.h"


#pragma package(smart_init)

CVisnComUnit VC;


template <typename T>
class CRunThread
{
    class TRunThread : public TThread
    {
        private:
            T Func;

        public:
            __fastcall TRunThread(T func) : TThread(false)
            {
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute()
            {
                Func();
            }
    };

    public:
        CRunThread(T func)
        {
            TRunThread*  thread = new TRunThread(func);
        }
};

/*  ** ����**
typedef void (__closure *Func)();
void __fastcall TForm1::Button3Click(TObject *Sender)
{
    Button3->Enabled = false ;
    CRunThread<Func> * func = new CRunThread<Func>(&TestFunc) ;
}

void TForm1::TestFunc()
{ 
    // ��û �ð��� �ɸ��� ��ƾ.
    int  k; 
    for(int c = 0; c < 299999999; c++)
    {
        k = c; 
        k++; 
        c = k; 
        c--; 
    } 
    Caption = k;
    Button3->Enabled = true;
}

*/

#define VISION_FOLDER "D:\\VisionLog\\"
#define LOT_LOG_FOLDER "D:\\LotLog\\"

CVisnComUnit::CVisnComUnit(void)
{
    //if (!DirectoryExists(OM.MstOptn.sPreEquipPath)) ForceDirectories(OM.MstOptn.sPreEquipPath) ;
    if (!DirectoryExists(OM.MstOptn.sVisnPath    )) ForceDirectories(OM.MstOptn.sVisnPath    ) ;


}

CVisnComUnit::~CVisnComUnit(void)
{
    Trace("","");
}

bool __fastcall CVisnComUnit::NetDrvConnect(AnsiString sDrv , AnsiString sIp, AnsiString sPath)
{
    AnsiString sTemp, slpLocalName, slpRemoteName ;
    NETRESOURCE NetRes;
    NetRes.dwScope = RESOURCE_GLOBALNET;
    NetRes.dwType  = RESOURCETYPE_ANY;
    NetRes.dwDisplayType = RESOURCEDISPLAYTYPE_GENERIC;
    NetRes.dwUsage = RESOURCEUSAGE_CONNECTABLE;
    slpLocalName = sDrv + ":"; // "Z:";
    NetRes.lpLocalName  = slpLocalName.c_str();
    slpRemoteName = "\\\\" + sIp + "\\" + sPath; //" \\\\192.168.0.76\\com";
    NetRes.lpRemoteName = slpRemoteName.c_str();
    NetRes.lpProvider    = NULL;

    DWORD Result;
    Result = WNetAddConnection2(&NetRes, "mds", "mds", CONNECT_INTERACTIVE);
    if(Result!=NO_ERROR){
        Result = WNetCancelConnection2(slpLocalName.c_str(),CONNECT_UPDATE_PROFILE,FALSE);       //Sever Network Drive
        if(Result != NO_ERROR) return false ; //FM_MsgOk("ERROR","Net Connect Failed! Check the Connection.");
        Result = WNetAddConnection2(&NetRes, "mds", "mds", CONNECT_INTERACTIVE); //Connect Network Drive
        if(Result != NO_ERROR) return false ; //FM_MsgOk("ERROR","Net Connect Failed! Check the Connection.");
    }

    return true ;
}

EN_CHIP_STAT CVisnComUnit::GetHighLevel (EN_CHIP_STAT _iOri , EN_CHIP_STAT _iNew)
{
    int iOriLevel ;
    int iNewLevel ;

    switch(_iOri) {
        default       : iOriLevel = -9999                   ; break ;
        case csRslt1  : iOriLevel = OM.CmnOptn.iLevelRslt1  ; break ;
        case csRslt2  : iOriLevel = OM.CmnOptn.iLevelRslt2  ; break ;
        case csRslt3  : iOriLevel = OM.CmnOptn.iLevelRslt3  ; break ;
        case csRslt4  : iOriLevel = OM.CmnOptn.iLevelRslt4  ; break ;
        case csRslt5  : iOriLevel = OM.CmnOptn.iLevelRslt5  ; break ;
        case csRslt6  : iOriLevel = OM.CmnOptn.iLevelRslt6  ; break ;
        case csRslt7  : iOriLevel = OM.CmnOptn.iLevelRslt7  ; break ;
        case csRslt8  : iOriLevel = OM.CmnOptn.iLevelRslt8  ; break ;
        case csRslt9  : iOriLevel = OM.CmnOptn.iLevelRslt9  ; break ;
        case csRslt10 : iOriLevel = OM.CmnOptn.iLevelRslt10 ; break ;
        case csRslt11 : iOriLevel = OM.CmnOptn.iLevelRslt11 ; break ;
        case csRslt12 : iOriLevel = OM.CmnOptn.iLevelRslt12 ; break ;
        case csRslt13 : iOriLevel = OM.CmnOptn.iLevelRslt13 ; break ;
        case csRslt14 : iOriLevel = OM.CmnOptn.iLevelRslt14 ; break ;
        case csFail   : iOriLevel = OM.CmnOptn.iLevelFail   ; break ;
        case csUnkwn  : iOriLevel = -9999                   ; break ;
    }

    switch(_iNew) {
        default       : iNewLevel = -9999                   ; break ;
        case csRslt1  : iNewLevel = OM.CmnOptn.iLevelRslt1  ; break ;
        case csRslt2  : iNewLevel = OM.CmnOptn.iLevelRslt2  ; break ;
        case csRslt3  : iNewLevel = OM.CmnOptn.iLevelRslt3  ; break ;
        case csRslt4  : iNewLevel = OM.CmnOptn.iLevelRslt4  ; break ;
        case csRslt5  : iNewLevel = OM.CmnOptn.iLevelRslt5  ; break ;
        case csRslt6  : iNewLevel = OM.CmnOptn.iLevelRslt6  ; break ;
        case csRslt7  : iNewLevel = OM.CmnOptn.iLevelRslt7  ; break ;
        case csRslt8  : iNewLevel = OM.CmnOptn.iLevelRslt8  ; break ;
        case csRslt9  : iNewLevel = OM.CmnOptn.iLevelRslt9  ; break ;
        case csRslt10 : iNewLevel = OM.CmnOptn.iLevelRslt10 ; break ;
        case csRslt11 : iNewLevel = OM.CmnOptn.iLevelRslt11 ; break ;
        case csRslt12 : iNewLevel = OM.CmnOptn.iLevelRslt12 ; break ;
        case csRslt13 : iNewLevel = OM.CmnOptn.iLevelRslt13 ; break ;
        case csRslt14 : iNewLevel = OM.CmnOptn.iLevelRslt14 ; break ;
        case csFail   : iNewLevel = OM.CmnOptn.iLevelFail   ; break ;
        case csUnkwn  : iNewLevel = -9999                   ; break ;
    }

    if( iOriLevel < iNewLevel) return _iNew ;
    else                       return _iOri ;

}

bool __fastcall CVisnComUnit::ReadResult(int iVisnNo, EN_ARAY_ID riId)
{
    //Local Var.
    int hFile       ;
    int iFileHandle ;
    int iFileLength ;
    int divpos      ;

    char *pszBuffer ;
    AnsiString AllField ;
    AnsiString sPath, Temp ,sName;

    int iFailCnt    ;
    int iFailRow    ;
    int iFailCol    ;
    int iFailItm    ;

    EN_CHIP_STAT iRslt ;

    sName = sName.sprintf("\\Vision%d.dat" ,iVisnNo );
    sPath = OM.MstOptn.sVisnPath + sName;

    if(!FileExists(sPath)) { Trace("Err",(sPath + " no file").c_str());  return false ;}

    iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    pszBuffer = new char[iFileLength+1];
    memset(pszBuffer , 0 , sizeof(char)*(iFileLength+1));

    //�޸� ���� �Ҵ� ���߿� �����ϱ�...!
    FileRead (iFileHandle, pszBuffer, iFileLength);
    FileClose(iFileHandle);

    AllField = pszBuffer;

    Trace(sName.c_str() , AllField.c_str());

    iFailCnt = AllField.SubString(1,3).ToIntDef(-1) ;
    if(iFailCnt == -1) { Trace("Err","Fail Cnt is not a No"); return false ; }

    if(iVisnNo == 1) m_iVs1TtlErrCnt = iFailCnt ;
    if(iVisnNo == 2) m_iVs2TtlErrCnt = iFailCnt ;


    DM.ARAY[riId].ChangeStat(csUnkwn , csWork );

    WriteLogVisnErr(AllField);

    AllField.Delete(1,3);

//    if(iVisnNo == 1 ) {
//        DM.ARAY[riId].ChangeStat(csRslt0 ,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt1 ,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt2 ,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt3 ,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt4 ,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt5 ,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt6 ,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt7 ,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt7 ,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt9 ,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt10,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt11,csWork);
//       DM.ARAY[riId].ChangeStat(csRslt12,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt13,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt14,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt15,csWork);
//        DM.ARAY[riId].ChangeStat(csRslt16,csWork);
//    }

//    if(iVisnNo == 2 ) {
        //DM.ShiftArrayData(riW2T,riId);
//    }

    for (int i = 0; i < iFailCnt; i ++ ) {
        //Col.
        iFailCol = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailCol <= 0                ) { Trace("Err","iFailCol < 0"      ); return false ; }
        if(iFailCol > OM.DevInfo.iColCnt) { Trace("Err","iFailCol > iColCnt"); return false ; }
        AllField.Delete(1,2);

        //Row.
        iFailRow = AllField.SubString(1,2).ToIntDef(-1);
        if(iFailRow <= 0                ) { Trace("Err","iFailRow < 0"      ); return false ; }
        if(iFailRow > OM.DevInfo.iRowCnt) { Trace("Err","iFailRow > iRowCnt"); return false ; }
        AllField.Delete(1,2);


        iFailItm = StrToInt("0x"+AllField.SubString(1,1));
        if(iFailItm <  0             ) { Trace("Err","iFailItm < 0"            ); return false ; }
        if(iFailItm >= MAX_CHIP_STAT ) { Trace("Err","iFailItm > MAX_CHIP_STAT"); return false ; }




        AllField.Delete(1,1);

        iRslt = GetHighLevel(DM.ARAY[riId].GetStat(OM.DevInfo.iRowCnt-iFailRow ,iFailCol-1),(EN_CHIP_STAT)iFailItm);

        if(iVisnNo == 1) m_iVs1ErrCnt[iRslt]++ ;
        if(iVisnNo == 2) m_iVs2ErrCnt[iRslt]++ ;


        DM.ARAY[riId].SetStat(OM.DevInfo.iRowCnt-iFailRow ,iFailCol-1 , iRslt) ;
    }

    WriteLogMap(riId);
    LT.WriteArayData(riId);

    delete [] pszBuffer;

    if(!DeleteFile(sPath)) { Trace("Err",(sPath + "is Can't Deleted").c_str());  return false ; }

    return true ;
}

int CVisnComUnit::GetLastErrCnt(int _iVisnNo , EN_CHIP_STAT _csStat)
{
    return (_iVisnNo == 1 ? m_iVs1ErrCnt[_csStat] : m_iVs2ErrCnt[_csStat]);
}

int CVisnComUnit::GetLastTtlErrCnt(int _iVisnNo )
{
    return (_iVisnNo == 1 ? m_iVs1TtlErrCnt : m_iVs2TtlErrCnt );
}

int CVisnComUnit::ClearErrCnt()
{
    memset(&m_iVs1ErrCnt , 0 ,sizeof(int) * MAX_CHIP_STAT);
    memset(&m_iVs2ErrCnt , 0 ,sizeof(int) * MAX_CHIP_STAT);

    m_iVs1TtlErrCnt = 0 ;
    m_iVs2TtlErrCnt = 0 ;

}

EN_VISN_RESET_SEL iVisnResetSel = vrBoth ;
typedef void (__closure *Func)();
bool CVisnComUnit::SendReset(EN_VISN_RESET_SEL _iSel , bool _bUseThread)
{
    iVisnResetSel = _iSel ;
    if(_bUseThread){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadReset) ;
    }
    else {
        ThreadReset();
    }
    return true ;

}
bool CVisnComUnit::SendLotStart(bool _bUseThread)
{
    if(_bUseThread){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadLotStart) ;
    }
    else {
        ThreadLotStart();
    }
    return true ;
}

bool CVisnComUnit::SendModelChange(bool _bUseThread)
{
    if(_bUseThread){
        CRunThread<Func> * func = new CRunThread<Func>(&ThreadModelChange) ;
    }
    else {
        ThreadModelChange();
    }
    return true ;
}


void CVisnComUnit::ThreadReset()
{
    CDelayTimer Timer ;
    bool bVisn1 = iVisnResetSel == vrVisn1 || iVisnResetSel == vrBoth ;
    bool bVisn2 = iVisnResetSel == vrVisn2 || iVisnResetSel == vrBoth ;

    int iStep    = 10 ;
    int iPreStep = 0  ;
    int iSendCnt = 0  ;


    while(1) {
        Application -> ProcessMessages() ;
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmReset.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 5000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiHED_VisnCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("CVisnComUnit",sTemp.c_str());
            IO_SetY(yHED_1Reset , false) ;
            IO_SetY(yHED_2Reset , false) ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("CVisnComUnit",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("CVisnComUnit", sTemp.c_str());
                      if(bVisn1)IO_SetY(yHED_1Reset , false) ;
                      if(bVisn2)IO_SetY(yHED_2Reset , false) ;
                      iStep = 0 ;
                      return ;

            case  10: iSendCnt = 0 ;
                      if(bVisn1)IO_SetY(yHED_1Reset , false) ;
                      if(bVisn2)IO_SetY(yHED_2Reset , false) ;
                      iStep++;
                      continue ;

            case  11: iSendCnt ++ ;
                      if(bVisn1)IO_SetY(yHED_1Reset , true) ;
                      if(bVisn2)IO_SetY(yHED_2Reset , true) ;
                      Timer.Clear() ;

                      iStep++;
                      continue ;

            case  12: //if(!Timer.OnDelay(true , 2000)) continue ;

                      if(bVisn1 && IO_GetX(xHED_1Ready,true)) continue ;
                      if(bVisn2 && IO_GetX(xHED_2Ready,true)) continue ;
                      iStep++;
                      continue ;

            case  13: if(bVisn1)IO_SetY(yHED_1Reset ,false) ;
                      if(bVisn2)IO_SetY(yHED_2Reset ,false) ;
                      iStep++;
                      continue ;

            case  14: if(bVisn1 && !IO_GetX(xHED_1Ready,true)) continue ;
                      if(bVisn2 && !IO_GetX(xHED_2Ready,true)) continue ;

                      Timer.Clear() ;
                      iStep++;
                      continue ;

            case  15: if(!Timer.OnDelay(true , 500)) continue ;

                      if(iSendCnt == 1 ) {
                          if((bVisn1 &&!IO_GetX(xHED_1Busy,true)) || (bVisn2 &&!IO_GetX(xHED_2Busy,true))) {  //������ �̻��ϰ� 2�� ���� ��� �˻���ġ�� Ŭ�����.
                              iStep = 11 ;
                              continue ;
                          }
                      }



                      if((bVisn1 && IO_GetX(xHED_1Busy,true)) || (bVisn2 &&IO_GetX(xHED_2Busy,true))) {
                          if(iSendCnt > 2) {  //2�� �ؼ��� ������ ������ ��Ǯ���� ����.
                              EM_SetDisp(true) ;
                              EM_SetErr(eiHED_VisnResetCnt) ;
                              return ;
                          }
                          iSendCnt++;
                          iStep = 11 ;
                          continue ;
                      }
                      Trace("VisnCom","SendReset Complete!");
                      iStep = 0 ;
                      return ;
        }
    }
}

void CVisnComUnit::ThreadLotStart()
{
    CDelayTimer Timer ;
    int iStep    = 10 ;
    int iPreStep = 0  ;
    int iSendCnt = 0  ;

    while(1) {
        Application -> ProcessMessages() ;
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmLotStart.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode , 2000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiHED_VisnCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("CVisnComUnit",sTemp.c_str());
            iStep = 0 ;
            IO_SetY(yHED_1LotStart , false) ;
            IO_SetY(yHED_2LotStart , false) ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("CVisnComUnit",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("CVisnComUnit", sTemp.c_str());
                      iStep = 0 ;
                      return ;

            case  10: if(!IO_GetX(xHED_1Ready,true) && !OM.CmnOptn.bVisn1Skip) {
                          EM_SetErr(eiHED_Visn1NotReady);
                          EM_SetDisp(true) ;
                          return ;
                      }
                      if(!IO_GetX(xHED_2Ready,true) && !OM.CmnOptn.bVisn2Skip) {
                          EM_SetErr(eiHED_Visn2NotReady);
                          EM_SetDisp(true) ;
                          return ;
                      }
                      WriteVisnLot();   //��� ���� ���� �۾� �ϸ��.
                      IO_SetY(yHED_1LotStart , true) ;
                      IO_SetY(yHED_2LotStart , true) ;

                      iStep++;
                      continue ;

            case  11: //if(!Timer.OnDelay(true , 100)) continue ;
                      if(IO_GetX(xHED_1Ready,true)&& !OM.CmnOptn.bVisn1Skip) continue ;
                      if(IO_GetX(xHED_2Ready,true)&& !OM.CmnOptn.bVisn2Skip) continue ;
                      iStep++;
                      continue ;

            case  12: IO_SetY(yHED_1LotStart ,false) ;
                      IO_SetY(yHED_2LotStart ,false) ;
                      iStep++;
                      continue ;

            case  13: if(!IO_GetX(xHED_1Ready,true)&& !OM.CmnOptn.bVisn1Skip) continue ;
                      if(!IO_GetX(xHED_2Ready,true)&& !OM.CmnOptn.bVisn2Skip) continue ;
                      Timer.Clear();
                      iStep++;
                      continue ;

            case  14: //if(Timer.OnDelay(true , 1000)) {
                      //    if(iSendCnt > 2) {  //2�� �ؼ��� ������ ������ ��Ǯ���� ����.
                      //        EM_SetDisp(true) ;
                      //        EM_SetErr(eiHED_VisnResetCnt) ;
                      //        return ;
                      //    }
                      //    iSendCnt++;
                      //    iStep = 11 ;
                      //    Trace("VisnCom","SendLotStart Retry!");
                      //    continue ;
                      //}

                      if(IO_GetX(xHED_1Busy,true)) continue ;
                      if(IO_GetX(xHED_2Busy,true)) continue ;
                      Trace("VisnCom","SendLotStart Complete!");
                      iStep = 0 ;
                      return ;

        }
    }
}

void CVisnComUnit::ThreadModelChange()
{
    CDelayTimer Timer ;
    int iStep    = 10 ;
    int iPreStep = 0  ;
    int iSendCnt = 0  ;
    while(1) {
        Application -> ProcessMessages() ;
        Sleep(1);
        //Check Cycle Time Out.
        AnsiString sTemp ;
        if (m_tmModelChange.OnDelay(iStep == iPreStep && !OM.MstOptn.bDebugMode ,iStep == 14 ? 20000 : 5000 )) {
            EM_SetDisp(true);
            EM_SetErr(eiHED_VisnCommTO);
            sTemp = sTemp.sprintf("%s TIMEOUT STATUS : iStep=%02d" , __FUNC__ , iStep );
            Trace("CVisnComUnit",sTemp.c_str());
            IO_SetY(yHED_1JobChange , false) ;
            IO_SetY(yHED_2JobChange , false) ;
            iStep = 0 ;
            return ;
        }

        if(iStep != iPreStep) {
            sTemp = sTemp.sprintf("%s iStep=%02d" , __FUNC__ , iStep );
            Trace("CVisnComUnit",sTemp.c_str());
        }

        iPreStep = iStep ;

        //Cycle.
        switch (iStep) {

            default : sTemp = sTemp.sprintf("Cycle Default Clear %s iStep=%02d" , __FUNC__ , iStep );
                      Trace("CVisnComUnit", sTemp.c_str());
                      iStep = 0 ;
                      return  ;

            case  10: if(!IO_GetX(xHED_1Ready,true)) {
                          EM_SetErr(eiHED_Visn1NotReady);
                          EM_SetDisp(true) ;
                          return ;
                      }
                      if(!IO_GetX(xHED_2Ready,true)) {
                          EM_SetErr(eiHED_Visn2NotReady);
                          EM_SetDisp(true) ;
                          return ;
                      }
                      WriteVisnModel();

                      IO_SetY(yHED_1JobChange , true) ;
                      IO_SetY(yHED_2JobChange , true) ;
                      Timer.Clear();
                      iStep++;
                      continue ;

            case  11: if(IO_GetX(xHED_1Ready,true)) continue ;
                      if(IO_GetX(xHED_2Ready,true)) continue ;
                      //if(!Timer.OnDelay(true , 1000)) continue ;

                      iStep++;
                      continue ;

            case  12: IO_SetY(yHED_1JobChange ,false) ;
                      IO_SetY(yHED_2JobChange ,false) ;
                      iStep++;
                      continue ;

            case  13: if(!IO_GetX(xHED_1Ready,true)) continue ;
                      if(!IO_GetX(xHED_2Ready,true)) continue ;
                      Timer.Clear();
                      iStep++;
                      continue ;

            //���ܹٲܶ� Ÿ�Ӿƿ� ����
            case  14: //if(Timer.OnDelay(true , 1000)) {
                      //    if(iSendCnt > 2) {  //2�� �ؼ��� ������ ������ ��Ǯ���� ����.
                      //        EM_SetDisp(true) ;
                      //        EM_SetErr(eiHED_VisnResetCnt) ;
                      //        return ;
                      //    }
                      //    iSendCnt++;
                      //    iStep = 11 ;
                      //    Trace("VisnCom","SendLotStart Retry!");
                      //    continue ;
                      //}
                      if(IO_GetX(xHED_1Busy,true)) continue ;
                      if(IO_GetX(xHED_2Busy,true)) continue ;
                      iStep = 0 ;
                      Trace("VisnCom","SendJobChange Complete!");
                      return ;
        }
    }
}

bool __fastcall CVisnComUnit::WriteVisnLot()
{
    //Local Var.
    int        hFile  ;
    AnsiString sPath  ;
    AnsiString sData  ;
    int        sState ;

    //Set Path.
    sPath = OM.MstOptn.sVisnPath ;
    sData = LT.GetCrntLot().Trim()+";" ;
    //

    if (!DirectoryExists(sPath)) CreateDir(sPath);
    sPath = OM.MstOptn.sVisnPath + "\\" + "LotData.dat";
    if (FileExists(sPath)) DeleteFile(sPath) ;


    hFile = FileOpen(sPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }
    }

    //AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'�� 'n'��' s'�� \r\n'");
    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);
    return true;
}

bool __fastcall CVisnComUnit::WriteVisnModel()
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sData       ;
    int        sState      ;

    //Set Path.

    sPath = OM.MstOptn.sVisnPath ;
    sData = OM.DevOptn.sVisnIndexId + ";" + OM.m_sCrntDev + ";";

    if (!DirectoryExists(sPath)) CreateDir(sPath);

    for(int i = 1 ; i< MAX_VISN+1 ; i++) {
        sPath = OM.MstOptn.sVisnPath + "\\" + "Change" + AnsiString(i) + ".dat";
        if (FileExists(sPath)) DeleteFile(sPath) ;
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }

        //AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'�� 'n'��' s'�� \r\n'");
        FileSeek (hFile , 0             , SEEK_END      );
        FileWrite(hFile , sData.c_str() , sData.Length());

        //Close File.
        FileClose(hFile);
    }
    return true;
}


void __fastcall CVisnComUnit::DelPastLog()
{
    UserFile.ClearDirDate(VISION_FOLDER , Now() - 30);
}

bool __fastcall CVisnComUnit::WriteLogVisnErr(AnsiString _sLog) //���� ��������� �α׷� ���� �Ѵ�.
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sTemp,sTime ;
    int        sState      ;
    TDateTime  tLotStartTime;

    DelPastLog();

    //Set Path.
    if(LT.m_bLotOpen) tLotStartTime.Val = LT.LotInfo.dStartTime;
    else               tLotStartTime     = Now()                 ;
    sTime = tLotStartTime.FormatString("hhnnss");

    if(LT.m_bLotOpen)sPath = VISION_FOLDER + LT.GetCrntLot() + "_" + sTime + ".dat";
    else             sPath = VISION_FOLDER + AnsiString("_") + sTime + ".dat";

    if (!DirectoryExists(sPath)) CreateDir(sPath);

    hFile = FileOpen(sPath.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }
    }

    _sLog= _sLog + "\r\n" ;
    AnsiString sDate = Now().CurrentDateTime().FormatString("hh::nn::ss ");
    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , _sLog.c_str() , _sLog.Length());

    //Close File.
    FileClose(hFile);
    return true;
}

bool __fastcall CVisnComUnit::WriteLogMap(EN_ARAY_ID riId )//�׳� ���� �; �ϳ� ����.
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath ,sFName ;
    AnsiString sData  ;
    AnsiString sTemp ,sTemp2 ;
    AnsiString sRslt  ;
    AnsiString sTime  ;
    TDateTime  CurrDate;
    TDateTime  CurrDateTime;
    int        iMgzCnt;
    int        hFile;
    DelPastLog();

    //Set Dir.
    sTemp        = CurrDate.CurrentDate().FormatString("yyyymmdd");
    sTime        = CurrDateTime.CurrentDate().FormatString("yyyymmdd_hh_nn_ss");
    sPath        = LOT_LOG_FOLDER + sTemp ;

    if(DM.ARAY[riId].GetLotNo() != "") sPath += "\\" + DM.ARAY[riId].GetLotNo();//   LT.GetCrntLot() ;
    else                               sPath += "\\NoLot"                      ;

    bool bRet = false ;

    if(!DirectoryExists(sPath)) bRet = UserFile.CreateDir(sPath);

    iMgzCnt = DM.ARAY[riId].GetID().ToIntDef(9999) / 100;

    //int iArayId = EN_ARAY_ID(riId);

    if(riId == riPRB) sFName = "PreBuffer" ;
    if(riId == riWK1) sFName = "Vision1"   ;
    if(riId == riWK2) sFName = "Vision2"   ;
    if(riId == riPSB) sFName = "PostBuffer";

    if(DM.ARAY[riId].GetLotNo() != "") sPath += "\\" + sFName + ".csv" ;
    else                               sPath += "\\" + sTime  + ".csv" ;

    //File Open.
    hFile = FileOpen(sPath.c_str() , fmOpenWrite);

    if (hFile == -1) {
        hFile = FileCreate(sPath.c_str());
        if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }
    }

    sData  = "=================================================================================\r\n";
    sData += "Time,LotNo,MgzNo,SlotNo\r\n"        ;
    sData += Now().FormatString("hh:nn:ss") + "," ;
    sData += DM.ARAY[riId].GetLotNo()       + "," ;
    sData += AnsiString(DM.ARAY[riId].GetID().ToIntDef(9999)/100) + ","   ;
    sData += AnsiString(DM.ARAY[riId].GetID().ToIntDef(99  )%100) + "\r\n";
    int iStat;
    for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
//        sRslt = "" ;
        for(int c = 0 ; c < OM.DevInfo.iColCnt ; c++) {
            //sRslt += IntToHex(((int)DM.ARAY[riId].GetStat(r,c)) , 2)  ;
            iStat = (int)DM.ARAY[riId].GetStat(r,c);

                 if((EN_CHIP_STAT)csFail    == iStat ) sData +=  "HEAT    ," ;
            else if((EN_CHIP_STAT)csEmpty   == iStat ) sData +=  "EMPTY   ," ;
            else if((EN_CHIP_STAT)csNone    == iStat ) sData +=  "NONE    ," ;
            else if((EN_CHIP_STAT)csUnkwn   == iStat ) sData +=  "UNKNWN  ," ;
            else if((EN_CHIP_STAT)csWork    == iStat ) sData +=  "        ," ;
            else if((EN_CHIP_STAT)csRslt1   == iStat ) sData +=  "Empty   ," ;
            else if((EN_CHIP_STAT)csRslt2   == iStat ) sData +=  "Chip    ," ;
            else if((EN_CHIP_STAT)csRslt3   == iStat ) sData +=  "Dust    ," ;
            else if((EN_CHIP_STAT)csRslt4   == iStat ) sData +=  "LFlow   ," ;
            else if((EN_CHIP_STAT)csRslt5   == iStat ) sData +=  "Broken  ," ;
            else if((EN_CHIP_STAT)csRslt6   == iStat ) sData +=  "Crack   ," ;
            else if((EN_CHIP_STAT)csRslt7   == iStat ) sData +=  "Burr    ," ;
            else if((EN_CHIP_STAT)csRslt8   == iStat ) sData +=  "Zener   ," ;
            else if((EN_CHIP_STAT)csRslt9   == iStat ) sData +=  "Wire    ," ;
            else if((EN_CHIP_STAT)csRslt10  == iStat ) sData +=  "Orient  ," ;
            else if((EN_CHIP_STAT)csRslt11  == iStat ) sData +=  "Pmeasure," ;
            else if((EN_CHIP_STAT)csRslt12  == iStat ) sData +=  "Pdist   ," ;
            else if((EN_CHIP_STAT)csRslt13  == iStat ) sData +=  "Match   ," ;
            else if((EN_CHIP_STAT)csRslt14  == iStat ) sData +=  "Flow    ," ;
//            else if((EN_CHIP_STAT)csToBuf   == iStat ) sData +=  "ToBuf "     ;
//            else if((EN_CHIP_STAT)csFromBuf == iStat ) sData +=  "FromBuf "   ;

//            sData += (AnsiString)iStat + ",";
//            sTemp2 = sTemp2.sprintf("%02d",(int)DM.ARAY[riId].GetStat(r,c)) + "_";
//            sRslt += sTemp2 ;
        }
//        sTemp.printf("R%02d", r);
//        UserINI.Save(sPath.c_str()  , sTime  , sTemp , sRslt );
        sData += "\r\n";
    }

    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return true ;
}
