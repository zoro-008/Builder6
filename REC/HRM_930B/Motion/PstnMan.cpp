
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
#include "LoaderFront.h"
#include "LoaderRear.h"
#include "Index.h"
#include "Stage.h"
#include "LeftTool.h"
#include "RightTool.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define NAME_COL_WIDTH   110 // juhyeon. 기존 사이즈 120
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
    //if(MAX_MOTR != MT_GetMotorCnt()) {ShowMessageT("Motor Count is wrong");}

    memset(&GridInfo , 0 , sizeof(SGridSelInfo)                 );
    memset( Value    , 0 , sizeof(SValue) * MAX_MOTR * MAX_PSTN );

    // 0. miLTL_XGenRr
    Value[miLTL_XGenRr][pvLTL_XWait           ].sName = "Wait           " ; Value[miLTL_XGenRr][pvLTL_XWait           ].bCommon=true  ;
    Value[miLTL_XGenRr][pvLTL_XGripGrip1      ].sName = "GripGrip1      " ; Value[miLTL_XGenRr][pvLTL_XGripGrip1      ].bCommon=false ;
    Value[miLTL_XGenRr][pvLTL_XGripGrip2      ].sName = "GripGrip2      " ; Value[miLTL_XGenRr][pvLTL_XGripGrip2      ].bCommon=false ;
    Value[miLTL_XGenRr][pvLTL_XGripPull       ].sName = "GripPull       " ; Value[miLTL_XGenRr][pvLTL_XGripPull       ].bCommon=false ;
    Value[miLTL_XGenRr][pvLTL_XIndxBack       ].sName = "IndxBack       " ; Value[miLTL_XGenRr][pvLTL_XIndxBack       ].bCommon=false ;
    Value[miLTL_XGenRr][pvLTL_XIndxWork       ].sName = "IndxWork       " ; Value[miLTL_XGenRr][pvLTL_XIndxWork       ].bCommon=false ;
    Value[miLTL_XGenRr][pvLTL_XDispClean      ].sName = "DispClean      " ; Value[miLTL_XGenRr][pvLTL_XDispClean      ].bCommon=true  ;
    Value[miLTL_XGenRr][pvLTL_XDispFtCheck    ].sName = "DispFtCheck    " ; Value[miLTL_XGenRr][pvLTL_XDispFtCheck    ].bCommon=true  ;
    Value[miLTL_XGenRr][pvLTL_XDispRrCheck    ].sName = "DispRrCheck    " ; Value[miLTL_XGenRr][pvLTL_XDispRrCheck    ].bCommon=true  ;
    Value[miLTL_XGenRr][pvLTL_XDispUVCheck    ].sName = "DispUVCheck    " ; Value[miLTL_XGenRr][pvLTL_XDispUVCheck    ].bCommon=true  ;
    Value[miLTL_XGenRr][pvLTL_XVisnHtCheck    ].sName = "VisnHtCheck    " ; Value[miLTL_XGenRr][pvLTL_XVisnHtCheck    ].bCommon=true  ;
    Value[miLTL_XGenRr][pvLTL_XVisnCheck      ].sName = "VisnCheck      " ; Value[miLTL_XGenRr][pvLTL_XVisnCheck      ].bCommon=true  ;
    Value[miLTL_XGenRr][pvLTL_XVisnCmsOri     ].sName = "VisnCmsOri     " ; Value[miLTL_XGenRr][pvLTL_XVisnCmsOri     ].bCommon=false ;
    Value[miLTL_XGenRr][pvLTL_XWorkOut        ].sName = "WorkOut        " ; Value[miLTL_XGenRr][pvLTL_XWorkOut        ].bCommon=false ;
    Value[miLTL_XGenRr][pvLTL_XVisnRttCnt     ].sName = "VisnRttCnt     " ; Value[miLTL_XGenRr][pvLTL_XVisnRttCnt     ].bCommon=false ;

    // 1. miLTL_XGenFt
    Value[miLTL_XGenFt][pvLTL_XGenFtWait      ].sName = "Wait           " ; Value[miLTL_XGenFt][pvLTL_XGenFtWait      ].bCommon=true  ;

    // 2. miLTL_YDispr
    Value[miLTL_YDispr][pvLTL_YDispWait       ].sName = "DispWait       " ; Value[miLTL_YDispr][pvLTL_YDispWait       ].bCommon=true  ;
    Value[miLTL_YDispr][pvLTL_YDispClean      ].sName = "DispClean      " ; Value[miLTL_YDispr][pvLTL_YDispClean      ].bCommon=true  ;
    Value[miLTL_YDispr][pvLTL_YDispFtCheck    ].sName = "DispFtCheck    " ; Value[miLTL_YDispr][pvLTL_YDispFtCheck    ].bCommon=true  ;
    Value[miLTL_YDispr][pvLTL_YDispRrCheck    ].sName = "DispRrCheck    " ; Value[miLTL_YDispr][pvLTL_YDispRrCheck    ].bCommon=true  ;
    Value[miLTL_YDispr][pvLTL_YDispUVCheck    ].sName = "UVCrCheck      " ; Value[miLTL_YDispr][pvLTL_YDispUVCheck    ].bCommon=true  ;

    // 3. miLTL_ZDispr
    Value[miLTL_ZDispr][pvLTL_ZDispWait       ].sName = "ZDispWait      " ; Value[miLTL_ZDispr][pvLTL_ZDispWait       ].bCommon=true  ;
    Value[miLTL_ZDispr][pvLTL_ZDispFtCheck    ].sName = "ZDispFtCheck   " ; Value[miLTL_ZDispr][pvLTL_ZDispFtCheck    ].bCommon=true  ;
    Value[miLTL_ZDispr][pvLTL_ZDispRrCheck    ].sName = "ZDispRrCheck   " ; Value[miLTL_ZDispr][pvLTL_ZDispRrCheck    ].bCommon=true  ;
    Value[miLTL_ZDispr][pvLTL_ZDispUVWork     ].sName = "ZDispUVWork    " ; Value[miLTL_ZDispr][pvLTL_ZDispUVWork     ].bCommon=false ;
    Value[miLTL_ZDispr][pvLTL_ZDispClean      ].sName = "ZDispClean     " ; Value[miLTL_ZDispr][pvLTL_ZDispClean      ].bCommon=true  ;
    Value[miLTL_ZDispr][pvLTL_ZDispUVTest     ].sName = "ZDispUVTest    " ; Value[miLTL_ZDispr][pvLTL_ZDispUVTest     ].bCommon=true  ;

    // 4. miRTL_XGenRr
    Value[miRTL_XGenRr][pvRTL_XWait           ].sName = "Wait           " ; Value[miRTL_XGenRr][pvRTL_XWait           ].bCommon=true  ;
    Value[miRTL_XGenRr][pvRTL_XLIdxOutStt     ].sName = "LIdxOutStt     " ; Value[miRTL_XGenRr][pvRTL_XLIdxOutStt     ].bCommon=false ;
    Value[miRTL_XGenRr][pvRTL_XLIdxOutEnd     ].sName = "LIdxOutEnd     " ; Value[miRTL_XGenRr][pvRTL_XLIdxOutEnd     ].bCommon=false ;
    Value[miRTL_XGenRr][pvRTL_XRIdxOutStt     ].sName = "RIdxOutStt     " ; Value[miRTL_XGenRr][pvRTL_XRIdxOutStt     ].bCommon=false ;
    Value[miRTL_XGenRr][pvRTL_XRIdxOutEnd     ].sName = "RIdxOutEnd     " ; Value[miRTL_XGenRr][pvRTL_XRIdxOutEnd     ].bCommon=false ;
    Value[miRTL_XGenRr][pvRTL_XTrsfCheck      ].sName = "TrsfCheck      " ; Value[miRTL_XGenRr][pvRTL_XTrsfCheck      ].bCommon=true  ;
    Value[miRTL_XGenRr][pvRTL_XTrsfRotator    ].sName = "TrsfRotator    " ; Value[miRTL_XGenRr][pvRTL_XTrsfRotator    ].bCommon=false ;
    Value[miRTL_XGenRr][pvRTL_XTrsfRotatorRvs ].sName = "TrsfRotatorRvs " ; Value[miRTL_XGenRr][pvRTL_XTrsfRotator    ].bCommon=false ;
    Value[miRTL_XGenRr][pvRTL_XVisnCheck      ].sName = "VisnCheck      " ; Value[miRTL_XGenRr][pvRTL_XVisnCheck      ].bCommon=true  ;
    Value[miRTL_XGenRr][pvRTL_XVisnStgCnt     ].sName = "VisnStgCnt     " ; Value[miRTL_XGenRr][pvRTL_XVisnStgCnt     ].bCommon=false ;
    Value[miRTL_XGenRr][pvRTL_XVisnHtCheck    ].sName = "HghtCheck      " ; Value[miRTL_XGenRr][pvRTL_XVisnHtCheck    ].bCommon=true  ;
    Value[miRTL_XGenRr][pvRTL_XVisnCfCheck    ].sName = "ConfocalCheck  " ; Value[miRTL_XGenRr][pvRTL_XVisnCfCheck    ].bCommon=true  ;
    //Value[miRTL_XGenRr][poRTL_XTrsfPlceOfs    ].sName = "TrsfPlaceOffset" ; Value[miRTL_XGenRr][poRTL_XTrsfPlceOfs    ].bCommon=false ; Value[miRTL_XGenRr][poRTL_XTrsfPlceOfs    ].iType=vtOffset ;


    // 5. miRTL_XGenFt
    Value[miRTL_XGenFt][pvRTL_XGenFtWait      ].sName = "Wait           " ; Value[miRTL_XGenFt][pvRTL_XGenFtWait      ].bCommon=true ;

    // 6. miRTL_YVisn
    Value[miRTL_YVisn ][pvRTL_YVisnWait       ].sName = "VisnWait       " ; Value[miRTL_YVisn ][pvRTL_YVisnWait       ].bCommon=true  ;
    Value[miRTL_YVisn ][pvRTL_YVisnCheck      ].sName = "VisnCheck      " ; Value[miRTL_YVisn ][pvRTL_YVisnCheck      ].bCommon=true  ;
    Value[miRTL_YVisn ][pvRTL_YVisnStgCnt     ].sName = "VisnStgCnt     " ; Value[miRTL_YVisn ][pvRTL_YVisnStgCnt     ].bCommon=false ;
    Value[miRTL_YVisn ][pvRTL_YVisnHtCheck    ].sName = "HghtCheck      " ; Value[miRTL_YVisn ][pvRTL_YVisnHtCheck    ].bCommon=true  ;
    Value[miRTL_YVisn ][pvRTL_YVisnCfCheck    ].sName = "ConfocalCheck  " ; Value[miRTL_YVisn ][pvRTL_YVisnCfCheck    ].bCommon=true  ;

    // 7. miRTL_ZVisn
    Value[miRTL_ZVisn ][pvRTL_ZVisnWait       ].sName = "VisnWait       " ; Value[miRTL_ZVisn][pvRTL_ZVisnWait        ].bCommon=true  ;
    Value[miRTL_ZVisn ][pvRTL_ZVisnCheckWork  ].sName = "VisnCheckWork  " ; Value[miRTL_ZVisn][pvRTL_ZVisnCheckWork   ].bCommon=true  ;
    Value[miRTL_ZVisn ][pvRTL_ZVisnCmsAlign   ].sName = "VisnCmsAlign   " ; Value[miRTL_ZVisn][pvRTL_ZVisnCmsAlign    ].bCommon=false ;
    Value[miRTL_ZVisn ][pvRTL_ZVisnCmsConfocal].sName = "VisnCmsConfocal" ; Value[miRTL_ZVisn][pvRTL_ZVisnCmsConfocal ].bCommon=false ;

    // 8. miLTL_YVisn
    Value[miLTL_YVisn ][pvLTL_YVisnWait       ].sName = "VisnWait       " ; Value[miLTL_YVisn][pvLTL_YVisnWait        ].bCommon=true  ;
    Value[miLTL_YVisn ][pvLTL_YVisnHtCheck    ].sName = "HghtCheck      " ; Value[miLTL_YVisn][pvLTL_YVisnHtCheck     ].bCommon=true  ;
    Value[miLTL_YVisn ][pvLTL_YVisnCheck      ].sName = "VisnCheck      " ; Value[miLTL_YVisn][pvLTL_YVisnCheck       ].bCommon=true  ;
    Value[miLTL_YVisn ][pvLTL_YVisnCmsOri     ].sName = "VisnCmsOri     " ; Value[miLTL_YVisn][pvLTL_YVisnCmsOri      ].bCommon=false ;
    Value[miLTL_YVisn ][pvLTL_YVisnRttCnt     ].sName = "VisnRttCnt     " ; Value[miLTL_YVisn][pvLTL_YVisnRttCnt      ].bCommon=false ;

    // 9. miRTL_YTrspr
    Value[miRTL_YTrspr][pvRTL_YTrsfWait       ].sName = "TrsfWait       " ; Value[miRTL_YTrspr][pvRTL_YTrsfWait       ].bCommon=true  ;
    Value[miRTL_YTrspr][pvRTL_YTrsfCheck      ].sName = "TrsfCheck      " ; Value[miRTL_YTrspr][pvRTL_YTrsfCheck      ].bCommon=true  ;
    Value[miRTL_YTrspr][pvRTL_YTrsfRoatator   ].sName = "TrsfRoatator   " ; Value[miRTL_YTrspr][pvRTL_YTrsfRoatator   ].bCommon=false ;
    Value[miRTL_YTrspr][pvRTL_YTrsfRotatorRvs ].sName = "TrsfRotatorRvs " ; Value[miRTL_YTrspr][pvRTL_YTrsfRotatorRvs ].bCommon=false ;

    // 10. miSTG_XStage
    Value[miSTG_XStage][pvSTG_XStageWait      ].sName = "StageWait      " ; Value[miSTG_XStage][pvSTG_XStageWait      ].bCommon=true  ;
    Value[miSTG_XStage][pvSTG_XStageCntr      ].sName = "StageCntr      " ; Value[miSTG_XStage][pvSTG_XStageCntr      ].bCommon=false ;

    // 11. miSTG_YStage
    Value[miSTG_YStage][pvSTG_YStageWait      ].sName = "StageWait      " ; Value[miSTG_YStage][pvSTG_YStageWait      ].bCommon=true  ;
    Value[miSTG_YStage][pvSTG_YStageCntr      ].sName = "StageCntr      " ; Value[miSTG_YStage][pvSTG_YStageCntr      ].bCommon=false ;

    // 12. miLTL_ZVisn
    Value[miLTL_ZVisn ][pvLTL_ZVisnWait       ].sName = "VisnWait       " ; Value[miLTL_ZVisn ][pvLTL_ZVisnWait       ].bCommon=true  ;
    Value[miLTL_ZVisn ][pvLTL_ZVisnCheck      ].sName = "VisnCheck      " ; Value[miLTL_ZVisn ][pvLTL_ZVisnCheck      ].bCommon=true  ;
    Value[miLTL_ZVisn ][pvLTL_ZVisnMid        ].sName = "VisnMid        " ; Value[miLTL_ZVisn ][pvLTL_ZVisnMid        ].bCommon=false ;
    Value[miLTL_ZVisn ][pvLTL_ZVisnCmsOri     ].sName = "VisnCms Ori    " ; Value[miLTL_ZVisn ][pvLTL_ZVisnCmsOri     ].bCommon=false ;
    Value[miLTL_ZVisn ][pvLTL_ZVisnCmsAlign   ].sName = "VisnCms Align  " ; Value[miLTL_ZVisn ][pvLTL_ZVisnCmsAlign   ].bCommon=false ;

    // 13. miIDX_XIndxR
    Value[miIDX_XIndxR][pvIDX_XIndxRWait      ].sName = "IndxRWait      " ; Value[miIDX_XIndxR][pvIDX_XIndxRWait      ].bCommon=true  ;
    Value[miIDX_XIndxR][pvIDX_XIndxRGetWait   ].sName = "IndxRGetWait   " ; Value[miIDX_XIndxR][pvIDX_XIndxRGetWait   ].bCommon=true  ;
    Value[miIDX_XIndxR][pvIDX_XIndxRCheck1    ].sName = "IndxRCheck1    " ; Value[miIDX_XIndxR][pvIDX_XIndxRCheck1    ].bCommon=false ;
    Value[miIDX_XIndxR][pvIDX_XIndxRCheck2    ].sName = "IndxRCheck2    " ; Value[miIDX_XIndxR][pvIDX_XIndxRCheck2    ].bCommon=false ;
    Value[miIDX_XIndxR][pvIDX_XIndxRRotator   ].sName = "IndxRRotator   " ; Value[miIDX_XIndxR][pvIDX_XIndxRRotator   ].bCommon=false ;
    Value[miIDX_XIndxR][poIDX_XIndxRGet       ].sName = "Check Get Ofs  " ; Value[miIDX_XIndxR][poIDX_XIndxRGet       ].bCommon=false ; Value[miIDX_XIndxR][poIDX_XIndxRGet     ].iType = vtOffset ;

    // 14. miRTL_ZTrsfr
    Value[miRTL_ZTrsfr][pvRTL_ZTrsfrWait      ].sName = "TrsfrWait      " ; Value[miRTL_ZTrsfr][pvRTL_ZTrsfrWait      ].bCommon=true  ;
    Value[miRTL_ZTrsfr][pvRTL_ZTrsfrPick      ].sName = "TrsfrPick      " ; Value[miRTL_ZTrsfr][pvRTL_ZTrsfrPick      ].bCommon=true  ;
    Value[miRTL_ZTrsfr][pvRTL_ZTrsfrPlace     ].sName = "TrsfrPlace     " ; Value[miRTL_ZTrsfr][pvRTL_ZTrsfrPlace     ].bCommon=false ;

    // 15. miRTL_YConvs
    Value[miRTL_YConvs][pvRTL_YConvsWait      ].sName = "ConvsnWait     " ; Value[miRTL_YConvs][pvRTL_YConvsWait      ].bCommon=false ;
    Value[miRTL_YConvs][pvRTL_YConvsWork1st   ].sName = "ConvsnWork1st  " ; Value[miRTL_YConvs][pvRTL_YConvsWork1st   ].bCommon=false ;
    Value[miRTL_YConvs][pvRTL_YConvsWork2nd   ].sName = "ConvsnWork2nd  " ; Value[miRTL_YConvs][pvRTL_YConvsWork2nd   ].bCommon=false ;

    // 16. miIDX_TRotat
    Value[miIDX_TRotat][pvIDX_TRotatWait      ].sName = "RotatWait      " ; Value[miIDX_TRotat][pvIDX_TRotatWait      ].bCommon=true  ;
    Value[miIDX_TRotat][pvIDX_TRotatRotate    ].sName = "RotatRotate    " ; Value[miIDX_TRotat][pvIDX_TRotatRotate    ].bCommon=false ;
    Value[miIDX_TRotat][pvIDX_TRotatNormal    ].sName = "RotatNormal    " ; Value[miIDX_TRotat][pvIDX_TRotatNormal    ].bCommon=false ;

    // 17. miSTG_YConvs
    Value[miSTG_YConvs][pvSTG_YConvsWait      ].sName = "ConvsnWait     " ; Value[miSTG_YConvs][pvSTG_YConvsWait      ].bCommon=true  ;
    Value[miSTG_YConvs][pvSTG_YConvsWork      ].sName = "ConvsnWork     " ; Value[miSTG_YConvs][pvSTG_YConvsWork      ].bCommon=false ;

    // 18. miLDR_YConvR
    Value[miLDR_YConvR][pvLDR_YConvRWait      ].sName = "ConvnWait      " ; Value[miLDR_YConvR][pvLDR_YConvRWait      ].bCommon=true  ;
    Value[miLDR_YConvR][pvLDR_YConvRWork      ].sName = "ConvnWork      " ; Value[miLDR_YConvR][pvLDR_YConvRWork      ].bCommon=false ;

    // 19. miLDR_ZElevR
    Value[miLDR_ZElevR][pvLDR_ZElevRWait      ].sName = "ElevRWait      " ; Value[miLDR_ZElevR][pvLDR_ZElevRWait      ].bCommon=true  ;
    Value[miLDR_ZElevR][pvLDR_ZElevRPickFrst  ].sName = "ElevRPickFrst  " ; Value[miLDR_ZElevR][pvLDR_ZElevRPickFrst  ].bCommon=false ;
    Value[miLDR_ZElevR][pvLDR_ZElevRPlceFrst  ].sName = "ElevRPlceFrst  " ; Value[miLDR_ZElevR][pvLDR_ZElevRPlceFrst  ].bCommon=false ;
    Value[miLDR_ZElevR][pvLDR_ZElevRCheckFrst ].sName = "ElevRCheckFrst " ; Value[miLDR_ZElevR][pvLDR_ZElevRCheckFrst ].bCommon=false ;

    // 20. miLDR_ZElevF
    Value[miLDR_ZElevF][pvLDR_ZElevFWait      ].sName = "ElevFWait      " ; Value[miLDR_ZElevF][pvLDR_ZElevFWait      ].bCommon=true ;
    Value[miLDR_ZElevF][pvLDR_ZElevFPickFrst  ].sName = "ElevFPickFrst  " ; Value[miLDR_ZElevF][pvLDR_ZElevFPickFrst  ].bCommon=true ;

    // 21. miSpare1
//  Value[miSpare1    ][pvSpare1Wait          ].sName = "pvSpare1Wai t  " ; Value[miSpare1    ][pvSpare1Wait          ].bCommon=true ;

    // 22. miULD_ZElevt
    Value[miULD_ZElevt][pvULD_ZElevtWait      ].sName = "ElevtWait      " ; Value[miULD_ZElevt][pvULD_ZElevtWait      ].bCommon=true ;
    Value[miULD_ZElevt][pvULD_ZElevtPickFrst  ].sName = "ElevtPickFrst  " ; Value[miULD_ZElevt][pvULD_ZElevtPickFrst  ].bCommon=true ;
    Value[miULD_ZElevt][pvULD_ZElevtPlce      ].sName = "ElevtPlce      " ; Value[miULD_ZElevt][pvULD_ZElevtPlce      ].bCommon=true ;

    // 23. miSpare2
//  Value[miSpare2    ][pvSpare2Wait          ].sName = "pvSpare2Wai t  " ; Value[miSpare2    ][pvSpare2Wait          ].bCommon=true ;

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
         sTemp = String(MT_GetName(GridInfo.iGrid)) + " 모터를 "+ Value[GridInfo.iGrid][GridInfo.iRow].sName + "으로 이동합니까?" ;

         if(FM_MsgYesNo("Confirm",sTemp.c_str()) != mrYes) {
             GridInfo.bDown = true ;
             return ;
         }


         sTemp = " going to goto " + Value[GridInfo.iGrid][GridInfo.iRow].sName + " Pos" ;
         Trace(MT_GetName(GridInfo.iGrid),sTemp.c_str()); //pi 와 pv를 귀찮아서 그냥 같이 쓴다. pi pv할당할때 조심....

         if(GridInfo.iGrid == miLTL_XGenFt) LTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLTL_XGenRr) LTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLTL_YDispr) LTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLTL_ZDispr) LTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miRTL_XGenFt) RTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miRTL_XGenRr) RTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miRTL_YVisn ) RTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miRTL_ZVisn ) RTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLTL_YVisn ) LTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miRTL_YTrspr) RTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTG_XStage) STG  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTG_YStage) STG  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLTL_ZVisn ) LTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miIDX_XIndxR) IDX  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miRTL_ZTrsfr) RTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miRTL_YConvs) RTL  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miSTG_YConvs) STG  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miIDX_TRotat) IDX  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLDR_YConvR) LDR_R.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLDR_ZElevR) LDR_R.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miLDR_ZElevF) LDR_F.MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
//         if(GridInfo.iGrid == miSpare1    )       MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
         if(GridInfo.iGrid == miULD_ZElevt) ULD  .MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
//         if(GridInfo.iGrid == miSpare2    )       MoveMotr((EN_MOTR_ID)GridInfo.iGrid , (EN_PSTN_ID)GridInfo.iRow);
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




