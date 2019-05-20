//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormAlgoPara.h"
#include "FormVisnMain.h"
#include "Vision.h"
#include "SLogUnit.h"
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
}
//---------------------------------------------------------------------------


void TFrmAlgoPara::ApplyPara(bool _bToTable)
{




}

void TFrmAlgoPara::FormShow (int  _iPage)
{
    pcAlgPara -> ActivePageIndex = _iPage ;
    tsOri ->TabVisible = tsOri -> PageIndex == _iPage ;
    tsWfr ->TabVisible = tsWfr -> PageIndex == _iPage ;
    tsRlb ->TabVisible = tsRlb -> PageIndex == _iPage ;
    tsRla ->TabVisible = tsRla -> PageIndex == _iPage ;
    tsWfo ->TabVisible = tsWfo -> PageIndex == _iPage ;

    VISN[_iPage]->ProcPkg.ApplyPara(true);

    VISN[_iPage]->SetViewMode(vmRslt);
    VISN[_iPage]->PaintCallback();

    Show();

}
void __fastcall TFrmAlgoPara::FormShow(TObject *Sender)
{
    static bool bFirst = true ;

    if(bFirst) {

        bFirst = false ;
    }


   // for(int i = 0 ; i <

}
//---------------------------------------------------------------------------

void TFrmAlgoPara::Init()
{
     VISN[viOri]->MakeVisnParaFrm(pnParaOri  );
     VISN[viOri]->MoveVisnParaFrm(pnParaOri  );

     VISN[viWfr]->MakeVisnParaFrm(pnParaWfr  );
     VISN[viWfr]->MoveVisnParaFrm(pnParaWfr  );

     VISN[viRbf]->MakeVisnParaFrm(pnParaRalBf);
     VISN[viRbf]->MoveVisnParaFrm(pnParaRalBf);

     VISN[viRat]->MakeVisnParaFrm(pnParaRalAt);
     VISN[viRat]->MoveVisnParaFrm(pnParaRalAt);

     VISN[viWfo]->MakeVisnParaFrm(pnParaWfrOr);
     VISN[viWfo]->MoveVisnParaFrm(pnParaWfrOr);
}

#include "FormTrainChk.h"
void __fastcall TFrmAlgoPara::btTrainClick(TObject *Sender)
{
    Trace("Operator", "FormAlgoPara Form_Train Button Click");

    bool bRet = VISN[pcAlgPara -> ActivePageIndex]->Train();

    FrmVisnMain -> FrmImg[pcAlgPara -> ActivePageIndex]->OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmAlgoPara::btCloseClick(TObject *Sender)
{
    Trace("Operator", "FormAlgoPara Form_Close Button Click");
    Close();
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

void __fastcall TFrmAlgoPara::Button5Click(TObject *Sender)
{
    unsigned long lInspTime = GetTickCount() ;

    VISN[pcAlgPara -> ActivePageIndex]->SetViewMode(vmRslt);
    bool bRet = VISN[pcAlgPara -> ActivePageIndex]->Inspect();

    if(!bRet) {
        ShowMessage(SVL_GetErrMsg()) ;
        return ;
    }

    FrmVisnMain -> FrmImg[pcAlgPara -> ActivePageIndex]->OnPaint();


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

    Trace("Operator", "FormAlgoPara Form_Check Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmAlgoPara::btParaSaveClick(TObject *Sender)
{

    Trace("Operator", "FormAlgoPara Form_Para Save Click");
    VISN[pcAlgPara -> ActivePageIndex]->ProcPkg.ApplyPara(false);
    VISN[pcAlgPara -> ActivePageIndex]->LoadJobFile(false );

//try{
//Trace("g_csPaint2","EnterStart");
//EnterCriticalSection(&g_csPaint2);
//Trace("g_csPaint2","EnterEnd");
    FrmVisnMain -> FrmImg[pcAlgPara -> ActivePageIndex]->OnPaint();
//}
//__finally{
//Trace("g_csPaint2","LeaveStart");
//LeaveCriticalSection(&g_csPaint2);
//Trace("g_csPaint2","LeaveEnd");
//}
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

void __fastcall TFrmAlgoPara::Timer1Timer(TObject *Sender)
{

    //VISN[pcAlgPara -> ActivePageIndex]->ProcPkg.GetProcType()
    //bool bRet = VISN[pcAlgPara -> ActivePageIndex]->Train();


/*
        if(m_iPrePage != VISN[m_iVisnId].ParaUi.pcParaBase -> ActivePageIndex ) {
            m_iPrePage = VISN[m_iVisnId].ParaUi.pcParaBase -> ActivePageIndex ;

        }

        VISN[m_iVisnId].ParaUi.pcParaBase -> ActivePageIndex != 0 ;




        if(VISN[m_iVisnId].ParaUi.pcParaBase -> ActivePageIndex == 0) { //ī�޶� ��.

        }
        else if(VISN[m_iVisnId].ParaUi.pcParaBase -> ActivePageIndex > 0) {
            btTrain -> Visible = ptOcv ==VISN[m_iVisnId].ProcPkg.GetProcType(VISN[m_iVisnId].ParaUi.pcParaBase -> ActivePageIndex - 1 ) ;
        }
*/


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

