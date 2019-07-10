//---------------------------------------------------------------------------
#ifndef VOptnManH
#define VOptnManH

#include "SVInterface.h"


//---------------------------------------------------------------------------
//검사시 필요한 파라.
struct CVDevPara {
    OCV_Para OriOcv ;
};

//Vision Device Info.
struct CVDevOptn {
//sun
    double dOriOriSinc  ; //검사시에 오리엔테이션 최저 싱크율.

};

struct CVCmnOptn {
    int   iOriImageSave ; //이미지 저장 관련.
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
