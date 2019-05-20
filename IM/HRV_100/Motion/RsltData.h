//---------------------------------------------------------------------------

#ifndef RsltDataH
#define RsltDataH
//---------------------------------------------------------------------------
#include "SVDefine.h"
#include "UtilDefine.h"

////////////////////////

struct TPointData {
    double x;
    double y;
    TPointData(){Clear();}
    ~TPointData(){}

    void Clear(){
        memset(this , 0 , sizeof(TPointData));
    }
};

struct TVisnRslt { //비젼 결과값.  단위는 픽셀 단위이고 이미지 왼쪽 위가 0,0 임.
    //컨텍하기전에 측정값. 20150211 IM요청 추가.
    //이놈과 컴텍후의 오프셑을 측정하여  이후 영상에 오프셑을 적용 하여 통채로 움직인다.
    double dBfCttX    ;
    double dBfCttY    ;
    TRect  BfCttRect  ;

    double dBfSttOfsX ; //컨텍전과 후의 오프셑...
    double dBfSttOfsY ;

    //컨텍하고 초기값 일때의 측정값
    double dSttCntX   ;
    double dSttCntY   ;
    TRect  SttRect    ;


    //20150430 이기동 대리 요청으로 원래 컨택하고 나서부터 측정한 드라이브 틸트값을
    //스타트 포지션을 넣어 대체함.
    double dDriveSttX  ;
    double dDriveSttY  ;
    TRect  DriveSttRect;



    //사용자가 세팅(OM.DevOptn.iDrvTtTestCrnt)한 ma일때의 측정값.
    double dDriveCntX;
    double dDriveCntY ;
    TRect  DriveRect  ;

    //최고 높은 ma일때 측정값.
    double dFullTiltX ;
    double dFullTiltY ;
    TRect  FullRect  ;

    //초기위치에서 가장 크게 차이나는값.
    double dMaxCntX   ;
    double dMaxCntY   ;
    TRect  MaxRect    ;

    TVisnRslt(){
        Clear();
    }
    ~TVisnRslt(){}

    void Clear(){
        memset(this , 0 , sizeof(TVisnRslt));
    }
};
extern TVisnRslt g_VisnRslt[MAX_STAGE_CNT];

///////////////////////////////////////

struct TVisnRsltPnt {  //비젼 50번 측정시에 50개의 빔 포인트
    int iMaxDataCnt ;
    int iDataCnt ;
    TPointData * tData ;

    TVisnRsltPnt(){tData = NULL ; }
    ~TVisnRsltPnt(){Delete();}

    void Delete(){
        if(tData){
            delete [] tData ;
            tData = NULL ;
            iMaxDataCnt = 0 ;
            iDataCnt    = 0 ;
        }
    }

    void SetMaxData(int _iDataCnt ){
        Delete();

        tData = new TPointData [_iDataCnt] ;
        memset(tData , 0 , sizeof(TPointData)*_iDataCnt);
        iMaxDataCnt = _iDataCnt ;
        iDataCnt = 0 ;

    }

    void PushData(TPointData _tData){
        if(iDataCnt >= iMaxDataCnt) {
            return ;
        }
        tData[iDataCnt] = _tData ;
        iDataCnt++;
    }
};
extern TVisnRsltPnt g_VisnRsltPnt[MAX_STAGE_CNT];



struct THeightRslt {
    int iMaxIncDataCnt ;
    int iIncDataCnt ;
    TPointData * tIncData ;

    int iMaxDecDataCnt ;
    int iDecDataCnt ;
    TPointData * tDecData ;

    THeightRslt(){tIncData = NULL ; tDecData = NULL ;}
    ~THeightRslt(){Delete();}

    void Delete(){
        if(tIncData){
            delete [] tIncData ;
            tIncData = NULL ;
            iMaxIncDataCnt = 0 ;
            iIncDataCnt = 0 ;
        }
        if(tDecData){
            delete [] tDecData ;
            tDecData = NULL ;
            iMaxDecDataCnt = 0 ;
            iDecDataCnt = 0 ;
        }

    }

    void SetMaxData(int _iIncDataCnt , int _iDecDataCnt){
        Delete();

        tIncData = new TPointData [_iIncDataCnt] ;
        memset(tIncData , 0 , sizeof(TPointData)*_iIncDataCnt);
        iMaxIncDataCnt = _iIncDataCnt ;
        iIncDataCnt = 0 ;

        tDecData = new TPointData [_iDecDataCnt] ;
        memset(tDecData , 0 , sizeof(TPointData)*_iDecDataCnt);
        iMaxDecDataCnt = _iDecDataCnt ;
        iDecDataCnt = 0 ;
    }

    void PushIncData(TPointData _tData){
        if(iIncDataCnt >= iMaxIncDataCnt) {
            return ;
        }
        tIncData[iIncDataCnt] = _tData ;
        iIncDataCnt++;
    }
    void PushDecData(TPointData _tData){
        if(iDecDataCnt >= iMaxDecDataCnt) {
            return ;
        }
        tDecData[iDecDataCnt] = _tData ;
        iDecDataCnt++;
    }
};
extern THeightRslt g_HeightRslt[MAX_STAGE_CNT];



/////////////////////-====================
//최종 양부 판정 데이터 화면에도 보여 준다.
//위의 데이터들을 이용 하여 최종 OK/NG 데이터를 뽑아 낸다.
struct TTiltData {
    double dTiltInitR    ; //0ma넣었을때초기 틸트 위치 X
    double dTiltInitT    ; //0ma넣었을때초기 틸트 위치 Y
    double dTiltInit     ; //0ma넣었을때 빔위치.

    double dTiltFull     ; //dTiltInit 에서 가장큰값의 mA를 넣었을때 위치차이
    double dTiltDrive    ; //dTiltInit 에서 특정 설정mA에서 측정한 빔위치 차이 를 구함.
    double dTiltDriveAbs ; //카메라 센터에서 특정 설정mA에서 측정한 빔위치 차이 를 구함.
    double dTiltMax      ; //구동중에 초기 위치에서 가장 많이 벋어난 빔위치.
    double dTiltLength   ; //구동중에  거리.

    //bool   bBeamOut      ; //구동중에 한번도 빔이 인식 안된경우.
    void Clear(){
        memset(this , 0 , sizeof(TTiltData));
        //bBeamOut = true ;
    }
};
extern TTiltData g_TiltData[MAX_STAGE_CNT];

struct TDriveData{
    double dPreLoad      ;
    double dPeakStroke   ;
    double dCustomStroke ;
    double dSensitivity  ;
    double dLinearity    ; //진직도...
    double dImpedance    ;
    double dHysteresis   ; //1차로 전구간 1.2배 스펙으로 측정후.
    //double dHysteresis2  ; //1차에서 OK된놈은 리피티 빌리티 높이기 위해 구간 측정 한다.
    bool   bCaseShort    ; //케이스 쇼트 됐는지 여부.
    bool   bOpenOk       ; //요거 항목 없애달라고함. 서버에 장비자료에 요청사항 ppt참조. 기존거 그대로 두고 화면에만 디피 안함.
    bool   bShortOk      ;
    void Clear(){
        memset(this , 0 , sizeof(TDriveData));
    }
};
extern TDriveData g_DriveData[MAX_STAGE_CNT];

extern bool g_bInspEnd[MAX_STAGE_CNT];





#endif
