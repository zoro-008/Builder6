//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Motion\FormLoading.cpp", FrmLoading);
USEFORM("Motion\FormMain.cpp", FrmMain);
USEFORM("Motion\FormOperation.cpp", FrmOperation);
USEFORM("Motion\FormDevice.cpp", FrmDevice);
USEFORM("Motion\FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("Motion\FormOption.cpp", FrmOption);
USEFORM("Motion\FormSPC.cpp", FrmSPC);
USEFORM("Motion\FormMaster.cpp", FrmMaster);
USEFORM("Motion\FrameMotr.cpp", FraMotr); /* TFrame: File Type */
USEFORM("Motion\FormLotOpen.cpp", FrmLotOpen);
USEFORM("Motion\FrameCyl.cpp", FraCyl); /* TFrame: File Type */
USEFORM("Motion\FormInform.cpp", FrmInform);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFrmMain), &FrmMain);
                 Application->CreateForm(__classid(TFrmOperation), &FrmOperation);
                 Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 Application->CreateForm(__classid(TFrmDeviceSet), &FrmDeviceSet);
                 Application->CreateForm(__classid(TFrmSPC), &FrmSPC);
                 Application->CreateForm(__classid(TFrmOption), &FrmOption);
                 Application->CreateForm(__classid(TFrmInform), &FrmInform);
                 Application->CreateForm(__classid(TFrmLotOpen), &FrmLotOpen);
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
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

