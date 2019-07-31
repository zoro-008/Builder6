//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "FormLoading.h"
//---------------------------------------------------------------------------
USEFORM("Vision\FormHisto.cpp", FrmHisto);
USEFORM("Vision\FormPixel.cpp", FrmPixel);
USEFORM("Vision\FormMag.cpp", FrmMag);
USEFORM("Vision\FormJob.cpp", FrmJob);
USEFORM("Vision\FormCamLightPara.cpp", FrmCamLightPara);
USEFORM("Vision\FormTrainChk.cpp", FrmTrainChk);
USEFORM("Vision\FormCalPara.cpp", FrmCalPara);
USEFORM("Vision\FormImage.cpp", FrmImage);
USEFORM("Vision\FormEndImg.cpp", FrmEndImg);
USEFORM("Vision\FormVisnPara.cpp", FrmVisnPara);
USEFORM("Vision\FormAlgoPara.cpp", FrmAlgoPara);
USEFORM("Vision\FormVisnMain.cpp", FrmVisnMain);
USEFORM("Motion\FormLoading.cpp", FrmLoading);
USEFORM("Motion\FormMain.cpp", FrmMain);
USEFORM("Motion\FormOperation.cpp", FrmOperation);
USEFORM("Motion\FormDevice.cpp", FrmDevice);
USEFORM("Motion\FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("Motion\FormOption.cpp", FrmOption);
USEFORM("Motion\FormSPC.cpp", FrmSPC);
USEFORM("Motion\FormMaster.cpp", FrmMaster);
USEFORM("Motion\FormLotOpen.cpp", FrmLotOpen);
USEFORM("Motion\FrameMotr.cpp", FraMotr); /* TFrame: File Type */
USEFORM("Motion\FrameCyl.cpp", FraCyl); /* TFrame: File Type */
USEFORM("Motion\FormInform.cpp", FrmInform);
USEFORM("Motion\FormMapViewer.cpp", FrmMapViewer);
USEFORM("Motion\FormInfo.cpp", FrmInfo);
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
         FrmLoading = new TFrmLoading(Application);
         FrmLoading->ShowModal();
         delete FrmLoading;
         Application->Initialize();
         Application->CreateForm(__classid(TFrmMain), &FrmMain);
                 Application->CreateForm(__classid(TFrmOperation), &FrmOperation);
                 Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 Application->CreateForm(__classid(TFrmDeviceSet), &FrmDeviceSet);
                 Application->CreateForm(__classid(TFrmOption), &FrmOption);
                 Application->CreateForm(__classid(TFrmSPC), &FrmSPC);
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 Application->CreateForm(__classid(TFrmHisto), &FrmHisto);
                 Application->CreateForm(__classid(TFrmMag), &FrmMag);
                 Application->CreateForm(__classid(TFrmJob), &FrmJob);
                 Application->CreateForm(__classid(TFrmVisnPara), &FrmVisnPara);
                 Application->CreateForm(__classid(TFrmVisnMain), &FrmVisnMain);
                 Application->CreateForm(__classid(TFrmLotOpen), &FrmLotOpen);
                 Application->CreateForm(__classid(TFrmCamLightPara), &FrmCamLightPara);
                 Application->CreateForm(__classid(TFrmCalPara), &FrmCalPara);
                 Application->CreateForm(__classid(TFrmAlgoPara), &FrmAlgoPara);
                 Application->CreateForm(__classid(TFrmTrainChk), &FrmTrainChk);
                 Application->CreateForm(__classid(TFrmInform), &FrmInform);
                 Application->CreateForm(__classid(TFrmInfo), &FrmInfo);
                 Application->CreateForm(__classid(TFrmMapViewer), &FrmMapViewer);
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


