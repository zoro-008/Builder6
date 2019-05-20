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
    bool     bUseDeviceCal ; //false : 비젼별 공용 켈리브레이션 사용. true : 잡파일별 파라메타 사용
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

        CRITICAL_SECTION csTsai  ; //칼리브레이션시에 포인터 넘겨주는 방식이라.. 쓰레드 사용시 비전간에 같이 접근함.
};

//---------------------------------------------------------------------------
extern CGlobalMan GM;

//---------------------------------------------------------------------------
#endif
