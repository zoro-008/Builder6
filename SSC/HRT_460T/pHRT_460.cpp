//---------------------------------------------------------------------------

#include <vcl.h>
#include "FormLoading.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Motion\FormOperation.cpp", FrmOperation);
USEFORM("Motion\FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("Motion\FormDevice.cpp", FrmDevice);
USEFORM("Motion\FormOption.cpp", FrmOption);
USEFORM("Motion\FormMaster.cpp", FrmMaster);
USEFORM("Motion\FormLotOpen.cpp", FrmLotOpen);
USEFORM("Motion\FormProbe.cpp", FrmProbe);
USEFORM("Motion\FormMain1.cpp", FrmMain);
USEFORM("Motion\FormSPC.cpp", FrmSPC);
USEFORM("Motion\FormRslt.cpp", FrmRslt);
USEFORM("Motion\FormConversion.cpp", FrmConversion);
USEFORM("Motion\FormInform.cpp", FrmInform);
USEFORM("Motion\FrameOutput.cpp", FraOutput); /* TFrame: File Type */
USEFORM("Motion\FrameMotr.cpp", FraMotr); /* TFrame: File Type */
USEFORM("Motion\FormLoading.cpp", FrmLoading);
USEFORM("Motion\FrameCylOneBt.cpp", FraCylOneBt); /* TFrame: File Type */

#define EXE_FOLDER ExtractFilePath(Application -> ExeName)
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HANDLE Mutex;
    int ModalResult;
    try
    {
        //HWND Handle;
        ////WinExec()
        //AnsiString sPath = EXE_FOLDER + "NeptuneReset\\NeptuneReset.exe";
        //ShellExecute(Handle, NULL, sPath.c_str(), "pHRT_460.exe", NULL, SW_SHOW);
       	//프로그램 중복실행 방지
        const char ProgMutex[] = "EpqBase";
        Mutex = OpenMutex(MUTEX_ALL_ACCESS, false, ProgMutex);
        if(! Mutex)
            CreateMutex(NULL, true, ProgMutex);
        else
            return 0;

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
                 Application->CreateForm(__classid(TFrmProbe), &FrmProbe);
                 Application->CreateForm(__classid(TFrmSPC), &FrmSPC);
                 Application->CreateForm(__classid(TFrmInform), &FrmInform);
                 Application->CreateForm(__classid(TFrmConversion), &FrmConversion);
                 Application->CreateForm(__classid(TFrmLoading), &FrmLoading);
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


