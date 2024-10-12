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
            setup_constraints();
            fts_fields_ = {

            };
            connect_->setup_full_text_search(table_name_, fts_fields_);
        }

        void update_all_fields(const objects::Medicament& record) override;
        void update_all_fields(const std::vector<objects::Medicament>& records) override;

    private:
        std::vector<std::shared_ptr<common::database::FieldBase>> medicament_fields_;
    };
}
