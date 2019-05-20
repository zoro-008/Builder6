//---------------------------------------------------------------------------
#ifndef ErrManH
#define ErrManH

//---------------------------------------------------------------------------
class CErrMan
{
    public:
          CErrMan();
         ~CErrMan();

    protected:
        struct TErr {
            bool       m_bOn   ;
            AnsiString m_sName ;
            int        m_iPart ;
            AnsiString m_sCmmt ;
            AnsiString m_sSolut;

            AnsiString m_sSetTime ;

            int        m_iOcrCnt  ; //¹ß»ý È½¼ö.
        };

        TErr * Err ;
        int    m_iMaxErr  ;
        int    m_iLastErr ;

    public:
        void  Clear (void    );   void ClearPart(int _iPart) ;
        void  SetErr(int _iNo);

        bool  IsErr     (void);       bool       IsPartErr ( int _iPart ) ;
        int   GetLastErr(    );
        AnsiString GetLastErrName();  AnsiString GetErrName( int _iNo   ) ;
        AnsiString GetErrCmnt(int _iNo );

        int   GetOcrCnt   (int _iNo);
        void  SetOcrCnt   (int _iNo , int _iCnt);
        void  ClearOcrCnt (void    );
        int   GetMaxErrCnt(void    );

        void  LoadDnmVar();
        void  SaveDnmVar();

        void  Load();
        void  Save();

        bool  m_bShowForm ;
};
//---------------------------------------------------------------------------
extern CErrMan EM;
#endif
