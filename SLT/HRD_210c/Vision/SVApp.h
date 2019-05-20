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

    SVA_API void            __stdcall Grab      (EN_VISN_ID _iVisnId );             //�������.
    SVA_API void            __stdcall Live      (EN_VISN_ID _iVisnId , bool _bOn);  //���̺� Ű��
    SVA_API bool            __stdcall GetGrabEnd(EN_VISN_ID _iVisnId );             //���������� �̹��� �ޱ� ��������.

    SVA_API void            __stdcall SetLight  (EN_VISN_ID _iVisnId );             //���� ������� �̸� ���� ���� �س���

    SVA_API void            __stdcall Inspect   (EN_VISN_ID _iVisnId , bool _bUseThread); //�˻�.
    SVA_API bool            __stdcall GetInspEnd(EN_VISN_ID _iVisnId );                          //�˻� ��??

    SVA_API int             __stdcall GetRslt   (EN_VISN_ID _iVisnId , TRetResult * _tRslt); //���.
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


        //���� �� �ٲٱ�.
    SVA_API void            __stdcall SetActiveVisn(EN_VISN_ID _iVisnId);

};












#endif


