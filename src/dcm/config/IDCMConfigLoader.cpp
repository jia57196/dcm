#include "IDCMConfigLoader.h"

namespace NDCM
{
    //-----------------------------------------------------------------------//
    IDCMConfigLoader::IDCMConfigLoader()
    : m_fileName ("")
    {
    }

    //-----------------------------------------------------------------------//
    IDCMConfigLoader::~IDCMConfigLoader()
    {
    }

    //-----------------------------------------------------------------------//
    const char* IDCMConfigLoader::getFileLocation()
    {
        return m_fileName.c_str();
    }    

};
