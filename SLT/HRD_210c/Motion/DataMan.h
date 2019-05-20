//---------------------------------------------------------------------------
#ifndef DataManH
#define DataManH

//---------------------------------------------------------------------------
#include "UtilDefine.h"
#include "Array.h"

///Max. Tray & Tool Pocket
//===========================================================================
extern bool g_bLocking ;
class CDataMan
{
    public:
        CArray ARAY[MAX_ARAY] ;

        __fastcall  CDataMan(void);
        __fastcall ~CDataMan(void);

        void Init();
        void Close();

    protected:
        //������ �� ������ �ǽɽ������� �ܺο��� UpdateAray�� ������ �Ѵ�.
        //TTimer * tmUpdate ;
        //void __fastcall tmUpdateTimer(TObject *Sender);

    public:
        //void __fastcall MakeArray(int _iCnt);
        //Clear.
        //static void SetManual(String _sId); //CallBack
        void __fastcall ClearMap(void);

        //Data Shift.
        void __fastcall ShiftArrayData(EN_ARAY_ID _iSrcId , EN_ARAY_ID _iDstId);

        //Data Load.
        void __fastcall LoadMap();
        void __fastcall SaveMap();

        //void __fastcall Load();
        //void __fastcall Save();

        void __fastcall UpdateAray();

};

extern CDataMan DM;
#endif
