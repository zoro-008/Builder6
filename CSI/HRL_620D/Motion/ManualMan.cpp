//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"

#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "PstnMan.h"
//#include "LotUnit.h"
//#include "Rs232Man.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Stage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CManualMan MM;
//---------------------------------------------------------------------------
__fastcall CManualMan::CManualMan(void)
{
}

//---------------------------------------------------------------------------
__fastcall CManualMan::~CManualMan(void)
{

}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Init(void)
{
    //Init. Var.
    m_iManStep    = 0     ;
    m_iPreManStep = 0     ;
    m_iManNo      = mcNoneCycle ;

//    m_bManSetting = false ;

    m_tmCycle.Clear() ;

}

//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","�߸��� �޴��� �����Դϴ�." ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","�߸��� �޴��� �����Դϴ�." ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","���� �޴��� ���� ���Դϴ�."); return false ;}
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","��� ���� ���Դϴ�."     ); return false ;}
    //if(!MT_GetHomeEndAll()           ) {FM_MsgOk("ERR","�ʱ�ȭ�� �����ϼ���!"   ); return false ;}

    //Check Alar
    if ( EM_IsErr()      ) { Init(); return false  ; } //�� �ؿ� ó�� �ϴ� �� ������ ��� �̷��� ��.

    EM_SetDisp(true);

    m_bManSetting = true ;
    m_iManNo = _iNo ;

    bool bRet = true ;

//***********************************************************************************************
// �������μ���
//***********************************************************************************************

    if(m_iManNo == mcAllHome ){
        if(!IO_GetX(xSTG_WrkPstDetect)                     ) {FM_MsgOk("ERROR","���̽� ���� ��ũ ��ġ�� ���� �ּ���."    ); MT_SetServo(miSTG_TBase , false); bRet = false ;} //���߿� ����JS
        if(!OM.CmnOptn.bNotUsedVAxis){
            if( IO_GetX(xSTG_WrkPstDetect) && !MT_GetServoAll()) {FM_MsgOk("ERROR","���̽� T�� ���� ���� ON�� Ȯ�� ���ּ���."); bRet = false ;}
        }
        else {
            if( IO_GetX(xSTG_WrkPstDetect) && (!MT_GetServo(miSTG_TBase) && !MT_GetServo(miWRK_UBrd1) && !MT_GetServo(miWRK_UBrd2))) {
                FM_MsgOk("ERROR","���̽� T�� ���� ���� ON�� Ȯ�� ���ּ���."); bRet = false ;
            }
        }
        if( IO_GetX(xSTG_TltDetect   )                     ) {FM_MsgOk("ERROR","ƿ�� ������ Ȯ�� ���ּ���."              ); bRet = false ;}


      //  if(!IO_GetX(xETC_PowerOn  )    ) {FM_MsgTime("ERR","Power On Plz        ",2000); bRet = false ;}
      //  if(!IO_GetX(xETC_MainSol  )    ) {FM_MsgTime("ERR","Check Air Sensor Plz",2000); bRet = false ;}
      //  if(!IO_GetY(yETC_MainSol  )    ) {FM_MsgTime("ERR","Check Air Button Plz",2000); bRet = false ;}

    }
    else if(m_iManNo ==  mcCycleRotate){
        if(!OM.CmnOptn.bNotUsedVAxis){
            if((MT_GetCmdPos(miWRK_UBrd1) >= PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait)+0.1) ||
               (MT_GetCmdPos(miWRK_UBrd1) <= PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait)-0.1) ||
               (MT_GetCmdPos(miWRK_UBrd2) >= PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait)+0.1) ||
               (MT_GetCmdPos(miWRK_UBrd2) <= PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait)-0.1)) {
                   FM_MsgOk("ERROR","��� ���� ���͵��� Wait �����ǿ� ��ġ ���ּ���."); bRet = false ;
            }
        }

        else{
            if((MT_GetCmdPos(miWRK_UBrd1) >= PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait)+0.1) ||
               (MT_GetCmdPos(miWRK_UBrd1) <= PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait)-0.1) ||
               (MT_GetCmdPos(miWRK_UBrd2) >= PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait)+0.1) ||
               (MT_GetCmdPos(miWRK_UBrd2) <= PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait)-0.1) ||
               (MT_GetCmdPos(miWRK_VBrd1) >= PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait)+0.1) ||
               (MT_GetCmdPos(miWRK_VBrd1) <= PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait)-0.1) ||
               (MT_GetCmdPos(miWRK_VBrd2) >= PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait)+0.1) ||
               (MT_GetCmdPos(miWRK_VBrd2) <= PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait)-0.1)) {
            //if((MT_GetCmdPos(miWRK_UBrd1) != PM.GetValue(miWRK_UBrd1 , pvWRK_UBrd1_Wait)) ||
            //   (MT_GetCmdPos(miWRK_UBrd2) != PM.GetValue(miWRK_UBrd2 , pvWRK_UBrd2_Wait)) ||
            //   (MT_GetCmdPos(miWRK_VBrd1) != PM.GetValue(miWRK_VBrd1 , pvWRK_VBrd1_Wait)) ||
            //   (MT_GetCmdPos(miWRK_VBrd2) != PM.GetValue(miWRK_VBrd2 , pvWRK_VBrd2_Wait)) ){
                   FM_MsgOk("ERROR","��� ���� ���͵��� Wait �����ǿ� ��ġ ���ּ���."); bRet = false ;
            }
        }

        // ������ �࿡ V�� ���� ��, V�� ����
        if( IO_GetX(xSTG_TltDetect)){
            FM_MsgOk("ERROR","ƿ�� ������ Ȯ�� ���ּ���." ); bRet = false ;
        }
    }

    else if(m_iManNo == mcCycleStart){
        if(!SEQ.InspectHomeEnd()                           ){ FM_MsgOk("ERROR","��� Ȩ ������ �Ϸ� ���ּ���"            ); bRet = false ;}
        if(!IO_GetX(xSTG_WrkPstDetect)                     ) {FM_MsgOk("ERROR","���̽��� ��ũ��ġ�� �����ֽð� Ȩ�� ����ּ���."    ); MT_SetServo(miSTG_TBase , false); bRet = false ;}
        if(!OM.CmnOptn.bNotUsedVAxis){
            if( IO_GetX(xSTG_WrkPstDetect) && !MT_GetServoAll()) {FM_MsgOk("ERROR","���̽� T�� ���� ���� ON�� Ȯ�� ���ּ���."); bRet = false ;}
        }
        else {
            if( IO_GetX(xSTG_WrkPstDetect) && (!MT_GetServo(miSTG_TBase) && !MT_GetServo(miWRK_UBrd1) && !MT_GetServo(miWRK_UBrd2))) {
                FM_MsgOk("ERROR","���̽� T�� ���� ���� ON�� Ȯ�� ���ּ���."); bRet = false ;
            }
        }
        if( IO_GetX(xSTG_TltDetect   )                     ) {FM_MsgOk("ERROR","ƿ�� ������ Ȯ�� ���ּ���."              ); bRet = false ;}
    }



    if(!bRet) Init();

//***********************************************************************************************
// ó��..
//***********************************************************************************************
         if (m_iManNo == mcAllHome ) {
             STG.InitHomeStep();
             //Home End False
             for(int i=0; i<MAX_MOTR; i++) MT_SetHomeEnd(i,false);
         }

    else if(m_iManNo == mcCycleRotate  ){STG.InitCycleStep();}
    else if(m_iManNo == mcCycleStart   ){STG.InitCycleStep();}

    else { m_iManNo = mcNoneCycle ; }

    m_bManSetting = false ;
    return true ;
}

EN_MANUAL_CYCLE __fastcall CManualMan::GetManNo()
{
    return m_iManNo ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Update(void)
{
    if( m_iManNo == mcNoneCycle ) return  ;
    if( m_bManSetting           ) return  ;
    if( m_bStop                 ) { m_iManNo = mcNoneCycle; m_bStop = false ; return ; } // Stop �߰� dd 0523 . PM 1h

    if(!OM.CmnOptn.bDoorSkip) SEQ.InspectDoor    ();
    if(m_iManNo != mcAllHome) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool bRet;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }

    //Cycle Step.
    if(m_iManNo == mcAllHome   ) {
        //if(!IO_GetX(xSTG_Detect)) {
        //    m_iManNo = mcNoneCycle;
        //    FM_MsgOk("ERR","���̽� ���� ��ũ ��ġ�� ���� �ּ���.");
       // }
       // else {
            bRet = STG.CycleHome();
            if(!bRet) return ;
            m_iManNo = mcNoneCycle;
            if(STG.m_bHomeEnd) FM_MsgOk("Confirm","��� Ȩ ������ �Ϸ� �Ǿ����ϴ�.!");
       // }
    }
    // ����Ŭ ����
    else if(m_iManNo == mcCycleRotate ){if(STG.CyclePlace()) m_iManNo = mcNoneCycle;}
    else if(m_iManNo == mcCycleStart  ){if(STG.CycleWork ()) m_iManNo = mcNoneCycle;}
    else                               {                     m_iManNo = mcNoneCycle;}

    //Ok.
    return ;
}

//---------------------------------------------------------------------------




