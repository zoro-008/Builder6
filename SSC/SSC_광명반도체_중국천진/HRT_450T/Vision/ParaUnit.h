//---------------------------------------------------------------------------

#ifndef ParaUnitH
#define ParaUnitH
//---------------------------------------------------------------------------
#include <ValEdit.hpp>

#include "SVInspStruct.h"

void LoadParaEdg(bool _bLoad , EDG_Para &_tPara, AnsiString _sFilePath , AnsiString _sId);  void UpdateParaEdg(bool _bToTable , TValueListEditor * _vlList , EDG_Para &_tPara);
void LoadParaLin(bool _bLoad , LIN_Para &_tPara, AnsiString _sFilePath , AnsiString _sId);  void UpdateParaLin(bool _bToTable , TValueListEditor * _vlList , LIN_Para &_tPara);
void LoadParaLcp(bool _bLoad , LCP_Para &_tPara, AnsiString _sFilePath , AnsiString _sId);  void UpdateParaLcp(bool _bToTable , TValueListEditor * _vlList , LCP_Para &_tPara);
void LoadParaRct(bool _bLoad , RCT_Para &_tPara, AnsiString _sFilePath , AnsiString _sId);  void UpdateParaRct(bool _bToTable , TValueListEditor * _vlList , RCT_Para &_tPara);
void LoadParaWrt(bool _bLoad , WRT_Para &_tPara, AnsiString _sFilePath , AnsiString _sId);  void UpdateParaWrt(bool _bToTable , TValueListEditor * _vlList , WRT_Para &_tPara);
void LoadParaTsd(bool _bLoad , TSD_Para &_tPara, AnsiString _sFilePath , AnsiString _sId);  void UpdateParaTsd(bool _bToTable , TValueListEditor * _vlList , TSD_Para &_tPara);
void LoadParaBlb(bool _bLoad , BLB_Para &_tPara, AnsiString _sFilePath , AnsiString _sId);  void UpdateParaBlb(bool _bToTable , TValueListEditor * _vlList , BLB_Para &_tPara);
void LoadParaCal(bool _bLoad , CAL_Para &_tPara, AnsiString _sFilePath , AnsiString _sId);  void UpdateParaCal(bool _bToTable , TValueListEditor * _vlList , CAL_Para &_tPara);
void LoadParaCcl(bool _bLoad , CCL_Para &_tPara, AnsiString _sFilePath , AnsiString _sId);  void UpdateParaCcl(bool _bToTable , TValueListEditor * _vlList , CCL_Para &_tPara);
void LoadParaOcv(bool _bLoad , OCV_Para &_tPara, AnsiString _sFilePath , AnsiString _sId);  void UpdateParaOcv(bool _bToTable , TValueListEditor * _vlList , OCV_Para &_tPara);



#endif
