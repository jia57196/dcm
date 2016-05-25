#API level build file. 
cmake_minimum_required(VERSION 2.8.3)
cmake_policy(SET CMP0011 NEW)

if ( NOT DEFINED DCM_CORE_CMAKE )
    set ( DCM_CORE_CMAKE 1 )

    message("Configuring distributed cache - root")

    #This will be the name for the library
    project(DCM_CORE)

    #This file will set up the global directory variables
    include ( ${CMAKE_CURRENT_LIST_DIR}/../../build/CMakePaths.txt )

    #include the macros file
    include ( ${DCM_CORE_DIR}/CMakeMacros.txt )


    #Sub directory listings
    set ( BASE_DIR  ${DCM_CORE_DIR}/base )
    set ( CONFIG_DIR ${DCM_CORE_DIR}/config )
    set ( CONTAINERS_DIR ${DCM_CORE_DIR}/containers )
    set ( KEYS_DIR ${DCM_CORE_DIR}/keys )
    set ( VALUES_DIR ${DCM_CORE_DIR}/values )

    #Add in the subdirectory source files
    include ( ${BASE_DIR}/CMakeSources.txt )
    include ( ${CONFIG_DIR}/CMakeSources.txt )
    include ( ${CONTAINERS_DIR}/CMakeSources.txt )
    include ( ${KEYS_DIR}/CMakeSources.txt )
    include ( ${VALUES_DIR}/CMakeSources.txt )

    #Include the actuall subdirectories so the build doesn't break
    include_directories ( ${BASE_DIR};
                          ${CONFIG_DIR};
                          ${CONTAINERS_DIR};
                          ${KEYS_DIR};
                          ${VALUES_DIR};
                          ${BOOST_INCLUDE_DIR};
                          ${BOOST_LIB_DIR};
                          ${XML_CONFIG_PARSER_INCLUDE_DIR} )
                          
    link_directories ( 
                       ${XML_CONFIG_PARSER_LIB_DIR};
                       ${XERCES_LIB_DIR};     
                      )

    #Set up the sources that we will use for this libary
    set ( DCM_CORE_SOURCES 
            ${DCM_BASE_SOURCES};
            ${DCM_CONFIG_SOURCES};
            ${DCM_CONTAINERS_SOURCES};
            ${DCM_KEYS_SOURCES};
            ${DCM_VALUES_SOURCES}
        )

    set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" )
    add_library ( dcm SHARED ${DCM_CORE_SOURCES} )

    target_link_libraries ( dcm
                            ${XERCES_LIB_DIR}/libxerces-c.so.27;
                            ${XML_CONFIG_PARSER_LIB_DIR}/libXmlConfigParser_ix86_64-linux-sles11.so 
                           )

endif()
