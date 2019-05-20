//---------------------------------------------------------------------------

#ifndef HexaPotH
#define HexaPotH

#include <windows.h>
//---------------------------------------------------------------------------
typedef BOOL (WINAPI *FP_ConnectRS232         )(int , int)                        ; extern FP_ConnectRS232          PI_ConnectRS232          ;
typedef BOOL (WINAPI *FP_CloseConnection      )(int )                             ; extern FP_CloseConnection       PI_CloseConnection       ;
typedef BOOL (WINAPI *FP_qIDN                 )(int , char*, int)                 ; extern FP_qIDN                  PI_qIDN                  ;
typedef BOOL (WINAPI *FP_GcsCommandset        )(int , char*)                      ; extern FP_GcsCommandset         PI_GcsCommandset         ;
typedef BOOL (WINAPI *FP_GcsGetAnswer         )(int , char*,int)                  ; extern FP_GcsGetAnswer          PI_GcsGetAnswer          ;
typedef BOOL (WINAPI *FP_GcsGetAnswerSize     )(int , int*)                       ; extern FP_GcsGetAnswerSize      PI_GcsGetAnswerSize      ;
typedef BOOL (WINAPI *FP_IsMoving             )(int , char*,BOOL*)                ; extern FP_IsMoving              PI_IsMoving              ; //
typedef BOOL (WINAPI *FP_qFRF                 )(int , const char*,BOOL*)          ; extern FP_qFRF	            PI_qFRF                  ;
typedef long (WINAPI *FP_ConnectTCPIP         )(const char* , long )              ; extern FP_ConnectTCPIP          PI_ConnectTCPIP          ;
typedef long (WINAPI *FP_EnumerateTCPIPDevices)(char* , long , const char* )      ; extern FP_EnumerateTCPIPDevices PI_EnumerateTCPIPDevices ;
typedef BOOL (WINAPI *FP_FRF                  )(int , const char*)                ; extern FP_FRF                   PI_FRF                   ;
typedef BOOL (WINAPI *FP_IsControllerReady    )(int , int* )                      ; extern FP_IsControllerReady     PI_IsControllerReady     ;
typedef BOOL (WINAPI *FP_MOV                  )(int , const char* , const double*); extern FP_MOV                   PI_MOV                   ;
typedef BOOL (WINAPI *FP_MVR                  )(int , const char* , const double*); extern FP_MVR                   PI_MVR                   ;
typedef BOOL (WINAPI *FP_qPOS                 )(int , const char* , double* )     ; extern FP_qPOS                  PI_qPOS                  ;
typedef BOOL (WINAPI *FP_SPI                  )(int , const char* , const double*); extern FP_SPI                   PI_SPI                   ;
typedef BOOL (WINAPI *FP_qCMN                 )(int , const char* , double* )     ; extern FP_qCMN                  PI_qCMN                  ;
typedef BOOL (WINAPI *FP_qCMX                 )(int , const char* , double* )     ; extern FP_qCMX                  PI_qCMX                  ;
typedef int  (WINAPI *FP_GetError             )(int )                             ; extern FP_GetError              PI_GetError              ;
typedef BOOL (WINAPI *FP_qERR                 )(int , long* )                     ; extern FP_qERR                  PI_qERR                  ;
typedef BOOL (WINAPI *FP_TranslateError       )(int , char* , int )               ; extern FP_TranslateError        PI_TranslateError        ;
typedef BOOL (WINAPI *FP_qMOV                 )(int , const char* , double* )     ; extern FP_qMOV                  PI_qMOV                  ;
typedef BOOL (WINAPI *FP_SVO                  )(int , const char* , const BOOL* ) ; extern FP_SVO                   PI_SVO                   ;


enum ePI_Axis {
    paX = 0 ,
    paY = 1 ,
    paZ = 2 ,
    paU = 3 ,
    paV = 4 ,
    paW = 5 ,
    MAX_PI_AXIS
};
//Max +-12.5
//#define HEX_Z_WAIT -12.0
const double HEX_X_WAIT = 0     ;
const double HEX_Y_WAIT = 0     ;
const double HEX_U_WAIT = 0     ;
const double HEX_V_WAIT = 0     ;
const double HEX_W_WAIT = 0     ;

const double HEX_Z_WAIT = -10.0 ;
const double HEX_Z_LORI =   7.0 ;
const double HEX_Z_GET  =  12.0 ;
//#define HEX_Z_WAIT -12.0
//#define
//#define HEX_Z_GET   12.0

//디폴트피봇은 스테이지 탑면이 아닌
//스테이지 탑면에서 아래로 34미리 밑에 있다.
//자제 놓을때 탑면 측정해서 피봇을 바꿔준다. UV 움직이기 전에.
const double HEX_Z_PIVOT_STAGE_TOP     = 34.0 ;
const double HEX_Z_PIVOT_PLACE_ZIG_TOP = 134.0 ;
//#define HEX_Z_PIVOT_STAGE_TOP     34.0
//#define HEX_Z_PIVOT_PLACE_ZIG_TOP 134.0

//각축 +방향.
//X 장비전면 우측.
//Y 장비전면 후방.
//Z 장비전면 위쪽.
//U 장비좌측 기준 시계방향.
//V 장비전면 기준 시계방향.
//W 장비상면 기준 반시계방향.

class CHexaPot{
    public :
        CHexaPot();
        ~CHexaPot();

    protected :
        static bool LoadDll();
        static bool FreeDll();

        long m_iId ;

        String m_sLastErrMsg ;
        int    m_iLastErrCode;

        double m_daTrgPos[MAX_PI_AXIS]; //마지막 명령 포지션.
        double m_daResPos[MAX_PI_AXIS]; //Move함수에서 사용하는 포지션.
        double m_daEncPos[MAX_PI_AXIS]; //Move함수에서 사용하는 포지션.

        double m_daLastTrgPos[MAX_PI_AXIS]; //저장 포지션.


        bool   CheckErr(bool bRslt);

        bool   MoveTo(char* szAxis,double *dTarget); //한번에 모두 움직임.

    public :
        void   Update();

        bool   DoHome();
        void   MoveAbsRes(ePI_Axis _eAxis , double _dTarget); //쓰기편하게 한축씩 예약하고
        void   MoveIncRes(ePI_Axis _eAxis , double _dTarget); //쓰기편하게 한축씩 예약하고
        bool   Move(); //위에서 예약한 곳으로 동시에 이동.

        bool   GetStop();
        bool   GetStopInpos();
        bool   SetPivotPnt(double _dX , double _dY , double _dZ);
        double GetEncPos(ePI_Axis _eAxis);
        double GetTrgPos(ePI_Axis _eAxis);
        double GetLmtMinPos(ePI_Axis _eAxis);
        double GetLmtMaxPos(ePI_Axis _eAxis);
        bool   GetHomeDone();

        double MoveSaveTrgRes(ePI_Axis _eAxis);

        bool   SetServo(bool _bOn);

        String GetLastErrMsg();

        void Load(bool _bLoad);



};

extern CHexaPot HexaPot ;




#endif
