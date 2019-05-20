//---------------------------------------------------------------------------

#ifndef CommonH
#define CommonH
#include <Classes.hpp>

#include "VisnDefine.h"

//페인트 콜백...
typedef void (__closure * PaintCallbackFunc)();
typedef void (__closure * InspectionFunc   )();

//런 쓰레드
template <typename T>
class CRunThread
{
    class TRunThread : public TThread {
        private:
            T Func;
        public:
            __fastcall TRunThread(T func) : TThread(false) {
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute() {
                Func();
            }
    };

    public:
        CRunThread(T func) {
            TRunThread*  thread = new TRunThread(func);
        }
};


//전역 잡파일 -----------------------------------------------------------------
extern AnsiString g_sJobFileName ;
extern AnsiString g_sJobFilePath ;

//현제 모드 관련.
extern EN_VISN_ID       g_iActivVisnId      ;
extern bool             g_bSettingMode      ; //현제 세팅창 모드인지.
extern EN_TRAIN_PAGE_ID g_iActivTrainPageId ; //현제 세팅창에서 어떤 창 모드인지.
extern EN_OPER_LEVEL    g_iCrntLevel        ; //현제 레벨.

//화면 야매
extern int g_iYameRet ;



//프로젝트별 바뀌는 값들.
//==============================================================================
//조명 컨트롤러 갯수.----------------------------------------------------------
enum EN_LIGHT_ID {
    liAll = 0 ,    //3채널 1개로 다씀.

    MAX_LIGHT_ID
};


//카메라의 갯수.---------------------------------------------------------------
enum EN_CAM_ID {
    ciHed  = 0 , //카메라 1개 있음. 1600*1200 8비트.

    MAX_CAM_ID
};


//캘리브레이션 갯수.-----------------------------------------------------------
enum EN_CAL_ID {
    cdNone =-1 , //캘을 안쓰는 비젼.  캘 없음.

    MAX_CAL_ID
};

#endif
