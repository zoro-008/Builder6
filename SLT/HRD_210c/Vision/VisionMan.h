//---------------------------------------------------------------------------

#ifndef VisionManH
#define VisionManH
//---------------------------------------------------------------------------
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "Vision.h"




class CVisionMan
{
    public :
        CVisionMan();
        ~CVisionMan();

    protected :
        //TWinControl * pOriParent ;

        void * m_hParentWnd ;
        void * m_hOriParentWnd ;

        TTimer *tmYame;

        EN_VISN_ID m_iInspectionId ; 

        TRetResult Rslt[MAX_VISN_ID];

        void SaveImage();
        void Inspection();

        void MakeWindows();
        void DelWindows();  //�ʿ� ������.

    public :
        void __fastcall tmYameTimer(TObject *Sender);

        bool InitVisn(AnsiString _sDevName);
        bool SetVisnForm(void * _hWnd);
        bool DelVisn ();

        void LoadJobFile(bool _bLoad , AnsiString _sDevName);

        void Grab      (EN_VISN_ID _iVisnId );             //�������.
        void Live      (EN_VISN_ID _iVisnId , bool _bOn);  //���̺� Ű��
        bool GetGrabEnd(EN_VISN_ID _iVisnId );             //���������� �̹��� �ޱ� ��������.

        void SetLight  (EN_VISN_ID _iVisnId );             //���� ������� �̸� ���� ���� �س���

        void Inspect   (EN_VISN_ID _iVisnId , bool _bUseThread = true); //�˻�.
        bool GetInspEnd(EN_VISN_ID _iVisnId );                          //�˻� ��??

        int  GetRslt   (EN_VISN_ID _iVisnId , TRetResult * _tRslt); //���.
        /*
        viWfr = 0:Pass , 1:RCT Fail , 2: Line Length Fail, 3: Angle Fail
        viWfo = 0:Pass , 1:OCV Fail , 2: Pixel Cnt Fail
        viWfp = 0:Pass , 1:TSD Fail , 2: Pen Pixel Cnt Fail
        viCal = 0:Pass , 1:CAL Fail
        */

        void SetLevel  (EN_OPER_LEVEL _iLevel);
        /*
        0 = Oper
        1 = Engineer
        2 = Master
        */
        void SetAutorun (bool _bAutorun);


        //���� �� �ٲٱ�.
        void SetActiveVisn(int _iVisnId);


};

extern CVisionMan VSM ;









#endif
