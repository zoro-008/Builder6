//---------------------------------------------------------------------------
#ifndef aEdgeH
#define aEdgeH
//---------------------------------------------------------------------------
#include "SVInspStruct.h"
#include "Image.h"

//
//

//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------
class CEdge {
    private :
    protected :

        //�����V ���� ��ġ Ȯ�ο�...
        TPoint m_pOfsLtToDk ;
        TPoint m_pOfsDkToLt ;

    public :
        //Functions
        CEdge();
        ~CEdge();

    public  :
        bool       Inspect  (CImage * _pImg, TPoint  _tSttPnt , TPoint  _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt);
        bool       Inspect  (CImage * _pImg, TFPoint _tSttPnt , TFPoint _tEndPnt , EDG_Para _tPara , EDG_Rslt * _pRslt); //float����Ʈ�� ���� �ϴ°� �ƴϰ� ���� int������ �ٲ㼭 �Ѵ�.
        void       PaintRslt(HDC _hHdc , EDG_Rslt * _pRslt , EDG_Disp _tDisp , float _fScaleX=1.0 , float _fScaleY=1.0);
};
//---------------------------------------------------------------------------
#endif