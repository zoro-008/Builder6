//---------------------------------------------------------------------------

#ifndef FormDeviceSetH
#define FormDeviceSetH
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "TEdit.h"
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Buttons.hpp>
#include <jpeg.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
//Motr Window Cylinder Window Frame Unit Refernce
//---------------------------------------------------------------------------
#include "FrameMotr.h"
#include "FrameCyl.h"
//---------------------------------------------------------------------------
//#include "Timer.h"
#include "UtilDefine.h"
#include <Dialogs.hpp>

//#include <ImgList.hpp>
//#include "LabelS.h"
//#include <ValEdit.hpp>
class TFrmDeviceSet : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdateTimer;
        TPanel *pnVisnSet;
        TPageControl *pcDeviceSet;
        TTabSheet *tsDeviceInfo;
        TPanel *Panel6;
        TImage *Image4;
        TPanel *Panel8;
        TStaticText *StaticText9;
        TEdit *edWfPitchX;
        TStaticText *StaticText12;
        TEdit *edWfPitchY;
        TStaticText *StaticText1;
        TEdit *edWfRowCnt;
        TStaticText *StaticText19;
        TEdit *edWfColCnt;
        TPanel *Panel11;
        TImage *Image6;
        TPanel *Panel12;
        TStaticText *StaticText3;
        TEdit *edCsSlotCnt;
        TStaticText *StaticText4;
        TEdit *edCsSlotPitch;
        TTabSheet *TabSheet4;
        TPageControl *PageControl1;
        TTabSheet *TabSheet17;
        TPanel *Panel69;
        TPanel *Panel70;
        TPanel *pnHED_XHedPs;
        TPanel *Panel72;
        TPanel *Panel73;
        TPanel *pnHED_YHedPs;
        TPanel *Panel81;
        TPanel *Panel82;
        TPanel *pnHED_TZomPs;
        TPanel *Panel53;
        TPanel *Panel56;
        TPanel *pnHED_ZVsnPs;
        TPanel *Panel37;
        TPanel *Panel62;
        TImage *Image38;
        TLabel *Label56;
        TGroupBox *GroupBox63;
        TPanel *pnSTG_Chk;
        TGroupBox *GroupBox49;
        TPanel *pnHED_Pen;
        TGroupBox *GroupBox72;
        TPanel *pnTRS_Cmp;
        TTabSheet *TabSheet5;
        TPageControl *PageControl2;
        TTabSheet *TabSheet11;
        TPanel *Panel30;
        TPageControl *pcSTG_MT;
        TTabSheet *STG_MOTOR2;
        TGroupBox *GroupBox30;
        TPanel *pnSTG_ZCst;
        TGroupBox *GroupBox31;
        TPanel *pnSTG_XTrs;
        TGroupBox *GroupBox32;
        TPanel *pnSTG_TStg;
        TTabSheet *HED_MOTOR2;
        TGroupBox *GroupBox7;
        TPanel *pnHED_XHed2;
        TGroupBox *GroupBox8;
        TPanel *pnHED_YHed2;
        TGroupBox *GroupBox10;
        TPanel *pnHED_ZVsn2;
        TGroupBox *GroupBox11;
        TPanel *pnHED_TZom2;
        TRadioGroup *rgJogUnit2;
        TEdit *edJogUnit2;
        TPanel *Panel84;
        TPanel *Panel85;
        TPanel *pnSTG_TStgPs;
        TPanel *Panel105;
        TPanel *Panel106;
        TPanel *pnSTG_ZCstPs;
        TPanel *Panel108;
        TPanel *Panel109;
        TPanel *pnSTG_XTrsPs;
        TPanel *Panel1;
        TPanel *Panel2;
        TImage *Image1;
        TLabel *Label2;
        TGroupBox *GroupBox2;
        TPanel *pnSTG_Chk2;
        TGroupBox *GroupBox3;
        TPanel *pnHED_Pen2;
        TGroupBox *GroupBox4;
        TPanel *pnTRS_Cmp2;
        TTabSheet *OPTION;
        TPanel *Panel7;
        TBitBtn *btWfr_Vac;
        TPanel *Panel4;
        TBitBtn *btWfr_Vac2;
        TStaticText *StaticText2;
        TStaticText *StaticText6;
        TBitBtn *btSaveDevice;
        TPanel *Panel3;
        TImage *Image2;
        TPanel *Panel5;
        TStaticText *StaticText11;
        TEdit *edBdXPch;
        TStaticText *StaticText13;
        TEdit *edBdYPch;
        TStaticText *StaticText15;
        TEdit *edBdYCnt;
        TStaticText *StaticText16;
        TEdit *edBdXCnt;
        TLabel *Label12;
        TLabel *Label13;
        TPanel *Panel25;
        TPageControl *pcHED_MT;
        TTabSheet *HED_MOTOR1;
        TGroupBox *GroupBox1;
        TPanel *pnHED_XHed;
        TGroupBox *GroupBox5;
        TPanel *pnHED_YHed;
        TGroupBox *GroupBox6;
        TPanel *pnHED_ZVsn;
        TGroupBox *GroupBox9;
        TPanel *pnHED_TZom;
        TTabSheet *STG_MOTOR1;
        TGroupBox *GroupBox12;
        TPanel *pnSTG_ZCst2;
        TGroupBox *GroupBox13;
        TPanel *pnSTG_XTrs2;
        TGroupBox *GroupBox14;
        TPanel *pnSTG_TStg2;
        TRadioGroup *rgJogUnit1;
        TEdit *edJogUnit1;
        TPanel *Panel9;
        TPanel *pnMSK;
        TPanel *Panel10;
        TPanel *Panel13;
        TLabel *Label5;
        TPanel *Panel14;
        TComboBox *cbOriMathod;
        TPanel *Panel104;
        TPanel *Panel113;
        TEdit *edVisnChkChipColCnt;
        TEdit *edPenCheckCnt;
        TEdit *edVisnChkChipRowCnt;
        TOpenDialog *odMap;
        TGroupBox *GroupBox16;
        TPanel *pnSTG_Alg;
        TGroupBox *GroupBox17;
        TPanel *pnSTG_Alg2;
        TGroupBox *pnCov;
        TPanel *pnHED_Cov;
        TGroupBox *GroupBox19;
        TPanel *pnHED_Cov2;
    TSaveDialog *sdMap;
    TGroupBox *GroupBox18;
    TStaticText *StaticText20;
    TStaticText *StaticText18;
    TImage *Image3;
    TImage *Image5;
    TLabel *Label1;
    TLabel *Label6;
    TGroupBox *GroupBox20;
    TImage *Image7;
    TImage *Image8;
    TLabel *Label8;
    TLabel *Label7;
    TGroupBox *GroupBox22;
    TImage *Image11;
    TImage *Image12;
    TLabel *Label11;
    TLabel *Label14;
    TGroupBox *GroupBox21;
    TImage *Image9;
    TImage *Image10;
    TLabel *Label9;
    TLabel *Label10;
    TGroupBox *GroupBox24;
    TImage *Image15;
    TImage *Image16;
    TLabel *Label16;
    TLabel *Label18;
    TGroupBox *GroupBox25;
    TImage *Image17;
    TImage *Image18;
    TLabel *Label19;
    TLabel *Label20;
    TGroupBox *GroupBox23;
    TImage *Image13;
    TImage *Image14;
    TLabel *Label15;
    TLabel *Label17;
        TTabSheet *VISION;
        TPanel *pnVision;
        TPanel *Panel16;
        TPanel *Panel17;
        TStaticText *stFstChipCol;
        TEdit *ediWFrstCol;
        TStaticText *stFstChipRow;
        TEdit *ediWFrstRow;
        TRadioGroup *rgFlatAngle;
        TStaticText *stMAPType;
        TComboBox *cbMapType;
    TLabel *Label22;
    TLabel *Label28;
    TSpeedButton *sbMapType;
    TSpeedButton *sbFrstRow;
    TSpeedButton *sbFrstCol;
    TSpeedButton *sbFlatAngle;
    TLabel *Label124;
    TLabel *Label4;
    TLabel *Label120;
        TLabel *Label3;
        TComboBox *cbAutoOrigin;
    TStaticText *stMskLine;
    TEdit *edMskLine;
    TGroupBox *GroupBox15;
    TPanel *pnVisnPara;
    TPanel *pnAlgoPara;
    TSpeedButton *sbVisnChkChipColCnt;
    TSpeedButton *sbVisnChkChipRowCnt;
    TSpeedButton *sbPenCheckCnt;
    TSpeedButton *sbOriMathod;
    TSpeedButton *sbAutoOrigin;
    TSpeedButton *sbMskLine;
        TPanel *Panel18;
        TRadioGroup *rgJogUnitVisn;
        TGroupBox *GroupBox28;
        TBitBtn *btHedY_Bwd;
        TBitBtn *btMotorStop;
        TBitBtn *btHedX_Rgh;
        TBitBtn *btStgT_Cw;
        TBitBtn *btHedY_Fwd;
        TBitBtn *btStgT_Ccw;
        TBitBtn *btHedX_Lft;
        TGroupBox *gbZoomT;
        TBitBtn *btZoom_Ccw;
        TBitBtn *btZoom_Cw;
        TGroupBox *gbVisnZ;
        TBitBtn *btHedZ_Up;
        TBitBtn *btHedZ_Dn;
        TBitBtn *BitBtn1;
        TEdit *edJogUnitVisn;
        TBitBtn *btMoveToStgHwCenter;
        TBitBtn *btMoveToVisnPos;
        TBitBtn *btVisnPosSave;
    TBitBtn *btVisnZMove;
    TBitBtn *btVisnTMove;
        TBitBtn *btVisnManual;
    TBitBtn *btVsnZTSave;
        TStaticText *StaticText5;
        TEdit *edExtention;
    TButton *btMakeMask;
    TCheckBox *cbUseHex;
    TButton *btHexConvert;
    TGroupBox *GroupBox26;
    TSpeedButton *sbMaskStr;
    TSpeedButton *sbEmptyStr;
    TSpeedButton *sbGoodStr;
    TLabel *lbSpace;
    TCheckBox *cbMskExist;
    TStaticText *stMarkChr;
    TStaticText *stEmptyChr;
    TEdit *edMaskStr1;
    TEdit *edEmptStr1;
    TStaticText *stGoodChr;
    TEdit *edGoodStr1;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall rgJogUnitClick(TObject *Sender);
        void __fastcall edJogUnitChange(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btWfr_VacClick(TObject *Sender);
        void __fastcall btMakeMaskClick(TObject *Sender);
        void __fastcall cbMapTypeChange(TObject *Sender);
    void __fastcall brMapSaveAsClick(TObject *Sender);

    void __fastcall btJogNMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btJogNMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall btJogPMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btJogPMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btMotorStopClick(TObject *Sender);
        void __fastcall rgJogUnitVisnClick(TObject *Sender);
    void __fastcall sbMaskStrClick(TObject *Sender);
        void __fastcall btMoveToStgHwCenterClick(TObject *Sender);
        void __fastcall btMoveToVisnPosClick(TObject *Sender);
        void __fastcall btVisnPosSaveClick(TObject *Sender);
        void __fastcall btVisnZMoveClick(TObject *Sender);
        void __fastcall btVisnManualClick(TObject *Sender);
    void __fastcall btVsnZTSaveClick(TObject *Sender);
    void __fastcall btHexConvertClick(TObject *Sender);






private:	// User declarations
    //Active Radio Button Page.
    TRadioGroup  * rgUnitUser ;
    TEdit        * edUnitUser ;


    double m_dUnit    ;
    double m_dPitch   ;
    int    m_iUnitType;


//    CDelayTimer    m_tmProbeTest ;


/*    ///ㅆ;ㅣ비,ㅜ리ㅏ루라ㅣ라 리그립
    int m_iPRBRgCol[MAX_REGRIP_ARRAY];
    int m_iPRBRgDis[MAX_REGRIP_ARRAY];

    int m_iWRERgCol[MAX_REGRIP_ARRAY];
    int m_iWRERgDis[MAX_REGRIP_ARRAY];

    int m_iWRDRgCol[MAX_REGRIP_ARRAY];
    int m_iWRDRgDis[MAX_REGRIP_ARRAY];

    int m_iPSBRgCol[MAX_REGRIP_ARRAY];
    int m_iPSBRgDis[MAX_REGRIP_ARRAY];
*/
    void SetUnitInit();
    void SetUnit(EN_UNIT_TYPE _iUnitType , double _dUnit);

    void __fastcall JobFileBackUp();
    void __fastcall MtWindowSetPitch();


public:		// User declarations
    TFraMotr *FraMotor   [MAX_MOTR];
    TFraCyl  *FraCylinder[MAX_ACTR];

    __fastcall TFrmDeviceSet(TComponent* Owner);


    void __fastcall MtWindowSet(); //Frame Motr set
    void __fastcall AtWindowSet(); //Frame Syl  set

    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);

    bool m_bFlatAngle;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
