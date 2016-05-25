#include "IDCMValue.h"
#include "CDCMMappedValue.h"

namespace NDCM
{

    //-----------------------------------------------------------------------//
    IDCMValue::IDCMValue()
    : m_ownMappedObject ( true )
    {
        mp_mappedValue = new CDCMMappedValue;
    }

    //-----------------------------------------------------------------------//
    IDCMValue::IDCMValue ( CDCMMappedValue* ap_value )
    : m_ownMappedObject ( false ),
      mp_mappedValue ( ap_value )
    { }

    //-----------------------------------------------------------------------//
    IDCMValue::~IDCMValue()
    {
        if ( m_ownMappedObject )
        {
            delete mp_mappedValue;
        }    
    }

    //-----------------------------------------------------------------------//
    CDCMMappedValue* IDCMValue::getDataObject()
    {
        return mp_mappedValue;
    }

    //-----------------------------------------------------------------------//
    void IDCMValue::mapDataToSharedSpace()
    {
        //First create a key large enough
        //TODO: Create a memory pool for the these raw key objects
        uint32_t l_rawSize = getSerializedSize();
        uint8_t* lp_rawData = new uint8_t[ l_rawSize ];

        //Serialize the key and populate the keydata array
        serialize ( lp_rawData, l_rawSize );

        //Now push the data down into the mapped key
        setData ( lp_rawData, l_rawSize );

        //Destroy the key data as it would have been mapped over
        delete [] lp_rawData;
    }

    //-----------------------------------------------------------------------//
    void IDCMValue::setDataObject( CDCMMappedValue* ap_data )
    {
        //Clean up the old object if it exists
        if ( NULL == mp_mappedValue )
        {
            mp_mappedValue = new CDCMMappedValue;
        }

        (*mp_mappedValue) = (*ap_data);
        m_ownMappedObject = true;
    }

    //-----------------------------------------------------------------------//
    const uint8_t* IDCMValue::getData() const 
    {
        return static_cast < const uint8_t* > ( mp_mappedValue->getData() );
    }

    //-----------------------------------------------------------------------//
    const uint32_t IDCMValue::getDataSize() const
    {
        return mp_mappedValue->getDataSize();
    }

    //-----------------------------------------------------------------------//
    void IDCMValue::setData ( uint8_t* ap_data, uint32_t a_dataSize )
    {
        mp_mappedValue->setData ( ap_data, a_dataSize );
    }

};
