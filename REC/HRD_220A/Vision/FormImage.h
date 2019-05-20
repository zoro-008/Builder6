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
#include "FormTrain.h"
#include "Form3d.h"

#include "FormJob.h"

#include "UnitTracker.h"
#include "UnitVision.h"


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
        TPanel *pnInspTime;
        TPanel *pnDisp;
        TPanel *pnGrabTime;
        TPanel *pnBase;
        TPanel *pnDraw;
        TTimer *tmLive;
        TSpeedButton *btTrain;
        TSpeedButton *btDevice;
        TSpeedButton *btNoRslt;
        TMenuItem *N1;
        TSpeedButton *btLiveInsp;
        TButton *Button1;
        TButton *Button2;
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
        void __fastcall lbMenuMouseEnter(TObject *Sender);
        void __fastcall lbMenuMouseLeave(TObject *Sender);
        void __fastcall lbMenuMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall miCntrLineClick(TObject *Sender);
        void __fastcall btMagClick(TObject *Sender);
        void __fastcall btPixelClick(TObject *Sender);
        void __fastcall btHistoClick(TObject *Sender);
        void __fastcall btCenterClick(TObject *Sender);
        void __fastcall pnDrawMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall btTrainClick(TObject *Sender);
        void __fastcall tmLiveTimer(TObject *Sender);
        void __fastcall btDeviceClick(TObject *Sender);
        void __fastcall btNoRsltClick(TObject *Sender);
        void __fastcall btLiveInspClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations

    CVision       *m_pVision   ;
    //CImage        *m_pActiveImg;

    TFrmMag     *FrmMag     ;
    TFrmPixel   *FrmPixel   ;
    TFrmHisto   *FrmHisto   ;
    TFrmTrain   *FrmTrain   ;
    TFrm3d      *Frm3d      ;


    //���� â.
    //TFrmCamLightPara *FrmCamLightPara ;

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

    //EN_VISN_ID m_iVisnId ;
    CTracker * RectTracker ;
    CTracker * LineTracker ;



    double GetScaleX();
    double GetScaleY();

    double m_dDispImgX ; //Ȯ��� �̹��� ���� ��ġ.
    double m_dDispImgY ;

    double m_dDispScaleX ; //ȭ�� ���÷��� ������.
    double m_dDispScaleY ;

    int m_iPreImgW ;
    int m_iPreImgH ;
    int m_iPrePanW ;
    int m_iPrePanH ;

    //void UpdateScale();

    int m_iLevel ;    //0:oper 1: engineer 2:Master
    bool m_bAutorun ;


public:		// User declarations
    int        m_iCnt ;

    __fastcall TFrmImage(TComponent* Owner);

    void       Init () ;
    void       Close();

    void SetVision(CVision * _pVision){
        m_pVision = _pVision;
    }


    void OnPaint();

    //�κ� Ȯ�� ����.
    void SetMagView(double _dLeft , double _dTop , double _dWidth , double _dHeight);
    void SetViewMode(bool _bStratch);




    void SetLive   (bool _bLive );
    void SetLevel  (int  _iLevel);  //0:oper 1: engineer 2:Master
    void SetAutorun(bool _bRun  );

};



//---------------------------------------------------------------------------
extern PACKAGE TFrmImage *FrmImage;
//---------------------------------------------------------------------------
#endif