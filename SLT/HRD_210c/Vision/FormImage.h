//---------------------------------------------------------------------------

#ifndef FormImageH
#define FormImageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
#include "FormMag.h"
#include "FormPixel.h"
#include "FormHisto.h"
#include "Form3d.h"
#include "FormCalPara.h"
#include "FormCamLightPara.h"


#include "Vision.h"
#include "Tracker.h"


#include <ComCtrls.hpp>
#include <Menus.hpp>



typedef void (__closure * InspCallbackFunc)(EN_VISN_ID ,bool);
class TFrmImage : public TForm
{
__published:	// IDE-managed Components
    TSavePictureDialog *SavePictureDialog1;
    TOpenPictureDialog *OpenPictureDialog1;
    TTimer *tmUpdate;
    TPanel *pnTopToolbar;
    TSpeedButton *btSave;
    TSpeedButton *btLoad;
    TSpeedButton *btGrab;
    TSpeedButton *btInsp;
    TPanel *pnThreshold;
    TScrollBar *sbThresh;
    TPanel *pnThreash;
    TPanel *pnBtmToolbar;
    TSpeedButton *btRealSize;
    TSpeedButton *btZoomIn;
    TSpeedButton *btZoomOut;
    TSpeedButton *btRealRatio;
    TSpeedButton *btStratch;
    TSpeedButton *btColor;
    TSpeedButton *btFalse;
    TSpeedButton *btBinary;
    TSpeedButton *btGray;
    TSpeedButton *btBlue;
    TSpeedButton *btGreen;
    TSpeedButton *btRed;
    TPanel *pnPixelInfo;
    TPanel *pnColor;
        TSpeedButton *btLive;
    TLabel *lbTime;
        TScrollBox *sbDraw;
        TPanel *pnDraw;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *lbMenu;
        TPopupMenu *puMenu;
        TMenuItem *kye1;
        TMenuItem *kyesdf1;
        TMenuItem *LoadImage1;
        TMenuItem *SaveImage1;
        TMenuItem *Analysis1;
        TMenuItem *PixelForm1;
        TMenuItem *HistoForm1;
        TMenuItem *TeachImageForm1;
        TMenuItem *Camera1;
        TMenuItem *CameraCalibration1;
        TMenuItem *miCntrLine;
        TMenuItem *VisionParameter1;
        TMenuItem *AlghorithmSetting1;
        TLabel *lbDpCnt;
        TLabel *Label3;
    TSpeedButton *btMag;
    TSpeedButton *btHisto;
    TSpeedButton *btPixel;
        TTimer *Timer2;
        TButton *Button1;
        TButton *Button2;
        TMenuItem *CameraLights1;
        TButton *Button3;
        TButton *Button4;
        TSpeedButton *bt3d;
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall pnDrawMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall btGrayClick(TObject *Sender);
    void __fastcall sbThreshChange(TObject *Sender);
    void __fastcall pnDrawMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnDrawMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btRealRatioClick(TObject *Sender);
    void __fastcall btLoadClick(TObject *Sender);
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall btZoomOutClick(TObject *Sender);
    void __fastcall btZoomInClick(TObject *Sender);
    void __fastcall btRealSizeClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btGrabClick(TObject *Sender);
    void __fastcall btInspClick(TObject *Sender);
    void __fastcall btLiveClick(TObject *Sender);
    void __fastcall pcParaChange(TObject *Sender);
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall lbMenuMouseEnter(TObject *Sender);
        void __fastcall lbMenuMouseLeave(TObject *Sender);
        void __fastcall lbMenuMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall CameraCalibration1Click(TObject *Sender);
        void __fastcall miCntrLineClick(TObject *Sender);
        void __fastcall VisionParameter1Click(TObject *Sender);
        void __fastcall AlghorithmSetting1Click(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall CameraLights1Click(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall btMagClick(TObject *Sender);
        void __fastcall btPixelClick(TObject *Sender);
        void __fastcall btHistoClick(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall bt3dClick(TObject *Sender);
        void __fastcall lbTimeClick(TObject *Sender);
private:	// User declarations

    int m_iOriImgWidth  ; //원래 이미지의 넓이와
    int m_iOriImgHeight ; // 높이.

    //fmMag
    void PaintCallback(); //Camera Callback

    //전체 검사 콜백.
    InspCallbackFunc InspCallback ;

    TFrmMag     *FrmMag     ;
    TFrmPixel   *FrmPixel   ;
    TFrmHisto   *FrmHisto   ;
    TFrm3d      *Frm3d      ;

    //세팅 창.
    TFrmCamLightPara *FrmCamLightPara ;
    TFrmCalPara      *FrmCalPara      ;


    int m_iDpCnt ;

    //화면 그리는 것 ....
    TWndMethod OriginalProc;
    void __fastcall PanelProc(Messages::TMessage &msg);

    struct TStat {
        bool bTrckrClick  ; //트렉커 클릭됨.
        bool bImgClick    ; //이미지 이동.
        int  iClickX      ; //트렉커 클릭X
        int  iClickY      ; //트렉커 클릭Y
    }Stat;

    EN_VISN_ID m_iVisnId ;



    //bool m_bChecked ;
    int  m_iPrePage ;

    TRectTracker * RectTracker ;
    TLineTracker * LineTracker ;

    bool m_bPaintLock ;


public:		// User declarations
    __fastcall TFrmImage(TComponent* Owner);

    void SetInspCallbackFunc (InspCallbackFunc _InspCallback);

    bool LoadData(AnsiString _sFilePath);
    bool SaveData(AnsiString _sFilePath);

    void      SetVisnId(EN_VISN_ID _iId) ;
    EN_VISN_ID GetVisnId(void);

    void SetLevel (int _iLevel);

    void SetFormEnable(bool _bTrue);

    void OnPaint();
    void OnPaintT();

    void SetPaintLock(bool _bEnable) { m_bPaintLock = _bEnable ; }
    bool GetPaintLock(             ) { return m_bPaintLock ;}

    void sun();

};



//---------------------------------------------------------------------------
//extern PACKAGE TFrmImage *FrmImage;
//---------------------------------------------------------------------------
#endif
