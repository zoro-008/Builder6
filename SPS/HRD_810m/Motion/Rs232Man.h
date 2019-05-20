//---------------------------------------------------------------------------

#ifndef Rs232ManH
#define Rs232ManH
//---------------------------------------------------------------------------
#include "uRs232c.h"
#include "DataMan.h"

class CRs232Man
{
    public:
        //Constructor
        CRs232Man (void);
        ~CRs232Man (void);

        void InitDispenser();
        void Close();
        void Shot();
        void SetTime(int _iTime);
        void ChangeCh(int _iCh);
        bool CheckShot();
    private:

    public:
        static void Dispenser(DWORD _cbInQue);
//        static void procComL(DWORD _cbInQue);
//        static void procComR(DWORD _cbInQue);

};
extern CRs232Man RSM;

extern TRS232C *Rs232_E ;
//extern TRS232C *Rs232_L ;
//extern TRS232C *Rs232_R ;
#endif
