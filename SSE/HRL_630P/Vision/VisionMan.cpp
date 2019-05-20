//---------------------------------------------------------------------------
#pragma hdrstop

#include "VisionMan.h"
#include "SVInterface.h"
#include "SLogUnit.h"
#include "UserFile.h"
#include "VOptnMan.h"

#include "CamUnit.h"
#include "LightUnit.h"

#include "FormAlgoPara.h"
#include "FormImage.h"
#include "FormVisnMain.h"
#include "FormJob.h"

#include "Package.h"

#include "FormPackage.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)
CVisionMan VSM ;

void __fastcall CVisionMan::tmYameTimer(TObject *Sender)
{
//    if(!FrmVisnMain || !FrmVisnMain -> Showing) return ;
    tmYame -> Enabled = false ;
    return  ;
/*
    AnsiString sBitmapPath ;
    static int i = 0 ;

    VISN[i]->GetImg() -> LoadBmp(GetExeDir("Util\\Images\\" + AnsiString(rand()%6)).c_str());
    VISN[i]->m_pPaintCallback();
    sBitmapPath = g_sJobFilePath + VISN[i]->GetVisnStaticPara().sVisnName + AnsiString(".bmp") ;
    VISN[i]->GetImg() -> LoadBmp(sBitmapPath.c_str());
    VISN[i]->m_pPaintCallback();
    if(g_iYameRet == 0) {tmYame -> Enabled = true ; return ;}

    if(i==MAX_VISN_ID-1){
        SetActiveVisn((EN_VISN_ID)(MAX_VISN_ID-1));
//        SetActiveVisn((EN_VISN_ID)0);
        return ;
    }
    i++;

    tmYame -> Enabled = true ;
    return ;*/

}



CVisionMan::CVisionMan()
{
    //Trace("","");
}



CVisionMan::~CVisionMan()
{
    static int i = 0 ;
    //Trace("",AnsiString(i).c_str());
    i++;
}

bool CVisionMan::InitVisn(void * hMainWnd , void * hParaWnd , AnsiString _sDevName)
{
    LoadSVL("pSVL");



    Cam  [ciHed] = new CImi() ; Cam  [ciHed]->Init();
    Light[liAll] = new CDkm() ; Light[liAll]->Init();

    
    TVisnStaticPara tPara ;
    
    VISN[viPkg] = new CPackage() ;
    tPara.iVisnId     = viPkg  ;
    tPara.sVisnName   = "Package";
    tPara.iUseCamId   = ciHed  ;
    tPara.iUseLightId = liAll  ;
    VISN[viPkg] -> Init(tPara) ;

    m_iInspectionId = viPkg ;
    
    LoadJobFile(true ,_sDevName) ;

    //폼 관련.
    RECT   rt ;
    //Application -> Handle = hMainWnd ;
    FrmVisnMain = new TFrmVisnMain(NULL);
    FrmVisnMain -> ParentWindow = hMainWnd ;
    GetClientRect(hMainWnd , &rt);
    FrmVisnMain -> Left = 0 ;
    FrmVisnMain -> Top  = 0 ;
    FrmVisnMain -> Width = rt.right  ;
    FrmVisnMain -> Height= rt.bottom ;
    FrmVisnMain -> Init();

    
    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        FrmImage[i] = new TFrmImage(FrmVisnMain);
        FrmImage[i] -> Init((EN_VISN_ID)i);
        FrmImage[i] -> Parent = FrmVisnMain -> tsVisn[i] ;
        FrmImage[i] -> Align = alClient ;
        //FrmImage[i] -> Left = 0 ;
        //FrmImage[i] -> Top  = 0 ;
        FrmImage[i] -> Show()   ;
    
    }
    
    FrmAlgoPara   = new TFrmAlgoPara(FrmVisnMain);
    FrmAlgoPara -> Init() ;
    
    FrmJob        = new TFrmJob     (FrmVisnMain);
    
    FrmPackage = new TFrmPackage(NULL);
    FrmPackage-> ParentWindow = hParaWnd ;
    GetClientRect(hParaWnd , &rt);
    FrmPackage-> Left = 0 ;
    FrmPackage-> Top  = 0 ;
    FrmPackage-> Width = rt.right  ;
    FrmPackage-> Height= rt.bottom ;
    FrmPackage-> Init();
    FrmPackage-> FormShow((EN_PAGE_ID)0);


    //LoadJobFile(true ,_sDevName) ;
    
    FrmVisnMain -> Show();
    
    tmYame = new TTimer(Application);
    tmYame -> Interval = 1 ;
    tmYame -> OnTimer = tmYameTimer ;
    tmYame -> Enabled = true ;

    g_iCrntLevel = olMaster ;

    return true ;
}

bool CVisionMan::DelVisn()
{


    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        VISN[i] -> Live(false);
    }
    Sleep(100); //혹시 그랩 터진직후면 이미지 들어오는시간 대기.
    
    delete tmYame ; tmYame = NULL ;
    
    

    FrmAlgoPara -> Close(); delete FrmAlgoPara   ; FrmAlgoPara   = NULL ;
    FrmJob      -> Close(); delete FrmJob        ; FrmJob        = NULL ;

    FrmPackage  -> Close(); delete FrmPackage    ; FrmPackage    = NULL ;

    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        FrmImage[i] -> Close();
        delete FrmImage[i] ; FrmImage[i] = NULL ;
    }
    
    FrmVisnMain -> Del();
    delete FrmVisnMain ; FrmVisnMain = NULL ;
    
    
    
    //소멸자 가상함수 선언 안하면 이렇게 써야 함.
    //delete ((CBfPlce  *)VISN[viBfPlce]) ; VISN[viBfPlce] = NULL ;
    //delete ((CAtPlce  *)VISN[viAtPlce]) ; VISN[viAtPlce] = NULL ;
    //delete ((CBfPick *)VISN[viBfPick]) ; VISN[viBfPick] = NULL ;
    //delete ((CAtPick *)VISN[viAtPick]) ; VISN[viAtPick] = NULL ;
    
    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        VISN[i] -> Close();
        delete VISN[i] ;
    }
    
    
    
    Cam[ciHed]->Close();
    delete ((CImi *)Cam[ciHed]) ; Cam[ciHed] = NULL ;
    
    Light[liAll]->Close();
    delete ((CDkm *)Light[liAll]) ; Light[liAll] = NULL ;


    FreeSVL() ;

    return true ;
}

void CVisionMan::SetImgForm(void * hImgBaseWnd , bool _bSettingMode)
{
    RECT   rt ;
    FrmVisnMain -> ParentWindow = hImgBaseWnd ;
    GetClientRect(hImgBaseWnd , &rt);
    FrmVisnMain -> Left = 0 ;
    FrmVisnMain -> Top  = 0 ;
    FrmVisnMain -> Width = rt.right  ;
    FrmVisnMain -> Height= rt.bottom ;
    g_bSettingMode = _bSettingMode ; 

}

void CVisionMan::LoadJobFile(bool _bLoad , AnsiString _sDevName)
{
    g_sJobFileName = _sDevName ;
    g_sJobFilePath = GetExeDir("JobFile\\") + _sDevName + "\\Vision\\";

    //알고리즘 파라 로드.
    VOM.LoadJobFile(_bLoad);
}


void CVisionMan::Grab(EN_VISN_ID _iVisnId )
{
    VISN[_iVisnId]->Grab() ;
}

void CVisionMan::Live(EN_VISN_ID _iVisnId , bool _bOn)
{
    VISN[_iVisnId]->Live(_bOn);
}

bool CVisionMan::GetGrabEnd(EN_VISN_ID _iVisnId )
{
    return VISN[_iVisnId]->GetGrabEnd();

}


bool CVisionMan::GetInspEnd(EN_VISN_ID _iVisnId )
{
    return Rslt[_iVisnId].bInspEnd ;

}

void CVisionMan::SetLight  (EN_VISN_ID _iVisnId )
{
    VISN[_iVisnId]->SetLight();
}



void CVisionMan::Inspect(EN_VISN_ID _iVisnId , bool _bUseThread)
{
    if(!_bUseThread) {
        VISN[_iVisnId] -> Inspection() ;
    }
    else {
        VISN[_iVisnId] -> InspectionThread() ;
    }
}

int CVisionMan::GetRslt (EN_VISN_ID _iVisnId , TRetResult * _tRslt)
{
    if(_iVisnId == viPkg) {
        _tRslt -> bInspEnd = ((CPackage *)VISN[_iVisnId]) -> Rslt.bInspEnd ;
        _tRslt -> iRet     = ((CPackage *)VISN[_iVisnId]) -> Rslt.iRet     ;
        _tRslt -> iFailCnt = ((CPackage *)VISN[_iVisnId]) -> Rslt.iFailCnt ;
    }


    memcpy(&Rslt[_iVisnId] , _tRslt , sizeof(TRetResult));
    return Rslt[_iVisnId].iRet ;
}

void CVisionMan::SetLevel(EN_OPER_LEVEL _iLevel)
{
    g_iCrntLevel = _iLevel ;
}
        /*
        0 = Oper
        1 = Engineer
        2 = Master
        */

void CVisionMan::SetAutorun(bool _bAutorun)
{


}


//비젼 탭 바꾸기.
void CVisionMan::SetActiveVisn(EN_VISN_ID _iVisnId)
{
    //Trace("csPaint",("SetActiveVisn" + AnsiString(_iVisnId)).c_str());
    FrmVisnMain -> pcVisnBase -> ActivePageIndex = _iVisnId ;

    //두번타게 되지만 상관없다.
    g_iActivVisnId = _iVisnId ;

    FrmPackage -> Visible = _iVisnId == viPkg ;

    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        FrmImage[i] -> tmUpdate -> Enabled = _iVisnId == i ;

    }
}








