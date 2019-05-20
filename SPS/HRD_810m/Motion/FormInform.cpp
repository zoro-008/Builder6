//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormInform.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmInform *FrmInform;
//---------------------------------------------------------------------------
__fastcall TFrmInform::TFrmInform(TComponent* Owner)
        : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;

}
//---------------------------------------------------------------------------






void TFrmInform::Show(int _iTag)
{
    mmExplain->Lines->Clear();


         if(_iTag ==  0) {mmExplain->Lines->Add("MAP FILE 형식입니다. 항목 중 '그림형 맵파일'은 위에 화면과 같이 웨이퍼 모양이 있는"          );
                          mmExplain->Lines->Add("MAP FILE이고 '*.WMAP' '*LOG2' 항목들은 좌표형식의 MAP FILE 입니다."                          );
                          pcExplain->ActivePage=tsAllMap;}
    else if(_iTag ==  1) {mmExplain->Lines->Add("MAP FILE에 GOOD 데이터 입니다."                                                              );
                          mmExplain->Lines->Add("위에 화면과 같은 경우는 GOOD 데이터 '1'을 입력하시면 됩니다."                                );
                          pcExplain->ActivePage=tsAllMap;}
    else if(_iTag ==  2) {mmExplain->Lines->Add("MAP FILE에 MASK 데이터 입니다. 좌측 화면은 MASK 데이터가 없고 우측 화면은 MASK "             );
                          mmExplain->Lines->Add("데이터가 있습니다. MASK가 좌측과 같이 없을 때는 '~' 우측과 같이 있을 때는 '*'을 입력."       );
                          pcExplain->ActivePage=tsMask;}
    else if(_iTag ==  3) {mmExplain->Lines->Add("MAP FILE에 EMPTY 데이터 입니다. 좌측 화면은 EMPTY 데이터가 있고 우측 화면은 EMPTY"           );
                          mmExplain->Lines->Add("데이터가 없습니다. EMPTY가 좌측과 같이 있을 때는 '.' 우측과 같이 없을 때는 '~'을 입력."      );
                          pcExplain->ActivePage=tsEmpty;}
    else if(_iTag ==  4) {mmExplain->Lines->Add("MAP FILE에 MapData를 가져 올 줄을 입력합니다. 위 MAP FILE과 같은 경우는"                     );
                          mmExplain->Lines->Add("웨이퍼 모양이 8번 줄 부터 시작 하므로 '8'을 입력 하시면 됩니다."                             );
                          pcExplain->ActivePage=tsAllMap;}
    else if(_iTag ==  5) {mmExplain->Lines->Add("MAP FILE에 MapData를 가져 올 칸을 입력합니다. 위 MAP FILE과 같은 경우는 "                    );
                          mmExplain->Lines->Add("웨이퍼 모양이 1번 칸 부터 시작 하므로 '1'울 압력 하시면 됩니다."                             );
                          pcExplain->ActivePage=tsAllMap;}
    else if(_iTag ==  6) {mmExplain->Lines->Add("MAP FILE 데이터를 화면에 나타 낼때 돌려주는 각도 입니다."                                    );
                          mmExplain->Lines->Add("장비에 들어간 실제 웨이퍼 모습과 맵데이터 모습을 일치 시켜주시면 됩니다."                    );
                          pcExplain->ActivePage=tsAllMap;}
    else if(_iTag ==  7) {mmExplain->Lines->Add("기준칩부터 작업을 하면 반대편 끝에서는 칩 얼라인이 틀어 질수도 있습니다. 다음 옵션은"        );
                          mmExplain->Lines->Add("기준칩에서 작업하러 이동 할 때 간격 만큼 이동 하면 중간 기준을 잡아주는 옵션입니다."         );
                          mmExplain->Lines->Add("EX) COL 간격 카운트가 3이면 칩을 3칸 COL 이동 하고 중간 기준을 잡아 줍니다."                 );
                          pcExplain->ActivePage=tsCntOption;}
    else if(_iTag ==  8) {mmExplain->Lines->Add("기준칩부터 작업을 하면 반대편 끝에서는 칩 얼라인이 틀어 질수도 있습니다. 다음 옵션은"        );
                          mmExplain->Lines->Add("기준칩에서 작업하러 이동 할 때 간격 만큼 이동 하면 중간 기준을 잡아주는 옵션입니다."         );
                          mmExplain->Lines->Add("EX) ROW 간격 카운트가 5이면 칩을 5칸 ROW 이동 하고 중간 기준을 잡아 줍니다."                 );
                          pcExplain->ActivePage=tsCntOption;}
    else if(_iTag ==  9) {mmExplain->Lines->Add("펜 마킹을 카운트 만큼 하고 비전으로 검사하는 옵션입니다."                                    );
                          mmExplain->Lines->Add("EX) 카운트가 5면 5번 찍고 비전 검사 합니다."                                                 );
                          pcExplain->ActivePage=tsCntOption;}
    else if(_iTag == 10) {mmExplain->Lines->Add("웨이퍼 기준칩을 매칭 하는 방법입니다. 'FIDUCIAL매칭'은 FIDUCIAL칩을 매칭 시키는 방법"        );
                          mmExplain->Lines->Add("이고 'CHIP 매칭'은 칩 모양 테두리를 매칭 시키는 방법입니다. 화면 상단에 사진은        "      );
                          mmExplain->Lines->Add("FIDUCIAL칩의 모양이고 화면 하단에 사진은 일반 칩 모양 입니다. FIDUCIAL칩이 존재 하면 "       );
                          mmExplain->Lines->Add("'FIDUCIAL매칭' 없으면 'CHIP 매칭'을 선택 합니다."                                            );
                          mmExplain->Lines->Add("기준칩 검사 자동은 프로그램이 자동 매칭 시키고 수동은 엔지니어가 눈으로 확인 후 동작 합니다.");
                          pcExplain->ActivePage=tsMatch;}
    else if(_iTag == 11) {mmExplain->Lines->Add("웨이퍼 기준칩을 매칭 하는 방법입니다. 'FIDUCIAL매칭'은 FIDUCIAL칩을 매칭 시키는 방법"        );
                          mmExplain->Lines->Add("이고 'CHIP 매칭'은 칩 모양 테두리를 매칭 시키는 방법입니다. 화면 상단에 사진은        "      );
                          mmExplain->Lines->Add("FIDUCIAL칩의 모양이고 화면 하단에 사진은 일반 칩 모양 입니다. FIDUCIAL칩이 존재 하면 "       );
                          mmExplain->Lines->Add("'FIDUCIAL매칭' 없으면 'CHIP 매칭'을 선택 합니다."                                            );
                          mmExplain->Lines->Add("기준칩 검사 자동은 프로그램이 자동 매칭 시키고 수동은 엔지니어가 눈으로 확인 후 동작 합니다.");
                          pcExplain->ActivePage=tsMatch;}
    else if(_iTag == 12) {mmExplain->Lines->Add("웨이퍼맵에 테두리 펜마킹영역이 없는 경우 수동으로 테두리 마킹 영역을 설정하는 수치 입니다."  );
                          mmExplain->Lines->Add("위와 같은 웨이퍼의 경우 2줄 마킹을 한경우이므로 2를 입력 하시면 됩니다."                     );
                          pcExplain->ActivePage=tsCntOption;}

    TForm::Show();
}
