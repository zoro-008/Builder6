//---------------------------------------------------------------------------

#ifndef SADLLDEFINEH
#define SADLLDEFINEH

enum SA_LEVEL{
    lvOperatr=0 ,
    lvEnginr    ,
    lvMastr     ,
//    lvContol     ,

    MAX_SA_LEVEL
};

enum SA_SEQ_STAT {
    saInit      = 0 ,
    saWarning       ,
    saError         ,
    saRunning       ,
    saStop          ,
    saMaint         ,
    saRunWarn       ,
    saWorkEnd       ,

    MAX_SA_STAT
};

#endif

