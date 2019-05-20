//---------------------------------------------------------------------------

#ifndef SVInterfaceH
#define SVInterfaceH

//#include "SVImage.h"
//#include "SVL.h"


#include "SVImage.h"
#include "SVArea.h"
#include "SVGdi.h"
#include "SVInspStruct.h"
//---------------------------------------------------------------------------

typedef IImage *(__stdcall *PIMG_New )(           );
typedef void    (__stdcall *PIMG_Del )(IImage* _p );
extern PIMG_New IMG_New ;
extern PIMG_Del IMG_Del ;


typedef IArea * (__stdcall *PARA_New )(           );
typedef void    (__stdcall *PARA_Del )(IArea * _p );
extern PARA_New ARA_New ;
extern PARA_Del ARA_Del ;


typedef IGdi  * (__stdcall *PGDI_New )(HDC     _hdc);
typedef void    (__stdcall *PGDI_Del )(IGdi  * _p  );
extern PGDI_New GDI_New ;
extern PGDI_Del GDI_Del ;


typedef char * (__stdcall *PSVL_GetErrMsg )();
extern PSVL_GetErrMsg SVL_GetErrMsg ;

/*  1D Edge */
typedef bool   (__stdcall *PEDG_Inspect   )(IImage * _pImg , TPoint  _tSttPnt , TPoint  _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt);
typedef void   (__stdcall *PEDG_PaintRslt )(IImage * _pImg , HDC _hHdc , EDG_Rslt * _pRslt , EDG_Disp _tDisp);
extern PEDG_Inspect   EDG_Inspect   ;
extern PEDG_PaintRslt EDG_PaintRslt ;

/*  Blob    */
typedef bool   (__stdcall *PBLB_Inspect   )(IImage * _pImg , TRect _tRect , BLB_Para _tPara , BLB_Rslt * _pRslt);
typedef void   (__stdcall *PBLB_PaintRslt )(IImage * _pImg , HDC _hHdc   , BLB_Rslt * _pRslt , BLB_Disp _tDisp );
extern PBLB_Inspect   BLB_Inspect   ;
extern PBLB_PaintRslt BLB_PaintRslt ;

/*  Calibration    */
typedef bool   (__stdcall *PCAL_Inspect   )(IImage * _pImg , TRect _tRect , CAL_Para _tPara , CAL_Rslt * _pRslt);
typedef void   (__stdcall *PCAL_PaintRslt )(IImage * _pImg , HDC _hHdc   , CAL_Rslt * _pRslt , CAL_Disp _tDisp );
extern PCAL_Inspect   CAL_Inspect   ;
extern PCAL_PaintRslt CAL_PaintRslt ;

/*  Circle  */
typedef bool   (__stdcall *PCCL_Inspect   )(IImage * _pImg , TCircle _tCircle , CCL_Para _tPara , CCL_Rslt * _pRslt);
typedef void   (__stdcall *PCCL_PaintRslt )(IImage * _pImg , HDC _hHdc   , CCL_Rslt * _pRslt , CCL_Disp _tDisp );
extern PCCL_Inspect   CCL_Inspect   ;
extern PCCL_PaintRslt CCL_PaintRslt ;

/*  OCV     */

typedef bool   (__stdcall *POCV_Train     )(IImage * _pImg , TRect _Rect , OCV_Para _tPara ,  IArea * _pTrainArea , IImage * _pTrainImg) ;
typedef bool   (__stdcall *POCV_Inspect   )(IImage * _pImg , IArea * _pTrainArea , IImage * _pTrainImg , TRect _tInspRect , OCV_Para _tPara , IArea * _pRsltArea , OCV_Rslt * _pRslt) ;
typedef void   (__stdcall *POCV_PaintRslt )(IImage * _pImg , HDC      _hHdc      ,IArea * _pRsltArea , OCV_Rslt *_pRslt , OCV_Disp   _tDisp);
extern POCV_Train     OCV_Train     ;
extern POCV_Inspect   OCV_Inspect   ;
extern POCV_PaintRslt OCV_PaintRslt ;

/*  Line    */
typedef bool   (__stdcall *PLIN_Inspect   )(IImage * _pImg , TRect _tRect , LIN_Para _tPara   , LIN_Rslt * _pRslt) ;
typedef void   (__stdcall *PLIN_PaintRslt )(IImage * _pImg , HDC _hHdc    , LIN_Rslt * _pRslt , LIN_Disp   _tDisp) ;
extern PLIN_Inspect   LIN_Inspect   ;
extern PLIN_PaintRslt LIN_PaintRslt ;

/*LineCrossPnt */
typedef bool   (__stdcall *PLCP_Inspect    )(IImage * _pImg , TRect _tRect1 , TRect _tRect2 , LCP_Para _tPara   , LCP_Rslt * _pRslt) ;
typedef void   (__stdcall *PLCP_PaintRslt  )(IImage * _pImg , HDC _hHdc     , LCP_Rslt * _pRslt , LCP_Disp   _tDisp) ;
extern PLCP_Inspect   LCP_Inspect   ;
extern PLCP_PaintRslt LCP_PaintRslt ;

/*Rect */
typedef bool   (__stdcall *PRCT_Inspect    )(IImage * _pImg , TRect _tRectL , TRect _tRectT , TRect _tRectR , TRect _tRectB, RCT_Para _tPara   , RCT_Rslt * _pRslt) ;
typedef void   (__stdcall *PRCT_PaintRslt  )(IImage * _pImg , HDC _hHdc     , RCT_Rslt * _pRslt , RCT_Disp   _tDisp) ;
extern PRCT_Inspect   RCT_Inspect   ;
extern PRCT_PaintRslt RCT_PaintRslt ;

/*Threshold*/
typedef bool   (__stdcall *PTSD_Inspect    )(IImage * _pImg , TRect _tRect  , TSD_Para _tPara   , TSD_Rslt * _pRslt) ;
typedef void   (__stdcall *PTSD_PaintRslt  )(IImage * _pImg , HDC _hHdc     , TSD_Rslt * _pRslt , TSD_Disp   _tDisp) ;
extern PTSD_Inspect   TSD_Inspect   ;
extern PTSD_PaintRslt TSD_PaintRslt ;

/*Wafer Rect */
typedef bool   (__stdcall *PWRT_Inspect    )(IImage * _pImg , TRect _tRectL , TRect _tRectT , TRect _tRectR , TRect _tRectB, WRT_Para _tPara   , WRT_Rslt * _pRslt) ;
typedef void   (__stdcall *PWRT_PaintRslt  )(IImage * _pImg , HDC _hHdc     , WRT_Rslt * _pRslt , WRT_Disp   _tDisp) ;
extern PWRT_Inspect   WRT_Inspect   ;
extern PWRT_PaintRslt WRT_PaintRslt ;

LoadSVL(char * _pFileName);
FreeSVL();



#endif
