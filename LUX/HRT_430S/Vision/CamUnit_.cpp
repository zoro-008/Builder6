//---------------------------------------------------------------------------
#pragma hdrstop
#include "CamUnit.h"
#include "UserINI.h"
//#include "SLogUnit.h.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define CLIP(t) (((t)>255)?255:(((t)<0)?0:(t)))//���� ������  0 <= t < 256 �ǵ��� �Ѵ�


CCam ** CCam::m_apCam =NULL;
int CCam::m_iMaxCam = 0 ;
int CCam::m_iMaxImi = 0 ;

int CCam::InitCam()
{
    TUserINI UserINI;
    String   sPath = EXE_FOLDER + "\\Util\\Vision\\CamPara.ini" ;

    int iCamCnt = 0 ;//��ī�޶� ����.

    //IMI CAM Count
    //Setting
    UserINI.Load(sPath.c_str() , "Common" , "m_iMaxImi" , m_iMaxImi );
    if(m_iMaxImi){
        CImi::InitImi();
        if(m_iMaxImi != CImi::m_iMaxImi) ShowMessage("Initialed Imi Camera Count is Wrong!");
        iCamCnt += m_iMaxImi ;
    }


    //etc...
    //���� imi ���� �ؼ� ������.
    //int      iEurCamCnt ;
    //UserINI.Load(sPath.c_str() , "Common" , "EurCamCnt" , iEurCamCnt );
    //CEur::InitEur();
    //iCamCnt += CEur::m_iMaxEur ;



    //Make Camera Pointer Memory.
    m_apCam = new CCam *[iCamCnt];


    int iMakeCamCnt = 0 ;
    //Make Imi
    for(int i = 0 ; i < m_iMaxImi  ; i++) {
        m_apCam[i] = new CImi();
        iMakeCamCnt++;
    }

    //etc...
    //for(int i = 0 ; i < iEurCamCnt ; i++) {
    //    m_apCam[i+iMakeCamCnt] = new CEur();
    //    iMakeCamCnt++
    //}

    return iCamCnt ;
}

bool CCam::CloseCam()
{
    int iDeleteCamCnt = 0 ;
    //IMI CAM Count
    //Setting
    if(m_iMaxImi) {

        for(int i = 0 ; i < m_iMaxImi ; i++) {
            delete m_apCam[i] ;
            m_apCam[i] = NULL ;
        }
        CImi::CloseImi();
        iDeleteCamCnt += m_iMaxImi ;
    }

    delete [] m_apCam ;
    m_apCam = NULL ;

    return true ;

}









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


        //����.  300ms     1600*1200
        //R1=CLIP((1.164*(Y1 - 16)) + (2.018*(U - 128)));
        //G1=CLIP((1.164*(Y1 - 16)) - (0.813*(V - 128)) - (0.391*(U - 128)));
        //B1=CLIP((1.164*(Y1 - 16)) + (1.596*(V - 128)));
        //R2=CLIP((1.164*(Y2 - 16)) + (2.018*(U - 128)));
        //G2=CLIP((1.164*(Y2 - 16)) - (0.813*(V - 128)) - (0.391*(U - 128)));
        //B2=CLIP((1.164*(Y2 - 16)) + (1.596*(V - 128)));

        //����.. floating ���� ����.  80ms  1600*1200  CPU i5 m560 2.67GHz
        //��������  640X480 8.33ms
        //���Ŀ� http://blog.naver.com/nhlsm/100023282661 ���� �����غ���.                 //�̸�����س�����...  �̸���� 2,
        R1=CLIP(( 76284*(Y1-16) + 132252*(U-128)                  ) >> 16);  //5           //
        G1=CLIP(( 76284*(Y1-16) -  53281*(V-128) -  25625*(U-128) ) >> 16);  //8
        B1=CLIP(( 76284*(Y1-16) + 104595*(V-128)                  ) >> 16);  //5
        R2=CLIP(( 76284*(Y2-16) + 132252*(U-128)                  ) >> 16);  //5
        G2=CLIP(( 76284*(Y2-16) -  53281*(V-128) -  25625*(U-128) ) >> 16);  //8
        B2=CLIP(( 76284*(Y2-16) + 104595*(V-128)                  ) >> 16);  //5
        /*                                                                   //36������.
        YUV to RGB ��ȯ(2)

         YUV->RGB ��ȯ�� ���� ������ ���� ����...
        �� ������ �ַ� �ϳ��� ����ȭ�鿡 ����Ǵ� �����̴�. ���� ���, 352x288, 176x144 ����� ����ȭ�鿡 ����� ���̴�. 100x100�� ����ȭ�鿡�� ��ȯ�� ����ȴٰ� �ϴ����, 10000 �� ȣ��ȴ�. �ſ� ����ϰ� ȣ��Ǵ� �����߿� �ϳ��̴�.
         ����ȭ�� �����̶��, ���� ���� ������ ������ �����ִ�.

          ���� ������ ���캸��,
          1) (Y-16), (U-128), (V-128), 76284*(Y-16) �� 3���� ����̵ȴ�.
          2) Y,U,V,R,G,B�� ��� 0~255�� ������ ���´�.
          ������ ����, 2)�׸��, ���� ������ ���� ���뿡�� �ſ� ����ϰ� ȣ��ȴٴ� ���̴�.
          �� ���, �ӵ��� ���� �޸𸮸� ����Ͽ�, ������ ���꿡 ���� 256 ���̺��� ����� ����� ����� �� �ִ�.


                int tab_76309[256];   // table (Y-16)*76284
                int tab_cbu[256];     // table coefficient blue U =(U-128)*1332252
                int tab_cgv[256];     // table coefficient green V =(V-128)*53281
                int tab_cgu[256];     // table coefficient green U =(U-128)*25625
                int tab_crv[256];     // table coefficient red V =(V-128)*104595
                BYTE *clp;

           void Yuv2Rgb(int Y, int U, int V, int &R, int &G, int &B)
           {
              int y = tab_76309[Y];
              B = clp[ ( y              + tab_cbu[U] ) >> 16 ];
              G = clp[ ( y - tab_cgv[V] - tab_cgu[U] ) >> 16 ];
              R = clp[ ( y + tab_crv[V] ) >> 16 ];
           }

           void init_dither_tab( void )
           {
              for( int i = 0; i < 256; i++ )
              {
                 crv_tab[i] = (i - 128) * 104595;
                 cbu_tab[i] = (i - 128) * 132252;
                 cgu_tab[i] = (i - 128) * 25625;
                 cgv_tab[i] = (i - 128) * 53281;
                 tab_76309[i] = 76284 * (i - 16);
              }
           }

           void InitColorDither( int thirty2 )
           {
              if( !(clp=(BYTE *)malloc(1024)) )
                 return;
              clp_org = clp;
              clp += 384;

              for( int i=-384; i<640; i++ )
                 clp[i] = (i<0) ? 0 : ((i>255) ? 255 : i);
           }

           => ���� �ڵ�� tmn3.0 H.263 decoder ���� �Ϻ� ������ ���̴�.

           76284*(Y-16)
           132252*(U-128)
           53281*(V-128)
           25625*(U-128)
          104595*(V-128)

             �� �κ��� ���ʿ� �ѹ��� ���ǵ��� ���̺��� �����, ���� ��ȯ�ô� �޸� access ������ �����ϵ��� �������.
             0~255 �� clipping �� �񱳿����� �ƴ� �޸� access ������ �����ϵ��� �������.

                �� ���� tmn3.0 decoder ���� ���ʹ� �ణ �ٸ� ������ ����Ѵ�.
                   ���ڰ� �� ������ �ٸ����� �𸣰ڴ�.(???)

           for (i = 0; i < 256; i++)
           {
              crv_tab[i] = (i - 128) * 104597;
              cbu_tab[i] = (i - 128) * 132201;
              cgu_tab[i] = (i - 128) * 25675;
              cgv_tab[i] = (i - 128) * 53279;
              tab_76309[i] = 76309 * (i - 16);
           }

        [��ó] YUV, RGB conversion ( 76284 ) (2)|�ۼ��� nhlsm
        */

        dst[i*3+2]= B1;
        dst[i*3+1]= G1;
        dst[i*3  ]= R1;
        dst[i*3+5]= B2;
        dst[i*3+4]= G2;
        dst[i*3+3]= R2;
    }

    dTime = GetTickCount() - dTime ;
}

//ComboBoxList        ,
void  SetList(TValueListEditor * _vePara , int _iRow , String _sList){
    TStringList * slList = new TStringList ;
    slList->CommaText = _sList ;

    _vePara ->ItemProps[_iRow]-> EditStyle = esPickList ;
    _vePara ->ItemProps[_iRow]-> PickList  = slList     ;

    delete slList ;
}


void UpdateListPara(bool _bToTable , TValueListEditor * _vePara , CLinkedList<CValue2*> * _pList){
    CValue2* Value ;
    if(_bToTable) {
        //_vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;
        //int iRowCount  = _vePara -> RowCount ;
        int iRowCount = _vePara -> Strings -> Count ;
        for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
            Value = _pList->GetCrntData(!i);
                 if(Value->GetValType()=="bool"  )_vePara -> Values[Value->GetName()] = Value->GetBoolStr();
            else if(Value->GetValType()=="int"   ){
                if(Value -> GetSelList() == "")   _vePara -> Values[Value->GetName()] = Value->GetInt    ();//int�̰� SelList������ Ȯ���ؾ���.. ���ڷ� �Ǵ��� �ƴϸ� Text�� �Ǵ���.
                else                              _vePara -> Values[Value->GetName()] = Value->GetIntString();//int�̰� SelList������ Ȯ���ؾ���.. ���ڷ� �Ǵ��� �ƴϸ� Text�� �Ǵ���.

            }
            else if(Value->GetValType()=="double")_vePara -> Values[Value->GetName()] = Value->GetDouble ();
            else if(Value->GetValType()=="String")_vePara -> Values[Value->GetName()] = Value->GetString ();
            if(Value->GetSelList()!="")SetList(_vePara , iRowCount + i , Value->GetSelList());
        }
    }
    else {
        for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
            Value = _pList->GetCrntData(!i);
                 if(Value->GetValType()=="bool"  ) Value->SetBoolFromStr  (_vePara -> Values[Value->GetName()]) ;
            else if(Value->GetValType()=="int"   ) Value->SetIntFromStr   (_vePara -> Values[Value->GetName()]) ;
            else if(Value->GetValType()=="double") Value->SetDoubleFromStr(_vePara -> Values[Value->GetName()]) ;
            else if(Value->GetValType()=="String") Value->Set             (_vePara -> Values[Value->GetName()]) ;
        }
    }
}

void LoadSaveList(bool _bLoad , String _sIniPath , String _sSection , CLinkedList<CValue2*> * _pList){
    TUserINI Ini ;
    if(_bLoad) {
        CValue2* Value ;
        for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
            Value = _pList->GetCrntData(!i);
                 if(Value->GetValType()=="bool"  )Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (bool  *)Value->GetValAdd());
            else if(Value->GetValType()=="int"   )Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (int   *)Value->GetValAdd());
            else if(Value->GetValType()=="double")Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (double*)Value->GetValAdd());
            else if(Value->GetValType()=="String")Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (String*)Value->GetValAdd());
        }
    }
    else {
        CValue2* Value ;
        for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
            Value = _pList->GetCrntData(!i);
                 if(Value->GetValType()=="bool"  )Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (bool  *)Value->GetValAdd());
            else if(Value->GetValType()=="int"   )Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (int   *)Value->GetValAdd());
            else if(Value->GetValType()=="double")Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (double*)Value->GetValAdd());
            else if(Value->GetValType()=="String")Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (String*)Value->GetValAdd());
        }
    }
}


/*
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
    ParaUi.lbVideoMode   -> Caption = "VideoMode   " ;
    ParaUi.lbFrameRate   -> Caption = "FrameRate   " ;

    ParaUi.lbBrightness  -> Caption = "Brightness  " ;
    ParaUi.lbSharpness   -> Caption = "Sharpness   " ;
    ParaUi.lbGamma       -> Caption = "Gamma       " ;
    ParaUi.lbExposure    -> Caption = "Exposure    " ;
    ParaUi.lbShutter     -> Caption = "Shutter     " ;
    ParaUi.lbGain        -> Caption = "Gain        " ;
    ParaUi.lbPan         -> Caption = "Pan         " ;
    ParaUi.lbTilt        -> Caption = "Tilt        " ;
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
*/

// callback which is used for grabbing original data from camera.
int CALLBACK CImi::ImiCallback(PVOID pContext, double SampleTime, BYTE * pBuffer, long lBufferSize )
{
    ((CImi *)pContext) -> SetBuffer(pBuffer , lBufferSize) ;
	return 0;
}

void CImi::SetBuffer(unsigned char * pBuffer, int lBufferSize)
{
    m_dGrabTime   = GetTickCount() - m_dGrabOnTime;

    m_iImgBit = lBufferSize * 8 / (m_iImgWidth * m_iImgHeight) ;
    m_pImgBuf = (unsigned char *)pBuffer ;

    if(Para.iVideoFormat == IMC_FORMAT_Y422 && m_iImgBit == 16) { //IMI ���� 8��Ʈ �� 16��Ʈ �� �ִµ� 8��Ʈ�� 256����� , 16��Ʈ�� �÷� YUV211�����̶� 24��Ʈ RGB888�� ������ ����� �Ѵ�.
        //������
        //yuv_to_rgb(m_iImgWidth, m_iImgHeight, pBuffer, m_pImgRGBBuf);
        ConvertYUV422toRGB(m_iImgWidth, m_iImgHeight, &pBuffer, &m_pImgRGBBuf);
        //YUY422_to_RGB24(m_iImgWidth, m_iImgHeight, pBuffer, m_pImgRGBBuf);
        m_iImgBit = 24 ; //RGB24
        m_pImgBuf = m_pImgRGBBuf ;
    }

    if(m_pCallbackFunc) m_pCallbackFunc(m_iCamNo);
}



int    CImi::m_iMaxCam = 0 ;
bool   CImi::m_bDllLoaded = false ;
//
bool CImi::InitImi()
{

    m_bDllLoaded = LoadImCamApi("ImCam.dll") ;
    if(!m_bDllLoaded) {ShowMessage("LoadImCamApi Fail"); return false ; }




    ImCamUninit();
    m_iMaxCam = ImCamInit();

    if(m_iMaxCam < 0 ) m_iMaxCam = 0 ;

    return true ;
}

bool CImi::CloseImi()
{

    if(!m_bDllLoaded) return false ;
    ImCamUninit();

    UnloadImCamApi();

    return true ;
}

CImi::CImi(void)
{

    Init();
}

CImi::~CImi(void)
{
    Close();
}

bool CImi::ApplyUserPara(TCamUserPara * _pCamUserPara)
{
    //ī�޶� �����ϰ� ���� �ȹٳ�.;;;;
    //÷�� ų���� �ٲ� ���Ķ���� �ٲٸ� �����Ѿ���.
    //�Ƹ� ī�޶� ��ŸƮ ������ �ٲ�� ������ ���� �ٲٷ��� ī�޶� ��ž �ϰ� �ٲ�� �ҵ�.
    //if(ImCamSetVideoFormat(Para.iPhysicalNo, Para.iVideoFormat)!= IMC_SUCCESS) {ShowMessage("Format not found!"   );  }//;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
    //if(ImCamSetVideoMode  (Para.iPhysicalNo, Para.iVideoMode  )!= IMC_SUCCESS) {ShowMessage("Mode not found!"     );  }//;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
    //if(ImCamSetFrameRate  (Para.iPhysicalNo, Para.iFrameRate  )!= IMC_SUCCESS) {ShowMessage("FrameRate not found!");  }//;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60
    IMC_FEATURE_PROPERTY	FeatureProp;
    memset(&FeatureProp, 0, sizeof(IMC_FEATURE_PROPERTY));

    TUserPara * UserPara = (TUserPara *)_pCamUserPara ;

    //�̰� �ټ��� �ϸ� 344ms ���� �ɸ�....
    //�׷� ���� ���� �ϰ� ������ ����� ����.
    //��ī�޶� ������ �����˻� ���� UserPara�� CameraPkg���� ������ �־�� �ϴµ�.
    //����ȯ ������ ������ �ȴ�. ==> �׳� �̹������ ��... void�� ���ڷ� ����.
    if(SetUserPara.iBrightness != UserPara->iBrightness) {
        FeatureProp.nValue = UserPara->iBrightness;
        SetUserPara.iBrightness = UserPara->iBrightness ;
        if(ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_BRIGHTNESS, &FeatureProp)!= IMC_SUCCESS) {
            ShowMessage("Failed Set BRIGHTNESS");
            return false ;
        }

    }

    if(SetUserPara.iSharpness != UserPara -> iSharpness){
        FeatureProp.nValue = UserPara->iSharpness ;
        SetUserPara.iSharpness = UserPara -> iSharpness ;
        if(ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_SHARPNESS , &FeatureProp)!= IMC_SUCCESS) {
            ShowMessage("Failed Set SHARPNESS ");
            return false ;
        }

    }

    if(SetUserPara.iGamma != UserPara -> iGamma){
        FeatureProp.nValue = UserPara->iGamma     ;
        SetUserPara.iGamma = UserPara -> iGamma;
        if(ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_GAMMA     , &FeatureProp)!= IMC_SUCCESS) {
            ShowMessage("Failed Set GAMMA     ");
            return false ;
        }

    }

    //�Ⱦ�.
    if(SetUserPara.iExposure != UserPara -> iExposure){
        FeatureProp.nValue = UserPara->iExposure  ;
        SetUserPara.iExposure = UserPara -> iExposure ;
        if(ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_EXPOSURE  , &FeatureProp)!= IMC_SUCCESS) {
            ShowMessage("Failed Set EXPOSURE  ");
            return false ;
        }

    }

    if(SetUserPara.iShutter != UserPara -> iShutter){
        FeatureProp.nValue = UserPara->iShutter   ;
        SetUserPara.iShutter != UserPara -> iShutter;
        if(ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_SHUTTER   , &FeatureProp)!= IMC_SUCCESS) {
            ShowMessage("Failed Set SHUTTER   ");
            return false ;
        } //�ܺο��� ����.

    }

    if(SetUserPara.iGain != UserPara -> iGain){
        FeatureProp.nValue = UserPara->iGain      ;
        SetUserPara.iGain = UserPara -> iGain ;
        if(ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_GAIN      , &FeatureProp)!= IMC_SUCCESS) {
            ShowMessage("Failed Set GAIN      ");
            return false ;
        }

    }

    //�Ⱦ�.
    if(SetUserPara.iPan != UserPara -> iPan){
        FeatureProp.nValue = UserPara->iPan       ;
        SetUserPara.iPan = UserPara -> iPan ;
        if(ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_PAN       , &FeatureProp)!= IMC_SUCCESS) {
            ShowMessage("Failed Set PAN       ");
            return false ;
        }

    }

    //�Ⱦ�.
    if(SetUserPara.iTilt != UserPara -> iTilt){
        FeatureProp.nValue = UserPara->iTilt      ;
        SetUserPara.iTilt = UserPara -> iTilt ;
        if(ImCamSetFeatureValue(Para.iPhysicalNo, IMC_FEATURE_TILT      , &FeatureProp)!= IMC_SUCCESS) {
            ShowMessage("Failed Set TILT      ");
            return false ;
        }

    }

    return true ;
}
bool CImi::Init()
{
    m_pCallbackFunc = NULL ;

    m_pImgBuf    = NULL ;
    m_pImgRGBBuf = NULL ;

    static int iCamNo = 0 ;
    m_iImiCamNo = iCamNo ;
    iCamNo++;



    //MakeUI();



    LoadPara(true , EXE_FOLDER + "\\Util\\Vision\\CamPara.ini");
    //Para    .iVideoFormat= 1   ; //;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
    //Para    .iVideoMode  = 5   ; //;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
    //Para    .iFrameRate  = 8   ; //;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60
    //UserPara.iBrightness = 490 ;
    //UserPara.iSharpness  = 550 ;
    //UserPara.iGamma      = 10  ;
    //UserPara.iExposure   = 50  ; //��� ���� ����....
    //UserPara.iShutter    = 1100;
    //UserPara.iGain       = 0   ;
    //UserPara.iPan        = 326 ;
    //UserPara.iTilt       = 246 ;
    //LoadPara(false ,EXE_FOLDER + "\\Util\\Vision\\ImCamPara" + AnsiString(m_iImiCamNo) + ".ini");

     





    //640,480��.
    if(ImCamSetVideoFormat(Para.iPhysicalNo, Para.iVideoFormat)!= IMC_SUCCESS) {ShowMessage("Format not found!"   ); return false ; }//;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
    if(ImCamSetVideoMode  (Para.iPhysicalNo, Para.iVideoMode  )!= IMC_SUCCESS) {ShowMessage("Mode not found!"     ); return false ; }//;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
    if(ImCamSetFrameRate  (Para.iPhysicalNo, Para.iFrameRate  )!= IMC_SUCCESS) {ShowMessage("FrameRate not found!"); return false ; }//;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60



    int iListCnt = 10 ;
    int iList[10] ;
    String sListItem ;
    if(ImCamGetFormatList(Para.iPhysicalNo , &iListCnt, iList) != IMC_SUCCESS) {
        ShowMessage("ImCamGetFormatList");
    }
    for(int i = 0 ; i < iListCnt ; i++) {
       sListItem = iList[i] ;
       //ShowMessage(sListItem);
    }

    //if(ImCamGetFormatList(Para.iPhysicalNo , &iListCnt, iList) != IMC_SUCCESS) {
    //    ShowMessage("ImCamGetFormatList");
    //}

    ImCamSetCallback(Para.iPhysicalNo,ImiCallback,this);


    //nCamIndex     : open ����� ������ camera�� index
    //bUseCallback  : callback �Լ��� ����� �������� ����ϴ� flag
    //bUsePreview   : preview window�� ����� �������� �����ϴ� flag
    //hPreviewWnd   : ����ڰ� �����ϴ� preview window�� ���� handle
    //ulResizeView  : user preview window�� ���� layout ����� ����
    //bFullView     : bUsePreview�� ������ ��쿡, preview window�� full ȭ�� ��带 �����ϴ� flag
    //grabMode      : Grab�Լ� �Ǵ� Callback�Լ����� �޴� �� frame ������ Ÿ�� ����
    //bColorConvert : Bayer color ��ȯ�� ������ �������� �����ϴ� flag
    //nColorPattern : Bayer color ��ȯ�� ������ ��, image data�� color pattern

    //JS
    	if(ImCamOpen(Para.iPhysicalNo, true, false, NULL , IMC_VIEW_STATIC, false, IMC_GRAB_RAW, 0, IMC_BAYER_GB_RG) != IMC_SUCCESS)
    {
    		ShowMessage("imi cam can't open");
    		return false;
    }

    int   iWidth , iHeight ,iBit;
    if(ImCamGetImageSize(Para.iPhysicalNo,&iWidth , &iHeight) != IMC_SUCCESS ||
       ImCamGetDataBits (Para.iPhysicalNo,&iBit             ) != IMC_SUCCESS ){
        iWidth  = 640 ; iHeight = 480 ; iBit    = 8   ;
        ShowMessage("imi cam Can't Get ImageSize");
    }
    m_iImgWidth  = iWidth  ;
    m_iImgHeight = iHeight ;
    m_iImgBit    = iBit    ;

    m_pImgRGBBuf = new unsigned char [m_iImgWidth * m_iImgHeight * 3] ;



    ImCamStart(Para.iPhysicalNo);

    SetTrgMode(tmSw);

    //
    //���� �������� ������ ApplyUserPara�Լ����� ������ �ȸԾ �Ϻη� ó���� �����Ҽ� ���� ������ �־��.
    SetUserPara.iBrightness = -1 ;
    SetUserPara.iSharpness  = -1 ;
    SetUserPara.iGamma      = -1 ;
    SetUserPara.iExposure   = -1 ;
    SetUserPara.iShutter    = -1 ;
    SetUserPara.iGain       = -1 ;
    SetUserPara.iPan        = -1 ;
    SetUserPara.iTilt       = -1 ;

    IMC_TRIGGER_INFO TrigInfo;
    memset(&TrigInfo, 0, sizeof(IMC_TRIGGER_INFO));
    if (ImCamGetTriggerInfo(Para.iPhysicalNo, &TrigInfo) != IMC_SUCCESS ) {
    	ShowMessage("This Camera doesn't support Trigger Function!.");
    	return false;
    }
    return true ;
}

ECamType CImi::GetType()
{
    return ctImi ;
}

bool CImi::GetImg(unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt)
{
    _pImg    = m_pImgBuf    ;
    _iWidth  = m_iImgWidth  ;
    _iHeight = m_iImgHeight ;
    _iBitCnt = m_iImgBit    ;

    return true ;
}

void CImi::LoadPara(bool _bLoad , String _sPath)
{
    //Local Var.
    TUserINI   UserINI;
    String sPath  ;
    String sTemp  ;

    //Set Dir.
    sTemp = String(m_iCamNo)+"_ImiCamera" ;
    sPath = _sPath;

    //if(FileExists(sPath)

    if(_bLoad) {
        //Physical Port. ���̿����� ���� �ø��� ������ ������ �Ǵ� �Ͱ���.. ��Ʈ �ٲ㲸�� ������ �ȹٲ�.
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iPhysicalNo"     , Para.iPhysicalNo     );

        //Load Device.
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iVideoFormat"    , Para.iVideoFormat    ); //;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iVideoMode  "    , Para.iVideoMode      ); //;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iFrameRate  "    , Para.iFrameRate      ); //;2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60
    }
    else {
        //Save Device.
        //UserINI.ClearFile(sPath) ;

        //Physical Port. ���̿����� ���� �ø��� ������ ������ �Ǵ� �Ͱ���.. ��Ʈ �ٲ㲸�� ������ �ȹٲ�.
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iPhysicalNo"     , Para.iPhysicalNo     );

        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iVideoFormat"    , Para.iVideoFormat    ); //;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iVideoMode  "    , Para.iVideoMode      ); //;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iFrameRate  "    , Para.iFrameRate      ); //;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60

    }


}

bool CImi::Close()
{

    if(m_bDllLoaded){
        ImCamSetCallback(Para.iPhysicalNo, NULL,NULL);
        ImCamStop       (Para.iPhysicalNo           );
        ImCamClose      (Para.iPhysicalNo           );
    }

    if(m_pImgRGBBuf) {
        delete [] m_pImgRGBBuf ;
        m_pImgRGBBuf = NULL ;
    }


    return true ;
}

bool CImi::Grab()
{
    if (ImCamGenerateSWTrigger(Para.iPhysicalNo) != IMC_SUCCESS ) {
	return false;
    }
    m_dGrabOnTime = GetTickCount() ;

    return true ;
}

bool CImi::SetTrgMode(ETrgMode _iMode)
{
    IMC_TRIGGER_PROPERTY	TrigProp;
    memset(&TrigProp, 0, sizeof(IMC_TRIGGER_PROPERTY));
    //TrigProp.nTriggerMode = 0;
    //TrigProp.nTriggerParam = 0;

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

    if ( ImCamSetTriggerMode(Para.iPhysicalNo, TrigProp) != IMC_SUCCESS ) {
        //�̻��ϰ� �������Ϸ� �ϸ� �޼����� ��.
    	//ShowMessage("This Camera doesn't support Trigger Function!..");
    	return false;
    }
    return true ;
}

void CImi::UpdateUserPara  (bool _bToTable , TValueListEditor * _vePara,TCamUserPara * _pCamUserPara)
{
    TUserPara * pUserPara = (TUserPara *)_pCamUserPara ;
    if(_bToTable) {
        UpdateListPara(_bToTable , _vePara , &pUserPara->List);
    }
    else {
        UpdateListPara(_bToTable , _vePara , &pUserPara->List);
    }














/*
    if(_bToTable) {
        _vePara -> Values["iBrightness"] = pUserPara->iBrightness;
        _vePara -> Values["iSharpness "] = pUserPara->iSharpness ;
        _vePara -> Values["iGamma     "] = pUserPara->iGamma     ;
        _vePara -> Values["iShutter   "] = pUserPara->iShutter   ;
        _vePara -> Values["iGain      "] = pUserPara->iGain      ;

        _vePara -> Values["iPan       "] = pUserPara->iPan       ;
        _vePara -> Values["iTilt      "] = pUserPara->iTilt      ;
        _vePara -> Values["iExposure  "] = pUserPara->iExposure  ;

        //iBrightness = 0   ;//0~1023
        //iSharpness  = 0   ;//0~1016
        //iGamma      = 4   ;//4~25
        //iExposure   = 0   ;//0~100
        //iShutter    = 400 ;//1~2000
        //iGain       = 0   ;//0~511
        //iPan        = 0   ;//0~1600
        //iTilt       = 0   ;//0~1230
    }
    else {
        pUserPara->iBrightness=StrToIntDef(_vePara -> Values["iBrightness"],pUserPara->iBrightness) ;
        pUserPara->iSharpness =StrToIntDef(_vePara -> Values["iSharpness "],pUserPara->iSharpness ) ;
        pUserPara->iGamma     =StrToIntDef(_vePara -> Values["iGamma     "],pUserPara->iGamma     ) ;
        pUserPara->iShutter   =StrToIntDef(_vePara -> Values["iShutter   "],pUserPara->iShutter   ) ;
        pUserPara->iGain      =StrToIntDef(_vePara -> Values["iGain      "],pUserPara->iGain      ) ;

        pUserPara->iPan       =StrToIntDef(_vePara -> Values["iPan       "],pUserPara->iPan       ) ;
        pUserPara->iTilt      =StrToIntDef(_vePara -> Values["iTilt      "],pUserPara->iTilt      ) ;
        pUserPara->iExposure  =StrToIntDef(_vePara -> Values["iExposure  "],pUserPara->iExposure  ) ;

    }   */

}

void CImi::LoadSaveUserPara(bool _bLoad , String _sFilePath,TCamUserPara * _pCamUserPara)
{
//CCam::m_apCam[MPara.iCamId] -> LoadSaveUserPara(_bLoad , sIniPath , CamPara);
    TUserPara * pUserPara = (TUserPara *)_pCamUserPara ;
    if(_bLoad) {
        if(FileExists(_sFilePath)){
            LoadSaveList(_bLoad , _sFilePath , "ImiUserPara" , &pUserPara->List);
        }
        else {
            pUserPara->SetDefault();
        }
    }
    else {
        LoadSaveList(_bLoad , _sFilePath , "ImiUserPara" , &pUserPara->List);
    }





/*
    TUserINI Ini ;
    TUserPara * pUserPara = (TUserPara *)_pCamUserPara ;
    if(_bLoad) {
        Ini.Load(_sFilePath, "ImiUserPara" , "iBrightness", pUserPara->iBrightness);
        Ini.Load(_sFilePath, "ImiUserPara" , "iSharpness ", pUserPara->iSharpness );
        Ini.Load(_sFilePath, "ImiUserPara" , "iGamma     ", pUserPara->iGamma     );
        Ini.Load(_sFilePath, "ImiUserPara" , "iExposure  ", pUserPara->iExposure  );
        Ini.Load(_sFilePath, "ImiUserPara" , "iShutter   ", pUserPara->iShutter   );
        Ini.Load(_sFilePath, "ImiUserPara" , "iGain      ", pUserPara->iGain      );
        Ini.Load(_sFilePath, "ImiUserPara" , "iPan       ", pUserPara->iPan       );
        Ini.Load(_sFilePath, "ImiUserPara" , "iTilt      ", pUserPara->iTilt      );
    }
    else {
        Ini.Save(_sFilePath, "ImiUserPara" , "iBrightness", pUserPara->iBrightness);
        Ini.Save(_sFilePath, "ImiUserPara" , "iSharpness ", pUserPara->iSharpness );
        Ini.Save(_sFilePath, "ImiUserPara" , "iGamma     ", pUserPara->iGamma     );
        Ini.Save(_sFilePath, "ImiUserPara" , "iExposure  ", pUserPara->iExposure  );
        Ini.Save(_sFilePath, "ImiUserPara" , "iShutter   ", pUserPara->iShutter   );
        Ini.Save(_sFilePath, "ImiUserPara" , "iGain      ", pUserPara->iGain      );
        Ini.Save(_sFilePath, "ImiUserPara" , "iPan       ", pUserPara->iPan       );
        Ini.Save(_sFilePath, "ImiUserPara" , "iTilt      ", pUserPara->iTilt      );
    }
*/
}

void CImi::SetGrabFunc(CamCallbackFunc _pFunc)
{
    m_pCallbackFunc = _pFunc ;

}




double CImi::GetGrabTime()
{
     return m_dGrabTime ;

}











