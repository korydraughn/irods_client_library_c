/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

/* apiNumber.h - header file for API number assignment
 */

#ifndef API_NUMBER_H__
#define API_NUMBER_H__

#define API_NUMBER(NAME, VALUE) extern const int NAME;

#ifdef __cplusplus
extern "C"
{
#endif

#include "apiNumberData.h"

#ifdef __cplusplus
}
#endif

#undef API_NUMBER

#endif  // API_NUMBER_H__
