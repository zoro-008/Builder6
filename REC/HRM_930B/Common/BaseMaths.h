//---------------------------------------------------------------------------

#ifndef SVMathsH
#define SVMathsH
#include <math.h>
#include "BaseDefine.h"
#include "limits.h"   //정수형 민맥스값 디파인.
#include "float.h"    //실수형 민맥스값.


#define MATH_PI 3.14159265358979323846
#define ROOT_2  1.4142135623730950488

//double
//#define MAX_FLOAT_VALUE   DBL_MAX
//#define MIN_FLOAT_VALUE  -DBL_MAX

//float
//#define MAX_FLOAT_VALUE   FLT_MAX
//#define MAX_FLOAT_VALUE  -FLT_MAX

#define MAX_INT_VALUE  INT_MAX
#define MIN_INT_VALUE  INT_MIN
class CMath
{
    public:
        //Line 관련.
        //======================================================================
        static double __stdcall GetLineA(double y, double  x, double b);
        static double __stdcall GetLineB(double y, double  a, double x);
        static double __stdcall GetLineX(double y, double  a, double b);
        static double __stdcall GetLineY(double a, double  x, double b);
        //x1 : Start Point x Of Linear, y1 : Start Point y Of Linear,
        //x2 : End Point x Of Linear, y2 : End Point x Of Linear,
        static double __stdcall GetLineA(double x1, double y1, double x2, double y2);
        static double __stdcall GetLineB(double x1, double y1, double x2, double y2);
        static double __stdcall GetLineX(double x1, double y1, double x2, double y2 , double y);
        static double __stdcall GetLineY(double x1, double y1, double x2, double y2 , double x);
        static double __stdcall GetLineA(TDPoint p1, TDPoint p2          );
        static double __stdcall GetLineB(TDPoint p1, TDPoint p2          );
        static double __stdcall GetLineX(TDPoint p1, TDPoint p2 , double y);
        static double __stdcall GetLineY(TDPoint p1, TDPoint p2 , double x);
        static double __stdcall GetLineA(TPoint p1, TPoint p2          );
        static double __stdcall GetLineB(TPoint p1, TPoint p2          );
        static double __stdcall GetLineX(TPoint p1, TPoint p2 , double y);
        static double __stdcall GetLineY(TPoint p1, TPoint p2 , double x);
        static double __stdcall GetLineAngleFromA(double _dA);
        static bool   __stdcall GetLineCrossPnt(TDPoint Phs, TDPoint Phe, TDPoint Pvs, TDPoint Pve , TDPoint &Ret); //한번 확인하기.
        static bool   __stdcall GetLineCrossPnt(TPoint Phs, TPoint Phe, TPoint Pvs, TPoint Pve , TPoint &Ret);

        static bool   __stdcall GetLineCrossPnt(double a1 , double b1 , double a2 , double b2 , TDPoint &Ret);

        static double __stdcall GetLinesIncAngle(TDPoint Phs, TDPoint Phe, TDPoint Pvs, TDPoint Pve);
        static double __stdcall GetLinesIncAngle(TDPoint P1, TDPoint P2, TDPoint P3);
        static double __stdcall GetLineLenth(double x1, double y1, double x2, double y2);
        static double __stdcall GetLineAngle(double x1, double y1, double x2, double y2);

        static double __stdcall GetLengthPntToLine(double _tPntX , double _tPntY , double _dLine1X, double _dLine1Y , double _dLint2X , double _dLine2Y);
        //이밑에 두개는 기울기 무한대 예외 처리 안되어 있음.
        static double __stdcall GetLengthPntToLine(TDPoint _tPnt , double _dA , double _dB);
        static double __stdcall GetLengthPntToLine(double _tPntX , double _tPntY , double _dA , double _dB);

        static bool   __stdcall GetLineABFromPntByLeastSqure(int _iPntCnt , TDPoint * _pPnt , double & _dA , double & _dB);
        static bool   __stdcall GetLineABFromPntByLeastSqure(int _iPntCnt , double * _pPntX , double * _pPntY , double & _dA , double & _dB);

        //시작점에서 부터 해당각도와 길이만큼 떨어진 점.
        static void __stdcall GetPntFromPntByAngLen(double  _dX , double  _dY , //입력 시작점.
                                                         double  _dAngle  ,         //입력 각도.
                                                         double  _dLength ,         //입력 길이.
                                                         double *_pX , double *_pY ); //출력 포인트.
        static void __stdcall GetPntFromPntByAngLen(double  _dX , double  _dY , //입력 시작점.
                                                         double  _dAngle  ,         //입력 각도.
                                                         double  _dLength ,         //입력 길이.
                                                         int    *_pX , int *_pY ); //출력 포인트.
        static void __stdcall GetRotPntFromCntrPnt(double _dX     , double _dY     , double _dAng ,  //입력 x , 입력 y , 돌릴각도. , +가 반시계방향.
                                                        double _dCntrX , double _dCntrY ,                //돌리는 기준점 x , y
                                                        double *_pRetX , double *_pRetY );                //결과값 x,y


        //Circle 관련.
        //======================================================================
        //3점의 위치를 입력하여 중점좌표 및 반지름을 구한다.
        //retern : 0 = successed ,
        //        -1 = failed 세점이 일직선상에 있음.
        static bool __stdcall GetCircleBy3Points(double  _dX1 , double  _dX2 , double  _dX3 ,
                                                      double  _dY1 , double  _dY2 , double  _dY3 ,
                                                      double *_pCx , double *_pCy , double *_pRad);

        static int __stdcall GetCircleBy3Points(TDPoint _Pnt1 , TDPoint _Pnt2 , TDPoint _Pnt3 ,
                                                     double  *_pdCx , double  *_pdCy , double  *_pdRad);
        //중심좌표가 _dCntrX , _dCntrY 이고 반지름이 _dRadius 인 원의 공식에서 임의의 점(_pdX, _pdY)이 원 안에 들어가는지 확인.
        //return : true  = 원 안에 들어가 있음.
        //         false = 원 밖에 있음.
        static bool __stdcall GetCircleInPoint(double _dCntrX , double _dCntrY , double _dRadius , double _pdX , double _pdY);

        //3포인트를 순서대로 이동했을때 CCW방향이면 1, 아니면 -1
        static int GetPntCCW(TDPoint _tPnt1, TDPoint _tPnt2, TDPoint _tPnt3);

        //2개의 선분이 교차되면 true
        static bool GetSegmentIntersect(TDPoint _tPntStt1, TDPoint _tPntEnd1, TDPoint _tPntStt2, TDPoint _tPntEnd2);

        //포인트들의 최외곽 직사각형.
        static bool  GetOutRectFromPnts(TDPoint *_pPoints, int _iPntCnt, TDRect * _tRect);

        //포인트가 구성하는 가장 작은 사각형 안에 해당 포인트가 있는지 확인.
        static bool  IsPntInPolygonOutRect(TDPoint *_pPoints, int _iPntCnt, TDPoint _tPnt, TDRect &_tOutRect);

        static bool IsPntInPolygon(TDPoint *_pPoints, int _iPntCnt, TDPoint _tPnt);

        //점이 네모 안에 있는지.
        static bool __stdcall IsPntInRect(double _dX1 , double _dY1 , double _dX2 , double _dY2 ,
                                               double _dX  , double _dY );
        //한개의 직선(pX1, pX2, pY1, pY2)를 가지고 원과의 접점갯수와 거리.
        //return 0 = 접점이 없는 경우
        //       1 = 접점이 1점인 경우
        //       2 = 접점이 2점인 경우
        static int  __stdcall GetCircleLineDistance(double  _dX1      , double _dY1      , double _dX2    , double _dY2 , //직선 점2개.
                                                         double  _dRadius  , double _dCntrX   , double _dCntrY ,  //원의 반지름 , 중심점.
                                                         double *_dDist);

        //한개의 직선(pX1, pX2, pY1, pY2)를 가지고 원과의 접점갯수와 위치.
        //return 0 = 접점이 없는 경우
        //       1 = 접점이 1점인 경우
        //       2 = 접점이 2점인 경우
        /*
        점과 직선 사이의 거리
        점 x1,y1 와 직선 ax+by+c = 0 사이의 거리를 d라 할때
        점과 직선 사이의 거리 d=절대값(a*x1+b*y1+c)/루트(이승(a)+이승(b))) 이다.
        */
        static int __stdcall GetCircleLineIntersection(double _dX1      , double _dY1      , double _dX2    , double _dY2 ,
                                                            double _dRadius  , double _dCntrX   , double _dCntrY ,
                                                            TDPoint *_pPnt1 , TDPoint *_pPnt2 );

        //원과 선분의 교점.
        //한개의 선분(pX1, pX2, pY1, pY2)를 가지고 원과의 접점을 구하는 공식
        //return 0 = 접점이 없는 경우
        //       1 = 접점이 1점인 경우
        //       2 = 접점이 2점인 경우
        static int __stdcall GetCircleSegmentIntersection(double _dX1      , double _dY1      , double _dX2    , double _dY2 ,
                                                               double _dRadius  , double _dCntrX   , double _dCntrY ,
                                                               TDPoint *_pPnt1 , TDPoint *_pPnt2 );

        //중점에서 360 도를 _iSampeCnt 만큼으로 분활하여 의 직선 기울기,y절편 구하기.
        static bool __stdcall GetPolarLine(int _iCx , int _iCy , int _iSampleCnt , double * _pA , double * _pB);

        //_iPntCnt개의 점들을 이용하여 Racsac알고리즘으로 _iGap안에 들어 오는 점들이 가장 많은 원을 찾는다.

        //특정 포인트가 다각형 내부에 존재 하는지 여부 리턴.
        //반올림
        static double __stdcall RoundOff(double _dVal , int _iUnderPoint = 0);

        //여기 스코어 이상한거 수정.
        static bool __stdcall GetCircleRansac(TDPoint *_pPnts  , int    _iPntCnt , double  _dGap    ,
                                                   double  *_pCntrX , double *_pCntrY  , double *_pRadius , double *_pScore);

        //_iMargin이 -이면 작은 다각형 , +면 큰다각형 리턴.
        static void _stdcall GetMarginPntsFromPnts(TDPoint * _aSrcPoints , TDPoint * _aDstPoints , int _iPntsCnt , int _iMargin);


        //기타 등등.
        //======================================================================
        //입력한 갯수만큼의 double형의 평균을 구한다.
        static double __stdcall Average(int _iCont, double *_pPnt);

        template< typename T >
        static void quick_sort( T* pArray, int _iLow, int _iHigh );
};
#endif
