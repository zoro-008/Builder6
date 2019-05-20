#ifndef VISNDEFINEH
#define VISNDEFINEH

//�ڵ鷯���� ��Ŭ��� �ؼ� ��� �� �� ��....��� �� �͵�.....


enum EN_VISN_ID {
    viNone   =-1 ,
    viPkg        , //��Ű�� ���� Ȯ�� ����

    MAX_VISN_ID
};

enum EN_PAGE_ID { //�� ������ �˻� ���̵�..
    //viEpx
    piPkgLCS     = 0 , //��Ű�� ���� ķ,����Ʈ ����.
    piPkgOcv     = 1 , //��Ű�� ���� ���������̼�
    piPkgTsd     = 2 , //��Ű�� ���� ������ �������
    MAX_PACKAGE_INSP

};

enum EN_TRAIN_PAGE_ID {
    tpPkgSet     = 0 , //��Ű�� Ʈ��Ŀ ��ġ �� �࿭ ���� ���� ����.

    MAX_TRAIN_PAGE_ID
};

//�׳� �̳� �ϳ� �����.
enum EN_VISN_ERR_ID {
    veOk          = 0 ,

    vePkgOk       = 0 , //�˻��.
    vePkgOriFail  = 1 , //�˻� ����
    vePkgOriNg    = 2 , //�˻� ��� NG
    vePkgInspFail = 3 ,
    vePkgInspNg   = 4 ,

};

#define MAX_INSP_ROW 20
#define MAX_INSP_COL 20
struct TRetResult {
    bool  bInspEnd ; //�˻� ���� ����.

    EN_VISN_ERR_ID iRet   ;  //��� ��...
    int   iFailCnt        ;  //���� ����.
    bool  bFailMap[MAX_INSP_ROW][MAX_INSP_COL] ;//1�� ����.
};

enum EN_OPER_LEVEL {
    olOper     = 0 ,
    olEngineer = 1 ,
    olMaster   = 2
};

#endif



