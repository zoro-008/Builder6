//---------------------------------------------------------------------------
#ifndef VOptnManH
#define VOptnManH

#include "SVInterface.h"


//---------------------------------------------------------------------------
//�˻�� �ʿ��� �Ķ�.
struct CVDevPara {
    OCV_Para PackageOcv ;
    TSD_Para PackageTsd ;

};

//Vision Device Info.
struct CVDevOptn {
    int    iCountX      ; //������ ��ġ�� ����. ZIG��ġ������ 90�� ������ X,Y�� �ݴ�� �ٲ��.
    int    iCountY      ; //������ RAIL ����. �� 90�� ������ ���� ����.
//sun
    double dPackageOriSinc  ; //������ �˻�ÿ� ���������̼� ���� ��ũ��.
    double dPackagePxMinPer ; //�ȼ� ���� �ۼ�Ʈ.
    bool   bPackageOverUnder; //�̻�,����.


};

struct CVCmnOptn {
    int   iPackageImageSave ; //�̹��� ���� ����.
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
