//---------------------------------------------------------------------------

#include <vcl.h>
#include "FormLoading.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("FormMain.cpp", FrmMain);
USEFORM("FormLoading.cpp", FrmLoading);
USEFORM("FormOperation.cpp", FrmOperation);
USEFORM("FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("FormDevice.cpp", FrmDevice);
USEFORM("FormOption.cpp", FrmOption);
USEFORM("FormMaster.cpp", FrmMaster);
USEFORM("FormLotOpen.cpp", FrmLotOpen);
USEFORM("FormProbe.cpp", FrmProbe);
USEFORM("FormSPC.cpp", FrmSPC);
USEFORM("FormRslt.cpp", FrmRslt);
USEFORM("FormConversion.cpp", FrmConversion);
USEFORM("FormInform.cpp", FrmInform);
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
