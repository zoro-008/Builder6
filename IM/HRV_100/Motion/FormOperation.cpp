//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "stdlib.h"


#include "FormOperation.h"
#include "FormLotOpen.h"
#include "FormMain.h"

//---------------------------------------------------------------------------
#include "Loader.h"
#include "LoaderSupply.h"
#include "Sort.h"
#include "Stage.h"

#include "FormVacErr.h"

//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "Sequence.h"

#include "ManualMan.h"
#include "SPCUnit.h"
#include "PstnMan.h"
#include "UserIni.h"

//#include "SortCmn.h"

//#include "SAInterface.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmOperation *FrmOperation;
//---------------------------------------------------------------------------
__fastcall TFrmOperation::TFrmOperation(TComponent* Owner)                       // 실행 시킬 때, 한 번만 실행하는 부분
    : TForm(Owner)
{
    DM.ARAY[riSRT].SetParent(pnSRT      );  DM.ARAY[riSRT].ClearDispItem();  DM.ARAY[riSRT].SetCellSize(false); DM.ARAY[riSRT].SetName("Sort"        );
    DM.ARAY[riZIG].SetParent(pnZIG      );  DM.ARAY[riZIG].ClearDispItem();  DM.ARAY[riZIG].SetCellSize(false); DM.ARAY[riZIG].SetName("Aligner"     );
    DM.ARAY[riSTF].SetParent(pnSGFrnt   );  DM.ARAY[riSTF].ClearDispItem();  DM.ARAY[riSTF].SetCellSize(false); DM.ARAY[riSTF].SetName("Front"       );
    DM.ARAY[riSTL].SetParent(pnSGLeft   );  DM.ARAY[riSTL].ClearDispItem();  DM.ARAY[riSTL].SetCellSize(false); DM.ARAY[riSTL].SetName("Left"        );
    DM.ARAY[riSTB].SetParent(pnSGRear   );  DM.ARAY[riSTB].ClearDispItem();  DM.ARAY[riSTB].SetCellSize(false); DM.ARAY[riSTB].SetName("Back"        );
    DM.ARAY[riSTR].SetParent(pnSGRght   );  DM.ARAY[riSTR].ClearDispItem();  DM.ARAY[riSTR].SetCellSize(false); DM.ARAY[riSTR].SetName("Right"       );
    DM.ARAY[riRJ1].SetParent(pnRJ1      );  DM.ARAY[riRJ1].ClearDispItem();  DM.ARAY[riRJ1].SetCellSize(false); DM.ARAY[riRJ1].SetName("Reject1"     );
    DM.ARAY[riRJ2].SetParent(pnRJ2      );  DM.ARAY[riRJ2].ClearDispItem();  DM.ARAY[riRJ2].SetCellSize(false); DM.ARAY[riRJ2].SetName("Reject2"     );
    DM.ARAY[riRJ3].SetParent(pnRJ3      );  DM.ARAY[riRJ3].ClearDispItem();  DM.ARAY[riRJ3].SetCellSize(false); DM.ARAY[riRJ3].SetName("Reject3"     );
    DM.ARAY[riRJ4].SetParent(pnRJ4      );  DM.ARAY[riRJ4].ClearDispItem();  DM.ARAY[riRJ4].SetCellSize(false); DM.ARAY[riRJ4].SetName("Reject4"     );
    DM.ARAY[riPRL].SetParent(pnLSP      );  DM.ARAY[riPRL].ClearDispItem();  DM.ARAY[riPRL].SetCellSize(false); DM.ARAY[riPRL].SetName("PreLoader"   );
    DM.ARAY[riLDR].SetParent(pnLDR      );  DM.ARAY[riLDR].ClearDispItem();  DM.ARAY[riLDR].SetCellSize(false); DM.ARAY[riLDR].SetName("Loader"      );
    DM.ARAY[riPSL].SetParent(pnLST      );  DM.ARAY[riPSL].ClearDispItem();  DM.ARAY[riPSL].SetCellSize(false); DM.ARAY[riPSL].SetName("PostLoader"  );
    DM.ARAY[riPRU].SetParent(pnUSP      );  DM.ARAY[riPRU].ClearDispItem();  DM.ARAY[riPRU].SetCellSize(false); DM.ARAY[riPRU].SetName("PreUnloader" );
    DM.ARAY[riULD].SetParent(pnULD      );  DM.ARAY[riULD].ClearDispItem();  DM.ARAY[riULD].SetCellSize(false); DM.ARAY[riULD].SetName("Unloader"    );
    DM.ARAY[riPSU].SetParent(pnUST      );  DM.ARAY[riPSU].ClearDispItem();  DM.ARAY[riPSU].SetCellSize(false); DM.ARAY[riPSU].SetName("PostUnloader");
    DM.ARAY[riMOS].SetParent(pnManRsltOS);  DM.ARAY[riMOS].ClearDispItem();  DM.ARAY[riMOS].SetCellSize(false); DM.ARAY[riMOS].SetName("ManOS"       );
    DM.ARAY[riMVT].SetParent(pnManRsltVT);  DM.ARAY[riMVT].ClearDispItem();  DM.ARAY[riMVT].SetCellSize(false); DM.ARAY[riMVT].SetName("ManVT"       );
    DM.ARAY[riMVS].SetParent(pnManRsltVS);  DM.ARAY[riMVS].ClearDispItem();  DM.ARAY[riMVS].SetCellSize(false); DM.ARAY[riMVS].SetName("ManVS"       );


        //Display Setting.
    DM.ARAY[riRJ1].ClearDispItem();
    DM.ARAY[riRJ1].SetDispItem(csNone   ,"NoTray"   ) ;
    DM.ARAY[riRJ1].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riRJ1].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;

    DM.ARAY[riRJ2].ClearDispItem();
    DM.ARAY[riRJ2].SetDispItem(csNone   ,"NoTray"   ) ;
    DM.ARAY[riRJ2].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riRJ2].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;

    DM.ARAY[riRJ3].ClearDispItem();
    DM.ARAY[riRJ3].SetDispItem(csNone   ,"NoTray"   ) ;
    DM.ARAY[riRJ3].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riRJ3].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;

    DM.ARAY[riRJ4].ClearDispItem();
    DM.ARAY[riRJ4].SetDispItem(csNone   ,"NoTray"   ) ;
    DM.ARAY[riRJ4].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riRJ4].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;

    DM.ARAY[riZIG].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riZIG].SetDispItem(csWork   ,"Aligned"  ) ;

    DM.ARAY[riSRT].ClearDispItem();
    DM.ARAY[riSRT].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riSRT].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riSRT].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riSRT].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riSRT].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riSTF].ClearDispItem();
    DM.ARAY[riSTF].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riSTF].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riSTF].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riSTF].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riSTF].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riSTL].ClearDispItem();
    DM.ARAY[riSTL].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riSTL].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riSTL].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riSTL].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riSTL].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riSTB].ClearDispItem();
    DM.ARAY[riSTB].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riSTB].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riSTB].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riSTB].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riSTB].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riSTR].ClearDispItem();
    DM.ARAY[riSTR].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riSTR].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riSTR].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riSTR].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riSTR].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riMOS].ClearDispItem();
    DM.ARAY[riMOS].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riMOS].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riMOS].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riMOS].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riMOS].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riMVT].ClearDispItem();
    DM.ARAY[riMVT].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riMVT].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riMVT].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riMVT].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riMVT].SetDispItem(csGood   ,"Good"     ) ;

    DM.ARAY[riMVS].ClearDispItem();
    DM.ARAY[riMVS].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riMVS].SetDispItem(csUnkwn  ,"Unknwn"   ) ;
    DM.ARAY[riMVS].SetDispItem(csWork   ,"Aligned"  ) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iOSTFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iOSTFailBin+1) +OM.GetBinName(OM.CmnOptn.iOSTFailBin+1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVTBLKBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTBLKBin  +1) +OM.GetBinName(OM.CmnOptn.iVTBLKBin  +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVTDFTBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDFTBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDFTBin  +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVTDIMBin  +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTDIMBin  +1) +OM.GetBinName(OM.CmnOptn.iVTDIMBin  +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVTVHBin   +1  ,"Bin"+AnsiString(OM.CmnOptn.iVTVHBin   +1) +OM.GetBinName(OM.CmnOptn.iVTVHBin   +1)) ;
    DM.ARAY[riMVS].SetDispItem(OM.CmnOptn.iVSNFailBin+1  ,"Bin"+AnsiString(OM.CmnOptn.iVSNFailBin+1) +OM.GetBinName(OM.CmnOptn.iVSNFailBin+1)) ;
    DM.ARAY[riMVS].SetDispItem(csGood   ,"Good"     ) ;










    DM.ARAY[riPRL].SetDispItem(csNone   ,"NoMgz"    ) ;
    DM.ARAY[riPRL].SetDispItem(csUnkwn  ,"Unknown"  ) ;

    DM.ARAY[riLDR].SetDispItem(csNone   ,"NoMgz"    ) ;
    DM.ARAY[riLDR].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riLDR].SetDispItem(csUnkwn  ,"Unknown"  ) ;

    DM.ARAY[riPSL].SetDispItem(csNone   ,"NoMgz"    ) ;
    DM.ARAY[riPSL].SetDispItem(csEmpty  ,"Empty"    ) ;

    DM.ARAY[riPRU].SetDispItem(csNone   ,"NoMgz"    ) ;
    DM.ARAY[riPRU].SetDispItem(csEmpty  ,"Empty"    ) ;

    DM.ARAY[riULD].SetDispItem(csNone   ,"NoMgz"    ) ;
    DM.ARAY[riULD].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riULD].SetDispItem(csGood   ,"Good "    ) ;

    DM.ARAY[riPSU].SetDispItem(csNone   ,"NoMgz"    ) ;
    DM.ARAY[riPSU].SetDispItem(csEmpty  ,"Empty"    ) ;
    DM.ARAY[riPSU].SetDispItem(csUnkwn  ,"Unknown"  ) ;

    DM.LoadMap();

    TUserINI   UserINI;
    AnsiString sPath  ;
    int iLang         ;
    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    UserINI.Load(sPath , "Member"     , "m_iLangSel"  , iLang);

    const bool bKorean = true;

    //나중에 추가되면 Visible 바꾸기.

    btMan1_1  -> Caption = "[" + AnsiString(mcLSP_CycleSupply    ) + "]" + (bKorean ? "로더 공급"          : "LSP Supply"     );  btMan1_1  -> Tag = (int)mcLSP_CycleSupply     ; btMan1_1  -> Visible = true  ;
    btMan1_2  -> Caption = "[" + AnsiString(mcLDR_CycleIn        ) + "]" + (bKorean ? "로더 진입"          : "LDR In"         );  btMan1_2  -> Tag = (int)mcLDR_CycleIn         ; btMan1_2  -> Visible = true  ;
    btMan1_3  -> Caption = "[" + AnsiString(mcLDR_CycleOut       ) + "]" + (bKorean ? "로더 배출"          : "LDR Out"        );  btMan1_3  -> Tag = (int)mcLDR_CycleOut        ; btMan1_3  -> Visible = true  ;
    btMan1_4  -> Caption = "[" + AnsiString(mcLST_CycleStock     ) + "]" + (bKorean ? "로더 스탁"          : "LST Stock"      );  btMan1_4  -> Tag = (int)mcLST_CycleStock      ; btMan1_4  -> Visible = true  ;
    btMan1_8  -> Caption = "[" + AnsiString(mcUSP_CycleSupply    ) + "]" + (bKorean ? "언로더공급"         : "USP Supply"     );  btMan1_8  -> Tag = (int)mcUSP_CycleSupply     ; btMan1_8  -> Visible = true  ;
    btMan1_9  -> Caption = "[" + AnsiString(mcULD_CycleIn        ) + "]" + (bKorean ? "언로더진입"         : "UDR In"         );  btMan1_9  -> Tag = (int)mcULD_CycleIn         ; btMan1_9  -> Visible = true  ;
    btMan1_10 -> Caption = "[" + AnsiString(mcULD_CycleOut       ) + "]" + (bKorean ? "언로더배출"         : "UDR Out"        );  btMan1_10 -> Tag = (int)mcULD_CycleOut        ; btMan1_10 -> Visible = true  ;
    btMan1_11 -> Caption = "[" + AnsiString(mcUST_CycleStock     ) + "]" + (bKorean ? "언로더스탁"         : "UST Stock"      );  btMan1_11 -> Tag = (int)mcUST_CycleStock      ; btMan1_11 -> Visible = true  ;

    btMan2_1  -> Caption = "[" + AnsiString(mcSRT_CyclePickLDR   ) + "]" + (bKorean ? "로더집기"           : "PickLDR"        );  btMan2_1  -> Tag = (int)mcSRT_CyclePickLDR    ; btMan2_1  -> Visible = true ;
    btMan2_2  -> Caption = "[" + AnsiString(mcSRT_CyclePlceLDR   ) + "]" + (bKorean ? "로더놓기"           : "PlceLDR"        );  btMan2_2  -> Tag = (int)mcSRT_CyclePlceLDR    ; btMan2_2  -> Visible = true ;
    btMan2_3  -> Caption = "[" + AnsiString(mcSRT_CyclePickSTG   ) + "]" + (bKorean ? "스테이지집기"       : "PickSTG"        );  btMan2_3  -> Tag = (int)mcSRT_CyclePickSTG    ; btMan2_3  -> Visible = true ;
    btMan2_4  -> Caption = "[" + AnsiString(mcSRT_CyclePlceSTG   ) + "]" + (bKorean ? "스테이지놓기"       : "PlceSTG"        );  btMan2_4  -> Tag = (int)mcSRT_CyclePlceSTG    ; btMan2_4  -> Visible = true ;
    btMan2_5  -> Caption = "[" + AnsiString(mcSRT_CyclePickRJT1  ) + "]" + (bKorean ? "리젝1집기"          : "PickRJT1"       );  btMan2_5  -> Tag = (int)mcSRT_CyclePickRJT1   ; btMan2_5  -> Visible = true ;
    btMan2_6  -> Caption = "[" + AnsiString(mcSRT_CyclePlceRJT1  ) + "]" + (bKorean ? "리젝1놓기"          : "PlceRJT1"       );  btMan2_6  -> Tag = (int)mcSRT_CyclePlceRJT1   ; btMan2_6  -> Visible = true ;
    btMan2_7  -> Caption = "[" + AnsiString(mcSRT_CyclePickRJT4  ) + "]" + (bKorean ? "리젝4집기"          : "PickRJT4"       );  btMan2_7  -> Tag = (int)mcSRT_CyclePickRJT4   ; btMan2_7  -> Visible = true ;
    btMan2_8  -> Caption = "[" + AnsiString(mcSRT_CyclePlceRJT4  ) + "]" + (bKorean ? "리젝4놓기"          : "PlceRJT4"       );  btMan2_8  -> Tag = (int)mcSRT_CyclePlceRJT4   ; btMan2_8  -> Visible = true ;
    btMan2_9  -> Caption = "[" + AnsiString(mcSRT_CyclePickULD   ) + "]" + (bKorean ? "언로더집기"         : "PickULD"        );  btMan2_9  -> Tag = (int)mcSRT_CyclePickULD    ; btMan2_9  -> Visible = true ;
    btMan2_10 -> Caption = "[" + AnsiString(mcSRT_CyclePlceULD   ) + "]" + (bKorean ? "언로더놓기"         : "PlceULD"        );  btMan2_10 -> Tag = (int)mcSRT_CyclePlceULD    ; btMan2_10 -> Visible = true ;
    btMan2_11 -> Caption = "[" + AnsiString(mcSRT_CyclePickZIG   ) + "]" + (bKorean ? "지그집기"           : "PickZIG"        );  btMan2_11 -> Tag = (int)mcSRT_CyclePickZIG    ; btMan2_11 -> Visible = true ;
    btMan2_12 -> Caption = "[" + AnsiString(mcSRT_CyclePlceZIG   ) + "]" + (bKorean ? "지그놓기"           : "PlceZIG"        );  btMan2_12 -> Tag = (int)mcSRT_CyclePlceZIG    ; btMan2_12 -> Visible = true ;
    btMan2_13 -> Caption = "[" + AnsiString(mcSRT_CycleMoveSTG   ) + "]" + (bKorean ? "스테이지로이동"     : "MovePocket"     );  btMan2_13 -> Tag = (int)mcSRT_CycleMoveSTG    ; btMan2_13 -> Visible = true ;

    btMan3_1  -> Caption = "[" + AnsiString(mcSTG_Turn90         ) + "]" + (bKorean ? "90도회전"           : "Turn90 "        );  btMan3_1  -> Tag = (int)mcSTG_Turn90          ; btMan3_1  -> Visible = true  ;
    btMan3_2  -> Caption = "[" + AnsiString(mcSTG_Turn90N        ) + "]" + (bKorean ? "-90도회전"          : "Turn90N"        );  btMan3_2  -> Tag = (int)mcSTG_Turn90N         ; btMan3_2  -> Visible = true  ;
    btMan3_3  -> Caption = "[" + AnsiString(mcSTG_Turn45         ) + "]" + (bKorean ? "45도회전"           : "Turn45 "        );  btMan3_3  -> Tag = (int)mcSTG_Turn45          ; btMan3_3  -> Visible = true  ;
    btMan3_4  -> Caption = "[" + AnsiString(mcSTG_Turn45N        ) + "]" + (bKorean ? "-45도회전"          : "Turn45N"        );  btMan3_4  -> Tag = (int)mcSTG_Turn45N         ; btMan3_4  -> Visible = true  ;
    btMan3_5  -> Caption = "[" + AnsiString(mcOST_CycleWork      ) + "]" + (bKorean ? "OST작업"            : "OST_CycleWork"  );  btMan3_5  -> Tag = (int)mcOST_CycleWork       ; btMan3_5  -> Visible = true  ;
    btMan3_6  -> Caption = "[" + AnsiString(mcOST_PshrDnUp       ) + "]" + (bKorean ? "OST푸셔"            : "OST_PshrDnUp"   );  btMan3_6  -> Tag = (int)mcOST_PshrDnUp        ; btMan3_6  -> Visible = true  ;
    btMan3_7  -> Caption = "[" + AnsiString(mcOST_PTolUpDn       ) + "]" + (bKorean ? "OST핀툴"            : "OST_PTolUpDn"   );  btMan3_7  -> Tag = (int)mcOST_PTolUpDn        ; btMan3_7  -> Visible = true  ;
    btMan3_8  -> Caption = "[" + AnsiString(mcVTI_CycleWork      ) + "]" + (bKorean ? "VT작업"             : "VTI_CycleWork"  );  btMan3_8  -> Tag = (int)mcVTI_CycleWork       ; btMan3_8  -> Visible = true  ;
    btMan3_9  -> Caption = "[" + AnsiString(mcVTI_PshrDnUp       ) + "]" + (bKorean ? "VT푸셔"             : "VTI_PshrDnUp"   );  btMan3_9  -> Tag = (int)mcVTI_PshrDnUp        ; btMan3_9  -> Visible = true  ;
    btMan3_10 -> Caption = "[" + AnsiString(mcVTI_PTolUpDn       ) + "]" + (bKorean ? "VT핀툴"             : "VTI_PTolUpDn"   );  btMan3_10 -> Tag = (int)mcVTI_PTolUpDn        ; btMan3_10 -> Visible = true  ;
    btMan3_11 -> Caption = "[" + AnsiString(mcVTI_MainPwr        ) + "]" + (bKorean ? "VT파워"             : "VTI_MainPwr"    );  btMan3_11 -> Tag = (int)mcVTI_MainPwr         ; btMan3_11 -> Visible = true  ;

    btMan4_1  -> Caption = "[" + AnsiString(mcSTG_Turn90         ) + "]" + (bKorean ? "90도회전"           : "Turn90"         );  btMan4_1  -> Tag = (int)mcSTG_Turn90          ; btMan4_1  -> Visible = true  ;
    btMan4_2  -> Caption = "[" + AnsiString(mcSTG_Turn90N        ) + "]" + (bKorean ? "-90도회전"          : "Turn90N"        );  btMan4_2  -> Tag = (int)mcSTG_Turn90N         ; btMan4_2  -> Visible = true  ;
    btMan4_3  -> Caption = "[" + AnsiString(mcSTG_Turn45         ) + "]" + (bKorean ? "45도회전"           : "Turn45"         );  btMan4_3  -> Tag = (int)mcSTG_Turn45          ; btMan4_3  -> Visible = true  ;
    btMan4_4  -> Caption = "[" + AnsiString(mcSTG_Turn45N        ) + "]" + (bKorean ? "-45도회전"          : "Turn45N"        );  btMan4_4  -> Tag = (int)mcSTG_Turn45N         ; btMan4_4  -> Visible = true  ;
    btMan4_5  -> Caption = "[" + AnsiString(mcVSN_CycleWork      ) + "]" + (bKorean ? "비전작업"           : "CycleWork"      );  btMan4_5  -> Tag = (int)mcVSN_CycleWork       ; btMan4_5  -> Visible = true  ;
    btMan4_6  -> Caption = "[" + AnsiString(mcVSN_MoveN          ) + "]" + (bKorean ? "비전-피치"          : "MoveN"          );  btMan4_6  -> Tag = (int)mcVSN_MoveN           ; btMan4_6  -> Visible = true  ;
    btMan4_7  -> Caption = "[" + AnsiString(mcVSN_MoveP          ) + "]" + (bKorean ? "비전+피치"          : "MoveP"          );  btMan4_7  -> Tag = (int)mcVSN_MoveP           ; btMan4_7  -> Visible = true  ;
    btMan4_8  -> Caption = "[" + AnsiString(mcVSN_MoveTrg1       ) + "]" + (bKorean ? "1비전위치"          : "MoveTrg1"       );  btMan4_8  -> Tag = (int)mcVSN_MoveTrg1        ; btMan4_8  -> Visible = true  ;
    btMan4_9  -> Caption = "[" + AnsiString(mcVSN_MoveTrg2       ) + "]" + (bKorean ? "2비전위치"          : "MoveTrg2"       );  btMan4_9  -> Tag = (int)mcVSN_MoveTrg2        ; btMan4_9  -> Visible = true  ;

    sgDayInfo -> DoubleBuffered = true ;
    sgLotInfo -> DoubleBuffered = true ;

   //  OM.LoadDevOptn(OM.GetCrntDev());


    //SEQ.Reset();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::FormShow(TObject *Sender)                         // 오퍼레이션 화면을 한 번 보여줄 때마다, 이 함수를 탐
{
    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;
}

//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPasswordCloseClick(TObject *Sender)
{
    plPassWord -> Visible = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

    int iLevel;
    static int iLev = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }

    /*
    switch (iLevel) {
        case lvOperator : if( iLev != lvOperator) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iLev = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iLev != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(OM.MstOptn.bDataAccEng);
                              }
                              iLev = lvEngineer ;
                          }
                          break ;
        case lvMaster   : if( iLev != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  if(i != (EN_ARAY_ID)riLDR && i != (EN_ARAY_ID)riULD) DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iLev = lvMaster ;
                          }
                          break ;
    }
    */

    //Manual Enabled
    pcManual -> Enabled = !SEQ._bRun ;

    btLotEnd  -> Enabled = LOT.GetLotOpen() ;
    btStart   -> Enabled = LOT.GetLotOpen() ;
    btLotOpen -> Enabled =!LOT.GetLotOpen() ;


    SPC.LOT.DispLotInfo(sgLotInfo);
    SPC.DAY.DispDayInfo(sgDayInfo);

    //EM_Disp
    AnsiString Str;
    static int iPreErrCnt  ;
    int        iCrntErrCnt ;
    for (int i = 0 ; i < EM_GetMaxErr() ; i++) {
        if (EM_GetErr(i)) iCrntErrCnt++;
    }
    if (iPreErrCnt != iCrntErrCnt ) {     
        lbErr -> Clear();
        AnsiString Str;
        lbErr -> Items -> Clear();
        int iErrNo = EM_GetLastErr();
        for (int i = 0 ; i < EM_GetMaxErr() ; i++) {
            if (EM_GetErr(i)) {
                Str.sprintf("[ERR%03d]" , i) ;
                Str += EM_GetErrName(i) + AnsiString(" ") + EM_GetErrMsg(i);
                lbErr -> Items -> Add(Str);
                if (iErrNo == i) lbErr -> ItemIndex = lbErr -> Count - 1 ;
            }
        }
    }
    iPreErrCnt = iCrntErrCnt ;



    pnIgnrDoor     -> Color  =  OM.CmnOptn.bIgnrDoor    ? clLime : clRed ;
    pnLoadingStop  -> Color  =  OM.CmnOptn.bLoadingStop ? clLime : clRed ;
    pnSkipOST      -> Color  =  OM.CmnOptn.bSkipOST     ? clLime : clRed ;
    pnSkipVT       -> Color  =  OM.CmnOptn.bSkipVT      ? clLime : clRed ;
    pnSkipVSN1     -> Color  =  OM.CmnOptn.bSkipVisn1   ? clLime : clRed ;
    pnSkipVSN2     -> Color  =  OM.CmnOptn.bSkipVisn2   ? clLime : clRed ;


    pnIgnrDoor     -> Caption  =  OM.CmnOptn.bIgnrDoor    ? "ON" : "OFF" ;
    pnLoadingStop  -> Caption  =  OM.CmnOptn.bLoadingStop ? "ON" : "OFF" ;
    pnSkipOST      -> Caption  =  OM.CmnOptn.bSkipOST     ? "ON" : "OFF" ;
    pnSkipVT       -> Caption  =  OM.CmnOptn.bSkipVT      ? "ON" : "OFF" ;
    pnSkipVSN1     -> Caption  =  OM.CmnOptn.bSkipVisn1   ? "ON" : "OFF" ;
    pnSkipVSN2     -> Caption  =  OM.CmnOptn.bSkipVisn2   ? "ON" : "OFF" ;


    AnsiString sBtnName ;
    TBitBtn *  pBtn     ;

    //메뉴얼 버튼 색깔 칠하기.
    for(int i = 1 ; i < 6/*페이지6개*/ ; i++) {
        for(int j = 1 ; j < 15 /*버튼14개*/; j++) {
            sBtnName = "btMan" + AnsiString(i) + "_" + AnsiString(j) ;
            pBtn = ((TBitBtn*)FindComponent(sBtnName)) ;                                                                                                
            if(pBtn == NULL)  continue ;

/*
    btMan3_6  -> Caption = "[" + AnsiString(mcOST_PshrDnUp       ) + "]" + (bKorean ? "OST푸셔"            : "OST_PshrDnUp"   );  btMan3_6  -> Tag = (int)mcOST_PshrDnUp        ; btMan3_6  -> Visible = true  ;
    btMan3_7  -> Caption = "[" + AnsiString(mcOST_PTolUpDn       ) + "]" + (bKorean ? "OST핀툴"            : "OST_PTolUpDn"   );  btMan3_7  -> Tag = (int)mcOST_PTolUpDn        ; btMan3_7  -> Visible = true  ;
    btMan3_9  -> Caption = "[" + AnsiString(mcVTI_PshrDnUp       ) + "]" + (bKorean ? "VT푸셔"             : "VTI_PshrDnUp"   );  btMan3_9  -> Tag = (int)mcVTI_PshrDnUp        ; btMan3_9  -> Visible = true  ;
    btMan3_10 -> Caption = "[" + AnsiString(mcVTI_PTolUpDn       ) + "]" + (bKorean ? "VT핀툴"             : "VTI_PTolUpDn"   );  btMan3_10 -> Tag = (int)mcVTI_PTolUpDn        ; btMan3_10 -> Visible = true  ;
*/
            if(pBtn -> Name =="btMan3_11") {
                pBtn -> Font -> Color = IO_GetY(yVTI_VTPower) ?  clLime : clBlack ;
            }
            else if(pBtn -> Name =="btMan3_6"){//mcOST_PshrDnUp   //턴테이블 회전시에 잘 안보여서 색으로 표현.
                pBtn -> Font -> Color = AT_Complete(aiOST_PshrDnUp , ccFwd) ? clRed : clBlue ;
            }
            else if(pBtn -> Name =="btMan3_7"){//mcOST_PTolUpDn
                pBtn -> Font -> Color = AT_Complete(aiOST_PTolUpDn , ccFwd) ? clRed : clBlue ;
            }
            else if(pBtn -> Name =="btMan3_9"){//mcVTI_PshrDnUp
                pBtn -> Font -> Color = AT_Complete(aiVTI_PshrDnUp , ccFwd) ? clRed : clBlue ;
            }
            else if(pBtn -> Name =="btMan3_10"){//mcVTI_PTolUpDn
                pBtn -> Font -> Color = AT_Complete(aiVTI_PTolUpDn , ccFwd) ? clRed : clBlue ;
            }
            else {
                if(pBtn -> Tag == MM.GetManNo()) pBtn -> Font -> Color = clLime  ;
                else                             pBtn -> Font -> Color = clBlack ;
            }
        }
    }

    //인제 필요 없다. ARAY에서 알아서 함.
    for(int i = 0 ; i < MAX_ARAY ; i++) {
        DM.ARAY[i].UpdateAray();
    }

    bool bAllArayNone = DM.ARAY[riSRT].CheckAllStat(csNone) && DM.ARAY[riZIG].CheckAllStat(csNone) && DM.ARAY[riSTF].CheckAllStat(csNone) &&
                        DM.ARAY[riSTL].CheckAllStat(csNone) && DM.ARAY[riSTB].CheckAllStat(csNone) && DM.ARAY[riSTR].CheckAllStat(csNone) &&
                        DM.ARAY[riRJ1].CheckAllStat(csNone) && DM.ARAY[riRJ2].CheckAllStat(csNone) && DM.ARAY[riRJ3].CheckAllStat(csNone) &&
                        DM.ARAY[riRJ4].CheckAllStat(csNone) && DM.ARAY[riPRL].CheckAllStat(csNone) && DM.ARAY[riLDR].CheckAllStat(csNone) &&
                        DM.ARAY[riPSL].CheckAllStat(csNone) && DM.ARAY[riPRU].CheckAllStat(csNone) && DM.ARAY[riULD].CheckAllStat(csNone) &&
                        DM.ARAY[riPSU].CheckAllStat(csNone) ;


    if(SRT.m_bLdrPickErrDisp){
        FrmVacErr -> PageControl1 -> ActivePageIndex = 0;
        FrmVacErr -> Show();
        SRT.m_bLdrPickErrDisp = false;
    }
    if(SRT.m_bZigPickErrDisp){
        FrmVacErr -> PageControl1 -> ActivePageIndex = 1;
        FrmVacErr -> Show();
        SRT.m_bZigPickErrDisp = false;
    }
    if(SRT.m_bStgPickErrDisp){
        FrmVacErr -> PageControl1 -> ActivePageIndex = 2;
        FrmVacErr -> Show();
        SRT.m_bStgPickErrDisp = false;
    }
    if(LSP.m_bNeedTrayErrDisp){
        FrmVacErr -> PageControl1 -> ActivePageIndex = 3;
        FrmVacErr -> Show();
        LSP.m_bNeedTrayErrDisp = false;

    }

    //static EN_STG_ANGLE iPreAngle = STG.m_iStgAngle ;
    //if(iPreAngle != STG.m_iStgAngle) {
    //    iPreAngle = STG.m_iStgAngle ;
    //
    //    ChangeArayParent(iPreAngle);
    //
    //}

    //도어오픈 엔지니어도 안되고 마스터에서만 되게 2014.06.27 이윤승 팀장 요청.
    pnDoor -> Visible = FM_GetLevel() == lvMaster ;
        //Door Sensor.
    bool isAllCloseDoor = IO_GetX(xETC_DoorFt) &&
                          IO_GetX(xETC_DoorLt) &&
                          IO_GetX(xETC_DoorRt) &&
                          IO_GetX(xETC_DoorRr) ;
    if(FM_GetLevel()!=lvOperator && isAllCloseDoor && SEQ._bRun){
        FM_SetLevel(lvOperator);
    }

    //pnManRsltOS -> Visible = !SEQ._bRun ;
    //pnManRsltVT -> Visible = !SEQ._bRun ;
    //pnManRsltVS -> Visible = !SEQ._bRun ;



    double dStgPos = MT_GetCmdPos(miSTG_TTble);
    double dAngle  = (dStgPos - PM.GetValue(miSTG_TTble , pvSTG_TTbleWork));
    Label19 -> Caption = dAngle ;








    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotOpenClick(TObject *Sender)
{
    FrmLotOpen -> ShowModal() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStartClick(TObject *Sender)
{
    SEQ._bBtnStart = true;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btStopClick(TObject *Sender)
{
    SEQ._bBtnStop = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btResetClick(TObject *Sender)
{
    SEQ._bBtnReset = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btLotEndClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Do you really want to LotEnd?") != mrYes) return ;

    /*
    bool LDRMgzDetect ;
    bool ULDMgzDetect ;

    LDRMgzDetect = IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2);
    ULDMgzDetect = IO_GetX(xULD_MgzDetect1) || IO_GetX(xULD_MgzDetect2);

    if(LDRMgzDetect && ULDMgzDetect) MM.SetManCycle(mcETC_LDRULDPlace);
    else if( LDRMgzDetect && !ULDMgzDetect) MM.SetManCycle(mcLDR_Place);
    else if(!LDRMgzDetect &&  ULDMgzDetect) MM.SetManCycle(mcULD_Place);
    */

    //kyekye 얼로더 메거진 체인지 더 일찍 하기 랏바뀌었을때.


    LOT.LotEnd ();

    DM.ARAY[riPRL].SetStat(csNone);
    DM.ARAY[riLDR].SetStat(csNone);
    DM.ARAY[riPSL].SetStat(csNone);

    DM.ARAY[riSRT].SetStat(csEmpty);
    DM.ARAY[riZIG].SetStat(csEmpty);

    DM.ARAY[riSTF].SetStat(csEmpty);
    DM.ARAY[riSTL].SetStat(csEmpty);
    DM.ARAY[riSTB].SetStat(csEmpty);
    DM.ARAY[riSTR].SetStat(csEmpty);

    //DM.ARAY[riRJ1].SetStat(csEmpty);
    //DM.ARAY[riRJ2].SetStat(csEmpty);
    //DM.ARAY[riRJ3].SetStat(csEmpty);
    //DM.ARAY[riRJ4].SetStat(csEmpty);

    DM.ARAY[riPRU].SetStat(csNone);
    DM.ARAY[riULD].SetStat(csNone);
    DM.ARAY[riPSU].SetStat(csNone);


}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnIgnoreTrimDblClick(TObject *Sender)
{
    TPanel * Panel  = (TPanel *)Sender ;
    int      iIndex =  Panel -> Tag ;

    if(FM_GetLevel() == lvOperator) return ;

    if (FM_MsgYesNo("Confirm", "Would you like to invert the Option?") != mrYes) return ;

    switch(iIndex) {
        case 0 :                                                           break ;
        case 1 : OM.CmnOptn.bIgnrDoor      = !OM.CmnOptn.bIgnrDoor       ; break ;
        case 2 : OM.CmnOptn.bLoadingStop   = !OM.CmnOptn.bLoadingStop    ; break ;
        case 3 : OM.CmnOptn.bSkipOST       = !OM.CmnOptn.bSkipOST        ; break ;
        case 4 : OM.CmnOptn.bSkipVT        = !OM.CmnOptn.bSkipVT         ; break ;
        case 5 : OM.CmnOptn.bSkipVisn1     = !OM.CmnOptn.bSkipVisn1      ; break ;
        case 6 : OM.CmnOptn.bSkipVisn2     = !OM.CmnOptn.bSkipVisn2      ; break ;


    }

    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btMan1_1Click(TObject *Sender)
{
    int iManNo = ((TBitBtn *)Sender) -> Tag ;
    MM.SetManCycle(iManNo);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperation::btPR1LotInputClick(TObject *Sender)
{
    /*
    if(SEQ._bRun) return ;


    int iTag = ((TButton*)Sender) -> Tag ;
    AnsiString sLotNo ;
    int iMgzNo , iSlotNo ;

    switch (iTag)
    {
        default :
            break ;

        case 1 :
            sLotNo  = edPRBLotNo ->Text ;
            iMgzNo  = edPRBMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edPRBSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riPRB].SetLotNo(sLotNo) ;            =
            DM.ARAY[riPRB].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 2 :
            sLotNo  = edInspLotNo ->Text ;
            iMgzNo  = edInspMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edInspSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riWRK].SetLotNo(sLotNo) ;
            DM.ARAY[riWRK].SetID(iMgzNo *100 + iSlotNo);
            break ;

        case 3 :
            sLotNo  = edPSBLotNo ->Text ;
            iMgzNo  = edPSBMgzNo ->Text.ToIntDef(1)  ;
            iSlotNo = edPSBSlotNo->Text.ToIntDef(99) ;
            DM.ARAY[riPSB].SetLotNo(sLotNo) ;
            DM.ARAY[riPSB].SetID(iMgzNo *100 + iSlotNo);
            break ;
     }

     */

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::btPR1InputClick(TObject *Sender)
{
//    int iTag = ((TButton*)Sender) -> Tag ;
//
//    switch (iTag)
//    {
//        default :
//            break ;
//
//        case 1 : DM.ARAY[riPR1].SetStat(csUnkwn);
//                 if(DM.ARAY[riLDR].CheckAllStat(csNone) || DM.ARAY[riLDR].CheckAllStat(csUnkwn)) DM.ARAY[riPR1].SetLotNo(PR1.GetWorkedLotNo()     ) ;
//                 else                                                                            DM.ARAY[riPR1].SetLotNo(DM.ARAY[riLDR].GetLotNo()) ;
//                 break;
//
//        case 2 : DM.ARAY[riPR1].SetStat(csUnkwn);
//                 if(DM.ARAY[riLDR].CheckAllStat(csNone) || DM.ARAY[riLDR].CheckAllStat(csUnkwn)) DM.ARAY[riPR1].SetLotNo(PR1.GetWorkedLotNo()     ) ;
//                 else                                                                            DM.ARAY[riPR1].SetLotNo(DM.ARAY[riLDR].GetLotNo()) ;
//                 break;
//    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperation::pnDayRunTimeDblClick(TObject *Sender)
{
    if(FM_GetLevel() != lvMaster) return ;

    if(FM_MsgYesNo("Confirm","Clear Day Info") != mrYes) return ;

    SPC.DAY.ClearData() ;

}
//---------------------------------------------------------------------------









void __fastcall TFrmOperation::btPrbClearClick(TObject *Sender)
{

    int iTag = ((TButton*)Sender) -> Tag ;

    DM.ARAY[iTag].ClearMap();

/*
    iTag += 1;

    AnsiString sLotNo , sSlotNo ,sMsg;
    switch (iTag)
    {
        default :
            break ;

        case 1 : DM.ARAY[riPRB].SetStat(csNone);
                 break;

        case 2 : DM.ARAY[riLSR].SetStat(csNone);
                 break;

        case 3 : DM.ARAY[riVSN].SetStat(csNone);
                 break ;

        case 4 : DM.ARAY[riPSB].SetStat(csNone);
                 break;

        case 5 : DM.ARAY[riPRB].SetStat(csMask);
                 break;

        case 6 : DM.ARAY[riLSR].SetStat(csMask);
                 break;

        case 7 : DM.ARAY[riVSN].SetStat(csWork);
                 break ;

        case 8 : DM.ARAY[riPSB].SetStat(csWork);
                 break;
    }
*/
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperation::btPrbInputClick(TObject *Sender)
{
/*
    if(SEQ._bRun) return ;

    int iTag = ((TButton*)Sender) -> Tag ;

    if(DM.ARAY[riLDR].GetCntStat(csEmpty)==0) {
        FM_MsgOk("ERROR","로더에 메거진이 없어 랏넘버를 알수 없습니다.");
        return ;
    }



    AnsiString sTemp = "스트립 랏넘버가 " + DM.ARAY[riLDR].GetLotNo() + "가 맞습니까?";

    if(FM_MsgYesNo("Check",sTemp.c_str() ) != mrYes) {
        return ;
    }

    DM.ARAY[iTag].SetStat(csUnkwn);
    int iSltCnt = OM.DevInfo.iCsSlCnt - DM.ARAY[riLDR].FindFrstRow(csEmpty)-1;
    DM.ARAY[iTag].SetID((DM.ARAY[riLDR].GetID().ToIntDef(0) / 100 * 100) + iSltCnt);
    DM.ARAY[iTag].SetLotNo(DM.ARAY[riLDR].GetLotNo());
    */
}
//---------------------------------------------------------------------------



void __fastcall TFrmOperation::btAllHomeClick(TObject *Sender)
{
    if (FM_MsgYesNo("Confirm" , "전체 홈을 잡으시겠습니까?") != mrYes)  return;
    MM.SetManCycle(mcAllHome);
}
//---------------------------------------------------------------------------



void __fastcall TFrmOperation::FormCreate(TObject *Sender)
{
    //STG.SetChangeArayParentFun(ChangeArayParent);
    //STG.ChangeSTGArayByAngle(STG.m_iStgAngle);
    //ChangeArayParent(STG.m_iStgAngle);
}
//---------------------------------------------------------------------------


/*
void TFrmOperation::ChangeArayParent(EN_STG_ANGLE _iAngle)
{
    //스테이지 돌아가면 그것에 따라 판넬에 페어런츠를 바꿔서 실제 위치로 디스플레이해줌.
    if(_iAngle == sa0) {
        DM.ARAY[riSG1].SetParent(pnSGFrnt );
        DM.ARAY[riSG2].SetParent(pnSGLeft );
        DM.ARAY[riSG3].SetParent(pnSGRear );
        DM.ARAY[riSG4].SetParent(pnSGRght );
    }
    else if(_iAngle == sa90) {
        DM.ARAY[riSG1].SetParent(pnSGLeft);
        DM.ARAY[riSG2].SetParent(pnSGRear);
        DM.ARAY[riSG3].SetParent(pnSGRght);
        DM.ARAY[riSG4].SetParent(pnSGFrnt);
    }
    else if(_iAngle == sa180) {
        DM.ARAY[riSG1].SetParent(pnSGRear);
        DM.ARAY[riSG2].SetParent(pnSGRght);
        DM.ARAY[riSG3].SetParent(pnSGFrnt);
        DM.ARAY[riSG4].SetParent(pnSGLeft);
    }
    else if(_iAngle == sa270) {
        DM.ARAY[riSG1].SetParent(pnSGRght);
        DM.ARAY[riSG2].SetParent(pnSGFrnt);
        DM.ARAY[riSG3].SetParent(pnSGLeft);
        DM.ARAY[riSG4].SetParent(pnSGRear);
    }
}
*/



void __fastcall TFrmOperation::btClearClick(TObject *Sender)
{
    int iSel = ((TButton *)Sender) -> Tag ;

    String sTemp = "리젝트레이를 클리어하시겠습니까?";

    if(FM_MsgYesNo("Comfirn",sTemp.c_str()) !=mrYes) return ;

    DM.ARAY[(EN_ARAY_ID)iSel].SetStat(csEmpty); 
}
//---------------------------------------------------------------------------

#include "UserFile.h"

