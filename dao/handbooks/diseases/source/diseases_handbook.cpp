#include "diseases_handbook.hpp"

namespace drug_lib::dao
{
    void DiseaseHandbook::setup(std::shared_ptr<common::database::interfaces::DbInterface> client) &
    {
        table_name_ = handbook_tables_name::Diseases;
        //creating fields
        const auto infectious_field = common::database::make_field_shared_by_default<bool>(
            objects::Disease::field_name::is_infectious);
        const auto type_field = common::database::make_field_shared_by_default<std::string>(
            objects::Disease::field_name::type);

        value_fields_.push_back(type_field);
        value_fields_.push_back(infectious_field);

        HandbookBase::setup(std::move(client));
    }
}
