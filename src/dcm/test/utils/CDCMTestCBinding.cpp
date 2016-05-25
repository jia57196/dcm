#include "CDCMTestCBinding.h"

/**
 * DCM CBinding Test Utility
 *
 * Author:       Mark Phaneuf 
 * Date Created: 4th April 2014 
 *
 * (C) COPYRIGHT JDS UNIPHASE 2014. ALL RIGHTS RESERVED. NO PART
 * OF THIS PROGRAM MAY BE PHOTOCOPIED REPRODUCED OR TRANSLATED
 * TO ANOTHER PROGRAM LANGUAGE WITHOUT THE PRIOR WRITTEN CONSENT
 * OF JDS UNIPHASE.
 */

#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <string>
#include <iostream>

// Globals
std::string sp_configFile = "dcmCBindingConfig.xml";
std::string sp_dbFile     = "dcmCBinding.db";

// Namespace usages

using namespace maa_dcmtestcbinding;

// Forward declarations

void usage();

//----------------------------------------------------------------------------//

int main(int argc, char* argv[])
{
    uint32_t l_numFinds(0);
    uint32_t l_numInserts(0);
    bool l_compareResults(false);

    //
    // Parse args
    //
  
    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);

        if ("-c" == arg)
        {
            l_compareResults = true;
        }
        else if ("-f" == arg)
        {
            ++i;
            if (i == argc)
            {
                std::cout << "ERROR: bad argument" << std::endl;
                usage();
                exit(-1);
            }
            l_numFinds = atoi(argv[i]);
        }
        else if ("-i" == arg)
        {            ++i;
            if (i == argc)
            {
                std::cout << "ERROR: bad argument" << std::endl;
                usage();
                exit(-1);
            }
            l_numInserts = atoi(argv[i]);
        }
    }
    
    std::cout << "INFO: Inserts: " << l_numInserts << std::endl;
    std::cout << "INFO: Searches: " << l_numFinds << std::endl;
    std::cout << "INFO: Compare: ";
    if (true == l_compareResults)
    {
        std::cout << "true" << std::endl;
    }
    else
    {
        std::cout << "false" << std::endl;
    }

    //
    // Initialize and configure DCM
    //
    
    CDCMTestCBinding testUtil;
    
    if (0 != testUtil.writeConfigFile(l_numInserts))
    {
        std::cout << "ERROR: cannot write configuration file" << std::endl;
        return -1;
    }

    if (false == testUtil.initDCM())
    {
        std::cout << "ERROR: initializing DCM failed" << std::endl;
        return 1;
    }
    
    //
    // Insert into DCM
    //
    
    if (false == testUtil.testInsert(l_numInserts))
    {
        std::cout << "FAILED: testInsert() failed" << std::endl;
        return 1;
    }

    if (false == testUtil.testFind(l_numFinds, l_compareResults))
    {
        std::cout << "FAILED: testFind() failed" << std::endl;
        return 1;
    }
        
    //
    // Clean up
    //
    
    if (! testUtil.cleanUp())
    {
        std::cout << "FAILED: cleanUp() failed" << std::endl;
        return 1;
    }
    
    return 0;
}

//----------------------------------------------------------------------------//

void usage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "    -c enables comparison of search results" << std::endl;
    std::cout << "    -f <find count>" << std::endl;
    std::cout << "    -i <insert count>" << std::endl;
}

//----------------------------------------------------------------------------//

namespace maa_dcmtestcbinding
{

CDCMTestCBinding::CDCMTestCBinding()
{
    resetContents();
}

CDCMTestCBinding::~CDCMTestCBinding()
{
    cleanUp();

    // Remove files
    unlink(sp_dbFile.c_str());
    unlink(sp_configFile.c_str());
}

//----------------------------------------------------------------------------//

int CDCMTestCBinding::writeConfigFile(const uint32_t a_inserts)
{
    int l_return(0);

    FILE *lp_file = fopen(sp_configFile.c_str(), "w");

    const uint32_t l_recordSize(512);      // Size of item inserted into DB + overhead
    const uint32_t l_minFileSize(1000000); // minimum DB size
    uint32_t l_fileSize = a_inserts * l_recordSize;
    l_fileSize = (l_fileSize > l_minFileSize) ? l_fileSize : l_minFileSize;

    if ( NULL == lp_file)
    {
        printf("Error opening file!\n");
        l_return = -1;
    }
    else
    {
        /* print some text */
        fprintf( lp_file, "<dcm>\n");
        fprintf( lp_file, "     <file_size>%u</file_size>\n", l_fileSize);
        fprintf( lp_file, "     <file_location>%s</file_location>\n", sp_dbFile.c_str());
        fprintf( lp_file, "     <session-table-size>%u</session-table-size>\n", a_inserts);
        fprintf( lp_file, "     <subscriber-table-size>%u</subscriber-table-size>\n", a_inserts);
        fprintf( lp_file, "</dcm>\n");

        fclose( lp_file);
    }

    return l_return;
}

//----------------------------------------------------------------------------//

bool CDCMTestCBinding::initDCM()
{
    bool l_return(true);

    std::cout << "**************************************" << std::endl;
    std::cout << "INFO: Creating DCM Storage            " << std::endl;
    std::cout << "**************************************" << std::endl;

    if (0 != (mp_dcmInstance = xcp::createDCMInstance()))
    {
        if (0 == xcp::initializeDCMObject(mp_dcmInstance, sp_configFile.c_str()))
        {
            std::cout << "INFO: DCM storage created successfully" << std::endl;

            if (true == createKeys())
            {
                if (true == createEnrichmentInfo())
                {
                    // Success
                }
                else
                {
                    std::cout << "ERROR: Failed to enrichment info" << std::endl;
                    l_return = false;
                }
            }
            else
            {
                std::cout << "ERROR: Failed to create keys" << std::endl;
                l_return = false;
            }
        }
        else
        {
            std::cout << "ERROR: Failed to initialize DCM storage" << std::endl;
            l_return = false;
        }
    }
    else
    {
        std::cout << "ERROR: Failed to create DCM instance" << std::endl;
        l_return = false;
    }


    return l_return;
}

//----------------------------------------------------------------------------//

bool CDCMTestCBinding::testInsert(const uint32_t a_count)
{
    bool l_return         = true;
    struct timeval l_time = {0, 0};
    uint32_t l_teid(0);
    uint32_t l_ip(0);

    // pick a random seed
    gettimeofday(&l_time, 0);
    srandom(l_time.tv_sec);

    for (uint32_t i = 0; i < a_count; i++)
    {
        l_teid = random();
        l_ip   = random();

        // Save the keys from the first two inserts for later searches
        if (0 == i)
        {
            xcp::setTeidIPAddressv4(mp_findKey1, l_ip, l_teid);
        }
        else if (1 == i)
        {
            xcp::setTeidIPAddressv4(mp_findKey2, l_ip, l_teid);
        }

        // For the address inserts
        if (0 != xcp::setTeidIPAddressv4(mp_testKey, l_ip, l_teid))
        {
            l_return = false;
            std::cout << "ERROR: failed to set teid/ip" << std::endl;
            break;
        }
        else
        {
            uint32_t l_u32(0);
            int32_t l_return(0);

            if (0 != (l_return = xcp::getTeid(mp_testKey, &l_u32)))
            {
                l_return = false;
                std::cout << "ERROR: unable to read TEID" << std::endl;
            }

            if (0 != (l_return = xcp::getIPAddressv4(mp_testKey, &l_u32)))
            {
                l_return = false;
                std::cout << "ERROR: unable to read IPv4 address" << std::endl;
            }

            if (0 != xcp::insertKeyIntoDCM(mp_dcmInstance, mp_testKey, mp_testEnrichmentInfo))
            {
                l_return = false;
                std::cout << "ERROR: failed to insert into db" << std::endl;
                break;
            }
        }
    }

    return l_return;
}

//----------------------------------------------------------------------------//

bool CDCMTestCBinding::testFind(const uint32_t a_count, const bool a_compare)
{
    bool l_return               = true;
    uint32_t l_found            = 0;
    xcp::tTeidIpKey* lp_findKey = 0;
    
    // Get all of the expected values
    uint64_t l_expectedImsi             = 0;
    uint64_t l_expectedImei             = 0;
    uint64_t l_expectedCgi              = 0;
    uint64_t l_expectedMsisdn           = 0;
    uint8_t  l_expectedQci              = 0;
    char     l_expectedApn[255]         = "\0";
    uint32_t l_expectedLength           = 0;
    struct timespec l_expectedTimestamp = { 0, 0 };

    if (true == a_compare)
    {
        xcp::getEnrichmentTimestampValue(mp_testEnrichmentInfo, ENRICHMENT_INFO_TIMESTAMP_TAG, &l_expectedTimestamp);
        xcp::getEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_IMSI_TAG, &l_expectedImsi);
        xcp::getEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_IMEI_TAG, &l_expectedImei);
        xcp::getEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_MSISDN_TAG, &l_expectedMsisdn);
        xcp::getEnrichmentUint8Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_QCI_TAG, &l_expectedQci);
        xcp::getEnrichmentOctetValue(mp_testEnrichmentInfo, ENRICHMENT_INFO_APN_TAG,
                                     reinterpret_cast<uint8_t*>(l_expectedApn), &l_expectedLength);
        xcp::getEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_CGI_TAG, &l_expectedCgi);
    }

    for (uint32_t i = 0; i < a_count; i++)
    {
        if (0 == (i % 2))
        {
            lp_findKey = mp_findKey1;
        }
        else
        {
            lp_findKey = mp_findKey2;
        }

        if (0 != xcp::findEnrichmentInfoInDCM(mp_dcmInstance, lp_findKey, &mp_findEnrichmentInfo))
        {
            l_return = false;
            std::cout << "ERROR: failed to locate key in db" << std::endl;
            break;
        }
        else
        {
            ++l_found;

            if (true == a_compare)
            {
                uint64_t l_imsi     = 0;
                uint64_t l_imei     = 0;
                uint64_t l_cgi      = 0;
                uint64_t l_msisdn   = 0;
                uint8_t  l_qci      = 0;
                char     l_apn[255] = "\0";
                uint32_t l_apnLen   = 0;
                struct timespec l_timestamp = { 0, 0 };

                xcp::getEnrichmentTimestampValue(mp_testEnrichmentInfo, ENRICHMENT_INFO_TIMESTAMP_TAG, &l_timestamp);
                xcp::getEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_IMSI_TAG, &l_imsi);
                xcp::getEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_IMEI_TAG, &l_imei);
                xcp::getEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_MSISDN_TAG, &l_msisdn);
                xcp::getEnrichmentUint8Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_QCI_TAG, &l_qci);
                xcp::getEnrichmentOctetValue(mp_testEnrichmentInfo, ENRICHMENT_INFO_APN_TAG,
                                             reinterpret_cast<uint8_t*>(l_apn), &l_apnLen);
                xcp::getEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_CGI_TAG, &l_cgi);

                if (l_imsi != l_expectedImsi)
                {
                    l_return = false;
                    std::cout << "ERROR: IMSI difference\n" << std::endl;
                }

                if (l_imei != l_expectedImei)
                {
                    l_return = false;
                    std::cout << "ERROR: IMEI difference\n" << std::endl;
                }

                if (l_cgi != l_expectedCgi)
                {
                    l_return = false;
                    std::cout << "ERROR: CGI difference\n" << std::endl;
                }

                if (l_msisdn != l_expectedMsisdn)
                {
                    l_return = false;
                    std::cout << "ERROR: MSISDN difference\n" << std::endl;
                }

                if (l_qci != l_expectedQci)
                {
                    l_return = false;
                    std::cout << "ERROR: QCI difference\n" << std::endl;
                }

                if (0 != strncmp(l_apn, l_expectedApn, sizeof(l_apn)))
                {
                    l_return = false;
                    std::cout << "ERROR: APN difference\n" << std::endl;
                }
            }
        }
    }

    std::cout << "INFO: Find count: " << l_found << std::endl;

    return l_return;
}

//----------------------------------------------------------------------------//


bool CDCMTestCBinding::cleanUp()
{
    std::cout << "INFO: cleanUp" << std::endl;

    if (0 != mp_dcmInstance)
    {
        xcp::destroyDCMObject(mp_dcmInstance);
        mp_dcmInstance = 0;
    }

    if (0 != mp_testEnrichmentInfo)
    {
        xcp::destroyEnrichmentInfo(mp_testEnrichmentInfo);
        mp_testEnrichmentInfo = 0;
    }

    if (0 != mp_findEnrichmentInfo)
    {
        xcp::destroyEnrichmentInfo(mp_findEnrichmentInfo);
        mp_findEnrichmentInfo = 0;
    }

    if (0 != mp_testKey)
    {
        xcp::destroyTeidIPKey(mp_testKey);
        mp_testKey = 0;
    }

    if (0 != mp_findKey1)
    {
        xcp::destroyTeidIPKey(mp_findKey1);
        mp_findKey1 = 0;
    }

    if (0 != mp_findKey2)
    {
        xcp::destroyTeidIPKey(mp_findKey2);
        mp_findKey2 = 0;
    }

    return true;
}

//----------------------------------------------------------------------------//
bool CDCMTestCBinding::createKeys()
{
    mp_testKey  = xcp::createTeidIPKeyObject();
    mp_findKey1 = xcp::createTeidIPKeyObject();
    mp_findKey2 = xcp::createTeidIPKeyObject();

    return ((0 != mp_testKey) && (0 != mp_findKey1) && (0 != mp_findKey2));
}

//----------------------------------------------------------------------------//
bool CDCMTestCBinding::createEnrichmentInfo()
{
    uint64_t l_imsi     = 0x9876876576546543;
    uint64_t l_imei     = 0x1234567898765432;
    uint64_t l_cgi      = 0x1111222233334444;
    uint64_t l_msisdn   = 0x9999999999999999;
    uint8_t  l_qci      = 0x0f;
    char     l_apn[255] = "apn.net\0";
    uint32_t l_apnLen   = strlen ( l_apn);
    struct timespec l_timestamp = { 0x123, 0x123456 };

    if (0 != (mp_testEnrichmentInfo = (xcp::tEnrichmentInfo*)xcp::createEnrichmentInfo()))
    {
        xcp::setEnrichmentTimestampValue(mp_testEnrichmentInfo, ENRICHMENT_INFO_TIMESTAMP_TAG, &l_timestamp);
        xcp::setEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_IMSI_TAG, l_imsi);
        xcp::setEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_IMEI_TAG, l_imei);
        xcp::setEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_MSISDN_TAG, l_msisdn);
        xcp::setEnrichmentUint8Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_QCI_TAG, l_qci);
        xcp::setEnrichmentOctetValue(mp_testEnrichmentInfo, ENRICHMENT_INFO_APN_TAG, reinterpret_cast<uint8_t*>(l_apn), l_apnLen);
        xcp::setEnrichmentUint64Value(mp_testEnrichmentInfo, ENRICHMENT_INFO_CGI_TAG, l_cgi);
    }
    else
    {
        std::cout << "ERROR: unable to create enrichment info" << std::endl;
    }

    if (0 != (mp_findEnrichmentInfo = (xcp::tEnrichmentInfo*)xcp::createEnrichmentInfo()))
    {
        // Success - used for output
    }
    else
    {
        std::cout << "ERROR: unable to create enrichment info" << std::endl;
    }

    return ((0 != mp_testEnrichmentInfo) && (0 != mp_findEnrichmentInfo));
}

//----------------------------------------------------------------------------//

void CDCMTestCBinding::resetContents()
{
    mp_dcmInstance        = 0;
    mp_testEnrichmentInfo = 0;
    mp_findEnrichmentInfo = 0;
    mp_testKey            = 0;
    mp_findKey1           = 0;
    mp_findKey2           = 0;
}

} // namespace maa_cdcmtestcbinding
