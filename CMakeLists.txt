cmake_minimum_required( VERSION "3.2.0" )

project( mutable CXX C )

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

FIND_LIBRARY(RDKAFKA rdkafka /usr/local/lib NO_DEFAULT_PATH)
FIND_LIBRARY(CPPKAFKA cppkafka /usr/local/lib NO_DEFAULT_PATH)

link_libraries( ${RDKAFKA} ${CPPKAFKA} )


find_path(HIREDIS_HEADER hiredis)
find_library(HIREDIS_LIB hiredis)

find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)


set( MUTABLE_COMPILE_OPTIONS
    -Wall
    -Wextra
    -O3
    -std=c++17
)


if( OPENSSL_FOUND )
	include_directories(        
	    src
        /usr/local/include/mongocxx/v_noabi
        /usr/local/include/bsoncxx/v_noabi
	    ${OPENSSL_INCLUDE_DIRS}
        lib/mysql-modern-cpp
        ${HIREDIS_HEADER}
        ${REDIS_PLUS_PLUS_HEADER}
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

    mongocxx
    bsoncxx
    sasl2
    ssl
    

    mysqlclient
    mysqlcppconn

    rdkafka
    cppkafka
    dl
    z

    hiredis
    redis++
    pthread  
)

set( MUTABLE_LIBRARY_SOURCES

    src/Mutable.cc

    src/clients/PostgresClient.cc
    src/clients/MongoClient.cc
    src/clients/MysqlClient.cc
    src/clients/ElasticSearchClient.cc
    src/clients/RedisClient.cc
    src/clients/KafkaProducer.cc
    src/clients/KafkaConsumer.cc

    src/consumers/ControlConsumer.cc
    src/consumers/MutatorConsumer.cc
    src/consumers/WalkerConsumer.cc

    src/tailers/MongoTailer.cc

    src/utils/File.cc
    src/utils/Json.cc
    src/utils/Common.cc
    src/utils/Url.cc

    src/chains/Chain.cc

    src/chains/mutable/MutableChain.cc

)

add_library( mutable_static STATIC ${MUTABLE_LIBRARY_SOURCES} )
set_target_properties( mutable_static
    PROPERTIES
    COMPILE_OPTIONS          "${MUTABLE_COMPILE_OPTIONS}"
)
target_link_libraries( mutable_static ${MUTABLE_LIBS} ${HIREDIS_LIB} ${REDIS_PLUS_PLUS_LIB} )


add_executable( mutable src/main.cc )
set_target_properties( mutable
    PROPERTIES
    COMPILE_OPTIONS          "${MUTABLE_COMPILE_OPTIONS}"
)
target_link_libraries( mutable mutable_static ${OPENSSL_LIBRARIES} ${HIREDIS_LIB} ${REDIS_PLUS_PLUS_LIB} )



