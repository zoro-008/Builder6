//---------------------------------------------------------------------------

#ifndef PartBaseH
#define PartBaseH
//---------------------------------------------------------------------------

class CPartInterface
{
    public:
        virtual void Reset     (){}; //���V ��ư �������� Ÿ�� �Լ�.

        //Running Functions.
        virtual bool ToStopCon (){return false ;}; //��ž�� �ϱ� ���� ������ ���� �Լ�.
        virtual bool ToStartCon(){return false ;}; //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        virtual bool ToStart   (){return false ;}; //��ŸƮ�� �ϱ� ���� �Լ�.
        virtual bool ToStop    (){return false ;}; //��ž�� �ϱ� ���� �Լ�.
        virtual bool Autorun   (){return false ;}; //���䷱�׽ÿ� ��� Ÿ�� �Լ�.

        virtual int          GetHomeStep    (){return 0;}; virtual int GetPreHomeStep   (){return 0;}; virtual void InitHomeStep (){};
        virtual int          GetToStartStep (){return 0;}; virtual int GetPreToStartStep(){return 0;};
        virtual int          GetSeqStep     (){return 0;}  virtual int GetPreSeqStep    (){return 0;};
        virtual int          GetCycleStep   (){return 0;}; virtual int GetPreCycleStep  (){return 0;}; virtual void InitCycleStep(){};
        virtual int          GetToStopStep  (){return 0;}; virtual int GetPreToStopStep (){return 0;};

        virtual AnsiString   GetCycleName   (int _iSeq){return "";};
        virtual int          GetCycleTime   (int _iSeq){return 0 ;};
        virtual AnsiString   GetPartName    (         ){return "";};

        virtual int          GetCycleMaxCnt (         ){return 0 ;}; //�ش���Ʈ�� ����Ŭ ���� ����.
        virtual void  Update(){}


};

//---------------------------------------------------------------------------
#endif


