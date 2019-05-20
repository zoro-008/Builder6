//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmMotorPos.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "UtilDefine.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "DataMan.h"
#include "SMInterfaceUnit.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "PreBuff.h"
#include "Work.h"

#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMotorPos *FormMotorPos;

//---------------------------------------------------------------------------
__fastcall TFormMotorPos::TFormMotorPos(TComponent* Owner)
    : TForm(Owner)
{
    //Position Window Set to Panel
    PM.SetWindow (pnPs_WRK_ZPch,miWRK_ZPch);
    PM.SetWindow (pnPs_PRB_XIdx,miPRB_XIdx);
    PM.SetWindow (pnPs_PRB_TFlp,miPRB_TFlp);
    PM.SetWindow (pnPs_WRK_XRjt,miWRK_XSrt);
    PM.SetWindow (pnPs_WRK_1Idx,miPRI_XIdx);
    PM.SetWindow (pnPs_WRK_2Idx,miPSI_XIdx);

    //Motor Window Set to Panel
    for(int i=0 ; i<MAX_MOTR ; i++) FrmMotor[i] = new TFrameMotr();

    FrmMotor[miWRK_ZPch]->SetIdType(miWRK_ZPch,dtCCwCw_Z);
    FrmMotor[miPRB_XIdx]->SetIdType(miPRB_XIdx,dtCCwCw_X);
    FrmMotor[miPRB_TFlp]->SetIdType(miPRB_TFlp,dtCCwCw_L);
    FrmMotor[miWRK_XSrt]->SetIdType(miWRK_XSrt,dtCCwCw_X);
    FrmMotor[miPRI_XIdx]->SetIdType(miPRI_XIdx,dtCCwCw_X);
    FrmMotor[miPSI_XIdx]->SetIdType(miPSI_XIdx,dtCCwCw_X);

    FrmMotor[miWRK_ZPch]->Parent = pnMt_WRK_ZPch ;
    FrmMotor[miPRB_XIdx]->Parent = pnMt_PRB_XIdx ;
    FrmMotor[miPRB_TFlp]->Parent = pnMt_PRB_TFlp ;
    FrmMotor[miWRK_XSrt]->Parent = pnMt_WRK_XRjt ;
    FrmMotor[miPRI_XIdx]->Parent = pnMt_WRK_1Idx ;
    FrmMotor[miPSI_XIdx]->Parent = pnMt_WRK_2Idx ;

    for(int i=0 ; i<MAX_MOTR ; i++) FrmMotor[i]->Show();

    //Cylinder Window Set to Panel
    for(int i=0 ; i<MAX_ACTR ; i++) FrmCylinder[i] = new TFrameCyl();

    FrmCylinder[aiPRB_Idx ]->SetIdType(aiPRB_Idx ,dtCCwCw_Z); //CCw 가 빽이라고 생각해주세요 ㅠㅠ.
    FrmCylinder[aiPRI_Idx ]->SetIdType(aiPRI_Idx ,dtCCwCw_Z); //나중에 고칠게요 ㅠㅠ ...나중에...dd
    FrmCylinder[aiPSI_Idx ]->SetIdType(aiPSI_Idx ,dtCCwCw_Z);

    FrmCylinder[aiPRB_Idx ]->Parent = pnPRB_Idx  ;
    FrmCylinder[aiPRI_Idx ]->Parent = pnWRK_1Idx ;
    FrmCylinder[aiPSI_Idx ]->Parent = pnWRK_2Idx ;

    for(int i=0 ; i<MAX_ACTR ; i++) FrmCylinder[i]->Show();

}
//---------------------------------------------------------------------------
void __fastcall TFormMotorPos::btSaveDeviceClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    //강제 세팅. 비젼 Y축 무조건 0.0으로.
//    PM.SetPos(miWRK_YVsn , pvWRK_YVsnWaitPs , 0.0 );

    PM.UpdatePstn(toBuff);
     PM.Save(OM.m_sCrntDev);

 //   DM.ARAY[riLDR].SetMaxColRow(1                 ,OM.DevInfo.iCsSlCnt);
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riPRI].SetMaxColRow(OM.DevInfo.iColCnt,OM.DevInfo.iRowCnt );
    DM.ARAY[riINB].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    DM.ARAY[riPSI].SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
}
//---------------------------------------------------------------------------
void __fastcall TFormMotorPos::FormShow(TObject *Sender)
{
    PM.UpdatePstn (toTabl);
    PageControl->ActivePageIndex = 0;
}
//-------------------------------------------------------------------------


void __fastcall TFormMotorPos::FormDestroy(TObject *Sender)
{
    for(int i = 0 ; i < MAX_MOTR ; i++ )
    {
        if(FrmMotor[i]) delete FrmMotor[i];
        FrmMotor[i] = NULL ;
    }

    for(int i = 0 ; i < MAX_ACTR ; i++ )
    {
        if(FrmCylinder[i]) delete FrmCylinder[i];
        FrmCylinder[i] = NULL ;
    }

}
//---------------------------------------------------------------------------

