//---------------------------------------------------------------------------

#ifndef PartBaseH
#define PartBaseH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"

//---------------------------------------------------------------------------

class CPartInterface
{
    public:
        virtual void Reset     (){}; //리셑 버튼 눌렀을때 타는 함수.

        //Running Functions.
        virtual bool ToStopCon (){}; //스탑을 하기 위한 조건을 보는 함수.
        virtual bool ToStartCon(){}; //스타트를 하기 위한 조건을 보는 함수.
        virtual bool ToStart   (){}; //스타트를 하기 위한 함수.
        virtual bool ToStop    (){}; //스탑을 하기 위한 함수.
        virtual bool Autorun   (){}; //오토런닝시에 계속 타는 함수.

        virtual AnsiString   GetPartName    (){};

        virtual int          GetHomeStep    (){}; virtual int GetPreHomeStep   (){};
        virtual int          GetToStartStep (){}; virtual int GetPreToStartStep(){};
        virtual int          GetCycleStep   (){}; virtual int GetPreCycleStep  (){};
        virtual int          GetToStopStep  (){}; virtual int GetPreToStopStep (){};
        virtual AnsiString   GetCycleName   (){};
        virtual AnsiString   GetPartName    (){};

};

//---------------------------------------------------------------------------
#endif
