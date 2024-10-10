#pragma once

#include "handbook.hpp"
#include "medicament.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class MedicamentsHandbook final : public HandbookBase<objects::Medicament>
    {
    public:
        ~MedicamentsHandbook() override;
        void remove_all() override;
        std::vector<objects::Medicament> get_all() override;

        MedicamentsHandbook()
        {
            table_name_ = handbook_tables_name::Medicaments;
        }
    };
}
