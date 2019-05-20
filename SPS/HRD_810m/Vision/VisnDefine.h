#ifndef VISNDEFINEH
#define VISNDEFINEH
#include <system.hpp>

extern AnsiString g_sJobFileName ;
extern AnsiString g_sJobFilePath ;
extern bool       g_bSettingMode ;

extern bool       g_bNeedWfrCenter     ;
extern bool       g_bNeedWfrOcv1Train  ;
extern bool       g_bNeedWfrOcv2Train  ;
extern bool       g_bNeedWfrSave       ;
extern bool       g_bNeedWfrOcv1Save   ;
extern bool       g_bNeedWfrOcv2Save   ;

extern bool       g_bNeedWfoCenter     ;
extern bool       g_bNeedWfoOcv1Train  ;
extern bool       g_bNeedWfoOcv2Train  ;
extern bool       g_bNeedWfoSave       ;
extern bool       g_bNeedWfoOcv1Save   ;
extern bool       g_bNeedWfoOcv2Save   ;

extern bool       g_bNeedWfpSave       ;

extern bool       g_bNeedCalSave       ;


//카메라의 갯수.
enum EN_CAM_ID {
    ciWfr  = 0 , //웨이퍼 검사 카메라..
    ciEnd  = 1 , //작업 완료 찍기 카메라. 검사하는 비젼이 아니고 디스플레이 폼도 따로 있음.. FormEndImg.

    MAX_CAM_ID
};

//조명 컨트롤러 갯수.
enum EN_LIGHT_ID {
    liWfr = 0 ,    //동축 3채널 1개로 다씀.

    MAX_LIGHT_ID
};

enum EN_VISN_ID {
    viAll        =-1 ,
    viWfr        = 0 , //칩
    viWfo            , //피니셜마크
    viWfp            , //펜 검사.
    viCal            , //캘리브레이션.

    MAX_VISN_ID
};
enum EN_INSP_ID { //MAX_VISN_ID 기준.
    iiWfrRct = 0 ,    //자제 Position
    iiWfrOcv1= 1 ,    //자제 기준
    iiWfrOcv2= 2 ,    //자제 기준2
    MAX_WFR_INSP ,

//  iiWfo    = 0 ,    //피니셜 마크 오리엔테이션
//  MAX_WFO_INSP ,

    iiWfoRct = 0 ,    //자제 Position
    iiWfoOcv1= 1 ,    //자제 기준
    iiWfoOcv2= 2 ,    //자제 기준2
    MAX_WFO_INSP ,

    iiWfpTsd = 0 ,    //펜 마킹 검사.
    MAX_WFP_INSP ,

    iiCalCal    = 0 ,    //캘리브레이션
    MAX_CAL_INSP
};
//extern AnsiString g_sVisnName[MAX_VISN_ID];

enum EN_VIEW_MODE {
    vmNone  = -1 , //아무그림도 안그림 이미지만 뜸.
    vmCal   = -2 , //켈리브레이션 이미지.
    vmAll   = -3 , //모든 검사 결과값만 디스플레이.
    vmRslt  =  0   //0~~ 0이상은 각 ProcPkg 순서대로 간다.
};

//

enum EN_VISN_ERR_ID {
    veWfrOk       = 0 ,
    veWfrRctFail  = 1 ,
    veWfrRct      = 2 ,
    veWfrAngle    = 3 ,
    veWfrCrack    = 4 ,
    veWfrDot      = 6 ,
    veWfrNoPkg    = 7 ,
    veWfrOcv1Fail = 8 ,
    veWfrOcv1Sinc = 9 ,
    veWfrOcv2Fail = 10,
    veWfrOcv2Sinc = 11,

    veWfoOk       = 0 ,
    veWfoRctFail  = 1 ,
    veWfoRct      = 2 ,
    veWfoAngle    = 3 ,
    veWfoCrack    = 4 ,
    veWfoDot      = 6 ,
    veWfoNoPkg    = 7 ,
    veWfoOcv1Fail = 8 ,
    veWfoOcv1Sinc = 9 ,
    veWfoOcv2Fail = 10,
    veWfoOcv2Sinc = 11,

//    veWfoOk       = 0 ,
//    veWfoOcvFail  = 1 ,
//    veWfoOcvSinc  = 2 ,

    veWfpOk       = 0 ,
    veWfpTsdFail  = 1 ,
    veWfpTsdPxCnt = 2 ,

    veCalOk       = 0 ,
    veCalCalFail  = 1
};


struct TRetResult {
    bool  bInspEnd ; //검사 끝남 변수.

    EN_VISN_ERR_ID iRet   ;  //결과 값...
    float fRsltX ;  //보정 X값.
    float fRsltY ;  //보정 Y값.
    float fRsltT ;  //보정 T값.
};










#endif



