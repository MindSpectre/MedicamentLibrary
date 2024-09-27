# PARENT_PROJECT_NAME = DrugLib.Common

set(TARGET ${PARENT_PROJECT_NAME}.HashCreator)

##############################################################################
# Hash Creator Interface
##############################################################################
add_library(${TARGET}.Interface INTERFACE)

target_include_directories(${TARGET}.Interface
        INTERFACE
        interface
)

##############################################################################
# Hash Utilities: SHA256
##############################################################################
add_library(${TARGET}.sha_256
        sha_256/source/sha_256.cpp
)

target_include_directories(${TARGET}.sha_256 PUBLIC
        sha_256/include
        sha_256/source
)

target_link_libraries(${TARGET}.sha_256
        PUBLIC
        ${TARGET}.Interface
        OpenSSL::SSL
        OpenSSL::Crypto
)

##############################################################################

##############################################################################
# Hash Creator Factory
##############################################################################
add_library(${TARGET}.Factory STATIC
        factory/hash_creator_factory.cpp
)

target_include_directories(${TARGET}.Factory PUBLIC
        factory
)

target_link_libraries(${TARGET}.Factory
        PUBLIC
        ${TARGET}.sha_256
        ${TARGET}.Interface
)
##############################################################################