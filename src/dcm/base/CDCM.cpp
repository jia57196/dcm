
#include "CDCM.h"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/flat_map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

#include <string>
#include <iostream>

using std::cout;
using std::endl;

#include "IDCMContainer.h"
#include "CDCMXmlConfigLoader.h"

namespace NDCM
{

    const char*      CDCM::CFG_INITIAL_FILE_SIZE      = "file_size";
    const char*      CDCM::CFG_CACHE_FILE_LOCATION    = "file_location";

    const ssize_t    CDCM::PAGE_SIZE                  = 4096;
    const uint64_t   CDCM::DEF_INITIAL_FILE_SIZE      = 1024 * 1024 * 1024; //1Gb table size
    const char*      CDCM::DEF_CACHE_FILE_LOCATION    = "/tmp/ubercache.db";

    managed_mapped_file* CDCM::mp_file                = NULL;

    //-----------------------------------------------------------------------//
    CDCM::CDCM()
    :   mp_container( NULL ),
        mp_config ( NULL ),
        m_fileName( DEF_CACHE_FILE_LOCATION ),
        m_fileSize( DEF_INITIAL_FILE_SIZE )
    { }

    //-----------------------------------------------------------------------//
    CDCM::~CDCM()
    {
        delete mp_file;
        mp_file = NULL;

        delete mp_container;
        mp_container = NULL;

        delete mp_config;
        mp_config = NULL;
    }

    //-----------------------------------------------------------------------//
    bool CDCM::initialize ( IDCMContainer* ap_container )
    {
        return ( NULL !=  ( mp_container = ap_container ) );
    }

    //-----------------------------------------------------------------------//
    bool CDCM::configure( const char* ap_fileName )
    {
        bool l_ret = true;

        //TODO: Change to plugin pattern. Should not hard link to XML
        mp_config = new CDCMXmlConfigLoader;
        l_ret = mp_config->loadConfig ( ap_fileName );

        std::string l_tag;

        if ( l_ret )
        {
            l_tag = CFG_CACHE_FILE_LOCATION;
            std::string l_defFileLocation = DEF_CACHE_FILE_LOCATION;
            if ( false == mp_config->loadValue ( l_tag, m_fileName, l_defFileLocation) )
            {
                //The file is required.
                l_ret = false;
            }
        }
        
        if ( l_ret )
        {
            l_tag = CFG_INITIAL_FILE_SIZE;
            mp_config->loadValue ( l_tag, m_fileSize, DEF_INITIAL_FILE_SIZE );
        }

        if ( l_ret )
        {
            //Try to open the file
            openFile ( m_fileName.c_str() );
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCM::insert( IDCMKey* ap_key, IDCMValue* ap_object )
    {
        bool l_ret = false;

        try
        {
            l_ret = mp_container->insert ( ap_key, ap_object );
        }
        catch ( const std::bad_alloc &ex )
        {
            //The cache probably ran out of memory
            resize();
            
            //Now try to insert again. 
            //The process will core here on a second fail
            mp_container->insert ( ap_key, ap_object );
        }

        return l_ret;

    }

    //-----------------------------------------------------------------------//
    bool CDCM::find ( IDCMKey* ap_key, IDCMValue* ap_value )
    {
        bool l_ret = false;

        if ( NULL != mp_container && NULL != ap_key )
        {
            l_ret = mp_container->find ( ap_key, ap_value );
        }
        
        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCM::remove ( IDCMKey* ap_key )
    {
        bool l_ret = false;

        if ( NULL != mp_container )
        {
            l_ret = mp_container->remove ( ap_key );
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    uint64_t CDCM::size()
    {
        uint64_t l_size = 0;

        if ( NULL != mp_container )
        {
            l_size = mp_container->size();
        }

        return l_size;
    }    

    //-----------------------------------------------------------------------//
    uint64_t CDCM::fileSize()
    {
        uint64_t l_size = 0;

        if ( NULL != mp_file )
        {
            l_size = mp_file->get_size();
        }

        return l_size;
    }

    //-----------------------------------------------------------------------//
    uint64_t CDCM::fileFreeMem()
    {
        uint64_t l_size = 0;

        if ( NULL != mp_file )
        {
            l_size = mp_file->get_free_memory();
        }

        return l_size;
    }

    //-----------------------------------------------------------------------//
    bool CDCM::openFile ( const char* ap_fileName )
    {
        bool l_ret = true;

        // Set the filename
        if ( NULL != ap_fileName )
        {
            m_fileName = ap_fileName;
        }

        //Now try to open it
        try 
        {
            cout << "Trying to create mapped file [name=" << m_fileName << "] "
                 << "[size=" << m_fileSize << "] "  << endl;
                
            if ( NULL == ( mp_file = new managed_mapped_file(  boost::interprocess::open_or_create, 
                                                               m_fileName.c_str(), 
                                                               m_fileSize ) ) )
            {
                //Memory allocation failed
                l_ret = false;
            }
            else
            {
                cout << "File created successfully" << endl;
            }    
                
                                                     
                                                     
            mp_container->initializeContainer (  mp_file );


        }
        catch ( boost::interprocess::interprocess_exception &l_ex )
        {
           //Something bad happened in the mmap oepn
           l_ret = false;
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCM::closeFile ()
    {
        bool l_ret = true;

        delete mp_file;
        mp_file = NULL;

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCM::resize ()
    {
        bool l_ret = true;

        //First close the old cache file
        if ( true == closeFile() )
        {

            // Get the number of pages currently 
            ssize_t l_numPages =  m_fileSize / PAGE_SIZE;

            // Now double the page size for the new file
            m_fileSize = l_numPages * 2 * PAGE_SIZE;
            
            //Clear out the old file object
            delete mp_file;
            mp_file = NULL;

            //Tell boost to reset the file
            managed_mapped_file::grow ( m_fileName.c_str(), m_fileSize );
            mp_file =  new managed_mapped_file( boost::interprocess::open_only, m_fileName.c_str() ); 

            //Now get our new container handle
            mp_container->initializeContainer (  mp_file );
        }
        else
        {
            l_ret = false;
        }

        return l_ret;
    }

};
