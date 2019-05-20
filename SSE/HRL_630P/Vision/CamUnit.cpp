//---------------------------------------------------------------------------
 //#include <system.hpp>
#pragma hdrstop
#include "CamUnit.h"



#include "UserINI.h"
//#include "SLogUnit.h" 이거 넣으면 메모리 누수 난다. 이유는 나중에 찾자.
//---------------------------------------------------------------------------
#pragma package(smart_init)
#define CLIP(t) (((t)>255)?255:(((t)<0)?0:(t)))//값의 범위가  0 <= t < 256 되도록 한다

CCam * Cam[MAX_CAM_ID] ;
/*
void InitCam (EN_CAM_TYPE * _iCamType)
{
    for(int i = 0 ; i < MAX_CAM_ID ; i++) {
             if(_iCamType[i] == ctEur     ) { Cam[i] = new CEur() ; }
        else if(_iCamType[i] == ctImi     ) { Cam[i] = new CImi() ; }
        else                                { ShowMessage("Unknown Cam Type!");}
    }
}
void CloseCam()
{
    for(int i = 0 ; i < MAX_CAM_ID ; i++) {
        delete Cam [i] ;
    }
}
*/

void ConvertYUV422toRGB(int width,int height , unsigned char** pSrc, unsigned char** pDst)
{
    int i,j;
    unsigned char* dst=*pDst;
    unsigned char* src=*pSrc;
    int Y1,Y2,U,V;
    int R1,G1,B1 , R2,G2,B2;

    double dTime = GetTickCount() ;

    for(i=0;i<width*height;i=i+2){
        Y1=src[i*2+1];
        Y2=src[i*2+3];
        U=src[i*2];
        V=src[i*2+2];


        //성공.  300ms     1600*1200
        //R1=CLIP((1.164*(Y1 - 16)) + (2.018*(U - 128)));
        //G1=CLIP((1.164*(Y1 - 16)) - (0.813*(V - 128)) - (0.391*(U - 128)));
        //B1=CLIP((1.164*(Y1 - 16)) + (1.596*(V - 128)));
        //R2=CLIP((1.164*(Y2 - 16)) + (2.018*(U - 128)));
        //G2=CLIP((1.164*(Y2 - 16)) - (0.813*(V - 128)) - (0.391*(U - 128)));
        //B2=CLIP((1.164*(Y2 - 16)) + (1.596*(V - 128)));

        //성공.. floating 연산 제거.  80ms  1600*1200
        R1=CLIP(( 76284*(Y1-16) + 132252*(U-128)                  ) >> 16);
        G1=CLIP(( 76284*(Y1-16) -  53281*(V-128) -  25625*(U-128) ) >> 16);
        B1=CLIP(( 76284*(Y1-16) + 104595*(V-128)                  ) >> 16);
        R2=CLIP(( 76284*(Y2-16) + 132252*(U-128)                  ) >> 16);
        G2=CLIP(( 76284*(Y2-16) -  53281*(V-128) -  25625*(U-128) ) >> 16);
        B2=CLIP(( 76284*(Y2-16) + 104595*(V-128)                  ) >> 16);

        dst[i*3+2]= B1;
        dst[i*3+1]= G1;
        dst[i*3  ]= R1;
        dst[i*3+5]= B2;
        dst[i*3+4]= G2;
        dst[i*3+3]= R2;
    }

    dTime = GetTickCount() - dTime ;
}

int g_iMaxImiCam ;
void CImi::MakeUI()
{
    ParaUi.pnBase = new TPanel(Application);

    ParaUi.lbVideoFormat = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbVideoFormat , 0 , 0 ); ParaUi.cbVideoFormat = new TComboBox(ParaUi.pnBase); SetArrange(ParaUi.cbVideoFormat , 1 , 0 );
    ParaUi.lbVideoMode   = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbVideoMode   , 0 , 1 ); ParaUi.cbVideoMode   = new TComboBox(ParaUi.pnBase); SetArrange(ParaUi.cbVideoMode   , 1 , 1 );
    ParaUi.lbFrameRate   = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbFrameRate   , 0 , 2 ); ParaUi.cbFrameRate   = new TComboBox(ParaUi.pnBase); SetArrange(ParaUi.cbFrameRate   , 1 , 2 );

    ParaUi.lbBrightness  = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbBrightness  , 0 , 3 ); ParaUi.edBrightness  = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edBrightness  , 1 , 3 );
    ParaUi.lbSharpness   = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbSharpness   , 0 , 4 ); ParaUi.edSharpness   = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edSharpness   , 1 , 4 );
    ParaUi.lbGamma       = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbGamma       , 0 , 5 ); ParaUi.edGamma       = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edGamma       , 1 , 5 );
    ParaUi.lbExposure    = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbExposure    , 0 , 6 ); ParaUi.edExposure    = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edExposure    , 1 , 6 );
    ParaUi.lbShutter     = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbShutter     , 0 , 7 ); ParaUi.edShutter     = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edShutter     , 1 , 7 );
    ParaUi.lbGain        = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbGain        , 0 , 8 ); ParaUi.edGain        = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edGain        , 1 , 8 );
    ParaUi.lbPan         = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbPan         , 0 , 9 ); ParaUi.edPan         = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edPan         , 1 , 9 );
    ParaUi.lbTilt        = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbTilt        , 0 , 10); ParaUi.edTilt        = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edTilt        , 1 , 10);


    ParaUi.lbVideoFormat -> Parent = ParaUi.pnBase ; ParaUi.cbVideoFormat -> Parent = ParaUi.pnBase ;
    ParaUi.lbVideoMode   -> Parent = ParaUi.pnBase ; ParaUi.cbVideoMode   -> Parent = ParaUi.pnBase ;
    ParaUi.lbFrameRate   -> Parent = ParaUi.pnBase ; ParaUi.cbFrameRate   -> Parent = ParaUi.pnBase ;

    ParaUi.lbBrightness  -> Parent = ParaUi.pnBase ; ParaUi.edBrightness  -> Parent = ParaUi.pnBase ;
    ParaUi.lbSharpness   -> Parent = ParaUi.pnBase ; ParaUi.edSharpness   -> Parent = ParaUi.pnBase ;
    ParaUi.lbGamma       -> Parent = ParaUi.pnBase ; ParaUi.edGamma       -> Parent = ParaUi.pnBase ;
    ParaUi.lbExposure    -> Parent = ParaUi.pnBase ; ParaUi.edExposure    -> Parent = ParaUi.pnBase ;
    ParaUi.lbShutter     -> Parent = ParaUi.pnBase ; ParaUi.edShutter     -> Parent = ParaUi.pnBase ;
    ParaUi.lbGain        -> Parent = ParaUi.pnBase ; ParaUi.edGain        -> Parent = ParaUi.pnBase ;
    ParaUi.lbPan         -> Parent = ParaUi.pnBase ; ParaUi.edPan         -> Parent = ParaUi.pnBase ;
    ParaUi.lbTilt        -> Parent = ParaUi.pnBase ; ParaUi.edTilt        -> Parent = ParaUi.pnBase ;


    ParaUi.lbVideoFormat -> Caption = "Video Format" ;
    ParaUi.lbVideoMode   -> Caption = "Video Mode  " ;
    ParaUi.lbFrameRate   -> Caption = "FrameRate   " ;

    ParaUi.lbBrightness  -> Caption = "Brightness  " ;
    ParaUi.lbSharpness   -> Caption = "Sharpness   " ;
    ParaUi.lbGamma       -> Caption = "Gamma       " ;
    ParaUi.lbExposure    -> Caption = "Exposure    " ;
    ParaUi.lbShutter     -> Caption = "Shutter     " ;
    ParaUi.lbGain        -> Caption = "Gain        " ;
    ParaUi.lbPan         -> Caption = "Pan         " ;
    ParaUi.lbTilt        -> Caption = "Tilt        " ;

/*
    ParaUi.cbVideoFormat -> AddItem("0 IMC_FORMAT_RGB24"  , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("1 IMC_FORMAT_Y800"   , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("2 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("3 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("4 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("5 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("6 IMC_FORMAT_Y1600"  , NULL) ;

    ParaUi.cbVideoMode   -> AddItem("1 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("2 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("3 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("4 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("5 IMC_MODE_640x480"  , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("6 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("7 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("8 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("9 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("10 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("11 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("12 IMC_MODE_1600x1200" , NULL) ;

    ParaUi.cbFrameRate   -> AddItem("1 IMC_FRATE_3_75" , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("1 IMC_FRATE_3_75" , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("2 IMC_FRATE_7_5"  , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("3 IMC_FRATE_10"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("4 IMC_FRATE_15"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("5 IMC_FRATE_20"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("6 IMC_FRATE_30"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("7 IMC_FRATE_40"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("8 IMC_FRATE_60"   , NULL) ;
*/
}

void CImi::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 60  ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;

}

// callback which is used for grabbing original data from camera.
int CALLBACK CImi::ImiCallback(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize )
{
    ((CImi *)pContext) -> SetBuffer(pBuffer , lBufferSize) ;
	return 0;
}

void CImi::SetBuffer(unsigned char * pBuffer, int lBufferSize)
{
    m_fGrabTime   = GetTickCount() - m_fGrabOnTime;

    m_iImgBit = lBufferSize * 8 / (m_iImgWidth * m_iImgHeight) ;
    m_pImgBuf = (unsigned char *)pBuffer ;

    if(Para.iVideoFormat == IMC_FORMAT_Y422 && m_iImgBit == 16) { //IMI 현재 8비트 와 16비트 가 있는데 8비트는 256색흑백 , 16비트는 컬러 YUV211형식이라 24비트 RGB888로 컨버팅 해줘야 한다.
        //컨버젼
        //yuv_to_rgb(m_iImgWidth, m_iImgHeight, pBuffer, m_pImgRGBBuf);
        ConvertYUV422toRGB(m_iImgWidth, m_iImgHeight, &pBuffer, &m_pImgRGBBuf);
        //YUY422_to_RGB24(m_iImgWidth, m_iImgHeight, pBuffer, m_pImgRGBBuf);
        m_iImgBit = 24 ; //RGB24
        m_pImgBuf = m_pImgRGBBuf ;
    }

    if(m_pCallbackFunc) m_pCallbackFunc(m_iCamNo);
}

CImi::CImi(void)
{
    //Init();
}

CImi::~CImi(void)
{
    //Close();
    static int i = 0 ;
    i++;
}

bool CImi::Init()
{
    m_pCallbackFunc = NULL ;

    m_pImgBuf    = NULL ;
    m_pImgRGBBuf = NULL ;

    static bool bFirst = true ;
    static int iCamNo = 0 ;
    //m_bInitialed = false ;
    m_iImiCamNo = iCamNo ;
    iCamNo++;                                                                  



    MakeUI();

    if(bFirst) {
        bFirst = false ;

        m_bDllLoaded = LoadImCamApi("ImCam.dll") ;
        if(!m_bDllLoaded) {ShowMessage("LoadImCamApi Fail"); return false ; }

        ImCamUninit();
        g_iMaxImiCam = ImCamInit();
        if(g_iMaxImiCam < 1) {
            ShowMessage("Camera loading Fail!");
            return false;
        }
    }

    m_iCamType = ctImi ;
    LoadPara(true , EXE_FOLDER + "\\Util\\Vision\\ImCamPara.ini"); //  일단 임시로.
    //Para    .iVideoFormat= 1   ; //;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
    //Para    .iVideoMode  = 5   ; //;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
    //Para    .iFrameRate  = 8   ; //;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60
    //UserPara.iBrightness = 490 ;
    //UserPara.iSharpness  = 550 ;
    //UserPara.iGamma      = 10  ;
    //UserPara.iExposure   = 50  ; //요건 동작 안함....
    //UserPara.iShutter    = 1100;
    //UserPara.iGain       = 0   ;
    //UserPara.iPan        = 326 ;
    //UserPara.iTilt       = 246 ;
    //LoadPara(false ,EXE_FOLDER + "\\Util\\Vision\\ImCamPara" + AnsiString(m_iImiCamNo) + ".ini");


    //
    if(ImCamSetVideoFormat(m_iImiCamNo, Para.iVideoFormat)!= IMC_SUCCESS) {ShowMessage("Format not found!"   ); return false ; }//;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
    if(ImCamSetVideoMode  (m_iImiCamNo, Para.iVideoMode  )!= IMC_SUCCESS) {ShowMessage("Mode not found!"     ); return false ; }//;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
    if(ImCamSetFrameRate  (m_iImiCamNo, Para.iFrameRate  )!= IMC_SUCCESS) {ShowMessage("FrameRate not found!"); return false ; }//;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60

    ApplyPara();

    int iListCnt = 10 ;
    int iList[10] ;
    AnsiString sListItem ;
    if(ImCamGetFormatList(m_iImiCamNo , &iListCnt, iList) != IMC_SUCCESS) {
        ShowMessage("ImCamGetFormatList");
    }
    for(int i = 0 ; i < iListCnt ; i++) {
       sListItem = iList[i] ;
       //ShowMessage(sListItem);
    }

    if(ImCamGetFormatList(m_iImiCamNo , &iListCnt, iList) != IMC_SUCCESS) {
        ShowMessage("ImCamGetFormatList");
    }

    ImCamSetCallback(m_iImiCamNo,ImiCallback,this);


    IMC_MEMORY_CHANNEL_PROPERTY MemChProperty;
    memset(&MemChProperty, 0, sizeof(IMC_MEMORY_CHANNEL_PROPERTY));
    MemChProperty.bSaveLoad = 0;
    MemChProperty.nChannelIndex = 5;//imi셑업에서 5번만 쓴다. JH

    if(ImCamSetMemoryChannel(m_iImiCamNo, &MemChProperty)!= IMC_SUCCESS) {
        ShowMessage("Can not load the memory channel");
        return false;
    }


    /*
    nCamIndex     : open 기능을 수행할 camera의 index
    bUseCallback  : callback 함수를 사용할 것인지를 사용하는 flag
    bUsePreview   : preview window를 사용할 것인지를 설정하는 flag
    hPreviewWnd   : 사용자가 지정하는 preview window에 대한 handle
    ulResizeView  : user preview window에 대한 layout 방법을 지정
    bFullView     : bUsePreview가 설정된 경우에, preview window의 full 화면 모드를 설정하는 flag
    grabMode      : Grab함수 또는 Callback함수에서 받는 한 frame 데이터 타입 설정
    bColorConvert : Bayer color 변환을 수행할 것인지를 설정하는 flag
    nColorPattern : Bayer color 변환을 수행할 때, image data의 color pattern
  if(pAS->m_iPixelFormat == XAS_PF_GRAY1 || pAS->m_iPixelFormat == XAS_PF_GRAY2)
             bError = ImCamOpen(pAS->m_iAcqNum, TRUE, FALSE, NULL, IMC_VIEW_RESIZE, FALSE, IMC_GRAB_RAW, FALSE, IMC_BAYER_GB_RG);
  else if(pAS->m_iPixelFormat == XAS_PF_RGB3)
             bError = ImCamOpen(pAS->m_iAcqNum, TRUE, TRUE, NULL, IMC_VIEW_RESIZE, FALSE, IMC_GRAB_RGB888, TRUE, IMC_BAYER_GB_RG);
    */

	if(ImCamOpen(m_iImiCamNo, true, false, NULL , IMC_VIEW_STATIC, false, IMC_GRAB_RAW, 0, IMC_BAYER_GB_RG) != IMC_SUCCESS) {
		ShowMessage("can't open");
		return false;
	}

    int   iWidth , iHeight ,iBit;
    /*
    if(ImCamGetImageSize(m_iImiCamNo,&iWidth , &iHeight) != IMC_SUCCESS ||
       ImCamGetDataBits (m_iImiCamNo,&iBit             ) != IMC_SUCCESS ){
        iWidth  = 640 ; iHeight = 480 ; iBit    = 8   ;
        ShowMessage("Can't Get ImageSize");
    }*/
    if(ImCamGetROISize(m_iImiCamNo,&iWidth , &iHeight) != IMC_SUCCESS ||
       ImCamGetDataBits (m_iImiCamNo,&iBit             ) != IMC_SUCCESS ){
        iWidth  = 640 ; iHeight = 480 ; iBit    = 8   ;
        ShowMessage("Can't Get ImageSize");
    }
    m_iImgWidth  = iWidth  ;
    m_iImgHeight = iHeight ;
    m_iImgBit    = iBit    ;

    m_pImgRGBBuf = new unsigned char [m_iImgWidth * m_iImgHeight * 3] ;



    ImCamStart(m_iImiCamNo);

    SetTrgMode(tmSw);

    IMC_TRIGGER_INFO TrigInfo;
    memset(&TrigInfo, 0, sizeof(IMC_TRIGGER_INFO));
    if (ImCamGetTriggerInfo(m_iImiCamNo, &TrigInfo) != IMC_SUCCESS ) {
    	ShowMessage("This Camera doesn't support Trigger Function!.");
    	return false;
    }
    return true ;
}

bool CImi::Close()
{
    static int iClosedCamCnt = 0 ;

    if(m_bDllLoaded){
        ImCamSetCallback(m_iImiCamNo, NULL,NULL);
        ImCamStop       (m_iImiCamNo           );
        ImCamClose      (m_iImiCamNo           );
    }

    if(m_pImgRGBBuf) {
        delete [] m_pImgRGBBuf ;
        m_pImgRGBBuf = NULL ;
    }

    iClosedCamCnt++;

    if(iClosedCamCnt == g_iMaxImiCam && m_bDllLoaded) UnloadImCamApi();

    return true ;
}

bool CImi::GetImg(unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt)
{
    _pImg    = m_pImgBuf    ;
    _iWidth  = m_iImgWidth  ;
    _iHeight = m_iImgHeight ;
    _iBitCnt = m_iImgBit    ;

    return true ;
}

void CImi::LoadPara(bool _bLoad , AnsiString _sPath)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sTemp = "ImiCamera" + AnsiString(m_iImiCamNo);
    sPath = _sPath;

    if(_bLoad) {
        //Load Device.
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iVideoFormat"    , Para.iVideoFormat    ); //;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iVideoMode  "    , Para.iVideoMode      ); //;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iFrameRate  "    , Para.iFrameRate      ); //;2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60

        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iBrightness" , UserPara.iBrightness );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iSharpness " , UserPara.iSharpness  );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iGamma     " , UserPara.iGamma      );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iExposure  " , UserPara.iExposure   );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iShutter   " , UserPara.iShutter    );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iGain      " , UserPara.iGain       );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iPan       " , UserPara.iPan        );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iTilt      " , UserPara.iTilt       );
    }
    else {
        //Save Device.
        //UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iVideoFormat"    , Para.iVideoFormat    ); //;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iVideoMode  "    , Para.iVideoMode      ); //;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iFrameRate  "    , Para.iFrameRate      ); //;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60

        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iBrightness" , UserPara.iBrightness );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iSharpness " , UserPara.iSharpness  );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iGamma     " , UserPara.iGamma      );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iExposure  " , UserPara.iExposure   );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iShutter   " , UserPara.iShutter    );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iGain      " , UserPara.iGain       );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iPan       " , UserPara.iPan        );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iTilt      " , UserPara.iTilt       );
        ApplyPara();
    }

}

/*
void CImi::SavePara(AnsiString _sPath)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sTemp = "ImiCamera" + m_iImiCamNo;
    sPath = _sPath ;

    UserINI.ClearFile(sPath) ;



    ApplyPara();
}*/

bool CImi::ApplyPara()
{
    //카메라 오픈하고 나선 안바낌.;;;;
    //첨에 킬때만 바뀌어서 이파라미터 바꾸면 껐다켜야함.
    //if(ImCamSetVideoFormat(m_iImiCamNo, Para.iVideoFormat)!= IMC_SUCCESS) {ShowMessage("Format not found!"   );  }//;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
    //if(ImCamSetVideoMode  (m_iImiCamNo, Para.iVideoMode  )!= IMC_SUCCESS) {ShowMessage("Mode not found!"     );  }//;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
    //if(ImCamSetFrameRate  (m_iImiCamNo, Para.iFrameRate  )!= IMC_SUCCESS) {ShowMessage("FrameRate not found!");  }//;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60
    IMC_FEATURE_PROPERTY	FeatureProp;

    memset(&FeatureProp, 0, sizeof(IMC_FEATURE_PROPERTY));
                                                                                                                                                                                                //1600.1200 default
    FeatureProp.nValue = UserPara.iBrightness; if(ImCamSetFeatureValue(m_iImiCamNo, IMC_FEATURE_BRIGHTNESS, &FeatureProp)!= IMC_SUCCESS) {ShowMessage("Failed Set BRIGHTNESS"); return false ; }//490
    FeatureProp.nValue = UserPara.iSharpness ; if(ImCamSetFeatureValue(m_iImiCamNo, IMC_FEATURE_SHARPNESS , &FeatureProp)!= IMC_SUCCESS) {ShowMessage("Failed Set SHARPNESS "); return false ; }//550
    FeatureProp.nValue = UserPara.iGamma     ; if(ImCamSetFeatureValue(m_iImiCamNo, IMC_FEATURE_GAMMA     , &FeatureProp)!= IMC_SUCCESS) {ShowMessage("Failed Set GAMMA     "); return false ; }//10
    FeatureProp.nValue = UserPara.iExposure  ;                                                                                                                                                  //50
    if(ImCamSetFeatureValue(m_iImiCamNo, IMC_FEATURE_EXPOSURE  , &FeatureProp)!= IMC_SUCCESS) {
        ShowMessage("Failed Set EXPOSURE  ");
        return false ;
    }
    FeatureProp.nValue = UserPara.iShutter   ; if(ImCamSetFeatureValue(m_iImiCamNo, IMC_FEATURE_SHUTTER   , &FeatureProp)!= IMC_SUCCESS) {ShowMessage("Failed Set SHUTTER   "); return false ; } //1605 외부에서 설정.
    FeatureProp.nValue = UserPara.iGain      ; if(ImCamSetFeatureValue(m_iImiCamNo, IMC_FEATURE_GAIN      , &FeatureProp)!= IMC_SUCCESS) {ShowMessage("Failed Set GAIN      "); return false ; } //0
    FeatureProp.nValue = UserPara.iPan       ; if(ImCamSetFeatureValue(m_iImiCamNo, IMC_FEATURE_PAN       , &FeatureProp)!= IMC_SUCCESS) {ShowMessage("Failed Set PAN       "); return false ; } //810
    FeatureProp.nValue = UserPara.iTilt      ; if(ImCamSetFeatureValue(m_iImiCamNo, IMC_FEATURE_TILT      , &FeatureProp)!= IMC_SUCCESS) {ShowMessage("Failed Set TILT      "); return false ; } //616

    return true ;
}

void CImi::UpdatePara(bool _bToTable)
{
    if(_bToTable) {
         if(ParaUi.cbVideoFormat) ParaUi.cbVideoFormat -> ItemIndex = Para   .iVideoFormat ;
         if(ParaUi.cbVideoMode  ) ParaUi.cbVideoMode   -> ItemIndex = Para   .iVideoMode   ;
         if(ParaUi.cbFrameRate  ) ParaUi.cbFrameRate   -> ItemIndex = Para   .iFrameRate   ;

         if(ParaUi.edBrightness ) ParaUi.edBrightness  -> Text      = UserPara.iBrightness ;
         if(ParaUi.edSharpness  ) ParaUi.edSharpness   -> Text      = UserPara.iSharpness  ;
         if(ParaUi.edGamma      ) ParaUi.edGamma       -> Text      = UserPara.iGamma      ;
         if(ParaUi.edExposure   ) ParaUi.edExposure    -> Text      = UserPara.iExposure   ;
         if(ParaUi.edShutter    ) ParaUi.edShutter     -> Text      = UserPara.iShutter    ;
         if(ParaUi.edGain       ) ParaUi.edGain        -> Text      = UserPara.iGain       ;
         if(ParaUi.edPan        ) ParaUi.edPan         -> Text      = UserPara.iPan        ;
         if(ParaUi.edTilt       ) ParaUi.edTilt        -> Text      = UserPara.iTilt       ;
     }

     else {
         int iTemp;
                
         if(ParaUi.cbVideoFormat && ParaUi.cbVideoFormat -> ItemIndex != Para   .iVideoFormat) ; {
             ShowMessage("Please Restart Program.");
             //Trace("Operation" , "Both Imi_VideoFormat values are different.");

         }
         if(ParaUi.cbVideoMode   && ParaUi.cbVideoMode   -> ItemIndex != Para   .iVideoMode  ) ; {
             ShowMessage("Please Restart Program.");
             //Trace("Operation" , "Both Imi_VideoMode values are different.");
         }
         if(ParaUi.cbFrameRate   && ParaUi.cbFrameRate   -> ItemIndex != Para   .iFrameRate  ) ; {
             ShowMessage("Please Restart Program.");
             //Trace("Operation" , "Both Imi_FrameRate values are different.");
         }



         //iTemp = ParaUi.cbVideoFormat -> ItemIndex; if(Para.iVideoFormat != iTemp){ Trace("Operation" , "Both Imi_VideoFormat values are different."); }
         //iTemp = ParaUi.cbVideoMode   -> ItemIndex; if(Para.iVideoMode   != iTemp){ Trace("Operation" , "Both Imi_VideoMode values are different."); }
         //iTemp = ParaUi.cbFrameRate   -> ItemIndex; if(Para.iFrameRate   != iTemp){ Trace("Operation" , "Both Imi_FrameRate values are different."); }
         //
         //iTemp = StrToIntDef(ParaUi.edBrightness -> Text , UserPara.iBrightness ); if(UserPara.iBrightness != iTemp){ Trace("Operation" , "Both Imi_Brightness values are different."); }
         //iTemp = StrToIntDef(ParaUi.edSharpness  -> Text , UserPara.iSharpness  ); if(UserPara.iSharpness  != iTemp){ Trace("Operation" , "Both Imi_Sharpness values are different."); }
         //iTemp = StrToIntDef(ParaUi.edGamma      -> Text , UserPara.iGamma      ); if(UserPara.iGamma      != iTemp){ Trace("Operation" , "Both Imi_Gamma values are different."); }
         //iTemp = StrToIntDef(ParaUi.edExposure   -> Text , UserPara.iExposure   ); if(UserPara.iExposure   != iTemp){ Trace("Operation" , "Both Imi_Exposure values are different."); }
         //iTemp = StrToIntDef(ParaUi.edShutter    -> Text , UserPara.iShutter    ); if(UserPara.iShutter    != iTemp){ Trace("Operation" , "Both Imi_Shutter values are different."); }
         //iTemp = StrToIntDef(ParaUi.edGain       -> Text , UserPara.iGain       ); if(UserPara.iGain       != iTemp){ Trace("Operation" , "Both Imi_Gain values are different."); }
         //iTemp = StrToIntDef(ParaUi.edPan        -> Text , UserPara.iPan        ); if(UserPara.iPan        != iTemp){ Trace("Operation" , "Both Imi_Pan values are different."); }
         //iTemp = StrToIntDef(ParaUi.edTilt       -> Text , UserPara.iTilt       ); if(UserPara.iTilt       != iTemp){ Trace("Operation" , "Both Imi_Tilt values are different."); }





         if(ParaUi.cbVideoFormat) Para    .iVideoFormat   = ParaUi.cbVideoFormat -> ItemIndex  ;
         if(ParaUi.cbVideoMode  ) Para    .iVideoMode     = ParaUi.cbVideoMode   -> ItemIndex  ;
         if(ParaUi.cbFrameRate  ) Para    .iFrameRate     = ParaUi.cbFrameRate   -> ItemIndex  ;

         if(ParaUi.edBrightness ) UserPara.iBrightness    = StrToIntDef(ParaUi.edBrightness -> Text , UserPara.iBrightness );
         if(ParaUi.edSharpness  ) UserPara.iSharpness     = StrToIntDef(ParaUi.edSharpness  -> Text , UserPara.iSharpness  );
         if(ParaUi.edGamma      ) UserPara.iGamma         = StrToIntDef(ParaUi.edGamma      -> Text , UserPara.iGamma      );
         if(ParaUi.edExposure   ) UserPara.iExposure      = StrToIntDef(ParaUi.edExposure   -> Text , UserPara.iExposure   );
         if(ParaUi.edShutter    ) UserPara.iShutter       = StrToIntDef(ParaUi.edShutter    -> Text , UserPara.iShutter    );
         if(ParaUi.edGain       ) UserPara.iGain          = StrToIntDef(ParaUi.edGain       -> Text , UserPara.iGain       );
         if(ParaUi.edPan        ) UserPara.iPan           = StrToIntDef(ParaUi.edPan        -> Text , UserPara.iPan        );
         if(ParaUi.edTilt       ) UserPara.iTilt          = StrToIntDef(ParaUi.edTilt       -> Text , UserPara.iTilt       );
     }


}

bool CImi::Grab()
{
    if (ImCamGenerateSWTrigger(m_iImiCamNo) != IMC_SUCCESS ) {
	ShowMessage("Trigger Failed!");
	return false;
    }
    m_fGrabOnTime = GetTickCount() ;

    return true ;
}

bool CImi::SetTrgMode(EN_TRG_MODE _iMode)
{
    IMC_TRIGGER_PROPERTY   TrigProp;
    IMC_TRIGGER_INFO	   TrigInfo;
    memset(&TrigProp, 0, sizeof(IMC_TRIGGER_PROPERTY));
    memset(&TrigInfo, 0, sizeof(IMC_TRIGGER_INFO    ));

    TrigProp.nTriggerMode = 0;
    TrigProp.nTriggerParam = 0;
    if ( ImCamGetTriggerInfo(m_iImiCamNo, &TrigInfo) != IMC_SUCCESS ){
        ShowMessage("This Camera model doesn't support Trigger");
        TrigProp.bTriggerOn = FALSE;
        return false;
    }

    if( _iMode == tmSw) {
	TrigProp.bTriggerOn = true;
	TrigProp.nTriggerSource = 7;		// SW trigger
    }
    else if (_iMode == tmHw){
        TrigProp.bTriggerOn = true;
        TrigProp.nTriggerSource = 0;
    }
    else if (_iMode == tmLive) {
        TrigProp.bTriggerOn = false;
	TrigProp.nTriggerSource = 0;
    }




    if ( ImCamSetTriggerMode(m_iImiCamNo, TrigProp) != IMC_SUCCESS ) {
        //이상하게 실행파일로 하면 메세지가 뜸.
    	ShowMessage("This Camera doesn't support Trigger Function!");
    	return false;
    }
    return true ;
}

/*
bool CImi::SetTrgMode(EN_TRG_MODE _iMode)
{
    IMC_TRIGGER_PROPERTY	TrigProp;
    memset(&TrigProp, 0, sizeof(IMC_TRIGGER_PROPERTY));

    TrigProp.nTriggerMode = 0;
    TrigProp.nTriggerParam = 0;

    if( _iMode == tmSw) {
	TrigProp.bTriggerOn = true;
	TrigProp.nTriggerSource = 7;		// SW trigger
    }
    else if (_iMode == tmHw){
        TrigProp.bTriggerOn = true;
        TrigProp.nTriggerSource = 0;
    }
    else if (_iMode == tmLive) {
        TrigProp.bTriggerOn = false;
	TrigProp.nTriggerSource = 0;
    }

    if ( ImCamSetTriggerMode(m_iImiCamNo, TrigProp) != IMC_SUCCESS ) {
        //이상하게 실행파일로 하면 메세지가 뜸.
    	ShowMessage("CamSetTriggerMode Failed!");
    	return false;
    }
    return true ;
}*/

void CImi::SetGrabFunc(CamCallbackFunc _pFunc)
{
    m_pCallbackFunc = _pFunc ;

}

bool CImi::ShowParaForm(TPanel * _pnBase)
{
    ParaUi.pnBase -> Parent = _pnBase ;
    ParaUi.pnBase -> Align  = alClient ;


    ParaUi.cbVideoFormat -> Items->Clear();
    ParaUi.cbVideoFormat -> AddItem("RGB24" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("Y800 " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("Y411 " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("Y422 " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("Y444 " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("VYUY " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("Y1600" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("RAW8 " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("RAW16" , NULL) ;

    ParaUi.cbVideoMode   -> Items->Clear();
    ParaUi.cbVideoMode   -> AddItem("160x120  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("200x150  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("320x240  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("340x240  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("636x480  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("640x480  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("800x600  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("1024x768 " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("1280x960 " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("1280x1024" , NULL) ;

    ParaUi.cbFrameRate   -> Items->Clear();
    ParaUi.cbFrameRate   -> AddItem("1.8"  , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("3.7"  , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("7.5"  , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("10"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("15"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("20"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("30"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("40"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("60"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("120"  , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("240"  , NULL) ;
    UpdatePara(true);

    return true ;
}

void CImi::SetExposure (int _iTime )
{
    IMC_FEATURE_PROPERTY	FeatureProp;
    bool  bRet ;
    memset(&FeatureProp, 0, sizeof(IMC_FEATURE_PROPERTY));

    FeatureProp.nValue = _iTime  ;

    bRet = (ImCamSetFeatureValue(m_iImiCamNo, IMC_FEATURE_SHUTTER   , &FeatureProp) == IMC_SUCCESS) ;

}

float CImi::GetGrabTime()
{
     return m_fGrabTime ;

}


















//Euresys Dual Base Board for Digital Cam.
//==============================================================================
#define EURESYS_SURFACE_COUNT 3
int g_iMaxEurCam = 0;
void CEur::MakeUI()
{
    ParaUi.pnBase = new TPanel(Application);

    ParaUi.lbStrobeDelay   = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbStrobeDelay  , 0 , 1 ); ParaUi.edStrobeDelay   = new TEdit(ParaUi.pnBase); SetArrange(ParaUi.edStrobeDelay  , 1 , 1 );
    ParaUi.lbTriggerDelay  = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbTriggerDelay , 0 , 2 ); ParaUi.edTriggerDelay  = new TEdit(ParaUi.pnBase); SetArrange(ParaUi.edTriggerDelay , 1 , 2 );
    ParaUi.lbExposure      = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbExposure     , 0 , 4 ); ParaUi.edExposure      = new TEdit(ParaUi.pnBase); SetArrange(ParaUi.edExposure     , 1 , 4 );
    ParaUi.lbCamFile       = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbCamFile      , 0 , 5 ); ParaUi.edCamFile       = new TEdit(ParaUi.pnBase); SetArrange(ParaUi.edCamFile      , 1 , 5 );
    ParaUi.lbConnector     = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbConnector    , 0 , 6 ); ParaUi.edConnector     = new TEdit(ParaUi.pnBase); SetArrange(ParaUi.edConnector    , 1 , 6 );

    ParaUi.lbStrobeDelay   -> Parent = ParaUi.pnBase ; ParaUi.edStrobeDelay  -> Parent = ParaUi.pnBase ;
    ParaUi.lbTriggerDelay  -> Parent = ParaUi.pnBase ; ParaUi.edTriggerDelay -> Parent = ParaUi.pnBase ;
    ParaUi.lbExposure      -> Parent = ParaUi.pnBase ; ParaUi.edExposure     -> Parent = ParaUi.pnBase ;
    ParaUi.lbCamFile       -> Parent = ParaUi.pnBase ; ParaUi.edCamFile      -> Parent = ParaUi.pnBase ;
    ParaUi.lbConnector     -> Parent = ParaUi.pnBase ; ParaUi.edConnector    -> Parent = ParaUi.pnBase ;

    ParaUi.lbStrobeDelay   -> Caption = "StrobeDelay " ;
    ParaUi.lbTriggerDelay  -> Caption = "TriggerDelay" ;
    ParaUi.lbExposure      -> Caption = "Exposure    " ;
    ParaUi.lbCamFile       -> Caption = "CamFile     " ;
    ParaUi.lbConnector     -> Caption = "Connector   " ;

}

void CEur::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 60  ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;

}

// callback which is used for grabbing original data from camera.
void WINAPI CEur::EureCallback (PMCSIGNALINFO SigInfo) //안되면 글로벌로 빼기.
{
    PVOID pImg;
    INT32 iSize;
    if (SigInfo && SigInfo->Context) {
        if (SigInfo->Signal == MC_SIG_SURFACE_PROCESSING) {
           // Update "current" surface address pointer
           McGetParamInt (SigInfo->SignalInfo, MC_SurfaceAddr, (PINT32) &pImg );
           McGetParamInt (SigInfo->SignalInfo, MC_SurfaceSize, (PINT32) &iSize);

           ((CEur*) SigInfo->Context) -> SetBuffer((BYTE*)pImg , iSize);
        }
        else if (SigInfo->Signal == MC_SIG_SURFACE_PROCESSING) {
           // Insert your failure handling code here.
        }
    }
}

void CEur::SetBuffer(unsigned char * pBuffer, int lBufferSize)
{
    m_fGrabTime   = GetTickCount() - m_fGrabOnTime;

    m_iImgBit = lBufferSize * 8 / (m_iImgWidth * m_iImgHeight) ;
    m_pImgBuf = (unsigned char *)pBuffer ;

    //일단 칼라 고려 하지 않음.
    //if(Para.iVideoFormat == IMC_FORMAT_Y422 && m_iImgBit == 16) { // 현재 8비트 와 16비트 가 있는데 8비트는 256색흑백 , 16비트는 컬러 YUV211형식이라 24비트 RGB888로 컨버팅 해줘야 한다.
    //    //컨버젼
    //    //yuv_to_rgb(m_iImgWidth, m_iImgHeight, pBuffer, m_pImgRGBBuf);
    //    ConvertYUV422toRGB(m_iImgWidth, m_iImgHeight, &pBuffer, &m_pImgRGBBuf);
    //    //YUY422_to_RGB24(m_iImgWidth, m_iImgHeight, pBuffer, m_pImgRGBBuf);
    //    m_iImgBit = 24 ; //RGB24
    //    m_pImgBuf = m_pImgRGBBuf ;
    //}

    if(m_pCallbackFunc) m_pCallbackFunc(m_iCamNo);
}

CEur::CEur(void)
{
    Init();
}

CEur::~CEur(void)
{
    //Close();
    static int i = 0 ;
    i++;
    //Close();
}

bool CEur::Init()
{
    m_pCallbackFunc = NULL ;

    m_pImgBuf    = NULL ;
    m_pImgRGBBuf = NULL ;

    m_iSizeX = 0 ;
    m_iSizeY = 0 ;
    //m_ChannelState = MC_ChannelState_ORPHAN;

    static bool bFirst = true ;
    if(bFirst) {
        bFirst = false ;
        // Initialize driver
        McOpenDriver (NULL);
        // Activate  message box error handling
        McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_MSGBOX);
        McSetParamStr (MC_CONFIGURATION, MC_ErrorLog, "error.log");

        // Set the board topology (only with a Grablink Expert 2)
        //McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_DUO);
        McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_DUO);

    }
    m_iEurCamNo = g_iMaxEurCam ;
    g_iMaxEurCam++;

    m_iCamType = ctEur ;


    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_hChanel);  //채널을 먼저 만들고.
    int iRet = McSetParamInt(m_hChanel, MC_DriverIndex, 0); //해당 채널의 그래버 인덱스 설정. 그래버 2장 이상이면 예외처리....

    if(iRet == -2) return false ;

    LoadPara(true  , EXE_FOLDER + "\\Util\\Vision\\EurCamPara.ini"); //  일단 임시로.
    //LoadPara(false , EXE_FOLDER + "\\Util\\Vision\\EurCamPara.ini"); //  일단 임시로.

    McSetParamStr(m_hChanel, MC_Connector, Para.sConnector.c_str()); //해당 채널의 그래버의 커넥터 설정.

    // Choose the video standard                  D:\BC6Works\SVLAppTest\bin\Util\Vision
    //AnsiString Temp = (ExtractFilePath(Application->ExeName) + "Util\\Vision\\SXC10_P120RG") ;
    AnsiString Temp = (ExtractFilePath(Application->ExeName) + Para.sCamFile) ;
    McSetParamStr(m_hChanel, MC_CamFile, Temp.c_str()); //캠파일 연결.

    // Choose the camera expose duration
    McSetParamInt(m_hChanel, MC_Expose_us, 1000); //노출시간
    // Choose the pixel color format
    McSetParamInt(m_hChanel, MC_ColorFormat, MC_ColorFormat_Y8);  //8비트 흑백.

    // Configure triggering mode
    McSetParamInt(m_hChanel, MC_TrigMode, MC_TrigMode_COMBINED);
    McSetParamInt(m_hChanel, MC_NextTrigMode, MC_NextTrigMode_COMBINED);

    // Configure triggering line
    // A rising edge on the triggering line generates a trigger.
    // See the TrigLine Parameter and the board documentation for more details.
    McSetParamInt(m_hChanel, MC_TrigLine, MC_TrigLine_IIN1);
    McSetParamInt(m_hChanel, MC_TrigEdge, MC_TrigEdge_GOHIGH);
    McSetParamInt(m_hChanel, MC_TrigFilter, MC_TrigFilter_ON);
    McSetParamInt(m_hChanel, MC_TrigCtl, MC_TrigCtl_ISO);

    // Choose the number of images to acquire
    McSetParamInt(m_hChanel, MC_SeqLength_Fr, MC_INDETERMINATE);


    // Retrieve image dimensions
    McGetParamInt (m_hChanel, MC_ImageSizeX, &m_iImgWidth );
    McGetParamInt (m_hChanel, MC_ImageSizeY, &m_iImgHeight);
    int iBufferBytePitch ;
    McGetParamInt (m_hChanel, MC_BufferPitch, &iBufferBytePitch);

    m_iImgBit = 8 * iBufferBytePitch / m_iImgWidth ;

    m_pImgRGBBuf = new unsigned char [m_iImgWidth * m_iImgHeight * 3] ; //YUV 형식 대비.

    // The memory allocation for the images is automatically done by Multicam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
    McSetParamInt (m_hChanel, MC_SurfaceCount, EURESYS_SURFACE_COUNT);

    // Enable MultiCam signals      콜백에 들어오는 시그널들.. 선택.
    McSetParamInt(m_hChanel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    McSetParamInt(m_hChanel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

    // Register the callback function
    McRegisterCallback(m_hChanel, EureCallback, this);

    // Start an acquisition sequence by activating the channel 카메라 활성화.
    McSetParamInt(m_hChanel, MC_ChannelState, MC_ChannelState_ACTIVE);

    MakeUI();

    ApplyPara();

    return true ;

}

bool CEur::Close()
{
    // Set the channel to IDLE before deleting it.
    int iChanelStat ;
    McGetParamInt (m_hChanel, MC_ChannelState, &iChanelStat);
    if(iChanelStat != MC_ChannelState_IDLE) {
        McSetParamInt (m_hChanel, MC_ChannelState, MC_ChannelState_IDLE);
    }

    // Delete the channel
    McDelete (m_hChanel);

    g_iMaxEurCam-- ;

    // Terminate driver
    if(g_iMaxEurCam==0)McCloseDriver ();

    if(m_pImgRGBBuf) {
        delete [] m_pImgRGBBuf ;
        m_pImgRGBBuf = NULL ;
    }

    return true ;
}

bool CEur::GetImg(unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt)
{
    _pImg    = m_pImgBuf    ;
    _iWidth  = m_iImgWidth  ;
    _iHeight = m_iImgHeight ;
    _iBitCnt = m_iImgBit    ;

    return true ;
}

void CEur::LoadPara(bool _bLoad , AnsiString _sPath)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sTemp = "EurCamera" + AnsiString(m_iEurCamNo);
    sPath = _sPath;

    if(_bLoad) {
        //Load Device.
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.sCamFile         " , Para.sCamFile      );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.sConnector       " , Para.sConnector    );

        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iStrobeDelay " , UserPara.iStrobeDelay  );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iTriggerDelay" , UserPara.iTriggerDelay );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iExposure    " , UserPara.iExposure     );
    }
    else {
        //Save Device.
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.sCamFile         " , Para.sCamFile      );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.sConnector       " , Para.sConnector    );

        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iStrobeDelay " , UserPara.iStrobeDelay  );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iTriggerDelay" , UserPara.iTriggerDelay );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iExposure    " , UserPara.iExposure     );
        ApplyPara();
    }


}

/*
void CEur::SavePara(AnsiString _sPath)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sTemp = "EurCamera" + m_iEurCamNo;
    sPath = _sPath ;

    UserINI.ClearFile(sPath) ;



    ApplyPara();
}
*/
bool CEur::ApplyPara()
{
    int iRet ;

    iRet = McSetParamInt(m_hChanel , MC_StrobePos   , UserPara.iStrobeDelay );
    iRet = McSetParamInt(m_hChanel , MC_TrigDelay_us, UserPara.iTriggerDelay);
    //iRet = McSetParamInt(m_hChanel , MC_Expose_us   , UserPara.iExposure    );  외부에서 지정.

    return true ;
}

void CEur::UpdatePara(bool _bToTable)
{
    if(_bToTable) {
         if(ParaUi.edCamFile   && ParaUi.edCamFile   -> Text != Para.sCamFile  ){ShowMessage("Please Restart Program.");  }
         if(ParaUi.edConnector && ParaUi.edConnector -> Text != Para.sConnector){ShowMessage("Please Restart Program.");  }

         if(ParaUi.edStrobeDelay  ) ParaUi.edStrobeDelay  -> Text = UserPara.iStrobeDelay  ;
         if(ParaUi.edTriggerDelay ) ParaUi.edTriggerDelay -> Text = UserPara.iTriggerDelay ;
         if(ParaUi.edExposure     ) ParaUi.edExposure     -> Text = UserPara.iExposure     ;

         if(ParaUi.edCamFile      ) ParaUi.edCamFile      -> Text = Para.sCamFile          ;
         if(ParaUi.edConnector    ) ParaUi.edConnector    -> Text = Para.sConnector        ;
     }

     else {
         int iTemp ;
         AnsiString sTemp ;

         //iTemp = StrToIntDef(ParaUi.edStrobeDelay  -> Text , UserPara.iStrobeDelay  ); if(UserPara.iStrobeDelay  != iTemp){ Trace("Operation" , "Both Eur_StrobeDelay values are different." ); }
         //iTemp = StrToIntDef(ParaUi.edTriggerDelay -> Text , UserPara.iTriggerDelay ); if(UserPara.iTriggerDelay != iTemp){ Trace("Operation" , "Both Eur_TriggerDelay values are different."); }
         //iTemp = StrToIntDef(ParaUi.edExposure     -> Text , UserPara.iExposure     ); if(UserPara.iExposure     != iTemp){ Trace("Operation" , "Both Eur_Exposure values are different."    ); }
         //
         //sTemp =             ParaUi.edCamFile      -> Text                           ; if(    Para.sCamFile      != sTemp){ Trace("Operation" , "Both Eur_CamFile values are different."     ); }
         //sTemp =             ParaUi.edConnector    -> Text                           ; if(    Para.sConnector    != sTemp){ Trace("Operation" , "Both Eur_Connector values are different."   ); }

         if(ParaUi.edStrobeDelay  ) UserPara.iStrobeDelay  = StrToIntDef(ParaUi.edStrobeDelay  -> Text , UserPara.iStrobeDelay  );
         if(ParaUi.edTriggerDelay ) UserPara.iTriggerDelay = StrToIntDef(ParaUi.edTriggerDelay -> Text , UserPara.iTriggerDelay );
         if(ParaUi.edExposure     ) UserPara.iExposure     = StrToIntDef(ParaUi.edExposure     -> Text , UserPara.iExposure     );

         if(ParaUi.edCamFile      )     Para.sCamFile      =             ParaUi.edCamFile      -> Text                           ;
         if(ParaUi.edConnector    )     Para.sConnector    =             ParaUi.edConnector    -> Text                           ;
     }


}

bool CEur::Grab()
{
   //if (ImCamGenerateSWTrigger(m_hChanel) != IMC_SUCCESS ) {
   //		ShowMessage("Trigger Failed!");
   //		return false;
   //	}
    //McRegisterCallback(m_hChanel, EureCallback, this);
    McSetParamInt(m_hChanel, MC_ForceTrig, MC_ForceTrig_TRIG);

    m_fGrabOnTime = GetTickCount() ;

    return true ;
}

bool CEur::SetTrgMode(EN_TRG_MODE _iMode)
{
    //귀찮다 나중에 쓸일 있으면 구현.
	ShowMessage("This Camera doesn't support Trigger Function!...");
	return false;

}

void CEur::SetGrabFunc(CamCallbackFunc _pFunc)
{
     m_pCallbackFunc = _pFunc ;

}

bool CEur::ShowParaForm(TPanel * _pnBase)
{
    ParaUi.pnBase -> Parent = _pnBase ;
    ParaUi.pnBase -> Align  = alClient ;

    UpdatePara(true);

    return true ;
}

void CEur::SetExposure (int _iTime )
{
    bool bRet ;
    bRet = McSetParamInt(m_hChanel, MC_Expose_us, _iTime);

}

float CEur::GetGrabTime()
{
     return m_fGrabTime ;

}
