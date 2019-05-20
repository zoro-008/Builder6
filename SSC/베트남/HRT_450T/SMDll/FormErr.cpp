//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Motors.h"
#include "Actuators.h"
#include "ErrMan.h"
#include "FormErr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmErr *FrmErr;

//---------------------------------------------------------------------------
const int MSG_SHOW = 0 ;
const int MSG_HIDE = 1 ;

void __fastcall ShowErr(int _iNo)
{
    if (FrmErr == NULL   ) return ;
    if (FrmErr ->Visible ) return ;

    //Show Form.
    PostMessage(FrmErr -> Handle , WM_USER , 0 , 0);
    //PostMessage(FrmErr -> Handle , WM_USER , MSG_SHOW , 0);
    //SendMessage(FrmErr -> Handle , WM_USER , MSG_SHOW , 0);

    //FrmErr -> Show() ;
}

void __fastcall HideErr()
{
    if ( FrmErr == NULL   ) return ;
    if (!FrmErr ->Visible ) return ;

    PostMessage(FrmErr -> Handle , WM_CLOSE , 0 , 0 );

    //PostMessage(FrmErr -> Handle , WM_USER , MSG_HIDE , 0);
    //SendMessage(FrmErr -> Handle , WM_USER , MSG_HIDE , 0);

}
//---------------------------------------------------------------------------
__fastcall TFrmErr::TFrmErr(TComponent* Owner)
    : TForm(Owner)
{
    //Left = 826 ;
    //Top  = 396 ;
    //Position = poMainFormCenter ;
    //Position = poDesktopCenter ;

    Left = 414 ;
    Top  = 138 ;

    //tmUpdate -> Enabled = true;
}

//---------------------------------------------------------------------------
__fastcall TFrmErr::~TFrmErr()
{

    tmUpdate -> Enabled = false;
    //Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmErr::WmUser(TMessage &Message)
{
    Message.Result = true;
    if (this == NULL && Visible) return;

    this -> Show ();

    

    /*
    Message.Result = true;
    if (this == NULL) return;

         if(Message.WParam == MSG_SHOW && !Visible) this -> Show ();
    else if(Message.WParam == MSG_HIDE &&  Visible) this -> Close();
    */
}

void __fastcall TFrmErr::btResetClick(TObject *Sender)
{
    MT.ResetAll();
    AT.Reset();
    EM.Clear();

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmErr::tmUpdateTimer(TObject *Sender)
{
 //   if     (pnTitle -> Color == 0x00FFBBBB) pnTitle -> Color =  clRed             ;
 //   else if(pnTitle -> Color == clRed     ) pnTitle -> Color = (TColor)0x00FFBBBB ;
 // TShape ±ôºý±ôºý
    spErrRect -> Visible  = !spErrRect -> Visible ;

}
//---------------------------------------------------------------------------

void __fastcall TFrmErr::btCloseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

static bool bFirst = true  ;
static bool bIng   = false ;
void __fastcall TFrmErr::FormShow(TObject *Sender)
{
    if(bIng) return ;
    bIng = true;
    tmUpdate -> Enabled = true ;
    //Position = poMainFormCenter ;
    //if(bFirst){
        //EM.DisplayErrForm(lbErrNo, lbName, mmErrAction, imageErr, spErrRect) ;
        DisplayErrForm(lbErrNo, lbName, mmErrAction, imageErr, spErrRect) ;

    //}

    btClose -> SetFocus() ;
    bIng = false ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmErr::FormHide(TObject *Sender)
{
    tmUpdate -> Enabled = false;
}
//---------------------------------------------------------------------------


void __fastcall TFrmErr::DisplayErrForm (TLabel *_lbErrNo    ,
                                         TLabel *_lbName     ,
                                         TMemo  *_mmAction   ,
                                         TImage *_imErr      ,
                                         TShape *_spErrRect  )
{
    //Local Var.
    AnsiString Temp,Str  ;
    int        iRect     ;
    int        Cnt,iCount;
    char       CChar     ;
    int        iErrNo    ;
    int        iPicWidth ;
    int        iPicHeight;

    iPicWidth  = _imErr -> Width  ;
    iPicHeight = _imErr -> Height ;


    iErrNo = EM.GetLastErr() ;





    //Set Caption.
    Temp = Temp.sprintf("ERR%03d", iErrNo );
    _lbErrNo   -> Caption = Temp;
    _lbName    -> Caption = EM.GetErrName(iErrNo);//  Err[].sName;
    _mmAction  -> Clear();

    //Display Solution
    Temp = EM.GetErrAction(iErrNo);  //Err[iErrNo].sAction;
    Cnt = 0;
    CChar = '*';
    for(register int i = 1 ; i <= Temp.Length() ; i++) if (Temp[i] == CChar) iCount++;
    Cnt = iCount;

    for (int i=0; i <= Cnt ; i++) {
        AnsiString Line  = "";
        int n = Temp.Pos("*");
        Line  = Temp.SubString(1, n ? n-1 : n);
        Temp  = Temp.Delete(1, n);
        if (!n) { _mmAction -> Lines -> Add(Temp); break; }
        else    { _mmAction -> Lines -> Add(Line);        }
    }


    Temp = EM.GetErrImgPath(iErrNo); // Err[].sImgPath ;
    if (Temp == "" || !FileExists(Temp)) {
        _imErr->Visible = false ;
    }
    else {
        //_imErr -> Visible = true ;
        //_imErr -> Picture->Assign(NULL);
        _imErr -> Picture -> LoadFromFile(Temp);
        _imErr -> Visible = true ;

    }


    if (EM.GetRectWidth(iErrNo) == 0|| EM.GetRectHeight(iErrNo) == 0) {
        _spErrRect -> Left   = 1;
        _spErrRect -> Top    = 1;
        _spErrRect -> Width  = 60;
        _spErrRect -> Height = 60;
    }
    else {
        _spErrRect -> Left   = (EM.GetRectLeft  (iErrNo) * iPicWidth )/100.0 ;
        _spErrRect -> Top    = (EM.GetRectTop   (iErrNo) * iPicHeight)/100.0 ;
        _spErrRect -> Width  = (EM.GetRectWidth (iErrNo) * iPicWidth )/100.0 ;
        _spErrRect -> Height = (EM.GetRectHeight(iErrNo) * iPicHeight)/100.0 ;
    }
}
void __fastcall TFrmErr::FormDestroy(TObject *Sender)
{
//    imageErr -> Free();
//    delete imageErr ;
    tmUpdate -> Enabled = false;

}
//---------------------------------------------------------------------------

