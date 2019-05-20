//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormLoading.h"
//---------------------------------------------------------------------------
USEFORM("Motion\FormLoading.cpp", FrmLoading);
USEFORM("Motion\FormMain.cpp", FrmMain);
USEFORM("Motion\FormOperation.cpp", FrmOperation);
USEFORM("Motion\FormDevice.cpp", FrmDevice);
USEFORM("Motion\FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("Motion\FormOption.cpp", FrmOption);
USEFORM("Motion\FormSPC.cpp", FrmSPC);
USEFORM("Motion\FormMaster.cpp", FrmMaster);
USEFORM("Motion\FormLotOpen.cpp", FrmLotOpen);
USEFORM("Motion\FormInform.cpp", FrmInform);
USEFORM("Motion\FrameCyl.cpp", FraCyl); /* TFrame: File Type */
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
                 Application->CreateForm(__classid(TFrmOperation), &FrmOperation);
                 Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 Application->CreateForm(__classid(TFrmDeviceSet), &FrmDeviceSet);
                 Application->CreateForm(__classid(TFrmInform), &FrmInform);
                 Application->CreateForm(__classid(TFrmLotOpen), &FrmLotOpen);
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 Application->CreateForm(__classid(TFrmOption), &FrmOption);
                 Application->CreateForm(__classid(TFrmSPC), &FrmSPC);
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
