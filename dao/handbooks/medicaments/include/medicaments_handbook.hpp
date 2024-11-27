#pragma once

#include "handbook_base.hpp"
#include "medicament.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class MedicamentsHandbook final : public HandbookBase<objects::Medicament>
    {
    public:
        MedicamentsHandbook() = default;

        explicit MedicamentsHandbook(std::shared_ptr<common::database::interfaces::DbInterface> connect)
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
