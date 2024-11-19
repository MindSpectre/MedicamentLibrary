#pragma once

#include "handbook_base.hpp"
#include "disease.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class DiseaseHandbook final : public HandbookBase<objects::Disease>
    {
    public:

    private:
        void tear_down() override;
        void setup() & override;
    };
}
