/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* sslSockComm.c - SSL socket communication routines
 */

#include "rodsClient.h"
#include "sslSockComm.h"
#include "irods_client_server_negotiation.hpp"
#include "rcGlobalExtern.h"
#include "packStruct.h"

// =-=-=-=-=-=-=-
// work around for SSL Macro version issues
#if OPENSSL_VERSION_NUMBER < 0x10100000
    #define ASN1_STRING_get0_data ASN1_STRING_data
#endif

/* module internal functions */
static SSL_CTX *sslInit( char *certfile, char *keyfile );
static SSL *sslInitSocket( SSL_CTX *ctx, int sock );
static void sslLogError( char *msg );

static int sslVerifyCallback( int ok, X509_STORE_CTX *store );
static int sslPostConnectionCheck( SSL *ssl, char *peer );

int sslStart( rcComm_t *rcComm ) {
    int status;
    sslStartInp_t sslStartInp;

    if ( rcComm == NULL ) {
        return USER__NULL_INPUT_ERR;
    }

    if ( rcComm->ssl_on ) {
        /* SSL connection turned on ... return */
        return 0;
    }

    /* ask the server if we can start SSL */
    memset( &sslStartInp, 0, sizeof( sslStartInp ) );
    status = rcSslStart( rcComm, &sslStartInp );
    if ( status < 0 ) {
        rodsLogError( LOG_ERROR, status, "sslStart: server refused our request to start SSL" );
        return status;
    }

    /* we have the go-ahead ... set up SSL on our side of the socket */
    rcComm->ssl_ctx = sslInit( NULL, NULL );
    if ( rcComm->ssl_ctx == NULL ) {
        rodsLog( LOG_ERROR, "sslStart: couldn't initialize SSL context" );
        return SSL_INIT_ERROR;
    }

    rcComm->ssl = sslInitSocket( rcComm->ssl_ctx, rcComm->sock );
    if ( rcComm->ssl == NULL ) {
        rodsLog( LOG_ERROR, "sslStart: couldn't initialize SSL socket" );
        SSL_CTX_free( rcComm->ssl_ctx );
        rcComm->ssl_ctx = NULL;
        return SSL_INIT_ERROR;
    }

    status = SSL_connect( rcComm->ssl );
    if ( status < 1 ) {
        sslLogError( "sslStart: error in SSL_connect" );
        SSL_free( rcComm->ssl );
        rcComm->ssl = NULL;
        SSL_CTX_free( rcComm->ssl_ctx );
        rcComm->ssl_ctx = NULL;
        return SSL_HANDSHAKE_ERROR;
    }

    rcComm->ssl_on = 1;

    if ( !sslPostConnectionCheck( rcComm->ssl, rcComm->host ) ) {
        rodsLog( LOG_ERROR, "sslStart: post connection certificate check failed" );
        sslEnd( rcComm );
        return SSL_CERT_ERROR;
    }

    snprintf( rcComm->negotiation_results, sizeof( rcComm->negotiation_results ),
              "%s", irods::CS_NEG_USE_SSL.c_str() );
    return 0;
}

int sslEnd( rcComm_t *rcComm ) {
    int status;
    sslEndInp_t sslEndInp;

    if ( rcComm == NULL ) {
        return USER__NULL_INPUT_ERR;
    }

    if ( !rcComm->ssl_on ) {
        /* no SSL connection turned on ... return */
        return 0;
    }

    memset( &sslEndInp, 0, sizeof( sslEndInp ) );
    status = rcSslEnd( rcComm, &sslEndInp );
    if ( status < 0 ) {
        rodsLogError( LOG_ERROR, status, "sslEnd: server refused our request to stop SSL" );
        return status;
    }

    /* shut down the SSL connection. First SSL_shutdown() sends "close notify" */
    status = SSL_shutdown( rcComm->ssl );
    if ( status == 0 ) {
        /* do second phase of shutdown */
        status = SSL_shutdown( rcComm->ssl );
    }
    if ( status != 1 ) {
        sslLogError( "sslEnd: error shutting down the SSL connection" );
        return SSL_SHUTDOWN_ERROR;
    }

    /* clean up the SSL state */
    SSL_free( rcComm->ssl );
    rcComm->ssl = NULL;
    SSL_CTX_free( rcComm->ssl_ctx );
    rcComm->ssl_ctx = NULL;
    rcComm->ssl_on = 0;

    snprintf( rcComm->negotiation_results, sizeof( rcComm->negotiation_results ),
              "%s", irods::CS_NEG_USE_TCP.c_str() );
    rodsLog( LOG_DEBUG, "sslShutdown: shut down SSL connection" );


    return 0;
}

int sslWrite( void *buf, int len,
          int *bytesWritten, SSL *ssl ) {
    int nbytes;
    int toWrite;
    char *tmpPtr;

    toWrite = len;
    tmpPtr = ( char * ) buf;

    if ( bytesWritten != NULL ) {
        *bytesWritten = 0;
    }

    while ( toWrite > 0 ) {
        nbytes = SSL_write( ssl, ( void * ) tmpPtr, toWrite );
        if ( SSL_get_error( ssl, nbytes ) != SSL_ERROR_NONE ) {
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
        if ( bytesWritten != NULL ) {
            *bytesWritten += nbytes;
        }
    }
    return len - toWrite;
}

/* Module internal support functions */

static SSL_CTX* sslInit( char *certfile, char *keyfile ) {
    static int init_done = 0;

    rodsEnv env;
    int status = getRodsEnv( &env );
    if ( status < 0 ) {
        rodsLog(
            LOG_ERROR,
            "sslInit - failed in getRodsEnv : %d",
            status );
        return NULL;

    }

    if ( !init_done ) {
        SSL_library_init();
        SSL_load_error_strings();
        init_done = 1;
    }

    /* in our test programs we set up a null signal
       handler for SIGPIPE */
    /* signal(SIGPIPE, sslSigpipeHandler); */

    SSL_CTX* ctx = SSL_CTX_new( SSLv23_method() );

    SSL_CTX_set_options( ctx, SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1 | SSL_OP_SINGLE_DH_USE );

    /* load our keys and certificates if provided */
    if ( certfile ) {
        if ( SSL_CTX_use_certificate_chain_file( ctx, certfile ) != 1 ) {
            sslLogError( "sslInit: couldn't read certificate chain file" );
            SSL_CTX_free( ctx );
            return NULL;
        }
        else {
            if ( SSL_CTX_use_PrivateKey_file( ctx, keyfile, SSL_FILETYPE_PEM ) != 1 ) {
                sslLogError( "sslInit: couldn't read key file" );
                SSL_CTX_free( ctx );
                return NULL;
            }
        }
    }

    /* set up CA paths and files here */
    const char *ca_path = strcmp( env.irodsSSLCACertificatePath, "" ) ? env.irodsSSLCACertificatePath : NULL;
    const char *ca_file = strcmp( env.irodsSSLCACertificateFile, "" ) ? env.irodsSSLCACertificateFile : NULL;
    if ( ca_path || ca_file ) {
        if ( SSL_CTX_load_verify_locations( ctx, ca_file, ca_path ) != 1 ) {
            sslLogError( "sslInit: error loading CA certificate locations" );
        }
    }
    if ( SSL_CTX_set_default_verify_paths( ctx ) != 1 ) {
        sslLogError( "sslInit: error loading default CA certificate locations" );
    }

    /* Set up the default certificate verification */
    /* if "none" is specified, we won't stop the SSL handshake
       due to certificate error, but will log messages from
       the verification callback */
    const char* verify_server = env.irodsSSLVerifyServer;
    if ( verify_server && !strcmp( verify_server, "none" ) ) {
        SSL_CTX_set_verify( ctx, SSL_VERIFY_NONE, sslVerifyCallback );
    }
    else {
        SSL_CTX_set_verify( ctx, SSL_VERIFY_PEER, sslVerifyCallback );
    }
    /* default depth is nine ... leave this here in case it needs modification */
    SSL_CTX_set_verify_depth( ctx, 9 );

    /* ciphers */
    if ( SSL_CTX_set_cipher_list( ctx, SSL_CIPHER_LIST ) != 1 ) {
        sslLogError( "sslInit: couldn't set the cipher list (no valid ciphers)" );
        SSL_CTX_free( ctx );
        return NULL;
    }

    return ctx;
}

static SSL* sslInitSocket( SSL_CTX *ctx, int sock ) {
    SSL *ssl;
    BIO *bio;

    bio = BIO_new_socket( sock, BIO_NOCLOSE );
    if ( bio == NULL ) {
        sslLogError( "sslInitSocket: BIO allocation error" );
        return NULL;
    }
    ssl = SSL_new( ctx );
    if ( ssl == NULL ) {
        sslLogError( "sslInitSocket: couldn't create a new SSL socket" );
        BIO_free( bio );
        return NULL;
    }
    SSL_set_bio( ssl, bio, bio );

    return ssl;
}

static void sslLogError( char *msg ) {
    unsigned long err;
    char buf[512];

    while ( ( err = ERR_get_error() ) ) {
        ERR_error_string_n( err, buf, 512 );
        rodsLog( LOG_ERROR, "%s. SSL error: %s", msg, buf );
    }
}

static int sslVerifyCallback( int ok, X509_STORE_CTX *store ) {
    char data[256];

    /* log any verification problems, even if we'll still accept the cert */
    if ( !ok ) {
        auto *cert = X509_STORE_CTX_get_current_cert( store );
        int  depth = X509_STORE_CTX_get_error_depth( store );
        int  err = X509_STORE_CTX_get_error( store );

        rodsLog( LOG_NOTICE, "sslVerifyCallback: problem with certificate at depth: %i", depth );
        X509_NAME_oneline( X509_get_issuer_name( cert ), data, 256 );
        rodsLog( LOG_NOTICE, "sslVerifyCallback:   issuer = %s", data );
        X509_NAME_oneline( X509_get_subject_name( cert ), data, 256 );
        rodsLog( LOG_NOTICE, "sslVerifyCallback:   subject = %s", data );
        rodsLog( LOG_NOTICE, "sslVerifyCallback:   err %i:%s", err,
                 X509_verify_cert_error_string( err ) );
    }

    return ok;
}

static int sslPostConnectionCheck( SSL *ssl, char *peer ) {
    rodsEnv env;
    int status = getRodsEnv( &env );
    if ( status < 0 ) {
        rodsLog(
            LOG_ERROR,
            "sslPostConnectionCheck - failed in getRodsEnv : %d",
            status );
        return status;

    }

    int match = 0;
    char cn[256];

    const char* verify_server = env.irodsSSLVerifyServer;
    if ( verify_server && strcmp( verify_server, "hostname" ) ) {
        /* not being asked to verify that the peer hostname
           is in the certificate. */
        return 1;
    }

    auto* cert = SSL_get_peer_certificate( ssl );
    if ( cert == NULL ) {
        /* no certificate presented */
        return 0;
    }

    if ( peer == NULL ) {
        /* no hostname passed to verify */
        X509_free( cert );
        return 0;
    }

    /* check if the peer name matches any of the subjectAltNames
       listed in the certificate */
    auto names = static_cast<STACK_OF(GENERAL_NAME)*>(X509_get_ext_d2i( cert, NID_subject_alt_name, NULL, NULL ));
    std::size_t num_names = sk_GENERAL_NAME_num( names );
    for ( std::size_t i = 0; i < num_names; i++ ) {
        auto* name = sk_GENERAL_NAME_value( names, i );
        if ( name->type == GEN_DNS ) {
            if ( !strcasecmp( reinterpret_cast<const char*>(ASN1_STRING_get0_data( name->d.dNSName )), peer ) ) {
                match = 1;
                break;
            }
        }
    }
    sk_GENERAL_NAME_free( names );

    /* if no match above, check the common name in the certificate */
    if ( !match &&
            ( X509_NAME_get_text_by_NID( X509_get_subject_name( cert ),
                                         NID_commonName, cn, 256 ) != -1 ) ) {
        cn[255] = 0;
        if ( !strcasecmp( cn, peer ) ) {
            match = 1;
        }
        else if ( cn[0] == '*' ) { /* wildcard domain */
            char *tmp = strchr( peer, '.' );
            if ( tmp && !strcasecmp( tmp, cn + 1 ) ) {
                match = 1;
            }
        }
    }

    X509_free( cert );

    if ( match ) {
        return 1;
    }
    else {
        return 0;
    }
}
