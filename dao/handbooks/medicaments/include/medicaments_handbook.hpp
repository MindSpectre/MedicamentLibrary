#pragma once

#include "handbook.hpp"
#include "medicament.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class MedicamentsHandbook final : public HandbookBase<objects::Medicament>
    {
    public:
        void tear_down() override;

    private:
        void setup(std::shared_ptr<common::database::interfaces::DbInterface> client) & override;
    };
}
