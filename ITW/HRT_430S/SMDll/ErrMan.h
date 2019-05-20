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

        TErr * Err ;         //���� �迭 ������.
        int    m_iMaxErr   ; //�ѿ��� ����.
        int    m_iLastErr  ; //���� �������� �µ� ����.
        int    m_iUpdated  ; //���� �µǾ� �ִ� ���� ����.
        bool   m_bShowForm ; //false�϶��� ����â�� ����.

        void  LoadDnmVar();
        void  SaveDnmVar();

    public:
        void  Clear   (void     ); //��翡�� Ŭ���� ��Ű��.
        void  SetErr  (int  _iNo); //�ش� ���� true ��Ű��.
        void  SetDisp (bool _bOn); //m_bShowForm �� Set ��Ų��.

        bool  IsErr     (void);  // ���������� Ȯ��.
        int   GetLastErr(    );  // �ึ������ Set�� ���� ��ȣ ����.
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
        //                                TComboBox   * _cbLan     ); //������ ����.
        void __fastcall DisplayList    (TStringGrid * _sgGrid    ); //��Ʈ�� �׸��� ������Ʈ.
        void __fastcall DisplayProperty(bool         _bToTble    ,
                                        TStringGrid *_sgTable    ,
                                        TEdit       *_edEnum     ,
                                        TEdit       *_edErrNo    ,
                                        TEdit       *_edName     ,
                                        TEdit       *_edImgPath  ,
                                        TMemo       *_mmAction   ,
                                        TImage      *_imImage    ,
                                        TShape      *_spErrRect  ); //��Ʈ�� �׸��� �ܿ� �� ������Ʈ.
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
