//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"

#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "FormOperation.h"
#include "SAInterface.h"
#include "MapUnit.h"

#include "Sequence.h"
#include "Head.h"
//---------------------------------------------------------------------------
#include "Rs232Man.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "LabelS"
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{
    //작명 방법 기술.
    //aiLDR_Stopper -> pnLDR_Stopper1 (1은 페이지 넘버)
    //miLDR_YCmp    -> pnLDR_YCmp1    (1은 페이지 넘버)

    PM.SetWindow(pnLDR_YCmpp , miLDR_YCmp);
    PM.SetWindow(pnLDR_ZCmpp , miLDR_ZCmp);
    PM.SetWindow(pnPRB_XIdxp , miPRB_XIdx);
    PM.SetWindow(pnPRB_YInsp , miPRB_YIns);
    PM.SetWindow(pnWRE_XIdxp , miWRE_XIdx);
    PM.SetWindow(pnWRE_XEpxp , miWRE_XEpx);
    PM.SetWindow(pnWRE_YEpxp , miWRE_YEpx);
    PM.SetWindow(pnWRE_ZEpxp , miWRE_ZEpx);
    PM.SetWindow(pnWRD_XIdxp , miWRD_XIdx);
    PM.SetWindow(pnWRD_YInsp , miWRD_YIns);
    PM.SetWindow(pnWRD_XHedp , miWRD_XHed);
    PM.SetWindow(pnWRD_YHedp , miWRD_YHed);
    PM.SetWindow(pnWRD_ZHedp , miWRD_ZHed);
    PM.SetWindow(pnWRD_THedp , miWRD_THed);
    PM.SetWindow(pnWRD_XStgp , miWRD_XStg);
    PM.SetWindow(pnWRD_YStgp , miWRD_YStg);
    PM.SetWindow(pnWRD_TStgp , miWRD_TStg);
    PM.SetWindow(pnWRD_ZEjtp , miWRD_ZEjt); //PM.SetWindow(pnWRD_ZEjtp1 , miWRD_ZEjt);
    PM.SetWindow(pnWRD_ZExdp , miWRD_ZExd);
    PM.SetWindow(pnWRD_ZCstp , miWRD_ZCst);
    PM.SetWindow(pnWRD_XTrsp , miWRD_XTrs);
    PM.SetWindow(pnPSB_XIdxp , miPSB_XIdx);
    PM.SetWindow(pnULD_YCmpp , miULD_YCmp);
    PM.SetWindow(pnULD_ZCmpp , miULD_ZCmp);

    //Motor Window Set to Panel
    MtWindowSet();
    AtWindowSet();

    pnVisnSet -> Left = 2  ;
    pnVisnSet -> Top  = 40 ;
    pnVisnSet -> Visible = false ;

    //Fixed Value .
//    PM.SetPos(miWRE_XIdx,pvWRE_XIdxPull,PM.GetValue (miWRE_XIdx , pvWRE_XIdxWorkStt ) + (OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 1))); //고정.
//    PM.SetPos(miWRD_XIdx,pvWRD_XIdxPull,PM.GetValue (miWRD_XIdx , pvWRD_XIdxWorkStt ) + (OM.DevInfo.dColPitch * (OM.DevInfo.iColCnt - 1)));

    //IO Num -> Tag
//    btEpoxySuction -> Tag = (int)yEJT_Suction ;

    btDieVauum     -> Tag = (int)yHED_Vaccum  ;
    btDieIjector   -> Tag = (int)yHED_Eject   ;

    btDieVauum1    -> Tag = (int)yHED_Vaccum  ;
    btDieIjector1  -> Tag = (int)yHED_Eject   ;

    btEjtSuction   -> Tag = (int)yEJT_Suction ;
    btEjtSuction1  -> Tag = (int)yEJT_Suction ;

    btWRDVac       -> Tag = (int)yWRD_Vaccum  ;
    btWRDEjt       -> Tag = (int)yWRD_Eject   ;


    //Cb Index
    cbPRB_XRgNo -> ItemIndex = 0 ;
    cbWRE_XRgNo -> ItemIndex = 0 ;
    cbWRD_XRgNo -> ItemIndex = 0 ;
    cbPSB_XRgNo -> ItemIndex = 0 ;

    for (int i = 0; i < ComponentCount; i++)
    {
        if (Components[i]->ClassNameIs("TEdit"))
        {
            ((TEdit*)FindComponent(Components[i]->Name))->ImeMode    = imDisable ;
            ((TEdit*)FindComponent(Components[i]->Name))->NumberOnly = true ;
            ((TEdit*)FindComponent(Components[i]->Name))->UseApoint  = true ;
            ((TEdit*)FindComponent(Components[i]->Name))->UseMinus   = true ;
        }
        else if(Components[i]->ClassNameIs("TPageControl"))
        {
            ((TPageControl*)FindComponent(Components[i]->Name))->ActivePageIndex = 0 ;
        }
    }
//    pcDeviceSet -> ActivePageIndex = 0 ;
//    pcSTG_MT    -> ActivePageIndex = 0 ;
//    pcDIE_MT    -> ActivePageIndex = 0 ;

    //rgUnitUser
    rgUnitUser = new TRadioGroup(this);
    edUnitUser = new TEdit      (this);

    edGoodStr1 -> NumberOnly = false ;
    edEmptStr1 -> NumberOnly = false ;

//    DM.ARAY[riDFM].SetParent(pnDFM ); DM.ARAY[riDFM].SetConfig(true  , "DFM "); DM.ARAY[riDFM].ClearDispItem();
//    DM.ARAY[riDFM].SetDispItem(csNone   ,"None "      ) ;
//    DM.ARAY[riDFM].SetDispItem(csUnkwn  ,"Unkwn "     ) ;
//    DM.ARAY[riDFM].PopupMenuEnable(true);
//
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::MtWindowSet()
{

    for(int i=0 ; i<MAX_MOTR ; i++) {
        FraMotor  [i] = new TFraMotr();
        FraMotor  [i] ->SetIdType(i ,MT_GetDirType(i));
        FraMotor  [i] ->Show();
    }
//    FraMotor  [miLDR_YCmp] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miLDR_ZCmp] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miPRB_XIdx] ->SetPitch(OM.DevInfo.dColPitch);
    FraMotor  [miPRB_YIns] ->SetPitch(OM.DevInfo.dRowPitch);
    FraMotor  [miWRE_XIdx] ->SetPitch(OM.DevInfo.dColPitch);
//    FraMotor  [miWRE_XEpx] ->SetPitch(OM.DevInfo.dRowPitch);
    FraMotor  [miWRE_YEpx] ->SetPitch(OM.DevInfo.dRowPitch);
//    FraMotor  [miWRE_ZEpx] ->SetPitch(OM.DevInfo.dColPitch);
    FraMotor  [miWRD_XIdx] ->SetPitch(OM.DevInfo.dColPitch);
    FraMotor  [miWRD_YIns] ->SetPitch(OM.DevInfo.dRowPitch);
    FraMotor  [miWRD_XHed] ->SetPitch(OM.DevInfo.dWf_PitchX);
    FraMotor  [miWRD_YHed] ->SetPitch(OM.DevInfo.dWf_PitchY);
//    FraMotor  [miWRD_ZHed] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
//    FraMotor  [miWRD_THed] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miWRD_XStg] ->SetPitch(OM.DevInfo.dWf_PitchX);
    FraMotor  [miWRD_YStg] ->SetPitch(OM.DevInfo.dWf_PitchY);
//    FraMotor  [miWRD_TStg] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
//    FraMotor  [miWRD_ZEjt] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
//    FraMotor  [miWRD_ZExd] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miWRD_ZCst] ->SetPitch(OM.DevInfo.dCsSlotPitch);
//    FraMotor  [miWRD_XTrs] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miPSB_XIdx] ->SetPitch(OM.DevInfo.dColPitch);
//    FraMotor  [miULD_YCmp] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miULD_ZCmp] ->SetPitch(OM.DevInfo.dMgzSlotPitch);

//    for(int i=0 ; i<MAX_MOTR ; i++) FraMotor[i]->Show();
}
void __fastcall TFrmDeviceSet::MtWindowSetPitch()
{
//    FraMotor  [miLDR_YCmp] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miLDR_ZCmp] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miPRB_XIdx] ->SetPitch(OM.DevInfo.dColPitch);
    FraMotor  [miPRB_YIns] ->SetPitch(OM.DevInfo.dRowPitch);
    FraMotor  [miWRE_XIdx] ->SetPitch(OM.DevInfo.dColPitch);
//    FraMotor  [miWRE_XEpx] ->SetPitch(OM.DevInfo.dRowPitch);
    FraMotor  [miWRE_YEpx] ->SetPitch(OM.DevInfo.dRowPitch);
//    FraMotor  [miWRE_ZEpx] ->SetPitch(OM.DevInfo.dColPitch);
    FraMotor  [miWRD_XIdx] ->SetPitch(OM.DevInfo.dColPitch);
    FraMotor  [miWRD_YIns] ->SetPitch(OM.DevInfo.dRowPitch);
    FraMotor  [miWRD_XHed] ->SetPitch(OM.DevInfo.dWf_PitchX);
    FraMotor  [miWRD_YHed] ->SetPitch(OM.DevInfo.dWf_PitchY);
//    FraMotor  [miWRD_ZHed] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
//    FraMotor  [miWRD_THed] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miWRD_XStg] ->SetPitch(OM.DevInfo.dWf_PitchX);
    FraMotor  [miWRD_YStg] ->SetPitch(OM.DevInfo.dWf_PitchY);
//    FraMotor  [miWRD_TStg] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
//    FraMotor  [miWRD_ZEjt] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
//    FraMotor  [miWRD_ZExd] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miWRD_ZCst] ->SetPitch(OM.DevInfo.dCsSlotPitch);
//    FraMotor  [miWRD_XTrs] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miPSB_XIdx] ->SetPitch(OM.DevInfo.dColPitch);
//    FraMotor  [miULD_YCmp] ->SetPitch(OM.DevInfo.dMgzSlotPitch);
    FraMotor  [miULD_ZCmp] ->SetPitch(OM.DevInfo.dMgzSlotPitch);

}
void __fastcall TFrmDeviceSet::AtWindowSet()
{
    //Cylinder Window Set to Panel
    for(int i=0 ; i<MAX_ACTR ; i++) {
        FraCylinder[i] = new TFraCyl();
        FraCylinder[i] ->SetIdType(i ,AT_GetDirType(i));
        FraCylinder[i] ->Show();
    }

//    for(int i=0 ; i<MAX_ACTR ; i++) FraCylinder[i]->Show();
}
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    AI_SetY(VOICE_COIL_ID , 0.0 ) ; //OM.DevOptn.dVoiceCoilMove) ; //보이스 코일

    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    //JobFileBackUp();

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    UpdateCmnOptn(toBuff);
    OM.SaveCmnOptn();

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    //DataMan SetMaxColRow
    DM.ARAY[riLDR].SetMaxColRow(1,OM.DevInfo.iMgzSlotCnt);
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riREV].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riREF].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riRDV].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riRDF].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riHED].SetMaxColRow(1,1);
    DM.ARAY[riCST].SetMaxColRow(1,OM.DevInfo.iCsSlotCnt);
    if(!OM.CmnOptn.bUseMapFile)DM.ARAY[riSTG].SetMaxColRow(1,1);//MAX_WAFER_COL,MAX_WAFER_ROW);
    DM.ARAY[riPSV].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riPSF].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riULD].SetMaxColRow(1,OM.DevInfo.iMgzSlotCnt);
    DM.ARAY[riMAP].SetMaxColRow(MAX_WAFER_COL,MAX_WAFER_ROW);

    MtWindowSetPitch();

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::JobFileBackUp()
{
    String sSorc  = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
    String sPath  = OM.MstOptn.sDeviceBackUpPath               + "\\" ;
    String SDest  = sPath + Now().FormatString("yyyymmdd") + "\\" + Now().FormatString("hh'h'nn'm'ss's'") + "\\" ;

    UserFile.ClearDirDate(sPath , Now() - 90);

    sPath += Now().FormatString("yyyymmdd") + "\\" + Now().FormatString("hh'h'nn'm'ss's'") + "\\" ;

    UserFile.CopyDir(sSorc , SDest);

    //Trace Log.
//    Trace("JobFile",(edToName -> Text + " is Maked by Copy").c_str());

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormDestroy(TObject *Sender)
{
    tmUpdate->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevInfo(bool bTo)
{
    if(bTo)
    {
        edColPitch      -> Text = OM.DevInfo.dColPitch     ;
        edRowPitch      -> Text = OM.DevInfo.dRowPitch     ;
        edColGroupCount -> Text = OM.DevInfo.iColGrCnt     ;
        edRowGroupCount -> Text = OM.DevInfo.iRowGrCnt     ;
        edColGroupGap   -> Text = OM.DevInfo.dColGrGap     ;
        edRowGroupGap   -> Text = OM.DevInfo.dRowGrGap     ;
        edColCount      -> Text = OM.DevInfo.iColCnt       ;
        edRowCount      -> Text = OM.DevInfo.iRowCnt       ;

        edMgzSlotCnt    -> Text = OM.DevInfo.iMgzSlotCnt   ;
        edMgzSlotPitch  -> Text = OM.DevInfo.dMgzSlotPitch ;

        edCsSlotCnt     -> Text = OM.DevInfo.iCsSlotCnt    ;
        edCsSlotPitch   -> Text = OM.DevInfo.dCsSlotPitch  ;

        edWf_PitchX     -> Text = OM.DevInfo.dWf_PitchX    ;
        edWf_PitchY     -> Text = OM.DevInfo.dWf_PitchY    ;
        edWf_BldOfs     -> Text = OM.DevInfo.dWf_BldOfs    ;
        edWf_Diameter   -> Text = OM.DevInfo.dWf_Diameter  ;

        cbMapType       -> ItemIndex = OM.DevInfo.iMapType    ;
        edWColCnt       -> Text      = OM.DevInfo.iWColCnt    ;
        edWRowCnt       -> Text      = OM.DevInfo.iWRowCnt    ;

        edGoodStr1      -> Text      = OM.DevInfo.sGoodStr1   ;
        edEmptStr1      -> Text      = OM.DevInfo.sEmptStr1   ;

        rgFlatAngle     -> ItemIndex = OM.DevInfo.iFlatAngle  ;

        edWXOffset      -> Text      = OM.DevInfo.iWXOffset   ;
        edWYOffset      -> Text      = OM.DevInfo.iWYOffset   ;

        ediStrSttRow    -> Text      = OM.DevInfo.iStrSttRow  ;
        ediStrSttCol    -> Text      = OM.DevInfo.iStrSttCol  ;


    }
    else
    {
        OM.DevInfo.dColPitch     = edColPitch      -> Text.ToDouble() ;
        OM.DevInfo.dRowPitch     = edRowPitch      -> Text.ToDouble() ;
        OM.DevInfo.iColGrCnt     = edColGroupCount -> Text.ToInt()    ;
        OM.DevInfo.iRowGrCnt     = edRowGroupCount -> Text.ToInt()    ;
        OM.DevInfo.dColGrGap     = edColGroupGap   -> Text.ToDouble() ;
        OM.DevInfo.dRowGrGap     = edRowGroupGap   -> Text.ToDouble() ;
        OM.DevInfo.iColCnt       = edColCount      -> Text.ToInt()    ;
        OM.DevInfo.iRowCnt       = edRowCount      -> Text.ToInt()    ;

        OM.DevInfo.iMgzSlotCnt   = edMgzSlotCnt    -> Text.ToInt()    ;
        OM.DevInfo.dMgzSlotPitch = edMgzSlotPitch  -> Text.ToDouble() ;

        OM.DevInfo.iCsSlotCnt    = edCsSlotCnt     -> Text.ToInt()    ;
        OM.DevInfo.dCsSlotPitch  = edCsSlotPitch   -> Text.ToDouble() ;

        OM.DevInfo.dWf_PitchX    = edWf_PitchX     -> Text.ToDouble() ;
        OM.DevInfo.dWf_PitchY    = edWf_PitchY     -> Text.ToDouble() ;
        OM.DevInfo.dWf_BldOfs    = edWf_BldOfs     -> Text.ToDouble() ;
        OM.DevInfo.dWf_Diameter  = edWf_Diameter   -> Text.ToDouble() ;

        OM.DevInfo.iMapType      = cbMapType       -> ItemIndex       ;
        OM.DevInfo.iWColCnt      = edWColCnt       -> Text.ToInt()    ;
        OM.DevInfo.iWRowCnt      = edWRowCnt       -> Text.ToInt()    ;

        OM.DevInfo.sGoodStr1     = edGoodStr1      -> Text            ;
        OM.DevInfo.sEmptStr1     = edEmptStr1      -> Text            ;

        OM.DevInfo.iFlatAngle    = rgFlatAngle     -> ItemIndex       ;

        OM.DevInfo.iWXOffset     = edWXOffset      -> Text.ToInt()    ;
        OM.DevInfo.iWYOffset     = edWYOffset      -> Text.ToInt()    ;

        OM.DevInfo.iStrSttRow    = ediStrSttRow    -> Text.ToInt()    ;
        OM.DevInfo.iStrSttCol    = ediStrSttCol    -> Text.ToInt()    ;

    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
    if(bTo)
    {
        ediLotEndMgzCnt    ->Text = OM.DevOptn.iLotEndMgzCnt   ;


        rgLdrOrder         ->ItemIndex = OM.DevOptn.iLdrOrder ;
        rgUdrOrder         ->ItemIndex = OM.DevOptn.iUdrOrder ;


        ediEpxEjtOnTime    ->Text = OM.DevOptn.iEpxEjtOnTime    ;
        ediEpxEjtOffTime   ->Text = OM.DevOptn.iEpxEjtOffTime   ;


        ediEpxClenCnt      -> Text = OM.DevOptn.iEpxClenCnt      ;
        ediEpxCleanDly     -> Text = OM.DevOptn.iEpxCleanDly     ;
        ediEpxCleanShotCnt -> Text = OM.DevOptn.iEpxCleanShotCnt ;

        ediEpxTime         -> Text = OM.DevOptn.iEpxTime         ;
        ediVacuumOnDly     -> Text = OM.DevOptn.iVacuumOnDly     ;
        ediVacuumOffDly    -> Text = OM.DevOptn.iVacuumOffDly    ;

        ediEjtSuckOnDly    -> Text = OM.DevOptn.iEjtSuckOnDly    ;

        ediOriVsFailCnt  ->Text = OM.DevOptn.iOriVsFailCnt   ;
        ediOriVsBfDly    ->Text = OM.DevOptn.iOriVsBfDly     ;
        ediOriVsGrabTO   ->Text = OM.DevOptn.iOriVsGrabTO    ;
        ediOriVsInspTO   ->Text = OM.DevOptn.iOriVsInspTO    ;

        ediWfrVsFailCnt   -> Text = OM.DevOptn.iWfrVsFailCnt    ;
        ediWfrVsBfDly     -> Text = OM.DevOptn.iWfrVsBfDly      ;
        ediWfrVsGrabTO    -> Text = OM.DevOptn.iWfrVsGrabTO     ;
        ediWfrVsInspTO    -> Text = OM.DevOptn.iWfrVsInspTO     ;

        ediRalBfVsFailCnt -> Text = OM.DevOptn.iRalBfVsFailCnt  ;
        ediRalBfVsBfDly   -> Text = OM.DevOptn.iRalBfVsBfDly    ;
        ediRalBfVsGrabTO  -> Text = OM.DevOptn.iRalBfVsGrabTO   ;
        ediRalBfVsInspTO  -> Text = OM.DevOptn.iRalBfVsInspTO   ;

        ediRalAtVsFailCnt -> Text = OM.DevOptn.iRalAtVsFailCnt  ;
        ediRalAtVsBfDly   -> Text = OM.DevOptn.iRalAtVsBfDly    ;
        ediRalAtVsGrabTO  -> Text = OM.DevOptn.iRalAtVsGrabTO   ;
        ediRalAtVsInspTO  -> Text = OM.DevOptn.iRalAtVsInspTO   ;

        eddVoiceCoilStrip -> Text = OM.DevOptn.dVoiceCoilStrip  ;
        eddVoiceCoilStrip1-> Text = OM.DevOptn.dVoiceCoilStrip1 ;

        eddVoiceCoilMove  -> Text = OM.DevOptn.dVoiceCoilMove   ;

        eddVoiceCoilWafer -> Text = OM.DevOptn.dVoiceCoilWafer  ;
        eddVoiceCoilWafer1-> Text = OM.DevOptn.dVoiceCoilWafer1 ;

        //Regrip SFJasdfjasdfj
        edPRB_XRgCnt    -> Text = OM.DevOptn.iPRB_XRgCnt      ;
        edWRE_XRgCnt    -> Text = OM.DevOptn.iWRE_XRgCnt      ;
        edWRD_XRgCnt    -> Text = OM.DevOptn.iWRD_XRgCnt      ;
        edPSB_XRgCnt    -> Text = OM.DevOptn.iPSB_XRgCnt      ;

        edPRB_XWkCon    -> Text = OM.DevOptn.iPRB_XWkCon      ;
        edWRE_XWkCon    -> Text = OM.DevOptn.iWRE_XWkCon      ;
        edWRD_XWkCon    -> Text = OM.DevOptn.iWRD_XWkCon      ;
        edPSB_XWkCon    -> Text = OM.DevOptn.iPSB_XWkCon      ;

        for(int i=0; i<MAX_REGRIP_ARRAY; i++){
            m_iPRBRgCol[i] = OM.DevOptn.iPRBRgCol[i] ;
            m_iPRBRgDis[i] = OM.DevOptn.iPRBRgDis[i] ;
            m_iWRERgCol[i] = OM.DevOptn.iWRERgCol[i] ;
            m_iWRERgDis[i] = OM.DevOptn.iWRERgDis[i] ;
            m_iWRDRgCol[i] = OM.DevOptn.iWRDRgCol[i] ;
            m_iWRDRgDis[i] = OM.DevOptn.iWRDRgDis[i] ;
            m_iPSBRgCol[i] = OM.DevOptn.iPSBRgCol[i] ;
            m_iPSBRgDis[i] = OM.DevOptn.iPSBRgDis[i] ;

        }

        edDFMDiameter   -> Text = OM.DevOptn.dDFMDiameter ;
        edDFMWidth      -> Text = OM.DevOptn.dDFMWidth    ;
        edDFMHeight     -> Text = OM.DevOptn.dDFMHeight   ;
        edDFMSttOfsX    -> Text = OM.DevOptn.dDFMSttOfsX  ;
        edDFMSttOfsY    -> Text = OM.DevOptn.dDFMSttOfsY  ;

        edPickWaitAc    -> Text      = OM.DevOptn.dPickWaitAc   ;
        edPickWaitDc    -> Text      = OM.DevOptn.dPickWaitDc   ;
        edPickWaitVel   -> Text      = OM.DevOptn.dPickWaitVel  ;

        edPlaceWaitAc   -> Text      = OM.DevOptn.dPlaceWaitAc  ;
        edPlaceWaitDc   -> Text      = OM.DevOptn.dPlaceWaitDc  ;
        edPlaceWaitVel  -> Text      = OM.DevOptn.dPlaceWaitVel ;

        ediEjectOnDly   -> Text      = OM.DevOptn.iEjectOnDly   ;
        edEjectorBfDnDly-> Text      = OM.DevOptn.iEjectorBfDnDly;

        cbOriMathod        -> ItemIndex = OM.DevOptn.iOriMathod        ;
        cbOperChckAtMatch  -> Checked   = OM.DevOptn.bOperChckAtMatch  ;
        edWfFullRctCntX    -> Text      = OM.DevOptn.iWfFullRctCntX    ;
        edWfFullRctCntY    -> Text      = OM.DevOptn.iWfFullRctCntY    ;
        edWfFullRctMapOfsX -> Text      = OM.DevOptn.iWfFullRctMapOfsX ;
        edWfFullRctMapOfsY -> Text      = OM.DevOptn.iWfFullRctMapOfsY ;
        edLdrCanHomePrbCnt -> Text      = OM.DevOptn.iLdrCanHomePrbCnt ;

        edSecondFindPos    -> Text      = OM.DevOptn.iSecondFindPos    ;

    }
    else
    {
        OM.DevOptn.iLdrOrder        = rgLdrOrder         ->ItemIndex       ;
        OM.DevOptn.iUdrOrder        = rgUdrOrder         ->ItemIndex       ;

        OM.DevOptn.iLotEndMgzCnt    = ediLotEndMgzCnt    ->Text.ToIntDef(0);

        OM.DevOptn.iEpxEjtOnTime    = ediEpxEjtOnTime    ->Text.ToIntDef(0);
        OM.DevOptn.iEpxEjtOffTime   = ediEpxEjtOffTime   ->Text.ToIntDef(0);


        OM.DevOptn.iEpxClenCnt      = ediEpxClenCnt      ->Text.ToIntDef(0);
        OM.DevOptn.iEpxCleanDly     = ediEpxCleanDly     ->Text.ToIntDef(0);
        OM.DevOptn.iEpxCleanShotCnt = ediEpxCleanShotCnt ->Text.ToIntDef(1);
        OM.DevOptn.iEpxTime         = ediEpxTime         ->Text.ToIntDef(5); if(OM.DevOptn.iEpxTime < 5) OM.DevOptn.iEpxTime = 5 ; //5이하이면 클린시에 통신에러.
        OM.DevOptn.iVacuumOnDly     = ediVacuumOnDly     ->Text.ToIntDef(0);
        OM.DevOptn.iVacuumOffDly    = ediVacuumOffDly    ->Text.ToIntDef(0);
        OM.DevOptn.iEjtSuckOnDly    = ediEjtSuckOnDly    ->Text.ToIntDef(0);


        OM.DevOptn.iOriVsFailCnt    = ediOriVsFailCnt    ->Text.ToIntDef(0);
        OM.DevOptn.iOriVsBfDly      = ediOriVsBfDly      ->Text.ToIntDef(0);
        OM.DevOptn.iOriVsGrabTO     = ediOriVsGrabTO     ->Text.ToIntDef(0);
        OM.DevOptn.iOriVsInspTO     = ediOriVsInspTO     ->Text.ToIntDef(0);

        OM.DevOptn.iWfrVsFailCnt    = ediWfrVsFailCnt    ->Text.ToIntDef(0);
        OM.DevOptn.iWfrVsBfDly      = ediWfrVsBfDly      ->Text.ToIntDef(0);
        OM.DevOptn.iWfrVsGrabTO     = ediWfrVsGrabTO     ->Text.ToIntDef(0);
        OM.DevOptn.iWfrVsInspTO     = ediWfrVsInspTO     ->Text.ToIntDef(0);

        OM.DevOptn.iRalBfVsFailCnt  = ediRalBfVsFailCnt  ->Text.ToIntDef(0);
        OM.DevOptn.iRalBfVsBfDly    = ediRalBfVsBfDly    ->Text.ToIntDef(0);
        OM.DevOptn.iRalBfVsGrabTO   = ediRalBfVsGrabTO   ->Text.ToIntDef(0);
        OM.DevOptn.iRalBfVsInspTO   = ediRalBfVsInspTO   ->Text.ToIntDef(0);

        OM.DevOptn.iRalAtVsFailCnt  = ediRalAtVsFailCnt  ->Text.ToIntDef(0);
        OM.DevOptn.iRalAtVsBfDly    = ediRalAtVsBfDly    ->Text.ToIntDef(0);
        OM.DevOptn.iRalAtVsGrabTO   = ediRalAtVsGrabTO   ->Text.ToIntDef(0);
        OM.DevOptn.iRalAtVsInspTO   = ediRalAtVsInspTO   ->Text.ToIntDef(0);

        OM.DevOptn.dVoiceCoilStrip  = eddVoiceCoilStrip  ->Text.ToDouble(); if(OM.DevOptn.dVoiceCoilStrip >400) OM.DevOptn.dVoiceCoilStrip  = 400 ; if(OM.DevOptn.dVoiceCoilStrip <20) OM.DevOptn.dVoiceCoilStrip  = 20 ;
        OM.DevOptn.dVoiceCoilStrip1 = eddVoiceCoilStrip1 ->Text.ToDouble(); if(OM.DevOptn.dVoiceCoilStrip1>400) OM.DevOptn.dVoiceCoilStrip1 = 400 ; if(OM.DevOptn.dVoiceCoilStrip1<20) OM.DevOptn.dVoiceCoilStrip1 = 20 ;
        OM.DevOptn.dVoiceCoilMove   = eddVoiceCoilMove   ->Text.ToDouble(); if(OM.DevOptn.dVoiceCoilMove  >400) OM.DevOptn.dVoiceCoilMove   = 400 ; if(OM.DevOptn.dVoiceCoilMove  <20) OM.DevOptn.dVoiceCoilMove   = 20 ;
        OM.DevOptn.dVoiceCoilWafer  = eddVoiceCoilWafer  ->Text.ToDouble(); if(OM.DevOptn.dVoiceCoilWafer >400) OM.DevOptn.dVoiceCoilWafer  = 400 ; if(OM.DevOptn.dVoiceCoilWafer <20) OM.DevOptn.dVoiceCoilWafer  = 20 ;
        OM.DevOptn.dVoiceCoilWafer1 = eddVoiceCoilWafer1 ->Text.ToDouble(); if(OM.DevOptn.dVoiceCoilWafer1>400) OM.DevOptn.dVoiceCoilWafer1 = 400 ; if(OM.DevOptn.dVoiceCoilWafer1<20) OM.DevOptn.dVoiceCoilWafer1 = 20 ;

        //Regrip sadfklllljs
        OM.DevOptn.iPRB_XRgCnt     = edPRB_XRgCnt     -> Text.ToIntDef(0);
        OM.DevOptn.iWRE_XRgCnt     = edWRE_XRgCnt     -> Text.ToIntDef(0);
        OM.DevOptn.iWRD_XRgCnt     = edWRD_XRgCnt     -> Text.ToIntDef(0);
        OM.DevOptn.iPSB_XRgCnt     = edPSB_XRgCnt     -> Text.ToIntDef(0);

        OM.DevOptn.iPRB_XWkCon     = edPRB_XWkCon     -> Text.ToIntDef(0);
        OM.DevOptn.iWRE_XWkCon     = edWRE_XWkCon     -> Text.ToIntDef(0);
        OM.DevOptn.iWRD_XWkCon     = edWRD_XWkCon     -> Text.ToIntDef(0);
        OM.DevOptn.iPSB_XWkCon     = edPSB_XWkCon     -> Text.ToIntDef(0);


        for(int i=0; i<MAX_REGRIP_ARRAY; i++){
            OM.DevOptn.iPRBRgCol[i] = m_iPRBRgCol[i];
            OM.DevOptn.iPRBRgDis[i] = m_iPRBRgDis[i];
            OM.DevOptn.iWRERgCol[i] = m_iWRERgCol[i];
            OM.DevOptn.iWRERgDis[i] = m_iWRERgDis[i];
            OM.DevOptn.iWRDRgCol[i] = m_iWRDRgCol[i];
            OM.DevOptn.iWRDRgDis[i] = m_iWRDRgDis[i];
            OM.DevOptn.iPSBRgCol[i] = m_iPSBRgCol[i];
            OM.DevOptn.iPSBRgDis[i] = m_iPSBRgDis[i];

        }

        OM.DevOptn.dDFMDiameter   =  edDFMDiameter   -> Text.ToDouble() ;
        OM.DevOptn.dDFMWidth      =  edDFMWidth      -> Text.ToDouble() ;
        OM.DevOptn.dDFMHeight     =  edDFMHeight     -> Text.ToDouble() ;
        OM.DevOptn.dDFMSttOfsX    =  edDFMSttOfsX    -> Text.ToDouble() ;
        OM.DevOptn.dDFMSttOfsY    =  edDFMSttOfsY    -> Text.ToDouble() ;

        OM.DevOptn.dPickWaitAc    = edPickWaitAc    -> Text.ToDouble( );
        OM.DevOptn.dPickWaitDc    = edPickWaitDc    -> Text.ToDouble( );
        OM.DevOptn.dPickWaitVel   = edPickWaitVel   -> Text.ToDouble( );

        OM.DevOptn.dPlaceWaitAc   = edPlaceWaitAc   -> Text.ToDouble( );
        OM.DevOptn.dPlaceWaitDc   = edPlaceWaitDc   -> Text.ToDouble( );
        OM.DevOptn.dPlaceWaitVel  = edPlaceWaitVel  -> Text.ToDouble( );

        OM.DevOptn.iEjectOnDly    = ediEjectOnDly   -> Text.ToIntDef(0);
        OM.DevOptn.iEjectorBfDnDly= edEjectorBfDnDly-> Text.ToIntDef(0);

        OM.DevOptn.iOriMathod        = cbOriMathod        -> ItemIndex ;
        OM.DevOptn.bOperChckAtMatch  = cbOperChckAtMatch  -> Checked   ;
        OM.DevOptn.iWfFullRctCntX    = edWfFullRctCntX    -> Text.ToIntDef(0) ;
        OM.DevOptn.iWfFullRctCntY    = edWfFullRctCntY    -> Text.ToIntDef(0) ;
        OM.DevOptn.iWfFullRctMapOfsX = edWfFullRctMapOfsX -> Text.ToIntDef(0) ;
        OM.DevOptn.iWfFullRctMapOfsY = edWfFullRctMapOfsY -> Text.ToIntDef(0) ;

        OM.DevOptn.iLdrCanHomePrbCnt = edLdrCanHomePrbCnt -> Text.ToIntDef(0) ;

        OM.DevOptn.iSecondFindPos    = edSecondFindPos    -> Text.ToIntDef(0) ;


        //Rs232 Dispensor -> Set Time
        RSM.SetTime(OM.DevOptn.iEpxTime );

        UpdateDevOptn(true);
    }
}

void __fastcall TFrmDeviceSet::UpdateCmnOptn(bool bTo)
{
    if(bTo){
        edServerPath -> Text = OM.CmnOptn.sSourcePath;
        edWorkedPath -> Text = OM.CmnOptn.sWorkedPath;
    }
    else {
        OM.CmnOptn.sSourcePath = edServerPath -> Text;
        OM.CmnOptn.sWorkedPath = edWorkedPath -> Text;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    UpdateCmnOptn (toTabl);
    PM.UpdatePstn (toTabl);

    pnVisnSet -> Visible = false ;

    tmUpdate->Enabled = true;
//    pcDeviceSet -> ActivePageIndex = 0 ;
    SV_SetVisnForm(pnVisnSet -> Handle);


     switch ( pcDeviceSet -> ActivePageIndex ) {
         case 0 :                           break ;
         case 1 :                           break ;
         case 2 : rgUnitUser = rgJogUnit  ;
                  edUnitUser = edJogUnit  ; break ;
         case 3 : rgUnitUser = rgJogUnit1 ;
                  edUnitUser = edJogUnit1 ; break ;
         case 4 : rgUnitUser = rgJogUnit2 ;
                  edUnitUser = edJogUnit2 ; break ;
         case 5 : rgUnitUser = rgJogUnit3 ;
                  edUnitUser = edJogUnit3 ; break ;
         case 6 : rgUnitUser = rgJogUnit4 ;
                  edUnitUser = edJogUnit4 ; break ;
         case 7 : rgUnitUser = rgJogUnit5 ;
                  edUnitUser = edJogUnit5 ; break ;
         case 8 : rgUnitUser = rgJogUnit6 ;
                  edUnitUser = edJogUnit6 ; break ;
     }

    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.01);
    if(rgUnitUser->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
    if(rgUnitUser->ItemIndex == 7) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utPitch,0  );
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{
    AI_SetY(VOICE_COIL_ID , 0.0 ) ; //OM.DevOptn.dVoiceCoilMove) ; //보이스 코일
    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 :

                 break;
        case 1 : break ;

        case 2 : FraMotor[miLDR_ZCmp]->Parent = pnLDR_ZCmp ;
                 FraMotor[miLDR_YCmp]->Parent = pnLDR_YCmp ;

                 FraCylinder[aiLDR_Stopper ]->Parent = pnLDR_Stopper1   ;
                 FraCylinder[aiLDR_MgzGrip ]->Parent = pnLDR_MgzGrip1   ;
                 FraCylinder[aiLDR_Out     ]->Parent = pnLDR_Out1       ;
                 FraCylinder[aiLDR_Pusher  ]->Parent = pnLDR_Pusher1    ;
                 FraCylinder[aiPRB_Stopper ]->Parent = pnPRB_Stopper1   ;
                 FraCylinder[aiPRB_Clamp   ]->Parent = pnPRB_Clamp1     ;
                 //FraCylinder[aiPRB_CAlign  ]->Parent = pnPRB_CAlign1    ;

                 PM.SetParent(pnLDR_ZCmpp ,miLDR_ZCmp);
                 PM.SetParent(pnLDR_YCmpp ,miLDR_YCmp);

                 break;

        case 3 : FraMotor[miPRB_XIdx]->Parent = pnPRB_XIdx ;
                 FraMotor[miPRB_YIns]->Parent = pnPRB_YIns ;

                 FraCylinder[aiLDR_Pusher  ]->Parent = pnLDR_Pusher2   ;
                 FraCylinder[aiPRB_Stopper ]->Parent = pnPRB_Stopper2  ;
                 FraCylinder[aiPRB_Clamp   ]->Parent = pnPRB_Clamp2    ;
                 //FraCylinder[aiPRB_CAlign  ]->Parent = pnPRB_CAlign2   ;
                 //FraCylinder[aiWRE_BAlign  ]->Parent = pnWRE_BAlign2   ;
                 FraCylinder[aiWRE_Clamp   ]->Parent = pnWRE_Clamp2    ;
                 //FraCylinder[aiWRE_CAlign  ]->Parent = pnWRE_CAlign2   ;

                 PM.SetParent(pnPRB_XIdxp ,miPRB_XIdx);
                 PM.SetParent(pnPRB_YInsp ,miPRB_YIns);

                 break;

        case 4 : FraMotor[miWRE_XIdx]->Parent = pnWRE_XIdx ;
                 FraMotor[miWRE_XEpx]->Parent = pnWRE_XEpx ;
                 FraMotor[miWRE_YEpx]->Parent = pnWRE_YEpx ;
                 FraMotor[miWRE_ZEpx]->Parent = pnWRE_ZEpx ;

                 //FraCylinder[aiWRE_BAlign   ]->Parent = pnWRE_BAlign3   ;
                 FraCylinder[aiWRE_Clamp    ]->Parent = pnWRE_Clamp3    ;
                 //FraCylinder[aiWRE_CAlign   ]->Parent = pnWRE_CAlign3   ;
//                 FraCylinder[aiWRD_Align    ]->Parent = pnWRD_Align3    ;
                 FraCylinder[aiWRD_Clamp    ]->Parent = pnWRD_Clamp3    ;
                 //FraCylinder[aiWRD_CAlign   ]->Parent = pnWRD_CAlign3   ;

                 PM.SetParent(pnWRE_XIdxp ,miWRE_XIdx);
                 PM.SetParent(pnWRE_XEpxp ,miWRE_XEpx);
                 PM.SetParent(pnWRE_YEpxp ,miWRE_YEpx);
                 PM.SetParent(pnWRE_ZEpxp ,miWRE_ZEpx);

                 break;

        case 5 : FraMotor[miWRD_XIdx]->Parent = pnWRD_XIdx ;
                 FraMotor[miWRD_YIns]->Parent = pnWRD_YIns ;
                 FraMotor[miWRD_ZEjt]->Parent = pnWRD_ZEjt ;
                 FraMotor[miWRD_XHed]->Parent = pnWRD_XHed ;
                 FraMotor[miWRD_YHed]->Parent = pnWRD_YHed ;
                 FraMotor[miWRD_ZHed]->Parent = pnWRD_ZHed ;
                 FraMotor[miWRD_THed]->Parent = pnWRD_THed ;

//                 FraCylinder[aiWRD_Align   ]->Parent = pnWRD_Align4    ;
                 FraCylinder[aiWRD_Clamp   ]->Parent = pnWRD_Clamp4    ;
                 //FraCylinder[aiWRD_CAlign  ]->Parent = pnWRD_CAlign4   ;
                 FraCylinder[aiPSB_Clamp   ]->Parent = pnPSB_Clamp4    ;
                 FraCylinder[aiEJT_UpDn    ]->Parent = pnEJT_UpDn4     ;

                 PM.SetParent(pnWRD_XIdxp ,miWRD_XIdx);
                 PM.SetParent(pnWRD_YInsp ,miWRD_YIns);
                 PM.SetParent(pnWRD_ZEjtp ,miWRD_ZEjt);
                 PM.SetParent(pnWRD_XHedp ,miWRD_XHed);
                 PM.SetParent(pnWRD_YHedp ,miWRD_YHed);
                 PM.SetParent(pnWRD_ZHedp ,miWRD_ZHed);
                 PM.SetParent(pnWRD_THedp ,miWRD_THed);

                 break;

        case 6 : FraMotor[miWRD_XStg]->Parent = pnWRD_XStg ;
                 FraMotor[miWRD_YStg]->Parent = pnWRD_YStg ;
                 FraMotor[miWRD_TStg]->Parent = pnWRD_TStg ;
                 FraMotor[miWRD_ZExd]->Parent = pnWRD_ZExd ;
                 FraMotor[miWRD_XTrs]->Parent = pnWRD_XTrs ;
                 FraMotor[miWRD_ZEjt]->Parent = pnWRD_ZEjt1 ;
                 FraMotor[miWRD_ZCst]->Parent = pnWRD_ZCst ;

//                 FraCylinder[aiWRD_Align  ]->Parent = pnWRD_Align5    ;
                 FraCylinder[aiWRD_Clamp  ]->Parent = pnWRD_Clamp5    ;
                 //FraCylinder[aiWRD_CAlign ]->Parent = pnWRD_CAlign5   ;
                 FraCylinder[aiEJT_UpDn   ]->Parent = pnEJT_UpDn5     ;
                 FraCylinder[aiTRS_Chuck  ]->Parent = pnTRS_Chuck5    ;
                 FraCylinder[aiTRS_UpDn   ]->Parent = pnTRS_UpDn5     ;

                 PM.SetParent(pnWRD_XStgp ,miWRD_XStg);
                 PM.SetParent(pnWRD_YStgp ,miWRD_YStg);
                 PM.SetParent(pnWRD_TStgp ,miWRD_TStg);
                 PM.SetParent(pnWRD_ZExdp ,miWRD_ZExd);
                 PM.SetParent(pnWRD_XTrsp ,miWRD_XTrs);
                 PM.SetParent(pnWRD_ZEjtp1,miWRD_ZEjt);
                 PM.SetParent(pnWRD_ZCstp ,miWRD_ZCst);

                 break;

        case 7 : FraMotor[miPSB_XIdx]->Parent = pnPSB_XIdx ;

                 //FraCylinder[aiWRD_CAlign  ]->Parent = pnWRD_CAlign6   ;
                 FraCylinder[aiWRD_Clamp   ]->Parent = pnWRD_Clamp6    ;
                 FraCylinder[aiPSB_Clamp   ]->Parent = pnPSB_Clamp6    ;
                 FraCylinder[aiEJT_UpDn    ]->Parent = pnEJT_UpDn6     ;

                 PM.SetParent(pnPSB_XIdxp ,miPSB_XIdx);

                 break;

        case 8 : FraMotor[miULD_ZCmp]->Parent = pnULD_ZCmp ;
                 FraMotor[miULD_YCmp]->Parent = pnULD_YCmp ;

                 FraCylinder[aiULD_Stopper ]->Parent = pnULD_Stopper7   ;
                 FraCylinder[aiULD_MgzGrip ]->Parent = pnULD_MgzGrip7   ;
                 FraCylinder[aiULD_Out     ]->Parent = pnULD_Out7       ;

                 PM.SetParent(pnULD_ZCmpp ,miULD_ZCmp);
                 PM.SetParent(pnULD_YCmpp ,miULD_YCmp);
                 break;

    }
    switch ( pcDeviceSet -> ActivePageIndex ) {
        case 0 :                           break ;
        case 1 :                           break ;
        case 2 : rgUnitUser = rgJogUnit  ;
                 edUnitUser = edJogUnit  ; break ;
        case 3 : rgUnitUser = rgJogUnit1 ;
                 edUnitUser = edJogUnit1 ; break ;
        case 4 : rgUnitUser = rgJogUnit2 ;
                 edUnitUser = edJogUnit2 ; break ;
        case 5 : rgUnitUser = rgJogUnit3 ;
                 edUnitUser = edJogUnit3 ; break ;
        case 6 : rgUnitUser = rgJogUnit4 ;
                 edUnitUser = edJogUnit4 ; break ;
        case 7 : rgUnitUser = rgJogUnit5 ;
                 edUnitUser = edJogUnit5 ; break ;
        case 8 : rgUnitUser = rgJogUnit6 ;
                 edUnitUser = edJogUnit6 ; break ;


    }

    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.01);
    if(rgUnitUser->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
    if(rgUnitUser->ItemIndex == 7) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utPitch,0  );


    switch ( pcDeviceSet -> ActivePageIndex ) {
        default: break ;
        case 3 : edPRB_XRgCol->Text = m_iPRBRgCol[cbPRB_XRgNo->ItemIndex] ;
                 edPRB_XRgDis->Text = m_iPRBRgDis[cbPRB_XRgNo->ItemIndex] ; break ;
        case 4 : edWRE_XRgCol->Text = m_iWRERgCol[cbWRE_XRgNo->ItemIndex] ;
                 edWRE_XRgDis->Text = m_iWRERgDis[cbWRE_XRgNo->ItemIndex] ; break ;
        case 5 : edWRD_XRgCol->Text = m_iWRDRgCol[cbWRD_XRgNo->ItemIndex] ;
                 edWRD_XRgDis->Text = m_iWRDRgDis[cbWRD_XRgNo->ItemIndex] ; break ;
        case 7 : edPSB_XRgCol->Text = m_iPSBRgCol[cbPSB_XRgNo->ItemIndex] ;
                 edPSB_XRgDis->Text = m_iPSBRgDis[cbPSB_XRgNo->ItemIndex] ; break ;
    }


}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::rgJogUnitClick(TObject *Sender)
{
    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.01);
    if(rgUnitUser->ItemIndex == 6) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
    if(rgUnitUser->ItemIndex == 7) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utPitch,0  );

}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btRAL_OutClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    bool bState =  IO_GetY(iSel);
    IO_SetY(iSel , !bState);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::edJogUnitChange(TObject *Sender)
{
    if(rgUnitUser->ItemIndex == 5)  for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmUpdate->Enabled = false ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btEpoxySuctionClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    IO_SetY(iSel , !IO_GetY(iSel));
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::cbPRB_XRgNoChange(TObject *Sender)
{
    String sName  = ((TComboBox *)Sender) -> Name ;
    int    iIndex = ((TComboBox *)Sender) -> ItemIndex ;

    if(sName == "cbPRB_XRgNo") {
        edPRB_XRgCol->Text = m_iPRBRgCol[iIndex] ;
        edPRB_XRgDis->Text = m_iPRBRgDis[iIndex] ;
    }
    else if(sName == "cbWRE_XRgNo") {
        edWRE_XRgCol->Text = m_iWRERgCol[iIndex] ;
        edWRE_XRgDis->Text = m_iWRERgDis[iIndex] ;

    }
    else if(sName == "cbWRD_XRgNo") {
        edWRD_XRgCol->Text = m_iWRDRgCol[iIndex] ;
        edWRD_XRgDis->Text = m_iWRDRgDis[iIndex] ;

    }
    else if(sName == "cbPSB_XRgNo") {
        edPSB_XRgCol->Text = m_iPSBRgCol[iIndex] ;
        edPSB_XRgDis->Text = m_iPSBRgDis[iIndex] ;

    }
/*
        m_iPRBRgCol[iIndex] = edPRB_XRgCol->Text.ToIntDef(0);
        m_iPRBRgDis[iIndex] = edPRB_XRgDis->Text.ToIntDef(0);
    }
    else if(sName == "cbWRE_XRgNo") {
        m_iWRERgCol[iIndex] = edWRE_XRgCol->Text.ToIntDef(0);
        m_iWRERgDis[iIndex] = edWRE_XRgDis->Text.ToIntDef(0);

    }
    else if(sName == "cbWRD_XRgNo") {
        m_iWRDRgCol[iIndex] = edWRD_XRgCol->Text.ToIntDef(0);
        m_iWRDRgDis[iIndex] = edWRD_XRgDis->Text.ToIntDef(0);

    }
    else if(sName == "cbPSB_XRgNo") {
        m_iPSBRgCol[iIndex] = edPSB_XRgCol->Text.ToIntDef(0);
        m_iPSBRgDis[iIndex] = edPSB_XRgDis->Text.ToIntDef(0);

    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::edPRB_XRgColChange(TObject *Sender)
{
    String sName  = ((TEdit *)Sender) -> Name ;

         if(sName == "edPRB_XRgCol") m_iPRBRgCol[cbPRB_XRgNo->ItemIndex] = edPRB_XRgCol->Text.ToIntDef(0);
    else if(sName == "edPRB_XRgDis") m_iPRBRgDis[cbPRB_XRgNo->ItemIndex] = edPRB_XRgDis->Text.ToIntDef(0);
    else if(sName == "edWRE_XRgCol") m_iWRERgCol[cbWRE_XRgNo->ItemIndex] = edWRE_XRgCol->Text.ToIntDef(0);
    else if(sName == "edWRE_XRgDis") m_iWRERgDis[cbWRE_XRgNo->ItemIndex] = edWRE_XRgDis->Text.ToIntDef(0);
    else if(sName == "edWRD_XRgCol") m_iWRDRgCol[cbWRD_XRgNo->ItemIndex] = edWRD_XRgCol->Text.ToIntDef(0);
    else if(sName == "edWRD_XRgDis") m_iWRDRgDis[cbWRD_XRgNo->ItemIndex] = edWRD_XRgDis->Text.ToIntDef(0);
    else if(sName == "edPSB_XRgCol") m_iPSBRgCol[cbPSB_XRgNo->ItemIndex] = edPSB_XRgCol->Text.ToIntDef(0);
    else if(sName == "edPSB_XRgDis") m_iPSBRgDis[cbPSB_XRgNo->ItemIndex] = edPSB_XRgDis->Text.ToIntDef(0);

}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btVCPickClick(TObject *Sender)
{
    String sName  = ((TEdit *)Sender) -> Name ;
         if(sName == "btVCPick" ) AI_SetY(VOICE_COIL_ID,OM.DevOptn.dVoiceCoilWafer) ;
    else if(sName == "btVCPlace") AI_SetY(VOICE_COIL_ID,OM.DevOptn.dVoiceCoilStrip) ;
    else if(sName == "btVCMove" ) AI_SetY(VOICE_COIL_ID,OM.DevOptn.dVoiceCoilMove ) ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::btShutClick(TObject *Sender)
{
    RSM.Shot();
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::btDisCheckClick(TObject *Sender)
{
    bool bTemp = RSM.CheckShot();
}
//---------------------------------------------------------------------------

//#include "SVMaths.h"
/*
#include "SVMaths.h"
void __fastcall TFrmDeviceSet::ApplyClick(TObject *Sender)
{
    double dDiameter = edDFMDiameter -> Text.ToDouble();
    double dRadius   = dDiameter / 2.0 ;
    double dWidth    = edDFMWidth    -> Text.ToDouble();
    double dHeight   = edDFMHeight   -> Text.ToDouble();
    double dSttOfsX  = edDFMSttOfsX  -> Text.ToDouble();
    double dSttOfsY  = edDFMSttOfsY  -> Text.ToDouble();

    int    iColCnt   = dDiameter / dWidth  ;
    int    iRowCnt   = dDiameter / dHeight ;

    DM.ARAY[riDFM].SetMaxColRow(iColCnt , iRowCnt);




    DM.ARAY[riDFM].SetStat(csNone);

    for(int y = 0 ; y < iRowCnt ; y++) {
        for(int x = 0 ; x < iColCnt ; x++) {
            //bool __stdcall MATH_GetCircleInPoint(float _dCntrX , float _dCntrY , float _dRadius , float _pdX , float _pdY)
            if(!MATH_GetCircleInPoint(dRadius+dSttOfsX , dRadius+dSttOfsY , dRadius , dWidth * x         , dHeight * y          ))continue ; //왼쪽위
            if(!MATH_GetCircleInPoint(dRadius+dSttOfsX , dRadius+dSttOfsY , dRadius , dWidth * x + dWidth, dHeight * y          ))continue ; //오른쪽위
            if(!MATH_GetCircleInPoint(dRadius+dSttOfsX , dRadius+dSttOfsY , dRadius , dWidth * x + dWidth, dHeight * y + dHeight))continue ; //오른쪽아래
            if(!MATH_GetCircleInPoint(dRadius+dSttOfsX , dRadius+dSttOfsY , dRadius , dWidth * x         , dHeight * y + dHeight))continue ; //왼쪽 아래.
            DM.ARAY[riDFM].SetStat(y,x,csUnkwn);
        }
    }


    DM.ARAY[riDFM].UpdateAray();

   // MATH_GetCircleInPoint(dRadius , dRadius , dRadius , float _pdX , float _pdY);
   // MATH_GetCircleInPoint(dRadius , dRadius , dRadius , float _pdX , float _pdY);
   // int    iCntCol =


} */
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btVoiceCoilTestClick(TObject *Sender)
{
    AI_SetY(VOICE_COIL_ID , HED.GetVoiceCoilVolt(edVoiceCoilTest->Text.ToDouble()) ) ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmDeviceSet::btVisionClick(TObject *Sender)
{
    if(pnVisnSet   -> Visible) {
        pnVisnSet -> Visible = false ;
    }
    else {
        pnVisnSet -> Visible = true ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::Button1Click(TObject *Sender)
{
    IO_SetY(yHED_VoiceCoilServo , true);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::Button2Click(TObject *Sender)
{
    IO_SetY(yHED_VoiceCoilServo , false);
}
//---------------------------------------------------------------------------




void __fastcall TFrmDeviceSet::btServerFolderClick(TObject *Sender)
{
    String Dir;
    SelectDirectory("Select a directory", "", Dir);
    edServerPath -> Text = Dir ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btOutputFolderClick(TObject *Sender)
{
    String Dir;
    SelectDirectory("Select a directory", "", Dir);
    ShowMessage(Dir);
    edWorkedPath -> Text = Dir ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::tmUpdateTimer(TObject *Sender)
{
     tmUpdate->Enabled = false;

    bool bFoward;
    AnsiString sMsg;

    btSaveDevice -> Enabled = !SEQ._bRun ;//&& !FrmMain -> tmMainSet -> Enabled && !FrmMain -> tmSubSet -> Enabled ;

//    btEpoxySuction ->Caption = IO_GetY(btEpoxySuction->Tag) ? "ON" : "OFF" ;
    btDieVauum     ->Caption = IO_GetY(btDieVauum    ->Tag) ? "ON" : "OFF" ; btDieVauum   -> Font  ->Color = IO_GetY(btDieVauum    ->Tag) ? clLime : clBlack ;
    btDieIjector   ->Caption = IO_GetY(btDieIjector  ->Tag) ? "ON" : "OFF" ; btDieIjector -> Font  ->Color = IO_GetY(btDieIjector  ->Tag) ? clLime : clBlack ;
    btDieVauum1    ->Caption = IO_GetY(btDieVauum1   ->Tag) ? "ON" : "OFF" ; btDieVauum1  -> Font  ->Color = IO_GetY(btDieVauum1   ->Tag) ? clLime : clBlack ;
    btDieIjector1  ->Caption = IO_GetY(btDieIjector1 ->Tag) ? "ON" : "OFF" ; btDieIjector1-> Font  ->Color = IO_GetY(btDieIjector1 ->Tag) ? clLime : clBlack ;

    btEjtSuction   ->Caption = IO_GetY(btEjtSuction  ->Tag) ? "ON" : "OFF" ; btEjtSuction -> Font  ->Color = IO_GetY(btEjtSuction  ->Tag) ? clLime : clBlack ;
    btEjtSuction1  ->Caption = IO_GetY(btEjtSuction1 ->Tag) ? "ON" : "OFF" ; btEjtSuction1-> Font  ->Color = IO_GetY(btEjtSuction1 ->Tag) ? clLime : clBlack ;

    btWRDVac       ->Caption = IO_GetY(btWRDVac      ->Tag) ? "ON" : "OFF" ; btEjtSuction -> Font  ->Color = IO_GetY(btEjtSuction  ->Tag) ? clLime : clBlack ;
    btWRDEjt       ->Caption = IO_GetY(btWRDEjt      ->Tag) ? "ON" : "OFF" ; btEjtSuction1-> Font  ->Color = IO_GetY(btEjtSuction1 ->Tag) ? clLime : clBlack ;

    btCleanerSuck  ->Caption = IO_GetY(btCleanerSuck ->Tag) ? "ON" : "OFF" ; btCleanerSuck-> Font  ->Color = IO_GetY(btCleanerSuck ->Tag) ? clLime : clBlack ;

    btDisCheck     ->Caption = RSM.CheckShot() ? "OK" : "NG" ;



    stHedVac       -> Color  = IO_GetX(xHED_Vaccum) ? clLime : clGray ;
    stRadVac       -> Color  = IO_GetX(xWRD_Vaccum) ? clLime : clGray ;

    pnWfrEdge -> Visible = cbOriMathod -> ItemIndex != omMatch ;

    static CDelayTimer tmVoiceCoil ;

    if(!SEQ._bRun) {
        if(tmVoiceCoil.OnDelay(IO_GetY(yHED_VoiceCoilServo) , 10000)) {
            IO_SetY(yHED_VoiceCoilServo,false);
            FM_MsgTime("Confirm","보이스코일 과열방지로 꺼짐.",1000);
        }
    }

    DM.UpdateAray();

    tmUpdate->Enabled = true;
}
//---------------------------------------------------------------------------





void __fastcall TFrmDeviceSet::RgCntChange(TObject *Sender)
{
    int iSel = ((TEdit *)Sender) -> Tag ;

    TEdit     * edRgCnt ;
    TComboBox * cbRgNo  ;

         if(iSel == 0 ) {edRgCnt = edPRB_XRgCnt ; cbRgNo = cbPRB_XRgNo ; }
    else if(iSel == 1 ) {edRgCnt = edWRE_XRgCnt ; cbRgNo = cbWRE_XRgNo ; }
    else if(iSel == 2 ) {edRgCnt = edWRD_XRgCnt ; cbRgNo = cbWRD_XRgNo ; }
    else if(iSel == 3 ) {edRgCnt = edPSB_XRgCnt ; cbRgNo = cbPSB_XRgNo ; }

    if(edRgCnt -> Text.ToIntDef(-999) <= 0 ) return ;

    int iRgCnt = edRgCnt -> Text.ToInt() ;
    if(iRgCnt > MAX_REGRIP_ARRAY) iRgCnt = MAX_REGRIP_ARRAY ;

    cbRgNo -> Items -> Clear() ;

    for(int i = 0 ; i < iRgCnt ; i++) {
        cbRgNo -> AddItem(AnsiString(i+1) + "번" , NULL);
    }

    cbRgNo -> ItemIndex = 0 ; 

}
//---------------------------------------------------------------------------







void __fastcall TFrmDeviceSet::btMoveStgCntrClick(TObject *Sender)
{
    if(!AT_Complete(aiEJT_UpDn , ccBwd)) {
        FM_MsgOk("ERROR","Ejector Up/Dn is Fwd(Up)");
        return ;
    }
    if(!AT_Complete(aiTRS_UpDn , ccBwd)) {
        FM_MsgOk("ERROR","Transfer Up/Dn is Fwd(Dn)");
        return ;
    }
    if(!MT_CmprPos(miWRD_ZExd,PM.GetValue(miWRD_ZExd,pvWRD_ZExdExpend))) {
        FM_MsgOk("ERROR","Expend_Z is Not Expend Position");
        return ;
    }

    MT_GoAbsMan(miWRD_XStg , OM.MstOptn.dStgXEjtCt); 
    MT_GoAbsMan(miWRD_YStg , OM.MstOptn.dStgYEjtCt); 
}
//---------------------------------------------------------------------------

