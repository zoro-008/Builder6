//---------------------------------------------------------------------------

#ifndef UnitVisionH
#define UnitVisionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
#include "PkgCamera.h"

class CVision;
class CThreadInspect : public TThread
{            
    private:
        CVision * m_pVision ;
    protected:
        void __fastcall Execute();
    public:
        __fastcall CThreadInspect(CVision * _pVision, bool CreateSuspended) : TThread(CreateSuspended)
        {
            m_pVision = _pVision;
        }
};
//---------------------------------------------------------------------------


class CVision : public CBase
{
    friend class CThreadInspect;

    public : //�ʱ�ȭ �Լ�.
        CVision(){ }
        virtual ~CVision(){}

        bool Init();
        bool Close();

    public : //struct
        enum EN_PKG_KIND {
            pkCamera_V01    = 0 ,
            pkMatch_V01     = 1 ,
            pkThreshold_V01 = 2 ,

            MAX_PKG_KIND
        };


        struct TTime { //ms
            int iGrab ;
            int iInsp ;
            TTime(){
                iGrab = 0 ;
                iInsp = 0 ;
            }
        };

        struct TStat {
            bool bGrabbing ;
            bool bInspectionning ;
            TStat(){
                bGrabbing       = false ;
                bInspectionning = false ;
            }

        };

        struct TRqst {
            bool bGrab ;
            bool bInsp ;
            bool bGrabInsp ;

            bool bTrainInsp  ;
            int  iTrainInspNo; //Ư���˻縸 �ϰ� �ö�...

            TRqst(){
                bGrab     =false ;
                bInsp     =false ;
                bGrabInsp =false ;

                bTrainInsp=false ;
            }
        };

    protected : //Member
        TTime Time ;
        TStat Stat ;
        TRqst Rqst ;

        CPkgBase    * m_pActivePkg  ; //Ʈ���νÿ� ������.PKG
        void        * m_hSendMsgWnd ;
        CCamera_V01 * m_pCamPkg     ;

    protected : //�޼��� ��Ƽ�� â���� ����.
        void   SendPaintMsg(){
            if(m_hSendMsgWnd)PostMessage(m_hSendMsgWnd , WM_PAINT , 0 , 0 );
        }
//==============================================================================
    protected :
        String m_asPkgNameTable[MAX_PKG_KIND];
        CLinkedList<CPkgBase *> m_lPkg ;
        String                  m_sCrntJobFilePath ;
        CValueList *            m_pValueList ; //�˻���� ����� ����Ʈ.
        CRITICAL_SECTION        m_csPkgList  ; //UI������� CThreadInspect ���� ���� ���� �ϸ� �ȉ´�.


    public :
        //List �ڵ鸵 ����.
        bool Add         (            String _sName    , String _sPkgName );
        bool Insert      (int _iIdx , String _sName    , String _sPkgName );
        bool MoveUp      (int _iIdx                                       );
        bool MoveDown    (int _iIdx                                       );
        bool Delete      (int _iIdx                                       );
        bool DeleteAll   (                                                );

        //
        CValueList              * GetValueList(        ){return m_pValueList;}
        String                    GetPkgList  (int _iNo){return m_asPkgNameTable[_iNo];}
        CLinkedList<CPkgBase *> * GetList     (        ){return &m_lPkg;}

        void UpdateList(bool _bToTable , TStringGrid * _veGrid);
        void LoadSave  (bool _bLoad , String _sFolderPath);
        void LoadSave  (bool _bLoad );
//==============================================================================


    protected : //�����忡�� �÷��� Ȯ�� �ؼ� ���� �ϴ� �Լ�..  ������ ����.
        CThreadInspect * m_pThreadInspect ;
        void CheckGrab();
        void CheckInsp();
        void CheckGrabInsp();
        void CheckTrainInsp(); //Ʈ����â���� ����.

    public : //m_pActivePkg�̿� �Լ�.
        void SetActivePkg(int _iNo){
            if(_iNo < 0 || _iNo >= m_lPkg.GetDataCnt()) m_pActivePkg = NULL ;    //�̷��� ����ó�� �Ȥ������� PKG �ϳ��� ���� �����Ͽ��� 2��° �ε��� ����.
            else                                        m_pActivePkg = m_lPkg.GetData(_iNo) ;
        }
        CPkgBase * GetCrntPkg(){ return m_pActivePkg ;}


        //ȭ�� ���.
        void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
        TTracker * MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        TTracker * MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        void       MouseUp(TMouseButton Button,TShiftState Shift );

        //�Ķ���� ���÷���
        void UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        void UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        void UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);


    public : //�ٸ������忡���� �̰ɷ� ����..  ������ ��� �Լ���....
        void   SetSendMsgWnd(void * _hWnd);
        void * GetSendMsgWnd();
        void   PaintRsltAll    (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);


        CImage * GetCrntImg ();
        CImage * GetTrainImg();

        void Grab();
        void Insp();
        void GrabInsp();

        void TrainInsp();  //Ʈ���� â���� ���� �˻�..

        bool GetGrabEnd();
        bool GetInspEnd();

        bool GetRslt();

        //�̰��� ũ��Ƽ�� ���� �ִ� ���� �Լ����� ��� �ϸ� �ȉ´�. ����� �ɸ�.
        int  GetPkgCnt();

        TTime GetTimeInf(){return Time ;}
};

#endif
/*


    protected : //ī�޶� ���� Ư�� �Լ���...
    public :
        CCam   * GetCam();
        CImage * GetImg();



CCam * CPkgList::GetCam()
{
//    CPkgBase * Pkg ;
//
//    int iCnt = m_lPkg.GetDataCnt() ;
//    for(int i = 0 ; i <iCnt ; i++) {
//        Pkg = m_lPkg.GetCrntData(!i);
//        if(Pkg -> GetComment() == m_asNameTable[pkCamera_V01]) (CCam *)Pkg ->
//
//    }

}

CImage * CPkgList::GetImg()








    //������ ���� �÷��� �� �Լ�.
    protected :
        CPkgListThread * m_Thread ;



*/