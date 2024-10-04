# PARENT_PROJECT_NAME = DrugLib.Common

set(TARGET ${COMMON_LIBS}_HashCreator)

##############################################################################
# Hash Creator Interface
##############################################################################
add_library(${TARGET}_Interface INTERFACE)

target_include_directories(${TARGET}_Interface
        INTERFACE
        interface
)

##############################################################################
# Hash Utilities: SHA256
##############################################################################
add_library(${TARGET}_SHA256
        sha_256/source/sha_256.cpp
)

target_include_directories(${TARGET}_SHA256 PUBLIC
        sha_256/include
        sha_256/source
)

target_link_libraries(${TARGET}_SHA256
        PUBLIC
        ${TARGET}_Interface
        OpenSSL::SSL
        OpenSSL::Crypto
)

##############################################################################

##############################################################################
# Hash Creator Factory
##############################################################################
add_library(${TARGET}_Factory STATIC
        factory/hash_creator_factory.cpp
        interface/hash_creator_interface.hpp
)

target_include_directories(${TARGET}_Factory PUBLIC
        factory
)

target_link_libraries(${TARGET}_Factory
        PUBLIC
        ${TARGET}_SHA256
        ${TARGET}_Interface
)
##############################################################################