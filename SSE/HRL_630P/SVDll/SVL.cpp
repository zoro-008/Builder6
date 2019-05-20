//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
    return 1;
}
//---------------------------------------------------------------------------

#include "SVL.h"
#include "Image.h"
#include "Area.h"
#include "GdiUnit.h"

extern "C" SVL_API
IImage* __stdcall IIMG_New()
{
	return new CImage();
}
extern "C" SVL_API
void	__stdcall  IIMG_Del(IImage* p)
{
        p -> Del();
	delete p;
}

extern "C" SVL_API
IArea * __stdcall IARA_New()
{
    //CArea();
	return new CArea();
}
extern "C" SVL_API
void	__stdcall  IARA_Del(IArea* p)
{
        p -> Del();
	delete p;
}

extern "C" SVL_API
IGdi * __stdcall IGDI_New(HDC _hDc)
{
	return new CGdi(_hDc);
}

extern "C" SVL_API
void	__stdcall  IGDI_Del(IGdi* _p)
{
	delete _p;
}



//////////////
//Alghorism //
////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "aEdge.h"
#include "aBlob.h"
#include "aCircle.h"
#include "aOcv.h"
#include "aLine.h"
#include "aLineCrossPnt.h"
#include "aRect.h"
#include "aThreshold.h"
#include "aCalibration.h"

extern "C" SVL_API char * __stdcall ISVL_GetErrMsg ()                                                                                               {return g_sErrMsg.c_str(); }

/*  1D Edge */
CEdge Edge ;
extern "C" SVL_API bool   __stdcall IEDG_Inspect   (IImage * _pImg , TPoint  _tSttPnt , TPoint  _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt)     {g_sErrMsg="";return Edge.Inspect    ((CImage *)_pImg , _tSttPnt , _tEndPnt , _tPara , _pRslt);}
extern "C" SVL_API void   __stdcall IEDG_PaintRslt (IImage * _pImg , HDC _hHdc        , EDG_Rslt *_pRslt , EDG_Disp _tDisp)                         {                    Edge.PaintRslt  (_hHdc           , _pRslt   , _tDisp   , _pImg->GetScaleX() , _pImg->GetScaleY());}

/*Line    */
CLine Line ;
extern "C" SVL_API bool   __stdcall ILIN_Inspect   (IImage * _pImg , TRect _tRect , LIN_Para _tPara   , LIN_Rslt * _pRslt)                          {g_sErrMsg="";return Line.Inspect    ((CImage *)_pImg , _tRect , _tPara , _pRslt );}
extern "C" SVL_API void   __stdcall ILIN_PaintRslt (IImage * _pImg , HDC _hHdc    , LIN_Rslt * _pRslt , LIN_Disp   _tDisp)                          {                    Line.PaintRslt  (_hHdc , _pRslt ,  _tDisp , _pImg->GetScaleX() , _pImg->GetScaleY());}

/*LineCrossPnt*/
CLineCrossPnt Lcp ;
extern "C" SVL_API bool   __stdcall ILCP_Inspect   (IImage * _pImg , TRect _tRect1 , TRect _tRect2 , LCP_Para _tPara   , LCP_Rslt * _pRslt)         {g_sErrMsg="";return Lcp.Inspect    ((CImage *)_pImg , _tRect1 , _tRect2 , _tPara , _pRslt );}
extern "C" SVL_API void   __stdcall ILCP_PaintRslt (IImage * _pImg , HDC _hHdc     , LCP_Rslt * _pRslt , LCP_Disp   _tDisp)                         {                    Lcp.PaintRslt  (_hHdc , _pRslt , _tDisp , _pImg->GetScaleX(), _pImg->GetScaleY());}
                                    //디스플레이용 엣지점이 맨마지막 한것 밖에 없어서 엦지는 디벅할때만 보기.
/*Rect*/
CRect Rct ;
extern "C" SVL_API bool   __stdcall IRCT_Inspect   (IImage * _pImg , TRect _tRectL ,
                                                                     TRect _tRectT ,
                                                                     TRect _tRectR ,
                                                                     TRect _tRectB , RCT_Para _tPara   , RCT_Rslt * _pRslt)                         {g_sErrMsg="";return Rct.Inspect    ((CImage *)_pImg , _tRectL , _tRectT , _tRectR , _tRectB , _tPara , _pRslt);}
extern "C" SVL_API void   __stdcall IRCT_PaintRslt (IImage * _pImg , HDC _hHdc     , RCT_Rslt * _pRslt , RCT_Disp   _tDisp)                         {                    Rct.PaintRslt  (_hHdc , _pRslt , _tDisp , _pImg->GetScaleX(), _pImg->GetScaleY());}

/*Threshold*/
CThreshold Tsd ;
extern "C" SVL_API bool   __stdcall ITSD_Inspect   (IImage * _pImg  , TRect _tRect , TSD_Para _tPara , TSD_Rslt * _pRslt )                          {g_sErrMsg="";return Tsd.Inspect    ((CImage *)_pImg , _tRect , _tPara , _pRslt);   }
extern "C" SVL_API void   __stdcall ITSD_PaintRslt (IImage * _pImg  , HDC _hHdc , TSD_Rslt * _pRslt , TSD_Disp _tDisp )                             {                    Tsd.PaintRslt  (_hHdc , _pRslt , _tDisp , _pImg->GetScaleX(), _pImg->GetScaleY());}



/*  Blob */
CBlob Blob ;
extern "C" SVL_API bool   __stdcall IBLB_Inspect   (IImage * _pImg , TRect  _tRect , BLB_Para   _tPara , BLB_Rslt * _pRslt)                         {g_sErrMsg="";return Blob.Inspect    ((CImage *)_pImg  , _tRect , _tPara , _pRslt);}
extern "C" SVL_API void   __stdcall IBLB_PaintRslt (IImage * _pImg , HDC    _hHdc  , BLB_Rslt  *_pRslt , BLB_Disp _tDisp )                          {                    Blob.PaintRslt  (_hHdc            , _pRslt , _tDisp , _pImg->GetScaleX() , _pImg->GetScaleY());}

/*  Calibration */
CCalibration Cal ;
extern "C" SVL_API bool   __stdcall ICAL_Inspect   (IImage * _pImg , TRect  _tRect , CAL_Para   _tPara , CAL_Rslt * _pRslt)                         {g_sErrMsg="";return Cal.Inspect    ((CImage *)_pImg  , _tRect , _tPara                      , _pRslt);}
extern "C" SVL_API void   __stdcall ICAL_PaintRslt (IImage * _pImg , HDC    _hHdc  , CAL_Rslt  *_pRslt , CAL_Disp _tDisp )                          {                    Cal.PaintRslt  (_hHdc            , _pRslt , _tDisp , _pImg->GetScaleX() , _pImg->GetScaleY());}



/* Circle */
CCircle Circle ;
extern "C" SVL_API bool   __stdcall ICCL_Inspect   (IImage * _pImg , TCircle  _tCircle , CCL_Para  _tPara , CCL_Rslt * _pRslt)                      {g_sErrMsg="";return Circle.Inspect  ((CImage *)_pImg  , _tCircle , _tPara , _pRslt);}
extern "C" SVL_API void   __stdcall ICCL_PaintRslt (IImage * _pImg , HDC      _hHdc    , CCL_Rslt *_pRslt , CCL_Disp   _tDisp)                      {                    Circle.PaintRslt(_hHdc            , _pRslt   , _tDisp , _pImg->GetScaleX() , _pImg->GetScaleY());}


/*OCV     */
COcv Ocv ;
extern "C" SVL_API bool   __stdcall IOCV_Train     (IImage * _pImg , TRect _Rect , OCV_Para _tPara ,  IArea * _pTrainArea , IImage * _pTrainImg)    {g_sErrMsg=""; return Ocv.Train      ((CImage *)_pImg , _Rect , _tPara , (CArea *) _pTrainArea , (CImage *) _pTrainImg);  }
extern "C" SVL_API bool   __stdcall IOCV_Inspect   (IImage * _pImg , IArea * _pTrainArea , IImage * _pTrainImg, TRect _tInspRect , OCV_Para _tPara , IArea * _pRsltArea , OCV_Rslt * _pRslt)
                                                                                                                                                    {g_sErrMsg=""; return Ocv.Inspect    ((CImage *)_pImg , (CArea *) _pTrainArea, (CImage *) _pTrainImg , _tInspRect , _tPara , (CArea *) _pRsltArea , _pRslt);}
extern "C" SVL_API void   __stdcall IOCV_PaintRslt (IImage * _pImg , HDC      _hHdc      ,IArea * _pRsltArea , OCV_Rslt *_pRslt , OCV_Disp   _tDisp){                     Ocv.PaintRslt  (_hHdc            ,(CArea *) _pRsltArea , _pRslt  , _tDisp , _pImg->GetScaleX() , _pImg->GetScaleY());}


/*WaferRect*/
CWfrRect Wrt ;
extern "C" SVL_API bool   __stdcall IWRT_Inspect   (IImage * _pImg , TRect _tRectL ,
                                                                     TRect _tRectT ,
                                                                     TRect _tRectR ,
                                                                     TRect _tRectB , WRT_Para _tPara   , WRT_Rslt * _pRslt)                         {g_sErrMsg="";return Wrt.Inspect    ((CImage *)_pImg , _tRectL , _tRectT , _tRectR , _tRectB , _tPara , _pRslt);}
extern "C" SVL_API void   __stdcall IWRT_PaintRslt (IImage * _pImg , HDC _hHdc     , WRT_Rslt * _pRslt , WRT_Disp   _tDisp)                         {                    Wrt.PaintRslt  (_hHdc , _pRslt , _tDisp , _pImg->GetScaleX(), _pImg->GetScaleY());}








