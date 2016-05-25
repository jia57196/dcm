#ifndef CDCMMappedKey_h
#define CDCMMappedKey_h

#include <stdint.h>
#include <string.h>

#include "tDCMTypedefs.h"

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

#include <iostream>

using namespace boost::interprocess;

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{
    /**
     * This is the object stored in the memory mapped file
     * It contains a simple serialized blob of data
     */
    class CDCMMappedKey
    {
        public:

            static const int DEF_KEY_SIZE = 256;

            CDCMMappedKey( uint8_t a_keyId )
            :   mp_file ( NULL ), 
                mp_keyValue(NULL),
                m_handle(0),
                m_keySize ( 0 ),
                m_hashCode ( ~0 ),
                m_keyId ( a_keyId )
            { 
                
                #ifdef KEY_ALLOCATION_TRACKING
                    std::cout << "CDCMMappedKey:IDCMKey() - [this=" << this << "] " << std::endl; 
                #endif
            } 

            /*
             * destructor
             */
            ~CDCMMappedKey();

            /**
             * The copy constructor
             */
            CDCMMappedKey ( const CDCMMappedKey& ar_other );

            /*
             * This function sets the key value 
             *
             * @param ap_data - the data used to set the key
             * @param a_size  - the size of the ap_data array
             */
            void setKey ( const uint8_t* ap_data, const uint32_t a_size );
                    
            /**
             * This function returns the key value
             */
            const void* getKey () const;
           
            /*
             * this function returns the size of the key 
             */
            const uint32_t getKeySize() const;

            /*
             * this function returns the hash code for the key
             */
            const uint32_t getHashCode() const;  
            
            /**
             * This function sets the hash code
             */
            void setHashCode( uint32_t a_hashCode ); 

            /**
             * Gets the Key ID
             */
            const uint8_t getKeyId() const;

            /**
             * The Equality operator
             */
            class CDCMMappedKeyEqualityOperator
            {
                public:
                    bool operator() ( const CDCMMappedKey& ar_one, const CDCMMappedKey& ar_two ) const;   
            };

            /**
             * The Hash operator
             */
            class CDCMMappedKeyHashOperator
            {
                public:
                    size_t operator() ( const CDCMMappedKey& ar_key ) const;
            };

            /**
             * The Less than operator
             */
            class CDCMMappedKeyLessThanOperator
            {
                public:
                    bool operator() ( const CDCMMappedKey& ar_one, const CDCMMappedKey& ar_two ) const;
            };

        private:

            /**
             * This function returns the objects mapped file
             */
            const managed_mapped_file* getFile();

            /**
             * this function returns the keyValue handle
             */
            const managed_mapped_file::handle_t getHandle() const;
            
        private:
            
            /** Mapped file area used to create the data string **/
            managed_mapped_file* mp_file;

            /* The value that will be used to do map lookups */ 
            void* mp_keyValue;
     
            /* the handle for the array to be allocated */
            managed_mapped_file::handle_t m_handle;

            /* The size of the array that contains the key */
            uint32_t m_keySize;

            /* The hash code for this object */
            uint32_t m_hashCode;

            /**
             * ID that allows for mapping keys to storage tables
             */
            uint8_t m_keyId; 


    };

};
#endif
