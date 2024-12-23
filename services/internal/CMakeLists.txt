##############################################################################
# Internal services
##############################################################################

set(InternalLibs
        DrugLib_Dao_SuperHandbook
        DrugLib_Common_Database_Factory
        DrugLib_Common_Crypto
        DrugLib_Common_Utilities
        DrugLib_Common_Stopwatch
)

add_subdirectory(search)
add_subdirectory(librarian)
add_subdirectory(treatment_manager)

add_subdirectory(patient_info_editor)

add_subdirectory(authenticator)