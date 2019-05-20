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
            bool  bInspEnd ; //검사 끝남 변수.
            
            int   iRet   ;  //스펙 설정치 실패....및에 주석 참조. bRslt == true && iRet == 0 이면 Pass.
            float fRsltX ;  //보정 X값.
            float fRsltY ;  //보정 Y값.
            float fRsltT ;  //보정 T값.
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



        //핸들러에서 쓰는 함수군.
        bool InitVisn(AnsiString _sDevName);
        bool SetParentVisnMain(TPanel * _pBase);
        bool SetParentVisnPara(TPanel * _pBase);
        bool SetParentAlgoPara(TPanel * _pBase);
        bool DelVisn ();

        void LoadJobFile(bool _bLoad , AnsiString _sDevName);

        //비전아이디로 제어.
        void Grab      (EN_VISN_ID _iVisnId );             //사진찍기.
        void Live      (EN_VISN_ID _iVisnId , bool _bOn);  //라이브 키기
        void LiveAllOff(bool _bCheckGrabEnd = true);             //라이브 다끄기.
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

        void SetLevel  (int _iLevel);
        /*
        0 = Oper
        1 = Engineer
        2 = Master
        */
        void SetAutorun (bool _bAutorun);


        //비젼 탭 바꾸기.
        void SetActiveVisn(int _iVisnId);
        int  GetActiveVisn();

        void SetSettingMode(bool _bSettingMode);
        void SaveButtonClick();

        void SetWfrOri1WideInspection(bool _bWfrOri1WideInspection);
        void SetWfoOri1WideInspection(bool _bWfoOri1WideInspection);
};

extern CVisionMan VSM ;









#endif
