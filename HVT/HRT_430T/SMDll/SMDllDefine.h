//---------------------------------------------------------------------------

#ifndef SMDLLDEFINEH
#define SMDLLDEFINEH

enum EN_LEVEL{
    lvOperator=0 ,
    lvEngineer   ,
    lvMaster     ,
//    lvContol     ,

    MAX_LEVEL
};

enum EN_SEQ_STAT {
    ssInit      = 0 ,
    ssWarning       ,
    ssError         ,
    ssRunning       ,
    ssStop          ,
    ssMaint         ,
    ssRunWarn       ,
    ssWorkEnd       ,

    MAX_SEQ_STAT
};

enum EN_LAN_SEL
{
    lsEnglish  = 0 ,
    lsKorean       ,
    lsChinese      ,

    MAX_LAN_SEL
};

#endif

