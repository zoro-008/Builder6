//---------------------------------------------------------------------------
#ifndef ErrManH
#define ErrManH

#include <Grids.hpp>
#include "TEdit.h"
#include "SMDllDefine.h"
//---------------------------------------------------------------------------
class CErrMan
{
    public:
         CErrMan();
        ~CErrMan();
        struct TErr {
            bool       bOn        ;
            AnsiString sEnum      ;
            AnsiString sName      ;
            AnsiString sAction    ;
            AnsiString sImgPath   ;
            double     dRectLeft  ;
            double     dRectTop   ;
            double     dRectWidth ;
            double     dRectHeight;
            AnsiString sRo        ;
        };

    protected:
        EN_LAN_SEL m_iLangSel  ; //Languge Selection.

        TErr * Err ;         //에러 배열 포인터.
        int    m_iMaxErr   ; //총에러 개수.
        int    m_iLastErr  ; //가장 마지막에 온된 에러.
        int    m_iUpdated  ; //현재 온되어 있는 에러 갯수.
        bool   m_bShowForm ; //false일때만 에러창을 띄운다.

        void  LoadDnmVar();
        void  SaveDnmVar();

    public:
        void  Clear   (void     ); //모든에러 클리어 시키기.
        void  SetErr  (int  _iNo); //해당 에러 true 시키기.
        void  SetDisp (bool _bOn); //m_bShowForm 를 Set 시킨다.

        bool  IsErr     (void);  // 에러중인지 확인.
        int   GetLastErr(    );  // 멘마지막에 Set된 에러 번호 리턴.
        AnsiString GetLastErrName();
        AnsiString GetErrName  ( int _iNo);
        AnsiString GetErrAction( int _iNo);
        TRect GetTRect(TStringGrid * _sgGrid);
        //void  ChangeDnmVar();
        void  Load();
        void  Save();

        //For Display.
        void __fastcall DispErr        (TListBox    * _lbListBox );
        //void __fastcall SelLan         (bool          _bToTble   ,
        //                                TComboBox   * _cbLan     ); //랭귀지 선택.
        void __fastcall DisplayList    (TStringGrid * _sgGrid    ); //스트링 그리드 업데이트.
        void __fastcall DisplayProperty(bool         _bToTble    ,
                                        TStringGrid *_sgTable    ,
                                        TEdit       *_edEnum     ,
                                        TEdit       *_edErrNo    ,
                                        TEdit       *_edName     ,
                                        TEdit       *_edImgPath  ,
                                        TMemo       *_mmAction   ,
                                        TImage      *_imImage    ,
                                        TShape      *_spErrRect  ); //스트링 그리드 외에 것 업데이트.
        void __fastcall DisplayErrForm (TLabel      *_lbErrNo    ,
                                        TLabel      *_lbName     ,
                                        TMemo       *_mmAction   ,
                                        TImage      *_imErr      ,
                                        TShape      *_spErrRect  );
//        void SetLang(EN_LAN_SEL _iLang) { m_iLangSel = _iLang;}
};
//---------------------------------------------------------------------------
extern CErrMan EM;
#endif
