/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

/* rcMisc.h - header file for rcMisc.c
 */

#ifndef RC_MISC_H__
#define RC_MISC_H__

#include "rods.h"
#include "rodsError.h"
#include "objInfo.h"
#include "rodsPath.h"
#include "bulkDataObjPut.h"
#include "bulkOprInfo.h"
#include <netinet/in.h>

#ifdef __cplusplus
extern "C" {
#endif

int
resizeStrArray( strArray_t *strArray, int newSize );
int
startsWith( const char * str, const char * prefix );
int
convertListToMultiString( char * strInput, int input );
char*
trimPrefix( char * str );
char*
trimSpaces( char * str ); 
int
getLine( FILE *fp, char *buf, int bufSize );
int
setForceFlagForRestart( bulkOprInp_t* bulkOprInp, bulkOprInfo_t* bulkOprInfo );
int
localToUnixTime( char *localTime, char *unixTime );
char*
getAttrNameFromAttrId( int cid );
int
getAttrIdFromAttrName( char *cname );
int
separateSelFuncFromAttr( char *t, char **aggOp, char **colNm );
int
getSelVal( char *c );
char*
getCondFromString( char * t );
int
isInteger(const char* inStr);
int
goodStrExpr(char* expr);
int
showAttrNames();
int
openRestartFile( char * restartFile, rodsRestart_t * rodsRestart );
int
getLineInBuf( char **inbuf, char * outbuf, int bufLen );
int
writeRestartFile( rodsRestart_t * rodsRestart, char * lastDonePath );
int
procAndWriteRestartFile( rodsRestart_t * rodsRestart, char * donePath );
int
setStateForRestart( rodsRestart_t * rodsRestart, rodsPath_t * targPath, rodsArguments_t * rodsArgs );
int
chkStateForResume( rcComm_t *conn, rodsRestart_t *rodsRestart,
                   char *targPath, rodsArguments_t *rodsArgs, objType_t objType,
                   keyValPair_t *condInput, int deleteFlag );
int
getPathStMode( const char* p );
int
initAttriArrayOfBulkOprInp( bulkOprInp_t *bulkOprInp );
int
fillAttriArrayOfBulkOprInp( char *objPath, int dataMode, char *inpChksum,
                            int offset, bulkOprInp_t *bulkOprInp );
int
getSpecCollTypeStr( specColl_t * specColl, char * outStr );
unsigned int
seedRandom();
void
getOffsetTimeStr( char *timeStr, const char *offSet );
int
fillGenQueryInpFromStrCond( char *str, genQueryInp_t *genQueryInp );
int
printGenQueryOut( FILE *fd, char *format, char *hint, genQueryOut_t *genQueryOut );
int
parseUserName( const char *fullUserNameIn, char *userName, char *userZone );
void
printReleaseInfo( char *cmdName );
int
printErrorStack( rError_t *rError );
void
getNowStr( char *timeStr );
int
getLocalTimeFromRodsTime( const char *timeStrIn, char *timeStrOut );
int
checkDateFormat( char *s );
void
clearModAVUMetadataInp( void* );
void
clearDataObjCopyInp( void* );
void
clearUnregDataObj( void* );
void
clearModDataObjMetaInp( void* );
void
clearRegReplicaInp( void *voidInp );
rodsLong_t
getFileSize( char *path );
int
freeBBuf( bytesBuf_t *myBBuf );
int
addRErrorMsg( rError_t *myError, int status, const char *msg );
int
clearBBuf( bytesBuf_t *myBBuf );
int
freeRError( rError_t *myError );
int
freeRErrorContent( rError_t *myError );
int
myHtonll( rodsLong_t inlonglong, rodsLong_t *outlonglong );
int
myNtohll( rodsLong_t inlonglong,  rodsLong_t *outlonglong );
int
getZoneNameFromHint( const char *rcatZoneHint, char *zoneName, int len );
int
freeDataObjInfo( dataObjInfo_t *dataObjInfo );
char *
getValByKey( const keyValPair_t *condInput, const char *keyWord );
int
replKeyVal( const keyValPair_t *srcCondInput, keyValPair_t *destCondInput );
int
replSpecColl( specColl_t *inSpecColl, specColl_t **outSpecColl );
int
addKeyVal( keyValPair_t *condInput, const char *keyWord, const char *value );
int
addInxIval( inxIvalPair_t *inxIvalPair, int inx, int value );
int
addInxVal( inxValPair_t *inxValPair, int inx, const char *value );
// JMC - backport 4590
int
clearKeyVal( keyValPair_t *condInput );
int
clearInxIval( inxIvalPair_t *inxIvalPair );
int
clearInxVal( inxValPair_t *inxValPair );
int
rmKeyVal( keyValPair_t *condInput, char *keyWord );
int
freeGenQueryOut( genQueryOut_t **genQueryOut );
void
clearGenQueryInp( void * voidInp );
sqlResult_t *
getSqlResultByInx( genQueryOut_t *genQueryOut, int attriInx );
void
clearGenQueryOut( void * );
void
clearBulkOprInp( void * );
int
getLocalTimeStr( struct tm *mytm, char *timeStr );
void
clearFileOpenInp( void* voidInp );
void
clearDataObjInp( void* );
void
clearCollInp( void* );
void
clearAuthResponseInp( void * myInStruct );
int
resolveSpecCollType( char *type, char *collection, char *collInfo1,
                     char *collInfo2, specColl_t *specColl );
int
getErrno( int errCode );
int
getIrodsErrno( int irodError );
int
printError( rcComm_t *Conn, int status, char *routineName );

int
parseCachedStructFileStr( char *collInfo2, specColl_t *specColl );
int
freeRodsObjStat( rodsObjStat_t *rodsObjStat );
int
splitMultiStr( char * strInput, strArray_t * strArray );
int
addStrArray( strArray_t *strArray, char *value );
int
parseMultiStr( char *strInput, strArray_t *strArray );
int
mySetenvStr( const char * envname, const char * envval );

int
hasSymlinkInDir( const char *mydir );

int myWrite( int sock, void *buf, int len, int *bytesWritten );
int myRead( int sock, void *buf, int len, int *bytesRead, struct timeval *tv );

// Special status that supresses reError header printing
static const int STDOUT_STATUS = 1000000;

int getaddrinfo_with_retry(const char *_node, const char *_service, const struct addrinfo *_hints, struct addrinfo **_res);
int get_canonical_name(const char *_hostname, char* _buf, size_t _len);
int load_in_addr_from_hostname(const char* _hostname, struct in_addr* _out);

#ifdef __cplusplus
}
#endif

#endif  // RC_MISC_H__
