//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <jpeg.hpp>
#include "FormDllMain.h"
#include "FormPassword.h"
#include "Actuators.h"
#include "IOs.h"
#include "Motors.h"
#include "TEdit.h"
#include "ErrMan.h"
#include "FormMsgOk.h"
#include "FormMsgYesNo.h"
#include "FormUpdate.h"
#include "TowerLamp.h"
#include "UserIni.h"
#include "SMInterfaceUnit.h"

#pragma package(smart_init)
//#pragma link "LabelPlus"
//#pragma link "NumEdit"
#pragma resource "*.dfm"

typedef void (__closure *Func)();
template <typename T>
class CRunThread
{
    class TRunThread : public TThread
    {
        private:
            T Func;
        public:
            __fastcall TRunThread(T func) : TThread(false)
            {
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute()
            {
                Func();
            }
    };

    public:
        CRunThread(T func)
        {
            TRunThread*  thread = new TRunThread(func);
        }
};


TFrmDllMain *FrmDllMain;
//---------------------------------------------------------------------------
__fastcall TFrmDllMain::TFrmDllMain(TComponent* Owner)
        : TForm(Owner)
{
//    // 에러 이벤트 설정
//    Application->OnException = ExceptionFilter;
    pnErrImg -> DoubleBuffered = true ;
}

//---------------------------------------------------------------------------
AnsiString TFrmDllMain::OnGetVer(const AnsiString &sQuery)
{
   //sQuery "CompanyName", "FileDescription", "FileVersion", "InternalName", "LegalCopyright", "LegalTradeMarks", "OriginalFileName", "ProductName", "ProductVersion", "Comments"

   DWORD dwHandle = 0, dwVersionInfoSize;
   UINT uLength;
   LPVOID pFileInfo, ptr;
   AnsiString sOut; // 리턴될 버전 정보.

   //"D:\\Works\\version\\Project1.exe"
   AnsiString filename = Application->ExeName;

   dwVersionInfoSize = GetFileVersionInfoSize(filename.c_str(), &dwHandle);

   pFileInfo = (LPVOID) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwVersionInfoSize);

   GetFileVersionInfo(filename.c_str(), dwHandle, dwVersionInfoSize, pFileInfo);
   VerQueryValue(pFileInfo, TEXT("\\VarFileInfo\\Translation"), &ptr, &uLength);

   WORD *id = (WORD *) ptr;
   AnsiString szString = "\\StringFileInfo\\" + IntToHex(id[0], 4) + IntToHex(id[1], 4) + "\\" + sQuery;

   VerQueryValue(pFileInfo, szString.c_str(), &ptr, &uLength);
   sOut = AnsiString((char *) ptr);
   HeapFree(GetProcessHeap(), 0, pFileInfo );
   return sOut;
}


void __fastcall TFrmDllMain::FormCreate(TObject *Sender)
{
    //TrakerUnit
//    pTracker = new TRectTracker(this);
//    pTracker -> Parent = pnErrImg;
//    pTracker ->Left    = 10;
//    pTracker ->Top 	   = 10;
//    pTracker ->Width   = 100;
//    pTracker ->Height  = 100;

    edActrName   -> NumberOnly = false ;
    edIOIndex    -> NumberOnly = false ;
    edActrAddrIF -> NumberOnly = false ;
    edIOName     -> NumberOnly = false ;
    edActrAddrOF -> NumberOnly = false ;
    edIOInv      -> NumberOnly = false ;
    edActrComt   -> NumberOnly = false ;
                                         edIOAdd    -> UseComma = false ; edIOAdd    -> UseMinus   = false ;
    edActrAddrIB -> NumberOnly = false ;
    edIOComt     -> NumberOnly = false ;
    edActrAddrOB -> NumberOnly = false ;
                                         edIODelay  -> UseComma = false ; edIODelay  -> UseMinus   = false ;
    edErrEnum    -> NumberOnly = false ;
    edErrName    -> NumberOnly = false ;
    edImgPath    -> NumberOnly = false ;
    edMotorName  -> NumberOnly = false ;

                                         edActrSync -> UseComma = false ; edActrSync -> UseMinus   = false ;
                                         edRptDelay -> UseComma = false ; edRptDelay -> UseMinus   = false ;

    //Synchronization Edit
    edActrSync -> Enabled = false ;
    //Display Version.
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'-'mm'-'dd' 'AM/PM' 'hh':'nn''");
    lbDate -> Caption = Date ;
    lbVer  -> Caption = "Ver " + OnGetVer("FileVersion") ;

    Trace("Program Start",OnGetVer("FileVersion").c_str());

    Caption = "SMDll" ;

    pcMain -> ActivePageIndex = 0 ;

    //Width  =  ;
    //Height = 550 ;

    tmUpdate -> Enabled = true ;

    //Actuator Tab
    AT.DisplayList(sgActuator);
    AT.DisplayProperty(true , edActrName     , edActrComt     ,
                              edActrAddrIF   , edActrAddrIB   ,
                              edActrAddrOF   , edActrAddrOB   ,
                              edActrOnDelayF , edActrOnDelayB ,
                              edActrTimeOutB , edActrTimeOutF ,
                              sgActuator     );

    //IO Tab
    IO.DisplayList    (cbIO -> ItemIndex , sgIO , true );
    IO.DisplayProperty(true,cbIO->ItemIndex , edIOIndex, edIOAdd , edIOHexAdd , edIOName , edIOComt , cbIOInverse , edIODelay , sgIO , cbHex -> Checked ) ;

    //Error Tab
//    int iLeft  = pTracker->Left,
//        iTop   = pTracker->Top,
//        iWidth = pTracker->Width,
//        iHeight=pTracker->Height;

    EM.DisplayList  (sgErrList);
    EM.DisplayProperty(true, sgErrList, edErrEnum , edErrNo, edErrName, edImgPath, mmErrAction, imErrPicture, spErrRect);

    //Motor Tab

    //아진 엑스텍 관련 SUB UI 생성.
    SetUI(pnSubPara);

    MT.GetMotorList(cbMotor) ;
    cbMotor -> ItemIndex = 0 ;
    MT.UpdatePara(cbMotor -> ItemIndex ,
                  true            ,
                  edMotorName     ,
                  cbMotionType    ,
                  edPulsePerRev   ,
                  edUnitPerRev    ,
                  edMaxPos        ,
                  edMinPos        ,
                  cbExistEnc      ,
                  cbHomeMethod    ,
                  cbSrvOnLevelInv ,
                  edBreakAdd      );

    MT.UpdateUsrPara(cbMotor -> ItemIndex ,
                     true            ,
                     edACCTime       ,
                     edRunSpeed      ,
                     edManualSpeed   ,
                     edHomeSpeed     ,
                     edLastHomeSpeed ,
                     edJogSpeed      ,
                     edInposition    ,
                     edHomeOffset    );

    AllDynamicCountLoad();

    //TOWER LAMP
    TL.Load(true); // true 시 로드함요.
    TL.DisplayProperty(true , rbRedOn  , rbRedOff , rbRedFlick ,
                              rbYelOn  , rbYelOff , rbYelFlick ,
                              rbGrnOn  , rbGrnOff , rbGrnFlick ,
                              rbBuz1   , rbBuzMute ,
                              edRedAdd , edYelAdd , edGrnAdd   , edBuzzAdd ,
                              sgTower  );
    TL.DisplayList(sgTower);

    cbMotor -> DropDownCount = MT.GetMotorCnt() + 1;
}

void __fastcall TFrmDllMain::cbMotorChange(TObject *Sender)
{
    Trace(   MT.GetName(((TComboBox *)Sender)->ItemIndex).c_str() , "Selected");

    MT.UpdatePara(cbMotor -> ItemIndex ,
                  true            ,
                  edMotorName     ,
                  cbMotionType    ,
                  edPulsePerRev   ,
                  edUnitPerRev    ,
                  edMaxPos        ,
                  edMinPos        ,
                  cbExistEnc      ,
                  cbHomeMethod    ,
                  cbSrvOnLevelInv ,
                  edBreakAdd      );

    MT.UpdateUsrPara(cbMotor -> ItemIndex ,
                     true            ,
                     edACCTime       ,
                     edRunSpeed      ,
                     edManualSpeed   ,
                     edHomeSpeed     ,
                     edLastHomeSpeed ,
                     edJogSpeed      ,
                     edInposition    ,
                     edHomeOffset    );
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    AnsiString sTemp = "" ;

    static int         PreLevel   = -1 ;
    static AnsiString  PreCaption = "" ;

    LvUpdate();

    switch(pcMain -> ActivePageIndex) {
        default:
            break ;

        case 1 : //IO/.
            IO.UpdateStatus(cbIO -> ItemIndex,sgIO ) ;
            break ;

        case 2 :
            TL.DisplayStatus(lbRed , lbYel , lbGrn , pnBuzz1) ;

        case 3 : //Actuator.
            AT.DisplayStatus(sgActuator-> Row - 1 ,  lbATFw , lbATBw , lbATAlarm) ;
            break ;



        case 4 : //Motor
            MT.UpdateStat(cbMotor -> ItemIndex ,
                          lbServoOn            ,
                          lbHomeSnsr           ,
                          lbPEndLimSnsr        ,
                          lbNEndLimSnsr        ,
                          lbHomeEnded          ,
                          lbAlarm              ,

                          lbPackInPosn         ,
                          lbStop               ,
                          lbReady              ,
                          lbOk                 ,
                          lbJogP               ,
                          lbJogN               ,
                          lbTrgPos             ,
                          lbCmdPos             ,
                          lbEncPos             ,
                          lbZphase             ,
                          lbBreakOff           );

            btBreakOff -> Caption = MT.GetBreakOff(cbMotor -> ItemIndex) ? "ON" : "OFF" ;

            break ;
    }

    tmUpdate -> Enabled = true ;


}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btSaveMotorClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    btSaveMotor->Enabled = false;
    int iItemIndex;
    MT.UpdatePara(cbMotor -> ItemIndex ,
                  false           ,
                  edMotorName     ,
                  cbMotionType    ,
                  edPulsePerRev   ,
                  edUnitPerRev    ,
                  edMaxPos        ,
                  edMinPos        ,
                  cbExistEnc      ,
                  cbHomeMethod    ,
                  cbSrvOnLevelInv ,
                  edBreakAdd      );

    MT.UpdateUsrPara(cbMotor -> ItemIndex ,
                     false           ,
                     edACCTime       ,
                     edRunSpeed      ,
                     edManualSpeed   ,
                     edHomeSpeed     ,
                     edLastHomeSpeed ,
                     edJogSpeed      ,
                     edInposition    ,
                     edHomeOffset    );

    MT.Save(cbMotor -> ItemIndex);

    //ComboBox Motor Select
    iItemIndex = cbMotor -> ItemIndex;
    cbMotor -> Clear();
    MT.GetMotorList(cbMotor) ;
    cbMotor -> ItemIndex = iItemIndex ;
    btSaveMotor->Enabled = true;

/*    MT.UpdatePara(cbMotor -> ItemIndex ,
                  true            ,
                  edMotorName     ,
                  cbMotionType    ,
                  edPulsePerRev   ,
                  edUnitPerRev    ,
                  edMaxPos        ,
                  edMinPos        ,
                  cbExistEnc      ,
                  cbHomeWithNSnsr ,
                  cbSrvOnLevelInv ,
                  edBreakAdd      );

    MT.UpdateUsrPara(cbMotor -> ItemIndex ,
                     true          ,
                     edACCTime     ,
                     edRunSpeed    ,
                     edManualSpeed ,
                     edHomeSpeed   ,
                     edJogSpeed    ,
                     edInposition  ,
                     edHomeOffset  );
*/
}

//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::cbIOChange(TObject *Sender)
{
    IO.DisplayList(cbIO -> ItemIndex , sgIO , true );
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::sgIODrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
    AnsiString sItem;
    if(ACol == 7 || ACol == 8)
    {
        sItem = sgIO->Cells[ACol][ARow];
        if(sItem != "")
        {
            if(sItem == "ON"  )    sgIO->Canvas->Brush->Color = clGreen;
            if(sItem == "OFF" )    sgIO->Canvas->Brush->Color = clRed  ;
            sgIO->Canvas->FillRect(Rect);
            sgIO->Canvas->TextOut(Rect.Left + 3, Rect.Top + (Rect.Height() - sgIO->Canvas->TextHeight(sItem)) / 2, sItem);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::sgIODblClick(TObject *Sender)
{
    if(!cbIO -> ItemIndex)  return;

    IO.SetY(sgIO -> Row -1 , !IO.GetY(sgIO -> Row -1));
}
//---------------------------------------------------------------------------


void __fastcall TFrmDllMain::btBwdClick(TObject *Sender)
{

    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    AT.Move(sgActuator -> Row - 1 , ccBwd );
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btFwdClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    AT.Move(sgActuator -> Row - 1 , ccFwd );
}
//---------------------------------------------------------------------------



void __fastcall TFrmDllMain::btApplyActrClick(TObject *Sender)
{
     Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    //Display.
    AT.DisplayProperty(false ,
                       edActrName     , edActrComt     ,
                       edActrAddrIF   , edActrAddrIB   ,
                       edActrAddrOF   , edActrAddrOB   ,
                       edActrOnDelayF , edActrOnDelayB ,
                       edActrTimeOutB , edActrTimeOutF ,
                       sgActuator     );
    AT.DisplayList    (sgActuator);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btSaveActrClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    btSaveActr->Enabled = false;
    AT.Save();
    btSaveActr->Enabled = true;    
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::sgActuatorClick(TObject *Sender)
{
    AT.DisplayProperty(true , edActrName     , edActrComt     ,
                              edActrAddrIF   , edActrAddrIB   ,
                              edActrAddrOF   , edActrAddrOB   ,
                              edActrOnDelayF , edActrOnDelayB ,
                              edActrTimeOutB , edActrTimeOutF ,
                              sgActuator     );
}
//---------------------------------------------------------------------------


void __fastcall TFrmDllMain::sgActuatorDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    AnsiString sItem;
    if((ACol == 3 || ACol == 4 || ACol == 5 || ACol == 6) && ARow )
    {
        sItem = sgActuator->Cells[ACol][ARow];
        if(sItem != "")
        {
            if(sItem.ToIntDef(-1) >= 0) sgActuator->Canvas->Brush->Color = clMoneyGreen ;
            sgActuator->Canvas->FillRect(Rect);
            sgActuator->Canvas->TextOut(Rect.Left + 1, Rect.Top + (Rect.Height() - sgActuator->Canvas->TextHeight(sItem)) / 2, sItem);
        }
    }
}
//---------------------------------------------------------------------------



void __fastcall TFrmDllMain::btATResetClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    AT.Reset();
}
//---------------------------------------------------------------------------



void __fastcall TFrmDllMain::btJogNMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.JogN(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btJogNMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace(((TButton *)Sender)->Name.c_str() , "UnClecked");
    MT.Stop(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDllMain::btJogPMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.JogP(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btJogPMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace(((TButton *)Sender)->Name.c_str() , "UnClicked");
    MT.Stop(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btMtSvOnClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.SetServo(cbMotor -> ItemIndex , true) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btMtSvOffClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.SetServo(cbMotor -> ItemIndex , false) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btMtStopClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.Stop(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btStopClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.StopAll() ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmDllMain::btMtResetClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.Reset(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btTestClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.DoRpt(cbMotor -> ItemIndex, edFirstPos->Text.ToDouble(), edSecondPos->Text.ToDouble(), edStopDelay->Text.ToIntDef(1000) );
}
//---------------------------------------------------------------------------




void __fastcall TFrmDllMain::lbHomeEndedDblClick(TObject *Sender)
{
    MT.SetHomeEnd(cbMotor -> ItemIndex , !MT.GetHomeEnd(cbMotor -> ItemIndex));
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btMtHomeClick(TObject *Sender)
{
    MT.DoHome(cbMotor -> ItemIndex ) ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmDllMain::sgIOClick(TObject *Sender)
{
   //IO.DisplayProperty(true,c)
   IO.DisplayProperty(true,cbIO->ItemIndex , edIOIndex, edIOAdd , edIOHexAdd , edIOName , edIOComt , cbIOInverse , edIODelay , sgIO , cbHex -> Checked ) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmDllMain::btSaveIOClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    btSaveIO->Enabled = false;
    IO.Save() ;
    btSaveIO->Enabled = true;    
}
//---------------------------------------------------------------------------





void __fastcall TFrmDllMain::btApplyIOClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    IO.DisplayProperty(false,cbIO->ItemIndex , edIOIndex , edIOAdd , edIOHexAdd , edIOName , edIOComt , cbIOInverse , edIODelay , sgIO , cbHex -> Checked ) ;
    IO.DisplayList(cbIO -> ItemIndex , sgIO , true );
}
//---------------------------------------------------------------------------



void __fastcall TFrmDllMain::btMtClrPosClick(TObject *Sender)
{
    MT.SetPos(cbMotor -> ItemIndex , 0.0) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmDllMain::lbVerClick(TObject *Sender)
{
    FrmUpdate -> Show() ;
}

void __fastcall TFrmDllMain::pcMainChange(TObject *Sender)
{
    Trace("MainPage" , ("Changed To" + pcMain -> ActivePage -> Caption).c_str()  );
    cbIOTestMode -> Checked = false ;
    IO.SetTestMode(false) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btApplyErrClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    EM.DisplayProperty(false, sgErrList, edErrEnum , edErrNo, edErrName, edImgPath, mmErrAction, imErrPicture, spErrRect);
    EM.DisplayList(sgErrList);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btSaveErrClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    btSaveErr->Enabled = false;
    EM.Save();
    btSaveErr->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::BitBtn8Click(TObject *Sender)
{
    if(!dgOpenPictur->Execute())    return ;
    edImgPath->Text = dgOpenPictur->FileName;
    imErrPicture->Visible = true;
    imErrPicture->Picture->LoadFromFile(dgOpenPictur->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::sgErrListClick(TObject *Sender)
{
    EM.DisplayProperty(true, sgErrList, edErrEnum , edErrNo, edErrName, edImgPath, mmErrAction, imErrPicture, spErrRect);
}
//---------------------------------------------------------------------------
static EN_CLICK_POSITION iClickPos ;
static int iX , iY , iPreX , iPreY ;
static bool bClick = false ;
void __fastcall TFrmDllMain::spErrRectMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

    TShape * spRect = (TShape *)Sender ;
    int iLineWidth = spRect -> Pen -> Width ;
    TPoint CurPt ;
    GetCursorPos(&CurPt);
    iPreX = iX = CurPt.x ;
    iPreY = iY = CurPt.y ;
    if(Button == mbLeft) {
             if(X < iLineWidth                   && Y < iLineWidth                   ) iClickPos = cpLT   ;
        else if(X > spRect -> Width - iLineWidth && Y > spRect -> Height - iLineWidth) iClickPos = cpRB   ;
        else if(X < iLineWidth                   && Y > spRect -> Height - iLineWidth) iClickPos = cpLB   ;
        else if(X > spRect -> Width - iLineWidth && Y < iLineWidth                   ) iClickPos = cpRT   ;
        else if(X < iLineWidth                                                       ) iClickPos = cpL    ;
        else if(X > spRect -> Width - iLineWidth                                     ) iClickPos = cpR    ;
        else if(Y < iLineWidth                                                       ) iClickPos = cpT    ;
        else if(Y > spRect -> Height - iLineWidth                                    ) iClickPos = cpB    ;
        else                                                                           iClickPos = cpMove ;
    }
    bClick = true ;

}
//---------------------------------------------------------------------------


void __fastcall TFrmDllMain::spErrRectMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    TShape * spRect = (TShape *)Sender ;
    int iLineWidth = spRect -> Pen -> Width ;
    TPoint CurPt ;
    GetCursorPos(&CurPt);
    iX = CurPt.x ;
    iY = CurPt.y ;

         if(X < iLineWidth                   && Y < iLineWidth                   ) spErrRect->Cursor = crSizeNWSE ;
    else if(X > spRect -> Width - iLineWidth && Y > spRect -> Height - iLineWidth) spErrRect->Cursor = crSizeNWSE ;
    else if(X < iLineWidth                   && Y > spRect -> Height - iLineWidth) spErrRect->Cursor = crSizeNESW ;
    else if(X > spRect -> Width - iLineWidth && Y < iLineWidth                   ) spErrRect->Cursor = crSizeNESW ;
    else if(X < iLineWidth                                                       ) spErrRect->Cursor = crSizeWE   ;
    else if(X > spRect -> Width - iLineWidth                                     ) spErrRect->Cursor = crSizeWE   ;
    else if(Y < iLineWidth                                                       ) spErrRect->Cursor = crSizeNS   ;
    else if(Y > spRect -> Height - iLineWidth                                    ) spErrRect->Cursor = crSizeNS   ;
    else                                                                           spErrRect->Cursor = crSizeAll  ;


    if(!bClick) return ;

    int iGapX = iX - iPreX ;
    int iGapY = iY - iPreY ;

    iPreX = iX ;
    iPreY = iY ;


    switch (iClickPos) {
        default     :                                                                                                           break ;
        case cpLT   : spRect -> Left += iGapX ; spRect -> Width -= iGapX ; spRect -> Top += iGapY ; spRect -> Height -= iGapY ; break ;
        case cpRB   :                           spRect -> Width += iGapX ;                          spRect -> Height += iGapY ; break ;
        case cpLB   : spRect -> Left += iGapX ; spRect -> Width -= iGapX ;                          spRect -> Height += iGapY ; break ;
        case cpRT   :                           spRect -> Width += iGapX ; spRect -> Top += iGapY ; spRect -> Height += iGapY ; break ;
        case cpL    : spRect -> Left += iGapX ; spRect -> Width -= iGapX ;                                                      break ;
        case cpR    :                           spRect -> Width += iGapX ;                                                      break ;
        case cpT    :                                                      spRect -> Top += iGapY ; spRect -> Height -= iGapY ; break ;
        case cpB    :                                                      spRect -> Top += iGapY ; spRect -> Height -= iGapY ; break ;
        case cpMove : spRect -> Left += iGapX ;                            spRect -> Top += iGapY ;                             break ;
    }
}

//---------------------------------------------------------------------------
void __fastcall TFrmDllMain::spErrRectMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    bClick = false ;

    TShape * spRect = (TShape *)Sender ;

    int iLeft , iTop , iWidth , iHeight ;

    if( spRect -> Width  < 0 ) {iLeft = spRect -> Left + spRect -> Width  ; iWidth  = spRect -> Width  * (-1) ; spRect -> Left = iLeft ; spRect -> Width  = iWidth  ;}
    if( spRect -> Height < 0 ) {iTop  = spRect -> Top  + spRect -> Height ; iHeight = spRect -> Height * (-1) ; spRect -> Top  = iTop  ; spRect -> Height = iHeight ;}


    spErrRect->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDllMain::SetCursor(TShape *spErrRect, int X, int Y )
{
    if (spErrRect == NULL) return;
         if((X < 10 && Y < 10) || (X > spErrRect->Width-10 && Y > spErrRect->Height-10)) spErrRect->Cursor = crSizeNWSE;
    else if((X < 10 && Y > spErrRect->Height-10) || (X > spErrRect->Width-10 && Y < 10)) spErrRect->Cursor = crSizeNESW;
    else                                                                                 spErrRect->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDllMain::FormDestroy(TObject *Sender)
{
//    DeleteUI();
//    delete FrmMsgYesNo;   FrmMsgYesNo = NULL;
//    delete FrmMsgOk   ;   FrmMsgOk    = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDllMain::imErrPictureDblClick(TObject *Sender)
{
    MsgYesNo("CONFIRM","세부 위치 지정 사각형을 초기화 하시겠습니까?");
    if(FrmMsgYesNo->ModalResult == mrYes) {
        spErrRect->Top   = 1;
        spErrRect->Left  = 1;
        spErrRect->Width = 60;
        spErrRect->Height= 60;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDllMain::btAllCountSaveClick(TObject *Sender)
{
    MsgYesNo("CONFIRM","모든 COUNT 값을 저장 하시겠습니까? \n (적용하려면 프로그램을 다시 실행 시켜 주세요.)");
    if(FrmMsgYesNo->ModalResult == mrYes) {
        AllDynamicCountSave();
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDllMain::AllDynamicCountSave()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Save(sPath , "CErrMan"    , "m_iMaxErr"   , edErrorCount->Text);
    UserINI.Save(sPath , "CMotors"    , "m_iMaxMotor" , edMotorCount->Text);
    UserINI.Save(sPath , "CIOs"       , "m_iMaxIn"    , edIoInputCount->Text);
    UserINI.Save(sPath , "CIOs"       , "m_iMaxOut"   , edIoOutputCount->Text);
    UserINI.Save(sPath , "CActuators" , "m_iMaxAct"   , deActuatorCount->Text);
}
//---------------------------------------------------------------------------
void __fastcall TFrmDllMain::AllDynamicCountLoad()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    int iCount;
    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Load(sPath , "CErrMan"    , "m_iMaxErr"   , iCount);    edErrorCount->Text    = iCount;
    UserINI.Load(sPath , "CMotors"    , "m_iMaxMotor" , iCount);    edMotorCount->Text    = iCount;
    UserINI.Load(sPath , "CIOs"       , "m_iMaxIn"    , iCount);    edIoInputCount->Text  = iCount;
    UserINI.Load(sPath , "CIOs"       , "m_iMaxOut"   , iCount);    edIoOutputCount->Text = iCount;
    UserINI.Load(sPath , "CActuators" , "m_iMaxAct"   , iCount);    deActuatorCount->Text = iCount;
}
//---------------------------------------------------------------------------


void __fastcall TFrmDllMain::btActrRptClick(TObject *Sender)
{
    if(cbActrSync -> Checked) AT.GoRpt(edRptDelay -> Text.ToIntDef(1000), sgActuator -> Row -1, edActrSync -> Text.ToIntDef(-1));
    else                      AT.GoRpt(edRptDelay -> Text.ToIntDef(1000), sgActuator -> Row -1                                 );
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::cbActrSyncClick(TObject *Sender)
{
    edActrSync -> Enabled = cbActrSync -> Checked ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btActrStopClick(TObject *Sender)
{
    AT.StopRpt();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::sgTowerClick(TObject *Sender)
{
    TL.DisplayProperty(true , rbRedOn  , rbRedOff , rbRedFlick ,
                              rbYelOn  , rbYelOff , rbYelFlick ,
                              rbGrnOn  , rbGrnOff , rbGrnFlick ,
                              rbBuz1   , rbBuzMute ,
                              edRedAdd , edYelAdd , edGrnAdd   , edBuzzAdd ,
                              sgTower  );
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btUpdateClick(TObject *Sender)
{
    TL.DisplayProperty(false , rbRedOn  , rbRedOff , rbRedFlick ,
                                      rbYelOn  , rbYelOff , rbYelFlick ,
                                      rbGrnOn  , rbGrnOff , rbGrnFlick ,
                                      rbBuz1   , rbBuzMute ,
                                      edRedAdd , edYelAdd , edGrnAdd   , edBuzzAdd ,
                                      sgTower );
    TL.DisplayList(sgTower);
    
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btSaveClick(TObject *Sender)
{
    btSave -> Enabled = false ;
    TL.Load(false); //void __fastcall CTowerLamp::Load(bool IsLoad) true 시 로드함.
    btSave -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::cbTestModeClick(TObject *Sender)
{
    TL.SetTestMode(cbTestMode-> Checked) ;
}
//---------------------------------------------------------------------------



void __fastcall TFrmDllMain::LvUpdate()
{
    int iLevel;
//    FrmPassword->GetLevel();
    iLevel = FM_GetLevel();
    switch (iLevel) {
        case lvOperator :
            btApplyErr     ->Enabled = false ;
            btSaveErr      ->Enabled = false ;
            btApplyIO      ->Enabled = false ;
            btSaveIO       ->Enabled = false ;
            btUpdate       ->Enabled = false ;
            btSave         ->Enabled = false ;
            btApplyActr    ->Enabled = false ;
            btSaveActr     ->Enabled = false ;
            btSaveMotor    ->Enabled = false ;
            btAllCountSave ->Enabled = false ;
            break ;
        case lvEngineer :
            btApplyErr     ->Enabled = true  ;
            btSaveErr      ->Enabled = true  ;
            btApplyIO      ->Enabled = false ;
            btSaveIO       ->Enabled = false ;
            btUpdate       ->Enabled = true  ;
            btSave         ->Enabled = true  ;
            btApplyActr    ->Enabled = false ;
            btSaveActr     ->Enabled = false ;
            btSaveMotor    ->Enabled = false ;
            btAllCountSave ->Enabled = false ;
            break ;
        case lvMaster   :
            btApplyErr     ->Enabled = true  ;
            btSaveErr      ->Enabled = true  ;
            btApplyIO      ->Enabled = true  ;
            btSaveIO       ->Enabled = true  ;
            btUpdate       ->Enabled = true  ;
            btSave         ->Enabled = true  ;
            btApplyActr    ->Enabled = true  ;
            btSaveActr     ->Enabled = true  ;
            btSaveMotor    ->Enabled = true  ;
            btAllCountSave ->Enabled = true  ;
            break ;
        default         :
            btApplyErr     ->Enabled = false ;
            btSaveErr      ->Enabled = false ;
            btApplyIO      ->Enabled = false ;
            btSaveIO       ->Enabled = false ;
            btUpdate       ->Enabled = false ;
            btSave         ->Enabled = false ;
            btApplyActr    ->Enabled = false ;
            btSaveActr     ->Enabled = false ;
            btSaveMotor    ->Enabled = false ;
            btAllCountSave ->Enabled = false ;
            break ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btBreakOffClick(TObject *Sender)
{
    if(MT.GetBreakOff(cbMotor -> ItemIndex) && MT.GetServo(cbMotor -> ItemIndex)) {MsgOk("ERROR","Can't Break When Servo On"); return ;}
    MT.SetBreakOff(cbMotor -> ItemIndex , !MT.GetBreakOff(cbMotor -> ItemIndex)) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmDllMain::pnErrImgClick(TObject *Sender)
{
    if(MsgYesNo("CONFIRM","세부 위치 지정 사각형을 초기화 하시겠습니까?") != mrYes) return ;

    spErrRect->Top   = 1;
    spErrRect->Left  = 1;
    spErrRect->Width = 60;
    spErrRect->Height= 60;

}
//---------------------------------------------------------------------------






void __fastcall TFrmDllMain::btAllHomeClick(TObject *Sender)
{
    if(MsgYesNo("Confirm","This commend does not consider \n any collision or interference")!= mrYes) return ;

    for(int i = 0 ; i < MT.GetMotorCnt() ; i++) {
        MT.DoHome(i) ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::cbIOTestModeClick(TObject *Sender)
{
    IO.SetTestMode(cbIOTestMode -> Checked) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btAllSrvOnClick(TObject *Sender)
{
    MT.SetServoAll(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btAllSrvOffClick(TObject *Sender)
{
    MT.SetServoAll(false);
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::FormHide(TObject *Sender)
{
    cbIOTestMode -> Checked = false ;
    IO.SetTestMode(false) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btStopAllClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.StopAll();
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btFirstGoClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.GoAbsRun(cbMotor -> ItemIndex , edFirstPos->Text.ToDouble());
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllMain::btScndGoClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.GoAbsRun(cbMotor -> ItemIndex , edSecondPos->Text.ToDouble());        
}
//---------------------------------------------------------------------------

