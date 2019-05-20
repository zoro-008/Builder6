//---------------------------------------------------------------------------

#ifndef FormCadPosH
#define FormCadPosH
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
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Grids.hpp>







class TFrmCadPos : public TForm
{
__published:	// IDE-managed Components
    TTimer *tmUpdate;
    TPanel *pnTopToolbar;
        TPanel *pnBase;
        TSpeedButton *btZoomOut;
        TSpeedButton *btZoomIn;
        TSpeedButton *btRealSize;
        TPanel *Panel1;
        TComboBox *cbObjectList;
        TPageControl *pcObjType;
        TBitBtn *btSave;
        TTabSheet *tsPoint;
        TTabSheet *tsCadDspCmd;
        TTabSheet *tsCadLine;
        TTabSheet *tsCadHeight;
        TTabSheet *tsCadRect;
        TEdit *edPntCntrPosX;
        TEdit *edPntCntrPosY;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *edDspCmdNodeCnt;
        TStringGrid *sgDspCmdPos;
        TEdit *edLineSttPosX;
        TEdit *edLineSttPosY;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *edLineEndPosX;
        TEdit *edLineEndPosY;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *edHghtSttPosX;
        TEdit *edHghtSttPosY;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *edHghtSubPosX;
        TEdit *edHghtSubPosY;
        TLabel *Label10;
        TLabel *Label11;
        TEdit *edHghtEndPosX;
        TEdit *edHghtEndPosY;
        TLabel *Label12;
        TLabel *Label13;
        TEdit *edRectLeft;
        TEdit *edRectTop;
        TLabel *Label14;
        TLabel *Label15;
        TEdit *edRectWidth;
        TEdit *edRectHeight;
        TLabel *Label16;
        TLabel *Label17;
        TEdit *edRectRight;
        TEdit *edRectBottom;
        TLabel *Label18;
        TLabel *Label19;
        TButton *btMOVE;
        TCheckBox *cbVisible;
        TTabSheet *tsHghtCmd;
        TLabel *Label26;
        TEdit *edHghtCmdNodeCnt;
        TStringGrid *sgHghtCmdPos;
        TColorDialog *cdColor;
        TButton *btColor;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
        TLabel *Label30;
        TLabel *Label31;
        TLabel *Label32;
        TLabel *Label41;
        TLabel *Label42;
        TEdit *edRhombusLeft;
        TEdit *edRhombusTop;
        TEdit *edRhombusWidth;
        TEdit *edRhombusHeight;
        TEdit *edRhombusRight;
        TEdit *edRhombusBottom;
        TEdit *edRhombusCntrPosX;
        TEdit *edRhombusCntrPosY;
        TLabel *Label33;
        TLabel *Label34;
        TEdit *edCntPntCntrPosX;
        TEdit *edCntPntCntrPosY;
        TLabel *Label35;
        TLabel *Label36;
        TOpenDialog *dgLoadFilePath;
        TSaveDialog *dgSaveFilePath;
        TPanel *pnColor;
        TPanel *pnDraw;
        TTabSheet *VisnPoint;
        TLabel *Label20;
        TLabel *Label21;
        TEdit *edVisnPntCntrPosX;
        TEdit *edVisnPntCntrPosY;
        TComboBox *cbToolID;
        TButton *btHxZGet;
        TButton *btHxZWait;
        TEdit *edShiftX;
        TEdit *edShiftY;
        TLabel *Label22;
        TLabel *Label23;
        TButton *Button1;
        TButton *btOpenAs;
        TButton *btSaveAs;
        TBitBtn *btSaveAll;
        TButton *btVisibleSub;
        TButton *btVisibleSubOff;
        TLabel *lbPos;
        TButton *Button2;
        TButton *Button3;

    void __fastcall tmUpdateTimer(TObject *Sender);
    void __fastcall pnDrawMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnDrawMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btZoomOutClick(TObject *Sender);
    void __fastcall btZoomInClick(TObject *Sender);
    void __fastcall btRealSizeClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall btCenterClick(TObject *Sender);
        void __fastcall pnDrawMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall cbObjectListChange(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall sgDspCmdPosDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall sgDspCmdPosSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall pcObjTypeChange(TObject *Sender);
        void __fastcall sgHghtCmdPosSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall btColorClick(TObject *Sender);
        void __fastcall cbVisibleClick(TObject *Sender);
        void __fastcall btMOVEClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall btOpenAsClick(TObject *Sender);
        void __fastcall btSaveAsClick(TObject *Sender);
        void __fastcall btSaveAllClick(TObject *Sender);
        void __fastcall btVisibleSubClick(TObject *Sender);
        void __fastcall btVisibleSubOffClick(TObject *Sender);
        void __fastcall cbToolIDChange(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
private:	// User declarations

    //화면 그리는 것 ....
    TWndMethod OriginalProc;
    void __fastcall PanelProc(Messages::TMessage &msg);

    //CTracker * LineTracker ;

    double m_dDispImgX ; //확대시 이미지 시작 위치.
    double m_dDispImgY ;

    double m_dDispScaleX ; //화면 디스플레이 스케일.
    double m_dDispScaleY ;

    //화면 이동.
    struct TStat {
        bool bImgClick    ; //이미지 이동.
        int  iClickX      ; //트렉커 클릭X
        int  iClickY      ; //트렉커 클릭Y
        TStat(){
            bImgClick = false ;
            iClickX   = 0 ;
            iClickY   = 0 ;
        }
    }Stat;


public:		// User declarations
    int        m_iCnt ;

    __fastcall TFrmCadPos(TComponent* Owner);

    void       Init () ;
    void       Close() ;

    void UpdateCadPosition(bool bToTable);

    void VisibleCheck();
    void OnPaint();


};



//---------------------------------------------------------------------------
extern PACKAGE TFrmCadPos *FrmCadPos;
//---------------------------------------------------------------------------
#endif
