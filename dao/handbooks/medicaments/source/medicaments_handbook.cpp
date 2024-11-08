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
        auto id_f = common::database::Field(objects::Medicament::fields::id, 0);
        auto name_f = common::database::Field<std::string>(objects::Medicament::fields::name, "");
        auto prop_f = common::database::Field(objects::Medicament::fields::properties, Json::Value());
        fts_fields_.push_back(std::make_shared<common::database::Field<std::string>>(name_f));
        fts_fields_.push_back(std::make_shared<common::database::Field<Json::Value>>(prop_f));

        value_fields_.push_back(std::make_shared<common::database::Field<std::string>>(name_f));
        value_fields_.push_back(std::make_shared<common::database::Field<Json::Value>>(prop_f));

        key_fields_.push_back(std::make_shared<common::database::Field<int32_t>>(id_f));

        HandbookBase::setup(std::move(client));
    }
}
