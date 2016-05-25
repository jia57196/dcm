
#include "CDCMHashMapContainer.h"

#include "CDCMMappedKey.h"
#include "CDCMMappedValue.h"

#include "IDCMKey.h"
#include "IDCMValue.h"

namespace NDCM
{

    static const uint32_t DEF_HASH_SIZE = 1024 * 1024; // 1MB for the default size
     
    //-----------------------------------------------------------------------//
    CDCMHashMapContainer::CDCMHashMapContainer( const char* ap_name )
    : mp_container ( NULL ),
      m_hashSize ( DEF_HASH_SIZE )
    {
        if ( NULL != ap_name )
        {
            m_containerName = ap_name;
        }
        {
            m_containerName = "CDCMHashMapContainer";
        }    
    }

    //-----------------------------------------------------------------------//
    CDCMHashMapContainer::~CDCMHashMapContainer()
    {
        //Boost will handle this object
        mp_container = NULL;
    }    

    //-----------------------------------------------------------------------//
    void CDCMHashMapContainer::initializeContainer ( managed_mapped_file* ap_file )
    {
        tDCMHashMapAllocator alloc_inst ( ap_file->get_segment_manager() );    

        //Is this a reconfigure
        if ( 0 == m_handle )
        {
                
            //Find the object
            mp_container = ap_file->find_or_construct   < tDCMHashMapContainer >
                                                        ( m_containerName.c_str() ) 
                                                        ( m_hashSize,
                                                          CDCMMappedKey::CDCMMappedKeyHashOperator(),
                                                          CDCMMappedKey::CDCMMappedKeyEqualityOperator(),
                                                          alloc_inst );

            //Collect the handle so that we can find the object again later
            m_handle = ap_file->get_handle_from_address ( mp_container );                                              
        }
        else
        {
            mp_container = static_cast < tDCMHashMapContainer* > ( ap_file->get_address_from_handle ( m_handle ) );
        }    
        
        if ( 0 == m_mutexHandle )
        {
            //Create the mutex to use during map updates
            mp_mutex = ap_file->find_or_construct < tDCMMutex > 
                                                  ( "DCMMutex" )
                                                  ();
            //Get the handle
            m_mutexHandle = ap_file->get_handle_from_address ( mp_mutex );
        }
        else
        {
            //Must be a reconfig or file change. Just update the mutex pointer
            mp_mutex = static_cast< tDCMMutex*> ( ap_file->get_address_from_handle ( m_mutexHandle ) );
        }    
    }    

    //-----------------------------------------------------------------------//
    bool CDCMHashMapContainer::insert ( IDCMKey* ap_key, IDCMValue* ap_value )
    {
        std::pair < tDCMHashMapContainerIter, bool > l_iter;
        
        //Serialize the key and value before placing in the hash map
        ap_key->mapKeyToSharedSpace();
        ap_value->mapDataToSharedSpace();

        {
            //Lock the map before insert
            scoped_lock < tDCMMutex > lock ( (*mp_mutex) );

            l_iter = mp_container->insert ( std::make_pair ( (*ap_key->getKeyObject()), (*ap_value->getDataObject()) ) );
        }

        return l_iter.second;
    }

    //-----------------------------------------------------------------------//
    bool  CDCMHashMapContainer::find ( IDCMKey* ap_key, IDCMValue* ap_value )
    {
        bool l_ret = false;
        
        //Serialize the key to make sure we are comparing the objects corectly
        ap_key->mapKeyToSharedSpace();

        tDCMHashMapContainerIter l_iter  = mp_container->find ( (*ap_key->getKeyObject()) );

        if ( mp_container->end() != l_iter )
        {
           ap_value->setDataObject ( &(l_iter->second) );

           //Move the mapped objects back into the top level object
           ap_value->deserialize();

           l_ret = true;
        }
        
        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCMHashMapContainer::remove ( IDCMKey* ap_key )
    {
        bool l_ret = false;
        
        //serialize the key prior to the find
        ap_key->mapKeyToSharedSpace();

        tDCMHashMapContainerIter l_iter = mp_container->find ( (*ap_key->getKeyObject() ) );

        if ( mp_container->end() != l_iter )
        {
            scoped_lock < tDCMMutex > lock ( (*mp_mutex ) );

            mp_container->erase ( l_iter );
            l_ret = true;
        }

        return l_ret;

    }

    //-----------------------------------------------------------------------//
    bool CDCMHashMapContainer::purge ()
    {
        //The container must be locked during this operation to stop some rogue
        //insert or find from happening
        scoped_lock < tDCMMutex > lock ( (*mp_mutex), try_to_lock );

        mp_container->clear();

        return mp_container->empty();
    }

    //-----------------------------------------------------------------------//
    uint64_t CDCMHashMapContainer::size()
    {
        uint64_t l_size = 0;

        if ( NULL != mp_container )
        {
            l_size = mp_container->size();
        }

        return l_size;
    }

    //-----------------------------------------------------------------------//
    void CDCMHashMapContainer::setHashSize ( uint32_t a_hashSize )
    {
        m_hashSize = a_hashSize;
    }

    //-----------------------------------------------------------------------//
    uint32_t CDCMHashMapContainer::getHashSize ()
    {
        return m_hashSize;
    }

};
