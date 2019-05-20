//---------------------------------------------------------------------------
//
#include <Dialogs.hpp>
#include <stdio.h>
#include <Types.hpp>

#pragma hdrstop

#include "UnitArea.h"
#include "BaseMaths.h"
#include "UnitGdi.h"





//
//---------------------------------------------------------------------------

#pragma package(smart_init)
//#define VPixel(x,y) (*(pVPixel + x + _tRect.Width() * y))

//GetPixelDirect(x,y) ���� ������ GetPixelDirect(i+1,y) �̷��� �ϸ� ���ڰ� i�� ����.
//�� GetPixelDirect((i+1),y) �̷��� ��ȣ �������� �ȴ�.
#define GetPixelDirect(x,y) *(m_pArea   + (x + m_iMemWidth * y))

CArea::CArea()
{
    m_pArea  = NULL ;
                                                              
    m_pBitInfo   = NULL ;

    m_iWidth  = 0  ;
    m_iHeight = 0  ;

    m_iMemWidth = 0 ;

    //8Bit color pallette
    BYTE R,G,B ;
/*
    for (int i=0; i<MAX_COLOR_8; i++) {
             if (i == 0      ){R = 0                 ;G = 0                 ;B = 0                 ;}
        else if (i == 1      ){R = 255               ;G = 0                 ;B = 0                 ;}
        else if (i == 2      ){R = 255               ;G = 255               ;B = 0                 ;}
        else if (i == 3      ){R = 255               ;G = 0                 ;B = 255               ;}
        else if (i == 4      ){R = 0                 ;G = 255               ;B = 0                 ;}
        else if (i == 5      ){R = 0                 ;G = 255               ;B = 255               ;}
        else if (i == 6      ){R = 0                 ;G = 0                 ;B = 255               ;}
        else if ((i % 7) == 1){R = 255 - ((i*16)%256);G = 0                 ;B = 0                 ;}
        else if ((i % 7) == 4){R = 255 - ((i*16)%256);G = 255 - ((i*16)%256);B = 0                 ;}
        else if ((i % 7) == 5){R = 255 - ((i*16)%256);G = 0                 ;B = 255 - ((i*16)%256);}
        else if ((i % 7) == 2){R = 0                 ;G = 255 - ((i*16)%256);B = 0                 ;}
        else if ((i % 7) == 6){R = 0                 ;G = 255 - ((i*16)%256);B = 255 - ((i*16)%256);}
        else if ((i % 7) == 3){R = 0                 ;G = 0                 ;B = 255 - ((i*16)%256);}
        else if ((i % 7) == 0){R = 255 - ((i*16)%256);G = 255 - ((i*16)%256);B = 255 - ((i*16)%256);}
        else                  {R = 255               ;G = 255               ;B = 255               ;}
        m_iPxColor[i] = RGB(R,G,B);
    }
*/
    //false color pallette Set



    for (int i=0,j=0; i<MAX_COLOR_8; i++) {
        j = i * 51 + ((i%2) ? 20 :  0) ;
        j %= 256 ;
             if (j>   0 && j<= 51){R = 255         ;G = i*5          ;B = 0        ;}
        else if (j>= 52 && j<=102){R = 255-(i-52)*5;G = 255          ;B = 0        ;}
        else if (j>=103 && j<=153){R = 0           ;G = 255          ;B = (i-103)*5;}
        else if (j>=154 && j<=203){R = 0           ;G = 255-(i-154)*5;B = 255      ;}
        else if (j>=204 && j<=254){R = (i-204)*5   ;G = 0            ;B = 255      ;}
        else                      {R = 0           ;G = 0            ;B = 0        ;}
        m_iPxColor[i] = RGB(R,G,B);
    }


}

CArea::~CArea()
{
    Del();


}

void __stdcall CArea::Del()
{
    delete [] (BYTE *)m_pBitInfo   ;

    if(m_pArea  ){GlobalFree(m_pArea );}//delete [] m_pArea  ;}
}

int __stdcall CArea::BitToByte(int _BitCnt) { return _BitCnt / BYTE_BIT_COUNT ;}


unsigned char * __stdcall CArea::GetArea(){ if(!m_pArea  ) return NULL ; return m_pArea   ; };  //�̹��������� ����.

//Get �Լ�.
int  __stdcall CArea::GetHeight(){return m_iHeight ;}
int  __stdcall CArea::GetWidth (){return m_iWidth  ;}
int  __stdcall CArea::GetMemWidth(){return m_iMemWidth ;}

bool __stdcall CArea::SetPalette(int _iWidth , int _iHeight)
{
    if(m_pBitInfo  ) {delete [] (BYTE *)m_pBitInfo   ; m_pBitInfo   = NULL ; }

    if(!m_pBitInfo  )m_pBitInfo   = (BITMAPINFO *) new BYTE[BMP_INFO_SIZE_8 ]; ZeroMemory(m_pBitInfo  ,sizeof(BMP_INFO_SIZE_8 ));

    int iActWidth ;
    if(_iWidth % 4) iActWidth = (_iWidth / 4)*4+4 ;
    else            iActWidth =  _iWidth          ;

    m_pBitInfo -> bmiHeader.biSize          = 40 ;
    m_pBitInfo -> bmiHeader.biWidth         = _iWidth  ;
    m_pBitInfo -> bmiHeader.biHeight        = _iHeight ;
    m_pBitInfo -> bmiHeader.biPlanes        = 1  ;
    m_pBitInfo -> bmiHeader.biBitCount      = 8  ;
    m_pBitInfo -> bmiHeader.biCompression   = BI_RGB;
    m_pBitInfo -> bmiHeader.biSizeImage     = iActWidth * _iHeight ;
    m_pBitInfo -> bmiHeader.biXPelsPerMeter = 0  ;
    m_pBitInfo -> bmiHeader.biYPelsPerMeter = 0  ;
    m_pBitInfo -> bmiHeader.biClrUsed       = 0  ;
    m_pBitInfo -> bmiHeader.biClrImportant  = 0  ;

    SetPaletteColor();

    return true ;
}

bool __stdcall CArea::SetPaletteColor()
{
    //8Bit color pallette
    for (int i=0; i<MAX_COLOR_8; i++) {
        m_pBitInfo-> bmiColors[i].rgbRed      = GetRValue(m_iPxColor[i]) ;
        m_pBitInfo-> bmiColors[i].rgbGreen    = GetGValue(m_iPxColor[i]) ;
        m_pBitInfo-> bmiColors[i].rgbBlue     = GetBValue(m_iPxColor[i]) ;
        m_pBitInfo-> bmiColors[i].rgbReserved = 0;
    }
    return true ;
}

bool __stdcall CArea::SetSize(int _iWidth , int _iHeight )
{
    int iActWidth ;
    if(_iWidth % 4) iActWidth = (_iWidth / 4)*4+4 ;
    else            iActWidth =  _iWidth ;

    if(m_iWidth != _iWidth || m_iHeight != _iHeight) {
        m_iWidth    = _iWidth   ;
        m_iHeight   = _iHeight  ;
        m_iMemWidth = iActWidth ;

        SetPalette(_iWidth , _iHeight);
        if(m_pArea  ){GlobalFree(m_pArea );}//delete [] m_pArea  ;}

        m_pArea  = (unsigned char*)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,m_iMemWidth *_iHeight); //new unsigned char [iActWidth * _iHeight ] ;
    }


    memset(m_pArea , 0 ,m_iMemWidth * _iHeight );

    return true ;
}

bool __stdcall CArea::SetStart(int _iStartX , int _iStartY )
{
    if(_iStartX < 0 || _iStartY < 0) return false ;
    m_iStartX = _iStartX ;
    m_iStartY = _iStartY ;

    return true ;
}

int  __stdcall CArea::GetStartX()
{
    return m_iStartX ;
}

int  __stdcall CArea::GetStartY()
{
    return m_iStartY ;
}

bool __stdcall CArea::LoadBmp (char * _FilePath)
{
    AnsiString filename = _FilePath;

    if(filename.Pos(".BMP") ||
       filename.Pos(".BMp") ||
       filename.Pos(".BmP") ||
       filename.Pos(".bMP") ||
       filename.Pos(".bMp") ||
       filename.Pos(".bmp") ) filename = filename ;
    else                      filename = filename +".bmp" ;

    FILE * file = fopen(filename.c_str(),"rb") ; //D:\BC6Works\HRD_810mVision\bin\JobFile\Small_1X10_UV\Vision

    if(file == NULL) {
        //ShowMessage(filename + " ���� ������ ���� �����ϴ�.");
        return false ;
    }

    BITMAPFILEHEADER  bfh ;
    BITMAPINFOHEADER  bih ;

//  ���� 32bit CPU  �ý��ۿ��� �޸��Ҵ��� 32bit�� �մϴ�.
//  ���� ���� ����ü�� �׳� sizeof�ϸ� 16byte�� ���ɴϴ�
//  ���� 14����Ʈ ��BITMAPFILEHEADER�� sizeof(BITMAPFILEHEADER) �� ���ʽÿ� 16 �� ���ð̴ϴ�.

    fread(&bfh.bfType      , 2, 1, file);     // ������ ���� �ƽ�Ű ex) BMP=19778   JPG=55551
    fread(&bfh.bfSize      , 4, 1, file);     // ������ ��ü ũ��   ex) 787510     4194358
    fread(&bfh.bfReserved1 , 2, 1, file);     // ������ 0���� ����
    fread(&bfh.bfReserved2 , 2, 1, file);     // ������ 0���� ����
    fread(&bfh.bfOffBits   , 4, 1, file);     // ��Ʈ�� ���� �ش��� ���� ���� �����ͱ����� ����Ʈ�Ÿ�. �ȷ�Ʈ ������1078 �Q���� 54

    if(bfh.bfType != 0x4d42){ShowMessage("��Ʈ�� �̹����� �ƴմϴ�."); fclose(file); return false ;}

    int iTemp  = sizeof(BITMAPFILEHEADER) - 2; //���� ���� 32��Ʈ cpu�� 4����Ʈ�� �Ҵ��Ͽ� ���� 14����Ʈ ũ���ε� 16���� ����.
                                      
    fread(&bih.biSize          , 4, 1, file); //DWORD  biSize;           40     BITMAPINFOHEADER ����ü�� ũ�� �ּ� 40�̻�.
    fread(&bih.biWidth         , 4, 1, file); //LONG   biWidth;          1024   �̹��� ����
    fread(&bih.biHeight        , 4, 1, file); //LONG   biHeight;         768    + �� ��� �Ʒ����� ���� ����, -�� ��� ������ �Ʒ��� ����.. biWidth�� ������� ���ʿ��� ���������� ����.
    fread(&bih.biPlanes        , 2, 1, file); //WORD   biPlanes;         1      �÷� Plane�� ��.(???)
    fread(&bih.biBitCount      , 2, 1, file); //WORD   biBitCount        8      ���Ʈ �̹�������(1,4,8,16,24).  (�ܻ�,16��,256��,24��Ʈ)
    fread(&bih.biCompression   , 4, 1, file); //DWORD  biCompression;    0      BI_RGB(������� ���� ��Ʈ��), BI_RLE8�̸� 8��Ʈ ����, BI_RLE4�̸� 4��Ʈ ����
    fread(&bih.biSizeImage     , 4, 1, file); //DWORD  biSizeImage;      786432 �̹��� ũ�⸦ ����Ʈ ������ ��Ÿ����, BI_RGB �� ��� 0
    fread(&bih.biXPelsPerMeter , 4, 1, file); //LONG   biXPelsPerMeter;  1      �����ػ�.
    fread(&bih.biYPelsPerMeter , 4, 1, file); //LONG   biYPelsPerMeter;  1      �����ػ�.
    fread(&bih.biClrUsed       , 4, 1, file); //DWORD  biClrUsed;        0      ��Ʈ�ʿ� ���� ������� �ǹ��ϸ�, �̰��� ���� RGBQUAD�� �迭�� �޸� �Ҵ��Ͽ��� �о���ø� �˴ϴ�. �� ���� 0�̸� ��� ������ �� ����Ѵٴ� �ǹ��Դϴ�.
    fread(&bih.biClrImportant  , 4, 1, file); //DWORD  biClrImportant;   0      ��Ʈ���� ����ϴµ� �ʼ����� ������� ��Ÿ����, �� ���� 0�̸� ��� ������ �� ����Ѵٴ� �ǹ��Դϴ�.

    int iTemp1 = sizeof(BITMAPINFOHEADER) ;

    if(bih.biBitCount != 8){ShowMessage("8��Ʈ�̹����� �ƴմϴ�."); fclose(file); return false ;} //8��Ʈ Ȥ�� 24��Ʈ �ƴϸ� �ε� �ȵ�.

    //�ù� ... ���� ���ҷ��� �ؿ� �޸� ���°� ������.
    //ȭ�鿡 �̹��� �ѷ��ٶ� ������ ��Ʈ������������� ���� ������ ��� �ؼ� �׳� ����.
    bih.biHeight = -bih.biHeight;

    SetSize(bih.biWidth , abs(bih.biHeight)); //m_pBitmapInfo�̰� �ǵ帮�� ���� �̹��� ���� ������ �°� �ٲ�.

    //�ȷ�Ʈ ����.
    RGBQUAD Temp ;
    for (int i = 0 ; i < MAX_COLOR_8 ; i++){

        fread(&Temp , 1, 1, file);
        fread(&Temp , 1, 1, file);
        fread(&Temp , 1, 1, file);
        fread(&Temp , 1, 1, file);

        //��Ʈ�����Ͽ� �ִ� �ȷ�Ʈ �Ⱦ�.
        //fread(&m_pBitInfo8 ->bmiColors[i].rgbBlue     , 1, 1, file);
        //fread(&m_pBitInfo8 ->bmiColors[i].rgbGreen    , 1, 1, file);
        //fread(&m_pBitInfo8 ->bmiColors[i].rgbRed      , 1, 1, file);
        //fread(&m_pBitInfo8 ->bmiColors[i].rgbReserved , 1, 1, file);
    }

    int iTemp2   =  sizeof(RGBQUAD) * MAX_COLOR_8 ;
    int iTemp012 = iTemp + iTemp1 + iTemp2 ;


//�̹��� ������ ����.

    RGBQUAD   rgb4 ;
    RGBTRIPLE rgb3 ;
    int a = sizeof(RGBTRIPLE);
    int iPxByte = BitToByte(bih.biBitCount) ;

    int r,g,b ;
    int rgb   ;
    int iTemp3 = bih.biWidth * bih.biHeight * bih.biBitCount/BYTE_BIT_COUNT;//* pow(2 , bih.biBitCount) ;
    int iTemp4 = iTemp + iTemp1 + iTemp2 + iTemp3 ;
    int iTemp5 = bfh.bfSize - iTemp4 ;

    if (bih.biHeight < 0) {
        for(int y=m_iHeight-1;y>=0;y--) {
            fread(m_pArea + y    * m_iMemWidth , m_iMemWidth, 1, file);
        }
    }


    else {
        BYTE * pLine = new BYTE [m_iMemWidth] ;
        for(int y=0;y<m_iHeight;y++) {
            fread(pLine , m_iMemWidth , 1 , file);
            memcpy(m_pArea + y * m_iMemWidth , pLine , m_iMemWidth);
        }
        delete [] pLine ;
    }


    int ret = fclose(file);

    return true ;

}

bool __stdcall CArea::SaveBmp (char * _FilePath)
{
    AnsiString filename = _FilePath;

    if(filename.Pos(".BMP") ||
       filename.Pos(".BMp") ||
       filename.Pos(".BmP") ||
       filename.Pos(".bMP") ||
       filename.Pos(".bMp") ||
       filename.Pos(".bmp") ) filename = filename ;
    else                      filename = filename +".bmp" ;

    if(!m_pArea) return false ;

    FILE * file = fopen(filename.c_str(),"wb") ;
    if(file == NULL) {ShowMessage("������ ���� �� �� �����ϴ�."); return false ;}

    //�ķ�Ʈ ���� �������� ����.
    SetPaletteColor();

    BITMAPFILEHEADER  bfh ;
    BITMAPINFOHEADER  bih ;

    bih = m_pBitInfo -> bmiHeader ;

    bih.biHeight =  m_iHeight ;
    bih.biWidth  =  m_iWidth  ;

    const int iMaxPx = pow(2,bih.biBitCount) ;  //���ȼ��� ǥ�� �Ҽ� �ִ� ���� ^
    const int iSizeOfBfh  = 14 ; //sizeof(BITMAPFILEHEADER) ; �������� �־ 14�ε� 16���� ����.
    const int iSizeOfPlt  = sizeof(RGBQUAD) * iMaxPx ;
    //biSizeImage = ((((bih.biWidth * m_iBitCnt) + 31) & ~31) >> 3) * bih.biHeight �����Ƽ� 4����Ʈ ���� �������.
    const int iSizeOfImg  = m_iMemWidth * bih.biHeight ;
    const int iOftToImg   = iSizeOfBfh + sizeof(BITMAPINFOHEADER) + iSizeOfPlt ;
    const int iSizeOfFile = iSizeOfBfh + sizeof(BITMAPINFOHEADER) + iSizeOfImg + iSizeOfPlt ;

    bfh.bfType          = 0x4d42;        // BM => Bitmap file
    bfh.bfSize          = iSizeOfFile ;  // FileSize.
    bfh.bfReserved1     = 0;             // not use
    bfh.bfReserved2     = 0;             // not use
    bfh.bfOffBits       = iOftToImg;     // ��Ʈ�� ���� �ش��� ���� ���� �����ͱ����� ����Ʈ�Ÿ�. �ȷ�Ʈ ������1078 �Q���� 54

    bih.biSizeImage     = iSizeOfImg;

    fwrite(&bfh.bfType      , 2, 1, file);     // 19778
    fwrite(&bfh.bfSize      , 4, 1, file);     // 787510
    fwrite(&bfh.bfReserved1 , 2, 1, file);     // 0
    fwrite(&bfh.bfReserved2 , 2, 1, file);     // 0
    fwrite(&bfh.bfOffBits   , 4, 1, file);     // 1078

    fwrite(&bih.biSize          , 4, 1, file); //DWORD  biSize;           40
    fwrite(&bih.biWidth         , 4, 1, file); //LONG   biWidth;          1024
    fwrite(&bih.biHeight        , 4, 1, file); //LONG   biHeight;         768
    fwrite(&bih.biPlanes        , 2, 1, file); //WORD   biPlanes;         1
    fwrite(&bih.biBitCount      , 2, 1, file); //WORD   biBitCount        8
    fwrite(&bih.biCompression   , 4, 1, file); //DWORD  biCompression;    0
    fwrite(&bih.biSizeImage     , 4, 1, file); //DWORD  biSizeImage;      786432
    fwrite(&bih.biXPelsPerMeter , 4, 1, file); //LONG   biXPelsPerMeter;  1
    fwrite(&bih.biYPelsPerMeter , 4, 1, file); //LONG   biYPelsPerMeter;  1
    fwrite(&bih.biClrUsed       , 4, 1, file); //DWORD  biClrUsed;        0
    fwrite(&bih.biClrImportant  , 4, 1, file); //DWORD  biClrImportant;   0

    for (int i = 0 ; i < MAX_COLOR_8 ; i++) {
        fwrite(&m_pBitInfo    ->bmiColors[i].rgbBlue     , 1, 1, file);
        fwrite(&m_pBitInfo    ->bmiColors[i].rgbGreen    , 1, 1, file);
        fwrite(&m_pBitInfo    ->bmiColors[i].rgbRed      , 1, 1, file);
        fwrite(&m_pBitInfo    ->bmiColors[i].rgbReserved , 1, 1, file);
    }

    if (bih.biHeight < 0) {
        for(int y=0;y<m_iHeight;y++) {
            fwrite(m_pArea + m_iMemWidth*y , m_iMemWidth , 1, file);
        }
    }
    else {
        for(int y=m_iHeight-1;y>=0;y--) {
            fwrite(m_pArea + m_iMemWidth*y , m_iMemWidth , 1, file);
        }
    }

    fclose(file);

    return true ;

}

bool __stdcall CArea::ShowArea(HDC _hHdc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY , int _x, int _y )
{

    if(!m_pArea || m_iHeight <=0 || m_iWidth <= 0) { return false ; }

    //���� �ҽ�
    //CGdi *Gdi = new CGdi(_hHdc);

    //TransparentStretchBlt() 

    CGdi *Gdi = new CGdi(_hHdc , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY);

    //Gdi -> SetScale( _dScaleX , _dScaleY );

    TPoint Pnt ;
    unsigned char Px , PrePx;
    bool bCrntPx , bPrePx ;
    int iSttX ;



    for(int y = 0 ; y <  m_iHeight ; y++) {
        PrePx = 0 ;
        iSttX = 0 ;
        for(int x = 0 ; x <  m_iWidth ; x++) {

            Px = *(m_pArea   + (x + m_iMemWidth * y));
            if(Px != PrePx) {
                if(PrePx) {
                    Gdi -> m_tPen  .Color = Gdi -> m_tBrush.Color = GetColor(PrePx) ;
                    Gdi -> Rect(true , iSttX+_x , y+_y , x-1+_x , y+1+_y , 0);
                }
                if(Px) {
                    iSttX = x ;

                }
            }
            PrePx = Px ;
        }
    }

    delete Gdi ;

    return true ;
}



/*
bool __stdcall CArea::ShowAreaOnBlack(HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight , COLORREF _iColor)
{
    int iRet  ;
    int iRet2 = 0 ;

    if(!m_pArea) {iRet = -1 ; return iRet ; }
    //SetDIBitsToDevice(handle of device context                           ,
    //                  x-coordinate of upper-left corner of dest. rect.   ,
    //                  y-coordinate of upper-left corner of dest. rect.   ,
    //                  source rectangle width                             ,
    //                  source rectangle height                            ,
    //                  x-coordinate of lower-left corner of source rect.  ,
    //                  y-coordinate of lower-left corner of source rect.  ,
    //                  first scan line in array                           ,
    //                  number of scan lines                               ,
    //                  address of array with DIB bits                     ,
    //                  address of structure with bitmap info.             ,
    //                  RGB or palette indices                             );
    BITMAPINFO * DpInfo ;

    int iIfByteCnt    = sizeof(BITMAPINFOHEADER) ;
    int iIfByteCntPal = iIfByteCnt + MAX_COLOR_8*sizeof(RGBQUAD) ;


    SetStretchBltMode(_hHdc, COLORONCOLOR);
    if(m_iWidth == _iWidth && m_iHeight == _iHeight) iRet = SetDIBitsToDevice(_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , 0        , m_iHeight , m_pArea , m_pBitInfo , DIB_RGB_COLORS) ;
    else                                             iRet = StretchDIBits    (_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , m_iWidth , m_iHeight , m_pArea , m_pBitInfo , DIB_RGB_COLORS , SRCCOPY) ;

    return iRet ;
}
*/
//Set Area.
bool __stdcall CArea::SetArea(int _iWidth , int _iHeight)
{
    if(_iWidth < 0 || _iHeight < 0) return false ;

    SetSize(_iWidth,_iHeight);

    //memcpy(m_pArea  , _pArea  , _iWidth * _iHeight);
}

void __stdcall CArea::ClrArea()
{
    memset(m_pArea , 0 , m_iMemWidth * m_iHeight) ;
}

//�˻� ���� ���� �̹����� ũ�⿡�� ��� �������� �̹����������� ���� �ϴ� �Լ���
bool __stdcall CArea::SetRect (TRect *_pRect , int _iMargin )
{
    bool bRet = false ;
    if(_pRect -> left   < _iMargin ){_pRect -> left   = _iMargin ; bRet = true ;}if(_pRect -> left   >= m_iWidth  - _iMargin ){_pRect -> left   = m_iWidth  - _iMargin /*�̰Ը´µ�- 1*/ ;bRet = true ;}
    if(_pRect -> top    < _iMargin ){_pRect -> top    = _iMargin ; bRet = true ;}if(_pRect -> top    >= m_iHeight - _iMargin ){_pRect -> top    = m_iHeight - _iMargin /*�̰Ը´µ�- 1*/ ;bRet = true ;}
    if(_pRect -> right  < _iMargin ){_pRect -> right  = _iMargin ; bRet = true ;}if(_pRect -> right  >= m_iWidth  - _iMargin ){_pRect -> right  = m_iWidth  - _iMargin /*�̰Ը´µ�- 1*/ ;bRet = true ;}
    if(_pRect -> bottom < _iMargin ){_pRect -> bottom = _iMargin ; bRet = true ;}if(_pRect -> bottom >= m_iHeight - _iMargin ){_pRect -> bottom = m_iHeight - _iMargin /*�̰Ը´µ�- 1*/ ;bRet = true ;}

    return bRet ;
};

void __stdcall CArea::AddCircle (TCircle _tCircle, unsigned char _iVal)// int _iX , int _iY , int _iRad , unsigned char _iVal , bool _bFull)
{
    int iStartX = _tCircle.cx-_tCircle.rad ; int iEndX = _tCircle.cx+_tCircle.rad ;
    int iStartY = _tCircle.cy-_tCircle.rad ; int iEndY = _tCircle.cy+_tCircle.rad ;

    if(iStartX <  0        ) iStartX = 0 ;
    if(iStartX >= m_iWidth ) iStartX = m_iWidth - 1 ;

    if(iEndX   <  0        ) iEndX = 0 ;
    if(iEndX   >= m_iWidth ) iEndX = m_iWidth - 1 ;

    if(iStartY <  0        ) iStartY = 0 ;
    if(iStartY >= m_iHeight) iStartY = m_iHeight - 1 ;

    if(iEndY   <  0        ) iEndY = 0 ;
    if(iEndY   >= m_iHeight) iEndY = m_iHeight - 1 ;




    TDPoint Pnt1 , Pnt2 ;
    TPoint  Pnt3 , Pnt4 ;
    int     iRet ;


    const bool _bFull = true ;
    if(_bFull) {
        for(int y = iStartY ; y < iEndY ; y++) {
            iRet = CMath::GetCircleLineIntersection(iStartX, y, iEndX ,y , _tCircle.rad,_tCircle.cx,_tCircle.cy, &Pnt1 ,&Pnt2);
            Pnt3 = Pnt1.GetTPoint();
            Pnt4 = Pnt2.GetTPoint();
            if(iRet == 2) {
                memset((m_pArea+ (Pnt3.y * m_iMemWidth + Pnt3.x)), _iVal , Pnt4.x - Pnt3.x );
            }
            if(iRet == 1) {
                SetPixel(Pnt3.x,Pnt3.y,_iVal);
            }
        }
    }
    else {
        for(int x = iStartX ; x < iEndX ; x++) {
            iRet = CMath::GetCircleLineIntersection(x, iStartY, x ,iEndY , _tCircle.rad,_tCircle.cx,_tCircle.cy, &Pnt1 ,&Pnt2);
            if(iRet == 2) {
                SetPixel(CMath::RoundOff(Pnt2.x),CMath::RoundOff(Pnt2.y),_iVal);
                SetPixel(CMath::RoundOff(Pnt1.x),CMath::RoundOff(Pnt1.y),_iVal);
            }
            if(iRet == 1) {
                SetPixel(CMath::RoundOff(Pnt1.x),CMath::RoundOff(Pnt1.y),_iVal);
            }
        }
        for(int y = iStartY ; y < iEndY ; y++) {
            iRet = CMath::GetCircleLineIntersection(iStartX, y, iEndX ,y , _tCircle.rad,_tCircle.cx,_tCircle.cy, &Pnt1 ,&Pnt2);
            if(iRet == 2) {
                SetPixel(CMath::RoundOff(Pnt2.x),CMath::RoundOff(Pnt2.y),_iVal);
                SetPixel(CMath::RoundOff(Pnt1.x),CMath::RoundOff(Pnt1.y),_iVal);
            }
            if(iRet == 1) {
                SetPixel(CMath::RoundOff(Pnt1.x),CMath::RoundOff(Pnt1.y),_iVal);
            }
        }
    }
}

void __stdcall CArea::AddRect(TRect   _tRect  , unsigned char _iVal)
{
    for(int y = _tRect.top ; y < _tRect.bottom ; y++) {
        for(int x = _tRect.left ; x < _tRect.right ; x++) {
            SetPixel(x,y,_iVal);
        }
    }
}

void __stdcall CArea::AddLine (TDPoint _tPnt1 , TDPoint _tPnt2 , unsigned char _iVal )
{
    AddLine(_tPnt1.x , _tPnt1.y , _tPnt2.x , _tPnt2.y , _iVal);
}

void __stdcall CArea::AddLine (double _dX1 , double _dX2 , double _dY1 , double _dY2 , unsigned char _iVal )
{
    TDPoint tPnt1(_dX1 , _dY1);
    TDPoint tPnt2(_dX2 , _dY2);

    int iSttX = (_dX1 <  _dX2 ? _dX1 : _dX2)+0.5 ;
    int iEndX = (_dX1 <  _dX2 ? _dX2 : _dX1)+0.5 ;
    int iSttY = (_dY1 <  _dY2 ? _dY1 : _dY2)+0.5 ;
    int iEndY = (_dY1 <  _dY2 ? _dY2 : _dY1)+0.5 ;

    int iStt , iEnd ;

    int iXLength = iEndX - iSttX ;
    int iYLength = iEndY - iSttY ;

    if(iYLength < iXLength){ //������ ������. ���ȣ Y
        iStt = iSttX ;
        iEnd = iEndX ;
        for(int i = iStt ; i < iEnd ; i++) {
            SetPixel(i ,CMath::GetLineY(tPnt1,tPnt2,i),_iVal);
        }
    }
    else { //���� ������.
        iStt = iSttY ;
        iEnd = iEndY ;
        for(int i = iStt ; i < iEnd ; i++) {
            SetPixel(CMath::GetLineX(tPnt1,tPnt2,i),i ,_iVal);
        }
    }
}
/*

int  nodes, nodeX[MAX_POLY_CORNERS], pixelX, pixelY, i, j, swap ;

//  Loop through the rows of the image.
for (pixelY=IMAGE_TOP; pixelY<IMAGE_BOT; pixelY++) {

  //  Build a list of nodes.
  nodes=0; j=polyCorners-1;
  for (i=0; i<polyCorners; i++) {
    if (polyY[i]<(double) pixelY && polyY[j]>=(double) pixelY
    ||  polyY[j]<(double) pixelY && polyY[i]>=(double) pixelY) {
      nodeX[nodes++]=(int) (polyX[i]+(pixelY-polyY[i])/(polyY[j]-polyY[i])
      *(polyX[j]-polyX[i])); }
    j=i; }

  //  Sort the nodes, via a simple ��Bubble�� sort.
  i=0;
  while (i<nodes-1) {
    if (nodeX[i]>nodeX[i+1]) {
      swap=nodeX[i]; nodeX[i]=nodeX[i+1]; nodeX[i+1]=swap; if (i) i--; }
    else {
      i++; }}

  //  Fill the pixels between node pairs.
  for (i=0; i<nodes; i+=2) {
    if   (nodeX[i  ]>=IMAGE_RIGHT) break;
    if   (nodeX[i+1]> IMAGE_LEFT ) {
      if (nodeX[i  ]< IMAGE_LEFT ) nodeX[i  ]=IMAGE_LEFT ;
      if (nodeX[i+1]> IMAGE_RIGHT) nodeX[i+1]=IMAGE_RIGHT;
      for (pixelX=nodeX[i]; pixelX<nodeX[i+1]; pixelX++) fillPixel(pixelX,pixelY); }}}

*/


void __stdcall CArea::AddPolygon(TDPoint * _aPoints, int _iPointCnt , unsigned char _iVal)//AddPolygon2�� �ʹ� ������ ���θ��� �׸��� ��ĥ�ϱ�� �ٲ㺽.
{
    if(_iPointCnt < 3 || !_aPoints) return ;
    double dOutLeft   = FLT_MAX ;
    double dOutTop    = FLT_MAX ;
    double dOutRight  = 0 ;
    double dOutBottom = 0 ;

    for(int i = 0 ; i < _iPointCnt ; i++) {
        if(_aPoints[i].x < dOutLeft  ) dOutLeft   = _aPoints[i].x ;
        if(_aPoints[i].x > dOutRight ) dOutRight  = _aPoints[i].x ;
        if(_aPoints[i].y < dOutTop   ) dOutTop    = _aPoints[i].y ;
        if(_aPoints[i].y > dOutBottom) dOutBottom = _aPoints[i].y ;
    }

    if(dOutLeft  < 0) dOutLeft   = 0 ;
    if(dOutRight < 0) dOutRight  = 0 ;
    if(dOutTop   < 0) dOutTop    = 0 ;
    if(dOutBottom< 0) dOutBottom = 0 ;


    if(GetWidth () <= dOutLeft  )  dOutLeft   = GetWidth () ;
    if(GetWidth () <= dOutRight )  dOutRight  = GetWidth () ;
    if(GetHeight() <= dOutTop   )  dOutTop    = GetHeight() ;
    if(GetHeight() <= dOutBottom)  dOutBottom = GetHeight() ;


    CLinkedList<int> lEdgeX ;
    int  iPxX, iPxY , iEdgeX , iEdgeCnt;
    int * aEdgeX ;

    //  Loop through the rows of the image.
    for (iPxY=dOutTop; iPxY<dOutBottom; iPxY++) {

        //  Build a list of iEdgeCnt.
        for (int i=0 , j=_iPointCnt-1 ; i<_iPointCnt; i++) {
            if (_aPoints[i].y<(double) iPxY && _aPoints[j].y>=(double) iPxY ||
                _aPoints[j].y<(double) iPxY && _aPoints[i].y>=(double) iPxY) {
                iEdgeX = _aPoints[i].x+(iPxY-_aPoints[i].y)/(_aPoints[j].y-_aPoints[i].y)*(_aPoints[j].x-_aPoints[i].x);
                lEdgeX.PushBack(iEdgeX);
            }
            j=i;
        }

        iEdgeCnt = lEdgeX.GetDataCnt();
        aEdgeX = new int[iEdgeCnt];
        for (int i=0 ; i < iEdgeCnt ; i++){
            aEdgeX[i] = lEdgeX.GetCrntData(!i);
        }
        lEdgeX.DeleteAll();

        //  Sort the iEdgeCnt, via a simple ��Bubble�� sort.
        int i=0;
        int iSwapTemp ;
        while (i<iEdgeCnt-1) {
            if (aEdgeX[i]>aEdgeX[i+1]) {
                iSwapTemp  = aEdgeX[i];
                aEdgeX[i]  = aEdgeX[i+1];
                aEdgeX[i+1]= iSwapTemp;
                if (i) i--;
            }
            else {
                i++;
            }
        }

        //  Fill the pixels between node pairs.
        for (int i=0; i<iEdgeCnt; i+=2) {
            if (aEdgeX[i  ]>=dOutRight) break;
            if (aEdgeX[i+1]> dOutLeft ) {
                if (aEdgeX[i  ]< dOutLeft ) aEdgeX[i  ]=dOutLeft ;
                if (aEdgeX[i+1]> dOutRight) aEdgeX[i+1]=dOutRight;
                memset((m_pArea+ (iPxY * m_iMemWidth + aEdgeX[i])), _iVal , aEdgeX[i+1] - aEdgeX[i] );
                //for (iPxX=aEdgeX[i]; iPxX<aEdgeX[i+1]; iPxX++) { //���� 1000X
                //    SetPixel(iPxX,iPxY,_iVal);
                //}
            }
        }
        delete [] aEdgeX ;
    }
}

void __stdcall CArea::CopyPolygonFromArea( TDPoint * _aPoints, int _iPointCnt , CArea * _pSrcArea )//AddPolygon2�� �ʹ� ������ ���θ��� �׸��� ��ĥ�ϱ�� �ٲ㺽.
{
/*
    if(_iPointCnt < 3 || !_aPoints) return ;
    double dOutLeft   = FLT_MAX ;
    double dOutTop    = FLT_MAX ;
    double dOutRight  = 0 ;
    double dOutBottom = 0 ;

    for(int i = 0 ; i < _iPointCnt ; i++) {
        if(_aPoints[i].x < dOutLeft  ) dOutLeft   = _aPoints[i].x ;
        if(_aPoints[i].x > dOutRight ) dOutRight  = _aPoints[i].x ;
        if(_aPoints[i].y < dOutTop   ) dOutTop    = _aPoints[i].y ;
        if(_aPoints[i].y > dOutBottom) dOutBottom = _aPoints[i].y ;
    }

    if(GetWidth () <= dOutLeft  ) return ;
    if(GetWidth () <= dOutRight ) return ;
    if(GetHeight() <= dOutTop   ) return ;
    if(GetHeight() <= dOutBottom) return ;


    CLinkedList<int> lEdgeX ;
    int  iPxX, iPxY , iEdgeX , iEdgeCnt;
    int * aEdgeX ;

    //  Loop through the rows of the image.
    for (iPxY=dOutTop; iPxY<dOutBottom; iPxY++) {

        //  Build a list of iEdgeCnt.
        for (int i=0 , j=_iPointCnt-1 ; i<_iPointCnt; i++) {
            if (_aPoints[i].y<(double) iPxY && _aPoints[j].y>=(double) iPxY ||
                _aPoints[j].y<(double) iPxY && _aPoints[i].y>=(double) iPxY) {
                iEdgeX = _aPoints[i].x+(iPxY-_aPoints[i].y)/(_aPoints[j].y-_aPoints[i].y)*(_aPoints[j].x-_aPoints[i].x);
                lEdgeX.PushBack(iEdgeX);
            }
            j=i;
        }

        iEdgeCnt = lEdgeX.GetDataCnt();
        aEdgeX = new int[iEdgeCnt];
        for (int i=0 ; i < iEdgeCnt ; i++){
            aEdgeX[i] = lEdgeX.GetCrntData(!i);
        }
        lEdgeX.DeleteAll();

        //  Sort the iEdgeCnt, via a simple ��Bubble�� sort.
        int i=0;
        int iSwapTemp ;
        while (i<iEdgeCnt-1) {
            if (aEdgeX[i]>aEdgeX[i+1]) {
                iSwapTemp  = aEdgeX[i];
                aEdgeX[i]  = aEdgeX[i+1];
                aEdgeX[i+1]= iSwapTemp;
                if (i) i--;
            }
            else {
                i++;
            }
        }

        //  Fill the pixels between node pairs.
        for (int i=0; i<iEdgeCnt; i+=2) {
            if (aEdgeX[i  ]>=dOutRight) break;
            if (aEdgeX[i+1]> dOutLeft ) {
                if (aEdgeX[i  ]< dOutLeft ) aEdgeX[i  ]=dOutLeft ;
                if (aEdgeX[i+1]> dOutRight) aEdgeX[i+1]=dOutRight;
                memcpy(
                memset((m_pArea+ (iPxY * m_iMemWidth + aEdgeX[i])), _iVal , aEdgeX[i+1] - aEdgeX[i] );
                //for (iPxX=aEdgeX[i]; iPxX<aEdgeX[i+1]; iPxX++) { //���� 1000X
                //    SetPixel(iPxX,iPxY,_iVal);
                //}
            }
        }
        delete [] aEdgeX ;
    }
    */
}

void __stdcall CArea::AddPolygon2(TDPoint * _aPoints, int _iPointCnt , unsigned char _iVal) //��¥�� �׳� ���� ���µ� �ʹ� ����.... 1300 X 1000 ���� ũ�� 300ms
{
    if(_iPointCnt < 3 || !_aPoints) return ;
    double dOutLeft   = FLT_MAX ;
    double dOutTop    = FLT_MAX ;
    double dOutRight  = 0 ;
    double dOutBottom = 0 ;

    //double dInLeft    = 0 ;
    //double dInTop     = 0 ;
    //double dInRight   = 999999999 ;
    //double dInBottom  = 999999999 ;

    for(int i = 0 ; i < _iPointCnt ; i++) {
        if(_aPoints[i].x < dOutLeft  ) dOutLeft   = _aPoints[i].x ;
        if(_aPoints[i].x > dOutRight ) dOutRight  = _aPoints[i].x ;
        if(_aPoints[i].y < dOutTop   ) dOutTop    = _aPoints[i].y ;
        if(_aPoints[i].y > dOutBottom) dOutBottom = _aPoints[i].y ;

        //if(_aPoints[i].x > dInLeft   ) dInLeft    = _aPoints[i].x ;
        //if(_aPoints[i].x < dInRight  ) dInRight   = _aPoints[i].x ;
        //if(_aPoints[i].y > dInTop    ) dInTop     = _aPoints[i].y ;
        //if(_aPoints[i].y < dInBottom ) dInBottom  = _aPoints[i].y ;
    }

    //TRect InRect(dInLeft , dInTop , dInRight ,dInBottom);
    //AddRect(InRect  , _iVal);

    //bool bYPosInInRect = false ; //Y�������� InRect���̿� �ִ��� ����.

    for(int y = dOutTop ; y < dOutBottom ; y++) {
        //bYPosInInRect = dInTop < y && y < dInBottom ;
        for(int x = dOutLeft ; x < dOutRight ; x++) {
            //if(bYPosInInRect && dInLeft < x) x = dInRight ; 
            //if(CMath::IsPntInPolygon(_aPoints , x , y , _iPointCnt))SetPixel(x,y,_iVal);
        }
    }
}

void __stdcall CArea::AddPolygon3(TDPoint * _aPoints, int _iPointCnt , unsigned char _iVal)//AddPolygon2�� �ʹ� ������ ���Ξy�� ����ã�� ��ĥ�ϱ�� �ٲ㺽.
{
    if(_iPointCnt < 3 || !_aPoints) return ;
    double dOutLeft   = FLT_MAX ;
    double dOutTop    = FLT_MAX ;
    double dOutRight  = 0 ;
    double dOutBottom = 0 ;

    //double dInLeft    = 0 ;
    //double dInTop     = 0 ;
    //double dInRight   = 999999999 ;
    //double dInBottom  = 999999999 ;

    for(int i = 0 ; i < _iPointCnt ; i++) {
        if(_aPoints[i].x < dOutLeft  ) dOutLeft   = _aPoints[i].x ;
        if(_aPoints[i].x > dOutRight ) dOutRight  = _aPoints[i].x ;
        if(_aPoints[i].y < dOutTop   ) dOutTop    = _aPoints[i].y ;
        if(_aPoints[i].y > dOutBottom) dOutBottom = _aPoints[i].y ;

        //if(_aPoints[i].x > dInLeft   ) dInLeft    = _aPoints[i].x ;
        //if(_aPoints[i].x < dInRight  ) dInRight   = _aPoints[i].x ;
        //if(_aPoints[i].y > dInTop    ) dInTop     = _aPoints[i].y ;
        //if(_aPoints[i].y < dInBottom ) dInBottom  = _aPoints[i].y ;
    }

    //TRect InRect(dInLeft , dInTop , dInRight ,dInBottom);
    //AddRect(InRect  , _iVal);

    //bool bYPosInInRect = false ; //Y�������� InRect���̿� �ִ��� ����.




    for(int y = dOutTop ; y < dOutBottom ; y++) {
        //bYPosInInRect = dInTop < y && y < dInBottom ;
        for(int x = dOutLeft ; x < dOutRight ; x++) {
            //if(bYPosInInRect && dInLeft < x) x = dInRight ; 
            //if(CMath::IsPntInPolygon(_aPoints , x , y , _iPointCnt))SetPixel(x,y,_iVal);
            if(CMath::IsPntInPolygon(_aPoints, _iPointCnt, TDPoint(x,y))) SetPixel(x,y,_iVal) ;
        }
    }
}




/*
template<typename A1, typename A2>
void print(A1 a1, A2 a2)
{
    cout << a1 << endl;
    cout << a2 << endl;
}



template <typename T>
class CRunThread
{
    class TRunThread : public TThread
    {
        private:
            T Func;

        public:
            __fastcall TRunThread(T func) : TThread(false)
            {
                FreeOnTerminate = true;
                Func = func;
            }
            void __fastcall Execute()
            {
                Func();
            }
    };

    public:
        CRunThread(T func)
        {
            TRunThread*  thread = new TRunThread(func);
        }
};

typedef void (__closure *Func)();

//void __fastcall TForm1::Button3Click(TObject *Sender)
//{
//    Button3->Enabled = false ;
//    CRunThread<Func> * func = new CRunThread<Func>(&TestFunc) ;
//}
//
//void TForm1::TestFunc()
//{
//    // ��û �ð��� �ɸ��� ��ƾ.
//    int  k;
//    for(int c = 0; c < 299999999; c++)
//    {
//        k = c;
//        k++;
//        c = k;
//        c--;
//    }
//    Caption = k;
//    Button3->Enabled = true;
//}
//

*/
int GetCoreCnt()
{
    int iCoreCnt = 1 ;
    const bool bIntelCpu = true ;
    if(bIntelCpu){ //intel
        _asm{
            mov    eax,4
            mov    ecx,0
            cpuid
            shr    eax,26
            and    eax,0x3f
            add    eax,1
            mov    iCoreCnt,eax
        }
    }
    else { //amd
        _asm{
            mov    eax,0x80000008
            cpuid
            and    ecx,0xff
            add    ecx,1
            mov    iCoreCnt,eax
        }
    }

    return iCoreCnt ;
}
typedef void (__closure *FuncA2)(TRect ,int );
class CMopThread
{
    class TMopThread : public TThread
    {
        private:
            FuncA2 pFunc ;
            TRect  tRect ;
            int    iSize ;
            bool * pEnd  ;

        public:
            __fastcall TMopThread(FuncA2 _pFunc  , TRect _tRect , int _iSize ,bool * _pEnd) : TThread(false)
            {
                pEnd  = _pEnd  ;
                pEnd  = _pEnd  ;
                tRect = _tRect ;
                iSize = _iSize ;
                pFunc = _pFunc ;

                *pEnd =  false ;

                FreeOnTerminate = true;


            }
            __fastcall ~TMopThread()
            {
                *pEnd = true ;
            }
            void __fastcall Execute()
            {
                pFunc(tRect ,iSize);
            }
    };

    public:
        CMopThread(FuncA2 _pFunc  , TRect _tRect , int _iSize ,bool * _pEnd)
        {
            TMopThread*  thread = new TMopThread(_pFunc ,_tRect, _iSize  , _pEnd);
        }
};

void CArea::ErosionThread(TRect _tRect , int _iSize)
{
    bool cCrntPx , cRCrntPx; //CrntPx
    bool cPrePx  , cRPrePx ; //pre px
    bool IsUpEg  = false ;
    bool IsRUpEg = false ;
    int  iErosionCnt  = 0 ;
    int  iRErosionCnt = 0 ;

    //X���� ���μ�.
    for (int y = _tRect.top ; y < _tRect.bottom ; y ++) {
        iErosionCnt  = 0 ;
        iRErosionCnt = 0 ;
        cCrntPx  = *(m_pArea + (_tRect.left    + m_iMemWidth * y));
        cRCrntPx = *(m_pArea + (_tRect.right-1 + m_iMemWidth * y));
        for (int x = _tRect.left , rx = _tRect.right-1 ; x < _tRect.right ; x ++ , rx--) {

            //���ʿ��� ������.
            cPrePx  = cCrntPx ;
            cCrntPx = *(m_pArea + (x + m_iMemWidth * y));
            IsUpEg = !cPrePx &&  cCrntPx ;
            if(IsUpEg || iErosionCnt){
               if(IsUpEg)iErosionCnt = _iSize ;
               *(m_pArea   + (x + m_iMemWidth * y)) = 0 ;
               iErosionCnt-- ;
            }

            //�����ʿ��� ����.
            cRPrePx  = cRCrntPx ;
            cRCrntPx = *(m_pArea   + (rx + m_iMemWidth * y));
            IsRUpEg = !cRPrePx &&  cRCrntPx ;
            if(IsRUpEg || iRErosionCnt){
               if(IsRUpEg)iRErosionCnt = _iSize ;
               *(m_pArea   + (rx + m_iMemWidth * y)) = 0 ;
               iRErosionCnt-- ;
            }
        }
    }

        //Y���� ���μ�.
    for (int x = _tRect.left ; x < _tRect.right ; x ++ ) {
        //�Ʒ�����.
        iErosionCnt  = 0 ;
        iRErosionCnt = 0 ;
        cCrntPx  = *(m_pArea + (x + m_iMemWidth *  _tRect.top      ));
        cRCrntPx = *(m_pArea + (x + m_iMemWidth * (_tRect.bottom-1)));
        for (int y = _tRect.top, ry = _tRect.bottom-1 ; y < _tRect.bottom ; y ++ ,ry--) {

            //�Ʒ�����.
            cPrePx  = cCrntPx ;
            cCrntPx = *(m_pArea + (x + m_iMemWidth * y));
            IsUpEg = !cPrePx &&  cCrntPx ;
            if(IsUpEg || iErosionCnt){
               if(IsUpEg)iErosionCnt = _iSize ;
               *(m_pArea   + (x + m_iMemWidth * y)) = 0 ;
               iErosionCnt-- ;
            }

            //������.
            cRPrePx  = cRCrntPx ;
            cRCrntPx = *(m_pArea   + (x + m_iMemWidth * ry));
            IsRUpEg = !cRPrePx &&  cRCrntPx ;
            if(IsRUpEg || iRErosionCnt){
               if(IsRUpEg)iRErosionCnt = _iSize ;
               *(m_pArea   + (x + m_iMemWidth * ry)) = 0 ;
               iRErosionCnt-- ;
            }
        }
    }
}

void __stdcall CArea::Erosion(TDRect _tRect , int _iSize)
{
    TRect tRect = _tRect.GetTRect();
    SetRect(&tRect);
    if(_iSize < 1 ) _iSize = 1 ;


    const int iMaxThread = 0 ;

    if(iMaxThread >0){
        bool * aEnd = new bool[iMaxThread];

        int iRange = _tRect.Height() ;
        int iSttY = 0, iEndY = 0;
        for(int i = 0 ; i < iMaxThread ; i++){
            iSttY = iEndY ;
            iEndY = iRange * (i+1)/ iMaxThread ;
            new CMopThread(&ErosionThread , TRect(_tRect.left,iSttY,_tRect.right,iEndY),_iSize , aEnd+i) ;
        }


        bool bAllEnded = true ;
        while(1){
            bAllEnded = true ;
            for(int i = 0 ; i < iMaxThread ; i++) {
                if(!aEnd[i])bAllEnded= false ;
            }
            if(bAllEnded)break ;
            Sleep(0);
        }
        delete [] aEnd ;
    }
    else {
        ErosionThread(tRect , _iSize);
    }
}


void __stdcall CArea::FindBaundary(unsigned char * _pPxEdge , TRect _tRect) //�׵θ��� ���鼭 ���� ����. ��.
{
                                  // 1    , LOOT_2 , 1    , LOOT_2 , 1     , LOOT_2, 1    , LOOT_2
    register const POINT nei[8] = { {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };

    memset(_pPxEdge , 0 , _tRect.Width() * _tRect.Height()) ;
    #define VPixel(x,y) (*(_pPxEdge + x + _tRect.Width() * y))

    //Local
    bool bCrntPx ; //CrntPx
    bool pPrePx  ; //PastPx
    //bool bcp ; //Back Step crnt pixel.
    //bool bnp ; //Back step next pixel.
    bool IsDnEg = false ;
    bool IsUpEg = false ;
    bool IsSide = false ;
    bool bSidePx = false ;

    register int iTraceSttX    ; // boundary  start X
    register int iTraceSttY    ; // boundary  startf y
    register int iNeiborCnt  ;
    register int iTraceX = 0 ; // Trace X
    register int iTraceY = 0 ;
    register int iNeiborX    ; // Trace neiborhood.
    register int iNeiborY    ;

    for (int iY = _tRect.top ; iY < _tRect.bottom ; iY ++ ) {
        bCrntPx = GetPixelDirect(_tRect.left,iY);//*(m_pArea   + (_tRect.left + m_iMemWidth * iY));
        for (int iX = _tRect.left ; iX < _tRect.right ; iX++ ) {
            
            pPrePx  = bCrntPx ;
            bCrntPx = GetPixelDirect(iX,iY);//*(m_pArea   + (iX + m_iMemWidth * iY));

            IsUpEg =  !pPrePx  && bCrntPx ;//&& !_tPara.bFindLtOnDk && !VPixel((iX-_tRect.left ),(iY-_tRect.top)) ;

            //IsEdge?
            if(IsUpEg && !VPixel((iX-_tRect.left ),(iY-_tRect.top))) {  // (*(pVPixel + x + _tRect.Width() * y))
                IsSide = false ;
                iTraceX = iTraceSttX = iX ;
                iTraceY = iTraceSttY = iY ;

                iNeiborCnt = 5 ;
                int k ;
                do {
                    IsSide = false ;
                    //Find next pixel.         ���� Ȥ�� ���� 8�������� �������� 8�������� ���� ã�ƾ� �����ʳ�???=>�ƴϱ� LB���� �˻��̶� ��� ����.
                    for (k=0; k<8; k++, iNeiborCnt=((iNeiborCnt+1) & 7)) {
                        iNeiborX = iTraceX + nei[iNeiborCnt].x ;
                        iNeiborY = iTraceY + nei[iNeiborCnt].y ;
                        if (iNeiborX<_tRect.left || iNeiborX>=_tRect.right ||
                            iNeiborY<_tRect.top  || iNeiborY>=_tRect.bottom){
                            IsSide  = true ;
                            bSidePx = true ;
                            continue;
                        }
                        if (bCrntPx == GetPixelDirect(iNeiborX,iNeiborY) > 0 ) {  //_pImg->GetPixel(iNeiborX,iNeiborY)
                            break;
                        }
                    }

                    VPixel((iTraceX-_tRect.left),(iTraceY-_tRect.top)) = true;

                    iTraceX = iNeiborX ;
                    iTraceY = iNeiborY ;

                    iNeiborCnt = (iNeiborCnt+5) & 7;
                }
                while(!(iTraceSttX==iTraceX && iTraceSttY==iTraceY)&& k<8);

                //if(IsSide ) break ; //��迡 ��ó �ִ� ��� ����.

            }
        }
    }
}

void __stdcall CArea::ErosionTrace(TDRect _tRect , int _iSize) //�׵θ��� ���鼭 ��.
{
    TRect tRect = _tRect.GetTRect();
    SetRect(&tRect);
    if(_iSize < 1 ) _iSize = 1 ;

                                  // 1    , LOOT_2 , 1    , LOOT_2 , 1     , LOOT_2, 1    , LOOT_2
    register const POINT nei[8] = { {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };

    unsigned char * pVPixel = new unsigned char[tRect.Width() * tRect.Height()] ; //�湮 �ȼ�. �� ������..
    #define VPixel(x,y) (*(pVPixel + x + tRect.Width() * y))
    FindBaundary(pVPixel , tRect);  //���� �׵θ��� ã�´�... �y���߿� ���� �ȼ��� ��ǥ��.

    int iYSizeN , iYSizeP ;
    int iXSizeN , iXSizeP ;
    int iTemp ;

    for (int iY = tRect.top ; iY < tRect.bottom-_iSize ; iY ++ ) {
        for (int iX = tRect.left ; iX < tRect.right-_iSize ; iX++ ) {
            if(VPixel((iX-tRect.left),(iY-tRect.top))){
                iXSizeN = (_iSize < tRect.left   + iX) ? _iSize : tRect.left   + iX;
                iXSizeP = (_iSize < tRect.right  - iX) ? _iSize : tRect.right  - iX;

                iYSizeN = (_iSize < tRect.top    + iY) ? _iSize : tRect.top    + iY;//tRect.bottom - iX;
                iYSizeP = (_iSize < tRect.bottom - iY) ? _iSize : tRect.bottom - iY;//tRect.bottom - iX;

                for(int y = (-iYSizeN)+1 ; y < iYSizeP ; y++) {
                    for(int x = (-iXSizeN)+1 ; x < iXSizeP ; x++) {
                        //if((iX + x) < 0 || (iX + x) > tRect.Width ()){
                        //    iTemp++ ;
                        //}
                        //if((iY + y) < 0 || (iY + y) > tRect.Height()){
                        //    iTemp++ ;
                        //}
                        GetPixelDirect((iX + x),(iY + y))=0;
                    }
                }
            }
        }
    }


    delete[] pVPixel ;


}

void __stdcall CArea::DilationTrace(TDRect _tRect , int _iSize, unsigned char _cPx) //�׵θ��� ���鼭 ��.
{
    TRect tRect = _tRect.GetTRect();
    SetRect(&tRect);
    if(_iSize < 1 ) _iSize = 1 ;

                                  // 1    , LOOT_2 , 1    , LOOT_2 , 1     , LOOT_2, 1    , LOOT_2
    register const POINT nei[8] = { {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };

    unsigned char * pVPixel = new unsigned char[tRect.Width() * tRect.Height()] ; //�湮 �ȼ�. �� ������..
    #define VPixel(x,y) (*(pVPixel + x + tRect.Width() * y))
    FindBaundary(pVPixel , tRect);  //���� �׵θ��� ã�´�... �y���߿� ���� �ȼ��� ��ǥ��.

    int iYSizeN , iYSizeP ;
    int iXSizeN , iXSizeP ;
    int iTemp ;
    unsigned char pPixel ;

    for (int iY = tRect.top ; iY < tRect.bottom-_iSize ; iY ++ ) {
        for (int iX = tRect.left ; iX < tRect.right-_iSize ; iX++ ) {
            if(VPixel((iX-tRect.left),(iY-tRect.top))){
                iXSizeN = (_iSize < tRect.left   + iX) ? _iSize : tRect.left   + iX;
                iXSizeP = (_iSize < tRect.right  - iX) ? _iSize : tRect.right  - iX;

                iYSizeN = (_iSize < tRect.top    + iY) ? _iSize : tRect.top    + iY;//tRect.bottom - iX;
                iYSizeP = (_iSize < tRect.bottom - iY) ? _iSize : tRect.bottom - iY;//tRect.bottom - iX;

                for(int y = -iYSizeN ; y < iYSizeP+1 ; y++) {
                    for(int x = -iXSizeN ; x < iXSizeP+1 ; x++) {
                        GetPixelDirect((iX + x),(iY + y))=_cPx;
                    }
                }
            }
        }
    }


    delete[] pVPixel ;


}



