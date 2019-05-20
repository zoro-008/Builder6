//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Stage.h"
#include "Sequence.h"

//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
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

    m_bManSetting  = false ;

    m_bUldNeedPick  = false;
    m_bUldNeedPlace = false;

    m_tmCycle.Clear() ;
    m_tmDelay.Clear() ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Close(void)
{
}

//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return false ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.

    //if(!IO_GetX(xETC_MainPower)                        ) {FM_MsgOk("ERR","Power On Plz");      return false ;}
    //if(!IO_GetX(xETC_MainAir) || !IO_GetY(yETC_MainAir)) {FM_MsgOk("ERROR","Check Main Air!"); return false ;}

    EM_SetDisp(true);

    m_bManSetting = true ; //SetManCycle함수는 화면 쓰레드. 업데이트 함수에서 다른쓰레드로 들어와서 갱신하기에 플레그 걸어 막아둠.
    m_iManNo = _iNo ;

    bool bRet = true ;

    /********************/
    /* 프리프로세서     */
    /********************/
         if(m_iManNo == mcNoneCycle          ){FM_MsgOk("ERROR","None Cycle이 선택되었습니다."    ); bRet = false ;}
    else if(m_iManNo == mcAllHome            ){}
    else if(m_iManNo == mcSTG_Home           ){}


    if(!bRet) Init();


    /********************/
    /* 처리..           */
    /********************/

         if(m_iManNo == mcNoneCycle          ){FM_MsgOk("ERROR","None Cycle이 선택되었습니다."    ); bRet = false ;}
    else if(m_iManNo == mcAllHome            ){MT_SetServoAll(true);
                                               STG.InitHomeStep ();
                                              }

    else if(m_iManNo == mcSTG_Home           ){STG.InitHomeStep ();                                        }
    m_bManSetting = false ; //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
    return true ;
}

EN_MANUAL_CYCLE __fastcall CManualMan::GetManNo()
{
    return m_iManNo ;
}

int CManualMan::GetManStep()
{
    return m_iManStep ;
}
//---------------------------------------------------------------------------
void __fastcall CManualMan::Update(void)
{

    if( m_iManNo == mcNoneCycle ) return ;
    if(m_bManSetting            ) return ;

    //if(!OM.CmnOptn.bIgnrDoor    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcSTG_Home &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();

    SEQ.InspectLightGrid();                                                   

    bool r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11 ;
    r1=r2=r3=r4=r5=r6=r7=r8=r9=r10=r11 = false ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }
    //Cycle Step.
    if      (m_iManNo == mcAllHome      ) { r5  = STG.CycleHome();
                                            if(r5) {
                                                //     MT_SetHomeEnd(miETC_Spare , true);
                                                m_iManNo = mcNoneCycle;
                                                FM_MsgTime("Confirm","All Home Finished!",1000);
                                            }
                                          }





    else if(m_iManNo == mcSTG_Home           ){if(STG.CycleHome    ()) m_iManNo = mcNoneCycle; }
    else                                      {                        m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;

}



















































































