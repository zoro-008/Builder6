#ifndef UTILDEFINEH
#define UTILDEFINEH

//sunsun �δ� ��δ� �ε����� ����ŸƮ ����
//���� �÷��̽� �� ����Ʈ ���� �÷��̽� ���� ���.
//��δ� ���۷����� ó�� �Ӷ� �ڷ� �����°�.
//������ÿ� �������� ȸ�� �ϴ°�.
//
#define EQP_SERIAL_LIST \
"\
IMC-14-1097,\
IMC-14-1099,\
IMC-15-1008,\
IMC-15-1015,\
IMC-15-1016,\
"
//20150603. ���� ��ġ ��Ȳ.
//Total 3��
//�ʸ��ɿ� 14-1097 - �ܹ���
//�ʸ��ɿ� 14-1099 - �����
//�ʸ��ɿ� 15-1008 - �ܹ���

//20151120 �߰� 2��
//���� 3���߿� 1�� �߱���...����.
//�ʸ��� 15-1015
//ȭ���� 15-1016


//#define COWELL //������ ���� �����͸� �����Ѱ͸� �����. 20150723 ���켮 ���� ��û.
//1���� 3����
#define FILEVERSION "1.3.1.1"
#define FILEDATE    "20151120"










#define MAX_STAGE_CNT 5

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

//��Ʈ ���̵�.
enum EN_PART_ID {
    piSTG  = 0 , //�ϳ� �ۿ� ����
    MAX_PART
};

enum EN_ARAY_ID {
    riSTG = 0 ,
    MAX_ARAY
};

//�������� �����˻� ���������� �����˻�
//Unkwn -> Mask     Work       Good
//                             Fail

//mi<��Ʈ��3�ڸ�>_<��漧1�ڸ�><�ΰ���Ī4>
enum EN_MOTR_ID {
    miSTG_XIndx  = 0 , // �������� x indx
    MAX_MOTR
};

//ai<��Ʈ��3�ڸ�>_<�ΰ���Ī><FWD�ÿ���ġ2�ڸ�><BWD�ÿ���ġ2�ڸ�>
//��Ⱑ Bwd �۵��� Fwd
enum EN_ACTR_ID {
    aiSTG_TbleInOt  = 0 ,  //�������� ���̺� �� �ƿ�
    aiSTG_ChckDnUp      ,  //�������� ô     ũ�ν� ����.
    aiSTG_ProbFwBw      ,  //�������� ���κ� Fw Bw
    MAX_ACTR

};

//�׳� ���̿�.  x+<��Ʈ 3�ڸ�>+_+<���μ���>
//�Ǹ��� ���̿� x+<��Ʈ 3�ڸ�>+_+<���μ���>+<�ش��ൿ2�ڸ� ex) Fw,Bw,Up,Dn>
enum EN_INPUT_ID {
    x0000 =  0 , xETC_StartSw            =  0 , //                                          x0000 =  0 , xETC_StartSw            =  0 , //
    x0001 =  1 , xETC_StopSw             =  1 , //                                          x0001 =  1 , xETC_StopSw             =  1 , //
    x0002 =  2 , xSTG_ChckDn             =  2 , //                                          x0002 =  2 , xETC_ResetSw            =  2 , //
    x0003 =  3 , xSTG_ProbFw             =  3 , //                                          x0003 =  3 , xETC_AirSw              =  3 , //
    x0004 =  4 , xSTG_ProbBw             =  4 , //                                          x0004 =  4 , xETC_InitSw             =  4 , //
    x0005 =  5 , xSTG_TbleOt             =  5 , //                                          x0005 =  5 , xLDR_SideEmgSw          =  5 , //
    x0006 =  6 , xSTG_TbleIn             =  6 , //                                          x0006 =  6 , xETC_FrntEmg1           =  6 , //
    x0007 =  7 , xSTG_CaseShort          =  7 , //                                          x0007 =  7 , xETC_FrntEmg2           =  7 , //
    x0008 =  8 , x008                    =  8 , //                                          x0008 =  8 , xETC_MainPower          =  8 , //
    x0009 =  9 , x009                    =  9 , //                                          x0009 =  9 , xETC_MainAir            =  9 , //
    x000A = 10 , x00A                    = 10 , //                                          x000A = 10 , xETC_FrntDoor1          = 10 , //
    x000B = 11 , x00B                    = 11 , //                                          x000B = 11 , xETC_FrntDoor2          = 11 , //
    x000C = 12 , x00C                    = 12 , //                                          x000C = 12 , xETC_RearDoor1          = 12 , //
    x000D = 13 , x00D                    = 13 , //                                          x000D = 13 , xETC_RearDoor2          = 13 , //
    x000E = 14 , x00E                    = 14 , //                                          x000E = 14 , xLST_Tray1              = 14 , //
    x000F = 15 , x00F                    = 15 , //                                          x000F = 15 , xLST_Tray2Full          = 15 , //

    MAX_INPUT                                                                               //MAX_INPUT
};

//�׳� ���̿�31.  y+<��Ʈ 3�ڸ�>+_+<���μ���>                                               �� ���̿�31.  y+<��Ʈ 3�ڸ�>+_+<���μ���>
//���� �Ǹ��� ���̿� y+<��Ʈ 3�ڸ�>+_+<���μ���>+<�ش��ൿ ex) Fw , Bw , Dn ��� 2�ڸ�>     �� �Ǹ��� ���̿� y+<��Ʈ 3�ڸ�>+_+<���μ���>+<�ش��ൿ ex) Fw , Bw , Dn ��� 2�ڸ�
//�ܵ� �Ǹ��� ���̿� y+<��Ʈ 3�ڸ�>+_+<���μ���>+<Fw�ÿ��ش��ൿ2�ڸ�><Bw�ÿ��ش��ൿ2�ڸ�> �� �Ǹ��� ���̿� y+<��Ʈ 3�ڸ�>+_+<���μ���>+<Fw�ÿ��ش��ൿ2�ڸ�><Bw�ÿ��ش��ൿ
enum EN_OUTPUT_ID  {                                                                        // EN_OUTPUT_ID  {
    y0000 =  0 , yETC_StartLp            =  0 , //                                          y0000 =  0 , yETC_StartLp            =  0 , //
    y0001 =  1 , yETC_StopLp             =  1 , //                                          y0001 =  1 , yETC_StopLp             =  1 , //
    y0002 =  2 , ySTG_ChckDn             =  2 , //                                          y0002 =  2 , yETC_ResetLp            =  2 , //
    y0003 =  3 , ySTG_ProbFw             =  3 , //                                          y0003 =  3 , yETC_AirLp              =  3 , //
    y0004 =  4 , ySTG_TbleIn             =  4 , //                                          y0004 =  4 , yETC_InitLp             =  4 , //
    y0005 =  5 , y005                    =  5 , //                                          y0005 =  5 , yETC_MainAir            =  5 , //
    y0006 =  6 , y006                    =  6 , //                                          y0006 =  6 , yETC_Light              =  6 , //
    y0007 =  7 , y007                    =  7 , //                                          y0007 =  7 , y007                    =  7 , //
    y0008 =  8 , y008                    =  8 , //                                          y0008 =  8 , yETC_TwRedLp            =  8 , //
    y0009 =  9 , y009                    =  9 , //                                          y0009 =  9 , yETC_TwYelLp            =  9 , //
    y000A = 10 , y00A                    = 10 , //                                          y000A = 10 , yETC_TwGrnLp            = 10 , //
    y000B = 11 , y00B                    = 11 , //                                          y000B = 11 , yETC_TwBzz              = 11 , //
    y000C = 12 , y00C                    = 12 , //                                          y000C = 12 , yLSP_GuidBw             = 12 , //
    y000D = 13 , y00D                    = 13 , //                                          y000D = 13 , yLDR_IndxUp             = 13 , //
    y000E = 14 , y00E                    = 14 , //                                          y000E = 14 , yLST_StckUp             = 14 , //
    y000F = 15 , y00F                    = 15 , //                                          y000F = 15 , yLDR_AirBlow            = 15 , //

    MAX_OUTPUT
};

// �ʹ� ���� �̿��� �׳� �Ⱦ���.
// ������ ����   pv+<��Ʈ3�ڸ�>+_+<�ΰ�����>+<�����Ǽ���>
// �����V �����V po+<��Ʈ3�ڸ�>+_+<�ΰ�����>+<�����V����>
enum EN_PSTN_VALUE {            //
    pvSTG_XIndxWait        = 0 ,
    pvSTG_XIndxWorkStt         ,
    poSTG_XIndxPtch            ,
    MAX_PSTN_MOTR0
};


// ������ ����   pi+<��Ʈ3�ڸ�>+_+<�ΰ�����>+<�����Ǽ���>
// �������� ������ ���� �ʴ� ���� (ex)piWRK_XIdxWork ,piLDR_ZCmpWork) �ڷ� ����
// PstnMan���� CheckSafe�� ��ư Ŭ�� �ÿ� �¾� ��������. ����....
enum EN_PSTN_ID {
    piSTG_XIndxWait        = 0 ,
    piSTG_XIndxWorkStt         ,
    piSTG_XIndxWork            ,
    MAX_PSTN_ID_MOTR0          ,
};

enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcSTG_Home             = 10 ,
    MAX_MANUAL_CYCLE
};

enum EN_ERR_ID {
/*000*/eiETC_MainAir       =0,
/*001*/ei001                 ,
/*002*/eiETC_Emergency       ,
/*003*/ei003                 ,
/*004*/eiETC_Door            ,
/*005*/ei005                 ,
/*006*/ei006                 ,
/*007*/ei007                 ,
/*008*/ei008                 ,
/*009*/eiETC_ToStartTO       ,
/*010*/eiETC_ToStopTO        , 
/*011*/eiETC_AllHomeTO       , 
/*012*/eiETC_ManCycleTO      , 
/*013*/ei013                 ,
/*014*/eiPRT_CycleTO         ,
/*015*/eiPRT_HomeTo          ,
/*016*/eiPRT_ToStartTO       ,
/*017*/eiPRT_ToStopTO        ,
/*018*/ei018                 , 
/*019*/eiMTR_HomeEnd         , 
/*020*/eiMTR_NegLim          , 
/*021*/eiMTR_PosLim          , 
/*022*/eiMTR_Alarm           ,
/*023*/ei023                 ,
/*024*/eiATR_TimeOut         ,
/*025*/ei025                 ,
/*026*/eiPKG_Dispr           ,
/*027*/eiPKG_Unknwn          , //--------------------------------- �����뿡��.
/*028*/ei028                 ,
/*029*/ei029                 ,
/*030*/ei030                 ,
/*031*/ei031                 ,
/*032*/ei032                 ,
/*033*/ei033                 ,
/*034*/ei034                 ,
/*035*/ei035                 ,
/*036*/ei036                 ,
/*037*/ei037                 ,
/*038*/ei038                 ,
/*039*/ei039                 ,
/*040*/ei040                 , 
/*041*/ei041                 , 
/*042*/ei042                 , 
/*043*/ei043                 , 
/*044*/ei044                 , 
/*045*/ei045                 ,
/*046*/ei046                 , 
/*047*/ei047                 , 
/*048*/ei048                 , 
/*059*/ei049                 ,
MAX_ERR
};


#endif

