//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSubErr.h"

#include "Sequence.h"
#include "DataMan.h"
#include "OptionMan.h"
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
    int r,c ;
    if(DM.ARAY[riSTG].GetCntStat(csCmsLAlign)){ //���� ���� �����.
        if(!STG.FindChip(r , c , csCmsLAlign)){
            return ;
        }
        DM.ARAY[riSTG].SetStat(r , c ,  csCmsRPreAlign);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csCmsRPreAlign)){
        if(!STG.FindChip(r , c , csCmsRPreAlign)){
            return ;
        }
        DM.ARAY[riSTG].SetStat(r,c,csCmsRHeight);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csCmsRHeight)){
        if(!STG.FindChip(r , c , csCmsRHeight )){
            return ;
        }
        DM.ARAY[riSTG].SetStat(r,c,csCmsRAlign);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csCmsRAlign)){
        if(!STG.FindChip(r , c , csCmsRAlign )){
            return ;
        }

        //�̺κ� ���꿡���������� �Ȱ��� ���Ƿ� ������� �ϸ� �ű� ������ ����.
        //������ ��ȭ�ÿ� Ʋ������ �� ������ �����V�� �ִ´�.
        double dTempX = 0 ;
        double dTempY = 0 ;
        if(r==1 && c==0){
            dTempX+=OM.DevOptn.dCmsAtachGapX;//�����V ����.
            dTempX-=OM.DevOptn.dStageInspOfsX;

            dTempX += OM.DevOptn.dBfUVOfsX01 ;
            dTempY += OM.DevOptn.dBfUVOfsY01 ;
        }
        else if(r==0 && c==1){
            dTempY+=OM.DevOptn.dCmsAtachGapY ;
            dTempY-=OM.DevOptn.dStageInspOfsY;

            dTempX += OM.DevOptn.dBfUVOfsX10 ;
            dTempY += OM.DevOptn.dBfUVOfsY10 ;
        }
        else if(r==1 && c==1){ //4��°�� ��� ���ʱ������� ���̱� ������ Y�����V�� ������ �ȵȴ�. 2��4�� ���̰� ������.
            dTempX+=OM.DevOptn.dCmsAtachGapX;//�����V ����.
            dTempX-=OM.DevOptn.dStageInspOfsX;
            dTempX += OM.DevOptn.dBfUVOfsX11 ;
            dTempY += OM.DevOptn.dBfUVOfsY11 ;
        }

        MT_GoIncAcDc(miSTG_XStage , dTempX ,1.0,1.0,1.0);
        MT_GoIncAcDc(miSTG_YStage , dTempY ,1.0,1.0,1.0);





        DM.ARAY[riSTG].SetStat(r,c,csCmsRZHeight);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csCmsREndAlign)){
        if(!STG.FindChip(r , c , csCmsREndAlign)){
            return ;
        }
        DM.ARAY[riSTG].SetStat(r,c,csCmsREndHeight);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csCmsREndHeight)){
        if(!STG.FindChip(r , c , csCmsREndHeight)){
            return ;
        }
        DM.ARAY[riSTG].SetStat(r,c,csWorkEnd);
    }






    SEQ.Reset();
    Close();
}
//---------------------------------------------------------------------------



void __fastcall TFrmSubErr::btVisnSkipClick(TObject *Sender)
{


    SEQ.Reset();
    Close();

}
//---------------------------------------------------------------------------

