/***************************************************************************/
/* Class: CJclOnDelay                                                      */
/* Create: 2003/08/                                                        */
/* Developer: T K Lee                                                      */
/* Note: 2010/12 SUN Kye Won Modified                                      */
/***************************************************************************/
#ifndef TIMERH
#define TIMERH

#include <WINDOWS.H>

//---------------------------------------------------------------------------
double __fastcall GetTickTime(void)
{

    LARGE_INTEGER liEndCounter,liFrequency ;
    ::QueryPerformanceCounter  (&liEndCounter);
    ::QueryPerformanceFrequency(&liFrequency );

    return ( ((double)liEndCounter.QuadPart / (double)liFrequency.QuadPart) * 1000.0 );
 //return 0.0 ;
}

class CDelayTimer
{
    protected :
    	bool   m_bScan       ; //0:count사용 1:GetTickTime사용.
    	double m_dPreTickTime;

    public :
    	bool   m_bOut    ;
    	double m_dSetTime; 
    	double m_dCurTime; 

    public :
    	//Creator.
    	CDelayTimer(bool _bScan = false) { m_bScan = _bScan; Clear(); }
        //Clear.
    	void Clear (void) { OnDelay(false , 0); }

        //OnDelay Function.
    	bool OnDelay(bool _SeqInput, DWORD _dwSetTime) { m_dSetTime = _dwSetTime; return OnDelay(_SeqInput); }
    	bool OnDelay(bool _SeqInput) {
    		double dCurTime = GetTickTime();
    		if (_SeqInput) {
    			if (m_bScan) m_dCurTime++;
    			else	     m_dCurTime += dCurTime - m_dPreTickTime;
    			if (m_dCurTime >= m_dSetTime) { m_dCurTime = m_dSetTime; m_bOut = true ; }
    			else                                                     m_bOut = false;
                }
    		else {
    			m_dCurTime = 0;
    			m_bOut = false;
    		    }
    		m_dPreTickTime = dCurTime;
    		return m_bOut;
    	}
};

class CCycleTimer
{
    protected :
    	double m_dPreTickTime;

    public :
    	//Creator.
		CCycleTimer() { Clear(); }
        //Clear.
    	void Clear  (void) { m_dPreTickTime = GetTickTime() ;}

        //Function.
    	double OnCheckCycle() {
    	    double dCurTick = GetTickTime();
	    double dRetTime ;
   	    dRetTime = dCurTick - m_dPreTickTime ;

            m_dPreTickTime = dCurTick;
    	    return dRetTime;
    	}
};

#endif
