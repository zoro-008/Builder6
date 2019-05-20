//---------------------------------------------------------------------------

#ifndef iRotateUnitH
#define iRotateUnitH

#include "Image.h"
#include "SVInspStruct.h"
//---------------------------------------------------------------------------
class SRotate
{
    protected :

    public :
        struct TRslt {
            float  fInspectionTime ; //1024*768  395ms ;
        } Rslt;

        struct TPara {
        } Para;

        SRotate();
        ~SRotate();

        OnRotate(CImage * image , TRect rect , float rotationAngle);

};
#endif
