#pragma once

#include "handbook_base.hpp"
#include "patient.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class PatientsHandbook final : public HandbookBase<objects::Patient>
    {
    public:

    private:
        void tear_down() override;
        void setup() & override;
    };
}
