//---------------------------------------------------------------------------
#ifndef IoStateH
#define IoStateH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <Buttons.hpp>

class CIoState
{
    public :
        CIoState();
        ~CIoState();

    protected :
        int m_iMaxIn  ;
        int m_iMaxOut ;

        struct SIo {
            AnsiString    sName  ;
            int           iIoAdd ;
        };

//        SIo* m_pIn ;
//        SIo* m_pOut;
//        TButton *m_pInButton ;
//        TButton *m_pOutButton;
        TPanel **m_pInPanel ;
        TPanel **m_pOutPanel;
        TTimer * tmInUpdate  ;
        TTimer * tmOutUpdate ;
        void __fastcall tmInUpdateTimer (TObject *Sender);
        void __fastcall tmOutUpdateTimer(TObject *Sender);
        void __fastcall pnDbClick       (TObject *Sender);

        void LoadDnmVar();




    public :
        void SetInPutWindow (TPanel * _pnBase , int _iFontSize = 10 , int _iRow = 8 ) ;
        void SetOutPutWindow(TPanel * _pnBase , int _iFontSize = 10 , int _iRow = 8 ) ;

};
extern CIoState IOS;

#endif
