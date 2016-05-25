cmake_minimum_required( VERSION 2.8.3 )

if ( NOT DEFINED DCM_VALUES_CMAKE )
    set ( DCM_VALUES_CMAKE 1 )

    message("Configuring distributed cache - values")

    project ( DCM_VALUES )

    # Include the global locations file
    include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakePaths.txt )

    # Include the list of source files here
    include ( ${DCM_CORE_VALUES_DIR}/CMakeSources.txt )

    #Creates statis and shared libraries
    add_library ( dcmvalues SHARED ${DCM_VALUES_SOURCES} )

endif()
