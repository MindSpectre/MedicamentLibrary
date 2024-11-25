
##############################################################################
# Hash Creator Interface
##############################################################################
add_library(DrugLib_Common_HashCreator_Interface INTERFACE
        interface/hash_creator_interface.hpp
)

target_include_directories(DrugLib_Common_HashCreator_Interface
        INTERFACE
        interface
)

##############################################################################
# Hash Utilities: SHA256
##############################################################################
add_library(DrugLib_Common_HashCreator_SHA256
        sha_256/source/sha_256.cpp
        sha_256/include/sha_256.h
)

target_include_directories(DrugLib_Common_HashCreator_SHA256 PUBLIC
        sha_256/include
)

target_link_libraries(DrugLib_Common_HashCreator_SHA256
        PUBLIC
        DrugLib_Common_HashCreator_Interface
        OpenSSL::SSL
        OpenSSL::Crypto
)

##############################################################################

##############################################################################
# Hash Utilities: PBKDF2
##############################################################################
add_library(DrugLib_Common_HashCreator_PBKDF2
        pbkdf2/source/pbkdf2.cpp
        pbkdf2/include/pbkdf2.hpp
)

target_include_directories(DrugLib_Common_HashCreator_PBKDF2 PUBLIC
        pbkdf2/include
)

target_link_libraries(DrugLib_Common_HashCreator_PBKDF2
        PUBLIC
        DrugLib_Common_HashCreator_Interface
        OpenSSL::SSL
        OpenSSL::Crypto
)

##############################################################################

##############################################################################
# Hash Creator Factory
##############################################################################
add_library(DrugLib_Common_HashCreator_Factory STATIC
        factory/hash_creator_factory.cpp
        interface/hash_creator_interface.hpp
)

target_include_directories(DrugLib_Common_HashCreator_Factory PUBLIC
        factory
)

target_link_libraries(DrugLib_Common_HashCreator_Factory
        PUBLIC
        DrugLib_Common_HashCreator_SHA256
        DrugLib_Common_HashCreator_PBKDF2
        DrugLib_Common_HashCreator_Interface
)
##############################################################################