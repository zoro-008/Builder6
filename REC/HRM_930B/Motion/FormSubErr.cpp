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
    int r,c ;
    if(DM.ARAY[riSTG].GetCntStat(csCmsLAlign)){ //왼쪽 비젼 얼라인.
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

        //이부분 서브에러폼에서도 똑같이 쓰므로 여기수정 하면 거기 가서도 수정.
        //에폭시 경화시에 틀어지는 것 때문에 오프셑을 넣는다.
        double dTempX = 0 ;
        double dTempY = 0 ;
        if(r==1 && c==0){
            dTempX+=OM.DevOptn.dCmsAtachGapX;//오프셑 조절.
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
        else if(r==1 && c==1){ //4번째의 경우 왼쪽기준으로 붙이기 때문에 Y오프셑을 넣으면 안된다. 2번4번 사이가 벌어짐.
            dTempX+=OM.DevOptn.dCmsAtachGapX;//오프셑 조절.
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

