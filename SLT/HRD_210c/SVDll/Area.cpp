//---------------------------------------------------------------------------
#include <Dialogs.hpp>
#include <jpeg.hpp>
#include <stdio.h>

#pragma hdrstop

#include "Area.h"
#include "SVDefine.h"
#include "SVMaths.h"
#include "GdiUnit.h"




//
//---------------------------------------------------------------------------

#pragma package(smart_init)

CArea::CArea()
{
    m_pArea  = NULL ;
                                                              
    m_pBitInfo   = NULL ;

    m_iWidth  = 0  ;
    m_iHeight = 0  ;

    m_iMemWidth = 0 ;

    //8Bit color pallette
    BYTE R,G,B ;
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
    /*
    for (int i=0; i<MAX_COLOR_8; i++) {   //if (i==  0          ){m_iPxColor[i] = 0x
             if (i==  0          ){R = 0           ;G = 0            ;B = 0        ;}
        else if (i>   0 && i<= 51){R = 255         ;G = i*5          ;B = 0        ;}
        else if (i>= 52 && i<=102){R = 255-(i-52)*5;G = 255          ;B = 0        ;}
        else if (i>=103 && i<=153){R = 0           ;G = 255          ;B = (i-103)*5;}
        else if (i>=154 && i<=203){R = 0           ;G = 255-(i-154)*5;B = 255      ;}
        else if (i>=204 && i<=254){R = (i-204)*5   ;G = 0            ;B = 255      ;}
        else                      {R = 0           ;G = 0            ;B = 0        ;}
        m_iPxColor[i] = RGB(R,G,B);   */
    }
}

CArea::~CArea()
{
    Del();


}

void __stdcall CArea::Del()
{
    delete [] (BYTE *)m_pBitInfo   ;

    if(m_pArea  ){delete [] m_pArea  ;}
}

int __stdcall CArea::BitToByte(int _BitCnt) { return _BitCnt / BYTE_BIT_COUNT ;}


//인라인.
//bool __stdcall CArea::CheckRangeOver(int _x , int _y) { return (0>_x || _x>=m_iWidth || 0>_y || _y>=m_iHeight);}

/* 인라인.
unsigned char __stdcall CArea::GetPixel (int _x , int _y )
{
    if(!m_pArea   || CheckRangeOver(_x,_y))return 0;
    return *(m_pArea   + (_x + m_iMemWidth * _y));
};  //픽셀밝기값을 얻어 온다.

void __stdcall CArea::SetPixel (int _x , int _y , unsigned char _v)
{
    if(!m_pArea   || CheckRangeOver(_x,_y))return ;
    *(m_pArea   + (_x + m_iMemWidth * _y))= _v ;


    //값을 세팅 할때는 세팅 하고
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
};  //픽셀밝기값을 세팅한다.
*/


unsigned char * __stdcall CArea::GetArea(){ if(!m_pArea  ) return NULL ; return m_pArea   ; };  //이미지포인터 리턴.

//Get 함수.
int  __stdcall CArea::GetHeight(){return m_iHeight ;}
int  __stdcall CArea::GetWidth (){return m_iWidth  ;}
int  __stdcall CArea::GetMemWidth(){return m_iMemWidth ;}

int  __stdcall CArea::GetCntVal(unsigned char _v)
{
    int iCnt ;
    for(int y = m_iStartY ; y < m_iEndY ; y++) {
        for(int x = m_iStartY ; x < m_iEndY ; x++) {
           if(GetPixel(x,y)==_v) iCnt++;
        }
    }

    return iCnt ;
}

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
}

/*인라인.
COLORREF __stdcall CArea::GetColor(int _iPx)
{
    return m_iPxColor[_iPx] ;

    //     if (i>=  0 && i<= 51){r = 255         ;g = i*5          ;b = 0        ;}
    //else if (i>= 52 && i<=102){r = 255-(i-52)*5;g = 255          ;b = 0        ;}
    //else if (i>=103 && i<=153){r = 0           ;g = 255          ;b = (i-103)*5;}
    //else if (i>=154 && i<=203){r = 0           ;g = 255-(i-154)*5;b = 255      ;}
    //else if (i>=204 && i<=254){r = (i-204)*5   ;g = 0            ;b = 255      ;}
    //else                      {r = 0           ;g = 0            ;b = 0        ;}

    //return (TColor)RGB(r,g,b) ;

}
*/



bool __stdcall CArea::SetSize(int _iWidth , int _iHeight)
{
    int iActWidth ;
    if(_iWidth % 4) iActWidth = (_iWidth / 4)*4+4 ;
    else            iActWidth =  _iWidth ;

    if(m_iWidth != _iWidth || m_iHeight != _iHeight) {
        SetPalette(_iWidth , _iHeight);
        if(m_pArea  ){delete [] m_pArea  ;}

        m_pArea  = new unsigned char [iActWidth * _iHeight ] ;
    }
    m_iWidth    = _iWidth   ;
    m_iHeight   = _iHeight  ;
    m_iMemWidth = iActWidth ;

    memset(m_pArea , 0 ,iActWidth * _iHeight );

    //m_iXOffset
    //m_iYOffset
    m_iStartX = _iWidth ;
    m_iStartY = _iHeight ;
    m_iEndX   = 0 ;
    m_iEndY   = 0 ;

    return true ;
}

//bool __stdcall SetOffset(int _iX , int _iY)
//{
//    m_iXOffset = _iX ;
//    m_iYOffset = _iY ;
//}

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
        ShowMessage("파일을 열수 없습니다.");
        return false ;
    }

    BITMAPFILEHEADER  bfh ;
    BITMAPINFOHEADER  bih ;

//  원래 32bit CPU  시스템에서 메모리할당을 32bit씩 합니다.
//  위와 같은 구조체를 그냥 sizeof하면 16byte가 나옵니다
//  원래 14바이트 인BITMAPFILEHEADER를 sizeof(BITMAPFILEHEADER) 해 보십시요 16 이 나올겁니다.

    fread(&bfh.bfType      , 2, 1, file);     // 파일의 종류 아스키 ex) BMP=19778   JPG=55551
    fread(&bfh.bfSize      , 4, 1, file);     // 파일의 전체 크기   ex) 787510     4194358
    fread(&bfh.bfReserved1 , 2, 1, file);     // 사용안함 0으로 세팅
    fread(&bfh.bfReserved2 , 2, 1, file);     // 사용안함 0으로 세팅
    fread(&bfh.bfOffBits   , 4, 1, file);     // 비트맵 파일 해더서 부터 실제 데이터까지의 바이트거리. 팔레트 있으면1078 첪으면 54

    if(bfh.bfType != 0x4d42){ShowMessage("비트맵 이미지가 아닙니다."); fclose(file); return false ;}

    int iTemp  = sizeof(BITMAPFILEHEADER) - 2; //빌더 버그 32비트 cpu라 4바이트씩 할당하여 원래 14바이트 크기인데 16으로 나옴.
                                      
    fread(&bih.biSize          , 4, 1, file); //DWORD  biSize;           40     BITMAPINFOHEADER 구조체의 크기 최소 40이상.
    fread(&bih.biWidth         , 4, 1, file); //LONG   biWidth;          1024   이미지 넓이
    fread(&bih.biHeight        , 4, 1, file); //LONG   biHeight;         768    + 일 경우 아래에서 위로 증가, -일 경우 위에서 아래로 증가.. biWidth는 상관없음 왼쪽에서 오른쪽으로 증가.
    fread(&bih.biPlanes        , 2, 1, file); //WORD   biPlanes;         1      컬러 Plane의 수.(???)
    fread(&bih.biBitCount      , 2, 1, file); //WORD   biBitCount        8      몇비트 이미지인지(1,4,8,16,24).  (단색,16색,256색,24비트)
    fread(&bih.biCompression   , 4, 1, file); //DWORD  biCompression;    0      BI_RGB(압축되지 않은 비트맵), BI_RLE8이면 8비트 압축, BI_RLE4이면 4비트 압축
    fread(&bih.biSizeImage     , 4, 1, file); //DWORD  biSizeImage;      786432 이미지 크기를 바이트 단위로 나타내며, BI_RGB 일 경우 0
    fread(&bih.biXPelsPerMeter , 4, 1, file); //LONG   biXPelsPerMeter;  1      수평해상도.
    fread(&bih.biYPelsPerMeter , 4, 1, file); //LONG   biYPelsPerMeter;  1      수직해상도.
    fread(&bih.biClrUsed       , 4, 1, file); //DWORD  biClrUsed;        0      비트맵에 사용된 색상수를 의미하며, 이값에 따라 RGBQUAD의 배열을 메모리 할당하여서 읽어오시면 됩니다. 이 값이 0이면 모든 색깔을 다 사용한다는 의미입니다.
    fread(&bih.biClrImportant  , 4, 1, file); //DWORD  biClrImportant;   0      비트맵을 출력하는데 필수적인 색상수를 나타내며, 이 값이 0이면 모든 색상을 다 사용한다는 의미입니다.

    int iTemp1 = sizeof(BITMAPINFOHEADER) ;

    if(bih.biBitCount != 8){ShowMessage("8비트이미지가 아닙니다."); fclose(file); return false ;} //8비트 혹은 24비트 아니면 로딩 안됨.

    //시발 ... 반전 안할려면 밑에 메모리 쓰는것 뒤집고.
    //화면에 이미지 뿌려줄때 보내는 비트맵인포헤더에서 높이 반전해 줘야 해서 그냥 쓴다.
    bih.biHeight = -bih.biHeight;

    SetSize(bih.biWidth , abs(bih.biHeight)); //m_pBitmapInfo이거 건드리기 전에 이미지 버퍼 싸이즈 맞게 바꿈.

    //팔레트 리딩.
    RGBQUAD Temp ;
    for (int i = 0 ; i < MAX_COLOR_8 ; i++){

        fread(&Temp , 1, 1, file);
        fread(&Temp , 1, 1, file);
        fread(&Temp , 1, 1, file);
        fread(&Temp , 1, 1, file);

        //비트맵파일에 있는 팔레트 안씀.
        //fread(&m_pBitInfo8 ->bmiColors[i].rgbBlue     , 1, 1, file);
        //fread(&m_pBitInfo8 ->bmiColors[i].rgbGreen    , 1, 1, file);
        //fread(&m_pBitInfo8 ->bmiColors[i].rgbRed      , 1, 1, file);
        //fread(&m_pBitInfo8 ->bmiColors[i].rgbReserved , 1, 1, file);
    }

    int iTemp2   =  sizeof(RGBQUAD) * MAX_COLOR_8 ;
    int iTemp012 = iTemp + iTemp1 + iTemp2 ;


//이미지 데이터 추출.

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




    /*
    if (bih.biHeight < 0) {
        for(int y=m_iHeight-1;y>=0;y--) {
            fread(m_pArea + y * m_iWidth , bih.biWidth, 1, file);
        }
    }

    else {
        BYTE * pLine = new BYTE [bih.biWidth] ;
        for(int y=0;y<m_iHeight;y++) {
            fread(pLine , bih.biWidth , 1 , file);
            memcpy(m_pArea + y * m_iWidth , pLine , bih.biWidth);
        }
        delete [] pLine ;
    }*/

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
    if(file == NULL) {ShowMessage("영역을 저장 할 수 없습니다."); return false ;}

    //파레트 색깔 최종으로 갱신.
    SetPaletteColor();

    BITMAPFILEHEADER  bfh ;
    BITMAPINFOHEADER  bih ;

    bih = m_pBitInfo -> bmiHeader ;

    bih.biHeight =  m_iHeight ;
    bih.biWidth  =  m_iWidth  ;

    const int iMaxPx = pow(2,bih.biBitCount) ;  //한픽셀이 표현 할수 있는 색수 ^
    const int iSizeOfBfh  = 14 ; //sizeof(BITMAPFILEHEADER) ; 빌더버그 있어서 14인데 16으로 나옴.
    const int iSizeOfPlt  = sizeof(RGBQUAD) * iMaxPx ;
    //biSizeImage = ((((bih.biWidth * m_iBitCnt) + 31) & ~31) >> 3) * bih.biHeight 귀찮아서 4바이트 단위 고려안함.
    const int iSizeOfImg  = m_iMemWidth * bih.biHeight ;
    const int iOftToImg   = iSizeOfBfh + sizeof(BITMAPINFOHEADER) + iSizeOfPlt ;
    const int iSizeOfFile = iSizeOfBfh + sizeof(BITMAPINFOHEADER) + iSizeOfImg + iSizeOfPlt ;

    bfh.bfType          = 0x4d42;        // BM => Bitmap file
    bfh.bfSize          = iSizeOfFile ;  // FileSize.
    bfh.bfReserved1     = 0;             // not use
    bfh.bfReserved2     = 0;             // not use
    bfh.bfOffBits       = iOftToImg;     // 비트맵 파일 해더서 부터 실제 데이터까지의 바이트거리. 팔레트 있으면1078 첪으면 54

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

    /*
    BYTE a ;
    for (int i = 0 ; i < MAX_COLOR_8 ; i++) {
        a = m_pBitInfo    ->bmiColors[i].rgbBlue     ;
        a = m_pBitInfo    ->bmiColors[i].rgbGreen    ;
        a = m_pBitInfo    ->bmiColors[i].rgbRed      ;
        a = m_pBitInfo    ->bmiColors[i].rgbReserved ;
        fwrite(&m_pBitInfo    ->bmiColors[i].rgbBlue     , 1, 1, file);
        fwrite(&m_pBitInfo    ->bmiColors[i].rgbGreen    , 1, 1, file);
        fwrite(&m_pBitInfo    ->bmiColors[i].rgbRed      , 1, 1, file);
        fwrite(&m_pBitInfo    ->bmiColors[i].rgbReserved , 1, 1, file);
    }
    for(int y=0;y<m_iHeight;y++) {
        fwrite(m_pArea + m_iWidth*y , m_iWidth , 1, file);
    }
    */


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

bool __stdcall CArea::ShowArea(HDC _hHdc , int _x, int _y, float _fScaleX , float _fScaleY )
{
    if(!m_pArea || m_iHeight <=0 || m_iWidth <= 0) { return false ; }

    CGdi *Gdi = new CGdi(_hHdc);
    Gdi -> SetScale( _fScaleX , _fScaleY );

    TPoint Pnt ;

    for(int y = 0 ; y <  m_iHeight ; y++) {
        for(int x = 0 ; x <  m_iWidth ; x++) {
            if(GetPixel(x,y))
                if(GetColor(GetPixel(x,y))) Gdi -> Pixel(x+_x,y+_y,GetColor(GetPixel(x,y))) ;

        }
    }

    delete Gdi ;

    return true ;
}

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

HBITMAP __stdcall CArea::GetHBitmapOnBlack(HDC _hHdc)
{
    if (!m_pArea) return NULL;

    hBitmap = CreateDIBitmap(_hHdc, &m_pBitInfo->bmiHeader , 0, NULL, NULL, 0);
    SetDIBits(_hHdc, hBitmap, 0, m_iHeight , (BYTE*)m_pArea , m_pBitInfo , DIB_RGB_COLORS);

    return hBitmap;
}

void __stdcall CArea::ShowBitmapOnBlack(HDC _hHdc, int _x, int _y, int _iWidth, int _iHeight)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    BITMAP bit;

    hBitmap = GetHBitmapOnBlack(_hHdc) ;

    hMemDC = CreateCompatibleDC(_hHdc);
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    GetObject(hBitmap, sizeof(BITMAP), &bit);
    SetStretchBltMode(_hHdc, COLORONCOLOR);
    if (_iWidth==bit.bmWidth && _iHeight==bit.bmHeight) BitBlt    (_hHdc,  0,  0, _iWidth, _iHeight, hMemDC, 0, 0, SRCCOPY);
    else                                                StretchBlt(_hHdc, _x, _y, _iWidth, _iHeight, hMemDC, 0, 0, _iWidth, _iHeight, SRCCOPY);

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}

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




    TFPoint Pnt1 , Pnt2 ;
    int     iRet ;


    const bool _bFull = true ;
    if(_bFull) {
        for(int y = iStartY ; y < iEndY ; y++) {
            for(int x = iStartX ; x < iEndX ; x++) {
                if(MATH_GetCircleInPoint(_tCircle.cx , _tCircle.cy , _tCircle.rad , x , y))SetPixel(x,y,_iVal);
            }
        }
    }
    else {
        for(int x = iStartX ; x < iEndX ; x++) {
            iRet = MATH_GetCircleLineIntersection(x, iStartY, x ,iEndY , _tCircle.rad,_tCircle.cx,_tCircle.cy, &Pnt1 ,&Pnt2);
            if(iRet == 2) {
                SetPixel(MATH_RoundOff(Pnt2.x),MATH_RoundOff(Pnt2.y),_iVal);
                SetPixel(MATH_RoundOff(Pnt1.x),MATH_RoundOff(Pnt1.y),_iVal);
            }
            if(iRet == 1) {
                SetPixel(MATH_RoundOff(Pnt1.x),MATH_RoundOff(Pnt1.y),_iVal);
            }
        }
        for(int y = iStartY ; y < iEndY ; y++) {
            iRet = MATH_GetCircleLineIntersection(iStartX, y, iEndX ,y , _tCircle.rad,_tCircle.cx,_tCircle.cy, &Pnt1 ,&Pnt2);
            if(iRet == 2) {
                SetPixel(MATH_RoundOff(Pnt2.x),MATH_RoundOff(Pnt2.y),_iVal);
                SetPixel(MATH_RoundOff(Pnt1.x),MATH_RoundOff(Pnt1.y),_iVal);
            }
            if(iRet == 1) {
                SetPixel(MATH_RoundOff(Pnt1.x),MATH_RoundOff(Pnt1.y),_iVal);
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


