#pragma once

#include "handbook.hpp"
#include "patient.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class PatientsHandbook final : public HandbookBase<objects::Patient>
    {
    public:
        void tear_down() override;

    private:
        void setup(std::shared_ptr<common::database::interfaces::DbInterface> client) & override;
    };
}
