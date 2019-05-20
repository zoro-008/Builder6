//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSubErr.h"

#include "Sequence.h"
#include "DataMan.h"
#include "Picker.h"
#include "Index.h"
#include "Stage.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

/*
enum EN_VAC_ERR_STAT { //현재 랏 런 하고 있는 상태.
    vsRetry            = 0 , //에러났던 곳을 다시 픽하고 안찝히면 다시 에러.
    vsErrSkipPick      = 1 , //에러 난 곳 스킵.
    vsErrLotEnd        = 2 , //랏엔드.
};
*/

TFrmSubErr *FrmSubErr;
//---------------------------------------------------------------------------
__fastcall TFrmSubErr::TFrmSubErr(TComponent* Owner)
    : TForm(Owner)
{


}
//---------------------------------------------------------------------------

void __fastcall TFrmSubErr::btRetryClick(TObject *Sender)
{
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmSubErr::btVacSkipClick(TObject *Sender)
{
    if(this -> Tag == etPCKVac){
        int        iWorkingCol ;
        EN_ARAY_ID riWork  ;
        IDX_F.FindChip(iWorkingCol ,riWork);
        DM.ARAY[riIDX_F].SetStat(0,iWorkingCol,csSkip);
        Trace("VacSkip","Picker");
    }
    else if(this -> Tag == etSTGVac){
        int        iWorkingCol ;
        EN_ARAY_ID riWork  ;
        IDX_R.FindChip(iWorkingCol ,riWork);
        DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip);
        Trace("VacSkip","Stage");

    }

    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------


void __fastcall TFrmSubErr::btLotEndClick(TObject *Sender)
{
    if(this -> Tag == etPCKVac){
        if(DM.ARAY[riSTG].CheckAllStat(csWorkEnd)||DM.ARAY[riSTG].CheckAllStat(csVisnAt)){ //스테이지에 자제가 있을경우 내리고 랏엔드 마스킹 해야함.
            //DM.ARAY[riSTG].SetStat(csSkip );
            //일단 붙인것들은 랏엔드 전에 검사 다하고 레포트 남겨야 한다.
            STG.m_bNeedLotEnd = true ;
        }
        else if(!DM.ARAY[riSTG].CheckAllStat(csNone)){
            DM.ARAY[riSTG].SetStat(csSkip );
            STG.m_bNeedLotEnd = true ;

        }
        else {
            if(!DM.ARAY[riPRI_R].CheckAllStat(csNone))DM.ARAY[riPRI_R].SetStat(csWork );
            if(!DM.ARAY[riIDX_R].CheckAllStat(csNone))DM.ARAY[riIDX_R].SetStat(csWork );
        }
        if(!DM.ARAY[riPRI_F].CheckAllStat(csNone))DM.ARAY[riPRI_F].SetStat(csEmpty);
        if(!DM.ARAY[riIDX_F].CheckAllStat(csNone))DM.ARAY[riIDX_F].SetStat(csEmpty);
        Trace("LotEnd","Picker");
    }
    else if(this -> Tag == etSTGVac){
        if(!DM.ARAY[riPRI_R].CheckAllStat(csNone))DM.ARAY[riPRI_R].SetStat(csSkip);
        if(!DM.ARAY[riIDX_R].CheckAllStat(csNone))DM.ARAY[riIDX_R].SetStat(csSkip);


        if(!DM.ARAY[riPCK].CheckAllStat(csNone)){ //Picker에 자제가 있을경우 보트에 다시 넣고 랏엔드 마스킹 해야함.
            DM.ARAY[riPCK].SetStat(csSkip );
            PCK.m_bNeedLotEnd = true ;
        }
        if(!DM.ARAY[riALN].CheckAllStat(csNone)){
            DM.ARAY[riALN].SetStat(csSkip );
            PCK.m_bNeedLotEnd = true ;
        }
        Trace("LotEnd","Stage");

    }


        //int        iLastWorkCol ;
        //EN_ARAY_ID riWork  ;
        //IDX_R.FindChip(iLastWorkCol ,riWork);
        //DM.ARAY[riIDX_R].SetStat(0,iLastWorkCol-1,csSkip);

    //if(!DM.ARAY[riPRI_R].CheckAllStat(csNone))DM.ARAY[riPRI_R].SetStat(csWork );
    //if(!DM.ARAY[riIDX_R].CheckAllStat(csNone))DM.ARAY[riIDX_R].SetStat(csWork );
    //if(!DM.ARAY[riPRI_F].CheckAllStat(csNone))DM.ARAY[riPRI_F].SetStat(csEmpty);
    //if(!DM.ARAY[riIDX_F].CheckAllStat(csNone))DM.ARAY[riIDX_F].SetStat(csEmpty);
    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmSubErr::btVisnSkipClick(TObject *Sender)
{
    if(this -> Tag == etPCKVsn){
        DM.ARAY[riPCK].SetStat(csSkip);
        Trace("VisnSkip","Picker");
    }
    else if(this -> Tag == etDSPVsnBf){
        DM.ARAY[riSTG].SetStat(csSkip);
        Trace("VisnSkip","Picker");

    }
    else if(this -> Tag == etDSPVsnAt){
        DM.ARAY[riSTG].SetStat(csSkip);
        Trace("VisnSkip","Picker");
    }


    SEQ.Reset();
    Close();

}
//---------------------------------------------------------------------------

