//---------------------------------------------------------------------------
#ifndef DataManH
#define DataManH

//---------------------------------------------------------------------------
#include <ExtCtrls.hpp>
#include "UtilDefine.h"
#include "Array.h"

///Max. Tray & Tool Pocket
//===========================================================================

class CDataMan
{
    public:
        CArray ARAY[MAX_ARAY] ;

        __fastcall  CDataMan(void);
        __fastcall ~CDataMan(void);
        void Init();
        void Close();

    protected:
        TTimer * tmUpdate ;
        void __fastcall tmUpdateTimer(TObject *Sender);

    public:
        void __fastcall MakeArray(int _iCnt);
        //Clear.
        void __fastcall ClearMap(void);

        //Data Shift.
        void __fastcall ShiftArrayData(EN_ARAY_ID _iSrcId , EN_ARAY_ID _iDstId);

        //Data Load.
        void __fastcall LoadMap();
        void __fastcall SaveMap();

        //void __fastcall Load();
        //void __fastcall Save();

};

extern CDataMan DM;
#endif
