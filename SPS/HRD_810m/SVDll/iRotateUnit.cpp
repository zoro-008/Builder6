//---------------------------------------------------------------------------


#pragma hdrstop

#include "iRotateUnit.h"
#include <math.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

SRotate::SRotate()
{

}

SRotate::~SRotate()
{

}

SRotate::OnRotate(CImage * image , TRect rect , float rotationAngle)
{
    //float TempTime = GetTickTime() ;
    image -> SetRect(&rect);
    
    BYTE  *pRotationImg;
    BYTE  newValue;
    int   height = rect.Height() ;
    int   width  = rect.Width()  ;
    int   center_r = rect.top  + rect.Height() / 2 ;
    int   center_c = rect.left + rect.Width () / 2 ;
    int   where;
    int   r,c;//타겟 이미지 좌표
    float r_orgr,r_orgc;//원 이미지 상의 해당 좌표 (실수값)
    int   i_orgr,i_orgc;//원 이미지 상의 해당 좌표 (정수값)
    float sr,sc;// 예 1.24-1=0.24
    float I1,I2,I3,I4;
    float cosAngle,sinAngle;
    float rotationAngleRad=(float)(rotationAngle*3.14159265/180);//angle->radian으로 변환

    //Rotation Image를 위한 동적 메모리 할당
    pRotationImg=new BYTE[height*width];

    for(r = rect.top ; r < rect.top + height ; r++) {
        for(c = rect.left ; c < rect.left + width ; c++) {
            cosAngle=(float)cos(rotationAngleRad);
            sinAngle=(float)sin(rotationAngleRad);
            //회전전의 원 이미지상의 좌표 구함
            r_orgr =-sinAngle * (c-center_c) + cosAngle * (r-center_r) + center_r;
            r_orgc = cosAngle * (c-center_c) + sinAngle * (r-center_r) + center_c;
            i_orgr = floor(r_orgr);//예: floor(2.8)=2.0
            i_orgc = floor(r_orgc);
            sr=r_orgr-i_orgr;
            sc=r_orgc-i_orgc;
            //범위 조사
            //원이미지의 범위를 벗어나는 경우 0값 할당

            if(i_orgr<0 || i_orgr >= image -> GetHeight() || i_orgc<0 || i_orgc >= image -> GetWidth()) {
                where=(r - rect.top) *width+(c - rect.left);
                pRotationImg[where]=0;
            }
            //원 이미지의 범위 내에 존재 이중 선형 보간 수행
            else {
                I1=(float)image -> GetPixel((i_orgc  ),(i_orgr  )) ;//(org_r,org_c)
                I2=(float)image -> GetPixel((i_orgc+1),(i_orgr  )) ;//(org_r,org_c+1)
                I3=(float)image -> GetPixel((i_orgc+1),(i_orgr+1)) ;//(org_r+1,org_c+1)
                I4=(float)image -> GetPixel((i_orgc  ),(i_orgr+1)) ;//(org_r+1,org_c)

                //이중 선형 보간을 통한 새로운 밝기값 계산
                newValue=(BYTE)(I1*(1-sc)*(1-sr)+I2*sc*(1-sr)+I3*sc*sr+I4*(1-sc)*sr);
                where=(r - rect.top)*width+(c - rect.left);
                pRotationImg[where]=newValue;
            }
        }
    }

    //결과 이미지로 영상 복사
    for(r = rect.top ; r < rect.top + height ; r++) {
        for(c = rect.left ; c < rect.left + width ; c++) {
            image -> SetPixel(c,r,pRotationImg[(r - rect.top)*width+(c - rect.left)]) ;
        }
    }

    //동적 할당 메모리 해제
    delete [] pRotationImg;
    //Rslt.fInspectionTime = GetTickTime() - TempTime ;
}