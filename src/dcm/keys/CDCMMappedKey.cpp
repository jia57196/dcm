#include "CDCMMappedKey.h"
#include "CDCM.h"

#include <iostream>
#include <string>

using namespace boost::interprocess;

namespace NDCM
{
    //-----------------------------------------------------------------------//
    CDCMMappedKey::CDCMMappedKey ( const CDCMMappedKey& ar_other )
    : mp_file ( CDCM::getFile() ), 
        mp_keyValue ( NULL ),
        m_handle ( 0 ),
        m_keySize ( ar_other.getKeySize() ),
        m_hashCode ( ar_other.getHashCode()  ),
        m_keyId ( ar_other.getKeyId() )
    {
        #ifdef KEY_ALLOCATION_TRACKING
        std::cout << "CDCMMappedKey::CDCMMappedKey() - [this =" << this << "] " 
                  << "[ar_other=" << &ar_other << "] "
                  << std::endl;
        #endif          

        mp_keyValue = mp_file->allocate ( m_keySize );
        memcpy ( mp_keyValue, ar_other.getKey(), m_keySize );

        m_handle = mp_file->get_handle_from_address ( mp_keyValue );
    }

    //-----------------------------------------------------------------------//
    CDCMMappedKey::~CDCMMappedKey()
    {
        #ifdef KEY_ALLOCATION_TRACKING
        std::cout << "CDCMMappedKey::~CDCMMappedKey() - [this =" << this << "] " 
                  << "[mp_keyValue=" << &mp_keyValue << "] "
                  << "[m_keySize=" << m_keySize <<"] "
                  << std::endl;
        #endif          

        if( m_handle )
        {
            //Make sure we have the right file pointer
            mp_file = CDCM::getFile();

            if ( NULL != mp_file )
            {
                //Make sure we have the right offset
                void* lp_mappedAddr = mp_file->get_address_from_handle ( m_handle );
                mp_file->deallocate ( lp_mappedAddr );
            }   
            /*else
            {
                //Mapped file could not be pulled from CDCM instance.
                //Is this a leak?
            }*/
        }

        mp_keyValue = NULL;
        m_keySize = 0;
        m_handle = 0;
    }

    //-----------------------------------------------------------------------//
    const void* CDCMMappedKey::getKey () const 
    { 
        return CDCM::getFile()->get_address_from_handle ( m_handle ); 
    }

    //-----------------------------------------------------------------------//
    const uint32_t CDCMMappedKey::getKeySize() const 
    { 
        return m_keySize; 
    } 

    //-----------------------------------------------------------------------//
    const uint32_t CDCMMappedKey::getHashCode() const
    {
        return m_hashCode;
    }

    //-----------------------------------------------------------------------//
    void CDCMMappedKey::setHashCode ( uint32_t a_hashCode )
    {
        m_hashCode = a_hashCode;
    }

    //-----------------------------------------------------------------------//
    const managed_mapped_file* CDCMMappedKey::getFile()
    { 
        mp_file = CDCM::getFile();
        return mp_file; 
    }

    //-----------------------------------------------------------------------//
    const managed_mapped_file::handle_t CDCMMappedKey::getHandle() const
    { 
        return m_handle; 
    }

    //-----------------------------------------------------------------------//
    const uint8_t CDCMMappedKey::getKeyId () const 
    {
        return m_keyId;
    }

    //-----------------------------------------------------------------------//
    void CDCMMappedKey::setKey ( const uint8_t* ap_data, const uint32_t a_size )
    {
        //Make sure we have the right file pointer
        mp_file = CDCM::getFile();

        //If the new value is hte same size, no need to reallocate
        if ( NULL == mp_keyValue || a_size != m_keySize )
        {

            #ifdef KEY_ALLOCATION_TRACKING
            std::cout << "CDCMMappedKey::setKey() - [this=" << this <<"] "
                      << "[m_keySize(old)=" << m_keySize << "] ";
            #endif
            if( m_handle )
            {
                //Make sure we have the right offset
                void* lp_mappedAddr = mp_file->get_address_from_handle ( m_handle );

                //Pace the shared memory back
                mp_file->deallocate ( lp_mappedAddr );
                #ifdef KEY_ALLOCATION_TRACKING
                std::cout << "[mp_keyValue(old)=" << &mp_keyValue << "] ";
                #endif

                mp_keyValue = NULL;
                m_keySize = 0;
                m_handle = 0;

            }
            else
            {
                #ifdef KEY_ALLOCATION_TRACKING
                std::cout << "[mp_keyValue(old)=NULL] ";
                #endif
            }    

            //Allocate the new block required.
            m_keySize = a_size;
            mp_keyValue = mp_file->allocate ( a_size );
            m_handle = mp_file->get_handle_from_address ( mp_keyValue );

            #ifdef KEY_ALLOCATION_TRACKING
            std::cout << "[m_keySize=" << m_keySize << "] "
                 << "[mp_keyValue=" << &mp_keyValue << "] "
                 << std::endl;
            #endif     
        }
        else
        {
            //Make sure the offset_prt is pointing to the correct place
            mp_keyValue = mp_file->get_address_from_handle ( m_handle );
        }    
            
        //In case we are using a generalized size. set the block to 0's
        memset ( mp_keyValue, 0, m_keySize );

        //Now just copy over the new value
        memcpy ( mp_keyValue, ap_data, m_keySize );

        #ifdef KEY_TRACKING
            std::string l_keyValue;
            l_keyValue.append ( reinterpret_cast<char*>(mp_keyValue), m_keySize );
            int l_size = l_keyValue.size();
            for ( int i = 0; i < l_size; i++ )
            {
                l_keyValue[i] += '0';
            }

            std::cout << "CDCMMappedKey::setKey() - [this=" << this <<"] "
                      << "[key Value=" << l_keyValue << "] "
                      << "[key Size=" << m_keySize << "] "
                      << std::endl;
        #endif

    }

    //-----------------------------------------------------------------------//
    size_t CDCMMappedKey::CDCMMappedKeyHashOperator::operator() ( const CDCMMappedKey& ar_key ) const
    {
       return ar_key.getHashCode(); 
    }

    //-----------------------------------------------------------------------//
    bool CDCMMappedKey::CDCMMappedKeyEqualityOperator::operator() ( const CDCMMappedKey& ar_one, const CDCMMappedKey& ar_two ) const
    {
        if ( ar_one.getKeySize() != ar_two.getKeySize() )
        {
            //Cant be equal if not the same size
            return false;
        }
        else
        {
            return ( 0 == memcmp ( ar_one.getKey(), ar_two.getKey(), ar_one.getKeySize() ) );
        }
    }

    //-----------------------------------------------------------------------//
    bool CDCMMappedKey::CDCMMappedKeyLessThanOperator::operator() ( const CDCMMappedKey& ar_one, const CDCMMappedKey& ar_two ) const
    {
        const uint32_t l_keyOneSize = ar_one.getKeySize();
        const uint32_t l_keyTwoSize = ar_two.getKeySize();

        if ( l_keyOneSize != l_keyTwoSize )
        {
            //Smaller keys are always less than
            return l_keyOneSize < l_keyTwoSize;
        }
        else
        {
            //Return the less based on a string compare
            return ( 0 > memcmp ( ar_one.getKey(), ar_two.getKey(), ar_one.getKeySize() ) );
        }
    }    

};
