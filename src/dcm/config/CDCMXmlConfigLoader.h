#ifndef CDCMXmlConfigLoader_h
#define CDCMXmlConfigLoader_h

#include "IDCMConfigLoader.h"

#include <stdint.h>
#include <string>

class IXmlNode;
class CXmlNode;

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{
    /**
     * This object provides a config implementation that can load
     * XML based values
     */
    class CDCMXmlConfigLoader: public IDCMConfigLoader
    {
        public:
            /*
             * Constructor
             */
            CDCMXmlConfigLoader();

            /**
             * Destructor
             */
            virtual ~CDCMXmlConfigLoader();

            /**
             * Loads the config file specified into an IXmlNode object
             *
             * @param ap_file - the file location
             */
            virtual bool loadConfig ( const char* ap_file );

            /**
             * Loads a uint64 value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param a_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , uint64_t& ar_value, uint64_t a_default ); 

            /**
             * Loads a int64 value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param a_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , int64_t& ar_value, int64_t a_default ); 

            /**
             * Loads a uint32 value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param a_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , uint32_t& ar_value, uint32_t a_default ); 

            /**
             * Loads a int32 value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param a_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , int32_t& ar_value, int32_t a_default ); 

            /**
             * Loads a boolean value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param a_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , bool& ar_value, bool a_default ); 

            /**
             * Loads a string value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param ar_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , std::string& ar_value, std::string& ar_default ); 


        private:

            /**
             * The Root node for the loaded file
             */
            CXmlNode* mp_root;

            /**
             * The current node 
             */
            CXmlNode* mp_currentNode; 
    };

};
#endif

