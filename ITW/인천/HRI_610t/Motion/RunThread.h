#ifndef RUNTHREADH
#define RUNTHREADH


template <typename T>
class CRunThread
{
    class TRunThread : public TThread
    {
        private:
            T Func;

        public:
            __fastcall TRunThread(T func) : TThread(false)
            {
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute()
            {
                Func();
            }
    };

    public:
        CRunThread(T func)
        {
            TRunThread*  thread = new TRunThread(func);
        }
};

/*  ** ����**
typedef void (__closure *Func)();
void __fastcall TForm1::Button3Click(TObject *Sender)
{
    Button3->Enabled = false ;
    CRunThread<Func> * func = new CRunThread<Func>(&TestFunc) ;
}

void TForm1::TestFunc()
{ 
    // ��û �ð��� �ɸ��� ��ƾ.
    int  k; 
    for(int c = 0; c < 299999999; c++)
    {
        k = c; 
        k++; 
        c = k; 
        c--; 
    } 
    Caption = k;
    Button3->Enabled = true;
}

*/

#endif






























































