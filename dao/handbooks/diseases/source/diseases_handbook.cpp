#include "diseases_handbook.hpp"

namespace drug_lib::dao
{
    void DiseaseHandbook::tear_down()
    {
    }

    void DiseaseHandbook::setup() &
    {
        fts_fields_.clear();
        key_fields_.clear();
        value_fields_.clear();
        table_name_ = table_names::diseases;
        //creating fields
        const auto infectious_field = common::database::make_field_shared<bool>(
            objects::disease::field_name::is_infectious);
        const auto type_field = common::database::make_field_shared<std::string>(
            objects::disease::field_name::type);
        const auto name_field = common::database::make_field_shared<std::string>(
                    objects::disease::field_name::name);
        value_fields_.push_back(type_field);
        value_fields_.push_back(infectious_field);
        value_fields_.push_back(name_field);
        HandbookBase::setup();
    }
}
