#ifndef SVAPPH
#define SVAPPH


//#include "SVImage.h"
//#include "SVArea.h"
//#include "SVGdi.h"
//#include "SVInspStruct.h"




#define SVA_API	__declspec(dllexport)

#include "VisnDefine.h"


extern "C" { //Alghorithm

    SVA_API bool            __stdcall InitVisn(char * _sDevName);
    SVA_API bool            __stdcall SetVisnForm(void * hWnd   );
    SVA_API bool            __stdcall DelVisn ();

    SVA_API void            __stdcall LoadJobFile(bool _bLoad , char * _sDevName);

    SVA_API void            __stdcall Grab      (EN_VISN_ID _iVisnId );             //사진찍기.
    SVA_API void            __stdcall Live      (EN_VISN_ID _iVisnId , bool _bOn);  //라이브 키기
    SVA_API bool            __stdcall GetGrabEnd(EN_VISN_ID _iVisnId );             //사진찍은거 이미지 받기 끝났는지.

    SVA_API void            __stdcall SetLight  (EN_VISN_ID _iVisnId );             //조명 느릴경우 미리 조명 세팅 해놓기

    SVA_API void            __stdcall Inspect   (EN_VISN_ID _iVisnId , bool _bUseThread); //검사.
    SVA_API bool            __stdcall GetInspEnd(EN_VISN_ID _iVisnId );                          //검사 끝??

    SVA_API int             __stdcall GetRslt   (EN_VISN_ID _iVisnId , TRetResult * _tRslt); //결과.
        /*
        viWfr = 0:Pass , 1:RCT Fail , 2: Line Length Fail, 3: Angle Fail
        viWfo = 0:Pass , 1:OCV Fail , 2: Pixel Cnt Fail
        viWfp = 0:Pass , 1:TSD Fail , 2: Pen Pixel Cnt Fail
        viCal = 0:Pass , 1:CAL Fail
        */

    SVA_API void            __stdcall SetLevel  (EN_OPER_LEVEL _iLevel);
        /*
        0 = Oper
        1 = Engineer
        2 = Master
        */
    SVA_API void            __stdcall SetAutorun (bool _bAutorun);


        //비젼 탭 바꾸기.
    SVA_API void            __stdcall SetActiveVisn(EN_VISN_ID _iVisnId);

};












#endif


