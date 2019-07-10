//---------------------------------------------------------------------------

#ifndef FormTrainH
#define FormTrainH
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

#include "Tracker.h"
#include "FormValueList.h"
#include "UnitVision.h"
#include "CamUnit.h"
#include "PkgCamera.h"


#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include <ImgList.hpp>



class TFrmTrain : public TForm
{
__published:	// IDE-managed Components
    TSavePictureDialog *SavePictureDialog1;
    TOpenPictureDialog *OpenPictureDialog1;
    TTimer *tmUpdate;
        TPanel *Panel1;
        TPanel *pnImgViewerBase;
        TPanel *pnTopToolbar;
        TSpeedButton *btMag;
        TSpeedButton *btHisto;
        TSpeedButton *btPixel;
        TPanel *pnInspTime;
        TPanel *pnDisp;
        TPanel *pnGrabTime;
        TPanel *pnImgCpyTime;
        TPanel *pnThreshold;
        TScrollBar *sbThresh;
        TPanel *pnThreash;
        TPanel *pnDraw;
        TSpeedButton *btGray;
        TSpeedButton *btBinary;
        TSpeedButton *btFalse;
        TSpeedButton *btColor;
        TSpeedButton *btRed;
        TSpeedButton *btGreen;
        TSpeedButton *btBlue;
        TSpeedButton *btStratch;
        TSpeedButton *btRealRatio;
        TSpeedButton *btZoomOut;
        TSpeedButton *btZoomIn;
        TSpeedButton *btRealSize;
        TSpeedButton *btCenter;
        TPanel *pnPixelInfo;
        TPanel *pnColor;
        TPanel *pnAdd;
        TEdit *edPkgName;
        TBitBtn *btInsertCancel;
        TBitBtn *btInsertList;
        TComboBox *cbPkgKind;
        TTimer *tmLive;
        TPanel *Panel2;
        TSpeedButton *btCommonPara;
        TSpeedButton *btLocalPara;
        TSpeedButton *btMasterPara;
        TValueListEditor *vlPara;
        TPanel *Panel109;
        TPanel *Panel3;
        TStringGrid *sgPkgList;
        TPanel *Panel4;
        TImageList *ImageList1;
        TPanel *pnPkgHandler;
        TPanel *Panel6;
        TSpeedButton *btNext;
        TSpeedButton *btRun;
        TSpeedButton *btTrain;
        TSpeedButton *btPre;
        TBitBtn *btUp;
        TBitBtn *btInsert;
        TBitBtn *btDel;
        TBitBtn *btDn;
        TBitBtn *btSavePara;
        TSpeedButton *btValueList;
        TPanel *pnCamera;
        TPanel *Panel7;
        TSpeedButton *btLoad;
        TSpeedButton *btSave;
        TSpeedButton *btLive;
        TSpeedButton *btCapture;
        TBitBtn *BitBtn1;
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
    void __fastcall pcParaChange(TObject *Sender);
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btMagClick(TObject *Sender);
        void __fastcall btPixelClick(TObject *Sender);
        void __fastcall btHistoClick(TObject *Sender);
        void __fastcall btCenterClick(TObject *Sender);
        void __fastcall pnDrawMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btInsertClick(TObject *Sender);
        void __fastcall btInsertCancelClick(TObject *Sender);
        void __fastcall btInsertListClick(TObject *Sender);
        void __fastcall btUpClick(TObject *Sender);
        void __fastcall btDnClick(TObject *Sender);
        void __fastcall btDelClick(TObject *Sender);
        void __fastcall sgPkgListClick(TObject *Sender);
        void __fastcall btLocalParaClick(TObject *Sender);
        void __fastcall tmLiveTimer(TObject *Sender);
        void __fastcall btLiveClick(TObject *Sender);
        void __fastcall btTrainClick(TObject *Sender);
        void __fastcall btRunClick(TObject *Sender);
        void __fastcall btPreClick(TObject *Sender);
        void __fastcall btNextClick(TObject *Sender);
        void __fastcall vlParaKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall btSaveParaClick(TObject *Sender);
        void __fastcall btValueListClick(TObject *Sender);
        void __fastcall btCaptureClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations

    //fmMag
    CVision       * m_pVision   ;
    CImage        * m_pActiveImg;
    void          * m_pOriMsgWnd;

    TFrmMag       * FrmMag      ;
    TFrmPixel     * FrmPixel    ;
    TFrmHisto     * FrmHisto    ;
    TFrmValueList * FrmValueList;


    //ȭ�� �׸��� �� ....
    TWndMethod OriginalProc;
    void __fastcall PanelProc(Messages::TMessage &msg);

    struct TStat {
        bool bTrckrClick  ; //Ʈ��Ŀ Ŭ����.
        bool bImgClick    ; //�̹��� �̵�.
        int  iClickX      ; //Ʈ��Ŀ Ŭ��X
        int  iClickY      ; //Ʈ��Ŀ Ŭ��Y
    }Stat;


    TRectTracker * RectTracker ; 
    TLineTracker * LineTracker ;


    int    m_iDpCnt ;

    double GetScaleX();
    double GetScaleY();

    double m_dDispImgX   ; //Ȯ��� �̹��� �����ÿ� �̹������� ��ġ.
    double m_dDispImgY   ;
    double m_dDispScaleX ; //ȭ�� ���÷��� ������.
    double m_dDispScaleY ;

    int m_iPreImgW ;
    int m_iPreImgH ;
    int m_iPrePanW ;
    int m_iPrePanH ;

    int m_iLevel ;    //0:oper 1: engineer 2:Master

public:		// User declarations
    __fastcall TFrmTrain(TComponent* Owner);
    bool Init ();
    bool Close();

    void SetVision(CVision * _pVision){m_pVision = _pVision;}


    void OnPaint();
    void OnPaintMsg();

    //�κ� Ȯ�� ����.
    void SetMagView(double _dLeft , double _dTop , double _dWidth , double _dHeight);
    void SetViewMode(bool _bStratch);

    void UpdateTable();

    void SetLevel(int _iLevel);  //0:oper 1: engineer 2:Master

};



//---------------------------------------------------------------------------
extern PACKAGE TFrmTrain *FrmTrain;
//---------------------------------------------------------------------------
#endif
