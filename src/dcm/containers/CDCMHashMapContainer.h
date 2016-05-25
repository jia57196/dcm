#ifndef CDCMHashMapContainer_h
#define CDCMHashMapContainer_h

#include "IDCMContainer.h"
#include "CDCMMappedKey.h"
#include "CDCMMappedValue.h"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{

    class IDCMKey;
    class IDCMValue;

    /**
     * Implementation of base container type extending for hash map functionality
     */
    class CDCMHashMapContainer: public IDCMContainer
    {
        public:

            /**
             * Constructor
             * 
             * @param ap_name - OPTIONAL: the container's name
             */
            CDCMHashMapContainer( const char* ap_name = NULL );

            /**
             * Destructor
             */
            virtual ~CDCMHashMapContainer();
            
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

            /**
             * Sets the hash size 
             */
            void setHashSize ( uint32_t a_hashSize );

            /** 
             * Gets the hash size
             */
            uint32_t getHashSize ();

        private:
            //Alocator typedefs
            typedef std::pair < const CDCMMappedKey, CDCMMappedValue > tDCMContainerObject; 
            typedef boost::interprocess::allocator < tDCMContainerObject, managed_mapped_file::segment_manager  > tDCMHashMapAllocator;

            //Map typedefs
            typedef boost::unordered_map < CDCMMappedKey, 
                                           CDCMMappedValue, 
                                           CDCMMappedKey::CDCMMappedKeyHashOperator, 
                                           CDCMMappedKey::CDCMMappedKeyEqualityOperator, 
                                           tDCMHashMapAllocator  > tDCMHashMapContainer;
            typedef tDCMHashMapContainer::iterator tDCMHashMapContainerIter;


            /* the map to insert into */
            tDCMHashMapContainer* mp_container;
             
            /** The size of the hash map **/
            uint32_t m_hashSize;

            /** This object allows for easily dumping out the contents of the map **/
            friend class CDCMS11Dumper;
    };

};

#endif
