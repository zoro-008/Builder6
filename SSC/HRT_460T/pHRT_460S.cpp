//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Motion\FormMain.cpp", FrmMain);
USEFORM("Motion\FormConversion.cpp", FrmConversion);
USEFORM("Motion\FormDevice.cpp", FrmDevice);
USEFORM("Motion\FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("Motion\FormInform.cpp", FrmInform);
USEFORM("Motion\FormLoading.cpp", FrmLoading);
USEFORM("Motion\FormLotOpen.cpp", FrmLotOpen);
USEFORM("Motion\FormMaster.cpp", FrmMaster);
USEFORM("Motion\FormMessage.cpp", FrmMessage);
USEFORM("Motion\FormOperation.cpp", FrmOperation);
USEFORM("Motion\FormOption.cpp", FrmOption);
USEFORM("Motion\FormProbe.cpp", FrmProbe);
USEFORM("Motion\FormRslt.cpp", FrmRslt);
USEFORM("Motion\FormSerial.cpp", FrmSerial);
USEFORM("Motion\FormSPC.cpp", FrmSPC);
USEFORM("Motion\FrameCylOneBt.cpp", FraCylOneBt); /* TFrame: File Type */
USEFORM("Motion\FrameMotr.cpp", FraMotr); /* TFrame: File Type */
USEFORM("Motion\FrameOutput.cpp", FraOutput); /* TFrame: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFrmMain), &FrmMain);
                 //Application->CreateForm(__classid(TFrmConversion), &FrmConversion);
                 //Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 //Application->CreateForm(__classid(TFrmDeviceSet), &FrmDeviceSet);
                 //Application->CreateForm(__classid(TFrmInform), &FrmInform);
                 //Application->CreateForm(__classid(TFrmLoading), &FrmLoading);
                 //Application->CreateForm(__classid(TFrmLotOpen), &FrmLotOpen);
                 //Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 //Application->CreateForm(__classid(TFrmMessage), &FrmMessage);
                 //Application->CreateForm(__classid(TFrmOperation), &FrmOperation);
                 //Application->CreateForm(__classid(TFrmOption), &FrmOption);
                 //Application->CreateForm(__classid(TFrmProbe), &FrmProbe);
                 //Application->CreateForm(__classid(TFrmRslt), &FrmRslt);
                 //Application->CreateForm(__classid(TFrmSPC), &FrmSPC);
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
