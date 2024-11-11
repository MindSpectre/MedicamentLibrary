#pragma once

#include "handbook.hpp"
#include "types/organization.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class OrganizationsHandbook final : public HandbookBase<objects::Organization>
    {
    public:
        void tear_down() override;

    private:
        void setup(std::shared_ptr<common::database::interfaces::DbInterface> client) & override;
    };
}
