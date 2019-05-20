//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "TowerLamp.h"
#include "IOs.h"
#include "UserIni.h"


CTowerLamp TL;
//---------------------------------------------------------------------------
void __fastcall CTowerLamp::Init(void)
{
     m_LampInfo[ssInit    ].iRed = loLampFlick ; m_LampInfo[ssInit    ].iGrn = loLampFlick ; m_LampInfo[ssInit    ].iYel = loLampFlick ; m_LampInfo[ssInit    ].iBuzz = boBuzOff ;
     m_LampInfo[ssWarning ].iRed = loLampOff   ; m_LampInfo[ssWarning ].iGrn = loLampOff   ; m_LampInfo[ssWarning ].iYel = loLampFlick ; m_LampInfo[ssWarning ].iBuzz = boBuzOn  ;
     m_LampInfo[ssError   ].iRed = loLampOn    ; m_LampInfo[ssError   ].iGrn = loLampOff   ; m_LampInfo[ssError   ].iYel = loLampOff   ; m_LampInfo[ssError   ].iBuzz = boBuzOn  ;
     m_LampInfo[ssRunning ].iRed = loLampOff   ; m_LampInfo[ssRunning ].iGrn = loLampOn    ; m_LampInfo[ssRunning ].iYel = loLampOff   ; m_LampInfo[ssRunning ].iBuzz = boBuzOff ;
     m_LampInfo[ssStop    ].iRed = loLampOff   ; m_LampInfo[ssStop    ].iGrn = loLampOff   ; m_LampInfo[ssStop    ].iYel = loLampOn    ; m_LampInfo[ssStop    ].iBuzz = boBuzOff ;
     m_LampInfo[ssMaint   ].iRed = loLampOff   ; m_LampInfo[ssMaint   ].iGrn = loLampOff   ; m_LampInfo[ssMaint   ].iYel = loLampOff   ; m_LampInfo[ssMaint   ].iBuzz = boBuzOff ;
     m_LampInfo[ssRunWarn ].iRed = loLampOff   ; m_LampInfo[ssRunWarn ].iGrn = loLampOff   ; m_LampInfo[ssRunWarn ].iYel = loLampOff   ; m_LampInfo[ssRunWarn ].iBuzz = boBuzOff ;
     m_LampInfo[ssWorkEnd ].iRed = loLampFlick ; m_LampInfo[ssWorkEnd ].iGrn = loLampOff   ; m_LampInfo[ssWorkEnd ].iYel = loLampFlick ; m_LampInfo[ssWorkEnd ].iBuzz = boBuzOff ;
}

void __fastcall CTowerLamp::Update(EN_SEQ_STAT Stat)
{
    m_iSeqStat = Stat ;

    //Local Var.
    int iStat = m_bTestLampBuzz ? m_iTestStat : Stat;

    if (m_bFlick) { m_FlickOnTimer .Clear(); if (m_FlickOffTimer.OnDelay( m_bFlick , 500)) m_bFlick = false; }
    else          { m_FlickOffTimer.Clear(); if (m_FlickOnTimer .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }

    if(IO.GetTestMode()) return ;

    switch (m_LampInfo[iStat].iRed) {
        case loLampOff  : IO.SetY(m_LampAdd.iRedAdd,false   ) ; break;
        case loLampOn   : IO.SetY(m_LampAdd.iRedAdd,true    ) ; break;
        case loLampFlick: IO.SetY(m_LampAdd.iRedAdd,m_bFlick) ; break;
    }

    switch (m_LampInfo[iStat].iGrn) {
        case loLampOff  : IO.SetY(m_LampAdd.iGrnAdd,false   ) ; break;
        case loLampOn   : IO.SetY(m_LampAdd.iGrnAdd,true    ) ; break;
        case loLampFlick: IO.SetY(m_LampAdd.iGrnAdd,m_bFlick) ; break;
    }

    switch (m_LampInfo[iStat].iYel) {
        case loLampOff  : IO.SetY(m_LampAdd.iYelAdd,false   ) ; break;
        case loLampOn   : IO.SetY(m_LampAdd.iYelAdd,true    ) ; break;
        case loLampFlick: IO.SetY(m_LampAdd.iYelAdd,m_bFlick) ; break;
    }

    //Buzzer Control.
    IO.SetY(m_LampAdd.iSndAdd,(m_LampInfo[iStat].iBuzz == 1) && (!m_bBuzzOff || m_bTestLampBuzz));
}
//---------------------------------------------------------------------------
void __fastcall CTowerLamp::DisplayList(TStringGrid *Table)
{
    //Local Var.
    int        rCount;
    AnsiString Temp;

    //Check Error.
    if (Table == NULL) return;

    Table -> RowCount = MAX_SEQ_STAT + 1 ;
    Table -> ColCount = 5  ;

        //Set Col.
    Table -> Cells[0][0] = "STATUS  " ;  Table -> ColWidths[0] = 150 ;
    Table -> Cells[1][0] = "RED     " ;  Table -> ColWidths[1] = 120 ;
    Table -> Cells[2][0] = "YELLOW  " ;  Table -> ColWidths[2] = 120 ;
    Table -> Cells[3][0] = "GREEN   " ;  Table -> ColWidths[3] = 120 ;
    Table -> Cells[4][0] = "SOUND   " ;  Table -> ColWidths[4] = 120 ;

    Table -> Cells[0][1 ] = "Init    " ;
    Table -> Cells[0][2 ] = "Warning " ;
    Table -> Cells[0][3 ] = "Error   " ;
    Table -> Cells[0][4 ] = "Running " ;
    Table -> Cells[0][5 ] = "Stop    " ;
    Table -> Cells[0][6 ] = "Maint   " ;
    Table -> Cells[0][7 ] = "RunWarn " ;
    Table -> Cells[0][8 ] = "LotEnd  " ;

    AnsiString sRed , sGrn , sYel , sSnd;

    for (int i = 0 ; i < MAX_SEQ_STAT ; i++) {
        switch (m_LampInfo[i].iRed)
        {
            case loLampOff    : sRed = "Off  " ; break ;
            case loLampOn     : sRed = "On   " ; break ;
            case loLampFlick  : sRed = "Flick" ; break ;
        }

        switch (m_LampInfo[i].iYel)
        {
            case loLampOff    : sYel = "Off  " ; break ;
            case loLampOn     : sYel = "On   " ; break ;
            case loLampFlick  : sYel = "Flick" ; break ;
        }

        switch (m_LampInfo[i].iGrn)
        {
            case loLampOff    : sGrn = "Off  " ; break ;
            case loLampOn     : sGrn = "On   " ; break ;
            case loLampFlick  : sGrn = "Flick" ; break ;
        }

        switch (m_LampInfo[i].iBuzz)
        {
            case boBuzOff    : sSnd = "Off  " ; break ;
            case boBuzOn     : sSnd = "On   " ; break ;
        }

        //Display.
        Table -> Cells[1][i + 1] = sRed ;
        Table -> Cells[2][i + 1] = sYel ;
        Table -> Cells[3][i + 1] = sGrn ;
        Table -> Cells[4][i + 1] = sSnd ;
        }
}

void __fastcall CTowerLamp::DisplayStatus (TLabel * Red  , TLabel * Yel  , TLabel * Grn  , TPanel * Snd)
{
    //Local Var.
    int iStat = m_bTestLampBuzz ? m_iTestStat : m_iSeqStat ;

    switch (m_LampInfo[iStat].iRed)
    {
        case loLampOff   : Red -> Color = clGray                    ; break;
        case loLampOn    : Red -> Color = clRed                     ; break;
        case loLampFlick : Red -> Color = m_bFlick ? clRed : clGray ; break;
    }

    switch (m_LampInfo[iStat].iYel)
    {
        case loLampOff   : Yel -> Color = clGray                       ; break;
        case loLampOn    : Yel -> Color = clYellow                     ; break;
        case loLampFlick : Yel -> Color = m_bFlick ? clYellow : clGray ; break;
    }

    switch (m_LampInfo[iStat].iGrn)
    {
        case loLampOff   : Grn -> Color = clGray                      ; break;
        case loLampOn    : Grn -> Color = clGreen                     ; break;
        case loLampFlick : Grn -> Color = m_bFlick ? clGreen : clGray ; break;
    }

    switch (m_LampInfo[iStat].iBuzz)
    {
        case boBuzOff : Snd -> Color = clGray ; break;
        case boBuzOn  : Snd -> Color = clAqua ; break;
    }

}

void __fastcall CTowerLamp::DisplayProperty(int ToTble , TRadioButton *RedOn , TRadioButton *RedOff , TRadioButton *RedFlick ,
                                                         TRadioButton *YelOn , TRadioButton *YelOff , TRadioButton *YelFlick ,
                                                         TRadioButton *GrnOn , TRadioButton *GrnOff , TRadioButton *GrnFlick ,
                                                         TRadioButton *BuzOn , TRadioButton *BuzOff ,
                                                         TEdit        *RedAdd, TEdit        *YelAdd , TEdit        *GrnAdd   , TEdit        *SndAdd ,
                                                         TStringGrid  *Table )
{
    EN_SEQ_STAT Stat = (EN_SEQ_STAT)(Table -> Row - 1);
    m_iTestStat = Stat ;

    if(ToTble)
    {
         RedOn     -> Checked = m_LampInfo[Stat].iRed  == loLampOn    ;
         RedOff    -> Checked = m_LampInfo[Stat].iRed  == loLampOff   ;
         RedFlick  -> Checked = m_LampInfo[Stat].iRed  == loLampFlick ;
         YelOn     -> Checked = m_LampInfo[Stat].iYel  == loLampOn    ;
         YelOff    -> Checked = m_LampInfo[Stat].iYel  == loLampOff   ;
         YelFlick  -> Checked = m_LampInfo[Stat].iYel  == loLampFlick ;
         GrnOn     -> Checked = m_LampInfo[Stat].iGrn  == loLampOn    ;
         GrnOff    -> Checked = m_LampInfo[Stat].iGrn  == loLampOff   ;
         GrnFlick  -> Checked = m_LampInfo[Stat].iGrn  == loLampFlick ;
         BuzOn     -> Checked = m_LampInfo[Stat].iBuzz == boBuzOn     ;
         BuzOff    -> Checked = m_LampInfo[Stat].iBuzz == boBuzOff    ;
         RedAdd    -> Text    = m_LampAdd.iRedAdd                     ;
         YelAdd    -> Text    = m_LampAdd.iYelAdd                     ;
         GrnAdd    -> Text    = m_LampAdd.iGrnAdd                     ;
         SndAdd    -> Text    = m_LampAdd.iSndAdd                     ;
    }
    else
    {
              if(RedOn    -> Checked) m_LampInfo[Stat].iRed  = loLampOn    ;
         else if(RedOff   -> Checked) m_LampInfo[Stat].iRed  = loLampOff   ;
         else if(RedFlick -> Checked) m_LampInfo[Stat].iRed  = loLampFlick ;
              if(YelOn    -> Checked) m_LampInfo[Stat].iYel  = loLampOn    ;
         else if(YelOff   -> Checked) m_LampInfo[Stat].iYel  = loLampOff   ;
         else if(YelFlick -> Checked) m_LampInfo[Stat].iYel  = loLampFlick ;
              if(GrnOn    -> Checked) m_LampInfo[Stat].iGrn  = loLampOn    ;
         else if(GrnOff   -> Checked) m_LampInfo[Stat].iGrn  = loLampOff   ;
         else if(GrnFlick -> Checked) m_LampInfo[Stat].iGrn  = loLampFlick ;
              if(BuzOn    -> Checked) m_LampInfo[Stat].iBuzz = boBuzOn     ;
         else if(BuzOff   -> Checked) m_LampInfo[Stat].iBuzz = boBuzOff    ;

         m_LampAdd.iRedAdd = RedAdd -> Text.ToInt() ;
         m_LampAdd.iYelAdd = YelAdd -> Text.ToInt() ;
         m_LampAdd.iGrnAdd = GrnAdd -> Text.ToInt() ;
         m_LampAdd.iSndAdd = SndAdd -> Text.ToInt() ;
    }
}
//---------------------------------------------------------------------------
void __fastcall CTowerLamp::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString Temp1 , Temp2;
    AnsiString Path;

    int R,Y,G,B ;

    //Make Dir.
    if (!DirectoryExists(ExtractFilePath(Application -> ExeName) + "Util"))
        CreateDir(ExtractFilePath(Application -> ExeName) + "Util");

    Path = ExtractFilePath(Application->ExeName) + "Util\\TowerLamp.INI";
    if (!FileExists(Path))  Init();
    if (IsLoad) {
        for (int i = 0 ; i < MAX_SEQ_STAT  ; i++) {
            Temp1.printf("TOWER_LAMP(%04d)" , i);
            UserINI.Load(Path.c_str(), "iRed " , Temp1.c_str() , &R);
            UserINI.Load(Path.c_str(), "iYel " , Temp1.c_str() , &Y);
            UserINI.Load(Path.c_str(), "iGrn " , Temp1.c_str() , &G);
            UserINI.Load(Path.c_str(), "iBuzz" , Temp1.c_str() , &B);

            m_LampInfo[i].iRed  = (EN_LAMP_OPER)R ;
            m_LampInfo[i].iYel  = (EN_LAMP_OPER)Y ;
            m_LampInfo[i].iGrn  = (EN_LAMP_OPER)G ;
            m_LampInfo[i].iBuzz = (EN_BUZZ_OPER)B ;
            }
        UserINI.Load(Path.c_str(), "m_LampAdd" , "iRedAdd" , &m_LampAdd.iRedAdd);
        UserINI.Load(Path.c_str(), "m_LampAdd" , "iYelAdd" , &m_LampAdd.iYelAdd);
        UserINI.Load(Path.c_str(), "m_LampAdd" , "iGrnAdd" , &m_LampAdd.iGrnAdd);
        UserINI.Load(Path.c_str(), "m_LampAdd" , "iSndAdd" , &m_LampAdd.iSndAdd);

        }
    else {
        for (int i = 0 ; i < MAX_SEQ_STAT  ; i++) {
            Temp1.printf("TOWER_LAMP(%04d)" , i);
            UserINI.Save(Path.c_str(), "iRed " , Temp1.c_str() , m_LampInfo[i].iRed  );
            UserINI.Save(Path.c_str(), "iYel " , Temp1.c_str() , m_LampInfo[i].iYel  );
            UserINI.Save(Path.c_str(), "iGrn " , Temp1.c_str() , m_LampInfo[i].iGrn  );
            UserINI.Save(Path.c_str(), "iBuzz" , Temp1.c_str() , m_LampInfo[i].iBuzz );
            }

        UserINI.Save(Path.c_str(), "m_LampAdd" , "iRedAdd" , m_LampAdd.iRedAdd );
        UserINI.Save(Path.c_str(), "m_LampAdd" , "iYelAdd" , m_LampAdd.iYelAdd );
        UserINI.Save(Path.c_str(), "m_LampAdd" , "iGrnAdd" , m_LampAdd.iGrnAdd );
        UserINI.Save(Path.c_str(), "m_LampAdd" , "iSndAdd" , m_LampAdd.iSndAdd );
        }
}
//---------------------------------------------------------------------------


