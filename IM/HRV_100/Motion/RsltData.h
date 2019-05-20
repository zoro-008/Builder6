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

struct TVisnRslt { //���� �����.  ������ �ȼ� �����̰� �̹��� ���� ���� 0,0 ��.
    //�����ϱ����� ������. 20150211 IM��û �߰�.
    //�̳�� �������� �����V�� �����Ͽ�  ���� ���� �����V�� ���� �Ͽ� ��ä�� �����δ�.
    double dBfCttX    ;
    double dBfCttY    ;
    TRect  BfCttRect  ;

    double dBfSttOfsX ; //�������� ���� �����V...
    double dBfSttOfsY ;

    //�����ϰ� �ʱⰪ �϶��� ������
    double dSttCntX   ;
    double dSttCntY   ;
    TRect  SttRect    ;


    //20150430 �⵿̱ �븮 ��û���� ���� �����ϰ� �������� ������ ����̺� ƿƮ����
    //��ŸƮ �������� �־� ��ü��.
    double dDriveSttX  ;
    double dDriveSttY  ;
    TRect  DriveSttRect;



    //����ڰ� ����(OM.DevOptn.iDrvTtTestCrnt)�� ma�϶��� ������.
    double dDriveCntX;
    double dDriveCntY ;
    TRect  DriveRect  ;

    //�ְ� ���� ma�϶� ������.
    double dFullTiltX ;
    double dFullTiltY ;
    TRect  FullRect  ;

    //�ʱ���ġ���� ���� ũ�� ���̳��°�.
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

struct TVisnRsltPnt {  //���� 50�� �����ÿ� 50���� �� ����Ʈ
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
//���� ��� ���� ������ ȭ�鿡�� ���� �ش�.
//���� �����͵��� �̿� �Ͽ� ���� OK/NG �����͸� �̾� ����.
struct TTiltData {
    double dTiltInitR    ; //0ma�־������ʱ� ƿƮ ��ġ X
    double dTiltInitT    ; //0ma�־������ʱ� ƿƮ ��ġ Y
    double dTiltInit     ; //0ma�־����� ����ġ.

    double dTiltFull     ; //dTiltInit ���� ����ū���� mA�� �־����� ��ġ����
    double dTiltDrive    ; //dTiltInit ���� Ư�� ����mA���� ������ ����ġ ���� �� ����.
    double dTiltDriveAbs ; //ī�޶� ���Ϳ��� Ư�� ����mA���� ������ ����ġ ���� �� ����.
    double dTiltMax      ; //�����߿� �ʱ� ��ġ���� ���� ���� ��� ����ġ.
    double dTiltLength   ; //�����߿�  �Ÿ�.

    //bool   bBeamOut      ; //�����߿� �ѹ��� ���� �ν� �ȵȰ��.
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
    double dLinearity    ; //������...
    double dImpedance    ;
    double dHysteresis   ; //1���� ������ 1.2�� �������� ������.
    //double dHysteresis2  ; //1������ OK�ȳ��� ����Ƽ ����Ƽ ���̱� ���� ���� ���� �Ѵ�.
    bool   bCaseShort    ; //���̽� ��Ʈ �ƴ��� ����.
    bool   bOpenOk       ; //��� �׸� ���ִ޶����. ������ ����ڷῡ ��û���� ppt����. ������ �״�� �ΰ� ȭ�鿡�� ���� ����.
    bool   bShortOk      ;
    void Clear(){
        memset(this , 0 , sizeof(TDriveData));
    }
};
extern TDriveData g_DriveData[MAX_STAGE_CNT];

extern bool g_bInspEnd[MAX_STAGE_CNT];





#endif
