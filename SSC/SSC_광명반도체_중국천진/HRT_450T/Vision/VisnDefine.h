#ifndef VISNDEFINEH
#define VISNDEFINEH

//�ڵ鷯���� ��Ŭ��� �ؼ� ��� �� �� ��....��� �� �͵�.....


enum EN_VISN_ID {
    viNone   =-1 ,
    viOri        , //PKG ���������̼�.

    MAX_VISN_ID
};

enum EN_PAGE_ID { //�� ������ �˻� ���̵�..
    //viEpx
    piOriLCS     = 0 , //���� ķ,����Ʈ ����.
    piOriOcv     = 1 , //���� ���������̼�
    MAX_ORI_INSP     ,

};

enum EN_TRAIN_PAGE_ID {
    tpOriSet     = 0 , //���� Ʈ���� ������.

    MAX_TRAIN_PAGE_ID
};

//�׳� �̳� �ϳ� �����.
enum EN_VISN_ERR_ID {
    veOk       = 0 ,

    veOriOk       = 0 , //�˻��.
    veOriOcvFail  = 1 , //�˻� ����
    veOriOcvNg    = 2 , //�˻� ��� NG
};

#define MAX_INSP_ROW 20
#define MAX_INSP_COL 20
struct TRetResult {
    bool  bInspEnd ; //�˻� ���� ����.

    EN_VISN_ERR_ID iRet   ;  //��� ��...
    //int   iFailCnt        ;  //���� ����.
    //bool  bFailMap[MAX_INSP_ROW][MAX_INSP_COL] ;//1�� ����.
};

enum EN_OPER_LEVEL {
    olOper     = 0 ,
    olEngineer = 1 ,
    olMaster   = 2
};

#endif



