//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
#include "FormPassword.h"
#include "FormErrList.h"
#include "FormDisplay.h"
#include "FormMode.h"
#include "PstnMan.h"
#include "Actuators.h"
#include "OptionMan.h"
#include "IOs.h"
#include "Motors.h"
#include "UtilEnums.h"
#include "TEdit.h"
#include "ErrMan.h"
#include "Sequence.h"
#include "FormMsgOk.h"
#include "FormMsgYesNo.h"
#include "LogUnit.h"
#include "UserIni.h"


#pragma package(smart_init)
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


bool bBarcodeScan2 = false ;

TFrmMain *FrmMain;
//---------------------------------------------------------------------------
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
        : TForm(Owner)
{
    SEQ.Init();

   // SEQ.LT.SetFuncAPCMsg (OnSendAPCMsg ) ; SEQ.RT.SetFuncAPCMsg (OnSendAPCMsg ) ;
   // SEQ.LT.SetFuncVisnMsg(&OnSendVisnMsg) ; SEQ.RT.SetFuncVisnMsg(&OnSendVisnMsg) ;
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


#define MUTEX_MOTION "Motion Mutex"
#define WIN_NAME  "HVSystemMain"
struct
{
   HANDLE  hMutex;
   HWND    Handle;
   int     cmd;
   char    Data[1024];
} m;

void __stdcall TFrmMain::OnSendVisnMsg(EN_MOTION_VISION_CMD cmd,  char * data)
{

/*
Motion -> Vision
(MC01)             //검사.
(MC02)             //AutoMode
(MC03)             //TestMode
(MC04)<>           //ERR
(MC05)             //LEFT Table
(MC06)             //Right Table
(MC07)             //본딩후 ReInspection (비젼쪽에 GT Shift 스펙을 다르게 가져감.
(MC08)             //Gt Up후 3번째 Inspection (비젼쪽에 GT Shift 스펙을 다르게 가져감.
*/
   int iCmd ;

        if(cmd == mvInsp  ) {iCmd = 1 ; bShiftNG  = bHorNG = bBeamNG = true ; iAngleNG = 1; bSizeNG = true;}
   else if(cmd == mvAuto  )  iCmd = 2 ;
   else if(cmd == mvTest  )  iCmd = 3 ;
   else if(cmd == mvMsg   )  iCmd = 4 ;
   else if(cmd == mvLT    )  iCmd = 5 ;
   else if(cmd == mvRT    )  iCmd = 6 ;
   else if(cmd == mvReInsp)  iCmd = 7 ;
   else if(cmd == mv3Insp )  iCmd = 8 ;
   else                      return   ;


   AnsiString sCmd  = "" ;
   AnsiString sData = "" ;
   sCmd.printf("(MC%02d)" , iCmd );

   if(data) {
       sData = data    ;
       if(sData.Length())sData = "<" + sData + ">" ;
   }

   static int iLastLogTick = GetTickCount();

   if( m.hMutex == NULL ) {
      m.hMutex = CreateMutex(NULL, FALSE, MUTEX_MOTION);
      if( m.hMutex == NULL) {
         m.hMutex = OpenMutex(MUTEX_ALL_ACCESS	, FALSE, MUTEX_MOTION);
         if( m.hMutex == NULL) {
            return;
         }
      }
   }

   char szBuffer[1024];

   memset ( szBuffer , 0 , 1024);

   wsprintf(szBuffer, "%s%s", sCmd.c_str(), sData.c_str());

   int nBuf = strlen(szBuffer);

   DWORD  dwError = GetLastError();
   COPYDATASTRUCT cds;
   cds.cbData = nBuf + 1;
   cds.lpData = (void *) szBuffer;

   if (m.Handle == 0) {
   	//m.Handle = FindWindow(WIN_NAME , NULL);
        m.Handle = FindWindow(NULL,WIN_NAME);
   }
   else if( GetTickCount() - iLastLogTick > 1000) {
	//m.Handle = FindWindow(WIN_NAME , NULL);
        m.Handle = FindWindow(NULL,WIN_NAME);
   }
                  //센드메세지 방식은 유실 되지 않는다.
   if (m.Handle) SendMessage(m.Handle, WM_COPYDATA, NULL, (LPARAM)&cds);

   Trace("Send To Vision",szBuffer);

   SetLastError(dwError);
   ReleaseMutex(m.hMutex);
}

void __fastcall TFrmMain::OnCopyDataEvent(TMessage& Msg)
{
/*
Vision -> Motion
(VC01)             //ALLHOME
(VC02)             //RESET
(VC03)             //CLOSE AP
(VC04)<좌표,좌표,....>  //검사결과.
(VC05)             //UI Show

(VC06)             //Test Mode On;
(VC07)             //Test Mode Off
(VC08)             //Left Table
(VC09)             //Right Table

*/
    COPYDATASTRUCT  *pCds;
    pCds = (PCOPYDATASTRUCT)Msg.LParam;

    if( pCds == NULL) return;

    static bool bLeftTable = false ;

    AnsiString str = (char*)pCds->lpData;

    AnsiString Temp2 ;
                                                                                                                           
    Trace("Receive From Vision",str.c_str());                                                                              
                                                                                                                           
    AnsiString Temp = str.SubString(1,6);

    int iTpAngle ; //임시 값...   //iAngleNG


//    switch(str.SubString(1,6)) {
//        case "(VC01)" :  break ;
//        case "(VC02)" :  break ;
//        case "(VC03)" :  break ;
//        case "(VC05)" :  break ;
//    }
//



         if(str.SubString(1,6) == "(VC01)") SEQ.ReqHome() ;    //All Home
    else if(str.SubString(1,6) == "(VC02)") SEQ.Reset  () ;    //Reset
    else if(str.SubString(1,6) == "(VC03)")     Close  () ;    //
    else if(str.SubString(1,6) == "(VC04)") {
        str = str.Delete(1,7) ; //좌표,좌표> 만남는다.
        lbVisnReq -> Caption = str ;                                               //가공전 순수데이터                    //비율로 가공된 장비용 데이터.
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; dRowAngleX     = StrToFloatDef(Temp,0) ; dAngleX     = -StrToFloatDef(Temp,0) * OM.DevOptn.dAgRes  ;
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; dRowAngleY     = StrToFloatDef(Temp,0) ; dAngleY     = -StrToFloatDef(Temp,0) * OM.DevOptn.dAgRes  ;
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ;                                          iTpAngle    =  Temp.ToIntDef(1)                           ;
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; dRowShift      = StrToFloatDef(Temp,0) ; dShift      =  StrToFloatDef(Temp,0) * OM.DevOptn.dSfRes  ;
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ;                                          bShiftNG    =  Temp.ToIntDef(1)                           ;
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; dRowVerGap     = StrToFloatDef(Temp,0) ; dVerGap     =  StrToFloatDef(Temp,0) * OM.DevOptn.dApcRes ;
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ;                                          bVerNG      =  Temp.ToIntDef(1)                           ;
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; dRowHorGap     = StrToFloatDef(Temp,0) ; dHorGap     =  StrToFloatDef(Temp,0) * OM.DevOptn.dApcRes ;
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ;                                          bHorNG      =  Temp.ToIntDef(1)                           ;

        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; dRowBeamX      = StrToFloatDef(Temp,0) ; 
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; dRowBeamY      = StrToFloatDef(Temp,0) ;
        Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ;                                          bBeamNG     =  Temp.ToIntDef(1)                           ;

        //정식으로 하려면 비젼쪽하고 핸들러에 바꿀게 많아서 야매로 처리.
        if(iTpAngle == 0) {iAngleNG = 0 ; bSizeNG = false ; }
        if(iTpAngle == 1) {iAngleNG = 1 ; bSizeNG = false ; }
        if(iTpAngle == 2) {iAngleNG = 2 ; bSizeNG = false ; }
        if(iTpAngle == 3) {iAngleNG = 0 ; bSizeNG = true  ; }


        //화면상에 완전 시커멋케 들어오면
        //NG는 들어오지만 서버 데이터가 전에껄로 들어와서 강제로
        //서버 데이터를 클리어 시킨다.
        if((iAngleNG && dRowAngleX == 0.0 && dRowAngleY ==0.0) && // 앵글검사 NG이고.
           (bHorNG   && dRowHorGap == 0.0                    ) ){  // 에이피시도 NG이면.
            //For Server Log [DVD]
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sAngle             = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sFA_R              = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sFA_T              = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sThree_Beam_Tilt   = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Size         = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sGT_Shift          = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sArea              = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sPeak_Ratio        = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sParallelism       = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sAbsolute_Center_X = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sAbsolute_Center_Y = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Center_X     = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Center_Y     = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sPeak_Center_X     = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sPeak_Center_Y     = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Shift        = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Shift_R      = "0.0"  ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Shift_T      = "0.0"  ;

            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sLD_Motor_Y        = Temp ;//handl
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sLD_MOtor_Z        = Temp ;
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sGTX_Motor_X       = Temp ;

            //For Server Log [COMMON]
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sJudge             = Temp ;
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sSerial            = Temp ;
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sFirst_NG          = Temp ;
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sAdjEqNum          = 0    ;
            Temp = str.SubString(1,str.Pos(">")-1); str = str.Delete(1,str.Pos(">")) ; sEvalTime          = "" ;
        }
        else {
            //For Server Log [DVD]
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sAngle             = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sFA_R              = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sFA_T              = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sThree_Beam_Tilt   = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Size         = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sGT_Shift          = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sArea              = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sPeak_Ratio        = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sParallelism       = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sAbsolute_Center_X = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sAbsolute_Center_Y = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Center_X     = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Center_Y     = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sPeak_Center_X     = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sPeak_Center_Y     = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Shift        = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Shift_R      = Temp ;
            Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sBeam_Shift_T      = Temp ;

            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sLD_Motor_Y        = Temp ;//handl
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sLD_MOtor_Z        = Temp ;
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sGTX_Motor_X       = Temp ;

            //For Server Log [COMMON]
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sJudge             = Temp ;
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sSerial            = Temp ;
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sFirst_NG          = Temp ;
            //Temp = str.SubString(1,str.Pos(",")-1); str = str.Delete(1,str.Pos(",")) ; sAdjEqNum          = 0    ;
            Temp = str.SubString(1,str.Pos(">")-1); str = str.Delete(1,str.Pos(">")) ; sEvalTime          = Temp ;
        }


        Temp2  = " dAngleX="  + AnsiString(dRowAngleX) ;
        Temp2 += " dAngleY="  + AnsiString(dRowAngleY) ;
        Temp2 += " iAngleNG=" + AnsiString(iAngleNG?"NG":"OK") ;
        Temp2 += " dShift="   + AnsiString(dRowShift ) ;
        Temp2 += " bShiftNG=" + AnsiString(bShiftNG?"NG":"OK") ;
        Temp2 += " dVerGap="  + AnsiString(dRowVerGap) ;
        Temp2 += " bVerNG="   + AnsiString(bVerNG?"NG":"OK") ;
        Temp2 += " dHorGap="  + AnsiString(dRowHorGap) ;
        Temp2 += " bHorNG="   + AnsiString(bHorNG?"NG":"OK") ;

        Trace("Row Visn Data" , Temp2.c_str()) ;

        Temp2  = " dAngleX="  + AnsiString(dAngleX  ) ;
        Temp2 += " dAngleY="  + AnsiString(dAngleY  ) ;
        Temp2 += " iAngleNG=" + AnsiString(iAngleNG?"NG":"OK") ;
        Temp2 += " dShift="   + AnsiString(dShift   ) ;
        Temp2 += " bShiftNG=" + AnsiString(bShiftNG?"NG":"OK") ;
        Temp2 += " dVerGap="  + AnsiString(dVerGap  ) ;
        Temp2 += " bVerNG="   + AnsiString(bVerNG?"NG":"OK") ;
        Temp2 += " dHorGap="  + AnsiString(dHorGap  ) ;
        Temp2 += " bHorNG="   + AnsiString(bHorNG?"NG":"OK") ;

        Trace("Apply Data" , Temp2.c_str()) ;

        Temp2  = " sAngle="             +AnsiString (sAngle            ) ;
        Temp2 += " sFA_R="              +AnsiString (sFA_R             ) ;
        Temp2 += " sFA_T="              +AnsiString (sFA_T             ) ;
        Temp2 += " sThree_Beam_Tilt="   +AnsiString (sThree_Beam_Tilt  ) ;
        Temp2 += " sBeam_Size="         +AnsiString (sBeam_Size        ) ;
        Temp2 += " sGT_Shift="          +AnsiString (sGT_Shift         ) ;
        Temp2 += " sArea="              +AnsiString (sArea             ) ;
        Temp2 += " sPeak_Ratio="        +AnsiString (sPeak_Ratio       ) ;
        Temp2 += " sParallelism="       +AnsiString (sParallelism      ) ;
        Temp2 += " sAbsolute_Center_X=" +AnsiString (sAbsolute_Center_X) ;
        Temp2 += " sAbsolute_Center_Y=" +AnsiString (sAbsolute_Center_Y) ;
        Temp2 += " sBeam_Center_X="     +AnsiString (sBeam_Center_X    ) ;
        Temp2 += " sBeam_Center_Y="     +AnsiString (sBeam_Center_Y    ) ;
        Temp2 += " sPeak_Center_X="     +AnsiString (sPeak_Center_X    ) ;
        Temp2 += " sPeak_Center_Y="     +AnsiString (sPeak_Center_Y    ) ;
        Temp2 += " sBeam_Shift="        +AnsiString (sBeam_Shift       ) ;
        Temp2 += " sBeam_Shift_R="      +AnsiString (sBeam_Shift_R     ) ;
        Temp2 += " sBeam_Shift_T="      +AnsiString (sBeam_Shift_T     ) ;
        Temp2 += " sEvalTime="          +AnsiString (sEvalTime         ) ;

        Trace("Server Data", Temp2.c_str());







        bRcvdRslt = true ;

        //야메~~~
        if(bHorNG && !dHorGap) {
            dHorGap = OM.DevOptn.dAPCUnit ;
            Trace("dHorGap  = 0.0" ,"APC값 최소 단위 올림.") ;
        }

    }
    else if(str.SubString(1,6) == "(VC05)") {
        this -> Left = 319 ;
        this -> Top  = 96  ;
        Hide();
        Application->ShowMainForm=true;
        Show();
    }
    else if(str.SubString(1,6) == "(VC06)") { //Test Mode On
//        if( bLeftTable && AT.GetCylStat(aiLT_PcbCmp,ccFwd)) {MsgOk("LeftTable","Need Clamping"); return ;}
//        if(!bLeftTable && AT.GetCylStat(aiRT_PcbCmp,ccFwd)) {MsgOk("LeftTable","Need Clamping"); return ;}

        OnSendAPCMsg("S2","3", AnsiString(OM.DevOptn.dAPCSetVal).c_str());
        OnSendAPCMsg("S1","5");

    }

    else if(str.SubString(1,6) == "(VC07)") { //Test Mode Off
        OnSendAPCMsg("S1","6");
    }

    else if(str.SubString(1,6) == "(VC08)") { //Left Table
        OnSendAPCMsg("S1","3");
        bLeftTable = true ;
    }

    else if(str.SubString(1,6) == "(VC09)") { //Right Table
        OnSendAPCMsg("S1","4");
        bLeftTable = false ;
    }

    else if(str.SubString(1,6) == "(VC10)") { //Up
        OnSendAPCMsg("S2","3",AnsiString(OM.DevOptn.dAPCUnit).c_str(),true);
    }

    else if(str.SubString(1,6) == "(VC11)") { //Dn
        OnSendAPCMsg("S2","3",AnsiString(-OM.DevOptn.dAPCUnit).c_str(),true);
    }
}

void __fastcall TFrmMain::OnSendAPCMsg(AnsiString _sCmd , AnsiString _sData , AnsiString _sVal , bool _bInc)
{
//RW  On    S1 1
//ROM ON    S1 2
//P1  On    S1 3
//P2  On    S1 4
//OUT On    S1 5
//OUT Off   S1 6
//Auto      S1 7
//Manual    S1 8

//OUT Up    S2 1
//OUT Dn    S2 2
//OUT SET   S2 3 Val

//RW  Volt    Req  G1 1
//ROM Current Req  G1 2

//3 + 발루 8E + 발루.

     static bool   bOn = false ;
     static double dLstSetVal  = OM.DevOptn.dAPCSetVal ;

     if(_sCmd == "S2" && _sData == "3") {

         if(_bInc){
             if( dLstSetVal+ StrToFloatDef(_sVal , 0) < OM.DevOptn.dAPCLim ) dLstSetVal+= StrToFloatDef(_sVal , 0) ;
             else                                                            dLstSetVal = OM.DevOptn.dAPCLim ;
         }
         else     {
             if( StrToFloatDef(_sVal , 0)  < OM.DevOptn.dAPCLim ) dLstSetVal = StrToFloatDef(_sVal , 0) ;
             else                                                 dLstSetVal = OM.DevOptn.dAPCLim ;

         }
         Trace("Set Apc Power" , AnsiString(dLstSetVal).c_str()) ;
     }

     //요건 외부용 //요건 이함수용.
     dLastAPCOut = dLstSetVal ;

     if(bOn && _sCmd =="S1" && (_sData == "1" || _sData == "2" || _sData == "3" || _sData == "4")) {
         Trace("return" , "bOn && _sCmd ==S1 && (_sData == 1 || _sData == 2 || _sData == 3 || _sData == 4)");
         MsgOk("APC ERROR","Cannot Change Turn Off first");
         return  ;
     }

     if(_sCmd == "S1" && _sData == "3") OnSendVisnMsg(mvLT);
     if(_sCmd == "S1" && _sData == "4") OnSendVisnMsg(mvRT);

     if(_sCmd == "S1" && _sData == "5") bOn = true  ;
     if(_sCmd == "S1" && _sData == "6") bOn = false ;

     int   iDataLength = 2+1+(_sData.Length() ? 1 : 0)+ (_sVal.Length()?1:0); //2:DataLength  1:CheckSum
     int   iVal        = (dLstSetVal /0.0195) ;


     char sMsg[1024] ;
     memset (sMsg , 0 , 1024);
     int   iDataCnt = 0;

     sMsg[iDataCnt++] = 0x02 ;
     sMsg[iDataCnt++] = iDataLength & 0x0000FF00 ;
     sMsg[iDataCnt++] = iDataLength & 0x000000FF ;
     sMsg[iDataCnt++] = (_sCmd.SubString(1,1) == "S") ? 'S' : 'G' ;
     sMsg[iDataCnt++] = 0x30 + (char)_sCmd.SubString(2,1).ToIntDef(1) ;
     sMsg[iDataCnt++] = _sData.ToIntDef(1) & 0x000000FF ;
     // if(_sCmd == "S2"&&_sData == "3" && _sVal.Length())sMsg[iDataCnt++] = (char)(StrToFloatDef(_sVal , 0) /0.0195) & 0x000000FF ;
      if(_sCmd == "S2"&&_sData == "3" && _sVal.Length())sMsg[iDataCnt++] = (char)iVal & 0x000000FF ;

     for(int i = 0 ; i < iDataCnt ; i++) sMsg[iDataCnt] += sMsg[i] ;


    if(!RS232C_APC->IsOpened())
    {
        MsgOk("APC RS-232C","Open Failed!");
        return;
    }



    RS232C_APC->ClearTxBuffer();

    if(!RS232C_APC->WriteData(iDataLength+3, sMsg)) //iDataLength+ 3(Start Code , DataLength)
    {
        MsgOk("APC RS-232C","Header Send Error");
    }

     for(int i = 0; i <= iDataCnt; i++) {
         Memo1->Lines->Add(IntToStr(i+1) + "=" + IntToHex(sMsg[i], 2));
     }

     Memo1->Lines->Add(sMsg);

     if(Memo1 -> Lines -> Count > 1000) Memo1 -> Clear() ;

}

// 쓰레드에 의해 Data가 수신되면 SendMessage에 의해 호출된다. 즉 수신버퍼에
// TMessage의 WParam의 길이만큼 Data가 수신된 후 호출된다
void __fastcall TFrmMain::UMEvnRxCharAPC(TMessage &Message)
{
    int iLen;
    BYTE RcvBuff[128];

    //if(RadioGroupRecvType->ItemIndex == 1) return;      //sun
    iLen = RS232C_APC->ReadData(Message.WParam, RcvBuff);

    //ShowRcvData(iLen, RcvBuff);
                                                   
    BYTE TempBuff[5];
    AnsiString RcvData;

    RcvData = "Rcv APC Data Lenth = " + IntToStr(iLen) + "\n";
    for(int i=0; i<iLen; i++)
    {
        sprintf(TempBuff, "%02X", RcvBuff[i]);
        RcvData = RcvData + " [" + (char *)TempBuff + "]";
    }
    Memo1->Lines->Add(RcvData);
    Trace("Rcv APC",RcvData.c_str());
}

void __fastcall TFrmMain::UMEvnRxCharBAR1(TMessage &Message)
{
    int iLen;
    char RcvBuff[128] = {0,};

    if(Bar1Delay.OnDelay(true , 500)) { edBar1 -> Text = ""; }

    Bar1Delay.Clear() ;

    SEQ.LT.m_bBarcodeScan = true ;

    //if(RadioGroupRecvType->ItemIndex == 1) return;
    iLen = RS232C_BAR1->ReadData(Message.WParam, RcvBuff);

    //ShowRcvData(iLen, RcvBuff);

    BYTE TempBuff[5];
    AnsiString RcvData = AnsiString(RcvBuff);

    edBar1 -> Text = edBar1 -> Text + RcvData ;

    while(edBar1 -> Text.Pos("\x02")){
        edBar1->Text = edBar1->Text.Delete(edBar1->Text.Pos("\x02"),1);
    }
    while(edBar1 -> Text.Pos("\x03")){
        edBar1->Text = edBar1->Text.Delete(edBar1->Text.Pos("\x03"),1);
    }

    Trace("BAR1 APC",RcvData.c_str());

    RcvData = "Rcv BAR1 Data Lenth = " + IntToStr(iLen) ;
    Memo1->Lines->Add(RcvData);
    Trace("BAR1 APC",RcvData.c_str());
}

void __fastcall TFrmMain::UMEvnRxCharBAR2(TMessage &Message)
{
    int iLen;
    char RcvBuff[128] = {0,};

    if(Bar2Delay.OnDelay(true , 500)) { edBar2 -> Text = ""; }

    Bar2Delay.Clear() ;

    SEQ.RT.m_bBarcodeScan = true ;

    //if(RadioGroupRecvType->ItemIndex == 1) return;
    iLen = RS232C_BAR2->ReadData(Message.WParam, RcvBuff);

    BYTE TempBuff[5];
    AnsiString RcvData = AnsiString(RcvBuff);

    edBar2 -> Text = edBar2 -> Text + RcvData ;

    while(edBar2 -> Text.Pos("\x02")){
        edBar2->Text = edBar2->Text.Delete(edBar2->Text.Pos("\x02"),1);
    }
    while(edBar2 -> Text.Pos("\x03")){
        edBar2->Text = edBar2->Text.Delete(edBar2->Text.Pos("\x03"),1);
    }



    Trace("BAR2 APC",RcvData.c_str());

    RcvData = "Rcv BAR2 Data Lenth = " + IntToStr(iLen) ;
    Memo1->Lines->Add(RcvData);
    Trace("BAR2 APC",RcvData.c_str());
}


void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    edActrName    -> NumberOnly = false ; edIOIndex -> NumberOnly = false ; edCmd -> NumberOnly = false ;
    edActrAddrIF  -> NumberOnly = false ; edIOName  -> NumberOnly = false ;
    edActrAddrOF  -> NumberOnly = false ; edIOInv   -> NumberOnly = false ;
    edActrComt    -> NumberOnly = false ; edIOAdd   -> NumberOnly = false ;
    edActrAddrIB  -> NumberOnly = false ; edIOComt  -> NumberOnly = false ;
    edActrAddrOB  -> NumberOnly = false ; edIODelay -> NumberOnly = false ;
    edBar1        -> NumberOnly = false ; edBar2    -> NumberOnly = false ;

    if(OM.MstOptn.bSunMistake){
        btLtDp1Out -> Tag = 10 ;  btRtDp1Out -> Tag = 24 ;
        btLtDp1In  -> Tag = 10 ;  btRtDp1In  -> Tag = 24 ;
        btLtDp1Up  -> Tag = 12 ;  btRtDp1Up  -> Tag = 26 ;
        btLtDp1Dn  -> Tag = 12 ;  btRtDp1Dn  -> Tag = 26 ;

        btLtDp2Out -> Tag = 9  ;  btRtDp2Out -> Tag = 23 ;
        btLtDp2In  -> Tag = 9  ;  btRtDp2In  -> Tag = 23 ;
        btLtDp2Up  -> Tag = 11 ;  btRtDp2Up  -> Tag = 25 ;
        btLtDp2Dn  -> Tag = 11 ;  btRtDp2Dn  -> Tag = 25 ;

    }















    //Display Version.
    int Age = FileAge(Application -> ExeName);
    AnsiString Date = FileDateToDateTime(Age).FormatString("''yyyy'-'mm'-'dd' 'AM/PM' 'hh':'nn''");
    lbDate -> Caption = Date ;
    lbVer  -> Caption = "Ver " + OnGetVer("FileVersion") ;

    Trace("Program Start",OnGetVer("FileVersion").c_str());



    OnAddTrayIcon();

    Caption = "TCosmos3MotionManager" ;

    pcMain -> ActivePageIndex = 0 ;

    //Width  = 900 ;
    //Height = 550 ;
    
    UpdateDevOptn(true) ;


    tmUpdate -> Enabled = true ;
    tmMode   -> Enabled = true ;

    Application->ShowMainForm = false;  // false인 경우 실행시에 TrayIcon으로 된다.

    //Control Tab
    btLTUVStart -> Glyph -> LoadFromResourceName((int)HInstance , "ROFF") ;
    btRTUVStart -> Glyph -> LoadFromResourceName((int)HInstance , "ROFF") ;

    btLTVacStart-> Glyph -> LoadFromResourceName((int)HInstance , "ROFF") ;
    btRTVacStart-> Glyph -> LoadFromResourceName((int)HInstance , "ROFF") ;

    AnsiString btName ;
    for(int i = 0 ; i < 6 ; i++) {
        btName = "btLTSol"+(AnsiString)(i+1); ((TBitBtn *)(FindComponent(btName))) -> Glyph -> LoadFromResourceName((int)HInstance , "GOFF") ;
        btName = "btLTVac"+(AnsiString)(i+1); ((TBitBtn *)(FindComponent(btName))) -> Glyph -> LoadFromResourceName((int)HInstance , "ROFF") ;
        btName = "btRTSol"+(AnsiString)(i+1); ((TBitBtn *)(FindComponent(btName))) -> Glyph -> LoadFromResourceName((int)HInstance , "GOFF") ;
        btName = "btRTVac"+(AnsiString)(i+1); ((TBitBtn *)(FindComponent(btName))) -> Glyph -> LoadFromResourceName((int)HInstance , "ROFF") ;
    }

    //Position Tab
    PM.SetWindow(pnLT_Z_Pos  , miLT_Z );
    PM.SetWindow(pnLT_Y_Pos  , miLT_Y );
    PM.SetWindow(pnLT_IN_Pos , miLT_I );
    PM.SetWindow(pnRT_Z_Pos  , miRT_Z );
    PM.SetWindow(pnRT_Y_Pos  , miRT_Y );
    PM.SetWindow(pnRT_IN_Pos , miRT_I );

    PM.UpdatePstn(true);

    //Option Tab
    edDispEndDelay -> NumberOnly = true ;
    edSuckTime     -> NumberOnly = true ;

    //Actuator Tab
    AT.DisplayList(sgActuator);
    AT.DisplayProperty(true , edActrName     , edActrComt     ,
                              edActrAddrIF   , edActrAddrIB   ,
                              edActrAddrOF   , edActrAddrOB   ,
                              edActrOnDelayF , edActrOnDelayB ,
                              edActrTimeOutB , edActrTimeOutF ,
                              edActrApplyTO  , edActrApplyOC  ,
                              edActrInv      , edActrInitStat , sgActuator     );


   /* AT.DisplayProperty(bool ToTble , TEdit * EName , TEdit * EComt ,
                                       TEdit * EFx   , TEdit * EBx   ,
                                       TEdit * EFy   , TEdit * EBy   ,
                                       TEdit * EFd   , TEdit * EBd   ,
                                       TEdit * EFt   , TEdit * EBt   ,
                                       TEdit * ETo   , TEdit * EOc   ,
                                       TEdit * EIv   , TStringGrid *Table ,
                                       int Row );                        */

    //IO Tab
    IO.DisplayList    (cbIO -> ItemIndex , sgIO , true , true );
    IO.DisplayProperty(true,cbIO->ItemIndex , edIOIndex, edIOAdd , edIOName , edIOComt , edIOInv , edIODelay , sgIO , cbHex -> Checked ) ;

    //Motor Tab
    MT.GetMotorList(cbMotor) ; cbMotor -> ItemIndex = miLT_Z ;
    MT.UpdatePara(cbMotor -> ItemIndex ,
                  true          ,
                  edACCTime     ,
                  edRunSpeed    ,
                  edManualSpeed ,
                  edHomeSpeed   ,
                  edJogSpeed    ,
                  edInposition  ,
                  edHomeOffset  ,
                  edFirstPos    ,
                  edSecondPos   ,
                  edStopDelay   );


    //APC Rs-232
    RS232C_APC = new TRS232C ;
    if(!RS232C_APC->Open(0,Handle)) {MsgOk("APC RS-232C","Open Failed!");}


    RS232C_BAR1 = new TRS232C ;
    if(!RS232C_BAR1->Open(1,Handle)) {MsgOk("BAR1 RS-232C","Open Failed!");}

    RS232C_BAR2 = new TRS232C ;
    if(!RS232C_BAR2->Open(2,Handle)) {MsgOk("BAR2 RS-232C","Open Failed!");}

    Show();
    Hide();
    //Show();

    SEQ.Reset() ;

    Load();


}

void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    OnSendAPCMsg("S1","6"); //Off

    Save();
    Trace("Program End",OnGetVer("FileVersion").c_str());
    Trace("<EXIT>","EXIT");

    delete RS232C_APC  ;
    delete RS232C_BAR1 ;
    delete RS232C_BAR2 ;

    OnDelTrayIcon() ;




    //delete ddd ;
}

void __fastcall TFrmMain::CreateParams(TCreateParams &Params)
{
    // 반드시 부모를 먼저 호출해야 한다.
    TForm::CreateParams(Params);

    // Titie Bar 안나오게 만들기
    Params.Style = (Params.Style | WS_POPUP) & ~WS_DLGFRAME;
}

void __fastcall TFrmMain::imTileBarMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    ReleaseCapture();
    SendMessage(Handle, WM_NCLBUTTONDOWN, HTCAPTION, 0);
}

void __fastcall TFrmMain::TrayEvent(TMessage& Msg)
{
    if( Msg.LParam==WM_LBUTTONDBLCLK )
    {
       	Application->ShowMainForm=true;
        Show();
    }
    else if (Msg.LParam==WM_RBUTTONDOWN )
    {
       POINT P;
       GetCursorPos(&P );
       SetForegroundWindow (Handle);
       pmTray->Popup(P.x, P.y);
    }
}

void TFrmMain::OnAddTrayIcon(void)
{
    NOTIFYICONDATA  icn;

    icn.cbSize = sizeof(NOTIFYICONDATA);
    icn.hWnd = Handle;
    icn.uID = 100;
    icn.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    icn.uCallbackMessage = WM_USER + 1;
    icn.hIcon = Application->Icon->Handle;
    lstrcpy(icn.szTip,"TCosmos3_Motion");
    Shell_NotifyIcon( NIM_ADD, &icn );
}

void TFrmMain::OnDelTrayIcon(void)
{
    NOTIFYICONDATA  icn;
    icn.cbSize = sizeof(NOTIFYICONDATA);
    icn.hWnd = Handle;
    icn.uID = 100;
    icn.uFlags = 0;
    Shell_NotifyIcon( NIM_DELETE, &icn );
}

void __fastcall TFrmMain::TrayIcon1Click(TObject *Sender)
{
    Application->ShowMainForm=true;
    Show();
    Hide();
}

void __fastcall TFrmMain::Exit1Click(TObject *Sender)
{
    Close();
}

void __fastcall TFrmMain::btMiniClick(TObject *Sender)
{
    pnLevel -> Visible = false ;
    Application->ShowMainForm=true;
    Show();
    Hide();

 //   FrmDisplay ->  FormDeactivate(Sender);

}

void __fastcall TFrmMain::cbMotorChange(TObject *Sender)
{
    Trace(   MT.GetName(((TComboBox *)Sender)->ItemIndex).c_str() , "Selected");
    MT.UpdatePara(cbMotor -> ItemIndex ,
                  true          ,
                  edACCTime     ,
                  edRunSpeed    ,
                  edManualSpeed ,
                  edHomeSpeed   ,
                  edJogSpeed    ,
                  edInposition  ,
                  edHomeOffset  ,
                  edFirstPos    ,
                  edSecondPos   ,
                  edStopDelay   );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    AnsiString sTemp = "" ;

    AnsiString sTemp1 , sTemp2 ;

    static int         PreLevel   = -1 ;
    static AnsiString  PreCaption = "" ;

    if(!FrmDisplay->Visible)  FrmDisplay -> Show() ;

    if(edBar1 -> Text != SEQ.LT.LOG.         Data.Serial) SEQ.LT.LOG.Data         .Serial = edBar1 -> Text;
    if(edBar1 -> Text != SEQ.LT.LOG.ServerLogData.Serial) SEQ.LT.SetServerDataSerial(edBar1 -> Text);

    if(edBar2 -> Text != SEQ.RT.LOG.         Data.Serial) SEQ.RT.LOG.Data         .Serial = edBar2 -> Text;
    if(edBar2 -> Text != SEQ.RT.LOG.ServerLogData.Serial) SEQ.RT.SetServerDataSerial(edBar1 -> Text);

    btSaveOptn -> Enabled = SEQ.GetStat() != ssRunning ;

    if(PreLevel != FrmPassword -> GetLevel()) {
        if(FrmPassword -> GetLevel() == lvOperator) {
            sTemp = "OP" ;

            tsControl  -> TabVisible = true  ;
            tsOption   -> TabVisible = false ;
            tsPosition -> TabVisible = false ;
            tsActuator -> TabVisible = false ;
            tsIo       -> TabVisible = false ;
            tsMotor    -> TabVisible = false ;
            tsTest     -> TabVisible = false ;

            edAgRes  -> ReadOnly = true ;
            edApcRes -> ReadOnly = true ;
            edSfRes  -> ReadOnly = true ;

        }
        else if(FrmPassword -> GetLevel() == lvEngineer) {
            sTemp = "EG" ;

            tsControl   -> TabVisible = true  ;
            tsOption    -> TabVisible = true  ;
            tsPosition  -> TabVisible = true  ;
            tsActuator  -> TabVisible = true  ;
            tsIo        -> TabVisible = true  ;
            tsMotor     -> TabVisible = true  ;
            tsTest      -> TabVisible = false ;

            edAgRes     -> ReadOnly = true ;
            edApcRes    -> ReadOnly = true ;
            edSfRes     -> ReadOnly = true ;

            btApplyActr -> Visible = false ;
            btSaveActr  -> Visible = false ;

            btApplyIO   -> Visible = false ;
            btSaveIO    -> Visible = false ;

        }
        else if(FrmPassword -> GetLevel() == lvMaster  ) {
            sTemp = "HR" ;

            tsControl   -> TabVisible = true  ;
            tsOption    -> TabVisible = true  ;
            tsPosition  -> TabVisible = true  ;
            tsActuator  -> TabVisible = true  ;
            tsIo        -> TabVisible = true  ;
            tsMotor     -> TabVisible = true  ;
            tsTest      -> TabVisible = true  ;

            edAgRes     -> ReadOnly = false ;
            edApcRes    -> ReadOnly = false ;
            edSfRes     -> ReadOnly = false ;

            btApplyActr -> Visible = true ;
            btSaveActr  -> Visible = true ;

            btApplyIO   -> Visible = true ;
            btSaveIO    -> Visible = true ;
        }
        btLevel -> Caption = sTemp ;

        Trace("ChangeLevel",sTemp.c_str());

    }

    PreLevel = FrmPassword -> GetLevel() ;







    lbErr -> Color = EM.IsErr() ? clRed : clLime ;
    //Label2 -> Caption = AnsiString(Left) + " " + AnsiString(Top);
    static bShow = false ;

    if(EM.IsErr() && EM.GetLastErr() == eiVisnInspFail && !bShow) {
        Application->ShowMainForm=true;
        pcMain -> ActivePageIndex = 0 ;
        this -> Left = 641 ;
        this -> Top  = 777 ;

        Show();
        bShow = true ;
    }
    if(!EM.IsErr())bShow = false ;

    lbErr     -> Caption = EM.GetLastErr()  ;
    lbErrName -> Caption = EM.GetLastErrName();

    switch (SEQ.GetStat()) {
        case  ssInit    : if(lbStat -> Caption != "INIT" ) {OnSendVisnMsg(mvMsg , "Motor Initial Start"      ) ; lbStat -> Caption = "INIT" ; lbStat -> Font -> Color = clYellow ; }break ;
        case  ssRunning : if(lbStat -> Caption != "RUN"  ) {OnSendVisnMsg(mvMsg , "Action Start"             ) ; lbStat -> Caption = "RUN"  ; lbStat -> Font -> Color = clGreen  ; }break ;
        case  ssStop    : if(lbStat -> Caption != "STOP" ) {OnSendVisnMsg(mvMsg , "Stop Status Start"        ) ; lbStat -> Caption = "STOP" ; lbStat -> Font -> Color = clBlack  ; }break ;
        case  ssError   : if(lbStat -> Caption != EM.GetLastErrName().c_str()) {
                              OnSendVisnMsg(mvMsg , EM.GetLastErrName().c_str()) ;
                              lbStat -> Caption       = EM.GetLastErrName().c_str()  ;
                              lbStat -> Font -> Color = clRed    ;
                          }
                          break ;
    }

    //for Button Image.
    bool bLTSolOn[6] = {0,}; static bool bPreLTSolOn[6] = {0,};
    bool bLTVacOn[6] = {0,}; static bool bPreLTVacOn[6] = {0,};
    bool bRTSolOn[6] = {0,}; static bool bPreRTSolOn[6] = {0,};
    bool bRTVacOn[6] = {0,}; static bool bPreRTVacOn[6] = {0,};

    bool bLTSuckOn = false ; static bool bPreLTSuckOn = false ;
    bool bRTSuckOn = false ; static bool bPreRTSuckOn = false ;

    AnsiString btName ;

    switch(pcMain -> ActivePageIndex) {
        default:
            break ;

        case 0 : //Control Tab

            for(int i = 0 ; i < 6 ; i++) {
                bLTSolOn[i] = IO.GetY(yLT_Sol1 + i) ;
//                bLTVacOn[i] = IO.GetY(yLT_Vac1 + i) ;
                bRTSolOn[i] = IO.GetY(yRT_Sol1 + i) ;
//                bRTVacOn[i] = IO.GetY(yRT_Vac1 + i) ;

                if(bLTSolOn[i] != bPreLTSolOn[i]) {
                    btName = "btLTSol"+(AnsiString)(i+1);
                    ((TBitBtn *)(FindComponent(btName))) -> Glyph -> LoadFromResourceName((int)HInstance , bLTSolOn[i]?"GON":"GOFF") ;
                }
                if(bLTVacOn[i] != bPreLTVacOn[i]) {
                    btName = "btLTVac"+(AnsiString)(i+1);
                    ((TBitBtn *)(FindComponent(btName))) -> Glyph -> LoadFromResourceName((int)HInstance , bLTVacOn[i]?"RON":"ROFF") ;
                }
                if(bRTSolOn[i] != bPreRTSolOn[i]) {
                    btName = "btRTSol"+(AnsiString)(i+1);
                    ((TBitBtn *)(FindComponent(btName))) -> Glyph -> LoadFromResourceName((int)HInstance , bRTSolOn[i]?"GON":"GOFF") ;
                }
                if(bRTVacOn[i] != bPreRTVacOn[i]) {
                    btName = "btRTVac"+(AnsiString)(i+1);
                    ((TBitBtn *)(FindComponent(btName))) -> Glyph -> LoadFromResourceName((int)HInstance , bRTVacOn[i]?"RON":"ROFF") ;
                }

                bPreLTSolOn[i] = bLTSolOn[i] ;
                bPreLTVacOn[i] = bLTVacOn[i] ;
                bPreRTSolOn[i] = bRTSolOn[i] ;
                bPreRTVacOn[i] = bRTVacOn[i] ;
            }

            bLTSuckOn = IO.GetY(yLT_SuckOn) ;
            bRTSuckOn = IO.GetY(yRT_SuckOn) ;

            if(bLTSuckOn != bPreLTSuckOn)btLTVacStart -> Glyph -> LoadFromResourceName((int)HInstance , bLTSuckOn ? "RON" : "ROFF") ;
            if(bRTSuckOn != bPreRTSuckOn)btRTVacStart -> Glyph -> LoadFromResourceName((int)HInstance , bRTSuckOn ? "RON" : "ROFF") ;

            bPreLTSuckOn = bLTSuckOn ;
            bPreRTSuckOn = bRTSuckOn ;

            lbLTWorkTime -> Caption = SEQ.LT.LOG.Data.WorkTime ;
            lbRTWorkTime -> Caption = SEQ.RT.LOG.Data.WorkTime ;

            break ;

        case 1 : //Option.
            break ;

        case 2 : //Position.
            lbEnc0 -> Caption = MT.GetCmdPos(miLT_Z ) ;
            lbEnc1 -> Caption = MT.GetCmdPos(miLT_Y ) ;
            lbEnc2 -> Caption = MT.GetCmdPos(miLT_I ) ;

            lbEnc3 -> Caption = MT.GetCmdPos(miRT_Z ) ;
            lbEnc4 -> Caption = MT.GetCmdPos(miRT_Y ) ;
            lbEnc5 -> Caption = MT.GetCmdPos(miRT_I ) ;
            break ;

        case 3 : //Actuator.
            AT.DisplayStatus(sgActuator-> Row - 1 ,  lbATFw , lbATBw , lbATAlram) ;

            sTemp1 = edActrAddrIF -> Text ;
            if(sTemp1.Length() == 5 && sTemp1.SubString(1,1) == "X") {
               sTemp1.Delete(1,3) ;
               sTemp2 = "DI" + AnsiString(sTemp1.SubString(1,1).ToIntDef(0) / 2 + 1);
               sTemp1.Delete(1,1) ;
               sTemp1 = StrToInt("x"+sTemp1);    //sTemp1.ToIntDef(0) ;
               sTemp1 = sTemp1.sprintf("_%02d" ,sTemp1.ToIntDef(0)) ;
               sTemp2 += sTemp1 ;
               edActrAddrIF_ -> Text = sTemp2 ;
            }

            sTemp1 = edActrAddrOF -> Text ;
            if(sTemp1.Length() == 5 && sTemp1.SubString(1,1) == "Y") {
               sTemp1.Delete(1,3) ;
               sTemp2 = "DO" + AnsiString(sTemp1.SubString(1,1).ToIntDef(0) / 2 + 1);
               sTemp1.Delete(1,1) ;
               sTemp1 = StrToInt("x"+sTemp1);    //sTemp1.ToIntDef(0) ;
               sTemp1 = sTemp1.sprintf("_%02d" ,sTemp1.ToIntDef(0)) ;
               sTemp2 += sTemp1 ;
               edActrAddrOF_ -> Text = sTemp2 ;
            }

            sTemp1 = edActrAddrIB -> Text ;
            if(sTemp1.Length() == 5 && sTemp1.SubString(1,1) == "X") {
               sTemp1.Delete(1,3) ;
               sTemp2 = "DI" + AnsiString(sTemp1.SubString(1,1).ToIntDef(0) / 2 + 1);
               sTemp1.Delete(1,1) ;
               sTemp1 = StrToInt("x"+sTemp1);    //sTemp1.ToIntDef(0) ;
               sTemp1 = sTemp1.sprintf("_%02d" ,sTemp1.ToIntDef(0)) ;
               sTemp2 += sTemp1 ;
               edActrAddrIB_ -> Text = sTemp2 ;
            }

            sTemp1 = edActrAddrOB -> Text ;
            if(sTemp1.Length() == 5 && sTemp1.SubString(1,1) == "Y") {
               sTemp1.Delete(1,3) ;
               sTemp2 = "DO" + AnsiString(sTemp1.SubString(1,1).ToIntDef(0) / 2 + 1);
               sTemp1.Delete(1,1) ;
               sTemp1 = StrToInt("x"+sTemp1);    //sTemp1.ToIntDef(0) ;
               sTemp1 = sTemp1.sprintf("_%02d" ,sTemp1.ToIntDef(0)) ;
               sTemp2 += sTemp1 ;
               edActrAddrOB_ -> Text = sTemp2 ;
            }


            //


            //
            //




            break ;
        case 4 : //IO/.
            IO.UpdateStatus(cbIO -> ItemIndex,sgIO ) ;
            break ;
        case 5 :
            MT.UpdateStat(cbMotor -> ItemIndex ,
                          lbServoOn            ,     
                          lbHomeSnsr           ,     
                          lbPEndLimSnsr        ,     
                          lbNEndLimSnsr        ,     
                          lbHomeEnded          ,     
                          lbAlarm              ,     
                          lbEncPos             ,     
                          lbPackInPosn         ,     
                          lbStop               ,     
                          lbBusy               ,     
                          lbReady              ,     
                          lbOk                 ,
                          lbJogP               ,     
                          lbJogN               ,
                          lbTrgPos             ,
                          lbCmdPos             );    

            break ;
        case 6 :
            lbLTStepSeq    -> Caption = SEQ.LT.Step.iSeq     ;
            lbLTStepCycle  -> Caption = SEQ.LT.Step.iCycle   ;
            lbLTStepStop   -> Caption = SEQ.LT.Step.iToStop  ;
            lbLTStepStart  -> Caption = SEQ.LT.Step.iToStart ;
            lbLTStepHome   -> Caption = SEQ.LT.Step.iHome    ;

            lbRTStepSeq    -> Caption = SEQ.RT.Step.iSeq     ;
            lbRTStepCycle  -> Caption = SEQ.RT.Step.iCycle   ;
            lbRTStepStop   -> Caption = SEQ.RT.Step.iToStop  ;
            lbRTStepStart  -> Caption = SEQ.RT.Step.iToStart ;
            lbRTStepHome   -> Caption = SEQ.RT.Step.iHome    ;
            break ;
    }

    tmUpdate -> Enabled = true ;


}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btSaveMotorClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    MT.UpdatePara(cbMotor -> ItemIndex ,
                  false         ,
                  edACCTime     ,
                  edRunSpeed    ,
                  edManualSpeed ,
                  edHomeSpeed   ,
                  edJogSpeed    ,
                  edInposition  ,
                  edHomeOffset  ,
                  edFirstPos    ,
                  edSecondPos   ,
                  edStopDelay   );

    MT.Save();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btSaveOptnClick(TObject *Sender)
{

    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    UpdateDevOptn(false) ;
    OM.SaveDevOptn("NONE");
    UpdateDevOptn(true) ;
    SEQ.SetOptn();

    if(OM.DevOptn.iModeSelect == 4) {//DataCheck.
        AT.MoveCyl(aiLT_Support,ccFwd);
        AT.MoveCyl(aiRT_Support,ccFwd);
    }
    else {
        AT.MoveCyl(aiLT_Support,ccBwd);
        AT.MoveCyl(aiRT_Support,ccBwd);
    }

}
//---------------------------------------------------------------------------

void TFrmMain::UpdateDevOptn (bool _bToTable )
{
    if(_bToTable) {
        cbModeSelect     -> ItemIndex = OM.DevOptn.iModeSelect   ;


        edSptrDnDelay    -> Text    = OM.DevOptn.iSptrDnDelay    ;
        edSptrEndUpDelay -> Text    = OM.DevOptn.iSptrEndUpDelay ;

        edAgRes          -> Text    = OM.DevOptn.dAgRes          ;
        edSfRes          -> Text    = OM.DevOptn.dSfRes          ;
        edMaxZPos        -> Text    = OM.DevOptn.dMaxZPos        ;
        edMaxYPos        -> Text    = OM.DevOptn.dMaxYPos        ;
        edMaxIPos        -> Text    = OM.DevOptn.dMaxIPos        ;
        edMaxInspCnt     -> Text    = OM.DevOptn.iMaxInspCnt     ;
        edInspDelay      -> Text    = OM.DevOptn.iInspDelay      ;

        cbInspAftrDisp   -> Checked = OM.DevOptn.bInspAftrDisp   ;
        cbInspBefrDisp   -> Checked = OM.DevOptn.bInspBefrDisp   ;
        cbInspDrngDisp   -> Checked = OM.DevOptn.bInspDrngDisp   ;
        cbAPCOnReadjust  -> Checked = OM.DevOptn.bAPCOnReadjust  ;

        cbAdjGT2ndInsp   -> Checked = OM.DevOptn.bAdjGT2ndInsp   ;
        cbDispFirst      -> Checked = OM.DevOptn.bDispFirst      ;
        cbBarcodeScan    -> Checked = OM.DevOptn.bBarcodeScan    ;

        cbUseAngleCal    -> Checked = OM.DevOptn.bUseAngleCal    ;
        cbUseShiftCal    -> Checked = OM.DevOptn.bUseShiftCal    ;
        cbUseAPCCal      -> Checked = OM.DevOptn.bUseAPCCal      ;

        for(int i = 0 ; i < MAX_DISP_NOZZLE ; i++) {
            for(int j = 0 ; j < MAX_DISP_CNT ; j++) {
                sgLTDispTime -> Cells[i][j] = OM.DevOptn.iLTDisp[i][j] ;
                sgRTDispTime -> Cells[i][j] = OM.DevOptn.iRTDisp[i][j] ;
            }
        }
        edDispEndDelay   -> Text    = OM.DevOptn.iDispEndDelay   ;

        cbUseBeamRslt    -> Checked = OM.DevOptn.bUseBeamRslt    ;
        cbUseAvrPos      -> Checked = OM.DevOptn.bUseAvrPos      ;
        edAvrPosCnt      -> Text    = OM.DevOptn.iAvrPosCnt      ;
        cbUseHomming     -> Checked = OM.DevOptn.bUseHomming     ;
        cbInspAfterDpDone-> Checked = OM.DevOptn.bInspAfterDpDone;

        edUVTime         -> Text    = OM.DevOptn.iUVTime         ;
        edSuckTime       -> Text    = OM.DevOptn.iSuckTime       ;

        edAPCSetVal      -> Text    = OM.DevOptn.dAPCSetVal      ;
        edAPCCalLim      -> Text    = OM.DevOptn.dAPCCalLim      ;
        edApcRes         -> Text    = OM.DevOptn.dApcRes         ;
        edApcLim         -> Text    = OM.DevOptn.dAPCLim         ;

        cbROMType        -> Checked = OM.DevOptn.bROMType        ;
        cbAskInsp        -> Checked = OM.DevOptn.bAskInsp        ;
        cbGrabMode       -> Checked = OM.DevOptn.bGrabMode       ;


        cbDryRun         -> Checked = OM.DevOptn.bDryRun         ;
        cbDataCheck      -> Checked = OM.DevOptn.bDataCheck      ;
        cbInfinityInsp   -> Checked = OM.DevOptn.bInfinityInsp   ;


        edAPCUnit        -> Text    = OM.DevOptn.dAPCUnit        ;
        edAPCOnDelay     -> Text    = OM.DevOptn.iAPCOnDelay     ;

        edAngZVibCnt     -> Text    = OM.DevOptn.iAngZVibCnt     ;
        edAngYVibCnt     -> Text    = OM.DevOptn.iAngYVibCnt     ;

        edSfVibCnt       -> Text    = OM.DevOptn.iSfVibCnt       ;
        edGTDnVibCnt     -> Text    = OM.DevOptn.iGTDnVibCnt     ;
        edGTDnVibRng     -> Text    = OM.DevOptn.dGTDnVibRng     ;
        edGTUpOffset     -> Text    = OM.DevOptn.dGTUpOffset     ;

        edGT3rdInspCnt   -> Text    = OM.DevOptn.iGT3rdInspCnt   ;

        edAngVibRng      -> Text    = OM.DevOptn.dAngVibRng      ;
        edFindAngle      -> Text    = OM.DevOptn.dFindAngle      ;
        edSfVibRng       -> Text    = OM.DevOptn.dSfVibRng       ;

        cbIgnrDisp       -> Checked = OM.DevOptn.bIgnrDisp       ;


    }

    else {
        OM.DevOptn.iModeSelect     = cbModeSelect     -> ItemIndex ;


        OM.DevOptn.iMaxInspCnt     = edMaxInspCnt     -> Text.ToIntDef(0) ;
        OM.DevOptn.iInspDelay      = edInspDelay      -> Text.ToIntDef(0) ;

        OM.DevOptn.iSptrDnDelay    = edSptrDnDelay    -> Text.ToIntDef(0) ;
        OM.DevOptn.iSptrEndUpDelay = edSptrEndUpDelay -> Text.ToIntDef(0) ;

        OM.DevOptn.dAgRes          = edAgRes          -> Text.ToDouble( ) ;
        OM.DevOptn.dSfRes          = edSfRes          -> Text.ToDouble( ) ;

        OM.DevOptn.dMaxZPos        = edMaxZPos        -> Text.ToDouble( ) ;
        OM.DevOptn.dMaxYPos        = edMaxYPos        -> Text.ToDouble( ) ;
        OM.DevOptn.dMaxIPos        = edMaxIPos        -> Text.ToDouble( ) ;

        OM.DevOptn.bInspAftrDisp   = cbInspAftrDisp   -> Checked  ;
        OM.DevOptn.bInspBefrDisp   = cbInspBefrDisp   -> Checked  ;
        OM.DevOptn.bInspDrngDisp   = cbInspDrngDisp   -> Checked  ;

        OM.DevOptn.bAPCOnReadjust  = cbAPCOnReadjust  -> Checked  ;

        OM.DevOptn.bAdjGT2ndInsp   = cbAdjGT2ndInsp   -> Checked  ;
        OM.DevOptn.bDispFirst      = cbDispFirst      -> Checked  ;

        OM.DevOptn.bBarcodeScan    = cbBarcodeScan    -> Checked  ;

        OM.DevOptn.bUseAngleCal    = cbUseAngleCal    -> Checked  ;
        OM.DevOptn.bUseShiftCal    = cbUseShiftCal    -> Checked  ;
        OM.DevOptn.bUseAPCCal      = cbUseAPCCal      -> Checked  ;

        for(int i = 0 ; i < MAX_DISP_NOZZLE ; i++) {
            for(int j = 0 ; j < MAX_DISP_CNT ; j++) {
                OM.DevOptn.iLTDisp[i][j] = sgLTDispTime -> Cells[i][j].ToIntDef(0) ;
                OM.DevOptn.iRTDisp[i][j] = sgRTDispTime -> Cells[i][j].ToIntDef(0) ;
            }
        }
        OM.DevOptn.bUseBeamRslt    = cbUseBeamRslt    -> Checked          ;
        OM.DevOptn.bUseAvrPos      = cbUseAvrPos      -> Checked          ;
        OM.DevOptn.iAvrPosCnt      = edAvrPosCnt      -> Text.ToIntDef(0) ;
        OM.DevOptn.bUseHomming     = cbUseHomming     -> Checked          ;
        OM.DevOptn.bInspAfterDpDone= cbInspAfterDpDone-> Checked          ;

        OM.DevOptn.iDispEndDelay   = edDispEndDelay   -> Text.ToIntDef(0) ;
        OM.DevOptn.iUVTime         = edUVTime         -> Text.ToIntDef(0) ;
        OM.DevOptn.iSuckTime       = edSuckTime       -> Text.ToIntDef(0) ;

        OM.DevOptn.dApcRes         = edApcRes         -> Text.ToDouble() ;
        OM.DevOptn.dAPCSetVal      = edAPCSetVal      -> Text.ToDouble() ;
        OM.DevOptn.dAPCCalLim      = edAPCCalLim      -> Text.ToDouble() ;
        OM.DevOptn.dAPCLim         = edApcLim         -> Text.ToDouble() ;

        OM.DevOptn.bROMType        = cbROMType        -> Checked         ;
        OM.DevOptn.bAskInsp        = cbAskInsp        -> Checked         ;
        OM.DevOptn.bGrabMode       = cbGrabMode       -> Checked         ;
        OM.DevOptn.bDryRun         = cbDryRun         -> Checked         ;
        OM.DevOptn.bDataCheck      = cbDataCheck      -> Checked         ;
        OM.DevOptn.bInfinityInsp   = cbInfinityInsp   -> Checked         ;

        OM.DevOptn.dAPCUnit        = edAPCUnit        -> Text.ToDouble() ;
        OM.DevOptn.iAPCOnDelay     = edAPCOnDelay     -> Text.ToIntDef(0);

        OM.DevOptn.iAngZVibCnt     = edAngZVibCnt     -> Text.ToIntDef(0) ;
        OM.DevOptn.iAngYVibCnt     = edAngYVibCnt     -> Text.ToIntDef(0) ;

        OM.DevOptn.iSfVibCnt       = edSfVibCnt       -> Text.ToIntDef(0) ;
        OM.DevOptn.iGTDnVibCnt     = edGTDnVibCnt     -> Text.ToIntDef(0) ;
        OM.DevOptn.dGTDnVibRng     = edGTDnVibRng     -> Text.ToDouble( ) ;
        OM.DevOptn.dGTUpOffset     = edGTUpOffset     -> Text.ToDouble( ) ;

        OM.DevOptn.iGT3rdInspCnt   = edGT3rdInspCnt   -> Text.ToIntDef(3) ;

        OM.DevOptn.dAngVibRng      = edAngVibRng      -> Text.ToDouble( ) ;
        OM.DevOptn.dFindAngle      = edFindAngle      -> Text.ToDouble( ) ;
        OM.DevOptn.dSfVibRng       = edSfVibRng       -> Text.ToDouble( ) ;


        OM.DevOptn.bIgnrDisp       = cbIgnrDisp       -> Checked          ;

    }
}

void __fastcall TFrmMain::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    PM.UpdatePstn(false);
    PM.Save() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::cbIOChange(TObject *Sender)
{
    IO.DisplayList(cbIO -> ItemIndex , sgIO , true , true );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::sgIODrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
    AnsiString sItem;
    if(ACol == 6 || ACol == 7)
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
    AT.MoveCyl(sgActuator -> Row - 1 , ccBwd );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btFwdClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    AT.MoveCyl(sgActuator -> Row - 1 , ccFwd );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Label1Click(TObject *Sender)
{
   //OnSendVisnMsg(1,"ssskkkeee");
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
                       edActrApplyTO  , edActrApplyOC  ,
                       edActrInv      , edActrInitStat , sgActuator     );
    AT.DisplayList    (sgActuator);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btSaveActrClick(TObject *Sender)
{
     Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    AT.Save();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::sgActuatorClick(TObject *Sender)
{
    AT.DisplayProperty(true , edActrName     , edActrComt     ,
                              edActrAddrIF   , edActrAddrIB   ,
                              edActrAddrOF   , edActrAddrOB   ,
                              edActrOnDelayF , edActrOnDelayB ,
                              edActrTimeOutB , edActrTimeOutF ,
                              edActrApplyTO  , edActrApplyOC  ,
                              edActrInv      , edActrInitStat , sgActuator     );
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
    AT.Rst(sgActuator -> Row - 1 );
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
#include "AxtLIB.h"
#include "AxtCAMCFS20.h"
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
    MT.DoRpt(cbMotor -> ItemIndex) ;
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

void __fastcall TFrmMain::Button4Click(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    SEQ.ReqHome();
}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::Button6Click(TObject *Sender)
{

     AnsiString sTemp = "" ;
     AnsiString sData = edRowTest -> Text ;
     AnsiString sMsg  = "" ;
     int        iDataLength ;//= 2+1+sData.Trim().Length()+ _sVal.Length(); //2:DataLength  1:CheckSum
     char       Sum   = 0 ;



     //야메
     switch(sData.ToIntDef(1)) {
     //                 ASCII STX         그냥 2바이트                ASCII "S1"                                  데이터 hex        다더한값 hex
         case 1 :iDataLength = 4 ; sMsg  =(char)0x02;sMsg +=(char)0x00;sMsg +=(char)0x04;sMsg +=(char)0x53;sMsg +=(char)0x31;sMsg +=(char)0x01; break ;//RW  On           sMsg +=(char)0x8B;
         case 2 :iDataLength = 4 ; sMsg  =(char)0x02;sMsg +=(char)0x00;sMsg +=(char)0x04;sMsg +=(char)0x53;sMsg +=(char)0x31;sMsg +=(char)0x02; break ;//ROM ON           sMsg +=(char)0x8C;
         case 3 :iDataLength = 4 ; sMsg  =(char)0x02;sMsg +=(char)0x00;sMsg +=(char)0x04;sMsg +=(char)0x53;sMsg +=(char)0x31;sMsg +=(char)0x03; break ;//P1  On           sMsg +=(char)0x8D;
         case 4 :iDataLength = 4 ; sMsg  =(char)0x02;sMsg +=(char)0x00;sMsg +=(char)0x04;sMsg +=(char)0x53;sMsg +=(char)0x31;sMsg +=(char)0x04; break ;//P2  On           sMsg +=(char)0x8E;
         case 5 :iDataLength = 4 ; sMsg  =(char)0x02;sMsg +=(char)0x00;sMsg +=(char)0x04;sMsg +=(char)0x53;sMsg +=(char)0x31;sMsg +=(char)0x05; break ;//OUT On           sMsg +=(char)0x8F;
         case 6 :iDataLength = 4 ; sMsg  =(char)0x02;sMsg +=(char)0x00;sMsg +=(char)0x04;sMsg +=(char)0x53;sMsg +=(char)0x31;sMsg +=(char)0x06; break ;//OUT Off          sMsg +=(char)0x90;

         case 7 :iDataLength = 4 ; sMsg  =(char)0x02;sMsg +=(char)0x00;sMsg +=(char)0x04;sMsg +=(char)0x47;sMsg +=(char)0x31;sMsg +=(char)0x01; break ;//RW Volt Req      sMsg +=(char)0x7F;
         case 8 :iDataLength = 4 ; sMsg  =(char)0x02;sMsg +=(char)0x00;sMsg +=(char)0x04;sMsg +=(char)0x47;sMsg +=(char)0x31;sMsg +=(char)0x02; break ;//RW Volt Req      sMsg +=(char)0x80;
         case 9 :iDataLength = 5 ; sMsg  =(char)0x02;sMsg +=(char)0x00;sMsg +=(char)0x05;sMsg +=(char)0x53;sMsg +=(char)0x32;sMsg +=(char)0x03; sMsg +=(char)0x64;break; //sMsg +=(char)0xF3; break ;   //2V
         case 10:iDataLength = 5 ; sMsg  =(char)0x02;sMsg +=(char)0x00;sMsg +=(char)0x05;sMsg +=(char)0x53;sMsg +=(char)0x32;sMsg +=(char)0x03; sMsg +=(char)0x6E;break;// sMsg +=(char)0xFD; break ;   //2.2V                                                                                                                       //0x8E+값   3V 0x96
         //0x02 0x00 0x05 0x53 0x32 0x03 0x64 0xF3
     }

         for(int i = 0 ; i < sMsg.Length() ; i++)Sum += sMsg[i+1] ;
         sMsg += Sum;

   if(!RS232C_APC->IsOpened())
    {
        MsgOk("APC RS-232C","Open Failed!");
        return;
    }
    RS232C_APC->ClearTxBuffer();

    if(!RS232C_APC->WriteData(iDataLength+3, sMsg.c_str()))
    {
        MsgOk("APC RS-232C","Header Send Error");
    }

     for(int i = 1; i <= sMsg.Length(); i++) Memo1->Lines->Add(IntToStr(i) + "=" + IntToHex(sMsg[i], 2));

     Memo1->Lines->Add(sMsg);
}



void __fastcall TFrmMain::SENDClick(TObject *Sender)
{

    OnSendAPCMsg(edCmd -> Text , edData -> Text , edVal  -> Text ) ;
//RW  On    S1 1
//ROM ON    S1 2
//P1  On    S1 3
//P2  On    S1 4
//OUT On    S1 5
//OUT Off   S1 6

//OUT Up    S2 1
//OUT Dn    S2 2
//OUT SET   S2 3 Val

//RW  Volt    Req  G1 1
//ROM Current Req  G1 2
}

void __fastcall TFrmMain::btVisnClick(TObject *Sender)
{
    OnSendVisnMsg(1);
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::sgIOClick(TObject *Sender)
{
   //IO.DisplayProperty(true,c)
   IO.DisplayProperty(true,cbIO->ItemIndex , edIOIndex, edIOAdd , edIOName , edIOComt , edIOInv , edIODelay , sgIO , cbHex -> Checked ) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btSaveIOClick(TObject *Sender)
{
     Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
   IO.Save() ;
}
//---------------------------------------------------------------------------





void __fastcall TFrmMain::btApplyIOClick(TObject *Sender)
{
     Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
   IO.DisplayProperty(false,cbIO->ItemIndex , edIOIndex , edIOAdd , edIOName , edIOComt , edIOInv , edIODelay , sgIO , cbHex -> Checked ) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btErrResetClick(TObject *Sender)
{
    SEQ.Reset() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button5Click(TObject *Sender)
{
    static bool bDone = false ;

    if(bDone) {
        FrmMain -> OnSendAPCMsg("S1","2"); //ROM
        FrmMain -> OnSendAPCMsg("S1","4"); //2P/U
    }

    else {
        FrmMain -> OnSendAPCMsg("S1","1"); //R/W
        FrmMain -> OnSendAPCMsg("S1","3"); //2P/U
    }

    bDone = !bDone ;

}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::Button7Click(TObject *Sender)
{
    static bool bDone = false ;

    OnSendAPCMsg("S1","7"); //Auto

    if(bDone) {
        OnSendAPCMsg("S1","5"); //oN
    }

    else {
        OnSendAPCMsg("S1","6"); //Off
    }

    bDone = !bDone ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button9Click(TObject *Sender)
{
    OnSendAPCMsg("S" , "1","2") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button8Click(TObject *Sender)
{
     OnSendAPCMsg("S" , "1","2.00") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::BitBtn1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
     Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");

     int        iSel  = ((TButton *)Sender) -> Tag ;
     bool       bFwd  = iSel / 10 ;
     int        iMotr = iSel % 10 ;
     bool       bJog  = rgJogUnit -> ItemIndex == 0 ;
     double     dUnit ;

     switch(rgJogUnit -> ItemIndex) {
         case 0 : dUnit = 0.0                         ; break ;
         case 1 : dUnit = 1                           ; break ;
         case 2 : dUnit = 0.5                         ; break ;
         case 3 : dUnit = 0.1                         ; break ;
         case 4 : dUnit = 0.05                        ; break ;
         case 5 : dUnit = edJogUsr -> Text.ToDouble() ; break ;
     }

     if(bJog) {
         if(bFwd)MT.JogP(iMotr) ;
         else    MT.JogN(iMotr) ;
         return ;

     }
     else {
         if(bFwd)MT.GoIncMan(iMotr, dUnit) ;
         else    MT.GoIncMan(iMotr,-dUnit) ;
     }



}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::BitBtn1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
     int        iSel  = ((TButton *)Sender) -> Tag ;
     int        iMotr = iSel % 10 ;

     MT.Stop(iMotr) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btLT_DnMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
     Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

     int        iSel  = ((TButton *)Sender) -> Tag ;
     bool       bFwd  = iSel / 10 ;
     int        iMotr = iSel % 10 ;
     bool       bJog  = rgJogUnit2 -> ItemIndex == 0 ;
     double     dUnit ;

     switch(rgJogUnit2 -> ItemIndex) {
         case 0 : dUnit = 0.0                         ; break ;
         case 1 : dUnit = 1                           ; break ;
         case 2 : dUnit = 0.5                         ; break ;
         case 3 : dUnit = 0.1                         ; break ;
         case 4 : dUnit = 0.05                        ; break ;
         case 5 : dUnit = edJogUsr -> Text.ToDouble() ; break ;
     }

     if(bJog) {
         if(bFwd)MT.JogP(iMotr) ;
         else    MT.JogN(iMotr) ;
         return ;

     }
     else {
         if(bFwd)MT.GoIncMan(iMotr, dUnit) ;
         else    MT.GoIncMan(iMotr,-dUnit) ;
     }



}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btLT_DnMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
     int        iSel  = ((TButton *)Sender) -> Tag ;
     int        iMotr = iSel % 10 ;

     MT.Stop(iMotr) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::Button10Click(TObject *Sender)
{
     FrmMain -> OnSendVisnMsg(mvTest) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button11Click(TObject *Sender)
{
     FrmMain -> OnSendVisnMsg(mvAuto) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::BitBtn28Click(TObject *Sender)
{
    SEQ.LT.Stat.bPosReset = true ;
    MT.SetPos(miLT_Z, PM.GetPos(miLT_Z,piLT_Z_Insp ) ) ;
    MT.SetPos(miLT_Y, PM.GetPos(miLT_Y,piLT_Y_Insp ) ) ;

    if(!MT.CmprPos(miLT_I , PM.GetPos(miLT_I,piLT_I_Wait))) MT.SetPos(miLT_I, PM.GetPos(miLT_I,piLT_I_Insp ) ) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::BitBtn29Click(TObject *Sender)
{
    SEQ.RT.Stat.bPosReset = true ;
    MT.SetPos(miRT_Z, PM.GetPos(miRT_Z,piLT_Z_Insp ) ) ;
    MT.SetPos(miRT_Y, PM.GetPos(miRT_Y,piLT_Y_Insp ) ) ;
    if(!MT.CmprPos(miRT_I , PM.GetPos(miRT_I,piRT_I_Wait))) MT.SetPos(miRT_I, PM.GetPos(miRT_I,piRT_I_Insp ) ) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btLTVacClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
//    IO.SetY(yLT_Vac1 + iSel , !IO.GetY(yLT_Vac1 + iSel));
}
//---------------------------------------------------------------------------
int iSelectedAdd = 0 ;
void __fastcall TFrmMain::btLTSolClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");

    int iSel = ((TBitBtn *)Sender) -> Tag ;
    IO.SetY(yLT_Sol1 + iSel , !IO.GetY(yLT_Sol1 + iSel));
    iSelectedAdd = yLT_Sol1 + iSel ;

    if(edLTSolTime -> Text.ToIntDef(0)) CRunThread<Func> * func = new CRunThread<Func>(&DelayedOff) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btRTSolClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");

    int iSel = ((TBitBtn *)Sender) -> Tag ;
    IO.SetY(yRT_Sol1 + iSel , !IO.GetY(yRT_Sol1 + iSel));
    iSelectedAdd = yRT_Sol1 + iSel ;

    if(edRTSolTime -> Text.ToIntDef(0)) CRunThread<Func> * func = new CRunThread<Func>(&DelayedOff) ;
}

void TFrmMain::DelayedOff()
{
    if(iSelectedAdd == yLT_Sol1) Sleep(edLTSolTime -> Text.ToIntDef(0));
    if(iSelectedAdd == yLT_Sol2) Sleep(edLTSolTime -> Text.ToIntDef(0));
    if(iSelectedAdd == yLT_Sol3) Sleep(edLTSolTime -> Text.ToIntDef(0));
    if(iSelectedAdd == yLT_Sol4) Sleep(edLTSolTime -> Text.ToIntDef(0));
    if(iSelectedAdd == yLT_Sol5) Sleep(edLTSolTime -> Text.ToIntDef(0));
    if(iSelectedAdd == yLT_Sol6) Sleep(edLTSolTime -> Text.ToIntDef(0));

    if(iSelectedAdd == yRT_Sol1) Sleep(edRTSolTime -> Text.ToIntDef(0));
    if(iSelectedAdd == yRT_Sol2) Sleep(edRTSolTime -> Text.ToIntDef(0));
    if(iSelectedAdd == yRT_Sol3) Sleep(edRTSolTime -> Text.ToIntDef(0));
    if(iSelectedAdd == yRT_Sol4) Sleep(edRTSolTime -> Text.ToIntDef(0));
    if(iSelectedAdd == yRT_Sol5) Sleep(edRTSolTime -> Text.ToIntDef(0));
    if(iSelectedAdd == yRT_Sol6) Sleep(edRTSolTime -> Text.ToIntDef(0));


    IO.SetY(iSelectedAdd , false);
}


//---------------------------------------------------------------------------

void __fastcall TFrmMain::btRTVacClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
//    IO.SetY(yRT_Vac1 + iSel , !IO.GetY(yRT_Vac1 + iSel));
}

//---------------------------------------------------------------------------


void __fastcall TFrmMain::btLTUVStartMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    //ilOnOff -> GetBitmap(1,((TBitBtn *)Sender) -> Glyph);

    EN_OUTPUT_ID yOutPut ;

    int iSel = ((TBitBtn *) Sender)->Tag ;

    yOutPut = !iSel ? yLT_HdStart : yRT_HdStart ;

    IO.SetY(yOutPut , true) ;

    ((TBitBtn *)Sender) -> Glyph -> LoadFromResourceName((int)HInstance , "RON") ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btLTUVStartMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{


    EN_OUTPUT_ID yOutPut ;

    int iSel = ((TBitBtn *) Sender)->Tag ;                   

    yOutPut = !iSel ? yLT_HdStart : yRT_HdStart ;

    IO.SetY(yOutPut , false) ;

    ((TBitBtn *)Sender) -> Glyph -> LoadFromResourceName((int)HInstance , "ROFF") ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::ActrBtnBwdClick(TObject *Sender)
{
    int iSelActr = ((TBitBtn *)Sender) -> Tag ;

    if(iSelActr < 14 || iSelActr == 28) SEQ.LT.MoveActr(iSelActr , ccBwd) ;
    else                                SEQ.RT.MoveActr(iSelActr , ccBwd) ;


}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::ActrBtnFwdClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");
    int iSelActr = ((TBitBtn *)Sender) -> Tag ;

    if(iSelActr < 14 || iSelActr == 28) SEQ.LT.MoveActr(iSelActr , ccFwd) ;
    else                                SEQ.RT.MoveActr(iSelActr , ccFwd) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btLT_UVCycleClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    int  iSel       = ((TButton *)Sender) -> Tag ;
    bool bLeftTable = (iSel/10 == 1) ;
    int  iIndex     = (iSel%10) ;

    if(iSel == 31) {OnSendAPCMsg("S1","7");OnSendAPCMsg("S1","6"); return ;}
    if(iSel == 32) {OnSendAPCMsg("S2","3",AnsiString(OM.DevOptn.dAPCUnit).c_str(),true); return ;}
    if(iSel == 33) {OnSendAPCMsg("S2","3",AnsiString(-OM.DevOptn.dAPCUnit).c_str(),true); return ;}

    if(bLeftTable) {
        switch(iIndex) {
            case 1 : SEQ.LT.Rqst.bDisp     =true ; SEQ.LT.Rqst.iDispNzl = -1 ; break ;
            case 2 : SEQ.LT.Rqst.bUVCycle  =true ; break ;
            case 3 : SEQ.LT.Rqst.bDispCycle=true ; break ;
            case 4 : SEQ.LT.Rqst.bInspCycle=true ; break ;
            case 5 : OnSendAPCMsg("S1","6"); //Off
                     FrmMain -> OnSendVisnMsg(mvTest) ;
                     OnSendAPCMsg("S2","3", AnsiString(OM.DevOptn.dAPCSetVal).c_str()); //APC POWER SETTING
                     OnSendAPCMsg("S1","3"); //1P/U
                     if(OM.DevOptn.bROMType) OnSendAPCMsg("S1","2"); //R/W
                     else                    OnSendAPCMsg("S1","1"); //R/W
                     OnSendAPCMsg("S1","5"); //On
                     break ;

        }
    }

    else {
        switch(iIndex) {
            case 1 : SEQ.RT.Rqst.bDisp     =true ;  SEQ.RT.Rqst.iDispNzl = -1 ; break ;
            case 2 : SEQ.RT.Rqst.bUVCycle  =true ; break ;
            case 3 : SEQ.RT.Rqst.bDispCycle=true ; break ;
            case 4 : SEQ.RT.Rqst.bInspCycle=true ; break ;
            case 5 : OnSendAPCMsg("S1","6"); //Off
                     FrmMain -> OnSendVisnMsg(mvTest) ;
                     OnSendAPCMsg("S2","3", AnsiString(OM.DevOptn.dAPCSetVal).c_str()); //APC POWER SETTING
                     OnSendAPCMsg("S1","4"); //1P/U
                     if(OM.DevOptn.bROMType) OnSendAPCMsg("S1","2"); //R/W
                     else                    OnSendAPCMsg("S1","1"); //R/W
                     OnSendAPCMsg("S1","5"); //On
                     break ;
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btRT_SampleBindClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");

    int  iSel       = ((TBitBtn *)Sender) -> Tag ;
    bool bLeftTable = (iSel/10 == 1) ;
    int  bBind      = (iSel%10) ;

    if(bLeftTable) {
        SEQ.LT.MoveActr( aiLT_Main   , !bBind) ;
        Sleep(500); //귀찮아서 야매,.
        SEQ.LT.MoveActr( aiLT_Sub    , !bBind) ;
        SEQ.LT.MoveActr( aiLT_PcbCmp , !bBind) ;
    }
    else {
        SEQ.RT.MoveActr( aiRT_Main   , !bBind) ;
        Sleep(500); //귀찮아서 야매,.
        SEQ.RT.MoveActr( aiRT_Sub    , !bBind) ;
        SEQ.RT.MoveActr( aiRT_PcbCmp , !bBind) ;
    }


}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btMtClrPosClick(TObject *Sender)
{
    MT.SetPos(cbMotor -> ItemIndex , true) ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btLevelClick(TObject *Sender)
{
    pnLevel -> Visible = !pnLevel -> Visible ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btOperClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender)-> Tag ;

    FrmPassword -> Show(iSel) ;

    pnLevel -> Visible = false ;



}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btVacStartClick(TObject *Sender)
{
    Trace(((TBitBtn *)Sender)->Name.c_str() , "Clicked");

    EN_OUTPUT_ID yOutPut ;

    int iSel = ((TBitBtn *) Sender)->Tag ;

    yOutPut =  iSel ? yRT_SuckOn : yLT_SuckOn;

    IO.SetY(yOutPut , !IO.GetY(yOutPut)) ;


}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::Button13Click(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");
    
    int iSel = ((TButton *)Sender) -> Tag ;

    SEQ.LT.m_iCcDpOlSetDispCnt = edLtCcDpOlTtDispCnt -> Text.ToIntDef(1) ;


    SEQ.LT.Rqst.iDispNzl = iSel ; //IO.SetY(yLT_Sol1 + iSel , !IO.GetY(yLT_Sol1 + iSel));
    SEQ.LT.Rqst.bDisp = true ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::Button17Click(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    int iSel = ((TButton *)Sender) -> Tag ;

    SEQ.RT.m_iCcDpOlSetDispCnt = edRtCcDpOlTtDispCnt -> Text.ToIntDef(1) ;


    SEQ.RT.Rqst.iDispNzl = iSel ; //IO.SetY(yLT_Sol1 + iSel , !IO.GetY(yLT_Sol1 + iSel));
    SEQ.RT.Rqst.bDisp = true ;
}
//---------------------------------------------------------------------------

#include "FormUpdate.h"
void __fastcall TFrmMain::lbVerClick(TObject *Sender)
{
    FrmUpdate -> Show() ;
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------



void __fastcall TFrmMain::cbROMTypeClick(TObject *Sender)
{
     Trace(((TCheckBox *)Sender)->Name.c_str() , ((TCheckBox *)Sender)->Checked ? "Check":"UnCheck");
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::edMaxInspCntExit(TObject *Sender)
{
     Trace(((TEdit *)Sender)->Name.c_str() , ((TEdit *)Sender)->Text.c_str());
}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::sgRTDispTimeKeyPress(TObject *Sender, char &Key)
{
    Trace(((TStringGrid *)Sender)->Name.c_str() , "Changed");
}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::pcMainChange(TObject *Sender)
{
    Trace("MainPage" , ("Changed To" + pcMain -> ActivePage -> Caption).c_str()  );
}
//---------------------------------------------------------------------------



void __fastcall TFrmMain::FormDeactivate(TObject *Sender)
{
    //FrmDisplay -> Show() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btOperChangeClick(TObject *Sender)
{
    if(mrYes!=MsgYesNo("Confirm","Are you sure?")) return ;

    AnsiString sPath = LOG_FOLDER + Now().FormatString("yyyymmdd_hhnnss") + ".csv";

    SEQ.LT.LOG.ChangeOper(sPath);
    SEQ.RT.LOG.ChangeOper(sPath);

    if(mrOk!=MsgYesNo("Confirm","You Want to Clear Count && Time Data?")) return ;

    SEQ.LT.DSP.ClearTotalData() ;
    SEQ.RT.DSP.ClearTotalData() ;
}
//---------------------------------------------------------------------------





void __fastcall TFrmMain::Button1Click(TObject *Sender)
{
//Server log test.
    SEQ.LT.LOG.WriteLog();
    SEQ.RT.LOG.WriteLog();

    SEQ.LT.LOG.WriteServerLog(true);
}
//---------------------------------------------------------------------------



                                                        
                                                        
                                                       

void __fastcall TFrmMain::edBar1Change(TObject *Sender)
{
    SEQ.LT.LOG.ServerLogData.Serial = edBar1 -> Text ;
    SEQ.LT.LOG.Data.Serial          = edBar1 -> Text ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::edBar2Change(TObject *Sender)
{
    SEQ.RT.LOG.ServerLogData.Serial = edBar2 -> Text ;
    SEQ.RT.LOG.Data.Serial          = edBar2 -> Text ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::Load()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "SeqData"))
        CreateDir(ExtractFilePath(Application->ExeName) + "SeqData");
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Save
    AnsiString sTemp  ;
    UserINI.Load(sPath, "TFrmMain"    , "LeftBarcode"  , sTemp); edBar1 -> Text = sTemp ;
    UserINI.Load(sPath, "TFrmMain"    , "RightBarcode" , sTemp); edBar2 -> Text = sTemp ;

}

void __fastcall TFrmMain::Save()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "SeqData"))
        CreateDir(ExtractFilePath(Application->ExeName) + "SeqData");
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Save
    UserINI.Save(sPath, "TFrmMain"    , "LeftBarcode"  , edBar1 -> Text );
    UserINI.Save(sPath, "TFrmMain"    , "RightBarcode" , edBar2 -> Text );
}



void __fastcall TFrmMain::cbModeSelectChange(TObject *Sender)
{
    cbInspDrngDisp -> Checked = false ;
    cbAdjGT2ndInsp -> Checked = false ;
    cbDispFirst    -> Checked = false ;
    cbAskInsp      -> Checked = false ;
    cbDryRun       -> Checked = false ;
    cbDataCheck    -> Checked = false ;

    switch(cbModeSelect -> ItemIndex) {
        case 0 : //NORMAL
                 cbInspBefrDisp -> Checked = true ;
                 break ;

        case 1 : //Re Adjust
                 cbInspBefrDisp -> Checked = true ;
                 cbInspDrngDisp -> Checked = true ;
                 cbAdjGT2ndInsp -> Checked = true ;
                 break ;

        case 2 : //Long Run
                 cbDryRun -> Checked = true ;
                 break ;

        case 3 : //Adjust Only
                 cbAskInsp -> Checked = true ;
                 break ;

        case 4 : //Data Check
                 cbDataCheck -> Checked = true ;
                 break ;

        case 5 : //Disp First
                 cbInspBefrDisp -> Checked = true ;
                 cbInspDrngDisp -> Checked = true ;
                 cbAdjGT2ndInsp -> Checked = true ;
                 cbDispFirst    -> Checked = true ;
                 break ;

    }
}
//---------------------------------------------------------------------------




void __fastcall TFrmMain::btErrListClick(TObject *Sender)
{
    FrmErrList -> Show() ;
}
//---------------------------------------------------------------------------





