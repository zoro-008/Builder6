//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "VisionCom.h"
#include <stdio.h>
#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
#include "OptionMan.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CVisionCom VSN_L;
CVisionCom VSN_R;

#define VISN_FOLDER "c:\\Data"
CVisionCom::CVisionCom(void)
{
    String sPath = VISN_FOLDER ;
    if (!DirectoryExists(sPath)) CreateDir(sPath);
}

CVisionCom::~CVisionCom (void)
{

}

void CVisionCom::Init()
{
    static int iVisnId = 0 ;

    if(iVisnId==0){
        VIO.xVisn_Ready     = xVisnL_Ready    ;
        VIO.xVisn_Busy      = xVisnL_Busy     ;
        VIO.xVisn_InspOk    = xVisnL_InspOk   ;
        VIO.xVisn_Spare1    = xVisnL_Spare1   ;
        VIO.xVisn_Spare2    = xVisnL_Spare2   ;

        VIO.yVisn_InspStart = yVisnL_InspStart;
        VIO.yVisn_Reset     = yVisnL_Reset    ;
        VIO.yVisn_Command   = yVisnL_Command  ;
        VIO.yVisn_JobChange = yVisnL_JobChange;
        VIO.yVisn_Live      = yVisnL_Spare1   ;
    }
    else {
        VIO.xVisn_Ready     = xVisnR_Ready    ;
        VIO.xVisn_Busy      = xVisnR_Busy     ;
        VIO.xVisn_InspOk    = xVisnR_InspOk   ;
        VIO.xVisn_Spare1    = xVisnR_Spare1   ;
        VIO.xVisn_Spare2    = xVisnR_Spare2   ;

        VIO.yVisn_InspStart = yVisnR_InspStart;
        VIO.yVisn_Reset     = yVisnR_Reset    ;
        VIO.yVisn_Command   = yVisnR_Command  ;
        VIO.yVisn_JobChange = yVisnR_JobChange;
        VIO.yVisn_Live      = yVisnR_Live    ;
    }                              
    m_iVisnId = iVisnId ;
    iVisnId++;

}

void CVisionCom::Close()
{

}

void CVisionCom::Reset()
{
    IO_SetY(VIO.yVisn_InspStart , false);
    IO_SetY(VIO.yVisn_Reset     , false);
    IO_SetY(VIO.yVisn_Command   , false);
    IO_SetY(VIO.yVisn_JobChange , false);
    //IO_SetY(VIO.yVisn_Live      , false);

    
}

bool CVisionCom::SaveVisnJobFile(AnsiString _sDevice)
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sData       ;

    //Set Path.
    sData = _sDevice;


    //폴더 뚫기 위해.
    sPath = VISN_FOLDER;
    if (!DirectoryExists(sPath)) CreateDir(sPath);
    sPath += String("\\Visn") + (m_iVisnId==0 ? "L" : "R") + "JobChange.dat" ;
    //String sTemp ;
    //sTemp = "\\Visn" ;


    //원래 있던파일 지우고.
    if (FileExists(sPath)) DeleteFile(sPath) ;
    hFile = FileCreate(sPath.c_str());
    if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }

    //AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'시 'n'분' s'초 \r\n'");
    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return true ;

}

//안씀-===================================.
bool CVisionCom::SaveVisnLotNo(AnsiString _sLotNo)
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sData       ;

    //Set Path.
    sData = _sLotNo;


    //폴더 뚫기 위해.
    sPath = VISN_FOLDER ;
    if (!DirectoryExists(sPath)) CreateDir(sPath);
    sPath += String("\\Visn") + (m_iVisnId==0 ? "L" : "R") + "LotNo.dat" ;

    //원래 있던파일 지우고.
    if (FileExists(sPath)) DeleteFile(sPath) ;
    hFile = FileCreate(sPath.c_str());
    if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }

    //AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'시 'n'분' s'초 \r\n'");
    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return true ;
}

bool CVisionCom::SaveVisnCommand(AnsiString _sCommand)
{
    //Local Var.
    int        hFile       ;
    AnsiString sPath       ;
    AnsiString sData       ;

    //Set Path.
    sData = _sCommand;


    //폴더 뚫기 위해.
    sPath = VISN_FOLDER ;
    if (!DirectoryExists(sPath)) CreateDir(sPath);
    sPath += String("\\Visn") + (m_iVisnId==0 ? "L" : "R") + "Command.dat" ;

    //원래 있던파일 지우고.
    if (FileExists(sPath)) DeleteFile(sPath) ;
    hFile = FileCreate(sPath.c_str());
    if (hFile == -1) { Trace("Err",(sPath + "is Can't made").c_str());  return false ; }

    //AnsiString sDate = Now().CurrentDateTime().FormatString("AM/PM h'시 'n'분' s'초 \r\n'");
    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return true ;
}

bool CVisionCom::LoadAlignRslt()
{
    //Align
    //비전이 쓰고 장비가 읽는다.
    //"c:\\Control\\VisnLResultAlign.dat“파일에
    //메인검사 카메라센터오프셑X,Y 서브검사 카메라센터오프셑 x,y 를 쓰고 구분자는’,’로한다.
    //Ex) X3.001,Y1.002,x1.032,y0.102

    //Local Var.
    int    hFile ;
    String sPath ;
    String sData ;
    String sTemp ;


    //폴더 뚫기 위해.
    sPath = VISN_FOLDER ;
    if (!DirectoryExists(sPath)) {
        CreateDir(sPath);
        Trace("Err",(sPath + " no Folder").c_str());
        return false ;
    }

    sPath += String("\\Visn") + (m_iVisnId==0 ? "L" : "R") + "ResultAlign.dat" ;
    if(!FileExists(sPath)) {
        Trace("Err",(sPath + " no File").c_str());
        return false ;
    }

    int iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    int iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    char * pData = new char[iFileLength+1];
    memset(pData , 0 , sizeof(char)*(iFileLength+1));

    //파일읽기.
    FileRead (iFileHandle, pData, iFileLength);
    sData = pData ;
    delete [] pData;
    FileClose(iFileHandle);

    //sData == "X3.001,Y1.002,x1.032,y0.102";
    memset(&m_tAlign , 0 , sizeof(TAlignResult));

    //메인 X값.
    if(sData.Pos("X")!=1){
        Trace("Err",(sPath + " no Data X").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData.SubString(1,sData.Pos(",")-1);
    sData.Delete(1,sData.Pos(","));
    m_tAlign.dMainX = StrToFloatDef(sTemp , 0.0);

    //메인 Y값.
    if(!sData.Pos("Y")){
        Trace("Err",(sPath + " no Data Y").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData.SubString(1,sData.Pos(",")-1);
    sData.Delete(1,sData.Pos(","));
    m_tAlign.dMainY = StrToFloatDef(sTemp , 0.0);

    //서브 X값.
    if(!sData.Pos("x")){
        Trace("Err",(sPath + " no Data x").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData.SubString(1,sData.Pos(",")-1);
    sData.Delete(1,sData.Pos(","));
    m_tAlign.dSubX = StrToFloatDef(sTemp , 0.0);

    //서브 Y값.
    if(!sData.Pos("y")){
        Trace("Err",(sPath + " no Data y").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData;
    sData.Delete(1,sData.Pos(","));
    m_tAlign.dSubY = StrToFloatDef(sTemp , 0.0);

    return true ;
}

void CVisionCom::DeleteAlignRslt()
{
    //폴더 뚫기 위해.
    String sPath ;
    sPath = VISN_FOLDER ;
    if (!DirectoryExists(sPath)) CreateDir(sPath);
    sPath += String("\\Visn") + (m_iVisnId==0 ? "L" : "R") + "ResultAlign.dat" ;

    //원래 있던파일 지우고.
    if (FileExists(sPath)) DeleteFile(sPath) ;
}


bool CVisionCom::LoadGapRslt()
{
    //Gap
    //비젼이 쓰고 장비가 읽는다.
    //"c:\\Control\\VisnLResultGap.dat“파일에
    //해당검사의 MainGap,SubGap을 기입한다.
    //Ex) G3.001,g0.234

    //Local Var.
    int    hFile ;
    String sPath ;
    String sData ;
    String sTemp ;

    //폴더 뚫기 위해.
    sPath = VISN_FOLDER ;
    if (!DirectoryExists(sPath)) {
        CreateDir(sPath);
        Trace("Err",(sPath + " no Folder").c_str());
        return false ;
    }

    sPath += String("\\Visn") + (m_iVisnId==0 ? "L" : "R") + "ResultGap.dat" ;
    if(!FileExists(sPath)) {
        Trace("Err",(sPath + " no File").c_str());
        return false ;
    }

    int iFileHandle = FileOpen(sPath.c_str(), fmOpenRead);
    int iFileLength = FileSeek(iFileHandle,0,2);
    FileSeek(iFileHandle,0,0);
    char * pData = new char[iFileLength+1];
    memset(pData , 0 , sizeof(char)*(iFileLength+1));

    //파일읽기.
    FileRead (iFileHandle, pData, iFileLength);
    sData = pData ;
    delete [] pData;
    FileClose(iFileHandle);

    //sData == "G3.001,g0.234";
    memset(&m_tGap , 0 , sizeof(TGapResult));

    //메인 X값.
    if(sData.Pos("G")!=1){
        Trace("Err",(sPath + " no Data G").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData.SubString(1,sData.Pos(",")-1);
    sData.Delete(1,sData.Pos(","));
    m_tGap.dMain = StrToFloatDef(sTemp , 0.0);

    //메인 Y값.
    if(!sData.Pos("g")){
        Trace("Err",(sPath + " no Data g").c_str());
        return false ;
    }
    sData.Delete(1,1);
    sTemp = sData;
    sData.Delete(1,sData.Pos(","));
    m_tGap.dSub = StrToFloatDef(sTemp , 0.0);

    return true ;

}

void CVisionCom::DeleteGapRslt()
{
    //폴더 뚫기 위해.
    String sPath ;
    sPath = VISN_FOLDER ;
    if (!DirectoryExists(sPath)) CreateDir(sPath);
    sPath += String("\\Visn") + (m_iVisnId==0 ? "L" : "R") + "ResultGap.dat" ;

    //원래 있던파일 지우고.
    if (FileExists(sPath)) DeleteFile(sPath) ;
}

//One Cycle.
bool CVisionCom::CycleInsp()
{

    //Check Cycle Time Out.
    String sTemp ;
    TCycle * Cycle = &VisnSendCycle[vsInsp] ;

    if (Cycle->tmDelay.OnDelay(Cycle->iStep == Cycle->iPreStep && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetDisp(true);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step=%02d" , __FUNC__ , Cycle->iStep );
        EM_SetErrMsg(eiVSN_ComErr , sTemp.c_str());
        Trace("Vision Communction",sTemp.c_str());

        IO_SetY(VIO.yVisn_InspStart , false) ;

        Cycle->iStep = 0 ;
        return true ;
    }

    if(Cycle->iStep != Cycle->iPreStep) {
        sTemp = sTemp.sprintf("%s Step=%02d" , __FUNC__ , Cycle->iStep );
        Trace("Vision Communction",sTemp.c_str());
    }

    Cycle->iPreStep = Cycle->iStep ;

    //Cycle.
    switch (Cycle->iStep) {

        default : sTemp = sTemp.sprintf("%s DEFAILT END STATUS : Step=%02d , PreStep=%02d" , __FUNC__ , Cycle->iStep , Cycle->iPreStep);
                  EM_SetErrMsg(eiVSN_ComErr , sTemp.c_str());
                  Trace("Vision Communction", sTemp.c_str());

                  IO_SetY(VIO.yVisn_InspStart , false) ;

                  Cycle->iStep = 0 ;
                  return true ;

        case  10: IO_SetY(VIO.yVisn_InspStart , false) ;
                  Cycle->iStep++;
                  return false ;

        case  11: if(IO_GetX(VIO.xVisn_Busy)) return false ;
                  IO_SetY(VIO.yVisn_InspStart , true) ;
                  Cycle->iStep++;
                  return false ;

        case  12: if(!IO_GetX(VIO.xVisn_Busy)) return false ;
                  IO_SetY(VIO.yVisn_InspStart , false ) ;
                  Cycle->iStep++;
                  return false ;

        case  13: if(IO_GetX(VIO.xVisn_Busy)) return false ;
                  //여기는 검사 결과값 가저오는것 넣어야 함.

                  m_bOk = IO_GetX(VIO.xVisn_InspOk);
                  //........
                  Cycle->iStep = 0 ;
                  return true;
    }
}

bool CVisionCom::CycleReset()
{

    //Check Cycle Time Out.
    String sTemp ;
    TCycle * Cycle = &VisnSendCycle[vsReset] ;

    if (Cycle->tmDelay.OnDelay(Cycle->iStep == Cycle->iPreStep && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetDisp(true);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step=%02d" , __FUNC__ , Cycle->iStep );
        EM_SetErrMsg(eiVSN_ComErr , sTemp.c_str());
        Trace("Vision Communction",sTemp.c_str());

        IO_SetY(VIO.yVisn_Reset , false) ;

        Cycle->iStep = 0 ;
        return true ;
    }

    if(Cycle->iStep != Cycle->iPreStep) {
        sTemp = sTemp.sprintf("%s Step=%02d" , __FUNC__ , Cycle->iStep );
        Trace("Vision Communction",sTemp.c_str());
    }

    Cycle->iPreStep = Cycle->iStep ;

    //Cycle.
    switch (Cycle->iStep) {

        default : sTemp = sTemp.sprintf("%s DEFAILT END STATUS : Step=%02d , PreStep=%02d" , __FUNC__ , Cycle->iStep , Cycle->iPreStep);
                  EM_SetErrMsg(eiVSN_ComErr , sTemp.c_str());
                  Trace("Vision Communction", sTemp.c_str());

                  IO_SetY(VIO.yVisn_Reset , false) ;

                  Cycle->iStep = 0 ;
                  return true ;

        case  10: IO_SetY(VIO.yVisn_Reset , false) ;
                  Cycle->iResetCnt = 0 ;
                  Cycle->iStep++;
                  return false ;

        case  11:
                  Cycle->iStep++;
                  return false ;

        case  12: IO_SetY(VIO.yVisn_Reset , true) ;
                  Cycle->tmDelay.Clear();
                  Cycle->iStep++;
                  return false ;

        case  13:
                  if(Cycle->tmDelay.OnDelay(true , 3500)){
                      IO_SetY(VIO.yVisn_Reset , false) ;
                  }
                  if(Cycle->tmDelay.OnDelay(true , 4000)){
                      sTemp = sTemp.sprintf("%s VISION RESET RETRY Step=%02d" , __FUNC__ , Cycle->iStep );
                      Trace("Vision Communction",sTemp.c_str());

                      Cycle->iStep = 11 ;
                      Cycle->iResetCnt++;
                  }

                  if(IO_GetX(VIO.xVisn_Busy)) return false ;

                  IO_SetY(VIO.yVisn_Reset , false ) ;
                  Cycle->iStep = 0 ;
                  return true;
    }

}

bool CVisionCom::CycleCommand()
{
    //Check Cycle Time Out.
    String sTemp ;
    TCycle * Cycle = &VisnSendCycle[vsCommand] ;

    if (Cycle->tmDelay.OnDelay(Cycle->iStep == Cycle->iPreStep && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetDisp(true);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step=%02d" , __FUNC__ , Cycle->iStep );
        EM_SetErrMsg(eiVSN_ComErr , sTemp.c_str());
        Trace("Vision Communction",sTemp.c_str());

        IO_SetY(VIO.yVisn_Command , false) ;

        Cycle->iStep = 0 ;
        return true ;
    }

    if(Cycle->iStep != Cycle->iPreStep) {
        sTemp = sTemp.sprintf("%s Step=%02d" , __FUNC__ , Cycle->iStep );
        Trace("Vision Communction",sTemp.c_str());
    }

    Cycle->iPreStep = Cycle->iStep ;

    //Cycle.
    switch (Cycle->iStep) {

        default : sTemp = sTemp.sprintf("%s DEFAILT END STATUS : Step=%02d , PreStep=%02d" , __FUNC__ , Cycle->iStep , Cycle->iPreStep);
                  EM_SetErrMsg(eiVSN_ComErr , sTemp.c_str());
                  Trace("Vision Communction", sTemp.c_str());

                  IO_SetY(VIO.yVisn_Command , false) ;

                  Cycle->iStep = 0 ;
                  return true ;

        case  10: IO_SetY(VIO.yVisn_Command , false) ;
                  //커멘드 쓴다.
                  //......
                  Cycle->iStep++;
                  return false ;

        case  11: if(IO_GetX(VIO.xVisn_Busy)) return false ;
                  IO_SetY(VIO.yVisn_Command , true) ;
                  Cycle->iStep++;
                  return false ;

        case  12: if(!IO_GetX(VIO.xVisn_Busy)) return false ;

                  IO_SetY(VIO.yVisn_Command , false ) ;
                  Cycle->iStep++;
                  return false ;

        case  13: if(IO_GetX(VIO.xVisn_Busy)) return false ;

                  Cycle->iStep = 0 ;
                  return true;
    }
}

bool CVisionCom::CycleJobChange()
{
    //Check Cycle Time Out.
    String sTemp ;
    TCycle * Cycle = &VisnSendCycle[vsJobChange] ;

    if (Cycle->tmDelay.OnDelay(Cycle->iStep == Cycle->iPreStep && !OM.MstOptn.bDebugMode , 15000 )) {
        EM_SetDisp(true);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step=%02d" , __FUNC__ , Cycle->iStep );
        EM_SetErrMsg(eiVSN_ComErr , sTemp.c_str());
        Trace("Vision Communction",sTemp.c_str());

        IO_SetY(VIO.yVisn_JobChange , false) ;

        Cycle->iStep = 0 ;
        return true ;
    }

    if(Cycle->iStep != Cycle->iPreStep) {
        sTemp = sTemp.sprintf("%s Step=%02d" , __FUNC__ , Cycle->iStep );
        Trace("Vision Communction",sTemp.c_str());
    }

    Cycle->iPreStep = Cycle->iStep ;

    //Cycle.
    switch (Cycle->iStep) {

        default : sTemp = sTemp.sprintf("%s DEFAILT END STATUS : Step=%02d , PreStep=%02d" , __FUNC__ , Cycle->iStep , Cycle->iPreStep);
                  EM_SetErrMsg(eiVSN_ComErr , sTemp.c_str());
                  Trace("Vision Communction", sTemp.c_str());

                  IO_SetY(VIO.yVisn_JobChange , false) ;

                  Cycle->iStep = 0 ;
                  return true ;

        case  10: IO_SetY(VIO.yVisn_JobChange , false) ;
                  //잡파일 쓰기.
                  //......
                  Cycle->iStep++;
                  return false ;

        case  11: if(IO_GetX(VIO.xVisn_Busy)) return false ;
                  IO_SetY(VIO.yVisn_JobChange , true) ;
                  Cycle->iStep++;
                  return false ;

        case  12: if(!IO_GetX(VIO.xVisn_Busy)) return false ;

                  IO_SetY(VIO.yVisn_JobChange , false ) ;
                  Cycle->iStep++;
                  return false ;

        case  13: if(IO_GetX(VIO.xVisn_Busy)) return false ;

                  Cycle->iStep = 0 ;
                  return true;
    }
}

void CVisionCom::Update()
{
    if(VisnSendCycle[vsInsp     ].iStep != 0) CycleInsp     () ;
    if(VisnSendCycle[vsReset    ].iStep != 0) CycleReset    () ;
    if(VisnSendCycle[vsCommand  ].iStep != 0) CycleCommand  () ;
    if(VisnSendCycle[vsJobChange].iStep != 0) CycleJobChange() ;
}

bool CVisionCom::SendInsp()
{
    if(!IO_GetX(VIO.xVisn_Ready)) return false;
    VisnSendCycle[vsInsp].iStep = 10 ;
    return true ;
}

bool CVisionCom::SendReset()
{
    if(!IO_GetX(VIO.xVisn_Ready)) return false;
    VisnSendCycle[vsReset].iStep = 10 ;
    return true ;
}

bool CVisionCom::SendCommand(String _sCommand)
{
    //여기에 커멘드 쓰는 것 넣기.
    if(!IO_GetX(VIO.xVisn_Ready)) return false;
    SaveVisnCommand(_sCommand);
    VisnSendCycle[vsCommand].iStep = 10 ;
    return true ;

}

bool CVisionCom::SendJobChange(String _sJobName)
{
    //if(!IO_GetX(VIO.xVisn_Ready)) return false;
    SaveVisnJobFile(_sJobName);
    VisnSendCycle[vsJobChange].iStep = 10 ;
    return true ;
}


bool CVisionCom::GetSendInspEnd()
{
    return VisnSendCycle[vsInsp].iStep == 0 ;
}

bool CVisionCom::GetSendResetEnd()
{
    return VisnSendCycle[vsReset].iStep == 0 ;
}
bool CVisionCom::GetSendCommandEnd()
{
    return VisnSendCycle[vsCommand].iStep == 0 ;
}
bool CVisionCom::GetSendJobChangeEnd()
{
    return VisnSendCycle[vsJobChange].iStep == 0 ;
}

bool CVisionCom::GetInspOk()
{
    return m_bOk ;
}

TAlignResult CVisionCom::GetAlignRslt()
{
    return m_tAlign ;
}

TGapResult CVisionCom::GetGapRslt()
{
    return m_tGap ;
}










