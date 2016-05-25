#ifndef CDCMMappedValue_h
#define CDCMMappedValue_h

#include <stdint.h>
#include <string.h>

#include "tDCMTypedefs.h"

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

using namespace boost::interprocess;

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{
    /**
     * This is the value object that lives in the managed mapped file
     */
    class CDCMMappedValue
    {
        public:
            static const int DEF_DATA_VALUE_SIZE = 1024;
            
            CDCMMappedValue()
            :   mp_file (NULL),
                mp_valueData(NULL),
                m_handle ( 0 ),
                m_dataValueSize ( 0 )
            {}


            ~CDCMMappedValue();

            CDCMMappedValue( const CDCMMappedValue& ar_copy);
           
            void    setData ( uint8_t* ap_data, uint32_t a_dataSize );
            
            const void* getData ();

            const uint32_t getDataSize() const;

            CDCMMappedValue& operator= ( const CDCMMappedValue& ar_data );

        private:

            /** The allocator for the value data **/
            managed_mapped_file* mp_file;

            /** The data stored in the object **/
            void* mp_valueData;

            /** The handle used to properly map the shared memory segment **/
            managed_mapped_file::handle_t m_handle;

            /** The size ofhte value data held **/
            uint32_t m_dataValueSize;
    };

};
#endif
