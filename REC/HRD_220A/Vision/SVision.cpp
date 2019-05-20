//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <stdio.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------


//extern "C" SVA_API bool __stdcall InitVisn     (void * hMainWnd , void * hParaWnd , char * _sDevName){/*return VSM.InitVisn     (hMainWnd , hParaWnd , _sDevName );*/}
//extern "C" SVA_API bool __stdcall DelVisn      (                                                    ){/*return VSM.DelVisn      (                                );*/}
//extern "C" SVA_API void __stdcall SetImgForm   (void * hImgBaseWnd  , bool _bSettingMode            ){/*       VSM.SetImgForm   (hImgBaseWnd , _bSettingMode     );*/}
//extern "C" SVA_API void __stdcall LoadJobFile  (bool _bLoad , char * _sDevName                      ){/*       VSM.LoadJobFile  (_bLoad , _sDevName              );*/}
//extern "C" SVA_API void __stdcall Grab         (int        _iVisnId                                 ){/*       VSM.Grab         (_iVisnId                        );*/}
//extern "C" SVA_API void __stdcall Live         (int        _iVisnId , bool _bOn                     ){/*       VSM.Live         (_iVisnId , _bOn                 );*/}
//extern "C" SVA_API bool __stdcall GetGrabEnd   (int        _iVisnId                                 ){/*return VSM.GetGrabEnd   (_iVisnId                        );*/}
//extern "C" SVA_API void __stdcall SetLight     (int        _iVisnId                                 ){/*       VSM.SetLight     (_iVisnId                        );*/}
//extern "C" SVA_API void __stdcall Inspect      (int        _iVisnId , bool _bUseThread              ){/*       VSM.Inspect      (_iVisnId , _bUseThread          );*/}
//extern "C" SVA_API bool __stdcall GetInspEnd   (int        _iVisnId                                 ){/*return VSM.GetInspEnd   (_iVisnId                        );*/}
//extern "C" SVA_API int  __stdcall GetRslt      (int        _iVisnId , bool  * _tRslt                ){/*       VSM.GetRslt      (_iVisnId , _tRslt               );*/}
//extern "C" SVA_API void __stdcall SetLevel     (int        _iLevel                                  ){/*return VSM.SetLevel     (_iLevel                         );*/}
//extern "C" SVA_API void __stdcall SetAutorun   (bool _bAutorun                                      ){/*       VSM.SetAutorun   (_bAutorun                       );*/}
//extern "C" SVA_API void __stdcall SetActiveVisn(int  _iVisnId                                       ){/*       VSM.SetActiveVisn(_iVisnId                        );*/}
#include "SVision.h"
#include "CamUnit.h"
#include "LightUnit.h"

#include "UnitVision.h"
#include "FormImage.h"

#include "GlobalMan.h"
#include "FormJob.h"

class CVisionSystem {
    public :
        CVisionSystem(){
            m_FrmImage = NULL;
            m_pVision  = NULL;

            m_bRsltRet = false; //�̰� �ΰԴ� �ֹߵɼ��� �־ �۷ι��� ��Ƽ� �Ѱ��ش�.
            m_sRsltStr = "";


        }

        bool        m_bRsltRet ;
        String      m_sRsltStr ;


        TFrmImage * m_FrmImage ;
        CVision   * m_pVision  ;
};

CVisionSystem ** VSN = NULL ;
int              g_iVisionCnt ;

void LoadJobFileAllVision(char * _sDevName)
{
    //������ �н� �۷ι� ���.
    GM.SetCrntJobFile(_sDevName);
    GM.Save();

    for(int i = 0 ; i < g_iVisionCnt ; i++) {
        VSN[i]->m_pVision->LoadSave(true , _sDevName);
        VSN[i]->m_pVision->SetActivePkg(0);
        VSN[i]->m_FrmImage->SetVision(VSN[i]->m_pVision);
        VSN[i]->m_FrmImage->OnPaint();
    }


}

extern "C"{
    SVA_API bool __stdcall InitVisn()
    {
            //��Ȳ ���� �ּ�ó������.
        TUserINI Ini ;
        String   sName ;
        int      iVisnCnt ;
        String sIniPath  = GM.GetGlobalFolder()+ "VisionProperty.ini" ;
        Ini.Load(sIniPath, "Common" , "iVisionCount" , iVisnCnt);

        g_iVisionCnt = iVisnCnt ;
        VSN = new CVisionSystem * [iVisnCnt] ;
        FrmJob = new TFrmJob(NULL);
        FrmJob -> SetLoadJobFileFunc(LoadJobFileAllVision);
        for(int i = 0 ; i < iVisnCnt ; i++) {
            VSN[i] = new CVisionSystem();

            VSN[i]->m_FrmImage = new TFrmImage(NULL); VSN[i]->m_FrmImage -> Init();
            VSN[i]->m_pVision  = new CVision  (    ); VSN[i]->m_pVision  -> Init();


            VSN[i]->m_FrmImage -> SetVision(VSN[i]->m_pVision);


            Ini.Load(sIniPath, "Vision"+String(i) , "sName" , sName);
            VSN[i]-> m_pVision ->SetName(sName) ;
        }

        //ī�޶� ���� Ȯ���ؼ� �ʱ�ȭ �c.
        CCam::InitCam();
        CLight::InitLight();

        LoadJobFileAllVision(GM.Stat.sCrntJobFile.c_str());

        return true ;
    }



    SVA_API bool __stdcall DelVisn( )
    {
        for(int i = 0 ; i < g_iVisionCnt ; i++) {

            VSN[i]->m_FrmImage-> Close();
            VSN[i]->m_pVision -> Close();

            delete VSN[i]->m_FrmImage; VSN[i]->m_FrmImage = NULL;
            delete VSN[i]->m_pVision ; VSN[i]->m_pVision  = NULL;


            delete VSN[i] ; VSN[i] = NULL ;
        }

        delete [] VSN ; VSN = NULL ;

        delete FrmJob ;
        FrmJob = NULL ;

        CCam::CloseCam();
        CLight::CloseLight();

        return true ;
    }

/*
    SVA_API void __stdcall SetVisnName(int _iVisnId , char * _sVisnName)
    {
        if(_iVisnId < 0 || g_iVisionCnt <= _iVisnId) {
            return ;
        }
        VSN[_iVisnId]-> m_pVision ->SetName(_sVisnName) ;
    }
*/
    SVA_API void __stdcall SetVisnForm(int _iVisnId , void * _hParentWnd)
    {
        if(_iVisnId < 0 || g_iVisionCnt <= _iVisnId) return ;

        if(_hParentWnd != NULL) {
            //�� ����.
            RECT   rt ;
            GetClientRect(_hParentWnd , &rt);

            VSN[_iVisnId] -> m_FrmImage -> ParentWindow = _hParentWnd ;
            VSN[_iVisnId] -> m_FrmImage -> BorderStyle = bsNone ;
            VSN[_iVisnId] -> m_FrmImage -> Left = 0 ;
            VSN[_iVisnId] -> m_FrmImage -> Top  = 0 ;
            VSN[_iVisnId] -> m_FrmImage -> Width = rt.right  ;
            VSN[_iVisnId] -> m_FrmImage -> Height= rt.bottom ;
        }
        else {
            VSN[_iVisnId] -> m_FrmImage -> BorderStyle = bsSizeable ;

        }

        VSN[_iVisnId] -> m_FrmImage -> Show();
    }




    SVA_API void __stdcall LoadJobFile(char * _sDevName)
    {
         LoadJobFileAllVision(_sDevName);
    }

    SVA_API void __stdcall Ready(int _iVisnId)
    {
        if(_iVisnId < 0 || g_iVisionCnt <= _iVisnId) return ;

        VSN[_iVisnId]->m_pVision->Ready();
    }

    SVA_API void __stdcall Grab(int _iVisnId)
    {
        if(_iVisnId < 0 || g_iVisionCnt <= _iVisnId) return ;
        VSN[_iVisnId]->m_FrmImage->SetLive(false);
        VSN[_iVisnId]->m_pVision->Grab();
    }

    SVA_API void __stdcall Live(int _iVisnId  , bool _bOn)
    {
        if(_iVisnId < 0 || g_iVisionCnt <= _iVisnId) return ;

        VSN[_iVisnId]->m_FrmImage->SetLive(_bOn);
    }

    SVA_API void __stdcall LiveOffAll()
    {
        for(int i = 0 ; i < g_iVisionCnt ; i++) {
            VSN[i]->m_FrmImage->SetLive(false);
        }
    }

    SVA_API bool __stdcall GetGrabEnd(int _iVisnId)
    {
        if(_iVisnId < 0 || g_iVisionCnt <= _iVisnId) return false;

        return VSN[_iVisnId]->m_pVision->GetGrabEnd() ;
    }

    SVA_API void __stdcall Inspect(int _iVisnId)
    {
        if(_iVisnId < 0 || g_iVisionCnt <= _iVisnId) return ;

        VSN[_iVisnId]->m_pVision->Insp();
    }

    SVA_API bool __stdcall GetInspEnd(int _iVisnId)
    {
        if(_iVisnId < 0 || g_iVisionCnt <= _iVisnId) return false;

        return VSN[_iVisnId]->m_pVision->GetInspEnd();
    }

    SVA_API void __stdcall GrabInsp(int _iVisnId)
    {
        if(_iVisnId < 0 || g_iVisionCnt <= _iVisnId) return ;

        VSN[_iVisnId]->m_pVision->GrabInsp();

    }

    SVA_API bool __stdcall GetRslt(int _iVisnId  , char  * _sRslt)
    {
        if(_iVisnId < 0 || g_iVisionCnt <= _iVisnId) {
            VSN[_iVisnId]->m_bRsltRet = false ;
            VSN[_iVisnId]->m_sRsltStr = "NG"  ;
            return false ;
        }

        if(!VSN[_iVisnId]->m_pVision->GetRslt()){
            VSN[_iVisnId]->m_bRsltRet = false ;
            VSN[_iVisnId]->m_sRsltStr = "NG-" + VSN[_iVisnId]->m_pVision->GetRsltMsg() ;
        }
        else {
            VSN[_iVisnId]->m_bRsltRet = true ;
            if(VSN[_iVisnId]->m_pVision->GetRsltMsg() != "") {
                VSN[_iVisnId]->m_sRsltStr = "OK-" + VSN[_iVisnId]->m_pVision->GetRsltMsg() ;
            }
            else {
                VSN[_iVisnId]->m_sRsltStr = "OK";
            }
        }

        sprintf(_sRslt, "%s", VSN[_iVisnId]->m_sRsltStr.c_str());

        //_sRslt = VSN[_iVisnId]->m_sRsltStr.c_str();


        return VSN[_iVisnId]->m_bRsltRet ;
    }

    /*
    */

    SVA_API void __stdcall  SetLevel  (int  _iLevel) //0:oper 1: engineer 2:Master
    {
        for(int i = 0 ; i < g_iVisionCnt ; i++) {
            VSN[i]->m_FrmImage->SetLevel(_iLevel);
        }

    }

    SVA_API void __stdcall  SetAutorun(bool _bRun  )
    {
        for(int i = 0 ; i < g_iVisionCnt ; i++) {
            VSN[i]->m_FrmImage->SetAutorun(_bRun);
        }
    }

    SVA_API int __stdcall  GetLastInsp()
    {
        return CVision::m_iLastInspId ;
    }

    SVA_API void __stdcall SetPkgID(int _iVisnId , char * _sPkgID)
    {
        String sOri = _sPkgID ;
        TReplaceFlags Flags ;

        String sChange = StringReplace(sOri,"/","_",Flags<<rfReplaceAll);
        VSN[_iVisnId]->m_pVision-> m_sPkgID = sChange ;
    }



}
