//---------------------------------------------------------------------------
#ifndef VOptnManH
#define VOptnManH

#include <vcl.h>

//---------------------------------------------------------------------------


//Vision Device Info.
struct CVDevOptn {

    //웨이퍼 칩 얼라인.
    double fWfrWidth     ; //칩의 넓이 스펙.
    double fWfrHeight    ; //칩의 높이 스펙.
    double fWfrGap       ; //칩의 넓이 높이 스펙 오프셑 넓이 높이설정값의 +- Gap안에 들어 오면 Pass
    double fWfrAngle     ; //칩의 각변간의 각도 편차.
    
    int    iWfrCCPxCnt   ; //크랙 칩 확인 픽셀 갯수.

    float  fWfrOriSinc   ; //웨이퍼 오리진 싱크.

    bool   bWfrIgnRctInsp; // 사각형 검사 스깁

    //웨이펴 오리
    float  fWfoOriSinc   ; //매치율

    bool   bWfoIgnRctInsp; //사각형 검사 스킵

    //웨이퍼 팬.
    int    iWfpPxCnt     ; //픽셀 카운트.
    int    iWfpPxHigh    ; //픽셀 밝기.  0~ iWfpPxHigh의 사이의 값들을 카운트 한다.

    //캘리브레이션.


    //엔드 비젼.
    int    iEndExposure ;  //엔드 비젼 익스포져.


} ;

struct CVCmnOptn {      // 이거 비져블 안보이게 해 놓고 검사.. 할 떄 강제적으로 떄려 놓게 하기 ... 수정 할 일이 없으니까 손대지 못하게 
    bool  bWfrFlipX     ; //핸들러에 보내주는 값 X 플립.
    bool  bWfrFlipY     ; //핸들러에 보내주는 값 Y 플립.
    bool  bWfrFlipT     ; //핸들러에 보내주는 값 T 플립.

    bool  bWfoFlipX     ; //핸들러에 보내주는 값 X 플립.
    bool  bWfoFlipY     ; //핸들러에 보내주는 값 Y 플립.
    bool  bWfoFlipT     ;

    bool  bWfpFlipX     ; //핸들러에 보내주는 값 X 플립.
    bool  bWfpFlipY     ; //핸들러에 보내주는 값 Y 플립.



    int   iWfrImageSave ; //이미지 저장 관련.
    int   iWfoImageSave ; //
    int   iWfpImageSave ; //
    int   iCalImageSave ; //


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
