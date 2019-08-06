/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

/* rcMisc.c - misc client routines
 */
#ifndef windows_platform
#include <sys/time.h>
#else
#include "Unix2Nt.hpp"
#endif
#include "rcMisc.h"
#include "apiHeaderAll.h"
#include "modDataObjMeta.h"
#include "rcGlobalExtern.h"
#include "rodsGenQueryNames.h"
#include "rodsType.h"
#include "dataObjPut.h"

#include "bulkDataObjPut.h"

#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <string>

// =-=-=-=-=-=-=-
#include "irods_virtual_path.hpp"
#include "irods_hierarchy_parser.hpp"

// =-=-=-=-=-=-=-
// boost includes
#include <boost/filesystem/operations.hpp>

rodsLong_t
getFileSize( char *myPath ) {
    namespace fs = boost::filesystem;

    fs::path p( myPath );

    if ( exists( p ) && is_regular_file( p ) ) {
        return file_size( p );
    }
    else {
        return -1;
    }
}

int freeBBuf( bytesBuf_t *myBBuf ) {
    if ( myBBuf == NULL ) {
        return 0;
    }

    if ( myBBuf->buf != NULL ) {
        free( myBBuf->buf );
    }
    free( myBBuf );
    return 0;
}

int
clearBBuf( bytesBuf_t *myBBuf ) {
    if ( myBBuf == NULL ) {
        return 0;
    }

    if ( myBBuf->buf != NULL ) {
        free( myBBuf->buf );
    }

    memset( myBBuf, 0, sizeof( bytesBuf_t ) );
    return 0;
}

int
freeRError( rError_t *myError ) {

    if ( myError == NULL ) {
        return 0;
    }

    freeRErrorContent( myError );
    free( myError );
    return 0;
}

int
freeRErrorContent( rError_t *myError ) {
    int i;

    if ( myError == NULL ) {
        return 0;
    }

    if ( myError->len > 0 ) {
        for ( i = 0; i < myError->len; i++ ) {
            free( myError->errMsg[i] );
        }
        free( myError->errMsg );
    }

    memset( myError, 0, sizeof( rError_t ) );

    return 0;
}

int
myHtonll( rodsLong_t inlonglong, rodsLong_t *outlonglong ) {
    char *inPtr, *outPtr;

    if ( outlonglong == NULL ) {
        return SYS_INTERNAL_NULL_INPUT_ERR;
    }

    if ( ntohl( 1 ) == 1 ) {
        *outlonglong = inlonglong;
        return 0;
    }

    inPtr = ( char * )( static_cast< void * >( &inlonglong ) );
    outPtr = ( char * )( static_cast<void *>( outlonglong ) );

    int i;
    int byte_length = sizeof( rodsLong_t );
    for ( i = 0; i < byte_length; i++ ) {
        outPtr[i] = inPtr[byte_length - 1 - i];
    }
    return 0;
}

int
myNtohll( rodsLong_t inlonglong,  rodsLong_t *outlonglong ) {
    char *inPtr, *outPtr;

    if ( outlonglong == NULL ) {
        return SYS_INTERNAL_NULL_INPUT_ERR;
    }

    if ( ntohl( 1 ) == 1 ) {
        *outlonglong = inlonglong;
        return 0;
    }

    inPtr = ( char * )( static_cast< void * >( &inlonglong ) );
    outPtr = ( char * )( static_cast<void *>( outlonglong ) );

    int i;
    int byte_length = sizeof( rodsLong_t );
    for ( i = 0; i < byte_length; i++ ) {
        outPtr[i] = inPtr[byte_length - 1 - i];
    }
    return 0;
}

int getZoneNameFromHint(
    const char* _hint,
    char*       _zone_name,
    int         _len ) {
    if ( !_hint ) {
        _zone_name[0] = '\0';
        return 0;
    }

    const std::string sep = irods::get_virtual_path_separator();

    std::string hint( _hint );
    if ( sep[0] == hint[0] ) {
        std::string::size_type pos = hint.find( sep, 1 );
        if ( std::string::npos != pos ) {
            hint = hint.substr( 1, pos - 1 );

        }
        else {
            hint = hint.substr( 1 );

        }
    }

    snprintf( _zone_name, _len, "%s", hint.c_str() );

    return 0;
}

int
freeDataObjInfo( dataObjInfo_t *dataObjInfo ) {
    if ( dataObjInfo == NULL ) {
        return 0;
    }

    clearKeyVal( &dataObjInfo->condInput );
    /* separate specColl */
    if ( dataObjInfo->specColl != NULL ) {
        free( dataObjInfo->specColl );
    }

    free( dataObjInfo );
    dataObjInfo = 0;
    return 0;
}

char *
getValByKey( const keyValPair_t *condInput, const char *keyWord ) {
    int i;

    if ( condInput == NULL ) {
        return NULL;
    }

    for ( i = 0; i < condInput->len; i++ ) {
        if ( strcmp( condInput->keyWord[i], keyWord ) == 0 ) {
            return condInput->value[i];
        }
    }

    return NULL;
}

int
rmKeyVal( keyValPair_t *condInput, char *keyWord ) {
    int i, j;

    if ( condInput == NULL ) {
        return 0;
    }

    for ( i = 0; i < condInput->len; i++ ) {
        if ( condInput->keyWord[i] != NULL &&
                strcmp( condInput->keyWord[i], keyWord ) == 0 ) {
            free( condInput->keyWord[i] );
            free( condInput->value[i] );
            condInput->len--;
            for ( j = i; j < condInput->len; j++ ) {
                condInput->keyWord[j] = condInput->keyWord[j + 1];
                condInput->value[j] = condInput->value[j + 1];
            }
            if ( condInput->len <= 0 ) {
                free( condInput->keyWord );
                free( condInput->value );
                condInput->value = condInput->keyWord = NULL;
            }
            break;
        }
    }
    return 0;
}

int
replKeyVal( const keyValPair_t *srcCondInput, keyValPair_t *destCondInput ) {
    int i;

    memset( destCondInput, 0, sizeof( keyValPair_t ) );

    for ( i = 0; i < srcCondInput->len; i++ ) {
        addKeyVal( destCondInput, srcCondInput->keyWord[i],
                   srcCondInput->value[i] );
    }
    return 0;
}

int
replSpecColl( specColl_t *inSpecColl, specColl_t **outSpecColl ) {
    if ( inSpecColl == NULL || outSpecColl == NULL ) {
        return USER__NULL_INPUT_ERR;
    }
    *outSpecColl = ( specColl_t * )malloc( sizeof( specColl_t ) );
    *( *outSpecColl ) = *inSpecColl;

    return 0;
}

int
addKeyVal( keyValPair_t *condInput, const char *keyWord, const char *value ) {
    if ( condInput == NULL ) {
        return SYS_INTERNAL_NULL_INPUT_ERR;
    }
    if ( condInput->keyWord == NULL || condInput->value == NULL ) {
        condInput->len = 0;
    }

    /* check if the keyword exists */
    for ( int i = 0; i < condInput->len; i++ ) {
        if ( condInput->keyWord[i] == NULL || strlen( condInput->keyWord[i] ) == 0 ) {
            free( condInput->keyWord[i] );
            free( condInput->value[i] );
            condInput->keyWord[i] = strdup( keyWord );
            condInput->value[i] = value ? strdup( value ) : NULL;
            return 0;
        }
        else if ( strcmp( keyWord, condInput->keyWord[i] ) == 0 ) {
            free( condInput->value[i] );
            condInput->value[i] = value ? strdup( value ) : NULL;
            return 0;
        }
    }

    if ( ( condInput->len % PTR_ARRAY_MALLOC_LEN ) == 0 ) {
        condInput->keyWord = ( char ** )realloc( condInput->keyWord,
                             ( condInput->len + PTR_ARRAY_MALLOC_LEN ) * sizeof( *condInput->keyWord ) );
        condInput->value = ( char ** )realloc( condInput->value,
                                               ( condInput->len + PTR_ARRAY_MALLOC_LEN ) * sizeof( *condInput->value ) );
        memset( condInput->keyWord + condInput->len, 0, PTR_ARRAY_MALLOC_LEN * sizeof( *condInput->keyWord ) );
        memset( condInput->value + condInput->len, 0, PTR_ARRAY_MALLOC_LEN * sizeof( *condInput->value ) );
    }

    condInput->keyWord[condInput->len] = strdup( keyWord );
    condInput->value[condInput->len] = value ? strdup( value ) : NULL;
    condInput->len++;

    return 0;
}

int
addInxIval( inxIvalPair_t *inxIvalPair, int inx, int value ) {
    int *newInx;
    int *newValue;
    int newLen;
    int i;

    if ( inxIvalPair == NULL ) {
        return SYS_INTERNAL_NULL_INPUT_ERR;
    }

    if ( ( inxIvalPair->len % PTR_ARRAY_MALLOC_LEN ) == 0 ) {
        newLen = inxIvalPair->len + PTR_ARRAY_MALLOC_LEN;
        newInx = ( int * ) malloc( newLen * sizeof( int ) );
        newValue = ( int * ) malloc( newLen * sizeof( int ) );
        memset( newInx, 0, newLen * sizeof( int ) );
        memset( newValue, 0, newLen * sizeof( int ) );
        for ( i = 0; i < inxIvalPair->len; i++ ) {
            newInx[i] = inxIvalPair->inx[i];
            newValue[i] = inxIvalPair->value[i];
        }
        if ( inxIvalPair->inx != NULL ) {
            free( inxIvalPair->inx );
        }
        if ( inxIvalPair->value != NULL ) {
            free( inxIvalPair->value );
        }
        inxIvalPair->inx = newInx;
        inxIvalPair->value = newValue;
    }

    inxIvalPair->inx[inxIvalPair->len] = inx;
    inxIvalPair->value[inxIvalPair->len] = value;
    inxIvalPair->len++;

    return 0;
}

int
addInxVal( inxValPair_t *inxValPair, int inx, const char *value ) {
    int *newInx;
    char **newValue;
    int newLen;
    int i;

    if ( inxValPair == NULL ) {
        return SYS_INTERNAL_NULL_INPUT_ERR;
    }

    if ( ( inxValPair->len % PTR_ARRAY_MALLOC_LEN ) == 0 ) {
        newLen = inxValPair->len + PTR_ARRAY_MALLOC_LEN;
        newInx = ( int * ) malloc( newLen * sizeof( *newInx ) );
        newValue = ( char ** ) malloc( newLen * sizeof( *newValue ) );
        memset( newInx, 0, newLen * sizeof( *newInx ) );
        memset( newValue, 0, newLen * sizeof( *newValue ) );
        for ( i = 0; i < inxValPair->len; i++ ) {
            newInx[i] = inxValPair->inx[i];
            newValue[i] = inxValPair->value[i];
        }
        if ( inxValPair->inx != NULL ) {
            free( inxValPair->inx );
        }
        if ( inxValPair->value != NULL ) {
            free( inxValPair->value );
        }
        inxValPair->inx = newInx;
        inxValPair->value = newValue;
    }

    inxValPair->inx[inxValPair->len] = inx;
    inxValPair->value[inxValPair->len] = strdup( value );
    inxValPair->len++;

    return 0;
}

int
clearKeyVal( keyValPair_t *condInput ) {

    if ( condInput == NULL || condInput->len < 1 ) {
        return 0;
    }

    for ( int i = 0; i < condInput->len; i++ ) {
        if ( condInput->keyWord != NULL ) {
            free( condInput->keyWord[i] );
        }
        if ( condInput->value != NULL ) {
            free( condInput->value[i] );
        }
    }

    free( condInput->keyWord );
    free( condInput->value );
    memset( condInput, 0, sizeof( keyValPair_t ) );
    return 0;
}

int
clearInxIval( inxIvalPair_t *inxIvalPair ) {
    if ( inxIvalPair == NULL || inxIvalPair->len <= 0 ) {
        return 0;
    }

    free( inxIvalPair->inx );
    free( inxIvalPair->value );
    memset( inxIvalPair, 0, sizeof( inxIvalPair_t ) );

    return 0;
}

int
clearInxVal( inxValPair_t *inxValPair ) {
    int i;

    if ( inxValPair == NULL || inxValPair->len <= 0 ) {
        return 0;
    }

    for ( i = 0; i < inxValPair->len; i++ ) {
        free( inxValPair->value[i] );
    }

    free( inxValPair->inx );
    free( inxValPair->value );
    memset( inxValPair, 0, sizeof( inxValPair_t ) );

    return 0;
}

void
clearGenQueryInp( void* voidInp ) {

    if ( voidInp == NULL ) {
        return;
    }

    genQueryInp_t *genQueryInp = ( genQueryInp_t* ) voidInp;
    clearInxIval( &genQueryInp->selectInp );
    clearInxVal( &genQueryInp->sqlCondInp );
    clearKeyVal( &genQueryInp->condInput );

    return;
}

int
freeGenQueryOut( genQueryOut_t **genQueryOut ) {
    if ( genQueryOut == NULL ) {
        return 0;
    }

    if ( *genQueryOut == NULL ) {
        return 0;
    }

    clearGenQueryOut( *genQueryOut );
    free( *genQueryOut );
    *genQueryOut = NULL;

    return 0;
}

void
clearGenQueryOut( void* voidInp ) {
    genQueryOut_t *genQueryOut = ( genQueryOut_t* ) voidInp;
    int i;

    if ( genQueryOut == NULL ) {
        return;
    }

    for ( i = 0; i < genQueryOut->attriCnt; i++ ) {
        if ( genQueryOut->sqlResult[i].value != NULL ) {
            free( genQueryOut->sqlResult[i].value );
        }
    }
    return;
}

void
clearBulkOprInp( void* voidInp ) {
    bulkOprInp_t *bulkOprInp = ( bulkOprInp_t* ) voidInp;
    if ( bulkOprInp == NULL ) {
        return;
    }
    clearGenQueryOut( &bulkOprInp->attriArray );
    clearKeyVal( &bulkOprInp->condInput );
    return;
}

sqlResult_t *
getSqlResultByInx( genQueryOut_t *genQueryOut, int attriInx ) {
    int i;

    if ( genQueryOut == NULL ) {
        return NULL;
    }

    for ( i = 0; i < genQueryOut->attriCnt; i++ ) {
        if ( genQueryOut->sqlResult[i].attriInx == attriInx ) {
            return &genQueryOut->sqlResult[i];
        }
    }
    return NULL;
}

void
clearModDataObjMetaInp( void* voidInp ) {
    modDataObjMeta_t *modDataObjMetaInp = ( modDataObjMeta_t* ) voidInp;
    if ( modDataObjMetaInp == NULL ) {
        return;
    }

    if ( modDataObjMetaInp->regParam != NULL ) {
        clearKeyVal( modDataObjMetaInp->regParam );
        free( modDataObjMetaInp->regParam );
    }

    if ( modDataObjMetaInp->dataObjInfo != NULL ) {
        freeDataObjInfo( modDataObjMetaInp->dataObjInfo );
    }

    return;
}

void
clearUnregDataObj( void* voidInp ) {
    unregDataObj_t *unregDataObjInp = ( unregDataObj_t* ) voidInp;
    if ( unregDataObjInp == NULL ) {
        return;
    }

    if ( unregDataObjInp->condInput != NULL ) {
        clearKeyVal( unregDataObjInp->condInput );
        free( unregDataObjInp->condInput );
    }

    if ( unregDataObjInp->dataObjInfo != NULL ) {
        freeDataObjInfo( unregDataObjInp->dataObjInfo );
    }

    return ;
}

void
clearRegReplicaInp( void* voidInp ) {
    regReplica_t *regReplicaInp = ( regReplica_t* ) voidInp;
    if ( regReplicaInp == NULL ) {
        return;
    }

    clearKeyVal( &regReplicaInp->condInput );

    if ( regReplicaInp->srcDataObjInfo != NULL ) {
        freeDataObjInfo( regReplicaInp->srcDataObjInfo );
    }

    if ( regReplicaInp->destDataObjInfo != NULL ) {
        freeDataObjInfo( regReplicaInp->destDataObjInfo );
    }

    memset( regReplicaInp, 0, sizeof( regReplica_t ) );

    return;
}

void
clearFileOpenInp( void* voidInp ) {
    fileOpenInp_t *fileOpenInp = ( fileOpenInp_t* ) voidInp;
    if ( fileOpenInp == NULL ) {
        return;
    }
    clearKeyVal( &fileOpenInp->condInput );
    memset( fileOpenInp, 0, sizeof( fileOpenInp_t ) );

    return;
}

void
clearDataObjInp( void* voidInp ) {
    dataObjInp_t *dataObjInp = ( dataObjInp_t* ) voidInp;
    if ( dataObjInp == NULL ) {
        return;
    }

    clearKeyVal( &dataObjInp->condInput );
    if ( dataObjInp->specColl != NULL ) {
        free( dataObjInp->specColl );
    }

    memset( dataObjInp, 0, sizeof( dataObjInp_t ) );

    return;
}

void
clearCollInp( void* voidInp ) {

    collInp_t *collInp = ( collInp_t* ) voidInp;
    if ( collInp == NULL ) {
        return;
    }

    clearKeyVal( &collInp->condInput );

    memset( collInp, 0, sizeof( collInp_t ) );

    return;
}

void
clearDataObjCopyInp( void* voidInp ) {
    dataObjCopyInp_t *dataObjCopyInp = ( dataObjCopyInp_t* ) voidInp;
    if ( dataObjCopyInp == NULL ) {
        return;
    }

    clearKeyVal( &dataObjCopyInp->destDataObjInp.condInput );
    clearKeyVal( &dataObjCopyInp->srcDataObjInp.condInput );

    if ( dataObjCopyInp->srcDataObjInp.specColl != NULL ) {
        free( dataObjCopyInp->srcDataObjInp.specColl );
    }

    memset( dataObjCopyInp, 0, sizeof( dataObjCopyInp_t ) );

    return;
}

/* Get the current time, in the  form: 2006-10-25-10.52.43 */
/*                                     0123456789012345678 */
extern char *tzname[2];

int
getLocalTimeStr( struct tm *mytm, char *timeStr ) {
    snprintf( timeStr, TIME_LEN, "%4d-%2d-%2d.%2d:%2d:%2d",
              mytm->tm_year + 1900, mytm->tm_mon + 1, mytm->tm_mday,
              mytm->tm_hour, mytm->tm_min, mytm->tm_sec );

    if ( timeStr[5] == ' ' ) {
        timeStr[5] = '0';
    }
    if ( timeStr[8] == ' ' ) {
        timeStr[8] = '0';
    }
    if ( timeStr[11] == ' ' ) {
        timeStr[11] = '0';
    }
    if ( timeStr[14] == ' ' ) {
        timeStr[14] = '0';
    }
    if ( timeStr[17] == ' ' ) {
        timeStr[17] = '0';
    }

    return 0;
}

int
resolveSpecCollType( char * type, char * collection, char * collInfo1,
                     char * collInfo2, specColl_t * specColl ) {
    int i;

    if ( specColl == NULL ) {
        return USER__NULL_INPUT_ERR;
    }

    if ( *type == '\0' ) {
        specColl->collClass = NO_SPEC_COLL;
        return SYS_UNMATCHED_SPEC_COLL_TYPE;
    }

    rstrcpy( specColl->collection, collection,
             MAX_NAME_LEN );

    if ( strcmp( type, MOUNT_POINT_STR ) == 0 ) {
        specColl->collClass = MOUNTED_COLL;
        rstrcpy( specColl->phyPath, collInfo1, MAX_NAME_LEN );

        irods::hierarchy_parser parse;
        parse.set_string( collInfo2 );

        std::string first_resc;
        parse.first_resc( first_resc );

        rstrcpy( specColl->resource, first_resc.c_str(), NAME_LEN );
        rstrcpy( specColl->rescHier, collInfo2, NAME_LEN );

        return 0;
    }
    else if ( strcmp( type, LINK_POINT_STR ) == 0 ) {
        specColl->collClass = LINKED_COLL;
        rstrcpy( specColl->phyPath, collInfo1, MAX_NAME_LEN );

        return 0;
    }
    else {
        for ( i = 0; i < NumStructFileType; i++ ) {
            if ( strcmp( type, StructFileTypeDef[i].typeName ) == 0 ) {
                specColl->collClass = STRUCT_FILE_COLL;
                specColl->type = StructFileTypeDef[i].type;
                rstrcpy( specColl->objPath, collInfo1,
                         MAX_NAME_LEN );
                parseCachedStructFileStr( collInfo2, specColl );
                return 0;
            }
        }

        specColl->collClass = NO_SPEC_COLL;
        rodsLog( LOG_ERROR,
                 "resolveSpecCollType: unmatch specColl type %s", type );
        return SYS_UNMATCHED_SPEC_COLL_TYPE;
    }
}

int
parseCachedStructFileStr( char * collInfo2, specColl_t * specColl ) {
    char *tmpPtr1, *tmpPtr2;
    int len;

    if ( collInfo2 == NULL || specColl == NULL ) {
        rodsLog( LOG_ERROR,
                 "parseCachedStructFileStr: NULL input" );
        return SYS_INTERNAL_NULL_INPUT_ERR;
    }

    if ( strlen( collInfo2 ) == 0 ) {
        /* empty */
        specColl->cacheDir[0] = specColl->resource[0] = '\0';
        return 0;
    }

    tmpPtr1 = strstr( collInfo2, ";;;" );

    if ( tmpPtr1 == NULL ) {
        rodsLog( LOG_NOTICE,
                 "parseCachedStructFileStr: collInfo2 %s format error 1", collInfo2 );
        return SYS_COLLINFO_2_FORMAT_ERR;
    }

    len = ( int )( tmpPtr1 - collInfo2 );
    strncpy( specColl->cacheDir, collInfo2, len );

    tmpPtr1 += 3;

    tmpPtr2 = strstr( tmpPtr1, ";;;" );

    if ( tmpPtr2 == NULL ) {
        rodsLog( LOG_NOTICE,
                 "parseCachedStructFileStr: collInfo2 %s format error 2", collInfo2 );
        return SYS_COLLINFO_2_FORMAT_ERR;
    }

    *tmpPtr2 = '\0';

    irods::hierarchy_parser parse;
    parse.set_string( tmpPtr1 );

    std::string first_resc;
    parse.first_resc( first_resc );

    snprintf( specColl->resource, sizeof( specColl->resource ),
              "%s", first_resc.c_str() );
    snprintf( specColl->rescHier, sizeof( specColl->rescHier ),
              "%s", tmpPtr1 );
    tmpPtr2 += 3;

    specColl->cacheDirty = atoi( tmpPtr2 );

    return 0;
}

int
getErrno( int irodError ) {
    int unixErrno = irodError % 1000;

    if ( unixErrno < 0 ) {
        unixErrno = -1 * unixErrno;
    }

    return unixErrno;
}

int
getIrodsErrno( int irodError ) {
    int irodsErrno = irodError / 1000 * 1000;
    return irodsErrno;
}


void
clearModAVUMetadataInp( void* voidInp ) {
    modAVUMetadataInp_t * modAVUMetadataInp = ( modAVUMetadataInp_t* )voidInp;
    free( modAVUMetadataInp->arg0 );
    free( modAVUMetadataInp->arg1 );
    free( modAVUMetadataInp->arg2 );
    free( modAVUMetadataInp->arg3 );
    free( modAVUMetadataInp->arg4 );
    free( modAVUMetadataInp->arg5 );
    free( modAVUMetadataInp->arg6 );
    free( modAVUMetadataInp->arg7 );
    free( modAVUMetadataInp->arg8 );
    free( modAVUMetadataInp->arg9 );
    memset( modAVUMetadataInp, 0, sizeof( modAVUMetadataInp_t ) );
    return;
}

/* freeRodsObjStat - free a rodsObjStat_t. Note that this should only
 * be used by the client because specColl also is freed which is cached
 * on the server
 */
int
freeRodsObjStat( rodsObjStat_t * rodsObjStat ) {
    if ( rodsObjStat == NULL ) {
        return 0;
    }

    if ( rodsObjStat->specColl != NULL ) {
        free( rodsObjStat->specColl );
    }

    free( rodsObjStat );

    return 0;
}

void
clearAuthResponseInp( void * inauthResponseInp ) {
    authResponseInp_t *authResponseInp;

    authResponseInp = ( authResponseInp_t * ) inauthResponseInp;

    if ( authResponseInp == NULL ) {
        return;
    }
    free( authResponseInp->username );
    free( authResponseInp->response );
    memset( authResponseInp, 0, sizeof( authResponseInp_t ) );

    return;
}


int
hasSymlinkInDir( const char * mydir ) {
    int status;
    char subfilePath[MAX_NAME_LEN];
    DIR *dirPtr;
    struct dirent *myDirent;
    struct stat statbuf;

    if ( mydir == NULL ) {
        return 0;
    }
    dirPtr = opendir( mydir );
    if ( dirPtr == NULL ) {
        return 0;
    }

    while ( ( myDirent = readdir( dirPtr ) ) != NULL ) {
        if ( strcmp( myDirent->d_name, "." ) == 0 ||
                strcmp( myDirent->d_name, ".." ) == 0 ) {
            continue;
        }
        snprintf( subfilePath, MAX_NAME_LEN, "%s/%s",
                  mydir, myDirent->d_name );
        status = lstat( subfilePath, &statbuf );
        if ( status != 0 ) {
            rodsLog( LOG_ERROR,
                     "hasSymlinkIndir: stat error for %s, errno = %d",
                     subfilePath, errno );
            continue;
        }
        if ( ( statbuf.st_mode & S_IFLNK ) == S_IFLNK ) {
            rodsLog( LOG_ERROR,
                     "hasSymlinkIndir: %s is a symlink",
                     subfilePath );
            closedir( dirPtr );
            return 1;
        }
        if ( ( statbuf.st_mode & S_IFDIR ) != 0 ) {
            if ( hasSymlinkInDir( subfilePath ) ) {
                closedir( dirPtr );
                return 1;
            }
        }
    }
    closedir( dirPtr );
    return 0;
}

static std::string stringify_addrinfo_hints(const struct addrinfo *_hints) {
    std::string ret;
    if (!_hints) {
        ret = "null hint pointer";
    } else {
        std::stringstream stream;
        stream << "ai_flags: [" << _hints->ai_flags << "] ai_family: [" << _hints->ai_family << "] ai_socktype: [" << _hints->ai_socktype << "] ai_protocol: [" << _hints->ai_protocol << "]";
        ret = stream.str();
    }
    return ret;
}

int
getaddrinfo_with_retry(const char *_node, const char *_service, const struct addrinfo *_hints, struct addrinfo **_res) {
    *_res = 0;
    const int max_retry = 300;
    for (int i=0; i<max_retry; ++i) {
        const int ret_getaddrinfo = getaddrinfo(_node, _service, _hints, _res);
        if (   ret_getaddrinfo == EAI_AGAIN
            || ret_getaddrinfo == EAI_NONAME
            || ret_getaddrinfo == EAI_NODATA) { // retryable errors

            struct timespec ts_requested;
            ts_requested.tv_sec = 0;
            ts_requested.tv_nsec = 100 * 1000 * 1000; // 100 milliseconds
            while (0 != nanosleep(&ts_requested, &ts_requested)) {
                const int errno_copy = errno;
                if (errno_copy != EINTR) {
                    rodsLog(LOG_ERROR, "getaddrinfo_with_retry: nanosleep error: errno [%d]", errno_copy);
                    return USER_RODS_HOSTNAME_ERR - errno_copy;
                }
            }
        } else if (ret_getaddrinfo != 0) { // non-retryable error
            if (ret_getaddrinfo == EAI_SYSTEM) {
                const int errno_copy = errno;
                std::string hint_str = stringify_addrinfo_hints(_hints);
                rodsLog(LOG_ERROR, "getaddrinfo_with_retry: getaddrinfo non-recoverable system error [%d] [%s] [%d] [%s] [%s]", ret_getaddrinfo, gai_strerror(ret_getaddrinfo), errno_copy, _node, hint_str.c_str());
            } else {
                std::string hint_str = stringify_addrinfo_hints(_hints);
                rodsLog(LOG_ERROR, "getaddrinfo_with_retry: getaddrinfo non-recoverable error [%d] [%s] [%s] [%s]", ret_getaddrinfo, gai_strerror(ret_getaddrinfo), _node, hint_str.c_str());
            }
            return USER_RODS_HOSTNAME_ERR;
        } else {
            return 0;
        }
        rodsLog(LOG_DEBUG, "getaddrinfo_with_retry retrying getaddrinfo. retry count [%d] hostname [%s]", i, _node);
    }
    std::string hint_str = stringify_addrinfo_hints(_hints);
    rodsLog(LOG_ERROR, "getaddrinfo_with_retry address resolution timeout [%s] [%s]", _node, hint_str.c_str());
    return USER_RODS_HOSTNAME_ERR;
}


int get_canonical_name(const char *_hostname, char* _buf, size_t _len) {
    struct addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    struct addrinfo *p_addrinfo;
    const int ret_getaddrinfo_with_retry = getaddrinfo_with_retry(_hostname, 0, &hint, &p_addrinfo);
    if (ret_getaddrinfo_with_retry ) {
        return ret_getaddrinfo_with_retry;
    }
    snprintf(_buf, _len, "%s", p_addrinfo->ai_canonname);
    freeaddrinfo(p_addrinfo);
    return 0;
}

int load_in_addr_from_hostname(const char* _hostname, struct in_addr* _out) {
    struct addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    struct addrinfo *p_addrinfo;
    const int ret_getaddrinfo_with_retry = getaddrinfo_with_retry(_hostname, 0, &hint, &p_addrinfo);
    if (ret_getaddrinfo_with_retry) {
        return ret_getaddrinfo_with_retry;
    }
    *_out = reinterpret_cast<struct sockaddr_in*>(p_addrinfo->ai_addr)->sin_addr;
    freeaddrinfo(p_addrinfo);
    return 0;
}


int
myWrite( int sock, void *buf, int len,
         int *bytesWritten ) {

    if ( bytesWritten ) {
        *bytesWritten = 0;
    }

    char *tmpPtr = ( char * ) buf;
    int toWrite = len;
    while ( toWrite > 0 ) {
        int nbytes;
#ifdef _WIN32
        if ( irodsDescType == SOCK_TYPE ) {
            nbytes = send( sock, tmpPtr, toWrite, 0 );
        }
        else {
            nbytes = write( sock, ( void * ) tmpPtr, toWrite );
        }
#else
        nbytes = write( sock, ( void * ) tmpPtr, toWrite );
#endif
        if ( nbytes <= 0 ) {
            if ( errno == EINTR ) {
                /* interrupted */
                errno = 0;
                nbytes = 0;
            }
            else {
                break;
            }
        }
        toWrite -= nbytes;
        tmpPtr += nbytes;
        if ( bytesWritten ) {
            *bytesWritten += nbytes;
        }
    }
    return len - toWrite;
}

int
myRead( int sock, void *buf, int len,
        int *bytesRead, struct timeval *tv ) {
    int nbytes;
    int toRead;
    char *tmpPtr;
    fd_set set;
    struct timeval timeout;
    int status;

    /* Initialize the file descriptor set. */
    FD_ZERO( &set );
    FD_SET( sock, &set );
    if ( tv != NULL ) {
        timeout = *tv;
    }

    toRead = len;
    tmpPtr = ( char * ) buf;

    if ( bytesRead != NULL ) {
        *bytesRead = 0;
    }

    while ( toRead > 0 ) {
#ifdef _WIN32
        if ( irodsDescType == SOCK_TYPE ) {
            nbytes = recv( sock, tmpPtr, toRead, 0 );
        }
        else {
            nbytes = read( sock, ( void * ) tmpPtr, toRead );
        }
#else
        if ( tv != NULL ) {
            status = select( sock + 1, &set, NULL, NULL, &timeout );
            if ( status == 0 ) {
                /* timedout */
                if ( len - toRead > 0 ) {
                    return len - toRead;
                }
                else {
                    return SYS_SOCK_READ_TIMEDOUT;
                }
            }
            else if ( status < 0 ) {
                if ( errno == EINTR ) {
                    continue;
                }
                else {
                    return SYS_SOCK_READ_ERR - errno;
                }
            }
        }
        nbytes = read( sock, ( void * ) tmpPtr, toRead );
#endif
        if ( nbytes <= 0 ) {
            if ( errno == EINTR ) {
                /* interrupted */
                errno = 0;
                nbytes = 0;
            }
            else {
                break;
            }
        }

        toRead -= nbytes;
        tmpPtr += nbytes;
        if ( bytesRead != NULL ) {
            *bytesRead += nbytes;
        }
    }
    return len - toRead;
}

/* addRErrorMsg - Add an error msg to the rError_t struct.
 *    rError_t *myError - the rError_t struct  for the error msg.
 *    int status - the input error status.
 *    char *msg - the error msg string. This string will be copied to myError.
 */
int
addRErrorMsg( rError_t *myError, int status, const char *msg ) {
    rErrMsg_t **newErrMsg;
    int newLen;
    int i;

    if ( myError == NULL ) {
        return SYS_INTERNAL_NULL_INPUT_ERR;
    }

    if ( ( myError->len % PTR_ARRAY_MALLOC_LEN ) == 0 ) {
        newLen = myError->len + PTR_ARRAY_MALLOC_LEN;
        newErrMsg = ( rErrMsg_t ** ) malloc( newLen * sizeof( *newErrMsg ) );
        memset( newErrMsg, 0, newLen * sizeof( *newErrMsg ) );
        for ( i = 0; i < myError->len; i++ ) {
            newErrMsg[i] = myError->errMsg[i];
        }
        if ( myError->errMsg != NULL ) {
            free( myError->errMsg );
        }
        myError->errMsg = newErrMsg;
    }

    myError->errMsg[myError->len] = ( rErrMsg_t* )malloc( sizeof( rErrMsg_t ) );
    strncpy( myError->errMsg[myError->len]->msg, msg, ERR_MSG_LEN - 1 );
    myError->errMsg[myError->len]->status = status;
    myError->len++;

    return 0;
}

int
addStrArray( strArray_t *strArray, char *value ) {
    char *newValue;
    int newLen;
    int i;
    int size;
    int myLen;

    if ( strArray == NULL ) {
        return SYS_INTERNAL_NULL_INPUT_ERR;
    }

    if ( strArray->size <= 0 ) {
        if ( strArray->len == 0 ) {
            /* default to NAME_LEN */

            strArray->size = NAME_LEN;
        }
        else {
            rodsLog( LOG_ERROR,
                     "addStrArray: invalid size %d, len %d",
                     strArray->size, strArray->len );
            return SYS_INTERNAL_NULL_INPUT_ERR;
        }
    }

    myLen = strlen( value );

    size = strArray->size;
    while ( size < myLen + 1 ) {
        size = size * 2;
    }


    /* XXXXXXX should be replaced by resizeStrArray after 2.3 release */
    if ( size != strArray->size ||
         ( strArray->len % PTR_ARRAY_MALLOC_LEN ) == 0 ) {
        int oldSize = strArray->size;
        /* have to redo it */
        strArray->size = size;
        newLen = strArray->len + PTR_ARRAY_MALLOC_LEN;
        newValue = ( char * ) malloc( newLen * size );
        memset( newValue, 0, newLen * size );
        for ( i = 0; i < strArray->len; i++ ) {
            rstrcpy( &newValue[i * size], &strArray->value[i * oldSize], size );
        }
        if ( strArray->value != NULL ) {
            free( strArray->value );
        }
        strArray->value = newValue;
    }

    rstrcpy( &strArray->value[strArray->len * size], value, size );
    strArray->len++;

    return 0;
}

int
splitMultiStr( char * strInput, strArray_t * strArray ) {
    char *startPtr, *endPtr;
    int endReached = 0;

    if ( strInput == NULL || strArray == NULL ) {
        return SYS_INTERNAL_NULL_INPUT_ERR;
    }

    startPtr = endPtr = strInput;

    while ( 1 ) {
        // two %% will be taken as an input % instead of as a delimiter
        while ( *endPtr != '%' && *endPtr != '\0' ) {
            endPtr ++;
        }
        if ( *endPtr == '%' ) {
            if ( *( endPtr + 1 ) == '%' ) {
                endPtr ++;
                endPtr ++;
                continue;
            }
            *endPtr = '\0';
        }
        else {
            endReached = 1;
        }

        char *str = strdup( startPtr );
        char *p = str;
        char *psrc = str;
        while ( *psrc != '\0' ) {
            while ( *psrc != '%' && *psrc != '\0' ) {
                *( p++ ) = *( psrc++ );
            }
            if ( *psrc == '%' ) {
                *( p++ ) = *( psrc++ );
                psrc++;
            }
        }
        *p = '\0';

        addStrArray( strArray, str );

        free( str );

        if ( endReached == 1 ) {
            break;
        }

        endPtr++;
        startPtr = endPtr;
    }

    return strArray->len;
}

int
parseMultiStr( char *strInput, strArray_t *strArray ) {
    char *startPtr, *endPtr;
    int endReached = 0;

    if ( strInput == NULL || strArray == NULL ) {
        return SYS_INTERNAL_NULL_INPUT_ERR;
    }

    startPtr = endPtr = strInput;

    while ( 1 ) {
        // two %% will be taken as an input instead of as a delimiter
        while ( *endPtr != '%' && *endPtr != '\0' ) {
            endPtr ++;
        }
        if ( *endPtr == '%' ) {
            if ( *( endPtr + 1 ) == '%' ) {
                endPtr ++;
                endPtr ++;
                continue;
            }
            *endPtr = '\0';
        }
        else {
            endReached = 1;
        }

        addStrArray( strArray, startPtr );

        if ( endReached == 1 ) {
            break;
        }

        endPtr++;
        startPtr = endPtr;
    }

    return strArray->len;
}

int mySetenvStr( const char * envname, const char * envval ) {
    int status;

#if defined(linux_platform)||defined(osx_platform)
    if ( envname == NULL || envval == NULL ) {
        return USER__NULL_INPUT_ERR;
    }
    status = setenv( envname, envval, 1 );
#else
    char *myBuf;
    int len;

    if ( envname == NULL || envval == NULL ) {
        return USER__NULL_INPUT_ERR;
    }
    len = strlen( envname ) + strlen( envval ) + 16;
    myBuf = ( char * )malloc( len );
    snprintf( myBuf, len, "%s=%s", envname, envval );
    status = putenv( myBuf );
    //      free( myBuf ); // JMC cppcheck - leak ==> backport 'fix' from comm trunk for solaris
#endif
    return status;
}
