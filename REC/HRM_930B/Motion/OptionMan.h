//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
enum EN_WORK_MODE { wmNormal = 0 , wmHeight = 1}; //0:정상 작업 1:로더에서 꺼내어 로테이터에서 높이 측정만 하고 다시 넣는다.
enum EN_DISP_MODE { dmBoth = 0 , dmFrst = 1 , dmScnd = 2 , dmNotUse = 3 }; //디스펜싱을 하는 조건 0:자연경화&순간경화 1: 자연경화 , 2:순간경화 , 3 = 안함.}


struct CDevInfo {   //device에 대한 Dev_Info
    double       dLDRFMgzSlotPitch; //Loader Tray
    int          iLDRFMgzSlotCnt  ;
    int          iLDRFMgzBayCnt   ;
    double       dLDRRMgzSlotPitch; //Loader Tray
    int          iLDRRMgzSlotCnt  ;
    int          iLDRRMgzBayCnt   ;
    double       dULDMgzSlotPitch ; //Loader Tray
    int          iULDMgzSlotCnt   ;
    int          iULDMgzBayCnt    ;

//   double        dMidWidth        ;
//    double       dMidHeight       ;
    int          iMidColCnt       ; // 10개.
    int          iMidRowCnt       ; // 10개.

//    double       dCmsWidth        ;
//    double       dCmsHeight       ;
//    double       dPcbWidth        ;
//    double       dPcbHeight       ;
} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.



    double       dPlaceZOfs        ; //미들블럭과 CmosPCB가 밀착해서 접합 되지 않는데 그거에 대한 오프셑.
    double       dAttachSpeed1     ; //Z축 dFosPlaceZOfs1~dFosPlaceZOfs2구간.
    double       dAttachSpeed2     ; //Z축 dFosPlaceZOfs2~Attach구간.
    double       dAttachForce      ; //합착 무게.g단위.
    double       dAttachForceOfs   ; //합착 무게.g단위. 오프셑.
    double       dAttachDelay      ; //합착 후 딜레이.

    int          iFixDelay         ; //에폭시 자연경화 딜레이

    double       dDispZOfs         ; //디스펜싱 높이 오프셑.

    double       dStageInspOfsX    ; //합착 할때에 스테이지 이동 오프셑 X,Y로 오프셑을 가지고 나서 높이 측정 및 비젼검사하는 포지션이다.
    double       dStageInspOfsY    ;

    int          iDspChFt          ; //디스펜서 사용 채널.
    int          iDspChRr          ; //디스펜서 사용 채널.

    int          iDspMinAmount     ; //디스펜서 잔양 에러 퍼센트...(20 이면 20이하남았을때 에러.)

    AnsiString   sEpoxyName        ;

    //29추가.
    double       dMidCmsGapFrst    ; //첫번째 장의 미들블럭과 시모스간의 갭.
    double       dMidCmsGap        ; //미들블럭과 시모스간의 갭.

    //bool         bEpoxyFirst       ; // 에폭시 먼저 작업 하는옵션.
    bool         bUsePush          ; // Cmos 누르는 공정 사용 여부.
    //bool         bUseDispRear      ; //이거 체크되어 있으면 Rear디스펜서 사용 안되어 있으면 프론트사용.
    double       dDispAccDcc       ; //디스펜싱 가감속.

    int          iUVTopBrightness  ; // Ch5      대겸 UV 밝기 세팅.
    int          iUVBtmBrightness1 ; // Ch1
    int          iUVBtmBrightness2 ; // Ch2
    int          iUVBtmBrightness3 ; // Ch3
    int          iUVBtmBrightness4 ; // Ch4
    bool         bUseBtmUV         ; // 하부 UV 사용 여부 체크

    double       dLVisnTolXY       ; //절대값 Stage 검사시에 검사 허용 치수.
    double       dLVisnTolAng      ; //절대값 각도.
    double       dRVisnTolXY       ; //절대값 Stage 검사시에 검사 허용 치수.
    double       dRVisnTolAng      ; //절대값 각도.
    double       dREndVisnTolXY    ; //마스터 상대값 마지막 붙이고 나서 검사하는 비젼.
    double       dREndVisnTolAng   ; //마스터 상대값 마지막 붙이고 나서 검사하는 비젼.
    double       dRHgtTolZ         ; //높이 보정 하고 높이 단차를 측정하여 확인함.

    double       dCmsAtachGapX     ; //Cmos간의 Attach 간격.. (ex 0.030mm)
    double       dCmsAtachGapY     ; //

    double       dActiveEdgeGapX   ; //2중접합면의 경우 부접합면의 치수 검사를 해서 접합할때 충돌 감지를 하는데 엑티브 에리어 기준으로 가져가는 자제는
    double       dActiveEdgeGapY   ; //자제 엣지면에서 엑티브에리어 까지의 치수를 넣어야 함. 그래야 충돌 감지됌.







    int          iLineHeightCnt    ; // 접합부 높이 측정시에 측정값의 갯수.

    int          iEpoxyDispCntF    ; //실린지 1ea로 사용 가능한 최대 샷
    int          iEpoxyDispCntR    ; //실린지 1ea로 사용 가능한 최대 샷

    bool         bUseSTG_Fix1      ; //스테이지 바인딩 실린더 사용여부.
    bool         bUseSTG_Fix2      ; //스테이지 바인딩 실린더 사용여부.
    bool         bUseSTG_Fix3      ; //스테이지 바인딩 실린더 사용여부.
    bool         bUseSTG_Fix4      ; //스테이지 바인딩 실린더 사용여부.
    bool         bUseSTG_Fix5      ; //스테이지 바인딩 실린더 사용여부.

    double       dDspVacPres       ; //무사시 디스펜서 배큠.(단위 0.01kp)
    double       dDspPrsPres       ; //무사시 디스펜서 정압.(단위 0.1kp)

    bool         bNotUseRotate     ; //C2MP자제는 안돌리고 붙인다.

    int          iUvCntFrst        ; //자연경화 UV 돌리는 횟수
    int          iUvCntScnd        ; //순간경화 UV 돌리는 횟수

    double       dSlaveHtOfsZ      ; //붙일때 슬래이브쪽은 Z축 오프셑을 준다.

    double       dHexaTopHeightOfs ; //가끔 헥사 포트 탑면이 파저 있는 놈들이 있어서....

    int          iHeightMethod     ; //MIN, MAX, AVERAGE
    double       dHeightPitchX     ; //자제 난반사로 인해 여러번 측정하여 가장 낮은 값으로 사용한다.
    double       dHeightPitchY     ; //자제 난반사로 인해 여러번 측정하여 가장 낮은 값으로 사용한다.
    int          iHeightStepCnt    ; //몇번 측정할지.

    bool         bUseSTG_Vac1      ; //스테이지 배큠1 사용
    bool         bUseSTG_Vac2      ; //스테이지 배큠2
    bool         bUseSTG_Vac3      ; //스테이지 배큠3

    bool         bUseSTG_Vac1Frst  ; //스테이지 배큠1 사용
    bool         bUseSTG_Vac2Frst  ; //스테이지 배큠2
    bool         bUseSTG_Vac3Frst  ; //스테이지 배큠3




    double       dBfUVOfsX00       ; //에폭시 경화때 틀어지는 양 보정 오프셑.
    double       dBfUVOfsX10       ; //에폭시 경화때 틀어지는 양 보정 오프셑.
    double       dBfUVOfsX01       ; //에폭시 경화때 틀어지는 양 보정 오프셑.
    double       dBfUVOfsX11       ; //에폭시 경화때 틀어지는 양 보정 오프셑.

    double       dBfUVOfsY00       ; //에폭시 경화때 틀어지는 양 보정 오프셑.
    double       dBfUVOfsY10       ; //에폭시 경화때 틀어지는 양 보정 오프셑.
    double       dBfUVOfsY01       ; //에폭시 경화때 틀어지는 양 보정 오프셑.
    double       dBfUVOfsY11       ; //에폭시 경화때 틀어지는 양 보정 오프셑.

    double       dUVTestSpeed      ; //UV Test 시 이동 속도.
    double       dUVTestDist       ; //UV Test 시 이동 거리.

    double       dMBStdHght        ; //미들블럭 기준 높이.
    double       dMBHghtTol        ; //미들블럭 높이측정 허용오차.
    double       dHexaTopStdHght   ; //헥사포트 탑면 기준 높이.
    double       dHexaTopHghtTol   ; //헥사포트 탑면 높이측정 허용오차.
    double       dCmsHeightOfs     ; //CMOS 측정 모드에서 리포트 디스플레이 하는데 적용하는 오프셑

    bool         bUseFstDisp       ; //디스펜싱 1 사용 유무 체크 (자연경화)
    bool         bUseSecDisp       ; //디스펜싱 2 사용 유무 체크 (순간경화)

    int          iFstDisp          ; //디스펜싱 1에 사용할 툴 (Front/Rear)
    int          iSecDisp          ; //디스펜싱 2에 사용할 툴 (Front/Rear)

    double       dLengthTol        ; //2018.08.08
                                     //CMOS 접합 전에 Vision 검사 한 결과의 길이가
                                     //원래 모터 포지션 길이보다 심하게 크거나 작으면 에러 띄우도록 하기위한 옵션
                                     //모터포지션 길이 - Vision 검사 결과 길이 < dLengthTol ||
                                     //모터포지션 길이 - Vision 검사 결과 길이 > dLengthTol
                                     //NG
    //스테이지 혼용 방지 에러 스킵하도록 옵션 처리 진섭
    bool         bStgMxErrSkip     ;

    EN_DISP_MODE GetDispMode(){
        if( bUseFstDisp && bUseSecDisp) return dmBoth ;
        if( bUseFstDisp &&!bUseSecDisp) return dmFrst ;
        if(!bUseFstDisp && bUseSecDisp) return dmScnd ;
        return dmNotUse ;
    }


} ;


struct CCmnOptn {   //장비 공용 옵션.
    bool    bFLoadingStop     ;
    bool    bRLoadingStop     ;
    bool    bIgnrDoor         ;
    bool    bVisnNgStop       ;

    int     iDisprClnTime     ;
    String  sLotFilePath      ;

    double  dMaxPlaceZOfs1    ;
    double  dMaxPlaceZOfs2    ;
    //double  dMaxAttachForce   ;

    int     iRightVisnDelay   ;
    int     iRightHeightDelay ;
    int     iRightHeightStepDelay;

    int     iHexaPotRelDelay  ;

    bool    bUseConfocal      ; //공초점 센서 사용 유무

    bool    bIgnrCmsREndAlign ; //CMOS Right End Align 사용 유무
    bool    bIgnrCmsREndHeight; //CMOS Right End Height 사용 유무
    bool    bIgnrCmsRFixAlign ; //CMOS Right Fix Align 사용 유무
    bool    bIgnrCmsRFixHeight; //CMOS Right Fix Height 사용 유무
    bool    bIgnrMidREndHeight; //CMOS End Height Check 사용 유무

    int     iTopUVLimitTime   ; //UV 라이프 타임 에러.
    int     iBtmUVLimitTime   ;

    bool    bCheckVisnPos     ; //비전검사후 모션보정전에 한번씩 스탑하면서 물어보는창 띄우기.

    EN_WORK_MODE iWorkMode    ;
} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool   bDebugMode           ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    //29추가
    double dLTL_RTL_Stroke      ; //레프트 라이트 간의 스트록.
    double dLTL_VISN_DISP_Strke ; //라이트 툴에서 비젼과 디스펜서 스트록.
    double dRTL_TRNS_VISN_Strke ; //레프트 툴에서 트렌스퍼와 비젼 스트록.

    bool   bDonUseMasterT       ; //마스터쪽 T값 같이 보정 안함.

    bool   bUseLotInfo          ;

};

//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
    AnsiString sModelName   ;
} ;


//저장 안함.
struct CEqpStat {
    //bool bShowVisnMsg ; //비전검사하고 모션 이동전에 메세지 확인 하는 것.
}   ;


//---------------------------------------------------------------------------
class COptionMan
{
    public:
        COptionMan();
        virtual ~COptionMan();

    protected:
        AnsiString  m_sCrntDev ; //Current open device.


    public:
        void Init();
        void Close();

        CDevInfo   DevInfo  ;
        CDevOptn   DevOptn  ;
        CCmnOptn   CmnOptn  ;
        CMstOptn   MstOptn  ;
        CEqpOptn   EqpOptn  ;
        CEqpStat   EqpStat  ;


        String GetCrntDev () { return m_sCrntDev ; }
        void SetCrntDev (AnsiString _sName);

        void LoadJobFile  (AnsiString _sDevName); void SaveJobFile  (AnsiString _sDevName);

        void LoadDevInfo  (AnsiString _sDevName); void SaveDevInfo  (AnsiString _sDevName);
        void LoadDevOptn  (AnsiString _sDevName); void SaveDevOptn  (AnsiString _sDevName);
        void LoadCmnOptn  (                    ); void SaveCmnOptn  (                    );
        void LoadMstOptn  (                    ); void SaveMstOptn  (                    );
        void LoadEqpOptn  (                    ); void SaveEqpOptn  (                    );
        void LoadLastInfo (                    ); void SaveLastInfo (                    );


};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
