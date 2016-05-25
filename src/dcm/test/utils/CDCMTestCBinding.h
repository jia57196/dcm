#if !defined(maa_cdcmtestcbinding)
#define maa_dcmtestutil_h

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

// External includes
#include <inttypes.h>

// Internal includes
#include "dcmCBinding.h"

// Forward declarations
namespace NDCM
{
	class CDCM;
    class IDCMKey;
    class IDCMValue;
}

namespace xcp
{
    class CEnrichmentInfo;
}

class IXmlNode;

namespace maa_dcmtestcbinding {

/**
 * CDCMTestUtil implementation
 */
class CDCMTestCBinding
{

public:

    /**
     * Constructor
     */
    CDCMTestCBinding();

    /**
     * Destructor
     */
    virtual ~CDCMTestCBinding();

    /**
     * Main function
     * @param argc number of args
     * @param argv arguments
     */
    int main(int argc, char* argv[]);
    
    /**
     * Prints the usage
     */
    void usage();
    
    /**
     * Write the configure file 
     *  
     * @param a_insert - number of inserts into tables 
     * 
     * @return int32_t = -1 is error, otherwise success
     */
    int32_t writeConfigFile(const uint32_t a_inserts);

    bool initDCM();
    
    /**
     * Test insert function 
     *  
     * @param a_count - number of entries to insert 
     *  
     * @return true if success
     */
    bool testInsert(const uint32_t a_count);
    
    /**
     * Test find function 
     *  
     * @param a_count - number of entries to search 
     * @param a_compare - compare search results 
     *  
     * @return true if success
     */
    bool testFind(const uint32_t a_count, const bool a_compare);
    
    /**
     * Test clean up function 
     *  
     * @return true if success
     */
    bool cleanUp();
    
private:

    /**
     * Create an DB keys (TEID + IP)
     * 
     * @return bool 
     */
    bool createKeys();

    /**
     * Create an enrichment information object
     * 
     * @return bool 
     */
    bool createEnrichmentInfo();

    /**
     * Reset object contents
     */
    void resetContents();

private:

    /** DCM Instance */
    xcp::tDCM* mp_dcmInstance;

    /** Enrichment information */
    xcp::tEnrichmentInfo* mp_testEnrichmentInfo;
    xcp::tEnrichmentInfo* mp_findEnrichmentInfo;

    /** DB index keys */
    xcp::tTeidIpKey* mp_testKey;
    xcp::tTeidIpKey* mp_findKey1;
    xcp::tTeidIpKey* mp_findKey2;
};

} // namespace maa_dcmtestcbinding

#endif // maa_cdcmtestcbinding
