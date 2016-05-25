
#include "CDCMMapContainer.h"

#include "CDCMMappedKey.h"
#include "CDCMMappedValue.h"

#include "IDCMKey.h"
#include "IDCMValue.h"

namespace NDCM
{

    //-----------------------------------------------------------------------//
    CDCMMapContainer::CDCMMapContainer( const char* ap_name )
    : mp_container ( NULL )
    {
        if ( NULL != ap_name )
        {
            m_containerName = ap_name;
        }
        {
            m_containerName = "CDCMMapContainer";
        }    
    }

    //-----------------------------------------------------------------------//
    CDCMMapContainer::~CDCMMapContainer()
    {
        //Boost will handle this object
        mp_container = NULL;
    }    

    //-----------------------------------------------------------------------//
    void CDCMMapContainer::initializeContainer ( managed_mapped_file* ap_file )
    {
        tDCMMapAllocator alloc_inst ( ap_file->get_segment_manager() );    

        //Is this a reconfigure
        if ( 0 == m_handle )
        {
                
            //Find the object
            mp_container = ap_file->find_or_construct   < tDCMMapContainer >
                                                        ( m_containerName.c_str() ) 
                                                        ( CDCMMappedKey::CDCMMappedKeyLessThanOperator(),
                                                          alloc_inst );

            //Collect the handle so that we can find the object again later
            m_handle = ap_file->get_handle_from_address ( mp_container );                                              
        }
        else
        {
            mp_container = static_cast < tDCMMapContainer* > ( ap_file->get_address_from_handle ( m_handle ) );
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
    bool CDCMMapContainer::insert ( IDCMKey* ap_key, IDCMValue* ap_value )
    {
        std::pair < tDCMMapContainerIter, bool > l_iter;
        
        #ifdef DEBUG
            CDCMMappedKey l_key  ( (*ap_key->getKeyObject() ) );
            CDCMMappedValue l_val ( *ap_value->getDataObject() );
        #endif

        //Serialize the objects prior to placing in map
        ap_key->mapKeyToSharedSpace();
        ap_value->mapDataToSharedSpace();

        {
            scoped_lock < tDCMMutex > lock ( (*mp_mutex), try_to_lock );

            l_iter = mp_container->insert ( std::make_pair ( (*ap_key->getKeyObject()), (*ap_value->getDataObject()) ) );
        }

        return l_iter.second;
    }

    //-----------------------------------------------------------------------//
    bool CDCMMapContainer::find ( IDCMKey* ap_key, IDCMValue* ap_value )
    {
        bool l_ret = false;
        
        //Serialize the key object to make sure we are querying off the right data
        ap_key->mapKeyToSharedSpace();

        tDCMMapContainerIter l_iter = mp_container->find ( (*ap_key->getKeyObject()) );

        if ( mp_container->end() != l_iter )
        {
            ap_value->setDataObject ( &(l_iter->second) );
            
            //Move the mapped data back into the top level object
            ap_value->deserialize();

            l_ret = true;
        }
        
        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CDCMMapContainer::remove ( IDCMKey* ap_key )
    {
        bool l_ret = false;

        //Serialize the key before the lookup
        ap_key->mapKeyToSharedSpace();

        tDCMMapContainerIter l_iter = mp_container->find ( (*ap_key->getKeyObject() ) );

        if ( mp_container->end() != l_iter )
        {
            //Lock the map
            scoped_lock < tDCMMutex > lock ( (*mp_mutex), try_to_lock );

            //Now remove from the map
            mp_container->erase ( l_iter );
            l_ret = true;
        }

        return l_ret;

    }

    //-----------------------------------------------------------------------//
    bool CDCMMapContainer::purge ()
    {
        //The container must be locked during this operation to stop some rogue
        //insert or find from happening
        scoped_lock < tDCMMutex > lock ( (*mp_mutex), try_to_lock );

        mp_container->clear();

        return mp_container->empty();
    }

    //-----------------------------------------------------------------------//
    uint64_t CDCMMapContainer::size()
    {
        uint64_t l_size = 0;

        if ( NULL != mp_container )
        {
            l_size = mp_container->size();
        }

        return l_size;
    }

};
