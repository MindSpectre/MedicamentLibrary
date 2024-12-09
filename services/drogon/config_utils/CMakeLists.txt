add_library(DrugLib_Services_Drogon_Config_Utils
        include/config_utils.hpp
        source/config_utils.cpp
)

target_include_directories(DrugLib_Services_Drogon_Config_Utils
        PUBLIC
        include/
)

target_link_libraries(DrugLib_Services_Drogon_Config_Utils
        PUBLIC
        JsonCpp::JsonCpp
        DrugLib_Common_Database_Factory
)