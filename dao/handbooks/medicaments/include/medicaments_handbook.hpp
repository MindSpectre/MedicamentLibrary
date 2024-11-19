#pragma once

#include "handbook_base.hpp"
#include "medicament.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class MedicamentsHandbook final : public HandbookBase<objects::Medicament>
    {
    public:

    private:
        void tear_down() override;
        void setup() & override;
    };
}
