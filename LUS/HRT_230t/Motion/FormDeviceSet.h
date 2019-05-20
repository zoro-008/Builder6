//---------------------------------------------------------------------------

#ifndef FormDeviceSetH
#define FormDeviceSetH
//---------------------------------------------------------------------------
#include "UserFile.h"
#include "TEdit.h"
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
//#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include <ComCtrls.hpp>
//#include <ExtCtrls.hpp>
//#include <Graphics.hpp>
//#include <Buttons.hpp>
//#include <jpeg.hpp>
//#include <Grids.hpp>
//---------------------------------------------------------------------------
//Motr Window Cylinder Window Frame Unit Refernce
//---------------------------------------------------------------------------
#include "FrameMotr.h"
#include "FrameCyl.h"
//---------------------------------------------------------------------------

#include <ImgList.hpp>
#include <ValEdit.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
class TFrmDeviceSet : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdateTimer;
        TPageControl *pcDeviceSet;
        TTabSheet *tsDeviceInfo;
        TTabSheet *TabSheet12;
        TPanel *Panel14;
        TPanel *Panel15;
        TImage *Image12;
        TLabel *Label57;
        TBitBtn *btSaveDevice;
    TPanel *Panel4;
    TPanel *Panel5;
    TImage *Image5;
    TLabel *Label23;
    TImage *Image2;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TStaticText *lbRowPitch;
    TEdit *edRowPitch;
    TStaticText *lbColPitch;
    TEdit *edColPitch;
    TStaticText *lbRowGroupCount;
    TEdit *edRowGroupCount;
    TStaticText *lbColCount;
    TEdit *edColCount;
    TStaticText *lbRowGroupGap;
    TEdit *edRowGroupGap;
    TStaticText *lbColGroupGap;
    TEdit *edColGroupGap;
    TStaticText *lbRowCount;
    TEdit *edRowCount;
    TStaticText *lbSrColCntrPitch;
    TEdit *edColGroupCount;
    TPanel *Panel6;
    TPanel *Panel8;
    TImage *Image7;
    TLabel *Label28;
    TStaticText *StaticText5;
    TEdit *edWf_BldOfs;
    TStaticText *StaticText7;
    TEdit *edWf_Diameter;
    TStaticText *StaticText9;
    TEdit *edWf_Width;
    TStaticText *StaticText12;
    TEdit *edWf_Height;
    TImage *Image4;
    TPanel *Panel9;
    TPanel *Panel10;
    TImage *Image8;
    TLabel *Label24;
    TImage *Image3;
    TStaticText *StaticText8;
    TEdit *edMgzSlotCnt;
    TStaticText *StaticText10;
    TEdit *edMgzSlotPitch;
    TPanel *Panel11;
    TImage *Image6;
    TPanel *Panel12;
    TImage *Image9;
    TLabel *Label25;
    TStaticText *StaticText3;
    TEdit *edCsSlotCnt;
    TStaticText *StaticText4;
    TEdit *edCsSlotPitch;
    TTabSheet *TabSheet3;
    TPanel *Panel20;
    TPageControl *pcEPX_MT;
    TTabSheet *TabSheet9;
    TGroupBox *GroupBox21;
        TPanel *pnRAL_XIdx;
    TGroupBox *GroupBox23;
        TPanel *pnWRK_ZPrb;
    TGroupBox *GroupBox25;
        TPanel *pnSRT_TSrt;
    TPanel *Panel57;
    TPanel *Panel58;
    TImage *Image14;
    TLabel *Label30;
        TPanel *pnRAL_XIdxp;
    TPanel *Panel60;
    TPanel *Panel61;
    TImage *Image15;
    TLabel *Label32;
        TPanel *pnWRK_ZPrbp;
    TPanel *Panel66;
    TPanel *Panel67;
    TImage *Image17;
    TLabel *Label34;
        TPanel *pnSRT_TSrtp;
    TPanel *Panel34;
    TPanel *Panel35;
    TImage *Image37;
    TLabel *Label55;
        TLabel *Label65;
        TLabel *Label66;
        TEdit *edBin1FullCnt;
        TLabel *Label69;
        TLabel *Label70;
        TEdit *edBin2FullCnt;
        TPanel *Panel27;
        TPanel *Panel28;
        TImage *Image42;
        TLabel *Label49;
        TLabel *Label71;
        TLabel *Label72;
        TEdit *edNoPkgSec;
        TPanel *Panel38;
        TPanel *Panel43;
        TImage *Image43;
        TLabel *Label3;
        TPanel *Panel44;
        TPanel *Panel68;
        TImage *Image44;
        TLabel *Label6;
        TLabel *Label73;
        TLabel *Label74;
        TEdit *edFailCnt;
    TPanel *Panel98;
    TLabel *Label111;
    TLabel *Label112;
    TLabel *Label113;
    TLabel *Label114;
    TPanel *Panel101;
    TImage *Image49;
    TLabel *Label115;
        TEdit *edTempSet1;
        TEdit *edTempGap;
        TRadioGroup *rgJogUnit1;
        TEdit *edJogUnit1;
    TStaticText *StaticText20;
    TComboBox *cbMapType;
    TStaticText *StaticText1;
    TEdit *edWRowCnt;
    TStaticText *StaticText19;
    TEdit *edWColCnt;
    TStaticText *StaticText21;
    TEdit *edGoodStr1;
    TRadioGroup *rgFlatAngle;
    TStaticText *StaticText22;
    TEdit *edWXOffset;
    TStaticText *StaticText23;
    TEdit *edWYOffset;
    TGroupBox *GroupBox61;
        TPanel *pnRAL_IdxUpDn;
    TPanel *Panel3;
        TBitBtn *btIO1;
    TStaticText *StaticText2;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *edBin3FullCnt;
        TEdit *edBin4FullCnt;
        TLabel *Label7;
        TLabel *Label8;
        TEdit *edBin5FullCnt;
        TLabel *Label15;
        TLabel *Label16;
        TEdit *edPrbStartDelay;
        TLabel *Label17;
        TLabel *Label18;
        TEdit *edPinChangeCnt;
        TLabel *Label21;
        TLabel *Label22;
        TEdit *edTempSet2;
        TTimer *tmHeat;
        TLabel *Label26;
        TLabel *lbPinCnt;
        TBitBtn *btPinCntReset;
        TPanel *Panel1;
        TBitBtn *btTestInsp;
        TStaticText *StaticText6;
    TLabel *Label27;
    TEdit *edPrbContactTime;
    TLabel *Label29;
    TPanel *Panel2;
    TPanel *Panel7;
    TImage *Image1;
    TLabel *Label44;
    TLabel *Label36;
    TLabel *Label37;
    TEdit *edEpmtyBin;
    TLabel *Label9;
    TLabel *Label14;
    TEdit *edBin3Deley;
        TLabel *Label42;
        TLabel *Label43;
        TEdit *edStartHeatSec;
        TLabel *Label45;
        TLabel *Label46;
        TEdit *edStopTemp;
        TLabel *Label47;
        TLabel *Label48;
        TEdit *edStopWaitSec;
        TLabel *Label77;
        TLabel *Label78;
        TEdit *edBlowerDelay;
        TCheckBox *cbSortPKGSensor;
        TLabel *Label38;
        TLabel *Label39;
        TEdit *edSortDeley;
        TLabel *Label59;
        TEdit *edFeedWaitTime;
        TLabel *Label60;
        TLabel *Label31;
        TLabel *Label35;
        TEdit *edFedFailCnt;
        TLabel *Label51;
        TLabel *Label61;
        TEdit *edPKGHeatSec;
        TLabel *Label62;
        TGroupBox *GroupBox1;
        TPanel *pnFED_StpFwBw;
        TLabel *Label33;
        TEdit *edPkgDetect;
        TLabel *Label63;
        TLabel *Label67;
        TEdit *edIdleTime;
        TLabel *Label64;
        TPanel *Panel13;
        TPanel *Panel16;
        TImage *Image10;
        TLabel *Label19;
        TPanel *pnETC_Sp;
        TGroupBox *GroupBox20;
        TPanel *pnFED_TFed;
    void __fastcall btSavePosClick(TObject *Sender);
    void __fastcall tmUpdateTimerTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall pcDeviceSetChange(TObject *Sender);
        void __fastcall rgJogUnit1Click(TObject *Sender);
        void __fastcall edJogUnitChange(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btIO1Click(TObject *Sender);
        void __fastcall btPinCntResetClick(TObject *Sender);
        void __fastcall btTestInspClick(TObject *Sender);
private:	// User declarations
    //Active Radio Button Page.
    TRadioGroup  * rgUnitUser ;
    TEdit        * edUnitUser ;


    ///ㅆ;ㅣ비,ㅜ리ㅏ루라ㅣ라 리그립
    int m_iPRBRgCol[MAX_REGRIP_ARRAY];
    int m_iPRBRgDis[MAX_REGRIP_ARRAY];

    int m_iWRERgCol[MAX_REGRIP_ARRAY];
    int m_iWRERgDis[MAX_REGRIP_ARRAY];

    int m_iWRDRgCol[MAX_REGRIP_ARRAY];
    int m_iWRDRgDis[MAX_REGRIP_ARRAY];

    int m_iPSBRgCol[MAX_REGRIP_ARRAY];
    int m_iPSBRgDis[MAX_REGRIP_ARRAY];

    void __fastcall JobFileBackUp();

public:		// User declarations
    TFraMotr *FraMotor   [MAX_MOTR];
    TFraCyl  *FraCylinder[MAX_ACTR];

    __fastcall TFrmDeviceSet(TComponent* Owner);

    void __fastcall MtWindowSet(); //Frame Motr set
    void __fastcall AtWindowSet(); //Frame Syl  set
    void __fastcall MtWindowSetPitch();

    void __fastcall UpdateDevInfo(bool bTo);
    void __fastcall UpdateDevOptn(bool bTo);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDeviceSet *FrmDeviceSet;
//---------------------------------------------------------------------------
#endif
