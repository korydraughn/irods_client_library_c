//
// bulkOprInfo.h
//

#ifndef IRODS_BULK_OPR_INFO_H
#define IRODS_BULK_OPR_INFO_H

#include "rodsDef.h"

typedef struct BulkOprInfo {
    int flags;
    int count;
    int forceFlagAdded;
    int size;
    char cwd[MAX_NAME_LEN];
    char phyBunDir[MAX_NAME_LEN];
    char cachedTargPath[MAX_NAME_LEN];
    char cachedSubPhyBunDir[MAX_NAME_LEN];
    char phyBunPath[MAX_NUM_BULK_OPR_FILES][MAX_NAME_LEN];
    bytesBuf_t bytesBuf;
} bulkOprInfo_t;

#endif // IRODS_BULK_OPR_INFO_H

