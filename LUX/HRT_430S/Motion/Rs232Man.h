//---------------------------------------------------------------------------

#ifndef Rs232ManH
#define Rs232ManH
//---------------------------------------------------------------------------
#include "uRs232c.h"

class CRs232Man
{
    public:
        //Constructor
        CRs232Man (void);
        ~CRs232Man (void);

        void Init();
        void Close();
        void Shot(int _iNum);
        void SetTime(int _iNum , int _iTime);
        void ChangeCh(int _iNum , int _iCh);
        bool CheckShot(int _iNum);
    private:

    public:
        //static void DispenserL(DWORD _cbInQue);
        static void HeatProbe1Ch(DWORD _cbInQue);
        static void HeatProbe2Ch(DWORD _cbInQue);

};
extern CRs232Man RSM;

//extern TRS232C *Rs232_E ;




#endif
