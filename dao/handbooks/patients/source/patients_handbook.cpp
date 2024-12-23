#include "patients_handbook.hpp"

namespace drug_lib::dao
{
    void PatientsHandbook::tear_down()
    {
    }

    void PatientsHandbook::setup() &
    {
        table_name_ = table_names::patients;
        //creating field
        const auto gender_field = common::database::make_field_shared<std::string>(
            objects::patient::field_name::gender);
        const auto birth_date_field = common::database::make_field_shared<
            std::chrono::system_clock::time_point>(
            objects::patient::field_name::birth_date);
        const auto contact_information_field = common::database::make_field_shared<std::string>(
            objects::patient::field_name::contact_information);
        const auto name_field = common::database::make_field_shared<std::string>(
                            objects::patient::field_name::name);
        value_fields_.push_back(gender_field);
        value_fields_.push_back(birth_date_field);
        value_fields_.push_back(contact_information_field);
        value_fields_.push_back(name_field);
        HandbookBase::setup();
    }
}
