#include "External.h"

BOOL gLockKeyFlag;
long  g_nSingleTestThread;
MOTION_RESULT_DATA gMotionResultData;
BOOL g_bLeftRight;
EN_TEST_MODE g_iTestMode = tmNormal;
bool g_bEndding = false ;//¿£µù ÇÒ¶§ ¿¢Æ¼ºêºä°¡ ¼Ò¸ê µÆ´Âµ¥ Á¢±ÙÇÏ´Â°Í Â÷´Ü.