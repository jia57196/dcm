cmake_minimum_required( VERSION 2.8.3 )

if ( NOT DEFINED DCM_CONFIG_CMAKE )
set ( DCM_CONFIG_CMAKE 1 )

message("Configuring distributed cache - config")

project ( DCM_CONFIG )

# Include the global locations file
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakePaths.txt )

set( DCM_CONFIG_SOURCE_DIR ${DCM_CONFIG_BASE_DIR} )

# Include the list of source files here
include ( ${DCM_CONFIG_SOURCE_DIR}/CMakeSources.txt )

#Creates statis and shared libraries
add_library ( dcmconfig SHARED ${DCM_CONFIG_SOURCES} )

target_link_libraries( dcmconfig
${XML_CONFIG_PARSER_LIB_DIR}/libXmlConfigParser_ix86_64-linux-sles11.so )

target_include_directories ( dcmconfig PUBLIC include )

endif()

