#ifndef IDCMContainer_h
#define IDCMContainer_h

#include "boost/interprocess/managed_mapped_file.hpp"        
#include <boost/interprocess/sync/scoped_lock.hpp>    

using boost::interprocess::managed_mapped_file;
using boost::interprocess::scoped_lock;

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{

    class IDCMKey;
    class IDCMValue;

    /**
     * This object provides an iterface for storing keys and values in the
     * shared memory space
     */
    class IDCMContainer
    {
        public:
            IDCMContainer( const char* ap_name="");

            virtual ~IDCMContainer();

            virtual void initializeContainer ( managed_mapped_file* ap_file ) =0;

            virtual bool insert ( IDCMKey* ap_key, IDCMValue* ap_value ) =0;

            virtual bool find ( IDCMKey* ap_key, IDCMValue* ap_value ) =0;

            virtual bool remove ( IDCMKey* ap_key ) =0;

            virtual bool purge () =0;

            virtual uint64_t size() =0;

            virtual const char* getName();

        protected:
            typedef boost::interprocess::interprocess_mutex tDCMMutex;

            /* The handle that links the stl object to the cached file */
            managed_mapped_file::handle_t m_handle; 
        
            /*
             * named object created in mmapped space
             *  NOTE: unique names allows for multiple
             *  instances of same object
             */
            std::string m_containerName;
            
            /**
             * Mutex used for concurrent inserts and removes.
             */
            tDCMMutex* mp_mutex;

            /**
             * Mutex handled use to find the mutex if te file resizes
             */
            managed_mapped_file::handle_t m_mutexHandle; 
             

    };

};
#endif
