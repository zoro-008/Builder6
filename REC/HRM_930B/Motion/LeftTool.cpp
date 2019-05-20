//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "LeftTool.h"
#include "RightTool.h"
#include "Stage.h"
#include "LoaderFront.h"
//---------------------------------------------------------------------------
#include "HexaPot.h"
#include "VisionCom.h"
#include "BaseMaths.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "UserFile.h"
#include "ManualMan.h"

#include "Rs232Keyence.h"
#include "Sequence.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CLeftTool LTL;

CLeftTool::CLeftTool(void)
{

}

CLeftTool::~CLeftTool (void)
{

}

void CLeftTool::Init()
{
    m_sPartName = "LeftTool ";

    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

    m_sCheckSafeMsg = "" ;

    m_bRqstPMSave   = false;
    m_iCrntDispNode = 0    ;
    m_iCrntUvNode   = 0    ;

    //MT_SetGantryEnable(miLTL_XGenRr , miLTL_XGenFt , 0 , 0.0 , 1.0);

}

void CLeftTool::Close()
{
    Load(false);
}

void CLeftTool::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

//마지막 엠티를 리턴.
bool CLeftTool::FindChip(int &r , int &c , EN_CHIP_STAT _iChip , EN_ARAY_ID _iId)
{
    if(_iId == riLDR_F) return LDR_F.FindChip(r , c , _iChip);
    else                return STG.FindChip  (r , c , _iChip);
}

void CLeftTool::SetCadOfsGain(EN_TOOL_ID _eTool , EN_COORDINATION _eCood)
{
    if(_eCood == cnStg) {
        //오른쪽 비젼입장에서 Check센서서 부터 스테이지 센터까지 거리.
        double dLVisnCheckToStgCntX = PM.GetValue(miRTL_XGenRr , pvRTL_XVisnStgCnt) - PM.GetValue(miRTL_XGenRr , pvRTL_XVisnCheck);  //+ 왼쪽에 있을때.
        double dLVisnCheckToStgCntY = PM.GetValue(miRTL_YVisn  , pvRTL_YVisnStgCnt) - PM.GetValue(miRTL_YVisn  , pvRTL_YVisnCheck);  //- 앞쪽에 있을때.

        double dXGain   , dYGain   ,dZGain  ;
        double dXOffset , dYOffset ,dZOffset;
        /*
        tlLVisn = 0,
        tlLHght ,
        tlLUV   ,
        tlLDspR ,
        tlLDspF ,
        tlRTrns ,
        tlRVisn ,
        tlRHght ,
        tlStage ,
        */
        if(_eTool == tlLHght){ // tlLHght
            Trace("SetCadOfsGain","tlLHght");
            dXGain  = 1 ;
            dYGain  = 1 ;
            dZGain  = 1 ;
            dXOffset = PM.GetValue(miLTL_XGenRr , pvLTL_XVisnHtCheck) - dLVisnCheckToStgCntX ;
            dYOffset = PM.GetValue(miLTL_YVisn  , pvLTL_YVisnHtCheck) - dLVisnCheckToStgCntY ;
            dZOffset = 0 ;
        }
        else if(_eTool == tlLVisn){ //tlLVisn
            Trace("SetCadOfsGain","tlLVisn");
            dXGain  = 1 ;
            dYGain  = 1 ;
            dZGain  = 1 ;
            dXOffset = PM.GetValue(miLTL_XGenRr , pvLTL_XVisnCheck  ) - dLVisnCheckToStgCntX ;
            dYOffset = PM.GetValue(miLTL_YVisn  , pvLTL_YVisnCheck  ) - dLVisnCheckToStgCntY ;
            dZOffset = 0 ;
        }
        else if(_eTool == tlLDspF){ //tlLDspF
            Trace("SetCadOfsGain","tlLDspF");
            dXGain  = 1 ;
            dYGain  =-1 ;
            dZGain  = 1 ;
            dXOffset = PM.GetValue(miLTL_XGenRr , pvLTL_XDispFtCheck) - dLVisnCheckToStgCntX ;
            dYOffset = PM.GetValue(miLTL_YDispr , pvLTL_YDispFtCheck) + dLVisnCheckToStgCntY ;
            dZOffset = PM.GetValue(miLTL_ZDispr , pvLTL_ZDispFtCheck) - STG.PkgStat.dMidTopHeight  - OM.DevOptn.dDispZOfs;

        }
        else if(_eTool == tlLDspR){ //tlLDspR
            Trace("SetCadOfsGain","tlLDspR");
            dXGain  = 1 ;
            dYGain  =-1 ;
            dZGain  = 1 ;
            dXOffset = PM.GetValue(miLTL_XGenRr , pvLTL_XDispRrCheck) - dLVisnCheckToStgCntX ;
            dYOffset = PM.GetValue(miLTL_YDispr , pvLTL_YDispRrCheck) + dLVisnCheckToStgCntY ;
            dZOffset = PM.GetValue(miLTL_ZDispr , pvLTL_ZDispRrCheck) - STG.PkgStat.dMidTopHeight  - OM.DevOptn.dDispZOfs;
        }
        else if(_eTool == tlLUV){ //tlLUV
            Trace("SetCadOfsGain","tlLUV");
            dXGain  = 1 ;
            dYGain  =-1 ;
            dZGain  = 1 ;
            dXOffset = PM.GetValue(miLTL_XGenRr , pvLTL_XDispUVCheck) - dLVisnCheckToStgCntX ;
            dYOffset = PM.GetValue(miLTL_YDispr , pvLTL_YDispUVCheck) + dLVisnCheckToStgCntY ;
            dZOffset = PM.GetValue(miLTL_ZDispr , pvLTL_ZDispUVWork ) - STG.PkgStat.dMidTopHeight  - OM.DevOptn.dDispZOfs;
            //dZOffset = 0 ;  //605.985 +
        }
        CAD.SetGainX(dXGain);
        CAD.SetGainY(dYGain);
        CAD.SetGainZ(dZGain);
        CAD.SetOffsetX(dXOffset);
        CAD.SetOffsetY(dYOffset);
        CAD.SetOffsetZ(dZOffset);
    }
    else {
        //왼쪽 비젼입장에서 Check센서서 부터 로테이터 센터까지 거리.
        double dLVisnCheckToRttCntX = PM.GetValue(miLTL_XGenRr , pvLTL_XVisnRttCnt) - PM.GetValue(miLTL_XGenRr , pvLTL_XVisnCheck);  //+ 왼쪽에 있을때.
        double dLVisnCheckToRttCntY = PM.GetValue(miLTL_YVisn  , pvLTL_YVisnRttCnt) - PM.GetValue(miLTL_YVisn  , pvLTL_YVisnCheck);  //- 앞쪽에 있을때.

        double dXGain   , dYGain   ,dZGain  ;
        double dXOffset , dYOffset ,dZOffset;

        if(_eTool == tlLHght){ // tlLHght
            Trace("SetCadOfsGain","tlLHght");
            dXGain  = 1 ;
            dYGain  = 1 ;
            dZGain  = 1 ;
            dXOffset = PM.GetValue(miLTL_XGenRr , pvLTL_XVisnHtCheck) + dLVisnCheckToRttCntX ;
            dYOffset = PM.GetValue(miLTL_YVisn  , pvLTL_YVisnHtCheck) + dLVisnCheckToRttCntY ;
            dZOffset = 0 ;
        }
        else if(_eTool == tlLVisn){ //tlLVisn
            Trace("SetCadOfsGain","tlLVisn");
            dXGain  = 1 ;
            dYGain  = 1 ;
            dZGain  = 1 ;
            dXOffset = PM.GetValue(miLTL_XGenRr , pvLTL_XVisnCheck  ) + dLVisnCheckToRttCntX ;
            dYOffset = PM.GetValue(miLTL_YVisn  , pvLTL_YVisnCheck  ) + dLVisnCheckToRttCntY ;
            dZOffset = 0 ;
        }
        else {
            Trace("SetCadOfsGain","Failed");
            return ;
        }
        CAD_Rtt.SetGainX(dXGain);
        CAD_Rtt.SetGainY(dYGain);
        CAD_Rtt.SetGainZ(dZGain);
        CAD_Rtt.SetOffsetX(dXOffset);
        CAD_Rtt.SetOffsetY(dYOffset);
        CAD_Rtt.SetOffsetZ(dZOffset);
    }
}

double CLeftTool::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dGripStt ;

    if(_iMotr == miLTL_XGenRr){
             if(_iPstnId == piLTL_XWait         ) dPos = PM.GetValue (_iMotr , pvLTL_XWait        );
        else if(_iPstnId == piLTL_XGripGrip1    ) dPos = PM.GetValue (_iMotr , pvLTL_XGripGrip1   );
        else if(_iPstnId == piLTL_XGripGrip2    ) dPos = PM.GetValue (_iMotr , pvLTL_XGripGrip2   );
        else if(_iPstnId == piLTL_XGripPull     ) dPos = PM.GetValue (_iMotr , pvLTL_XGripPull    );
        else if(_iPstnId == piLTL_XIndxBack     ) dPos = PM.GetValue (_iMotr , pvLTL_XIndxBack    );
        else if(_iPstnId == piLTL_XIndxWork     ) dPos = PM.GetValue (_iMotr , pvLTL_XIndxWork    );
        else if(_iPstnId == piLTL_XDispClean    ) dPos = PM.GetValue (_iMotr , pvLTL_XDispClean   );
        else if(_iPstnId == piLTL_XDispFtCheck  ) dPos = PM.GetValue (_iMotr , pvLTL_XDispFtCheck );
        else if(_iPstnId == piLTL_XDispRrCheck  ) dPos = PM.GetValue (_iMotr , pvLTL_XDispRrCheck );
        else if(_iPstnId == piLTL_XDispUVCheck  ) dPos = PM.GetValue (_iMotr , pvLTL_XDispUVCheck );
        else if(_iPstnId == piLTL_XVisnHtCheck  ) dPos = PM.GetValue (_iMotr , pvLTL_XVisnHtCheck );
        else if(_iPstnId == piLTL_XVisnCheck    ) dPos = PM.GetValue (_iMotr , pvLTL_XVisnCheck   );
        else if(_iPstnId == piLTL_XVisnCmsOri   ) dPos = PM.GetValue (_iMotr , pvLTL_XVisnCmsOri  );
        else if(_iPstnId == piLTL_XWorkOut      ) dPos = PM.GetValue (_iMotr , pvLTL_XWorkOut     );
        else if(_iPstnId == piLTL_XVisnRttCnt   ) dPos = PM.GetValue (_iMotr , pvLTL_XVisnRttCnt  );
    }
    else if(_iMotr == miLTL_YDispr){
             if(_iPstnId == piLTL_YDispWait     ) dPos = PM.GetValue (_iMotr , pvLTL_YDispWait    );
        else if(_iPstnId == piLTL_YDispClean    ) dPos = PM.GetValue (_iMotr , pvLTL_YDispClean   );
        else if(_iPstnId == piLTL_YDispFtCheck  ) dPos = PM.GetValue (_iMotr , pvLTL_YDispFtCheck );
        else if(_iPstnId == piLTL_YDispRrCheck  ) dPos = PM.GetValue (_iMotr , pvLTL_YDispRrCheck );
        else if(_iPstnId == piLTL_YDispUVCheck  ) dPos = PM.GetValue (_iMotr , pvLTL_YDispUVCheck );

    }
    else if(_iMotr == miLTL_YVisn){
             if(_iPstnId == piLTL_YVisnWait     ) dPos = PM.GetValue (_iMotr , pvLTL_YVisnWait    );
        else if(_iPstnId == piLTL_YVisnHtCheck  ) dPos = PM.GetValue (_iMotr , pvLTL_YVisnHtCheck );
        else if(_iPstnId == piLTL_YVisnCheck    ) dPos = PM.GetValue (_iMotr , pvLTL_YVisnCheck   );
        else if(_iPstnId == piLTL_YVisnCmsOri   ) dPos = PM.GetValue (_iMotr , pvLTL_YVisnCmsOri  );
        else if(_iPstnId == piLTL_YVisnRttCnt   ) dPos = PM.GetValue (_iMotr , pvLTL_YVisnRttCnt  ); 

    }
    else if(_iMotr == miLTL_ZDispr){
             if(_iPstnId == piLTL_ZDispWait     ) dPos = PM.GetValue (_iMotr , pvLTL_ZDispWait    );
        else if(_iPstnId == piLTL_ZDispFtCheck  ) dPos = PM.GetValue (_iMotr , pvLTL_ZDispFtCheck );
        else if(_iPstnId == piLTL_ZDispRrCheck  ) dPos = PM.GetValue (_iMotr , pvLTL_ZDispRrCheck );
        else if(_iPstnId == piLTL_ZDispUVWork   ) dPos = PM.GetValue (_iMotr , pvLTL_ZDispUVWork  );
        else if(_iPstnId == piLTL_ZDispClean    ) dPos = PM.GetValue (_iMotr , pvLTL_ZDispClean   );
        else if(_iPstnId == piLTL_ZDispUVTest   ) dPos = PM.GetValue (_iMotr , piLTL_ZDispUVTest  );
    }
    else if(_iMotr == miLTL_ZVisn){
             if(_iPstnId == piLTL_ZVisnWait     ) dPos = PM.GetValue (_iMotr , pvLTL_ZVisnWait    );
        else if(_iPstnId == piLTL_ZVisnCheck    ) dPos = PM.GetValue (_iMotr , pvLTL_ZVisnCheck   );
        else if(_iPstnId == piLTL_ZVisnMid      ) dPos = PM.GetValue (_iMotr , pvLTL_ZVisnMid     );
        else if(_iPstnId == piLTL_ZVisnCmsOri   ) dPos = PM.GetValue (_iMotr , pvLTL_ZVisnCmsOri  );
        else if(_iPstnId == piLTL_ZVisnCmsAlign ) dPos = PM.GetValue (_iMotr , pvLTL_ZVisnCmsAlign);
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CLeftTool::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CLeftTool::Update()
{
    if(Step.iSeq == scIdle){
        if(!MT_GetStop(miLTL_XGenRr) && (IO_GetXUp(xLTL_WrkPshSnsr) && AT_GetCmd(aiLTL_WrkPshDnUp))){
            MT_Stop(miLTL_XGenRr);
            EM_SetErrMsg(eiPRT_OverLoad , "OverLoad Sensing!");
        }
    }


}

bool CLeftTool::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //IO_SetY(yLTL_Dispr1OnOff , false);
    //IO_SetY(yLTL_Dispr2OnOff , false);
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLeftTool::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLeftTool::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: if(!IO_GetX(xLTL_GriperBw) && AT_GetCmd(aiLTL_GriperFwBW)==ccBwd) {
                     EM_SetErrMsg(eiPRT_Crash , "미들블럭 그리퍼가 Bwd되어있지 않습니다.");
                     Step.iToStart = 0 ;
                     return true ;
                 }

                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);

                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);

                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);
                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 if(IO_GetX(xLTL_GriperBw)){MoveMotr(miLTL_XGenRr , piLTL_XWait    );}

                 MoveMotr(miLTL_YDispr , piLTL_YDispWait);
                 MoveMotr(miLTL_YVisn  , piLTL_YVisnWait);

                 Step.iToStart++ ;
                 return false ;

        case 12: if(!MoveMotr(miLTL_XGenRr , piLTL_XWait    )) return false ;

                 if(!MoveMotr(miLTL_YDispr , piLTL_YDispWait)) return false ;
                 if(!MoveMotr(miLTL_YVisn  , piLTL_YVisnWait)) return false ;


                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLeftTool::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: IO_SetY(yLTL_UVHumeSuction , false );
                 IO_SetY(yLTL_Dispr1OnOff , false);
                 IO_SetY(yLTL_Dispr2OnOff , false);

                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);

                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);
                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);   //&& AT_GetCmd(aiLTL_GriperFwBW)==ccBwd

                 if(IO_GetX(xLTL_GriperBw)) {MoveActr(aiLTL_MGrperDnUp , ccBwd);}



                 Step.iToStop++;

                 return false ;
        case 11: if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;
                 if(!AT_Done (aiLTL_MGrperDnUp        )) return false ;

                 //서프라이 하다가 마그네틱 떨어졌을때는 그자리에 있는다.
                 //다시 대기 위치로 가다가 처박음.
                 if(IO_GetX(xLTL_GriperBw)){MoveMotr(miLTL_XGenRr , piLTL_XWait    );}

                 MoveMotr(miLTL_YDispr , piLTL_YDispWait);
                 MoveMotr(miLTL_YVisn  , piLTL_YVisnWait);

                 Step.iToStop ++ ;
                 return false ;

        case 12: if(IO_GetX(xLTL_GriperBw) && !MoveMotr(miLTL_XGenRr , piLTL_XWait    )) return false ;
                 if(!MoveMotr(miLTL_YDispr , piLTL_YDispWait)) return false ;
                 if(!MoveMotr(miLTL_YVisn  , piLTL_YVisnWait)) return false ;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLeftTool::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == scIdle) {
        if (Stat.bReqStop)return false ;
        if( DM.ARAY[riSTG].CheckAllStat(csNone) &&  IO_GetX(xSTG_Detect4)) {EM_SetErrMsg(eiPKG_Unknwn , "Stage Unknwn PKG Error."   ); return false;}   

        //일단 셋업할때 빼자.
        //if(!DM.ARAY[riSTG].CheckAllStat(csNone) && !IO_GetX(xSTG_Detect4)) {EM_SetErrMsg(eiPKG_Dispr  , "Stage Disappear PKG Error."); return false;}


        //if( DM.ARAY[riPRL].CheckAllStat(csNone) &&  IO_GetX(xPRL_Detect2)) {EM_SetErrMsg(eiPKG_Unknwn , "Loader Front Unknwn PKG Error."   ); return false;}
        //if(!DM.ARAY[riPRL].CheckAllStat(csNone) && !IO_GetX(xPRL_Detect2)) {EM_SetErrMsg(eiPKG_Dispr  , "Loader Front Disappear PKG Error."); return false;}



        //높이측정만 하는 모드일경우.
        //bool isCycleHeightCheck = DM.ARAY[riRTT].CheckAllStat(csUnkwn) && OM.CmnOptn.iWorkMode == wmHeight;

        if( DM.ARAY[riPRL].CheckAllStat(csNone) &&  IO_GetX(xPRL_Detect2)) {EM_SetErrMsg(eiPKG_Unknwn , "프리레일 알수없는 자제 감지");   return false;}
        if(!DM.ARAY[riPRL].CheckAllStat(csNone) && !IO_GetX(xPRL_Detect2)) {EM_SetErrMsg(eiPKG_Dispr  , "프리레일 자제 사라짐"       ); return false;}


        //if(RTL.GetSeqStep() != CRightTool::scIdle) return false ;


        
        bool isCycleMidSupply = !OM.CmnOptn.bFLoadingStop &&
                                MT_CmprPos(miLDR_ZElevF , LDR_F.GetMotrPos(miLDR_ZElevF , piLDR_ZElevFPick )) &&
                               (RTL.GetSeqStep() == CRightTool::scIdle || RTL.GetSeqStep() == CRightTool::scOut)&&
                                DM.ARAY[riPTL].CheckAllStat(csNone  )&&
                                DM.ARAY[riWKO].CheckAllStat(csNone  )&&
                                DM.ARAY[riSTG].CheckAllStat(csNone  )&&
                                DM.ARAY[riPRL].CheckAllStat(csNone  )&&
                                DM.ARAY[riLDR_F].GetCntStat(csUnkwn )&&
                               !DM.ARAY[riLDR_F].GetCntStat(csDetect)&&LDR_F.GetSeqStep() == CLoaderFront::scIdle ;
        bool isCycleMidToWork = RTL.GetSeqStep() == CRightTool::scIdle && DM.ARAY[riSTG].CheckAllStat(csNone )&& DM.ARAY[riPRL  ].GetCntStat(csUnkwn) ;
        //bool isCycleClean     =


        //false ; //나중에 만들자.

        //bool bEpoxyFirstDisp    = OM.DevOptn.bUseFstDisp && (DM.ARAY[riSTG].GetCntStat(csCmsLDisp1)+DM.ARAY[riSTG].GetCntStat(csMidREndHeight) == OM.DevInfo.iMidColCnt * OM.DevInfo.iMidRowCnt) ;
        //bool bNoEpoxyFirstDisp  = OM.DevOptn.bUseSecDisp ;
        //
        bool bNeedDisp = (DM.ARAY[riSTG].GetCntStat(csCmsLDisp1)+ DM.ARAY[riSTG].GetCntStat(csCmsLDisp2)+DM.ARAY[riSTG].GetCntStat(csMidREndHeight) == OM.DevInfo.iMidColCnt * OM.DevInfo.iMidRowCnt) ; //!DM.ARAY[riSTG].GetCntStat(csCmsRNeed);

        bool isCycleCmsLHeight= RTL.GetSeqStep() == CRightTool::scIdle && OM.CmnOptn.iWorkMode == wmHeight && DM.ARAY[riRTT].GetCntStat(csCmsLHeight );

        bool isCycleMidLOri   = RTL.GetSeqStep() == CRightTool::scIdle && DM.ARAY[riSTG].GetCntStat  (csMidLOri  );
        bool isCycleMidLAlign = RTL.GetSeqStep() == CRightTool::scIdle && DM.ARAY[riSTG].GetCntStat  (csMidLAlign);
        bool isCycleCmsLAlign = RTL.GetSeqStep() == CRightTool::scIdle && DM.ARAY[riSTG].GetCntStat  (csCmsLAlign);
        bool isCycleCmsLTopUV = RTL.GetSeqStep() == CRightTool::scIdle && (DM.ARAY[riSTG].GetCntStat  (csCmsLTopUV1) || DM.ARAY[riSTG].GetCntStat  (csCmsLTopUV2));
        bool isCycleCmsLDisp  = RTL.GetSeqStep() == CRightTool::scIdle && (DM.ARAY[riSTG].GetCntStat  (csCmsLDisp1 ) || DM.ARAY[riSTG].GetCntStat(csCmsLDisp2))  && bNeedDisp ;//&& (bEpoxyFirstDisp || bNoEpoxyFirstDisp); //20160510 진섭
        bool isCycleWorkOut   = RTL.GetSeqStep() == CRightTool::scIdle && DM.ARAY[riSTG].CheckAllStat(csWorkEnd  );
        bool isCycleCmsLOri   = RTL.GetSeqStep() == CRightTool::scIdle && DM.ARAY[riRTT].GetCntStat  (csCmsLOri  );




        bool isCycleToolVoid  = Stat.bReqToolVoid ;

        bool isCycleEnd       = DM.ARAY[riSTG].CheckAllStat(csNone )&&
                                DM.ARAY[riRTT].CheckAllStat(csNone )&&
                                DM.ARAY[riPRL].CheckAllStat(csNone );

        //여기부터 조건 잡자.
        if(EM_IsErr()) return false ;
         //Normal Decide Step.
             if (isCycleCmsLHeight ) {Trace(m_sPartName.c_str(),"CycleCmdLHeightStt"); Step.iSeq = scCmsLHeight; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleMidSupply  ) {Trace(m_sPartName.c_str(),"CycleMidSupply Stt"); Step.iSeq = scMidSupply ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleMidToWork  ) {Trace(m_sPartName.c_str(),"CycleMidToWork Stt"); Step.iSeq = scMidToWork ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        //else if (isCycleClean      ) {Trace(m_sPartName.c_str(),"CycleClean     Stt"); Step.iSeq = scClean     ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleMidLOri    ) {Trace(m_sPartName.c_str(),"CycleMidLOri   Stt"); Step.iSeq = scMidLOri   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleMidLAlign  ) {Trace(m_sPartName.c_str(),"CycleMidLAlign Stt"); Step.iSeq = scMidLAlign ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleCmsLTopUV  ) {Trace(m_sPartName.c_str(),"CycleCmsLTopUV Stt"); Step.iSeq = scCmsLTopUV ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleCmsLAlign  ) {Trace(m_sPartName.c_str(),"CycleCmsLAlign Stt"); Step.iSeq = scCmsLAlign ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleCmsLDisp   ) {Trace(m_sPartName.c_str(),"CycleCmsLDisp  Stt"); Step.iSeq = scCmsLDisp  ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleWorkOut    ) {Trace(m_sPartName.c_str(),"CycleWorkOut   Stt"); Step.iSeq = scWorkOut   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleToolVoid   ) {Trace(m_sPartName.c_str(),"CycleToolVoid  Stt"); Step.iSeq = scToolVoid  ; InitCycleStep(); m_iCycleSttTime=GetTime();}
        else if (isCycleCmsLOri    ) {Trace(m_sPartName.c_str(),"CycleCmsLOri   Stt"); Step.iSeq = scCmsLOri   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd        ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    Step.iLastSeq = Step.iSeq ;
    switch (Step.iSeq) {
        default          :                        Trace(m_sPartName.c_str(),"default         End");                                                   Step.iSeq = scIdle ;  return false ;
        case scIdle      :                                                                                                                                                  return false ;
        case scCmsLHeight: if(CycleCmsLHeight()){ Trace(m_sPartName.c_str(),"CycleCmsLHeight End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scMidSupply : if(CycleMidSupply ()){ Trace(m_sPartName.c_str(),"CycleMidSupply  End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scMidToWork : if(CycleMidToWork ()){ Trace(m_sPartName.c_str(),"CycleMidToWork  End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
      //case scClean     : if(CycleClean     ()){ Trace(m_sPartName.c_str(),"CycleClean      End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scMidLOri   : if(CycleMidLOri   ()){ Trace(m_sPartName.c_str(),"CycleMidLOri    End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scMidLAlign : if(CycleMidLAlign ()){ Trace(m_sPartName.c_str(),"CycleMidLAlign  End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scCmsLTopUV : if(CycleTopLUV    ()){ Trace(m_sPartName.c_str(),"CycleTopLUV     End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scCmsLDisp  : if(CycleCmsLDisp  ()){ Trace(m_sPartName.c_str(),"CycleCmsLDisp   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scCmsLAlign : if(CycleCmsLAlign ()){ Trace(m_sPartName.c_str(),"CycleCmsLAlign  End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scCmsLOri   : if(CycleCmsLOri   ()){ Trace(m_sPartName.c_str(),"CycleCmsLOri    End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scWorkOut   : if(CycleWorkOut   ()){ Trace(m_sPartName.c_str(),"CycleWorkOut    End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scToolVoid  : if(CycleToolVoid  ()){ Trace(m_sPartName.c_str(),"CycleToolVoid   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }
    return false ;
}

bool CLeftTool::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iHome=%02d" , __FUNC__ , Step.iHome );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiPRT_HomeTo,sTemp.c_str());
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    if(Stat.bReqStop) {
        //Step.iHome = 0;
        //return true ;
    }

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 Step.iHome++;
                 return false;

        case 11: if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false;

                 MoveActr(aiLTL_GriperFwBW , ccBwd);
                 Step.iHome++;
                 return false;

        case 12: if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;


                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);


                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);


                 Step.iHome++;
                 return false ;

        case 13: if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 //Sensor확인 에러.
                 if(IO_GetX(xLTL_GrprSnsr)){
                     EM_SetErrMsg(eiPRT_HomeTo, "Gripper Sensor가 감지 됩니다.");
                     Step.iHome = 0;
                     return true;
                 }







                 MT_DoHome(miLTL_ZDispr);
                 MT_DoHome(miLTL_ZVisn );

                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miLTL_ZDispr))return false ;
                 if(!MT_GetHomeEnd(miLTL_ZVisn ))return false ;


                 Step.iHome++;
                 return false;

        case 15: //if(!MT_GetHomeEnd(miLTL_XGenRr)) return false ;
                 MT_DoHome(miLTL_XGenRr);
                 MT_DoHome(miLTL_YDispr);
                 MT_DoHome(miLTL_YVisn );
                 Step.iHome++;
                 return false ;

        case 16: //if(!MT_GetHomeResult(miLTL_XGenRr)) return false ;
                 if(!MT_GetHomeEnd(miLTL_XGenRr)) return false ;
                 if(!MT_GetHomeEnd(miLTL_YDispr)) return false ;
                 if(!MT_GetHomeEnd(miLTL_YVisn )) return false ;
                 //Gantry하구 난 후에 강제로 켜줌.
                 MT_SetHomeEnd(miLTL_XGenFt , true);
                 Step.iHome++;
                 return false;

        case 17: MT_GoAbsMan(miLTL_XGenRr , PM.GetValue(miLTL_XGenRr , pvLTL_XWait     ));
                 MT_GoAbsMan(miLTL_YDispr , PM.GetValue(miLTL_YDispr , pvLTL_YDispClean));
                 MT_GoAbsMan(miLTL_YVisn  , PM.GetValue(miLTL_YVisn  , pvLTL_YVisnWait ));
                 Step.iHome++;
                 return false;

        case 18: if(!MT_GoAbsMan(miLTL_XGenRr , PM.GetValue(miLTL_XGenRr , pvLTL_XWait))) return false ;
                 if(!MT_GoAbsMan(miLTL_YDispr , PM.GetValue(miLTL_YDispr , pvLTL_YDispClean))) return false ;
                 if(!MT_GoAbsMan(miLTL_YVisn  , PM.GetValue(miLTL_YVisn  , pvLTL_YVisnWait))) return false ;

                 MT_GoAbsMan(miLTL_ZDispr , PM.GetValue(miLTL_ZDispr , pvLTL_ZDispClean));
                 MT_GoAbsMan(miLTL_ZVisn  , PM.GetValue(miLTL_ZVisn  , pvLTL_ZVisnWait ));
                 Step.iHome++;
                 return false;


        case 19: if(!MT_GoAbsMan(miLTL_ZDispr , PM.GetValue(miLTL_ZDispr , pvLTL_ZDispClean))) return false ;
                 if(!MT_GoAbsMan(miLTL_ZVisn  , PM.GetValue(miLTL_ZVisn  , pvLTL_ZVisnWait ))) return false ;

                 Step.iHome = 0;
                 return true ;

    }
}

bool CLeftTool::CycleMidSupply(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }

    int r,c ;
    FindChip(r,c,csUnkwn,riLDR_F);
    int iRc = OM.DevInfo.iLDRFMgzBayCnt - c - 1;
    String sMidID ;

    String TraceTemp ;


    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: if(RTL.GetSeqStep() != CRightTool::scOut){
                     RTL.ToolVoid(true);
                 }
                 HexaPot.MoveAbsRes(paX, HEX_X_WAIT);
                 HexaPot.MoveAbsRes(paY, HEX_Y_WAIT);
                 HexaPot.MoveAbsRes(paZ, HEX_Z_WAIT);
                 HexaPot.MoveAbsRes(paU, HEX_U_WAIT);
                 HexaPot.MoveAbsRes(paV, HEX_V_WAIT);
                 HexaPot.MoveAbsRes(paW, HEX_W_WAIT);
                 HexaPot.Move();
                 MoveActr(aiLTL_GriperFwBW , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 11: if(RTL.GetSeqStep() != CRightTool::scOut){
                     if(!RTL.ToolVoid()) return false ;
                 }
                 if(!HexaPot.GetStopInpos()) return false ;
                 if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);

                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);



                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 MoveMotr(miLTL_XGenRr , piLTL_XWait);
                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_XGenRr , piLTL_XWait)) return false ;
                 MoveActr(aiLTL_MGrperDnUp , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveActr(aiLTL_MGrperDnUp , ccFwd)) return false ;
                 if(iRc == 0) MoveActr(aiLTL_GriperFwBW , ccBwd);
                 if(iRc == 1) MoveActr(aiLTL_GriperFwBW , ccFwd);
                 Step.iCycle++;
                 return false;

        case 15: if(iRc == 0 && !MoveActr(aiLTL_GriperFwBW , ccBwd))return false;
                 if(iRc == 1 && !MoveActr(aiLTL_GriperFwBW , ccFwd))return false;
                 if(iRc == 0) MoveMotr(miLTL_XGenRr , piLTL_XGripGrip1);
                 if(iRc == 1) MoveMotr(miLTL_XGenRr , piLTL_XGripGrip2);
                 Step.iCycle++;
                 return false ;

        case 16: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!IO_GetX(xLTL_GrprSnsr)){//얼래 없네..
                     Step.iCycle = 50 ;
                     return false ;
                 }

                 MT_GoIncRun(miLDR_ZElevF , -0.3); //1mm 낮춰서. 집는다.
                 Step.iCycle++;
                 return false ;

        case 17: if(!MT_GetStopInpos(miLDR_ZElevF)) return false ;
                 MoveActr(aiLTL_SGrperDnUp , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 18: if(!MoveActr(aiLTL_SGrperDnUp , ccFwd)) return false ;
                 MoveActr(aiLTL_GriperFwBW , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 19: if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 MoveMotr(miLTL_XGenRr , piLTL_XGripPull , true);
                 Step.iCycle++;
                 return false ;

        case 20: if(!IO_GetX(xLTL_GriperBw)){//실린더 마그넷 떨어졌네.
                     MoveActr(aiLTL_SGrperDnUp , ccBwd);
                 }
                 if(!IO_GetX(xLTL_GrprSnsr)){//얼래 놓쳤네.
                     MoveActr(aiLTL_SGrperDnUp , ccBwd);
                 }

                 if(!MoveMotr(miLTL_XGenRr , piLTL_XGripPull , true)) return false ;
                 if(!IO_GetX(xLTL_GrprSnsr)){//얼래 놓쳤네.
                     Step.iCycle = 60 ;
                     return false ;
                 }
                 if(!IO_GetX(xLTL_GriperBw)){//실린더 마그넷 떨어졌네.
                     Step.iCycle = 60 ;
                     return false ;
                 }
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);


                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                 MT_GoIncRun(miLTL_XGenRr , 70.0); //치수상. 36.7mm 인데 혹시 끌려올까봐 더 간다.
                 Step.iCycle++;
                 return false ;

        case 22: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 23: if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 sMidID = DM.ARAY[riLDR_F].CHPS[r][c].GetID();
                 DM.ARAY[riLDR_F].SetStat(r,c,csEmpty);
                 DM.ARAY[riLDR_F].CHPS[r][c].SetID("");

                 DM.ARAY[riPRL  ].SetStat(csUnkwn    );
                 DM.ARAY[riPRL  ].CHPS[0][0].SetID(sMidID);
                 DM.ARAY[riPRL  ].SetID(sMidID);

                 Step.iCycle=0;
                 return true ;





        //예외상황.
        //로더에 찝으러 갔더니 없음...
        //다시 해야함JS.
        case 50 : MoveActr(aiLTL_GriperFwBW , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 51 : if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                  MoveMotr(miLTL_XGenRr , piLTL_XWait);
                  Step.iCycle++;
                  return false;

        case 52 : if(!MoveMotr(miLTL_XGenRr , piLTL_XWait)) return false;
                  MoveActr(aiLTL_MGrperDnUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 53 : if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                  //MoveMotr(miLTL_XGenRr , piLTL_XWait);
                  Step.iCycle++;
                  return false ;

        case 54 : //if(!MoveMotr(miLTL_XGenRr , piLTL_XWait)) return false ;
                  MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 55 : if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                  EM_SetErrMsg(eiPRT_Missed , "그리퍼센서가 로더자제를 감지 하지 못하였습니다.");
                  Step.iCycle=0;
                  return true ;


        //로더에서 찝고 나오면서 놓침.
        case 60 : MoveActr(aiLTL_SGrperDnUp , ccBwd);
                  //MoveActr(aiLTL_GriperFwBW , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 61 : if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                  //if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                  MT_GoIncRun(miLTL_XGenRr , 130.0);
                  Step.iCycle++;
                  return false ;

        case 62 : if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                  MoveActr(aiLTL_MGrperDnUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 63 : if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                  MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 64 : if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                  sMidID = DM.ARAY[riLDR_F].CHPS[r][c].GetID();
                  DM.ARAY[riLDR_F].SetStat(r,c,csEmpty);
                  DM.ARAY[riLDR_F].CHPS[r][c].SetID("");

                  DM.ARAY[riPRL  ].SetStat(csUnkwn    );
                  DM.ARAY[riPRL  ].CHPS[0][0].SetID(sMidID);
                  DM.ARAY[riPRL  ].SetID(sMidID);

                  TraceTemp = DM.ARAY[riPRL].GetStat(r,c);
                  Trace("riPRL Aray", TraceTemp.c_str());
                  if(!IO_GetX(xLTL_GriperBw)){
                      EM_SetErrMsg(eiPRT_Missed , "그리퍼 전후진 실린더 후진이 되지 않았습니다. 실린더를 확인해주세요.");
                  }
                  if(!IO_GetX(xLTL_GrprSnsr)){
                      EM_SetErrMsg(eiPRT_Missed , "그리퍼가 미들블럭을 놓쳤습니다. 프리버퍼 감지 센서 위치에 넣어주세요.");
                  }
                  Step.iCycle=0;
                  return true ;
    }
}

bool CLeftTool::CycleMidToWork()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }

    int r,c ;
    FindChip(r,c,csUnkwn,riLDR_F);
    String sMidID ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: HexaPot.MoveAbsRes(paX, HEX_X_WAIT);
                 HexaPot.MoveAbsRes(paY, HEX_Y_WAIT);
                 HexaPot.MoveAbsRes(paZ, HEX_Z_WAIT);
                 HexaPot.MoveAbsRes(paU, HEX_U_WAIT);
                 HexaPot.MoveAbsRes(paV, HEX_V_WAIT);
                 HexaPot.MoveAbsRes(paW, HEX_W_WAIT);
                 HexaPot.Move();
                 RTL.ToolVoid(true);
                 MoveActr(aiLTL_GriperFwBW , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 11: if(!RTL.ToolVoid()) return false ;
                 if(!HexaPot.GetStopInpos()) return false ;
                 if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;

                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);


                 MoveActr(aiSTG_Fix1stFwBw , ccBwd);
                 MoveActr(aiSTG_Fix2ndFwBw , ccBwd);
                 MoveActr(aiSTG_Fix3rdFwBw , ccBwd);
                 MoveActr(aiSTG_Fix4thFwBw , ccBwd);
                 MoveActr(aiSTG_Fix5thFwBw , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;

                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 
                 if(!MoveActr(aiSTG_Fix1stFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix2ndFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix3rdFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix4thFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix5thFwBw , ccBwd)) return false ;


                 MT_GoAbsRun(miLTL_XGenRr , PM.GetValue(miLTL_XGenRr , pvLTL_XIndxBack)+10.0);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 MoveMotr(miLTL_XGenRr , piLTL_XIndxBack , true);
                 Step.iCycle++;
                 return false ;

        case 14: if(IO_GetX(xLTL_WrkPshSnsr)){//얼레 인덱스 내리는 구간에 머가 있네??
                     EM_SetErrMsg(eiPRT_Detect , "왼쪽툴 인덱스 다운구간에 물체가 감지되었습니다.");
                     Step.iCycle=0;
                     return true ;
                 }
                 if(!MoveMotr(miLTL_XGenRr , piLTL_XIndxBack , true)) return false ;
                 MoveActr(aiLTL_WrkPshDnUp , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 15: if(!MoveActr(aiLTL_WrkPshDnUp , ccFwd)) return false ;
                 MoveMotr(miLTL_XGenRr , piLTL_XIndxWork , true);
                 Step.iCycle++;
                 return false ;

        case 16: if(IO_GetX(xLTL_WrkPshSnsr)){//오바로드다~~~
                     MT_Stop(miLTL_XGenRr);
                     Step.iCycle = 50 ;
                     return false ;
                 }
                 if(!MoveMotr(miLTL_XGenRr , piLTL_XIndxWork , true)) return false ;
                 if(OM.DevOptn.bUseSTG_Fix1)MoveActr(aiSTG_Fix1stFwBw , ccFwd);
                 if(OM.DevOptn.bUseSTG_Fix2)MoveActr(aiSTG_Fix2ndFwBw , ccFwd);
                 if(OM.DevOptn.bUseSTG_Fix3)MoveActr(aiSTG_Fix3rdFwBw , ccFwd);
                 if(OM.DevOptn.bUseSTG_Fix4)MoveActr(aiSTG_Fix4thFwBw , ccFwd);
                 if(OM.DevOptn.bUseSTG_Fix5)MoveActr(aiSTG_Fix5thFwBw , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 17: if(!AT_Done(aiSTG_Fix1stFwBw)) return false ;
                 if(!AT_Done(aiSTG_Fix2ndFwBw)) return false ;
                 if(!AT_Done(aiSTG_Fix3rdFwBw)) return false ;
                 if(!AT_Done(aiSTG_Fix4thFwBw)) return false ;
                 if(!AT_Done(aiSTG_Fix5thFwBw)) return false ;

                 MoveActr(aiSTG_Fix1stFwBw , ccBwd);
                 MoveActr(aiSTG_Fix2ndFwBw , ccBwd);
                 MoveActr(aiSTG_Fix3rdFwBw , ccBwd);
                 MoveActr(aiSTG_Fix4thFwBw , ccBwd);
                 MoveActr(aiSTG_Fix5thFwBw , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 18: if(!MoveActr(aiSTG_Fix1stFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix2ndFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix3rdFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix4thFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix5thFwBw , ccBwd)) return false ;

                 if(OM.DevOptn.bUseSTG_Fix1)MoveActr(aiSTG_Fix1stFwBw , ccFwd);
                 if(OM.DevOptn.bUseSTG_Fix2)MoveActr(aiSTG_Fix2ndFwBw , ccFwd);
                 if(OM.DevOptn.bUseSTG_Fix3)MoveActr(aiSTG_Fix3rdFwBw , ccFwd);
                 if(OM.DevOptn.bUseSTG_Fix4)MoveActr(aiSTG_Fix4thFwBw , ccFwd);
                 if(OM.DevOptn.bUseSTG_Fix5)MoveActr(aiSTG_Fix5thFwBw , ccFwd);

                 Step.iCycle++;
                 return false ;

        case 19: if(!AT_Done(aiSTG_Fix1stFwBw)) return false ;
                 if(!AT_Done(aiSTG_Fix2ndFwBw)) return false ;
                 if(!AT_Done(aiSTG_Fix3rdFwBw)) return false ;
                 if(!AT_Done(aiSTG_Fix4thFwBw)) return false ;
                 if(!AT_Done(aiSTG_Fix5thFwBw)) return false ;

                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 20: if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;


                 sMidID = DM.ARAY[riPRL  ].GetID();
                 DM.ARAY[riSTG].SetStat(csMidLOri);
                 DM.ARAY[riSTG].SetID  (sMidID   );
                 DM.ARAY[riPRL].ClearMap();

                 STG.Spc.Clear();
                 STG.Spc.dWorkSttTime = Now().Val ;

                 Step.iCycle=0;
                 return true ;

        //예외상황.
        //오버로드.
        case 50 : MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 51 : if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                  MoveMotr(miLTL_XGenRr , piLTL_XIndxBack);
                  Step.iCycle++;
                  return false ;

        case 52 : if(!MoveMotr(miLTL_XGenRr , piLTL_XIndxBack)) return false ;
                  EM_SetErrMsg(eiPRT_OverLoad , "왼쪽툴 인덱스 오버로드 감지. 걸림을 확인해주세요.");
                  Step.iCycle=0;
                  return true ;
    }
}

bool CLeftTool::CycleMidLOri()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }

    static CCadVisnPoint * pCadVisnPoint;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10:
                 WorkInfo.eStat     = csMidRHeight ;
                 WorkInfo.iRow      = 0 ;
                 WorkInfo.iCol      = 0 ;

                 pCadVisnPoint = CAD.MB.MB_LOri ;
                 SetCadOfsGain(tlLVisn );
                 MoveActr(aiLTL_GriperFwBW , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 11: RTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!RTL.ToolVoid()) return false ;
                 if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);

                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;

                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 MoveMotr(miLTL_YDispr , piLTL_YDispWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miLTL_YDispr , piLTL_YDispWait)) return false ;

                 Step.iCycle = 20 ;
                 return false ;





        case 20: MT_GoAbsRun(miLTL_XGenRr , pCadVisnPoint->GetCntrPosX());
                 MT_GoAbsRun(miLTL_YVisn  , pCadVisnPoint->GetCntrPosY());

                 if(!VSN_L.SendCommand("MBO")){//미들블럭 오리엔테이션 으로 세팅.
                     EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 21: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YVisn )) return false ;
                 if(!VSN_L.GetSendCommandEnd()) return false ;

                 MoveMotr(miLTL_ZVisn , piLTL_ZVisnMid);
                 Step.iCycle++;
                 return false ;

        case 22: if(!MoveMotr(miLTL_ZVisn , piLTL_ZVisnMid)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 23: if(!m_tmDelay.OnDelay(true , 100)) return false ;

                 if(!VSN_L.SendInsp()){
		     EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");
		     Step.iCycle=0;
		     return true ;
		 }
                 Step.iCycle++;
                 return false ;

        case 24: if(!VSN_L.GetSendInspEnd()) return false ;
                 if(!VSN_L.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "미들블럭 Orientation 비젼 에러");
                     Step.iCycle=0 ;
                     return true ;
                 }
                 MoveMotr(miLTL_ZVisn , piLTL_ZVisnWait);
                 Step.iCycle++;
                 return false ;

        case 25: if(!MoveMotr(miLTL_ZVisn , piLTL_ZVisnWait)) return false ;

                 DM.ARAY[riSTG].SetStat(csMidLAlign);

                 Step.iCycle=0;
                 return true ;
    }
}

bool CLeftTool::CycleMidLAlign()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }

    double dOriAngle  ;
    double dInspAngle ;

    static CCadVisnPoint * pCadVisnPointM ;
    static CCadVisnPoint * pCadVisnPointS ;




    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: 
                 WorkInfo.eStat = csMidRHeight ;
                 WorkInfo.iRow  = 0 ;
                 WorkInfo.iCol  = 0 ;

                 pCadVisnPointM = CAD.MB.MB_LAlnM ;
                 pCadVisnPointS = CAD.MB.MB_LAlnS ;
                 SetCadOfsGain(tlLVisn );
                 MoveActr(aiLTL_GriperFwBW , ccBwd);


                 Step.iCycle++;
                 return false ;

        case 11: RTL.ToolVoid(true);
                 if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 Step.iCycle++;
                 return false ;

        case 12: if(!RTL.ToolVoid()) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);

                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);


                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;

                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;


                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 MoveMotr(miLTL_YDispr , piLTL_YDispWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miLTL_YDispr , piLTL_YDispWait)) return false ;

                 Step.iCycle = 20 ;
                 return false ;



        case 20: if(!VSN_L.SendCommand("MBA")){ //미들블럭 얼라인  으로 세팅.
                     EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");
                     Step.iCycle=0;
                     return true ;
                 }

                 MT_GoAbsRun(miLTL_XGenRr , pCadVisnPointM->GetCntrPosX());
                 MT_GoAbsRun(miLTL_YVisn  , pCadVisnPointM->GetCntrPosY());
                 Step.iCycle++;
                 return false ;

        case 21: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YVisn )) return false ;
                 if(!VSN_L.GetSendCommandEnd()) return false ;

                 MoveMotr(miLTL_ZVisn , piLTL_ZVisnMid);
                 Step.iCycle++;
                 return false ;

        case 22: if(!MoveMotr(miLTL_ZVisn , piLTL_ZVisnMid)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 23: if(!m_tmDelay.OnDelay(true , 100)) return false ;
                 if(!VSN_L.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 24: if(!VSN_L.GetSendInspEnd()) return false ;
                 //if(!VSN_L.GetInspOk()){
                 //    EM_SetErrMsg(eiVSN_InspNG , "미들블럭 Main Align 비젼 검사실패");
                 //    Step.iCycle=0 ;
                 //    return true ;
                 //}
                 SetCadOfsGain(tlLVisn );
                 MT_GoAbsRun(miLTL_XGenRr , pCadVisnPointS->GetCntrPosX());
                 MT_GoAbsRun(miLTL_YVisn  , pCadVisnPointS->GetCntrPosY());
                 Step.iCycle++;
                 return false ;

        case 25: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YVisn )) return false ;
                 //if(!VSN_L.GetSendCommandEnd()) return false ;
                 MoveMotr(miLTL_ZVisn , piLTL_ZVisnMid);
                 Step.iCycle++;
                 return false ;

        case 26: if(!MoveMotr(miLTL_ZVisn , piLTL_ZVisnMid)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 27: if(!m_tmDelay.OnDelay(true , 100)) return false ;
                 if(!VSN_L.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");  
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 28: if(!VSN_L.GetSendInspEnd()) return false ;
                 if(!VSN_L.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "미들블럭 Sub Align 비젼 검사실패");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 if(!VSN_L.LoadAlignRslt()){
                     EM_SetErrMsg(eiVSN_InspNG , "미들블럭 Align 결과값 로딩 실패");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 

                 dOriAngle  = CMath::GetLineAngle(pCadVisnPointM->GetCntrPosX(),
                                                  pCadVisnPointM->GetCntrPosY(),
                                                  pCadVisnPointS->GetCntrPosX(),
                                                  pCadVisnPointS->GetCntrPosY());

                 dInspAngle = CMath::GetLineAngle(pCadVisnPointM->GetCntrPosX()+VSN_L.GetAlignRslt().dMainX ,
                                                  pCadVisnPointM->GetCntrPosY()+VSN_L.GetAlignRslt().dMainY ,
                                                  pCadVisnPointS->GetCntrPosX()+VSN_L.GetAlignRslt().dSubX  ,
                                                  pCadVisnPointS->GetCntrPosY()+VSN_L.GetAlignRslt().dSubY  );

                 g_tMidPosOfs.dX = VSN_L.GetAlignRslt().dMainX ;
                 g_tMidPosOfs.dY = VSN_L.GetAlignRslt().dMainY ;
                 g_tMidPosOfs.dT = dInspAngle - dOriAngle;


                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);


                 Step.iCycle++;
                 return false ;

        case 29: if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 MoveMotr(miLTL_YVisn , piLTL_YVisnWait);
                 MoveMotr(miLTL_XGenRr , piLTL_XDispClean);

                 Step.iCycle++;
                 return false;

        case 30: if(!MoveMotr(miLTL_YVisn , piLTL_YVisnWait)) return false;
                 if(!MoveMotr(miLTL_XGenRr , piLTL_XDispClean)) return false ;
                 MoveMotr(miLTL_YDispr , piLTL_YDispClean);
                 Step.iCycle++;
                 return false ;

        case 31: if(!MoveMotr(miLTL_YDispr , piLTL_YDispClean)) return false ;
                 Step.iCycle++ ;
                 return false ;

        case 32:

                 DM.ARAY[riSTG].SetStat(csMidRHeight);

                 Step.iCycle=0;
                 return true ;
    }
}




bool CLeftTool::CycleCmsLDisp(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        IO_SetY(yLTL_Dispr1OnOff , false);
        IO_SetY(yLTL_Dispr2OnOff , false);
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }


    const int iSincMotrMax = 3 ;
    static int iMotorAxis[iSincMotrMax] ; // 보간 운동 모터 축 설정
    double dMotorPos [iSincMotrMax] ; // 보간 운동 모터 포지션 X,Y,Z
    const int iCoodination = 0 ;
    static int * pDispCnt ;
    static int iDispCntLmt ;
    static EN_ACTR_ID aiLTL_DisprDnUp ;
    static EN_OUTPUT_ID yLTL_DisprOnOff ;
    static CCadDspCmd * pCadDspCmd ;
    const int iUseDispFrnt = 0 ;//옵션이 콤보박스로 되어 있음.
    const int iUseDispRear = 1 ;

    //Cycle.
    switch (Step.iCycle) {
        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iCycle = 0 ;
                 return true ;


        case 10: //Disp1 , Disp2 자연순간 같이 쓰면 같이 나오는 조건이 있는데 그때 2번부터 진행 하기 위해.
                 if(DM.ARAY[riSTG].GetCntStat(csCmsLDisp2))WorkInfo.eStat = csCmsLDisp2 ;
                 else                                      WorkInfo.eStat = csCmsLDisp1 ;

                 //해당 칩찾기.
                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG)){
                     Step.iCycle = 0 ;
                     return true ;
                 }

                 if(WorkInfo.eStat == csCmsLDisp1){//첫번째 디스펜싱 일때(자연경화)
                     if(!OM.DevOptn.bUseFstDisp) {//안쓰면 그냥 다음 상태로 넘김.
                         DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsLTopUV1  );
                         Step.iCycle = 0 ;
                         return true ;
                     }

                     pCadDspCmd  = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LDsp1 ;

                     //노드 갯수 확인.
                     if(pCadDspCmd->GetNodeCnt() <= 1){
                         DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsLTopUV1   );
                         Step.iCycle = 0 ;
                         return true ;
                     }


                     if(OM.DevOptn.iFstDisp == iUseDispFrnt){//Front 사용.
                         pDispCnt    = &m_iDispCntF ;
                         iDispCntLmt = OM.DevOptn.iEpoxyDispCntF ;
                         aiLTL_DisprDnUp = aiLTL_DisprFDnUp ;
                         yLTL_DisprOnOff = yLTL_Dispr1OnOff ;
                         SetCadOfsGain(tlLDspF );
                     }
                     else { //Rear 사용.iUseDispRear
                         pDispCnt    = &m_iDispCntR ;
                         iDispCntLmt = OM.DevOptn.iEpoxyDispCntR ;
                         aiLTL_DisprDnUp = aiLTL_DisprRDnUp ;
                         yLTL_DisprOnOff = yLTL_Dispr2OnOff ;
                         SetCadOfsGain(tlLDspR );
                     }
                 }
                 else if(WorkInfo.eStat == csCmsLDisp2){ //두번째디스펜싱 조건(순간경화)
                     if(!OM.DevOptn.bUseSecDisp) {
                         DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsREndAlign);
                         Step.iCycle = 0 ;
                         return true ;
                     }
                     pCadDspCmd  = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LDsp2 ;

                     //노드 갯수 확인.
                     if(pCadDspCmd->GetNodeCnt() <= 1){
                         DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsREndAlign);
                         Step.iCycle = 0 ;
                         return true ;
                     }

                     if(OM.DevOptn.iSecDisp == iUseDispFrnt){//Front 사용.
                         pDispCnt    = &m_iDispCntF ;
                         iDispCntLmt = OM.DevOptn.iEpoxyDispCntF ;
                         aiLTL_DisprDnUp = aiLTL_DisprFDnUp ;
                         yLTL_DisprOnOff = yLTL_Dispr1OnOff ;
                         SetCadOfsGain(tlLDspF );
                     }
                     else { //Rear 사용.
                         pDispCnt    = &m_iDispCntR ;
                         iDispCntLmt = OM.DevOptn.iEpoxyDispCntR ;
                         aiLTL_DisprDnUp = aiLTL_DisprRDnUp ;
                         yLTL_DisprOnOff = yLTL_Dispr2OnOff ;
                         SetCadOfsGain(tlLDspR );
                     }
                 }

                 //에폭시 카운팅 에러.
                 if(*pDispCnt >= iDispCntLmt) {
                     if(aiLTL_DisprDnUp = aiLTL_DisprFDnUp){
                         EM_SetErrMsg (eiLTL_Disp , "프론트 에폭시 잔량 부족 에러");
                     }
                     else {
                         EM_SetErrMsg (eiLTL_Disp , "리어 에폭시 잔량 부족 에러");
                     }
                     Step.iCycle = 0;
                     return true;
                 }










                 //여기부터 ===============================================================


                 MoveActr(aiLTL_GriperFwBW , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 11: RTL.ToolVoid(true);
                 if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 Step.iCycle++;
                 return false ;

        case 12: if(!RTL.ToolVoid()) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);

                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);


                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;

                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 if(OM.CmnOptn.iDisprClnTime == 0) {
                     Step.iCycle=19;
                     return false ;
                 }

                 MoveMotr(miLTL_YVisn  , piLTL_YVisnWait ) ;
                 MoveMotr(miLTL_YDispr , piLTL_YDispClean) ;
                 MoveMotr(miLTL_XGenRr , piLTL_XDispClean) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miLTL_YVisn  , piLTL_YVisnWait )) return false ;
                 if(!MoveMotr(miLTL_YDispr , piLTL_YDispClean)) return false ;
                 if(!MoveMotr(miLTL_XGenRr , piLTL_XDispClean)) return false ;

                 MoveActr(aiLTL_DisprFDnUp , ccFwd) ;
                 MoveActr(aiLTL_DisprRDnUp , ccFwd) ;
                 Step.iCycle++;
                 return false ;

        case 15: if(!MoveActr(aiLTL_DisprFDnUp , ccFwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccFwd)) return false ;

                 MoveMotr(miLTL_ZDispr , piLTL_ZDispClean);
                 Step.iCycle++;
                 return false;

        case 16: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispClean)) return false;
                 //Clean하자!!!!
                 IO_SetY(yLTL_CleanOn , true);
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

                 //옵션 추가.
        case 17: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iDisprClnTime));
                 IO_SetY(yLTL_CleanOn , false);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd) ;
                 MoveActr(aiLTL_DisprRDnUp , ccBwd) ;


                 Step.iCycle++;
                 return false;

        case 18: if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;

                 Step.iCycle++;
                 return false ;

        //위에서 씀.
        case 19: MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);

                 Step.iCycle++;
                 return false;

        case 20: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;

                 Step.iCycle ++ ;
                 return false ;


        case 21: //     if((DM.ARAY[riSTG].GetCntStat(csCmsLDisp1) && OM.DevOptn.bUseFstDisp   && OM.DevOptn.iFstDisp == 0) ||
                 //        (DM.ARAY[riSTG].GetCntStat(csCmsLDisp2) && OM.DevOptn.bUseSecDisp   && OM.DevOptn.iSecDisp == 0) ) SetCadOfsGain(tlLDspF );
                 //else if((DM.ARAY[riSTG].GetCntStat(csCmsLDisp1) && OM.DevOptn.bUseFstDisp   && OM.DevOptn.iFstDisp == 1) ||
                 //        (DM.ARAY[riSTG].GetCntStat(csCmsLDisp2) && OM.DevOptn.bUseSecDisp   && OM.DevOptn.iSecDisp == 1) ) SetCadOfsGain(tlLDspR );

                 MT_GoAbsRun(miLTL_XGenRr , pCadDspCmd->GetPosX(0)+g_tMidPosOfs.dX);
                 MT_GoAbsRun(miLTL_YDispr , pCadDspCmd->GetPosY(0)-g_tMidPosOfs.dY);

                 Step.iCycle++;
                 return false ;

        case 22: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YDispr)) return false ;

                 MoveActr(aiLTL_DisprDnUp , ccFwd);

                 Step.iCycle++;
                 return false ;

        case 23: if(!AT_Done(aiLTL_DisprDnUp)) return false ;
                 Step.iCycle++;
                 return false ;

        case 24: MT_GoAbsRun(miLTL_ZDispr , pCadDspCmd->GetPosZ(0));
                 Step.iCycle++;
                 return false ;

        case 25: if(!MT_GetStopInpos(miLTL_ZDispr))return false ;
                 m_iCrntDispNode = 0 ;

                 //직선 보간 맵핑 설정.

                 iMotorAxis[0] = miLTL_XGenRr ;
                 iMotorAxis[1] = miLTL_YDispr ;
                 iMotorAxis[2] = miLTL_ZDispr ;
                 //0은 코디네이션 번호.
                 MT_ContiSetAxisMap(iCoodination, iSincMotrMax, iMotorAxis) ;
                 MT_ContiSetAbsRelMode(iCoodination, 0) ; //0:절대모드 1:상대모드
                 m_iCrntDispNode++;
                 (*pDispCnt)++;

                 Step.iCycle++;                    //+g_tMidPosOfs.dX
                 return false ;                    //+g_tMidPosOfs.dY

        //밑에서 씀.
        case 26: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YVisn )) return false ;
                 if(!MT_GetStopInpos(miLTL_ZDispr)) return false ;

                 dMotorPos[0] = pCadDspCmd->GetPosX(m_iCrntDispNode)+g_tMidPosOfs.dX;
                 dMotorPos[1] = pCadDspCmd->GetPosY(m_iCrntDispNode)-g_tMidPosOfs.dY;
                 dMotorPos[2] = pCadDspCmd->GetPosZ(m_iCrntDispNode) ;

                 Trace((String("DispPntX") + m_iCrntDispNode).c_str(),String(dMotorPos[0]).c_str());
                 Trace((String("DispPntY") + m_iCrntDispNode).c_str(),String(dMotorPos[1]).c_str());
                 Trace((String("DispPntZ") + m_iCrntDispNode).c_str(),String(dMotorPos[2]).c_str());

                 //해당 노드의 디스펜싱 온을 가져와서 맞게 토출.
                 IO_SetY(yLTL_DisprOnOff , pCadDspCmd->GetDispOn(m_iCrntDispNode));

                 MT_LineMove(iCoodination ,
                             iMotorAxis   ,
                             dMotorPos    ,
                             pCadDspCmd->GetSpeed(m_iCrntDispNode) ,
                             OM.DevOptn.dDispAccDcc ,
                             OM.DevOptn.dDispAccDcc ,
                             iSincMotrMax);

                 Step.iCycle++;
                 return false ;

        case 27: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YDispr)) return false ;
                 if(!MT_GetStopInpos(miLTL_ZDispr)) return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 28: if(!m_tmDelay.OnDelay(true , pCadDspCmd->GetDelay(m_iCrntDispNode)))return false ;

                 m_iCrntDispNode++;
                 sTemp = sTemp.sprintf("m_iCrntDispNode = %d" , m_iCrntDispNode);
                 Trace(m_sPartName.c_str(), sTemp.c_str());
                 sTemp = MT_GetTrgPos(miLTL_XGenRr);
                 Trace(m_sPartName.c_str(), sTemp.c_str());
                 sTemp = MT_GetTrgPos(miLTL_YDispr);
                 Trace(m_sPartName.c_str(), sTemp.c_str());
                 if(m_iCrntDispNode < pCadDspCmd->GetNodeCnt()){
                     Step.iCycle = 26 ;
                     return false ;
                 }

                 //해당 노드의 디스펜싱 토출 오프
                 IO_SetY(yLTL_DisprOnOff , false);

                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 Step.iCycle ++ ;
                 return false ;

        case 29: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(WorkInfo.eStat == csCmsLDisp1){
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsLTopUV1  );
                 }
                 else {
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsREndAlign);
                 }

                 Step.iCycle = 0 ;
                 return true ;
    }
}


bool CLeftTool::CycleTopLUV()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        IO_SetY(yLTL_UVHumeSuction , false );
        //sunsun 대겸 UV 끄기.
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }


    const int iSincMotrMax = 3 ;
    static int iMotorAxis[iSincMotrMax] ; // 보간 운동 모터 축 설정
    double dMotorPos [iSincMotrMax] ; // 보간 운동 모터 포지션 X,Y,Z
    const int iCoodination = 0 ;
    //static bool bFstDisp = false;

    //const int iUvCnt = 5 ;
    static int iUvRunCnt = 0 ;

    static CCadDspCmd * pCadDspCmd ;//= CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LDsp1;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: if(DM.ARAY[riSTG].GetCntStat(csCmsLTopUV1))WorkInfo.eStat = csCmsLTopUV1 ;
                 else                                       WorkInfo.eStat = csCmsLTopUV2 ;

                 //해당 칩찾기.
                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG)){
                     Step.iCycle = 0 ;
                     return true ;
                 }


                 if(WorkInfo.eStat == csCmsLTopUV1){//첫번째 디스펜싱 일때(자연경화)
                     if(!OM.DevOptn.bUseFstDisp) {
                         DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRNeed  );

                         //순간 경화만 쓰는 자제가 정전이 나서 m_iLastUVSttTime의 시간이 너무 옛날로 되어 있는경우.
                         //순간경화 하고 FixDelay 10초로 설정 되어 있는데 한참 걸리게 되어 마치 홀드되는 현상때문에.
                         //순간 경화만 쓰는 제품은 일단 m_iLastUVSttTime를 세팅 한다.

                         //아직왜 그런지 모르지만 자꾸 오랫만에하면 자연경화 방식에서 병신짓을 함. 클리어가 안됌.
                         Trace(" m_iLastUVSttTime bf" , String(m_iLastUVSttTime).c_str());
                         m_iLastUVSttTime = GetTime() ;
                         Trace(" m_iLastUVSttTime at" , String(m_iLastUVSttTime).c_str());
                         m_iLastUVSttTime = GetTime() ;
                         Trace(" m_iLastUVSttTime at2" , String(m_iLastUVSttTime).c_str());

                         Step.iCycle = 0 ;
                         return true ;
                     }

                     pCadDspCmd = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LDsp1 ;
                     if(pCadDspCmd->GetNodeCnt() <= 1){
                         DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRNeed  );
                         Step.iCycle = 0 ;
                         return true ;
                     }
                 }
                 else {
                     if(!OM.DevOptn.bUseSecDisp) {
                         DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsFix  );
                         Step.iCycle = 0 ;
                         return true ;
                     }
                     pCadDspCmd = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LDsp2 ;
                     if(pCadDspCmd->GetNodeCnt() <= 1){
                         DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsFix  );
                         Step.iCycle = 0 ;
                         return true ;
                     }
                 }

                 //UV 사용시간 초과 에러 추가 진섭
                 if(Rs232UV.GetTopUvUseTime_ms()/(1000 * 60 * 60) >= OM.CmnOptn.iTopUVLimitTime) {
                     EM_SetErrMsg (eiLTL_UVUseTime , "Top UV 사용 시간 초과 에러");
                     Step.iCycle = 0;
                     return true;
                 }

                 SetCadOfsGain(tlLUV );
                 MoveActr(aiLTL_GriperFwBW , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 11: RTL.ToolVoid(true);
                 if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 Step.iCycle++;
                 return false ;

        case 12: if(!RTL.ToolVoid()) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 MoveMotr(miLTL_YVisn , piLTL_YVisnWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miLTL_YVisn , piLTL_YVisnWait)) return false ;

                 iUvRunCnt = 0 ;

                 Step.iCycle = 20 ;
                 return false ;

        //밑에서씀.
        //위에서씀.
        case 20: MT_GoAbsRun(miLTL_XGenRr , pCadDspCmd->GetPosX(0)+g_tMidPosOfs.dX);
                 MT_GoAbsRun(miLTL_YDispr , pCadDspCmd->GetPosY(0)-g_tMidPosOfs.dY);

                 Step.iCycle++;
                 return false ;

        case 21: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YDispr)) return false ;

                 MoveActr(aiLTL_UVCureDnUp , ccFwd);

                 Step.iCycle++;
                 return false ;

        case 22: if(!AT_Done(aiLTL_UVCureDnUp)) return false ;

                 Step.iCycle++;
                 return false ;

        case 23: MoveMotr(miLTL_ZDispr , piLTL_ZDispUVWork);
                 Step.iCycle++;
                 return false ;

        case 24: if(!MT_GetStopInpos(miLTL_ZDispr))return false ;

                 //조사 여러번 할때중에 처음에 하는거고 자연경화 케이스에만 가져간다.


                 if(WorkInfo.eStat == csCmsLTopUV1 && iUvRunCnt == 0) {
                     //아직왜 그런지 모르지만 자꾸 오랫만에하면 자연경화 방식에서 병신짓을 함. 클리어가 안됌.
                     Trace("m_iLastUVSttTime bf" , String(m_iLastUVSttTime).c_str());
                     m_iLastUVSttTime = GetTime() ;
                     Trace("m_iLastUVSttTime at" , String(m_iLastUVSttTime).c_str());

                     m_iLastUVCol     = WorkInfo.iCol ;
                     m_iLastUVRow     = WorkInfo.iRow ;
                 }







                 m_iCrntUvNode = 0 ;

                 //직선 보간 맵핑 설정.
                 iMotorAxis[0] = miLTL_XGenRr ;
                 iMotorAxis[1] = miLTL_YDispr ;
                 iMotorAxis[2] = miLTL_ZDispr ;// PM.GetValue(miLTL_ZDispr , pvLTL_ZDispUVWork) ;
                 //0은 코디네이션 번호.
                 MT_ContiSetAxisMap   (iCoodination, iSincMotrMax, iMotorAxis) ;
                 MT_ContiSetAbsRelMode(iCoodination, 0) ; //0:절대모드 1:상대모드
                 m_iCrntUvNode++;
                 //UV 광량으로 조사 하자!

                 //UV 광량 셋팅.
                 Rs232UV.SetCuring(uvCh5, OM.DevOptn.iUVTopBrightness);

                 Step.iCycle++;
                 return false ;

        //밑에서 씀.
        case 25: dMotorPos[0] = pCadDspCmd->GetPosX(m_iCrntUvNode)+g_tMidPosOfs.dX;
                 dMotorPos[1] = pCadDspCmd->GetPosY(m_iCrntUvNode)-g_tMidPosOfs.dY;
                 dMotorPos[2] = PM.GetValue(miLTL_ZDispr , pvLTL_ZDispUVWork);

                 //UV 켜기.
                 //Rs232UV.SetCuringOnOff(uvCh5, true );
                 Rs232UV.SetCuringOnOff(uvCh5, pCadDspCmd->GetDispOn(m_iCrntUvNode));

                 m_iLastTopUVSttTime = GetTime() ;

                 IO_SetY(yLTL_UVHumeSuction , true );
                 MT_LineMove(iCoodination ,
                             iMotorAxis   ,
                             dMotorPos    ,
                             pCadDspCmd->GetUVSpeed(m_iCrntUvNode),
                             OM.DevOptn.dDispAccDcc ,
                             OM.DevOptn.dDispAccDcc ,
                             iSincMotrMax);

                 Step.iCycle++;
                 return false ;

        case 26: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YDispr)) return false ;
                 if(!MT_GetStopInpos(miLTL_ZDispr)) return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 27: if(!m_tmDelay.OnDelay(true , pCadDspCmd->GetDelay(m_iCrntUvNode)))return false ;
                 m_iCrntUvNode++;
                 if(m_iCrntUvNode < pCadDspCmd->GetNodeCnt()){
                     Step.iCycle = 25 ;
                     return false ;
                 }

                 iUvRunCnt++;//3회 조사한다.
                 if(WorkInfo.eStat == csCmsLTopUV1){//첫번째 디스펜싱 일때(자연경화)
                     if(OM.DevOptn.iUvCntFrst > iUvRunCnt){
                         SetCadOfsGain(tlLUV );
                         dMotorPos[0] = pCadDspCmd->GetPosX(0)+g_tMidPosOfs.dX;
                         dMotorPos[1] = pCadDspCmd->GetPosY(0)-g_tMidPosOfs.dY;
                         dMotorPos[2] = PM.GetValue(miLTL_ZDispr , pvLTL_ZDispUVWork);

                         MT_LineMove(iCoodination ,
                                     iMotorAxis   ,
                                     dMotorPos    ,
                                     pCadDspCmd->GetUVSpeed(0) ,
                                     OM.DevOptn.dDispAccDcc ,
                                     OM.DevOptn.dDispAccDcc ,
                                     iSincMotrMax);
                         Step.iCycle = 20 ;
                         return false ;

                     }
                 }
                 else {
                     if(OM.DevOptn.iUvCntScnd > iUvRunCnt){
                         SetCadOfsGain(tlLUV );
                         dMotorPos[0] = pCadDspCmd->GetPosX(0)+g_tMidPosOfs.dX;
                         dMotorPos[1] = pCadDspCmd->GetPosY(0)-g_tMidPosOfs.dY;
                         dMotorPos[2] = PM.GetValue(miLTL_ZDispr , pvLTL_ZDispUVWork);

                         MT_LineMove(iCoodination ,
                                     iMotorAxis   ,
                                     dMotorPos    ,
                                     pCadDspCmd->GetUVSpeed(0) ,
                                     OM.DevOptn.dDispAccDcc ,
                                     OM.DevOptn.dDispAccDcc ,
                                     iSincMotrMax);
                         Step.iCycle = 20 ;
                         return false ;
                     }
                 }

                 //UV 끄기.
                 Rs232UV.SetCuringOnOff(uvCh5, false);

                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle ++;
                 return false ;

        case 28: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 IO_SetY(yLTL_UVHumeSuction , false );

                 if(WorkInfo.eStat == csCmsLTopUV1){
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRNeed  );
                 }
                 else {
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsFix);
                 }

                 Step.iCycle = 0 ;
                 return true ;
    }
}

bool CLeftTool::CycleCmsLAlign()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }

    double dOriAngle  ;
    double dInspAngle ;
    String sInspId    ;

    double dTempX  ;
    double dTempY  ;

    static double dVisnX  ;
    static double dVisnY  ;
    static double dVisnT  ;

    static double dMoveX  ;
    static double dMoveY  ;

    static double dVisn_HexaCntrOfsX ;
    static double dVisn_HexaCntrOfsY ;

    static int iAlignInspCnt = 0;

    static CCadVisnPoint * pCadVisnMain ;
    static CCadVisnPoint * pCadVisnSub  ;
    static CCadRect      * pCadRectCms  ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: 
                 WorkInfo.eStat     = csCmsLAlign ;
                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG)){
                     Step.iCycle = 0 ;
                     return true ;

                 }
                 //CM_LAlnM_01
                 pCadVisnMain = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnM ;
                 pCadVisnSub  = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnS ;
                 pCadRectCms  = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_Rect  ;
                 SetCadOfsGain(tlLVisn );
                 MoveActr(aiLTL_GriperFwBW , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 11: RTL.ToolVoid(true);
                 if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 Step.iCycle++;
                 return false ;

        case 12: if(!RTL.ToolVoid()) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);


                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 MoveMotr(miLTL_YDispr , piLTL_YDispWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miLTL_YDispr , piLTL_YDispWait)) return false ;

                 Step.iCycle = 20 ;
                 return false ;




        case 20: //sInspId = String("CMA_") + WorkInfo.iCol + WorkInfo.iRow ;
                 //if(!VSN_L.SendCommand(sInspId)){ //시모스  얼라인  으로 세팅.
                 //    EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");
                 //    Step.iCycle=0;
                 //    return true ;
                 //}

                 //포지션.

                 dTempX = pCadVisnMain -> GetCntrPosX() + g_tMidPosOfs.dX;
                 dTempY = pCadVisnMain -> GetCntrPosY() + g_tMidPosOfs.dY;
                 if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 MT_GoAbsRun(miLTL_XGenRr , dTempX);
                 MT_GoAbsRun(miLTL_YVisn  , dTempY);

                 HexaPot.MoveAbsRes(paZ , HEX_Z_LORI);
                 HexaPot.Move();

                 iAlignInspCnt = 0 ;

                 Step.iCycle++;
                 return false ;

        //밑에서씀.
        case 21: if(!HexaPot.GetStopInpos()) return false ;

                 sInspId = String("CMA_") + WorkInfo.iCol + WorkInfo.iRow ;
                 if(!VSN_L.SendCommand(sInspId)){ //시모스  얼라인  으로 세팅.
                     EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 22: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YVisn )) return false ;

                 MoveMotr(miLTL_ZVisn , piLTL_ZVisnCmsAlign);
                 Step.iCycle++;
                 return false ;

        case 23: if(!MoveMotr(miLTL_ZVisn , piLTL_ZVisnCmsAlign)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 24: if(!m_tmDelay.OnDelay(true , 100)) return false ;
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnCmsAlign);
                 Step.iCycle++;
                 return false ;

        case 25: if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnCmsAlign)) return false ;
                 if(!VSN_L.GetSendCommandEnd()) return false ;

                 if(!VSN_L.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 26: if(!VSN_L.GetSendInspEnd()) return false ;
                 //if(!VSN_L.GetInspOk()){
                 //    EM_SetErrMsg(eiVSN_InspNG , "Left 시모스 Main Align 비젼 검사실패");
                 //    Step.iCycle=0 ;
                 //    return true ;
                 //}

                 dTempX = MT_GetCmdPos(miLTL_XGenRr) ;
                 dTempY = MT_GetCmdPos(miLTL_YVisn ) ;
                 dVisn_HexaCntrOfsX = GetCrntStgCntVisnPosX() - dTempX ;
                 dVisn_HexaCntrOfsY = GetCrntStgCntVisnPosY() - dTempY ;



                 dTempX = pCadVisnSub->GetCntrPosX() + g_tMidPosOfs.dX;
                 dTempY = pCadVisnSub->GetCntrPosY() + g_tMidPosOfs.dY;
                 if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 MT_GoAbsRun(miLTL_XGenRr , dTempX);
                 MT_GoAbsRun(miLTL_YVisn  , dTempY);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnCmsAlign);

                 Step.iCycle++;
                 return false ;

        case 27: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YVisn )) return false ;
                 //if(!VSN_L.GetSendCommandEnd()) return false ;

                 if(!VSN_L.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 28: if(!VSN_L.GetSendInspEnd()) return false ;
                 if(!VSN_L.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "Left 시모스 Sub Align 비젼 검사실패");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 if(!VSN_L.LoadAlignRslt()){
                     EM_SetErrMsg(eiVSN_InspNG , "Left 시모스  Align 결과값 로딩 실패");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 dOriAngle  = CMath::GetLineAngle(CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnM->GetCntrPosX(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnM->GetCntrPosY(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnS->GetCntrPosX(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnS->GetCntrPosY());

                 dInspAngle = CMath::GetLineAngle(CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnM->GetCntrPosX()+VSN_L.GetAlignRslt().dMainX ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnM->GetCntrPosY()+VSN_L.GetAlignRslt().dMainY ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnS->GetCntrPosX()+VSN_L.GetAlignRslt().dSubX  ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnS->GetCntrPosY()+VSN_L.GetAlignRslt().dSubY  );

                 //왼쪽에서 오른쪽으로 가는 각도는 1도 + 일때는 1 1도- 일때는 359도 이렇게 표현되서 바꿈
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;

                 dVisnX = VSN_L.GetAlignRslt().dMainX ; //물체가 왼쪽에 가있으면 음수.
                 dVisnY = VSN_L.GetAlignRslt().dMainY ; //물체가 뒤쪽에 가있으면 음수
                 dVisnT = dInspAngle - dOriAngle; //시계방향이 양수.


                 if(fabs(dVisnX) > OM.DevOptn.dLVisnTolXY){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Left Cmos Align 비젼의 X보정값이 설정범위를 넘었습니다.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 if(fabs(dVisnY) > OM.DevOptn.dLVisnTolXY){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Left Cmos Align 비젼의 Y보정값이 설정범위를 넘었습니다.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 if(fabs(dVisnT) > OM.DevOptn.dLVisnTolAng){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Left Cmos Align 비젼의 T보정값이 설정범위를 넘었습니다.");
                     Step.iCycle = 0 ;
                     return true ;
                 }


                 iAlignInspCnt++;

                 //디버깅용.
                 dTempX = pCadVisnMain->GetCntrPosX() ;
                 dTempY = pCadVisnMain->GetCntrPosY() ;
                 dTempX = pCadRectCms ->GetCntrPosX() ;
                 dTempY = pCadRectCms ->GetCntrPosY() ;

                 //현재 위치에서 회전 중점.
                 //왼쪽 비젼 4사분면사용.
                 //스테이지 XY 4사분면사용.

                 /*
                 void CStage::GetRotPntFromCntrPnt(double _dX     , double _dY     , double _dAng , //입력 x 왼쪽이-, 입력 y위쪽이- , 돌릴각도. , +가 반시계방향.
                                                   double _dCntrX , double _dCntrY ,                //돌리는 기준점 x , y
                                                   double *_pRetX , double *_pRetY )                //결과값 x,y
                 {
                     double dRadT =  _dAng*3.14159265358979323846/180.0;
                     double x = _dX - _dCntrX;
                     double y = _dY - _dCntrY;
                     *_pRetX = x*cos(dRadT) - y*sin(dRadT) ;
                     *_pRetY = x*sin(dRadT) + y*cos(dRadT) ;
                     *_pRetX += _dCntrX ;
                     *_pRetY += _dCntrY ;
                 */


                 //한번 검사하고 멈추는 옵션.
                 if(OM.CmnOptn.bCheckVisnPos && !Stat.bMsgChecked)
                 {
                     Stat.bMsgChecked  = true ;
                     Stat.bShowVisnMsg = true ;
                     SEQ ._bBtnStop    = true ;
                 }

                 //계산상의 좌표는 4사분면을 쓴다.
                 //비전 결가값 4사분면이기 때문..
                 //상대값을 찾기위해 돌리 포인트를 0,0으로 넣고  현재 위치에서 스테이지 센터
                 //헥사포트는 1사분면.
                 STG.GetRotPntFromCntrPnt( dVisnX ,
                                           dVisnY ,
                                           dVisnT , //비젼결과값이 틀어진 정도이기 때문에 이를 반듯하게 하려면 -로 돌려준다.OK
                                           dVisn_HexaCntrOfsX + HexaPot.GetEncPos(paX) , //dVisn_HexaCntrOfsX에다가 헥사포드 포지션, 비젼결과 포지션 다 더해서 돌린다.
                                           dVisn_HexaCntrOfsY - HexaPot.GetEncPos(paY) , //dVisn_HexaCntrOfsY에다가 헥사포드 포지션, 비젼결과 포지션 다 더해서 돌린다.
                                           &dMoveX ,
                                           &dMoveY );
                 Step.iCycle++;
                 return false ;

        case 29:
                 //헥사포트 1사분면이기 때문에..Y축 반전해서 이동.
                 HexaPot.MoveIncRes(paX , -dMoveX ); //핵사 포트 1사분면
                 HexaPot.MoveIncRes(paY ,  dMoveY ); //
                 HexaPot.MoveIncRes(paW , -dVisnT ); //CCW +

                 HexaPot.Move();
                 Step.iCycle++;
                 return false ;

        case 30: if(!HexaPot.GetStopInpos()) return false ;
                 if(iAlignInspCnt < 2) { //얼라인 검사 2번 실시.
                     //포지션.
                     dTempX = pCadVisnMain -> GetCntrPosX() + g_tMidPosOfs.dX;
                     dTempY = pCadVisnMain -> GetCntrPosY() + g_tMidPosOfs.dY;
                     if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                     if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                     MT_GoAbsRun(miLTL_XGenRr , dTempX);
                     MT_GoAbsRun(miLTL_YVisn  , dTempY);
                     Step.iCycle=21;
                     return false ;
                 }

                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);

                 if(WorkInfo.iRow==0 && WorkInfo.iCol==0){ //첫장은 정규 사이즈로 붙이고
                     HexaPot.MoveIncRes(paZ , STG.PkgStat.dMidTopHeight -  STG.PkgStat.dHexaTopHeight + OM.DevOptn.dMidCmsGapFrst);
                     Trace("HexaDown Frst" , String(STG.PkgStat.dMidTopHeight -  STG.PkgStat.dHexaTopHeight + OM.DevOptn.dMidCmsGapFrst).c_str());


                 }
                 else { //둘째장은 높게붙여야 틸트 보정이 된다.
                     Trace("HexaDown" , String(STG.PkgStat.dMidTopHeight -  STG.PkgStat.dHexaTopHeight + OM.DevOptn.dMidCmsGap).c_str());
                     HexaPot.MoveIncRes(paZ , STG.PkgStat.dMidTopHeight -  STG.PkgStat.dHexaTopHeight + OM.DevOptn.dMidCmsGap);
                 }
                 HexaPot.Move();

                 Step.iCycle++;
                 return false ;

        case 31: if(!MoveMotr(miLTL_ZVisn , piLTL_ZVisnWait)) return false ;
                 if(!HexaPot.GetStopInpos()) return false ;

                 MoveMotr(miLTL_YVisn  , piLTL_YVisnWait);
                 MoveMotr(miLTL_XGenRr , piLTL_XDispClean);

                 m_tmDelay.Clear();

                 Step.iCycle++ ;
                 return false ;

        case 32: if(!MoveMotr(miLTL_YVisn   , piLTL_YVisnWait )) return false ;
                 if(!MoveMotr(miLTL_XGenRr  , piLTL_XDispClean)) return false ;
                 MoveMotr(miLTL_YDispr , piLTL_YDispClean);

                 Step.iCycle++;
                 return false ;


        case 33: if(!MoveMotr(miLTL_YDispr , piLTL_YDispClean)) return false ;

                 //DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol ,  csCmsRHeight);
                 DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol ,  csCmsRPreAlign);

                 Step.iCycle=0;
                 return true ;
    }
}

bool CLeftTool::CycleCmsLOri()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }

    static int r,c ;
    static String sSendCmd ;


    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: 
                 WorkInfo.eStat = csMidRHeight ;
                 WorkInfo.iRow  = 0 ;
                 WorkInfo.iCol  = 0 ;
                 MoveActr(aiLTL_GriperFwBW , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 RTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!RTL.ToolVoid()) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 MoveMotr(miLTL_YDispr , piLTL_YDispWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miLTL_YDispr , piLTL_YDispWait)) return false ;
                 Step.iCycle = 20 ;
                 return false ;

        case 20: MoveMotr(miLTL_XGenRr , piLTL_XVisnCmsOri);
                 MoveMotr(miLTL_YVisn  , piLTL_YVisnCmsOri);


                 //일단 한개만 쓰는 것으로 하고..
                 //나중에 한개로 안되면 RC 넣어서 한다.
                 if(STG.FindChip(r,c,csCmsRNeed)){
                     sSendCmd = String("CMO_") + c+r ;
                 }
                 else {
                     sSendCmd = "CMO_00" ;
                 }
                 if(!VSN_L.SendCommand(sSendCmd)){ //시모스  오리엔테이션 으로 세팅.
                     EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveMotr(miLTL_XGenRr , piLTL_XVisnCmsOri)) return false ;
                 if(!MoveMotr(miLTL_YVisn  , piLTL_YVisnCmsOri)) return false ;
                 if(!VSN_L.GetSendCommandEnd()) return false ;

                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnCmsOri);
                 Step.iCycle++;
                 return false;

        case 22: if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnCmsOri)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 23: if(!m_tmDelay.OnDelay(true , 100)) return false ;
                 if(!VSN_L.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "비전 통신 준비 안됨 에러");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 24: if(!VSN_L.GetSendInspEnd()) return false ;
                 if(!VSN_L.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "시모스 Orientation 비젼 에러");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 Step.iCycle++;
                 return false ;

        case 25: if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 DM.ARAY[riRTT].SetStat(csAttach);
                 Step.iCycle=0;
                 return true ;
    }
}
//---------------------------------------------------------------------------
bool CLeftTool::CycleWorkOut()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }

    String sMidID ;


    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //WorkInfo.eStat = csWorkEnd ;
                 //DM.ARAY[riWKO].SetStat(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat);
                 //
                 //
                 MoveActr(aiLTL_GriperFwBW , ccBwd);
                 Step.iCycle++;
                 return false;

        case 11: if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 RTL.ToolVoid(true);
                 Step.iCycle++;
                 return false;

        case 12: if(!RTL.ToolVoid()) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);


                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;


                 MoveActr(aiSTG_Fix1stFwBw , ccBwd);
                 MoveActr(aiSTG_Fix2ndFwBw , ccBwd);
                 MoveActr(aiSTG_Fix3rdFwBw , ccBwd);
                 MoveActr(aiSTG_Fix4thFwBw , ccBwd);
                 MoveActr(aiSTG_Fix5thFwBw , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 14: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MoveActr(aiSTG_Fix1stFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix2ndFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix3rdFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix4thFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix5thFwBw , ccBwd)) return false ;

                 //
                 //모터 움직여 놓구 시작 되면 쳐박....
                 //if(MT_GetCmdPos(miLTL_XGenRr) >= PM.GetValue(miLTL_XGenRr , pvLTL_XIndxWork)){
                 //

                 //}
                 MT_GoAbsRun(miLTL_XGenRr , PM.GetValue(miLTL_XGenRr , pvLTL_XIndxWork) - 20);
                 IO_SetY(ySTG_VcumPump1On , false );
                 IO_SetY(ySTG_VcumPump2On , false );
                 IO_SetY(ySTG_VcumPump3On , false );
                 Step.iCycle++;
                 return false ;

        case 15: if(!MT_GetStopInpos(miLTL_XGenRr)) return false;
                 MT_GoIncSlow(miLTL_XGenRr , -10);
                 Step.iCycle++;
                 return false ;

        case 16: if(IO_GetX(xLTL_WrkPshSnsr)){//얼레 인덱스 내리는 구간에 머가 있네??
                     EM_SetErrMsg(eiPRT_Detect , "왼쪽툴 인덱스 다운구간에 물체가 감지되었습니다.");
                     Step.iCycle=0;
                     return true ;
                 }

                 if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 MoveActr(aiLTL_WrkPshDnUp, ccFwd);
                 HexaPot.MoveAbsRes(paZ , HEX_Z_WAIT);
                 HexaPot.Move();
                 Step.iCycle++;
                 return false ;

        case 17: if(!HexaPot.GetStopInpos()) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp, ccFwd)) return false ;
                 MoveMotr(miLTL_XGenRr, piLTL_XWorkOut,true);
                 Step.iCycle++;
                 return false ;

        case 18: if(IO_GetX(xLTL_WrkPshSnsr)){//오바로드다~~~
                     MT_Stop(miLTL_XGenRr);
                     Step.iCycle = 50 ;
                     return false ;
                 }

        case 19: if(!MoveMotr(miLTL_XGenRr, piLTL_XWorkOut,true)) return false ;

                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 Step.iCycle++;
                 return false;

        case 20: if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false;
                 MoveMotr(miLTL_XGenRr, piLTL_XWait);
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveMotr(miLTL_XGenRr, piLTL_XWait)) return false;
                 sMidID = DM.ARAY[riSTG  ].GetID();
                 DM.ARAY[riWKO].SetStat(csWork );
                 DM.ARAY[riWKO].SetID  (sMidID );
                 DM.ARAY[riWKO].CHPS[0][0].SetID(sMidID);
                 DM.ARAY[riSTG].ClearMap();

                 Step.iCycle=0;
                 return true ;

        //예외상황.
        //오버로드.
        case 50 : MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 51 : if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                  MoveMotr(miLTL_XGenRr , piLTL_XIndxBack);
                  Step.iCycle++;
                  return false ;

        case 52 : if(!MoveMotr(miLTL_XGenRr , piLTL_XIndxBack)) return false ;
                  EM_SetErrMsg(eiPRT_OverLoad , "왼쪽툴 인덱스 오버로드 감지. 걸림을 확인해주세요.");
                  Step.iCycle=0;
                  return true ;
    }
}
//---------------------------------------------------------------------------
bool CLeftTool::CycleClean(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());

        IO_SetY(yLTL_CleanOn , false);

        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }


    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;



        case 10: MoveActr(aiLTL_GriperFwBW , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;


                 MoveMotr(miLTL_XGenRr , piLTL_XDispClean);
                 MoveMotr(miLTL_YDispr , piLTL_YDispClean);
                 Step.iCycle++;
                 return false ;


        case 13: if(!MoveMotr(miLTL_XGenRr , piLTL_XDispClean)) return false ;
                 if(!MoveMotr(miLTL_YDispr , piLTL_YDispClean)) return false ;

                 MoveActr(aiLTL_DisprFDnUp , ccFwd);
                 MoveActr(aiLTL_DisprRDnUp , ccFwd);

                 MoveMotr(miLTL_ZDispr , piLTL_ZDispClean);
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveActr(aiLTL_DisprFDnUp , ccFwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccFwd)) return false ;
                 if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispClean)) return false ;

                 IO_SetY(yLTL_CleanOn , true);

                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;

        case 15: if(!m_tmDelay.OnDelay(true, OM.CmnOptn.iDisprClnTime)) return false ;
                 IO_SetY(yLTL_CleanOn , false);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);

                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);

                 Step.iCycle++;
                 return false ;

        case 16: if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 Step.iCycle=0;
                 return true ;
    }
}

//---------------------------------------------------------------------------
bool CLeftTool::CycleHtReZero(){
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //WorkInfo.eStat = csWorkEnd ;
                 //FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riULD);
                 MoveActr(aiLTL_GriperFwBW , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 RTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);


                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 Step.iCycle = 20;
                 return false ;

        // Left Tool 실제 높이 측정 움직이는 곳!
        case 20: MoveMotr(miLTL_XGenRr, piLTL_XVisnHtCheck);
                 MoveMotr(miLTL_YVisn,  piLTL_YVisnHtCheck);
                 MoveMotr(miLTL_ZVisn,  piLTL_ZVisnCheck  );
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveMotr(miLTL_XGenRr, piLTL_XVisnHtCheck)) return false ;
                 if(!MoveMotr(miLTL_YVisn,  piLTL_YVisnHtCheck)) return false ;
                 if(!MoveMotr(miLTL_ZVisn,  piLTL_ZVisnCheck  )) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 22: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                 Rs232Keyence.SetZero(kcChB);
                 Step.iCycle++;
                 return false ;

        case 23: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 MoveMotr(miLTL_ZVisn,  piLTL_ZVisnWait);
                 MoveMotr(miLTL_YVisn,  piLTL_YVisnWait);
                 MoveMotr(miLTL_XGenRr, piLTL_XWait    );
                 Step.iCycle = 0;
                 return true;
    }
}
//---------------------------------------------------------------------------
bool CLeftTool::CycleManNdlCheck(){
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    bool bNeedFrntCal = (OM.DevOptn.bUseFstDisp && OM.DevOptn.iFstDisp == 0) ||
                        (OM.DevOptn.bUseSecDisp && OM.DevOptn.iSecDisp == 0) ;

    bool bNeedRearCal = (OM.DevOptn.bUseFstDisp && OM.DevOptn.iFstDisp == 1) ||
                        (OM.DevOptn.bUseSecDisp && OM.DevOptn.iSecDisp == 1) ;





    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //WorkInfo.eStat = csWorkEnd ;
                 //FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riULD);
                 MoveActr(aiLTL_GriperFwBW , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 RTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;

                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;
                 Step.iCycle++;
                 return false ;

                 MoveMotr(miLTL_YVisn , piLTL_YVisnWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miLTL_YVisn , piLTL_YVisnWait)) return false ;

                 Step.iCycle++ ;
                 return false ;

        case 15:
                 if(bNeedFrntCal){
                     Step.iCycle = 20;
                     return false ;
                 }
                 else if(bNeedRearCal){
                     Step.iCycle = 30;
                     return false ;
                 }

                 Step.iCycle=0;
                 return true ;


        // Front 쪽 디스펜서 니들 체크 부분
        case 20: MoveMotr(miLTL_XGenRr, piLTL_XDispFtCheck);
                 MoveMotr(miLTL_YDispr, piLTL_YDispFtCheck);
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveMotr(miLTL_XGenRr, piLTL_XDispFtCheck)) return false ;
                 if(!MoveMotr(miLTL_YDispr, piLTL_YDispFtCheck)) return false ;
                 MoveActr(aiLTL_DisprFDnUp, ccFwd);
                 Step.iCycle++;
                 return false ;

        case 22: if(!MoveActr(aiLTL_DisprFDnUp, ccFwd)) return false ;
                 MoveMotr(miLTL_ZDispr, piLTL_ZDispFtCheck, true);
                 Step.iCycle++;
                 return false ;

        case 23: if(!IO_GetX(xETC_NdleNotCheck)){
                     MT_Stop(miLTL_ZDispr);
                 }
                 if(!MT_GetStop(miLTL_ZDispr)) return false ;
                 //if(!MoveMotr(miLTL_ZDispr, piLTL_ZDispFtCheck)) return false ;
                 if(IO_GetX(xETC_NdleNotCheck)){
                     MT_GoInc(miLTL_ZDispr, 3.0, 1.0);
                 }
                 Step.iCycle++;
                 return false ;

                 //if(!MT_GetStop(miLTL_ZDispr)) return false ;

        case 24: if(!IO_GetX(xETC_NdleNotCheck)){
                     MT_EmgStop(miLTL_ZDispr);
                 }
                 if(!MT_GetStop(miLTL_ZDispr)) return false ;
                 if(IO_GetX(xETC_NdleNotCheck)){
                     Step.iCycle = 40;
                     return false ;
                 }

                 //체크 완료 되면 뒤로 빼면서...
                 //체크 해제
                 MT_GoInc(miLTL_ZDispr , -10.0 , 0.1);
                 Step.iCycle++;
                 return false ;

        case 25: if(IO_GetX(xETC_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miLTL_ZDispr);
                 }
                 if(!MT_GetStop(miLTL_ZDispr)) return false ;
                 PM.SetValue(miLTL_ZDispr , pvLTL_ZDispFtCheck , MT_GetCmdPos(miLTL_ZDispr));
                 MoveMotr(miLTL_ZDispr, piLTL_ZDispWait);
                 MoveActr(aiLTL_DisprFDnUp, ccBwd);
                 Step.iCycle++;
                 return false ;

        case 26: if(!MoveMotr(miLTL_ZDispr, piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(aiLTL_DisprFDnUp, ccBwd      )) return false ;
                 MoveMotr(miLTL_XGenRr, piLTL_XWait);
                 MoveMotr(miLTL_YDispr, piLTL_YDispWait);
                 Step.iCycle++;
                 return false;

        case 27: if(!MoveMotr(miLTL_XGenRr, piLTL_XWait    )) return false ;
                 if(!MoveMotr(miLTL_YDispr, piLTL_YDispWait)) return false ;
                 if(bNeedRearCal){
                     Step.iCycle = 30;
                     return false ;
                 }

                 Step.iCycle = 0;
                 return true;

        // Rear 쪽 디스펜서 니들 체크 부분
        case 30: MoveMotr(miLTL_XGenRr, piLTL_XDispRrCheck);
                 MoveMotr(miLTL_YDispr, piLTL_YDispRrCheck);
                 Step.iCycle++;
                 return false ;

        case 31: if(!MoveMotr(miLTL_XGenRr, piLTL_XDispRrCheck)) return false ;
                 if(!MoveMotr(miLTL_YDispr, piLTL_YDispRrCheck)) return false ;
                 MoveActr(aiLTL_DisprRDnUp, ccFwd);
                 Step.iCycle++;
                 return false ;


        case 32: if(!MoveActr(aiLTL_DisprRDnUp, ccFwd)) return false ;
                 MoveMotr(miLTL_ZDispr, piLTL_ZDispRrCheck, true);
                 Step.iCycle++;
                 return false ;

        case 33: if(!MoveMotr(miLTL_ZDispr, piLTL_ZDispRrCheck)) return false ;
                 // 니들 Check 부분
                 if(!IO_GetX(xETC_NdleNotCheck)){
                     MT_EmgStop(miLTL_ZDispr);
                 }
                 if(!MT_GetStop(miLTL_ZDispr)) return false ;
                 if(IO_GetX(xETC_NdleNotCheck)){
                     MT_GoInc(miLTL_ZDispr, 3.0, 1.0);
                 }
                 Step.iCycle++;
                 return false ;

        case 34: if(!IO_GetX(xETC_NdleNotCheck)){
                     MT_EmgStop(miLTL_ZDispr);
                 }
                 if(!MT_GetStop(miLTL_ZDispr)) return false ;
                 if(IO_GetX(xETC_NdleNotCheck)){
                     Step.iCycle = 40;
                     return false ;
                 }

                 //체크 완료 되면 뒤로 빼면서...
                 //체크 해제
                 MT_GoInc(miLTL_ZDispr , -10.0 , 0.1);
                 Step.iCycle++;
                 return false ;

        case 35: if(IO_GetX(xETC_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miLTL_ZDispr);
                 }
                 if(!MT_GetStop(miLTL_ZDispr)) return false ;
                 PM.SetValue(miLTL_ZDispr , pvLTL_ZDispRrCheck , MT_GetCmdPos(miLTL_ZDispr));
                 MoveMotr(miLTL_ZDispr, piLTL_ZDispWait);
                 MoveActr(aiLTL_DisprRDnUp, ccBwd);
                 Step.iCycle++;
                 return false ;

        case 36: if(!MoveMotr(miLTL_ZDispr, piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(aiLTL_DisprRDnUp, ccBwd      )) return false ;
                 MoveMotr(miLTL_XGenRr, piLTL_XDispClean);
                 MoveMotr(miLTL_YDispr, piLTL_YDispClean);
                 Step.iCycle++;
                 return false;

        case 37: if(!MoveMotr(miLTL_XGenRr, piLTL_XDispClean)) return false ;
                 if(!MoveMotr(miLTL_YDispr, piLTL_YDispClean)) return false ;
                 FM_MsgOk("Comfirm", "Check Sensor를 닦아주세요.");
                 Step.iCycle = 0;
                 return true;


        //Error시에 대기 위치로 올라거서 Stop.
        case 40: MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveActr(aiLTL_DisprRDnUp, ccBwd);
                 Step.iCycle++;
                 return false;

        case 41: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false;
                 if(!MoveActr(aiLTL_DisprRDnUp, ccBwd       )) return false;
                 EM_SetErrMsg(eiPRT_CheckErr, "Dispensor가 높이 세팅을 하지 못하였습니다.");
                 MoveMotr(miLTL_XGenRr, piLTL_XDispClean);
                 MoveMotr(miLTL_YDispr, piLTL_YDispClean);
                 Step.iCycle++;
                 return false;

        case 42: if(!MoveMotr(miLTL_XGenRr, piLTL_XDispClean)) return false ;
                 if(!MoveMotr(miLTL_YDispr, piLTL_YDispClean)) return false ;
                 Step.iCycle = 0;
                 return true;

    }
}
//---------------------------------------------------------------------------
bool CLeftTool::CycleToolVoid()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }

    bool bZDispPosOk  ;
    bool bZVisnPosOk  ;
    bool bXPosOk  ;



    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10:  //if(GetSeqStep() != scIdle) return false ;
                  bXPosOk     = MT_GetTrgPos(miLTL_XGenRr) <= PM.GetValue(miLTL_XGenRr , pvLTL_XWait     ) && MT_GetStopInpos(miLTL_XGenRr) ;
                  if(bXPosOk) {
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  bZDispPosOk = MT_GetTrgPos(miLTL_ZDispr) <= PM.GetValue(miLTL_ZDispr , pvLTL_ZDispWait) && MT_GetStopInpos(miLTL_ZDispr) ;
                  bZVisnPosOk = MT_GetTrgPos(miLTL_ZVisn ) <= PM.GetValue(miLTL_ZVisn  , pvLTL_ZVisnWait) && MT_GetStopInpos(miLTL_ZVisn ) ;


                  if(!bZDispPosOk){
                      MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                  }

                  if(!bZVisnPosOk){
                      MoveMotr(miLTL_ZVisn , piLTL_ZVisnWait);
                  }

                  MoveActr(aiLTL_MGrperDnUp , ccBwd);
                  MoveActr(aiLTL_SGrperDnUp , ccBwd);
                  MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                  MoveActr(aiLTL_GriperFwBW , ccBwd);

                  MoveActr(aiLTL_DisprFDnUp , ccBwd);
                  MoveActr(aiLTL_DisprRDnUp , ccBwd);
                  MoveActr(aiLTL_UVCureDnUp , ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11:  if(!MT_GetStopInpos(miLTL_ZDispr)) return false ;
                  if(!MT_GetStopInpos(miLTL_ZVisn )) return false ;

                  if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                  if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                  if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;
                  if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;

                  if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                  if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                  if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                  MoveMotr(miLTL_XGenRr , piLTL_XWait) ;
                  Step.iCycle++;
                  return false ;

        case 12:  if(!MoveMotr(miLTL_XGenRr , piLTL_XWait )) return false ;
                  Step.iCycle = 0;
                  return true;
    }
}
//---------------------------------------------------------------------------
bool CLeftTool::CycleCmsLHeight()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }

    bool bZDispPosOk  ;
    bool bZVisnPosOk  ;
    bool bXPosOk  ;

    static int    iHtStep      ;
    static double dCrntHeight  ;

    static int    iCrntNodeCnt ;

    static CCadHghtCmd * pHghtCmd ;
    String sCrntHeight ;

    double dTempX , dTempY ;


    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: 
                 WorkInfo.eStat = csCmsLHeight ;
                 WorkInfo.iRow  = 0 ;
                 WorkInfo.iCol  = 0 ;
                 pHghtCmd = CAD_Rtt.CM.CM_RHgtCheck ;
                 SetCadOfsGain(tlLHght , cnRtt);

                 MoveActr(aiLTL_GriperFwBW , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 RTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!RTL.ToolVoid()) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 MoveMotr(miLTL_YDispr , piLTL_YDispWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miLTL_YDispr , piLTL_YDispWait)) return false ;
                 Step.iCycle = 20 ;
                 return false ;

        case 20:
                 iCrntNodeCnt = 0 ;
                 Step.iCycle = 25 ;

                 IO_SetY(yIDX_RttPumpOn , false);

                 STG.Spc.Clear();
                 STG.Spc.dWorkSttTime = Now().Val ;


                 return false ;

        case 25: SetCadOfsGain(tlLHght , cnRtt );
                 dTempX = pHghtCmd->GetPosX(iCrntNodeCnt); //- g_tMidPosOfs.dX;
                 dTempY = pHghtCmd->GetPosY(iCrntNodeCnt); //- g_tMidPosOfs.dY;

                 Trace("pHghtCmd->GetPosX",String(pHghtCmd->GetPosX(iCrntNodeCnt)).c_str());
                 Trace("pHghtCmd->GetPosY",String(pHghtCmd->GetPosY(iCrntNodeCnt)).c_str());


                 //dMotorPos[0] = dTempX;
                 //dMotorPos[1] = dTempY;
                 //MT_LineMove(iCoodination ,
                 //            iMotorAxis   ,
                 //            dMotorPos    ,
                 //            MT_GetRunVel(miRTL_XGenRr) ,
                 //            1000 ,
                 //            1000 ,
                 //            iSincMotrMax);

                 iHtStep     = 10 ;
                 dCrntHeight = 0  ;
                 if(OM.CmnOptn.iWorkMode == 1) CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY);   //20160426 진섭 수정

                 Trace("dTempX",String(dTempX).c_str());
                 Trace("dTempY",String(dTempY).c_str());

                 Step.iCycle++;
                 return false ;

        case 26: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY)) return false ;
                 sCrntHeight.sprintf("%.4f",dCrntHeight-OM.DevOptn.dCmsHeightOfs);

                 STG.Spc.sBfHeight00 = STG.Spc.sBfHeight00 + " " + sCrntHeight ;

                 iCrntNodeCnt++;
                 if(iCrntNodeCnt < pHghtCmd->GetNodeCnt()){
                     Step.iCycle = 25 ;
                     return false ;
                 }

                 DM.ARAY[riRTT].SetStat(csWorkEnd );
                 Trace("Bf UV sBfHeight00" , STG.Spc.sBfHeight00.c_str());

                 STG.Spc.dWorkEndTime = Now().Val ;
                 STG.WriteSpc();

                 Step.iCycle = 0;
                 return true;
    }
}

bool CLeftTool::CycleMultiHeightCheck(int &_iStep  , double &_dHeight ,double _dPosX , double _dPosY)
{
    static CDelayTimer tmDelay ;
    static double dMinHeight  ;
    static double dMaxHeight  ;
    static double dChechSum   ;
           double dCheckAvr   ;

    static double dCrntHeight ;

    int iHeightStepCnt = OM.DevOptn.iHeightStepCnt ;
    if(iHeightStepCnt <1) iHeightStepCnt = 1 ;
    const int iStepMaxCnt = iHeightStepCnt ;
    static int iStepCnt = 0 ;


    //측정하려는 포지션이 센터로 오게 함.
    double dTotalRangeX = OM.DevOptn.dHeightPitchX * (iStepMaxCnt-1) ;
    double dTotalRangeY = OM.DevOptn.dHeightPitchY * (iStepMaxCnt-1) ;

    static double dSttPosX ;
    static double dSttPosY ;

    const int iStepDelay = 1 ;

    static String sCheckDatas ;


    //Cycle.
    switch (_iStep) {
        default :
                  return true ;

        case 10: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YVisn )) return false ;
                  //포지션.
                 dMinHeight  = 999999 ;
                 dMaxHeight  =-999999 ;
                 dCrntHeight = 0.0 ;
                 dChechSum   = 0.0 ;

                 iStepCnt = 0 ;

                 sCheckDatas = "";

                 //총레인지에서 반띵해서 뺀수치가 스타트 포인트.
                 dSttPosX = _dPosX - dTotalRangeX / 2.0 ;
                 dSttPosY = _dPosY - dTotalRangeY / 2.0 ;


                 Trace("dSttPosX",String(dSttPosX).c_str());
                 Trace("dSttPosY",String(dSttPosY).c_str());
                 MT_GoAbsRun(miLTL_XGenRr , dSttPosX);
                 MT_GoAbsRun(miLTL_YVisn  , dSttPosY);

                 _iStep++;
                 return false ;


        //밑에서 씀.
        //반복측정 패턴.
        case 11: if(!MT_GetStopInpos(miLTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miLTL_YVisn )) return false ;
                 tmDelay.Clear();

                 _iStep++;
                 return false ;

        case 12:
                 if(iStepCnt == 0 && !tmDelay.OnDelay(true , OM.CmnOptn.iRightHeightDelay))return false ;
                 if(iStepCnt != 0 && !tmDelay.OnDelay(true , iStepDelay                  ))return false ;

                 iStepCnt++;
                 Rs232Keyence.CheckHeight(kcChB); //Right센서.
                 _iStep++;
                 return false ;

        case 13: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 if(LASER_MAX == Rs232Keyence.GetHeightData(kcChB)) {
                     EM_SetErrMsg(eiHGT_RangeErr , "Right 높이 측정기 측정포지션을 확인하세요.");
                     return true ;
                 }
                 dCrntHeight = Rs232Keyence.GetHeightData(kcChB) ;

                 dChechSum   += dCrntHeight ;

                 sCheckDatas += String(dCrntHeight) + "," ;


                 if(dMinHeight > dCrntHeight) dMinHeight = dCrntHeight ;
                 if(dMaxHeight < dCrntHeight) dMaxHeight = dCrntHeight ;


                 if(iStepCnt < iStepMaxCnt) {//여기서 1안빼주면 10설정시에 21번 측정함.


                     MT_GoIncRun(miLTL_XGenRr , OM.DevOptn.dHeightPitchX);
                     MT_GoIncRun(miLTL_YVisn  , OM.DevOptn.dHeightPitchY);

                     _iStep = 11 ;
                     return false ;
                 }

                 if(iStepMaxCnt){
                     dCheckAvr = dChechSum /(double)(iStepMaxCnt);
                 }
                 else { //여기 들어오면 안된다.
                     dCheckAvr = dCrntHeight ;
                 }

                 dCheckAvr = ((int)(dCheckAvr * 10000))/(double)10000.0 ;

                 Trace("MultiHeightDatas" , sCheckDatas.c_str());
                 Trace("MultiHeightMin"   , String(dMinHeight).c_str());
                 Trace("MultiHeightMax"   , String(dMaxHeight).c_str());
                 Trace("MultiHeightAvr"   , String(dCheckAvr ).c_str());


                 if(OM.DevOptn.iHeightMethod==1){
                     _dHeight = dMaxHeight;
                 }
                 else if(OM.DevOptn.iHeightMethod==2){
                     _dHeight = dCheckAvr;
                 }
                 else {
                     _dHeight = dMinHeight;
                 }


                 _iStep=0;
                 return true ;
        }
}

bool CLeftTool::CycleManUVTest()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
    }

    bool bZDispPosOk  ;
    bool bZVisnPosOk  ;
    bool bXPosOk  ;

    static int    iHtStep      ;
    static double dCrntHeight  ;

    static int    iCrntNodeCnt ;

    static CCadHghtCmd * pHghtCmd ;
    String sCrntHeight ;

    double dTempX , dTempY ;


    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: 

                 MoveActr(aiLTL_GriperFwBW , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveActr(aiLTL_GriperFwBW , ccBwd)) return false ;
                 RTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!RTL.ToolVoid()) return false ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
                 MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait);
                 MoveActr(aiLTL_WrkPshDnUp , ccBwd);
                 MoveActr(aiLTL_MGrperDnUp , ccBwd);
                 MoveActr(aiLTL_SGrperDnUp , ccBwd);

                 MoveActr(aiLTL_DisprFDnUp , ccBwd);
                 MoveActr(aiLTL_DisprRDnUp , ccBwd);
                 MoveActr(aiLTL_UVCureDnUp , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 if(!MoveMotr(miLTL_ZVisn  , piLTL_ZVisnWait)) return false ;
                 if(!MoveActr(aiLTL_MGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_SGrperDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_WrkPshDnUp , ccBwd)) return false ;

                 if(!MoveActr(aiLTL_DisprFDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_DisprRDnUp , ccBwd)) return false ;
                 if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;

                 MoveMotr(miLTL_YDispr , piLTL_YDispWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miLTL_YDispr , piLTL_YDispWait)) return false ;
                 Step.iCycle = 20 ;
                 return false ;

        //UV Test 포지션
        case 20: MoveMotr(miLTL_YDispr , piLTL_YDispClean) ;
                 Step.iCycle++ ;
                 return false ;

        case 21: if(!MoveMotr(miLTL_YDispr , piLTL_YDispClean)) return false ;
                 MT_GoIncRun(miLTL_YDispr , -OM.DevOptn.dUVTestDist) ;
                 Step.iCycle++ ;
                 return false ;

        case 22: if(!MT_GetStopInpos(miLTL_YDispr)) return false ;
                 MoveActr(aiLTL_UVCureDnUp , ccFwd) ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispUVTest) ;
                 Step.iCycle++ ;
                 return false ;

        case 23: if(!MoveActr(aiLTL_UVCureDnUp , ccFwd)) return false ;
                 if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispUVTest)) return false ;
                 Rs232UV.SetCuring(uvCh5, OM.DevOptn.iUVTopBrightness);
                 Step.iCycle++ ;
                 return false ;

        case 24: Rs232UV.SetCuringOnOff(uvCh5 , true) ;
                 IO_SetY(yLTL_UVHumeSuction , true) ;
                 Step.iCycle++ ;
                 return false ;

        case 25: MT_GoInc(miLTL_YDispr , OM.DevOptn.dUVTestDist * 2.0 , OM.DevOptn.dUVTestSpeed) ;
                 Step.iCycle++ ;
                 return false ;

        case 26: if(!MT_GetStopInpos(miLTL_YDispr)) return false ;
                 Rs232UV.SetCuringOnOff(uvCh5 , false) ;
                 IO_SetY(yLTL_UVHumeSuction , false) ;
                 Step.iCycle++ ;
                 return false ;

        case 27: MoveActr(aiLTL_UVCureDnUp , ccBwd) ;
                 MoveMotr(miLTL_ZDispr , piLTL_ZDispWait) ;
                 Step.iCycle++ ;
                 return false ;

        case 28: if(!MoveActr(aiLTL_UVCureDnUp , ccBwd)) return false ;
                 if(!MoveMotr(miLTL_ZDispr , piLTL_ZDispWait)) return false ;
                 MoveMotr(miLTL_YDispr , piLTL_YDispWait) ;
                 Step.iCycle++ ;
                 return false ;

        case 29: if(!MoveMotr(miLTL_YDispr , piLTL_YDispWait)) return false ;
                 Step.iCycle = 0 ;
                 return true ;

    }
}

bool CLeftTool::ToolVoid(bool _bInit)
{
    bool bXPosOk = MT_GetTrgPos(miLTL_XGenRr) <= PM.GetValue(miLTL_XGenRr , pvLTL_XWait     ) && MT_GetStopInpos(miLTL_XGenRr) ;
    if(bXPosOk){
        Stat.bReqToolVoid = false ;
        return true ;
    }

    //오토런시에
    if(Step.iSeq != scIdle){
        Stat.bReqToolVoid = true ;
        return false ;
    }
    else { //메뉴얼 작업 일때.
        if(_bInit){
            InitCycleStep();
        }
        CycleToolVoid();
    }
}

/*
bool CLeftTool::ToolVoid() //디스펜서 일할때 피해주기.
{
    //if(!AT_Complete(aiLTL_MGrperDnUp , ccBwd)) return false ; //이걸 여기서 올리기엔 너무 변수가 많다.

    bool bActrBwd    = MoveActr(aiLTL_DisprFDnUp , ccBwd) &&
                       MoveActr(aiLTL_DisprRDnUp , ccBwd) &&
                       MoveActr(aiLTL_MGrperDnUp , ccBwd) &&
                       MoveActr(aiLTL_UVCureDnUp , ccBwd) &&
                       MoveActr(aiLTL_WrkPshDnUp , ccBwd) ;


    bool bZDispPosOk = MT_GetTrgPos(miLTL_ZDispr) <= PM.GetValue(miLTL_ZDispr , pvLTL_ZDispWait ) && MT_GetStopInpos(miLTL_ZDispr) ;
    bool bXPosOk     = MT_GetTrgPos(miLTL_XGenRr) <= PM.GetValue(miLTL_XGenRr , pvLTL_XDispClean) && MT_GetStopInpos(miLTL_XGenRr) ;

    if(bXPosOk){
        return true ;
    }

    if(!bActrBwd){
        return false ;
    }



    bool bRetZ ;
    if(!bZDispPosOk) {
        bRetZ = MoveMotr(miLTL_ZDispr , piLTL_ZDispWait);
    }
    else {
        bRetZ = true ;
    }
    if(!bRetZ) return false ;



    bool bRetX ;
    if(!bXPosOk) {
        bRetX = MoveMotr(miLTL_XGenRr , piLTL_XDispClean) ;
    }
    else {
        bRetX = true ;
    }
    return bRetX ;
}
*/
//---------------------------------------------------------------------------
bool CLeftTool::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ; // 트랜스퍼 z축, x축 Rr 갠츄리가 x와 y가 로테이트 위치에 있는지 (스테이지 레일 위치에 있는지)
    AnsiString sMsg ;

    // Left X
    double dLTL_XGenRrEnc = MT_GetEncPos(miLTL_XGenRr);
    double dLTL_XGenRrTrg = MT_GetTrgPos(miLTL_XGenRr);
    double dLTL_XGenRrPos = dLTL_XGenRrEnc < dLTL_XGenRrTrg ? dLTL_XGenRrTrg : dLTL_XGenRrEnc ;
    // Right X
    double dRTL_XGenRrEnc = MT_GetEncPos(miRTL_XGenRr);
    double dRTL_XGenRrTrg = MT_GetTrgPos(miRTL_XGenRr);
    double dRTL_XGenRrPos = dRTL_XGenRrEnc < dRTL_XGenRrTrg ? dRTL_XGenRrTrg : dRTL_XGenRrEnc ;

    bool   bLTL_WrkPshDn  = AT_Complete(aiLTL_WrkPshDnUp,ccFwd);
    bool   bLTL_SGrperDn  = AT_Complete(aiLTL_SGrperDnUp,ccFwd);
    bool   bSTG_FixFwd    = AT_Complete(aiSTG_Fix1stFwBw,ccFwd) || AT_Complete(aiSTG_Fix2ndFwBw,ccFwd) ||
                            AT_Complete(aiSTG_Fix3rdFwBw,ccFwd) || AT_Complete(aiSTG_Fix4thFwBw,ccFwd) ||
                            AT_Complete(aiSTG_Fix5thFwBw,ccFwd);

    bool   bRAL_Detect    = IO_GetX(xSTG_Detect3) || IO_GetX(xSTG_Detect4) || IO_GetX(xWKO_Detect5);

    bool   bLTL_ActDn     = AT_Complete(aiLTL_DisprFDnUp,ccFwd)||
                            AT_Complete(aiLTL_DisprRDnUp,ccFwd)||
                            AT_Complete(aiLTL_UVCureDnUp,ccFwd);

    // X축 갠츄리
    if(_iMotr == miLTL_XGenRr){
        if(bLTL_ActDn) {
            sMsg = String("툴실린더가 내려와 있습니다.") ;
            bRet = false ;
        }
        if(dLTL_XGenRrPos + dRTL_XGenRrPos > OM.MstOptn.dLTL_RTL_Stroke){
            sMsg = MT_GetName(miLTL_XGenRr) + String("모터가 충돌 위치에 있습니다.") ;
            bRet = false ;
        }
        if((bLTL_WrkPshDn || bLTL_SGrperDn) && bSTG_FixFwd && bRAL_Detect){
            sMsg = MT_GetName(miLTL_XGenRr) + String("레일 자재가 충돌 위치에 있습니다.") ;
            bRet = false;
        }
    }
    // Y축 갠츄리
    else if(_iMotr == miLTL_YDispr){
        if(bLTL_ActDn) {
            sMsg = String("툴실린더가 내려와 있습니다.") ;
            bRet = false ;
        }
        if(MT_GetTrgPos(miLTL_YVisn) + MT_GetCmdPos(_iMotr) > OM.MstOptn.dLTL_VISN_DISP_Strke ||
           MT_GetCmdPos(miLTL_YVisn) + MT_GetTrgPos(_iMotr) > OM.MstOptn.dLTL_VISN_DISP_Strke ){
           sMsg = MT_GetName(miLTL_YVisn) + String("모터가 충돌 위치에 있습니다.") ;
           bRet = false ;
        }
    }
    else if(_iMotr == miLTL_YVisn){
        if(MT_GetTrgPos(miLTL_YDispr) + MT_GetCmdPos(_iMotr) > OM.MstOptn.dLTL_VISN_DISP_Strke ||
           MT_GetCmdPos(miLTL_YDispr) + MT_GetTrgPos(_iMotr) > OM.MstOptn.dLTL_VISN_DISP_Strke ){
           sMsg = MT_GetName(miLTL_YDispr) + String("모터가 충돌 위치에 있습니다.") ;
           bRet = false ;
        }
    }
    // Z축 디스펜서
    else if(_iMotr == miLTL_ZDispr){
        if(!MT_GetStop(miLTL_XGenRr)){
            sMsg = MT_GetName(miLTL_XGenRr) + String("모터가 이동중 입니다.") ;
            bRet = false ;
        }
        if(!MT_GetStop(miLTL_YDispr)){
            sMsg = MT_GetName(miLTL_YDispr)  + String("모터가 이동중 입니다.") ;
            bRet = false ;
        }
        //if(MT_GetCmdPos(miLTL_ZDispr) > PM.GetValue(miLTL_ZDispr , pvLTL_ZDispWait) + 1.0){
        //    sMsg = MT_GetName(miLTL_ZDispr)  + String("모터의 높이가 대기위치보다 높습니다.") ;
        //    bRet = false ;
        //}
    }

    // Z축 오리엔테이션 비전
    else if(_iMotr == miLTL_ZVisn){
        if(!MT_GetStop(miLTL_XGenRr)){
            sMsg = MT_GetName(miLTL_XGenRr) + String("모터가 이동중 입니다.") ;
            bRet = false ;
        }
        if(!MT_GetStop(miLTL_YVisn)){
            sMsg = MT_GetName(miLTL_YVisn)  + String("모터가 이동중 입니다.") ;
            bRet = false ;
        }
        //if(MT_GetCmdPos(miLTL_ZVisn) > PM.GetValue(miLTL_ZVisn , pvRTL_ZVisnWait) + 1.0){
        //    sMsg = MT_GetName(miLTL_ZVisn)  + String("모터의 높이가 대기위치보다 높습니다.") ;
        //    bRet = false ;
        //}
    }

    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iSeq)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CLeftTool::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiLTL_DisprFDnUp){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }

    else if(_iActr == aiLTL_DisprRDnUp){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == aiLTL_UVCureDnUp){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == aiLTL_MGrperDnUp){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == aiLTL_SGrperDnUp){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == aiLTL_WrkPshDnUp){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
            // juhyeon

        }
    }
    else if(_iActr == aiSTG_Fix1stFwBw){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == aiSTG_Fix2ndFwBw){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == aiSTG_Fix3rdFwBw){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == aiSTG_Fix4thFwBw){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == aiSTG_Fix5thFwBw){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == aiLTL_GriperFwBW){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }

    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

bool CLeftTool::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        return MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }

    return false ;
}


bool CLeftTool::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CLeftTool::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(miLTL_ZDispr);
    return ;

}

double CLeftTool::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return dLastIdxPos ;
}

bool CLeftTool::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miLTL_ZDispr)) return true  ; //이파트는 필요 없다.
    else                                            return false ;

}

bool CLeftTool::CheckStop()
{
    if(!MT_GetStop(miLTL_XGenRr)) return false ;
    if(!MT_GetStop(miLTL_YDispr)) return false ;
    if(!MT_GetStop(miLTL_ZDispr)) return false ;

    if(!AT_Done(aiLTL_DisprFDnUp)) return false ;
    if(!AT_Done(aiLTL_DisprRDnUp)) return false ;

    return true ;
}

void CLeftTool::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos    " , m_dLastIdxPos   );
        UserINI.Load(sPath, "Member" , "m_iLastUVSttTime " , m_iLastUVSttTime);

        UserINI.Load(sPath, "Member" , "m_iLastUVRow     " , m_iLastUVRow    );
        UserINI.Load(sPath, "Member" , "m_iLastUVCol     " , m_iLastUVCol    );
        UserINI.Load(sPath, "Member" , "m_iDispCntF      " , m_iDispCntF     );
        UserINI.Load(sPath, "Member" , "m_iDispCntR      " , m_iDispCntR     );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos    " , m_dLastIdxPos   );
        UserINI.Save(sPath, "Member" , "m_iLastUVSttTime " , m_iLastUVSttTime);
        UserINI.Save(sPath, "Member" , "m_iLastUVRow     " , m_iLastUVRow    );
        UserINI.Save(sPath, "Member" , "m_iLastUVCol     " , m_iLastUVCol    );
        UserINI.Save(sPath, "Member" , "m_iDispCntF      " , m_iDispCntF     );
        UserINI.Save(sPath, "Member" , "m_iDispCntR      " , m_iDispCntR     );
    }
}

double CLeftTool::GetCrntStgCntVisnPosX()
{
    //왼쪽비젼의 스테이지 센터 포지션 계산.
    //레프트 비젼의 레일 센터 절대 위치. 전디바이스 공용 포지션.
    double dVisnRailCntX = PM.GetValue(miLTL_XGenRr , pvLTL_XVisnCheck) - RTL.GetTouchToStgCntX() ;

    //현재 스테이지 위치.
    double dStgCrnt    = MT_GetCmdPos(miSTG_XStage);

    //스테이지 입장에서 센터에서 현재 위치까지 거리.
    double dCntToStgGap = dStgCrnt - PM.GetValue(miSTG_XStage , pvSTG_XStageCntr) ;
    double dVisnStgPos  = dVisnRailCntX + dCntToStgGap ;

    return dVisnStgPos ;
}
double CLeftTool::GetCrntStgCntVisnPosY()
{
    //왼쪽비젼의 스테이지 센터 포지션 계산.
    //레프트 비젼의 레일 센터 절대 위치. 전디바이스 공용 포지션.
    double dVisnRailCntY = PM.GetValue(miLTL_YVisn  , pvLTL_YVisnCheck) - RTL.GetTouchToStgCntY() ;

    //현재 스테이지 위치.
    double dStgCrnt    = MT_GetCmdPos(miSTG_YStage);

    //스테이지 입장에서 센터에서 현재 위치까지 거리.
    double dCntToStgGap = dStgCrnt - PM.GetValue(miSTG_YStage , pvSTG_YStageCntr) ;
    double dVisnStgPos  = dVisnRailCntY + dCntToStgGap ;

    return dVisnStgPos ;
}

