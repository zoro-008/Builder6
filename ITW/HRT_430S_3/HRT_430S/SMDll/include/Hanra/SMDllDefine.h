//---------------------------------------------------------------------------

#ifndef SMDLLDEFINEH
#define SMDLLDEFINEH

enum EN_LEVEL{
    lvOperator=0 ,
    lvEngineer   ,
    lvMaster     ,

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

#endif

