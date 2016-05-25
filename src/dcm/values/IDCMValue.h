#ifndef IDCMValue_h
#define IDCMValue_h

#include <stdint.h>

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{

    class CDCMMappedValue;

    /**
     * This object provides a interface to allow C++
     * objects to live in the shared memory space
     */
    class IDCMValue
    {
        public:

            /**
             * Constructor
             */
            IDCMValue();

            /**
             * Constructor
             *
             * @param CDCMMappedValue - the mapped object
             */
            IDCMValue( CDCMMappedValue* ap_value );

            /**
             * Destructor
             */
            virtual ~IDCMValue();

            /**
             * Returns the mapped value object
             *
             * @return CDCMMappedValue
             */
            CDCMMappedValue* getDataObject();

            /**
             * Maps the value object into the shared memory space
             */
            void mapDataToSharedSpace();

            /**
             * deserialize method used to pull data out  of mapped object when recieved
             * over IPC
             */
            virtual void deserialize() =0;


            /**
             * set the mapped value object
             *
             * @param ap_data - the pointer to the data object
             *
             * @return CDCMMappedValue
             */
            void setDataObject( CDCMMappedValue* ap_data );
    
            /**
             * Returns true if the provided value is equal
             * @param ap_cmp the value to compare against
             * @return true if the values are equal
             */
            virtual bool equals(IDCMValue* ap_cmp) = 0;
            

        protected:

            /**
             * serialize method, used to place data into mapped object before inserting
             *  into map
             *
             * @param apr_valueData - the raw data to be populated with the objects data members
             * @param ar_valueSize - the size of the serialized object
             */
            virtual void serialize( uint8_t*& apr_valueData, uint32_t& ar_valueSize ) =0;

            /**
             * Returns the serialized size of the object
             */
            virtual uint32_t getSerializedSize() =0;

            /**
             * Returns the raw data held by the mapped object
             *
             * @return uint8_t* - the raw data
             */
            const uint8_t* getData() const;

            /**
             * Returns the size of the raw data held by the mapped object
             *
             * @return uint32_t - the raw data size
             */
            const uint32_t getDataSize() const ;
        
            /**
             * Sets the data in the underlying mapped object
             *
             * @param ap_data - the data to store
             * @param a_dataSize - the size of the data
             */
            virtual void setData ( uint8_t* ap_data, uint32_t a_dataSize );

        private:

            /**
             * The mapped value object
             */
            CDCMMappedValue* mp_mappedValue;

            /**
             * Own the mapped object 
             */
            bool m_ownMappedObject; 
    };

};
#endif
