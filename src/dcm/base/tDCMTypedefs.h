#ifndef tDCMTypedefs_h
#define tDCMTypedefs_h

#include <map>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/smart_ptr/weak_ptr.hpp> 

using boost::interprocess::managed_mapped_file;
using boost::interprocess::allocator;
using boost::interprocess::managed_weak_ptr;

//Raw Type
typedef managed_weak_ptr  <uint8_t, managed_mapped_file>::type  tDCMManagedRawUint8Array; 


/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{

    //Allocators
    typedef boost::interprocess::allocator < void, managed_mapped_file::segment_manager  > tDCMVoidAllocator;
    typedef boost::interprocess::allocator < uint8_t, managed_mapped_file::segment_manager  > tDCMUint8Allocator;
    typedef boost::interprocess::allocator< tDCMManagedRawUint8Array, managed_mapped_file::segment_manager>    tDCMManagedRawUint8ArrayAllocator; 

    typedef enum
    {
        eMap,
        eHashMap,
        eSet,
        eHashSet,
        eMultiMap
    } tDCMContainerType;
            
    typedef enum
    {
        eFailed,
        eInserted,
        eUpdated,
        eFound,
        eRemoved
    } tDCMInsertStatus;

};
#endif
