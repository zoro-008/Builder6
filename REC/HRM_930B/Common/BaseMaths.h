//---------------------------------------------------------------------------

#ifndef SVMathsH
#define SVMathsH
#include <math.h>
#include "BaseDefine.h"
#include "limits.h"   //������ �θƽ��� ������.
#include "float.h"    //�Ǽ��� �θƽ���.


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
        //Line ����.
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
        static bool   __stdcall GetLineCrossPnt(TDPoint Phs, TDPoint Phe, TDPoint Pvs, TDPoint Pve , TDPoint &Ret); //�ѹ� Ȯ���ϱ�.
        static bool   __stdcall GetLineCrossPnt(TPoint Phs, TPoint Phe, TPoint Pvs, TPoint Pve , TPoint &Ret);

        static bool   __stdcall GetLineCrossPnt(double a1 , double b1 , double a2 , double b2 , TDPoint &Ret);

        static double __stdcall GetLinesIncAngle(TDPoint Phs, TDPoint Phe, TDPoint Pvs, TDPoint Pve);
        static double __stdcall GetLinesIncAngle(TDPoint P1, TDPoint P2, TDPoint P3);
        static double __stdcall GetLineLenth(double x1, double y1, double x2, double y2);
        static double __stdcall GetLineAngle(double x1, double y1, double x2, double y2);

        static double __stdcall GetLengthPntToLine(double _tPntX , double _tPntY , double _dLine1X, double _dLine1Y , double _dLint2X , double _dLine2Y);
        //�̹ؿ� �ΰ��� ���� ���Ѵ� ���� ó�� �ȵǾ� ����.
        static double __stdcall GetLengthPntToLine(TDPoint _tPnt , double _dA , double _dB);
        static double __stdcall GetLengthPntToLine(double _tPntX , double _tPntY , double _dA , double _dB);

        static bool   __stdcall GetLineABFromPntByLeastSqure(int _iPntCnt , TDPoint * _pPnt , double & _dA , double & _dB);
        static bool   __stdcall GetLineABFromPntByLeastSqure(int _iPntCnt , double * _pPntX , double * _pPntY , double & _dA , double & _dB);

        //���������� ���� �ش簢���� ���̸�ŭ ������ ��.
        static void __stdcall GetPntFromPntByAngLen(double  _dX , double  _dY , //�Է� ������.
                                                         double  _dAngle  ,         //�Է� ����.
                                                         double  _dLength ,         //�Է� ����.
                                                         double *_pX , double *_pY ); //��� ����Ʈ.
        static void __stdcall GetPntFromPntByAngLen(double  _dX , double  _dY , //�Է� ������.
                                                         double  _dAngle  ,         //�Է� ����.
                                                         double  _dLength ,         //�Է� ����.
                                                         int    *_pX , int *_pY ); //��� ����Ʈ.
        static void __stdcall GetRotPntFromCntrPnt(double _dX     , double _dY     , double _dAng ,  //�Է� x , �Է� y , ��������. , +�� �ݽð����.
                                                        double _dCntrX , double _dCntrY ,                //������ ������ x , y
                                                        double *_pRetX , double *_pRetY );                //����� x,y


        //Circle ����.
        //======================================================================
        //3���� ��ġ�� �Է��Ͽ� ������ǥ �� �������� ���Ѵ�.
        //retern : 0 = successed ,
        //        -1 = failed ������ �������� ����.
        static bool __stdcall GetCircleBy3Points(double  _dX1 , double  _dX2 , double  _dX3 ,
                                                      double  _dY1 , double  _dY2 , double  _dY3 ,
                                                      double *_pCx , double *_pCy , double *_pRad);

        static int __stdcall GetCircleBy3Points(TDPoint _Pnt1 , TDPoint _Pnt2 , TDPoint _Pnt3 ,
                                                     double  *_pdCx , double  *_pdCy , double  *_pdRad);
        //�߽���ǥ�� _dCntrX , _dCntrY �̰� �������� _dRadius �� ���� ���Ŀ��� ������ ��(_pdX, _pdY)�� �� �ȿ� ������ Ȯ��.
        //return : true  = �� �ȿ� �� ����.
        //         false = �� �ۿ� ����.
        static bool __stdcall GetCircleInPoint(double _dCntrX , double _dCntrY , double _dRadius , double _pdX , double _pdY);

        //3����Ʈ�� ������� �̵������� CCW�����̸� 1, �ƴϸ� -1
        static int GetPntCCW(TDPoint _tPnt1, TDPoint _tPnt2, TDPoint _tPnt3);

        //2���� ������ �����Ǹ� true
        static bool GetSegmentIntersect(TDPoint _tPntStt1, TDPoint _tPntEnd1, TDPoint _tPntStt2, TDPoint _tPntEnd2);

        //����Ʈ���� �ֿܰ� ���簢��.
        static bool  GetOutRectFromPnts(TDPoint *_pPoints, int _iPntCnt, TDRect * _tRect);

        //����Ʈ�� �����ϴ� ���� ���� �簢�� �ȿ� �ش� ����Ʈ�� �ִ��� Ȯ��.
        static bool  IsPntInPolygonOutRect(TDPoint *_pPoints, int _iPntCnt, TDPoint _tPnt, TDRect &_tOutRect);

        static bool IsPntInPolygon(TDPoint *_pPoints, int _iPntCnt, TDPoint _tPnt);

        //���� �׸� �ȿ� �ִ���.
        static bool __stdcall IsPntInRect(double _dX1 , double _dY1 , double _dX2 , double _dY2 ,
                                               double _dX  , double _dY );
        //�Ѱ��� ����(pX1, pX2, pY1, pY2)�� ������ ������ ���������� �Ÿ�.
        //return 0 = ������ ���� ���
        //       1 = ������ 1���� ���
        //       2 = ������ 2���� ���
        static int  __stdcall GetCircleLineDistance(double  _dX1      , double _dY1      , double _dX2    , double _dY2 , //���� ��2��.
                                                         double  _dRadius  , double _dCntrX   , double _dCntrY ,  //���� ������ , �߽���.
                                                         double *_dDist);

        //�Ѱ��� ����(pX1, pX2, pY1, pY2)�� ������ ������ ���������� ��ġ.
        //return 0 = ������ ���� ���
        //       1 = ������ 1���� ���
        //       2 = ������ 2���� ���
        /*
        ���� ���� ������ �Ÿ�
        �� x1,y1 �� ���� ax+by+c = 0 ������ �Ÿ��� d�� �Ҷ�
        ���� ���� ������ �Ÿ� d=���밪(a*x1+b*y1+c)/��Ʈ(�̽�(a)+�̽�(b))) �̴�.
        */
        static int __stdcall GetCircleLineIntersection(double _dX1      , double _dY1      , double _dX2    , double _dY2 ,
                                                            double _dRadius  , double _dCntrX   , double _dCntrY ,
                                                            TDPoint *_pPnt1 , TDPoint *_pPnt2 );

        //���� ������ ����.
        //�Ѱ��� ����(pX1, pX2, pY1, pY2)�� ������ ������ ������ ���ϴ� ����
        //return 0 = ������ ���� ���
        //       1 = ������ 1���� ���
        //       2 = ������ 2���� ���
        static int __stdcall GetCircleSegmentIntersection(double _dX1      , double _dY1      , double _dX2    , double _dY2 ,
                                                               double _dRadius  , double _dCntrX   , double _dCntrY ,
                                                               TDPoint *_pPnt1 , TDPoint *_pPnt2 );

        //�������� 360 ���� _iSampeCnt ��ŭ���� ��Ȱ�Ͽ� �� ���� ����,y���� ���ϱ�.
        static bool __stdcall GetPolarLine(int _iCx , int _iCy , int _iSampleCnt , double * _pA , double * _pB);

        //_iPntCnt���� ������ �̿��Ͽ� Racsac�˰������� _iGap�ȿ� ��� ���� ������ ���� ���� ���� ã�´�.

        //Ư�� ����Ʈ�� �ٰ��� ���ο� ���� �ϴ��� ���� ����.
        //�ݿø�
        static double __stdcall RoundOff(double _dVal , int _iUnderPoint = 0);

        //���� ���ھ� �̻��Ѱ� ����.
        static bool __stdcall GetCircleRansac(TDPoint *_pPnts  , int    _iPntCnt , double  _dGap    ,
                                                   double  *_pCntrX , double *_pCntrY  , double *_pRadius , double *_pScore);

        //_iMargin�� -�̸� ���� �ٰ��� , +�� ū�ٰ��� ����.
        static void _stdcall GetMarginPntsFromPnts(TDPoint * _aSrcPoints , TDPoint * _aDstPoints , int _iPntsCnt , int _iMargin);


        //��Ÿ ���.
        //======================================================================
        //�Է��� ������ŭ�� double���� ����� ���Ѵ�.
        static double __stdcall Average(int _iCont, double *_pPnt);

        template< typename T >
        static void quick_sort( T* pArray, int _iLow, int _iHigh );
};
#endif
