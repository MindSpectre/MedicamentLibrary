add_library(DrugLib_Common_Crypto INTERFACE)
target_link_libraries(DrugLib_Common_Crypto INTERFACE
    DrugLib_Common_HashCreator_Factory
    DrugLib_Common_SaltGenerator
)
##############################################################################
# Hash Creator Interface
##############################################################################
add_library(DrugLib_Common_HashCreator_Interface INTERFACE
        hash_creator/interface/hash_creator_interface.hpp
)

target_include_directories(DrugLib_Common_HashCreator_Interface
        INTERFACE
        hash_creator/interface
)

##############################################################################
# Hash Utilities: SHA256
##############################################################################
add_library(DrugLib_Common_HashCreator_SHA256
        hash_creator/sha_256/source/sha_256.cpp
        hash_creator/sha_256/include/sha_256.h
)

target_include_directories(DrugLib_Common_HashCreator_SHA256 PUBLIC
        hash_creator/sha_256/include
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
        hash_creator/pbkdf2/source/pbkdf2.cpp
        hash_creator/pbkdf2/include/pbkdf2.hpp
)

target_include_directories(DrugLib_Common_HashCreator_PBKDF2 PUBLIC
        hash_creator/pbkdf2/include
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
        hash_creator/factory/hash_creator_factory.cpp
        hash_creator/interface/hash_creator_interface.hpp
)

target_include_directories(DrugLib_Common_HashCreator_Factory PUBLIC
        hash_creator/factory
)

target_link_libraries(DrugLib_Common_HashCreator_Factory
        PUBLIC
        DrugLib_Common_HashCreator_SHA256
        DrugLib_Common_HashCreator_PBKDF2
        DrugLib_Common_HashCreator_Interface
)
##############################################################################


##############################################################################
# Salt Generator
##############################################################################
add_library(DrugLib_Common_SaltGenerator STATIC
        salt_generator/source/salt_generator.cpp
)

target_include_directories(DrugLib_Common_SaltGenerator PUBLIC
        salt_generator/include
)

target_link_libraries(DrugLib_Common_SaltGenerator
        PUBLIC
        OpenSSL::SSL
        OpenSSL::Crypto
)
##############################################################################