
##############################################################################
# Test Disease Base Class
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Disease_Base
        disease/test_disease_base.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Disease_Base
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(UnitTestObjectDiseaseBase ${UNIT_TESTING_TARGET}_Objects_Disease_Base)
##############################################################################

##############################################################################
# Test Disease Properties Classes
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Disease_Properties
        disease/properties/test_symptoms.hpp
        disease/properties/properties_test.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Disease_Properties
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(UnitTestObjectDiseaseProperties ${UNIT_TESTING_TARGET}_Objects_Disease_Properties)
##############################################################################


##############################################################################
# Test Medicament Base Class
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Medicament_Base
        medicament/test_medicament_base.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Medicament_Base
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(UnitTestObjectMedicamentBase ${UNIT_TESTING_TARGET}_Objects_Medicament_Base)
##############################################################################

##############################################################################
# Test Medicament Properties Classes
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Medicament_Properties
        medicament/properties/properties_test.cpp
        medicament/properties/test_prescription_drug.hpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Medicament_Properties
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(UnitTestObjectMedicamentProperties ${UNIT_TESTING_TARGET}_Objects_Medicament_Properties)
##############################################################################


##############################################################################
# Test Patient Class
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Patient_Base
        patient/test_patient_base.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Patient_Base
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(UnitTestObjectPatientBase ${UNIT_TESTING_TARGET}_Objects_Patient_Base)
##############################################################################

##############################################################################
# Test Patient Properties Classes
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Patient_Properties
        patient/properties/properties_test.cpp
        patient/properties/test_current_diseases.hpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Patient_Properties
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(UnitTestObjectPatientProperties ${UNIT_TESTING_TARGET}_Objects_Patient_Properties)
##############################################################################


##############################################################################
# Test Organization Base Class
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Organization_Base
        organization/test_organization_base.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Organization_Base
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(UnitTestObjectOrganizationBase ${UNIT_TESTING_TARGET}_Objects_Organization_Base)
##############################################################################

##############################################################################
# Test Organization Properties Classes
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Organization_Properties
        organization/properties/properties_test.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Organization_Properties
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(UnitTestObjectOrganizationProperties ${UNIT_TESTING_TARGET}_Objects_Organization_Properties)
##############################################################################


##############################################################################
# Test Property Collection
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_PropertyCollection
        support_objects/property_collection_test.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_PropertyCollection
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(UnitTestObjectPropertyCollection ${UNIT_TESTING_TARGET}_Objects_PropertyCollection)
##############################################################################