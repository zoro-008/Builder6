/*****************************************************
** Module : CBitmapEx                               **
** Purpose : Manage and draw bitmap template        **
**                                                  **
** Copyright James Spibey, 2000 (spib@bigfoot.com)  **
** Feel free to use and distribute. May not be sold ** 
** for profit.                                      **
*****************************************************/

#include "stdafx.h"
#include "BitmapEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitmapEx::CBitmapEx()
{

}

CBitmapEx::CBitmapEx(CString strBmpFile)
{
	m_nWidth = 0;
	m_nHeight = 0;

	if (!strBmpFile.IsEmpty())
		LoadBitmap(strBmpFile);
}

CBitmapEx::CBitmapEx(UINT ID)
{
	m_nWidth	= 0;
	m_nHeight	= 0;

	if (ID != 0)
		LoadBitmap(ID);
}

CBitmapEx::~CBitmapEx()
{
	//Delete the bitmap
	m_cBitmap.DeleteObject();
	::DeleteObject(m_hBitmap);
}


BOOL CBitmapEx::LoadBitmap(CString strBmpFile)
{
	m_hBitmap=(HBITMAP)LoadImage( NULL, strBmpFile, IMAGE_BITMAP, 0,0,
		LR_DEFAULTCOLOR |
		LR_LOADFROMFILE |
		LR_CREATEDIBSECTION);

	if (m_hBitmap == NULL)
		return FALSE;
	
	if (m_cBitmap.Attach(m_hBitmap)==FALSE)
		return FALSE;
	
	if (m_cBitmap.GetBitmap(&m_Bitmap)==0)
		return FALSE;
	
	m_nWidth = m_Bitmap.bmWidth;
	m_nHeight = m_Bitmap.bmHeight;
	
	return TRUE;
}

BOOL CBitmapEx::LoadBitmap(UINT uID)
{
	//Load the bitmap from the ID resource
	//Get the size of the loaded bitmap
	if ( m_cBitmap.LoadBitmap(uID) && m_cBitmap.GetBitmap(&m_Bitmap)) 
	{
		//Set the public value
		m_nWidth	= m_Bitmap.bmWidth;
		m_nHeight	= m_Bitmap.bmHeight;
		return TRUE;
	}
	return FALSE;
}


BOOL CBitmapEx::Draw(BOOL bPaint, CWnd *pWnd, int xDest, int yDest, int nDx, int nDy, int nFromWhereX, int nFromWhereY)
{
	CDC memDC;
	CPaintDC dcPaint(pWnd);
	CClientDC dcClient(pWnd);

	if(bPaint) 
	{
		if(!memDC.CreateCompatibleDC(&dcPaint))
			return FALSE;

		memDC.SelectObject(&m_cBitmap);
		
		return(dcPaint.BitBlt(xDest, yDest,
		(nDx == 0) ? m_nWidth : nDx,
		(nDy == 0) ? m_nHeight : nDy, 
		&memDC,
		(nFromWhereX == -1) ? 0 : nFromWhereX,
		(nFromWhereY == -1) ? 0 : nFromWhereY,
		SRCCOPY));
	} 
	else 
	{
		if(!memDC.CreateCompatibleDC(&dcClient))
			return FALSE;

		memDC.SelectObject(&m_cBitmap);
		
		return(dcClient.BitBlt(xDest, yDest,
		(nDx == 0) ? m_nWidth : nDx,
		(nDy == 0) ? m_nHeight : nDy, 
		&memDC,
		(nFromWhereX == -1) ? 0 : nFromWhereX,
		(nFromWhereY == -1) ? 0 : nFromWhereY,
		SRCCOPY));
	}
}


HRGN CBitmapEx::CreateRegion(BOOL bFromImage /* = TRUE */, COLORREF colour /* = NULL */)
{
	HRGN mainRgn;
	HRGN tmpRgn;
	COLORREF tc;
	int x, y, l = 0;
	BOOL bFirstTime = TRUE; 
	BOOL bEndofLine = FALSE;
	CDC memDC;
	
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject(m_cBitmap);

	// Scan the bitmap pixel by pixel looking for transparencies
	// and then creating a region by ORing the transparencies to remove them
	for(y = 0; y < m_nHeight; y++) 
	{
		for(x = 0; x <= m_nWidth; x++) 
		{
			//If we are in the top left corner
			//get the colour of this pixel
			//This will be used as our transparency color
			if ((x == 0 || y == 0) && bFromImage)
				tc = memDC.GetPixel(0,0);
			
			//If this pixel is the same color as our tansparency
			// or we are on the right hand side of the image
			if((memDC.GetPixel(x, y) == tc) || (x == m_nWidth))	
			{
				//If this boolean value is true
				if(bEndofLine) 
				{
					bEndofLine = FALSE;
					
					//Create a region which is as big as our image so far
					tmpRgn = CreateRectRgn(l, y, x, y+1);
					
					//If this is the first time round
					if(bFirstTime) 
					{
						mainRgn = tmpRgn;
						bFirstTime = FALSE;
					} 
					else 
						CombineRgn(mainRgn, mainRgn, tmpRgn, RGN_OR);
				}
			} 
			else 
			{
				if(!bEndofLine) 
				{
					bEndofLine = TRUE;
					l = x;
				}
			}
		}
	}
	
	return mainRgn;
}
