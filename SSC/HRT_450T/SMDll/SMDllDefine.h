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
    adLR = 0 , //���鿡��   ������ Left �� - Right�� +
    adRL     , //���鿡��   ������ Right�� - Left �� +
    adBF     , //���鿡��   ������ Bwd  �� - Fwd  �� +
    adFB     , //���鿡��   ������ Fwd  �� - Bwd  �� +
    adUD     , //���鿡��   ������ Up   �� - Down �� +
    adDU     , //���鿡��   ������ Down �� - Up   �� +
    adCA     , //ȸ���࿡�� ������ Clock�� - AntiC�� +
    adAC       //ȸ���࿡�� ������ AntiC�� - Clock�� +
};

//Motor
enum EN_MTOR_DIRECTION {
    mdLR = 0 , //���鿡��   ������ Left �� - Right�� +
    mdRL     , //���鿡��   ������ Right�� - Left �� +
    mdBF     , //���鿡��   ������ Bwd  �� - Fwd  �� +
    mdFB     , //���鿡��   ������ Fwd  �� - Bwd  �� +
    mdUD     , //���鿡��   ������ Up   �� - Down �� +
    mdDU     , //���鿡��   ������ Down �� - Up   �� +
    mdCA     , //ȸ���࿡�� ������ Clock�� - AntiC�� +
    mdAC       //ȸ���࿡�� ������ AntiC�� - Clock�� +
};

typedef void (__stdcall * PosCallbackFunc)(long _lAxis, DWORD _dwFlag);
#endif

