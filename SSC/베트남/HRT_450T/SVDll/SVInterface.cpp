//---------------------------------------------------------------------------


#pragma hdrstop

#include "SVInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
PIMG_New IMG_New ;
PIMG_Del IMG_Del ;

PARA_New ARA_New ;
PARA_Del ARA_Del ;                    

PGDI_New GDI_New ;
PGDI_Del GDI_Del ;


PSVL_GetErrMsg SVL_GetErrMsg ;

PEDG_Inspect   EDG_Inspect   ;
PEDG_PaintRslt EDG_PaintRslt ;

PBLB_Inspect   BLB_Inspect   ;
PBLB_PaintRslt BLB_PaintRslt ;

PCAL_Inspect   CAL_Inspect   ;
PCAL_PaintRslt CAL_PaintRslt ;

PCCL_Inspect   CCL_Inspect   ;
PCCL_PaintRslt CCL_PaintRslt ;

POCV_Train     OCV_Train     ;
POCV_Inspect   OCV_Inspect   ;
POCV_PaintRslt OCV_PaintRslt ;

PLIN_Inspect   LIN_Inspect   ;
PLIN_PaintRslt LIN_PaintRslt ;

PLCP_Inspect   LCP_Inspect   ;
PLCP_PaintRslt LCP_PaintRslt ;

PRCT_Inspect   RCT_Inspect   ;
PRCT_PaintRslt RCT_PaintRslt ;

PTSD_Inspect   TSD_Inspect   ;
PTSD_PaintRslt TSD_PaintRslt ;

PWRT_Inspect   WRT_Inspect   ;
PWRT_PaintRslt WRT_PaintRslt ;

HINSTANCE g_hInst ;
LoadSVL(char * _pFileName)
{
// DLL 로드
    g_hInst = LoadLibrary (_pFileName);

    // 클래스 적재 함수 받음
    IMG_New  = (PIMG_New )GetProcAddress(g_hInst, TEXT("IIMG_New" ));
    IMG_Del  = (PIMG_Del )GetProcAddress(g_hInst, TEXT("IIMG_Del" ));

    ARA_New  = (PARA_New )GetProcAddress(g_hInst, TEXT("IARA_New" ));
    ARA_Del  = (PARA_Del )GetProcAddress(g_hInst, TEXT("IARA_Del" ));
    
    GDI_New  = (PGDI_New )GetProcAddress(g_hInst, TEXT("IGDI_New" ));
    GDI_Del  = (PGDI_Del )GetProcAddress(g_hInst, TEXT("IGDI_Del" ));



    SVL_GetErrMsg = (PSVL_GetErrMsg)GetProcAddress(g_hInst, TEXT("ISVL_GetErrMsg" ));

    EDG_Inspect   = (PEDG_Inspect  )GetProcAddress(g_hInst, TEXT("IEDG_Inspect"   ));
    EDG_PaintRslt = (PEDG_PaintRslt)GetProcAddress(g_hInst, TEXT("IEDG_PaintRslt" ));

    BLB_Inspect   = (PBLB_Inspect  )GetProcAddress(g_hInst, TEXT("IBLB_Inspect"   ));
    BLB_PaintRslt = (PBLB_PaintRslt)GetProcAddress(g_hInst, TEXT("IBLB_PaintRslt" ));

    CAL_Inspect   = (PCAL_Inspect  )GetProcAddress(g_hInst, TEXT("ICAL_Inspect"   ));
    CAL_PaintRslt = (PCAL_PaintRslt)GetProcAddress(g_hInst, TEXT("ICAL_PaintRslt" ));

    CCL_Inspect   = (PCCL_Inspect  )GetProcAddress(g_hInst, TEXT("ICCL_Inspect"   ));
    CCL_PaintRslt = (PCCL_PaintRslt)GetProcAddress(g_hInst, TEXT("ICCL_PaintRslt" ));

    OCV_Train     = (POCV_Train    )GetProcAddress(g_hInst, TEXT("IOCV_Train"     ));
    OCV_Inspect   = (POCV_Inspect  )GetProcAddress(g_hInst, TEXT("IOCV_Inspect"   ));
    OCV_PaintRslt = (POCV_PaintRslt)GetProcAddress(g_hInst, TEXT("IOCV_PaintRslt" ));

    LIN_Inspect   = (PLIN_Inspect  )GetProcAddress(g_hInst, TEXT("ILIN_Inspect"   ));
    LIN_PaintRslt = (PLIN_PaintRslt)GetProcAddress(g_hInst, TEXT("ILIN_PaintRslt" ));

    LCP_Inspect   = (PLCP_Inspect  )GetProcAddress(g_hInst, TEXT("ILCP_Inspect"   ));
    LCP_PaintRslt = (PLCP_PaintRslt)GetProcAddress(g_hInst, TEXT("ILCP_PaintRslt" ));

    RCT_Inspect   = (PRCT_Inspect  )GetProcAddress(g_hInst, TEXT("IRCT_Inspect"   ));
    RCT_PaintRslt = (PRCT_PaintRslt)GetProcAddress(g_hInst, TEXT("IRCT_PaintRslt" ));

    TSD_Inspect   = (PTSD_Inspect  )GetProcAddress(g_hInst, TEXT("ITSD_Inspect"   ));
    TSD_PaintRslt = (PTSD_PaintRslt)GetProcAddress(g_hInst, TEXT("ITSD_PaintRslt" ));

    WRT_Inspect   = (PWRT_Inspect  )GetProcAddress(g_hInst, TEXT("IWRT_Inspect"   ));
    WRT_PaintRslt = (PWRT_PaintRslt)GetProcAddress(g_hInst, TEXT("IWRT_PaintRslt" ));





}

FreeSVL()
{

    FreeLibrary(g_hInst);


}

