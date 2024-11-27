#pragma once

#include "handbook_base.hpp"
#include "organization.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class OrganizationsHandbook final : public HandbookBase<objects::Organization>
    {
    public:
        OrganizationsHandbook() = default;

        explicit OrganizationsHandbook(std::shared_ptr<common::database::interfaces::DbInterface> connect)
        {
            connect_ = std::move(connect);
            this->setup();
        }

        void set_connection(std::shared_ptr<common::database::interfaces::DbInterface> connect) override
        {
            connect_ = std::move(connect);
            this->setup();
        }

    private:
        void tear_down() override;
        void setup() & override;
    };
}
