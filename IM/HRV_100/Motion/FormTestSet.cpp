//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormTestSet.h"

#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "UtilDefine.h"
//#include "SPCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmTestSet *FrmTestSet;
//---------------------------------------------------------------------------
__fastcall TFrmTestSet::TFrmTestSet(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmTestSet::btSaveClick(TObject *Sender)
{
    //Check Running Status.
    if (SEQ._bRun) {
        FM_MsgOk("Warning" , "Can't Save during Autorunning!");
        return;
    }
    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    UpdateComOptn(toBuff);
    OM.SaveCmnOptn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmTestSet::UpdateComOptn(bool _bToTable)
{                                                                                   
/*
    if (_bToTable == toTabl) {
        cbLoadingStop  -> Checked   = OM.CmnOptn.bLoadingStop    ;
        cbIgnrDoor     -> Checked   = OM.CmnOptn.bIgnrDoor       ;
        cbSkipOST      -> Checked   = OM.CmnOptn.bSkipOST        ;
        cbSkipVT       -> Checked   = OM.CmnOptn.bSkipVT         ;
        cbSkipVSN1     -> Checked   = OM.CmnOptn.bSkipVisn1      ;
        cbSkipVSN2     -> Checked   = OM.CmnOptn.bSkipVisn2      ;

        cbSkip1stTol   -> Checked   = OM.CmnOptn.bSkip1stTol     ;
        cbSkip2ndTol   -> Checked   = OM.CmnOptn.bSkip2ndTol     ;
        cbSkip3thTol   -> Checked   = OM.CmnOptn.bSkip3thTol     ;
        cbSkip4thTol   -> Checked   = OM.CmnOptn.bSkip4thTol     ;

        cbFailIBLK     -> ItemIndex = OM.CmnOptn.iVTBLKBin       ;
        cbFailIDFT     -> ItemIndex = OM.CmnOptn.iVTDFTBin       ;
        cbFailIDIM     -> ItemIndex = OM.CmnOptn.iVTDIMBin       ;
        cbFailIOST     -> ItemIndex = OM.CmnOptn.iOSTFailBin     ;
        cbFailIVH      -> ItemIndex = OM.CmnOptn.iVTVHBin        ;
        cbFailIVSN     -> ItemIndex = OM.CmnOptn.iVSNFailBin     ;

        edBfrVTPowrDelay -> Text    = OM.CmnOptn.iBfrVTPowrDelay ;
        edBfrVTInspDelay -> Text    = OM.CmnOptn.iBfrVTInspDelay ;
        edVTContBlkCnt   -> Text    = OM.CmnOptn.iVTContBlkCnt   ;
    }
    else {
        OM.CmnOptn.bLoadingStop     =  cbLoadingStop  -> Checked                  ;
        OM.CmnOptn.bIgnrDoor        =  cbIgnrDoor     -> Checked                  ;
        OM.CmnOptn.bSkipOST         =  cbSkipOST      -> Checked                  ;
        OM.CmnOptn.bSkipVT          =  cbSkipVT       -> Checked                  ;
        OM.CmnOptn.bSkipVisn1       =  cbSkipVSN1     -> Checked                  ;
        OM.CmnOptn.bSkipVisn2       =  cbSkipVSN2     -> Checked                  ;

        OM.CmnOptn.bSkip1stTol      =  cbSkip1stTol   -> Checked                  ;
        OM.CmnOptn.bSkip2ndTol      =  cbSkip2ndTol   -> Checked                  ;
        OM.CmnOptn.bSkip3thTol      =  cbSkip3thTol   -> Checked                  ;
        OM.CmnOptn.bSkip4thTol      =  cbSkip4thTol   -> Checked                  ;

        OM.CmnOptn.iVTBLKBin        =  cbFailIBLK     -> ItemIndex                ;
        OM.CmnOptn.iVTDFTBin        =  cbFailIDFT     -> ItemIndex                ;
        OM.CmnOptn.iVTDIMBin        =  cbFailIDIM     -> ItemIndex                ;
        OM.CmnOptn.iOSTFailBin      =  cbFailIOST     -> ItemIndex                ;
        OM.CmnOptn.iVTVHBin         =  cbFailIVH      -> ItemIndex                ;
        OM.CmnOptn.iVSNFailBin      =  cbFailIVSN     -> ItemIndex                ;

        OM.CmnOptn.iBfrVTPowrDelay  = StrToIntDef(edBfrVTPowrDelay -> Text ,1000) ;
        OM.CmnOptn.iBfrVTInspDelay  = StrToIntDef(edBfrVTInspDelay -> Text ,1000) ;
        OM.CmnOptn.iVTContBlkCnt    = StrToIntDef(edVTContBlkCnt   -> Text ,10  ) ;
    }
*/
}
//---------------------------------------------------------------------------
void __fastcall TFrmTestSet::FormShow(TObject *Sender)
{

//    cbIgnrDoor -> Visible = FM_GetLevel() == lvMaster ;
//    UpdateComOptn(toTabl);
}
//---------------------------------------------------------------------------




