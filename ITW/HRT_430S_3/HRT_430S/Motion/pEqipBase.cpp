//---------------------------------------------------------------------------

#include <vcl.h>
#include "FormLoading.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("FormLoading.cpp", FrmLoading);
USEFORM("FormMain.cpp", FrmMain);
USEFORM("FormOperation.cpp", FrmOperation);
USEFORM("FormDevice.cpp", FrmDevice);
USEFORM("FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("FormOption.cpp", FrmOption);
USEFORM("MasterOptionTest.cpp", FrmMasterOptionTest);
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
         Application->CreateForm(__classid(TFrmMasterOptionTest), &FrmMasterOptionTest);
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
