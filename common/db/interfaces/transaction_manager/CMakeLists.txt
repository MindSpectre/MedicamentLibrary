set(ProjectName "v7")
set(ProjectSubName "Common.Services")

add_library(${ProjectName}.${ProjectSubName}.TransactionManager
    Source/TransactionManager.cpp
)
target_include_directories(${ProjectName}.${ProjectSubName}.TransactionManager
    PUBLIC
    Source/
    Include/
)
target_link_libraries(${ProjectName}.${ProjectSubName}.TransactionManager
    JsonCpp::JsonCpp
    v7.Common.IStationDatabase
    v7.Common.IStationDatabaseFactory
    v7.Common.Services.Exceptions
)
