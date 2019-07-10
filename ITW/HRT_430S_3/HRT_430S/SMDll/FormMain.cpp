//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <jpeg.hpp>
#include "FormMain.h"
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


TFrmMain *FrmMain;
//---------------------------------------------------------------------------
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
        : TForm(Owner)
{
//    // 에러 이벤트 설정
//    Application->OnException = ExceptionFilter;
}

//---------------------------------------------------------------------------
AnsiString TFrmMain::OnGetVer(const AnsiString &sQuery)
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


void __fastcall TFrmMain::FormCreate(TObject *Sender)
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
    EM.DisplayProperty(true, sgErrList, edErrNo, edErrName, edImgPath, mmErrAction, imErrPicture, spErrRect);

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
                  cbHomeWithNSnsr ,
                  cbSrvOnLevelInv ,
                  true            );

    MT.UpdateUsrPara(cbMotor -> ItemIndex ,
                     true          ,
                     edACCTime     ,
                     edRunSpeed    ,
                     edManualSpeed ,
                     edHomeSpeed   ,
                     edJogSpeed    ,
                     edInposition  ,
                     edHomeOffset  );

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
}

void __fastcall TFrmMain::cbMotorChange(TObject *Sender)
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
                  cbHomeWithNSnsr ,
                  cbSrvOnLevelInv ,
                  true            );

    MT.UpdateUsrPara(cbMotor -> ItemIndex ,
                     true          ,
                     edACCTime     ,
                     edRunSpeed    ,
                     edManualSpeed ,
                     edHomeSpeed   ,
                     edJogSpeed    ,
                     edInposition  ,
                     edHomeOffset  );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    AnsiString sTemp = "" ;

    static int         PreLevel   = -1 ;
    static AnsiString  PreCaption = "" ;

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
                          lbEncPos             );   
            break ;
    }

    tmUpdate -> Enabled = true ;


}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btSaveMotorClick(TObject *Sender)
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
                  cbHomeWithNSnsr ,
                  cbSrvOnLevelInv ,
                  false            );

    MT.UpdateUsrPara(cbMotor -> ItemIndex ,
                     false         ,
                     edACCTime     ,
                     edRunSpeed    ,
                     edManualSpeed ,
                     edHomeSpeed   ,
                     edJogSpeed    ,
                     edInposition  ,
                     edHomeOffset  );

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
                  false            );

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

void __fastcall TFrmMain::cbIOChange(TObject *Sender)
{
    IO.DisplayList(cbIO -> ItemIndex , sgIO , true );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::sgIODrawCell(TObject *Sender, int ACol, int ARow,
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

void __fastcall TFrmMain::sgIODblClick(TObject *Sender)
{
    if(!cbIO -> ItemIndex)  return;

    IO.SetY(sgIO -> Row -1 , !IO.GetY(sgIO -> Row -1));
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btBwdClick(TObject *Sender)
{

    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    AT.Move(sgActuator -> Row - 1 , ccBwd );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btFwdClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    AT.Move(sgActuator -> Row - 1 , ccFwd );
}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::btApplyActrClick(TObject *Sender)
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

void __fastcall TFrmMain::btSaveActrClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    btSaveActr->Enabled = false;
    AT.Save();
    btSaveActr->Enabled = true;    
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::sgActuatorClick(TObject *Sender)
{
    AT.DisplayProperty(true , edActrName     , edActrComt     ,
                              edActrAddrIF   , edActrAddrIB   ,
                              edActrAddrOF   , edActrAddrOB   ,
                              edActrOnDelayF , edActrOnDelayB ,
                              edActrTimeOutB , edActrTimeOutF ,
                              sgActuator     );
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::sgActuatorDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    AnsiString sItem;
    if((ACol == 3 || ACol == 4 || ACol == 5 || ACol == 6) && ARow )
    {
        sItem = sgActuator->Cells[ACol][ARow];
        if(sItem != "")
        {
            if(sItem != "X0000" && sItem != "Y0000" ) sgActuator->Canvas->Brush->Color = clMoneyGreen ;
            sgActuator->Canvas->FillRect(Rect);
            sgActuator->Canvas->TextOut(Rect.Left + 1, Rect.Top + (Rect.Height() - sgActuator->Canvas->TextHeight(sItem)) / 2, sItem);
        }
    }
}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::btATResetClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    AT.Reset();
}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::btJogNMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.JogN(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btJogNMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace(((TButton *)Sender)->Name.c_str() , "UnClecked");
    MT.Stop(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btJogPMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.JogP(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btJogPMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace(((TButton *)Sender)->Name.c_str() , "UnClicked");
    MT.Stop(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btMtSvOnClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.SetServo(cbMotor -> ItemIndex , true) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btMtSvOffClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.SetServo(cbMotor -> ItemIndex , false) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btMtStopClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.Stop(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btStopClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.Stop(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btMtResetClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.Reset(cbMotor -> ItemIndex) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btTestClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    MT.DoRpt(cbMotor -> ItemIndex, edFirstPos->Text.ToDouble(), edSecondPos->Text.ToDouble(), edStopDelay->Text.ToIntDef(1000) );
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::lbHomeEndedDblClick(TObject *Sender)
{
    MT.SetHomeEnd(cbMotor -> ItemIndex , !MT.GetHomeEnd(cbMotor -> ItemIndex));
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btMtHomeClick(TObject *Sender)
{
    MT.DoHome(cbMotor -> ItemIndex ) ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::sgIOClick(TObject *Sender)
{
   //IO.DisplayProperty(true,c)
   IO.DisplayProperty(true,cbIO->ItemIndex , edIOIndex, edIOAdd , edIOHexAdd , edIOName , edIOComt , cbIOInverse , edIODelay , sgIO , cbHex -> Checked ) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btSaveIOClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    btSaveIO->Enabled = false;
    IO.Save() ;
    btSaveIO->Enabled = true;    
}
//---------------------------------------------------------------------------





void __fastcall TFrmMain::btApplyIOClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    IO.DisplayProperty(false,cbIO->ItemIndex , edIOIndex , edIOAdd , edIOHexAdd , edIOName , edIOComt , cbIOInverse , edIODelay , sgIO , cbHex -> Checked ) ;
    IO.DisplayList(cbIO -> ItemIndex , sgIO , true );
//void DisplayList    (bool _bOutput , TStringGrid * _sgTable , bool _bFixedWidth = false , bool _bDispHexAdd = false );
}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::btMtClrPosClick(TObject *Sender)
{
    MT.SetPos(cbMotor -> ItemIndex , 0.0) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::lbVerClick(TObject *Sender)
{
    FrmUpdate -> Show() ;
}

void __fastcall TFrmMain::pcMainChange(TObject *Sender)
{
    Trace("MainPage" , ("Changed To" + pcMain -> ActivePage -> Caption).c_str()  );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btApplyErrClick(TObject *Sender)
{
//int iLeft  = pTracker->Left,
//    iTop   = pTracker->Top,
//    iWidth = pTracker->Width,
//    iHeight= pTracker->Height;

    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    EM.DisplayProperty(false, sgErrList, edErrNo, edErrName, edImgPath, mmErrAction, imErrPicture, spErrRect);
    EM.DisplayList(sgErrList);
    TRect tRect;
    tRect = EM.GetTRect(sgErrList);

//    pTracker->Left    = tRect.Left;
//    pTracker->Top     = tRect.Top;
//    pTracker->Width   = tRect.Right;
//    pTracker->Height  = tRect.Bottom;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btSaveErrClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    btSaveErr->Enabled = false;
    EM.Save();
    btSaveErr->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::BitBtn8Click(TObject *Sender)
{
    if(!dgOpenPictur->Execute())    return ;
    edImgPath->Text = dgOpenPictur->FileName;
    imErrPicture->Visible = true;
    imErrPicture->Picture->LoadFromFile(dgOpenPictur->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::sgErrListClick(TObject *Sender)
{
    EM.DisplayProperty(true, sgErrList, edErrNo, edErrName, edImgPath, mmErrAction, imErrPicture, spErrRect);
//    EM.DisplayList(sgErrList);
//    TRect tRect;
//    tRect = EM.GetTRect(sgErrList);

//    pTracker->Left    = tRect.Left;
//    pTracker->Top     = tRect.Top;
//    pTracker->Width   = tRect.Right;
//    pTracker->Height  = tRect.Bottom;

}
//---------------------------------------------------------------------------
static TReszNMvPos StartPos1;
void __fastcall TFrmMain::spErrRectMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
//      ERR.RectMouseDown(spErrRect,Button, Shift, X, Y);
    if(Button == mbLeft)
     //  Shift.Contains(ssLeft))
     {
        GetCursorPos(&PrePt);  //현재 마우스 좌표를 저장
        StartPos1 = GetResizePos(spErrRect, X, Y );
     }

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::spErrRectMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
//    ERR.RectMouseMove(spErrRect,Shift, X, Y);
/*    iLeftPos  = spErrRect->Left   ;
    iTopPos   = spErrRect->Top    ;
    iWidth    = spErrRect->Width  ;
    iHeight   = spErrRect->Height ;*/

    if (spErrRect == NULL) return;

    GetCursorPos(&CurPt); //마우스 움직인 후의 현재 마우스 가져옴

    SetCursor(spErrRect, X, Y );
    if( Shift.Contains(ssLeft) ){
        switch( StartPos1 ){
            case RESIZ_LTOP:{
                spErrRect -> Left   = spErrRect -> Left   + (CurPt.x-PrePt.x);
//                Label43 -> Caption =                 spErrRect -> Left;
                if(spErrRect -> Left  <= 0 ) spErrRect -> Left   = 0 ;
                else spErrRect -> Width  = spErrRect -> Width  - (CurPt.x-PrePt.x);
                spErrRect -> Top    = spErrRect -> Top    + (CurPt.y-PrePt.y);
//                Label46 -> Caption =                 spErrRect -> Top   ;
                if(spErrRect -> Top  <= 0 ) spErrRect -> Top   = 0 ;
                else spErrRect -> Height = spErrRect -> Height - (CurPt.y-PrePt.y);
//                Label47 -> Caption = spErrRect -> Width;
                if(spErrRect -> Width <= 10) spErrRect -> Width  = 40;

//                Label48 -> Caption = spErrRect -> Height;
                if(spErrRect -> Height <= 10) spErrRect -> Height  = 40;
            }break;
            case RESIZ_RBTM:{


//                Label49 -> Caption = spErrRect -> Width;
                if(spErrRect -> Left + spErrRect -> Width >= 636) spErrRect -> Width = 636 - spErrRect -> Left;
                spErrRect -> Width  = spErrRect -> Width  + (CurPt.x-PrePt.x);
                if(spErrRect -> Width <= 10) spErrRect -> Width  = 40;

                if(spErrRect -> Top + spErrRect -> Height >= 306) spErrRect -> Height = 306 - spErrRect -> Top;
                spErrRect -> Height = spErrRect -> Height + (CurPt.y-PrePt.y);
                //                Label50 -> Caption = spErrRect -> Height;
                if(spErrRect -> Height <= 10) spErrRect -> Height  = 40;
            } break;
            case RESIZ_RTOP:{
                spErrRect -> Top    = spErrRect -> Top    + (CurPt.y-PrePt.y);
//                Label51-> Caption = spErrRect -> Top;
                if(spErrRect -> Top  <= 0 ) spErrRect -> Top   = 0 ;
                else                 spErrRect -> Height = spErrRect -> Height - (CurPt.y-PrePt.y);

//                Label52 -> Caption = spErrRect -> Width;
                if(spErrRect -> Left + spErrRect -> Width >= 636) spErrRect -> Width = 636 - spErrRect -> Left;
                spErrRect -> Width  = spErrRect -> Width  + (CurPt.x-PrePt.x);

                if(spErrRect -> Width <= 10) spErrRect -> Width  = 40;

//                Label53 -> Caption = spErrRect -> Height;
                if(spErrRect -> Height <= 10) spErrRect -> Height  = 40;
            } break;
            case RESIZ_LBTM:{
                spErrRect -> Left   = spErrRect -> Left   + (CurPt.x-PrePt.x);
//                Label54 -> Caption = spErrRect -> Left;
                if(spErrRect -> Left  <= 0 ) spErrRect -> Left   = 0 ;
                else spErrRect -> Width  = spErrRect -> Width  - (CurPt.x-PrePt.x);
//                Label55 -> Caption = spErrRect -> Width;
                if(spErrRect -> Width <= 10) spErrRect -> Width  = 40;

                if(spErrRect -> Top + spErrRect -> Height >= 306) spErrRect -> Height = 306 - spErrRect -> Top;
                                 spErrRect -> Height = spErrRect -> Height + (CurPt.y-PrePt.y);
                //                Label56 -> Caption = spErrRect -> Height;
                if(spErrRect -> Height <= 10) spErrRect -> Height  = 40;
            } break;
            case MOVE_SHAPE:{
                spErrRect -> Left   = spErrRect -> Left   + (CurPt.x-PrePt.x);
//                Label57 -> Caption = spErrRect -> Left;
                if(spErrRect -> Left <= 0) spErrRect -> Left = 0;
                if(spErrRect -> Left + spErrRect -> Width >= 636) spErrRect -> Left = 636 - spErrRect -> Width;
                spErrRect -> Top    = spErrRect -> Top    + (CurPt.y-PrePt.y);
//                Label58 -> Caption = spErrRect -> Top;
                if(spErrRect -> Top <= 0) spErrRect -> Top = 0;
                if(spErrRect -> Top + spErrRect -> Height >= 306) spErrRect -> Top = 306 - spErrRect -> Height;
            } break;
        }

    }
    PrePt=CurPt; // 현재 좌표를 이전좌표로 기억해둠
}
//---------------------------------------------------------------------------
int __fastcall TFrmMain::GetResizePos(TShape *spErrRect,int X, int Y)
{
    int range = 15;
    int Width  ;
    int Height ;

    Width = spErrRect->Width;
    Height= spErrRect->Height;

    TReszNMvPos rmPos;
    if( X <         range && Y <          range ) rmPos = RESIZ_LTOP; else
    if( X > Width - range && Y > Height - range ) rmPos = RESIZ_RBTM; else
    if( X > Width - range && Y <          range ) rmPos = RESIZ_RTOP; else
    if( X <         range && Y > Height - range ) rmPos = RESIZ_LBTM;

    if( X >         range && X < Width  - range &&
        Y >         range && Y < Height - range ) rmPos = MOVE_SHAPE;

    return rmPos;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::spErrRectMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
//    ERR.RectMouseUp(spErrRect,Button, Shift, X, Y);
    spErrRect->Cursor = crDefault	;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::SetCursor(TShape *spErrRect, int X, int Y )
{
    if (spErrRect == NULL) return;
         if((X < 10 && Y < 10) || (X > spErrRect->Width-10 && Y > spErrRect->Height-10)) spErrRect->Cursor = crSizeNWSE;
    else if((X < 10 && Y > spErrRect->Height-10) || (X > spErrRect->Width-10 && Y < 10)) spErrRect->Cursor = crSizeNESW;
    else                                                                                 spErrRect->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
//    DeleteUI();
    delete FrmMsgYesNo;   FrmMsgYesNo = NULL;
    delete FrmMsgOk   ;   FrmMsgOk    = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::imErrPictureDblClick(TObject *Sender)
{
    MsgYesNo("CONFIRM","세부 위치 지정 사각형을 초기화 하시겠습니까?");
    if(FrmMsgYesNo->ModalResult == mrYes)
    {
        spErrRect->Top   = 1;
        spErrRect->Left  = 1;
        spErrRect->Width = 60;
        spErrRect->Height= 60;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::btAllCountSaveClick(TObject *Sender)
{
    MsgYesNo("CONFIRM","모든 COUNT 값을 저장 하시겠습니까? \n (적용하려면 프로그램을 다시 실행 시켜 주세요.)");
    if(FrmMsgYesNo->ModalResult == mrYes)
    {
        AllDynamicCountSave();
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::AllDynamicCountSave()
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
void __fastcall TFrmMain::AllDynamicCountLoad()
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


void __fastcall TFrmMain::btActrRptClick(TObject *Sender)
{
    if(cbActrSync -> Checked) AT.GoRpt(edRptDelay -> Text.ToIntDef(1000), sgActuator -> Row -1, edActrSync -> Text.ToIntDef(-1));
    else                      AT.GoRpt(edRptDelay -> Text.ToIntDef(1000), sgActuator -> Row -1                                 );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::cbActrSyncClick(TObject *Sender)
{
    edActrSync -> Enabled = cbActrSync -> Checked ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btActrStopClick(TObject *Sender)
{
    AT.StopRpt();    
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::sgTowerClick(TObject *Sender)
{
    TL.DisplayProperty(true , rbRedOn  , rbRedOff , rbRedFlick ,
                              rbYelOn  , rbYelOff , rbYelFlick ,
                              rbGrnOn  , rbGrnOff , rbGrnFlick ,
                              rbBuz1   , rbBuzMute ,
                              edRedAdd , edYelAdd , edGrnAdd   , edBuzzAdd ,
                              sgTower  );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btUpdateClick(TObject *Sender)
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

void __fastcall TFrmMain::btSaveClick(TObject *Sender)
{
    btSave -> Enabled = false ;
    TL.Load(false); //void __fastcall CTowerLamp::Load(bool IsLoad) true 시 로드함.
    btSave -> Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::cbTestModeClick(TObject *Sender)
{
    TL.SetTestMode(cbTestMode-> Checked) ;
}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::LvUpdate()
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

