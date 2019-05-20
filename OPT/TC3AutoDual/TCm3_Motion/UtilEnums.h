#ifndef ENUMSH
#define ENUMSH

enum EN_MOTR_ID {
    miLT_Z = 0 ,
    miLT_Y     ,
    miLT_I     ,
    miRT_Z     ,
    miRT_Y     ,
    miRT_I     ,

    MAX_MOTR
};

enum EN_ACTR_ID {
    aiLT_Main   = 0 ,  //0
    aiLT_Sub        ,  //1
    aiLT_LdCal      ,  //2
    aiLT_Gp1        ,  //3
    aiLT_Support    ,  //4
    aiLT_PcbCmp     ,  //5
    aiLT_GtCal      ,  //6
    aiLT_UvFB       ,  //7
    aiLT_UvUD       ,  //8
    aiLT_Dp1IO      ,  //9
    aiLT_Dp2IO      ,  //10
    aiLT_Dp1UD      ,  //11                 //실린더 작업.
    aiLT_Dp2UD      ,  //12
    aiLT_PadUD      ,  //13
    aiRT_Main       ,  //14
    aiRT_Sub        ,  //15
    aiRT_LdCal      ,  //16
    aiRT_Gp1        ,  //17
    aiRT_Support    ,  //18
    aiRT_PcbCmp     ,  //19
    aiRT_GtCal      ,  //20
    aiRT_UvFB       ,  //21
    aiRT_UvUD       ,  //22
    aiRT_Dp1IO      ,  //23
    aiRT_Dp2IO      ,  //24
    aiRT_Dp1UD      ,  //25
    aiRT_Dp2UD      ,  //26
    aiRT_PadUD      ,  //27

    aiLT_LdIO       ,  //28
    aiRT_LdIO       ,  //29

    MAX_ACTR           //
};

enum EN_PART_ID {
    piBase        = 0 ,
    piLT          = 1 ,
    piRT          = 2 ,

    MAX_PART_ID
};

enum EN_ERR_ID {
    eiLT_HomeTO    =  0  ,
    eiLT_StartTO   =  1  ,
    eiLT_StopTO    =  2  ,
    eiLT_CycleTO   =  3  ,
    eiLT_CkBtnTO   =  4  ,
    eiRT_HomeTO    =  5  ,
    eiRT_StartTO   =  6  ,
    eiRT_StopTO    =  7  ,
    eiRT_CycleTO   =  8  ,
    eiRT_CkBtnTO   =  9  ,
    eiLT_ZHomeEnd  = 10  ,
    eiLT_YHomeEnd  = 11  ,
    eiLT_IHomeEnd  = 12  ,
    eiRT_ZHomeEnd  = 13  ,
    eiRT_YHomeEnd  = 14  ,
    eiRT_IHomeEnd  = 15  ,
    eiLT_ZNegLim   = 16  ,
    eiLT_YNegLim   = 17  ,
    eiLT_INegLim   = 18  ,
    eiRT_ZNegLim   = 19  ,
    eiRT_YNegLim   = 20  ,
    eiRT_INegLim   = 21  ,
    eiLT_ZPosLim   = 22  ,
    eiLT_YPosLim   = 23  ,
    eiLT_IPosLim   = 24  ,
    eiRT_ZPosLim   = 25  ,
    eiRT_YPosLim   = 26  ,
    eiRT_IPosLim   = 27  ,
    eiLT_ZAlarm    = 28  ,
    eiLT_YAlarm    = 29  ,
    eiLT_IAlarm    = 30  ,
    eiRT_ZAlarm    = 31  ,
    eiRT_YAlarm    = 32  ,
    eiRT_IAlarm    = 33  ,
    ei34           = 34  ,
    ei35           = 35  ,
    ei36           = 36  ,
    ei37           = 37  ,
    ei38           = 38  ,
    ei39           = 39  ,
    ei40           = 40  ,
    ei41           = 41  ,
    ei42           = 42  ,
    ei43           = 43  ,
    ei44           = 44  ,
    ei45           = 45  ,
    ei46           = 46  ,
    ei47           = 47  ,
    ei48           = 48  ,
    ei49           = 49  ,
    eiLT_MainTO    = 50  ,
    eiLT_SubTO     = 51  ,
    eiLT_LdCalTO   = 52  ,
    eiLT_Gp1TO     = 53  ,
    eiLT_SupportTO = 54  ,
    eiLT_PcbCmpTO  = 55  ,
    eiLT_GtCalTO   = 56  ,
    eiLT_UvFBTO    = 57  ,
    eiLT_UvUDTO    = 58  ,
    eiLT_Dp1IOTO   = 59  ,
    eiLT_Dp2IOTO   = 60  ,
    eiLT_Dp1UDTO   = 61  ,
    eiLT_Dp2UDTO   = 62  ,
    eiLT_PadUDTO   = 63  ,
    eiRT_MainTO    = 64  ,
    eiRT_SubTO     = 65  ,
    eiRT_LdCalTO   = 66  ,
    eiRT_Gp1TO     = 67  ,
    eiRT_SupportTO = 68  ,
    eiRT_PcbCmpTO  = 69  ,
    eiRT_GtCalTO   = 70  ,
    eiRT_UvFBTO    = 71  ,
    eiRT_UvUDTO    = 72  ,
    eiRT_Dp1IOTO   = 73  ,
    eiRT_Dp2IOTO   = 74  ,
    eiRT_Dp1UDTO   = 75  ,
    eiRT_Dp2UDTO   = 76  ,
    eiRT_PadUDTO   = 77  ,
    eiMainAir      = 78  ,
    eiVisnInspFail = 79  ,
    eiVisnRangOver = 80  ,
    eiAPCRangOver  = 81  ,
    eiShiftRangOver= 82  ,
    eiLT_LdIOTO    = 83  ,
    eiRT_LdIOTO    = 84  ,
    eiVisnBeam     = 85  ,
    eiEmgSw        = 86  ,
    eiAngleSize    = 87  ,

    MAX_ERR
};                   
                     
//enum EN_MAN_ID {
//    miLT_Home = 0  ,
//
//    MAX_MAN
//};

enum EN_INPUT_ID {
x0000 =  0 ,  xLT_StartSw  =  0 , // LeftTable START S/W
x0001 =  1 ,  xLT_PickupSw =  1 , // LeftTable PICK UP S/W
x0002 =  2 ,  xLT_UpDnSw   =  2 , // LeftTable UP/ DOWN  S/W
x0003 =  3 ,  xLT_VacSw    =  3 , // LeftTable Vacuum S/W
x0004 =  4 ,  xLT_ChuckSw  =  4 , // LeftTable Chucking S/W
x0005 =  5 ,  xLT_StopSw   =  5 , // LeftTable STOP S/W
x0006 =  6 ,  xLT_MainBw   =  6 , // LeftTable Main Fixer  BW
x0007 =  7 ,  xLT_LdBw     =  7 , // LeftTable LD DN
x0008 =  8 ,  xLT_UvFw     =  8 , // LeftTable UV IN
x0009 =  9 ,  xLT_UvBw     =  9 , // LeftTable UV OUT
x000A = 10 ,  xLT_Dp1Fw    = 10 , // LeftTable Dispensor  IN1
x000B = 11 ,  xLT_Dp2Fw    = 11 , // LeftTable Dispensor  IN2
x000C = 12 ,  xLT_Dp1Bw    = 12 , // LeftTable Dispensor OUT1
x000D = 13 ,  xLT_Dp2Bw    = 13 , // LeftTable Dispensor OUT2
x000E = 14 ,  xLT_Dp1Up    = 14 , // LeftTable Dispensor UP1
x000F = 15 ,  xLT_Dp2Up    = 15 , // LeftTable Dispensor UP2

x0010 = 16 ,  xLT_Dp1Dn    = 16 , // LeftTable Dispensor  DOWN1
x0011 = 17 ,  xLT_Dp2Dn    = 17 , // LeftTable Dispensor  DOWN2
x0012 = 18 ,  xLT_HdUp     = 18 , // LeftTable UV UP
x0013 = 19 ,  xLT_HdDn     = 19 , // LeftTable UV DOWN
x0014 = 20 ,  xLT_HdEnd    = 20 , // LeftTable UV END(Ushio)
x0015 = 21 ,  xMainSol     = 21 , // Main Sol Check
x0016 = 22 ,  xLT_CnUp     = 22 , // LeftTable Cleaner Pad Up
x0017 = 23 ,  xLT_LdIn     = 23 , // LeftTable LD IN
x0018 = 24 ,  xLT_ResetSw  = 24 , // LeftTable Reset S/W
x0019 = 25 ,  xEmgSw       = 25 , // EMG S/W
x001A = 26 ,  x026         = 26 , //
x001B = 27 ,  x027         = 27 , //
x001C = 28 ,  x028         = 28 , //
x001D = 29 ,  x029         = 29 , //
x001E = 30 ,  x030         = 30 , //
x001F = 31 ,  x031         = 31 , //

x0020 = 32 ,  xRT_StartSw  = 32 , // RightTable START S/W
x0021 = 33 ,  xRT_PickupSw = 33 , // RightTable PICK UP S/W
x0022 = 34 ,  xRT_UpDnSw   = 34 , // RightTable UP/ DOWN  S/W
x0023 = 35 ,  xRT_VacSw    = 35 , // RightTable Vacuum S/W
x0024 = 36 ,  xRT_ChuckSw  = 36 , // RightTable Chucking S/W
x0025 = 37 ,  xRT_StopSw   = 37 , // RightTable STOP S/W
x0026 = 38 ,  xRT_MainBw   = 38 , // RightTable Main Fixer  BW
x0027 = 39 ,  xRT_LdBw     = 39 , // RightTable LD DN
x0028 = 40 ,  xRT_UvFw     = 40 , // RightTable UV IN
x0029 = 41 ,  xRT_UvBw     = 41 , // RightTable UV OUT
x002A = 42 ,  xRT_Dp1Fw    = 42 , // RightTable Dispensor  IN1
x002B = 43 ,  xRT_Dp2Fw    = 43 , // RightTable Dispensor  IN2
x002C = 44 ,  xRT_Dp1Bw    = 44 , // RightTable Dispensor OUT1
x002D = 45 ,  xRT_Dp2Bw    = 45 , // RightTable Dispensor OURT2
x002E = 46 ,  xRT_Dp1Up    = 46 , // RightTable Dispensor UP1
x002F = 47 ,  xRT_Dp2Up    = 47 , // RightTable Dispensor UP2

x0030 = 48 ,  xRT_Dp1Dn    = 48 , // RightTable Dispensor  DOWN1
x0031 = 49 ,  xRT_Dp2Dn    = 49 , // RightTable Dispensor  DOWN2
x0032 = 50 ,  xRT_HdUp     = 50 , // RightTable UV UP
x0033 = 51 ,  xRT_HdDn     = 51 , // RightTable UV DOWN
x0034 = 52 ,  xRT_HdEnd    = 52 , // RightTable UV END(Ushio)
x0035 = 53 ,  xRT_CnUp     = 53 , // RightTable Cleaner Pad Up
x0036 = 54 ,  x054         = 54 , //
x0037 = 55 ,  xRT_LdIn     = 55 , // RightTable LD IN
x0038 = 56 ,  xRT_ResetSw  = 56 , // RightTable Reset S/W
x0039 = 57 ,  x057         = 57 , //
x003A = 58 ,  x058         = 58 , //
x003B = 59 ,  x059         = 59 , //
x003C = 60 ,  x060         = 60 , //
x003D = 61 ,  x061         = 61 , //
x003E = 62 ,  x062         = 62 , //
x003F = 63 ,  x063         = 63 , //

MAX_INPUT
};

enum EN_OUTPUT_ID {
y0000 =  0 ,  yLT_StartLp  =  0 , // LeftTable Start A/W (LAMP)
y0001 =  1 ,  yLT_PickupLp =  1 , // LeftTable Pick Up S/W (LAMP)
y0002 =  2 ,  yLT_UpDnLp   =  2 , // LeftTable Up/Down  S/W (LAMP)
y0003 =  3 ,  yLT_VacLp    =  3 , // LeftTable Vacuum S/W (LAMP)
y0004 =  4 ,  yLT_ChuckLp  =  4 , // LeftTable Chucking S/W (LAMP)
y0005 =  5 ,  yLT_StopLp   =  5 , // LeftTable Stop S/W (LAMP)
y0006 =  6 ,  yLT_MainBw   =  6 , // LeftTable Main Fixer Bwd
y0007 =  7 ,  yLT_SubFw    =  7 , // LeftTable Sub Fixer Bwd
y0008 =  8 ,  yLT_LdBw     =  8 , // LeftTable LD Adjuster Bwd
y0009 =  9 ,  yLT_Gp1Fw    =  9 , // LeftTable Gripper Fwd
y000A = 10 ,  yLT_SupportFw= 10 , // LeftTable Supporter Fwd
y000B = 11 ,  yLT_SuckOn   = 11 , // LeftTable Sucker On
y000C = 12 ,  yLT_PcbCmpBw = 12 , // LeftTable PCB Connector Clamp
y000D = 13 ,  yLT_GtFw     = 13 , // LeftTable GT Adjuster UP
y000E = 14 ,  yLT_UvFw     = 14 , // LeftTable UV Fwd
y000F = 15 ,  yLT_HdUp     = 15 , // LeftTable UV Up

y0010 = 16 ,  yLT_Dp1Fw    = 16 , // LeftTable Dispensor IN 1
y0011 = 17 ,  yLT_Dp2Fw    = 17 , // LeftTable Dispensor IN 2
y0012 = 18 ,  yLT_Dp1Bw    = 18 , // LeftTable Dispensor OUT1
y0013 = 19 ,  yLT_Dp2Bw    = 19 , // LeftTable Dispensor OUT2
y0014 = 20 ,  yLT_Dp1Up    = 20 , // LeftTable Dispensor UP1
y0015 = 21 ,  yLT_Dp2Up    = 21 , // LeftTable Dispensor UP2
y0016 = 22 ,  yLT_Dp1Dn    = 22 , // LeftTable Dispensor DOWN1
y0017 = 23 ,  yLT_Dp2Dn    = 23 , // LeftTable Dispensor DOWN2
y0018 = 24 ,  yLT_HdStart  = 24 , // LeftTable UV START(Ushiho)
y0019 = 25 ,  yLT_HdDn     = 25 , // LeftTable UV Down
y001A = 26 ,  yLT_CnUp     = 26 , // LeftTable Dispensor Cleaner Pad Up
y001B = 27 ,  yLT_ResetLp  = 27 , // LeftTable Reset S/W (LAMP)
y001C = 28 ,  y028         = 28 , //
y001D = 29 ,  y029         = 29 , //
y001E = 30 ,  y030         = 30 , //
y001F = 31 ,  y031         = 31 , //

y0020 = 32 ,  yRT_StartLp  = 32 , // RightTable Start A/W (LAMP)
y0021 = 33 ,  yRT_PickupLp = 33 , // RightTable Pick Up S/W (LAMP)
y0022 = 34 ,  yRT_UpDnLp   = 34 , // RightTable Up/Down  S/W (LAMP)
y0023 = 35 ,  yRT_VacLp    = 35 , // RightTable Vacuum S/W (LAMP)
y0024 = 36 ,  yRT_ChuckLp  = 36 , // RightTable Chucking S/W (LAMP)
y0025 = 37 ,  yRT_StopLp   = 37 , // RightTable Stop S/W (LAMP)
y0026 = 38 ,  yRT_MainBw   = 38 , // RightTable Main Fixer Bwd
y0027 = 39 ,  yRT_SubFw    = 39 , // RightTable Sub Fixer Bwd
y0028 = 40 ,  yRT_LdBw     = 40 , // RightTable LD Adjuster Bwd
y0029 = 41 ,  yRT_Gp1Fw    = 41 , // RightTable Gripper Fwd
y002A = 42 ,  yRT_SupportFw= 42 , // RightTable Supporter Fwd
y002B = 43 ,  yRT_SuckOn   = 43 , // RightTable Sucker On
y002C = 44 ,  yRT_PcbCmpBw = 44 , // RightTable PCB Connector Clamp
y002D = 45 ,  yRT_GtFw     = 45 , // RightTable GT Adjuster UP
y002E = 46 ,  yRT_UvFw     = 46 , // RightTable UV Fwd
y002F = 47 ,  yRT_HdUp     = 47 , // RightTable UV Up

y0030 = 48 ,  yRT_Dp1Fw    = 48 , // RightTable Dispensor IN 1
y0031 = 49 ,  yRT_Dp2Fw    = 49 , // RightTable Dispensor IN 2
y0032 = 50 ,  yRT_Dp1Bw    = 50 , // RightTable Dispensor OUT1
y0033 = 51 ,  yRT_Dp2Bw    = 51 , // RightTable Dispensor OUT2
y0034 = 52 ,  yRT_Dp1Up    = 52 , // RightTable Dispensor UP1
y0035 = 53 ,  yRT_Dp2Up    = 53 , // RightTable Dispensor UP2
y0036 = 54 ,  yRT_Dp1Dn    = 54 , // RightTable Dispensor DOWN1
y0037 = 55 ,  yRT_Dp2Dn    = 55 , // RightTable Dispensor DOWN2
y0038 = 56 ,  yRT_HdStart  = 56 , // RightTable UV START(Usiho)
y0039 = 57 ,  yRT_HdDn     = 57 , // RightTable UV Down
y003A = 58 ,  yRT_CnUp     = 58 , // RightTable Dispensor Cleaner Pad Up
y003B = 59 ,  yRT_ResetLp  = 59 , // RightTable Reset S/W (LAMP)
y003C = 60 ,  y060         = 60 , //
y003D = 61 ,  y061         = 61 , //
y003E = 62 ,  y062         = 62 , //
y003F = 63 ,  y063         = 63 , //

y0040 = 64 ,  yLT_Sol1     = 64 , // LeftTable SOL  1
y0041 = 65 ,  yLT_Sol2     = 65 , // LeftTable SOL  2
y0042 = 66 ,  yLT_Sol3     = 66 , // LeftTable SOL  3
y0043 = 67 ,  yLT_Sol4     = 67 , // LeftTable SOL  4
y0044 = 68 ,  yLT_Sol5     = 68 , // LeftTable SOL  5
y0045 = 69 ,  yLT_Sol6     = 69 , // LeftTable SOL  6
y0046 = 70 ,  yLT_LdFw     = 70 , // LeftTable LD Adjuster Fwd
y0047 = 71 ,  yLT_UvBw     = 71 , // LeftTable UV Bwd
y0048 = 72 ,  yLT_LdIn     = 72 , //
y0049 = 73 ,  yRT_LdIn     = 73 , //
y004A = 74 ,  y074         = 74 , //
y004B = 75 ,  y075         = 75 , //
y004C = 76 ,  y076         = 76 , //
y004D = 77 ,  y077         = 77 , //
y004E = 78 ,  y078         = 78 , //
y004F = 79 ,  y079         = 79 , //

y0050 = 80 ,  yRT_Sol1     = 80 , // RightTable SOL  1
y0051 = 81 ,  yRT_Sol2     = 81 , // RightTable SOL  2
y0052 = 82 ,  yRT_Sol3     = 82 , // RightTable SOL  3
y0053 = 83 ,  yRT_Sol4     = 83 , // RightTable SOL  4
y0054 = 84 ,  yRT_Sol5     = 84 , // RightTable SOL  5
y0055 = 85 ,  yRT_Sol6     = 85 , // RightTable SOL  6
y0056 = 86 ,  yRT_LdFw     = 86 , // RightTable LD Adjuster Fwd
y0057 = 87 ,  yRT_UvBw     = 87 , // RightTable UV Bwd
y0058 = 88 ,  y088         = 88 , //
y0059 = 89 ,  y089         = 89 , //
y005A = 90 ,  y090         = 90 , //
y005B = 91 ,  y091         = 91 , //
y005C = 92 ,  y092         = 92 , //
y005D = 93 ,  y093         = 93 , //
y005E = 94 ,  y094         = 94 , //
y005F = 95 ,  y095         = 95 , //

MAX_OUTPUT
};

#endif


