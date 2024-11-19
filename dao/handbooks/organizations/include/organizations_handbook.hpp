#pragma once

#include "handbook_base.hpp"
#include "organization.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class OrganizationsHandbook final : public HandbookBase<objects::Organization>
    {
    public:

    private:
        void tear_down() override;
        void setup() & override;
    };
}
