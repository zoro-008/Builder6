//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "VOptnMan.h"
#include "UserIni.h"
#include "UserFile.h"
#include "Vision.h"
#include "SLogUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CVOptnMan VOM;

//---------------------------------------------------------------------------
CVOptnMan::CVOptnMan(void)
{
    //Init. Buffer.
    memset(&VDevOptn  , 0 , sizeof(CVDevOptn ));
}

//---------------------------------------------------------------------------
CVOptnMan::~CVOptnMan(void)
{
    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;
}

//---------------------------------------------------------------------------
void CVOptnMan::LoadJobFile(bool _bLoad)
{
    LoadVDevOptn(_bLoad );
    LoadVCmnOptn(_bLoad );
}

//---------------------------------------------------------------------------
//void CVOptnMan::LoadJobFile(bool _bLoad , int _iVisnId , AnsiString _sDevName)
//{
//
//    //알고리즘 파라 로드.
//    if(_sDevName == "") _sDevName = m_sJobName ;
//    AnsiString Path = GetExeDir("JobFile\\") + _sDevName + "\\Vision\\";
//
//    VISN[_iVisnId].LoadJobFile(_bLoad , Path);
//
//    m_sJobName = _sDevName ;
//
//    LoadVDevOptn(_bLoad ,_sDevName);
//    LoadVCmnOptn(_bLoad );
//}

//---------------------------------------------------------------------------
void CVOptnMan::LoadVDevOptn(bool _bLoad)
{

    //Local Var.
    TUserINI   UserINI;

    //Set Dir.
    AnsiString sJobPath = g_sJobFilePath + "VisnOption.INI" ;

    if(_bLoad) {
        //Load Device.
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfrWidth     " , VDevOptn.fWfrWidth     );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfrHeight    " , VDevOptn.fWfrHeight    );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfrGap       " , VDevOptn.fWfrGap       );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfrAngle     " , VDevOptn.fWfrAngle     );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfoOriSinc   " , VDevOptn.fWfoOriSinc   );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "iWfpPxCnt     " , VDevOptn.iWfpPxCnt     );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "iWfpPxHigh    " , VDevOptn.iWfpPxHigh    );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "iWfrCCPxCnt   " , VDevOptn.iWfrCCPxCnt   );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfrOriSinc   " , VDevOptn.fWfrOriSinc   );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "bWfoIgnRctInsp" , VDevOptn.bWfoIgnRctInsp);
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "bWfrIgnRctInsp" , VDevOptn.bWfrIgnRctInsp);
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "iEndExposure  " , VDevOptn.iEndExposure  );
    }

    else {
        //Save Device.
        //UserINI.ClearFile(sJobPath) ;
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfrWidth     " , VDevOptn.fWfrWidth     );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfrHeight    " , VDevOptn.fWfrHeight    );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfrGap       " , VDevOptn.fWfrGap       );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfrAngle     " , VDevOptn.fWfrAngle     );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfoOriSinc   " , VDevOptn.fWfoOriSinc   );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "iWfpPxCnt     " , VDevOptn.iWfpPxCnt     );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "iWfpPxHigh    " , VDevOptn.iWfpPxHigh    );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "iWfrCCPxCnt   " , VDevOptn.iWfrCCPxCnt   );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfrOriSinc   " , VDevOptn.fWfrOriSinc   );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "bWfoIgnRctInsp" , VDevOptn.bWfoIgnRctInsp);
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "bWfrIgnRctInsp" , VDevOptn.bWfrIgnRctInsp);
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "iEndExposure  " , VDevOptn.iEndExposure  );
    }
}

//---------------------------------------------------------------------------
void CVOptnMan::LoadVCmnOptn(bool _bLoad )
{

    //알고리즘 파라 로드.
    AnsiString Path = GetExeDir("Util\\Vision\\");

    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= Path + "VCmnOptn.INI" ; //이함수의 부터.

    if(_bLoad) {
        //Load Device.
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bWfrFlipX    "   , VCmnOptn.bWfrFlipX    );
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bWfrFlipY    "   , VCmnOptn.bWfrFlipY    );
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bWfrFlipT    "   , VCmnOptn.bWfrFlipT    );

        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bWfoFlipX    "   , VCmnOptn.bWfoFlipX    );
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bWfoFlipY    "   , VCmnOptn.bWfoFlipY    );
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bWfoFlipT    "   , VCmnOptn.bWfoFlipT    );

        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bWfpFlipX    "   , VCmnOptn.bWfpFlipX    );
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bWfpFlipY    "   , VCmnOptn.bWfpFlipY    );

        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "iWfrImageSave"   , VCmnOptn.iWfrImageSave);
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "iWfoImageSave"   , VCmnOptn.iWfoImageSave);
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "iWfpImageSave"   , VCmnOptn.iWfpImageSave);
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "iCalImageSave"   , VCmnOptn.iCalImageSave);

    }
    else {
        //Save Device.
        //UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfrFlipX    "   , VCmnOptn.bWfrFlipX    );
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfrFlipY    "   , VCmnOptn.bWfrFlipY    );
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfrFlipT    "   , VCmnOptn.bWfrFlipT    );

        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfoFlipX    "   , VCmnOptn.bWfoFlipX    ); 
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfoFlipY    "   , VCmnOptn.bWfoFlipY    );
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfoFlipT    "   , VCmnOptn.bWfoFlipT    );

        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfpFlipX    "   , VCmnOptn.bWfpFlipX    );
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfpFlipY    "   , VCmnOptn.bWfpFlipY    );

        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "iWfrImageSave"   , VCmnOptn.iWfrImageSave);
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "iWfoImageSave"   , VCmnOptn.iWfoImageSave);
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "iWfpImageSave"   , VCmnOptn.iWfpImageSave);
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "iCalImageSave"   , VCmnOptn.iCalImageSave);

    }
}
