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
            int        iAdd    ; //�ּ�
            AnsiString sName   ; //�̸�
            AnsiString SComt   ; //����
            bool       bInv    ; //����
            int        iDelay  ; //�����ð�

            //Value.
            bool       bSetVal ; //��ɰ�
            bool       bGetVal ; //������
            bool       bUpEdge ; //������
            bool       bDnEdge ; //�ٿ��
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
        void SetY  (int _iNo , bool _bVal , bool _bDirect = false); //Direct = true�ϰ�� 1����Ŭ ���� ������ ������.
        bool GetY  (int _iNo );
        bool GetYDn(int _iNo );
        bool GetYUp(int _iNo );

        bool GetX  (int _iNo , bool _bDirect = false);              //Direct = true�ϰ�� 1����Ŭ ���� ������ ������.
        bool GetXDn(int _iNo );
        bool GetXUp(int _iNo );

        //Read/Write Para.
        void Load();
        void Save();

        //Load Save Dynamic Var.
        void LoadDnmVar();
        void SaveDnmVar();

        //Display.                                                    //�Լ����ο��� ������ �Ƚ�  //�ּҹ��� 16���� ǥ��.
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

