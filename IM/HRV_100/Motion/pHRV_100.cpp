//---------------------------------------------------------------------------

#include <vcl.h>
#include "FormLoading.h"
#include "FormSerial.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("FormLoading.cpp", FrmLoading);
USEFORM("FormMain.cpp", FrmMain);
USEFORM("FormMulti.cpp", FrmMulti);
USEFORM("FormRslt.cpp", FrmRslt);
USEFORM("FormImage.cpp", FrmImage);
USEFORM("FormMaster.cpp", FrmMaster);
USEFORM("FrameCylOneBt.cpp", FraCylOneBt); /* TFrame: File Type */
USEFORM("FrameMotr.cpp", FraMotr); /* TFrame: File Type */
USEFORM("FrameOutput.cpp", FraOutput); /* TFrame: File Type */
USEFORM("FormInform.cpp", FrmInform);
USEFORM("FormPosSet.cpp", FrmPosSet);
USEFORM("FormTestCon.cpp", FrmTestCon);
USEFORM("FormOption.cpp", FrmOption);
USEFORM("FormDevice.cpp", FrmDevice);
USEFORM("..\Vision\FormCamPara.cpp", FrmCamPara);
USEFORM("FormSerial.cpp", FrmSerial);
USEFORM("FormGraph.cpp", FrmGraph);
USEFORM("FormRepeat.cpp", FrmRepeat);
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
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 Application->CreateForm(__classid(TFrmInform), &FrmInform);
                 Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 Application->CreateForm(__classid(TFrmPosSet), &FrmPosSet);
                 Application->CreateForm(__classid(TFrmMulti), &FrmMulti);
                 Application->CreateForm(__classid(TFrmTestCon), &FrmTestCon);
                 Application->CreateForm(__classid(TFrmOption), &FrmOption);
                 Application->CreateForm(__classid(TFrmCamPara), &FrmCamPara);
                 Application->CreateForm(__classid(TFrmRepeat), &FrmRepeat);
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
