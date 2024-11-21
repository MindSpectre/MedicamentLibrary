add_executable(DrugLib_Tools_SampleDbFiller
        db_tools/filler/filler_with_easy_data.cpp
)

target_link_libraries(DrugLib_Tools_SampleDbFiller
        PRIVATE
        DrugLib_Dao_SuperHandbook
        DrugLib_Common_Database_Factory
)

add_executable(DrugLib_Tools_DropTables
        db_tools/dropper/drop_all_tables.cpp
)

target_link_libraries(DrugLib_Tools_DropTables
        PRIVATE
        DrugLib_Dao_SuperHandbook
        DrugLib_Common_Database_Factory
)