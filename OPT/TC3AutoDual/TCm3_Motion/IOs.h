//---------------------------------------------------------------------------
#ifndef IOsH
#define IOsH

//---------------------------------------------------------------------------
#include <Grids.hpp>

#include "Axt_DIO.h"

//#include "Commisdf.h"

#include "Timer.h"
#include "TEdit.h"

#define MAX_SG_COL 7 +1

//---------------------------------------------------------------------------
class CIOs
{
    public:
        //Creater & Destroyer.
        CIOs();
        virtual ~CIOs();

    protected:
        struct CBit {
            //Para.
            int        iAdd    ; //주소
            AnsiString sName   ; //이름
            AnsiString SComt   ; //설명
            bool       bInv    ; //반전
            int        iDelay  ; //지연시간

            //Value.
            bool       bSetVal ; //명령값
            bool       bGetVal ; //실제값
            bool       bUpEdge ; //업엣지
            bool       bDnEdge ; //다운엣지
        };

        CBit * m_pIn  ; CDelayTimer * m_pInDelay  ;
        CBit * m_pOut ; CDelayTimer * m_pOutDelay ;

        CAxtDIO AxtDIO ;

        int m_iMaxIn  ;
        int m_iMaxOut ;

        //Direct I/O/
        inline void SetOut (int _iNo , bool _bVal);
        inline bool GetOut (int _iNo             );
        inline bool GetIn  (int _iNo             );

    public:
        //Update.
        void Update();

        //UserIO
        void SetY  (int _iNo , bool _bVal , bool _bDirect = false); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
        bool GetY  (int _iNo );
        bool GetYDn(int _iNo );
        bool GetYUp(int _iNo );

        bool GetX  (int _iNo , bool _bDirect = false);              //Direct = true일경우 1싸이클 동안 엣지는 못본다.
        bool GetXDn(int _iNo );
        bool GetXUp(int _iNo );

        //Read/Write Para.
        void Load();
        void Save();

        //Load Save Dynamic Var.
        void LoadDnmVar();
        void SaveDnmVar();

        //Display.                                                    //함수내부에서 셀넓이 픽스  //주소번지 16진수 표시.
        void DisplayList    (bool _bOutput , TStringGrid * _sgTable , bool _bFixedWidth = false , bool _bDispHexAdd = false );
        void UpdateStatus   (bool _bOutput , TStringGrid * _sgTable );
        void DisplayProperty(bool          _bTable    ,
                           int           _bOutput   ,
                           TEdit       * _edIndex   ,
                           TEdit       * _edAdd     ,
                           TEdit       * _edName    ,
                           TEdit       * _edComt    ,
                           TEdit       * _edInv     ,
                           TEdit       * _edDelay   ,
                           TStringGrid *_sgTable    ,
                           bool         _bDispHexAdd) ;


};

//---------------------------------------------------------------------------
extern CIOs IO;

//---------------------------------------------------------------------------
#endif

