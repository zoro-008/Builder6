#ifndef VISNDEFINEH
#define VISNDEFINEH

//핸들러에서 인클루드 해서 써야 할 것 들....써야 될 것들.....


enum EN_VISN_ID {
    viNone   =-1 ,
    viColi       , //일단 안씀... 복잡해져서.
    MAX_VISN_ID
};


enum EN_IMG_ID {
    iiSTGCrnt   = 0 ,
    iiSTG1Stt   = 1 ,
    iiSTG2Stt   = 2 ,
    iiSTG3Stt   = 3 ,
    iiSTG4Stt   = 4 ,
    iiSTG5Stt   = 5 ,
    iiSTG1Max   = 6 ,
    iiSTG2Max   = 7 ,
    iiSTG3Max   = 8 ,
    iiSTG4Max   = 9 ,
    iiSTG5Max   = 10,
    iiSTG1Rslt  = 11,
    iiSTG2Rslt  = 12,
    iiSTG3Rslt  = 13,
    iiSTG4Rslt  = 14,
    iiSTG5Rslt  = 15,
    iiSTG1Drive = 16,
    iiSTG2Drive = 17,
    iiSTG3Drive = 18,
    iiSTG4Drive = 19,
    iiSTG5Drive = 20,
    iiSTG1Full  = 21,
    iiSTG2Full  = 22,
    iiSTG3Full  = 23,
    iiSTG4Full  = 24,
    iiSTG5Full  = 25,
    MAX_IMG_ID
};


//조명 컨트롤러 갯수.----------------------------------------------------------
enum EN_LIGHT_ID {
    liNone = -1 ,    //조명 없음.

    MAX_LIGHT_ID = 0
};


//카메라의 갯수.---------------------------------------------------------------
enum EN_CAM_ID {
    ciCam1  = 0 , //카메라 1개 있음. 640x480 8비트.

    MAX_CAM_ID
};



#endif



