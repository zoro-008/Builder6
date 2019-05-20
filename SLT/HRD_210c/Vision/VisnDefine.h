#ifndef VISNDEFINEH
#define VISNDEFINEH
//


//카메라의 갯수.
enum EN_CAM_ID {
    ciOri  = 0 , //오리엔테이션 비젼.
    ciWfr      , //웨이퍼 비젼.
    ciRal      , //레일 비젼.

    MAX_CAM_ID
};

//조명 컨트롤러 갯수.
enum EN_LIGHT_ID {
    liAll = 0 ,    //6채널 1개로 다씀.

    MAX_LIGHT_ID
};

enum EN_VISN_ID {
    viNone       =-1 ,

    viOri        = 0 , //자제방향.
    viWfr            , //웨이퍼 칩 사각형.
    viRbf            , //웨이퍼 내려 놓는 위치.
    viRat            , //웨이퍼 안착 검사         viRat
    viWfo            , //웨이퍼 첫자제.           viWfo

    MAX_VISN_ID
};
//extern AnsiString g_sVisnName[MAX_VISN_ID];

enum EN_VIEW_MODE {
    vmNone  = -1 , //아무그림도 안그림 이미지만 뜸.
    vmCal   = -2 , //켈리브레이션 이미지.
    vmAll   = -3 , //모든 검사 결과값만 디스플레이.
    vmRslt  =  0   //0~~ 0이상은 각 ProcPkg 순서대로 간다.
};

/*원래꺼
viOri    = 0:Pass , 1:OCV Fail
viWfr    = 0:Pass , 1:RCT Fail , 2: Nodevice or crack 3:FailDevice  4:Standard Device (Mapmode)
viRbf    = 0:Pass , 1:LCP Fail , 2: Epoxy Fail
viRat    = 0:Pass , 1:LCP Fail , 2: Chip Place Fail
viWfo    = 0:Pass , 1:OCV Fail

바꾼거.
viOri    = 0:Pass , 1:OCV Fail , 2: OCV NG
viWfr    = 0:Pass , 1:RCT Fail , 2: RCT NG   3:RCT ANGLE NG  4:CRACK CHIP     5:TSD Fail      6:DOT Chip or Standard Device (Mapmode)
viRbf    = 0:Pass , 1:TSD Fail , 2: NoPKG    3:LCP Fail      4:TSD Fail       5:EPOXY NG
viRat    = 0:Pass , 1:LCP Fail , 2: RCT Fail 3:RCT LENGTH NG 4:Rct Offset     5:Rct Angle
viWfo    = 0:Pass , 1:OCV Fail   2: OCV NG
*/
//그냥 이넘 하나 만든다.

enum EN_VISN_ERR_ID {
    veOriOk       = 0 ,
    veOriOcvFail  = 1 ,
    veOriOcv      = 2 ,

    veWfrOk       = 0 ,
    veWfrRctFail  = 1 ,
    veWfrRct      = 2 ,
    veWfrAngle    = 3 ,
    veWfrCrack    = 4 ,
    veWfrTsdFail  = 5 ,
    veWfrDot      = 6 ,
    veWfrNoPkg    = 7 ,

    veRbfOk       = 0 ,
    veRbfTsdFail  = 1 ,
    veRbfNoPkg    = 2 ,
    veRbfLCPFail  = 3 ,
    veRbfTsdFail2 = 4 ,
    veRbfEpoxy    = 5 ,

    veRatOk       = 0 ,
    veRatLcpFail  = 1 ,
    veRatRctFail  = 2 ,
    veRatLen      = 3 ,
    veRatRctOfs   = 4 ,
    veRatRctAng   = 5 ,

    veWfoOk       = 0 ,
    veWfoOcvFail  = 1 ,
    veWfoOcv      = 2 ,
};


struct TRetResult {
    bool  bInspEnd ; //검사 끝남 변수.

    EN_VISN_ERR_ID iRet   ;  //결과 값...
    float fRsltX ;  //보정 X값.
    float fRsltY ;  //보정 Y값.
    float fRsltT ;  //보정 T값.
};



enum EN_OPER_LEVEL {
    olOper     = 0 ,
    olEngineer = 1 ,
    olMaster   = 2 
};

#endif



