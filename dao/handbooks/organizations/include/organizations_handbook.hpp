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
        void update_all_fields(const objects::Organization& record) override;
        void update_all_fields(const std::vector<objects::Organization>& records) override;
        OrganizationsHandbook()
        {
            table_name_ = handbook_tables_name::Organizations;
            setup_constraints();
            fts_fields_ = {

            };
            connect_->setup_full_text_search(table_name_, fts_fields_);
        }
    };
}
