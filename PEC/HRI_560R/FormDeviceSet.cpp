//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormDeviceSet.h"
//#include "UserFile.h"
//#include "FormMain.h"


#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "DataMan.h"
#include "ManualMan.h"
#include "Sequence.h"

//#include "Loader.h"
//#include "Rail.h"
#include "Head.h"
//#include "PstBuff.h"
//#include "UnLoader.h"
//#include "UnLoaderMotor.h"

//#include "UserFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmDeviceSet *FrmDeviceSet;
int    iMrkManCol = -1;
int    iMrkManRow = -1;
//---------------------------------------------------------------------------
__fastcall TFrmDeviceSet::TFrmDeviceSet(TComponent* Owner)
    : TForm(Owner)
{

    PM.SetWindow(pnLDR_XPsh , miLDR_XPsh);
    PM.SetWindow(pnLDR_YFed , miLDR_YFed);
    PM.SetWindow(pnLDR_XPck , miLDR_XPck);
    PM.SetWindow(pnLDR_ZPck , miLDR_ZPck);
    PM.SetWindow(pnWK1_YFlp , miWK1_YFlp);
    PM.SetWindow(pnWK1_XIns , miWK1_XIns);
    PM.SetWindow(pnWRK_XVsn , miWRK_XVsn);
    PM.SetWindow(pnWRK_YVsn , miWRK_YVsn);
    PM.SetWindow(pnPSB_XMrk , miPSB_XMrk);
    PM.SetWindow(pnPSB_YMrk , miPSB_YMrk);
    PM.SetWindow(pnULD_ZPck , miULD_ZPck);
    PM.SetWindow(pnULD_YPck , miULD_YPck);
    PM.SetWindow(pnULD_ZTrI , miULD_ZTrI);
    PM.SetWindow(pnULD_ZTrO , miULD_ZTrO);
    PM.SetWindow(pnULD_XStp , miULD_XStp);
    PM.SetWindow(pnULD_ZPpr , miULD_ZPpr);

    pcDeviceSet -> ActivePageIndex = 0 ;

    btPRB_Feeding     -> Tag = (int)yPRB_FeedingAC;
    btPRB_Feeding2    -> Tag = (int)yPRB_FeedingAC;
    btWRK_Feeding     -> Tag = (int)yWK1_FeedingAC;
    btPSB_Feeding     -> Tag = (int)yPSB_FeedingAC;
    btULD_Feeding     -> Tag = (int)yULD_FeedingAC;
    btULD_Feeding2    -> Tag = (int)yULD_FeedingAC;

    MtWindowSet();
    AtWindowSet();


    for (int i = 0; i < ComponentCount; i++)
    {
        if (Components[i]->ClassNameIs("TEdit"))
        {
            ((TEdit*)FindComponent(Components[i]->Name))->ImeMode    = imDisable ;
            ((TEdit*)FindComponent(Components[i]->Name))->NumberOnly = true ;
            ((TEdit*)FindComponent(Components[i]->Name))->UseApoint  = true ;
            ((TEdit*)FindComponent(Components[i]->Name))->UseMinus   = true ;
        }
        else if(Components[i]->ClassNameIs("TPageControl"))
        {
            ((TPageControl*)FindComponent(Components[i]->Name))->ActivePageIndex = 0 ;
        }
    }
}
void __fastcall TFrmDeviceSet::MtWindowSet()
{
    for(int i=0 ; i<MAX_MOTR ; i++) {
        FraMotor  [i] = new TFraMotr();
        FraMotor  [i] ->SetIdType(i ,MT_GetDirType(i));
        FraMotor  [i] ->Show();
    }

//    for(int i=0 ; i<MAX_MOTR ; i++) FraMotor[i]->Show();
}
void __fastcall TFrmDeviceSet::AtWindowSet()
{
    //Cylinder Window Set to Panel
    for(int i=0 ; i<MAX_ACTR ; i++) {
        FraCylinder[i] = new TFraCyl();
        FraCylinder[i] ->SetIdType(i ,AT_GetDirType(i));
        FraCylinder[i] ->Show();
    }

//    for(int i=0 ; i<MAX_ACTR ; i++) FraCylinder[i]->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btSavePosClick(TObject *Sender)
{
    Trace(((TButton *)Sender)->Name.c_str() , "Clicked");

    if (FM_MsgYesNo("Confirm" , "Are you Sure?") != mrYes)  return;

    JobFileBackUp();

    UpdateDevInfo(toBuff);
    OM.SaveDevInfo(OM.m_sCrntDev);

    UpdateDevOptn(toBuff);
    OM.SaveDevOptn(OM.m_sCrntDev);

    PM.UpdatePstn(toBuff);
    PM.Save(OM.m_sCrntDev);

    DM.ARAY[riLDR].SetMaxColRow(OM.DevInfo.iLDRColCnt , OM.DevInfo.iLDRRowCnt );
    DM.ARAY[riPRB].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );
    DM.ARAY[riWR1].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );
    DM.ARAY[riVS1].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );
    DM.ARAY[riWR2].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );
    DM.ARAY[riVS2].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );
    DM.ARAY[riWR3].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );
    DM.ARAY[riVS3].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );
    DM.ARAY[riPSB].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );
    DM.ARAY[riVS4].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );
    DM.ARAY[riULD].SetMaxColRow(OM.DevInfo.iColCnt    , OM.DevInfo.iRowCnt    );
    DM.ARAY[riTRY].SetMaxColRow(OM.DevInfo.iULDColCnt , OM.DevInfo.iULDRowCnt );
    DM.ARAY[riUPK].SetMaxColRow(1                     , OM.DevInfo.iULDPkgCnt );

    HED.DpAray1.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    HED.DpAray2.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );
    HED.DpAray3.SetMaxColRow(OM.DevInfo.iColCnt , OM.DevInfo.iRowCnt );



}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::JobFileBackUp()
{
    String sSorc  = ExtractFilePath(Application -> ExeName) + "JobFile\\" ;
    String sPath  = ExtractFilePath(Application -> ExeName) + "BackUp\\JobFile\\" ;
    String SDest  = sPath + Now().FormatString("yyyymmdd") + "\\" + Now().FormatString("hh'h'nn'm'ss's'") + "\\" ;

    UserFile.ClearDirDate(sPath , Now() - 90);

    sPath += Now().FormatString("yyyymmdd") + "\\" + Now().FormatString("hh'h'nn'm'ss's'") + "\\" ;

    UserFile.CopyDir(sSorc , SDest);

    //Trace Log.
//    Trace("JobFile",(edToName -> Text + " is Maked by Copy").c_str());

}
//---------------------------------------------------------------------------
bool __fastcall TFrmDeviceSet::MotorCheck(EN_MOTR_ID _iMotor)
{
    bool bRet1 = true;

         if(_iMotor == miLDR_XPsh) {}
    else if(_iMotor == miLDR_YFed) {}
    else if(_iMotor == miLDR_XPck) {}
    else if(_iMotor == miLDR_ZPck) {}
    else if(_iMotor == miWK1_YFlp) {}
    else if(_iMotor == miWRK_XVsn) {}
    else if(_iMotor == miWRK_YVsn) {}
    else if(_iMotor == miPSB_XMrk) {if(!AT_Complete(aiPSB_Marking, ccBwd)) {AT_MoveCyl(aiPSB_Marking, ccBwd); bRet1 = false; } }
    else if(_iMotor == miPSB_YMrk) {if(!AT_Complete(aiPSB_Marking, ccBwd)) {AT_MoveCyl(aiPSB_Marking, ccBwd); bRet1 = false; } }
    else if(_iMotor == miWK1_XIns) {}
    else if(_iMotor == miULD_ZPck) {}
    else if(_iMotor == miULD_YPck) {}
    else if(_iMotor == miULD_ZTrI) {}
    else if(_iMotor == miULD_ZTrO) {}
    else if(_iMotor == miULD_XStp) {}
    else if(_iMotor == miULD_ZPpr) {}

    return bRet1 ;
}
//---------------------------------------------------------------------------

double TFrmDeviceSet::GetMotrPosTable(EN_MOTR_ID _iMotr , int _iRow , int _iCol)
{
    int    r , c ;
    double dStartYPos ;
    double dStartXPos ;
    double dYPos      ;
    double dXPos      ;

    r = _iRow ;//OM.DevOptn.iRowInspCnt ? _iRow/OM.DevOptn.iRowInspCnt * OM.DevOptn.iRowInspCnt : _iRow ;
    c = _iCol ;//OM.DevOptn.iColInspCnt ? _iCol/OM.DevOptn.iColInspCnt * OM.DevOptn.iColInspCnt : _iCol ;

    if(cbManualAction->ItemIndex == 0 ) { // Vision
        dStartYPos = PM.GetValue(_iMotr , pvPSB_YMrkVisnStart ) ;
        dStartXPos = PM.GetValue(_iMotr , pvPSB_XMrkVisnStart ) ;
    }
    else {
        dStartYPos = PM.GetValue(_iMotr , pvPSB_YMrkWorkStart ) ;
        dStartXPos = PM.GetValue(_iMotr , pvPSB_XMrkWorkStart ) ;
    }

    if(_iMotr != miPSB_YMrk && _iMotr != miPSB_XMrk) return MT_GetCmdPos(_iMotr) ;

    dXPos = dStartXPos - c * OM.DevInfo.dColPitch ;
    dYPos = dStartYPos + r * OM.DevInfo.dRowPitch ;

    if(OM.DevInfo.dColGrGap && OM.DevInfo.iColGrCnt) {
        dXPos = dXPos - (c / OM.DevInfo.iColGrCnt) * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) ;
    }


    if(OM.DevInfo.dRowGrGap && OM.DevInfo.iRowGrCnt) {
        dYPos = dYPos + (r / OM.DevInfo.iRowGrCnt) * (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) ;
    }

    if     (_iMotr == miPSB_YMrk) return dYPos ;
    else if(_iMotr == miPSB_XMrk) return dXPos ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::tmUpdateTimerTimer(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;

    bool bFoward;
    AnsiString sMsg;

    switch ( pcDeviceSet -> ActivePageIndex )
    {
        case 0 :
        break;

        case 1 :
        break;

        case 2 : btPRB_Feeding   ->Font->Color   = IO_GetY(yPRB_FeedingAC ) ? clLime : clBlack ;
                 btPRB_Feeding   ->Caption       = IO_GetY(yPRB_FeedingAC ) ? "ON"   : "OFF"   ;
        break;

        case 3 : btPRB_Feeding2  ->Font->Color   = IO_GetY(yPRB_FeedingAC ) ? clLime : clBlack ;
                 btPRB_Feeding2  ->Caption       = IO_GetY(yPRB_FeedingAC ) ? "ON"   : "OFF"   ;
                 btWRK_Feeding   ->Font->Color   = IO_GetY(yWK1_FeedingAC ) ? clLime : clBlack ;
                 btWRK_Feeding   ->Caption       = IO_GetY(yWK1_FeedingAC ) ? "ON"   : "OFF"   ;
        break;

        case 4 : btPSB_Feeding   ->Font->Color   = IO_GetY(yPSB_FeedingAC ) ? clLime : clBlack ;
                 btPSB_Feeding   ->Caption       = IO_GetY(yPSB_FeedingAC ) ? "ON"   : "OFF"   ;
        break;

        case 5 : btULD_Feeding   ->Font->Color   = IO_GetY(yULD_FeedingAC ) ? clLime : clBlack ;
                 btULD_Feeding   ->Caption       = IO_GetY(yULD_FeedingAC ) ? "ON"   : "OFF"   ;
                 btULD_Vaccum    ->Font->Color   =(IO_GetY(yULD_PickVaccum1) || IO_GetY(yULD_PickVaccum2)) ? clLime : clBlack ;
                 btULD_Vaccum    ->Caption       =(IO_GetY(yULD_PickVaccum1) || IO_GetY(yULD_PickVaccum2)) ? "ON"   : "OFF"   ;
                 btULD_Ejectr    ->Font->Color   =(IO_GetY(yULD_PickEjectr1) || IO_GetY(yULD_PickEjectr2)) ? clLime : clBlack ;
                 btULD_Ejectr    ->Caption       =(IO_GetY(yULD_PickEjectr1) || IO_GetY(yULD_PickEjectr2)) ? "ON"   : "OFF"   ;
        break;

        case 6 : btULD_Feeding2  ->Font->Color   = IO_GetY(yULD_FeedingAC ) ? clLime : clBlack ;
                 btULD_Feeding2  ->Caption       = IO_GetY(yULD_FeedingAC ) ? "ON"   : "OFF"   ;
                 btULD_Vaccum2   ->Font->Color   =(IO_GetY(yULD_PickVaccum1) || IO_GetY(yULD_PickVaccum2)) ? clLime : clBlack ;
                 btULD_Vaccum2   ->Caption       =(IO_GetY(yULD_PickVaccum1) || IO_GetY(yULD_PickVaccum2)) ? "ON"   : "OFF"   ;
                 btULD_Ejectr2   ->Font->Color   =(IO_GetY(yULD_PickEjectr1) || IO_GetY(yULD_PickEjectr2)) ? clLime : clBlack ;
                 btULD_Ejectr2   ->Caption       =(IO_GetY(yULD_PickEjectr1) || IO_GetY(yULD_PickEjectr2)) ? "ON"   : "OFF"   ;
        break;

        case 7 :

        break ;

    }

    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormDestroy(TObject *Sender)
{
    tmUpdateTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevInfo(bool bTo)
{
    if(bTo) {
        edColPitch      -> Text = OM.DevInfo.dColPitch     ;
        edRowPitch      -> Text = OM.DevInfo.dRowPitch     ;
        edColGroupCount -> Text = OM.DevInfo.iColGrCnt     ;
        edRowGroupCount -> Text = OM.DevInfo.iRowGrCnt     ;

        edColGroupGap   -> Text = OM.DevInfo.dColGrGap     ;
        edRowGroupGap   -> Text = OM.DevInfo.dRowGrGap     ;

        edColCount      -> Text = OM.DevInfo.iColCnt       ;
        edRowCount      -> Text = OM.DevInfo.iRowCnt       ;

        edLDRColCnt     -> Text = OM.DevInfo.iLDRColCnt    ;
        edLDRColPitch   -> Text = OM.DevInfo.dLDRColPitch  ;

        edLDRRowCnt     -> Text = OM.DevInfo.iLDRRowCnt    ;
        edLDRRowPitch   -> Text = OM.DevInfo.dLDRRowPitch  ;

        edULDColCnt     -> Text = OM.DevInfo.iULDColCnt    ;
        edULDColPitch   -> Text = OM.DevInfo.dULDColPitch  ;

        edULDRowCnt     -> Text = OM.DevInfo.iULDRowCnt    ;
        edULDRowPitch   -> Text = OM.DevInfo.dULDRowPitch  ;

        edULDPkgCnt     -> Text = OM.DevInfo.iULDPkgCnt    ;

        edVisnIndexId   -> Text = OM.DevInfo.sVisnIndexId  ;

        edInsColCnt     -> Text = OM.DevInfo.iColInspCnt   ;
        edInsRowCnt     -> Text = OM.DevInfo.iRowInspCnt   ;

    }
    else {
        OM.DevInfo.dColPitch    = edColPitch      -> Text.ToDouble() ;
        OM.DevInfo.dRowPitch    = edRowPitch      -> Text.ToDouble() ;

        OM.DevInfo.iColGrCnt    = edColGroupCount -> Text.ToInt()    ;
        OM.DevInfo.iRowGrCnt    = edRowGroupCount -> Text.ToInt()    ;

        OM.DevInfo.dColGrGap    = edColGroupGap   -> Text.ToDouble() ;
        OM.DevInfo.dRowGrGap    = edRowGroupGap   -> Text.ToDouble() ;

        OM.DevInfo.iColCnt      = edColCount      -> Text.ToInt()    ;
        OM.DevInfo.iRowCnt      = edRowCount      -> Text.ToInt()    ;

        OM.DevInfo.iLDRColCnt   = edLDRColCnt     -> Text.ToInt()    ;
        OM.DevInfo.iLDRRowCnt   = edLDRRowCnt     -> Text.ToInt()    ;
        OM.DevInfo.dLDRColPitch = edLDRColPitch   -> Text.ToDouble() ;
        OM.DevInfo.dLDRRowPitch = edLDRRowPitch   -> Text.ToDouble() ;

        OM.DevInfo.iULDColCnt   = edULDColCnt     -> Text.ToInt()    ;
        OM.DevInfo.iULDRowCnt   = edULDRowCnt     -> Text.ToInt()    ;
        OM.DevInfo.iULDPkgCnt   = edULDPkgCnt     -> Text.ToInt()    ;
        OM.DevInfo.dULDColPitch = edULDColPitch   -> Text.ToDouble() ;
        OM.DevInfo.dULDRowPitch = edULDRowPitch   -> Text.ToDouble() ;

        OM.DevInfo.sVisnIndexId = edVisnIndexId   -> Text            ;

        OM.DevInfo.iColInspCnt  = edInsColCnt     -> Text.ToInt()    ;
        OM.DevInfo.iRowInspCnt  = edInsRowCnt     -> Text.ToInt()    ;

    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::UpdateDevOptn(bool bTo)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::FormShow(TObject *Sender)
{
    UpdateDevInfo (toTabl);
    UpdateDevOptn (toTabl);
    PM.UpdatePstn (toTabl);
    tmUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::pcDeviceSetChange(TObject *Sender)
{

     switch ( pcDeviceSet -> ActivePageIndex ) {
         case 0 :                              break ;
         case 1 : rgUnitUser = rgLDRJogUnit ;
                  edUnitUser = edLDRJogUnit ;

                  FraMotor[miLDR_XPsh]->Parent = pnLDR_XPshMT ;
                  FraMotor[miLDR_YFed]->Parent = pnLDR_YFedMT ;
                  FraMotor[miLDR_XPck]->Parent = pnLDR_XPckMT ;
                  FraMotor[miLDR_ZPck]->Parent = pnLDR_ZPckMT ;

                  FraCylinder[aiLDR_Pusher    ]->Parent = pnLDR_Pusher    ;
                  FraCylinder[aiLDR_Rotator   ]->Parent = pnLDR_Rotator   ;
                  FraCylinder[aiLDR_Clamp     ]->Parent = pnLDR_Clamp     ;
                  FraCylinder[aiLDR_InMgz     ]->Parent = pnLDR_InMgz     ;
                  FraCylinder[aiLDR_OutMgz    ]->Parent = pnLDR_OutMgz    ;
                  pcLoader1 -> ActivePage = tabLD1_MT1;

                  break ;

         case 2 : rgUnitUser = rgWK1JogUnit  ;
                  edUnitUser = edWK1JogUnit  ;

                  FraMotor[miWK1_YFlp]->Parent = pnWK1_YFlpMT ;
                  FraMotor[miWK1_XIns]->Parent = pnWK1_XInsMT ;
                  FraMotor[miWRK_XVsn]->Parent = pnWRK_XVsnMT ;
                  FraMotor[miWRK_YVsn]->Parent = pnWRK_YVsnMT ;

                  FraCylinder[aiPRB_Stopper   ]->Parent = pnPRB_Stpr      ;
                  FraCylinder[aiWK1_Stopper   ]->Parent = pnWK1_Stpr      ;
                  FraCylinder[aiWK1_Align     ]->Parent = pnWK1_Algn      ;
                  FraCylinder[aiWK1_FlpCmp    ]->Parent = pnWK1_Flp       ;
                  FraCylinder[aiWK1_Ins       ]->Parent = pnWK1_Ins       ;
                  pcWork1 -> ActivePage = tabWK1_MT1 ;

                  break ;

         case 3 : rgUnitUser = rgWRKJogUnit  ;
                  edUnitUser = edWRKJogUnit  ;

                  FraMotor[miWK1_YFlp]->Parent = pnWK1_YFlpMT2 ;
                  FraMotor[miWK1_XIns]->Parent = pnWK1_XInsMT2 ;
                  FraMotor[miWRK_XVsn]->Parent = pnWRK_XVsnMT2 ;
                  FraMotor[miWRK_YVsn]->Parent = pnWRK_YVsnMT2 ;

                  FraCylinder[aiWK2_Stopper   ]->Parent = pnWK2_Stpr      ;
                  FraCylinder[aiWK2_Align     ]->Parent = pnWK2_Algn      ;
                  FraCylinder[aiWK3_Stopper   ]->Parent = pnWK3_Stpr      ;
                  FraCylinder[aiWK3_Align     ]->Parent = pnWK3_Algn      ;
                  pcWorkVsn -> ActivePage = tabWRK_MT2 ;

                  break ;

         case 4 : rgUnitUser = rgPSBJogUnit  ;
                  edUnitUser = edPSBJogUnit  ;

                  FraMotor[miPSB_XMrk]->Parent = pnPSB_XMrkMT ;
                  FraMotor[miPSB_YMrk]->Parent = pnPSB_YMrkMT ;

                  FraCylinder[aiPSB_Stopper   ]->Parent = pnPSB_Stpr      ;
                  FraCylinder[aiPSB_Align     ]->Parent = pnPSB_Algn      ;
                  FraCylinder[aiPSB_Marking   ]->Parent = pnPSB_Mrk       ;
                  FraCylinder[aiPSB_RjctOpen  ]->Parent = pnPSB_RjctFB    ;
                  FraCylinder[aiPSB_RjctUpDn  ]->Parent = pnPSB_RjctUD    ;

                  break ;

         case 5 : rgUnitUser = rgULDJogUnit1  ;
                  edUnitUser = edULDJogUnit1  ;

                  FraMotor[miULD_ZPck]->Parent = pnULD_ZPckMT ;
                  FraMotor[miULD_YPck]->Parent = pnULD_YPckMT ;
                  FraMotor[miULD_XStp]->Parent = pnULD_XStpMT ;
                  FraMotor[miULD_ZPpr]->Parent = pnULD_ZPprMT ;
                  FraMotor[miULD_ZTrI]->Parent = pnULD_ZTrIMT ;
                  FraMotor[miULD_ZTrO]->Parent = pnULD_ZTrOMT ;

                  FraCylinder[aiULD_TrayClamp     ]->Parent = pnULD_Cmp       ;
                  FraCylinder[aiULD_TrayClampPick ]->Parent = pnULD_Pck       ;
                  FraCylinder[aiULD_TrayClampMove ]->Parent = pnULD_Move      ;
                  FraCylinder[aiULD_TrayColSel    ]->Parent = pnULD_Cnvs      ;
                  FraCylinder[aiULD_TrayOpen      ]->Parent = pnULD_Open      ;
                  FraCylinder[aiULD_Hold          ]->Parent = pnULD_Hold      ;
                  FraCylinder[aiULD_PaperClamp    ]->Parent = pnULD_PprCmp    ;
                  FraCylinder[aiULD_PprRotator    ]->Parent = pnULD_PprRotator;
                  pcUnLoader1 -> ActivePage = tabULD1_1 ;

                  break ;

         case 6 : rgUnitUser = rgULDJogUnit2  ;
                  edUnitUser = edULDJogUnit2  ;

                  FraMotor[miULD_ZPck]->Parent = pnULD_ZPckMT2 ;
                  FraMotor[miULD_YPck]->Parent = pnULD_YPckMT2 ;
                  FraMotor[miULD_XStp]->Parent = pnULD_XStpMT2 ;
                  FraMotor[miULD_ZPpr]->Parent = pnULD_ZPprMT2 ;
                  FraMotor[miULD_ZTrI]->Parent = pnULD_ZTrIMT2 ;
                  FraMotor[miULD_ZTrO]->Parent = pnULD_ZTrOMT2 ;

                  FraCylinder[aiULD_TrayClamp     ]->Parent = pnULD_Cmp2       ;
                  FraCylinder[aiULD_TrayClampPick ]->Parent = pnULD_Pck2       ;
                  FraCylinder[aiULD_TrayClampMove ]->Parent = pnULD_Move2      ;
                  FraCylinder[aiULD_TrayColSel    ]->Parent = pnULD_Cnvs2      ;
                  FraCylinder[aiULD_TrayOpen      ]->Parent = pnULD_Open2      ;
                  FraCylinder[aiULD_Hold          ]->Parent = pnULD_Hold2      ;
                  FraCylinder[aiULD_PaperClamp    ]->Parent = pnULD_PprCmp2    ;
                  FraCylinder[aiULD_PprRotator    ]->Parent = pnULD_PprRotator2;
                  pcUnLoader2 -> ActivePage = tabULD2_2 ;

                  break ;

         case 7 : break ;
     }
}
//---------------------------------------------------------------------------
void __fastcall TFrmDeviceSet::btPRB_FeedingClick(TObject *Sender)
{
    int iSel = ((TBitBtn *)Sender) -> Tag ;
    bool bState =  IO_GetY(iSel);
    IO_SetY(iSel , !bState);
}
//---------------------------------------------------------------------------


void __fastcall TFrmDeviceSet::rgLDRJogUnitClick(TObject *Sender)
{
    if(rgUnitUser->ItemIndex == 0) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utJog ,0   );
    if(rgUnitUser->ItemIndex == 1) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,1   );
    if(rgUnitUser->ItemIndex == 2) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.5 );
    if(rgUnitUser->ItemIndex == 3) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.1 );
    if(rgUnitUser->ItemIndex == 4) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.05);
//    if(rgJogUnit->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,0.01);
    if(rgUnitUser->ItemIndex == 5) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utMove,StrToFloatDef(edUnitUser->Text,0.0));
//    if(rgJogUnit->ItemIndex == 7) for(int i=0;i<MAX_MOTR;i++) FraMotor[i]->SetUnit(utPitch,0  );

}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btULD_VaccumClick(TObject *Sender)
{
    if(IO_GetY(yULD_PickVaccum1) || IO_GetY(yULD_PickVaccum2)){
        IO_SetY(yULD_PickVaccum1 , false);
        IO_SetY(yULD_PickVaccum2 , false);
    }
    else if(!IO_GetY(yULD_PickVaccum1) || !IO_GetY(yULD_PickVaccum2)){
        IO_SetY(yULD_PickVaccum1 , true);
        IO_SetY(yULD_PickVaccum2 , true);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btULD_Ejectr2Click(TObject *Sender)
{
    if(IO_GetY(yULD_PickEjectr1) || IO_GetY(yULD_PickEjectr2)){
        IO_SetY(yULD_PickEjectr1 , false);
        IO_SetY(yULD_PickEjectr2 , false);
    }
    else if(!IO_GetY(yULD_PickEjectr1) || !IO_GetY(yULD_PickEjectr2)){
        IO_SetY(yULD_PickEjectr1 , true);
        IO_SetY(yULD_PickEjectr2 , true);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btPSB_TrmYnClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_Marking , ccBwd);
    if(!MotorCheck(miPSB_XMrk)) return ;
    if(!MotorCheck(miPSB_YMrk)) return ;

    iMrkManRow-- ;
    if(iMrkManCol < 0 ) iMrkManCol = 0 ;
    if(iMrkManRow < 0 ) iMrkManRow = 0 ;
    if(iMrkManCol >= OM.DevInfo.iColCnt) iMrkManCol = 0;
    if(iMrkManRow >= OM.DevInfo.iRowCnt) iMrkManRow = 0;
    MT_GoAbsMan(miPSB_XMrk , GetMotrPosTable(miPSB_XMrk , iMrkManRow , iMrkManCol)) ;
    MT_GoAbsMan(miPSB_YMrk , GetMotrPosTable(miPSB_YMrk , iMrkManRow , iMrkManCol)) ;
}

//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btPSB_TrmXnClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_Marking , ccBwd);
    if(!MotorCheck(miPSB_XMrk)) return ;
    if(!MotorCheck(miPSB_YMrk)) return ;

    iMrkManCol-- ;
    if(iMrkManCol < 0 ) iMrkManCol = 0 ;
    if(iMrkManRow < 0 ) iMrkManRow = 0 ;
    if(iMrkManCol >= OM.DevInfo.iColCnt) iMrkManCol = 0;
    if(iMrkManRow >= OM.DevInfo.iRowCnt) iMrkManRow = 0;
    MT_GoAbsMan(miPSB_XMrk , GetMotrPosTable(miPSB_XMrk , iMrkManRow , iMrkManCol)) ;
    MT_GoAbsMan(miPSB_YMrk , GetMotrPosTable(miPSB_YMrk , iMrkManRow , iMrkManCol)) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btPSB_TrmXpClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_Marking , ccBwd);
    if(!MotorCheck(miPSB_XMrk)) return ;
    if(!MotorCheck(miPSB_YMrk)) return ;

    iMrkManCol++ ;
    if(iMrkManCol < 0 ) iMrkManCol = 0 ;
    if(iMrkManRow < 0 ) iMrkManRow = 0 ;
    if(iMrkManCol >= OM.DevInfo.iColCnt) iMrkManCol = 0;
    if(iMrkManRow >= OM.DevInfo.iRowCnt) iMrkManRow = 0;
    MT_GoAbsMan(miPSB_XMrk , GetMotrPosTable(miPSB_XMrk , iMrkManRow , iMrkManCol)) ;
    MT_GoAbsMan(miPSB_YMrk , GetMotrPosTable(miPSB_YMrk , iMrkManRow , iMrkManCol)) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btPSB_TrmYpClick(TObject *Sender)
{
    if(MM.GetManNo()) {FM_MsgOk("Error","Doing Manual Cycle!!"); return ;}
    AT_MoveCyl(aiPSB_Marking , ccBwd);
    if(!MotorCheck(miPSB_XMrk)) return ;
    if(!MotorCheck(miPSB_YMrk)) return ;

    iMrkManRow++ ;
    if(iMrkManCol < 0 ) iMrkManCol = 0 ;
    if(iMrkManRow < 0 ) iMrkManRow = 0 ;
    if(iMrkManCol >= OM.DevInfo.iColCnt) iMrkManCol = 0;
    if(iMrkManRow >= OM.DevInfo.iRowCnt) iMrkManRow = 0;
    MT_GoAbsMan(miPSB_XMrk , GetMotrPosTable(miPSB_XMrk , iMrkManRow , iMrkManCol)) ;
    MT_GoAbsMan(miPSB_YMrk , GetMotrPosTable(miPSB_YMrk , iMrkManRow , iMrkManCol)) ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDeviceSet::btMrkLeftUpPsClick(TObject *Sender)
{
    int iTag = ((TButton *)Sender) -> Tag ;

    double dSttXPos , dSttYPos = 0.0 ;
    double dMaxXPos , dMaxYPos = 0.0 ;


    if(!MotorCheck(miPSB_XMrk)) return ;
    if(!MotorCheck(miPSB_YMrk)) return ;

    if(cbManualAction->ItemIndex == 0 ) {// Vision À§¿¡ ÀÖ´Âµ¥ ¶Ç ›§³Ä... dd
        dSttXPos =  PM.GetValue(miPSB_XMrk , pvPSB_XMrkVisnStart) ;
        dSttYPos =  PM.GetValue(miPSB_YMrk , pvPSB_YMrkVisnStart) ;
        dMaxXPos =  PM.GetValue(miPSB_XMrk , pvPSB_XMrkVisnStart) -
                    (OM.DevInfo.iColCnt - 1) * OM.DevInfo.dColPitch ;

        dMaxYPos =  PM.GetValue(miPSB_YMrk , pvPSB_YMrkVisnStart) +
                    (OM.DevInfo.iRowCnt - 1) * OM.DevInfo.dRowPitch ;
    }
    else {// Marking
        dSttXPos =  PM.GetValue(miPSB_XMrk , pvPSB_XMrkWorkStart) ;
        dSttYPos =  PM.GetValue(miPSB_YMrk , pvPSB_YMrkWorkStart) ;
        dMaxXPos =  PM.GetValue(miPSB_XMrk , pvPSB_XMrkWorkStart) -
                    (OM.DevInfo.iColCnt - 1) * OM.DevInfo.dColPitch ;

        dMaxYPos =  PM.GetValue(miPSB_YMrk , pvPSB_YMrkWorkStart) +
                    (OM.DevInfo.iRowCnt - 1) * OM.DevInfo.dRowPitch ;
    }


    int gc , gr ;
    int c, r ;

    if( OM.DevInfo.iColGrCnt) {
        gc = c / OM.DevInfo.iColGrCnt ;
        dMaxXPos = dMaxXPos + (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * gc ;
    }

    if( OM.DevInfo.iRowGrCnt) {
        gr = r / OM.DevInfo.iRowGrCnt ;
        dMaxYPos = dMaxYPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * gr;
    }

    if(!AT_Complete(aiPSB_Marking , ccBwd)) {FM_MsgOk("Error","Mark Pen     is Fwd!!");  return ; } // ¹Ì¾áÇÏÁö¸¸ ±ÍÂú dd

    if (iTag == 0 ) { //lt
        MT_GoAbsMan(miPSB_XMrk, dSttXPos);
        MT_GoAbsMan(miPSB_YMrk, dSttYPos);
        iMrkManCol = 0 ;
        iMrkManRow = 0 ;
    }
    else if (iTag == 1) { //rt
        MT_GoAbsMan(miPSB_XMrk, dMaxXPos ) ;
        MT_GoAbsMan(miPSB_YMrk, dSttYPos ) ;
        iMrkManCol = OM.DevInfo.iColCnt - 1 ;
        iMrkManRow = 0 ;
    }
    else if (iTag == 2) { //lb
        MT_GoAbsMan(miPSB_XMrk, dSttXPos );
        MT_GoAbsMan(miPSB_YMrk, dMaxYPos );
        iMrkManCol = 0 ;
        iMrkManRow = OM.DevInfo.iRowCnt - 1 ;
    }

    else if (iTag == 3) { //rb
        MT_GoAbsMan(miPSB_XMrk, dMaxXPos );
        MT_GoAbsMan(miPSB_YMrk, dMaxYPos );
        iMrkManCol = OM.DevInfo.iColCnt - 1 ;
        iMrkManRow = OM.DevInfo.iRowCnt - 1 ;
    }
}
//---------------------------------------------------------------------------

