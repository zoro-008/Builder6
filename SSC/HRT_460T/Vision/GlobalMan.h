//---------------------------------------------------------------------------
#ifndef GlobalManH
#define GlobalManH

#include <system.hpp>

//---------------------------------------------------------------------------
//Master Option.
struct CStat {
    String   sCrntJobFile ;
    CStat(){
        sCrntJobFile  = ""    ;
    }
    ~CStat(){
    }
} ;
//Master Option.
struct COptn {
    bool     bUseDeviceCal ; //false : ������ ���� �̸��극�̼� ���. true : �����Ϻ� �Ķ��Ÿ ���
    //String   sCrntJobFile ;
    COptn(){
        //bUseDeviceCal = false ;
   //     sCrntJobFile  = ""    ;
    }
    ~COptn(){
        int a ;
        a++ ;
    }
} ;

//---------------------------------------------------------------------------
class CGlobalMan
{
    public:
        CGlobalMan();
        virtual ~CGlobalMan();

    public:
        COptn Optn  ;
        CStat Stat  ;


        void Close(void);
        void Init(void);

        void Load  ();
        void Save  ();

        String GetGlobalFolder ();
        String GetJobFileFolder();

        void   SetCrntJobFile(String _sJobFileName);
        String GetCrntJobFile(                    );

        CRITICAL_SECTION csTsai  ; //Į���극�̼ǽÿ� ������ �Ѱ��ִ� ����̶�.. ������ ���� �������� ���� ������.
};

//---------------------------------------------------------------------------
extern CGlobalMan GM;

//---------------------------------------------------------------------------
#endif
