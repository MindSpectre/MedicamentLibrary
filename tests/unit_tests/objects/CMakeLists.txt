set(UnitTestObject "UnitTest_Object")
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
add_test(${UnitTestObject}_DiseaseBase ${UNIT_TESTING_TARGET}_Objects_Disease_Base)
set_tests_properties(${UnitTestObject}_DiseaseBase PROPERTIES LABELS "unit")
##############################################################################

##############################################################################
# Test Disease Properties Classes
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Disease_Properties
        disease/properties/properties_test.cpp
        disease/properties/test_symptoms.cpp
        disease/properties/test_curative_drugs.cpp
        disease/properties/test_affected_age_groups.cpp
        disease/properties/test_risk_factors.cpp
        disease/properties/test_complications.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Disease_Properties
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(${UnitTestObject}_DiseaseProperties ${UNIT_TESTING_TARGET}_Objects_Disease_Properties)
set_tests_properties(${UnitTestObject}_DiseaseProperties PROPERTIES LABELS "unit")
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
add_test(${UnitTestObject}_MedicamentBase ${UNIT_TESTING_TARGET}_Objects_Medicament_Base)
set_tests_properties(${UnitTestObject}_MedicamentBase PROPERTIES LABELS "unit")
##############################################################################

##############################################################################
# Test Medicament Properties Classes
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Medicament_Properties
        medicament/properties/properties_test.cpp
        medicament/properties/test_prescription.cpp
        medicament/properties/test_active_ingredients.cpp
        medicament/properties/test_inactive_ingredients.cpp
        medicament/properties/test_dosage_form.cpp
        medicament/properties/test_side_effects.cpp
        medicament/properties/test_strength.cpp

)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Medicament_Properties
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(${UnitTestObject}_MedicamentProperties ${UNIT_TESTING_TARGET}_Objects_Medicament_Properties)
set_tests_properties(${UnitTestObject}_MedicamentProperties PROPERTIES LABELS "unit")
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
add_test(${UnitTestObject}_PatientBase ${UNIT_TESTING_TARGET}_Objects_Patient_Base)
set_tests_properties(${UnitTestObject}_PatientBase PROPERTIES LABELS "unit")
##############################################################################

##############################################################################
# Test Patient Properties Classes
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Patient_Properties
        patient/properties/properties_test.cpp
        patient/properties/test_current_diseases.cpp
        patient/properties/test_current_medicaments.cpp
        patient/properties/test_allergies.cpp
        patient/properties/test_blood_type.cpp
        patient/properties/test_insurance.cpp
        patient/properties/test_medical_history.cpp
        patient/properties/test_vaccines.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Patient_Properties
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(${UnitTestObject}_PatientProperties ${UNIT_TESTING_TARGET}_Objects_Patient_Properties)
set_tests_properties(${UnitTestObject}_PatientProperties PROPERTIES LABELS "unit")
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
add_test(${UnitTestObject}_OrganizationBase ${UNIT_TESTING_TARGET}_Objects_Organization_Base)
set_tests_properties(${UnitTestObject}_OrganizationBase PROPERTIES LABELS "unit")
##############################################################################

##############################################################################
# Test Organization Properties Classes
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_Objects_Organization_Properties
        organization/properties/properties_test.cpp
        organization/properties/test_license.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_Objects_Organization_Properties
        PRIVATE
        DrugLib_Data_Objects
        ${TEST_NECESSARY_LIBS}
)
add_test(${UnitTestObject}_OrganizationProperties ${UNIT_TESTING_TARGET}_Objects_Organization_Properties)
set_tests_properties(${UnitTestObject}_OrganizationProperties PROPERTIES LABELS "unit")
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
add_test(${UnitTestObject}_PropertyCollection ${UNIT_TESTING_TARGET}_Objects_PropertyCollection)
set_tests_properties(${UnitTestObject}_PropertyCollection PROPERTIES LABELS "unit")
##############################################################################