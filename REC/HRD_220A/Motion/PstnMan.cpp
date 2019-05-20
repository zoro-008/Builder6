
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PstnMan.h"
#include "UserINI.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "Sequence.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Index.h"
#include "Picker.h"
#include "Stage.h"
#include "Dispensor.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define NAME_COL_WIDTH   120
#define POS_COL_WIDTH    102
#define GO_COL_WIDTH     52
#define INPUT_COL_WIDTH  52
#define ROW_HEIGHT       25

CPstnMan PM;
//---------------------------------------------------------------------------
CPstnMan::CPstnMan()
{

}

CPstnMan::~CPstnMan()
{
//    SaveLastInfo();
/*  parent 할당시에 자동 삭제 됌.
    for( int i=0;i<MAX_MOTR;i++){
        if(m_pPstnList[i]) {
            delete m_pPstnList[i];
            m_pPstnList[i] = NULL ;
        }
        if(m_pPstnEdit[i]) {
            delete m_pPstnEdit[i] ;
            m_pPstnEdit[i] = NULL ;
        }
    }
*/
}

void CPstnMan::Init()
{
    if(MAX_MOTR != MT_GetMotorCnt()) {ShowMessageT("Motor Count is wrong");}

    memset(&GridInfo , 0 , sizeof(SGridSelInfo)             );
    memset( Value    , 0 , sizeof(SValue)*MAX_MOTR*MAX_PSTN );

    Value[miLDR_ZElevF][pvLDR_ZElevFWait        ].sName = "Wait           " ; Value[miLDR_ZElevF][pvLDR_ZElevFWait        ].bCommon=true ;
    Value[miLDR_ZElevF][pvLDR_ZElevFWorkStartBtm].sName = "SttBtm         " ; Value[miLDR_ZElevF][pvLDR_ZElevFWorkStartBtm].bCommon=true ;
    Value[miLDR_ZElevF][pvLDR_ZElevFWorkStartTop].sName = "SttTop         " ; Value[miLDR_ZElevF][pvLDR_ZElevFWorkStartTop].bCommon=true ;

    Value[miLDR_ZElevR][pvLDR_ZElevRWait        ].sName = "Wait           " ; Value[miLDR_ZElevR][pvLDR_ZElevRWait        ].bCommon=true ;
    Value[miLDR_ZElevR][pvLDR_ZElevRWorkStartBtm].sName = "SttBtm         " ; Value[miLDR_ZElevR][pvLDR_ZElevRWorkStartBtm].bCommon=true ;
    Value[miLDR_ZElevR][pvLDR_ZElevRWorkStartTop].sName = "SttTop         " ; Value[miLDR_ZElevR][pvLDR_ZElevRWorkStartTop].bCommon=true ;

    Value[miIDX_XIndxF][pvIDX_XIndxFWait        ].sName = "Wait           " ; Value[miIDX_XIndxF][pvIDX_XIndxFWait        ].bCommon=true ;
    Value[miIDX_XIndxF][pvIDX_XIndxFGet         ].sName = "Get            " ; Value[miIDX_XIndxF][pvIDX_XIndxFGet         ].bCommon=true ;
    Value[miIDX_XIndxF][pvIDX_XIndxFWorkStart   ].sName = "WorkStt        " ; Value[miIDX_XIndxF][pvIDX_XIndxFWorkStart   ].bCommon=true ;
    Value[miIDX_XIndxF][pvIDX_XIndxFOut         ].sName = "Out            " ; Value[miIDX_XIndxF][pvIDX_XIndxFOut         ].bCommon=true ;

    Value[miIDX_XIndxR][pvIDX_XIndxRWait        ].sName = "Wait           " ; Value[miIDX_XIndxR][pvIDX_XIndxRWait        ].bCommon=true ;
    Value[miIDX_XIndxR][pvIDX_XIndxRGet         ].sName = "Get            " ; Value[miIDX_XIndxR][pvIDX_XIndxRGet         ].bCommon=true ;
    Value[miIDX_XIndxR][pvIDX_XIndxRWorkStart   ].sName = "WorkStt        " ; Value[miIDX_XIndxR][pvIDX_XIndxRWorkStart   ].bCommon=true ;
    Value[miIDX_XIndxR][pvIDX_XIndxROut         ].sName = "Out            " ; Value[miIDX_XIndxR][pvIDX_XIndxROut         ].bCommon=true ;

    Value[miDSP_XDispr][pvDSP_XDisprWait        ].sName = "Wait           " ; Value[miDSP_XDispr][pvDSP_XDisprWait        ].bCommon=true ;
    Value[miDSP_XDispr][pvDSP_XDisprVsStageCntr ].sName = "Vs StgCntr     " ; Value[miDSP_XDispr][pvDSP_XDisprVsStageCntr ].bCommon=true ;
    Value[miDSP_XDispr][pvDSP_XDisprVsPad       ].sName = "Vs Pad         " ; Value[miDSP_XDispr][pvDSP_XDisprVsPad       ].bCommon=true ;
    Value[miDSP_XDispr][pvDSP_XDisprNdCheck     ].sName = "Nd Check       " ; Value[miDSP_XDispr][pvDSP_XDisprNdCheck     ].bCommon=true ;
    Value[miDSP_XDispr][pvDSP_XDisprNdClean     ].sName = "Nd Clean       " ; Value[miDSP_XDispr][pvDSP_XDisprNdClean     ].bCommon=true ;
    Value[miDSP_XDispr][pvDSP_XDisprHtCheck     ].sName = "Ht Check       " ; Value[miDSP_XDispr][pvDSP_XDisprHtCheck     ].bCommon=true ;
    Value[miDSP_XDispr][pvDSP_XDisprHtStage     ].sName = "Ht Stage       " ; Value[miDSP_XDispr][pvDSP_XDisprHtStage     ].bCommon=true ;
    Value[miDSP_XDispr][poDSP_XDisprVsNiddle    ].sName = "Visn Ndl Ofs   " ; Value[miDSP_XDispr][poDSP_XDisprVsNiddle    ].bCommon=true ; Value[miDSP_XDispr][poDSP_XDisprVsNiddle   ].iType = vtOffset;
    Value[miDSP_XDispr][poDSP_XDisprVsHeight    ].sName = "Visn Hgt Ofs   " ; Value[miDSP_XDispr][poDSP_XDisprVsHeight    ].bCommon=true ; Value[miDSP_XDispr][poDSP_XDisprVsHeight   ].iType = vtOffset;
    Value[miDSP_XDispr][pvDSP_XDisprStgFlatness1].sName = "Stage Flatness1" ; Value[miDSP_XDispr][pvDSP_XDisprStgFlatness1].bCommon=true ;
    Value[miDSP_XDispr][pvDSP_XDisprStgFlatness2].sName = "Stage Flatness2" ; Value[miDSP_XDispr][pvDSP_XDisprStgFlatness2].bCommon=true ;
    Value[miDSP_XDispr][pvDSP_XDisprStgFlatness3].sName = "Stage Flatness3" ; Value[miDSP_XDispr][pvDSP_XDisprStgFlatness3].bCommon=true ;
    Value[miDSP_XDispr][pvDSP_XDisprStgFlatness4].sName = "Stage Flatness4" ; Value[miDSP_XDispr][pvDSP_XDisprStgFlatness4].bCommon=true ;

    Value[miDSP_YDispr][pvDSP_YDisprWait        ].sName = "Wait           " ; Value[miDSP_YDispr][pvDSP_YDisprWait        ].bCommon=true ;
    Value[miDSP_YDispr][pvDSP_YDisprVsStageCntr ].sName = "Vs StgCntr     " ; Value[miDSP_YDispr][pvDSP_YDisprVsStageCntr ].bCommon=true ;
    Value[miDSP_YDispr][pvDSP_YDisprVsPad       ].sName = "Vs Pad         " ; Value[miDSP_YDispr][pvDSP_YDisprVsPad       ].bCommon=true ;
    Value[miDSP_YDispr][pvDSP_YDisprNdCheck     ].sName = "Nd Check       " ; Value[miDSP_YDispr][pvDSP_YDisprNdCheck     ].bCommon=true ;
    Value[miDSP_YDispr][pvDSP_YDisprNdClean     ].sName = "Nd Clean       " ; Value[miDSP_YDispr][pvDSP_YDisprNdClean     ].bCommon=true ;
    Value[miDSP_YDispr][pvDSP_YDisprHtCheck     ].sName = "Ht Check       " ; Value[miDSP_YDispr][pvDSP_YDisprHtCheck     ].bCommon=true ;
    Value[miDSP_YDispr][pvDSP_YDisprHtStage     ].sName = "Ht Stage       " ; Value[miDSP_YDispr][pvDSP_YDisprHtStage     ].bCommon=true ;
    Value[miDSP_YDispr][poDSP_YDisprVsNiddle    ].sName = "Visn Ndl Ofs   " ; Value[miDSP_YDispr][poDSP_YDisprVsNiddle    ].bCommon=true ; Value[miDSP_YDispr][poDSP_YDisprVsNiddle   ].iType = vtOffset;
    Value[miDSP_YDispr][poDSP_YDisprVsHeight    ].sName = "Visn Hgt Ofs   " ; Value[miDSP_YDispr][poDSP_YDisprVsHeight    ].bCommon=true ; Value[miDSP_YDispr][poDSP_YDisprVsHeight   ].iType = vtOffset;
    Value[miDSP_YDispr][pvDSP_YDisprStgFlatness1].sName = "Stage Flatness1" ; Value[miDSP_YDispr][pvDSP_YDisprStgFlatness1].bCommon=true ;
    Value[miDSP_YDispr][pvDSP_YDisprStgFlatness2].sName = "Stage Flatness2" ; Value[miDSP_YDispr][pvDSP_YDisprStgFlatness2].bCommon=true ;
    Value[miDSP_YDispr][pvDSP_YDisprStgFlatness3].sName = "Stage Flatness3" ; Value[miDSP_YDispr][pvDSP_YDisprStgFlatness3].bCommon=true ;
    Value[miDSP_YDispr][pvDSP_YDisprStgFlatness4].sName = "Stage Flatness4" ; Value[miDSP_YDispr][pvDSP_YDisprStgFlatness4].bCommon=true ;

    Value[miPCK_YPickr][pvPCK_YPickrWait        ].sName = "Wait           " ; Value[miPCK_YPickr][pvPCK_YPickrWait        ].bCommon=true ;
    Value[miPCK_YPickr][pvPCK_YPickrStageCntr   ].sName = "StgCntr        " ; Value[miPCK_YPickr][pvPCK_YPickrStageCntr   ].bCommon=true ;
    Value[miPCK_YPickr][pvPCK_YPickrPick        ].sName = "Pick           " ; Value[miPCK_YPickr][pvPCK_YPickrPick        ].bCommon=true ;
    Value[miPCK_YPickr][pvPCK_YPickrAlign       ].sName = "Align          " ; Value[miPCK_YPickr][pvPCK_YPickrAlign       ].bCommon=true ;
    Value[miPCK_YPickr][pvPCK_YPickrHeight      ].sName = "Height         " ; Value[miPCK_YPickr][pvPCK_YPickrHeight      ].bCommon=true ;
    Value[miPCK_YPickr][pvPCK_YPickrVision      ].sName = "Vision         " ; Value[miPCK_YPickr][pvPCK_YPickrVision      ].bCommon=true ;
    Value[miPCK_YPickr][pvPCK_YPickrCheck       ].sName = "Check          " ; Value[miPCK_YPickr][pvPCK_YPickrCheck       ].bCommon=true ;

    Value[miPCK_ZPickr][pvPCK_ZPickrWait        ].sName = "Wait           " ; Value[miPCK_ZPickr][pvPCK_ZPickrWait        ].bCommon=true ;
    Value[miPCK_ZPickr][pvPCK_ZPickrMove        ].sName = "Move           " ; Value[miPCK_ZPickr][pvPCK_ZPickrMove        ].bCommon=true ;
    Value[miPCK_ZPickr][pvPCK_ZPickrPick        ].sName = "Pick           " ;
    Value[miPCK_ZPickr][pvPCK_ZPickrAlign       ].sName = "Align          " ;
    Value[miPCK_ZPickr][pvPCK_ZPickrCheck       ].sName = "Check          " ; Value[miPCK_ZPickr][pvPCK_ZPickrCheck       ].bCommon=true ;
    Value[miPCK_ZPickr][poPCK_ZPickrVsDefect    ].sName = "VsDefect Ofs   " ; Value[miPCK_ZPickr][poPCK_ZPickrVsDefect    ].bCommon=true ; Value[miPCK_ZPickr][poPCK_ZPickrVsDefect   ].iType  =vtOffset ;
    Value[miPCK_ZPickr][poPCK_ZPickrVsAlign     ].sName = "VsAlign  Ofs   " ; Value[miPCK_ZPickr][poPCK_ZPickrVsAlign     ].bCommon=true ; Value[miPCK_ZPickr][poPCK_ZPickrVsAlign    ].iType  =vtOffset ;


    Value[miPRI_XIndxF][pvPRI_XIndxFWait        ].sName = "Wait           " ; Value[miPRI_XIndxF][pvPRI_XIndxFWait        ].bCommon=true ;
    Value[miPRI_XIndxF][pvPRI_XIndxFGet         ].sName = "Get            " ; Value[miPRI_XIndxF][pvPRI_XIndxFGet         ].bCommon=true ;
    Value[miPRI_XIndxF][pvPRI_XIndxFOut         ].sName = "Out            " ; Value[miPRI_XIndxF][pvPRI_XIndxFOut         ].bCommon=true ;
    Value[miPRI_XIndxF][pvPRI_XIndxFOutChckOn   ].sName = "Out Check On   " ; Value[miPRI_XIndxF][pvPRI_XIndxFOutChckOn   ].bCommon=true ;
    Value[miPRI_XIndxF][pvPRI_XIndxFOutChckOff  ].sName = "Out Chcek Off  " ; Value[miPRI_XIndxF][pvPRI_XIndxFOutChckOff  ].bCommon=true ;

    Value[miPRI_XIndxR][pvPRI_XIndxRWait        ].sName = "Wait           " ; Value[miPRI_XIndxR][pvPRI_XIndxRWait        ].bCommon=true ;
    Value[miPRI_XIndxR][pvPRI_XIndxRGet         ].sName = "Get            " ; Value[miPRI_XIndxR][pvPRI_XIndxRGet         ].bCommon=true ;
    Value[miPRI_XIndxR][pvPRI_XIndxROut         ].sName = "Out            " ; Value[miPRI_XIndxR][pvPRI_XIndxROut         ].bCommon=true ;
    Value[miPRI_XIndxR][pvPRI_XIndxROutChckOn   ].sName = "Out Check On   " ; Value[miPRI_XIndxR][pvPRI_XIndxROutChckOn   ].bCommon=true ;
    Value[miPRI_XIndxR][pvPRI_XIndxROutChckOff  ].sName = "Out Chcek Off  " ; Value[miPRI_XIndxR][pvPRI_XIndxROutChckOff  ].bCommon=true ;

    Value[miDSP_ZDispr][pvDSP_ZDisprWait        ].sName = "Wait           " ; Value[miDSP_ZDispr][pvDSP_ZDisprWait        ].bCommon=true ;
    Value[miDSP_ZDispr][pvDSP_ZDisprCheck       ].sName = "Check          " ;
    Value[miDSP_ZDispr][pvDSP_ZDisprMove        ].sName = "Move           " ; Value[miDSP_ZDispr][pvDSP_ZDisprMove        ].bCommon=true ;
    Value[miDSP_ZDispr][pvDSP_ZDisprClean       ].sName = "Clean          " ; Value[miDSP_ZDispr][pvDSP_ZDisprClean       ].bCommon=true ;

    Value[miSTG_TStage][pvSTG_TStagWait         ].sName = "Wait           " ; Value[miSTG_TStage][pvSTG_TStagWait         ].bCommon=true ;
    Value[miSTG_TStage][pvSTG_TStagWork         ].sName = "Work           " ; Value[miSTG_TStage][pvSTG_TStagWork         ].bCommon=true ;

    Value[miSTG_XStage][pvSTG_XStagWait         ].sName = "Wait           " ; Value[miSTG_XStage][pvSTG_XStagWait         ].bCommon=true ;
    Value[miSTG_XStage][pvSTG_XStagCenter       ].sName = "Center         " ; Value[miSTG_XStage][pvSTG_XStagCenter       ].bCommon=true ;

    Value[miSTG_YStage][pvSTG_YStagWait         ].sName = "Wait           " ; Value[miSTG_YStage][pvSTG_YStagWait         ].bCommon=true ;
    Value[miSTG_YStage][pvSTG_YStagCenter       ].sName = "Center         " ; Value[miSTG_YStage][pvSTG_YStagCenter       ].bCommon=true ;

    Value[miPCK_XAlign][pvPCK_XAlignWait        ].sName = "Wait           " ;
    Value[miPCK_XAlign][pvPCK_XAlignWork        ].sName = "Center         " ;

    Value[miPCK_YAlign][pvPCK_YAlignWait        ].sName = "Wait           " ;
    Value[miPCK_YAlign][pvPCK_YAlignWork        ].sName = "Center         " ;

    Value[miULD_ZElevF][pvULD_ZElevFWait        ].sName = "Wait           " ; Value[miULD_ZElevF][pvULD_ZElevFWait        ].bCommon=true ;
    Value[miULD_ZElevF][pvULD_ZElevFWorkStartTop].sName = "SttTop         " ; Value[miULD_ZElevF][pvULD_ZElevFWorkStartTop].bCommon=true ;
    Value[miULD_ZElevF][pvULD_ZElevFWorkStartBtm].sName = "SttBtm         " ; Value[miULD_ZElevF][pvULD_ZElevFWorkStartBtm].bCommon=true ;

    Value[miULD_ZElevR][pvULD_ZElevRWait        ].sName = "Wait           " ; Value[miULD_ZElevR][pvULD_ZElevRWait        ].bCommon=true ;
    Value[miULD_ZElevR][pvULD_ZElevRWorkStartTop].sName = "SttTop         " ; Value[miULD_ZElevR][pvULD_ZElevRWorkStartTop].bCommon=true ;
    Value[miULD_ZElevR][pvULD_ZElevRWorkStartBtm].sName = "SttBtm         " ; Value[miULD_ZElevR][pvULD_ZElevRWorkStartBtm].bCommon=true ;

    Value[miSTG_ZStage][pvSTG_ZStagWait         ].sName = "Wait           " ; Value[miSTG_ZStage][pvSTG_ZStagWait         ].bCommon=true ;
    Value[miSTG_ZStage][pvSTG_ZStagPick         ].sName = "Pick           " ;
    Value[miSTG_ZStage][pvSTG_ZStagCheck        ].sName = "Check          " ; Value[miSTG_ZStage][pvSTG_ZStagCheck        ].bCommon=true ;

    LoadLastInfo();

    Load(m_sCrntDev);
}


void CPstnMan::Close()
{

}


double CPstnMan::GetValue(int _iAxisNo , EN_PSTN_VALUE _iPstnNo)
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return 0.0 ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {
        ShowMessageT("Position Index Range is wrong");
        return 0.0 ;
    }

    return Value[_iAxisNo][_iPstnNo].dValue ;
}

EN_VALUE_TYPE CPstnMan::GetType(int _iAxisNo , EN_PSTN_VALUE _iPstnNo)
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return vtPosition ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {
        ShowMessageT("Position Index Range is wrong");
        return vtPosition ;
    }

    return Value[_iAxisNo][_iPstnNo].iType ;
}

bool CPstnMan::SetType  (int _iAxisNo , EN_PSTN_VALUE _iPstnNo , EN_VALUE_TYPE _iType)
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return 0.0 ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {
        ShowMessageT("Position Index Range is wrong");
        return 0.0 ;
    }

    Value[_iAxisNo][_iPstnNo].iType = _iType ;

    return true ;

}

double CPstnMan::SetValue(int _iAxisNo , EN_PSTN_VALUE _iPstnNo , double _dValue )
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return Value[_iAxisNo][_iPstnNo].dValue ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {
        ShowMessageT("Position Index Range is wrong");
        return Value[_iAxisNo][_iPstnNo].dValue ;
    }

    if(_dValue > MT_GetMaxPos(_iAxisNo) || _dValue < MT_GetMinPos(_iAxisNo)) {
        ShowMessageT("Position Range is wrong(Check Position Limit Max Or Min)");
        return Value[_iAxisNo][_iPstnNo].dValue ;
    }


    Value[_iAxisNo][_iPstnNo].dValue = _dValue ;
    //JS.
    //m_pPstnList[_iAxisNo]->Cells[1][_iPstnNo] = Value[_iAxisNo][_iPstnNo].dValue ;

    return Value[_iAxisNo][_iPstnNo].dValue ;


}

/*
bool CPstnMan::CmprPos(int _iAxisNo , EN_PSTN_VALUE _iPstnNo)
{
    if( _iAxisNo < 0 || _iAxisNo >= MAX_MOTR          ) {ShowMessageT("Motor Axis Range is wrong"    ); return 0.0 ;}
    if( _iPstnNo < 0 || _iPstnNo >= PSTN_CNT[_iAxisNo]) {ShowMessageT("Position Index Range is wrong"); return 0.0 ;}

    return MT_CmprPos(_iAxisNo , Value[_iAxisNo][_iPstnNo].dValue ) ;

}
*/
void CPstnMan::Save(AnsiString sJobName)
{
    //Local Var.
    TUserINI   UserINI     ;
    AnsiString sDevicePath ;
    AnsiString sCommonPath ;

    //Save Device.
    for(int m = 0 ; m < MAX_MOTR ; m++) {
        //Set Dir.
        sDevicePath = EXE_FOLDER + "JobFile\\"+sJobName+"\\"+MT_GetName(m)+"_Pos.INI" ;
        sCommonPath = EXE_FOLDER + "Util\\"+MT_GetName(m)+"_Pos.INI" ;
        UserINI.ClearFile(sDevicePath) ;
        UserINI.ClearFile(sCommonPath) ;

        for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
            if(Value[m][i].bCommon) {
                UserINI.Save(sCommonPath, (AnsiString)i+"_Pstn" , "sName " , Value[m][i].sName  );
                UserINI.Save(sCommonPath, (AnsiString)i+"_Pstn" , "dValue" , Value[m][i].dValue );
                UserINI.Save(sCommonPath, (AnsiString)i+"_Pstn" , "iType " , Value[m][i].iType  );
            }
            else {
                UserINI.Save(sDevicePath, (AnsiString)i+"_Pstn" , "sName " , Value[m][i].sName  );
                UserINI.Save(sDevicePath, (AnsiString)i+"_Pstn" , "dValue" , Value[m][i].dValue );
                UserINI.Save(sDevicePath, (AnsiString)i+"_Pstn" , "iType " , Value[m][i].iType  );
            }
        }
    }
}

void CPstnMan::Load(AnsiString sJobName)
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sDevicePath ;
    AnsiString sCommonPath ;

    //Load Device.
    for(int m = 0 ; m < MAX_MOTR ; m++) {
        //Set Dir.
        sDevicePath = EXE_FOLDER + "JobFile\\"+sJobName+"\\"+MT_GetName(m)+"_Pos.INI" ;
        sCommonPath = EXE_FOLDER + "Util\\"+MT_GetName(m)+"_Pos.INI" ;
        for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
            if(Value[m][i].bCommon) {
                UserINI.Load(sCommonPath, (AnsiString)i+"_Pstn" , "dValue" , Value[m][i].dValue );
            }
            else {
                UserINI.Load(sDevicePath, (AnsiString)i+"_Pstn" , "dValue" , Value[m][i].dValue );
            }
        }
    }
}

void CPstnMan::UpdatePstn(bool _bToTable)
{
    if(_bToTable) {
        for(int m = 0 ; m < MAX_MOTR ; m++) {
            for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
                m_pPstnList[m] -> Cells[0][i] = (Value[m][i].sName  != "" ) ?             Value[m][i].sName  : "NoName"+(AnsiString)i;
                m_pPstnList[m] -> Cells[1][i] = (Value[m][i].dValue != 0.0) ? (AnsiString)Value[m][i].dValue : (AnsiString)"0.0"     ;
            }
        }
    }

    else {
        for(int m = 0 ; m < MAX_MOTR ; m++) {
            for(int i = 0 ; i < PSTN_CNT[m] ; i++) {
                Value[m][i].sName  = m_pPstnList[m] -> Cells[0][i]           ;
                Value[m][i].dValue = m_pPstnList[m] -> Cells[1][i].ToDouble();
            }
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall CPstnMan::pPstnListDrawCell(TObject *Sender, int ACol,
      int ARow, const TRect &Rect, TGridDrawState State)
{
    TRect CellRect,TextRect;
    TStringGrid* StringGrid = static_cast<TStringGrid*>(Sender);
    int iSel = StringGrid -> Tag ;
    TCanvas* ACanvas = StringGrid->Canvas;

    CellRect = Rect ;

    if( ACol == 0) {
        if(Value[iSel][ARow].bCommon) {
             AnsiString CellText = Value[iSel][ARow].sName ;

             unsigned char step = 30;
             unsigned char r = 0x80,g = 0x80,b = 0x80;
             TColor clr_highlight = static_cast<TColor>( PALETTERGB(r+step,g+step,b+step) );
             TColor clr_shadow = static_cast<TColor>( PALETTERGB(r-step,g-step,b-step) );

             TextRect = CellRect ;
             GridInfo.bDown = false ; //이셀은 눌리는 셀이 아니다.
             TextRect.Left = TextRect.Left  + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 6  : 5 ) ;
             TextRect.Top  = TextRect.Top   + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 1  : 0 ) ;

             //if(Value[iSel][ARow].iType == vtPosition ) {
                 ACanvas->Brush->Color = (TColor)RGB(200,200,200);
                 ACanvas->FillRect( CellRect );
                 if(GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol && iSel == GridInfo.iGrid)
                      Frame3D( ACanvas,CellRect,clr_shadow,clr_highlight,2 );
                 else Frame3D( ACanvas,CellRect,clr_highlight,clr_shadow,2 );

                 DrawText( ACanvas->Handle,CellText.c_str(),CellText.Length(),&TextRect,
                                        DT_LEFT | DT_VCENTER | DT_SINGLELINE );
             //}
         }
         else {
         }
    }

    if( ACol == 1) {
        ACanvas->Brush -> Style = bsClear ;
        ACanvas->Pen   -> Color = clBlack ;

        TextRect = CellRect;
        TextRect.Left = TextRect.Left ;
        ACanvas->FillRect (CellRect) ;
        ACanvas->Rectangle(CellRect) ;

//        if(GridInfo.bDown ) {
//            if(ARow == GridInfo.iRow && ACol == GridInfo.iCol ) {
//                m_pPstnEdit[GridInfo.iGrid] -> Left    = GridInfo.iColX ;
//                m_pPstnEdit[GridInfo.iGrid] -> Top     = GridInfo.iRowY ;
//                m_pPstnEdit[GridInfo.iGrid] -> Visible = true ;
//                GridInfo.bEdit = true ;
//            }
//        }
    }

    if( ACol == 2) {
         AnsiString CellText = "GO";

         unsigned char step = 30;
         unsigned char r = 0x80,g = 0x80,b = 0xC0;
         TColor clr_highlight = static_cast<TColor>( PALETTERGB(r+step,g+step,b+step) );
         TColor clr_shadow = static_cast<TColor>( PALETTERGB(r-step,g-step,b-step) );

         TextRect = CellRect;
         TextRect.Left = TextRect.Left  + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 16 : 15) ;
         TextRect.Top  = TextRect.Top   + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 1  : 0 ) ;


         if(Value[iSel][ARow].iType == vtPosition) {
             ACanvas->Brush->Color = (TColor)RGB(128,128,192);
             ACanvas->FillRect( CellRect );
             if(GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol && iSel == GridInfo.iGrid)
                  Frame3D( ACanvas,CellRect,clr_shadow,clr_highlight,2 );
             else Frame3D( ACanvas,CellRect,clr_highlight,clr_shadow,2 );

             DrawText( ACanvas->Handle,CellText.c_str(),CellText.Length(),&TextRect,
                                    DT_LEFT | DT_VCENTER | DT_SINGLELINE );
         }
         else {
             ACanvas->Brush->Color = (TColor)RGB(255,255,255);
             ACanvas->FillRect( CellRect );
         }
    }

    if( ACol == 3) {
         AnsiString CellText = "INPUT";

         unsigned char step = 30;
         unsigned char r = 0xC0,g = 0xC0,b = 0x80;
         TColor clr_highlight = static_cast<TColor>( PALETTERGB(r+step,g+step,b+step) );
         TColor clr_shadow = static_cast<TColor>( PALETTERGB(r-step,g-step,b-step) );

         TextRect = CellRect ;
         TextRect.Left = TextRect.Left  + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 6  : 5 ) ;
         TextRect.Top  = TextRect.Top   + ((GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol ) ? 1  : 0 ) ;

         if(Value[iSel][ARow].iType == vtPosition ) {
             ACanvas->Brush->Color = (TColor)RGB(192,192,128);
             ACanvas->FillRect( CellRect );
             if(GridInfo.bDown && ARow == GridInfo.iRow && ACol == GridInfo.iCol && iSel == GridInfo.iGrid)
                  Frame3D( ACanvas,CellRect,clr_shadow,clr_highlight,2 );
             else Frame3D( ACanvas,CellRect,clr_highlight,clr_shadow,2 );

             DrawText( ACanvas->Handle,CellText.c_str(),CellText.Length(),&TextRect,
                                    DT_LEFT | DT_VCENTER | DT_SINGLELINE );
         }
         else {
             ACanvas->Brush->Color = (TColor)RGB(255,255,255);
             ACanvas->FillRect( CellRect );
         }
     }
}

//---------------------------------------------------------------------------
void __fastcall CPstnMan::pPstnListMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iSel      = ((TStringGrid *)Sender)-> Tag ;
    int iX        = X ;
    int iTempColX = 0 ;

    for( int i=0;i<MAX_MOTR;i++){
        if(m_pPstnEdit[i] -> Visible ) {m_pPstnEdit[i] -> SetFocus(); return ; }
    }

    GridInfo.iGrid = iSel ;
    GridInfo.bDown = true ;
    for(int i = 0 ; i < m_pPstnList[iSel] -> ColCount ; i ++) {
        if((iX -= m_pPstnList[iSel] -> ColWidths[i] )> 0) {
            iTempColX += m_pPstnList[iSel] -> ColWidths[i] ;
            continue ;
        }
        GridInfo.iCol = i;
        break ;
    }

    const int iGridOftY = 3 ;
    GridInfo.iRow  = Y/(m_pPstnList[iSel]->DefaultRowHeight+1);

    if(GridInfo.iRow >= PSTN_CNT[iSel]) return ;
    GridInfo.iX    = X ;
    GridInfo.iY    = Y ;
    GridInfo.iColX = iTempColX ;
    GridInfo.iRowY = GridInfo.iRow * (m_pPstnList[iSel]->DefaultRowHeight+1);

    m_pPstnEdit[GridInfo.iGrid] -> Text = m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] ;
    if(1 == GridInfo.iCol && !Value[(EN_MOTR_ID)GridInfo.iGrid][(EN_PSTN_ID)GridInfo.iRow].bFix ) {
        if(Value[(EN_MOTR_ID)GridInfo.iGrid][(EN_PSTN_ID)GridInfo.iRow].bCommon)FM_MsgOkModal("공용포지션","이포지션은 모든 디바이스 같이 사용되는 포지션입니다.");
        m_pPstnEdit[GridInfo.iGrid] -> Left    = GridInfo.iColX ;
        m_pPstnEdit[GridInfo.iGrid] -> Top     = GridInfo.iRowY ;
        m_pPstnEdit[GridInfo.iGrid] -> Visible = true ;
        m_pPstnEdit[GridInfo.iGrid] -> SetFocus()     ;
    }

    if(2 == GridInfo.iCol ) {
         AnsiString sTemp ;
         sTemp = " going to goto " + Value[GridInfo.iGrid][GridInfo.iRow].sName + " Pos" ;
         Trace(MT_GetName(GridInfo.iGrid),sTemp.c_str()); //pi 와 pv를 귀찮아서 그냥 같이 쓴다. pi pv할당할때 조심....
         if(GridInfo.iGrid == miLDR_ZElevF) LDR_F.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLDR_ZElevR) LDR_R.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miIDX_XIndxF) IDX_F.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miIDX_XIndxR) IDX_R.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miDSP_XDispr) DSP  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miDSP_YDispr) DSP  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPCK_YPickr) PCK  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPCK_ZPickr) PCK  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPRI_XIndxF) PRI_F.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPRI_XIndxR) PRI_R.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miDSP_ZDispr) DSP  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTG_TStage) STG  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTG_XStage) STG  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTG_YStage) STG  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPCK_XAlign) PCK  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miPCK_YAlign) PCK  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_ZElevF) ULD_F.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_ZElevR) ULD_R.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTG_ZStage) STG  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
    }

    if(3 == GridInfo.iCol ) {
        Trace(MT_GetName(GridInfo.iGrid) , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+MT_GetCmdPos(GridInfo.iGrid)).c_str());
        if(Value[(EN_MOTR_ID)GridInfo.iGrid][(EN_PSTN_ID)GridInfo.iRow].iType == vtPosition) {
            m_pPstnList[GridInfo.iGrid] -> Cells[1][GridInfo.iRow] = MT_GetCmdPos(GridInfo.iGrid) ;
        }
    }

    m_pPstnList[GridInfo.iGrid] -> Repaint() ;
}
//---------------------------------------------------------------------------
void __fastcall CPstnMan::pPstnListMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    GridInfo.bDown = false ;
    m_pPstnList[GridInfo.iGrid] -> Repaint() ;
}
void CPstnMan::SetParent(TPanel * _pnBase , int _iAxisNo)
{
    m_pPstnList[_iAxisNo]-> Parent = _pnBase ;
    m_pPstnEdit[_iAxisNo]-> Parent = _pnBase ;
}
void CPstnMan::SetWindow(TPanel * _pnBase , int _iAxisNo)
{
    if(_iAxisNo >= MAX_MOTR) {ShowMessageT("Motor Count is wrong"); return ; }

    m_pPstnList[_iAxisNo]                    = new TStringGrid(_pnBase);
    m_pPstnList[_iAxisNo]-> Tag              = _iAxisNo;
    m_pPstnList[_iAxisNo]-> Parent           = _pnBase ;
    m_pPstnList[_iAxisNo]-> Tag              = _iAxisNo  ;
    m_pPstnList[_iAxisNo]-> ColCount         = 4  ;
    m_pPstnList[_iAxisNo]-> DefaultRowHeight = ROW_HEIGHT ;
    m_pPstnList[_iAxisNo]-> DefaultColWidth  = 65 ;
    m_pPstnList[_iAxisNo]-> ColWidths[0]     = NAME_COL_WIDTH  ;
    m_pPstnList[_iAxisNo]-> ColWidths[1]     = POS_COL_WIDTH   ;
    m_pPstnList[_iAxisNo]-> ColWidths[2]     = GO_COL_WIDTH    ;
    m_pPstnList[_iAxisNo]-> ColWidths[3]     = INPUT_COL_WIDTH ;
    m_pPstnList[_iAxisNo]-> FixedRows        = 0  ;
    m_pPstnList[_iAxisNo]-> FixedCols        = 1  ;
    m_pPstnList[_iAxisNo]-> Options          >> goEditing     ;
    m_pPstnList[_iAxisNo]-> Options          << goRowSelect   ;
    m_pPstnList[_iAxisNo]-> Options          >> goRangeSelect ;
    m_pPstnList[_iAxisNo]-> ScrollBars       = ssVertical     ;
    m_pPstnList[_iAxisNo]-> Align            = alClient       ;
    m_pPstnList[_iAxisNo]-> Font -> Charset  = ARABIC_CHARSET ;
    m_pPstnList[_iAxisNo]-> Font -> Size     = 11             ;
    m_pPstnList[_iAxisNo]-> OnMouseDown      = pPstnListMouseDown ;
    m_pPstnList[_iAxisNo]-> OnMouseUp        = pPstnListMouseUp   ;
    m_pPstnList[_iAxisNo]-> OnDrawCell       = pPstnListDrawCell  ;

    m_pPstnList[_iAxisNo]-> RowCount         = PSTN_CNT[_iAxisNo] ;
    m_pPstnList[_iAxisNo]-> Visible          = true ;

    m_pPstnEdit[_iAxisNo]                = new TEdit(_pnBase);
    m_pPstnEdit[_iAxisNo]-> Parent       = _pnBase           ;
    m_pPstnEdit[_iAxisNo]-> ImeMode      = imDisable         ;
    m_pPstnEdit[_iAxisNo]-> NumberOnly   = true              ;
    m_pPstnEdit[_iAxisNo]-> UseApoint    = true              ;
    m_pPstnEdit[_iAxisNo]-> UseMinus     = true              ;
    m_pPstnEdit[_iAxisNo]-> UseComma     = false             ;
    m_pPstnEdit[_iAxisNo]-> Alignment    = taRightJustify    ;
    m_pPstnEdit[_iAxisNo]-> Height       = ROW_HEIGHT        ;//에디트가 좀더 큰듯.
    m_pPstnEdit[_iAxisNo]-> Width        = POS_COL_WIDTH     ;

    m_pPstnEdit[_iAxisNo]-> OnKeyDown    = pPstnEditKeyDown  ;
    m_pPstnEdit[_iAxisNo]-> OnExit       = pPstnEditExit     ;
    m_pPstnEdit[_iAxisNo]-> Visible      = false ;




}

//---------------------------------------------------------------------------
void __fastcall CPstnMan::pPstnEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
/*
   if(Key == VK_RETURN){
        Trace(MT_GetName(GridInfo.iGrid).c_str() , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
        m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble() ;
        m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
        m_pPstnList[GridInfo.iGrid] -> Repaint() ;
   }
   else if(Key == VK_ESCAPE) {
        m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
        m_pPstnList[GridInfo.iGrid] -> Repaint() ;
   }
*/
    if(Key == VK_RETURN){
        if(m_pPstnEdit[GridInfo.iGrid] -> Text == "") m_pPstnEdit[GridInfo.iGrid] -> Text = 0.0 ;
        Trace(MT_GetName(GridInfo.iGrid) , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
        m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble() ;
        m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
        m_pPstnList[GridInfo.iGrid] -> Repaint() ;
    }
    else if(Key == VK_ESCAPE) {
        m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
        m_pPstnList[GridInfo.iGrid] -> Repaint() ;
    }

}


void __fastcall CPstnMan::pPstnEditExit(TObject *Sender)
{
    Trace(MT_GetName(GridInfo.iGrid) , (m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow]+"->"+m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble()).c_str());
    m_pPstnList[GridInfo.iGrid] -> Cells[GridInfo.iCol][GridInfo.iRow] = m_pPstnEdit[GridInfo.iGrid] -> Text.ToDouble() ;
    m_pPstnEdit[GridInfo.iGrid] -> Visible = false ;
    m_pPstnList[GridInfo.iGrid] -> Repaint() ;

}

void CPstnMan::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO" , "Device"  , m_sCrntDev );

    if (m_sCrntDev == "") m_sCrntDev  = "NONE";
}




