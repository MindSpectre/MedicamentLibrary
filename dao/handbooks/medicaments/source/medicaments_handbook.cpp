#include "medicaments_handbook.hpp"
#include <pqxx/except>

namespace drug_lib::dao
{
    void MedicamentsHandbook::tear_down()
    {
    }


    void MedicamentsHandbook::setup() &
    {
        table_name_ = handbook_tables_name::medicaments;
        //creating fields
        const auto infectious_field = common::database::make_field_shared_by_default<bool>(
            objects::Medicament::field_name::requires_prescription);
        const auto type_field = common::database::make_field_shared_by_default<std::string>(
            objects::Medicament::field_name::type);
        const auto approval_status_field = common::database::make_field_shared_by_default<std::string>(
            objects::Medicament::field_name::approval_status);
        const auto approval_number_field = common::database::make_field_shared_by_default<std::string>(
            objects::Medicament::field_name::approval_number);
        const auto atc_code_field = common::database::make_field_shared_by_default<std::string>(
            objects::Medicament::field_name::atc_code);
        value_fields_.push_back(type_field);
        value_fields_.push_back(infectious_field);
        value_fields_.push_back(approval_status_field);
        value_fields_.push_back(approval_number_field);
        value_fields_.push_back(atc_code_field);
        HandbookBase::setup();
    }
}
