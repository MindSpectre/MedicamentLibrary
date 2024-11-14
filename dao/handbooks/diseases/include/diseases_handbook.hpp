#pragma once

#include "handbook.hpp"
#include "disease.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class DiseaseHandbook final : public HandbookBase<objects::Disease>
    {
    public:
        void tear_down() override;

    private:
        void setup(std::shared_ptr<common::database::interfaces::DbInterface> client) & override;
    };
}
