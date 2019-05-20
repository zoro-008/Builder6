//---------------------------------------------------------------------------
#ifndef ActuatorsH
#define ActuatorsH

//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <stdio.h>
#include <Io.h>


//---------------------------------------------------------------------------
#include "Timer.h"
#include "IOs.h"
#include "TEdit.h"
enum { ccBwd = 0 , ccFwd = 1};
//---------------------------------------------------------------------------
class CKaBit
{
    private :
        //Functions.

    protected :
        bool DifuFlag;
        bool Enabled;
        int  Axis;
        int  Tag;

    public  :
        bool On;
        bool Off;

    public  :
        __fastcall CKaBit (void);
        __fastcall ~CKaBit(void);

        void __fastcall Clear  (void             );
        void __fastcall Init   (void             );
        void __fastcall Out    (bool SeqCondition);
        void __fastcall Flk    (void             ); // Repeat On/Off.
        void __fastcall KeepOn (void             );
        void __fastcall KeepOff(void             );                                                  
        void __fastcall Set    (void             );
        void __fastcall Rst    (void             );                                                  
        void __fastcall Difu   (int Condition    );
};
                                                                                                     
//---------------------------------------------------------------------------                        
class TActuator : public CKaBit {                                                                    
    private:                                                                                         
        //String.                                                                                    
        AnsiString Name;
        AnsiString Comt;                                                                             

        //I/O Index.                                                                                 
        int   xfwdId   ;  //FWD Sensor Address.
        int   xbwdId   ;  //BWD Sensor Address.
        int   yfwdId   ;  //FWD Output Address.
        int   ybwdId   ;  //BWD Output Address.

        int   xfwdIndex;  //FWD Sensor Index No.
        int   xbwdIndex;  //BWD Sensor Index No.
        int   yfwdIndex;  //FWD Output Index No.
        int   ybwdIndex;  //BWD Output Index No.

        //Delay Time.                                                                                
        double FwdOnDelayTime     ;                                                                  
        double BwdOnDelayTime     ;                                                                  
        double FwdTimeOutDelayTime;                                                                  
        double BwdTimeOutDelayTime;

        //Timer.
        CDelayTimer FwdTimeOutTimer;
        CDelayTimer BwdTimeOutTimer;
        CDelayTimer FwdOnDelayTimer;
        CDelayTimer BwdOnDelayTimer;

        //
        int ThreadApplyTime;
        int iInv;            //Inverse Output.

        int iInitStat ;

    private:
        int LastOutCommand;

    protected:
        int  __fastcall Input (int n         );
        int  __fastcall Output(int n         );
        void __fastcall Output(int n , int on);

    public :
        bool ApplyTimeout;
        bool ApplyOutComplete;
        bool ApplyKillOutput;
        bool EnableLastOutCmdTOut;

        bool vfy,dfy, dfx, fx, vfx, odfx, ftoe, Ltftoe;
        bool vby,dby, dbx, bx, vbx, odbx, btoe, Ltbtoe;

    public :
        //Creator & Destroyer.
        __fastcall TActuator(void)  {Init(); }
        __fastcall ~TActuator(void) { }

        //Init.
        void __fastcall Init(void);

        //basic funcions.
        void       __fastcall Clear      (void           );
        void       __fastcall Update     (void           );
        void       __fastcall UpdateInput(void           );
        AnsiString __fastcall GetName    (void           ) { return Name; }
        void       __fastcall SetName    (AnsiString data) { Name = data; }
        AnsiString __fastcall GetComt    (void           ) { return Comt; }
        void       __fastcall SetComt    (AnsiString data) { Comt = data; }

        //status funtions.
        int  __fastcall GetLastCmd(void   ) { return LastOutCommand; }
        int  __fastcall Complete  (int Cmd); //1:On,Fwd,Up,Close,Rotate 0:Off,Bwd,Dw, Open.
        bool __fastcall Trg       (int Cmd); //Last Command.
        int  __fastcall Err       (void   );
        void __fastcall Rst       (void   );
        bool __fastcall GetTimeOut(void   );

        //run funtions.
        bool __fastcall Out(int Cmd);  //Force.
        int  __fastcall Run(int Cmd);  //Check Time Out.

        //ID funtions.
            //Set
        void __fastcall SetThreadApplyTime(int Time) { ThreadApplyTime = Time; }
        void __fastcall SetInv            (int Inv ) { iInv            = Inv ; }
        void __fastcall SetAllId          (int xf, int xb, int yf, int yb);
        void __fastcall SetxfwdId         (int n                         ) { xfwdId  = n; xfwdIndex = n;}
        void __fastcall SetxbwdId         (int n                         ) { xbwdId  = n; xbwdIndex = n;}
        void __fastcall SetyfwdId         (int n                         ) { yfwdId  = n; yfwdIndex = n;}
        void __fastcall SetybwdId         (int n                         ) { ybwdId  = n; ybwdIndex = n;}
        void __fastcall SetInitStat       (int Stat                      ) { iInitStat = Stat ;         }

            //Get
        int  __fastcall GetThreadApplyTime(void)  { return ThreadApplyTime; }
        int  __fastcall GetxfwdId         (void)  { return xfwdId;          }
        int  __fastcall GetxbwdId         (void)  { return xbwdId;          }
        int  __fastcall GetyfwdId         (void)  { return yfwdId;          }
        int  __fastcall GetybwdId         (void)  { return ybwdId;          }

        int  __fastcall GetxfwdIndex      (void)  { return xfwdIndex;       }
        int  __fastcall GetxbwdIndex      (void)  { return xbwdIndex;       }
        int  __fastcall GetyfwdIndex      (void)  { return yfwdIndex;       }
        int  __fastcall GetybwdIndex      (void)  { return ybwdIndex;       }
        int  __fastcall GetInv            (void)  { return iInv;            }
        int  __fastcall GetInitStat       (void)  { return iInitStat;       }

        //Set funtions.
            //Set.
        void   __fastcall SetFwdOnDelayTime     (double data) { FwdOnDelayTime      = data; }
        void   __fastcall SetBwdOnDelayTime     (double data) { BwdOnDelayTime      = data; }
        void   __fastcall SetFwdTimeOutDelayTime(double data) { FwdTimeOutDelayTime = data; }
        void   __fastcall SetBwdTimeOutDelayTime(double data) { BwdTimeOutDelayTime = data; }
        void   __fastcall SetApplyTimeout       (int    data) { ApplyTimeout        = data; }
        void   __fastcall SetApplyOutComplete   (int    data) { ApplyOutComplete    = data; }
            //Get.
        double __fastcall GetFwdOnDelayTime     (void)        { return FwdOnDelayTime;      }
        double __fastcall GetBwdOnDelayTime     (void)        { return BwdOnDelayTime;      }
        double __fastcall GetFwdTimeOutDelayTime(void)        { return FwdTimeOutDelayTime; }
        double __fastcall GetBwdTimeOutDelayTime(void)        { return BwdTimeOutDelayTime; }
        int    __fastcall GetApplyTimeout       (void)        { return ApplyTimeout;        }
        int    __fastcall GetApplyOutComplete   (void)        { return ApplyOutComplete;    }

        //Read/Write Para.
        void __fastcall Load(int Act , AnsiString Path);
        void __fastcall Save(int Act , AnsiString Path);
};

//-------------------------- TActuatorTable -------------------------
class CActuators
{
    protected :
        //TActuator * Actuator ;
        int m_iMaxAct  ;

    public :
       TActuator * Actuator ;
        //Basic functions.
        __fastcall CActuators(void) ;
        __fastcall ~CActuators(void);

        //Init.
        void __fastcall Init(void);
        void __fastcall ResetAll(void) { for (register int i = 0 ; i < m_iMaxAct ; i++) Rst(i); }
        void __fastcall Reset(int aNum) { Rst(aNum); }

        //Normal Control.
        int  __fastcall Run        (int aNum , int Act) { return Actuator[aNum].Run     (Act); }
        bool __fastcall Out        (int aNum , int Act) { return Actuator[aNum].Out     (Act); }
        bool __fastcall Trg        (int aNum , int Act) { return Actuator[aNum].Trg     (Act); }
        void __fastcall Rst        (int aNum          ) {        Actuator[aNum].Rst     (   ); }
        int  __fastcall Err        (int aNum          ) { return Actuator[aNum].Err     (   ); }
        int  __fastcall Complete   (int aNum , int Act) { return Actuator[aNum].Complete(Act); }

        //Cylinder Control.
        int         __fastcall GetLastCmd(int aNum          ) { return Actuator[aNum].GetLastCmd(   ); }
        bool        __fastcall Getdfx    (int aNum          ) { return Actuator[aNum].dfx            ; }
        bool        __fastcall Getdbx    (int aNum          ) { return Actuator[aNum].dbx            ; }
        bool        __fastcall GetCylStat(int aNum , int Act) { return Actuator[aNum].Complete  (Act); }
        AnsiString  __fastcall GetName   (int aNum          ) { return Actuator[aNum].GetName   (   ); }
        bool        __fastcall MoveCyl   (int aNum , int Act);

        //Update.
        void __fastcall Update(void);

        //Read/Write Para.
        void __fastcall Load();
        void __fastcall Save();
        void LoadDnmVar();
        void SaveDnmVar();

        //Display.
        void __fastcall DisplayList    (TStringGrid *Table);
        void __fastcall DisplayStatus  (int Index , TLabel   *FwdStat , TLabel  *BwdStat , TLabel *Alarm);
        void __fastcall DisplayButton  (int Index , TButton  *BwdStat , TButton *FwdStat);
        void __fastcall DisplayProperty(bool ToTble ,
                                        TEdit * EName , TEdit * EComt ,
                                        TEdit * EFx   , TEdit * EBx   ,
                                        TEdit * EFy   , TEdit * EBy   ,
                                        TEdit * EFd   , TEdit * EBd   ,
                                        TEdit * EFt   , TEdit * EBt   ,
                                        TEdit * ETo   , TEdit * EOc   ,
                                        TEdit * EIv   , TEdit * EIs   , TStringGrid *Table );
};

//Actuator Object.
extern CActuators AT;

//---------------------------------------------------------------------------
#endif

