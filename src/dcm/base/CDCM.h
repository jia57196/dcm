#ifndef CDCM_h
#define CDCM_h

#include "boost/interprocess/managed_mapped_file.hpp"
#include <string>

#include "tDCMTypedefs.h"

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{

    class IDCMContainer;
    class CConfigFileParser;
    class tConfigurationReport;
    class IDCMValue;
    class IDCMKey;
    class IDCMConfigLoader;

    /**
     * This object is the public interface to the shared memory space
     * persistant file object
     */
    class CDCM
    {
        public:

            static const ssize_t    PAGE_SIZE;

            /* Statics used in config */
            static const char*      CFG_INITIAL_FILE_SIZE;
            static const char*      CFG_CACHE_FILE_LOCATION;

            static const uint64_t   DEF_INITIAL_FILE_SIZE;
            static const char*      DEF_CACHE_FILE_LOCATION;

        public:
            /* Default Constructor */
            CDCM();
        
            /* Default Destructor */
            virtual ~CDCM();

            /**
             * Initialize the object 
             *
             * @param ap_container - the type of object the map will store
             * @return - initialized correctly
             */
            virtual bool initialize ( IDCMContainer* ap_container );

            /**
             * Load config information 
             *
             * @param ap_fileName - the location of the config file
             * @bool - was the load successfull
             */
            virtual bool configure( const char* ap_fileName );

            /**
             * Insert a cache object into the container 
             *
             * @param ap_object - the object to insert
             * @return - did the insert work
             */
            virtual bool insert ( IDCMKey* ap_key, IDCMValue* ap_object );

           
            /**
             * Return a cache object if the key is found
             *
             * @param ap_key - the cache key
             * @param ap_value - the value to populate
             * @return bool - true if found, false otherwise
             */
            virtual bool find ( IDCMKey* ap_key, IDCMValue* ap_value );

            /**
             * Remove an object from the container
             *
             * @param ap_key - the key object to remove
             * @return was the object found in the map
             */
            virtual bool remove ( IDCMKey* ap_key );

            /**
             * Returns the size of the cache container
             *
             * @return the container size
             */
            virtual uint64_t size(); 

            /**
             * Returns the size of the mapped file object
             */
            virtual uint64_t fileSize();

            /**
             * Returns the size of the mapped file free memory
             */
            virtual uint64_t fileFreeMem();

            /**
             * Setters and getters for hte file location and initial size
             */
            inline void         setFileName ( const char* ap_fileName ) { m_fileName = ap_fileName; }
            inline void         setFileSize ( size_t a_fileSize )       { m_fileSize = a_fileSize; }
            inline const char*  getFileName ()                          { return m_fileName.c_str(); }
            inline size_t       getFileSize ()                          { return m_fileSize; }
            
            static boost::interprocess::managed_mapped_file* getFile () { return mp_file; }

            /**
             *  Getter for the container object held
             *
             */
            inline IDCMContainer* getContainer()                        { return mp_container; }
        protected:

            /**
             * This function is responsible for initializing the file object
             *
             * @param ap_filename - the path to file that should be created
             *   if NULL, the configured filename will be used
             * @return was the file opened
             */
            virtual bool openFile ( const char* ap_filename = NULL);

            /**
             * This function closes down the file
             *
             * @return was the file closed
             */
            bool closeFile(); 

            /**
             * This function will resize the cache file with a larger size
             *
             */
            virtual bool resize(); 

            /* The mmapped file */
            static boost::interprocess::managed_mapped_file*     mp_file;

            /* The name of the container type */
            std::string                     m_containerTypeString;
            
            /* The container object */
            IDCMContainer*            mp_container;

            /* Helper object used to configure members */
            IDCMConfigLoader* mp_config;
        private:
            
            /* The bin db file location */
            std::string                     m_fileName;
            
            /* The current size of the file */
            uint64_t                        m_fileSize;

            /* The container type */
            tDCMContainerType         m_containerType;

             

    };

};
#endif
