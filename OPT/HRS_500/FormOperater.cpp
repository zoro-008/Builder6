//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormOperater.h"

#include "SMInterfaceUnit.h"
#include "SMDllDefine.h"
#include "DataMan.h"
#include "UtilDefine.h"
#include "Sequence.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "ManualMan.h"
#include "Rs232Man.h"
#include "SLogUnit.h"
#include "SpcSubUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
//#include "Loader.h"
//#include "Rail.h"
//#include "UnLoader.h"
#include "Picker.h"
//#include "Sorter.h"
#include "Supply.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmOperater *FrmOperater;
//---------------------------------------------------------------------------
__fastcall TFrmOperater::TFrmOperater(TComponent* Owner)
    : TForm(Owner)
{
    Height = 919  ;
    Width  = 1024 ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperater::FormShow(TObject *Sender)
{
    tmUpdate->Enabled = true;

    seBn9Sel -> Value = OM.DevOptn.iBn9Sel ;
    seBn8Sel -> Value = OM.DevOptn.iBn8Sel ;
    seBn7Sel -> Value = OM.DevOptn.iBn7Sel ;
    seBn6Sel -> Value = OM.DevOptn.iBn6Sel ;

    //pnLBLim0 -> Color = clStat[csRslt0            ] ;  pnRBLim0 -> Color = clStat[csRslt0            ] ;
    //pnLBLim1 -> Color = clStat[csRslt1            ] ;  pnRBLim1 -> Color = clStat[csRslt1            ] ;
    //pnLBLim2 -> Color = clStat[csRslt2            ] ;  pnRBLim2 -> Color = clStat[csRslt2            ] ;
    //pnLBLim3 -> Color = clStat[csRslt3            ] ;  pnRBLim3 -> Color = clStat[csRslt3            ] ;
    //pnLBLim4 -> Color = clStat[csRslt4            ] ;  pnRBLim4 -> Color = clStat[csRslt4            ] ;
    //pnLBLim5 -> Color = clStat[csRslt5            ] ;  pnRBLim5 -> Color = clStat[csRslt5            ] ;
    //pnLBLim6 -> Color = clStat[OM.DevOptn.iBn6Sel ] ;  pnRBLim6 -> Color = clStat[OM.DevOptn.iBn6Sel ] ;
    //pnLBLim7 -> Color = clStat[OM.DevOptn.iBn7Sel ] ;  pnRBLim7 -> Color = clStat[OM.DevOptn.iBn7Sel ] ;
    //pnLBLim8 -> Color = clStat[OM.DevOptn.iBn8Sel ] ;  pnRBLim8 -> Color = clStat[OM.DevOptn.iBn8Sel ] ;
    //pnLBLim9 -> Color = clStat[OM.DevOptn.iBn9Sel ] ;  pnRBLim9 -> Color = clStat[OM.DevOptn.iBn9Sel ] ;



}
//---------------------------------------------------------------------------

void __fastcall TFrmOperater::btOperatorClick(TObject *Sender)
{
    plPassWord->Visible = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperater::btPasswordCloseClick(TObject *Sender)
{
    plPassWord -> Visible = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperater::btOperClick(TObject *Sender)
{
    FM_PswdShow((EN_LEVEL)((TBitBtn *)Sender)->Tag);
    plPassWord->Visible = false;
    //FM_Update();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperater::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperater::tmUpdateTimer(TObject *Sender)
{
    tmUpdate->Enabled = false;

    int iLevel;
    static int iPreLevel = -1;

    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator : btOperator -> Caption = "OPERATOR"; break ;
        case lvEngineer : btOperator -> Caption = "ENGINEER"; break ;
        case lvMaster   : btOperator -> Caption = " MASTER "; break ;
        default         : btOperator -> Caption = " ERROR  "; break ;
    }

    switch (iLevel) {
        case lvOperator : if( iPreLevel != lvOperator) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iPreLevel = lvOperator ;
                          }
                          break ;
        case lvEngineer : if( iPreLevel != lvEngineer) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(false);
                              }
                              iPreLevel = lvEngineer ;
                          }
                          break ;
        case lvMaster   : if( iPreLevel != lvMaster) {
                              for(int i =0 ; i < MAX_ARAY ; i++ ) {
                                  DM.ARAY[i].PopupMenuEnable(true);
                              }
                              iPreLevel = lvMaster ;
                          }
                          break ;
    }


    String sTemp ;
    sTemp = "FAIL"                                                          ; pnLBLim0 -> Caption = sTemp ; pnLBCnt0 -> Caption = SPL.GetPkgCnt(0);
    sTemp = (String)OM.DevOptn.dMinBn1 + " ~ " + (String)OM.DevOptn.dMaxBn1 ; pnLBLim1 -> Caption = sTemp ; pnLBCnt1 -> Caption = SPL.GetPkgCnt(1);
    sTemp = (String)OM.DevOptn.dMinBn2 + " ~ " + (String)OM.DevOptn.dMaxBn2 ; pnLBLim2 -> Caption = sTemp ; pnLBCnt2 -> Caption = SPL.GetPkgCnt(2);
    sTemp = (String)OM.DevOptn.dMinBn3 + " ~ " + (String)OM.DevOptn.dMaxBn3 ; pnLBLim3 -> Caption = sTemp ; pnLBCnt3 -> Caption = SPL.GetPkgCnt(3);
    sTemp = (String)OM.DevOptn.dMinBn4 + " ~ " + (String)OM.DevOptn.dMaxBn4 ; pnLBLim4 -> Caption = sTemp ; pnLBCnt4 -> Caption = SPL.GetPkgCnt(4);
    sTemp = (String)OM.DevOptn.dMinBn5 + " ~ " + (String)OM.DevOptn.dMaxBn5 ; pnLBLim5 -> Caption = sTemp ; pnLBCnt5 -> Caption = SPL.GetPkgCnt(5);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn6Sel                          ; pnLBLim6 -> Caption = sTemp ; pnLBCnt6 -> Caption = SPL.GetPkgCnt(6);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn7Sel                          ; pnLBLim7 -> Caption = sTemp ; pnLBCnt7 -> Caption = SPL.GetPkgCnt(7);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn8Sel                          ; pnLBLim8 -> Caption = sTemp ; pnLBCnt8 -> Caption = SPL.GetPkgCnt(8);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn9Sel                          ; pnLBLim9 -> Caption = sTemp ; pnLBCnt9 -> Caption = SPL.GetPkgCnt(9);

    sTemp = "FAIL"                                                          ; pnRBLim0 -> Caption = sTemp ; pnRBCnt0 -> Caption = SPR.GetPkgCnt(0);
    sTemp = (String)OM.DevOptn.dMinBn1 + " ~ " + (String)OM.DevOptn.dMaxBn1 ; pnRBLim1 -> Caption = sTemp ; pnRBCnt1 -> Caption = SPR.GetPkgCnt(1);
    sTemp = (String)OM.DevOptn.dMinBn2 + " ~ " + (String)OM.DevOptn.dMaxBn2 ; pnRBLim2 -> Caption = sTemp ; pnRBCnt2 -> Caption = SPR.GetPkgCnt(2);
    sTemp = (String)OM.DevOptn.dMinBn3 + " ~ " + (String)OM.DevOptn.dMaxBn3 ; pnRBLim3 -> Caption = sTemp ; pnRBCnt3 -> Caption = SPR.GetPkgCnt(3);
    sTemp = (String)OM.DevOptn.dMinBn4 + " ~ " + (String)OM.DevOptn.dMaxBn4 ; pnRBLim4 -> Caption = sTemp ; pnRBCnt4 -> Caption = SPR.GetPkgCnt(4);
    sTemp = (String)OM.DevOptn.dMinBn5 + " ~ " + (String)OM.DevOptn.dMaxBn5 ; pnRBLim5 -> Caption = sTemp ; pnRBCnt5 -> Caption = SPR.GetPkgCnt(5);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn6Sel                          ; pnRBLim6 -> Caption = sTemp ; pnRBCnt6 -> Caption = SPR.GetPkgCnt(6);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn7Sel                          ; pnRBLim7 -> Caption = sTemp ; pnRBCnt7 -> Caption = SPR.GetPkgCnt(7);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn8Sel                          ; pnRBLim8 -> Caption = sTemp ; pnRBCnt8 -> Caption = SPR.GetPkgCnt(8);
    sTemp = "BUFFER " + (String)OM.DevOptn.iBn9Sel                          ; pnRBLim9 -> Caption = sTemp ; pnRBCnt9 -> Caption = SPR.GetPkgCnt(9);


    static bool bPreRun = SEQ._bRun ;
    bPreRun = SEQ._bRun ;


    tmUpdate->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperater::btLbRo0Click(TObject *Sender)
{

    if(SPL.GetSeqStep()) {
//        FM_MsgOk("Confirm","Sorting Left Is Running");
        return ;
    }
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Remove Left Bin " + (String)(iTag) + "?" ;
//    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SPL.SetBin(iTag,false);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperater::btLbRa0Click(TObject *Sender)
{
    if(SPL.GetSeqStep()){
//        FM_MsgOk("Confirm","Sorting Left Is Running");
        return ;
    }
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Remove Left Bin " + (String)(iTag) + " All ?" ;
    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SPL.SetBin(iTag,true);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperater::btRbRo0Click(TObject *Sender)
{
    if(SPR.GetSeqStep()) {
//        FM_MsgOk("Confirm","Sorting Right Is Running");
        return ;
    }
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Remove Right Bin " + (String)(iTag) + "?" ;
    //if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SPR.SetBin(iTag,false);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperater::btRbRa0Click(TObject *Sender)
{
    if(SPR.GetSeqStep()){
        FM_MsgOk("Confirm","Sorting Right Is Running");
        return ;
    }
    int iTag = ((TButton*)Sender)->Tag ;
    String sTemp = "Remove Right Bin " + (String)(iTag) + "All ?" ;
    if(FM_MsgYesNo("Confirm",sTemp) != mrYes ) return ;

    SPR.SetBin(iTag,true);
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperater::pnLBCnt7Click(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Bin Count Clear ?") != mrYes ) return ;
    TPanel *pPn = dynamic_cast <TPanel *> (Sender);
    String  sName= pPn->Name;

    //ºó¹øÈ£ ¼ø¼­ Left:9,8,7,6,5,4,3,2,1,0   Right:10,11,12,13,14,15,16,17,18,19
         if(sName == "pnLBCnt9") { SPL.m_iPkgCnt[9] = 0 ; Trace("Button Click","Clear Left  Supply Bin 9 Cnt "); }
    else if(sName == "pnLBCnt8") { SPL.m_iPkgCnt[8] = 0 ; Trace("Button Click","Clear Left  Supply Bin 8 Cnt "); }
    else if(sName == "pnLBCnt7") { SPL.m_iPkgCnt[7] = 0 ; Trace("Button Click","Clear Left  Supply Bin 7 Cnt "); }
    else if(sName == "pnLBCnt6") { SPL.m_iPkgCnt[6] = 0 ; Trace("Button Click","Clear Left  Supply Bin 6 Cnt "); }
    else if(sName == "pnLBCnt5") { SPL.m_iPkgCnt[5] = 0 ; Trace("Button Click","Clear Left  Supply Bin 5 Cnt "); }
    else if(sName == "pnLBCnt4") { SPL.m_iPkgCnt[4] = 0 ; Trace("Button Click","Clear Left  Supply Bin 4 Cnt "); }
    else if(sName == "pnLBCnt3") { SPL.m_iPkgCnt[3] = 0 ; Trace("Button Click","Clear Left  Supply Bin 3 Cnt "); }
    else if(sName == "pnLBCnt2") { SPL.m_iPkgCnt[2] = 0 ; Trace("Button Click","Clear Left  Supply Bin 2 Cnt "); }
    else if(sName == "pnLBCnt1") { SPL.m_iPkgCnt[1] = 0 ; Trace("Button Click","Clear Left  Supply Bin 1 Cnt "); }
    else if(sName == "pnLBCnt0") { SPL.m_iPkgCnt[0] = 0 ; Trace("Button Click","Clear Left  Supply Bin 0 Cnt "); }

    else if(sName == "pnRBCnt0") { SPR.m_iPkgCnt[0] = 0 ; Trace("Button Click","Clear Right Supply Bin 0 Cnt "); }
    else if(sName == "pnRBCnt1") { SPR.m_iPkgCnt[1] = 0 ; Trace("Button Click","Clear Right Supply Bin 1 Cnt "); }
    else if(sName == "pnRBCnt2") { SPR.m_iPkgCnt[2] = 0 ; Trace("Button Click","Clear Right Supply Bin 2 Cnt "); }
    else if(sName == "pnRBCnt3") { SPR.m_iPkgCnt[3] = 0 ; Trace("Button Click","Clear Right Supply Bin 3 Cnt "); }
    else if(sName == "pnRBCnt4") { SPR.m_iPkgCnt[4] = 0 ; Trace("Button Click","Clear Right Supply Bin 4 Cnt "); }
    else if(sName == "pnRBCnt5") { SPR.m_iPkgCnt[5] = 0 ; Trace("Button Click","Clear Right Supply Bin 5 Cnt "); }
    else if(sName == "pnRBCnt6") { SPR.m_iPkgCnt[6] = 0 ; Trace("Button Click","Clear Right Supply Bin 6 Cnt "); }
    else if(sName == "pnRBCnt7") { SPR.m_iPkgCnt[7] = 0 ; Trace("Button Click","Clear Right Supply Bin 7 Cnt "); }
    else if(sName == "pnRBCnt8") { SPR.m_iPkgCnt[8] = 0 ; Trace("Button Click","Clear Right Supply Bin 8 Cnt "); }
    else if(sName == "pnRBCnt9") { SPR.m_iPkgCnt[9] = 0 ; Trace("Button Click","Clear Right Supply Bin 9 Cnt "); }






}
//---------------------------------------------------------------------------

void __fastcall TFrmOperater::FormDestroy(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperater::btCountAllClearClick(TObject *Sender)
{
    if(FM_MsgYesNo("Confirm","Bin Count Clear ?") != mrYes ) return ;


    SPL.m_iPkgCnt[9] = 0 ;
    SPL.m_iPkgCnt[8] = 0 ;
    SPL.m_iPkgCnt[7] = 0 ;
    SPL.m_iPkgCnt[6] = 0 ;
    SPL.m_iPkgCnt[5] = 0 ;
    SPL.m_iPkgCnt[4] = 0 ;
    SPL.m_iPkgCnt[3] = 0 ;
    SPL.m_iPkgCnt[2] = 0 ;
    SPL.m_iPkgCnt[1] = 0 ;
    SPL.m_iPkgCnt[0] = 0 ;

    SPR.m_iPkgCnt[0] = 0 ;
    SPR.m_iPkgCnt[1] = 0 ;
    SPR.m_iPkgCnt[2] = 0 ;
    SPR.m_iPkgCnt[3] = 0 ;
    SPR.m_iPkgCnt[4] = 0 ;
    SPR.m_iPkgCnt[5] = 0 ;
    SPR.m_iPkgCnt[6] = 0 ;
    SPR.m_iPkgCnt[7] = 0 ;
    SPR.m_iPkgCnt[8] = 0 ;
    SPR.m_iPkgCnt[9] = 0 ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmOperater::btSupplyAllClick(TObject *Sender)
{
    SPR.m_bSetSupplyAll = true ;
    SPL.m_bSetSupplyAll = true ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmOperater::btSetBinClick(TObject *Sender)
{
    OM.DevOptn.iBn6Sel = seBn6Sel -> Value ;
    OM.DevOptn.iBn7Sel = seBn7Sel -> Value ;
    OM.DevOptn.iBn8Sel = seBn8Sel -> Value ;
    OM.DevOptn.iBn9Sel = seBn9Sel -> Value ;

    OM.SaveDevOptn(OM.m_sCrntDev) ;

    pnLBLim0 -> Color = clStat[csRslt0            ] ;  pnRBLim0 -> Color = clStat[csRslt0            ] ;
    pnLBLim1 -> Color = clStat[csRslt1            ] ;  pnRBLim1 -> Color = clStat[csRslt1            ] ;
    pnLBLim2 -> Color = clStat[csRslt2            ] ;  pnRBLim2 -> Color = clStat[csRslt2            ] ;
    pnLBLim3 -> Color = clStat[csRslt3            ] ;  pnRBLim3 -> Color = clStat[csRslt3            ] ;
    pnLBLim4 -> Color = clStat[csRslt4            ] ;  pnRBLim4 -> Color = clStat[csRslt4            ] ;
    pnLBLim5 -> Color = clStat[csRslt5            ] ;  pnRBLim5 -> Color = clStat[csRslt5            ] ;
    pnLBLim6 -> Color = clStat[OM.DevOptn.iBn6Sel ] ;  pnRBLim6 -> Color = clStat[OM.DevOptn.iBn6Sel ] ;
    pnLBLim7 -> Color = clStat[OM.DevOptn.iBn7Sel ] ;  pnRBLim7 -> Color = clStat[OM.DevOptn.iBn7Sel ] ;
    pnLBLim8 -> Color = clStat[OM.DevOptn.iBn8Sel ] ;  pnRBLim8 -> Color = clStat[OM.DevOptn.iBn8Sel ] ;
    pnLBLim9 -> Color = clStat[OM.DevOptn.iBn9Sel ] ;  pnRBLim9 -> Color = clStat[OM.DevOptn.iBn9Sel ] ;
}
//---------------------------------------------------------------------------


