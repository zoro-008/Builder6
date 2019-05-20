//---------------------------------------------------------------------------

#ifndef GridDataH
#define GridDataH

#include <system.hpp>
//---------------------------------------------------------------------------
class CGridData
{
    public :
        CGridData();
        ~CGridData();

    private:
        int m_iMaxCol ;
        int m_iMaxRow ;

        int m_iTitleCol ;
        int m_iTitleRow ;

        String ** sCells ;

    public:

        void   ClearCells ();

        void   SetMaxColRow      (int    _iMaxCol   , int _iMaxRow);
        int    GetMaxCol         ();
        int    GetMaxRow         ();

        String GetCell           (int    _iCol      , int _iRow   );
        bool   SetCell           (int    _iCol      , int _iRow , String _sCell);

        bool   SetCellByColTitle (String _sColTitle , int _iRow , String _sCell);
        String GetCellByColTitle (String _sColTitle , int _iRow );

        bool   SetCellByRowTitle (String _sRowTitle , int _iCol , String _sCell);
        String GetCellByRowTitle (String _sRowTitle , int _iCol );

        bool   SetCellByColRowTitle (String _sColTitle , String _sRowTitle , String _sCell);
        String GetCellByColRowTitle (String _sColTitle , String _sRowTitle );

        void   SetTitleCol      (int _iCol);
        int    GetTitleCol      ();

        void   SetTitleRow      (int _iRow);
        int    GetTitleRow      ();




        bool   LoadFromCsv (String _sFilePath);
        bool   SaveToCsv   (String _sFilePath);
        bool   SaveToXls   (String _sFilePath);
};




#endif
