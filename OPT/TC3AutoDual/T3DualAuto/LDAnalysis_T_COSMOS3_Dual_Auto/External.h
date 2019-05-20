#ifndef ExternalH
#define ExternalH


//#include "MainFrm.h"
//#include "ImgDisplay.h"
//#include "ImgDisplay2.h"
//#include "ImgDisplay3.h"
//#include "TitleBar.h"


extern BOOL gLockKeyFlag;

/*
Vision -> Motion
(VC01)             //ALLHOME//Btn
(VC02)             //RESET//Btn
(VC03)             //CLOSE AP
(VC04)<좌표,좌표,OK/NG>  //검사결과.//cmd 여기서만 Para1,2,3 들어감. Para3이 OK = 1, NG = 2
(VC05)             //UI Show//Btn
 
Motion -> Vision
(MC01)             //검사.
*/
//테스트가 normal , retest 3rdtest로 되어있음.
enum EN_TEST_MODE {
    tmNormal  = 0 ,
    tmReTest  = 1 ,
	tm3rdTest = 2 ,

	MAX_TEST_MODE
};


extern EN_TEST_MODE g_iTestMode ;
struct MOTION_RESULT_DATA
{
	int		nAngleOkNg;  //Angle.  0 = Ok , 1 = NG
	int     nSizeOkNg;   //
	double	dAngleX;
	double	dAngleY;

	int		nInlineROkNg;   //인라인 좌우값 결과.
	double	dInlineRShift;   //인라인 좌우값  


	int		nGauVirOkNg; //APC
	int		nGauHorOkNg;
	double	dVerticalCMD;
	double	dHosizontalCMD;

	int     nBeamOkNg;   //Beam
    double  dRShift  ;
	double  dTShift  ;

	//Server Log.
    double  dAngle             ;
	double  dFA_R              ;
    double  dFA_T              ;
    double  dThree_Beam_Tilt   ;
    double  dBeam_Size         ;
    double  dGT_Shift          ;
    double  dArea              ;
    double  dPeak_Ratio        ;
    double  dParallelism       ;
    double  dAbsolute_Center_X ;
    double  dAbsolute_Center_Y ;
    double  dBeam_Center_X     ;
    double  dBeam_Center_Y     ;
    double  dPeak_Center_X     ;
    double  dPeak_Center_Y     ;
    double  dBeam_Shift        ;
    double  dBeam_Shift_R      ;
    double  dBeam_Shift_T      ;
};


/*
struct MOTION_RESULT_DATA
{
	int		nAngleOkNg;
	double	dAngleX;
	double	dAngleY;

	int		nInlineOkNg;
	double	dInlineShift;

	int		nGauVirOkNg;
	int		nGauHorOkNg;
	double	dVerticalCMD;
	double	dHosizontalCMD;

	
	//Motion Recieve
	BOOL	bInspection;//1이면 검사, 2이면 ready
	
	BOOL	bModeSelectAuto;
//	BOOL	bModeChange;
	BOOL	bMessageRequest;
	CString csMessage;

};
*/
extern MOTION_RESULT_DATA gMotionResultData;

extern long  g_nSingleTestThread;

extern BOOL g_bLeftRight;

extern bool g_bEndding ;




#endif
