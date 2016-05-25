cmake_minimum_required( VERSION 2.6 )

if ( NOT DEFINED DCM_TEST_UTILS_CMAKE )
    set ( DCM_TEST_UTILS_CMAKE 1 )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -g" )

    
    project ( DCM_TEST_UTILS )

    # Include the global locations file
    include ( ../../../../build/CMakePaths.txt )

    # Include the list of source files here
    include ( ${DCM_TEST_UTILS_DIR}/CMakeSources.txt )
    
    # Include DCM libs
    include ( ${DCM_CORE_BASE_DIR}/CMakeLists.txt )
    include ( ${DCM_CORE_CONTAINERS_DIR}/CMakeLists.txt )
    include ( ${DCM_CORE_KEYS_DIR}/CMakeLists.txt )
    include ( ${DCM_CORE_VALUES_DIR}/CMakeLists.txt )
    include ( ${DCMKEY_BASE_DIR}/CMakeLists.txt )
    include ( ${DCMKEY_KEY_DIR}/CMakeLists.txt )
    include ( ${DCMKEY_VALUES_DIR}/CMakeLists.txt )
    include ( ${DCMKEY_C_BINDING_DIR}/CMakeLists.txt )

    # Add external library
    link_directories ( ${XERCES_LIB_DIR};
                       ${XML_CONFIG_PARSER_LIB_DIR};
                       ${BOOST_LIBS_DIR};
                     )

    # Creates executable
    set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DCM_TEST_UTILS_DIR}" )

    add_executable ( CDCMTestUtil ${DCM_TEST_UTILS_SOURCES} )

    add_executable ( CDCMTestCBinding ${DCM_TEST_CBINDING_SOURCES} )
    
    add_dependencies ( CDCMTestUtil
                       dcmbase
                       dcmcontainers
                       dcmkeys
                       dcmvalues
                       DcmKeyImpl
                       DcmValueImpl
                       enrichment )

    add_dependencies ( CDCMTestCBinding
                       enrichmentcbinding
                       dcmbase
                       dcmcontainers
                       dcmkeys
                       dcmvalues
                       DcmKeyImpl
                       DcmValueImpl
                       enrichment )

    target_link_libraries ( CDCMTestUtil
                            dcmbase
                            dcmcontainers
                            dcmkeys
                            dcmvalues
                            DcmKeyImpl
                            DcmValueImpl
                            enrichment
                            xerces-c
                            XmlConfigParser_ix86_64-linux-sles11.so )

    target_link_libraries ( CDCMTestCBinding
                            enrichmentcbinding
                            dcmbase
                            dcmcontainers
                            dcmkeys
                            dcmvalues
                            DcmKeyImpl
                            DcmValueImpl
                            enrichment
                            xerces-c
                            XmlConfigParser_ix86_64-linux-sles11.so )

endif()

