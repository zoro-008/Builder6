//---------------------------------------------------------------------------
#ifndef CalibrationH
#define CalibrationH
//---------------------------------------------------------------------------
#include "BasePkg.h"
#include "aBlob.h"
#include "UnitImage.h"
#include "TsaiCalibrator.h"
//201504 선계원 제작.
class CCalibration : public CPkgBase
{
    //일단 등록안한다.
    //REGIST_STATICLLINK_HEADER(CCalibration)
    public  :

        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            //DECLARATION(bool  ,bUseCommonCal); //true:비젼별 공용으로 캘 false:잡파일별 캘(ex줌비젼).
            DECLARATION(bool  ,bUseSimpleCal); //그냥 단순하게 PixelResolution만 계산하는 방식.

            TMasterPara(){
                //PROPERTY(List , bool  ,bUseCommonCal,"Use Common Calibration" , "공용켈리브레이션 사용");
                PROPERTY(List , bool  ,bUseSimpleCal,"Use Simple Calibration" , "Simple Cal 사용      ");
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
                PROPERTY(List , double,dDotPitch   ,"Dot Pitch         " , "Dot간격(mm) "            );
                PROPERTY(List , double,dCellSize   ,"Cell Size         " , "Image Sensor 1Cell Size "); VdCellSize.SetMinMax(0.0001 , 0.02);

                PROPERTY(List , double,dPitchX     ,"PitchX            " , "검색 간격X  "); VdPitchX   .SetMinMax(0,100);
                PROPERTY(List , double,dPitchY     ,"PitchY            " , "검색 간격Y  "); VdPitchY   .SetMinMax(0,100);
                PROPERTY(List , int   ,iThreshold  ,"Threshold         " , "임계값      "); ViThreshold.SetMinMax(0,255);
                PROPERTY(List , bool  ,bFindLtOnDk ,"Find Light On Dark" , "밝은물체검색");
                PROPERTY(List , bool  ,bIgnrInside ,"Ignore Inside     " , "내부블럽무시");
                PROPERTY(List , bool  ,bIgnrSide   ,"Ignore Side       " , "외곽블럽무시");




                PROPERTY(List , int   ,iMinArea    ,"Min Area          " , "최소영역    ");
                PROPERTY(List , int   ,iMaxArea    ,"Max Area          " , "최대영역    ");
                PROPERTY(List , int   ,iMinWidth   ,"Min Width         " , "최소넓이    ");
                PROPERTY(List , int   ,iMaxWidth   ,"Max Width         " , "최대넓이    ");
                PROPERTY(List , int   ,iMinHeight  ,"Min Height        " , "최소높이    ");
                PROPERTY(List , int   ,iMaxHeight  ,"Max Height        " , "최대높이    ");
                PROPERTY(List , double,dMinBoundary,"Min Boundary      " , "최소외곽    ");
                PROPERTY(List , double,dMaxBoundary,"Max Boundary      " , "최대외곽    ");
                PROPERTY(List , double,dMinCompact ,"Min Compact       " , "최소조밀도  ");
                PROPERTY(List , double,dMaxCompact ,"Max Compact       " , "최대조밀도  ");
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

            double dPxResolSimp ; //심플방식 레졸루션.

            double dPxResolLenz ; //렌즈보정방식 레졸루션.
            double dMmResolLenz ;

            double dWorldSttX ;
            double dWorldSttY ;

            double dWorldEndX ;
            double dWorldEndY ;

            double dImgToWldCntOfsX ;
            double dImgToWldCntOfsY ;


            CBlob::TRslt BlobRslt ;
            TDPoint *    apCalPnt ; //캘리브레이션 하고 난 후에 센터 포인트들.

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



    //이PKG에 특화된 내부에서만 쓰는 변수 함수들.===============================
    private :

        TMasterPara  MPara ;
        TCommonPara  CPara ;

        //Tsai Cal Para....
        camera_parameters      CP ; //카메라 하드웨어 스펙.
        calibration_data       CD ; //캘패드 실제 사이즈 좌표.
        calibration_constants  CC ; //캘리브레이션 상수.

        CImage * m_pCalImg ;

        TRslt  Rslt ;

        CLinkedList<CTracker*>   m_lTrckInsp ;
        CLinkedList<TLocalPara*> m_lLPara    ;   //갯수가 트레커에 갯수에 따라 늘어남.


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


        //순수 가상 함수 단================================================
        virtual bool Init();
        virtual bool Close();

        //검사 및 러닝 관련.
        virtual bool   Train            (CImage * _pImg);
        virtual void   RsltClear        (); //검사 결과값을 검사전에 클리어 한번 하고 한다.
        virtual bool   GetRslt          ();
        virtual bool   Run              (CImage * _pImg);
        virtual String GetErrMsg        ();

        //화면 출력.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //이미지 화면 클릭 이벤트
        virtual bool MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual bool MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void MouseUp(TMouseButton Button,TShiftState Shift );

        //파라미터 디스플레이
        virtual void UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);

        //로드 세이브.
        virtual void LoadSave(bool _bLoad , String _sVisnName);
};


#endif



/*
이것은 C++의 연산자 겹지정(operator overloading)을 사용한 예입니다.
C++에서는 피연산자(operand)에 따라 임의로 연산자를 정의할 수 있습니다. 

가장 많이 사용되고 유명한 <<, >> 연산자의 겹지정은 iostream에서 찾을 수 있습니다. 
바로 istream::operator>>(), ostream::operator<<() 지요. 
(즉, cin >> ... , cout << ... 을 말하는 겁니다.) 
이것은 쉬프트 연산과 전혀 상관없이 스트림 입출력을 나타냅니다.

C++ 코드에서 어떤 연산자가 사용될 때는 항상 겹지정된 연산자인지를 확인해야 합니다.
빌더 도움말에서 TOpenDialog::Options을 검색하면 다음과 같은 표현이 나옵니다. 

typedef Set<TOpenOption, ofReadOnly, ofNoDereferenceLinks> TOpenOptions;

이것은 TOpenDialog::Options의 타입은 TOpenOptions이고, 
TOpenOptions은 Set<TOpenOption, ofReadOnly, ofNoDereferenceLinks> 으로 정의된 타입임을 알 수 있습니다.

여기서 다시 'Set'부분의 링크를 클릭하면, 이것은 VCL/CLX에서 제공하는 자료형인 Set(집합)이라는 템플릿 클래스을 알 수 있습니다. 
(참고로 ANSI C++ 표준 라이브러리의 std::set과는 전혀 다른 겁니다. 주의하세요.) 
즉, Set::operator<<() 는 집합에 원소를 삽입하는 연산자이고,
Set::operator>>()는 집합에서 원소를 삭제하는 연산자입니다. 

그러므로, 아래의 코드는 

:   OpenDialog1->Options >> ofAllowMultiSelect; 
:   OpenDialog1->Options >> ofNoChangeDir; 
:   OpenDialog1->Options << ofExtensionDifferent; 

OpenDialog1->Options 집합에서 ofAllowMultiSelect, ofNoChangeDir라는 원소를 삭제하고, 
ofExtensionDifferent라는 원소를 삽입한다는 의미지요.

: 전 <<, >> 는 다음에 나오는 수만큼 비트수가 옮겨가는 건줄 알았는데..
: 아울러 이런것도 있던데.. 
: OpenDialog1->Options << ofFileMustExist << ofHideReadOnly << ofNoChangeDir;.. 

물론 Set::operator<<()도 ostream::operator<<()처럼 연속으로 나열해서 사용할 수 있습니다. 
Set::operator<<()의 리턴값이 Set& 가 되기 때문이죠.

이러한 집합 타입은 VCL/CLX에서 (두개 이상) 복수로 옵션을 선택하는 모든 경우에
광범위하게 사용됩니다. 사용법을 반드시 숙지해두시기 바랍니다.

예를 들어 간단한 메시지 다이얼로그를 표시하는 데 자주 사용하는
MessageDlg 함수를 쓸 때도 그렇죠. 예를 들면... 

if (MessageDlg("정말로 파일을 삭제하시겠습니까?", mtWarning, 
           TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
{          // TMsgDlgButtons 집합 타입의 임시 객체를 생성하고 mbYes와 mbNo 원소를 삽입 
    DeleteFile(OpenDialog1->FileName);
}

C++빌더는 C++ 개발툴인 만큼 C++에 대한 충분한 이해가 필요합니다.
C 지식만으로는 절대 제대로 다룰 수 없는 툴입니다.
이 경우에도 연산자 겹지정과 템플릿, 임시 객체 생성 방법에 대한 이해가 필요하지요.
C++을 먼저 확실히 익혀두기를 바랍니다.
*/



