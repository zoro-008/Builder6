#ifndef _DELETEDIRECTORY_H

#define _DELETEDIRECTORY_H

class CDeleteDirectory
{
public:
	CDeleteDirectory(){}
	~CDeleteDirectory() {}

	BOOL DeleteDirectory(const TCHAR* sPath); 
	BOOL IsDots(const TCHAR* str);
};

#endif