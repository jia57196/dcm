#include "CDCMXmlConfigLoader.h"
#include "CXmlNode.h"
#include "CXmlParserFactory.h"
#include "CConfigSupport.h"

namespace NDCM
{

    static const char* ELEMENT_NAME = "dcm";

    //-----------------------------------------------------------------------//
    CDCMXmlConfigLoader::CDCMXmlConfigLoader()
    : mp_root ( NULL )
    {}

    //-----------------------------------------------------------------------//
    CDCMXmlConfigLoader::~CDCMXmlConfigLoader()
    {
        delete mp_root;
    }

    //-----------------------------------------------------------------------//
    bool CDCMXmlConfigLoader::loadConfig( const char* ap_file )
    {
        bool l_ret = false;
        IXmlNode* lp_root = NULL;

        if ( CXmlParserFactory::XML_SUCCESS ==  CXmlParserFactory::parseToSimpleRepresentation ( lp_root, NULL, ap_file ) &&
             0 == strcasecmp( lp_root->xmlTagStr().c_str(), ELEMENT_NAME ) )
        {
            mp_root = static_cast < CXmlNode* > ( lp_root );
            mp_currentNode = mp_root;

            if ( NULL != mp_currentNode )
            {
                l_ret = true;
            }    
        }
        
        //Set the filename before returning
        m_fileName = ap_file;

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCMXmlConfigLoader::loadValue ( std::string& ar_configTag, uint64_t& ar_value, uint64_t a_default )
    {
        bool l_ret = true;

        IXmlNode* lp_node = mp_currentNode->element ( ar_configTag );

        if ( NULL == lp_node )
        {
            ar_value = a_default;
        }
        else
        {
            l_ret = CConfigSupport::uint64FromXml ( lp_node, ar_value );
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCMXmlConfigLoader::loadValue ( std::string& ar_configTag, int64_t& ar_value, int64_t a_default )
    {
        bool l_ret = true;

        IXmlNode* lp_node = mp_currentNode->element ( ar_configTag );

        if ( NULL == lp_node )
        {
            ar_value = a_default;
        }
        else
        {
            l_ret = CConfigSupport::uint64FromXml ( lp_node, (uint64_t&)ar_value );
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCMXmlConfigLoader::loadValue ( std::string& ar_configTag, uint32_t& ar_value, uint32_t a_default )
    {
        bool l_ret = true;

        IXmlNode* lp_node = mp_currentNode->element ( ar_configTag );

        if ( NULL == lp_node )
        {
            ar_value = a_default;
        }
        else
        {
            l_ret = CConfigSupport::uint32FromXml ( lp_node, ar_value );
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCMXmlConfigLoader::loadValue ( std::string& ar_configTag, int32_t& ar_value, int32_t a_default )
    {
        bool l_ret = true;

        IXmlNode* lp_node = mp_currentNode->element ( ar_configTag );

        if ( NULL == lp_node )
        {
            ar_value = a_default;
        }
        else
        {
            l_ret = CConfigSupport::uint32FromXml ( lp_node, (uint32_t&)ar_value );
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCMXmlConfigLoader::loadValue ( std::string& ar_configTag, bool& ar_value, bool a_default )
    {
        bool l_ret = true;

        IXmlNode* lp_node = mp_currentNode->element ( ar_configTag );

        if ( NULL == lp_node )
        {
            ar_value = a_default;
        }
        else
        {
            l_ret = CConfigSupport::boolFromXml ( lp_node, ar_value );
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCMXmlConfigLoader::loadValue ( std::string& ar_configTag, std::string& ar_value, std::string& ar_default )
    {
        bool l_ret = true;

        IXmlNode* lp_node = mp_currentNode->element ( ar_configTag );

        if ( NULL == lp_node )
        {
            ar_value = ar_default;
        }
        else
        {
            l_ret = CConfigSupport::stringFromXml ( lp_node, ar_value );
        }

        return l_ret;
    }

};
