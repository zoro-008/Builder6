//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOption.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
#include "SPCUnit.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOption *FrmOption;
//---------------------------------------------------------------------------
__fastcall TFrmOption::TFrmOption(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::btSaveClick(TObject *Sender)
{
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Warning" , "Can't Save during Autorunning!");
        return;
    }
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();

        //Display Setting.
    DM.ARAY[riRJ1].ClearDispItem();
    DM.ARAY[riRJ1].SetDispItem(csNone   ,"NoTray"   ) ;
    DM.ARAY[riRJ1].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;

    DM.ARAY[riRJ2].ClearDispItem();
    DM.ARAY[riRJ2].SetDispItem(csNone   ,"NoTray"   ) ;
    DM.ARAY[riRJ2].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;

    DM.ARAY[riRJ3].ClearDispItem();
    DM.ARAY[riRJ3].SetDispItem(csNone   ,"NoTray"   ) ;
    DM.ARAY[riRJ3].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;

    DM.ARAY[riRJ4].ClearDispItem();
    DM.ARAY[riRJ4].SetDispItem(csNone   ,"NoTray"   ) ;
    DM.ARAY[riRJ4].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;

    DM.ARAY[riZIG].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riZIG].SetDispItem(csWork   ,"Aligned"  ) ;

    DM.ARAY[riSRT].ClearDispItem();
    DM.ARAY[riSRT].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riSRT].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riSRT].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riSRT].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riSTF].ClearDispItem();
    DM.ARAY[riSTF].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riSTF].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riSTF].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riSTF].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riSTL].ClearDispItem();
    DM.ARAY[riSTL].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riSTL].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riSTL].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riSTL].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riSTB].ClearDispItem();
    DM.ARAY[riSTB].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riSTB].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riSTB].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riSTB].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riSTR].ClearDispItem();
    DM.ARAY[riSTR].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riSTR].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riSTR].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riSTR].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riMOS].ClearDispItem();
    DM.ARAY[riMOS].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riMOS].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riMOS].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riMOS].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riMVT].ClearDispItem();
    DM.ARAY[riMVT].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riMVT].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riMVT].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riMVT].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riMVS].ClearDispItem();
    DM.ARAY[riMVS].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riMVS].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riMVS].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iOSTOSTBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTOSTBin +1) +OM.GetBinName(OM.CmnOptn.iOSTOSTBin +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iOSTCrnBin +1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTCrnBin +1) +OM.GetBinName(OM.CmnOptn.iOSTCrnBin +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVTDFT2Bin +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFT2Bin +1) +OM.GetBinName(OM.CmnOptn.iVTDFT2Bin +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riMVS].SetDispItem(csGood   ,"Good"     ) ;






}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::UpdateComOptn(bool _bToTable)
{                                                                                   
    if (_bToTable == toTabl) {                                                      
        cbLoadingStop   -> Checked   = OM.CmnOptn.bLoadingStop    ;
        cbIgnrDoor      -> Checked   = OM.CmnOptn.bIgnrDoor       ;
        cbSkipOST       -> Checked   = OM.CmnOptn.bSkipOST        ;
        cbSkipVT        -> Checked   = OM.CmnOptn.bSkipVT         ;
        cbSkipVSN1      -> Checked   = OM.CmnOptn.bSkipVisn1      ;
        cbSkipVSN2      -> Checked   = OM.CmnOptn.bSkipVisn2      ;
        cbOSTNotUseCrnt -> Checked   = OM.CmnOptn.bOSTNotUseCrnt  ;
        cbOSTNotUseOst  -> Checked   = OM.CmnOptn.bOSTNotUseOst   ;


        cbSkip1stTol   -> Checked   = OM.CmnOptn.bSkip1stTol     ;
        cbSkip2ndTol   -> Checked   = OM.CmnOptn.bSkip2ndTol     ;
        cbSkip3thTol   -> Checked   = OM.CmnOptn.bSkip3thTol     ;
        cbSkip4thTol   -> Checked   = OM.CmnOptn.bSkip4thTol     ;

        cbFailIBLK     -> ItemIndex = OM.CmnOptn.iVTBLKBin       ;
        cbFailIDFT     -> ItemIndex = OM.CmnOptn.iVTDFTBin       ;
        cbFailIDFT2    -> ItemIndex = OM.CmnOptn.iVTDFT2Bin      ;
        cbFailIDIM     -> ItemIndex = OM.CmnOptn.iVTDIMBin       ;
        cbFailIOST     -> ItemIndex = OM.CmnOptn.iOSTOSTBin      ;
        cbFailICrnt    -> ItemIndex = OM.CmnOptn.iOSTCrnBin      ;
        cbFailIVH      -> ItemIndex = OM.CmnOptn.iVTVHBin        ;
        cbFailIVSN     -> ItemIndex = OM.CmnOptn.iVSNFailBin     ;

        edBfrVTPowrDelay -> Text    = OM.CmnOptn.iBfrVTPowrDelay ;
        edBfrVTInspDelay -> Text    = OM.CmnOptn.iBfrVTInspDelay ;

        edVTContBlkCnt   -> Text    = OM.CmnOptn.iVTContBlkCnt   ;
        edVTContDftCnt   -> Text    = OM.CmnOptn.iVTContDftCnt   ;
        edVTContDft2Cnt  -> Text    = OM.CmnOptn.iVTContDft2Cnt  ;
        edVTContVHCnt    -> Text    = OM.CmnOptn.iVTContVHCnt    ;


        edVSContNGCnt    -> Text    = OM.CmnOptn.iVSContNGCnt    ;
        edOSContNGCnt    -> Text    = OM.CmnOptn.iOSContNGCnt    ;
        edStgAftStopDelay-> Text    = OM.CmnOptn.iStgAftStopDelay;

        cbVisnAutoConv -> Checked   = OM.CmnOptn.bVisnAutoConv   ;

        edOSTLogDays     -> Text    = OM.CmnOptn.iOSTLogDays     ;




    }
    else {
        OM.CmnOptn.bLoadingStop     =  cbLoadingStop   -> Checked                  ;
        OM.CmnOptn.bIgnrDoor        =  cbIgnrDoor      -> Checked                  ;
        OM.CmnOptn.bSkipOST         =  cbSkipOST       -> Checked                  ;
        OM.CmnOptn.bSkipVT          =  cbSkipVT        -> Checked                  ;
        OM.CmnOptn.bSkipVisn1       =  cbSkipVSN1      -> Checked                  ;
        OM.CmnOptn.bSkipVisn2       =  cbSkipVSN2      -> Checked                  ;

        OM.CmnOptn.bOSTNotUseCrnt   =  cbOSTNotUseCrnt -> Checked                  ;
        OM.CmnOptn.bOSTNotUseOst    =  cbOSTNotUseOst  -> Checked                  ; 

        OM.CmnOptn.bSkip1stTol      =  cbSkip1stTol    -> Checked                  ;
        OM.CmnOptn.bSkip2ndTol      =  cbSkip2ndTol    -> Checked                  ;
        OM.CmnOptn.bSkip3thTol      =  cbSkip3thTol    -> Checked                  ;
        OM.CmnOptn.bSkip4thTol      =  cbSkip4thTol    -> Checked                  ;

        OM.CmnOptn.iVTBLKBin        =  cbFailIBLK      -> ItemIndex                ;
        OM.CmnOptn.iVTDFTBin        =  cbFailIDFT      -> ItemIndex                ;
        OM.CmnOptn.iVTDFT2Bin       =  cbFailIDFT2     -> ItemIndex                ;
        OM.CmnOptn.iVTDIMBin        =  cbFailIDIM      -> ItemIndex                ;
        OM.CmnOptn.iOSTOSTBin       =  cbFailIOST      -> ItemIndex                ;
        OM.CmnOptn.iOSTCrnBin       =  cbFailICrnt     -> ItemIndex                ;
        OM.CmnOptn.iVTVHBin         =  cbFailIVH       -> ItemIndex                ;
        OM.CmnOptn.iVSNFailBin      =  cbFailIVSN      -> ItemIndex                ;

        OM.CmnOptn.iBfrVTPowrDelay  = StrToIntDef(edBfrVTPowrDelay -> Text ,1000) ;
        OM.CmnOptn.iBfrVTInspDelay  = StrToIntDef(edBfrVTInspDelay -> Text ,1000) ;

        OM.CmnOptn.iVTContBlkCnt    = StrToIntDef(edVTContBlkCnt   -> Text ,10  ) ;
        OM.CmnOptn.iVTContDftCnt    = StrToIntDef(edVTContDftCnt   -> Text ,10  ) ;
        OM.CmnOptn.iVTContDft2Cnt   = StrToIntDef(edVTContDft2Cnt  -> Text ,10  ) ;
        OM.CmnOptn.iVTContVHCnt     = StrToIntDef(edVTContVHCnt    -> Text ,10  ) ;




        OM.CmnOptn.iVSContNGCnt     = StrToIntDef(edVSContNGCnt    -> Text ,10  ) ;
        OM.CmnOptn.iOSContNGCnt     = StrToIntDef(edOSContNGCnt    -> Text ,10  ) ;
        OM.CmnOptn.iStgAftStopDelay = StrToIntDef(edStgAftStopDelay-> Text ,0   ) ;

        OM.CmnOptn.bVisnAutoConv    = cbVisnAutoConv   -> Checked                 ;

        OM.CmnOptn.iOSTLogDays      = StrToIntDef(edOSTLogDays     -> Text ,1   ) ;


    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOption::FormShow(TObject *Sender)
{

    cbIgnrDoor -> Visible = FM_GetLevel() == lvMaster ;
    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------









void __fastcall TFrmOption::btRejectOptnClick(TObject *Sender)
{
    pnRejOpotn -> Visible = !pnRejOpotn -> Visible ;
}
//---------------------------------------------------------------------------
