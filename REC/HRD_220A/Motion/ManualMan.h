//---------------------------------------------------------------------------
#ifndef ManualManH
#define ManualManH

#include "UtilDefine.h"
#include "Timer.h"

//---------------------------------------------------------------------------
enum EN_SORT_POS_ID {
    spNone  = 0 , //���� �����ǿ��� ��,�÷��̽�.
    spLDR       ,
    spZIG       ,
    spSTG       ,
    spRJ1       ,
    spRJ2       ,
    spRJ3       ,
    spRJ4       ,
    spULD       ,
    MAX_SORT_POS_ID
};






class CManualMan
{
    private:
        //Object.
        int             m_iManStep , m_iPreManStep ;
        EN_MANUAL_CYCLE m_iManNo   ;

        CDelayTimer     m_tmCycle   ; //�޴��� ����Ŭ Ÿ�Ӿƿ���.
        CDelayTimer     m_tmDelay   ; //�޴��� ����Ŭ


        bool            m_bManSetting ; //��������.
        bool            m_bStdCalPick ; //Picker StdCal�� �� Pick ���� �ϱ� ����.


        double  m_dVisnRsltGapX ;
        double  m_dVisnRsltGapY ;
        double  m_dVisnRsltGapT ;

    public:
        CManualMan(void);
        ~CManualMan(void);

        //Init.
        void Init (void);
        void Close(void);
        void Reset(void){Init();}

        EN_MANUAL_CYCLE  GetManNo  ();
        int              GetManStep();

        //Manual Processing.
        bool SetManCycle(EN_MANUAL_CYCLE _iNo);
        void Update();

        //ManCycle.
        bool CycleVisnCal(); //��Ŀ�� ����ο��� ��� �����˻��ϰ� ���������� ���� ���� At�������� �˻��غ��� �����V�� �󸶳� �߻��ϴ��� Ȯ��.
        bool CycleAlgnPlce(); //����ο� ���� �޴���.
        bool CycleStgFlatness();


        bool GetVisnGap(double * _dVisnX , double * _dVisnY , double * _dVisnT);

        double m_dStgFlatness1;
        double m_dStgFlatness2;
        double m_dStgFlatness3;
        double m_dStgFlatness4;
} ;

//---------------------------------------------------------------------------
extern CManualMan MM;

//---------------------------------------------------------------------------
#endif
