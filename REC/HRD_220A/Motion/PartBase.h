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
        virtual void Reset     (){}; //���V ��ư �������� Ÿ�� �Լ�.

        //Running Functions.
        virtual bool ToStopCon (){}; //��ž�� �ϱ� ���� ������ ���� �Լ�.
        virtual bool ToStartCon(){}; //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        virtual bool ToStart   (){}; //��ŸƮ�� �ϱ� ���� �Լ�.
        virtual bool ToStop    (){}; //��ž�� �ϱ� ���� �Լ�.
        virtual bool Autorun   (){}; //���䷱�׽ÿ� ��� Ÿ�� �Լ�.

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
