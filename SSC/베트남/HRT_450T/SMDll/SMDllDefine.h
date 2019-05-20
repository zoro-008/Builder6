//---------------------------------------------------------------------------

#ifndef SMDLLDEFINEH
#define SMDLLDEFINEH

enum EN_LEVEL{
    lvOperator=0 ,
    lvEngineer   ,
    lvMaster     ,
    lvContol     ,

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

//Actuator.

enum EN_ACTR_DIRECTION {
    adLR = 0 , //정면에서   봤을때 Left 가 - Right가 +
    adRL     , //정면에서   봤을때 Right가 - Left 가 +
    adBF     , //정면에서   봤을때 Bwd  가 - Fwd  가 +
    adFB     , //정면에서   봤을때 Fwd  가 - Bwd  가 +
    adUD     , //정면에서   봤을때 Up   가 - Down 가 +
    adDU     , //정면에서   봤을때 Down 가 - Up   가 +
    adCA     , //회전축에서 봤을때 Clock가 - AntiC가 +
    adAC       //회전축에서 봤을때 AntiC가 - Clock가 +
};

//Motor
enum EN_MTOR_DIRECTION {
    mdLR = 0 , //정면에서   봤을때 Left 가 - Right가 +
    mdRL     , //정면에서   봤을때 Right가 - Left 가 +
    mdBF     , //정면에서   봤을때 Bwd  가 - Fwd  가 +
    mdFB     , //정면에서   봤을때 Fwd  가 - Bwd  가 +
    mdUD     , //정면에서   봤을때 Up   가 - Down 가 +
    mdDU     , //정면에서   봤을때 Down 가 - Up   가 +
    mdCA     , //회전축에서 봤을때 Clock가 - AntiC가 +
    mdAC       //회전축에서 봤을때 AntiC가 - Clock가 +
};

typedef void (__stdcall * PosCallbackFunc)(long _lAxis, DWORD _dwFlag);
#endif

