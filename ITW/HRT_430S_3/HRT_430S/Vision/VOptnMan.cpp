//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "VOptnMan.h"
#include "Common.h"
#include "UserIni.h"
#include "UserFile.h"
#include "SLogUnit.h"
#include "VisionBase.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CVOptnMan VOM;

//---------------------------------------------------------------------------
CVOptnMan::CVOptnMan(void)
{
    //Init. Buffer.
    memset(&VDevPara  , 0 , sizeof(CVDevOptn ));
    memset(&VDevOptn  , 0 , sizeof(CVDevOptn ));
    memset(&VCmnOptn  , 0 , sizeof(CVCmnOptn ));

}

//---------------------------------------------------------------------------
CVOptnMan::~CVOptnMan(void)
{
    static int i = 0 ;
    //이거 넣으면 왠지 모를 메모리누수발생.
    //Trace("",AnsiString(i).c_str());
    i++;
}

//---------------------------------------------------------------------------
void CVOptnMan::LoadJobFile(bool _bLoad)
{
    LoadVDevOptn(_bLoad ); // LoadVDevPara보다 꼭 위에 있어야 함.
    LoadVDevPara(_bLoad );
    LoadVCmnOptn(_bLoad );

    VISN[viOri] -> LoadImg(_bLoad);

}

//---------------------------------------------------------------------------
void CVOptnMan::LoadVDevPara(bool _bLoad)
{
    VISN[viOri]->Load(_bLoad);
}

//---------------------------------------------------------------------------
void CVOptnMan::LoadVDevOptn(bool _bLoad)
{

    //Local Var.
    TUserINI   UserINI;

    //Set Dir.
    AnsiString sJobPath = g_sJobFilePath + "VisnDevOption.INI" ;

    if(_bLoad) {
        //Load Device.
        UserINI.Load(sJobPath.c_str()  , "VDevOptn"  , "dOriOriSinc  " , VDevOptn.dOriOriSinc  );
    }
    else {
        //Save Device.
        UserINI.ClearFile(sJobPath) ;
        UserINI.Save(sJobPath.c_str()  , "VDevOptn"  , "dOriOriSinc  " , VDevOptn.dOriOriSinc  );
    }

    //칩 갯수 변경 됐을 것을 대비.
    VISN[viOri]->ApplyDevOptn();


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
    sPath= Path + "VisnCmnOptn.INI" ; //이함수의 부터.

    if(_bLoad) {
        //Load Device.
        UserINI.Load(sPath.c_str()  , "VCmnOptn"  , "iOriImageSave"   , VCmnOptn.iOriImageSave);
    }
    else {
        //Save Device.
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath.c_str()  , "VCmnOptn"  , "iOriImageSave"   , VCmnOptn.iOriImageSave);
    }
}
