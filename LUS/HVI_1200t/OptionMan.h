//---------------------------------------------------------------------------
#ifndef OptionManH
#define OptionManH

//---------------------------------------------------------------------------

//enum EN_LEVEL { lvOperator = 0, lvEngineer , lvMaster };

//Device Informations.
struct CDevInfo {   //device에 대한 Dev_Info
    double dColPitch  ;
    double dRowPitch  ;
    int    iColGrCnt  ;
    int    iRowGrCnt  ;
    double dColGrGap  ;
    double dRowGrGap  ;
    int    iColCnt    ;
    int    iRowCnt    ;
    int    iCsSlCnt   ;
    double dCsSlPitch ;

    //Vision
    String sVisnIndexId ;
    int    iColInspCnt  ;
    int    iRowInspCnt  ;

    int    iRowSubGroupCount ;
    double dRowSubGroupGap   ;

    int    iColSubGroupCount ;
    double dColSubGroupGap   ;


} ;

//Device Option.
struct CDevOptn {   //Device에 따라 바뀌는 옵션.
    int    iLotEndMgzCnt     ;
    bool   bDWidth           ; //긴자제..

    int    iPunchingDelay    ;

} ;

//Common Option.
struct CCmnOptn {   //장비 공용 옵션.
    bool   bCam1Skip         ; //카메라 1번 고장 났을때 잡파일.
    bool   bCam2Skip         ; //카메라 2번 고장 났을때 잡파일.

    bool   bTrm1Skip         ;
    bool   bTrmAllSkip       ;
    bool   bLoadingStop      ;
    bool   bVs1_1Skip        ;
    bool   bVs2_1Skip        ;
    bool   bVs3_1Skip        ;
    bool   bVs1_2Skip        ;
    bool   bVs2_2Skip        ;
    bool   bVs3_2Skip        ;
    bool   bVsTrmSkip        ;
    bool   bDoorSkip         ;     
    bool   bVsSndSkip        ;     
    bool   bAutoVsChange     ;
    int    iPRBStprDnDelay   ;
    int    iWK1StprDnDelay   ;
    int    iWK2StprDnDelay   ;
    bool   bEmptyNoTrm       ;
    int    iPSBSortMoveDelay ;
    bool   bUseTrimMachine   ;
    int    iCheckFailCnt     ;
    bool   bIdleRun          ;
    bool   bUsePenMrk        ;

//Option
    int    iLDRChangeCnt     ;
    int    iUldOutDelay      ; 
    bool   bDWidth           ;
//    bool   bUsePenMrkMvDn    ;
//    double dPenMrkMoveDnLen  ;

//    bool    bUsePenMrkMvX    ;
//    bool    bUsePenMrkMvY    ;
//    double  dPenMrkMvX       ;
//    double  dPenMrkMvY       ;

    bool    bEmptyOnlyTrm    ;
    bool    b1stBMOnlyTrm    ;


    int    iRsltLevel0       ; int iCheckFailCnt0; String sRsltName0; int iSort0; int iRsltColor0;
    int    iRsltLevel1       ; int iCheckFailCnt1; String sRsltName1; int iSort1; int iRsltColor1;
    int    iRsltLevel2       ; int iCheckFailCnt2; String sRsltName2; int iSort2; int iRsltColor2;
    int    iRsltLevel3       ; int iCheckFailCnt3; String sRsltName3; int iSort3; int iRsltColor3;
    int    iRsltLevel4       ; int iCheckFailCnt4; String sRsltName4; int iSort4; int iRsltColor4;
    int    iRsltLevel5       ; int iCheckFailCnt5; String sRsltName5; int iSort5; int iRsltColor5;
    int    iRsltLevel6       ; int iCheckFailCnt6; String sRsltName6; int iSort6; int iRsltColor6;
    int    iRsltLevel7       ; int iCheckFailCnt7; String sRsltName7; int iSort7; int iRsltColor7;
    int    iRsltLevel8       ; int iCheckFailCnt8; String sRsltName8; int iSort8; int iRsltColor8;
    int    iRsltLevel9       ; int iCheckFailCnt9; String sRsltName9; int iSort9; int iRsltColor9;
    int    iRsltLevelA       ; int iCheckFailCntA; String sRsltNameA; int iSortA; int iRsltColorA;
    int    iRsltLevelB       ; int iCheckFailCntB; String sRsltNameB; int iSortB; int iRsltColorB;
    int    iRsltLevelC       ; int iCheckFailCntC; String sRsltNameC; int iSortC; int iRsltColorC;
    int    iRsltLevelD       ; int iCheckFailCntD; String sRsltNameD; int iSortD; int iRsltColorD;
    int    iRsltLevelE       ; int iCheckFailCntE; String sRsltNameE; int iSortE; int iRsltColorE;
    int    iRsltLevelF       ; int iCheckFailCntF; String sRsltNameF; int iSortF; int iRsltColorF;
    int    iRsltLevelG       ; int iCheckFailCntG; String sRsltNameG; int iSortG; int iRsltColorG;
    int    iRsltLevelH       ; int iCheckFailCntH; String sRsltNameH; int iSortH; int iRsltColorH;
    int    iRsltLevelI       ; int iCheckFailCntI; String sRsltNameI; int iSortI; int iRsltColorI;
    int    iRsltLevelJ       ; int iCheckFailCntJ; String sRsltNameJ; int iSortJ; int iRsltColorJ;
    int    iRsltLevelK       ; int iCheckFailCntK; String sRsltNameK; int iSortK; int iRsltColorK;
    int    iRsltLevelL       ; int iCheckFailCntL; String sRsltNameL; int iSortL; int iRsltColorL;

    int    iRsltLevel22      ; int iCheckFailCnt22; String sRsltName22; int iSort22; int iRsltColor22;
    int    iRsltLevel24      ; int iCheckFailCnt24; String sRsltName24; int iSort24; int iRsltColor24;
    int    iRsltLevel25      ; int iCheckFailCnt25; String sRsltName25; int iSort25; int iRsltColor25;
    int    iRsltLevel26      ; int iCheckFailCnt26; String sRsltName26; int iSort26; int iRsltColor26;
    int    iRsltLevel27      ; int iCheckFailCnt27; String sRsltName27; int iSort27; int iRsltColor27;    

    bool bAutoOperation ;
    bool bVisionAllSkip ;
} ;

//Master Option.
struct CMstOptn {   //마스타 옵션. 화면상에 있음 FrmMain 디바이스 라벨 더블 클릭.
    bool       bDebugMode         ; //true = 로그기능강화, 디버깅시 타임아웃무시.
    bool       bUld_YHome         ;
    bool       bLdr_YHome         ;
    bool       bDataAccEng        ;
    String     sVisnPath          ;
    double     dTrigerOffset      ;
    bool       bPsbVsErrIgnr      ;
    bool       bDryRun            ;
    bool       bEmptyNoTrm        ;

    String     sPstEquipPath      ;
    String     sPreEquipPath      ;

    bool       bUseRamVsData      ;
    bool       bUseLotAuto        ;

    bool       bUseZMarkingMT     ;
} ;

//Eqipment Option.
struct CEqpOptn {   //모델별 하드웨어 옵션.  화면상에 없고 텍스트 파일에 있음.
//  int        iModelName      ; //모델별 소프트 웨어 옵션을 위해 이용.  하드웨어 다른점으로 이용하면 안됨.   TModelNo
    AnsiString sModelName      ; //sModelName 모델 이름 디스플레이용 이다.
    bool       bExistLoader   ;
    bool       bExistUnLoader ;
} ;


//---------------------------------------------------------------------------
class COptionMan
{
    public:
        COptionMan();
        virtual ~COptionMan();

    public:
        CDevInfo   DevInfo  ;
        CDevOptn   DevOptn  ;
        CCmnOptn   CmnOptn  ;
        CMstOptn   MstOptn  ;
        CEqpOptn   EqpOptn  ;

        AnsiString  m_sCrntDev ; //Current open device.

        int    iTotalChip     ;
        int    iTotalFailChip ;

        AnsiString GetCrntDev () { return m_sCrntDev ; }

        void SetCrntDev (AnsiString _sName);
        void Init(void);
        void LoadJobFile  (AnsiString _sDevName); void SaveJobFile  (AnsiString _sDevName);

        void LoadDevInfo  (AnsiString _sDevName); void SaveDevInfo  (AnsiString _sDevName);
        void LoadDevOptn  (AnsiString _sDevName); void SaveDevOptn  (AnsiString _sDevName);
        void LoadCmnOptn  (                    ); void SaveCmnOptn  (                    );
        void LoadMstOptn  (                    ); void SaveMstOptn  (                    );
        void LoadEqpOptn  (                    ); void SaveEqpOptn  (                    );
        void LoadLastInfo (                    ); void SaveLastInfo (                    );

        void DMSetMaxColRow(bool _bClear = false); //여기서 이거나좀 해주라.dd 쓰는애들이 많네.dd

};

//---------------------------------------------------------------------------
extern COptionMan OM;

//---------------------------------------------------------------------------
#endif
