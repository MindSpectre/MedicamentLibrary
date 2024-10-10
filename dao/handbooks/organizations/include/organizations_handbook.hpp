#pragma once

#include "handbook.hpp"
#include "organization.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class OrganizationsHandbook final : public HandbookBase<objects::Organization>
    {
    public:
        ~OrganizationsHandbook() override;
        void remove_all() override;
        std::vector<objects::Organization> get_all() override;

        OrganizationsHandbook()
        {
            table_name_ = handbook_tables_name::Organizations;
        }
    };
}
