#ifndef VISNDEFINEH
#define VISNDEFINEH
//


//ī�޶��� ����.
enum EN_CAM_ID {
    ciOri  = 0 , //���������̼� ����.
    ciWfr      , //������ ����.
    ciRal      , //���� ����.

    MAX_CAM_ID
};

//���� ��Ʈ�ѷ� ����.
enum EN_LIGHT_ID {
    liAll = 0 ,    //6ä�� 1���� �پ�.

    MAX_LIGHT_ID
};

enum EN_VISN_ID {
    viNone       =-1 ,

    viOri        = 0 , //��������.
    viWfr            , //������ Ĩ �簢��.
    viRbf            , //������ ���� ���� ��ġ.
    viRat            , //������ ���� �˻�         viRat
    viWfo            , //������ ù����.           viWfo

    MAX_VISN_ID
};
//extern AnsiString g_sVisnName[MAX_VISN_ID];

enum EN_VIEW_MODE {
    vmNone  = -1 , //�ƹ��׸��� �ȱ׸� �̹����� ��.
    vmCal   = -2 , //�̸��극�̼� �̹���.
    vmAll   = -3 , //��� �˻� ������� ���÷���.
    vmRslt  =  0   //0~~ 0�̻��� �� ProcPkg ������� ����.
};

/*������
viOri    = 0:Pass , 1:OCV Fail
viWfr    = 0:Pass , 1:RCT Fail , 2: Nodevice or crack 3:FailDevice  4:Standard Device (Mapmode)
viRbf    = 0:Pass , 1:LCP Fail , 2: Epoxy Fail
viRat    = 0:Pass , 1:LCP Fail , 2: Chip Place Fail
viWfo    = 0:Pass , 1:OCV Fail

�ٲ۰�.
viOri    = 0:Pass , 1:OCV Fail , 2: OCV NG
viWfr    = 0:Pass , 1:RCT Fail , 2: RCT NG   3:RCT ANGLE NG  4:CRACK CHIP     5:TSD Fail      6:DOT Chip or Standard Device (Mapmode)
viRbf    = 0:Pass , 1:TSD Fail , 2: NoPKG    3:LCP Fail      4:TSD Fail       5:EPOXY NG
viRat    = 0:Pass , 1:LCP Fail , 2: RCT Fail 3:RCT LENGTH NG 4:Rct Offset     5:Rct Angle
viWfo    = 0:Pass , 1:OCV Fail   2: OCV NG
*/
//�׳� �̳� �ϳ� �����.

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
    bool  bInspEnd ; //�˻� ���� ����.

    EN_VISN_ERR_ID iRet   ;  //��� ��...
    float fRsltX ;  //���� X��.
    float fRsltY ;  //���� Y��.
    float fRsltT ;  //���� T��.
};



enum EN_OPER_LEVEL {
    olOper     = 0 ,
    olEngineer = 1 ,
    olMaster   = 2 
};

#endif



