#include "patients_handbook.hpp"

namespace drug_lib::dao
{
    void PatientsHandbook::setup() &
    {
        table_name_ = handbook_tables_name::Patients;
        //creating field
        const auto gender_field = common::database::make_field_shared_by_default<std::string>(
            objects::Patient::field_name::gender);
        const auto birth_date_field = common::database::make_field_shared_by_default<
            std::chrono::system_clock::time_point>(
            objects::Patient::field_name::birth_date);
        const auto contact_information_field = common::database::make_field_shared_by_default<std::string>(
            objects::Patient::field_name::contact_information);

        value_fields_.push_back(gender_field);
        value_fields_.push_back(birth_date_field);
        value_fields_.push_back(contact_information_field);

        HandbookBase::setup();
    }
}
