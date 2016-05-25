
#if !defined(DCM_CBITMASK_H)
#define DCM_CBITMASK_H

#include <stdint.h>

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{

    class CBitmaskTest;
    class CSessionInfoTest;
    class CSubscriberIntoTest;
    class CEnrichmentInfoTest;

    /**
     * This object provides a common way to deliver
     * bitmasked functionality
     */
    class CBitmask
    {
        public:
            /*
             * Constructor
             */
            CBitmask();

            /*
             * Destructor
             */
            //
            // TODO: to test base and derived class destructor
            //
            ~CBitmask();

            /*
             * Copy constructor
             *
             * @param ar_right - Righthand argument
             */
            CBitmask(const CBitmask& ar_right);

            /*
             * Operator=
             *
             * @param ar_right - Righthand argument
             * @return CBitmask& - The Object
             */
            CBitmask& operator=(const CBitmask& ar_right);
            
            /*
             * isBitmaskSet - to check bitmask whether set or not
             *
             * @param a_bitmask - bitmask which requests to check
             *
             * @return bool - true if set, otherwise false
             */
        public:
            bool isBitmaskSet(uint32_t a_bitmask)
            {
                bool ret = false;

                if((m_bitmask & a_bitmask) == a_bitmask)
                {
                    ret = true;
                }

                return ret;
            }

            /*
             * setBitmask - to set corresponding bit
             *
             * @param - the bitmask need to be set
             *
             * @return void
             */
            void setBitmask(uint32_t a_bitmask)
            {
                m_bitmask |= a_bitmask;
            }

            /*
             * unsetBitmask - to clear corresponding bit
             *
             * @param - the bitmask need to be clear
             *
             * @return void
             */
            void unsetBitmask(uint32_t a_bitmask)
            {
                m_bitmask &= ~a_bitmask;
            }

            /*
             * clear - set bitmask to zero
             */
            void clear()
            {
                m_bitmask = 0;
            }

        protected:
            uint32_t m_bitmask;

        private:

            friend class CBitmaskTest;
            friend class CSessionInfoTest;
            friend class CSubscriberInfoTest;
            friend class CEnrichmentInfoTest;
    };

};

#endif //DCM_CBITMASK_H

