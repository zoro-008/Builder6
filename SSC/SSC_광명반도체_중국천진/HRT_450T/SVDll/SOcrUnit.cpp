//---------------------------------------------------------------------------
#pragma hdrstop
#include "SOcrUnit.h"
#include "SGdiUnit.h"
#include "SAutoThresUnit.h"
#include "OnDelay.h"
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
SOcr::SOcr()
{

}

SOcr::~SOcr()
{
    OnClearTrain();
}


int SOcr::OnTrain (SImage * image , TRect rect , AnsiString string)
{
    //Image
    image -> SetRect(&rect) ;

    if(!Para.iThreshold){
        SAutoThres Auto ;
        Auto.OnAutoThres(image,rect);
        Para.iThreshold = Auto.Rslt.iThreshold ;
    }

    STrace::Para = (STrace::TPara)Para ;
    STrace::Fltr = (STrace::TFltr)Fltr ;

    STrace::OnInspection(image , rect);

    AnsiString sTemp ;
    if(STrace::Rslt.lBlobs.GetDataCnt() != string.Length()) {
        sTemp = "검색된 문자의 갯수(" + (AnsiString)STrace::Rslt.lBlobs.GetDataCnt() + ")와 입력된문자 갯수(" + (AnsiString)string.Length() + ")가 틀립니다." ;
        ShowMessage(sTemp);
        return -1;
    }

    SBlob TBlob ;
    TRect Rect  ;

    //OnClearTrain();

    for(register int i = 0 ; i < STrace::Rslt.lBlobs.GetDataCnt() ; i++) {
        if(MAX_OCR_CHAR <= i+ Train.OcrBlobCnt) return -2 ;
        TBlob = STrace::Rslt.lBlobs.GetCrntData(!i);

        if(!Para.iThreshold){
            TRect      Rect ;
            SAutoThres Auto ;
            Rect.left   = TBlob.left   ;
            Rect.top    = TBlob.top    ;
            Rect.right  = TBlob.right  ;
            Rect.bottom = TBlob.bottom ;
            Auto.OnAutoThres(image,Rect);
            Para.iThreshold = Auto.Rslt.iThreshold ;
        }

        Train.OcrBlob[i+Train.OcrBlobCnt].OnClear() ;

        Rect.left   = TBlob.left   - Para.iInspOffset ;
        Rect.right  = TBlob.right  + Para.iInspOffset ;
        Rect.top    = TBlob.top    - Para.iInspOffset ;
        Rect.bottom = TBlob.bottom + Para.iInspOffset ;

        image -> SetRect(&Rect) ;

        TBlob.left   = Rect.left   ;
        TBlob.right  = Rect.right  ;
        TBlob.top    = Rect.top    ;
        TBlob.bottom = Rect.bottom ;

        Train.OcrBlob[i+Train.OcrBlobCnt].Blob  = TBlob ;
        Train.OcrBlob[i+Train.OcrBlobCnt].sText = string.SubString(i+1,1) ;

        //Boundary check.======================================================

        Train.OcrBlob[i+Train.OcrBlobCnt].InspInfo = new unsigned char[Rect.Width() * Rect.Height()] ;
        memset(Train.OcrBlob[i+Train.OcrBlobCnt].InspInfo , NO_INSP_AREA , Rect.Width() * Rect.Height() ) ;

        //Visited Pixel
        #define IPixel(x,y) (*(Train.OcrBlob[i+Train.OcrBlobCnt].InspInfo + x + Rect.Width() * y))

        int sx , tx , u;
        int sy , ty , v;

        bool isLt ;
        bool isDk ;

        unsigned char px ;

        for(register int y = Rect.top ; y < Rect.bottom ; y++) {
            for(register int x = Rect.left ; x < Rect.right ; x++) {
               isLt = false ;
               isDk = false ;

               if(Para.iThreshold > image ->GetPixel(x,y)) {
                   for(register int by = -Para.iDkIgnrCnt ; by <= Para.iDkIgnrCnt ; by++) {
                       for(register int bx = -Para.iDkIgnrCnt ; bx <= Para.iDkIgnrCnt ; bx++) {
                           px = image ->GetPixel((x+bx),(y+by)) ;
                           if(Para.iThreshold < px ) isLt = true ;
                           if(Para.iThreshold > px ) isDk = true ;
                       }
                   }
               }

               else {
                   for(register int by = -Para.iLtIgnrCnt ; by <= Para.iLtIgnrCnt ; by++) {
                       for(register int bx = -Para.iLtIgnrCnt ; bx <= Para.iLtIgnrCnt ; bx++) {
                           px = image ->GetPixel((x+bx),(y+by)) ;
                           if(Para.iThreshold < px ) isLt = true ;
                           if(Para.iThreshold > px ) isDk = true ;
                       }
                   }
               }

               if(!isLt) IPixel((x-Rect.left) ,(y-Rect.top)) = DARK_INSP_AREA   ;
               if(!isDk) IPixel((x-Rect.left) ,(y-Rect.top)) = LIGHT_INSP_AREA  ;
            }
        }

        //=====================================================================
    }

    Train.OcrBlobCnt += STrace::Rslt.lBlobs.GetDataCnt() ;

}

void SOcr::OnClearTrain()
{
    for(register int i = 0 ; i < MAX_OCR_CHAR ; i++) {

        Train.OcrBlob[i].OnClear();
    }

    Train.OcrBlobCnt = 0 ;
}

int SOcr::OnInspection(SImage * image , TRect rect , AnsiString string)
{
    float TempTime = GetTickTime() ;

    //Image
    image -> SetRect(&rect) ;

    if(!Para.iThreshold){
        SAutoThres Auto ;
        Auto.OnAutoThres(image,rect);
        Para.iThreshold = Auto.Rslt.iThreshold ;
    }

    STrace::Para = (STrace::TPara)Para ;
    STrace::Fltr = (STrace::TFltr)Fltr ;

    STrace::OnInspection(image , rect);

    Rslt.LtFailPx.DeleteAll();
    Rslt.DkFailPx.DeleteAll();

    AnsiString sTemp ;
    if(string!="") {
         if(STrace::Rslt.lBlobs.GetDataCnt() != string.Length()) {
             sTemp = "입력된문자 갯수(" + (AnsiString)STrace::Rslt.lBlobs.GetDataCnt() + ")와 검색된 문자의 갯수(" + (AnsiString)string.Length() + ")가 틀립니다." ;
             ShowMessage(sTemp);
             return -1;
         }
    }

    AnsiString InspChar   ;
    bool       IsBlobPass = false ;
    SBlob      Blob ;

    if(string!="") {   //객관식 오씨알.
        for(register int i = 0 ; i < STrace::Rslt.lBlobs.GetDataCnt() ; i++) {
            InspChar = string.SubString(i+1,1) ;
            Blob = STrace::Rslt.lBlobs.GetCrntData(!i) ;
            //iMaxFBlobInPxCnt = 0 ;
            //iMaxFBlobOtPxCnt = 0 ;

            for(register int c = 0 ; c < MAX_OCR_CHAR ; c++) {
                if(Train.OcrBlobCnt<=c) break ;

                if(Train.OcrBlob[c].sText == InspChar) {
                    //iFBlobInPxCnt = 0 ;
                    //iFBlobOtPxCnt = 0 ;
                    //Train.OcrBlob[c].Blob.dCentroidX - Train.OcrBlob[c].Blob.left  ;
                    //Train.OcrBlob[c].Blob.dCentroidY - Train.OcrBlob[c].Blob.top   ;

                    #define IPixel(x,y) (*(Train.OcrBlob[c].InspInfo + x + Train.OcrBlob[c].Blob.Width() * y))
                    for(register int by = 0 ; by < Train.OcrBlob[c].Blob.Height() ; by++) {
                       for(register int bx = 0 ; bx < Train.OcrBlob[c].Blob.Width() ; bx++) {

                           int  sx  = Blob.dCentroidX - (Train.OcrBlob[c].Blob.dCentroidX - Train.OcrBlob[c].Blob.left) ;
                           int  sy  = Blob.dCentroidY - (Train.OcrBlob[c].Blob.dCentroidY - Train.OcrBlob[c].Blob.top ) ;
                           BYTE px1 = image ->GetPixel((sx+bx  ),(sy+by  )) ;
                           BYTE px2 = image ->GetPixel((sx+bx+1),(sy+by  )) ;
                           BYTE px3 = image ->GetPixel((sx+bx+1),(sy+by+1)) ;
                           BYTE px4 = image ->GetPixel((sx+bx  ),(sy+by+1)) ;

                           TPoint pnt1 ; pnt1.x = sx+bx   ; pnt1.y = sy+by   ;
                           TPoint pnt2 ; pnt2.x = sx+bx+1 ; pnt2.y = sy+by   ;
                           TPoint pnt3 ; pnt3.x = sx+bx+1 ; pnt3.y = sy+by+1 ;
                           TPoint pnt4 ; pnt4.x = sx+bx   ; pnt4.y = sy+by+1 ;

                           if(IPixel(bx ,by) == DARK_INSP_AREA ) {

                               if(Para.bFitting) {
                                   if(Para.iThreshold < px1 - Para.iDkOffset &&
                                      Para.iThreshold < px2 - Para.iDkOffset &&
                                      Para.iThreshold < px3 - Para.iDkOffset &&
                                      Para.iThreshold < px4 - Para.iDkOffset )
                                       Rslt.DkFailPx.PushBack(pnt1) ;
                               //  else
                               //      Rslt.DkGoodPx.PushBack(pnt1) ;
                               }

                               else {
                                   if(Para.iThreshold < px1 - Para.iDkOffset )
                                       Rslt.DkFailPx.PushBack(pnt1) ;
                               //  else
                               //      Rslt.DkGoodPx.PushBack(pnt1) ;
                               }


                           }

                           if(IPixel(bx ,by) == LIGHT_INSP_AREA) {
                               if(Para.bFitting) {
                                   if(Para.iThreshold > px1 + Para.iLtOffset &&
                                      Para.iThreshold > px2 + Para.iLtOffset &&
                                      Para.iThreshold > px3 + Para.iLtOffset &&
                                      Para.iThreshold > px4 + Para.iLtOffset )
                                       Rslt.LtFailPx.PushBack(pnt1) ;
                                   //   else
                                   //       Rslt.LtGoodPx.PushBack(pnt1) ;
                               }
                               else {
                                   if(Para.iThreshold > px1 + Para.iLtOffset )
                                       Rslt.LtFailPx.PushBack(pnt1) ;
                               }
                           }

                       }
                   }
                   break ;
                }
            }
        }
    }

    else {  //주관식 오씨알.
        int iMinGap = 0 ;
        int iGapCnt = 0 ;
        AnsiString sChar = "" ;
        Rslt.sString = "" ;

        for(register int i = 0 ; i < STrace::Rslt.lBlobs.GetDataCnt() ; i++) {
            Blob = STrace::Rslt.lBlobs.GetCrntData(!i) ;

            if(!Para.iThreshold){
                TRect      Rect ;
                SAutoThres Auto ;
                Rect.left   = Blob.left   ;
                Rect.top    = Blob.top    ;
                Rect.right  = Blob.right  ;
                Rect.bottom = Blob.bottom ;
                Auto.OnAutoThres(image,Rect);
                Para.iThreshold = Auto.Rslt.iThreshold ;
            }

            iMinGap = Blob.Height() * Blob.Width()  ;
            sChar   = "" ;
            for(register int c = 0 ; c < Train.OcrBlobCnt ; c++) {
                iGapCnt = 0  ;
                #define IPixel(x,y) (*(Train.OcrBlob[c].InspInfo + x + Train.OcrBlob[c].Blob.Width() * y))
                for(register int by = 0 ; by < Train.OcrBlob[c].Blob.Height() ; by++) {
                    for(register int bx = 0 ; bx < Train.OcrBlob[c].Blob.Width() ; bx++) {

                        int  sx  = Blob.dCentroidX - (Train.OcrBlob[c].Blob.dCentroidX - Train.OcrBlob[c].Blob.left) ;
                        int  sy  = Blob.dCentroidY - (Train.OcrBlob[c].Blob.dCentroidY - Train.OcrBlob[c].Blob.top ) ;
                        BYTE px1 = image ->GetPixel((sx+bx  ),(sy+by  )) ;
                        BYTE px2 = image ->GetPixel((sx+bx+1),(sy+by  )) ;
                        BYTE px3 = image ->GetPixel((sx+bx+1),(sy+by+1)) ;
                        BYTE px4 = image ->GetPixel((sx+bx  ),(sy+by+1)) ;

                        TPoint pnt1 ; pnt1.x = sx+bx   ; pnt1.y = sy+by   ;
                        TPoint pnt2 ; pnt2.x = sx+bx+1 ; pnt2.y = sy+by   ;
                        TPoint pnt3 ; pnt3.x = sx+bx+1 ; pnt3.y = sy+by+1 ;
                        TPoint pnt4 ; pnt4.x = sx+bx   ; pnt4.y = sy+by+1 ;

                        if(IPixel(bx ,by) == DARK_INSP_AREA ) {

                            if(Para.bFitting) {
                                if(Para.iThreshold < px1 - Para.iDkOffset &&
                                   Para.iThreshold < px2 - Para.iDkOffset &&
                                   Para.iThreshold < px3 - Para.iDkOffset &&
                                   Para.iThreshold < px4 - Para.iDkOffset )
                                    iGapCnt++ ;
                            }

                            else {
                                if(Para.iThreshold < px1 - Para.iDkOffset )
                                    iGapCnt++ ;
                            }
                        }

                        if(IPixel(bx ,by) == LIGHT_INSP_AREA) {
                            if(Para.bFitting) {
                                if(Para.iThreshold > px1 + Para.iLtOffset &&
                                   Para.iThreshold > px2 + Para.iLtOffset &&
                                   Para.iThreshold > px3 + Para.iLtOffset &&
                                   Para.iThreshold > px4 + Para.iLtOffset )
                                    iGapCnt++ ;
                            }
                            else {
                                if(Para.iThreshold > px1 + Para.iLtOffset )
                                    iGapCnt++ ;
                            }
                        }


                    }
                }
                if(iGapCnt < iMinGap) {
                    iMinGap = iGapCnt ;
                    sChar   = Train.OcrBlob[c].sText ;
                }

            }
            Rslt.sString += sChar ;
        }
    }

    Rslt.fInspectionTime = GetTickTime() - TempTime ;
//    Rslt.bDsply = true ;
}


void SOcr::OnPaintRslt(HWND h)
{
    SGdi *Gdi = new SGdi(h);

    #define IPixel(x,y) (*(Train.OcrBlob[i].InspInfo + x + Train.OcrBlob[i].Blob.Width() * y))

    TPoint     pnt   ;
    SBlob      blob  ;
    AnsiString sTemp ;

//    Gdi -> CText.Color = Graphics::clBlue ;
//    Gdi -> Text( STrace::Rslt.lBlobs.GetCrntData(true).left , STrace::Rslt.lBlobs.GetCrntData(true).top - 25 ,  Rslt.sString.c_str()) ;


    for(register int i = 0 ; i < STrace::Rslt.lBlobs.GetDataCnt() ; i++) {
        blob = STrace::Rslt.lBlobs.GetCrntData(!i);
        Gdi -> CPen .Color = Graphics::clBlue;
        Gdi -> Rect(0,blob.left - 2 , blob.top - 2 , blob.right + 2 , blob.bottom + 2);
        //sTemp.sprintf("%03d",i);
//        sTemp = Rslt.sString.SubString(i+1,1);
//        Gdi -> CText.Color = Graphics::clBlue ;
//        Gdi -> Text( blob.right , blob.bottom + 1 , sTemp.c_str()) ;
    }

    for(register int i = 0 ; i < STrace::Rslt.lBlobs.GetDataCnt() ; i++) {
        blob = STrace::Rslt.lBlobs.GetCrntData(!i);
        Gdi -> CPen .Color = Graphics::clAqua;
        sTemp = Rslt.sString.SubString(i+1,1);
        Gdi -> Text( blob.right , blob.bottom + 1 , sTemp.c_str()) ;
    }

//    for(register int i = 0 ; i < Rslt.LtGoodPx.GetDataCnt() ; i++) {
//        pnt = Rslt.LtGoodPx.GetCrntData(!i) ;
//        Gdi -> Pixel(pnt,clYellow);
//    }
//
//    for(register int i = 0 ; i < Rslt.DkGoodPx.GetDataCnt() ; i++) {
//        pnt = Rslt.DkGoodPx.GetCrntData(!i) ;
//        Gdi -> Pixel(pnt,clBlack);
//    }

    for(register int i = 0 ; i < Rslt.DkFailPx.GetDataCnt() ; i++) {
        pnt = Rslt.DkFailPx.GetCrntData(!i) ;
        Gdi -> Pixel(pnt,Graphics::clRed);
    }

    for(register int i = 0 ; i < Rslt.LtFailPx.GetDataCnt() ; i++) {
        pnt = Rslt.LtFailPx.GetCrntData(!i) ;
        Gdi -> Pixel(pnt,Graphics::clFuchsia);
    }

//    for(register int i = 0 ; i < Train.OcrBlobCnt ; i++) {
//        for(register int y = 0 ; y < Train.OcrBlob[i].Blob.Height() ; y++) {
//            for(register int x = 0 ; x < Train.OcrBlob[i].Blob.Width() ; x++) {
//
//               if     (IPixel(x,y) == DARK_INSP_AREA )
//                   Gdi -> Pixel(Train.OcrBlob[i].Blob.left + x , Train.OcrBlob[i].Blob.top + y - 40,clLime);
//               else if(IPixel(x,y) == LIGHT_INSP_AREA  )
//                   Gdi -> Pixel(Train.OcrBlob[i].Blob.left + x , Train.OcrBlob[i].Blob.top + y - 40,clBlue);
//            }
//        }
//    }
//
//    for(register int i = 0 ; i < Train.OcrBlobCnt ; i++) {
//        Gdi -> CText.Color = clBlue ;
//        Gdi -> Text( Train.OcrBlob[i].Blob.right , Train.OcrBlob[i].Blob.bottom , Train.OcrBlob[i].sText.c_str()) ;
//    }

    Gdi -> CText.Color = Graphics::clFuchsia ;
    sTemp.sprintf("%02d",(int)(Rslt.fInspectionTime));
    //sTemp = (AnsiString)"Insp Time : " + sTemp + (AnsiString)"ms" ;
    Gdi -> Text( 5, 5 , sTemp.c_str() ) ;

    delete Gdi;
}

void SOcr::OnPaintTrain(HWND h)
{
    SGdi *Gdi = new SGdi(h);

    #define IPixel(x,y) (*(Train.OcrBlob[i].InspInfo + x + Train.OcrBlob[i].Blob.Width() * y))

    for(register int i = 0 ; i < Train.OcrBlobCnt ; i++) {
        for(register int y = 0 ; y < Train.OcrBlob[i].Blob.Height() ; y++) {
            for(register int x = 0 ; x < Train.OcrBlob[i].Blob.Width() ; x++) {

               if     (IPixel(x,y) == DARK_INSP_AREA )
                   Gdi -> Pixel(Train.OcrBlob[i].Blob.left + x , Train.OcrBlob[i].Blob.top + y ,Graphics::clLime);
               else if(IPixel(x,y) == LIGHT_INSP_AREA )
                   Gdi -> Pixel(Train.OcrBlob[i].Blob.left + x , Train.OcrBlob[i].Blob.top + y ,Graphics::clBlue);
            }
        }
    }

    for(register int i = 0 ; i < Train.OcrBlobCnt ; i++) {
        Gdi -> CText.Color = Graphics::clAqua ;
        Gdi -> Text( Train.OcrBlob[i].Blob.right , Train.OcrBlob[i].Blob.bottom , Train.OcrBlob[i].sText.c_str()) ;
    }

    delete Gdi;
}

bool SOcr::OnLoad(AnsiString FilePath)
{

}

bool SOcr::OnSave(AnsiString FilePath)
{

}
