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
#include "FormCamLightPara.h"

#include "VisionBase.h"
#include "Tracker.h"


#include <ComCtrls.hpp>
#include <Menus.hpp>



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
        TMenuItem *miCntrLine;
        TMenuItem *AlgorithmParameter1;
    TSpeedButton *btMag;
    TSpeedButton *btHisto;
    TSpeedButton *btPixel;
        TMenuItem *CameraLights1;
        TSpeedButton *btCenter;
        TSpeedButton *btNoCenter;
        TPanel *pnInspTime;
        TPanel *pnDisp;
        TPanel *pnGrabTime;
        TPanel *pnImgCpyTime;
        TLabel *lbX;
        TLabel *lbY;
        TLabel *lbHeight;
        TLabel *lbWidth;
        TPanel *pnBase;
        TPanel *pnDraw;
        TSpeedButton *SpeedButton1;
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
        void __fastcall miCntrLineClick(TObject *Sender);
        void __fastcall AlgorithmParameter1Click(TObject *Sender);
        void __fastcall CameraLights1Click(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall btMagClick(TObject *Sender);
        void __fastcall btPixelClick(TObject *Sender);
        void __fastcall btHistoClick(TObject *Sender);
        void __fastcall btCenterClick(TObject *Sender);
        void __fastcall pnDrawMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations

    //fmMag
    void PaintCallback(); //Camera Callback

    TFrmMag     *FrmMag     ;
    TFrmPixel   *FrmPixel   ;
    TFrmHisto   *FrmHisto   ;

    //세팅 창.
    //TFrmCamLightPara *FrmCamLightPara ;

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

    //EN_VISN_ID m_iVisnId ;

    TRectTracker * RectTracker ;
    TLineTracker * LineTracker ;

    float GetScaleX();
    float GetScaleY();

    float m_fDispImgX ; //확대시 이미지 시작 위치.
    float m_fDispImgY ;

    float m_fDispScaleX ; //화면 디스플레이 스케일.
    float m_fDispScaleY ;

    int m_iPreImgW ;
    int m_iPreImgH ;
    int m_iPrePanW ;
    int m_iPrePanH ;

    //void UpdateScale();

public:		// User declarations
    EN_VISN_ID m_iVisnId ;
    int        m_iCnt ;

    __fastcall TFrmImage(TComponent* Owner);

    void       Init (EN_VISN_ID _iId) ;
    void       Close();
    EN_VISN_ID GetVisnId(void);


    void OnPaint();
    void OnPaintT();

    //부분 확대 영상.
    void SetMagView(float _fLeft , float _fTop , float _fWidth , float _fHeight);
    void SetViewMode(bool _bStratch);

};



//---------------------------------------------------------------------------
extern PACKAGE TFrmImage *FrmImage[MAX_VISN_ID];
//---------------------------------------------------------------------------
#endif
