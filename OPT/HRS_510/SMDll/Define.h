#ifndef DEFINEH
#define DEFINEH

#define AJIN_AXT //���� ���� ���̺귯�� ����.
//#define AJIN_AXL //���� ���� ���̺귯�� ����.





#ifdef AJIN_AXT
    #pragma comment(lib, "AxtLib.lib")            // Ajinextek Library
    #include "AxtLIB.h"
#endif //AJIN_AXT
#ifdef AJIN_AXL
    #include <vcl.h> //�ƾ��� ���� �ȵų�...���� ���ε�
    //#include <winnt.h>
    #pragma comment(lib, "AXL.lib")            // Ajinextek Library
    #include "AXL.h"
#endif //AJIN_AXL


#define BYTE_BIT_COUNT 8




#endif


/*
void CAxlQI10::ApplyAxlPara ()��� ã�Ƽ� Ȯ���ϱ�.


*/
