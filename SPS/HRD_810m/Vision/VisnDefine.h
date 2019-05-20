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


//ī�޶��� ����.
enum EN_CAM_ID {
    ciWfr  = 0 , //������ �˻� ī�޶�..
    ciEnd  = 1 , //�۾� �Ϸ� ��� ī�޶�. �˻��ϴ� ������ �ƴϰ� ���÷��� ���� ���� ����.. FormEndImg.

    MAX_CAM_ID
};

//���� ��Ʈ�ѷ� ����.
enum EN_LIGHT_ID {
    liWfr = 0 ,    //���� 3ä�� 1���� �پ�.

    MAX_LIGHT_ID
};

enum EN_VISN_ID {
    viAll        =-1 ,
    viWfr        = 0 , //Ĩ
    viWfo            , //�Ǵϼȸ�ũ
    viWfp            , //�� �˻�.
    viCal            , //Ķ���극�̼�.

    MAX_VISN_ID
};
enum EN_INSP_ID { //MAX_VISN_ID ����.
    iiWfrRct = 0 ,    //���� Position
    iiWfrOcv1= 1 ,    //���� ����
    iiWfrOcv2= 2 ,    //���� ����2
    MAX_WFR_INSP ,

//  iiWfo    = 0 ,    //�Ǵϼ� ��ũ ���������̼�
//  MAX_WFO_INSP ,

    iiWfoRct = 0 ,    //���� Position
    iiWfoOcv1= 1 ,    //���� ����
    iiWfoOcv2= 2 ,    //���� ����2
    MAX_WFO_INSP ,

    iiWfpTsd = 0 ,    //�� ��ŷ �˻�.
    MAX_WFP_INSP ,

    iiCalCal    = 0 ,    //Ķ���극�̼�
    MAX_CAL_INSP
};
//extern AnsiString g_sVisnName[MAX_VISN_ID];

enum EN_VIEW_MODE {
    vmNone  = -1 , //�ƹ��׸��� �ȱ׸� �̹����� ��.
    vmCal   = -2 , //�̸��극�̼� �̹���.
    vmAll   = -3 , //��� �˻� ������� ���÷���.
    vmRslt  =  0   //0~~ 0�̻��� �� ProcPkg ������� ����.
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
    bool  bInspEnd ; //�˻� ���� ����.

    EN_VISN_ERR_ID iRet   ;  //��� ��...
    float fRsltX ;  //���� X��.
    float fRsltY ;  //���� Y��.
    float fRsltT ;  //���� T��.
};










#endif



