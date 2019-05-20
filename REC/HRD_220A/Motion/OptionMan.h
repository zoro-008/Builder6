//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

#include "UtilDefine.h"

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
enum EN_WORK_MODE { wmFos = 0 , wmDie = 1};
struct CDevInfo {   //device에 대한 Dev_Info
    EN_WORK_MODE iWorkMode        ;

    double       dMgzSlotPitch    ; //Loader Tray
    int          iMgzSlotCnt      ;
    double       dMgzTopToFrstSlot; //현제 21.0mm 삭제.  스타트 포지션 2개로 감.
    double       dMgzBtmToLastSlot; //현제 13.0mm 삭제.
    int          iBoatColCnt      ; // 10개.
    double       dBoatColPitch    ; // 41.6mm
    double       dFosWidth        ;
    double       dFosHeight       ;
    double       dFosChamper      ;
    double       dHicWidth        ;
    double       dHicHeight       ;
    double       dHicChamper      ;
    double       dDieWidth        ;
    double       dDieHeight       ;
    double       dDieChamper      ;
} ;
                 
//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    bool         bStIgnrVisnBefore ;
    bool         bStIgnrVisnAfter  ;
    bool         bPkIgnrAlign      ;
    bool         bPkIgnrVisnAlign  ;
    bool         bPkIgnrVisnDefect ;

    int          iPkPickDelay      ; //Pick Delay.
    int          iFrstPkgDelay     ; //리어 첫자제는 들어서 딜레이가 있어야 한다.

    double       dPlaceYOfs        ; //자제를 정중앙에 내려 놓는것이 아니고 Y축으로 오프셑이 있다.
    double       dPlaceXOfs        ; //X오프셑이 있다.

    //이놈들이 넘어가면 보정하면 기구 처박을 염려 있음.
    double       dStageTolXY       ; //Stage 검사시에 검사 허용 치수.
    double       dStageTolAng      ; //각도.
    double       dPickerTolXY      ; // 검사시에 검사 허용 치수.
    double       dPickerTolAng     ; //각도.
    double       dPlaceTolXY       ; //After 검사시에 검사 허용 치수.
    double       dPlaceTolAng      ; //각도.

    double       dFosPlaceSttYOfs  ; //Fos의 경우 내릴때 Y축의 오프셑을 가지고 내리고 디스펜싱후에 최종 Place위치로 가져간다.
    double       dFosPlaceSttYSpd  ; //Fos의 경우 내릴때 Y축의 오프셑을 가지고 내리고 디스펜싱후에 최종 Place위치로 가져간다. 속도.
    int          iFosPlaceBfDelay  ; //Fos의 용액의 자연확산을 위해 노젓기 끝나고 기다린다.

    double       dPlaceZOfs1       ; //FOS의 경우 Dispensing시에 약간떠서 액을 주입한다. 그 오프셑이고.  플레이스시에 1포지션 오프셑 지점까지 갔다가.
    double       dPlaceZOfs2       ; // 2포지션 오프셑 지점 까지 가고 그후 Attach까지 간다.
    double       dAttachSpeed1     ; //Z축 dFosPlaceZOfs1~dFosPlaceZOfs2구간.
    double       dAttachSpeed2     ; //Z축 dFosPlaceZOfs2~Attach구간.
    double       dAttachForce      ; //합착 무게.g단위.
    double       dAttachForceOfs   ; //합착 무게.g단위. 오프셑.

    double       dDispZOfs         ; //디스펜싱 높이 오프셑.
    int          iPkPushDelay      ;

    double       dAttachStageOfsX  ; //합착 할때에 스테이지 이동 오프셑...
    double       dAttachStageOfsY  ; // 평탄도 세팅을 아무리 해도 어느정도의 오프셑이 발생 되므로 넣는다.
    double       dAttachStageOfsT  ; // 평탄도 세팅을 아무리 해도 어느정도의 오프셑이 발생 되므로 넣는다.

    int          iRailBtmTemp      ;
    int          iRailTopTemp      ;
    int          iStageTemp        ;

    //int          iDspVacPres       ; //무사시 디스펜서 배큠.(단위 0.01kp)
    //int          iDspPrsPres       ; //무사시 디스펜서 정압.(단위 0.1kp)

    int          iDspCh            ; //디스펜서 사용 채널.
    int          iDspMinAmount     ; //디스펜서 잔양 에러 퍼센트...(20 이면 20이하남았을때 에러.)
    double       dDspVacPres       ; //무사시 디스펜서 배큠.(단위 0.01kp)
    double       dDspPrsPres       ; //무사시 디스펜서 정압.(단위 0.1kp)


    double       dReptSttXPos      ;
    double       dReptSttYPos      ;
    double       dReptEndXPos      ;
    double       dReptEndYPos      ;
    double       dReptSpd          ;
    int          iReptCnt          ;

    AnsiString   sEpoxyName        ;

} ;


struct CCmnOptn {   //장비 공용 옵션.
    bool    bLoadingStop      ;
    bool    bIgnrDoor         ;

    int     iDisprClnCnt      ;
    bool    bVisnNgStop       ;

    bool    bTempIgnrErrorChk ;
    double  dTempErrorTol     ;

    String  sLotFilePath      ;

    double  dMaxPlaceZOfs1    ;
    double  dMaxPlaceZOfs2    ;
    double  dMaxAttachForce   ;
} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool   bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.

    double dPckDspStroke      ; //Picker 와 Dispensor간의 Stroke ;

    bool   bSkipPickrDispr    ; //Picker 와 Dispensor는 스킵.

    double dPckToStgAngleOfs  ; //스테이지와 픽커간의 앵글 오프셑.
};
//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
    AnsiString sModelName   ;
} ;


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
