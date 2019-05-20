#ifndef SVLH
#define SVLH


#include "SVImage.h"
#include "SVArea.h"
#include "SVGdi.h"
#include "SVInspStruct.h"


#define SVL_API		__declspec(dllexport)
extern "C"//Class New And Del.
{
    SVL_API  IImage* __stdcall  IIMG_New();
    SVL_API  void    __stdcall  IIMG_Del(IImage* p);
}

extern "C" //Class New And Del.
{
    SVL_API  IArea * __stdcall  IARA_New();
    SVL_API  void    __stdcall  IARA_Del(IArea* p);
}

extern "C" //Class New And Del.
{
    SVL_API  IGdi  * __stdcall  IGDI_New (HDC  _hDc);
    SVL_API  void    __stdcall  IGDI_Del (IGdi * _p);
}



extern "C" { //Alghorithm

    SVL_API char *          __stdcall ISVL_GetErrMsg  ();

    //////////////
    //Alghorism //
    ////////////////////////////////////////////////////////////////////////////
/*  1D Edge */
    SVL_API bool            __stdcall IEDG_Inspect   (IImage * _pImg , TPoint  _tSttPnt , TPoint  _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt);
    SVL_API void            __stdcall IEDG_PaintRslt (IImage * _pImg , HDC _hHdc , EDG_Rslt *_pRslt , EDG_Disp _tDisp);

/*  Line    */
    SVL_API bool            __stdcall ILIN_Inspect   (IImage * _pImg , TRect _tRect , LIN_Para _tPara   , LIN_Rslt * _pRslt);
    SVL_API void            __stdcall ILIN_PaintRslt (IImage * _pImg , HDC _hHdc    , LIN_Rslt * _pRslt , LIN_Disp   _tDisp);

/*  LineCrossPnt*/
    SVL_API bool            __stdcall ILCP_Inspect   (IImage * _pImg , TRect _tRect1 , TRect _tRect2 , LCP_Para _tPara   , LCP_Rslt * _pRslt);
    SVL_API void            __stdcall ILCP_PaintRslt (IImage * _pImg , HDC _hHdc     , LCP_Rslt * _pRslt , LCP_Disp   _tDisp);

/*  Rect*/
    SVL_API bool            __stdcall IRCT_Inspect   (IImage * _pImg , TRect _tRectL , TRect _tRectT , TRect _tRectR , TRect _tRectB , RCT_Para _tPara , RCT_Rslt * _pRslt);
    SVL_API void            __stdcall IRCT_PaintRslt (IImage * _pImg , HDC _hHdc     , RCT_Rslt * _pRslt , RCT_Disp _tDisp);

/*  Threshold*/
    SVL_API bool            __stdcall ITSD_Inspect   (IImage * _pImg , TRect _tRect  , TSD_Para _tPara , TSD_Rslt * _pRslt);
    SVL_API void            __stdcall ITSD_PaintRslt (IImage * _pImg , HDC _hHdc     , TSD_Rslt * _pRslt , TSD_Disp _tDisp);

/*  Blob    */
    SVL_API bool            __stdcall IBLB_Inspect   (IImage * _pImg , TRect _tRect , BLB_Para _tPara , BLB_Fltr _tFltr , BLB_Rslt * _pRslt);
    SVL_API void            __stdcall IBLB_PaintRslt (IImage * _pImg , HDC _hHdc   , BLB_Rslt *_pRslt , BLB_Disp _tDisp );

/*  Calibration */
    SVL_API bool            __stdcall ICAL_Inspect   (IImage * _pImg , TRect _tRect , CAL_Para _tPara , CAL_Rslt * _pRslt);
    SVL_API void            __stdcall ICAL_PaintRslt (IImage * _pImg , HDC _hHdc    , CAL_Rslt *_pRslt , CAL_Disp _tDisp );

/*  Circle  */
    SVL_API bool            __stdcall ICCL_Inspect   (IImage * _pImg , TCircle _tCircle , CCL_Para _tPara , CCL_Rslt * _pRslt);
    SVL_API void            __stdcall ICCL_PaintRslt (IImage * _pImg , HDC _hHdc   , CCL_Rslt *_pRslt , CCL_Disp _tDisp );

/*  OCV     */
    SVL_API bool            __stdcall IOCV_Train     (IImage * _pImg , TRect _Rect , OCV_TrPr _tTrPr ,  IArea * _pTrainArea , IImage * _pTrainImg) ;
    SVL_API bool            __stdcall IOCV_Inspect   (IImage * _pImg , IArea * _pTrainArea , IImage * _pTrainImg, TRect _tInspRect , OCV_Para _tPara , IArea * _pRsltArea , OCV_Rslt * _pRslt);
    SVL_API void            __stdcall IOCV_PaintRslt (IImage * _pImg , HDC      _hHdc      ,IArea * _pRsltArea , OCV_Rslt *_pRslt , OCV_Disp   _tDisp) ;



};












#endif


