#ifndef VISNDEFINEH
#define VISNDEFINEH

//핸들러에서 인클루드 해서 써야 할 것 들....써야 될 것들.....


enum EN_VISN_ID {
    viNone   =-1 ,
    viPkg        , //패키지 도팅 확인 비젼

    MAX_VISN_ID
};

enum EN_PAGE_ID { //각 비젼별 검사 아이디..
    //viEpx
    piPkgLCS     = 0 , //패키지 비젼 캠,라이트 세팅.
    piPkgOcv     = 1 , //패키지 비젼 오리엔테이션
    piPkgTsd     = 2 , //패키지 비젼 에폭시 쓰레숄드
    MAX_PACKAGE_INSP

};

enum EN_TRAIN_PAGE_ID {
    tpPkgSet     = 0 , //패키지 트렉커 위치 및 행열 간격 갯수 세팅.

    MAX_TRAIN_PAGE_ID
};

//그냥 이넘 하나 만든다.
enum EN_VISN_ERR_ID {
    veOk          = 0 ,

    vePkgOk       = 0 , //검사굿.
    vePkgOriFail  = 1 , //검사 실패
    vePkgOriNg    = 2 , //검사 결과 NG
    vePkgInspFail = 3 ,
    vePkgInspNg   = 4 ,

};

#define MAX_INSP_ROW 20
#define MAX_INSP_COL 20
struct TRetResult {
    bool  bInspEnd ; //검사 끝남 변수.

    EN_VISN_ERR_ID iRet   ;  //결과 값...
    int   iFailCnt        ;  //페일 갯수.
    bool  bFailMap[MAX_INSP_ROW][MAX_INSP_COL] ;//1이 페일.
};

enum EN_OPER_LEVEL {
    olOper     = 0 ,
    olEngineer = 1 ,
    olMaster   = 2
};

#endif



