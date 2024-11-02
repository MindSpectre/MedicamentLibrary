#pragma once

#include "handbook.hpp"
#include "organization.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class OrganizationsHandbook final : public HandbookBase<objects::Organization>
    {
    public:
        void force_insert(const objects::Organization& record) override;
        void force_insert(const std::vector<objects::Organization>& records) override;
        void remove_by_id(int32_t id) override;
        void remove_by_name(const std::string& name) override;
        std::vector<objects::Organization> get_by_id(int32_t id) override;
        std::vector<objects::Organization> get_by_name(const std::string& name) override;
        std::vector<objects::Organization> search(const std::string& pattern) override;
        void tear_down() override;
        std::vector<objects::Organization> select() override;
    };
}
