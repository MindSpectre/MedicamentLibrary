
list(APPEND DbNecessaryLibs "DrugLib_Common_Database_Base"
        "DrugLib_Common_Database_PqxxClient"
        "DrugLib_Common_Database_Factory"
        "DrugLib_Common_Database_Pool"
)
add_subdirectory(handbooks)

add_library(DrugLib_Dao_AuthDataHolder
        auth_data_holder/auth_data_holder.cpp
        auth_data_holder/auth_data_holder.hpp
)
target_link_libraries(DrugLib_Dao_AuthDataHolder
        PUBLIC
        ${DbNecessaryLibs}
        DrugLib_Data_AuthObject
)
target_include_directories(DrugLib_Dao_AuthDataHolder
    PUBLIC
        auth_data_holder/
)
