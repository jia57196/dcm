#ifndef CDCMMultiTableTimeSlicedContainer_h
#define CDCMMultiTableTimeSlicedContainer_h

#include "IDCMContainer.h"

#include <boost/interprocess/managed_mapped_file.hpp>
#include <set>

class IDCMKey;
class IDCMValue;

class CDCMMultiTableTimeSlicedContainer: public IDCMContainer
{
    public:
        
        /*
         * Constructor
         */
        CDCMMultiTableTimeSlicedContainer();

        /**
         * Destructor
         */
        virtual ~CDCMMultiTableTimeSlicedContainer();

        /**
         * Sets up the container object
         */
        virtual void initializeContainer ( managed_mapped_file* ap_file );

        /**
         * Inserts key into one of the child tables
         *
         * @param ap_key - the key to insert
         * @param ap_value - the value to store
         * @return - true if the insert succeeded
         */
        virtual bool insert ( IDCMKey* ap_key, IDCMValue* ap_value );

        /**
         * Finds and returns the value linked to hte key object
         * 
         * @param ap_key - the key to find
         * @param ap_value - the value to update with found value
         * @return bool - true if found
         */
        virtual bool find ( IDCMKey* ap_key, IDCMValue* ap_value );

        /**
         * Remove a key object from the mapped table
         *
         * @param ap_key - the key to remove
         * @return bool - true if object removed
         */
        virtual bool remove ( IDCMKey* ap_key );

        /**
         * This function deletes all elements contained in the map
         *
         * @return bool - true if all items successfully purged
         */
        virtual bool purge();

        /**
         * The size in bytes of all populated tables
         */
        virtual uint64_t size();

        /**
         * Adds a new table to the array. 
         *
         * @param a_keyId - the key id's to be stored
         * @param a_tableSize - the initial size of the table in bytes
         * @return - true if a new table was created.
         */
        virtual bool addTable ( uint8_t a_keyId, uint32_t a_tableSize );

        /**
         * Destroys a mapped table fromteh array
         *
         * @param a_keyId - the table to destroy
         * @return - true if found and destroyed
         */
        virtual bool removeTable ( uint8_t a_keyId );

    private:

        /** The map of tables being accessed 
        /* The current table that we
        CDCMMultiTableContainer* mp_currentSet;

        /* Map of populated tables indexes */
        std::set < uint8_t > m_populatedTables;

        /** The mapped file used to create new tables **/
        boost::interprocess::managed_mapped_file* mp_file;

};

#endif
