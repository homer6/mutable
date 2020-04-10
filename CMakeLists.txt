cmake_minimum_required( VERSION "3.2.0" )

project( mutable CXX C )

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)


set( MUTABLE_COMPILE_OPTIONS
    -Wall
    -Wextra
    -O3
    -std=c++14
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
    pqxx
    pq    
    crypto
    ${OPENSSL_LIBRARIES}
)

set( MUTABLE_LIBRARY_SOURCES

    src/Mutable.cc

    src/clients/PostgresClient.cc

    src/mutators/Mutator.cc

    src/chains/Chain.cc

    src/chains/mutable/MutableChain.cc

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



