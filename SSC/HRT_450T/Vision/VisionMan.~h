//---------------------------------------------------------------------------

#ifndef VisionManH
#define VisionManH
//---------------------------------------------------------------------------
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

#include "VisnDefine.h"

class CVisionMan
{
    public :
        CVisionMan();
        ~CVisionMan();

    protected :
        TTimer *tmYame;

        EN_VISN_ID m_iInspectionId ;

        TRetResult Rslt[MAX_VISN_ID];

    public :
        void __fastcall tmYameTimer(TObject *Sender);

        //비전 초기화 및 창 페이런트 설정.
        bool InitVisn   (void * hMainWnd , void * hParaWnd , AnsiString _sDevName);
        bool DelVisn    ();
        void SetImgForm (void * hImgBaseWnd , bool _bSettingMode);

        //잡파일 체인지.
        void LoadJobFile(bool _bLoad , AnsiString _sDevName);

        //카메라 관련.
        void Grab       (EN_VISN_ID _iVisnId );             //사진찍기.
        void Live       (EN_VISN_ID _iVisnId , bool _bOn);  //라이브 키기
        bool GetGrabEnd (EN_VISN_ID _iVisnId );             //사진찍은거 이미지 받기 끝났는지.

        //조명 관련.
        void SetLight   (EN_VISN_ID _iVisnId );             //조명 느릴경우 미리 조명 세팅 해놓기

        //검사 관련.
        void Inspect    (EN_VISN_ID _iVisnId , bool _bUseThread = true); //검사.
        bool GetInspEnd (EN_VISN_ID _iVisnId );                          //검사 끝??

        //결과값 받아오기.
        int  GetRslt    (EN_VISN_ID _iVisnId , TRetResult * _tRslt); //결과.

        //레벨 설정 및 오토런.
        void SetLevel   (EN_OPER_LEVEL _iLevel);
        void SetAutorun (bool _bAutorun);

        //비젼 탭 바꾸기.
        void SetActiveVisn(EN_VISN_ID _iVisnId);


};

extern CVisionMan VSM ;

#endif
