#pragma once

#include "handbook.hpp"
#include "patient.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class PatientsHandbook final : public HandbookBase<objects::Patient>
    {
    public:
        ~PatientsHandbook() override;
        void remove_all() override;
        std::vector<objects::Patient> get_all() override;

        PatientsHandbook()
        {
            table_name_ = handbook_tables_name::Patients;
        }
    };
}
