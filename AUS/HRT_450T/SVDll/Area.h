//---------------------------------------------------------------------------

#ifndef AreaH
#define AreaH

#include "SVArea.h"


//---------------------------------------------------------------------------








class CArea: public IArea{


    protected : //Var.
        //������ �əV   �����۾��ϸ� ������.
        //int m_iXOffset; //�̹��� ���� Area X�߿� ���� ������
        //int m_iYOffset; //�̹��� ���� Area Y�߿� ���� ������
        //int m_iStartX ; //Area�ȿ��� 0�� �ƴѳ����� ���õǾ� �ִ� ���� ���� X
        //int m_iStartY ; //Area�ȿ��� 0�� �ƴѳ����� ���õǾ� �ִ� ���� ���� y
        //int m_iEndX   ; //Area�ȿ��� 0�� �ƴѳ����� ���õǾ� �ִ� ���� ū X
        //int m_iEndY   ; //Area�ȿ��� 0�� �ƴѳ����� ���õǾ� �ִ� ���� ū y

        ////���� ����.
        //int             m_iWidth     ;  //�̹����� ����.
        //int             m_iHeight    ;  //�̹����� ����.
        //
        ////���� �޸�.
        //unsigned char * m_pArea      ;  //�̹����� ó�� ���� ������ �׷��� �̹���.
        //
        ////Display��.
        //BITMAPINFO    * m_pBitInfo   ;  //��ȯ ���÷��̿�. ��Ʈ������. ex)falseColor , Binary.. etc..
        //HBITMAP         hBitmap      ;
        //

    protected : //Func.
        int  __stdcall BitToByte(int _BitCnt) ;
        bool __stdcall SetPalette(int _iWidth , int _iHeight);
        bool __stdcall SetPaletteColor();


        COLORREF __stdcall GetColor(int _iPx){return m_iPxColor[_iPx] ;}

    public : //������ �Ҹ���.

        CArea();
        ~CArea();

        void __stdcall Del();

        //Overriding the Operator
        //void operator = (CArea& _Src) {
        //    SetSize(_Src.m_iMemWidth , _Src.m_iHeight);
        //    memcpy(m_pArea      , _Src.m_pArea      , m_iMemWidth * m_iHeight );
        //    memcpy(m_pBitInfo   , _Src.m_pBitInfo   , BMP_INFO_SIZE_8 );
        //    memcpy(hBitmap      , _Src.hBitmap      , sizeof(HBITMAP) );
        //}

    public :
        bool __stdcall CheckRangeOver(int _x , int _y) { return (0>_x || _x>=m_iWidth || 0>_y || _y>=m_iHeight);}

        unsigned char __stdcall GetPixel (int _x , int _y){
            if(!m_pArea   || CheckRangeOver(_x,_y))return 0;
            return *(m_pArea   + (_x + m_iMemWidth * _y));
        };
        void __stdcall SetPixel (int _x , int _y , unsigned char _v){
            if(!m_pArea   || CheckRangeOver(_x,_y))return ;
            *(m_pArea   + (_x + m_iMemWidth * _y))= _v ;


            //���� ���� �Ҷ��� ���� �ϰ�
            if(_v) {
                if(m_iStartX > _x ) m_iStartX = _x ;
                if(m_iStartY > _y ) m_iStartY = _y ;

                if(m_iEndX   < _x ) m_iEndX   = _x ;
                if(m_iEndY   < _y ) m_iEndY   = _y ;
            }
            else {
                if(m_iStartX == _x || m_iStartY == _y || m_iEndX  == _x || m_iEndY == _y){
                    for(int x = m_iStartX ; x < m_iEndX ; x++){
                        for(int y = m_iStartY ; y < m_iEndY ; y++) {

                        }
                    }
                }
            }
            return   ;
        }

        unsigned char * __stdcall GetArea();

        //Get �Լ�.
        int  __stdcall GetHeight();
        int  __stdcall GetWidth ();
        int  __stdcall GetMemWidth();


        int  __stdcall GetCntVal (unsigned char _v);

    public :
        //Load Save �Լ�.
        bool __stdcall LoadBmp (char * _FilePath); //�̹����� �ε� �ϴ� �Լ�.
        bool __stdcall SaveBmp (char * _FilePath); //�̹����� ���̺� �ϴ� �Լ�.

        bool __stdcall SetSize(int _iWidth , int _iHeight); //������ �ٲٴ� �Լ� �ε�. ��ż���Ǹ� ��Ŭ���� ��.
        //bool __stdcall SetOffset(int _iX , int _iY);

        
        


        //��Ʈ�� ������ �� ��´�. �޹�� ��� ����.
        bool __stdcall ShowArea (HDC _hHdc , int _x, int _y , float _fScaleX , float _fScaleY );

        //�ܺο��� ������۸� �Ҷ� �޹�� �������� ������ �Ƹ��Ƹ� �׷���. ����.
        bool __stdcall ShowAreaOnBlack (HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight , COLORREF _iColor);

        //ȭ�鿡 �̹����� ���̷�Ʈ�� �׸���� ������۸� ���� �Լ�. �޹�� �������� ������ �Ƹ��Ƹ� �׷���. ����.
        //���� �ȉ� ���߿� ����.
        //HBITMAP __stdcall GetHBitmapOnBlack(HDC _hHdc);
        //void    __stdcall ShowBitmapOnBlack(HDC _hHdc, int _x, int _y, int _iWidth, int _iHeight);


        //Set Area
        bool __stdcall SetArea  (int _iWidth , int _iHeight) ;
        void __stdcall ClrArea  ();

        //���� ���� �Լ�.
        void __stdcall AddCircle (TCircle _tCircle, unsigned char _iVal );
        void __stdcall AddRect   (TRect   _tRect  , unsigned char _iVal );


        //todo

        //Ȯ�� �Լ�.
        //void __stdcall ExpendLine  (TPoint  _tPnt   ){} //����   - ���Է��Ͽ� ���� �ȼ��� '1'�̸� ���� �ȼ� ���� �������� �� '1'�� ����.
        //void __stdcall ExpendRect  (TRect   _tRect  ){} //�簢�� - ���� ���� �Է� �Ͽ� ���� �ȼ��� '1'�̸� ���� �ȼ��� �߽����� �Ͽ� Ȯ��.
        //void __stdcall ExpendCircle(TCircle _tCircle){} //����   - �����Է� �Ͽ� ���� �ȼ��� '1'�̸� �����ȼ��� �߽����� ������ �� '1'�� ����.

        //��� �Լ�.
        //void __stdcall ReduceLine  (TPoint  _tPnt   ){} //����   - ���Է��Ͽ� ���� �ȼ��� '0'�̸� ���� �ȼ� ���� �������� �� '0'�� ����.
        //void __stdcall ReduceRect  (TRect   _tRect  ){} //�簢�� - ���� ���� �Է� �Ͽ� ���� �ȼ��� '0'�̸� ���� �ȼ��� �߽����� �Ͽ� '0'���� ����.
        //void __stdcall ReduceCircle(TCircle _tCircle){} //����   - �����Է� �Ͽ� ���� �ȼ��� '0'�̰� �����ȼ��� �߽����� ������ �� '0'�� ����.

        //���� �Լ�.
        //void __stdcall ConnectLine  (TPoint  _tPnt   ){} //����   - ���Է��Ͽ� ���� �ȼ��� '1'�̰� �Էµ� ����'1'�̸� ���� �ȼ� ���� �������� �� '1'�� ����.

        //������ ��Ʈ���� ���� �ְ�.
        




};

#endif





