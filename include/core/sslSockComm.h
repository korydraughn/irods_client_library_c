/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* sslSockComm.h - header file for sslSockComm.c
 */

#ifndef SSL_SOCK_COMM_H__
#define SSL_SOCK_COMM_H__

#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>

#include "rodsDef.h"
#include "rcConnect.h"
#include "rodsPackInstruct.h"

#define SSL_CIPHER_LIST "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"

#ifdef __cplusplus
extern "C" {
#endif

int sslStart( rcComm_t *rcComm );
int sslEnd( rcComm_t *rcComm );
int sslWrite( void *buf, int len, int *bytesWritten, SSL *ssl );

#ifdef __cplusplus
}
#endif

#endif	/* SSL_SOCK_COMM_H__ */
