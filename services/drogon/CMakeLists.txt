set(NecessaryDrogonLibs
        Drogon::Drogon
        DrugLib_Services_Drogon_Config_Utils
)


add_subdirectory(config_utils)

add_subdirectory(search_service)

add_subdirectory(librarian_service)

add_subdirectory(test_service)

add_subdirectory(gateway)
