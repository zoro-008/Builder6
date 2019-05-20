#ifndef UTILDEFINEH
#define UTILDEFINEH

#define EQP_SERIAL_LIST \
"\
ITW-14-1025,\
"

enum { ccBwd  = 0 , ccFwd  = 1};
enum { toBuff = 0 , toTabl = 1};

//��Ʈ ���̵�.
enum EN_PART_ID {
    piJIP = 0  , // Stage
    piDIP      , // Dispenser
    piPLC      , // Place

    MAX_PART
};

enum EN_ARAY_ID {
    riDSP  = 0 , // Dispenser
    riJIG      , // JIG
    riPIK      , // Picker
    MAX_ARAY
};

//�������� �����˻� ���������� �����˻�
//Unkwn -> Mask     Work       Good
//                             Fail

//mi<��Ʈ��3�ڸ�>_<��漧1�ڸ�><�ΰ���Ī4>
enum EN_MOTR_ID {
    miDSP_XDsps  = 0  , // Dsps X ��
    miSTG_ZPick  = 1  , // Pick Z ��

    MAX_MOTR
};

//ai<��Ʈ��3�ڸ�>_<�ΰ���Ī><FWD�ÿ���ġ2�ڸ�><BWD�ÿ���ġ2�ڸ�>
//��Ⱑ Bwd �۵��� Fwd
enum EN_ACTR_ID {
    aiDSP_IdxUpDn  = 0 ,
    aiSTG_PshFwBw      ,

    MAX_ACTR

};

//�׳� ���̿�.  x+<��Ʈ 3�ڸ�>+_+<���μ���>
//�Ǹ��� ���̿� x+<��Ʈ 3�ڸ�>+_+<���μ���>+<�ش��ൿ2�ڸ� ex) Fw,Bw,Up,Dn>
enum EN_INPUT_ID {
    x0000 =  0 , xETC_StartSw            =  0 , //
    x0001 =  1 , xETC_StopSw             =  1 , //
    x0002 =  2 , xETC_EmgSw              =  2 , //
    x0003 =  3 , xSTG_PickFw             =  3 , //
    x0004 =  4 , xSTG_PickBw             =  4 , //
    x0005 =  5 , xDSP_DspsUp             =  5 , //
    x0006 =  6 , xDSP_DspsDn             =  6 , //
    x0007 =  7 , xETC_PickVacuum         =  7 , //
    x0008 =  8 , xETC_TempCont           =  8 , //
    x0009 =  9 , xETC_MainPower          =  9 , //
    x000A = 10 , xETC_MainAirCheck       = 10 , //
    x000B = 11 , x011                    = 11 , //
    x000C = 12 , x012                    = 12 , //
    x000D = 13 , x013                    = 13 , //
    x000E = 14 , x014                    = 14 , //
    x000F = 15 , x015                    = 15 , //

    MAX_INPUT
};

//�׳� ���̿�31.  y+<��Ʈ 3�ڸ�>+_+<���μ���>
//���� �Ǹ��� ���̿� y+<��Ʈ 3�ڸ�>+_+<���μ���>+<�ش��ൿ ex) Fw , Bw , Dn ��� 2�ڸ�>
//�ܵ� �Ǹ��� ���̿� y+<��Ʈ 3�ڸ�>+_+<���μ���>+<Fw�ÿ��ش��ൿ2�ڸ�><Bw�ÿ��ش��ൿ2�ڸ�>
enum EN_OUTPUT_ID  {
    y0000 =  0 , yETC_StartLp            =  0 , //
    y0001 =  1 , yETC_StopLp             =  1 , //
    y0002 =  2 , yETC_MainAirSol         =  2 , //
    y0003 =  3 , ySTG_Break              =  3 , //
    y0004 =  4 , ySTG_PickFw             =  4 , //
    y0005 =  5 , ySTG_PickBw             =  5 , //
    y0006 =  6 , ySTG_DisnrUp            =  6 , // Dispenser Up
    y0007 =  7 , ySTG_DisnrDn            =  7 , // Dispenser Dn
    y0008 =  8 , yETC_VCMAirOnOff        =  8 , // Vacuum Air On Off
    y0009 =  9 , yETC_Vacuum             =  9 , // Vaccum
    y000A = 10 , yETC_Eject              = 10 , // Eject
    y000B = 11 , yETC_Buzz               = 11 , //
    y000C = 12 , y012                    = 12 , //
    y000D = 13 , y013                    = 13 , //
    y000E = 14 , y014                    = 14 , //
    y000F = 15 , y015                    = 15 , //

    MAX_OUTPUT
};

// ������ ����   pv+<��Ʈ3�ڸ�>+_+<�ΰ�����>+<�����Ǽ���>
// �����V �����V po+<��Ʈ3�ڸ�>+_+<�ΰ�����>+<�����V����>
enum EN_PSTN_VALUE {
    pvDSP_XIndxWait    = 0 , // ���� ó�� ��ġ
    pvDSP_XIndxWorkStt     , // ���� ���漭 ��� �Ѹ��� ���� ��ġ
    pvDSP_XIndxEnd         , // ���� ���漭 ��� ������ ��ġ
    MAX_PSTN_MOTR0         ,

    pvSTG_ZPickWait    = 0 , // ���� ��� ó�� ��ġ
    pvSTG_ZPickJIG         , // ���� ���� ��ġ
    pvSTG_ZPickWork        , // ���� ���� �� ��ġ
    pvSTG_ZPickPlace       , // ���� �� �����ְ� ���� �ö󰬴ٰ� �ڷ� ���� wait�� ���� ��ġ
    MAX_PSTN_MOTR1         ,

};

// ������ ����   pi+<��Ʈ3�ڸ�>+_+<�ΰ�����>+<�����Ǽ���>
// �������� ������ ���� �ʴ� ���� (ex)piWRK_XIdxWork ,piLDR_ZCmpWork) �ڷ� ����
// PstnMan���� CheckSafe�� ��ư Ŭ�� �ÿ� �¾� ��������. ����....
enum EN_PSTN_ID {
    piDSP_XIndxWait    = 0 ,
    piDSP_XIndxWorkStt     ,
    piDSP_XIndxEnd         ,
    MAX_PSTN_ID_MOTR0      ,

    piSTG_ZPickWait    = 0 ,
    piSTG_ZPickJIG         ,
    piSTG_ZPickWork        ,
    piSTG_ZPickPlace       ,
    MAX_PSTN_ID_MOTR1      ,

};


enum EN_MANUAL_CYCLE
{
    mcNoneCycle            =  0 ,
    mcAllHome              =  1 ,

    mcJIP_CycleJigPick     = 10 ,
    mcDIP_CycleDispenser        ,
    mcPLC_CyclePlace            ,
    mcSTG_AllSttClear      = 16 ,
    mcSTG_IdxUpDn          = 18 ,
    mcSTG_PshFwBw               ,
    mcSTG_Vacuum                ,
    mcSTG_Eject                 ,

    MAX_MANUAL_CYCLE
};

enum EN_ERR_ID {
/*000*/eiETC_MainAir       =0,
/*001*/ei001                 ,
/*002*/eiETC_Emergency       ,
/*003*/ei003                 ,
/*004*/ei004                 ,
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
/*030*/eiPIC_Vaccum          , //PIK
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
/*050*/ei049                 ,
/*051*/ei050                 ,
/*052*/ei051                 ,
/*053*/ei053                 ,
/*054*/ei054                 ,
/*055*/ei055                 ,
/*056*/ei056                 ,
/*057*/ei057                 ,
/*058*/ei058                 ,
/*059*/ei059                 ,
/*060*/MAX_ERR
};


#endif



