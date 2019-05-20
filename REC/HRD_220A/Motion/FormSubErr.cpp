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
enum EN_VAC_ERR_STAT { //���� �� �� �ϰ� �ִ� ����.
    vsRetry            = 0 , //�������� ���� �ٽ� ���ϰ� �������� �ٽ� ����.
    vsErrSkipPick      = 1 , //���� �� �� ��ŵ.
    vsErrLotEnd        = 2 , //������.
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
        if(DM.ARAY[riSTG].CheckAllStat(csWorkEnd)||DM.ARAY[riSTG].CheckAllStat(csVisnAt)){ //���������� ������ ������� ������ ������ ����ŷ �ؾ���.
            //DM.ARAY[riSTG].SetStat(csSkip );
            //�ϴ� ���ΰ͵��� ������ ���� �˻� ���ϰ� ����Ʈ ���ܾ� �Ѵ�.
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


        if(!DM.ARAY[riPCK].CheckAllStat(csNone)){ //Picker�� ������ ������� ��Ʈ�� �ٽ� �ְ� ������ ����ŷ �ؾ���.
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

