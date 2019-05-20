//---------------------------------------------------------------------------
#ifndef IOsH
#define IOsH

//---------------------------------------------------------------------------
#include <Grids.hpp>

#include "Axt_DIO.h"

//#include "Commisdf.h"

#include "Timer.h"
#include "TEdit.h"

#define MAX_SG_COL 7 +2

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
            int        iAdd      ; //�ּ�
            AnsiString sName     ; //�̸�
            AnsiString SComt     ; //����
            bool       bInv      ; //����
            int        iOnDelay  ; //�����ð�
            int        iOffDelay ; //�����ð�

            //Value.
            bool       bSetVal ; //��ɰ�
            bool       bGetVal ; //������
            bool       bUpEdge ; //������
            bool       bDnEdge ; //�ٿ��
        };

        SBit * m_pIn  ; 
        SBit * m_pOut ;

        CDelayTimer * m_pInOnDelay   ;
        CDelayTimer * m_pInOffDelay  ;
        CDelayTimer * m_pOutOnDelay  ;
        CDelayTimer * m_pOutOffDelay ;

        CAxtDIO AxtDIO ;

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
        void SetY  (int _iNo , bool _bVal , bool _bDirect = false); //Direct = true�ϰ�� TestMode������ ����� ������.
        bool GetY  (int _iNo , bool _bDirect = false);
        bool GetYDn(int _iNo );
        bool GetYUp(int _iNo );

        bool GetX  (int _iNo , bool _bDirect = false);              //Direct = true�ϰ�� 1����Ŭ ���� ������ ������.
        bool GetXDn(int _iNo );
        bool GetXUp(int _iNo );

        bool GetInModuleNo (int _iNo , int &_iModuleNo , int &_iOffset);
        bool GetOutModuleNo(int _iNo , int &_iModuleNo , int &_iOffset);

        void SetTestMode(bool _bOn) {m_bTestMode = _bOn;}
        bool GetTestMode(         ) {return m_bTestMode;}

        //Read/Write Para.
        void Load();
        void Save();

        //Load Save Dynamic Var.
        void LoadDnmVar();
        void SaveDnmVar();

        //Display.                                                    //�Լ����ο��� ������ �Ƚ�  //�ּҹ��� 16���� ǥ��.
        void DisplayList    (bool _bOutput , TStringGrid * _sgTable , bool _bFixedWidth = false );
        void UpdateStatus   (bool _bOutput , TStringGrid * _sgTable );
        void DisplayProperty(bool          _bTable      ,
                             int           _bOutput     ,
                             TEdit       * _edIndex     ,
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

