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

#include "Orientation.h"

#include "FormOri.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)
CVisionMan VSM ;

//정수 오브더 야매..... 누가 해결좀....
void __fastcall CVisionMan::tmYameTimer(TObject *Sender)
{
//    if(!FrmVisnMain || !FrmVisnMain -> Showing) return ;
    tmYame -> Enabled = false ;
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
    return ;
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



    //Cam  [ciWrk] = new CImi() ; Cam  [ciWrk]->Init();

    //20180530 PC바꾸면서 OS를 Win7으로 업데이트하는바람에 문제가 생겨 ImiCam->Neptune으로 업데이트 ㅜ
    Cam  [ciWrk] = new CNeptune() ; Cam  [ciWrk]->Init();
    Light[liAll] = new CDkm() ; Light[liAll]->Init();


    TVisnStaticPara tPara ;
    
    VISN[viOri] = new COrientation() ;
    tPara.iVisnId     = viOri  ;
    tPara.sVisnName   = "Orientation";
    tPara.iUseCamId   = ciWrk  ;
    tPara.iUseLightId = liAll  ;
    VISN[viOri] -> Init(tPara) ;

    m_iInspectionId = viOri ;

    //비젼등등 만들어 지고 로드 한다.
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
    
    FrmOri = new TFrmOri(NULL);
    FrmOri -> ParentWindow = hParaWnd ;
    GetClientRect(hParaWnd , &rt);
    FrmOri -> Left = 0 ;
    FrmOri -> Top  = 0 ;
    FrmOri -> Width = rt.right  ;
    FrmOri -> Height= rt.bottom ;
    FrmOri -> Init();
    FrmOri -> FormShow((EN_PAGE_ID)0);



    
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

    FrmOri     ->Close(); delete FrmOri      ; FrmOri      = NULL ;

    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        FrmImage[i] -> Close();
        delete FrmImage[i] ; FrmImage[i] = NULL ;
    }
    
    FrmVisnMain -> Del();
    delete FrmVisnMain ; FrmVisnMain = NULL ;
    
    
    
    //소멸자 가상함수 선언 안하면 이렇게 써야 함.
    //delete ((CBfPlce  *)VISN[viOri]) ; VISN[viOri] = NULL ;
    //delete ((CAtPlce  *)VISN[viAtPlce]) ; VISN[viAtPlce] = NULL ;
    //delete ((CBfPick *)VISN[viBfPick]) ; VISN[viBfPick] = NULL ;
    //delete ((CAtPick *)VISN[viAtPick]) ; VISN[viAtPick] = NULL ;
    
    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        VISN[i] -> Close();
        delete VISN[i] ;
    }
    
    
    
    Cam[ciWrk]->Close();
    delete ((CImi *)Cam[ciWrk]) ; Cam[ciWrk] = NULL ;
    
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
    if(_iVisnId == viOri) {
        _tRslt -> bInspEnd = ((COrientation *)VISN[_iVisnId]) -> Rslt.bInspEnd ;
        _tRslt -> iRet     = ((COrientation *)VISN[_iVisnId]) -> Rslt.iRet     ;
    }

    memcpy(&Rslt[_iVisnId],_tRslt  , sizeof(TRetResult));
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

    FrmOri -> Visible = _iVisnId == viOri ;

    for(int i = 0 ; i < MAX_VISN_ID ; i++) {
        FrmImage[i] -> tmUpdate -> Enabled = _iVisnId == i ;

    }
}








