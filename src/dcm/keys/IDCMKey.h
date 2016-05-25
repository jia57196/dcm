#ifndef IDCMKey_h
#define IDCMKey_h

#include <stdint.h>

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{
    class CDCMMappedKey;

    /**
     * This object provides an extendable mechanism for storing 
     * C++ objects in the shared memory space
     */
    class IDCMKey
    {
        public:

            /**
             * Constructor
             */
            IDCMKey( uint8_t a_keyId ); 

            /**
             * Destructor object
             */
            virtual ~IDCMKey ();

            /**
             * This function calls the chain of operations required to map the key into
             *  shared memory space
             */
            virtual void mapKeyToSharedSpace ();
            
            /**
             * Returns the serialized size of the raw key data
             */
            virtual uint32_t getSerializedSize() =0;

            /**
             * Returns the keys ID
             *
             * @return uint8_t 
             */
            const uint8_t getKeyId() const;

            /**
             * Returns the key object held within this class
             *
             * @return CDCMMappedKey - the key object that is shared betweeen processes
             */
             CDCMMappedKey* getKeyObject();

            /**
             * Deserialization method required for IPC key transfer
             */
            virtual void deserialize() =0;


        protected:

            /**
             * Serialization method required for IPC key transfer
             *
             * @param apr_keyData - the raw data to populate
             * @param ar_keySize - the size of the raw data array
             */
            virtual void serialize( uint8_t*& apr_keyData, uint32_t& ar_keySize ) =0;

            /**
             * Sets the raw key data. Will also set up the hash code based
             *  on hasher function
             *
             * @param ap_data - the key data
             * @param a_dataSize - the key data size
             */
            virtual void setKey ( uint8_t* ap_data, uint32_t a_keySize ); 

            /**
             * Returns the raw bytes mapped to shmem
             *
             * @return uint8_t* - array of bytes
             */
            const uint8_t* getKey() const ;

            /**
             * Returns the size of the data stored in the key object
             *
             * @return uint32_t - the size of the mapped key data
             */
            const uint32_t getKeySize() const;

            /**
             * Hash function used in setting up the key storage code
             */
            virtual uint32_t createHashKey () =0; 
             
        private:

            /**
             * The key object mmapped into the shared memory space
             */
            CDCMMappedKey* mp_mappedKey;

    };

};
#endif
