
#include "CDCMMultiTableContainer.h"
#include "CDCMHashMapContainer.h"

#include "IDCMKey.h"
#include "IDCMValue.h"

CDCMMultiTableContainer::CDCMMultiTableContainer()
: mp_file ( NULL )
{
    m_containerName = "CDCMMultiTableContainer";

    //Initialize all of the pointers
    for ( int i = 0; i < CDCMMultiTableContainer::TABLE_SIZE; i++ )
    {
        mp_tables[i] = NULL;
    }
    
    m_populatedTables.clear();
}

CDCMMultiTableContainer::~CDCMMultiTableContainer()
{
}    

void CDCMMultiTableContainer::initializeContainer ( managed_mapped_file* ap_file )
{
    //Just hold on to the file until the tables are added
    mp_file = ap_file;
}    


bool CDCMMultiTableContainer::insert ( IDCMKey* ap_key, IDCMValue* ap_value )
{
    bool l_ret = false;

    IDCMContainer* lp_table = mp_tables[ ap_key->getKeyId() ];

    if ( NULL != lp_table )
    {
        l_ret = lp_table->insert ( ap_key, ap_value );
    }

    return l_ret;
}

bool CDCMMultiTableContainer::find ( IDCMKey* ap_key, IDCMValue* ap_value )
{
    bool l_ret = false;
    IDCMContainer* lp_table = mp_tables[ap_key->getKeyId()];
     
    if ( NULL != lp_table )
    {
        l_ret = lp_table->find ( ap_key, ap_value );
    }

    return l_ret;
}

bool CDCMMultiTableContainer::remove ( IDCMKey* ap_key )
{
    bool l_ret = false;

    IDCMContainer* lp_table = mp_tables[ap_key->getKeyId() ];

    if ( NULL != lp_table )
    {
        l_ret = lp_table->remove ( ap_key );
    }

    return l_ret;
}

bool CDCMMultiTableContainer::purge ()
{
    bool l_ret = true;

    IDCMContainer* lp_table = NULL;

    std::set<uint8_t>::iterator l_iter = m_populatedTables.begin();

    while ( l_iter != m_populatedTables.end() )
    {
        lp_table = mp_tables[ (*l_iter) ];
        
        if ( false == lp_table->purge() )
        {
            l_ret = false;
        }    

        l_iter++;
    }

    return l_ret;
}

uint64_t CDCMMultiTableContainer::size()
{
    uint64_t l_size = 0;
    IDCMContainer* lp_table = NULL;

    std::set<uint8_t>::iterator l_iter = m_populatedTables.begin();

    while ( l_iter != m_populatedTables.end() )
    {
        lp_table = mp_tables[ (*l_iter) ];
        l_size += lp_table->size();

        l_iter++;
    }

    return l_size;
}

bool CDCMMultiTableContainer::addTable ( uint8_t a_keyId, uint32_t a_tableSize )
{
    bool l_ret = false;

    //Don't create if already created.
    if ( NULL == mp_tables[ a_keyId ] )
    {
        std::stringstream l_stream;
        std::string l_name;

        l_stream << "CDCMHashMapContainer_" << a_keyId;
        l_name = l_stream.str();
        
        //Create the new table
        IDCMContainer* lp_table = new CDCMHashMapContainer ( l_name.c_str() );

        if ( lp_table )
        {
            static_cast<CDCMHashMapContainer*>(lp_table)->setHashSize ( a_tableSize );

            //Try to initialize the table
            lp_table->initializeContainer ( mp_file );
            l_ret = true;
        }

        mp_tables[ a_keyId ]  = lp_table;
        m_populatedTables.insert ( a_keyId );
        
    }

    return l_ret;
}

bool CDCMMultiTableContainer::removeTable ( uint8_t a_keyId )
{
    bool l_ret = false;

    if ( NULL != mp_tables[ a_keyId ] )
    {
        //Clean up the object
        delete mp_tables[ a_keyId ];
        mp_tables[ a_keyId ] = NULL;

        //Remove from the table set
        m_populatedTables.erase ( a_keyId );

        l_ret = true;
    }

    return l_ret;
}
