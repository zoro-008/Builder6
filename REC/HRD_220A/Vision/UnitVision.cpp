//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitVision.h"
#include "PkgMatch.h"
#include "PkgThreshold.h"
#include "PkgResult.h"
#include "GlobalMan.h"
#include "SLogUnit.h"
#include "UserFile.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall CThreadInspect::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

/*
__fastcall CThreadInspect::CThreadInspect(CVision * _pVision , bool CreateSuspended)
        : TThread(CreateSuspended)
{
    m_pVision = _pVision ;
}*/
//---------------------------------------------------------------------------
void __fastcall CThreadInspect::Execute()
{
        //---- Place thread code here ----

    while(!Terminated)
    {

        String sTemp = String(m_pVision -> m_iVisnID+4) + "," + m_pVision -> GetName();
        Trace("<@SETBUFF>",(sTemp + "_1").c_str());
        m_pVision -> CheckGrab();
        Trace("<@SETBUFF>",(sTemp + "_2").c_str());
        m_pVision -> CheckInsp();
        Trace("<@SETBUFF>",(sTemp + "_3").c_str());
        m_pVision -> CheckGrabInsp();
        Trace("<@SETBUFF>",(sTemp + "_End").c_str());
        m_pVision -> CheckTrainInsp();
        Sleep(1);
    }
}
//---------------------------------------------------------------------------

int CVision::m_iLastInspId = 0 ;

bool CVision::Init()
{
    //��Ȳ ���� �ּ�ó������.
    TUserINI Ini ;

    //if(!_bLoad)Ini.ClearFile(GM.GetJobFileFolder() ) ;

//    String sIniPath  = GM.GetGlobalFolder()+ "VisionProperty.ini" ;
//    Ini.Load(sIniPath, "Total" , "Count" , iCnt);
    static int iVisnCnt = 0 ;

    m_iVisnID = iVisnCnt ;
    iVisnCnt++;


    int iCnt ;

    String sName    ;
    String sComment ;

    CPkgBase * Pkg ;

/*
    if(_bLoad) {
        //���� �ִ���� �޸𸮿� ������  ����.
        DeleteAll();
        //������������ �ִ� ���� �ε� �ߴٰ� ���� ���� �ε� �ϸ� ���⼭ ����...m_pActivePkg = NULL ;
        //SVision���� �ε� �ϸ� PKG 0���� ������ NULLó��.
        Ini.Load(sIniPath, "Total" , "Count" , iCnt);
            Ini.Load(sIniPath, String(i) , "sName   ", sName    );
            Ini.Load(sIniPath, String(i) , "sComment", sComment );

            Add(sName ,sComment);
        }
EnterCriticalSection(&m_csPkgList);
        for(int i=0 ; i <m_lPkg.GetDataCnt() ; i++){
            Pkg = m_lPkg.GetCrntData(!i);
            Pkg -> LoadSave(_bLoad, m_sName);
        }
*/



    InitializeCriticalSection(&m_csPkgList);

    m_pThreadInspect = new CThreadInspect(this , false);
    m_pActivePkg = NULL ;
    m_pCamPkg    = NULL ;

//    m_asPkgNameTable[pkCamera_V01    ] = "Camera_V01"     ;
//    m_asPkgNameTable[pkMatch_V01     ] = "Match_V01"      ;
//    m_asPkgNameTable[pkThreshold_V01 ] = "Threshold_V01"  ;

    m_pValueList = new CValueList();
    m_pValueList -> Init() ;
    m_pValueList -> SetName("DeviceValue");
    m_pValueList -> SetComment("Device Pkg Rslt Communication");

    m_sPkgID = "" ;


    return true ;
}
bool CVision::Close()
{

    delete m_pThreadInspect ;
    m_pThreadInspect = NULL ;


    DeleteAll();
    m_pValueList -> Close();
    delete m_pValueList ;
    m_pValueList = NULL ;

    DeleteCriticalSection(&m_csPkgList);




    return false ;
}

void CVision::CheckReady()
{
    double dSttTime ;
    if(Rqst.bReady) {
        dSttTime =  GetTime();
        Stat.bGrabbing = true  ;
        Rqst.bGrab     = false ;


        CPkgBase * pPkg ;

        if(m_pCamPkg)m_pCamPkg ->Ready(); //ī�޶� ������ �̹��� ���ö����� Ȧ�� �ȴ�.
        else         ShowMessage("There is no Camera Pkg!");

        Stat.bGrabbing = false ;
        SendPaintMsg();
        Time.iGrab = GetTime() - dSttTime ;
    }
}

void CVision::CheckGrab()
{
    double dSttTime ;
    if(Rqst.bGrab) {
        dSttTime =  GetTime();
        Stat.bGrabbing = true  ;
        Rqst.bGrab     = false ;


        CPkgBase * pPkg ;

        //����� ���� Ŭ���� �ϰ�.
Trace("<@SETBUFF>","10,CriticalSection Enter Stt");
EnterCriticalSection(&m_csPkgList);
Trace("<@SETBUFF>","10,CriticalSection Enter End");

        int iCnt = m_lPkg.GetDataCnt() ;
        for(int i = 0 ; i <iCnt ; i++) {
            pPkg = m_lPkg.GetCrntData(!i);
            pPkg -> RsltClear();
        }
Trace("<@SETBUFF>","10,CriticalSection Leave Stt");
LeaveCriticalSection(&m_csPkgList);
Trace("<@SETBUFF>","10,CriticalSection Leave End");
        if(m_pCamPkg){
            if(!m_pCamPkg -> Run(NULL)){
                Trace("Camera","CamPkg Run Fail!");
            }
        }
        else ShowMessage("There is no Camera Pkg!");

        Stat.bGrabbing = false ;
        SendPaintMsg();
        Time.iGrab = GetTime() - dSttTime ;
    }
}
void CVision::CheckInsp()
{
    double dSttTime ;
    if(Rqst.bInsp){
        dSttTime =  GetTime();
        Stat.bInspectionning = true ;
        Rqst.bInsp = false ;



        CImage * pImg ;
        CPkgBase * pPkg ;

EnterCriticalSection(&m_csPkgList);

        //����� ���� Ŭ���� �ϰ�.
        for(int i = 0 ; i <m_lPkg.GetDataCnt() ; i++) {
            pPkg = m_lPkg.GetCrntData(!i);
            pPkg -> RsltClear();
        }

        //�˻�..
        for(int i = 0 ; i <m_lPkg.GetDataCnt() ; i++) {
            pPkg = m_lPkg.GetCrntData(!i);
            if(pPkg -> GetProperty().bCamera) pImg = ((CCamera_V01 *)pPkg) -> GetCrntImg();
            else                              pPkg -> Run(pImg);
        }

//20151108 �̹��� ����.
        bool bRet = true ;
        int  iSaveOptn ;
        for(int i = 0 ; i <m_lPkg.GetDataCnt() ; i++) {
            pPkg = m_lPkg.GetCrntData(!i);
            if(pPkg -> GetProperty().bCamera){
                iSaveOptn = ((CCamera_V01 *)pPkg) -> GetMPara() -> iSaveInspImg ;
            }
            else {
                if(!pPkg -> GetRslt()) {bRet = false ;}
            }
        }

        //NotUse,NgOnly,OkOnly,All
        TDateTime CrntTime = Now();
        String sFile = "D:\\InspImg\\";

        if(!DirectoryExists(sFile)) {
            ForceDirectories(sFile);
        }
        UserFile.ClearDirDate(sFile , Now() - 30 ); //30��.

        sFile+=CrntTime.FormatString("mmdd_hhnnsszzz_") ;

        if(m_sName != ""){
            sFile+= m_sName ;
            sFile+= "_" ;
        }

        if(m_sPkgID != ""){
            sFile+= m_sPkgID ;
            sFile+= "_" ;
        }

        sFile = sFile + (bRet?"OK":"NG") ;
        if(iSaveOptn == 0) { //������.
        }
        else if(iSaveOptn == 1) {//Ng��
            if(!bRet) {
                pImg -> SaveBmp(sFile.c_str());
            }
        }
        else if(iSaveOptn == 2) {//Ok��
            if(bRet) {
                pImg -> SaveBmp(sFile.c_str());
            }

        }
        else if(iSaveOptn == 3) {//All
            pImg -> SaveBmp(sFile.c_str());
        }
LeaveCriticalSection(&m_csPkgList);
        Stat.bInspectionning = false ;

        SendPaintMsg();

        Time.iInsp = GetTime() - dSttTime ;

        m_iLastInspId = m_iVisnID ;
    }
}
void CVision::CheckGrabInsp()
{
    double dSttTime ;
    if(Rqst.bGrabInsp) {
        dSttTime =  GetTime();
        Rqst.bGrabInsp = false ;

        Stat.bGrabbing = true  ;

        CPkgBase * pPkg ;
        CImage * pImg ;

EnterCriticalSection(&m_csPkgList);
        //����� ���� Ŭ���� �ϰ�.
        for(int i = 0 ; i <m_lPkg.GetDataCnt() ; i++) {
            pPkg = m_lPkg.GetCrntData(!i);
            pPkg -> RsltClear();
        }
LeaveCriticalSection(&m_csPkgList);

        //���� ���.

        if(m_pCamPkg)m_pCamPkg -> Run(NULL); //ī�޶�� �̹��� ������ �ѱ� �ʿ� ����. //ī�޶� ������ �̹��� ���ö����� Ȧ�� �ȴ�.
        else         ShowMessage("There is no Camera Pkg!");

        SendPaintMsg();
        Time.iGrab = GetTime() - dSttTime ;
        dSttTime =  GetTime();


        //�˻�..
        Stat.bInspectionning = true ;//=========================================
        Stat.bGrabbing = false ;
EnterCriticalSection(&m_csPkgList);
        if(m_pCamPkg){
            for(int i = 0 ; i <m_lPkg.GetDataCnt() ; i++) {
                pPkg = m_lPkg.GetCrntData(!i);
                if(pPkg -> GetProperty().bCamera) {
                    pImg = ((CCamera_V01 *)pPkg) -> GetCrntImg(); //ī�޶� ��Ű������ �̹��� �޾Ƽ�.
                }
                else {
                    pPkg -> Run(pImg); //�˻� ����.
                }
            }
        }

        //20151108 �̹��� ����.
        bool bRet = true ;
        int  iSaveOptn ;
        for(int i = 0 ; i <m_lPkg.GetDataCnt() ; i++) {
            pPkg = m_lPkg.GetCrntData(!i);
            if(pPkg -> GetProperty().bCamera){
                iSaveOptn = ((CCamera_V01 *)pPkg) -> GetMPara() -> iSaveInspImg ;
            }
            else {
                if(!pPkg -> GetRslt()) {bRet = false ;}
            }
        }

        //NotUse,NgOnly,OkOnly,All
        TDateTime CrntTime = Now();
        String sFile = "D:\\InspImg\\";

        if(!DirectoryExists(sFile)) {
            ForceDirectories(sFile);
        }
        UserFile.ClearDirDate(sFile , Now() - 30 ); //30��.

        sFile+=CrntTime.FormatString("mmdd_hhnnsszzz_") ;

        if(m_sName != ""){
            sFile+= m_sName ;
            sFile+= "_" ;
        }

        if(m_sPkgID != ""){
            sFile+= m_sPkgID ;
            sFile+= "_" ;
        }

        sFile = sFile + (bRet?"OK":"NG") ;
        if(iSaveOptn == 0) { //������.
        }
        else if(iSaveOptn == 1) {//Ng��
            if(!bRet) {
                pImg -> SaveBmp(sFile.c_str());
            }
        }
        else if(iSaveOptn == 2) {//Ok��
            if(bRet) {
                pImg -> SaveBmp(sFile.c_str());
            }

        }
        else if(iSaveOptn == 3) {//All
            pImg -> SaveBmp(sFile.c_str());
        }

LeaveCriticalSection(&m_csPkgList);
        Stat.bInspectionning = false ;//========================================

        SendPaintMsg();

        Time.iInsp = GetTime() - dSttTime ;

        m_iLastInspId = m_iVisnID ;


    }
}

void CVision::CheckTrainInsp()
{
    double dSttTime ;
    if(Rqst.bTrainInsp){
        dSttTime =  GetTime();
        Stat.bInspectionning = true ;
        Rqst.bTrainInsp = false ;

        CImage   * pImg = NULL;
        CPkgBase * pPkg ;

        //Ʈ���� �̹��� ��������.
        pImg = GetTrainImg();

        //����� ���� Ŭ���� �ϰ�.
        m_pActivePkg -> RsltClear();
        m_pActivePkg -> Run(pImg);

        Stat.bInspectionning = false ;

        SendPaintMsg();

        Time.iInsp = GetTime() - dSttTime ;
    }
}



//ȭ�� ���.
void CVision::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY )
{
     if(m_pActivePkg==NULL) return ;
     m_pActivePkg-> PaintRslt(_hDc , _dScaleX , _dScaleY ,_dOffsetX , _dOffsetY);
}
void CVision::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
     if(m_pActivePkg==NULL) return ;
     m_pActivePkg-> PaintTracker(_hDc , _dScaleX , _dScaleY ,_dOffsetX , _dOffsetY);
}

//�̹��� ȭ�� Ŭ�� �̺�Ʈ
bool CVision::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
     if(m_pActivePkg==NULL) return false ;
     return m_pActivePkg->MouseDw(Button,Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY);
}
bool CVision::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
     if(m_pActivePkg==NULL) return false;
     return m_pActivePkg->MouseMv(Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY);
}
void CVision::MouseUp(TMouseButton Button,TShiftState Shift )
{
     if(m_pActivePkg==NULL) return ;
     m_pActivePkg->MouseUp(Button, Shift );
}

//�Ķ���� ���÷���
void CVision::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
{
     if(m_pActivePkg==NULL) return ;
     m_pActivePkg->UpdateMasterPara(_bToTable , _vePara);
}
void CVision::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
{
     if(m_pActivePkg==NULL) return ;
     m_pActivePkg->UpdateCommonPara(_bToTable , _vePara);
}
void CVision::UpdateLocalPara(bool _bToTable , TValueListEditor * _vePara)
{
     if(m_pActivePkg==NULL) return ;
     m_pActivePkg->UpdateLocalPara(_bToTable , _vePara);
}


void CVision::SetSendMsgWnd(void * _hWnd)
{
    m_hSendMsgWnd = _hWnd;
}

/*
bool CVision::Add(String _sName,String _sPkgName )  //�˻翡 ���� Ŭ����PKG �̸�.
{
    CPkgBase * Pkg ;
    bool bRet = false ;
    if(_sPkgName == m_asPkgNameTable[pkCamera_V01]){
        Pkg = new CCamera_V01();
        Pkg -> Init();
        Pkg -> SetComment(m_asPkgNameTable[pkCamera_V01]) ;
        Pkg -> SetName(_sName) ;
        Pkg -> SetValueList(m_pValueList);
EnterCriticalSection(&m_csPkgList);
        bRet = m_lPkg.PushBack(Pkg);
LeaveCriticalSection(&m_csPkgList);

    }
    else if(_sPkgName == m_asPkgNameTable[pkMatch_V01 ]){
        Pkg = new CMatch_V01 ();
        Pkg -> Init();
        Pkg -> SetComment(m_asPkgNameTable[pkMatch_V01]) ;
        Pkg -> SetName(_sName) ;
        Pkg -> SetValueList(m_pValueList);
EnterCriticalSection(&m_csPkgList);
        bRet = m_lPkg.PushBack(Pkg);
LeaveCriticalSection(&m_csPkgList);
    }
    else if(_sPkgName == m_asPkgNameTable[pkThreshold_V01 ]){
        Pkg = new CThreshold_V01 ();
        Pkg -> Init();
        Pkg -> SetComment(m_asPkgNameTable[pkThreshold_V01]) ;
        Pkg -> SetName(_sName) ;
        Pkg -> SetValueList(m_pValueList);
EnterCriticalSection(&m_csPkgList);
        bRet = m_lPkg.PushBack(Pkg);
LeaveCriticalSection(&m_csPkgList);
    }

    return bRet ;
} */

bool CVision::Add(String _sName,String _sPkgName )  //�˻翡 ���� Ŭ����PKG �̸�.
{
    CPkgBase * Pkg ;
    bool bRet ;

    CStaticPkgLink * pPkgLink = CStaticPkgLink::GetStaticPkgLink(_sPkgName.c_str());
    if(pPkgLink) {
        Pkg = pPkgLink->CreatePkg();
        Pkg -> Init();
        Pkg -> SetComment(_sPkgName) ;
        Pkg -> SetName(_sName) ;
        Pkg -> SetValueList(m_pValueList);  //�������λ����� �ߴµ� ��������Ʈ �׷��� �ϸ� �ȵǳ�... �������� ���ΰ������ߵǼ�.
        Pkg -> SetOnPaint(SendPaintMsg);
EnterCriticalSection(&m_csPkgList);
        bRet = m_lPkg.PushBack(Pkg);
LeaveCriticalSection(&m_csPkgList);
    }


    return bRet ;
}


bool CVision::MoveUp(int _iIdx)
{
    bool bRet ;
EnterCriticalSection(&m_csPkgList);
    bRet = m_lPkg.Move(_iIdx , _iIdx -1);
LeaveCriticalSection(&m_csPkgList);
    return bRet ;
}

bool CVision::MoveDown(int _iIdx)
{
    bool bRet ;
EnterCriticalSection(&m_csPkgList);
    bRet = m_lPkg.Move(_iIdx , _iIdx +1);
LeaveCriticalSection(&m_csPkgList);
    return bRet ;
}

bool CVision::Insert(int _iIdx , String _sName , String _sPkgName ) //�˻翡 ���� Ŭ����PKG �̸�.
{
    CPkgBase * Pkg ;
    bool bRet ;

    CStaticPkgLink * pPkgLink = CStaticPkgLink::GetStaticPkgLink(_sPkgName.c_str());
    Pkg = pPkgLink->CreatePkg();
    Pkg -> Init();
    Pkg -> SetComment(_sPkgName) ;
    Pkg -> SetName(_sName) ;
    Pkg -> SetValueList(m_pValueList);
    Pkg -> SetOnPaint(SendPaintMsg);
EnterCriticalSection(&m_csPkgList);
    bRet =  m_lPkg.Insert(Pkg , _iIdx);
LeaveCriticalSection(&m_csPkgList);

    return bRet ;
}

bool CVision::Delete(int _iIdx )
{
    bool bRet = false ;
    CPkgBase * Pkg = m_lPkg.GetData(_iIdx) ;
    if(Pkg!=NULL) {
        Pkg -> Close();  //�Ҹ��� vitual�̱⿡ �׳� �̷��� �ص���.
        delete Pkg ;
    }
EnterCriticalSection(&m_csPkgList);
    bRet =  m_lPkg.Delete(_iIdx);
LeaveCriticalSection(&m_csPkgList);
    return bRet ;
}

bool CVision::DeleteAll()
{
    CPkgBase * Pkg = NULL ;
    bool bRet = false ;
EnterCriticalSection(&m_csPkgList);
    for(int i = 0 ; i < m_lPkg.GetDataCnt() ; i++) {
        Pkg = m_lPkg.GetCrntData(!i);
        if(Pkg!=NULL) Pkg -> Close();
        delete Pkg ;
    }
    bRet =  m_lPkg.DeleteAll();
LeaveCriticalSection(&m_csPkgList);
    return bRet ;
}

void CVision::UpdateList(bool _bToTable , TStringGrid * _veGrid)
{
    if(_bToTable) {
        _veGrid -> ColCount = 4 ;

        _veGrid -> Cells[0][0] = "NO"   ;
        _veGrid -> Cells[1][0] = "SKIP" ;
        _veGrid -> Cells[2][0] = "NAME" ;
        _veGrid -> Cells[3][0] = "TYPE" ;
EnterCriticalSection(&m_csPkgList);
        _veGrid -> RowCount = m_lPkg.GetDataCnt() + 1 < 2 ? 2 : m_lPkg.GetDataCnt() + 1;
        CPkgBase * Pkg ;
        for (int i = 0 ;i < m_lPkg.GetDataCnt() ; i++) {
            Pkg = m_lPkg.GetCrntData(!i) ;

            _veGrid -> Cells[0][i+1] = i ;
            _veGrid -> Cells[1][i+1] = Pkg->GetSkip() ? "SKIP" : "" ;
            _veGrid -> Cells[2][i+1] = Pkg->GetName()   ;
            _veGrid -> Cells[3][i+1] = Pkg->GetComment();
        }
LeaveCriticalSection(&m_csPkgList);
    }
    else {
        //�޸𸮿� ���°��� ���⼭ ���� �ʴ´�...
    }

}

void CVision::LoadSave(bool _bLoad , String _sJobFileName)
{
    //��Ȳ ���� �ּ�ó������.
    TUserINI Ini ;

    //if(!_bLoad)Ini.ClearFile(GM.GetJobFileFolder() ) ;

    String sIniPath  = GM.GetJobFileFolder() + m_sName + ".ini" ;


    int iCnt ;

    String sName    ;
    String sComment ;

    CPkgBase * Pkg ;

    if(_bLoad) {
        //���� �ִ���� �޸𸮿� ������  ����.
        DeleteAll();
        //������������ �ִ� ���� �ε� �ߴٰ� ���� ���� �ε� �ϸ� ���⼭ ����...m_pActivePkg = NULL ;
        //SVision���� �ε� �ϸ� PKG 0���� ������ NULLó��.
        Ini.Load(sIniPath, "Total" , "Count" , iCnt);
        for(int i=0; i < iCnt ; i++) {

            Ini.Load(sIniPath, String(i) , "sName   ", sName    );
            Ini.Load(sIniPath, String(i) , "sComment", sComment );

            Add(sName ,sComment);
        }
EnterCriticalSection(&m_csPkgList);
        for(int i=0 ; i <m_lPkg.GetDataCnt() ; i++){
            Pkg = m_lPkg.GetCrntData(!i);
            Pkg -> LoadSave(_bLoad, m_sName);
        }
LeaveCriticalSection(&m_csPkgList);
    }
    else {
EnterCriticalSection(&m_csPkgList);
        Ini.Save(sIniPath, "Total" , "Count" , m_lPkg.GetDataCnt() );
        for(int i=0; i < m_lPkg.GetDataCnt()  ; i++) {

            Pkg = m_lPkg.GetCrntData(!i) ;
            sName   =Pkg->GetName   ();Ini.Save(sIniPath, String(i) , "sName   ", sName    );
            sComment=Pkg->GetComment();Ini.Save(sIniPath, String(i) , "sComment", sComment );

            Pkg -> LoadSave(_bLoad, m_sName);
        }
LeaveCriticalSection(&m_csPkgList);
    }

    //���θ��� ����Ʈ���� ī�޶�PKG�� �˾Ƶΰ� ī�޶�PKg�� ������ �޼���.
    bool bExistCam = false ;
    CPkgBase * pPkg ;
    String     sCamPkgName = "";

EnterCriticalSection(&m_csPkgList);
    for(int i = 0 ; i <m_lPkg.GetDataCnt() ; i++) {
        pPkg = m_lPkg.GetCrntData(!i);
        if(pPkg-> GetProperty().bCamera){
            bExistCam = true ;
            m_pCamPkg = (CCamera_V01 *)pPkg ;//
        }
    }
LeaveCriticalSection(&m_csPkgList);
    if(!bExistCam){
        ShowMessage("Please Insert CameraPkg"); //ī�޶� ��Ű���� ������ �Է� �ϵ��� ��.....
        //m_pCamPkg = (CCamera_V01 *)m_lPkg.GetData(0);
        m_pCamPkg = NULL ;
    }

    m_pValueList -> LoadSave(_bLoad, m_sName);


    //Cal Setting.
    if(m_pCamPkg) {
        SetGetPxResol(m_pCamPkg -> GetCal() -> GetPxResol);
        SetGetCalPos(m_pCamPkg -> GetCal() -> GetCalPos );
    }




}

void CVision::LoadSave(bool _bLoad )
{
    LoadSave(_bLoad , GM.GetCrntJobFile());
}

void CVision::SetGetPxResol(FGetPxResol _fpGetPxResol)
{
    CPkgBase * pPkg ;
EnterCriticalSection(&m_csPkgList);
    for(int i=0; i < m_lPkg.GetDataCnt()  ; i++) {

        pPkg = m_lPkg.GetCrntData(!i) ;
        pPkg -> SetGetPxResol(_fpGetPxResol) ;
    }
LeaveCriticalSection(&m_csPkgList);
}

void CVision::SetGetCalPos(FGetCalPos _fpGetCalPos )
{
    CPkgBase * pPkg ;
EnterCriticalSection(&m_csPkgList);
    for(int i=0; i < m_lPkg.GetDataCnt()  ; i++) {

        pPkg = m_lPkg.GetCrntData(!i) ;

        pPkg -> SetGetCalPos(_fpGetCalPos) ;
    }
LeaveCriticalSection(&m_csPkgList);

}

void * CVision::GetSendMsgWnd()
{                                 
    return m_hSendMsgWnd ;
}

//ȭ�� ���.
void CVision::PaintRsltAll(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
     if(m_pActivePkg==NULL) return ;

     CPkgBase * pPkg ;
EnterCriticalSection(&m_csPkgList);
     int iCnt = m_lPkg.GetDataCnt() ;
     for(int i = 0 ; i <iCnt ; i++) {
         pPkg = m_lPkg.GetCrntData(!i);
         pPkg -> PaintRslt(_hDc , _dScaleX , _dScaleY ,_dOffsetX , _dOffsetY);
     }
LeaveCriticalSection(&m_csPkgList);
}

void CVision::Ready()
{
    Stat.bGrabbing = true  ; //������ ��縮 ����� �̸� ���������� �س��´�.
    Rqst.bReady = true ;

}

void CVision::Grab()
{
    Stat.bGrabbing = true  ; //������ ��縮 ����� �̸� ���������� �س��´�.
    Rqst.bGrab = true ;

}

void CVision::Insp()
{
    Stat.bInspectionning = true ; //������ ��縮 ����� �̸� ���������� �س��´�.
    Rqst.bInsp   = true ;

}

void CVision::GrabInsp()
{
    Stat.bGrabbing = true  ; //������ ��縮 ����� �̸� ���������� �س��´�.
    Rqst.bGrabInsp = true ;
}

void CVision::TrainInsp()
{
    Stat.bInspectionning = true ; //������ ��縮 ����� �̸� ���������� �س��´�.
    Rqst.bTrainInsp = true ;
}



bool CVision::GetGrabEnd()
{
    return !Stat.bGrabbing ;
}

bool CVision::GetInspEnd()
{
    return !Stat.bInspectionning ;

}

CImage * CVision::GetCrntImg()
{
    if(m_pCamPkg) {
        return m_pCamPkg -> GetCrntImg();
    }
    return NULL ;
}

CImage * CVision::GetTrainImg()
{
    if(m_pCamPkg) {
        return m_pCamPkg -> GetTrainImg();
    }
    return NULL ;


}

bool CVision::GetRslt()
{
    CPkgBase * pPkg ;
    bool bRet = true ;

EnterCriticalSection(&m_csPkgList);
    for(int i = 0 ; i <m_lPkg.GetDataCnt() ; i++) {
        pPkg = m_lPkg.GetCrntData(!i);
        if(!pPkg -> GetRslt()) {bRet = false ;}
    }
LeaveCriticalSection(&m_csPkgList);
    return bRet ;

}

String CVision::GetRsltMsg()
{
    CPkgBase * pPkg ;
    String sRet = "" ;

EnterCriticalSection(&m_csPkgList);
    for(int i = 0 ; i <m_lPkg.GetDataCnt() ; i++) {
        pPkg = m_lPkg.GetCrntData(!i);
        if(pPkg -> GetProperty().bResult) {
            sRet = ((CResult_V01 *)pPkg) -> GetRsltMsg() ;
        }
    }
LeaveCriticalSection(&m_csPkgList);
    return sRet ;

}

int CVision::GetPkgCnt() //�̰��� ũ��Ƽ�� ���� �ִ� ���� �Լ����� ��� �ϸ� �ȉ´�. ����� �ɸ�.
{
    int iPkgCnt ;
EnterCriticalSection(&m_csPkgList);
    iPkgCnt = m_lPkg.GetDataCnt() ;
LeaveCriticalSection(&m_csPkgList);
    return iPkgCnt ;
}





