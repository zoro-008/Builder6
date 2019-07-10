
//---------------------------------------------------------------------------

#include <vcl.h>
#include "FormLoading.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Motion\FormMain.cpp", FrmMain);
USEFORM("Motion\FormLoading.cpp", FrmLoading);
USEFORM("Motion\FormOperation.cpp", FrmOperation);
USEFORM("Motion\FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("Motion\FormDevice.cpp", FrmDevice);
USEFORM("Motion\FormOption.cpp", FrmOption);
USEFORM("Motion\FormMaster.cpp", FrmMaster);
USEFORM("Motion\FormLotOpen.cpp", FrmLotOpen);
USEFORM("Motion\FormProbe.cpp", FrmProbe);
USEFORM("Motion\FormSPC.cpp", FrmSPC);
USEFORM("Motion\FormRslt.cpp", FrmRslt);
USEFORM("Motion\FormConversion.cpp", FrmConversion);
USEFORM("Motion\FormInform.cpp", FrmInform);
USEFORM("Motion\FormVisnErr.cpp", FrmVisnErr);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HANDLE Mutex;
    int ModalResult;
    try
    {
       	//프로그램 중복실행 방지
        const char ProgMutex[] = "EpqBase";
        Mutex = OpenMutex(MUTEX_ALL_ACCESS, false, ProgMutex);
        if(! Mutex)
            CreateMutex(NULL, true, ProgMutex);
        else
            return 0;

         FrmLoading = new TFrmLoading(Application);
         ModalResult = FrmLoading->ShowModal();
         delete FrmLoading;

         Application->Initialize();
         Application->CreateForm(__classid(TFrmMain), &FrmMain);
                 Application->CreateForm(__classid(TFrmOperation), &FrmOperation);
                 Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 Application->CreateForm(__classid(TFrmDeviceSet), &FrmDeviceSet);
                 Application->CreateForm(__classid(TFrmOption), &FrmOption);
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 Application->CreateForm(__classid(TFrmLotOpen), &FrmLotOpen);
                 Application->CreateForm(__classid(TFrmProbe), &FrmProbe);
                 Application->CreateForm(__classid(TFrmSPC), &FrmSPC);
                 Application->CreateForm(__classid(TFrmInform), &FrmInform);
                 Application->CreateForm(__classid(TFrmConversion), &FrmConversion);
                 Application->CreateForm(__classid(TFrmVisnErr), &FrmVisnErr);
                 Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
