//---------------------------------------------------------------------------

#ifndef PartBaseH
#define PartBaseH
//---------------------------------------------------------------------------

class CPartInterface
{
    public:
        virtual void Reset     (){}; //리셑 버튼 눌렀을때 타는 함수.

        //Running Functions.
        virtual bool ToStopCon (){return false ;}; //스탑을 하기 위한 조건을 보는 함수.
        virtual bool ToStartCon(){return false ;}; //스타트를 하기 위한 조건을 보는 함수.
        virtual bool ToStart   (){return false ;}; //스타트를 하기 위한 함수.
        virtual bool ToStop    (){return false ;}; //스탑을 하기 위한 함수.
        virtual bool Autorun   (){return false ;}; //오토런닝시에 계속 타는 함수.

        virtual int          GetHomeStep    (){return 0;}; virtual int GetPreHomeStep   (){return 0;}; virtual void InitHomeStep (){};
        virtual int          GetToStartStep (){return 0;}; virtual int GetPreToStartStep(){return 0;};
        virtual int          GetSeqStep     (){return 0;}  virtual int GetPreSeqStep    (){return 0;};
        virtual int          GetCycleStep   (){return 0;}; virtual int GetPreCycleStep  (){return 0;}; virtual void InitCycleStep(){};
        virtual int          GetToStopStep  (){return 0;}; virtual int GetPreToStopStep (){return 0;};

        virtual AnsiString   GetCycleName   (int _iSeq){return "";};
        virtual int          GetCycleTime   (int _iSeq){return 0 ;};
        virtual AnsiString   GetPartName    (         ){return "";};

        virtual int          GetCycleMaxCnt (         ){return 0 ;}; //해당파트의 싸이클 갯수 리턴.
        virtual void  Update(){}


};

//---------------------------------------------------------------------------
#endif


