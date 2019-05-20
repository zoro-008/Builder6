//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormUpdate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmUpdate *FrmUpdate;
//---------------------------------------------------------------------------

__fastcall TFrmUpdate::TFrmUpdate(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
/*
2011.09.22(1.2.4)
에러 카운트 검색 가능하게 수정
카운트 클리어 버튼 오퍼레벨에서 못쓰게 수정.
토출량 측정용 토출 버튼에 횟수 설정할 수 있게 수정.
바코드를 찍으면 자동으로 동작 하게 수정.
재조정시에 APC 조정 할 건 지 말건지 옵션 처리.

//구형 z y in 오리엔탈 crk523hpmbpb
//2b4082

2015.03.17
NO 납품일  EqipSerial 특이사항	                    메인보드 모델	          Serial USB Convertor	          z모터분해능 Y모터분해능 X,Y모터모델  In모터모델
1  2011.02 11-1400               	            GA-P85-D3	                  다모일	                  10000	      10000       crk523hpmbpb crk523hpmbpb
2  2012.05 12-1640    RightUpDown 스위치불량	    PA65-UD3-B3(슬롯순서왼쪽우선) 강원전자 파란색 E-B032-02-0646  2000	      1000        MD5-ND14     crk523hpmbpb
3  2011.02 11-1399    PC파워가 잘안켜짐.	    Supermicro CS2BC-Q            다모일                          10000	      10000       crk523hpmbpb crk523hpmbpb
4  2012.01 12-1620                                  PA65-UD3-B3(슬롯순서왼쪽우선) 알수없는 검은색	          2000	      1000        MD5-ND14     crk523hpmbpb
5  2012.05 12-1638                                  GA-P75-D3(슬롯순서오른쪽우선) 강원전자 파란색 E-B032-02-0646  2000	      1000        MD5-ND14     crk523hpmbpb
6  2011.08 11-1572    CD드라이버이상	            Supermicro CS2BC-Q            알수없는 검은색	          10000	      10000       crk523hpmbpb crk523hpmbpb
7  2011.08 11-1573    오른쪽스테이지 Y축 +리밋 고장 Supermicro CS2BC-Q            알수없는 검은색	          10000	      10000       crk523hpmbpb crk523hpmbpb
8  2011.08 11-1575             	                    Supermicro CS2BC-Q            알수없는 검은색	          10000	      10000       crk523hpmbpb crk523hpmbpb
9  2012.05 12-1639             	                    Supermicro CS2BC-Q            강원전자 파란색 E-B032-02-0646  2000	      1000        MD5-ND14     crk523hpmbpb
10 2011.08 11-1574                   	            Supermicro CS2BC-Q            알수없는 검은색	          10000	      10000       crk523hpmbpb crk523hpmbpb
11 2012.01 12-1619    Ground작업필요	            GA-P85-D3                     알수없는 검은색	          2000	      1000        MD5-ND14     crk523hpmbpb

일단 오늘 11대 설비 모터 분해능 통일
3개다 드라이버 똑같은 모델은 모든드라이버 왼쪽에 있는 딥스위치중에3번만 왼쪽에 가있게 해야되고 그상태에서 Data1이337337 Data2는 0
2개는 오토닉스 1개만 오리엔탈인경우 오토닉스껀 분해능 하프(밑으로)로 설정 하고 오리엔탈 딥스위치는 모두 오른쪽.

     분해능  리드 가속 속도
Z축  2000    0.25 0.2  1
Y축  1000    1    0.2  5
In축 10000   6    0.2  20









*/
