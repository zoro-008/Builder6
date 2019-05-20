/*
Vision -> Motion
(VC01)             //ALLHOME//Btn
(VC02)             //RESET//Btn
(VC03)             //CLOSE AP
(VC04)<좌표,좌표,OK/NG>  //검사결과.//cmd 여기서만 Para1,2,3 들어감. Para3이 OK = 1, NG = 2
(VC05)             //UI Show//Btn
(VC06)				//TEST ON
(VC07)				//TEST OFF
(VC08)				//MOTION_APC_LEFT
(VC09)				//MOTION_APC_RIGHT
(VC10)				//APC POWER QUANTITY UP
(VC11)				//APC POWER QUANTITY DOWN
 
Motion -> Vision
(MC01)             //검사.
*/

#define MOTION_ALL_HOME			1
#define MOTION_RESET			2
#define MOTION_UI_CLOSE			3
#define MOTION_POSITION			4
#define MOTION_UI_SHOW			5
#define MOTION_TEST_ON			6
#define MOTION_TEST_OFF			7
#define MOTION_APC_LEFT			8
#define MOTION_APC_RIGHT		9
#define MOTION_APC_POWER_UP		10
#define MOTION_APC_POWER_DOWN	11

#define VISION_OK		0
#define VISION_NG		1 //
#define VISION_ERROR	2 //빔을 못찾음.
#define VISION_SIZENG   3 //빔의 크기가 큼.

#define VISION_INSPECTION	1
#define VISION_READY		2

#define CENTER_OF_GAUSSIAN_HIGH_LOW	((m_cSettingDataEx.iApcOk + m_cSettingDataEx.iApcOkHigh)/2.0)

#define MUTEX_VISION "Motion Mutex"
#define WIN_NAME  "TCosmos3MotionManager"