//---------------------------------------------------------------------------

#ifndef FormImageH
#define FormImageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "UtilDefine.h"
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>

#include "VisnDefine.h"
#include "UtilDefine.h"
//---------------------------------------------------------------------------
class TFrmImage : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnDraw;
        TPanel *pnTopToolbar;
        TSpeedButton *btSave;
        TSpeedButton *btLoad;
        TSpeedButton *btGrab;
        TSpeedButton *btLive;
        TSpeedButton *btCenter;
        TOpenPictureDialog *OpenPictureDialog1;
        TSavePictureDialog *SavePictureDialog1;
        TButton *btMax;
        TLabel *Label1;
        TTimer *Timer1;
        TLabel *Label2;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btGrabClick(TObject *Sender);
        void __fastcall btLiveClick(TObject *Sender);
        void __fastcall btLoadClick(TObject *Sender);
        void __fastcall btCenterClick(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
        void __fastcall pnDrawClick(TObject *Sender);
        void __fastcall btMaxClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Label2Click(TObject *Sender);
private:	// User declarations
        TWinControl * m_pOriParent ;
        TWinControl * m_pMaxParent ;
public:		// User declarations
        __fastcall TFrmImage(TComponent* Owner);
        void SetStgId(int _iStgId);


        //화면 그리는 것 ....
        TWndMethod OriginalProc;
        void __fastcall PanelProc(Messages::TMessage &msg);
        void OnPaint();

        int m_iStgId ;

        EN_IMG_ID m_iActiveImg ;

        float m_fDispScaleX ;
        float m_fDispScaleY ;


        void SetMaxParent(TWinControl * _pMaxParent);
};
//---------------------------------------------------------------------------
extern TFrmImage *FrmImage[MAX_STAGE_CNT];
//---------------------------------------------------------------------------
#endif
