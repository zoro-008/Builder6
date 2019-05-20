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

//sqrt 디버깅용.
double __stdcall MATH_Sqrt(double _dVal)
{


    if(_dVal < 0) {
        Trace("MATH_Sqrt",String(_dVal).c_str());

    }
    return sqrt(_dVal);
}


//Line 관련.
//======================================================================
float __stdcall MATH_GetLineA(float y, float  x, float b)
{
    if(x==0)return MAX_FLOAT_VALUE ;   //오차 발생.
    return (y-b)/x;
}

float __stdcall MATH_GetLineB(float y, float  a, float x)
{
    return y-(a*x);
}

float __stdcall MATH_GetLineX(float y, float  a, float b)
{
    if(a==0)return MAX_FLOAT_VALUE ;   //오차 발생.
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
    if(x2==x1)return MAX_FLOAT_VALUE ; //오차발생. //일직선상에 있으면 오류남.X점이 수직으로 있을경우.
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

bool __stdcall MATH_GetLineCrossPnt(TFPoint Phs, TFPoint Phe, TFPoint Pvs, TFPoint Pve , TFPoint &Ret) //한번 확인하기.
{
    float t;
    //float s;
    float under = (Pve.y-Pvs.y)*(Phe.x-Phs.x)-(Pve.x-Pvs.x)*(Phe.y-Phs.y);
    if(under==0) return false; //교점이 없음. 두 직선이 수평.

    float _t = (Pve.x-Pvs.x)*(Phs.y-Pvs.y) - (Pve.y-Pvs.y)*(Phs.x-Pvs.x);
    float _s = (Phe.x-Phs.x)*(Phs.y-Pvs.y) - (Phe.y-Phs.y)*(Phs.x-Pvs.x);

    t = _t/under;
    //s = _s/under;

    //if(t<0.0 || t>1.0 || s<0.0 || s>1.0) return false;   //선분끼리의 교점을 구하는 옵션.
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

    if (dl==0) return 0.0; //직선의 길이가 0이라 판별 못함.

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

    //직선 방정식 형태ax+b=y -> 직선방정식 형태Ax+By+C=0 변환 하고 C=1로 정한다.
    A = _fB ? _fA/_fB :  MAX_FLOAT_VALUE ;
    B = _fB ? -(1/_fB): -MAX_FLOAT_VALUE ;
    C = 1.0 ;

    fRet = fabs(A*_tPnt.x + B*_tPnt.y + C) / MATH_Sqrt(pow(A,2)+pow(B,2)) ; //여기부터 보기.

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

    if(_iPntCnt < 2) return false ; //2개 이상이여야지만 데이터가 나옴.

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
        return false ; //모든 x가 같을때

    }

    _fA = fSumUp / fSumDn ;
    _fB = fAvrY - _fA * fAvrX ;

    return true ;
}

//시작점에서 부터 해당각도와 길이만큼 떨어진 점.
bool __stdcall MATH_GetPntFromPntByAngLen(float  _fX , float  _fY , //입력 시작점.
                                          float  _fAngle  ,         //입력 각도.
                                          float  _fLength ,         //입력 길이.
                                          float *_pX , float *_pY ) //출력 포인트.
{
    //float sss = cos(_fAngle*MATH_PI/180.0);
    *_pX = _fLength * cos(_fAngle*MATH_PI/180.0)  + _fX;
    *_pY =-_fLength * sin(_fAngle*MATH_PI/180.0)  + _fY;
    return true ;
}

bool __stdcall MATH_GetRotPntFromCntrPnt(float _fX     , float _fY     , float _fAng ,  //입력 x , 입력 y , 돌릴각도. , +가 반시계방향.
                                         float _fCntrX , float _fCntrY ,                //돌리는 기준점 x , y
                                         float *_pRetX , float *_pRetY )                //결과값 x,y
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
//     //위 아래 범위 결정
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
//     //변의 y범위에 속해있지 않으면 절대 교차하지 않음
//     if(p3.y < top || p3.y >= bottom)return false;
//     //변 안의 p3와 같은 y좌표에 있는 점 구하기
//     double new_x;
//     if(p1.y == p2.y) //수직선
//     {
//          new_x = p1.x;
//     }
//     else //아닌 경우 x의 변화량을 구함
//     {
//          double gradient = (p2.x - p1.x) / (p2.y - p1.y);
//          new_x = (p3.y-p1.y)*gradient+p1.x;
//     }
//     //그 점이 비교하는 점보다 왼쪽에 있으면 교차하지 않음, 오른쪽이면 교차
//     if(new_x < p3.x)return false;
//          else return true;
//}
//
////다각형에서 어떤점이 내부에 있는지 외부에 있는지 판단.
////검증 필요.
//bool PointIn(TPoint * polygon, int point_num, TPoint point)
//{
//     int collusion=0;
//     //각 변을 점과 비교(점에서 오른쪽으로 그은 수평선과 변이 교차하는지 검사)
//     for(int i=0; i<point_num; ++i)
//     {
//          int j = i+1;
//          if(j>=point_num)j=0;
//          if(LineIn(polygon[i], polygon[j], point))++collusion;
//     }
//     //교차하는 수가 짝수이면 밖, 홀수이면 안
//     if((collusion%2)==0)return false;
//     else return true;
//}





//Circle 관련.
//======================================================================
//3점의 위치를 입력하여 중점좌표 및 반지름을 구한다.
//retern : 0 = successed ,
//        -1 = failed 세점이 일직선상에 있음.
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

//중심좌표가 _dCntrX , _dCntrY 이고 반지름이 _dRadius 인 원의 공식에서 임의의 점(_pdX, _pdY)이 원 안에 들어가는지 확인.
//return : true  = 원 안에 들어가 있음.
//         false = 원 밖에 있음.
bool __stdcall MATH_GetCircleInPoint(float _dCntrX , float _dCntrY , float _dRadius , float _pdX , float _pdY)
{
    float _dLeng = MATH_Sqrt((_pdX - _dCntrX) * (_pdX - _dCntrX) + (_pdY - _dCntrY) * (_pdY - _dCntrY));

    if (_dLeng  <= _dRadius) return true;

    return false;
}

//한개의 점 P1, P2 과 각도 _Ang를 가지고 기울기 *_dSlp 와 절편 *dInterCPT를 구하는 공식
//만약 각도가 0일 떄는 계산 오차 .
bool __stdcall MATH_GetSlope(float P1, float P2, float _Ang, float *_dSlp, float *_dInterCPT)
{ //검증 안해봄.
    if(_Ang == 0.0 || _Ang == 270.0 ) _Ang = ~(1<<(sizeof(float)*8-1)) ;//return false;

    *_dSlp = tan(_Ang);
    *_dInterCPT = (*_dSlp * P1) + P2;

    return true;
}

//점이 네모 안에 있는지.
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

//한개의 직선(pX1, pX2, pY1, pY2)를 가지고 원과의 접점갯수와 거리.
//return 0 = 접점이 없는 경우
//       1 = 접점이 1점인 경우
//       2 = 접점이 2점인 경우
int  __stdcall MATH_GetCircleLineDistance(float  _fX1      , float _fY1      , float _fX2    , float _fY2 , //직선 점2개.
                                          float  _fRadius  , float _fCntrX   , float _fCntrY ,  //원의 반지름 , 중심점.
                                          float *_fDist)
{
    float m = (_fY2-_fY1) / (_fX2-_fX1);
    float a = m;
    float b = -1;
    float c = -m*_fX1+_fY1;
    int   iRet ;

    //점(_fCntrX   , _fCntrY)과 직선(a*x+b*y+c=0) 사이의 최단 거리 d=절대값(a*x1+b*y1+c)/루트(이승(a)+이승(b))) 이다.
    float fTemp = (a*_fCntrX + b*_fCntrY +c)*(a*_fCntrX + b*_fCntrY +c) ;
    float d = fabs(fTemp) / MATH_Sqrt((a*a) + (b*b));
         if (d < _fRadius) {iRet = 2;} //printf("교점은 2개입니다");
    else if (d = _fRadius) {iRet = 1;} //printf("교점은 1개입니다");
    else if (d > _fRadius) {iRet = 0;} //printf("교점은 없습니다" );
    *_fDist = d ;
    return iRet ;
}

//한개의 직선(pX1, pX2, pY1, pY2)를 가지고 원과의 접점갯수와 위치.
//return 0 = 접점이 없는 경우
//       1 = 접점이 1점인 경우
//       2 = 접점이 2점인 경우
/*
점과 직선 사이의 거리
점 x1,y1 와 직선 ax+by+c = 0 사이의 거리를 d라 할때
점과 직선 사이의 거리 d=절대값(a*x1+b*y1+c)/루트(이승(a)+이승(b))) 이다.
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

    det = b * b - 4 * a * c ; //해의 갯수 판별식.

    if(a==0 || det < 0) {//교점이 없는 경우 직선의 최단거리가 원의 반지름 보다 크다.
        return 0 ;
    }
    else if (det == 0) { //직선이 원의 접선으로 있음 교점이 1개 있음.
        t = -b / (2*a) ;
        _pPnt1 -> x = _fX1 + t*dx ;
        _pPnt1 -> y = _fY1 + t*dy ;
        return 1 ;
    }
    else { //직선이 원을 관통하고 있어서 교점 2개.
        t = (-b - MATH_Sqrt(det)) / (2 * a);
        _pPnt1 -> x = _fX1 + t*dx ;
        _pPnt1 -> y = _fY1 + t*dy ;

        t = (-b + MATH_Sqrt(det)) / (2 * a);
        _pPnt2 -> x = _fX1 + t*dx ;
        _pPnt2 -> y = _fY1 + t*dy ;
        return 2 ;
    }


}



//원과 선분의 교점.
//한개의 선분(pX1, pX2, pY1, pY2)를 가지고 원과의 접점을 구하는 공식
//return 0 = 접점이 없는 경우
//       1 = 접점이 1점인 경우
//       2 = 접점이 2점인 경우
int __stdcall MATH_GetCircleSegmentIntersection(float _fX1      , float _fY1      , float _fX2    , float _fY2 ,
                                                float _fRadius  , float _fCntrX   , float _fCntrY ,
                                                TFPoint *_pPnt1 , TFPoint *_pPnt2 )
{
    int iRet = MATH_GetCircleLineIntersection( _fX1     , _fY1    , _fX2    , _fY2 ,
                                               _fRadius , _fCntrX , _fCntrY ,
                                               _pPnt1   , _pPnt2 );

         if(iRet == 0) {return iRet ; }
    else if(iRet == 1) {
        if(!MATH_IsPntInRect(_fX1 , _fY1 , _fX2 , _fY2 , _pPnt1 -> x , _pPnt1 -> y )) { //1번 접점이 시작점과 끝점 으로 그린 사각형 안에 있으면 1번접점이 선분안에 있음.
            _pPnt1 -> x = _pPnt1 -> y = 0 ;
            return 0 ;
        }
    }
    else if(iRet == 2) {
        if(!MATH_IsPntInRect(_fX1 , _fY1 , _fX2 , _fY2 , _pPnt1 -> x , _pPnt1 -> y )) { //1번 접점이 시작점과 끝점 으로 그린 사각형 안에 있으면 1번접점이 선분안에 있음.
            _pPnt1 -> x = _pPnt1 -> y = 0 ;
            iRet-- ;
        }
        if(!MATH_IsPntInRect(_fX1 , _fY1 , _fX2 , _fY2 , _pPnt2 -> x , _pPnt2 -> y )) { //2번 접점이 시작점과 끝점 으로 그린 사각형 안에 있으면 2번접점이 선분안에 있음.
            _pPnt2 -> x = _pPnt2 -> y = 0 ;
            iRet-- ;
        }

        if( iRet == 1 && _pPnt1 -> x == 0 && _pPnt1 -> y == 0 ){ //접점이 1개만 남았을때에는 1번 접점으로 옮긴다.
            _pPnt1 -> x = _pPnt2 -> x ; _pPnt2 -> x = 0 ;
            _pPnt1 -> y = _pPnt2 -> y ; _pPnt2 -> y = 0 ;
        }

        return iRet ;
    }
    else {
        return 0 ;

    }
}


//중점에서 360 도를 _iSampeCnt 만큼으로 분활하여 의 직선 기울기,y절편 구하기.
//return 0 = 접점이 없는 경우
//       1 = 접점이 1점인 경우
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

//_iPntCnt개의 점들을 이용하여 Racsac알고리즘으로 _iGap안에 들어 오는 점들이 가장 많은 원을 찾는다.



//반올림
float __stdcall MATH_RoundOff(float _fVal , int _iUnderPoint = 0)
{
    if(((int)(_fVal * pow(10 , _iUnderPoint) + 0.5)) == 0) {
        Trace("MATH_RoundOff" , "0 devided");
        //return 0 ;
    }
    return ((int)(_fVal * pow(10 , _iUnderPoint) + 0.5))/(float)pow(10,_iUnderPoint);
}


//여기 스코어 이상한거 수정.
bool __stdcall MATH_GetCircleRansac(TFPoint *_pPnts  , int    _iPntCnt , float  _fGap    ,
                          float   *_pCntrX , float *_pCntrY  , float *_pRadius , float *_pScore)
{
    DWORD dwTime =GetTickCount();
    *_pCntrX = *_pCntrY = *_pRadius = *_pScore = 0.0 ;


    const int iMinSample = 3;
    if (_iPntCnt<iMinSample) {/*sprintf(pErr,"_iPntCnt<iMinSample");*/ return false; }

    //std::vector<TFPoint> consensus(_iPntCnt);
    //std::vector<TFPoint> Maxconsensus(_iPntCnt);

    int    iGapInCnt    ; //갭 안쪽으로 들어온 숫자.
    int    iGapInCntMax ; //최대 숫자.
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





//기타 등등.
//======================================================================
//입력한 갯수만큼의 float형의 평균을 구한다.
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
 
    //인덱스 점검
    if( _iLow >= _iHigh ) {
        return ;
    }

    int iLow, iHigh; //인덱스
    int pivot;       //비교할 기준 값

    iLow = _iLow;
    iHigh = _iHigh + 1;
    pivot = pArray[ _iLow ]; //기준 데이터는 최하 인덱스의 데이터로

    do {
        //교체할 데이터 위치를 찾는다.
        do {}while( pArray[ ++iLow ] < pivot );

         //교체할 데이터 위치를 찾는다.
        do {}while( pArray[ --iHigh ] > pivot );

        //기준보다 큰 데이터와 적은 데이터를 서로 교환
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
 
     //정렬 후, pivot과 iHigh 교체
     pArray[ _iLow ] = pArray[ iHigh ];
     pArray[ iHigh ] = pivot;
 
     //분할 정복 
     quick_sort( pArray, _iLow, iHigh - 1 );
     quick_sort( pArray, iHigh + 1, _iHigh );
 
}//void quick_sort



#endif
