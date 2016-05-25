#ifndef IDCMConfigLoader_h
#define IDCMConfigLoader_h

#include <string>
#include <stdint.h>

/**
 * Namespace used to wrap DCM API Objects
 */
namespace NDCM
{

    /**
     * This object provides an interface for loading config into the DCM
     */
    class IDCMConfigLoader
    {
        public:
            /**
             * Constructor
             */
            IDCMConfigLoader();

            /**
             * Destructor
             */
            virtual ~IDCMConfigLoader();
            
            /**
             * Loads the config using the supplied file name
             *
             * @param ap_file - location of config file
             */
            virtual bool loadConfig ( const char* ap_file ) =0;

            /**
             * Loads a int64 value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param a_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , uint64_t& ar_value, uint64_t a_default ) =0; 

            /**
             * Loads a int64 value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param a_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , int64_t& ar_value, int64_t a_default ) =0; 

            /**
             * Loads a uint32 value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param a_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , uint32_t& ar_value, uint32_t a_default ) =0; 

            /**
             * Loads a int32 value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param a_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , int32_t& ar_value, int32_t a_default ) =0; 

            /**
             * Loads a boolean value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param a_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , bool& ar_value, bool a_default ) =0; 

            /**
             * Loads a string value from the current node
             *
             * @paran ar_configTag - the node name
             * @param ar_value - the variable to update
             * @param ar_default - the default of the variable
             * @return bool - true if param loaded, false if using default value
             */
            virtual bool loadValue ( std::string& ar_configTag , std::string& ar_value, std::string& ar_default ) =0; 

            /**
             * Return the file location
             */
            const char* getFileLocation();

        protected:

            /**
             * The file location
             */
            std::string m_fileName;

    };

};
#endif
