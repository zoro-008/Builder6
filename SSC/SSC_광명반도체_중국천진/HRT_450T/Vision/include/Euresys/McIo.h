
// MCIO.H -- MULTICAM IO DEFINITIONS

#ifndef _MULTICAM_IO_
#define _MULTICAM_IO_

// I/O ADDRESSING
#define MC_A1IN         28
#define MC_A1OUT        28
#define MC_A2IN         29
#define MC_A2OUT        29
#define MC_ACC1         21
#define MC_ACC2         22
#define MC_ACC3         23
#define MC_ACC4         24
#define MC_ACK          27
#define MC_ADVAL        23
#define MC_AFVAL        22
#define MC_ALVAL        21
#define MC_ASPARE       24
#define MC_B1IN         38
#define MC_B1OUT        38
#define MC_B2IN         39
#define MC_B2OUT        39
#define MC_BCC1         31
#define MC_BCC2         32
#define MC_BCC3         33
#define MC_BCC4         34
#define MC_BCK          37
#define MC_BDVAL        33
#define MC_BFVAL        32
#define MC_BLVAL        31
#define MC_BSPARE       34
#define MC_CC1          21
#define MC_CC2          22
#define MC_CC3          23
#define MC_CC4          24
#define MC_CK           27
#define MC_DVAL         23
#define MC_FVAL         22
#define MC_GRN          52
#define MC_GRNA         52
#define MC_GRNB         54
#define MC_GRNX         52
#define MC_GRNY         54
#define MC_GRNZ         56
#define MC_IN1          1
#define MC_IN2          2
#define MC_IN3          3
#define MC_IN4          4
#define MC_IN5          5
#define MC_IN6          6
#define MC_IN7          7
#define MC_IN8          8
#define MC_IN9          9
#define MC_IN10         10
#define MC_IN11         11
#define MC_IN12         12
#define MC_IN13         13
#define MC_IN14         14
#define MC_IN15         15
#define MC_IN16         16
#define MC_IN17         17
#define MC_IN18         18
#define MC_IN19         19
#define MC_IN20         20
#define MC_LVAL         21
#define MC_OUT1         1
#define MC_OUT2         2
#define MC_OUT3         3
#define MC_OUT4         4
#define MC_OUT5         5
#define MC_OUT6         6
#define MC_OUT7         7
#define MC_OUT8         8
#define MC_OUT9         9
#define MC_OUT10        10
#define MC_OUT11        11
#define MC_OUT12        12
#define MC_OUT13        13
#define MC_OUT14        14
#define MC_OUT15        15
#define MC_OUT16        16
#define MC_OUT17        17
#define MC_OUT18        18
#define MC_OUT19        19
#define MC_OUT20        20
#define MC_RED          51
#define MC_REDA         51
#define MC_REDB         53
#define MC_REDX         51
#define MC_REDY         53
#define MC_REDZ         55
#define MC_SPARE        24
#define MC_STR          17
#define MC_STX          17
#define MC_STY          18
#define MC_STZ          19
#define MC_TRG          17
#define MC_TRX          17
#define MC_TRY          18
#define MC_TRZ          19
#define MC_XCK          27
#define MC_XEIN         26
#define MC_XEOUT        26
#define MC_XGIN         24
#define MC_XGOUT        24
#define MC_XHIN         23
#define MC_XHOUT        23
#define MC_XRIN         22
#define MC_XROUT        22
#define MC_XVIN         25
#define MC_XVOUT        25
#define MC_YCK          37
#define MC_YEIN         36
#define MC_YEOUT        36
#define MC_YGIN         34
#define MC_YGOUT        34
#define MC_YHIN         33
#define MC_YHOUT        33
#define MC_YRIN         32
#define MC_YROUT        32
#define MC_YVIN         35
#define MC_YVOUT        35
#define MC_ZCK          47
#define MC_ZEIN         46
#define MC_ZEOUT        46
#define MC_ZGIN         44
#define MC_ZGOUT        44
#define MC_ZHIN         43
#define MC_ZHOUT        43
#define MC_ZRIN         42
#define MC_ZROUT        42
#define MC_ZVIN         45
#define MC_ZVOUT        45
#define MC_MV0_IN1      21
#define MC_MV0_IN2      22
#define MC_MV0_IN3      23
#define MC_MV0_IN4      24
#define MC_MV0_IN5      25
#define MC_MV1_IN1      26
#define MC_MV1_IN2      27
#define MC_MV1_IN3      28
#define MC_MV1_IN4      29
#define MC_MV1_IN5      30
#define MC_MV2_IN1      31
#define MC_MV2_IN2      32
#define MC_MV2_IN3      33
#define MC_MV2_IN4      34
#define MC_MV2_IN5      35
#define MC_MV3_IN1      36
#define MC_MV3_IN2      37
#define MC_MV3_IN3      38
#define MC_MV3_IN4      39
#define MC_MV3_IN5      40
#define MC_MV0_OUT1     21
#define MC_MV0_OUT2     22
#define MC_MV0_OUT3     23
#define MC_MV0_OUT4     24
#define MC_MV0_OUT5     25
#define MC_MV1_OUT1     26
#define MC_MV1_OUT2     27
#define MC_MV1_OUT3     28
#define MC_MV1_OUT4     29
#define MC_MV1_OUT5     30
#define MC_MV2_OUT1     31
#define MC_MV2_OUT2     32
#define MC_MV2_OUT3     33
#define MC_MV2_OUT4     34
#define MC_MV2_OUT5     35
#define MC_MV3_OUT1     36
#define MC_MV3_OUT2     37
#define MC_MV3_OUT3     38
#define MC_MV3_OUT4     39
#define MC_MV3_OUT5     40
#define MC_MIO0_IN1     100
#define MC_MIO0_IN2     101
#define MC_MIO0_IN3     102
#define MC_MIO0_IN4     103
#define MC_MIO1_IN1     104
#define MC_MIO1_IN2     105
#define MC_MIO1_IN3     106
#define MC_MIO1_IN4     107
#define MC_MIO2_IN1     108
#define MC_MIO2_IN2     109
#define MC_MIO2_IN3     110
#define MC_MIO2_IN4     111
#define MC_MIO3_IN1     112
#define MC_MIO3_IN2     113
#define MC_MIO3_IN3     114
#define MC_MIO3_IN4     115
#define MC_MIO4_IN1     116
#define MC_MIO4_IN2     117
#define MC_MIO4_IN3     118
#define MC_MIO4_IN4     119
#define MC_MIOSB_IN1    120
#define MC_MIOSB_IN2    121
#define MC_MIOSB_IN3    122
#define MC_MIOSB_IN4    123
#define MC_MIOSB_IN5    124
#define MC_MIOSB_IN6    125
#define MC_MIO0_OUT1    100
#define MC_MIO0_OUT2    101
#define MC_MIO0_OUT3    102
#define MC_MIO0_OUT4    103
#define MC_MIO1_OUT1    104
#define MC_MIO1_OUT2    105
#define MC_MIO1_OUT3    106
#define MC_MIO1_OUT4    107
#define MC_MIO2_OUT1    108
#define MC_MIO2_OUT2    109
#define MC_MIO2_OUT3    110
#define MC_MIO2_OUT4    111
#define MC_MIO3_OUT1    112
#define MC_MIO3_OUT2    113
#define MC_MIO3_OUT3    114
#define MC_MIO3_OUT4    115
#define MC_MIO4_OUT1    116
#define MC_MIO4_OUT2    117
#define MC_MIO4_OUT3    118
#define MC_MIO4_OUT4    119
#define MC_MIOSB_OUT1   120
#define MC_MIOSB_OUT2   121
#define MC_MIOSB_OUT3   122
#define MC_MIOSB_OUT4   123
#define MC_MIOSB_OUT5   124
#define MC_MIOSB_OUT6   125

#define MC_MAX_IO_INDEX 125

#endif // _MULTICAM_IO_
