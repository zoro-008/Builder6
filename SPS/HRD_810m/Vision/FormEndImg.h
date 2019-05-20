//---------------------------------------------------------------------------

#ifndef FormEndImgH
#define FormEndImgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "CamUnit.h"
#include "SVInterface.h"
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
//---------------------------------------------------------------------------
class TFrmEndImg : public TForm
{
__published:	// IDE-managed Components
        TTimer *tmClose;
        TPanel *pnDraw;
        TPanel *Panel1;
        TEdit *edExps;
        TButton *Button1;
        TButton *Button2;
        TButton *btSaveAs;
        TSavePictureDialog *SavePictureDialog1;
        TLabel *Label1;
        TButton *Button3;
        TButton *Button4;
        void __fastcall tmCloseTimer(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall btSaveAsClick(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmEndImg(TComponent* Owner);


        //Main Thread������ ȣ���ؾ߉�.
        void OnPaint();

        //ī�޶� ������ִ� �ݹ�.
        void CamCallback (int _iCamNo);
        void Grab();
        void Live(bool _bOn);
        bool GetGrabEnd();

        bool SaveImg(String _sFilePath);


        //ȭ�� �׸��� �� ....
        TWndMethod OriginalProc;
        void __fastcall PanelProc(Messages::TMessage &msg);

        //imi ī�޶� Ŭ����.
        CImi * EndCam ;
        //�̹��� Ŭ����
        IImage * Img ;
        bool m_bGrabbing ;
        bool m_bLive ;


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmEndImg *FrmEndImg;


//---------------------------------------------------------------------------
#endif
