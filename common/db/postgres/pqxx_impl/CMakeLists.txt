# PARENT_PROJECT_NAME = DrugLib.Common.Database
set(TARGET ${PARENT_PROJECT_NAME}.PqxxClient)

##############################################################################
# Postgres Database client implementation used on PQXX lib ONLY FOR C++
##############################################################################
add_library(${TARGET}
        source/pqxx_client.cpp
)

target_link_libraries(${TARGET}
        JsonCpp::JsonCpp
        DrugLib.Common.Database.Interface
        DrugLib.Common.Database.Exceptions
        DrugLib.Common.Database.Behavioral.OperationsStrategies
        libpqxx::pqxx
)

target_include_directories(${TARGET}
        PUBLIC
        source/
        include/
        ${CMAKE_HOME_DIRECTORY}/common
)
##############################################################################
