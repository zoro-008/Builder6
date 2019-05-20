//---------------------------------------------------------------------------

#ifndef ProbeComUnitH
#define ProbeComUnitH

#include "Array.h"
#include "UtilDefine.h"

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

//Master Chip Data.
extern EN_PROB_RSLT iMChipRslt[MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];
extern double       dMChipVf  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];
extern double       dMChipVr  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT];

class CProbeComUnit
{
    public:
        __fastcall  CProbeComUnit(void);
        __fastcall ~CProbeComUnit(void);

        void Init (void);
        void Close(void);

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

        int  iChipCnt   ;
        bool bUseZenner ;
        
        EN_PROB_RSLT iMapRslt[MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT] ;
        double       dMapVf  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT] ;
        double       dMapVr  [MAX_ARRAY_ROW][MAX_ARRAY_COL][MAX_CHIPCNT] ;

        bool __fastcall ReadMasterData    (EN_ARAY_ID _iId);
        void __fastcall WriteMasterDataCsv(EN_ARAY_ID _iId, bool _bDateFolder);
        String sArray[4];
        int    iArray[4][17];
};
//---------------------------------------------------------------------------
extern CProbeComUnit PCM;
//---------------------------------------------------------------------------
#endif
