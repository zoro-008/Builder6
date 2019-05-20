//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "VOptnMan.h"
#include "UserIni.h"
#include "UserFile.h"
#include "Vision.h"
#include "Common.h"
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
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fOriSinc     "   , VDevOptn.fOriSinc     );

        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfrWidth    "   , VDevOptn.fWfrWidth    );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfrHeight   "   , VDevOptn.fWfrHeight   );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfrGap      "   , VDevOptn.fWfrGap      );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfrAngle    "   , VDevOptn.fWfrAngle    ); 
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "iWfrCCPxCnt  "   , VDevOptn.iWfrCCPxCnt  );
//        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfrNcpPxAvr "   , VDevOptn.fWfrNcpPxAvr );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "iWfrFcpPxCnt "   , VDevOptn.iWfrFcpPxCnt );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "iWfrStdPxCnt "   , VDevOptn.iWfrStdPxCnt );


        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "iRlbPkgPxAvr "   , VDevOptn.iRlbPkgPxAvr ); 
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fRlbOffsetX  "   , VDevOptn.fRlbOffsetX  );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fRlbOffsetY  "   , VDevOptn.fRlbOffsetY  );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "iRlbPxCnt    "   , VDevOptn.iRlbPxCnt    );

        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fRlaWidth    "   , VDevOptn.fRlaWidth    );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fRlaHeight   "   , VDevOptn.fRlaHeight   );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fRlaGap      "   , VDevOptn.fRlaGap      );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fRlaAngle    "   , VDevOptn.fRlaAngle    );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fRlaAngleGap "   , VDevOptn.fRlaAngleGap );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fRlaOffsetX  "   , VDevOptn.fRlaOffsetX  );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fRlaOffsetY  "   , VDevOptn.fRlaOffsetY  );
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fRlaOffsetGap"   , VDevOptn.fRlaOffsetGap);

        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "fWfoSinc     "   , VDevOptn.fWfoSinc     );


    }
    else {
        //Save Device.
        //UserINI.ClearFile(sJobPath) ;
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fOriSinc     "   , VDevOptn.fOriSinc     );

        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfrWidth    "   , VDevOptn.fWfrWidth    );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfrHeight   "   , VDevOptn.fWfrHeight   );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfrGap      "   , VDevOptn.fWfrGap      );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfrAngle    "   , VDevOptn.fWfrAngle    );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "iWfrCCPxCnt  "   , VDevOptn.iWfrCCPxCnt  );
//        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfrNcpPxAvr "   , VDevOptn.fWfrNcpPxAvr );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "iWfrFcpPxCnt "   , VDevOptn.iWfrFcpPxCnt );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "iWfrStdPxCnt "   , VDevOptn.iWfrStdPxCnt );

        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "iRlbPkgPxAvr "   , VDevOptn.iRlbPkgPxAvr );  
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fRlbOffsetX  "   , VDevOptn.fRlbOffsetX  );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fRlbOffsetY  "   , VDevOptn.fRlbOffsetY  );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "iRlbPxCnt    "   , VDevOptn.iRlbPxCnt    );

        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fRlaWidth    "   , VDevOptn.fRlaWidth    );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fRlaHeight   "   , VDevOptn.fRlaHeight   );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fRlaGap      "   , VDevOptn.fRlaGap      );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fRlaAngle    "   , VDevOptn.fRlaAngle    );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fRlaAngleGap "   , VDevOptn.fRlaAngleGap );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fRlaOffsetX  "   , VDevOptn.fRlaOffsetX  );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fRlaOffsetY  "   , VDevOptn.fRlaOffsetY  );
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fRlaOffsetGap"   , VDevOptn.fRlaOffsetGap);

        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "fWfoSinc     "   , VDevOptn.fWfoSinc     );


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

        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bRlbFlipX    "   , VCmnOptn.bRlbFlipX    );
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bRlbFlipY    "   , VCmnOptn.bRlbFlipY    );
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bRlbFlipT    "   , VCmnOptn.bRlbFlipT    );
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bRlbNotUseAng"   , VCmnOptn.bRlbNotUseAng);

        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bWfoFlipX    "   , VCmnOptn.bWfoFlipX    );
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "bWfoFlipY    "   , VCmnOptn.bWfoFlipY    );

        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "iOriImageSave"   , VCmnOptn.iOriImageSave);
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "iWfrImageSave"   , VCmnOptn.iWfrImageSave);
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "iRlbImageSave"   , VCmnOptn.iRlbImageSave);
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "iRlaImageSave"   , VCmnOptn.iRlaImageSave);
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "iWfoImageSave"   , VCmnOptn.iWfoImageSave);

    }
    else {
        //Save Device.
        //UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfrFlipX    "   , VCmnOptn.bWfrFlipX    );
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfrFlipY    "   , VCmnOptn.bWfrFlipY    );
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfrFlipT    "   , VCmnOptn.bWfrFlipT    );

        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bRlbFlipX    "   , VCmnOptn.bRlbFlipX    );
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bRlbFlipY    "   , VCmnOptn.bRlbFlipY    );
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bRlbFlipT    "   , VCmnOptn.bRlbFlipT    );
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bRlbNotUseAng"   , VCmnOptn.bRlbNotUseAng); 

        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfoFlipX    "   , VCmnOptn.bWfoFlipX    );
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "bWfoFlipY    "   , VCmnOptn.bWfoFlipY    );

        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "iOriImageSave"   , VCmnOptn.iOriImageSave);
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "iWfrImageSave"   , VCmnOptn.iWfrImageSave);
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "iRlbImageSave"   , VCmnOptn.iRlbImageSave);
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "iRlaImageSave"   , VCmnOptn.iRlaImageSave);
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "iWfoImageSave"   , VCmnOptn.iWfoImageSave);


    }
}
