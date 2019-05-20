//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "RightTool.h"
#include "Stage.h"
#include "UnLoader.h"
#include "LeftTool.h"

#include "HexaPot.h"
#include "Confocal.h"
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
#include "Index.h"
#include "DispLogUnit.h"

#include "Rs232Keyence.h"
#include "Rs232UVCuring.h"

#include "BaseMaths.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
//---------------------------------------------------------------------------
CRightTool RTL;

CRightTool::CRightTool(void)
{

}

CRightTool::~CRightTool (void)
{

}

void CRightTool::Init()
{
    m_sPartName = "RightTool ";
    
    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

    m_sCheckSafeMsg = "" ;

    m_bNeedHexaJog = false ;

    //MT_SetGantryEnable(miRTL_XGenRr , miRTL_XGenFt , 0 , 0.0 , 1.0);
}

void CRightTool::Close()
{
    Load(false);
}

void CRightTool::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

//������ ��Ƽ�� ����.
bool CRightTool::FindChip(int &r , int &c , EN_CHIP_STAT _iChip , EN_ARAY_ID _iId)
{
    if(_iId == riULD) return ULD.FindChip (r , c , _iChip);
    else              return STG.FindChip (r , c , _iChip);
}

void CRightTool::SetCadOfsGain(EN_TOOL_ID _eTool)
{
    //������ �������忡�� Check������ ���� �������� ���ͱ��� �Ÿ�.
    double dLVisnCheckToStgCntX = PM.GetValue(miRTL_XGenRr , pvRTL_XVisnStgCnt) - PM.GetValue(miRTL_XGenRr , pvRTL_XVisnCheck);  //+ ���ʿ� ������.
    double dLVisnCheckToStgCntY = PM.GetValue(miRTL_YVisn  , pvRTL_YVisnStgCnt) - PM.GetValue(miRTL_YVisn  , pvRTL_YVisnCheck);  //- ���ʿ� ������.

    double dXGain ;
    double dYGain ;
    double dZGain ;
    double dXOffset ;
    double dYOffset ;
    double dZOffset ;

    /*
    tlRTrns ,
    tlRVisn ,
    tlRHght ,
    tlStage ,
    */
    if(_eTool == tlNone ){
        Trace("SetCadOfsGain","tlNone");
        dXGain  =  1 ;
        dYGain  =  1 ;
        dZGain  =  1 ;
        dXOffset = 0 ;
        dYOffset = 0 ;
        dZOffset = 0 ;
    }
    else if(_eTool == tlRHght){
        Trace("SetCadOfsGain","tlRHght");
        dXGain  = -1 ;
        dYGain  = -1 ;
        dZGain  =  1 ;
        dXOffset = PM.GetValue(miRTL_XGenRr , pvRTL_XVisnHtCheck) + dLVisnCheckToStgCntX ;
        dYOffset = PM.GetValue(miRTL_YVisn  , pvRTL_YVisnHtCheck) + dLVisnCheckToStgCntY ;
        dZOffset = 0 ;
    }
    else if(_eTool == tlRVisn){
        Trace("SetCadOfsGain","tlRVisn");
        dXGain  = -1 ;
        dYGain  = -1 ;
        dZGain  =  1 ;
        dXOffset = PM.GetValue(miRTL_XGenRr , pvRTL_XVisnStgCnt) ;
        dYOffset = PM.GetValue(miRTL_YVisn  , pvRTL_YVisnStgCnt) ;
        dZOffset = 0 ;
    }
    else if(_eTool == tlRTrns){
        Trace("SetCadOfsGain","tlRTrns");
        dXGain  = -1 ;
        dYGain  =  1 ;
        dZGain  =  1 ;
        dXOffset = PM.GetValue(miRTL_XGenRr , pvRTL_XTrsfCheck ) + dLVisnCheckToStgCntX ;
        dYOffset = PM.GetValue(miRTL_YTrspr , pvRTL_YTrsfCheck ) - dLVisnCheckToStgCntY ;
        dZOffset = 0 ;
    }
    else if(_eTool == tlStage){
        Trace("SetCadOfsGain","tlStage");
        dXGain   = 1 ;
        dYGain   = 1 ;
        dZGain   = 1 ;
        dXOffset = PM.GetValue(miSTG_XStage , pvSTG_XStageCntr) ;
        dYOffset = PM.GetValue(miSTG_YStage , pvSTG_YStageCntr) ;
        dZOffset = 0 ;
    }
    else if(_eTool == tlRConfocal){
        Trace("SetCadOfsGain","tlRConfocal");
        dXGain   = -1 ;
        dYGain   = -1 ;
        dZGain   =  1 ;
        dXOffset = PM.GetValue(miRTL_XGenRr , pvRTL_XVisnCfCheck) + dLVisnCheckToStgCntX ;
        dYOffset = PM.GetValue(miRTL_YVisn  , pvRTL_YVisnCfCheck) + dLVisnCheckToStgCntY ;
        dZOffset = 0 ;
    }

    CAD.SetGainX(dXGain);
    CAD.SetGainY(dYGain);
    CAD.SetGainZ(dZGain);
    CAD.SetOffsetX(dXOffset);
    CAD.SetOffsetY(dYOffset);
    CAD.SetOffsetZ(dZOffset);
}


double CRightTool::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dGripStt ;

    if(_iMotr == miRTL_XGenRr){
             if(_iPstnId == piRTL_XWait             ) dPos = PM.GetValue (_iMotr , pvRTL_XWait              );
        else if(_iPstnId == piRTL_XLIdxOutStt       ) dPos = PM.GetValue (_iMotr , pvRTL_XLIdxOutStt        );
        else if(_iPstnId == piRTL_XLIdxOutEnd       ) dPos = PM.GetValue (_iMotr , pvRTL_XLIdxOutEnd        );
        else if(_iPstnId == piRTL_XRIdxOutStt       ) dPos = PM.GetValue (_iMotr , pvRTL_XRIdxOutStt        );
        else if(_iPstnId == piRTL_XRIdxOutEnd       ) dPos = PM.GetValue (_iMotr , pvRTL_XRIdxOutEnd        );
        else if(_iPstnId == piRTL_XTrsfCheck        ) dPos = PM.GetValue (_iMotr , pvRTL_XTrsfCheck         );
        else if(_iPstnId == piRTL_XTrsfRotator      ) dPos = PM.GetValue (_iMotr , pvRTL_XTrsfRotator       );
        else if(_iPstnId == piRTL_XTrsfRotatorRvs   ) dPos = PM.GetValue (_iMotr , pvRTL_XTrsfRotatorRvs    );
        else if(_iPstnId == piRTL_XVisnCheck        ) dPos = PM.GetValue (_iMotr , pvRTL_XVisnCheck         );
        else if(_iPstnId == piRTL_XVisnStgCnt       ) dPos = PM.GetValue (_iMotr , pvRTL_XVisnStgCnt        );
        else if(_iPstnId == piRTL_XVisnHtCheck      ) dPos = PM.GetValue (_iMotr , pvRTL_XVisnHtCheck       );
        else if(_iPstnId == piRTL_XVisnCfCheck      ) dPos = PM.GetValue (_iMotr , pvRTL_XVisnCfCheck       );
    }
    else if(_iMotr == miRTL_YTrspr){
             if(_iPstnId == piRTL_YTrsfWait         ) dPos = PM.GetValue (_iMotr , pvRTL_YTrsfWait          );
        else if(_iPstnId == piRTL_YTrsfCheck        ) dPos = PM.GetValue (_iMotr , pvRTL_YTrsfCheck         );
        else if(_iPstnId == piRTL_YTrsfRoatator     ) dPos = PM.GetValue (_iMotr , pvRTL_YTrsfRoatator      );
        else if(_iPstnId == piRTL_YTrsfRotatorRvs   ) dPos = PM.GetValue (_iMotr , pvRTL_YTrsfRotatorRvs    );
    }
    else if(_iMotr == miRTL_ZTrsfr){
             if(_iPstnId == piRTL_ZTrsfrWait        ) dPos = PM.GetValue (_iMotr , pvRTL_ZTrsfrWait         );
        else if(_iPstnId == piRTL_ZTrsfrPick        ) dPos = PM.GetValue (_iMotr , pvRTL_ZTrsfrPick         );
        else if(_iPstnId == piRTL_ZTrsfrPlace       ) dPos = PM.GetValue (_iMotr , pvRTL_ZTrsfrPlace        );
    }
    else if(_iMotr == miRTL_ZVisn){
             if(_iPstnId == piRTL_ZVisnWait         ) dPos = PM.GetValue (_iMotr , pvRTL_ZVisnWait          );
        else if(_iPstnId == piRTL_ZVisnCheckWork    ) dPos = PM.GetValue (_iMotr , pvRTL_ZVisnCheckWork     );
        else if(_iPstnId == piRTL_ZVisnCmsAlign     ) dPos = PM.GetValue (_iMotr , pvRTL_ZVisnCmsAlign      );
        else if(_iPstnId == piRTL_ZVisnCmsConfocal  ) dPos = PM.GetValue (_iMotr , pvRTL_ZVisnCmsConfocal   );

    }
    else if(_iMotr == miRTL_YConvs){
             if(_iPstnId == piRTL_YConvsWait        ) dPos = PM.GetValue (_iMotr , pvRTL_YConvsWait         );
        else if(_iPstnId == piRTL_YConvsWork1st     ) dPos = PM.GetValue (_iMotr , pvRTL_YConvsWork1st      );
        else if(_iPstnId == piRTL_YConvsWork2nd     ) dPos = PM.GetValue (_iMotr , pvRTL_YConvsWork2nd      );
    }
    else if(_iMotr == miRTL_YVisn){
             if(_iPstnId == piRTL_YVisnWait         ) dPos = PM.GetValue (_iMotr , pvRTL_YVisnWait          );
        else if(_iPstnId == piRTL_YVisnCheck        ) dPos = PM.GetValue (_iMotr , pvRTL_YVisnCheck         );
        else if(_iPstnId == piRTL_YVisnStgCnt       ) dPos = PM.GetValue (_iMotr , pvRTL_YVisnStgCnt        );
        else if(_iPstnId == piRTL_YVisnHtCheck      ) dPos = PM.GetValue (_iMotr , pvRTL_YVisnHtCheck       );
        else if(_iPstnId == piRTL_YVisnCfCheck      ) dPos = PM.GetValue (_iMotr , pvRTL_YVisnCfCheck       );

    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CRightTool::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CRightTool::Update()
{
    if(Step.iSeq == scIdle){
        if(!MT_GetStop(miRTL_XGenRr) &&((IO_GetXUp(xRTL_PullDetect) && AT_GetCmd(aiRTL_LWkPshUpDn)) ||
                                        (IO_GetXUp(xRTL_OutDetect ) && AT_GetCmd(aiRTL_RWkPshUpDn)))){
            MT_Stop(miRTL_XGenRr);
            EM_SetErrMsg(eiPRT_OverLoad , "OverLoad Sensing!");
        }
    }
}

bool CRightTool::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CRightTool::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CRightTool::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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

        case 10:


                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork)) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;
                 //Rs232_LoadCl.WeightCheck(); //��� �ϱ����� ���� ���� Ȯ�� �� ��� �׽�Ʈ..
                 Step.iToStart++;
                 return false ;

        case 12: //if(m_tmDelay.OnDelay(true , 2000)) {
                 //    EM_SetErrMsg(eiPRT_LoadCell , "�ε弿 ����� �����Ǿ����ϴ�.");
                 //    Step.iToStart=0;
                 //    return true ;
                 //}
                 //
                 //if(!Rs232_LoadCl.GetMsgEnd      ()) return false;
                 if(OM.DevOptn.bUsePush && !IO_GetX(xRTL_TrsfJigDetect)) {
                     EM_SetErrMsg(eiPRT_Detect , "Use Push �����ε� Weight Block�� ���� ���� �ʽ��ϴ�.");
                 }
                 if(!OM.DevOptn.bUsePush && IO_GetX(xRTL_TrsfJigDetect)) {
                     EM_SetErrMsg(eiPRT_Detect , "Use Push ���°� �ƴѵ� Weight Block�� ���� �˴ϴ�.");
                 }

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CRightTool::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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

        case 10: MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork)) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                 //if(!IO_GetX(xSTG_StgVcum1) && !IO_GetX(xSTG_StgVcum1)){
                 //    HexaPot.MoveAbsRes(paZ , HEX_Z_WAIT);
                 //    HexaPot.Move();
                 //}

                 //Rs232UV.SetCuringOnOff(uvCh1, false);
                 //Rs232UV.SetCuringOnOff(uvCh2, false);
                 //Rs232UV.SetCuringOnOff(uvCh3, false);
                 //Rs232UV.SetCuringOnOff(uvCh4, false);
                 //
                 //IO_SetY(ySTG_UVAirSol , false);

                 Step.iToStop++;
                 return false ;

        case 12: if(!HexaPot.GetStopInpos()) return false ;

                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CRightTool::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if(CRightTool::GetSeqStep() != CRightTool::scWkoToPtl &&  DM.ARAY[riPTL].CheckAllStat(csNone) &&  IO_GetX(xPTL_Detect6)) {EM_SetErrMsg(eiPKG_Unknwn , "����Ʈ �ƿ� ���� �� �� ���� ���簡 �ֽ��ϴ�."); return false;}
    if(CRightTool::GetSeqStep() != CRightTool::scOut      && !DM.ARAY[riPTL].CheckAllStat(csNone) && !IO_GetX(xPTL_Detect6)) {EM_SetErrMsg(eiPKG_Dispr  , "����Ʈ �ƿ� ���� ���簡 ��������ϴ�."       ); return false;}


    //Check Error & Decide Step.
    if (Step.iSeq == scIdle) {
        if (Stat.bReqStop)return false ;

        bool bStgNeedPick ;
        if(OM.DevOptn.bUseFstDisp){
            bStgNeedPick = DM.ARAY[riSTG].GetCntStat  (csCmsRNeed) != 0 || DM.ARAY[riSTG].GetCntStat  (csCmsLDisp1) != 0 ;
        }         //(DM.ARAY[riSTG].GetCntStat(csCmsLDisp1)+DM.ARAY[riSTG].GetCntStat(csMidREndHeight) == OM.DevInfo.iMidColCnt * OM.DevInfo.iMidRowCnt) ;//DM.ARAY[riSTG].GetCntStat(csCmsLDisp1     );
        else { //3���� �����Ǿ��ִ���...
            bStgNeedPick = DM.ARAY[riSTG].CheckAllStat(csCmsRNeed , csCmsLDisp1 , csMidREndHeight); //DM.ARAY[riSTG].GetCntStat  (csCmsRNeed) + DM.ARAY[riSTG].GetCntStat(csMidREndHeight) == CAD.GetCol() * CAD.GetRow() ;
        }

        //Transfer Work             //���� ���������� ����� �ְ� ����� �������� ��� �۾��� ��������.
        bool isCycleTrsfPick      = LTL.GetSeqStep() == CLeftTool::scIdle  &&
                                    bStgNeedPick                           &&
                                    DM.ARAY[riTRF].CheckAllStat(csNone  )  &&
                                    DM.ARAY[riRTT].CheckAllStat(csAttach)  && OM.CmnOptn.iWorkMode == wmNormal;
        bool isCycleTrsfPlace     = LTL.GetSeqStep() == CLeftTool::scIdle &&
                                    DM.ARAY[riTRF].CheckAllStat(csAttach)  &&
                                    DM.ARAY[riSTG].GetCntStat  (csCmsRNeed)&& OM.CmnOptn.iWorkMode == wmNormal;

        //Jinseop
        bool isCycleMidRHeight    = OM.CmnOptn.iWorkMode == 0 && LTL.GetSeqStep() == CLeftTool::scIdle && DM.ARAY[riSTG].GetCntStat(csMidRHeight   );

        bool isCycleCmsRHeight    = LTL.GetSeqStep() == CLeftTool::scIdle && DM.ARAY[riSTG].GetCntStat(csCmsRHeight   );
        bool isCycleCmsRAlign     = LTL.GetSeqStep() == CLeftTool::scIdle && (DM.ARAY[riSTG].GetCntStat(csCmsRAlign    ) || DM.ARAY[riSTG].GetCntStat(csCmsRPreAlign));
        bool isCycleCmsRZHeight   = LTL.GetSeqStep() == CLeftTool::scIdle && DM.ARAY[riSTG].GetCntStat(csCmsRZHeight  );
        bool isCycleCmsREndAlign  = LTL.GetSeqStep() == CLeftTool::scIdle &&
                                   (DM.ARAY[riSTG].GetCntStat(csCmsREndAlign ) ||
                                    DM.ARAY[riSTG].GetCntStat(csCmsRFixAlign ) );

        bool isCycleMidREndHeight = LTL.GetSeqStep() == CLeftTool::scIdle &&
                                   (DM.ARAY[riSTG].GetCntStat  (csMidREndHeight) &&DM.ARAY[riSTG].CheckAllStat(csMidREndHeight , csWorkEnd) || //�Ǹ����� 2���̸� 2�� �ٺ��̰� üũ �ϴ°�
                                    DM.ARAY[riSTG].GetCntStat  (csCmsRFixHeight)|| //������ ���� Ȯ�� �ϴ°�.
                                    DM.ARAY[riSTG].GetCntStat  (csCmsREndHeight)); //���̱� ������ Ȯ�� �ϴ°�.
        bool isCycleWkoToPtl      = DM.ARAY[riWKO].CheckAllStat(csWork    ) &&
                                    DM.ARAY[riPTL].CheckAllStat(csNone    ) &&
                                    (LTL.GetSeqStep() == CLeftTool::scIdle || LTL.GetSeqStep() == CLeftTool::scMidSupply);

        //Unloader Work
        bool isCycleOut           = LTL.GetSeqStep() == CLeftTool::scIdle &&
                                    DM.ARAY[riPTL].CheckAllStat(csWork)&&
                                    DM.ARAY[riULD].GetCntStat  (csEmpty)&& ULD.GetSeqStep() == CUnLoader::scIdle ;

        //Jinseop
        bool isCycleMidRCheck     = OM.CmnOptn.iWorkMode == 2 && LTL.GetSeqStep() == CLeftTool::scIdle && DM.ARAY[riSTG].GetCntStat(csMidRHeight   );

        bool isCycleToolVoid      = Stat.bReqToolVoid ;

        bool isCycleEnd           = DM.ARAY[riSTG].CheckAllStat(csNone )&&
                                    DM.ARAY[riRTT].CheckAllStat(csNone )&&
                                    DM.ARAY[riPTL].CheckAllStat(csNone )&&
                                    DM.ARAY[riWKO].CheckAllStat(csNone );

        if(EM_IsErr()) return false ;


             if (isCycleTrsfPick     ){Trace(m_sPartName.c_str(),"TrsfPick      Stt"); Step.iSeq = scTrsfPick      ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleTrsfPlace    ){Trace(m_sPartName.c_str(),"TrsfPlace     Stt"); Step.iSeq = scTrsfPlace     ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleMidRHeight   ){Trace(m_sPartName.c_str(),"MidRHeight    Stt"); Step.iSeq = scMidRHeight    ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleCmsRHeight   ){Trace(m_sPartName.c_str(),"CmsRHeight    Stt"); Step.iSeq = scCmsRHeight    ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleCmsRAlign    ){Trace(m_sPartName.c_str(),"CmsRAlign     Stt"); Step.iSeq = scCmsRAlign     ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleCmsRZHeight  ){Trace(m_sPartName.c_str(),"CmsRZHeight   Stt"); Step.iSeq = scCmsRZHeight   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleCmsREndAlign ){Trace(m_sPartName.c_str(),"CmsREndAlign  Stt"); Step.iSeq = scCmsREndAlign  ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleMidREndHeight){Trace(m_sPartName.c_str(),"MidREndHeight Stt"); Step.iSeq = scMidREndHeight ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleWkoToPtl     ){Trace(m_sPartName.c_str(),"WkoToPtl      Stt"); Step.iSeq = scWkoToPtl      ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleOut          ){Trace(m_sPartName.c_str(),"Out           Stt"); Step.iSeq = scOut           ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleMidRCheck    ){Trace(m_sPartName.c_str(),"MidRCheck     Stt"); Step.iSeq = scMidRCheck     ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleToolVoid     ){Trace(m_sPartName.c_str(),"ToolVoid      Stt"); Step.iSeq = scToolVoid      ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd          ){Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    Step.iLastSeq = Step.iSeq ;
    switch (Step.iSeq) {
        default              :                           Trace(m_sPartName.c_str(),"default            End");                                                   Step.iSeq = scIdle ;  return false ;
        case scIdle          :                                                                                                                                                        return false ;
        case scTrsfPick      : if(CycleTrsfPick     ()){ Trace(m_sPartName.c_str(),"CycleTrsfPick      End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scTrsfPlace     : if(CycleTrsfPlace    ()){ Trace(m_sPartName.c_str(),"CycleTrsfPlace     End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scMidRHeight    : if(CycleMidRHeight   ()){ Trace(m_sPartName.c_str(),"CycleMidRHeight    End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scCmsRHeight    : if(CycleCmsRHeight   ()){ Trace(m_sPartName.c_str(),"CycleCmsRHeight    End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scCmsRAlign     : if(CycleCmsRAlign    ()){ Trace(m_sPartName.c_str(),"CycleCmsRAlign     End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scCmsRZHeight   : if(CycleCmsRZHeight  ()){ Trace(m_sPartName.c_str(),"CycleCmsRZHeight   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scCmsREndAlign  : if(CycleCmsREndAlign ()){ Trace(m_sPartName.c_str(),"CycleCmsREndAlign  End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scMidREndHeight : if(CycleMidREndHeight()){ Trace(m_sPartName.c_str(),"CycleMidREndHeight End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scWkoToPtl      : if(CycleWkoToPtl     ()){ Trace(m_sPartName.c_str(),"CycleWkoToPtl      End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scOut           : if(CycleOut          ()){ Trace(m_sPartName.c_str(),"CycleOut           End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scMidRCheck     : if(CycleMidRCheck    ()){ Trace(m_sPartName.c_str(),"CycleMidRCheck     End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scToolVoid      : if(CycleToolVoid     ()){ Trace(m_sPartName.c_str(),"CycleToolVoid      End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CRightTool::CycleHome()     //sun DLL direct access.
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

        case 10: if(IO_GetX(xRTL_TransfrVcum1) || IO_GetX(xRTL_TransfrVcum1)){
                     EM_SetErrMsg(eiPRT_RemovePkg , "Ʈ�����ۿ� ������ �������ּ���.");
                     Step.iHome = 0 ;
                     return true ;
                 }

                 if(!DM.ARAY[riTRF].CheckAllStat(csNone)){
                     EM_SetErrMsg(eiPRT_RemovePkg , "Ʈ�����ۿ� ������ �����͸� �����ϼ���.");
                     Step.iHome = 0 ;
                     return true ;
                 }

                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);
                 MT_DoHome(miRTL_ZTrsfr);
                 MT_DoHome(miRTL_ZVisn );

                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;
                 if(!MT_GetHomeEnd(miRTL_ZTrsfr)) return false ;
                 if(!MT_GetHomeEnd(miRTL_ZVisn )) return false ;

                 MT_DoHome(miRTL_XGenRr);

                 MT_DoHome(miRTL_YConvs);
                 MT_DoHome(miRTL_YTrspr);
                 MT_DoHome(miRTL_YVisn );

                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miRTL_XGenRr)) return false ;
                 //if(!MT_GetHomeResult(miRTL_XGenRr)) return false ;
                 if(!MT_GetHomeEnd(miRTL_YConvs)) return false ;
                 if(!MT_GetHomeEnd(miRTL_YTrspr)) return false ;
                 if(!MT_GetHomeEnd(miRTL_YVisn )) return false ;

                 ////Gantry�ϱ� �� �Ŀ� ������ ����.
                 MT_SetHomeEnd(miRTL_XGenFt , true);

                 MT_GoAbsMan(miRTL_ZTrsfr , PM.GetValue(miRTL_ZTrsfr , pvRTL_ZTrsfrWait));
                 MT_GoAbsMan(miRTL_ZVisn  , PM.GetValue(miRTL_ZVisn  , pvRTL_ZVisnWait ));

                 MT_GoAbsMan(miRTL_XGenRr , PM.GetValue(miRTL_XGenRr , pvRTL_XWait     ));
                 MT_GoAbsMan(miRTL_YConvs , PM.GetValue(miRTL_YConvs , pvRTL_YConvsWait));
                 MT_GoAbsMan(miRTL_YTrspr , PM.GetValue(miRTL_YTrspr , pvRTL_YTrsfWait ));
                 MT_GoAbsMan(miRTL_YVisn  , PM.GetValue(miRTL_YVisn  , pvRTL_YVisnWait ));

                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miRTL_ZTrsfr , PM.GetValue(miRTL_ZTrsfr , pvRTL_ZTrsfrWait))) return false ;
                 if(!MT_GoAbsMan(miRTL_ZVisn  , PM.GetValue(miRTL_ZVisn  , pvRTL_ZVisnWait ))) return false ;

                 if(!MT_GoAbsMan(miRTL_XGenRr , PM.GetValue(miRTL_XGenRr , pvRTL_XWait     ))) return false ;
                 if(!MT_GoAbsMan(miRTL_YConvs , PM.GetValue(miRTL_YConvs , pvRTL_YConvsWait))) return false ;
                 if(!MT_GoAbsMan(miRTL_YTrspr , PM.GetValue(miRTL_YTrspr , pvRTL_YTrsfWait ))) return false ;
                 if(!MT_GoAbsMan(miRTL_YVisn  , PM.GetValue(miRTL_YVisn  , pvRTL_YVisnWait ))) return false ;

                 //����Ѱ� ù���� �׻� ������ ���� �ѹ� ������ �غ���.
                 Rs232Keyence.CheckHeight(kcChA); //Right����.

                 Step.iHome = 0;
                 return true ;

    }
}

bool CRightTool::CycleMidRHeight()
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    static int    iHeightCnt = 0 ;
    static double dHeightSum = 0.0;
    static double dTempX ;
    static double dTempY ;

    static int iHtStep ;
    static double dCrntHeight ;

    static CCadRhombus * pCadRhombus ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: 
                 WorkInfo.eStat    = csMidRHeight   ;
                 FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG);
                 //WorkInfo.iRow  = 0 ;
                 //WorkInfo.iCol  = 0 ;
                 pCadRhombus = CAD.MB.MB_RHgt ;
                 SetCadOfsGain(tlRHght );

                 Step.iCycle++;
                 return false ;

        case 11: LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);
                 //MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     );
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);
                 HexaPot.MoveAbsRes(paX, HEX_X_WAIT);
                 HexaPot.MoveAbsRes(paY, HEX_Y_WAIT);
                 HexaPot.MoveAbsRes(paZ, HEX_Z_WAIT);
                 HexaPot.MoveAbsRes(paU, HEX_U_WAIT);
                 HexaPot.MoveAbsRes(paV, HEX_V_WAIT);
                 HexaPot.MoveAbsRes(paW, HEX_W_WAIT);
                 HexaPot.Move();

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork)) return false ;
                 //if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     )) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;
                 if(!HexaPot.GetStopInpos()) return false ;

                 MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false ;

                 iHeightCnt = 0 ;

                 Step.iCycle++;
                 return false ;

        //�ؿ��� ��.
        case 15:

                 if(iHeightCnt == 0 ){//������ ����.
                     dTempX = pCadRhombus -> GetCntrPosX() - g_tMidPosOfs.dX;
                     dTempY = pCadRhombus -> GetTop()      - g_tMidPosOfs.dY;
                 }
                 else if(iHeightCnt == 1 ){//������ ������.
                     dTempX = pCadRhombus -> GetRight()    - g_tMidPosOfs.dX;
                     dTempY = pCadRhombus -> GetCntrPosY() - g_tMidPosOfs.dY;
                 }
                 else if(iHeightCnt == 2 ){//������ �Ʒ���
                     dTempX = pCadRhombus -> GetCntrPosX() - g_tMidPosOfs.dX;
                     dTempY = pCadRhombus -> GetBottom()   - g_tMidPosOfs.dY;
                 }
                 else if(iHeightCnt == 3 ){//������ ����.
                     dTempX = pCadRhombus -> GetLeft()     - g_tMidPosOfs.dX;
                     dTempY = pCadRhombus -> GetCntrPosY() - g_tMidPosOfs.dY;
                 }
                 //MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 //MT_GoAbsRun(miRTL_YVisn  , dTempY);
                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleSigleHeightCheck(iHtStep  , dCrntHeight ,dTempX , dTempY , false);

                 Step.iCycle++;
                 return false ;

        //�������� ���� ����Ŭ �߰� . ����
        case 16: if(!CycleSigleHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY, false)) return false ;
                 SendLog(String("MB Height:")+dCrntHeight);
                 Trace("MB Height:" ,String(dCrntHeight).c_str());
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , "Right ������ ���� ������ ������������ Ȯ���ϼ���.");
                     Step.iCycle=0;
                     return true ;
                 }



                 Step.iCycle++ ;
                 return false ;

        case 17: if(OM.DevOptn.dMBHghtTol != 0) {
                     if((dCrntHeight>OM.DevOptn.dMBStdHght+OM.DevOptn.dMBHghtTol) ||
                         (dCrntHeight<OM.DevOptn.dMBStdHght-OM.DevOptn.dMBHghtTol)) {
                         EM_SetErrMsg(eiHGT_RangeErr , "Right ���� ���� ������ �����ϰ� �߻��ϰ� �ֽ��ϴ�.");
                         Step.iCycle = 0;
                         return true ;
                     }
                 }

		 if(iHeightCnt == 0 ){
                     dHeightSum = dCrntHeight ;
                 }
                 else {//������.
                     dHeightSum += dCrntHeight ;
                 }
                 iHeightCnt++;
                 if(iHeightCnt < 4 ){
                     Step.iCycle=15 ;
                     return false ;
                 }
                 STG.PkgStat.dMidTopHeight = dHeightSum / 4.0 ;

                 DM.ARAY[riSTG].SetStat(csCmsLDisp1);

                 Step.iCycle=0;
                 return true ;
    }
}

bool CRightTool::CycleTrsfPick()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true ;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    String sModID ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 11: if(!LTL.ToolVoid()) return false ;



                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait );
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait)) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait )) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                 MoveMotr(miRTL_YVisn , piRTL_YVisnWait) ;
                 MoveMotr(miRTL_YConvs , piRTL_YConvsWait    );

                 //�۾� �Ϸ�� ���̵��� Ȧ���̸� 180��
                 //¦���̸� 0��.
                 if(!OM.DevOptn.bNotUseRotate&& DM.ARAY[riSTG].GetCntStat(csMidREndHeight)%2){
                     MoveMotr(miRTL_XGenRr , piRTL_XTrsfRotatorRvs  );
                     MoveMotr(miRTL_YTrspr , piRTL_YTrsfRotatorRvs  );
                 }
                 else {
                     MoveMotr(miRTL_XGenRr , piRTL_XTrsfRotator  );
                     MoveMotr(miRTL_YTrspr , piRTL_YTrsfRoatator );
                 }

                 Step.iCycle++ ;
                 return false ;

        case 13: if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait )) return false ;
                 if(!MoveMotr(miRTL_YConvs , piRTL_YConvsWait)) return false ;
                 //�۾� �Ϸ�� ���̵��� Ȧ���̸� 180��
                 //¦���̸� 0��.
                 if(!OM.DevOptn.bNotUseRotate && DM.ARAY[riSTG].GetCntStat(csMidREndHeight)%2){
                     IDX.MoveMotr(miIDX_TRotat , piIDX_TRotatRotate);
                 }
                 else {
                     IDX.MoveMotr(miIDX_TRotat , piIDX_TRotatNormal);
                 }


                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miRTL_YConvs , piRTL_YConvsWait    )) return false ;
                 if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YTrspr)) return false ;
                 if(!MT_GetStopInpos(miIDX_TRotat)) return false ;

                 IO_SetY(yTRS_TRSIonizer , true);
                 MT_GoAbsRun(miRTL_ZTrsfr , PM.GetValue(miRTL_ZTrsfr , pvRTL_ZTrsfrPick) - 10);
                 Step.iCycle++;
                 return false ;

        case 15: if(!MT_GetStopInpos(miRTL_ZTrsfr)) return false;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrPick, true);




                 Step.iCycle++;
                 return false ;

        case 16: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrPick , true)) return false ;

                 //¦¦�� �������� �߰�.
                 //�۾��Ϸ�� ������ �ľ��ؼ� 1������ ���� 2������ ���� ����.
                 if(DM.ARAY[riSTG].GetCntStat(csMidREndHeight) %2){
                     MoveMotr(miRTL_YConvs , piRTL_YConvsWork2nd);
                 }
                 else {
                     MoveMotr(miRTL_YConvs , piRTL_YConvsWork1st);
                 }



                 //if((DM.ARAY[riSTG].GetCntStat(csCmsRNeed) + DM.ARAY[riSTG].GetCntStat(csCmsLDisp1))%2){
                 //    MoveMotr(miRTL_YConvs , piRTL_YConvsWork1st);
                 //}
                 //else {
                 //    MoveMotr(miRTL_YConvs , piRTL_YConvsWork2nd);
                 //}


                 //if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrPick , true)) return false ;
                 //MoveMotr(miRTL_YConvs , piRTL_YConvsWork);

                 //20160420 ���� ����


                 Step.iCycle++;
                 return false ;

        case 17: if(!MT_GetStop(miRTL_YConvs)) return false;

                 IO_SetY(yRTL_TransfrVac1 , true);
                 IO_SetY(yRTL_TransfrVac2 , true);
                 IO_SetY(yTRS_TRSIonizer  , false);
                 IO_SetY(yIDX_RttPumpOn   , false);
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 Step.iCycle++;
                 return false ;

        case 18: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait)) return false ;

                 sModID = DM.ARAY[riRTT].GetID();
                 DM.ARAY[riTRF].SetStat(csAttach);
                 DM.ARAY[riTRF].CHPS[0][0].SetID(sModID);
                 DM.ARAY[riTRF].SetID(sModID);
                 DM.ARAY[riRTT].ClearMap();

                 Step.iCycle=0;
                 return true ;
    }
}

bool CRightTool::CycleTrsfPlace()
{
//Check Cycle Time Out.                                                                OM.DevOptn.dAttachDelay
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && Step.iCycle!=29 && !OM.MstOptn.bDebugMode, 6000 )) {
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    //memset(&WorkInfo , 0 , sizeof(TWorkInfo);
    //     if (isCycleTrsfPick     ){WorkInfo.eStat = csTrsfPick      FindChip(WorkInfo.iRow , WorkInfo.iRow , WorkInfo.eStat , riSTG);}
    //else if (isCycleTrsfPlace    ){WorkInfo.eStat = csTrsfPlace     FindChip(WorkInfo.iRow , WorkInfo.iRow , WorkInfo.eStat , riSTG);}
    //else if (isCycleMidRHeight   ){WorkInfo.eStat = csMidRHeight    FindChip(WorkInfo.iRow , WorkInfo.iRow , WorkInfo.eStat , riSTG);}
    //else if (isCycleCmsRHeight   ){WorkInfo.eStat = csCmsRHeight    FindChip(WorkInfo.iRow , WorkInfo.iRow , WorkInfo.eStat , riSTG);}
    //else if (isCycleCmsRAlign    ){WorkInfo.eStat = csCmsRAlign     FindChip(WorkInfo.iRow , WorkInfo.iRow , WorkInfo.eStat , riSTG);}
    //else if (isCycleCmsRGap      ){WorkInfo.eStat = csCmsREndAlign  FindChip(WorkInfo.iRow , WorkInfo.iRow , WorkInfo.eStat , riSTG);}
    //else if (isCycleCmsRNeed     ){}
    //else if (isCycleMidREndHeight){WorkInfo.eStat = csMidREndHeight FindChip(WorkInfo.iRow , WorkInfo.iRow , WorkInfo.eStat , riSTG);}
    //else if (isCycleOut          ){WorkInfo.eStat = csOut           FindChip(WorkInfo.iRow , WorkInfo.iRow , WorkInfo.eStat , riULD);}

    static double dTempX ;
    static double dTempY ;

    double dVel1   ;
    double dVel2   ;
    double dVelMax ;

    double dTempHeight ;

    static CCadRect * pPcbRect ;
    static CCadRect * pCmsRect ;

    static int    iHtStep     ;
    static double dCrntHeight ;



    String sModID ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10:
                 WorkInfo.eStat    = csCmsRNeed    ;
                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG)){
                     Step.iCycle = 0 ;
                     return true ;

                 }

                 pPcbRect = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].PC_Rect ;
                 pCmsRect = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_Rect ;

                 Rs232_LoadCl.AutoZero();

                 LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 11: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    );
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);
                 MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     );
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 HexaPot.MoveAbsRes(paZ, HEX_Z_WAIT);
                 HexaPot.Move();

                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 Trace("Check 12" , "piRTL_ZTrsfrWait");
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork)) return false ;
                 Trace("Check 12" , "piRTL_ZVisnCheckWork");
                 if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     )) return false ;
                 Trace("Check 12" , "piRTL_YVisnWait");
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 Trace("Check 12" , "aiRTL_LWkPshUpDn");
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;
                 Trace("Check 12" , "aiRTL_RWkPshUpDn");
                 if(!HexaPot.GetStopInpos()) return false ;
                 Trace("Check 12" , "HexaPot");


                 HexaPot.MoveAbsRes(paX, HEX_X_WAIT);
                 HexaPot.MoveAbsRes(paY, HEX_Y_WAIT);

                 HexaPot.MoveAbsRes(paU, HEX_U_WAIT);
                 HexaPot.MoveAbsRes(paV, HEX_V_WAIT);
                 HexaPot.MoveAbsRes(paW, HEX_W_WAIT);
                 HexaPot.Move();

                 MoveMotr(miRTL_YVisn , piRTL_YVisnWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 13: if(!MoveMotr(miRTL_YVisn , piRTL_YVisnWait)) return false ;
                 if(!HexaPot.GetStopInpos()) return false ;
                 
                 //�������� XY
                 SetCadOfsGain(tlStage); //���������� ���� ���� ���⶧���� �������� ���� ����� �Ѵ�.
                 dTempX = pCmsRect -> GetCntrPosX() + g_tMidPosOfs.dX;
                 dTempY = pCmsRect -> GetCntrPosY() + g_tMidPosOfs.dY;
                 if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 MT_GoAbsRun(miSTG_XStage , dTempX);
                 MT_GoAbsRun(miSTG_YStage , dTempY);

                 //MoveMotr(miRTL_YTrspr , piRTL_YTrsfRoatator );

                 Step.iCycle++;
                 return false ;

        case 14: if(!MT_GetStopInpos(miSTG_XStage)) return false ;
                 if(!MT_GetStopInpos(miSTG_YStage)) return false ;

                 //if(!MoveMotr(miRTL_YTrspr  , piRTL_YTrsfRoatator )) return false ;

                 HexaPot.MoveAbsRes(paX, HEX_X_WAIT);
                 HexaPot.MoveAbsRes(paY, HEX_Y_WAIT);
                 HexaPot.MoveAbsRes(paZ, HEX_Z_LORI);
                 HexaPot.MoveAbsRes(paU, HEX_U_WAIT);
                 HexaPot.MoveAbsRes(paV, HEX_V_WAIT);
                 HexaPot.MoveAbsRes(paW, HEX_W_WAIT);

                 HexaPot.Move();
                 Step.iCycle++;
                 return false ;

        case 15: if(!HexaPot.GetStopInpos()) return false ;


                 //�ޱ� ���� �������� ž�� ���� ����.
                 //CAD.CM[WorkInfo.iCol][WorkInfo.iRow].PC_Rect ;
                 SetCadOfsGain(tlRHght); //���������� ��(Ʈ������,����������)�� ���� ���⶧���� �������� ���� ����� �Ѵ�.
                 dTempX = pCmsRect -> GetCntrPosX() - g_tMidPosOfs.dX;
                 dTempY = pCmsRect -> GetCntrPosY() - g_tMidPosOfs.dY;
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;



                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);

                 Step.iCycle++;
                 return false ;

        case 16: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;

                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleSigleHeightCheck(iHtStep  , dCrntHeight ,dTempX , dTempY, false);

                 Step.iCycle++;
                 return false ;

        case 17: if(!CycleSigleHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY, false)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , "Right ������ ���� ������ ������������ Ȯ���ϼ���.");
                     Step.iCycle=0;
                     return true ;
                 }
                 WorkHeightInfo.dSubPntHeight = dCrntHeight ;
                 Step.iCycle++;
                 return false ;

        case 18: STG.PkgStat.dHexaTopHeight = dCrntHeight+OM.DevOptn.dHexaTopHeightOfs ;
                 SendLog(String("HexaTopHeight:")+STG.PkgStat.dHexaTopHeight);
                 Trace("HexaTopHeight:",String(STG.PkgStat.dHexaTopHeight).c_str());
                 //���� ���� ���� �߰�. ����
                 if(OM.DevOptn.dHexaTopHghtTol != 0) {
                     if(((STG.PkgStat.dHexaTopHeight-OM.DevOptn.dHexaTopStdHght)>OM.DevOptn.dHexaTopHghtTol) ||
                         (STG.PkgStat.dHexaTopHeight-OM.DevOptn.dHexaTopStdHght)<(-OM.DevOptn.dHexaTopHghtTol)) {
                         EM_SetErrMsg(eiHGT_RangeErr , "Right ���� ���� ������ �����ϰ� �߻��ϰ� �ֽ��ϴ�.");
                         Step.iCycle = 0;
                         return true ;
                     }
                 }

                 MoveMotr(miRTL_YVisn , piRTL_YVisnWait);
                 HexaPot.MoveAbsRes(paZ, HEX_Z_GET);
                 HexaPot.Move();




                 Step.iCycle++;
                 return false ;

        case 19: if(!MoveMotr(miRTL_YVisn , piRTL_YVisnWait)) return false ;
                 if(!HexaPot.GetStopInpos()) return false ;

                 //Ʈ������ XY
                 SetCadOfsGain(tlRTrns );
                 dTempX = pCmsRect -> GetCntrPosX() - g_tMidPosOfs.dX;
                 dTempY = pCmsRect -> GetCntrPosY() + g_tMidPosOfs.dY;
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;

                 //20160720
                 //¦¦�� ���� �������� Ʈ�����۰� �������Ϳ��� �����Ϳ��� ������ ���Ѵ�.
                 //dTempX += PM.GetValue(miRTL_XGenRr , poRTL_XTrsfPlceOfs) ;


                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YTrspr , dTempY);
                 Step.iCycle++;
                 return false ;

        case 20: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YTrspr)) return false ;
                 MT_GoAbsRun(miRTL_ZTrsfr , PM.GetValue(miRTL_ZTrsfr, pvRTL_ZTrsfrPlace) - 10);
                 Step.iCycle++;
                 return false ;

        case 21: if(!MT_GetStopInpos(miRTL_ZTrsfr)) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrPlace , true);

                 //Ʈ������ �̵� �� ��Ũ ���� 20160419
                 if(DM.ARAY[riSTG].GetCntStat(csMidREndHeight)){
                     if(OM.DevOptn.bUseSTG_Vac1) IO_SetY(ySTG_VcumPump1On , true);
                     if(OM.DevOptn.bUseSTG_Vac2) IO_SetY(ySTG_VcumPump2On , true);
                     if(OM.DevOptn.bUseSTG_Vac3) IO_SetY(ySTG_VcumPump3On , true);
                 }
                 else {
                     if(OM.DevOptn.bUseSTG_Vac1Frst) IO_SetY(ySTG_VcumPump1On , true);
                     if(OM.DevOptn.bUseSTG_Vac2Frst) IO_SetY(ySTG_VcumPump2On , true);
                     if(OM.DevOptn.bUseSTG_Vac3Frst) IO_SetY(ySTG_VcumPump3On , true);
                 }

                 IO_SetY(yRTL_TransfrVac1 , false);
                 IO_SetY(yRTL_TransfrVac2 , false);
                 Step.iCycle++;
                 return false ;

        case 22: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrPlace , true)) return false ;

                 if(!OM.DevOptn.bUsePush){
                     Step.iCycle = 28;
                     return false ;
                 }


                 Rs232_LoadCl.WeightCheck(); //��� �ϱ����� ���� ���� Ȯ�� �� ��� �׽�Ʈ..
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 23: if(m_tmDelay.OnDelay(true , 2000)) {
                     EM_SetErrMsg(eiPRT_LoadCell , "�ε弿 ����� �����Ǿ����ϴ�.");
                     Step.iCycle=0;
                     return true ;
                 }
                 if(!Rs232_LoadCl.GetMsgEnd      ()) return false;
                 if( Rs232_LoadCl.GetLoadCellData() * 1000 > OM.DevOptn.dAttachForce + OM.DevOptn.dAttachForceOfs) {
                     EM_SetErrMsg(eiPRT_LoadCell , "�ε弿�� ���԰� ������ ���� ������ �̻��Դϴ�.");
                     Step.iCycle=0;
                     return true ;
                 }

                 //�����°� �ӵ� 2�ܰ��
                 dVel1   = OM.DevOptn.dAttachSpeed1 > MT_GetRunVel(miRTL_ZTrsfr) ? MT_GetRunVel(miRTL_ZTrsfr) : OM.DevOptn.dAttachSpeed1 ;
                 dVel2   = OM.DevOptn.dAttachSpeed2 > MT_GetRunVel(miRTL_ZTrsfr) ? MT_GetRunVel(miRTL_ZTrsfr) : OM.DevOptn.dAttachSpeed2 ;
                 dVelMax = dVel1 > dVel2 ? dVel1 : dVel2 ;
                 MT_SetOverrideMaxSpeed(miRTL_ZTrsfr , dVelMax) ;

                 MT_GoInc(miRTL_ZTrsfr , 5.0 , dVel1);
                 Step.iCycle++;
                 return false ;

        //�ؿ��� ��.   1���� ���˶����� ����.
        case 24: Rs232_LoadCl.WeightCheck();
                 Step.iCycle++;
                 return false ;

        case 25: if(!Rs232_LoadCl.GetMsgEnd      ()) return false;
                 if( Rs232_LoadCl.GetLoadCellData() * 1000 > (OM.DevOptn.dAttachForce- OM.DevOptn.dAttachForceOfs)/2.0) { //50g�Ѿ�� �ӵ� ����.
                     dVel2   = OM.DevOptn.dAttachSpeed2 > MT_GetRunVel(miRTL_ZTrsfr) ? MT_GetRunVel(miRTL_ZTrsfr) : OM.DevOptn.dAttachSpeed2 ;
                     MT_SetOverrideVel(miRTL_ZTrsfr , dVel2);
                     Step.iCycle++;
                     return false ;
                 }
                 Step.iCycle = 24 ;
                 return false ;

        //�ؿ��� ��. 2���� ���� ���Ա��� ����.
        case 26: Rs232_LoadCl.WeightCheck();
                 Step.iCycle++;
                 return false ;

        case 27: if(!Rs232_LoadCl.GetMsgEnd      ()) return false;
                 if( Rs232_LoadCl.GetLoadCellData() * 1000 > OM.DevOptn.dAttachForce- OM.DevOptn.dAttachForceOfs) { //50g�Ѿ�� �ӵ� ����.
                     dVel2   = OM.DevOptn.dAttachSpeed2 > MT_GetRunVel(miRTL_ZTrsfr) ? MT_GetRunVel(miRTL_ZTrsfr) : OM.DevOptn.dAttachSpeed2 ;
                     MT_Stop(miRTL_ZTrsfr);  //�� ������ ������ Ʋ���� ����... ����.
                 }
                 if(!MT_GetStop(miRTL_ZTrsfr)) {
                     Step.iCycle = 26 ;
                     return false ;
                 }
                 Step.iCycle++;
                 return false ;

        //��������.
        case 28: m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;


        //Ÿ�Ӿƿ����ǿ����� ��.
        case 29: if(!m_tmDelay.OnDelay(true , OM.DevOptn.dAttachDelay)) return false ;  //���⿡ �������� ��Ũ ���� �־��� ���� 20160419

                 MoveMotr(miRTL_YConvs , piRTL_YConvsWait);
                 Step.iCycle++;
                 return false ;

        case 30: if(!MoveMotr(miRTL_YConvs , piRTL_YConvsWait)) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);

                 Step.iCycle++;
                 return false ;

        case 31: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait)) return false ;

                 sModID = DM.ARAY[riTRF].GetID();
                 DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsLAlign );
                 DM.ARAY[riSTG].CHPS[WorkInfo.iRow][WorkInfo.iCol].SetID(sModID);
                 DM.ARAY[riTRF].ClearMap();

                 if(WorkInfo.iCol==0 && WorkInfo.iRow == 0) {
                     STG.Spc.sModuleID00 = sModID ;
                 }
                 else if(WorkInfo.iCol==1 && WorkInfo.iRow == 0) {
                     STG.Spc.sModuleID10 = sModID ;
                 }
                 else if(WorkInfo.iCol==0 && WorkInfo.iRow == 1) {
                     STG.Spc.sModuleID01 = sModID ;
                 }
                 else {
                     STG.Spc.sModuleID11 = sModID ;
                 }

                 if(!IO_GetX(xSTG_StgVcum1) &&!IO_GetX(xSTG_StgVcum2) &&!IO_GetX(xSTG_StgVcum3)){
                     EM_SetErrMsg(eiPRT_VacErr , "Stage�� ������ �������� �ʾҽ��ϴ�.");

                 }

                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 Step.iCycle++;
                 return false ;

        case 32: if(!MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false ;
                 Step.iCycle=0;
                 return true ;
    }
}

bool CRightTool::CycleCmsRHeight()
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    static int    iHeightCnt = 0 ;

    double dTemp ;

    //ù��° �� ���϶� ����.
    static double dHeightPos[4];
    static double dHeight[4];
    double dTiltX , dTiltY ;



    //�ι�°�̻� ���϶� ����. �ﰢ�� ����̴�.
    static CCadHeight  * pStandHeightPos ;
    static CCadHeight  * pWorkHeightPos  ;

    //4��¥�� ������ ����.
    static THeightInfo * pStandHeightInfo ;
    static THeightInfo * pWorkHeightInfo  ;


    double dHeightSum      ;
    double dWorkHeightAvr  ;
    double dStandHeightAvr ;

    static double dPitch ;

    //4��° �� ���϶� 3�� �ϴ� 4�� ���.
    static CCadHeight  * pSubStandHeightPos ;
    static CCadHeight  * pSubWorkHeightPos  ;

    //static bool bWorkHeightScdWork ;
    static int  iWorkHeightCnt ;

    String sName ;


    static double dTempX ;
    static double dTempY ;

    static int iHtStep ;
    static double dCrntHeight ;




    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: 
                 WorkInfo.eStat = csCmsRHeight ;
                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG)){
                     Step.iCycle = 0 ;
                     return true ;

                 }

                 if(OM.CmnOptn.bUseConfocal)SetCadOfsGain(tlRConfocal );
                 else                       SetCadOfsGain(tlRHght     );

                 pSubStandHeightPos = NULL ;
                 pSubWorkHeightPos  = NULL ;

                 if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0){ //ù��° ���� ������� ���δ�.
                     //pFrstHeightPos  = CAD.CM_RHgtFrst ;
                          if(CAD.GetCol()>1)pWorkHeightPos = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RHgtR ; //�����۾��ϴ³��� ������� �ϴ� ����
                     else if(CAD.GetRow()>1)pWorkHeightPos = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RHgtB ; //�����۾��ϴ³��� ������� �ϴ� ����
                 }
                 else if(WorkInfo.iCol>0){
                     pStandHeightPos = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow].CM_RHgtR ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pWorkHeightPos  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow].CM_RHgtL ;

                     //���伣... 4��° ��.
                     if(WorkInfo.iRow>0){ //���� ���ʿ��� ���ߴµ�.. ���� �߻� �Ͽ� ������.
                         pSubStandHeightPos = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_RHgtB ; //�����۾��ϴ³��� ������� �ϴ� ����
                         pSubWorkHeightPos  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RHgtT ;
                     }
                 }
                 else if(WorkInfo.iRow>0){
                     pStandHeightPos = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_RHgtB ;  //�����۾��ϴ³��� ������� �ϴ� ����
                     sName = pStandHeightPos -> GetName() ;
                     pWorkHeightPos  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RHgtT ;
                     sName = pWorkHeightPos -> GetName() ;
                 }
                 else {
                     Step.iCycle = 0 ;
                     return true ;
                 }


                 Step.iCycle++;
                 return false ;

        case 11: LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);
                 MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     );
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork)) return false ;
                 if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     )) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                 MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false ;
                 if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0){ //ù��° ���� ������� ���δ�.
                     iHeightCnt = 0  ;
                     Step.iCycle=40;
                     return false ;
                 }
                 else if (WorkInfo.iCol > 0 && WorkInfo.iRow > 0){ //���伣 4��°�� ���϶�.
                     iHeightCnt = 0  ;
                     Step.iCycle=80;
                     return false ;
                 }
                 else {
                     iHeightCnt = 0  ;
                     Step.iCycle=50;
                     return false ;
                 }
                 Step.iCycle++;
                 return false ;

        //������ ��.
        //�ؿ��� ��. ù��° �� ����.
        /*
        case 30:

                 if(iHeightCnt == 0 ){//������ ����.
                     dTempX = pFrstHeightPos -> GetCntrPosX() - g_tMidPosOfs.dX;
                     dTempY = pFrstHeightPos -> GetTop()      - g_tMidPosOfs.dY;
                 }
                 else if(iHeightCnt == 1 ){//������ ������.
                     dTempX = pFrstHeightPos -> GetRight()    - g_tMidPosOfs.dX;
                     dTempY = pFrstHeightPos -> GetCntrPosY() - g_tMidPosOfs.dY;
                 }
                 else if(iHeightCnt == 2 ){//������ �Ʒ���
                     dTempX = pFrstHeightPos -> GetCntrPosX() - g_tMidPosOfs.dX;
                     dTempY = pFrstHeightPos -> GetBottom()   - g_tMidPosOfs.dY;
                 }
                 else if(iHeightCnt == 3 ){//������ ����.
                     dTempX = pFrstHeightPos -> GetLeft()     - g_tMidPosOfs.dX;
                     dTempY = pFrstHeightPos -> GetCntrPosY() - g_tMidPosOfs.dY;
                 }
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;

                 //�� �Ʒ��� Y�� ������ �¿�� X�� ������.
                 dHeightPos[iHeightCnt] = (iHeightCnt%2) ? dTempX : dTempY ;

                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY);

                 Step.iCycle++;
                 return false ;

        case 31: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }
                 dHeight[iHeightCnt] = dCrntHeight ;
                 iHeightCnt++;
                 if(iHeightCnt < 4 ){
                     Step.iCycle=30 ;
                     return false ;
                 }
                 //�ɵ嵵�� �������� �����Ѵ�. 4��и�.
                 //dTiltX = CMath::GetLineAngle(dHeightPos[3],dHeight[3],dHeightPos[0],dHeight[0]);
                 dTiltX = CMath::GetLineAngle(dHeightPos[3],dHeight[3],dHeightPos[1],dHeight[1])-180;    //0���� 360���� ���ü� �־�� �쿡�� �·� ����.
                 HexaPot.MoveAbsRes(paV , dTiltX); //ok
                 dTiltY = CMath::GetLineAngle(dHeightPos[0],dHeight[0],dHeightPos[2],dHeight[2])-180;
                 HexaPot.MoveAbsRes(paU , dTiltY); //ok
                 //���߿� ��� ��Ʈ ���� Ȯ�� �ؼ� �ϱ�.
                 //U(Y)�ð� V(X)�ð�  W�ݽð�/
                 //ƿƮX�� +�̸� ��� ���鿡�� ������ ���򿡼� �ð�������� ���ư� ����.
                 HexaPot.Move();
                 Step.iCycle++;
                 return false ;

        case 32: if(!HexaPot.GetStopInpos())return false ;
                 DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRAlign);

                 Step.iCycle=0;
                 return true ;
        */


        //������ �Ⱦ��� �׳� ���� ���� �ﰢ������ ù�� ���̴� ����.
        case 40: if(!HexaPot.GetStopInpos()) return false ;

                 iHeightCnt = 0 ;

                 //�޸� Ȯ��.
                 WorkHeightInfo.SetLineHeightCnt(OM.DevOptn.iLineHeightCnt);

                 //iLineHeightCnt �� ���� ��ġ ���.
                 if(pWorkHeightPos->GetSttPosX() == pWorkHeightPos->GetEndPosX()) { //��Ƽ��
                     WorkHeightInfo.bVertical= true ;
                     dPitch = (pWorkHeightPos->GetEndPosY() - pWorkHeightPos->GetSttPosY())/(OM.DevOptn.iLineHeightCnt-1) ;
                 }
                 else {
                     WorkHeightInfo.bVertical = false ;
                     dPitch = (pWorkHeightPos->GetEndPosX() - pWorkHeightPos->GetSttPosX())/(OM.DevOptn.iLineHeightCnt-1) ;
                 }

                 if(OM.CmnOptn.bUseConfocal)SetCadOfsGain(tlRConfocal );
                 else                       SetCadOfsGain(tlRHght     );

                 dTempX = pWorkHeightPos->GetSubPosX() - g_tMidPosOfs.dX;
                 dTempY = pWorkHeightPos->GetSubPosY() - g_tMidPosOfs.dY;
                 //ù���� �ƴϸ� �����V�� �����.. CMOS���� ���� �� �־.
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;

                 if(WorkHeightInfo.bVertical)dTempY += dPitch * iHeightCnt ;
                 else                        dTempX += dPitch * iHeightCnt ;


                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Step.iCycle++;
                 return false ;

        case 41: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }
                 WorkHeightInfo.dSubPntHeight = dCrntHeight ;
                 Trace( "Work Sub Height" ,String(WorkHeightInfo.dSubPntHeight).c_str());
                 SendLog("Work Sub Height:" + String(WorkHeightInfo.dSubPntHeight) );
                 Step.iCycle++;
                 return false ;

        //�ؿ�����.
        case 42: dTempX = pWorkHeightPos->GetSttPosX() -g_tMidPosOfs.dX;
                 dTempY = pWorkHeightPos->GetSttPosY() -g_tMidPosOfs.dY;
                 //ù���� �ƴϸ� �����V�� �����.. CMOS���� ���� �� �־.
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;
                 //���ظ��� �����V���� �ϸ� �ȉ�.
                 if(WorkHeightInfo.bVertical) dTempY += dPitch * iHeightCnt ;
                 else                         dTempX += dPitch * iHeightCnt ;

                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Step.iCycle++;
                 return false ;

        case 43: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }

                 if(WorkHeightInfo.bVertical) WorkHeightInfo.adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_YVisn );
                 else                         WorkHeightInfo.adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_XGenRr);
                 WorkHeightInfo.adLineHeight[iHeightCnt] = dCrntHeight ;

                 iHeightCnt++;
                 if(iHeightCnt < WorkHeightInfo.iLineHeightCnt ){
                     Step.iCycle=42 ;
                     return false ;
                 }

                 //�̰� �Լ� ������ �ȵǳ�...
                 //GetLineABFromPntByLeastSqure(int _iPntCnt , double * _pPntX , double * _pPntY , double & _dA , double & _dB);
                 //��ŸƮ ����Ʈ���� �׷����� ������ �ǹ���.
                 if(!CMath::GetLineABFromPntByLeastSqure(WorkHeightInfo.iLineHeightCnt  ,
                                                         WorkHeightInfo.adLineHeightPos ,
                                                         WorkHeightInfo.adLineHeight    ,
                                                         WorkHeightInfo.dLineA          ,
                                                         WorkHeightInfo.dLineB          )){
                     for(int i = 0 ; i < WorkHeightInfo.iLineHeightCnt ; i++){
                         Trace( (String("Work Pos")+i).c_str(),String(WorkHeightInfo.adLineHeightPos[i]).c_str());
                         Trace( (String("Work Height")+i).c_str(),String(WorkHeightInfo.adLineHeight   [i]).c_str());

                     }
                     EM_SetErrMsg(eiETC_CalErr , "�ּ� �ڽ¹� ���꿡��");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 for(int i = 0 ; i < WorkHeightInfo.iLineHeightCnt ; i++){
                     Trace( (String("Work Pos"   )+i).c_str(),String(WorkHeightInfo.adLineHeightPos[i]).c_str());
                     Trace( (String("Work Height")+i).c_str(),String(WorkHeightInfo.adLineHeight   [i]).c_str());
                     SendLog(String("Work Height")+i +":" + String(WorkHeightInfo.adLineHeight   [i]));

                 }


                 //ó���������� ������ �߰��� ���ؼ�
                 //�̰���.. ������ �������� Ȯ�� �ϴ� ���.
                 dTemp = (WorkHeightInfo.adLineHeightPos[WorkHeightInfo.iLineHeightCnt-1] + WorkHeightInfo.adLineHeightPos[0])/2.0 ;
                 //�ּ��ڽ¹����� ���� ���������Ŀ� �����ؼ� �߰����� ���̸� ���Ѵ�.
                 //���̴� �� ���� �߰� ���̾�� �Ѵ�.
                 WorkHeightInfo.dLineCntHeight = WorkHeightInfo.dLineA * dTemp + WorkHeightInfo.dLineB ;
                 WorkHeightInfo.dLineTilt      = CMath::GetLineAngleFromA(WorkHeightInfo.dLineA) ; //���θ��� �����Ʈ ������ ƿƮ��.

                 if(WorkHeightInfo.bVertical){ //�������������Ŀ��� SubPnt��ġ������ ���̰��� ���Ѵ�.
                     WorkHeightInfo.dSubToLinePntHeight = WorkHeightInfo.dLineA * pWorkHeightPos->GetSubPosY() + WorkHeightInfo.dLineB ;
                     WorkHeightInfo.dSubA =  CMath::GetLineA(pWorkHeightPos->GetSubPosX(),
                                                             WorkHeightInfo.dSubPntHeight,
                                                             pWorkHeightPos->GetSttPosX(),
                                                             WorkHeightInfo.dSubToLinePntHeight);

                     WorkHeightInfo.dSubB =  CMath::GetLineB(pWorkHeightPos->GetSubPosX(),
                                                             WorkHeightInfo.dSubPntHeight,
                                                             pWorkHeightPos->GetSttPosX(),
                                                             WorkHeightInfo.dSubToLinePntHeight);
                 }
                 else {
                     WorkHeightInfo.dSubToLinePntHeight = WorkHeightInfo.dLineA * pWorkHeightPos->GetSubPosX() + WorkHeightInfo.dLineB ;
                     WorkHeightInfo.dSubA =  CMath::GetLineA(pWorkHeightPos->GetSubPosY(),
                                                             WorkHeightInfo.dSubPntHeight,
                                                             pWorkHeightPos->GetSttPosY(),
                                                             WorkHeightInfo.dSubToLinePntHeight);

                     WorkHeightInfo.dSubB =  CMath::GetLineB(pWorkHeightPos->GetSubPosY(),
                                                             WorkHeightInfo.dSubPntHeight,
                                                             pWorkHeightPos->GetSttPosY(),
                                                             WorkHeightInfo.dSubToLinePntHeight);
                 }
                 WorkHeightInfo.dSubTilt = CMath::GetLineAngleFromA(WorkHeightInfo.dSubA) ; //���θ��� �����Ʈ ������ ƿƮ��.


                 //���߿� �����Ʈ ���� Ȯ�� �ϰ� ����..
                 if(WorkHeightInfo.bVertical) {
                     HexaPot.MoveIncRes(paU , -WorkHeightInfo.dLineTilt) ; //Ok
                     HexaPot.MoveIncRes(paV , -WorkHeightInfo.dSubTilt ) ; //Ok
                     SendLog("HexaU:"+String(-WorkHeightInfo.dLineTilt));
                     SendLog("HexaV:"+String(-WorkHeightInfo.dSubTilt ));
                 }
                 else {
                     HexaPot.MoveIncRes(paV , -WorkHeightInfo.dLineTilt) ; //Ok
                     HexaPot.MoveIncRes(paU , -WorkHeightInfo.dSubTilt ) ; //Ok
                     SendLog("HexaV:"+String(-WorkHeightInfo.dLineTilt));
                     SendLog("HexaU:"+String(-WorkHeightInfo.dSubTilt ));

                 }
                 HexaPot.Move();
                 DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRAlign);
                 Step.iCycle++;
                 return false ;

        case 44: if(!HexaPot.GetStopInpos()) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 45: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 if(!IO_GetX(xSTG_StgVcum1) &&!IO_GetX(xSTG_StgVcum2)&&!IO_GetX(xSTG_StgVcum3)){
                    EM_SetErrMsg(eiPRT_VacErr , "Stage�� ������ �������� �ʾҽ��ϴ�.");
                 }

                 Step.iCycle=0;
                 return true ;



        //2��°�� ���� ��������-------------------------------------------------
        //������ ��.
        case 50:


                 Step.iCycle++;
                 return false ;


        //���ظ� ����.=========================================================
        case 51:

                 if(OM.CmnOptn.bUseConfocal)SetCadOfsGain(tlRConfocal );
                 else                       SetCadOfsGain(tlRHght     );

                 iHeightCnt = 0 ;

                 //�޸� Ȯ��.
                 StandHeightInfo.SetLineHeightCnt(OM.DevOptn.iLineHeightCnt);

                 //iLineHeightCnt �� ���� ��ġ ���.
                 if(pStandHeightPos->GetSttPosX() == pStandHeightPos->GetEndPosX()) { //��Ƽ��
                     StandHeightInfo.bVertical= true ;
                     dPitch = (pStandHeightPos->GetEndPosY() - pStandHeightPos->GetSttPosY())/(OM.DevOptn.iLineHeightCnt-1) ;
                     dPitch = dPitch ;//fabs(dPitch);
                 }
                 else {
                     StandHeightInfo.bVertical = false ;
                     dPitch = (pStandHeightPos->GetEndPosX() - pStandHeightPos->GetSttPosX())/(OM.DevOptn.iLineHeightCnt-1) ;
                     dPitch = dPitch ; //fabs(dPitch);
                 }

                 sTemp = pStandHeightPos->GetName();
                 dTempX = pStandHeightPos->GetSubPosX() - g_tMidPosOfs.dX;
                 dTempY = pStandHeightPos->GetSubPosY() - g_tMidPosOfs.dY;
                 //ù���� �ƴϸ� �����V�� �����.. CMOS���� ���� �� �־.
                 if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;

                 dTempX-= VisnRslt.dMasterX ; 
                 dTempY-= VisnRslt.dMasterY ;

                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Step.iCycle++;
                 return false ;

        case 52: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;

                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }



                 StandHeightInfo.dSubPntHeight = dCrntHeight ;
                 Trace( "Stand Sub Height" ,String(StandHeightInfo.dSubPntHeight).c_str());
                 SendLog("Stand Sub Height:" + String(StandHeightInfo.dSubPntHeight) );
                 Step.iCycle++;
                 return false ;

        //�ؿ�����.
        case 53:
                 sTemp = pStandHeightPos->GetName();
                 dTempX = pStandHeightPos->GetSttPosX() - g_tMidPosOfs.dX;
                 dTempY = pStandHeightPos->GetSttPosY() - g_tMidPosOfs.dY;
                 //ù���� �ƴϸ� �����V�� �����.. CMOS���� ���� �� �־.
                 //if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 //���ظ��� �����V���� �ϸ� �ȉ�.
                 if(StandHeightInfo.bVertical)dTempY += dPitch * iHeightCnt ;
                 else                         dTempX += dPitch * iHeightCnt ;

                 dTempX-= VisnRslt.dMasterX ;
                 dTempY-= VisnRslt.dMasterY ;

                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Step.iCycle++;
                 return false ;

        case 54: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }
                 if(StandHeightInfo.bVertical) StandHeightInfo.adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_YVisn );
                 else                          StandHeightInfo.adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_XGenRr);
                 StandHeightInfo.adLineHeight[iHeightCnt] = dCrntHeight ;

                 iHeightCnt++;
                 if(iHeightCnt < StandHeightInfo.iLineHeightCnt ){
                     Step.iCycle=53 ;
                     return false ;
                 }

                 //�̰� �Լ� ������ �ȵǳ�...
                 //GetLineABFromPntByLeastSqure(int _iPntCnt , double * _pPntX , double * _pPntY , double & _dA , double & _dB);
                 //��ŸƮ ����Ʈ���� 1��и� �׷����� ������ �ǹ���.
                 if(!CMath::GetLineABFromPntByLeastSqure(StandHeightInfo.iLineHeightCnt  ,
                                                         StandHeightInfo.adLineHeightPos ,
                                                         StandHeightInfo.adLineHeight    ,
                                                         StandHeightInfo.dLineA          ,
                                                         StandHeightInfo.dLineB          )){
                     for(int i = 0 ; i < StandHeightInfo.iLineHeightCnt ; i++){
                         Trace( (String("Stand Pos"   )+i).c_str(),String(StandHeightInfo.adLineHeightPos[i]).c_str());
                         Trace( (String("Stand Height")+i).c_str(),String(StandHeightInfo.adLineHeight   [i]).c_str());

                     }
                     EM_SetErrMsg(eiETC_CalErr , "�ּ� �ڽ¹� ���꿡��");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 for(int i = 0 ; i < StandHeightInfo.iLineHeightCnt ; i++){
                     Trace( (String("Stand Pos"   )+i).c_str(),String(StandHeightInfo.adLineHeightPos[i]).c_str());
                     Trace( (String("Stand Height")+i).c_str(),String(StandHeightInfo.adLineHeight   [i]).c_str());
                     SendLog(String("Stand Height")+i +":" + String(StandHeightInfo.adLineHeight   [i]));
                 }
                 //�ȵŰڴ� �ϴ�... 2���� �̿��Ͽ� ���Ѵ�.
                 //StandHeightInfo.dLineA = CMath::GetLineA(StandHeightInfo.adLineHeightPos[0] ,
                 //                                         StandHeightInfo.adLineHeight   [0] ,
                 //                                         StandHeightInfo.adLineHeightPos[StandHeightInfo.iLineHeightCnt-1],
                 //                                         StandHeightInfo.adLineHeight   [StandHeightInfo.iLineHeightCnt-1]);
                 //
                 //StandHeightInfo.dLineB = CMath::GetLineB(StandHeightInfo.adLineHeightPos[0] ,
                 //                                         StandHeightInfo.adLineHeight   [0] ,
                 //                                         StandHeightInfo.adLineHeightPos[StandHeightInfo.iLineHeightCnt-1],
                 //                                         StandHeightInfo.adLineHeight   [StandHeightInfo.iLineHeightCnt-1]);




                 //ó���������� ������ �߰��� ���ؼ�
                 //�̰���.. ������ �������� Ȯ�� �ϴ� ���.

                 dTemp = (StandHeightInfo.adLineHeightPos[StandHeightInfo.iLineHeightCnt-1] + StandHeightInfo.adLineHeightPos[0])/2.0 ;
                 //�ּ��ڽ¹����� ���� ���������Ŀ� �����ؼ� �߰����� ���̸� ���Ѵ�.
                 //���̴� �� ���� �߰� ���̾�� �Ѵ�.
                 StandHeightInfo.dLineCntHeight = StandHeightInfo.dLineA * dTemp + StandHeightInfo.dLineB ;
                 StandHeightInfo.dLineTilt      = CMath::GetLineAngleFromA(StandHeightInfo.dLineA) ; //���θ��� �����Ʈ ������ ƿƮ��.


                 //�ȵŰڴ�. �ϴ�.  2������ �̿�.
                 //StandHeightInfo.adLineHeightPos[0] ,
                 // ,
                 //StandHeightInfo.adLineHeightPos[StandHeightInfo.iLineHeightCnt-1],
                 //StandHeightInfo.dLineCntHeight = (StandHeightInfo.adLineHeight   [0] + StandHeightInfo.adLineHeight   [StandHeightInfo.iLineHeightCnt-1])/2.0 ;
                 //StandHeightInfo.dLineTilt      = CMath::GetLineAngleFromA(StandHeightInfo.dLineA) ; //���θ��� �����Ʈ ������ ƿƮ��.



                 if(StandHeightInfo.bVertical){ //�������������Ŀ��� SubPnt��ġ������ ���̰��� ���Ѵ�.
                     StandHeightInfo.dSubToLinePntHeight = StandHeightInfo.dLineA * pStandHeightPos->GetSubPosY() + StandHeightInfo.dLineB ;
                     StandHeightInfo.dSubA =  CMath::GetLineA(pStandHeightPos->GetSubPosX(),
                                                              StandHeightInfo.dSubPntHeight,
                                                              pStandHeightPos->GetSttPosX(),
                                                              StandHeightInfo.dSubToLinePntHeight);

                     StandHeightInfo.dSubB =  CMath::GetLineB(pStandHeightPos->GetSubPosX(),
                                                              StandHeightInfo.dSubPntHeight,
                                                              pStandHeightPos->GetSttPosX(),
                                                              StandHeightInfo.dSubToLinePntHeight);
                 }
                 else {
                     StandHeightInfo.dSubToLinePntHeight = StandHeightInfo.dLineA * pStandHeightPos->GetSubPosX() + StandHeightInfo.dLineB ;
                     StandHeightInfo.dSubA =  CMath::GetLineA(pStandHeightPos->GetSubPosY(),
                                                              StandHeightInfo.dSubPntHeight,
                                                              pStandHeightPos->GetSttPosY(),
                                                              StandHeightInfo.dSubToLinePntHeight);

                     StandHeightInfo.dSubB =  CMath::GetLineB(pStandHeightPos->GetSubPosY(),
                                                              StandHeightInfo.dSubPntHeight,
                                                              pStandHeightPos->GetSttPosY(),
                                                              StandHeightInfo.dSubToLinePntHeight);
                 }
                 StandHeightInfo.dSubTilt = CMath::GetLineAngleFromA(StandHeightInfo.dSubA) ; //���θ��� �����Ʈ ������ ƿƮ��.

                 iWorkHeightCnt = 0 ;

                 Step.iCycle=60;
                 return false ;



        //���̴³� ���� ����=========================================================
        //�ϰ� ƿƮ ���� �����Ŀ� �ѹ� �� ���� �Ͽ� Z���� ���� �Ѵ�.

        //�ؿ��� ��.
        case 60: if(!HexaPot.GetStopInpos()) return false ;

                 iHeightCnt = 0 ;

                 //�޸� Ȯ��.
                 WorkHeightInfo.SetLineHeightCnt(OM.DevOptn.iLineHeightCnt);

                 //iLineHeightCnt �� ���� ��ġ ���.
                 if(pWorkHeightPos->GetSttPosX() == pWorkHeightPos->GetEndPosX()) { //��Ƽ��
                     WorkHeightInfo.bVertical= true ;
                     dPitch = (pWorkHeightPos->GetEndPosY() - pWorkHeightPos->GetSttPosY())/(OM.DevOptn.iLineHeightCnt-1) ;
                 }
                 else {
                     WorkHeightInfo.bVertical = false ;
                     dPitch = (pWorkHeightPos->GetEndPosX() - pWorkHeightPos->GetSttPosX())/(OM.DevOptn.iLineHeightCnt-1) ;
                 }

                 dTempX = pWorkHeightPos->GetSubPosX() - g_tMidPosOfs.dX;
                 dTempY = pWorkHeightPos->GetSubPosY() - g_tMidPosOfs.dY;
                 //ù���� �ƴϸ� �����V�� �����.. CMOS���� ���� �� �־.
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;

                 dTempX-= VisnRslt.dMasterX ;
                 dTempY-= VisnRslt.dMasterY ;


                 if(StandHeightInfo.bVertical)dTempY += dPitch * iHeightCnt ;
                 else                         dTempX += dPitch * iHeightCnt ;



                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Step.iCycle++;
                 return false ;

        case 61: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }

                 WorkHeightInfo.dSubPntHeight = dCrntHeight ;
                 Trace( "Work Sub Height" ,String(WorkHeightInfo.dSubPntHeight).c_str());
                 SendLog("Work Sub Height:" + String(WorkHeightInfo.dSubPntHeight) );
                 Step.iCycle++;
                 return false ;

        //�ؿ�����.
        case 62: dTempX = pWorkHeightPos->GetSttPosX() -g_tMidPosOfs.dX;
                 dTempY = pWorkHeightPos->GetSttPosY() -g_tMidPosOfs.dY;
                 //ù���� �ƴϸ� �����V�� �����.. CMOS���� ���� �� �־.
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;

                 dTempX-= VisnRslt.dMasterX ;
                 dTempY-= VisnRslt.dMasterY ;

                 //���ظ��� �����V���� �ϸ� �ȉ�.
                 if(WorkHeightInfo.bVertical) dTempY += dPitch * iHeightCnt ;
                 else                         dTempX += dPitch * iHeightCnt ;

                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Step.iCycle++;
                 return false ;

        case 63: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }

                 if(WorkHeightInfo.bVertical) WorkHeightInfo.adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_YVisn );
                 else                         WorkHeightInfo.adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_XGenRr);
                 WorkHeightInfo.adLineHeight[iHeightCnt] = dCrntHeight ;

                 iHeightCnt++;
                 if(iHeightCnt < WorkHeightInfo.iLineHeightCnt ){
                     Step.iCycle=62 ;
                     return false ;
                 }

                 //�̰� �Լ� ������ �ȵǳ�...
                 //GetLineABFromPntByLeastSqure(int _iPntCnt , double * _pPntX , double * _pPntY , double & _dA , double & _dB);
                 //��ŸƮ ����Ʈ���� �׷����� ������ �ǹ���.
                 WorkHeightInfo.dLineA =  CMath::GetLineA(WorkHeightInfo.adLineHeightPos[0],
                                                          WorkHeightInfo.adLineHeight[0],
                                                          WorkHeightInfo.adLineHeightPos[WorkHeightInfo.iLineHeightCnt-1],
                                                          WorkHeightInfo.adLineHeight[WorkHeightInfo.iLineHeightCnt-1]);
                 Trace("Simple A" ,String(WorkHeightInfo.dLineA).c_str());

                 if(!CMath::GetLineABFromPntByLeastSqure(WorkHeightInfo.iLineHeightCnt  ,
                                                         WorkHeightInfo.adLineHeightPos ,
                                                         WorkHeightInfo.adLineHeight    ,
                                                         WorkHeightInfo.dLineA          ,
                                                         WorkHeightInfo.dLineB          )){
                     for(int i = 0 ; i < WorkHeightInfo.iLineHeightCnt ; i++){
                         Trace( (String("Work Pos")+i).c_str(),String(WorkHeightInfo.adLineHeightPos[i]).c_str());
                         Trace( (String("Work Height")+i).c_str(),String(WorkHeightInfo.adLineHeight   [i]).c_str());

                     }
                     EM_SetErrMsg(eiETC_CalErr , "�ּ� �ڽ¹� ���꿡��");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 Trace("LeastSqure A" ,String(WorkHeightInfo.dLineA).c_str());
                 for(int i = 0 ; i < WorkHeightInfo.iLineHeightCnt ; i++){
                     Trace( (String("Work Pos"   )+i).c_str(),String(WorkHeightInfo.adLineHeightPos[i]).c_str());
                     Trace( (String("Work Height")+i).c_str(),String(WorkHeightInfo.adLineHeight   [i]).c_str());
                     SendLog(String("Work Height")+i +":" + String(WorkHeightInfo.adLineHeight   [i]));

                 }


                 //ó���������� ������ �߰��� ���ؼ�
                 //�̰���.. ������ �������� Ȯ�� �ϴ� ���.
                 dTemp = (WorkHeightInfo.adLineHeightPos[WorkHeightInfo.iLineHeightCnt-1] + WorkHeightInfo.adLineHeightPos[0])/2.0 ;
                 //�ּ��ڽ¹����� ���� ���������Ŀ� �����ؼ� �߰����� ���̸� ���Ѵ�.
                 //���̴� �� ���� �߰� ���̾�� �Ѵ�.
                 WorkHeightInfo.dLineCntHeight = WorkHeightInfo.dLineA * dTemp + WorkHeightInfo.dLineB ;
                 WorkHeightInfo.dLineTilt      = CMath::GetLineAngleFromA(WorkHeightInfo.dLineA) ; //���θ��� �����Ʈ ������ ƿƮ��.

                 if(WorkHeightInfo.bVertical){ //�������������Ŀ��� SubPnt��ġ������ ���̰��� ���Ѵ�.
                     WorkHeightInfo.dSubToLinePntHeight = WorkHeightInfo.dLineA * pWorkHeightPos->GetSubPosY() + WorkHeightInfo.dLineB ;
                     WorkHeightInfo.dSubA =  CMath::GetLineA(pWorkHeightPos->GetSubPosX(),
                                                             WorkHeightInfo.dSubPntHeight,
                                                             pWorkHeightPos->GetSttPosX(),
                                                             WorkHeightInfo.dSubToLinePntHeight);

                     WorkHeightInfo.dSubB =  CMath::GetLineB(pWorkHeightPos->GetSubPosX(),
                                                             WorkHeightInfo.dSubPntHeight,
                                                             pWorkHeightPos->GetSttPosX(),
                                                             WorkHeightInfo.dSubToLinePntHeight);
                 }
                 else {
                     WorkHeightInfo.dSubToLinePntHeight = WorkHeightInfo.dLineA * pWorkHeightPos->GetSubPosX() + WorkHeightInfo.dLineB ;
                     WorkHeightInfo.dSubA =  CMath::GetLineA(pWorkHeightPos->GetSubPosY(),
                                                             WorkHeightInfo.dSubPntHeight,
                                                             pWorkHeightPos->GetSttPosY(),
                                                             WorkHeightInfo.dSubToLinePntHeight);

                     WorkHeightInfo.dSubB =  CMath::GetLineB(pWorkHeightPos->GetSubPosY(),
                                                             WorkHeightInfo.dSubPntHeight,
                                                             pWorkHeightPos->GetSttPosY(),
                                                             WorkHeightInfo.dSubToLinePntHeight);
                 }
                 WorkHeightInfo.dSubTilt = CMath::GetLineAngleFromA(WorkHeightInfo.dSubA) ; //���θ��� �����Ʈ ������ ƿƮ��.

                 if(iWorkHeightCnt==1 ||iWorkHeightCnt==3){ //����° �۾��ÿ��� Z���� �����.
                     iWorkHeightCnt++ ;
                     HexaPot.MoveIncRes(paZ , (StandHeightInfo.dLineCntHeight - WorkHeightInfo.dLineCntHeight)) ;
                     SendLog("HexaZ:"+String(StandHeightInfo.dLineCntHeight - WorkHeightInfo.dLineCntHeight));
                     HexaPot.Move();
                     Step.iCycle=60 ;
                     return false ;
                 }
                 else if(iWorkHeightCnt==4){ //�׹�°�� Ȯ�θ� �Ѵ�
                     dTemp = 0 ;
                     for(int i = 0 ; i < StandHeightInfo.iLineHeightCnt ; i++){
                         Trace( (String("Height Gap")+i).c_str(),String(StandHeightInfo.adLineHeight[i]-WorkHeightInfo.adLineHeight[i]).c_str());
                         SendLog(String("Height Gap")+i +":"+ String(StandHeightInfo.adLineHeight[i]-WorkHeightInfo.adLineHeight[i]));

                         dTemp += StandHeightInfo.adLineHeight[i]-WorkHeightInfo.adLineHeight[i] ;

                         if(fabs(StandHeightInfo.adLineHeight[i]-WorkHeightInfo.adLineHeight[i]) > OM.DevOptn.dRHgtTolZ){
                             EM_SetErrMsg(eiHGT_RangeOver , ("������ �����̺갣 ������"+String(StandHeightInfo.adLineHeight[i]-WorkHeightInfo.adLineHeight[i])+"�Դϴ�.").c_str());
                             Step.iCycle = 0 ;
                             return true ;
                         }
                     }

                     dTemp = dTemp / StandHeightInfo.iLineHeightCnt ;
                     Trace  (String("Height Gap Average").c_str(),String(dTemp).c_str());
                     SendLog(String("Height Gap:")+ String(dTemp));

                     if(fabs(dTemp) > OM.DevOptn.dRHgtTolZ){
                         EM_SetErrMsg(eiHGT_RangeOver , ("������ �����̺갣 ������"+String(dTemp)+"�Դϴ�.").c_str());
                         Step.iCycle = 0 ;
                         return true ;
                     }

                 }
                 else{ //1,2��° �۾��ÿ��� ƿƮ�� ���߰�.
                     iWorkHeightCnt++ ;

                     //���߿� �����Ʈ ���� Ȯ�� �ϰ� ����..
                     if(StandHeightInfo.bVertical) {

                         HexaPot.MoveIncRes(paU , -(WorkHeightInfo.dLineTilt  - StandHeightInfo.dLineTilt)) ; //Ok
                         SendLog("HexaU:"+String(-(WorkHeightInfo.dLineTilt  - StandHeightInfo.dLineTilt)));

                         if(iWorkHeightCnt==0){
                             HexaPot.MoveIncRes(paV , -(WorkHeightInfo.dSubTilt   - StandHeightInfo.dSubTilt )) ; //Ok
                             SendLog("HexaV:"+String(-(WorkHeightInfo.dSubTilt   - StandHeightInfo.dSubTilt )));
                         }
                     }
                     else {
                         if(iWorkHeightCnt==0){
                             SendLog("HexaU:"+String(-(WorkHeightInfo.dSubTilt   - StandHeightInfo.dSubTilt )));
                             HexaPot.MoveIncRes(paU , -(WorkHeightInfo.dSubTilt   - StandHeightInfo.dSubTilt )) ; //Ok
                         }
                         HexaPot.MoveIncRes(paV , -(WorkHeightInfo.dLineTilt  - StandHeightInfo.dLineTilt)) ; //Ok
                         SendLog("HexaV:"+String(-(WorkHeightInfo.dLineTilt  - StandHeightInfo.dLineTilt)));

                     }
                     HexaPot.Move();
                     Step.iCycle=60 ;
                     return false ;
                 }

                 DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRAlign);
                 Step.iCycle++;
                 return false ;

        case 64: if(!HexaPot.GetStopInpos()) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 65: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 if(!IO_GetX(xSTG_StgVcum1) &&!IO_GetX(xSTG_StgVcum2)&&!IO_GetX(xSTG_StgVcum3)){
                    EM_SetErrMsg(eiPRT_VacErr , "Stage�� ������ �������� �ʾҽ��ϴ�.");

                 }
                 Step.iCycle=0;
                 return true ;





//=====4��° �� ���� �߿� Standard==============================================
        //������ ��.
        case 80: iWorkHeightCnt = 0 ;
                 Step.iCycle++;
                 return false ;


        //���ظ� ����.=========================================================
        //�ؿ�����.
        case 81: if(OM.CmnOptn.bUseConfocal)SetCadOfsGain(tlRConfocal );
                 else                       SetCadOfsGain(tlRHght     );

                 //�� 4�� ���⸦ ���� ���µ� ���߿� 0�� 2���� �������ո� 3,4�� ���� ���ո�.
                 //                                 1�� 3���� �������ո� 2,4�� ���� ���ո�.
                 if(iWorkHeightCnt%2 == 0) {
                     pStandHeightPos    = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow].CM_RHgtR ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pWorkHeightPos     = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow].CM_RHgtL ;

                     pStandHeightInfo   = &StandHeightInfo ;
                     pWorkHeightInfo    = &WorkHeightInfo  ;

                 }
                 else {
                     pStandHeightPos    = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_RHgtB ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pWorkHeightPos     = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RHgtT ;

                     pStandHeightInfo   = &SubStandHeightInfo ;
                     pWorkHeightInfo    = &SubWorkHeightInfo  ;
                 }

                 iHeightCnt = 0 ;

                 //�޸� Ȯ��.
                 pStandHeightInfo->SetLineHeightCnt(OM.DevOptn.iLineHeightCnt);

                 //iLineHeightCnt �� ���� ��ġ ���.
                 if(pStandHeightPos->GetSttPosX() == pStandHeightPos->GetEndPosX()) { //��Ƽ��
                     pStandHeightInfo->bVertical= true ;
                     dPitch = (pStandHeightPos->GetEndPosY() - pStandHeightPos->GetSttPosY())/(OM.DevOptn.iLineHeightCnt-1) ;
                     dPitch = dPitch ;//fabs(dPitch);
                 }
                 else {
                     pStandHeightInfo->bVertical = false ;
                     dPitch = (pStandHeightPos->GetEndPosX() - pStandHeightPos->GetSttPosX())/(OM.DevOptn.iLineHeightCnt-1) ;
                     dPitch = dPitch ; //fabs(dPitch);
                 }

                 Step.iCycle++;
                 return false ;

        case 82: //����� üũ �� �ʿ� ����...
                 Step.iCycle++;
                 return false ;

        //�ؿ�����.
        case 83:
                 sTemp = pStandHeightPos->GetName();
                 dTempX = pStandHeightPos->GetSttPosX() - g_tMidPosOfs.dX;
                 dTempY = pStandHeightPos->GetSttPosY() - g_tMidPosOfs.dY;
                 //ù���� �ƴϸ� �����V�� �����.. CMOS���� ���� �� �־.
                 //if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 //���ظ��� �����V���� �ϸ� �ȉ�.
                 if(pStandHeightInfo->bVertical)dTempY += dPitch * iHeightCnt ;
                 else                           dTempX += dPitch * iHeightCnt ;

                 dTempX-= VisnRslt.dMasterX ;
                 dTempY-= VisnRslt.dMasterY ;

                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Step.iCycle++;
                 return false ;

        case 84: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }
                 if(pStandHeightInfo->bVertical) pStandHeightInfo->adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_YVisn );
                 else                            pStandHeightInfo->adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_XGenRr);
                 pStandHeightInfo->adLineHeight[iHeightCnt] = dCrntHeight ;

                 iHeightCnt++;
                 if(iHeightCnt < pStandHeightInfo->iLineHeightCnt ){
                     Step.iCycle=83 ;
                     return false ;
                 }

                 //�̰� �Լ� ������ �ȵǳ�...
                 //GetLineABFromPntByLeastSqure(int _iPntCnt , double * _pPntX , double * _pPntY , double & _dA , double & _dB);
                 //��ŸƮ ����Ʈ���� 1��и� �׷����� ������ �ǹ���.
                 if(!CMath::GetLineABFromPntByLeastSqure(pStandHeightInfo->iLineHeightCnt  ,
                                                         pStandHeightInfo->adLineHeightPos ,
                                                         pStandHeightInfo->adLineHeight    ,
                                                         pStandHeightInfo->dLineA          ,
                                                         pStandHeightInfo->dLineB          )){
                     for(int i = 0 ; i < pStandHeightInfo->iLineHeightCnt ; i++){
                         Trace( (String("Stand Pos"   )+i).c_str(),String(pStandHeightInfo->adLineHeightPos[i]).c_str());
                         Trace( (String("Stand Height")+i).c_str(),String(pStandHeightInfo->adLineHeight   [i]).c_str());

                     }
                     EM_SetErrMsg(eiETC_CalErr , "�ּ� �ڽ¹� ���꿡��");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 for(int i = 0 ; i < pStandHeightInfo->iLineHeightCnt ; i++){
                     Trace( (String("Stand Pos"   )+i).c_str(),String(pStandHeightInfo->adLineHeightPos[i]).c_str());
                     Trace( (String("Stand Height")+i).c_str(),String(pStandHeightInfo->adLineHeight   [i]).c_str());
                     SendLog(String("Stand Height")+i +":" + String(pStandHeightInfo->adLineHeight   [i]));
                 }

                 //ó���������� ������ �߰��� ���ؼ�
                 //�̰���.. ������ �������� Ȯ�� �ϴ� ���.
                 dTemp = (pStandHeightInfo->adLineHeightPos[pStandHeightInfo->iLineHeightCnt-1] + pStandHeightInfo->adLineHeightPos[0])/2.0 ;
                 //�ּ��ڽ¹����� ���� ���������Ŀ� �����ؼ� �߰����� ���̸� ���Ѵ�.
                 //���̴� �� ���� �߰� ���̾�� �Ѵ�.

                 //4�忡�� �ʿ� ����.
                 //pStandHeightInfo->dLineCntHeight = pStandHeightInfo->dLineA * dTemp + pStandHeightInfo->dLineB ;
                 pStandHeightInfo->dLineTilt      = CMath::GetLineAngleFromA(pStandHeightInfo->dLineA) ; //���θ��� �����Ʈ ������ ƿƮ��.




                 Step.iCycle=90;
                 return false ;




//���̴³� ���� ����=========================================================
        //�ϰ� ƿƮ ���� �����Ŀ� �ѹ� �� ���� �Ͽ� Z���� ���� �Ѵ�.

        //�ؿ��� ��.
        case 90: if(!HexaPot.GetStopInpos()) return false ;

                 iHeightCnt = 0 ;

                 //�޸� Ȯ��.
                 pWorkHeightInfo->SetLineHeightCnt(OM.DevOptn.iLineHeightCnt);

                 //iLineHeightCnt �� ���� ��ġ ���.
                 if(pWorkHeightPos->GetSttPosX() == pWorkHeightPos->GetEndPosX()) { //��Ƽ��
                     pWorkHeightInfo->bVertical= true ;
                     dPitch = (pWorkHeightPos->GetEndPosY() - pWorkHeightPos->GetSttPosY())/(OM.DevOptn.iLineHeightCnt-1) ;
                 }
                 else {
                     pWorkHeightInfo->bVertical = false ;
                     dPitch = (pWorkHeightPos->GetEndPosX() - pWorkHeightPos->GetSttPosX())/(OM.DevOptn.iLineHeightCnt-1) ;
                 }

                 Step.iCycle++;
                 return false ;

        case 91: Step.iCycle++;
                 return false ;

        //�ؿ�����.
        case 92: dTempX = pWorkHeightPos->GetSttPosX() -g_tMidPosOfs.dX;
                 dTempY = pWorkHeightPos->GetSttPosY() -g_tMidPosOfs.dY;
                 //ù���� �ƴϸ� �����V�� �����.. CMOS���� ���� �� �־.
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;

                 dTempX-= VisnRslt.dMasterX ;
                 dTempY-= VisnRslt.dMasterY ;

                 //���ظ��� �����V���� �ϸ� �ȉ�.
                 if(pWorkHeightInfo->bVertical) dTempY += dPitch * iHeightCnt ;
                 else                         dTempX += dPitch * iHeightCnt ;

                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Step.iCycle++;
                 return false ;

        case 93: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }

                 if(pWorkHeightInfo->bVertical) pWorkHeightInfo->adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_YVisn );
                 else                         pWorkHeightInfo->adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_XGenRr);
                 pWorkHeightInfo->adLineHeight[iHeightCnt] = dCrntHeight ;

                 iHeightCnt++;
                 if(iHeightCnt < pWorkHeightInfo->iLineHeightCnt ){
                     Step.iCycle=92 ;
                     return false ;
                 }

                 //�̰� �Լ� ������ �ȵǳ�...
                 //GetLineABFromPntByLeastSqure(int _iPntCnt , double * _pPntX , double * _pPntY , double & _dA , double & _dB);
                 //��ŸƮ ����Ʈ���� �׷����� ������ �ǹ���.
                 if(!CMath::GetLineABFromPntByLeastSqure(pWorkHeightInfo->iLineHeightCnt  ,
                                                         pWorkHeightInfo->adLineHeightPos ,
                                                         pWorkHeightInfo->adLineHeight    ,
                                                         pWorkHeightInfo->dLineA          ,
                                                         pWorkHeightInfo->dLineB          )){
                     for(int i = 0 ; i < pWorkHeightInfo->iLineHeightCnt ; i++){
                         Trace( (String("Work Pos")+i).c_str(),String(pWorkHeightInfo->adLineHeightPos[i]).c_str());
                         Trace( (String("Work Height")+i).c_str(),String(pWorkHeightInfo->adLineHeight   [i]).c_str());

                     }
                     EM_SetErrMsg(eiETC_CalErr , "�ּ� �ڽ¹� ���꿡��");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 Trace("LeastSqure A" ,String(pWorkHeightInfo->dLineA).c_str());
                 for(int i = 0 ; i < pWorkHeightInfo->iLineHeightCnt ; i++){
                     Trace( (String("Work Pos"   )+i).c_str(),String(pWorkHeightInfo->adLineHeightPos[i]).c_str());
                     Trace( (String("Work Height")+i).c_str(),String(pWorkHeightInfo->adLineHeight   [i]).c_str());
                     SendLog(String("Work Height")+i +":" + String(pWorkHeightInfo->adLineHeight   [i]));

                 }


                 //ó���������� ������ �߰��� ���ؼ�
                 //�̰���.. ������ �������� Ȯ�� �ϴ� ���.
                 //dTemp = (pWorkHeightInfo->adLineHeightPos[pWorkHeightInfo->iLineHeightCnt-1] + pWorkHeightInfo->adLineHeightPos[0])/2.0 ;
                 //�ּ��ڽ¹����� ���� ���������Ŀ� �����ؼ� �߰����� ���̸� ���Ѵ�.
                 //���̴� �� ���� �߰� ���̾�� �Ѵ�.
                 //pWorkHeightInfo->dLineCntHeight = pWorkHeightInfo->dLineA * dTemp + pWorkHeightInfo->dLineB ;
                 pWorkHeightInfo->dLineTilt      = CMath::GetLineAngleFromA(pWorkHeightInfo->dLineA) ; //���θ��� �����Ʈ ������ ƿƮ��.


                 //0: 3,4�� �� ���ո� ƿƮ �˻�.
                 //1: 2,4�� �� ���ո� ƿƮ �˻�. ƿƮ����.
                 //2: 3,4�� �� ���ո� ���� ���� �˻�.
                 //3: 2,4�� �� ���ո� ���� ���� �˻�. ���� ����.
                 if(iWorkHeightCnt == 1) {
                     //���߿� �����Ʈ ���� Ȯ�� �ϰ� ����..
                     if(StandHeightInfo.bVertical) {

                         HexaPot.MoveIncRes(paU , -(WorkHeightInfo.dLineTilt  - StandHeightInfo.dLineTilt)) ; //Ok
                         SendLog("HexaU:"+String(-(WorkHeightInfo.dLineTilt  - StandHeightInfo.dLineTilt)));

                         HexaPot.MoveIncRes(paV , -(SubWorkHeightInfo.dLineTilt   - SubStandHeightInfo.dLineTilt )) ; //Ok
                         SendLog("HexaV:"+String(-(SubWorkHeightInfo.dLineTilt   - SubStandHeightInfo.dLineTilt )));
                     }
                     else { //���� ������ 2,4�����ϰ��.
                         //����� ������ �ȉ�....
                         //������ �ʱ� ������ ������ ���غ���.
                         //���� ���� Ȯ�� �ؾ���.
                         HexaPot.MoveIncRes(paV , -(WorkHeightInfo.dLineTilt  - StandHeightInfo.dLineTilt)) ; //Ok
                         SendLog("HexaV:"+String(-(WorkHeightInfo.dLineTilt  - StandHeightInfo.dLineTilt)));

                         HexaPot.MoveIncRes(paU , -(SubWorkHeightInfo.dLineTilt   - SubStandHeightInfo.dLineTilt )) ; //Ok
                         SendLog("HexaU:"+String(-(SubWorkHeightInfo.dLineTilt   - SubStandHeightInfo.dLineTilt )));

                     }
                     HexaPot.Move();
                 }
                 if(iWorkHeightCnt == 3) {
                     //3�� ���ո� ���� , 2�� ���ո� ������ ����� ���Ѵ�.
                     dHeightSum = 0 ;
                     for(int i = 0 ; i < StandHeightInfo.iLineHeightCnt ; i++){
                         Trace( (String("Main Stand Pos"   )+i).c_str(),String(StandHeightInfo.adLineHeightPos[i]).c_str());
                         Trace( (String("Main Stand Height")+i).c_str(),String(StandHeightInfo.adLineHeight   [i]).c_str());
                         dHeightSum += StandHeightInfo.adLineHeight[i] ;
                     }
                     for(int i = 0 ; i < SubStandHeightInfo.iLineHeightCnt ; i++){
                         Trace( (String("Sub Stand Pos"   )+i).c_str(),String(SubStandHeightInfo.adLineHeightPos[i]).c_str());
                         Trace( (String("Sub Stand Height")+i).c_str(),String(SubStandHeightInfo.adLineHeight   [i]).c_str());
                         dHeightSum += SubStandHeightInfo.adLineHeight[i] ;
                     }
                     dStandHeightAvr = dHeightSum / (double)(StandHeightInfo.iLineHeightCnt + SubStandHeightInfo.iLineHeightCnt) ;



                     //4���� ���� ���ո� ����� ���Ѵ�.
                     dHeightSum = 0 ;
                     for(int i = 0 ; i < WorkHeightInfo.iLineHeightCnt ; i++){
                         Trace( (String("Main Work Pos"   )+i).c_str(),String(WorkHeightInfo.adLineHeightPos[i]).c_str());
                         Trace( (String("Main Work Height")+i).c_str(),String(WorkHeightInfo.adLineHeight   [i]).c_str());
                         dHeightSum += WorkHeightInfo.adLineHeight[i] ;
                     }
                     for(int i = 0 ; i < SubWorkHeightInfo.iLineHeightCnt ; i++){
                         Trace( (String("Main Work Pos"   )+i).c_str(),String(SubWorkHeightInfo.adLineHeightPos[i]).c_str());
                         Trace( (String("Main Work Height")+i).c_str(),String(SubWorkHeightInfo.adLineHeight   [i]).c_str());
                         dHeightSum += SubWorkHeightInfo.adLineHeight[i] ;
                     }
                     dWorkHeightAvr = dHeightSum / (double)(WorkHeightInfo.iLineHeightCnt + SubWorkHeightInfo.iLineHeightCnt) ;


                     Trace( "dStandHeightAvr",String(dStandHeightAvr).c_str());
                     SendLog("StandHeightAvr:"+String(dStandHeightAvr));
                     Trace( "dWorkHeightAvr" ,String(dWorkHeightAvr ).c_str());
                     SendLog("WorkHeightAvr:"+String(dWorkHeightAvr));

                     HexaPot.MoveIncRes(paZ , (dStandHeightAvr - dWorkHeightAvr)+OM.DevOptn.dSlaveHtOfsZ) ;
                     Trace( "HexaWorkOffsetZ" ,String((dStandHeightAvr - dWorkHeightAvr)+OM.DevOptn.dSlaveHtOfsZ ).c_str());
                     SendLog("WorkOffsetZ:"+String((dStandHeightAvr - dWorkHeightAvr)+OM.DevOptn.dSlaveHtOfsZ ));

                     HexaPot.Move();
                 }

                 Step.iCycle++;
                 return false ;

        case 94: if(!HexaPot.GetStopInpos()) return false ;

                 iWorkHeightCnt++;
                 if(iWorkHeightCnt < 4){
                     Step.iCycle=81 ;
                     return false ;
                 }

                 DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRAlign);




                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 95: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 if(!IO_GetX(xSTG_StgVcum1) &&!IO_GetX(xSTG_StgVcum2)&&!IO_GetX(xSTG_StgVcum3)){
                    EM_SetErrMsg(eiPRT_VacErr , "Stage�� ������ �������� �ʾҽ��ϴ�.");

                 }
                 Step.iCycle=0;
                 return true ;

    }
}

bool CRightTool::CycleVisnHtCal(int &_iStep  ,  CCadRect * _pCmsRect , CCadVisnPoint * _pVisnPos , int _iOfsX , int _iOfsY)
{
    static double dCheckX ;
    static double dCheckY ;
    static double dCheckOffset ;
    double dCheckHt ;

    static int iHtStep ;
    static double dCrntHeight ;

    //Cycle.
    switch (_iStep) {
        default : _iStep = 0 ;
                  return true ;

        case 10: //������.

                 SetCadOfsGain(tlNone );

                 //if(OM.CmnOptn.bUseConfocal)SetCadOfsGain(tlRConfocal );
                 //else                       SetCadOfsGain(tlRHght     );

                 //IsPntRectInOfsGain <- �Լ���  tlNone���� �����V ������ ���õǾ�� �Ѵ�.
                 if(_pCmsRect -> IsPntRectInOfsGain(_pVisnPos -> GetCntrPosX()-4,_pVisnPos -> GetCntrPosY()-4)){

                     SetCadOfsGain(tlRHght     );
                     dCheckX =_pVisnPos -> GetCntrPosX()+2;//4��и� ��ǥ�踦 ������ ��� �Ѵ�.
                     dCheckY =_pVisnPos -> GetCntrPosY()+2;

                 }
                 else if(_pCmsRect -> IsPntRectInOfsGain(_pVisnPos -> GetCntrPosX()+4,_pVisnPos -> GetCntrPosY()-4)){
                     SetCadOfsGain(tlRHght     );
                     dCheckX =_pVisnPos -> GetCntrPosX()-2 ;
                     dCheckY =_pVisnPos -> GetCntrPosY()+2 ;

                 }
                 else if(_pCmsRect -> IsPntRectInOfsGain(_pVisnPos -> GetCntrPosX()+4,_pVisnPos -> GetCntrPosY()+4)){
                     SetCadOfsGain(tlRHght     );
                     dCheckX =_pVisnPos -> GetCntrPosX()-2 ;
                     dCheckY =_pVisnPos -> GetCntrPosY()-2 ;

                 }
                 else if(_pCmsRect -> IsPntRectInOfsGain(_pVisnPos -> GetCntrPosX()-4,_pVisnPos -> GetCntrPosY()+4)){
                     SetCadOfsGain(tlRHght     );
                     dCheckX =_pVisnPos -> GetCntrPosX()+2;
                     dCheckY =_pVisnPos -> GetCntrPosY()-2;

                 }
                 else   {EM_SetErrMsg(eiHGT_RangeErr , "�����˻��� ���� ���� ��ġ Ȯ�κҰ�"); return true ;}



                 dCheckX -=  g_tMidPosOfs.dX ;
                 dCheckY -=  g_tMidPosOfs.dY ;

                 dCheckX +=  _iOfsX ;
                 dCheckY +=  _iOfsY ;

                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dCheckX , dCheckY , false);

                 _iStep++;
                 return false ;

        case 11: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dCheckX , dCheckY , false)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }
                 dCheckOffset = dCrntHeight ;
                 dCheckHt = PM.GetValue(miRTL_ZVisn , pvRTL_ZVisnCheckWork);

                 PM.SetValue(miRTL_ZVisn , pvRTL_ZVisnCmsAlign ,dCheckHt - dCheckOffset);

                 MoveMotr(miRTL_ZVisn , piRTL_ZVisnCmsAlign);
                 _iStep++;
                 return false ;

        case 12: if(!MoveMotr(miRTL_ZVisn , piRTL_ZVisnCmsAlign)) return false ;

                 _iStep=0;
                 return true ;
        }
}

//return true �ÿ� _iStepȮ�� �ϸ� 0�̿��� ���� �Ϸ� 0�� �ƴϸ� ����.
bool CRightTool::CycleMultiHeightCheck(int &_iStep  , double &_dHeight ,double _dPosX , double _dPosY,bool _bUseConfocal)
{
    static CDelayTimer tmDelay ;
    static double dMinHeight  ;
    static double dMaxHeight  ;
    static double dChechSum   ;
           double dCheckAvr   ;

    static double dCrntHeight ;


    //int iHeightStepCnt = OM.DevOptn.iHeightStepCnt ;
    //if(iHeightStepCnt <=0) iHeightStepCnt = 1 ;
    //const int iStepMaxCnt = iHeightStepCnt*2 ;
    //static int iStepCnt = 0 ;

    int iHeightStepCnt = OM.DevOptn.iHeightStepCnt ;
    if(iHeightStepCnt <1) iHeightStepCnt = 1 ;
    const int iStepMaxCnt = iHeightStepCnt ;
    static int iStepCnt = 0 ;


    //�����Ϸ��� �������� ���ͷ� ���� ��.
    double dTotalRangeX = OM.DevOptn.dHeightPitchX * (iStepMaxCnt-1) ;
    double dTotalRangeY = OM.DevOptn.dHeightPitchY * (iStepMaxCnt-1) ;

    static double dSttPosX ;
    static double dSttPosY ;

    const int iStepDelay = OM.CmnOptn.iRightHeightStepDelay ;

    static String sCheckDatas ;


    //Cycle.
    switch (_iStep) {
        default :
                  return true ;

        case 10: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!MT_GetStopInpos(miRTL_ZVisn )) return false ;
                  //������.
                 dMinHeight  = 999999 ;
                 dMaxHeight  =-999999 ;
                 dCrntHeight = 0.0 ;
                 dChechSum   = 0.0 ;

                 iStepCnt = 0 ;

                 sCheckDatas = "";

                 //�ѷ��������� �ݶ��ؼ� ����ġ�� ��ŸƮ ����Ʈ.
                 dSttPosX = _dPosX + dTotalRangeX / 2.0 ;
                 dSttPosY = _dPosY + dTotalRangeY / 2.0 ;


                 Trace("dSttPosX",String(dSttPosX).c_str());
                 Trace("dSttPosY",String(dSttPosY).c_str());
                 MT_GoAbsRun(miRTL_XGenRr , dSttPosX);
                 MT_GoAbsRun(miRTL_YVisn  , dSttPosY);
                 if(_bUseConfocal) {
                     MoveMotr(miRTL_ZVisn , piRTL_ZVisnCmsConfocal);
                 }

                 _iStep++;
                 return false ;


        //�ؿ��� ��.
        //�ݺ����� ����.
        case 11: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!MT_GetStopInpos(miRTL_ZVisn )) return false ;
                 tmDelay.Clear();

                 _iStep++;
                 return false ;

        case 12:
                 if(iStepCnt == 0 && !tmDelay.OnDelay(true , OM.CmnOptn.iRightHeightDelay))return false ;
                 if(iStepCnt != 0 && !tmDelay.OnDelay(true , iStepDelay                  ))return false ;

                 iStepCnt++;
                 if(!_bUseConfocal) {
                     Rs232Keyence.CheckHeight(kcChA); //Right����.
                 }
                 _iStep++;
                 return false ;

        case 13: if(!_bUseConfocal) {
                     if(!Rs232Keyence.GetMsgEnd()) return false ;
                     dCrntHeight = Rs232Keyence.GetHeightData(kcChA) ;
                     if(LASER_MAX == dCrntHeight) {
                         _dHeight = LASER_MAX ;
                         sCheckDatas += String(dCrntHeight) + "," ;
                         Trace("Min Detect MultiHeightDatas" , sCheckDatas.c_str());
                         //EM_SetErrMsg(eiHGT_RangeErr , "Right ������ ���� ������ ������������ Ȯ���ϼ���.");
                         return true ;
                     }
                 }
                 else {
                     dCrntHeight = Confocal.GetHeight();
                     if(dCrntHeight <= CONFOCAL_MIN){ //���� �̻�.
                         _dHeight = CONFOCAL_MIN ;
                         sCheckDatas += String(dCrntHeight) + "," ;
                         Trace("Min Detect MultiHeightDatas" , sCheckDatas.c_str());
                         //EM_SetErrMsg(eiHGT_RangeErr , "Right Confocal ���� ������ ������������ Ȯ���ϼ���.");
                         return true ;
                     }
                     if(dCrntHeight >= CONFOCAL_MAX) { //���� �̻�.
                         _dHeight = CONFOCAL_MAX ;
                         sCheckDatas += String(dCrntHeight) + "," ;
                         Trace("Max Detect MultiHeightDatas" , sCheckDatas.c_str());
                         //EM_SetErrMsg(eiHGT_RangeErr , "Right Confocal ���� ������ ������������ Ȯ���ϼ���.");
                         return true ;
                     }
                 }
                 dChechSum   += dCrntHeight ;

                 sCheckDatas += String(dCrntHeight) + "," ;

                 if(dMinHeight > dCrntHeight) dMinHeight = dCrntHeight ;
                 if(dMaxHeight < dCrntHeight) dMaxHeight = dCrntHeight ;

                 if(iStepCnt < iStepMaxCnt) {//���⼭ 1�Ȼ��ָ� 10�����ÿ� 21�� ������.

                     MT_GoIncRun(miRTL_XGenRr , -OM.DevOptn.dHeightPitchX);
                     MT_GoIncRun(miRTL_YVisn  , -OM.DevOptn.dHeightPitchY);

                     _iStep = 11 ;
                     return false ;
                 }

                 if(iStepMaxCnt){
                     dCheckAvr = dChechSum /(double)(iStepMaxCnt);
                 }
                 else { //���� ������ �ȵȴ�.
                     dCheckAvr = dCrntHeight ;
                 }

                 dCheckAvr = ((int)(dCheckAvr * 10000))/(double)10000.0 ;


                 Trace("MultiHeightDatas" , sCheckDatas.c_str());
                 Trace("MultiHeightMin"   , String(dMinHeight).c_str());
                 Trace("MultiHeightMax"   , String(dMaxHeight).c_str());
                 Trace("MultiHeightSum"   , String(dChechSum).c_str());
                 Trace("MultiHeightCnt"   , String((double)(iStepMaxCnt)).c_str());
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

bool CRightTool::CycleSigleHeightCheck(int &_iStep  , double &_dHeight ,double _dPosX , double _dPosY,bool _bUseConfocal)
{
    static CDelayTimer tmDelay ;
    static double dCrntHeight ;

    //�����Ϸ��� �������� ���ͷ� ���� ��.
    const int iStepDelay = 1 ;

    //Cycle.
    switch (_iStep) {
        default :
                  return true ;

        case 10: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!MT_GetStopInpos(miRTL_ZVisn )) return false ;

                 MT_GoAbsRun(miRTL_XGenRr , _dPosX);
                 MT_GoAbsRun(miRTL_YVisn  , _dPosY);
                 if(_bUseConfocal) {
                     MoveMotr(miRTL_ZVisn , piRTL_ZVisnCmsConfocal);
                 }


                 _iStep++;
                 return false ;


        //�ؿ��� ��.
        //�ݺ����� ����.
        case 11: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!MT_GetStopInpos(miRTL_ZVisn )) return false ;
                 tmDelay.Clear();

                 _iStep++;
                 return false ;

        case 12:
                 if(!tmDelay.OnDelay(true , OM.CmnOptn.iRightHeightDelay))return false ;

                 if(!_bUseConfocal) {
                     Rs232Keyence.CheckHeight(kcChA); //Right����.
                 }
                 _iStep++;
                 return false ;

        case 13: if(!_bUseConfocal) {
                     if(!Rs232Keyence.GetMsgEnd()) return false ;
                     dCrntHeight = Rs232Keyence.GetHeightData(kcChA) ;
                     if(LASER_MAX == dCrntHeight) {
                         _dHeight = LASER_MAX ;
                         Trace("Min Detect SingleHeightDatas" , String(dCrntHeight).c_str());

                         return true ;
                     }
                 }
                 else {
                     dCrntHeight = Confocal.GetHeight();
                     if(dCrntHeight <= CONFOCAL_MIN){ //���� �̻�.
                         _dHeight = CONFOCAL_MIN ;
                         Trace("Min Detect SingleHeightDatas" , String(dCrntHeight).c_str());
                         return true ;
                     }
                     if(dCrntHeight >= CONFOCAL_MAX) { //���� �̻�.
                         _dHeight = CONFOCAL_MAX ;
                         Trace("Max Detect SingleHeightDatas" , String(dCrntHeight).c_str());
                         return true ;
                     }
                 }

                 _dHeight = dCrntHeight;

                 _iStep=0;
                 return true ;
        }
}

bool CRightTool::CycleCmsRAlign()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 20000 )) {
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    //���� ���ո��.
    static String          sMasterInspId     ;
    static String          sSlaveInspId      ;
    static CCadVisnPoint * pStandVisnPosMain ;
    static CCadVisnPoint * pStandVisnPosSub  ;
    static CCadVisnPoint * pWorkVisnPosMain  ;
    static CCadVisnPoint * pWorkVisnPosSub   ;
    static CCadRect      * pStandCmsRect     ;
    static CCadRect      * pWorkCmsRect      ;


    //4��°�� ���հ˻��.
    static String          sMasterInspId0     ;
    static String          sSlaveInspId0      ;
    static CCadVisnPoint * pStandVisnPosMain0 ;
    static CCadVisnPoint * pStandVisnPosSub0  ;
    static CCadVisnPoint * pWorkVisnPosMain0  ;
    static CCadVisnPoint * pWorkVisnPosSub0   ;
    static CCadRect      * pStandCmsRect0     ;
    static CCadRect      * pWorkCmsRect0      ;
    static double          dStandMasterVisnPosX ;
    static double          dStandMasterVisnPosY ;
    static double          dStandSlaveVisnPosX  ;
    static double          dStandSlaveVisnPosY  ;
    static double          dWorkMasterVisnPosX  ;
    static double          dWorkMasterVisnPosY  ;
    static double          dWorkSlaveVisnPosX   ;
    static double          dWorkSlaveVisnPosY   ;



    double dOriAngle  ;
    double dInspAngle ;

    double dTempX  ;
    double dTempY  ;

    double dTemp ;

    static double dVisnMainX  , dStdVisnMainX;
    static double dVisnMainY  , dStdVisnMainY;
    static double dVisnT      , dStdVisnT;
    static double dVisnSubX   , dStdVisnSubX;
    static double dVisnSubY   , dStdVisnSubY;





    static double dMoveX  ;
    static double dMoveY  ;

    static double dVisn_HexaCntrOfsX ;
    static double dVisn_HexaCntrOfsY ;

    static int iAlignInspCnt = 0;

    String sName ;

    static int iVisnHtCalStep ;


    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: 
                 if(DM.ARAY[riSTG].GetCntStat(csCmsRAlign)) WorkInfo.eStat = csCmsRAlign    ;
                 else                                       WorkInfo.eStat = csCmsRPreAlign ;

                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG)){
                     Step.iCycle=0;
                     return true ;
                 }
                 SetCadOfsGain(tlRVisn );

                 sMasterInspId0     = ""   ;
                 sSlaveInspId0      = ""   ;
                 pStandVisnPosMain0 = NULL ;
                 pStandVisnPosSub0  = NULL ;
                 pWorkVisnPosMain0  = NULL ;
                 pWorkVisnPosSub0   = NULL ;
                 pStandCmsRect      = NULL ;
                 pWorkCmsRect       = NULL ;

                 if(WorkInfo.iCol>0 && WorkInfo.iRow>0){
                     //2�� ���ո� ������ ���� ���ո�.
                     pStandVisnPosMain = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow  ].CM_RAlnMR ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pStandVisnPosSub  = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow  ].CM_RAlnSR ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pStandCmsRect     = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pWorkVisnPosMain  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnML ;
                     pWorkVisnPosSub   = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnSL ;
                     pWorkCmsRect      = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����
                     sMasterInspId     = String("CMA_") + (WorkInfo.iCol-1) + WorkInfo.iRow +"_" + WorkInfo.iCol + WorkInfo.iRow  ;
                     sSlaveInspId      = String("CSA_") + (WorkInfo.iCol-1) + WorkInfo.iRow +"_" + WorkInfo.iCol + WorkInfo.iRow  ;

                     //2�� ���ո� ������ ���� ���ո�.
                     sMasterInspId0     = String("CMA_") + WorkInfo.iCol + (WorkInfo.iRow-1) +"_" + WorkInfo.iCol + WorkInfo.iRow  ;
                     sSlaveInspId0      = String("CSA_") + WorkInfo.iCol + (WorkInfo.iRow-1) +"_" + WorkInfo.iCol + WorkInfo.iRow  ;

                     pStandVisnPosMain0 = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_RAlnMB ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pStandVisnPosSub0  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_RAlnSB ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pWorkVisnPosMain0  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnMT ;
                     pWorkVisnPosSub0   = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnST ;
                     pStandCmsRect0     = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pWorkCmsRect0      = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����

                 }
                 else if(WorkInfo.iCol>0){
                     pStandVisnPosMain = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow  ].CM_RAlnMR ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pStandVisnPosSub  = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow  ].CM_RAlnSR ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pStandCmsRect     = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����

                     pWorkVisnPosMain  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnML ;
                     pWorkVisnPosSub   = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnSL ;
                     pWorkCmsRect      = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����

                     sMasterInspId = String("CMA_") + (WorkInfo.iCol-1) + WorkInfo.iRow +"_" + WorkInfo.iCol + WorkInfo.iRow  ;
                     sSlaveInspId  = String("CSA_") + (WorkInfo.iCol-1) + WorkInfo.iRow +"_" + WorkInfo.iCol + WorkInfo.iRow  ;
                 }
                 else if(WorkInfo.iRow>0){
                     pStandVisnPosMain = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_RAlnMB ;
                     pStandVisnPosSub  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_RAlnSB ;
                     pStandCmsRect     = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����

                     pWorkVisnPosMain  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnMT ;
                     pWorkVisnPosSub   = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnST ;
                     pWorkCmsRect      = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����

                     sMasterInspId = String("CMA_") + WorkInfo.iCol + (WorkInfo.iRow-1) +"_" + WorkInfo.iCol + WorkInfo.iRow  ;
                     sSlaveInspId  = String("CSA_") + WorkInfo.iCol + (WorkInfo.iRow-1) +"_" + WorkInfo.iCol + WorkInfo.iRow  ;
                 }
                 else { //ù°��. ù��° ���� ���ؾ��� �׳� ���� �������� ���δ�.
                     pWorkVisnPosMain  = NULL ;
                     pWorkVisnPosSub   = NULL ;
                     pWorkCmsRect      = NULL ;

                     if(CAD.GetCol()>1) {
                         pStandVisnPosMain = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnMR ; //�����۾��ϴ³��� ������� �ϴ� ����
                         pStandVisnPosSub  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnSR ; //
                         pStandCmsRect     = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����
                         sMasterInspId     = String("CMA_00_10") ;
                     }
                     else if(CAD.GetRow()>1) {
                         pStandVisnPosMain = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnMB ; //�����۾��ϴ³��� ������� �ϴ� ����
                         pStandVisnPosSub  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnSB ; //
                         pStandCmsRect     = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����
                         sMasterInspId     = String("CMA_00_01") ;

                     }
                     sSlaveInspId   = "" ;
                 }

                 sMasterInspId    += ";N" ;
                 sSlaveInspId     += ";N" ;
                 sMasterInspId0   += ";N" ;
                 sSlaveInspId0    += ";N" ;

                 Step.iCycle++;
                 return false ;

        case 11: LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCmsAlign);
                 //MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     );
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait   )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCmsAlign)) return false ;
                 //if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait    )) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                 MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false ;
                 if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0){//ù��° ���� ����.
                     Step.iCycle = 20 ;
                     return false ;
                 }
                 Step.iCycle = 50;
                 return false ;


        //��������.
        //ù���� ����.===========================================================
        case 20: iAlignInspCnt = 0 ;

                 Step.iCycle++;
                 return false ;

        //�ؿ�����.
        case 21: iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;


        case 22:
                 if(!CycleVisnHtCal(iVisnHtCalStep  ,  pStandCmsRect , pStandVisnPosMain ,0 ,0)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }

                 SetCadOfsGain(tlRVisn );
                 dTempX = pStandVisnPosMain -> GetCntrPosX() -g_tMidPosOfs.dX;
                 dTempY = pStandVisnPosMain -> GetCntrPosY() -g_tMidPosOfs.dY;
                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);



                 Step.iCycle++;
                 return false ;



        case 23: if(!VSN_R.SendCommand(sMasterInspId)){ //�ø�  �����  ���� ����.
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 24: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!VSN_R.GetSendCommandEnd()) return false ;

                 //����� 4��и���..
                 dVisn_HexaCntrOfsX =  MT_GetCmdPos(miRTL_XGenRr) - GetCrntStgCntVisnPosX() ;
                 dVisn_HexaCntrOfsY =-(MT_GetCmdPos(miRTL_YVisn ) - GetCrntStgCntVisnPosY()) ;


                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCmsAlign);
                 Step.iCycle++;
                 return false;

        case 25: if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCmsAlign)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 26: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;
                 if(!VSN_R.SendInsp()){ //�ø�  �����  ���� ����.
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 27: if(!VSN_R.GetSendInspEnd()) return false ;
                 iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;

        case 28: if(!CycleVisnHtCal(iVisnHtCalStep  ,  pStandCmsRect , pStandVisnPosSub ,0 ,0)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }

                 SetCadOfsGain(tlRVisn);
                 dTempX = pStandVisnPosSub->GetCntrPosX() -g_tMidPosOfs.dX;
                 dTempY = pStandVisnPosSub->GetCntrPosY() -g_tMidPosOfs.dY;
                 //if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);
                 //MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);

                 Step.iCycle++;
                 return false ;

        case 29: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 30: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;

                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right ���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 31: if(!VSN_R.GetSendInspEnd()) return false ;
                 if(!VSN_R.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right �ø� Sub Align ���� �˻����");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 if(!VSN_R.LoadAlignRslt()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right �ø� Align ����� �ε� ����");
                     Step.iCycle=0 ;
                     return true ;
                 }



                 dOriAngle  = CMath::GetLineAngle(pStandVisnPosMain->GetCntrPosX(),
                                                  pStandVisnPosMain->GetCntrPosY(),
                                                  pStandVisnPosSub ->GetCntrPosX(),
                                                  pStandVisnPosSub ->GetCntrPosY());


                 dInspAngle = CMath::GetLineAngle(pStandVisnPosMain->GetCntrPosX()+VSN_R.GetAlignRslt().dMainX ,
                                                  pStandVisnPosMain->GetCntrPosY()+VSN_R.GetAlignRslt().dMainY ,
                                                  pStandVisnPosSub ->GetCntrPosX()+VSN_R.GetAlignRslt().dSubX  ,
                                                  pStandVisnPosSub ->GetCntrPosY()+VSN_R.GetAlignRslt().dSubY  );
                 //���ʿ��� ���������� ���� ������ 1�� + �϶��� 1 1��- �϶��� 359�� �̷��� ǥ���Ǽ� �ٲ�
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;

                 dVisnMainX = VSN_R.GetAlignRslt().dMainX ; //��ü�� ���ʿ� �������� ����.
                 dVisnMainY = VSN_R.GetAlignRslt().dMainY ; //��ü�� ���ʿ� �������� ����
                 dVisnSubX  = VSN_R.GetAlignRslt().dSubX  ; //��ü�� ���ʿ� �������� ����.
                 dVisnSubY  = VSN_R.GetAlignRslt().dSubY  ; //��ü�� ���ʿ� �������� ����
                 //dVisnT = dInspAngle - dOriAngle; //�ð������ ���.
                 dVisnT = dOriAngle - dInspAngle ; //�̻��ϴ� ����Ʈ�� �Ӱ� �ٸ��� �𸣰���.


                 if(fabs(dVisnMainX) > OM.DevOptn.dRVisnTolXY){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ X�������� ���������� �Ѿ����ϴ�.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 if(fabs(dVisnMainY) > OM.DevOptn.dRVisnTolXY){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ Y�������� ���������� �Ѿ����ϴ�.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 if(fabs(dVisnT) > OM.DevOptn.dRVisnTolAng){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ T�������� ���������� �Ѿ����ϴ�.");
                     Step.iCycle = 0 ;
                     return true ;
                 }




                 SetCadOfsGain(tlRVisn );
                 //������.
                 dTempX = pStandVisnPosMain ->GetCntrPosX() ;
                 dTempY = pStandVisnPosMain ->GetCntrPosY() ;
                 dTempX = pStandVisnPosSub  ->GetCntrPosX() ;
                 dTempY = pStandVisnPosSub  ->GetCntrPosY() ;

                 //���� ��ġ���� ȸ�� ����.
                 //dVisn_HexaCntrOfsX = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_Rect->GetCntrPosX() - CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnM-> GetCntrPosX();
                 //dVisn_HexaCntrOfsY = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_Rect->GetCntrPosY() - CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_LAlnM-> GetCntrPosY();
                 //dTempX = PM.GetValue(miLTL_XGenRr , pvLTL_XVisnCheck) + GetTouchToStgCntX() ;
                 //dTempY = PM.GetValue(miLTL_YVisn  , pvLTL_YVisnCheck) + GetTouchToStgCntY() ;  \\


                 //�̰� 4��и�.
                 //��밪�� ã������ ���� ����Ʈ�� 0,0���� �ְ�  ���� ��ġ���� �������� ����
                 STG.GetRotPntFromCntrPnt( dVisnMainX,
                                           dVisnMainY,
                                           dVisnT ,
                                           dVisn_HexaCntrOfsX + HexaPot.GetEncPos(paX) ,
                                           dVisn_HexaCntrOfsY - HexaPot.GetEncPos(paY) ,
                                           &dMoveX ,
                                           &dMoveY );
                 Step.iCycle++;
                 return false ;

        case 32:

                 //�ٻ� ��Ʈ 1��и�
                 HexaPot.MoveIncRes(paX ,  -dMoveX); //�ٻ� ��Ʈ 1��и�
                 HexaPot.MoveIncRes(paY ,   dMoveY); ////
                 HexaPot.MoveIncRes(paW ,  -dVisnT); //CCW +//CCW +
                 HexaPot.Move();
                 Step.iCycle++;
                 return false ;

        case 33: if(!HexaPot.GetStopInpos()) return false ;
                 iAlignInspCnt++;
                 if(iAlignInspCnt < 2) { //����� �˻� 2�� �ǽ�.
                     Step.iCycle=21;
                     return false ;
                 }

                 MoveMotr(miRTL_XGenRr , piRTL_XWait    );
                 MoveMotr(miRTL_YVisn  , piRTL_YVisnWait);

                 Step.iCycle++ ;
                 return false ;

        case 34: if(!MoveMotr(miRTL_XGenRr , piRTL_XWait    )) return false ;
                 if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait)) return false ;

                 if(WorkInfo.eStat == csCmsRAlign) {
                     SetCadOfsGain(tlStage); //���������� ��(Ʈ������,����������)�� ���� ���⶧���� �������� ���� ����� �Ѵ�.
                     //dTempX = CAD.CM[0][0].PC_Rect -> GetCntrPosX() - g_tMidPosOfs.dX;
                     //dTempY = CAD.CM[0][0].PC_Rect -> GetCntrPosY() - g_tMidPosOfs.dY;
                     dTempX=OM.DevOptn.dBfUVOfsX00;
                     dTempY=OM.DevOptn.dBfUVOfsY00;
                     MT_GoIncRun(miSTG_XStage , dTempX);
                     MT_GoIncRun(miSTG_YStage , dTempY);
                 }

                 //�������� XY
                 //SetCadOfsGain(tlStage); //���������� ���� ���� ���⶧���� �������� ���� ����� �Ѵ�.
                 //dTempX = pCmsRect -> GetCntrPosX() + g_tMidPosOfs.dX;
                 //dTempY = pCmsRect -> GetCntrPosY() + g_tMidPosOfs.dY;
                 //if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 //MT_GoAbsRun(miSTG_XStage , dTempX);
                 //MT_GoAbsRun(miSTG_YStage , dTempY);

                 //SetCadOfsGain(tlStage);
                 //dTempX = pStandVisnPosMain -> GetCntrPosX() ;
                 //dTempY = pStandVisnPosMain -> GetCntrPosY() ;
                 //MT_GoAbsRun(miSTG_XStage , dTempX);
                 //MT_GoAbsRun(miSTG_YStage , dTempY);

                 Step.iCycle++;
                 return false ;

        case 35: if(!MT_GetStopInpos(miSTG_XStage))return false ;
                 if(!MT_GetStopInpos(miSTG_YStage))return false ;

                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait);
                 Step.iCycle++;
                 return false ;


        case 36: if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait)) return false ;
                 if(WorkInfo.eStat == csCmsRAlign) {
                     if(!OM.DevOptn.bUseSecDisp) DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol ,  csCmsREndAlign );
                     else                        DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol ,  csCmsLDisp2    );
                 }
                 else {
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol ,  csCmsRHeight );

                 }

                 Step.iCycle=0;
                 return true ;

        //��������.
        //ù���� �̿� Standard�� �˻�===========================================================
        case 50: //������.
                 iAlignInspCnt = 0 ;
                 iVisnHtCalStep = 10 ;

                 dStdVisnMainX = 0 ; //��ü�� ���ʿ� �������� ����.
                 dStdVisnMainY = 0 ; //��ü�� ���ʿ� �������� ����
                 dStdVisnSubX  = 0 ; //��ü�� ���ʿ� �������� ����.
                 dStdVisnSubY  = 0 ; //��ü�� ���ʿ� �������� ����
                 dStdVisnT     = 0 ;   //�̻��ϴ� ����Ʈ�� �Ӱ� �ٸ��� �𸣰���.


                 Step.iCycle++;
                 return false ;


        case 51:
                 if(!CycleVisnHtCal(iVisnHtCalStep  ,  pStandCmsRect , pStandVisnPosMain ,0 ,0)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }
                 SetCadOfsGain(tlRVisn );

                 dTempX = pStandVisnPosMain -> GetCntrPosX();
                 dTempY = pStandVisnPosMain -> GetCntrPosY();

                 dTempX -=g_tMidPosOfs.dX ;
                 dTempY -=g_tMidPosOfs.dY ;

                 //dTempX -=dStdVisnMainX;
                 //dTempY -=dStdVisnMainY;







                 //if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);



                 Step.iCycle++;
                 return false ;

        case 52: if(!VSN_R.SendCommand(sMasterInspId)){ //�ø�  �����  ���� ����.
                     EM_SetErrMsg(eiVSN_InspNG , "Right ���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 //����� 4��и���..
                 dVisn_HexaCntrOfsX =  MT_GetCmdPos(miRTL_XGenRr) - GetCrntStgCntVisnPosX() ;
                 dVisn_HexaCntrOfsY =-(MT_GetCmdPos(miRTL_YVisn ) - GetCrntStgCntVisnPosY()) ;
                 Step.iCycle++;
                 return false ;

        case 53: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!VSN_R.GetSendCommandEnd()) return false ;

                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;

        case  54: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;

                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true;
                 }
                 Step.iCycle++;
                 return false ;

        case 55: if(!VSN_R.GetSendInspEnd()) return false ;
                 iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;


        case 56: if(!CycleVisnHtCal(iVisnHtCalStep  ,  pStandCmsRect , pStandVisnPosSub ,0 ,0)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }
                 SetCadOfsGain(tlRVisn );
                 dTempX = pStandVisnPosSub->GetCntrPosX() -g_tMidPosOfs.dX - dStdVisnMainX ; //���ٴٵ� ���������� ÷�� 0�̴�.
                 dTempY = pStandVisnPosSub->GetCntrPosY() -g_tMidPosOfs.dY - dStdVisnMainY ;
                 //if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;

                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);
                 //MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);

                 Step.iCycle++;
                 return false ;

        case 57: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 //if(!VSN_R.GetSendCommandEnd()) return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++ ;
                 return false ;

        case 58: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;


                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 59: if(!VSN_R.GetSendInspEnd()) return false ;
                 if(!VSN_R.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right���� �ø� Sub Align ���� �˻����");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 if(!VSN_R.LoadAlignRslt()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right���� �̵�� Align ����� �ε� ����");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 dOriAngle  = CMath::GetLineAngle(pStandVisnPosMain->GetCntrPosX(),
                                                  pStandVisnPosMain->GetCntrPosY(),
                                                  pStandVisnPosSub ->GetCntrPosX(),
                                                  pStandVisnPosSub ->GetCntrPosY());

                 dInspAngle = CMath::GetLineAngle(pStandVisnPosMain->GetCntrPosX()+VSN_R.GetAlignRslt().dMainX ,
                                                  pStandVisnPosMain->GetCntrPosY()+VSN_R.GetAlignRslt().dMainY ,
                                                  pStandVisnPosSub ->GetCntrPosX()+VSN_R.GetAlignRslt().dSubX  ,
                                                  pStandVisnPosSub ->GetCntrPosY()+VSN_R.GetAlignRslt().dSubY  );

                 //���ʿ��� ���������� ���� ������ 1�� + �϶��� 1 1��- �϶��� 359�� �̷��� ǥ���Ǽ� �ٲ�
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;


                 dStdVisnMainX = VSN_R.GetAlignRslt().dMainX ; //��ü�� ���ʿ� �������� ����.
                 dStdVisnMainY = VSN_R.GetAlignRslt().dMainY ; //��ü�� ���ʿ� �������� ����
                 dStdVisnSubX  = VSN_R.GetAlignRslt().dSubX  ; //��ü�� ���ʿ� �������� ����.
                 dStdVisnSubY  = VSN_R.GetAlignRslt().dSubY  ; //��ü�� ���ʿ� �������� ����
                 //dVisnT = dInspAngle - dOriAngle; //�ð������ ���.
                 dStdVisnT     = dOriAngle - dInspAngle;   //�̻��ϴ� ����Ʈ�� �Ӱ� �ٸ��� �𸣰���.

                 VisnRslt.dMasterX = dStdVisnMainX ;
                 VisnRslt.dMasterY = dStdVisnMainY ;


                 if(fabs(dStdVisnMainX) > OM.DevOptn.dRVisnTolXY){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ X�������� ���������� �Ѿ����ϴ�.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 if(fabs(dStdVisnMainY) > OM.DevOptn.dRVisnTolXY){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ Y�������� ���������� �Ѿ����ϴ�.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 if(fabs(dStdVisnT) > OM.DevOptn.dRVisnTolAng){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ T�������� ���������� �Ѿ����ϴ�.");
                     Step.iCycle = 0 ;
                     return true ;
                 }

                 dTempX = PM.GetValue(miRTL_XGenRr , pvRTL_XVisnStgCnt);// - RTL.GetTouchToStgCntX()
                 dTempY = PM.GetValue(miRTL_YVisn  , pvRTL_YVisnStgCnt);//+ RTL.GetTouchToStgCntY()

                 //if(iAlignInspCnt<1){
                 //    iAlignInspCnt++;
                 //    iVisnHtCalStep = 10 ;
                 //    Step.iCycle=51;
                 //    return false ;
                 //
                 //}




                 Step.iCycle= 60 ;
                 return false ;




        //��ũ �۾� �ϱ�...
        //������ ��.
        case 60: iAlignInspCnt = 0 ;

                 Step.iCycle++;
                 return false ;


        case 61: iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;

        case 62: dTempX=0 ;
                 dTempY=0 ;
                 if(WorkInfo.iCol>0)dTempX=-OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY=-OM.DevOptn.dStageInspOfsY;




                 if(!CycleVisnHtCal(iVisnHtCalStep  ,  pWorkCmsRect , pWorkVisnPosMain ,dTempX ,dTempY)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

                 //������.
        case 63: SetCadOfsGain(tlRVisn );
                 dTempX = pWorkVisnPosMain -> GetCntrPosX();
                 dTempY = pWorkVisnPosMain -> GetCntrPosY();

                 dTempX -= g_tMidPosOfs.dX;
                 dTempY -= g_tMidPosOfs.dY;



                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;

                 //�˻� �����ǿ� ���Ĵٵ� ���� ���� ���Ѵ�.
                 //���ٴٵ� �������� �ٿ��� �Ѵ�.
                 //Ȥ�ó� �̰����� ��������� ���ؽø𽺿� ���̴� �ø𽺰��� ���Ÿ��� ���� XY�����V�� �־���� �Ѵ�.
                 //��� �������� �ȳ־� �ɵ�.
                 //X,
                 dTempX -= dStdVisnMainX ;
                 dTempY -= dStdVisnMainY ;

                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);


                 if(!VSN_R.SendCommand(sSlaveInspId)){ //�ø� �����̺�  �����  ���� ����.
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 64: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!VSN_R.GetSendCommandEnd()) return false ;

                 //����� 4��и���..
                 dVisn_HexaCntrOfsX =  MT_GetCmdPos(miRTL_XGenRr) - GetCrntStgCntVisnPosX() ;
                 dVisn_HexaCntrOfsY =-(MT_GetCmdPos(miRTL_YVisn ) - GetCrntStgCntVisnPosY()) ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 65: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;

                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right ���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 66: if(!VSN_R.GetSendInspEnd()) return false ;
                 iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;


        case 67: dTempX=0;
                 dTempY=0;
                 if(WorkInfo.iCol>0)dTempX=-OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY=-OM.DevOptn.dStageInspOfsY;
                 if(!CycleVisnHtCal(iVisnHtCalStep  ,  pWorkCmsRect , pWorkVisnPosSub ,dTempX ,dTempY)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }

                 SetCadOfsGain(tlRVisn );
                 dTempX = pWorkVisnPosSub->GetCntrPosX() -g_tMidPosOfs.dX;
                 dTempY = pWorkVisnPosSub->GetCntrPosY() -g_tMidPosOfs.dY;
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;

                 dTempX -= dStdVisnMainX ;
                 dTempY -= dStdVisnMainY ;

                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);

                 Step.iCycle++;
                 return false ;

        case 68: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 //if(!VSN_R.GetSendCommandEnd()) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 69: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;

                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right ���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 70: if(!VSN_R.GetSendInspEnd()) return false ;
                 if(!VSN_R.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right �ø� Sub Align ���� �˻����");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 if(!VSN_R.LoadAlignRslt()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right �̵�� Align ����� �ε� ����");
                     Step.iCycle=0 ;
                     return true ;
                 }



                 dOriAngle  = CMath::GetLineAngle(CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosX(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosY(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosX(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosY());

                 dInspAngle = CMath::GetLineAngle(CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosX()+VSN_R.GetAlignRslt().dMainX ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosY()+VSN_R.GetAlignRslt().dMainY ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosX()+VSN_R.GetAlignRslt().dSubX  ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosY()+VSN_R.GetAlignRslt().dSubY  );

                 //���ʿ��� ���������� ���� ������ 1�� + �϶��� 1 1��- �϶��� 359�� �̷��� ǥ���Ǽ� �ٲ�
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;


                 sName = pWorkVisnPosMain -> GetName();
                 dOriAngle  = CMath::GetLineAngle(pWorkVisnPosMain->GetCntrPosX(),
                                                  pWorkVisnPosMain->GetCntrPosY(),
                                                  pWorkVisnPosSub ->GetCntrPosX(),
                                                  pWorkVisnPosSub ->GetCntrPosY());

                 sName = pWorkVisnPosSub -> GetName();
                 dInspAngle = CMath::GetLineAngle(pWorkVisnPosMain->GetCntrPosX()+VSN_R.GetAlignRslt().dMainX ,
                                                  pWorkVisnPosMain->GetCntrPosY()+VSN_R.GetAlignRslt().dMainY ,
                                                  pWorkVisnPosSub ->GetCntrPosX()+VSN_R.GetAlignRslt().dSubX  ,
                                                  pWorkVisnPosSub ->GetCntrPosY()+VSN_R.GetAlignRslt().dSubY  );

                 //���ʿ��� ���������� ���� ������ 1�� + �϶��� 1 1��- �϶��� 359�� �̷��� ǥ���Ǽ� �ٲ�
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;

                 dVisnMainX = VSN_R.GetAlignRslt().dMainX ; //��ü�� ���ʿ� �������� ����.
                 dVisnMainY = VSN_R.GetAlignRslt().dMainY ; //��ü�� ���ʿ� �������� ����
                 dVisnT = dOriAngle - dInspAngle ; //�ð������ ���.
                 dVisnSubX = VSN_R.GetAlignRslt().dSubX ;
                 dVisnSubY = VSN_R.GetAlignRslt().dSubY ;

                 VisnRslt.dSlaveX = dVisnMainX ;
                 VisnRslt.dSlaveY = dVisnMainY ;


                 if(fabs(dStdVisnMainX) > OM.DevOptn.dRVisnTolXY){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ X�������� ���������� �Ѿ����ϴ�.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 if(fabs(dStdVisnMainY) > OM.DevOptn.dRVisnTolXY){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ Y�������� ���������� �Ѿ����ϴ�.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 if(fabs(dStdVisnT) > OM.DevOptn.dRVisnTolAng){
                     EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ T�������� ���������� �Ѿ����ϴ�.");
                     Step.iCycle = 0 ;
                     return true ;
                 }


                 //���� ��ġ���� ȸ�� ����.
                 dTempX = PM.GetValue(miRTL_XGenRr , pvRTL_XVisnStgCnt);// - RTL.GetTouchToStgCntX()
                 dTempY = PM.GetValue(miRTL_YVisn  , pvRTL_YVisnStgCnt);//+ RTL.GetTouchToStgCntY()




                 STG.GetRotPntFromCntrPnt( dVisnMainX,
                                           dVisnMainY,
                                           dVisnT ,
                                           dVisn_HexaCntrOfsX + HexaPot.GetEncPos(paX) ,
                                           dVisn_HexaCntrOfsY - HexaPot.GetEncPos(paY) ,
                                           &dMoveX ,
                                           &dMoveY );

                 Trace("dVisnMainX"                                      ,(String(dVisnMainX)).c_str()) ;
                 Trace("dVisnMainY"                                      ,(String(dVisnMainX)).c_str()) ;
                 Trace("dVisnT"                                          ,(String(dVisnT    )).c_str()) ;
                 Trace("dVisn_HexaCntrOfsX + HexaPot.GetEncPos(paX)"     ,(String(dVisn_HexaCntrOfsX + HexaPot.GetEncPos(paX)    )).c_str()) ;
                 Trace("dVisn_HexaCntrOfsY + HexaPot.GetEncPos(paY)"     ,(String(dVisn_HexaCntrOfsY + HexaPot.GetEncPos(paY)    )).c_str()) ;
                 Trace("���ٴٵ� ������ �̵�" ,(String("X:") + dMoveX + "Y:" + dMoveY +"T:" +dVisnT ).c_str()) ;

                 //���ٴٵ� ��Ÿ ���� �̵���.
                 STG.GetRotPntFromCntrPnt( dVisnMainX,
                                           dVisnMainY,
                                           dStdVisnT - dVisnT ,
                                           dVisn_HexaCntrOfsX + HexaPot.GetEncPos(paX) ,
                                           dVisn_HexaCntrOfsY - HexaPot.GetEncPos(paY) ,
                                           &dMoveX ,
                                           &dMoveY );
                 Trace("���ٴٵ� ���� �̵�" ,(String("X:") + dMoveX + "Y:" + dMoveY +"T:" +(dStdVisnT - dVisnT) ).c_str()) ;




                 if(OM.MstOptn.bDonUseMasterT){
                     //���ٴٵ� ���� �̵���.
                     STG.GetRotPntFromCntrPnt( dVisnMainX,
                                               dVisnMainY,
                                               dVisnT ,
                                               dVisn_HexaCntrOfsX + HexaPot.GetEncPos(paX) ,
                                               dVisn_HexaCntrOfsY - HexaPot.GetEncPos(paY) ,
                                               &dMoveX ,
                                               &dMoveY );
                 }
                 else {
                     //���ٴٵ� ��Ÿ ���� �̵���.
                     STG.GetRotPntFromCntrPnt( dVisnMainX,
                                               dVisnMainY,
                                               dStdVisnT - dVisnT ,
                                               dVisn_HexaCntrOfsX + HexaPot.GetEncPos(paX) ,
                                               dVisn_HexaCntrOfsY - HexaPot.GetEncPos(paY) ,
                                               &dMoveX ,
                                               &dMoveY );
                 }

                 Step.iCycle++;
                 return false ;


        case 71:
                 //�ٻ� ��Ʈ 1��и�
                 if(iAlignInspCnt ==0) {
                     HexaPot.MoveIncRes(paX ,  -dMoveX); //�ٻ� ��Ʈ 1��и�
                     HexaPot.MoveIncRes(paY ,   dMoveY); ////
                     HexaPot.MoveIncRes(paW ,  dStdVisnT-dVisnT); //CCW +//CCW +
                 }
                 else {
                     HexaPot.MoveIncRes(paX ,  -dVisnMainX); //�ٻ� ��Ʈ 1��и�
                     HexaPot.MoveIncRes(paY ,   dVisnMainY); ////
                     //HexaPot.MoveIncRes(paW ,  dStdVisnT-dVisnT); //CCW +//CCW +
                 }


                 HexaPot.Move();
                 Step.iCycle++;
                 return false ;

        case 72: if(!HexaPot.GetStopInpos()) return false ;
                 iAlignInspCnt++;

                 //�ϴ��ӽ÷� �ѹ���.
                 if(iAlignInspCnt < 3) { //����� �˻� 2�� �ǽ�.
                     Step.iCycle=61;
                     return false ;
                 }

                 //���ո��� 1���� 2�� �˻� ���� ���� ���.
                 if(WorkInfo.iCol == 0 || WorkInfo.iRow == 0 || WorkInfo.eStat != csCmsRAlign){
                     Step.iCycle=110 ;
                     return false ;
                 }

                 //���ո��� 2���� 2�� �˻� �ϴ� ���.
                 Step.iCycle=80 ;
                 return false ;

//====================================================================================
//2�����ո� Standard�� �˻�===========================================================
        //��������.
        case 80: //������.
                 iAlignInspCnt = 0 ;
                 iVisnHtCalStep = 10 ;

                 dStdVisnMainX = 0 ; //��ü�� ���ʿ� �������� ����.
                 dStdVisnMainY = 0 ; //��ü�� ���ʿ� �������� ����
                 dStdVisnSubX  = 0 ; //��ü�� ���ʿ� �������� ����.
                 dStdVisnSubY  = 0 ; //��ü�� ���ʿ� �������� ����
                 dStdVisnT     = 0 ; //�̻��ϴ� ����Ʈ�� �Ӱ� �ٸ��� �𸣰���.


                 Step.iCycle++;
                 return false ;


        case 81: if(!CycleVisnHtCal(iVisnHtCalStep  ,  pStandCmsRect0 , pStandVisnPosMain0 ,0 ,0)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }
                 SetCadOfsGain(tlRVisn );

                 dTempX = pStandVisnPosMain0 -> GetCntrPosX();
                 dTempY = pStandVisnPosMain0 -> GetCntrPosY();

                 dTempX -=g_tMidPosOfs.dX ;
                 dTempY -=g_tMidPosOfs.dY ;

                 //dTempX -=dStdVisnMainX;
                 //dTempY -=dStdVisnMainY;
                 dStandMasterVisnPosX = dTempX ;
                 dStandMasterVisnPosY = dTempY ;

                 //if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);


                 Step.iCycle++;
                 return false ;

        case 82: if(!VSN_R.SendCommand(sMasterInspId0)){ //�ø�  �����  ���� ����.
                     EM_SetErrMsg(eiVSN_InspNG , "Right ���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 //����� 4��и���..
                 dVisn_HexaCntrOfsX =  MT_GetCmdPos(miRTL_XGenRr) - GetCrntStgCntVisnPosX() ;
                 dVisn_HexaCntrOfsY =-(MT_GetCmdPos(miRTL_YVisn ) - GetCrntStgCntVisnPosY()) ;
                 Step.iCycle++;
                 return false ;

        case 83: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!VSN_R.GetSendCommandEnd()) return false ;

                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;

        case 84: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;

                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true;
                 }
                 Step.iCycle++;
                 return false ;

        case 85: if(!VSN_R.GetSendInspEnd()) return false ;
                 iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;


        case 86: if(!CycleVisnHtCal(iVisnHtCalStep  ,  pStandCmsRect0 , pStandVisnPosSub0 ,0 ,0)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }
                 SetCadOfsGain(tlRVisn );
                 dTempX = pStandVisnPosSub0->GetCntrPosX() -g_tMidPosOfs.dX - dStdVisnMainX ; //���ٴٵ� ���������� ÷�� 0�̴�.
                 dTempY = pStandVisnPosSub0->GetCntrPosY() -g_tMidPosOfs.dY - dStdVisnMainY ;

                 dStandSlaveVisnPosX = dTempX ;
                 dStandSlaveVisnPosY = dTempY ;




                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);

                 Step.iCycle++;
                 return false ;

        case 87: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++ ;
                 return false ;

        case 88: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;
                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 89: if(!VSN_R.GetSendInspEnd()) return false ;
                 if(!VSN_R.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right���� �ø� Sub Align ���� �˻����");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 if(!VSN_R.LoadAlignRslt()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right���� �̵�� Align ����� �ε� ����");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 dOriAngle  = CMath::GetLineAngle(pStandVisnPosMain0->GetCntrPosX(),
                                                  pStandVisnPosMain0->GetCntrPosY(),
                                                  pStandVisnPosSub0 ->GetCntrPosX(),
                                                  pStandVisnPosSub0 ->GetCntrPosY());

                 dInspAngle = CMath::GetLineAngle(pStandVisnPosMain0->GetCntrPosX()+VSN_R.GetAlignRslt().dMainX ,
                                                  pStandVisnPosMain0->GetCntrPosY()+VSN_R.GetAlignRslt().dMainY ,
                                                  pStandVisnPosSub0 ->GetCntrPosX()+VSN_R.GetAlignRslt().dSubX  ,
                                                  pStandVisnPosSub0 ->GetCntrPosY()+VSN_R.GetAlignRslt().dSubY  );

                 //���ʿ��� ���������� ���� ������ 1�� + �϶��� 1 1��- �϶��� 359�� �̷��� ǥ���Ǽ� �ٲ�
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;


                 dStdVisnMainX = VSN_R.GetAlignRslt().dMainX ; //��ü�� ���ʿ� �������� ����.
                 dStdVisnMainY = VSN_R.GetAlignRslt().dMainY ; //��ü�� ���ʿ� �������� ����
                 dStdVisnSubX  = VSN_R.GetAlignRslt().dSubX  ; //��ü�� ���ʿ� �������� ����.
                 dStdVisnSubY  = VSN_R.GetAlignRslt().dSubY  ; //��ü�� ���ʿ� �������� ����
                 //dVisnT = dInspAngle - dOriAngle; //�ð������ ���.
                 dStdVisnT     = dOriAngle - dInspAngle;   //�̻��ϴ� ����Ʈ�� �Ӱ� �ٸ��� �𸣰���.

                 dTempX = PM.GetValue(miRTL_XGenRr , pvRTL_XVisnStgCnt);// - RTL.GetTouchToStgCntX()
                 dTempY = PM.GetValue(miRTL_YVisn  , pvRTL_YVisnStgCnt);//+ RTL.GetTouchToStgCntY()
                 Step.iCycle= 90 ;
                 return false ;




        //��ũ �۾� �ϱ�...
        //������ ��.
        case 90: iAlignInspCnt = 0 ;

                 Step.iCycle++;
                 return false ;


        case 91: iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;

        case 92: dTempX=0 ;
                 dTempY=0 ;
                 if(WorkInfo.iCol>0)dTempX=-OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY=-OM.DevOptn.dStageInspOfsY;

                 if(!CycleVisnHtCal(iVisnHtCalStep  ,  pWorkCmsRect0 , pWorkVisnPosMain0 ,dTempX ,dTempY)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

                 //������.
        case 93: SetCadOfsGain(tlRVisn );
                 dTempX = pWorkVisnPosMain0 -> GetCntrPosX();
                 dTempY = pWorkVisnPosMain0 -> GetCntrPosY();

                 dTempX -= g_tMidPosOfs.dX;
                 dTempY -= g_tMidPosOfs.dY;

                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;

                 //�˻� �����ǿ� ���Ĵٵ� ���� ���� ���Ѵ�.
                 //���ٴٵ� �������� �ٿ��� �Ѵ�.
                 //Ȥ�ó� �̰����� ��������� ���ؽø𽺿� ���̴� �ø𽺰��� ���Ÿ��� ���� XY�����V�� �־���� �Ѵ�.
                 //��� �������� �ȳ־� �ɵ�.
                 //X,
                 dTempX -= dStdVisnMainX ;
                 dTempY -= dStdVisnMainY ;

                 dWorkMasterVisnPosX = dTempX ;
                 dWorkMasterVisnPosY = dTempY ;

                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);








                 if(!VSN_R.SendCommand(sSlaveInspId0)){ //�ø� �����̺�  �����  ���� ����.
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 94: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!VSN_R.GetSendCommandEnd()) return false ;

                 //����� 4��и���..
                 dVisn_HexaCntrOfsX =  MT_GetCmdPos(miRTL_XGenRr) - GetCrntStgCntVisnPosX() ;
                 dVisn_HexaCntrOfsY =-(MT_GetCmdPos(miRTL_YVisn ) - GetCrntStgCntVisnPosY()) ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 95: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;

                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right ���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 96: if(!VSN_R.GetSendInspEnd()) return false ;
                 iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;


        case 97: dTempX=0;
                 dTempY=0;
                 if(WorkInfo.iCol>0)dTempX=-OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY=-OM.DevOptn.dStageInspOfsY;
                 if(!CycleVisnHtCal(iVisnHtCalStep  ,  pWorkCmsRect0 , pWorkVisnPosSub0 ,dTempX ,dTempY)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }

                 SetCadOfsGain(tlRVisn );
                 dTempX = pWorkVisnPosSub0->GetCntrPosX() -g_tMidPosOfs.dX;
                 dTempY = pWorkVisnPosSub0->GetCntrPosY() -g_tMidPosOfs.dY;
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;

                 dTempX -= dStdVisnMainX ;
                 dTempY -= dStdVisnMainY ;

                 dWorkSlaveVisnPosX = dTempX ;
                 dWorkSlaveVisnPosY = dTempY ;

                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);

                 Step.iCycle++;
                 return false ;

        case 98: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 //if(!VSN_R.GetSendCommandEnd()) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 99: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;

                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right ���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 100:if(!VSN_R.GetSendInspEnd()) return false ;
                 if(!VSN_R.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right �ø� Sub Align ���� �˻����");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 if(!VSN_R.LoadAlignRslt()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right �̵�� Align ����� �ε� ����");
                     Step.iCycle=0 ;
                     return true ;
                 }



                 dOriAngle  = CMath::GetLineAngle(CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosX(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosY(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosX(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosY());

                 dInspAngle = CMath::GetLineAngle(CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosX()+VSN_R.GetAlignRslt().dMainX ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosY()+VSN_R.GetAlignRslt().dMainY ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosX()+VSN_R.GetAlignRslt().dSubX  ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosY()+VSN_R.GetAlignRslt().dSubY  );

                 //���ʿ��� ���������� ���� ������ 1�� + �϶��� 1 1��- �϶��� 359�� �̷��� ǥ���Ǽ� �ٲ�
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;


                 sName = pWorkVisnPosMain -> GetName();
                 dOriAngle  = CMath::GetLineAngle(pWorkVisnPosMain0->GetCntrPosX(),
                                                  pWorkVisnPosMain0->GetCntrPosY(),
                                                  pWorkVisnPosSub0 ->GetCntrPosX(),
                                                  pWorkVisnPosSub0 ->GetCntrPosY());

                 sName = pWorkVisnPosSub -> GetName();
                 dInspAngle = CMath::GetLineAngle(pWorkVisnPosMain0->GetCntrPosX()+VSN_R.GetAlignRslt().dMainX ,
                                                  pWorkVisnPosMain0->GetCntrPosY()+VSN_R.GetAlignRslt().dMainY ,
                                                  pWorkVisnPosSub0 ->GetCntrPosX()+VSN_R.GetAlignRslt().dSubX  ,
                                                  pWorkVisnPosSub0 ->GetCntrPosY()+VSN_R.GetAlignRslt().dSubY  );

                 //���ʿ��� ���������� ���� ������ 1�� + �϶��� 1 1��- �϶��� 359�� �̷��� ǥ���Ǽ� �ٲ�
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;

                 dVisnMainX = VSN_R.GetAlignRslt().dMainX ; //��ü�� ���ʿ� �������� ����.
                 dVisnMainY = VSN_R.GetAlignRslt().dMainY ; //��ü�� ���ʿ� �������� ����
                 dVisnT     = dOriAngle - dInspAngle ; //�ð������ ���.
                 dVisnSubX  = VSN_R.GetAlignRslt().dSubX ;
                 dVisnSubY  = VSN_R.GetAlignRslt().dSubY ;

                 dTempX = 0 ; //���̵��ؾ��� �Ÿ�.
                 dTempY = 0 ; //���̵��ؾ��� �Ÿ�.
                 if(WorkInfo.iCol==0&&WorkInfo.iRow==1){
                     dTempY+=OM.DevOptn.dCmsAtachGapY ;
                     dTempY-=OM.DevOptn.dStageInspOfsY;

                     dTempX += OM.DevOptn.dBfUVOfsX01 ;
                     dTempY += OM.DevOptn.dBfUVOfsY01 ;
                 }
                 else if(WorkInfo.iCol==1&&WorkInfo.iRow==0){
                     dTempX+=OM.DevOptn.dCmsAtachGapX;//�����V ����.
                     dTempX-=OM.DevOptn.dStageInspOfsX;

                     dTempX += OM.DevOptn.dBfUVOfsX10 ;
                     dTempY += OM.DevOptn.dBfUVOfsY10 ;
                 }
                 //4��°�� ��� ���ʱ������� ���̱� ������ Y�����V�� ������ �ȵȴ�. 2��4�� ���̰� ������.
                 else if(WorkInfo.iCol==1&&WorkInfo.iRow==1){
                     dTempX+=OM.DevOptn.dCmsAtachGapX;//�����V ����.
                     dTempX-=OM.DevOptn.dStageInspOfsX;

                     dTempY-=OM.DevOptn.dStageInspOfsY;

                     dTempX += OM.DevOptn.dBfUVOfsX11 ;
                     dTempY += OM.DevOptn.dBfUVOfsY11 ;
                 }

                 //�̽ĺ��� ����ϸ��.
                 //�������ո� Main��.
                 dTemp  = dStandMasterVisnPosY - dWorkMasterVisnPosY ;//���� ���� �����ǰ�.
                 dTemp -= dStdVisnMainY ;
                 dTemp += dVisnMainY    ;
                 dTemp -= OM.DevOptn.dActiveEdgeGapY * 2; //���Ʒ� �� �����ؼ� 2����.
                 if(dTemp < -dTempY){
                     sTemp.sprintf("���� ���ո� Main�˻� Y���� %.3f ��ŭ �̵��ؾ� �ϴµ� ������ %.3f�Դϴ�.");
                     EM_SetErrMsg(eiVSN_InspRangeOver , sTemp.c_str());
                     Step.iCycle = 0 ;
                     return true ;
                 }

                 //�������ո� Sub��.
                 dTemp  = dStandSlaveVisnPosY - dWorkSlaveVisnPosY ;//���� ���� �����ǰ�.
                 dTemp -= dStdVisnSubY ;
                 dTemp += dVisnSubY    ;
                 dTemp -= OM.DevOptn.dActiveEdgeGapY * 2;
                 if(dTemp < -dTempY){
                     sTemp.sprintf("���� ���ո� Sub�˻� Y���� %.3f ��ŭ �̵��ؾ� �ϴµ� ������ %.3f�Դϴ�.");
                     EM_SetErrMsg(eiVSN_InspRangeOver , sTemp.c_str());
                     Step.iCycle = 0 ;
                     return true ;
                 }




                 iAlignInspCnt++;

                 //���ո��� 2���� 2�� �˻� �ϴ� ���.
                 Step.iCycle=110 ;
                 return false ;
//==============================================================================
        //������ ���̱�...
        //��������.
        case 110:


                 if(WorkInfo.eStat == csCmsRAlign) {
                     SetCadOfsGain(tlStage);
                     //���߿� ��Ȳ ���� ��� ��Ʈ�� �� �����V�� �ű�°��� ������.
                     //�޴���� ���� ����Ŭ ��� ������ ���Ͼ� ���������� �̿����V �ű��.
                     //��� ��Ʈ�� �׸�ŭ ���� �ϴ� ���°� ��.
                     //�۷ι� �̵�� ����� ���������V�� ���Ͼ �԰�
                     //���� �ϳ��ϳ��� ������ �Դ� �����V�� ��� ��Ʈ�� �������� ���� ������.


                     //�̺κ� ���꿡���������� �Ȱ��� ���Ƿ� ������� �ϸ� �ű� ������ ����.
                     //������ ��ȭ�ÿ� Ʋ������ �� ������ �����V�� �ִ´�.
                     dTempX = 0 ;
                     dTempY = 0 ;
                     if(WorkInfo.iCol==0&&WorkInfo.iRow==1){
                         dTempY+=OM.DevOptn.dCmsAtachGapY ;
                         dTempY-=OM.DevOptn.dStageInspOfsY;

                         dTempX += OM.DevOptn.dBfUVOfsX01 ;
                         dTempY += OM.DevOptn.dBfUVOfsY01 ;
                     }
                     else if(WorkInfo.iCol==1&&WorkInfo.iRow==0){
                         dTempX+=OM.DevOptn.dCmsAtachGapX;//�����V ����.
                         dTempX-=OM.DevOptn.dStageInspOfsX;

                         dTempX += OM.DevOptn.dBfUVOfsX10 ;
                         dTempY += OM.DevOptn.dBfUVOfsY10 ;
                     }
                     //4��°�� ��� ���ʱ������� ���̱� ������ Y�����V�� ������ �ȵȴ�. 2��4�� ���̰� ������.
                     else if(WorkInfo.iCol==1&&WorkInfo.iRow==1){
                         dTempX+=OM.DevOptn.dCmsAtachGapX;//�����V ����.
                         dTempX-=OM.DevOptn.dStageInspOfsX;

                         dTempY-=OM.DevOptn.dStageInspOfsY;

                         dTempX += OM.DevOptn.dBfUVOfsX11 ;
                         dTempY += OM.DevOptn.dBfUVOfsY11 ;
                     }

                     /*
                     dStdVisnMainX
                     dStdVisnMainY
                     dStdVisnSubX
                     dStdVisnSubY
                     //dVisnT = dI
                     dStdVisnT
                     */
                     //int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc
                     MT_GoIncAcDc(miSTG_XStage , dTempX ,1.0,1.0,1.0);
                     MT_GoIncAcDc(miSTG_YStage , dTempY ,1.0,1.0,1.0);
                 }

                 Step.iCycle++;
                 return false ;

        case 111:if(!MT_GetStopInpos(miSTG_XStage))return false ;
                 if(!MT_GetStopInpos(miSTG_YStage))return false ;

                 //����� �˻� �� �̵� �� ���� ������ ���ܼ�.....
                 //DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsREndAlign);
                 if(WorkInfo.eStat == csCmsRAlign) DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRZHeight );
                 else                              DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRHeight  );

                 Step.iCycle=0;
                 return false ;
    }
}

/*
bool CRightTool::TestCode()
{
    double dVisnMainX ;
    double dVisnMainX = VSN_R.GetAlignRslt().dMainX ; //��ü�� ���ʿ� �������� ����.
    double dVisnMainY = VSN_R.GetAlignRslt().dMainY ; //��ü�� ���ʿ� �������� ����
    double dVisnT     = dOriAngle - dInspAngle ; //�ð������ ���.
    double dVisnSubX = VSN_R.GetAlignRslt().dSubX ;
    double dVisnSubY = VSN_R.GetAlignRslt().dSubY ;




    STG.GetRotPntFromCntrPnt( dVisnMainX,
                              dVisnMainY,
                              dVisnT ,
                              dVisn_HexaCntrOfsX + HexaPot.GetEncPos(paX) ,
                              dVisn_HexaCntrOfsY - HexaPot.GetEncPos(paY) ,
                             &dMoveX ,
                             &dMoveY );

    Trace("���ٴٵ� ������ �̵�" ,(String("X:") + dMoveX + "Y:" + dMoveY +"T:" +dVisnT ).c_str()) ;

    //���ٴٵ� ��Ÿ ���� �̵���.
    STG.GetRotPntFromCntrPnt( dVisnMainX,
                              dVisnMainY,
                              dStdVisnT - dVisnT ,
                              dVisn_HexaCntrOfsX + HexaPot.GetEncPos(paX) ,
                              dVisn_HexaCntrOfsY - HexaPot.GetEncPos(paY) ,
                              &dMoveX ,
                              &dMoveY );
    Trace("���ٴٵ� ���� �̵�" ,(String("X:") + dMoveX + "Y:" + dMoveY +"T:" +(dStdVisnT - dVisnT) ).c_str()) ;



}
*/
bool CRightTool::CycleCmsRZHeight()
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    static double dHeightSum = 0.0;
    static int    iHeightCnt = 0 ;

    static double dStandHeightAvr ;
    static double dWorkHeightAvr  ;

    double dTemp ;

    //ù��° �� ���϶� ����.
    static double dHeightPos[4];
    static double dHeight[4];
    double dTiltX , dTiltY ;


    //�ι�°�̻� ���϶� ����. �ﰢ�� ����̴�.
    static CCadHeight  * pStandHeightPos ;
    static CCadHeight  * pWorkHeightPos  ;
    static double dPitch ;

    static bool bWorkHeightScdWork ;

    static double dTempX ;
    static double dTempY ;
    static int iHtStep ;
    static double dCrntHeight ;





    String sName ;


    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: 
                 WorkInfo.eStat = csCmsRZHeight ;
                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG)){
                     Step.iCycle = 0 ;
                     return true ;

                 }

                 if(OM.CmnOptn.bUseConfocal)SetCadOfsGain(tlRConfocal );
                 else                       SetCadOfsGain(tlRHght     );

                 if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0){ //ù��° ���� �˻� �ϸ� �ȉ�.
                     Step.iCycle=0;
                     return true ;
                 }
                 else if(WorkInfo.iCol>0 && WorkInfo.iRow>0){ //���ո��� 2�� �ΰ��� �̹� Height�˻�ÿ� �� ������� ������ ��ŵ�Ѵ�.
                     if(OM.DevOptn.bUseFstDisp  )    DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsREndAlign );
                     else if(OM.DevOptn.bUseSecDisp) DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsLDisp2    );
                     Step.iCycle=0;
                     return true ;
                 }
                 else if(WorkInfo.iCol>0){
                     pStandHeightPos = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow].CM_RHgtR ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pWorkHeightPos  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow].CM_RHgtL ;
                 }
                 else if(WorkInfo.iRow>0){
                     pStandHeightPos = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_RHgtB ;  //�����۾��ϴ³��� ������� �ϴ� ����
                     sName = pStandHeightPos -> GetName() ;
                     pWorkHeightPos  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RHgtT ;
                     sName = pWorkHeightPos -> GetName() ;
                 }
                 else {
                     Step.iCycle = 0 ;
                     return true ;
                 }


                 Step.iCycle++;
                 return false ;

        case 11: LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);

                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork)) return false ;

                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                 MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     );
                 MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait) ;
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait)) return false ;
                 if(!MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false ;

                 Step.iCycle++ ;
                 return false ;

        case 15: iHeightCnt = 0  ;
                 Step.iCycle=50;
                 return false ;


        //2��°�� ���� ��������-------------------------------------------------
        //������ ��.
        //���ظ� ����.=========================================================
        case 50: iHeightCnt = 0 ;

                 //�޸� Ȯ��.
                 StandHeightInfo.SetLineHeightCnt(OM.DevOptn.iLineHeightCnt);

                 //iLineHeightCnt �� ���� ��ġ ���.
                 if(pStandHeightPos->GetSttPosX() == pStandHeightPos->GetEndPosX()) { //��Ƽ��
                     StandHeightInfo.bVertical= true ;
                     dPitch = (pStandHeightPos->GetEndPosY() - pStandHeightPos->GetSttPosY())/(OM.DevOptn.iLineHeightCnt-1) ;
                     dPitch = dPitch ;//fabs(dPitch);
                 }
                 else {
                     StandHeightInfo.bVertical = false ;
                     dPitch = (pStandHeightPos->GetEndPosX() - pStandHeightPos->GetSttPosX())/(OM.DevOptn.iLineHeightCnt-1) ;
                     dPitch = dPitch ; //fabs(dPitch);
                 }

                 Step.iCycle=55;
                 return false ;

        //��������.
        //�ؿ�����.
        case 55:
                 sTemp = pStandHeightPos->GetName();
                 dTempX = pStandHeightPos->GetSttPosX() - g_tMidPosOfs.dX;
                 dTempY = pStandHeightPos->GetSttPosY() - g_tMidPosOfs.dY;
                 //ù���� �ƴϸ� �����V�� �����.. CMOS���� ���� �� �־.
                 //if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 //���ظ��� �����V���� �ϸ� �ȉ�.
                 if(StandHeightInfo.bVertical)dTempY += dPitch * iHeightCnt ;
                 else                         dTempX += dPitch * iHeightCnt ;

                 dTempX-= VisnRslt.dMasterX ;
                 dTempY-= VisnRslt.dMasterY ;


                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Step.iCycle++;
                 return false ;

        case 56: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }

                 if(StandHeightInfo.bVertical) StandHeightInfo.adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_YVisn );
                 else                          StandHeightInfo.adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_XGenRr);
                 StandHeightInfo.adLineHeight[iHeightCnt] = dCrntHeight ;

                 iHeightCnt++;
                 if(iHeightCnt < StandHeightInfo.iLineHeightCnt ){
                     Step.iCycle=55 ;
                     return false ;
                 }

                 dHeightSum = 0;
                 for(int i = 0 ; i < StandHeightInfo.iLineHeightCnt ; i++){
                     Trace( (String("Stand Pos")+i).c_str(),String(StandHeightInfo.adLineHeightPos[i]).c_str());
                     Trace( (String("Stand Height")+i).c_str(),String(StandHeightInfo.adLineHeight   [i]).c_str());
                      dHeightSum += StandHeightInfo.adLineHeight[i] ;
                 }

                 dStandHeightAvr = dHeightSum / StandHeightInfo.iLineHeightCnt ;

                 Step.iCycle=60;
                 return false ;



        //���̴³� ���� ����=========================================================
        //�ϰ� ƿƮ ���� �����Ŀ� �ѹ� �� ���� �Ͽ� Z���� ���� �Ѵ�.

        //�ؿ��� ��.
        case 60: if(!HexaPot.GetStopInpos()) return false ;

                 iHeightCnt = 0 ;

                 //�޸� Ȯ��.
                 WorkHeightInfo.SetLineHeightCnt(OM.DevOptn.iLineHeightCnt);

                 //iLineHeightCnt �� ���� ��ġ ���.
                 if(pWorkHeightPos->GetSttPosX() == pWorkHeightPos->GetEndPosX()) { //��Ƽ��
                     WorkHeightInfo.bVertical= true ;
                     dPitch = (pWorkHeightPos->GetEndPosY() - pWorkHeightPos->GetSttPosY())/(OM.DevOptn.iLineHeightCnt-1) ;
                 }
                 else {
                     WorkHeightInfo.bVertical = false ;
                     dPitch = (pWorkHeightPos->GetEndPosX() - pWorkHeightPos->GetSttPosX())/(OM.DevOptn.iLineHeightCnt-1) ;
                 }
                 Step.iCycle++;
                 return false ;


        //��������.
        //�ؿ�����.
        case 61: dTempX = pWorkHeightPos->GetSttPosX() -g_tMidPosOfs.dX;
                 dTempY = pWorkHeightPos->GetSttPosY() -g_tMidPosOfs.dY;
                 //ù���� �ƴϸ� �����V�� �����.. CMOS���� ���� �� �־.
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;
                 //���ظ��� �����V���� �ϸ� �ȉ�.
                 if(WorkHeightInfo.bVertical) dTempY += dPitch * iHeightCnt ;
                 else                         dTempX += dPitch * iHeightCnt ;

                 dTempX-= VisnRslt.dMasterX ;
                 dTempY-= VisnRslt.dMasterY ;

                 iHtStep     = 10 ;
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Step.iCycle++;
                 return false ;

        case 62: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }

                 if(WorkHeightInfo.bVertical) WorkHeightInfo.adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_YVisn );
                 else                         WorkHeightInfo.adLineHeightPos[iHeightCnt] = MT_GetCmdPos(miRTL_XGenRr);
                 WorkHeightInfo.adLineHeight[iHeightCnt] = dCrntHeight ;

                 iHeightCnt++;
                 if(iHeightCnt < WorkHeightInfo.iLineHeightCnt ){
                     Step.iCycle=61 ;
                     return false ;
                 }

                 dHeightSum = 0 ;
                 for(int i = 0 ; i < WorkHeightInfo.iLineHeightCnt ; i++){
                     Trace( (String("Work Pos")+i).c_str(),String(WorkHeightInfo.adLineHeightPos[i]).c_str());
                     Trace( (String("Work Height")+i).c_str(),String(WorkHeightInfo.adLineHeight   [i]).c_str());
                     dHeightSum += WorkHeightInfo.adLineHeight[i] ;

                 }
                 dWorkHeightAvr = dHeightSum / (double)WorkHeightInfo.iLineHeightCnt ;

                 Trace( "dStandHeightAvr",String(dStandHeightAvr).c_str());
                 Trace( "dWorkHeightAvr" ,String(dWorkHeightAvr ).c_str());

                 HexaPot.MoveIncRes(paZ , (dStandHeightAvr - dWorkHeightAvr)+OM.DevOptn.dSlaveHtOfsZ) ;
                 Trace( "HexaWorkOffsetZ" ,String((dStandHeightAvr - dWorkHeightAvr)+OM.DevOptn.dSlaveHtOfsZ ).c_str());
                 HexaPot.Move();


                 Step.iCycle++;
                 return false ;

        case 63: if(!HexaPot.GetStopInpos()) return false ;

                 //�ι�° Dispensor �ɼ� Ȯ�� �� �̵�.
                 if(OM.DevOptn.bUseSecDisp) DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsLDisp2     );
                 else                       DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsREndAlign  );
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 64: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 if(!IO_GetX(xSTG_StgVcum1) &&!IO_GetX(xSTG_StgVcum2)&&!IO_GetX(xSTG_StgVcum3)){
                    EM_SetErrMsg(eiPRT_VacErr , "Stage�� ������ �������� �ʾҽ��ϴ�.");

                 }
                 Step.iCycle=0;
                 return true ;
    }
}

bool CRightTool::CycleCmsREndAlign()
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    static String sMasterInspId ;
    static String sSlaveInspId  ;

    String sRslt ;

    static CCadVisnPoint * pStandVisnPosMain ;
    static CCadVisnPoint * pStandVisnPosSub  ;
    static CCadVisnPoint * pWorkVisnPosMain  ;
    static CCadVisnPoint * pWorkVisnPosSub   ;

    static CCadRect      * pStandCmsRect ;
    static CCadRect      * pWorkCmsRect ;



    double dOriAngle  ;
    double dInspAngle ;

    double dTempX  ;
    double dTempY  ;

    static double dVisnMainX  , dStdVisnMainX , dRsltVisnX  ;
    static double dVisnMainY  , dStdVisnMainY , dRsltVisnY  ;
    static double dVisnT      , dStdVisnT     , dRsltVisnT  ;
    static double dVisnSubX   , dStdVisnSubX  ;
    static double dVisnSubY   , dStdVisnSubY  ;

    static double dStdMotrPosX ;
    static double dStdMotrPosY ;

    static double dWrkMotrPosX ;
    static double dWrkMotrPosY ;





    static double dMoveX  ;
    static double dMoveY  ;

    static double dVisn_HexaCntrOfsX ;
    static double dVisn_HexaCntrOfsY ;

    static int iAlignInspCnt = 0;

    String sName ;

    static int iVisnHtCalStep ;






    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: 
                 if(DM.ARAY[riSTG].GetCntStat(csCmsREndAlign)) WorkInfo.eStat = csCmsREndAlign ;
                 else if(DM.ARAY[riSTG].GetCntStat(csCmsRFixAlign)) WorkInfo.eStat = csCmsRFixAlign ;
                 else  {
                     Step.iCycle=0 ;
                     return true ;
                 }

                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG)){
                     Step.iCycle = 0 ;
                     return true ;

                 }

                 if(WorkInfo.eStat == csCmsREndAlign && OM.CmnOptn.bIgnrCmsREndAlign){
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsREndHeight);
					 Step.iCycle=0;
					 return true ;
                 }

                 if(WorkInfo.eStat == csCmsRFixAlign && OM.CmnOptn.bIgnrCmsRFixAlign){
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRFixHeight);
					 Step.iCycle=0;
					 return true ;
                 }

                 
                 SetCadOfsGain(tlRVisn );

                 if(WorkInfo.iCol>0){
                     pStandVisnPosMain = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow  ].CM_RAlnMR ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pStandVisnPosSub  = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow  ].CM_RAlnSR ; //�����۾��ϴ³��� ������� �ϴ� ����
                     pStandCmsRect     = CAD.CM[WorkInfo.iCol-1][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����

                     pWorkVisnPosMain  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnML ;
                     pWorkVisnPosSub   = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnSL ;
                     pWorkCmsRect      = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����

                     sMasterInspId = String("CMA_") + (WorkInfo.iCol-1) + WorkInfo.iRow +"_" + WorkInfo.iCol + WorkInfo.iRow  ;
                     sSlaveInspId  = String("CSA_") + (WorkInfo.iCol-1) + WorkInfo.iRow +"_" + WorkInfo.iCol + WorkInfo.iRow  ;
                 }
                 else if(WorkInfo.iRow>0){
                     pStandVisnPosMain = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_RAlnMB ;
                     pStandVisnPosSub  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_RAlnSB ;
                     pStandCmsRect     = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow-1].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����

                     pWorkVisnPosMain  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnMT ;
                     pWorkVisnPosSub   = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnST ;
                     pWorkCmsRect      = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����

                     sMasterInspId = String("CMA_") + WorkInfo.iCol + (WorkInfo.iRow-1) +"_" + WorkInfo.iCol + WorkInfo.iRow  ;
                     sSlaveInspId  = String("CSA_") + WorkInfo.iCol + (WorkInfo.iRow-1) +"_" + WorkInfo.iCol + WorkInfo.iRow  ;
                 }
                 else { //ù°��. ù��° ���� ���ؾ��� �׳� ���� �������� ���δ�.
                     pWorkVisnPosMain  = NULL ;
                     pWorkVisnPosSub   = NULL ;
                     pWorkCmsRect      = NULL ;

                     if(CAD.GetCol()>1) {
                         pStandVisnPosMain = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnMR ; //�����۾��ϴ³��� ������� �ϴ� ����
                         pStandVisnPosSub  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnSR ; //
                         pStandCmsRect     = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����
                         sMasterInspId  = String("CMA_00_10") ;
                     }
                     else if(CAD.GetRow()>1) {
                         pStandVisnPosMain = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnMB ; //�����۾��ϴ³��� ������� �ϴ� ����
                         pStandVisnPosSub  = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_RAlnSB ; //
                         pStandCmsRect     = CAD.CM[WorkInfo.iCol  ][WorkInfo.iRow  ].CM_Rect   ; //�����۾��ϴ³��� ������� �ϴ� ����
                         sMasterInspId  = String("CMA_00_01") ;

                     }
                     sSlaveInspId   = "" ;
                 }

                 if(WorkInfo.eStat == csCmsRFixAlign){
                     sMasterInspId    += ";Y" ; //�̹��� ����.
                     sSlaveInspId     += ";Y" ;
                 }
                 else {
                     sMasterInspId    += ";N" ; //�̹��� ���� ����.
                     sSlaveInspId     += ";N" ;
                 }






                 Step.iCycle++;
                 return false ;

        case 11: LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCmsAlign);
                 //MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     );
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait   )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCmsAlign)) return false ;
                 //if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait    )) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                 MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false ;
                 //if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0){//ù��° ���� �� �۾� ���� �ʴ´�.
                 //    if(OM.DevOptn.iDispSeq == 1){DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol ,  csCmsFix  );}
                 //    else                      {DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol ,  csCmsLDisp1);}
                 //
                 //    Step.iCycle=0;
                 //    return true ;
                 //}
                 Step.iCycle = 50;
                 return false ;


        //��������.
        //ù���� �̿� Standard�� �˻�===========================================================
        case 50: //������.
                 iAlignInspCnt = 0 ;
                 iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;


        case 51:
                 if(!CycleVisnHtCal(iVisnHtCalStep  ,  pStandCmsRect , pStandVisnPosMain ,0 ,0)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }
                 SetCadOfsGain(tlRVisn );
                 dTempX = pStandVisnPosMain -> GetCntrPosX() -g_tMidPosOfs.dX ;
                 dTempY = pStandVisnPosMain -> GetCntrPosY() -g_tMidPosOfs.dY ;
                 //if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;
                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);

                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;

        case 52: if(!m_tmDelay.OnDelay(true , 1000)) return false ;

                 if(!VSN_R.SendCommand(sMasterInspId)){ //�ø�  �����  ���� ����.
                     EM_SetErrMsg(eiVSN_InspNG , "Right ���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 //����� 4��и���..
                 dVisn_HexaCntrOfsX =  MT_GetCmdPos(miRTL_XGenRr) - GetCrntStgCntVisnPosX() ;
                 dVisn_HexaCntrOfsY =-(MT_GetCmdPos(miRTL_YVisn ) - GetCrntStgCntVisnPosY()) ;
                 Step.iCycle++;
                 return false ;

        case 53: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!VSN_R.GetSendCommandEnd()) return false ;

                 dStdMotrPosX = MT_GetEncPos(miRTL_XGenRr) ;
                 dStdMotrPosY = MT_GetEncPos(miRTL_YVisn ) ;

                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false;

        case 54: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;
                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true;
                 }
                 Step.iCycle++;
                 return false ;

        case 55: if(!VSN_R.GetSendInspEnd()) return false ;
                 iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;


        case 56: if(!CycleVisnHtCal(iVisnHtCalStep  ,  pStandCmsRect , pStandVisnPosSub ,0 ,0)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }
                 SetCadOfsGain(tlRVisn );
                 dTempX = pStandVisnPosSub->GetCntrPosX() -g_tMidPosOfs.dX ;
                 dTempY = pStandVisnPosSub->GetCntrPosY() -g_tMidPosOfs.dY ;
                 //if(WorkInfo.iCol>0)dTempX+=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY+=OM.DevOptn.dStageInspOfsY;

                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);
                 //MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);

                 Step.iCycle++;
                 return false ;

        case 57: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 //if(!VSN_R.GetSendCommandEnd()) return false ;
                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false;



        case 58: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;
                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 59: if(!VSN_R.GetSendInspEnd()) return false ;
                 if(!VSN_R.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right���� �ø� Sub Align ���� �˻����");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 if(!VSN_R.LoadAlignRslt()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right���� �̵�� Align ����� �ε� ����");
                     Step.iCycle=0 ;
                     return true ;
                 }



                 dOriAngle  = CMath::GetLineAngle(pStandVisnPosMain->GetCntrPosX(),
                                                  pStandVisnPosMain->GetCntrPosY(),
                                                  pStandVisnPosSub ->GetCntrPosX(),
                                                  pStandVisnPosSub ->GetCntrPosY());

                 dInspAngle = CMath::GetLineAngle(pStandVisnPosMain->GetCntrPosX()+VSN_R.GetAlignRslt().dMainX ,
                                                  pStandVisnPosMain->GetCntrPosY()+VSN_R.GetAlignRslt().dMainY ,
                                                  pStandVisnPosSub ->GetCntrPosX()+VSN_R.GetAlignRslt().dSubX  ,
                                                  pStandVisnPosSub ->GetCntrPosY()+VSN_R.GetAlignRslt().dSubY  );

                 //���ʿ��� ���������� ���� ������ 1�� + �϶��� 1 1��- �϶��� 359�� �̷��� ǥ���Ǽ� �ٲ�
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;


                 dStdVisnMainX = VSN_R.GetAlignRslt().dMainX ; //��ü�� ���ʿ� �������� ����.
                 dStdVisnMainY = VSN_R.GetAlignRslt().dMainY ; //��ü�� ���ʿ� �������� ����
                 dStdVisnSubX  = VSN_R.GetAlignRslt().dSubX  ; //��ü�� ���ʿ� �������� ����.
                 dStdVisnSubY  = VSN_R.GetAlignRslt().dSubY  ; //��ü�� ���ʿ� �������� ����
                 //dVisnT = dInspAngle - dOriAngle; //�ð������ ���.
                 dStdVisnT     = dOriAngle - dInspAngle;   //�̻��ϴ� ����Ʈ�� �Ӱ� �ٸ��� �𸣰���.

                 VisnRslt.dMasterX = dStdVisnMainX;
                 VisnRslt.dMasterY = dStdVisnMainY;




                 if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0){//ù��° ���� �� ��ũ �۾� ���� �ʴ´�.

                     //4�� �̻� ¥�������� �˻縦 �ؾ� �ҵ�.
                     if(WorkInfo.eStat != csCmsRFixAlign) {
                         if(fabs(dStdVisnMainX) > OM.DevOptn.dREndVisnTolXY){
                             EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ X���� ���������� �Ѿ����ϴ�.");

                             Step.iCycle = 0 ;
                             return true ;
                         }
                         if(fabs(dStdVisnMainY) > OM.DevOptn.dREndVisnTolXY){
                             EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ Y���� ���������� �Ѿ����ϴ�.");
                             Step.iCycle = 0 ;
                             return true ;
                         }
                         if(fabs(dStdVisnT) > OM.DevOptn.dREndVisnTolAng){
                             EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ T���� ���������� �Ѿ����ϴ�.");
                             Step.iCycle = 0 ;
                             return true ;
                         }
                     }
                     dRsltVisnX = dStdVisnMainX ;
                     dRsltVisnY = dStdVisnMainY ;
                     dRsltVisnT = dStdVisnT     ;

                     //dRsltVisnX = (int)(dRsltVisnX*10000)  ;
                     //dRsltVisnY = (int)(dRsltVisnY*10000)  ;
                     //dRsltVisnT = (int)(dRsltVisnT*10000)  ;
                     //dRsltVisnX = dRsltVisnX / (double)10000 ;
                     //dRsltVisnY = dRsltVisnY / (double)10000 ;
                     //dRsltVisnT = dRsltVisnT / (double)10000 ;




                     if(WorkInfo.eStat == csCmsREndAlign){//��ȭ����.
                         STG.Spc.sBfAlign00.sprintf("X:%.4f Y:%.4f T:%.4f",dRsltVisnX,dRsltVisnY,dRsltVisnT);
                         //STG.Spc.sBfAlign00 = String("X:") + dRsltVisnX + " Y:" + dRsltVisnY + " T:" + dRsltVisnT ;
                         DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsREndHeight);
                         Trace("BfUV Align Rslt",STG.Spc.sBfAlign00.c_str());
                     }
                     else if(WorkInfo.eStat == csCmsRFixAlign){ //��ȭ����.
                         STG.Spc.sAtAlign00.sprintf("X:%.4f Y:%.4f T:%.4f",dRsltVisnX,dRsltVisnY,dRsltVisnT);
                         //STG.Spc.sAtAlign00 = String("X:") + dRsltVisnX + " Y:" + dRsltVisnY + " T:" + dRsltVisnT ;
                         DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRFixHeight);
                         Trace("AtUV Align Rslt",STG.Spc.sAtAlign00.c_str());
                     }




                     Step.iCycle=0;
                     return true ;
                 }

                 Step.iCycle = 60 ;
                 return false ;




        //��ũ �۾� �ϱ�...
        //������ ��.
        case 60: iAlignInspCnt = 0 ;

                 Step.iCycle++;
                 return false ;


        case 61: iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;

        case 62: dTempX=0 ;
                 dTempY=0 ;
                 //Ȯ�κ����� �̹� �پ��ֱ� ������ �˻� �����V ���� �ϸ� �ȉ�.
                 //if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                 //if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;
                 if(!CycleVisnHtCal(iVisnHtCalStep  ,  pWorkCmsRect , pWorkVisnPosMain ,dTempX ,dTempY)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

                 //������.
        case 63: SetCadOfsGain(tlRVisn );
                 dTempX = pWorkVisnPosMain -> GetCntrPosX() -g_tMidPosOfs.dX;
                 dTempY = pWorkVisnPosMain -> GetCntrPosY() -g_tMidPosOfs.dY;
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dCmsAtachGapX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dCmsAtachGapY;




                 //�˻� �����ǿ� ���Ĵٵ� ���� ���� ���Ѵ�.
                 //���ٴٵ� �������� �ٿ��� �Ѵ�.
                 //Ȥ�ó� �̰����� ��������� ���ؽø𽺿� ���̴� �ø𽺰��� ���Ÿ��� ���� XY�����V�� �־���� �Ѵ�.
                 //��� �������� �ȳ־� �ɵ�.
                 //X,
                 //dTempX -= dStdVisnMainX ;
                 //dTempY -= dStdVisnMainY ;

                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);

                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false;

        case 64: if(!m_tmDelay.OnDelay(true , 1000)) return false ;
                 if(!VSN_R.SendCommand(sSlaveInspId)){ //�ø� �����̺�  �����  ���� ����.
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 65: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 if(!VSN_R.GetSendCommandEnd()) return false ;

                 dWrkMotrPosX = MT_GetEncPos(miRTL_XGenRr) ;
                 dWrkMotrPosY = MT_GetEncPos(miRTL_YVisn ) ;


                 //����� 4��и���..
                 dVisn_HexaCntrOfsX =  MT_GetCmdPos(miRTL_XGenRr) - GetCrntStgCntVisnPosX() ;
                 dVisn_HexaCntrOfsY =-(MT_GetCmdPos(miRTL_YVisn ) - GetCrntStgCntVisnPosY()) ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 66: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;
                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right ���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }
                 Step.iCycle++;
                 return false ;

        case 67: if(!VSN_R.GetSendInspEnd()) return false ;
                 iVisnHtCalStep = 10 ;
                 Step.iCycle++;
                 return false ;


        case 68: dTempX=0;
                 dTempY=0;
                 if(WorkInfo.iCol>0)dTempX=-OM.DevOptn.dCmsAtachGapX;
                 if(WorkInfo.iRow>0)dTempY=-OM.DevOptn.dCmsAtachGapY;
                 if(!CycleVisnHtCal(iVisnHtCalStep  ,  pWorkCmsRect , pWorkVisnPosSub ,dTempX ,dTempY)) return false ;
                 if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                     Step.iCycle=0 ;
                     return true ;
                 }

                 SetCadOfsGain(tlRVisn );
                 dTempX = pWorkVisnPosSub->GetCntrPosX() -g_tMidPosOfs.dX;
                 dTempY = pWorkVisnPosSub->GetCntrPosY() -g_tMidPosOfs.dY;
                 if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dCmsAtachGapX;
                 if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dCmsAtachGapY;

                 dTempX -= dStdVisnMainX ;
                 dTempY -= dStdVisnMainY ;


                 MT_GoAbsRun(miRTL_XGenRr , dTempX);
                 MT_GoAbsRun(miRTL_YVisn  , dTempY);

                 Step.iCycle++;
                 return false ;

        case 69: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;
                 //if(!VSN_R.GetSendCommandEnd()) return false ;
                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false;

        case 70: if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iRightVisnDelay)) return false ;

                 if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right ���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true ;
                 }


                 //UV ���� ����.
                 Rs232UV.SetCuring(uvCh1, OM.DevOptn.iUVBtmBrightness1);
                 Rs232UV.SetCuring(uvCh2, OM.DevOptn.iUVBtmBrightness2);
                 Rs232UV.SetCuring(uvCh3, OM.DevOptn.iUVBtmBrightness3);
                 Rs232UV.SetCuring(uvCh4, OM.DevOptn.iUVBtmBrightness4);






                 Step.iCycle++;
                 return false ;

        case 71: if(!VSN_R.GetSendInspEnd()) return false ;
                 if(!VSN_R.GetInspOk()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right �ø� Sub Align ���� �˻����");
                     Step.iCycle=0 ;
                     return true ;
                 }

                 if(!VSN_R.LoadAlignRslt()){
                     EM_SetErrMsg(eiVSN_InspNG , "Right �̵�� Align ����� �ε� ����");
                     Step.iCycle=0 ;
                     return true ;
                 }



                 dOriAngle  = CMath::GetLineAngle(CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosX(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosY(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosX(),
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosY());

                 dInspAngle = CMath::GetLineAngle(CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosX()+VSN_R.GetAlignRslt().dMainX ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnMB->GetCntrPosY()+VSN_R.GetAlignRslt().dMainY ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosX()+VSN_R.GetAlignRslt().dSubX  ,
                                                  CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RAlnSB->GetCntrPosY()+VSN_R.GetAlignRslt().dSubY  );

                 //���ʿ��� ���������� ���� ������ 1�� + �϶��� 1 1��- �϶��� 359�� �̷��� ǥ���Ǽ� �ٲ�
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;


                 sName = pWorkVisnPosMain -> GetName();
                 dOriAngle  = CMath::GetLineAngle(pWorkVisnPosMain->GetCntrPosX(),
                                                  pWorkVisnPosMain->GetCntrPosY(),
                                                  pWorkVisnPosSub ->GetCntrPosX(),
                                                  pWorkVisnPosSub ->GetCntrPosY());

                 sName = pWorkVisnPosSub -> GetName();
                 dInspAngle = CMath::GetLineAngle(pWorkVisnPosMain->GetCntrPosX()+VSN_R.GetAlignRslt().dMainX ,
                                                  pWorkVisnPosMain->GetCntrPosY()+VSN_R.GetAlignRslt().dMainY ,
                                                  pWorkVisnPosSub ->GetCntrPosX()+VSN_R.GetAlignRslt().dSubX  ,
                                                  pWorkVisnPosSub ->GetCntrPosY()+VSN_R.GetAlignRslt().dSubY  );

                 //���ʿ��� ���������� ���� ������ 1�� + �϶��� 1 1��- �϶��� 359�� �̷��� ǥ���Ǽ� �ٲ�
                 if(dOriAngle  >315) dOriAngle  -= 360 ;
                 if(dInspAngle >315) dInspAngle -= 360 ;

                 dVisnMainX = VSN_R.GetAlignRslt().dMainX ; //��ü�� ���ʿ� �������� ����.
                 dVisnMainY = VSN_R.GetAlignRslt().dMainY ; //��ü�� ���ʿ� �������� ����
                 dVisnT     = dOriAngle - dInspAngle ; //�ð������ ���.
                 dVisnSubX  = VSN_R.GetAlignRslt().dSubX ;
                 dVisnSubY  = VSN_R.GetAlignRslt().dSubY ;

                 VisnRslt.dSlaveX = dVisnMainX;
                 VisnRslt.dSlaveY = dVisnMainY;
                 /* ������ ������ �����V ���� ������ ���ϰ� ����.
                 if(WorkInfo.eStat != csCmsRFixAlign) {
                     if(fabs(dVisnMainX) > OM.DevOptn.dREndVisnTolXY){
                         EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ X�������� ���������� �Ѿ����ϴ�.");
                         Step.iCycle = 100 ;
                         return true ;
                     }
                     if(fabs(dVisnMainY) > OM.DevOptn.dREndVisnTolXY){
                         EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ Y�������� ���������� �Ѿ����ϴ�.");
                         Step.iCycle = 100 ;
                         return true ;
                     }
                     if(fabs(dVisnT) > OM.DevOptn.dREndVisnTolAng){
                         EM_SetErrMsg(eiVSN_InspRangeOver , "Right Cmos Align ������ T�������� ���������� �Ѿ����ϴ�.");
                         Step.iCycle = 100 ;
                         return true ;
                     }
                 }*/

                 //������ ���� �����V ���� ������ ��..
                 if(WorkInfo.eStat != csCmsRFixAlign) { //��ȭ�Ĵ� �ǹ� ����.
                     if(fabs(dVisnMainX - dStdVisnMainX) > OM.DevOptn.dREndVisnTolXY){
                         EM_SetErrMsg(eiVSN_InspRangeOver , ("Right Cmos Align ������ X��������" +String(dVisnMainX - dStdVisnMainX)+ "�� ���������� �Ѿ����ϴ�.").c_str());
                         Step.iCycle = 100 ;
                         return true ;
                     }
                     if(fabs(dVisnMainY - dStdVisnMainY) > OM.DevOptn.dREndVisnTolXY){
                         EM_SetErrMsg(eiVSN_InspRangeOver , ("Right Cmos Align ������ Y��������" +String(dVisnMainY - dStdVisnMainY)+ "�� ���������� �Ѿ����ϴ�.").c_str());
                         Step.iCycle = 100 ;
                         return true ;
                     }
                     if(fabs(dVisnT     - dStdVisnT) > OM.DevOptn.dREndVisnTolAng){
                         EM_SetErrMsg(eiVSN_InspRangeOver , ("Right Cmos Align ������ T��������" +String(dVisnT     - dStdVisnT    )+ "�� ���������� �Ѿ����ϴ�.").c_str());
                         Step.iCycle = 100 ;
                         return true ;
                     }
                 }

                 //dRsltVisnX = (int)(dRsltVisnX*10000)  ;
                 //dRsltVisnY = (int)(dRsltVisnY*10000)  ;
                 //dRsltVisnT = (int)(dRsltVisnT*10000)  ;
                 //dRsltVisnX = dRsltVisnX / (double)10000 ;
                 //dRsltVisnY = dRsltVisnY / (double)10000 ;
                 //dRsltVisnT = dRsltVisnT / (double)10000 ;

                 dRsltVisnX = dVisnMainX - dStdVisnMainX +fabs(dStdMotrPosX - dWrkMotrPosX );
                 dRsltVisnY = dVisnMainY - dStdVisnMainY +fabs(dStdMotrPosY - dWrkMotrPosY );
                 dRsltVisnT = dVisnT     - dStdVisnT     ;

                 //SPC
                 sRslt.sprintf("X:%.4f Y:%.4f T:%.4f",dRsltVisnX,dRsltVisnY,dRsltVisnT);
                 SendLog("VisnRslt=>" + sRslt);
                 Trace("VisnRslt", sRslt.c_str());
                 if(WorkInfo.eStat == csCmsREndAlign){//��ȭ����.
                          if(WorkInfo.iCol==1 && WorkInfo.iRow==0)STG.Spc.sBfAlign00_10 = sRslt;
                     else if(WorkInfo.iCol==0 && WorkInfo.iRow==1)STG.Spc.sBfAlign00_01 = sRslt;
                     else if(WorkInfo.iCol==1 && WorkInfo.iRow==1)STG.Spc.sBfAlign01_11 = sRslt;
                 }
                 else if(WorkInfo.eStat == csCmsRFixAlign){ //��ȭ����.
                          if(WorkInfo.iCol==1 && WorkInfo.iRow==0)STG.Spc.sAtAlign00_10 = sRslt;
                     else if(WorkInfo.iCol==0 && WorkInfo.iRow==1)STG.Spc.sAtAlign00_01 = sRslt;
                     else if(WorkInfo.iCol==1 && WorkInfo.iRow==1)STG.Spc.sAtAlign01_11 = sRslt;
                 }


                 Step.iCycle++;
                 return false ;

        case 72 :if(WorkInfo.eStat == csCmsREndAlign){
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsREndHeight);
                 }
                 else if(WorkInfo.eStat == csCmsRFixAlign){
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csCmsRFixHeight);
                 }


                 if(WorkInfo.eStat == csCmsRFixAlign){
                     //Step.iCycle=90;
                     //return false ;
                 }
                 Step.iCycle=0;
                 return true ;

        case 90 :if(!VSN_R.SendCommand("SaveImg")){
                    EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                    Step.iCycle=0;
                    return true ;
                 }
                 Step.iCycle++;
                 return false ;


        case 91 :if(!VSN_R.GetSendCommandEnd()) return false ;
                 Step.iCycle++;
                 return false;

        case 92: if(!VSN_R.SendInsp()){
                     EM_SetErrMsg(eiVSN_InspNG , "���� ��� �غ� �ȵ� ����");
                     Step.iCycle=0;
                     return true;
                 }
                 Step.iCycle++;
                 return false ;

        case 93: if(!VSN_R.GetSendInspEnd()) return false ;

                 Step.iCycle=0;
                 return false ;




        //������ ġ������ �˻簡 �̻� �Ҷ�.
        case 100: iVisnHtCalStep = 10 ;
                  Step.iCycle++;
                  return false ;

        case 101: dTempX=0 ;
                  dTempY=0 ;
                  //Ȯ�κ����� �̹� �پ��ֱ� ������ �˻� �����V ���� �ϸ� �ȉ�.
                  //if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dStageInspOfsX;
                  //if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dStageInspOfsY;
                  if(!CycleVisnHtCal(iVisnHtCalStep  ,  pWorkCmsRect , pWorkVisnPosMain ,dTempX ,dTempY)) return false ;
                  if(iVisnHtCalStep != 0) { //����������� �ʾҴ�.
                      Step.iCycle=0 ;
                      return true ;
                  }
                  Step.iCycle++;
                  return false ;

                  //������.
        case 102: SetCadOfsGain(tlRVisn );
                  dTempX = pWorkVisnPosMain -> GetCntrPosX() -g_tMidPosOfs.dX;
                  dTempY = pWorkVisnPosMain -> GetCntrPosY() -g_tMidPosOfs.dY;
                  if(WorkInfo.iCol>0)dTempX-=OM.DevOptn.dCmsAtachGapX;
                  if(WorkInfo.iRow>0)dTempY-=OM.DevOptn.dCmsAtachGapY;


                  //�˻� �����ǿ� ���Ĵٵ� ���� ���� ���Ѵ�.
                  //���ٴٵ� �������� �ٿ��� �Ѵ�.
                  //Ȥ�ó� �̰����� ��������� ���ؽø𽺿� ���̴� �ø𽺰��� ���Ÿ��� ���� XY�����V�� �־���� �Ѵ�.
                  //��� �������� �ȳ־� �ɵ�.
                  //X,
                  //dTempX -= dStdVisnMainX ;
                  //dTempY -= dStdVisnMainY ;

                  MT_GoAbsRun(miRTL_XGenRr , dTempX);
                  MT_GoAbsRun(miRTL_YVisn  , dTempY);
                  Step.iCycle++;
                  return false;

        case 103: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                  if(!MT_GetStopInpos(miRTL_YVisn )) return false ;

                  if(WorkInfo.eStat == csCmsREndAlign){
                      m_bNeedHexaJog = true ;
                  }
                  Step.iCycle=0;
                  return true ;
    }
}

//bool CRightTool::CycleCmsRFixEnd()
//{
//    //Check Cycle Time Out.
//    AnsiString sTemp ;
//    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
//        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
//        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
//        Trace(m_sPartName.c_str(),sTemp.c_str());
//        Step.iCycle = 0 ;
//        return true;
//    }
//
//    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
//    if(Step.iCycle != PreStep.iCycle) {
//        Trace(m_sPartName.c_str(),sTemp.c_str());
//    }
//
//    PreStep.iCycle = Step.iCycle ;
//
//    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
//    }
//
//    //Cycle.
//    switch (Step.iCycle) {
//        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
//                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
//                  Step.iCycle = 0 ;
//                  return true ;
//
//
//        case 10: WorkInfo.eStat = csCmsFix ;
//                 FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG);
//                 Step.iCycle++;
//                 return false ;
//
//        case 11: IO_SetY(ySTG_VcumPump1On , false);
//                 IO_SetY(ySTG_VcumPump2On , false);
//                 IO_SetY(ySTG_VcumPump3On , false);
//
//                 m_tmDelay.Clear();
//                 Step.iCycle++;
//                 return false ;
//
//
//
//        case 12: if(!m_tmDelay.OnDelay(true , 1000)) return false ;
//                 HexaPot.MoveIncRes(paZ , -2.0 );
//                 HexaPot.Move();
//                 Step.iCycle++;
//                 return false ;
//
//        case 13: if(!HexaPot.GetStopInpos()) return false ;
//                 HexaPot.MoveAbsRes(paX , HEX_X_WAIT);
//                 HexaPot.MoveAbsRes(paY , HEX_Y_WAIT);
//                 HexaPot.MoveAbsRes(paZ , HEX_Z_WAIT);
//                 HexaPot.MoveAbsRes(paU , HEX_U_WAIT);
//                 HexaPot.MoveAbsRes(paV , HEX_V_WAIT);
//                 HexaPot.MoveAbsRes(paW , HEX_W_WAIT);
//                 Step.iCycle++;
//                 return false ;
//
//        case 14: if(!HexaPot.GetStopInpos()) return false ;
//                 DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csWork);
//
//
//                 Step.iCycle = 20;
//                 return false ;
//        }
//
//}

bool CRightTool::CycleMidREndHeight()
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    static CCadHghtCmd * pHghtCmd ;

    const int iSincMotrMax = 2 ;
    static int iMotorAxis[iSincMotrMax] ; // ���� � ���� �� ����
    double dMotorPos [iSincMotrMax] ; // ���� � ���� ������ X,Y,Z

    //�̰� �ڵ���̼� �ٲ�� �Ҽ��� ����... 1�� 0�� ���� 1�� ������ �̷���.
    const int iCoodination = 0 ;
    static int iCrntNodeCnt ;

    static double dTempX ;
    static double dTempY ;

    static int iHtStep ;
    static double dCrntHeight ;

    String sCrntHeight ;

    //static int iMidREndHeightMode = -1 ; //�Ǹ����� ��ü �˻� ����� ���� �ѹ濡 �� �˻��ؾ� �Ѵ�.


    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10:


                 Step.iCycle++;
                 return false ;

        case 11: 
                 if (DM.ARAY[riSTG].GetCntStat(csCmsREndHeight)) { //��ȭ���� �˻�.
                     WorkInfo.eStat = csCmsREndHeight ;
                 }
                 else if (DM.ARAY[riSTG].GetCntStat(csCmsRFixHeight)) { //��ȭ ���� �˻�.
                     WorkInfo.eStat = csCmsRFixHeight ;
                 }
                 else if(DM.ARAY[riSTG].GetCntStat(csMidREndHeight)) { //�Ǹ����� ��ü �˻� ���
                     WorkInfo.eStat = csMidREndHeight ;
                 }
                 else {
                     Step.iCycle=0;
                     return true ;
                 }



                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG)){
                     Step.iCycle= 0;
                     return true ;
                 }
		 if(WorkInfo.eStat == csCmsREndHeight && OM.CmnOptn.bIgnrCmsREndHeight){
                     if(OM.DevOptn.bUseSecDisp) DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol ,  csCmsLTopUV2);
                     else                       DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol ,  csCmsFix    );

		     Step.iCycle=0;
		     return true ;
                 }

                 if(WorkInfo.eStat == csCmsRFixHeight && OM.CmnOptn.bIgnrCmsRFixHeight){
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csMidREndHeight);
		     Step.iCycle=0;
		     return true ;
                 }

                 if(WorkInfo.eStat == csMidREndHeight && OM.CmnOptn.bIgnrMidREndHeight){    //20160218 ����
                     STG.Spc.dWorkEndTime = Now().Val ;

                     STG.Spc.sDispPres  = (OM.DevOptn.iFstDisp == 1 || OM.DevOptn.iSecDisp == 1) ? Rs232_DisprRr.GetDispData().dPrsPres : Rs232_DisprFt.GetDispData().dPrsPres;

                         //�׳� ���Ƿ� ù������ ���ǵ带 �����´�.
                     for(int i = 0 ; i < CAD.CM[0][0].CM_LDsp1->GetNodeCnt() ; i++){
                         if(CAD.CM[0][0].CM_LDsp1->GetDispOn(i)){
                             STG.Spc.sDispSpeed = CAD.CM[0][0].CM_LDsp1->GetSpeed(i);
                             break ;
                         }
                     }
                     STG.Spc.sUvCycle   = OM.DevOptn.iUvCntScnd ;
                     STG.Spc.sUvPower   = OM.DevOptn.iUVTopBrightness ;
                     STG.Spc.sUvHeight  = PM.GetValue(miLTL_ZDispr , pvLTL_ZDispUVWork);
                     //�׳� ���Ƿ� ù������ ���ǵ带 �����´�.
                     for(int i = 0 ; i < CAD.CM[0][0].CM_LDsp1->GetNodeCnt() ; i++){
                         if(CAD.CM[0][0].CM_LDsp1->GetDispOn(i)){
                             STG.Spc.sUvSpeed = CAD.CM[0][0].CM_LDsp1->GetUVSpeed(i);
                             break ;
                         }
                     }
                     STG.Spc.sUvFix     = OM.DevOptn.iFixDelay ;
                     STG.Spc.s1stZGap   = OM.DevOptn.dMidCmsGapFrst ;
                     STG.Spc.sSensor    = OM.CmnOptn.bUseConfocal ? "Confocal" : "Laser" ;
                     STG.Spc.sBottomUv  = OM.DevOptn.bUseBtmUV ? "Use" : "NotUse" ;

                     STG.WriteSpc();

                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csWorkEnd);
		     Step.iCycle=0;
		     return true ;
                 }
                 pHghtCmd = CAD.CM[WorkInfo.iCol][WorkInfo.iRow].CM_RHgtCheck ;


                 if(OM.CmnOptn.bUseConfocal)SetCadOfsGain(tlRConfocal );
                 else                       SetCadOfsGain(tlRHght     );

                 Step.iCycle++;
                 return false ;

        case 12: LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 13: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork)) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                 MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 15: if(!MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false ;

                 Step.iCycle = 20;
                 return false ;


        //��������.===========================================================
        case 20: iCrntNodeCnt = 0 ;

                 ////���� ���� ���� ����.
                 //iMotorAxis[0] = miRTL_XGenRr ;
                 //iMotorAxis[1] = miRTL_YVisn  ;
                 ////0�� �ڵ���̼� ��ȣ.
                 //MT_ContiSetAxisMap(iCoodination, iSincMotrMax, iMotorAxis) ;
                 //MT_ContiSetAbsRelMode(iCoodination, 0) ; //0:������ 1:�����
                 if (DM.ARAY[riSTG].GetCntStat(csCmsREndHeight)) { //��ȭ���� �˻�.
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0) {STG.Spc.sBfHeight00 = "" ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 0) {STG.Spc.sBfHeight10 = "" ;}
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 1) {STG.Spc.sBfHeight01 = "" ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 1) {STG.Spc.sBfHeight11 = "" ;}
                 }
                 else if(DM.ARAY[riSTG].GetCntStat(csCmsRFixHeight)) { //��ȭ ���� �˻�.
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0) {STG.Spc.sAtHeight00  = "" ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 0) {STG.Spc.sAtHeight10  = "" ;}
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 1) {STG.Spc.sAtHeight01  = "" ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 1) {STG.Spc.sAtHeight11  = "" ;}
                 }
                 else if(DM.ARAY[riSTG].GetCntStat(csMidREndHeight)) { //�Ǹ����� ��ü �˻� ����� ���� �ѹ濡 �� �˻��ؾ� �Ѵ�.
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0) {STG.Spc.sEndHeight00  = "" ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 0) {STG.Spc.sEndHeight10  = "" ;}
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 1) {STG.Spc.sEndHeight01  = "" ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 1) {STG.Spc.sEndHeight11  = "" ;}
                 }
                 else {
                     Step.iCycle=0;
                     return true ;
                 }




                 Step.iCycle=25;
                 return false ;

        //�ؿ��� ��.
        case 25: if(OM.CmnOptn.bUseConfocal)SetCadOfsGain(tlRConfocal );
                 else                       SetCadOfsGain(tlRHght     );



                 dTempX = pHghtCmd->GetPosX(iCrntNodeCnt) - g_tMidPosOfs.dX;
                 dTempY = pHghtCmd->GetPosY(iCrntNodeCnt) - g_tMidPosOfs.dY;

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
                 dCrntHeight = 0 ;
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal);

                 Trace("dTempX",String(dTempX).c_str());
                 Trace("dTempY",String(dTempY).c_str());

                 Step.iCycle++;
                 return false ;

        case 26: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY,OM.CmnOptn.bUseConfocal)) return false ;
                 if(iHtStep != 0){
                     EM_SetErrMsg(eiHGT_RangeErr , (String(dCrntHeight) + " ���� ���� ���� �Ұ�.").c_str());
                     Step.iCycle=0;
                     return true ;
                 }

                 sCrntHeight.sprintf("%.4f",dCrntHeight);

                 if (DM.ARAY[riSTG].GetCntStat(csCmsREndHeight)) { //��ȭ���� �˻�.
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0) {STG.Spc.sBfHeight00 = STG.Spc.sBfHeight00 + " " + sCrntHeight ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 0) {STG.Spc.sBfHeight10 = STG.Spc.sBfHeight10 + " " + sCrntHeight ;}
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 1) {STG.Spc.sBfHeight01 = STG.Spc.sBfHeight01 + " " + sCrntHeight ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 1) {STG.Spc.sBfHeight11 = STG.Spc.sBfHeight11 + " " + sCrntHeight ;}
                 }
                 else if(DM.ARAY[riSTG].GetCntStat(csCmsRFixHeight)) { //��ȭ ���� �˻�.
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0) {STG.Spc.sAtHeight00  = STG.Spc.sAtHeight00 + " " + sCrntHeight ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 0) {STG.Spc.sAtHeight10  = STG.Spc.sAtHeight10 + " " + sCrntHeight ;}
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 1) {STG.Spc.sAtHeight01  = STG.Spc.sAtHeight01 + " " + sCrntHeight ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 1) {STG.Spc.sAtHeight11  = STG.Spc.sAtHeight11 + " " + sCrntHeight ;}
                 }
                 else if(DM.ARAY[riSTG].GetCntStat(csMidREndHeight)) { //�Ǹ����� ��ü �˻� ����� ���� �ѹ濡 �� �˻��ؾ� �Ѵ�.
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 0) {STG.Spc.sEndHeight00  = STG.Spc.sEndHeight00 + " " + sCrntHeight ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 0) {STG.Spc.sEndHeight10  = STG.Spc.sEndHeight10 + " " + sCrntHeight ;}
                     if(WorkInfo.iCol == 0 && WorkInfo.iRow == 1) {STG.Spc.sEndHeight01  = STG.Spc.sEndHeight01 + " " + sCrntHeight ;}
                     if(WorkInfo.iCol == 1 && WorkInfo.iRow == 1) {STG.Spc.sEndHeight11  = STG.Spc.sEndHeight11 + " " + sCrntHeight ;}
                 }
                 else {
                     Step.iCycle=0;
                     return true ;
                 }




                 iCrntNodeCnt++;
                 if(iCrntNodeCnt < pHghtCmd->GetNodeCnt()){
                     Step.iCycle = 25 ;
                     return false ;
                 }

                 if (DM.ARAY[riSTG].GetCntStat(csCmsREndHeight)) { //��ȭ���� �˻�.
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol ,  csCmsLTopUV2);

                     Trace("Bf UV sBfHeight00" , STG.Spc.sBfHeight00.c_str());
                     Trace("Bf UV sBfHeight10" , STG.Spc.sBfHeight10.c_str());
                     Trace("Bf UV sBfHeight01" , STG.Spc.sBfHeight01.c_str());
                     Trace("Bf UV sBfHeight11" , STG.Spc.sBfHeight11.c_str());

                     //UV���ð� �ʰ� ���� ���߿� ���� ���� ���� �ٽ� Ȯ�� ����
                     if(Rs232UV.GetBtmUvUseTime_ms()/(1000 * 60 * 60) >= OM.CmnOptn.iBtmUVLimitTime) {
                         EM_SetErrMsg (eiLTL_UVUseTime , "Bottom UV ��� �ð� �ʰ� ����");
                         Step.iCycle = 0;
                         return true;
                     }


                     //20160204 ���� ����
                     if(OM.DevOptn.bUseBtmUV){
                         Rs232UV.SetCuringOnOff(uvCh1, true);
                         Rs232UV.SetCuringOnOff(uvCh2, true);
                         Rs232UV.SetCuringOnOff(uvCh3, true);
                         Rs232UV.SetCuringOnOff(uvCh4, true);

                         IO_SetY(ySTG_UVAirSol , true);
                     }
                     else {
                         Rs232UV.SetCuringOnOff(uvCh1, false);
                         Rs232UV.SetCuringOnOff(uvCh2, false);
                         Rs232UV.SetCuringOnOff(uvCh3, false);
                         Rs232UV.SetCuringOnOff(uvCh4, false);

                         IO_SetY(ySTG_UVAirSol , false);
                     }

                     m_iLastBtmUVSttTime = GetTime();



                 }
                 else if (DM.ARAY[riSTG].GetCntStat(csCmsRFixHeight)) { //��ȭ ���� �˻�.
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csMidREndHeight);
                     //MoveMotr(miRTL_XGenRr , piRTL_XWait    );
                     //MoveMotr(miRTL_YVisn  , piRTL_YVisnWait);
                     Trace("At UV sBfHeight00" , STG.Spc.sAtHeight00.c_str());
                     Trace("At UV sBfHeight10" , STG.Spc.sAtHeight10.c_str());
                     Trace("At UV sBfHeight01" , STG.Spc.sAtHeight01.c_str());
                     Trace("At UV sBfHeight11" , STG.Spc.sAtHeight11.c_str());

                 }
                 else if(DM.ARAY[riSTG].GetCntStat(csMidREndHeight)) { //�Ǹ����� ��ü �˻� ���
                     DM.ARAY[riSTG].SetStat(WorkInfo.iRow , WorkInfo.iCol , csWorkEnd);
                     if(!DM.ARAY[riSTG].GetCntStat(csMidREndHeight)){

                         MoveMotr(miRTL_XGenRr , piRTL_XWait    );
                         MoveMotr(miRTL_YVisn  , piRTL_YVisnWait);

                         Trace("End UV sEndHeight00" , STG.Spc.sEndHeight00.c_str());
                         Trace("End UV sEndHeight10" , STG.Spc.sEndHeight10.c_str());
                         Trace("End UV sEndHeight01" , STG.Spc.sEndHeight01.c_str());
                         Trace("End UV sEndHeight11" , STG.Spc.sEndHeight11.c_str());



                         STG.Spc.dWorkEndTime = Now().Val ;

                         STG.Spc.sDispPres  = (OM.DevOptn.iFstDisp == 1 || OM.DevOptn.iSecDisp == 1) ? Rs232_DisprRr.GetDispData().dPrsPres : Rs232_DisprFt.GetDispData().dPrsPres;

                         //�׳� ���Ƿ� ù������ ���ǵ带 �����´�.
                         for(int i = 0 ; i < CAD.CM[0][0].CM_LDsp1->GetNodeCnt() ; i++){
                             if(CAD.CM[0][0].CM_LDsp1->GetDispOn(i)){
                                 STG.Spc.sDispSpeed = CAD.CM[0][0].CM_LDsp1->GetSpeed(i);
                                 break ;
                             }
                         }
                         STG.Spc.sUvCycle   = OM.DevOptn.iUvCntScnd ;
                         STG.Spc.sUvPower   = OM.DevOptn.iUVTopBrightness ;
                         STG.Spc.sUvHeight  = PM.GetValue(miLTL_ZDispr , pvLTL_ZDispUVWork);
                         //�׳� ���Ƿ� ù������ ���ǵ带 �����´�.
                         for(int i = 0 ; i < CAD.CM[0][0].CM_LDsp1->GetNodeCnt() ; i++){
                             if(CAD.CM[0][0].CM_LDsp1->GetDispOn(i)){
                                 STG.Spc.sUvSpeed = CAD.CM[0][0].CM_LDsp1->GetUVSpeed(i);
                                 break ;
                             }
                         }
                         STG.Spc.sUvFix     = OM.DevOptn.iFixDelay ;
                         STG.Spc.s1stZGap   = OM.DevOptn.dMidCmsGapFrst ;
                         STG.Spc.sSensor    = OM.CmnOptn.bUseConfocal ? "Confocal" : "Laser" ;
                         STG.Spc.sBottomUv  = OM.DevOptn.bUseBtmUV ? "Use" : "NotUse" ;

                         STG.WriteSpc();

                     }

                 }
                 else {
                     Step.iCycle=0;
                     return true ;
                 }


                 Step.iCycle ++ ;
                 return false ;

        case 27: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MT_GetStopInpos(miRTL_YVisn )) return false ;

                 Step.iCycle = 0 ;
                 return true ;
    }
}

//2015.11.05 ����Ʈ���� �߰� JinS
bool CRightTool::CycleWkoToPtl()
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    String sMidID ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10:
                 WorkInfo.eStat = csWork ;
                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riULD)){
                     Step.iCycle = 0 ;
                     return true ;

                 }

                 HexaPot.MoveAbsRes(paX, HEX_X_WAIT);
                 HexaPot.MoveAbsRes(paY, HEX_Y_WAIT);
                 HexaPot.MoveAbsRes(paZ, HEX_Z_WAIT);
                 HexaPot.MoveAbsRes(paU, HEX_U_WAIT);
                 HexaPot.MoveAbsRes(paV, HEX_V_WAIT);
                 HexaPot.MoveAbsRes(paW, HEX_W_WAIT);
                 HexaPot.Move();

                 Step.iCycle++;
                 return false ;

        case 11: if(!HexaPot.GetStopInpos()) return false ;
                 LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;


        case 12: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait );
                 MoveMotr(miRTL_YVisn  , piRTL_YVisnWait );
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait     )) return false ;
                 if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     )) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                 Step.iCycle = 20;
                 return false ;


        //��������.===========================================================
        case 20: MT_GoAbsRun(miRTL_XGenRr , PM.GetValue(miRTL_XGenRr , piRTL_XLIdxOutStt)-10.0);//MoveMotr(
                 MoveActr(aiSTG_Fix1stFwBw , ccBwd);
                 MoveActr(aiSTG_Fix2ndFwBw , ccBwd);
                 MoveActr(aiSTG_Fix3rdFwBw , ccBwd);
                 MoveActr(aiSTG_Fix4thFwBw , ccBwd);
                 MoveActr(aiSTG_Fix5thFwBw , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 21: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 if(!MoveActr(aiSTG_Fix1stFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix2ndFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix3rdFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix4thFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiSTG_Fix5thFwBw , ccBwd)) return false ;

                 MoveMotr(miRTL_XGenRr , piRTL_XLIdxOutStt , true);
                 Step.iCycle++;
                 return false ;

        case 22: if(IO_GetX(xRTL_PullDetect)){//�� �ε��� ������ ������ �Ӱ� �ֳ�??
                     EM_SetErrMsg(eiPRT_Detect , "�������� ���� �ε��� �ٿ���� ��ü�� �����Ǿ����ϴ�.");
                     Step.iCycle=0;
                     return true ;
                 }
                 if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 MoveActr(aiRTL_LWkPshUpDn , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 23: if(!MoveActr(aiRTL_LWkPshUpDn , ccFwd)) return false ;
                 MoveMotr(miRTL_XGenRr , piRTL_XLIdxOutEnd,true);
                 Step.iCycle++;
                 return false ;

        case 24: if(IO_GetX(xRTL_PullDetect)){//���ٷε��~~~
                     MT_Stop(miRTL_XGenRr);
                     Step.iCycle = 50 ;
                     return false ;
                 }

                 if(!MoveMotr(miRTL_XGenRr , piRTL_XLIdxOutEnd,true)) return false ;
                 MoveActr(aiRTL_LWkPshUpDn,ccBwd);
                 Step.iCycle++;
                 return false ;

        case 25: if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 sMidID = DM.ARAY[riWKO].GetID();
                 DM.ARAY[riPTL].SetStat(csWork );
                 DM.ARAY[riPTL].CHPS[0][0].SetID(sMidID);
                 DM.ARAY[riPTL].SetID(sMidID);
                 DM.ARAY[riWKO].ClearMap();

                 Step.iCycle = 0 ;
                 return true ;

        //���ܻ�Ȳ.
        //�����ε�.
        case 50: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 MT_GoIncRun(miRTL_XGenRr , 10.0);
                 Step.iCycle++;
                 return false ;

        case 51: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 52: if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 EM_SetErrMsg(eiPRT_OverLoad , "������ �� ���� �ε��� �����ε� ����. �ɸ��� Ȯ�����ּ���.");
                 Step.iCycle=0;
                 return true ;

    }
}

bool CRightTool::CycleOut()
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    static int iRc ;

    String sMidID ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: if(IO_GetX(xULD_MidDetect)){
                     EM_SetErrMsg(eiPKG_Unknwn , "��δ� �۾��ϴ� ���Կ� �˼� ���� ��ü�� �������Դϴ�.");
                     Step.iCycle=0;
                     return false ;
                 }
                 WorkInfo.eStat = csEmpty ;
                 if(!FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riULD)){
                     Step.iCycle = 0 ;
                     return true ;

                 }
                 //iRc = OM.DevInfo.iULDMgzBayCnt - WorkInfo.iCol - 1;
                 Step.iCycle++;
                 return false ;

        case 11: LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait );
                 MoveMotr(miRTL_YVisn  , piRTL_YVisnWait );
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnWait     )) return false ;
                 if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     )) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                 Step.iCycle = 20;
                 return false ;

      //2��° �ε��� ���� ����.
        case 20: //MoveMotr(miRTL_XGenRr , piRTL_XRIdxOutStt);
                 MT_GoAbsRun(miRTL_XGenRr , PM.GetValue(miRTL_XGenRr , piRTL_XRIdxOutStt)-10.0);
                 Step.iCycle++;
                 return false ;

        case 21: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 MoveMotr(miRTL_XGenRr , piRTL_XRIdxOutStt , true);
                 Step.iCycle++;
                 return false ;

        case 22: if(IO_GetX(xRTL_OutDetect)){//�� �ε��� ������ ������ �Ӱ� �ֳ�??
                     EM_SetErrMsg(eiPRT_Detect , "�������� ���� �ε��� �ٿ���� ��ü�� �����Ǿ����ϴ�.");
                     Step.iCycle=0;
                     return true ;
                 }
                 if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 MoveActr(aiRTL_RWkPshUpDn , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 23: if(!MoveActr(aiRTL_RWkPshUpDn , ccFwd)) return false ;
                 MoveMotr(miRTL_XGenRr , piRTL_XRIdxOutEnd,true);
                 Step.iCycle++;
                 return false ;

        case 24: if(IO_GetX(xRTL_OutDetect)){//���ٷε��~~~
                     MT_Stop(miRTL_XGenRr);
                     Step.iCycle = 50 ;
                     return false ;
                 }

                 if(!MoveMotr(miRTL_XGenRr , piRTL_XRIdxOutEnd,true)) return false ;
                 MT_GoIncRun(miRTL_XGenRr , 20.0);
                 Step.iCycle++;
                 return false ;

        case 25: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 MoveActr(aiRTL_RWkPshUpDn,ccBwd);
                 Step.iCycle++;
                 return false ;

        case 26: if(!MoveActr(aiRTL_RWkPshUpDn,ccBwd)) return false ;

                 sMidID = DM.ARAY[riPTL].GetID();
                 DM.ARAY[riULD].SetStat(WorkInfo.iRow , WorkInfo.iCol , csWork);
                 DM.ARAY[riULD].CHPS[WorkInfo.iRow][WorkInfo.iCol].SetID(sMidID);

                 DM.ARAY[riPTL].ClearMap();

                 Step.iCycle = 0 ;
                 return true ;


        //����Ʈ�� �����ε�
        case 50: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 MT_GoIncRun(miRTL_XGenRr , 10.0);
                 Step.iCycle++;
                 return false ;

        case 51: if(!MT_GetStopInpos(miRTL_XGenRr)) return false ;
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 52: if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 EM_SetErrMsg(eiPRT_OverLoad , "������ �� ���� �ε��� �����ε� ����. �ɸ��� Ȯ�����ּ���.");
                 Step.iCycle=0;
                 return true ;
    }
}


bool CRightTool::CycleMidRCheck()
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    //static int    iHeightCnt = 0 ;
    //static double dHeightSum = 0.0;

    static int iHtStep ;
    static double dCrntHeight ;
    static int    iCrntNodeCnt ;

    static CCadHghtCmd * pHghtCmd ;

    String sCrntHeight ;

    double dTempX , dTempY ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: 
                 WorkInfo.eStat = csMidRHeight ;
                 FindChip(WorkInfo.iRow , WorkInfo.iCol , WorkInfo.eStat , riSTG);
                 //WorkInfo.iRow  = 0 ;
                 //WorkInfo.iCol  = 0 ;
                 pHghtCmd = CAD.MB.MB_RChk ;
                 SetCadOfsGain(tlRHght );

                 Step.iCycle++;
                 return false ;

        case 11: LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 12: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);
                 //MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     );
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);
                 HexaPot.MoveAbsRes(paX, HEX_X_WAIT);
                 HexaPot.MoveAbsRes(paY, HEX_Y_WAIT);
                 HexaPot.MoveAbsRes(paZ, HEX_Z_WAIT);
                 HexaPot.MoveAbsRes(paU, HEX_U_WAIT);
                 HexaPot.MoveAbsRes(paV, HEX_V_WAIT);
                 HexaPot.MoveAbsRes(paW, HEX_W_WAIT);
                 HexaPot.Move();

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork)) return false ;
                 //if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     )) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;
                 if(!HexaPot.GetStopInpos()) return false ;

                 MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 14: if(!MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false ;

                 Step.iCycle = 20;
                 return false ;

        //�ؿ��� ��.
        case 20:
                 iCrntNodeCnt = 0 ;
                 Step.iCycle = 25 ;

                 STG.Spc.Clear();
                 STG.Spc.dWorkSttTime = Now().Val ;


                 return false ;

        case 25: SetCadOfsGain(tlRHght);
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
                 CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY, false);

                 Trace("dTempX",String(dTempX).c_str());
                 Trace("dTempY",String(dTempY).c_str());

                 Step.iCycle++;
                 return false ;

        case 26: if(!CycleMultiHeightCheck  (iHtStep  , dCrntHeight ,dTempX , dTempY , false)) return false ;
                 sCrntHeight.sprintf("%.4f",dCrntHeight);

                 STG.Spc.sBfHeight00 = STG.Spc.sBfHeight00 + " " + sCrntHeight ;

                 iCrntNodeCnt++;
                 if(iCrntNodeCnt < pHghtCmd->GetNodeCnt()){
                     Step.iCycle = 25 ;
                     return false ;
                 }

                 DM.ARAY[riSTG].SetStat(csWorkEnd );
                 Trace("Bf UV sBfHeight00" , STG.Spc.sBfHeight00.c_str());

                 STG.Spc.dWorkEndTime = Now().Val ;
                 STG.WriteSpc();

                 Step.iCycle = 0;
                 return true;
    }
}


bool CRightTool::CycleHtReZero()
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: LTL.ToolVoid(true);
                 Step.iCycle++;
                 return false ;

        case 11: if(!LTL.ToolVoid()) return false ;
                 MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                 MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork);
                 MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     );
                 MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                 MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait    )) return false ;
                 if(!MoveMotr(miRTL_ZVisn  , piRTL_ZVisnCheckWork)) return false ;
                 if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnWait     )) return false ;
                 if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;

                 MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait) ;

                 Step.iCycle++ ;
                 return false ;

        case 13: if(!MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false ;

                 Step.iCycle = 20;
                 return false ;

                 // Right Tool ���� ���� ���� �����̴� ��!
        case 20: if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                 MoveMotr(miRTL_XGenRr , piRTL_XVisnHtCheck);
                 MoveMotr(miRTL_YVisn  , piRTL_YVisnHtCheck);

                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveMotr(miRTL_XGenRr , piRTL_XVisnHtCheck)) return false ;
                 if(!MoveMotr(miRTL_YVisn  , piRTL_YVisnHtCheck)) return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 22: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                 Rs232Keyence.SetZero(kcChA);

                 Step.iCycle++;
                 return false ;

        case 23: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 MoveMotr(miRTL_YVisn  , piRTL_YVisnWait);
                 MoveMotr(miRTL_XGenRr , piRTL_XWait    );

                 Step.iCycle = 0;
                 return true;
    }
}

bool CRightTool::CycleToolVoid()
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

    if(Stat.bReqStop) { //��ž�ÿ� �׳� ����.
    }

    bool bZTrnsPosOk  ;
    bool bXPosOk      ;



    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10:  //if(GetSeqStep() != scIdle) return false ;


                  bXPosOk     = MT_GetTrgPos(miRTL_XGenRr) <= PM.GetValue(miRTL_XGenRr , pvRTL_XWait     ) && MT_GetStopInpos(miRTL_XGenRr) ;
                  if(bXPosOk) {
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  bZTrnsPosOk = MT_GetTrgPos(miRTL_ZTrsfr) <= PM.GetValue(miRTL_ZTrsfr , pvRTL_ZTrsfrWait) && MT_GetStopInpos(miRTL_ZTrsfr) ;
                  if(!bZTrnsPosOk){
                      MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
                  }

                  MoveActr(aiRTL_LWkPshUpDn , ccBwd);
                  MoveActr(aiRTL_RWkPshUpDn , ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11:  if(!MT_GetStopInpos(miRTL_ZTrsfr )) return false ;
                  if(!MoveActr(aiRTL_LWkPshUpDn , ccBwd)) return false ;
                  if(!MoveActr(aiRTL_RWkPshUpDn , ccBwd)) return false ;
                  MoveMotr(miRTL_XGenRr , piRTL_XWait) ;
                  MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait) ;
                  Step.iCycle++;
                  return false ;

        case 12:  if(!MoveMotr(miRTL_XGenRr , piRTL_XWait    )) return false ;
                  if(!MoveMotr(miRTL_YTrspr , piRTL_YTrsfWait)) return false ;
                  Step.iCycle = 0;
                  return true;
    }
}



bool CRightTool::ToolVoid(bool _bInit)
{
    bool bXPosOk = MT_GetTrgPos(miRTL_XGenRr) <= PM.GetValue(miRTL_XGenRr , pvRTL_XWait     ) && MT_GetStopInpos(miRTL_XGenRr) ;
    if(bXPosOk){
        Stat.bReqToolVoid = false ;
        return true ;
    }

    //���䷱�ÿ�
    if(Step.iSeq != scIdle){
        Stat.bReqToolVoid = true ;
        return false ;
    }
    else { //�޴��� �۾� �϶�.
        if(_bInit){
            InitCycleStep();
        }
        CycleToolVoid();
    }
}


/*
bool CRightTool::ToolVoid() //���漭 ���Ҷ� �����ֱ�.
{



    bool bZTrnsPosOk = MT_GetTrgPos(miRTL_ZTrsfr) <= PM.GetValue(miRTL_ZTrsfr , pvRTL_ZTrsfrWait) && MT_GetStopInpos(miRTL_ZTrsfr) ;
    bool bXPosOk     = MT_GetTrgPos(miRTL_XGenRr) <= PM.GetValue(miRTL_XGenRr , pvRTL_XWait     ) && MT_GetStopInpos(miRTL_XGenRr) ;
    bool bActrBwd    = MoveActr(aiRTL_LWkPshUpDn , ccBwd) &&
                       MoveActr(aiRTL_RWkPshUpDn , ccBwd) ;

    if(bXPosOk){
        return true ;
    }

    bool bRetZ ;
    if(!bZTrnsPosOk) {
        bRetZ = MoveMotr(miRTL_ZTrsfr , piRTL_ZTrsfrWait);
    }
    else {
        bRetZ = true ;
    }
    if(!bRetZ) return false ;



    bool bRetX ;
    if(!bXPosOk) {
        bRetX = MoveMotr(miLTL_XGenRr , piRTL_XWait) ;
    }
    else {
        bRetX = true ;
    }
    return bRetX ;
}  */

//---------------------------------------------------------------------------
bool CRightTool::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ; // Ʈ������ z��, x�� Rr ���򸮰� x�� y�� ������Ʈ ��ġ�� �ִ��� (�������� ���� ��ġ�� �ִ���)
    AnsiString sMsg ;

    // Left X
    double dLTL_XGenRrEnc = MT_GetEncPos(miLTL_XGenRr);
    double dLTL_XGenRrTrg = MT_GetTrgPos(miLTL_XGenRr);
    double dLTL_XGenRrPos = dLTL_XGenRrEnc < dLTL_XGenRrTrg ? dLTL_XGenRrTrg : dLTL_XGenRrEnc ;
    // Right X
    double dRTL_XGenRrEnc = MT_GetEncPos(miRTL_XGenRr);
    double dRTL_XGenRrTrg = MT_GetTrgPos(miRTL_XGenRr);
    double dRTL_XGenRrPos = dRTL_XGenRrEnc < dRTL_XGenRrTrg ? dRTL_XGenRrTrg : dRTL_XGenRrEnc ;

    bool   bRTL_LWkPshDn  = AT_GetCmd(aiRTL_LWkPshUpDn);
    bool   bRTL_RWkPshDn  = AT_GetCmd(aiRTL_RWkPshUpDn);
    bool   bSTG_FixFwd    = AT_GetCmd(aiSTG_Fix1stFwBw) || AT_GetCmd(aiSTG_Fix2ndFwBw) ||
                            AT_GetCmd(aiSTG_Fix3rdFwBw) || AT_GetCmd(aiSTG_Fix4thFwBw) ||
                            AT_GetCmd(aiSTG_Fix5thFwBw) ;
    bool   bRAL_Detect    = IO_GetX(xSTG_Detect3) || IO_GetX(xSTG_Detect4) || IO_GetX(xWKO_Detect5);

    // X�� ����
    if(_iMotr == miRTL_XGenRr){
        if(dLTL_XGenRrPos + dRTL_XGenRrPos > OM.MstOptn.dLTL_RTL_Stroke){
            sMsg = MT_GetName(miRTL_XGenRr) + String("���Ͱ� �浹 ��ġ�� �ֽ��ϴ�.") ;
            bRet = false ;
        }
        if((bRTL_LWkPshDn || bRTL_RWkPshDn) && bSTG_FixFwd && bRAL_Detect){
            sMsg = MT_GetName(miLTL_XGenRr) + String("���� ���簡 �浹 ��ġ�� �ֽ��ϴ�.") ;
            bRet = false;
        }
    }                                                              
    // Y�� ����
    else if(_iMotr == miRTL_YVisn){
        if(MT_GetTrgPos(miRTL_YTrspr) + GetMotrPos(_iMotr, _iPstnId) > OM.MstOptn.dRTL_TRNS_VISN_Strke ||
           MT_GetCmdPos(miRTL_YTrspr) + GetMotrPos(_iMotr, _iPstnId) > OM.MstOptn.dRTL_TRNS_VISN_Strke ){
           sMsg = MT_GetName(miRTL_YTrspr) + String("���Ͱ� �浹 ��ġ�� �ֽ��ϴ�.") ;
           bRet = false ;
        }
    }
    else if(_iMotr == miRTL_YTrspr){
        if(MT_GetTrgPos(miRTL_YVisn) + GetMotrPos(_iMotr, _iPstnId) > OM.MstOptn.dRTL_TRNS_VISN_Strke ||
           MT_GetCmdPos(miRTL_YVisn) + GetMotrPos(_iMotr, _iPstnId) > OM.MstOptn.dRTL_TRNS_VISN_Strke ){
           sMsg = MT_GetName(miRTL_YVisn) + String("���Ͱ� �浹 ��ġ�� �ֽ��ϴ�.") ;
           bRet = false ;
        }
    }
    // Z�� ����� ����
    else if(_iMotr == miRTL_ZVisn){
        //if(!MT_GetStop(miRTL_XGenRr)){
        //    sMsg = MT_GetName(miRTL_XGenRr) + String("���Ͱ� �̵��� �Դϴ�.") ;
        //    bRet = false ;
        //}
        //if(!MT_GetStop(miRTL_YVisn)){
        //    sMsg = MT_GetName(miRTL_YVisn)  + String("���Ͱ� �̵��� �Դϴ�.") ;
        //    bRet = false ;
        //}
        //if(MT_GetCmdPos(miRTL_ZVisn) > PM.GetValue(miRTL_ZVisn , pvRTL_ZVisnWait) + 1.0){
        //    sMsg = MT_GetName(miRTL_ZVisn)  + String("������ ���̰� �����ġ���� �����ϴ�.") ;
        //    bRet = false ;
        //}
    }
    // Z�� Ʈ������
    else if(_iMotr == miRTL_ZTrsfr){
        if(!MT_GetStop(miRTL_XGenRr)){
            sMsg = MT_GetName(miRTL_XGenRr) + String("���Ͱ� �̵��� �Դϴ�.") ;
            bRet = false ;
        }
        if(!MT_GetStop(miRTL_YTrspr)){
            sMsg = MT_GetName(miRTL_YTrspr) + String("���Ͱ� �̵��� �Դϴ�.") ;
            bRet = false ;
        }
        if(MT_GetCmdPos(miRTL_YTrspr) < (PM.GetValue(miRTL_YTrspr , pvRTL_YTrsfCheck)+100)){
            if(MT_GetCmdPos(miIDX_XIndxR) > PM.GetValue(miIDX_XIndxR,pvIDX_XIndxRWait)){
                sMsg = "�ε�����  Wait������ ���� ���� ��ġ �Դϴ�.";
                bRet = false;
            }
        }
    }
    // Z�� Ʈ������
    else if(_iMotr == miRTL_YConvs){
        if(!MT_GetStop(miRTL_XGenRr)){
            sMsg = MT_GetName(miRTL_XGenRr) + String("���Ͱ� �̵��� �Դϴ�.") ;
            bRet = false ;
        }
        if(!MT_GetStop(miRTL_YTrspr)){
            sMsg = MT_GetName(miRTL_YTrspr) + String("���Ͱ� �̵��� �Դϴ�.") ;
            bRet = false ;
        }
        //if(MT_GetCmdPos(miRTL_ZTrsfr) > PM.GetValue(miRTL_ZTrsfr , pvRTL_ZTrsfrWait) + 1.0){
        //    sMsg = MT_GetName(miRTL_ZTrsfr) + String("������ ���̰� �����ġ���� �����ϴ�.") ;
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
bool CRightTool::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;  // ������ �Ǹ��� 2��(�Ǹ��� üũ������)
//
    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiRTL_LWkPshUpDn){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray ���� ������!"); bRet = false ;}
        }
    }
    else if(_iActr == aiRTL_RWkPshUpDn){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray ���� ������!"); bRet = false ;}
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

bool CRightTool::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
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


bool CRightTool::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CRightTool::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(miRTL_ZTrsfr);
    return ;

}

double CRightTool::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return dLastIdxPos ;
}

bool CRightTool::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miRTL_ZTrsfr)) return true  ; //����Ʈ�� �ʿ� ����.
    else                                            return false ;

}

bool CRightTool::CheckStop()
{
    if(!MT_GetStop(miRTL_XGenRr)) return false ;
    if(!MT_GetStop(miRTL_YConvs)) return false ;
    if(!MT_GetStop(miRTL_YTrspr)) return false ;
    if(!MT_GetStop(miRTL_YVisn )) return false ;
    if(!MT_GetStop(miRTL_ZTrsfr)) return false ;
    if(!MT_GetStop(miRTL_ZTrsfr)) return false ;

    if(!AT_Done(aiRTL_RWkPshUpDn)) return false ;
    if(!AT_Done(aiRTL_LWkPshUpDn)) return false ;

    return true ;
}

void CRightTool::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos     );
        UserINI.Load(sPath, "Member" , "dMasterX     " , VisnRslt.dMasterX );
        UserINI.Load(sPath, "Member" , "dMasterY     " , VisnRslt.dMasterY );
        UserINI.Load(sPath, "Member" , "dSlaveX      " , VisnRslt.dSlaveX  );
        UserINI.Load(sPath, "Member" , "dSlaveY      " , VisnRslt.dSlaveY  );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos     );
        UserINI.Load(sPath, "Member" , "dMasterX     " , VisnRslt.dMasterX );
        UserINI.Load(sPath, "Member" , "dMasterY     " , VisnRslt.dMasterY );
        UserINI.Load(sPath, "Member" , "dSlaveX      " , VisnRslt.dSlaveX  );
        UserINI.Load(sPath, "Member" , "dSlaveY      " , VisnRslt.dSlaveY  );

    }
}

double CRightTool::GetTouchToStgCntX()
{
    return PM.GetValue(miRTL_XGenRr , pvRTL_XVisnStgCnt)-PM.GetValue(miRTL_XGenRr , pvRTL_XVisnCheck) ;

}

double CRightTool::GetTouchToStgCntY()
{
    return PM.GetValue(miRTL_YVisn , pvRTL_YVisnStgCnt)-PM.GetValue(miRTL_YVisn , pvRTL_YVisnCheck) ;

}

double CRightTool::GetCrntStgCntVisnPosX()
{
    //���ʺ����� �������� ���� ������ ���.
    //����Ʈ ������ ���� ���� ���� ��ġ. ������̽� ���� ������.
    double dVisnRailCntX = PM.GetValue(miRTL_XGenRr , pvRTL_XVisnStgCnt);

    //���� �������� ��ġ.
    double dStgCrnt    = MT_GetCmdPos(miSTG_XStage);

    //�������� ���忡�� ���Ϳ��� ���� ��ġ���� �Ÿ�.
    double dCntToStgGap = dStgCrnt - PM.GetValue(miSTG_XStage , pvSTG_XStageCntr) ;
    double dVisnStgPos  = dVisnRailCntX - dCntToStgGap ;

    return dVisnStgPos ;
}
double CRightTool::GetCrntStgCntVisnPosY()
{
    //���ʺ����� �������� ���� ������ ���.
    //����Ʈ ������ ���� ���� ���� ��ġ. ������̽� ���� ������.
    double dVisnRailCntY = PM.GetValue(miRTL_YVisn  , pvRTL_YVisnStgCnt) ;

    //���� �������� ��ġ.
    double dStgCrnt    = MT_GetCmdPos(miSTG_YStage);

    //�������� ���忡�� ���Ϳ��� ���� ��ġ���� �Ÿ�.
    double dCntToStgGap = dStgCrnt - PM.GetValue(miSTG_YStage , pvSTG_YStageCntr) ;
    double dVisnStgPos  = dVisnRailCntY + dCntToStgGap ;

    return dVisnStgPos ;
}







