//---------------------------------------------------------------------------

#ifndef SpcSubUnitH
#define SpcSubUnitH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <Series.hpp>
//---------------------------------------------------------------------------
#include "DataMan.h"
class CSpcSub
{
    private:
    public :
        __fastcall CSpcSub:: CSpcSub(void);
        __fastcall CSpcSub::~CSpcSub(void);

        //Rotor Height Inspection
        void ClearData();
        //void __fastcall WriteDataCsv();
        //void __fastcall WritePlaceDataCsv(AnsiString _sData);
        //bool __fastcall WriteHeightData(String sFileName);
        //bool __fastcall ReadHeightData (String sPath , TStringGrid * _sgMap);

        void __fastcall DelPastLotLog();

};

extern CSpcSub SPCS;


#endif
