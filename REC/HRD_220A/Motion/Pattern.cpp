//---------------------------------------------------------------------------
#pragma hdrstop

#include "Pattern.h"
#include "UserIni.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CPattern PTT ;
//---------------------------------------------------------------------------
CPattern::CPattern()
{

}
//---------------------------------------------------------------------------
CPattern::~CPattern()
{

}

void CPattern::Init()
{
    LoadLastInfo() ;
    Load(m_sCrntDev);

}

//---------------------------------------------------------------------------
double CPattern::GetPosX(int _iNo)
{
    if(_iNo < 0          ) return 0.0 ;
    if(_iNo > m_iPosCnt-1) return 0.0 ;


    return m_dXPos[_iNo] ;
}
//---------------------------------------------------------------------------
double CPattern::GetPosY(int _iNo)
{
    if(_iNo < 0          ) return 0.0 ;
    if(_iNo > m_iPosCnt-1) return 0.0 ;

    return m_dYPos[_iNo] ;
}
//---------------------------------------------------------------------------
double CPattern::GetScalePosX(int _iNo)
{
    if(_iNo < 0          ) return 0.0 ;
    if(_iNo > m_iPosCnt-1) return 0.0 ;

    //return m_dXScalePos[_iNo];
    return m_dXPos[_iNo] * m_dXScale + m_dXSttOffset;
}
//---------------------------------------------------------------------------
double CPattern::GetScalePosY(int _iNo)
{
    if(_iNo < 0          ) return 0.0 ;
    if(_iNo > m_iPosCnt-1) return 0.0 ;

    //return m_dYScalePos[_iNo];
    return m_dYPos[_iNo] * m_dYScale + m_dYSttOffset;

}
//---------------------------------------------------------------------------
double CPattern::GetScaleX()
{
    return m_dXScale;
}
//---------------------------------------------------------------------------
double CPattern::GetScaleY()
{
    return m_dYScale;
}
//---------------------------------------------------------------------------
double CPattern::GetSpeed()
{
    return m_dSpeed ;
}
//---------------------------------------------------------------------------
double CPattern::GetSttOffsetX()
{
    return m_dXSttOffset ;
}
//---------------------------------------------------------------------------
double CPattern::GetSttOffsetY()
{
    return m_dYSttOffset ;
}
//---------------------------------------------------------------------------
double CPattern::GetAcc()
{
    return m_dAcc;
}
//---------------------------------------------------------------------------
double CPattern::GetDec()
{
    return m_dDec;
}
//---------------------------------------------------------------------------
bool CPattern::GetDispOn(int _iNo)
{
    if(_iNo < 0          ) return false ;
    if(_iNo > m_iPosCnt-1) return false ;

    return m_bDispOn[_iNo];
}



//---------------------------------------------------------------------------
void CPattern::SetScale(double _dX, double _dY)
{
    //double dCvsData = 0.01 ;

    if(_dX > 1) _dX = 1;
    if(_dY > 1) _dY = 1;
    m_dXScale = _dX;
    m_dYScale = _dY;
}
//---------------------------------------------------------------------------
void CPattern::SetPosX(int _iNo, double _dPos)
{
    if(_iNo < 0          ) return  ;
    if(_iNo > m_iPosCnt-1) return  ;

    m_dXPos[_iNo] = _dPos ;
}
//---------------------------------------------------------------------------
void CPattern::SetPosY(int _iNo, double _dPos)
{
    if(_iNo < 0          ) return  ;
    if(_iNo > m_iPosCnt-1) return  ;

    m_dYPos[_iNo] = _dPos ;
}

void CPattern::SetPosCnt(int _iCnt)
{
    m_iPosCnt = _iCnt;
}
//---------------------------------------------------------------------------
void CPattern::SetAccDec(double _dAcc , double _dDec)
{
    m_dAcc = _dAcc;
    m_dDec = _dDec;
}
//---------------------------------------------------------------------------
void CPattern::SetSttOffset(double _dX, double _dY)
{
    m_dXSttOffset = _dX ;
    m_dYSttOffset = _dY ;
}
//---------------------------------------------------------------------------
void CPattern::SetSpeed(double _dVal)
{
    m_dSpeed = _dVal ;
}

void CPattern::SetDispOn(int _iNo , bool _bOn)
{
    if(_iNo < 0          ) return  ;
    if(_iNo > m_iPosCnt-1) return  ;

    m_bDispOn[_iNo] = _bOn ;

}
//---------------------------------------------------------------------------
void CPattern::Load(AnsiString _sDevName)
{
    TUserINI   UserINI ;
    AnsiString sPath, sNum ;
    int iNum ;

    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\PatternPara.pat";

    LoadPat(sPath);
}
//---------------------------------------------------------------------------
void CPattern::Save(AnsiString _sDevName)
{
    TUserINI   UserINI ;
    AnsiString sPath, sNum ;
    int iNum ;

    sPath = EXE_FOLDER + "JobFile\\" + _sDevName + "\\PatternPara.pat";

    SavePat(sPath);


}
//---------------------------------------------------------------------------
void CPattern::LoadPat(AnsiString _sFileName)
{
    TUserINI   UserINI ;
    AnsiString sPath, sNum ;
    int iNum ;

    sPath = _sFileName;

    for(iNum = 0; iNum <= MAX_PATTERN_POS; iNum++) {
        sNum = IntToStr(iNum);
        UserINI.Load(sPath.c_str() , "PatternPara" , "m_dXPos"      + sNum, m_dXPos     [iNum]);
        UserINI.Load(sPath.c_str() , "PatternPara" , "m_dYPos"      + sNum, m_dYPos     [iNum]);
        UserINI.Load(sPath.c_str() , "PatternPara" , "m_bDispOn"    + sNum, m_bDispOn   [iNum]);
    }

    UserINI.Load(sPath.c_str() , "PatternPara" , "m_dXScale"     , m_dXScale    );
    UserINI.Load(sPath.c_str() , "PatternPara" , "m_dYScale"     , m_dYScale    );

    UserINI.Load(sPath.c_str() , "PatternPara" , "m_dXSttOffset" , m_dXSttOffset);
    UserINI.Load(sPath.c_str() , "PatternPara" , "m_dYSttOffset" , m_dYSttOffset);

    UserINI.Load(sPath.c_str() , "PatternPara" , "m_dSpeed"      , m_dSpeed     );
    UserINI.Load(sPath.c_str() , "PatternPara" , "m_iPosCnt"     , m_iPosCnt    );

    UserINI.Load(sPath.c_str() , "PatternPara" , "m_dAcc"        , m_dAcc       );
    UserINI.Load(sPath.c_str() , "PatternPara" , "m_dDec"        , m_dDec       );



}
void CPattern::SavePat(AnsiString _sFileName)
{
    TUserINI   UserINI ;
    AnsiString sPath, sNum ;
    int iNum ;

    sPath = _sFileName;

    for(iNum = 0; iNum <= MAX_PATTERN_POS; iNum++) {
        sNum = IntToStr(iNum);
        UserINI.Save(sPath.c_str() , "PatternPara" , "m_dXPos"      + sNum, m_dXPos     [iNum]);
        UserINI.Save(sPath.c_str() , "PatternPara" , "m_dYPos"      + sNum, m_dYPos     [iNum]);
        UserINI.Save(sPath.c_str() , "PatternPara" , "m_bDispOn"    + sNum, m_bDispOn   [iNum]);
    }

    UserINI.Save(sPath.c_str() , "PatternPara" , "m_dXScale"     , m_dXScale    );
    UserINI.Save(sPath.c_str() , "PatternPara" , "m_dYScale"     , m_dYScale    );

    UserINI.Save(sPath.c_str() , "PatternPara" , "m_dXSttOffset" , m_dXSttOffset);
    UserINI.Save(sPath.c_str() , "PatternPara" , "m_dYSttOffset" , m_dYSttOffset);

    UserINI.Save(sPath.c_str() , "PatternPara" , "m_dSpeed"      , m_dSpeed     );
    UserINI.Save(sPath.c_str() , "PatternPara" , "m_iPosCnt"     , m_iPosCnt    );

    UserINI.Save(sPath.c_str() , "PatternPara" , "m_dAcc"        , m_dAcc       );
    UserINI.Save(sPath.c_str() , "PatternPara" , "m_dDec"        , m_dDec       );

    UserINI.Save(sPath.c_str() , "PatternPara" , "m_bDispOn"     , m_bDispOn    );

}

int CPattern::GetPosCnt()
{
    return m_iPosCnt ;

}

void CPattern::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO" , "Device"  , m_sCrntDev );

    if (m_sCrntDev == "") m_sCrntDev  = "NONE";
}




