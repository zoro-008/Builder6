//---------------------------------------------------------------------------
#ifndef IOsH
#define IOsH

//---------------------------------------------------------------------------
#include <Grids.hpp>

#include "Ajin_DIO.h"

//#include "Commisdf.h"

#include "SMDllDefine.h"

#include "Timer.h"
#include "TEdit.h"  

#define MAX_SG_COL 7 + 3

//---------------------------------------------------------------------------
class CIOs
{
    public:
        //Creater & Destroyer.
        CIOs();
        virtual ~CIOs();

    protected:
        struct SBit {
            //Para.
            int        iAdd      ; //주소
            AnsiString sEnum     ; //이넘
            AnsiString sName     ; //이름
            AnsiString SComt     ; //설명
            bool       bInv      ; //반전
            int        iOnDelay  ; //지연시간  출력은 사용 안함.
            int        iOffDelay ; //지연시간

            //Value.
            bool       bVtrVal ; //가상의 값 프로그램에서 이값으로 쓴다.
            bool       bAtrVal ; //실제값
            bool       bUpEdge ; //업엣지
            bool       bDnEdge ; //다운엣지
        };

        EN_LAN_SEL m_iLangSel  ; //Languge Selection.

        SBit * m_pIn  ;
    public:
        SBit * m_pOut ;

    protected:

        CDelayTimer * m_pInOnDelay   ;
        CDelayTimer * m_pInOffDelay  ;
        CDelayTimer * m_pOutOnDelay  ;
        CDelayTimer * m_pOutOffDelay ;

        CAjinDIO AjinDIO ;

        int m_iMaxIn  ;
        int m_iMaxOut ;

        bool m_bTestMode ;

        //Direct I/O/
        inline void SetOut (int _iNo , bool _bVal);
        inline bool GetOut (int _iNo             );
        inline bool GetIn  (int _iNo             );

    public:
        //Update.
        void Update();

        //UserIO
        void SetY  (int _iNo , bool _bVal , bool _bDirect = false); //Direct = true일경우 TestMode에서도 출력이 나간다.
        bool GetY  (int _iNo , bool _bDirect = false);
        bool GetYDn(int _iNo );
        bool GetYUp(int _iNo );

        bool GetX  (int _iNo , bool _bDirect = false);              //Direct = true일경우 1싸이클 동안 엣지는 못본다.
        bool GetXDn(int _iNo );
        bool GetXUp(int _iNo );

        bool GetInModuleNo (int _iNo , int &_iModuleNo , int &_iOffset);
        bool GetOutModuleNo(int _iNo , int &_iModuleNo , int &_iOffset);

        void SetTestMode(bool _bOn) {m_bTestMode = _bOn;}
        bool GetTestMode(         ) {return m_bTestMode;}

        String GetXName(int _iNo ) { return m_pIn [_iNo].sName ; }
        String GetYName(int _iNo ) { return m_pOut[_iNo].sName ; }

        String GetXEnum(int _iNo ) { return m_pIn [_iNo].sEnum ; }
        String GetYEnum(int _iNo ) { return m_pOut[_iNo].sEnum ; }

        //Read/Write Para.
        void Load();
        void Save();

        //Load Save Dynamic Var.
        void LoadDnmVar();
        void SaveDnmVar();

        //Display.                                                    //함수내부에서 셀넓이 픽스  //주소번지 16진수 표시.
        void DisplayList    (bool _bOutput , TStringGrid * _sgTable , bool _bFixedWidth = false );
        void UpdateStatus   (bool _bOutput , TStringGrid * _sgTable );
        void DisplayProperty(bool          _bTable      ,
                             int           _bOutput     ,
                             TEdit       * _edIndex     ,
                             TEdit       * _edEnum      ,
                             TEdit       * _edAdd       ,
                             TEdit       * _edIOHexAdd  ,
                             TEdit       * _edName      ,
                             TEdit       * _edComt      ,
                             TCheckBox   * _cbIOInverse ,
                             TEdit       * _edOnDelay   ,
                             TEdit       * _edOffDelay  ,
                             TStringGrid *_sgTable      ,
                             bool         _bDispHexAdd  );

};

//---------------------------------------------------------------------------
extern CIOs IO;

//---------------------------------------------------------------------------
#endif

