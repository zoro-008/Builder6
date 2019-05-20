//---------------------------------------------------------------------------

#ifndef SVMathsH
#define SVMathsH
#include <math.h>
#include <stdio.h>
#include <Types.hpp>
#include <vector.h>
#include "SVDefine.h"
#include "SLogUnit.h"

#define MATH_PI 3.14159265358979323846
#define ROOT_2  1.4142135623730950488

#define MAX_FLOAT_VALUE  ~(1<<(sizeof(float )*8-1))
#define MAX_INT_VALUE    ~(1<<(sizeof(int   )*8-1))
#define MAX_DOUBLE_VALUE ~(1<<(sizeof(double)*8-1))

//sqrt ������.
double __stdcall MATH_Sqrt(double _dVal)
{


    if(_dVal < 0) {
        Trace("MATH_Sqrt",String(_dVal).c_str());

    }
    return sqrt(_dVal);
}


//Line ����.
//======================================================================
float __stdcall MATH_GetLineA(float y, float  x, float b)
{
    if(x==0)return MAX_FLOAT_VALUE ;   //���� �߻�.
    return (y-b)/x;
}

float __stdcall MATH_GetLineB(float y, float  a, float x)
{
    return y-(a*x);
}

float __stdcall MATH_GetLineX(float y, float  a, float b)
{
    if(a==0)return MAX_FLOAT_VALUE ;   //���� �߻�.
    return (y-b)/a;
}

float __stdcall MATH_GetLineY(float a, float  x, float b)
{
    return (a*x)+b;
}

//x1 : Start Point x Of Linear, y1 : Start Point y Of Linear,
//x2 : End Point x Of Linear, y2 : End Point x Of Linear,
float __stdcall MATH_GetLineA(float x1, float y1, float x2, float y2)
{
    if(x2==x1)return MAX_FLOAT_VALUE ; //�����߻�. //�������� ������ ������.X���� �������� �������.
    return (y2-y1)/(x2-x1);
}

float __stdcall MATH_GetLineB(float x1, float y1, float x2, float y2)
{
    return MATH_GetLineB(y1, MATH_GetLineA(x1, y1, x2, y2), x1);
}

float __stdcall MATH_GetLineX(float x1, float y1, float x2, float y2 , float y)
{
    if(x1 == x2) return x1 ;
    float a = MATH_GetLineA(x1, y1, x2, y2);
    float b = MATH_GetLineB(x1, y1, x2, y2);
    return (y-b)/a ;
}

float __stdcall MATH_GetLineY(float x1, float y1, float x2, float y2 , float x)
{
    float a = MATH_GetLineA(x1, y1, x2, y2);
    float b = MATH_GetLineB(x1, y1, x2, y2);
    return a*x+b ;
}

float __stdcall MATH_GetLineA(TFPoint p1, TFPoint p2          )
{
    return MATH_GetLineA(p1.x, p1.y, p2.x, p2.y   );
}

float __stdcall MATH_GetLineB(TFPoint p1, TFPoint p2          )
{
    return MATH_GetLineB(p1.x, p1.y, p2.x, p2.y   );
}

float __stdcall MATH_GetLineX(TFPoint p1, TFPoint p2 , float y) {
    return MATH_GetLineX(p1.x, p1.y, p2.x, p2.y ,y);
}

float __stdcall MATH_GetLineY(TFPoint p1, TFPoint p2 , float x)
{
    return MATH_GetLineY(p1.x, p1.y, p2.x, p2.y ,x);
}

float __stdcall MATH_GetLineA(TPoint p1, TPoint p2          )
{
    return MATH_GetLineA(p1.x, p1.y, p2.x, p2.y   );
}

float __stdcall MATH_GetLineB(TPoint p1, TPoint p2          )
{
    return MATH_GetLineB(p1.x, p1.y, p2.x, p2.y   );
}

float __stdcall MATH_GetLineX(TPoint p1, TPoint p2 , float y)
{
    return MATH_GetLineX(p1.x, p1.y, p2.x, p2.y ,y);
}

float __stdcall MATH_GetLineY(TPoint p1, TPoint p2 , float x)
{
    return MATH_GetLineY(p1.x, p1.y, p2.x, p2.y ,x);
}

bool __stdcall MATH_GetLineCrossPnt(TFPoint Phs, TFPoint Phe, TFPoint Pvs, TFPoint Pve , TFPoint &Ret) //�ѹ� Ȯ���ϱ�.
{
    float t;
    //float s;
    float under = (Pve.y-Pvs.y)*(Phe.x-Phs.x)-(Pve.x-Pvs.x)*(Phe.y-Phs.y);
    if(under==0) return false; //������ ����. �� ������ ����.

    float _t = (Pve.x-Pvs.x)*(Phs.y-Pvs.y) - (Pve.y-Pvs.y)*(Phs.x-Pvs.x);
    float _s = (Phe.x-Phs.x)*(Phs.y-Pvs.y) - (Phe.y-Phs.y)*(Phs.x-Pvs.x);

    t = _t/under;
    //s = _s/under;

    //if(t<0.0 || t>1.0 || s<0.0 || s>1.0) return false;   //���г����� ������ ���ϴ� �ɼ�.
    if(_t==0 && _s==0) return false;

    Ret.x = Phs.x + t * (Phe.x-Phs.x);
    Ret.y = Phs.y + t * (Phe.y-Phs.y);

    return true;
}

bool __stdcall MATH_GetLineCrossPnt(TPoint Phs, TPoint Phe, TPoint Pvs, TPoint Pve , TPoint &Ret)
{
    TFPoint Pnt1 , Pnt2 , Pnt3 , Pnt4 ;
    TFPoint Pnt ;
    bool    bRet ;

    Pnt1.x = Phs.x ; Pnt1.y = Phs.y ;
    Pnt2.x = Phe.x ; Pnt2.y = Phe.y ;
    Pnt3.x = Pvs.x ; Pnt3.y = Pvs.y ;
    Pnt4.x = Pve.x ; Pnt4.y = Pve.y ;

    bRet = MATH_GetLineCrossPnt( Pnt1,  Pnt2,  Pnt3,  Pnt4 , Pnt);

    Ret.x = (long)Pnt.x ;
    Ret.y = (long)Pnt.y ;

    return bRet;
}

//float __stdcall MATH_GetLineLenth(int x1, int y1, int x2, int y2)
//{
//    float dx, dy, dl;
//
//    dx = x2 - x1;
//    dy = y2 - y1;
//    dl = sqrt(pow(dx, 2) + pow(dy, 2));
//
//    return dl;
//}
float __stdcall MATH_GetLineLenth(float x1, float y1, float x2, float y2)
{
    float dx, dy, dl;

    dx = x2 - x1;
    dy = y2 - y1;
    dl = MATH_Sqrt(pow(dx, 2) + pow(dy, 2));

    return dl;
}

//float __stdcall MATH_GetLineAngle(int x1, int y1, int x2, int y2)
//{
//    return MATH_GetLineAngle((float)x1, (float)y1, (float)x2, (float)y2);
//}

float __stdcall MATH_GetLineAngle(float x1, float y1, float x2, float y2)
{
    float rad, deg;
    float cntval ;
    float dx, dy, dl;

    dx = x2 - x1;
    dy = y2 - y1;
    dl = MATH_Sqrt(pow(dx, 2) + pow(dy, 2));

    if (dl==0) return 0.0; //������ ���̰� 0�̶� �Ǻ� ����.

    rad = asin(fabs(dy/dl));

    if (dx >= 0 && dy >= 0) rad = MATH_PI*2 - rad;
    if (dx  < 0 && dy >= 0) rad = MATH_PI   + rad;
    if (dx  < 0 && dy  < 0) rad = MATH_PI   - rad;
    if (dx >= 0 && dy  < 0) rad =             rad;

    deg = (rad*180)/MATH_PI;

    return deg;
}

float __stdcall MATH_GetLengthPntToLine(TFPoint _tPnt , float _fA , float _fB)
{
    float A,B,C ;
    float fRet ;

    //���� ������ ����ax+b=y -> ���������� ����Ax+By+C=0 ��ȯ �ϰ� C=1�� ���Ѵ�.
    A = _fB ? _fA/_fB :  MAX_FLOAT_VALUE ;
    B = _fB ? -(1/_fB): -MAX_FLOAT_VALUE ;
    C = 1.0 ;

    fRet = fabs(A*_tPnt.x + B*_tPnt.y + C) / MATH_Sqrt(pow(A,2)+pow(B,2)) ; //������� ����.

    return fRet ;
}
float __stdcall MATH_GetLengthPntToLine(float _tPntX , float _tPntY , float _fA , float _fB)
{
    TFPoint tPnt ;
    tPnt.x = _tPntX ;
    tPnt.y = _tPntY ;
    return MATH_GetLengthPntToLine(tPnt , _fA , _fB) ;
}

bool __stdcall MATH_GetLineABFromPntByLeastSqure(int _iPntCnt , TFPoint * _pPnt , float & _fA , float & _fB)
{
    float fAvrX , fAvrY ;
    float fSumX , fSumY ;
    float fSumUp , fSumDn ;
    float fRsltA , fRsltB ;

    if(_iPntCnt < 2) return false ; //2�� �̻��̿������� �����Ͱ� ����.

    fSumX = fSumY = 0.0 ;
    for(int i= 0 ; i< _iPntCnt ; i++) {
        fSumX += _pPnt[i].x ;
        fSumY += _pPnt[i].y ;
    }
    fAvrX = fSumX / _iPntCnt ;
    fAvrY = fSumY / _iPntCnt ;

    fSumUp = fSumDn = 0.0 ;
    for(int i= 0 ; i < _iPntCnt ; i++) {
        fSumUp = (fAvrY - _pPnt[i].y)*(fAvrX - _pPnt[i].x) ;
        fSumDn = pow((_pPnt[i].x - fAvrX),2) ;
    }

    if(fSumDn ==0) {
        _fA = MAX_FLOAT_VALUE ;
        _fB = fAvrY - _fA * fAvrX ;
        return false ; //��� x�� ������

    }

    _fA = fSumUp / fSumDn ;
    _fB = fAvrY - _fA * fAvrX ;

    return true ;
}

//���������� ���� �ش簢���� ���̸�ŭ ������ ��.
bool __stdcall MATH_GetPntFromPntByAngLen(float  _fX , float  _fY , //�Է� ������.
                                          float  _fAngle  ,         //�Է� ����.
                                          float  _fLength ,         //�Է� ����.
                                          float *_pX , float *_pY ) //��� ����Ʈ.
{
    //float sss = cos(_fAngle*MATH_PI/180.0);
    *_pX = _fLength * cos(_fAngle*MATH_PI/180.0)  + _fX;
    *_pY =-_fLength * sin(_fAngle*MATH_PI/180.0)  + _fY;
    return true ;
}

bool __stdcall MATH_GetRotPntFromCntrPnt(float _fX     , float _fY     , float _fAng ,  //�Է� x , �Է� y , ��������. , +�� �ݽð����.
                                         float _fCntrX , float _fCntrY ,                //������ ������ x , y
                                         float *_pRetX , float *_pRetY )                //����� x,y
{
    float x = _fX - _fCntrX;
    float y = _fY - _fCntrY;
    float t = -_fAng ;

    *_pRetX = x*cos(t*MATH_PI/180.0) - y*sin(t*MATH_PI/180.0) ;
    *_pRetY = x*sin(t*MATH_PI/180.0) + y*cos(t*MATH_PI/180.0) ;

    *_pRetX += _fCntrX ;
    *_pRetY += _fCntrY ;

    return true ;
}



//bool LineIn(TPoint p1, TPoint p2, TPoint p3)
//{
//     double top, bottom;
//     //�� �Ʒ� ���� ����
//     if(p1.y < p2.y)
//     {
//          top = p1.y;
//          bottom = p2.y;
//     }
//     else
//     {
//          top = p2.y;
//          bottom = p1.y;
//     }
//     //���� y������ �������� ������ ���� �������� ����
//     if(p3.y < top || p3.y >= bottom)return false;
//     //�� ���� p3�� ���� y��ǥ�� �ִ� �� ���ϱ�
//     double new_x;
//     if(p1.y == p2.y) //������
//     {
//          new_x = p1.x;
//     }
//     else //�ƴ� ��� x�� ��ȭ���� ����
//     {
//          double gradient = (p2.x - p1.x) / (p2.y - p1.y);
//          new_x = (p3.y-p1.y)*gradient+p1.x;
//     }
//     //�� ���� ���ϴ� ������ ���ʿ� ������ �������� ����, �������̸� ����
//     if(new_x < p3.x)return false;
//          else return true;
//}
//
////�ٰ������� ����� ���ο� �ִ��� �ܺο� �ִ��� �Ǵ�.
////���� �ʿ�.
//bool PointIn(TPoint * polygon, int point_num, TPoint point)
//{
//     int collusion=0;
//     //�� ���� ���� ��(������ ���������� ���� ���򼱰� ���� �����ϴ��� �˻�)
//     for(int i=0; i<point_num; ++i)
//     {
//          int j = i+1;
//          if(j>=point_num)j=0;
//          if(LineIn(polygon[i], polygon[j], point))++collusion;
//     }
//     //�����ϴ� ���� ¦���̸� ��, Ȧ���̸� ��
//     if((collusion%2)==0)return false;
//     else return true;
//}





//Circle ����.
//======================================================================
//3���� ��ġ�� �Է��Ͽ� ������ǥ �� �������� ���Ѵ�.
//retern : 0 = successed ,
//        -1 = failed ������ �������� ����.
bool __stdcall MATH_GetCircleBy3Points(float  _fX1 , float  _fX2 , float  _fX3 ,
                                       float  _fY1 , float  _fY2 , float  _fY3 ,
                                       float *_pCx , float *_pCy , float *_pRad)
{
    float fX2nX1 = _fX2-_fX1 ;
    float fY2nY1 = _fY2-_fY1 ;
    float fX3nX1 = _fX3-_fX1 ;
    float fY3nY1 = _fY3-_fY1 ;
    float fY3nY2 = _fY3-_fY2 ;
    float fX3nX2 = _fX3-_fX2 ;

    float fX1pX2 = _fX1+_fX2 ;
    float fY1pY2 = _fY1+_fY2 ;
    float fX1pX3 = _fX1+_fX3 ;
    float fY1pY3 = _fY1+_fY3 ;

    float f1_2 = fX2nX1*fX1pX2 + fY2nY1*fY1pY2;
    float f1_3 = fX3nX1*fX1pX3 + fY3nY1*fY1pY3;
    float f123 = 2.*(fX2nX1*fY3nY2-fY2nY1*fX3nX2);

    if(f123==0.) return false;

    *_pCx=(fY3nY1*f1_2 - fY2nY1*f1_3)/f123;
    *_pCy=(fX2nX1*f1_3 - fX3nX1*f1_2)/f123;

    float fx = *_pCx - _fX1 ;
    float fy = *_pCy - _fY1 ;

    *_pRad = MATH_Sqrt(fx * fx + fy * fy);

    return true ;
}

int __stdcall MATH_GetCircleBy3Points(TFPoint _Pnt1 , TFPoint _Pnt2 , TFPoint _Pnt3 ,
                                      float  *_pdCx , float  *_pdCy , float  *_pdRad)
{
    return MATH_GetCircleBy3Points( _Pnt1.x ,  _Pnt2.x ,  _Pnt3.x ,
                                    _Pnt1.y ,  _Pnt2.y ,  _Pnt3.y ,
                                    _pdCx   ,  _pdCy   ,  _pdRad  );

}

//�߽���ǥ�� _dCntrX , _dCntrY �̰� �������� _dRadius �� ���� ���Ŀ��� ������ ��(_pdX, _pdY)�� �� �ȿ� ������ Ȯ��.
//return : true  = �� �ȿ� �� ����.
//         false = �� �ۿ� ����.
bool __stdcall MATH_GetCircleInPoint(float _dCntrX , float _dCntrY , float _dRadius , float _pdX , float _pdY)
{
    float _dLeng = MATH_Sqrt((_pdX - _dCntrX) * (_pdX - _dCntrX) + (_pdY - _dCntrY) * (_pdY - _dCntrY));

    if (_dLeng  <= _dRadius) return true;

    return false;
}

//�Ѱ��� �� P1, P2 �� ���� _Ang�� ������ ���� *_dSlp �� ���� *dInterCPT�� ���ϴ� ����
//���� ������ 0�� ���� ��� ���� .
bool __stdcall MATH_GetSlope(float P1, float P2, float _Ang, float *_dSlp, float *_dInterCPT)
{ //���� ���غ�.
    if(_Ang == 0.0 || _Ang == 270.0 ) _Ang = ~(1<<(sizeof(float)*8-1)) ;//return false;

    *_dSlp = tan(_Ang);
    *_dInterCPT = (*_dSlp * P1) + P2;

    return true;
}

//���� �׸� �ȿ� �ִ���.
bool __stdcall MATH_IsPntInRect(float _fX1 , float _fY1 , float _fX2 , float _fY2 ,
                                float _fX  , float _fY )
{
    float fLeft   = _fX1 < _fX2 ? _fX1 : _fX2 ;
    float fRight  = _fX1 < _fX2 ? _fX2 : _fX1 ;
    float fTop    = _fY1 < _fY2 ? _fY1 : _fY2 ;
    float fBottom = _fY1 < _fY2 ? _fY2 : _fY1 ;

    if(fLeft <= _fX && _fX <= fRight  &&
       fTop  <= _fY && _fY <= fBottom ) return true ;

    return false ;
}

//�Ѱ��� ����(pX1, pX2, pY1, pY2)�� ������ ������ ���������� �Ÿ�.
//return 0 = ������ ���� ���
//       1 = ������ 1���� ���
//       2 = ������ 2���� ���
int  __stdcall MATH_GetCircleLineDistance(float  _fX1      , float _fY1      , float _fX2    , float _fY2 , //���� ��2��.
                                          float  _fRadius  , float _fCntrX   , float _fCntrY ,  //���� ������ , �߽���.
                                          float *_fDist)
{
    float m = (_fY2-_fY1) / (_fX2-_fX1);
    float a = m;
    float b = -1;
    float c = -m*_fX1+_fY1;
    int   iRet ;

    //��(_fCntrX   , _fCntrY)�� ����(a*x+b*y+c=0) ������ �ִ� �Ÿ� d=���밪(a*x1+b*y1+c)/��Ʈ(�̽�(a)+�̽�(b))) �̴�.
    float fTemp = (a*_fCntrX + b*_fCntrY +c)*(a*_fCntrX + b*_fCntrY +c) ;
    float d = fabs(fTemp) / MATH_Sqrt((a*a) + (b*b));
         if (d < _fRadius) {iRet = 2;} //printf("������ 2���Դϴ�");
    else if (d = _fRadius) {iRet = 1;} //printf("������ 1���Դϴ�");
    else if (d > _fRadius) {iRet = 0;} //printf("������ �����ϴ�" );
    *_fDist = d ;
    return iRet ;
}

//�Ѱ��� ����(pX1, pX2, pY1, pY2)�� ������ ������ ���������� ��ġ.
//return 0 = ������ ���� ���
//       1 = ������ 1���� ���
//       2 = ������ 2���� ���
/*
���� ���� ������ �Ÿ�
�� x1,y1 �� ���� ax+by+c = 0 ������ �Ÿ��� d�� �Ҷ�
���� ���� ������ �Ÿ� d=���밪(a*x1+b*y1+c)/��Ʈ(�̽�(a)+�̽�(b))) �̴�.
*/
int __stdcall MATH_GetCircleLineIntersection(float _fX1      , float _fY1      , float _fX2    , float _fY2 ,
                                             float _fRadius  , float _fCntrX   , float _fCntrY ,
                                             TFPoint *_pPnt1 , TFPoint *_pPnt2 )
{
    float dx  ,dy , a , b , c , det , t ;

    _pPnt1 -> x = _pPnt1 -> y =_pPnt2 -> x =_pPnt2 -> y =0 ;

    dx = _fX2 - _fX1 ;
    dy = _fY2 - _fY1 ;

    a = dx * dx + dy * dy ;
    b = 2 * (dx * (_fX1 - _fCntrX) + dy * (_fY1 - _fCntrY)) ;
    c = (_fX1 - _fCntrX) * (_fX1 - _fCntrX) + (_fY1 - _fCntrY) * (_fY1 - _fCntrY) - _fRadius * _fRadius ;

    det = b * b - 4 * a * c ; //���� ���� �Ǻ���.

    if(a==0 || det < 0) {//������ ���� ��� ������ �ִܰŸ��� ���� ������ ���� ũ��.
        return 0 ;
    }
    else if (det == 0) { //������ ���� �������� ���� ������ 1�� ����.
        t = -b / (2*a) ;
        _pPnt1 -> x = _fX1 + t*dx ;
        _pPnt1 -> y = _fY1 + t*dy ;
        return 1 ;
    }
    else { //������ ���� �����ϰ� �־ ���� 2��.
        t = (-b - MATH_Sqrt(det)) / (2 * a);
        _pPnt1 -> x = _fX1 + t*dx ;
        _pPnt1 -> y = _fY1 + t*dy ;

        t = (-b + MATH_Sqrt(det)) / (2 * a);
        _pPnt2 -> x = _fX1 + t*dx ;
        _pPnt2 -> y = _fY1 + t*dy ;
        return 2 ;
    }


}



//���� ������ ����.
//�Ѱ��� ����(pX1, pX2, pY1, pY2)�� ������ ������ ������ ���ϴ� ����
//return 0 = ������ ���� ���
//       1 = ������ 1���� ���
//       2 = ������ 2���� ���
int __stdcall MATH_GetCircleSegmentIntersection(float _fX1      , float _fY1      , float _fX2    , float _fY2 ,
                                                float _fRadius  , float _fCntrX   , float _fCntrY ,
                                                TFPoint *_pPnt1 , TFPoint *_pPnt2 )
{
    int iRet = MATH_GetCircleLineIntersection( _fX1     , _fY1    , _fX2    , _fY2 ,
                                               _fRadius , _fCntrX , _fCntrY ,
                                               _pPnt1   , _pPnt2 );

         if(iRet == 0) {return iRet ; }
    else if(iRet == 1) {
        if(!MATH_IsPntInRect(_fX1 , _fY1 , _fX2 , _fY2 , _pPnt1 -> x , _pPnt1 -> y )) { //1�� ������ �������� ���� ���� �׸� �簢�� �ȿ� ������ 1�������� ���оȿ� ����.
            _pPnt1 -> x = _pPnt1 -> y = 0 ;
            return 0 ;
        }
    }
    else if(iRet == 2) {
        if(!MATH_IsPntInRect(_fX1 , _fY1 , _fX2 , _fY2 , _pPnt1 -> x , _pPnt1 -> y )) { //1�� ������ �������� ���� ���� �׸� �簢�� �ȿ� ������ 1�������� ���оȿ� ����.
            _pPnt1 -> x = _pPnt1 -> y = 0 ;
            iRet-- ;
        }
        if(!MATH_IsPntInRect(_fX1 , _fY1 , _fX2 , _fY2 , _pPnt2 -> x , _pPnt2 -> y )) { //2�� ������ �������� ���� ���� �׸� �簢�� �ȿ� ������ 2�������� ���оȿ� ����.
            _pPnt2 -> x = _pPnt2 -> y = 0 ;
            iRet-- ;
        }

        if( iRet == 1 && _pPnt1 -> x == 0 && _pPnt1 -> y == 0 ){ //������ 1���� ������������ 1�� �������� �ű��.
            _pPnt1 -> x = _pPnt2 -> x ; _pPnt2 -> x = 0 ;
            _pPnt1 -> y = _pPnt2 -> y ; _pPnt2 -> y = 0 ;
        }

        return iRet ;
    }
    else {
        return 0 ;

    }
}


//�������� 360 ���� _iSampeCnt ��ŭ���� ��Ȱ�Ͽ� �� ���� ����,y���� ���ϱ�.
//return 0 = ������ ���� ���
//       1 = ������ 1���� ���
bool __stdcall MATH_GetPolarLine(int _iCx , int _iCy , int _iSampleCnt , float * _pA , float * _pB)
{
    float fAngleUnit = 360 / _iSampleCnt ;

    for(int i = 0 ; i < _iSampleCnt ; i++) {
        if( i * fAngleUnit == 90 || i * fAngleUnit == 270) {
            _pA[i] = ~(1<<(sizeof(float)*8-1)) ;
        }
        else {
            _pA[i] = tan(i*fAngleUnit /180*MATH_PI);
        }
        _pB[i] = _pA[i] * _iCx - _iCy ;
    }
    return true ;
}

//_iPntCnt���� ������ �̿��Ͽ� Racsac�˰������� _iGap�ȿ� ��� ���� ������ ���� ���� ���� ã�´�.



//�ݿø�
float __stdcall MATH_RoundOff(float _fVal , int _iUnderPoint = 0)
{
    if(((int)(_fVal * pow(10 , _iUnderPoint) + 0.5)) == 0) {
        Trace("MATH_RoundOff" , "0 devided");
        //return 0 ;
    }
    return ((int)(_fVal * pow(10 , _iUnderPoint) + 0.5))/(float)pow(10,_iUnderPoint);
}


//���� ���ھ� �̻��Ѱ� ����.
bool __stdcall MATH_GetCircleRansac(TFPoint *_pPnts  , int    _iPntCnt , float  _fGap    ,
                          float   *_pCntrX , float *_pCntrY  , float *_pRadius , float *_pScore)
{
    DWORD dwTime =GetTickCount();
    *_pCntrX = *_pCntrY = *_pRadius = *_pScore = 0.0 ;


    const int iMinSample = 3;
    if (_iPntCnt<iMinSample) {/*sprintf(pErr,"_iPntCnt<iMinSample");*/ return false; }

    //std::vector<TFPoint> consensus(_iPntCnt);
    //std::vector<TFPoint> Maxconsensus(_iPntCnt);

    int    iGapInCnt    ; //�� �������� ���� ����.
    int    iGapInCntMax ; //�ִ� ����.
    float  fX , fY ;
    float  fDev ;
    float  fDevSum ;
    float  fDevMinSum ;
    float  fCntrPntX , fCntrPntY;
    float  fRadius ;

    TFPoint RetPnt[iMinSample] ;

    fDevMinSum    = ~(1<<(sizeof(float)*8-1));
    iGapInCntMax = 0   ;
    int a = 0 , b = 0 , c = 0 ;

    for(a = 0 ; a < _iPntCnt ; a++) {
        b=a+1;
        while(b < _iPntCnt) {
            c=b+1;
            while(c < _iPntCnt) {
                if (!MATH_GetCircleBy3Points(_pPnts[a], _pPnts[b], _pPnts[c], &fCntrPntX, &fCntrPntY, &fRadius)) {
                    c++;
                    continue;
                }

                iGapInCnt = 0 ;
                fDevSum   = 0 ;
                for (int k=0; k<_iPntCnt; k++) {
                    fX  = _pPnts[k].x-fCntrPntX;
                    fY  = _pPnts[k].y-fCntrPntY;
                    fDev = MATH_Sqrt((fX*fX + fY*fY)) - fRadius;
                    if (fabs(fDev) <= _fGap) {
                        //consensus[iGapInCnt] = _pPnts[k];
                        fDevSum += fabs(fDev) ;
                        iGapInCnt++;
                    }
                }

                if(iGapInCntMax <= iGapInCnt) {
                    if(fDevMinSum > fDevSum || iGapInCntMax < iGapInCnt) {
                        iGapInCntMax = iGapInCnt ;
                        fDevMinSum = fDevSum ;
                        RetPnt[0] = _pPnts[a] ;
                        RetPnt[1] = _pPnts[b] ;
                        RetPnt[2] = _pPnts[c] ;
                        //for(int i = 0 ; i < iGapInCntMax ; i++) {
                        //    Maxconsensus[i] = consensus[i] ;
                        //}
                    }
                }
                c++;
            }
            b++;
        }
    }

    if(iGapInCntMax == 0) {/*sprintf(pErr,"iGapInCntMax == 0");*/ return false; }

    if (!MATH_GetCircleBy3Points(RetPnt[0], RetPnt[1], RetPnt[2] , &fCntrPntX, &fCntrPntY, &fRadius)) {/*sprintf(pErr,"GetCircleBy3Points Failed");*/ return false; }

    *_pCntrX  = fCntrPntX ;
    *_pCntrY  = fCntrPntY ;
    *_pRadius = fRadius   ;
    *_pScore  = MATH_RoundOff(iGapInCntMax * 100 / (float)_iPntCnt , 2) ;

    dwTime = GetTickCount()- dwTime ;
    return true ;
}





//��Ÿ ���.
//======================================================================
//�Է��� ������ŭ�� float���� ����� ���Ѵ�.
float __stdcall MATH_Average(int _iCont, float *_pPnt)
{
    float dAvge ;
    float dSum  ;

    for(int i = 0; i <= _iCont ; i++) {
        dSum += _pPnt[i];
    }

    dAvge = dSum / (float)_iCont;

    return dAvge;
}

template< typename T >
void quick_sort( T* pArray, int _iLow, int _iHigh ) {
 
    //�ε��� ����
    if( _iLow >= _iHigh ) {
        return ;
    }

    int iLow, iHigh; //�ε���
    int pivot;       //���� ���� ��

    iLow = _iLow;
    iHigh = _iHigh + 1;
    pivot = pArray[ _iLow ]; //���� �����ʹ� ���� �ε����� �����ͷ�

    do {
        //��ü�� ������ ��ġ�� ã�´�.
        do {}while( pArray[ ++iLow ] < pivot );

         //��ü�� ������ ��ġ�� ã�´�.
        do {}while( pArray[ --iHigh ] > pivot );

        //���غ��� ū �����Ϳ� ���� �����͸� ���� ��ȯ
        if( iLow < iHigh ) {
            //  pArray[ iLow ] ^= pArray[ iHigh ] ^= pArray[ iLow ] ^= pArray[ iHigh ];         //Swap! only integer, short available
            T temp = pArray[ iLow ];
            pArray[ iLow ] = pArray[ iHigh ];
            pArray[ iHigh ] = temp;
         }
         else {
            break;
         }
 
     }while(true);
 
     //���� ��, pivot�� iHigh ��ü
     pArray[ _iLow ] = pArray[ iHigh ];
     pArray[ iHigh ] = pivot;
 
     //���� ���� 
     quick_sort( pArray, _iLow, iHigh - 1 );
     quick_sort( pArray, iHigh + 1, _iHigh );
 
}//void quick_sort



#endif
