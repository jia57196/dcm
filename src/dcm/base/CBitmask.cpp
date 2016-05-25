#include <CBitmask.h>

namespace NDCM
{
    //-----------------------------------------------------------------------//
    CBitmask::CBitmask():
        m_bitmask(0)
    {

    }

    //-----------------------------------------------------------------------//
    CBitmask::~CBitmask()
    {
        m_bitmask = 0;
    }

    //-----------------------------------------------------------------------//
    CBitmask::CBitmask(const CBitmask& ar_right)
    {
        m_bitmask = ar_right.m_bitmask;
    }

    //-----------------------------------------------------------------------//
    CBitmask& CBitmask::operator=(const CBitmask& ar_right)
    {
        if(this != &ar_right)
        {
            m_bitmask = ar_right.m_bitmask;
        }

        return *this;
    }
};
