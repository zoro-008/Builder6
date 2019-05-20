/*****************************************************
** Module : CBitmapEx                               **
** Purpose : Manage and draw bitmap template        **
**                                                  **
** Copyright James Spibey, 2000 (spib@bigfoot.com)  **
** Feel free to use and distribute. May not be sold ** 
** for profit.                                      **
*****************************************************/

#if !defined(AFX_BITMAPEX_H__018CE6D7_1E42_11D3_B643_00C04F6801E7__INCLUDED_)
#define AFX_BITMAPEX_H__018CE6D7_1E42_11D3_B643_00C04F6801E7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CBitmapEx  
{
public:
	CBitmapEx();
	CBitmapEx(UINT ID);
	CBitmapEx(CString strBmpFile);
	virtual ~CBitmapEx();

protected:
	BITMAP	m_Bitmap;
	HBITMAP m_hBitmap;
	CBitmap	m_cBitmap;
	int		m_nWidth;
	int		m_nHeight;

public:
	BOOL	LoadBitmap(CString strBmpFile);
	BOOL	LoadBitmap(UINT uID);				
	
	BOOL	Draw(BOOL bPaint, CWnd *pWnd, int xDest, int yDest, int nDx = 0, int nDy = 0, int nFromWhereX = -1, int nFromWhereY = -1);
	BOOL	Draw(BOOL bPaint, CWnd *pWnd, int XDest, int YDest, CRect Image)
	{
		return Draw(bPaint, pWnd, 
			XDest, 
			YDest,
			Image.Width(),
			Image.Height(),
			Image.left,
			Image.top);
	}

	BOOL	Draw(BOOL bPaint, CWnd *pWnd, CRect Dest, CRect Image)
	{
		return Draw(bPaint, pWnd, 
			Dest.left, 
			Dest.top,
			Image.Width(),
			Image.Height(),
			Image.left,
			Image.top);
	}

	HRGN	CreateRegion(BOOL bFromImage = TRUE, COLORREF colour = NULL);
	
	//Access Functions
	inline	int	GetHeight(){return m_nHeight;}
	inline	int	GetWidth(){return m_nWidth;}
	
};

#endif // !defined(AFX_BITMAPEX_H__018CE6D7_1E42_11D3_B643_00C04F6801E7__INCLUDED_)
