//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"
//---------------------------------------------------------------------------
USEFORM("Motion\FormDevice.cpp", FrmDevice);
USEFORM("Motion\FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("Motion\FormInform.cpp", FrmInform);
USEFORM("Motion\FormLoading.cpp", FrmLoading);
USEFORM("Motion\FormMain.cpp", FrmMain);
USEFORM("Motion\FormOperation.cpp", FrmOperation);
USEFORM("Motion\FormMaster.cpp", FrmMaster);
USEFORM("Motion\FrameMotr.cpp", FraMotr); /* TFrame: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HANDLE hMutex;
    try
    {
        const char ProgMutex[] = "EpqBase";
        hMutex = CreateMutex(NULL, TRUE, ProgMutex);
        if ( hMutex == NULL )
            throw Exception("Error: CreateMutex failed.!");
         if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
            OutputDebugStringA("Named Mutex already exist. skip Application Init..");
            CloseHandle( hMutex );
            return 0;
         }


         FrmLoading = new TFrmLoading(Application,true);
         FrmLoading->ShowModal();
         delete FrmLoading;

         Application->Initialize();
         Application->CreateForm(__classid(TFrmMain), &FrmMain);
                 Application->CreateForm(__classid(TFrmInform), &FrmInform);
                 Application->CreateForm(__classid(TFrmLoading), &FrmLoading);
                 Application->CreateForm(__classid(TFrmOperation), &FrmOperation);
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 Application->CreateForm(__classid(TFrmDeviceSet), &FrmDeviceSet);
                 Application->Run();

                 int a ;
                 a++;
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


