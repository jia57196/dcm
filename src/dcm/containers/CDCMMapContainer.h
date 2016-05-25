#ifndef CDCMMapContainer_h
#define CDCMMapContainer_h

#include "IDCMContainer.h"
#include "CDCMMappedKey.h"
#include "CDCMMappedValue.h"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/flat_map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using boost::interprocess::managed_mapped_file;

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{

    class IDCMKey;
    class IDCMValue;

    /**
     * This object extends the container interface to provide map storage
     * in the shared memory space
     */
    class CDCMMapContainer: public IDCMContainer
    {
        public:

            /**
             * Constructor
             * 
             * @param ap_name - OPTIONAL: the container's name
             */
            CDCMMapContainer( const char* ap_name = NULL );

            /**
             * Destructor
             */
            virtual ~CDCMMapContainer();
            
            /**
             * sets up the underlying boost map
             */
            virtual void initializeContainer ( managed_mapped_file* ap_file );

            /**
             * Inserts a key into the map
             *
             * @param ap_key - the key to insert
             * @param ap_value - the value object ot insert
             * @return did the insert succeed
             */
            virtual bool insert ( IDCMKey* ap_key, IDCMValue* ap_value );

            /**
             * Finds and returns the value linked to hte key object
             * 
             * @param ap_key - the key to find
             * @param ap_value - the value to update with found value
             * @return bool - true if found
             */
            virtual bool find ( IDCMKey* ap_key, IDCMValue* ap_value );

            /**
             * Removes an item from the map
             *
             * @param ap_key - the key to remove
             * @return bool - true if hte key was found and removed
             */
            virtual bool remove ( IDCMKey* ap_key );

            /**
             * This function deletes all elements contained in the map
             *
             * @return bool - true if all items successfully purged
             */
            virtual bool purge();

            /**
             * The size of the map in bytes
             */
            virtual uint64_t size();

        private:
            //Alocator typedefs
            typedef std::pair < const CDCMMappedKey, CDCMMappedValue > tDCMContainerObject; 
            typedef boost::interprocess::allocator < tDCMContainerObject, managed_mapped_file::segment_manager  > tDCMMapAllocator;

            //Map typedefs
            typedef boost::interprocess::map < CDCMMappedKey, CDCMMappedValue, CDCMMappedKey::CDCMMappedKeyLessThanOperator, tDCMMapAllocator  > tDCMMapContainer;
            typedef tDCMMapContainer::iterator tDCMMapContainerIter;


            /* the map to insert into */
            tDCMMapContainer* mp_container;
             
            /*
             * named object created in mmapped space
             *  NOTE: unique names allows for multiple
             *  instances of same object
             */
            std::string m_containerName;
    };

};
#endif
