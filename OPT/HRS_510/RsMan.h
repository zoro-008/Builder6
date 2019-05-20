//---------------------------------------------------------------------------

#ifndef RsManH
#define RsManH
//---------------------------------------------------------------------------
#include "uRs232c.h"
class CRsMan
{
    public:
        //Constructor
        CRsMan (void);
        ~CRsMan (void);

        void Init();
        void Close();

    private:

    public:
        static void procCom1(DWORD _cbInQue);
        static void procCom2(DWORD _cbInQue);
        static void procCom3(DWORD _cbInQue);
};
extern TRS232C *RS232C1 ;
extern TRS232C *RS232C2 ;
extern TRS232C *RS232C3 ;
#endif
