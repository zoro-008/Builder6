//---------------------------------------------------------------------------

#ifndef MapUnitH
#define MapUnitH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include "UtilDefine.h"
//#define SOURCE_FOLDER "D:\\HRA_MAP\\SOURCE\\"
//#define OUTPUT_FOLDER "D:\\HRA_MAP\\OUTPUT\\"
//#define WRKEND_FOLDER "D:\\HRA_MAP\\WORKEND\\"

enum EN_MAP_TYPE { //�ϴ��� ���� ��ó���� ������ �ܾ ���...
    mtNone         = 0 ,
    mtDEVICE           ,
    mtWaferMapData     ,
    mtXmlversion       ,
    MAX_MAP_TYPE
};

enum EN_MAP_ANGLE {
    ma0            = 0,
    ma90              ,
    ma180             ,
    ma270             ,
    MAX_MAP_ANGLE
};

class CMapUnit
{
    protected:



    public :
        TStringList * m_slRowList ;

        __fastcall CMapUnit::CMapUnit(void);
        __fastcall CMapUnit::~CMapUnit(void);

        //void SetMemo (TMemo *_mmRowMap);


        //_sSrcFilePath��Ʈ�� �ִ� ������ ��� _mmDst �޸��忡 �׳� �ؽ�Ʈ ���Ϸ� ����.
        bool OpenRawMap  (String _sSrcFilePath);

        //_sSrcFilePath -> _sDstFolderPath ���� ����.
        bool Copyfile    (String _sSrcFilePath , String _sDstFolderPath);

        //_sSrcFilePath��Ʈ�� �ִ� ������ ��� _mmDst�޸��忡 �ؽ�Ʈ ���Ϸ� ����. _riDst�� ���� �ø���.
        bool ConvertCmn (String _sSrcFilePath , EN_ARAY_ID _riDst);

        //_riDst��  _dDia�� ���� , _dChipWidth �� Ĩ ���� _dChipHeight�� Ĩ ����. �� �̿��Ͽ� ����ũ�� �ø���.
        bool SetMask (EN_ARAY_ID _riDst);

        // _riMask �ø��� _riMap�� ���� �Ѱ��� _riDst����.
        bool MergeArray  (EN_ARAY_ID _riMsk , EN_ARAY_ID _riMap , EN_ARAY_ID _riDst );

        // EX)  -2,5,516,20,0,85,1
        bool ConvertCoordi1 (String _sSrcFilePath , EN_ARAY_ID _riDst);

        // EX)  1[-002,0001,0001]   3         '    FAIL   '                     1.0 '       PASS   '       OVER   *F*
        bool ConvertCoordi2 (String _sSrcFilePath , EN_ARAY_ID _riDst);

        // EX)  X= 0192 Y= 0136 B= 006 D= 001 S= 0001     8��ġ
        bool ConvertCoordi8in (String _sSrcFilePath , EN_ARAY_ID _riDst);

        // csNone�� csMask�� ��ȯ
        bool ChangeMaskNone (String _sSrcFilePath, int _iMapType , EN_ARAY_ID _riDst);

        // ���鸸 �����ϴ� �� �����ϱ�..
        bool DeleteNullColRow (EN_ARAY_ID _riMap , EN_ARAY_ID _riDst);

        

};

extern CMapUnit MAPU;
#endif
