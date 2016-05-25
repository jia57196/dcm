cmake_minimum_required( VERSION 2.8.3 )

if ( NOT DEFINED DCM_BASE_CMAKE )
    set ( DCM_BASE_CMAKE 1 )

    message("Configuring distributed cache - base")

    project ( DCM_BASE )

    # Include the global locations file
    include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakePaths.txt )

    # Include the list of source files here
    include ( ${DCM_CORE_BASE_DIR}/CMakeSources.txt )

    include_directories ( ${DCM_BASE_SOURCE_DIR};
                          ${DCM_CONFIG_BASE_DIR}; 
                          ${DCM_CORE_CONTAINERS_DIR}; 
                          ${XML_CONFIG_PARSER_INCLUDE_DIR} )

    #Creates shared libraries
    add_library ( dcmbase SHARED ${DCM_BASE_SOURCES} )

    include( ${DCM_CONFIG_BASE_DIR}/CMakeLists.txt )
    target_link_libraries( dcmbase
                            ${XML_CONFIG_PARSER_LIB_DIR}/libXmlConfigParser_ix86_64-linux-sles11.so;
                            dcmconfig )


endif()                        
