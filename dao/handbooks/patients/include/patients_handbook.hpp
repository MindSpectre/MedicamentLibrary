#pragma once

#include "handbook_base.hpp"
#include "patient.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class PatientsHandbook final : public HandbookBase<objects::Patient>
    {
    public:
        PatientsHandbook() = default;

        explicit PatientsHandbook(std::shared_ptr<common::database::interfaces::DbInterface> connect)
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
