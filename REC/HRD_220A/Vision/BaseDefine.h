//---------------------------------------------------------------------------

#ifndef BASEDEFINE
#define BASEDEFINE
//---------------------------------------------------------------------------

#ifndef TypesHPP
  struct TPoint : public POINT
  {
    TPoint() {}
    TPoint(int _x, int _y) { x=_x; y=_y; }
    TPoint(POINT& pt)
    {
      x = pt.x;
      y = pt.y;
    }
  };
 
  typedef TPoint tagPoint;
 
  struct TRect : public RECT
  {
    TRect() {}
    TRect(const TPoint& TL, const TPoint& BR) { left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }
    TRect(int l, int t, int r, int b)         { left=l;    top=t;    right=r;    bottom=b;    }
    TRect(RECT& r)
    {
      left    = r.left;
      top     = r.top;
      right   = r.right;
      bottom  = r.bottom;
    }
    int Width () const { return right  - left; }
    int Height() const { return bottom - top ; }
    bool operator ==(const TRect& rc) const 
    {
       return left ==  rc.left  && top==rc.top &&     
              right == rc.right && bottom==rc.bottom; 
    }
    bool operator !=(const TRect& rc) const 
    {  return !(rc==*this); }
 
       __property LONG Left    = { read=left,   write=left   }; 
       __property LONG Top     = { read=top,       write=top }; 
       __property LONG Right   = { read=right,  write=right  }; 
       __property LONG Bottom  = { read=bottom, write=bottom }; 
  };
#endif
//---------------------------------------------------------------------------
struct TTagPoint : public TPoint
{
    int iTag ;
};

struct TFPoint
{
    float x , y ;
    TFPoint() {x=0.0 ; y=0.0;}
    TFPoint(float _fX, float _fY) { x=_fX; y=_fY; }
    void Clear(){x=0.0;y=0.0;}
};

struct TDPoint
{
    double x , y ;
    TDPoint() {x=0.0 ; y=0.0;}
    TDPoint(double _fX, double _fY) { x=_fX; y=_fY; }
    void Clear(){x=0.0;y=0.0;}
    TPoint GetTPoint(){TPoint Point ; Point.x = x ; Point.y = y ; return Point;}
};

struct TFRect
{

    float left,top,right,bottom ;
    TFRect() {Clear();}
    void Clear(){left  =0.0;top   =0.0;right =0.0;bottom=0.0;}
    TFRect(const TPoint& TL, const TPoint& BR){ left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }
    TFRect(float l, float t, float r, float b){ left=l;    top=t;    right=r;    bottom=b;    }
    TFRect(TFRect& r)
    {
      left    = r.left;
      top     = r.top;
      right   = r.right;
      bottom  = r.bottom;
    }
    float Width () const { return right  - left; }
    float Height() const { return bottom - top ; }
    RECT GetRECT() {RECT Rect ; Rect.left = left ; Rect.top = top ; Rect.right = right ; Rect.bottom = bottom ; return Rect ; }


    bool operator ==(const TFRect& rc) const
    {
       return left ==  rc.left  && top==rc.top &&
              right == rc.right && bottom==rc.bottom;
    }
    bool operator !=(const TFRect& rc) const
    {  return !(rc==*this); }

       __property float Left    = { read=left  , write=left  };
       __property float Top     = { read=top   , write=top   };
       __property float Right   = { read=right , write=right };
       __property float Bottom  = { read=bottom, write=bottom};
};

struct TDRect
{

    double left,top,right,bottom ;
    TDRect(){Clear();}
    void Clear(){left  =0.0;top   =0.0;right =0.0;bottom=0.0;}
    TDRect(const TPoint& TL, const TPoint& BR)    { left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }
    TDRect(double l, double t, double r, double b){ left=l;    top=t;    right=r;    bottom=b;    }
    TDRect(TFRect& r)
    {
      left    = r.left;
      top     = r.top;
      right   = r.right;
      bottom  = r.bottom;
    }
    double Width () const { return right  - left; }
    double Height() const { return bottom - top ; }
    RECT GetRECT() {RECT Rect ; Rect.left = left ; Rect.top = top ; Rect.right = right ; Rect.bottom = bottom ; return Rect ; }
    TRect GetTRect() {TRect Rect ; Rect.left = left ; Rect.top = top ; Rect.right = right ; Rect.bottom = bottom ; return Rect ; }


    bool operator ==(const TDRect& rc) const
    {
       return left ==  rc.left  && top==rc.top &&
              right == rc.right && bottom==rc.bottom;
    }
    bool operator !=(const TDRect& rc) const
    {  return !(rc==*this); }

       __property double Left    = { read=left  , write=left  };
       __property double Top     = { read=top   , write=top   };
       __property double Right   = { read=right , write=right };
       __property double Bottom  = { read=bottom, write=bottom};
};

struct TCircle
{
    int cx,cy,rad ;
    TCircle(){Clear();}
    void Clear(){cx=cy=rad=0;}
    TCircle(int _fCx , int _fCy , int _fRad){cx=_fCx; cy=_fCy ; rad=_fRad;}
};

struct TFCircle
{
    float cx,cy,rad ;
    TFCircle(){Clear();}
    void Clear(){cx=cy=rad=0;}
    TFCircle(float _fCx , float _fCy , float _fRad){cx=_fCx; cy=_fCy ; rad=_fRad;}
};

struct TDCircle
{
    double cx,cy,rad ;
    TDCircle(){Clear();}
    void Clear(){cx=cy=rad=0;}
    TDCircle(double _fCx , double _fCy , double _fRad){cx=_fCx; cy=_fCy ; rad=_fRad;}
};

/*
struct _SBlob : public TRect {
    int      iBoundCnt ;

    int    iCenterX     ;
    int    iCenterY     ;
    int    iWidth       ;
    int    iHeight      ;
    int    iArea        ;
    float  fBoundary    ;

    int    iStartX      ;
    int    iStartY      ;


    float  fCompactness ;
    float  fCentroidX   ;
    float  fCentroidY   ;
    float  fAngle       ;
    float  fAspectRatio ;

    float  fXA          ;
    float  fXB          ;

    float  fYA          ;
    float  fYB          ;
};
*/


//Pop�Լ��� T���� ���� �ϰ� ������ ������ �ϰ�� �ʱ�ȭ�� ���� �������� ���󰡰� �Ǿ�..
//�׳� Bool �������� �Ѵ�. ���߿� ����� ã�ƺ���.
//CLinkedList<int> LinkedList ;
template <typename T>
class CLinkedList
{
    private : //���� Ȯ�� ���ѳ��. ���߿� Ȯ�� �ϰ� Public���� ����.
        T & operator [] (int Pos) const
        {   T * RetData ;
            if (m_iDataCnt == 0  ) return * RetData;
            if (Pos     <  0     ) return * RetData;
            if (m_iDataCnt <= Pos) return * RetData;

            TNode * Crnt = m_pSttNode ;
            int i = 0 ;
            while(i<m_iDataCnt) {
               if(i == Pos ) {
                   RetData = &Crnt -> Data ;
                   return * RetData ;
               }
               Crnt = Crnt -> NextNode ;
               i++ ;
            }
            return * RetData ;
        }


    private :
    //public :
        struct TNode  {
           T      Data     ;
           TNode *NextNode ;
           TNode (             )            { NextNode = NULL ; };
           TNode (const T& data): Data(data){ NextNode = NULL ; };
           ~TNode(             )            {                   };
        };

        int    m_iDataCnt ;
        TNode *m_pSttNode ;
        TNode *m_pEndNode ;
        //TNode *m_pTmpNode ;
        //TNode *pTmpNod ;

        int    m_iCrntNodeId ;
        TNode *m_pCrntNode   ;

    public :

        CLinkedList()
        {
           m_iDataCnt    = 0 ;
           m_iCrntNodeId = 0 ;
        }

        ~CLinkedList()
        {
           DeleteAll();
        }

        bool DeleteAll() //Ȯ��
        {
            if(m_iDataCnt == 0) return false ;
            TNode * Crnt = m_pSttNode ;
            TNode * pTmpNode ;
            for(int i = 0 ; i < m_iDataCnt ; i++) {
                pTmpNode = Crnt -> NextNode ;
                delete Crnt ;
                Crnt = pTmpNode ;
            }
            m_iDataCnt    = 0 ;
            m_iCrntNodeId = 0 ;

            return true ;
        }

        bool PushFrnt(T Data) //OK
        {
            TNode * pTmpNode = new TNode(Data) ;
            if (m_iDataCnt==0) {
                m_pSttNode = pTmpNode ;
                m_pEndNode = pTmpNode ;
                pTmpNode-> NextNode = NULL ;
            }
            else {
                pTmpNode -> NextNode = m_pSttNode ;
                m_pSttNode             = pTmpNode ;
            }
            m_iDataCnt++;

            return true ;
        };

        bool PushBack(T Data) //OK
        {
            TNode * pTmpNode = new TNode(Data) ;
            pTmpNode -> NextNode = NULL ;
            if (m_iDataCnt==0) {
                m_pSttNode = pTmpNode ;
                m_pEndNode = pTmpNode ;

            }
            else {
                m_pEndNode -> NextNode = pTmpNode;
                m_pEndNode             = pTmpNode;
            }
            m_iDataCnt++ ;

            return true ;
        };

        bool PopFrnt()
        {
            if (m_iDataCnt == 0) return false ;

            TNode * pTmpNode = m_pSttNode -> NextNode ;

            delete m_pSttNode ;
            m_pSttNode = pTmpNode ;
            m_iDataCnt-- ;

            return true ;
        };

        bool PopBack()
        {
            if (m_iDataCnt == 0) return false ;

            // �� �̷��� ���� m_pEndNode�� �ִµ�......
            // ��.. Node�� NextNode�� �־(SingleLinkedList) �׷�����...���߿� PreNode�� ������...
            TNode * Crnt = m_pSttNode ;
            TNode * pPreNode = NULL ;

            int i = 0 ;

            while(Crnt != NULL) {
               if(i + 1 == m_iDataCnt) { //�Ǹ�������� ã��.
                  pPreNode -> NextNode = NULL ;
                  m_pEndNode = pPreNode ;
                  delete Crnt ;
                  m_iDataCnt-- ;
                  return true ;
               }
               pPreNode = Crnt ;
               Crnt = Crnt -> NextNode ;
               i++ ;
            }
            return true ;


        };


        bool Insert(T Data, int Pos) //OK
        {
            if (Pos > m_iDataCnt ) return false;
            if (Pos == 0         ) { return PushFrnt(Data); } //�̷��κ� ���߿� ��������..
            if (Pos == m_iDataCnt) { return PushBack(Data); } //PushFront Back �� ���ְ� �μ�Ʈ���� ���ذ��ϰ�...

            TNode * pTmpNode = new TNode(Data) ;

            TNode *Crnt = m_pSttNode ;
            int  i      = 0          ;

            while(Crnt!=NULL){
                if (i + 1 == Pos){
                    pTmpNode -> NextNode = Crnt -> NextNode ;
                    Crnt -> NextNode = pTmpNode ;
                    m_iDataCnt++;
                    return true ;
                }
                Crnt =  Crnt -> NextNode ;
                i++;
            }
            return true;
        };



        bool Delete(int Pos)  //T�� �����ϰ� ������ �������� ��� �ʱ�ȭ�� �ȵż� �ȉ�.
        {
            //T RetData ; //���⵵ ���� �ʱ�ȭ ���� ����.
            if (m_iDataCnt ==  0 ) return false;
            if (Pos        <   0 ) return false;
            if (m_iDataCnt <= Pos) return false;

            if (Pos == 0           ) { PopFrnt(); return false; } //PopFrnt�� �� ���ϰ��� �����ϰ� ������ �������� ��� �ʱ�ȭ�� �ȵż� �ȉ�.
            if (Pos == m_iDataCnt-1) { PopBack(); return false; } //�������� ��� �ϴ� �ð��� �ɷ��� GetData�� �̿��Ͽ� �ش������� ��ȯ�������� �޸�Delete��Ű��
                                                               //Delete �Լ� �����ؾ� �Ѵ�.���߿� ��������..
            TNode *Crnt = m_pSttNode ;
            TNode *_pTmpNode ;

            int i = 0 ;

            while(i < m_iDataCnt) {
               if(i + 1 == Pos) {
                  _pTmpNode = Crnt -> NextNode ;

                  Crnt -> NextNode = _pTmpNode -> NextNode ;

                  delete _pTmpNode ;    //���⼭ ���� T�� �޾Ƴ��� ���� �ϰ� ������....
                  m_iDataCnt-- ;
                  return true ;
               }
               Crnt = Crnt -> NextNode ;
               i++ ;
            }
            return false ;

        };

        int GetDataCnt()
        {
            return m_iDataCnt ;
        };

        T GetData(int Pos)
        {
            T RetData ;
            if (m_iDataCnt == 0  ) return RetData;
            if (Pos        <  0  ) return RetData;
            if (m_iDataCnt <= Pos) return RetData;

            TNode * Crnt = m_pSttNode ;
            int i = 0 ;
            while(i<m_iDataCnt) {
               if(i == Pos ) {
                   RetData = Crnt -> Data ;
                   return RetData ;
               }
               Crnt = Crnt -> NextNode ;
               i++ ;
            }
            return RetData ;
        };

        //�� �ϴ� ������???
        //������ "=" �� �Ǵ� ��ü�� ��밡����......  �ʿ� ���� ����...
        bool  Move(int Pos1 , int Pos2) //Pos1 ��带 Pos2�� ����.
        {
            if (m_iDataCnt == 0   ) return false;
            if (Pos1       <  0   ) return false;
            if (Pos2       <  0   ) return false;
            if (m_iDataCnt <= Pos1) return false;
            if (m_iDataCnt <= Pos2) return false;
            if (Pos1       == Pos2) return false;

            //Insert(T Data, int Pos) //OK

            int pos1 = (Pos1 < Pos2) ? Pos1 : Pos2 ;
            int pos2 = (Pos1 < Pos2) ? Pos2 : Pos1 ;

            Insert(GetData(pos1),pos2+1);
            Insert(GetData(pos2),pos1+1);
            Delete(pos1);
            Delete(pos1);
            return true ;
        }

        //���������� ���� ���� ����.   �������� GetCrntData(!i) �̷��� ���̸� 0��°�� ù��° �ε����� �̵� �ϰ� 1���ʹ� �̾ ����.
        T GetCrntData(bool FromFirst)
        {
            T RetData ;
            if (m_iDataCnt == 0 ) return RetData ;   //���� ���߿� Ŭ���� �ؼ� �����ϴ°� ����.
            if (FromFirst) {
                RetData    = m_pSttNode -> Data  ;
                m_pCrntNode   = m_pSttNode -> NextNode ;
                m_iCrntNodeId = 0 ;
                return RetData ;
            }
            if (m_iDataCnt <  m_iCrntNodeId) return RetData ;

            RetData = m_pCrntNode -> Data ;
            m_iCrntNodeId ++ ;
            if(m_pCrntNode -> NextNode == NULL) m_pCrntNode = m_pSttNode ;
            else                                m_pCrntNode = m_pCrntNode -> NextNode ;
            return RetData ;
        };

        int GetCrntNodeId()
        {
            if (m_iDataCnt == 0         ) return -1 ;
            if (m_iDataCnt <  m_iCrntNodeId) return -1 ;
            return m_iCrntNodeId ;

        };



        //Overriding the Operator
        void operator = (CLinkedList<T>& rhs) {
            DeleteAll();

            for(int i = 0 ; i < rhs.m_iDataCnt ; i++) {
                PushBack(rhs.GetCrntData(!i));
            }
        }
};
//#include "SLogUnit.h"
//----------------------------------------------------------------------------
double __fastcall GetTime(void)
{
//Trace("GetTime","stt");
    LARGE_INTEGER liEndCounter,liFrequency  ;
    QueryPerformanceCounter  (&liEndCounter);
    QueryPerformanceFrequency(&liFrequency );

//Trace("GetTime","end");
    return (((double)liEndCounter.QuadPart / (double)liFrequency.QuadPart) * 1000.0 );
}




#endif


