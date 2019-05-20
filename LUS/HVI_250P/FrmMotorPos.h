//---------------------------------------------------------------------------

#ifndef FrmMotorPosH
#define FrmMotorPosH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
#include "UtilDefine.h"
//---------------------------------------------------------------------------
//Motr Window Cylinder Window Frame Unit Refernce
//---------------------------------------------------------------------------
#include "FlmMotr.h"
#include "FlmCyl.h"
//---------------------------------------------------------------------------

class TFormMotorPos : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *btSaveDevice;
        TPageControl *PageControl;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox1;
        TPanel *pnPs_PRB_XIdx;
        TGroupBox *GroupBox2;
        TPanel *pnPs_PRB_TFlp;
        TTabSheet *TabSheet2;
        TGroupBox *GroupBox3;
        TPanel *pnPs_WRK_1Idx;
        TGroupBox *GroupBox4;
        TPanel *pnPs_WRK_2Idx;
        TTabSheet *TabSheet3;
        TGroupBox *GroupBox5;
        TPanel *pnPs_WRK_ZPch;
        TGroupBox *GroupBox6;
        TPanel *pnPs_WRK_XRjt;
        TPanel *pnMt_PRB_XIdx;
        TPanel *pnMt_PRB_TFlp;
        TPanel *pnMt_WRK_1Idx;
        TPanel *pnMt_WRK_2Idx;
        TPanel *pnMt_WRK_XRjt;
        TPanel *pnMt_WRK_ZPch;
        TGroupBox *GroupBox7;
        TPanel *pnPRB_Idx;
        TGroupBox *GroupBox8;
        TPanel *pnWRK_1Idx;
        TGroupBox *GroupBox9;
        TPanel *pnWRK_2Idx;
        TBitBtn *btSave;
        void __fastcall btSaveDeviceClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
//    void __fastcall UpdateDevOptn(bool bTable);
//    EN_MOTR_ID m_iMotor ;
//    bool bRet1;

public:		// User declarations

    TFrameMotr *FrmMotor   [MAX_MOTR];
    TFrameCyl  *FrmCylinder[MAX_ACTR];

    __fastcall TFormMotorPos(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMotorPos *FormMotorPos;
//---------------------------------------------------------------------------
#endif

