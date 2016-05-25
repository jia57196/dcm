#include "IDCMKey.h"
#include "CDCMMappedKey.h"

namespace NDCM
{

    //-----------------------------------------------------------------------//
    IDCMKey::IDCMKey ( uint8_t a_keyId )
    {
        //Create our mapped key object
        mp_mappedKey = new CDCMMappedKey( a_keyId );
    }

    //-----------------------------------------------------------------------//
    IDCMKey::~IDCMKey()
    {
        delete mp_mappedKey;
    }

    //-----------------------------------------------------------------------//
    void IDCMKey::mapKeyToSharedSpace()
    {
        //First create a key large enough
        //TODO: Create a memory pool for the these raw key objects
        uint32_t l_keySize = getSerializedSize();
        uint8_t* lp_keyData = new uint8_t[ l_keySize ];

        //Serialize the key and populate the keydata array
        serialize ( lp_keyData, l_keySize );

        //Now push the data down into the mapped key
        setKey ( lp_keyData, l_keySize );

        //Destroy the key data as it would have been mapped over
        delete [] lp_keyData;
    }

    //-----------------------------------------------------------------------//
    void IDCMKey::setKey ( uint8_t* ap_data, uint32_t a_keySize )
    {
        mp_mappedKey->setKey ( ap_data, a_keySize );
        mp_mappedKey->setHashCode ( createHashKey() );
    }    

    //-----------------------------------------------------------------------//
    const uint8_t* IDCMKey::getKey() const 
    {
        return static_cast< const uint8_t*>( mp_mappedKey->getKey() );
    }

    //-----------------------------------------------------------------------//
    const uint32_t IDCMKey::getKeySize() const 
    {
        return mp_mappedKey->getKeySize();
    }    

    //-----------------------------------------------------------------------//
    const uint8_t IDCMKey::getKeyId() const 
    {
        return mp_mappedKey->getKeyId();
    }    

    //-----------------------------------------------------------------------//
    CDCMMappedKey* IDCMKey::getKeyObject()
    {
        return mp_mappedKey;
    }    

};
