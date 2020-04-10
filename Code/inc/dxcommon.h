/*
 *	copy hge 2008.12
 */

#ifndef DXCOMMON_H
#define DXCOMMON_H

#include <windows.h>

#ifdef __BORLANDC__
#define floorf (float)floor
#define sqrtf (float)sqrt
#define acosf (float)acos
#define atan2f (float)atan2
#define cosf (float)cos
#define sinf (float)sin
#define powf (float)pow
#define fabsf (float)fabs

#define min(x,y) ((x) < (y)) ? (x) : (y)
#define max(x,y) ((x) > (y)) ? (x) : (y)
#endif

/*
** Common data types
*/
#ifndef DWORD
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
#endif


/*
** Common math constants
*/
#ifndef M_PI
#define M_PI	3.14159265358979323846f
#define M_PI_2	1.57079632679489661923f
#define M_PI_4	0.785398163397448309616f
#define M_1_PI	0.318309886183790671538f
#define M_2_PI	0.636619772367581343076f
#endif

#endif