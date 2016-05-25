cmake_minimum_required( VERSION 2.8.3 )

if ( NOT DEFINED DCM_CONTAINERS_CMAKE )
    set ( DCM_CONTAINERS_CMAKE 1 )

    message("Configuring distributed cache - containers")

    project ( DCM_CONTAINERS )

    # Include the global locations file
    include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakePaths.txt )

    # Include the list of source files here
    include ( ${DCM_CORE_CONTAINERS_DIR}/CMakeSources.txt )

    #Creates statis and shared libraries
    add_library ( dcmcontainers SHARED ${DCM_CONTAINERS_SOURCES} )


endif()    
