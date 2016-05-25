#include "CDCMMappedValue.h"

#include "CDCM.h"

using namespace boost::interprocess; 

namespace NDCM
{
    //-----------------------------------------------------------------------//
    CDCMMappedValue::~CDCMMappedValue()
    {
        if ( m_handle )
        {
            mp_file = CDCM::getFile();
            void* lp_mappedAddr = mp_file->get_address_from_handle( m_handle );
            mp_file->deallocate ( lp_mappedAddr );
        }    

        mp_valueData = NULL;
        m_dataValueSize = 0;
        m_handle = 0;
    }

    //-----------------------------------------------------------------------//
    CDCMMappedValue::CDCMMappedValue( const CDCMMappedValue& ar_copy )
    :   mp_file( ar_copy.mp_file ),
        mp_valueData(NULL ),
        m_handle ( 0 ),
        m_dataValueSize ( ar_copy.getDataSize() )
    {
        mp_file = CDCM::getFile();
        mp_valueData = mp_file->allocate ( m_dataValueSize );
        memcpy ( mp_valueData, mp_file->get_address_from_handle( ar_copy.m_handle ), m_dataValueSize );

        m_handle = mp_file->get_handle_from_address ( mp_valueData );
    }

    //-----------------------------------------------------------------------//
    CDCMMappedValue& CDCMMappedValue::operator= ( const CDCMMappedValue& ar_copy )
    {
        if ( this != &ar_copy )
        {
            mp_file = CDCM::getFile();
            uint8_t* lp_data = (uint8_t*)mp_file->get_address_from_handle( ar_copy.m_handle );

            this->setData ( lp_data, ar_copy.m_dataValueSize );
        }
    }

    //-----------------------------------------------------------------------//
    const void* CDCMMappedValue::getData ()
    { 
        mp_file = CDCM::getFile();

        return mp_file->get_address_from_handle( m_handle ); 
    }

    //-----------------------------------------------------------------------//
    const uint32_t CDCMMappedValue::getDataSize() const 
    { 
        return m_dataValueSize; 
    }

    //-----------------------------------------------------------------------//
    void CDCMMappedValue::setData ( uint8_t* ap_data, uint32_t a_dataSize )
    {
        //Make sure the file pointer is up to date
        mp_file = CDCM::getFile();

        if ( NULL == mp_valueData || a_dataSize != m_dataValueSize )
        {
            if ( m_handle )
            {
                //Make sure the data pointer is up to date
                mp_valueData = mp_file->get_address_from_handle( m_handle );

                mp_file->deallocate ( mp_valueData );
                m_handle = 0;
            }
            
            m_dataValueSize = a_dataSize;

            mp_valueData = static_cast<uint8_t*>(mp_file->allocate ( m_dataValueSize ));
            m_handle = mp_file->get_handle_from_address ( mp_valueData );
        }
        else
        {
            //Make sure the offset_prt is at the correct location
            mp_valueData = mp_file->get_address_from_handle ( m_handle );
        }     
     
        memset ( mp_valueData, 0, m_dataValueSize );
        memcpy ( mp_valueData, ap_data, m_dataValueSize );
    }

};
