cmake_minimum_required( VERSION "2.8.0" )

project( mutable CXX C )

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)


set( MUTABLE_COMPILE_OPTIONS
    -Wall
    -Wextra
    -O3
    -std=c++17
)


if( OPENSSL_FOUND )
	include_directories(
	    src
	    ${OPENSSL_INCLUDE_DIRS}
	)
	link_directories( ${OPENSSL_LIBRARY_DIRS} ) 
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()


set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build )


set( MUTABLE_LIBS
    crypto
    ${OPENSSL_LIBRARIES}
)

set( MUTABLE_LIBRARY_SOURCES

    src/Mutable.cc

    src/mutators/Mutator.cc

)

add_library( mutable_static STATIC ${MUTABLE_LIBRARY_SOURCES} )
set_target_properties( mutable_static
    PROPERTIES
    COMPILE_OPTIONS          "${MUTABLE_COMPILE_OPTIONS}"
)
target_link_libraries( mutable_static ${MUTABLE_LIBS} )


add_executable( mutable src/main.cc )
set_target_properties( mutable
    PROPERTIES
    COMPILE_OPTIONS          "${WORKER_COMPILE_OPTIONS}"
)
target_link_libraries( mutable mutable_static ${OPENSSL_LIBRARIES} )



