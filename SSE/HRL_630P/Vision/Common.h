//---------------------------------------------------------------------------

#ifndef CommonH
#define CommonH
#include <Classes.hpp>

#include "VisnDefine.h"

//����Ʈ �ݹ�...
typedef void (__closure * PaintCallbackFunc)();
typedef void (__closure * InspectionFunc   )();

//�� ������
template <typename T>
class CRunThread
{
    class TRunThread : public TThread {
        private:
            T Func;
        public:
            __fastcall TRunThread(T func) : TThread(false) {
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute() {
                Func();
            }
    };

    public:
        CRunThread(T func) {
            TRunThread*  thread = new TRunThread(func);
        }
};


//���� ������ -----------------------------------------------------------------
extern AnsiString g_sJobFileName ;
extern AnsiString g_sJobFilePath ;

//���� ��� ����.
extern EN_VISN_ID       g_iActivVisnId      ;
extern bool             g_bSettingMode      ; //���� ����â �������.
extern EN_TRAIN_PAGE_ID g_iActivTrainPageId ; //���� ����â���� � â �������.
extern EN_OPER_LEVEL    g_iCrntLevel        ; //���� ����.

//ȭ�� �߸�
extern int g_iYameRet ;



//������Ʈ�� �ٲ�� ����.
//==============================================================================
//���� ��Ʈ�ѷ� ����.----------------------------------------------------------
enum EN_LIGHT_ID {
    liAll = 0 ,    //3ä�� 1���� �پ�.

    MAX_LIGHT_ID
};


//ī�޶��� ����.---------------------------------------------------------------
enum EN_CAM_ID {
    ciHed  = 0 , //ī�޶� 1�� ����. 1600*1200 8��Ʈ.

    MAX_CAM_ID
};


//Ķ���극�̼� ����.-----------------------------------------------------------
enum EN_CAL_ID {
    cdNone =-1 , //Ķ�� �Ⱦ��� ����.  Ķ ����.

    MAX_CAL_ID
};

#endif
