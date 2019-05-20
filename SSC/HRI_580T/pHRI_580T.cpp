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
USEFORM("FormSPC.cpp", FrmSPC);
USEFORM("FormMaster.cpp", FrmMaster);
USEFORM("FormLotOpen.cpp", FrmLotOpen);
USEFORM("FormRslt.cpp", FrmRslt);
USEFORM("FormRsltBig.cpp", FrmRsltBig);
USEFORM("FormInform.cpp", FrmInform);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HANDLE Mutex;
    int ModalResult;
    try
    {
       	//���α׷� �ߺ����� ����
        const char ProgMutex[] = "HRI_580T";
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
                 Application->CreateForm(__classid(TFrmSPC), &FrmSPC);
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 Application->CreateForm(__classid(TFrmLotOpen), &FrmLotOpen);
                 Application->CreateForm(__classid(TFrmOption), &FrmOption);
                 Application->CreateForm(__classid(TFrmRsltBig), &FrmRsltBig);
                 Application->CreateForm(__classid(TFrmInform), &FrmInform);
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



