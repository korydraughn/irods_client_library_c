#include "apiNumber.h"

#define API_NUMBER(NAME, VALUE) const int NAME = VALUE;

#ifdef __cplusplus
extern "C"
{
#endif

#include "apiNumberData.h"

#ifdef __cplusplus
}
#endif

#undef API_NUMBER
