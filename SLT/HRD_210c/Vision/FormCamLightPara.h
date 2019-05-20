//---------------------------------------------------------------------------

#ifndef FormCamLightParaH
#define FormCamLightParaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmCamLightPara : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnParaBase;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TEdit *edUseCamNo;
        TLabel *Label2;
        TEdit *edCamExposure;
        TLabel *Label3;
        TEdit *edCntOffsetX;
        TLabel *Label4;
        TEdit *edCntOffsetY;
        TGroupBox *GroupBox2;
        TLabel *Label5;
        TEdit *edUseLightNo;
        TLabel *Label6;
        TEdit *edUseLightCh1;
        TLabel *Label7;
        TEdit *edLtBright1;
        TLabel *Label8;
        TEdit *edUseLightCh2;
        TLabel *Label9;
        TEdit *edLtBright2;
        TBitBtn *btLotEnd;
        TBitBtn *btSaveDevice;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TEdit *edUseLightCh3;
        TEdit *edLtBright3;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btLotEndClick(TObject *Sender);
        void __fastcall btSaveDeviceClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        int m_iVisnId ;
public:		// User declarations
        __fastcall TFrmCamLightPara(TComponent* Owner);

        //__fastcall TFrmCamLightPara(TComponent* Owner , int _iVisnId);

        void ApplyPara(bool _bToTable);

        void Show(int _iVisnId);


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCamLightPara *FrmCamLightPara;
//---------------------------------------------------------------------------
#endif
