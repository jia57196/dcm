#include "CDCMTestUtil.h"

/**
 * Concrete class for CDCMTestUtil
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

// DCM includes

#include <CDCM.h>
#include <CDCMUserPlaneEnrichment.h>
#include <IDCMContainer.h>
#include <CDCMHashMapContainer.h>
#include <CDCMMapContainer.h>
#include <CDCMMultiTableContainer.h>
#include <CImsiKey.h>
#include <CTeidIpAddressKey.h>
#include <CSubscriberInfo.h>
#include <CSessionInfo.h>
#include <CEnrichmentInfo.h>

// XML includes

#include <CXmlParserFactory.h>
#include <CXmlNode.h>
#include <CConfigSupport.h>

// Namespace usages

using namespace std;
using namespace maa_dcmtestutils;
using namespace NDCM;
using namespace xcp;

// Forward declarations

void usage();

//----------------------------------------------------------------------------//

int main(int argc, char* argv[])
{
    //
    // Parse args
    //
  
    std::string l_configFile = "";
    std::string l_containerType = "";
    std::string l_dcmType = "";
    std::string l_testDataFile = "";
    bool l_read = false;
    bool l_write = false;
    
    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        
        if ("-c" == arg)
        {
            l_containerType = argv[++i];
        }
        else if ("-d" == arg)
        {
            l_dcmType = argv[++i];
        }
        else if ("-f" == arg)
        {
            l_configFile = argv[++i];
        }
        else if ("-r" == arg)
        {
            l_read = true;
        }
        else if ("-w" == arg)
        {
            l_write = true;
        }
        else if ("-t" == arg)
        {
            l_testDataFile = argv[++i];
        }
        
    }
    
    cout << "INFO: Config file = " << l_configFile << endl;
    cout << "INFO: Container Type = " << l_containerType << endl;
    cout << "INFO: DCM Type = " << l_dcmType << endl;
    cout << "INFO: Read = " << l_read << endl;
    cout << "INFO: Write = " << l_write << endl;
    cout << "INFO: Test Data file = " << l_testDataFile << endl;
    
    if ("" == l_configFile ||
        "" == l_containerType ||
        "" == l_dcmType ||
        "" == l_testDataFile ||
        (!l_read && !l_write))
    {
        cout << "ERROR: invalid parameters specified" << endl;
        usage();
        return 1;
    }
    
    //
    // Initialize and configure DCM
    //
    
    CDCMTestUtil testUtil;
    
    if (! testUtil.initDCM(l_configFile, l_dcmType, l_containerType))
    {
        cout << "ERROR: initializing DCM failed" << endl;
        return 1;
    }
    
    //
    // Read test data file
    //
    if (! testUtil.readTestData(l_testDataFile))
    {
        cout << "ERROR: reading test data failed" << endl;
        return 1;
    }
    
    //
    // Insert into DCM
    //
    
    if (l_write)
    {
        if (! testUtil.testInsert())
        {
            cout << "FAILED: testInsert() failed" << endl;
            return 1;
        }
    }
    
    //
    // Find values in DCM
    //
    
    if (l_read)
    {
        if (! testUtil.testFind())
        {
            cout << "FAILED: testFind() failed" << endl;
            
            unlink("/tmp/dcm.db");
            return 1;
        }
        
        // Remove file
        
        unlink("/tmp/dcm.db");
    }
    
    //
    // Clean up
    //
    
    if (! testUtil.cleanUp())
    {
        cout << "FAILED: cleanUp() failed" << endl;
        return 1;
    }
    
    return 0;
}

//----------------------------------------------------------------------------//

void usage()
{
    cout << "Usage:" << endl;
    cout << "    -w &| -r utility writes to DCM and/or reads from DCM" << endl;
    cout << "    -f <config file>" << endl;
    cout << "    -t <test data file>" << endl;
    cout << "    -d <DCM type> where DCM type is one of:" << endl;
    cout << "        d = CDCM" << endl;
    cout << "        u = CDCMUserPlaneEnrichment" << endl;
    cout << "    -c <container type> where container type is one of:" << endl;
    cout << "        h = CDCMHashMapContainer" << endl;
    cout << "        m = CDCMMapContainer" << endl;
    cout << "        t = CDCMMultiTableContainer" << endl;
}

//----------------------------------------------------------------------------//

namespace maa_dcmtestutils
{


//----------------------------------------------------------------------------//


bool CDCMTestUtil::initDCM(std::string a_configFile,
                           std::string a_dcmType,
                           std::string a_containerType)
{
    bool l_ret = true;
    
    //
    // Get DCM container type
    //
    
    IDCMContainer* lp_container;
    
    if (0 == a_containerType.compare("h"))
    {
        lp_container = new CDCMHashMapContainer;
    }
    else if (0 == a_containerType.compare("m"))
    {
        lp_container = new CDCMMapContainer;
    }
    else if (0 == a_containerType.compare("t"))
    {
        lp_container = new CDCMMultiTableContainer;
    }
    else
    {
        cout << "ERROR: invalid container type - " << a_containerType << endl;
        l_ret = false;
    }
    
    //
    // Get DCM type
    //
    
    if (l_ret)
    {	
        if (0 == a_dcmType.compare("d"))
        {
            cout << "INFO: creating CDCM" << endl;
            mp_dcm = new CDCM;
        }
        else if (0 == a_dcmType.compare("u"))
        {
            cout << "INFO: creating CDCMUserPlaneEnrichment" << endl;
            mp_dcm = new CDCMUserPlaneEnrichment;
        }
        else
        {
            cout << "ERROR: invalid DCM type - " << a_dcmType << endl;
            l_ret = false;
        }
    }
    
    //
    // Initialize DCM
    //
    
    if (l_ret)
    {
        if (mp_dcm->initialize(lp_container))
        {
            cout << "INFO: DCM init success" << endl;
        }
        else
        {
            cout << "INFO: DCM init fail" << endl;
            l_ret = false;
        }
    }
    
    //
    // Configure DCM
    //
    
    if (l_ret)
    {
        if (mp_dcm->configure(a_configFile.c_str()))
        {
            cout << "INFO: DCM configure success" << endl;
        }
        else
        {
            cout << "INFO: DCM configure fail" << endl;
            l_ret = false;
        }
    }
    
    return l_ret;
}
  

//----------------------------------------------------------------------------//

bool CDCMTestUtil::readTestData(std::string a_testDataFile)
{
    bool l_ret = true;
    
    //
    // Init insert and find containers
    //
    
    mp_findKeys = 0;
    mp_expectedFindValues = 0;
    mp_findValues = 0;
    mp_expectedEnrichmentFindValues = 0;
    mp_enrichmentFindValues = 0;
    
    IXmlNode* lp_root = 0;
    
    if ( CXmlParserFactory::XML_SUCCESS == CXmlParserFactory::parseToSimpleRepresentation ( lp_root, 0, a_testDataFile.c_str() ) &&
         0 == strcasecmp( lp_root->xmlTagStr().c_str(), "test-data" ) )
    {
        if ( 0 == lp_root )
        {
            l_ret = false;
        }
        
        //
        // Extract inserts and finds node
        //
        
        if (l_ret)
        {
            uint32_t l_numParentElements = static_cast <CXmlNode*> (lp_root)->numElements();
            
            uint32_t l_insertInstance = 0;
            
            for (uint32_t l_parentNodeIndex = 0;
                 l_parentNodeIndex < l_numParentElements;
                 l_parentNodeIndex++)
            {
                IXmlNode* lp_currentNode = static_cast <CXmlNode*> (lp_root)->element(l_parentNodeIndex);
                
                if ( 0 == strcasecmp(lp_currentNode->xmlTagStr().c_str(), "insert") )
                {
                    cout << "***********************************" << endl;
                    cout << "INFO: extracting insert values" << endl;
                    cout << "***********************************" << endl;
                    
                    if (getAttributeUint64Value(lp_currentNode, "numItems", m_numInserts))
                    {
                        IDCMKey** lp_keys = new IDCMKey*[m_numInserts];
                        mp_insertKeys.push_back(lp_keys);
                        
                        IDCMValue** lp_values = new IDCMValue*[m_numInserts];
                        mp_insertValues.push_back(lp_values);
                        
                        extractKeyValues(lp_currentNode, m_numInserts, mp_insertKeys[l_insertInstance], mp_insertValues[l_insertInstance], 0);
                        
                        l_insertInstance++;
                    }
                    else
                    {
                        cout << "ERROR: didn't find numItems attribute" << endl;
                        l_ret = false;
                    }
                }
                if ( 0 == strcasecmp(lp_currentNode->xmlTagStr().c_str(), "find") )
                {
                    cout << "***********************************" << endl;
                    cout << "INFO: extracting find values" << endl;
                    cout << "***********************************" << endl;
                    
                    if (getAttributeUint64Value(lp_currentNode, "numItems", m_numFinds))
                    {
                        mp_findKeys = new IDCMKey*[m_numFinds];
                        mp_expectedFindValues = new IDCMValue*[m_numFinds];
                        mp_findValues = new IDCMValue*[m_numFinds];
                    
                        extractKeyValues(lp_currentNode,
                                         m_numFinds,
                                         mp_findKeys,
                                         mp_expectedFindValues,
                                         mp_findValues);
                    }
                    else
                    {
                        cout << "ERROR: didn't find numItems attribute" << endl;
                        l_ret = false;
                    }
                }
            }
        }
        
    }
    
    return true;
}

           
//----------------------------------------------------------------------------//

bool CDCMTestUtil::getAttributeUint64Value(IXmlNode* ap_node,
                                           std::string a_attributeName,
                                           uint64_t& ar_val)
{
    bool ret_val = false;

    ret_val = CConfigSupport::uint64FromXmlAttribute(ap_node,
                                                     a_attributeName,
                                                     ar_val);

    cout << "INFO: get uint64 attribute " << a_attributeName << " = " << ar_val << endl;
    
    return ret_val;
}
                    
//----------------------------------------------------------------------------//

bool CDCMTestUtil::extractKeyValues(IXmlNode* ap_node,
                                    uint64_t a_numItems,
                                    IDCMKey**& arp_keyContainer,
                                    IDCMValue**& arp_populatedValueContainer,
                                    IDCMValue** arp_emptyValueContainer)
{
    uint32_t l_numElements = static_cast <CXmlNode*> (ap_node)->numElements();
    
    for (uint32_t l_nodeIndex = 0; l_nodeIndex < l_numElements; l_nodeIndex++)
    {
        IXmlNode* lp_node = static_cast <CXmlNode*> (ap_node)->element(l_nodeIndex);
        
        cout << "INFO: node name = " << lp_node->xmlTagStr() << endl;
        
        if ( 0 == strcasecmp(lp_node->xmlTagStr().c_str(), "ImsiKey") )
        {
            extractImsiKey(lp_node, a_numItems, arp_keyContainer);
        }
        else if ( 0 == strcasecmp(lp_node->xmlTagStr().c_str(), "TeidIpAddressKey") )
        {
            extractTeidIpAddressKey(lp_node, a_numItems, arp_keyContainer);
        }
        else if ( 0 == strcasecmp(lp_node->xmlTagStr().c_str(), "SubscriberInfoValue") )
        {
            extractSubscriberInfoValue(lp_node, a_numItems, arp_populatedValueContainer);
            
            //
            // Add new items of correct type to the value container
            // so find is called with correct argument.
            //

            for (uint64_t i = 0; i < a_numItems; i++)
            {
                if (0 != arp_emptyValueContainer)
                {
                    IDCMValue* lp_findValue = new CSubscriberInfo;
                
                    arp_emptyValueContainer[i] = lp_findValue;
                }
            }
        }
        else if ( 0 == strcasecmp(lp_node->xmlTagStr().c_str(), "SessionInfoValue") )
        {
            extractSessionInfoValue(lp_node, a_numItems, arp_populatedValueContainer);
            
            //
            // Add new items of correct type to the value container
            // so find is called with correct argument.
            //

            for (uint64_t i = 0; i < a_numItems; i++)
            {
                if (0 != arp_emptyValueContainer)
                {
                    IDCMValue* lp_findValue = new CSessionInfo;
                
                    arp_emptyValueContainer[i] = lp_findValue;
                }
            }
        }
        else if ( 0 == strcasecmp(lp_node->xmlTagStr().c_str(), "EnrichmentInfoValue") )
        {
            mp_enrichmentFindValues = new CEnrichmentInfo*[a_numItems];
            mp_expectedEnrichmentFindValues = new CEnrichmentInfo*[a_numItems];
            
            extractEnrichmentInfoValue(lp_node, a_numItems, mp_expectedEnrichmentFindValues);
            
            //
            // Add new items of correct type to the value container
            // so find is called with correct argument.
            //
            
            for (uint64_t i = 0; i < a_numItems; i++)
            {
                CEnrichmentInfo* lp_findValue = new CEnrichmentInfo;
                
                mp_enrichmentFindValues[i] = lp_findValue;
            }
        }
    }


    return true;
}

//----------------------------------------------------------------------------//

void CDCMTestUtil::extractImsiKey(IXmlNode* ap_node,
                                  uint64_t a_numItems,
                                  IDCMKey**& arp_keyContainer)
{
    std::string l_imsiStr = getChildNodeValue(ap_node, "imsi-start-value");
    
    cout << "INFO: node value = " << l_imsiStr << endl;
    
    for (uint64_t i = 0; i < a_numItems; i++)
    {
        IDCMKey* lp_key = new CImsiKey;
        
        uint64_t l_imsi = atoi(l_imsiStr.c_str()) + i;
        
        ((CImsiKey*)lp_key)->setImsi(l_imsi);
        
        arp_keyContainer[i] = lp_key;
    }
}

//----------------------------------------------------------------------------//

void CDCMTestUtil::extractTeidIpAddressKey(IXmlNode* ap_node,
                                           uint64_t a_numItems,
                                           IDCMKey**& arp_keyContainer)
{
    std::string l_teidStr = getChildNodeValue(ap_node, "teid-start-value");
    
    cout << "INFO: node value = " << l_teidStr << endl;
    
    std::string l_ipStr = getChildNodeValue(ap_node, "ip-start-value");
    
    cout << "INFO: node value = " << l_ipStr << endl;
    
    for (uint64_t i = 0; i < a_numItems; i++)
    {
        IDCMKey* lp_key = new CTeidIpAddressKey;

        uint64_t l_teid = atoi(l_teidStr.c_str()) + i;
        uint32_t l_ip = atoi(l_ipStr.c_str()) + i;
        
        ((CTeidIpAddressKey*)lp_key)->setTeid(l_teid);
        ((CTeidIpAddressKey*)lp_key)->setIp(l_ip);
        
        arp_keyContainer[i] = lp_key;
    }
}


//----------------------------------------------------------------------------//

void CDCMTestUtil::extractSubscriberInfoValue(IXmlNode* ap_node,
                                              uint64_t a_numItems,
                                              IDCMValue**& arp_valueContainer)
{
    std::string l_imeiStr = getChildNodeValue(ap_node, "imei-start-value");
    cout << "INFO: node value = " << l_imeiStr << endl;

    std::string l_msisdnStr = getChildNodeValue(ap_node, "msisdn-start-value");
    cout << "INFO: node value = " << l_msisdnStr << endl;
    
    std::string l_cgiStr = getChildNodeValue(ap_node, "cgi-start-value");
    cout << "INFO: node value = " << l_cgiStr << endl;
    
    std::string l_timeStr = getChildNodeValue(ap_node, "time-start-value");
    cout << "INFO: node value = " << l_timeStr << endl;
    
    timespec l_time = {10000, 20000};
    
    for (uint64_t i = 0; i < a_numItems; i++)
    {
        //
        // Add expected find values to expected container
        //
        
        IDCMValue* lp_value = new CSubscriberInfo;

        uint64_t l_imei = atoi(l_imeiStr.c_str()) + i;
        uint64_t l_msisdn = atoi(l_msisdnStr.c_str()) + i;
        uint64_t l_cgi = atoi(l_cgiStr.c_str()) + i;
        
        ((CSubscriberInfo*)lp_value)->addTimestamp(CSubscriberInfo::S_TIMESTAMP_TAG, l_time);
        ((CSubscriberInfo*)lp_value)->addUbit64(CSubscriberInfo::S_IMEI_TAG, l_imei);
        ((CSubscriberInfo*)lp_value)->addUbit64(CSubscriberInfo::S_MSISDN_TAG, l_msisdn);
        ((CSubscriberInfo*)lp_value)->addUbit64(CSubscriberInfo::S_CGI_TAG, l_cgi);
        
        arp_valueContainer[i] = lp_value;
    }
}

//----------------------------------------------------------------------------//

void CDCMTestUtil::extractSessionInfoValue(IXmlNode* ap_node,
                                           uint64_t a_numItems,
                                           IDCMValue**& arp_valueContainer)
{
    std::string l_imsiStr = getChildNodeValue(ap_node, "imsi-start-value");
    cout << "INFO: node value = " << l_imsiStr << endl;
    
    std::string l_apnStr = getChildNodeValue(ap_node, "apn-start-value");
    cout << "INFO: node value = " << l_apnStr << endl;
    
    std::string l_qciStr = getChildNodeValue(ap_node, "qci-start-value");
    cout << "INFO: node value = " << l_qciStr << endl;
    
    std::string l_timeStr = getChildNodeValue(ap_node, "time-start-value");
    cout << "INFO: node value = " << l_timeStr << endl;
    
    timespec l_time = {10000, 20000};
    
    for (uint64_t i = 0; i < a_numItems; i++)
    {
        //
        // Add expected find values to expected container
        //
        
        IDCMValue* lp_value = new CSessionInfo;

        uint64_t l_imsi = atoi(l_imsiStr.c_str()) + i;
        uint32_t l_apn = atoi(l_apnStr.c_str()) + i;
        uint8_t l_qci = (atoi(l_qciStr.c_str()) + i) % 100;
        
        ((CSessionInfo*)lp_value)->addTimestamp(CSessionInfo::S_TIMESTAMP_TAG, l_time);
        ((CSessionInfo*)lp_value)->addUbit64(CSessionInfo::S_IMSI_TAG, l_imsi);
        ((CSessionInfo*)lp_value)->addUbit32(CSessionInfo::S_APNID_TAG, l_apn);
        ((CSessionInfo*)lp_value)->addUbit8(CSessionInfo::S_QCI_TAG, l_qci);
        
        arp_valueContainer[i] = lp_value;
    }
}


//----------------------------------------------------------------------------//

void CDCMTestUtil::extractEnrichmentInfoValue(IXmlNode* ap_node,
                                              uint64_t a_numItems,
                                              CEnrichmentInfo**& arp_valueContainer)
{
    std::string l_imsiStr = getChildNodeValue(ap_node, "imsi-start-value");
    cout << "INFO: node value = " << l_imsiStr << endl;
    
    std::string l_apnStr = getChildNodeValue(ap_node, "apn-start-value");
    cout << "INFO: node value = " << l_apnStr << endl;
    
    std::string l_qciStr = getChildNodeValue(ap_node, "qci-start-value");
    cout << "INFO: node value = " << l_qciStr << endl;
    
    std::string l_imeiStr = getChildNodeValue(ap_node, "imei-start-value");
    cout << "INFO: node value = " << l_imeiStr << endl;

    std::string l_msisdnStr = getChildNodeValue(ap_node, "msisdn-start-value");
    cout << "INFO: node value = " << l_msisdnStr << endl;
    
    std::string l_cgiStr = getChildNodeValue(ap_node, "cgi-start-value");
    cout << "INFO: node value = " << l_cgiStr << endl;
    
    std::string l_timeStr = getChildNodeValue(ap_node, "time-start-value");
    cout << "INFO: node value = " << l_timeStr << endl;
    
    timespec l_time = {10000, 20000};
    
    for (uint64_t i = 0; i < a_numItems; i++)
    {
        //
        // Add expected find values to expected container
        //
        
        CEnrichmentInfo* lp_value = new CEnrichmentInfo;

        uint64_t l_imsi = atoi(l_imsiStr.c_str()) + i;
        //uint64_t l_apn = atoi(l_apnStr.c_str()) + i;
        uint8_t l_qci = (atoi(l_qciStr.c_str()) + i) % 100;
        uint64_t l_imei = atoi(l_imeiStr.c_str()) + i;
        uint64_t l_msisdn = atoi(l_msisdnStr.c_str()) + i;
        uint64_t l_cgi = atoi(l_cgiStr.c_str()) + i;
        
        ((CEnrichmentInfo*)lp_value)->addTimestamp(CEnrichmentInfo::S_TIMESTAMP_TAG, l_time);
        ((CEnrichmentInfo*)lp_value)->addUbit64(CEnrichmentInfo::S_IMEI_TAG, l_imei);
        ((CEnrichmentInfo*)lp_value)->addUbit64(CEnrichmentInfo::S_MSISDN_TAG, l_msisdn);
        ((CEnrichmentInfo*)lp_value)->addUbit64(CEnrichmentInfo::S_CGI_TAG, l_cgi);
        ((CEnrichmentInfo*)lp_value)->addUbit64(CEnrichmentInfo::S_IMSI_TAG, l_imsi);
        //((CEnrichmentInfo*)lp_value)->addUbit64(CEnrichmentInfo::S_APN_TAG, l_apn);
        ((CEnrichmentInfo*)lp_value)->addUbit8(CEnrichmentInfo::S_QCI_TAG, l_qci);
        
        arp_valueContainer[i] = lp_value;
    }
}

//----------------------------------------------------------------------------//

std::string CDCMTestUtil::getChildNodeValue(IXmlNode* ap_parentNode,
                                            std::string a_childNodeName)
{
    IXmlNode* lp_dataNode = static_cast <CXmlNode*> (ap_parentNode)->element(a_childNodeName.c_str());
    
    std::string ret_str = "";
    
    if ( 0 != lp_dataNode )
    {
        cout << "INFO: node name = " << lp_dataNode->xmlTagStr() << endl;
        
        CConfigSupport::stringFromXml ( lp_dataNode, ret_str );
    }

    return ret_str;
}

//----------------------------------------------------------------------------//


bool CDCMTestUtil::testInsert()
{
    bool l_ret = true;
    
    cout << "**************************************" << endl;
    cout << "INFO: test insert" << endl;
    cout << "**************************************" << endl;
    
    cout << "INFO: DCM size pre-insert = " << mp_dcm->size() << endl;
    
    for (uint64_t i = 0; i < mp_insertKeys.size(); i++)
    {
        for (uint64_t j = 0; j < m_numInserts; j++)
        {
            if (! mp_dcm->insert(mp_insertKeys[i][j], mp_insertValues[i][j]))
            {
                cout << "INFO: insert fail" << endl;
                l_ret = false;
                
                break;
            }
        }
    }
    
    cout << "INFO: DCM size after inserts = " << mp_dcm->size() << endl;
    
    return l_ret;
}


//----------------------------------------------------------------------------//


bool CDCMTestUtil::testFind()
{
    bool l_ret = true;
    
    cout << "**************************************" << endl;
    cout << "INFO: test find, number of finds = " << m_numFinds << endl;
    cout << "**************************************" << endl;
    
    CDCMUserPlaneEnrichment* lp_upEnrichmentDcm = dynamic_cast <CDCMUserPlaneEnrichment*> (mp_dcm);

    if (0 != lp_upEnrichmentDcm)
    {
        cout << "INFO: UP enrichment table" << endl;
        
        for (uint64_t i = 0; i < m_numFinds; i++)
        {
            CTeidIpAddressKey* lp_key = dynamic_cast <CTeidIpAddressKey*> (mp_findKeys[i]);
            
            if (CDCMUserPlaneEnrichment::eFound == lp_upEnrichmentDcm->find(lp_key, mp_enrichmentFindValues[i]))
            {
                if (! mp_enrichmentFindValues[i]->equals(mp_expectedEnrichmentFindValues[i]))
                {
                    cout << "INFO: equals failed, i = " << i << endl;
                    l_ret = false;
                    
                    break;
                }
            }
            else
            {
                cout << "INFO: find fail, i = " << i << endl;
                l_ret = false;
                
                break;
            }
        }
    }
    else
    {
        for (uint64_t i = 0; i < m_numFinds; i++)
        {
            if (mp_dcm->find(mp_findKeys[i], mp_findValues[i]))
            {
                if (! mp_findValues[i]->equals(mp_expectedFindValues[i]))
                {
                    cout << "INFO: equals failed, i = " << i << endl;
                    l_ret = false;
                    
                    break;
                }
            }
            else
            {
                cout << "INFO: find fail, i = " << i << endl;
                l_ret = false;
                
                break;
            }
        }
    }
    
    return l_ret;
}


//----------------------------------------------------------------------------//


bool CDCMTestUtil::cleanUp()
{
    cout << "INFO: cleanUp" << endl;
    
    delete mp_dcm;
    mp_dcm = 0;

    for (uint32_t i = 0; i < mp_insertKeys.size(); i++)
    {
        for (uint64_t j = 0; j < m_numInserts; j++)
        {
            delete mp_insertKeys[i][j];
            mp_insertKeys[i][j] = 0;
        }
        
        delete[] mp_insertKeys[i];
        mp_insertKeys[i] = 0;
    }
    mp_insertKeys.clear();
    
    for (uint32_t i = 0; i < mp_insertValues.size(); i++)
    {
        delete[] mp_insertValues[i];
        mp_insertValues[i] = 0;
    }
    mp_insertValues.clear();
    
    if (0 != mp_findKeys)
    {
        for (uint64_t i = 0; i < m_numFinds; i++)
        {
            delete mp_findKeys[i];
            mp_findKeys[i] = 0;
        }
    
        delete[] mp_findKeys;
        mp_findKeys = 0;
    }
    
    if (0 != mp_expectedFindValues)
    {
        delete[] mp_expectedFindValues;
        mp_expectedFindValues = 0;
    }
    
    
    if (0 != mp_findValues)
    {
        delete[] mp_findValues;
        mp_findValues = 0;
    }
    
    if (0 != mp_expectedEnrichmentFindValues)
    {
        for (uint64_t i = 0; i < m_numFinds; i++)
        {
            delete mp_expectedEnrichmentFindValues[i];
            mp_expectedEnrichmentFindValues[i] = 0;
        }
    
        delete[] mp_expectedEnrichmentFindValues;
        mp_expectedEnrichmentFindValues = 0;
    }
    
    if (0 != mp_enrichmentFindValues)
    {
        for (uint64_t i = 0; i < m_numFinds; i++)
        {
            delete mp_enrichmentFindValues[i];
            mp_enrichmentFindValues[i] = 0;
        }
    
        delete[] mp_enrichmentFindValues;
        mp_enrichmentFindValues = 0;
    }
    
    return true;
}


//----------------------------------------------------------------------------//

} // namespace maa_dcmtestutils
