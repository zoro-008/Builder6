//---------------------------------------------------------------------------

#include <vcl.h>
#include "FormLoading.h"
#include "FormSerial.h"
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
USEFORM("FormSPC.cpp", FrmSPC);
USEFORM("FrameCylOneBt.cpp", FraCylOneBt); /* TFrame: File Type */
USEFORM("FrameMotr.cpp", FraMotr); /* TFrame: File Type */
USEFORM("FrameOutput.cpp", FraOutput); /* TFrame: File Type */
USEFORM("FormInform.cpp", FrmInform);
USEFORM("FormSerial.cpp", FrmSerial);
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

        FrmSerial = new TFrmSerial(Application);
        if(!FrmSerial -> CheckSourceSerial()){
            ModalResult = FrmSerial -> ShowModal() ;
        }
        delete FrmSerial ;

         FrmLoading = new TFrmLoading(Application , true);
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
                 Application->CreateForm(__classid(TFrmSPC), &FrmSPC);
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
