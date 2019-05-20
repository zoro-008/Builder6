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
    TSpeedButton *btMag;
    TSpeedButton *btHisto;
    TSpeedButton *btPixel;
        TMenuItem *CameraLights1;
        TSpeedButton *btCenter;
        TSpeedButton *btNoCenter;
        TLabel *lbDisp;
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
        void __fastcall CameraLights1Click(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall btMagClick(TObject *Sender);
        void __fastcall btPixelClick(TObject *Sender);
        void __fastcall btHistoClick(TObject *Sender);
        void __fastcall btCenterClick(TObject *Sender);
private:	// User declarations

    int m_iOriImgWidth  ; //���� �̹����� ���̿�
    int m_iOriImgHeight ; // ����.

    //fmMag
    void PaintCallback(); //Camera Callback

    //��ü �˻� �ݹ�.
    InspCallbackFunc InspCallback ;

    TFrmMag     *FrmMag     ;
    TFrmPixel   *FrmPixel   ;
    TFrmHisto   *FrmHisto   ;

    //���� â.
    TFrmCamLightPara *FrmCamLightPara ;
    TFrmCalPara      *FrmCalPara      ;


    int m_iDpCnt ;

    //ȭ�� �׸��� �� ....
    TWndMethod OriginalProc;
    void __fastcall PanelProc(Messages::TMessage &msg);

    struct TStat {
        bool bTrckrClick  ; //Ʈ��Ŀ Ŭ����.
        bool bImgClick    ; //�̹��� �̵�.
        int  iClickX      ; //Ʈ��Ŀ Ŭ��X
        int  iClickY      ; //Ʈ��Ŀ Ŭ��Y
    }Stat;

    EN_VISN_ID m_iVisnId ;



    //bool m_bChecked ;
    int  m_iPrePage ;

    TRectTracker * RectTracker ;
    TLineTracker * LineTracker ;

    bool m_bPaintLock ;

    void OnPaint();


public:		// User declarations
    __fastcall TFrmImage(TComponent* Owner);

    void SetInspCallbackFunc (InspCallbackFunc _InspCallback);

    bool LoadData(AnsiString _sFilePath);
    bool SaveData(AnsiString _sFilePath);

    void      SetVisnId(EN_VISN_ID _iId) ;
    EN_VISN_ID GetVisnId(void);

    void SetLevel (int _iLevel);

    void SetFormEnable(bool _bTrue);

    void OnPaintMsg();
};



//---------------------------------------------------------------------------
//extern PACKAGE TFrmImage *FrmImage;
//---------------------------------------------------------------------------
#endif