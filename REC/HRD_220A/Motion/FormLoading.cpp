//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
//Part Reference.
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Index.h"
#include "Picker.h"
#include "Stage.h"
#include "Dispensor.h"
#include "Unloader.h"
//---------------------------------------------------------------------------
//Rs232 Part.
//---------------------------------------------------------------------------
#include "Rs232Dispr.h"
#include "Rs232Keyence.h"
#include "Rs232LoadCl.h"
#include "Rs232Temp.h"
//---------------------------------------------------------------------------
#include "Pattern.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#include "Sequence.h"
#include "SMInterfaceUnit.h"
#include "SVInterface.h"
#include "PstnMan.h"
#include "Pattern.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "UserINI.h"
#include "LotUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLoading *FrmLoading;
//---------------------------------------------------------------------------
__fastcall TFrmLoading::TFrmLoading(TComponent* Owner , bool _bLoad)
    : TForm(Owner)
{
    m_bLoad = _bLoad ; 
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::FormShow(TObject *Sender)
{
    SetWindowRgn(Handle, CreateRoundRectRgn(ClientRect.Left, ClientRect.Top, ClientRect.Right + 1, ClientRect.Bottom + 1, 2, 2), FALSE);

    //Display Version.
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'�� 'm'�� 'd'�� 'AM/PM' 'h'�� 'n'��'");
    lbVersion -> Caption = Date ;
    pbProcess->Position = 0;
    if(m_bLoad) tmLoad->Enabled = true ;
    else        tmSave->Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmLoad->Enabled = false ;
    tmSave->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::tmLoadTimer(TObject *Sender)   //���ε� �޴��� ��ư �����.
{
    tmLoad -> Enabled = false ;

    static int ProcessIndex = 0 ;

    switch (ProcessIndex) {

        //case 0 : pbProcess->Position = 0  ; GetNormalEnd();                                                                  break ;
        case 0 : pbProcess->Position = 0  ; /*GetNormalEnd();  */                                                            break ;
        case 1 : pbProcess->Position = 5  ; OM   .Init(); lbName    -> Caption  = OM.EqpOptn.sModelName ; LoadSML("SMDll" ); break ;
        case 2 : pbProcess->Position = 10 ; LoadSVision("pSVision.dll" );                                                    break ;
        case 3 : pbProcess->Position = 15 ; DM   .Init(     );                                                               break ;
        case 4 : pbProcess->Position = 20 ; PM   .Init(     );                                                               break ;
        case 5 : pbProcess->Position = 25 ; MM   .Init(     );                                                               break ;
        case 6 : pbProcess->Position = 30 ; OM   .Init(     );                                                               break ;
        case 7 : pbProcess->Position = 35 ; LDR_F.Init(true );                                                               break ;
        case 8 : pbProcess->Position = 40 ; LDR_R.Init(false);                                                               break ;
        case 9 : pbProcess->Position = 45 ; PRI_F.Init(true );                                                               break ;
        case 10: pbProcess->Position = 50 ; PRI_R.Init(false);                                                               break ;
        case 11: pbProcess->Position = 55 ; IDX_F.Init(true );                                                               break ;
        case 12: pbProcess->Position = 60 ; IDX_R.Init(false);                                                               break ;
        case 13: pbProcess->Position = 65 ; PCK  .Init(     );                                                               break ;
        case 14: pbProcess->Position = 70 ; STG  .Init(     );                                                               break ;
        case 15: pbProcess->Position = 75 ; DSP  .Init(     );                                                               break ;
        case 16: pbProcess->Position = 80 ; ULD_F.Init(true );                                                               break ;
        case 17: pbProcess->Position = 85 ; ULD_R.Init(false);                                                               break ;
        case 18: pbProcess->Position = 90 ; SPC  .Init(     );                                                               break ;
        case 19: pbProcess->Position = 95 ; LOT  .Init(     ); PTT.Init();                                                   break ;
        case 20: pbProcess->Position = 100; SEQ  .Init(     );  ModalResult = mrOk;                                          break ;
    }

    ProcessIndex++;
    tmLoad -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmLoading::tmSaveTimer(TObject *Sender)
{
    tmSave -> Enabled = false ;

    static int ProcessIndex = 0 ;

    switch (ProcessIndex) {
        case 0 : pbProcess->Position = 0  ; OM  .Close();                 break ;
        case 1 : pbProcess->Position = 5  ; /*FreeSVA("pSVApp");*/        break ;
        case 2 : pbProcess->Position = 10 ; DM   .Close();                break ;  //�̰� �����ɸ�.
        case 3 : pbProcess->Position = 15 ; PM   .Close();                break ;
        case 4 : pbProcess->Position = 20 ; MM   .Close();                break ;
        case 5 : pbProcess->Position = 25 ; OM   .Close();                    break ;
        case 6 : pbProcess->Position = 30 ; LDR_F.Close();                break ;
        case 7 : pbProcess->Position = 35 ; LDR_R.Close();                break ;
        case 8 : pbProcess->Position = 40 ; PRI_F.Close();                break ;
        case 9 : pbProcess->Position = 45 ; PRI_R.Close();                break ;
        case 10: pbProcess->Position = 50 ; IDX_F.Close();                break ;
        case 11: pbProcess->Position = 55 ; IDX_R.Close();                break ;
        case 12: pbProcess->Position = 60 ; PCK  .Close();                break ;
        case 13: pbProcess->Position = 65 ; STG  .Close();                break ;
        case 14: pbProcess->Position = 70 ; DSP  .Close();                break ;
        case 15: pbProcess->Position = 75 ; ULD_F.Close();                break ;
        case 16: pbProcess->Position = 80 ; ULD_R.Close();                break ;
        case 17: pbProcess->Position = 85 ; SPC  .Close();                break ;
        case 18: pbProcess->Position = 90 ; LOT  .Close();                break ;
        //case 19: pbProcess->Position = 95 ; SetNormalEnd();               break ;
        case 19: pbProcess->Position = 100; ModalResult = mrOk;           break ;
    }

    ProcessIndex++;
    tmSave -> Enabled = true ;
}

//---------------------------------------------------------------------------
/*
bool TFrmLoading::GetNormalEnd()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    sPath= EXE_FOLDER + "SeqData\\NormalEnd.INI" ;

    //Load
    AnsiString sIndex   ;
    AnsiString sTemp    ;

    int iNormalEnd ;
    UserINI.Load(sPath, "NormalEnd" , "__NormalEnded__", iNormalEnd );
    if(!iNormalEnd) {
        ShowMessage("���α׷��� �������� ���� �ʾҽ��ϴ�. Ʈ���� �� �� �����͸� �ʱ�ȭ���ּ���");
        Trace("Warning","���α׷��� �������� ���� �ʾҽ��ϴ�. Ʈ���� �� �� �����͸� �ʱ�ȭ���ּ���");
    }
    UserINI.Save(sPath, "NormalEnd" , "__NormalEnded__", 0 );

    return true ;
}

*/