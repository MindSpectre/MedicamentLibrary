##############################################################################
# Internal services
##############################################################################

set(InternalLibs
        DrugLib_Dao_SuperHandbook
        DrugLib_Common_Database_Factory
        DrugLib_Common_HashCreator_Factory)

add_subdirectory(lookup)
add_subdirectory(librarian)
add_subdirectory(treatment_manager)

add_subdirectory(patient_info_editor)

add_subdirectory(authenticator)