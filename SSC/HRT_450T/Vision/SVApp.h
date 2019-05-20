#ifndef SVAPPH
#define SVAPPH

#define SVA_API	__declspec(dllexport)

#include "VisnDefine.h"


extern "C" { //Alghorithm

    //���� �ʱ�ȭ �� â ���̷�Ʈ ����.
    SVA_API bool __stdcall InitVisn     (void * hMainWnd , void * hParaWnd , char * _sDevName);
    SVA_API bool __stdcall DelVisn      ();
    SVA_API void __stdcall SetImgForm   (void * hImgBaseWnd , bool _bSettingMode);

    //������ ü����.
    SVA_API void __stdcall LoadJobFile  (bool _bLoad , char * _sDevName);

    //ī�޶� ����.
    SVA_API void __stdcall Grab         (EN_VISN_ID _iVisnId );             //�������.
    SVA_API void __stdcall Live         (EN_VISN_ID _iVisnId , bool _bOn);  //���̺� Ű��
    SVA_API bool __stdcall GetGrabEnd   (EN_VISN_ID _iVisnId );             //���������� �̹��� �ޱ� ��������.

    //���� ����.
    SVA_API void __stdcall SetLight     (EN_VISN_ID _iVisnId );             //���� ������� �̸� ���� ���� �س���

    //�˻� ����.
    SVA_API void __stdcall Inspect      (EN_VISN_ID _iVisnId , bool _bUseThread = true); //�˻�.
    SVA_API bool __stdcall GetInspEnd   (EN_VISN_ID _iVisnId );                          //�˻� ��??

    //����� �޾ƿ���.
    SVA_API int  __stdcall GetRslt      (EN_VISN_ID _iVisnId , TRetResult * _tRslt); //���.

    //���� ���� �� ���䷱.
    SVA_API void __stdcall SetLevel     (EN_OPER_LEVEL _iLevel);
    SVA_API void __stdcall SetAutorun   (bool _bAutorun);

    //���� �� �ٲٱ�.
    SVA_API void __stdcall SetActiveVisn(EN_VISN_ID _iVisnId);

};












#endif


