//---------------------------------------------------------------------------

#ifndef PkgPolygonH
#define PkgPolygonH

#include "BasePkg.h"
#include "aRect.h"
#include "UnitImage.h"
#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 ����� ����.
class CPolygon_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CPolygon_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(String,sRefOfsXAdd);
            DECLARATION(String,sRefOfsYAdd);

            DECLARATION(String,sOutAreaAdd);

            DECLARATION(int ,iLnSampleGap );
            DECLARATION(int ,iLnLineGap   );
            DECLARATION(int ,iLnEndOffset );
            DECLARATION(int ,iLnCntrOffset);

            DECLARATION(int ,iEgAverage   );
            DECLARATION(int ,iEgCompare   );
            DECLARATION(int ,iEgChkPxLow  );
            DECLARATION(int ,iEgChkPxHgh  );
            DECLARATION(int ,iEgOfsGap    );
            DECLARATION(int ,iEgOfsPxLow  );
            DECLARATION(int ,iEgOfsPxHgh  );

            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd ,"Ref OffsetX Add             " , "���ذ� offsetX �ּ�         ");
                PROPERTY(List , String,sRefOfsYAdd ,"Ref OffsetY Add             " , "���ذ� offsetY �ּ�         ");

                PROPERTY(List , String,sOutAreaAdd ,"Out Area Add                " , "����� Area �ּ�            ");


                PROPERTY(List , int ,iLnSampleGap  ,"Line-Sample Gap             " , "���� Edge ���ø� ����       ");
                PROPERTY(List , int ,iLnLineGap    ,"Line-Line In Range          " , "���� ���� ����              ");
                PROPERTY(List , int ,iLnEndOffset  ,"Line-Insp Offset Of End     " , "���� �˻� ���� ����         ");
                PROPERTY(List , int ,iLnCntrOffset ,"Line-Insp Offset Of Center  " , "�߰� �˻� ���� ����         ");

                PROPERTY(List , int ,iEgAverage    ,"Edge-Average Range          " , "���� �����źȭ ����        ");
                PROPERTY(List , int ,iEgCompare    ,"Edge-Diff Compare           " , "���� �� ����              ");
                PROPERTY(List , int ,iEgChkPxLow   ,"Edge-Check Pixel Low        " , "���� ��ȿ �ּ� �Ӱ谪       ");
                PROPERTY(List , int ,iEgChkPxHgh   ,"Edge-Check Pixel High       " , "���� ��ȿ �ִ� �Ӱ谪       ");
                PROPERTY(List , int ,iEgOfsGap     ,"Edge-Check Offset Pixel Pos " , "���� ��ȿ �����V �Ÿ�       ");
                PROPERTY(List , int ,iEgOfsPxLow   ,"Edge-Check Offset Pixel Low " , "���� ��ȿ �����V �ּ� �Ӱ谪");
                PROPERTY(List , int ,iEgOfsPxHgh   ,"Edge-Check Offset Pixel High" , "���� ��ȿ �����V �ִ� �Ӱ谪");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd="";
                sRefOfsYAdd="";

                iLnSampleGap  = 5 ;
                iLnLineGap    = 1 ;
                iLnEndOffset  = 0 ;
                iLnCntrOffset = 0 ;

                iEgAverage    = 1 ;
                iEgCompare    = 1 ;
                iEgChkPxLow   = 0 ;
                iEgChkPxHgh   = 0 ;
                iEgOfsGap     = 0 ;
                iEgOfsPxLow   = 0 ;
                iEgOfsPxHgh   = 0 ;
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(bool  ,bLtOnDk        );

            TCommonPara(){
                PROPERTY(List , bool  ,bLtOnDk      ,"Find Light On Dark  " , "���� �簢�� ã��");
                SetDefault();
            }
            void SetDefault(){
                bLtOnDk    = false ;
            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool    bInspEnd ;
            bool    bRsltOk  ;

            int       iConnerCnt ;
            TDPoint * aConners ; // �ٰ��� �ڳ� ����Ʈ �迭.
            CLinkedList<TDPoint> lEdgeList ;

            TRslt(){
                aConners = NULL;
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;

                iConnerCnt = 0 ;
                if(aConners) {
                    delete [] aConners ;
                    aConners = NULL;
                }

                lEdgeList.DeleteAll();

            }
        };

        //Functions
        CPolygon_V01();
        ~CPolygon_V01();



    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt  Rslt ;

        CLinkedList<CTracker*> m_lTrckInsp ;
        CLinkedList<TLocalPara  *> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.


    private :
        void OnTrackerChanged();

    public : //���� ���� �Լ� ��================================================
        virtual bool Init();
        virtual bool Close();

        //�˻� �� ���� ����.
        virtual bool   Train            (CImage * _pImg);
        virtual void   RsltClear        (); //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
        virtual bool   GetRslt          ();
        virtual bool   Run              (CImage * _pImg);
        virtual String GetErrMsg        ();

        //ȭ�� ���.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
        virtual bool MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual bool MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void MouseUp(TMouseButton Button,TShiftState Shift );

        //�Ķ���� ���÷���
        virtual void UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);

        //�ε� ���̺�.
        virtual void LoadSave(bool _bLoad , String _sVisnName);
};
#endif


