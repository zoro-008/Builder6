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

        struct TRetResult {
            bool  bInspEnd ; //�˻� ���� ����.
            
            int   iRet   ;  //���� ����ġ ����....�׿� �ּ� ����. bRslt == true && iRet == 0 �̸� Pass.
            float fRsltX ;  //���� X��.
            float fRsltY ;  //���� Y��.
            float fRsltT ;  //���� T��.
        };

    protected :
        //TWinControl * pOriParent ;
        TTimer *tmTrackerMoved ;

        EN_VISN_ID m_iInspectionId ;

        bool m_bWfrOri1WideInspection ;
        bool m_bWfoOri1WideInspection ;

        TRetResult Rslt[MAX_VISN_ID];
        /*
        viWfr = 0:Pass , 1:RCT Fail , 2: ChipSize , 3: Chip Angle
        viWfo = 0:Pass , 1:OCV Fail , 2: Sinc
        viWfp = 0:Pass , 1:TSD Fail , 2: Px Cnt
        viCal = 0:Pass , 1:CAL Fail ,
        */
        void SaveImage();
        void Inspection();

    public :
        void __fastcall tmYameTimer(TObject *Sender);
        void __fastcall tmTrackerMovedTimer(TObject *Sender);


        bool SetRectCenter();



        //�ڵ鷯���� ���� �Լ���.
        bool InitVisn(AnsiString _sDevName);
        bool SetParentVisnMain(TPanel * _pBase);
        bool SetParentVisnPara(TPanel * _pBase);
        bool SetParentAlgoPara(TPanel * _pBase);
        bool DelVisn ();

        void LoadJobFile(bool _bLoad , AnsiString _sDevName);

        //�������̵�� ����.
        void Grab      (EN_VISN_ID _iVisnId );             //�������.
        void Live      (EN_VISN_ID _iVisnId , bool _bOn);  //���̺� Ű��
        void LiveAllOff(bool _bCheckGrabEnd = true);             //���̺� �ٲ���.
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

        void SetLevel  (int _iLevel);
        /*
        0 = Oper
        1 = Engineer
        2 = Master
        */
        void SetAutorun (bool _bAutorun);


        //���� �� �ٲٱ�.
        void SetActiveVisn(int _iVisnId);
        int  GetActiveVisn();

        void SetSettingMode(bool _bSettingMode);
        void SaveButtonClick();

        void SetWfrOri1WideInspection(bool _bWfrOri1WideInspection);
        void SetWfoOri1WideInspection(bool _bWfoOri1WideInspection);
};

extern CVisionMan VSM ;









#endif
