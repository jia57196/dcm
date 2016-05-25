#include "Common.h"

namespace NDCM
{
    //-----------------------------------------------------------------------//
    uint32_t hash32Shift(uint32_t a)
    {
        uint32_t x = a;

        x = ~x + (x << 15);
        x =  x ^ (x >> 12);
        x =  x + (x << 2);
        x =  x ^ (x >> 4);
        x =  x * 2057;
        x =  x ^ (x >> 16);

        return x;
    }

    //-----------------------------------------------------------------------//
    uint64_t hash64Shift(uint64_t a)
    {
        uint64_t x = a;

        x = ~x + (x << 21);
        x =  x ^ (x >> 24);
        x = (x + (x << 3)) + (x << 8);
        x =  x ^ (x >> 14);
        x = (x + (x << 2)) + (x << 4);
        x =  x ^ (x >> 28);
        x =  x + (x << 31);

        return x;
    }

    //-----------------------------------------------------------------------//
    uint32_t hashString(const char* ap_str)
    {
        uint32_t hash = 0;
        uint32_t ch   = 0;

        while(0 != (ch = *ap_str++))
        {
            hash = ch + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
    }

    //-----------------------------------------------------------------------//
    uint32_t hashOctets(const uint8_t* ap_oct, const uint32_t a_len)
    {
        uint32_t hash = 0;

        for(uint32_t i = 0; i < a_len; ++i)
        {
            hash = ap_oct[i] + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
    }

};
