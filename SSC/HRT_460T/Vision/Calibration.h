//---------------------------------------------------------------------------
#ifndef CalibrationH
#define CalibrationH
//---------------------------------------------------------------------------
#include "BasePkg.h"
#include "aBlob.h"
#include "UnitImage.h"
#include "TsaiCalibrator.h"
//201504 ����� ����.
class CCalibration : public CPkgBase
{
    //�ϴ� ��Ͼ��Ѵ�.
    //REGIST_STATICLLINK_HEADER(CCalibration)
    public  :

        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            //DECLARATION(bool  ,bUseCommonCal); //true:������ �������� Ķ false:�����Ϻ� Ķ(ex�ܺ���).
            DECLARATION(bool  ,bUseSimpleCal); //�׳� �ܼ��ϰ� PixelResolution�� ����ϴ� ���.

            TMasterPara(){
                //PROPERTY(List , bool  ,bUseCommonCal,"Use Common Calibration" , "�����̸��극�̼� ���");
                PROPERTY(List , bool  ,bUseSimpleCal,"Use Simple Calibration" , "Simple Cal ���      ");
                SetDefault();
            }
            void SetDefault(){
                bUseSimpleCal=true;
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(double,dDotPitch   );
            DECLARATION(double,dCellSize   );

            DECLARATION(double,dPitchX     );
            DECLARATION(double,dPitchY     );
            DECLARATION(int   ,iThreshold  );
            DECLARATION(bool  ,bFindLtOnDk );
            DECLARATION(bool  ,bIgnrInside );
            DECLARATION(bool  ,bIgnrSide   );

            DECLARATION(int   ,iMinArea    );
            DECLARATION(int   ,iMaxArea    );
            DECLARATION(int   ,iMinWidth   );
            DECLARATION(int   ,iMaxWidth   );
            DECLARATION(int   ,iMinHeight  );
            DECLARATION(int   ,iMaxHeight  );
            DECLARATION(double,dMinBoundary);
            DECLARATION(double,dMaxBoundary);
            DECLARATION(double,dMinCompact );
            DECLARATION(double,dMaxCompact );

            TCommonPara(){
                PROPERTY(List , double,dDotPitch   ,"Dot Pitch         " , "Dot����(mm) "            );
                PROPERTY(List , double,dCellSize   ,"Cell Size         " , "Image Sensor 1Cell Size "); VdCellSize.SetMinMax(0.0001 , 0.02);

                PROPERTY(List , double,dPitchX     ,"PitchX            " , "�˻� ����X  "); VdPitchX   .SetMinMax(0,100);
                PROPERTY(List , double,dPitchY     ,"PitchY            " , "�˻� ����Y  "); VdPitchY   .SetMinMax(0,100);
                PROPERTY(List , int   ,iThreshold  ,"Threshold         " , "�Ӱ谪      "); ViThreshold.SetMinMax(0,255);
                PROPERTY(List , bool  ,bFindLtOnDk ,"Find Light On Dark" , "������ü�˻�");
                PROPERTY(List , bool  ,bIgnrInside ,"Ignore Inside     " , "���κ�����");
                PROPERTY(List , bool  ,bIgnrSide   ,"Ignore Side       " , "�ܰ�������");




                PROPERTY(List , int   ,iMinArea    ,"Min Area          " , "�ּҿ���    ");
                PROPERTY(List , int   ,iMaxArea    ,"Max Area          " , "�ִ뿵��    ");
                PROPERTY(List , int   ,iMinWidth   ,"Min Width         " , "�ּҳ���    ");
                PROPERTY(List , int   ,iMaxWidth   ,"Max Width         " , "�ִ����    ");
                PROPERTY(List , int   ,iMinHeight  ,"Min Height        " , "�ּҳ���    ");
                PROPERTY(List , int   ,iMaxHeight  ,"Max Height        " , "�ִ����    ");
                PROPERTY(List , double,dMinBoundary,"Min Boundary      " , "�ּҿܰ�    ");
                PROPERTY(List , double,dMaxBoundary,"Max Boundary      " , "�ִ�ܰ�    ");
                PROPERTY(List , double,dMinCompact ,"Min Compact       " , "�ּ����е�  ");
                PROPERTY(List , double,dMaxCompact ,"Max Compact       " , "�ִ����е�  ");
                SetDefault();
            }

            void SetDefault(){
                dDotPitch   =1.0  ;
                dCellSize   =0.0099;

                dPitchX     =1.0  ;
                dPitchY     =1.0  ;
                iThreshold  =0    ;
                bFindLtOnDk =false;
                bIgnrInside =false;
                bIgnrSide   =true ;

                iMinArea    =0    ;
                iMaxArea    =0    ;
                iMinWidth   =0    ;
                iMaxWidth   =0    ;
                iMinHeight  =0    ;
                iMaxHeight  =0    ;
                dMinBoundary=0.0  ;
                dMaxBoundary=0.0  ;
                dMinCompact =0.0  ;
                dMaxCompact =0.0  ;
            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool   bInspEnd ;
            bool   bBlobEnd ;
            bool   bRsltOk  ;

            double dPxResolSimp ; //���ù�� �������.

            double dPxResolLenz ; //�������� �������.
            double dMmResolLenz ;

            double dWorldSttX ;
            double dWorldSttY ;

            double dWorldEndX ;
            double dWorldEndY ;

            double dImgToWldCntOfsX ;
            double dImgToWldCntOfsY ;


            CBlob::TRslt BlobRslt ;
            TDPoint *    apCalPnt ; //Ķ���극�̼� �ϰ� �� �Ŀ� ���� ����Ʈ��.

            int    iRowCnt ;
            int    iColCnt ;

            double dCalPadAngle ;

            TRslt(){
                apCalPnt = NULL ;
                Clear();
            }
            ~TRslt(){
                apCalPnt = NULL ;
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bBlobEnd = false ;
                bRsltOk  = false ;

                dPxResolSimp = 1.0 ;

                dPxResolLenz = 0.0 ;
                dMmResolLenz = 0.0 ;


                iRowCnt  = 0 ;
                iColCnt  = 0 ;

                dWorldSttX = 0.0 ;
                dWorldSttY = 0.0 ;

                dWorldEndX = 0.0 ;
                dWorldEndY = 0.0 ;

                dCalPadAngle = 0.0 ;

                if(apCalPnt){
                    delete [] apCalPnt ;
                    apCalPnt = NULL ;
                }

                BlobRslt.Clear();


            }
        };

        //Functions
        CCalibration();
        ~CCalibration();



    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :

        TMasterPara  MPara ;
        TCommonPara  CPara ;

        //Tsai Cal Para....
        camera_parameters      CP ; //ī�޶� �ϵ���� ����.
        calibration_data       CD ; //Ķ�е� ���� ������ ��ǥ.
        calibration_constants  CC ; //Ķ���극�̼� ���.

        CImage * m_pCalImg ;

        TRslt  Rslt ;

        CLinkedList<CTracker*>   m_lTrckInsp ;
        CLinkedList<TLocalPara*> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.


    private :
        void OnTrackerChanged();

    public :
        CImage * GetCalImg();
        bool GetCalImgFromImg(CImage *_pSrcImg , CImage * _pDstImg);
        void GetCalPos(double _dImgX , double _dImgY , double * _dWrdX , double * _dWrdY);
        bool FindBlob(CImage * _pImg);
        double GetPxResolSimp();
        void   SetPxResolSimp(double _dPxResol);
        double GetPxResolLenz();
        void   SetPxResolLenz(double _dPxResol);
        double GetPxResol();


        //���� ���� �Լ� ��================================================
        virtual bool Init();
        virtual bool Close();

        //�˻� �� ���� ����.
        virtual bool   Train            (CImage * _pImg);
        virtual void   RsltClear        (); //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
        virtual bool   GetRslt          ();
        virtual bool   Run              (CImage * _pImg);
        virtual String GetErrMsg        ();

        //ȭ�� ���.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
        virtual bool MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual bool MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void MouseUp(TMouseButton Button,TShiftState Shift );

        //�Ķ���� ���÷���
        virtual void UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);

        //�ε� ���̺�.
        virtual void LoadSave(bool _bLoad , String _sVisnName);
};


#endif



/*
�̰��� C++�� ������ ������(operator overloading)�� ����� ���Դϴ�.
C++������ �ǿ�����(operand)�� ���� ���Ƿ� �����ڸ� ������ �� �ֽ��ϴ�. 

���� ���� ���ǰ� ������ <<, >> �������� �������� iostream���� ã�� �� �ֽ��ϴ�. 
�ٷ� istream::operator>>(), ostream::operator<<() ����. 
(��, cin >> ... , cout << ... �� ���ϴ� �̴ϴ�.) 
�̰��� ����Ʈ ����� ���� ������� ��Ʈ�� ������� ��Ÿ���ϴ�.

C++ �ڵ忡�� � �����ڰ� ���� ���� �׻� �������� ������������ Ȯ���ؾ� �մϴ�.
���� ���򸻿��� TOpenDialog::Options�� �˻��ϸ� ������ ���� ǥ���� ���ɴϴ�. 

typedef Set<TOpenOption, ofReadOnly, ofNoDereferenceLinks> TOpenOptions;

�̰��� TOpenDialog::Options�� Ÿ���� TOpenOptions�̰�, 
TOpenOptions�� Set<TOpenOption, ofReadOnly, ofNoDereferenceLinks> ���� ���ǵ� Ÿ������ �� �� �ֽ��ϴ�.

���⼭ �ٽ� 'Set'�κ��� ��ũ�� Ŭ���ϸ�, �̰��� VCL/CLX���� �����ϴ� �ڷ����� Set(����)�̶�� ���ø� Ŭ������ �� �� �ֽ��ϴ�. 
(����� ANSI C++ ǥ�� ���̺귯���� std::set���� ���� �ٸ� �̴ϴ�. �����ϼ���.) 
��, Set::operator<<() �� ���տ� ���Ҹ� �����ϴ� �������̰�,
Set::operator>>()�� ���տ��� ���Ҹ� �����ϴ� �������Դϴ�. 

�׷��Ƿ�, �Ʒ��� �ڵ�� 

:   OpenDialog1->Options >> ofAllowMultiSelect; 
:   OpenDialog1->Options >> ofNoChangeDir; 
:   OpenDialog1->Options << ofExtensionDifferent; 

OpenDialog1->Options ���տ��� ofAllowMultiSelect, ofNoChangeDir��� ���Ҹ� �����ϰ�, 
ofExtensionDifferent��� ���Ҹ� �����Ѵٴ� �ǹ�����.

: �� <<, >> �� ������ ������ ����ŭ ��Ʈ���� �Űܰ��� ���� �˾Ҵµ�..
: �ƿ﷯ �̷��͵� �ִ���.. 
: OpenDialog1->Options << ofFileMustExist << ofHideReadOnly << ofNoChangeDir;.. 

���� Set::operator<<()�� ostream::operator<<()ó�� �������� �����ؼ� ����� �� �ֽ��ϴ�. 
Set::operator<<()�� ���ϰ��� Set& �� �Ǳ� ��������.

�̷��� ���� Ÿ���� VCL/CLX���� (�ΰ� �̻�) ������ �ɼ��� �����ϴ� ��� ��쿡
�������ϰ� ���˴ϴ�. ������ �ݵ�� �����صνñ� �ٶ��ϴ�.

���� ��� ������ �޽��� ���̾�α׸� ǥ���ϴ� �� ���� ����ϴ�
MessageDlg �Լ��� �� ���� �׷���. ���� ���... 

if (MessageDlg("������ ������ �����Ͻðڽ��ϱ�?", mtWarning, 
           TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
{          // TMsgDlgButtons ���� Ÿ���� �ӽ� ��ü�� �����ϰ� mbYes�� mbNo ���Ҹ� ���� 
    DeleteFile(OpenDialog1->FileName);
}

C++������ C++ �������� ��ŭ C++�� ���� ����� ���ذ� �ʿ��մϴ�.
C ���ĸ����δ� ���� ����� �ٷ� �� ���� ���Դϴ�.
�� ��쿡�� ������ �������� ���ø�, �ӽ� ��ü ���� ����� ���� ���ذ� �ʿ�������.
C++�� ���� Ȯ���� �����α⸦ �ٶ��ϴ�.
*/



