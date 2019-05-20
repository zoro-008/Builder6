//---------------------------------------------------------------------------

#ifndef CsvUnitH
#define CsvUnitH
//---------------------------------------------------------------------------
class CCsv
{
    private:
        int m_iMaxCol ;
        int m_iMaxRow ;

        String ** sCell ;

    public:


        void   SetMaxColRow(int _iMaxCol , int _iMaxRow);
        String GetCell     (int _iCol , int _iRow);
        void   SetCell     (int _iCol , int _iRow , String _sCell);



};




#endif
