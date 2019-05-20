//---------------------------------------------------------------------------
#ifndef VOptnManH
#define VOptnManH

#include "SVInterface.h"


//---------------------------------------------------------------------------
//검사시 필요한 파라.
struct CVDevPara {
    OCV_Para PackageOcv ;
    TSD_Para PackageTsd ;

};

//Vision Device Info.
struct CVDevOptn {
    int    iCountX      ; //자제의 핏치와 갯수. ZIG위치에서는 90도 돌려서 X,Y가 반대로 바뀐다.
    int    iCountY      ; //기준은 RAIL 기준. 즉 90도 돌리고 난후 기준.
//sun
    double dPackageOriSinc  ; //에폭시 검사시에 오리엔테이션 최저 싱크율.
    double dPackagePxMinPer ; //픽셀 비율 퍼센트.
    bool   bPackageOverUnder; //이상,이하.


};

struct CVCmnOptn {
    int   iPackageImageSave ; //이미지 저장 관련.
};


//---------------------------------------------------------------------------
 class CVOptnMan
 {
     public:
         CVOptnMan();
         virtual ~CVOptnMan();

     public:
         CVDevPara   VDevPara  ;
         CVDevOptn   VDevOptn  ;
         CVCmnOptn   VCmnOptn  ;
 
 
         void LoadJobFile (bool _bLoad );
 
         void LoadVDevPara(bool _bLoad );
         void LoadVDevOptn(bool _bLoad );
         void LoadVCmnOptn(bool _bLoad );
 };

 //---------------------------------------------------------------------------
 extern CVOptnMan VOM;

//---------------------------------------------------------------------------
#endif
