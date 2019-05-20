//---------------------------------------------------------------------------

#ifndef ProbeComUnitH
#define ProbeComUnitH

#include "Array.h"
#include "UtilDefine.h"
#include "uRs232C.h"

#define PROBE_CH1 1
#define PROBE_CH2 2

enum EN_PROB_RSLT
{
    prReady = -1 ,  //�˻��������.
    prOk    =  0 ,  // �˻� Good
    prVFNg  =  1 ,  // VF Fail
    prVRNg  =  2 ,  // VR Fail
    prVFVRNg=  3 ,  // VF VR Fail
    prDRNg  =  4 ,  // Drop Fail.

    MAX_PROB_RSLT
};

#define MAX_CONTACT_COL 4
#define MAX_CHIPCNT 3

//�޴��� ���۽ÿ� ȭ�鿡 ���÷����ϴ� �ѹ� ���� �������� ������.
extern EN_PROB_RSLT iCChipRsltCrnt[MAX_ARRAY_ROW][MAX_CONTACT_COL][MAX_CHIPCNT] ;
extern double       dCChipVfCrnt  [MAX_ARRAY_ROW][MAX_CONTACT_COL][MAX_CHIPCNT] ;
extern double       dCChipVrCrnt  [MAX_ARRAY_ROW][MAX_CONTACT_COL][MAX_CHIPCNT] ;

//���� �۾� ���� ��Ʈ���� ������
extern EN_PROB_RSLT iCChipRslt[MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];
extern double       dCChipVf  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];
extern double       dCChipVr  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];

//���� �۾� �ߴ� ��Ʈ���� ������.
extern EN_PROB_RSLT iPChipRslt[MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];
extern double       dPChipVf  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];
extern double       dPChipVr  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];

class CProbeComUnit
{
    public:
        __fastcall  CProbeComUnit(void);
        __fastcall ~CProbeComUnit(void);

        void Init() ;
        void Close();

    private:
        int m_iMaxRow ;
        int m_iMaxCol ;
        void __fastcall WriteProbeDataIni (EN_ARAY_ID _iId);
//        void __fastcall WriteProbeDataCsv (EN_ARAY_ID _iId);
        void __fastcall DelPastData       (               );

    public:
        void __fastcall WriteProbeDataCsv (EN_ARAY_ID _iId);
        void __fastcall WritePastProbeDataCsv(AnsiString sPath);        
        void __fastcall SetMaxColRow   (int c,int r) { m_iMaxCol = c; m_iMaxRow = r;}
        int  __fastcall GetMaxRow      (void ) { return  m_iMaxRow ;}
        int  __fastcall GetMaxCol      (void ) { return  m_iMaxCol ;}

        void __fastcall ClearVf        () { memset(&dCChipVf, 0 , sizeof(double)*MAX_ARRAY_ROW*MAX_ARRAY_COL );
                                            memset(&dCChipVr, 0 , sizeof(double)*MAX_ARRAY_ROW*MAX_ARRAY_COL ); }
        void __fastcall ShiftArrayData (EN_ARAY_ID _iId);
        void __fastcall CChip (int iC,int iR,int iChip,double dVf,double dVr = 0);

        void __fastcall ReadProbeData  (AnsiString sPath);

        bool __fastcall Start(int iCh = 0);
        void __fastcall Set         ();
        void __fastcall SetDaegyeom ();
        bool __fastcall GetDataCh   (EN_ARAY_ID aiWhere , AnsiString sMsg , int _iCh , bool _bRpt = false);
        bool __fastcall CheckDataCnt(EN_ARAY_ID aiWhere , AnsiString sMsg);

        void __fastcall ClearDataCrnt();
        void __fastcall ClearData    ();
        void __fastcall ClearPstData ();

        String sArray[8];
        int    iArray[8][17];

        //CallBack ��� Rs232
        static void HeatProbe1Ch(DWORD _cbInQue);
        static void HeatProbe2Ch(DWORD _cbInQue);

        bool bPb1Rslt ;
        bool bPb2Rslt ;
        AnsiString sPb1Msg ;
        AnsiString sPb2Msg ;

        String sPb1Bin[2] ;
        String sPb2Bin[2] ;
        String sPb1Vf [2] ;
        String sPb2Vf [2] ;
};
//---------------------------------------------------------------------------
extern CProbeComUnit PCM;

extern TRS232C *RS232C_Pb1ch  ;
extern TRS232C *RS232C_Pb2ch  ;

//---------------------------------------------------------------------------
#endif
