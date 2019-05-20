//---------------------------------------------------------------------------
#ifndef VOptnManH
#define VOptnManH

#include <vcl.h>

//---------------------------------------------------------------------------


//Vision Device Info.
struct CVDevOptn {

    //������ Ĩ �����.
    double fWfrWidth     ; //Ĩ�� ���� ����.
    double fWfrHeight    ; //Ĩ�� ���� ����.
    double fWfrGap       ; //Ĩ�� ���� ���� ���� �����V ���� ���̼������� +- Gap�ȿ� ��� ���� Pass
    double fWfrAngle     ; //Ĩ�� �������� ���� ����.
    
    int    iWfrCCPxCnt   ; //ũ�� Ĩ Ȯ�� �ȼ� ����.

    float  fWfrOriSinc   ; //������ ������ ��ũ.

    bool   bWfrIgnRctInsp; // �簢�� �˻� ����

    //������ ����
    float  fWfoOriSinc   ; //��ġ��

    bool   bWfoIgnRctInsp; //�簢�� �˻� ��ŵ

    //������ ��.
    int    iWfpPxCnt     ; //�ȼ� ī��Ʈ.
    int    iWfpPxHigh    ; //�ȼ� ���.  0~ iWfpPxHigh�� ������ ������ ī��Ʈ �Ѵ�.

    //Ķ���극�̼�.


    //���� ����.
    int    iEndExposure ;  //���� ���� �ͽ�����.


} ;

struct CVCmnOptn {      // �̰� ������ �Ⱥ��̰� �� ���� �˻�.. �� �� ���������� ���� ���� �ϱ� ... ���� �� ���� �����ϱ� �մ��� ���ϰ� 
    bool  bWfrFlipX     ; //�ڵ鷯�� �����ִ� �� X �ø�.
    bool  bWfrFlipY     ; //�ڵ鷯�� �����ִ� �� Y �ø�.
    bool  bWfrFlipT     ; //�ڵ鷯�� �����ִ� �� T �ø�.

    bool  bWfoFlipX     ; //�ڵ鷯�� �����ִ� �� X �ø�.
    bool  bWfoFlipY     ; //�ڵ鷯�� �����ִ� �� Y �ø�.
    bool  bWfoFlipT     ;

    bool  bWfpFlipX     ; //�ڵ鷯�� �����ִ� �� X �ø�.
    bool  bWfpFlipY     ; //�ڵ鷯�� �����ִ� �� Y �ø�.



    int   iWfrImageSave ; //�̹��� ���� ����.
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
