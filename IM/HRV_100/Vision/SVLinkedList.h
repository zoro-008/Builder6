//---------------------------------------------------------------------------
#ifndef SVLinkedListH
#define SVLinkedListH

#include <SysUtils.hpp>

//---------------------------------------------------------------------------
// 2009.01.15
// SUN Kye Won
//---------------------------------------------------------------------------
template <typename T>
class CLinkedList
{
    private :
        struct TNode  {
           T      Data     ;
           TNode *NextNode ;
           TNode (             )            { NextNode = NULL ; };
           TNode (const T& data): Data(data){ NextNode = NULL ; };
           ~TNode(             )            {                   };
        };

        int    DataCnt ;
        TNode *SttNode ;
        TNode *EndNode ;
        TNode *NewNode ;

        int    CrntNodeId ;
        TNode *CrntNode   ;

    public :

    CLinkedList()
    {
       DataCnt    = 0 ;
       CrntNodeId = 0 ;
    }

    ~CLinkedList()
    {
       DeleteAll();
    }

    void DeleteAll()
    {
        if(DataCnt == 0) return ;
        TNode * Next = SttNode ;
        for(register int i = 0 ; i < DataCnt ; i++) {
            NewNode = Next -> NextNode ;
            delete Next ;
            Next = NewNode ;
        }
        DataCnt    = 0 ;
        CrntNodeId = 0 ;
    }

    void PushFrnt(T Data)
    {
        NewNode = new TNode(Data) ;
        if (!DataCnt) {
            SttNode = NewNode ;
            EndNode = NewNode ;
            NewNode-> NextNode = NULL ;
        }
        else {
            NewNode -> NextNode = SttNode ;
            SttNode             = NewNode ;
        }
        DataCnt++;
    };

    void PushBack(T Data)
    {
        NewNode = new TNode(Data) ;
        if (!DataCnt) {
            SttNode = NewNode ;
            EndNode = NewNode ;
            NewNode -> NextNode = NULL ;
        }
        else {
            EndNode -> NextNode = NewNode;
            EndNode             = NewNode;
        }
        DataCnt++ ;
    };

    int Insert(T Data, int Pos)
    {
        if (Pos > DataCnt ) return -1;
        if (Pos == 0      ) { PushFrnt(Data); return 0; }
        if (Pos == DataCnt) { PushBack(Data); return 0; }

        NewNode = new TNode(Data) ;
    
        TNode         *NextPnt = SttNode ;
        register int  i        = 0       ;
    
        while(NextPnt){
            if (i + 1 == Pos){
                NewNode -> NextNode = NextPnt -> NextNode ;
                NextPnt -> NextNode = NewNode ;
                DataCnt++;
                return 0 ;
            }
            NextPnt =  NextPnt -> NextNode ;
            i++;
        }
        return -1;
    };

    void PopFrnt()
    {
        if (DataCnt == 0) return ;
    
        NewNode = SttNode -> NextNode ;
        delete SttNode ;
        SttNode = NewNode ;
        DataCnt-- ;
    };

    void PopBack()
    {
        if (DataCnt == 0) return ;
        TNode * Next = SttNode ;
        register int i = 0 ;
        while(1) {
           if(i + 1 == DataCnt) {
              NewNode = Next -> NextNode ;
              Next -> NextNode = NULL ;
              delete NewNode ;
              DataCnt-- ;
              return ;
           }
           Next = Next -> NextNode ;
           i++ ;
        }
    };

    int Delete(int Pos)
    {
        if (DataCnt == 0 ) return -1;
        if (Pos    <  0 ) return -1;
        if (DataCnt < Pos) return -1;
    
        if (Pos == 0     ) { PopFrnt(); return 0; }
        if (Pos == DataCnt) { PopBack(); return 0; }
    
        TNode *Next = SttNode ;
    
        register int i = 0 ;
    
        while(i < DataCnt) {
           if(i + 1 == Pos) {
              NewNode = Next -> NextNode ;
    
              Next -> NextNode = NewNode -> NextNode ;
    
              delete NewNode ;
              DataCnt-- ;
              return 0 ;
           }
           Next = Next -> NextNode ;
           i++ ;
        }
        return -1 ;

    };

    int GetDataCnt()
    {
        return DataCnt ;
    };

    T GetData(int Pos)
    {
        T RetData ;
        if (DataCnt == 0  ) return RetData;
        if (Pos     <  0  ) return RetData;
        if (DataCnt <= Pos) return RetData;

        TNode * Next = SttNode ;
        register int i = 0 ;
        while(i<DataCnt) {
           if(i == Pos ) {
               RetData = Next -> Data ;
               return RetData ;
           }
           Next = Next -> NextNode ;
           i++ ;
        }
        return RetData ;
    };

    void SwapData(int Pos1 , int Pos2)
    {
        if (DataCnt == 0   ) return ;
        if (Pos1    <  0   ) return ;
        if (Pos2    <  0   ) return ;
        if (DataCnt <= Pos1) return ;
        if (DataCnt <= Pos2) return ;
        T RetData ;

        TNode * Next = SttNode ;
    }

    //빠른패턴을 위해 쓰면 좋다.   포문에서 GetCrntData(!i) 이렇게 붙이면 0번째에 첫번째 인덱스로 이동 하고 1부터는 이어서 진행.
    T GetCrntData(bool FromFirst)
    {
        T RetData ;
        if (DataCnt == 0         ) return RetData ;   //여기 나중에 클리어 해서 리턴하는거 삽입.
        if (FromFirst) {
            RetData    = SttNode -> Data  ;
            CrntNode   = SttNode -> NextNode ;
            CrntNodeId = 0 ;
            return RetData ;
        }
        if (DataCnt <  CrntNodeId) return RetData ;

        RetData = CrntNode -> Data ;
        CrntNodeId ++ ;
        if(CrntNode -> NextNode == NULL) CrntNode = SttNode ;
        else                             CrntNode = CrntNode -> NextNode ;
        return RetData ;
    };

    int GetCrntNodeId()
    {
        if (DataCnt == 0         ) return -1 ;
        if (DataCnt <  CrntNodeId) return -1 ;
        return CrntNodeId ;

    };

    bool Load (AnsiString filepath)
    {
        AnsiString Path = filepath ;
        //Local Var.
        FILE * fp;
        int    Cnt;
        //File Open.
        fp = fopen(Path.c_str() , "rb");
        if (fp == NULL) return false ;

        DeleteAll();

        fread (&Cnt , sizeof(DataCnt) , 1 , fp);

        T Data ;

        for (register int i = 0 ; i < Cnt ; i++) {
            fread (&Data , sizeof(Data) , 1 , fp);
            PushBack (Data);
        }

        DataCnt = Cnt ;

        fclose(fp);
    };

    bool Save (AnsiString filepath)
    {
        AnsiString Path = filepath;

        //Local Var.
        FILE * fp;

        //File Open.
        fp = fopen(Path.c_str() , "wb");
        if (fp == NULL) return false ;

        fwrite (&DataCnt , sizeof(DataCnt) , 1 , fp);

        T Data ;

        for (register int i = 0 ; i < DataCnt ; i++) {
            Data = GetCrntData(!i);
            fwrite (&Data , sizeof(Data) , 1 , fp);
        }
        fclose(fp);
    };

    T & operator [] (int Pos) const
    {   T * RetData ;
        if (DataCnt == 0  ) return * RetData;
        if (Pos     <  0  ) return * RetData;
        if (DataCnt <= Pos) return * RetData;

        TNode * Next = SttNode ;
        register int i = 0 ;
        while(i<DataCnt) {
           if(i == Pos ) {
               RetData = &Next -> Data ;
               return * RetData ;
           }
           Next = Next -> NextNode ;
           i++ ;
        }
        return * RetData ;
    }

    //Overriding the Operator
    void operator = (CLinkedList<T>& rhs) {
        DeleteAll();

        for(int i = 0 ; i < rhs.DataCnt ; i++) {
            PushBack(rhs.GetCrntData(!i));
        }
    }
};
#endif
