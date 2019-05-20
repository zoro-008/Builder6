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
        void DelWindows();  //필요 없을듯.

    public :
        void __fastcall tmYameTimer(TObject *Sender);

        bool InitVisn(AnsiString _sDevName);
        bool SetVisnForm(void * _hWnd);
        bool DelVisn ();

        void LoadJobFile(bool _bLoad , AnsiString _sDevName);

        void Grab      (EN_VISN_ID _iVisnId );             //사진찍기.
        void Live      (EN_VISN_ID _iVisnId , bool _bOn);  //라이브 키기
        bool GetGrabEnd(EN_VISN_ID _iVisnId );             //사진찍은거 이미지 받기 끝났는지.

        void SetLight  (EN_VISN_ID _iVisnId );             //조명 느릴경우 미리 조명 세팅 해놓기

        void Inspect   (EN_VISN_ID _iVisnId , bool _bUseThread = true); //검사.
        bool GetInspEnd(EN_VISN_ID _iVisnId );                          //검사 끝??

        int  GetRslt   (EN_VISN_ID _iVisnId , TRetResult * _tRslt); //결과.
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


        //비젼 탭 바꾸기.
        void SetActiveVisn(int _iVisnId);


};

extern CVisionMan VSM ;









#endif
