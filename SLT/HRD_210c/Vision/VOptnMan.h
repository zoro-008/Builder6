//---------------------------------------------------------------------------
#ifndef VOptnManH
#define VOptnManH

#include <vcl.h>

//---------------------------------------------------------------------------


//Vision Device Info.
struct CVDevOptn {
    //오리엔테이션.
    float fOriSinc      ; //매치율

    //웨이퍼 칩 얼라인.
    double fWfrWidth     ; //칩의 넓이 스펙.
    double fWfrHeight    ; //칩의 높이 스펙.
    double fWfrGap       ; //칩의 넓이 높이 스펙 오프셑 넓이 높이설정값의 +- Gap안에 들어 오면 Pass
    double fWfrAngle     ; //칩의 각변간의 각도 편차.

    int    iWfrCCPxCnt   ; //크랙 칩 확인 픽셀 갯수.
    int    iWfrStdPxCnt  ; //웨이퍼 맵모드 스타트 기준자제 픽셀 카운트.
    int    iWfrFcpPxCnt  ; //웨이퍼 페일칩 픽셀 카운트

    //Bonding before.
    int    iRlbPkgPxAvr  ; //칩이 있는지 유무 평균값.

    double fRlbOffsetX   ; //얼라인 XY값에서 오프셑 만큼 떨어진곳을 핸들러에게 보내 준다. (본딩 포인트)
    double fRlbOffsetY   ; //얼라인 XY값에서 오프셑 만큼 떨어진곳을 핸들러에게 보내 준다. (본딩 포인트)

    int   iRlbPxCnt     ; //본딩된곳 픽셀값 평균 스펙.


    //Bonding After.
    double fRlaWidth     ; //칩의 넓이.
    double fRlaHeight    ; //칩의 높이.
    double fRlaGap       ; //넓이 높이 갭.
    double fRlaAngle     ; //각도 칩의 얼라인 먼저치고 거기서 기준값을 가지고 칩의 각도를 본다.
    double fRlaAngleGap  ; //각도 갭
    double fRlaOffsetX   ; //칩의 붙은 위치 X (본딩전 검사 스펙과 동일해야함.)
    double fRlaOffsetY   ; //칩의 붙은 위치 Y (본딩전 검사 스펙과 동일해야함.)
    double fRlaOffsetGap ; //칩의 붙은 위치 Gap.

    //웨이펴 오리
    float fWfoSinc      ; //매치율

} ;

struct CVCmnOptn {
    bool  bWfrFlipX     ; //핸들러에 보내주는 값 X 플립.
    bool  bWfrFlipY     ; //핸들러에 보내주는 값 Y 플립.
    bool  bWfrFlipT     ; //핸들러에 보내주는 값 T 플립.

    bool  bRlbFlipX     ; //핸들러에 보내주는 값 X플립.
    bool  bRlbFlipY     ; //핸들러에 보내주는 값 Y플립.
    bool  bRlbFlipT     ; //핸들러에 보내주는 값 T플립.
    bool  bRlbNotUseAng ; //Angle값 항상0.0으로.

    bool  bWfoFlipX     ;
    bool  bWfoFlipY     ;

    int   iOriImageSave ;
    int   iWfrImageSave ;
    int   iRlbImageSave ;
    int   iRlaImageSave ;
    int   iWfoImageSave ;


};


//---------------------------------------------------------------------------
class CVOptnMan
{
    public:
        CVOptnMan();
        virtual ~CVOptnMan();

    public:
        CVDevOptn   VDevOptn  ;
        CVCmnOptn   VCmnOptn  ;


        void LoadJobFile(bool _bLoad );

        void LoadVDevOptn(bool _bLoad );
        void LoadVCmnOptn(bool _bLoad );
};

//---------------------------------------------------------------------------
extern CVOptnMan VOM;

//---------------------------------------------------------------------------
#endif
