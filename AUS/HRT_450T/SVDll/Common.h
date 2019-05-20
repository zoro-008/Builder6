//---------------------------------------------------------------------------

#ifndef CommonH
#define CommonH

#include <dstring.h>

extern AnsiString g_sErrMsg ;

typedef void (__closure *Func)();
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
            //thread -> WaitFor();
            //thread -> Free();
        }
};







/*


template <typename T>
class CThread
{
    class TRunThread : public TThread {
        private:
            T Func;
        public:
            __fastcall TRunThread(T func) : TThread(false) {  // CreateSuspended항목 만듬과 동시에 쓰레드 런...
                FreeOnTerminate = false;
                Func = func;
            }
            void __fastcall Execute() {
                Func();
            }
    };

    public:
        CThread(T func) {

                TRunThread*  thread = new TRunThread(func);
                thread -> WaitFor();
                thread -> Free();

                //delete thread ;
        }
};


struct TMultiRange {
    int iStart ;
    int iEnd   ;
    int iStep  ;
};
template <typename T>
class CMultiThread
{
    class TRunThread : public TThread {
        private:
            T m_pFunc;
        public:
            __fastcall TRunThread(T _pFunc) : TThread(false) {  // CreateSuspended항목 만듬과 동시에 쓰레드 런...
                FreeOnTerminate = false;
                m_pFunc     = _pFunc;
            }
            void __fastcall Execute() {
                m_pFunc();
            }
    };

    protected:

    public:
        CMultiThread(const int _iFuncCnt,T * _pFunc){
            //int * pInt[_iFuncCnt] ;
            TRunThread ** pThread ;//[_iFuncCnt] ;

            pThread = new TRunThread*[_iFuncCnt];


            for(int i = 0 ; i < _iFuncCnt ; i++) {

                pThread[i] = new TRunThread(_pFunc[i]);
            }

            for(int i = 0 ; i < _iFuncCnt ; i++) {
                pThread[i] -> WaitFor();
                delete pThread[i] ;
            }

            delete [] pThread ;
        }
        ~CMultiThread(){
        }

};


class CMultiThread1
{
    class TRunThread : public TThread {
        private:
            T m_pFunc;
            U m_tFuncPara ;
            TMultiRange m_tRange ;
        public:
            __fastcall TRunThread(T _pFunc , TMultiRange _tRange , U _tFuncPara) : TThread(false) {  // CreateSuspended항목 만듬과 동시에 쓰레드 런...
                FreeOnTerminate = false;
                m_pFunc     = _pFunc;
                m_tFuncPara = _pFuncPara ;
                m_tRange    = _tRange ;
            }
            void __fastcall Execute() {
                m_pFunc(_tRange , m_tFuncPara);
            }
    };

    protected:
        int   m_iFuncCnt    ;
        bool *m_bFinished   ;
        int   m_iCrntFuncCnt;

    public:
        CMultiThread(int _iFuncCnt,T _pFunc[] , TMultiRange _tRange[] , U _tFuncPara[]){

            TRunThread*  pThread[_iFuncCnt] ;

            for(int i = 0 ; i < _iFuncCnt ; i++) {
                pThread[i] = new TRunThread(_pFunc[i] , _tFuncPara[i]);
            }

            for(int i = 0 ; i < _iFuncCnt ; i++) {
                pThread[i] -> WaitFor();
            }

            delete [] pThread ;
        }
        ~CMultiThread(){
        }

};  */

/*
int a = 100;
    pthread_t thread_t;
    int status;
 
    // 쓰레드를 생성한다. 
    if (pthread_create(&thread_t, NULL, test, (void *)&a) < 0)
    {
        perror("thread create error:");
        exit(0);
    }
 
    // 쓰레드가 종료되기를 기다린후 
    // 쓰레드의 리턴값을 출력한다. 
    pthread_join(thread_t, (void **)&status);
    printf("Thread End %d\n", status);
    return 1;
*/
//CRunThread<Func> * func = new CRunThread<Func>(&Rotate2);


/*
class CMultiThread
{
    CMultiThread()
    {
        SYSTEM_INFO tInfo ;
        GetSystemInfo(&tInfo);

        bool   * pRqstStop = new bool  [tInfo.dwBumberOfProcessors];
        HANDLE * hThread   = new HANDLE[tInfo.dwBumberOfProcessors];

        for(int i = 0 ; i < tInfo.dwNumberOfProcessors ; i++) {
            pRqstStop[i] = false ;




        }





    }






};*/


/*

typedef void (__closure *MultiFor)(int _iStart , int _iEnd );
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
*/






//---------------------------------------------------------------------------
#endif


