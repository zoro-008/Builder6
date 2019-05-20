//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormUpdate.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmUpdate *FrmUpdate;
//---------------------------------------------------------------------------

__fastcall TFrmUpdate::TFrmUpdate(TComponent* Owner)
        : TForm(Owner)
{
/*
<SM>
- 작은창 버젼 합치기.
- 커미조아 붙여보기
-



<EM>
- 메세지 인자 전달 하게 수정.
- 타이머 워닝 창 추가.

<AT>
-




*/
//Home()  요건 없에고 DoHome만 남기기.



    Add("2014.01.05(1.3.4.1)선계원                                            ");
    Add(" 정규 DLL화 작업");
    Add(" SetErrMsg함수 추가 하여 에러에 메세지 삽입.");
    Add(" ");

    Add("2013.03.05(1.3.3.1)선계원                                            ");
    Add(" MT        Z상이용한 홈 삽질 하는 것 잡음 Z상 액티브래벨 바꿈        ");
    Add(" MT        가감속시간 모드일때 홈안잡히는것 수정                     ");
    Add("                                                                     ");
    Add("2012.11.23(1.3.2.1)선계원                                            ");
    Add(" MT        AXL만 Position Interrupt 추가.                            ");
    Add("                                                                     ");
    Add("                                                                     ");
    Add("2012.11.23(1.3.1.1)선계원                                            ");
    Add(" MT        가속시간 을 가속율도 선택 할수 있게                       ");
    Add("           GetMinPos ,GetMaxPos , GetRunVel , GetManVel 추가         ");
    Add(" AI        Analog Input Output 추가 테스트필요.                      ");
    Add("                                                                     ");
    Add("                                                                     ");
    Add("2012.10.09(1.2.6.1)선계원                                            ");
    Add(" FM        타이머 폼 추가 FM_MsgTime                                 ");
    Add("                                                                     ");
    Add("2012.09.03(1.2.5.1)선계원                                            ");
    Add(" MT        작동방향 디스플레이용 추가                                ");
    Add(" SD        AXL 컴파일 옵션으로 추가.                                 ");
    Add("                                                                     ");
    Add("2012.03.18(1.2.4.1)선계원                                            ");
    Add(" IO        아이오 아웃풋 딜레이 안먹는 버그 수정                     ");
    Add(" IO        아이오 디스프레이창에 업엦지 없애고 실제값 명령값으로수정 ");
    Add(" MOTOR     리밋센서 감지시 급정지로 바꿈.                            ");
    Add(" MOTOR     +리밋센서 감지시에도 홈 잡을수 있게 바꿈                  ");
    Add("                                                                     ");
    Add("2012.03.17(1.2.3.1)선계원                                            ");
    Add(" ERROR     에러리스트 Apply눌렀을때 리스트 변화안돼는 버그 수정      ");
    Add(" ACTUATOR  Enum추가                                                  ");
    Add(" IO        출력에도 On Off Delay 추가.                               ");
    Add(" MOTOR     홈플레그 서보 오프 시킬때 안죽는 것 수정.                 ");
    Add("           리피트 시간 무조건 default 1초로 먹는 것 수정.            ");
    Add("                                                                     ");
    Add("2012.03.07(1.2.2.1)선계원                                            ");
    Add("IO에 Enum추가 하고 화면 및 인터페이스에 GetName함수 추가.            ");
    Add("                                                                     ");
    Add("2011.11.07(1.2.1.1)선계원                                            ");
    Add("인터페이스유닛에 랭귀지받아오기 추가. 해더 업뎃해야함.               ");
    Add("EN_LAN_SEL __stdcall FM_GetLan (              );                     ");
    Add("                                                                     ");
    Add("기존에 언어가 에러에만 적용이 되던것을 아이오,실린더에도             ");
    Add("적용이 되게 수정.                                                    ");
    Add("마스터 탭에서 설정 하게 되어 있음.                                   ");
    Add("기존장비에 깔때 Ini앞에 K_ 수정 작업 해야함.                         ");
    Add("                                                                     ");
    Add("2011.09.05(김동우)                                                   ");
    Add("ACTUATOR Repeat 동작시 센서 미감지로 타임아웃적용 없앰.              ");
    Add("IO On/Off 딜레이 두개로 나눔.                                        ");
    Add("ACTUATOR MOTOR 조그 버튼 타입설정으로 방향성 부여함.                 ");
    Add("                                                                     ");
    Add("2011.08.31(1.1.2.2)                                                  ");
    Add("트리거 세팅 할때 뻑나던것 수정.                                      ");
    Add("IO주소 한라 포멧에 맞게 디스플레이되도록 수정                        ");
    Add("                                                                     ");
    Add("2011.07.25(1.1.2.0)                                                  ");
    Add("Error List 랭귀지 선택 가능 하게 수정.                               ");
    Add("                                                                     ");
    Add("2011.02.24(1.1.1.5)                                                  ");
    Add("Control 창에 Sol Time 테이블 별로 구분.                              ");

}
//---------------------------------------------------------------------------
void TFrmUpdate::Add(AnsiString _Str)
{
    mmUpdate -> Lines -> Add(_Str);
}





















































































































































void __fastcall TFrmUpdate::FormDestroy(TObject *Sender)
{
    Trace("","");        
}
//---------------------------------------------------------------------------

