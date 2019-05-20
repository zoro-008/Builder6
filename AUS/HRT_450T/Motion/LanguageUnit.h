//---------------------------------------------------------------------------

#ifndef LanguageUnitH
#define LanguageUnitH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <Grids.hpp>

#include <Forms.hpp>
//---------------------------------------------------------------------------
#include "SMDllDefine.h"
class CLangUnit
{
    private:

    public:
        CLangUnit(void);
        ~CLangUnit(void);

    bool __fastcall LanguageSel (TForm *_Frm , EN_LAN_SEL lvType);
    bool __fastcall LanguageLoad(TForm *_Frm , EN_LAN_SEL lvType);
    bool __fastcall LanguageSave(TForm *_Frm , EN_LAN_SEL lvType);
};
extern CLangUnit LAG;
#endif
