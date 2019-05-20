//---------------------------------------------------------------------------

#ifndef FormGraphH
#define FormGraphH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include "UtilDefine.h"
#include "RsltData.h"

//---------------------------------------------------------------------------


struct TGraphPara {
    double m_dLeftX    ; //dhlsW
    double m_dBottomY  ;
    double m_dRightX   ;
    double m_dTopY     ;
    double m_dStepX    ; //������ �Ѵ���
    double m_dStepY    ;
    double m_dScaleX   ; //�׷��� ������ ���� ������
    double m_dScaleY   ;
    double m_dBigStepY ; //������ ū �Ѵ���.
    int    m_iLOfsX    ;
    int    m_iROfsX    ;
    int    m_iTOfsY    ;
    int    m_iBOfsY    ;
};


class TFrmGraph : public TForm
{
    __published:	// IDE-managed Components
        TPanel *pnBase;
        TTimer *tmUpdate;
        TButton *Button1;
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);


    private: // User declarations



        int m_iIncDataCnt ;
        TPointData * m_pIncData ;

        int m_iDecDataCnt ;
        TPointData * m_pDecData ;

        //ȭ�� �޼��� ó��.
        TWndMethod OriginalProc;
        void __fastcall PanelProc(Messages::TMessage &msg);

        double GetXOnGraph(TRect _tRect , double _dData);
        double GetYOnGraph(TRect _tRect , double _dData);


        void __fastcall UpdateGraphBk(HDC _hDc ,TRect _tRect); //�׷��� ���� �׸���
        void __fastcall UpdateGraphDt(HDC _hDc ,TRect _tRect); //�׷��� ������ �׸���.   '

        int m_iStgId ;

        TWinControl * m_pOriParent ;
        TWinControl * m_pMaxParent ;


    public:		// User declarations

        __fastcall TFrmGraph(TComponent* Owner);

        TGraphPara Para ;
        void __fastcall UpdateView();

        void SetGraphPara(TGraphPara _tPara);

        //void SetIncData(TPointData * _pIncData , int _iCnt);
        //void SetDecData(TPointData * _pDecData , int _iCnt);

        //void SetIncDataString(String _sMsg);
        //void SetDecDataString(String _sMsg);

        void SetStgId(int _iStgId);

        void SetMaxParent(TWinControl * _pMaxParent);






};

//---------------------------------------------------------------------------
extern PACKAGE TFrmGraph *FrmGraph[MAX_STAGE_CNT];


//---------------------------------------------------------------------------
#endif
