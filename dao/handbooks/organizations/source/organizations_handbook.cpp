#include "organizations_handbook.hpp"

namespace drug_lib::dao
{
    void OrganizationsHandbook::tear_down()
    {
    }

    void OrganizationsHandbook::setup() &
    {
        table_name_ = handbook_tables_name::Organizations;
        const auto country_field = common::database::make_field_shared_by_default<std::string>(
            objects::Organization::field_name::country);
        const auto type_field = common::database::make_field_shared_by_default<std::string>(
            objects::Organization::field_name::type);
        const auto contact_details_field = common::database::make_field_shared_by_default<std::string>(
            objects::Organization::field_name::contact_details);

        value_fields_.push_back(type_field);
        value_fields_.push_back(country_field);
        value_fields_.push_back(contact_details_field);

        HandbookBase::setup();
    }
}
