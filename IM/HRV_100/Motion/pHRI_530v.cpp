//---------------------------------------------------------------------------

#include <vcl.h>
#include "FormLoading.h"
#include "FormSerial.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("FormMain.cpp", FrmMain);
USEFORM("FormLoading.cpp", FrmLoading);
USEFORM("FormMaster.cpp", FrmMaster);
USEFORM("FrameCylOneBt.cpp", FraCylOneBt); /* TFrame: File Type */
USEFORM("FrameMotr.cpp", FraMotr); /* TFrame: File Type */
USEFORM("FrameOutput.cpp", FraOutput); /* TFrame: File Type */
USEFORM("FormInform.cpp", FrmInform);
USEFORM("FormPosSet.cpp", FrmPosSet);
USEFORM("FormTestSet.cpp", FrmTestSet);
USEFORM("FormDevice.cpp", FrmDevice);
USEFORM("FormSerial.cpp", FrmSerial);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HANDLE Mutex;
    int ModalResult;
    try
    {
       	//���α׷� �ߺ����� ����
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
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 Application->CreateForm(__classid(TFrmInform), &FrmInform);
                 Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 Application->CreateForm(__classid(TFrmPosSet), &FrmPosSet);
                 Application->CreateForm(__classid(TFrmTestSet), &FrmTestSet);
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
