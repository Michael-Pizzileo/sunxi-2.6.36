/** @file csr_tls.h
 *
 * Security Manager TLS support functions.
 *
 * @section LEGAL
 *   CONFIDENTIAL
 *
 *   Copyright (C) Cambridge Silicon Radio Ltd 2008. All rights reserved.
 *
 *   Refer to LICENSE.txt included with this source for details on the
 *   license terms.
 *
 * @section DESCRIPTION
 *   TLS support functions.
 *
 ****************************************************************************
 *
 * @section REVISION
 *   $Id: //depot/dot11/v7.0p/host/lib_security/plugins/tls/csr_tls.h#1 $
 *
 ****************************************************************************/
#ifndef SECURITY_TLSSUP_H
#define SECURITY_TLSSUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_rc4/csr_rc4.h"
#include "csr_aes/csr_aes.h"
#include "csr_md5/csr_md5.h"
#include "csr_sha1/csr_sha1.h"
#include "abstractions/osa.h"
#include "plugins/security_method.h"
#include "csr_security_private.h"

#define MAC_LENGTH CSR_SHA1_DIGEST_LENGTH

/* Bit definitions for the flags field  */
#define TLS_STARTBIT     0x20
#define TLS_MOREBIT      0x40
#define TLS_LENGTHBIT    0x80

#define EAPTLS_HEADER_LENGTH (1)
#define EAPTLS_MESSAGE_LENGTH_LENGTH (4)
#define TLS_MAX_MESSAGE_LENGTH 8192
#define TLS_MAX_SESSION_ID_LENGTH 32
#define TLS_MASTER_SECRET_LENGTH 48

#define EAP_TLS_START_BIT_IS_SET(eapReqData) ((((eaptls_packet*)(&eapReqData.u.req.u.data))->flags & TLS_STARTBIT) ? TRUE:FALSE)

/* Cypher suites */
#define TLS_RSA_WITH_RC4_128_SHA 0x0005
#define TLS_RSA_WITH_3DES_EDE_CBC_SHA 0x000a
#define TLS_RSA_WITH_AES_128_CBC_SHA 0x002f
#define TLS_DH_anon_WITH_AES_128_CBC_SHA 0x0034

/* The protocol version is a constant (version 3.1)*/
#define TLS_PROTOCOL_VERSION_MAJOR 3
#define TLS_PROTOCOL_VERSION_MINOR 1

#define TLS_CONTENT_TYPE_HANDSHAKE          22
#define TLS_CONTENT_TYPE_APPLICATION_DATA   23

#define RANDOM_CHALLENGE_BYTE_SIZE 28
typedef struct Random
{
    CsrUint32 gmt_unix_time; /* from time() in time.h */
    CsrUint8 data[RANDOM_CHALLENGE_BYTE_SIZE];
} Random;

typedef struct phase1_key_block
{
    CsrUint8 client_write_MAC_secret[MAC_LENGTH];
    CsrUint8 server_write_MAC_secret[MAC_LENGTH];
    CsrUint8 client_write_key[16];
    CsrUint8 server_write_key[16];
    CsrUint8 session_key_seed[40];
} phase1_key_block;

typedef struct key_block
{
    CsrUint8 client_write_MAC_secret[20];
    CsrUint8 server_write_MAC_secret[20];
    CsrUint8 client_write_key[24];
    CsrUint8 server_write_key[24];
    CsrUint8 client_write_IV[16];
    CsrUint8 server_write_IV[16];
    CsrUint8 session_key_seed[40];
} key_block;

typedef struct tls_data
{
    CsrUint8 dh_g;           /* Diffie-Hellman base, read from the server hello. */
    Random client_random; /* Client-generate random challenge put in the client hello. */
    Random server_random; /* Server-generate random challenge read from the server hello. */
    CsrUint8 server_prime[256];          /* DH prime, read from the server hello. */
    CsrUint8 server_dh_public_key[256];  /* Server public key, read from the server hello. */
    CsrUint8 client_dh_public_key[256];  /* Client public key. Calculated by us. */
    CsrUint8 client_dh_private_key[256]; /* Client private key. Generated by us. */
    CsrUint16 pending_cypher_suite;    /* Awaiting a change cypher spec message */
    CsrUint16 cypher_suite;            /* cypher currently in action */

    CsrUint32 session_flags;
    CsrUint8 session_id[TLS_MAX_SESSION_ID_LENGTH];
    CsrUint32 session_id_length;

    CsrUint8 master_secret[TLS_MASTER_SECRET_LENGTH]; /* Used to generate the key block and for MIC calculation */
    key_block keys;          /* Contains current keying data */

    /* Ongoing dynamic keying data.
     * This is carried over from one operation to the next */
    CsrRc4Key rc4_server_write_key;
    CsrRc4Key rc4_client_write_key;

    /* Read and write sequence numbers, as per RFC: 2246 */
    /* Need to be used in big-endian form */
    CsrUint8 read_seq_no[8];
    CsrUint8 write_seq_no[8];

    CsrUint8 gtc_reqId;

    CsrUint8 nonce[32]; /* Nonce sent from server in a crypto binding message. */

    CsrUint8 crypto_binding_tlv[60]; /* Needs to be kept for compound MAC calculation. */

    CsrUint8 imck1[60]; /* Inner Method Compound Key */

    /* Keys used by TLS */
    CsrUint8  PAC_key[32];

    /* Server RSA Public Key parameters */
    CsrUint8 server_modulus[257];
    CsrUint32 server_modulus_length;
    CsrUint8 server_exponent[3];
    CsrUint32 server_exponent_length;

    /* Hash contexts for maintaining running hash over handshake messages */
    CSR_CRYPTO_MD5_CTX *md5_ctx;
    CSR_CRYPTO_SHA1_CTX *sha1_ctx;
} tls_data;

typedef enum tls_state
{
    TLS_AWAIT_START,               /* awaiting EAP-TLS start from server */
    TLS_AWAIT_SERVER_HELLO,
    TLS_AWAIT_SERVER_FINISHED,
    TLS_AWAIT_SUCCESS
} tls_state;

/* State communicated to inner methods */
typedef enum tls_phase1_state
{
    TLS_CONTINUES,             /* More TLS message exchanges to take place */
    TLS_DONE,                  /* TLS done and awaiting success */
    TLS_SESSION_RESUMPTION     /* TLS session resumed, awaiting send packet and rx success, key ready to be generated */
} tls_phase1_state;

/* This is the control structure for the TLS state machine. */
typedef struct TlsContext
{
    CsrWifiSecurityContext* context;
    tls_state state;

    /* All the generic TLS parameters go in here. */
    tls_data tls;

    /* Hold our eventual session key. */
    eapKey key;

    CsrUint8 tls_pkt_flags;
    CsrUint32 tls_parse_flags;

    CsrUint8 tls_message[TLS_MAX_MESSAGE_LENGTH];
    CsrUint32 tls_message_length;

    DataBuffer dataref;

    CsrUint8 eapType;

    /* Challenge material used by inner authentication methods such as MSCHAPv2 in case of TTLS */
    CsrUint8 outputChallenge[17];

} TlsContext;

typedef struct eaptls_packet
{
    /* (EAP header omitted) */

    CsrUint8 flags;
    union
    {
        struct lengthed2
        {
            CsrUint8 length[4];
            CsrUint8 data[1];    /* variable length data */
        } lengthed;
        CsrUint8 data[1];    /* variable length data */
    }u;
} eaptls_packet;

/* Flags for update_flags field of parsing functions. */
#define DH_G_MODIFIED                              0x00000001
#define SERVER_RANDOM_MODIFIED                     0x00000002
#define SERVER_PRIME_MODIFIED                      0x00000004
#define SERVER_DH_PUBLIC_KEY_MODIFIED              0x00000008
#define CYPHER_SUITE_MODIFIED                      0x00000010
#define CHANGE_CYPHER_SPEC_INCLUDED                0x00000020
#define ENCAPSULATED_ID_REQ_INCLUDED               0x00000040
#define ENCAPSULATED_MSCHAPv2_CHALLENGE_INCLUDED   0x00000080
#define ENCAPSULATED_MSCHAPV2_SUCCESS_INCLUDED     0x00000100
#define ENCAPSULATED_INTERMEDIATE_RESULT_INCLUDED  0x00000200
#define ENCAPSULATED_CRYPTO_BINDING_NONCE_INCLUDED 0x00000400
#define PAC_KEY_INCLUDED                           0x00000800
#define PAC_OPAQUE_INCLUDED                        0x00001000
#define PAC_AID_INCLUDED                           0x00002000
#define ENCAPSULATED_RESULT_INCLUDED               0x00004000
#define SERVER_DONE                                0x00008000

/* Flags for session ID information */
#define SESSION_ID_MATCH                           0x00000001
#define SESSION_ID_UPDATED                         0x00000002

/**
 * @brief The TLS callback for all application-specific data.
 *
 * @par Description
 *      The TLS routines only parse and process TLS standard data. All
 *      application-specific data is passed to a callback function for
 *      processing.
 *      This is the callback type definition.
 *
 * @return
 *      TRUE = processed ok. FALSE = error.
 *
 * @param[in/out]       data : pointer to the start of the (decyphered)
 *                             application data. The callback function should
 *                             advance this to the first byte after the
 *                             application data on return.
 * @param[in]           size : size of the data buffer following the data
 *                             parameter. Should be at least as big as the
 *                             actual application data. May be bigger due to
 *                             padding.
 * @param[out]  update_flags : flags to indicate which fields of tls have been
 *                             modified and what content has been discovered in
 *                             the packet.
 * @param[in]           tls  : the tls context structure,
 */
typedef CsrBool (*parse_tls_callback)(void *clientContext, CsrUint8 **data, CsrUint16 size, CsrUint32 *update_flags, tls_data *tls);

/**
 * @brief The PRF (pseudo-randome function) as defined by RFC: 2246
 *
 * @par Description
 *      The PRF is used for calculating master secrets from pre-master secrets.
 *      The output (master secret) can be as long as you like. The PRF will
 *      generate enough data to fill it.
 *
 * @param[in]   secret : the pre-master shared secret
 * @param[in]   secret_len : the length of the pre-master shared secret in octets
 * @param[in]   label : ascii string
 * @param[in]   label_len : length of the ascii string
 * @param[in]   seed : the PRF seed
 * @param[in]   seed_len : length of the PRF seed in octets
 * @param[out]  answer : buffer for containing the answer
 * @param[in]   answer_len : the size the output buffer in octets
 */
void prf(const CsrUint8 *secret, const CsrUint32 secret_len,
         const char *label, const CsrUint32 label_len,
         const CsrUint8 *seed, const CsrUint32 seed_len,
         CsrUint8 *answer, const CsrUint32 answer_len);

/**
 * @brief This is the T-PRF function
 *
 * @par Description
 *      The T-PRF function is simpler than the TLS PRF and is used
 *      during EAP-FAST. Briefly:
 *              T-PRF (Key, S, OutputLength) = T1 + T2 + T3 + T4 + ...
 *              Where:
 *                      S = label + 0x00 + seed; and
 *                      T1 = HMAC-SHA1 (Key, S + OutputLength + 0x01)
 *                      T2 = HMAC-SHA1 (Key, T1 + S + OutputLength + 0x02)
 *                      T3 = HMAC-SHA1 (Key, T2 + S + OutputLength + 0x03)
 *                      etc.
 *                      OutputLength is 2 big-endian octets.
 *                      '+' indicates concatenation.
 *
 * @param[in]   key : the pre-master shared secret
 * @param[in]   key_len : the length of the pre-master shared secret in octets
 * @param[in]   label : ascii string
 * @param[in]   label_len : length of the ascii string
 * @param[in]   seed : the PRF seed
 * @param[in]   seed_len : length of the PRF seed in octets
 * @param[out]  output : buffer for containing the output
 * @param[in]   output_len : the size the output buffer in octets
 */
void t_prf(const CsrUint8 *key, const CsrUint32 key_len,
           const char *label, const CsrUint32 label_len,
           const CsrUint8 *seed, const CsrUint32 seed_len,
           CsrUint8 *output, const CsrUint32 output_len);

/**
 * @brief TLS key expansion from master secret.
 *
 * @param[in]      tls : the tls_data context structure
 *
 * @return
 *      void
 *
 * @post
 *      The supplied "tls" structure will have its relevant
 *      key elements updated.
 */
void tls_key_expansion(tls_data *tls);

/**
 * @brief Parse an incoming TLS record packet and fill out a tls_data structure
 *        with whatever it contains.
 *
 * @param[in]   packet : the incoming TLS packet
 * @param[in]   length : the length of the TLS packet
 * @param[in]      tls : the tls_data context structure which we may update
 * @param[out]  update_flags : flags to indicate which fields of tls have been
 *                             modified and what content has been discovered in
 *                             the packet.
 * @param[in] callback : callback function for protocol-specific data. This
 *                       may be NULL, indicating that no protocol-specific
 *                       data should be expected in the packet.
 *
 * @return
 *      TRUE = parsed ok. FALSE = error.
 *
 * @post
 *      On success, the supplied "tls" structure will have its relevent
 *      elements updated. The others will be unchanged. On failure, the
 *      contents of the "tls" structure should not be trusted.
 */
CsrBool parse_tls (void *clientContext,
                   CsrUint8 *packet,
                   CsrUint32 length,
                   tls_data *tls,
                   CsrUint32 *update_flags,
                   parse_tls_callback callback);

void inc_seq_no(CsrUint8 *seq_no);

/* Function used to set the appropriate EAP type TLS, TTLS, PEAP, FAST etc */
void TlsSetEapType(eapMethod* context, CsrUint8 eapType);

void BuildTlsApplicationData(eapMethod* method, CsrUint8 *data, CsrUint16 length, CsrUint8 *output, CsrUint32 *outLength);

/* Function to return implicit challenge and keying material as required by TTLS etc */
extern void TlsGetPrf(eapMethod* method, const char *label, CsrUint8 *outputChallenge, CsrUint16 outputLength);

extern tls_phase1_state TlsPhase1State(eapMethod* method);

extern void install_tls_session(TlsContext* tlsContext);

#ifdef __cplusplus
}
#endif

#endif /*SECURITY_TLSSUP_H*/

