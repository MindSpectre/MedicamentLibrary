#include "medicaments_handbook.hpp"
#include <pqxx/except>

namespace drug_lib::dao
{
    void MedicamentsHandbook::tear_down()
    {
    }


    void MedicamentsHandbook::setup(std::shared_ptr<common::database::interfaces::DbInterface> client) &
    {
        table_name_ = handbook_tables_name::Medicaments;
        //creating fields
        const auto infectious_field = common::database::make_field_shared_by_default<bool>(
            objects::Medicament::field_name::requires_prescription);
        const auto type_field = common::database::make_field_shared_by_default<std::string>(
            objects::Medicament::field_name::type);
        const auto manufacturer_id_field = common::database::make_field_shared_by_default<int32_t>(
            objects::Medicament::field_name::manufacturer_id);

        value_fields_.push_back(type_field);
        value_fields_.push_back(infectious_field);
        value_fields_.push_back(manufacturer_id_field);
        HandbookBase::setup(std::move(client));
    }
}
