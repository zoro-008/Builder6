#ifndef __AXT_TRIGGER_H__
#define __AXT_TRIGGER_H__

/*------------------------------------------------------------------------------------------------*
	AXTMOTION Library - CAMC-FS20 Module
	적용제품
		SMC-2V02 Ver 2.0 이상
 *------------------------------------------------------------------------------------------------*/
#include "AxtLIBDef.h"
#include "AxtTriggerDef.h"

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	BOOL	PASCAL EXPORT CFS20Trig_Profile_Reset(INT16 axis);	
	BOOL	PASCAL EXPORT CFS20Trig_Absolute_Profile_Set(INT16 axis, INT16 trig_num, double* trig_pos, double trig_time, BOOL trig_target);		
	BOOL	PASCAL EXPORT CFS20Trig_Periodic_Profile_Set(INT16 axis, double start_pos, double end_pos, double cycle, double trig_time, BOOL trig_target);
	BOOL	PASCAL EXPORT CFS20Trig_Periodic_Profile_Set2(INT16 axis, double start_pos, int nCount, double cycle, double trig_time, BOOL trig_target);
	BOOL	PASCAL EXPORT CFS20Trig_One_Shot_Set(INT16 axis, UINT8 trig_level, double trig_time);
	BOOL	PASCAL EXPORT CFS20Trig_Set_Level(INT16 axis, UINT8 level);
	

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_TRIGGER_H__
