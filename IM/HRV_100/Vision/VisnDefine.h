#ifndef VISNDEFINEH
#define VISNDEFINEH

//�ڵ鷯���� ��Ŭ��� �ؼ� ��� �� �� ��....��� �� �͵�.....


enum EN_VISN_ID {
    viNone   =-1 ,
    viColi       , //�ϴ� �Ⱦ�... ����������.
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


//���� ��Ʈ�ѷ� ����.----------------------------------------------------------
enum EN_LIGHT_ID {
    liNone = -1 ,    //���� ����.

    MAX_LIGHT_ID = 0
};


//ī�޶��� ����.---------------------------------------------------------------
enum EN_CAM_ID {
    ciCam1  = 0 , //ī�޶� 1�� ����. 640x480 8��Ʈ.

    MAX_CAM_ID
};



#endif



