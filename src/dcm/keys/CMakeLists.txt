cmake_minimum_required( VERSION 2.8.3 )

if ( NOT DEFINED DCM_KEYS_CMAKE )
    set ( DCM_KEYS_CMAKE 1 )

    message("Configuring distributed cache - keys")

    project ( DCM_KEYS )

    # Include the global locations file
    include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakePaths.txt )

    # Include the list of source files here
    include ( ${DCM_CORE_KEYS_DIR}/CMakeSources.txt )

    #Creates statis and shared libraries
    add_library ( dcmkeys SHARED ${DCM_KEYS_SOURCES} )

endif()
