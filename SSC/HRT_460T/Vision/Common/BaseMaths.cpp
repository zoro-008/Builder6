
//---------------------------------------------------------------------------

#include <stdio.h>
#include <Types.hpp>
#include <vector.h>
#pragma hdrstop
#include "BaseMaths.h"



#pragma package(smart_init)

//---------------------------------------------------------------------------





//Line 관련.
//======================================================================
double __stdcall CMath::GetLineA(double y, double  x, double b)
{
    if(x==0){
             if((y-b) ==0 ) return      0.0 ;     //이거 DBL_MAX로 쓰면 오버플로우 발생함.....
        else if((y-b) > 0 ) return  FLT_MAX ;//FLT_MAX ;   //오차 발생.
        else if((y-b) < 0 ) return -FLT_MAX ;//FLT_MAX ;   //오차 발생.
    }
    return (y-b)/x;
}

double __stdcall CMath::GetLineB(double y, double  a, double x)
{
    return y-(a*x);
}

double __stdcall CMath::GetLineX(double y, double  a, double b)
{
    //if(a==0)return FLT_MAX ;   //오차 발생.
    if(a==0){
             if((y-b) ==0 ) return      0.0 ;
        else if((y-b) > 0 ) return  FLT_MAX ;// FLT_MAX ;   //오차 발생.
        else if((y-b) < 0 ) return -FLT_MAX ;//-FLT_MAX ;   //오차 발생.
    }
    return (y-b)/a;
}

double __stdcall CMath::GetLineY(double a, double  x, double b)
{
    return (a*x)+b;
}

//x1 : Start Point x Of Linear, y1 : Start Point y Of Linear,
//x2 : End Point x Of Linear, y2 : End Point x Of Linear,
double __stdcall CMath::GetLineA(double x1, double y1, double x2, double y2)
{
    //if(x2==x1)return FLT_MAX ; //오차발생. //일직선상에 있으면 오류남.X점이 수직으로 있을경우.
    if(x2==x1){
             if((y2-y1) ==0 ) return      0.0 ;
        else if((y2-y1) > 0 ) return  FLT_MAX ;// FLT_MAX ;   //오차 발생.
        else if((y2-y1) < 0 ) return -FLT_MAX ;//-FLT_MAX ;   //오차 발생.
    }
    return (y2-y1)/(x2-x1);
}

double __stdcall CMath::GetLineB(double x1, double y1, double x2, double y2)
{
    return CMath::GetLineB(y1, CMath::GetLineA(x1, y1, x2, y2), x1);
}

double __stdcall CMath::GetLineX(double x1, double y1, double x2, double y2 , double y)
{
    //if(x1 == x2) return x1 ;
    if(x1 == x2){
        return x1 ;
    }

    double a = CMath::GetLineA(x1, y1, x2, y2);
    double b = CMath::GetLineB(x1, y1, x2, y2);
    return (y-b)/a ;
}

double __stdcall CMath::GetLineY(double x1, double y1, double x2, double y2 , double x)
{
    double a = CMath::GetLineA(x1, y1, x2, y2);
    double b = CMath::GetLineB(x1, y1, x2, y2);
    return a*x+b ;
}

double __stdcall CMath::GetLineA(TDPoint p1, TDPoint p2          )
{
    return CMath::GetLineA(p1.x, p1.y, p2.x, p2.y   );
}

double __stdcall CMath::GetLineB(TDPoint p1, TDPoint p2          )
{
    return CMath::GetLineB(p1.x, p1.y, p2.x, p2.y   );
}

double __stdcall CMath::GetLineX(TDPoint p1, TDPoint p2 , double y) {
    return CMath::GetLineX(p1.x, p1.y, p2.x, p2.y ,y);
}

double __stdcall CMath::GetLineY(TDPoint p1, TDPoint p2 , double x)
{
    return CMath::GetLineY(p1.x, p1.y, p2.x, p2.y ,x);
}

double __stdcall CMath::GetLineA(TPoint p1, TPoint p2          )
{
    return CMath::GetLineA(p1.x, p1.y, p2.x, p2.y   );
}

double __stdcall CMath::GetLineB(TPoint p1, TPoint p2          )
{
    return CMath::GetLineB(p1.x, p1.y, p2.x, p2.y   );
}

double __stdcall CMath::GetLineX(TPoint p1, TPoint p2 , double y)
{
    return CMath::GetLineX(p1.x, p1.y, p2.x, p2.y ,y);
}

double __stdcall CMath::GetLineY(TPoint p1, TPoint p2 , double x)
{
    return CMath::GetLineY(p1.x, p1.y, p2.x, p2.y ,x);
}

double __stdcall CMath::GetLineAngleFromA(double _dA)
{
    return atan(_dA)* 180.0/MATH_PI;

}

//#include "SLogUnit.h"
bool __stdcall CMath::GetLineCrossPnt(TDPoint Phs, TDPoint Phe, TDPoint Pvs, TDPoint Pve , TDPoint &Ret) //한번 확인하기.
{
    //String sTemp ;

    //sTemp.sprintf("Phs.x=%f y=%f Phe.x=%f y=%f ", Phs.x , Phs.y ,Phe.x , Phe.y  );
    //Trace("Horizen",sTemp.c_str());


    //sTemp.sprintf("Pvs.x=%f y=%f Pve.x=%f y=%f ", Pvs.x , Pvs.y ,Pve.x , Pve.y  );
    //Trace("Vertical",sTemp.c_str());

/*
    double dd1 = 565  ;
    double dd2 = 305  ;
    double dd3 = 1273 ;
    double dd4 = 553  ;

    //double s;
    double ddTemp1 ;
    ddTemp1  = (dd1-dd2);
    ddTemp1 *= (dd3-dd4) ;






*/
    double t;

    double d1 = Pve.y ;
    double d2 = Pvs.y ;
    double d3 = Phe.x ;
    double d4 = Phs.x ;

    //double s;
    double dTemp1 = (d1-d2)*(d3-d4) ;
    double dTemp2 = (Pve.x-Pvs.x)*(Phe.y-Phs.y);
    double under = dTemp1 - dTemp2 ;
    if(under==0) return false; //교점이 없음. 두 직선이 수평.

    double _t = (Pve.x-Pvs.x)*(Phs.y-Pvs.y) - (Pve.y-Pvs.y)*(Phs.x-Pvs.x);
    double _s = (Phe.x-Phs.x)*(Phs.y-Pvs.y) - (Phe.y-Phs.y)*(Phs.x-Pvs.x);

    t = _t/under;
    //s = _s/under;

    //if(t<0.0 || t>1.0 || s<0.0 || s>1.0) return false;   //선분끼리의 교점을 구하는 옵션.
    if(_t==0 && _s==0) return false;

    Ret.x = Phs.x + t * (Phe.x-Phs.x);
    Ret.y = Phs.y + t * (Phe.y-Phs.y);

    //sTemp.sprintf("Ret.x=%.f y=%.f ", Ret.x , Ret.y );
    //Trace("Ret",sTemp.c_str());

    return true;
}

bool __stdcall CMath::GetLineCrossPnt(TPoint Phs, TPoint Phe, TPoint Pvs, TPoint Pve , TPoint &Ret)
{
    TDPoint Pnt1 , Pnt2 , Pnt3 , Pnt4 ;
    TDPoint Pnt ;
    bool    bRet ;

    Pnt1.x = Phs.x ; Pnt1.y = Phs.y ;
    Pnt2.x = Phe.x ; Pnt2.y = Phe.y ;
    Pnt3.x = Pvs.x ; Pnt3.y = Pvs.y ;
    Pnt4.x = Pve.x ; Pnt4.y = Pve.y ;

    //Trace("CMath::GetLineCrossPnt1","stt");
    bRet = CMath::GetLineCrossPnt( Pnt1,  Pnt2,  Pnt3,  Pnt4 , Pnt);
    //Trace("CMath::GetLineCrossPnt2","stt");
    Ret.x = (long)Pnt.x ;
    Ret.y = (long)Pnt.y ;

    return bRet;
}

//리턴 false 는 두직선이 수평일때.
bool __stdcall CMath::GetLineCrossPnt(double a1 , double b1 , double a2 , double b2 , TDPoint &Ret)
{
    if(a1 == a2) {
        Ret.Clear();
        return false ;
    }

    Ret.x = (b2-b1)/(a1-a2);
    Ret.y = a1*Ret.x + b1 ;

    return true ;

}


double __stdcall CMath::GetLinesIncAngle(TDPoint Phs, TDPoint Phe, TDPoint Pvs, TDPoint Pve)
{
    double angle1 = atan2(Phs.y-Phe.y, Phs.x-Phe.x)* 180 / MATH_PI;
    double angle2 = atan2(Pvs.y-Pve.y, Pvs.x-Pve.x)* 180 / MATH_PI;
    double result = (angle2-angle1) ;

    if(result < 0) result += 360 ;

    result = 180 - result ;

    return result ;
/*

    double dA1 = GetLineA(Phs.x,Phs.y,Phe.x,Phe.y);
    double dA2 = GetLineA(Pvs.x,Pvs.y,Pve.x,Pve.y);

    double dGapA =GetLineAngleFromA(fabs(dA1 - dA2));


    double mod1   = sqrt(pow(y2-y1,2)+pow(x2-x1,2));
    double mod2   = sqrt(pow(y3-y1,2)+pow(x2-x1,2));
    double innerp = (x2-x1)*(x2-x1) + (y2-y1)*(y3-y1);
    double angle  = acos(innerp / (mod1 * mod2));

    return dGapA ;
*/
}

double __stdcall CMath::GetLinesIncAngle(TDPoint P1, TDPoint P2, TDPoint P3)
{
    double angle1 = atan2(P1.y-P2.y, P1.x-P2.x)* 180 / MATH_PI;
    double angle2 = atan2(P2.y-P3.y, P2.x-P3.x)* 180 / MATH_PI;

    double result = (angle2-angle1) ;

    if(result < 0) result += 360 ;

    result = 180 - result ;
                                                             


    //double result = (angle2+angle1) ;
    //if (result<0) {
    //    result+=360;
    //}
    return result;
}


//double __stdcall CMath::GetLineLenth(int x1, int y1, int x2, int y2)
//{
//    double dx, dy, dl;
//
//    dx = x2 - x1;
//    dy = y2 - y1;
//    dl = sqrt(pow(dx, 2) + pow(dy, 2));
//
//    return dl;
//}
double __stdcall CMath::GetLineLenth(double x1, double y1, double x2, double y2)
{
    double dx, dy, dl;

    dx = x2 - x1;
    dy = y2 - y1;
    dl = sqrt(pow(dx, 2) + pow(dy, 2));

    return dl;
}

//double __stdcall CMath::GetLineAngle(int x1, int y1, int x2, int y2)
//{
//    return CMath::GetLineAngle((double)x1, (double)y1, (double)x2, (double)y2);
//}

double __stdcall CMath::GetLineAngle(double x1, double y1, double x2, double y2)
{
    double rad, deg;
    double cntval ;
    double dx, dy, dl;

    dx = x2 - x1;
    dy = y2 - y1;
    dl = sqrt(pow(dx, 2) + pow(dy, 2));

    if (dl==0) return 0.0; //직선의 길이가 0이라 판별 못함.

    rad = asin(fabs(dy/dl));

    if (dx >= 0 && dy >= 0) rad = MATH_PI*2 - rad;
    if (dx  < 0 && dy >= 0) rad = MATH_PI   + rad;
    if (dx  < 0 && dy  < 0) rad = MATH_PI   - rad;
    if (dx >= 0 && dy  < 0) rad =             rad;

    deg = (rad*180)/MATH_PI;

    return deg;
}
/*

double __stdcall CMath::GetLengthPntToLine(TDPoint _tPnt , double _dA , double _dB)
{
    double A,B,C ;
    double fRet ;

    //직선 방정식 형태ax+b=y -> 직선방정식 형태Ax+By+C=0 변환 하고 C=1로 정한다.
    A = _dB ? _dA/_dB :  FLT_MAX ;
    B = _dB ? -(1/_dB): -FLT_MAX ;
    C = 1.0 ;

    //_dB가 0일 경우 더블형 오버플로우 남.
    fRet = fabs(A*_tPnt.x + B*_tPnt.y + C) / sqrt(pow(A,2)+pow(B,2)) ; //여기부터 보기.

    return fRet ;
}
*/
double __stdcall CMath::GetLengthPntToLine(TDPoint _tPnt , double _dA , double _dB)
{
    double A,B,C ;
    double fRet ;

    //직선 방정식 형태ax+b=y -> 직선방정식 형태Ax+By+C=0 변환 하고 C=1로 정한다.
    if(_dB !=0){
        A = _dA/_dB ;
        B = -(1/_dB);
        C = 1.0 ;
        fRet = fabs(A*_tPnt.x + B*_tPnt.y + C) / sqrt(pow(A,2)+pow(B,2)) ; //여기부터 보기.
    }
    else {
        fRet = (A*_tPnt.x + B*_tPnt.y + C) * (A*_tPnt.x + B*_tPnt.y + C) ;
        fRet = fRet /pow(A,2)+pow(B,2) ;
    }

    return fRet ;
}
double __stdcall CMath::GetLengthPntToLine(double _tPntX , double _tPntY , double _dA , double _dB)
{
    TDPoint tPnt ;
    tPnt.x = _tPntX ;
    tPnt.y = _tPntY ;
    return CMath::GetLengthPntToLine(tPnt , _dA , _dB) ;
}


//***** 최소 자승법은 버티컬라인일 경우.. 제대로 측정 되지 않는다.
//X에 대한 Y의 편차에 대한 해이기 때문에 라인이 일정 수준 이상으로 기울기가 올라가면 해를 찾을수 없다.
//http://darkpgmr.tistory.com/143 최소자승법 보완해야함.  기하학적으로 보완.직선 최단거리로 보완 해야함.
bool __stdcall CMath::GetLineABFromPntByLeastSqure(int _iPntCnt , TDPoint * _pPnt , double & _dA , double & _dB)
{
    if(_iPntCnt < 2) return false ; //2개 이상이여야지만 데이터가 나옴.

    double sx = 0.0, sy = 0.0, sxx = 0.0, sxy= 0.0;
    double ex = 0.0, ey = 0.0 ;

    for (int i = 0; i < _iPntCnt; ++i){
        sx += _pPnt[i].x;
        sy += _pPnt[i].y;

        sxx += (_pPnt[i].x * _pPnt[i].x) ;
        sxy += (_pPnt[i].x * _pPnt[i].y) ;
    }
    ex = sx/_iPntCnt ;
    ey = sy/_iPntCnt ;

    _dA = (sxy - _iPntCnt*ex*ey)/(sxx - _iPntCnt*ex*ex) ;
    _dB = (ey*sxx - ex*sxy)/(sxx - _iPntCnt*ex*ex);

    return true ;
}


//bool __stdcall CMath::GetLineABFromPntByLeastSqure(int _iPntCnt, const REAL x[], const REAL y[], REAL* m, REAL* b, REAL* r)
//bool __stdcall CMath::GetLineABFromPntByLeastSqure(int _iPntCnt , TDPoint * _pPnt , double & _dA , double & _dB)
//    {
//        double   sumx = 0.0;                        /* sum of x                      */
//        double   sumx2 = 0.0;                       /* sum of x**2                   */
//        double   sumxy = 0.0;                       /* sum of x * y                  */
//        double   sumy = 0.0;                        /* sum of y                      */
//        double   sumy2 = 0.0;                       /* sum of y**2                   */
//
//       for (int i=0;i<_iPntCnt;i++)   
//          {
//          sumx  += _pPnt[i].x;
//          sumx2 += _pPnt[i].x * _pPnt[i].x;
//          sumxy += _pPnt[i].x * _pPnt[i].y;
//          sumy  += _pPnt[i].y;
//          sumy2 += _pPnt[i].y * _pPnt[i].y; 
//          }
//
//       double denom = (_iPntCnt * sumx2 - sumx * sumx);
//       if (denom == 0) {
//           // singular matrix. can't solve the problem.
//           _dA = 0;
//           _dB = 0;
//           return false;
//       }
//
//       _dA = (_iPntCnt * sumxy  -  sumx * sumy) / denom;
//       _dB = (sumy * sumx2  -  sumx * sumxy) / denom;
//
//
//       // n = number of data points
//       // *b = output intercept
//       // *m  = output slope
//       // *r = output correlation coefficient (can be NULL if you don't want it)
//       // double r = (sumxy - sumx * sumy / _iPntCnt) /          /* compute correlation coeff     */
//       //         sqrt((sumx2 - sqr(sumx)/_iPntCnt) *
//       //         (sumy2 - sqr(sumy)/_iPntCnt));
//
//       return true;
//    }

//시작점에서 부터 해당각도와 길이만큼 떨어진 점.
void __stdcall CMath::GetPntFromPntByAngLen(double  _dX , double  _dY , //입력 시작점.
                                          double  _dAngle  ,         //입력 각도.
                                          double  _dLength ,         //입력 길이.
                                          double *_pX , double *_pY ) //출력 포인트.
{
    //double sss = cos(_dAngle*MATH_PI/180.0);
    *_pX = _dLength * cos(_dAngle*MATH_PI/180.0)  + _dX;
    *_pY =-_dLength * sin(_dAngle*MATH_PI/180.0)  + _dY;
}
void __stdcall CMath::GetPntFromPntByAngLen(double  _dX , double  _dY , //입력 시작점.
                                          double  _dAngle  ,         //입력 각도.
                                          double  _dLength ,         //입력 길이.
                                          int    *_pX , int *_pY ) //출력 포인트.
{
    CMath::GetPntFromPntByAngLen( _dX ,  _dY , _dAngle  ,
                                _dLength ,
                                _pX , _pY );
}

void __stdcall CMath::GetRotPntFromCntrPnt(double _dX     , double _dY     , double _dAng ,  //입력 x , 입력 y , 돌릴각도. , +가 반시계방향.
                                         double _dCntrX , double _dCntrY ,                //돌리는 기준점 x , y
                                         double *_pRetX , double *_pRetY )                //결과값 x,y
{
    double x = _dX - _dCntrX;
    double y = _dY - _dCntrY;
    double t = -_dAng ;

    *_pRetX = x*cos(t*MATH_PI/180.0) - y*sin(t*MATH_PI/180.0) ;
    *_pRetY = x*sin(t*MATH_PI/180.0) + y*cos(t*MATH_PI/180.0) ;

    *_pRetX += _dCntrX ;
    *_pRetY += _dCntrY ;

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
bool __stdcall CMath::GetCircleBy3Points(double  _dX1 , double  _dX2 , double  _dX3 ,
                                       double  _dY1 , double  _dY2 , double  _dY3 ,
                                       double *_pCx , double *_pCy , double *_pRad)
{
    double fX2nX1 = _dX2-_dX1 ;
    double fY2nY1 = _dY2-_dY1 ;
    double fX3nX1 = _dX3-_dX1 ;
    double fY3nY1 = _dY3-_dY1 ;
    double fY3nY2 = _dY3-_dY2 ;
    double fX3nX2 = _dX3-_dX2 ;

    double fX1pX2 = _dX1+_dX2 ;
    double fY1pY2 = _dY1+_dY2 ;
    double fX1pX3 = _dX1+_dX3 ;
    double fY1pY3 = _dY1+_dY3 ;

    double f1_2 = fX2nX1*fX1pX2 + fY2nY1*fY1pY2;
    double f1_3 = fX3nX1*fX1pX3 + fY3nY1*fY1pY3;
    double f123 = 2.*(fX2nX1*fY3nY2-fY2nY1*fX3nX2);

    if(f123==0.) return false;

    *_pCx=(fY3nY1*f1_2 - fY2nY1*f1_3)/f123;
    *_pCy=(fX2nX1*f1_3 - fX3nX1*f1_2)/f123;

    double fx = *_pCx - _dX1 ;
    double fy = *_pCy - _dY1 ;

    *_pRad = sqrt(fx * fx + fy * fy);

    return true ;
}

int __stdcall CMath::GetCircleBy3Points(TDPoint _Pnt1 , TDPoint _Pnt2 , TDPoint _Pnt3 ,
                                      double  *_pdCx , double  *_pdCy , double  *_pdRad)
{
    return CMath::GetCircleBy3Points( _Pnt1.x ,  _Pnt2.x ,  _Pnt3.x ,
                                    _Pnt1.y ,  _Pnt2.y ,  _Pnt3.y ,
                                    _pdCx   ,  _pdCy   ,  _pdRad  );

}

//중심좌표가 _dCntrX , _dCntrY 이고 반지름이 _dRadius 인 원의 공식에서 임의의 점(_pdX, _pdY)이 원 안에 들어가는지 확인.
//return : true  = 원 안에 들어가 있음.
//         false = 원 밖에 있음.
bool __stdcall CMath::GetCircleInPoint(double _dCntrX , double _dCntrY , double _dRadius , double _pdX , double _pdY)
{
    double _dLeng = sqrt((_pdX - _dCntrX) * (_pdX - _dCntrX) + (_pdY - _dCntrY) * (_pdY - _dCntrY));

    if (_dLeng  <= _dRadius) return true;

    return false;
}

//한개의 점 P1, P2 과 각도 _Ang를 가지고 기울기 *_dSlp 와 절편 *dInterCPT를 구하는 공식
//만약 각도가 0일 떄는 계산 오차 .
//bool __stdcall CMath::GetSlope(double _iX, double _iY, double _Ang, double *_dSlp, double *_dInterCPT )  //이함수 고쳤는데 쓰는곳 있으면 에러네기위해...체크해봐야함.
//{ //검증 안해봄.
//
//    //이거 왜 이렇게 했지? 일단 바꿈.. 밑에꺼 20150416
//    //if(_Ang == 0.0 || _Ang == 270.0 ) _Ang = ~(1<<(sizeof(double)*8-1)) ;//return false;
//    //
//    //*_dSlp = tan(_Ang);
//    //*_dInterCPT = (*_dSlp * _iX) + _iY;
//
////    if(_Ang == 0.0 || _Ang == 270.0 ){
////        *_dSlp = 0;
////        *_dInterCPT = _iY ;
////        return true ;
////    }
////    else if(_Ang == 90 || _Ang == 360){
////        *_dSlp = ~(1<<(sizeof(double)*8-1));
////
////
////    }
//
//
//
//
//    return true;
//}

//3포인트를 순서대로 이동했을때 CCW방향이면 1, 아니면 -1
int CMath::GetPntCCW(TDPoint _tPnt1, TDPoint _tPnt2, TDPoint _tPnt3)

{

    LONG dx1, dx2 ;
    LONG dy1, dy2 ;

    dx1 = _tPnt2.x - _tPnt1.x ; dx2 = _tPnt3.x - _tPnt1.x ;
    dy1 = _tPnt2.y - _tPnt1.y ; dy2 = _tPnt3.y - _tPnt1.y ;

    /* This is basically a slope comparison: we don't do divisions because

     * of divide by zero possibilities with pure horizontal and pure
     * vertical lines.
     */

    return ((dx1 * dy2 > dy1 * dx2) ? 1 : -1) ;

}



//2개의 선분이 교차되면 true
bool CMath::GetSegmentIntersect(TDPoint _tPntStt1, TDPoint _tPntEnd1, TDPoint _tPntStt2, TDPoint _tPntEnd2)
{

    return (((CMath::GetPntCCW(_tPntStt1, _tPntEnd1, _tPntStt2) * CMath::GetPntCCW(_tPntStt1, _tPntEnd1, _tPntEnd2)) <= 0)&&
            ((CMath::GetPntCCW(_tPntStt2, _tPntEnd2, _tPntStt1) * CMath::GetPntCCW(_tPntStt2, _tPntEnd2, _tPntEnd1)  <= 0) )) ;

}

//포인트들의 최외곽 직사각형.
bool  CMath::GetOutRectFromPnts(TDPoint *_pPoints, int _iPntCnt, TDRect * _tRect)
{

    // If a bounding rect has not been passed in, calculate it

    int  iXMin, iXMax, iYMin, iYMax ;
    int  i ;

    iXMin = iYMin = INT_MAX ;
    iXMax = iYMax = -INT_MAX ;

    for (i=0 ; i < _iPntCnt ; i++){
        if (_pPoints[i].x < iXMin)iXMin = _pPoints[i].x ;
        if (_pPoints[i].x > iXMax)iXMax = _pPoints[i].x ;
        if (_pPoints[i].y < iYMin)iYMin = _pPoints[i].y ;
        if (_pPoints[i].y > iYMax)iYMax = _pPoints[i].y ;
    }
    _tRect->left   = iXMin ;
    _tRect->top    = iYMin ;
    _tRect->right  = iXMax ;
    _tRect->bottom = iYMax ;

    return true ;

}





//포인트가 구성하는 가장 작은 사각형 안에 해당 포인트가 있는지 확인.
bool  CMath::IsPntInPolygonOutRect(TDPoint *_pPoints, int _iPntCnt, TDPoint _tPnt)
{

    TDRect tOutRect ;
    // If a bounding rect has not been passed in, calculate it

    int   iXMin, iXMax, iYMin, iYMax ;
    TDPoint *ppt ;
    int  i ;

    iXMin = iYMin = INT_MAX ;
    iXMax = iYMax = -INT_MAX ;

    for (i=0, ppt = _pPoints ; i < _iPntCnt ; i++, ppt++){
        if (ppt->x < iXMin)iXMin = ppt->x ;
        if (ppt->x > iXMax)iXMax = ppt->x ;
        if (ppt->y < iYMin)iYMin = ppt->y ;
        if (ppt->y > iYMax)iYMax = ppt->y ;
    }
    tOutRect.left   = iXMin ;
    tOutRect.top    = iYMin ;
    tOutRect.right  = iXMax ;
    tOutRect.bottom = iYMax ;

    return (tOutRect.left <= _tPnt.x && _tPnt.x <= tOutRect.right  &&
            tOutRect.top  <= _tPnt.y && _tPnt.y <= tOutRect.bottom );
}





bool CMath::IsPntInPolygon(TDPoint *_pPoints, int _iPntCnt, TDPoint _tPnt)
{

    TDRect   r ;
    TDPoint  *ppt ;
    int   i ;
    TDPoint  pt1, pt2 ;
    int   wnumintsct = 0 ;

    if (!CMath::IsPntInPolygonOutRect(_pPoints,_iPntCnt,_tPnt)) return false ;

    pt1 = pt2 = _tPnt ;
    pt2.x = r.right + 50 ;

    // Now go through each of the lines in the polygon and see if it
    // intersects
    for (i = 0, ppt = _pPoints ; i < _iPntCnt-1 ; i++, ppt++)
    {
       if (CMath::GetSegmentIntersect(_tPnt, pt2, *ppt, *(ppt+1)))
          wnumintsct++ ;
    }

    // And the last line
    if (CMath::GetSegmentIntersect(_tPnt, pt2, *ppt, *_pPoints))
       wnumintsct++ ;

    return (wnumintsct&1) ;

}


//점이 네모 안에 있는지.
bool __stdcall CMath::IsPntInRect(double _dX1 , double _dY1 , double _dX2 , double _dY2 ,
                                double _dX  , double _dY )
{
    double fLeft   = _dX1 < _dX2 ? _dX1 : _dX2 ;
    double fRight  = _dX1 < _dX2 ? _dX2 : _dX1 ;
    double fTop    = _dY1 < _dY2 ? _dY1 : _dY2 ;
    double fBottom = _dY1 < _dY2 ? _dY2 : _dY1 ;

    if(fLeft <= _dX && _dX <= fRight  &&
       fTop  <= _dY && _dY <= fBottom ) return true ;

    return false ;
}

//한개의 직선(pX1, pX2, pY1, pY2)를 가지고 원과의 접점갯수와 거리.
//return 0 = 접점이 없는 경우
//       1 = 접점이 1점인 경우
//       2 = 접점이 2점인 경우
int  __stdcall CMath::GetCircleLineDistance(double  _dX1      , double _dY1      , double _dX2    , double _dY2 , //직선 점2개.
                                          double  _dRadius  , double _dCntrX   , double _dCntrY ,  //원의 반지름 , 중심점.
                                          double *_dDist)
{
    double m = (_dY2-_dY1) / (_dX2-_dX1);
    double a = m;
    double b = -1;
    double c = -m*_dX1+_dY1;
    int   iRet ;

    //점(_dCntrX   , _dCntrY)과 직선(a*x+b*y+c=0) 사이의 최단 거리 d=절대값(a*x1+b*y1+c)/루트(이승(a)+이승(b))) 이다.
    double fTemp = (a*_dCntrX + b*_dCntrY +c)*(a*_dCntrX + b*_dCntrY +c) ;
    double d = fabs(fTemp) / sqrt((a*a) + (b*b));
         if (d < _dRadius) {iRet = 2;} //printf("교점은 2개입니다");
    else if (d ==_dRadius) {iRet = 1;} //printf("교점은 1개입니다");
    else if (d > _dRadius) {iRet = 0;} //printf("교점은 없습니다" );
    *_dDist = d ;
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
int __stdcall CMath::GetCircleLineIntersection(double _dX1      , double _dY1      , double _dX2    , double _dY2 ,
                                             double _dRadius  , double _dCntrX   , double _dCntrY ,
                                             TDPoint *_pPnt1 , TDPoint *_pPnt2 )
{
    double dx  ,dy , a , b , c , det , t ;

    _pPnt1 -> x = _pPnt1 -> y =_pPnt2 -> x =_pPnt2 -> y =0 ;

    dx = _dX2 - _dX1 ;
    dy = _dY2 - _dY1 ;

    a = dx * dx + dy * dy ;
    b = 2 * (dx * (_dX1 - _dCntrX) + dy * (_dY1 - _dCntrY)) ;
    c = (_dX1 - _dCntrX) * (_dX1 - _dCntrX) + (_dY1 - _dCntrY) * (_dY1 - _dCntrY) - _dRadius * _dRadius ;

    det = b * b - 4 * a * c ; //해의 갯수 판별식.

    if(a==0 || det < 0) {//교점이 없는 경우 직선의 최단거리가 원의 반지름 보다 크다.
        return 0 ;
    }
    else if (det == 0) { //직선이 원의 접선으로 있음 교점이 1개 있음.
        t = -b / (2*a) ;
        _pPnt1 -> x = _dX1 + t*dx ;
        _pPnt1 -> y = _dY1 + t*dy ;
        return 1 ;
    }
    else { //직선이 원을 관통하고 있어서 교점 2개.
        t = (-b - sqrt(det)) / (2 * a);
        _pPnt1 -> x = _dX1 + t*dx ;
        _pPnt1 -> y = _dY1 + t*dy ;

        t = (-b + sqrt(det)) / (2 * a);
        _pPnt2 -> x = _dX1 + t*dx ;
        _pPnt2 -> y = _dY1 + t*dy ;
        return 2 ;
    }


}



//원과 선분의 교점.
//한개의 선분(pX1, pX2, pY1, pY2)를 가지고 원과의 접점을 구하는 공식
//return 0 = 접점이 없는 경우
//       1 = 접점이 1점인 경우
//       2 = 접점이 2점인 경우
int __stdcall CMath::GetCircleSegmentIntersection(double _dX1      , double _dY1      , double _dX2    , double _dY2 ,
                                                double _dRadius  , double _dCntrX   , double _dCntrY ,
                                                TDPoint *_pPnt1 , TDPoint *_pPnt2 )
{
    int iRet = CMath::GetCircleLineIntersection( _dX1     , _dY1    , _dX2    , _dY2 ,
                                               _dRadius , _dCntrX , _dCntrY ,
                                               _pPnt1   , _pPnt2 );

         if(iRet == 0) {return iRet ; }
    else if(iRet == 1) {
        if(!CMath::IsPntInRect(_dX1 , _dY1 , _dX2 , _dY2 , _pPnt1 -> x , _pPnt1 -> y )) { //1번 접점이 시작점과 끝점 으로 그린 사각형 안에 있으면 1번접점이 선분안에 있음.
            _pPnt1 -> x = _pPnt1 -> y = 0 ;
            return 0 ;
        }
    }
    else if(iRet == 2) {
        if(!CMath::IsPntInRect(_dX1 , _dY1 , _dX2 , _dY2 , _pPnt1 -> x , _pPnt1 -> y )) { //1번 접점이 시작점과 끝점 으로 그린 사각형 안에 있으면 1번접점이 선분안에 있음.
            _pPnt1 -> x = _pPnt1 -> y = 0 ;
            iRet-- ;
        }
        if(!CMath::IsPntInRect(_dX1 , _dY1 , _dX2 , _dY2 , _pPnt2 -> x , _pPnt2 -> y )) { //2번 접점이 시작점과 끝점 으로 그린 사각형 안에 있으면 2번접점이 선분안에 있음.
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

    return 0 ;
}


//중점에서 360 도를 _iSampeCnt 만큼으로 분활하여 의 직선 기울기,y절편 구하기.
bool __stdcall CMath::GetPolarLine(int _iCx , int _iCy , int _iSampleCnt , double * _pA , double * _pB)
{
    double fAngleUnit = 360 / _iSampleCnt ;

    for(int i = 0 ; i < _iSampleCnt ; i++) {
        if( i * fAngleUnit == 90){
            _pA[i] = FLT_MAX ;
        }
        else if(i * fAngleUnit == 270) {
            _pA[i] = -FLT_MAX ;
        }
        else {
            _pA[i] = tan(i*fAngleUnit /180*MATH_PI);
        }
        _pB[i] = _pA[i] * _iCx - _iCy ;
    }
    return true ;
}

//_iPntCnt개의 점들을 이용하여 Racsac알고리즘으로 _iGap안에 들어 오는 점들이 가장 많은 원을 찾는다.

//특정 포인트가 다각형 내부에 존재 하는지 여부 리턴.
/* 속도 느려서 교체함.
bool CMath::IsPntInPolygon(TDPoint *Points, double _dX , double _dY , int _iPntCnt){
    TDPoint P1, P2;
    bool bRet = false;

    P1 = Points[0];
    P2 = Points[_iPntCnt - 1];

    for(int i = 1; i <= _iPntCnt; i++){
        bRet ^= (((_dY >= P1.y) ^ (_dY >= P2.y)) && (_dX - P1.x < MulDiv(P2.x - P1.x, _dY - P1.y, P2.y - P1.y)));
        P2 = P1;
        P1 = Points[i % _iPntCnt];
    }
    return bRet;
}
*/



//반올림
double __stdcall CMath::RoundOff(double _dVal , int _iUnderPoint)
{
    return ((int)(_dVal * pow(10 , _iUnderPoint) + 0.5))/(double)pow(10,_iUnderPoint);
}


//여기 스코어 이상한거 수정.
bool __stdcall CMath::GetCircleRansac(TDPoint *_pPnts  , int    _iPntCnt , double  _dGap    ,
                          double   *_pCntrX , double *_pCntrY  , double *_pRadius , double *_pScore)
{
    DWORD dwTime =GetTickCount();
    *_pCntrX = *_pCntrY = *_pRadius = *_pScore = 0.0 ;


    const int iMinSample = 3;
    if (_iPntCnt<iMinSample) {/*sprintf(pErr,"_iPntCnt<iMinSample");*/ return false; }

    //std::vector<TDPoint> consensus(_iPntCnt);
    //std::vector<TDPoint> Maxconsensus(_iPntCnt);

    int    iGapInCnt    ; //갭 안쪽으로 들어온 숫자.
    int    iGapInCntMax ; //최대 숫자.
    double  fX , fY ;
    double  fDev ;
    double  fDevSum ;
    double  fDevMinSum ;
    double  fCntrPntX , fCntrPntY;
    double  fRadius ;

    TDPoint RetPnt[iMinSample] ;

    fDevMinSum    = FLT_MAX ; //~(1<<(sizeof(double)*8-1)) 20150519 수정.
    iGapInCntMax = 0   ;
    int a = 0 , b = 0 , c = 0 ;

    for(a = 0 ; a < _iPntCnt ; a++) {
        b=a+1;
        while(b < _iPntCnt) {
            c=b+1;
            while(c < _iPntCnt) {
                if (!CMath::GetCircleBy3Points(_pPnts[a], _pPnts[b], _pPnts[c], &fCntrPntX, &fCntrPntY, &fRadius)) {
                    c++;
                    continue;
                }

                iGapInCnt = 0 ;
                fDevSum   = 0 ;
                for (int k=0; k<_iPntCnt; k++) {
                    fX  = _pPnts[k].x-fCntrPntX;
                    fY  = _pPnts[k].y-fCntrPntY;
                    fDev = sqrt((fX*fX + fY*fY)) - fRadius;
                    if (fabs(fDev) <= _dGap) {
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

    if (!CMath::GetCircleBy3Points(RetPnt[0], RetPnt[1], RetPnt[2] , &fCntrPntX, &fCntrPntY, &fRadius)) {/*sprintf(pErr,"GetCircleBy3Points Failed");*/ return false; }

    *_pCntrX  = fCntrPntX ;
    *_pCntrY  = fCntrPntY ;
    *_pRadius = fRadius   ;
    *_pScore  = CMath::RoundOff(iGapInCntMax * 100 / (double)_iPntCnt , 2) ;

    dwTime = GetTickCount()- dwTime ;
    return true ;
}


//Circle CircleFitByTaubin (Data& data)
///*  
//      Circle fit to a given set of data points (in 2D)
//      
//      This is an algebraic fit, due to Taubin, based on the journal article
//     
//      G. Taubin, "Estimation Of Planar Curves, Surfaces And Nonplanar
//                  Space Curves Defined By Implicit Equations, With 
//                  Applications To Edge And Range Image Segmentation",
//                  IEEE Trans. PAMI, Vol. 13, pages 1115-1138, (1991)
//
//      Input:  data     - the class of data (contains the given points):
//		
//	      data.n   - the number of data points
//	      data.X[] - the array of X-coordinates
//	      data.Y[] - the array of Y-coordinates
//     
//     Output:	       
//               circle - parameters of the fitting circle:
//		        
//	       circle.a - the X-coordinate of the center of the fitting circle
//	       circle.b - the Y-coordinate of the center of the fitting circle
// 	       circle.r - the radius of the fitting circle
// 	       circle.s - the root mean square error (the estimate of sigma)
// 	       circle.j - the total number of iterations
//             
//     The method is based on the minimization of the function
//     
//                  sum [(x-a)^2 + (y-b)^2 - R^2]^2
//              F = -------------------------------
//                      sum [(x-a)^2 + (y-b)^2]
//                 
//     This method is more balanced than the simple Kasa fit.
//        
//     It works well whether data points are sampled along an entire circle or
//     along a small arc. 
//     
//     It still has a small bias and its statistical accuracy is slightly
//     lower than that of the geometric fit (minimizing geometric distances),
//     but slightly higher than that of the very similar Pratt fit. 
//     Besides, the Taubin fit is slightly simpler than the Pratt fit
//     
//     It provides a very good initial guess for a subsequent geometric fit. 
//     
//       Nikolai Chernov  (September 2012)
//
//*/
//{
//    int i,iter,IterMAX=99;
//    
//    reals Xi,Yi,Zi;
//    reals Mz,Mxy,Mxx,Myy,Mxz,Myz,Mzz,Cov_xy,Var_z;
//    reals A0,A1,A2,A22,A3,A33;
//    reals Dy,xnew,x,ynew,y;
//    reals DET,Xcenter,Ycenter;
//    
//    Circle circle;
//
//    data.means();   // Compute x- and y- sample means (via a function in the class "data")
//
////     computing moments 
//
//	Mxx=Myy=Mxy=Mxz=Myz=Mzz=0.;
//    
//    for (i=0; i<data.n; i++)
//    {
//        Xi = data.X[i] - data.meanX;   //  centered x-coordinates
//        Yi = data.Y[i] - data.meanY;   //  centered y-coordinates
//        Zi = Xi*Xi + Yi*Yi;
//        
//        Mxy += Xi*Yi;
//        Mxx += Xi*Xi;
//        Myy += Yi*Yi;
//        Mxz += Xi*Zi;
//        Myz += Yi*Zi;
//        Mzz += Zi*Zi;
//    }
//    Mxx /= data.n;
//    Myy /= data.n;
//    Mxy /= data.n;
//    Mxz /= data.n;
//    Myz /= data.n;
//    Mzz /= data.n;
//    
////      computing coefficients of the characteristic polynomial
//    
//    Mz = Mxx + Myy;
//    Cov_xy = Mxx*Myy - Mxy*Mxy;
//    Var_z = Mzz - Mz*Mz;
//    A3 = Four*Mz;
//    A2 = -Three*Mz*Mz - Mzz;
//    A1 = Var_z*Mz + Four*Cov_xy*Mz - Mxz*Mxz - Myz*Myz;
//    A0 = Mxz*(Mxz*Myy - Myz*Mxy) + Myz*(Myz*Mxx - Mxz*Mxy) - Var_z*Cov_xy;
//    A22 = A2 + A2;
//    A33 = A3 + A3 + A3;
//
////    finding the root of the characteristic polynomial
////    using Newton's method starting at x=0  
////     (it is guaranteed to converge to the right root)
//    
//    for (x=0.,y=A0,iter=0; iter<IterMAX; iter++)  // usually, 4-6 iterations are enough
//    {
//    	    Dy = A1 + x*(A22 + A33*x);
//        xnew = x - y/Dy;
//        if ((xnew == x)||(!isfinite(xnew))) break;
//        ynew = A0 + xnew*(A1 + xnew*(A2 + xnew*A3));
//        if (abs(ynew)>=abs(y))  break;
//        x = xnew;  y = ynew;
//    }
//     
////       computing paramters of the fitting circle
//    
//    DET = x*x - x*Mz + Cov_xy;
//    Xcenter = (Mxz*(Myy - x) - Myz*Mxy)/DET/Two;
//    Ycenter = (Myz*(Mxx - x) - Mxz*Mxy)/DET/Two;
//
////       assembling the output
//
//    circle.a = Xcenter + data.meanX;
//    circle.b = Ycenter + data.meanY;
//    circle.r = sqrt(Xcenter*Xcenter + Ycenter*Ycenter + Mz);
//    circle.s = Sigma(data,circle);
//    circle.i = 0;
//    circle.j = iter;  //  return the number of iterations, too
//    
//    return circle;
//}


//_iMargin이 -이면 작은 다각형 , +면 큰다각형 리턴.
void _stdcall CMath::GetMarginPntsFromPnts(TDPoint * _aSrcPoints , TDPoint * _aDstPoints , int _iPntsCnt , int _iMargin)
{

    if(_iPntsCnt < 3)return ;


    TDPoint tPrePnt ;
    TDPoint tCrtPnt ;
    TDPoint tNxtPnt ;

    TDPoint tPreSttPntMargin ;
    TDPoint tPreEndPntMargin ;

    TDPoint tNxtSttPntMargin ;
    TDPoint tNxtEndPntMargin ;


    double  dPreLineAngle ;
    double  dNxtLineAngle ;

    for(int i = 0 ; i < _iPntsCnt ; i++) {
        tPrePnt = _aSrcPoints[(i+_iPntsCnt-1)%_iPntsCnt] ;
        tCrtPnt = _aSrcPoints[ i                       ] ;
        tNxtPnt = _aSrcPoints[(i+_iPntsCnt+1)%_iPntsCnt] ;
        dPreLineAngle = CMath::GetLineAngle(tPrePnt.x , tPrePnt.y , tCrtPnt.x , tCrtPnt.y);
        CMath::GetPntFromPntByAngLen( tPrePnt.x , tPrePnt.y ,                      //입력 시작점.
                                    dPreLineAngle+90.0    ,                      //입력 각도.
                                    _iMargin              ,                      //입력 길이.
                                    &tPreSttPntMargin.x , &tPreSttPntMargin.y ); //출력 포인트.

        CMath::GetPntFromPntByAngLen( tCrtPnt.x , tCrtPnt.y ,                      //입력 시작점.
                                    dPreLineAngle+90.0    ,                      //입력 각도.
                                    _iMargin              ,                      //입력 길이.
                                    &tPreEndPntMargin.x , &tPreEndPntMargin.y ); //출력 포인트.

        //여기부터.
        dNxtLineAngle = CMath::GetLineAngle(tCrtPnt.x , tCrtPnt.y , tNxtPnt.x , tNxtPnt.y);
        CMath::GetPntFromPntByAngLen( tCrtPnt.x , tCrtPnt.y ,                      //입력 시작점.
                                    dNxtLineAngle+90.0    ,                      //입력 각도.
                                    _iMargin              ,                      //입력 길이.
                                    &tNxtSttPntMargin.x , &tNxtSttPntMargin.y ); //출력 포인트.

        CMath::GetPntFromPntByAngLen( tNxtPnt.x , tNxtPnt.y ,                      //입력 시작점.
                                    dNxtLineAngle+90.0    ,                      //입력 각도.
                                    _iMargin              ,                      //입력 길이.
                                    &tNxtEndPntMargin.x , &tNxtEndPntMargin.y ); //출력 포인트.

        CMath::GetLineCrossPnt(tPreSttPntMargin, tPreEndPntMargin, tNxtSttPntMargin, tNxtEndPntMargin , _aDstPoints[i]); //한번 확인하기.

    }
}





//기타 등등.
//======================================================================
//입력한 갯수만큼의 double형의 평균을 구한다.
double __stdcall CMath::Average(int _iCont, double *_pPnt)
{
    double dAvge ;
    double dSum  ;

    for(int i = 0; i <= _iCont ; i++) {
        dSum += _pPnt[i];
    }

    dAvge = dSum / (double)_iCont;

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




