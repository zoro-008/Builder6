//---------------------------------------------------------------------------
#include <Dialogs.hpp>
#include <math.h>
#include <stdio.h>

#pragma hdrstop

#include "Image.h"
#include "SVMaths.h"

#include "Common.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

#define MAX_COLOR_8 256                                               //8비트 영상 색상수.
#define BMP_INFO_SIZE_8  sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD) //팔레트 가지고 있는 256컬러 인포 크기.
#define BMP_INFO_SIZE_24 sizeof(BITMAPINFO)                           //펄레트 필요 없는 24비트 인포 크기.


int  __stdcall CImage::BitToByte (int _BitCnt) { return _BitCnt / BYTE_BIT_COUNT ;}
unsigned char * __stdcall CImage::GetImg (){ if(!m_pImage ) return NULL ; return m_pImage  ; };  //이미지포인터 리턴.
unsigned char * __stdcall CImage::GetImgC(){ if(!m_pImageC) return NULL ; return m_pImageC ; };  //이미지포인터 리턴.


int  __stdcall CImage::GetHeight(){return m_iHeight ;}
int  __stdcall CImage::GetWidth (){return m_iWidth  ;}
int  __stdcall CImage::GetImgBit(){return m_iBitCnt ;}
int  __stdcall CImage::GetMem8Width(){return m_iMemWidth8;}
int  __stdcall CImage::GetMem24Width(){return m_iMemWidth24;}


float __stdcall CImage::GetScaleX() { return m_fScaleX ;}
float __stdcall CImage::GetScaleY() { return m_fScaleY ;}

int __stdcall CImage::GetImgX(int _x){ return _x/GetScaleX();}
int __stdcall CImage::GetImgY(int _y){ return _y/GetScaleY();}

CImage::CImage()
{
    m_pImage   = NULL ;
    m_pImageC  = NULL ;

    //ss m_pBitInfoD  = NULL ;
    m_pBitInfo8  = NULL ;
    m_pBitInfo24 = NULL ;

    m_iWidth  = 0  ;
    m_iHeight = 0  ;
    m_iBitCnt = 8  ;
    m_iMemWidth8 = 0 ;
    m_iMemWidth24 = 0 ;


    //InitializeCriticalSection(&g_pTemp);

    //SetSize(640,480,8); //1,0,0,1
    //SetSize(4096,4096,24); //1,0,0,1
    //SetSize(1600,1200,24); //1,0,1,0

    //SetSize(1600,1200,24); //1,0,1,0
    //SetSize(1280 , 1024 , 24);// 1,0,0,0
    //SetSize(1280 , 1024 , 8); 1,0,0,0
    //SetSize(640 , 480 , 8); 1,0,0,0

    //SetSize(0 , 0 , 8);
    //SetSize(1024 , 768 , 8); 1,1,0,0

    //static int i = 0 ;
    //if(i==0)SetSize(1024 , 768 , 8); 
    //if(i==1)SetSize(1024 , 768 , 8);
    //if(i==2)SetSize(1600 , 768 , 24);
    //if(i==3)SetSize(1024 , 1200 , 8);
    //i++;

    //SetSize(640,480,8); //1,0,0,0

    //SetSize(4,4,24);

    //SetSize(40,6000,8);

    //SetSize(60,40,8 );
}

CImage::~CImage()
{
    Del();
}

void __stdcall CImage::Del()
{
    delete [] (BYTE *)m_pBitInfo8  ;
    delete [] (BYTE *)m_pBitInfo24 ;
    //ss delete [] (BYTE *)m_pBitInfoD  ;

    if(m_pImage  ){delete [] m_pImage  ;}
    if(m_pImageC ){delete [] m_pImageC ;}
}

bool __stdcall CImage::SetPalette(int _iWidth , int _iHeight)
{
    //ss if(m_pBitInfoD ) {delete [] (BYTE *)m_pBitInfoD  ; m_pBitInfoD  = NULL ; }
    if(m_pBitInfo8 ) {delete [] (BYTE *)m_pBitInfo8  ; m_pBitInfo8  = NULL ; }
    if(m_pBitInfo24) {delete [] (BYTE *)m_pBitInfo24 ; m_pBitInfo24 = NULL ; }

    //ss if(!m_pBitInfoD )m_pBitInfoD  = (BITMAPINFO *) new BYTE[BMP_INFO_SIZE_8 ]; ZeroMemory(m_pBitInfoD ,sizeof(BMP_INFO_SIZE_8 ));
    if(!m_pBitInfo8 )m_pBitInfo8  = (BITMAPINFO *) new BYTE[BMP_INFO_SIZE_8 ]; ZeroMemory(m_pBitInfo8 ,sizeof(BMP_INFO_SIZE_8 ));
    if(!m_pBitInfo24)m_pBitInfo24 = (BITMAPINFO *) new BYTE[BMP_INFO_SIZE_24]; ZeroMemory(m_pBitInfo24,sizeof(BMP_INFO_SIZE_24));

    int iMemWidth8 , iMemWidth24;
    if(_iWidth % 4) iMemWidth8 = (_iWidth / 4)*4+4 ;
    else            iMemWidth8 =  _iWidth          ;

    //24비트 3바이트다.
    if((_iWidth * 3)%4) iMemWidth24 = ((_iWidth * 3)/4)*4+4 ;
    else                iMemWidth24 = _iWidth * 3 ;



    m_pBitInfo8-> bmiHeader.biSize          = 40 ;
    m_pBitInfo8-> bmiHeader.biWidth         = _iWidth  ;
    m_pBitInfo8-> bmiHeader.biHeight        = -_iHeight ;
    m_pBitInfo8-> bmiHeader.biPlanes        = 1  ;
    m_pBitInfo8-> bmiHeader.biBitCount      = 8  ;
    m_pBitInfo8-> bmiHeader.biCompression   = BI_RGB;
    m_pBitInfo8-> bmiHeader.biSizeImage     = iMemWidth8 * _iHeight ;
    m_pBitInfo8-> bmiHeader.biXPelsPerMeter = 0  ;
    m_pBitInfo8-> bmiHeader.biYPelsPerMeter = 0  ;
    m_pBitInfo8-> bmiHeader.biClrUsed       = 0  ;
    m_pBitInfo8-> bmiHeader.biClrImportant  = 0  ;

    //8Bit color pallette
    for (int i=0; i<MAX_COLOR_8; i++) {
        m_pBitInfo8-> bmiColors[i].rgbRed      = i ;
        m_pBitInfo8-> bmiColors[i].rgbGreen    = i ;
        m_pBitInfo8-> bmiColors[i].rgbBlue     = i ;
        m_pBitInfo8-> bmiColors[i].rgbReserved = 0 ;
    }

    //ss memcpy(&m_pBitInfo8  -> bmiHeader , m_pBitInfoD, BMP_INFO_SIZE_8 );
    memcpy(&m_pBitInfo24 -> bmiHeader, m_pBitInfo8, BMP_INFO_SIZE_24);
    m_pBitInfo24 -> bmiHeader.biBitCount = 24 ;
    m_pBitInfo24 -> bmiHeader.biSizeImage = iMemWidth24 * _iHeight ;

    return true ;
}

bool __stdcall CImage::SetSize(int _iWidth , int _iHeight , int _iBitCnt)
{
    if(_iBitCnt != 8 && _iBitCnt != 24) return false;
    _iHeight = abs(_iHeight) ;

    int iPxByteCnt = BitToByte(_iBitCnt) ;




    int iMemWidth8 , iMemWidth24;
    if(_iWidth % 4) iMemWidth8 = (_iWidth / 4)*4+4 ;
    else            iMemWidth8 =  _iWidth          ;

    //24비트 3바이트다.
    if((_iWidth * 3)%4) iMemWidth24 = ((_iWidth * 3)/4)*4+4 ;
    else                iMemWidth24 = _iWidth * 3 ;














    if(m_iWidth != _iWidth || m_iHeight != _iHeight || m_iBitCnt != _iBitCnt) {
        SetPalette(_iWidth , _iHeight);
        if(m_pImage  ){delete [] m_pImage  ;}
        if(m_pImageC ){delete [] m_pImageC ;}

        m_pImage  = new unsigned char [iMemWidth8  * _iHeight ] ;
        m_pImageC = new unsigned char [iMemWidth24 * _iHeight ] ;
    }

    memset (m_pImage  , 0 , iMemWidth8  * _iHeight);
    memset (m_pImageC , 0 , iMemWidth24 * _iHeight);

    m_iWidth  = _iWidth  ;
    m_iHeight = _iHeight ;
    m_iBitCnt = _iBitCnt ;

    m_iMemWidth8  = iMemWidth8  ;
    m_iMemWidth24 = iMemWidth24 ;

    return true ;
}

bool __stdcall CImage::LoadBmp (char * _FilePath)
{
    AnsiString filename = _FilePath;

    if(filename.Pos(".BMP") ||
       filename.Pos(".BMp") ||
       filename.Pos(".BmP") ||
       filename.Pos(".bMP") ||
       filename.Pos(".bMp") ||
       filename.Pos(".bmp") ||
       filename.Pos(".Bmp") ) filename = _FilePath ;
    else                      filename = AnsiString(_FilePath)+".bmp" ;

     FILE * file = fopen(filename.c_str(),"rb") ;    //C:\HRD_210c_last\bin\Util\1.Bmp

    if(file == NULL) {
        ShowMessage("이미지 파일을 열수 없습니다.");
        return false ;
    }

    BITMAPFILEHEADER  bfh ;
    BITMAPINFOHEADER  bih ;

//  원래 32bit CPU  시스템에서 메모리할당을 32bit씩 합니다.
//  위와 같은 구조체를 그냥 sizeof하면 16byte가 나옵니다
//  원래 14바이트 인BITMAPFILEHEADER를 sizeof(BITMAPF9ILEHEADER) 해 보십시요 16 이 나올겁니다.
                                              //124*74_8 //125*74_8 //124*74_24 //280*250*24 //280*251*24
    fread(&bfh.bfType          , 2, 1, file); //19778    19778      19778       19778        19778        // 파일의 종류 아스키 ex) BMP=19778   JPG=55551
    fread(&bfh.bfSize          , 4, 1, file); //10254    10550      27582       210054       211054       // 파일의 전체 크기   ex) 787510     4194358
    fread(&bfh.bfReserved1     , 2, 1, file); //0        0          0           0            0            // 사용안함 0으로 세팅
    fread(&bfh.bfReserved2     , 2, 1, file); //0        0          0           0            0            // 사용안함 0으로 세팅
    fread(&bfh.bfOffBits       , 4, 1, file); //1078     1078       54          54           54           // 비트맵 파일 해더서 부터 실제 데이터까지의 바이트거리. 팔레트 있으면1078 첪으면 54
                                                                                             //24비트는 281*3 하고서 4의 배수확인해야함.
    fread(&bih.biSize          , 4, 1, file); //40       40         40          40           40           //DWORD  biSize;           40     BITMAPINFOHEADER 구조체의 크기 최소 40이상.
    fread(&bih.biWidth         , 4, 1, file); //124      125        124         280          281          //LONG   biWidth;          1024   이미지 넓이
    fread(&bih.biHeight        , 4, 1, file); //74       74         74          250          250          //LONG   biHeight;         768    + 일 경우 아래에서 위로 증가, -일 경우 위에서 아래로 증가.. biWidth는 상관없음 왼쪽에서 오른쪽으로 증가.
    fread(&bih.biPlanes        , 2, 1, file); //1        1          1           1            1            //WORD   biPlanes;         1      컬러 Plane의 수.(???)
    fread(&bih.biBitCount      , 2, 1, file); //8        8          24          24           24           //WORD   biBitCount        8      몇비트 이미지인지(1,4,8,16,24).  (단색,16색,256색,24비트)
    fread(&bih.biCompression   , 4, 1, file); //0        0          0           0            0            //DWORD  biCompression;    0      BI_RGB(압축되지 않은 비트맵), BI_RLE8이면 8비트 압축, BI_RLE4이면 4비트 압축
    fread(&bih.biSizeImage     , 4, 1, file); //9176     9472       27528       210000       211000       //DWORD  biSizeImage;      786432 biWidth * biHeight * biBitCount /8 이미지 크기를 바이트 단위로 나타내며, BI_RGB 일 경우 0
    fread(&bih.biXPelsPerMeter , 4, 1, file); //0        0          0           0            0            //LONG   biXPelsPerMeter;  1      수평해상도.
    fread(&bih.biYPelsPerMeter , 4, 1, file); //0        0          0           0            0            //LONG   biYPelsPerMeter;  1      수직해상도.
    fread(&bih.biClrUsed       , 4, 1, file); //0        0          0           0            0            //DWORD  biClrUsed;        0      비트맵에 사용된 색상수를 의미하며, 이값에 따라 RGBQUAD의 배열을 메모리 할당하여서 읽어오시면 됩니다. 이 값이 0이면 모든 색깔을 다 사용한다는 의미입니다.
    fread(&bih.biClrImportant  , 4, 1, file); //0        0          0           0            0            //DWORD  biClrImportant;   0      비트맵을 출력하는데 필수적인 색상수를 나타내며, 이 값이 0이면 모든 색상을 다 사용한다는 의미입니다.

    int iTemp  = sizeof(BITMAPFILEHEADER) - 2; //빌더 버그 32비트 cpu라 4바이트씩 할당하여 원래 14바이트 크기인데 16으로 나옴.

    if(bfh.bfType != 0x4d42){ShowMessage("비트맵 이미지가 아닙니다."); fclose(file); return false ;}
    if(bih.biBitCount != 8 && bih.biBitCount != 24 ){ShowMessage("8 , 24 비트이미지가 아닙니다."); fclose(file); return false ;} //8비트 혹은 24비트 아니면 로딩 안됨.

    int iTemp1 = sizeof(BITMAPINFOHEADER) ;

    //시발 ... 반전 안할려면 밑에 메모리 쓰는것 뒤집고.
    //화면에 이미지 뿌려줄때 보내는 비트맵인포헤더에서 높이 반전해 줘야 해서 그냥 쓴다.
    bih.biHeight = -bih.biHeight;

    SetSize(bih.biWidth , abs(bih.biHeight) , bih.biBitCount); //m_pBitmapInfo이거 건드리기 전에 이미지 버퍼 싸이즈 맞게 바꿈.


    //비트맵 인포 해더 할당.
    if(bih.biBitCount == 8)memcpy(&m_pBitInfo8  ->bmiHeader , &bih , sizeof(BITMAPINFOHEADER)); //m_pBitInfo8     -> bmiHeader = bih ;
    else                   memcpy(&m_pBitInfo24 ->bmiHeader , &bih , sizeof(BITMAPINFOHEADER)); //m_pBitInfo24    -> bmiHeader = bih ;

    //팔레트 리딩.
    if(bih.biBitCount == 8) { // 1,4,8에만 팔라트 존재.
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
    }

    int iTemp2   = bih.biBitCount <= 8 ? sizeof(RGBQUAD) * MAX_COLOR_8 : 0 ;
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
        if ( bih.biBitCount == 8 ) {
            for(int y=m_iHeight-1;y>=0;y--) {
                fread(m_pImage + y    * m_iMemWidth8 , m_iMemWidth8, 1, file);
                //fread(m_pImage +(y+1) * m_iMemWidth8 , iDummyCnt  , 1, file);
            }
        }
        else if(bih.biBitCount == 24) {
            for(int y=m_iHeight-1;y>=0;y--) {
                fread(m_pImageC + y * m_iMemWidth24 , m_iMemWidth24 , 1, file);
                for(int x = 0 ; x < m_iWidth; x++ ){
                    memcpy(&rgb3 , m_pImageC + y * m_iMemWidth24 + x * iPxByte , sizeof(RGBTRIPLE) );
                    rgb = (rgb3.rgbtRed + rgb3.rgbtGreen + rgb3.rgbtBlue )/3 ;
                    *(m_pImage  + y * m_iMemWidth8 + x) =  rgb ;  //(rgb.Red * 0.222 + rgb.Green * 0.707 + rgb.Blue * 0.071)/3 ;
                }
            }
        }
    }


    else {
        if ( bih.biBitCount == 8 ) {
            BYTE * pLine = new BYTE [m_iMemWidth8] ;
            for(int y=0;y<m_iHeight;y++) {
                fread(pLine , m_iMemWidth8 , 1 , file);
                memcpy(m_pImage + y * m_iMemWidth8 , pLine , m_iMemWidth8);
            }
            delete [] pLine ;
        }
        else if(bih.biBitCount == 24) {
            for(int y=0;y<m_iHeight;y++) {
                fread(m_pImageC + y * m_iMemWidth24 , m_iMemWidth24 , 1, file);
                for(int x = 0 ; x < m_iWidth; x++ ){
                    memcpy(&rgb3 , m_pImageC + y * m_iMemWidth24 + x * iPxByte , sizeof(RGBTRIPLE) );
                    rgb = (rgb3.rgbtRed + rgb3.rgbtGreen + rgb3.rgbtBlue )/3 ;
                    *(m_pImage  + y * m_iMemWidth8 + x) = rgb ;  //(rgb.Red * 0.222 + rgb.Green * 0.707 + rgb.Blue * 0.071)/3 ;
                }
            }
        }
    }

    int ret = !fclose(file);

    return ret ;

}

bool __stdcall CImage::LoadJpg (char * _FilePath)
{
    return false ;
}

bool __stdcall CImage::SaveBmp (char * _FilePath)
{
    AnsiString filename = _FilePath;

    if(filename.Pos(".BMP") ||
       filename.Pos(".BMp") ||
       filename.Pos(".BmP") ||
       filename.Pos(".bMP") ||
       filename.Pos(".bMp") ||
       filename.Pos(".bmp") ||
       filename.Pos(".Bmp") ) filename = filename ;
    else                      filename = filename+".bmp" ;

    if(!m_pImage) return false ;

    FILE * file = fopen(filename.c_str(),"wb") ;
    if(file == NULL) {ShowMessage("이미지를 저장 할 수 없습니다."); return false ;}

    BITMAPFILEHEADER  bfh ;
    BITMAPINFOHEADER  bih ;

    if(m_iBitCnt < 24) bih = m_pBitInfo8     -> bmiHeader ;
    else               bih = m_pBitInfo24    -> bmiHeader ;

    //bih.biHeight =  m_iHeight ;
    bih.biHeight = -bih.biHeight ;
    //bih.biWidth  =  m_iWidth  ;

    const int iMaxPx = pow(2,bih.biBitCount) ;  //한픽셀이 표현 할수 있는 색수 ^
    const int iSizeOfBfh  = 14 ; //sizeof(BITMAPFILEHEADER) ; 빌더버그 있어서 14인데 16으로 나옴.
    const int iSizeOfPlt  = m_iBitCnt < 24 ? sizeof(RGBQUAD) * iMaxPx : 0 ;
    //biSizeImage = ((((bih.biWidth * m_iBitCnt) + 31) & ~31) >> 3) * bih.biHeight 귀찮아서 4바이트 단위 고려안함.
    const int iSizeOfImg  = m_iBitCnt == 8 ? m_iMemWidth8 * abs(bih.biHeight) : m_iMemWidth24 * abs(bih.biHeight) ;
    const int iOftToImg   = iSizeOfBfh + sizeof(BITMAPINFOHEADER) + iSizeOfPlt ;
    const int iSizeOfFile = iSizeOfBfh + sizeof(BITMAPINFOHEADER) + iSizeOfImg + iSizeOfPlt ;

    bfh.bfType          = 0x4d42;        // BM => Bitmap file
    bfh.bfSize          = iSizeOfFile ;  // FileSize.
    bfh.bfReserved1     = 0;             // not use
    bfh.bfReserved2     = 0;             // not use
    bfh.bfOffBits       = iOftToImg;     // 비트맵 파일 해더서 부터 실제 데이터까지의 바이트거리. 팔레트 있으면1078 첪으면 54

    bih.biSizeImage     = iSizeOfImg;
    //bih.biHeight        = -bih.biHeight;//abs(bih.biHeight);

    fwrite(&bfh.bfType      , 2, 1, file);     // 19778
    fwrite(&bfh.bfSize      , 4, 1, file);     // 787510
    fwrite(&bfh.bfReserved1 , 2, 1, file);     // 0
    fwrite(&bfh.bfReserved2 , 2, 1, file);     // 0
    fwrite(&bfh.bfOffBits   , 4, 1, file);     // 1078

    bih.biWidth = GetWidth() ;

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

    if(m_iBitCnt == 8){
        BYTE a ;
        for (int i = 0 ; i < MAX_COLOR_8 ; i++) {
            a = m_pBitInfo8    ->bmiColors[i].rgbBlue     ;
            a = m_pBitInfo8    ->bmiColors[i].rgbGreen    ;
            a = m_pBitInfo8    ->bmiColors[i].rgbRed      ;
            a = m_pBitInfo8    ->bmiColors[i].rgbReserved ;
            fwrite(&m_pBitInfo8    ->bmiColors[i].rgbBlue     , 1, 1, file);
            fwrite(&m_pBitInfo8    ->bmiColors[i].rgbGreen    , 1, 1, file);
            fwrite(&m_pBitInfo8    ->bmiColors[i].rgbRed      , 1, 1, file);
            fwrite(&m_pBitInfo8    ->bmiColors[i].rgbReserved , 1, 1, file);
        }

        if (bih.biHeight < 0) {
            for(int y=0;y<m_iHeight;y++) {
                fwrite(m_pImage + m_iMemWidth8*y , m_iMemWidth8 , 1, file);
            }
        }
        else {
            for(int y=m_iHeight-1;y>=0;y--) {
                fwrite(m_pImage + m_iMemWidth8*y, m_iMemWidth8 , 1, file);
            }
        }
    }

    else {
        //fwrite(&m_pBitInfo8    ->bmiColors[0].rgbBlue     , 1, 1, file);
        //fwrite(&m_pBitInfo8    ->bmiColors[0].rgbGreen    , 1, 1, file);
        //fwrite(&m_pBitInfo8    ->bmiColors[0].rgbRed      , 1, 1, file);
        //fwrite(&m_pBitInfo8    ->bmiColors[0].rgbReserved , 1, 1, file);
        if(bih.biHeight < 0) {
            for(int y=0;y<m_iHeight;y++) {
                fwrite(m_pImageC + m_iMemWidth24*y , m_iMemWidth24 , 1, file);
            }
        }
        else {
            for(int y=m_iHeight-1;y>=0;y--) {
                fwrite(m_pImageC + m_iMemWidth24*y , m_iMemWidth24 , 1, file);
            }
        }
    }



    fclose(file);

    return true ;

}

bool __stdcall CImage::SaveJpg (char * _FilePath)
{
    return false ;
}

//Set Image에 따라서 3색이냐 단색이냐 자동 쎄팅.
bool __stdcall CImage::SetImage(unsigned char * _pImage  , int _iWidth , int _iHeight , int _iBitCnt)
{
    if(_iWidth < 0 || _iHeight < 0 ||( _iBitCnt != 8 && _iBitCnt !=24)) return false ;
//try {
//EnterCriticalSection(&g_pTemp) ;
    SetSize(_iWidth,_iHeight,_iBitCnt);

    //혹시나 카메라의 데이터 넓이가 4의 배수가 아닐경우 예외처리 해야함. 골치 아프네.. 느려질까봐.
    if(_iBitCnt == 8) {
        memcpy(m_pImage  , _pImage  , _iWidth * _iHeight);
    }
    else { //컬러 카메라 m_iWidth * 3이 4의 배수가 안나오면 골때려짐. 화면에 옆으로 쓰러짐.
           //다행이 많이 쓰는 해상도540,1024,1280,2048등등 *3/4해도 떨어짐.
        memcpy(m_pImageC , _pImage  , _iWidth * _iHeight* BitToByte(_iBitCnt));

        RGBTRIPLE rgb3 ;
        unsigned char rgb ;
        for(int y=0;y<_iHeight;y++) {
            for(int x = 0 ; x < _iWidth; x++ ){
                memcpy(&rgb3 , m_pImageC + y * m_iMemWidth24 + x * 3 , sizeof(RGBTRIPLE) );
                rgb = (rgb3.rgbtRed + rgb3.rgbtGreen + rgb3.rgbtBlue )/3 ;
                *(m_pImage  + y * m_iMemWidth8 + x) = rgb ;  //(rgb.Red * 0.222 + rgb.Green * 0.707 + rgb.Blue * 0.071)/3 ;
            }
        }
    }
//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}
    return true ;
}

void __stdcall CImage::ClrImage()
{
    if(m_pImage     ) {delete m_pImage  ; m_pImage  = NULL ; }
    if(m_pImageC    ) {delete m_pImageC ; m_pImageC = NULL ; }
    m_iWidth    = 0 ;
    m_iMemWidth8 = 0 ;
    m_iMemWidth24 = 0 ;

    m_iHeight   = 0 ;
}

int __stdcall CImage::ShowImg(HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight)
{
    int iRet  ;
    int iRet2 = 0 ;

    if(!m_pImage) {iRet = -1 ; return iRet ; }
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

    m_fScaleX = _iWidth  / (float)m_iWidth  ;
    m_fScaleY = _iHeight / (float)m_iHeight ;

    BITMAPINFO * DpInfo ;

    int iIfByteCnt    = sizeof(BITMAPINFOHEADER) ;
    int iIfByteCntPal = iIfByteCnt + MAX_COLOR_8*sizeof(RGBQUAD) ;


    //SetStretchBltMode(_hHdc, COLORONCOLOR);
    SetStretchBltMode(_hHdc, HALFTONE);






    /*
    int mult = 32 / bitcnt;
    int compatibleWidth = w % iMult ? w + (iMult - w % iMult) : w;

    */
//try{
//EnterCriticalSection(&g_pTemp) ;
    if(m_iBitCnt == 8){
        if(m_iWidth == _iWidth && m_iHeight == _iHeight) iRet = SetDIBitsToDevice(_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , 0        , m_iHeight , m_pImage , m_pBitInfo8 , DIB_RGB_COLORS) ;
        else                                             iRet = StretchDIBits    (_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , m_iWidth , m_iHeight , m_pImage , m_pBitInfo8 , DIB_RGB_COLORS , SRCCOPY) ;

    }
    else {
        if(m_iWidth == _iWidth && m_iHeight == _iHeight) iRet = SetDIBitsToDevice(_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , 0        , m_iHeight , m_pImageC , m_pBitInfo24 , DIB_RGB_COLORS) ;
        else                                             iRet = StretchDIBits    (_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , m_iWidth , m_iHeight , m_pImageC , m_pBitInfo24 , DIB_RGB_COLORS , SRCCOPY) ;
    }
    //ReleaseDC(FrmImage -> pnDraw , hdc);



/*
    //이미지 버퍼 있나 테스트용.
    for(int y = 0 ; y < m_iHeight ; y++) {
        for(int x = 0 ; x < m_iWidth ; x++) {
            if(m_iBitCnt == 24) {
                for(int z = 0 ; z < m_iBitCnt/BYTE_BIT_COUNT ; z++) {
                    iRet2 +=(int) (*(m_pImageC + y*m_iWidth* m_iBitCnt/BYTE_BIT_COUNT + z)) ;
                }
            }
            else {
                iRet2 += (int)(*(m_pImage + (y*m_iWidth + x))) ;
            }
        }
    }
*/

//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}

    return iRet ;
}

int __stdcall CImage::ShowImgGrey(HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight)
{
    int iRet  ;
    int iRet2 = 0 ;

    if(!m_pImage) {iRet = -1 ; return iRet ; }

    m_fScaleX = _iWidth  / (float)m_iWidth  ;
    m_fScaleY = _iHeight / (float)m_iHeight ;

    SetStretchBltMode(_hHdc, COLORONCOLOR);
    //SetStretchBltMode(_hHdc, HALFTONE);

    iRet = -1111 ;
    iRet = GDI_ERROR ;

//try{
//EnterCriticalSection(&g_pTemp) ;
    if(m_iWidth == _iWidth && m_iHeight == _iHeight)
        iRet = SetDIBitsToDevice(_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , 0        , m_iHeight , m_pImage , m_pBitInfo8 , DIB_RGB_COLORS) ;
    else
        iRet = StretchDIBits    (_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , m_iWidth , m_iHeight , m_pImage , m_pBitInfo8 , DIB_RGB_COLORS , SRCCOPY) ;
//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}

    if(iRet == 0) {
        DWORD e = GetLastError();         // 실패의 이유를 조사
        TCHAR* s = 0;
        AnsiString sTep ;
        FormatMessage(
           FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
           0,               // 메세지 DLL의 핸들
           e,               // 에러 번호
           0,               //
           (TCHAR*)&s, 0,   // 메세지를 담아올 버퍼 // 실제로는 이중포인터를 보내지만 함수 모양이 포인터로// 만들어져 있으므로 형식은 1차 포인터로 보내야 한다.
           0);              //
           sTep = sTep.sprintf( _T("%s\n"), s );
           LocalFree( s );                         // FormatMessage()는 LocalAlloc()으로메모리를할당한다.

    }
    DWORD dTemp ;
    AnsiString sTemp ;
    if(iRet == GDI_ERROR) {
        dTemp = GetLastError();
        sTemp.sprintf("DeviceIo Configure ERROR : %d" , GetLastError()) ;
    }

        //iRet = StretchDIBits      (_hHdc, _x , _y , 1000 , 1000 , 400 , 400 , 1000 , 1000   , m_pImage , m_pBitInfo8 , DIB_RGB_COLORS , SRCCOPY) ;

    //이미지 버퍼 있나 테스트용.      FormatMessage
    //for(int y = 0 ; y < m_iHeight ; y++) {
    //    for(int x = 0 ; x < m_iWidth ; x++) {
    //        if(m_iBitCnt == 24) {
    //            for(int z = 0 ; z < m_iBitCnt/BYTE_BIT_COUNT ; z++) {
    //                iRet2 +=(int) (*(m_pImageC + y*m_iWidth* m_iBitCnt/BYTE_BIT_COUNT + z)) ;
    //            }
    //        }
    //        else {
    //            iRet2 += (int)(*(m_pImage + (y*m_iWidth + x))) ;
    //        }
    //    }
    //}

    return iRet ;
}

int __stdcall CImage::ShowImgFalse(HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight)
{
    int iRet  ;
    int iRet2 = 0 ;
    BITMAPINFO * pBitInfo = (BITMAPINFO *) new BYTE[BMP_INFO_SIZE_8 ];
    //BITMAPINFO * pBitInfo = new BITMAPINFO ;

    memcpy (pBitInfo , m_pBitInfo8 , BMP_INFO_SIZE_8) ;


    //memcpy(&m_pBitInfo8  -> bmiHeader , m_pBitInfoD, BMP_INFO_SIZE_8 );

    if(!m_pImage) {iRet = -1 ; return iRet ; }

    m_fScaleX = _iWidth  / (float)m_iWidth  ;
    m_fScaleY = _iHeight / (float)m_iHeight ;

    //false color pallette Set
    for (int i=0; i<MAX_COLOR_8; i++) {
             if (i>=  0 && i<= 51){pBitInfo-> bmiColors[i].rgbRed = 255         ;pBitInfo-> bmiColors[i].rgbGreen = i*5          ;pBitInfo-> bmiColors[i].rgbBlue = 0        ;}
        else if (i>= 52 && i<=102){pBitInfo-> bmiColors[i].rgbRed = 255-(i-52)*5;pBitInfo-> bmiColors[i].rgbGreen = 255          ;pBitInfo-> bmiColors[i].rgbBlue = 0        ;}
        else if (i>=103 && i<=153){pBitInfo-> bmiColors[i].rgbRed = 0           ;pBitInfo-> bmiColors[i].rgbGreen = 255          ;pBitInfo-> bmiColors[i].rgbBlue = (i-103)*5;}
        else if (i>=154 && i<=203){pBitInfo-> bmiColors[i].rgbRed = 0           ;pBitInfo-> bmiColors[i].rgbGreen = 255-(i-154)*5;pBitInfo-> bmiColors[i].rgbBlue = 255      ;}
        else if (i>=204 && i<=254){pBitInfo-> bmiColors[i].rgbRed = (i-204)*5   ;pBitInfo-> bmiColors[i].rgbGreen = 0            ;pBitInfo-> bmiColors[i].rgbBlue = 255      ;}
        else                      {pBitInfo-> bmiColors[i].rgbRed = 0           ;pBitInfo-> bmiColors[i].rgbGreen = 0            ;pBitInfo-> bmiColors[i].rgbBlue = 0        ;}
        pBitInfo-> bmiColors[i].rgbReserved = 0;
    }

//try{
//EnterCriticalSection(&g_pTemp) ;

    //SetStretchBltMode(_hHdc, COLORONCOLOR);
    SetStretchBltMode(_hHdc, HALFTONE);

    if(m_iWidth == _iWidth && m_iHeight == _iHeight)
        iRet = SetDIBitsToDevice(_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , 0        , m_iHeight , m_pImage , pBitInfo, DIB_RGB_COLORS) ;
    else
        iRet = StretchDIBits    (_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , m_iWidth , m_iHeight , m_pImage , pBitInfo, DIB_RGB_COLORS , SRCCOPY) ;


    /*

     // 화면을 캡쳐한 후 Zoom 시킴
     iSourceX = m_iPreX - (imMagnifying->Width  / 2 / m_iScale);
     iSourceY = m_iPreY - (imMagnifying->Height / 2 / m_iScale);

     if(iSourceX < 0) iSourceX = 0;
     if(iSourceY < 0) iSourceY = 0;
     if(iSourceX + m_pBmp->Width  > Screen->Width)  iSourceX = Screen->Width  - m_pBmp->Width;
     if(iSourceY + m_pBmp->Height > Screen->Height) iSourceY = Screen->Height - m_pBmp->Height;


     iMagnifyX = ((m_iPreX - iSourceX) * m_iScale) + imMagnifying->Left;
     iMagnifyY = ((m_iPreY - iSourceY) * m_iScale) + imMagnifying->Top;

     itDC = GetDC(0);
     BitBlt(m_pBmp->Canvas->Handle, 0, 0,
            m_pBmp->Width, m_pBmp->Height, itDC,
            iSourceX, iSourceY,
            SRCCOPY);
     ReleaseDC(0, itDC);

    */

    /*
    //이미지 버퍼 있나 테스트용.
    for(int y = 0 ; y < m_iHeight ; y++) {
        for(int x = 0 ; x < m_iWidth ; x++) {
            if(m_iBitCnt == 24) {
                for(int z = 0 ; z < m_iBitCnt/BYTE_BIT_COUNT ; z++) {
                    iRet2 +=(int) (*(m_pImageC + y*m_iWidth* m_iBitCnt/BYTE_BIT_COUNT + z)) ;
                }
            }
            else {
                iRet2 += (int)(*(m_pImage + (y*m_iWidth + x))) ;
            }
        }
    }
    */

//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}
    delete [] ((BYTE * )pBitInfo) ;

    return iRet ;
}

int __stdcall CImage::ShowImgColor(HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight)
{
    int iRet  ;
    int iRet2 = 0 ;

    if(!m_pImageC) {iRet = -1 ; return iRet ; }

    m_fScaleX = _iWidth  / (float)m_iWidth  ;
    m_fScaleY = _iHeight / (float)m_iHeight ;

    //SetStretchBltMode(_hHdc, COLORONCOLOR);
    SetStretchBltMode(_hHdc, HALFTONE);

    m_pBitInfo24-> bmiColors[0].rgbBlue     = 0 ;
    m_pBitInfo24-> bmiColors[0].rgbRed      = 0 ;
    m_pBitInfo24-> bmiColors[0].rgbGreen    = 0 ;
    m_pBitInfo24-> bmiColors[0].rgbReserved = 0 ;

    //이미지 찌그러지는 것 방지.
    //#define HBYTES(w, b)  ((((w) * (b) + 31) & ~31) / 8)
    //int iOriWidth ;
    //iOriWidth = m_pBitInfo24->bmiHeader.biWidth ;
    //int iVal  = iOriWidth / 4 ;
    //int iElse = iOriWidth % 4 ;
    //
    //if(iElse) m_pBitInfo24->bmiHeader.biWidth = iVal * 4 + 4 ;
//try{
//EnterCriticalSection(&g_pTemp) ;
    if(m_iWidth == _iWidth && m_iHeight == _iHeight)
        iRet = SetDIBitsToDevice(_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , 0        , m_iHeight , m_pImageC , m_pBitInfo24 , DIB_RGB_COLORS) ;
    else
        iRet = StretchDIBits    (_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , m_iWidth , m_iHeight , m_pImageC , m_pBitInfo24 , DIB_RGB_COLORS , SRCCOPY) ;

    //이미지 버퍼 있나 테스트용.
   //for(int y = 0 ; y < m_iHeight ; y++) {
   //     for(int x = 0 ; x < m_iWidth ; x++) {
   //         if(m_iBitCnt == 24) {
   //             for(int z = 0 ; z < m_iBitCnt/BYTE_BIT_COUNT ; z++) {
   //                 iRet2 +=(int) (*(m_pImageC + y*m_iWidth* m_iBitCnt/BYTE_BIT_COUNT + z)) ;
   //             }
   //         }
   //         else {
   //             iRet2 += (int)(*(m_pImage + (y*m_iWidth + x))) ;
   //         }
   //     }
   // }
//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}

    return iRet ;
}

int __stdcall CImage::ShowImgBinary(HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight , int _iThreshold)
{
    int iRet  ;
    int iRet2 = 0 ;
    BITMAPINFO * pBitInfo = (BITMAPINFO *) new BYTE[BMP_INFO_SIZE_8 ];
    //BITMAPINFO * pBitInfo = new BITMAPINFO ;

    if(!m_pImage) {iRet = -1 ; return iRet ; }

    memcpy (pBitInfo , m_pBitInfo8 , BMP_INFO_SIZE_8) ;

    m_fScaleX = _iWidth  / (float)m_iWidth  ;
    m_fScaleY = _iHeight / (float)m_iHeight ;

    int level=0;

    //pallette Set
    for (int i=0; i<MAX_COLOR_8; i++) {
        pBitInfo-> bmiColors[i].rgbRed      = i >= _iThreshold ? 255 : 0 ;
        pBitInfo-> bmiColors[i].rgbGreen    = i >= _iThreshold ? 255 : 0 ;
        pBitInfo-> bmiColors[i].rgbBlue     = i >= _iThreshold ? 255 : 0 ;
        pBitInfo-> bmiColors[i].rgbReserved = 0 ;
    }

    pBitInfo-> bmiColors[_iThreshold].rgbRed      = 255 ;
    pBitInfo-> bmiColors[_iThreshold].rgbGreen    = 0   ;
    pBitInfo-> bmiColors[_iThreshold].rgbBlue     = 0   ;

//try{
//EnterCriticalSection(&g_pTemp) ;
    //SetStretchBltMode(_hHdc, COLORONCOLOR);
    SetStretchBltMode(_hHdc, HALFTONE);
    if(m_iWidth == _iWidth && m_iHeight == _iHeight)
        iRet = SetDIBitsToDevice(_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , 0        , m_iHeight , m_pImage , pBitInfo, DIB_RGB_COLORS) ;
    else
        iRet = StretchDIBits    (_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , m_iWidth , m_iHeight , m_pImage , pBitInfo, DIB_RGB_COLORS , SRCCOPY) ;


        /*
    //이미지 버퍼 있나 테스트용.
    for(int y = 0 ; y < m_iHeight ; y++) {
        for(int x = 0 ; x < m_iWidth ; x++) {
            if(m_iBitCnt == 24) {
                for(int z = 0 ; z < m_iBitCnt/BYTE_BIT_COUNT ; z++) {
                    iRet2 +=(int) (*(m_pImageC + y*m_iWidth* m_iBitCnt/BYTE_BIT_COUNT + z)) ;
                }
            }
            else {
                iRet2 += (int)(*(m_pImage + (y*m_iWidth + x))) ;
            }
        }
    }
    */
//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}

    //delete pBitInfo ;
    delete [] ((BYTE * )pBitInfo) ;

    return iRet ;
}

int __stdcall CImage::ShowImgR(HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight)
{
    int iRet  ;
    int iRet2 = 0 ;
    BITMAPINFO * pBitInfo = (BITMAPINFO *) new BYTE[BMP_INFO_SIZE_8 ];
    unsigned char * pImage ;
    pImage = new unsigned char [m_iMemWidth8 * m_iHeight] ;

    if(!m_pImage) {iRet = -1 ; return iRet ; }

    m_fScaleX = _iWidth  / (float)m_iWidth  ;
    m_fScaleY = _iHeight / (float)m_iHeight ;

    int level=0;

    //pallette Set
    for (int i=0; i<MAX_COLOR_8; i++) {
        pBitInfo-> bmiColors[i].rgbRed      = i ;
        pBitInfo-> bmiColors[i].rgbGreen    = 0 ;
        pBitInfo-> bmiColors[i].rgbBlue     = 0 ;
        pBitInfo-> bmiColors[i].rgbReserved = 0 ;
    }

    for (int y = 0 ; y < m_iHeight ; y++) {
        for (int x = 0 ; x < m_iMemWidth8 ; x++) {
            pImage[y*m_iMemWidth8 + x] = GetPixelR(x,y) ;
        }
    }
//try{
//EnterCriticalSection(&g_pTemp) ;

    //SetStretchBltMode(_hHdc, COLORONCOLOR);
    SetStretchBltMode(_hHdc, HALFTONE);
    if(m_iWidth == _iWidth && m_iHeight == _iHeight)
        iRet = SetDIBitsToDevice(_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , 0        , m_iHeight , pImage , pBitInfo, DIB_RGB_COLORS) ;
    else
        iRet = StretchDIBits    (_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , m_iWidth , m_iHeight , pImage , pBitInfo, DIB_RGB_COLORS , SRCCOPY) ;
//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}
    delete [] pImage ;
    delete pBitInfo ;
    return iRet ;
}

int __stdcall CImage::ShowImgG(HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight)
{
    int iRet  ;
    int iRet2 = 0 ;
    BITMAPINFO * pBitInfo = (BITMAPINFO *) new BYTE[BMP_INFO_SIZE_8 ];
    unsigned char * pImage ;
    pImage = new unsigned char [m_iMemWidth8 * m_iHeight] ;

    if(!m_pImage) {iRet = -1 ; return iRet ; }

    m_fScaleX = _iWidth  / (float)m_iWidth  ;
    m_fScaleY = _iHeight / (float)m_iHeight ;

    int level=0;

    //pallette Set
    for (int i=0; i<MAX_COLOR_8; i++) {
        pBitInfo-> bmiColors[i].rgbRed      = 0 ;
        pBitInfo-> bmiColors[i].rgbGreen    = i ;
        pBitInfo-> bmiColors[i].rgbBlue     = 0 ;                   
        pBitInfo-> bmiColors[i].rgbReserved = 0 ;
    }

    for (int y = 0 ; y < m_iHeight ; y++) {
        for (int x = 0 ; x < m_iMemWidth8 ; x++) {
            pImage[y*m_iMemWidth8 + x] = GetPixelG(x,y) ;
        }
    }

//try{
//EnterCriticalSection(&g_pTemp) ;

    //SetStretchBltMode(_hHdc, COLORONCOLOR);
    SetStretchBltMode(_hHdc, HALFTONE);
    if(m_iWidth == _iWidth && m_iHeight == _iHeight)
        iRet = SetDIBitsToDevice(_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , 0        , m_iHeight , m_pImage , pBitInfo, DIB_RGB_COLORS) ;
    else
        iRet = StretchDIBits    (_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , m_iWidth , m_iHeight , m_pImage , pBitInfo, DIB_RGB_COLORS , SRCCOPY) ;
//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}

    delete [] pImage ;
    delete pBitInfo ;
    return iRet ;
}


int __stdcall CImage::ShowImgB(HDC _hHdc , int _x, int _y, int _iWidth, int _iHeight)
{
    int iRet  ;
    int iRet2 = 0 ;
    BITMAPINFO * pBitInfo = (BITMAPINFO *) new BYTE[BMP_INFO_SIZE_8 ];
    unsigned char * pImage ;
    pImage = new unsigned char [m_iMemWidth8 * m_iHeight] ;

    if(!m_pImage) {iRet = -1 ; return iRet ; }

    m_fScaleX = _iWidth  / (float)m_iWidth  ;
    m_fScaleY = _iHeight / (float)m_iHeight ;

    int level=0;

    //pallette Set
    for (int i=0; i<MAX_COLOR_8; i++) {
        pBitInfo-> bmiColors[i].rgbRed      = 0 ;
        pBitInfo-> bmiColors[i].rgbGreen    = 0 ;
        pBitInfo-> bmiColors[i].rgbBlue     = i ;
        pBitInfo-> bmiColors[i].rgbReserved = 0 ;
    }

    for (int y = 0 ; y < m_iHeight ; y++) {
        for (int x = 0 ; x < m_iMemWidth8 ; x++) {
            pImage[y*m_iMemWidth8 + x] = GetPixelB(x,y) ;
        }
    }
//try{
//EnterCriticalSection(&g_pTemp) ;

    //SetStretchBltMode(_hHdc, COLORONCOLOR);
    SetStretchBltMode(_hHdc, HALFTONE);
    if(m_iWidth == _iWidth && m_iHeight == _iHeight)
        iRet = SetDIBitsToDevice(_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , 0        , m_iHeight , m_pImage , pBitInfo, DIB_RGB_COLORS) ;
        //iRet = SetDIBitsToDevice(_hHdc, _x , _y , FrmImage -> sbDraw -> Width , FrmImage -> sbDraw -> Height , 0 , 0 , 0        , m_iHeight , m_pImageD , pBitInfo, DIB_RGB_COLORS) ;
    else
        iRet = StretchDIBits    (_hHdc, _x , _y , _iWidth , _iHeight , 0 , 0 , m_iWidth , m_iHeight , m_pImage , pBitInfo, DIB_RGB_COLORS , SRCCOPY) ;
        //iRet = StretchDIBits      (_hHdc, 400 , 400 , 1000 , 1000      , 400 , 400 , 1000 , 1000 , m_pImageD , pBitInfo,      DIB_RGB_COLORS , SRCCOPY) ;
//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}
    delete [] pImage ;
    delete pBitInfo ;
    return iRet ;
}

HBITMAP __stdcall CImage::GetHBitmap(HDC _hHdc)
{
    if (!m_pImage) return NULL;
    HBITMAP hBitmap ;
//try{
//EnterCriticalSection(&g_pTemp) ;

    if(m_iBitCnt == 8) {
        hBitmap = CreateDIBitmap(_hHdc, &m_pBitInfo8->bmiHeader , 0, NULL, NULL, 0);
        SetDIBits(_hHdc, hBitmap, 0, m_iHeight , (BYTE*)m_pImage , m_pBitInfo8 , DIB_RGB_COLORS);
    }
    else {
        hBitmap = CreateDIBitmap(_hHdc, &m_pBitInfo24->bmiHeader, 0, NULL, NULL, 0);
        SetDIBits(_hHdc, hBitmap, 0, m_iHeight , (BYTE*)m_pImageC, m_pBitInfo24, DIB_RGB_COLORS);
    }

    //DeleteObject(hbitmap); CreateDIBitmap로 만든것은 해제 안해도 되는듯.
//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}

    return hBitmap;
}

void __stdcall CImage::ShowBitmap(HDC _hHdc, int _x, int _y, int _iWidth, int _iHeight)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    BITMAP bit;

    HBITMAP hBitmap ;
    hBitmap = GetHBitmap(_hHdc) ;

    hMemDC = CreateCompatibleDC(_hHdc);
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    GetObject(hBitmap, sizeof(BITMAP), &bit);

//try{
//EnterCriticalSection(&g_pTemp) ;

    //SetStretchBltMode(_hHdc, COLORONCOLOR);
    SetStretchBltMode(_hHdc, HALFTONE);
    if (_iWidth==bit.bmWidth && _iHeight==bit.bmHeight) BitBlt    (_hHdc,  0,  0, _iWidth, _iHeight, hMemDC, 0, 0, SRCCOPY);
    else                                                StretchBlt(_hHdc, _x, _y, _iWidth, _iHeight, hMemDC, 0, 0, _iWidth, _iHeight, SRCCOPY);
//}
//__finally{
//LeaveCriticalSection(&g_pTemp);
//}
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}


//8비트만 됌...    어.. .문제 있네.... 1도씩 돌려보샘.
void __stdcall CImage::Rotate(TRect _tRect , float _fAngle)
{

    //float TempTime = GetTickTimeDouble() ;
    SetRect(&_tRect);

    BYTE  *pRotationImg;
    BYTE  newValue;
    int   iHeight = _tRect.Height() ;
    int   iWidth  = _tRect.Width()  ;
    int   iRotCntY = _tRect.top  + _tRect.Height() / 2 ; //회전 중심 점... 렉트의 센터로 한다.
    int   iRotCntX = _tRect.left + _tRect.Width () / 2 ;
    int   iPnt;
    int   y,x;//타겟 이미지 좌표
    float fOriY,fOriX;//원 이미지 상의 해당 좌표 (실수값)
    int   iOriY,iOriX;//원 이미지 상의 해당 좌표 (정수값)
    float fRestY,fRestX;// 예 1.24-1=0.24
    int   iLTPx,iRTPx,iRBPx,iLBPx;
    float fCosRad,fSinRad;
    float fRotRad=(float)(-_fAngle*MATH_PI/180.0);//angle->radian으로 변환
    fCosRad=(float)cos(fRotRad);
    fSinRad=(float)sin(fRotRad);

    //Rotation Image를 위한 동적 메모리 할당
    pRotationImg=new BYTE[iHeight*iWidth];

    for(y = _tRect.top ; y < _tRect.top + iHeight ; y++) {
        for(x = _tRect.left ; x < _tRect.left + iWidth ; x++) {


            //회전전의 원 이미지상의 좌표 구함
            fOriY =-fSinRad * (x-iRotCntX) + fCosRad * (y-iRotCntY) + iRotCntY;
            fOriX = fCosRad * (x-iRotCntX) + fSinRad * (y-iRotCntY) + iRotCntX;
            iOriY = floor(fOriY);//예: floor(2.8)=2.0
            iOriX = floor(fOriX);
            fRestY= fOriY-iOriY;
            fRestX= fOriX-iOriX;
            //범위 조사
            //원이미지의 범위를 벗어나는 경우 0값 할당

            if(iOriY<0 || iOriY >= GetHeight() || iOriX<0 || iOriX >= GetWidth()) {
                pRotationImg[(y - _tRect.top)*iWidth+(x - _tRect.left)]=0;
                continue ;
            }

            //속도 느려서 GetPixel안씀. 원 이미지의 범위 내에 존재 이중 선형 보간 수행
            iLTPx=*(m_pImage  + (iOriX     + m_iMemWidth8 *  iOriY     ));  // GetPixel((iOriX  ),(iOriY  )) ;//(org_r,org_c)
            iRTPx=*(m_pImage  + (iOriX + 1 + m_iMemWidth8 *  iOriY     ));  // GetPixel((iOriX+1),(iOriY  )) ;//(org_r,org_c+1)
            iRBPx=*(m_pImage  + (iOriX + 1 + m_iMemWidth8 * (iOriY + 1)));  // GetPixel((iOriX+1),(iOriY+1)) ;//(org_r+1,org_c+1)
            iLBPx=*(m_pImage  + (iOriX     + m_iMemWidth8 * (iOriY + 1)));  // GetPixel((iOriX  ),(iOriY+1)) ;//(org_r+1,org_c)

            //이중 선형 보간을 통한 새로운 밝기값 계산
            newValue=(BYTE)(iLTPx*(1-fRestX)*(1-fRestY)+
                            iRTPx*   fRestX *(1-fRestY)+
                            iRBPx*   fRestX *   fRestY +
                            iLBPx*(1-fRestX)*   fRestY);

            //iPnt=;
            pRotationImg[(y - _tRect.top)*iWidth+(x - _tRect.left)]=newValue;
        }
    }

    //결과 이미지로 영상 복사
    for(y = _tRect.top ; y < _tRect.top + iHeight ; y++) {
        memcpy(m_pImage + y * GetWidth() +  _tRect.left , pRotationImg + (y - _tRect.top)*iWidth , iWidth) ;
    }

    //동적 할당 메모리 해제
    delete [] pRotationImg;
    //Rslt.fInspectionTime = GetTickTimeDouble() - TempTime ;

}

void __stdcall CImage::Trim  (TRect _tRect , IImage * _tDstImg)
{
    SetRect(&_tRect);

    ((CImage *)_tDstImg) -> SetSize(_tRect.Width() , _tRect.Height() , m_iBitCnt);


    //결과 이미지로 영상 복사
    for(int y = _tRect.top ; y < _tRect.top + _tRect.Height() ; y++) {
        memcpy( _tDstImg->GetImg() + (y - _tRect.top) *_tDstImg->GetMem8Width() ,
                m_pImage + (y * m_iMemWidth8 +  _tRect.left) ,
                _tDstImg->GetMem8Width());
    }

    if(m_iBitCnt == 24) {
        for(int y = _tRect.top ; y < _tRect.top + _tRect.Height() ; y++) {
            memcpy( _tDstImg->GetImgC() + (y - _tRect.top) * _tDstImg->GetMem8Width() * BitToByte(m_iBitCnt) ,
                    m_pImageC + y * m_iMemWidth24 +  _tRect.left * BitToByte(m_iBitCnt) ,
                    _tDstImg->GetMem8Width()* BitToByte(m_iBitCnt)) ;
        }
    }
}


/*
struct TFuncPara {
    TRect           tRect        ;
    float           fAngle       ;
    BYTE          * pRotationImg ;
    CImage        * pImg         ;
    unsigned char * pImage       ;
};

class CRotateTask
{
    protected :
        TFuncPara   m_tPara  ;

    public :
        SetPara(TFuncPara _tPara)
        {
            m_tPara  = _tPara  ;
        }

        CRotateTask()
        {
        }

        CRotateTask(TFuncPara _tPara)
        {
            SetPara(_tPara);
        }

        void operator()(void)
        {
            int   iHeight = m_tPara.tRect.Height() ;
            int   iWidth  = m_tPara.tRect.Width () ;
            int   iRotCntY = m_tPara.tRect.top  + m_tPara.tRect.Height() / 2 ; //회전 중심 점... 렉트의 센터로 한다.
            int   iRotCntX = m_tPara.tRect.left + m_tPara.tRect.Width () / 2 ;
            float fRotRad=(float)(-m_tPara.fAngle*MATH_PI/180.0);//angle->radian으로 변환
            float fCosRad=(float)cos(fRotRad);
            float fSinRad=(float)sin(fRotRad);


            float fOriY,fOriX;//원 이미지 상의 해당 좌표 (실수값)
            int   iOriY,iOriX;//원 이미지 상의 해당 좌표 (정수값)
            float fRestY,fRestX;// 예 1.24-1=0.24
            int   iLTPx,iRTPx,iRBPx,iLBPx;
            BYTE  newValue;
            int   iPnt;

            for(int y = m_tPara.tRect.top ; y < m_tPara.tRect.bottom ; y++) {
                for(int x = m_tPara.tRect.left ; x < m_tPara.tRect.left + iWidth ; x++) {


                    //회전전의 원 이미지상의 좌표 구함
                    fOriY =-fSinRad * (x-iRotCntX) + fCosRad * (y-iRotCntY) + iRotCntY;
                    fOriX = fCosRad * (x-iRotCntX) + fSinRad * (y-iRotCntY) + iRotCntX;
                    iOriY = floor(fOriY);//예: floor(2.8)=2.0
                    iOriX = floor(fOriX);
                    fRestY= fOriY-iOriY;
                    fRestX= fOriX-iOriX;
                    //범위 조사
                    //원이미지의 범위를 벗어나는 경우 0값 할당

                    if(iOriY<0 || iOriY >= m_tPara.pImg->GetHeight() || iOriX<0 || iOriX >= m_tPara.pImg->GetWidth()) {
                        m_tPara.pRotationImg[(y - m_tPara.tRect.top)*iWidth+(x - m_tPara.tRect.left)]=0;
                        continue ;
                    }

                    //속도 느려서 GetPixel안씀. 원 이미지의 범위 내에 존재 이중 선형 보간 수행
                    iLTPx=*(m_tPara.pImage  + (iOriX     + m_tPara.pImg->GetWidth() * iOriY    ));  // GetPixel((iOriX  ),(iOriY  )) ;//(org_r,org_c)
                    iRTPx=*(m_tPara.pImage  + (iOriX + 1 + m_tPara.pImg->GetWidth() * iOriY    ));  // GetPixel((iOriX+1),(iOriY  )) ;//(org_r,org_c+1)
                    iRBPx=*(m_tPara.pImage  + (iOriX + 1 + m_tPara.pImg->GetWidth() * iOriY + 1));  // GetPixel((iOriX+1),(iOriY+1)) ;//(org_r+1,org_c+1)
                    iLBPx=*(m_tPara.pImage  + (iOriX     + m_tPara.pImg->GetWidth() * iOriY + 1));  // GetPixel((iOriX  ),(iOriY+1)) ;//(org_r+1,org_c)

                    //이중 선형 보간을 통한 새로운 밝기값 계산
                    newValue=(BYTE)(iLTPx*(1-fRestX)*(1-fRestY)+
                                    iRTPx*   fRestX *(1-fRestY)+
                                    iRBPx*   fRestX *   fRestY +
                                    iLBPx*(1-fRestX)*   fRestY);

                    //iPnt=;
                    m_tPara.pRotationImg[(y - m_tPara.tRect.top)*iWidth+(x - m_tPara.tRect.left)]=newValue;
                }
            }
        }


};

void __stdcall CImage::Rotate(TRect _tRect , float _fAngle)
{

    SetRect(&_tRect);
    int   iHeight = _tRect.Height() ;
    int   iWidth  = _tRect.Width () ;
    BYTE * pRotationImg=new BYTE[iHeight*iWidth];



    TFuncPara   Para ;
    Para.tRect        = _tRect ;
    Para.fAngle       = _fAngle ;
    Para.pRotationImg = pRotationImg ;
    Para.pImg         = this ;
    Para.pImage       = m_pImage ;

    CRotateTask Task(Para) ;

    CThread<CRotateTask> * Thread = new CThread<CRotateTask>(Task);
    //CRunThread<CRotateTask> * Thread = new CRunThread<CRotateTask>(Task);

    //CMultiThread<CRotateTask> * Thread = new CMultiThread<CRotateTask> (Task);

    //결과 이미지로 영상 복사
    for(int y = _tRect.top ; y < _tRect.top + iHeight ; y++) {
        memcpy(m_pImage + y * GetWidth() +  _tRect.left , pRotationImg + (y - _tRect.top)*iWidth , iWidth) ;
    }

    //동적 할당 메모리 해제
    delete [] pRotationImg;
}

void __stdcall CImage::RotateMulti(int _iYStart ,
                                   int _iYEnd   ,
                                   int _iYStep  ,

                                   TRect _tRect  ,
                                   float _fAngle ,
                                   BYTE  *_pRotationImg )
{


    int   iHeight = _tRect.Height() ;
    int   iWidth  = _tRect.Width () ;
    int   iRotCntY = _tRect.top  + _tRect.Height() / 2 ; //회전 중심 점... 렉트의 센터로 한다.
    int   iRotCntX = _tRect.left + _tRect.Width () / 2 ;
    float fRotRad=(float)(-_fAngle*MATH_PI/180.0);//angle->radian으로 변환
    float fCosRad=(float)cos(fRotRad);
    float fSinRad=(float)sin(fRotRad);


    float fOriY,fOriX;//원 이미지 상의 해당 좌표 (실수값)
    int   iOriY,iOriX;//원 이미지 상의 해당 좌표 (정수값)
    float fRestY,fRestX;// 예 1.24-1=0.24
    int   iLTPx,iRTPx,iRBPx,iLBPx;
    BYTE  newValue;
    int   iPnt;

    for(int y = _iYStart ; y < _iYEnd ; y++) {
        for(int x = _tRect.left ; x < _tRect.left + iWidth ; x++) {


            //회전전의 원 이미지상의 좌표 구함
            fOriY =-fSinRad * (x-iRotCntX) + fCosRad * (y-iRotCntY) + iRotCntY;
            fOriX = fCosRad * (x-iRotCntX) + fSinRad * (y-iRotCntY) + iRotCntX;
            iOriY = floor(fOriY);//예: floor(2.8)=2.0
            iOriX = floor(fOriX);
            fRestY= fOriY-iOriY;
            fRestX= fOriX-iOriX;
            //범위 조사
            //원이미지의 범위를 벗어나는 경우 0값 할당

            if(iOriY<0 || iOriY >= GetHeight() || iOriX<0 || iOriX >= GetWidth()) {
                _pRotationImg[(y - _tRect.top)*iWidth+(x - _tRect.left)]=0;
                continue ;
            }

            //속도 느려서 GetPixel안씀. 원 이미지의 범위 내에 존재 이중 선형 보간 수행
            iLTPx=*(m_pImage  + (iOriX     + m_iMemWidth8 * iOriY    ));  // GetPixel((iOriX  ),(iOriY  )) ;//(org_r,org_c)
            iRTPx=*(m_pImage  + (iOriX + 1 + m_iMemWidth8 * iOriY    ));  // GetPixel((iOriX+1),(iOriY  )) ;//(org_r,org_c+1)
            iRBPx=*(m_pImage  + (iOriX + 1 + m_iMemWidth8 * iOriY + 1));  // GetPixel((iOriX+1),(iOriY+1)) ;//(org_r+1,org_c+1)
            iLBPx=*(m_pImage  + (iOriX     + m_iMemWidth8 * iOriY + 1));  // GetPixel((iOriX  ),(iOriY+1)) ;//(org_r+1,org_c)

            //이중 선형 보간을 통한 새로운 밝기값 계산
            newValue=(BYTE)(iLTPx*(1-fRestX)*(1-fRestY)+
                            iRTPx*   fRestX *(1-fRestY)+
                            iRBPx*   fRestX *   fRestY +
                            iLBPx*(1-fRestX)*   fRestY);

            //iPnt=;
            _pRotationImg[(y - _tRect.top)*iWidth+(x - _tRect.left)]=newValue;
        }
    }
}
*/




//검사 영역 들이 이미지의 크기에서 벋어나 있을때에 이미지영역으로 세팅 하는 함수군
bool __stdcall CImage::SetRect (TRect *_pRect , int _iMargin )
{
    bool bRet = false ;
    if(_pRect -> left   < _iMargin ){_pRect -> left   = _iMargin ; bRet = true ;}if(_pRect -> left   >= m_iWidth  - _iMargin ){_pRect -> left   = m_iWidth  - _iMargin - 1 ;bRet = true ;}
    if(_pRect -> top    < _iMargin ){_pRect -> top    = _iMargin ; bRet = true ;}if(_pRect -> top    >= m_iHeight - _iMargin ){_pRect -> top    = m_iHeight - _iMargin - 1 ;bRet = true ;}
    if(_pRect -> right  < _iMargin ){_pRect -> right  = _iMargin ; bRet = true ;}if(_pRect -> right  >= m_iWidth  - _iMargin ){_pRect -> right  = m_iWidth  - _iMargin - 1 ;bRet = true ;}
    if(_pRect -> bottom < _iMargin ){_pRect -> bottom = _iMargin ; bRet = true ;}if(_pRect -> bottom >= m_iHeight - _iMargin ){_pRect -> bottom = m_iHeight - _iMargin - 1 ;bRet = true ;}

    return bRet ;
};

bool __stdcall CImage::SetLine (TPoint *_pSpnt , TPoint *_pEpnt , int _iMargin )
{
    bool bRet = false ;
    if(_pSpnt->x < _iMargin) {_pSpnt->x = _iMargin ;bRet = true ;} if(_pSpnt->x >= m_iWidth  - _iMargin) {_pSpnt->x = m_iWidth  - _iMargin - 1 ; bRet = true ;}
    if(_pSpnt->y < _iMargin) {_pSpnt->y = _iMargin ;bRet = true ;} if(_pSpnt->y >= m_iHeight - _iMargin) {_pSpnt->y = m_iHeight - _iMargin - 1 ; bRet = true ;}
    if(_pEpnt->x < _iMargin) {_pEpnt->x = _iMargin ;bRet = true ;} if(_pEpnt->x >= m_iWidth  - _iMargin) {_pEpnt->x = m_iWidth  - _iMargin - 1 ; bRet = true ;}
    if(_pEpnt->y < _iMargin) {_pEpnt->y = _iMargin ;bRet = true ;} if(_pEpnt->y >= m_iHeight - _iMargin) {_pEpnt->y = m_iHeight - _iMargin - 1 ; bRet = true ;}

    return bRet ;
};

bool __stdcall CImage::SetPoint (TPoint *_pPnt , int _iMargin)
{
    bool bRet = false ;
    if(_pPnt->x < _iMargin) {_pPnt->x = _iMargin ; bRet = true ;}if(_pPnt->x >= m_iWidth  - _iMargin) {_pPnt->x = m_iWidth  - _iMargin - 1 ; bRet = true ;}
    if(_pPnt->y < _iMargin) {_pPnt->y = _iMargin ; bRet = true ;}if(_pPnt->y >= m_iHeight - _iMargin) {_pPnt->y = m_iHeight - _iMargin - 1 ; bRet = true ;}

    return bRet ;
};



