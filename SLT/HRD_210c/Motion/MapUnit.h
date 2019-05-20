//---------------------------------------------------------------------------

#ifndef MapUnitH
#define MapUnitH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include "UtilDefine.h"
#include "DataMan.h"

#define MAX_MAP_CNT 50

class CMapUnit
{
    private:
        TMemo * m_mmRowMap ;


    public :
        __fastcall CMapUnit:: CMapUnit(void);
        __fastcall CMapUnit::~CMapUnit(void);

        void Init();

        CArray _riDst;

        void SetMemo (TMemo *_mmRowMap);

        //_sSrcFilePath루트에 있는 파일을 열어서 _mmDst 메모장에 그냥 텍스트 파일로 연다.
        bool OpenRawMap  (String _sSrcFilePath);

        //_sSrcFilePath -> _sDstFolderPath 파일 복사.
        bool Copyfile    (String _sSrcFilePath , String _sDstFolderPath);

        //_sSrcFilePath루트에 있는 파일을 역어서 _mmDst메모장에 텍스트 파일로 열고. _riDst에 맵을 올린다.
        bool ConvertCmn(String _sSrcFilePath    , EN_ARAY_ID _riDst);
        bool ConvertCmn(TStringGrid * _sgSource , EN_ARAY_ID _riDst);
        bool ConvertCmn(                          EN_ARAY_ID _riDst);  

        //_riDst에  _dDia의 지름 , _dChipWidth 의 칩 넓이 _dChipHeight의 칩 높이. 를 이용하여 마스크를 올린다.
        bool SetMask (EN_ARAY_ID _riDst);

        // _riMask 올리고 _riMap맵 덥고 한것을 _riDst생성.
        bool MergeArray  (EN_ARAY_ID _riMsk , EN_ARAY_ID _riMap , EN_ARAY_ID _riDst);

        bool ConvertNxt(String _sSrcFilePath    , EN_ARAY_ID _riDst);
        bool ConvertNxt(TStringGrid * _sgSource , EN_ARAY_ID _riDst);
        bool ConvertNxt(                          EN_ARAY_ID _riDst);

        // EX)  -2,5,516,20,0,85,1
        //bool ConvertCoordi1 (String _sSrcFilePath    , EN_ARAY_ID _riDst);
        //bool ConvertCoordi1 (TStringGrid * _sgSource , EN_ARAY_ID _riDst);

        // EX)  1[-002,0001,0001]   3         '    FAIL   '                     1.0 '       PASS   '       OVER   *F*
        //bool ConvertCoordi2 (String _sSrcFilePath    , EN_ARAY_ID _riDst);
        //bool ConvertCoordi2 (TStringGrid * _sgSource , EN_ARAY_ID _riDst);

        // EX)  X= 0192 Y= 0136 B= 006 D= 001 S= 0001     8인치
        //bool ConvertCoordi8in (String _sSrcFilePath    , EN_ARAY_ID _riDst);
        //bool ConvertCoordi8in (TStringGrid * _sgSource , EN_ARAY_ID _riDst);

        // csNone를 csMask로 변환
        bool ChangeMaskNone (String _sSrcFilePath, int _iMapType , EN_ARAY_ID _riDst);

        bool CheckConvert();

        bool GetSourceName(String &_sName );

        void SaveHanraMap(EN_ARAY_ID _iAray , AnsiString _sFileName);
        void LoadHanraMap(EN_ARAY_ID _iAray , AnsiString _sFileName);


        //bool ConvertFile (TStringGrid * _sgSource , TStringGrid *_sgOutput , TMemo *_mmMemo);
        //bool ConvertMap  (TStringGrid * _sgOutput , EN_ARAY_ID _riMAP );
        //bool ConvertMap  ( EN_ARAY_ID riMAP );



        //bool Copyfile(String _sSrcFilePath , String _sDstFolderPath);
        //bool ChangeMaskNone (String _sSrcFilePath , int _iMapType , EN_ARAY_ID _riDst);
        //bool ConvertCoordi1 (String _sSrcFilePath , EN_ARAY_ID _riDst);
        //bool ConvertCoordi2 (String _sSrcFilePath , EN_ARAY_ID _riDst);
//        void __fastcall DelPastLotLog();

    AnsiString sSource ;
    AnsiString sWorked ;

    AnsiString sMapBackUpName;

};

extern CMapUnit MAPU;
#endif
 