//---------------------------------------------------------------------------
#include "FormSerial.h"
#include "FormLoading.h"
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("FormLoading.cpp", FrmLoading);
USEFORM("FormMain.cpp", FrmMain);
USEFORM("FormOperation.cpp", FrmOperation);
USEFORM("FormDevice.cpp", FrmDevice);
USEFORM("FormMaster.cpp", FrmMaster);
USEFORM("FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("FormOption.cpp", FrmOption);
USEFORM("FormSPC.cpp", FrmSPC);
USEFORM("FrameMotr.cpp", FraMotr); /* TFrame: File Type */
USEFORM("FrameOutput.cpp", FraOutput); /* TFrame: File Type */
USEFORM("FrameCylOneBt.cpp", FraCylOneBt); /* TFrame: File Type */
USEFORM("FormManual.cpp", FrmManual);
USEFORM("FormLotOpen.cpp", FrmLotOpen);
USEFORM("FormSerial.cpp", FrmSerial);
USEFORM("FormArrayInfo.cpp", FrmArrayInfo);
USEFORM("FormSubErr.cpp", FrmSubErr);
USEFORM("FormDispCh.cpp", FrmDispCh);
USEFORM("FormCadPos.cpp", FrmCadPos);
USEFORM("FormCadPosRtt.cpp", FrmCadPosRtt);
USEFORM("FormHexaPot.cpp", FrmHexaPot);
USEFORM("FormEpoxyChange.cpp", FrmEpoxyChange);
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
                 Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 Application->CreateForm(__classid(TFrmDeviceSet), &FrmDeviceSet);
                 Application->CreateForm(__classid(TFrmManual), &FrmManual);
                 Application->CreateForm(__classid(TFrmOption), &FrmOption);
                 Application->CreateForm(__classid(TFrmOperation), &FrmOperation);
                 Application->CreateForm(__classid(TFrmLotOpen), &FrmLotOpen);
                 Application->CreateForm(__classid(TFrmLoading), &FrmLoading);
                 Application->CreateForm(__classid(TFrmArrayInfo), &FrmArrayInfo);
                 Application->CreateForm(__classid(TFrmSubErr), &FrmSubErr);
                 Application->CreateForm(__classid(TFrmDispCh), &FrmDispCh);
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 Application->CreateForm(__classid(TFrmSPC), &FrmSPC);
                 Application->CreateForm(__classid(TFrmCadPosRtt), &FrmCadPosRtt);
                 Application->CreateForm(__classid(TFrmHexaPot), &FrmHexaPot);
                 Application->CreateForm(__classid(TFrmCadPos), &FrmCadPos);
                 Application->CreateForm(__classid(TFrmEpoxyChange), &FrmEpoxyChange);
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
