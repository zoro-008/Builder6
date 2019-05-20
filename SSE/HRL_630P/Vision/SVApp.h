#ifndef SVAPPH
#define SVAPPH

#define SVA_API	__declspec(dllexport)

#include "VisnDefine.h"


extern "C" { //Alghorithm

    //비전 초기화 및 창 페이런트 설정.
    SVA_API bool __stdcall InitVisn     (void * hMainWnd , void * hParaWnd , char * _sDevName);
    SVA_API bool __stdcall DelVisn      ();
    SVA_API void __stdcall SetImgForm   (void * hImgBaseWnd , bool _bSettingMode);

    //잡파일 체인지.
    SVA_API void __stdcall LoadJobFile  (bool _bLoad , char * _sDevName);

    //카메라 관련.
    SVA_API void __stdcall Grab         (EN_VISN_ID _iVisnId );             //사진찍기.
    SVA_API void __stdcall Live         (EN_VISN_ID _iVisnId , bool _bOn);  //라이브 키기
    SVA_API bool __stdcall GetGrabEnd   (EN_VISN_ID _iVisnId );             //사진찍은거 이미지 받기 끝났는지.

    //조명 관련.
    SVA_API void __stdcall SetLight     (EN_VISN_ID _iVisnId );             //조명 느릴경우 미리 조명 세팅 해놓기

    //검사 관련.
    SVA_API void __stdcall Inspect      (EN_VISN_ID _iVisnId , bool _bUseThread = true); //검사.
    SVA_API bool __stdcall GetInspEnd   (EN_VISN_ID _iVisnId );                          //검사 끝??

    //결과값 받아오기.
    SVA_API int  __stdcall GetRslt      (EN_VISN_ID _iVisnId , TRetResult * _tRslt); //결과.

    //레벨 설정 및 오토런.
    SVA_API void __stdcall SetLevel     (EN_OPER_LEVEL _iLevel);
    SVA_API void __stdcall SetAutorun   (bool _bAutorun);

    //비젼 탭 바꾸기.
    SVA_API void __stdcall SetActiveVisn(EN_VISN_ID _iVisnId);

};












#endif


