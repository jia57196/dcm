
#include "IDCMContainer.h"

namespace NDCM
{
    //-----------------------------------------------------------------------//
    IDCMContainer::IDCMContainer( const char* ap_name )
    :   m_handle ( 0 ),
        m_containerName ( ap_name ),
        mp_mutex ( NULL ),
        m_mutexHandle ( 0 )
    {}

    //-----------------------------------------------------------------------//
    IDCMContainer::~IDCMContainer()
    {
        m_handle = 0;
        m_mutexHandle = 0;
    }

    //-----------------------------------------------------------------------//
    const char* IDCMContainer::getName()
    {
        return m_containerName.c_str();
    }

};
