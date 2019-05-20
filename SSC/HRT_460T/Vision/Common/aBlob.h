//---------------------------------------------------------------------------
#ifndef aBlobH
#define aBlobH
//---------------------------------------------------------------------------
#include "UnitImage.h"
#include "UnitArea.h"
#include "BaseDefine.h"

class CBlob
{
    public :
        struct TBlob : public TRect {
            int    iCenterX     ; //최외곽들의 센터
            int    iCenterY     ;
            int    iWidth       ;
            int    iHeight      ;
            int    iArea        ;
            double dBoundary    ; //테두리 길이.
            double dCompact     ; //에리아/길이. 원형일수록 높다.  원의 컴펙트네스는 파이/반지름.
            double dCentroidX   ; //에리아 무게중심.
            double dCentroidY   ;

            CLinkedList<TPoint> plBoundary ; //외각 구성원.

            TBlob() {
                Clear();
            };
            ~TBlob() {
                Clear();
            };
            void Clear() {

                left       = 0 ;
                top        = 0 ;
                right      = 0 ;
                bottom     = 0 ;

                iCenterX   = 0 ;
                iCenterY   = 0 ;
                iWidth     = 0 ;
                iHeight    = 0 ;
                iArea      = 0 ;

                dBoundary  = 0.0;
                dCompact   = 0.0;
                dCentroidX = 0.0;
                dCentroidY = 0.0;

                plBoundary.DeleteAll() ;
            }
            void Copy(TBlob & rhs) {
                left       = rhs.left       ;
                top        = rhs.top        ;
                right      = rhs.right      ;
                bottom     = rhs.bottom     ;

                iCenterX   = rhs.iCenterX   ;
                iCenterY   = rhs.iCenterY   ;
                iWidth     = rhs.iWidth     ;
                iHeight    = rhs.iHeight    ;
                iArea      = rhs.iArea      ;

                dBoundary  = rhs.dBoundary  ;
                dCompact   = rhs.dCompact   ;
                dCentroidX = rhs.dCentroidX ;
                dCentroidY = rhs.dCentroidY ;

                for(int i = 0 ; i < plBoundary.GetDataCnt() ; i++){
                    plBoundary.PushBack(rhs.plBoundary.GetCrntData(!i)) ;

                }


            }
            //Overriding the Operator
            void operator = (TBlob & rhs){
                Copy(rhs);
            }
        };

        struct TPara {
            double dPitchX     ; //블랍 검색 X 피치
            double dPitchY     ; //블랍 검색 Y 피치
            int    iThreshold  ; //검색 트레숄드.
            bool   bFindLtOnDk ; //검은바탕흰물체인지 흰바탕 검은물체인지.
            bool   bIgnrInside ; //블랍 내부 블랍 무시.
            bool   bIgnrSide   ; //사각형 테투리 경계에 있는 놈들 무시.
            int    iStartXOfs  ;
            int    iStartYOfs  ;
        } ;

        struct TFltr {
            //Filtering.
            int    iMinArea      ;
            int    iMaxArea      ; //에리아 민맥스값
            int    iMinWidth     ;
            int    iMaxWidth     ; //넓이
            int    iMinHeight    ;
            int    iMaxHeight    ; //놉이
            double dMinBoundary  ;
            double dMaxBoundary  ; //테두리
            double dMinCompact   ;
            double dMaxCompact   ; //에리아/테두리
        } ;

        struct TRslt {
            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                iBlobCnt = 0 ;

                dInspTime=0.0;
                sErrMsg  = "";

                TBlob* Blob ;
                for(int i=0 ; i < plBlob.GetDataCnt() ; i++) {
                    Blob = plBlob.GetCrntData(!i);
                    Blob->Clear() ;
                    delete Blob ;
                }
                plBlob.DeleteAll();

            }

            int     iBlobCnt  ;
            CLinkedList<TBlob*> plBlob ;

            double  dInspTime ;
            String  sErrMsg   ;
        } ;

        CBlob();
        ~CBlob();

    protected :
        static bool Inspect(unsigned char *_pPxl , int _iPxlWidth , TRect _tRect , TPara _tPara , TFltr _tFltr , TRslt *_pRslt);

    public :
        static bool Inspect(CImage *_pImg , TRect _tRect , TPara _tPara , TFltr _tFltr , TRslt *_pRslt);
        static bool Inspect(CArea  *_pAra , TRect _tRect , TPara _tPara , TFltr _tFltr , TRslt *_pRslt);

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
