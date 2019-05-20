//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormAlgoPara.h"
#include "FormVisnPara.h"
#include "FormVisnMain.h"
#include "VOptnMan.h"
#include "Vision.h"
#include "SLogUnit.h"
#include "SVMaths.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmAlgoPara *FrmAlgoPara;
//---------------------------------------------------------------------------
__fastcall TFrmAlgoPara::TFrmAlgoPara(TComponent* Owner)
        : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;

    Width  = 500 ;
    Height = 800 ;


    tsWfr ->TabVisible = false; //tsWfr -> PageIndex == _iPage ;
    tsWfo ->TabVisible = false; //tsWfo -> PageIndex == _iPage ;
    tsWfp ->TabVisible = false; //tsWfp -> PageIndex == _iPage ;
    tsCal ->TabVisible = false; //tsCal -> PageIndex == _iPage ;

}
//---------------------------------------------------------------------------


void TFrmAlgoPara::ApplyPara(bool _bToTable)
{




}

void TFrmAlgoPara::FormShow (int  _iPage)
{
    pcAlgPara -> ActivePageIndex = _iPage ;

    Width  = 500 ;
    Height = 800 ;

    tsWfr ->TabVisible = false; //tsWfr -> PageIndex == _iPage ;
    tsWfo ->TabVisible = false; //tsWfo -> PageIndex == _iPage ;
    tsWfp ->TabVisible = false; //tsWfp -> PageIndex == _iPage ;
    tsCal ->TabVisible = false; //tsCal -> PageIndex == _iPage ;

    VISN[_iPage]->ProcPkg.ApplyPara(true);

    VISN[_iPage]->SetViewMode(vmRslt);
    VISN[_iPage]->PaintCallback();

    Show();

    tmUpdate -> Enabled = true ;

}

void TFrmAlgoPara::Init()
{
     VISN[viWfr ]->MakeVisnParaFrm(pnParaWfr);
     VISN[viWfr ]->MoveVisnParaFrm(pnParaWfr);

     VISN[viWfo ]->MakeVisnParaFrm(pnParaWfo);
     VISN[viWfo ]->MoveVisnParaFrm(pnParaWfo);

     VISN[viWfp ]->MakeVisnParaFrm(pnParaWfp);
     VISN[viWfp ]->MoveVisnParaFrm(pnParaWfp);

     VISN[viCal ]->MakeVisnParaFrm(pnParaCal);
     VISN[viCal ]->MoveVisnParaFrm(pnParaCal);
}

void __fastcall TFrmAlgoPara::btTrainClick(TObject *Sender)
{
    EN_VISN_ID iActivVisn = pcAlgPara -> ActivePageIndex ;
    int        iActivInsp = VISN[iActivVisn] ->ParaUi.pcParaBase -> ActivePageIndex ;

    if(VISN[iActivVisn]->ProcPkg.m_pProc[iActivInsp]->GetProcType() != ptOcv ) return ;


    Trace("Operator", "FormAlgoPara Form_Train Button Click");

    VISN[iActivVisn]->ProcPkg.ApplyPara(false);
    VISN[iActivVisn]->LoadJobFile(false );

    //FrmVisnMain -> FrmImg[pcAlgPara -> ActivePageIndex]->OnPaintMsg();
    Trace("Operator", "Start Train");
    bool bRet = VISN[pcAlgPara -> ActivePageIndex]->Train();
    Trace("Operator", "End Train");
    if(iActivVisn == viWfr) {
             if(iActivInsp == iiWfrOcv1)g_bNeedWfrOcv1Train = false ;
        else if(iActivInsp == iiWfrOcv2)g_bNeedWfrOcv2Train = false ;
    }
    if(iActivVisn == viWfo) {
             if(iActivInsp == iiWfrOcv1)g_bNeedWfoOcv1Train = false ;
        else if(iActivInsp == iiWfrOcv2)g_bNeedWfoOcv2Train = false ;
    }

    Trace("Operator", "Start PaintMsg");
    FrmVisnMain -> FrmImg[pcAlgPara -> ActivePageIndex]->OnPaintMsg();
    Trace("Operator", "End PaintMsg");
}
//---------------------------------------------------------------------------


void __fastcall TFrmAlgoPara::btPreClick(TObject *Sender)
{
    if(VISN[pcAlgPara -> ActivePageIndex]->ParaUi.pcParaBase) {
        VISN[pcAlgPara -> ActivePageIndex]->ParaUi.pcParaBase -> ActivePageIndex-- ;
        VISN[pcAlgPara -> ActivePageIndex]->PageControlChange(Sender) ;
    }

    btPre  -> Visible = VISN[pcAlgPara -> ActivePageIndex]->ParaUi.pcParaBase -> ActivePageIndex != 0 ;
    Trace("Operator", "FormAlgoPara Form_Pre Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmAlgoPara::btInspClick(TObject *Sender)
{
    unsigned long lInspTime = GetTickCount() ;

    VISN[pcAlgPara -> ActivePageIndex]->SetViewMode(vmRslt);
    bool bRet = VISN[pcAlgPara -> ActivePageIndex]->Inspect();

    if(!bRet) {
        ShowMessage(SVL_GetErrMsg()) ;
        return ;
    }

    FrmVisnMain -> FrmImg[pcAlgPara -> ActivePageIndex]->OnPaintMsg();


//try{
//Trace("g_csPaint2","EnterStart");
//EnterCriticalSection(&g_csPaint2);
//Trace("g_csPaint2","EnterEnd");

//}
//__finally{
//Trace("g_csPaint2","LeaveStart");
//LeaveCriticalSection(&g_csPaint2);
//Trace("g_csPaint2","LeaveEnd");
//}

    lInspTime = GetTickCount() -  lInspTime ;

    Trace("Operator", "FormAlgoPara Form_Insp Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmAlgoPara::btParaSaveClick(TObject *Sender)
{

    Trace("Operator", "FormAlgoPara Form_Para Save Click");
    EN_VISN_ID iActivVisn = pcAlgPara -> ActivePageIndex ;
    int        iActivInsp = VISN[iActivVisn]->ParaUi.pcParaBase -> ActivePageIndex ;

    if(iActivVisn == viWfr) {
        if(iActivInsp == iiWfrRct )g_bNeedWfrSave     = false ;
        if(iActivInsp == iiWfrOcv1)g_bNeedWfrOcv1Save = false ;
        if(iActivInsp == iiWfrOcv2)g_bNeedWfrOcv2Save = false ;
    }
    else if(iActivVisn == viWfo){
        if(iActivInsp == iiWfoRct )g_bNeedWfoSave     = false ;
        if(iActivInsp == iiWfoOcv1)g_bNeedWfoOcv1Save = false ;
        if(iActivInsp == iiWfoOcv2)g_bNeedWfoOcv2Save = false ;
    }
    else if(iActivVisn == viWfp){
        if(iActivInsp == iiWfpTsd )g_bNeedWfpSave     = false ;
    }
    else if(iActivVisn == viCal){
        if(iActivInsp == iiCalCal )g_bNeedCalSave     = false ;
    }


    if(g_bSettingMode) {
        FrmVisnPara -> ApplyPara(false);
        VOM.LoadVDevOptn(false);
        VOM.LoadVCmnOptn(false);
    }

    VISN[iActivVisn]->ProcPkg.ApplyPara(false);
    VISN[iActivVisn]->LoadJobFile(false );

    FrmVisnMain -> FrmImg[pcAlgPara -> ActivePageIndex]->OnPaintMsg();
}
//---------------------------------------------------------------------------

void __fastcall TFrmAlgoPara::btNextClick(TObject *Sender)
{
    if(VISN[pcAlgPara -> ActivePageIndex]->ParaUi.pcParaBase) {
        VISN[pcAlgPara -> ActivePageIndex]->ParaUi.pcParaBase -> ActivePageIndex++ ;
        VISN[pcAlgPara -> ActivePageIndex]->PageControlChange(Sender) ;
    }
    btNext -> Visible = VISN[pcAlgPara -> ActivePageIndex]->ParaUi.pcParaBase -> ActivePageIndex != VISN[pcAlgPara -> ActivePageIndex]->ParaUi.pcParaBase -> PageCount - 1  ;
    Trace("Operator", "FormAlgoPara Form_Next Button Click");
}
//---------------------------------------------------------------------------


void __fastcall TFrmAlgoPara::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    //for(int i = 0 ; i < MAX_VISN_ID ; i++) {
    //    VISN[i].SetViewMode(vm)
    //
    //
    //
    //}

}
//---------------------------------------------------------------------------

void __fastcall TFrmAlgoPara::FormActivate(TObject *Sender)
{
    VISN[pcAlgPara -> ActivePageIndex]->PageControlChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrmAlgoPara::FormDestroy(TObject *Sender)
{
    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;

}
//---------------------------------------------------------------------------


void __fastcall TFrmAlgoPara::Panel1Click(TObject *Sender)
{

    VISN[pcAlgPara -> ActivePageIndex]->LoadJobFile(true );
    VISN[pcAlgPara -> ActivePageIndex]->ProcPkg.ApplyPara(true);
}
//---------------------------------------------------------------------------

#include "VisionMan.h"
void __fastcall TFrmAlgoPara::btCenterClick(TObject *Sender)
{
    EN_VISN_ID iActivVisn = pcAlgPara -> ActivePageIndex ;
    int        iActivInsp = VISN[iActivVisn]->ParaUi.pcParaBase -> ActivePageIndex ;


    if(VISN[iActivVisn]->ProcPkg.m_pProc[iActivInsp]->GetProcType() != ptWrt &&
       VISN[iActivVisn]->ProcPkg.m_pProc[iActivInsp]->GetProcType() != ptRct ) return ;


    VISN[iActivVisn]->SetViewMode(vmRslt);
    //bool bRet = VISN[iActivVisn]->Inspect();


    RCT_Rslt RctRslt ;
    WRT_Rslt WrtRslt ;

    bool bRet ;
    TRectTracker * Tracker ;
    if(VISN[iActivVisn]->ProcPkg.m_pProc[iActivInsp]->GetProcType() == ptRct) {
        bRet = VISN[iActivVisn]->ProcPkg.Process(iActivInsp , VISN[iActivVisn]->Img , &RctRslt);
        Tracker = ((CRct *)VISN[iActivVisn]->ProcPkg.m_pProc[iActivInsp])->Tracker ;
    }
    else if(VISN[iActivVisn]->ProcPkg.m_pProc[iActivInsp]->GetProcType() == ptWrt) {

        bRet = VISN[iActivVisn]->ProcPkg.Process(iActivInsp , VISN[iActivVisn]->Img , &WrtRslt);
        Tracker = ((CWrt *)VISN[iActivVisn]->ProcPkg.m_pProc[iActivInsp])->Tracker ;
    }
    if(!bRet) {
        ShowMessage(SVL_GetErrMsg()) ;
        return ;
    }

    float fLength1,fLength2 ;
    fLength1 = MATH_GetLineLenth(RctRslt.CrossPntLB.x , RctRslt.CrossPntLB.y , RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y); //좌변.
    fLength2 = MATH_GetLineLenth(RctRslt.CrossPntLT.x , RctRslt.CrossPntLT.y , RctRslt.CrossPntRT.x , RctRslt.CrossPntRT.y); //윗변
    if(iActivVisn == viWfr){
        VOM.VDevOptn.fWfrHeight = fLength1* VISN[viWfr]->Cal->Rslt.fXPxRes ;
        VOM.VDevOptn.fWfrWidth  = fLength2* VISN[viWfr]->Cal->Rslt.fYPxRes ;
        FrmVisnPara -> ApplyPara(true);
        VOM.LoadVDevOptn(false);
    }

    Tracker-> Width = fLength2 ;
    Tracker-> Height= fLength1 ;

    Tracker->Left += RctRslt.CenterPnt.x - Tracker -> GetCenterX() ;
    Tracker->Top  += RctRslt.CenterPnt.y - Tracker -> GetCenterY() ;


    int iRefX = VISN[iActivVisn]->GetCenterX() - Tracker -> GetCenterX() ;
    int iRefY = VISN[iActivVisn]->GetCenterY() - Tracker -> GetCenterY() ;

    Tracker->SetRefX(iRefX);
    Tracker->SetRefY(iRefY);







    FrmVisnMain -> FrmImg[pcAlgPara -> ActivePageIndex]->OnPaintMsg();

    if(iActivVisn == viWfr && iActivInsp == iiWfrRct)g_bNeedWfrCenter = false ;
    if(iActivVisn == viWfo && iActivInsp == iiWfoRct)g_bNeedWfoCenter = false ;



    Trace("Operator", "FormAlgoPara Form_Center Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmAlgoPara::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    EN_VISN_ID iActivVisn = pcAlgPara -> ActivePageIndex ;
    int        iActivInsp = VISN[iActivVisn] ->ParaUi.pcParaBase -> ActivePageIndex ;

    lbMessage -> Visible = !lbMessage -> Visible ;

    if(iActivVisn == viWfr) {
             if(g_bNeedWfrCenter   )lbMessage -> Caption = "칩검사 RECT의 CENTER를 수행 해주세요";
        else if(g_bNeedWfrSave     )lbMessage -> Caption = "칩검사 RECT를 SAVE 해주세요.";
        else if(g_bNeedWfrOcv1Train)lbMessage -> Caption = "칩검사 OCV1번을 TRAIN 해주세요.";
        else if(g_bNeedWfrOcv1Save )lbMessage -> Caption = "칩검사 OCV1번을 SAVE 해주세요.";
        else if(g_bNeedWfrOcv2Train)lbMessage -> Caption = "칩검사 OCV2번을 TRAIN 해주세요.";
        else if(g_bNeedWfrOcv2Save )lbMessage -> Caption = "칩검사 OCV2번을 SAVE 해주세요.";
        else                        lbMessage -> Caption = "";
    }
    else if(iActivVisn == viWfo) {
             if(g_bNeedWfoCenter   )lbMessage -> Caption = "칩기준검사 RECT의 CENTER를 수행 해주세요";
        else if(g_bNeedWfoSave     )lbMessage -> Caption = "칩기준검사 RECT를 SAVE 해주세요.";
        else if(g_bNeedWfoOcv1Train)lbMessage -> Caption = "칩기준검사 OCV1번을 TRAIN 해주세요.";
        else if(g_bNeedWfoOcv1Save )lbMessage -> Caption = "칩기준검사 OCV1번을 SAVE 해주세요.";
        else if(g_bNeedWfoOcv2Train)lbMessage -> Caption = "칩기준검사 OCV2번을 TRAIN 해주세요.";
        else if(g_bNeedWfoOcv2Save )lbMessage -> Caption = "칩기준검사 OCV2번을 SAVE 해주세요.";
        else                        lbMessage -> Caption = "";
    }
    else if(iActivVisn == viWfp) {
             if(g_bNeedWfpSave     )lbMessage -> Caption = "펜마킹검사 TSD을 SAVE 해주세요.";
        else                        lbMessage -> Caption = "";
    }
    else if(iActivVisn == viCal) {
             if(g_bNeedCalSave     )lbMessage -> Caption = "캘리브레이션 Cal을 SAVE 해주세요.";
        else                        lbMessage -> Caption = "";     
    }



    btCenter -> Visible = (VISN[iActivVisn]->ProcPkg.m_pProc[iActivInsp]->GetProcType() == ptWrt || VISN[iActivVisn]->ProcPkg.m_pProc[iActivInsp]->GetProcType() == ptRct) ;
    btTrain  -> Visible =  VISN[iActivVisn]->ProcPkg.m_pProc[iActivInsp]->GetProcType() == ptOcv ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

