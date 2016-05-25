#if !defined(maa_dcmtestutil_h)
#define maa_dcmtestutil_h

/**
 * DCM Test Utility
 *
 * Author:       Jamie Dickey
 * Date Created: 26th March 2014
 *
 * (C) COPYRIGHT JDS UNIPHASE 2014. ALL RIGHTS RESERVED. NO PART
 * OF THIS PROGRAM MAY BE PHOTOCOPIED REPRODUCED OR TRANSLATED
 * TO ANOTHER PROGRAM LANGUAGE WITHOUT THE PRIOR WRITTEN CONSENT
 * OF JDS UNIPHASE.
 */

#include <iostream>
#include <vector>

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

namespace maa_dcmtestutils {

/**
 * CDCMTestUtil implementation
 */
class CDCMTestUtil
{

public:
  
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
     * Init DCM
     * @param a_configFile the DCM config file
     * @param a_dcmType the DCM type
     * @param a_containerType the DCM container type
     * @return true if initialization of DCM was successful
     */
    bool initDCM(std::string a_configFile,
                 std::string a_dcmType,
                 std::string a_containerType);
    
    /**
     * Read the test data file
     * @param a_testDataFile the test data file to read
     * @return true if reading data was success
     */
    bool readTestData(std::string a_testDataFile);
    
    /**
     * Test insert function
     * @return true if success
     */
    bool testInsert();
    
    /**
     * Test find function
     * @return true if success
     */
    bool testFind();
    
    /**
     * Test clean up function
     * @return true if success
     */
    bool cleanUp();
    
private:
    
    /**
     * Extracts key values and inserts into provided containers
     * @param ap_node the node from which to start
     * @param a_numItems the number of items to insert
     * @param arp_keyContainer the key container to insert to
     * @param arp_populatedValueContainer the value container to insert to with
     *        populated values
     * @param arp_emptyValueContainer the value container to insert to with
     *        empty values, i.e. an empty value of the correct type (optional)
     * @return true if the extraction and insert was successful
     */
    bool extractKeyValues(IXmlNode* ap_node,
                          uint64_t a_numItems,
                          NDCM::IDCMKey**& arp_keyContainer,
                          NDCM::IDCMValue**& arp_populatedValueContainer,
                          NDCM::IDCMValue** arp_emptyValueContainer=0);
    
    /**
     * Extracts the ImsiKey and populates the number of items into the key container
     * @param ap_node the XML node
     * @param a_numItems the number of items to populate
     * @param arp_keyContainer the container to insert items
     */
    void extractImsiKey(IXmlNode* ap_node,
                        uint64_t a_numItems,
                        NDCM::IDCMKey**& arp_keyContainer);
    
    /**
     * Extracts the TeidIpAddressKey and populates the number of items into the key container
     * @param ap_node the XML node
     * @param a_numItems the number of items to populate
     * @param arp_keyContainer the container to insert items
     */
    void extractTeidIpAddressKey(IXmlNode* ap_node,
                                 uint64_t a_numItems,
                                 NDCM::IDCMKey**& arp_keyContainer);
    
    /**
     * Extracts the SubscriberInfoValue and populates the number of items into the value container
     * @param ap_node the XML node
     * @param a_numItems the number of items to populate
     * @param arp_valueContainer the container to insert items
     */
    void extractSubscriberInfoValue(IXmlNode* ap_node,
                                    uint64_t a_numItems,
                                    NDCM::IDCMValue**& arp_valueContainer);
                                    
    /**
     * Extracts the SessionInfoValue and populates the number of items into the value container
     * @param ap_node the XML node
     * @param a_numItems the number of items to populate
     * @param arp_valueContainer the container to insert items
     */
    void extractSessionInfoValue(IXmlNode* ap_node,
                                 uint64_t a_numItems,
                                 NDCM::IDCMValue**& arp_valueContainer);
                                    
    /**
     * Extracts the EnrichmentInfoValue and populates the number of items into the value container
     * @param ap_node the XML node
     * @param a_numItems the number of items to populate
     * @param arp_valueContainer the container to insert items
     */
    void extractEnrichmentInfoValue(IXmlNode* ap_node,
                                    uint64_t a_numItems,
                                    xcp::CEnrichmentInfo**& arp_valueContainer);
    /**
     * Get the child node value
     * @param ap_parentNode the parent node
     * @param a_childNodeName the child node name
     * @return string the child node value
     */
    std::string getChildNodeValue(IXmlNode* ap_parentNode, std::string a_childNodeName);
    
    /**
     * Get the attribute uint64 value
     * @param ap_node the Xml node
     * @param a_attributeName the attribute name
     * @param ar_val the value to populate
     * @return bool true if success
     */
    bool getAttributeUint64Value(IXmlNode* ap_node, std::string a_attributeName, uint64_t& ar_val);
    
    /**
     * The DCM instance
     */
    NDCM::CDCM* mp_dcm;
    
    /**
     * The number of inserts
     */
    uint64_t m_numInserts;
    
    /**
     * The number of finds
     */
    uint64_t m_numFinds;
    
    /**
     * The DCM keys to insert
     */
    std::vector<NDCM::IDCMKey**> mp_insertKeys;
    
    /**
     * The DCM values to insert
     */
    std::vector<NDCM::IDCMValue**> mp_insertValues;
    
    /**
     * The DCM keys to find
     */
    NDCM::IDCMKey** mp_findKeys;
    
    /**
     * The expected DCM values to be found
     */
    NDCM::IDCMValue** mp_expectedFindValues;
    
    /**
     * The DCM values to pass to find function
     */
    NDCM::IDCMValue** mp_findValues;
    
    /**
     * The expected DCM enrichment values to be found
     */
    xcp::CEnrichmentInfo** mp_expectedEnrichmentFindValues;
    
    /**
     * The DCM enrichment values to pass to find function
     */
    xcp::CEnrichmentInfo** mp_enrichmentFindValues;
    
    
};

} // namespace maa_dcmtestutils

#endif 
