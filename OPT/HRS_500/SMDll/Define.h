#ifndef DEFINEH
#define DEFINEH

#define AJIN_AXT //아진 구형 라이브러리 쓸때.
//#define AJIN_AXL //아진 신형 라이브러리 쓸때.





#ifdef AJIN_AXT
    #pragma comment(lib, "AxtLib.lib")            // Ajinextek Library
    #include "AxtLIB.h"
#endif //AJIN_AXT
#ifdef AJIN_AXL
    #include <vcl.h> //아씨발 빼면 안돼네...순서 꼬인듯
    //#include <winnt.h>
    #pragma comment(lib, "AXL.lib")            // Ajinextek Library
    #include "AXL.h"
#endif //AJIN_AXL


#define BYTE_BIT_COUNT 8




#endif


/*
void CAxlQI10::ApplyAxlPara ()요거 찾아서 확인하기.


*/
