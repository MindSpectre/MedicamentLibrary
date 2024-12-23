#include "organizations_handbook.hpp"

namespace drug_lib::dao
{
    void OrganizationsHandbook::tear_down()
    {
    }

    void OrganizationsHandbook::setup() &
    {
        table_name_ = table_names::organizations;
        const auto country_field = common::database::make_field_shared<std::string>(
            objects::organization::field_name::country);
        const auto type_field = common::database::make_field_shared<std::string>(
            objects::organization::field_name::type);
        const auto contact_details_field = common::database::make_field_shared<std::string>(
            objects::organization::field_name::contact_details);

        value_fields_.push_back(type_field);
        value_fields_.push_back(country_field);
        value_fields_.push_back(contact_details_field);

        HandbookBase::setup();
    }
}
