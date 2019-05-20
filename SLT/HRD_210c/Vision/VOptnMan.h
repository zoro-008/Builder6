//---------------------------------------------------------------------------
#ifndef VOptnManH
#define VOptnManH

#include <vcl.h>

//---------------------------------------------------------------------------


//Vision Device Info.
struct CVDevOptn {
    //���������̼�.
    float fOriSinc      ; //��ġ��

    //������ Ĩ �����.
    double fWfrWidth     ; //Ĩ�� ���� ����.
    double fWfrHeight    ; //Ĩ�� ���� ����.
    double fWfrGap       ; //Ĩ�� ���� ���� ���� �����V ���� ���̼������� +- Gap�ȿ� ��� ���� Pass
    double fWfrAngle     ; //Ĩ�� �������� ���� ����.

    int    iWfrCCPxCnt   ; //ũ�� Ĩ Ȯ�� �ȼ� ����.
    int    iWfrStdPxCnt  ; //������ �ʸ�� ��ŸƮ �������� �ȼ� ī��Ʈ.
    int    iWfrFcpPxCnt  ; //������ ����Ĩ �ȼ� ī��Ʈ

    //Bonding before.
    int    iRlbPkgPxAvr  ; //Ĩ�� �ִ��� ���� ��հ�.

    double fRlbOffsetX   ; //����� XY������ �����V ��ŭ ���������� �ڵ鷯���� ���� �ش�. (���� ����Ʈ)
    double fRlbOffsetY   ; //����� XY������ �����V ��ŭ ���������� �ڵ鷯���� ���� �ش�. (���� ����Ʈ)

    int   iRlbPxCnt     ; //�����Ȱ� �ȼ��� ��� ����.


    //Bonding After.
    double fRlaWidth     ; //Ĩ�� ����.
    double fRlaHeight    ; //Ĩ�� ����.
    double fRlaGap       ; //���� ���� ��.
    double fRlaAngle     ; //���� Ĩ�� ����� ����ġ�� �ű⼭ ���ذ��� ������ Ĩ�� ������ ����.
    double fRlaAngleGap  ; //���� ��
    double fRlaOffsetX   ; //Ĩ�� ���� ��ġ X (������ �˻� ����� �����ؾ���.)
    double fRlaOffsetY   ; //Ĩ�� ���� ��ġ Y (������ �˻� ����� �����ؾ���.)
    double fRlaOffsetGap ; //Ĩ�� ���� ��ġ Gap.

    //������ ����
    float fWfoSinc      ; //��ġ��

} ;

struct CVCmnOptn {
    bool  bWfrFlipX     ; //�ڵ鷯�� �����ִ� �� X �ø�.
    bool  bWfrFlipY     ; //�ڵ鷯�� �����ִ� �� Y �ø�.
    bool  bWfrFlipT     ; //�ڵ鷯�� �����ִ� �� T �ø�.

    bool  bRlbFlipX     ; //�ڵ鷯�� �����ִ� �� X�ø�.
    bool  bRlbFlipY     ; //�ڵ鷯�� �����ִ� �� Y�ø�.
    bool  bRlbFlipT     ; //�ڵ鷯�� �����ִ� �� T�ø�.
    bool  bRlbNotUseAng ; //Angle�� �׻�0.0����.

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
