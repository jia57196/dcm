#ifndef Common_h
#define Common_h

#include <stdint.h>

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{

    //
    // Constant Variables
    //
    const uint32_t IMSI_LEN    = 8;
    const uint32_t IMEI_LEN    = 8;
    const uint32_t MSISDN_LEN  = 8;
    const uint32_t CGI_LEN     = 8;
    const uint32_t APNIDX_LEN  = 4;
    const uint32_t QCI_LEN     = 1;
    const uint32_t MAX_APN_LEN = 100;
    const uint32_t IPV6_ADDR_LEN = 16;
    const uint32_t UINT32_LEN = sizeof ( uint32_t );

    //
    // hash algorithm for 32bit integer
    //
    uint32_t hash32Shift(uint32_t a);

    //
    // hash algorithm for 64bit integer
    //
    uint64_t hash64Shift(uint64_t a);

    // 
    // hash for string
    //
    uint32_t hashString(const char* ap_str);

    //
    // hash for octets
    //
    uint32_t hashOctets(const uint8_t* ap_oct, const uint32_t a_len);

};
#endif //DCM_COMMON_H
