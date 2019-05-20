//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormRslt.h"

#include "OptionMan.h"
#include "DataMan.h"
#include "Sequence.h"

#include "FormImage.h"
#include "FormGraph.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmRslt *FrmRslt[MAX_STAGE_CNT]={NULL,NULL,NULL,NULL,NULL};
TStringGrid *g_pLastSelectedStringGrid = NULL ;
//---------------------------------------------------------------------------
__fastcall TFrmRslt::TFrmRslt(TComponent* Owner)
        : TForm(Owner)
{
    sgTiltRslt -> ColWidths[0] = 20 ;  sgTiltRslt -> Cells[0][0] = "No"      ;
    sgTiltRslt -> ColWidths[1] = 80 ;  sgTiltRslt -> Cells[1][0] = "Item"    ;
    sgTiltRslt -> ColWidths[2] = 50 ;  sgTiltRslt -> Cells[2][0] = "Measure" ;
    sgTiltRslt -> ColWidths[3] = 90 ;  sgTiltRslt -> Cells[3][0] = "Spec"    ;
    sgTiltRslt -> ColWidths[4] = 70 ;  sgTiltRslt -> Cells[4][0] = "Result"  ;

    sgDriveRslt -> ColWidths[0] = 20 ;  sgDriveRslt -> Cells[0][0] = "No"      ;
    sgDriveRslt -> ColWidths[1] = 80 ;  sgDriveRslt -> Cells[1][0] = "Item"    ;
    sgDriveRslt -> ColWidths[2] = 50 ;  sgDriveRslt -> Cells[2][0] = "Measure" ;
    sgDriveRslt -> ColWidths[3] = 90 ;  sgDriveRslt -> Cells[3][0] = "Spec"    ;
    sgDriveRslt -> ColWidths[4] = 70 ;  sgDriveRslt -> Cells[4][0] = "Result"  ;

    SetStringGrid();

    m_iPreWorkingStg = -1;

}

void TFrmRslt::SetStringGrid()
{
    int iRowCnt ;
    String sTemp ;

    //검사 하는 것들만 스트링 그리드 표시.
    iRowCnt = 1 ;
    if(OM.DevOptn.bUseTiltInitR   )iRowCnt++;
    if(OM.DevOptn.bUseTiltInitT   )iRowCnt++;
    if(OM.DevOptn.bUseTiltInit    )iRowCnt++;
    if(OM.DevOptn.bUseTiltDrive   )iRowCnt++;
    if(OM.DevOptn.bUseTiltDriveAbs)iRowCnt++;
    if(OM.DevOptn.bUseTiltFull    )iRowCnt++;
    if(OM.DevOptn.bUseTiltMax     )iRowCnt++;
    if(OM.DevOptn.bUseTiltLength  )iRowCnt++;

    sgTiltRslt -> RowCount = iRowCnt ;
    for(int r = 1 ; r < sgTiltRslt -> RowCount ; r++) {
        sgTiltRslt -> Cells[0][r] = r  ;
    }
    iRowCnt = 1 ;
    if(OM.DevOptn.bUseTiltInitR    ) sgTiltRslt -> Cells[1][iRowCnt++] = "Init Tilt X Abs";
    if(OM.DevOptn.bUseTiltInitT    ) sgTiltRslt -> Cells[1][iRowCnt++] = "Init Tilt Y Abs";
    if(OM.DevOptn.bUseTiltInit     ) sgTiltRslt -> Cells[1][iRowCnt++] = "Init Tilt Abs"  ;
    if(OM.DevOptn.bUseTiltDrive    ) sgTiltRslt -> Cells[1][iRowCnt++] = "Drive Tilt"     ;
    if(OM.DevOptn.bUseTiltDriveAbs ) sgTiltRslt -> Cells[1][iRowCnt++] = "Total Tilt Abs" ;//"Drive Tilt Abs" ;
    if(OM.DevOptn.bUseTiltFull     ) sgTiltRslt -> Cells[1][iRowCnt++] = "Full Tilt"      ;
    if(OM.DevOptn.bUseTiltMax      ) sgTiltRslt -> Cells[1][iRowCnt++] = "Max Tilt"       ;
    if(OM.DevOptn.bUseTiltLength   ) sgTiltRslt -> Cells[1][iRowCnt++] = "Length"         ;

    //스펙
    iRowCnt = 1 ;
    if(OM.DevOptn.bUseTiltInitR    ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltInitR   ,OM.DevOptn.dMaxTiltInitR   ); sgTiltRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseTiltInitT    ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltInitT   ,OM.DevOptn.dMaxTiltInitT   ); sgTiltRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseTiltInit     ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltInit    ,OM.DevOptn.dMaxTiltInit    ); sgTiltRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseTiltDrive    ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltDrive   ,OM.DevOptn.dMaxTiltDrive   ); sgTiltRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseTiltDriveAbs ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltDriveAbs,OM.DevOptn.dMaxTiltDriveAbs); sgTiltRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseTiltFull     ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltFull    ,OM.DevOptn.dMaxTiltFull    ); sgTiltRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseTiltMax      ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltMax     ,OM.DevOptn.dMaxTiltMax     ); sgTiltRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseTiltLength   ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinTiltLength  ,OM.DevOptn.dMaxTiltLength  ); sgTiltRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}


    iRowCnt = 1 ;
    if(OM.DevOptn.bUsePreLoad     )iRowCnt++;
    if(OM.DevOptn.bUsePeakStroke  )iRowCnt++;
    if(OM.DevOptn.bUseCustomStroke)iRowCnt++;
    if(OM.DevOptn.bUseSensitivity )iRowCnt++;
    if(OM.DevOptn.bUseLinearity   )iRowCnt++;
    if(OM.DevOptn.bUseImpedance   )iRowCnt++;
    if(OM.DevOptn.bUseHysteresis  )iRowCnt++;
    if(OM.DevOptn.bUseCaseShort   )iRowCnt++;
    sgDriveRslt -> RowCount = iRowCnt ;
    for(int r = 1 ; r < sgDriveRslt -> RowCount ; r++) {
        sgDriveRslt -> Cells[0][r] = r  ;
    }
    iRowCnt = 1 ;
    if(OM.DevOptn.bUsePreLoad     ) sgDriveRslt -> Cells[1][iRowCnt++] = "StartingCurrent";
    if(OM.DevOptn.bUsePeakStroke  ) sgDriveRslt -> Cells[1][iRowCnt++] = "MaxStroke"      ;
    if(OM.DevOptn.bUseCustomStroke) sgDriveRslt -> Cells[1][iRowCnt++] = "RatedStroke"    ;
    if(OM.DevOptn.bUseSensitivity ) sgDriveRslt -> Cells[1][iRowCnt++] = "Sensitivity"    ;
    if(OM.DevOptn.bUseLinearity   ) sgDriveRslt -> Cells[1][iRowCnt++] = "Linearity"      ;
    if(OM.DevOptn.bUseImpedance   ) sgDriveRslt -> Cells[1][iRowCnt++] = "Resistance"     ;
    if(OM.DevOptn.bUseHysteresis  ) sgDriveRslt -> Cells[1][iRowCnt++] = "Hysteresis"     ;
    if(OM.DevOptn.bUseCaseShort   ) sgDriveRslt -> Cells[1][iRowCnt++] = "Case Short"     ;


    //스펙
    iRowCnt = 1 ;
    if(OM.DevOptn.bUsePreLoad     ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinPreLoad      ,OM.DevOptn.dMaxPreLoad     ); sgDriveRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUsePeakStroke  ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinPeakStroke   ,OM.DevOptn.dMaxPeakStroke  ); sgDriveRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseCustomStroke){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinCustomStroke ,OM.DevOptn.dMaxCustomStroke); sgDriveRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseSensitivity ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinSensitivity  ,OM.DevOptn.dMaxSensitivity ); sgDriveRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseLinearity   ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinLinearity    ,OM.DevOptn.dMaxLinearity   ); sgDriveRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseImpedance   ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinImpedance    ,OM.DevOptn.dMaxImpedance   ); sgDriveRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseHysteresis  ){sTemp.sprintf("%.1f~%.1f",OM.DevOptn.dMinHysteresis   ,OM.DevOptn.dMaxHysteresis  ); sgDriveRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}
    if(OM.DevOptn.bUseCaseShort   ){sTemp= "";                                                                           sgDriveRslt -> Cells[3][iRowCnt++] = sTemp.c_str() ;}


}

void TFrmRslt::SetStgId(int _iStgId)
{
    m_iStgId = _iStgId ;


    if(FrmImage[m_iStgId] == NULL) {
        FrmImage[m_iStgId] = new TFrmImage(pnVisn) ;
        FrmImage[m_iStgId] -> Parent = pnVisn ;

        FrmImage[m_iStgId] -> Show() ;
    }

    if(FrmGraph[m_iStgId] == NULL) {
        FrmGraph[m_iStgId] = new TFrmGraph(pnVisn) ;
        FrmGraph[m_iStgId] -> Parent = pnGraph ;
        FrmGraph[m_iStgId] -> Show() ;
    }


    FrmImage[m_iStgId] -> SetStgId(m_iStgId);
    FrmGraph[m_iStgId] -> SetStgId(m_iStgId);




}

//---------------------------------------------------------------------------

double GetVal(double _dVal )
{
    int iRet = _dVal * 10 ;

    double dRet = iRet / 10.0 ;

    return dRet ;

}



void __fastcall TFrmRslt::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    //범위값 변경.
    if(OM.m_bSaved[m_iStgId]) {
        OM.m_bSaved[m_iStgId] = false ;  //하.. 짜증나...5개라 하나 가지고 하면 안되네...
        if(FrmImage[m_iStgId] !=NULL){
            if(OM.DevOptn.bUseDuplex) {
                FrmGraph[m_iStgId] -> Para.m_dLeftX    = OM.DevOptn.iTestCrntMin ;
                //FrmGraph[m_iStgId] -> Para.m_dBottomY  = -300                    ;

            }
            else {
                FrmGraph[m_iStgId] -> Para.m_dLeftX    = 0                       ;
                //FrmGraph[m_iStgId] -> Para.m_dBottomY  = 0                       ;
            }
            FrmGraph[m_iStgId] -> Para.m_dBottomY  = OM.DevOptn.iGraphMinY   ;
            FrmGraph[m_iStgId] -> Para.m_dRightX   = OM.DevOptn.iTestCrntMax ;
            FrmGraph[m_iStgId] -> Para.m_dTopY     = OM.DevOptn.iGraphMaxY   ;
            FrmGraph[m_iStgId] -> Para.m_dStepX    = (int)(FrmGraph[m_iStgId] -> Para.m_dRightX - FrmGraph[m_iStgId] -> Para.m_dLeftX  ) / 8 ;
            FrmGraph[m_iStgId] -> Para.m_dStepY    = (int)(FrmGraph[m_iStgId] -> Para.m_dTopY   - FrmGraph[m_iStgId] -> Para.m_dBottomY) / 15;
            FrmGraph[m_iStgId] -> Para.m_dBigStepY = (int)(FrmGraph[m_iStgId] -> Para.m_dTopY   - FrmGraph[m_iStgId] -> Para.m_dBottomY) / 6 ;

            FrmGraph[m_iStgId] -> UpdateView();
        }
    }


    //sgDriveRslt -> ColWidths[0] = 20 ;  _sgTable -> Cells[0][0] = "No"      ;
    //sgDriveRslt -> ColWidths[1] = 80 ;  _sgTable -> Cells[1][0] = "Item"    ;
    //sgDriveRslt -> ColWidths[2] = 50 ;  _sgTable -> Cells[2][0] = "Measure" ;
    //sgDriveRslt -> ColWidths[3] = 90 ;  _sgTable -> Cells[3][0] = "Spec"    ;
    //sgDriveRslt -> ColWidths[4] = 80 ;  _sgTable -> Cells[4][0] = "Result"  ;
    SetStringGrid();

    //int iWorkingStg = DM.ARAY[riSTG].FindFrstCol(csUnkwn);
    int iWorkingStg = DM.ARAY[riSTG].GetCntStat(csUnkwn);

    //검사 모두 끝나면 한번만 디스플레이하게.
    if(iWorkingStg != m_iPreWorkingStg ) {

        String sTemp ;
        int iRowCnt ;
        String sFailItem = "";


        //==============================================Tilt=============================
        double dTiltInitR    = g_TiltData[m_iStgId].dTiltInitR    * OM.MstOptn.d2GainTiltInitR    + OM.MstOptn.d2OfstTiltInitR   ;
        double dTiltInitT    = g_TiltData[m_iStgId].dTiltInitT    * OM.MstOptn.d2GainTiltInitT    + OM.MstOptn.d2OfstTiltInitT   ;
        double dTiltInit     = g_TiltData[m_iStgId].dTiltInit     * OM.MstOptn.d2GainTiltInit     + OM.MstOptn.d2OfstTiltInit    ;
        double dTiltDrive    = g_TiltData[m_iStgId].dTiltDrive    * OM.MstOptn.d2GainTiltDrive    + OM.MstOptn.d2OfstTiltDrive   ;
        double dTiltDriveAbs = g_TiltData[m_iStgId].dTiltDriveAbs * OM.MstOptn.d2GainTiltDriveAbs + OM.MstOptn.d2OfstTiltDriveAbs;
        double dTiltFull     = g_TiltData[m_iStgId].dTiltFull     * OM.MstOptn.d2GainTiltFull     + OM.MstOptn.d2OfstTiltFull    ;
        double dTiltMax      = g_TiltData[m_iStgId].dTiltMax      * OM.MstOptn.d2GainTiltMax      + OM.MstOptn.d2OfstTiltMax     ;
        double dTiltLength   = g_TiltData[m_iStgId].dTiltLength   * OM.MstOptn.d2GainTiltLength   + OM.MstOptn.d2OfstTiltLength  ;

        dTiltInitR    = int(dTiltInitR    * 10)/10.0 ;
        dTiltInitT    = int(dTiltInitT    * 10)/10.0 ;
        dTiltInit     = int(dTiltInit     * 10)/10.0 ;
        dTiltDrive    = int(dTiltDrive    * 10)/10.0 ;
        dTiltDriveAbs = int(dTiltDriveAbs * 10)/10.0 ;
        dTiltFull     = int(dTiltFull     * 10)/10.0 ;
        dTiltMax      = int(dTiltMax      * 10)/10.0 ;
        dTiltLength   = int(dTiltLength   * 10)/10.0 ;

        //결과값.
        iRowCnt = 1 ;
        if(OM.DevOptn.bUseTiltInitR    ){sgTiltRslt -> Cells[2][iRowCnt++] = dTiltInitR    != 999.0 ? String(dTiltInitR   ) : String("FAIL");}
        if(OM.DevOptn.bUseTiltInitT    ){sgTiltRslt -> Cells[2][iRowCnt++] = dTiltInitT    != 999.0 ? String(dTiltInitT   ) : String("FAIL");} 
        if(OM.DevOptn.bUseTiltInit     ){sgTiltRslt -> Cells[2][iRowCnt++] = dTiltInit     != 999.0 ? String(dTiltInit    ) : String("FAIL");} 
        if(OM.DevOptn.bUseTiltDrive    ){sgTiltRslt -> Cells[2][iRowCnt++] = dTiltDrive    != 999.0 ? String(dTiltDrive   ) : String("FAIL");} 
        if(OM.DevOptn.bUseTiltDriveAbs ){sgTiltRslt -> Cells[2][iRowCnt++] = dTiltDriveAbs != 999.0 ? String(dTiltDriveAbs) : String("FAIL");} 
        if(OM.DevOptn.bUseTiltFull     ){sgTiltRslt -> Cells[2][iRowCnt++] = dTiltFull     != 999.0 ? String(dTiltFull    ) : String("FAIL");} 
        if(OM.DevOptn.bUseTiltMax      ){sgTiltRslt -> Cells[2][iRowCnt++] = dTiltMax      != 999.0 ? String(dTiltMax     ) : String("FAIL");}
        if(OM.DevOptn.bUseTiltLength   ){sgTiltRslt -> Cells[2][iRowCnt++] = dTiltLength   != 999.0 ? String(dTiltLength  ) : String("FAIL");}


        bool   bTemp ;
        double dTemp ;
        //양부 판정.
        iRowCnt = 1 ;
        if(OM.DevOptn.bUseTiltInitR    ){bTemp = (OM.DevOptn.dMinTiltInitR    <= dTiltInitR    && dTiltInitR    <=OM.DevOptn.dMaxTiltInitR    )  ; sgTiltRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ; if(!bTemp)sFailItem = "InitTiltXAbs";}
        if(OM.DevOptn.bUseTiltInitT    ){bTemp = (OM.DevOptn.dMinTiltInitT    <= dTiltInitT    && dTiltInitT    <=OM.DevOptn.dMaxTiltInitT    )  ; sgTiltRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ; if(!bTemp)sFailItem = "InitTiltYAbs";}
        if(OM.DevOptn.bUseTiltInit     ){bTemp = (OM.DevOptn.dMinTiltInit     <= dTiltInit     && dTiltInit     <=OM.DevOptn.dMaxTiltInit     )  ; sgTiltRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ; if(!bTemp)sFailItem = "InitTiltAbs ";}
        if(OM.DevOptn.bUseTiltDrive    ){bTemp = (OM.DevOptn.dMinTiltDrive    <= dTiltDrive    && dTiltDrive    <=OM.DevOptn.dMaxTiltDrive    )  ; sgTiltRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ; if(!bTemp)sFailItem = "DriveTilt   ";}
        if(OM.DevOptn.bUseTiltDriveAbs ){bTemp = (OM.DevOptn.dMinTiltDriveAbs <= dTiltDriveAbs && dTiltDriveAbs <=OM.DevOptn.dMaxTiltDriveAbs )  ; sgTiltRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ; if(!bTemp)sFailItem = "TotalTiltAbs";}
        if(OM.DevOptn.bUseTiltFull     ){bTemp = (OM.DevOptn.dMinTiltFull     <= dTiltFull     && dTiltFull     <=OM.DevOptn.dMaxTiltFull     )  ; sgTiltRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ; if(!bTemp)sFailItem = "FullTilt    ";}
        if(OM.DevOptn.bUseTiltMax      ){bTemp = (OM.DevOptn.dMinTiltMax      <= dTiltMax      && dTiltMax      <=OM.DevOptn.dMaxTiltMax      )  ; sgTiltRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ; if(!bTemp)sFailItem = "MaxTilt     ";}
        if(OM.DevOptn.bUseTiltLength   ){bTemp = (OM.DevOptn.dMinTiltLength   <= dTiltLength   && dTiltLength   <=OM.DevOptn.dMaxTiltLength   )  ; sgTiltRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ; if(!bTemp)sFailItem = "Length      ";}

        iRowCnt = 1 ;
        if(OM.DevOptn.bUseTiltInitR   &&!g_bInspEnd[m_iStgId])sgTiltRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseTiltInitT   &&!g_bInspEnd[m_iStgId])sgTiltRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseTiltInit    &&!g_bInspEnd[m_iStgId])sgTiltRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseTiltDrive   &&!g_bInspEnd[m_iStgId])sgTiltRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseTiltDriveAbs&&!g_bInspEnd[m_iStgId])sgTiltRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseTiltFull    &&!g_bInspEnd[m_iStgId])sgTiltRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseTiltMax     &&!g_bInspEnd[m_iStgId])sgTiltRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseTiltLength  &&!g_bInspEnd[m_iStgId])sgTiltRslt -> Cells[4][iRowCnt++] = "" ;

    //==============================================================================


    //==============================================Drive=============================
        //20150616 드라이브 데이터에 잡파일별로 게인 오프셑 넣기... 요청사항.
        //20150825 코헬향 숨김 게인 오프셑 추가 요청.
        double dPreLoad      = g_DriveData[m_iStgId].dPreLoad      * OM.MstOptn.d2GainPreLoad      + OM.MstOptn.d2OfstPreLoad      ;
        double dPeakStroke   = g_DriveData[m_iStgId].dPeakStroke   * OM.MstOptn.d2GainPeakStroke   + OM.MstOptn.d2OfstPeakStroke   ;
        double dCustomStroke = g_DriveData[m_iStgId].dCustomStroke * OM.MstOptn.d2GainCustomStroke + OM.MstOptn.d2OfstCustomStroke ;
        double dSensitivity  = g_DriveData[m_iStgId].dSensitivity  * OM.MstOptn.d2GainSensitivity  + OM.MstOptn.d2OfstSensitivity  ;
        double dLinearity    = g_DriveData[m_iStgId].dLinearity    * OM.MstOptn.d2GainLinearity    + OM.MstOptn.d2OfstLinearity    ;
        double dImpedance    = g_DriveData[m_iStgId].dImpedance    * OM.MstOptn.d2GainImpedance    + OM.MstOptn.d2OfstImpedance    ;
        double dHysteresis   = g_DriveData[m_iStgId].dHysteresis   * OM.MstOptn.d2GainHysteresis   + OM.MstOptn.d2OfstHysteresis   ;
        bool   bCaseShortOk  =!g_DriveData[m_iStgId].bCaseShort ;

        dPreLoad      = g_DriveData[m_iStgId].dPreLoad      * OM.MstDevOptn.dGainPreLoad      + OM.MstDevOptn.dOfstPreLoad      ;
        dPeakStroke   = g_DriveData[m_iStgId].dPeakStroke   * OM.MstDevOptn.dGainPeakStroke   + OM.MstDevOptn.dOfstPeakStroke   ;
        dCustomStroke = g_DriveData[m_iStgId].dCustomStroke * OM.MstDevOptn.dGainCustomStroke + OM.MstDevOptn.dOfstCustomStroke ;
        dSensitivity  = g_DriveData[m_iStgId].dSensitivity  * OM.MstDevOptn.dGainSensitivity  + OM.MstDevOptn.dOfstSensitivity  ;
        dLinearity    = g_DriveData[m_iStgId].dLinearity    * OM.MstDevOptn.dGainLinearity    + OM.MstDevOptn.dOfstLinearity    ;
        dImpedance    = g_DriveData[m_iStgId].dImpedance    * OM.MstDevOptn.dGainImpedance    + OM.MstDevOptn.dOfstImpedance    ;
        dHysteresis   = g_DriveData[m_iStgId].dHysteresis   * OM.MstDevOptn.dGainHysteresis   + OM.MstDevOptn.dOfstHysteresis   ;









        dPreLoad      = int(dPreLoad      * 10)/10.0 ;
        dPeakStroke   = int(dPeakStroke   * 10)/10.0 ;
        dCustomStroke = int(dCustomStroke * 10)/10.0 ;
        dSensitivity  = int(dSensitivity  * 10)/10.0 ;
        dLinearity    = int(dLinearity    * 10)/10.0 ;
        dImpedance    = int(dImpedance    * 10)/10.0 ;
        dHysteresis   = int(dHysteresis   * 10)/10.0 ;


        iRowCnt = 1 ;
        if(OM.DevOptn.bUsePreLoad     ){sgDriveRslt -> Cells[2][iRowCnt++] = dPreLoad      ;}
        if(OM.DevOptn.bUsePeakStroke  ){sgDriveRslt -> Cells[2][iRowCnt++] = dPeakStroke   ;}
        if(OM.DevOptn.bUseCustomStroke){sgDriveRslt -> Cells[2][iRowCnt++] = dCustomStroke ;}
        if(OM.DevOptn.bUseSensitivity ){sgDriveRslt -> Cells[2][iRowCnt++] = dSensitivity  ;}
        if(OM.DevOptn.bUseLinearity   ){sgDriveRslt -> Cells[2][iRowCnt++] = dLinearity    ;}
        if(OM.DevOptn.bUseImpedance   ){sgDriveRslt -> Cells[2][iRowCnt++] = dImpedance    ;}
        if(OM.DevOptn.bUseHysteresis  ){sgDriveRslt -> Cells[2][iRowCnt++] = dHysteresis   ;}
        if(OM.DevOptn.bUseCaseShort   ){sgDriveRslt -> Cells[2][iRowCnt++] = bCaseShortOk ? "OK" : "NG" ;}

        //양부 판정.
        iRowCnt = 1 ;
        if(OM.DevOptn.bUsePreLoad     ){ bTemp = (OM.DevOptn.dMinPreLoad      <= dPreLoad      && dPreLoad      <=OM.DevOptn.dMaxPreLoad     ) ; sgDriveRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ;if(!bTemp)sFailItem = "StartingCurrent";}
        if(OM.DevOptn.bUsePeakStroke  ){ bTemp = (OM.DevOptn.dMinPeakStroke   <= dPeakStroke   && dPeakStroke   <=OM.DevOptn.dMaxPeakStroke  ) ; sgDriveRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ;if(!bTemp)sFailItem = "MaxStroke      ";}
        if(OM.DevOptn.bUseCustomStroke){ bTemp = (OM.DevOptn.dMinCustomStroke <= dCustomStroke && dCustomStroke <=OM.DevOptn.dMaxCustomStroke) ; sgDriveRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ;if(!bTemp)sFailItem = "RatedStroke    ";}
        if(OM.DevOptn.bUseSensitivity ){ bTemp = (OM.DevOptn.dMinSensitivity  <= dSensitivity  && dSensitivity  <=OM.DevOptn.dMaxSensitivity ) ; sgDriveRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ;if(!bTemp)sFailItem = "Sensitivity    ";}
        if(OM.DevOptn.bUseLinearity   ){ bTemp = (OM.DevOptn.dMinLinearity    <= dLinearity    && dLinearity    <=OM.DevOptn.dMaxLinearity   ) ; sgDriveRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ;if(!bTemp)sFailItem = "Linearity      ";}
        if(OM.DevOptn.bUseImpedance   ){ bTemp = (OM.DevOptn.dMinImpedance    <= dImpedance    && dImpedance    <=OM.DevOptn.dMaxImpedance   ) ; sgDriveRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ;if(!bTemp)sFailItem = "Registance     ";}
        if(OM.DevOptn.bUseHysteresis  ){ bTemp = (OM.DevOptn.dMinHysteresis   <= dHysteresis   && dHysteresis   <=OM.DevOptn.dMaxHysteresis  ) ; sgDriveRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ;if(!bTemp)sFailItem = "Hysteresis     ";}
        if(OM.DevOptn.bUseCaseShort   ){ bTemp = bCaseShortOk;                                                                                   sgDriveRslt -> Cells[4][iRowCnt++] = bTemp ? "OK" : "NG" ;if(!bTemp)sFailItem = "Case Short     ";}

        iRowCnt = 1 ;
        if(OM.DevOptn.bUsePreLoad     &&!g_bInspEnd[m_iStgId])sgDriveRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUsePeakStroke  &&!g_bInspEnd[m_iStgId])sgDriveRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseCustomStroke&&!g_bInspEnd[m_iStgId])sgDriveRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseSensitivity &&!g_bInspEnd[m_iStgId])sgDriveRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseLinearity   &&!g_bInspEnd[m_iStgId])sgDriveRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseImpedance   &&!g_bInspEnd[m_iStgId])sgDriveRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseHysteresis  &&!g_bInspEnd[m_iStgId])sgDriveRslt -> Cells[4][iRowCnt++] = "" ;
        if(OM.DevOptn.bUseCaseShort   &&!g_bInspEnd[m_iStgId])sgDriveRslt -> Cells[4][iRowCnt++] = "" ;


        bool  bNG = false ;
        for(int i = 1 ; i < sgTiltRslt -> RowCount ; i++) {
            if(sgTiltRslt -> Cells[4][i] != "OK") bNG = true ;
        }
        for(int i = 1 ; i < sgDriveRslt -> RowCount ; i++) {
            if(sgDriveRslt -> Cells[4][i] != "OK") bNG = true ;
        }

        if(!g_bInspEnd[m_iStgId]) {
            lbRslt -> Caption = "WAIT" ;
            lbRslt -> Font -> Color = clWhite ;
        }
        else if(bNG){
            lbRslt -> Caption = "NG" ;
            lbRslt -> Font -> Color = clRed;
        }
        else {
            lbRslt -> Caption = "OK" ;
            lbRslt -> Font -> Color = clLime;
        }

        if(iWorkingStg ==0) {
            if(bNG){
                lbPreRslt -> Caption = "PreInsp:"+sFailItem ;
                lbPreRslt -> Font -> Color = clRed;
            }
            else {
                lbPreRslt -> Caption = "PreInsp:OK" ;
                lbPreRslt -> Font -> Color = clLime;
            }
        }
    }

    m_iPreWorkingStg  = iWorkingStg ;

//    Label1 -> Caption = String(g_VisnRsltPnt[m_iStgId].iMaxDataCnt) + " " + String(g_VisnRsltPnt[m_iStgId].iDataCnt) ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmRslt::sgTiltRsltDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    AnsiString sItem;

    TStringGrid * pSelGrid = dynamic_cast<TStringGrid *>(Sender) ;
    sItem = pSelGrid->Cells[ACol][ARow];

    if(State.Contains(gdSelected) ) {
        pSelGrid->Canvas->Brush->Color = clWhite ;
        pSelGrid->Canvas->Pen  ->Color = clSilver;
        pSelGrid->Canvas->Font ->Color = clBlack ;

        pSelGrid->Canvas->FillRect(Rect);
        pSelGrid->Canvas->TextOut(Rect.Left + 2, Rect.Top + 2, sItem);

    }

    pSelGrid->Canvas->Brush->Color = clWhite ;
    pSelGrid->Canvas->Pen  ->Color = clSilver;
    pSelGrid->Canvas->Font ->Color = clBlack ;

    if(ACol == 4 && ARow != 0){
        if(sItem != ""){
                 if(!g_bInspEnd[m_iStgId]) pSelGrid->Canvas->Brush->Color = clWhite;
            else if( sItem == "OK"       ) pSelGrid->Canvas->Brush->Color = clGreen;
            else if( sItem == "NG"       ) pSelGrid->Canvas->Brush->Color = clRed  ;

            pSelGrid->Canvas->FillRect(Rect);
            pSelGrid->Canvas->TextOut(Rect.Left + 3, Rect.Top + (Rect.Height() - pSelGrid->Canvas->TextHeight(sItem)) / 2, sItem);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmRslt::sgTiltRsltClick(TObject *Sender)
{
    g_pLastSelectedStringGrid = dynamic_cast<TStringGrid *>(Sender) ;
}
//---------------------------------------------------------------------------


