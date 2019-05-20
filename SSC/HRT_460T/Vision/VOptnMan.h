//---------------------------------------------------------------------------
#ifndef VOptnManH
#define VOptnManH

#include "SVInterface.h"


//---------------------------------------------------------------------------
//�˻�� �ʿ��� �Ķ�.
struct CVDevPara {
    OCV_Para OriOcv ;
};

//Vision Device Info.
struct CVDevOptn {
//sun
    double dOriOriSinc  ; //�˻�ÿ� ���������̼� ���� ��ũ��.

};

struct CVCmnOptn {
    int   iOriImageSave ; //�̹��� ���� ����.
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
